/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_multicast.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_MULTICAST_H
#define __FAL_TIGER_MULTICAST_H

#include "fal_cmm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern yt_ret_t fal_tiger_multicast_vlan_add(yt_unit_t unit, yt_vlan_t vid);
extern yt_ret_t fal_tiger_multicast_vlan_get(yt_unit_t unit, yt_multi_vlan_t *pvlanarray);
extern yt_ret_t fal_tiger_multicast_vlan_del(yt_unit_t unit, yt_vlan_t vid);
extern yt_ret_t fal_tiger_multicast_ipv4_add(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv4_info_t *pIpinfo, yt_port_mask_t portMask);
extern yt_ret_t fal_tiger_multicast_ipv6_add(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv6_info_t *pIpinfo, yt_port_mask_t portMask);
extern yt_ret_t fal_tiger_multicast_ipv4_del(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv4_info_t *pIpinfo);
extern yt_ret_t fal_tiger_multicast_ipv6_del(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv6_info_t *pIpinfo);
extern yt_ret_t fal_tiger_multicast_ipv4_withIpAndVid_get(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv4_info_t *pIpinfo,  yt_mcast_ipv4_data_t *pData);
extern yt_ret_t fal_tiger_multicast_ipv4_withindex_get(yt_unit_t unit, uint16_t index, yt_mcast_ipv4_data_t *pData);
extern yt_ret_t fal_tiger_multicast_ipv4_withindex_getnext(yt_unit_t unit, uint16_t index, uint16_t *pNextIdx, yt_mcast_ipv4_data_t *pData);
extern yt_ret_t fal_tiger_multicast_ipv6_withIpAndVid_get(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv6_info_t *pIpinfo,  yt_mcast_ipv6_data_t *pData);
extern yt_ret_t fal_tiger_multicast_ipv6_withindex_get(yt_unit_t unit, uint16_t index, yt_mcast_ipv6_data_t *pData);
extern yt_ret_t fal_tiger_multicast_ipv6_withindex_getnext(yt_unit_t unit, uint16_t index, uint16_t *pNextIdx, yt_mcast_ipv6_data_t *pData);
extern yt_ret_t fal_tiger_multicast_macaddr_withMacAndVid_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_mcast_mac_data_t *pData);
extern yt_ret_t fal_tiger_multicast_macaddr_withindex_get(yt_unit_t unit, uint16_t index, yt_mcast_mac_data_t *pData);
extern yt_ret_t fal_tiger_multicast_macaddr_withindex_getnext(yt_unit_t unit, uint16_t index,  uint16_t *pNextIdx, yt_mcast_mac_data_t *pData);
extern yt_ret_t fal_tiger_multicast_all_ipmc_flush(yt_unit_t unit);
extern yt_ret_t fal_tiger_multicast_vid_ipmc_flush(yt_unit_t unit, yt_vlan_t vid);
extern yt_ret_t fal_tiger_multicast_hwmc_flush_all(yt_unit_t unit, yt_mcast_hw_mode_t mode);
extern yt_ret_t fal_tiger_multicast_hwmc_flush_by_vid(yt_unit_t unit, yt_mcast_hw_mode_t mode, yt_vlan_t vid);
extern yt_ret_t fal_tiger_multicast_hwmc_flush_by_portmask(yt_unit_t unit, yt_mcast_hw_mode_t mode, yt_port_mask_t portMask);
extern yt_ret_t fal_tiger_multicast_hwmc_mac_del(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_port_mask_t portMask);
extern yt_ret_t fal_tiger_multicast_hwmc_ipv4_del(yt_unit_t unit, yt_vlan_t vid, yt_ip_addr_t gip, yt_port_mask_t portMask);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
