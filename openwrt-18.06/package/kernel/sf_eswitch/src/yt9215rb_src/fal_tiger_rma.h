/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_rma.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_RMA_H
#define __FAL_TIGER_RMA_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "fal_cmm.h"


extern yt_ret_t  fal_tiger_rma_action_set(yt_unit_t unit, yt_rma_da_t  macda, yt_act_type_t action);
extern yt_ret_t  fal_tiger_rma_action_get(yt_unit_t unit, yt_rma_da_t  macda, yt_act_type_t *pAction);
extern yt_ret_t  fal_tiger_rma_cpu_code_set(yt_unit_t unit, yt_rma_da_t  macda, yt_rma_cpu_code_t cpuCode);
extern yt_ret_t  fal_tiger_rma_cpu_code_get(yt_unit_t unit, yt_rma_da_t  macda, yt_rma_cpu_code_t *pCpuCode);
extern yt_ret_t  fal_tiger_rma_bypass_set(yt_unit_t unit, yt_rma_da_t  macda, yt_rma_bypass_t rmaBypass);
extern yt_ret_t  fal_tiger_rma_bypass_get(yt_unit_t unit, yt_rma_da_t  macda, yt_rma_bypass_t *pRmaBypass);
extern yt_ret_t  fal_tiger_rma_sa_learn_dis_set(yt_unit_t unit, yt_rma_da_t macda, yt_enable_t enable);
extern yt_ret_t  fal_tiger_rma_sa_learn_dis_get(yt_unit_t unit, yt_rma_da_t macda, yt_enable_t *pEnable);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
