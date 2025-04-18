/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef __PHY_INT_SERDES_H__
#define __PHY_INT_SERDES_H__

#include "phy_drv.h"

extern yt_phy_drv_t int_serdes_drv;

yt_ret_t int_serdes_init(yt_unit_t);
yt_ret_t int_serdes_restart(yt_unit_t unit, uint8_t phy_addr);
yt_ret_t int_serdes_enable_set(yt_unit_t, uint8_t, yt_enable_t);
yt_ret_t int_serdes_enable_get(yt_unit_t, uint8_t, yt_enable_t *);
yt_ret_t int_serdes_medium_set(yt_unit_t, uint8_t, yt_port_medium_t);
yt_ret_t int_serdes_medium_get(yt_unit_t, uint8_t, yt_port_medium_t *);
yt_ret_t int_serdes_autoNeg_enable_set(yt_unit_t, uint8_t, yt_enable_t);
yt_ret_t int_serdes_autoNeg_enable_get(yt_unit_t, uint8_t, yt_enable_t *);
yt_ret_t int_serdes_autoNeg_ability_set(yt_unit_t, uint8_t, yt_port_an_ability_t);
yt_ret_t int_serdes_autoNeg_ability_get(yt_unit_t, uint8_t, yt_port_an_ability_t *);
yt_ret_t int_serdes_force_speed_duplex_set(yt_unit_t, uint8_t, yt_port_speed_duplex_t);
yt_ret_t int_serdes_force_speed_duplex_get(yt_unit_t, uint8_t, yt_port_speed_duplex_t *);
yt_ret_t int_serdes_link_status_get(yt_unit_t, uint8_t, yt_port_linkStatus_all_t *);
yt_ret_t int_serdes_eee_enable_set(yt_unit_t, uint8_t, yt_enable_t);
yt_ret_t int_serdes_eee_enable_get(yt_unit_t, uint8_t, yt_enable_t *);
yt_ret_t int_serdes_combo_mode_set(yt_unit_t, uint8_t, yt_combo_mode_t);
yt_ret_t int_serdes_combo_mode_get(yt_unit_t, uint8_t, yt_combo_mode_t *);
yt_ret_t int_serdes_cable_diag(yt_unit_t, uint8_t, yt_port_cableDiag_t *);
yt_ret_t int_serdes_interrupt_status_get(yt_unit_t, uint8_t, uint16_t *);
yt_ret_t int_serdes_test_template(yt_unit_t, uint8_t, yt_utp_template_testmode_t);

#endif
