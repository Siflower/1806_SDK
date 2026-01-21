/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_rma.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_rma.h"
#include "fal_dispatch.h"

yt_ret_t  yt_rma_action_set(yt_unit_t unit, yt_rma_da_t  da, yt_act_type_t action)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((da > YT_RMA_DA_MRP_2F || da < YT_RMA_DA_BRG_GROUP), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rma_action_set(unit, da, action);
}

yt_ret_t  yt_rma_action_get(yt_unit_t unit, yt_rma_da_t  da, yt_act_type_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((da > YT_RMA_DA_MRP_2F || da < YT_RMA_DA_BRG_GROUP), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rma_action_get(unit, da, pAction);
}

yt_ret_t  yt_rma_cpu_code_set(yt_unit_t unit, yt_rma_da_t  da, yt_rma_cpu_code_t cpuCode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((cpuCode > YT_RMA_CPU_CODE_RESV9 || cpuCode < YT_RMA_CPU_CODE_RESV0), CMM_ERR_INPUT);
    CMM_PARAM_CHK((da > YT_RMA_DA_MRP_2F || da < YT_RMA_DA_BRG_GROUP), CMM_ERR_INPUT);
   
    return YT_DISPATCH(unit)->rma_cpu_code_set(unit, da, cpuCode);
}

yt_ret_t  yt_rma_cpu_code_get(yt_unit_t unit, yt_rma_da_t  da, yt_rma_cpu_code_t *pCpuCode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((da > YT_RMA_DA_MRP_2F || da < YT_RMA_DA_BRG_GROUP), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pCpuCode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rma_cpu_code_get(unit, da, pCpuCode);
}

yt_ret_t yt_rma_bypass_set(yt_unit_t unit, yt_rma_da_t  da, yt_rma_bypass_t rmaBypass)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((da > YT_RMA_DA_MRP_2F || da < YT_RMA_DA_BRG_GROUP), CMM_ERR_INPUT);
   
    return YT_DISPATCH(unit)->rma_bypass_set(unit, da, rmaBypass);
}

yt_ret_t yt_rma_bypass_get(yt_unit_t unit, yt_rma_da_t  da, yt_rma_bypass_t *pRmaBypass)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((da > YT_RMA_DA_MRP_2F || da < YT_RMA_DA_BRG_GROUP), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pRmaBypass), CMM_ERR_NULL_POINT);
   
    return YT_DISPATCH(unit)->rma_bypass_get(unit, da, pRmaBypass);
}

yt_ret_t  yt_rma_sa_learn_dis_set(yt_unit_t unit, yt_rma_da_t da, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((da > YT_RMA_DA_MRP_2F || da < YT_RMA_DA_BRG_GROUP), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rma_sa_learn_dis_set(unit, da, enable);
}

yt_ret_t  yt_rma_sa_learn_dis_get(yt_unit_t unit, yt_rma_da_t da, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((da > YT_RMA_DA_MRP_2F || da < YT_RMA_DA_BRG_GROUP), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rma_sa_learn_dis_get(unit, da, pEnable);
}