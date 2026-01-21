/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_l2.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_L2_H
#define __YT_L2_H

#include "yt_cmm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum yt_l2_learn_mode_e
{
    YT_L2_LEARN_MODE_AUTO,    /* all learned fdb entry is dynamic */
    YT_L2_LEARN_MODE_AUTO_AND_COPY, /* all learned fdb entry is dynamic */
    YT_L2_LEARN_MODE_SUSPEND, /* all learned fdb entry is pending regardless of suspend act */
} yt_l2_learn_mode_t;

typedef enum yt_l2_fdb_type_e
{
    YT_L2_FDB_TYPE_INVALID,
    YT_L2_FDB_TYPE_DYNAMIC,
    YT_L2_FDB_TYPE_STATIC,
    YT_L2_FDB_TYPE_PENDING,
}yt_l2_fdb_type_t;

typedef enum yt_l2_ucastMacaddr_action_e
{
    YT_L2_FDB_ACTION_TYPE_FORWARD,
    YT_L2_FDB_ACTION_TYPE_DA_DROP,
    YT_L2_FDB_ACTION_TYPE_SA_DROP,
#if defined(SWITCH_SERIES_TIGER)
    YT_L2_FDB_ACTION_TYPE_COPY_TO_CPU,
#endif
    YT_L2_FDB_ACTION_TYPE_MAX,
}yt_l2_ucastMacaddr_action_t;

typedef struct l2_ucastMacAddr_info_s
{
    yt_port_t port;
    yt_bool_t isLag;
    yt_vlan_t vid;
    yt_mac_addr_t macaddr;
    yt_l2_fdb_type_t type; /*invalid, static, dynamic, pending*/
    yt_l2_ucastMacaddr_action_t action;
    yt_bool_t forceFlag;    /* just for add operation,if entry is full and set forceFlag to true,it can use new entry to replace an existing dynamic entry forcefully */
}l2_ucastMacAddr_info_t;

typedef enum yt_l2_fdb_notify_type_e
{
    YT_L2_FDB_NOTIFY_TYPE_INVALID,
    YT_L2_FDB_NOTIFY_TYPE_DYNAMIC,
    YT_L2_FDB_NOTIFY_TYPE_STATIC,
    YT_L2_FDB_NOTIFY_TYPE_DYNAMIC_STATIC,
    YT_L2_FDB_NOTIFY_TYPE_PENDING,
    YT_L2_FDB_NOTIFY_TYPE_PENDING_DYNAMIC,
    YT_L2_FDB_NOTIFY_TYPE_PENDING_STATIC,
    YT_L2_FDB_NOTIFY_TYPE_PENDING_STATIC_DYNAMIC,
}yt_l2_fdb_notify_type_t;

typedef void (*l2_fdb_add_notify)(l2_ucastMacAddr_info_t UcastMac);  /* add notify function for l2 db */
typedef void (*l2_fdb_del_notify)(yt_vlan_t vlan, yt_mac_addr_t mac); /* del notufy function for l2 db */


/**
 * @internal      yt_l2_init
 * @endinternal
 *
 * @brief         Init l2 module
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 */
extern yt_ret_t  yt_l2_init(yt_unit_t unit);

/**
 * @internal      yt_l2_fdb_ucast_addr_add
 * @endinternal
 *
 * @brief         Add/Update l2 fdb unicast entry 
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     pUcastMac           -fdb unicast entry info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_ENTRY_FULL  -on entry is full
 * @retval        CMM_ERR_EXCEED_RANGE    -on vid exceed range
 * @retval        CMM_ERR_PORT            -on port is not valid
 * @retval        CMM_ERR_SAMEENTRY_EXIST -on same entry is exist
 */
extern yt_ret_t  yt_l2_fdb_ucast_addr_add(yt_unit_t unit, const l2_ucastMacAddr_info_t *pUcastMac);

/**
 * @internal      yt_l2_fdb_ucast_addr_del
 * @endinternal
 *
 * @brief         Del l2 fdb unicast entry
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     macAddr             -mac address
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_EXCEED_RANGE    -on vid exceed range
 * @retval        CMM_ERR_ENTRY_NOT_FOUND -on not find entry
 */
extern yt_ret_t  yt_l2_fdb_ucast_addr_del(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr);

/**
 * @internal      yt_l2_fdb_uc_withindex_get
 * @endinternal
 *
 * @brief         Get l2 fdb unicast entry by index
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     index               -entry index
 * @param[out]    pUcastMac           -fdb unicast entry info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_EXCEED_RANGE    -on index exceed range
 * @retval        CMM_ERR_ENTRY_NOT_FOUND -on not find entry
 * @retval        CMM_ERR_NULL_POINT      -on point is null
 */
extern yt_ret_t  yt_l2_fdb_uc_withindex_get(yt_unit_t unit, uint16_t index, l2_ucastMacAddr_info_t *pUcastMac);

/**
 * @internal      yt_l2_fdb_uc_withMacAndVid_get
 * @endinternal
 *
 * @brief         Get ucast info by mac addr and vlan
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     macAddr             -mac address
 * @param[out]    pUcastMac           -fdb unicast entry info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT      -on point is null
 * @retval        CMM_ERR_ENTRY_NOT_FOUND -on not find entry
 */
extern yt_ret_t  yt_l2_fdb_uc_withMacAndVid_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, l2_ucastMacAddr_info_t *pUcastMac);

/**
 * @internal      yt_l2_fdb_uc_withindex_getnext
 * @endinternal
 *
 * @brief         Get next vaild ucast info by current index
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     index               -current index
 * @param[out]    pNextIndex          -the index of next vaild entry 
 * @param[out]    pUcastMac           -fdb unicast entry info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT      -on point is null
 * @retval        CMM_ERR_EXCEED_RANGE    -on index exceed range
 * @retval        CMM_ERR_ENTRY_NOT_FOUND -on not find entry
 */
extern yt_ret_t  yt_l2_fdb_uc_withindex_getnext(yt_unit_t unit, uint16_t index, uint16_t *pNextIndex, l2_ucastMacAddr_info_t *pUcastMac);

/**
 * @internal      yt_l2_fdb_type_get
 * @endinternal
 *
 * @brief         Get mac address fdb type
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     macAddr             -mac address
 * @param[out]    pType               -FDB type,dynamic or static
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT      -on point is null
 * @retval        CMM_ERR_EXCEED_RANGE    -on index exceed range
 */
extern yt_ret_t  yt_l2_fdb_type_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_l2_fdb_type_t *pType);

/**
 * @internal      yt_l2_fdb_all_ucast_flush
 * @endinternal
 *
 * @brief         flush all dynamic unicast mac address
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 */
extern yt_ret_t  yt_l2_fdb_all_ucast_flush(yt_unit_t unit);

/**
 * @internal      yt_l2_fdb_port_ucast_flush
 * @endinternal
 *
 * @brief         flush all dynamic unicast mac address on port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     isLag               -port is lag or not
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 */
extern yt_ret_t  yt_l2_fdb_port_ucast_flush(yt_unit_t unit, yt_port_t port, yt_bool_t isLag);

/**
 * @internal      yt_l2_fdb_vlan_ucast_flush
 * @endinternal
 *
 * @brief         flush all dynamic unicast mac address on VLAN
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_EXCEED_RANGE    -input value out of range
 */
extern yt_ret_t  yt_l2_fdb_vlan_ucast_flush(yt_unit_t unit, yt_vlan_t vid);

/**
 * @internal      yt_l2_fdb_vlan_port_ucast_flush
 * @endinternal
 *
 * @brief         flush all dynamic unicast mac address on port and VLAN
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     port                -port num
 * @param[in]     isLag                -port is lag
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_EXCEED_RANGE    -input value out of range
 */
extern yt_ret_t  yt_l2_fdb_vlan_port_ucast_flush(yt_unit_t unit, yt_vlan_t vid, yt_port_t port, yt_bool_t isLag);

/**
 * @internal      yt_l2_fdb_uc_cnt_get
 * @endinternal
 *
 * @brief         get system dynamic unicast mac address count
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pCnt                -count
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_fdb_uc_cnt_get(yt_unit_t unit,  uint32 *pCnt);

/**
 * @internal      yt_l2_fdb_port_uc_cnt_get
 * @endinternal
 *
 * @brief         get system dynamic unicast mac address count by port 
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pCnt                -count
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_fdb_port_uc_cnt_get(yt_unit_t unit, yt_port_t port, uint32 *pCnt);

/**
 * @internal      yt_l2_fdb_lag_uc_cnt_get
 * @endinternal
 *
 * @brief         get system dynamic unicast mac address count by lag group id
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -groupId
 * @param[out]    pCnt                -count
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_fdb_lag_uc_cnt_get(yt_unit_t unit, uint8_t groupId, uint32_t *pCnt);

/**
 * @internal      yt_l2_fdb_aging_port_en_set
 * @endinternal
 *
 * @brief         enable fdb aging on port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 */
extern yt_ret_t  yt_l2_fdb_aging_port_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_l2_fdb_aging_port_en_get
 * @endinternal
 *
 * @brief         get enable state of fdb aging on port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_fdb_aging_port_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_l2_fdb_aging_time_set
 * @endinternal
 *
 * @brief         set fdb aging time,time range is MIN_TIME~MAX_TIME seconds,MAX_TIME is 5*65535 for Tiger, 6*65535 for Shark and Whale.
 *                      MIN_TIME is 6 for Tiger and Shark, 12 for Whale,the configured time should be a multiple of 5(Tiger) or 6(Shark, Whale),
 *                      otherwise they will be automatically converted.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     sec                 -second
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 */
extern yt_ret_t  yt_l2_fdb_aging_time_set(yt_unit_t unit,  uint32_t sec);

/**
 * @internal      yt_l2_fdb_aging_time_get
 * @endinternal
 *
 * @brief         get fdb aging time
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pSec                -second
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_fdb_aging_time_get(yt_unit_t unit,  uint32_t *pSec);

/**
 * @internal      yt_l2_fdb_linkdownFlush_en_set
 * @endinternal
 *
 * @brief         enable or disable fdb hw flush when port linkdown
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 */
extern yt_ret_t  yt_l2_fdb_linkdownFlush_en_set(yt_unit_t unit, yt_enable_t enable);

/**
 * @internal      yt_l2_fdb_linkdownFlush_en_get
 * @endinternal
 *
 * @brief         get status of fdb hw flush when port linkdown
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_fdb_linkdownFlush_en_get(yt_unit_t unit, yt_enable_t *pEnable);

/**
 * @internal      yt_l2_fdb_add_notify_register
 * @endinternal
 *
 * @brief         register l2 fdb add notify function for l2 db
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     func                -add func
 * @param[in]     type                -notify type of l2 fdb
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE    -input value out of range
 */
extern yt_ret_t  yt_l2_fdb_add_notify_register(yt_unit_t unit, l2_fdb_add_notify func, yt_l2_fdb_notify_type_t type);

/**
 * @internal      yt_l2_fdb_del_notify_register
 * @endinternal
 *
 * @brief         register l2 fdb del notify function for l2 db
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     func                -add func
 * @param[in]     type                -notify type of l2 fdb
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE    -input value out of range
 */
extern yt_ret_t  yt_l2_fdb_del_notify_register(yt_unit_t unit, l2_fdb_del_notify func, yt_l2_fdb_notify_type_t type);

/**
 * @internal      yt_l2_fdb_uc_getFirst_fromDB
 * @endinternal
 *
 * @brief         get first l2 fdb entry from l2 db
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pUcastMac           -fdb unicast entry info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_fdb_uc_getFirst_fromDB(yt_unit_t unit, l2_ucastMacAddr_info_t *pUcastMac);

/**
 * @internal      yt_l2_fdb_uc_getNext_withMacAndVid_fromDB
 * @endinternal
 *
 * @brief         get next l2 fdb entry from l2 db by vid and macAddr
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     macAddr             -mac address
 * @param[out]    pUcastMac           -fdb unicast entry info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE    -input value out of range
 */
extern yt_ret_t  yt_l2_fdb_uc_getNext_withMacAndVid_fromDB(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, l2_ucastMacAddr_info_t *pUcastMac);

/**
 * @internal      yt_l2_port_learnlimit_en_set
 * @endinternal
 *
 * @brief         enable learn limitation on port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 */
extern yt_ret_t  yt_l2_port_learnlimit_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_l2_port_learnlimit_en_get
 * @endinternal
 *
 * @brief         get enable state of learn limitation on port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_port_learnlimit_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_l2_port_learnlimit_cnt_set
 * @endinternal
 *
 * @brief         set limitation count on port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     maxCnt              -max count
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_EXCEED_RANGE    -input value out of range
 */
extern yt_ret_t  yt_l2_port_learnlimit_cnt_set(yt_unit_t unit, yt_port_t port, uint32_t maxCnt);

/**
 * @internal      yt_l2_port_learnlimit_cnt_get
 * @endinternal
 *
 * @brief         get limitation count on port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pMaxCnt             -max count
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_port_learnlimit_cnt_get(yt_unit_t unit, yt_port_t port, uint32_t *pMaxCnt);

/**
 * @internal      yt_l2_port_learnlimit_act_set
 * @endinternal
 *
 * @brief         set act type when learnt count exceed limitation
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     actType             -act type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_EXCEED_RANGE    -input value out of range
 */
extern yt_ret_t  yt_l2_port_learnlimit_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t actType);

/**
 * @internal      yt_l2_port_learnlimit_act_get
 * @endinternal
 *
 * @brief         Get act type when learnt count exceed limitation
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pActType            -act type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_port_learnlimit_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pActType);

/**
 * @internal      yt_l2_system_learnlimit_en_set
 * @endinternal
 *
 * @brief         enable system global learn limitation
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 */
extern yt_ret_t  yt_l2_system_learnlimit_en_set(yt_unit_t unit, yt_enable_t enable);

/**
 * @internal      yt_l2_system_learnlimit_en_get
 * @endinternal
 *
 * @brief         get enable state of system global learn port limitation
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_system_learnlimit_en_get(yt_unit_t unit, yt_enable_t *pEnable);

/**
 * @internal      yt_l2_system_learnlimit_cnt_set
 * @endinternal
 *
 * @brief         set system global limitation count
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     maxCnt              -max count
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_EXCEED_RANGE    -input value out of range
 */
extern yt_ret_t  yt_l2_system_learnlimit_cnt_set(yt_unit_t unit, uint32_t maxCnt);

/**
 * @internal      yt_l2_system_learnlimit_cnt_get
 * @endinternal
 *
 * @brief         get system global limitation count
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pMaxCnt             -max count
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_system_learnlimit_cnt_get(yt_unit_t unit, uint32_t *pMaxCnt);

/**
 * @internal      yt_l2_system_learnlimit_act_set
 * @endinternal
 *
 * @brief         Set act type when learnt count exceed system limitation
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     actType             -act type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_EXCEED_RANGE    -input value out of range
 */
extern yt_ret_t  yt_l2_system_learnlimit_act_set(yt_unit_t unit, yt_act_type_t actType);

/**
 * @internal      yt_l2_system_learnlimit_act_get
 * @endinternal
 *
 * @brief         get act type when learnt count exceed system limitation
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pActType            -act type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_system_learnlimit_act_get(yt_unit_t unit, yt_act_type_t *pActType);

/**
 * @internal      yt_l2_lag_learnlimit_en_set
 * @endinternal
 *
 * @brief         enable learn limitation on lag port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -lag group id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 */
extern yt_ret_t  yt_l2_lag_learnlimit_en_set(yt_unit_t unit, uint8_t groupId,  yt_enable_t enable);

/**
 * @internal      yt_l2_lag_learnlimit_en_get
 * @endinternal
 *
 * @brief         get enable state of learn limitation on lag port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -lag group id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_lag_learnlimit_en_get(yt_unit_t unit, uint8_t groupId, yt_enable_t *pEnable);

/**
 * @internal      yt_l2_lag_learnlimit_cnt_set
 * @endinternal
 *
 * @brief         set limitaion count on lag port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -lag group id
 * @param[in]     maxCnt              -max count
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_EXCEED_RANGE    -input value out of range
 */
extern yt_ret_t  yt_l2_lag_learnlimit_cnt_set(yt_unit_t unit, uint8_t groupId,  uint32_t maxCnt);

/**
 * @internal      yt_l2_lag_learnlimit_cnt_get
 * @endinternal
 *
 * @brief         get limitaion count on lag port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -lag group id
 * @param[out]    pMaxCnt             -max count
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_lag_learnlimit_cnt_get(yt_unit_t unit, uint8_t groupId,  uint32_t *pMaxCnt);

/**
 * @internal      yt_l2_lag_learnlimit_act_set
 * @endinternal
 *
 * @brief         set act type when learnt count exceed limitation on lag
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -lag group id
 * @param[in]     actType             -act type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_EXCEED_RANGE    -input value out of range
 */
extern yt_ret_t  yt_l2_lag_learnlimit_act_set(yt_unit_t unit, uint8_t groupId, yt_act_type_t actType);

/**
 * @internal      yt_l2_lag_learnlimit_act_get
 * @endinternal
 *
 * @brief         get act type when learnt count exceed limitation on lag
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -lag group id
 * @param[out]    pActType            -act type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_lag_learnlimit_act_get(yt_unit_t unit, uint8_t groupId, yt_act_type_t *pActType);

/**
 * @internal      yt_l2_port_learn_en_set
 * @endinternal
 *
 * @brief         set l2 port learn status 
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 */
extern yt_ret_t  yt_l2_port_learn_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_l2_port_learn_en_get
 * @endinternal
 *
 * @brief         get l2 port learn status 
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_port_learn_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_l2_port_learn_mode_set
 * @endinternal
 *
 * @brief         set port learn mode
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     mode                -learn mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_EXCEED_RANGE    -input value out of range
 */
extern yt_ret_t  yt_l2_port_learn_mode_set(yt_unit_t unit, yt_port_t port, yt_l2_learn_mode_t mode);

/**
 * @internal      yt_l2_port_learn_mode_get
 * @endinternal
 *
 * @brief         get port learn mode
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pMode               -learn mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_port_learn_mode_get(yt_unit_t unit, yt_port_t port, yt_l2_learn_mode_t *pMode);

/**
 * @internal      yt_l2_port_suspend_act_set
 * @endinternal
 *
 * @brief         set port action for port learn mode is suspend
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     actType             -act type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_EXCEED_RANGE    -input value out of range
 */
extern yt_ret_t  yt_l2_port_suspend_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t actType);

/**
 * @internal      yt_l2_port_suspend_act_get
 * @endinternal
 *
 * @brief         get port action for port learn mode is suspend
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pActType            -act type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_port_suspend_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pActType);

/**
 * @internal      yt_l2_new_sa_act_set
 * @endinternal
 *
 * @brief         set port action for pkt sa is new,for dop action will not learn fdb entry and for other action will learn dynamic fdb entry
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     actType             -act type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_EXCEED_RANGE    -input value out of range
 */
extern yt_ret_t  yt_l2_new_sa_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t actType);

/**
 * @internal      yt_l2_new_sa_act_get
 * @endinternal
 *
 * @brief         get port action for pkt sa is new
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pActType            -act type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_l2_new_sa_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pActType);

/**
 * @internal      yt_l2_unknown_ucast_act_set
 * @endinternal
 *
 * @brief         set unknown ucast pakets act for per port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     act_ctrl            -l2 packet action,FWD,DROP,COPY,TRAP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -invalid input
 * @retval        CMM_ERR_PORT        -invalid port
 * @retval        CMM_ERR_NOT_INIT    -module not init
 */
extern yt_ret_t yt_l2_unknown_ucast_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t action);

/**
 * @internal      yt_l2_unknown_ucast_act_get
 * @endinternal
 *
 * @brief         get unknown ucast pakets act for per port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pAct_ctrl           -l2 packet action,FWD,DROP,COPY,TRAP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_PORT        -invalid port
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 * @retval        CMM_ERR_NOT_INIT    -module not init
 */
extern yt_ret_t yt_l2_unknown_ucast_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pAction);

/**
 * @internal      yt_l2_unknown_mcast_act_set
 * @endinternal
 *
 * @brief         set unknown mcast pakets act for per port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     act_ctrl            -l2 packet action,FWD,DROP,COPY,TRAP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -invalid input
 * @retval        CMM_ERR_PORT        -invalid port
 * @retval        CMM_ERR_NOT_INIT    -module not init
 */
extern yt_ret_t yt_l2_unknown_mcast_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t action);

/**
 * @internal      yt_l2_unknown_mcast_act_get
 * @endinternal
 *
 * @brief         get unknown mcast pakets act for per port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pAct_ctrl           -l2 packet action,FWD,DROP,COPY,TRAP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_PORT        -invalid port
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 * @retval        CMM_ERR_NOT_INIT    -module not init
 */
extern yt_ret_t yt_l2_unknown_mcast_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pAction);

/**
 * @internal      yt_l2_filter_mcast_set
 * @endinternal
 *
 * @brief         enable multicast filter on ports
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port_mask           -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_PORTLIST    -input port mask error
 */
extern yt_ret_t  yt_l2_filter_mcast_set(yt_unit_t unit, yt_port_mask_t port_mask);


/**
 * @internal      yt_l2_filter_mcast_get
 * @endinternal
 *
 * @brief         get the ports that enable multicast filter
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pport_mask          -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input parameter error
 */
extern yt_ret_t  yt_l2_filter_mcast_get(yt_unit_t unit, yt_port_mask_t *pport_mask);

/**
 * @internal      yt_l2_filter_bcast_set
 * @endinternal
 *
 * @brief         enable boardcast filter on ports
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port_mask           -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_PORTLIST    -input port mask error
 */
extern yt_ret_t  yt_l2_filter_bcast_set(yt_unit_t unit, yt_port_mask_t port_mask);


/**
 * @internal      yt_l2_filter_bcast_get
 * @endinternal
 *
 * @brief         get the ports that enable boardcast filter
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pport_mask          -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input parameter error
 */
extern yt_ret_t  yt_l2_filter_bcast_get(yt_unit_t unit, yt_port_mask_t *pport_mask);


/**
 * @internal      yt_l2_filter_unknown_ucast_set
 * @endinternal
 *
 * @brief         enable unknown unicast filter on ports
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port_mask           -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_l2_filter_unknown_ucast_set(yt_unit_t unit, yt_port_mask_t port_mask);


/**
 * @internal      yt_l2_filter_unknown_ucast_get
 * @endinternal
 *
 * @brief         get the ports that enable unknown unicast filter
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pport_mask          -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_l2_filter_unknown_ucast_get(yt_unit_t unit, yt_port_mask_t *pport_mask);

/**
 * @internal      yt_l2_filter_unknown_mcast_set
 * @endinternal
 *
 * @brief         enable unknown multicast filter on ports
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port_mask           -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_l2_filter_unknown_mcast_set(yt_unit_t unit, yt_port_mask_t port_mask);


/**
 * @internal      yt_l2_filter_unknown_mcast_get
 * @endinternal
 *
 * @brief         get the ports that enable unknown multicast filter
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pport_mask          -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_l2_filter_unknown_mcast_get(yt_unit_t unit, yt_port_mask_t *pport_mask);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
