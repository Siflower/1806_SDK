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
#include "yt_igmp_mld.h"
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
#include "yt_debug.h"
#include "yt_mirror.h"
#include "yt_rspan.h"
#include "yt_isolation.h"
#include "yt_gpio.h"

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
    /*VLAN*/
    yt_ret_t (*vlan_init)(yt_unit_t);
    yt_ret_t (*vlan_port_set)(yt_unit_t, yt_vlan_t, yt_port_mask_t, yt_port_mask_t);
    yt_ret_t (*vlan_port_get)(yt_unit_t, yt_vlan_t, yt_port_mask_t *, yt_port_mask_t  *);
    yt_ret_t (*vlan_svlMode_enable_set)(yt_unit_t, yt_vlan_t, yt_enable_t);
    yt_ret_t (*vlan_svlMode_enable_get)(yt_unit_t, yt_vlan_t, yt_enable_t *);
    yt_ret_t (*vlan_fid_set)(yt_unit_t, yt_vlan_t,  yt_fid_t);
    yt_ret_t (*vlan_fid_get)(yt_unit_t, yt_vlan_t,  yt_fid_t *);
    yt_ret_t (*vlan_igrTpid_set)(yt_unit_t, yt_tpid_profiles_t);
    yt_ret_t (*vlan_igrTpid_get)(yt_unit_t, yt_tpid_profiles_t *);
    yt_ret_t (*vlan_port_igrTpidSel_set)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_tpidprofile_id_mask_t);
    yt_ret_t (*vlan_port_igrTpidSel_get)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_tpidprofile_id_mask_t *);
    yt_ret_t (*vlan_port_igrPvid_set)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_vlan_t);
    yt_ret_t (*vlan_port_igrPvid_get)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_vlan_t *);
    yt_ret_t (*vlan_port_igrDefPri_set)(yt_unit_t, yt_vlan_type_t , yt_port_t , uint8_t);
    yt_ret_t (*vlan_port_igrDefPri_get)(yt_unit_t, yt_vlan_type_t , yt_port_t , uint8_t *);
    yt_ret_t (*vlan_port_igrFilter_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*vlan_port_igrFilter_enable_get)(yt_unit_t, yt_port_t, yt_enable_t *);
    yt_ret_t (*vlan_igrTransparent_set)(yt_unit_t, yt_port_t, yt_port_mask_t);
    yt_ret_t (*vlan_igrTransparent_get)(yt_unit_t, yt_port_t, yt_port_mask_t *);
    yt_ret_t (*vlan_port_aft_set)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_vlan_aft_t);
    yt_ret_t (*vlan_port_aft_get)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_vlan_aft_t *);
    yt_ret_t (*vlan_port_egrTagMode_set)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_egr_tag_mode_t);
    yt_ret_t (*vlan_port_egrTagMode_get)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_egr_tag_mode_t *);
    yt_ret_t (*vlan_port_egrDefVid_set)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_vlan_t);
    yt_ret_t (*vlan_port_egrDefVid_get)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_vlan_t *);
    yt_ret_t (*vlan_egrTpid_set)(yt_unit_t, yt_tpid_profiles_t);
    yt_ret_t (*vlan_egrTpid_get)(yt_unit_t, yt_tpid_profiles_t *);
    yt_ret_t (*vlan_port_egrTpidSel_set)(yt_unit_t, yt_vlan_type_t, yt_port_t, uint8_t);
    yt_ret_t (*vlan_port_egrTpidSel_get)(yt_unit_t, yt_vlan_type_t, yt_port_t,  uint8_t *);
    yt_ret_t (*vlan_port_egrTransparent_set)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_enable_t, yt_port_mask_t);
    yt_ret_t (*vlan_port_egrTransparent_get)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_enable_t *, yt_port_mask_t *);
    yt_ret_t (*vlan_port_egrFilter_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*vlan_port_egrFilter_enable_get)(yt_unit_t, yt_port_t, yt_enable_t *);
    yt_ret_t (*vlan_port_vidTypeSel_set)(yt_unit_t, yt_port_t, yt_vlan_type_t);
    yt_ret_t (*vlan_port_vidTypeSel_get)(yt_unit_t, yt_port_t, yt_vlan_type_t *);
    yt_ret_t (*vlan_protocolBasedVlan_group_set)(yt_unit_t, uint8_t, const yt_vlan_protocol_key_t *);
    yt_ret_t (*vlan_protocolBasedVlan_group_get)(yt_unit_t, uint8_t, yt_vlan_protocol_key_t *);
    yt_ret_t (*vlan_protocolBasedVlan_table_add)(yt_unit_t, yt_port_t, uint8_t, const yt_vlan_protocol_action_t *);
    yt_ret_t (*vlan_protocolBasedVlan_table_get)(yt_unit_t, yt_port_t, uint8_t, yt_vlan_protocol_action_t *);
    yt_ret_t (*vlan_protocolBasedVlan_table_del)(yt_unit_t, yt_port_t, uint8_t);
    yt_ret_t (*vlan_mac_ipSub_tbl_arrange_set)(yt_unit_t, uint8_t, uint8_t, yt_vlan_mac_ip_subnet_pri_t );
    yt_ret_t (*vlan_mac_ipSub_tbl_arrange_get)(yt_unit_t, uint8_t *, uint8_t *, yt_vlan_mac_ip_subnet_pri_t  *);
    yt_ret_t (*vlan_macBasedVlan_table_add)(yt_unit_t, const yt_mac_vlan_rule_t *, const yt_mac_vlan_action_t *, yt_mac_vlan_table_idx_t *);
    yt_ret_t (*vlan_macBasedVlan_table_add_by_index)(yt_unit_t, yt_mac_vlan_table_idx_t, const yt_mac_vlan_rule_t *, const yt_mac_vlan_action_t *);
    yt_ret_t (*vlan_macBasedVlan_table_get)(yt_unit_t, yt_mac_vlan_table_idx_t, yt_mac_vlan_rule_t *, yt_mac_vlan_action_t *);
    yt_ret_t (*vlan_macBasedVlan_table_del)(yt_unit_t, yt_mac_vlan_table_idx_t);
    yt_ret_t (*vlan_ipSubnetBasedVlan_table_add)(yt_unit_t, const yt_ip_subnet_vlan_rule_t *, const yt_ip_subnet_vlan_action_t *, yt_ip_subnet_vlan_table_idx_t *);
    yt_ret_t (*vlan_ipSubnetBasedVlan_table_add_by_index)(yt_unit_t, yt_ip_subnet_vlan_table_idx_t, const yt_ip_subnet_vlan_rule_t *, const yt_ip_subnet_vlan_action_t *);
    yt_ret_t (*vlan_ipSubnetBasedVlan_table_get)(yt_unit_t, yt_ip_subnet_vlan_table_idx_t, yt_ip_subnet_vlan_rule_t *, yt_ip_subnet_vlan_action_t *);
    yt_ret_t (*vlan_ipSubnetBasedVlan_table_del)(yt_unit_t, yt_ip_subnet_vlan_table_idx_t);
    yt_ret_t (*vlan_ipSubnetBasedVlan_bypass_arp_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*vlan_ipSubnetBasedVlan_bypass_arp_get)(yt_unit_t, yt_enable_t *);
    yt_ret_t (*vlan_policy_idx_mapping_set)(yt_unit_t, yt_vlan_t,  uint8_t);
    yt_ret_t (*vlan_policy_idx_mapping_get)(yt_unit_t, yt_vlan_t,  uint8_t *);
    yt_ret_t (*vlan_policy_table_add)(yt_unit_t, const yt_vlan_policy_info_t *,  uint8_t *);
    yt_ret_t (*vlan_policy_table_add_by_index)(yt_unit_t, uint8_t, const yt_vlan_policy_info_t *);
    yt_ret_t (*vlan_policy_table_get)(yt_unit_t, uint8_t, yt_vlan_policy_info_t *);
    yt_ret_t (*vlan_policy_table_del)(yt_unit_t, uint8_t);

    /*vlan translate*/
    yt_ret_t (*vlan_igr_trans_untagPvidIgnore_set)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_enable_t);
    yt_ret_t (*vlan_igr_trans_untagPvidIgnore_get)(yt_unit_t, yt_vlan_type_t, yt_port_t, yt_enable_t *);
    yt_ret_t (*vlan_igr_trans_prio_tagPvidIgnore_set)(yt_unit_t, yt_vlan_type_t,  yt_port_t , yt_enable_t);
    yt_ret_t (*vlan_igr_trans_prio_tagPvidIgnore_get)(yt_unit_t, yt_vlan_type_t ,  yt_port_t , yt_enable_t *);
    yt_ret_t (*vlan_igr_trans_lookup_miss_ctrl_set)(yt_unit_t, yt_port_t , yt_vlan_type_t , yt_act_type_t);
    yt_ret_t (*vlan_igr_trans_lookup_miss_ctrl_get)(yt_unit_t, yt_port_t , yt_vlan_type_t, yt_act_type_t *);
    yt_ret_t (*vlan_igr_trans_rangeProfile_add)(yt_unit_t, yt_profile_id_t, const yt_vlan_range_group_t *);
    yt_ret_t (*vlan_igr_trans_rangeProfile_get)(yt_unit_t , yt_profile_id_t , yt_vlan_range_group_t *);
    yt_ret_t (*vlan_igr_trans_rangeProfile_del)(yt_unit_t , yt_profile_id_t);
    yt_ret_t (*vlan_igr_trans_port_rangeProfileSel_set)(yt_unit_t, yt_port_t, yt_profile_id_t);
    yt_ret_t (*vlan_igr_trans_port_rangeProfileSel_get)(yt_unit_t, yt_port_t,  yt_profile_id_t *);
    yt_ret_t (*vlan_igr_trans_mode_set)(yt_unit_t, yt_port_t, yt_vlan_range_trans_mode_t);
    yt_ret_t (*vlan_igr_trans_mode_get)(yt_unit_t, yt_port_t,  yt_vlan_range_trans_mode_t *);
    yt_ret_t (*vlan_igr_trans_table_add)(yt_unit_t, const yt_vlan_trans_tbl_t *, const yt_vlan_trans_action_tbl_t *, yt_trans_tbl_id_t *);
    yt_ret_t (*vlan_igr_trans_table_add_by_index)(yt_unit_t, yt_trans_tbl_id_t, const yt_vlan_trans_tbl_t *,  const yt_vlan_trans_action_tbl_t *);
    yt_ret_t (*vlan_igr_trans_table_get)(yt_unit_t, yt_trans_tbl_id_t, yt_vlan_trans_tbl_t *, yt_vlan_trans_action_tbl_t *);
    yt_ret_t (*vlan_igr_trans_table_del)(yt_unit_t, yt_trans_tbl_id_t);
    yt_ret_t (*vlan_egr_trans_lookup_miss_ctrl_set)(yt_unit_t, yt_port_t , yt_vlan_type_t, yt_vlan_egrtrans_lookup_miss_act_t);
    yt_ret_t (*vlan_egr_trans_lookup_miss_ctrl_get)(yt_unit_t, yt_port_t, yt_vlan_type_t, yt_vlan_egrtrans_lookup_miss_act_t *);
    yt_ret_t (*vlan_egr_trans_rangeProfile_add)(yt_unit_t, yt_profile_id_t, const yt_vlan_range_group_t *);
    yt_ret_t (*vlan_egr_trans_rangeProfile_get)(yt_unit_t, yt_profile_id_t, yt_vlan_range_group_t *);
    yt_ret_t (*vlan_egr_trans_rangeProfile_del)(yt_unit_t, yt_profile_id_t);
    yt_ret_t (*vlan_egr_trans_table_add)(yt_unit_t, const yt_egr_vlan_trans_tbl_t *, const yt_egr_vlan_trans_action_tbl_t *, yt_trans_tbl_id_t *);
    yt_ret_t (*vlan_egr_trans_table_add_by_index)(yt_unit_t, yt_trans_tbl_id_t, const yt_egr_vlan_trans_tbl_t  *, const yt_egr_vlan_trans_action_tbl_t *);
    yt_ret_t (*vlan_egr_trans_table_get)(yt_unit_t, yt_trans_tbl_id_t, yt_egr_vlan_trans_tbl_t *, yt_egr_vlan_trans_action_tbl_t *);
    yt_ret_t (*vlan_egr_trans_table_del)(yt_unit_t, yt_trans_tbl_id_t);

    yt_ret_t (*dos_init)(yt_unit_t);
    yt_ret_t (*dos_port_en_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*dos_port_en_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*dos_action_set)(yt_unit_t, yt_dos_type_t, yt_act_type_t);
    yt_ret_t (*dos_action_get)(yt_unit_t, yt_dos_type_t, yt_act_type_t *);
    yt_ret_t (*dos_salearn_dis_set)(yt_unit_t, yt_dos_type_t, yt_enable_t);
    yt_ret_t (*dos_salearn_dis_get)(yt_unit_t, yt_dos_type_t, yt_enable_t *);
    yt_ret_t (*dos_large_packet_size_set)(yt_unit_t, yt_dos_packet_type_t, yt_dos_packet_version_t, uint16_t);
    yt_ret_t (*dos_large_packet_size_get)(yt_unit_t, yt_dos_packet_type_t, yt_dos_packet_version_t, uint16_t *);
    yt_ret_t (*dos_drop_en_set)(yt_unit_t, yt_dos_type_t, yt_enable_t);
    yt_ret_t (*dos_drop_en_get)(yt_unit_t, yt_dos_type_t, yt_enable_t *);

    yt_ret_t (*dot1x_init)(yt_unit_t);
    yt_ret_t (*dot1x_portBasedEnable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*dot1x_portBasedEnable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*dot1x_portBasedAuthStatus_set)(yt_unit_t, yt_port_t, yt_dot1x_auth_status_t);
    yt_ret_t (*dot1x_portBasedAuthStatus_get)(yt_unit_t, yt_port_t,  yt_dot1x_auth_status_t *);
    yt_ret_t (*dot1x_portBasedDirection_set)(yt_unit_t, yt_port_t, yt_dot1x_direction_t);
    yt_ret_t (*dot1x_portBasedDirection_get)(yt_unit_t, yt_port_t,  yt_dot1x_direction_t *);
    yt_ret_t (*dot1x_macBasedEnable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*dot1x_macBasedEnable_get)(yt_unit_t, yt_port_t, yt_enable_t*);
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
    yt_ret_t (*dot1x_eapol_act_set)(yt_unit_t, yt_port_t, yt_act_type_t);
    yt_ret_t (*dot1x_eapol_act_get)(yt_unit_t, yt_port_t, yt_act_type_t*);

    yt_ret_t (*oam_en_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*oam_en_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*oam_bypass_accessctrl_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*oam_bypass_accessctrl_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*oam_parser_act_set)(yt_unit_t, yt_port_t, yt_oam_parser_action_t);
    yt_ret_t (*oam_parser_act_get)(yt_unit_t, yt_port_t,  yt_oam_parser_action_t *);
    yt_ret_t (*oam_mux_act_set)(yt_unit_t, yt_port_t, yt_oam_mux_action_t);
    yt_ret_t (*oam_mux_act_get)(yt_unit_t, yt_port_t,  yt_oam_mux_action_t *);
    yt_ret_t (*oam_mac_swap_en_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*oam_mac_swap_en_get)(yt_unit_t, yt_port_t, yt_enable_t *);

    yt_ret_t (*qos_init)(yt_unit_t);
    yt_ret_t (*qos_intPri_sel_precedence_set)(yt_unit_t, yt_port_t, yt_qos_intPri_sel_precedence_t *);
    yt_ret_t (*qos_intPri_sel_precedence_get)(yt_unit_t, yt_port_t,  yt_qos_intPri_sel_precedence_t *);
    yt_ret_t (*qos_intPri_portDefPri_set )(yt_unit_t, yt_port_t, yt_enable_t, yt_pri_t);
    yt_ret_t (*qos_intPri_portDefPri_get )(yt_unit_t, yt_port_t,  yt_enable_t *,  yt_pri_t *);
	yt_ret_t (*qos_intPri_intCpri_map_set)(yt_unit_t, yt_pri_t, yt_pri_t);
    yt_ret_t (*qos_intPri_intCpri_map_get)(yt_unit_t, yt_pri_t, yt_pri_t *);
    yt_ret_t (*qos_intPri_intSpri_map_set)(yt_unit_t, yt_pri_t, yt_pri_t);
    yt_ret_t (*qos_intPri_intSpri_map_get)(yt_unit_t, yt_pri_t, yt_pri_t *);
    yt_ret_t (*qos_intPri_cpri_map_set)(yt_unit_t, yt_qos_pmap_tci_t, yt_pri_t);
    yt_ret_t (*qos_intPri_cpri_map_get)(yt_unit_t, yt_qos_pmap_tci_t,  yt_pri_t *);
    yt_ret_t (*qos_intDP_cpri_map_set)(yt_unit_t, yt_qos_pmap_tci_t, yt_dp_t);
    yt_ret_t (*qos_intDP_cpri_map_get)(yt_unit_t, yt_qos_pmap_tci_t,  yt_dp_t *);
    yt_ret_t (*qos_intPri_spri_map_set)(yt_unit_t, yt_qos_pmap_tci_t, yt_pri_t);
    yt_ret_t (*qos_intPri_spri_map_get)(yt_unit_t, yt_qos_pmap_tci_t,  yt_pri_t *);
    yt_ret_t (*qos_intDP_spri_map_set)(yt_unit_t, yt_qos_pmap_tci_t, yt_dp_t);
    yt_ret_t (*qos_intDP_spri_map_get)(yt_unit_t, yt_qos_pmap_tci_t,  yt_dp_t *);
    yt_ret_t (*qos_intPri_dscp_map_set)(yt_unit_t, yt_dscp_t, yt_pri_t);
    yt_ret_t (*qos_intPri_dscp_map_get)(yt_unit_t, yt_dscp_t,  yt_pri_t *);
    yt_ret_t (*qos_intDP_dscp_map_set)(yt_unit_t, yt_dscp_t, yt_dp_t);
    yt_ret_t (*qos_intDP_dscp_map_get)(yt_unit_t, yt_dscp_t,  yt_dp_t *);
    yt_ret_t (*qos_intPri_map_igrMirror_set)(yt_unit_t, yt_mirror_group_t, yt_enable_t, yt_pri_t);
    yt_ret_t (*qos_intPri_map_igrMirror_get)(yt_unit_t, yt_mirror_group_t, yt_enable_t *, yt_pri_t *);
    yt_ret_t (*qos_intPri_map_egrMirror_set)(yt_unit_t, yt_mirror_group_t, yt_enable_t, yt_pri_t);
    yt_ret_t (*qos_intPri_map_egrMirror_get)(yt_unit_t, yt_mirror_group_t, yt_enable_t *, yt_pri_t *);
    yt_ret_t (*qos_que_map_ucast_set)(yt_unit_t, yt_port_t, yt_qos_qmap_t *);
    yt_ret_t (*qos_que_map_ucast_get)(yt_unit_t, yt_port_t,  yt_qos_qmap_t *);
    yt_ret_t (*qos_que_map_mcast_set)(yt_unit_t, yt_port_t, yt_qos_qmap_t *);
    yt_ret_t (*qos_que_map_mcast_get)(yt_unit_t, yt_port_t,  yt_qos_qmap_t *);
    yt_ret_t (*qos_que_forceDrop_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*qos_que_forceDrop_enable_get)(yt_unit_t, yt_port_t, yt_enable_t *);
    yt_ret_t (*qos_remark_port_set)(yt_unit_t, yt_port_t, const yt_qos_remark_ctrl_t *);
    yt_ret_t (*qos_remark_port_get)(yt_unit_t, yt_port_t,  yt_qos_remark_ctrl_t *);
    yt_ret_t (*qos_remark_dscp_set)(yt_unit_t , yt_qos_remark_info_t, yt_dscp_t);
    yt_ret_t (*qos_remark_dscp_get)(yt_unit_t, yt_qos_remark_info_t,  yt_dscp_t *);
    yt_ret_t (*qos_remark_cpri_set)(yt_unit_t, yt_qos_remark_info_t, yt_qos_pmap_tci_t);
    yt_ret_t (*qos_remark_cpri_get)(yt_unit_t, yt_qos_remark_info_t,  yt_qos_pmap_tci_t *);
    yt_ret_t (*qos_remark_spri_set)(yt_unit_t, yt_qos_remark_info_t, yt_qos_pmap_tci_t);
    yt_ret_t (*qos_remark_spri_get)(yt_unit_t, yt_qos_remark_info_t,  yt_qos_pmap_tci_t *);
    yt_ret_t (*qos_schedule_queue_pri_set)(yt_unit_t, yt_qid_t, yt_queue_pri_t);
    yt_ret_t (*qos_schedule_queue_pri_get)(yt_unit_t, yt_qid_t,  yt_queue_pri_t *);
    yt_ret_t (*qos_schedule_dwrr_mode_set)(yt_unit_t, yt_qid_t, yt_rate_mode_t);
    yt_ret_t (*qos_schedule_dwrr_mode_get)(yt_unit_t, yt_qid_t,  yt_rate_mode_t *);
    yt_ret_t (*qos_schedule_dwrr_weight_set)(yt_unit_t, yt_qid_t, yt_queue_weight_t);
    yt_ret_t (*qos_schedule_dwrr_weight_get)(yt_unit_t, yt_qid_t,  yt_queue_weight_t *);
    yt_ret_t (*qos_intPri_vlan_map_set)(yt_unit_t, yt_vlan_t, yt_enable_t, yt_pri_t);
    yt_ret_t (*qos_intPri_vlan_map_get)(yt_unit_t, yt_vlan_t,  yt_enable_t *,  yt_pri_t *);

    yt_ret_t (*rate_init)(yt_unit_t);
    yt_ret_t (*rate_igrBandwidthCtrlEnable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*rate_igrBandwidthCtrlEnable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*rate_igrBandwidthCtrlMode_set)(yt_unit_t, yt_port_t, yt_port_rate_mode_t *);
    yt_ret_t (*rate_igrBandwidthCtrlMode_get)(yt_unit_t, yt_port_t,  yt_port_rate_mode_t *);
    yt_ret_t (*rate_igrBandwidthCtrlRate_set)(yt_unit_t, yt_port_t, yt_rate_t);
    yt_ret_t (*rate_igrBandwidthCtrlRate_get)(yt_unit_t, yt_port_t,  yt_rate_t *);
    yt_ret_t (*rate_meter_vlan_enable_set)(yt_unit_t, yt_vlan_t, yt_meterid_t, yt_enable_t);
    yt_ret_t (*rate_meter_vlan_enable_get)(yt_unit_t, yt_vlan_t,  yt_meterid_t *,  yt_enable_t *);
    yt_ret_t (*rate_meter_enable_set)(yt_unit_t, yt_meterid_t, yt_enable_t);
    yt_ret_t (*rate_meter_enable_get)(yt_unit_t, yt_meterid_t,  yt_enable_t *);
    yt_ret_t (*rate_meter_mode_set)(yt_unit_t, yt_meterid_t, yt_rate_meter_mode_t *);
    yt_ret_t (*rate_meter_mode_get)(yt_unit_t, yt_meterid_t,  yt_rate_meter_mode_t *);
    yt_ret_t (*rate_meter_rate_set)(yt_unit_t, yt_meterid_t, yt_qos_two_rate_t *);
    yt_ret_t (*rate_meter_rate_get)(yt_unit_t, yt_meterid_t,  yt_qos_two_rate_t *);
    yt_ret_t (*rate_shaping_port_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*rate_shaping_port_enable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*rate_shaping_port_mode_set)(yt_unit_t, yt_port_t, yt_shaping_mode_t *);
    yt_ret_t (*rate_shaping_port_mode_get)(yt_unit_t, yt_port_t, yt_shaping_mode_t *);
    yt_ret_t (*rate_shaping_port_rate_set)(yt_unit_t, yt_port_t, yt_rate_t);
    yt_ret_t (*rate_shaping_port_rate_get)(yt_unit_t, yt_port_t,  yt_rate_t *);
    yt_ret_t (*rate_shaping_queue_enable_set)(yt_unit_t, yt_qid_t, yt_enable_t, yt_enable_t);
    yt_ret_t (*rate_shaping_queue_enable_get)(yt_unit_t, yt_qid_t, yt_enable_t *,  yt_enable_t *);
    yt_ret_t (*rate_shaping_queue_mode_set)(yt_unit_t, yt_qid_t, yt_shaping_mode_t *);
    yt_ret_t (*rate_shaping_queue_mode_get)(yt_unit_t, yt_qid_t, yt_shaping_mode_t *);
    yt_ret_t (*rate_shaping_queue_rate_set)(yt_unit_t, yt_qid_t, yt_qos_two_rate_t *);
    yt_ret_t (*rate_shaping_queue_rate_get)(yt_unit_t, yt_qid_t, yt_qos_two_rate_t *);

    yt_ret_t (*stat_mib_init)(yt_unit_t);     
    yt_ret_t (*stat_mib_enable_set)(yt_unit_t, yt_enable_t);      
    yt_ret_t (*stat_mib_enable_get)(yt_unit_t,  yt_enable_t *);      
    yt_ret_t (*stat_mib_clear)(yt_unit_t, yt_port_t);       
    yt_ret_t (*stat_mib_clear_all)(yt_unit_t);      
    yt_ret_t (*stat_mib_port_get)(yt_unit_t, yt_port_t,  yt_stat_mib_port_cnt_t *);
    yt_ret_t (*stat_mib_port_singleType_cnt_get)(yt_unit_t, yt_port_t, yt_stat_mib_t, uint64 *);
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
    yt_ret_t (*stp_instance_map_vlanlist_set)(yt_unit_t, yt_vlan_mask_t *, yt_stp_id_t);
    yt_ret_t (*stp_bpdu_egr_filter_set)(yt_unit_t, yt_stp_bpdu_type_t, yt_port_mask_t);
    yt_ret_t (*stp_bpdu_egr_filter_get)(yt_unit_t, yt_stp_bpdu_type_t, yt_port_mask_t *);
    yt_ret_t (*stp_bpdu_act_set)(yt_unit_t, yt_port_t, yt_stp_bpdu_type_t, yt_act_type_t);
    yt_ret_t (*stp_bpdu_act_get)(yt_unit_t, yt_port_t, yt_stp_bpdu_type_t, yt_act_type_t *);   

    yt_ret_t (*isolation_port_set)(yt_unit_t, yt_port_t, yt_port_mask_t);
    yt_ret_t (*isolation_port_get)(yt_unit_t, yt_port_t,  yt_port_mask_t *);
    yt_ret_t (*isolation_vlan_add)(yt_unit_t, yt_iso_vlan_info_t, uint16_t *);
    yt_ret_t (*isolation_vlan_del_by_index)(yt_unit_t, uint16_t);
    yt_ret_t (*isolation_vlan_set_by_index)(yt_unit_t, uint16_t, yt_iso_vlan_info_t);
    yt_ret_t (*isolation_vlan_get_by_index)(yt_unit_t, uint16_t, yt_iso_vlan_info_t *);
    yt_ret_t (*isolation_vlan_type_set)(yt_unit_t, yt_iso_vlan_type_t);
    yt_ret_t (*isolation_vlan_type_get)(yt_unit_t, yt_iso_vlan_type_t *);

    yt_ret_t (*loop_detect_enable_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*loop_detect_enable_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*loop_detect_tpid_set)(yt_unit_t, yt_tpid_t);
    yt_ret_t (*loop_detect_tpid_get)(yt_unit_t,  yt_tpid_t *);
    yt_ret_t (*loop_detect_generate_way_set)(yt_unit_t, yt_generate_way_t);
    yt_ret_t (*loop_detect_generate_way_get)(yt_unit_t,  yt_generate_way_t *);
    yt_ret_t (*loop_detect_unitID_set)(yt_unit_t, yt_local_id_t, yt_remote_id_t);
    yt_ret_t (*loop_detect_unitID_get)(yt_unit_t, yt_local_id_t *, yt_remote_id_t *);
    yt_ret_t (*loop_detect_loopedPorts_get)(yt_unit_t, yt_port_mask_t *);
    yt_ret_t (*loop_detect_interval_set)(yt_unit_t, uint32_t);
    yt_ret_t (*loop_detect_interval_get)(yt_unit_t,  uint32_t *);
    yt_ret_t (*loop_detect_prevent_enable_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*loop_detect_prevent_enable_get)(yt_unit_t,  yt_enable_t *);

    yt_ret_t (*storm_ctrl_init)(yt_unit_t);
    yt_ret_t (*storm_ctrl_enable_set)(yt_unit_t, yt_port_t, yt_storm_type_t, yt_enable_t);
    yt_ret_t (*storm_ctrl_enable_get)(yt_unit_t, yt_port_t, yt_storm_type_t,  yt_enable_t *);
    yt_ret_t (*storm_ctrl_rate_mode_set)(yt_unit_t, yt_port_t, yt_storm_type_t, yt_port_rate_mode_t);
    yt_ret_t (*storm_ctrl_rate_mode_get)(yt_unit_t, yt_port_t, yt_storm_type_t,  yt_port_rate_mode_t *);
    yt_ret_t (*storm_ctrl_rate_set)(yt_unit_t, yt_port_t, yt_storm_type_t, uint32_t);
    yt_ret_t (*storm_ctrl_rate_get)(yt_unit_t, yt_port_t, yt_storm_type_t,  uint32_t *);

    yt_ret_t (*lag_init)(yt_unit_t);
    yt_ret_t (*lag_en_get)(yt_unit_t, yt_port_t, yt_enable_t *, uint8_t *);
    yt_ret_t (*lag_hash_sel_set)(yt_unit_t, uint8_t);
    yt_ret_t (*lag_hash_sel_get)(yt_unit_t,  uint8_t *);
    yt_ret_t (*lag_group_port_set)(yt_unit_t, uint8_t, yt_port_mask_t);
    yt_ret_t (*lag_group_port_get)(yt_unit_t, uint8_t, yt_port_mask_t *);

    yt_ret_t (*l2_init)(yt_unit_t);
    yt_ret_t (*l2_fdb_ucast_addr_add)(yt_unit_t, const l2_ucastMacAddr_info_t *);
    yt_ret_t (*l2_fdb_ucast_addr_del)(yt_unit_t, yt_vlan_t , yt_mac_addr_t);
    yt_ret_t (*l2_fdb_uc_withindex_get)(yt_unit_t, uint16_t , l2_ucastMacAddr_info_t *);
    yt_ret_t (*l2_fdb_uc_withMacAndVid_get)(yt_unit_t, yt_vlan_t , yt_mac_addr_t , l2_ucastMacAddr_info_t *);
    yt_ret_t (*l2_fdb_uc_withindex_getnext)(yt_unit_t, uint16_t , uint16_t *, l2_ucastMacAddr_info_t *);
    yt_ret_t (*l2_fdb_type_get)(yt_unit_t, yt_vlan_t , yt_mac_addr_t , yt_l2_fdb_type_t *);
    yt_ret_t (*l2_fdb_all_ucast_flush)(yt_unit_t);
    yt_ret_t (*l2_fdb_port_ucast_flush)(yt_unit_t, yt_port_t , yt_bool_t);
    yt_ret_t (*l2_fdb_vlan_ucast_flush)(yt_unit_t, yt_vlan_t );
    yt_ret_t (*l2_fdb_vlan_port_ucast_flush)(yt_unit_t, yt_vlan_t , yt_port_t , yt_bool_t);
    yt_ret_t (*l2_fdb_uc_cnt_get)(yt_unit_t,  uint32 *);
    yt_ret_t (*l2_fdb_port_uc_cnt_get)(yt_unit_t, yt_port_t , uint32 *);
    yt_ret_t (*l2_fdb_lag_uc_cnt_get)(yt_unit_t, uint8_t , uint32_t *);
    yt_ret_t (*l2_fdb_aging_port_en_set)(yt_unit_t, yt_port_t , yt_enable_t );
    yt_ret_t (*l2_fdb_aging_port_en_get)(yt_unit_t, yt_port_t , yt_enable_t *);
    yt_ret_t (*l2_fdb_aging_time_set)(yt_unit_t,  uint32_t );
    yt_ret_t (*l2_fdb_aging_time_get)(yt_unit_t ,  uint32_t *);
    yt_ret_t (*l2_fdb_linkdownFlush_en_set)(yt_unit_t , yt_enable_t );
    yt_ret_t (*l2_fdb_linkdownFlush_en_get)(yt_unit_t , yt_enable_t *);
    yt_ret_t (*l2_fdb_add_notify_register)(yt_unit_t, l2_fdb_add_notify, yt_l2_fdb_notify_type_t);
    yt_ret_t (*l2_fdb_del_notify_register)(yt_unit_t, l2_fdb_del_notify, yt_l2_fdb_notify_type_t);
    yt_ret_t (*l2_fdb_uc_get_first_from_db)(yt_unit_t, l2_ucastMacAddr_info_t *);
    yt_ret_t (*l2_fdb_uc_get_next_from_db)(yt_unit_t, yt_vlan_t, yt_mac_addr_t, l2_ucastMacAddr_info_t *);
    yt_ret_t (*l2_port_learnlimit_en_set)(yt_unit_t , yt_port_t , yt_enable_t );
    yt_ret_t (*l2_port_learnlimit_en_get)(yt_unit_t , yt_port_t , yt_enable_t *);
    yt_ret_t (*l2_port_learnlimit_cnt_set)(yt_unit_t , yt_port_t , uint32_t );
    yt_ret_t (*l2_port_learnlimit_cnt_get)(yt_unit_t , yt_port_t , uint32_t *);
    yt_ret_t (*l2_port_learnlimit_act_set)(yt_unit_t , yt_port_t , yt_act_type_t );
    yt_ret_t (*l2_port_learnlimit_act_get)(yt_unit_t , yt_port_t , yt_act_type_t *);
    yt_ret_t (*l2_system_learnlimit_en_set)(yt_unit_t , yt_enable_t );
    yt_ret_t (*l2_system_learnlimit_en_get)(yt_unit_t , yt_enable_t *);
    yt_ret_t (*l2_system_learnlimit_cnt_set)(yt_unit_t , uint32_t );
    yt_ret_t (*l2_system_learnlimit_cnt_get)(yt_unit_t , uint32_t *);
    yt_ret_t (*l2_system_learnlimit_act_set)(yt_unit_t , yt_act_type_t );
    yt_ret_t (*l2_system_learnlimit_act_get)(yt_unit_t , yt_act_type_t *);
    yt_ret_t (*l2_lag_learnlimit_en_set)(yt_unit_t , uint8_t ,  yt_enable_t );
    yt_ret_t (*l2_lag_learnlimit_en_get)(yt_unit_t , uint8_t ,  yt_enable_t *);
    yt_ret_t (*l2_lag_learnlimit_cnt_set)(yt_unit_t , uint8_t ,  uint32_t );
    yt_ret_t (*l2_lag_learnlimit_cnt_get)(yt_unit_t , uint8_t ,  uint32_t *);
    yt_ret_t (*l2_lag_learnlimit_act_set)(yt_unit_t , uint8_t , yt_act_type_t );
    yt_ret_t (*l2_lag_learnlimit_act_get)(yt_unit_t , uint8_t , yt_act_type_t *);
    yt_ret_t (*l2_port_learn_en_set)(yt_unit_t , yt_port_t , yt_enable_t );
    yt_ret_t (*l2_port_learn_en_get)(yt_unit_t , yt_port_t , yt_enable_t *);
    yt_ret_t (*l2_port_learn_mode_set)(yt_unit_t, yt_port_t, yt_l2_learn_mode_t);
    yt_ret_t (*l2_port_learn_mode_get)(yt_unit_t, yt_port_t, yt_l2_learn_mode_t *);
    yt_ret_t (*l2_port_suspend_act_set)(yt_unit_t, yt_port_t, yt_act_type_t);
    yt_ret_t (*l2_port_suspend_act_get)(yt_unit_t, yt_port_t, yt_act_type_t *);
    yt_ret_t (*l2_new_sa_act_set)(yt_unit_t, yt_port_t, yt_act_type_t);
    yt_ret_t (*l2_new_sa_act_get)(yt_unit_t, yt_port_t, yt_act_type_t *);
    yt_ret_t (*l2_unknown_ucast_act_set)(yt_unit_t, yt_port_t, yt_act_type_t);
    yt_ret_t (*l2_unknown_ucast_act_get)(yt_unit_t, yt_port_t, yt_act_type_t *);
    yt_ret_t (*l2_unknown_mcast_act_set)(yt_unit_t, yt_port_t, yt_act_type_t);
    yt_ret_t (*l2_unknown_mcast_act_get)(yt_unit_t, yt_port_t, yt_act_type_t *);
    yt_ret_t (*l2_filter_mcast_set)(yt_unit_t, yt_port_mask_t);
    yt_ret_t (*l2_filter_mcast_get)(yt_unit_t,  yt_port_mask_t *);
    yt_ret_t (*l2_filter_bcast_set)(yt_unit_t, yt_port_mask_t);
    yt_ret_t (*l2_filter_bcast_get)(yt_unit_t,  yt_port_mask_t *);
    yt_ret_t (*l2_filter_unknown_ucast_set)(yt_unit_t, yt_port_mask_t);
    yt_ret_t (*l2_filter_unknown_ucast_get)(yt_unit_t,  yt_port_mask_t *);
    yt_ret_t (*l2_filter_unknown_mcast_set)(yt_unit_t, yt_port_mask_t);
    yt_ret_t (*l2_filter_unknown_mcast_get)(yt_unit_t,  yt_port_mask_t *);

    yt_ret_t (*multicast_ipv4_add)(yt_unit_t, yt_mcast_mode_t, const yt_mcast_ipv4_info_t *, yt_port_mask_t);
    yt_ret_t (*multicast_ipv6_add)(yt_unit_t, yt_mcast_mode_t, const yt_mcast_ipv6_info_t *, yt_port_mask_t);
    yt_ret_t (*multicast_macaddr_add)(yt_unit_t, yt_vlan_t, yt_mac_addr_t, yt_port_mask_t);
    yt_ret_t (*multicast_ipv4_del)(yt_unit_t, yt_mcast_mode_t, const yt_mcast_ipv4_info_t *);
    yt_ret_t (*multicast_ipv6_del)(yt_unit_t, yt_mcast_mode_t, const yt_mcast_ipv6_info_t *);
    yt_ret_t (*multicast_macaddr_del)(yt_unit_t, yt_vlan_t, yt_mac_addr_t);
    yt_ret_t (*multicast_ipv4_withIpAndVid_get)(yt_unit_t, yt_mcast_mode_t, const yt_mcast_ipv4_info_t *,  yt_mcast_ipv4_data_t *);
    yt_ret_t (*multicast_ipv4_withindex_get)(yt_unit_t, uint16_t, yt_mcast_ipv4_data_t *);
    yt_ret_t (*multicast_ipv4_withindex_getnext)(yt_unit_t, uint16_t, uint16_t *, yt_mcast_ipv4_data_t *);
    yt_ret_t (*multicast_ipv6_withIpAndVid_get)(yt_unit_t, yt_mcast_mode_t, const yt_mcast_ipv6_info_t *,  yt_mcast_ipv6_data_t *);
    yt_ret_t (*multicast_ipv6_withindex_get)(yt_unit_t, uint16_t, yt_mcast_ipv6_data_t *);
    yt_ret_t (*multicast_ipv6_withindex_getnext)(yt_unit_t, uint16_t, uint16_t *, yt_mcast_ipv6_data_t *);
    yt_ret_t (*multicast_macaddr_withMacAndVid_get)(yt_unit_t, yt_vlan_t, yt_mac_addr_t, yt_mcast_mac_data_t *);
    yt_ret_t (*multicast_macaddr_withindex_get)(yt_unit_t, uint16_t, yt_mcast_mac_data_t *);
    yt_ret_t (*multicast_macaddr_withindex_getnext)(yt_unit_t, uint16_t,  uint16_t *, yt_mcast_mac_data_t *);
    yt_ret_t (*multicast_all_ipmc_flush)(yt_unit_t);
    yt_ret_t (*multicast_vid_ipmc_flush)(yt_unit_t, yt_vlan_t);
    yt_ret_t (*multicast_all_macaddr_flush)(yt_unit_t);
    yt_ret_t (*multicast_vid_macaddr_flush)(yt_unit_t, yt_vlan_t);
    yt_ret_t (*multicast_port_macaddr_flush)(yt_unit_t, yt_port_t);
    yt_ret_t (*multicast_macaddr_cnt_get)(yt_unit_t,  uint32 *);
    yt_ret_t (*multicast_vlan_add)(yt_unit_t, yt_vlan_t);
    yt_ret_t (*multicast_vlan_get)(yt_unit_t,  yt_multi_vlan_t *);
    yt_ret_t (*multicast_vlan_del)(yt_unit_t, yt_vlan_t);
    yt_ret_t (*multicast_hwmc_flush_all)(yt_unit_t unit, yt_mcast_hw_mode_t mode);
    yt_ret_t (*multicast_hwmc_flush_by_vid)(yt_unit_t unit, yt_mcast_hw_mode_t mode, yt_vlan_t vid);
    yt_ret_t (*multicast_hwmc_flush_by_portmask)(yt_unit_t unit, yt_mcast_hw_mode_t mode, yt_port_mask_t portMask);
    yt_ret_t (*multicast_hwmc_mac_del)(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_port_mask_t portMask);
    yt_ret_t (*multicast_hwmc_ipv4_del)(yt_unit_t unit, yt_vlan_t vid, yt_ip_addr_t gip, yt_port_mask_t portMask);

#if defined(SWITCH_SERIES_TIGER)
    yt_ret_t (*igmp_opmode_set)(yt_unit_t, yt_igmp_mld_op_mode_t);
    yt_ret_t (*igmp_opmode_get)(yt_unit_t,   yt_igmp_mld_op_mode_t *);
    yt_ret_t (*mld_opmode_set)(yt_unit_t, yt_igmp_mld_op_mode_t);
    yt_ret_t (*mld_opmode_get)(yt_unit_t,   yt_igmp_mld_op_mode_t *);
    yt_ret_t (*igmp_mld_port_report_allow_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*igmp_mld_port_report_allow_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*igmp_mld_port_leave_allow_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*igmp_mld_port_leave_allow_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*igmp_mld_port_query_allow_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*igmp_mld_port_query_allow_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*igmp_mld_fastleave_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*igmp_mld_fastleave_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*igmp_mld_learnlimit_en_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*igmp_mld_learnlimit_en_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*igmp_mld_learnlimit_maxgroup_set)(yt_unit_t, uint32_t);
    yt_ret_t (*igmp_mld_learnlimit_maxgroup_get)(yt_unit_t,  uint32_t  *);
    yt_ret_t (*igmp_mld_fwd_routerport_only_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*igmp_mld_fwd_routerport_only_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*igmp_mld_fwd_routerport_primary_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*igmp_mld_fwd_routerport_primary_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*igmp_mld_bypass_grouprange_set)(yt_unit_t, yt_igmp_mld_learn_bypass_range_t);
    yt_ret_t (*igmp_mld_bypass_grouprange_get)(yt_unit_t,  yt_igmp_mld_learn_bypass_range_t *);
    yt_ret_t (*igmp_mld_dynamic_routerport_get)(yt_unit_t,  yt_igmp_mld_router_port_t *);
    yt_ret_t (*igmp_mld_dynamic_routerport_allow_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*igmp_mld_dynamic_routerport_allow_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*igmp_mld_dynamic_routerport_agingtime_set)(yt_unit_t, uint32_t);
    yt_ret_t (*igmp_mld_dynamic_routerport_agingtime_get)(yt_unit_t,  uint32_t *);
    yt_ret_t (*igmp_mld_static_routerport_set)(yt_unit_t, yt_port_mask_t);
    yt_ret_t (*igmp_mld_static_routerport_get)(yt_unit_t,  yt_port_mask_t *);
    yt_ret_t (*igmp_mld_bypass_portIsolation_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*igmp_mld_bypass_portIsolation_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*igmp_mld_ipmc_bypass_portIsolation_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*igmp_mld_ipmc_bypass_portIsolation_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*igmp_mld_bypass_vlanIngrfilter_en_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*igmp_mld_bypass_vlanIngrfilter_en_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
#endif

#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    yt_ret_t (*igmp_mld_init)(yt_unit_t unit);
    yt_ret_t (*igmp_dynamicRouterPort_aging_time_set)(yt_unit_t unit,  uint32_t sec);
    yt_ret_t (*igmp_dynamicRouterPort_aging_time_get)(yt_unit_t unit,  uint32_t *pSec);
    yt_ret_t (*igmp_dynamicRouterPort_get)(yt_unit_t unit,  uint8_t idx, yt_vlan_t *pVid, yt_port_t *pPort, uint32_t *pTime);
    yt_ret_t (*igmp_forbidden_dynamicRouterPort_en_set)(yt_unit_t unit,  yt_port_t port, yt_enable_t enable);
    yt_ret_t (*igmp_forbidden_dynamicRouterPort_en_get)(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable);
    yt_ret_t (*igmp_staticRouterPort_add_by_idx)(yt_unit_t unit,  uint8_t idx, yt_vlan_t vid, yt_port_mask_t portMask);
    yt_ret_t (*igmp_staticRouterPort_get)(yt_unit_t unit,  uint8_t idx, yt_vlan_t *pVid, yt_port_mask_t *pPortMask);
    yt_ret_t (*igmp_staticRouterPort_del)(yt_unit_t unit,  uint8_t idx);
    yt_ret_t (*igmp_fast_leave_en_set)(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t enable);
    yt_ret_t (*igmp_fast_leave_en_get)(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t *pEnable);
    yt_ret_t (*igmp_report_leave_fwd_ctrl_set)(yt_unit_t unit, yt_igmp_version_t version, yt_igmp_mld_fwd_ports_t fwdCtrl);
    yt_ret_t (*igmp_report_leave_fwd_ctrl_get)(yt_unit_t unit, yt_igmp_version_t version, yt_igmp_mld_fwd_ports_t *pFwdCtrl);
    yt_ret_t (*igmp_ctrlPkt_act_set)(yt_unit_t unit,  yt_port_t port, yt_act_type_t actType);
    yt_ret_t (*igmp_ctrlPkt_act_get)(yt_unit_t unit,  yt_port_t port, yt_act_type_t *pActType);
    yt_ret_t (*igmp_v1v2_ctrlPkt_act_set)(yt_unit_t unit,  yt_igmp_v1v2_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t actType);
    yt_ret_t (*igmp_v1v2_ctrlPkt_act_get)(yt_unit_t unit,  yt_igmp_v1v2_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t *pActType);
    yt_ret_t (*igmp_v3_ctrlPkt_act_set)(yt_unit_t unit,  yt_igmp_v3_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t actType);
    yt_ret_t (*igmp_v3_ctrlPkt_act_get)(yt_unit_t unit,  yt_igmp_v3_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t *pActType);
    yt_ret_t (*igmp_v1v2_port_learn_en_set)(yt_unit_t unit,  yt_port_t port, yt_enable_t enable);
    yt_ret_t (*igmp_v1v2_port_learn_en_get)(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable);
    yt_ret_t (*igmp_v3_port_learn_en_set)(yt_unit_t unit,  yt_port_t port, yt_enable_t enable);
    yt_ret_t (*igmp_v3_port_learn_en_get)(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable);
    yt_ret_t (*igmp_routerPort_dataFwd_en_set)(yt_unit_t unit, yt_enable_t enable);
    yt_ret_t (*igmp_routerPort_dataFwd_en_get)(yt_unit_t unit, yt_enable_t *pEnable);
    yt_ret_t (*mld_dynamicRouterPort_aging_time_set)(yt_unit_t unit,  uint32_t sec);
    yt_ret_t (*mld_dynamicRouterPort_aging_time_get)(yt_unit_t unit,  uint32_t *pSec);
    yt_ret_t (*mld_dynamicRouterPort_get)(yt_unit_t unit,  uint8_t idx, yt_vlan_t *pVid, yt_port_t *pPort, uint32_t *pTime);
    yt_ret_t (*mld_forbidden_dynamicRouterPort_en_set)(yt_unit_t unit,  yt_port_t port, yt_enable_t enable);
    yt_ret_t (*mld_forbidden_dynamicRouterPort_en_get)(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable);
    yt_ret_t (*mld_staticRouterPort_add_by_idx)(yt_unit_t unit,  uint8_t idx, yt_vlan_t vid, yt_port_mask_t portMask);
    yt_ret_t (*mld_staticRouterPort_get)(yt_unit_t unit,  uint8_t idx, yt_vlan_t *pVid, yt_port_mask_t *pPortMask);
    yt_ret_t (*mld_staticRouterPort_del)(yt_unit_t unit,  uint8_t idx);
    yt_ret_t (*mld_fast_leave_en_set)(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t enable);
    yt_ret_t (*mld_fast_leave_en_get)(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t *pEnable);
    yt_ret_t (*mld_report_leave_fwd_ctrl_set)(yt_unit_t unit, yt_mld_version_t version, yt_igmp_mld_fwd_ports_t fwdCtrl);
    yt_ret_t (*mld_report_leave_fwd_ctrl_get)(yt_unit_t unit, yt_mld_version_t version, yt_igmp_mld_fwd_ports_t *pFwdCtrl);
    yt_ret_t (*mld_ctrlPkt_act_set)(yt_unit_t unit,  yt_port_t port, yt_act_type_t actType);
    yt_ret_t (*mld_ctrlPkt_act_get)(yt_unit_t unit,  yt_port_t port, yt_act_type_t *pActType);
    yt_ret_t (*mld_v1_ctrlPkt_act_set)(yt_unit_t unit,  yt_mld_v1_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t actType);
    yt_ret_t (*mld_v1_ctrlPkt_act_get)(yt_unit_t unit,  yt_mld_v1_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t *pActType);
    yt_ret_t (*mld_v2_ctrlPkt_act_set)(yt_unit_t unit,  yt_mld_v2_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t actType);
    yt_ret_t (*mld_v2_ctrlPkt_act_get)(yt_unit_t unit,  yt_mld_v2_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t *pActType);
    yt_ret_t (*mld_v1_port_learn_en_set)(yt_unit_t unit,  yt_port_t port, yt_enable_t enable);
    yt_ret_t (*mld_v1_port_learn_en_get)(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable);
    yt_ret_t (*mld_v2_port_learn_en_set)(yt_unit_t unit,  yt_port_t port, yt_enable_t enable);
    yt_ret_t (*mld_v2_port_learn_en_get)(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable);
    yt_ret_t (*mld_routerPort_dataFwd_en_set)(yt_unit_t unit, yt_enable_t enable);
    yt_ret_t (*mld_routerPort_dataFwd_en_get)(yt_unit_t unit, yt_enable_t *pEnable);
    yt_ret_t (*igmp_mld_system_learnlimit_set)(yt_unit_t unit, yt_enable_t enable, uint16_t maxNum);
    yt_ret_t (*igmp_mld_system_learnlimit_get)(yt_unit_t unit, yt_enable_t *pEnable, uint16_t *pMaxNum);
    yt_ret_t (*igmp_mld_system_learnCnt_get)(yt_unit_t unit, uint16_t *pCnt);
    yt_ret_t (*igmp_mld_port_learnlimit_set)(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t enable, uint16_t maxNum);
    yt_ret_t (*igmp_mld_port_learnlimit_get)(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t *pEnable, uint16_t *pMaxNum);
    yt_ret_t (*igmp_mld_port_learnCnt_get)(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, uint16_t *pCnt);
    yt_ret_t (*igmp_mld_memberports_aging_time_set)(yt_unit_t unit,  uint32_t sec);
    yt_ret_t (*igmp_mld_memberports_aging_time_get)(yt_unit_t unit,  uint32_t *pSec);
    yt_ret_t (*igmp_mld_memberport_time_get)(yt_unit_t unit,  uint8_t idx, yt_port_t port, uint32_t *pTime);
    yt_ret_t (*igmp_mld_leaveports_aging_time_set)(yt_unit_t unit,  yt_igmp_mld_leave_aging_t agingEnum);
    yt_ret_t (*igmp_mld_leaveports_aging_time_get)(yt_unit_t unit,  yt_igmp_mld_leave_aging_t *pAgingEnum);
#endif

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
    yt_ret_t (*nic_int_cputag_en_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*nic_int_cputag_en_get)(yt_unit_t,   yt_enable_t *);
    yt_ret_t (*nic_cpu_pkt_keepAll_en_set)(yt_unit_t, yt_enable_t, yt_enable_t);
    yt_ret_t (*nic_cpu_pkt_keepAll_en_get)(yt_unit_t, yt_enable_t *, yt_enable_t *);
    yt_ret_t (*nic_cputag_mode_set)(yt_unit_t, yt_cputag_mode_t);
    yt_ret_t (*nic_cputag_mode_get)(yt_unit_t, yt_cputag_mode_t *);

    yt_ret_t (*mirror_init)(yt_unit_t);
    yt_ret_t (*mirror_group_set)(yt_unit_t, yt_mirror_group_t, yt_mirror_entry_t *);
    yt_ret_t (*mirror_group_get)(yt_unit_t, yt_mirror_group_t, yt_mirror_entry_t *);

	yt_ret_t (*rspan_vlan_info_set)(yt_unit_t, yt_mirror_group_t, yt_rspan_vlan_info_t *);
    yt_ret_t (*rspan_vlan_info_get)(yt_unit_t, yt_mirror_group_t, yt_rspan_vlan_info_t *);
    yt_ret_t (*rspan_smac_learn_set)(yt_unit_t, yt_mirror_group_t, yt_enable_t);
    yt_ret_t (*rspan_smac_learn_get)(yt_unit_t, yt_mirror_group_t, yt_enable_t *);
    yt_ret_t (*rspan_src_dev_tag_op_set)(yt_unit_t, yt_mirror_group_t, yt_rspan_src_dev_tag_op_t);
    yt_ret_t (*rspan_src_dev_tag_op_get)(yt_unit_t, yt_mirror_group_t, yt_rspan_src_dev_tag_op_t *);
    yt_ret_t (*rspan_dst_dev_tag_op_set)(yt_unit_t, yt_mirror_group_t, yt_rspan_dst_dev_tag_op_t);
    yt_ret_t (*rspan_dst_dev_tag_op_get)(yt_unit_t, yt_mirror_group_t, yt_rspan_dst_dev_tag_op_t *);
    yt_ret_t (*rspan_egr_enable_set)(yt_unit_t, yt_mirror_group_t, yt_enable_t);
    yt_ret_t (*rspan_egr_enable_get)(yt_unit_t, yt_mirror_group_t, yt_enable_t *);
    yt_ret_t (*rspan_igr_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*rspan_igr_enable_get)(yt_unit_t, yt_port_t, yt_enable_t *);
    yt_ret_t (*rspan_fwd_pkt_filter_enable_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*rspan_fwd_pkt_filter_enable_get)(yt_unit_t, yt_enable_t *);

    yt_ret_t (*rma_action_set)(yt_unit_t, yt_rma_da_t, yt_act_type_t);
    yt_ret_t (*rma_action_get)(yt_unit_t, yt_rma_da_t,  yt_act_type_t *);
    yt_ret_t (*rma_cpu_code_set)(yt_unit_t, yt_rma_da_t, yt_rma_cpu_code_t);
    yt_ret_t (*rma_cpu_code_get)(yt_unit_t, yt_rma_da_t,  yt_rma_cpu_code_t *);
    yt_ret_t (*rma_bypass_set)(yt_unit_t, yt_rma_da_t, yt_rma_bypass_t);
    yt_ret_t (*rma_bypass_get)(yt_unit_t, yt_rma_da_t, yt_rma_bypass_t *);
    yt_ret_t (*rma_sa_learn_dis_set)(yt_unit_t, yt_rma_da_t, yt_enable_t);
    yt_ret_t (*rma_sa_learn_dis_get)(yt_unit_t, yt_rma_da_t, yt_enable_t *);

    yt_ret_t (*wol_switch_enable_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*wol_switch_enable_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*wol_switch_etherType_set)(yt_unit_t, yt_tpid_t);
    yt_ret_t (*wol_switch_etherType_get)(yt_unit_t,  yt_tpid_t *);
    yt_ret_t (*wol_port_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*wol_port_enable_get)(yt_unit_t,  yt_port_t, yt_enable_t *);
    yt_ret_t (*wol_port_macAddr_set)(yt_unit_t, yt_port_t, yt_mac_addr_t);
    yt_ret_t (*wol_port_macAddr_get)(yt_unit_t, yt_port_t, yt_mac_addr_t *);

    yt_ret_t (*sys_mac_get)(yt_unit_t , yt_mac_addr_t * );
    yt_ret_t (*sys_chip_reset)(yt_unit_t);
    yt_ret_t (*sys_database_reset)(yt_unit_t);
    yt_ret_t (*sys_chipInfo_get)(yt_unit_t,  yt_switch_chip_t *);

    yt_ret_t (*port_init)(yt_unit_t);
    yt_ret_t (*port_default_cfg_set)(yt_unit_t);
    yt_ret_t (*port_mac_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*port_mac_enable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
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
    yt_ret_t (*port_mac_force_set)(yt_unit_t, yt_port_t, yt_port_speed_duplex_t);
    yt_ret_t (*port_mac_force_get)(yt_unit_t, yt_port_t, yt_port_speed_duplex_t *);
    yt_ret_t (*port_mac_fc_set)(yt_unit_t, yt_port_t, yt_enable_t, yt_enable_t, yt_enable_t);
    yt_ret_t (*port_mac_fc_get)(yt_unit_t, yt_port_t,  yt_enable_t *, yt_enable_t *, yt_enable_t *);
    yt_ret_t (*port_mac_block_set)(yt_unit_t, yt_port_t, yt_enable_t, yt_enable_t);
    yt_ret_t (*port_mac_block_get)(yt_unit_t, yt_port_t,  yt_enable_t *, yt_enable_t *);
    yt_ret_t (*port_mac_eee_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*port_mac_eee_enable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*port_mac_eee_timer_set)(yt_unit_t, yt_port_t, uint32_t, uint32_t);
    yt_ret_t (*port_mac_eee_timer_get)(yt_unit_t, yt_port_t, uint32_t *, uint32_t *);
    yt_ret_t (*port_extif_mode_set)(yt_unit_t, yt_port_t, yt_extif_mode_t);
    yt_ret_t (*port_extif_mode_get)(yt_unit_t, yt_port_t,  yt_extif_mode_t *);
    yt_ret_t (*port_extif_force_set)(yt_unit_t, yt_port_t, yt_extif_force_ctrl_t);
    yt_ret_t (*port_extif_force_get)(yt_unit_t, yt_port_t, yt_extif_force_ctrl_t *);
    yt_ret_t (*port_extif_rgmii_delay_set)(yt_unit_t, yt_port_t, uint8_t, uint8_t, yt_enable_t);
    yt_ret_t (*port_extif_rgmii_delay_get)(yt_unit_t, yt_port_t,  uint8_t *,  uint8_t *,  yt_enable_t *);
    yt_ret_t (*port_phy_enable_set)(yt_unit_t, yt_port_t, yt_phy_chip_mode_t, yt_enable_t);
    yt_ret_t (*port_phy_enable_get)(yt_unit_t, yt_port_t,  yt_phy_chip_mode_t, yt_enable_t *);
    yt_ret_t (*port_phyAutoNeg_enable_set)(yt_unit_t, yt_port_t, yt_phy_chip_mode_t, yt_enable_t);
    yt_ret_t (*port_phyAutoNeg_enable_get)(yt_unit_t, yt_port_t,  yt_phy_chip_mode_t, yt_enable_t *);
    yt_ret_t (*port_phyAutoNeg_ability_set)(yt_unit_t, yt_port_t, yt_phy_chip_mode_t, yt_port_an_ability_t);
    yt_ret_t (*port_phyAutoNeg_ability_get)(yt_unit_t, yt_port_t,  yt_phy_chip_mode_t, yt_port_an_ability_t *);
    yt_ret_t (*port_phy_force_set)(yt_unit_t, yt_port_t, yt_phy_chip_mode_t, yt_port_speed_duplex_t);
    yt_ret_t (*port_phy_force_get)(yt_unit_t, yt_port_t,  yt_phy_chip_mode_t, yt_port_speed_duplex_t *);
    yt_ret_t (*port_phy_linkstatus_get)(yt_unit_t, yt_port_t,  yt_phy_chip_mode_t, yt_port_linkStatus_all_t *);
    yt_ret_t (*port_phy_reg_set)(yt_unit_t, yt_port_t, uint32_t, uint16_t, yt_phy_type_t, yt_phy_reg_type_t regType);
    yt_ret_t (*port_phy_reg_get)(yt_unit_t, yt_port_t, uint32_t,  uint16_t *, yt_phy_type_t, yt_phy_reg_type_t regType);
    yt_ret_t (*port_phy_eee_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*port_phy_eee_enable_get)(yt_unit_t, yt_port_t, yt_enable_t *);
    yt_ret_t (*port_phy_eee_status_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
#if defined(SWITCH_SERIES_TIGER)
    yt_ret_t (*port_jumbo_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*port_jumbo_enable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*port_jumbo_size_set)(yt_unit_t, yt_port_t, uint32_t);
    yt_ret_t (*port_jumbo_size_get)(yt_unit_t, yt_port_t,  uint32_t *);
#else
    yt_ret_t (*port_jumbo_enable_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*port_jumbo_enable_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*port_jumbo_size_set)(yt_unit_t, uint32_t);
    yt_ret_t (*port_jumbo_size_get)(yt_unit_t,  uint32_t *);
#endif
    yt_ret_t (*port_cable_diag_start)(yt_unit_t, yt_port_t);
    yt_ret_t (*port_cable_diag_result_get)(yt_unit_t, yt_port_t,  yt_port_cableDiag_t *);
    yt_ret_t (*port_phyCrossover_mode_set)(yt_unit_t, yt_port_t, yt_utp_crossover_mode_t);
    yt_ret_t (*port_phyCrossover_mode_get)(yt_unit_t, yt_port_t,  yt_utp_crossover_mode_t *);
    yt_ret_t (*port_phy_chip_mode_set)(yt_unit_t, yt_port_t, yt_phy_chip_mode_t);
    yt_ret_t (*port_phy_chip_mode_get)(yt_unit_t, yt_port_t,  yt_phy_chip_mode_t *);
    yt_ret_t (*port_phyCombo_mode_set)(yt_unit_t, yt_port_t, yt_combo_mode_t);
    yt_ret_t (*port_phyCombo_mode_get)(yt_unit_t, yt_port_t,  yt_combo_mode_t *);
    yt_ret_t (*port_phyCrossover_status_get)(yt_unit_t, yt_port_t, yt_utp_crossover_status_t *);
    yt_ret_t (*port_phy_green_start)(yt_unit_t, yt_port_t);
    yt_ret_t (*port_phy_green_result_get)(yt_unit_t, yt_port_t, yt_port_cableDiag_t *);
    yt_ret_t (*port_phy_smart_downgrade_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*port_phy_smart_downgrade_get)(yt_unit_t, yt_port_t, yt_enable_t *);
    yt_ret_t (*port_extif_xmiiClk_invert_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*port_extif_xmiiClk_invert_get)(yt_unit_t, yt_port_t, yt_enable_t *);
    yt_ret_t (*port_parallel_detection_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*port_parallel_detection_get)(yt_unit_t, yt_port_t, yt_enable_t *);
    yt_ret_t (*port_dvddio_power_level_set)(yt_unit_t unit, yt_dvddio_power_pad_t  powerPad, yt_dvddio_power_level_t powerLevel);
    yt_ret_t (*port_dvddio_power_level_get)(yt_unit_t unit, yt_dvddio_power_pad_t  powerPad, yt_dvddio_power_level_t *pPowerLevel);
    yt_ret_t (*port_polling_enable_set)(yt_unit_t, yt_port_t,  yt_enable_t);
    yt_ret_t (*port_polling_enable_get)(yt_unit_t, yt_port_t,  yt_enable_t *);

    /* control packet */
    yt_ret_t (*ctrlpkt_act_set)(yt_unit_t, yt_port_t, yt_ctrlpkt_type_t, yt_act_type_t);
    yt_ret_t (*ctrlpkt_act_get)(yt_unit_t, yt_port_t, yt_ctrlpkt_type_t, yt_act_type_t *);
    yt_ret_t (*ctrlpkt_myMac_act_set)(yt_unit_t, yt_ctrlpkt_myMac_act_t *);
    yt_ret_t (*ctrlpkt_myMac_act_get)(yt_unit_t, yt_ctrlpkt_myMac_act_t *);
    yt_ret_t (*ctrlpkt_bypass_set)(yt_unit_t, yt_port_t, yt_ctrlpkt_type_t, yt_ctrlpkt_bypass_t *);
    yt_ret_t (*ctrlpkt_bypass_get)(yt_unit_t, yt_port_t, yt_ctrlpkt_type_t, yt_ctrlpkt_bypass_t *);
    yt_ret_t (*ctrlpkt_bypass_stp_set)(yt_unit_t, yt_ctrlpkt_type_t, yt_enable_t);
    yt_ret_t (*ctrlpkt_bypass_stp_get)(yt_unit_t, yt_ctrlpkt_type_t, yt_enable_t *);
    yt_ret_t (*ctrlpkt_sa_learn_set)(yt_unit_t, yt_port_t, yt_ctrlpkt_type_t, yt_enable_t);
    yt_ret_t (*ctrlpkt_sa_learn_get)(yt_unit_t, yt_port_t, yt_ctrlpkt_type_t, yt_enable_t *);
    yt_ret_t (*ctrlpkt_parse_en_set)(yt_unit_t, yt_ctrlpkt_parse_type_t, yt_enable_t);
    yt_ret_t (*ctrlpkt_parse_en_get)(yt_unit_t, yt_ctrlpkt_parse_type_t, yt_enable_t *);
    yt_ret_t (*ctrlpkt_rrpp_parse_set)(yt_unit_t, yt_ctrlpkt_rrpp_parse_t *);
    yt_ret_t (*ctrlpkt_rrpp_parse_get)(yt_unit_t, yt_ctrlpkt_rrpp_parse_t *);

    /* interrupt */
    yt_ret_t (*int_polarity_set)(yt_unit_t, yt_int_polarity_t);
    yt_ret_t (*int_polarity_get)(yt_unit_t,  yt_int_polarity_t *);
    yt_ret_t (*int_control_set)(yt_unit_t, yt_int_type_t, yt_enable_t);
    yt_ret_t (*int_control_get)(yt_unit_t, yt_int_type_t,  yt_enable_t *);
    yt_ret_t (*int_status_get)(yt_unit_t,  yt_intr_status_t *);
    yt_ret_t (*int_typeStatus_get)(yt_unit_t, yt_int_type_t, yt_intr_status_t,  yt_bool_t *);
    yt_ret_t (*int_port_int_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*int_port_int_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*int_allPortIntStatus_get)(yt_unit_t,  yt_intr_status_t *);
    yt_ret_t (*int_portIntStatus_get)(yt_unit_t, yt_port_t, yt_intr_status_t,  yt_bool_t *);
    yt_ret_t (*int_wol_signalOutputType_set)(yt_unit_t, yt_int_wol_outputType_t);
    yt_ret_t (*int_wol_signalOutputType_get)(yt_unit_t, yt_int_wol_outputType_t *);
    yt_ret_t (*int_wol_port_enable_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*int_wol_port_enable_get)(yt_unit_t, yt_port_t, yt_enable_t *);
    yt_ret_t (*int_wol_allPortIntStatus_get)(yt_unit_t, yt_intr_status_t *);

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
    yt_ret_t (*led_serial_outputMode_set)(yt_unit_t, yt_sled_dataNum_t);
    yt_ret_t (*led_serial_outputMode_get)(yt_unit_t, yt_sled_dataNum_t *);
    yt_ret_t (*led_serial_activeMode_set)(yt_unit_t, yt_sled_activeMode_t);
    yt_ret_t (*led_serial_activeMode_get)(yt_unit_t, yt_sled_activeMode_t *);
    yt_ret_t (*led_serial_remapping_set)(yt_unit_t, uint8_t, yt_led_remapping_t);
    yt_ret_t (*led_serial_remapping_get)(yt_unit_t, uint8_t, yt_led_remapping_t *);
    yt_ret_t (*led_parallel_output_set)(yt_unit_t, yt_port_mask_t);
    yt_ret_t (*led_parallel_output_get)(yt_unit_t, yt_port_mask_t *);
    yt_ret_t (*led_parallel_remapping_set)(yt_unit_t, yt_led_remapping_t, yt_led_remapping_t);
    yt_ret_t (*led_parallel_remapping_get)(yt_unit_t, yt_led_remapping_t, yt_led_remapping_t *);
    yt_ret_t (*led_parallel_pos_invert_set)(yt_unit_t, yt_port_t, yt_led_id_t, yt_enable_t);
    yt_ret_t (*led_parallel_pos_invert_get)(yt_unit_t, yt_port_t, yt_led_id_t, yt_enable_t *);
    yt_ret_t (*led_serial_port_info_set)(yt_unit_t, yt_led_seled_info_t);
    yt_ret_t (*led_serial_port_info_get)(yt_unit_t, yt_led_seled_info_t *); 
    yt_ret_t (*led_serial_total_ledNum_set)(yt_unit_t, yt_unit_t);
    yt_ret_t (*led_serial_total_ledNum_get)(yt_unit_t, yt_unit_t *); 

    /* gpio */
    yt_ret_t (*gpio_init)(yt_unit_t);
    yt_ret_t (*gpio_mdioMaster_pinGroup_set)(yt_unit_t, yt_mdio_master_t, yt_enable_t);
    yt_ret_t (*gpio_mdioMaster_pinGroup_get)(yt_unit_t, yt_mdio_master_t, yt_enable_t *);

#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    /* acl */
    yt_ret_t (*acl_init)(yt_unit_t);
    yt_ret_t (*acl_deinit)(yt_unit_t);
    yt_ret_t (*acl_unmatch_fwd_en_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*acl_unmatch_fwd_en_get)(yt_unit_t, yt_port_t, yt_enable_t *);
    yt_ret_t (*acl_redirect_force_en_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*acl_redirect_force_en_get)(yt_unit_t, yt_enable_t *);
    yt_ret_t (*acl_global_en_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*acl_global_en_get)(yt_unit_t, yt_enable_t *);
    yt_ret_t (*acl_port_en_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*acl_portlist_en_set)(yt_unit_t, yt_port_mask_t, yt_enable_t);
    yt_ret_t (*acl_port_en_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*acl_instance_create)(yt_unit_t, yt_acl_instance_pri_t, yt_acl_key_mask_t, yt_acl_instance_id_t *);
    yt_ret_t (*acl_instance_get)(yt_unit_t, yt_acl_instance_id_t, yt_acl_instance_t **);
    yt_ret_t (*acl_instance_exist_check)(yt_unit_t, yt_acl_instance_id_t, yt_bool_t *);
    yt_ret_t (*acl_instance_delete)(yt_unit_t, yt_acl_instance_id_t);
    yt_ret_t (*acl_entry_create)(yt_unit_t, yt_acl_instance_id_t, yt_acl_entry_id_t *);
    yt_ret_t (*acl_entry_delete)(yt_unit_t, yt_acl_instance_id_t, yt_acl_entry_id_t);
    yt_ret_t (*acl_entry_get)(yt_unit_t, yt_acl_instance_id_t, yt_acl_entry_id_t, yt_acl_entry_t **);
    yt_ret_t (*acl_entry_destory)(yt_unit_t, yt_acl_instance_id_t, yt_acl_entry_id_t);
    yt_ret_t (*acl_entry_active)(yt_unit_t, yt_acl_instance_id_t, yt_acl_entry_id_t);
    yt_ret_t (*acl_entry_inactive)(yt_unit_t, yt_acl_instance_id_t, yt_acl_entry_id_t);
    yt_ret_t (*acl_entry_pri_set)(yt_unit_t, yt_acl_instance_id_t, yt_acl_entry_id_t, uint32_t);
    yt_ret_t (*acl_key_set)(yt_unit_t, yt_acl_instance_id_t, yt_acl_entry_id_t, yt_acl_rule_key_t, yt_acl_comm_key_t *);
    yt_ret_t (*acl_key_get)(yt_unit_t, yt_acl_instance_id_t, yt_acl_entry_id_t, yt_acl_rule_key_t, yt_acl_comm_key_t *);
    yt_ret_t (*acl_key_clear)(yt_unit_t, yt_acl_instance_id_t, yt_acl_entry_id_t);
    yt_ret_t (*acl_action_add)(yt_unit_t, yt_acl_instance_id_t, yt_acl_entry_id_t,yt_acl_action_t *);
    yt_ret_t (*acl_action_get)(yt_unit_t, yt_acl_instance_id_t, yt_acl_entry_id_t,yt_acl_action_t *);
    yt_ret_t (*acl_action_del)(yt_unit_t, yt_acl_instance_id_t, yt_acl_entry_id_t);
    yt_ret_t (*acl_range_key_add)(yt_unit_t, yt_acl_range_type_t, yt_acl_rangeKey_t *, uint16_t *);
    yt_ret_t (*acl_range_key_get)(yt_unit_t, yt_acl_range_type_t, uint16_t, yt_acl_rangeKey_t *);
    yt_ret_t (*acl_range_key_del)(yt_unit_t, yt_acl_range_type_t, uint16_t);
    yt_ret_t (*acl_udf_add)(yt_unit_t, yt_acl_udf_t *, uint16_t *);
    yt_ret_t (*acl_udf_get)(yt_unit_t, uint16_t, yt_acl_udf_t *);
    yt_ret_t (*acl_udf_check_exist)(yt_unit_t, uint16_t, yt_bool_t *);
    yt_ret_t (*acl_udf_del)(yt_unit_t, uint16_t);
#ifdef SWITCH_SERIES_SHARK
    yt_ret_t (*acl_hit_en_set)(yt_unit_t, yt_acl_instance_id_t, yt_acl_entry_id_t, yt_acl_entry_id_t, yt_enable_t);
    yt_ret_t (*acl_hit_en_get)(yt_unit_t, yt_acl_instance_id_t, yt_acl_entry_id_t, yt_enable_t *);
#else
    yt_ret_t (*acl_hit_en_set)(yt_unit_t, yt_acl_instance_id_t, yt_enable_t);
    yt_ret_t (*acl_hit_en_get)(yt_unit_t, yt_acl_instance_id_t, yt_enable_t *);
#endif
    yt_ret_t (*acl_hit_get)(yt_unit_t, yt_acl_instance_id_t, yt_acl_entry_id_t, yt_bool_t *);
#elif SWITCH_SERIES_TIGER
    /* acl */
    yt_ret_t (*acl_init)(yt_unit_t);
    yt_ret_t (*acl_port_en_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*acl_port_en_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*acl_unmatch_permit_en_set)(yt_unit_t, yt_port_t, yt_enable_t);
    yt_ret_t (*acl_unmatch_permit_en_get)(yt_unit_t, yt_port_t,  yt_enable_t *);
    yt_ret_t (*acl_udf_rule_set)(yt_unit_t, uint8_t, yt_acl_udf_type_t, uint8_t);
    yt_ret_t (*acl_rule_init)(yt_unit_t);
    yt_ret_t (*acl_rule_reset)(yt_unit_t);
    yt_ret_t (*acl_rule_key_add)(yt_unit_t, yt_igrAcl_key_type_t,  yt_acl_comm_key_t *);
    yt_ret_t (*acl_rule_action_add)(yt_unit_t, yt_acl_action_type_t,  yt_acl_comm_act_t *);
    yt_ret_t (*acl_rule_create)(yt_unit_t, uint16_t, yt_bool_t,  uint32_t *);
    yt_ret_t (*acl_rule_active)(yt_unit_t, uint32_t);
    yt_ret_t (*acl_rule_del)(yt_unit_t, uint32_t);
#endif

	/* sensor */
    yt_ret_t (*sensor_temp_enable_set)(yt_unit_t, yt_enable_t);
    yt_ret_t (*sensor_temp_enable_get)(yt_unit_t,  yt_enable_t *);
    yt_ret_t (*sensor_temp_value_get)(yt_unit_t,  yt_bool_t *,  uint16_t *);

    /* debug */
    yt_ret_t (*debug_phyTemplate_test_set)(yt_unit_t, yt_port_t,  yt_utp_template_testmode_t);
    yt_ret_t (*debug_phyLoopback_test_set)(yt_unit_t, yt_port_t,  yt_phy_chip_mode_t, yt_phy_loopback_mode_t);
    yt_ret_t (*debug_utp_snr_get)(yt_unit_t, yt_port_t, yt_utp_snr_t *);

    yt_ret_t (*exit)(yt_unit_t);
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
