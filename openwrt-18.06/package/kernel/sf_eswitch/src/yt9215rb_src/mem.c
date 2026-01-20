/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "yt_error.h"
#include "yt_lock.h"
#include "mem.h"

/**************************************************
 *      Constants or macros Declaration            *
 **************************************************/
#define REG(x) (*((volatile unsigned int *)(x)))

uint32_t mem_switch_write(uint32_t reg_addr, uint32_t reg_value)
{   
    ACCESS_LOCK();
    REG(reg_addr) = reg_value;
    ACCESS_UNLOCK();
    
    return CMM_ERR_OK;
}

uint32_t mem_switch_read(uint32_t reg_addr, uint32_t *reg_value)
{
    ACCESS_LOCK();
    *reg_value = REG(reg_addr);
    ACCESS_UNLOCK();

    return CMM_ERR_OK;
}

