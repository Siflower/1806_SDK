/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef __SWITCH_DRV_H__
#define __SWITCH_DRV_H__

#include "chipdef.h"
#include "yt_error.h"
#include "yt_board.h"

typedef struct yt_switch_drv_func_s
{
    yt_ret_t (*switch_init)(yt_unit_t);
    yt_ret_t (*switch_mac_config)(yt_unit_t);
    yt_ret_t (*switch_led_config)(yt_unit_t);
    yt_ret_t (*switch_intif_read)(yt_unit_t, uint8_t, uint32_t, uint16_t *);/*unit, intif id, reg, data*/
    yt_ret_t (*switch_intif_write)(yt_unit_t, uint8_t, uint32_t, uint16_t);/*unit, intif id, reg, data*/
    yt_ret_t (*switch_extif_read)(yt_unit_t, uint8_t, uint32_t, uint16_t *);/*unit, extif id, reg, data*/
    yt_ret_t (*switch_extif_write)(yt_unit_t, uint8_t, uint32_t, uint16_t);/*unit, extif id, reg, data*/
    yt_ret_t (*switch_extif_polling_config)(yt_unit_t);
    yt_ret_t (*switch_edata_read)(yt_unit_t, uint16_t, uint8_t *);/*unit, addr, data*/
}yt_switch_drv_func_t;

typedef struct yt_switch_drv_s
{
    yt_switch_chip_id_t    chip_id;
    /*yt_switch_chip_model_t chip_model;*/
    const yt_switch_drv_func_t   *pDrvFunc;
}yt_switch_drv_t;

extern const yt_switch_drv_t * const gpSwDrvList[];

#endif
