/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#include "sw_drv.h"

extern const yt_switch_drv_t yt9232_drv;

#define INT_IF_ACCESS_FRAME_CTRL   (0xf0000)
#define INT_IF_ACCESS_ADDR_CTRL    (0xf0004)
#define INT_IF_ACCESS_DATA_0_ADDR  (0xf0008)
#define INT_IF_ACCESS_DATA_1_ADDR  (0xf000c)

#define EXT_IF_ACCESS_FRAME_CTRL   (0xf1000)
#define EXT_IF_ACCESS_ADDR_CTRL    (0xf1004)
#define EXT_IF_ACCESS_DATA_0_ADDR  (0xf1008)
#define EXT_IF_ACCESS_DATA_1_ADDR  (0xf100c)

#define EXT_IF_INDEX_OFFSET    (0x1000)
#define EXT_IF_FREQUENCY_SEL   (4U)

#define E_OP_CTRL_0   (0xE0000)
#define E_OP_CTRL_1   (0xE0004)

#define MAX_BUSYING_WAIT_TIME   (200)

yt_ret_t yt9232_init(yt_unit_t unit);
yt_ret_t yt9232_mac_config(yt_unit_t unit);
yt_ret_t yt9232_led_config(yt_unit_t unit);
yt_ret_t yt9232_intif_read(yt_unit_t unit, uint8_t intif_addr, uint32_t regAddr, uint16_t *pData);
yt_ret_t yt9232_intif_write(yt_unit_t unit, uint8_t intif_addr, uint32_t regAddr, uint16_t data);
yt_ret_t yt9232_extif_read(yt_unit_t unit, uint8_t extif_addr, uint32_t regAddr, uint16_t *pData);
yt_ret_t yt9232_extif_write(yt_unit_t unit, uint8_t extif_addr, uint32_t regAddr, uint16_t data);
yt_ret_t yt9232_extif_polling_config(yt_unit_t unit);
yt_ret_t yt9232_edata_read(yt_unit_t unit, uint16_t addr, uint8_t *pData);
