/**
 ******************************************************************************
 *
 * @file siwifi_rx.c
 *
 * Copyright (C) Siflower 2018-2025
 *
 ******************************************************************************
 */
#include <linux/dma-mapping.h>
#include <linux/ieee80211.h>
#include <linux/etherdevice.h>
#include <net/ieee80211_radiotap.h>
#include <linux/vmstat.h>
#ifdef CONFIG_WIFI_RX_THREAD
#include <linux/kthread.h>
#endif

#include "siwifi_defs.h"
#include "siwifi_rx.h"
#include "siwifi_tx.h"
#include "siwifi_prof.h"
#include "ipc_host.h"
#include "siwifi_events.h"
#include "siwifi_compat.h"
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
#include "siwifi_ioctl.h"
#endif
#if defined(CONFIG_SF19A28_FULLMASK) && IS_ENABLED(CONFIG_SFAX8_HNAT_DRIVER) && IS_ENABLED(CONFIG_NF_FLOW_TABLE)
#include "sf_hnat.h"
#endif
#ifdef CONFIG_VDR_HW
#include "hw_interface.h"
#endif
#ifdef CONFIG_SF19A28_WIFI_LED
#include "siwifi_led.h"
#endif
#ifdef CONFIG_SIWIFI_IGMP
#include "siwifi_igmp.h"
#endif
#include "siwifi_mem.h"
#ifdef CONFIG_SIWIFI_REPEATER
#include "siwifi_repeater.h"
#endif
const u8 legrates_lut[] = {
    0,                          /* 0 */
    1,                          /* 1 */
    2,                          /* 2 */
    3,                          /* 3 */
    -1,                         /* 4 */
    -1,                         /* 5 */
    -1,                         /* 6 */
    -1,                         /* 7 */
    10,                         /* 8 */
    8,                          /* 9 */
    6,                          /* 10 */
    4,                          /* 11 */
    11,                         /* 12 */
    9,                          /* 13 */
    7,                          /* 14 */
    5                           /* 15 */
};

struct vendor_radiotap_hdr {
    u8 oui[3];
    u8 subns;
    u16 len;
    u8 data[];
};

/**
 * siwifi_rx_vip_frame - Check frame is a vip frame
 *
 * @skb: skb received
 *
 * Check the frame is dhcp or eapol frame.
 */
static bool siwifi_rx_vip_frame(struct sk_buff *skb)
{
    struct ethhdr *eth;

    skb_reset_mac_header(skb);
    eth = (struct ethhdr *)(skb->data);
    if (siwifi_check_skb_is_dhcp(skb)) {
        printk("%s DHCP: from [%02x:%02x:%02x:%02x:%02x:%02x] to [%02x:%02x:%02x:%02x:%02x:%02x]\n", __func__, eth->h_source[0], eth->h_source[1],
                eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5], eth->h_dest[0], eth->h_dest[1],
                eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
        return true;
    }

    if(eth->h_proto == cpu_to_be16(ETH_P_PAE)) {
        printk("%s EAPOL: from [%02x:%02x:%02x:%02x:%02x:%02x] to [%02x:%02x:%02x:%02x:%02x:%02x]\n", __func__, eth->h_source[0], eth->h_source[1],
                eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5], eth->h_dest[0], eth->h_dest[1],
                eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
        return true;
    }

    return false;
}

/**
 * siwifi_rx_sta_stats - Fill information into siwifi_sta stats
 *
 * @sta: store information relative to managed stations
 * @rxhdr: Hw rx descriptor
 *
 * Get and calculate information relative to managed stations and put them into
 * siwifi_sta->stats.
 * TODO 1: Separate this function from siwifi_rx.c
 * TODO 2: Try to improve method of average calculating
 */
void siwifi_rx_sta_stats(struct siwifi_sta *sta, struct hw_rxhdr *rxhdr)
{
	struct rx_vector_1 *rx_vect1 = &rxhdr->hwvect.rx_vect1;
	uint8_t nss = 0;
	uint8_t format_mod = 0;
	uint8_t mcs = rx_vect1->mcs;
	uint8_t sgi = rx_vect1->short_gi;
	uint8_t bw = rx_vect1->ch_bw;
	// calculate nss
	nss = rx_vect1->stbc ? (rx_vect1->n_sts / 2) : rx_vect1->n_sts;
	// FORMATMOD_VHT 4
	format_mod = rx_vect1->format_mod;
	// init min value and max value and sum of rssi
	if (!sta->stats.data_rssi_old[0]){
		sta->stats.data_rssi_min = rx_vect1->rssi1;
		sta->stats.data_rssi_max = rx_vect1->rssi1;
		sta->stats.data_rssi_sum = 0;
	}
	// update max and min of rssi
	if (sta->stats.data_rssi_min > rx_vect1->rssi1)
		sta->stats.data_rssi_min = rx_vect1->rssi1;
	if (sta->stats.data_rssi_max < rx_vect1->rssi1)
		sta->stats.data_rssi_max = rx_vect1->rssi1;
	//reset count if count reach at 256
	if (sta->stats.count > 255)
		sta->stats.count = 0;
	// calculate average of rssi
	if (!sta->stats.data_rssi_old[255])
	{
		sta->stats.data_rssi_sum += rx_vect1->rssi1;
		sta->stats.data_rssi_avg = sta->stats.data_rssi_sum / (sta->stats.count + 1);
	} else {
		sta->stats.data_rssi_sum += (rx_vect1->rssi1 - sta->stats.data_rssi_old[sta->stats.count]);
		sta->stats.data_rssi_avg = sta->stats.data_rssi_sum >> 8;
	}
	sta->stats.leg_rate = rx_vect1->leg_rate;
	sta->stats.data_rssi_old[sta->stats.count] = rx_vect1->rssi1;
	sta->stats.data_rssi = rx_vect1->rssi1;
	sta->stats.data_rx_bw = bw;
	sta->stats.data_rx_mcs = mcs;
	sta->stats.data_rx_nss = nss;
	sta->stats.format_mod = format_mod;
	sta->stats.short_gi = sgi;
	sta->stats.count++;
}


/**
 * siwifi_rx_get_vif - Return pointer to the destination vif
 *
 * @siwifi_hw: main driver data
 * @vif_idx: vif index present in rx descriptor
 *
 * Select the vif that should receive this frame. Returns NULL if the destination
 * vif is not active or vif is not specified in the descriptor.
 */
static inline
struct siwifi_vif *siwifi_rx_get_vif(struct siwifi_hw *siwifi_hw, int vif_idx)
{
    struct siwifi_vif *siwifi_vif = NULL;

    if (vif_idx < NX_VIRT_DEV_MAX) {
        siwifi_vif = siwifi_hw->vif_table[vif_idx];
        if (!siwifi_vif || !siwifi_vif->up)
            return NULL;
    }

    return siwifi_vif;
}

/**
 * siwifi_rx_statistic - save some statistics about received frames
 *
 * @siwifi_hw: main driver data.
 * @hw_rxhdr: Rx Hardware descriptor of the received frame.
 * @sta: STA that sent the frame.
 */
static void siwifi_rx_statistic(struct siwifi_hw *siwifi_hw, struct hw_rxhdr *hw_rxhdr,
                              struct siwifi_sta *sta)
{
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    struct siwifi_stats *stats = &siwifi_hw->stats;
    struct siwifi_rx_rate_stats *rate_stats = &sta->stats.rx_rate;
    struct rx_vector_1 *rxvect = &hw_rxhdr->hwvect.rx_vect1;
    int mpdu, ampdu, mpdu_prev, rate_idx;

    /* save complete hwvect */
    sta->stats.last_rx = hw_rxhdr->hwvect;

    /* update ampdu rx stats */
    mpdu = hw_rxhdr->hwvect.mpdu_cnt;
    ampdu = hw_rxhdr->hwvect.ampdu_cnt;
    mpdu_prev = stats->ampdus_rx_map[ampdu];

    if (mpdu_prev < mpdu) {
        stats->ampdus_rx_miss += mpdu - mpdu_prev - 1;
    } else {
#if DEBUG_ARRAY_CHECK
        BUG_ON(mpdu_prev >= IEEE80211_MAX_AMPDU_BUF);
#endif
        stats->ampdus_rx[mpdu_prev]++;
    }
    stats->ampdus_rx_map[ampdu] = mpdu;
    if (rxvect->fec_coding)
        stats->ldpc_rx++;
    else
        stats->bcc_rx++;

    if (rxvect->stbc)
        stats->stbc_rx++;

    /* update rx rate statistic */
    if (!rate_stats->size)
        return;

    if (rxvect->format_mod > FORMATMOD_NON_HT_DUP_OFDM) {
        int mcs;
        int bw = rxvect->ch_bw;
        int sgi;
        int nss;
        switch (rxvect->format_mod) {
            case FORMATMOD_HT_MF:
            case FORMATMOD_HT_GF:
#ifdef CONFIG_SFA28_FULLMASK
                mcs = rxvect->mcs % 8;
                nss = rxvect->mcs / 8;
                sgi = rxvect->short_gi;
#else
                mcs = rxvect->ht.mcs % 8;
                nss = rxvect->ht.mcs / 8;
                sgi = rxvect->ht.short_gi;
#endif
                rate_idx = 16 + nss * 32 + mcs * 4 +  bw * 2 + sgi;
                break;
            case FORMATMOD_VHT:
#ifdef CONFIG_SFA28_FULLMASK
                mcs = rxvect->mcs & 0x0F;
                nss = rxvect->stbc ? rxvect->n_sts/2 : rxvect->n_sts;
                sgi = rxvect->short_gi;
#else
                mcs = rxvect->vht.mcs;
                nss = rxvect->vht.nss;
                sgi = rxvect->vht.short_gi;
#endif
                rate_idx = 144 + nss * 80 + mcs * 8 + bw * 2 + sgi;
                break;
            default:
                return;
                break;
        }
    } else {
        int idx = legrates_lut[rxvect->leg_rate];
        if (idx < 4) {
            rate_idx = idx * 2 + rxvect->pre_type;
        } else {
            rate_idx = 8 + idx - 4;
        }
    }
    if (rate_idx < rate_stats->size) {
        rate_stats->table[rate_idx]++;
        rate_stats->cpt++;
    } else {
        wiphy_err(siwifi_hw->wiphy, "RX: Invalid index conversion => %d/%d\n",
                  rate_idx, rate_stats->size);
    }
#endif
}

/* See IEEE 802.1H for LLC/SNAP encapsulation/decapsulation */
/* Ethernet-II snap header (RFC1042 for most EtherTypes) */
const unsigned char rfc1042_header_local[] __aligned(2) =
	{ 0xaa, 0xaa, 0x03, 0x00, 0x00, 0x00 };

/* Bridge-Tunnel header (for EtherTypes ETH_P_AARP and ETH_P_IPX) */
const unsigned char bridge_tunnel_header_local[] __aligned(2) =
	{ 0xaa, 0xaa, 0x03, 0x00, 0x00, 0xf8 };

static bool siwifi_single_amsdu_to_8023s(struct sk_buff *frame, struct sk_buff_head *list)
{
    u8 *payload;
    u16 ethertype;
    struct ethhdr eth;
    memcpy(&eth, frame->data, sizeof(eth));
#ifdef CONFIG_FA_PATCH
    if (ether_addr_equal(eth.h_dest, rfc1042_header_local))
        return false;
#endif
    payload = frame->data + ETH_HLEN;
    ethertype = (payload[6] << 8) | payload[7];
    if (likely((ether_addr_equal(payload, rfc1042_header_local) &&
                    ethertype != ETH_P_AARP && ethertype != ETH_P_IPX) ||
                ether_addr_equal(payload, bridge_tunnel_header_local))) {
        eth.h_proto = htons(ethertype);
        skb_pull(frame, ETH_ALEN + 2);
        memcpy(frame->data, &eth, sizeof(eth));
    }
    __skb_queue_tail(list, frame);
    return true;
}

static bool siwifi_rx_get_skb_list(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif,
                                    struct sk_buff *skb, struct hw_rxhdr *rxhdr,
                                    struct sk_buff **amsdu_skb, struct sk_buff_head *list)
{
    bool amsdu = rxhdr->flags_is_amsdu;

    __skb_queue_head_init(list);

    if (amsdu) {
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        int count;
#endif

#ifdef CFG_DEAGG
        int i, len, subframe_len;
        struct ethhdr *eth;

        if (!siwifi_single_amsdu_to_8023s(skb, list))
            return false;

        for (i = 0; i < RX_AMSDU_CNT_MAX - 1; i++) {
            if (!amsdu_skb[i])
                break;
            amsdu_skb[i]->dev = siwifi_vif->ndev;
            amsdu_skb[i]->priority = 256 + rxhdr->flags_user_prio;
            eth = (struct ethhdr *)(amsdu_skb[i]->data);
            len = ntohs(eth->h_proto);
            subframe_len = sizeof(struct ethhdr) + len;

            if (len < 0 || subframe_len > siwifi_hw->ipc_env->rx_bufsz || (amsdu_skb[i]->tail + subframe_len) > amsdu_skb[i]->end) {
                dev_err(siwifi_hw->dev, "Invalid skb len %d for amsdu[%d] tail %p end %p\n",
                        subframe_len, i, amsdu_skb[i]->tail, amsdu_skb[i]->end);
                return false;
            }
            skb_put(amsdu_skb[i], subframe_len);
            if (!siwifi_single_amsdu_to_8023s(amsdu_skb[i], list))
                return false;
        }
#else
        ieee80211_amsdu_to_8023s(skb, list, siwifi_vif->ndev->dev_addr,
                SIWIFI_VIF_TYPE(siwifi_vif), 0, NULL, NULL);
#endif

#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        count = skb_queue_len(list);
        if (count > ARRAY_SIZE(siwifi_hw->stats.amsdus_rx))
            count = ARRAY_SIZE(siwifi_hw->stats.amsdus_rx);
        siwifi_hw->stats.amsdus_rx[count - 1]++;
#endif
    } else {
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        siwifi_hw->stats.amsdus_rx[0]++;
#endif
        __skb_queue_head(list, skb);
    }
    return true;
}
#ifdef CONFIG_WIFI_RX_THREAD
/*
 When we use dedicated rx thread to process rx skb, in rx data skb we only did two things:
 1, get and push all skbs include amsdu skbs into rx process skb list
 2, wake up rx thread
 */
static bool siwifi_rx_data_skb(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif,
                            struct sk_buff *skb,  struct hw_rxhdr *rxhdr, struct sk_buff **amsdu_skb)
{
    struct siwifi_sta *sta = NULL;
    bool resend = false, forward = true;
    struct sk_buff *rx_skb = NULL;

    struct rx_skb_element *rx_elt = NULL;

    skb->dev = siwifi_vif->ndev;
    rx_elt = (struct rx_skb_element *)siwifi_kmalloc(sizeof(struct rx_skb_element), GFP_ATOMIC);

    if (!rx_elt) {
        printk("oom, can not alloc rx_skb_element\n");
        return false;
    }

    // get all skbs
    if (!siwifi_rx_get_skb_list(siwifi_hw, siwifi_vif,
                skb, rxhdr, amsdu_skb, &rx_elt->skb_list)) {
        siwifi_kfree(rx_elt);
        return false;
    }

    if (((SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_AP) ||
         (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_AP_VLAN) ||
         (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_P2P_GO)) &&
        !(siwifi_vif->ap.flags & SIWIFI_AP_ISOLATE)) {
		const struct ethhdr *eth;
		rx_skb = skb_peek(&rx_elt->skb_list);
		skb_reset_mac_header(rx_skb);
		eth = eth_hdr(rx_skb);

        if (unlikely(is_multicast_ether_addr(eth->h_dest))) {
            /* broadcast pkt need to be forwared to upper layer and resent
               on wireless interface */
            resend = true;
        } else {
            /* unicast pkt for STA inside the BSS, no need to forward to upper
               layer simply resend on wireless interface */
            if (rxhdr->flags_dst_idx < (NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX))
            {
                sta = &siwifi_hw->sta_table[rxhdr->flags_dst_idx];
                if (sta->valid && (sta->vlan_idx == siwifi_vif->vif_index))
                {
                    forward = false;
                    resend = true;
                }
            }
        }
    } else if (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_MESH_POINT) {
        const struct ethhdr *eth;
        rx_skb = skb_peek(&rx_elt->skb_list);
        skb_reset_mac_header(rx_skb);
        eth = eth_hdr(rx_skb);

        if (!is_multicast_ether_addr(eth->h_dest)) {
            /* unicast pkt for STA inside the BSS, no need to forward to upper
               layer simply resend on wireless interface */
            if (rxhdr->flags_dst_idx != SIWIFI_INVALID_STA)
            {
                forward = false;
                resend = true;
            }
        }
    }

    if (!sta && (rxhdr->flags_sta_idx < (NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX))) {
        sta = &siwifi_hw->sta_table[rxhdr->flags_sta_idx];
    } else if (rxhdr->flags_sta_idx != SIWIFI_INVALID_STA) {
        printk("wtf, invalid flags_sta_idx passed from lmac, %d\n", rxhdr->flags_sta_idx);
    }

    rx_elt->vif = siwifi_vif;
    rx_elt->sta = sta;
    rx_elt->rxhdr = rxhdr;
    rx_elt->forward = forward;
    rx_elt->resend = resend;
    rx_elt->skip_len = skip_after_eth_hdr;

    spin_lock_bh(&siwifi_hw->rx_lock);
    list_add_tail(&rx_elt->list, &siwifi_hw->rx_skbs);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    siwifi_hw->rx_thread_dbg.elt_cur_cnt ++;
    if (siwifi_hw->rx_thread_dbg.elt_cur_cnt > siwifi_hw->rx_thread_dbg.elt_max_cnt) {
        siwifi_hw->rx_thread_dbg.elt_max_cnt = siwifi_hw->rx_thread_dbg.elt_cur_cnt;
    }
    siwifi_hw->rx_thread_dbg.skb_cur_cnt += skb_queue_len(&rx_elt->skb_list);
    if (skb_queue_len(&rx_elt->skb_list) > siwifi_hw->rx_thread_dbg.skb_max_per_elt) {
        siwifi_hw->rx_thread_dbg.skb_max_per_elt = skb_queue_len(&rx_elt->skb_list);
    }
    if (siwifi_hw->rx_thread_dbg.skb_cur_cnt > siwifi_hw->rx_thread_dbg.skb_max_cnt) {
        siwifi_hw->rx_thread_dbg.skb_max_cnt = siwifi_hw->rx_thread_dbg.skb_cur_cnt;
    }
#endif
    spin_unlock_bh(&siwifi_hw->rx_lock);
    atomic_inc(&siwifi_hw->rx_pending_count);
    wake_up_all(&siwifi_hw->rx_wq);
    return true;
}

#else
//make sure define as same with net/core/dev.c
#define SF_HNAT_DEV_OFFSET             0 // skb->dev pointer offset
#define SF_HNAT_FLAG                   47 // skb hw hnat finish flag
#define SF_CB_HNAT_FORWARD             22
// for skb search
/**
 * siwifi_rx_data_skb - Process one data frame
 *
 * @siwifi_hw: main driver data
 * @siwifi_vif: vif that received the buffer
 * @skb: skb received
 * @rxhdr: HW rx descriptor
 * @return: true if buffer has been forwarded to upper layer
 *
 * If buffer is amsdu , it is first split into a list of skb.
 * Then each skb may be:
 * - forwarded to upper layer
 * - resent on wireless interface
 *
 * When vif is a STA interface, every skb is only forwarded to upper layer.
 * When vif is an AP interface, multicast skb are forwarded and resent, whereas
 * skb for other BSS's STA are only resent.
 */
static bool siwifi_rx_data_skb(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif,
                            struct sk_buff *skb,  struct hw_rxhdr *rxhdr, struct sk_buff **amsdu_skb)
{
    struct sk_buff_head list;
    struct sk_buff *rx_skb;
    bool resend = false, forward = true;
    struct siwifi_sta *sta = NULL;
    struct siwifi_sta *dst_sta = NULL;
    uint32_t tx_dropped = 0;
    uint32_t tx_errors = 0;
    uint32_t forward_len = 0;
    uint32_t forward_num = 0;

    skb->dev = siwifi_vif->ndev;
    if (!siwifi_rx_get_skb_list(siwifi_hw, siwifi_vif,
                skb, rxhdr, amsdu_skb, &list))
        return false;

    if (((SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_AP) ||
         (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_AP_VLAN) ||
         (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_P2P_GO)) &&
        !(siwifi_vif->ap.flags & SIWIFI_AP_ISOLATE)) {
		const struct ethhdr *eth;
		rx_skb = skb_peek(&list);
		skb_reset_mac_header(rx_skb);
		eth = eth_hdr(rx_skb);

        if (unlikely(is_multicast_ether_addr(eth->h_dest))) {
            /* broadcast pkt need to be forwared to upper layer and resent
               on wireless interface */
            resend = true;
        } else {
            /* unicast pkt for STA inside the BSS, no need to forward to upper
               layer simply resend on wireless interface */
            if (rxhdr->flags_dst_idx < (NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX))
            {
                dst_sta = &siwifi_hw->sta_table[rxhdr->flags_dst_idx];
                if (dst_sta->valid && (dst_sta->vlan_idx == siwifi_vif->vif_index))
                {
                    forward = false;
                    resend = true;
                }
            }
        }
    } else if (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_MESH_POINT) {
        const struct ethhdr *eth;
        rx_skb = skb_peek(&list);
        skb_reset_mac_header(rx_skb);
        eth = eth_hdr(rx_skb);

        if (!is_multicast_ether_addr(eth->h_dest)) {
            /* unicast pkt for STA inside the BSS, no need to forward to upper
               layer simply resend on wireless interface */
            if (rxhdr->flags_dst_idx != SIWIFI_INVALID_STA)
            {
                forward = false;
                resend = true;
            }
        }
    }

    if (!sta && (rxhdr->flags_sta_idx < (NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX))) {
        sta = &siwifi_hw->sta_table[rxhdr->flags_sta_idx];
    }

    while (!skb_queue_empty(&list)) {
        rx_skb = __skb_dequeue(&list);

#ifdef CONFIG_SIWIFI_REPEATER
        if (siwifi_vif->rp_info) {
            if (repeater_rev_proc(siwifi_vif->rp_info, &rx_skb) < 0) {
                dev_kfree_skb(rx_skb);
                continue;
            }
        } else
#endif
        {
            if (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_STATION) {
                struct ethhdr *sf_eth = NULL;
                struct siwifi_src_filter *src_filter = NULL;
                skb_reset_mac_header(rx_skb);
                sf_eth = eth_hdr(rx_skb);
                src_filter = siwifi_src_filter_hash_search(siwifi_vif, sf_eth->h_source);
                if (src_filter) {
                    src_filter->drop_count++;
                    dev_kfree_skb(rx_skb);
                    continue;
                }
            }
        }

#ifdef CONFIG_VDR_HW
        if (sta != NULL && sta->valid) {
            vendor_hook_rxdata(sta, rx_skb, rxhdr);
        }
#endif
        /* resend pkt on wireless interface */
        if (resend) {
            struct sk_buff *skb_copy;
            int res;
            /* always need to copy buffer when forward=0 to get enough headrom for tsdesc */

#if 0
            skb_copy = skb_copy_expand(rx_skb, sizeof(struct siwifi_txhdr) +
                                       SIWIFI_SWTXHDR_ALIGN_SZ, 0, GFP_ATOMIC);
            if (!skb_copy) {
                netdev_err(siwifi_vif->ndev, "Failed to copy skb");
                goto FORWARD;
            }
#else
            if (forward) {
                //TODO, check the headroom
                skb_copy = skb_clone(rx_skb, GFP_ATOMIC);
                if (!skb_copy) {
                    netdev_err(siwifi_vif->ndev, "Failed to copy skb");
                    goto FORWARD;
                }
            } else {
                skb_copy = rx_skb;
            }
#endif
            skb_copy->protocol = htons(ETH_P_802_3);
            skb_reset_network_header(skb_copy);
            skb_reset_mac_header(skb_copy);

            if (siwifi_vif)
                siwifi_vif->is_resending = true;
            /*
             * in dev_queue_xmit,will go to siwifi_start_xmit,we use cb_lock in siwifi_start_xmit
             * so must unlock first,otherwise will dead lock
             * */
            spin_unlock(&siwifi_hw->cb_lock);
            res = dev_queue_xmit(skb_copy);
            spin_lock(&siwifi_hw->cb_lock);
            if (siwifi_vif)
                siwifi_vif->is_resending = false;

            if (res == NET_XMIT_DROP) {
                tx_dropped ++;
            } else if (res != NET_XMIT_SUCCESS) {
                tx_errors ++;
            }
        }
FORWARD:
        /* forward pkt to upper layer */
        if (forward) {
#if defined(CONFIG_SF19A28_FULLMASK) && IS_ENABLED(CONFIG_SFAX8_HNAT_DRIVER) && IS_ENABLED(CONFIG_NF_FLOW_TABLE)
            struct sf_hnat_priv *phnat_priv;
            struct platform_device *hnat_pdev;
#endif
            skb_reset_mac_header(rx_skb);
            /* eth_type_trans should execute before xmit_prepare because it will change skb->dev */
            memset(rx_skb->cb, 0, sizeof(rx_skb->cb));
            siwifi_trace_rx_in(siwifi_hw, sta, rx_skb);
            rx_skb->protocol = eth_type_trans(rx_skb, siwifi_vif->ndev);
#ifdef CONFIG_SIWIFI_IGMP
            siwifi_check_igmp(siwifi_hw, rx_skb);
#endif
#if defined(CONFIG_SF19A28_FULLMASK) && IS_ENABLED(CONFIG_SFAX8_HNAT_DRIVER) && IS_ENABLED(CONFIG_NF_FLOW_TABLE)
            phnat_priv = siwifi_vif->phnat_priv;
            hnat_pdev = siwifi_vif->hnat_pdev;
            if (phnat_priv) {
                phnat_priv->search(hnat_pdev, rx_skb);
            }
#endif
            spin_unlock(&siwifi_hw->cb_lock);
            REG_SW_SET_PROFILING(siwifi_hw, SW_PROF_IEEE80211RX);
            forward_num ++;
            forward_len += rx_skb->len;

#if defined(CONFIG_SF19A28_FULLMASK) && IS_ENABLED(CONFIG_SFAX8_HNAT_DRIVER) && IS_ENABLED(CONFIG_NF_FLOW_TABLE)
            //direct forward to dst device
            if(rx_skb->cb[SF_HNAT_FLAG] == SF_CB_HNAT_FORWARD){
                rx_skb->dev = (struct net_device *)(*((unsigned int*)(rx_skb->cb + SF_HNAT_DEV_OFFSET)));
                skb_push(rx_skb, ETH_HLEN);
                rx_skb->dev->netdev_ops->ndo_start_xmit(rx_skb, rx_skb->dev);
            }else
#endif
            {
                netif_receive_skb(rx_skb);
            }
            //netif_receive_skb(rx_skb);
            REG_SW_CLEAR_PROFILING(siwifi_hw, SW_PROF_IEEE80211RX);
			spin_lock(&siwifi_hw->cb_lock);
		}
        siwifi_hw->total_rx ++;
        siwifi_vif->total_rx ++;
	}
    // update statiscs
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    siwifi_hw->stats.total_rx += forward_num;
#endif
    if (siwifi_vif) {
        if (tx_dropped) {
            siwifi_vif->net_stats.rx_dropped += tx_dropped;
            siwifi_vif->net_stats.tx_dropped += tx_dropped;
        }
        if (tx_errors) {
            siwifi_vif->net_stats.tx_errors += tx_errors;
        }

        siwifi_vif->net_stats.rx_packets += forward_num;
        siwifi_vif->net_stats.rx_bytes += forward_len;
        if (sta->valid) {
            sta->stats.rx_packets += forward_num;
            sta->stats.rx_bytes += forward_len;
            if (tx_dropped) {
                sta->stats.rx_dropped += tx_dropped;
                if(dst_sta)
                    dst_sta->stats.tx_dropped += tx_dropped;
            }
            if (tx_errors && dst_sta)
                dst_sta->stats.tx_failed += tx_errors;
        }
    }

    return (forward || resend);
}
#endif

#ifdef CONFIG_SIWIFI_EASYMESH
/**
 * siwifi_easymesh_mgmt_rx_hook - Function to handle management frame reception events and report them as WiFi notify
 *                                events within the siwifi EasyMesh module.
 *
 * @param mgmt: Pointer to the received IEEE 802.11 management frame.
 * @param skb: Pointer to the socket buffer containing the management frame data.
 *
 * This function checks the received management frame type and, if matched, reports the event as a WiFi notify event.
 */
static void siwifi_easymesh_mgmt_rx_hook(struct ieee80211_mgmt *mgmt, struct sk_buff *skb)
{
    /* Declare and initialize an instance of sf_wifi_event_data. */
    sf_wifi_event_data event_data;
    bool matched = false;

    /* Check if the received frame is an Association Request. */
    if (ieee80211_is_assoc_req(mgmt->frame_control)) {
        event_data.data.mgmt_rx_event.frame_type = SF_TUNNELED_MSG_ASSOC_REQ;
        matched = true;
    } else if (ieee80211_is_reassoc_req(mgmt->frame_control)) {
        event_data.data.mgmt_rx_event.frame_type = SF_TUNNELED_MSG_REASSOC_REQ;
        matched = true;
    } else {
        /* Check if the received frame is an Action frame. */
        if (ieee80211_is_action(mgmt->frame_control)) {
            /**
             * "Because there is no corresponding union for WNM and PUBLIC action frames within ieee80211_mgmt, we use
             * `wme_action` as a substitute to obtain the `action_code`."
             */
            u8 action_code = mgmt->u.action.u.wme_action.action_code;
            switch (mgmt->u.action.category) {
                case WLAN_CATEGORY_WNM:
                    if (action_code == WLAN_WNM_NOTIFY_REQ) {
                        event_data.data.mgmt_rx_event.frame_type = SF_TUNNELED_MSG_WNM_REQ;
                        matched = true;
                    } else if (action_code == WLAN_WNM_BTM_QUERY) {
                        event_data.data.mgmt_rx_event.frame_type = SF_TUNNELED_MSG_BTM_QUERY;
                        matched = true;
                    } else if (action_code == WLAN_WNM_BTM_RESPONSE) {
                        event_data.data.mgmt_rx_event.frame_type = SF_TUNNELED_MSG_BTM_RESPONSE;
                        matched = true;
                    }
                    break;
                case WLAN_CATEGORY_PUBLIC:
                    if (action_code == WLAN_PA_GAS_INITIAL_REQ) {
                        event_data.data.mgmt_rx_event.frame_type = SF_TUNNELED_MSG_ANQP_NEIGHBOR_REQ;
                        matched = true;
                    }
                    break;
                case WLAN_CATEGORY_RADIO_MEASUREMENT:
                    action_code = mgmt->u.action.u.measurement.action_code;
                    if (action_code == RADIO_MEASUREMENT_REPORT) {
                        event_data.data.mgmt_rx_event.frame_type = SF_TUNNELED_MSG_RADIO_MEASUREMENT_REPORT;
                        matched = true;
                    }
                default:
                    break;
            }
        }
    }

    /* If a match was found, report the event to EasyMesh. */
    if (matched) {
        /* Safely copy management frame data. */
        event_data.type = SF_NOTIFY_WIFI_MGMT_RX_EVENT;
        event_data.data.mgmt_rx_event.frame_length = skb->len;

        /* Allocate memory for frame data copy. */
        event_data.data.mgmt_rx_event.frame_data = kmalloc(skb->len, GFP_ATOMIC);
        if (!event_data.data.mgmt_rx_event.frame_data) {
            printk("Failed to allocate memory for management frame data.\n");
            return;
        }

        /* Copy frame data to allocated memory. */
        if (skb_copy_bits(skb, 0, (void *)event_data.data.mgmt_rx_event.frame_data, skb->len) != 0) {
            printk("Failed to copy management frame data.\n");
            kfree(event_data.data.mgmt_rx_event.frame_data);
            return;
        }

        /* Report the management frame reception event. */
        siwifi_report_event_to_easymesh(&event_data);

        /* Free allocated memory for frame data copy. */
        kfree(event_data.data.mgmt_rx_event.frame_data);
    }
}
#endif /* CONFIG_SIWIFI_EASYMESH */

/**
 * siwifi_rx_mgmt - Process one 802.11 management frame
 *
 * @siwifi_hw: main driver data
 * @siwifi_vif: vif to upload the buffer to
 * @skb: skb received
 * @rxhdr: HW rx descriptor
 *
 * Forward the management frame to a given interface.
 */
static void siwifi_rx_mgmt(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif,
                         struct sk_buff *skb,  struct hw_rxhdr *hw_rxhdr)
{
    struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)skb->data;
    struct rx_vector_1 *rxvect = &hw_rxhdr->hwvect.rx_vect1;

    siwifi_trace_mgmt_rx_in(siwifi_hw, skb);

#ifdef CONFIG_SIWIFI_EASYMESH
    /* Check if the connection should be blocked. */
    if (siwifi_check_connection_block(siwifi_hw, mgmt))
        return;

    /* Check if the STA is blocked on the current BSS. */
    /* If the STA is blocked, return immediately and do not process further. */
    if (siwifi_easymesh_is_sta_blocked(siwifi_hw, mgmt->sa, mgmt->bssid))
        return;

    /* Call the EasyMesh management frame receive hook. */

    siwifi_easymesh_mgmt_rx_hook(mgmt, skb);
#endif

    if (ieee80211_is_deauth(mgmt->frame_control) ||
                ieee80211_is_disassoc(mgmt->frame_control)){
       printk("rx pkt(%d) %s from [%02x:%02x:%02x:%02x:%02x:%02x] reasoncode: %d\n", siwifi_hw->mod_params->is_hb, ieee80211_is_deauth(mgmt->frame_control) ? "deauth" : "disassoc",
               mgmt->sa[0], mgmt->sa[1], mgmt->sa[2], mgmt->sa[3], mgmt->sa[4], mgmt->sa[5], mgmt->u.deauth.reason_code);
    }
    if (ieee80211_is_beacon(mgmt->frame_control)) {
        if ((SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_MESH_POINT) &&
            hw_rxhdr->flags_new_peer) {
            cfg80211_notify_new_peer_candidate(siwifi_vif->ndev, mgmt->sa,
                                               mgmt->u.beacon.variable,
                                               skb->len - offsetof(struct ieee80211_mgmt,
                                                                   u.beacon.variable),
                                               GFP_ATOMIC);
        } else {
            cfg80211_report_obss_beacon(siwifi_hw->wiphy, skb->data, skb->len,
                                        hw_rxhdr->phy_info.phy_prim20_freq,
                                        rxvect->rssi1);
        }
        siwifi_vif->mgmt_stats.rx_bcn_cnts++;
    } else if ((ieee80211_is_deauth(mgmt->frame_control) ||
                ieee80211_is_disassoc(mgmt->frame_control)) &&
               (mgmt->u.deauth.reason_code == WLAN_REASON_CLASS2_FRAME_FROM_NONAUTH_STA ||
                mgmt->u.deauth.reason_code == WLAN_REASON_CLASS3_FRAME_FROM_NONASSOC_STA)) {
        cfg80211_rx_unprot_mlme_mgmt(siwifi_vif->ndev, skb->data, skb->len);
        siwifi_vif->mgmt_stats.rx_non_cnts++;
    } else if ((SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_STATION) &&
               (ieee80211_is_action(mgmt->frame_control) &&
                (mgmt->u.action.category == 6))) {
        struct cfg80211_ft_event_params ft_event;
        ft_event.target_ap = (uint8_t *)&mgmt->u.action + ETH_ALEN + 2;
        ft_event.ies = (uint8_t *)&mgmt->u.action + ETH_ALEN * 2 + 2;
        ft_event.ies_len = skb->len - (ft_event.ies - (uint8_t *)mgmt);
        ft_event.ric_ies = NULL;
        ft_event.ric_ies_len = 0;
        cfg80211_ft_event(siwifi_vif->ndev, &ft_event);
        siwifi_vif->mgmt_stats.rx_ft_cnts++;
    } else {
        cfg80211_rx_mgmt(&siwifi_vif->wdev, hw_rxhdr->phy_info.phy_prim20_freq,
                         rxvect->rssi1, skb->data, skb->len, 0);
        siwifi_vif->mgmt_stats.rx_mgmt_cnts++;
    }
}

/**
 * siwifi_rx_mgmt_any - Process one 802.11 management frame
 *
 * @siwifi_hw: main driver data
 * @skb: skb received
 * @rxhdr: HW rx descriptor
 *
 * Process the management frame and free the corresponding skb.
 * If vif is not specified in the rx descriptor, the the frame is uploaded
 * on all active vifs.
 */
static void siwifi_rx_mgmt_any(struct siwifi_hw *siwifi_hw, struct sk_buff *skb,
                             struct hw_rxhdr *hw_rxhdr)
{
    struct siwifi_vif *siwifi_vif;
    int vif_idx = hw_rxhdr->flags_vif_idx;

    trace_mgmt_rx(hw_rxhdr->phy_info.phy_prim20_freq, vif_idx,
                  hw_rxhdr->flags_sta_idx, (struct ieee80211_mgmt *)skb->data);

    if (vif_idx == SIWIFI_INVALID_VIF) {
        list_for_each_entry(siwifi_vif, &siwifi_hw->vifs, list) {
            if (! siwifi_vif->up)
                continue;
            siwifi_rx_mgmt(siwifi_hw, siwifi_vif, skb, hw_rxhdr);
        }
    } else {
        siwifi_vif = siwifi_rx_get_vif(siwifi_hw, vif_idx);
        if (siwifi_vif)
            siwifi_rx_mgmt(siwifi_hw, siwifi_vif, skb, hw_rxhdr);
    }

    dev_kfree_skb(skb);
}

/**
 * siwifi_rx_rtap_hdrlen - Return radiotap header length
 *
 * @rxvect: Rx vector used to fill the radiotap header
 * @has_vend_rtap: boolean indicating if vendor specific data is present
 *
 * Compute the length of the radiotap header based on @rxvect and vendor
 * specific data (if any).
 */
static u8 siwifi_rx_rtap_hdrlen(struct rx_vector_1 *rxvect,
                              bool has_vend_rtap)
{
    u8 rtap_len;

    /* Compute radiotap header length */
    rtap_len = sizeof(struct ieee80211_radiotap_header) + 8;

    // Check for multiple antennas
    if (hweight32(rxvect->antenna_set) > 1)
        // antenna and antenna signal fields
        rtap_len += 4 * hweight8(rxvect->antenna_set);

    // TSFT
    if (!has_vend_rtap) {
        rtap_len = ALIGN(rtap_len, 8);
        rtap_len += 8;
    }

    // IEEE80211_HW_SIGNAL_DBM
    rtap_len++;

    // Check if single antenna
    if (hweight32(rxvect->antenna_set) == 1)
        rtap_len++; //Single antenna

    // padding for RX FLAGS
    rtap_len = ALIGN(rtap_len, 2);

    // Check for HT frames
    if ((rxvect->format_mod == FORMATMOD_HT_MF) ||
        (rxvect->format_mod == FORMATMOD_HT_GF))
        rtap_len += 3;

    // Check for AMPDU
#ifdef CONFIG_SFA28_FULLMASK
    if (!(has_vend_rtap) && ((rxvect->format_mod >= FORMATMOD_VHT) ||
                             ((rxvect->format_mod > FORMATMOD_NON_HT_DUP_OFDM) &&
                                                     (rxvect->aggregation)))) {
        rtap_len = ALIGN(rtap_len, 4);
        rtap_len += 8;
    }
#else
    if (!(has_vend_rtap) && ((rxvect->format_mod >= FORMATMOD_VHT) ||
                             ((rxvect->format_mod > FORMATMOD_NON_HT_DUP_OFDM) &&
                                                     (rxvect->ht.aggregation)))) {
        rtap_len = ALIGN(rtap_len, 4);
        rtap_len += 8;
    }
#endif

    // Check for VHT frames
    if (rxvect->format_mod == FORMATMOD_VHT) {
        rtap_len = ALIGN(rtap_len, 2);
        rtap_len += 12;
    }

    // Check for HE frames
    if (rxvect->format_mod == FORMATMOD_HE_SU) {
        rtap_len = ALIGN(rtap_len, 2);
        rtap_len += sizeof(struct ieee80211_radiotap_he);
    }

    // Check for multiple antennas
    if (hweight32(rxvect->antenna_set) > 1) {
        // antenna and antenna signal fields
        rtap_len += 2 * hweight8(rxvect->antenna_set);
    }

    // Check for vendor specific data
    if (has_vend_rtap) {
        /* vendor presence bitmap */
        rtap_len += 4;
        /* alignment for fixed 6-byte vendor data header */
        rtap_len = ALIGN(rtap_len, 2);
    }

    return rtap_len;
}

/**
 * siwifi_rx_add_rtap_hdr - Add radiotap header to sk_buff
 *
 * @siwifi_hw: main driver data
 * @skb: skb received (will include the radiotap header)
 * @rxvect: Rx vector
 * @phy_info: Information regarding the phy
 * @hwvect: HW Info (NULL if vendor specific data is available)
 * @rtap_len: Length of the radiotap header
 * @vend_rtap_len: radiotap vendor length (0 if not present)
 * @vend_it_present: radiotap vendor present
 *
 * Builds a radiotap header and add it to @skb.
 */
static void siwifi_rx_add_rtap_hdr(struct siwifi_hw* siwifi_hw,
                                 struct sk_buff *skb,
                                 struct rx_vector_1 *rxvect,
                                 struct phy_channel_info_desc *phy_info,
                                 struct hw_vect *hwvect,
                                 int rtap_len,
                                 u8 vend_rtap_len,
                                 u32 vend_it_present)
{
    struct ieee80211_radiotap_header *rtap;
    u8 *pos, rate_idx;
    __le32 *it_present;
    u32 it_present_val = 0;
    bool fec_coding = false;
    bool short_gi = false;
    bool stbc = false;
    bool aggregation = false;

    rtap = (struct ieee80211_radiotap_header *)skb_push(skb, rtap_len);
    memset((u8*) rtap, 0, rtap_len);

    rtap->it_version = 0;
    rtap->it_pad = 0;
    rtap->it_len = cpu_to_le16(rtap_len + vend_rtap_len);

    it_present = &rtap->it_present;

    // Check for multiple antennas
    if (hweight32(rxvect->antenna_set) > 1) {
        int chain;
        unsigned long chains = rxvect->antenna_set;

        for_each_set_bit(chain, &chains, IEEE80211_MAX_CHAINS) {
            it_present_val |=
                BIT(IEEE80211_RADIOTAP_EXT) |
                BIT(IEEE80211_RADIOTAP_RADIOTAP_NAMESPACE);
            put_unaligned_le32(it_present_val, it_present);
            it_present++;
            it_present_val = BIT(IEEE80211_RADIOTAP_ANTENNA) |
                             BIT(IEEE80211_RADIOTAP_DBM_ANTSIGNAL);
        }
    }

    // Check if vendor specific data is present
    if (vend_rtap_len) {
        it_present_val |= BIT(IEEE80211_RADIOTAP_VENDOR_NAMESPACE) |
                          BIT(IEEE80211_RADIOTAP_EXT);
        put_unaligned_le32(it_present_val, it_present);
        it_present++;
        it_present_val = vend_it_present;
    }

    put_unaligned_le32(it_present_val, it_present);
    pos = (void *)(it_present + 1);

    // IEEE80211_RADIOTAP_TSFT
    if (hwvect) {
        rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_TSFT);
        // padding
        while ((pos - (u8 *)rtap) & 7)
            *pos++ = 0;
        put_unaligned_le64((((u64)le32_to_cpu(hwvect->tsf_hi) << 32) +
                            (u64)le32_to_cpu(hwvect->tsf_lo)), pos);
        pos += 8;
    }

    // IEEE80211_RADIOTAP_FLAGS
    rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_FLAGS);
    if (hwvect && (!hwvect->frm_successful_rx))
        *pos |= IEEE80211_RADIOTAP_F_BADFCS;
    if (!rxvect->pre_type
            && (rxvect->format_mod <= FORMATMOD_NON_HT_DUP_OFDM))
        *pos |= IEEE80211_RADIOTAP_F_SHORTPRE;
    pos++;

    // IEEE80211_RADIOTAP_RATE
    // check for HT, VHT or HE frames
    if (rxvect->format_mod >= FORMATMOD_HE_SU) {
#ifdef CONFIG_SFA28_FULLMASK
        // just for compile, on A28 platform, we did not support HE
        rate_idx = rxvect->mcs;
        fec_coding = rxvect->fec_coding;
        stbc = rxvect->stbc;
#else
        rate_idx = rxvect->he.mcs;
        fec_coding = rxvect->he.fec;
        stbc = rxvect->he.stbc;
#endif
        aggregation = true;
        *pos = 0;
    } else if (rxvect->format_mod == FORMATMOD_VHT) {
#ifdef CONFIG_SFA28_FULLMASK
        rate_idx = rxvect->mcs & 0x0F;
        fec_coding = rxvect->fec_coding;
        short_gi = rxvect->short_gi;
        stbc = rxvect->stbc;
#else
        rate_idx = rxvect->vht.mcs;
        fec_coding = rxvect->vht.fec;
        short_gi = rxvect->vht.short_gi;
        stbc = rxvect->vht.stbc;
#endif
        aggregation = true;
        *pos = 0;
    } else if (rxvect->format_mod > FORMATMOD_NON_HT_DUP_OFDM) {
#ifdef CONFIG_SFA28_FULLMASK
        rate_idx = rxvect->mcs;
        fec_coding = rxvect->fec_coding;
        short_gi = rxvect->short_gi;
        stbc = rxvect->stbc;
        aggregation = rxvect->aggregation;
#else
        rate_idx = rxvect->ht.mcs;
        fec_coding = rxvect->ht.fec;
        short_gi = rxvect->ht.short_gi;
        stbc = rxvect->ht.stbc;
        aggregation = rxvect->ht.aggregation;
#endif
        *pos = 0;
    } else {
        struct ieee80211_supported_band* band =
                siwifi_hw->wiphy->bands[phy_info->phy_band];
        rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_RATE);
        BUG_ON((rate_idx = legrates_lut[rxvect->leg_rate]) == -1);
        if (phy_info->phy_band == NL80211_BAND_5GHZ)
            rate_idx -= 4;  /* siwifi_ratetable_5ghz[0].hw_value == 4 */
        *pos = DIV_ROUND_UP(band->bitrates[rate_idx].bitrate, 5);
    }
    pos++;

    // IEEE80211_RADIOTAP_CHANNEL
    rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_CHANNEL);
    put_unaligned_le16(phy_info->phy_prim20_freq, pos);
    pos += 2;

    if (phy_info->phy_band == NL80211_BAND_5GHZ)
        put_unaligned_le16(IEEE80211_CHAN_OFDM | IEEE80211_CHAN_5GHZ, pos);
    else if (rxvect->format_mod > FORMATMOD_NON_HT_DUP_OFDM)
        put_unaligned_le16(IEEE80211_CHAN_DYN | IEEE80211_CHAN_2GHZ, pos);
    else
        put_unaligned_le16(IEEE80211_CHAN_CCK | IEEE80211_CHAN_2GHZ, pos);
    pos += 2;

    if (hweight32(rxvect->antenna_set) == 1) {
        // IEEE80211_RADIOTAP_DBM_ANTSIGNAL
        rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_DBM_ANTSIGNAL);
        *pos++ = rxvect->rssi1;

        // IEEE80211_RADIOTAP_ANTENNA
        rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_ANTENNA);
        *pos++ = rxvect->antenna_set;
    }

    // IEEE80211_RADIOTAP_LOCK_QUALITY is missing
    // IEEE80211_RADIOTAP_DB_ANTNOISE is missing

    // IEEE80211_RADIOTAP_RX_FLAGS
    rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_RX_FLAGS);
    // 2 byte alignment
    if ((pos - (u8 *)rtap) & 1)
        *pos++ = 0;
    put_unaligned_le16(0, pos);
    //Right now, we only support fcs error (no RX_FLAG_FAILED_PLCP_CRC)
    pos += 2;

    // Check if HT
    if ((rxvect->format_mod == FORMATMOD_HT_MF)
            || (rxvect->format_mod == FORMATMOD_HT_GF)) {
        rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_MCS);
        *pos++ = IEEE80211_RADIOTAP_MCS_HAVE_MCS |
                 IEEE80211_RADIOTAP_MCS_HAVE_GI |
                 IEEE80211_RADIOTAP_MCS_HAVE_BW;
        *pos = 0;
        if (short_gi)
            *pos |= IEEE80211_RADIOTAP_MCS_SGI;
        if (rxvect->ch_bw  == __CHBW_CBW40)
            *pos |= IEEE80211_RADIOTAP_MCS_BW_40;
        if (rxvect->format_mod == FORMATMOD_HT_GF)
            *pos |= IEEE80211_RADIOTAP_MCS_FMT_GF;
        if (fec_coding)
            *pos |= IEEE80211_RADIOTAP_MCS_FEC_LDPC;
        *pos++ |= stbc << IEEE80211_RADIOTAP_MCS_STBC_SHIFT;
        *pos++ = rate_idx;
    }

    // check for HT or VHT frames
    if (aggregation && hwvect) {
        // 4 byte alignment
        while ((pos - (u8 *)rtap) & 3)
            pos++;
        rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_AMPDU_STATUS);
        put_unaligned_le32(hwvect->ampdu_cnt, pos);
        pos += 4;
        put_unaligned_le32(0, pos);
        pos += 4;
    }

    // Check for VHT frames
    if (rxvect->format_mod == FORMATMOD_VHT) {
        u16 vht_details = IEEE80211_RADIOTAP_VHT_KNOWN_GI |
                          IEEE80211_RADIOTAP_VHT_KNOWN_BANDWIDTH;
#ifdef CONFIG_SFA28_FULLMASK
        u8 vht_nss = (rxvect->stbc ? rxvect->n_sts/2 : rxvect->n_sts) + 1;
#else
        u8 vht_nss = rxvect->vht.nss + 1;
#endif

        rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_VHT);

        if ((rxvect->ch_bw == __CHBW_CBW160)
                && phy_info->phy_center2_freq)
            vht_details &= ~IEEE80211_RADIOTAP_VHT_KNOWN_BANDWIDTH;
        put_unaligned_le16(vht_details, pos);
        pos += 2;

        // flags
        if (short_gi)
            *pos |= IEEE80211_RADIOTAP_VHT_FLAG_SGI;
        if (stbc)
            *pos |= IEEE80211_RADIOTAP_VHT_FLAG_STBC;
        pos++;

        // bandwidth
        if (rxvect->ch_bw == __CHBW_CBW40)
            *pos++ = 1;
        if (rxvect->ch_bw == __CHBW_CBW80)
            *pos++ = 4;
        else if ((rxvect->ch_bw == __CHBW_CBW160)
                && phy_info->phy_center2_freq)
            *pos++ = 0; //80P80
        else if  (rxvect->ch_bw == __CHBW_CBW160)
            *pos++ = 11;
        else // 20 MHz
            *pos++ = 0;

        // MCS/NSS
        *pos = (rate_idx << 4) | vht_nss;
        pos += 4;
        if (fec_coding)
            *pos |= IEEE80211_RADIOTAP_CODING_LDPC_USER0;
        pos++;
        // group ID
        pos++;
        // partial_aid
        pos += 2;
    }

    // Rx Chains
    if (hweight32(rxvect->antenna_set) > 1) {
        int chain;
        unsigned long chains = rxvect->antenna_set;
        u8 rssis[4] = {rxvect->rssi1, rxvect->rssi1, rxvect->rssi1, rxvect->rssi1};

        for_each_set_bit(chain, &chains, IEEE80211_MAX_CHAINS) {
            *pos++ = rssis[chain];
            *pos++ = chain;
        }
    }
}

/**
 * siwifi_rx_monitor - Build radiotap header for skb an send it to netdev
 *
 * @siwifi_hw: main driver data
 * @siwifi_vif: vif that received the buffer
 * @skb: sk_buff received
 * @hw_rxhdr_ptr: Pointer to HW RX header
 * @rtap_len: Radiotap Header length
 *
 * Add radiotap header to the receved skb and send it to netdev
 */
static int siwifi_rx_monitor(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif,
                           struct sk_buff *skb,  struct hw_rxhdr *hw_rxhdr_ptr,
                           u8 rtap_len)
{
    skb->dev = siwifi_vif->ndev;

    if (siwifi_vif->wdev.iftype != NL80211_IFTYPE_MONITOR) {
        netdev_err(siwifi_vif->ndev, "not a monitor vif\n");
        return -1;
    }

    /* Add RadioTap Header */
    siwifi_rx_add_rtap_hdr(siwifi_hw, skb, &hw_rxhdr_ptr->hwvect.rx_vect1,
                         &hw_rxhdr_ptr->phy_info, &hw_rxhdr_ptr->hwvect,
                         rtap_len, 0, 0);

    skb_reset_mac_header(skb);
    skb->ip_summed = CHECKSUM_UNNECESSARY;
    skb->pkt_type = PACKET_OTHERHOST;
    skb->protocol = htons(ETH_P_802_2);

    netif_receive_skb(skb);

    return 0;
}

/**
 * siwifi_unsup_rx_vec_ind() - IRQ handler callback for %IPC_IRQ_E2A_UNSUP_RX_VEC
 *
 * LMAC has triggered an IT saying that a rx vector of an unsupported frame has been
 * captured and sent to upper layer. Then we need to fill the rx status, create a vendor
 * specific header and fill it with the HT packet length. Finally, we need to specify at
 * least 2 bytes of data and send the sk_buff to mac80211.
 *
 * @pthis: Pointer to main driver data
 * @hostid: Pointer to IPC elem from e2aradars_pool
 */
u8 siwifi_unsup_rx_vec_ind(void *pthis, void *hostid) {
    struct siwifi_hw *siwifi_hw = pthis;
    struct siwifi_ipc_skb_elem *elem = hostid;
    struct rx_vector_desc *rx_desc;
    struct sk_buff *skb;
    struct rx_vector_1 *rx_vect1;
    struct phy_channel_info_desc *phy_info;
    struct vendor_radiotap_hdr *rtap;
    u16 ht_length;
    struct siwifi_vif *siwifi_vif;
    struct rx_vector_desc rx_vect_desc;
    u8 rtap_len, vend_rtap_len = sizeof(*rtap);

    dma_sync_single_for_cpu(siwifi_hw->dev, elem->dma_addr,
                            sizeof(struct rx_vector_desc), DMA_FROM_DEVICE);

    skb = elem->skb;
    if (((struct rx_vector_desc *) (skb->data))->pattern == 0) {
        /*sync is needed even if the driver did not modify the memory*/
        dma_sync_single_for_device(siwifi_hw->dev, elem->dma_addr,
                                     sizeof(struct rx_vector_desc), DMA_FROM_DEVICE);
        return -1;
    }

    if (siwifi_hw->monitor_vif == SIWIFI_INVALID_VIF) {
        /* Unmap will synchronize buffer for CPU */
        dma_unmap_single(siwifi_hw->dev, elem->dma_addr, siwifi_hw->ipc_env->unsuprxvec_bufsz,
                         DMA_FROM_DEVICE);
        elem->skb = NULL;

        /* Free skb */
        dev_kfree_skb(skb);

        /* Allocate and push a new buffer to fw to replace this one */
        if (siwifi_ipc_unsup_rx_vec_elem_allocs(siwifi_hw, elem))
            dev_err(siwifi_hw->dev, "Failed to alloc new unsupported rx vector buf\n");
        return -1;
    }
#if DEBUG_ARRAY_CHECK
    BUG_ON(siwifi_hw->monitor_vif >= NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX);
#endif
    siwifi_vif = siwifi_hw->vif_table[siwifi_hw->monitor_vif];
    skb->dev = siwifi_vif->ndev;
    memcpy(&rx_vect_desc, skb->data, sizeof(rx_vect_desc));
    rx_desc = &rx_vect_desc;

    rx_vect1 = (struct rx_vector_1 *) (rx_desc->rx_vect1);
    phy_info = (struct phy_channel_info_desc *) (&rx_desc->phy_info);
    if (rx_vect1->format_mod >= FORMATMOD_VHT)
        ht_length = 0;
    else
#ifdef CONFIG_SFA28_FULLMASK
        ht_length = (u16) le32_to_cpu(rx_vect1->ht_length);
#endif

    // Reserve space for radiotap
    skb_reserve(skb, RADIOTAP_HDR_MAX_LEN);

    /* Fill vendor specific header with fake values */
    rtap = (struct vendor_radiotap_hdr *) skb->data;
    rtap->oui[0] = 0x00;
    rtap->oui[1] = 0x25;
    rtap->oui[2] = 0x3A;
    rtap->subns  = 0;
    rtap->len = sizeof(ht_length);
    put_unaligned_le16(ht_length, rtap->data);
    vend_rtap_len += rtap->len;
    skb_put(skb, vend_rtap_len);

    /* Copy fake data */
    put_unaligned_le16(0, skb->data + vend_rtap_len);
    skb_put(skb, UNSUP_RX_VEC_DATA_LEN);

    /* Get RadioTap Header length */
    rtap_len = siwifi_rx_rtap_hdrlen(rx_vect1, true);

    /* Check headroom space */
    if (skb_headroom(skb) < rtap_len) {
        netdev_err(siwifi_vif->ndev, "not enough headroom %d need %d\n", skb_headroom(skb), rtap_len);
        dma_unmap_single(siwifi_hw->dev, elem->dma_addr, siwifi_hw->ipc_env->unsuprxvec_bufsz,
                DMA_FROM_DEVICE);
        elem->skb = NULL;
        dev_kfree_skb(skb);

        /* Allocate and push a new buffer to fw to replace this one */
        if (siwifi_ipc_unsup_rx_vec_elem_allocs(siwifi_hw, elem))
            dev_err(siwifi_hw->dev, "Failed to alloc new unsupported rx vector buf\n");

        return -1;
    }

    /* Add RadioTap Header */
    siwifi_rx_add_rtap_hdr(siwifi_hw, skb, rx_vect1, phy_info, NULL,
                         rtap_len, vend_rtap_len, BIT(0));

    skb_reset_mac_header(skb);
    skb->ip_summed = CHECKSUM_UNNECESSARY;
    skb->pkt_type = PACKET_OTHERHOST;
    skb->protocol = htons(ETH_P_802_2);

    /* Unmap will synchronize buffer for CPU */
    dma_unmap_single(siwifi_hw->dev, elem->dma_addr, siwifi_hw->ipc_env->unsuprxvec_bufsz,
                     DMA_FROM_DEVICE);
    elem->skb = NULL;

    netif_receive_skb(skb);

    /* Allocate and push a new buffer to fw to replace this one */
    if (siwifi_ipc_unsup_rx_vec_elem_allocs(siwifi_hw, elem))
        netdev_err(siwifi_vif->ndev, "Failed to alloc new unsupported rx vector buf\n");
    return 0;
}


static void siwifi_rx_monitor_skb(struct siwifi_hw *siwifi_hw, struct sk_buff *skb, u16_l status, int msdu_offset)
{
    struct sk_buff *skb_monitor;
    struct hw_rxhdr hw_rxhdr_copy;
    struct hw_rxhdr *hw_rxhdr;
    struct siwifi_vif *siwifi_vif;
    u8 rtap_len;
    u16 frm_len;
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    siwifi_hw->stats.monitor_rx ++;
#endif
    //Check if monitor interface exists and is open
    siwifi_vif = siwifi_rx_get_vif(siwifi_hw, siwifi_hw->monitor_vif);
    if (!siwifi_vif) {
        //dev_err(siwifi_hw->dev, "Received monitor frame but there is no monitor interface open\n");
        siwifi_hw->stats.rx_monitor_interface_close ++;
        return ;
    }

    hw_rxhdr = (struct hw_rxhdr *)skb->data;
    rtap_len = siwifi_rx_rtap_hdrlen(&hw_rxhdr->hwvect.rx_vect1, false);

    // Move skb->data pointer to MAC Header or Ethernet header
    skb->data += msdu_offset;

    //Save frame length
    frm_len = le32_to_cpu(hw_rxhdr->hwvect.len);

    // Reserve space for frame
    skb->len = frm_len;

    if (status == RX_STAT_MONITOR) {
        /* Remove the SK buffer from the rxbuf_elems table. It will also
           unmap the buffer and then sync the buffer for the cpu */
        siwifi_ipc_rxbuf_elem_pull(siwifi_hw, skb);

        //Check if there is enough space to add the radiotap header
        if (skb_headroom(skb) > rtap_len) {

            skb_monitor = skb;

            //Duplicate the HW Rx Header to override with the radiotap header
            memcpy(&hw_rxhdr_copy, hw_rxhdr, sizeof(hw_rxhdr_copy));

            hw_rxhdr = &hw_rxhdr_copy;
        } else {
            //Duplicate the skb and extend the headroom
            skb_monitor = skb_copy_expand(skb, rtap_len, 0, GFP_ATOMIC);

            //Reset original skb->data pointer
            skb->data = (void*) hw_rxhdr;
        }
    }
    else
    {
#ifdef CONFIG_SIWIFI_MON_DATA
        // Check if MSDU
        // TODO has a bug when buffer contain amsdu
        if (!hw_rxhdr->flags_is_80211_mpdu) {
            // MSDU
            // Extract MAC header
            u16 machdr_len = hw_rxhdr->mac_hdr_backup.buf_len;
            u8* machdr_ptr = hw_rxhdr->mac_hdr_backup.buffer;

            //Pull Ethernet header from skb
            skb_pull(skb, sizeof(struct ethhdr));

            // Copy skb and extend for adding the radiotap header and the MAC header
            skb_monitor = skb_copy_expand(skb,
                    rtap_len + machdr_len,
                    0, GFP_ATOMIC);

            //Reserve space for the MAC Header
            skb_push(skb_monitor, machdr_len);

            //Copy MAC Header
            memcpy(skb_monitor->data, machdr_ptr, machdr_len);

            //Update frame length
            frm_len += machdr_len - sizeof(struct ethhdr);
        } else {
            // MPDU
            skb_monitor = skb_copy_expand(skb, rtap_len, 0, GFP_ATOMIC);
        }

        //Reset original skb->data pointer
        skb->data = (void*) hw_rxhdr;
#else
        //Reset original skb->data pointer
        skb->data = (void*) hw_rxhdr;

        wiphy_err(siwifi_hw->wiphy, "RX status %d is invalid when MON_DATA is disabled\n", status);
        return ;
#endif
    }

    skb_reset_tail_pointer(skb);
    skb->len = 0;
    skb_reset_tail_pointer(skb_monitor);
    skb_monitor->len = 0;

    skb_put(skb_monitor, frm_len);
    if (siwifi_rx_monitor(siwifi_hw, siwifi_vif, skb_monitor, hw_rxhdr, rtap_len))
        dev_kfree_skb(skb_monitor);

    if (status == RX_STAT_MONITOR) {
        if (skb_monitor != skb) {
            dev_kfree_skb(skb);
        }
    }
}

/**
 * siwifi_rxdataind - Process rx buffer
 *
 * @pthis: Pointer to the object attached to the IPC structure
 *         (points to struct siwifi_hw is this case)
 * @hostid: Address of the RX descriptor
 *
 * This function is called for each buffer received by the fw
 *
 */
u8 siwifi_rxdataind(void *pthis, void *hostid)
{
    struct siwifi_hw *siwifi_hw = pthis;
    struct siwifi_ipc_elem *elem = hostid;
    struct hw_rxhdr *hw_rxhdr = NULL;
    struct rxdesc_tag *rxdesc;
    struct siwifi_vif *siwifi_vif;
    struct sk_buff *skb = NULL;
#ifdef CFG_DEAGG
    struct sk_buff *amsdu_skb[RX_AMSDU_CNT_MAX - 1] = { NULL };
#endif
    uint32_t rx_buff_idx;
    //int msdu_offset = sizeof(struct hw_rxhdr) + 2;
    int msdu_offset = siwifi_hw->ipc_env->rx_pd_offset;

    int peek_len    = msdu_offset + sizeof(struct ethhdr);
#ifdef CFG_DEAGG
    int i = 0;
    uint32_t rx_buff_idx_amsdu;
    int get_amsdu = 0;
    int free_skb = 0;
#endif
    u16_l status;
    u16_l amsdu_num;


    REG_SW_SET_PROFILING(siwifi_hw, SW_PROF_SIWIFIDATAIND);
    //when HW DEAD driver will reprobe,do not care anything,return directlty
    if (test_bit(SIWIFI_DEV_HW_DEAD, &siwifi_hw->drv_flags)) {
        return -1;
    }

    /* Get the ownership of the descriptor */
    dma_sync_single_for_cpu(siwifi_hw->dev, elem->dma_addr,
                            sizeof(struct rxdesc_tag), DMA_FROM_DEVICE);

    rxdesc = elem->addr;
    status = rxdesc->status;
    amsdu_num = rxdesc->amsdu_num;

    /* check that frame is completely uploaded */
    if (!status) {
        /* Get the ownership of the descriptor */
        dma_sync_single_for_device(siwifi_hw->dev, elem->dma_addr,
                                   sizeof(struct rxdesc_tag), DMA_FROM_DEVICE);
        return -1;
    }

    /* Get the buffer linked with the received descriptor */
    rx_buff_idx = SIWIFI_RXBUFF_HOSTID_TO_IDX(rxdesc->host_id);
    if (SIWIFI_RXBUFF_VALID_IDX(rx_buff_idx))
        skb = siwifi_hw->rxbuf_elems.skb[rx_buff_idx];

    if (!skb) {
        dev_err(siwifi_hw->dev, "RX Buff invalid idx [%d]\n", rx_buff_idx);
        return -1;
    }

    /* Check the pattern */
    if (SIWIFI_RXBUFF_PATTERN_GET(skb) != siwifi_rxbuff_pattern) {
        printk("RXBUFF_PATTERN_GET(skb): %08x, siwifi_rxbuff_pattern:%08x\n", SIWIFI_RXBUFF_PATTERN_GET(skb), siwifi_rxbuff_pattern);
        dev_err(siwifi_hw->dev, "RX Buff Pattern not correct\n");
        BUG();
    }

    spin_lock(&siwifi_hw->cb_lock);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    siwifi_hw->stats.rxind_cnts++;
#endif

    /* Check if we need to delete the buffer */
    if (status & RX_STAT_DELETE) {
        if (!(status & RX_STAT_ALLOC)) {
            siwifi_hw->reord_release_cnt++;
        }
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        siwifi_hw->stats.delete_rx ++;
#endif
        /* Remove the SK buffer from the rxbuf_elems table */
        siwifi_ipc_rxbuf_elem_pull(siwifi_hw, skb);
        /* Free the buffer */
#ifdef CFG_DEAGG
        hw_rxhdr = (struct hw_rxhdr *)skb->data;
        if (hw_rxhdr->flags_is_amsdu) {
			get_amsdu = 1;
			free_skb = 1;
		} else {
            dev_kfree_skb(skb);
        }
#else
        dev_kfree_skb(skb);
#endif
        goto end;
    }

    /* Check if we need to forward the buffer coming from a monitor interface */
    if (unlikely(status & RX_STAT_MONITOR)) {
        siwifi_rx_monitor_skb(siwifi_hw, skb, status, msdu_offset);
        status |= RX_STAT_ALLOC;
    }

    /* Check if we need to update the length */
    if (status & RX_STAT_LEN_UPDATE) {
        dma_addr_t dma_addr;
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        siwifi_hw->stats.len_update_rx ++;
#endif
        dma_addr = SIWIFI_RXBUFF_DMA_ADDR_GET(skb);
        dma_sync_single_for_cpu(siwifi_hw->dev, dma_addr,
                                peek_len, DMA_FROM_DEVICE);

        hw_rxhdr = (struct hw_rxhdr *)skb->data;

        hw_rxhdr->hwvect.len = rxdesc->frame_len;

        if (status & RX_STAT_ETH_LEN_UPDATE) {
            /* Update Length Field inside the Ethernet Header */
            struct ethhdr *hdr = (struct ethhdr *)((u8 *)hw_rxhdr + msdu_offset);

            hdr->h_proto = htons(rxdesc->frame_len - sizeof(struct ethhdr));
        }

        dma_sync_single_for_device(siwifi_hw->dev, dma_addr,
                                   peek_len, DMA_BIDIRECTIONAL);
        goto end;
    }
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
    if (siwifi_hw->ate_env.rx_frame_start)
        siwifi_ate_rx_cb_rssi(siwifi_hw, skb);
#endif

    /* Check if it must be discarded after informing upper layer */
    if (unlikely(status & RX_STAT_SPURIOUS)) {
        struct ieee80211_hdr *hdr;
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        siwifi_hw->stats.spurious_rx ++;
#endif
        /* Read mac header to obtain Transmitter Address */
        siwifi_ipc_rxbuf_elem_sync(siwifi_hw, skb, msdu_offset + sizeof(*hdr));

        hw_rxhdr = (struct hw_rxhdr *)skb->data;
        hdr = (struct ieee80211_hdr *)(skb->data + msdu_offset);
        siwifi_vif = siwifi_rx_get_vif(siwifi_hw, hw_rxhdr->flags_vif_idx);
        if (siwifi_vif) {
            cfg80211_rx_spurious_frame(siwifi_vif->ndev, hdr->addr2, GFP_ATOMIC);
        }
        siwifi_ipc_rxbuf_elem_repush(siwifi_hw, skb);
        goto end;
    }

    /* Check if we need to forward the buffer */
    if (status & RX_STAT_FORWARD) {
        /* Remove the SK buffer from the rxbuf_elems table. It will also
           unmap the buffer and then sync the buffer for the cpu */
        //siwifi_ipc_rxbuf_elem_pull(siwifi_hw, skb);

        //force invalid skb cache
        siwifi_ipc_rxbuf_elem_pull_inv(siwifi_hw, skb, true);
        if (!(status & RX_STAT_ALLOC)) {
            siwifi_hw->reord_release_cnt++;
        }

        hw_rxhdr = (struct hw_rxhdr *)skb->data;
        skb_reserve(skb, msdu_offset);
#ifdef CFG_DEAGG
        if (hw_rxhdr->flags_is_amsdu) {
            int len, subframe_len;
            struct ethhdr *eth;

            eth = (struct ethhdr *)(skb->data);
            len = ntohs(eth->h_proto);
            subframe_len = sizeof(struct ethhdr) + len;
            if (len < 0 || subframe_len > siwifi_hw->ipc_env->rx_bufsz || (skb->tail + subframe_len) > skb->end) {
                dev_err(siwifi_hw->dev, "Invalid skb len %d for first amsdu tail %p end %p\n",
                        subframe_len, skb->tail, skb->end);
				get_amsdu = 1;
				free_skb = 1;
                goto check_alloc;
            }
            skb_put(skb, subframe_len);
        } else {
            skb_put(skb, le32_to_cpu(hw_rxhdr->hwvect.len));
        }
#else
        skb_put(skb, le32_to_cpu(hw_rxhdr->hwvect.len));
#endif

        if (hw_rxhdr->flags_is_80211_mpdu) {
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
            siwifi_hw->stats.mgmt_rx ++;
#endif
            siwifi_rx_mgmt_any(siwifi_hw, skb, hw_rxhdr);
        } else {
            siwifi_vif = siwifi_rx_get_vif(siwifi_hw, hw_rxhdr->flags_vif_idx);

            if (!siwifi_vif) {
                dev_err(siwifi_hw->dev, "Frame received but no active vif (%d, %d)",
                        hw_rxhdr->flags_vif_idx, ((struct hw_rxhdr *)CKSEG1ADDR(hw_rxhdr))->flags_vif_idx);
                dev_err(siwifi_hw->dev, "siwifi hw vif map : 0x%x\n", siwifi_hw->avail_idx_map);

#ifndef CFG_DEAGG
                dev_kfree_skb(skb);
#else
                // At least vif index is valid, then I think we may trust the content of hw rx header
                if (hw_rxhdr->flags_vif_idx < NX_VIRT_DEV_MAX
                        && hw_rxhdr->flags_is_amsdu) {
                    get_amsdu = 1;
                    free_skb = 1;
                } else {
                    dev_kfree_skb(skb);
                }
#endif
                goto check_alloc;
            }

            if (hw_rxhdr->flags_sta_idx < (NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX)) {
                struct siwifi_sta *sta;

                sta = &siwifi_hw->sta_table[hw_rxhdr->flags_sta_idx];
				sta->stats.idle = ktime_get_seconds();

                siwifi_rx_statistic(siwifi_hw, hw_rxhdr, sta);
                siwifi_hw->siwifi_static_txinfo[sta->sta_idx].siwifi_static_rx++;

                if (sta != NULL && sta->valid) {
                    struct ethhdr eth;
                    memcpy(&eth, skb->data, sizeof(eth));
                    if (!is_multicast_ether_addr(eth.h_dest)){
                        siwifi_rx_sta_stats(sta, hw_rxhdr);
                    }
                }

                if (sta->vlan_idx != siwifi_vif->vif_index) {
                    siwifi_vif = siwifi_hw->vif_table[sta->vlan_idx];
                    if (!siwifi_vif) {
						#ifndef CFG_DEAGG
                        dev_kfree_skb(skb);
						#else
                        if (hw_rxhdr->flags_is_amsdu) {
						    get_amsdu = 1;
						    free_skb = 1;
                        } else {
                            dev_kfree_skb(skb);
                        }
						#endif
                        goto check_alloc;
                    }
                }

                if (hw_rxhdr->flags_is_4addr && !siwifi_vif->use_4addr) {
                    cfg80211_rx_unexpected_4addr_frame(siwifi_vif->ndev,
                                                       sta->mac_addr, GFP_ATOMIC);
                }
            } else {
                dev_err(siwifi_hw->dev, "Frame received but no active sta (%d)",
                        hw_rxhdr->flags_sta_idx);
            }

            skb->priority = 256 + hw_rxhdr->flags_user_prio;
//            printk("%s receive a skb len %d\n", __func__, skb->len);

#ifndef CFG_DEAGG
            if (!siwifi_rx_data_skb(siwifi_hw, siwifi_vif, skb, hw_rxhdr, NULL))
                dev_kfree_skb(skb);
#else
            if (hw_rxhdr->flags_is_amsdu) {
                for (i = 0; i < RX_AMSDU_CNT_MAX - 1; i++) {
                    if (!hw_rxhdr->amsdu_hostids[i])
                        break;

                    /* Get the buffer linked with the received descriptor */
                    rx_buff_idx_amsdu = SIWIFI_RXBUFF_HOSTID_TO_IDX(hw_rxhdr->amsdu_hostids[i]);
                    if (!SIWIFI_RXBUFF_VALID_IDX(rx_buff_idx_amsdu)) {
                        dev_err(siwifi_hw->dev, "RX Buff invalid2 idx [%d]\n", rx_buff_idx_amsdu);
						free_skb = 1;
                        break;
                    }
                    amsdu_skb[i] = siwifi_hw->rxbuf_elems.skb[rx_buff_idx_amsdu];
                    if (!amsdu_skb[i]) {
                        dev_err(siwifi_hw->dev, "amsdu_skb is null[%d]\n", rx_buff_idx_amsdu);
						free_skb = 1;
                        break;
                    }
                    /* Check the pattern */
                    if (SIWIFI_RXBUFF_PATTERN_GET(amsdu_skb[i]) != siwifi_rxbuff_pattern) {
                        dev_err(siwifi_hw->dev, "RX Buff Pattern not correct\n");
						free_skb = 1;
                        break;
                    }
                    siwifi_ipc_rxbuf_elem_pull(siwifi_hw, amsdu_skb[i]);
                    if (!(status & RX_STAT_ALLOC)) {
                        siwifi_hw->reord_release_cnt++;
                    }
                    skb_reserve(amsdu_skb[i], msdu_offset);
                }

                //transmit the main skb and amsdu_skb
                if (!siwifi_rx_data_skb(siwifi_hw, siwifi_vif, skb, hw_rxhdr, amsdu_skb)) {
					free_skb = 1;
                }
            } else {
                if (!siwifi_rx_data_skb(siwifi_hw, siwifi_vif, skb, hw_rxhdr, amsdu_skb))
                    dev_kfree_skb(skb);
            }
#endif
#ifdef CONFIG_SF19A28_WIFI_LED
			siwifi_led_rx(siwifi_hw);
#endif
        }
    }

check_alloc:
    /* Check if we need to allocate a new buffer */
    if ((status & RX_STAT_ALLOC)) {
        siwifi_ipc_rxbuf_elem_allocs(siwifi_hw);
        if (!(status & (RX_STAT_FORWARD | RX_STAT_DELETE))) {
            siwifi_hw->reord_alloc_cnt++;
        }

#ifdef CFG_DEAGG
        if (amsdu_num != 0) {
            //alloacte new amsdu hostid buffers
            for (i = 0; i < amsdu_num; i++) {
                siwifi_ipc_rxbuf_elem_allocs(siwifi_hw);
                if (!(status & (RX_STAT_FORWARD | RX_STAT_DELETE))) {
                    siwifi_hw->reord_alloc_cnt++;
                }
            }
        }
#endif
    }

end:
#ifdef CFG_DEAGG
    /* Fetch all AMSDUs*/
	if (get_amsdu)
	{
		for (i = 0; i < RX_AMSDU_CNT_MAX - 1; i++) {
			if (!hw_rxhdr->amsdu_hostids[i])
				break;

			/* Get the buffer linked with the received descriptor */
			rx_buff_idx_amsdu = SIWIFI_RXBUFF_HOSTID_TO_IDX(hw_rxhdr->amsdu_hostids[i]);
			if (!SIWIFI_RXBUFF_VALID_IDX(rx_buff_idx_amsdu)) {
				dev_err(siwifi_hw->dev, "get amsdu RX Buff invalid idx [%d]\n", rx_buff_idx_amsdu);
                continue;
			}
			amsdu_skb[i] = siwifi_hw->rxbuf_elems.skb[rx_buff_idx_amsdu];
			if (!amsdu_skb[i]) {
				dev_err(siwifi_hw->dev, "get amsdu amsdu_skb is null[%d]\n", rx_buff_idx_amsdu);
                continue;
			}
			/* Check the pattern */
			if (SIWIFI_RXBUFF_PATTERN_GET(amsdu_skb[i]) != siwifi_rxbuff_pattern) {
				dev_err(siwifi_hw->dev, "get amsdu RX Buff Pattern not correct\n");
                continue;
			}
			siwifi_ipc_rxbuf_elem_pull(siwifi_hw, amsdu_skb[i]);
		}
	}
	if (free_skb)
	{
		for (i = 0; i < RX_AMSDU_CNT_MAX - 1; i++) {
			if (!hw_rxhdr->amsdu_hostids[i])
				break;
            if (amsdu_skb[i])
			    dev_kfree_skb(amsdu_skb[i]);
		}
		dev_kfree_skb(skb);
	}
#endif /*CFG_DEAGG*/
    //siwifi_ipc_rxbuf_elem_fail_check(siwifi_hw);
    spin_unlock(&siwifi_hw->cb_lock);
    REG_SW_CLEAR_PROFILING(siwifi_hw, SW_PROF_SIWIFIDATAIND);

    /* Reset and repush descriptor to FW */
    siwifi_ipc_rxdesc_elem_repush(siwifi_hw, elem);

    return 0;
}

#ifdef CONFIG_WIFI_RX_THREAD
static void siwifi_rx_thread_process_skbs(struct siwifi_hw *siwifi_hw, struct rx_skb_element *rx_elt)
{
    struct siwifi_vif *vif;
    struct sk_buff *rx_skb;
    uint32_t tx_dropped = 0;
    uint32_t tx_errors = 0;
    uint32_t forward_len = 0;
    uint32_t forward_num = 0;

    while ((rx_skb = __skb_dequeue(&rx_elt->skb_list))) {
        spin_lock_bh(&siwifi_hw->cb_lock);
        if (rx_elt->sta != NULL && rx_elt->sta->valid) {
#ifdef CONFIG_VDR_HW
            vendor_hook_rxdata(rx_elt->sta, rx_skb, rx_elt->rxhdr);
#endif
            siwifi_rx_sta_stats(rx_elt->sta, rx_elt->rxhdr);
        }
        spin_unlock_bh(&siwifi_hw->cb_lock);
        /* resend pkt on wireless interface */
#ifdef CONFIG_SIWIFI_REPEATER
        if (rx_elt->vif->rp_info) {
            if (repeater_rev_proc(rx_elt->vif->rp_info, &rx_skb) < 0) {
                dev_kfree_skb(rx_skb);
                continue;
            }
        } else
#endif
        {
            if (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_STATION) {
                struct ethhdr *sf_eth = NULL;
                struct siwifi_src_filter *src_filter = NULL;
                skb_reset_mac_header(rx_skb);
                sf_eth = eth_hdr(rx_skb);
                src_filter = siwifi_src_filter_hash_search(siwifi_vif, sf_eth->h_source);
                if (src_filter) {
                    src_filter->drop_count++;
                    dev_kfree_skb(rx_skb);
                    continue;
                }
            }
        }

        if (rx_elt->resend) {
            struct sk_buff *skb_copy;
            int res;
#if 0
            /* always need to copy buffer when forward=0 to get enough headrom for tsdesc */
            skb_copy = skb_copy_expand(rx_skb, sizeof(struct siwifi_txhdr) +
                    SIWIFI_SWTXHDR_ALIGN_SZ, 0, GFP_ATOMIC);
            if (!skb_copy) {
                printk("%s,%d skb copy expand failed\n",
                        siwifi_mod_params.is_hb ? "HB" : "LB", __LINE__);
                goto FORWARD;
            }
#else
            if (rx_elt->forward) {
                /* always need to copy buffer when forward=0 to get enough headrom for tsdesc */
                skb_copy = skb_clone(rx_skb, GFP_ATOMIC);
                if (!skb_copy) {
                    printk("%s,%d skb copy expand failed\n",
                            siwifi_mod_params.is_hb ? "HB" : "LB", __LINE__);
                    goto FORWARD;
                }
            } else {
                skb_copy = rx_skb;
            }
#endif
            skb_copy->protocol = htons(ETH_P_802_3);
            skb_reset_network_header(skb_copy);
            skb_reset_mac_header(skb_copy);

            /*
             * in dev_queue_xmit,will go to siwifi_start_xmit,we use cb_lock in siwifi_start_xmit
             * so must unlock first,otherwise will dead lock
             * */
            res = dev_queue_xmit(skb_copy);
            if (res == NET_XMIT_DROP) {
                tx_dropped ++;
            } else if (res != NET_XMIT_SUCCESS) {
                tx_errors ++;
            }
        }
FORWARD:
        /* forward pkt to upper layer */
        if (rx_elt->forward) {
#if defined(CONFIG_SF19A28_FULLMASK) && IS_ENABLED(CONFIG_SFAX8_HNAT_DRIVER) && IS_ENABLED(CONFIG_NF_FLOW_TABLE)
            struct sf_hnat_priv *phnat_priv;
            struct platform_device *hnat_pdev;
#endif
            skb_reset_mac_header(rx_skb);
            /* eth_type_trans should execute before xmit_prepare because it will change skb->dev */
            memset(rx_skb->cb, 0, sizeof(rx_skb->cb));
            // rx trace in, here we have a risk to access rx_elt->sta
            siwifi_trace_rx_in(siwifi_hw, rx_elt->sta, rx_skb);

#ifdef CONFIG_SIWIFI_IGMP
            siwifi_check_igmp(siwifi_hw, rx_skb);
#endif
            spin_lock_bh(&siwifi_hw->cb_lock);
            list_for_each_entry(vif, &siwifi_hw->vifs, list) {
                if (rx_elt->vif != vif)
                    continue;
                rx_skb->protocol = eth_type_trans(rx_skb, vif->ndev);
#if defined(CONFIG_SF19A28_FULLMASK) && IS_ENABLED(CONFIG_SFAX8_HNAT_DRIVER) && IS_ENABLED(CONFIG_NF_FLOW_TABLE)
                phnat_priv = vif->phnat_priv;
                hnat_pdev = vif->hnat_pdev;
                if (phnat_priv) {
                    phnat_priv->search(hnat_pdev, rx_skb);
                }
#endif
            }
            spin_unlock_bh(&siwifi_hw->cb_lock);
            REG_SW_SET_PROFILING(siwifi_hw, SW_PROF_IEEE80211RX);

            forward_num ++;
            forward_len += rx_skb->len;

            netif_receive_skb(rx_skb);
            REG_SW_CLEAR_PROFILING(siwifi_hw, SW_PROF_IEEE80211RX);
        }
        if (!rx_elt->forward && !rx_elt->resend)
            dev_kfree_skb(rx_skb);
        siwifi_hw->total_rx ++;
        rx_elt->vif->total_rx ++;
    }

    // update statiscs
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    siwifi_hw->stats.total_rx += forward_num;
#endif
    spin_lock_bh(&siwifi_hw->cb_lock);
    list_for_each_entry(vif, &siwifi_hw->vifs, list) {
        if (vif != rx_elt->vif)
            continue;
        if (tx_dropped) {
            vif->net_stats.rx_dropped += tx_dropped;
            vif->net_stats.tx_dropped += tx_dropped;
        }
        if (tx_errors) {
            vif->net_stats.tx_errors += tx_errors;
        }

        vif->net_stats.rx_packets += forward_num;
        vif->net_stats.rx_bytes += forward_len;
        if (rx_elt->sta && rx_elt->sta->valid) {
            rx_elt->sta->stats.rx_packets += forward_num;
            rx_elt->sta->stats.rx_bytes += forward_len;
            if (tx_dropped) {
                rx_elt->sta->stats.rx_dropped += tx_dropped;
                rx_elt->sta->stats.tx_dropped += tx_dropped;
            }
            if (tx_errors)
                rx_elt->sta->stats.tx_failed += tx_errors;
        }
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);
}

static int siwifi_rx_thread(void *data)
{
    int result = 0;
    struct siwifi_hw *siwifi_hw = (struct siwifi_hw *)data;
    size_t pending;
    struct rx_skb_element *rx_elt;
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    int loop_time;
#endif

    spin_lock_bh(&siwifi_hw->rx_lock);
    BUG_ON(siwifi_hw->rx_kthread != NULL);
    siwifi_hw->rx_kthread = current;
    spin_unlock_bh(&siwifi_hw->rx_lock);

    // big loop to process all rx skbs
    while (1) {
        pending = 0;
        wait_event_interruptible(
                siwifi_hw->rx_wq,
                (kthread_should_stop()
                 || (pending = atomic_read(&siwifi_hw->rx_pending_count)))
                );
        /* check this first! */
        if (kthread_should_stop()) {
            printk("user want rx thread(%s) exit\n", siwifi_mod_params.is_hb ? "HB" : "LB");
            break;
        }
        if (pending == 0)
            continue;

        do {
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
            loop_time = 0;
#endif
            spin_lock_bh(&siwifi_hw->rx_lock);
            if (list_empty(&siwifi_hw->rx_skbs)) {
                spin_unlock_bh(&siwifi_hw->rx_lock);
                // if we process all rx skbs, then clear the rx_pending_count to save
                // cpu usage
                atomic_set(&siwifi_hw->rx_pending_count, 0);
                break;
            }
            rx_elt = list_first_entry(&siwifi_hw->rx_skbs, struct rx_skb_element, list);
            list_del(&rx_elt->list);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
            siwifi_hw->rx_thread_dbg.elt_cur_cnt --;
            siwifi_hw->rx_thread_dbg.skb_cur_cnt -= skb_queue_len(&rx_elt->skb_list);

#endif
            spin_unlock_bh(&siwifi_hw->rx_lock);

            // To advoid this loop take too much time
            if (kthread_should_stop()) {
                printk("user want rx thread(%s) exit\n", siwifi_mod_params.is_hb ? "HB" : "LB");
                break;
            }

            // check if we have skbs in this elt
            if (unlikely(!skb_queue_len(&rx_elt->skb_list))) {
                printk("Wanrning : there is no skb in rx skb elemnt%s\n", siwifi_mod_params.is_hb ? "HB" : "LB");
                siwifi_kfree(rx_elt);
                continue;
            }
            // process the skbs in this element;
            siwifi_rx_thread_process_skbs(siwifi_hw, rx_elt);
            // free the rx elements
            siwifi_kfree(rx_elt);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
            if (++loop_time > siwifi_hw->rx_thread_dbg.elt_loop_max_cnt)
                siwifi_hw->rx_thread_dbg.elt_loop_max_cnt = loop_time;
#endif
        } while (1);
    }

    // clear all resources
    spin_lock_bh(&siwifi_hw->rx_lock);
    siwifi_hw->rx_kthread = NULL;
    // free all rx elements
    while (!list_empty(&siwifi_hw->rx_skbs)) {
        struct sk_buff *rx_skb;

        rx_elt = list_first_entry(&siwifi_hw->rx_skbs, struct rx_skb_element, list);
        list_del(&rx_elt->list);

#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
            siwifi_hw->rx_thread_dbg.elt_cur_cnt --;
            siwifi_hw->rx_thread_dbg.skb_cur_cnt -= skb_queue_len(&rx_elt->skb_list);
#endif
        // free the skb in each rx elements
        while ((rx_skb = __skb_dequeue(&rx_elt->skb_list))) {
            dev_kfree_skb(rx_skb);
        }
        siwifi_kfree(rx_elt);
    }
    spin_unlock_bh(&siwifi_hw->rx_lock);

    return result;
}

void siwifi_rx_thread_flush(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif)
{
    struct rx_skb_element *rx_elt, *rx_elt1;
    struct sk_buff *rx_skb;

    printk("siwifi_rx_thread_flush\n");
    spin_lock_bh(&siwifi_hw->rx_lock);
    list_for_each_entry_safe(rx_elt, rx_elt1, &siwifi_hw->rx_skbs, list)
    {
        if (rx_elt->vif != vif)
            continue;
        list_del(&rx_elt->list);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        siwifi_hw->rx_thread_dbg.elt_cur_cnt --;
        siwifi_hw->rx_thread_dbg.skb_cur_cnt -= skb_queue_len(&rx_elt->skb_list);
#endif
        while ((rx_skb = __skb_dequeue(&rx_elt->skb_list))) {
            dev_kfree_skb(rx_skb);
        }
        siwifi_kfree(rx_elt);
    }
    spin_unlock_bh(&siwifi_hw->rx_lock);
    printk("siwifi_rx_thread_flush over!\n");
}

int siwifi_rx_thread_setup(struct siwifi_hw *siwifi_hw)
{
    struct task_struct *kthread;
    int result = 0;
    printk("siwifi_rx_thread_setup ++\n");
    if (!list_empty(&siwifi_hw->rx_skbs)) {
        printk("rx skbs list should be empty when setup\n");
        return -1;
    }
    kthread = kthread_run(siwifi_rx_thread, siwifi_hw, "%s-rx",
            siwifi_mod_params.is_hb ? "HB" : "LB");
    if (IS_ERR(kthread)) {
        result = PTR_ERR(kthread);
        printk("RX: cannot start thread: %d\n", result);
    }
    return result;
}

void siwifi_rx_thread_release(struct siwifi_hw *siwifi_hw)
{
    struct task_struct *kthread;

    printk("siwifi_rx_thread_release++\n");

    spin_lock_bh(&siwifi_hw->rx_lock);
    kthread = siwifi_hw->rx_kthread;
    siwifi_hw->rx_kthread = NULL;
    spin_unlock_bh(&siwifi_hw->rx_lock);
    if (kthread)
        kthread_stop(kthread);
    else
        printk("RX: kthread had already exited, %s\n",
                siwifi_mod_params.is_hb ? "HB" : "LB");
}

#endif
