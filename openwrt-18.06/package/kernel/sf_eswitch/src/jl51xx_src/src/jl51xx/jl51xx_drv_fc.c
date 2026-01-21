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
#include "jl51xx/jl51xx_drv_fc.h"

#define CELLS_MASK					0x3FF //bit0-9

#define TAILDROP_DROP_COUNTER_MASK	0xFFFFFF //bit0-23

jl_ret_t jl51xx_taildrop_global_ffa_threshold_set(jl_enable_t enable, jl_uint32 cells)
{
	jl_ret_t ret;
	jl_uint32 reg_addr;
	jl_uint32 val;

	if (cells > CELLS_MASK)
		cells = CELLS_MASK;

	reg_addr = SWCORE_BASE + TAIL_DROP_FFA_THRESHOLD_OFFSET;

	ret = jl_apb_reg_read(reg_addr, &val);
	if (ret)
		return ret;
	if (enable == ENABLED) {
		SET_BIT(val, 10);
		CLR_BITS(val, 0, 9);
		val |= (cells & CELLS_MASK);
	} else if (enable == DISABLED) {
		CLR_BITS(val, 0, 10);
		val |= (cells & CELLS_MASK);
	} else
		return JL_ERR_PARAM;

	JL_DBG_MSG(JL_FLAG_ALL, _DBG_ON,
			"%s reg:0x%x, val: 0x%x, cells:0x%x\n",
			__func__, reg_addr, val, cells);

	return jl_apb_reg_write(reg_addr, val);
}

jl_ret_t jl51xx_taildrop_global_ffa_threshold_get(jl_enable_t *penable, jl_uint32 *pcells)
{
	jl_ret_t ret;
	jl_uint32 reg_addr;
	jl_uint32 val;

	if (penable == NULL || pcells == NULL)
		return JL_ERR_OK;

	reg_addr = SWCORE_BASE + TAIL_DROP_FFA_THRESHOLD_OFFSET;

	ret = jl_apb_reg_read(reg_addr, &val);
	if (ret)
		return ret;

	*pcells = val & CELLS_MASK;

	if (val & BIT(10))
		*penable = ENABLED;
	else
		*penable = DISABLED;

	JL_DBG_MSG(JL_FLAG_ALL, _DBG_ON,
			"%s reg:0x%x, val: 0x%x, cells:0x%x, enable=0x%x\n",
			__func__, reg_addr, val, *pcells, *penable);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_taildrop_port_ffa_threshold_set(jl_port_t phyport, jl_enable_t enable, jl_uint32 cells)
{
	jl_ret_t ret;
	jl_uint32 reg_addr;
	jl_uint32 val;
	jl_port_t mac_port = JL_PORT_P2M(phyport);

	reg_addr = SWCORE_BASE + PORT_TAIL_DROP_FFA_THRESHOLD_OFFSET + mac_port;

	if (cells > CELLS_MASK)
		cells = CELLS_MASK;

	ret = jl_apb_reg_read(reg_addr, &val);
	if (ret)
		return ret;

	if (enable == ENABLED) {
		SET_BIT(val, 10);
		CLR_BITS(val, 0, 9);
		val |= (cells & CELLS_MASK);
	} else if (enable == DISABLED) {
		CLR_BITS(val, 0, 10);
		val |= (cells & CELLS_MASK);
	} else
		return JL_ERR_PARAM;

	JL_DBG_MSG(JL_FLAG_ALL, _DBG_ON,
			"%s reg:0x%x, val: 0x%x, cells:0x%x\n",
			__func__, reg_addr, val, cells);
	return jl_apb_reg_write(reg_addr, val);
}

jl_ret_t jl51xx_taildrop_port_ffa_threshold_get(jl_port_t phyport, jl_enable_t *penable, jl_uint32 *pcells)
{
	jl_ret_t ret;
	jl_uint32 reg_addr;
	jl_uint32 val;
	jl_port_t mac_port = JL_PORT_P2M(phyport);

	if (penable == NULL || pcells == NULL)
		return JL_ERR_OK;

	reg_addr = SWCORE_BASE + PORT_TAIL_DROP_FFA_THRESHOLD_OFFSET + mac_port;

	ret = jl_apb_reg_read(reg_addr, &val);
	if (ret)
		return ret;

	*pcells = val & CELLS_MASK;

	if (val & BIT(10))
		*penable = ENABLED;
	else
		*penable = DISABLED;

	JL_DBG_MSG(JL_FLAG_ALL, _DBG_ON,
			"%s reg:0x%x, val: 0x%x, cells:0x%x, enable=0x%x\n",
			__func__, reg_addr, val, *pcells, *penable);
	return JL_ERR_OK;
}

jl_ret_t jl51xx_taildrop_enable_set(jl_port_t phyport, jl_enable_t enable)
{
	jl_ret_t ret;
	jl_uint32 reg_addr;
	jl_uint32 val;
	jl_port_t mac_port = JL_PORT_P2M(phyport);

	reg_addr = SWCORE_BASE + PORT_TAIL_DROP_SETTINGS_OFFSET + mac_port;

	ret = jl_apb_reg_read(reg_addr, &val);
	if (ret)
		return ret;
	if (enable == ENABLED)
		SET_BIT(val, 0);
	else if (enable == DISABLED)
		CLR_BIT(val, 0);
	else
		return JL_ERR_PARAM;

	JL_DBG_MSG(JL_FLAG_ALL, _DBG_ON,
			"%s reg:0x%x, val: 0x%x\n",
			__func__, reg_addr, val);
	return jl_apb_reg_write(reg_addr, val);
}

jl_ret_t jl51xx_taildrop_enable_get(jl_port_t phyport, jl_enable_t *penable)
{
	jl_ret_t ret;
	jl_uint32 reg_addr;
	jl_uint32 val;
	jl_port_t mac_port = JL_PORT_P2M(phyport);

	if (penable == NULL)
		return JL_ERR_OK;

	reg_addr = SWCORE_BASE + PORT_TAIL_DROP_SETTINGS_OFFSET + mac_port;

	ret = jl_apb_reg_read(reg_addr, &val);
	if (ret)
		return ret;

	JL_DBG_MSG(JL_FLAG_ALL, _DBG_ON,
			"%s reg:0x%x, val: 0x%x\n",
			__func__, reg_addr, val);

	if (val & BIT(0))
		*penable = ENABLED;
	else
		*penable = DISABLED;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_ingress_resource_manager_drop_counter_get(jl_uint32 *pcntr)
{
	jl_ret_t ret;
	jl_uint32 reg_addr;
	jl_uint32 val;

	if (pcntr == NULL)
		return JL_ERR_OK;

	reg_addr = SWCORE_BASE + INGRESS_RESOURCE_MANAGER_DROP_OFFSET;

	ret = jl_apb_reg_read(reg_addr, &val);
	if (ret)
		return ret;

	*pcntr = val & TAILDROP_DROP_COUNTER_MASK;

	JL_DBG_MSG(JL_FLAG_ALL, _DBG_ON,
			"%s reg:0x%x, val: 0x%x, counter=0x%x\n",
			__func__, reg_addr, val, *pcntr);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_fc_enable_set(jl_port_t port, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint64 tx_flow, rx_flow, tx_pause_en;
	jl_uint32 port_pause_set_val[1] = {0};
	jl_uint32 ctrl_moder_val[1] = {0};
	jl_uint32 tx_pause_frm_ctrl_val[1] = {0};
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = NULL;

	JL_ASSERT(enable < JL_ENABLE_END);

	if (enable == ENABLED) {
		rx_flow = 1;
		tx_flow = 1;
	    tx_pause_en = 1;
	} else {
		rx_flow = 0; //default is 0
		tx_flow = 1; //default is 1
		tx_pause_en = 0;

	}

	do {
		reg_addr = SWCORE_BASE + PORT_PAUSE_SETTINGS_OFFSET + JL_PORT_P2M(port) * 1;
		preg_val = (jl_uint8 *)&port_pause_set_val[0];

		ret = jl_apb_reg_burst_read(reg_addr, &port_pause_set_val[0], 1);
		if (ret)
			return ret;

		jl_write_bits(preg_val, (jl_uint64)enable, 0, 1);

		ret = jl_apb_reg_burst_write(reg_addr, &port_pause_set_val[0], 1);
		if (ret)
			return ret;
	} while (0);

	do {
		reg_addr = ETHMAC0_BASE + ETHMAC0_BLOCK_SIZE * JL_PORT_P2M(port) + CTRL_MODER_OFFSET;
		preg_val = (jl_uint8 *)&ctrl_moder_val[0];

		ret = jl_apb_reg_burst_read(reg_addr, &ctrl_moder_val[0], 1);
		if (ret)
			return ret;

		jl_write_bits(preg_val, rx_flow, 1, 1);
		jl_write_bits(preg_val, tx_flow, 2, 1);

		ret = jl_apb_reg_burst_write(reg_addr, &ctrl_moder_val[0], 1);
		if (ret)
			return ret;
	} while (0);

	do {
		reg_addr = ETHMAC0_BASE + ETHMAC0_BLOCK_SIZE * JL_PORT_P2M(port) + TXPAUSE_FRAME_CONTROL_OFFSET;
		preg_val = (jl_uint8 *)&tx_pause_frm_ctrl_val[0];

		ret = jl_apb_reg_burst_read(reg_addr, &tx_pause_frm_ctrl_val[0], 1);
		if (ret)
			return ret;

		jl_write_bits(preg_val, tx_pause_en, 17, 1);

		ret = jl_apb_reg_burst_write(reg_addr, &tx_pause_frm_ctrl_val[0], 1);
		if (ret)
			return ret;
	} while (0);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_fc_enable_get(jl_port_t port, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 port_pause_set_val = 0;
	jl_uint32 reg_addr = 0;

	reg_addr = SWCORE_BASE + PORT_PAUSE_SETTINGS_OFFSET + JL_PORT_P2M(port) * 1;
	ret = jl_apb_reg_read(reg_addr, &port_pause_set_val);
	if (ret)
		return ret;

	*penable = port_pause_set_val & 0x1;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_fc_threshold_set(jl_port_t port,
			jl_uint32 threshold_on, jl_uint32 threshold_off)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint64 cells = 0;
	jl_uint32 port_xon_ffa_thresold_val[1] = {0};
	jl_uint32 port_xoff_ffa_threshold_val[1] = {0};
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = NULL;

	if (threshold_on >= threshold_off)
		return JL_ERR_PARAM;

	do {
		reg_addr = SWCORE_BASE + PORT_XON_FFA_THRESHOLD_OFFSET + JL_PORT_P2M(port) * 1;
		preg_val = (jl_uint8 *)&port_xon_ffa_thresold_val[0];

		ret = jl_apb_reg_burst_read(reg_addr, &port_xon_ffa_thresold_val[0], 1);
		if (ret)
			return ret;

		cells = threshold_on;
		jl_write_bits(preg_val, cells, 0, 10);

		ret = jl_apb_reg_burst_write(reg_addr, &port_xon_ffa_thresold_val[0], 1);
		if (ret)
			return ret;
	} while (0);

	do {
		reg_addr = SWCORE_BASE + PORT_XOFF_FFA_THRESHOLD_OFFSET + JL_PORT_P2M(port) * 1;
		preg_val = (jl_uint8 *)&port_xoff_ffa_threshold_val[0];

		ret = jl_apb_reg_burst_read(reg_addr, &port_xoff_ffa_threshold_val[0], 1);
		if (ret)
			return ret;

		cells  = threshold_off;
		jl_write_bits(preg_val, cells, 0, 10);

		ret = jl_apb_reg_burst_write(reg_addr, &port_xoff_ffa_threshold_val[0], 1);
		if (ret)
			return ret;
	} while (0);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_fc_threshold_get(jl_port_t port,
			jl_uint32 *pthreshold_on, jl_uint32 *pthreshold_off)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint64 cells = 0;
	jl_uint32 port_xon_ffa_thresold_val[1] = {0};
	jl_uint32 port_xoff_ffa_threshold_val[1] = {0};
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = NULL;

	reg_addr = SWCORE_BASE + PORT_XON_FFA_THRESHOLD_OFFSET + JL_PORT_P2M(port) * 1;
	preg_val = (jl_uint8 *)&port_xon_ffa_thresold_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &port_xon_ffa_thresold_val[0], 1);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &cells, 0, 10);
	*pthreshold_on = cells;

	reg_addr = SWCORE_BASE + PORT_XOFF_FFA_THRESHOLD_OFFSET + JL_PORT_P2M(port) * 1;
	preg_val = (jl_uint8 *)&port_xoff_ffa_threshold_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &port_xoff_ffa_threshold_val[0], 1);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &cells, 0, 10);
	*pthreshold_off = cells;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_fc_pause_time_set(jl_port_t port, jl_uint32 pause_time)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint64 tx_pause_tv;
	jl_uint32 tx_pause_frm_ctrl_val[1] = {0};
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = NULL;

	if (pause_time > 0xffff)
		return JL_ERR_PARAM;

	reg_addr = ETHMAC0_BASE + ETHMAC0_BLOCK_SIZE * JL_PORT_P2M(port) + TXPAUSE_FRAME_CONTROL_OFFSET;
	preg_val = (jl_uint8 *)&tx_pause_frm_ctrl_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &tx_pause_frm_ctrl_val[0], 1);
	if (ret)
		return ret;

	tx_pause_tv = pause_time;
	jl_write_bits(preg_val, tx_pause_tv, 0, 16);

	ret = jl_apb_reg_burst_write(reg_addr, &tx_pause_frm_ctrl_val[0], 1);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_fc_pause_time_get(jl_port_t port, jl_uint32 *ppause_time)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint64 tx_pause_tv;
	jl_uint32 tx_pause_frm_ctrl_val[1] = {0};
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = NULL;

	reg_addr = ETHMAC0_BASE + ETHMAC0_BLOCK_SIZE * JL_PORT_P2M(port) + TXPAUSE_FRAME_CONTROL_OFFSET;
	preg_val = (jl_uint8 *)&tx_pause_frm_ctrl_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &tx_pause_frm_ctrl_val[0], 1);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &tx_pause_tv, 0, 16);
	*ppause_time = tx_pause_tv;

	return JL_ERR_OK;
}

