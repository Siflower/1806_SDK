/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef __SW_CTRL_COMM_SHARE_H__
#define __SW_CTRL_COMM_SHARE_H__
#include <linux/types.h>

#ifndef BIT
#define BIT(nr)			(1UL << (nr))
#endif

#define SW_CTRL_FLAG_READ       BIT(0)
#define SW_CTRL_FLAG_WRITE      BIT(1)

typedef struct sw_ctrl_comm_data_s {
    uint32_t regAddr;
    uint32_t regVal;
    uint8_t unit;
    uint8_t flags;
}sw_ctrl_comm_data_t;

#define SWACCESSIOCTL   _IOWR('s', 0x27, struct sw_ctrl_comm_data_s)

#endif