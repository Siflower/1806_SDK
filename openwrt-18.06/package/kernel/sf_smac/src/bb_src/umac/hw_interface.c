/*
 * =====================================================================================
 *
 *       Filename:  hw_interface.c
 *
 *    Description:  support interface for Hw
 *
 *        Version:  1.0
 *        Created:  01/11/2021 02:05:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Siflower
 *
 * =====================================================================================
 */

#include "hw_interface.h"
#include <linux/etherdevice.h>
#include "reg_mac_core.h"
#include "siwifi_msg_tx.h"

extern const uint8_t legrates_lut[];
static const uint32_t legrates_tbl[] = {10, 20, 55, 110,
    60, 90, 120, 180, 240, 360, 480, 540};
static const uint32_t vht_rate_tbl[4][10] = {
    {6500, 13000, 19500, 26000, 39000, 52000, 58500, 65000, 78000, 86500},
    {13500, 27000, 40500, 54000, 81000, 108000, 121500, 135000, 162000, 180000},
    {29300, 58500, 87800, 117000, 175500, 234000, 263300, 292500, 351000, 390000},
    {58500, 117000, 175500, 234000, 351000, 468000, 526500, 585000, 702000, 780000},
};

static int8_t max_scan_rssi;
static uint8_t scan_results[MAX_SCAN_RESULTS][ETH_ALEN];
static uint8_t scan_used_idx;
static unsigned char freq_to_chan(u32 freq)
{
    if (freq > 5000) {
        return (freq - 5000) / 5;
    } else if (freq > 2000) {
        return (freq == 2484) ? 14: (freq - 2407) / 5;
    } else {
        return 0;
    }
}

static uint32_t calculate_leg_rate(uint8_t legrate)
{
    int rate_idx;
    uint32_t rate_kbps = 0;
    if (legrate > 15) {
        printk("%s invalid legrate: %u\n", __func__, legrate);
        return rate_kbps;
    }
    rate_idx = legrates_lut[legrate];
    if (rate_idx < 0 || rate_idx > 11) {
        printk("%s invalid legrate: %u\n", __func__, legrate);
        return rate_kbps;
    }
    return legrates_tbl[rate_idx];
}

static uint32_t calculate_ht_vht_rate(uint8_t bw, uint8_t mcs, uint8_t shortgi, uint8_t nss)
{
    uint32_t rate_kbps = 0;
    if (bw > BW_80MHZ || mcs >= MAX_VHT_MCS_NUM) {
        printk("%s invalid, bw: %u, mcs: %u\n", __func__, bw, mcs);
        return rate_kbps;
    }
    rate_kbps = vht_rate_tbl[bw][mcs] * (nss + 1);
    if (shortgi) {
        rate_kbps = (rate_kbps / 9) * 10;
    }
    return (rate_kbps + 49) / 100;
}

static inline uint8_t get_config_bw(uint16_t rate_config)
{
    return (rate_config & BW_TX_RCX_MASK) >> BW_TX_RCX_OFT;
}

static uint32_t get_config_rate(uint16_t rate_config)
{
    uint32_t rate_kbps = 0;
    uint32_t format = (rate_config & FORMAT_MOD_TX_RCX_MASK) >> FORMAT_MOD_TX_RCX_OFT;
    uint32_t mcs_idx = (rate_config & MCS_INDEX_TX_RCX_MASK) >> MCS_INDEX_TX_RCX_OFT;

    if (format >= FORMATMOD_HT_MF) {
        uint8_t nss, bw, mcs, sgi;

        bw = (rate_config & BW_TX_RCX_MASK) >> BW_TX_RCX_OFT;
        sgi = (rate_config & SHORT_GI_TX_RCX_MASK) >> SHORT_GI_TX_RCX_OFT;
        if (format == FORMATMOD_VHT) {
            nss = (mcs_idx & VHT_NSS_MASK) >> VHT_NSS_OFT;
            mcs = (mcs_idx & VHT_MCS_MASK);
        } else if (format == FORMATMOD_HE_SU) {
            // unsupported HE
            return rate_kbps;
        } else {
            nss = (mcs_idx & HT_NSS_MASK) >> HT_NSS_OFT;
            mcs = (mcs_idx & HT_MCS_MASK);
        }
        if (bw < 4 && mcs < 10 && nss < 2) {
            rate_kbps = vht_rate_tbl[bw][mcs] * (nss + 1);
            if (sgi) {
                rate_kbps = (rate_kbps / 9) * 10;
            }
            rate_kbps = (rate_kbps + 49) / 100;
#ifdef CONFIG_SIWIFI_DBG
            printk("bw:%u, mcs:%u, nss:%u, sgi:%u, rate:%u\n", bw, mcs, nss + 1, sgi, rate_kbps);
#endif
        }
    } else {
        if (mcs_idx < 12) {
            rate_kbps = legrates_tbl[mcs_idx];
        }
    }
    return rate_kbps;
}

static uint8_t vendor_11ac_nss_max(uint16_t mcs_map)
{
    uint8_t nss_max;

    // Go through the MCS map to check how many SS are supported
    for (nss_max = 7; nss_max > 0; nss_max--)
    {
        if (((mcs_map >> (2 * nss_max)) & MAC_VHT_MCS_MAP_MSK) != MAC_VHT_MCS_MAP_NONE)
            break;
    }

    return (nss_max);
}

static uint8_t vendor_11n_nss_max(uint8_t *mcs_set)
{
    uint8_t nss_max;

    // Go through the MCS map to check how many SS are supported
    for (nss_max = 3; nss_max > 0; nss_max--)
    {
        if (mcs_set[nss_max] != 0)
            break;
    }

    return (nss_max);
}

static void calculate_sta_tp(struct siwifi_sta *sta)
{
    uint32_t tx_bytes, rx_bytes;
    uint32_t cur_jiffies = jiffies;
    uint32_t delta_jiffies = cur_jiffies - sta->stats.last_calc_tp;

    if (delta_jiffies < HZ) {
        return;
    }

    if (cur_jiffies > sta->stats.last_calc_tp) {
        tx_bytes = (uint32_t)(sta->stats.tx_bytes - sta->stats.last_tx_bytes);
        rx_bytes = (uint32_t)(sta->stats.rx_bytes - sta->stats.last_rx_bytes);
        sta->stats.last_tx_tp = tx_bytes / delta_jiffies * HZ;
        sta->stats.last_rx_tp = rx_bytes / delta_jiffies * HZ;
    }

    sta->stats.last_calc_tp = cur_jiffies;
    sta->stats.last_tx_bytes = sta->stats.tx_bytes;
    sta->stats.last_rx_bytes = sta->stats.rx_bytes;
}

static void print_vap_status(struct vap_status_diag_info *vap_info_resp)
{
    printk("print vap status\n");
    printk("bssid: %pM", vap_info_resp->bssid);
    printk("cur_channel: %u\n", vap_info_resp->cur_channel);
    printk("second_channel: %u\n", vap_info_resp->second_channel);
    printk("bandwidth: %u\n", vap_info_resp->bandwidth);
    printk("cca_shreshold: %u\n", vap_info_resp->cca_shreshold);
    printk("sta_cnt: %u\n", vap_info_resp->sta_cnt);
    printk("scan_mode: %u\n", vap_info_resp->scan_mode);
    printk("skb_remain_buffer: %u\n", vap_info_resp->skb_remain_buffer);
    printk("vo_remain_count: %u\n", vap_info_resp->vo_remain_count);
    printk("vi_remain_count: %u\n", vap_info_resp->vi_remain_count);
    printk("be_remain_count: %u\n", vap_info_resp->be_remain_count);
    printk("bk_remain_count: %u\n", vap_info_resp->bk_remain_count);
    printk("vo_edca: 0x%x\n", vap_info_resp->vo_edca);
    printk("vi_edca: 0x%x\n", vap_info_resp->vi_edca);
    printk("be_edca: 0x%x\n", vap_info_resp->be_edca);
    printk("bk_edca: 0x%x\n", vap_info_resp->bk_edca);
    printk("vapState: %u\n", vap_info_resp->vapState);
    printk("QosEnabled: %u\n", vap_info_resp->QosEnabled);
    printk("max_aggre_timeout: %u\n", vap_info_resp->max_aggre_timeout);
    printk("max_aggre_packages: %u\n", vap_info_resp->max_aggre_packages);
    printk("cur_ch_ap_count: %u\n", vap_info_resp->cur_ch_ap_count);
    printk("cur_ch_max_rssi: %d\n", vap_info_resp->cur_ch_max_rssi);
    printk("rts_enable_reason: %u\n", vap_info_resp->rts_enable_reason);
}

static void print_sta_status(struct sta_status_diag_info *sta_info_resp)
{
    printk("\nprint sta status\n");
    printk("mac: %pM\n", sta_info_resp->mac);
    printk("dot11Cap: %u\n", sta_info_resp->dot11Cap);
    printk("wl_mode: %u\n", sta_info_resp->wl_mode);
    printk("agg_mode: %u\n", sta_info_resp->agg_mode);
    printk("rssi: %d\n", (int8_t)sta_info_resp->rssi);
    printk("snr: %d\n", sta_info_resp->snr);
    printk("tx_bandwidth: %u\n", sta_info_resp->tx_bandwidth);
    printk("rx_bandwidth: %u\n", sta_info_resp->rx_bandwidth);
    printk("tx_rate: %u\n", sta_info_resp->tx_rate);
    printk("rx_rate: %u\n", sta_info_resp->rx_rate);
    printk("tx_minrate: %u\n", sta_info_resp->tx_minrate);
    printk("tx_maxrate: %u\n", sta_info_resp->tx_maxrate);
    printk("sleep_times: %u(s)\n", sta_info_resp->sleep_times / 1000);
    printk("per: %u\n", sta_info_resp->per);
    printk("max_agg_num: %u\n", sta_info_resp->max_agg_num);
    printk("rts_rate: %u\n", sta_info_resp->rts_rate);
    printk("rts_retry_cnt: %u\n", sta_info_resp->rts_retry_cnt);
    printk("tx_pkts: %u\n", sta_info_resp->tx_pkts);
    printk("tx_fail: %u\n", sta_info_resp->tx_fail);
    printk("tx_retry_cnt: %u\n", sta_info_resp->tx_retry_cnt);
    printk("rx_pkts: %u\n", sta_info_resp->rx_pkts);
    printk("tx_unicast_bytes: %llu\n", sta_info_resp->tx_unicast_bytes);
    printk("rx_unicast_bytes: %llu\n", sta_info_resp->rx_unicast_bytes);
    printk("tx_mcast_bytes: %llu\n", sta_info_resp->tx_mcast_bytes);
    printk("rx_mcast_bytes: %llu\n", sta_info_resp->rx_mcast_bytes);
    //sta_info_resp->sta_tx_mcs_cnt[MAX_MCS_CNT];
    //sta_info_resp->sta_rx_mcs_cnt[MAX_MCS_CNT];
    printk("current_tx_rate: %u\n", sta_info_resp->current_tx_rate);
    printk("tx_prev_rate: %u\n", sta_info_resp->tx_prev_rate);
    printk("tx_th: %u kbps\n", sta_info_resp->tx_th * 8 / 1000);
    printk("rx_th: %u kbps\n", sta_info_resp->rx_th * 8 / 1000);
    printk("connected_time: %u(s)\n", sta_info_resp->connected_time);
    printk("QosEnabled :%u\n", sta_info_resp->QosEnabled);
    printk("mimo_num: %u\n", sta_info_resp->mimo_num);
    printk("short_GI: %u\n", sta_info_resp->short_GI);
    printk("wl_Mode: %u\n", sta_info_resp->wl_Mode);
    printk("Tx_bandwidth: %u\n", sta_info_resp->Tx_bandwidth);
    printk("TXPowerOffset: %u\n", sta_info_resp->TXPowerOffset);
    printk("rx_sequence_jump_counter: %u\n", sta_info_resp->rx_sequence_jump_counter);
    printk("vo_counter: %u\n", sta_info_resp->vo_counter);
    printk("vi_counter: %u\n", sta_info_resp->vi_counter);
    printk("be_counter: %u\n", sta_info_resp->be_counter);
    printk("bk_counter: %u\n", sta_info_resp->bk_counter);
    printk("tx_retry_ratio: %u\n", sta_info_resp->tx_retry_ratio);
}

int vendor_get_vap_status(struct net_device *dev, struct vap_status_diag_info *data)
{
    int tid;
    int ret = -1;
    struct siwifi_hw *siwifi_hw = NULL;
    struct siwifi_vif *siwifi_vif = netdev_priv(dev);
    struct vap_status_diag_info vap_info_resp;
    struct siwifi_sta *siwifi_sta;
    struct siwifi_txq *txq;

    if (siwifi_vif == NULL) {
        printk(" no vif \n");
        return ret;
    }

    siwifi_hw = siwifi_vif->siwifi_hw;
    if (siwifi_hw == NULL) {
        printk("no siwifi hw\n");
        return ret;
    }

    if (SIWIFI_VIF_TYPE(siwifi_vif) != NL80211_IFTYPE_AP) {
        printk(" VIF TYPE is %u\n", SIWIFI_VIF_TYPE(siwifi_vif));
        return ret;
    }
    if (!siwifi_vif->ap_settings) {
        printk(" Ap is not starting\n");
        return ret;
    }

    memset(&vap_info_resp, 0, sizeof(vap_info_resp));

    memcpy(vap_info_resp.bssid, dev->dev_addr, ETH_ALEN);
    vap_info_resp.cur_channel = freq_to_chan(siwifi_vif->ap_settings->chandef.chan->center_freq);
    vap_info_resp.second_channel = freq_to_chan(siwifi_vif->ap_settings->chandef.center_freq2);
    vap_info_resp.bandwidth = siwifi_vif->ap_settings->chandef.width;
    vap_info_resp.cca_shreshold = 0; //TODO
    vap_info_resp.scan_mode == siwifi_hw->scaning ? 1 : 0;
    vap_info_resp.skb_remain_buffer = 0;
    list_for_each_entry(siwifi_sta, &siwifi_vif->ap.sta_list, list) {
        vap_info_resp.sta_cnt++;
        foreach_sta_txq(siwifi_sta, txq, tid, siwifi_hw) {
            int ac_type = siwifi_tid2hwq[tid];
            int skb_ready_len = skb_queue_len(&txq->sk_list);
#ifdef CONFIG_BRIDGE_ACCELERATE
            skb_ready_len += skb_queue_len(&txq->accel_sk_list);
#endif
            if (ac_type == SIWIFI_HWQ_BE) {
                vap_info_resp.be_remain_count += skb_ready_len;
            } else if (ac_type == SIWIFI_HWQ_BK) {
                vap_info_resp.bk_remain_count += skb_ready_len;
            } else if (ac_type == SIWIFI_HWQ_VI) {
                vap_info_resp.vi_remain_count += skb_ready_len;
            } else if (ac_type == SIWIFI_HWQ_VO) {
                vap_info_resp.vo_remain_count += skb_ready_len;
            }
        }
    }
    vap_info_resp.vo_edca = nxmac_edca_ac_3_get();
    vap_info_resp.vi_edca = nxmac_edca_ac_2_get();
    vap_info_resp.be_edca = nxmac_edca_ac_1_get();
    vap_info_resp.bk_edca = nxmac_edca_ac_0_get();
    vap_info_resp.vapState = siwifi_vif->up ? 1 : 0;
    vap_info_resp.QosEnabled = 1; // wmm_enabled controlled in hostapd, here always enabled
    vap_info_resp.max_aggre_timeout = 0;
    vap_info_resp.max_aggre_packages = siwifi_hw->mod_params->ampdu_max_cnt;
    vap_info_resp.cur_ch_ap_count = scan_used_idx;
    vap_info_resp.cur_ch_max_rssi = max_scan_rssi;
    vap_info_resp.rts_enable_reason = 0; //TODO
    vap_info_resp.rts_thrshld = 0; //TODO;

    memcpy(data, &vap_info_resp, sizeof(vap_info_resp));
    print_vap_status(data);
    return ret;
}

int vendor_get_sta_status(struct net_device *dev, struct siwifi_sta *sta, struct sta_status_diag_info *data)
{
    int ret = -1;
    uint32_t tx_sample;
    struct siwifi_hw *siwifi_hw = NULL;
    struct siwifi_vif *siwifi_vif = netdev_priv(dev);
    struct sta_status_diag_info sta_info_resp;
    struct dbg_get_vendor_info_cfm cfm;

    if (siwifi_vif == NULL) {
        printk(" no vif \n");
        return ret;
    }

    siwifi_hw = siwifi_vif->siwifi_hw;
    if (siwifi_hw == NULL) {
        printk("no siwifi hw\n");
        return ret;
    }

    if (SIWIFI_VIF_TYPE(siwifi_vif) != NL80211_IFTYPE_AP) {
        printk(" VIF TYPE is %u\n", SIWIFI_VIF_TYPE(siwifi_vif));
        return ret;
    }

    memset(&sta_info_resp, 0, sizeof(sta_info_resp));
    memset(&cfm, 0, sizeof(struct dbg_get_vendor_info_cfm));
    if (siwifi_send_dbg_get_vendor_info_req(siwifi_hw, &cfm,
                0, sta->vif_idx, sta->sta_idx) == 0 && cfm.value == 0) {
        sta_info_resp.agg_mode = ((cfm.ampdu_num > 0) | cfm.amsdu_on << 1);
        sta_info_resp.tx_bandwidth = get_config_bw(cfm.tx_rate);
        sta_info_resp.tx_rate = get_config_rate(cfm.tx_rate);
        sta_info_resp.tx_minrate = get_config_rate(cfm.min_tx_rate);
        sta_info_resp.tx_maxrate = get_config_rate(cfm.max_tx_rate);
        sta_info_resp.tx_retry_cnt = cfm.tx_retry_cnt;
        sta_info_resp.max_agg_num = cfm.ampdu_num;
        sta_info_resp.rts_rate = cfm.rts_rate;
        sta_info_resp.rts_retry_cnt = cfm.rts_retry_cnt;
        sta_info_resp.current_tx_rate = sta_info_resp.tx_rate;
        sta_info_resp.tx_prev_rate = get_config_rate(cfm.tx_prev_rate);
        sta_info_resp.rx_sequence_jump_counter = cfm.rx_seq_jump_cnt;
#ifdef CONFIG_SIWIFI_DBG
        printk("received cfm: value: %u\n", cfm.value);
        printk("received cfm: ampdu_num: %u\n", cfm.ampdu_num);
        printk("received cfm: amsdu_on: %u\n", cfm.amsdu_on);
        printk("received cfm: rts_rate: %u\n", cfm.rts_rate);
        printk("received cfm: rts_retry_cnt: %u\n", cfm.rts_retry_cnt);
        printk("received cfm: tx_rate: %u\n", cfm.tx_rate);
        printk("received cfm: tx_prev_rate: %u\n", cfm.tx_prev_rate);
        printk("received cfm: min_tx_rate: %u\n", cfm.min_tx_rate);
        printk("received cfm: max_tx_rate: %u\n", cfm.max_tx_rate);
        printk("received cfm: tx_retry_cnt: %u\n", cfm.tx_retry_cnt);
        printk("received cfm: rx_seq_jump_cnt: %u\n", cfm.rx_seq_jump_cnt);
#endif
    }

    memcpy(sta_info_resp.mac, sta->mac_addr, ETH_ALEN);
    sta_info_resp.dot11Cap = 0;
    sta_info_resp.wl_mode = sta->vht ? 3 : sta->ht ? 2: 1;
    sta_info_resp.rssi = sta->stats.data_rssi;
    sta_info_resp.snr = 0; // unsupported
    sta_info_resp.rx_bandwidth = sta->stats.data_rx_bw;
    sta_info_resp.rx_rate = sta->stats.data_rx_rate;
    sta_info_resp.sleep_times = sta->stats.ps_times;
    sta_info_resp.per = 0;
    sta_info_resp.tx_pkts = sta->stats.tx_packets;
    sta_info_resp.tx_fail = sta->stats.tx_failed;
    sta_info_resp.rx_pkts = sta->stats.rx_packets;
    sta_info_resp.tx_unicast_bytes = sta->stats.tx_unicast_bytes;
    sta_info_resp.rx_unicast_bytes = sta->stats.rx_unicast_bytes;
    sta_info_resp.tx_mcast_bytes = sta->stats.tx_mcast_bytes;
    sta_info_resp.rx_mcast_bytes = sta->stats.rx_mcast_bytes;
    //sta_info_resp.sta_tx_mcs_cnt[MAX_MCS_CNT];
    //sta_info_resp.sta_rx_mcs_cnt[MAX_MCS_CNT];
    sta_info_resp.tx_th = sta->stats.last_tx_tp;
    sta_info_resp.connected_time = ktime_get_seconds() - sta->stats.connected_time;
    sta_info_resp.rx_th = sta->stats.last_rx_tp;
    sta_info_resp.QosEnabled = sta->qos;
    sta_info_resp.mimo_num = sta->stats.no_ss;
    sta_info_resp.short_GI = sta->stats.short_gi;
    sta_info_resp.wl_Mode = sta->vht ? 3 : sta->ht ? 2 : 1;
    sta_info_resp.Tx_bandwidth = sta->width;
    sta_info_resp.TXPowerOffset = 0;
    sta_info_resp.vo_counter = sta->stats.ac_pkts[AC_VO];
    sta_info_resp.vi_counter = sta->stats.ac_pkts[AC_VI];
    sta_info_resp.be_counter = sta->stats.ac_pkts[AC_BE];
    sta_info_resp.bk_counter = sta->stats.ac_pkts[AC_BK];
    tx_sample = sta_info_resp.tx_retry_cnt + sta->stats.tx_packets;
    sta_info_resp.tx_retry_ratio = tx_sample > 0 ? sta_info_resp.tx_retry_cnt / tx_sample : 0;

    memcpy(data, &sta_info_resp, sizeof(sta_info_resp));
    print_sta_status(data);
    return ret;
}


void vendor_hook_scan_req(void)
{
#ifdef CONFIG_SIWIFI_DBG
    printk("scan request\n");
#endif
    memset(scan_results, 0, sizeof(scan_results));
    scan_used_idx = 0;
    max_scan_rssi = -100;
}

void vendor_hook_scan_done(void)
{
#ifdef CONFIG_SIWIFI_DBG
    printk("scan result: ap number: %u max rssi:%d\n", scan_used_idx, max_scan_rssi);
#endif
}

void vendor_hook_scan_result_ind(uint16_t center_freq, struct ieee80211_mgmt *mgmt, int8_t rssi)
{
    uint8_t idx;

    if (rssi > max_scan_rssi) {
        max_scan_rssi = rssi;
    }
#ifdef CONFIG_SIWIFI_DBG
    printk("received: center_freq: %u da:%pM sa:%pM bssid:%pM rssi: %d\n", center_freq, mgmt->da, mgmt->sa, mgmt->bssid, rssi);
#endif
    if (scan_used_idx >= MAX_SCAN_RESULTS || mgmt == NULL) {
        return;
    }
    for (idx = 0; idx < scan_used_idx; idx++) {
        if (memcmp(&scan_results[idx][0], mgmt->bssid, ETH_ALEN) == 0) {
            return;
        }
    }
    memcpy(&scan_results[scan_used_idx][0], mgmt->bssid, ETH_ALEN);
    scan_used_idx++;
}

void vendor_hook_sta_added(struct siwifi_sta *sta, struct station_parameters *params)
{
    if (sta == NULL || params == NULL) {
        return;
    }

    // handle sta->no_ss, only consider the capability of station
    if (params->vht_capa) {
        sta->stats.no_ss = vendor_11ac_nss_max(cpu_to_le16(params->vht_capa->supp_mcs.rx_mcs_map));
    } else if (params->ht_capa) {
        sta->stats.no_ss = vendor_11n_nss_max((uint8_t *)&params->ht_capa->mcs);
    } else {
        sta->stats.no_ss = 0; // ignore HE
    }

    // handle sta->stats.short_gi
    sta->stats.short_gi = 0;
    if (sta->width == BW_20MHZ) {
        if (params->ht_capa && (cpu_to_le16(params->ht_capa->cap_info) & MAC_HTCAPA_SHORTGI_20)) {
            sta->stats.short_gi = 1;
        }
    } else if (sta->width == BW_40MHZ) {
        if (params->ht_capa && (cpu_to_le16(params->ht_capa->cap_info) & MAC_HTCAPA_SHORTGI_40)) {
            sta->stats.short_gi = 1;
        }
    } else if (sta->width == BW_80MHZ) {
        if (params->vht_capa && (cpu_to_le32(params->vht_capa->vht_cap_info) & MAC_VHTCAPA_SHORTGI_80)) {
            sta->stats.short_gi = 1;
        }
    } else if (sta->width == BW_160MHZ) {
        if (params->vht_capa && (cpu_to_le32(params->vht_capa->vht_cap_info) & MAC_VHTCAPA_SHORTGI_160)) {
            sta->stats.short_gi = 1;
        }
    } else {
        sta->stats.short_gi = 0;
    }
}

void vendor_hook_rxdata(struct siwifi_sta *sta, struct sk_buff *skb, struct hw_rxhdr *rxhdr)
{
    struct rx_vector_1 *rx_vect1 = &rxhdr->hwvect.rx_vect1;
    uint8_t nss, mcs, sgi;
    uint8_t bw = rx_vect1->ch_bw;
    uint8_t data_rssi = rx_vect1->rssi1;
    struct ethhdr *eth;

    skb_reset_mac_header(skb);
    eth = eth_hdr(skb);
    if (is_multicast_ether_addr(eth->h_dest)) {
        sta->stats.rx_mcast_bytes += skb->len;
    } else {
        sta->stats.rx_unicast_bytes += skb->len;
    }

    calculate_sta_tp(sta);

    sta->stats.data_rx_bw = bw;
    sta->stats.data_rssi = (sta->stats.data_rssi + data_rssi) / 2;
    if (rx_vect1->format_mod > FORMATMOD_VHT) {
        // unsupport FORMATMOD_HE
    } else if (rx_vect1->format_mod == FORMATMOD_VHT) {
        nss = rx_vect1->vht.nss;
        mcs = rx_vect1->vht.mcs;
        sgi = rx_vect1->vht.short_gi;
        if (nss < MAX_NSS_NUM && mcs < MAX_VHT_MCS_NUM && bw < BW_160MHZ) {
            sta->stats.data_rx_rate = calculate_ht_vht_rate(bw, mcs, sgi, nss);
        }
    } else if (rx_vect1->format_mod > FORMATMOD_NON_HT_DUP_OFDM) {
        nss = rx_vect1->ht.mcs / MAX_HT_MCS_NUM;
        mcs = rx_vect1->ht.mcs % MAX_HT_MCS_NUM;
        sgi = rx_vect1->ht.short_gi;
        if (nss < MAX_NSS_NUM && mcs < MAX_HT_MCS_NUM && bw < BW_80MHZ) {
            sta->stats.data_rx_rate = calculate_ht_vht_rate(bw, mcs, sgi, nss);
        }
    } else {
        sta->stats.data_rx_rate = calculate_leg_rate(rx_vect1->leg_rate);
    }
}

void vendor_hook_txdata(struct siwifi_sw_txhdr *sw_txhdr)
{
    uint16_t frame_len;
    struct siwifi_sta *sta = sw_txhdr->siwifi_sta;

    if (sta == NULL) {
        return;
    }

    if (sw_txhdr->hw_queue < SIWIFI_HWQ_BCMC) {
        sta->stats.ac_pkts[sw_txhdr->hw_queue]++;
    }

#ifdef CONFIG_SIWIFI_AMSDUS_TX
    if (sw_txhdr->amsdu.nb > 0) {
        frame_len = sw_txhdr->amsdu.len;
    } else
#endif
    {
        frame_len = sw_txhdr->frame_len;
    }

    if (is_multicast_ether_addr((u8 *)&sw_txhdr->desc.host.eth_dest_addr)) {
        sta->stats.tx_mcast_bytes += frame_len;
    } else {
        sta->stats.tx_unicast_bytes += frame_len;
    }
    calculate_sta_tp(sta);
}

void vendor_hook_sta_ps(struct siwifi_sta *sta, bool enable)
{
    if (enable) {
        sta->stats.last_ps_ts = jiffies;
    } else {
        if (sta->ps.active) {
            sta->stats.ps_times += jiffies_to_msecs(jiffies - sta->stats.last_ps_ts);
        }
    }
}
