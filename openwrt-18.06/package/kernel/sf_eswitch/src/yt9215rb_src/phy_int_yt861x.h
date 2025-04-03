/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "phy_drv.h"

extern yt_phy_drv_t int_yt861x_drv;

yt_ret_t int_yt861x_init(yt_unit_t);
yt_ret_t int_yt861x_restart(yt_unit_t unit, uint8_t phy_addr);
yt_ret_t int_yt861x_enable_set(yt_unit_t, uint8_t, yt_enable_t);
yt_ret_t int_yt861x_enable_get(yt_unit_t, uint8_t, yt_enable_t *);
yt_ret_t int_yt861x_medium_set(yt_unit_t, uint8_t, yt_port_medium_t);
yt_ret_t int_yt861x_medium_get(yt_unit_t, uint8_t, yt_port_medium_t *);
yt_ret_t int_yt861x_autoNeg_enable_set(yt_unit_t, uint8_t, yt_enable_t);
yt_ret_t int_yt861x_autoNeg_enable_get(yt_unit_t, uint8_t, yt_enable_t *);
yt_ret_t int_yt861x_autoNeg_ability_set(yt_unit_t, uint8_t, yt_port_an_ability_t);
yt_ret_t int_yt861x_autoNeg_ability_get(yt_unit_t, uint8_t, yt_port_an_ability_t *);
yt_ret_t int_yt861x_force_speed_duplex_set(yt_unit_t, uint8_t, yt_port_speed_duplex_t);
yt_ret_t int_yt861x_force_speed_duplex_get(yt_unit_t, uint8_t, yt_port_speed_duplex_t *);
yt_ret_t int_yt861x_link_status_get(yt_unit_t, uint8_t, yt_port_linkStatus_all_t *);
yt_ret_t int_yt861x_eee_enable_set(yt_unit_t, uint8_t, yt_enable_t);
yt_ret_t int_yt861x_eee_enable_get(yt_unit_t, uint8_t, yt_enable_t *);
yt_ret_t int_yt861x_combo_mode_set(yt_unit_t, uint8_t, yt_combo_mode_t);
yt_ret_t int_yt861x_combo_mode_get(yt_unit_t, uint8_t, yt_combo_mode_t *);
yt_ret_t int_yt861x_cable_diag(yt_unit_t, uint8_t, yt_port_cableDiag_t *);
yt_ret_t int_yt861x_interrupt_status_get(yt_unit_t, uint8_t, uint16_t *);
yt_ret_t int_yt861x_test_template(yt_unit_t, uint8_t, yt_utp_template_testmode_t);
