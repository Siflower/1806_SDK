/*
 * Copyright (c) 2014-2024 JLSemi Limited
 * All Rights Reserved
 *
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of JLSemi Limited
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code.
 *
 * No part of this code may be reproduced, stored in a retrieval system,
 * or transmitted, in any form or by any means, electronic, mechanical,
 * photocopying, recording, or otherwise, without the prior written
 * permission of JLSemi Limited
 */

#include "jl_switch.h"

#include "jl51xx/jl51xx_drv_phy.h"
#include "jl51xx/jl51xx_drv_port.h"
#include "jl51xx/jl51xx_mac_uctrl.h"
#include "port.h"

static jl_ret_t _jl_port_phy_autoneg_ability_set(jl_port_t port, jl_port_phy_ability_t *pability)
{
	jl_ret_t ret;
	jl_uint32 reg_val;
	jl_uint32 reg0_mask = 0;
	jl_uint32 reg4_mask = 0;
	jl_uint32 macuctrl_mask = 0;

	if (pability->half_duplex_10 >= JL_ENABLE_END || pability->full_duplex_10 >= JL_ENABLE_END || \
			pability->half_duplex_100 >= JL_ENABLE_END || pability->full_duplex_100 >= JL_ENABLE_END || \
			pability->flow_control >= JL_ENABLE_END || pability->asym_flow_control >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	/*for PHY auto mode setup*/
	pability->auto_negotiation = 1;

	/*
	 * 0.6   0.13 speed_selection
	 *   0    0    10Mbps
	 *   0	  1    100Mbps
     *   1    0    1000Mbps
     *   1    1    Reserved
	 */

	if (pability->half_duplex_10 == 1)
		reg4_mask |= BIT(5);

	if (pability->full_duplex_10 == 1)
		reg4_mask |= BIT(6);

	if (pability->half_duplex_100 == 1)
		reg4_mask |= BIT(7);

	if (pability->full_duplex_100 == 1)
		reg4_mask |= BIT(8);

	if (pability->auto_negotiation == 1) {
		reg0_mask |= BIT(12);
		macuctrl_mask &= (~BIT(4));
		macuctrl_mask &= (~BIT(5));
		macuctrl_mask &= (~BIT(7));
	}

	if (pability->flow_control == 1)
		reg4_mask |= BIT(10);

	if (pability->asym_flow_control == 1)
		reg4_mask |= BIT(11);

	/* If auto negotiation is capable, restart it */
	if (1 == pability->auto_negotiation)
		reg0_mask |= BIT(9);
    /* Auto-Negotiation Advertisement Register setting*/
	if ((ret = jl51xx_port_phy_an_adv_reg_get(
				port,
				&reg_val)) != JL_ERR_OK)
		return ret;
	reg_val = (reg_val & (~0x0DE0)) | reg4_mask;

	if ((ret = jl51xx_port_phy_an_adv_reg_set(
				port,
				reg_val)) != JL_ERR_OK)
		return ret;

	/* Mac User Control Register setting */
	ret = jl51xx_mac_uctrl_read(port, &reg_val);
	if (ret != JL_ERR_OK)
		return ret;

	reg_val = (reg_val & (~0xB0)) | macuctrl_mask;

	ret = jl51xx_mac_uctrl_write(port, reg_val);
	if (ret != JL_ERR_OK)
		return ret;

	/* Basic Mode Control Register setting */
	if ((ret = jl51xx_port_phy_control_reg_get(
				port,
				&reg_val)) != JL_ERR_OK)
		return ret;

	reg_val = (reg_val & (~0x1200)) | reg0_mask;

	if ((ret = jl51xx_port_phy_control_reg_set(
				port,
				reg_val)) != JL_ERR_OK)
		return ret;

	return JL_ERR_OK;
}

static jl_ret_t _jl_port_phy_autoneg_ability_get(jl_port_t port, jl_port_phy_ability_t *pability)
{
	jl_ret_t ret;
	jl_uint32 reg0_val;
	jl_uint32 reg4_val = 0;

    /* Auto-Negotiation Advertisement Register setting*/
	if ((ret = jl51xx_port_phy_an_adv_reg_get(
				port,
				&reg4_val)) != JL_ERR_OK)
		return ret;

	if (reg4_val & BIT(11))
		pability->asym_flow_control = 1;
	else
		pability->asym_flow_control = 0;

	if (reg4_val & BIT(10))
		pability->flow_control = 1;
	else
		pability->flow_control = 0;

	if (reg4_val & BIT(8))
		pability->full_duplex_100 = 1;
	else
		pability->full_duplex_100 = 0;

	if (reg4_val & BIT(7))
		pability->half_duplex_100 = 1;
	else
		pability->half_duplex_100 = 0;

	if (reg4_val & BIT(6))
		pability->full_duplex_10 = 1;
	else
		pability->full_duplex_10 = 0;

	if (reg4_val & BIT(5))
		pability->half_duplex_10 = 1;
	else
		pability->half_duplex_10 = 0;

	/* Basic Mode Control Register setting */
	if ((ret = jl51xx_port_phy_control_reg_get(
				port,
				&reg0_val)) != JL_ERR_OK)
		return ret;

	if (reg0_val & BIT(12))
		pability->auto_negotiation = 1;
	else
		pability->auto_negotiation = 0;

	return JL_ERR_OK;
}

static jl_ret_t _jl_port_phy_force_ability_set(jl_port_t port, jl_port_phy_ability_t *pability)
{
	jl_ret_t ret;
	jl_uint32 reg_val;
	jl_uint32 reg0_mask = 0;
	jl_uint32 macuctrl_mask = 0;

	if (pability->half_duplex_10 >= JL_ENABLE_END || pability->full_duplex_10 >= JL_ENABLE_END || \
			pability->half_duplex_100 >= JL_ENABLE_END || pability->full_duplex_100 >= JL_ENABLE_END || \
			pability->flow_control >= JL_ENABLE_END || pability->asym_flow_control >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	/*for PHY auto mode setup*/
	pability->auto_negotiation = 0;

	/*
	 * 0.6   0.13 speed_selection
	 *   0    0    10Mbps
	 *   0	  1    100Mbps
     *   1    0    1000Mbps
     *   1    1    Reserved
	 */

	if (pability->half_duplex_10 == 1) {
		reg0_mask &= (~BIT(8));
		reg0_mask &= (~BIT(13));
	}

	if (pability->full_duplex_10 == 1) {
		reg0_mask |= BIT(8);
		reg0_mask &= (~BIT(13));
	}

	if (pability->half_duplex_100 == 1) {
		reg0_mask &= (~BIT(8));
		reg0_mask |= BIT(13);
	}

	if (pability->full_duplex_100 == 1) {
		reg0_mask |= BIT(8);
		reg0_mask |= BIT(13);
	}

	if (pability->auto_negotiation == 0)
		reg0_mask &= (~BIT(12));

	if (pability->flow_control == 1) {
		macuctrl_mask |= BIT(4);
		macuctrl_mask |= BIT(5);
		macuctrl_mask |= BIT(7);
	}

	/* If auto negotiation is capable, restart it */
	reg0_mask |= BIT(9);

	/* Mac User Control Register setting */
	ret = jl51xx_mac_uctrl_read(port, &reg_val);
	if (ret != JL_ERR_OK)
		return ret;

	reg_val = (reg_val & (~0xB0)) | macuctrl_mask;

	ret = jl51xx_mac_uctrl_write(port, reg_val);
	if (ret != JL_ERR_OK)
		return ret;

	/* Basic Mode Control Register setting */
	if ((ret = jl51xx_port_phy_control_reg_get(
				port,
				&reg_val)) != JL_ERR_OK)
		return ret;

	reg_val = (reg_val & (~0x3300)) | reg0_mask;

	if ((ret = jl51xx_port_phy_control_reg_set(
				port,
				reg_val)) != JL_ERR_OK)
		return ret;

	return JL_ERR_OK;
}

static jl_ret_t _jl_port_phy_force_ability_get(jl_port_t port, jl_port_phy_ability_t *pability)
{
	jl_ret_t ret;
	jl_uint32 reg0_val = 0;
	jl_uint32 macuctrl_val = 0;

	/* Basic Mode Control Register setting */
	if ((ret = jl51xx_port_phy_control_reg_get(
				port,
				&reg0_val)) != JL_ERR_OK)
		return ret;

	if (reg0_val & BIT(12))
		pability->auto_negotiation = 1;
	else
		pability->auto_negotiation = 0;

	if ((reg0_val & BIT(8)) &&
			(reg0_val & BIT(13)))
		pability->full_duplex_100 = 1;
	else if  ((!(reg0_val & BIT(8))) &&
			(reg0_val & BIT(13)))
		pability->half_duplex_100 = 1;
	else if  ((reg0_val & BIT(8)) &&
			(!(reg0_val & BIT(13))))
		pability->full_duplex_10 = 1;
	else
		pability->half_duplex_10 = 1;

	/* Mac User Control Register setting */
	if ((ret = jl51xx_mac_uctrl_read(
				port,
				&macuctrl_val)) != JL_ERR_OK)
		return ret;

	if ((macuctrl_val & BIT(7)) &&
			(macuctrl_val & BIT(5)) &&
			(macuctrl_val & BIT(4))
			)
		pability->flow_control = 1;
	else
		pability->flow_control = 0;

	pability->asym_flow_control = 0;

    /* Auto-Negotiation Advertisement Register setting */

	return JL_ERR_OK;
}

static jl_ret_t _jl_port_mac_status_get(jl_port_t port, jl_port_mac_ability_t *pability)
{
	jl_ret_t ret;
	jl_uint32 reg1_val;
	jl_uint8 phy_speed = 0;
	jl_uint8 fsm = 0;

	//Get MAC_force_mode flag
	if ((ret = jl51xx_port_mac_force_link_get(
					port,
					pability)) != JL_ERR_OK)
		return ret;

	if ((ret = jl51xx_port_phy_status_reg_get(
				port,
				&reg1_val)) != JL_ERR_OK)
		return ret;

	fsm = reg1_val & 0x0F;
	if (fsm != 2) {
		pability->link = 0; //FW not ready
		return JL_ERR_OK;
	}
	pability->link = 1;

	phy_speed = (reg1_val >> 4) & 0xF;
	if (phy_speed == 0) {
		pability->duplex = PORT_HALF_DUPLEX;
		pability->speed = PORT_SPEED_10M;
	} else if (phy_speed == 1) {
		pability->duplex = PORT_FULL_DUPLEX;
		pability->speed = PORT_SPEED_10M;
	} else if (phy_speed == 2) {
		pability->duplex = PORT_HALF_DUPLEX;
		pability->speed = PORT_SPEED_100M;
	} else if (phy_speed == 3) {
		pability->duplex = PORT_FULL_DUPLEX;
		pability->speed = PORT_SPEED_100M;
	} else if (phy_speed == 4) {
		pability->link = 0;
	}

	if (reg1_val & BIT(14))
		pability->rx_pause = 1;
	else
		pability->rx_pause = 0;

	if (reg1_val & BIT(15))
		pability->tx_pause = 1;
	else
		pability->tx_pause = 0;

	return JL_ERR_OK;
}

static jl_ret_t _jl_port_phy_status_get(jl_port_t port, jl_port_link_status_t *pstatus, jl_port_duplex_t *pduplex, jl_port_speed_t *pspeed)
{
	jl_ret_t ret;
	jl_uint32 reg1_val;
	jl_uint8 phy_speed = 0;
	jl_uint8 fsm = 0;

	if ((ret = jl51xx_port_phy_status_reg_get(
				port,
				&reg1_val)) != JL_ERR_OK)
		return ret;

	*pduplex = 0;
	*pspeed = 0;
	fsm = reg1_val & 0x0F;
	if (fsm != 2) {
		*pstatus = 0; //FW not ready
		return JL_ERR_OK;
	}
	*pstatus = 1;

	phy_speed = (reg1_val >> 4) & 0x0F;
	if (phy_speed == 0) {
		*pduplex = PORT_HALF_DUPLEX;
		*pspeed = PORT_SPEED_10M;
	} else if (phy_speed == 1) {
		*pduplex = PORT_FULL_DUPLEX;
		*pspeed = PORT_SPEED_10M;
	} else if (phy_speed == 2) {
		*pduplex = PORT_HALF_DUPLEX;
		*pspeed = PORT_SPEED_100M;
	} else if (phy_speed == 3) {
		*pduplex = PORT_FULL_DUPLEX;
		*pspeed = PORT_SPEED_100M;
	} else if (phy_speed == 4) {
		*pstatus = 0;
	}

	return JL_ERR_OK;
}

static jl_ret_t _jl_port_phy_isolate_set(jl_port_t port, jl_enable_t isolate)
{
	return jl51xx_port_phy_isolate_set(port, isolate);
}

static jl_ret_t _jl_port_phy_isolate_get(jl_port_t port, jl_enable_t *pisolate)
{
	return jl51xx_port_phy_isolate_get(port, pisolate);
}

static jl_ret_t _jl_port_phy_enable_set(jl_port_t port, jl_enable_t enable)
{
	return jl51xx_port_phy_enable_set(port, enable);
}

static jl_ret_t _jl_port_phy_enable_get(jl_port_t port, jl_enable_t *penable)
{
	return jl51xx_port_phy_enable_get(port, penable);
}

static jl_ret_t _jl_port_phy_indirect_reg_set(jl_port_t port, jl_port_phy_reg_t reg, jl_port_phy_data_t regval)
{
	return jl51xx_phy_indirect_write(port, reg, regval);
}

static jl_ret_t _jl_port_phy_indirect_reg_get(jl_port_t port, jl_port_phy_reg_t reg, jl_port_phy_data_t *pregval)
{
	return jl51xx_phy_indirect_read(port, reg, pregval);
}

static jl_ret_t _jl_port_mac_force_link_set(jl_port_t port, jl_port_mac_ability_t *pability)
{
	if (pability->speed > 1 || pability->force_mode > 1 ||
			pability->duplex > 1 || pability->link > 1 ||
			pability->tx_pause > 1 ||
			pability->rx_pause > 1)
		return JL_ERR_PARAM;
	return jl51xx_port_mac_force_link_set(port, pability);
}

static jl_ret_t _jl_port_mac_force_link_ext_set(jl_port_t port, jl_port_ext_mac_ability_t *pability)
{
	if (pability->speed > 1 || pability->force_mode > 1 ||
			pability->duplex > 1 || pability->link > 1 ||
			pability->tx_pause > 1 ||
			pability->rx_pause > 1)
		return JL_ERR_PARAM;
	return jl51xx_port_mac_force_link_ext_set(port, pability);
}

static jl_ret_t _jl_port_mac_force_link_ext_get(jl_port_t port, jl_port_ext_mac_ability_t *pability)
{
	return jl51xx_port_mac_force_link_ext_get(port, pability);
}
static jl_ret_t _jl_port_mac_force_link_get(jl_port_t port, jl_port_mac_ability_t *pability)
{
	return jl51xx_port_mac_force_link_get(port, pability);
}

static jl_ret_t _jl_port_mac_loopback_get(jl_port_t port, jl_enable_t *pen_loopback)
{
	return jl51xx_port_mac_loopback_get(port, pen_loopback);
}

static jl_ret_t _jl_port_mac_loopback_set(jl_port_t port, jl_enable_t en_loopback)
{
	return jl51xx_port_mac_loopback_set(port, en_loopback);
}

jl_api_ret_t jl_port_phy_autoneg_ability_set(jl_port_t port, jl_port_phy_ability_t *pability)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(port);

	JL_CHECK_POINTER(pability);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_phy_autoneg_ability_set(port, pability);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_phy_autoneg_ability_set);

jl_api_ret_t jl_port_phy_autoneg_ability_get(jl_port_t port, jl_port_phy_ability_t *pability)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(port);

	JL_CHECK_POINTER(pability);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_phy_autoneg_ability_get(port, pability);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_phy_autoneg_ability_get);

jl_api_ret_t jl_port_phy_force_ability_set(jl_port_t port, jl_port_phy_ability_t *pability)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(port);

	JL_CHECK_POINTER(pability);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_phy_force_ability_set(port, pability);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_phy_force_ability_set);

jl_api_ret_t jl_port_phy_force_ability_get(jl_port_t port, jl_port_phy_ability_t *pability)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(port);

	JL_CHECK_POINTER(pability);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_phy_force_ability_get(port, pability);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_phy_force_ability_get);

jl_api_ret_t jl_port_phy_link_status_get(jl_port_t port, jl_port_link_status_t *pstatus, jl_port_duplex_t *pduplex, jl_port_speed_t *pspeed)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(port);

	if (NULL == pstatus || NULL == pduplex || NULL == pspeed)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_port_phy_status_get(port, pstatus, pduplex, pspeed);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_phy_link_status_get);

jl_api_ret_t jl_port_phy_reg_set(jl_port_t port, jl_port_phy_page_t page, jl_port_phy_reg_t reg, jl_port_phy_data_t regval)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = jl51xx_phy_direct_write_ext(port, page, reg, regval);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_phy_reg_set);

jl_api_ret_t jl_port_phy_reg_get(jl_port_t port, jl_port_phy_page_t page, jl_port_phy_reg_t reg, jl_port_phy_data_t *pregval)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(port);

	JL_CHECK_POINTER(pregval);

	JL_API_MUTEX_LOCK();
	ret = jl51xx_phy_direct_read_ext(port, page, reg, pregval);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_phy_reg_get);

jl_api_ret_t jl_port_phy_indirect_reg_set(jl_port_t port, jl_port_phy_reg_t reg, jl_port_phy_data_t regval)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_phy_indirect_reg_set(port, reg, regval);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_phy_indirect_reg_set);

jl_api_ret_t jl_port_phy_indirect_reg_get(jl_port_t port, jl_port_phy_reg_t reg, jl_port_phy_data_t *pregval)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(port);

	JL_CHECK_POINTER(pregval);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_phy_indirect_reg_get(port, reg, pregval);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_phy_indirect_reg_get);

jl_api_ret_t jl_port_phy_isolate_set(jl_port_t port, jl_enable_t isolate)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(port);

	if (isolate >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_port_phy_isolate_set(port, isolate);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_phy_isolate_set);

jl_api_ret_t jl_port_phy_isolate_get(jl_port_t port, jl_enable_t *pisolate)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(port);

	JL_CHECK_POINTER(pisolate);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_phy_isolate_get(port, pisolate);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_phy_isolate_get);

jl_api_ret_t jl_port_phy_enable_set(jl_port_t port, jl_enable_t enable)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(port);

	if (enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_port_phy_enable_set(port, enable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_phy_enable_set);

jl_api_ret_t jl_port_phy_enable_get(jl_port_t port, jl_enable_t *penable)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(port);

	JL_CHECK_POINTER(penable);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_phy_enable_get(port, penable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_phy_enable_get);

jl_api_ret_t jl_port_phy_all_enable_set(jl_enable_t enable)
{
	jl_api_ret_t ret = JL_ERR_OK;
	jl_port_t port;

	/* Check System state */
	JL_CHECK_STAT();

	if (enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	JL_FOR_EACH_PHY_UTP_PORT(port) {
		if ((ret = _jl_port_phy_enable_set(port, enable)) != JL_ERR_OK)
			break;
	}
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_phy_all_enable_set);

jl_api_ret_t jl_port_mac_force_link_set(jl_port_t port, jl_port_mac_ability_t *pability)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(port);

	JL_CHECK_POINTER(pability);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_mac_force_link_set(port, pability);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_mac_force_link_set);

jl_api_ret_t jl_port_mac_force_link_get(jl_port_t port, jl_port_mac_ability_t *pability)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(port);

	JL_CHECK_POINTER(pability);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_mac_force_link_get(port, pability);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_mac_force_link_get);

jl_ret_t jl_port_mac_force_link_ext_set(jl_port_t port, jl_port_ext_mac_ability_t *pability)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Ext Port */
	JL_CHECK_EXT_PORT(port);

	JL_CHECK_POINTER(pability);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_mac_force_link_ext_set(port, pability);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_mac_force_link_ext_set);

jl_api_ret_t jl_port_mac_force_link_ext_get(jl_port_t port, jl_port_ext_mac_ability_t *pability)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Ext Port */
	JL_CHECK_EXT_PORT(port);

	JL_CHECK_POINTER(pability);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_mac_force_link_ext_get(port, pability);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_mac_force_link_ext_get);

jl_api_ret_t jl_port_mac_status_get(jl_port_t port, jl_port_mac_ability_t *pability)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_PORT(port);

	JL_CHECK_POINTER(pability);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_mac_status_get(port, pability);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_mac_status_get);

jl_api_ret_t jl_port_mac_loopback_set(jl_port_t port, jl_enable_t enable)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(port);

	if (enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_port_mac_loopback_set(port, enable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_mac_loopback_set);

jl_api_ret_t jl_port_mac_loopback_get(jl_port_t port, jl_enable_t *penable)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(port);

	JL_CHECK_POINTER(penable);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_mac_loopback_get(port, penable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_mac_loopback_get);
