/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_multicast.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_MULTICAST_H
#define __YT_MULTICAST_H

#include "yt_cmm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(SWITCH_SERIES_TIGER)
#define YT_MAX_MULTICAST_VLAN_NUM       (16U)
#endif
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
#define YT_MAX_MULTICAST_VLAN_NUM       (32U)
#endif

typedef enum yt_multi_fdb_type_e
{
    YT_MULTI_TYPE_STATIC,
    YT_MULTI_TYPE_DYNAMIC,
}yt_multi_fdb_type_t;

typedef struct yt_multi_vlan_s
{
    uint16_t    vlanid[YT_MAX_MULTICAST_VLAN_NUM];
} yt_multi_vlan_t;

typedef enum yt_mcast_mode_e
{
    YT_MCAST_MODE_GIP,
    YT_MCAST_MODE_GIP_VID,
    YT_MCAST_MODE_SIP_GIP,
    YT_MCAST_MODE_SIP_GIP_VID,
} yt_mcast_mode_t;

typedef enum yt_mcast_hw_mode_e
{
    YT_MCAST_HW_MODE_MAC,
    YT_MCAST_HW_MODE_IP,
    YT_MCAST_HW_MODE_MAC_AND_IP,
} yt_mcast_hw_mode_t;

typedef struct yt_multicast_ipv4_info_s
{
    yt_vlan_t    vid;
    yt_ip_addr_t sip;
    yt_ip_addr_t gip;
} yt_mcast_ipv4_info_t;

typedef struct yt_mcast_ipv6_info_s {
    yt_vlan_t      vid;
    yt_ipv6_addr_t sip;
    yt_ipv6_addr_t gip;
} yt_mcast_ipv6_info_t;

typedef struct yt_mcast_ipv4_data_s
{
    uint16_t        vid;
    yt_ip_addr_t    sip;
    yt_ip_addr_t    gip;
    yt_mcast_mode_t mode;
    yt_port_mask_t  portMask;
    yt_multi_fdb_type_t type; /* static, dynamic */
}yt_mcast_ipv4_data_t;

typedef struct yt_mcast_ipv6_data_s
{
    uint16_t        vid;
    yt_ipv6_addr_t  sip;
    yt_ipv6_addr_t  gip;
    yt_mcast_mode_t mode;
    yt_port_mask_t  portMask; 
}yt_mcast_ipv6_data_t;

typedef struct yt_mcast_mac_data_s 
{
    yt_multi_fdb_type_t type; /* static, dynamic */
    yt_port_mask_t portMask; 
    yt_mac_addr_t  macAddr;
    uint16_t       vid;
}yt_mcast_mac_data_t;


/**
 * @internal      yt_multicast_ipv4_add
 * @endinternal
 *
 * @brief         add ipv4 multicast entry
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     mode                -entry mode
 * @param[in]     pIpinfo             -ipv4 info
 * @param[in]     portMask            -port mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_ipv4_add(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv4_info_t *pIpinfo, yt_port_mask_t portMask);

/**
 * @internal      yt_multicast_ipv6_add
 * @endinternal
 *
 * @brief         add ipv6 multicast entry
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     mode                -entry mode
 * @param[in]     pIpinfo             -ipv6 info
 * @param[in]     portMask            -port mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_ipv6_add(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv6_info_t *pIpinfo, yt_port_mask_t portMask);

/**
 * @internal      yt_multicast_macaddr_add
 * @endinternal
 *
 * @brief         add macaddr multicast entry
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     macAddr             -mac address
 * @param[in]     portMask            -port mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_macaddr_add(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_port_mask_t portMask);

/**
 * @internal      yt_multicast_ipv4_del
 * @endinternal
 *
 * @brief         del ipv4 multicast entry
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     mode                -entry mode
 * @param[in]     pIpinfo             -ipv4 info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_ipv4_del(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv4_info_t *pIpinfo);

/**
 * @internal      yt_multicast_ipv6_del
 * @endinternal
 *
 * @brief         del ipv6 multicast entry
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     mode                -entry mode
 * @param[in]     pIpinfo             -ipv4 info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_ipv6_del(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv6_info_t *pIpinfo);

/**
 * @internal      yt_multicast_macaddr_del
 * @endinternal
 *
 * @brief         del macaddr multicast entry
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     macAddr             -mac address
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_macaddr_del(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr);

/**
 * @internal      yt_multicast_ipv4_withIpAndVid_get
 * @endinternal
 *
 * @brief         get ipv4 multicast entry by ip and vid
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     mode                -entry mode
 * @param[in]     pIpinfo             -ipv4 info
 * @param[out]    pData               -data info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_ipv4_withIpAndVid_get(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv4_info_t *pIpinfo,  yt_mcast_ipv4_data_t *pData);

/**
 * @internal      yt_multicast_ipv4_withindex_get
 * @endinternal
 *
 * @brief         get ipv4 multicast entry by index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     index               -index
 * @param[out]    pData               -data info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_ipv4_withindex_get(yt_unit_t unit, uint16_t index, yt_mcast_ipv4_data_t *pData);

/**
 * @internal      yt_multicast_ipv4_withindex_getnext
 * @endinternal
 *
 * @brief         get ipv4 next multicast entry by index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     index               -index
 * @param[out]    pNextIdx            -next index
 * @param[out]    pData               -data info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_ipv4_withindex_getnext(yt_unit_t unit, uint16_t index, uint16_t *pNextIdx, yt_mcast_ipv4_data_t *pData);

/**
 * @internal      yt_multicast_ipv6_withIpAndVid_get
 * @endinternal
 *
 * @brief         get ipv6 multicast entry by ip and vid
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     mode                -entry mode
 * @param[in]     pIpinfo             -ipv6 info
 * @param[out]    pData               -data info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_ipv6_withIpAndVid_get(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv6_info_t *pIpinfo,  yt_mcast_ipv6_data_t *pData);

/**
 * @internal      yt_multicast_ipv6_withindex_get
 * @endinternal
 *
 * @brief         get ipv6 multicast entry by index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     index               -index
 * @param[out]    pData               -data info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_ipv6_withindex_get(yt_unit_t unit, uint16_t index, yt_mcast_ipv6_data_t *pData);

/**
 * @internal      yt_multicast_ipv6_withindex_getnext
 * @endinternal
 *
 * @brief         get ipv6 next multicast entry by index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     index               -index
 * @param[out]    pNextIdx            -next index
 * @param[out]    pData               -data info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_ipv6_withindex_getnext(yt_unit_t unit, uint16_t index, uint16_t *pNextIdx, yt_mcast_ipv6_data_t *pData);

/**
 * @internal      yt_multicast_ipv6_withIpAndVid_get
 * @endinternal
 *
 * @brief         get macaddr multicast entry by mac and vid
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     macAddr             -mac address
 * @param[out]    pData               -data info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_macaddr_withMacAndVid_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_mcast_mac_data_t *pData);

/**
 * @internal      yt_multicast_macaddr_withindex_get
 * @endinternal
 *
 * @brief         get macaddr multicast entry by index
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     index               -index
 * @param[out]    pData               -data info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_macaddr_withindex_get(yt_unit_t unit, uint16_t index, yt_mcast_mac_data_t *pData);

/**
 * @internal      yt_multicast_macaddr_withindex_getnext
 * @endinternal
 *
 * @brief         get macaddr next multicast entry by index
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     index               -index
 * @param[out]    pNextIdx            -next index
 * @param[out]    pData               -data info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_macaddr_withindex_getnext(yt_unit_t unit, uint16_t index,  uint16_t *pNextIdx, yt_mcast_mac_data_t *pData);

/**
 * @internal      yt_multicast_all_ipmc_flush
 * @endinternal
 *
 * @brief         flush all ip multicast entry
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_all_ipmc_flush(yt_unit_t unit);

/**
 * @internal      yt_multicast_vid_ipmc_flush
 * @endinternal
 *
 * @brief         flush all ip multicast entry by vid
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_vid_ipmc_flush(yt_unit_t unit, yt_vlan_t vid);

/**
 * @internal      yt_multicast_all_macaddr_flush
 * @endinternal
 *
 * @brief         flush all macaddr multicast entry
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_all_macaddr_flush(yt_unit_t unit);

/**
 * @internal      yt_multicast_vid_macaddr_flush
 * @endinternal
 *
 * @brief         flush all macaddr multicast entry by vid
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_vid_macaddr_flush(yt_unit_t unit, yt_vlan_t vid);

/**
 * @internal      yt_multicast_port_macaddr_flush
 * @endinternal
 *
 * @brief         flush all multicast macaddr entry by port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_multicast_port_macaddr_flush(yt_unit_t unit, yt_port_t port);

/**
 * @internal      yt_multicast_macaddr_cnt_get
 * @endinternal
 *
 * @brief         get dynamic multicast macaddr entry
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     pcnt                -mac cnt
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_multicast_macaddr_cnt_get(yt_unit_t unit,  uint32 *pcnt);

/**
 * @internal      yt_multicast_vlan_add
 * @endinternal
 *
 * @brief         add multicast vlan
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_SAMEENTRY_EXIST    -already exist same entry
 * @retval        CMM_ERR_ENTRY_FULL -multicast vlan table full
 */
extern yt_ret_t yt_multicast_vlan_add(yt_unit_t unit, yt_vlan_t vid);


/**
 * @internal      yt_multicast_vlan_get
 * @endinternal
 *
 * @brief         get multicast vlan table info
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pvlanarray          -multicast vlan info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_ENTRY_NOT_FOUND   -no entry
 */
extern yt_ret_t yt_multicast_vlan_get(yt_unit_t unit, yt_multi_vlan_t *pvlanarray);


/**
 * @internal      yt_multicast_vlan_del
 * @endinternal
 *
 * @brief         delete 
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_ENTRY_NOT_FOUND   -no entry
 */
extern yt_ret_t yt_multicast_vlan_del(yt_unit_t unit, yt_vlan_t vid);


/**
 * @internal      yt_multicast_hwmc_flush_all
 * @endinternal
 *
 * @brief         flush all hwmc
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     mode                -YT_MCAST_HW_MODE_MAC, YT_MCAST_HW_MODE_IP, YT_MCAST_HW_MODE_MAC_AND_IP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_ENTRY_NOT_FOUND   -no entry
 */
extern yt_ret_t yt_multicast_hwmc_flush_all(yt_unit_t unit, yt_mcast_hw_mode_t mode);


/**
 * @internal      yt_multicast_hwmc_flush_by_vid
 * @endinternal
 *
 * @brief         flush all hwmc by vid
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     mode                -YT_MCAST_HW_MODE_MAC, YT_MCAST_HW_MODE_IP, YT_MCAST_HW_MODE_MAC_AND_IP
 * @param[in]     vid                 -vid
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_ENTRY_NOT_FOUND   -no entry
 */
extern yt_ret_t yt_multicast_hwmc_flush_by_vid(yt_unit_t unit, yt_mcast_hw_mode_t mode, yt_vlan_t vid);


/**
 * @internal      yt_multicast_hwmc_flush_by_portmask
 * @endinternal
 *
 * @brief         flush all hwmc by port
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     mode                -YT_MCAST_HW_MODE_MAC, YT_MCAST_HW_MODE_IP, YT_MCAST_HW_MODE_MAC_AND_IP
 * @param[in]     portMask            -port mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_ENTRY_NOT_FOUND   -no entry
 */
extern yt_ret_t yt_multicast_hwmc_flush_by_portmask(yt_unit_t unit, yt_mcast_hw_mode_t mode, yt_port_mask_t portMask);


/**
 * @internal      yt_multicast_hwmc_mac_del
 * @endinternal
 *
 * @brief         delete hwmc by MAC+VID
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     macAddr             -mac addr
 * @param[in]     portMask            -port mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_ENTRY_NOT_FOUND   -no entry
 */
extern yt_ret_t yt_multicast_hwmc_mac_del(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_port_mask_t portMask);


/**
 * @internal      yt_multicast_hwmc_ipv4_del
 * @endinternal
 *
 * @brief         delete hwmc by GIP+VID
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     gip                 -ipv4 addr
 * @param[in]     portMask            -port mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_ENTRY_NOT_FOUND   -no entry
 */
extern yt_ret_t yt_multicast_hwmc_ipv4_del(yt_unit_t unit, yt_vlan_t vid, yt_ip_addr_t gip, yt_port_mask_t portMask);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
