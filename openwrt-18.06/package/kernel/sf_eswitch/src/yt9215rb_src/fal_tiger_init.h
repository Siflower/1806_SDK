/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#include "yt_error.h"
#include "yt_types.h"
#include "yt_port.h"
#include "osal_mem.h"

extern yt_ret_t fal_tiger_init(yt_unit_t unit);
yt_ret_t CMM_CALSdsMode_to_YTExtMode(yt_serdes_mode_t sdsMode, yt_extif_mode_t *pExtifMode);
