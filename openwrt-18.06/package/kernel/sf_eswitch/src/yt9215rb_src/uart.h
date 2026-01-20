/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef __DRV_UART_H__
#define __DRV_UART_H__
#include "yt_types.h"

extern int32_t uart_init(uint8_t *dev);
extern void uart_exit(void);
extern int32_t uart_switch_write(uint32_t regAddr,uint32_t regValue);
extern int32_t uart_switch_read(uint32_t regAddr, uint32_t *pRegAddr);
extern int32_t phy_operation(uint32_t addr);
#endif

