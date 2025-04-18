/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

/*
 * Include Files
 */
#include "yt_error.h"
#include "yt_types.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_dispatch.h"
#include "fal_tiger_dispatch.h"
#include "fal_tiger_vlan.h"
#include "fal_tiger_acl.h"
#include "fal_tiger_qos.h"
#include "fal_tiger_dos.h"
#include "fal_tiger_l2.h"
#include "fal_tiger_nic.h"
#include "fal_tiger_stp.h"
#include "fal_tiger_loopdetect.h"
#include "fal_tiger_multicast.h"
#include "fal_tiger_mirror.h"
#include "fal_tiger_lag.h"
#include "fal_tiger_rma.h"
#include "fal_tiger_port.h"
#include "fal_tiger_port_isolation.h"
#include "fal_tiger_dot1x.h"
#include "fal_tiger_oam.h"
#include "fal_tiger_stat.h"
#include "fal_tiger_storm_ctrl.h"
#include "fal_tiger_wol.h"
#include "fal_tiger_sys.h"
#include "fal_tiger_ctrlpkt.h"
#include "fal_tiger_rate.h"
#include "fal_tiger_port.h"
#include "fal_tiger_vlan_translate.h"
#include "fal_tiger_interrupt.h"
#include "fal_tiger_led.h"
#include "fal_tiger_sensor.h"
/*
 * Symbol Definition
 */

/*
 * Macro Declaration
 */

/*
 * Data Declaration
 */
fal_dispatch_t  fal_tiger_dispatch = 
{
#ifdef VLAN_INCLUDED
    .vlan_init = fal_tiger_vlan_init,
    .vlan_port_set = fal_tiger_vlan_port_set,
    .vlan_port_get = fal_tiger_vlan_port_get,
    .vlan_svlMode_enable_set = fal_tiger_vlan_svlMode_enable_set,
    .vlan_svlMode_enable_get = fal_tiger_vlan_svlMode_enable_get,
    .vlan_fid_set = fal_tiger_vlan_fid_set,
    .vlan_fid_get = fal_tiger_vlan_fid_get,
    .vlan_igrTpid_set = fal_tiger_vlan_ingrTpid_set,
    .vlan_igrTpid_get = fal_tiger_vlan_ingrTpid_get,
    .vlan_port_igrTpidSel_set = fal_tiger_vlan_port_ingrTpidMask_set,
    .vlan_port_igrTpidSel_get = fal_tiger_vlan_port_ingrTpidMask_get,
    .vlan_port_igrDefVlan_set = fal_tiger_vlan_port_ingrDefaultVlan_set,
    .vlan_port_igrDefVlan_get = fal_tiger_vlan_port_ingrDefaultVlan_get,
    .vlan_port_igrFilter_enable_set = fal_tiger_vlan_port_ingrFilter_enable_set,
    .vlan_port_igrFilter_enable_get = fal_tiger_vlan_port_ingrFilter_enable_get,
    .vlan_igrTransparent_set = fal_tiger_vlan_ingrTransparent_set,
    .vlan_igrTransparent_get = fal_tiger_vlan_ingrTransparent_get,
    .vlan_port_aft_set = fal_tiger_vlan_port_aft_set,
    .vlan_port_aft_get = fal_tiger_vlan_port_aft_get,
    .vlan_port_egrTagMode_set = fal_tiger_vlan_port_egrTagMode_set,
    .vlan_port_egrTagMode_get = fal_tiger_vlan_port_egrTagMode_get,
    .vlan_port_egrDefVid_set = fal_tiger_vlan_port_egrDefaultVid_set,
    .vlan_port_egrDefVid_get = fal_tiger_vlan_port_egrDefaultVid_get,
    .vlan_egrTpid_set = fal_tiger_vlan_egrTpid_set,
    .vlan_egrTpid_get = fal_tiger_vlan_egrTpid_get,
    .vlan_port_egrTpidSel_set = fal_tiger_vlan_port_egrTpidIdx_set,
    .vlan_port_egrTpidSel_get = fal_tiger_vlan_port_egrTpidIdx_get,
    .vlan_port_egrTransparent_set = fal_tiger_vlan_port_egrTransparent_set,
    .vlan_port_egrTransparent_get = fal_tiger_vlan_port_egrTransparent_get,
    .vlan_port_egrFilter_enable_set = fal_tiger_vlan_port_egrFilter_en_set,
    .vlan_port_egrFilter_enable_get = fal_tiger_vlan_port_egrFilter_en_get,
    .vlan_port_vidTypeSel_set = fal_tiger_port_vidTypeSel_set,
    .vlan_port_vidTypeSel_get = fal_tiger_port_vidTypeSel_get,
#endif

#ifdef VLAN_TRANS_INCLUDED
    .vlan_trans_untagPvidIgnore_set = fal_tiger_vlan_trans_untagPvidIgnore_set,
    .vlan_trans_untagPvidIgnore_get = fal_tiger_vlan_trans_untagPvidIgnore_get,
    .vlan_trans_rangeProfile_add = fal_tiger_vlan_trans_rangeProfile_add,
    .vlan_trans_rangeProfile_get = fal_tiger_vlan_trans_rangeProfile_get,
    .vlan_trans_rangeProfile_del = fal_tiger_vlan_trans_rangeProfile_del,
    .vlan_trans_port_rangeProfileSel_set = fal_tiger_vlan_trans_port_rangeProfileSel_set,
    .vlan_trans_port_rangeProfileSel_get = fal_tiger_vlan_trans_port_rangeProfileSel_get,
    .vlan_trans_mode_set = fal_tiger_vlan_trans_mode_set,
    .vlan_trans_mode_get = fal_tiger_vlan_trans_mode_get,
    .vlan_igr_trans_table_add = fal_tiger_vlan_igr_trans_table_add,
    .vlan_igr_trans_table_get = fal_tiger_vlan_igr_trans_table_get,
    .vlan_igr_trans_table_del = fal_tiger_vlan_igr_trans_table_del,
    .vlan_egr_trans_table_add = fal_tiger_vlan_egr_trans_table_add,
    .vlan_egr_trans_table_get = fal_tiger_vlan_egr_trans_table_get,
    .vlan_egr_trans_table_del = fal_tiger_vlan_egr_trans_table_del,
#endif

#ifdef DOS_INCLUDED
    .dos_init = fal_tiger_dos_init,
    .dos_port_en_set = fal_tiger_dos_port_en_set,
    .dos_port_en_get = fal_tiger_dos_port_en_get,
    .dos_drop_en_set = fal_tiger_dos_drop_en_set,
    .dos_drop_en_get = fal_tiger_dos_drop_en_get,
    .dos_large_icmp_size_set = fal_tiger_dos_large_icmp_size_set,
    .dos_large_icmp_size_get = fal_tiger_dos_large_icmp_size_get,
#endif

#ifdef ACL_INCLUDED
    .acl_init = fal_tiger_acl_init,
    .acl_port_en_set = fal_tiger_acl_port_en_set,
    .acl_port_en_get = fal_tiger_acl_port_en_get,
    .acl_unmatch_permit_en_set = fal_tiger_acl_unmatch_permit_en_set,
    .acl_unmatch_permit_en_get = fal_tiger_acl_unmatch_permit_en_get,
    .acl_udf_rule_set = fal_tiger_acl_udf_rule_set,
    .acl_rule_init = fal_tiger_acl_rule_init,
    .acl_rule_reset = fal_tiger_acl_rule_reset,
    .acl_rule_key_add = fal_tiger_acl_rule_key_add,
    .acl_rule_action_add = fal_tiger_acl_rule_action_add,
    .acl_rule_create = fal_tiger_acl_rule_create,
    .acl_rule_active = fal_tiger_acl_rule_active,
    .acl_rule_del = fal_tiger_acl_rule_del,
#endif

#ifdef DOT1X_INCLUDED
    .dot1x_init = fal_tiger_dot1x_init,
    .dot1x_portBasedEnable_set = fal_tiger_dot1x_portBasedEnable_set,
    .dot1x_portBasedEnable_get = fal_tiger_dot1x_portBasedEnable_get,
    .dot1x_portBasedAuthStatus_set = fal_tiger_dot1x_portBasedAuthStatus_set,
    .dot1x_portBasedAuthStatus_get = fal_tiger_dot1x_portBasedAuthStatus_get,
    .dot1x_portBasedDirection_set = fal_tiger_dot1x_portBasedDirection_set,
    .dot1x_portBasedDirection_get = fal_tiger_dot1x_portBasedDirection_get,
    .dot1x_guest_vlan_set = fal_tiger_dot1x_guest_vlan_set,
    .dot1x_guest_vlan_get = fal_tiger_dot1x_guest_vlan_get,
    .dot1x_tx_bypass_bc_set = fal_tiger_dot1x_tx_bypass_bc_set,
    .dot1x_tx_bypass_bc_get = fal_tiger_dot1x_tx_bypass_bc_get,
    .dot1x_tx_bypass_mc_set = fal_tiger_dot1x_tx_bypass_mc_set,
    .dot1x_tx_bypass_mc_get = fal_tiger_dot1x_tx_bypass_mc_get,
    .dot1x_rx_bypass_bc_set = fal_tiger_dot1x_rx_bypass_bc_set,
    .dot1x_rx_bypass_bc_get = fal_tiger_dot1x_rx_bypass_bc_get,
    .dot1x_rx_bypass_mc_set = fal_tiger_dot1x_rx_bypass_mc_set,
    .dot1x_rx_bypass_mc_get = fal_tiger_dot1x_rx_bypass_mc_get,
#endif

#ifdef OAM_INCLUDED
    .oam_en_set = fal_tiger_oam_en_set,
    .oam_en_get = fal_tiger_oam_en_get,
    .oam_bypass_accessctrl_set = fal_tiger_oam_bypass_accessctrl_set,
    .oam_bypass_accessctrl_get = fal_tiger_oam_bypass_accessctrl_get,
    .oam_parser_act_set = fal_tiger_oam_parser_act_set,
    .oam_parser_act_get = fal_tiger_oam_parser_act_get,
    .oam_mux_act_set = fal_tiger_oam_mux_act_set,
    .oam_mux_act_get = fal_tiger_oam_mux_act_get,
#endif

#ifdef QOS_INCLUDED
    .qos_intPri_map_weight_set = fal_tiger_qos_intPri_map_weight_set,
    .qos_intPri_map_weight_get = fal_tiger_qos_intPri_map_weight_get,
    .qos_intPri_portDefPri_set  = fal_tiger_qos_intPri_portDefPri_set ,
    .qos_intPri_portDefPri_get  = fal_tiger_qos_intPri_portDefPri_get ,
    .qos_intPri_cpri_map_set = fal_tiger_qos_intPri_cpri_map_set,
    .qos_intPri_cpri_map_get = fal_tiger_qos_intPri_cpri_map_get,
    .qos_intDP_cpri_map_set = fal_tiger_qos_intDP_cpri_map_set,
    .qos_intDP_cpri_map_get = fal_tiger_qos_intDP_cpri_map_get,
    .qos_intPri_spri_map_set = fal_tiger_qos_intPri_spri_map_set,
    .qos_intPri_spri_map_get = fal_tiger_qos_intPri_spri_map_get,
    .qos_intDP_spri_map_set = fal_tiger_qos_intDP_spri_map_set,
    .qos_intDP_spri_map_get = fal_tiger_qos_intDP_spri_map_get,
    .qos_intPri_dscp_map_set = fal_tiger_qos_intPri_dscp_map_set,
    .qos_intPri_dscp_map_get = fal_tiger_qos_intPri_dscp_map_get,
    .qos_intDP_dscp_map_set = fal_tiger_qos_intDP_dscp_map_set,
    .qos_intDP_dscp_map_get = fal_tiger_qos_intDP_dscp_map_get,
    .qos_intPri_map_igrMirror_set = fal_tiger_qos_intPri_map_igrMirror_set,
    .qos_intPri_map_igrMirror_get = fal_tiger_qos_intPri_map_igrMirror_get,
    .qos_intPri_map_egrMirror_set = fal_tiger_qos_intPri_map_egrMirror_set,
    .qos_intPri_map_egrMirror_get = fal_tiger_qos_intPri_map_egrMirror_get,
    .qos_que_map_ucast_set = fal_tiger_qos_que_map_ucast_set,
    .qos_que_map_ucast_get = fal_tiger_qos_que_map_ucast_get,
    .qos_que_map_mcast_set = fal_tiger_qos_que_map_mcast_set,
    .qos_que_map_mcast_get = fal_tiger_qos_que_map_mcast_get,
    .qos_que_forceDrop_enable_set = fal_tiger_qos_que_forceDrop_enable_set,
    .qos_que_forceDrop_enable_get = fal_tiger_qos_que_forceDrop_enable_get,
    .qos_remark_port_set = fal_tiger_qos_remark_port_set,
    .qos_remark_port_get = fal_tiger_qos_remark_port_get,
    .qos_remark_dscp_set = fal_tiger_qos_remark_dscp_set,
    .qos_remark_dscp_get = fal_tiger_qos_remark_dscp_get,
    .qos_remark_cpri_set = fal_tiger_qos_remark_cpri_set,
    .qos_remark_cpri_get = fal_tiger_qos_remark_cpri_get,
    .qos_remark_spri_set = fal_tiger_qos_remark_spri_set,
    .qos_remark_spri_get = fal_tiger_qos_remark_spri_get,
    .qos_schedule_sp_set = fal_tiger_qos_schedule_sp_set,
    .qos_schedule_sp_get = fal_tiger_qos_schedule_sp_get,
    .qos_schedule_dwrr_mode_set = fal_tiger_qos_schedule_dwrr_mode_set,
    .qos_schedule_dwrr_mode_get = fal_tiger_qos_schedule_dwrr_mode_get,
    .qos_schedule_dwrr_set = fal_tiger_qos_schedule_dwrr_set,
    .qos_schedule_dwrr_get = fal_tiger_qos_schedule_dwrr_get,
    .qos_intPri_vlan_map_set = fal_tiger_qos_intPri_vlan_map_set,
    .qos_intPri_vlan_map_get = fal_tiger_qos_intPri_vlan_map_get,
#endif

#ifdef RATE_INCLUDED
    .rate_init = fal_tiger_rate_init,
    .rate_igrBandwidthCtrlEnable_set = fal_tiger_rate_igrBandwidthCtrlEnable_set,
    .rate_igrBandwidthCtrlEnable_get = fal_tiger_rate_igrBandwidthCtrlEnable_get,
    .rate_igrBandwidthCtrlMode_set = fal_tiger_rate_igrBandwidthCtrlMode_set,
    .rate_igrBandwidthCtrlMode_get = fal_tiger_rate_igrBandwidthCtrlMode_get,
    .rate_igrBandwidthCtrlRate_set = fal_tiger_rate_igrBandwidthCtrlRate_set,
    .rate_igrBandwidthCtrlRate_get = fal_tiger_rate_igrBandwidthCtrlRate_get,
    .rate_meter_vlan_enable_set = fal_tiger_rate_meter_vlan_enable_set,
    .rate_meter_vlan_enable_get = fal_tiger_rate_meter_vlan_enable_get,
    .rate_meter_enable_set = fal_tiger_rate_meter_enable_set,
    .rate_meter_enable_get = fal_tiger_rate_meter_enable_get,
    .rate_meter_mode_set = fal_tiger_rate_meter_mode_set,
    .rate_meter_mode_get = fal_tiger_rate_meter_mode_get,
    .rate_meter_rate_set = fal_tiger_rate_meter_rate_set,
    .rate_meter_rate_get = fal_tiger_rate_meter_rate_get,
    .rate_shaping_port_enable_set = fal_tiger_rate_shaping_port_enable_set,
    .rate_shaping_port_enable_get = fal_tiger_rate_shaping_port_enable_get,
    .rate_shaping_port_mode_set = fal_tiger_rate_shaping_port_mode_set,
    .rate_shaping_port_mode_get = fal_tiger_rate_shaping_port_mode_get,
    .rate_shaping_port_rate_set = fal_tiger_rate_shaping_port_rate_set,
    .rate_shaping_port_rate_get = fal_tiger_rate_shaping_port_rate_get,
    .rate_shaping_queue_enable_set = fal_tiger_rate_shaping_queue_enable_set,
    .rate_shaping_queue_enable_get = fal_tiger_rate_shaping_queue_enable_get,
    .rate_shaping_queue_mode_set = fal_tiger_rate_shaping_queue_mode_set,
    .rate_shaping_queue_mode_get = fal_tiger_rate_shaping_queue_mode_get,
    .rate_shaping_queue_rate_set = fal_tiger_rate_shaping_queue_rate_set,
    .rate_shaping_queue_rate_get = fal_tiger_rate_shaping_queue_rate_get,
#endif

#ifdef STAT_INCLUDED
    .stat_mib_init = fal_tiger_stat_mib_init,    
    .stat_mib_enable_set = fal_tiger_stat_mib_enable_set,   
    .stat_mib_enable_get = fal_tiger_stat_mib_enable_get,   
    .stat_mib_clear = fal_tiger_stat_mib_clear,    
    .stat_mib_clear_all = fal_tiger_stat_mib_clear_all,    
    .stat_mib_port_get = fal_tiger_stat_mib_port_get,    
    .stat_flow_enable_set = fal_tiger_stat_flow_enable_set, 
    .stat_flow_enable_get = fal_tiger_stat_flow_enable_get,
    .stat_flow_mode_set = fal_tiger_stat_flow_mode_set,    
    .stat_flow_mode_get = fal_tiger_stat_flow_mode_get,    
    .stat_flow_count_set = fal_tiger_stat_flow_count_set,   
    .stat_flow_count_get = fal_tiger_stat_flow_count_get,
#endif

#ifdef HW_STP_INCLUDED
    .stp_state_set = fal_tiger_stp_state_set,
    .stp_state_get = fal_tiger_stp_state_get,
    .stp_instance_map_set = fal_tiger_stp_instance_map_set,
    .stp_instance_map_get = fal_tiger_stp_instance_map_get,
#endif

#ifdef PORT_ISO_INCLUDED
    .port_isolation_set = fal_tiger_port_isolation_set,
    .port_isolation_get = fal_tiger_port_isolation_get,
#endif

#ifdef HW_LOOPDETECT_INCLUDED
    .loop_detect_enable_set = fal_tiger_loop_detect_enable_set,
    .loop_detect_enable_get = fal_tiger_loop_detect_enable_get,
    .loop_detect_tpid_set = fal_tiger_loop_detect_tpid_set,
    .loop_detect_tpid_get = fal_tiger_loop_detect_tpid_get,
    .loop_detect_generate_way_set = fal_tiger_loop_detect_generate_way_set,
    .loop_detect_generate_way_get = fal_tiger_loop_detect_generate_way_get,
    .loop_detect_unitID_set = fal_tiger_loop_detect_unitID_set,
    .loop_detect_unitID_get = fal_tiger_loop_detect_unitID_get,
#endif

#ifdef STORM_CTRL_INCLUDED
    .storm_ctrl_init = fal_tiger_storm_ctrl_init,
    .storm_ctrl_enable_set = fal_tiger_storm_ctrl_enable_set,
    .storm_ctrl_enable_get = fal_tiger_storm_ctrl_enable_get,
    .storm_ctrl_rate_mode_set = fal_tiger_storm_ctrl_rate_mode_set,
    .storm_ctrl_rate_mode_get = fal_tiger_storm_ctrl_rate_mode_get,
    .storm_ctrl_rate_include_gap_set = fal_tiger_storm_ctrl_rate_include_gap_set,
    .storm_ctrl_rate_include_gap_get = fal_tiger_storm_ctrl_rate_include_gap_get,
    .storm_ctrl_rate_set = fal_tiger_storm_ctrl_rate_set,
    .storm_ctrl_rate_get = fal_tiger_storm_ctrl_rate_get,
#endif

#ifdef LAG_INCLUDED
    .lag_hash_sel_set = fal_tiger_lag_hash_sel_set,
    .lag_hash_sel_get = fal_tiger_lag_hash_sel_get,
    .lag_group_port_set = fal_tiger_lag_group_port_set,
    .lag_group_info_get = fal_tiger_lag_group_info_get,
#endif

#ifdef L2_INCLUDED
    .l2_init = fal_tiger_l2_init,
    .l2_mcast_addr_add = fal_tiger_l2_fdb_mcast_addr_add,
    .l2_fdb_ucast_addr_add = fal_tiger_l2_fdb_ucast_addr_add,
    .l2_fdb_ucast_addr_del = fal_tiger_l2_fdb_ucast_addr_del,
    .l2_mcast_addr_del = fal_tiger_l2_fdb_mcast_addr_del,
    .l2_fdb_linkdownFlush_en_set = fal_tiger_l2_fdb_linkdownFlush_en_set,
    .l2_fdb_linkdownFlush_en_get = fal_tiger_l2_fdb_linkdownFlush_en_get,
    .l2_fdb_all_ucast_flush = fal_tiger_l2_fdb_all_ucast_flush,
    .l2_fdb_port_ucast_flush = fal_tiger_l2_fdb_port_ucast_flush,
    .l2_fdb_vlan_ucast_flush = fal_tiger_l2_fdb_vlan_ucast_flush,
    .l2_all_mcast_flush = fal_tiger_l2_fdb_all_mcast_flush,
    .l2_port_mcast_flush = fal_tiger_l2_fdb_port_mcast_flush,
    .l2_vlan_mcast_flush = fal_tiger_l2_fdb_vlan_mcast_flush,
    .l2_fdb_type_get = fal_tiger_l2_fdb_type_get,
    .l2_port_learnlimit_en_set = fal_tiger_l2_port_learnlimit_en_set,
    .l2_port_learnlimit_en_get = fal_tiger_l2_port_learnlimit_en_get,
    .l2_port_learnlimit_cnt_set = fal_tiger_l2_port_learnlimit_cnt_set,
    .l2_port_learnlimit_cnt_get = fal_tiger_l2_port_learnlimit_cnt_get,
    .l2_port_learnlimit_exceed_drop_set = fal_tiger_l2_port_learnlimit_exceed_drop_set,
    .l2_port_learnlimit_exceed_drop_get = fal_tiger_l2_port_learnlimit_exceed_drop_get,
    .l2_system_learnlimit_en_set = fal_tiger_l2_system_learnlimit_en_set,
    .l2_system_learnlimit_en_get = fal_tiger_l2_system_learnlimit_en_get,
    .l2_system_learnlimit_cnt_set = fal_tiger_l2_system_learnlimit_cnt_set,
    .l2_system_learnlimit_cnt_get = fal_tiger_l2_system_learnlimit_cnt_get,
    .l2_system_learnlimit_exceed_drop_set = fal_tiger_l2_system_learnlimit_exceed_drop_set,
    .l2_system_learnlimit_exceed_drop_get = fal_tiger_l2_system_learnlimit_exceed_drop_get,
    .l2_fdb_lag_uc_cnt_get = fal_tiger_l2_fdb_lag_uc_cnt_get,
    .l2_lag_learnlimit_en_set = fal_tiger_l2_lag_learnlimit_en_set,
    .l2_lag_learnlimit_en_get = fal_tiger_l2_lag_learnlimit_en_get,
    .l2_lag_learnlimit_cnt_set = fal_tiger_l2_lag_learnlimit_cnt_set,
    .l2_lag_learnlimit_cnt_get = fal_tiger_l2_lag_learnlimit_cnt_get,
    .l2_lag_learnlimit_exceed_drop_set = fal_tiger_l2_lag_learnlimit_exceed_drop_set,
    .l2_lag_learnlimit_exceed_drop_get = fal_tiger_l2_lag_learnlimit_exceed_drop_get,
    .l2_fdb_drop_sa_set = fal_tiger_l2_fdb_drop_sa_set,
    .l2_fdb_drop_sa_get = fal_tiger_l2_fdb_drop_sa_get,
    .l2_fdb_drop_da_set = fal_tiger_l2_fdb_drop_da_set,
    .l2_fdb_drop_da_get = fal_tiger_l2_fdb_drop_da_get,
    .l2_fdb_copy2cpu_set = fal_tiger_l2_fdb_copy2cpu_set,
    .l2_fdb_copy2cpu_get = fal_tiger_l2_fdb_copy2cpu_get,
    .l2_filter_mcast_set = fal_tiger_l2_filter_mcast_set,
    .l2_filter_mcast_get = fal_tiger_l2_filter_mcast_get,
    .l2_filter_bcast_set = fal_tiger_l2_filter_bcast_set,
    .l2_filter_bcast_get = fal_tiger_l2_filter_bcast_get,
    .l2_filter_unknown_ucast_set = fal_tiger_l2_filter_unknown_ucast_set,
    .l2_filter_unknown_ucast_get = fal_tiger_l2_filter_unknown_ucast_get,
    .l2_filter_unknown_mcast_set = fal_tiger_l2_filter_unknown_mcast_set,
    .l2_filter_unknown_mcast_get = fal_tiger_l2_filter_unknown_mcast_get,
    .l2_rma_bypass_unknown_mcast_filter_set = fal_tiger_l2_rma_bypass_unknown_mcast_filter_set,
    .l2_rma_bypass_unknown_mcast_filter_get = fal_tiger_l2_rma_bypass_unknown_mcast_filter_get,
    .l2_fdb_port_uc_cnt_get = fal_tiger_l2_fdb_port_uc_cnt_get,
    .l2_fdb_uc_cnt_get = fal_tiger_l2_fdb_uc_cnt_get,
    .l2_mc_cnt_get = fal_tiger_l2_fdb_mc_cnt_get,
    .l2_fdb_aging_port_en_set = fal_tiger_l2_fdb_aging_port_en_set,
    .l2_fdb_aging_port_en_get = fal_tiger_l2_fdb_aging_port_en_get,
    .l2_fdb_aging_time_set = fal_tiger_l2_fdb_aging_time_set,
    .l2_fdb_aging_time_get = fal_tiger_l2_fdb_aging_time_get,
    .l2_fdb_uc_withindex_get = fal_tiger_l2_fdb_uc_withindex_get,
    .l2_fdb_uc_withMacAndVid_get = fal_tiger_l2_fdb_uc_withMacAndVid_get,
    .l2_fdb_uc_withindex_getnext = fal_tiger_l2_fdb_uc_withindex_getnext,
    .l2_port_learn_en_set = fal_tiger_l2_port_learn_en_set,
    .l2_port_learn_en_get = fal_tiger_l2_port_learn_en_get,
#endif

#ifdef HW_MULTICAST_INCLUDED
    .multicast_igmp_opmode_set = fal_tiger_multicast_igmp_opmode_set,
    .multicast_igmp_opmode_get = fal_tiger_multicast_igmp_opmode_get,
    .multicast_mld_opmode_set = fal_tiger_multicast_mld_opmode_set,
    .multicast_mld_opmode_get = fal_tiger_multicast_mld_opmode_get,
    .multicast_port_report_allow_set = fal_tiger_multicast_port_report_allow_set,
    .multicast_port_report_allow_get = fal_tiger_multicast_port_report_allow_get,
    .multicast_port_leave_allow_set = fal_tiger_multicast_port_leave_allow_set,
    .multicast_port_leave_allow_get = fal_tiger_multicast_port_leave_allow_get,
    .multicast_port_query_allow_set = fal_tiger_multicast_port_query_allow_set,
    .multicast_port_query_allow_get = fal_tiger_multicast_port_query_allow_get,
    .multicast_fastleave_set = fal_tiger_multicast_fastleave_set,
    .multicast_fastleave_get = fal_tiger_multicast_fastleave_get,
    .multicast_learnlimit_en_set = fal_tiger_multicast_learnlimit_en_set,
    .multicast_learnlimit_en_get = fal_tiger_multicast_learnlimit_en_get,
    .multicast_learnlimit_maxgroup_set = fal_tiger_multicast_learnlimit_maxgroup_set,
    .multicast_learnlimit_maxgroup_get = fal_tiger_multicast_learnlimit_maxgroup_get,
    .multicast_fwd_routerport_only_set = fal_tiger_multicast_fwd_routerport_only_set,
    .multicast_fwd_routerport_only_get = fal_tiger_multicast_fwd_routerport_only_get,
    .multicast_fwd_routerport_primary_set = fal_tiger_multicast_fwd_routerport_primary_set,
    .multicast_fwd_routerport_primary_get = fal_tiger_multicast_fwd_routerport_primary_get,
    .multicast_bypass_grouprange_set = fal_tiger_multicast_bypass_grouprange_set,
    .multicast_bypass_grouprange_get = fal_tiger_multicast_bypass_grouprange_get,
    .multicast_dynamic_routerport_get = fal_tiger_multicast_dynamic_routerport_get,
    .multicast_routerport_dynamic_allow_set = fal_tiger_multicast_routerport_dynamic_allow_set,
    .multicast_routerport_dynamic_allow_get = fal_tiger_multicast_routerport_dynamic_allow_get,
    .multicast_dynamic_routerport_agingtime_set = fal_tiger_multicast_dynamic_routerport_agingtime_set,
    .multicast_dynamic_routerport_agingtime_get = fal_tiger_multicast_dynamic_routerport_agingtime_get,
    .multicast_vlan_add = fal_tiger_multicast_vlan_add,
    .multicast_vlan_get = fal_tiger_multicast_vlan_get,
    .multicast_vlan_del = fal_tiger_multicast_vlan_del,
    .multicast_static_routerport_set = fal_tiger_multicast_static_routerport_set,
    .multicast_static_routerport_get = fal_tiger_multicast_static_routerport_get,
    .multicast_igmp_bypass_port_isolation_set = fal_tiger_multicast_igmp_bypass_port_isolation_set,
    .multicast_igmp_bypass_port_isolation_get = fal_tiger_multicast_igmp_bypass_port_isolation_get,
    .multicast_ipmc_bypass_port_isolation_set = fal_tiger_multicast_ipmc_bypass_port_isolation_set,
    .multicast_ipmc_bypass_port_isolation_get = fal_tiger_multicast_ipmc_bypass_port_isolation_get,
    .multicast_port_igmp_bypass_ingrfilter_en_set = fal_tiger_multicast_port_igmp_bypass_ingrfilter_en_set,
    .multicast_port_igmp_bypass_ingrfilter_en_get = fal_tiger_multicast_port_igmp_bypass_ingrfilter_en_get,
#endif

#ifdef NIC_INCLUDED
    .nic_init = fal_tiger_nic_init,
    .nic_cpuport_mode_set = fal_tiger_nic_cpuport_mode_set,
    .nic_cpuport_mode_get = fal_tiger_nic_cpuport_mode_get,
    .nic_ext_cpuport_en_set = fal_tiger_nic_ext_cpuport_en_set,
    .nic_ext_cpuport_en_get = fal_tiger_nic_ext_cpuport_en_get,
    .nic_ext_cpuport_port_set = fal_tiger_nic_ext_cpuport_port_set,
    .nic_ext_cpuport_port_get = fal_tiger_nic_ext_cpuport_port_get,
    .nic_cpuport_tagtpid_set = fal_tiger_nic_cpuport_tagtpid_set,
    .nic_cpuport_tagtpid_get = fal_tiger_nic_cpuport_tagtpid_get,
    .nic_ext_cputag_en_set = fal_tiger_nic_ext_cputag_en_set,
    .nic_ext_cputag_en_get = fal_tiger_nic_ext_cputag_en_get,
#endif

#ifdef PORT_MIRROR_INCLUDED
    .mirror_port_set = fal_tiger_mirror_port_set,
    .mirror_port_get = fal_tiger_mirror_port_get,
#endif

#ifdef RMA_INCLUDED
    .rma_action_set = fal_tiger_rma_action_set,
    .rma_action_get = fal_tiger_rma_action_get,
    .rma_bypass_port_isolation_set = fal_tiger_rma_bypass_port_isolation_set,
    .rma_bypass_port_isolation_get = fal_tiger_rma_bypass_port_isolation_get,
    .rma_bypass_vlan_filter_set = fal_tiger_rma_bypass_vlan_filter_set,
    .rma_bypass_vlan_filter_get = fal_tiger_rma_bypass_vlan_filter_get,
#endif

#ifdef WOL_INCLUDED
    .wol_ctrl_set = fal_tiger_wol_ctrl_set,
    .wol_ctrl_get = fal_tiger_wol_ctrl_get,
    .wol_ethertype_set = fal_tiger_wol_ethertype_set,
    .wol_ethertype_get = fal_tiger_wol_ethertype_get,
#endif

#ifdef SYS_INCLUDED
    .sys_mac_get = fal_tiger_sys_mac_get,
    .sys_chip_reset = fal_tiger_sys_chip_reset,
    .sys_database_reset = fal_tiger_sys_database_reset,
    .sys_chipInfo_get = fal_tiger_sys_chipInfo_get,
#endif

#ifdef PORT_INCLUDED
    .port_init = fal_tiger_port_init,
    .port_enable_set = fal_tiger_port_enable_set,
    .port_enable_get = fal_tiger_port_enable_get,
    .port_link_status_get = fal_tiger_port_link_status_get,
    .port_link_status_all_get = fal_tiger_port_link_status_all_get,
    .port_backpress_enable_set = fal_tiger_port_backpress_enable_set,
    .port_backpress_enable_get = fal_tiger_port_backpress_enable_get,
    .port_cascade_set = fal_tiger_port_cascade_set,
    .port_cascade_get = fal_tiger_port_cascade_get,
    .port_pkt_gap_set = fal_tiger_port_pkt_gap_set,
    .port_pkt_gap_get = fal_tiger_port_pkt_gap_get,
    .port_macAutoNeg_enable_set = fal_tiger_port_macAutoNeg_enable_set,
    .port_macAutoNeg_enable_get = fal_tiger_port_macAutoNeg_enable_get,
    .port_mac_force_set = fal_tiger_port_mac_force_set,
    .port_mac_force_get = fal_tiger_port_mac_force_get,
    .port_mac_fc_set = fal_tiger_port_mac_fc_set,
    .port_mac_fc_get = fal_tiger_port_mac_fc_get,
    .port_extif_mode_set = fal_tiger_port_extif_mode_set,
    .port_extif_mode_get = fal_tiger_port_extif_mode_get,
    .port_extif_rgmii_delay_set = fal_tiger_port_extif_rgmii_delay_set,
    .port_extif_rgmii_delay_get = fal_tiger_port_extif_rgmii_delay_get,
    .port_phyAutoNeg_enable_set = fal_tiger_port_phyAutoNeg_enable_set,
    .port_phyAutoNeg_enable_get = fal_tiger_port_phyAutoNeg_enable_get,
    .port_phyAutoNeg_ability_set = fal_tiger_port_phyAutoNeg_ability_set,
    .port_phyAutoNeg_ability_get = fal_tiger_port_phyAutoNeg_ability_get,
    .port_phy_force_set = fal_tiger_port_phy_force_set,
    .port_phy_force_get = fal_tiger_port_phy_force_get,
    .port_phy_linkstatus_get = fal_tiger_port_phy_linkstatus_get,
    .port_phy_reg_set = fal_tiger_port_phy_reg_set,
    .port_phy_reg_get = fal_tiger_port_phy_reg_get,
    .port_eee_enable_set = fal_tiger_port_eee_enable_set,
    .port_eee_enable_get = fal_tiger_port_eee_enable_get,
    .port_jumbo_enable_set = fal_tiger_port_jumbo_enable_set,
    .port_jumbo_enable_get = fal_tiger_port_jumbo_enable_get,
    .port_cable_diag = fal_tiger_port_cable_diag,
    .port_phyTemplate_test_set = fal_tiger_port_phyTemplate_test_set,
#endif

#ifdef CTRLPKT_INCLUDED
    .ctrlpkt_unknown_ucast_act_set = fal_tiger_ctrlpkt_unknown_ucast_act_set,
    .ctrlpkt_unknown_ucast_act_get = fal_tiger_ctrlpkt_unknown_ucast_act_get,
    .ctrlpkt_unknown_mcast_act_set = fal_tiger_ctrlpkt_unknown_mcast_act_set,
    .ctrlpkt_unknown_mcast_act_get = fal_tiger_ctrlpkt_unknown_mcast_act_get,
    .ctrlpkt_arp_act_set = fal_tiger_ctrlpkt_arp_act_set,
    .ctrlpkt_arp_act_get = fal_tiger_ctrlpkt_arp_act_get,
    .ctrlpkt_nd_act_set = fal_tiger_ctrlpkt_nd_act_set,
    .ctrlpkt_nd_act_get = fal_tiger_ctrlpkt_nd_act_get,
    .ctrlpkt_lldp_eee_act_set = fal_tiger_ctrlpkt_lldp_eee_act_set,
    .ctrlpkt_lldp_eee_act_get = fal_tiger_ctrlpkt_lldp_eee_act_get,
    .ctrlpkt_lldp_act_set = fal_tiger_ctrlpkt_lldp_act_set,
    .ctrlpkt_lldp_act_get = fal_tiger_ctrlpkt_lldp_act_get,
#endif

#ifdef INTERRUPT_INCLUDED
    .int_polarity_set = fal_tiger_int_polarity_set,
    .int_polarity_get = fal_tiger_int_polarity_get,
    .int_control_set = fal_tiger_int_control_set,
    .int_control_get = fal_tiger_int_control_get,
    .int_status_get = fal_tiger_int_status_get,
#endif

#ifdef LED_INCLUDED
    .led_enable = fal_tiger_led_enable,
    .led_mode_set= fal_tiger_led_mode_set,
    .led_mode_get= fal_tiger_led_mode_get,
    .led_action_set= fal_tiger_led_action_set,
    .led_action_get= fal_tiger_led_action_get,
    .led_blink_freq_set= fal_tiger_led_blink_freq_set,
    .led_blink_freq_get= fal_tiger_led_blink_freq_get,
    .led_blink_duty_set= fal_tiger_led_blink_duty_set,
    .led_blink_duty_get= fal_tiger_led_blink_duty_get,
    .led_loopdetect_blink_rate_set= fal_tiger_led_loopdetect_blink_rate_set,
    .led_loopdetect_blink_rate_get= fal_tiger_led_loopdetect_blink_rate_get,
    .led_force_mode_set= fal_tiger_led_force_mode_set,
    .led_force_mode_get= fal_tiger_led_force_mode_get,
    .led_force_rate_set= fal_tiger_led_force_rate_set,
    .led_force_rate_get= fal_tiger_led_force_rate_get,
    .led_serial_activeMode_set= fal_tiger_led_serial_activeMode_set,
    .led_serial_activeMode_get= fal_tiger_led_serial_activeMode_get,
    .led_serial_remapping_set= fal_tiger_led_serial_remapping_set,
    .led_serial_remapping_get= fal_tiger_led_serial_remapping_get,
    .led_serial_enable_set= fal_tiger_led_serial_enable_set,
    .led_serial_enable_get= fal_tiger_led_serial_enable_get,
    .led_parallel_output_set= fal_tiger_led_parallel_output_set,
    .led_parallel_output_get= fal_tiger_led_parallel_output_get,
    .led_parallel_remapping_set= fal_tiger_led_parallel_remapping_set,
    .led_parallel_remapping_get= fal_tiger_led_parallel_remapping_get,
    .led_parallel_pos_invert_set= fal_tiger_led_parallel_pos_invert_set,
    .led_parallel_pos_invert_get= fal_tiger_led_parallel_pos_invert_get,
#endif

#ifdef SENSOR_INCLUDED
    .sensor_temp_enable_set = fal_tiger_sensor_temp_enable_set,
    .sensor_temp_enable_get = fal_tiger_sensor_temp_enable_get,
    .sensor_temp_value_get = fal_tiger_sensor_temp_value_get,
#endif
};
