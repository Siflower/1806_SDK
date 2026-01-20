/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#include "sw_drv.h"

extern const yt_switch_drv_t yt9215_drv;
extern const yt_switch_drv_t yt9218_drv;

#define INT_IF_ACCESS_FRAME_CTRL   (0xf0000)
#define INT_IF_ACCESS_ADDR_CTRL    (0xf0004)
#define INT_IF_ACCESS_DATA_0_ADDR  (0xf0008)
#define INT_IF_ACCESS_DATA_1_ADDR  (0xf000c)

#define EXT_IF_ACCESS_FRAME_CTRL   (0x6a000)
#define EXT_IF_ACCESS_ADDR_CTRL    (0x6a004)
#define EXT_IF_ACCESS_DATA_0_ADDR  (0x6a008)
#define EXT_IF_ACCESS_DATA_1_ADDR  (0x6a00c)

#define EXT_IF_FREQUENCY_SEL   (4U)

#define E_OP_CTRL_0   (0xE0000)
#define E_OP_CTRL_1   (0xE0004)

#define MAX_BUSYING_WAIT_TIME   (200)

yt_ret_t yt9215_init(yt_unit_t unit);
yt_ret_t yt9215_mac_config(yt_unit_t unit);
yt_ret_t yt9215_led_config(yt_unit_t unit);
yt_ret_t yt9215_intif_read(yt_unit_t unit, uint8_t intif_addr, uint32_t regAddr, uint16_t *pData);
yt_ret_t yt9215_intif_write(yt_unit_t unit, uint8_t intif_addr, uint32_t regAddr, uint16_t data);
yt_ret_t yt9215_extif_read(yt_unit_t unit, uint8_t extif_addr, uint32_t regAddr, uint16_t *pData);
yt_ret_t yt9215_extif_write(yt_unit_t unit, uint8_t extif_addr, uint32_t regAddr, uint16_t data);
yt_ret_t yt9215_extif_polling_config(yt_unit_t unit);
yt_ret_t yt9215_edata_read(yt_unit_t unit, uint16_t addr, uint8_t *pData);