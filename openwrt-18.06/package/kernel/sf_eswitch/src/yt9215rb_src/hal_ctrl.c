/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#include "hal_ctrl.h"
#include "cal_cmm.h"
#include "yt_error.h"

yt_hal_ctrl_t gHalCtrl[YT_MAX_UNIT];

uint32_t hal_ctrl_init(void)
{
    yt_unit_t unit;
    yt_switch_chip_model_t swModel;

    for(unit = 0; unit < YT_UNIT_NUM; unit++)
    {
        swModel = CAL_SWCHIP_MODEL(unit);
        if(swModel >= YT_SW_MODEL_END)
        {
            return CMM_ERR_FAIL;
        }

        gHalCtrl[unit].pHalSwDrv = gpSwDrvList[swModel];
    }

    return CMM_ERR_OK;
}

