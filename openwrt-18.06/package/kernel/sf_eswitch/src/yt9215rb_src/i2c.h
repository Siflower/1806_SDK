/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef __CTRLIF_I2C_H__
#define __CTRLIF_I2C_H__
#include "yt_types.h"

uint32_t i2c_switch_write(uint8_t unit, uint32_t regAddr, uint32_t regValue);
uint32_t i2c_switch_read(uint8_t unit, uint32_t regAddr, uint32_t *pRegValue);
#endif