/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_l2.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_l2.h"
#include "fal_dispatch.h"


yt_ret_t yt_l2_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->l2_init(unit);
}

yt_ret_t  yt_l2_fdb_ucast_addr_add(yt_unit_t unit, const l2_ucastMacAddr_info_t *pUcastMac)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    if(!pUcastMac->isLag)
    {
        CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, pUcastMac->port))), CMM_ERR_PORT);
    }
    else
    {
        CMM_PARAM_CHK((YT_LAG_NUM_MAX <= pUcastMac->port), CMM_ERR_PORT);
    }
    CMM_PARAM_CHK((IS_MCAST_ADDR(pUcastMac->macaddr.addr)), CMM_ERR_INPUT);
    CMM_PARAM_CHK(((pUcastMac->type != YT_L2_FDB_TYPE_STATIC) && (pUcastMac->type != YT_L2_FDB_TYPE_DYNAMIC)), CMM_ERR_INPUT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < pUcastMac->vid) || (YT_VLAN_ID_MIN == pUcastMac->vid)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((YT_L2_FDB_ACTION_TYPE_FORWARD > pUcastMac->action) || (YT_L2_FDB_ACTION_TYPE_MAX <= pUcastMac->action)), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->l2_fdb_ucast_addr_add(unit, pUcastMac);
}

yt_ret_t  yt_l2_fdb_ucast_addr_del(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < vid) || (YT_VLAN_ID_MIN == vid)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((IS_MCAST_ADDR(macAddr.addr)), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->l2_fdb_ucast_addr_del(unit, vid, macAddr);
}

yt_ret_t yt_l2_fdb_uc_withindex_get(yt_unit_t unit, uint16_t index, l2_ucastMacAddr_info_t *pUcastMac)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_L2_FDB_NUM_MAX(unit) <= index), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pUcastMac), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_fdb_uc_withindex_get(unit, index, pUcastMac);
}

yt_ret_t yt_l2_fdb_uc_withMacAndVid_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, l2_ucastMacAddr_info_t *pUcastMac)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < vid) || (YT_VLAN_ID_MIN == vid)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((IS_MCAST_ADDR(macAddr.addr)), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pUcastMac), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_fdb_uc_withMacAndVid_get(unit, vid, macAddr, pUcastMac);
}

yt_ret_t  yt_l2_fdb_uc_withindex_getnext(yt_unit_t unit, uint16_t index, uint16_t *pNextIndex, l2_ucastMacAddr_info_t *pUcastMac)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_L2_FDB_NUM_MAX(unit) <= index), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pNextIndex), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pUcastMac), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_fdb_uc_withindex_getnext(unit, index, pNextIndex, pUcastMac);
}

yt_ret_t  yt_l2_fdb_type_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_l2_fdb_type_t *pType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < vid) || (YT_VLAN_ID_MIN == vid)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((IS_MCAST_ADDR(macAddr.addr)), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pType), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_fdb_type_get(unit, vid, macAddr, pType);
}

yt_ret_t yt_l2_fdb_all_ucast_flush(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->l2_fdb_all_ucast_flush(unit);
}

yt_ret_t yt_l2_fdb_port_ucast_flush(yt_unit_t unit, yt_port_t port, yt_bool_t isLag)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    if(!isLag)
    {
        CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    }
    else
    {
        CMM_PARAM_CHK((YT_LAG_NUM_MAX <= port), CMM_ERR_PORT);
    }

    return YT_DISPATCH(unit)->l2_fdb_port_ucast_flush(unit, port, isLag);
}

yt_ret_t yt_l2_fdb_vlan_ucast_flush(yt_unit_t unit, yt_vlan_t vid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < vid) || (YT_VLAN_ID_MIN == vid)), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->l2_fdb_vlan_ucast_flush(unit, vid);
}

yt_ret_t yt_l2_fdb_vlan_port_ucast_flush(yt_unit_t unit, yt_vlan_t vid, yt_port_t port, yt_bool_t isLag)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < vid) || (YT_VLAN_ID_MIN == vid)), CMM_ERR_EXCEED_RANGE);
    if(!isLag)
    {
        CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    }
    else
    {
        CMM_PARAM_CHK((YT_LAG_NUM_MAX <= port), CMM_ERR_PORT);
    }

    return YT_DISPATCH(unit)->l2_fdb_vlan_port_ucast_flush(unit, vid, port, isLag);
}

yt_ret_t  yt_l2_fdb_uc_cnt_get(yt_unit_t unit,  uint32 *pCnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pCnt), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_fdb_uc_cnt_get(unit, pCnt);
}

yt_ret_t  yt_l2_fdb_port_uc_cnt_get(yt_unit_t unit, yt_port_t port, uint32 *pCnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pCnt), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_fdb_port_uc_cnt_get(unit, port, pCnt);
}

yt_ret_t  yt_l2_fdb_lag_uc_cnt_get(yt_unit_t unit, uint8_t groupId, uint32_t *pCnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_LAG_NUM_MAX <= groupId), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pCnt), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_fdb_lag_uc_cnt_get(unit, groupId, pCnt);
}

yt_ret_t  yt_l2_fdb_aging_port_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->l2_fdb_aging_port_en_set(unit, port, enable);
}

yt_ret_t  yt_l2_fdb_aging_port_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_fdb_aging_port_en_get(unit, port, pEnable);
}

yt_ret_t  yt_l2_fdb_aging_time_set(yt_unit_t unit,  uint32_t sec)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((0 == sec), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->l2_fdb_aging_time_set(unit, sec);
}

yt_ret_t  yt_l2_fdb_aging_time_get(yt_unit_t unit,  uint32_t *pSec)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pSec), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_fdb_aging_time_get(unit, pSec);
}

yt_ret_t  yt_l2_fdb_linkdownFlush_en_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->l2_fdb_linkdownFlush_en_set(unit, enable);
}

yt_ret_t  yt_l2_fdb_linkdownFlush_en_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_fdb_linkdownFlush_en_get(unit, pEnable);
}

yt_ret_t  yt_l2_fdb_add_notify_register(yt_unit_t unit, l2_fdb_add_notify func, yt_l2_fdb_notify_type_t type)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == func), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_L2_FDB_NOTIFY_TYPE_PENDING_STATIC_DYNAMIC < type || YT_L2_FDB_NOTIFY_TYPE_INVALID > type), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->l2_fdb_add_notify_register(unit, func, type);
}

yt_ret_t  yt_l2_fdb_del_notify_register(yt_unit_t unit, l2_fdb_del_notify func, yt_l2_fdb_notify_type_t type)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == func), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_L2_FDB_NOTIFY_TYPE_PENDING_STATIC_DYNAMIC < type || YT_L2_FDB_NOTIFY_TYPE_INVALID > type), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->l2_fdb_del_notify_register(unit, func, type);
}

yt_ret_t  yt_l2_fdb_uc_getFirst_fromDB(yt_unit_t unit, l2_ucastMacAddr_info_t *pUcastMac)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pUcastMac), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_fdb_uc_get_first_from_db(unit, pUcastMac);
}

yt_ret_t  yt_l2_fdb_uc_getNext_withMacAndVid_fromDB(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, l2_ucastMacAddr_info_t *pUcastMac)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < vid) || (YT_VLAN_ID_MIN == vid)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((IS_MCAST_ADDR(macAddr.addr)), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pUcastMac), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_fdb_uc_get_next_from_db(unit, vid, macAddr, pUcastMac);
}

yt_ret_t  yt_l2_port_learnlimit_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->l2_port_learnlimit_en_set(unit, port, enable);
}

yt_ret_t  yt_l2_port_learnlimit_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_port_learnlimit_en_get(unit, port, pEnable);
}

yt_ret_t  yt_l2_port_learnlimit_cnt_set(yt_unit_t unit, yt_port_t port, uint32_t maxCnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_L2_FDB_NUM_MAX(unit) < maxCnt), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->l2_port_learnlimit_cnt_set(unit, port, maxCnt);
}

yt_ret_t  yt_l2_port_learnlimit_cnt_get(yt_unit_t unit, yt_port_t port, uint32_t *pMaxCnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pMaxCnt), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_port_learnlimit_cnt_get(unit, port, pMaxCnt);
}

yt_ret_t  yt_l2_port_learnlimit_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t actType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK(((ACT_TYPE_FWD > actType) || (ACT_TYPE_TRAP < actType)), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->l2_port_learnlimit_act_set(unit, port, actType);
}

yt_ret_t  yt_l2_port_learnlimit_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pActType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pActType), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_port_learnlimit_act_get(unit, port, pActType);
}

yt_ret_t  yt_l2_system_learnlimit_en_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->l2_system_learnlimit_en_set(unit, enable);
}

yt_ret_t  yt_l2_system_learnlimit_en_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_system_learnlimit_en_get(unit, pEnable);
}

yt_ret_t  yt_l2_system_learnlimit_cnt_set(yt_unit_t unit, uint32_t maxCnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_L2_FDB_NUM_MAX(unit) < maxCnt), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->l2_system_learnlimit_cnt_set(unit, maxCnt);
}

yt_ret_t  yt_l2_system_learnlimit_cnt_get(yt_unit_t unit, uint32_t *pMaxCnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pMaxCnt), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_system_learnlimit_cnt_get(unit, pMaxCnt);
}

yt_ret_t  yt_l2_system_learnlimit_act_set(yt_unit_t unit, yt_act_type_t actType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((ACT_TYPE_FWD > actType) || (ACT_TYPE_TRAP < actType)), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->l2_system_learnlimit_act_set(unit, actType);
}

yt_ret_t  yt_l2_system_learnlimit_act_get(yt_unit_t unit, yt_act_type_t *pActType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pActType), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_system_learnlimit_act_get(unit, pActType);
}

yt_ret_t  yt_l2_lag_learnlimit_en_set(yt_unit_t unit, uint8_t groupId,  yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_LAG_NUM_MAX <= groupId), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->l2_lag_learnlimit_en_set(unit, groupId, enable);
}

yt_ret_t  yt_l2_lag_learnlimit_en_get(yt_unit_t unit, uint8_t groupId, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_LAG_NUM_MAX <= groupId), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_lag_learnlimit_en_get(unit, groupId, pEnable);
}

yt_ret_t  yt_l2_lag_learnlimit_cnt_set(yt_unit_t unit, uint8_t groupId,  uint32_t maxCnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_LAG_NUM_MAX <= groupId), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_L2_FDB_NUM_MAX(unit) < maxCnt), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->l2_lag_learnlimit_cnt_set(unit, groupId, maxCnt);
}

yt_ret_t  yt_l2_lag_learnlimit_cnt_get(yt_unit_t unit, uint8_t groupId,  uint32_t *pMaxCnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_LAG_NUM_MAX <= groupId), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pMaxCnt), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_lag_learnlimit_cnt_get(unit, groupId, pMaxCnt);
}

yt_ret_t  yt_l2_lag_learnlimit_act_set(yt_unit_t unit, uint8_t groupId, yt_act_type_t actType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_LAG_NUM_MAX <= groupId), CMM_ERR_PORT);
    CMM_PARAM_CHK(((ACT_TYPE_FWD > actType) || (ACT_TYPE_TRAP < actType)), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->l2_lag_learnlimit_act_set(unit, groupId, actType);
}

yt_ret_t  yt_l2_lag_learnlimit_act_get(yt_unit_t unit, uint8_t groupId, yt_act_type_t *pActType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_LAG_NUM_MAX <= groupId), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pActType), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_lag_learnlimit_act_get(unit, groupId, pActType);
}

yt_ret_t  yt_l2_port_learn_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->l2_port_learn_en_set(unit, port, enable);
}

yt_ret_t  yt_l2_port_learn_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_port_learn_en_get(unit, port, pEnable);
}

yt_ret_t  yt_l2_port_learn_mode_set(yt_unit_t unit, yt_port_t port, yt_l2_learn_mode_t mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK(((YT_L2_LEARN_MODE_AUTO > mode) || (YT_L2_LEARN_MODE_SUSPEND < mode)), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->l2_port_learn_mode_set(unit, port, mode);
}

yt_ret_t  yt_l2_port_learn_mode_get(yt_unit_t unit, yt_port_t port, yt_l2_learn_mode_t *pMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_port_learn_mode_get(unit, port, pMode);
}

yt_ret_t  yt_l2_port_suspend_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t actType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < actType) || (ACT_TYPE_FWD > actType)), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->l2_port_suspend_act_set(unit, port, actType);
}

yt_ret_t  yt_l2_port_suspend_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pActType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pActType), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_port_suspend_act_get(unit, port, pActType);
}

yt_ret_t  yt_l2_new_sa_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t actType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < actType) || (ACT_TYPE_FWD > actType)), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->l2_new_sa_act_set(unit, port, actType);
}

yt_ret_t  yt_l2_new_sa_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pActType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pActType), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_new_sa_act_get(unit, port, pActType);
}

yt_ret_t yt_l2_unknown_ucast_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t action)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((ACT_TYPE_TRAP < action || ACT_TYPE_FWD > action), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->l2_unknown_ucast_act_set(unit, port, action);
}

yt_ret_t yt_l2_unknown_ucast_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_unknown_ucast_act_get(unit, port, pAction);
}

yt_ret_t yt_l2_unknown_mcast_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t action)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((ACT_TYPE_TRAP < action || ACT_TYPE_FWD > action), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->l2_unknown_mcast_act_set(unit, port, action);
}

yt_ret_t yt_l2_unknown_mcast_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_unknown_mcast_act_get(unit, port, pAction);
}

yt_ret_t  yt_l2_filter_mcast_set(yt_unit_t unit, yt_port_mask_t port_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PLIST_CPUPORT_VALID(unit, port_mask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->l2_filter_mcast_set(unit, port_mask);
}

yt_ret_t  yt_l2_filter_mcast_get(yt_unit_t unit, yt_port_mask_t *pport_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pport_mask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_filter_mcast_get(unit, pport_mask);
}

yt_ret_t  yt_l2_filter_bcast_set(yt_unit_t unit, yt_port_mask_t port_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PLIST_CPUPORT_VALID(unit, port_mask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->l2_filter_bcast_set(unit, port_mask);
}

yt_ret_t  yt_l2_filter_bcast_get(yt_unit_t unit, yt_port_mask_t *pport_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pport_mask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_filter_bcast_get(unit, pport_mask);
}

yt_ret_t  yt_l2_filter_unknown_ucast_set(yt_unit_t unit, yt_port_mask_t port_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PLIST_CPUPORT_VALID(unit, port_mask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->l2_filter_unknown_ucast_set(unit, port_mask);
}

yt_ret_t  yt_l2_filter_unknown_ucast_get(yt_unit_t unit, yt_port_mask_t *pport_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pport_mask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_filter_unknown_ucast_get(unit, pport_mask);
}

yt_ret_t  yt_l2_filter_unknown_mcast_set(yt_unit_t unit, yt_port_mask_t port_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PLIST_CPUPORT_VALID(unit, port_mask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->l2_filter_unknown_mcast_set(unit, port_mask);
}

yt_ret_t  yt_l2_filter_unknown_mcast_get(yt_unit_t unit, yt_port_mask_t *pport_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pport_mask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->l2_filter_unknown_mcast_get(unit, pport_mask);
}

