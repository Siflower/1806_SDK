/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_igmp_mld.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_igmp_mld.h"
#include "fal_dispatch.h"

#if defined(SWITCH_SERIES_TIGER)
yt_ret_t  yt_igmp_opmode_set(yt_unit_t unit,  yt_igmp_mld_op_mode_t mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MULTI_OP_MODE_LEARN_AND_COPY_TO_CPU < mode || YT_MULTI_OP_MODE_LEARN > mode), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_opmode_set(unit, mode);
}

yt_ret_t  yt_igmp_opmode_get(yt_unit_t unit,  yt_igmp_mld_op_mode_t *pMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_opmode_get(unit, pMode);
}

yt_ret_t  yt_mld_opmode_set(yt_unit_t unit,  yt_igmp_mld_op_mode_t mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MULTI_OP_MODE_LEARN_AND_COPY_TO_CPU < mode ||
        YT_MULTI_OP_MODE_LEARN > mode), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->mld_opmode_set(unit, mode);
}

yt_ret_t  yt_mld_opmode_get(yt_unit_t unit,  yt_igmp_mld_op_mode_t *pMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->mld_opmode_get(unit, pMode);
}

yt_ret_t  yt_igmp_mld_port_report_allow_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_mld_port_report_allow_set(unit, port, enable);
}

yt_ret_t  yt_igmp_mld_port_report_allow_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_port_report_allow_get(unit, port, pEnable);
}

yt_ret_t  yt_igmp_mld_port_leave_allow_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_mld_port_leave_allow_set(unit, port, enable);
}

yt_ret_t  yt_igmp_mld_port_leave_allow_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_port_leave_allow_get(unit, port, pEnable);
}

yt_ret_t  yt_igmp_mld_port_query_allow_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_mld_port_query_allow_set(unit, port, enable);
}

yt_ret_t  yt_igmp_mld_port_query_allow_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_port_query_allow_get(unit, port, pEnable);
}

yt_ret_t  yt_igmp_mld_fastleave_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_mld_fastleave_set(unit, enable);
}

yt_ret_t  yt_igmp_mld_fastleave_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_fastleave_get(unit, pEnable);
}

yt_ret_t  yt_igmp_mld_learnlimit_en_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_mld_learnlimit_en_set(unit, enable);
}

yt_ret_t  yt_igmp_mld_learnlimit_en_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_learnlimit_en_get(unit, pEnable);
}

yt_ret_t  yt_igmp_mld_learnlimit_maxgroup_set(yt_unit_t unit, uint32_t  maxgroup)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->igmp_mld_learnlimit_maxgroup_set(unit, maxgroup);
}

yt_ret_t  yt_igmp_mld_learnlimit_maxgroup_get(yt_unit_t unit, uint32_t  *pmaxgroup)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pmaxgroup), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_learnlimit_maxgroup_get(unit, pmaxgroup);
}

yt_ret_t  yt_igmp_mld_fwd_routerport_only_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_mld_fwd_routerport_only_set(unit, enable);
}

yt_ret_t  yt_igmp_mld_fwd_routerport_only_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_fwd_routerport_only_get(unit, pEnable);
}

yt_ret_t  yt_igmp_mld_fwd_routerport_primary_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_mld_fwd_routerport_primary_set(unit, enable);
}

yt_ret_t  yt_igmp_mld_fwd_routerport_primary_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_fwd_routerport_primary_get(unit, pEnable);
}

yt_ret_t yt_igmp_mld_bypass_grouprange_set(yt_unit_t unit, yt_igmp_mld_learn_bypass_range_t bypass)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->igmp_mld_bypass_grouprange_set(unit, bypass);
}

yt_ret_t yt_igmp_mld_bypass_grouprange_get(yt_unit_t unit, yt_igmp_mld_learn_bypass_range_t *pbypass)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pbypass), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_bypass_grouprange_get(unit, pbypass);
}

yt_ret_t  yt_igmp_mld_dynamic_routerport_get(yt_unit_t unit, yt_igmp_mld_router_port_t *prouter_port)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == prouter_port), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_dynamic_routerport_get(unit, prouter_port);
}

yt_ret_t yt_igmp_mld_dynamic_routerport_allow_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_mld_dynamic_routerport_allow_set(unit, port, enable);
}

yt_ret_t yt_igmp_mld_dynamic_routerport_allow_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_dynamic_routerport_allow_get(unit, port, pEnable);
}

yt_ret_t yt_igmp_mld_dynamic_routerport_agingtime_set(yt_unit_t unit, uint32_t sec)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->igmp_mld_dynamic_routerport_agingtime_set(unit, sec);
}

yt_ret_t yt_igmp_mld_dynamic_routerport_agingtime_get(yt_unit_t unit, uint32_t *psec)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == psec), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_dynamic_routerport_agingtime_get(unit, psec);
}

yt_ret_t  yt_igmp_mld_static_routerport_set(yt_unit_t unit, yt_port_mask_t port_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit,port_mask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->igmp_mld_static_routerport_set(unit, port_mask);
}

yt_ret_t  yt_igmp_mld_static_routerport_get(yt_unit_t unit, yt_port_mask_t *pport_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->igmp_mld_static_routerport_get(unit, pport_mask);
}

yt_ret_t yt_igmp_mld_bypass_portIsolation_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_mld_bypass_portIsolation_set(unit, port, enable);
}

yt_ret_t yt_igmp_mld_bypass_portIsolation_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_bypass_portIsolation_get(unit, port, pEnable);
}

yt_ret_t yt_igmp_mld_ipmc_bypass_portIsolation_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_mld_ipmc_bypass_portIsolation_set(unit, enable);
}

yt_ret_t yt_igmp_mld_ipmc_bypass_portIsolation_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_ipmc_bypass_portIsolation_get(unit, pEnable);
}

yt_ret_t  yt_igmp_mld_bypass_vlanIngrfilter_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_mld_bypass_vlanIngrfilter_en_set(unit, port, enable);
}

yt_ret_t  yt_igmp_mld_bypass_vlanIngrfilter_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_bypass_vlanIngrfilter_en_get(unit, port, pEnable);
}
#endif

#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)

yt_ret_t  yt_igmp_mld_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->igmp_mld_init(unit);
}

yt_ret_t  yt_igmp_dynamicRouterPort_aging_time_set(yt_unit_t unit,  uint32_t sec)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(YT_IGMP_ROUTER_PORT_AGING_MAXTIME <= sec, CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->igmp_dynamicRouterPort_aging_time_set(unit, sec);
}

yt_ret_t  yt_igmp_dynamicRouterPort_aging_time_get(yt_unit_t unit,  uint32_t *pSec)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pSec), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_dynamicRouterPort_aging_time_get(unit, pSec);
}

yt_ret_t  yt_igmp_dynamicRouterPort_get(yt_unit_t unit,  uint8_t idx, yt_vlan_t *pVid, yt_port_t *pPort, uint32_t *pTime)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(YT_IGMP_DYNAMIC_ROUTER_GROUP_NUM <= idx, CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pVid), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pPort), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pTime), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_dynamicRouterPort_get(unit, idx, pVid, pPort, pTime);
}

yt_ret_t  yt_igmp_forbidden_dynamicRouterPort_en_set(yt_unit_t unit,  yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_forbidden_dynamicRouterPort_en_set(unit, port, enable);
}

yt_ret_t  yt_igmp_forbidden_dynamicRouterPort_en_get(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_forbidden_dynamicRouterPort_en_get(unit, port, pEnable);
}

yt_ret_t  yt_igmp_staticRouterPort_add_by_idx(yt_unit_t unit,  uint8_t idx, yt_vlan_t vid, yt_port_mask_t portMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(YT_IGMP_STATIC_ROUTER_GROUP_NUM <= idx, CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid || YT_VLAN_ID_MIN == vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, portMask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->igmp_staticRouterPort_add_by_idx(unit, idx, vid, portMask);
}

yt_ret_t  yt_igmp_staticRouterPort_get(yt_unit_t unit,  uint8_t idx, yt_vlan_t *pVid, yt_port_mask_t *pPortMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(YT_IGMP_STATIC_ROUTER_GROUP_NUM <= idx, CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pVid), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pPortMask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_staticRouterPort_get(unit, idx, pVid, pPortMask);
}

yt_ret_t  yt_igmp_staticRouterPort_del(yt_unit_t unit,  uint8_t idx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(YT_IGMP_STATIC_ROUTER_GROUP_NUM <= idx, CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->igmp_staticRouterPort_del(unit, idx);
}

yt_ret_t  yt_igmp_fast_leave_en_set(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    if(0 == isLag)
    {
        CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    }
    else
    {
        CMM_PARAM_CHK((YT_LAG_NUM_MAX <= port), CMM_ERR_PORT);
    }
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_fast_leave_en_set(unit, port, isLag, enable);
}

yt_ret_t  yt_igmp_fast_leave_en_get(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    if(0 == isLag)
    {
        CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    }
    else
    {
        CMM_PARAM_CHK((YT_LAG_NUM_MAX <= port), CMM_ERR_PORT);
    }
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_fast_leave_en_get(unit, port, isLag, pEnable);
}

yt_ret_t  yt_igmp_report_leave_fwd_ctrl_set(yt_unit_t unit, yt_igmp_version_t version, yt_igmp_mld_fwd_ports_t fwdCtrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_IGMP_V3 < version || YT_IGMP_V1V2 > version), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_IGMP_MLD_ROUTER_PORTS < fwdCtrl || YT_IGMP_MLD_FLOOD > fwdCtrl), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_report_leave_fwd_ctrl_set(unit, version, fwdCtrl);
}

yt_ret_t  yt_igmp_report_leave_fwd_ctrl_get(yt_unit_t unit, yt_igmp_version_t version, yt_igmp_mld_fwd_ports_t *pFwdCtrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_IGMP_V3 < version || YT_IGMP_V1V2 > version), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pFwdCtrl), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_report_leave_fwd_ctrl_get(unit, version, pFwdCtrl);
}

yt_ret_t  yt_igmp_ctrlPkt_act_set(yt_unit_t unit,  yt_port_t port, yt_act_type_t actType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((ACT_TYPE_TRAP < actType || ACT_TYPE_FWD > actType), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_ctrlPkt_act_set(unit, port, actType);
}

yt_ret_t  yt_igmp_ctrlPkt_act_get(yt_unit_t unit,  yt_port_t port, yt_act_type_t *pActType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pActType), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_ctrlPkt_act_get(unit, port, pActType);
}

yt_ret_t  yt_igmp_v1v2_ctrlPkt_act_set(yt_unit_t unit,  yt_igmp_v1v2_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t actType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_IGMP_V2_LEAVE < ctrlPkt || YT_IGMP_V1V2_QUERY > ctrlPkt), CMM_ERR_INPUT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((ACT_TYPE_TRAP < actType || ACT_TYPE_FWD > actType), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_v1v2_ctrlPkt_act_set(unit, ctrlPkt, port, actType);
}

yt_ret_t  yt_igmp_v1v2_ctrlPkt_act_get(yt_unit_t unit,  yt_igmp_v1v2_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t *pActType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_IGMP_V2_LEAVE < ctrlPkt || YT_IGMP_V1V2_QUERY > ctrlPkt), CMM_ERR_INPUT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pActType), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_v1v2_ctrlPkt_act_get(unit, ctrlPkt, port, pActType);
}

yt_ret_t  yt_igmp_v3_ctrlPkt_act_set(yt_unit_t unit,  yt_igmp_v3_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t actType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_IGMP_V3_REPORT < ctrlPkt || YT_IGMP_V3_QUERY > ctrlPkt), CMM_ERR_INPUT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((ACT_TYPE_TRAP < actType || ACT_TYPE_FWD > actType), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_v3_ctrlPkt_act_set(unit, ctrlPkt, port, actType);
}

yt_ret_t  yt_igmp_v3_ctrlPkt_act_get(yt_unit_t unit,  yt_igmp_v3_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t *pActType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_IGMP_V3_REPORT < ctrlPkt || YT_IGMP_V3_QUERY > ctrlPkt), CMM_ERR_INPUT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pActType), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_v3_ctrlPkt_act_get(unit, ctrlPkt, port, pActType);
}

yt_ret_t  yt_igmp_v1v2_port_learn_en_set(yt_unit_t unit,  yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_v1v2_port_learn_en_set(unit, port, enable);
}

yt_ret_t  yt_igmp_v1v2_port_learn_en_get(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_v1v2_port_learn_en_get(unit, port, pEnable);
}

yt_ret_t  yt_igmp_v3_port_learn_en_set(yt_unit_t unit,  yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_v3_port_learn_en_set(unit, port, enable);
}

yt_ret_t  yt_igmp_v3_port_learn_en_get(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_v3_port_learn_en_get(unit, port, pEnable);
}

yt_ret_t  yt_igmp_routerPort_dataFwd_en_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_routerPort_dataFwd_en_set(unit, enable);
}

yt_ret_t  yt_igmp_routerPort_dataFwd_en_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_routerPort_dataFwd_en_get(unit, pEnable);
}

yt_ret_t  yt_mld_dynamicRouterPort_aging_time_set(yt_unit_t unit,  uint32_t sec)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(YT_MLD_ROUTER_PORT_AGING_MAXTIME <= sec, CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->mld_dynamicRouterPort_aging_time_set(unit, sec);
}

yt_ret_t  yt_mld_dynamicRouterPort_aging_time_get(yt_unit_t unit,  uint32_t *pSec)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pSec), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->mld_dynamicRouterPort_aging_time_get(unit, pSec);
}

yt_ret_t  yt_mld_dynamicRouterPort_get(yt_unit_t unit,  uint8_t idx, yt_vlan_t *pVid, yt_port_t *pPort, uint32_t *pTime)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(YT_MLD_DYNAMIC_ROUTER_GROUP_NUM <= idx, CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pVid), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pPort), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pTime), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->mld_dynamicRouterPort_get(unit, idx, pVid, pPort, pTime);
}

yt_ret_t  yt_mld_forbidden_dynamicRouterPort_en_set(yt_unit_t unit,  yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->mld_forbidden_dynamicRouterPort_en_set(unit, port, enable);
}

yt_ret_t  yt_mld_forbidden_dynamicRouterPort_en_get(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->mld_forbidden_dynamicRouterPort_en_get(unit, port, pEnable);
}

yt_ret_t  yt_mld_staticRouterPort_add_by_idx(yt_unit_t unit,  uint8_t idx, yt_vlan_t vid, yt_port_mask_t portMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(YT_MLD_STATIC_ROUTER_GROUP_NUM <= idx, CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid || YT_VLAN_ID_MIN == vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, portMask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->mld_staticRouterPort_add_by_idx(unit, idx, vid, portMask);
}

yt_ret_t  yt_mld_staticRouterPort_get(yt_unit_t unit,  uint8_t idx, yt_vlan_t *pVid, yt_port_mask_t *pPortMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(YT_MLD_STATIC_ROUTER_GROUP_NUM <= idx, CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pVid), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pPortMask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->mld_staticRouterPort_get(unit, idx, pVid, pPortMask);
}

yt_ret_t  yt_mld_staticRouterPort_del(yt_unit_t unit,  uint8_t idx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(YT_MLD_STATIC_ROUTER_GROUP_NUM <= idx, CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->mld_staticRouterPort_del(unit, idx);
}

yt_ret_t  yt_mld_fast_leave_en_set(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    if(0 == isLag)
    {
        CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    }
    else
    {
        CMM_PARAM_CHK((YT_LAG_NUM_MAX <= port), CMM_ERR_PORT);
    }
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->mld_fast_leave_en_set(unit, port, isLag, enable);
}

yt_ret_t  yt_mld_fast_leave_en_get(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    if(0 == isLag)
    {
        CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    }
    else
    {
        CMM_PARAM_CHK((YT_LAG_NUM_MAX <= port), CMM_ERR_PORT);
    }
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->mld_fast_leave_en_get(unit, port, isLag, pEnable);
}

yt_ret_t  yt_mld_report_leave_fwd_ctrl_set(yt_unit_t unit, yt_mld_version_t version, yt_igmp_mld_fwd_ports_t fwdCtrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MLD_V2 < version || YT_MLD_V1 > version), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_IGMP_MLD_ROUTER_PORTS < fwdCtrl || YT_IGMP_MLD_FLOOD > fwdCtrl), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->mld_report_leave_fwd_ctrl_set(unit, version, fwdCtrl);
}

yt_ret_t  yt_mld_report_leave_fwd_ctrl_get(yt_unit_t unit, yt_mld_version_t version, yt_igmp_mld_fwd_ports_t *pFwdCtrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MLD_V2 < version || YT_MLD_V1 > version), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pFwdCtrl), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->mld_report_leave_fwd_ctrl_get(unit, version, pFwdCtrl);
}

yt_ret_t  yt_mld_ctrlPkt_act_set(yt_unit_t unit,  yt_port_t port, yt_act_type_t actType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((ACT_TYPE_TRAP < actType || ACT_TYPE_FWD > actType), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->mld_ctrlPkt_act_set(unit, port, actType);
}

yt_ret_t  yt_mld_ctrlPkt_act_get(yt_unit_t unit,  yt_port_t port, yt_act_type_t *pActType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pActType), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->mld_ctrlPkt_act_get(unit, port, pActType);
}

yt_ret_t  yt_mld_v1_ctrlPkt_act_set(yt_unit_t unit,  yt_mld_v1_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t actType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MLD_V1_LEAVE < ctrlPkt || YT_MLD_V1_QUERY > ctrlPkt), CMM_ERR_INPUT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((ACT_TYPE_TRAP < actType || ACT_TYPE_FWD > actType), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->mld_v1_ctrlPkt_act_set(unit, ctrlPkt, port, actType);
}

yt_ret_t  yt_mld_v1_ctrlPkt_act_get(yt_unit_t unit,  yt_mld_v1_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t *pActType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MLD_V1_LEAVE < ctrlPkt || YT_MLD_V1_QUERY > ctrlPkt), CMM_ERR_INPUT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pActType), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->mld_v1_ctrlPkt_act_get(unit, ctrlPkt, port, pActType);
}

yt_ret_t  yt_mld_v2_ctrlPkt_act_set(yt_unit_t unit,  yt_mld_v2_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t actType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MLD_V2_REPORT < ctrlPkt || YT_MLD_V2_QUERY > ctrlPkt), CMM_ERR_INPUT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((ACT_TYPE_TRAP < actType || ACT_TYPE_FWD > actType), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->mld_v2_ctrlPkt_act_set(unit, ctrlPkt, port, actType);
}

yt_ret_t  yt_mld_v2_ctrlPkt_act_get(yt_unit_t unit,  yt_mld_v2_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t *pActType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MLD_V2_REPORT < ctrlPkt || YT_MLD_V2_QUERY > ctrlPkt), CMM_ERR_INPUT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pActType), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->mld_v2_ctrlPkt_act_get(unit, ctrlPkt, port, pActType);
}

yt_ret_t  yt_mld_v1_port_learn_en_set(yt_unit_t unit,  yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->mld_v1_port_learn_en_set(unit, port, enable);
}

yt_ret_t  yt_mld_v1_port_learn_en_get(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->mld_v1_port_learn_en_get(unit, port, pEnable);
}

yt_ret_t  yt_mld_v2_port_learn_en_set(yt_unit_t unit,  yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->mld_v2_port_learn_en_set(unit, port, enable);
}

yt_ret_t  yt_mld_v2_port_learn_en_get(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->mld_v2_port_learn_en_get(unit, port, pEnable);
}

yt_ret_t  yt_mld_routerPort_dataFwd_en_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->mld_routerPort_dataFwd_en_set(unit, enable);
}

yt_ret_t  yt_mld_routerPort_dataFwd_en_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->mld_routerPort_dataFwd_en_get(unit, pEnable);
}

yt_ret_t  yt_igmp_mld_system_learnlimit_set(yt_unit_t unit, yt_enable_t enable, uint16_t maxNum)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);
    CMM_PARAM_CHK(YT_IGMP_MLD_MULTICAST_GROUP_NUM <= maxNum, CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->igmp_mld_system_learnlimit_set(unit, enable, maxNum);
}

yt_ret_t  yt_igmp_mld_system_learnlimit_get(yt_unit_t unit, yt_enable_t *pEnable, uint16_t *pMaxNum)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_system_learnlimit_get(unit, pEnable, pMaxNum);
}

yt_ret_t  yt_igmp_mld_system_learnCnt_get(yt_unit_t unit, uint16_t *pCnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pCnt), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_system_learnCnt_get(unit, pCnt);
}

yt_ret_t  yt_igmp_mld_port_learnlimit_set(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t enable, uint16_t maxNum)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    if(0 == isLag)
    {
        CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    }
    else
    {
        CMM_PARAM_CHK((YT_LAG_NUM_MAX <= port), CMM_ERR_PORT);
    }
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);
    CMM_PARAM_CHK(YT_IGMP_MLD_MULTICAST_PER_PORT_GROUP_NUM <= maxNum, CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->igmp_mld_port_learnlimit_set(unit, port, isLag, enable, maxNum);
}

yt_ret_t  yt_igmp_mld_port_learnlimit_get(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t *pEnable, uint16_t *pMaxNum)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    if(0 == isLag)
    {
        CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    }
    else
    {
        CMM_PARAM_CHK((YT_LAG_NUM_MAX <= port), CMM_ERR_PORT);
    }
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pMaxNum), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_port_learnlimit_get(unit, port, isLag, pEnable, pMaxNum);
}

yt_ret_t  yt_igmp_mld_port_learnCnt_get(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, uint16_t *pCnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    if(0 == isLag)
    {
        CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    }
    else
    {
        CMM_PARAM_CHK((YT_LAG_NUM_MAX <= port), CMM_ERR_PORT);
    }
    CMM_PARAM_CHK((NULL == pCnt), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_port_learnCnt_get(unit, port, isLag, pCnt);
}

yt_ret_t  yt_igmp_mld_memberports_aging_time_set(yt_unit_t unit,  uint32_t sec)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(YT_IGMP_MLD_MEMBER_PORT_AGING_MAXTIME <= sec, CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->igmp_mld_memberports_aging_time_set(unit, sec);
}

yt_ret_t  yt_igmp_mld_memberports_aging_time_get(yt_unit_t unit,  uint32_t *pSec)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pSec), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_memberports_aging_time_get(unit, pSec);
}

yt_ret_t  yt_igmp_mld_memberport_time_get(yt_unit_t unit,  uint8_t idx, yt_port_t port, uint32_t *pTime)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pTime), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_memberport_time_get(unit, idx, port, pTime);
}

yt_ret_t  yt_igmp_mld_leaveports_aging_time_set(yt_unit_t unit,  yt_igmp_mld_leave_aging_t agingEnum)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_IGMP_MLD_10S < agingEnum || YT_IGMP_MLD_2S > agingEnum), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->igmp_mld_leaveports_aging_time_set(unit, agingEnum);
}

yt_ret_t  yt_igmp_mld_leaveports_aging_time_get(yt_unit_t unit,  yt_igmp_mld_leave_aging_t *pAgingEnum)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pAgingEnum), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->igmp_mld_leaveports_aging_time_get(unit, pAgingEnum);
}

#endif

