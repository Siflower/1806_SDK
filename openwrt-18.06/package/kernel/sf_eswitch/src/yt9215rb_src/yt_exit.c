/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/*
 * Include Files
 */

#include "yt_types.h"
#include "yt_error.h"
#include "yt_exit.h"
#include "hal_mem.h"
#include "yt_util.h"
#include "yt_init.h"
#include "fal_dispatch.h"

#ifdef YT_PKTGEN_EN
#include "yt_pkt_gen.h"
#endif

yt_ret_t yt_exit(uint8_t unit)
{
    cmm_err_t ret = CMM_ERR_OK;

    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_ERR_CHK(YT_DISPATCH(unit)->exit(unit),ret);

    hal_table_reg_exit();

#ifdef YT_PKTGEN_EN
    CMM_ERR_CHK(yt_pkt_gen_exit(), ret);
#endif
    cal_mgm_exit();
    yt_device_close(unit);

    return CMM_ERR_OK;
}

#if defined(LINUX_KERNEL_MODE)
EXPORT_SYMBOL(yt_exit);
#endif
