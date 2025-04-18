/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "hal_mem.h"
#include "hal_cmm.h"
#include "hal_ctrl.h"

uint32_t hal_init(void)
{
    yt_ret_t ret = CMM_ERR_OK;

    hal_mem32_init();
    hal_table_reg_init();

    CMM_ERR_CHK(hal_ctrl_init(), ret);

    return CMM_ERR_OK;
}
