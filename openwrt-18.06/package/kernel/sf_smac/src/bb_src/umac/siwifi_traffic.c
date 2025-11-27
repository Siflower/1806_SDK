/**
 * siwifi_traffic.c
 *
 * traffic function definitions
 *
 * Copyright (C) Siflower 2018-2025
 */

#include "siwifi_defs.h"
#include "reg_access.h"
#include "siwifi_traffic.h"
#include <linux/ip.h>
#include <linux/if_vlan.h>


//#define CONFIG_BRIDGE_ACCELERATE 1

static struct siwifi_traffic_info traffic_info[AMSDU_MAX_TID] = { 0 };

void traffic_info_init(void)
{
    int i;
    memset(traffic_info, 0 , sizeof(traffic_info));
    for(i = 0; i < AMSDU_MAX_TID; i++)
    {
        traffic_info[i].next_jiffies = jiffies;
    }
}

void traffic_detect_for_amsdu(struct siwifi_hw *siwifi_hw, struct siwifi_txq *txq, u8 tid)
{
#if defined CONFIG_SIWIFI_AMSDUS_TX
    uint32_t amsdu_threshold = AMSDU_THRESHOLD_M;
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    amsdu_threshold = siwifi_hw->amsdu_threshold;
#endif
    if (tid >= AMSDU_MAX_TID) return;

    traffic_info[tid].pkt_cnt++;
    if (time_after(jiffies, traffic_info[tid].next_jiffies)) {
        traffic_info[tid].next_jiffies = jiffies + AMSDU_CHECK_INTERVAL;
        if (traffic_info[tid].pkt_cnt > amsdu_threshold) {
            txq->amsdu_maxnb = AMSDU_NB_V;
        } else {
            txq->amsdu_maxnb = AMSDU_NB_L;
        }
        traffic_info[tid].pkt_cnt = 0;
    }
    if(txq->amsdu_maxnb == 0) {
        traffic_info[tid].next_jiffies = jiffies;
    }
#endif
}

void update_vif_wmm_param(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif)
{
	u32 step = siwifi_vif->hw_last_wmm_step;
    u32 counter_max = 0;
	u32 be_threshold = 0;
    u32 vif_hw_be_cnt = siwifi_vif->hw_be_cnt;
    u32 vif_hw_vi_cnt = siwifi_vif->hw_vi_cnt;
    u32 vif_hw_vo_cnt = siwifi_vif->hw_vo_cnt;

    u32 vif_hw_other_cnt = siwifi_vif->hw_all_cnt - vif_hw_be_cnt;
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    counter_max = siwifi_hw->wmm_edca_counter_drop;
    be_threshold = siwifi_hw->wmm_edca_pkt_threshold;
#else
    counter_max = EDCA_WMM_COUNTER;
    be_threshold = EDCA_BE_THRESHOLD;
#endif
    if ((vif_hw_be_cnt > EDCA_BE_THRESHOLD) && ((vif_hw_other_cnt << EDCA_BE_RATIO_E) < vif_hw_be_cnt)
            && (vif_hw_be_cnt > ((siwifi_vif->total_rx - siwifi_vif->hw_rx_cnt) * 2))) {
        siwifi_vif->be_wmm_param = EDCA_BE_AGGRESSIVE;
        siwifi_vif->hw_wmm_counter = 0;
        step = 1;
    } else if (vif_hw_be_cnt > EDCA_VI_THRESHOLD && (vif_hw_vi_cnt > EDCA_VI_THRESHOLD || vif_hw_vo_cnt > EDCA_VI_THRESHOLD)) {
        siwifi_vif->be_wmm_param = EDCA_BK_DEFAULT;
        siwifi_vif->hw_wmm_counter = 0;
        step = 2;
    } else {
        if(siwifi_vif->hw_wmm_counter < counter_max)
            siwifi_vif->hw_wmm_counter ++;
        if(siwifi_vif->hw_wmm_counter  == counter_max || siwifi_vif->hw_last_wmm_step == 0) {
            siwifi_vif->be_wmm_param = EDCA_BE_DEFAULT;
            step = 3;
        }
    }

    if(step != siwifi_vif->hw_last_wmm_step && siwifi_hw->wmm_debug_enable == 1) {
        printk("vif %u tye %u change from step %d to %d (1 for tx 3 for rx) other-be-vi-vo=[%u %u %u %u] rx %llu\n",
                siwifi_vif->vif_index, SIWIFI_VIF_TYPE(siwifi_vif), siwifi_vif->hw_last_wmm_step, step, vif_hw_other_cnt, vif_hw_be_cnt,
                vif_hw_vi_cnt, vif_hw_vo_cnt, siwifi_vif->total_rx - siwifi_vif->hw_rx_cnt);
    }

    siwifi_vif->hw_last_wmm_step = step;
    siwifi_vif->hw_be_cnt = 0;
    siwifi_vif->hw_vi_cnt = 0;
    siwifi_vif->hw_vo_cnt = 0;
    siwifi_vif->hw_all_cnt = 0;
    siwifi_vif->hw_rx_cnt = 0;
}

static u32 hw_be_cnt = 0;
static u32 hw_all_cnt = 0;
static unsigned long hw_next_jiffies = 0;
static u32 hw_vi_cnt = 0;
static u32 hw_vo_cnt = 0;
static u64 hw_rx_cnt = 0;
static u32 hw_last_wmm_step = 0;
static u32 hw_wmm_counter = 0;
void traffic_detect_be_edca(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif, u8 ac, u8 pkt_num)
{
	u32 step = hw_last_wmm_step;
	u32 counter_max = 0;
	u32 be_threshold = 0;
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
	if (siwifi_hw->disable_wmm_edca)
		return;
#endif

	hw_all_cnt += pkt_num;
	if (ac == AC_BE) hw_be_cnt += pkt_num;
	if (ac == AC_VI) hw_vi_cnt += pkt_num;
	if (ac == AC_VO) hw_vo_cnt += pkt_num;

	if(hw_rx_cnt == 0) hw_rx_cnt = siwifi_hw->total_rx;
	if (hw_next_jiffies == 0)
		hw_next_jiffies = jiffies;

    if(siwifi_vif)
    {
        siwifi_vif->hw_all_cnt += pkt_num;
        if (ac == AC_BE) siwifi_vif->hw_be_cnt += pkt_num;
	    if (ac == AC_VI) siwifi_vif->hw_vi_cnt += pkt_num;
	    if (ac == AC_VO) siwifi_vif->hw_vo_cnt += pkt_num;
	    if (siwifi_vif->hw_rx_cnt == 0) siwifi_vif->hw_rx_cnt = siwifi_vif->total_rx;
    }

	if (time_after(jiffies, hw_next_jiffies)) {
		u32 hw_other_cnt = hw_all_cnt - hw_be_cnt;
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
		hw_next_jiffies = jiffies + siwifi_hw->wmm_edca_interval;
		counter_max = siwifi_hw->wmm_edca_counter_drop;
		be_threshold = siwifi_hw->wmm_edca_pkt_threshold;
#else
		hw_next_jiffies = jiffies + EDCA_CHECK_INTERVAL;
		counter_max = EDCA_WMM_COUNTER;
		be_threshold = EDCA_BE_THRESHOLD;
#endif
		if ((hw_be_cnt > EDCA_BE_THRESHOLD) && ((hw_other_cnt << EDCA_BE_RATIO_E) < hw_be_cnt) && (hw_be_cnt > ((siwifi_hw->total_rx - hw_rx_cnt) * 2))) {
            writel(EDCA_BE_AGGRESSIVE, (void*)EDCA_AC_1_ADDR(siwifi_hw->mod_params->is_hb));
			hw_wmm_counter = 0;
			step = 1;
		} else if (hw_be_cnt > EDCA_VI_THRESHOLD && (hw_vi_cnt > EDCA_VI_THRESHOLD || hw_vo_cnt > EDCA_VI_THRESHOLD)) {
            writel(EDCA_BK_DEFAULT, (void*)EDCA_AC_1_ADDR(siwifi_hw->mod_params->is_hb));
			hw_wmm_counter = 0;
			step = 2;
		} else {
			if(hw_wmm_counter < counter_max)
				hw_wmm_counter ++;
			if(hw_wmm_counter  == counter_max || hw_last_wmm_step == 0) {
                writel(EDCA_BE_DEFAULT, (void*)EDCA_AC_1_ADDR(siwifi_hw->mod_params->is_hb));
				step = 3;
			}
		}

		if(step != hw_last_wmm_step && siwifi_hw->wmm_debug_enable == 1) {
			printk("change from step %d to %d (1 for tx 3 for rx) other-be-vi-vo=[%u %u %u %u] rx %llu\n", hw_last_wmm_step,
					step, hw_other_cnt, hw_be_cnt, hw_vi_cnt, hw_vo_cnt, siwifi_hw->total_rx - hw_rx_cnt);
		}

		hw_last_wmm_step = step;
		hw_be_cnt = 0;
		hw_vi_cnt = 0;
		hw_vo_cnt = 0;
		hw_all_cnt = 0;
		hw_rx_cnt = 0;

        if(siwifi_vif)
        {
            update_vif_wmm_param(siwifi_hw, siwifi_vif);
        }
	}
}

#ifdef CONFIG_BRIDGE_ACCELERATE

typedef int (*device_drv_hook_fn)(struct sk_buff *skb, struct net_device *outdev);
extern int backlog_skb_handler_register(device_drv_hook_fn hook);
extern int backlog_skb_handler_unregister(device_drv_hook_fn hook);

int accel_enable = 1;
u64 accelerate_cnt = 0; // only for debug
u64 accel_pkt_in = 0; // only for debug
struct net_device *g_crrent_accel_dev = NULL;
int g_crrent_accel_dev_idx = 0;

struct siwifi_device_traffic
{
    struct net_device *dev;
    unsigned long pkt;
    unsigned long next_jiffies;
    struct net_device *src_dev;
    u16 vlan_tci;
};

#define SIWIFI_DEV_MAX (NX_VIRT_DEV_MAX * 2)
struct siwifi_device_traffic g_siwifi_dev_traffic[SIWIFI_DEV_MAX];

void siwifi_device_traffic_init(void)
{
    int i;
    memset(g_siwifi_dev_traffic, 0 ,sizeof(g_siwifi_dev_traffic));
    for(i = 0; i < SIWIFI_DEV_MAX; i++)
    {
        g_siwifi_dev_traffic[i].next_jiffies = jiffies;
    }
}

int siwifi_xmit_hook(struct sk_buff *skb, struct net_device *outdev)
{
	u16 protocol = 0;
	struct iphdr *ip4h;
	struct vlan_ethhdr *veth;
	struct siwifi_sta *sta = NULL;
	struct siwifi_vif *vif = NULL;
	u8 *eth_header = skb->data - ETH_HLEN;
	struct net_device *dev = g_crrent_accel_dev;
	//u16 vlan_tci;

	if(!accel_enable) return NET_RX_DROP;
	if(!g_crrent_accel_dev) {
		return NET_RX_DROP;
	} else if (g_crrent_accel_dev != outdev){
		return NET_RX_DROP;
	}
	// skip too small packet or skb_shared
	if (skb->len < MIN_XMIT_SKB_LEN || skb_shared(skb))
		return NET_RX_DROP;

	// only handle ipv4 udp/tcp
	if(skb->protocol != cpu_to_be16(ETH_P_8021Q)) {
		ip4h = (struct iphdr *)(skb->data);
		protocol = skb->protocol;
	} else {
		veth = (struct vlan_ethhdr *) (skb->head + skb->mac_header);
		ip4h = (struct iphdr *)(skb->data + VLAN_HLEN);
		protocol = veth->h_vlan_encapsulated_proto;
	}
	if (protocol != cpu_to_be16(ETH_P_IP) || (ip4h->protocol != IPPROTO_TCP &&
		ip4h->protocol != IPPROTO_UDP)) {
		return NET_RX_DROP;
	}

	//struct ethhdr *eth;
	if (NULL == dev || NULL == dev->netdev_ops ||
			NULL == dev->netdev_ops->ndo_start_xmit)
		return NET_RX_DROP;
	vif = netdev_priv(dev);
	if (!vif)
		return NET_RX_DROP;

	if (SIWIFI_VIF_TYPE(vif) == NL80211_IFTYPE_AP) {
		sta = siwifi_sta_hash_get(vif, eth_header);
		if (!sta)
			return NET_RX_DROP;
	} else if (SIWIFI_VIF_TYPE(vif) != NL80211_IFTYPE_STATION) {
		return NET_RX_DROP;
	}

	accelerate_cnt++;
	// struct vlan_hdr *vhdr;
	// vhdr = (struct vlan_hdr *)skb->data;
	// vlan_tci = ntohs(vhdr->h_vlan_TCI);
	//update src dev and tci
#if DEBUG_ARRAY_CHECK
	BUG_ON(g_crrent_accel_dev_idx >= SIWIFI_DEV_MAX);
#endif
	g_siwifi_dev_traffic[g_crrent_accel_dev_idx].src_dev = skb->dev;
	// g_siwifi_dev_traffic[g_crrent_accel_dev_idx].vlan_tci = vlan_tci;
	//untag tci
	if(skb->protocol != cpu_to_be16(ETH_P_8021Q))
		skb_push(skb, ETH_HLEN);
	else {
		memmove(eth_header + VLAN_HLEN, eth_header, 2 * ETH_ALEN);
		skb_push(skb, ETH_HLEN - VLAN_HLEN);
	}
	//mark skb as accel
	skb->cb[SF_WIFI_ACCEL] = SF_WIFI_ACCEL_FLAG;
	dev->netdev_ops->ndo_start_xmit(skb, dev);

	return NET_RX_SUCCESS;
}

static void siwifi_clear_hook_dev(void)
{
    g_crrent_accel_dev = NULL;
}

void siwifi_dev_traffic_in(struct sk_buff *skb, struct net_device *dev)
{
    int i;
    bool found = false;
    int first_index = -1;
    if (!dev || !skb) return;
    accel_pkt_in++;
    for (i = 0; i < SIWIFI_DEV_MAX; i++)
    {
        if (!g_siwifi_dev_traffic[i].dev && first_index == -1)
            first_index = i;
        if (g_siwifi_dev_traffic[i].dev == dev)
        {
            g_siwifi_dev_traffic[i].pkt++;
            if (time_after(jiffies, g_siwifi_dev_traffic[i].next_jiffies)) {
                if (g_crrent_accel_dev == dev && g_siwifi_dev_traffic[i].pkt < 5000) {
                    //clear hook
                    g_crrent_accel_dev = NULL;
                    siwifi_clear_hook_dev();
                } else if (g_crrent_accel_dev != dev && g_siwifi_dev_traffic[i].pkt > 20000) {
                    //add fast path hook
                    siwifi_clear_hook_dev();
                    g_crrent_accel_dev = dev;
                    g_crrent_accel_dev_idx = i;
                }
                g_siwifi_dev_traffic[i].pkt = 0;
                g_siwifi_dev_traffic[i].src_dev = NULL;
                g_siwifi_dev_traffic[i].next_jiffies = jiffies + 250;
            }
            found = true;
            break;
        }
    }
    if (!found && first_index != -1) {
        g_siwifi_dev_traffic[first_index].dev = dev;
        g_siwifi_dev_traffic[first_index].pkt = 0;
        g_siwifi_dev_traffic[first_index].src_dev = NULL;
        g_siwifi_dev_traffic[first_index].next_jiffies = jiffies + 250;
        found = true;
    }
    if (!found) printk("ERROR: siwifi_dev_traffic_in dev=%p\n", dev);
}

void siwifi_clear_hook(struct net_device *dev)
{
    int i;
    if(g_crrent_accel_dev == dev){
        siwifi_clear_hook_dev();
    }
    for(i = 0; i < SIWIFI_DEV_MAX; i++)
    {
        if(g_siwifi_dev_traffic[i].dev == dev)
        {
            g_siwifi_dev_traffic[i].pkt = 0;
            g_siwifi_dev_traffic[i].next_jiffies = jiffies;
            g_siwifi_dev_traffic[i].dev = NULL;
            g_siwifi_dev_traffic[i].src_dev = NULL;
        }
    }
}

void siwifi_hook_xmit_reigster(void)
{
    backlog_skb_handler_register(siwifi_xmit_hook);
}

void siwifi_hook_xmit_unreigster(void)
{
    backlog_skb_handler_unregister(siwifi_xmit_hook);
}


#endif
