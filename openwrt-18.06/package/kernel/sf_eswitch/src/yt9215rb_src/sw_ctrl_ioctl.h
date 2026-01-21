/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef __SW_CTRL_IOCTL_H__
#define __SW_CTRL_IOCTL_H__

uint32_t sw_ctrl_ioctl_write(uint8_t unit, uint32_t regAddr, uint32_t regValue);
uint32_t sw_ctrl_ioctl_read(uint8_t unit, uint32_t regAddr, uint32_t *pRegVale);

int32_t sw_ctrl_ioctl_init(void);
void sw_ctrl_ioctl_exit(void);

#endif