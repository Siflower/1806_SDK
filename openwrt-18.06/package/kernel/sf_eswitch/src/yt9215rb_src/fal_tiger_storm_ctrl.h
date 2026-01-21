/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_storm_ctrl.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_STORM_CTRL_H
#define __FAL_TIGER_STORM_CTRL_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "fal_cmm.h"

#define STORM_DEFAULT_TIMESLOT  41
#define STORM_DEFAULT_CBS_BYTE  1000
#define STORM_DEFAULT_CBS_PACKET  0x1E8


extern yt_ret_t fal_tiger_storm_ctrl_init(yt_unit_t unit);
extern yt_ret_t fal_tiger_storm_ctrl_enable_set(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, yt_enable_t enable);
extern yt_ret_t fal_tiger_storm_ctrl_enable_get(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, yt_enable_t *pEnable);
extern yt_ret_t fal_tiger_storm_ctrl_rate_mode_set(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, yt_port_rate_mode_t rateMode);
extern yt_ret_t fal_tiger_storm_ctrl_rate_mode_get(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, yt_port_rate_mode_t *pRateMode);
extern yt_ret_t fal_tiger_storm_ctrl_rate_set(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, uint32_t rate);
extern yt_ret_t fal_tiger_storm_ctrl_rate_get(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, uint32_t *pRate);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
