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
#include "fal_tiger_igmp_mld.h"
#include "fal_tiger_mirror.h"
#include "fal_tiger_lag.h"
#include "fal_tiger_rma.h"
#include "fal_tiger_port.h"
#include "fal_tiger_isolation.h"
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
#include "fal_tiger_debug.h"
#include "fal_tiger_exit.h"
#include "fal_tiger_gpio.h"

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
    .vlan_port_igrPvid_set = fal_tiger_vlan_port_ingrDefaultVlan_set,
    .vlan_port_igrPvid_get = fal_tiger_vlan_port_ingrDefaultVlan_get,
    .vlan_port_igrDefPri_set = fal_tiger_vlan_port_igrDefPri_set,
    .vlan_port_igrDefPri_get = fal_tiger_vlan_port_igrDefPri_get,
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
    .vlan_protocolBasedVlan_group_set = fal_tiger_vlan_protocolBasedVlan_group_set,
    .vlan_protocolBasedVlan_group_get = fal_tiger_vlan_protocolBasedVlan_group_get,
    .vlan_protocolBasedVlan_table_add = fal_tiger_vlan_protocolBasedVlan_table_add,
    .vlan_protocolBasedVlan_table_get = fal_tiger_vlan_protocolBasedVlan_table_get,
    .vlan_protocolBasedVlan_table_del = fal_tiger_vlan_protocolBasedVlan_table_del,
    .vlan_mac_ipSub_tbl_arrange_set = fal_tiger_vlan_mac_ipSub_tbl_arrange_set,
    .vlan_mac_ipSub_tbl_arrange_get = fal_tiger_vlan_mac_ipSub_tbl_arrange_get,
    .vlan_macBasedVlan_table_add = fal_tiger_vlan_macBasedVlan_table_add,
    .vlan_macBasedVlan_table_add_by_index = fal_tiger_vlan_macBasedVlan_table_add_by_index,
    .vlan_macBasedVlan_table_get = fal_tiger_vlan_macBasedVlan_table_get,
    .vlan_macBasedVlan_table_del = fal_tiger_vlan_macBasedVlan_table_del,
    .vlan_ipSubnetBasedVlan_table_add = fal_tiger_vlan_ipSubnetBasedVlan_table_add,
    .vlan_ipSubnetBasedVlan_table_add_by_index = fal_tiger_vlan_ipSubnetBasedVlan_table_add_by_index,
    .vlan_ipSubnetBasedVlan_table_get = fal_tiger_vlan_ipSubnetBasedVlan_table_get,
    .vlan_ipSubnetBasedVlan_table_del = fal_tiger_vlan_ipSubnetBasedVlan_table_del,
    .vlan_ipSubnetBasedVlan_bypass_arp_set = fal_tiger_vlan_ipSubnetBasedVlan_bypass_arp_set,
    .vlan_ipSubnetBasedVlan_bypass_arp_get = fal_tiger_vlan_ipSubnetBasedVlan_bypass_arp_get,
    .vlan_policy_idx_mapping_set = fal_tiger_vlan_policy_idx_mapping_set,
    .vlan_policy_idx_mapping_get = fal_tiger_vlan_policy_idx_mapping_get,
    .vlan_policy_table_add = fal_tiger_vlan_policy_table_add,
    .vlan_policy_table_add_by_index = fal_tiger_vlan_policy_table_add_by_index,
    .vlan_policy_table_get = fal_tiger_vlan_policy_table_get,
    .vlan_policy_table_del = fal_tiger_vlan_policy_table_del,
#endif

#ifdef VLAN_TRANS_INCLUDED
    /*Translate*/
    .vlan_igr_trans_untagPvidIgnore_set = fal_tiger_vlan_trans_untagPvidIgnore_set,
    .vlan_igr_trans_untagPvidIgnore_get = fal_tiger_vlan_trans_untagPvidIgnore_get,
    .vlan_igr_trans_prio_tagPvidIgnore_set = fal_tiger_vlan_igr_trans_prio_tagPvidIgnore_set,
    .vlan_igr_trans_prio_tagPvidIgnore_get = fal_tiger_vlan_igr_trans_prio_tagPvidIgnore_get,
    .vlan_igr_trans_lookup_miss_ctrl_set = fal_tiger_vlan_igr_trans_lookup_miss_ctrl_set,
    .vlan_igr_trans_lookup_miss_ctrl_get = fal_tiger_vlan_igr_trans_lookup_miss_ctrl_get,
    .vlan_igr_trans_rangeProfile_add = fal_tiger_vlan_trans_rangeProfile_add,
    .vlan_igr_trans_rangeProfile_get = fal_tiger_vlan_trans_rangeProfile_get,
    .vlan_igr_trans_rangeProfile_del = fal_tiger_vlan_trans_rangeProfile_del,
    .vlan_igr_trans_port_rangeProfileSel_set = fal_tiger_vlan_trans_port_rangeProfileSel_set,
    .vlan_igr_trans_port_rangeProfileSel_get = fal_tiger_vlan_trans_port_rangeProfileSel_get,
    .vlan_igr_trans_mode_set = fal_tiger_vlan_trans_mode_set,
    .vlan_igr_trans_mode_get = fal_tiger_vlan_trans_mode_get,
    .vlan_igr_trans_table_add = fal_tiger_vlan_igr_trans_table_add,
    .vlan_igr_trans_table_add_by_index = fal_tiger_vlan_igr_trans_table_add_by_index,
    .vlan_igr_trans_table_get = fal_tiger_vlan_igr_trans_table_get,
    .vlan_igr_trans_table_del = fal_tiger_vlan_igr_trans_table_del,
    .vlan_egr_trans_lookup_miss_ctrl_set = fal_tiger_vlan_egr_trans_lookup_miss_ctrl_set,
    .vlan_egr_trans_lookup_miss_ctrl_get = fal_tiger_vlan_egr_trans_lookup_miss_ctrl_get,
    .vlan_egr_trans_rangeProfile_add = fal_tiger_vlan_egr_trans_rangeProfile_add,
    .vlan_egr_trans_rangeProfile_get = fal_tiger_vlan_egr_trans_rangeProfile_get,
    .vlan_egr_trans_rangeProfile_del = fal_tiger_vlan_egr_trans_rangeProfile_del,
    .vlan_egr_trans_table_add = fal_tiger_vlan_egr_trans_table_add,
    .vlan_egr_trans_table_add_by_index = fal_tiger_vlan_egr_trans_table_add_by_index,
    .vlan_egr_trans_table_get = fal_tiger_vlan_egr_trans_table_get,
    .vlan_egr_trans_table_del = fal_tiger_vlan_egr_trans_table_del,
#endif

#ifdef DOS_INCLUDED
    .dos_init = fal_tiger_dos_init,
    .dos_port_en_set = fal_tiger_dos_port_en_set,
    .dos_port_en_get = fal_tiger_dos_port_en_get,
    .dos_drop_en_set = fal_tiger_dos_drop_en_set,
    .dos_drop_en_get = fal_tiger_dos_drop_en_get,
    .dos_large_packet_size_set = fal_tiger_dos_large_packet_size_set,
    .dos_large_packet_size_get = fal_tiger_dos_large_packet_size_get,
    .dos_action_set = fal_tiger_dos_action_set,
    .dos_action_get = fal_tiger_dos_action_get,
    .dos_salearn_dis_set = fal_tiger_dos_salearn_dis_set,
    .dos_salearn_dis_get = fal_tiger_dos_salearn_dis_get,
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
    .dot1x_macBasedEnable_set = fal_tiger_dot1x_macBasedEnable_set,
    .dot1x_macBasedEnable_get = fal_tiger_dot1x_macBasedEnable_get,
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
    .dot1x_eapol_act_set = fal_tiger_dot1x_eapol_act_set,
    .dot1x_eapol_act_get = fal_tiger_dot1x_eapol_act_get,
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
    .oam_mac_swap_en_set = fal_tiger_oam_mac_swap_en_set,
    .oam_mac_swap_en_get = fal_tiger_oam_mac_swap_en_get,
#endif

#ifdef QOS_INCLUDED
    .qos_init = fal_tiger_qos_init,
    .qos_intPri_sel_precedence_set = fal_tiger_qos_intPri_sel_precedence_set,
    .qos_intPri_sel_precedence_get = fal_tiger_qos_intPri_sel_precedence_get,
    .qos_intPri_portDefPri_set  = fal_tiger_qos_intPri_portDefPri_set ,
    .qos_intPri_portDefPri_get  = fal_tiger_qos_intPri_portDefPri_get ,
    .qos_intPri_intCpri_map_set = fal_tiger_qos_intPri_intCpri_map_set,
    .qos_intPri_intCpri_map_get = fal_tiger_qos_intPri_intCpri_map_get,
    .qos_intPri_intSpri_map_set = fal_tiger_qos_intPri_intSpri_map_set,
    .qos_intPri_intSpri_map_get = fal_tiger_qos_intPri_intSpri_map_get,
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
    .qos_schedule_queue_pri_set = fal_tiger_qos_schedule_queue_pri_set,
    .qos_schedule_queue_pri_get = fal_tiger_qos_schedule_queue_pri_get,
    .qos_schedule_dwrr_mode_set = fal_tiger_qos_schedule_dwrr_mode_set,
    .qos_schedule_dwrr_mode_get = fal_tiger_qos_schedule_dwrr_mode_get,
    .qos_schedule_dwrr_weight_set = fal_tiger_qos_schedule_dwrr_weight_set,
    .qos_schedule_dwrr_weight_get = fal_tiger_qos_schedule_dwrr_weight_get,
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
    .stat_mib_port_singleType_cnt_get = fal_tiger_stat_mib_port_singleType_cnt_get,
    .stat_flow_enable_set = fal_tiger_stat_flow_enable_set, 
    .stat_flow_enable_get = fal_tiger_stat_flow_enable_get,
    .stat_flow_mode_set = fal_tiger_stat_flow_mode_set,    
    .stat_flow_mode_get = fal_tiger_stat_flow_mode_get,    
    .stat_flow_count_set = fal_tiger_stat_flow_count_set,   
    .stat_flow_count_get = fal_tiger_stat_flow_count_get,
#endif

#ifdef STP_INCLUDED
    .stp_state_set = fal_tiger_stp_state_set,
    .stp_state_get = fal_tiger_stp_state_get,
    .stp_instance_map_set = fal_tiger_stp_instance_map_set,
    .stp_instance_map_get = fal_tiger_stp_instance_map_get,
    .stp_instance_map_vlanlist_set = fal_tiger_stp_instance_map_vlanlist_set,
    .stp_bpdu_egr_filter_set = fal_tiger_stp_bpdu_egr_filter_set,
    .stp_bpdu_egr_filter_get = fal_tiger_stp_bpdu_egr_filter_get,
    .stp_bpdu_act_set = fal_tiger_stp_bpdu_act_set,
    .stp_bpdu_act_get = fal_tiger_stp_bpdu_act_get,
#endif

#ifdef PORT_ISO_INCLUDED
    .isolation_port_set = fal_tiger_isolation_port_set,
    .isolation_port_get = fal_tiger_isolation_port_get,
    .isolation_vlan_add = fal_tiger_isolation_vlan_add,
    .isolation_vlan_del_by_index = fal_tiger_isolation_vlan_del_by_index,
    .isolation_vlan_set_by_index = fal_tiger_isolation_vlan_set_by_index,
    .isolation_vlan_get_by_index = fal_tiger_isolation_vlan_get_by_index,
    .isolation_vlan_type_set = fal_tiger_isolation_vlan_type_set,
    .isolation_vlan_type_get = fal_tiger_isolation_vlan_type_get,
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
    .loop_detect_loopedPorts_get = fal_tiger_loop_detect_loopedPorts_get,
    .loop_detect_interval_set = fal_tiger_loop_detect_interval_set,
    .loop_detect_interval_get = fal_tiger_loop_detect_interval_get,
    .loop_detect_prevent_enable_set = fal_tiger_loop_detect_prevent_enable_set,
    .loop_detect_prevent_enable_get = fal_tiger_loop_detect_prevent_enable_get,
#endif

#ifdef STORM_CTRL_INCLUDED
    .storm_ctrl_init = fal_tiger_storm_ctrl_init,
    .storm_ctrl_enable_set = fal_tiger_storm_ctrl_enable_set,
    .storm_ctrl_enable_get = fal_tiger_storm_ctrl_enable_get,
    .storm_ctrl_rate_mode_set = fal_tiger_storm_ctrl_rate_mode_set,
    .storm_ctrl_rate_mode_get = fal_tiger_storm_ctrl_rate_mode_get,
    .storm_ctrl_rate_set = fal_tiger_storm_ctrl_rate_set,
    .storm_ctrl_rate_get = fal_tiger_storm_ctrl_rate_get,
#endif

#ifdef LAG_INCLUDED
    .lag_init = fal_tiger_lag_init,
    .lag_en_get = fal_tiger_lag_en_get,
    .lag_hash_sel_set = fal_tiger_lag_hash_sel_set,
    .lag_hash_sel_get = fal_tiger_lag_hash_sel_get,
    .lag_group_port_set = fal_tiger_lag_group_port_set,
    .lag_group_port_get = fal_tiger_lag_group_port_get,
#endif

#ifdef L2_INCLUDED
    .l2_init = fal_tiger_l2_init,
    .l2_fdb_ucast_addr_add = fal_tiger_l2_fdb_ucast_addr_add,
    .l2_fdb_ucast_addr_del = fal_tiger_l2_fdb_ucast_addr_del,
    .l2_fdb_uc_withindex_get = fal_tiger_l2_fdb_uc_withindex_get,
    .l2_fdb_uc_withMacAndVid_get = fal_tiger_l2_fdb_uc_withMacAndVid_get,
    .l2_fdb_uc_withindex_getnext = fal_tiger_l2_fdb_uc_withindex_getnext,
    .l2_fdb_type_get = fal_tiger_l2_fdb_type_get,
    .l2_fdb_all_ucast_flush = fal_tiger_l2_fdb_all_ucast_flush,
    .l2_fdb_port_ucast_flush = fal_tiger_l2_fdb_port_ucast_flush,
    .l2_fdb_vlan_ucast_flush = fal_tiger_l2_fdb_vlan_ucast_flush,
    .l2_fdb_vlan_port_ucast_flush = fal_tiger_l2_fdb_vlan_port_ucast_flush,
    .l2_fdb_uc_cnt_get = fal_tiger_l2_fdb_uc_cnt_get,
    .l2_fdb_port_uc_cnt_get = fal_tiger_l2_fdb_port_uc_cnt_get,
    .l2_fdb_lag_uc_cnt_get = fal_tiger_l2_fdb_lag_uc_cnt_get,
    .l2_fdb_aging_port_en_set = fal_tiger_l2_fdb_aging_port_en_set,
    .l2_fdb_aging_port_en_get = fal_tiger_l2_fdb_aging_port_en_get,
    .l2_fdb_aging_time_set = fal_tiger_l2_fdb_aging_time_set,
    .l2_fdb_aging_time_get = fal_tiger_l2_fdb_aging_time_get,
    .l2_fdb_linkdownFlush_en_set = fal_tiger_l2_fdb_linkdownFlush_en_set,
    .l2_fdb_linkdownFlush_en_get = fal_tiger_l2_fdb_linkdownFlush_en_get,
    .l2_fdb_add_notify_register = fal_tiger_l2_fdb_add_notify_register,
    .l2_fdb_del_notify_register = fal_tiger_l2_fdb_del_notify_register,
    .l2_fdb_uc_get_first_from_db = fal_tiger_l2_fdb_uc_get_first_from_db,
    .l2_fdb_uc_get_next_from_db = fal_tiger_l2_fdb_uc_get_next_from_db,
    .l2_port_learnlimit_en_set = fal_tiger_l2_port_learnlimit_en_set,
    .l2_port_learnlimit_en_get = fal_tiger_l2_port_learnlimit_en_get,
    .l2_port_learnlimit_cnt_set = fal_tiger_l2_port_learnlimit_cnt_set,
    .l2_port_learnlimit_cnt_get = fal_tiger_l2_port_learnlimit_cnt_get,
    .l2_port_learnlimit_act_set = fal_tiger_l2_port_learnlimit_act_set,
    .l2_port_learnlimit_act_get = fal_tiger_l2_port_learnlimit_act_get,
    .l2_system_learnlimit_en_set = fal_tiger_l2_system_learnlimit_en_set,
    .l2_system_learnlimit_en_get = fal_tiger_l2_system_learnlimit_en_get,
    .l2_system_learnlimit_cnt_set = fal_tiger_l2_system_learnlimit_cnt_set,
    .l2_system_learnlimit_cnt_get = fal_tiger_l2_system_learnlimit_cnt_get,
    .l2_system_learnlimit_act_set = fal_tiger_l2_system_learnlimit_act_set,
    .l2_system_learnlimit_act_get = fal_tiger_l2_system_learnlimit_act_get,
    .l2_lag_learnlimit_en_set = fal_tiger_l2_lag_learnlimit_en_set,
    .l2_lag_learnlimit_en_get = fal_tiger_l2_lag_learnlimit_en_get,
    .l2_lag_learnlimit_cnt_set = fal_tiger_l2_lag_learnlimit_cnt_set,
    .l2_lag_learnlimit_cnt_get = fal_tiger_l2_lag_learnlimit_cnt_get,
    .l2_lag_learnlimit_act_set = fal_tiger_l2_lag_learnlimit_act_set,
    .l2_lag_learnlimit_act_get = fal_tiger_l2_lag_learnlimit_act_get,
    .l2_port_learn_en_set = fal_tiger_l2_port_learn_en_set,
    .l2_port_learn_en_get = fal_tiger_l2_port_learn_en_get,
    .l2_port_learn_mode_set = fal_tiger_l2_port_learn_mode_set,
    .l2_port_learn_mode_get = fal_tiger_l2_port_learn_mode_get,
    .l2_port_suspend_act_set = fal_tiger_l2_port_suspend_act_set,
    .l2_port_suspend_act_get = fal_tiger_l2_port_suspend_act_get,
    .l2_new_sa_act_set = fal_tiger_l2_new_sa_act_set,
    .l2_new_sa_act_get = fal_tiger_l2_new_sa_act_get,
    .l2_unknown_ucast_act_set = fal_tiger_l2_unknown_ucast_act_set,
    .l2_unknown_ucast_act_get = fal_tiger_l2_unknown_ucast_act_get,
    .l2_unknown_mcast_act_set = fal_tiger_l2_unknown_mcast_act_set,
    .l2_unknown_mcast_act_get = fal_tiger_l2_unknown_mcast_act_get,
    .l2_filter_mcast_set = fal_tiger_l2_filter_mcast_set,
    .l2_filter_mcast_get = fal_tiger_l2_filter_mcast_get,
    .l2_filter_bcast_set = fal_tiger_l2_filter_bcast_set,
    .l2_filter_bcast_get = fal_tiger_l2_filter_bcast_get,
    .l2_filter_unknown_ucast_set = fal_tiger_l2_filter_unknown_ucast_set,
    .l2_filter_unknown_ucast_get = fal_tiger_l2_filter_unknown_ucast_get,
    .l2_filter_unknown_mcast_set = fal_tiger_l2_filter_unknown_mcast_set,
    .l2_filter_unknown_mcast_get = fal_tiger_l2_filter_unknown_mcast_get,
#endif

#ifdef MULTICAST_INCLUDED
    .multicast_ipv4_add = fal_tiger_multicast_ipv4_add,
    .multicast_ipv6_add = fal_tiger_multicast_ipv6_add,
    .multicast_macaddr_add = fal_tiger_l2_fdb_mcast_addr_add,
    .multicast_ipv4_del = fal_tiger_multicast_ipv4_del,
    .multicast_ipv6_del = fal_tiger_multicast_ipv6_del,
    .multicast_macaddr_del = fal_tiger_l2_fdb_mcast_addr_del,
    .multicast_ipv4_withIpAndVid_get = fal_tiger_multicast_ipv4_withIpAndVid_get,
    .multicast_ipv4_withindex_get = fal_tiger_multicast_ipv4_withindex_get,
    .multicast_ipv4_withindex_getnext = fal_tiger_multicast_ipv4_withindex_getnext,
    .multicast_ipv6_withIpAndVid_get = fal_tiger_multicast_ipv6_withIpAndVid_get,
    .multicast_ipv6_withindex_get = fal_tiger_multicast_ipv6_withindex_get,
    .multicast_ipv6_withindex_getnext = fal_tiger_multicast_ipv6_withindex_getnext,
    .multicast_macaddr_withMacAndVid_get = fal_tiger_l2_mcast_addr_withMacAndVid_get,
    .multicast_macaddr_withindex_get = fal_tiger_l2_mcast_addr_withindex_get,
    .multicast_macaddr_withindex_getnext = fal_tiger_l2_mcast_addr_withindex_getnext,
    .multicast_all_ipmc_flush = fal_tiger_multicast_all_ipmc_flush,
    .multicast_vid_ipmc_flush = fal_tiger_multicast_vid_ipmc_flush,
    .multicast_all_macaddr_flush = fal_tiger_l2_fdb_all_mcast_flush,
    .multicast_vid_macaddr_flush = fal_tiger_l2_fdb_vlan_mcast_flush,
    .multicast_port_macaddr_flush = fal_tiger_l2_fdb_port_mcast_flush,
    .multicast_macaddr_cnt_get = fal_tiger_l2_fdb_mc_cnt_get,
    .multicast_vlan_add = fal_tiger_multicast_vlan_add,
    .multicast_vlan_get = fal_tiger_multicast_vlan_get,
    .multicast_vlan_del = fal_tiger_multicast_vlan_del,
    .multicast_hwmc_flush_all = fal_tiger_multicast_hwmc_flush_all,
    .multicast_hwmc_flush_by_vid = fal_tiger_multicast_hwmc_flush_by_vid,
    .multicast_hwmc_flush_by_portmask = fal_tiger_multicast_hwmc_flush_by_portmask,
    .multicast_hwmc_mac_del = fal_tiger_multicast_hwmc_mac_del,
    .multicast_hwmc_ipv4_del = fal_tiger_multicast_hwmc_ipv4_del,
 #endif

#ifdef IGMP_MLD_INCLUDED
    .igmp_opmode_set = fal_tiger_igmp_opmode_set,
    .igmp_opmode_get = fal_tiger_igmp_opmode_get,
    .mld_opmode_set = fal_tiger_mld_opmode_set,
    .mld_opmode_get = fal_tiger_mld_opmode_get,
    .igmp_mld_port_report_allow_set = fal_tiger_igmp_mld_port_report_allow_set,
    .igmp_mld_port_report_allow_get = fal_tiger_igmp_mld_port_report_allow_get,
    .igmp_mld_port_leave_allow_set = fal_tiger_igmp_mld_port_leave_allow_set,
    .igmp_mld_port_leave_allow_get = fal_tiger_igmp_mld_port_leave_allow_get,
    .igmp_mld_port_query_allow_set = fal_tiger_igmp_mld_port_query_allow_set,
    .igmp_mld_port_query_allow_get = fal_tiger_igmp_mld_port_query_allow_get,
    .igmp_mld_fastleave_set = fal_tiger_igmp_mld_fastleave_set,
    .igmp_mld_fastleave_get = fal_tiger_igmp_mld_fastleave_get,
    .igmp_mld_learnlimit_en_set = fal_tiger_igmp_mld_learnlimit_en_set,
    .igmp_mld_learnlimit_en_get = fal_tiger_igmp_mld_learnlimit_en_get,
    .igmp_mld_learnlimit_maxgroup_set = fal_tiger_igmp_mld_learnlimit_maxgroup_set,
    .igmp_mld_learnlimit_maxgroup_get = fal_tiger_igmp_mld_learnlimit_maxgroup_get,
    .igmp_mld_fwd_routerport_only_set = fal_tiger_igmp_mld_fwd_routerport_only_set,
    .igmp_mld_fwd_routerport_only_get = fal_tiger_igmp_mld_fwd_routerport_only_get,
    .igmp_mld_fwd_routerport_primary_set = fal_tiger_igmp_mld_fwd_routerport_primary_set,
    .igmp_mld_fwd_routerport_primary_get = fal_tiger_igmp_mld_fwd_routerport_primary_get,
    .igmp_mld_bypass_grouprange_set = fal_tiger_igmp_mld_bypass_grouprange_set,
    .igmp_mld_bypass_grouprange_get = fal_tiger_igmp_mld_bypass_grouprange_get,
    .igmp_mld_dynamic_routerport_get = fal_tiger_igmp_mld_dynamic_routerport_get,
    .igmp_mld_dynamic_routerport_allow_set = fal_tiger_igmp_mld_dynamic_routerport_allow_set,
    .igmp_mld_dynamic_routerport_allow_get = fal_tiger_igmp_mld_dynamic_routerport_allow_get,
    .igmp_mld_dynamic_routerport_agingtime_set = fal_tiger_igmp_mld_dynamic_routerport_agingtime_set,
    .igmp_mld_dynamic_routerport_agingtime_get = fal_tiger_igmp_mld_dynamic_routerport_agingtime_get,
    .igmp_mld_static_routerport_set = fal_tiger_igmp_mld_static_routerport_set,
    .igmp_mld_static_routerport_get = fal_tiger_igmp_mld_static_routerport_get,
    .igmp_mld_bypass_portIsolation_set = fal_tiger_igmp_mld_bypass_portIsolation_set,
    .igmp_mld_bypass_portIsolation_get = fal_tiger_igmp_mld_bypass_portIsolation_get,
    .igmp_mld_ipmc_bypass_portIsolation_set = fal_tiger_igmp_mld_ipmc_bypass_portIsolation_set,
    .igmp_mld_ipmc_bypass_portIsolation_get = fal_tiger_igmp_mld_ipmc_bypass_portIsolation_get,
    .igmp_mld_bypass_vlanIngrfilter_en_set = fal_tiger_igmp_mld_bypass_vlanIngrfilter_en_set,
    .igmp_mld_bypass_vlanIngrfilter_en_get = fal_tiger_igmp_mld_bypass_vlanIngrfilter_en_get,
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
    .nic_int_cputag_en_set = fal_tiger_nic_int_cputag_en_set,
    .nic_int_cputag_en_get = fal_tiger_nic_int_cputag_en_get,
    .nic_cpu_pkt_keepAll_en_set = fal_tiger_nic_cpu_pkt_keepAll_en_set,
    .nic_cpu_pkt_keepAll_en_get = fal_tiger_nic_cpu_pkt_keepAll_en_get,
    .nic_cputag_mode_set = fal_tiger_nic_cputag_mode_set,
    .nic_cputag_mode_get = fal_tiger_nic_cputag_mode_get,
#endif

#ifdef PORT_MIRROR_INCLUDED
    .mirror_init = fal_tiger_mirror_init,
    .mirror_group_set = fal_tiger_mirror_group_set,
    .mirror_group_get = fal_tiger_mirror_group_get,
#endif
#ifdef RMA_INCLUDED
    .rma_action_set = fal_tiger_rma_action_set,
    .rma_action_get = fal_tiger_rma_action_get,
    .rma_cpu_code_set = fal_tiger_rma_cpu_code_set,
    .rma_cpu_code_get = fal_tiger_rma_cpu_code_get,
    .rma_bypass_set = fal_tiger_rma_bypass_set,
    .rma_bypass_get = fal_tiger_rma_bypass_get,
    .rma_sa_learn_dis_set = fal_tiger_rma_sa_learn_dis_set,
    .rma_sa_learn_dis_get = fal_tiger_rma_sa_learn_dis_get,
#endif

#ifdef WOL_INCLUDED
    .wol_switch_enable_set = fal_tiger_wol_ctrl_set,
    .wol_switch_enable_get = fal_tiger_wol_ctrl_get,
    .wol_switch_etherType_set = fal_tiger_wol_ethertype_set,
    .wol_switch_etherType_get = fal_tiger_wol_ethertype_get,
    .wol_port_enable_set = fal_tiger_wol_port_enable_set,
    .wol_port_enable_get = fal_tiger_wol_port_enable_get,
    .wol_port_macAddr_set = fal_tiger_wol_port_macAddr_set,
    .wol_port_macAddr_get = fal_tiger_wol_port_macAddr_get,
#endif

#ifdef SYS_INCLUDED
    .sys_mac_get = fal_tiger_sys_mac_get,
    .sys_chip_reset = fal_tiger_sys_chip_reset,
    .sys_database_reset = fal_tiger_sys_database_reset,
    .sys_chipInfo_get = fal_tiger_sys_chipInfo_get,
#endif

#ifdef PORT_INCLUDED
    .port_init = fal_tiger_port_init,
    .port_default_cfg_set = fal_tiger_port_default_cfg_set,
    .port_mac_enable_set = fal_tiger_port_mac_enable_set,
    .port_mac_enable_get = fal_tiger_port_mac_enable_get,
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
    .port_mac_block_set = fal_tiger_port_mac_block_set,
    .port_mac_block_get = fal_tiger_port_mac_block_get,
    .port_mac_eee_enable_set = fal_tiger_port_mac_eee_enable_set,
    .port_mac_eee_enable_get = fal_tiger_port_mac_eee_enable_get,
    .port_mac_eee_timer_set = fal_tiger_port_mac_eee_timer_set,
    .port_mac_eee_timer_get = fal_tiger_port_mac_eee_timer_get,
    .port_extif_mode_set = fal_tiger_port_extif_mode_set,
    .port_extif_mode_get = fal_tiger_port_extif_mode_get,
    .port_extif_rgmii_delay_set = fal_tiger_port_extif_rgmii_delay_set,
    .port_extif_rgmii_delay_get = fal_tiger_port_extif_rgmii_delay_get,
    .port_phy_enable_set = fal_tiger_port_phy_enable_set,
    .port_phy_enable_get = fal_tiger_port_phy_enable_get,
    .port_phyAutoNeg_enable_set = fal_tiger_port_phyAutoNeg_enable_set,
    .port_phyAutoNeg_enable_get = fal_tiger_port_phyAutoNeg_enable_get,
    .port_phyAutoNeg_ability_set = fal_tiger_port_phyAutoNeg_ability_set,
    .port_phyAutoNeg_ability_get = fal_tiger_port_phyAutoNeg_ability_get,
    .port_phy_force_set = fal_tiger_port_phy_force_set,
    .port_phy_force_get = fal_tiger_port_phy_force_get,
    .port_phy_linkstatus_get = fal_tiger_port_phy_linkstatus_get,
    .port_phy_reg_set = fal_tiger_port_phy_reg_set,
    .port_phy_reg_get = fal_tiger_port_phy_reg_get,
    .port_phy_eee_enable_set = fal_tiger_port_phy_eee_enable_set,
    .port_phy_eee_enable_get = fal_tiger_port_phy_eee_enable_get,
    .port_phy_eee_status_get = fal_tiger_port_phy_eee_status_get,
    .port_jumbo_enable_set = fal_tiger_port_jumbo_enable_set,
    .port_jumbo_enable_get = fal_tiger_port_jumbo_enable_get,
    .port_jumbo_size_set = fal_tiger_port_jumbo_size_set,
    .port_jumbo_size_get = fal_tiger_port_jumbo_size_get,
    .port_cable_diag_start = fal_tiger_port_cable_diag_start,
    .port_cable_diag_result_get = fal_tiger_port_cable_diag_result_get,
    .port_phyCrossover_mode_set = fal_tiger_port_phyCrossover_mode_set,
    .port_phyCrossover_mode_get = fal_tiger_port_phyCrossover_mode_get,
    .port_phyCrossover_status_get = fal_tiger_port_phyCrossover_status_get,
    .port_phy_green_start = fal_tiger_port_phyGreen_start,
    .port_phy_green_result_get = fal_tiger_port_phyGreen_result_get,
    .port_phy_smart_downgrade_set = fal_tiger_port_physmart_downgrade_set,
    .port_phy_smart_downgrade_get = fal_tiger_port_physmart_downgrade_get,
    .port_extif_xmiiClk_invert_set = fal_tiger_port_extif_xmiiClk_invert_set,
    .port_extif_xmiiClk_invert_get = fal_tiger_port_extif_xmiiClk_invert_get,
    .port_parallel_detection_set = fal_tiger_port_parallel_detection_set,
    .port_parallel_detection_get = fal_tiger_port_parallel_detection_get,
    .port_dvddio_power_level_set = fal_tiger_port_dvddio_power_level_set,
    .port_dvddio_power_level_get = fal_tiger_port_dvddio_power_level_get,
    .port_phyCombo_mode_set = fal_tiger_port_phyCombo_mode_set,
    .port_phyCombo_mode_get = fal_tiger_port_phyCombo_mode_get,
    .port_phy_chip_mode_set = fal_tiger_port_phy_chip_mode_set,
    .port_phy_chip_mode_get = fal_tiger_port_phy_chip_mode_get,
    .port_polling_enable_set = fal_tiger_port_polling_enable_set,
    .port_polling_enable_get = fal_tiger_port_polling_enable_get,
#endif

#ifdef CTRLPKT_INCLUDED
    .ctrlpkt_act_set = fal_tiger_ctrlpkt_act_set,
    .ctrlpkt_act_get = fal_tiger_ctrlpkt_act_get,
    .ctrlpkt_myMac_act_set = fal_tiger_ctrlpkt_myMac_act_set,
    .ctrlpkt_myMac_act_get = fal_tiger_ctrlpkt_myMac_act_get,
    .ctrlpkt_bypass_set = fal_tiger_ctrlpkt_bypass_set,
    .ctrlpkt_bypass_get = fal_tiger_ctrlpkt_bypass_get,
    .ctrlpkt_bypass_stp_set = fal_tiger_ctrlpkt_bypass_stp_set,
    .ctrlpkt_bypass_stp_get = fal_tiger_ctrlpkt_bypass_stp_get,
    .ctrlpkt_sa_learn_set = fal_tiger_ctrlpkt_sa_learn_set,
    .ctrlpkt_sa_learn_get = fal_tiger_ctrlpkt_sa_learn_get,
    .ctrlpkt_parse_en_set = fal_tiger_ctrlpkt_parse_en_set,
    .ctrlpkt_parse_en_get = fal_tiger_ctrlpkt_parse_en_get,
    .ctrlpkt_rrpp_parse_set = fal_tiger_ctrlpkt_rrpp_parse_set,
    .ctrlpkt_rrpp_parse_get = fal_tiger_ctrlpkt_rrpp_parse_get,
#endif

#ifdef INTERRUPT_INCLUDED
    .int_polarity_set = fal_tiger_int_polarity_set,
    .int_polarity_get = fal_tiger_int_polarity_get,
    .int_control_set = fal_tiger_int_control_set,
    .int_control_get = fal_tiger_int_control_get,
    .int_status_get = fal_tiger_int_status_get,
    .int_typeStatus_get = fal_tiger_int_typeStatus_get,
    .int_port_int_set = fal_tiger_int_port_int_set,
    .int_port_int_get = fal_tiger_int_port_int_get,
    .int_allPortIntStatus_get = fal_tiger_int_allPortIntStatus_get,
    .int_portIntStatus_get = fal_tiger_int_portIntStatus_get,
    .int_wol_signalOutputType_set = fal_tiger_int_wol_signalOutputType_set,
    .int_wol_signalOutputType_get = fal_tiger_int_wol_signalOutputType_get,
    .int_wol_port_enable_set = fal_tiger_int_wol_port_enable_set,
    .int_wol_port_enable_get = fal_tiger_int_wol_port_enable_get,
    .int_wol_allPortIntStatus_get = fal_tiger_int_wol_allPortIntStatus_get,
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
    .led_serial_outputMode_set= fal_tiger_led_serial_outputMode_set,
    .led_serial_outputMode_get= fal_tiger_led_serial_outputMode_get,
    .led_serial_activeMode_set= fal_tiger_led_serial_activeMode_set,
    .led_serial_activeMode_get= fal_tiger_led_serial_activeMode_get,
    .led_serial_remapping_set= fal_tiger_led_serial_remapping_set,
    .led_serial_remapping_get= fal_tiger_led_serial_remapping_get,
    .led_parallel_output_set= fal_tiger_led_parallel_output_set,
    .led_parallel_output_get= fal_tiger_led_parallel_output_get,
    .led_parallel_remapping_set= fal_tiger_led_parallel_remapping_set,
    .led_parallel_remapping_get= fal_tiger_led_parallel_remapping_get,
    .led_parallel_pos_invert_set= fal_tiger_led_parallel_pos_invert_set,
    .led_parallel_pos_invert_get= fal_tiger_led_parallel_pos_invert_get,
    .led_serial_port_info_set= fal_tiger_led_serial_port_info_set,
    .led_serial_port_info_get= fal_tiger_led_serial_port_info_get,
    .led_serial_total_ledNum_set= fal_tiger_led_serial_total_ledNum_set,
    .led_serial_total_ledNum_get= fal_tiger_led_serial_total_ledNum_get,
#endif

#ifdef SENSOR_INCLUDED
    .sensor_temp_enable_set = fal_tiger_sensor_temp_enable_set,
    .sensor_temp_enable_get = fal_tiger_sensor_temp_enable_get,
    .sensor_temp_value_get = fal_tiger_sensor_temp_value_get,
#endif

#ifdef GPIO_INCLUDED
    .gpio_init = fal_tiger_gpio_init,
    .gpio_mdioMaster_pinGroup_get = fal_tiger_gpio_mdioMaster_pinGroup_get,
    .gpio_mdioMaster_pinGroup_set = fal_tiger_gpio_mdioMaster_pinGroup_set,
#endif

#if defined(DEBUG_INCLUDED) && defined(PORT_INCLUDED)
    .debug_phyTemplate_test_set = fal_tiger_debug_phyTemplate_test_set,
    .debug_phyLoopback_test_set = fal_tiger_debug_phyLoopback_test_set,
    .debug_utp_snr_get = fal_tiger_debug_utp_snr_get,
#endif

    .exit = fal_tiger_exit,
};

