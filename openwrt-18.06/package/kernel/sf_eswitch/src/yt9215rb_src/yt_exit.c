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

yt_ret_t yt_exit(uint8_t unit)
{
    CMM_UNUSED_PARAM(unit);

    hal_table_reg_exit();

    return CMM_ERR_OK;
}
