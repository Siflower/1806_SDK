/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_interrupt.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_INTERRUPT_H__
#define __FAL_TIGER_INTERRUPT_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "fal_cmm.h"

extern yt_ret_t fal_tiger_int_polarity_set(yt_unit_t unit, yt_int_polarity_t type);

extern yt_ret_t fal_tiger_int_polarity_get(yt_unit_t unit, yt_int_polarity_t *pType);

extern yt_ret_t fal_tiger_int_control_set(yt_unit_t unit, yt_int_type_t type, yt_enable_t enable);

extern yt_ret_t fal_tiger_int_control_get(yt_unit_t unit, yt_int_type_t type, yt_enable_t *pEnable);

extern yt_ret_t fal_tiger_int_status_get(yt_unit_t unit, yt_intr_status_t *pIntStatus);

extern yt_ret_t fal_tiger_int_typeStatus_get(yt_unit_t unit, yt_int_type_t type, yt_intr_status_t intStatus, yt_bool_t *pActive);

extern yt_ret_t fal_tiger_int_port_int_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);

extern yt_ret_t fal_tiger_int_port_int_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

extern yt_ret_t fal_tiger_int_allPortIntStatus_get(yt_unit_t unit, yt_intr_status_t *pIntStatus);

extern yt_ret_t fal_tiger_int_portIntStatus_get(yt_unit_t unit, yt_port_t port, yt_intr_status_t intStatus, yt_bool_t *pActive);

extern yt_ret_t fal_tiger_int_wol_signalOutputType_set(yt_unit_t unit, yt_int_wol_outputType_t outputType);

extern yt_ret_t fal_tiger_int_wol_signalOutputType_get(yt_unit_t unit, yt_int_wol_outputType_t *pOutputType);

extern yt_ret_t fal_tiger_int_wol_port_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);

extern yt_ret_t fal_tiger_int_wol_port_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

extern yt_ret_t fal_tiger_int_wol_allPortIntStatus_get(yt_unit_t unit, yt_intr_status_t *pIntStatus);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
