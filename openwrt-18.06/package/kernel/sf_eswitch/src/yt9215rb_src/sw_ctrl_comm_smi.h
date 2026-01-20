/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef __SW_CTRL_COMM_SMI_H__
#define __SW_CTRL_COMM_SMI_H__

uint32_t switch_ctrl_smi_write(uint8_t unit, uint32_t addr, uint32_t data);
uint32_t switch_ctrl_smi_read(uint8_t unit, uint32_t addr, uint32_t *data);

void switch_ctrl_smi_init(void);

#endif