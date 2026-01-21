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

#include "jl_base.h"
#include "jl51xx/jl51xx_drv_phy.h"
#include "jl51xx/reg_phy.h"
#include "jl51xx/jl51xx_drv_port.h"
#include "jl51xx/jl51xx_mac_uctrl.h"

jl_ret_t jl51xx_port_phy_control_reg_set(jl_port_t port, jl_uint32 regval)
{
	jl_ret_t ret = JL_ERR_OK;

	ret = jl51xx_phy_direct_write_ext(port, 0, BMCR_OFFSET, regval);

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "%s reg=0x%4x\n", __func__, regval);
	return ret;
}

jl_ret_t jl51xx_port_phy_control_reg_get(jl_port_t port, jl_uint32 *pregval)
{
	jl_ret_t ret;

	*pregval = 0;
	ret = jl51xx_phy_direct_read_ext(port, 0, BMCR_OFFSET, pregval);

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "%s reg=0x%4x\n", __func__, *pregval);
	return ret;
}

jl_ret_t jl51xx_port_phy_an_adv_reg_set(jl_port_t port, jl_uint32 regval)
{
	jl_ret_t ret;

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "%s reg=0x%4x\n", __func__, regval);

	ret = jl51xx_phy_direct_write_ext(port, 0, ANAR_OFFSET, regval);
	return ret;
}

jl_ret_t jl51xx_port_phy_an_adv_reg_get(jl_port_t port, jl_uint32 *pregval)
{
	jl_ret_t ret;

	ret = jl51xx_phy_direct_read_ext(port, 0, ANAR_OFFSET, pregval);

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "%s reg=0x%4x\n", __func__, *pregval);

	return ret;
}

jl_ret_t jl51xx_port_phy_status_reg_get(jl_port_t port, jl_uint32 *pregval)
{
	jl_ret_t ret;


	ret = jl51xx_sw_rsvd_read(port, pregval);

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "%s reg=0x%4x\n", __func__, *pregval);
	return ret;
}

jl_ret_t jl51xx_port_phy_enable_set(jl_port_t port, jl_enable_t enable)
{
	jl_uint32 regval;
	jl_ret_t ret = JL_ERR_OK;

	if (port >= JL_PORT_MAX)
		return JL_ERR_PARAM;

	ret = jl51xx_phy_direct_read_ext(port, 0, BMCR_OFFSET,
					&regval);
	if (ret)
		return ret;
	if (enable == 0)
		SET_BIT(regval, 11);
	else
		CLR_BIT(regval, 11);

	return jl51xx_phy_direct_write_ext(port, 0, BMCR_OFFSET, regval);
}

jl_ret_t jl51xx_port_phy_enable_get(jl_port_t port, jl_enable_t *penable)
{
	jl_uint32 regval;
	jl_ret_t ret = JL_ERR_OK;

	if (port >= JL_PORT_MAX)
		return JL_ERR_PARAM;

	ret = jl51xx_phy_direct_read_ext(port, 0, BMCR_OFFSET,
					&regval);
	if (ret)
		return ret;

	*penable = (regval & BIT(11)) ? 0 : 1;
	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_phy_isolate_set(jl_port_t port, jl_enable_t enable)
{
	jl_uint32 regval;
	jl_ret_t ret = JL_ERR_OK;

	if (port >= JL_PORT_MAX)
		return JL_ERR_PARAM;

	ret = jl51xx_phy_direct_read_ext(port, 0, BMCR_OFFSET,
					&regval);
	if (ret)
		return ret;
	if (enable == 1)
		SET_BIT(regval, 10);
	else
		CLR_BIT(regval, 10);

	return jl51xx_phy_direct_write_ext(port, 0, BMCR_OFFSET, regval);
}

jl_ret_t jl51xx_port_phy_isolate_get(jl_port_t port, jl_enable_t *penable)
{
	jl_uint32 regval;
	jl_ret_t ret = JL_ERR_OK;

	if (port >= JL_PORT_MAX)
		return JL_ERR_PARAM;

	ret = jl51xx_phy_direct_read_ext(port, 0, BMCR_OFFSET,
					&regval);
	if (ret)
		return ret;

	*penable = (regval & BIT(10)) ? 1 : 0;
	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_phy_loopback_get(jl_port_t port, jl_enable_t *pen_loopback)
{
	jl_uint32 regval;
	jl_ret_t ret = JL_ERR_OK;

	if (port >= JL_PORT_MAX)
		return JL_ERR_PARAM;

	ret = jl51xx_phy_direct_read_ext(port, 0, BMCR_OFFSET,
					&regval);
	if (ret)
		return ret;

	*pen_loopback = (regval & BIT(14)) ? 1 : 0;
	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_phy_loopback_set(jl_port_t port, jl_enable_t en_loopback)
{
	jl_uint32 regval;
	jl_ret_t ret = JL_ERR_OK;

	if (port >= JL_PORT_MAX)
		return JL_ERR_PARAM;

	ret = jl51xx_phy_direct_read_ext(port, 0, BMCR_OFFSET,
					&regval);
	if (ret)
		return ret;
	if (en_loopback == 1)
		SET_BIT(regval, 14);
	else
		CLR_BIT(regval, 14);

	return jl51xx_phy_direct_write_ext(port, 0, BMCR_OFFSET, regval);
}

jl_ret_t jl51xx_port_mac_force_link_set(jl_port_t phy_port, jl_port_mac_ability_t *pability)
{
	jl_ret_t ret;
	jl_uint64 mac_is_link, mac_duplex, mac_speed;
	jl_uint64 mac_tx_fc_en, mac_rx_fc_en, mac_force_fc_en, mac_force_status_en;
	jl_uint32 mac_user_ctrl_val[1];
	jl_uint8 *preg_val = NULL;

	preg_val = (jl_uint8 *)&mac_user_ctrl_val[0];

	ret = jl51xx_mac_uctrl_read(phy_port, &mac_user_ctrl_val[0]);
	if (ret)
		return ret;

	//mac_user_ctrl_val[0] &= ~(0x40B7);
	/* mac force status control set*/
	mac_force_status_en = pability->force_mode;
	mac_is_link = pability->link;
	mac_duplex = pability->duplex;
	mac_speed = pability->speed;
	/* mac force flow contorl set*/
	mac_force_fc_en = pability->force_fc_en;
	mac_tx_fc_en = pability->tx_pause;
	mac_rx_fc_en = pability->rx_pause;

	jl_write_bits(preg_val, mac_is_link, 0, 1);
	jl_write_bits(preg_val, mac_duplex, 1, 1);
	jl_write_bits(preg_val, mac_speed, 2, 1);
	jl_write_bits(preg_val, mac_tx_fc_en, 4, 1);
	jl_write_bits(preg_val, mac_rx_fc_en, 5, 1);
	jl_write_bits(preg_val, mac_force_fc_en, 7, 1);
	jl_write_bits(preg_val, mac_force_status_en, 14, 1);

	ret = jl51xx_mac_uctrl_write(phy_port, mac_user_ctrl_val[0]);
	if (ret)
		return ret;

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "%s reg=0x%4x\n", __func__, mac_user_ctrl_val[0]);
	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_mac_force_link_get(jl_port_t phy_port, jl_port_mac_ability_t *pability)
{
	jl_ret_t ret;
	jl_uint64 mac_is_link, mac_duplex, mac_speed,
		mac_tx_fc_en, mac_rx_fc_en, mac_force_status_en;
	jl_uint32 mac_user_ctrl_val[1];
	jl_uint8 *preg_val = NULL;

	preg_val = (jl_uint8 *)&mac_user_ctrl_val[0];

	ret = jl51xx_mac_uctrl_read(phy_port, &mac_user_ctrl_val[0]);
	if (ret)
		return ret;
	jl_read_bits(preg_val, &mac_is_link, 0, 1);
	jl_read_bits(preg_val, &mac_duplex, 1, 1);
	jl_read_bits(preg_val, &mac_speed, 2, 1);
	jl_read_bits(preg_val, &mac_force_status_en, 14, 1);
	if (mac_user_ctrl_val[0] & BIT(7)) {
		jl_read_bits(preg_val, &mac_tx_fc_en, 4, 1);
		jl_read_bits(preg_val, &mac_rx_fc_en, 5, 1);
	} else {
		mac_tx_fc_en = 0;
		mac_rx_fc_en = 0;
	}

	pability->link = (jl_uint32)mac_is_link;
	pability->duplex = (jl_uint32)mac_duplex;
	pability->speed = (jl_uint32)mac_speed;
	pability->tx_pause = (jl_uint32)mac_tx_fc_en;
	pability->rx_pause = (jl_uint32)mac_rx_fc_en;
	pability->force_mode = (jl_uint32)mac_force_status_en;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_mac_force_link_ext_set(jl_port_t port, jl_port_ext_mac_ability_t *pability)
{
	jl_ret_t ret;
	jl_uint64 mac_is_link, mac_duplex, mac_speed;
	jl_uint64 mac_tx_fc_en, mac_rx_fc_en;
	jl_uint32 mac_user_ctrl_val[1];
	jl_uint8 *preg_val = NULL;

	/* set EXT MAC force link ability */
	preg_val = (jl_uint8 *)&mac_user_ctrl_val[0];

	ret = jl51xx_mac_uctrl_read(port, &mac_user_ctrl_val[0]);
	if (ret)
		return ret;

	mac_duplex = pability->duplex;
	mac_speed = pability->speed;
	mac_tx_fc_en = pability->tx_pause;
	mac_rx_fc_en = pability->rx_pause;
	mac_is_link = pability->link;

	pability->force_mode = 1;

	jl_write_bits(preg_val, mac_is_link, 0, 1);
	jl_write_bits(preg_val, mac_duplex, 1, 1);
	jl_write_bits(preg_val, mac_speed, 2, 1);
	jl_write_bits(preg_val, mac_tx_fc_en, 4, 1);
	jl_write_bits(preg_val, mac_rx_fc_en, 5, 1);

	ret = jl51xx_mac_uctrl_write(port, mac_user_ctrl_val[0]);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_mac_force_link_ext_get(jl_port_t port, jl_port_ext_mac_ability_t *pability)
{
	jl_uint32 mac_user_ctrl_val[1];
	jl_ret_t ret;
	jl_uint64 mac_is_link, mac_duplex, mac_speed,
		mac_tx_fc_en, mac_rx_fc_en;
	jl_uint8 *preg_val = NULL;

	/* get ext mac force link ability */
	preg_val = (jl_uint8 *)&mac_user_ctrl_val[0];

	ret = jl51xx_mac_uctrl_read(port, &mac_user_ctrl_val[0]);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &mac_is_link, 0, 1);
	jl_read_bits(preg_val, &mac_duplex, 1, 1);
	jl_read_bits(preg_val, &mac_speed, 2, 1);
	jl_read_bits(preg_val, &mac_tx_fc_en, 4, 1);
	jl_read_bits(preg_val, &mac_rx_fc_en, 5, 1);

	pability->link = (jl_uint32)mac_is_link;
	pability->duplex = (jl_uint32)mac_duplex;
	pability->speed = (jl_uint32)mac_speed;
	pability->tx_pause = (jl_uint32)mac_tx_fc_en;
	pability->rx_pause = (jl_uint32)mac_rx_fc_en;
	pability->force_mode = 1;

	return JL_ERR_OK;
}

#define ETHMAC_REG_ADDR(idx, ofs)			(ETHMAC0_BASE + idx * ETHMAC0_BLOCK_SIZE + ofs)
jl_ret_t jl51xx_port_mac_loopback_set(jl_port_t port, jl_enable_t en_loopback)
{
	jl_ret_t ret;
	jl_uint32 val;
	jl_uint32 regaddr;

	regaddr = ETHMAC_REG_ADDR(JL_PORT_P2M(port), MAC_MODER_OFFSET);

	ret = jl_apb_reg_read(regaddr, &val);
	if (ret)
		return ret;

	if (en_loopback == 1)
		SET_BIT(val, 3);
	else if (en_loopback == 0)
		CLR_BIT(val, 3);
	else
		return JL_ERR_PARAM;

	return jl_apb_reg_write(regaddr, val);
}

jl_ret_t jl51xx_port_mac_loopback_get(jl_port_t port, jl_enable_t *pen_loopback)
{
	jl_ret_t ret;
	jl_uint32 val;
	jl_uint32 regaddr;

	regaddr = ETHMAC_REG_ADDR(JL_PORT_P2M(port), MAC_MODER_OFFSET);

	ret = jl_apb_reg_read(regaddr, &val);
	if (ret)
		return ret;

	*pen_loopback = (val & BIT(3)) ? 1 : 0;

	return JL_ERR_OK;
}
