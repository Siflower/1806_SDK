/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_multicast.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_multicast.h"
#include "fal_dispatch.h"


yt_ret_t  yt_multicast_ipv4_add(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv4_info_t *pIpinfo, yt_port_mask_t portMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_MCAST_MODE_SIP_GIP_VID < mode) || YT_MCAST_MODE_GIP > mode), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pIpinfo), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < pIpinfo->vid)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, portMask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->multicast_ipv4_add(unit, mode, pIpinfo, portMask);
}

yt_ret_t  yt_multicast_ipv6_add(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv6_info_t *pIpinfo, yt_port_mask_t portMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_MCAST_MODE_SIP_GIP_VID < mode) || YT_MCAST_MODE_GIP > mode), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pIpinfo), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < pIpinfo->vid)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, portMask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->multicast_ipv6_add(unit, mode, pIpinfo, portMask);
}

yt_ret_t  yt_multicast_macaddr_add(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_port_mask_t portMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, portMask))), CMM_ERR_PORTLIST);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < vid) || (YT_VLAN_ID_MIN == vid)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((!IS_MCAST_ADDR(macAddr.addr)), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->multicast_macaddr_add(unit, vid, macAddr, portMask);
}

yt_ret_t  yt_multicast_ipv4_del(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv4_info_t *pIpinfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_MCAST_MODE_SIP_GIP_VID < mode) || YT_MCAST_MODE_GIP > mode), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pIpinfo), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < pIpinfo->vid)), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->multicast_ipv4_del(unit, mode, pIpinfo);
}

yt_ret_t  yt_multicast_ipv6_del(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv6_info_t *pIpinfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_MCAST_MODE_SIP_GIP_VID < mode) || YT_MCAST_MODE_GIP > mode), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pIpinfo), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < pIpinfo->vid)), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->multicast_ipv6_del(unit, mode, pIpinfo);
}

yt_ret_t  yt_multicast_macaddr_del(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < vid) || (YT_VLAN_ID_MIN == vid)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((!IS_MCAST_ADDR(macAddr.addr)), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->multicast_macaddr_del(unit, vid, macAddr);
}

yt_ret_t  yt_multicast_ipv4_withIpAndVid_get(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv4_info_t *pIpinfo,  yt_mcast_ipv4_data_t *pData)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_MCAST_MODE_SIP_GIP_VID < mode) || YT_MCAST_MODE_GIP > mode), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pIpinfo), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < pIpinfo->vid)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pData), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->multicast_ipv4_withIpAndVid_get(unit, mode, pIpinfo, pData);
}

yt_ret_t  yt_multicast_ipv4_withindex_get(yt_unit_t unit, uint16_t index, yt_mcast_ipv4_data_t *pData)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_L2_FDB_NUM_MAX(unit) <= index), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pData), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->multicast_ipv4_withindex_get(unit, index, pData);
}

yt_ret_t  yt_multicast_ipv4_withindex_getnext(yt_unit_t unit, uint16_t index, uint16_t *pNextIdx, yt_mcast_ipv4_data_t *pData)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_L2_FDB_NUM_MAX(unit) <= index), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pNextIdx), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pData), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->multicast_ipv4_withindex_getnext(unit, index, pNextIdx, pData);
}

yt_ret_t  yt_multicast_ipv6_withIpAndVid_get(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv6_info_t *pIpinfo,  yt_mcast_ipv6_data_t *pData)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_MCAST_MODE_SIP_GIP_VID < mode) || YT_MCAST_MODE_GIP > mode), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pIpinfo), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < pIpinfo->vid)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pData), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->multicast_ipv6_withIpAndVid_get(unit, mode, pIpinfo, pData);
}

yt_ret_t  yt_multicast_ipv6_withindex_get(yt_unit_t unit, uint16_t index, yt_mcast_ipv6_data_t *pData)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_L2_FDB_NUM_MAX(unit) <= index), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pData), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->multicast_ipv6_withindex_get(unit, index, pData);
}

yt_ret_t  yt_multicast_ipv6_withindex_getnext(yt_unit_t unit, uint16_t index, uint16_t *pNextIdx, yt_mcast_ipv6_data_t *pData)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_L2_FDB_NUM_MAX(unit) <= index), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pNextIdx), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pData), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->multicast_ipv6_withindex_getnext(unit, index, pNextIdx, pData);
}

yt_ret_t  yt_multicast_macaddr_withMacAndVid_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_mcast_mac_data_t *pData)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < vid) || (YT_VLAN_ID_MIN == vid)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((!IS_MCAST_ADDR(macAddr.addr)), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pData), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->multicast_macaddr_withMacAndVid_get(unit, vid, macAddr, pData);
}

yt_ret_t  yt_multicast_macaddr_withindex_get(yt_unit_t unit, uint16_t index, yt_mcast_mac_data_t *pData)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_L2_FDB_NUM_MAX(unit) <= index), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pData), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->multicast_macaddr_withindex_get(unit, index, pData);
}

yt_ret_t  yt_multicast_macaddr_withindex_getnext(yt_unit_t unit, uint16_t index,  uint16_t *pNextIdx, yt_mcast_mac_data_t *pData)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_L2_FDB_NUM_MAX(unit) <= index), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pNextIdx), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pData), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->multicast_macaddr_withindex_getnext(unit, index, pNextIdx, pData);
}

yt_ret_t  yt_multicast_all_ipmc_flush(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->multicast_all_ipmc_flush(unit);
}

yt_ret_t  yt_multicast_vid_ipmc_flush(yt_unit_t unit, yt_vlan_t vid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < vid) || (YT_VLAN_ID_MIN == vid)), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->multicast_vid_ipmc_flush(unit, vid);
}

yt_ret_t  yt_multicast_all_macaddr_flush(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->multicast_all_macaddr_flush(unit);
}

yt_ret_t  yt_multicast_vid_macaddr_flush(yt_unit_t unit, yt_vlan_t vid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < vid) || (YT_VLAN_ID_MIN == vid)), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->multicast_vid_macaddr_flush(unit, vid);
}

yt_ret_t yt_multicast_port_macaddr_flush(yt_unit_t unit, yt_port_t port)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->multicast_port_macaddr_flush(unit, port);
}

yt_ret_t  yt_multicast_macaddr_cnt_get(yt_unit_t unit,  uint32 *pcnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pcnt), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->multicast_macaddr_cnt_get(unit, pcnt);
}

yt_ret_t yt_multicast_vlan_add(yt_unit_t unit, yt_vlan_t vid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->multicast_vlan_add(unit, vid);
}

yt_ret_t yt_multicast_vlan_get(yt_unit_t unit, yt_multi_vlan_t *pvlanarray)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pvlanarray), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->multicast_vlan_get(unit, pvlanarray);
}

yt_ret_t yt_multicast_vlan_del(yt_unit_t unit, yt_vlan_t vid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->multicast_vlan_del(unit, vid);
}

yt_ret_t yt_multicast_hwmc_flush_all(yt_unit_t unit, yt_mcast_hw_mode_t mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_MCAST_HW_MODE_MAC_AND_IP < mode) || YT_MCAST_HW_MODE_MAC > mode), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->multicast_hwmc_flush_all(unit, mode);
}

yt_ret_t yt_multicast_hwmc_flush_by_vid(yt_unit_t unit, yt_mcast_hw_mode_t mode, yt_vlan_t vid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_MCAST_HW_MODE_MAC_AND_IP < mode) || YT_MCAST_HW_MODE_MAC > mode), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->multicast_hwmc_flush_by_vid(unit, mode, vid);
}

yt_ret_t yt_multicast_hwmc_flush_by_portmask(yt_unit_t unit, yt_mcast_hw_mode_t mode, yt_port_mask_t portMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_MCAST_HW_MODE_MAC_AND_IP < mode) || YT_MCAST_HW_MODE_MAC > mode), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, portMask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->multicast_hwmc_flush_by_portmask(unit, mode, portMask);
}

yt_ret_t yt_multicast_hwmc_mac_del(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_port_mask_t portMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((!IS_MCAST_ADDR(macAddr.addr)), CMM_ERR_INPUT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, portMask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->multicast_hwmc_mac_del(unit, vid, macAddr, portMask);
}

yt_ret_t yt_multicast_hwmc_ipv4_del(yt_unit_t unit, yt_vlan_t vid, yt_ip_addr_t gip, yt_port_mask_t portMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, portMask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->multicast_hwmc_ipv4_del(unit, vid, gip, portMask);
}

