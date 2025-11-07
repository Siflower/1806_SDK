/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#ifndef __FAL_DISPATCH_H__
#define __FAL_DISPATCH_H__

/*
 * Include Files
 */
#include "yt_vlan.h"
#include "yt_acl.h"
#include "yt_qos.h"
#include "yt_rate.h"
#include "yt_dos.h"
#include "yt_nic.h"
#include "yt_stp.h"
#include "yt_l2.h"
#include "yt_loopdetect.h"
#include "yt_multicast.h"
#include "yt_rma.h"
#include "yt_oam.h"
#include "yt_storm_ctrl.h"
#include "yt_port.h"
#include "yt_lag.h"
#include "yt_dot1x.h"
#include "yt_ctrlpkt.h"
#include "yt_vlan_translate.h"
#include "yt_stat.h"
#include "yt_interrupt.h"
#include "yt_led.h"
#include "yt_sys.h"

//will add macro  to isolate it
/*
 * Symbol Definition
 */

/*
 * Macro Definition
 */

/*
 * Data Type Declaration
 */
typedef struct fal_dispatch_s
{
    uint8_t is_inited;

    /*VLAN*/
    yt_ret_t (*vlan_init)(yt_unit_t);
    yt_ret_t (*vlan_port_set)(yt_unit_t, yt_vlan_t, yt_port_mask_t, yt_port_mask_t);
    yt_ret_t (*vlan_port_get)(yt_unit_t, yt_vlan_t, yt_port_mask_t  *, yt_port_mask_t  *);
    yt_ret_t (*vlan_svlMode_enable_set)(yt_unit_t, yt_vlan_t, yt_enable_t);
    yt_ret_t (*vlan_svlMode_enable_get)(yt_unit_t, yt_vlan_t,   yt_enable_t *);
    yt_ret_t (*vlan_fid_set)(yt_unit_t, yt_vlan_t, yt_fid_t);
    yt_ret_t (*vlan_fid_get)(yt_unit_t, yt_vlan_t,   yt_fid_t *);
    yt_ret_t (*vlan_igrTpid_set)(yt_unit_t, yt_tpid_profiles_t);
    yt_ret_t (*vlan_igrTpid_get)(yt_unit_t,   yt_tpid_profiles_t *);
    yt_ret_t (*vlan_port_igrTpidSel_set)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_tpidprofile_id_mask_t);
    yt_ret_t (*vlan_port_igrTpidSel_get)(yt_unit_t, yt_vlan_type_t, yt_port_t,  yt_tpidprofile_id_mask_t *);
    yt_ret_t (*vlan_port_igrDefVlan_set)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_vlan_t);
    yt_ret_t (*vlan_port_igrDefVlan_get)(yt_unit_t, yt_vlan_type_t, yt_port_t,  yt_vlan_t *);
    yt_ret_t (*vlan_port_igrFilter_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*vlan_port_igrFilter_enable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*vlan_igrTransparent_set)(yt_unit_t, yt_port_t, yt_port_mask_t);
    yt_ret_t (*vlan_igrTransparent_get)(yt_unit_t, yt_port_t,  yt_port_mask_t *);
    yt_ret_t (*vlan_port_aft_set)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_vlan_aft_t);
    yt_ret_t (*vlan_port_aft_get)(yt_unit_t, yt_vlan_type_t, yt_port_t,  yt_vlan_aft_t *);
    yt_ret_t (*vlan_port_egrTagMode_set)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_egr_tag_mode_t);
    yt_ret_t (*vlan_port_egrTagMode_get)(yt_unit_t, yt_vlan_type_t, yt_port_t,   yt_egr_tag_mode_t *);
    yt_ret_t (*vlan_port_egrDefVid_set)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_vlan_t);
    yt_ret_t (*vlan_port_egrDefVid_get)(yt_unit_t, yt_vlan_type_t, yt_port_t,  yt_vlan_t *);
    yt_ret_t (*vlan_egrTpid_set)(yt_unit_t, yt_tpid_profiles_t);
    yt_ret_t (*vlan_egrTpid_get)(yt_unit_t,   yt_tpid_profiles_t *);
    yt_ret_t (*vlan_port_egrTpidSel_set)(yt_unit_t, yt_vlan_type_t, yt_port_t, uint8_t);
    yt_ret_t (*vlan_port_egrTpidSel_get)(yt_unit_t, yt_vlan_type_t, yt_port_t,  uint8_t *);
    yt_ret_t (*vlan_port_egrTransparent_set)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_enable_t, yt_port_mask_t);
    yt_ret_t (*vlan_port_egrTransparent_get)(yt_unit_t, yt_vlan_type_t, yt_port_t,   yt_enable_t *,  yt_port_mask_t *);
    yt_ret_t (*vlan_port_egrFilter_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*vlan_port_egrFilter_enable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*vlan_port_vidTypeSel_set)(yt_unit_t, yt_port_t, yt_vlan_type_t);
    yt_ret_t (*vlan_port_vidTypeSel_get)(yt_unit_t, yt_port_t,  yt_vlan_type_t *);

    /*vlan translate*/
    yt_ret_t (*vlan_trans_untagPvidIgnore_set)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_enable_t);
    yt_ret_t (*vlan_trans_untagPvidIgnore_get)(yt_unit_t, yt_vlan_type_t, yt_port_t,   yt_enable_t *);
    yt_ret_t (*vlan_trans_rangeProfile_add)(yt_unit_t, yt_vlan_range_group_t,  yt_profile_id_t *);
    yt_ret_t (*vlan_trans_rangeProfile_get)(yt_unit_t, yt_profile_id_t,  yt_vlan_range_group_t *);
    yt_ret_t (*vlan_trans_rangeProfile_del)(yt_unit_t, yt_profile_id_t);
    yt_ret_t (*vlan_trans_port_rangeProfileSel_set)(yt_unit_t, yt_port_t, yt_profile_id_t);
    yt_ret_t (*vlan_trans_port_rangeProfileSel_get)(yt_unit_t, yt_port_t,  yt_profile_id_t *);
    yt_ret_t (*vlan_trans_mode_set)(yt_unit_t, yt_port_t, yt_vlan_range_trans_mode_t);
    yt_ret_t (*vlan_trans_mode_get)(yt_unit_t, yt_port_t,  yt_vlan_range_trans_mode_t *);
    yt_ret_t (*vlan_igr_trans_table_add)(yt_unit_t,  yt_vlan_trans_tbl_t *,   yt_vlan_trans_action_tbl_t *,  yt_trans_tbl_id_t *);
    yt_ret_t (*vlan_igr_trans_table_get)(yt_unit_t, yt_trans_tbl_id_t,  yt_vlan_trans_tbl_t *,   yt_vlan_trans_action_tbl_t *);
    yt_ret_t (*vlan_igr_trans_table_del)(yt_unit_t, yt_trans_tbl_id_t);
    yt_ret_t (*vlan_egr_trans_table_add)(yt_unit_t,  yt_egr_vlan_trans_tbl_t  *,  yt_egr_vlan_trans_action_tbl_t *,  yt_trans_tbl_id_t *);
    yt_ret_t (*vlan_egr_trans_table_get)(yt_unit_t, yt_trans_tbl_id_t,  yt_egr_vlan_trans_tbl_t  *,  yt_egr_vlan_trans_action_tbl_t *);
    yt_ret_t (*vlan_egr_trans_table_del)(yt_unit_t, yt_trans_tbl_id_t);

    yt_ret_t (*dos_init)(yt_unit_t);
    yt_ret_t (*dos_port_en_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*dos_port_en_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*dos_drop_en_set)(yt_unit_t, yt_dos_type_t, yt_enable_t);
    yt_ret_t (*dos_drop_en_get)(yt_unit_t, yt_dos_type_t,  yt_enable_t *);
    yt_ret_t (*dos_large_icmp_size_set)(yt_unit_t, yt_dos_icmp_version_t, uint16_t);
    yt_ret_t (*dos_large_icmp_size_get)(yt_unit_t, yt_dos_icmp_version_t,  uint16_t *);

    yt_ret_t (*acl_init)(yt_unit_t);
    yt_ret_t (*acl_port_en_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*acl_port_en_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*acl_unmatch_permit_en_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*acl_unmatch_permit_en_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*acl_udf_rule_set)(yt_unit_t, uint8_t, yt_acl_udf_type_t, uint8_t);
    yt_ret_t (*acl_rule_init)(yt_unit_t);
    yt_ret_t (*acl_rule_reset)(yt_unit_t);
    yt_ret_t (*acl_rule_key_add)(yt_unit_t, yt_igrAcl_key_type_t,  yt_comm_key_t *);
    yt_ret_t (*acl_rule_action_add)(yt_unit_t, yt_acl_action_type_t,  yt_comm_act_t *);
    yt_ret_t (*acl_rule_create)(yt_unit_t, uint16_t, yt_bool_t,  uint32_t *);
    yt_ret_t (*acl_rule_active)(yt_unit_t, uint32_t);
    yt_ret_t (*acl_rule_del)(yt_unit_t, uint32_t);

    yt_ret_t (*dot1x_init)(yt_unit_t);
    yt_ret_t (*dot1x_portBasedEnable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*dot1x_portBasedEnable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*dot1x_portBasedAuthStatus_set)(yt_unit_t, yt_port_t, yt_dot1x_auth_status_t);
    yt_ret_t (*dot1x_portBasedAuthStatus_get)(yt_unit_t, yt_port_t,  yt_dot1x_auth_status_t *);
    yt_ret_t (*dot1x_portBasedDirection_set)(yt_unit_t, yt_port_t, yt_dot1x_direction_t);
    yt_ret_t (*dot1x_portBasedDirection_get)(yt_unit_t, yt_port_t,  yt_dot1x_direction_t *);
    yt_ret_t (*dot1x_guest_vlan_set)(yt_unit_t, yt_vlan_t, yt_enable_t);
    yt_ret_t (*dot1x_guest_vlan_get)(yt_unit_t, yt_vlan_t,  yt_enable_t *);
    yt_ret_t (*dot1x_tx_bypass_bc_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*dot1x_tx_bypass_bc_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*dot1x_tx_bypass_mc_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*dot1x_tx_bypass_mc_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*dot1x_rx_bypass_bc_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*dot1x_rx_bypass_bc_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*dot1x_rx_bypass_mc_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*dot1x_rx_bypass_mc_get)(yt_unit_t,  yt_enable_t *);

    yt_ret_t (*oam_en_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*oam_en_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*oam_bypass_accessctrl_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*oam_bypass_accessctrl_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*oam_parser_act_set)(yt_unit_t, yt_port_t, yt_oam_parser_action_t);
    yt_ret_t (*oam_parser_act_get)(yt_unit_t, yt_port_t,  yt_oam_parser_action_t *);
    yt_ret_t (*oam_mux_act_set)(yt_unit_t, yt_port_t, yt_oam_mux_action_t);
    yt_ret_t (*oam_mux_act_get)(yt_unit_t, yt_port_t,  yt_oam_mux_action_t *);

    yt_ret_t (*qos_intPri_map_weight_set)(yt_unit_t, yt_port_t, yt_qos_intPri_map_weight_t);
    yt_ret_t (*qos_intPri_map_weight_get)(yt_unit_t, yt_port_t,  yt_qos_intPri_map_weight_t *);
    yt_ret_t (*qos_intPri_portDefPri_set )(yt_unit_t, yt_port_t, yt_enable_t, yt_pri_t);
    yt_ret_t (*qos_intPri_portDefPri_get )(yt_unit_t, yt_port_t,  yt_enable_t *,  yt_pri_t *);
    yt_ret_t (*qos_intPri_cpri_map_set)(yt_unit_t, yt_port_t, yt_qos_pmap_tci_t, yt_pri_t);
    yt_ret_t (*qos_intPri_cpri_map_get)(yt_unit_t, yt_port_t, yt_qos_pmap_tci_t,  yt_pri_t *);
    yt_ret_t (*qos_intDP_cpri_map_set)(yt_unit_t, yt_port_t, yt_qos_pmap_tci_t, yt_dp_t);
    yt_ret_t (*qos_intDP_cpri_map_get)(yt_unit_t, yt_port_t, yt_qos_pmap_tci_t,  yt_dp_t *);
    yt_ret_t (*qos_intPri_spri_map_set)(yt_unit_t, yt_port_t, yt_qos_pmap_tci_t, yt_pri_t);
    yt_ret_t (*qos_intPri_spri_map_get)(yt_unit_t, yt_port_t, yt_qos_pmap_tci_t,  yt_pri_t *);
    yt_ret_t (*qos_intDP_spri_map_set)(yt_unit_t, yt_port_t, yt_qos_pmap_tci_t, yt_dp_t);
    yt_ret_t (*qos_intDP_spri_map_get)(yt_unit_t, yt_port_t, yt_qos_pmap_tci_t,  yt_dp_t *);
    yt_ret_t (*qos_intPri_dscp_map_set)(yt_unit_t, yt_dscp_t, yt_pri_t);
    yt_ret_t (*qos_intPri_dscp_map_get)(yt_unit_t, yt_dscp_t,  yt_pri_t *);
    yt_ret_t (*qos_intDP_dscp_map_set)(yt_unit_t, yt_dscp_t, yt_dp_t);
    yt_ret_t (*qos_intDP_dscp_map_get)(yt_unit_t, yt_dscp_t,  yt_dp_t *);
    yt_ret_t (*qos_intPri_map_igrMirror_set)(yt_unit_t, yt_enable_t, yt_pri_t);
    yt_ret_t (*qos_intPri_map_igrMirror_get)(yt_unit_t,  yt_enable_t *,  yt_pri_t *);
    yt_ret_t (*qos_intPri_map_egrMirror_set)(yt_unit_t, yt_enable_t, yt_pri_t);
    yt_ret_t (*qos_intPri_map_egrMirror_get)(yt_unit_t,  yt_enable_t *,  yt_pri_t *);
    yt_ret_t (*qos_que_map_ucast_set)(yt_unit_t, yt_port_t, yt_qos_qmap_t);
    yt_ret_t (*qos_que_map_ucast_get)(yt_unit_t, yt_port_t,  yt_qos_qmap_t *);
    yt_ret_t (*qos_que_map_mcast_set)(yt_unit_t, yt_port_t, yt_qos_qmap_t);
    yt_ret_t (*qos_que_map_mcast_get)(yt_unit_t, yt_port_t,  yt_qos_qmap_t *);
    yt_ret_t (*qos_que_forceDrop_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*qos_que_forceDrop_enable_get)(yt_unit_t, yt_port_t, yt_enable_t *);
    yt_ret_t (*qos_remark_port_set)(yt_unit_t, yt_port_t, yt_qos_remark_en_t);
    yt_ret_t (*qos_remark_port_get)(yt_unit_t, yt_port_t,  yt_qos_remark_en_t *);
    yt_ret_t (*qos_remark_dscp_set)(yt_unit_t , yt_qos_remark_info_t, yt_dscp_t);
    yt_ret_t (*qos_remark_dscp_get)(yt_unit_t, yt_qos_remark_info_t,  yt_dscp_t *);
    yt_ret_t (*qos_remark_cpri_set)(yt_unit_t, yt_port_t, yt_qos_remark_info_t, yt_qos_pmap_tci_t);
    yt_ret_t (*qos_remark_cpri_get)(yt_unit_t, yt_port_t, yt_qos_remark_info_t,  yt_qos_pmap_tci_t *);
    yt_ret_t (*qos_remark_spri_set)(yt_unit_t, yt_port_t, yt_qos_remark_info_t, yt_qos_pmap_tci_t);
    yt_ret_t (*qos_remark_spri_get)(yt_unit_t, yt_port_t, yt_qos_remark_info_t,  yt_qos_pmap_tci_t *);
    yt_ret_t (*qos_schedule_sp_set)(yt_unit_t, yt_qid_t, yt_queue_pri_t);
    yt_ret_t (*qos_schedule_sp_get)(yt_unit_t, yt_qid_t,  yt_queue_pri_t *);
    yt_ret_t (*qos_schedule_dwrr_mode_set)(yt_unit_t, yt_qid_t, yt_rate_mode_t);
    yt_ret_t (*qos_schedule_dwrr_mode_get)(yt_unit_t, yt_qid_t,  yt_rate_mode_t *);
    yt_ret_t (*qos_schedule_dwrr_set)(yt_unit_t, yt_qid_t, yt_queue_weight_t);
    yt_ret_t (*qos_schedule_dwrr_get)(yt_unit_t, yt_qid_t,  yt_queue_weight_t *);
    yt_ret_t (*qos_intPri_vlan_map_set)(yt_unit_t, yt_vlan_t, yt_enable_t, yt_pri_t);
    yt_ret_t (*qos_intPri_vlan_map_get)(yt_unit_t, yt_vlan_t,  yt_enable_t *,  yt_pri_t *);

    yt_ret_t (*rate_init)(yt_unit_t);
    yt_ret_t (*rate_igrBandwidthCtrlEnable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*rate_igrBandwidthCtrlEnable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*rate_igrBandwidthCtrlMode_set)(yt_unit_t, yt_port_t, yt_port_rate_mode_t);
    yt_ret_t (*rate_igrBandwidthCtrlMode_get)(yt_unit_t, yt_port_t,  yt_port_rate_mode_t *);
    yt_ret_t (*rate_igrBandwidthCtrlRate_set)(yt_unit_t, yt_port_t, uint32_t);
    yt_ret_t (*rate_igrBandwidthCtrlRate_get)(yt_unit_t, yt_port_t,  uint32_t *);
    yt_ret_t (*rate_meter_vlan_enable_set)(yt_unit_t, yt_vlan_t, yt_meterid_t, yt_enable_t);
    yt_ret_t (*rate_meter_vlan_enable_get)(yt_unit_t, yt_vlan_t,  yt_meterid_t *,  yt_enable_t *);
    yt_ret_t (*rate_meter_enable_set)(yt_unit_t, yt_meterid_t, yt_enable_t);
    yt_ret_t (*rate_meter_enable_get)(yt_unit_t, yt_meterid_t,  yt_enable_t *);
    yt_ret_t (*rate_meter_mode_set)(yt_unit_t, yt_meterid_t, yt_rate_meter_mode_t);
    yt_ret_t (*rate_meter_mode_get)(yt_unit_t, yt_meterid_t,  yt_rate_meter_mode_t *);
    yt_ret_t (*rate_meter_rate_set)(yt_unit_t, yt_meterid_t, yt_qos_two_rate_t);
    yt_ret_t (*rate_meter_rate_get)(yt_unit_t, yt_meterid_t,  yt_qos_two_rate_t *);
    yt_ret_t (*rate_shaping_port_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*rate_shaping_port_enable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*rate_shaping_port_mode_set)(yt_unit_t, yt_port_t, yt_shaping_mode_t);
    yt_ret_t (*rate_shaping_port_mode_get)(yt_unit_t, yt_port_t,  yt_shaping_mode_t *);
    yt_ret_t (*rate_shaping_port_rate_set)(yt_unit_t, yt_port_t, uint32_t);
    yt_ret_t (*rate_shaping_port_rate_get)(yt_unit_t, yt_port_t,  uint32_t *);
    yt_ret_t (*rate_shaping_queue_enable_set)(yt_unit_t, yt_qid_t, yt_enable_t, yt_enable_t);
    yt_ret_t (*rate_shaping_queue_enable_get)(yt_unit_t, yt_qid_t,  yt_enable_t *,  yt_enable_t *);
    yt_ret_t (*rate_shaping_queue_mode_set)(yt_unit_t, yt_qid_t, yt_shaping_mode_t);
    yt_ret_t (*rate_shaping_queue_mode_get)(yt_unit_t, yt_qid_t,  yt_shaping_mode_t *);
    yt_ret_t (*rate_shaping_queue_rate_set)(yt_unit_t, yt_qid_t, yt_qos_two_rate_t);
    yt_ret_t (*rate_shaping_queue_rate_get)(yt_unit_t, yt_qid_t,  yt_qos_two_rate_t *);

    yt_ret_t (*stat_mib_init)(yt_unit_t);
    yt_ret_t (*stat_mib_enable_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*stat_mib_enable_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*stat_mib_clear)(yt_unit_t, yt_port_t);
    yt_ret_t (*stat_mib_clear_all)(yt_unit_t);
    yt_ret_t (*stat_mib_port_get)(yt_unit_t, yt_port_t,  yt_stat_mib_port_cnt_t *);
    yt_ret_t (*stat_flow_enable_set)(yt_unit_t, uint32_t, yt_enable_t);
    yt_ret_t (*stat_flow_enable_get)(yt_unit_t, uint32_t, yt_enable_t *);
    yt_ret_t (*stat_flow_mode_set)(yt_unit_t, uint32_t, yt_stat_type_t, yt_stat_mode_t);
    yt_ret_t (*stat_flow_mode_get)(yt_unit_t, uint32_t, yt_stat_type_t *, yt_stat_mode_t *);
    yt_ret_t (*stat_flow_count_set)(yt_unit_t, uint32_t, uint64);
    yt_ret_t (*stat_flow_count_get)(yt_unit_t, uint32_t,  uint64 *);

    yt_ret_t (*stp_state_set)(yt_unit_t, uint8_t, yt_port_t, yt_stp_state_t);
    yt_ret_t (*stp_state_get)(yt_unit_t, uint8_t, yt_port_t,  yt_stp_state_t *);
    yt_ret_t (*stp_instance_map_set)(yt_unit_t, yt_vlan_t, yt_stp_id_t);
    yt_ret_t (*stp_instance_map_get)(yt_unit_t, yt_vlan_t,  yt_stp_id_t *);

    yt_ret_t (*port_isolation_set)(yt_unit_t, yt_port_t, yt_port_mask_t);
    yt_ret_t (*port_isolation_get)(yt_unit_t, yt_port_t,  yt_port_mask_t *);

    yt_ret_t (*loop_detect_enable_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*loop_detect_enable_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*loop_detect_tpid_set)(yt_unit_t, yt_tpid_t);
    yt_ret_t (*loop_detect_tpid_get)(yt_unit_t,  yt_tpid_t *);
    yt_ret_t (*loop_detect_generate_way_set)(yt_unit_t, yt_generate_way_t);
    yt_ret_t (*loop_detect_generate_way_get)(yt_unit_t,  yt_generate_way_t *);
    yt_ret_t (*loop_detect_unitID_set)(yt_unit_t, yt_local_id_t, yt_remote_id_t);
    yt_ret_t (*loop_detect_unitID_get)(yt_unit_t,  yt_local_id_t *,  yt_remote_id_t *);

    yt_ret_t (*storm_ctrl_init)(yt_unit_t);
    yt_ret_t (*storm_ctrl_enable_set)(yt_unit_t, yt_port_t, yt_storm_type_t, yt_enable_t);
    yt_ret_t (*storm_ctrl_enable_get)(yt_unit_t, yt_port_t, yt_storm_type_t,  yt_enable_t *);
    yt_ret_t (*storm_ctrl_rate_mode_set)(yt_unit_t, yt_port_t, yt_storm_type_t, yt_storm_rate_mode_t);
    yt_ret_t (*storm_ctrl_rate_mode_get)(yt_unit_t, yt_port_t, yt_storm_type_t,  yt_storm_rate_mode_t *);
    yt_ret_t (*storm_ctrl_rate_include_gap_set)(yt_unit_t, yt_port_t, yt_storm_type_t, yt_storm_rate_gap_t);
    yt_ret_t (*storm_ctrl_rate_include_gap_get)(yt_unit_t, yt_port_t, yt_storm_type_t,  yt_storm_rate_gap_t *);
    yt_ret_t (*storm_ctrl_rate_set)(yt_unit_t, yt_port_t, yt_storm_type_t, uint32_t);
    yt_ret_t (*storm_ctrl_rate_get)(yt_unit_t, yt_port_t, yt_storm_type_t,  uint32_t *);

    yt_ret_t (*lag_hash_sel_set)(yt_unit_t, uint8_t);
    yt_ret_t (*lag_hash_sel_get)(yt_unit_t,  uint8_t *);
    yt_ret_t (*lag_group_port_set)(yt_unit_t, uint8_t, yt_port_mask_t);
    yt_ret_t (*lag_group_info_get)(yt_unit_t, uint8_t,  yt_link_agg_group_t *);

    yt_ret_t (*l2_init)(yt_unit_t);
    yt_ret_t (*l2_mcast_addr_add)(yt_unit_t, yt_vlan_t, yt_mac_addr_t, yt_port_mask_t);
    yt_ret_t (*l2_fdb_ucast_addr_add)(yt_unit_t, yt_vlan_t, yt_mac_addr_t, yt_port_t, yt_bool_t);
    yt_ret_t (*l2_fdb_ucast_addr_del)(yt_unit_t, yt_vlan_t, yt_mac_addr_t);
    yt_ret_t (*l2_mcast_addr_del)(yt_unit_t, yt_vlan_t, yt_mac_addr_t);

    yt_ret_t (*l2_fdb_linkdownFlush_en_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*l2_fdb_linkdownFlush_en_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*l2_fdb_all_ucast_flush)(yt_unit_t);
    yt_ret_t (*l2_fdb_port_ucast_flush)(yt_unit_t, yt_port_t);
    yt_ret_t (*l2_fdb_vlan_ucast_flush)(yt_unit_t, yt_vlan_t);
    yt_ret_t (*l2_all_mcast_flush)(yt_unit_t);
    yt_ret_t (*l2_port_mcast_flush)(yt_unit_t, yt_port_t);
    yt_ret_t (*l2_vlan_mcast_flush)(yt_unit_t, yt_vlan_t);
    yt_ret_t (*l2_fdb_type_get)(yt_unit_t, yt_vlan_t, yt_mac_addr_t,  yt_l2_fdb_type_t *);
    yt_ret_t (*l2_port_learnlimit_en_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*l2_port_learnlimit_en_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*l2_port_learnlimit_cnt_set)(yt_unit_t, yt_port_t, uint32_t);
    yt_ret_t (*l2_port_learnlimit_cnt_get)(yt_unit_t, yt_port_t,  uint32_t *);
    yt_ret_t (*l2_port_learnlimit_exceed_drop_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*l2_port_learnlimit_exceed_drop_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*l2_system_learnlimit_en_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*l2_system_learnlimit_en_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*l2_system_learnlimit_cnt_set)(yt_unit_t, uint32_t);
    yt_ret_t (*l2_system_learnlimit_cnt_get)(yt_unit_t,  uint32_t *);
    yt_ret_t (*l2_system_learnlimit_exceed_drop_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*l2_system_learnlimit_exceed_drop_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*l2_fdb_lag_uc_cnt_get)(yt_unit_t, uint8_t,  uint32_t *);
    yt_ret_t (*l2_lag_learnlimit_en_set)(yt_unit_t, uint8_t, yt_enable_t);
    yt_ret_t (*l2_lag_learnlimit_en_get)(yt_unit_t, uint8_t,   yt_enable_t *);
    yt_ret_t (*l2_lag_learnlimit_cnt_set)(yt_unit_t, uint8_t, uint32_t);
    yt_ret_t (*l2_lag_learnlimit_cnt_get)(yt_unit_t, uint8_t,   uint32_t *);
    yt_ret_t (*l2_lag_learnlimit_exceed_drop_set)(yt_unit_t, uint8_t, yt_enable_t);
    yt_ret_t (*l2_lag_learnlimit_exceed_drop_get)(yt_unit_t, uint8_t,  yt_enable_t *);
    yt_ret_t (*l2_fdb_drop_sa_set)(yt_unit_t, yt_vlan_t, yt_mac_addr_t, yt_enable_t);
    yt_ret_t (*l2_fdb_drop_sa_get)(yt_unit_t, yt_vlan_t, yt_mac_addr_t,   yt_enable_t *);
    yt_ret_t (*l2_fdb_drop_da_set)(yt_unit_t, yt_vlan_t, yt_mac_addr_t, yt_enable_t);
    yt_ret_t (*l2_fdb_drop_da_get)(yt_unit_t, yt_vlan_t, yt_mac_addr_t,   yt_enable_t *);
    yt_ret_t (*l2_fdb_copy2cpu_set)(yt_unit_t, yt_vlan_t, yt_mac_addr_t, yt_enable_t);
    yt_ret_t (*l2_fdb_copy2cpu_get)(yt_unit_t, yt_vlan_t, yt_mac_addr_t,  yt_enable_t *);
    yt_ret_t (*l2_filter_mcast_set)(yt_unit_t, yt_port_mask_t);
    yt_ret_t (*l2_filter_mcast_get)(yt_unit_t,  yt_port_mask_t *);
    yt_ret_t (*l2_filter_bcast_set)(yt_unit_t, yt_port_mask_t);
    yt_ret_t (*l2_filter_bcast_get)(yt_unit_t,  yt_port_mask_t *);
    yt_ret_t (*l2_filter_unknown_ucast_set)(yt_unit_t, yt_port_mask_t);
    yt_ret_t (*l2_filter_unknown_ucast_get)(yt_unit_t,  yt_port_mask_t *);
    yt_ret_t (*l2_filter_unknown_mcast_set)(yt_unit_t, yt_port_mask_t);
    yt_ret_t (*l2_filter_unknown_mcast_get)(yt_unit_t,  yt_port_mask_t *);
    yt_ret_t (*l2_rma_bypass_unknown_mcast_filter_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*l2_rma_bypass_unknown_mcast_filter_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*l2_fdb_port_uc_cnt_get)(yt_unit_t, yt_port_t,  uint32 *);
    yt_ret_t (*l2_fdb_uc_cnt_get)(yt_unit_t,   uint32 *);
    yt_ret_t (*l2_mc_cnt_get)(yt_unit_t,   uint32 *);
    yt_ret_t (*l2_fdb_aging_port_en_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*l2_fdb_aging_port_en_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*l2_fdb_aging_time_set)(yt_unit_t, uint32_t);
    yt_ret_t  (*l2_fdb_aging_time_get)(yt_unit_t,  uint32_t *);
    yt_ret_t (*l2_fdb_uc_withindex_get)(yt_unit_t, uint16_t,  l2_ucastMacAddr_info_t *);
    yt_ret_t (*l2_fdb_uc_withMacAndVid_get)(yt_unit_t, yt_vlan_t, yt_mac_addr_t, l2_ucastMacAddr_info_t *);
    yt_ret_t  (*l2_fdb_uc_withindex_getnext)(yt_unit_t, uint16_t, uint16_t *, l2_ucastMacAddr_info_t *);
    yt_ret_t (*l2_port_learn_en_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*l2_port_learn_en_get)(yt_unit_t, yt_port_t, yt_enable_t *);

    yt_ret_t (*multicast_igmp_opmode_set)(yt_unit_t, yt_multi_op_mode_t);
    yt_ret_t (*multicast_igmp_opmode_get)(yt_unit_t,   yt_multi_op_mode_t *);
    yt_ret_t (*multicast_mld_opmode_set)(yt_unit_t, yt_multi_op_mode_t);
    yt_ret_t (*multicast_mld_opmode_get)(yt_unit_t,   yt_multi_op_mode_t *);
    yt_ret_t (*multicast_port_report_allow_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*multicast_port_report_allow_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*multicast_port_leave_allow_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*multicast_port_leave_allow_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*multicast_port_query_allow_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*multicast_port_query_allow_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*multicast_fastleave_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*multicast_fastleave_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*multicast_learnlimit_en_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*multicast_learnlimit_en_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*multicast_learnlimit_maxgroup_set)(yt_unit_t, uint32_t);
    yt_ret_t (*multicast_learnlimit_maxgroup_get)(yt_unit_t,  uint32_t  *);
    yt_ret_t (*multicast_fwd_routerport_only_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*multicast_fwd_routerport_only_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*multicast_fwd_routerport_primary_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*multicast_fwd_routerport_primary_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*multicast_bypass_grouprange_set)(yt_unit_t, yt_multi_learn_bypass_range_t);
    yt_ret_t (*multicast_bypass_grouprange_get)(yt_unit_t,  yt_multi_learn_bypass_range_t *);
    yt_ret_t (*multicast_dynamic_routerport_get)(yt_unit_t,  yt_multi_router_port_t *);
    yt_ret_t (*multicast_routerport_dynamic_allow_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*multicast_routerport_dynamic_allow_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*multicast_dynamic_routerport_agingtime_set)(yt_unit_t, uint32_t);
    yt_ret_t (*multicast_dynamic_routerport_agingtime_get)(yt_unit_t,  uint32_t *);
    yt_ret_t (*multicast_vlan_add)(yt_unit_t, yt_vlan_t);
    yt_ret_t (*multicast_vlan_get)(yt_unit_t,  yt_multi_vlan_t *);
    yt_ret_t (*multicast_vlan_del)(yt_unit_t, yt_vlan_t);
    yt_ret_t (*multicast_static_routerport_set)(yt_unit_t, yt_port_mask_t);
    yt_ret_t (*multicast_static_routerport_get)(yt_unit_t,  yt_port_mask_t *);
    yt_ret_t (*multicast_igmp_bypass_port_isolation_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*multicast_igmp_bypass_port_isolation_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*multicast_ipmc_bypass_port_isolation_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*multicast_ipmc_bypass_port_isolation_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*multicast_port_igmp_bypass_ingrfilter_en_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*multicast_port_igmp_bypass_ingrfilter_en_get)(yt_unit_t, yt_port_t,  yt_enable_t *);

    yt_ret_t (*nic_init)(yt_unit_t);
    yt_ret_t (*nic_cpuport_mode_set)(yt_unit_t, yt_cpuport_mode_t);
    yt_ret_t (*nic_cpuport_mode_get)(yt_unit_t,  yt_cpuport_mode_t *);
    yt_ret_t (*nic_ext_cpuport_en_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*nic_ext_cpuport_en_get)(yt_unit_t,   yt_enable_t *);
    yt_ret_t (*nic_ext_cpuport_port_set)(yt_unit_t, yt_port_t);
    yt_ret_t (*nic_ext_cpuport_port_get)(yt_unit_t,   yt_port_t *);
    yt_ret_t (*nic_cpuport_tagtpid_set)(yt_unit_t, uint16_t);
    yt_ret_t (*nic_cpuport_tagtpid_get)(yt_unit_t,  uint16_t *);
    yt_ret_t (*nic_ext_cputag_en_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*nic_ext_cputag_en_get)(yt_unit_t,   yt_enable_t *);

    yt_ret_t (*mirror_port_set)(yt_unit_t, yt_port_t, yt_port_mask_t, yt_port_mask_t);
    yt_ret_t (*mirror_port_get)(yt_unit_t,  yt_port_t *,  yt_port_mask_t *,  yt_port_mask_t *);

    yt_ret_t (*rma_action_set)(yt_unit_t, yt_rma_da_t, yt_rma_action_t);
    yt_ret_t (*rma_action_get)(yt_unit_t, yt_rma_da_t,  yt_rma_action_t *);
    yt_ret_t (*rma_bypass_port_isolation_set)(yt_unit_t, yt_rma_da_t, yt_enable_t);
    yt_ret_t (*rma_bypass_port_isolation_get)(yt_unit_t, yt_rma_da_t,  yt_enable_t *);
    yt_ret_t (*rma_bypass_vlan_filter_set)(yt_unit_t, yt_rma_da_t, yt_enable_t);
    yt_ret_t (*rma_bypass_vlan_filter_get)(yt_unit_t, yt_rma_da_t,  yt_enable_t *);

    yt_ret_t (*wol_ctrl_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*wol_ctrl_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*wol_ethertype_set)(yt_unit_t, yt_tpid_t);
    yt_ret_t (*wol_ethertype_get)(yt_unit_t,  yt_tpid_t *);

    yt_ret_t (*sys_mac_get)(yt_unit_t , yt_mac_addr_t * );
    yt_ret_t (*sys_chip_reset)(yt_unit_t);
    yt_ret_t (*sys_database_reset)(yt_unit_t);
    yt_ret_t (*sys_chipInfo_get)(yt_unit_t,  yt_switch_chip_t *);

    yt_ret_t (*port_init)(yt_unit_t);
    yt_ret_t (*port_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*port_enable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*port_link_status_get)(yt_unit_t, yt_port_t,  yt_port_link_status_t *);
    yt_ret_t (*port_link_status_all_get)(yt_unit_t, yt_port_t,  yt_port_linkStatus_all_t *);
    yt_ret_t (*port_backpress_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*port_backpress_enable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*port_cascade_set)(yt_unit_t, yt_cascade_info_t);
    yt_ret_t (*port_cascade_get)(yt_unit_t,  yt_cascade_info_t *);
    yt_ret_t (*port_pkt_gap_set)(yt_unit_t, yt_port_t, uint8_t);
    yt_ret_t (*port_pkt_gap_get)(yt_unit_t, yt_port_t,  uint8_t *);
    yt_ret_t (*port_macAutoNeg_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*port_macAutoNeg_enable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*port_mac_force_set)(yt_unit_t, yt_port_t, yt_port_force_ctrl_t);
    yt_ret_t (*port_mac_force_get)(yt_unit_t, yt_port_t,  yt_port_force_ctrl_t *);
    yt_ret_t (*port_mac_fc_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*port_mac_fc_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*port_extif_mode_set)(yt_unit_t, yt_port_t, yt_extif_mode_t);
    yt_ret_t (*port_extif_mode_get)(yt_unit_t, yt_port_t,  yt_extif_mode_t *);
    yt_ret_t (*port_extif_force_set)(yt_unit_t, yt_port_t, yt_extif_force_ctrl_t);
    yt_ret_t (*port_extif_force_get)(yt_unit_t, yt_port_t, yt_extif_force_ctrl_t *);
    yt_ret_t (*port_extif_rgmii_delay_set)(yt_unit_t, yt_port_t, uint8_t, uint8_t, yt_enable_t);
    yt_ret_t (*port_extif_rgmii_delay_get)(yt_unit_t, yt_port_t,  uint8_t *,  uint8_t *,  yt_enable_t *);
    yt_ret_t (*port_phyAutoNeg_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*port_phyAutoNeg_enable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*port_phyAutoNeg_ability_set)(yt_unit_t, yt_port_t, yt_port_an_ability_t);
    yt_ret_t (*port_phyAutoNeg_ability_get)(yt_unit_t, yt_port_t,  yt_port_an_ability_t *);
    yt_ret_t (*port_phy_force_set)(yt_unit_t, yt_port_t, yt_port_speed_duplex_t);
    yt_ret_t (*port_phy_force_get)(yt_unit_t, yt_port_t,  yt_port_speed_duplex_t *);
    yt_ret_t (*port_phy_linkstatus_get)(yt_unit_t, yt_port_t,  yt_port_linkStatus_all_t *);
    yt_ret_t (*port_phy_reg_set)(yt_unit_t, yt_port_t, uint32_t, uint16_t, yt_phy_type_t);
    yt_ret_t (*port_phy_reg_get)(yt_unit_t, yt_port_t, uint32_t,  uint16_t *, yt_phy_type_t);
    yt_ret_t (*port_eee_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*port_eee_enable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*port_jumbo_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*port_jumbo_enable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*port_jumbo_size_set)(yt_unit_t, yt_port_t, uint32_t);
    yt_ret_t (*port_jumbo_size_get)(yt_unit_t, yt_port_t,  uint32_t *);
    yt_ret_t (*port_cable_diag)(yt_unit_t, yt_port_t,  yt_port_cableDiag_t *);
    yt_ret_t (*port_phyTemplate_test_set)(yt_unit_t, yt_port_t,  yt_utp_template_testmode_t);

    /* control packet */
    yt_ret_t (*ctrlpkt_unknown_ucast_act_set)(yt_unit_t, yt_port_t, yt_ctrlpkt_l2_action_t);
    yt_ret_t (*ctrlpkt_unknown_ucast_act_get)(yt_unit_t, yt_port_t,  yt_ctrlpkt_l2_action_t *);
    yt_ret_t (*ctrlpkt_unknown_mcast_act_set)(yt_unit_t, yt_port_t, yt_ctrlpkt_l2_action_t);
    yt_ret_t (*ctrlpkt_unknown_mcast_act_get)(yt_unit_t, yt_port_t,  yt_ctrlpkt_l2_action_t *);
    yt_ret_t (*ctrlpkt_arp_act_set)(yt_unit_t, yt_port_t, yt_ctrlpkt_l2_action_t);
    yt_ret_t (*ctrlpkt_arp_act_get)(yt_unit_t, yt_port_t,  yt_ctrlpkt_l2_action_t *);
    yt_ret_t (*ctrlpkt_nd_act_set)(yt_unit_t, yt_port_t, yt_ctrlpkt_l2_action_t);
    yt_ret_t (*ctrlpkt_nd_act_get)(yt_unit_t, yt_port_t,  yt_ctrlpkt_l2_action_t *);
    yt_ret_t (*ctrlpkt_lldp_eee_act_set)(yt_unit_t, yt_port_t, yt_ctrlpkt_l2_action_t);
    yt_ret_t (*ctrlpkt_lldp_eee_act_get)(yt_unit_t, yt_port_t,  yt_ctrlpkt_l2_action_t *);
    yt_ret_t (*ctrlpkt_lldp_act_set)(yt_unit_t, yt_port_t, yt_ctrlpkt_l2_action_t);
    yt_ret_t (*ctrlpkt_lldp_act_get)(yt_unit_t, yt_port_t,  yt_ctrlpkt_l2_action_t *);

    /* interrupt */
    yt_ret_t (*int_polarity_set)(yt_unit_t, yt_int_polarity_t);
    yt_ret_t (*int_polarity_get)(yt_unit_t,  yt_int_polarity_t *);
    yt_ret_t (*int_control_set)(yt_unit_t, yt_int_type_t, yt_enable_t);
    yt_ret_t (*int_control_get)(yt_unit_t, yt_int_type_t,  yt_enable_t *);
    yt_ret_t (*int_status_get)(yt_unit_t,  yt_intr_status_t *);

    /* led */
    yt_ret_t (*led_enable)(yt_unit_t);
    yt_ret_t (*led_mode_set)(yt_unit_t,  yt_led_mode_t);
    yt_ret_t (*led_mode_get)(yt_unit_t, yt_led_mode_t *);
    yt_ret_t (*led_action_set)(yt_unit_t, yt_port_t, yt_led_id_t, yt_led_act_cfg_t);
    yt_ret_t (*led_action_get)(yt_unit_t, yt_port_t, yt_led_id_t, yt_led_act_cfg_t *);
    yt_ret_t (*led_blink_freq_set)(yt_unit_t, yt_port_t, yt_led_blink_event_t, yt_led_blink_freq_t);
    yt_ret_t (*led_blink_freq_get)(yt_unit_t, yt_port_t, yt_led_blink_event_t, yt_led_blink_freq_t *);
    yt_ret_t (*led_blink_duty_set)(yt_unit_t, yt_port_t, yt_led_blink_duty_t);
    yt_ret_t (*led_blink_duty_get)(yt_unit_t, yt_port_t, yt_led_blink_duty_t *);
    yt_ret_t (*led_loopdetect_blink_rate_set)(yt_unit_t unit, yt_led_loopdetect_blink_rate_t rate);
    yt_ret_t (*led_loopdetect_blink_rate_get)(yt_unit_t unit, yt_led_loopdetect_blink_rate_t *pRate);
    yt_ret_t (*led_force_mode_set)(yt_unit_t, yt_port_t, yt_led_id_t, yt_led_force_mode_t);
    yt_ret_t (*led_force_mode_get)(yt_unit_t, yt_port_t, yt_led_id_t, yt_led_force_mode_t *);
    yt_ret_t (*led_force_rate_set)(yt_unit_t, yt_port_t, yt_led_id_t, yt_led_force_rate_t);
    yt_ret_t (*led_force_rate_get)(yt_unit_t, yt_port_t, yt_led_id_t, yt_led_force_rate_t *);
    yt_ret_t (*led_serial_activeMode_set)(yt_unit_t, yt_sled_activeMode_t);
    yt_ret_t (*led_serial_activeMode_get)(yt_unit_t, yt_sled_activeMode_t *);
    yt_ret_t (*led_serial_remapping_set)(yt_unit_t, uint8_t, yt_led_remapping_t);
    yt_ret_t (*led_serial_remapping_get)(yt_unit_t, uint8_t, yt_led_remapping_t *);
    yt_ret_t (*led_serial_enable_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*led_serial_enable_get)(yt_unit_t, yt_enable_t *);
    yt_ret_t (*led_parallel_output_set)(yt_unit_t, yt_port_mask_t);
    yt_ret_t (*led_parallel_output_get)(yt_unit_t, yt_port_mask_t *);
    yt_ret_t (*led_parallel_remapping_set)(yt_unit_t, yt_led_remapping_t, yt_led_remapping_t);
    yt_ret_t (*led_parallel_remapping_get)(yt_unit_t, yt_led_remapping_t, yt_led_remapping_t *);
    yt_ret_t (*led_parallel_pos_invert_set)(yt_unit_t, yt_port_t, yt_led_id_t, yt_enable_t);
    yt_ret_t (*led_parallel_pos_invert_get)(yt_unit_t, yt_port_t, yt_led_id_t, yt_enable_t *);

	/* sensor */
    yt_ret_t (*sensor_temp_enable_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*sensor_temp_enable_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*sensor_temp_value_get)(yt_unit_t,  yt_bool_t *,  uint16_t *);
} fal_dispatch_t;

typedef struct fal_dispatch_info_s
{
    uint32_t chip_id;
    uint32_t device_id;
    fal_dispatch_t *pdispatch;
} fal_dispatch_info_t;

/*
 * Function Declaration
 */
extern fal_dispatch_t *gpfal_dispatch[YT_MAX_UNIT];

#define YT_DISPATCH(unit) gpfal_dispatch[unit]


extern uint32_t fal_dispatch_get(fal_dispatch_t **pdispatch, uint8_t unit);
extern yt_ret_t fal_func_unavailable(void);
extern uint32_t fal_dispatch_init(void);

#endif
