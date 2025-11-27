/**
 * siwifi_traffic.h
 *
 * traffic function declarations
 *
 * Copyright (C) Siflower 2018-2025
 */
#ifndef _SIWIFI_TRAFFIC_H_
#define _SIWIFI_TRAFFIC_H_

#if 1
#define AMSDU_NB_L 1
#define AMSDU_NB_M 2
#define AMSDU_NB_H 4
#define AMSDU_NB_V 4
#else
#define AMSDU_NB_L 2
#define AMSDU_NB_M 2
#define AMSDU_NB_H 2
#define AMSDU_NB_V 2
#endif

#define AMSDU_THRESHOLD_L 400
#define AMSDU_THRESHOLD_M 200
#define AMSDU_THRESHOLD_H 2000

#define EDCA_AC_1_ADDR(band) (WIFI_BASE_ADDR(band) + 0x00080000 + 0x0204)
#define AMSDU_MAX_TID 8
#define AMSDU_CHECK_INTERVAL 10

#define EDCA_WMM_COUNTER 5
#define EDCA_CHECK_INTERVAL 25
#define EDCA_BE_DEFAULT 0xA43
#define EDCA_BK_DEFAULT 0xA67
#define EDCA_BE_AGGRESSIVE 0x211
#define EDCA_BE_THRESHOLD 800
#define EDCA_VI_THRESHOLD 200
// exponential, BE count > (Others << EDCA_BE_RATIO_E)
#define EDCA_BE_RATIO_E 4

#define MAX_XMIT_HOOK_CNT 4
#define XMIT_HOOK_IDX_MASK 3
// must bigger than ip header + vlan
#define MIN_XMIT_SKB_LEN 50
#define MCAST_MAC_ADDR_BIT 1

#define SF_WIFI_ACCEL 41
#define SF_WIFI_ACCEL_FLAG 0x1a

struct siwifi_traffic_info {
    u32 pkt_cnt;
    unsigned long next_jiffies;
};

void traffic_detect_for_amsdu(struct siwifi_hw *siwifi_hw, struct siwifi_txq *txq, u8 tid);
void traffic_detect_be_edca(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif, u8 ac, u8 pkt_num);

void traffic_info_init(void);
void siwifi_hook_xmit_reigster(void);
void siwifi_hook_xmit_unreigster(void);
void siwifi_dev_traffic_in(struct sk_buff *skb, struct net_device *dev);
void siwifi_device_traffic_init(void);
void siwifi_clear_hook(struct net_device *dev);

static inline bool siwifi_skb_is_acceled(struct sk_buff *skb)
{
#ifdef CONFIG_BRIDGE_ACCELERATE
    return (skb->cb[SF_WIFI_ACCEL] == SF_WIFI_ACCEL_FLAG);
#else
    return false;
#endif
}


#endif /* _SIWIFI_TRAFFIC_H_ */
