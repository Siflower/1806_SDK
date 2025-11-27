/**
 ****************************************************************************************
 *
 * @file siwifi_msg_tx.h
 *
 * @brief TX function declarations
 *
 * Copyright (C) Siflower 2018-2025
 *
 ****************************************************************************************
 */

#ifndef _SIWIFI_MSG_TX_H_
#define _SIWIFI_MSG_TX_H_

#include "siwifi_defs.h"

/*
 * c.f LMAC/src/co/mac/mac_frame.h
 */
#define MAC_RSNIE_CIPHER_WEP40    0x00
#define MAC_RSNIE_CIPHER_TKIP     0x01
#define MAC_RSNIE_CIPHER_CCMP     0x02
#define MAC_RSNIE_CIPHER_WEP104   0x03
#define MAC_RSNIE_CIPHER_SMS4     0x04
#define MAC_RSNIE_CIPHER_AES_CMAC 0x05

enum siwifi_chan_types {
    PHY_CHNL_BW_20,
    PHY_CHNL_BW_40,
    PHY_CHNL_BW_80,
    PHY_CHNL_BW_160,
    PHY_CHNL_BW_80P80,
    PHY_CHNL_BW_OTHER,
};

enum siwifi_phy_txpower_lvl {
	SIWIFI_TXPOWER_LVL_SLEEPMODE = 0,
	SIWIFI_TXPOWER_LVL_LOW = 1,
    SIWIFI_TXPOWER_LVL_STANDARD = 2,
    SIWIFI_TXPOWER_LVL_HIGH = 3
};

extern const int chnl2bw[];

int siwifi_send_reset(struct siwifi_hw *siwifi_hw);
int siwifi_send_start(struct siwifi_hw *siwifi_hw);
int siwifi_send_version_req(struct siwifi_hw *siwifi_hw, struct mm_version_cfm *cfm);
int siwifi_send_add_if(struct siwifi_hw *siwifi_hw, const unsigned char *mac,
                     enum nl80211_iftype iftype, bool p2p, bool use_4addr, struct mm_add_if_cfm *cfm);
#ifdef CONFIG_SIWIFI_CH_OFFSET
int siwifi_send_set_channel_offset(struct siwifi_hw *siwifi_hw, int phy_idx,
                          struct mm_set_channel_cfm *cfm, int ch_offset);
#endif
int siwifi_send_remove_if(struct siwifi_hw *siwifi_hw, u8 vif_index);
int siwifi_send_set_channel(struct siwifi_hw *siwifi_hw, int phy_idx,
                          struct mm_set_channel_cfm *cfm);
int siwifi_send_set_channel_for_macbypass_tx(struct siwifi_hw *siwifi_hw, int phy_idx,struct mm_set_channel_cfm *cfm,struct cfg80211_chan_def *chandef);
int siwifi_send_key_add(struct siwifi_hw *siwifi_hw, u8 vif_idx, u8 sta_idx, bool pairwise,
                      u8 *key, u8 key_len, u8 key_idx, u8 cipher_suite,
                      struct mm_key_add_cfm *cfm);
int siwifi_send_key_del(struct siwifi_hw *siwifi_hw, uint8_t hw_key_idx);
int siwifi_send_default_key_add(struct siwifi_hw *siwifi_hw, u8 vif_idx, u8 default_key_idx, struct mm_default_key_add_cfm *cfm);
int siwifi_send_bcn_change(struct siwifi_hw *siwifi_hw, u8 vif_idx, u32 bcn_addr,
                         u16 bcn_len, u16 tim_oft, u16 tim_len, u16 *csa_oft);
int siwifi_send_tim_update(struct siwifi_hw *siwifi_hw, u8 vif_idx, u16 aid,
                         u8 tx_status);
int siwifi_send_roc(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif,
                  struct ieee80211_channel *chan, unsigned int duration);
int siwifi_send_cancel_roc(struct siwifi_hw *siwifi_hw);
int siwifi_send_set_power(struct siwifi_hw *siwifi_hw,  u8 vif_idx, s8 pwr,
                        struct mm_set_power_cfm *cfm);
int siwifi_send_set_power_lvl(struct siwifi_hw *siwifi_hw, u8 power_lvl);
int siwifi_send_set_fixed_gain(struct siwifi_hw *siwifi_hw, int fixed_gain, int temp_ctrl_enable);
int siwifi_send_set_antenna_number(struct siwifi_hw *siwifi_hw, u8 antenna_number);
int siwifi_send_set_edca(struct siwifi_hw *siwifi_hw, u8 hw_queue, u32 param,
                       bool uapsd, u8 inst_nbr);
int siwifi_send_tdls_chan_switch_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif,
                                   struct siwifi_sta *siwifi_sta, bool sta_initiator,
                                   u8 oper_class, struct cfg80211_chan_def *chandef,
                                   struct tdls_chan_switch_cfm *cfm);
int siwifi_send_tdls_cancel_chan_switch_req(struct siwifi_hw *siwifi_hw,
                                          struct siwifi_vif *siwifi_vif,
                                          struct siwifi_sta *siwifi_sta,
                                          struct tdls_cancel_chan_switch_cfm *cfm);

#ifdef CONFIG_SIWIFI_P2P_DEBUGFS
int siwifi_send_p2p_oppps_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif,
                            u8 ctw, struct mm_set_p2p_oppps_cfm *cfm);
int siwifi_send_p2p_noa_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif,
                          int count, int interval, int duration,
                          bool dyn_noa, struct mm_set_p2p_noa_cfm *cfm);
#endif /* CONFIG_SIWIFI_P2P_DEBUGFS */

int siwifi_send_read_cde_info(struct siwifi_hw *siwifi_hw);

#ifdef CONFIG_SIWIFI_RF_RECALI
int siwifi_send_rf_ctrl(struct siwifi_hw *siwifi_hw, uint8_t ctrl);
int siwifi_send_rf_calibrate(struct siwifi_hw *siwifi_hw, uint16_t conf);
#endif

#ifdef CONFIG_SIWIFI_RF_CMD_DUMP
int siwifi_send_rf_cmd_dump(struct siwifi_hw *siwifi_hw, struct mm_rf_cmd_dump_cfm *cfm,
                uint32_t address, uint32_t size, uint8_t flag);
#endif
#ifdef CONFIG_SIWIFI_TX_POWER_CALI
int siwifi_send_txpower_cali_enable_req(struct siwifi_hw *siwifi_hw);
#endif
int siwifi_send_rf_get_temperature(struct siwifi_hw *siwifi_hw, struct mm_rf_get_temp_cfm *cfm);
#ifdef CONFIG_SIWIFI_COOLING_TEMP
int siwifi_send_change_power_req(struct siwifi_hw *siwifi_hw,int change_power,int change_power_trend);
int siwifi_send_cooling_temp_req(struct siwifi_hw *siwifi_hw);
#endif
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
int siwifi_send_me_ate_tools_op(struct siwifi_hw *siwifi_hw, struct cfg80211_chan_def *def,
                struct me_ate_tools_op_cfm *cfm, uint8_t flag);
#endif
int siwifi_send_me_config_req(struct siwifi_hw *siwifi_hw);
int siwifi_send_me_chan_config_req(struct siwifi_hw *siwifi_hw);
int siwifi_send_me_set_control_port_req(struct siwifi_hw *siwifi_hw, bool opened,
                                      u8 sta_idx);
int siwifi_send_me_sta_add(struct siwifi_hw *siwifi_hw, struct station_parameters *params,
                         const u8 *mac, u8 inst_nbr, struct me_sta_add_cfm *cfm);
int siwifi_send_me_sta_del(struct siwifi_hw *siwifi_hw, u8 sta_idx, bool tdls_sta);
int siwifi_send_me_traffic_ind(struct siwifi_hw *siwifi_hw, u8 sta_idx, bool uapsd, u8 tx_status);
int siwifi_send_me_rc_stats(struct siwifi_hw *siwifi_hw, u8 sta_idx,
                          struct me_rc_stats_cfm *cfm);
int siwifi_send_me_rc_set_rate(struct siwifi_hw *siwifi_hw,
                             u8 sta_idx,
                             u16 rate_idx);
int siwifi_send_me_rc_set_no_ss(struct siwifi_hw *siwifi_hw, u8 no_ss);
int siwifi_send_assoc_insert_info(struct siwifi_hw *siwifi_hw);
int siwifi_send_auth_insert_info(struct siwifi_hw *siwifi_hw);
int siwifi_send_sm_connect_req(struct siwifi_hw *siwifi_hw,
                             struct siwifi_vif *siwifi_vif,
                             struct cfg80211_connect_params *sme,
                             struct sm_connect_cfm *cfm);
int siwifi_send_sm_disconnect_req(struct siwifi_hw *siwifi_hw,
                                struct siwifi_vif *siwifi_vif,
                                u16 reason);
int siwifi_send_sm_external_auth_required_rsp(struct siwifi_hw *siwifi_hw,
                                            struct siwifi_vif *siwifi_vif,
                                            u16 status);
int siwifi_send_apm_start_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif,
                            struct cfg80211_ap_settings *settings,
                            struct apm_start_cfm *cfm,
                            struct siwifi_ipc_elem_var *elem);
int siwifi_send_apm_stop_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif);
int siwifi_send_scanu_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif,
                        struct cfg80211_scan_request *param);
int siwifi_send_apm_start_cac_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif,
                                struct cfg80211_chan_def *chandef,
                                struct apm_start_cac_cfm *cfm);
int siwifi_send_apm_stop_cac_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif);
int siwifi_send_tdls_peer_traffic_ind_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif);
int siwifi_send_config_monitor_req(struct siwifi_hw *siwifi_hw,
                                 struct cfg80211_chan_def *chandef,
                                 struct me_config_monitor_cfm *cfm);
int siwifi_send_mesh_start_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif,
                             const struct mesh_config *conf, const struct mesh_setup *setup,
                             struct mesh_start_cfm *cfm);
int siwifi_send_mesh_stop_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif,
                            struct mesh_stop_cfm *cfm);
int siwifi_send_mesh_update_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif,
                              u32 mask, const struct mesh_config *p_mconf, struct mesh_update_cfm *cfm);
int siwifi_send_mesh_peer_info_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif,
                                 u8 sta_idx, struct mesh_peer_info_cfm *cfm);
void siwifi_send_mesh_peer_update_ntf(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif,
                                    u8 sta_idx, u8 mlink_state);
void siwifi_send_mesh_path_create_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif, u8 *p_tgt_addr);
int siwifi_send_mesh_path_update_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif, const u8 *p_tgt_addr,
                                   const u8 *p_nhop_addr, struct mesh_path_update_cfm *p_cfm);
void siwifi_send_mesh_proxy_add_req(struct siwifi_hw *siwifi_hw, struct siwifi_vif *vif, u8 *p_ext_addr);

#ifdef CONFIG_SIWIFI_BFMER

void siwifi_send_bfmer_enable(struct siwifi_hw *siwifi_hw, struct siwifi_sta *siwifi_sta,
                            const struct ieee80211_vht_cap *vht_cap);
#ifdef CONFIG_SIWIFI_MUMIMO_TX
int siwifi_send_mu_group_update_req(struct siwifi_hw *siwifi_hw, struct siwifi_sta *siwifi_sta);
#endif /* CONFIG_SIWIFI_MUMIMO_TX */
#endif /* CONFIG_SIWIFI_BFMER */

/* Debug messages */
int siwifi_send_debug_frame(struct siwifi_hw *siwifi_hw, struct mm_send_debug_frame_req *params, uint8_t *payload);

int siwifi_send_dbg_trigger_req(struct siwifi_hw *siwifi_hw, char *msg ,uint8_t type);
int siwifi_send_dbg_mem_read_req(struct siwifi_hw *siwifi_hw, u32 mem_addr,
                               struct dbg_mem_read_cfm *cfm);
int siwifi_send_dbg_mem_write_req(struct siwifi_hw *siwifi_hw, u32 mem_addr,
                                u32 mem_data);
int siwifi_send_dbg_set_mod_filter_req(struct siwifi_hw *siwifi_hw, u32 filter);
int siwifi_send_dbg_set_sev_filter_req(struct siwifi_hw *siwifi_hw, u32 filter);
int siwifi_send_dbg_get_sys_stat_req(struct siwifi_hw *siwifi_hw,
                                   struct dbg_get_sys_stat_cfm *cfm);
#ifdef CONFIG_SIWIFI_TRX_STAT
int siwifi_send_dbg_get_tx_stat_req(struct siwifi_hw *siwifi_hw,
                                    struct dbg_get_tx_stat_cfm *cfm, uint32_t clear);
int siwifi_send_dbg_get_rx_stat_req(struct siwifi_hw *siwifi_hw,
                                    struct dbg_get_rx_stat_cfm *cfm, uint32_t clear);
#endif

int siwifi_send_cfg_rssi_req(struct siwifi_hw *siwifi_hw, u8 vif_index, int rssi_thold, u32 rssi_hyst);
#ifdef CONFIG_HEART_BEAT
int siwifi_send_heart(struct siwifi_hw *siwifi_hw);
#endif
int siwifi_send_vif_dump(struct siwifi_hw *siwifi_hw);

#ifdef NEW_SCHEDULE
int siwifi_send_dbg_print_burst_info(struct siwifi_hw *siwifi_hw, int ac);
#endif

#if defined CONFIG_VDR_HW
int siwifi_send_dbg_get_vendor_info_req(struct siwifi_hw *siwifi_hw,
        struct dbg_get_vendor_info_cfm *cfm, uint32_t clear, uint8_t vif_idx, uint8_t sta_idx);
#endif

int siwifi_send_dbg_get_vendor_mp_info_req(struct siwifi_hw *siwifi_hw,
        struct dbg_get_vendor_mp_info_cfm *cfm);
int siwifi_send_apm_deauth_req(struct siwifi_hw *siwifi_hw,
        uint8_t vif_idx, const u8 *mac, uint16_t reason_code);
int siwifi_send_apm_deassoc_req(struct siwifi_hw *siwifi_hw,
        uint8_t vif_idx, const u8 *mac, uint16_t reason_code);
int siwifi_send_probe_client(struct siwifi_hw *siwifi_hw, u8 sta_idx);
int siwifi_send_dbg_get_mgmt_info_req(struct siwifi_hw *siwifi_hw, struct dbg_get_mgmt_info_cfm *cfm, uint32_t clear);
int siwifi_send_dbg_get_ctrl_info_req(struct siwifi_hw *siwifi_hw, struct dbg_get_ctrl_info_cfm *cfm, uint32_t clear);
int siwifi_send_dbg_set_cca_parameter_req(struct siwifi_hw *siwifi_hw, uint32_t *param);
#endif /* _SIWIFI_MSG_TX_H_ */
