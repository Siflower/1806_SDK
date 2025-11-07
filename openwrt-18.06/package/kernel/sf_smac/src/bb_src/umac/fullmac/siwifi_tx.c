/**
 ******************************************************************************
 *
 * @file siwifi_tx.c
 *
 * Copyright (C) Siflower 2018-2025
 *
 ******************************************************************************
 */
#include <linux/dma-mapping.h>
#include <linux/etherdevice.h>
#include <net/sock.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#include "siwifi_defs.h"
#include "siwifi_utils.h"
#include "siwifi_tx.h"
#include "siwifi_msg_tx.h"
#include "siwifi_mesh.h"
#include "siwifi_events.h"
#include "siwifi_compat.h"
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
#include "siwifi_ioctl.h"
#endif
#ifdef NEW_SCHEDULE
#include "ipc_host.h"
#endif
#include "siwifi_traffic.h"
#ifdef CONFIG_VDR_HW
#include "hw_interface.h"
#endif
#ifdef CONFIG_SF19A28_WIFI_LED
#include "siwifi_led.h"
#endif
#include "siwifi_mem.h"
#include "siwifi_trace.h"
#ifdef CONFIG_SIWIFI_IGMP
#include "siwifi_igmp.h"
#endif
#ifdef CONFIG_SIWIFI_REPEATER
#include "siwifi_repeater.h"
#endif
#include "reg_access.h"

#define EDCA_AC_1_ADDR(band) (WIFI_BASE_ADDR(band) + 0x00080000 + 0x0204)

void txq_stat_handler(struct work_struct *wk)
{
    struct siwifi_hw *siwifi_hw = container_of(wk, struct siwifi_hw, txq_stat_work.work);
    struct siwifi_vif *vif = NULL;
    struct siwifi_sta *siwifi_sta = NULL;
    struct siwifi_txq *txq = NULL;
    int tid = 0;

    if(siwifi_hw->ate_env.ate_start) return;
    spin_lock_bh(&siwifi_hw->cb_lock);
    list_for_each_entry(vif, &siwifi_hw->vifs, list) {
        if (SIWIFI_VIF_TYPE(vif) ==  NL80211_IFTYPE_AP){
            list_for_each_entry(siwifi_sta, &vif->ap.sta_list, list) {
                foreach_sta_txq(siwifi_sta, txq, tid, siwifi_hw) {
                    if (txq->time_stat.inlmac_total == 0)
                        continue;
                    txq->last_timer_time_stat.inlmac_0ms = txq->time_stat.inlmac_0ms - txq->record_time_stat.inlmac_0ms;
                    txq->record_time_stat.inlmac_0ms = txq->time_stat.inlmac_0ms;
                    txq->last_timer_time_stat.inlmac_10ms = txq->time_stat.inlmac_10ms - txq->record_time_stat.inlmac_10ms;
                    txq->record_time_stat.inlmac_10ms = txq->time_stat.inlmac_10ms;
                    txq->last_timer_time_stat.inlmac_20ms = txq->time_stat.inlmac_20ms - txq->record_time_stat.inlmac_20ms;
                    txq->record_time_stat.inlmac_20ms = txq->time_stat.inlmac_20ms;
                    txq->last_timer_time_stat.inlmac_50ms = txq->time_stat.inlmac_50ms - txq->record_time_stat.inlmac_50ms;
                    txq->record_time_stat.inlmac_50ms = txq->time_stat.inlmac_50ms;
                    txq->last_timer_time_stat.inlmac_100ms = txq->time_stat.inlmac_100ms - txq->record_time_stat.inlmac_100ms;
                    txq->record_time_stat.inlmac_100ms = txq->time_stat.inlmac_100ms;
                    txq->last_timer_time_stat.inlmac_total = txq->time_stat.inlmac_total - txq->record_time_stat.inlmac_total;
                    txq->record_time_stat.inlmac_total = txq->time_stat.inlmac_total;
                    txq->last_timer_time_stat.inlmac_retry = txq->time_stat.inlmac_retry - txq->record_time_stat.inlmac_retry;
                    txq->record_time_stat.inlmac_retry = txq->time_stat.inlmac_retry;
                    txq->last_timer_time_stat.come_xmit = txq->time_stat.come_xmit - txq->record_time_stat.come_xmit;
                    txq->record_time_stat.come_xmit = txq->time_stat.come_xmit;
                    txq->last_timer_time_stat.free_xmit = txq->time_stat.free_xmit - txq->record_time_stat.free_xmit;
                    txq->record_time_stat.free_xmit = txq->time_stat.free_xmit;

                    if (txq->atf.record_rateinfo == 0 || txq->atf.enable == 0)
                        continue;
                    txq->atf.debug_get_token_cnt_last = txq->atf.debug_get_token_cnt - txq->atf.debug_get_token_cnt_record;
                    txq->atf.debug_get_token_cnt_record = txq->atf.debug_get_token_cnt;
                    txq->atf.debug_skip_token_cnt_last = txq->atf.debug_skip_token_cnt - txq->atf.debug_skip_token_cnt_record;
                    txq->atf.debug_skip_token_cnt_record = txq->atf.debug_skip_token_cnt;
                }
            }
        }
    }
    schedule_delayed_work(&siwifi_hw->txq_stat_work, MSECS(SIWIFI_TXQ_STAT_TIME_MS));
    spin_unlock_bh(&siwifi_hw->cb_lock);
    return;
}

/******************************************************************************
 * Power Save functions
 *****************************************************************************/
/**
 * siwifi_set_traffic_status - Inform FW if traffic is available for STA in PS
 *
 * @siwifi_hw: Driver main data
 * @sta: Sta in PS mode
 * @available: whether traffic is buffered for the STA
 * @ps_id: type of PS data requested (@LEGACY_PS_ID or @UAPSD_ID)
  */
void siwifi_set_traffic_status(struct siwifi_hw *siwifi_hw,
                             struct siwifi_sta *sta,
                             bool available,
                             u8 ps_id)
{
    if (sta->tdls.active) {
#if DEBUG_ARRAY_CHECK
        BUG_ON(sta->vif_idx >= NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX);
#endif
        if (siwifi_hw->vif_table[sta->vif_idx])
            siwifi_send_tdls_peer_traffic_ind_req(siwifi_hw,
                            siwifi_hw->vif_table[sta->vif_idx]);
    } else {
        bool uapsd = (ps_id != LEGACY_PS_ID);
        siwifi_send_me_traffic_ind(siwifi_hw, sta->sta_idx, uapsd, available);
        trace_ps_traffic_update(sta->sta_idx, available, uapsd);
    }
}

/**
 * siwifi_ps_bh_enable - Enable/disable PS mode for one STA
 *
 * @siwifi_hw: Driver main data
 * @sta: Sta which enters/leaves PS mode
 * @enable: PS mode status
 *
 * This function will enable/disable PS mode for one STA.
 * When enabling PS mode:
 *  - Stop all STA's txq for SIWIFI_TXQ_STOP_STA_PS reason
 *  - Count how many buffers are already ready for this STA
 *  - For BC/MC sta, update all queued SKB to use hw_queue BCMC
 *  - Update TIM if some packet are ready
 *
 * When disabling PS mode:
 *  - Start all STA's txq for SIWIFI_TXQ_STOP_STA_PS reason
 *  - For BC/MC sta, update all queued SKB to use hw_queue AC_BE
 *  - Update TIM if some packet are ready (otherwise fw will not update TIM
 *    in beacon for this STA)
 *
 * All counter/skb updates are protected from TX path by taking tx_lock
 *
 * NOTE: _bh_ in function name indicates that this function is called
 * from a bottom_half tasklet.
 */
void siwifi_ps_bh_enable(struct siwifi_hw *siwifi_hw, struct siwifi_sta *sta,
                       uint8_t ps_state)
{
    struct siwifi_txq *txq;
    u16 ps_pkg_ready;
    u16 uapsd_pkg_ready;
    bool enable = (ps_state == MM_PS_MODE_ON || ps_state == MM_PS_MODE_ON_DYN);
    u16 sta_pushed[2] = {0};

#ifdef CONFIG_VDR_HW
    vendor_hook_sta_ps(sta, enable);
#endif
    if (enable) {
        trace_ps_enable(sta);

        spin_lock_bh(&siwifi_hw->tx_lock);
        sta->ps.active = true;
        sta->ps.sp_cnt[LEGACY_PS_ID] = 0;
        sta->ps.sp_cnt[UAPSD_ID] = 0;
        siwifi_txq_sta_stop(sta, SIWIFI_TXQ_STOP_STA_PS, siwifi_hw);

        if (is_multicast_sta(sta->sta_idx)) {
            txq = siwifi_txq_sta_get(sta, 0, siwifi_hw);
            if (txq->status & SIWIFI_TXQ_NDEV_FLOW_CTRL) {
                printk("vif BCMC txq %d, try to reduce ps skbs to advoid ps station block ndevq\n", txq->idx);
                siwifi_txq_ps_drop_skb(siwifi_hw, txq);
            }

            sta->ps.pkt_ready[LEGACY_PS_ID] = skb_queue_len(&txq->sk_list);
#ifdef CONFIG_BRIDGE_ACCELERATE
            sta->ps.pkt_ready[LEGACY_PS_ID] += skb_queue_len(&txq->accel_sk_list);
#endif
            sta->ps.pkt_ready[UAPSD_ID] = 0;
            txq->hwq = &siwifi_hw->hwq[SIWIFI_HWQ_VI];
            txq->ps_active_change++;
        } else {
            int i;
            sta->ps.pkt_ready[LEGACY_PS_ID] = 0;
            sta->ps.pkt_ready[UAPSD_ID] = 0;
            foreach_sta_txq(sta, txq, i, siwifi_hw) {
                if (txq->status & SIWIFI_TXQ_NDEV_FLOW_CTRL) {
                    printk("%pM, txq %d, try to reduce ps skbs to advoid ps station block ndevq\n",
                            sta->mac_addr, txq->idx);
                    siwifi_txq_ps_drop_skb(siwifi_hw, txq);
                }
#if DEBUG_ARRAY_CHECK
                BUG_ON(txq->ps_id > 1);
#endif
                sta->ps.pkt_ready[txq->ps_id] += skb_queue_len(&txq->sk_list);
#ifdef CONFIG_BRIDGE_ACCELERATE
                sta->ps.pkt_ready[txq->ps_id] += skb_queue_len(&txq->accel_sk_list);
#endif
                sta_pushed[txq->ps_id] += txq->pkt_pushed[SIWIFI_TXQ_POS_ID(txq)];
                txq->ps_active_change++;
            }
        }
        ps_pkg_ready = sta->ps.pkt_ready[LEGACY_PS_ID];
        uapsd_pkg_ready = sta->ps.pkt_ready[UAPSD_ID];
        spin_unlock_bh(&siwifi_hw->tx_lock);

        if (ps_pkg_ready)
            siwifi_set_traffic_status(siwifi_hw, sta, true, LEGACY_PS_ID);

        if (uapsd_pkg_ready)
            siwifi_set_traffic_status(siwifi_hw, sta, true, UAPSD_ID);
    } else {
        trace_ps_disable(sta->sta_idx);

        spin_lock_bh(&siwifi_hw->tx_lock);
        sta->ps.active = false;

        if (is_multicast_sta(sta->sta_idx)) {
            txq = siwifi_txq_sta_get(sta, 0, siwifi_hw);
            // RM#9571 If we switch hwq, we need to make sure it's not associated with wrong hwq schedule list
            // The txq could be added into other hwq schedule list then ps traffic request happen
            siwifi_txq_del_from_hw_list(txq);

            txq->hwq = &siwifi_hw->hwq[SIWIFI_HWQ_BE];
            txq->push_limit = 0;

            // Flush all skbs when ps state enter into force power saving
            // For multicast sta, this condition should not
            if (ps_state == MM_PS_MODE_OFF_FORCE) {
                txq->ps_off_drop += skb_queue_len(&txq->sk_list);
#ifdef CONFIG_BRIDGE_ACCELERATE
                txq->ps_off_drop += skb_queue_len(&txq->accel_sk_list);
#endif
                printk("[%d]multicast txq : force ps drop len : %d\n", siwifi_hw->mod_params->is_hb, txq->ps_off_drop);
                siwifi_txq_flush(siwifi_hw, txq);
                siwifi_txq_del_from_stuck_check_list(txq);
            }
        } else {
            int i;
            u32 force_ps_drop = 0;
            foreach_sta_txq(sta, txq, i, siwifi_hw) {
                txq->push_limit = 0;
                // Flush all skbs when ps state enter into force power saving
                if (ps_state == MM_PS_MODE_OFF_FORCE) {
                    txq->ps_off_drop += skb_queue_len(&txq->sk_list);
#ifdef CONFIG_BRIDGE_ACCELERATE
                    txq->ps_off_drop += skb_queue_len(&txq->accel_sk_list);
#endif
                    force_ps_drop += txq->ps_off_drop;
                    siwifi_txq_flush(siwifi_hw, txq);
                    siwifi_txq_del_from_stuck_check_list(txq);
                }
            }
            if (force_ps_drop)
                printk("[%d]Total force ps drop len : %d\n", siwifi_hw->mod_params->is_hb, force_ps_drop);
        }

        siwifi_txq_sta_start(sta, SIWIFI_TXQ_STOP_STA_PS, siwifi_hw);

        ps_pkg_ready = sta->ps.pkt_ready[LEGACY_PS_ID];
        uapsd_pkg_ready = sta->ps.pkt_ready[UAPSD_ID];
        spin_unlock_bh(&siwifi_hw->tx_lock);

        if (ps_pkg_ready || sta_pushed[LEGACY_PS_ID])
            siwifi_set_traffic_status(siwifi_hw, sta, true, LEGACY_PS_ID);

        if (uapsd_pkg_ready || sta_pushed[UAPSD_ID])
            siwifi_set_traffic_status(siwifi_hw, sta, true, UAPSD_ID);
    }
}

/**
 * siwifi_ps_bh_traffic_req - Handle traffic request for STA in PS mode
 *
 * @siwifi_hw: Driver main data
 * @sta: Sta which enters/leaves PS mode
 * @pkt_req: number of pkt to push
 * @ps_id: type of PS data requested (@LEGACY_PS_ID or @UAPSD_ID)
 *
 * This function will make sure that @pkt_req are pushed to fw
 * whereas the STA is in PS mode.
 * If request is 0, send all traffic
 * If request is greater than available pkt, reduce request
 * Note: request will also be reduce if txq credits are not available
 *
 * All counter updates are protected from TX path by taking tx_lock
 *
 * NOTE: _bh_ in function name indicates that this function is called
 * from the bottom_half tasklet.
 */
void siwifi_ps_bh_traffic_req(struct siwifi_hw *siwifi_hw, struct siwifi_sta *sta,
                            u16 pkt_req, u8 ps_id)
{
    int pkt_ready_all;
    u16 txq_len;
    struct siwifi_txq *txq;
    //RM#12193 add debug info when sta print is not in Power Save mode and workaround for it
    if(!sta->ps.active) {
#if 0
        struct siwifi_vif *siwifi_vif = NULL;
		struct siwifi_sta *sta2 = NULL;
        printk("sta %pM is not in Power Save mode(%d) sp_cnt_le %d sp_cnt_upsd %d valid=%d\n",
                sta->mac_addr, siwifi_hw->mod_params->is_hb, sta->ps.sp_cnt[LEGACY_PS_ID], sta->ps.sp_cnt[UAPSD_ID], sta->valid);
        printk("pkt_req=%d ps_id=%d\n", pkt_req, ps_id);
        //tranversal all station
        siwifi_vif = siwifi_hw->vif_table[sta->vif_idx];
        list_for_each_entry(sta2, &siwifi_vif->ap.sta_list, list) {
            printk("print sta %pM band(%d) sp_cnt_le %d sp_cnt_upsd %d valid=%d active=%d\n",
                sta2->mac_addr, siwifi_hw->mod_params->is_hb, sta2->ps.sp_cnt[LEGACY_PS_ID], sta2->ps.sp_cnt[UAPSD_ID], sta2->valid, sta2->ps.active);
        }
#endif
        siwifi_set_traffic_status(siwifi_hw, sta, false, LEGACY_PS_ID);
        return;
    }
    trace_ps_traffic_req(sta, pkt_req, ps_id);

    spin_lock(&siwifi_hw->tx_lock);

#if DEBUG_ARRAY_CHECK
    BUG_ON(ps_id > 1);
#endif

    pkt_ready_all = (sta->ps.pkt_ready[ps_id] - sta->ps.sp_cnt[ps_id]);

    /* Don't start SP until previous one is finished or we don't have
       packet ready (which must not happen for U-APSD) */
    if (sta->ps.sp_cnt[ps_id] || pkt_ready_all <= 0) {
        goto done;
    }

    /* Adapt request to what is available. */
    if (pkt_req == 0 || pkt_req > pkt_ready_all) {
        pkt_req = pkt_ready_all;
    }

    /* Reset the SP counter */
    sta->ps.sp_cnt[ps_id] = 0;

    /* "dispatch" the request between txq */
    if (is_multicast_sta(sta->sta_idx)) {
        txq = siwifi_txq_sta_get(sta, 0, siwifi_hw);
        if (txq->credits <= 0)
            goto done;
        // txq maybe flush in stuck list check
        txq_len = skb_queue_len(&txq->sk_list);
#ifdef CONFIG_BRIDGE_ACCELERATE
        txq_len += skb_queue_len(&txq->accel_sk_list);
#endif
        if (!txq_len)
            goto done;
        if (txq_len > txq->credits)
            txq_len = txq->credits;

        if (pkt_req > txq_len)
            pkt_req = txq_len;
        txq->push_limit = pkt_req;
        sta->ps.sp_cnt[ps_id] = pkt_req;
        siwifi_txq_add_to_hw_list(txq);
    } else {
        int i, tid;

        for (i = 0; i < NX_NB_TID_PER_STA ; i++) {
            tid = nx_tid_prio[i];
            txq = siwifi_txq_sta_get(sta, tid, siwifi_hw);
            if (txq->ps_id != ps_id)
                continue;
            txq_len = skb_queue_len(&txq->sk_list);
#ifdef CONFIG_BRIDGE_ACCELERATE
            txq_len += skb_queue_len(&txq->accel_sk_list);
#endif
            if (txq->credits <= 0 || txq_len == 0)
                continue;

            if (txq_len > txq->credits)
                txq_len = txq->credits;

            if (txq_len < pkt_req) {
                /* Not enough pkt queued in this txq, add this
                   txq to hwq list and process next txq */
                pkt_req -= txq_len;
                txq->push_limit = txq_len;
                sta->ps.sp_cnt[ps_id] += txq_len;
                siwifi_txq_add_to_hw_list(txq);
            } else {
                /* Enough pkt in this txq to comlete the request
                   add this txq to hwq list and stop processing txq */
                txq->push_limit = pkt_req;
                sta->ps.sp_cnt[ps_id] += pkt_req;
                siwifi_txq_add_to_hw_list(txq);
                break;
            }
        }
    }

done:
    spin_unlock(&siwifi_hw->tx_lock);
}

/******************************************************************************
 * TX functions
 *****************************************************************************/
#define PRIO_STA_NULL 0xAA

static const int siwifi_down_hwq2tid[3] = {
    [SIWIFI_HWQ_BK] = 2,
    [SIWIFI_HWQ_BE] = 3,
    [SIWIFI_HWQ_VI] = 5,
};

static void siwifi_downgrade_ac(struct siwifi_sta *sta, struct sk_buff *skb)
{
    int8_t ac = siwifi_tid2hwq[skb->priority];
#if DEBUG_ARRAY_CHECK
    BUG_ON(skb->priority >= IEEE80211_NUM_TIDS);
#endif

    if (WARN((ac > SIWIFI_HWQ_VO),
             "Unexepcted ac %d for skb before downgrade", ac))
        ac = SIWIFI_HWQ_VO;

    while (sta->acm & BIT(ac)) {
        if (ac == SIWIFI_HWQ_BK) {
            skb->priority = 1;
            return;
        }
        ac--;
#if DEBUG_ARRAY_CHECK
        BUG_ON(ac < 0 || ac > 2);
#endif
        skb->priority = siwifi_down_hwq2tid[ac];
    }
}


u16 siwifi_select_txq(struct siwifi_vif *siwifi_vif, struct sk_buff *skb)
{
    struct siwifi_hw *siwifi_hw = siwifi_vif->siwifi_hw;
    struct wireless_dev *wdev = &siwifi_vif->wdev;
    struct siwifi_sta *sta = NULL;
    struct siwifi_txq *txq;
    u16 netdev_queue;
    bool tdls_mgmgt_frame = false;

    switch (wdev->iftype) {
    case NL80211_IFTYPE_STATION:
    case NL80211_IFTYPE_P2P_CLIENT:
    {
        struct ethhdr *eth;
        eth = (struct ethhdr *)skb->data;
        if (eth->h_proto == cpu_to_be16(ETH_P_TDLS)) {
            tdls_mgmgt_frame = true;
        }
        if ((siwifi_vif->tdls_status == TDLS_LINK_ACTIVE) &&
            (siwifi_vif->sta.tdls_sta != NULL) &&
            (memcmp(eth->h_dest, siwifi_vif->sta.tdls_sta->mac_addr, ETH_ALEN) == 0))
            sta = siwifi_vif->sta.tdls_sta;
        else
            sta = siwifi_vif->sta.ap;
        break;
    }
    case NL80211_IFTYPE_AP_VLAN:
        if (siwifi_vif->ap_vlan.sta_4a) {
            sta = siwifi_vif->ap_vlan.sta_4a;
            break;
        }

        /* AP_VLAN interface is not used for a 4A STA,
           fallback searching sta amongs all AP's clients */
        siwifi_vif = siwifi_vif->ap_vlan.master;
    case NL80211_IFTYPE_AP:
    case NL80211_IFTYPE_P2P_GO:
    {
        struct ethhdr *eth = (struct ethhdr *)skb->data;

        if (is_multicast_ether_addr(eth->h_dest)) {
#if DEBUG_ARRAY_CHECK
            BUG_ON(siwifi_vif->ap.bcmc_index >= NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX);
#endif
            sta = &siwifi_hw->sta_table[siwifi_vif->ap.bcmc_index];
        } else {
			sta = siwifi_sta_hash_get(siwifi_vif, eth->h_dest);
        }

        break;
    }
    case NL80211_IFTYPE_MESH_POINT:
    {
        struct ethhdr *eth = (struct ethhdr *)skb->data;

        if (!siwifi_vif->is_resending) {
            /*
             * If ethernet source address is not the address of a mesh wireless interface, we are proxy for
             * this address and have to inform the HW
             */
            if (memcmp(&eth->h_source[0], &siwifi_vif->ndev->perm_addr[0], ETH_ALEN)) {
                /* Check if LMAC is already informed */
                if (!siwifi_get_mesh_proxy_info(siwifi_vif, (u8 *)&eth->h_source, true)) {
                    siwifi_send_mesh_proxy_add_req(siwifi_hw, siwifi_vif, (u8 *)&eth->h_source);
                }
            }
        }

        if (is_multicast_ether_addr(eth->h_dest)) {
#if DEBUG_ARRAY_CHECK
            BUG_ON(siwifi_vif->ap.bcmc_index >= NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX);
#endif
            sta = &siwifi_hw->sta_table[siwifi_vif->ap.bcmc_index];
        } else {
            /* Path to be used */
            struct siwifi_mesh_path *p_mesh_path = NULL;
            struct siwifi_mesh_path *p_cur_path;
            /* Check if destination is proxied by a peer Mesh STA */
            struct siwifi_mesh_proxy *p_mesh_proxy = siwifi_get_mesh_proxy_info(siwifi_vif, (u8 *)&eth->h_dest, false);
            /* Mesh Target address */
            struct mac_addr *p_tgt_mac_addr;

            if (p_mesh_proxy) {
                p_tgt_mac_addr = &p_mesh_proxy->proxy_addr;
            } else {
                p_tgt_mac_addr = (struct mac_addr *)&eth->h_dest;
            }

            /* Look for path with provided target address */
            list_for_each_entry(p_cur_path, &siwifi_vif->ap.mpath_list, list) {
                if (!memcmp(&p_cur_path->tgt_mac_addr, p_tgt_mac_addr, ETH_ALEN)) {
                    p_mesh_path = p_cur_path;
                    break;
                }
            }

            if (p_mesh_path) {
                sta = p_mesh_path->p_nhop_sta;
            } else {
                siwifi_send_mesh_path_create_req(siwifi_hw, siwifi_vif, (u8 *)p_tgt_mac_addr);
            }
        }

        break;
    }
    default:
        break;
    }

    if (sta && sta->qos)
    {
        if (tdls_mgmgt_frame) {
            skb_set_queue_mapping(skb, NX_STA_NDEV_IDX(skb->priority, sta->sta_idx));
        } else {
            /* use the data classifier to determine what 802.1d tag the
             * data frame has */
            skb->priority = cfg80211_classify8021d(skb, NULL) & IEEE80211_QOS_CTL_TAG1D_MASK;
        }
        if (sta->acm)
            siwifi_downgrade_ac(sta, skb);

        txq = siwifi_txq_sta_get(sta, skb->priority, siwifi_hw);
        netdev_queue = txq->ndev_idx;
    }
    else if (sta)
    {
        skb->priority = 0xFF;
        txq = siwifi_txq_sta_get(sta, 0, siwifi_hw);
        netdev_queue = txq->ndev_idx;
    }
    else
    {
        /* This packet will be dropped in xmit function, still need to select
           an active queue for xmit to be called. As it most likely to happen
           for AP interface, select BCMC queue
           (TODO: select another queue if BCMC queue is stopped) */
        skb->priority = PRIO_STA_NULL;
		netdev_queue = SIWIFI_HWQ_BE;
    }

    BUG_ON(netdev_queue >= siwifi_hw->tx_queue_num);

    return netdev_queue;
}

/**
 * siwifi_set_more_data_flag - Update MORE_DATA flag in tx sw desc
 *
 * @siwifi_hw: Driver main data
 * @sw_txhdr: Header for pkt to be pushed
 *
 * If STA is in PS mode
 *  - Set EOSP in case the packet is the last of the UAPSD service period
 *  - Set MORE_DATA flag if more pkt are ready for this sta
 *  - Update TIM if this is the last pkt buffered for this sta
 *
 * note: tx_lock already taken.
 */
static inline void siwifi_set_more_data_flag(struct siwifi_hw *siwifi_hw,
                                           struct siwifi_sw_txhdr *sw_txhdr)
{
    struct siwifi_sta *sta = sw_txhdr->siwifi_sta;
    struct siwifi_vif *vif = sw_txhdr->siwifi_vif;
    struct siwifi_txq *txq = sw_txhdr->txq;

    if (unlikely(sta->ps.active)) {
#if DEBUG_ARRAY_CHECK
        BUG_ON(txq->ps_id > 1);
#endif
        if (sta->ps.pkt_ready[txq->ps_id])
            sta->ps.pkt_ready[txq->ps_id]--;
        if (sta->ps.sp_cnt[txq->ps_id])
            sta->ps.sp_cnt[txq->ps_id]--;

        trace_ps_push(sta);

        if (((txq->ps_id == UAPSD_ID) || (vif->wdev.iftype == NL80211_IFTYPE_MESH_POINT) || (sta->tdls.active))
                && !sta->ps.sp_cnt[txq->ps_id]) {
            sw_txhdr->desc.host.flags |= TXU_CNTRL_EOSP;
        }

        if (sta->ps.pkt_ready[txq->ps_id]) {
            sw_txhdr->desc.host.flags |= TXU_CNTRL_MORE_DATA;
        }
    }
}

/**
 * siwifi_get_tx_info - Get STA and tid for one skb
 *
 * @siwifi_vif: vif ptr
 * @skb: skb
 * @tid: pointer updated with the tid to use for this skb
 *
 * @return: pointer on the destination STA (may be NULL)
 *
 * skb has already been parsed in siwifi_select_queue function
 * simply re-read information form skb.
 */
static struct siwifi_sta *siwifi_get_tx_info(struct siwifi_vif *siwifi_vif,
		struct sk_buff *skb,
		u8 *tid
#ifdef CONFIG_SIWIFI_IGMP
        , unsigned char *unicast_addr
#endif
        )
{
	struct siwifi_hw *siwifi_hw = siwifi_vif->siwifi_hw;
	struct siwifi_sta *sta = NULL;
	*tid = skb->priority;
	if (unlikely(skb->priority == PRIO_STA_NULL)) {
		return NULL;
	} else {
#if 0
	    int sta_idx;
		int ndev_idx = skb_get_queue_mapping(skb);

		if (ndev_idx == NX_BCMC_TXQ_NDEV_IDX)
			sta_idx = NX_REMOTE_STA_MAX + master_vif_idx(siwifi_vif);
		else
			sta_idx = ndev_idx / NX_NB_TID_PER_STA;

		sta = &siwifi_hw->sta_table[sta_idx];
#else
		struct wireless_dev *wdev = &siwifi_vif->wdev;
        // vif and sta list information should be protected
		switch (wdev->iftype) {
			case NL80211_IFTYPE_STATION:
			case NL80211_IFTYPE_P2P_CLIENT:
				{
					struct ethhdr *eth;
					eth = (struct ethhdr *)skb->data;

					if ((siwifi_vif->tdls_status == TDLS_LINK_ACTIVE) &&
							(siwifi_vif->sta.tdls_sta != NULL) &&
							(memcmp(eth->h_dest, siwifi_vif->sta.tdls_sta->mac_addr, ETH_ALEN) == 0))
						sta = siwifi_vif->sta.tdls_sta;
					else
						sta = siwifi_vif->sta.ap;

                    // RM#13042 use bk queue to send multicast frame for sta_vif
                    // siwifi_tid2hwq[1] = SIWIFI_HWQ_BK
                    if (is_multicast_ether_addr(eth->h_dest))
                        *tid = 1;
					break;
				}
			case NL80211_IFTYPE_AP_VLAN:
				if (siwifi_vif->ap_vlan.sta_4a) {
					sta = siwifi_vif->ap_vlan.sta_4a;
					break;
				}

				/* AP_VLAN interface is not used for a 4A STA,
				   fallback searching sta amongs all AP's clients */
				siwifi_vif = siwifi_vif->ap_vlan.master;
			case NL80211_IFTYPE_AP:
			case NL80211_IFTYPE_P2P_GO:
				{
					struct ethhdr *eth = (struct ethhdr *)skb->data;
                    unsigned char *eth_dest = eth->h_dest;
#ifdef CONFIG_SIWIFI_IGMP
                    if(unicast_addr) {
                        *tid = 0;
                        eth_dest = unicast_addr;
                    }
#endif
					if (is_multicast_ether_addr(eth->h_dest)
#ifdef CONFIG_SIWIFI_IGMP
                         && unicast_addr == NULL
#endif
                        ) {
#if DEBUG_ARRAY_CHECK
                        BUG_ON(siwifi_vif->ap.bcmc_index >= NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX);
#endif
						sta = &siwifi_hw->sta_table[siwifi_vif->ap.bcmc_index];
					} else {
						sta = siwifi_sta_hash_get(siwifi_vif, eth_dest);
					}

					break;
				}
			case NL80211_IFTYPE_MESH_POINT:
				{
					struct ethhdr *eth = (struct ethhdr *)skb->data;

					if (!siwifi_vif->is_resending) {
						/*
						 * If ethernet source address is not the address of a mesh wireless interface, we are proxy for
						 * this address and have to inform the HW
						 */
						if (memcmp(&eth->h_source[0], &siwifi_vif->ndev->perm_addr[0], ETH_ALEN)) {
							/* Check if LMAC is already informed */
							if (!siwifi_get_mesh_proxy_info(siwifi_vif, (u8 *)&eth->h_source, true)) {
								siwifi_send_mesh_proxy_add_req(siwifi_hw, siwifi_vif, (u8 *)&eth->h_source);
							}
						}
					}

					if (is_multicast_ether_addr(eth->h_dest)) {
#if DEBUG_ARRAY_CHECK
                        BUG_ON(siwifi_vif->ap.bcmc_index >= NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX);
#endif
						sta = &siwifi_hw->sta_table[siwifi_vif->ap.bcmc_index];
					} else {
						/* Path to be used */
						struct siwifi_mesh_path *p_mesh_path = NULL;
						struct siwifi_mesh_path *p_cur_path;
						/* Check if destination is proxied by a peer Mesh STA */
						struct siwifi_mesh_proxy *p_mesh_proxy = siwifi_get_mesh_proxy_info(siwifi_vif, (u8 *)&eth->h_dest, false);
						/* Mesh Target address */
						struct mac_addr *p_tgt_mac_addr;

						if (p_mesh_proxy) {
							p_tgt_mac_addr = &p_mesh_proxy->proxy_addr;
						} else {
							p_tgt_mac_addr = (struct mac_addr *)&eth->h_dest;
						}

						/* Look for path with provided target address */
						list_for_each_entry(p_cur_path, &siwifi_vif->ap.mpath_list, list) {
							if (!memcmp(&p_cur_path->tgt_mac_addr, p_tgt_mac_addr, ETH_ALEN)) {
								p_mesh_path = p_cur_path;
								break;
							}
						}

						if (p_mesh_path) {
							sta = p_mesh_path->p_nhop_sta;
						} else {
                            spin_unlock_bh(&siwifi_hw->cb_lock);
							siwifi_send_mesh_path_create_req(siwifi_hw, siwifi_vif, (u8 *)p_tgt_mac_addr);
                            spin_lock_bh(&siwifi_hw->cb_lock);
						}
					}

					break;
				}
			default:
				break;
		}
#endif
	}

	return sta;
}

/**
 * siwifi_prep_tx - Prepare buffer for DMA transmission
 *
 * @siwifi_hw: Driver main data
 * @txhdr: Tx descriptor
 *
 * Maps hw_txhdr and buffer data for transmission via DMA.
 * - Data buffer with be downloaded by embebded side.
 * - hw_txhdr will be uploaded by embedded side when buffer has been
 *   transmitted over the air.
 */
static int siwifi_prep_tx(struct siwifi_hw *siwifi_hw, struct siwifi_txhdr *txhdr)
{
    struct siwifi_sw_txhdr *sw_txhdr = txhdr->sw_hdr;
    struct siwifi_hw_txhdr *hw_txhdr = &txhdr->hw_hdr;
    dma_addr_t dma_addr;

    /* MAP (and sync) memory for DMA */
    dma_addr = dma_map_single(siwifi_hw->dev, hw_txhdr,
                              sw_txhdr->map_len, DMA_BIDIRECTIONAL);
    if (WARN_ON(dma_mapping_error(siwifi_hw->dev, dma_addr)))
        return -1;

    sw_txhdr->dma_addr = dma_addr;

    return 0;
}


#ifdef NEW_SCHEDULE
static void siwifi_store_host_id(void *target_host_id, void *src_host_id)
{
    struct sk_buff_head *target = (struct sk_buff_head *)target_host_id;
    struct sk_buff_head *src = (struct sk_buff_head *)src_host_id;

    skb_queue_splice(src, target);
}

/**
 *  siwifi_tx_push_burst - Push packets to fw
 *
 * @siwifi_hw: Driver main data
 * @txhdr: tx desc of the buffer to push
 * @flags: push flags (see @siwifi_push_flags)
 *
 * Push one packet to fw. Sw desc of the packet has already been updated.
 * Only MORE_DATA flag will be set if needed.
 */
int siwifi_tx_push_burst(struct siwifi_hw *siwifi_hw, struct siwifi_hwq *hwq, struct siwifi_txq *txq, struct sk_buff_head *sk_list)
{
    struct sk_buff *skb;
    struct siwifi_txhdr *txhdr;
    struct siwifi_sw_txhdr *sw_txhdr;
    volatile struct txdesc_host *txdesc_host;
    int flags;
    void *dest_desc_non_cache;
    u16 hw_queue = hwq->id;
    int skb_index = 0;
    int user = 0;

    lockdep_assert_held(&siwifi_hw->tx_lock);

    txdesc_host = (volatile struct txdesc_host *)ipc_host_txdesc_get(siwifi_hw->ipc_env, hw_queue, user);
    BUG_ON(!txdesc_host);

    dest_desc_non_cache = (void *)CKSEG1ADDR(txdesc_host->phy_addr);

    skb_queue_walk(sk_list, skb) {
        flags = 0;
        txhdr = (struct siwifi_txhdr *)skb->data;
        sw_txhdr = txhdr->sw_hdr;
        BUG_ON(skb != sw_txhdr->skb);

        /* RETRY flag is not always set so retest here */
        if (txq->nb_retry) {
            flags |= SIWIFI_PUSH_RETRY;
            txq->nb_retry--;
            if (txq->nb_retry == 0) {
                WARN(skb != txq->last_retry_skb,
                     "last retry buffer is not the expected one");
                txq->last_retry_skb = NULL;
            }
        } else if (!(flags & SIWIFI_PUSH_RETRY)) {
            txq->pkt_sent++;
        }

#ifdef CONFIG_SIWIFI_AMSDUS_TX
        if (txq->amsdu == sw_txhdr) {
            WARN((flags & SIWIFI_PUSH_RETRY), "End A-MSDU on a retry");
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
            siwifi_hw->stats.amsdus[sw_txhdr->amsdu.nb - 1].done++;
            if (sw_txhdr->siwifi_sta)
                sw_txhdr->siwifi_sta->stats.amsdus[sw_txhdr->amsdu.nb - 1].done++;
#endif
            txq->amsdu = NULL;
        }
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        else if (!(flags & SIWIFI_PUSH_RETRY) &&
            !(sw_txhdr->desc.host.flags & TXU_CNTRL_AMSDU)) {
            siwifi_hw->stats.amsdus[0].done++;
            if (sw_txhdr->siwifi_sta)
                sw_txhdr->siwifi_sta->stats.amsdus[0].done++;
        }
#endif
#endif /* CONFIG_SIWIFI_AMSDUS_TX */

        /* Wait here to update hw_queue, as for multicast STA hwq may change
           between queue and push (because of PS) */
        sw_txhdr->hw_queue = hw_queue;

#ifdef CONFIG_SIWIFI_MUMIMO_TX
        /* MU group is only selected during hwq processing */
        sw_txhdr->desc.host.mumimo_info = txq->mumimo_info;
#endif /* CONFIG_SIWIFI_MUMIMO_TX */

        if (sw_txhdr->siwifi_sta) {
            /* only for AP mode */
            siwifi_set_more_data_flag(siwifi_hw, sw_txhdr);
        }
        //copy the txdesc to hwq description
        memcpy(dest_desc_non_cache + skb_index * sizeof(struct txdesc_api), &sw_txhdr->desc, sizeof(struct txdesc_api));
        skb_index++;
    }
    txdesc_host->length = skb_index;

#ifdef CONFIG_SIWIFI_MUMIMO_TX
    user = SIWIFI_TXQ_POS_ID(txq);
#endif /* CONFIG_SIWIFI_MUMIMO_TX */

    txq->credits -= skb_index;
#if DEBUG_ARRAY_CHECK
    BUG_ON(user >= CONFIG_USER_MAX);
#endif
    txq->pkt_pushed[user] += skb_index;
    if (txq->credits <= 0)
        siwifi_txq_stop(txq, SIWIFI_TXQ_STOP_FULL, SIWIFI_TXQ_STOP_POS_PUSH_FULL);
    ipc_host_txdesc_push_burst(siwifi_hw->ipc_env, hw_queue, user, &siwifi_store_host_id, sk_list);
    if (txq->push_limit > skb_index)
        txq->push_limit -= skb_index;
    else
        txq->push_limit = 0;

#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
#if DEBUG_ARRAY_CHECK
    BUG_on(hw_queue >= NX_TXQ_CNT);
#endif
    siwifi_hw->stats.cfm_balance[hw_queue] += skb_index;
#endif
    return 0;
}
#endif

static void siwifi_static_timer_cb(struct timer_list *t)
{
    int i = 0;
    struct siwifi_hw *siwifi_hw = from_timer(siwifi_hw, t, siwifi_static_timer);
    int timer_cur = siwifi_hw->siwifi_static_timer_cur;

    if (siwifi_hw->siwifi_static_timer_stop)
        return;

    for (i = 0; i < SIWIFI_STATIC_INFO_MAX; i++) {
        struct siwifi_sta *sta = &siwifi_hw->sta_table[i];
        struct siwifi_txq *txq_t = NULL;
        int tid_t = 0;

        memcpy(&siwifi_hw->siwifi_static_timeinfo[timer_cur][i], &siwifi_hw->siwifi_static_txinfo[i], sizeof(struct siwifi_static_txinfo));

        foreach_sta_txq(sta, txq_t, tid_t, siwifi_hw) {
            siwifi_hw->siwifi_static_timeinfo[timer_cur][i].siwifi_static_ready[tid_t]         = skb_queue_len(&txq_t->sk_list);
            siwifi_hw->siwifi_static_timeinfo[timer_cur][i].siwifi_static_ps_change[tid_t]     = txq_t->ps_active_change;
            siwifi_hw->siwifi_static_timeinfo[timer_cur][i].siwifi_static_txq_start[tid_t]     = txq_t->siwifi_static_start;
            siwifi_hw->siwifi_static_timeinfo[timer_cur][i].siwifi_static_txq_stop[tid_t]      = txq_t->siwifi_static_stop;
            siwifi_hw->siwifi_static_timeinfo[timer_cur][i].siwifi_static_txq_process[tid_t]   = txq_t->siwifi_static_process;
            siwifi_hw->siwifi_static_timeinfo[timer_cur][i].siwifi_static_pro_in_hwq[tid_t]    = txq_t->siwifi_static_pro_in_hwq;
            siwifi_hw->siwifi_static_timeinfo[timer_cur][i].siwifi_static_pro_reorder[tid_t]   = txq_t->siwifi_static_pro_reorder;
            siwifi_hw->siwifi_static_timeinfo[timer_cur][i].siwifi_static_not_ready[tid_t]     = txq_t->siwifi_static_not_ready;
            txq_t->ps_active_change               = 0;
            txq_t->siwifi_static_start            = 0;
            txq_t->siwifi_static_stop             = 0;
            txq_t->siwifi_static_process          = 0;
            txq_t->siwifi_static_pro_in_hwq       = 0;
            txq_t->siwifi_static_pro_reorder      = 0;
        }

        siwifi_hw->siwifi_static_timeinfo[timer_cur][i].siwifi_static_hwq_process = siwifi_hw->siwifi_static_hwq_process;
        memcpy(siwifi_hw->siwifi_static_timeinfo[timer_cur][i].siwifi_static_hwq_push, siwifi_hw->siwifi_static_hwq_push, NX_TXQ_CNT * sizeof(uint16_t));
        memcpy(siwifi_hw->siwifi_static_timeinfo[timer_cur][i].siwifi_static_break_reason, siwifi_hw->siwifi_static_break_reason, 3 * sizeof(uint16_t));

        siwifi_hw->siwifi_static_timeinfo[timer_cur][i].siwifi_static_ps_state = sta->ps.active;
        siwifi_hw->siwifi_static_timeinfo[timer_cur][i].siwifi_static_use = 1;

        memset(&siwifi_hw->siwifi_static_txinfo[i], 0, sizeof(struct siwifi_static_txinfo));
    }

    siwifi_hw->siwifi_static_timer_cur   = (timer_cur + 1) % SIWIFI_STATIC_TIME_MAX;
    siwifi_hw->siwifi_static_hwq_process = 0;
    memset(siwifi_hw->siwifi_static_hwq_push, 0, NX_TXQ_CNT * sizeof(uint16_t));
    memset(siwifi_hw->siwifi_static_break_reason, 0, SIWIFI_STATIC_BREAK_REASON_MAX * sizeof(uint16_t));

    if (!siwifi_hw->siwifi_static_timer_stop)
        mod_timer(&siwifi_hw->siwifi_static_timer, jiffies + SIWIFI_STATIC_INTERVAL);
}

void siwifi_static_timer_clear(struct siwifi_hw *siwifi_hw)
{
    int i = 0;
    int j = 0;

    siwifi_hw->siwifi_static_timer_stop = 1;
    printk("clear timer info timer_stop %d \n", siwifi_hw->siwifi_static_timer_stop);

    for (i = 0; i < SIWIFI_STATIC_INFO_MAX; i++) {
        memset(&siwifi_hw->siwifi_static_txinfo[i], 0, sizeof(struct siwifi_static_txinfo));
    }
    for (j = 0; j < SIWIFI_STATIC_TIME_MAX; j++) {
        for (i = 0; i < SIWIFI_STATIC_INFO_MAX; i++) {
            memset(&siwifi_hw->siwifi_static_timeinfo[j][i], 0, sizeof(struct siwifi_static_txinfo));
        }
    }
    siwifi_hw->siwifi_static_timer_cur = 0;
    return;
}

void siwifi_static_timer_print(struct siwifi_hw *siwifi_hw, int sta_idx)
{
    int i = 0, j = 0;

    struct siwifi_sta *sta = &siwifi_hw->sta_table[sta_idx];

    printk("print timer info sta[%d] %pM aid %d \n", sta_idx, sta->mac_addr, sta->aid);
    printk("[V]time  xmit  push  cfm  ack  ret [att stt]  rx  intxq psdr tidr ps  ready ps_chan start  stop proce  hwq_p  hw1p  inhwq reord notrd reason  cred token.\n");
//  printk("[-]  --  ----  ---- ---- ---- ---- [       ] ---   ---- ---- ----  - [----] [-----] [----  ----  ----][----- -----][----- ----- -----][----- ----- -----] \n"
// printk("[%d]  %2d  %4d  %4d %4d %4d %4d [%3d %3d] %3d   %4d %4d %4d  %d [%4d] [%5d] [%4d  %4d  %4d][%5d %5d][%5d %5d %5d][%5d %5d %5d]\n",

    for (j = 0; j < SIWIFI_STATIC_TIME_MAX; j++) {
        for (i = 1; i < NX_NB_TXQ_PER_STA; i++) {
            siwifi_hw->siwifi_static_timeinfo[j][sta_idx].siwifi_static_ready[0]       += siwifi_hw->siwifi_static_timeinfo[j][sta_idx].siwifi_static_ready[i];
            siwifi_hw->siwifi_static_timeinfo[j][sta_idx].siwifi_static_ps_change[0]   += siwifi_hw->siwifi_static_timeinfo[j][sta_idx].siwifi_static_ps_change[i];
            siwifi_hw->siwifi_static_timeinfo[j][sta_idx].siwifi_static_txq_start[0]   += siwifi_hw->siwifi_static_timeinfo[j][sta_idx].siwifi_static_txq_start[i];
            siwifi_hw->siwifi_static_timeinfo[j][sta_idx].siwifi_static_txq_stop[0]    += siwifi_hw->siwifi_static_timeinfo[j][sta_idx].siwifi_static_txq_stop[i];
            siwifi_hw->siwifi_static_timeinfo[j][sta_idx].siwifi_static_txq_process[0] += siwifi_hw->siwifi_static_timeinfo[j][sta_idx].siwifi_static_txq_process[i];
            siwifi_hw->siwifi_static_timeinfo[j][sta_idx].siwifi_static_pro_in_hwq[0]  += siwifi_hw->siwifi_static_timeinfo[j][sta_idx].siwifi_static_pro_in_hwq[i];
            siwifi_hw->siwifi_static_timeinfo[j][sta_idx].siwifi_static_pro_reorder[0] += siwifi_hw->siwifi_static_timeinfo[j][sta_idx].siwifi_static_pro_reorder[i];
            siwifi_hw->siwifi_static_timeinfo[j][sta_idx].siwifi_static_not_ready[0]   += siwifi_hw->siwifi_static_timeinfo[j][sta_idx].siwifi_static_not_ready[i];
        }
    }

    for (i = siwifi_hw->siwifi_static_timer_cur, j = 0; j < SIWIFI_STATIC_TIME_MAX; j++, i = (i + 1) % SIWIFI_STATIC_TIME_MAX) {
        printk("[%d]  %2d  %4d  %4d %4d %4d %4d [%3d %3d] %3d   %4d %4d %4d  %d [%4d] [%5d] [%4d  %4d  %4d][%5d %5d][%5d %5d %5d][%5d %5d %5d]\n",
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_use,
                j,
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_txxmit,
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_txpush,
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_txcfm,
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_cfm_ack,
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_cfm_ret,
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_a_tt,
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_s_tt,
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_rx,
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_intxq,
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_ps_drop,
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_drop_skb,
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_ps_state,
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_ready[0],
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_ps_change[0],
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_txq_start[0],
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_txq_stop[0],
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_txq_process[0],
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_hwq_process,
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_hwq_push[1],
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_pro_in_hwq[0],
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_pro_reorder[0],
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_not_ready[0],
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_break_reason[0],
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_break_reason[1],
                siwifi_hw->siwifi_static_timeinfo[i][sta_idx].siwifi_static_break_reason[2]
                );
    }

    printk("[V]time  xmit  push  cfm  ack  ret [att stt]  rx  intxq psdr tidr ps  ready ps_chan start  stop proce  hwq_p  hw1p  inhwq reord notrd reason  cred token.\n");
    return;
}

void siwifi_static_timer_start(struct siwifi_hw *siwifi_hw)
{
    siwifi_hw->siwifi_static_timer_stop = 0;
    printk("start timer info timer_stop %d \n", siwifi_hw->siwifi_static_timer_stop);
    timer_setup(&siwifi_hw->siwifi_static_timer, siwifi_static_timer_cb, 0);
    mod_timer(&siwifi_hw->siwifi_static_timer, jiffies + SIWIFI_STATIC_INTERVAL);
}


#ifndef NEW_SCHEDULE
/**
 *  siwifi_tx_push - Push one packet to fw
 *
 * @siwifi_hw: Driver main data
 * @txhdr: tx desc of the buffer to push
 * @flags: push flags (see @siwifi_push_flags)
 *
 * Push one packet to fw. Sw desc of the packet has already been updated.
 * Only MORE_DATA flag will be set if needed.
 */
int siwifi_tx_push(struct siwifi_hw *siwifi_hw, struct siwifi_txhdr *txhdr, int flags)
{
    struct siwifi_sw_txhdr *sw_txhdr = txhdr->sw_hdr;
    struct sk_buff *skb = sw_txhdr->skb;
    struct siwifi_txq *txq = sw_txhdr->txq;
    u16 hw_queue = txq->hwq->id;
    int user = 0;
    struct siwifi_vif *siwifi_vif = sw_txhdr->siwifi_vif;
    u16 be_wmm_param;

#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
	if (siwifi_hw->disable_wmm_edca) {
        be_wmm_param = readl((void*)EDCA_AC_1_ADDR(siwifi_hw->mod_params->is_hb));
    }
    else {
#else
    {
#endif
        if((siwifi_vif->be_wmm_param == EDCA_BE_AGGRESSIVE) || (siwifi_vif->be_wmm_param == EDCA_BE_DEFAULT))
            be_wmm_param = siwifi_vif->be_wmm_param;
        else
            be_wmm_param = EDCA_BE_DEFAULT;
    }

    lockdep_assert_held(&siwifi_hw->tx_lock);

    if (sw_txhdr->siwifi_sta)
        siwifi_hw->siwifi_static_txinfo[sw_txhdr->siwifi_sta->sta_idx].siwifi_static_txpush++;

    siwifi_trace_tx_push(siwifi_hw, txq->sta, skb);
    /* RETRY flag is not always set so retest here */
    if (txq->nb_retry) {
        flags |= SIWIFI_PUSH_RETRY;
        txq->nb_retry--;
        if (txq->nb_retry == 0) {
            WARN(skb != txq->last_retry_skb,
                 "last retry buffer is not the expected one");
            txq->last_retry_skb = NULL;
        }
    } else if (!(flags & SIWIFI_PUSH_RETRY)) {
        txq->pkt_sent++;
    }

#ifdef CONFIG_SIWIFI_AMSDUS_TX
    if (txq->amsdu == sw_txhdr) {
        WARN((flags & SIWIFI_PUSH_RETRY), "End A-MSDU on a retry");
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        siwifi_hw->stats.amsdus[sw_txhdr->amsdu.nb - 1].done++;
        if (sw_txhdr->siwifi_sta)
            sw_txhdr->siwifi_sta->stats.amsdus[sw_txhdr->amsdu.nb - 1].done++;
#endif
        txq->amsdu = NULL;
    }
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    else if (!(flags & SIWIFI_PUSH_RETRY) &&
               !(sw_txhdr->desc.host.flags & TXU_CNTRL_AMSDU)) {
        siwifi_hw->stats.amsdus[0].done++;
        if (sw_txhdr->siwifi_sta)
            sw_txhdr->siwifi_sta->stats.amsdus[0].done++;
    }
#endif
#endif /* CONFIG_SIWIFI_AMSDUS_TX */

    /* Wait here to update hw_queue, as for multicast STA hwq may change
       between queue and push (because of PS) */
    sw_txhdr->hw_queue = hw_queue;

#ifdef CONFIG_SIWIFI_MUMIMO_TX
    /* MU group is only selected during hwq processing */
    sw_txhdr->desc.host.mumimo_info = txq->mumimo_info;
    user = SIWIFI_TXQ_POS_ID(txq);
#endif /* CONFIG_SIWIFI_MUMIMO_TX */

    if (sw_txhdr->siwifi_sta) {
        /* only for AP mode */
        siwifi_set_more_data_flag(siwifi_hw, sw_txhdr);
    }

    trace_push_desc(skb, sw_txhdr, flags);
    txq->credits--;
#if DEBUG_ARRAY_CHECK
    BUG_ON(user > CONFIG_USER_MAX - 1);
#endif
    txq->pkt_pushed[user]++;
    if (txq->credits <= 0)
        siwifi_txq_stop(txq, SIWIFI_TXQ_STOP_FULL, SIWIFI_TXQ_STOP_POS_PUSH_FULL);

    if (txq->push_limit)
        txq->push_limit--;

    siwifi_ipc_txdesc_push(siwifi_hw, &sw_txhdr->desc, skb, hw_queue, user, be_wmm_param);
    sw_txhdr->push_to_lmac_time = ktime_get_ns();
    txq->hwq->credits[user]--;
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    siwifi_hw->stats.cfm_balance[hw_queue]++;
#endif
    siwifi_hw->siwifi_static_hwq_push[hw_queue]++;
    return 0;
}
#endif



/**
 * siwifi_tx_retry - Push an AMPDU pkt that need to be retried
 *
 * @siwifi_hw: Driver main data
 * @skb: pkt to re-push
 * @txhdr: tx desc of the pkt to re-push
 * @sw_retry: Indicates if fw decide to retry this buffer
 *            (i.e. it has never been transmitted over the air)
 *
 * Called when a packet needs to be repushed to the firmware.
 * First update sw descriptor and then queue it in the retry list.
 */
static void siwifi_tx_retry(struct siwifi_hw *siwifi_hw, struct sk_buff *skb,
                           struct siwifi_txhdr *txhdr, bool sw_retry, s16 credits)
{
    struct siwifi_sw_txhdr *sw_txhdr = txhdr->sw_hdr;
    struct tx_cfm_tag *cfm = &txhdr->hw_hdr.cfm;
    struct siwifi_txq *txq = sw_txhdr->txq;
    int peek_off = offsetof(struct siwifi_hw_txhdr, cfm);
    int peek_len = sizeof(((struct siwifi_hw_txhdr *)0)->cfm);

    if (!sw_retry) {
        /* update sw desc */
        sw_txhdr->desc.host.sn = cfm->sn;
        sw_txhdr->desc.host.pn[0] = cfm->pn[0];
        sw_txhdr->desc.host.pn[1] = cfm->pn[1];
        sw_txhdr->desc.host.pn[2] = cfm->pn[2];
        sw_txhdr->desc.host.pn[3] = cfm->pn[3];
        sw_txhdr->desc.host.timestamp = cfm->timestamp;
        sw_txhdr->desc.host.flags |= TXU_CNTRL_RETRY;

#ifdef CONFIG_SIWIFI_AMSDUS_TX
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        if (sw_txhdr->desc.host.flags & TXU_CNTRL_AMSDU) {
            siwifi_hw->stats.amsdus[sw_txhdr->amsdu.nb - 1].failed++;
            if (sw_txhdr->siwifi_sta)
                sw_txhdr->siwifi_sta->stats.amsdus[sw_txhdr->amsdu.nb - 1].failed++;
        }
#endif
#endif
    }

    /* MORE_DATA will be re-set if needed when pkt will be repushed */
    sw_txhdr->desc.host.flags &= ~TXU_CNTRL_MORE_DATA;
    /* RM#1004574 wireless: add retry frame flag to avoid add amsdu*/
    sw_txhdr->flags.retry_frame = 1;

    cfm->status.value = 0;
    dma_sync_single_for_device(siwifi_hw->dev, sw_txhdr->dma_addr + peek_off,
                               peek_len, DMA_BIDIRECTIONAL);

	txq->credits += credits;
    if (txq->credits > 0)
        siwifi_txq_start(txq, SIWIFI_TXQ_STOP_FULL);

#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    siwifi_hw->stats.tx_retry++;
#endif
    /* Queue the buffer */
    siwifi_txq_queue_skb(skb, txq, siwifi_hw, true);
}


#ifdef CONFIG_SIWIFI_AMSDUS_TX
/* return size of subframe (including header) */
static inline int siwifi_amsdu_subframe_length(struct ethhdr *eth, int eth_len)
{
    /* ethernet header is replaced with amdsu header that have the same size
       Only need to check if LLC/SNAP header will be added */
    int len = eth_len;
    if (ntohs(eth->h_proto) >= ETH_P_802_3_MIN)
    {
        len += sizeof(rfc1042_header) + 2;
    }

    return len;
}

static inline bool siwifi_amsdu_is_aggregable(struct sk_buff *skb)
{
    /* need to add some check on buffer to see if it can be aggregated ? */
    return true;
}


/**
 * siwifi_amsdu_del_subframe_header - remove AMSDU header
 *
 * amsdu_txhdr: amsdu tx descriptor
 *
 * Move back the ethernet header at the "beginning" of the data buffer.
 * (which has been moved in @siwifi_amsdu_add_subframe_header)
 */
static void siwifi_amsdu_del_subframe_header(struct siwifi_amsdu_txhdr *amsdu_txhdr)
{
    struct sk_buff *skb = amsdu_txhdr->skb;
    struct ethhdr *eth;
    u8 *pos;

    pos = skb->data;
    pos += sizeof(struct siwifi_amsdu_txhdr);
    eth = (struct ethhdr*)pos;
    pos += amsdu_txhdr->pad + sizeof(struct ethhdr);

    if (ntohs(eth->h_proto) >= ETH_P_802_3_MIN) {
        pos += sizeof(rfc1042_header) + 2;
    }

    memmove(pos, eth, sizeof(*eth));
    skb_pull(skb, (pos - skb->data));
}

/**
 * siwifi_amsdu_add_subframe_header - Add AMSDU header and link subframe
 *
 * @siwifi_hw Driver main data
 * @skb Buffer to aggregate
 * @sw_txhdr Tx descriptor for the first A-MSDU subframe
 *
 * return 0 on sucess, -1 otherwise
 *
 * This functions Add A-MSDU header and LLC/SNAP header in the buffer
 * and update sw_txhdr of the first subframe to link this buffer.
 * If an error happens, the buffer will be queued as a normal buffer.
 *
 *
 *            Before           After
 *         +-------------+  +-------------+
 *         | HEADROOM    |  | HEADROOM    |
 *         |             |  +-------------+ <- data
 *         |             |  | amsdu_txhdr |
 *         |             |  | * pad size  |
 *         |             |  +-------------+
 *         |             |  | ETH hdr     | keep original eth hdr
 *         |             |  |             | to restore it once transmitted
 *         |             |  +-------------+ <- packet_addr[x]
 *         |             |  | Pad         |
 * data -> +-------------+  +-------------+
 *         |             |  | AMSDU HDR   |
 *         | ETH hdr     |  +-------------+
 *         |             |  | LLC/SNAP    |
 *         +-------------+  +-------------+
 *         | DATA        |  | DATA        |
 *         |             |  |             |
 *         +-------------+  +-------------+
 *
 * Called with tx_lock hold
 */
static int siwifi_amsdu_add_subframe_header(struct siwifi_hw *siwifi_hw,
                                          struct sk_buff *skb,
                                          struct siwifi_sw_txhdr *sw_txhdr)
{
    struct siwifi_amsdu *amsdu = &sw_txhdr->amsdu;
    struct siwifi_amsdu_txhdr *amsdu_txhdr;
    struct ethhdr *amsdu_hdr, *eth = (struct ethhdr *)skb->data;
    int headroom_need, map_len, msdu_len;
    dma_addr_t dma_addr;
    u8 *pos, *map_start;

    msdu_len = skb->len - sizeof(*eth);
    headroom_need = sizeof(*amsdu_txhdr) + amsdu->pad +
        sizeof(*amsdu_hdr);
    if (ntohs(eth->h_proto) >= ETH_P_802_3_MIN) {
        headroom_need += sizeof(rfc1042_header) + 2;
        msdu_len += sizeof(rfc1042_header) + 2;
    }

    /* we should have enough headroom (checked in xmit) */
    if (WARN_ON(skb_headroom(skb) < headroom_need)) {
        return -1;
    }

    /* allocate headroom */
    pos = skb_push(skb, headroom_need);
    amsdu_txhdr = (struct siwifi_amsdu_txhdr *)pos;
    pos += sizeof(*amsdu_txhdr);

    /* move eth header */
    memmove(pos, eth, sizeof(*eth));
    eth = (struct ethhdr *)pos;
    pos += sizeof(*eth);

    /* Add padding from previous subframe */
    map_start = pos;
    memset(pos, 0, amsdu->pad);
    pos += amsdu->pad;

    /* Add AMSDU hdr */
    amsdu_hdr = (struct ethhdr *)pos;
    memcpy(amsdu_hdr->h_dest, eth->h_dest, ETH_ALEN);
    memcpy(amsdu_hdr->h_source, eth->h_source, ETH_ALEN);
    amsdu_hdr->h_proto = htons(msdu_len);
    pos += sizeof(*amsdu_hdr);

    if (ntohs(eth->h_proto) >= ETH_P_802_3_MIN) {
        memcpy(pos, rfc1042_header, sizeof(rfc1042_header));
        pos += sizeof(rfc1042_header);
    }

    /* MAP (and sync) memory for DMA */
    map_len = msdu_len + amsdu->pad + sizeof(*amsdu_hdr);
    dma_addr = dma_map_single(siwifi_hw->dev, map_start, map_len,
                              DMA_BIDIRECTIONAL);
    if (WARN_ON(dma_mapping_error(siwifi_hw->dev, dma_addr))) {
        pos -= sizeof(*eth);
        memmove(pos, eth, sizeof(*eth));
        skb_pull(skb, headroom_need);
        return -1;
    }

    /* update amdsu_txhdr */
    amsdu_txhdr->map_len = map_len;
    amsdu_txhdr->dma_addr = dma_addr;
    amsdu_txhdr->skb = skb;
    amsdu_txhdr->pad = amsdu->pad;

    /* update siwifi_sw_txhdr (of the first subframe) */
    BUG_ON(amsdu->nb != sw_txhdr->desc.host.packet_cnt);
    sw_txhdr->desc.host.packet_addr[amsdu->nb] = dma_addr;
    sw_txhdr->desc.host.packet_len[amsdu->nb] = map_len;
    sw_txhdr->desc.host.packet_cnt++;
    amsdu->nb++;
    amsdu->tx_amsdu_memory_usage += skb->truesize;

    amsdu->pad = AMSDU_PADDING(map_len - amsdu->pad);
    list_add_tail(&amsdu_txhdr->list, &amsdu->hdrs);
    amsdu->len += map_len;

    trace_amsdu_subframe(sw_txhdr);
    return 0;
}

/**
 * siwifi_amsdu_add_subframe - Add this buffer as an A-MSDU subframe if possible
 *
 * @siwifi_hw Driver main data
 * @skb Buffer to aggregate if possible
 * @sta Destination STA
 * @txq sta's txq used for this buffer
 *
 * Tyr to aggregate the buffer in an A-MSDU. If it succeed then the
 * buffer is added as a new A-MSDU subframe with AMSDU and LLC/SNAP
 * headers added (so FW won't have to modify this subframe).
 *
 * To be added as subframe :
 * - sta must allow amsdu
 * - buffer must be aggregable (to be defined)
 * - at least one other aggregable buffer is pending in the queue
 *  or an a-msdu (with enough free space) is currently in progress
 *
 * returns true if buffer has been added as A-MDSP subframe, false otherwise
 *
 */
static bool siwifi_amsdu_add_subframe(struct siwifi_hw *siwifi_hw, struct sk_buff *skb,
                                    struct siwifi_sta *sta, struct siwifi_txq *txq, u8 tid)
{
    bool res = false;
    struct ethhdr *eth;
    int amsdu_maxnb;
    struct siwifi_vif *vif = netdev_priv(txq->ndev);

    traffic_detect_for_amsdu(siwifi_hw, txq, tid);
    amsdu_maxnb = txq->amsdu_maxnb;
   //amsdu_maxnb = 2;
    if (siwifi_hw->mod_params->amsdu_maxnb < amsdu_maxnb)
        amsdu_maxnb = siwifi_hw->mod_params->amsdu_maxnb;

    /* immediately return if amsdu are not allowed for this sta */
    if (!txq->amsdu_len || amsdu_maxnb < 2 ||
            !siwifi_amsdu_is_aggregable(skb) || (txq->status & SIWIFI_TXQ_NDEV_FLOW_CTRL))
        return false;

    spin_lock_bh(&siwifi_hw->tx_lock);

    // workaround RM17157: BUG_ON(amsdu->nb != sw_txhdr->desc.host.packet_cnt)
    if (txq->amsdu && !(txq->amsdu->desc.host.flags & TXU_CNTRL_AMSDU)) {
        txq->amsdu = NULL;
    }

    if (txq->amsdu) {
        /* aggreagation already in progress, add this buffer if enough space
           available, otherwise end the current amsdu */
        struct siwifi_sw_txhdr *sw_txhdr = txq->amsdu;
        eth = (struct ethhdr *)(skb->data);
        if (((sw_txhdr->amsdu.len + sw_txhdr->amsdu.pad +
              siwifi_amsdu_subframe_length(eth, skb->len)) > txq->amsdu_len) ||
            siwifi_amsdu_add_subframe_header(siwifi_hw, skb, sw_txhdr)) {
            txq->amsdu = NULL;
            goto end;
        }

        if (sw_txhdr->amsdu.nb >= amsdu_maxnb) {
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
            siwifi_hw->stats.amsdus[sw_txhdr->amsdu.nb - 1].done++;
            if (sta)
                sta->stats.amsdus[sw_txhdr->amsdu.nb - 1].done++;
#endif
            /* max number of subframes reached */
            txq->amsdu = NULL;
        }
    } else {
        /* Check if a new amsdu can be started with the previous buffer
           (if any) and this one */
        struct sk_buff *skb_prev;
        struct siwifi_txhdr *txhdr;
        struct siwifi_sw_txhdr *sw_txhdr;
        int len1, len2;
        int peek_off = offsetof(struct siwifi_hw_txhdr, cfm);
        int peek_len = sizeof(((struct siwifi_hw_txhdr *)0)->cfm);

#ifdef CONFIG_BRIDGE_ACCELERATE
        if (siwifi_skb_is_acceled(skb))
            skb_prev = skb_peek_tail(&txq->accel_sk_list);
        else
#endif
        skb_prev = skb_peek_tail(&txq->sk_list);

        if ((!skb_prev || !siwifi_amsdu_is_aggregable(skb_prev)))
            goto end;

        txhdr = (struct siwifi_txhdr *)skb_prev->data;
        sw_txhdr = txhdr->sw_hdr;
        if ((sw_txhdr->amsdu.len) ||
            (sw_txhdr->flags.retry_frame))
            /* previous buffer is already a complete amsdu or a retry */
            goto end;

        eth = (struct ethhdr *)(skb_prev->data + sw_txhdr->headroom);
        if (is_multicast_ether_addr(eth->h_dest)){
            goto end;
        }
        len1 = siwifi_amsdu_subframe_length(eth, (sw_txhdr->frame_len +
                                                sizeof(struct ethhdr)));

        //RM#8261 fix a problem of value of cfm->status not updated in txdatacfm
        //sync headroom of skb_prev case the cache of skb_pre.data will become dirty after access of eth header
        dma_sync_single_for_device(siwifi_hw->dev, sw_txhdr->dma_addr + peek_off,
                               peek_len, DMA_BIDIRECTIONAL);

        eth = (struct ethhdr *)(skb->data);
        len2 = siwifi_amsdu_subframe_length(eth, skb->len);
        if (len1 + AMSDU_PADDING(len1) + len2 > txq->amsdu_len)
            /* not enough space to aggregate those two buffers */
            goto end;

        /* Add subframe header.
           Note: Fw will take care of adding AMDSU header for the first
           subframe while generating 802.11 MAC header */
        INIT_LIST_HEAD(&sw_txhdr->amsdu.hdrs);
        sw_txhdr->amsdu.len = len1;
        sw_txhdr->amsdu.nb = 1;
        sw_txhdr->amsdu.pad = AMSDU_PADDING(len1);
        if (siwifi_amsdu_add_subframe_header(siwifi_hw, skb, sw_txhdr))
            goto end;

        sw_txhdr->desc.host.flags |= TXU_CNTRL_AMSDU;

        if (sw_txhdr->amsdu.nb < amsdu_maxnb)
            txq->amsdu = sw_txhdr;
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        else {
            siwifi_hw->stats.amsdus[sw_txhdr->amsdu.nb - 1].done++;
            if (sta)
                sta->stats.amsdus[sw_txhdr->amsdu.nb - 1].done++;
        }
#endif
    }

    res = true;
    vif->lm_ctl[txq->ndev_idx].amsdu_tx_cnt ++;
    vif->lm_ctl[txq->ndev_idx].amsdu_tx_memory_usage += skb->truesize;

  end:
    spin_unlock_bh(&siwifi_hw->tx_lock);
    return res;
}
#endif /* CONFIG_SIWIFI_AMSDUS_TX */

int siwifi_tcp_check(struct sk_buff *skb)
{
    struct tcphdr *tcphdr = NULL;
    struct iphdr *iphdr = NULL;
    struct ethhdr *ethhdr = (struct ethhdr *)(skb->data);
    if (ethhdr->h_proto != htons(ETH_P_IP))
        return 0;
    iphdr = (struct iphdr *)((char *)ethhdr + sizeof(struct ethhdr));
    if (iphdr->protocol != IPPROTO_TCP)
        return 0;
    // ip地址满足 0x0000a8c0 192.168.*.*
    if (((iphdr->saddr & 0x0000ffff) != 0x0000a8c0) || ((iphdr->daddr & 0x0000ffff) != 0x0000a8c0))
        return 0;
    // ip地址满足 0x0100a8c0 192.168.1.*
    if (((iphdr->saddr & 0xff00ffff) == 0x0100a8c0) || ((iphdr->daddr & 0xff00ffff) == 0x0100a8c0))
        return 0;
    tcphdr = (struct tcphdr *)((char *)iphdr + iphdr->ihl * 4);

    if (tcphdr->rst) {
        printk("[TCP RST][%pM -> %pM] (%pI4 -> %pI4) port(%5d)->(%5d) fail\n",
        ethhdr->h_source, ethhdr->h_dest,
        &iphdr->saddr, &iphdr->daddr,
        ntohs(tcphdr->source), ntohs(tcphdr->dest));
        return 1;
    }

    return 0;
}

/**
 * netdev_tx_t (*ndo_start_xmit)(struct sk_buff *skb,
 *                               struct net_device *dev);
 *	Called when a packet needs to be transmitted.
 *	Must return NETDEV_TX_OK , NETDEV_TX_BUSY.
 *        (can also return NETDEV_TX_LOCKED if NETIF_F_LLTX)
 *
 *  - Initialize the desciptor for this pkt (stored in skb before data)
 *  - Push the pkt in the corresponding Txq
 *  - If possible (i.e. credit available and not in PS) the pkt is pushed
 *    to fw
 */
#ifdef CONFIG_SIWIFI_IGMP
netdev_tx_t siwifi_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
    return _siwifi_start_xmit(skb, dev, NULL);
}
netdev_tx_t _siwifi_start_xmit(struct sk_buff *skb, struct net_device *dev,
                                unsigned char *_unicast_addr)
#else
netdev_tx_t siwifi_start_xmit(struct sk_buff *skb, struct net_device *dev)
#endif
{
    struct siwifi_vif *siwifi_vif = netdev_priv(dev);
    struct siwifi_hw *siwifi_hw = siwifi_vif->siwifi_hw;
    struct siwifi_txhdr *txhdr;
    struct siwifi_sw_txhdr *sw_txhdr;
    struct ethhdr *eth;
#ifdef CONFIG_SIWIFI_IGMP
    unsigned char *unicast_addr = NULL;
#endif
    int siwifi_static_tcp_check = 0;
    struct txdesc_api *desc;
    struct siwifi_sta *sta;
    struct siwifi_txq *txq;
    int headroom;
    int hdr_pads;
    u16 frame_len;
    u16 frame_oft;
    u8 tid;
    bool is_pae_frame = false;

    if (test_bit(SIWIFI_DEV_HW_DEAD, &siwifi_hw->drv_flags)) {
        printk(KERN_CRIT "%s: bypassing (SIWIFI_DEV_HW_DEAD set)\n", __func__);
        return -EBUSY;
    }
#ifdef CONFIG_SIWIFI_IGMP
    unicast_addr = _unicast_addr;
    if (unicast_addr == NULL) {
#endif
    sk_pacing_shift_update(skb->sk, siwifi_hw->tcp_pacing_shift);
    /* check whether the current skb can be used */
    if (skb_shared(skb) || (skb_headroom(skb) < dev->needed_headroom) ||
        (skb_cloned(skb) && (dev->priv_flags & IFF_BRIDGE_PORT))) {
        struct sk_buff *newskb = skb_copy_expand(skb, dev->needed_headroom, 0,
                                                 GFP_ATOMIC);
        if (unlikely(newskb == NULL)) {
            dev_kfree_skb_any(skb);
            return NETDEV_TX_OK;
        }
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        siwifi_hw->stats.tx_copy_expand ++;
#endif

        dev_kfree_skb_any(skb);

        skb = newskb;
    }

#ifdef CONFIG_SIWIFI_IGMP
        if (siwifi_hw->enable_multicast_to_unicast){
                if(siwifi_vif->enable_multicast_to_unicast){
                    unicast_addr = siwifi_multicast_to_unicast(siwifi_hw, skb, dev);
                }
        }
    }
#endif
    if (siwifi_hw->siwifi_static_enable_tcp_check == 1)
        siwifi_static_tcp_check = siwifi_tcp_check(skb);
    /* Retrieve the pointer to the Ethernet data */
    eth = (struct ethhdr *)skb->data;
#if defined(CONFIG_SF19A28_FULLMASK) && IS_ENABLED(CONFIG_SFAX8_HNAT_DRIVER) && IS_ENABLED(CONFIG_NF_FLOW_TABLE)

    //if the skb is with ethernet vlan type , untag vlan
    if (eth->h_proto == htons(ETH_P_8021Q)) {
        memmove((uint8_t*)eth + VLAN_HLEN, (uint8_t*)eth, 2 * ETH_ALEN);
        skb_pull(skb, VLAN_HLEN);
        eth = (struct ethhdr *)(skb->data);
    }
#endif
    // set priority to 0xff for all multicast addr, lmac will think it noqos for tid 0xff
    if (is_multicast_ether_addr(eth->h_dest))
        skb->priority = 0xff;
    //RM#13172 modify skb->priority again in wifi tx
    ///@siwifi_select_txq will not be called when litememory case num of ndeq is 1
    //skb from fast path may not set skb->priority
    //don't touch skb which priority is not zero
    if (skb->priority == 0) {
        //clear vlan info
        skb->vlan_tci = 0;
        //reset proto
        skb->protocol = eth->h_proto;
        if(skb->protocol == htons(ETH_P_IP) || skb->protocol == htons(ETH_P_IPV6)){
            skb_set_network_header(skb, ETH_HLEN);
            skb->priority = cfg80211_classify8021d(skb, NULL) & IEEE80211_QOS_CTL_TAG1D_MASK;
        }
    }
    /* Get the STA id and TID information */
    spin_lock_bh(&siwifi_hw->cb_lock);
#ifdef CONFIG_SIWIFI_IGMP
    sta = siwifi_get_tx_info(siwifi_vif, skb, &tid, unicast_addr);
#else
    sta = siwifi_get_tx_info(siwifi_vif, skb, &tid);
#endif
    if (!sta) {
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        siwifi_hw->stats.tx_drop_sta_null++;
#endif
        goto free;
    }

    if (siwifi_static_tcp_check && !siwifi_hw->siwifi_static_timer_stop) {
        struct siwifi_txq *txq_t = NULL;
        int tid_t = 0;
        foreach_sta_txq(sta, txq_t, tid_t, siwifi_hw) {
            printk("tid_t %2d credits %8d ready %8d time_drop %8d last_inhwq %8d \n",
                tid_t, txq_t->credits, skb_queue_len(&txq_t->sk_list),
                txq_t->siwifi_static_time_drop, jiffies_to_msecs(jiffies - txq_t->siwifi_static_last_inhwq));
        }
        siwifi_static_timer_print(siwifi_hw, sta->sta_idx);
        siwifi_static_timer_clear(siwifi_hw);
    }

    siwifi_hw->siwifi_static_txinfo[sta->sta_idx].siwifi_static_txxmit++;

    if (ntohs(eth->h_proto) == ETH_P_ARP) {
        tid = 5;
    }
    if(sta->user_tid >= 0 && sta->user_tid != SIWIFI_USER_TID_NOT_SET) {
        tid = sta->user_tid;
    }
    txq = siwifi_txq_sta_get(sta, tid, siwifi_hw);
    spin_lock_bh(&siwifi_hw->tx_lock);
    txq->time_stat.come_xmit++;
    if (txq->idx == TXQ_INACTIVE
        /* This means TXQ has been stopped by lite memory control
           no matter the queue is from uplayer stack or from fast xmit path
           all skb should be dropped, otherwise, oom could happen
           */
        || (txq->status & SIWIFI_TXQ_MEMORY_CTRL) || (txq->status & SIWIFI_TXQ_NDEV_FLOW_CTRL)
    ) {
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        if (txq->idx == TXQ_INACTIVE)
            siwifi_hw->stats.tx_drop_txq_inactive++;
        else
            siwifi_hw->stats.tx_drop_full++;
#endif
        txq->time_stat.free_xmit++;
        spin_unlock_bh(&siwifi_hw->tx_lock);
        goto free;
    }
    spin_unlock_bh(&siwifi_hw->tx_lock);
#ifdef CONFIG_SIWIFI_REPEATER
    if (siwifi_vif->rp_info) {
        if (repeater_send_proc(siwifi_vif->rp_info, &skb) < 0) {
            goto free;
        }
    } else
#endif
    {
        siwifi_update_src_filter(siwifi_vif, eth->h_source);
    }

#ifdef CONFIG_BRIDGE_ACCELERATE
    siwifi_dev_traffic_in(skb, dev);
#endif
    //handle pae frame
    if (eth->h_proto == cpu_to_be16(ETH_P_PAE)) {
        is_pae_frame = true;
        tid = 7;
        /*
           u8 *ptr = (u8*)eth;
           printk("tx(%d) eapol key(%d) V(%d) len %d to mac[%02x:%02x:%02x:%02x:%02x:%02x]\n",
           CONFIG_BAND_TYPE, *(ptr + 15), *(ptr + 14), skb->len,
           eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);*/
    }

    siwifi_trace_tx_in(siwifi_hw, sta, skb);
#ifdef CONFIG_SIWIFI_AMSDUS_TX
    if (!is_multicast_ether_addr(eth->h_dest) && siwifi_amsdu_add_subframe(siwifi_hw, skb, sta, txq, tid)) {
        spin_unlock_bh(&siwifi_hw->cb_lock);
        return NETDEV_TX_OK;
    }
#endif

    hdr_pads  = SIWIFI_SWTXHDR_ALIGN_PADS((long)eth);
    headroom  = sizeof(struct siwifi_txhdr) + hdr_pads;
    skb_push(skb, headroom);

    txhdr = (struct siwifi_txhdr *)skb->data;
    sw_txhdr = siwifi_alloc_swtxhdr(siwifi_hw, skb);
    if (unlikely(sw_txhdr == NULL)) {
        skb_pull(skb, headroom);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        siwifi_hw->stats.tx_drop_hdr_alloc_fail++;
#endif
        goto free;
    }

    txhdr->sw_hdr = sw_txhdr;
    desc = &sw_txhdr->desc;

    sw_txhdr->txq       = txq;
    sw_txhdr->txq_init_time = txq->init_time;
    sw_txhdr->mgmt_frame = 0;
    sw_txhdr->flags.mgmt_frame = 0;
    sw_txhdr->flags.retry_frame = 0;
    sw_txhdr->siwifi_sta  = sta;
    sw_txhdr->siwifi_vif  = siwifi_vif;
    sw_txhdr->skb       = skb;
    sw_txhdr->map_len   = skb->len - offsetof(struct siwifi_txhdr, hw_hdr);
#ifdef CONFIG_SIWIFI_AMSDUS_TX
    sw_txhdr->amsdu.len = 0;
    sw_txhdr->amsdu.nb = 0;
    sw_txhdr->amsdu.tx_amsdu_memory_usage = 0;
#endif
    sw_txhdr->headroom  = headroom;
	frame_len = (u16)skb->len - headroom - sizeof(*eth);
	sw_txhdr->frame_len = frame_len;

	// Fill-in the descriptor
	desc->host.ethertype = eth->h_proto;
#ifdef CONFIG_SIWIFI_IGMP
    if (unicast_addr)
        memcpy(&desc->host.eth_dest_addr, unicast_addr, ETH_ALEN);
    else
#endif
        memcpy(&desc->host.eth_dest_addr, eth->h_dest, ETH_ALEN);

    memcpy(&desc->host.eth_src_addr, eth->h_source, ETH_ALEN);

    desc->host.staid = sta->sta_idx;
    desc->host.tid = tid;

    if (unlikely(siwifi_vif->wdev.iftype == NL80211_IFTYPE_AP_VLAN))
        desc->host.vif_idx = siwifi_vif->ap_vlan.master->vif_index;
    else
        desc->host.vif_idx = siwifi_vif->vif_index;

    if (siwifi_vif->use_4addr && (sta->sta_idx < NX_REMOTE_STA_MAX))
        desc->host.flags = TXU_CNTRL_USE_4ADDR;
    else
        desc->host.flags = 0;

    if ((siwifi_vif->tdls_status == TDLS_LINK_ACTIVE) &&
        siwifi_vif->sta.tdls_sta &&
        (memcmp(desc->host.eth_dest_addr.array, siwifi_vif->sta.tdls_sta->mac_addr, ETH_ALEN) == 0)) {
        desc->host.flags |= TXU_CNTRL_TDLS;
        siwifi_vif->sta.tdls_sta->tdls.last_tid = desc->host.tid;
        siwifi_vif->sta.tdls_sta->tdls.last_sn = desc->host.sn;
    }

    if (siwifi_vif->wdev.iftype == NL80211_IFTYPE_MESH_POINT) {
        if (siwifi_vif->is_resending) {
            desc->host.flags |= TXU_CNTRL_MESH_FWD;
        }
    }
    if (is_pae_frame)
        desc->host.flags |= TXU_CNTRL_RATE_PAE;

    /* RM#13644 Limiting multicast rate will affect the performance of 2.4G multi-sta streaming
    //  limit bcmc data rate to basic 6Mbps
    if (
#ifdef CONFIG_SIWIFI_IGMP
            unicast_addr == NULL &&
#endif
            is_multicast_ether_addr(eth->h_dest)) {
        desc->host.flags |= TXU_CNTRL_RATE_PAE;
    }
    */

#ifdef CONFIG_SIWIFI_SPLIT_TX_BUF
    desc->host.packet_len[0] = frame_len;
#else
    desc->host.packet_len = frame_len;
#endif

    txhdr->hw_hdr.cfm.status.value = 0;

    if (unlikely(siwifi_prep_tx(siwifi_hw, txhdr))) {
        siwifi_free_swtxhdr(siwifi_hw, sw_txhdr);
        skb_pull(skb, headroom);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        siwifi_hw->stats.tx_drop_prep_tx++;
#endif
        goto free;
    }

    /* Fill-in TX descriptor */
    frame_oft = sizeof(struct siwifi_txhdr) - offsetof(struct siwifi_txhdr, hw_hdr)
                + hdr_pads + sizeof(*eth);

#ifdef CONFIG_SIWIFI_SPLIT_TX_BUF
    desc->host.packet_addr[0] = sw_txhdr->dma_addr + frame_oft;
    desc->host.packet_cnt = 1;
#else
    desc->host.packet_addr = sw_txhdr->dma_addr + frame_oft;
#endif
    desc->host.status_desc_addr = sw_txhdr->dma_addr;
#ifdef CONFIG_SF19A28_WIFI_LED
		siwifi_led_tx(siwifi_hw);
#endif

    /*
     * cb_lock is to protect variable vif which is used in function siwifi_set_traffic_status
     * but some places,for example,in func "siwifi_rx_chan_switch_ind",first use cb_lock,then use siwifi_hw->tx_lock
     * in func "siwifi_txq_tdls_sta_start";
     * so we must first use cb_lock,then use tx_lock,otherwise will deadlock
     *
     * */
    BUG_ON(txq->ndev_idx >= siwifi_hw->tx_queue_num);
    spin_lock_bh(&siwifi_hw->tx_lock);
    if (txq->idx == TXQ_INACTIVE) {
        spin_unlock_bh(&siwifi_hw->tx_lock);
        siwifi_free_swtxhdr(siwifi_hw, sw_txhdr);
        skb_pull(skb, headroom);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        siwifi_hw->stats.tx_drop_txq_inactive++;
#endif
        goto free;
    }

    if (siwifi_txq_queue_skb(skb, txq, siwifi_hw, false))
        siwifi_hwq_process(siwifi_hw, txq->hwq);
    spin_unlock_bh(&siwifi_hw->tx_lock);
    spin_unlock_bh(&siwifi_hw->cb_lock);

    return NETDEV_TX_OK;

free:
    spin_unlock_bh(&siwifi_hw->cb_lock);
    dev_kfree_skb_any(skb);

    return NETDEV_TX_OK;
}

/**
 * siwifi_start_mgmt_xmit - Transmit a management frame
 *
 * @vif: Vif that send the frame
 * @sta: Destination of the frame. May be NULL if the destiantion is unknown
 *       to the AP.
 * @params: Mgmt frame parameters
 * @offchan: Indicate whether the frame must be send via the offchan TXQ.
 *           (is is redundant with params->offchan ?)
 * @cookie: updated with a unique value to identify the frame with upper layer
 *
 */
int siwifi_start_mgmt_xmit(struct siwifi_vif *vif, struct siwifi_sta *sta,
                         struct cfg80211_mgmt_tx_params *params, bool offchan,
                         u64 *cookie)
{
    struct siwifi_hw *siwifi_hw = vif->siwifi_hw;
    struct siwifi_txhdr *txhdr;
    struct siwifi_sw_txhdr *sw_txhdr;
    struct txdesc_api *desc;
    struct sk_buff *skb;
    u16 frame_len, headroom, frame_oft;
    u8 *data;
    struct siwifi_txq *txq;
    bool robust;
    struct ieee80211_mgmt *mgmt;
    u16 extra_headroom = 0;

    headroom = sizeof(struct siwifi_txhdr);
#ifdef CONFIG_SIWIFI_SAVE_TXHDR_ALLOC
    extra_headroom = sizeof(struct siwifi_sw_txhdr);
#endif
    frame_len = params->len;

    //----------------------------------------------------------------------

    /* Set TID and Queues indexes */
    if (sta) {
        txq = siwifi_txq_sta_get(sta, 8, siwifi_hw);
    } else {
        if (offchan)
            txq = &siwifi_hw->txq[NX_OFF_CHAN_TXQ_IDX];
        else
            txq = siwifi_txq_vif_get(vif, NX_UNK_TXQ_TYPE);
    }

    /* Ensure that TXQ is active */
    if (txq->idx == TXQ_INACTIVE) {
        netdev_dbg(vif->ndev, "TXQ inactive\n");
        vif->mgmt_stats.tx_mgmt_drop++;
        return -EBUSY;
    }

    /*
     * Create a SK Buff object that will contain the provided data
     */
    skb = dev_alloc_skb(headroom + frame_len + extra_headroom);

    if (!skb) {
        return -ENOMEM;
    }

    *cookie = (unsigned long)skb;

    /*
     * Move skb->data pointer in order to reserve room for siwifi_txhdr
     * headroom value will be equal to sizeof(struct siwifi_txhdr)
     * if support save swhdr alloc, then headroom will be equal to
     * sizeof(struct siwifi_txhdr) + extra_headroom
     */
    skb_reserve(skb, headroom + extra_headroom);

    /*
     * Extend the buffer data area in order to contain the provided packet
     * len value (for skb) will be equal to param->len
     */
    data = skb_put(skb, frame_len);
    /* Copy the provided data */
    memcpy(data, params->buf, frame_len);
    robust = ieee80211_is_robust_mgmt_frame(skb);

    //printk skb
    mgmt = (struct ieee80211_mgmt *)skb->data;
    siwifi_trace_mgmt_tx_in(siwifi_hw, skb);
    if (ieee80211_is_deauth(mgmt->frame_control) ||
                ieee80211_is_disassoc(mgmt->frame_control)){
       printk("tx pkt(%d) %s to [%02x:%02x:%02x:%02x:%02x:%02x] reasoncode: %d\n", siwifi_hw->mod_params->is_hb, ieee80211_is_deauth(mgmt->frame_control) ? "deauth" : "disassoc",
               mgmt->da[0], mgmt->da[1], mgmt->da[2], mgmt->da[3], mgmt->da[4], mgmt->da[5], mgmt->u.deauth.reason_code);
    }

    if (siwifi_hw->enable_dbg_sta_conn) {
        if (ieee80211_is_auth(mgmt->frame_control)) {
            printk("tx auth to [%pM] status code %d\n", mgmt->da, mgmt->u.auth.status_code);
        }
        if (ieee80211_is_assoc_req(mgmt->frame_control)) {
            printk("tx assoc req to [%pM] status code\n", mgmt->da);
        }
        if (ieee80211_is_assoc_resp(mgmt->frame_control)) {
            printk("tx assoc resp to [%pM] status code %d\n", mgmt->da, mgmt->u.assoc_resp.status_code);
        }
        if (ieee80211_is_reassoc_resp(mgmt->frame_control)) {
            printk("tx reassoc resp to [%pM] status code %d\n", mgmt->da, mgmt->u.reassoc_resp.status_code);
        }
        if (ieee80211_is_disassoc(mgmt->frame_control)) {
            printk("tx disassoc to [%pM] reason code %d\n", mgmt->da, mgmt->u.disassoc.reason_code);
        }
        if (ieee80211_is_deauth(mgmt->frame_control)) {
            printk("tx deauth to [%pM] reason code %d\n", mgmt->da, mgmt->u.deauth.reason_code);
        }
    }

    /* Update CSA counter if present */
    if (unlikely(params->n_csa_offsets) &&
        vif->wdev.iftype == NL80211_IFTYPE_AP &&
        vif->ap.csa) {
        int i;

        data = skb->data;
        for (i = 0; i < params->n_csa_offsets ; i++) {
            data[params->csa_offsets[i]] = vif->ap.csa->count;
        }
    }

    /*
     * Go back to the beginning of the allocated data area
     * skb->data pointer will move backward
     */
    skb_push(skb, headroom);

    //----------------------------------------------------------------------

    /* Fill the TX Header */
    txhdr = (struct siwifi_txhdr *)skb->data;

    txhdr->hw_hdr.cfm.status.value = 0;

    //----------------------------------------------------------------------

    /* Fill the SW TX Header */
    sw_txhdr = siwifi_alloc_swtxhdr(siwifi_hw, skb);
    if (unlikely(sw_txhdr == NULL)) {
        dev_kfree_skb(skb);
        return -ENOMEM;
    }
    txhdr->sw_hdr = sw_txhdr;

    sw_txhdr->txq = txq;
    sw_txhdr->txq_init_time = txq->init_time;
    sw_txhdr->mgmt_frame = 1;
    /* RM#1004574 wireless: Modify the memory usage of mgmt frame and retry frame*/
    sw_txhdr->flags.mgmt_frame = 1;
    sw_txhdr->flags.retry_frame = 0;
    sw_txhdr->frame_len = frame_len;
    sw_txhdr->siwifi_sta = sta;
    sw_txhdr->siwifi_vif = vif;
    sw_txhdr->skb = skb;
    sw_txhdr->headroom = headroom;
    sw_txhdr->map_len = skb->len - offsetof(struct siwifi_txhdr, hw_hdr);
#ifdef CONFIG_SIWIFI_AMSDUS_TX
    sw_txhdr->amsdu.len = 0;
    sw_txhdr->amsdu.nb = 0;
    sw_txhdr->amsdu.tx_amsdu_memory_usage = 0;
#endif
    //----------------------------------------------------------------------

    /* Fill the Descriptor to be provided to the MAC SW */
    desc = &sw_txhdr->desc;

    desc->host.staid = (sta) ? sta->sta_idx : 0xFF;
    desc->host.vif_idx = vif->vif_index;
    desc->host.tid = 0xFF;
    desc->host.flags = TXU_CNTRL_MGMT;
    if (robust)
        desc->host.flags |= TXU_CNTRL_MGMT_ROBUST;

#ifdef CONFIG_SIWIFI_SPLIT_TX_BUF
    desc->host.packet_len[0] = frame_len;
#else
    desc->host.packet_len = frame_len;
#endif
    if (params->no_cck)
        desc->host.flags |= TXU_CNTRL_MGMT_NO_CCK;

    if (params->dont_wait_for_ack)
        desc->host.flags |= TXU_CNTRL_MGMT_NO_ACK;

    /* Get DMA Address */
    if (unlikely(siwifi_prep_tx(siwifi_hw, txhdr))) {
        siwifi_free_swtxhdr(siwifi_hw, sw_txhdr);
        dev_kfree_skb(skb);
        return -EBUSY;
    }

    frame_oft = sizeof(struct siwifi_txhdr) - offsetof(struct siwifi_txhdr, hw_hdr);
#ifdef CONFIG_SIWIFI_SPLIT_TX_BUF
    desc->host.packet_addr[0] = sw_txhdr->dma_addr + frame_oft;
    desc->host.packet_cnt = 1;
#else
    desc->host.packet_addr = sw_txhdr->dma_addr + frame_oft;
#endif
    desc->host.status_desc_addr = sw_txhdr->dma_addr;

    //----------------------------------------------------------------------

    spin_lock_bh(&siwifi_hw->tx_lock);
    if (txq->idx == TXQ_INACTIVE) {
        spin_unlock_bh(&siwifi_hw->tx_lock);
        siwifi_free_swtxhdr(siwifi_hw, sw_txhdr);
        goto free;
    }
    vif->mgmt_stats.tx_mgmt++;
    if (siwifi_txq_queue_skb(skb, txq, siwifi_hw, false))
        siwifi_hwq_process(siwifi_hw, txq->hwq);
    spin_unlock_bh(&siwifi_hw->tx_lock);
    return 0;

free:
    dev_kfree_skb_any(skb);
    return 0;
}

#ifdef NEW_SCHEDULE
int siwifi_txdatacfm_burst(void *pthis, void *host_id, uint32_t burst_length)
{
    struct siwifi_hw *siwifi_hw = (struct siwifi_hw *)pthis;
    struct sk_buff_head *skb_list = host_id;
    struct sk_buff *skb;
    struct siwifi_txhdr *txhdr;
    union siwifi_hw_txstatus siwifi_txst;
    struct siwifi_sw_txhdr *sw_txhdr;
    struct siwifi_hwq *hwq;
    struct siwifi_txq *txq;
    struct siwifi_txq *txq_debug;
    int first_check = 0;
    u8 hwq_id;

    int peek_off = offsetof(struct siwifi_hw_txhdr, cfm);
    int peek_len = sizeof(((struct siwifi_hw_txhdr *)0)->cfm);

    while ((skb = __skb_dequeue(skb_list)) != NULL) {
        txhdr = (struct siwifi_txhdr *)skb->data;
        sw_txhdr = txhdr->sw_hdr;

        dma_sync_single_for_cpu(siwifi_hw->dev, sw_txhdr->dma_addr + peek_off,
                                peek_len, DMA_FROM_DEVICE);

        /* Read status in the TX control header */
        siwifi_txst = txhdr->hw_hdr.cfm.status;

        /* Check status in the header. If status is null, it means that the buffer
         * was not transmitted and we have to return immediately */
        BUG_ON(siwifi_txst.value == 0);

        txq = sw_txhdr->txq;
        /* don't use txq->hwq as it may have changed between push and confirm */
        hwq = &siwifi_hw->hwq[sw_txhdr->hw_queue];
        if (!first_check) {
            first_check = 1;
            txq_debug = txq;
            hwq_id = sw_txhdr->hw_queue;
            siwifi_txq_confirm_any(siwifi_hw, txq, hwq, sw_txhdr, 1);
        } else {
            BUG_ON(txq_debug != txq);
            BUG_ON(hwq_id != sw_txhdr->hw_queue);
            siwifi_txq_confirm_any(siwifi_hw, txq, hwq, sw_txhdr, 0);
        }
        /* Update txq and HW queue credits */
        if (sw_txhdr->desc.host.flags & TXU_CNTRL_MGMT) {
            /* For debug purpose (use ftrace kernel option) */
            trace_mgmt_cfm(sw_txhdr->siwifi_vif->vif_index,
                           (sw_txhdr->siwifi_sta) ? sw_txhdr->siwifi_sta->sta_idx : 0xFF,
                           !(siwifi_txst.retry_required || siwifi_txst.sw_retry_required));

            /* Confirm transmission to CFG80211 */
            cfg80211_mgmt_tx_status(&sw_txhdr->siwifi_vif->wdev,
                                    (unsigned long)skb,
                                    (skb->data + sw_txhdr->headroom),
                                    sw_txhdr->frame_len,
                                    !(siwifi_txst.retry_required || siwifi_txst.sw_retry_required),
                                    GFP_ATOMIC);
        } else if ((txq->idx != TXQ_INACTIVE) &&
                   (siwifi_txst.retry_required || siwifi_txst.sw_retry_required)) {
            bool sw_retry = (siwifi_txst.sw_retry_required) ? true : false;
            s16 credits = (siwifi_txst.single_mpdu_retry) ? txhdr->hw_hdr.cfm.credits : 1;

            /* Reset the status */
            txhdr->hw_hdr.cfm.status.value = 0;

            /* The confirmed packet was part of an AMPDU and not acked
             * correctly, so reinject it in the TX path to be retried */
            siwifi_tx_retry(siwifi_hw, skb, txhdr, sw_retry, credits);
            continue;
        }
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        if (!siwifi_txst.tx_successful) {
                siwifi_hw->stats.tx_failed++;
        }
        if (siwifi_txst.sw_flush) {
                siwifi_hw->stats.tx_flush++;
        }else if (siwifi_txst.sw_discard) {
                siwifi_hw->stats.tx_discard++;
        }
#endif

        trace_skb_confirm(skb, txq, hwq, &txhdr->hw_hdr.cfm);

        /* STA may have disconnect (and txq stopped) when buffers were stored
           in fw. In this case do nothing when they're returned */
        if (txq->idx != TXQ_INACTIVE) {
            if (txhdr->hw_hdr.cfm.credits) {
                txq->credits += txhdr->hw_hdr.cfm.credits;
                if (txq->credits <= 0)
                    siwifi_txq_stop(txq, SIWIFI_TXQ_STOP_FULL, SIWIFI_TXQ_STOP_POS_DATA_CFM);
                else if (txq->credits > 0)
                    siwifi_txq_start(txq, SIWIFI_TXQ_STOP_FULL);
            }

            /* continue service period */
            if (unlikely(txq->push_limit && !siwifi_txq_is_full(txq))) {
                siwifi_txq_add_to_hw_list(txq);
            }
        }

#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        if (txhdr->hw_hdr.cfm.ampdu_size &&
            txhdr->hw_hdr.cfm.ampdu_size < IEEE80211_MAX_AMPDU_BUF)
            siwifi_hw->stats.ampdus_tx[txhdr->hw_hdr.cfm.ampdu_size - 1]++;
#endif

#ifdef CONFIG_SIWIFI_AMSDUS_TX
        if (txq->idx != TXQ_INACTIVE)
            txq->amsdu_len = txhdr->hw_hdr.cfm.amsdu_size;
#endif
        /* Update statistics */
#ifdef CONFIG_SIWIFI_AMSDUS_TX
    if (sw_txhdr->amsdu.nb > 0) {
        sw_txhdr->siwifi_vif->net_stats.tx_packets += sw_txhdr->amsdu.nb;
        sw_txhdr->siwifi_vif->net_stats.tx_bytes += sw_txhdr->amsdu.len;
        if (sw_txhdr->siwifi_sta) {
            sw_txhdr->siwifi_sta->stats.tx_packets += sw_txhdr->amsdu.nb;
            sw_txhdr->siwifi_sta->stats.tx_bytes += sw_txhdr->amsdu.len;
    } else
#endif
    {
        sw_txhdr->siwifi_vif->net_stats.tx_packets++;
        sw_txhdr->siwifi_vif->net_stats.tx_bytes += sw_txhdr->frame_len;
        if (sw_txhdr->siwifi_sta) {
            sw_txhdr->siwifi_sta->stats.tx_packets++;
            sw_txhdr->siwifi_sta->stats.tx_bytes += sw_txhdr->frame_len;
        }
    }

    if (sw_txhdr->siwifi_sta && !(sw_txhdr->desc.host.flags & TXU_CNTRL_MGMT)
            && !is_multicast_ether_addr((const u8 *)&txhdr->sw_hdr->desc.host.eth_dest_addr)){
        sw_txhdr->siwifi_sta->stats.last_tx_rate_config = txhdr->hw_hdr.cfm.rate_config;
    }

#ifdef CONFIG_VDR_HW
    vendor_hook_txdata(sw_txhdr);
#endif

#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
        if (siwifi_hw->ate_env.tx_frame_start)
            siwifi_ate_tx_cb(siwifi_hw, skb);
#endif

        /* Release SKBs */
#ifdef CONFIG_SIWIFI_AMSDUS_TX
        if (sw_txhdr->desc.host.flags & TXU_CNTRL_AMSDU)
            siwifi_free_amsdu(siwifi_hw, sw_txhdr);
#endif /* CONFIG_SIWIFI_AMSDUS_TX */

        /* unmap with the least costly DMA_TO_DEVICE since we don't need to inval */
        dma_unmap_single(siwifi_hw->dev, sw_txhdr->dma_addr, sw_txhdr->map_len,
                         DMA_TO_DEVICE);

        skb_pull(skb, sw_txhdr->headroom);
        siwifi_free_swtxhdr(siwifi_hw, sw_txhdr);
        consume_skb(skb);
    }

    return 0;
}
#endif

//minimum update interval for ATF, 100ms
#define ATF_MIN_UPDATE_INTERVAL 100000000
//forced update interval for ATF, 200ms
#define ATF_FORCED_UPDATE_INTERVAL 200000000

void siwifi_update_atf(struct siwifi_hw *siwifi_hw, uint8_t force_update)
{
    u32 current_time = ktime_get_ns();
    u32 time_to_current = current_time - siwifi_hw->atf.last_update_time;
    struct siwifi_vif *vif = NULL;
    struct siwifi_sta *sta = NULL;
    struct siwifi_txq *txq = NULL;
    int tid = 0;
    uint32_t max_rateinfo = 0;

    if (siwifi_hw->atf.enable == 0)
        return;

    if (force_update)
        time_to_current = ATF_FORCED_UPDATE_INTERVAL;

    //less than 100ms since last update or (less than 200ms & not all stas have sent packages), don't update atf
    //more than 200ms since the last update, forced update atf
    if ((time_to_current < ATF_MIN_UPDATE_INTERVAL) || (time_to_current < ATF_FORCED_UPDATE_INTERVAL && (siwifi_hw->atf.txq_nosent_cnt > 0))) {
            return;
    } else {
            siwifi_hw->atf.last_update_time = current_time;
            siwifi_hw->atf.txq_nosent_cnt = 0;
    }

    list_for_each_entry(vif, &siwifi_hw->vifs, list) {
        if (SIWIFI_VIF_TYPE(vif) ==  NL80211_IFTYPE_AP){
            list_for_each_entry(sta, &vif->ap.sta_list, list) {
                foreach_sta_txq(sta, txq, tid, siwifi_hw) {
                    if (txq->atf.rateinfo == 0 || txq->atf.enable == 0)
                        continue;
                    txq->atf.record_rateinfo = txq->atf.rateinfo;
                    txq->atf.addup_rateinfo = 0;
                    txq->atf.have_sent = 0;
                    if (txq->atf.record_rateinfo > max_rateinfo)
                        max_rateinfo = txq->atf.record_rateinfo;
                    siwifi_hw->atf.txq_nosent_cnt++;
                }
            }
        }
    }

    siwifi_hw->atf.max_rateinfo = max_rateinfo;
    siwifi_hw->atf.update_cnt++;
    return;
}
/**
 * siwifi_txdatacfm - FW callback for TX confirmation
 *
 * called with tx_lock hold
 */
int siwifi_txdatacfm(void *pthis, void *host_id)
{
    struct siwifi_hw *siwifi_hw = (struct siwifi_hw *)pthis;
    struct sk_buff *skb = host_id;

    struct siwifi_txhdr *txhdr;
    union siwifi_hw_txstatus siwifi_txst;
    struct siwifi_sw_txhdr *sw_txhdr;
    struct siwifi_hwq *hwq;
    struct siwifi_txq *txq;
    int peek_off = offsetof(struct siwifi_hw_txhdr, cfm);
    int peek_len = sizeof(((struct siwifi_hw_txhdr *)0)->cfm);
    struct siwifi_sta *sta = NULL;

    txhdr = (struct siwifi_txhdr *)skb->data;
    sw_txhdr = txhdr->sw_hdr;
    sta = sw_txhdr->siwifi_sta;
    dma_sync_single_for_cpu(siwifi_hw->dev, sw_txhdr->dma_addr + peek_off,
                            peek_len, DMA_FROM_DEVICE);

    /* Read status in the TX control header */
    siwifi_txst = txhdr->hw_hdr.cfm.status;

    /* Check status in the header. If status is null, it means that the buffer
     * was not transmitted and we have to return immediately */
    if (siwifi_txst.value == 0) {
        dma_sync_single_for_device(siwifi_hw->dev, sw_txhdr->dma_addr + peek_off,
                                   peek_len, DMA_FROM_DEVICE);
        return -1;
    }

    txq = sw_txhdr->txq;
    /* don't use txq->hwq as it may have changed between push and confirm */
    hwq = &siwifi_hw->hwq[sw_txhdr->hw_queue];

     if (sta && !is_multicast_sta(sta->sta_idx) &&
            !(sw_txhdr->desc.host.flags & TXU_CNTRL_MGMT)) {
        siwifi_hw->siwifi_static_txinfo[sta->sta_idx].siwifi_static_txcfm++;
        if (siwifi_txst.tx_successful)
            siwifi_hw->siwifi_static_txinfo[sta->sta_idx].siwifi_static_cfm_ack++;
        if (siwifi_txst.sw_retry_required)
            siwifi_hw->siwifi_static_txinfo[sta->sta_idx].siwifi_static_cfm_ret++;
        if (siwifi_txst.ampdu_timeout)
            siwifi_hw->siwifi_static_txinfo[sta->sta_idx].siwifi_static_a_tt++;
        if (siwifi_txst.single_timeout)
            siwifi_hw->siwifi_static_txinfo[sta->sta_idx].siwifi_static_s_tt++;
    }

    // This is value is not equal. that means this txq has been reinitialized
    // between skb push and skb confirm
    if (txq->init_time != sw_txhdr->txq_init_time) {
        printk("txq has been reinitialized during this frame's tx progress\n");
        siwifi_txq_confirm_any(siwifi_hw, NULL, hwq, sw_txhdr, 1);
        goto end;
    }

    siwifi_txq_confirm_any(siwifi_hw, txq, hwq, sw_txhdr, 1);

#ifdef TOKEN_ENABLE
    // FIXME, it's better to move this segment code into txq.c
    // just dont want add more parameters to function 'siwifi_txq_confirm_any'
	if (txhdr->token_id < NUM_TX_DESCS_PER_AC) {
        if (txq->idx != TXQ_INACTIVE) {
            BUG_ON(hwq->id >= NX_TXQ_CNT);
            BUG_ON(txhdr->token_id >= NUM_TX_DESCS_PER_AC);
            BUG_ON(txq->token_pkt_num[hwq->id][txhdr->token_id] <= 0);
            txq->token_pkt_num[hwq->id][txhdr->token_id]--;
            if (!txq->token_pkt_num[hwq->id][txhdr->token_id])
            {
                hwq->token_status[txhdr->token_id] = 0;
                hwq->outstanding_tokens--;
            }
        }


        if (txq->atf.enable && siwifi_hw->atf.enable == 1 &&
                !(sw_txhdr->desc.host.flags & TXU_CNTRL_MGMT) &&
                txhdr->hw_hdr.cfm.rateinfo >= 0 &&
                txhdr->hw_hdr.cfm.rateinfo <= 3467000 &&
                txq->atf.record_rateinfo != txhdr->hw_hdr.cfm.rateinfo)
        {
            txq->atf.rateinfo = txhdr->hw_hdr.cfm.rateinfo;
            //There is a new txq, forced update atf
            if (txq->atf.record_rateinfo == 0)
                siwifi_update_atf(siwifi_hw, 1);
            else
                siwifi_update_atf(siwifi_hw, 0);
        }
    }

#endif /* TOKEN_ENABLE */

#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    if (!siwifi_txst.tx_successful) {
        siwifi_hw->stats.tx_failed++;
    } else {
        if (sw_txhdr->siwifi_sta) {
            if (sw_txhdr->siwifi_sta->sta_idx < NX_REMOTE_STA_MAX)
                sw_txhdr->siwifi_sta->stats.idle = ktime_get_seconds();
        }
    }
    if (txq) {
        txq->pkt_send_total++;
        if (siwifi_txst.tx_successful) txq->pkt_send_success++;
    }
    if (siwifi_txst.sw_flush) {
        siwifi_hw->stats.tx_flush++;
    } else if (siwifi_txst.sw_discard) {
        siwifi_hw->stats.tx_discard++;
    }
#endif
    /* Update txq and HW queue credits */
    if (sw_txhdr->desc.host.flags & TXU_CNTRL_MGMT) {

        /* For debug purpose (use ftrace kernel option) */
        trace_mgmt_cfm(sw_txhdr->siwifi_vif->vif_index,
                       (sw_txhdr->siwifi_sta) ? sw_txhdr->siwifi_sta->sta_idx : 0xFF,
                       !(siwifi_txst.retry_required || siwifi_txst.sw_retry_required));

        siwifi_trace_mgmt_tx_end(siwifi_hw, skb, &siwifi_txst);
        /* Confirm transmission to CFG80211 */
		if(txq->idx != TXQ_INACTIVE)
			cfg80211_mgmt_tx_status(&sw_txhdr->siwifi_vif->wdev,
					(unsigned long)skb,
					(skb->data + sw_txhdr->headroom),
					sw_txhdr->frame_len,
					!(siwifi_txst.retry_required || siwifi_txst.sw_retry_required),
					GFP_ATOMIC);
    } else if ((txq->idx != TXQ_INACTIVE) &&
               (siwifi_txst.retry_required || siwifi_txst.sw_retry_required)) {
        bool sw_retry = (siwifi_txst.sw_retry_required) ? true : false;
        //RM#8081 for single ampdu maybe in baw window in fullmac when cfm
		s16 credits = (siwifi_txst.single_mpdu_retry) ? txhdr->hw_hdr.cfm.credits : 1;

        /* Reset the status */
        txhdr->hw_hdr.cfm.status.value = 0;

#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        if(siwifi_txst.single_mpdu_retry)
            siwifi_hw->stats.single_retry++;
#endif
        txq->time_stat.inlmac_retry++;
        /* The confirmed packet was part of an AMPDU and not acked
         * correctly, so reinject it in the TX path to be retried */
        siwifi_tx_retry(siwifi_hw, skb, txhdr, sw_retry, credits);
        return 0;
    }
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    else {
        if(!siwifi_txst.tx_successful)
            siwifi_hw->stats.tx_failed++;
    }
#endif
    siwifi_trace_tx_end(siwifi_hw, skb, &siwifi_txst, sw_txhdr->headroom);
    trace_skb_confirm(skb, txq, hwq, &txhdr->hw_hdr.cfm);

    /* STA may have disconnect (and txq stopped) when buffers were stored
       in fw. In this case do nothing when they're returned */
    if (txq->idx != TXQ_INACTIVE) {
        if (txhdr->hw_hdr.cfm.credits) {
            txq->credits += txhdr->hw_hdr.cfm.credits;
            if (txq->credits <= 0)
                siwifi_txq_stop(txq, SIWIFI_TXQ_STOP_FULL, SIWIFI_TXQ_STOP_POS_DATA_CFM);
            else if (txq->credits > 0)
                siwifi_txq_start(txq, SIWIFI_TXQ_STOP_FULL);
        }

        /* continue service period */
        if (unlikely(txq->push_limit && !siwifi_txq_is_full(txq))) {
            siwifi_txq_add_to_hw_list(txq);
        }
    }

#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    if (txhdr->hw_hdr.cfm.ampdu_size &&
        txhdr->hw_hdr.cfm.ampdu_size < IEEE80211_MAX_AMPDU_BUF)
        siwifi_hw->stats.ampdus_tx[txhdr->hw_hdr.cfm.ampdu_size - 1]++;
#endif

#ifdef CONFIG_SIWIFI_AMSDUS_TX
    if (txq->idx != TXQ_INACTIVE)
        txq->amsdu_len = txhdr->hw_hdr.cfm.amsdu_size;
#endif

    /* Update statistics */
#ifdef CONFIG_SIWIFI_AMSDUS_TX
    if (sw_txhdr->amsdu.nb > 0) {
        siwifi_hw->stats.tx_pkt += sw_txhdr->amsdu.nb;
        sw_txhdr->siwifi_vif->net_stats.tx_packets += sw_txhdr->amsdu.nb;
        sw_txhdr->siwifi_vif->net_stats.tx_bytes += sw_txhdr->amsdu.len;
        if (sw_txhdr->siwifi_sta) {
            sw_txhdr->siwifi_sta->stats.tx_packets += sw_txhdr->amsdu.nb;
            sw_txhdr->siwifi_sta->stats.tx_bytes += sw_txhdr->amsdu.len;
		}
    } else
#endif
    {
        siwifi_hw->stats.tx_pkt++;
        sw_txhdr->siwifi_vif->net_stats.tx_packets++;
        sw_txhdr->siwifi_vif->net_stats.tx_bytes += sw_txhdr->frame_len;
        if (sw_txhdr->siwifi_sta) {
            sw_txhdr->siwifi_sta->stats.tx_packets++;
            sw_txhdr->siwifi_sta->stats.tx_bytes += sw_txhdr->frame_len;
        }
    }

    if (sw_txhdr->siwifi_sta && !(sw_txhdr->desc.host.flags & TXU_CNTRL_MGMT)
            && !is_multicast_ether_addr((const u8 *)&txhdr->sw_hdr->desc.host.eth_dest_addr)){
        sw_txhdr->siwifi_sta->stats.last_tx_info = txhdr->hw_hdr.cfm.rateinfo;
    }
#ifdef CONFIG_VDR_HW
    vendor_hook_txdata(sw_txhdr);
#endif

    sw_txhdr->confirm_time = ktime_get_ns();
    {
        u64 time_inlmac = sw_txhdr->confirm_time - sw_txhdr->push_to_lmac_time;
        txq->time_stat.inlmac_total++;
        if (time_inlmac > 100000000) {
            txq->time_stat.inlmac_100ms++;
        } else if( time_inlmac > 50000000) {
            txq->time_stat.inlmac_50ms++;
        } else if (time_inlmac > 20000000) {
            txq->time_stat.inlmac_20ms++;
        } else if (time_inlmac > 10000000) {
            txq->time_stat.inlmac_10ms++;
        } else {
            txq->time_stat.inlmac_0ms++;
        }
    }

#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
    if (siwifi_hw->ate_env.tx_frame_start)
        siwifi_ate_tx_cb(siwifi_hw, skb);
#endif

end:
    /* Release SKBs */
#ifdef CONFIG_SIWIFI_AMSDUS_TX
    if (sw_txhdr->desc.host.flags & TXU_CNTRL_AMSDU)
        siwifi_free_amsdu(siwifi_hw, sw_txhdr);
#endif /* CONFIG_SIWIFI_AMSDUS_TX */

    /* unmap with the least costly DMA_TO_DEVICE since we don't need to inval */
    dma_unmap_single(siwifi_hw->dev, sw_txhdr->dma_addr, sw_txhdr->map_len,
                     DMA_TO_DEVICE);


    skb_pull(skb, sw_txhdr->headroom);
    siwifi_free_swtxhdr(siwifi_hw, sw_txhdr);
    consume_skb(skb);

    return 0;
}

/**
 * siwifi_txq_credit_update - Update credit for one txq
 *
 * @siwifi_hw: Driver main data
 * @sta_idx: STA idx
 * @tid: TID
 * @update: offset to apply in txq credits
 *
 * Called when fw send ME_TX_CREDITS_UPDATE_IND message.
 * Apply @update to txq credits, and stop/start the txq if needed
 */
void siwifi_txq_credit_update(struct siwifi_hw *siwifi_hw, int sta_idx, u8 tid,
                            s16 update)
{
    struct siwifi_sta *sta = &siwifi_hw->sta_table[sta_idx];
    struct siwifi_txq *txq;
    struct siwifi_vif *siwifi_vif;

    txq = siwifi_txq_sta_get(sta, tid, siwifi_hw);
    siwifi_vif = siwifi_hw->vif_table[sta->vif_idx];

    spin_lock(&siwifi_hw->tx_lock);

    if (txq->idx != TXQ_INACTIVE) {
        txq->credits += update;
        if (txq->hwq != NULL)
            traffic_detect_be_edca(siwifi_hw, siwifi_vif, txq->hwq->id, 1);
        trace_credit_update(txq, update);
        if (txq->credits <= 0)
            siwifi_txq_stop(txq, SIWIFI_TXQ_STOP_FULL,SIWIFI_TXQ_STOP_POS_CREDIT_UPDATE);
        else
            siwifi_txq_start(txq, SIWIFI_TXQ_STOP_FULL);
    }

    spin_unlock(&siwifi_hw->tx_lock);
}

#ifdef CONFIG_SIWIFI_AMSDUS_TX
void siwifi_free_amsdu(struct siwifi_hw *siwifi_hw, struct siwifi_sw_txhdr *sw_txhdr)
{
    struct siwifi_amsdu_txhdr *amsdu_txhdr, *tmp;
    struct siwifi_txq *txq = NULL;
    struct siwifi_vif *vif = NULL;

    BUG_ON(sw_txhdr->desc.host.packet_cnt <= 1);
    if (sw_txhdr)
        txq = sw_txhdr->txq;
    if (txq)
        vif = netdev_priv(txq->ndev);
    if (vif) {
        //The packet_cnt include the first skb that is counted in tx_cnt instead of amsdu_tx_cnt
        vif->lm_ctl[txq->ndev_idx].amsdu_tx_cnt -= (sw_txhdr->desc.host.packet_cnt - 1);
        //The amsdu_tx_memory_usage does not include the first skb,so subtract it directly
        vif->lm_ctl[txq->ndev_idx].amsdu_tx_memory_usage -= sw_txhdr->amsdu.tx_amsdu_memory_usage;
    }
    list_for_each_entry_safe(amsdu_txhdr, tmp,  &sw_txhdr->amsdu.hdrs, list) {
        siwifi_amsdu_del_subframe_header(amsdu_txhdr);
        dma_unmap_single(siwifi_hw->dev, amsdu_txhdr->dma_addr,
                amsdu_txhdr->map_len, DMA_TO_DEVICE);
        consume_skb(amsdu_txhdr->skb);
    }
}
#endif

struct siwifi_sw_txhdr *siwifi_alloc_swtxhdr(struct siwifi_hw *siwifi_hw, struct sk_buff *skb)
{
#ifdef CONFIG_SIWIFI_SAVE_TXHDR_ALLOC
    struct siwifi_sw_txhdr *sw_txhdr = (struct siwifi_sw_txhdr *)(skb->data - sizeof(struct siwifi_sw_txhdr));
#else //CONFIG_SIWIFI_SAVE_TXHDR_ALLOC
#ifdef CONFIG_SIWIFI_CACHE_ALLOC
    struct siwifi_sw_txhdr *sw_txhdr = kmem_cache_alloc(sw_txhdr_cache, GFP_ATOMIC);
#else
    struct siwifi_sw_txhdr *sw_txhdr = siwifi_kzalloc(sizeof(struct siwifi_sw_txhdr), GFP_ATOMIC);
#endif
#endif //CONFIG_SIWIFI_SAVE_TXHDR_ALLOC
    return sw_txhdr;
}

void siwifi_free_swtxhdr(struct siwifi_hw *siwifi_hw, struct siwifi_sw_txhdr *sw_txhdr)
{
#ifdef CONFIG_SIWIFI_SAVE_TXHDR_ALLOC
    return;
#else
#ifdef CONFIG_SIWIFI_CACHE_ALLOC
    kmem_cache_free(sw_txhdr_cache, sw_txhdr);
#else
    siwifi_kfree(sw_txhdr);
#endif
#endif
}
