/**
 ****************************************************************************************
 *
 * @file siwifi_msg_rx.c
 *
 * @brief RX function definitions
 *
 * Copyright (C) Siflower 2018-2025
 *
 ****************************************************************************************
 */
#include <net/mac80211.h>
#include "siwifi_defs.h"
#include "siwifi_prof.h"
#include "siwifi_tx.h"
#ifdef CONFIG_SIWIFI_BFMER
#include "siwifi_bfmer.h"
#endif //(CONFIG_SIWIFI_BFMER)
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
#include "siwifi_debugfs.h"
#endif
#ifdef CONFIG_VDR_HW
#include "hw_interface.h"
#endif
#include "siwifi_msg_tx.h"
#include "siwifi_tdls.h"
#include "siwifi_mem.h"
#include "siwifi_events.h"
#include "siwifi_compat.h"
#include "siwifi_lmac_glue.h"

#if defined(CONFIG_SIWIFI_ACS) || defined(CONFIG_SIWIFI_ACS_INTERNAL)
#include "siwifi_acs.h"
#endif

static int siwifi_freq_to_idx(struct siwifi_hw *siwifi_hw, int freq)
{
    struct ieee80211_supported_band *sband;
    int band, ch, idx = 0;

    for (band = NL80211_BAND_2GHZ; band < NUM_NL80211_BANDS; band++) {
        sband = siwifi_hw->wiphy->bands[band];
        if (!sband) {
            continue;
        }

        for (ch = 0; ch < sband->n_channels; ch++, idx++) {
            if (sband->channels[ch].center_freq == freq) {
                goto exit;
            }
        }
    }

    BUG_ON(1);

exit:
    // Channel has been found, return the index
    return idx;
}

/***************************************************************************
 * Messages from MM task
 **************************************************************************/
static inline int siwifi_rx_chan_pre_switch_ind(struct siwifi_hw *siwifi_hw,
                                              struct siwifi_cmd *cmd,
                                              struct ipc_e2a_msg *msg)
{
    struct siwifi_vif *siwifi_vif;
    int chan_idx = ((struct mm_channel_pre_switch_ind *)msg->param)->chan_index;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    REG_SW_SET_PROFILING_CHAN(siwifi_hw, SW_PROF_CHAN_CTXT_PSWTCH_BIT);

    spin_lock_bh(&siwifi_hw->cb_lock);
    list_for_each_entry(siwifi_vif, &siwifi_hw->vifs, list) {
        if (siwifi_vif->up && siwifi_vif->ch_index == chan_idx) {
            siwifi_txq_vif_stop(siwifi_vif, SIWIFI_TXQ_STOP_CHAN, siwifi_hw);
        }
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);

    REG_SW_CLEAR_PROFILING_CHAN(siwifi_hw, SW_PROF_CHAN_CTXT_PSWTCH_BIT);

    return 0;
}

static inline int siwifi_rx_send_debug_frame_ind(struct siwifi_hw *siwifi_hw,
                                                struct siwifi_cmd *cmd,
                                                struct ipc_e2a_msg *msg)
{
    struct mm_send_debug_frame_ind *ind = (struct mm_send_debug_frame_ind *)msg->param;
    u32_l statinfo = ind->statinfo;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    siwifi_ipc_elem_var_deallocs(siwifi_hw, &siwifi_hw->debug_frame_buf);

    siwifi_hw->debug_frame_buf.addr = NULL;

    siwifi_hw->debug_frame_statinfo = statinfo;

    return 0;
}

static inline int siwifi_rx_chan_switch_ind(struct siwifi_hw *siwifi_hw,
                                          struct siwifi_cmd *cmd,
                                          struct ipc_e2a_msg *msg)
{
    struct siwifi_vif *siwifi_vif;
    int chan_idx = ((struct mm_channel_switch_ind *)msg->param)->chan_index;
    bool roc     = ((struct mm_channel_switch_ind *)msg->param)->roc;
    bool roc_tdls = ((struct mm_channel_switch_ind *)msg->param)->roc_tdls;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    REG_SW_SET_PROFILING_CHAN(siwifi_hw, SW_PROF_CHAN_CTXT_SWTCH_BIT);

    spin_lock_bh(&siwifi_hw->cb_lock);
    if (roc_tdls) {
        u8 vif_index = ((struct mm_channel_switch_ind *)msg->param)->vif_index;
        list_for_each_entry(siwifi_vif, &siwifi_hw->vifs, list) {
            if (siwifi_vif->vif_index == vif_index) {
                siwifi_vif->roc_tdls = true;
                siwifi_txq_tdls_sta_start(siwifi_vif, SIWIFI_TXQ_STOP_CHAN, siwifi_hw);
            }
        }
    } else if (!roc) {
        list_for_each_entry(siwifi_vif, &siwifi_hw->vifs, list) {
            if (siwifi_vif->up && siwifi_vif->ch_index == chan_idx) {
                siwifi_txq_vif_start(siwifi_vif, SIWIFI_TXQ_STOP_CHAN, siwifi_hw);
            }
        }
    } else {
        /* Retrieve the allocated RoC element */
        struct siwifi_roc_elem *roc_elem = siwifi_hw->roc_elem;
        if (roc_elem) {
            /* Get VIF on which RoC has been started */
            siwifi_vif = netdev_priv(roc_elem->wdev->netdev);

            /* For debug purpose (use ftrace kernel option) */
            trace_switch_roc(siwifi_vif->vif_index);

            /* If mgmt_roc is true, remain on channel has been started by ourself */
            if (!roc_elem->mgmt_roc) {
                /* Inform the host that we have switch on the indicated off-channel */
                cfg80211_ready_on_channel(roc_elem->wdev, (u64)(siwifi_hw->roc_cookie_cnt),
                                roc_elem->chan, roc_elem->duration, GFP_ATOMIC);
            }

            /* Keep in mind that we have switched on the channel */
            roc_elem->on_chan = true;
        }

        // Enable traffic on OFF channel queue
        siwifi_txq_offchan_start(siwifi_hw);
    }

    siwifi_hw->cur_chanctx = chan_idx;
    siwifi_radar_detection_enable_on_cur_channel(siwifi_hw);

#if defined CONFIG_SF16A18_WIFI_ATE_TOOLS
    siwifi_hw->ate_env.vif_ctx_flag = true;
#endif
    spin_unlock_bh(&siwifi_hw->cb_lock);

    REG_SW_CLEAR_PROFILING_CHAN(siwifi_hw, SW_PROF_CHAN_CTXT_SWTCH_BIT);

    return 0;
}

static inline int siwifi_rx_tdls_chan_switch_cfm(struct siwifi_hw *siwifi_hw,
                                                struct siwifi_cmd *cmd,
                                                struct ipc_e2a_msg *msg)
{
    return 0;
}

static inline int siwifi_rx_tdls_chan_switch_ind(struct siwifi_hw *siwifi_hw,
                                               struct siwifi_cmd *cmd,
                                               struct ipc_e2a_msg *msg)
{
    spin_lock_bh(&siwifi_hw->cb_lock);
    // Enable traffic on OFF channel queue
    siwifi_txq_offchan_start(siwifi_hw);
    spin_unlock_bh(&siwifi_hw->cb_lock);

    return 0;
}

static inline int siwifi_rx_tdls_chan_switch_base_ind(struct siwifi_hw *siwifi_hw,
                                                    struct siwifi_cmd *cmd,
                                                    struct ipc_e2a_msg *msg)
{
    struct siwifi_vif *siwifi_vif;
    u8 vif_index = ((struct tdls_chan_switch_base_ind *)msg->param)->vif_index;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    spin_lock_bh(&siwifi_hw->cb_lock);
    list_for_each_entry(siwifi_vif, &siwifi_hw->vifs, list) {
        if (siwifi_vif->up && siwifi_vif->vif_index == vif_index) {
            siwifi_vif->roc_tdls = false;
            siwifi_txq_tdls_sta_stop(siwifi_vif, SIWIFI_TXQ_STOP_CHAN, siwifi_hw);
        }
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);
    return 0;
}

static inline int siwifi_rx_tdls_peer_ps_ind(struct siwifi_hw *siwifi_hw,
                                           struct siwifi_cmd *cmd,
                                           struct ipc_e2a_msg *msg)
{
    struct siwifi_vif *siwifi_vif;
    u8 vif_index = ((struct tdls_peer_ps_ind *)msg->param)->vif_index;
    bool ps_on = ((struct tdls_peer_ps_ind *)msg->param)->ps_on;

    spin_lock_bh(&siwifi_hw->cb_lock);
    list_for_each_entry(siwifi_vif, &siwifi_hw->vifs, list) {
        if (siwifi_vif->up && siwifi_vif->vif_index == vif_index) {
            siwifi_vif->sta.tdls_sta->tdls.ps_on = ps_on;
            // Update PS status for the TDLS station
            siwifi_ps_bh_enable(siwifi_hw, siwifi_vif->sta.tdls_sta, ps_on ? MM_PS_MODE_ON : MM_PS_MODE_OFF);
        }
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);

    return 0;
}

static inline int siwifi_rx_remain_on_channel_exp_ind(struct siwifi_hw *siwifi_hw,
                                                    struct siwifi_cmd *cmd,
                                                    struct ipc_e2a_msg *msg)
{
    /* Retrieve the allocated RoC element */
    struct siwifi_roc_elem *roc_elem;
    /* Get VIF on which RoC has been started */
    struct siwifi_vif *siwifi_vif;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    spin_lock_bh(&siwifi_hw->cb_lock);
    roc_elem = siwifi_hw->roc_elem;
    if (!roc_elem) {
        spin_unlock_bh(&siwifi_hw->cb_lock);
        return 0;
    }
    siwifi_vif = netdev_priv(roc_elem->wdev->netdev);
    /* For debug purpose (use ftrace kernel option) */
    trace_roc_exp(siwifi_vif->vif_index);

    /* If mgmt_roc is true, remain on channel has been started by ourself */
    /* If RoC has been cancelled before we switched on channel, do not call cfg80211 */
    if (!roc_elem->mgmt_roc && roc_elem->on_chan) {
        /* Inform the host that off-channel period has expired */
        cfg80211_remain_on_channel_expired(roc_elem->wdev, (u64)(siwifi_hw->roc_cookie_cnt),
                                           roc_elem->chan, GFP_ATOMIC);
    }
    spin_lock_bh(&siwifi_hw->tx_lock);
    /* De-init offchannel TX queue */
    siwifi_txq_offchan_deinit(siwifi_vif);
    spin_unlock_bh(&siwifi_hw->tx_lock);
    /* Increase the cookie counter cannot be zero */
    siwifi_hw->roc_cookie_cnt++;

    if (siwifi_hw->roc_cookie_cnt == 0) {
        siwifi_hw->roc_cookie_cnt = 1;
    }

    /* Free the allocated RoC element */
    siwifi_kfree(roc_elem);
    siwifi_hw->roc_elem = NULL;

    spin_unlock_bh(&siwifi_hw->cb_lock);
    return 0;
}

static inline int siwifi_rx_p2p_vif_ps_change_ind(struct siwifi_hw *siwifi_hw,
                                                struct siwifi_cmd *cmd,
                                                struct ipc_e2a_msg *msg)
{
    int vif_idx  = ((struct mm_p2p_vif_ps_change_ind *)msg->param)->vif_index;
    int ps_state = ((struct mm_p2p_vif_ps_change_ind *)msg->param)->ps_state;
    struct siwifi_vif *vif_entry;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    spin_lock_bh(&siwifi_hw->cb_lock);
#if DEBUG_ARRAY_CHECK
    BUG_ON(vif_idx >= NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX);
#endif
    vif_entry = siwifi_hw->vif_table[vif_idx];
    if (vif_entry) {
        if (ps_state == MM_PS_MODE_OFF || ps_state == MM_PS_MODE_OFF_FORCE) {
            // Start TX queues for provided VIF
            siwifi_txq_vif_start(vif_entry, SIWIFI_TXQ_STOP_VIF_PS, siwifi_hw);
        }
        else {
            // Stop TX queues for provided VIF
            siwifi_txq_vif_stop(vif_entry, SIWIFI_TXQ_STOP_VIF_PS, siwifi_hw);
        }
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);
    return 0;
}

static inline int siwifi_rx_channel_survey_ind(struct siwifi_hw *siwifi_hw,
                                             struct siwifi_cmd *cmd,
                                             struct ipc_e2a_msg *msg)
{
    struct mm_channel_survey_ind *ind = (struct mm_channel_survey_ind *)msg->param;
    // Get the channel index
    int idx = siwifi_freq_to_idx(siwifi_hw, ind->freq);
    // Get the survey
    struct siwifi_survey_info *siwifi_survey;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    // Store the received parameters
    spin_lock_bh(&siwifi_hw->cb_lock);
#if DEBUG_ARRAY_CHECK
    BUG_ON(idx >= SCAN_CHANNEL_MAX);
#endif
    siwifi_survey = &siwifi_hw->survey[idx];
    siwifi_survey->chan_time_ms = ind->chan_time_ms;
    siwifi_survey->chan_time_busy_ms = ind->chan_time_busy_ms;
    siwifi_survey->noise_dbm = ind->noise_dbm;
    siwifi_survey->filled = (SURVEY_INFO_TIME |
                           SURVEY_INFO_TIME_BUSY);

    if (ind->noise_dbm != 0) {
        siwifi_survey->filled |= SURVEY_INFO_NOISE_DBM;
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);

    return 0;
}

static inline int siwifi_rx_p2p_noa_upd_ind(struct siwifi_hw *siwifi_hw,
                                          struct siwifi_cmd *cmd,
                                          struct ipc_e2a_msg *msg)
{
    return 0;
}

static inline int siwifi_rx_rssi_status_ind(struct siwifi_hw *siwifi_hw,
                                          struct siwifi_cmd *cmd,
                                          struct ipc_e2a_msg *msg)
{
    struct mm_rssi_status_ind *ind = (struct mm_rssi_status_ind *)msg->param;
    int vif_idx  = ind->vif_index;
    bool rssi_status = ind->rssi_status;

    struct siwifi_vif *vif_entry;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    spin_lock_bh(&siwifi_hw->cb_lock);
#if DEBUG_ARRAY_CHECK
    BUG_ON(vif_idx >= NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX);
#endif
    vif_entry = siwifi_hw->vif_table[vif_idx];
    if (vif_entry) {
        cfg80211_cqm_rssi_notify(vif_entry->ndev,
                                 rssi_status ? NL80211_CQM_RSSI_THRESHOLD_EVENT_LOW :
                                               NL80211_CQM_RSSI_THRESHOLD_EVENT_HIGH,
                                 ind->rssi, GFP_ATOMIC);
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);

    return 0;
}

static inline int siwifi_rx_pktloss_notify_ind(struct siwifi_hw *siwifi_hw,
                                             struct siwifi_cmd *cmd,
                                             struct ipc_e2a_msg *msg)
{
    struct mm_pktloss_ind *ind = (struct mm_pktloss_ind *)msg->param;
    struct siwifi_vif *vif_entry;
    int vif_idx  = ind->vif_index;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    spin_lock_bh(&siwifi_hw->cb_lock);
#if DEBUG_ARRAY_CHECK
    BUG_ON(vif_idx >= NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX);
#endif
    vif_entry = siwifi_hw->vif_table[vif_idx];
    if (vif_entry) {
        cfg80211_cqm_pktloss_notify(vif_entry->ndev, (const u8 *)ind->mac_addr.array,
                                    ind->num_packets, GFP_ATOMIC);
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);
    return 0;
}

static inline int siwifi_rx_csa_counter_ind(struct siwifi_hw *siwifi_hw,
                                          struct siwifi_cmd *cmd,
                                          struct ipc_e2a_msg *msg)
{
    struct mm_csa_counter_ind *ind = (struct mm_csa_counter_ind *)msg->param;
    struct siwifi_vif *vif;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    // Look for VIF entry
    spin_lock_bh(&siwifi_hw->cb_lock);
    list_for_each_entry(vif, &siwifi_hw->vifs, list) {
        if (vif->up) {
            if (vif->vif_index == ind->vif_index) {
                if (vif->ap.csa)
                    vif->ap.csa->count = ind->csa_count;
                else
                    netdev_err(vif->ndev, "CSA counter update but no active CSA");
                break;
            }
        }
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);

    return 0;
}

static inline int siwifi_rx_csa_finish_ind(struct siwifi_hw *siwifi_hw,
                                         struct siwifi_cmd *cmd,
                                         struct ipc_e2a_msg *msg)
{
    struct mm_csa_finish_ind *ind = (struct mm_csa_finish_ind *)msg->param;
    struct siwifi_vif *vif;
    bool found = false;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    spin_lock_bh(&siwifi_hw->cb_lock);
    // Look for VIF entry
    list_for_each_entry(vif, &siwifi_hw->vifs, list) {
        if (vif->up && vif->vif_index == ind->vif_index) {
            found=true;
            break;
        }
    }

    if (found) {
        if (SIWIFI_VIF_TYPE(vif) == NL80211_IFTYPE_AP ||
            SIWIFI_VIF_TYPE(vif) == NL80211_IFTYPE_P2P_GO) {
            if (vif->ap.csa) {
                vif->ap.csa->status = ind->status;
                vif->ap.csa->ch_idx = ind->chan_idx;
                schedule_work(&vif->ap.csa->work);
            } else
                netdev_err(vif->ndev, "CSA finish indication but no active CSA");
        } else {
            if (ind->status == 0) {
                                struct cfg80211_chan_def chandef;
                struct ieee80211_channel *chan = NULL;
                memset(&chandef, 0, sizeof(struct cfg80211_chan_def));
                chan = ieee80211_get_channel(siwifi_hw->wiphy, ind->center_freq);
                if (chan != NULL)
                    chandef.chan = chan;
                else {
                    netdev_err(vif->ndev, "STA CSA finish indication but can not get correct channel");
                    spin_unlock_bh(&siwifi_hw->cb_lock);
                    return -1;
                }
                chandef.center_freq1 = ind->center_freq1;
                chandef.center_freq2 = ind->center_freq2;
                switch (ind->bw) {
                case 0:
                    chandef.width = NL80211_CHAN_WIDTH_20;
                    break;
                case 1:
                    chandef.width = NL80211_CHAN_WIDTH_40;
                    break;
                case 2:
                    chandef.width = NL80211_CHAN_WIDTH_80;
                    break;
                default:
                    chandef.width = NL80211_CHAN_WIDTH_80;
                    break;
                }
                siwifi_chanctx_unlink(vif);
                siwifi_chanctx_link(vif, ind->chan_idx, &chandef);
                if (siwifi_hw->cur_chanctx == ind->chan_idx) {
                    siwifi_radar_detection_enable_on_cur_channel(siwifi_hw);
                    siwifi_txq_vif_start(vif, SIWIFI_TXQ_STOP_CHAN, siwifi_hw);
                } else
                    siwifi_txq_vif_stop(vif, SIWIFI_TXQ_STOP_CHAN, siwifi_hw);
            }
        }
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);

    return 0;
}

static inline int siwifi_rx_csa_traffic_ind(struct siwifi_hw *siwifi_hw,
                                          struct siwifi_cmd *cmd,
                                          struct ipc_e2a_msg *msg)
{
    struct mm_csa_traffic_ind *ind = (struct mm_csa_traffic_ind *)msg->param;
    struct siwifi_vif *vif;
    bool found = false;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    spin_lock_bh(&siwifi_hw->cb_lock);
    // Look for VIF entry
    list_for_each_entry(vif, &siwifi_hw->vifs, list) {
        if (vif->up && vif->vif_index == ind->vif_index) {
            found=true;
            break;
        }
    }

    if (found) {
        if (ind->enable)
            siwifi_txq_vif_start(vif, SIWIFI_TXQ_STOP_CSA, siwifi_hw);
        else
            siwifi_txq_vif_stop(vif, SIWIFI_TXQ_STOP_CSA, siwifi_hw);
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);
    return 0;
}

static inline int siwifi_rx_ps_change_ind(struct siwifi_hw *siwifi_hw,
                                        struct siwifi_cmd *cmd,
                                        struct ipc_e2a_msg *msg)
{
    struct mm_ps_change_ind *ind = (struct mm_ps_change_ind *)msg->param;
    struct siwifi_sta *sta;

    //SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    spin_lock_bh(&siwifi_hw->cb_lock);
    sta = &siwifi_hw->sta_table[ind->sta_idx];

    if (ind->sta_idx >= (NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX)) {
        wiphy_err(siwifi_hw->wiphy, "Invalid sta index reported by fw %d\n",
                  ind->sta_idx);
        spin_unlock_bh(&siwifi_hw->cb_lock);
        return 1;
    }

#if DEBUG_ARRAY_CHECK
    BUG_ON(sta->vif_idx >= NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX);
#endif
    //RM#11027 for siwifi_hw->vif_table[sta->vif_idx]_table maybe set to null in recovery @siwifi_restart_driver
    if (!siwifi_hw->vif_table[sta->vif_idx] && !siwifi_hw->adding_sta) {
        printk("invalid vif_idx=%d\n", sta->vif_idx);
        spin_unlock_bh(&siwifi_hw->cb_lock);
        return 1;
    }

    if (siwifi_hw->vif_table[sta->vif_idx]){
        if (ind->ps_state == MM_PS_MODE_OFF_FORCE) {
            netdev_dbg(siwifi_hw->vif_table[sta->vif_idx]->ndev,
                    "Sta %d, Force change PS mode to OFF", sta->sta_idx);
        } else {
            netdev_dbg(siwifi_hw->vif_table[sta->vif_idx]->ndev,
                    "Sta %d, change PS mode to %s", sta->sta_idx,
                    ind->ps_state ? "ON" : "OFF");
        }
    }

    if (sta->valid) {
        siwifi_ps_bh_enable(siwifi_hw, sta, ind->ps_state);
    } else if (siwifi_hw->adding_sta) {
        if (ind->ps_state == MM_PS_MODE_ON ||
                ind->ps_state == MM_PS_MODE_ON_DYN)
            sta->ps.active = true;
        else
            sta->ps.active = false;
    } else {
        netdev_err(siwifi_hw->vif_table[sta->vif_idx]->ndev,
                   "Ignore PS mode change on invalid sta\n");
    }

    spin_unlock_bh(&siwifi_hw->cb_lock);
    return 0;
}


static inline int siwifi_rx_traffic_req_ind(struct siwifi_hw *siwifi_hw,
                                          struct siwifi_cmd *cmd,
                                          struct ipc_e2a_msg *msg)
{
    struct siwifi_sta *sta;
    struct mm_traffic_req_ind *ind = (struct mm_traffic_req_ind *)msg->param;
    //SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    spin_lock_bh(&siwifi_hw->cb_lock);
#if DEBUG_ARRAY_CHECK
    BUG_ON(ind->sta_idx >= NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX);
#endif
    sta = &siwifi_hw->sta_table[ind->sta_idx];
#if DEBUG_ARRAY_CHECK
    BUG_ON(sta->vif_idx >= NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX);
#endif

    if (siwifi_hw->vif_table[sta->vif_idx])
        netdev_dbg(siwifi_hw->vif_table[sta->vif_idx]->ndev,
                        "Sta %d, asked for %d pkt", sta->sta_idx, ind->pkt_cnt);

    if (sta->valid)
        siwifi_ps_bh_traffic_req(siwifi_hw, sta, ind->pkt_cnt,
                        ind->uapsd ? UAPSD_ID : LEGACY_PS_ID);

    spin_unlock_bh(&siwifi_hw->cb_lock);
    return 0;
}

static inline int siwifi_rx_init_complete_ind(struct siwifi_hw *siwifi_hw,
                                              struct siwifi_cmd *cmd,
                                              struct ipc_e2a_msg *msg)
{
    struct mm_init_complete_ind *ind = (struct mm_init_complete_ind *)msg->param;
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    //TODO: change this to siwifi_hw platform ops
    if (ind->complete == 0x1)
        notify_lmac_complete_ipc(siwifi_hw);
    return 0;
}

static inline int siwifi_la_init_ind(struct siwifi_hw *siwifi_hw,
                struct siwifi_cmd *cmd,
                struct ipc_e2a_msg *msg)
{
    struct mm_init_la_ind *ind = (struct mm_init_la_ind *)msg->param;
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    notify_lmac_la_init_ipc(siwifi_hw, ind->type, ind->enable);
    return 0;
}

/***************************************************************************
 * Messages from SCANU task
 **************************************************************************/
static inline int siwifi_rx_scanu_start_cfm(struct siwifi_hw *siwifi_hw,
                                          struct siwifi_cmd *cmd,
                                          struct ipc_e2a_msg *msg)
{
#ifdef CONFIG_SIWIFI_SORT_SCAN
    struct ieee80211_channel *chan;
    struct cfg80211_bss *bss = NULL;
#if MY_LINUX_VERSION_CODE >= KERNEL_VERSION(4, 8, 0)
    struct cfg80211_scan_info info;
#endif
    int i, j = 0;
#endif

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    siwifi_ipc_elem_var_deallocs(siwifi_hw, &siwifi_hw->scan_ie);
    spin_lock_bh(&siwifi_hw->cb_lock);

#ifdef CONFIG_SIWIFI_EASYMESH
    /* Check if the scan was triggered by easymesh. */
    if (siwifi_hw->easymesh_scan_enbale) {
        spin_unlock_bh(&siwifi_hw->cb_lock);
        /* EasyMesh scanning is complete, process the scan results. */
        siwifi_easymesh_scan_done_hook(siwifi_hw);
        /* Return directly without further processing. */
        return 0;
    }
#endif /* CONFIG_SIWIFI_EASYMESH */

    if (siwifi_hw->scan_request
#if defined(CONFIG_SIWIFI_ACS) || defined(CONFIG_SIWIFI_ACS_INTERNAL)
            || siwifi_hw->acs_request
#endif
            ) {
#ifdef CONFIG_SIWIFI_SORT_SCAN
        //sort the bss by rssi
        for (i = 0; i < siwifi_hw->scan_num; i++) {
            for (j = i + 1; j < siwifi_hw->scan_num; j++) {
                if (((struct scanu_result_ind *)siwifi_hw->scan_results[i].param)->rssi < ((struct scanu_result_ind *)siwifi_hw->scan_results[j].param)->rssi) {
                    siwifi_hw->result_temp = siwifi_hw->scan_results[i];
                    siwifi_hw->scan_results[i] = siwifi_hw->scan_results[j];
                    siwifi_hw->scan_results[j] = siwifi_hw->result_temp;
                }
            }
        }
        //upload bss
        for (i = 0; i < siwifi_hw->scan_num; i++) {
            struct scanu_result_ind *ind = (struct scanu_result_ind *)siwifi_hw->scan_results[i].param;
            chan = ieee80211_get_channel(siwifi_hw->wiphy, ind->center_freq);
            if (chan != NULL) {
                bss = cfg80211_inform_bss_frame(siwifi_hw->wiphy, chan,
                        (struct ieee80211_mgmt *)ind->payload,
                        ind->length, ind->rssi * 100, GFP_ATOMIC);
            }
            if (bss != NULL) {
                cfg80211_put_bss(siwifi_hw->wiphy, bss);
            }
            //get the bsses put successfully
            siwifi_hw->bss_results[i] = cfg80211_get_bss(siwifi_hw->wiphy, NULL, (const u8 *)(((struct ieee80211_mgmt *)ind->payload)->bssid), NULL, 0, IEEE80211_BSS_TYPE_ANY, IEEE80211_PRIVACY_ANY);
        }
#endif

        if (siwifi_hw->scan_request) {
#if MY_LINUX_VERSION_CODE >= KERNEL_VERSION(4, 8, 0)
#ifdef CONFIG_SIWIFI_SORT_SCAN
            info.aborted = false,
#else
            struct cfg80211_scan_info info = {
                .aborted = false,
            };
#endif
            cfg80211_scan_done(siwifi_hw->scan_request, &info);
#else
            cfg80211_scan_done(siwifi_hw->scan_request, false);
#endif
        }
#ifdef CONFIG_VDR_HW
        vendor_hook_scan_done();
#endif
    }

    siwifi_hw->scan_request = NULL;

#if defined(CONFIG_SIWIFI_ACS) || defined(CONFIG_SIWIFI_ACS_INTERNAL)
    if (siwifi_hw->acs_scan) {
        siwifi_acs_scan_done(siwifi_hw);
    }
#endif

	if (siwifi_hw->scaning)
	{
		complete_all(&siwifi_hw->scan_complete);
		siwifi_hw->scaning = false;
	}
    spin_unlock_bh(&siwifi_hw->cb_lock);

    return 0;
}

static inline int siwifi_rx_scanu_result_ind(struct siwifi_hw *siwifi_hw,
                                           struct siwifi_cmd *cmd,
                                           struct ipc_e2a_msg *msg)
{
    struct scanu_result_ind *ind = (struct scanu_result_ind *)msg->param;
#ifdef CONFIG_SIWIFI_SORT_SCAN
    int i;
    int16_t rssi1 = 0;//imm_time_rssi
    int16_t rssi2 = 0;//last_time_rssi
#else
    struct cfg80211_bss *bss = NULL;
    struct ieee80211_channel *chan;
#endif

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

#ifdef CONFIG_SIWIFI_EASYMESH
    /* Check if the scan was triggered by easymesh. */
    if (siwifi_hw->easymesh_scan_enbale) {
        /* Processing easymesh scan results. */
        siwifi_easymesh_update_scan_result(siwifi_hw, ind);
        /* Return directly without further processing. */
        return 0;
    }
#endif /* CONFIG_SIWIFI_EASYMESH */

#ifdef CONFIG_VDR_HW
    vendor_hook_scan_result_ind(ind->center_freq,
            (struct ieee80211_mgmt *)ind->payload, ind->rssi);
#endif

#ifdef CONFIG_SIWIFI_SORT_SCAN
    /*Avoid duplicate bss*/
    for (i = 0; i < siwifi_hw->scan_num; i++) {
        struct scanu_result_ind *ind_temp = (struct scanu_result_ind *)(siwifi_hw->scan_results[i].param);
        if (memcmp(((struct ieee80211_mgmt *)ind_temp->payload)->bssid, ((struct ieee80211_mgmt *)ind->payload)->bssid, ETH_ALEN) == 0) {
            rssi1 = ~(ind->rssi -1);
            rssi2 = ~(ind_temp->rssi -1);
            if(((rssi1 - rssi2) > 10) || ((rssi2 - rssi1) > 10))
            {
                    //printk(">>>>>>>>>>>>>>rssi diff too much >>>sta is %pM >>> rssi1 : %d rssi2 : %d>>>\n",((struct ieee80211_mgmt *)ind->payload)->bssid, rssi1, rssi2);
                    return 0;
            }
            siwifi_hw->scan_results[i] = *msg;
            //printk("ind_temp rssi : %d ssid is %pM >>>>>>>> ind rssi : %d  ssid is %pM \n",ind_temp->rssi,((struct ieee80211_mgmt *)ind_temp->payload)->bssid,ind->rssi,((struct ieee80211_mgmt *)ind->payload)->bssid);
            return 0;
        }
    }
    /*If there is no duplicate bss and scan_num is less than 64*/
    if (siwifi_hw->scan_num < MAX_SCAN_BSS_CNT) {
        siwifi_hw->scan_results[i] = *msg;
        siwifi_hw->scan_num++;
    }
    else
        printk("error: scan result is out of range");
#else
#ifdef CONFIG_SIWIFI_CH_OFFSET
    chan = ieee80211_get_channel(siwifi_hw->wiphy, ind->center_freq - siwifi_hw->ch_offset);
#else
    chan = ieee80211_get_channel(siwifi_hw->wiphy, ind->center_freq);
#endif
    if (chan != NULL)
        bss = cfg80211_inform_bss_frame(siwifi_hw->wiphy, chan,
                                        (struct ieee80211_mgmt *)ind->payload,
                                        ind->length, ind->rssi * 100, GFP_ATOMIC);
    if (bss != NULL)
        cfg80211_put_bss(siwifi_hw->wiphy, bss);
#endif

    return 0;
}

/***************************************************************************
 * Messages from ME task
 **************************************************************************/
static inline int siwifi_rx_me_tkip_mic_failure_ind(struct siwifi_hw *siwifi_hw,
                                                  struct siwifi_cmd *cmd,
                                                  struct ipc_e2a_msg *msg)
{
    struct me_tkip_mic_failure_ind *ind = (struct me_tkip_mic_failure_ind *)msg->param;
    struct siwifi_vif *siwifi_vif;
    struct net_device *dev;
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    spin_lock_bh(&siwifi_hw->cb_lock);
#if DEBUG_ARRAY_CHECK
    BUG_ON(ind->vif_idx >= NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX);
#endif
    siwifi_vif = siwifi_hw->vif_table[ind->vif_idx];
    if (!siwifi_vif) {
        spin_unlock_bh(&siwifi_hw->cb_lock);
        return 0;
    }
    dev = siwifi_vif->ndev;
    cfg80211_michael_mic_failure(dev, (u8 *)&ind->addr, (ind->ga?NL80211_KEYTYPE_GROUP:
                                 NL80211_KEYTYPE_PAIRWISE), ind->keyid,
                                 (u8 *)&ind->tsc, GFP_ATOMIC);
    spin_unlock_bh(&siwifi_hw->cb_lock);

    return 0;
}

static inline int siwifi_rx_me_tx_credits_update_ind(struct siwifi_hw *siwifi_hw,
                                                   struct siwifi_cmd *cmd,
                                                   struct ipc_e2a_msg *msg)
{
    struct me_tx_credits_update_ind *ind = (struct me_tx_credits_update_ind *)msg->param;

    //SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    spin_lock_bh(&siwifi_hw->cb_lock);
    siwifi_txq_credit_update(siwifi_hw, ind->sta_idx, ind->tid, ind->credits);
    spin_unlock_bh(&siwifi_hw->cb_lock);

    return 0;
}

/***************************************************************************
 * Messages from SM task
 **************************************************************************/
static inline int siwifi_rx_sm_repeater_status_ind(struct siwifi_hw *siwifi_hw,
                                         struct siwifi_cmd *cmd,
                                         struct ipc_e2a_msg *msg)
{
    struct sm_repeater_status_ind *ind = (struct sm_repeater_status_ind *)msg->param;
    struct siwifi_vif *siwifi_vif;

    if (!ind->status)
        return 0;

    siwifi_vif = siwifi_hw->vif_table[ind->vif_idx];

    if(!siwifi_vif)
        return 0;

    if (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_STATION) {
        set_repeater_status(siwifi_vif, ind->status);
    } else {
        printk("The vif type is incorrect\n");
    }

    return 0;
}


/*
* get supported rate, ht capability and vht capability information from ies data to
* fill struct station_parameters *params
*/
static inline void siwifi_fill_station_parameters(const u8 *ies, size_t ies_len,
                                                struct station_parameters *params)
{
    const u8 *supp_rates_ie;
    const u8 *ext_supp_rates_ie;
    u8 *merged_supp_rates;
    const u8 *ht_capa_ie;
    const u8 *vht_capa_ie;


    /* Look for SUPPORTED RATES Information Element */
    supp_rates_ie = cfg80211_find_ie(WLAN_EID_SUPP_RATES, ies, ies_len);
    ext_supp_rates_ie = cfg80211_find_ie(WLAN_EID_EXT_SUPP_RATES, ies, ies_len);
    if (supp_rates_ie) {
        if (ext_supp_rates_ie) {
            merged_supp_rates = siwifi_kmalloc(supp_rates_ie[1] + ext_supp_rates_ie[1],
                                                GFP_ATOMIC);

            memcpy((u8 *)merged_supp_rates, supp_rates_ie + 2, supp_rates_ie[1]);
            memcpy(((u8 *)merged_supp_rates) + supp_rates_ie[1], ext_supp_rates_ie + 2,
                    ext_supp_rates_ie[1]);

            params->supported_rates = merged_supp_rates;
            params->supported_rates_len = supp_rates_ie[1] + ext_supp_rates_ie[1];
        } else {
            merged_supp_rates = siwifi_kmalloc(supp_rates_ie[1], GFP_ATOMIC);

            memcpy((u8 *)merged_supp_rates, supp_rates_ie + 2, supp_rates_ie[1]);
            params->supported_rates = merged_supp_rates;
            params->supported_rates_len = supp_rates_ie[1];
        }
    }

    /* Look for HT Capability Information Element */
    ht_capa_ie = cfg80211_find_ie(WLAN_EID_HT_CAPABILITY, ies, ies_len);
    if (ht_capa_ie) {
        params->ht_capa = (const struct ieee80211_ht_cap *)(ht_capa_ie + 2);
    }

    /* Look for VHT Capability Information Element */
    vht_capa_ie = cfg80211_find_ie(WLAN_EID_VHT_CAPABILITY, ies, ies_len);
    if (vht_capa_ie) {
        params->vht_capa = (const struct ieee80211_vht_cap *)(vht_capa_ie + 2);
    }
}

struct station_parameters *siwifi_rebuild_sta_params(struct station_parameters *parameters);

static inline int siwifi_rx_sm_connect_ind(struct siwifi_hw *siwifi_hw,
                                         struct siwifi_cmd *cmd,
                                         struct ipc_e2a_msg *msg)
{
    struct sm_connect_ind *ind = (struct sm_connect_ind *)msg->param;
    struct siwifi_vif *siwifi_vif;
    struct net_device *dev;
    const u8 *req_ie, *rsp_ie;
    const u8 *extcap_ie;
    const struct ieee_types_extcap *extcap;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    spin_lock_bh(&siwifi_hw->cb_lock);
#if DEBUG_ARRAY_CHECK
    BUG_ON(ind->vif_idx >= NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX);
#endif
    siwifi_vif = siwifi_hw->vif_table[ind->vif_idx];
    if (!siwifi_vif) {
        spin_unlock_bh(&siwifi_hw->cb_lock);
        return 0;
    }
    dev = siwifi_vif->ndev;
    /* Retrieve IE addresses and lengths */
    req_ie = (const u8 *)ind->assoc_ie_buf;
    rsp_ie = req_ie + ind->assoc_req_ie_len;

#if DEBUG_ARRAY_CHECK
    BUG_ON(ind->ap_idx >= NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX);
#endif
    // Fill-in the AP information
    if (ind->status_code == 0)
    {
        struct siwifi_sta *sta = &siwifi_hw->sta_table[ind->ap_idx];
        u8 txq_status;
        struct ieee80211_channel *chan;
        struct cfg80211_chan_def chandef;
        struct station_parameters params;

        sta->valid = true;
        sta->sta_idx = ind->ap_idx;
        sta->ch_idx = ind->ch_idx;
        sta->vif_idx = ind->vif_idx;
        sta->vlan_idx = sta->vif_idx;
        sta->qos = ind->qos;
        sta->acm = ind->acm;
        sta->ps.active = false;
        sta->aid = ind->aid;
        sta->band = ind->band;
        sta->center_freq = ind->center_freq;
        sta->center_freq1 = ind->center_freq1;
        sta->center_freq2 = ind->center_freq2;

        memset(&params, 0, sizeof(struct station_parameters));
        siwifi_fill_station_parameters(rsp_ie, ind->assoc_rsp_ie_len, &params);
        sta->rec_info.params = siwifi_rebuild_sta_params(&params);
        sta->ht = params.ht_capa ? 1 : 0;
        sta->vht = params.vht_capa ? 1 : 0;
        siwifi_kfree(params.supported_rates);

        siwifi_vif->sta.ap = sta;
        chan = ieee80211_get_channel(siwifi_hw->wiphy, ind->center_freq);
        cfg80211_chandef_create(&chandef, chan, NL80211_CHAN_NO_HT);
        switch (ind->width) {
        case 0:
            sta->width = NL80211_CHAN_WIDTH_20;
            break;
        case 1:
            sta->width = NL80211_CHAN_WIDTH_40;
            break;
        case 2:
            sta->width = NL80211_CHAN_WIDTH_80;
            break;
        default:
            sta->width = NL80211_CHAN_WIDTH_80;
            break;
        }
#if DEBUG_ARRAY_CHECK
        BUG_ON(ind->width > PHY_CHNL_BW_80P80);
#endif
        if (!siwifi_hw->mod_params->ht_on)
            chandef.width = NL80211_CHAN_WIDTH_20_NOHT;
        else
            chandef.width = sta->width;
        chandef.center_freq1 = ind->center_freq1;
        chandef.center_freq2 = ind->center_freq2;
        siwifi_chanctx_link(siwifi_vif, ind->ch_idx, &chandef);
        memcpy(sta->mac_addr, ind->bssid.array, ETH_ALEN);
        if (ind->ch_idx == siwifi_hw->cur_chanctx) {
            txq_status = 0;
        } else {
            txq_status = SIWIFI_TXQ_STOP_CHAN;
        }
        memcpy(sta->ac_param, ind->ac_param, sizeof(sta->ac_param));
        spin_lock_bh(&siwifi_hw->tx_lock);
        siwifi_txq_sta_init(siwifi_hw, sta, txq_status);
        siwifi_txq_tdls_vif_init(siwifi_vif);
        spin_unlock_bh(&siwifi_hw->tx_lock);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        siwifi_dbgfs_register_rc_stat(siwifi_hw, sta);
#endif
        siwifi_mu_group_sta_init(sta, NULL);
        /* Look for TDLS Channel Switch Prohibited flag in the Extended Capability
         * Information Element*/
        extcap_ie = cfg80211_find_ie(WLAN_EID_EXT_CAPABILITY, rsp_ie, ind->assoc_rsp_ie_len);
        if (extcap_ie && extcap_ie[1] >= 5) {
            extcap = (void *)(extcap_ie);
            siwifi_vif->tdls_chsw_prohibited = extcap->ext_capab[4] & WLAN_EXT_CAPA5_TDLS_CH_SW_PROHIBITED;
        }

#ifdef CONFIG_SIWIFI_BFMER
        /* If Beamformer feature is activated, check if features can be used
         * with the new peer device
         */
        if (siwifi_hw->mod_params->bfmer) {
            const u8 *vht_capa_ie;
            const struct ieee80211_vht_cap *vht_cap;

            do {
                /* Look for VHT Capability Information Element */
                vht_capa_ie = cfg80211_find_ie(WLAN_EID_VHT_CAPABILITY, rsp_ie,
                                               ind->assoc_rsp_ie_len);

                /* Stop here if peer device does not support VHT */
                if (!vht_capa_ie) {
                    break;
                }

                vht_cap = (const struct ieee80211_vht_cap *)(vht_capa_ie + 2);

                /* Send MM_BFMER_ENABLE_REQ message if needed */
                siwifi_send_bfmer_enable(siwifi_hw, sta, vht_cap);
            } while (0);
        }
#endif //(CONFIG_SIWIFI_BFMER)
    }

    if(SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_STATION){
        siwifi_vif->wds_success = 1;
    }

    if (!ind->roamed)
        cfg80211_connect_result(dev, (const u8 *)ind->bssid.array, req_ie,
                                ind->assoc_req_ie_len, rsp_ie,
                                ind->assoc_rsp_ie_len, ind->status_code,
                                GFP_ATOMIC);

    netif_tx_start_all_queues(dev);
    netif_carrier_on(dev);

    spin_unlock_bh(&siwifi_hw->cb_lock);
    return 0;
}

static inline int siwifi_rx_sm_disconnect_ind(struct siwifi_hw *siwifi_hw,
                                            struct siwifi_cmd *cmd,
                                            struct ipc_e2a_msg *msg)
{
    struct sm_disconnect_ind *ind = (struct sm_disconnect_ind *)msg->param;
    struct siwifi_vif *siwifi_vif;
    struct net_device *dev;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    spin_lock_bh(&siwifi_hw->cb_lock);
#if DEBUG_ARRAY_CHECK
    BUG_ON(ind->vif_idx >= NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX);
#endif
    siwifi_vif = siwifi_hw->vif_table[ind->vif_idx];
    if (!siwifi_vif) {
        spin_unlock_bh(&siwifi_hw->cb_lock);
        return 0;
    }
    dev = siwifi_vif->ndev;

    /* if vif is not up, siwifi_close has already been called */
    if (siwifi_vif->up) {
        if (!ind->ft_over_ds) {
            cfg80211_disconnected(dev, ind->reason_code, NULL, 0,
                                  (ind->reason_code <= 1), GFP_ATOMIC);
        }
        netif_tx_stop_all_queues(dev);
        netif_carrier_off(dev);
    }

#ifdef CONFIG_SIWIFI_BFMER
    /* Disable Beamformer if supported */
    siwifi_bfmer_report_del(siwifi_hw, siwifi_vif->sta.ap);
#endif //(CONFIG_SIWIFI_BFMER)
    spin_lock_bh(&siwifi_hw->tx_lock);
    siwifi_txq_sta_deinit(siwifi_hw, siwifi_vif->sta.ap);
    siwifi_txq_tdls_vif_deinit(siwifi_vif);
    spin_unlock_bh(&siwifi_hw->tx_lock);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    siwifi_dbgfs_unregister_rc_stat(siwifi_hw, siwifi_vif->sta.ap);
#endif
    siwifi_vif->sta.ap->valid = false;
    siwifi_vif->sta.ap = NULL;
    siwifi_external_auth_disable(siwifi_vif);
    siwifi_chanctx_unlink(siwifi_vif);
    if(SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_STATION){
        siwifi_vif->wds_success = 0;
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);
    return 0;
}

static inline int siwifi_rx_sm_external_auth_required_ind(struct siwifi_hw *siwifi_hw,
                                                        struct siwifi_cmd *cmd,
                                                        struct ipc_e2a_msg *msg)
{
    struct siwifi_vif *siwifi_vif;
    struct sm_external_auth_required_ind *ind =
        (struct sm_external_auth_required_ind *)msg->param;
#if MY_LINUX_VERSION_CODE >= KERNEL_VERSION(3, 18, 0)
    struct net_device *dev;
    struct cfg80211_external_auth_params params;
#endif

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    spin_lock_bh(&siwifi_hw->cb_lock);
#if DEBUG_ARRAY_CHECK
    BUG_ON(ind->vif_idx >= NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX);
#endif
    siwifi_vif = siwifi_hw->vif_table[ind->vif_idx];

#if MY_LINUX_VERSION_CODE >= KERNEL_VERSION(3, 18, 0)
    dev = siwifi_vif->ndev;

    params.action = NL80211_EXTERNAL_AUTH_START;
    memcpy(params.bssid, ind->bssid.array, ETH_ALEN);
    params.ssid.ssid_len = ind->ssid.length;
    memcpy(params.ssid.ssid, ind->ssid.array,
           min_t(size_t, ind->ssid.length, sizeof(params.ssid.ssid)));
    params.key_mgmt_suite = ind->akm;

    if ((ind->vif_idx > NX_VIRT_DEV_MAX) || !siwifi_vif->up ||
        (SIWIFI_VIF_TYPE(siwifi_vif) != NL80211_IFTYPE_STATION) ||
        cfg80211_external_auth_request(dev, &params, GFP_ATOMIC)) {
        wiphy_err(siwifi_hw->wiphy, "Failed to start external auth on vif %d",
                  ind->vif_idx);
        siwifi_send_sm_external_auth_required_rsp(siwifi_hw, siwifi_vif,
                                                WLAN_STATUS_UNSPECIFIED_FAILURE);
        spin_unlock_bh(&siwifi_hw->cb_lock);
        return 0;
    }

    siwifi_external_auth_enable(siwifi_vif);
#else

    siwifi_send_sm_external_auth_required_rsp(siwifi_hw, siwifi_vif,
                                            WLAN_STATUS_UNSPECIFIED_FAILURE);
#endif
    spin_unlock_bh(&siwifi_hw->cb_lock);
    return 0;
}


static inline int siwifi_rx_mesh_path_create_cfm(struct siwifi_hw *siwifi_hw,
                                               struct siwifi_cmd *cmd,
                                               struct ipc_e2a_msg *msg)
{
    struct mesh_path_create_cfm *p_cfm = (struct mesh_path_create_cfm *)msg->param;
    struct siwifi_vif *siwifi_vif;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    spin_lock_bh(&siwifi_hw->cb_lock);
#if DEBUG_ARRAY_CHECK
    BUG_ON(p_cfm->vif_idx >= NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX);
#endif
    siwifi_vif = siwifi_hw->vif_table[p_cfm->vif_idx];
    /* Check we well have a Mesh Point Interface */
    if (siwifi_vif && (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_MESH_POINT)) {
        siwifi_vif->ap.create_path = false;
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);

    return 0;
}

static inline int siwifi_rx_mesh_peer_update_ind(struct siwifi_hw *siwifi_hw,
                                               struct siwifi_cmd *cmd,
                                               struct ipc_e2a_msg *msg)
{
    struct mesh_peer_update_ind *p_ind = (struct mesh_peer_update_ind *)msg->param;
    struct siwifi_vif *siwifi_vif;
    struct siwifi_sta *p_siwifi_sta;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    spin_lock_bh(&siwifi_hw->cb_lock);
#if DEBUG_ARRAY_CHECK
    BUG_ON(p_ind->vif_idx >= NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX);
#endif
    siwifi_vif = siwifi_hw->vif_table[p_ind->vif_idx];
#if DEBUG_ARRAY_CHECK
    BUG_ON(p_ind->sta_idx >= NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX);
#endif
    p_siwifi_sta = &siwifi_hw->sta_table[p_ind->sta_idx];
    if ((p_ind->vif_idx >= (NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX)) ||
        (siwifi_vif && (SIWIFI_VIF_TYPE(siwifi_vif) != NL80211_IFTYPE_MESH_POINT)) ||
        (p_ind->sta_idx >= NX_REMOTE_STA_MAX) ||
        !siwifi_vif) {
        spin_unlock_bh(&siwifi_hw->cb_lock);
        return 1;
    }

    /* Check we well have a Mesh Point Interface */
    if (!siwifi_vif->user_mpm)
    {
        /* Check if peer link has been established or lost */
        if (p_ind->estab) {
            if (!p_siwifi_sta->valid) {
                u8 txq_status;

                p_siwifi_sta->valid = true;
                p_siwifi_sta->sta_idx = p_ind->sta_idx;
                p_siwifi_sta->ch_idx = siwifi_vif->ch_index;
                p_siwifi_sta->vif_idx = p_ind->vif_idx;
                p_siwifi_sta->vlan_idx = p_siwifi_sta->vif_idx;
                p_siwifi_sta->ps.active = false;
                p_siwifi_sta->qos = true;
                p_siwifi_sta->aid = p_ind->sta_idx + 1;
                //p_siwifi_sta->acm = p_ind->acm;
                memcpy(p_siwifi_sta->mac_addr, p_ind->peer_addr.array, ETH_ALEN);

                siwifi_chanctx_link(siwifi_vif, p_siwifi_sta->ch_idx, NULL);

                /* Add the station in the list of VIF's stations */
                INIT_LIST_HEAD(&p_siwifi_sta->list);
                list_add_tail(&p_siwifi_sta->list, &siwifi_vif->ap.sta_list);

                /* Initialize the TX queues */
                if (p_siwifi_sta->ch_idx == siwifi_hw->cur_chanctx) {
                    txq_status = 0;
                } else {
                    txq_status = SIWIFI_TXQ_STOP_CHAN;
                }

                spin_lock_bh(&siwifi_hw->tx_lock);
                siwifi_txq_sta_init(siwifi_hw, p_siwifi_sta, txq_status);
                spin_unlock_bh(&siwifi_hw->tx_lock);
				/* Add sta to vif hash table */
				siwifi_sta_hash_add(siwifi_vif, p_siwifi_sta);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
                siwifi_dbgfs_register_rc_stat(siwifi_hw, p_siwifi_sta);
#endif
            } else {
                WARN_ON(0);
            }
        } else {
            if (p_siwifi_sta->valid) {
                p_siwifi_sta->ps.active = false;
                p_siwifi_sta->valid = false;

                /* Remove the station from the list of VIF's station */
                list_del_init(&p_siwifi_sta->list);
                spin_lock_bh(&siwifi_hw->tx_lock);
                siwifi_txq_sta_deinit(siwifi_hw, p_siwifi_sta);
                spin_unlock_bh(&siwifi_hw->tx_lock);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
                siwifi_dbgfs_unregister_rc_stat(siwifi_hw, p_siwifi_sta);
#endif
            } else {
                WARN_ON(0);
            }
        }
    }
    else {
        if (!p_ind->estab && p_siwifi_sta->valid) {
            /* There is no way to inform upper layer for lost of peer, still
               clean everything in the driver */
            p_siwifi_sta->ps.active = false;
            p_siwifi_sta->valid = false;

            /* Remove the station from the list of VIF's station */
            list_del_init(&p_siwifi_sta->list);
            spin_lock_bh(&siwifi_hw->tx_lock);
            siwifi_txq_sta_deinit(siwifi_hw, p_siwifi_sta);
            spin_unlock_bh(&siwifi_hw->tx_lock);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
            siwifi_dbgfs_unregister_rc_stat(siwifi_hw, p_siwifi_sta);
#endif
        } else {
            WARN_ON(0);
        }
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);

    return 0;
}

static inline int siwifi_rx_mesh_path_update_ind(struct siwifi_hw *siwifi_hw,
                                               struct siwifi_cmd *cmd,
                                               struct ipc_e2a_msg *msg)
{
    struct mesh_path_update_ind *p_ind = (struct mesh_path_update_ind *)msg->param;
    struct siwifi_vif *siwifi_vif;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    if (p_ind->vif_idx >= (NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX))
        return 1;

    spin_lock_bh(&siwifi_hw->cb_lock);
#if DEBUG_ARRAY_CHECK
    BUG_ON(p_ind->vif_idx >= NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX);
#endif
    siwifi_vif = siwifi_hw->vif_table[p_ind->vif_idx];
    /* Check we well have a Mesh Point Interface */
    if (siwifi_vif && (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_MESH_POINT)) {
        struct siwifi_mesh_path *p_mesh_path;
        bool found = false;

        /* Look for path with provided target address */
        list_for_each_entry(p_mesh_path, &siwifi_vif->ap.mpath_list, list) {
            if (p_mesh_path->path_idx == p_ind->path_idx) {
                found = true;
                break;
            }
        }

        /* Check if element has been deleted */
        if (p_ind->delete) {
            if (found) {
                /* Remove element from list */
                list_del_init(&p_mesh_path->list);
                /* Free the element */
                siwifi_kfree(p_mesh_path);
            }
        }
        else {
            if (found) {
                // Update the Next Hop STA
#if DEBUG_ARRAY_CHECK
                BUG_ON(p_ind->nhop_sta_idx >= NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX);
#endif
                p_mesh_path->p_nhop_sta = &siwifi_hw->sta_table[p_ind->nhop_sta_idx];
            } else {
                // Allocate a Mesh Path structure
                p_mesh_path = (struct siwifi_mesh_path *)siwifi_kmalloc(sizeof(struct siwifi_mesh_path), GFP_ATOMIC);

                if (p_mesh_path) {
                    INIT_LIST_HEAD(&p_mesh_path->list);

                    p_mesh_path->path_idx = p_ind->path_idx;
                    p_mesh_path->p_nhop_sta = &siwifi_hw->sta_table[p_ind->nhop_sta_idx];
                    memcpy(&p_mesh_path->tgt_mac_addr, &p_ind->tgt_mac_addr, MAC_ADDR_LEN);

                    // Insert the path in the list of path
                    list_add_tail(&p_mesh_path->list, &siwifi_vif->ap.mpath_list);
                }
            }
        }
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);

    return 0;
}

static inline int siwifi_rx_mesh_proxy_update_ind(struct siwifi_hw *siwifi_hw,
                                               struct siwifi_cmd *cmd,
                                               struct ipc_e2a_msg *msg)
{
    struct mesh_proxy_update_ind *p_ind = (struct mesh_proxy_update_ind *)msg->param;
    struct siwifi_vif *siwifi_vif;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    if (p_ind->vif_idx >= (NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX))
        return 1;

    spin_lock_bh(&siwifi_hw->cb_lock);
#if DEBUG_ARRAY_CHECK
    BUG_ON(p_ind->vif_idx >= NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX);
#endif
    siwifi_vif = siwifi_hw->vif_table[p_ind->vif_idx];
    /* Check we well have a Mesh Point Interface */
    if (siwifi_vif && (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_MESH_POINT)) {
        struct siwifi_mesh_proxy *p_mesh_proxy;
        bool found = false;

        /* Look for path with provided external STA address */
        list_for_each_entry(p_mesh_proxy, &siwifi_vif->ap.proxy_list, list) {
            if (!memcmp(&p_ind->ext_sta_addr, &p_mesh_proxy->ext_sta_addr, ETH_ALEN)) {
                found = true;
                break;
            }
        }

        /* Check if element has been deleted */
        if (p_ind->delete) {
            if (found) {
                /* Remove element from list */
                list_del_init(&p_mesh_proxy->list);
                /* Free the element */
                siwifi_kfree(p_mesh_proxy);
            }
        }
        else {
            if (!found) {
                // Allocate a Mesh Path structure
                p_mesh_proxy = (struct siwifi_mesh_proxy *)siwifi_kmalloc(sizeof(struct siwifi_mesh_proxy), GFP_ATOMIC);

                if (p_mesh_proxy) {
                    INIT_LIST_HEAD(&p_mesh_proxy->list);

                    memcpy(&p_mesh_proxy->ext_sta_addr, &p_ind->ext_sta_addr, MAC_ADDR_LEN);
                    p_mesh_proxy->local = p_ind->local;

                    if (!p_ind->local) {
                        memcpy(&p_mesh_proxy->proxy_addr, &p_ind->proxy_mac_addr, MAC_ADDR_LEN);
                    }

                    // Insert the path in the list of path
                    list_add_tail(&p_mesh_proxy->list, &siwifi_vif->ap.proxy_list);
                }
            }
        }
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);

    return 0;
}

static inline int siwifi_rx_dying_gasp_ind(struct siwifi_hw *siwifi_hw,
                                               struct siwifi_cmd *cmd,
                                               struct ipc_e2a_msg *msg)
{
    struct mm_dying_gasp_ind *dying_gasp_ind = (struct mm_dying_gasp_ind *)msg->param;
    WARN_ON(dying_gasp_ind->dma_addr != siwifi_hw->dying_gasp_info_elem.dma_addr);
    dma_sync_single_for_cpu(siwifi_hw->dev, dying_gasp_ind->dma_addr,
                                sizeof(struct mm_dying_gasp), DMA_FROM_DEVICE);
    siwifi_hw->dying_gasp_valid = true;
    return 0;
}

//RM#10611 update idle time when receive null date frame
static inline int siwifi_rx_update_idle_time_ind(struct siwifi_hw *siwifi_hw,
                                                struct siwifi_cmd *cmd,
                                                struct ipc_e2a_msg *msg)
{
    struct mm_update_idle_time_ind * update_idle_time_ind = (struct mm_update_idle_time_ind *)msg->param;
    struct siwifi_sta *sta = NULL;
    spin_lock_bh(&siwifi_hw->cb_lock);
    if (update_idle_time_ind->sta_idx < (NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX)) {
        sta = &siwifi_hw->sta_table[update_idle_time_ind->sta_idx];
        if (sta->valid) {
            sta->stats.idle = ktime_get_seconds();
            sta->update_time_count++;
        }
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);

    return 0;
}

static inline int siwifi_rx_cca_drop_step_ind(struct siwifi_hw *siwifi_hw,
                            struct siwifi_cmd *cmd,
                            struct ipc_e2a_msg *msg)
{
    struct mm_cca_drop_step_ind *mm_cca_drop_step_ind = (struct mm_cca_drop_step_ind *)msg->param;

    if(siwifi_hw->disable_cca_channel_switch){
        //printk("cca channel switch disabled, step: %d\n", mm_cca_drop_step_ind->cca_step);
        return 0;
    }

    printk("siwifi_rx_cca_drop_step_ind step %d\n", mm_cca_drop_step_ind->cca_step);
    if (mm_cca_drop_step_ind->cca_step == 6)
        siwifi_fast_channel_switch(siwifi_hw);
    return 0;
}

//functions for debug task
void call_rfcmd_err(struct work_struct *wk)
{
    char *envp[] = {"PATH=/sbin:/usr/sbin:/bin:/usr/bin", NULL};
    char **argv;
    int argc, ret;
    char *cmd_buf;
    int cmd_len;
    const char *cmd = "/bin/rfcmd_err";

	cmd_len = strlen(cmd);
    cmd_buf = (char *)siwifi_kmalloc(cmd_len + 8, in_interrupt() ? GFP_ATOMIC : GFP_KERNEL);
    if (!cmd_buf) {
		return;
	}

    sprintf(cmd_buf, "%s\n", cmd);
    argv = argv_split(in_interrupt() ? GFP_ATOMIC : GFP_KERNEL, cmd_buf, &argc);
    if (!argc) {
        siwifi_kfree(cmd_buf);
		return;
    }

    if ((ret = call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC)))
		printk(KERN_CRIT "Failed to call %s (%s returned %d)\n",
						argv[0], cmd, ret);
    argv_free(argv);
    siwifi_kfree(cmd_buf);

    return;
}

/***************************************************************************
 * Messages from APM task
 **************************************************************************/


int sfwifi_reset_work(struct siwifi_hw *siwifi_hw){
	//stop siwifi_send_msg to lmac
	set_bit(SIWIFI_DEV_HW_DEAD, &siwifi_hw->drv_flags);
	INIT_WORK(&siwifi_hw->restart_work_rfcmd, call_rfcmd_err);
	schedule_work(&siwifi_hw->restart_work_rfcmd);
	return 0;
}

/***************************************************************************
 * Messages from DEBUG task
 **************************************************************************/
static inline int siwifi_rx_dbg_error_ind(struct siwifi_hw *siwifi_hw,
                                        struct siwifi_cmd *cmd,
                                        struct ipc_e2a_msg *msg)
{
    struct dbg_error_ind *ind = (struct dbg_error_ind *)msg->param;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

	if (ind->error_type == DBG_ERROR_RFCMD_FAIL) {
		//send rf cmd fail,call "/bin/rfcmd_err(sfwifi reset fmac)"
		sfwifi_reset_work(siwifi_hw);
	}else{
		spin_lock_bh(&siwifi_hw->cb_lock);
		siwifi_error_ind(siwifi_hw, ind->error_type == DBG_ERROR_FATAL, ind->dump_en);
		spin_unlock_bh(&siwifi_hw->cb_lock);
	}
	return 0;
}

static msg_cb_fct mm_hdlrs[MSG_I(MM_MAX)] = {
    [MSG_I(MM_CHANNEL_SWITCH_IND)]     = siwifi_rx_chan_switch_ind,
    [MSG_I(MM_CHANNEL_PRE_SWITCH_IND)] = siwifi_rx_chan_pre_switch_ind,
    [MSG_I(MM_REMAIN_ON_CHANNEL_EXP_IND)] = siwifi_rx_remain_on_channel_exp_ind,
    [MSG_I(MM_PS_CHANGE_IND)]          = siwifi_rx_ps_change_ind,
    [MSG_I(MM_TRAFFIC_REQ_IND)]        = siwifi_rx_traffic_req_ind,
    [MSG_I(MM_P2P_VIF_PS_CHANGE_IND)]  = siwifi_rx_p2p_vif_ps_change_ind,
    [MSG_I(MM_CSA_COUNTER_IND)]        = siwifi_rx_csa_counter_ind,
    [MSG_I(MM_CSA_FINISH_IND)]         = siwifi_rx_csa_finish_ind,
    [MSG_I(MM_CSA_TRAFFIC_IND)]        = siwifi_rx_csa_traffic_ind,
    [MSG_I(MM_CHANNEL_SURVEY_IND)]     = siwifi_rx_channel_survey_ind,
    [MSG_I(MM_P2P_NOA_UPD_IND)]        = siwifi_rx_p2p_noa_upd_ind,
    [MSG_I(MM_RSSI_STATUS_IND)]        = siwifi_rx_rssi_status_ind,
    [MSG_I(MM_PKTLOSS_IND)]            = siwifi_rx_pktloss_notify_ind,
    [MSG_I(MM_INIT_COMPLETE)]          = siwifi_rx_init_complete_ind,
    [MSG_I(MM_LA_INIT_IND)]            = siwifi_la_init_ind,
    [MSG_I(MM_DYING_GASP_IND)]         = siwifi_rx_dying_gasp_ind,
    [MSG_I(MM_UPDATE_IDLE_TIME_IND)]   = siwifi_rx_update_idle_time_ind,
    [MSG_I(MM_CCA_DROP_STEP_IND)]      = siwifi_rx_cca_drop_step_ind,
    [MSG_I(MM_SEND_DEBUG_FRAME_IND)]   = siwifi_rx_send_debug_frame_ind,
};

static msg_cb_fct scan_hdlrs[MSG_I(SCANU_MAX)] = {
    [MSG_I(SCANU_START_CFM)]           = siwifi_rx_scanu_start_cfm,
    [MSG_I(SCANU_RESULT_IND)]          = siwifi_rx_scanu_result_ind,
};

static msg_cb_fct me_hdlrs[MSG_I(ME_MAX)] = {
    [MSG_I(ME_TKIP_MIC_FAILURE_IND)] = siwifi_rx_me_tkip_mic_failure_ind,
    [MSG_I(ME_TX_CREDITS_UPDATE_IND)] = siwifi_rx_me_tx_credits_update_ind,
};

static msg_cb_fct sm_hdlrs[MSG_I(SM_MAX)] = {
    [MSG_I(SM_CONNECT_IND)]    = siwifi_rx_sm_connect_ind,
    [MSG_I(SM_DISCONNECT_IND)] = siwifi_rx_sm_disconnect_ind,
    [MSG_I(SM_EXTERNAL_AUTH_REQUIRED_IND)] = siwifi_rx_sm_external_auth_required_ind,
    [MSG_I(SM_REPEATER_STATUS_IND)] = siwifi_rx_sm_repeater_status_ind,
};

static msg_cb_fct apm_hdlrs[MSG_I(APM_MAX)] = {
};

static msg_cb_fct mesh_hdlrs[MSG_I(MESH_MAX)] = {
    [MSG_I(MESH_PATH_CREATE_CFM)]  = siwifi_rx_mesh_path_create_cfm,
    [MSG_I(MESH_PEER_UPDATE_IND)]  = siwifi_rx_mesh_peer_update_ind,
    [MSG_I(MESH_PATH_UPDATE_IND)]  = siwifi_rx_mesh_path_update_ind,
    [MSG_I(MESH_PROXY_UPDATE_IND)] = siwifi_rx_mesh_proxy_update_ind,
};

static msg_cb_fct dbg_hdlrs[MSG_I(DBG_MAX)] = {
    [MSG_I(DBG_ERROR_IND)]                = siwifi_rx_dbg_error_ind,
};

static msg_cb_fct tdls_hdlrs[MSG_I(TDLS_MAX)] = {
    [MSG_I(TDLS_CHAN_SWITCH_CFM)] = siwifi_rx_tdls_chan_switch_cfm,
    [MSG_I(TDLS_CHAN_SWITCH_IND)] = siwifi_rx_tdls_chan_switch_ind,
    [MSG_I(TDLS_CHAN_SWITCH_BASE_IND)] = siwifi_rx_tdls_chan_switch_base_ind,
    [MSG_I(TDLS_PEER_PS_IND)] = siwifi_rx_tdls_peer_ps_ind,
};

static msg_cb_fct *msg_hdlrs[] = {
    [TASK_MM]    = mm_hdlrs,
    [TASK_DBG]   = dbg_hdlrs,
    [TASK_TDLS]  = tdls_hdlrs,
    [TASK_SCANU] = scan_hdlrs,
    [TASK_ME]    = me_hdlrs,
    [TASK_SM]    = sm_hdlrs,
    [TASK_APM]   = apm_hdlrs,
    [TASK_MESH]  = mesh_hdlrs,
};

/**
 *
 */
void siwifi_rx_handle_msg(struct siwifi_hw *siwifi_hw, struct ipc_e2a_msg *msg)
{
    siwifi_hw->cmd_mgr.msgind(&siwifi_hw->cmd_mgr, msg,
                            msg_hdlrs[MSG_T(msg->id)][MSG_I(msg->id)]);
}