/**
******************************************************************************
*
* @file siwifi_mod_params.c
*
* @brief Set configuration according to modules parameters
*
* Copyright (C) Siflower 2018-2025
*
******************************************************************************
*/
#include <linux/module.h>

#include "siwifi_defs.h"
#include "siwifi_tx.h"
#include "hal_desc.h"
#include "siwifi_cfgfile.h"
#include "reg_access.h"
#include "siwifi_compat.h"
#include "siwifi_mem.h"
#include "ipc_host.h"
#include "siwifi_msg_tx.h"

#define COMMON_PARAM(name, default_softmac, default_fullmac)    \
    .name = default_fullmac,
#define SOFTMAC_PARAM(name, default)
#define FULLMAC_PARAM(name, default) .name = default,

#ifndef CONFIG_SF16A18_WIFI_LB_LA_ENABLE
extern struct siwifi_mod_params hb_mod_param;
module_param_named(amsdu_maxnb_hb, hb_mod_param.amsdu_maxnb, int, S_IRUGO | S_IWUSR);
module_param_named(ap_uapsd_on_hb, hb_mod_param.ap_uapsd_on, bool, S_IRUGO);
module_param_named(uapsd_timeout_hb, hb_mod_param.uapsd_timeout, int, S_IRUGO | S_IWUSR);
module_param_named(ps_on_hb, hb_mod_param.ps_on, bool, S_IRUGO);
module_param_named(amsdu_rx_max_hb, hb_mod_param.amsdu_rx_max, int, S_IRUGO);
module_param_named(rts_cts_change_hb, hb_mod_param.rts_cts_change, int, S_IRUGO);
module_param_named(nss_hb, hb_mod_param.nss, int, S_IRUGO);
#endif

#ifndef CONFIG_SF16A18_WIFI_HB_LA_ENABLE
extern struct siwifi_mod_params lb_mod_param;
module_param_named(amsdu_maxnb_lb, lb_mod_param.amsdu_maxnb, int, S_IRUGO | S_IWUSR);
module_param_named(ap_uapsd_on_lb, lb_mod_param.ap_uapsd_on, bool, S_IRUGO);
module_param_named(uapsd_timeout_lb, lb_mod_param.uapsd_timeout, int, S_IRUGO | S_IWUSR);
module_param_named(ps_on_lb, lb_mod_param.ps_on, bool, S_IRUGO);
module_param_named(amsdu_rx_max_lb, lb_mod_param.amsdu_rx_max, int, S_IRUGO);
module_param_named(rts_cts_change_lb, lb_mod_param.rts_cts_change, int, S_IRUGO);
module_param_named(nss_lb, lb_mod_param.nss, int, S_IRUGO);
#endif

/* Regulatory rules */
static const struct ieee80211_regdomain siwifi_regdom = {
    .n_reg_rules = 3,
    .alpha2 = "99",
    .reg_rules = {
        REG_RULE(2412 - 10, 2472 + 10, 40, 0, 1000, 0),
        REG_RULE(2484 - 10, 2484 + 10, 20, 0, 1000, 0),
        REG_RULE(5150 - 10, 5850 + 10, 80, 0, 1000, 0),
    }
};

static const int mcs_map_to_rate[4][3] = {
    [PHY_CHNL_BW_20][IEEE80211_VHT_MCS_SUPPORT_0_7] = 72,
    [PHY_CHNL_BW_20][IEEE80211_VHT_MCS_SUPPORT_0_8] = 86,
    [PHY_CHNL_BW_20][IEEE80211_VHT_MCS_SUPPORT_0_9] = 86,
    [PHY_CHNL_BW_40][IEEE80211_VHT_MCS_SUPPORT_0_7] = 150,
    [PHY_CHNL_BW_40][IEEE80211_VHT_MCS_SUPPORT_0_8] = 180,
    [PHY_CHNL_BW_40][IEEE80211_VHT_MCS_SUPPORT_0_9] = 200,
    [PHY_CHNL_BW_80][IEEE80211_VHT_MCS_SUPPORT_0_7] = 325,
    [PHY_CHNL_BW_80][IEEE80211_VHT_MCS_SUPPORT_0_8] = 390,
    [PHY_CHNL_BW_80][IEEE80211_VHT_MCS_SUPPORT_0_9] = 433,
    [PHY_CHNL_BW_160][IEEE80211_VHT_MCS_SUPPORT_0_7] = 650,
    [PHY_CHNL_BW_160][IEEE80211_VHT_MCS_SUPPORT_0_8] = 780,
    [PHY_CHNL_BW_160][IEEE80211_VHT_MCS_SUPPORT_0_9] = 866,
};

#define MAX_VHT_RATE(bw_max, map, nss) (mcs_map_to_rate[bw_max][map] * nss)

/**
 * Do some sanity check
 *
 */
static int siwifi_check_fw_hw_feature(struct siwifi_hw *siwifi_hw,
                                    struct wiphy *wiphy)
{
    u32_l sys_feat = siwifi_hw->version_cfm.features;
    u32_l mac_feat = siwifi_hw->version_cfm.version_machw_1;
    u32_l phy_feat = siwifi_hw->version_cfm.version_phy_1;
    u32_l phy_vers = siwifi_hw->version_cfm.version_phy_2;
    u16_l max_sta_nb = siwifi_hw->version_cfm.max_sta_nb;
    u8_l max_vif_nb = siwifi_hw->version_cfm.max_vif_nb;
    int bw, res = 0;
    int amsdu_rx;

    if (!(sys_feat & BIT(MM_FEAT_UMAC_BIT))) {
        wiphy_err(wiphy,
                  "Loading softmac firmware with fullmac driver\n");
        res = -1;
    }

    if (!(sys_feat & BIT(MM_FEAT_ANT_DIV_BIT))) {
        siwifi_hw->mod_params->ant_div = false;
    }

    if (!(sys_feat & BIT(MM_FEAT_VHT_BIT))) {
        siwifi_hw->mod_params->vht_on = false;
    }

    // Check if HE is supported
    if (!(sys_feat & BIT(MM_FEAT_HE_BIT)))
        siwifi_hw->mod_params->he_on = false;

    if (!(sys_feat & BIT(MM_FEAT_PS_BIT))) {
        siwifi_hw->mod_params->ps_on = false;
    }

    /* AMSDU (non)support implies different shared structure definition
       so insure that fw and drv have consistent compilation option */
    if (sys_feat & BIT(MM_FEAT_AMSDU_BIT)) {
#ifndef CONFIG_SIWIFI_SPLIT_TX_BUF
        wiphy_err(wiphy,
                  "AMSDU enabled in firmware but support not compiled in driver\n");
        res = -1;
#else
        if (siwifi_hw->mod_params->amsdu_maxnb > NX_TX_PAYLOAD_MAX)
            siwifi_hw->mod_params->amsdu_maxnb = NX_TX_PAYLOAD_MAX;
#endif /* CONFIG_SIWIFI_SPLIT_TX_BUF */
    } else {
#ifdef CONFIG_SIWIFI_SPLIT_TX_BUF
        wiphy_err(wiphy,
                  "AMSDU disabled in firmware but support compiled in driver\n");
        res = -1;
#endif /* CONFIG_SIWIFI_SPLIT_TX_BUF */
    }

    if (!(sys_feat & BIT(MM_FEAT_UAPSD_BIT))) {
        siwifi_hw->mod_params->uapsd_timeout = 0;
    }

    if (!(sys_feat & BIT(MM_FEAT_BFMEE_BIT))) {
        siwifi_hw->mod_params->bfmee = false;
    }

    if ((sys_feat & BIT(MM_FEAT_BFMER_BIT))) {
#ifndef CONFIG_SIWIFI_BFMER
        wiphy_err(wiphy,
                  "BFMER enabled in firmware but support not compiled in driver\n");
        res = -1;
#endif /* CONFIG_SIWIFI_BFMER */
        // Check PHY and MAC HW BFMER support and update parameter accordingly
        if (!(phy_feat & MDM_BFMER_BIT) || !(mac_feat & NXMAC_BFMER_BIT)) {
            siwifi_hw->mod_params->bfmer = false;
            // Disable the feature in the bitfield so that it won't be displayed
            sys_feat &= ~BIT(MM_FEAT_BFMER_BIT);
        }
    } else {
#ifdef CONFIG_SIWIFI_BFMER
        wiphy_err(wiphy,
                  "BFMER disabled in firmware but support compiled in driver\n");
        res = -1;
#else
        siwifi_hw->mod_params->bfmer = false;
#endif /* CONFIG_SIWIFI_BFMER */
    }

    if (!(sys_feat & BIT(MM_FEAT_MESH_BIT))) {
        siwifi_hw->mod_params->mesh = false;
    }

    if (!(sys_feat & BIT(MM_FEAT_TDLS_BIT))) {
        siwifi_hw->mod_params->tdls = false;
    }

    if (!(sys_feat & BIT(MM_FEAT_UF_BIT))) {
        siwifi_hw->mod_params->uf = false;
    }

    if ((sys_feat & BIT(MM_FEAT_MON_DATA_BIT))) {
#ifndef CONFIG_SIWIFI_MON_DATA
        wiphy_err(wiphy,
                  "Monitor+Data interface support (MON_DATA) is enabled in firmware but support not compiled in driver\n");
        res = -1;
#endif /* CONFIG_SIWIFI_MON_DATA */
    } else {
#ifdef CONFIG_SIWIFI_MON_DATA
        wiphy_err(wiphy,
                  "Monitor+Data interface support (MON_DATA) disabled in firmware but support compiled in driver\n");
        res = -1;
#endif /* CONFIG_SIWIFI_MON_DATA */
    }

    // Check supported AMSDU RX size
    amsdu_rx = (sys_feat >> MM_AMSDU_MAX_SIZE_BIT0) & 0x03;
    if (amsdu_rx < siwifi_hw->mod_params->amsdu_rx_max) {
        siwifi_hw->mod_params->amsdu_rx_max = amsdu_rx;
    }

    // Check supported BW
    bw = (phy_feat & MDM_CHBW_MASK) >> MDM_CHBW_LSB;
    // Check if VHT is supported
    // if (bw < 2)
    //     siwifi_hw->mod_params->vht_on = false;
    // Check if HT40 is supported
    if (bw < 1)
        siwifi_hw->mod_params->use_2040 = false;

    if (!(sys_feat & BIT(MM_FEAT_MU_MIMO_RX_BIT)) ||
        !siwifi_hw->mod_params->bfmee) {
        siwifi_hw->mod_params->murx = false;
    }

    // For now STBC is not supported in modem >= 3.0
    if (__MDM_MAJOR_VERSION(phy_vers) > 0)
        siwifi_hw->mod_params->stbc_on = false;

    if ((sys_feat & BIT(MM_FEAT_MU_MIMO_TX_BIT))) {
#ifndef CONFIG_SIWIFI_MUMIMO_TX
        wiphy_err(wiphy,
                  "MU-MIMO TX enabled in firmware but support not compiled in driver\n");
        res = -1;
#endif /* CONFIG_SIWIFI_MUMIMO_TX */
        if (!siwifi_hw->mod_params->bfmer)
            siwifi_hw->mod_params->mutx = false;
        // Check PHY and MAC HW MU-MIMO TX support and update parameter accordingly
        else if (!(phy_feat & MDM_MUMIMOTX_BIT) || !(mac_feat & NXMAC_MU_MIMO_TX_BIT)) {
                siwifi_hw->mod_params->mutx = false;
                // Disable the feature in the bitfield so that it won't be displayed
                sys_feat &= ~BIT(MM_FEAT_MU_MIMO_TX_BIT);
        }
    } else {
#ifdef CONFIG_SIWIFI_MUMIMO_TX
        wiphy_err(wiphy,
                  "MU-MIMO TX disabled in firmware but support compiled in driver\n");
        res = -1;
#else
        siwifi_hw->mod_params->mutx = false;
#endif /* CONFIG_SIWIFI_MUMIMO_TX */
    }

    if (sys_feat & BIT(MM_FEAT_WAPI_BIT)) {
        siwifi_enable_wapi(siwifi_hw);
    }

    if (sys_feat & BIT(MM_FEAT_MFP_BIT)) {
        siwifi_enable_mfp(siwifi_hw);
    }

#define QUEUE_NAME "Broadcast/Multicast queue "

    if (sys_feat & BIT(MM_FEAT_BCN_BIT)) {
#if NX_TXQ_CNT == 4
        wiphy_err(wiphy, QUEUE_NAME
                  "enabled in firmware but support not compiled in driver\n");
        res = -1;
#endif /* NX_TXQ_CNT == 4 */
    } else {
#if NX_TXQ_CNT == 5
        wiphy_err(wiphy, QUEUE_NAME
                  "disabled in firmware but support compiled in driver\n");
        res = -1;
#endif /* NX_TXQ_CNT == 5 */
    }
#undef QUEUE_NAME

#ifdef CONFIG_SIWIFI_RADAR
    if (sys_feat & BIT(MM_FEAT_RADAR_BIT)) {
        /* Enable combination with radar detection */
        wiphy->n_iface_combinations++;
    }
#else
    //Now no matter radar function is enable or not
    //we should export that we can support the limitation of DFS interface combination
    wiphy->n_iface_combinations++;
#endif /* CONFIG_SIWIFI_RADAR */

    if (sys_feat & BIT(MM_FEAT_LITE_MEM_BIT)) {
#ifndef CONFIG_WIFI_LITE_MEMORY
        wiphy_err(wiphy,
                  "LITE_MEMORY enabled in LMAC but disabled in UMAC\n");
        return -1;
#endif
    } else {
#ifdef CONFIG_WIFI_LITE_MEMORY
        wiphy_err(wiphy,
                  "LITE_MEMORY disabled in LMAC but enabled in UMAC\n");
        return -1;
#endif
    }

#ifndef CONFIG_SIWIFI_SDM
    switch (__MDM_PHYCFG_FROM_VERS(phy_feat)) {
        case MDM_PHY_CONFIG_TRIDENT:
        case MDM_PHY_CONFIG_ELMA:
            siwifi_hw->mod_params->nss = 1;
            break;
        case MDM_PHY_CONFIG_AETNENSIS:
#ifndef CONFIG_SF16A18_FULL_MASK
        case MDM_PHY_CONFIG_BEGONIA_FULLMASK:
		case MDM_PHY_CONFIG_BEGONIA:
#endif
			{
                int nss_supp = (phy_feat & MDM_NSS_MASK) >> MDM_NSS_LSB;
                if (siwifi_hw->mod_params->nss > nss_supp)
                    siwifi_hw->mod_params->nss = nss_supp;
            }
            break;
		default:
            WARN_ON(1);
            break;
    }
#endif /* CONFIG_SIWIFI_SDM */

    if (siwifi_hw->mod_params->nss < 1 || siwifi_hw->mod_params->nss > 2)
        siwifi_hw->mod_params->nss = 1;

    if (siwifi_hw->mod_params->phy_cfg < 0 || siwifi_hw->mod_params->phy_cfg > 5)
        siwifi_hw->mod_params->phy_cfg = 2;

    if (siwifi_hw->mod_params->mcs_map < 0 || siwifi_hw->mod_params->mcs_map > 2)
        siwifi_hw->mod_params->mcs_map = 0;

    wiphy_info(wiphy, "PHY features: [NSS=%d][CHBW=%d]%s\n",
               siwifi_hw->mod_params->nss,
               20 * (1 << ((phy_feat & MDM_CHBW_MASK) >> MDM_CHBW_LSB)),
               siwifi_hw->mod_params->ldpc_on ? "[LDPC]" : "");

#define PRINT_SIWIFI_FEAT(feat)                                   \
    (sys_feat & BIT(MM_FEAT_##feat##_BIT) ? "["#feat"]" : "")

    wiphy_info(wiphy, "FW features: %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
               PRINT_SIWIFI_FEAT(BCN),
               PRINT_SIWIFI_FEAT(AUTOBCN),
               PRINT_SIWIFI_FEAT(HWSCAN),
               PRINT_SIWIFI_FEAT(CMON),
               PRINT_SIWIFI_FEAT(MROLE),
               PRINT_SIWIFI_FEAT(RADAR),
               PRINT_SIWIFI_FEAT(PS),
               PRINT_SIWIFI_FEAT(UAPSD),
               PRINT_SIWIFI_FEAT(DPSM),
               PRINT_SIWIFI_FEAT(AMPDU),
               PRINT_SIWIFI_FEAT(AMSDU),
               PRINT_SIWIFI_FEAT(CHNL_CTXT),
               PRINT_SIWIFI_FEAT(REORD),
               PRINT_SIWIFI_FEAT(P2P),
               PRINT_SIWIFI_FEAT(P2P_GO),
               PRINT_SIWIFI_FEAT(UMAC),
               PRINT_SIWIFI_FEAT(VHT),
               PRINT_SIWIFI_FEAT(HE),
               PRINT_SIWIFI_FEAT(BFMEE),
               PRINT_SIWIFI_FEAT(BFMER),
               PRINT_SIWIFI_FEAT(WAPI),
               PRINT_SIWIFI_FEAT(MFP),
               PRINT_SIWIFI_FEAT(MU_MIMO_RX),
               PRINT_SIWIFI_FEAT(MU_MIMO_TX),
               PRINT_SIWIFI_FEAT(MESH),
               PRINT_SIWIFI_FEAT(TDLS),
               PRINT_SIWIFI_FEAT(ANT_DIV),
               PRINT_SIWIFI_FEAT(LITE_MEM));
#undef PRINT_SIWIFI_FEAT

    if (max_sta_nb != NX_REMOTE_STA_MAX)
    {
        wiphy_err(wiphy, "Different number of supported stations between driver and FW (%d != %d)\n",
                  NX_REMOTE_STA_MAX, max_sta_nb);
        res = -1;
    }

    if(max_vif_nb != NX_VIRT_DEV_MAX)
    {
        wiphy_err(wiphy, "Different number of supported virtual interfaces between driver and FW (%d != %d)\n",
                  NX_VIRT_DEV_MAX, max_vif_nb);
        res = -1;
    }

    return res;
}

void siwifi_set_vht_capa(struct siwifi_hw *siwifi_hw, struct wiphy *wiphy)
{
    int band;
    int i;
    int nss = siwifi_hw->mod_params->nss;
    int mcs_map;
    int bw_max;

    if (!siwifi_hw->mod_params->vht_on)
        return;

    for (band = NL80211_BAND_2GHZ; band <= NL80211_BAND_5GHZ; band++) {
        struct ieee80211_sta_vht_cap *vht_cap = NULL;

        if(wiphy->bands[band] != NULL) {
            vht_cap = &wiphy->bands[band]->vht_cap;
        }

        if (!vht_cap)
            continue;

        vht_cap->vht_supported = true;

        if (siwifi_hw->mod_params->use_80 && band == NL80211_BAND_5GHZ)
            bw_max = PHY_CHNL_BW_80;
        else if (siwifi_hw->mod_params->use_2040)
            bw_max = PHY_CHNL_BW_40;
        else
            bw_max = PHY_CHNL_BW_20;

        if (siwifi_hw->mod_params->sgi80 && (bw_max >= PHY_CHNL_BW_80))
            vht_cap->cap |= IEEE80211_VHT_CAP_SHORT_GI_80;
        if (siwifi_hw->mod_params->stbc_on)
#ifdef CONFIG_NSS_2
        vht_cap->cap |= IEEE80211_VHT_CAP_RXSTBC_2;
#else
        vht_cap->cap |= IEEE80211_VHT_CAP_RXSTBC_1;
#endif
        if (siwifi_hw->mod_params->ldpc_on)
            vht_cap->cap |= IEEE80211_VHT_CAP_RXLDPC;
        if (siwifi_hw->mod_params->bfmee) {
            vht_cap->cap |= IEEE80211_VHT_CAP_SU_BEAMFORMEE_CAPABLE;
            vht_cap->cap |= (2 * nss - 1) << IEEE80211_VHT_CAP_BEAMFORMEE_STS_SHIFT;
        }
        if (nss > 1)
            vht_cap->cap |= IEEE80211_VHT_CAP_TXSTBC;

        vht_cap->cap |= siwifi_hw->mod_params->amsdu_rx_max;

        if (siwifi_hw->mod_params->bfmer) {
            vht_cap->cap |= IEEE80211_VHT_CAP_SU_BEAMFORMER_CAPABLE;
            /* Set number of sounding dimensions */
            vht_cap->cap |= (nss - 1) << IEEE80211_VHT_CAP_SOUNDING_DIMENSIONS_SHIFT;
        }
        if (siwifi_hw->mod_params->murx)
            vht_cap->cap |= IEEE80211_VHT_CAP_MU_BEAMFORMEE_CAPABLE;
        if (siwifi_hw->mod_params->mutx)
            vht_cap->cap |= IEEE80211_VHT_CAP_MU_BEAMFORMER_CAPABLE;

        if (bw_max == PHY_CHNL_BW_20)
            // VHT spec doesn't allow MCS9 at 20MHz/1SS
            mcs_map = min_t(int, siwifi_hw->mod_params->mcs_map, IEEE80211_VHT_MCS_SUPPORT_0_8);
        else
            mcs_map = min_t(int, siwifi_hw->mod_params->mcs_map, IEEE80211_VHT_MCS_SUPPORT_0_9);

        vht_cap->vht_mcs.rx_mcs_map = cpu_to_le16(0);
        for (i = 0; i < nss; i++) {
            vht_cap->vht_mcs.rx_mcs_map |= cpu_to_le16(mcs_map << (i*2));
            vht_cap->vht_mcs.rx_highest = MAX_VHT_RATE(bw_max, mcs_map, nss);
            if (bw_max >= PHY_CHNL_BW_80)
                mcs_map = IEEE80211_VHT_MCS_SUPPORT_0_9;
        }
        for (; i < 8; i++) {
            vht_cap->vht_mcs.rx_mcs_map |= cpu_to_le16(IEEE80211_VHT_MCS_NOT_SUPPORTED << (i*2));
        }

        vht_cap->vht_mcs.tx_mcs_map = cpu_to_le16(0);
        for (i = 0; i < nss; i++) {
            vht_cap->vht_mcs.tx_mcs_map |= cpu_to_le16(mcs_map << (i*2));
            vht_cap->vht_mcs.tx_highest = MAX_VHT_RATE(bw_max, mcs_map, nss);
            if (bw_max >= PHY_CHNL_BW_80)
                mcs_map = min_t(int, siwifi_hw->mod_params->mcs_map, IEEE80211_VHT_MCS_SUPPORT_0_9);
        }
        for (; i < 8; i++) {
            vht_cap->vht_mcs.tx_mcs_map |= cpu_to_le16(
                IEEE80211_VHT_MCS_NOT_SUPPORTED << (i*2));
        }

        if (!siwifi_hw->mod_params->use_80) {
    #ifdef CONFIG_VENDOR_SIWIFI
            vht_cap->cap |= IEEE80211_VHT_CAP_NOT_SUP_WIDTH_80;
    #endif
            vht_cap->cap &= ~(IEEE80211_VHT_CAP_SHORT_GI_80 | IEEE80211_VHT_CAP_RXSTBC_MASK);
        }
    }
}

void siwifi_set_ht_capa(struct siwifi_hw *siwifi_hw, struct wiphy *wiphy)
{
    struct ieee80211_supported_band *band_5GHz = wiphy->bands[NL80211_BAND_5GHZ];
    struct ieee80211_supported_band *band_2GHz = wiphy->bands[NL80211_BAND_2GHZ];
    int i;
    int nss = siwifi_hw->mod_params->nss;

    if (!siwifi_hw->mod_params->ht_on) {
        band_2GHz->ht_cap.ht_supported = false;
        band_5GHz->ht_cap.ht_supported = false;
        return;
    }
    band_2GHz->ht_cap.ampdu_density = siwifi_hw->mod_params->ampdu_density;
    // disable SMPS, enable SMPS will cause compability problems
    // 00 static smps mode
    // 11 smps disabled
    // 01 dynamic smps mode
    band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_SM_PS;

    if (siwifi_hw->mod_params->stbc_on)
        band_2GHz->ht_cap.cap |= 1 << IEEE80211_HT_CAP_RX_STBC_SHIFT;
    if (siwifi_hw->mod_params->ldpc_on)
        band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_LDPC_CODING;
    if (siwifi_hw->mod_params->use_2040) {
        band_2GHz->ht_cap.mcs.rx_mask[4] = 0x1; /* MCS32 */
        band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_SUP_WIDTH_20_40;
        band_2GHz->ht_cap.mcs.rx_highest = cpu_to_le16(135 * nss);
    } else {
        band_2GHz->ht_cap.mcs.rx_highest = cpu_to_le16(65 * nss);
    }
    if (nss > 1)
        band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_TX_STBC;

    // Update the AMSDU max RX size
    if (siwifi_hw->mod_params->amsdu_rx_max)
        band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_MAX_AMSDU;

    if (siwifi_hw->mod_params->sgi) {
        band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_SGI_20;
        if (siwifi_hw->mod_params->use_2040) {
            band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_SGI_40;
            band_2GHz->ht_cap.mcs.rx_highest = cpu_to_le16(150 * nss);
        } else
            band_2GHz->ht_cap.mcs.rx_highest = cpu_to_le16(72 * nss);
    }
    band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_GRN_FLD;

    for (i = 0; i < nss; i++) {
        band_2GHz->ht_cap.mcs.rx_mask[i] = 0xFF;
    }

    band_5GHz->ht_cap = band_2GHz->ht_cap;
}

void siwifi_set_he_capa(struct siwifi_hw *siwifi_hw, struct wiphy *wiphy)
{
#if MY_LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0)
    struct ieee80211_supported_band *band_5GHz = wiphy->bands[NL80211_BAND_5GHZ];
    struct ieee80211_supported_band *band_2GHz = wiphy->bands[NL80211_BAND_2GHZ];
    int i;
    int nss = siwifi_hw->mod_params->nss;
    struct ieee80211_sta_he_cap *he_cap;
    int mcs_map;

    if (!siwifi_hw->mod_params->he_on) {
        band_2GHz->iftype_data = NULL;
        band_2GHz->n_iftype_data = 0;
        band_5GHz->iftype_data = NULL;
        band_5GHz->n_iftype_data = 0;
        return;
    }

    he_cap = (struct ieee80211_sta_he_cap *) &band_2GHz->iftype_data->he_cap;
    he_cap->has_he = true;
    he_cap->he_cap_elem.phy_cap_info[1] |= IEEE80211_HE_PHY_CAP1_HE_LTF_AND_GI_FOR_HE_PPDUS_0_8US;
    if (siwifi_hw->mod_params->use_2040) {
        he_cap->he_cap_elem.phy_cap_info[0] |=
                        IEEE80211_HE_PHY_CAP0_CHANNEL_WIDTH_SET_40MHZ_IN_2G |
                        IEEE80211_HE_PHY_CAP0_CHANNEL_WIDTH_SET_40MHZ_80MHZ_IN_5G;
    }
    if (siwifi_hw->mod_params->ldpc_on) {
        he_cap->he_cap_elem.phy_cap_info[1] |= IEEE80211_HE_PHY_CAP1_LDPC_CODING_IN_PAYLOAD;
    } else {
        // If no LDPC is supported, we have to limit to MCS0_9, as LDPC is mandatory
        // for MCS 10 and 11
        siwifi_hw->mod_params->he_mcs_map = min_t(int, siwifi_hw->mod_params->mcs_map,
                                                IEEE80211_HE_MCS_SUPPORT_0_9);
    }
    mcs_map = siwifi_hw->mod_params->he_mcs_map;
    memset(&he_cap->he_mcs_nss_supp, 0, sizeof(he_cap->he_mcs_nss_supp));
    for (i = 0; i < nss; i++) {
        __le16 unsup_for_ss = cpu_to_le16(IEEE80211_HE_MCS_NOT_SUPPORTED << (i*2));
        he_cap->he_mcs_nss_supp.rx_mcs_80 |= cpu_to_le16(mcs_map << (i*2));
        he_cap->he_mcs_nss_supp.rx_mcs_160 |= unsup_for_ss;
        he_cap->he_mcs_nss_supp.rx_mcs_80p80 |= unsup_for_ss;
        mcs_map = IEEE80211_HE_MCS_SUPPORT_0_7;
    }
    for (; i < 8; i++) {
        __le16 unsup_for_ss = cpu_to_le16(IEEE80211_HE_MCS_NOT_SUPPORTED << (i*2));
        he_cap->he_mcs_nss_supp.rx_mcs_80 |= unsup_for_ss;
        he_cap->he_mcs_nss_supp.rx_mcs_160 |= unsup_for_ss;
        he_cap->he_mcs_nss_supp.rx_mcs_80p80 |= unsup_for_ss;
    }
    mcs_map = siwifi_hw->mod_params->he_mcs_map;
    band_5GHz->vht_cap.vht_mcs.tx_mcs_map = cpu_to_le16(0);
    for (i = 0; i < nss; i++) {
        __le16 unsup_for_ss = cpu_to_le16(IEEE80211_HE_MCS_NOT_SUPPORTED << (i*2));
        he_cap->he_mcs_nss_supp.tx_mcs_80 |= cpu_to_le16(mcs_map << (i*2));
        he_cap->he_mcs_nss_supp.tx_mcs_160 |= unsup_for_ss;
        he_cap->he_mcs_nss_supp.tx_mcs_80p80 |= unsup_for_ss;
        mcs_map = min_t(int, siwifi_hw->mod_params->mcs_map,
                        IEEE80211_HE_MCS_SUPPORT_0_7);
    }
    for (; i < 8; i++) {
        __le16 unsup_for_ss = cpu_to_le16(IEEE80211_HE_MCS_NOT_SUPPORTED << (i*2));
        he_cap->he_mcs_nss_supp.tx_mcs_80 |= unsup_for_ss;
        he_cap->he_mcs_nss_supp.tx_mcs_160 |= unsup_for_ss;
        he_cap->he_mcs_nss_supp.tx_mcs_80p80 |= unsup_for_ss;
    }
#endif
}

void siwifi_set_wiphy_params(struct siwifi_hw *siwifi_hw, struct wiphy *wiphy)
{
    /* FULLMAC specific parameters */
    wiphy->flags |= WIPHY_FLAG_REPORTS_OBSS;
    wiphy->max_scan_ssids = SCAN_SSID_MAX;
    wiphy->max_scan_ie_len = IEEE80211_MAX_DATA_LEN;

    if (siwifi_hw->mod_params->tdls) {
        /* TDLS support */
        wiphy->flags |= WIPHY_FLAG_SUPPORTS_TDLS;
        /* TDLS external setup support */
        wiphy->flags |= WIPHY_FLAG_TDLS_EXTERNAL_SETUP;
    }

    if (siwifi_hw->mod_params->ap_uapsd_on)
        wiphy->flags |= WIPHY_FLAG_AP_UAPSD;

}

void siwifi_set_rf_params(struct siwifi_hw *siwifi_hw, struct wiphy *wiphy)
{
#ifndef CONFIG_SIWIFI_SDM
    struct ieee80211_supported_band *band_5GHz = wiphy->bands[NL80211_BAND_5GHZ];
    struct ieee80211_supported_band *band_2GHz = wiphy->bands[NL80211_BAND_2GHZ];
    u32 mdm_phy_cfg = __MDM_PHYCFG_FROM_VERS(siwifi_hw->version_cfm.version_phy_1);

    /*
     * Get configuration file depending on the RF
     */
    if (mdm_phy_cfg == MDM_PHY_CONFIG_TRIDENT) {
        struct siwifi_phy_conf_file phy_conf;
        // Retrieve the Trident configuration
        siwifi_parse_phy_configfile(siwifi_hw, SIWIFI_PHY_CONFIG_TRD_NAME,
                                  &phy_conf, siwifi_hw->mod_params->phy_cfg);
        memcpy(&siwifi_hw->phy_config, &phy_conf.trd, sizeof(phy_conf.trd));
    } else if (mdm_phy_cfg == MDM_PHY_CONFIG_ELMA) {
    } else if (mdm_phy_cfg == MDM_PHY_CONFIG_AETNENSIS) {
#ifndef CONFIG_SIWIFI_TX_POWER_CALI
        struct siwifi_phy_conf_file phy_conf;
        // We use the NSS parameter as is
        // Retrieve the aetnensis configuration
        siwifi_parse_phy_configfile(siwifi_hw, SIWIFI_PHY_CONFIG_AETNENSIS_NAME,
                                  &phy_conf, siwifi_hw->mod_params->phy_cfg);

        memcpy(&siwifi_hw->phy_config, &phy_conf.aetnensis, sizeof(phy_conf.aetnensis));
#endif
    }
#ifndef CONFIG_SF16A18_FULL_MASK
	else if (mdm_phy_cfg == MDM_PHY_CONFIG_BEGONIA || mdm_phy_cfg == MDM_PHY_CONFIG_BEGONIA_FULLMASK) {
	}
#endif
	else {
        WARN_ON(1);
    }

    /*
     * adjust caps depending on the RF
     */
    switch (mdm_phy_cfg) {
        case MDM_PHY_CONFIG_TRIDENT:
        {
            SIWIFI_DBG("%s: found Trident phy .. using phy bw tweaks\n", __func__);
            siwifi_hw->use_phy_bw_tweaks = true;
#ifdef CONFIG_VENDOR_SIWIFI
            band_5GHz->vht_cap.cap |= IEEE80211_VHT_CAP_NOT_SUP_WIDTH_80;
#endif
            band_5GHz->vht_cap.cap &= ~(IEEE80211_VHT_CAP_SHORT_GI_80 |
                                        IEEE80211_VHT_CAP_RXSTBC_MASK);
            break;
        }
        case MDM_PHY_CONFIG_ELMA:
            SIWIFI_DBG("%s: found ELMA phy .. disabling 2.4GHz and greenfield rx\n", __func__);
            wiphy->bands[NL80211_BAND_2GHZ] = NULL;
            band_2GHz->ht_cap.cap &= ~IEEE80211_HT_CAP_GRN_FLD;
            band_5GHz->ht_cap.cap &= ~IEEE80211_HT_CAP_GRN_FLD;
            band_5GHz->vht_cap.cap &= ~IEEE80211_VHT_CAP_RXSTBC_MASK;
            break;
        case MDM_PHY_CONFIG_AETNENSIS:
#ifndef CONFIG_SF16A18_FULL_MASK
        case MDM_PHY_CONFIG_BEGONIA_FULLMASK:
		case MDM_PHY_CONFIG_BEGONIA:
#endif
		{
            if(!siwifi_hw->mod_params->is_hb)
                wiphy->bands[NL80211_BAND_5GHZ] = NULL;
            else
                wiphy->bands[NL80211_BAND_2GHZ] = NULL;
            break;
        }
        default:
            WARN_ON(1);
            break;
    }
    wiphy->bands[NL80211_BAND_60GHZ] = NULL;
    if (siwifi_hw->mod_params->custregd) {
        printk(KERN_CRIT
               "\n\n%s: CAUTION: USING PERMISSIVE CUSTOM REGULATORY RULES\n\n",
               __func__);
        wiphy->regulatory_flags |= REGULATORY_CUSTOM_REG;
        wiphy->regulatory_flags |= REGULATORY_IGNORE_STALE_KICKOFF;
        //before call wiphy_apply_custom_regulatory, we must make sure all wiphy->bands[] members has been set with properly value
        wiphy_apply_custom_regulatory(wiphy, &siwifi_regdom);
    }
#endif /* CONFIG_SIWIFI_SDM */
}

int siwifi_handle_dynparams(struct siwifi_hw *siwifi_hw, struct wiphy *wiphy)
{
    int ret;
    int max_amsdu_skb_size;

    /* Check compatibility between requested parameters and HW/SW features */
    ret = siwifi_check_fw_hw_feature(siwifi_hw, wiphy);
    if (ret)
        return ret;

#if defined(CFG_DEAGG)
    max_amsdu_skb_size = RX_AMSDU_MAX_SKB_SIZE;
#else
    max_amsdu_skb_size = (4 * (siwifi_hw->mod_params->amsdu_rx_max + 1) + 1) * 1024;
#endif

#ifdef CONFIG_PRIV_RX_BUFFER_POOL
    if (siwifi_init_private_rxskbs((siwifi_hw->ipc_env->rx_bufnb + 128), max_amsdu_skb_size))
    {
        wiphy_err(wiphy, "can not init private rx skbs\n");
        return ret;
    }
#endif

    ret = siwifi_ipc_rxbuf_init(siwifi_hw, max_amsdu_skb_size);
    if (ret) {
        wiphy_err(wiphy, "Cannot allocate the RX buffers\n");
        return ret;
    }

    /* Set wiphy parameters */
    siwifi_set_wiphy_params(siwifi_hw, wiphy);

    /* Set VHT capabilities */
    siwifi_set_vht_capa(siwifi_hw, wiphy);

    /* Set HE capabilities */
    siwifi_set_he_capa(siwifi_hw, wiphy);

    /* Set HT capabilities */
    siwifi_set_ht_capa(siwifi_hw, wiphy);

    /* Set RF specific parameters (shall be done last as it might change some
       capabilities previously set) */
    siwifi_set_rf_params(siwifi_hw, wiphy);

    return 0;
}
