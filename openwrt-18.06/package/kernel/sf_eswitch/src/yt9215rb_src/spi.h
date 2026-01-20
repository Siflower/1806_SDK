/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef __CTRLIF_SPI_H__
#define __CTRLIF_SPI_H__
#include "yt_types.h"

uint32_t spi_switch_write(uint8_t unit, uint32_t regAddr, uint32_t regValue);
uint32_t spi_switch_read(uint8_t unit, uint32_t regAddr, uint32_t *pRegValue);
#endif