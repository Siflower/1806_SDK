/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef __MEM_H__
#define __MEM_H__
#include "yt_types.h"

uint32_t mem_switch_write(uint32_t reg_addr, uint32_t reg_value);
uint32_t mem_switch_read(uint32_t reg_addr, uint32_t *reg_value);
#endif