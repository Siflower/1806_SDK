/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#ifndef __CHIP_DEFINE_H__
#define __CHIP_DEFINE_H__

#include "yt_types.h"
#if defined(SWITCH_SERIES_TIGER)
#include "chipdef_tiger.h"
#endif
#if defined(SWITCH_SERIES_SHARK)
#include "chipdef_shark.h"
#endif
#if defined(SWITCH_SERIES_WHALE)
#include "chipdef_whale.h"
#endif

#define INVALID_ID  (0xFF)
#define MAX_EXTIF_NUM  6

typedef enum yt_switch_chip_rev_e
{
    YT_SW_REV_A	 = 1,
    YT_SW_REV_B,
    YT_SW_REV_C,
    YT_SW_REV_D
}yt_switch_chip_rev_t;

/* capability of switch unit */
typedef struct yt_sw_chip_cap_s
{
    uint8_t max_ucast_queue_num;
    uint8_t max_mcast_queue_num;
    uint8_t max_meter_entry_num;
    uint8_t max_value_of_int_pri;
    uint8_t max_value_of_int_drop;
    uint8_t max_value_of_dscp;
    uint8_t max_vlan_xlate_entry_num;
    uint8_t max_vlan_egr_xlate_tbl_num;
    uint8_t max_protocol_vlan_tbl_num;
    uint8_t max_value_of_msti;
    uint8_t max_vlan_range_profile_num;
    uint8_t max_vlan_meter_entry_num;
    uint16_t max_l2_fdb_num;
}yt_swchip_cap_t;

/* chip interface define */
typedef struct yt_sw_chip_extif_s
{
    /*mac id range map to ext interface id*/
    uint8_t mac_start_id;
    uint8_t mac_end_id;
    uint8_t extif_id;
}yt_swchip_extif_t;

typedef struct yt_sw_chip_if_info_s
{
    uint8_t allif_num;
    uint8_t intif_start_mac_id;/*internal start macid*/
    uint8_t intif_end_mac_id;/*internal end macid*/
    uint8_t intcpu_mac_id;
    uint8_t extif_num;
    yt_swchip_extif_t extIf[MAX_EXTIF_NUM];
}yt_swchip_if_info_t;

/* data define */
#if defined(SWITCH_SERIES_TIGER)
extern const yt_swchip_cap_t yt9218_capacity;
extern const yt_swchip_cap_t yt9215_capacity;
extern const yt_swchip_if_info_t yt9218_intf_info;
extern const yt_swchip_if_info_t yt9218n_intf_info;
extern const yt_swchip_if_info_t yt9215_intf_info;
extern const yt_swchip_if_info_t yt9213_intf_info;
extern const yt_swchip_if_info_t yt9214_intf_info;
#endif

#if defined(SWITCH_SERIES_SHARK)
extern const yt_swchip_cap_t yt9232_capacity;
extern const yt_swchip_if_info_t yt9232_intf_info;
extern const yt_swchip_if_info_t yt9232_26_intf_info;
extern const yt_swchip_if_info_t yt9230_intf_info;
extern const yt_swchip_if_info_t yt9231_intf_info;
extern const yt_swchip_if_info_t yt9231_6_intf_info;
#endif

#if defined(SWITCH_SERIES_WHALE)
extern const yt_swchip_cap_t yt922x_capacity;
extern const yt_swchip_if_info_t yt9228_intf_info;
extern const yt_swchip_if_info_t yt9224_intf_info;
#endif

extern const yt_swchip_cap_t * const gpChipCapList[];
extern const yt_swchip_if_info_t * const gpChipIntfInfoList[];

/* function define */
extern uint8_t chipdef_get_extif_by_macid(yt_macid_t mac_id, const yt_swchip_if_info_t *pChipIfInfo);

#endif /* __CHIP_DEFINE_H__ */
