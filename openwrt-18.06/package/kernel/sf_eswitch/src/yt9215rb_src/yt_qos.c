/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_qos.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_qos.h"
#include "fal_dispatch.h"

yt_ret_t yt_qos_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->qos_init(unit);
}

yt_ret_t yt_qos_intPri_sel_precedence_set(yt_unit_t unit, yt_port_t port, yt_qos_intPri_sel_precedence_t *pPriTbl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pPriTbl), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((0 == pPriTbl->flags), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intPri_sel_precedence_set(unit, port, pPriTbl);
}

yt_ret_t yt_qos_intPri_sel_precedence_get(yt_unit_t unit, yt_port_t port, yt_qos_intPri_sel_precedence_t *pPriTbl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pPriTbl), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((0 == pPriTbl->flags), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intPri_sel_precedence_get(unit, port, pPriTbl);
}

yt_ret_t yt_qos_intPri_portDefPri_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable, yt_pri_t pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < pri, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intPri_portDefPri_set (unit, port, enable, pri);
}

yt_ret_t  yt_qos_intPri_portDefPri_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable, yt_pri_t *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pPri), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intPri_portDefPri_get (unit, port, pEnable, pPri);
}

yt_ret_t yt_qos_intPri_intCpri_map_set(yt_unit_t unit, yt_pri_t cpri, yt_pri_t pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < pri, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < cpri, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intPri_intCpri_map_set(unit, cpri, pri);
}

yt_ret_t yt_qos_intPri_intCpri_map_get(yt_unit_t unit, yt_pri_t cpri, yt_pri_t *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < cpri, CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == pPri, CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intPri_intCpri_map_get(unit, cpri, pPri);
}

yt_ret_t yt_qos_intPri_intSpri_map_set(yt_unit_t unit, yt_pri_t spri, yt_pri_t pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < pri, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < spri, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intPri_intSpri_map_set(unit, spri, pri);
}

yt_ret_t yt_qos_intPri_intSpri_map_get(yt_unit_t unit, yt_pri_t spri, yt_pri_t *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < spri, CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == pPri, CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intPri_intSpri_map_get(unit, spri, pPri);
}

yt_ret_t yt_qos_intPri_cpri_map_set(yt_unit_t unit, yt_qos_pmap_tci_t cpri, yt_pri_t pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < pri, CMM_ERR_INPUT);
    CMM_PARAM_CHK(TRUE < cpri.dei, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < cpri.prio, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intPri_cpri_map_set(unit, cpri, pri);
}

yt_ret_t yt_qos_intPri_cpri_map_get(yt_unit_t unit, yt_qos_pmap_tci_t cpri, yt_pri_t *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(TRUE < cpri.dei, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < cpri.prio, CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pPri), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intPri_cpri_map_get(unit, cpri, pPri);
}

yt_ret_t yt_qos_intPri_spri_map_set(yt_unit_t unit, yt_qos_pmap_tci_t spri, yt_pri_t pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < pri, CMM_ERR_INPUT);
    CMM_PARAM_CHK(TRUE < spri.dei, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < spri.prio, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intPri_spri_map_set(unit, spri, pri);
}

yt_ret_t yt_qos_intPri_spri_map_get(yt_unit_t unit, yt_qos_pmap_tci_t spri, yt_pri_t *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pPri), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(TRUE < spri.dei, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < spri.prio, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intPri_spri_map_get(unit, spri, pPri);
}

yt_ret_t yt_qos_intPri_dscp_map_set(yt_unit_t unit, yt_dscp_t dscp, yt_pri_t pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_DSCP(unit) < dscp, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < pri, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intPri_dscp_map_set(unit, dscp, pri);
}

yt_ret_t yt_qos_intPri_dscp_map_get(yt_unit_t unit, yt_dscp_t dscp, yt_pri_t *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pPri), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_DSCP(unit) < dscp, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intPri_dscp_map_get(unit, dscp, pPri);
}

yt_ret_t  yt_qos_intPri_vlan_map_set(yt_unit_t unit, yt_vlan_t vid, yt_enable_t enable, yt_pri_t pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < pri, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intPri_vlan_map_set(unit, vid, enable, pri);
}

yt_ret_t  yt_qos_intPri_vlan_map_get(yt_unit_t unit, yt_vlan_t vid, yt_enable_t *pEnable, yt_pri_t *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pPri), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intPri_vlan_map_get(unit, vid, pEnable, pPri);
}

yt_ret_t yt_qos_intPri_map_igrMirror_set(yt_unit_t unit, yt_mirror_group_t grpId, yt_enable_t enable, yt_pri_t pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MIRROR_MAX_GROUP_NUM <= grpId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < pri, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intPri_map_igrMirror_set(unit, grpId, enable, pri);
}

yt_ret_t yt_qos_intPri_map_igrMirror_get(yt_unit_t unit, yt_mirror_group_t grpId, yt_enable_t *pEnable, yt_pri_t *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MIRROR_MAX_GROUP_NUM <= grpId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pPri), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intPri_map_igrMirror_get(unit, grpId, pEnable, pPri);
}

yt_ret_t yt_qos_intPri_map_egrMirror_set(yt_unit_t unit, yt_mirror_group_t grpId, yt_enable_t enable, yt_pri_t pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MIRROR_MAX_GROUP_NUM <= grpId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < pri, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intPri_map_egrMirror_set(unit, grpId, enable, pri);
}

yt_ret_t yt_qos_intPri_map_egrMirror_get(yt_unit_t unit, yt_mirror_group_t grpId, yt_enable_t *pEnable, yt_pri_t *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MIRROR_MAX_GROUP_NUM <= grpId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pPri), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intPri_map_egrMirror_get(unit, grpId, pEnable, pPri);
}

yt_ret_t yt_qos_intDP_cpri_map_set(yt_unit_t unit, yt_qos_pmap_tci_t cpri, yt_dp_t dp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_DROP(unit) < dp, CMM_ERR_INPUT);
    CMM_PARAM_CHK(TRUE < cpri.dei, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < cpri.prio, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intDP_cpri_map_set(unit, cpri, dp);
}

yt_ret_t yt_qos_intDP_cpri_map_get(yt_unit_t unit, yt_qos_pmap_tci_t cpri, yt_dp_t *pDp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(TRUE < cpri.dei, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < cpri.prio, CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pDp), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intDP_cpri_map_get(unit, cpri, pDp);
}

yt_ret_t yt_qos_intDP_spri_map_set(yt_unit_t unit, yt_qos_pmap_tci_t spri, yt_dp_t dp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_DROP(unit) < dp, CMM_ERR_INPUT);
    CMM_PARAM_CHK(TRUE < spri.dei, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < spri.prio, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intDP_spri_map_set(unit, spri, dp);
}

yt_ret_t yt_qos_intDP_spri_map_get(yt_unit_t unit, yt_qos_pmap_tci_t spri, yt_dp_t *pDp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pDp), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(TRUE < spri.dei, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < spri.prio, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intDP_spri_map_get(unit, spri, pDp);
}

yt_ret_t yt_qos_intDP_dscp_map_set(yt_unit_t unit, yt_dscp_t dscp, yt_dp_t dp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_DSCP(unit) < dscp, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_DROP(unit) < dp, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intDP_dscp_map_set(unit, dscp, dp);
}

yt_ret_t yt_qos_intDP_dscp_map_get(yt_unit_t unit, yt_dscp_t dscp, yt_dp_t *pDp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pDp), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_DSCP(unit) < dscp, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intDP_dscp_map_get(unit, dscp, pDp);
}

yt_ret_t yt_qos_que_map_ucast_set(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *pQmap)
{
    uint32_t i;

    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pQmap), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((0 == pQmap->flags), CMM_ERR_INPUT);
    for (i = 0; i < YT_QOS_MAX_QUEUE_NUM; i++)
    {
        if (IS_BIT_SET(pQmap->flags, i))
        {
            CMM_PARAM_CHK((CAL_MAX_UCAST_QUEUE_NUM(unit) <= pQmap->qid[i]), CMM_ERR_INPUT);
        }
    }

    return YT_DISPATCH(unit)->qos_que_map_ucast_set(unit, port, pQmap);
}

yt_ret_t yt_qos_que_map_ucast_get(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *pQmap)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pQmap), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((0 == pQmap->flags), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_que_map_ucast_get(unit, port, pQmap);
}

yt_ret_t yt_qos_que_map_mcast_set(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *pQmap)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pQmap), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((0 == pQmap->flags), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_que_map_mcast_set(unit, port, pQmap);
}

yt_ret_t yt_qos_que_map_mcast_get(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *pQmap)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pQmap), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((0 == pQmap->flags), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_que_map_mcast_get(unit, port, pQmap);
}

yt_ret_t yt_qos_que_forceDrop_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_que_forceDrop_enable_get(unit, port, pEnable);
}

yt_ret_t yt_qos_que_forceDrop_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_que_forceDrop_enable_set(unit, port, enable);
}

yt_ret_t yt_qos_remark_port_set(yt_unit_t unit, yt_port_t port, const yt_qos_remark_ctrl_t *pRemarkCtrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pRemarkCtrl), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((0 == pRemarkCtrl->flags), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_remark_port_set(unit, port, pRemarkCtrl);
}

yt_ret_t yt_qos_remark_port_get(yt_unit_t unit, yt_port_t port, yt_qos_remark_ctrl_t *pRemarkCtrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pRemarkCtrl), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((0 == pRemarkCtrl->flags), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_remark_port_get(unit, port, pRemarkCtrl);
}

yt_ret_t yt_qos_remark_dscp_set(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_dscp_t newDscp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < srcInfo.prio, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_DROP(unit) < srcInfo.dp, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_DSCP(unit) < newDscp, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_remark_dscp_set(unit, srcInfo, newDscp);
}

yt_ret_t yt_qos_remark_dscp_get(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_dscp_t *pNewDscp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pNewDscp), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < srcInfo.prio, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_DROP(unit) < srcInfo.dp, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_remark_dscp_get(unit, srcInfo, pNewDscp);
}

yt_ret_t yt_qos_remark_cpri_set(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t dstInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < srcInfo.prio, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_DROP(unit) < srcInfo.dp, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < dstInfo.prio, CMM_ERR_INPUT);
    CMM_PARAM_CHK(TRUE < dstInfo.dei, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_remark_cpri_set(unit, srcInfo, dstInfo);
}

yt_ret_t yt_qos_remark_cpri_get(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t *pDstInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pDstInfo), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < srcInfo.prio, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_DROP(unit) < srcInfo.dp, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_remark_cpri_get(unit, srcInfo, pDstInfo);
}

yt_ret_t yt_qos_remark_spri_set(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t dstInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < srcInfo.prio, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_DROP(unit) < srcInfo.dp, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < dstInfo.prio, CMM_ERR_INPUT);
    CMM_PARAM_CHK(TRUE < dstInfo.dei, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_remark_spri_set(unit, srcInfo, dstInfo);
}

yt_ret_t yt_qos_remark_spri_get(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t *pDstInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pDstInfo), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < srcInfo.prio, CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_INT_DROP(unit) < srcInfo.dp, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_remark_spri_get(unit, srcInfo, pDstInfo);
}

yt_ret_t yt_qos_schedule_queue_pri_set(yt_unit_t unit, yt_qid_t qinfo, yt_queue_pri_t qpri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, qinfo.port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_QOS_QUEUE_MAX_PRI < qpri), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_schedule_queue_pri_set(unit, qinfo, qpri);
}

yt_ret_t yt_qos_schedule_queue_pri_get(yt_unit_t unit, yt_qid_t qinfo, yt_queue_pri_t *pQpri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, qinfo.port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pQpri), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_schedule_queue_pri_get(unit, qinfo, pQpri);
}

yt_ret_t yt_qos_schedule_dwrr_mode_set(yt_unit_t unit, yt_qid_t qinfo, yt_rate_mode_t dwrrMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, qinfo.port))), CMM_ERR_PORT);
    CMM_PARAM_CHK(YT_RATE_MODE_PPS < dwrrMode, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_schedule_dwrr_mode_set(unit, qinfo, dwrrMode);
}

yt_ret_t yt_qos_schedule_dwrr_mode_get(yt_unit_t unit, yt_qid_t qinfo, yt_rate_mode_t *pDwrrMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, qinfo.port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pDwrrMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_schedule_dwrr_mode_get(unit, qinfo, pDwrrMode);
}

yt_ret_t yt_qos_schedule_dwrr_weight_set(yt_unit_t unit, yt_qid_t qinfo, yt_queue_weight_t qweight)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, qinfo.port))), CMM_ERR_PORT);
    CMM_PARAM_CHK(YT_QOS_DWRR_MAX_WEIGHT < qweight, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_schedule_dwrr_weight_set(unit, qinfo, qweight);
}

yt_ret_t yt_qos_schedule_dwrr_weight_get(yt_unit_t unit, yt_qid_t qinfo, yt_queue_weight_t *pQweight)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, qinfo.port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pQweight), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_schedule_dwrr_weight_get(unit, qinfo, pQweight);
}

