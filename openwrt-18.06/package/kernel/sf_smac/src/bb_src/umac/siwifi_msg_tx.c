/**
 ******************************************************************************
 *
 * @file siwifi_msg_tx.c
 *
 * @brief TX function definitions
 *
 * Copyright (C) Siflower 2018-2025
 *
 ******************************************************************************
 */

#include <net/mac80211.h>
#include "siwifi_msg_tx.h"
#include "siwifi_mod_params.h"
#include "reg_access.h"
#ifdef CONFIG_SIWIFI_BFMER
#include "siwifi_bfmer.h"
#endif //(CONFIG_SIWIFI_BFMER)
#include "siwifi_compat.h"
#include "lmac_msg.h"

#if defined CONFIG_SF16A18_WIFI_ATE_TOOLS
#include "siwifi_defs.h"
#endif
#include "siwifi_mem.h"
#include "ipc_host.h"
static const struct mac_addr mac_addr_bcst = {{0xFFFF, 0xFFFF, 0xFFFF}};

/* Default MAC Rx filters that can be changed by mac80211
 * (via the configure_filter() callback) */
#define SIWIFI_MAC80211_CHANGEABLE        (                                       \
                                         NXMAC_ACCEPT_BA_BIT                  | \
                                         NXMAC_ACCEPT_BAR_BIT                 | \
                                         NXMAC_ACCEPT_OTHER_DATA_FRAMES_BIT   | \
                                         NXMAC_ACCEPT_PROBE_REQ_BIT           | \
                                         NXMAC_ACCEPT_PS_POLL_BIT               \
                                        )

/* Default MAC Rx filters that cannot be changed by mac80211 */
#define SIWIFI_MAC80211_NOT_CHANGEABLE    (                                       \
                                         NXMAC_ACCEPT_QO_S_NULL_BIT           | \
                                         NXMAC_ACCEPT_Q_DATA_BIT              | \
                                         NXMAC_ACCEPT_DATA_BIT                | \
                                         NXMAC_ACCEPT_OTHER_MGMT_FRAMES_BIT   | \
                                         NXMAC_ACCEPT_MY_UNICAST_BIT          | \
                                         NXMAC_ACCEPT_BROADCAST_BIT           | \
                                         NXMAC_ACCEPT_BEACON_BIT              | \
                                         NXMAC_ACCEPT_PROBE_RESP_BIT            \
                                        )

/* Default MAC Rx filter */
#define SIWIFI_DEFAULT_RX_FILTER  (SIWIFI_MAC80211_CHANGEABLE | SIWIFI_MAC80211_NOT_CHANGEABLE)

static const int bw2chnl[] = {
    [NL80211_CHAN_WIDTH_20_NOHT] = PHY_CHNL_BW_20,
    [NL80211_CHAN_WIDTH_20]      = PHY_CHNL_BW_20,
    [NL80211_CHAN_WIDTH_40]      = PHY_CHNL_BW_40,
    [NL80211_CHAN_WIDTH_80]      = PHY_CHNL_BW_80,
    [NL80211_CHAN_WIDTH_160]     = PHY_CHNL_BW_160,
    [NL80211_CHAN_WIDTH_80P80]   = PHY_CHNL_BW_80P80,
};

const int chnl2bw[] = {
    [PHY_CHNL_BW_20]      = NL80211_CHAN_WIDTH_20,
    [PHY_CHNL_BW_40]      = NL80211_CHAN_WIDTH_40,
    [PHY_CHNL_BW_80]      = NL80211_CHAN_WIDTH_80,
    [PHY_CHNL_BW_160]     = NL80211_CHAN_WIDTH_160,
    [PHY_CHNL_BW_80P80]   = NL80211_CHAN_WIDTH_80P80,
};

/*****************************************************************************/
/*
 * Parse the ampdu density to retrieve the value in usec, according to the
 * values defined in ieee80211.h
 */
static inline u8 siwifi_ampdudensity2usec(u8 ampdudensity)
{
    switch (ampdudensity) {
    case IEEE80211_HT_MPDU_DENSITY_NONE:
        return 0;
        /* 1 microsecond is our granularity */
    case IEEE80211_HT_MPDU_DENSITY_0_25:
    case IEEE80211_HT_MPDU_DENSITY_0_5:
    case IEEE80211_HT_MPDU_DENSITY_1:
        return 1;
    case IEEE80211_HT_MPDU_DENSITY_2:
        return 2;
    case IEEE80211_HT_MPDU_DENSITY_4:
        return 4;
    case IEEE80211_HT_MPDU_DENSITY_8:
        return 8;
    case IEEE80211_HT_MPDU_DENSITY_16:
        return 16;
    default:
        return 0;
    }
}

static inline bool use_pairwise_key(struct cfg80211_crypto_settings *crypto)
{
    if ((crypto->cipher_group ==  WLAN_CIPHER_SUITE_WEP40) ||
        (crypto->cipher_group ==  WLAN_CIPHER_SUITE_WEP104))
        return false;

    return true;
}

static inline bool is_non_blocking_msg(int id) {
    return ((id == MM_TIM_UPDATE_REQ) || (id == ME_RC_SET_RATE_REQ) ||
            (id == MM_BFMER_ENABLE_REQ) || (id == ME_TRAFFIC_IND_REQ) ||
            (id == TDLS_PEER_TRAFFIC_IND_REQ) ||
            (id == MESH_PATH_CREATE_REQ) || (id == MESH_PROXY_ADD_REQ) ||
            (id == SM_EXTERNAL_AUTH_REQUIRED_RSP));
}

static inline uint8_t passive_scan_flag(enum nl80211_iftype type, uint32_t flags) {
    if (type == NL80211_IFTYPE_STATION) {
        if (flags & IEEE80211_CHAN_NO_IR)
            return SCAN_PASSIVE_BIT;
    } else {
        if (flags & (IEEE80211_CHAN_NO_IR | IEEE80211_CHAN_RADAR))
            return SCAN_PASSIVE_BIT;
    }
    return 0;
}

/**
 ******************************************************************************
 * @brief Allocate memory for a message
 *
 * This primitive allocates memory for a message that has to be sent. The memory
 * is allocated dynamically on the heap and the length of the variable parameter
 * structure has to be provided in order to allocate the correct size.
 *
 * Several additional parameters are provided which will be preset in the message
 * and which may be used internally to choose the kind of memory to allocate.
 *
 * The memory allocated will be automatically freed by the kernel, after the
 * pointer has been sent to ke_msg_send(). If the message is not sent, it must
 * be freed explicitly with ke_msg_free().
 *
 * Allocation failure is considered critical and should not happen.
 *
 * @param[in] id        Message identifier
 * @param[in] dest_id   Destination Task Identifier
 * @param[in] src_id    Source Task Identifier
 * @param[in] param_len Size of the message parameters to be allocated
 *
 * @return Pointer to the parameter member of the ke_msg. If the parameter
 *         structure is empty, the pointer will point to the end of the message
 *         and should not be used (except to retrieve the message pointer or to
 *         send the message)
 ******************************************************************************
 */
static inline void *siwifi_msg_zalloc(lmac_msg_id_t const id,
                                    lmac_task_id_t const dest_id,
                                    lmac_task_id_t const src_id,
                                    uint16_t const param_len)
{
    struct lmac_msg *msg;
    gfp_t flags;

    if (in_softirq())
        flags = GFP_ATOMIC;
    else
        flags = GFP_KERNEL;

    msg = (struct lmac_msg *)siwifi_kzalloc(sizeof(struct lmac_msg) + param_len,
                                     flags);
    if (msg == NULL) {
        printk(KERN_CRIT "%s: msg allocation failed\n", __func__);
        return NULL;
    }

    msg->id = id;
    msg->dest_id = dest_id;
    msg->src_id = src_id;
    msg->param_len = param_len;

    return msg->param;
}

static void siwifi_msg_free(struct siwifi_hw *siwifi_hw, const void *msg_params)
{
    struct lmac_msg *msg = container_of((void *)msg_params,
                                        struct lmac_msg, param);

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Free the message */
    siwifi_kfree(msg);
}

#ifdef CONFIG_SIWIFI_RF_RECALI
static int siwifi_send_msg_with_flag(struct siwifi_hw *siwifi_hw, const void *msg_params,
                         int reqcfm, lmac_msg_id_t reqid, void *cfm, uint16_t flags)
{
    struct lmac_msg *msg;
    struct siwifi_cmd *cmd;
    bool nonblock;
    int ret;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    msg = container_of((void *)msg_params, struct lmac_msg, param);

    if (!test_bit(SIWIFI_DEV_STARTED, &siwifi_hw->drv_flags) &&
        reqid != MM_RESET_CFM && reqid != MM_VERSION_CFM &&
        reqid != MM_START_CFM && reqid != MM_SET_IDLE_CFM &&
        reqid != ME_CONFIG_CFM && reqid != MM_SET_PS_MODE_CFM &&
        reqid != ME_CHAN_CONFIG_CFM && reqid != MM_RF_CTRL_CFM) {
        printk(KERN_CRIT "%s: bypassing (SIWIFI_DEV_RESTARTING set) 0x%02x\n",
               __func__, reqid);
        siwifi_kfree(msg);
        return -EBUSY;
    } else if (!siwifi_hw->ipc_env) {
        printk(KERN_CRIT "%s: bypassing (restart must have failed)\n", __func__);
        siwifi_kfree(msg);
        return -EBUSY;
    }

    nonblock = is_non_blocking_msg(msg->id);

    cmd = siwifi_kzalloc(sizeof(struct siwifi_cmd), nonblock ? GFP_ATOMIC : GFP_KERNEL);
    cmd->result  = -EINTR;
    cmd->id      = msg->id;
    cmd->reqid   = reqid;
    cmd->a2e_msg = msg;
    cmd->e2a_msg = cfm;
    cmd->flags = flags;
    if (nonblock)
        cmd->flags |= SIWIFI_CMD_FLAG_NONBLOCK;
    if (reqcfm)
        cmd->flags |= SIWIFI_CMD_FLAG_REQ_CFM;
    ret = siwifi_hw->cmd_mgr.queue(&siwifi_hw->cmd_mgr, cmd);

    if (!nonblock) {
        ret = cmd->result;
        siwifi_kfree(cmd);
    }

    return ret;
}
#endif

static int siwifi_send_msg(struct siwifi_hw *siwifi_hw, const void *msg_params,
                         int reqcfm, lmac_msg_id_t reqid, void *cfm)
{
    struct lmac_msg *msg;
    struct siwifi_cmd *cmd;
    bool nonblock;
    int ret;

    msg = container_of((void *)msg_params, struct lmac_msg, param);

    if (test_bit(SIWIFI_DEV_HW_DEAD, &siwifi_hw->drv_flags)) {
        printk(KERN_CRIT "%s: bypassing (SIWIFI_DEV_HW_DEAD set)\n", __func__);
        siwifi_kfree(msg);
        return -EBUSY;
    } else if (!test_bit(SIWIFI_DEV_STARTED, &siwifi_hw->drv_flags) &&
                    reqid != MM_RESET_CFM && reqid != MM_VERSION_CFM &&
                    reqid != MM_START_CFM && reqid != MM_SET_IDLE_CFM &&
                    reqid != ME_CONFIG_CFM && reqid != MM_SET_PS_MODE_CFM &&
                    reqid != ME_CHAN_CONFIG_CFM && reqid != MM_HEART_CFM) {
        printk(KERN_CRIT "%s: bypassing (SIWIFI_DEV_RESTARTING set) 0x%02x\n",
               __func__, reqid);
        siwifi_kfree(msg);
        return -EBUSY;
    } else if (!siwifi_hw->ipc_env) {
        printk(KERN_CRIT "%s: bypassing (restart must have failed)\n", __func__);
        siwifi_kfree(msg);
        return -EBUSY;
    }

    nonblock = is_non_blocking_msg(msg->id);

    cmd = siwifi_kzalloc(sizeof(struct siwifi_cmd), nonblock ? GFP_ATOMIC : GFP_KERNEL);
    cmd->result  = -EINTR;
    cmd->id      = msg->id;
    cmd->reqid   = reqid;
    cmd->a2e_msg = msg;
    cmd->e2a_msg = cfm;
    if (nonblock)
        cmd->flags = SIWIFI_CMD_FLAG_NONBLOCK;
    if (reqcfm)
        cmd->flags |= SIWIFI_CMD_FLAG_REQ_CFM;
    ret = siwifi_hw->cmd_mgr.queue(&siwifi_hw->cmd_mgr, cmd);

    // if nonblock msg, actually , if ret is zero, then cmd result is always zero
    if (!ret && !nonblock) {
        ret = cmd->result;
    }
    if (ret && cmd->a2e_msg) {
        siwifi_kfree(cmd->a2e_msg);
        cmd->a2e_msg = NULL;
    }

    if (!nonblock && !(cmd->flags & SIWIFI_CMD_FLAG_NONBLOCK)) {
        siwifi_kfree(cmd);
    } else if (ret && nonblock) {
        siwifi_kfree(cmd);
    }

    return ret;
}

/******************************************************************************
 *    Control messages handling functions (SOFTMAC and  FULLMAC)
 *****************************************************************************/

int siwifi_send_read_cde_info(struct siwifi_hw *siwifi_hw)
{
    void *void_param;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* RESET REQ has no parameter */
    void_param = siwifi_msg_zalloc(MM_GET_CHAN_INFO, TASK_MM, DRV_TASK_ID, 0);
    if (!void_param)
        return -ENOMEM;

    return siwifi_send_msg(siwifi_hw, void_param, 0, -1, NULL);
}

int siwifi_send_reset(struct siwifi_hw *siwifi_hw)
{
    void *void_param;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* RESET REQ has no parameter */
    void_param = siwifi_msg_zalloc(MM_RESET_REQ, TASK_MM, DRV_TASK_ID, 0);
    if (!void_param)
        return -ENOMEM;

    return siwifi_send_msg(siwifi_hw, void_param, 1, MM_RESET_CFM, NULL);
}

int siwifi_send_vif_dump(struct siwifi_hw *siwifi_hw)
{
    void *void_param;
    void_param = siwifi_msg_zalloc(MM_VIF_DUMP_REQ, TASK_MM, DRV_TASK_ID, 0);
    if (!void_param)
        return -ENOMEM;
    return siwifi_send_msg(siwifi_hw, void_param, 1, MM_VIF_DUMP_CFM, NULL);

}
#ifdef CONFIG_HEART_BEAT
int siwifi_send_heart(struct siwifi_hw *siwifi_hw)
{
    void *void_param;
    //SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    void_param = siwifi_msg_zalloc(MM_HEART_REQ, TASK_MM, DRV_TASK_ID, 0);
    if (!void_param)
        return -ENOMEM;

    return siwifi_send_msg(siwifi_hw, void_param, 1, MM_HEART_CFM, NULL);
}
#endif

int siwifi_send_start(struct siwifi_hw *siwifi_hw)
{
    struct mm_start_req *start_req_param;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the START REQ message */
    start_req_param = siwifi_msg_zalloc(MM_START_REQ, TASK_MM, DRV_TASK_ID,
                                      sizeof(struct mm_start_req));
    if (!start_req_param)
        return -ENOMEM;

    /* Set parameters for the START message */
    memcpy(&start_req_param->phy_cfg, &siwifi_hw->phy_config, sizeof(siwifi_hw->phy_config));
    start_req_param->uapsd_timeout = (u32_l)siwifi_hw->mod_params->uapsd_timeout;
    start_req_param->lp_clk_accuracy = (u16_l)siwifi_hw->mod_params->lp_clk_ppm;
    if(siwifi_hw->mod_params->addr_maskall){
        start_req_param->global_flags |= MM_START_GLOBAL_FLAG_ADDR_MASK_ALL;
    }
	if(siwifi_hw->mod_params->not_send_null){
		start_req_param->global_flags |= MM_START_GLOBAL_FLAG_NOT_SEND_NULL;
	}
	//change rts and cts
	start_req_param->parameters[RTS_CTS_CHANGE] = siwifi_hw->mod_params->rts_cts_change;
    //add ampdu max cnt
    start_req_param->parameters[MM_AMPDU_MAX_CNT] = siwifi_hw->mod_params->ampdu_max_cnt;
#ifdef INDEPENDENT_ANTENNA_CONTROL
	start_req_param->parameters[MM_INDEPENDENT_ANTENNA_CONTROL] = siwifi_hw->mod_params->independent_antenna_control;
#endif
    /* Send the START REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, start_req_param, 1, MM_START_CFM, NULL);
}

int siwifi_send_version_req(struct siwifi_hw *siwifi_hw, struct mm_version_cfm *cfm)
{
    void *void_param;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* VERSION REQ has no parameter */
    void_param = siwifi_msg_zalloc(MM_VERSION_REQ, TASK_MM, DRV_TASK_ID, 0);
    if (!void_param)
        return -ENOMEM;

    return siwifi_send_msg(siwifi_hw, void_param, 1, MM_VERSION_CFM, cfm);
}

int siwifi_send_add_if(struct siwifi_hw *siwifi_hw, const unsigned char *mac,
                     enum nl80211_iftype iftype, bool p2p, bool use_4addr, struct mm_add_if_cfm *cfm)
{
    struct mm_add_if_req *add_if_req_param;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the ADD_IF_REQ message */
    add_if_req_param = siwifi_msg_zalloc(MM_ADD_IF_REQ, TASK_MM, DRV_TASK_ID,
                                       sizeof(struct mm_add_if_req));
    if (!add_if_req_param)
        return -ENOMEM;

    /* Set parameters for the ADD_IF_REQ message */
    memcpy(&(add_if_req_param->addr.array[0]), mac, ETH_ALEN);
    switch (iftype) {
    case NL80211_IFTYPE_P2P_CLIENT:
        add_if_req_param->p2p = true;
        // no break
    case NL80211_IFTYPE_STATION:
        add_if_req_param->type = MM_STA;
        break;

    case NL80211_IFTYPE_ADHOC:
        add_if_req_param->type = MM_IBSS;
        break;

    case NL80211_IFTYPE_P2P_GO:
        add_if_req_param->p2p = true;
        // no break
    case NL80211_IFTYPE_AP:
        add_if_req_param->type = MM_AP;
        break;
    case NL80211_IFTYPE_MESH_POINT:
        add_if_req_param->type = MM_MESH_POINT;
        break;
    case NL80211_IFTYPE_AP_VLAN:
        return -1;
    case NL80211_IFTYPE_MONITOR:
        add_if_req_param->type = MM_MONITOR;
        add_if_req_param->uf = siwifi_hw->mod_params->uf;
        break;
    default:
        add_if_req_param->type = MM_STA;
        break;
    }

    add_if_req_param->use_4addr = use_4addr;

    if (cfm->status != 0) {
        add_if_req_param->req_index_valid = true;
        add_if_req_param->req_index = cfm->inst_nbr;
    }

    /* Send the ADD_IF_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, add_if_req_param, 1, MM_ADD_IF_CFM, cfm);
}

int siwifi_send_remove_if(struct siwifi_hw *siwifi_hw, u8 vif_index)
{
    struct mm_remove_if_req *remove_if_req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MM_REMOVE_IF_REQ message */
    remove_if_req = siwifi_msg_zalloc(MM_REMOVE_IF_REQ, TASK_MM, DRV_TASK_ID,
                                    sizeof(struct mm_remove_if_req));
    if (!remove_if_req)
        return -ENOMEM;

    /* Set parameters for the MM_REMOVE_IF_REQ message */
    remove_if_req->inst_nbr = vif_index;

    /* Send the MM_REMOVE_IF_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, remove_if_req, 1, MM_REMOVE_IF_CFM, NULL);
}

/*
 * func:
 * @set channel offset ,such as netbridge need ch add 5 or 10
 *
 * */
#if defined CONFIG_SIWIFI_CH_OFFSET
int siwifi_send_set_channel_offset(struct siwifi_hw *siwifi_hw, int phy_idx,
			struct mm_set_channel_cfm *cfm, int ch_offset)
{
    struct mm_set_channel_req *set_chnl_par;
    struct siwifi_vif *siwifi_vif;
    printk("%s \n",__func__);
    if (phy_idx >= siwifi_hw->phy_cnt)
        return -ENOTSUPP;
    set_chnl_par = siwifi_msg_zalloc(MM_SET_CHANNEL_REQ, TASK_MM, DRV_TASK_ID,
                                   sizeof(struct mm_set_channel_req));
    if (!set_chnl_par)
        return -ENOMEM;

    spin_lock_bh(&siwifi_hw->cb_lock);
    list_for_each_entry(siwifi_vif, &siwifi_hw->vifs, list) {
        if(SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_AP) {
            struct cfg80211_chan_def *chandef = &siwifi_vif->siwifi_hw->chanctx_table[siwifi_vif->ch_index].chan_def;
            set_chnl_par->band = chandef->chan->band;
#if DEBUG_ARRAY_CHECK
            BUG_ON(chandef->width > NL80211_CHAN_WIDTH_80P80);
#endif
            set_chnl_par->type = bw2chnl[chandef->width];
            set_chnl_par->prim20_freq = chandef->chan->center_freq;
            set_chnl_par->center1_freq = chandef->center_freq1;
            set_chnl_par->center2_freq = chandef->center_freq2;
            set_chnl_par->tx_power = chandef->chan->max_power;
            set_chnl_par->index = 0;
            set_chnl_par->setchannel=1;
            set_chnl_par->channel_off = ch_offset;
            set_chnl_par->mode = 2;
        } else if(SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_STATION) {
            set_chnl_par->channel_off = ch_offset;
            set_chnl_par->mode = 0;
        }
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);
    /* Send the MM_SET_CHANNEL_REQ REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, set_chnl_par, 1, MM_SET_CHANNEL_CFM, cfm);
}
#endif

/*
 * func:
 * @only set channel,no need calculate center frequency
 *
 * */
int siwifi_send_set_channel_for_macbypass_tx(struct siwifi_hw *siwifi_hw, int phy_idx,struct mm_set_channel_cfm *cfm,struct cfg80211_chan_def *chandef){
	u16 center_freq, center_freq1, center_freq2;
    s8 tx_power = 0;
    struct mm_set_channel_req *set_chnl_par;
    enum nl80211_band band;
	printk("%s",__func__);

    if (phy_idx >= siwifi_hw->phy_cnt)
        return -ENOTSUPP;

    set_chnl_par = siwifi_msg_zalloc(MM_SET_CHANNEL_REQ, TASK_MM, DRV_TASK_ID,
                                   sizeof(struct mm_set_channel_req));
	if (!set_chnl_par)
	  return -ENOMEM;

	band  = chandef->chan->band;
	center_freq  = chandef->chan->center_freq;
	center_freq1 = chandef->center_freq1;
	center_freq2 = chandef->center_freq2;
	tx_power = chandef->chan->max_power;

	set_chnl_par->band = band;
#if DEBUG_ARRAY_CHECK
	BUG_ON(chandef->width > NL80211_CHAN_WIDTH_80P80);
#endif
	set_chnl_par->type = bw2chnl[chandef->width];
	set_chnl_par->prim20_freq  = center_freq;
	set_chnl_par->center1_freq = center_freq1;
	set_chnl_par->center2_freq = center_freq2;
	set_chnl_par->tx_power = tx_power;
	set_chnl_par->index = 0;
	set_chnl_par->setchannel=1;

	printk("mac80211:   freq=%d(c1:%d - c2:%d)/width=%d - band=%d\n"
				"   hw(%d): prim20=%d(c1:%d - c2:%d)/ type=%d - band=%d\n",
				center_freq, center_freq1,
				center_freq2, chandef->width, band,
				set_chnl_par->index, set_chnl_par->prim20_freq, set_chnl_par->center1_freq,
				set_chnl_par->center2_freq, set_chnl_par->type, set_chnl_par->band);

	/* Send the MM_SET_CHANNEL_REQ REQ message to LMAC FW */
	return siwifi_send_msg(siwifi_hw, set_chnl_par, 1, MM_SET_CHANNEL_CFM, cfm);
}


int siwifi_send_set_channel(struct siwifi_hw *siwifi_hw, int phy_idx,
			struct mm_set_channel_cfm *cfm)
{
    struct mm_set_channel_req *set_chnl_par;
    enum nl80211_chan_width width;
    u16 center_freq, center_freq1, center_freq2;
    s8 tx_power = 0;
    enum nl80211_band band;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    if (phy_idx >= siwifi_hw->phy_cnt)
        return -ENOTSUPP;

    set_chnl_par = siwifi_msg_zalloc(MM_SET_CHANNEL_REQ, TASK_MM, DRV_TASK_ID,
                                   sizeof(struct mm_set_channel_req));
    if (!set_chnl_par)
        return -ENOMEM;

    if (phy_idx == 0) {
        /* On FULLMAC only setting channel of secondary chain */
        wiphy_err(siwifi_hw->wiphy, "Trying to set channel of primary chain");
        return 0;
    } else {
        struct siwifi_sec_phy_chan *chan = &siwifi_hw->sec_phy_chan;

#if DEBUG_ARRAY_CHECK
        BUG_ON(chan->type > PHY_CHNL_BW_80P80);
#endif
        width = chnl2bw[chan->type];
        band  = chan->band;
        center_freq  = chan->prim20_freq;
        center_freq1 = chan->center_freq1;
        center_freq2 = chan->center_freq2;
    }

    set_chnl_par->band = band;
    set_chnl_par->type = bw2chnl[width];
    set_chnl_par->prim20_freq  = center_freq;
    set_chnl_par->center1_freq = center_freq1;
    set_chnl_par->center2_freq = center_freq2;
    set_chnl_par->index = phy_idx;
    set_chnl_par->tx_power = tx_power;

    if (siwifi_hw->use_phy_bw_tweaks) {
        /* XXX Tweak for 80MHz VHT */
        if (width > NL80211_CHAN_WIDTH_40) {
            int _offs = center_freq1 - center_freq;
            set_chnl_par->type = PHY_CHNL_BW_40;
            set_chnl_par->center1_freq = center_freq + 10 *
                (_offs > 0 ? 1 : -1) * (abs(_offs) > 10 ? 1 : -1);
            SIWIFI_DBG("Tweak for 80MHz VHT: 80MHz chan requested\n");
        }
    }

    SIWIFI_DBG("mac80211:   freq=%d(c1:%d - c2:%d)/width=%d - band=%d\n"
             "   hw(%d): prim20=%d(c1:%d - c2:%d)/ type=%d - band=%d\n",
             center_freq, center_freq1,
             center_freq2, width, band,
             phy_idx, set_chnl_par->prim20_freq, set_chnl_par->center1_freq,
             set_chnl_par->center2_freq, set_chnl_par->type, set_chnl_par->band);

    /* Send the MM_SET_CHANNEL_REQ REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, set_chnl_par, 1, MM_SET_CHANNEL_CFM, cfm);
}


int siwifi_send_key_add(struct siwifi_hw *siwifi_hw, u8 vif_idx, u8 sta_idx, bool pairwise,
                      u8 *key, u8 key_len, u8 key_idx, u8 cipher_suite,
                      struct mm_key_add_cfm *cfm)
{
    struct mm_key_add_req *key_add_req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MM_KEY_ADD_REQ message */
    key_add_req = siwifi_msg_zalloc(MM_KEY_ADD_REQ, TASK_MM, DRV_TASK_ID,
                                  sizeof(struct mm_key_add_req));
    if (!key_add_req)
        return -ENOMEM;

    /* Set parameters for the MM_KEY_ADD_REQ message */
    key_add_req->sta_idx = sta_idx;
    if (sta_idx == 0xFF) {
        key_add_req->key_idx = (u8_l)key_idx; /* only useful for default keys */
    }
    key_add_req->pairwise = pairwise;
    key_add_req->inst_nbr = vif_idx;
    key_add_req->key.length = key_len;
    memcpy(&(key_add_req->key.array[0]), key, key_len);

    key_add_req->cipher_suite = cipher_suite;
    key_add_req->recovering = false;
    if (siwifi_hw->dying_gasp_valid &&
            test_bit(SIWIFI_DEV_STACK_RESTARTING, &siwifi_hw->drv_flags)) {
        int i;
        struct mm_dying_gasp *dying_gasp_info = (struct mm_dying_gasp *)siwifi_hw->dying_gasp_info_elem.addr;
        if (sta_idx != 0xFF) {
            //search correct sta key information
            for (i = 0; i < dying_gasp_info->sta_len; i++) {
                if (dying_gasp_info->stas[i].index != sta_idx)
                    continue;
                // Check dying gasp information is correct
                BUG_ON(dying_gasp_info->stas[i].vif_index != vif_idx);
                // pass the tx_pn value to lmac
                key_add_req->tx_pn = dying_gasp_info->stas[i].key.tx_pn;
                key_add_req->recovering = true;
            }
        } else {
            int j = 0;
            for (i = 0; i < dying_gasp_info->vif_len; i++)
            {
                if (dying_gasp_info->vifs[i].index != vif_idx)
                    continue;
                for (j = 0; j < dying_gasp_info->vifs[i].key_len; j ++)
                {
                    if (key_idx != dying_gasp_info->vifs[i].keys[j].key_idx)
                        continue;
                    key_add_req->tx_pn = dying_gasp_info->vifs[i].keys[j].tx_pn;
                    key_add_req->recovering = true;
                }
            }
        }
    }
    if (key_add_req->recovering) {
        printk("key_add_req->tx_pn : %lld\n", key_add_req->tx_pn);
    }

    SIWIFI_DBG("%s: sta_idx:%d key_idx:%d inst_nbr:%d cipher:%d key_len:%d\n", __func__,
             key_add_req->sta_idx, key_add_req->key_idx, key_add_req->inst_nbr,
             key_add_req->cipher_suite, key_add_req->key.length);
#if defined(CONFIG_SIWIFI_DBG) || defined(CONFIG_DYNAMIC_DEBUG)
    print_hex_dump_bytes("key: ", DUMP_PREFIX_OFFSET, key_add_req->key.array, key_add_req->key.length);
#endif

    /* Send the MM_KEY_ADD_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, key_add_req, 1, MM_KEY_ADD_CFM, cfm);
}

int siwifi_send_key_del(struct siwifi_hw *siwifi_hw, uint8_t hw_key_idx)
{
    struct mm_key_del_req *key_del_req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MM_KEY_DEL_REQ message */
    key_del_req = siwifi_msg_zalloc(MM_KEY_DEL_REQ, TASK_MM, DRV_TASK_ID,
                                  sizeof(struct mm_key_del_req));
    if (!key_del_req)
        return -ENOMEM;

    /* Set parameters for the MM_KEY_DEL_REQ message */
    key_del_req->hw_key_idx = hw_key_idx;

    /* Send the MM_KEY_DEL_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, key_del_req, 1, MM_KEY_DEL_CFM, NULL);
}

int siwifi_send_default_key_add(struct siwifi_hw *siwifi_hw, u8 vif_idx, u8 default_key_idx, struct mm_default_key_add_cfm *cfm)
{
    struct mm_default_key_add_req *default_key_add_req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MM_DEFAULT_KEY_ADD_REQ message */
    default_key_add_req = siwifi_msg_zalloc(MM_DEFAULT_KEY_ADD_REQ, TASK_MM, DRV_TASK_ID,
                                  sizeof(struct mm_default_key_add_req));
    if (!default_key_add_req)
        return -ENOMEM;

    /* Set parameters for the MM_KEY_DEL_REQ message */
    default_key_add_req->default_key_idx = default_key_idx;
    default_key_add_req->inst_nbr = vif_idx;

    /* Send the MM_KEY_DEL_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, default_key_add_req, 1, MM_DEFAULT_KEY_ADD_CFM, cfm);
}

int siwifi_send_bcn_change(struct siwifi_hw *siwifi_hw, u8 vif_idx, u32 bcn_addr,
                         u16 bcn_len, u16 tim_oft, u16 tim_len, u16 *csa_oft)
{
    struct mm_bcn_change_req *req;

    //SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    /* Build the MM_BCN_CHANGE_REQ message */
    req = siwifi_msg_zalloc(MM_BCN_CHANGE_REQ, TASK_MM, DRV_TASK_ID,
                          sizeof(struct mm_bcn_change_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the MM_BCN_CHANGE_REQ message */
    req->bcn_ptr = bcn_addr;
    req->bcn_len = bcn_len;
    req->tim_oft = tim_oft;
    req->tim_len = tim_len;
    req->inst_nbr = vif_idx;

    if (csa_oft) {
        int i;
        for (i = 0; i < BCN_MAX_CSA_CPT; i++) {
            req->csa_oft[i] = csa_oft[i];
        }
    }

    /* Send the MM_BCN_CHANGE_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, MM_BCN_CHANGE_CFM, NULL);
}

int siwifi_send_roc(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif,
                  struct ieee80211_channel *chan, unsigned  int duration)
{
    struct mm_remain_on_channel_req *req;
    struct cfg80211_chan_def chandef;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Create channel definition structure */
    cfg80211_chandef_create(&chandef, chan, NL80211_CHAN_NO_HT);

    /* Build the MM_REMAIN_ON_CHANNEL_REQ message */
    req = siwifi_msg_zalloc(MM_REMAIN_ON_CHANNEL_REQ, TASK_MM, DRV_TASK_ID,
                          sizeof(struct mm_remain_on_channel_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the MM_REMAIN_ON_CHANNEL_REQ message */
    req->op_code      = MM_ROC_OP_START;
    req->vif_index    = vif->vif_index;
    req->duration_ms  = duration;
    req->band         = chan->band;
    req->type         = bw2chnl[chandef.width];
    req->prim20_freq  = chan->center_freq;
    req->center1_freq = chandef.center_freq1;
    req->center2_freq = chandef.center_freq2;
    req->tx_power     = chan->max_power;

    /* Send the MM_REMAIN_ON_CHANNEL_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, MM_REMAIN_ON_CHANNEL_CFM, NULL);
}

int siwifi_send_cancel_roc(struct siwifi_hw *siwifi_hw)
{
    struct mm_remain_on_channel_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MM_REMAIN_ON_CHANNEL_REQ message */
    req = siwifi_msg_zalloc(MM_REMAIN_ON_CHANNEL_REQ, TASK_MM, DRV_TASK_ID,
                          sizeof(struct mm_remain_on_channel_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the MM_REMAIN_ON_CHANNEL_REQ message */
    req->op_code = MM_ROC_OP_CANCEL;

    /* Send the MM_REMAIN_ON_CHANNEL_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
}

int siwifi_send_set_power(struct siwifi_hw *siwifi_hw, u8 vif_idx, s8 pwr,
                        struct mm_set_power_cfm *cfm)
{
    struct mm_set_power_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MM_SET_POWER_REQ message */
    req = siwifi_msg_zalloc(MM_SET_POWER_REQ, TASK_MM, DRV_TASK_ID,
                          sizeof(struct mm_set_power_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the MM_SET_POWER_REQ message */
    req->inst_nbr = vif_idx;
    req->power = pwr;

    /* Send the MM_SET_POWER_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, MM_SET_POWER_CFM, cfm);
}

int siwifi_send_set_power_lvl(struct siwifi_hw *siwifi_hw, u8 power_lvl)
{
    struct mm_set_power_lvl_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MM_SET_POWER_REQ message */
    req = siwifi_msg_zalloc(MM_SET_POWER_LVL_REQ, TASK_MM, DRV_TASK_ID,
                          sizeof(struct mm_set_power_lvl_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the MM_SET_POWER_REQ message */
    req->power_lvl = power_lvl;

    /* Send the MM_SET_POWER_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
}

int siwifi_send_set_fixed_gain(struct siwifi_hw *siwifi_hw, int fixed_gain, int temp_ctrl_enable)
{
    struct mm_set_gain_control_req *req;
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    /* Build the MM_SET_POWER_REQ message */
    req = siwifi_msg_zalloc(MM_GAIN_CONTROL_REQ, TASK_MM, DRV_TASK_ID,
                          sizeof(struct mm_set_gain_control_req));
    if (!req)
        return -ENOMEM;
    /* Set parameters for the MM_SET_POWER_REQ message */
    req->action = MM_GAIN_CONTROL_ACTION_FIX_GAIN;
    req->param1 = fixed_gain;
    req->param2 = temp_ctrl_enable;
    return siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
}

int siwifi_send_set_antenna_number(struct siwifi_hw *siwifi_hw, u8 antenna_number)
{
    struct mm_set_antenna_number_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MM_SET_POWER_REQ message */
    req = siwifi_msg_zalloc(MM_SET_ANTENNA_NUMBER_REQ, TASK_MM, DRV_TASK_ID,
                          sizeof(struct mm_set_antenna_number_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the MM_SET_POWER_REQ message */
    req->antenna_number = antenna_number;

    /* Send the MM_SET_POWER_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
}

int siwifi_send_set_edca(struct siwifi_hw *siwifi_hw, u8 hw_queue, u32 param,
                       bool uapsd, u8 inst_nbr)
{
    struct mm_set_edca_req *set_edca_req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MM_SET_EDCA_REQ message */
    set_edca_req = siwifi_msg_zalloc(MM_SET_EDCA_REQ, TASK_MM, DRV_TASK_ID,
                                   sizeof(struct mm_set_edca_req));
    if (!set_edca_req)
        return -ENOMEM;

    /* Set parameters for the MM_SET_EDCA_REQ message */
    set_edca_req->ac_param = param;
    set_edca_req->uapsd = uapsd;
    set_edca_req->hw_queue = hw_queue;
    set_edca_req->inst_nbr = inst_nbr;

    /* Send the MM_SET_EDCA_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, set_edca_req, 1, MM_SET_EDCA_CFM, NULL);
}

#ifdef CONFIG_SIWIFI_P2P_DEBUGFS
int siwifi_send_p2p_oppps_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif,
                            u8 ctw, struct mm_set_p2p_oppps_cfm *cfm)
{
    struct mm_set_p2p_oppps_req *p2p_oppps_req;
    int error;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MM_SET_P2P_OPPPS_REQ message */
    p2p_oppps_req = siwifi_msg_zalloc(MM_SET_P2P_OPPPS_REQ, TASK_MM, DRV_TASK_ID,
                                    sizeof(struct mm_set_p2p_oppps_req));

    if (!p2p_oppps_req) {
        return -ENOMEM;
    }

    /* Fill the message parameters */
    p2p_oppps_req->vif_index = siwifi_vif->vif_index;
    p2p_oppps_req->ctwindow = ctw;

    /* Send the MM_P2P_OPPPS_REQ message to LMAC FW */
    error = siwifi_send_msg(siwifi_hw, p2p_oppps_req, 1, MM_SET_P2P_OPPPS_CFM, cfm);

    return (error);
}

int siwifi_send_p2p_noa_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif,
                          int count, int interval, int duration, bool dyn_noa,
                          struct mm_set_p2p_noa_cfm *cfm)
{
    struct mm_set_p2p_noa_req *p2p_noa_req;
    int error;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Param check */
    if (count > 255)
        count = 255;

    if (duration >= interval) {
        dev_err(siwifi_hw->dev, "Invalid p2p NOA config: interval=%d <= duration=%d\n",
                interval, duration);
        return -EINVAL;
    }

    /* Build the MM_SET_P2P_NOA_REQ message */
    p2p_noa_req = siwifi_msg_zalloc(MM_SET_P2P_NOA_REQ, TASK_MM, DRV_TASK_ID,
                                  sizeof(struct mm_set_p2p_noa_req));

    if (!p2p_noa_req) {
        return -ENOMEM;
    }

    /* Fill the message parameters */
    p2p_noa_req->vif_index = siwifi_vif->vif_index;
    p2p_noa_req->noa_inst_nb = 0;
    p2p_noa_req->count = count;

    if (count) {
        p2p_noa_req->duration_us = duration * 1024;
        p2p_noa_req->interval_us = interval * 1024;
        p2p_noa_req->start_offset = (interval - duration - 10) * 1024;
        p2p_noa_req->dyn_noa = dyn_noa;
    }

    /* Send the MM_SET_2P_NOA_REQ message to LMAC FW */
    error = siwifi_send_msg(siwifi_hw, p2p_noa_req, 1, MM_SET_P2P_NOA_CFM, cfm);

    return (error);
}
#endif /* CONFIG_SIWIFI_P2P_DEBUGFS */

#ifdef CONFIG_SIWIFI_RF_CMD_DUMP
int siwifi_send_rf_cmd_dump(struct siwifi_hw *siwifi_hw, struct mm_rf_cmd_dump_cfm *cfm, uint32_t address, uint32_t size, uint8_t flag)
{
    struct mm_rf_cmd_dump_req *req;
    /*Build the MM_RF_CMD_DUMP_REQ message */
    req = siwifi_msg_zalloc(MM_RF_CMD_DUMP_REQ, TASK_MM, DRV_TASK_ID,
                            sizeof(struct mm_rf_cmd_dump_req));
    if(!req)
        return -ENOMEM;

    /*  Set parameters */
    req->address = (uint32_t)(CPHYSADDR(address));
    req->size = size;
    req->flag = flag;
    return siwifi_send_msg(siwifi_hw, req, 1, MM_RF_CMD_DUMP_CFM, cfm);
}
#endif

#ifdef CONFIG_SIWIFI_RF_RECALI
int siwifi_send_rf_ctrl(struct siwifi_hw *siwifi_hw, uint8_t ctrl)
{
    struct mm_rf_ctrl_req *req;
    uint16_t flags = 0;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    /*Build the MM_STOP_RF_COM message */
    req = siwifi_msg_zalloc(MM_RF_CTRL_REQ, TASK_MM, DRV_TASK_ID,
                            sizeof(struct mm_rf_ctrl_req));
    if(!req)
        return -ENOMEM;

    /*  Set parameters */
    req->ctrl_id = ctrl;

    if (ctrl == MM_RF_CTRL_STOP_COMMUNICATION)
        flags = SIWIFI_CMD_FLAG_ASK_PAUSE;
    else if(ctrl == MM_RF_CTRL_START_COMMUNICATION)
        flags = (SIWIFI_CMD_FLAG_FORCE_PUSH | SIWIFI_CMD_FLAG_ASK_RESUME);

    return siwifi_send_msg_with_flag(siwifi_hw, req, 1, MM_RF_CTRL_CFM, NULL, flags);
}

int siwifi_send_rf_calibrate(struct siwifi_hw *siwifi_hw, uint16_t conf)
{
    struct mm_rf_cali_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    /*Build the MM_STOP_RF_COM message */
    req = siwifi_msg_zalloc(MM_RF_CALI_REQ, TASK_MM, DRV_TASK_ID,
                            sizeof(struct mm_rf_cali_req));
    if(!req)
        return -ENOMEM;

    /*  Set parameters */
    req->conf = conf;
    req->bw = 0;
    req->chan = 0;
    return siwifi_send_msg(siwifi_hw, req, 1, MM_RF_CALI_CFM, NULL);
}
#endif

#ifdef CONFIG_SIWIFI_TX_POWER_CALI
int siwifi_send_txpower_cali_enable_req(struct siwifi_hw *siwifi_hw)
{
    struct mm_txpower_cali_enable_req *req;
    struct phy_aetnensis_cfg_tag *phy_tag;
    phy_tag = (struct phy_aetnensis_cfg_tag *)&siwifi_hw->phy_config;
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    /*Build the MM_TXPOW_CALI_ENABLE_REQ message */
    req = siwifi_msg_zalloc(MM_TXPOWER_CALI_ENABLE_REQ, TASK_MM, DRV_TASK_ID,
                            sizeof(struct mm_rf_cali_req));
    if(!req)
        return -ENOMEM;

    /*  Set parameters */
    req->enable_flag = phy_tag->flag;
    return siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
}
#endif

int siwifi_send_rf_get_temperature(struct siwifi_hw *siwifi_hw, struct mm_rf_get_temp_cfm *cfm)
{
    void *void_param;
    /*MM_RF_GET_TEMP_REQ has no parameter */
    void_param = siwifi_msg_zalloc(MM_RF_GET_TEMP_REQ, TASK_MM, DRV_TASK_ID, 0);
    if (!void_param)
        return -ENOMEM;

    return siwifi_send_msg(siwifi_hw, void_param, 1, MM_RF_GET_TEMP_CFM, cfm);
}

#ifdef CONFIG_SIWIFI_COOLING_TEMP
#ifdef CONFIG_SFA28_FULLMASK
int	siwifi_send_change_power_req(struct siwifi_hw *siwifi_hw,int change_power,int change_power_trend){
	struct mm_change_power_req *req;

	SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
	req = siwifi_msg_zalloc(MM_CHANGE_POWER_REQ, TASK_MM, DRV_TASK_ID,sizeof(struct mm_change_power_req));
	if(!req)
		return -ENOMEM;
	req->change_power = change_power;
        req->change_power_trend = change_power_trend;
	return siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
}
#else
int siwifi_send_cooling_temp_req(struct siwifi_hw *siwifi_hw)
{
    struct mm_cooling_temp_req *req;
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    /*Build the MM_COOLING_TEMP_REQ message */
    req = siwifi_msg_zalloc(MM_COOLING_TEMP_REQ, TASK_MM, DRV_TASK_ID,
                            sizeof(struct mm_cooling_temp_req));
    if(!req)
        return -ENOMEM;

    /*  Set parameters */
    req->max_pow_idx = siwifi_hw->max_txpower_idx;
    return siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
}
#endif
#endif

#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
int siwifi_send_me_ate_tools_op(struct siwifi_hw *siwifi_hw,
                struct cfg80211_chan_def *def,
                struct me_ate_tools_op_cfm *cfm,
                uint8_t flag)
{
    struct me_ate_tools_op_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the ME_ATE_TOOLS_OP_REQ message */
    req = siwifi_msg_zalloc(ME_ATE_TOOLS_OP_REQ, TASK_ME, DRV_TASK_ID,
                                       sizeof(struct me_ate_tools_op_req));
    if (flag == DELETE_CHANTX_OP || flag == SET_HW_IDLE) {
        req->vif_idx = siwifi_hw->ate_env.siwifi_vif->vif_index;
        req->ch_index = siwifi_hw->ate_env.siwifi_vif->ch_index;
        req->flag = flag;
        goto DONE;
    }

    if (((!req) || (!siwifi_hw->ate_env.siwifi_vif) || (!def))){
        printk("sfwifi_hw has no vif and command flags is not SET_HW_IDLE\n");
        return -ENOMEM;
    }
    /* Set parameters for the ME_ATE_TOOLS_OP_REQ message */
    req->chan.band = def->chan->band;
#if DEBUG_ARRAY_CHECK
    BUG_ON(def->width > NL80211_CHAN_WIDTH_80P80);
#endif
    req->ch_width = bw2chnl[def->width];
    req->chan.freq = def->chan->center_freq;
    req->center_freq1 = def->center_freq1;
    req->center_freq2 = def->center_freq2;
    req->chan.tx_power = def->chan->max_power;
    req->vif_idx = siwifi_hw->ate_env.siwifi_vif->vif_index;
    req->flag = flag;
    if (siwifi_hw->use_phy_bw_tweaks) {
        /* XXX Tweak for 80MHz VHT */
        if (def->width > NL80211_CHAN_WIDTH_40) {
            int _offs = req->center_freq1 - req->chan.freq;
            req->ch_width = PHY_CHNL_BW_40;
            req->center_freq1 = req->chan.freq + 10 *
                (_offs > 0 ? 1 : -1) * (abs(_offs) > 10 ? 1 : -1);
            SIWIFI_DBG("Tweak for 80MHz VHT: 80MHz chan requested\n");
        }
    }
    //set rate info
    req->mode = siwifi_hw->ate_env.conf.rate.mode;
    req->rate_idx = (u8_l)siwifi_hw->ate_env.conf.rate.rate_idx;
    req->use_short_preamble = siwifi_hw->ate_env.conf.rate.use_short_preamble;
    req->use_short_gi = siwifi_hw->ate_env.conf.rate.use_short_gi;
    req->frame_bw = siwifi_hw->ate_env.conf.pkg.frame_bw;
    //mac_addr
    memcpy(&req->vif_mac_addr, &siwifi_hw->ate_env.conf.qos_machdr.siwifi_sa, ETH_ALEN);
    memcpy(&req->sta_mac_addr, &siwifi_hw->ate_env.conf.qos_machdr.siwifi_bssid, ETH_ALEN);
DONE:
    /* Send the ME_ATE_TOOLS_OP_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, ME_ATE_TOOLS_OP_CFM, cfm);
}
#endif

/******************************************************************************
 *    Control messages handling functions (FULLMAC only)
 *****************************************************************************/
int siwifi_send_me_config_req(struct siwifi_hw *siwifi_hw)
{
    struct me_config_req *req;
    struct wiphy *wiphy = siwifi_hw->wiphy;
    struct ieee80211_sta_ht_cap *ht_cap = NULL;
    struct ieee80211_sta_vht_cap *vht_cap = NULL;
#if MY_LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0)
    struct ieee80211_sta_he_cap const *he_cap;
#endif
    uint8_t *ht_mcs = NULL;
    int i;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    if ( wiphy->bands[NL80211_BAND_2GHZ] != NULL ) {
        ht_cap = &wiphy->bands[NL80211_BAND_2GHZ]->ht_cap;
        vht_cap = &wiphy->bands[NL80211_BAND_2GHZ]->vht_cap;
        ht_mcs = (uint8_t *)&ht_cap->mcs;
    }

    if ( wiphy->bands[NL80211_BAND_5GHZ] != NULL ) {
        ht_cap = &wiphy->bands[NL80211_BAND_5GHZ]->ht_cap;
        vht_cap = &wiphy->bands[NL80211_BAND_5GHZ]->vht_cap;
        ht_mcs = (uint8_t *)&ht_cap->mcs;
    }

    if (!ht_cap) {
        printk("fatal error, no ht_cap, not inited?\n");
        return -EINVAL;
    }

    /* Build the ME_CONFIG_REQ message */
    req = siwifi_msg_zalloc(ME_CONFIG_REQ, TASK_ME, DRV_TASK_ID,
                                   sizeof(struct me_config_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the ME_CONFIG_REQ message */
    req->ht_supp = ht_cap->ht_supported;
    req->vht_supp = vht_cap ? vht_cap->vht_supported : 0;
    req->ht_cap.ht_capa_info = cpu_to_le16(ht_cap->cap);
    req->ht_cap.a_mpdu_param = ht_cap->ampdu_factor |
                                     (ht_cap->ampdu_density <<
                                         IEEE80211_HT_AMPDU_PARM_DENSITY_SHIFT);
    for (i = 0; i < sizeof(ht_cap->mcs); i++)
        req->ht_cap.mcs_rate[i] = ht_mcs[i];
    req->ht_cap.ht_extended_capa = 0;
    req->ht_cap.tx_beamforming_capa = 0;
    req->ht_cap.asel_capa = 0;

    if (vht_cap) {
        req->vht_cap.vht_capa_info = cpu_to_le32(vht_cap->cap);
        req->vht_cap.rx_highest = cpu_to_le16(vht_cap->vht_mcs.rx_highest);
        req->vht_cap.rx_mcs_map = cpu_to_le16(vht_cap->vht_mcs.rx_mcs_map);
        req->vht_cap.tx_highest = cpu_to_le16(vht_cap->vht_mcs.tx_highest);
        req->vht_cap.tx_mcs_map = cpu_to_le16(vht_cap->vht_mcs.tx_mcs_map);

#if MY_LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0)
        if (wiphy->bands[NL80211_BAND_5GHZ]->iftype_data != NULL) {
            he_cap = &wiphy->bands[NL80211_BAND_5GHZ]->iftype_data->he_cap;

            req->he_supp = he_cap->has_he;
            for (i = 0; i < MAC_HE_MAC_CAPA_LEN; i++) {
                req->he_cap.mac_cap_info[i] = he_cap->he_cap_elem.mac_cap_info[i];
            }
            for (i = 0; i < MAC_HE_PHY_CAPA_LEN; i++) {
                req->he_cap.phy_cap_info[i] = he_cap->he_cap_elem.phy_cap_info[i];
            }
            req->he_cap.mcs_supp.rx_mcs_80 = cpu_to_le16(he_cap->he_mcs_nss_supp.rx_mcs_80);
            req->he_cap.mcs_supp.tx_mcs_80 = cpu_to_le16(he_cap->he_mcs_nss_supp.tx_mcs_80);
            req->he_cap.mcs_supp.rx_mcs_160 = cpu_to_le16(he_cap->he_mcs_nss_supp.rx_mcs_160);
            req->he_cap.mcs_supp.tx_mcs_160 = cpu_to_le16(he_cap->he_mcs_nss_supp.tx_mcs_160);
            req->he_cap.mcs_supp.rx_mcs_80p80 = cpu_to_le16(he_cap->he_mcs_nss_supp.rx_mcs_80p80);
            req->he_cap.mcs_supp.tx_mcs_80p80 = cpu_to_le16(he_cap->he_mcs_nss_supp.tx_mcs_80p80);
        }
#else
        req->he_supp = false;
#endif
    }
    req->ps_on = siwifi_hw->mod_params->ps_on;
    req->tx_lft = siwifi_hw->mod_params->tx_lft;
    req->ant_div_on = siwifi_hw->mod_params->ant_div;
    req->rx_bufsz = siwifi_hw->ipc_env->rx_bufsz;
    req->rx_pd_offset = siwifi_hw->ipc_env->rx_pd_offset;

    printk("rx_bufsz - %d, rx_pd_offset - %d\n", req->rx_bufsz, req->rx_pd_offset);
    /* Send the ME_CONFIG_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, ME_CONFIG_CFM, NULL);
}

int siwifi_send_me_chan_config_req(struct siwifi_hw *siwifi_hw)
{
    struct me_chan_config_req *req;
    struct wiphy *wiphy = siwifi_hw->wiphy;
    int i;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the ME_CHAN_CONFIG_REQ message */
    req = siwifi_msg_zalloc(ME_CHAN_CONFIG_REQ, TASK_ME, DRV_TASK_ID,
                                            sizeof(struct me_chan_config_req));
    if (!req)
        return -ENOMEM;

    req->chan2G4_cnt=  0;
    if (wiphy->bands[NL80211_BAND_2GHZ] != NULL) {
        struct ieee80211_supported_band *b = wiphy->bands[NL80211_BAND_2GHZ];
        for (i = 0; i < b->n_channels; i++) {
            req->chan2G4[req->chan2G4_cnt].flags = 0;
            if (b->channels[i].flags & IEEE80211_CHAN_DISABLED)
                req->chan2G4[req->chan2G4_cnt].flags |= SCAN_DISABLED_BIT;
            req->chan2G4[req->chan2G4_cnt].flags |= passive_scan_flag(NL80211_IFTYPE_STATION, b->channels[i].flags);
            req->chan2G4[req->chan2G4_cnt].band = NL80211_BAND_2GHZ;
            req->chan2G4[req->chan2G4_cnt].freq = b->channels[i].center_freq;
            req->chan2G4[req->chan2G4_cnt].tx_power = b->channels[i].max_power;
            req->chan2G4_cnt++;
            if (req->chan2G4_cnt == SCAN_CHANNEL_2G4)
                break;
        }
    }

    req->chan5G_cnt = 0;
    if (wiphy->bands[NL80211_BAND_5GHZ] != NULL) {
        struct ieee80211_supported_band *b = wiphy->bands[NL80211_BAND_5GHZ];
        for (i = 0; i < b->n_channels; i++) {
            req->chan5G[req->chan5G_cnt].flags = 0;
            if (b->channels[i].flags & IEEE80211_CHAN_DISABLED)
                req->chan5G[req->chan5G_cnt].flags |= SCAN_DISABLED_BIT;
            req->chan5G[req->chan5G_cnt].flags |= passive_scan_flag(NL80211_IFTYPE_STATION, b->channels[i].flags);
            req->chan5G[req->chan5G_cnt].band = NL80211_BAND_5GHZ;
            req->chan5G[req->chan5G_cnt].freq = b->channels[i].center_freq;
            req->chan5G[req->chan5G_cnt].tx_power = b->channels[i].max_power;
            req->chan5G_cnt++;
            if (req->chan5G_cnt == SCAN_CHANNEL_5G)
                break;
        }
    }

    /* Send the ME_CHAN_CONFIG_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, ME_CHAN_CONFIG_CFM, NULL);
}

int siwifi_send_me_set_control_port_req(struct siwifi_hw *siwifi_hw, bool opened, u8 sta_idx)
{
    struct me_set_control_port_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the ME_SET_CONTROL_PORT_REQ message */
    req = siwifi_msg_zalloc(ME_SET_CONTROL_PORT_REQ, TASK_ME, DRV_TASK_ID,
                                   sizeof(struct me_set_control_port_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the ME_SET_CONTROL_PORT_REQ message */
    req->sta_idx = sta_idx;
    req->control_port_open = opened;

    /* Send the ME_SET_CONTROL_PORT_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, ME_SET_CONTROL_PORT_CFM, NULL);
}

int siwifi_send_me_sta_add(struct siwifi_hw *siwifi_hw, struct station_parameters *params,
                         const u8 *mac, u8 inst_nbr, struct me_sta_add_cfm *cfm)
{
    struct me_sta_add_req *req;
    u8 *ht_mcs = (u8 *)&params->ht_capa->mcs;
    int i;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    /* Build the MM_STA_ADD_REQ message */
    req = siwifi_msg_zalloc(ME_STA_ADD_REQ, TASK_ME, DRV_TASK_ID,
                                  sizeof(struct me_sta_add_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the MM_STA_ADD_REQ message */
    memcpy(&(req->mac_addr.array[0]), mac, ETH_ALEN);

    req->rate_set.length = params->supported_rates_len;
    for (i = 0; i < params->supported_rates_len; i++)
        req->rate_set.array[i] = params->supported_rates[i];

    req->flags = 0;
    if (params->ht_capa) {
        const struct ieee80211_ht_cap *ht_capa = params->ht_capa;

        req->flags |= STA_HT_CAPA;
        req->ht_cap.ht_capa_info = cpu_to_le16(ht_capa->cap_info);
        req->ht_cap.a_mpdu_param = ht_capa->ampdu_params_info;
        for (i = 0; i < sizeof(ht_capa->mcs); i++)
            req->ht_cap.mcs_rate[i] = ht_mcs[i];
        req->ht_cap.ht_extended_capa = cpu_to_le16(ht_capa->extended_ht_cap_info);
        req->ht_cap.tx_beamforming_capa = cpu_to_le32(ht_capa->tx_BF_cap_info);
        req->ht_cap.asel_capa = ht_capa->antenna_selection_info;
    }
    if (params->vht_capa || (params->supported_channels &&
                             siwifi_params_5g_channel_check(params->supported_channels,
                                                            params->supported_channels_len))) {
        struct ieee80211_vht_cap vht_capa_temp;
        const struct ieee80211_vht_cap *vht_capa = NULL;
        int temp = 0;

        if (params->vht_capa) {
            vht_capa = params->vht_capa;
        } else {
            /* Build default VHT capability */
            int mcs_map, bw_max;
            const int mcs_map_to_rate[4][3] = {
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
#define MAX_VHT_RATE(map, nss, bw) (mcs_map_to_rate[bw][map] * (nss))

            vht_capa_temp.vht_cap_info = 0;
            memset(&vht_capa_temp.supp_mcs.tx_mcs_map, 0xff, sizeof(uint16_t));
            memset(&vht_capa_temp.supp_mcs.rx_mcs_map, 0xff, sizeof(uint16_t));
            vht_capa_temp.supp_mcs.tx_highest = 0;
            vht_capa_temp.supp_mcs.rx_highest = 0;
            mcs_map = IEEE80211_VHT_MCS_SUPPORT_0_7;
            bw_max = PHY_CHNL_BW_20;

            switch (siwifi_hw->vif_table[0]->wdev.chandef.width) {
                case NL80211_CHAN_WIDTH_20_NOHT:
                case NL80211_CHAN_WIDTH_20:
                    mcs_map = IEEE80211_VHT_MCS_SUPPORT_0_8;
                    bw_max = PHY_CHNL_BW_20;
                    break;
                case NL80211_CHAN_WIDTH_40:
                    mcs_map = IEEE80211_VHT_MCS_SUPPORT_0_9;
                    bw_max = PHY_CHNL_BW_40;
                    break;
                default:
                    break;
            }
            vht_capa_temp.supp_mcs.tx_highest = MAX_VHT_RATE(mcs_map, siwifi_hw->mod_params->nss, bw_max);
            vht_capa_temp.supp_mcs.rx_highest = MAX_VHT_RATE(mcs_map, siwifi_hw->mod_params->nss, bw_max);
            for (temp = 0; temp < siwifi_hw->mod_params->nss; temp++) {
                vht_capa_temp.supp_mcs.tx_mcs_map = vht_capa_temp.supp_mcs.tx_mcs_map << 2;
                vht_capa_temp.supp_mcs.rx_mcs_map = vht_capa_temp.supp_mcs.rx_mcs_map << 2;
                vht_capa_temp.supp_mcs.tx_mcs_map |= mcs_map;
                vht_capa_temp.supp_mcs.rx_mcs_map |= mcs_map;
            }
            vht_capa_temp.vht_cap_info = 7 << IEEE80211_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_SHIFT;
            vht_capa_temp.vht_cap_info |= IEEE80211_VHT_CAP_TXSTBC;
            vht_capa_temp.vht_cap_info |= IEEE80211_VHT_CAP_MAX_MPDU_LENGTH_11454;
            vht_capa_temp.vht_cap_info |= IEEE80211_VHT_CAP_RXLDPC;
            vht_capa_temp.vht_cap_info |= IEEE80211_VHT_CAP_RXSTBC_1;
            vht_capa_temp.vht_cap_info |= IEEE80211_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_MASK;
            vht_capa_temp.vht_cap_info |= IEEE80211_VHT_CAP_RX_ANTENNA_PATTERN;
            vht_capa_temp.vht_cap_info |= IEEE80211_VHT_CAP_TX_ANTENNA_PATTERN;

            vht_capa = &vht_capa_temp;
        }
        req->flags |= STA_VHT_CAPA;
        req->vht_cap.vht_capa_info = cpu_to_le32(vht_capa->vht_cap_info);
        req->vht_cap.rx_highest = cpu_to_le16(vht_capa->supp_mcs.rx_highest);
        req->vht_cap.rx_mcs_map = cpu_to_le16(vht_capa->supp_mcs.rx_mcs_map);
        req->vht_cap.tx_highest = cpu_to_le16(vht_capa->supp_mcs.tx_highest);
        req->vht_cap.tx_mcs_map = cpu_to_le16(vht_capa->supp_mcs.tx_mcs_map);
    }

#if MY_LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0)
    if (params->he_capa) {
        const struct ieee80211_he_cap_elem *he_capa = params->he_capa;

        req->flags |= STA_HE_CAPA;
        for (i = 0; i < MAC_HE_MAC_CAPA_LEN; i++) {
            req->he_cap.mac_cap_info[i] = he_capa->mac_cap_info[i];
        }
        for (i = 0; i < MAC_HE_PHY_CAPA_LEN; i++) {
            req->he_cap.phy_cap_info[i] = he_capa->phy_cap_info[i];
        }
        req->he_cap.mcs_supp.rx_mcs_80 = cpu_to_le16(0xfffa);
        req->he_cap.mcs_supp.tx_mcs_80 = cpu_to_le16(0xfffa);
        req->he_cap.mcs_supp.rx_mcs_160 = cpu_to_le16(0xffff);
        req->he_cap.mcs_supp.tx_mcs_160 = cpu_to_le16(0xffff);
        req->he_cap.mcs_supp.rx_mcs_80p80 = cpu_to_le16(0xffff);
        req->he_cap.mcs_supp.tx_mcs_80p80 = cpu_to_le16(0xffff);
    }
#endif

    if (params->sta_flags_set & BIT(NL80211_STA_FLAG_WME))
        req->flags |= STA_QOS_CAPA;

    if (params->sta_flags_set & BIT(NL80211_STA_FLAG_MFP))
        req->flags |= STA_MFP_CAPA;

    if (params->opmode_notif_used) {
        req->flags |= STA_OPMOD_NOTIF;
        req->opmode = params->opmode_notif;
    }

    req->aid = cpu_to_le16(params->aid);
    req->uapsd_queues = params->uapsd_queues;
    req->max_sp_len = params->max_sp * 2;
    req->vif_idx = inst_nbr;

    if (params->sta_flags_set & BIT(NL80211_STA_FLAG_TDLS_PEER)) {
        struct siwifi_vif *siwifi_vif = siwifi_hw->vif_table[inst_nbr];
        req->tdls_sta = true;
        if ((params->ext_capab[3] & WLAN_EXT_CAPA4_TDLS_CHAN_SWITCH) &&
            !siwifi_vif->tdls_chsw_prohibited)
            req->tdls_chsw_allowed = true;
        if (siwifi_vif->tdls_status == TDLS_SETUP_RSP_TX)
            req->tdls_sta_initiator = true;
    }
    if (cfm->status) {
        req->req_index_valid = true;
        req->req_index = cfm->sta_idx;
    }
    req->recovering = false;
    // recovering dying gasp information to this station
    if (req->req_index_valid &&
            siwifi_hw->dying_gasp_valid &&
            test_bit(SIWIFI_DEV_STACK_RESTARTING, &siwifi_hw->drv_flags))
    {
        struct mm_dying_gasp *dying_gasp_info = (struct mm_dying_gasp *)siwifi_hw->dying_gasp_info_elem.addr;
        // Get dying gasp information
        for (i = 0; i < dying_gasp_info->sta_len; i++) {
            if (dying_gasp_info->stas[i].index != req->req_index)
                continue;
            BUG_ON(dying_gasp_info->stas[i].vif_index != inst_nbr);
            req->recovering = true;
            req->dialog_token_allocator = dying_gasp_info->stas[i].dialog_token_allocator;
            memcpy(req->seq_nbr, dying_gasp_info->stas[i].seq_nbr, sizeof(uint16_t) * TID_MAX);
        }
    }
    if (req->recovering) {
        printk("dialog_token_allocator %d, TID[%d %d %d %d %d %d %d %d %d]\n",
                req->dialog_token_allocator, req->seq_nbr[0], req->seq_nbr[1], req->seq_nbr[2], req->seq_nbr[3],
                req->seq_nbr[4], req->seq_nbr[5], req->seq_nbr[6], req->seq_nbr[7], req->seq_nbr[8]);
    }

    /* Send the ME_STA_ADD_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, ME_STA_ADD_CFM, cfm);
}

int siwifi_send_me_sta_del(struct siwifi_hw *siwifi_hw, u8 sta_idx, bool tdls_sta)
{
    struct me_sta_del_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MM_STA_DEL_REQ message */
    req = siwifi_msg_zalloc(ME_STA_DEL_REQ, TASK_ME, DRV_TASK_ID,
                          sizeof(struct me_sta_del_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the MM_STA_DEL_REQ message */
    req->sta_idx = sta_idx;
    req->tdls_sta = tdls_sta;

    /* Send the ME_STA_DEL_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, ME_STA_DEL_CFM, NULL);
}

int siwifi_send_me_traffic_ind(struct siwifi_hw *siwifi_hw, u8 sta_idx, bool uapsd, u8 tx_status)
{
    struct me_traffic_ind_req *req;

    //SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    /* Build the ME_UTRAFFIC_IND_REQ message */
    req = siwifi_msg_zalloc(ME_TRAFFIC_IND_REQ, TASK_ME, DRV_TASK_ID,
                          sizeof(struct me_traffic_ind_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the ME_TRAFFIC_IND_REQ message */
    req->sta_idx = sta_idx;
    req->tx_avail = tx_status;
    req->uapsd = uapsd;

    /* Send the ME_TRAFFIC_IND_REQ to UMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, ME_TRAFFIC_IND_CFM, NULL);
}

int siwifi_send_me_rc_stats(struct siwifi_hw *siwifi_hw,
                          u8 sta_idx,
                          struct me_rc_stats_cfm *cfm)
{
    struct me_rc_stats_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the ME_RC_STATS_REQ message */
    req = siwifi_msg_zalloc(ME_RC_STATS_REQ, TASK_ME, DRV_TASK_ID,
                                  sizeof(struct me_rc_stats_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the ME_RC_STATS_REQ message */
    req->sta_idx = sta_idx;

    /* Send the ME_RC_STATS_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, ME_RC_STATS_CFM, cfm);
}

int siwifi_send_me_rc_set_rate(struct siwifi_hw *siwifi_hw,
                             u8 sta_idx,
                             u16 rate_cfg)
{
    struct me_rc_set_rate_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the ME_RC_SET_RATE_REQ message */
    req = siwifi_msg_zalloc(ME_RC_SET_RATE_REQ, TASK_ME, DRV_TASK_ID,
                          sizeof(struct me_rc_set_rate_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the ME_RC_SET_RATE_REQ message */
    req->sta_idx = sta_idx;
    req->fixed_rate_cfg = rate_cfg;

    /* Send the ME_RC_SET_RATE_REQ message to FW */
    return siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
}

int siwifi_send_me_rc_set_no_ss(struct siwifi_hw *siwifi_hw, u8 no_ss)
{
    struct me_rc_set_no_ss_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the ME_RC_SET_NO_SS_REQ message */
    req = siwifi_msg_zalloc(ME_RC_SET_NO_SS_REQ, TASK_ME, DRV_TASK_ID,
                          sizeof(struct me_rc_set_no_ss_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the ME_RC_SET_NO_SS_REQ message */
    req->no_ss = no_ss;

    /* Send the ME_RC_SET_NO_SS_REQ message to FW */
    return siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
}

int siwifi_send_assoc_insert_info(struct siwifi_hw *siwifi_hw)
{
    struct me_assoc_insert_info_req *req = NULL;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    req = siwifi_msg_zalloc(ME_ASSOC_INSERT_INFO_REQ, TASK_ME, DRV_TASK_ID,
                        sizeof(struct me_assoc_insert_info_req));

    if (!req) {
        return -ENOMEM;
    }

    req->info_dmalength = siwifi_hw->assoc_insert.info_dmalength;
    req->info_dmaaddr = siwifi_hw->assoc_insert.info_dmaaddr;

    return siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
}

int siwifi_send_auth_insert_info(struct siwifi_hw *siwifi_hw)
{
    struct me_auth_insert_info_req *req = NULL;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    req = siwifi_msg_zalloc(ME_AUTH_INSERT_INFO_REQ, TASK_ME, DRV_TASK_ID,
                        sizeof(struct me_auth_insert_info_req));

    if (!req) {
        return -ENOMEM;
    }

    req->info_dmalength = siwifi_hw->auth_insert.info_dmalength;
    req->info_dmaaddr = siwifi_hw->auth_insert.info_dmaaddr;

    return siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
}

int siwifi_send_sm_connect_req(struct siwifi_hw *siwifi_hw,
                             struct siwifi_vif *siwifi_vif,
                             struct cfg80211_connect_params *sme,
                             struct sm_connect_cfm *cfm)
{
    struct sm_connect_req *req;
    int i;
    u32_l flags = 0;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the SM_CONNECT_REQ message */
    req = siwifi_msg_zalloc(SM_CONNECT_REQ, TASK_SM, DRV_TASK_ID,
                                   sizeof(struct sm_connect_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the SM_CONNECT_REQ message */
    if (sme->crypto.n_ciphers_pairwise &&
        ((sme->crypto.ciphers_pairwise[0] == WLAN_CIPHER_SUITE_WEP40) ||
         (sme->crypto.ciphers_pairwise[0] == WLAN_CIPHER_SUITE_TKIP) ||
         (sme->crypto.ciphers_pairwise[0] == WLAN_CIPHER_SUITE_WEP104)))
        flags |= DISABLE_HT;

    if (sme->crypto.control_port)
        flags |= CONTROL_PORT_HOST;

    if (sme->crypto.control_port_no_encrypt)
        flags |= CONTROL_PORT_NO_ENC;

    if (use_pairwise_key(&sme->crypto))
        flags |= WPA_WPA2_IN_USE;

    if (sme->mfp == NL80211_MFP_REQUIRED)
        flags |= MFP_IN_USE;

    req->ctrl_port_ethertype = sme->crypto.control_port_ethertype;

    if (sme->bssid)
        memcpy(&req->bssid, sme->bssid, ETH_ALEN);
    else
        req->bssid = mac_addr_bcst;
    req->vif_idx = siwifi_vif->vif_index;
    if (sme->channel) {
        req->chan.band = sme->channel->band;
        req->chan.freq = sme->channel->center_freq;
        req->chan.flags = passive_scan_flag(SIWIFI_VIF_TYPE(siwifi_vif), sme->channel->flags);
    } else {
        req->chan.freq = (u16_l)-1;
    }

    for (i = 0; i < sme->ssid_len; i++)
        req->ssid.array[i] = sme->ssid[i];
    req->ssid.length = sme->ssid_len;
    req->flags = flags;
    if (WARN_ON(sme->ie_len > sizeof(req->ie_buf)))
        goto invalid_param;
    if (sme->ie_len)
        memcpy(req->ie_buf, sme->ie, sme->ie_len);
    req->ie_len = sme->ie_len;
    req->listen_interval = siwifi_hw->mod_params->listen_itv;
    req->dont_wait_bcmc = !siwifi_hw->mod_params->listen_bcmc;

    /* Set auth_type */
    if (sme->auth_type == NL80211_AUTHTYPE_AUTOMATIC)
        req->auth_type = WLAN_AUTH_OPEN;
    else if (sme->auth_type == NL80211_AUTHTYPE_OPEN_SYSTEM)
        req->auth_type = WLAN_AUTH_OPEN;
    else if (sme->auth_type == NL80211_AUTHTYPE_SHARED_KEY)
        req->auth_type = WLAN_AUTH_SHARED_KEY;
    else if (sme->auth_type == NL80211_AUTHTYPE_FT)
        req->auth_type = WLAN_AUTH_FT;
    else if (sme->auth_type == NL80211_AUTHTYPE_SAE)
        req->auth_type = WLAN_AUTH_SAE;
    else
        goto invalid_param;

    /* Set UAPSD queues */
    req->uapsd_queues = siwifi_hw->mod_params->uapsd_queues;

    /* Send the SM_CONNECT_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, SM_CONNECT_CFM, cfm);

invalid_param:
    siwifi_msg_free(siwifi_hw, req);
    return -EINVAL;
}

int siwifi_send_sm_disconnect_req(struct siwifi_hw *siwifi_hw,
                                struct siwifi_vif *siwifi_vif,
                                u16 reason)
{
    struct sm_disconnect_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the SM_DISCONNECT_REQ message */
    req = siwifi_msg_zalloc(SM_DISCONNECT_REQ, TASK_SM, DRV_TASK_ID,
                                   sizeof(struct sm_disconnect_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the SM_DISCONNECT_REQ message */
    req->reason_code = reason;
    req->vif_idx = siwifi_vif->vif_index;

    /* Send the SM_DISCONNECT_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, SM_DISCONNECT_CFM, NULL);
}

int siwifi_send_sm_external_auth_required_rsp(struct siwifi_hw *siwifi_hw,
                                            struct siwifi_vif *siwifi_vif,
                                            u16 status)
{
    struct sm_external_auth_required_rsp *rsp;

    /* Build the SM_EXTERNAL_AUTH_CFM message */
    rsp = siwifi_msg_zalloc(SM_EXTERNAL_AUTH_REQUIRED_RSP, TASK_SM, DRV_TASK_ID,
                          sizeof(struct sm_external_auth_required_rsp));
    if (!rsp)
        return -ENOMEM;

    rsp->status = status;
    rsp->vif_idx = siwifi_vif->vif_index;

    /* send the SM_EXTERNAL_AUTH_REQUIRED_RSP message UMAC FW */
    return siwifi_send_msg(siwifi_hw, rsp, 0, 0, NULL);
}

int siwifi_send_apm_start_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif,
                            struct cfg80211_ap_settings *settings,
                            struct apm_start_cfm *cfm,
                            struct siwifi_ipc_elem_var *elem)
{
    struct apm_start_req *req;
    struct siwifi_bcn *bcn = &vif->ap.bcn;
    u8 *buf;
    u8 *real_addr;
    u32 flags = 0;
    const u8 *rate_ie;
    u8 rate_len = 0;
    int var_offset = offsetof(struct ieee80211_mgmt, u.beacon.variable);
    const u8 *var_pos;
    int len, i, error;
    int ret;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the APM_START_REQ message */
    req = siwifi_msg_zalloc(APM_START_REQ, TASK_APM, DRV_TASK_ID,
                                   sizeof(struct apm_start_req));
    if (!req)
        return -ENOMEM;

    // Build the beacon
    bcn->dtim = (u8)settings->dtim_period;
    buf = siwifi_build_bcn(bcn, &settings->beacon, &real_addr);
    if (!buf) {
        siwifi_msg_free(siwifi_hw, req);
        return -ENOMEM;
    }

    // Retrieve the basic rate set from the beacon buffer
    len = bcn->len - var_offset;
    var_pos = buf + var_offset;

// Assume that rate higher that 54 Mbps are BSS membership
#define IS_BASIC_RATE(r) (r & 0x80) && ((r & ~0x80) <= (54 * 2))

    rate_ie = cfg80211_find_ie(WLAN_EID_SUPP_RATES, var_pos, len);
    if (rate_ie) {
        const u8 *rates = rate_ie + 2;
        for (i = 0; (i < rate_ie[1]) && (rate_len < MAC_RATESET_LEN); i++) {
            if (IS_BASIC_RATE(rates[i]))
                req->basic_rates.array[rate_len++] = rates[i];
        }
    }
    rate_ie = cfg80211_find_ie(WLAN_EID_EXT_SUPP_RATES, var_pos, len);
    if (rate_ie) {
        const u8 *rates = rate_ie + 2;
        for (i = 0; (i < rate_ie[1]) && (rate_len < MAC_RATESET_LEN); i++) {
            if (IS_BASIC_RATE(rates[i]))
                req->basic_rates.array[rate_len++] = rates[i];
        }
    }
    req->basic_rates.length = rate_len;
#undef IS_BASIC_RATE

    // Sync buffer for FW
    if ((error = siwifi_ipc_elem_var_allocs(siwifi_hw, elem, bcn->len,
                                          DMA_TO_DEVICE, buf, NULL, NULL, real_addr))) {
        return error;
    }

    /* Set parameters for the APM_START_REQ message */
    req->vif_idx = vif->vif_index;
    req->bcn_addr = elem->dma_addr;
    req->bcn_len = bcn->len;
    req->tim_oft = bcn->head_len;
    req->tim_len = bcn->tim_len;
    req->chan.band = settings->chandef.chan->band;
    req->chan.freq = settings->chandef.chan->center_freq;
    req->chan.flags = 0;
    req->chan.tx_power = settings->chandef.chan->max_power;
    req->center_freq1 = settings->chandef.center_freq1;
    req->center_freq2 = settings->chandef.center_freq2;
#if DEBUG_ARRAY_CHECK
    BUG_ON(settings->chandef.width > NL80211_CHAN_WIDTH_80P80);
#endif
    req->ch_width = bw2chnl[settings->chandef.width];
    req->bcn_int = settings->beacon_interval;
    req->recovering = test_bit(SIWIFI_DEV_STACK_RESTARTING, &siwifi_hw->drv_flags);
    req->is_radar_channel = (settings->chandef.chan->flags & IEEE80211_CHAN_RADAR) ? true : false;
    if (settings->crypto.control_port)
        flags |= CONTROL_PORT_HOST;

    if (settings->crypto.control_port_no_encrypt)
        flags |= CONTROL_PORT_NO_ENC;

    if (use_pairwise_key(&settings->crypto))
        flags |= WPA_WPA2_IN_USE;

    if (settings->crypto.control_port_ethertype)
        req->ctrl_port_ethertype = settings->crypto.control_port_ethertype;
    else
        req->ctrl_port_ethertype = ETH_P_PAE;
    req->flags = flags;

    /* Send the APM_START_REQ message to LMAC FW */
    ret = siwifi_send_msg(siwifi_hw, req, 1, APM_START_CFM, cfm);
    if (cfm->status == 0) {
        vif->txpower = settings->chandef.chan->max_power;
        vif->txpower_idx = settings->chandef.chan->max_power;
    }

    return ret;
}

int siwifi_send_apm_stop_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif)
{
    struct apm_stop_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the APM_STOP_REQ message */
    req = siwifi_msg_zalloc(APM_STOP_REQ, TASK_APM, DRV_TASK_ID,
                                   sizeof(struct apm_stop_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the APM_STOP_REQ message */
    req->vif_idx = vif->vif_index;

    /* Send the APM_STOP_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, APM_STOP_CFM, NULL);
}

uint16_t  concatenate_u8_to_u16(u8 value1, u8 value2)
{
    uint16_t result = ((u16)value1 << 8) | value2;
    return result;
}

int siwifi_send_scanu_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif,
                        struct cfg80211_scan_request *param)
{
    struct scanu_start_req *req;
    int i;
    uint8_t chan_flags = 0;
    uint16_t mac_array = 0;


    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the SCANU_START_REQ message */
    req = siwifi_msg_zalloc(SCANU_START_REQ, TASK_SCANU, DRV_TASK_ID,
                          sizeof(struct scanu_start_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters */
    req->vif_idx = siwifi_vif->vif_index;
    req->chan_cnt = (u8)min_t(int, SCAN_CHANNEL_MAX, param->n_channels);
    req->ssid_cnt = (u8)min_t(int, SCAN_SSID_MAX, param->n_ssids);

    if (param->bssid) {
        mac_array = (concatenate_u8_to_u16(param->bssid[1],param->bssid[0]));
        memcpy(&(req->bssid.array[0]), &mac_array, ETH_ALEN/3);
        mac_array = (concatenate_u8_to_u16(param->bssid[3],param->bssid[2]));
        memcpy(&(req->bssid.array[1]), &mac_array, ETH_ALEN/3);
        mac_array = (concatenate_u8_to_u16(param->bssid[5],param->bssid[4]));
        memcpy(&(req->bssid.array[2]), &mac_array, ETH_ALEN/3);
    }
    else {
        req->bssid = mac_addr_bcst;
    }

    req->no_cck = param->no_cck;

    if (req->ssid_cnt == 0)
        chan_flags |= SCAN_PASSIVE_BIT;
    for (i = 0; i < req->ssid_cnt; i++) {
        int j;
        for (j = 0; j < param->ssids[i].ssid_len; j++)
            req->ssid[i].array[j] = param->ssids[i].ssid[j];
        req->ssid[i].length = param->ssids[i].ssid_len;
    }

    if (param->ie) {
        if (siwifi_ipc_elem_var_allocs(siwifi_hw, &siwifi_hw->scan_ie,
                                     param->ie_len, DMA_TO_DEVICE,
                                     NULL, param->ie, NULL, NULL))
            goto error;

        req->add_ie_len = param->ie_len;
        req->add_ies = siwifi_hw->scan_ie.dma_addr;
    }
    else {
        req->add_ie_len = 0;
        req->add_ies = 0;
    }

    for (i = 0; i < req->chan_cnt; i++) {
        struct ieee80211_channel *chan = param->channels[i];

        req->chan[i].band = chan->band;
        req->chan[i].freq = chan->center_freq;
        req->chan[i].flags = chan_flags | passive_scan_flag(SIWIFI_VIF_TYPE(siwifi_vif), chan->flags);
        req->chan[i].tx_power = chan->max_reg_power;
    }

    /* Send the SCANU_START_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
error:
    if (req != NULL)
        siwifi_msg_free(siwifi_hw, req);
    return -ENOMEM;
}

int siwifi_send_apm_start_cac_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif,
                                struct cfg80211_chan_def *chandef,
                                struct apm_start_cac_cfm *cfm)
{
    struct apm_start_cac_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the APM_START_CAC_REQ message */
    req = siwifi_msg_zalloc(APM_START_CAC_REQ, TASK_APM, DRV_TASK_ID,
                          sizeof(struct apm_start_cac_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the APM_START_CAC_REQ message */
    req->vif_idx = vif->vif_index;
    req->chan.band = chandef->chan->band;
    req->chan.freq = chandef->chan->center_freq;
    req->chan.flags = 0;
    req->center_freq1 = chandef->center_freq1;
    req->center_freq2 = chandef->center_freq2;
    req->ch_width = bw2chnl[chandef->width];

    /* Send the APM_START_CAC_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, APM_START_CAC_CFM, cfm);
}

int siwifi_send_apm_stop_cac_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif)
{
    struct apm_stop_cac_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the APM_STOP_CAC_REQ message */
    req = siwifi_msg_zalloc(APM_STOP_CAC_REQ, TASK_APM, DRV_TASK_ID,
                          sizeof(struct apm_stop_cac_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the APM_STOP_CAC_REQ message */
    req->vif_idx = vif->vif_index;

    /* Send the APM_STOP_CAC_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, APM_STOP_CAC_CFM, NULL);
}

int siwifi_send_mesh_start_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif,
                             const struct mesh_config *conf, const struct mesh_setup *setup,
                             struct mesh_start_cfm *cfm)
{
    // Message to send
    struct mesh_start_req *req;
    // Supported basic rates

    struct ieee80211_supported_band *band = NULL;
    /* Counter */
    int i;
    /* Return status */
    int status;
    /* DMA Address to be unmapped after confirmation reception */
    u32 dma_addr = 0;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    if (siwifi_hw->wiphy->bands[NL80211_BAND_2GHZ] != NULL)
        band = siwifi_hw->wiphy->bands[NL80211_BAND_2GHZ];

    if (siwifi_hw->wiphy->bands[NL80211_BAND_5GHZ] != NULL)
        band = siwifi_hw->wiphy->bands[NL80211_BAND_5GHZ];

    if (!band) {
        printk("no invalid band information, aborting.., %s\n", __func__);
        return -EINVAL;
    }

    /* Build the MESH_START_REQ message */
    req = siwifi_msg_zalloc(MESH_START_REQ, TASK_MESH, DRV_TASK_ID,
                          sizeof(struct mesh_start_req));
    if (!req) {
        return -ENOMEM;
    }

    req->vif_index = vif->vif_index;
    req->bcn_int = setup->beacon_interval;
    req->dtim_period = setup->dtim_period;
    req->mesh_id_len = setup->mesh_id_len;

    for (i = 0; i < setup->mesh_id_len; i++) {
        req->mesh_id[i] = *(setup->mesh_id + i);
    }

    req->user_mpm = setup->user_mpm;
    req->is_auth = setup->is_authenticated;
    req->auth_id = setup->auth_id;
    req->ie_len = setup->ie_len;

    if (setup->ie_len) {
        /*
         * Need to provide a Virtual Address to the MAC so that it can download the
         * additional information elements.
         */
        req->ie_addr = dma_map_single(siwifi_hw->dev, (void *)setup->ie,
                                      setup->ie_len, DMA_FROM_DEVICE);

        /* Check DMA mapping result */
        if (dma_mapping_error(siwifi_hw->dev, req->ie_addr)) {
            printk(KERN_CRIT "%s - DMA Mapping error on additional IEs\n", __func__);

            /* Consider there is no Additional IEs */
            req->ie_len = 0;
        } else {
            /* Store DMA Address so that we can unmap the memory section once MESH_START_CFM is received */
            dma_addr = req->ie_addr;
        }
    }

    /* Provide rate information */
    req->basic_rates.length = 0;
    for (i = 0; i < band->n_bitrates; i++) {
        u16 rate = band->bitrates[i].bitrate;

        /* Read value is in in units of 100 Kbps, provided value is in units
         * of 1Mbps, and multiplied by 2 so that 5.5 becomes 11 */
        rate = (rate << 1) / 10;

        if (setup->basic_rates & CO_BIT(i)) {
            rate |= 0x80;
        }

        req->basic_rates.array[i] = (u8)rate;
        req->basic_rates.length++;
    }

    /* Provide channel information */
    req->chan.band = setup->chandef.chan->band;
    req->chan.freq = setup->chandef.chan->center_freq;
    req->chan.flags = 0;
    req->chan.tx_power = setup->chandef.chan->max_power;
    req->center_freq1 = setup->chandef.center_freq1;
    req->center_freq2 = setup->chandef.center_freq2;
#if DEBUG_ARRAY_CHECK
    BUG_ON(setup->chandef.width > NL80211_CHAN_WIDTH_80P80);
#endif
    req->ch_width = bw2chnl[setup->chandef.width];

    /* Send the MESH_START_REQ message to UMAC FW */
    status = siwifi_send_msg(siwifi_hw, req, 1, MESH_START_CFM, cfm);

    /* Unmap DMA area */
    if (setup->ie_len) {
        dma_unmap_single(siwifi_hw->dev, dma_addr, setup->ie_len, DMA_TO_DEVICE);
    }

    /* Return the status */
    return (status);
}

int siwifi_send_mesh_stop_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif,
                            struct mesh_stop_cfm *cfm)
{
    // Message to send
    struct mesh_stop_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MESH_STOP_REQ message */
    req = siwifi_msg_zalloc(MESH_STOP_REQ, TASK_MESH, DRV_TASK_ID,
                          sizeof(struct mesh_stop_req));
    if (!req) {
        return -ENOMEM;
    }

    req->vif_idx = vif->vif_index;

    /* Send the MESH_STOP_REQ message to UMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, MESH_STOP_CFM, cfm);
}

int siwifi_send_mesh_update_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif,
                              u32 mask, const struct mesh_config *p_mconf, struct mesh_update_cfm *cfm)
{
    // Message to send
    struct mesh_update_req *req;
    // Keep only bit for fields which can be updated
    u32 supp_mask = (mask << 1) & (CO_BIT(NL80211_MESHCONF_GATE_ANNOUNCEMENTS)
                                   | CO_BIT(NL80211_MESHCONF_HWMP_ROOTMODE)
                                   | CO_BIT(NL80211_MESHCONF_FORWARDING)
                                   | CO_BIT(NL80211_MESHCONF_POWER_MODE));


    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    if (!supp_mask) {
        return -ENOENT;
    }

    /* Build the MESH_UPDATE_REQ message */
    req = siwifi_msg_zalloc(MESH_UPDATE_REQ, TASK_MESH, DRV_TASK_ID,
                          sizeof(struct mesh_update_req));

    if (!req) {
        return -ENOMEM;
    }

    req->vif_idx = vif->vif_index;

    if (supp_mask & CO_BIT(NL80211_MESHCONF_GATE_ANNOUNCEMENTS))
    {
        req->flags |= CO_BIT(MESH_UPDATE_FLAGS_GATE_MODE_BIT);
        req->gate_announ = p_mconf->dot11MeshGateAnnouncementProtocol;
    }

    if (supp_mask & CO_BIT(NL80211_MESHCONF_HWMP_ROOTMODE))
    {
        req->flags |= CO_BIT(MESH_UPDATE_FLAGS_ROOT_MODE_BIT);
        req->root_mode = p_mconf->dot11MeshHWMPRootMode;
    }

    if (supp_mask & CO_BIT(NL80211_MESHCONF_FORWARDING))
    {
        req->flags |= CO_BIT(MESH_UPDATE_FLAGS_MESH_FWD_BIT);
        req->mesh_forward = p_mconf->dot11MeshForwarding;
    }

    if (supp_mask & CO_BIT(NL80211_MESHCONF_POWER_MODE))
    {
        req->flags |= CO_BIT(MESH_UPDATE_FLAGS_LOCAL_PSM_BIT);
        req->local_ps_mode = p_mconf->power_mode;
    }

    /* Send the MESH_UPDATE_REQ message to UMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, MESH_UPDATE_CFM, cfm);
}

int siwifi_send_mesh_peer_info_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif,
                                 u8 sta_idx, struct mesh_peer_info_cfm *cfm)
{
    // Message to send
    struct mesh_peer_info_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MESH_PEER_INFO_REQ message */
    req = siwifi_msg_zalloc(MESH_PEER_INFO_REQ, TASK_MESH, DRV_TASK_ID,
                          sizeof(struct mesh_peer_info_req));
    if (!req) {
        return -ENOMEM;
    }

    req->sta_idx = sta_idx;

    /* Send the MESH_PEER_INFO_REQ message to UMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, MESH_PEER_INFO_CFM, cfm);
}

void siwifi_send_mesh_peer_update_ntf(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif,
                                    u8 sta_idx, u8 mlink_state)
{
    // Message to send
    struct mesh_peer_update_ntf *p_ntf;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MESH_PEER_UPDATE_NTF message */
    p_ntf = siwifi_msg_zalloc(MESH_PEER_UPDATE_NTF, TASK_MESH, DRV_TASK_ID,
                            sizeof(struct mesh_peer_update_ntf));

    if (p_ntf) {
        p_ntf->vif_idx = vif->vif_index;
        p_ntf->sta_idx = sta_idx;
        p_ntf->state = mlink_state;

        /* Send the MESH_PEER_INFO_REQ message to UMAC FW */
        siwifi_send_msg(siwifi_hw, p_ntf, 0, 0, NULL);
    }
}

void siwifi_send_mesh_path_create_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif, u8 *p_tgt_addr)
{
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Check if we are already waiting for a confirmation */
    if (!vif->ap.create_path) {
        // Message to send
        struct mesh_path_create_req *req;

        /* Build the MESH_PATH_CREATE_REQ message */
        req = siwifi_msg_zalloc(MESH_PATH_CREATE_REQ, TASK_MESH, DRV_TASK_ID,
                              sizeof(struct mesh_path_create_req));

        if (req) {
            req->vif_idx = vif->vif_index;
            memcpy(&req->tgt_mac_addr, p_tgt_addr, ETH_ALEN);

            vif->ap.create_path = true;

            /* Send the MESH_PATH_CREATE_REQ message to UMAC FW */
            siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
        }
    }
}

int siwifi_send_mesh_path_update_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif, const u8 *p_tgt_addr,
                                   const u8 *p_nhop_addr, struct mesh_path_update_cfm *p_cfm)
{
    // Message to send
    struct mesh_path_update_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MESH_PATH_UPDATE_REQ message */
    req = siwifi_msg_zalloc(MESH_PATH_UPDATE_REQ, TASK_MESH, DRV_TASK_ID,
                          sizeof(struct mesh_path_update_req));
    if (!req) {
        return -ENOMEM;
    }

    req->delete = (p_nhop_addr == NULL);
    req->vif_idx = vif->vif_index;
    memcpy(&req->tgt_mac_addr, p_tgt_addr, ETH_ALEN);

    if (p_nhop_addr) {
        memcpy(&req->nhop_mac_addr, p_nhop_addr, ETH_ALEN);
    }

    /* Send the MESH_PATH_UPDATE_REQ message to UMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, MESH_PATH_UPDATE_CFM, p_cfm);
}

void siwifi_send_mesh_proxy_add_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif, u8 *p_ext_addr)
{
    // Message to send
    struct mesh_proxy_add_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MESH_PROXY_ADD_REQ message */
    req = siwifi_msg_zalloc(MESH_PROXY_ADD_REQ, TASK_MESH, DRV_TASK_ID,
                          sizeof(struct mesh_proxy_add_req));

    if (req) {
        req->vif_idx = vif->vif_index;
        memcpy(&req->ext_sta_addr, p_ext_addr, ETH_ALEN);

        /* Send the MESH_PROXY_ADD_REQ message to UMAC FW */
        siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
    }
}

int siwifi_send_tdls_peer_traffic_ind_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif)
{
    struct tdls_peer_traffic_ind_req *tdls_peer_traffic_ind_req;

    if (!siwifi_vif->sta.tdls_sta)
        return -ENOLINK;

    /* Build the TDLS_PEER_TRAFFIC_IND_REQ message */
    tdls_peer_traffic_ind_req = siwifi_msg_zalloc(TDLS_PEER_TRAFFIC_IND_REQ, TASK_TDLS, DRV_TASK_ID,
                                           sizeof(struct tdls_peer_traffic_ind_req));

    if (!tdls_peer_traffic_ind_req)
        return -ENOMEM;

    /* Set parameters for the TDLS_PEER_TRAFFIC_IND_REQ message */
    tdls_peer_traffic_ind_req->vif_index = siwifi_vif->vif_index;
    tdls_peer_traffic_ind_req->sta_idx = siwifi_vif->sta.tdls_sta->sta_idx;
    memcpy(&(tdls_peer_traffic_ind_req->peer_mac_addr.array[0]),
           siwifi_vif->sta.tdls_sta->mac_addr, ETH_ALEN);
    tdls_peer_traffic_ind_req->dialog_token = 0; // check dialog token value
    tdls_peer_traffic_ind_req->last_tid = siwifi_vif->sta.tdls_sta->tdls.last_tid;
    tdls_peer_traffic_ind_req->last_sn = siwifi_vif->sta.tdls_sta->tdls.last_sn;

    /* Send the TDLS_PEER_TRAFFIC_IND_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, tdls_peer_traffic_ind_req, 0, 0, NULL);
}

int siwifi_send_config_monitor_req(struct siwifi_hw *siwifi_hw,
                                 struct cfg80211_chan_def *chandef,
                                 struct me_config_monitor_cfm *cfm)
{
    struct me_config_monitor_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the ME_CONFIG_MONITOR_REQ message */
    req = siwifi_msg_zalloc(ME_CONFIG_MONITOR_REQ, TASK_ME, DRV_TASK_ID,
                                   sizeof(struct me_config_monitor_req));
    if (!req)
        return -ENOMEM;

    if (chandef) {
        /* Set parameters for the ME_CONFIG_MONITOR_REQ message */
        req->band = chandef->chan->band;
#if DEBUG_ARRAY_CHECK
        BUG_ON(chandef->width > NL80211_CHAN_WIDTH_80P80);
#endif
        req->type = bw2chnl[chandef->width];
        req->prim20_freq = chandef->chan->center_freq;
        req->center1_freq = chandef->center_freq1;
        req->center2_freq = chandef->center_freq2;
        req->tx_power = chandef->chan->max_power;
        if (siwifi_hw->use_phy_bw_tweaks) {
            /* XXX Tweak for 80MHz VHT */
            if (chandef->width > NL80211_CHAN_WIDTH_40) {
                int _offs = req->center1_freq - req->prim20_freq;
                req->type = PHY_CHNL_BW_40;
                req->center1_freq = req->prim20_freq + 10 *
                    (_offs > 0 ? 1 : -1) * (abs(_offs) > 10 ? 1 : -1);
                SIWIFI_DBG("Tweak for 80MHz VHT: 80MHz chan requested\n");
            }
        }
    }

    /* Send the ME_CONFIG_MONITOR_REQ message to FW */
    return siwifi_send_msg(siwifi_hw, req, 1, ME_CONFIG_MONITOR_CFM, cfm);
}

int siwifi_send_tdls_chan_switch_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif,
                                   struct siwifi_sta *siwifi_sta, bool sta_initiator,
                                   u8 oper_class, struct cfg80211_chan_def *chandef,
                                   struct tdls_chan_switch_cfm *cfm)
{
    struct tdls_chan_switch_req *tdls_chan_switch_req;

    /* Build the TDLS_CHAN_SWITCH_REQ message */
    tdls_chan_switch_req = siwifi_msg_zalloc(TDLS_CHAN_SWITCH_REQ, TASK_TDLS, DRV_TASK_ID,
                                           sizeof(struct tdls_chan_switch_req));

    if (!tdls_chan_switch_req)
        return -ENOMEM;

    /* Set parameters for the TDLS_CHAN_SWITCH_REQ message */
    tdls_chan_switch_req->vif_index = siwifi_vif->vif_index;
    tdls_chan_switch_req->sta_idx = siwifi_sta->sta_idx;
    memcpy(&(tdls_chan_switch_req->peer_mac_addr.array[0]),
           siwifi_sta_addr(siwifi_sta), ETH_ALEN);
    tdls_chan_switch_req->initiator = sta_initiator;
    tdls_chan_switch_req->band = chandef->chan->band;
#if DEBUG_ARRAY_CHECK
    BUG_ON(chandef->width > NL80211_CHAN_WIDTH_80P80);
#endif
    tdls_chan_switch_req->type = bw2chnl[chandef->width];
    tdls_chan_switch_req->prim20_freq = chandef->chan->center_freq;
    tdls_chan_switch_req->center1_freq = chandef->center_freq1;
    tdls_chan_switch_req->center2_freq = chandef->center_freq2;
    tdls_chan_switch_req->tx_power = chandef->chan->max_power;
    tdls_chan_switch_req->op_class = oper_class;

    /* Send the TDLS_CHAN_SWITCH_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, tdls_chan_switch_req, 1, TDLS_CHAN_SWITCH_CFM, cfm);
}

int siwifi_send_tdls_cancel_chan_switch_req(struct siwifi_hw *siwifi_hw,
                                          struct siwifi_vif *siwifi_vif,
                                          struct siwifi_sta *siwifi_sta,
                                          struct tdls_cancel_chan_switch_cfm *cfm)
{
    struct tdls_cancel_chan_switch_req *tdls_cancel_chan_switch_req;

    /* Build the TDLS_CHAN_SWITCH_REQ message */
    tdls_cancel_chan_switch_req = siwifi_msg_zalloc(TDLS_CANCEL_CHAN_SWITCH_REQ, TASK_TDLS, DRV_TASK_ID,
                                           sizeof(struct tdls_cancel_chan_switch_req));
    if (!tdls_cancel_chan_switch_req)
        return -ENOMEM;

    /* Set parameters for the TDLS_CHAN_SWITCH_REQ message */
    tdls_cancel_chan_switch_req->vif_index = siwifi_vif->vif_index;
    tdls_cancel_chan_switch_req->sta_idx = siwifi_sta->sta_idx;
    memcpy(&(tdls_cancel_chan_switch_req->peer_mac_addr.array[0]),
           siwifi_sta_addr(siwifi_sta), ETH_ALEN);

    /* Send the TDLS_CHAN_SWITCH_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, tdls_cancel_chan_switch_req, 1, TDLS_CANCEL_CHAN_SWITCH_CFM, cfm);
}

#ifdef CONFIG_SIWIFI_BFMER
void siwifi_send_bfmer_enable(struct siwifi_hw *siwifi_hw, struct siwifi_sta *siwifi_sta,
                            const struct ieee80211_vht_cap *vht_cap)
{
    struct mm_bfmer_enable_req *bfmer_en_req;
    __le32 vht_capability;
    u8 rx_nss = 0;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    if (!vht_cap) {
        goto end;
    }

    vht_capability = vht_cap->vht_cap_info;

    if (!(vht_capability & IEEE80211_VHT_CAP_SU_BEAMFORMEE_CAPABLE)) {
        goto end;
    }

    rx_nss = siwifi_bfmer_get_rx_nss(vht_cap);

    /* Allocate a structure that will contain the beamforming report */
    if (siwifi_bfmer_report_add(siwifi_hw, siwifi_sta, SIWIFI_BFMER_REPORT_SPACE_SIZE))
    {
        goto end;
    }

    /* Build the MM_BFMER_ENABLE_REQ message */
    bfmer_en_req = siwifi_msg_zalloc(MM_BFMER_ENABLE_REQ, TASK_MM, DRV_TASK_ID,
                                   sizeof(struct mm_bfmer_enable_req));

    /* Check message allocation */
    if (!bfmer_en_req) {
        /* Free memory allocated for the report */
        siwifi_bfmer_report_del(siwifi_hw, siwifi_sta);

        /* Do not use beamforming */
        goto end;
    }

    /* Provide DMA address to the MAC */
    bfmer_en_req->host_bfr_addr = siwifi_sta->bfm_report->dma_addr;
    bfmer_en_req->host_bfr_size = SIWIFI_BFMER_REPORT_SPACE_SIZE;
    bfmer_en_req->sta_idx = siwifi_sta->sta_idx;
    bfmer_en_req->aid = siwifi_sta->aid;
    bfmer_en_req->rx_nss = rx_nss;

    if (vht_capability & IEEE80211_VHT_CAP_MU_BEAMFORMEE_CAPABLE) {
        bfmer_en_req->vht_mu_bfmee = true;
    } else {
        bfmer_en_req->vht_mu_bfmee = false;
    }

    /* Send the MM_BFMER_EN_REQ message to LMAC FW */
    siwifi_send_msg(siwifi_hw, bfmer_en_req, 0, 0, NULL);

end:
    return;
}

#ifdef CONFIG_SIWIFI_MUMIMO_TX
int siwifi_send_mu_group_update_req(struct siwifi_hw *siwifi_hw, struct siwifi_sta *siwifi_sta)
{
    struct mm_mu_group_update_req *req;
    int group_id, i = 0;
    u64 map;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MM_MU_GROUP_UPDATE_REQ message */
    req = siwifi_msg_zalloc(MM_MU_GROUP_UPDATE_REQ, TASK_MM, DRV_TASK_ID,
                          sizeof(struct mm_mu_group_update_req) +
                          siwifi_sta->group_info.cnt * sizeof(req->groups[0]));

    /* Check message allocation */
    if (!req)
        return -ENOMEM;

    /* Go through the groups the STA belongs to */
    group_sta_for_each(siwifi_sta, group_id, map) {
        int user_pos = siwifi_mu_group_sta_get_pos(siwifi_hw, siwifi_sta, group_id);

        if (WARN((i >= siwifi_sta->group_info.cnt),
                 "STA%d: Too much group (%d)\n",
                 siwifi_sta->sta_idx, i + 1))
            break;

        req->groups[i].group_id = group_id;
        req->groups[i].user_pos = user_pos;

        i++;
    }

    req->group_cnt = siwifi_sta->group_info.cnt;
    req->sta_idx = siwifi_sta->sta_idx;

    /* Send the MM_MU_GROUP_UPDATE_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, MM_MU_GROUP_UPDATE_CFM, NULL);
}
#endif /* CONFIG_SIWIFI_MUMIMO_TX */
#endif /* CONFIG_SIWIFI_BFMER */

/**********************************************************************
 *    Debug Messages
 *********************************************************************/
int siwifi_send_dbg_trigger_req(struct siwifi_hw *siwifi_hw, char *msg, uint8_t type)
{
    struct mm_dbg_trigger_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MM_DBG_TRIGGER_REQ message */
    req = siwifi_msg_zalloc(MM_DBG_TRIGGER_REQ, TASK_MM, DRV_TASK_ID,
                          sizeof(struct mm_dbg_trigger_req));
    if (!req)
        return -ENOMEM;

    req->type = type;
    /* Set parameters for the MM_DBG_TRIGGER_REQ message */
    strncpy(req->error, msg, sizeof(req->error));

    /* Send the MM_DBG_TRIGGER_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 0, -1, NULL);
}

int siwifi_send_dbg_set_cca_parameter_req(struct siwifi_hw *siwifi_hw, uint32_t *param)
{
    struct dbg_set_cca_parameter_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the DBG_SET_CCA_PARAMETER_REQ message */
    req = siwifi_msg_zalloc(DBG_SET_CCA_PARAMETER_REQ, TASK_DBG, DRV_TASK_ID,
                          sizeof(struct dbg_set_cca_parameter_req));
    if (!req)
        return -ENOMEM;

    req->set_cca_parameter_enable = param[0];
    req->bk_timeout = param[1];
    req->be_timeout = param[2];
    req->vi_timeout = param[3];
    req->vo_timeout = param[4];
    req->bcn_timeout = param[5];
    req->cca_threshold_fall = param[6];
    req->cca_threshold_rise = param[7];
    req->trigger_interval_low = param[8];
    req->trigger_interval_high = param[9];
    req->step_max = param[10];
    req->step_distance = param[11];

    /* Send DBG_SET_CCA_PARAMETER_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 0, -1, NULL);
}

int siwifi_send_dbg_mem_read_req(struct siwifi_hw *siwifi_hw, u32 mem_addr,
                               struct dbg_mem_read_cfm *cfm)
{
    struct dbg_mem_read_req *mem_read_req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the DBG_MEM_READ_REQ message */
    mem_read_req = siwifi_msg_zalloc(DBG_MEM_READ_REQ, TASK_DBG, DRV_TASK_ID,
                                   sizeof(struct dbg_mem_read_req));
    if (!mem_read_req)
        return -ENOMEM;

    /* Set parameters for the DBG_MEM_READ_REQ message */
    mem_read_req->memaddr = mem_addr;

    /* Send the DBG_MEM_READ_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, mem_read_req, 1, DBG_MEM_READ_CFM, cfm);
}

int siwifi_send_dbg_mem_write_req(struct siwifi_hw *siwifi_hw, u32 mem_addr,
                                u32 mem_data)
{
    struct dbg_mem_write_req *mem_write_req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the DBG_MEM_WRITE_REQ message */
    mem_write_req = siwifi_msg_zalloc(DBG_MEM_WRITE_REQ, TASK_DBG, DRV_TASK_ID,
                                    sizeof(struct dbg_mem_write_req));
    if (!mem_write_req)
        return -ENOMEM;

    /* Set parameters for the DBG_MEM_WRITE_REQ message */
    mem_write_req->memaddr = mem_addr;
    mem_write_req->memdata = mem_data;

    /* Send the DBG_MEM_WRITE_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, mem_write_req, 1, DBG_MEM_WRITE_CFM, NULL);
}

int siwifi_send_dbg_set_mod_filter_req(struct siwifi_hw *siwifi_hw, u32 filter)
{
    struct dbg_set_mod_filter_req *set_mod_filter_req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the DBG_SET_MOD_FILTER_REQ message */
    set_mod_filter_req =
        siwifi_msg_zalloc(DBG_SET_MOD_FILTER_REQ, TASK_DBG, DRV_TASK_ID,
                        sizeof(struct dbg_set_mod_filter_req));
    if (!set_mod_filter_req)
        return -ENOMEM;

    /* Set parameters for the DBG_SET_MOD_FILTER_REQ message */
    set_mod_filter_req->mod_filter = filter;

    /* Send the DBG_SET_MOD_FILTER_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, set_mod_filter_req, 1, DBG_SET_MOD_FILTER_CFM, NULL);
}

int siwifi_send_dbg_set_sev_filter_req(struct siwifi_hw *siwifi_hw, u32 filter)
{
    struct dbg_set_sev_filter_req *set_sev_filter_req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the DBG_SET_SEV_FILTER_REQ message */
    set_sev_filter_req =
        siwifi_msg_zalloc(DBG_SET_SEV_FILTER_REQ, TASK_DBG, DRV_TASK_ID,
                        sizeof(struct dbg_set_sev_filter_req));
    if (!set_sev_filter_req)
        return -ENOMEM;

    /* Set parameters for the DBG_SET_SEV_FILTER_REQ message */
    set_sev_filter_req->sev_filter = filter;

    /* Send the DBG_SET_SEV_FILTER_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, set_sev_filter_req, 1, DBG_SET_SEV_FILTER_CFM, NULL);
}

int siwifi_send_dbg_get_sys_stat_req(struct siwifi_hw *siwifi_hw,
                                   struct dbg_get_sys_stat_cfm *cfm)
{
    void *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Allocate the message */
    req = siwifi_msg_zalloc(DBG_GET_SYS_STAT_REQ, TASK_DBG, DRV_TASK_ID, 0);
    if (!req)
        return -ENOMEM;

    /* Send the DBG_MEM_READ_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 1, DBG_GET_SYS_STAT_CFM, cfm);
}

#ifdef CONFIG_SIWIFI_TRX_STAT
int siwifi_send_dbg_get_tx_stat_req(struct siwifi_hw *siwifi_hw,
                                   struct dbg_get_tx_stat_cfm *cfm, uint32_t clear)
{
    struct dbg_get_tx_stat_req *req;
    int ret;
    u32_l req_cfm_dma_addr;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Allocate the message */
    req = siwifi_msg_zalloc(DBG_GET_TX_STAT_REQ, TASK_DBG, DRV_TASK_ID, sizeof(struct dbg_get_tx_stat_req));
    if (!req)
        return -ENOMEM;
    req->clear = clear;
    if (!clear) {
        // alloc cfm dma buffer size to get cfm memory
        req->cfm_dma_addr = siwifi_hw->dbg_trx_stat_elem.dma_addr;
    } else {
        req->cfm_dma_addr = 0;
    }
    // RM#13270 req will be freed after siwifi_send_msg
    req_cfm_dma_addr = req->cfm_dma_addr;

    /* Send the DBG_MEM_READ_REQ message to LMAC FW */
    ret = siwifi_send_msg(siwifi_hw, req, 1, DBG_GET_TX_STAT_CFM, cfm);
    // check the dma address
    WARN_ON(req_cfm_dma_addr != cfm->dma_addr);
    dma_sync_single_for_cpu(siwifi_hw->dev, req_cfm_dma_addr,
                        sizeof(struct dbg_tx_stat), DMA_FROM_DEVICE);
    return ret;
}


int siwifi_send_dbg_get_rx_stat_req(struct siwifi_hw *siwifi_hw,
                                   struct dbg_get_rx_stat_cfm *cfm, uint32_t clear)
{
    struct dbg_get_rx_stat_req *req;
    int ret;
    u32_l req_cfm_dma_addr;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Allocate the message */
    req = siwifi_msg_zalloc(DBG_GET_RX_STAT_REQ, TASK_DBG, DRV_TASK_ID, sizeof(struct dbg_get_rx_stat_req));
    if (!req)
        return -ENOMEM;
    req->clear = clear;
    if (!clear) {
        // alloc cfm dma buffer size to get cfm memory
        req->cfm_dma_addr = siwifi_hw->dbg_trx_stat_elem.dma_addr;
    } else {
        req->cfm_dma_addr = 0;
    }
    // RM#13270 req will be freed after siwifi_send_msg
    req_cfm_dma_addr = req->cfm_dma_addr;

    /* Send the DBG_MEM_READ_REQ message to LMAC FW */
    ret = siwifi_send_msg(siwifi_hw, req, 1, DBG_GET_RX_STAT_CFM, cfm);
    // check the dma address
    WARN_ON(req_cfm_dma_addr != cfm->dma_addr);
    dma_sync_single_for_cpu(siwifi_hw->dev, req_cfm_dma_addr,
            sizeof(struct dbg_rx_stat), DMA_FROM_DEVICE);
    return ret;
}
#endif

#ifdef NEW_SCHEDULE
int siwifi_send_dbg_print_burst_info(struct siwifi_hw *siwifi_hw, int ac)
{
    struct dbg_print_burst_info_cfm cfm;
    struct dbg_print_burst_info_req *req;

    req = siwifi_msg_zalloc(DBG_PRINT_TX_BURST_INFO_REQ, TASK_DBG, DRV_TASK_ID, sizeof(struct dbg_get_rx_stat_req));
    if (!req)
        return -ENOMEM;
    req->ac = ac;

    return siwifi_send_msg(siwifi_hw, req, 1, DBG_PRINT_TX_BURST_INFO_CFM, &cfm);
}
#endif

int siwifi_send_cfg_rssi_req(struct siwifi_hw *siwifi_hw, u8 vif_index, int rssi_thold, u32 rssi_hyst)
{
    struct mm_cfg_rssi_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Build the MM_CFG_RSSI_REQ message */
    req = siwifi_msg_zalloc(MM_CFG_RSSI_REQ, TASK_MM, DRV_TASK_ID,
                          sizeof(struct mm_cfg_rssi_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the MM_CFG_RSSI_REQ message */
    req->vif_index = vif_index;
    req->rssi_thold = (s8)rssi_thold;
    req->rssi_hyst = (u8)rssi_hyst;

    /* Send the MM_CFG_RSSI_REQ message to LMAC FW */
    return siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
}

#if defined CONFIG_VDR_HW
int siwifi_send_dbg_get_vendor_info_req(struct siwifi_hw *siwifi_hw,
        struct dbg_get_vendor_info_cfm *cfm, uint32_t clear, uint8_t vif_idx, uint8_t sta_idx)
{
    struct dbg_get_vendor_info_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Allocate the message */
    req = siwifi_msg_zalloc(DBG_GET_VDR_INFO_REQ, TASK_DBG, DRV_TASK_ID, sizeof(struct dbg_get_vendor_info_req));
    if (!req)
        return -ENOMEM;
    req->clear = clear;
    req->vif_idx = vif_idx;
    req->sta_idx = sta_idx;

    return siwifi_send_msg(siwifi_hw, req, 1, DBG_GET_VDR_INFO_CFM, cfm);
}
#endif

int siwifi_send_dbg_get_vendor_mp_info_req(struct siwifi_hw *siwifi_hw,
        struct dbg_get_vendor_mp_info_cfm *cfm)
{
    struct dbg_get_vendor_mp_info_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Allocate the message */
    req = siwifi_msg_zalloc(DBG_VDR_GET_MP_INFO_REQ, TASK_DBG, DRV_TASK_ID, sizeof(struct dbg_get_vendor_mp_info_req));
    if (!req)
        return -ENOMEM;
    req->params = 0;

    return siwifi_send_msg(siwifi_hw, req, 1, DBG_VDR_GET_MP_INFO_CFM, cfm);
}

int siwifi_send_apm_deauth_req(struct siwifi_hw *siwifi_hw,
        uint8_t vif_idx, const u8 *mac, uint16_t reason_code)
{
    struct apm_send_deauth_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Allocate the message */
    req = siwifi_msg_zalloc(APM_SEND_DEAUTH_REQ, TASK_APM, DRV_TASK_ID, sizeof(struct apm_send_deauth_req));
    if (!req)
        return -ENOMEM;

    req->vif_idx = vif_idx;
    memcpy(&req->macaddr, mac, ETH_ALEN);
    req->reason_code = reason_code;

    return siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
}

int siwifi_send_apm_deassoc_req(struct siwifi_hw *siwifi_hw,
        uint8_t vif_idx, const u8 *mac, uint16_t reason_code)
{
    struct apm_send_deassoc_req *req;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Allocate the message */
    req = siwifi_msg_zalloc(APM_SEND_DEASSOC_REQ, TASK_APM, DRV_TASK_ID, sizeof(struct apm_send_deassoc_req));
    if (!req)
        return -ENOMEM;

    req->vif_idx = vif_idx;
    memcpy(&req->macaddr, mac, ETH_ALEN);
    req->reason_code = reason_code;

    return siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
}

int siwifi_send_get_channel_info_req(struct siwifi_hw *siwifi_hw,
    struct mm_get_channel_info_cfm *cfm)
{
    struct mm_get_channel_info_req *req;
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    req = siwifi_msg_zalloc(MM_GET_CHANNEL_INFO_REQ, TASK_MM, DRV_TASK_ID, sizeof(struct mm_get_channel_info_req));
    if (!req)
        return -ENOMEM;
    req->params = 0;
    return siwifi_send_msg(siwifi_hw, req, 1, MM_GET_CHANNEL_INFO_CFM, cfm);
}

int siwifi_send_probe_client(struct siwifi_hw *siwifi_hw, u8 sta_idx)
{
    struct mm_send_null_frame_req *req;
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    req = siwifi_msg_zalloc(MM_SEND_NULL_FRAME_REQ, TASK_MM, DRV_TASK_ID,
                            sizeof(struct mm_send_null_frame_req));
    if(!req)
        return -ENOMEM;
    req->sta_idx = sta_idx;
    return siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
}

int siwifi_send_dbg_get_mgmt_info_req(struct siwifi_hw *siwifi_hw,
                                   struct dbg_get_mgmt_info_cfm *cfm, uint32_t clear)
{
    struct dbg_get_mgmt_info_req *req;
    int ret;
    u32_l req_cfm_dma_addr;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Allocate the message */
    req = siwifi_msg_zalloc(DBG_GET_MGMT_INFO_REQ, TASK_DBG, DRV_TASK_ID, sizeof(struct dbg_get_mgmt_info_req));

    if (!req)
        return -ENOMEM;
    req->clear = clear;
    if (!clear) {
        // alloc cfm dma buffer size to get cfm memory
        req->cfm_dma_addr = siwifi_hw->dbg_mgmt_info_elem.dma_addr;
    } else {
        req->cfm_dma_addr = 0;
    }
    // RM#13270 req will be freed after siwifi_send_msg
    req_cfm_dma_addr = req->cfm_dma_addr;

    ret = siwifi_send_msg(siwifi_hw, req, 1, DBG_GET_MGMT_INFO_CFM, cfm);
    WARN_ON(req_cfm_dma_addr != cfm->dma_addr);
    dma_sync_single_for_cpu(siwifi_hw->dev, req_cfm_dma_addr,
                        sizeof(struct dbg_mgmt_info), DMA_FROM_DEVICE);
    return ret;
}

int siwifi_send_dbg_get_ctrl_info_req(struct siwifi_hw *siwifi_hw,
                                   struct dbg_get_ctrl_info_cfm *cfm, uint32_t clear)
{
    struct dbg_get_ctrl_info_req *req;
    int ret;
    u32_l req_cfm_dma_addr;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Allocate the message */
    req = siwifi_msg_zalloc(DBG_GET_CTRL_INFO_REQ, TASK_DBG, DRV_TASK_ID, sizeof(struct dbg_get_ctrl_info_req));

    if (!req)
        return -ENOMEM;
    req->clear = clear;
    if (!clear) {
        // alloc cfm dma buffer size to get cfm memory
        req->cfm_dma_addr = siwifi_hw->dbg_ctrl_info_elem.dma_addr;
    } else {
        req->cfm_dma_addr = 0;
    }
    // RM#13270 req will be freed after siwifi_send_msg
    req_cfm_dma_addr = req->cfm_dma_addr;

    ret = siwifi_send_msg(siwifi_hw, req, 1, DBG_GET_CTRL_INFO_CFM, cfm);
    WARN_ON(req_cfm_dma_addr != cfm->dma_addr);
    dma_sync_single_for_cpu(siwifi_hw->dev, req_cfm_dma_addr,
                        sizeof(struct dbg_ctrl_info), DMA_FROM_DEVICE);
    return ret;
}

int siwifi_send_debug_frame(struct siwifi_hw *siwifi_hw, struct mm_send_debug_frame_req *params, u8 *payload)
{
    struct mm_send_debug_frame_req *req;
    int ret;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    if (siwifi_hw->debug_frame_buf.addr != NULL)
        return -ENOMEM;
    req = siwifi_msg_zalloc(MM_SEND_DEBUG_FRAME_REQ, TASK_MM, DRV_TASK_ID, sizeof(struct mm_send_debug_frame_req));

    if (!req)
        return -ENOMEM;
    if (payload) {
        if ((ret = siwifi_ipc_elem_var_allocs(siwifi_hw, &siwifi_hw->debug_frame_buf, params->frmlen,
            DMA_TO_DEVICE, NULL, payload, NULL, NULL)))
        {
            dev_err(siwifi_hw->dev, "Failed to allocate IPC buffer for send debug frames\n");
            printk("Error Code: %d", ret);
            if (req != NULL)
                siwifi_msg_free(siwifi_hw, req);
            return -ENOMEM;
        }
        memcpy(req, params, sizeof(struct mm_send_debug_frame_req));
        req->payload_addr = siwifi_hw->debug_frame_buf.dma_addr;
    }
    else
    {
        siwifi_msg_free(siwifi_hw, req);
        return -EINVAL;
    }
    return siwifi_send_msg(siwifi_hw, req, 0, 0, NULL);
}
