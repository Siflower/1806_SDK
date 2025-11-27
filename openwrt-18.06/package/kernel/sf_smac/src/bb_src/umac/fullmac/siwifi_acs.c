/**
 *
 ******************************************************************************
 *
 * @file siwifi_vendor.c
 *
 * @brief SIWIFI driver vendor cmd
 *
 * Copyright (C) Siflower 2018-2025
 *
 ******************************************************************************
 */

#include <net/mac80211.h>
#include <net/netlink.h>

#include "siwifi_acs.h"
#include "siwifi_defs.h"
#include "hw_interface.h"
#include "siwifi_msg_tx.h"
#include "siwifi_mem.h"

extern int siwifi_freq_to_idx(struct siwifi_hw *siwifi_hw, int freq);
int siwifi_acs_scan_abord(struct siwifi_hw *siwifi_hw, bool abord)
{
	printk("%s start \n", __func__);
	if (siwifi_hw == NULL)
		return -EINVAL;
	/* Abort scan request on the vif */
	if (siwifi_hw->acs_request) {
		siwifi_kfree(siwifi_hw->acs_request);
		siwifi_hw->acs_request = NULL;
	}
    complete_all(&siwifi_hw->scan_complete);
	siwifi_hw->scaning = false;
	siwifi_hw->acs_scan = false;
	return 0;
}
#ifdef CONFIG_SIWIFI_ACS

static const struct nla_policy siwifi_vendor_attr_policy[SIWIFI_WLAN_VENDOR_ATTR_ACS_MAX + 1] = {
	[SIWIFI_WLAN_VENDOR_ATTR_ACS_HW_MODE]			= { .type = NLA_U8 },
	[SIWIFI_WLAN_VENDOR_ATTR_ACS_HT_ENABLED]		= { .type = NLA_FLAG },
	[SIWIFI_WLAN_VENDOR_ATTR_ACS_HT40_ENABLED]		= { .type = NLA_FLAG },
	[SIWIFI_WLAN_VENDOR_ATTR_ACS_VHT_ENABLED]		= { .type = NLA_FLAG },
	[SIWIFI_WLAN_VENDOR_ATTR_ACS_CHWIDTH]			= { .type = NLA_U16 },
	[SIWIFI_WLAN_VENDOR_ATTR_ACS_CH_LIST]					= { .type = NLA_BINARY },
	[SIWIFI_WLAN_VENDOR_ATTR_ACS_FREQ_LIST]					= { .type = NLA_BINARY },
};

int siwifi_acs_report_ideal_chan(struct siwifi_hw *siwifi_hw, struct hostapd_acs_chan_param *param)
{
	struct sk_buff *skb;
	int payload;
	struct siwifi_vif *vif;

	printk("%s start \n", __func__);
	vif = siwifi_hw->vif_table[param->vif_index];
	if (!vif) {
		return -EINVAL;
	}
	payload = sizeof(struct hostapd_acs_chan_param);

	skb = cfg80211_vendor_event_alloc(siwifi_hw->wiphy, &vif->wdev, payload, 0, GFP_KERNEL);
	if (!skb) {
		return -ENOMEM;
	}

	if(nla_put_u32(skb, NL80211_ATTR_IFINDEX, param->vif_index) ||
			nla_put_u8(skb, SIWIFI_WLAN_VENDOR_ATTR_ACS_PRIMARY_CHANNEL, param->primary_chan) ||
			nla_put_u8(skb, SIWIFI_WLAN_VENDOR_ATTR_ACS_SECONDARY_CHANNEL, param->sec_chan) ||
			nla_put_u8(skb, SIWIFI_WLAN_VENDOR_ATTR_ACS_HW_MODE, param->hw_mode) ||
			nla_put_u8(skb, SIWIFI_WLAN_VENDOR_ATTR_ACS_CHWIDTH, param->ch_width) ||
			nla_put_u8(skb, SIWIFI_WLAN_VENDOR_ATTR_ACS_VHT_SEG0_CENTER_CHANNEL, param->seg0_chan) ||
			nla_put_u8(skb, SIWIFI_WLAN_VENDOR_ATTR_ACS_VHT_SEG1_CENTER_CHANNEL, param->seg1_chan)
            ){
		kfree_skb(skb);
		return -ENOMEM;
	}

	cfg80211_vendor_event(skb, GFP_KERNEL);

	return 0;

}
#endif

int siwifi_calc_acs_select_chan(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif)
{
	struct siwifi_survey_info *survey;
	struct ieee80211_channel *chan;
	int i, idx, freq, ideal_freq, tmp_nd, chidx = 1;

#ifdef CONFIG_SIWIFI_ACS
	struct hostapd_acs_chan_param *rep_params;
    uint32_t sec_freq;
#endif

	//TODO: find ideal channel by scan result.
	ideal_freq = 0;
	tmp_nd = 0;

	for (i = 0; i < siwifi_hw->acs_request->n_channels; i++) {
		freq = ((struct ieee80211_channel *)siwifi_hw->acs_request->channels[i])->center_freq;
		//jump radar channel
        //if(siwifi_hw->acs_dfs_ctrl) {
		    if (freq >= 5220 && freq <= 5500)
			    continue;
        //}
		idx = siwifi_freq_to_idx(siwifi_hw, freq);
		if (idx > SCAN_CHANNEL_MAX) {
			printk("channel center frequency error, freq:%d, idx:%d\n", freq, idx);
			return -EINVAL;
		}
		survey = &siwifi_hw->survey[idx];

		if ((tmp_nd > survey->chan_time_busy_ms) || (i == 0)) {
			tmp_nd = survey->chan_time_busy_ms;
			ideal_freq = freq;
			chidx = i;
		}

		printk("ACS complete: freq: %d , noise: %d cca_busy_time: %d \n", freq, survey->noise_dbm, survey->chan_time_busy_ms);
	}

	chan = ieee80211_get_channel(siwifi_hw->wiphy, ideal_freq);

#ifdef CONFIG_SIWIFI_ACS_INTERNAL
    if (siwifi_hw->acs_internal) {
        uint32_t ch_width = (siwifi_hw->mod_params->is_hb) ? 3 : 2;
        printk("%s: select freq: %d \n", __func__, ideal_freq);
        if (ideal_freq == 5825)
            ch_width = 1;
        siwifi_hw->last_time = ktime_get_seconds();
        WARN_ON(!vif->ap_settings);
        siwifi_chandef_create(&siwifi_hw->csa_chandef, chan, ch_width);
        schedule_work(&siwifi_hw->csa_work);
    } else {
#endif
#ifdef CONFIG_SIWIFI_ACS
	    rep_params = &siwifi_hw->acs_params[vif->vif_index];
	    rep_params->primary_chan = ieee80211_frequency_to_channel(ideal_freq);
	    switch (rep_params->ch_width)
	    {
            case 20:
            case 40:
			case 80:
                if ((chidx < (siwifi_hw->acs_request->n_channels - 2)) && (rep_params->primary_chan != 48)
                        && (rep_params->primary_chan != 64) && (rep_params->primary_chan != 161)) {
                    sec_freq = ((struct ieee80211_channel *)siwifi_hw->acs_request->channels[chidx + 1])->center_freq;
                } else {
                    sec_freq = ((struct ieee80211_channel *)siwifi_hw->acs_request->channels[chidx - 1])->center_freq;
                }
                rep_params->sec_chan = ieee80211_frequency_to_channel(sec_freq);
                if ((rep_params->primary_chan >= 36) && (rep_params->primary_chan <= 48)) {
                    rep_params->seg0_chan = 42;
                } else if ((rep_params->primary_chan >= 52) && (rep_params->primary_chan <= 64)) {
                    rep_params->seg0_chan = 58;
                } else if ((rep_params->primary_chan >= 149) && (rep_params->primary_chan <= 161)) {
                    rep_params->seg0_chan = 155;
                } else {
                    rep_params->seg0_chan = 0;
                }
                rep_params->seg1_chan = 0;
                break;
            default:
                break;
        }
        printk("%s: select freq: %d chan: %d \n", __func__, ideal_freq, rep_params->primary_chan);
        siwifi_hw->acs_result.last_time = ktime_get_seconds();
        siwifi_hw->acs_result.primary_chan = rep_params->primary_chan;
        siwifi_hw->acs_result.sec_chan = rep_params->sec_chan;
        siwifi_hw->acs_result.seg0_chan = rep_params->seg0_chan;
        siwifi_hw->acs_result.seg1_chan = rep_params->seg1_chan;
        if (siwifi_acs_report_ideal_chan(siwifi_hw, &siwifi_hw->acs_params[vif->vif_index]) != 0) {
            printk("report acs channel data failed!\n");
            return -EINVAL;
        }
#endif
#ifdef CONFIG_SIWIFI_ACS_INTERNAL
    }
#endif
	return 0;
}

int siwifi_acs_scan_done(struct siwifi_hw *siwifi_hw)
{
	struct siwifi_vif *vif = NULL;
	int err = 0;

	printk("%s start \n", __func__);
	if (siwifi_hw == NULL) {
		err = -EINVAL;
		goto out;
	}

	if (siwifi_hw->acs_request) {
		vif = container_of(siwifi_hw->acs_request->wdev, struct siwifi_vif, wdev);
	}

	if (!vif) {
		err = -EINVAL;
		goto out;
	}

	if (siwifi_calc_acs_select_chan(siwifi_hw, vif) != 0) {
		printk("acs channel failed!\n");
	}

out:
	siwifi_acs_scan_abord(siwifi_hw, false);
	return err;
}

static int siwifi_acs_scan_start(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif,
		struct cfg80211_scan_request *request)
{
	int error;
    //int i;

	if (siwifi_hw->acs_request)
		siwifi_kfree(siwifi_hw->acs_request);

	siwifi_hw->acs_request = request;

    if(!siwifi_hw->acs_request->n_channels) {
	    printk("%s request null \n", __func__);
        return -1;
    }

#ifdef CONFIG_SIWIFI_SORT_SCAN
    for (i = 0; i < siwifi_hw->scan_num; i++) {
        if (siwifi_hw->bss_results[i]) {
            cfg80211_unlink_bss(siwifi_hw->wiphy, siwifi_hw->bss_results[i]);
            cfg80211_put_bss(siwifi_hw->wiphy, siwifi_hw->bss_results[i]);
        }
    }
    siwifi_hw->scan_num = 0;
    memset(siwifi_hw->scan_results, 0, sizeof(siwifi_hw->scan_results));
    memset(siwifi_hw->bss_results, 0, sizeof(siwifi_hw->bss_results));
#endif

	if ((error = siwifi_send_scanu_req(siwifi_hw, siwifi_vif, request))) {
		printk("send scanu request return error! \n");
		goto fail;
	}

	siwifi_hw->acs_scan = true;
    siwifi_hw->scaning = true;
	init_completion(&siwifi_hw->scan_complete);

	return 0;
fail:
	if (siwifi_hw->acs_request) {
		siwifi_kfree(siwifi_hw->acs_request);
		siwifi_hw->acs_request = NULL;
	}
	return error;
}

#ifdef CONFIG_SIWIFI_ACS
static int siwifi_vendor_cmd_do_acs(struct wiphy *wiphy,
				    struct wireless_dev *wdev,
				    const void *data, int data_len)
{
	struct siwifi_hw *siwifi_hw = wiphy_priv(wiphy);
	struct siwifi_vif *vif = NULL;
	struct hostapd_acs_param_t cfg_acs_params = {0};

	struct cfg80211_scan_request *request = NULL;
	struct hostapd_acs_chan_param *acs_params;

	struct nlattr *attr[SIWIFI_WLAN_VENDOR_ATTR_ACS_MAX + 1];
	struct nlattr *nla;

	uint8_t hw_mode = 0;
	int i, ret;

	printk("%s start \n", __func__);
	if (wdev) {
		vif = container_of(wdev, struct siwifi_vif, wdev);
	}

	if (!vif) {
		printk("vendor do acs: no vif\n");
		ret = -1;
		goto end;
	}
	acs_params = &siwifi_hw->acs_params[vif->vif_index];

	acs_params->vif_index = vif->vif_index;
	ret = nla_parse(attr, SIWIFI_WLAN_VENDOR_ATTR_ACS_MAX, data, data_len, siwifi_vendor_attr_policy, NULL);

	if (ret) {
		goto end;
	}

	nla = nla_find(data, data_len, SIWIFI_WLAN_VENDOR_ATTR_ACS_CH_LIST);

	if (nla) {
		cfg_acs_params.ch_list_len = nla_len(nla);
		if (cfg_acs_params.ch_list_len) {
			cfg_acs_params.ch_list = kmalloc(cfg_acs_params.ch_list_len, GFP_KERNEL);
			if ( cfg_acs_params.ch_list)
				nla_memcpy((void *)cfg_acs_params.ch_list, attr[SIWIFI_WLAN_VENDOR_ATTR_ACS_CH_LIST], cfg_acs_params.ch_list_len);
		}
	}

	nla = nla_find(data, data_len, SIWIFI_WLAN_VENDOR_ATTR_ACS_FREQ_LIST);

	if (nla) {
		cfg_acs_params.ch_list_len = nla_len(nla);
		if (cfg_acs_params.ch_list_len) {
			cfg_acs_params.freq_list = kmalloc(cfg_acs_params.ch_list_len, GFP_KERNEL);
			if ( cfg_acs_params.freq_list)
				nla_memcpy((void *)cfg_acs_params.freq_list,
                        attr[SIWIFI_WLAN_VENDOR_ATTR_ACS_FREQ_LIST], cfg_acs_params.ch_list_len);
			cfg_acs_params.ch_list_len = cfg_acs_params.ch_list_len / sizeof(uint32_t);
		}
	}

	if (attr[SIWIFI_WLAN_VENDOR_ATTR_ACS_HW_MODE]) {
		hw_mode = nla_get_u8(attr[SIWIFI_WLAN_VENDOR_ATTR_ACS_HW_MODE]);
		acs_params->hw_mode = hw_mode;
	}

	if (attr[SIWIFI_WLAN_VENDOR_ATTR_ACS_HT_ENABLED]) {
		cfg_acs_params.ht_enabled = nla_get_flag(attr[SIWIFI_WLAN_VENDOR_ATTR_ACS_HT_ENABLED]);
		acs_params->ht_enabled = cfg_acs_params.ht_enabled;
	}

	if (attr[SIWIFI_WLAN_VENDOR_ATTR_ACS_HT40_ENABLED]) {
		cfg_acs_params.ht40_enabled = nla_get_flag(attr[SIWIFI_WLAN_VENDOR_ATTR_ACS_HT40_ENABLED]);
		acs_params->ht40_enabled = cfg_acs_params.ht40_enabled;
	}

	if (attr[SIWIFI_WLAN_VENDOR_ATTR_ACS_VHT_ENABLED]) {
		cfg_acs_params.vht_enabled = nla_get_flag(attr[SIWIFI_WLAN_VENDOR_ATTR_ACS_VHT_ENABLED]);
		acs_params->vht_enabled = cfg_acs_params.vht_enabled;
	}

	if (attr[SIWIFI_WLAN_VENDOR_ATTR_ACS_CHWIDTH]) {
		cfg_acs_params.ch_width = nla_get_u16(attr[SIWIFI_WLAN_VENDOR_ATTR_ACS_CHWIDTH]);
		acs_params->ch_width = cfg_acs_params.ch_width;
	}

	if (siwifi_hw->acs_scan) {
		printk("scan already started\n");
		ret = 0;
		goto end;
	}

	if ((siwifi_hw->acs_result.last_time > 0) && (ktime_get_seconds() - siwifi_hw->acs_result.last_time < 5)) {
		printk("report last scan result\n");
		acs_params->primary_chan = siwifi_hw->acs_result.primary_chan;
		acs_params->sec_chan = siwifi_hw->acs_result.sec_chan;
		acs_params->seg0_chan = siwifi_hw->acs_result.seg0_chan;
		acs_params->seg1_chan = siwifi_hw->acs_result.seg1_chan;

		siwifi_acs_report_ideal_chan(siwifi_hw, &siwifi_hw->acs_params[vif->vif_index]);

		ret = 0;
		goto end;
	}

	request = kzalloc(sizeof(*request) + sizeof(*request->channels) * cfg_acs_params.ch_list_len, GFP_KERNEL);
	if (!request) {
		ret = -ENOMEM;
		goto free;
	}

	request->n_channels = cfg_acs_params.ch_list_len;
	request->wdev = wdev;
	request->wiphy = wiphy;

	for (i = 0; i < cfg_acs_params.ch_list_len; i++) {
		int freq = cfg_acs_params.freq_list[i];
		struct ieee80211_channel *chan;
		chan = ieee80211_get_channel(wiphy, freq);
		request->channels[i] = chan;
	}
	printk("scan params hw_mode: %d, chan_width: %d, ht: %d, ht40: %d, vht: %d \n, n_channels: %d", hw_mode, cfg_acs_params.ch_width, cfg_acs_params.ht_enabled, cfg_acs_params.ht40_enabled, cfg_acs_params.vht_enabled, cfg_acs_params.ch_list_len);

	siwifi_acs_scan_start(siwifi_hw, vif, request);

free:
	if (cfg_acs_params.ch_list) {
		siwifi_kfree(cfg_acs_params.ch_list);
		cfg_acs_params.ch_list = NULL;
	}
	if (cfg_acs_params.freq_list) {
		siwifi_kfree(cfg_acs_params.freq_list);
		cfg_acs_params.freq_list = NULL;
	}

end:
	return ret;
}

static const struct wiphy_vendor_command siwifi_vendor_commands[] = {
	{
		.info = {
			.vendor_id = SF_OUI,
			.subcmd = SIWIFI_VENDOR_CMD_DO_ACS,
		},
		.flags = WIPHY_VENDOR_CMD_NEED_NETDEV |
			 WIPHY_VENDOR_CMD_NEED_RUNNING,
		.doit = siwifi_vendor_cmd_do_acs,
	},
};

static const struct nl80211_vendor_cmd_info siwifi_vendor_events[] = {
	{
		.vendor_id = SF_OUI,
		.subcmd = SIWIFI_VENDOR_CMD_DO_ACS,
	},
};

void siwifi_set_vendor_commands(struct wiphy *wiphy)
{
	printk("Siwifi vendor commands register ! \n");
	wiphy->vendor_commands = siwifi_vendor_commands;
	wiphy->n_vendor_commands = ARRAY_SIZE(siwifi_vendor_commands);
	wiphy->vendor_events = siwifi_vendor_events;
	wiphy->n_vendor_events = ARRAY_SIZE(siwifi_vendor_events);
}
#endif

#ifdef CONFIG_SIWIFI_ACS_INTERNAL
int siwifi_do_acs(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif)
{
    struct cfg80211_scan_request *request = NULL;
    struct wiphy *wiphy = siwifi_hw->wiphy;
    struct ieee80211_supported_band *sband;
    int i = 0;
    u8 n_ssids = 1;
    uint32_t n_channels = 0;
    printk("%s start \n", __func__);
    if (!siwifi_vif) {
        printk("can't do acs with no vif\n");
        return -1;
    }
    if (siwifi_hw->acs_scan) {
        printk("scan already started\n");
        return -1;
    }
    if ((siwifi_hw->last_time > 0) && (ktime_get_seconds() - siwifi_hw->last_time < 5)) {
		printk("report last scan result\n");
        schedule_work(&siwifi_hw->csa_work);
		return 0;
    }
    sband = (siwifi_hw->mod_params->is_hb) ? wiphy->bands[NL80211_BAND_5GHZ] : wiphy->bands[NL80211_BAND_2GHZ];
    request = kzalloc(sizeof(*request) + sizeof(*request->channels) * sband->n_channels + sizeof(*request->ssids) * n_ssids, GFP_KERNEL);
    if (!request) {
        printk("alloc request fail for siwifi_do_acs\n");
        return -1;
    }
    request->wdev = &(siwifi_vif->wdev);
    request->wiphy = wiphy;
    request->n_ssids = n_ssids;
    for (i = 0; i < sband->n_channels; i++) {
        struct ieee80211_channel *chan = &sband->channels[i];
        if (chan->flags & IEEE80211_CHAN_DISABLED)
            continue;
        request->channels[n_channels] = chan;
        n_channels++;
    }
    if (n_ssids)
        request->ssids = (void *)&request->channels[n_channels];
    request->n_channels = n_channels;
    siwifi_acs_scan_start(siwifi_hw, siwifi_vif, request);
    return 0;
}
/* Convert center_freq into channel idx*/
uint16_t siwifi_freq_to_channel(uint16_t freq)
{
    if (freq == 2484)
        return 14;
    else if (freq < 2484)
        return (freq - 2407) / 5;
    else if (freq >= 4910 && freq <= 4980)
        return (freq - 4000) / 5;
    else
        return (freq - 5000) / 5;
}
static u8 *siwifi_build_bcn_csa(struct siwifi_bcn *bcn, struct siwifi_vif *siwifi_vif, u8 **real_addr, enum nl80211_chan_width width)
{
    u8 *buf, *pos;
    int i = 0;
    u8 elem_id, elem_len;
    struct cfg80211_beacon_data *new;
    struct siwifi_hw *siwifi_hw = siwifi_vif->siwifi_hw;
    struct cfg80211_ap_settings *ap_settings = siwifi_vif->ap_settings;
    u8 sec_ch;
    u8 add_len = 0, csa_ie_len = 0;
    if(!ap_settings) {
        printk("!ap_settings\n");
        return NULL;
    }
    new = &ap_settings->beacon;
    if (new->head) {
        u8 *head = siwifi_kmalloc(new->head_len, GFP_KERNEL);
        if (!head) {
            printk("!head\n");
            return NULL;
        }
        if (bcn->head)
            kfree(bcn->head);
        bcn->head = head;
        bcn->head_len = new->head_len;
        memcpy(bcn->head, new->head, new->head_len);
    }
    if (new->tail) {
        u8 *tail = siwifi_kmalloc(new->tail_len, GFP_KERNEL);
        if (!tail) {
            printk("!tail\n");
            return NULL;
        }
        if (bcn->tail)
            kfree(bcn->tail);
        bcn->tail = tail;
        bcn->tail_len = new->tail_len;
        memcpy(bcn->tail, new->tail, new->tail_len);
    }
    if (!bcn->head) {
        printk("%s, %d\n", __func__, __LINE__);
        return NULL;
    }
    bcn->tim_len = 6;
    csa_ie_len =  (width > NL80211_CHAN_WIDTH_20) ? 21 : 11;
    bcn->len = bcn->head_len + bcn->tail_len + bcn->ies_len + bcn->tim_len + csa_ie_len;
    sec_ch = (siwifi_hw->csa_chandef.chan->center_freq > siwifi_hw->csa_chandef.center_freq1) ? 1 : 3;
    buf = siwifi_kmalloc(bcn->len + 2 * cache_line_size(), GFP_KERNEL);
    if (!buf) {
        printk("%s, %d\n", __func__, __LINE__);
        return NULL;
    }
    *real_addr = buf;
    buf = buf + cache_line_size();
    // Build the beacon buffer
    pos = buf;
    memcpy(pos, bcn->head, bcn->head_len);
    pos += bcn->head_len;
    *pos++ = WLAN_EID_TIM;
    *pos++ = 4;
    *pos++ = 0;
    *pos++ = bcn->dtim;
    *pos++ = 0;
    *pos++ = 0;
    /*
     * add 5 ie for csa to tail
     * 1, 0x20
     * 2, 0x3c
     * 3, 0x3e
     * 4, 0xc4
     * 5, 0xc2
     */
    *pos++ = WLAN_EID_CHANNEL_SWITCH;
    *pos++ = 0x3;
    *pos++ = 0x0; //block tx
    *pos++ = siwifi_freq_to_channel(siwifi_hw->csa_chandef.chan->center_freq); //channel
    *pos++ = 5; //count
    add_len += 5;
    *pos++ = WLAN_EID_EXT_CHANSWITCH_ANN;
    *pos++ = 4; //len
    *pos++ = 0; //block tx
    *pos++ = 0x80;
    *pos++ = siwifi_freq_to_channel(siwifi_hw->csa_chandef.chan->center_freq); //channel
    *pos++ = 5; //count
    add_len += 6;
    if(width > NL80211_CHAN_WIDTH_20)
    {
        *pos++ = WLAN_EID_SECONDARY_CHANNEL_OFFSET;
        *pos++ = 1; //len
        *pos++ = sec_ch; //sec channel
        add_len += 3;
        *pos++ = WLAN_EID_CHANNEL_SWITCH_WRAPPER;
        *pos++ = 5; //len
        *pos++ = WLAN_EID_WIDE_BW_CHANNEL_SWITCH;
        *pos++ = 3;
        if(width == NL80211_CHAN_WIDTH_80)
        {
            *pos++ = 1; // bw 80M
            *pos++ = siwifi_freq_to_channel(siwifi_hw->csa_chandef.center_freq1); // New Channel Center Frequency Segment 0
        }
        else
        {
            *pos++ = 0; // bw HT
            *pos++ = siwifi_freq_to_channel(siwifi_hw->csa_chandef.center_freq1); // New Channel Center Frequency Segment 0
        }
        *pos++ = 0; //New Channel Center Frequency Segment 1
        add_len += 7;
    }
    if (bcn->tail) {
        memcpy(pos, bcn->tail, bcn->tail_len);
        pos += bcn->tail_len;
    }
    bcn->tail_len += add_len;
    if (bcn->ies) {
        memcpy(pos, bcn->ies, bcn->ies_len);
    }
    pos = buf + 36; // 24 + 8 + 2 + 2
    i = bcn->len - 36;
    while(i > 0)
    {
        struct cfg80211_chan_def *chandef = &siwifi_hw->chanctx_table[siwifi_vif->ch_index].chan_def;
        u16 cur_center_freq = chandef->chan->center_freq;
        u32 cur_center_freq1 = chandef->center_freq1;
        enum nl80211_chan_width cur_width = chandef->width;
        elem_id = pos[0];
        elem_len = pos[1];
        switch(elem_id)
        {
            case WLAN_EID_DS_PARAMS:
                pos[2] = siwifi_freq_to_channel(cur_center_freq);
                break;
            case WLAN_EID_HT_OPERATION:
                pos[2] = siwifi_freq_to_channel(cur_center_freq);
                break;
            case WLAN_EID_VHT_OPERATION:
                if(cur_width == NL80211_CHAN_WIDTH_80)
                    pos[3] = siwifi_freq_to_channel(cur_center_freq1);
                else
                    pos[3] = 0;
                break;
            case WLAN_EID_VHT_TX_POWER_ENVELOPE:
                pos[3] = 46;
                if(cur_width == NL80211_CHAN_WIDTH_80)
                {
                    pos[4] = 46;
                    pos[5] = 46;
                }
                else if(cur_width == NL80211_CHAN_WIDTH_40)
                    pos[4] = 46;
                break;
            default:
                break;
        }
        pos += 2;
        pos += elem_len;
        i -= 2;
        i -= elem_len;
    }
    return buf;
}
u8 * siwifi_build_bcn_after(struct siwifi_bcn *bcn, struct cfg80211_beacon_data *new, u8 **real_addr, struct siwifi_vif *vif, enum nl80211_chan_width width)
{
    struct siwifi_hw *siwifi_hw = vif->siwifi_hw;
    u8 *buf, *pos, elem_id, elem_len;
    int i = 0;
    if (new->head) {
        u8 *head = siwifi_kmalloc(new->head_len, GFP_KERNEL);
        if (!head)
            return NULL;
        if (bcn->head)
            kfree(bcn->head);
        bcn->head = head;
        bcn->head_len = new->head_len;
        memcpy(bcn->head, new->head, new->head_len);
    }
    if (new->tail) {
        u8 *tail = siwifi_kmalloc(new->tail_len, GFP_KERNEL);
        if (!tail)
            return NULL;
        if (bcn->tail)
            kfree(bcn->tail);
        bcn->tail = tail;
        bcn->tail_len = new->tail_len;
        memcpy(bcn->tail, new->tail, new->tail_len);
    }
    if (!bcn->head)
        return NULL;
    bcn->tim_len = 6;
    bcn->len = bcn->head_len + bcn->tail_len + bcn->ies_len + bcn->tim_len;
    buf = siwifi_kmalloc(bcn->len +  2 * cache_line_size(), GFP_KERNEL);
    if (!buf)
        return NULL;
    *real_addr = buf;
    buf = buf + cache_line_size();
    // Build the beacon buffer
    pos = buf;
    memcpy(pos, bcn->head, bcn->head_len);
    pos += bcn->head_len;
    *pos++ = WLAN_EID_TIM;
    *pos++ = 4;
    *pos++ = 0;
    *pos++ = bcn->dtim;
    *pos++ = 0;
    *pos++ = 0;
    if (bcn->tail) {
        memcpy(pos, bcn->tail, bcn->tail_len);
        pos += bcn->tail_len;
    }
    if (bcn->ies) {
        memcpy(pos, bcn->ies, bcn->ies_len);
    }
    pos = buf + 36; // 24 + 8 + 2 + 2
    i = bcn->len - 36;
    while(i > 0)
    {
        elem_id = pos[0];
        elem_len = pos[1];
        //such eid need to be changed when channel changed:
        switch(elem_id)
        {
            case WLAN_EID_DS_PARAMS:
                pos[2] = siwifi_freq_to_channel(siwifi_hw->csa_chandef.chan->center_freq);
                break;
            case WLAN_EID_HT_OPERATION:
                pos[2] = siwifi_freq_to_channel(siwifi_hw->csa_chandef.chan->center_freq);
                break;
            case WLAN_EID_VHT_OPERATION:
                if(width == NL80211_CHAN_WIDTH_80) {
                    pos[3] = siwifi_freq_to_channel(siwifi_hw->csa_chandef.center_freq1);
                }
                else
                    pos[3] = 0;
                break;
            case WLAN_EID_VHT_TX_POWER_ENVELOPE:
                pos[3] = 46;
                if(width == NL80211_CHAN_WIDTH_80)
                {
                    pos[4] = 46;
                    pos[5] = 46;
                }
                else if(width == NL80211_CHAN_WIDTH_40)
                    pos[4] = 46;
                break;
            default:
                break;
        }
        pos += 2;
        pos += elem_len;
        i -= 2;
        i -= elem_len;
        printk("left len %d\n",i);
    }
    return buf;
}
static void siwifi_del_bcn(struct siwifi_bcn *bcn)
{
    if (bcn->head) {
        kfree(bcn->head);
        bcn->head = NULL;
    }
    bcn->head_len = 0;
    if (bcn->tail) {
        kfree(bcn->tail);
        bcn->tail = NULL;
    }
    bcn->tail_len = 0;
    if (bcn->ies) {
        kfree(bcn->ies);
        bcn->ies = NULL;
    }
    bcn->ies_len = 0;
    bcn->tim_len = 0;
    bcn->dtim = 0;
    bcn->len = 0;
}
static void siwifi_del_csa(struct siwifi_vif *vif)
{
    struct siwifi_hw *siwifi_hw = vif->siwifi_hw;
    struct siwifi_csa *csa = vif->ap.csa;
    if (!csa)
        return;
    siwifi_ipc_elem_var_deallocs(siwifi_hw, &csa->elem);
    siwifi_del_bcn(&csa->bcn);
    kfree(csa);
    vif->ap.csa = NULL;
}
static void siwifi_acs_csa_finish(struct work_struct *ws)
{
    struct siwifi_csa *csa = container_of(ws, struct siwifi_csa, work);
    struct siwifi_vif *vif = csa->vif;
    struct siwifi_hw *siwifi_hw = vif->siwifi_hw;
    int error = csa->status;
    if (!error)
        error = siwifi_send_bcn_change(siwifi_hw, vif->vif_index, csa->elem.dma_addr,
                csa->bcn.len, csa->bcn.head_len,
                csa->bcn.tim_len, NULL);
    if (error)
        cfg80211_stop_iface(siwifi_hw->wiphy, &vif->wdev, GFP_KERNEL);
    else {
        mutex_lock(&vif->wdev.mtx);
        __acquire(&vif->wdev.mtx);
        spin_lock_bh(&siwifi_hw->cb_lock);
        siwifi_chanctx_unlink(vif);
        siwifi_chanctx_link(vif, csa->ch_idx, &csa->chandef);
        if (siwifi_hw->cur_chanctx == csa->ch_idx) {
            siwifi_radar_detection_enable_on_cur_channel(siwifi_hw);
            siwifi_txq_vif_start(vif, SIWIFI_TXQ_STOP_CHAN, siwifi_hw);
        } else
        {
            siwifi_txq_vif_stop(vif, SIWIFI_TXQ_STOP_CHAN, siwifi_hw);
        }
        spin_unlock_bh(&siwifi_hw->cb_lock);
        cfg80211_ch_switch_notify(vif->ndev, &csa->chandef);
        mutex_unlock(&vif->wdev.mtx);
        __release(&vif->wdev.mtx);
    }
    vif->ap.channel_switching = false;
    siwifi_del_csa(vif);
}
static int siwifi_acs_channel_switch(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif)
{
    struct siwifi_ipc_elem_var elem;
    struct siwifi_bcn *bcn, *bcn_after;
    struct siwifi_csa *csa;
    u16 csa_oft[BCN_MAX_CSA_CPT];
    u8 *buf;
    u8 *real_addr;
    int error = 0;
    int csa_count = 5;
    struct cfg80211_beacon_data *new;
    struct cfg80211_ap_settings *ap_settings = vif->ap_settings;
    printk("%s start \n", __func__);
    if(!ap_settings) {
        return -EINVAL;
    }
    if (vif->ap.csa) {
        siwifi_del_csa(vif);
    }
    /* Build the new beacon with CSA IE */
    bcn = &vif->ap.bcn;
    buf = siwifi_build_bcn_csa(bcn, vif, &real_addr, siwifi_hw->csa_chandef.width);
    if (!buf) {
        return -ENOMEM;
    }
    memset(csa_oft, 0, sizeof(csa_oft));
    csa_oft[0] = 4 + bcn->head_len + bcn->tim_len;
    csa_oft[1] = 10 + bcn->head_len + bcn->tim_len;
    if ((error = siwifi_ipc_elem_var_allocs(siwifi_hw, &elem, bcn->len,
                    DMA_TO_DEVICE, buf, NULL, NULL, real_addr))) {
        goto end;
    }
    /* Build the beacon to use after CSA. It will only be sent to fw once
       CSA is over, but do it before sending the beacon as it must be ready
       when CSA is finished. */
    csa = siwifi_kzalloc(sizeof(struct siwifi_csa), GFP_KERNEL);
    if (!csa) {
        error = -ENOMEM;
        goto end;
    }
    bcn_after = &csa->bcn;
    new = &ap_settings->beacon;
    buf = siwifi_build_bcn_after(bcn_after, new, &real_addr, vif, siwifi_hw->csa_chandef.width);
    if (!buf) {
        error = -ENOMEM;
        printk("siwifi_send_bcn_after failed\n");
        siwifi_del_csa(vif);
        goto end;
    }
    if ((error = siwifi_ipc_elem_var_allocs(siwifi_hw, &csa->elem, bcn_after->len,
                    DMA_TO_DEVICE, buf, NULL, NULL, real_addr))) {
        goto end;
    }
    vif->ap.csa = csa;
    csa->vif = vif;
    csa->chandef = siwifi_hw->csa_chandef;
    if(SIWIFI_VIF_TYPE(vif) != NL80211_IFTYPE_AP)
        printk("**** err: vif is not ap\n");
    else
        vif->ap.channel_switching = true;

    /* Send new Beacon. FW will extract channel and count from the beacon */
    error = siwifi_send_bcn_change(siwifi_hw, vif->vif_index, elem.dma_addr,
            bcn->len, bcn->head_len, bcn->tim_len, csa_oft);
    if (error) {
        printk("siwifi_send_bcn_change failed\n");
        if(SIWIFI_VIF_TYPE(vif) == NL80211_IFTYPE_AP)
            vif->ap.channel_switching = false;
        siwifi_del_csa(vif);
        goto end;
    } else {
        printk("siwifi_send_bcn_change success\n");
        INIT_WORK(&csa->work, siwifi_acs_csa_finish);
        cfg80211_ch_switch_started_notify(vif->ndev, &csa->chandef, csa_count);
    }
end:
    siwifi_ipc_elem_var_deallocs(siwifi_hw, &elem);
    return error;
}
void siwifi_channel_ctxt_switch(struct work_struct *ws)
{
    struct siwifi_hw *siwifi_hw = container_of(ws, struct siwifi_hw, csa_work);
    struct siwifi_vif *siwifi_vif, *__siwifi_vif;
    int ret;
    list_for_each_entry_safe(siwifi_vif, __siwifi_vif, &siwifi_hw->vifs, list)
    {
        if (siwifi_vif->up && SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_AP) {
            ret = siwifi_acs_channel_switch(siwifi_hw, siwifi_vif);
        }
    }
}
int siwifi_get_center_freq1(enum nl80211_chan_width width , unsigned long freq, int freq1_diff)
{
    unsigned int center_freq1 = freq, j;
    unsigned int vht80[] = { 5180, 5260, 5500, 5580, 5660, 5745 };
    switch (width) {
    case NL80211_CHAN_WIDTH_80:
        /* setup center_freq1 */
        for (j = 0; j < ARRAY_SIZE(vht80); j++) {
            if (freq >= vht80[j] && freq < vht80[j] + 80)
                break;
        }
        if (j == ARRAY_SIZE(vht80))
            break;
        center_freq1 = vht80[j] + 30;
        break;
    default:
        center_freq1 = freq + freq1_diff;
        break;
    }
    return center_freq1;
}
void siwifi_chandef_create(struct cfg80211_chan_def *chandef,
        struct ieee80211_channel *chan,
        enum nl80211_chan_width width)
{
    if (WARN_ON(!chan))
        return;
    memset(chandef, 0, sizeof(struct cfg80211_chan_def));
    chandef->chan = chan;
    chandef->center_freq2 = 0;
    chandef->width = width;
    switch(width)
    {
        case NL80211_CHAN_WIDTH_20_NOHT:
        case NL80211_CHAN_WIDTH_20:
            chandef->center_freq1 = siwifi_get_center_freq1(width , chan->center_freq, 0);
            break;
        case NL80211_CHAN_WIDTH_40:
            //we only support
            chandef->center_freq1 = siwifi_get_center_freq1(width , chan->center_freq, 10);
            break;
        case NL80211_CHAN_WIDTH_80:
            chandef->center_freq1 = siwifi_get_center_freq1(width , chan->center_freq, 0);
            break;
        default:
            WARN_ON(1);
            break;
    }
}
extern int siwifi_send_dbg_get_vendor_mp_info_req(struct siwifi_hw *siwifi_hw, struct dbg_get_vendor_mp_info_cfm *cfm);
int siwifi_check_cca(struct siwifi_vif *siwifi_vif)
{
    struct siwifi_hw *siwifi_hw = NULL;
    struct dbg_get_vendor_mp_info_cfm cfm;
    struct siwifi_vif *vif = NULL;
    printk("%s\n", __func__);
    if (!siwifi_vif)
        return -1;
    siwifi_hw = siwifi_vif->siwifi_hw;
    siwifi_hw->acs_internal = true;
    memset(&cfm, 0, sizeof(struct dbg_get_vendor_mp_info_cfm));
    if (!siwifi_send_dbg_get_vendor_mp_info_req(siwifi_hw, &cfm)) {
        printk("vif %s cca total=%d us; cca=%d us; cca20=%d us;cca40=%d us\n",
                siwifi_vif->ndev->name,
                cfm.last_cal_time_us,
                cfm.cca_busy_time_us,
                cfm.cca20_busy_time_us,
                cfm.cca40_busy_time_us
                );
        // cca > 95%
        if (cfm.cca_busy_time_us < (cfm.last_cal_time_us * 95 / 100))
            return -1;
        // check repeater
        list_for_each_entry(vif, &siwifi_hw->vifs, list) {
            if (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_STATION)
                return -1;
        }
        // get the optimal channel (acs)
        siwifi_do_acs(siwifi_hw, siwifi_vif);
    }
    return 0;
}
int siwifi_fast_channel_switch(struct siwifi_hw *siwifi_hw)
{
    uint32_t ch_width = (siwifi_hw->mod_params->is_hb) ? 3 : 2;
    struct siwifi_vif *vif = NULL;
    struct ieee80211_channel *chan = NULL;
    int ret = 0, current_freq = 0, select_freq = 0;
    uint32_t current_second = ktime_get_seconds();
    int random_num = current_second & 0x3;

    if (!siwifi_hw->mod_params->is_hb)
        return 0;

    if (current_second - siwifi_hw->fast_csa_time < 5) {
        printk("skip fast csa: less than 5 seconds have passed since the last csa\n");
        return 0;
    }
    spin_lock_bh(&siwifi_hw->cb_lock);
    list_for_each_entry(vif, &siwifi_hw->vifs, list) {
        if (!vif->up)
            continue;
        if (SIWIFI_VIF_TYPE(vif) == NL80211_IFTYPE_AP && (siwifi_hw->chanctx_table[vif->ch_index].chan_def.chan)) {
            current_freq = siwifi_hw->chanctx_table[vif->ch_index].chan_def.chan->center_freq;
        }
        if (SIWIFI_VIF_TYPE(vif) == NL80211_IFTYPE_STATION) {
            ret = -1;
        }
    }
    if (ret || !current_freq)
    {
        spin_unlock_bh(&siwifi_hw->cb_lock);
        return 0;
    }
    if (current_freq < 5240)
        select_freq = 5745 + random_num * 20;
    else
        select_freq = 5180 + random_num * 20;
    printk("%s: select freq: %d \n", __func__, select_freq);
    // change channel
    chan = ieee80211_get_channel(siwifi_hw->wiphy, select_freq);
    //WARN_ON(!vif->ap_settings);
    if(!vif->ap_settings){
        printk("!vif->ap_settings\n");
    }
    siwifi_chandef_create(&siwifi_hw->csa_chandef, chan, ch_width);
    schedule_work(&siwifi_hw->csa_work);
    siwifi_hw->fast_csa_time = current_second;
    spin_unlock_bh(&siwifi_hw->cb_lock);
    return 1;
}
#endif
