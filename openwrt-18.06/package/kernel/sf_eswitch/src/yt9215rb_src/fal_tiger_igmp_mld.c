/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_igmp_mld.c
*
* @brief x
*
********************************************************************************
*/
/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_tiger_igmp_mld.h"
#include "fal_tiger_l2.h"

#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"

yt_ret_t  fal_tiger_igmp_opmode_set(yt_unit_t unit,  yt_igmp_mld_op_mode_t mode)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_global_ctrl_t l2_igmp_global_ctrl;
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);
    HAL_FIELD_SET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_IGMP_OPf, &l2_igmp_global_ctrl, mode);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_opmode_get(yt_unit_t unit,  yt_igmp_mld_op_mode_t *pMode)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_global_ctrl_t l2_igmp_global_ctrl;
    uint32 mode;
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);
    HAL_FIELD_GET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_IGMP_OPf, &l2_igmp_global_ctrl, &mode);
    *pMode = mode;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_mld_opmode_set(yt_unit_t unit,  yt_igmp_mld_op_mode_t mode)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_global_ctrl_t l2_igmp_global_ctrl;
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);
    HAL_FIELD_SET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_MLD_OPf, &l2_igmp_global_ctrl, mode);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);
    
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_mld_opmode_get(yt_unit_t unit,  yt_igmp_mld_op_mode_t *pMode)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_global_ctrl_t l2_igmp_global_ctrl;
    uint32_t mode;
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);
    HAL_FIELD_GET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_MLD_OPf, &l2_igmp_global_ctrl, &mode);
    *pMode = mode;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_port_report_allow_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_per_port_ctrl_t  l2_igmp_per_port_ctrl;
    yt_macid_t macid;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_PER_PORT_CTRLm, macid, sizeof(l2_igmp_per_port_ctrl_t), &l2_igmp_per_port_ctrl), ret);
    HAL_FIELD_SET(L2_IGMP_PER_PORT_CTRLm, L2_IGMP_PER_PORT_CTRL_ALLOW_REPORT_PKTf, &l2_igmp_per_port_ctrl, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_IGMP_PER_PORT_CTRLm, macid, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_per_port_ctrl), ret);

    fal_tiger_l2_igmp_bypass_unknown_mcast_filter_set(unit, enable);
   
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_port_report_allow_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_per_port_ctrl_t  l2_igmp_per_port_ctrl;
    yt_macid_t macid;
    uint32 enable;

    macid = CAL_YTP_TO_MAC(unit,port);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_PER_PORT_CTRLm, macid, sizeof(l2_igmp_per_port_ctrl_t), &l2_igmp_per_port_ctrl), ret);
    HAL_FIELD_GET(L2_IGMP_PER_PORT_CTRLm, L2_IGMP_PER_PORT_CTRL_ALLOW_REPORT_PKTf, &l2_igmp_per_port_ctrl, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_port_leave_allow_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_per_port_ctrl_t  l2_igmp_per_port_ctrl;
    yt_macid_t macid;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_PER_PORT_CTRLm, macid, sizeof(l2_igmp_per_port_ctrl_t), &l2_igmp_per_port_ctrl), ret);
    HAL_FIELD_SET(L2_IGMP_PER_PORT_CTRLm, L2_IGMP_PER_PORT_CTRL_ALLOW_LEAVE_PKTf, &l2_igmp_per_port_ctrl, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_IGMP_PER_PORT_CTRLm, macid, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_per_port_ctrl), ret);

     fal_tiger_l2_igmp_bypass_unknown_mcast_filter_set(unit, enable);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_port_leave_allow_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_per_port_ctrl_t  l2_igmp_per_port_ctrl;
    yt_macid_t macid;
    uint32_t enable;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_PER_PORT_CTRLm, macid, sizeof(l2_igmp_per_port_ctrl_t), &l2_igmp_per_port_ctrl), ret);
    HAL_FIELD_GET(L2_IGMP_PER_PORT_CTRLm, L2_IGMP_PER_PORT_CTRL_ALLOW_LEAVE_PKTf, &l2_igmp_per_port_ctrl, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;
   
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_port_query_allow_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_per_port_ctrl_t  l2_igmp_per_port_ctrl;
    yt_macid_t macid;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_PER_PORT_CTRLm, macid, sizeof(l2_igmp_per_port_ctrl_t), &l2_igmp_per_port_ctrl), ret);
    HAL_FIELD_SET(L2_IGMP_PER_PORT_CTRLm, L2_IGMP_PER_PORT_CTRL_ALLOW_QUERY_PKTf, &l2_igmp_per_port_ctrl, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_IGMP_PER_PORT_CTRLm, macid, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_per_port_ctrl), ret);

    fal_tiger_l2_igmp_bypass_unknown_mcast_filter_set(unit, enable);
     
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_port_query_allow_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_per_port_ctrl_t  l2_igmp_per_port_ctrl;
    yt_macid_t macid;
    uint32_t enable;
    
    macid = CAL_YTP_TO_MAC(unit,port);
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_PER_PORT_CTRLm, macid, sizeof(l2_igmp_per_port_ctrl_t), &l2_igmp_per_port_ctrl), ret);
    HAL_FIELD_GET(L2_IGMP_PER_PORT_CTRLm, L2_IGMP_PER_PORT_CTRL_ALLOW_QUERY_PKTf, &l2_igmp_per_port_ctrl, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

   
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_fastleave_set(yt_unit_t unit, yt_enable_t enable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_global_ctrl_t l2_igmp_global_ctrl;
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);
    HAL_FIELD_SET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_FAST_LEAVE_ENf, &l2_igmp_global_ctrl, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);
    
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_fastleave_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_global_ctrl_t l2_igmp_global_ctrl;
    uint32_t enable;
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);
    HAL_FIELD_GET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_FAST_LEAVE_ENf, &l2_igmp_global_ctrl, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_learnlimit_en_set(yt_unit_t unit, yt_enable_t enable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_learn_ctrl_t l2_learn_igmp_ctrl;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_LEARN_CTRLm, 0, sizeof(l2_igmp_learn_ctrl_t), &l2_learn_igmp_ctrl), ret);
    HAL_FIELD_SET(L2_IGMP_LEARN_CTRLm, L2_IGMP_LEARN_CTRL_LIMIT_ENf, &l2_learn_igmp_ctrl, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_IGMP_LEARN_CTRLm, 0, sizeof(l2_igmp_learn_ctrl_t), &l2_learn_igmp_ctrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_learnlimit_en_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_learn_ctrl_t l2_learn_igmp_ctrl;
    uint32_t enable;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_LEARN_CTRLm, 0, sizeof(l2_igmp_learn_ctrl_t), &l2_learn_igmp_ctrl), ret);
    HAL_FIELD_GET(L2_IGMP_LEARN_CTRLm, L2_IGMP_LEARN_CTRL_LIMIT_ENf, &l2_learn_igmp_ctrl, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_learnlimit_maxgroup_set(yt_unit_t unit, uint32_t  maxgroup)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_learn_ctrl_t l2_learn_igmp_ctrl;

    if(maxgroup > CAL_L2_FDB_NUM_MAX(unit))
        return CMM_ERR_INPUT;
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_LEARN_CTRLm, 0, sizeof(l2_igmp_learn_ctrl_t), &l2_learn_igmp_ctrl), ret);
    HAL_FIELD_SET(L2_IGMP_LEARN_CTRLm, L2_IGMP_LEARN_CTRL_MAX_GROUP_NUMf, &l2_learn_igmp_ctrl, maxgroup);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_IGMP_LEARN_CTRLm, 0, sizeof(l2_igmp_learn_ctrl_t), &l2_learn_igmp_ctrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_learnlimit_maxgroup_get(yt_unit_t unit, uint32_t  *pmaxgroup)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_learn_ctrl_t l2_learn_igmp_ctrl;
    uint32_t maxgroup;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_LEARN_CTRLm, 0, sizeof(l2_igmp_learn_ctrl_t), &l2_learn_igmp_ctrl), ret);
    HAL_FIELD_GET(L2_IGMP_LEARN_CTRLm, L2_IGMP_LEARN_CTRL_MAX_GROUP_NUMf, &l2_learn_igmp_ctrl, &maxgroup);
    *pmaxgroup = maxgroup;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_fwd_routerport_only_set(yt_unit_t unit, yt_enable_t enable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_global_ctrl_t l2_igmp_global_ctrl;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);
    HAL_FIELD_SET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_IGMP_FORCE_ROUTER_DSTf, &l2_igmp_global_ctrl, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_fwd_routerport_only_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_global_ctrl_t l2_igmp_global_ctrl;
    uint32_t enable;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);
    HAL_FIELD_GET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_IGMP_FORCE_ROUTER_DSTf, &l2_igmp_global_ctrl, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_fwd_routerport_primary_set(yt_unit_t unit, yt_enable_t enable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_global_ctrl_t l2_igmp_global_ctrl;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);
    HAL_FIELD_SET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_REPORT_LEAVE_ROUTER_FWD_CTRLf, &l2_igmp_global_ctrl, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_fwd_routerport_primary_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_global_ctrl_t l2_igmp_global_ctrl;
    uint32_t enable;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);
    HAL_FIELD_GET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_REPORT_LEAVE_ROUTER_FWD_CTRLf, &l2_igmp_global_ctrl, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;
    
    return CMM_ERR_OK;
}

 yt_ret_t fal_tiger_igmp_mld_bypass_grouprange_set(yt_unit_t unit, yt_igmp_mld_learn_bypass_range_t bypass)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_global_ctrl_t l2_igmp_global_ctrl;
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);
    HAL_FIELD_SET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_BYPASS_224_0_0_Xf, &l2_igmp_global_ctrl, bypass.bypass_224_0_0_x_en);
    HAL_FIELD_SET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_BYPASS_224_0_1_Xf, &l2_igmp_global_ctrl, bypass.bypass_224_0_1_x_en);
    HAL_FIELD_SET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_BYPASS_239_255_255_Xf, &l2_igmp_global_ctrl, bypass.bypass_239_255_255_x_en);
    HAL_FIELD_SET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_BYPASS_IPV6_00XXf, &l2_igmp_global_ctrl, bypass.bypass_ipv6_00xx_en);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_igmp_mld_bypass_grouprange_get(yt_unit_t unit, yt_igmp_mld_learn_bypass_range_t *pbypass)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_global_ctrl_t l2_igmp_global_ctrl;
    uint32_t enable;

    CMM_PARAM_CHK((NULL == pbypass), CMM_ERR_NULL_POINT);
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_GLOBAL_CTRLm, 0, sizeof(l2_igmp_global_ctrl_t), &l2_igmp_global_ctrl), ret);
    HAL_FIELD_GET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_BYPASS_224_0_0_Xf, &l2_igmp_global_ctrl, &enable);
    pbypass->bypass_224_0_0_x_en = enable;
    HAL_FIELD_GET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_BYPASS_224_0_1_Xf, &l2_igmp_global_ctrl, &enable);
    pbypass->bypass_224_0_1_x_en = enable;
    HAL_FIELD_GET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_BYPASS_239_255_255_Xf, &l2_igmp_global_ctrl, &enable);
    pbypass->bypass_239_255_255_x_en = enable;
    HAL_FIELD_GET(L2_IGMP_GLOBAL_CTRLm, L2_IGMP_GLOBAL_CTRL_BYPASS_IPV6_00XXf, &l2_igmp_global_ctrl, &enable);
    pbypass->bypass_ipv6_00xx_en = enable;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_dynamic_routerport_get(yt_unit_t unit, yt_igmp_mld_router_port_t *prouter_port)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_dynamic_router_port_dummy_t l2_igmp_dynamic_router_port;
    uint32_t port0;
    uint32_t port1;

    CMM_PARAM_CHK((NULL == prouter_port), CMM_ERR_NULL_POINT);
   
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit,L2_IGMP_DYNAMIC_ROUTER_PORT_DUMMYm, 0, sizeof(l2_igmp_dynamic_router_port), &l2_igmp_dynamic_router_port), ret);
    
    HAL_FIELD_GET(L2_IGMP_DYNAMIC_ROUTER_PORT_DUMMYm, L2_IGMP_DYNAMIC_ROUTER_PORT_DUMMY_PORT0f, &l2_igmp_dynamic_router_port, &port0);
    HAL_FIELD_GET(L2_IGMP_DYNAMIC_ROUTER_PORT_DUMMYm, L2_IGMP_DYNAMIC_ROUTER_PORT_DUMMY_PORT1f, &l2_igmp_dynamic_router_port, &port1);
    if((PORT_NUM_INVALID == port0) && (PORT_NUM_INVALID == port1))
    {
        return CMM_ERR_ENTRY_NOT_FOUND;
    }

    prouter_port->valid[0]  = ((PORT_NUM_INVALID != port0) ? TRUE: FALSE);
    prouter_port->valid[1]  = ((PORT_NUM_INVALID != port1) ? TRUE: FALSE);
    prouter_port->port[0]   = port0;
    prouter_port->port[1]   = port1;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_igmp_mld_dynamic_routerport_allow_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_dynamic_router_port_ctrl_t igmp_dynamic_router_port_ctrl;
    yt_macid_t macid;
    uint32_t port_mask;
    
    macid = CAL_YTP_TO_MAC(unit,port);
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit,L2_IGMP_DYNAMIC_ROUTER_PORT_CTRLm, 0, sizeof(l2_igmp_dynamic_router_port_ctrl_t), &igmp_dynamic_router_port_ctrl), ret);
    HAL_FIELD_GET(L2_IGMP_DYNAMIC_ROUTER_PORT_CTRLm, L2_IGMP_DYNAMIC_ROUTER_PORT_CTRL_ALLOW_DYNAMIC_PORT_MASKf, &igmp_dynamic_router_port_ctrl, &port_mask);
    if(enable == YT_ENABLE)
    {
        port_mask |= (1<<macid);
    }
    else
    {
        port_mask &= ~(1<<macid);
    }
    HAL_FIELD_SET(L2_IGMP_DYNAMIC_ROUTER_PORT_CTRLm, L2_IGMP_DYNAMIC_ROUTER_PORT_CTRL_ALLOW_DYNAMIC_PORT_MASKf, &igmp_dynamic_router_port_ctrl, port_mask);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit,L2_IGMP_DYNAMIC_ROUTER_PORT_CTRLm, 0, sizeof(l2_igmp_dynamic_router_port_ctrl_t), &igmp_dynamic_router_port_ctrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_igmp_mld_dynamic_routerport_allow_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_dynamic_router_port_ctrl_t igmp_dynamic_router_port_ctrl;
    yt_macid_t macid;
    uint32_t port_mask;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit,L2_IGMP_DYNAMIC_ROUTER_PORT_CTRLm, 0, sizeof(l2_igmp_dynamic_router_port_ctrl_t), &igmp_dynamic_router_port_ctrl), ret);
    HAL_FIELD_GET(L2_IGMP_DYNAMIC_ROUTER_PORT_CTRLm, L2_IGMP_DYNAMIC_ROUTER_PORT_CTRL_ALLOW_DYNAMIC_PORT_MASKf, &igmp_dynamic_router_port_ctrl, &port_mask);
    *pEnable = IS_BIT_SET(port_mask, macid) ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_igmp_mld_dynamic_routerport_agingtime_set(yt_unit_t unit, uint32_t sec)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_dynamic_router_port0_timer_t router_port_timer;
    l2_igmp_dynamic_router_port_ctrl_t l2_igmp_dynamic_router_port_ctrl;
    igmp_router_port_aging_ctrl_t      igmp_router_port_aging_ctrl;
    uint32_t interval = 0;

    osal_memset(&router_port_timer, sizeof(router_port_timer), 0, sizeof(router_port_timer));
    osal_memset(&l2_igmp_dynamic_router_port_ctrl, sizeof(l2_igmp_dynamic_router_port_ctrl), 0, sizeof(l2_igmp_dynamic_router_port_ctrl));
    osal_memset(&igmp_router_port_aging_ctrl, sizeof(igmp_router_port_aging_ctrl), 0, sizeof(igmp_router_port_aging_ctrl));

    if(sec > ROUTER_PORT_AGING_CNT*255)/*timer val * (2^8-1)*/
        return CMM_ERR_INPUT;
    
    interval = sec/ROUTER_PORT_AGING_CNT;
    interval = interval == 0 ? 1 : interval;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit,L2_IGMP_DYNAMIC_ROUTER_PORT_CTRLm, 0, sizeof(l2_igmp_dynamic_router_port_t), &l2_igmp_dynamic_router_port_ctrl), ret);
    HAL_FIELD_SET(L2_IGMP_DYNAMIC_ROUTER_PORT_CTRLm, L2_IGMP_DYNAMIC_ROUTER_PORT_CTRL_TIMER_VALf, &l2_igmp_dynamic_router_port_ctrl, ROUTER_PORT_AGING_CNT);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit,L2_IGMP_DYNAMIC_ROUTER_PORT_CTRLm, 0, sizeof(l2_igmp_dynamic_router_port_t), &l2_igmp_dynamic_router_port_ctrl), ret);

    HAL_FIELD_SET(IGMP_ROUTER_PORT_AGING_CTRLm, IGMP_ROUTER_PORT_AGING_CTRL_AGING_INTERVALf, &igmp_router_port_aging_ctrl, interval);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit,IGMP_ROUTER_PORT_AGING_CTRLm, 0, sizeof(igmp_router_port_aging_ctrl_t), &igmp_router_port_aging_ctrl), ret);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit,L2_IGMP_DYNAMIC_ROUTER_PORT0_TIMERm, 0, sizeof(l2_igmp_dynamic_router_port0_timer_t), &router_port_timer), ret);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit,L2_IGMP_DYNAMIC_ROUTER_PORT1_TIMERm, 0, sizeof(l2_igmp_dynamic_router_port0_timer_t), &router_port_timer), ret);
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_igmp_mld_dynamic_routerport_agingtime_get(yt_unit_t unit, uint32_t *psec)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_igmp_dynamic_router_port_ctrl_t l2_igmp_dynamic_router_port_ctrl;
    igmp_router_port_aging_ctrl_t      igmp_router_port_aging_ctrl;
    uint32_t timer_val;
    uint32_t interval;

    osal_memset(&l2_igmp_dynamic_router_port_ctrl, sizeof(l2_igmp_dynamic_router_port_ctrl), 0, sizeof(l2_igmp_dynamic_router_port_ctrl));
    osal_memset(&igmp_router_port_aging_ctrl, sizeof(igmp_router_port_aging_ctrl), 0, sizeof(igmp_router_port_aging_ctrl));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit,L2_IGMP_DYNAMIC_ROUTER_PORT_CTRLm, 0, sizeof(l2_igmp_dynamic_router_port_t), &l2_igmp_dynamic_router_port_ctrl), ret);
    HAL_FIELD_GET(L2_IGMP_DYNAMIC_ROUTER_PORT_CTRLm, L2_IGMP_DYNAMIC_ROUTER_PORT_CTRL_TIMER_VALf, &l2_igmp_dynamic_router_port_ctrl, &timer_val);  
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit,IGMP_ROUTER_PORT_AGING_CTRLm, 0, sizeof(igmp_router_port_aging_ctrl_t), &igmp_router_port_aging_ctrl), ret);
    HAL_FIELD_GET(IGMP_ROUTER_PORT_AGING_CTRLm, IGMP_ROUTER_PORT_AGING_CTRL_AGING_INTERVALf, &igmp_router_port_aging_ctrl, &interval);
    *psec = timer_val * interval;
    
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_static_routerport_set(yt_unit_t unit, yt_port_mask_t port_mask)
{
    cmm_err_t ret = CMM_ERR_OK;
    l2_igmp_static_router_port_mask_t router_port_mask;
    yt_port_mask_t macmask;

    CAL_YTPLIST_TO_MLIST(unit,port_mask, macmask);
    
    HAL_FIELD_SET(L2_IGMP_STATIC_ROUTER_PORT_MASKm, L2_IGMP_STATIC_ROUTER_PORT_MASK_PORT_MASKf, &router_port_mask, macmask.portbits[0]);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_IGMP_STATIC_ROUTER_PORT_MASKm, 0, sizeof(l2_igmp_static_router_port_mask_t), &router_port_mask), ret);
    
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_static_routerport_get(yt_unit_t unit, yt_port_mask_t *pport_mask)
{
    cmm_err_t ret = CMM_ERR_OK;
    l2_igmp_static_router_port_mask_t router_port_mask;
    yt_port_mask_t macmask;
    uint32_t port_mask;
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_STATIC_ROUTER_PORT_MASKm, 0, sizeof(l2_igmp_static_router_port_mask_t), &router_port_mask), ret);
    HAL_FIELD_GET(L2_IGMP_STATIC_ROUTER_PORT_MASKm, L2_IGMP_STATIC_ROUTER_PORT_MASK_PORT_MASKf, &router_port_mask, &port_mask);
    
    macmask.portbits[0] = port_mask;
    CAL_MLIST_TO_YTPLIST(unit, macmask, (*pport_mask));
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_igmp_mld_bypass_portIsolation_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    l2_igmp_per_port_ctrl_t  entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_PER_PORT_CTRLm, macid, sizeof(l2_igmp_per_port_ctrl_t), &entry), ret);
    HAL_FIELD_SET(L2_IGMP_PER_PORT_CTRLm, L2_IGMP_PER_PORT_CTRL_PORT_ISOLATE_LEAKYf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_IGMP_PER_PORT_CTRLm, macid, sizeof(l2_igmp_per_port_ctrl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_igmp_mld_bypass_portIsolation_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    l2_igmp_per_port_ctrl_t  entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid;
    uint32_t enable;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_PER_PORT_CTRLm, macid, sizeof(l2_igmp_per_port_ctrl_t), &entry), ret);
    HAL_FIELD_GET(L2_IGMP_PER_PORT_CTRLm, L2_IGMP_PER_PORT_CTRL_PORT_ISOLATE_LEAKYf, &entry, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_igmp_mld_ipmc_bypass_portIsolation_set(yt_unit_t unit, yt_enable_t enable)
{
    ipmc_leaky_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, IPMC_LEAKY_CTRLm, 0, sizeof(ipmc_leaky_ctrl_t), &entry), ret);
    HAL_FIELD_SET(IPMC_LEAKY_CTRLm, IPMC_LEAKY_CTRL_IPMC_PORT_ISOLATION_LEAKYf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, IPMC_LEAKY_CTRLm, 0, sizeof(ipmc_leaky_ctrl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_igmp_mld_ipmc_bypass_portIsolation_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    ipmc_leaky_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t enable;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, IPMC_LEAKY_CTRLm, 0, sizeof(ipmc_leaky_ctrl_t), &entry), ret);
    HAL_FIELD_GET(IPMC_LEAKY_CTRLm, IPMC_LEAKY_CTRL_IPMC_PORT_ISOLATION_LEAKYf, &entry, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_bypass_vlanIngrfilter_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    l2_vlan_ingress_filter_en_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);
    uint32_t port_mask;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_INGRESS_FILTER_ENm, 0, sizeof(l2_vlan_ingress_filter_en_t), &entry), ret);
    HAL_FIELD_GET(L2_VLAN_INGRESS_FILTER_ENm, L2_VLAN_INGRESS_FILTER_EN_IGMP_BYPASS_ENf, &entry, &port_mask);
    if(enable)
    {
        SET_BIT(port_mask, macid);
    }
    else
    {
        CLEAR_BIT(port_mask, macid);
    }
    HAL_FIELD_SET(L2_VLAN_INGRESS_FILTER_ENm, L2_VLAN_INGRESS_FILTER_EN_IGMP_BYPASS_ENf, &entry, port_mask);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_VLAN_INGRESS_FILTER_ENm, 0, sizeof(l2_vlan_ingress_filter_en_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_igmp_mld_bypass_vlanIngrfilter_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    l2_vlan_ingress_filter_en_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);
    uint32_t port_mask;

    *pEnable = YT_DISABLE;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_INGRESS_FILTER_ENm, 0, sizeof(l2_vlan_ingress_filter_en_t), &entry), ret);
    HAL_FIELD_GET(L2_VLAN_INGRESS_FILTER_ENm, L2_VLAN_INGRESS_FILTER_EN_IGMP_BYPASS_ENf, &entry, &port_mask);
    if (IS_BIT_SET(port_mask, macid))
    {
        *pEnable = YT_ENABLE;
    }

    return CMM_ERR_OK;
}

