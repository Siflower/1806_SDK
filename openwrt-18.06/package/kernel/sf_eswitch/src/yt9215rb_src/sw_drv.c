/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#include "sw_drv.h"
#if defined(SWITCH_SERIES_TIGER)
#include "sw_yt921x.h"
#endif

#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
#include "sw_yt923x.h"
#endif

const yt_switch_drv_t * const gpSwDrvList[] =
{
#ifdef SWITCH_SERIES_TIGER
    [YT_SW_MODEL_9215] = &yt9215_drv,
    [YT_SW_MODEL_9218] = &yt9218_drv,
    [YT_SW_MODEL_9218N] = &yt9218_drv,
    [YT_SW_MODEL_9213] = &yt9215_drv,
    [YT_SW_MODEL_9214] = &yt9215_drv,
#endif
#ifdef SWITCH_SERIES_SHARK
    [YT_SW_MODEL_9232] = &yt9232_drv,
    [YT_SW_MODEL_9232_26] = &yt9232_drv,
    [YT_SW_MODEL_9230] = &yt9232_drv,
    [YT_SW_MODEL_9231] = &yt9232_drv,
    [YT_SW_MODEL_9231_6] = &yt9232_drv,
#endif
#ifdef SWITCH_SERIES_WHALE
    [YT_SW_MODEL_9228] = &yt9232_drv,
    [YT_SW_MODEL_9224] = &yt9232_drv,
#endif
    [YT_SW_MODEL_END] = NULL,
};
