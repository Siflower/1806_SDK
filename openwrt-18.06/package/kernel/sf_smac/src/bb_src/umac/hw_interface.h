/*
 * =====================================================================================
 *
 *       Filename:  hw_interface.h
 *
 *    Description:  support interface for HW
 *
 *        Version:  1.0
 *        Created:  01/11/2021 09:42:44 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Siflower
 *
 * =====================================================================================
 */

#ifndef _HW_INTERFACE_H_
#define _HW_INTERFACE_H_

#ifdef CONFIG_VDR_HW

#include <linux/if_ether.h>
#include <linux/ieee80211.h>
#include <net/cfg80211.h>
#include "siwifi_defs.h"

#define BW_20MHZ                0
#define BW_40MHZ                1
#define BW_80MHZ                2
#define BW_160MHZ               3
#define MAC_VHT_MCS_MAP_MSK     0x03
#define MAC_VHT_MCS_MAP_NONE    0x03
#define MAC_HTCAPA_SHORTGI_20   BIT(5)
#define MAC_HTCAPA_SHORTGI_40   BIT(6)
#define MAC_VHTCAPA_SHORTGI_80  BIT(5)
#define MAC_VHTCAPA_SHORTGI_160 BIT(6)

#define MAX_NSS_NUM             2
#define MAX_HT_MCS_NUM          8
#define MAX_VHT_MCS_NUM         10

#define MAX_SCAN_RESULTS        64

#define MCS_INDEX_TX_RCX_OFT    0
#define MCS_INDEX_TX_RCX_MASK   (0X7F << MCS_INDEX_TX_RCX_OFT)
#define BW_TX_RCX_OFT           7
#define BW_TX_RCX_MASK          (0X3 << BW_TX_RCX_OFT)
#define SHORT_GI_TX_RCX_OFT     9
#define SHORT_GI_TX_RCX_MASK    (0x1 << SHORT_GI_TX_RCX_OFT)
#define FORMAT_MOD_TX_RCX_OFT   11
#define FORMAT_MOD_TX_RCX_MASK  (0X7 << FORMAT_MOD_TX_RCX_OFT)
#define VHT_NSS_OFT             4
#define VHT_NSS_MASK            (0x7 << VHT_NSS_OFT)
#define VHT_MCS_OFT             0
#define VHT_MCS_MASK            (0xF << VHT_MCS_OFT)
#define HT_NSS_OFT              3
#define HT_NSS_MASK             (0x3 << HT_NSS_OFT)
#define HT_MCS_OFT              0
#define HT_MCS_MASK             (0x7 << HT_MCS_OFT)
#define FORMATMOD_NON_HT        0
#define FORMATMOD_NON_HT_DUP_OFDM 1
#define FORMATMOD_HT_MF         2
#define FORMATMOD_HT_GF         3
#define FORMATMOD_VHT           4

typedef struct vap_status_diag_info {
    /* basic setting */
    unsigned char bssid[ETH_ALEN];
    unsigned char cur_channel;
    unsigned char second_channel;
    unsigned char bandwidth;
    unsigned char dig;
    unsigned char cca_shreshold;
    unsigned char channel_usage;
    unsigned char hisi_dig;
    unsigned char hisi_cca_shreshold[2];
    unsigned char sta_cnt;
    unsigned char scan_mode;
    unsigned short noise;
    unsigned short cca;
    unsigned int hisi_channel_busy;

    /* agg setting */
    unsigned int skb_remain_buffer;
    unsigned int vo_remain_count;
    unsigned int vi_remain_count;
    unsigned int be_remain_count;
    unsigned int bk_remain_count;

    /* edca setting */
    unsigned int vo_edca;
    unsigned int vi_edca;
    unsigned int be_edca;
    unsigned int bk_edca;
    unsigned char vapState;
    unsigned char resv[3];
    unsigned char QosEnabled;
    unsigned char nhm;
    unsigned char clm;
    unsigned char pad;
    unsigned int check_txhang_counter;
    unsigned int check_rxhang_counter;
    unsigned int check_beaconhang_counter;
    unsigned int max_aggre_timeout;
    unsigned int max_aggre_packages;
    unsigned int cur_ch_ap_count;
    unsigned int cur_ch_max_rssi;
    unsigned int rts_enable_reason;
    unsigned int rts_thrshld;
    unsigned int arp_req_cnt1;
    unsigned int arp_req_cnt2;
    unsigned int arp_resp_cnt1;
    unsigned int arp_resp_cnt2;
} IEEE80211_VAP_STATUS_INFO_ST;

#define MAX_MCS_CNT 16
#define MAX_RTT_NUM_PER_QUERY 1
#define GLORY_PPS_MAX 1
typedef struct sta_status_diag_info {
    unsigned char mac[ETH_ALEN];
    unsigned char dot11Cap;
    unsigned char chip_vendor[3];
    unsigned char wl_mode;
    unsigned char agg_mode;
    unsigned char rssi;
    unsigned char snr;
    unsigned char tx_bandwidth;
    unsigned char rx_bandwidth;
    unsigned short tx_rate;
    unsigned short rx_rate;
    unsigned short tx_minrate;
    unsigned short tx_maxrate;
    unsigned short sleep_times;
    unsigned int tx_retry_cnt;
    unsigned char per;
    unsigned char max_agg_num;
    unsigned char rts_rate;
    unsigned char rts_retry_cnt;
    unsigned int tx_pkts;
    unsigned int tx_fail;
    unsigned int rx_pkts;
    unsigned long long tx_unicast_bytes;
    unsigned long long rx_unicast_bytes;
    unsigned long long tx_mcast_bytes;
    unsigned long long rx_mcast_bytes;
    unsigned int sta_tx_mcs_cnt[MAX_MCS_CNT];
    unsigned int sta_rx_mcs_cnt[MAX_MCS_CNT];
    unsigned int tx_spending;
    unsigned int tx_th;
    unsigned int tx_spending_smoothing_rate;
    unsigned int current_tx_rate;
    unsigned int tx_prev_rate;
    unsigned int connected_time;
    /* mesh to do */
    unsigned int rx_th;
    unsigned int inactive_time;
    unsigned int master_inactive_time;
    unsigned int plink_state;
    unsigned char QosEnabled;
    unsigned char mimo_num;
    unsigned char short_GI;
    unsigned char wl_Mode;
    unsigned char Tx_bandwidth;
    unsigned char temp[2];
    unsigned char TXPowerOffset;
    unsigned int rx_sequence_jump_counter;
    unsigned int vo_counter;
    unsigned int vi_counter;
    unsigned int be_counter;
    unsigned int bk_counter;
    unsigned int max_xmit_time_delay;
    unsigned int xmit_delaying_times;
    unsigned int max_kernel_time_delay;
    unsigned int kernel_delaying_times;
    unsigned int max_eth_time_delay;
    unsigned int eth_delaying_times;
    unsigned int max_e2e_time_delay;
    unsigned int e2e_delaying_times;
    unsigned int xmit_softq_timeout_counter;
    unsigned int xmit_softq_ready_counter;
    unsigned int tx_retry_ratio;
    //delay_info wifi_delay_info;
    unsigned short rtt[MAX_RTT_NUM_PER_QUERY];
    unsigned char pps[GLORY_PPS_MAX];
} IEEE80211_STA_STATUS_INFO_ST;

extern int vendor_get_vap_status(struct net_device *dev, struct vap_status_diag_info *data);
extern int vendor_get_sta_status(struct net_device *dev, struct siwifi_sta* sta, struct sta_status_diag_info *data);
extern void vendor_hook_scan_req(void);
extern void vendor_hook_scan_done(void);
extern void vendor_hook_scan_result_ind(uint16_t center_freq, struct ieee80211_mgmt *mgmt, int8_t rssi);
extern void vendor_hook_sta_added(struct siwifi_sta *sta, struct station_parameters *params);
extern void vendor_hook_txdata(struct siwifi_sw_txhdr *sw_txhdr);
extern void vendor_hook_rxdata(struct siwifi_sta *sta, struct sk_buff *skb, struct hw_rxhdr *rxhdr);
extern void vendor_hook_sta_ps(struct siwifi_sta *sta, bool enable);
#endif /* CONFIG_VDR_HW */
#endif /* _HW_INTERFACE_H_ */
