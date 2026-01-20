/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#ifndef __HAL_CTRL_H__
#define __HAL_CTRL_H__

#include "yt_types.h"
#include "yt_cmm.h"
#include "sw_drv.h"
#include "phy_drv.h"

typedef struct yt_hal_ctrl_s
{
    const yt_switch_drv_t    *pHalSwDrv;
}yt_hal_ctrl_t;

extern yt_hal_ctrl_t gHalCtrl[YT_MAX_UNIT];

#define HALCTRL(unit)   gHalCtrl[unit]
#define HALSWDRV(unit)  gHalCtrl[unit].pHalSwDrv
#define HALSWDRV_FUNC(unit)  gHalCtrl[unit].pHalSwDrv->pDrvFunc

extern uint32_t hal_ctrl_init(void);

#endif
