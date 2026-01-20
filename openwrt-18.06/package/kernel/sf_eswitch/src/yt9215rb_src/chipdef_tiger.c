/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef __CHIP_DEFINE_TIGER_C__
#define __CHIP_DEFINE_TIGER_C__

#include "chipdef.h"

/* chip capability */
/* chip 9001 related to yt9218 */
const yt_swchip_cap_t yt9218_capacity =
{
    .max_ucast_queue_num = 8,
    .max_mcast_queue_num = 4,
    .max_meter_entry_num = 64,
    .max_value_of_int_pri = 7,
    .max_value_of_int_drop = 2,
    .max_value_of_dscp = 63,
    .max_vlan_xlate_entry_num = 64,
    .max_vlan_egr_xlate_tbl_num = 32,
    .max_protocol_vlan_tbl_num = 4,
    .max_value_of_msti = 15,
    .max_vlan_range_profile_num = 10,
    .max_vlan_meter_entry_num = 32,
    .max_l2_fdb_num = 4096
};

/* chip 9002 related to yt9215 */
const yt_swchip_cap_t yt9215_capacity =
{
    .max_ucast_queue_num = 8,
    .max_mcast_queue_num = 4,
    .max_meter_entry_num = 64,
    .max_value_of_int_pri = 7,
    .max_value_of_int_drop = 2,
    .max_value_of_dscp = 63,
    .max_vlan_xlate_entry_num = 64,
    .max_vlan_egr_xlate_tbl_num = 32,
    .max_protocol_vlan_tbl_num = 0,
    .max_value_of_msti = 15,
    .max_vlan_range_profile_num = 10,
    .max_vlan_meter_entry_num = 32,
    .max_l2_fdb_num = 4096
};

const yt_swchip_if_info_t yt9218_intf_info =
{
    .allif_num = 10,
    .intif_start_mac_id = 0,
    .intif_end_mac_id = 7,
    .intcpu_mac_id = 10,
    .extif_num = 2,
    .extIf[0] = {8, 8, 0},
    .extIf[1] = {9, 9, 1},
};

const yt_swchip_if_info_t yt9218n_intf_info =
{
    .allif_num = 8,
    .intif_start_mac_id = 0,
    .intif_end_mac_id = 7,
    .intcpu_mac_id = 10,
    .extif_num = 0,
};

const yt_swchip_if_info_t yt9215_intf_info =
{
    .allif_num = 7,
    .intif_start_mac_id = 0,
    .intif_end_mac_id = 4,
    .intcpu_mac_id = 10,
    .extif_num = 2,
    .extIf[0] = {8, 8, 0},
    .extIf[1] = {9, 9, 1},
};

const yt_swchip_if_info_t yt9213_intf_info =
{
	.allif_num = 3,
	.intif_start_mac_id = 1,
    .intif_end_mac_id = 3,
	.intcpu_mac_id = 10,
	.extif_num = 1,
    .extIf[0] = {9, 9, 1},
};

const yt_swchip_if_info_t yt9214_intf_info =
{
	.allif_num = 4,
	.intif_start_mac_id = 1,
    .intif_end_mac_id = 3,
	.intcpu_mac_id = 10,
	.extif_num = 2,
    .extIf[0] = {8, 8, 0},
    .extIf[1] = {9, 9, 1},
};

#endif
