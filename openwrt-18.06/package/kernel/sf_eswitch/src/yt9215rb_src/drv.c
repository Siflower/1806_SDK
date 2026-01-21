/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "drv.h"
#ifdef ACC_UART
#include "uart.h"
#endif
#ifdef SW_CTRL_IOCTL
#include "sw_ctrl_ioctl.h"
#endif
#include "yt_error.h"
#include "yt_lock.h"
#include "yt_util.h"

uint32_t drv_init(yt_unit_t unit)
{
    cmm_err_t ret = CMM_ERR_OK;

    CMM_UNUSED_PARAM(unit);
    ACCESS_LOCK_INIT();
    INT_SMI_LOCK_INIT();
    EXT_SMI_LOCK_INIT();

#if defined(SW_CTRL_IOCTL)
    ret = sw_ctrl_ioctl_init();
#endif

    return ret;
}

uint32_t drv_close(void)
{    
#ifdef ACC_UART
    uart_exit();
#elif defined(SW_CTRL_IOCTL)
    sw_ctrl_ioctl_exit();
#endif

    return CMM_ERR_OK;
}
