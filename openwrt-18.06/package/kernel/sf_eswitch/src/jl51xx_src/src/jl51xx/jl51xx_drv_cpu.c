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
#include "driver/jl_reg_io.h"
#include "jl51xx/jl51xx_drv_cpu.h"
#include "jl51xx/reg_swcore.h"

jl_ret_t jl51xx_cpu_allow_bpdu_enable_set(jl_enable_t enable)
{
	jl_ret_t ret;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SEND_TO_CPU_OFFSET;

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	if (enable == ENABLED)
		SET_BIT(src_port_tab_val[0], 0);
	else if (enable == DISABLED)
		CLR_BIT(src_port_tab_val[0], 0);
	else
		return JL_ERR_PARAM;

	ret = jl_apb_reg_burst_write(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON,
			"%s reg=0x%x, val[0]=0x%x, enable=0x%x\n",
			__func__, reg_addr, src_port_tab_val[0], enable);
	return JL_ERR_OK;
}

jl_ret_t jl51xx_cpu_allow_bpdu_enable_get(jl_enable_t *penable)
{
	jl_ret_t ret;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SEND_TO_CPU_OFFSET;

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	if ((src_port_tab_val[0] & BIT(0)) == 1)
		*penable = 1;
	else
		*penable = 0;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON,
			"%s reg=0x%x, val[0]=0x%x, enable=0x%x\n",
			__func__, reg_addr, src_port_tab_val[0], *penable);
	return JL_ERR_OK;
}

jl_ret_t jl51xx_cpu_allow_rst_bpdu_enable_set(jl_enable_t enable)
{
	jl_ret_t ret;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SEND_TO_CPU_OFFSET;

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	if (enable == ENABLED)
		SET_BIT(src_port_tab_val[0], 1);
	else if (enable == DISABLED)
		CLR_BIT(src_port_tab_val[0], 1);
	else
		return JL_ERR_PARAM;

	ret = jl_apb_reg_burst_write(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON,
			"%s reg=0x%x, val[0]=0x%x, enable=0x%x\n",
			__func__, reg_addr, src_port_tab_val[0], enable);
	return JL_ERR_OK;
}

jl_ret_t jl51xx_cpu_allow_rst_bpdu_enable_get(jl_enable_t *penable)
{
	jl_ret_t ret;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SEND_TO_CPU_OFFSET;

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	if ((src_port_tab_val[0] & BIT(1)) == BIT(1))
		*penable = 1;
	else
		*penable = 0;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON,
			"%s reg=0x%x, val[0]=0x%x, enable=0x%x\n",
			__func__, reg_addr, src_port_tab_val[0], *penable);
	return JL_ERR_OK;
}

jl_ret_t jl51xx_to_cpu_tag_enable_set(jl_enable_t enable)
{
	jl_ret_t ret;
	jl_uint32 disable_cpu_port[1];
	jl_uint32 reg_addr = SWCORE_BASE + DISABLE_CPU_TAG_ON_CPU_PORT_OFFSET;

	ret = jl_apb_reg_read(reg_addr, &disable_cpu_port[0]);
	if (ret)
		return ret;

	if (enable == ENABLED)
		CLR_BIT(disable_cpu_port[0], 0);
	else if (enable == DISABLED)
		SET_BIT(disable_cpu_port[0], 0);
	else
		return JL_ERR_PARAM;

	ret = jl_apb_reg_write(reg_addr, disable_cpu_port[0]);
	if (ret)
		return ret;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON,
			"%s regaddr=0x%x, regval=0x%x, enable=0x%x\n",
			__func__, reg_addr, disable_cpu_port[0], enable);
	return JL_ERR_OK;
}

jl_ret_t jl51xx_to_cpu_tag_enable_get(jl_enable_t *penable)
{
	jl_ret_t ret;
	jl_uint32 disable_cpu_port[1];
	jl_uint32 reg_addr = SWCORE_BASE + DISABLE_CPU_TAG_ON_CPU_PORT_OFFSET;

	ret = jl_apb_reg_read(reg_addr, &disable_cpu_port[0]);
	if (ret)
		return ret;

	if (disable_cpu_port[0] & BIT(0))
		*penable = 0;
	else
		*penable = 1;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON,
			"%s regaddr=0x%x, regval=0x%x, enable=0x%x\n",
			__func__, reg_addr, disable_cpu_port[0], *penable);
	return JL_ERR_OK;
}

jl_ret_t jl51xx_cpu_allow_lldp_enable_set(jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 val = 0;
	jl_uint32 portmask_offset = 5;
	jl_uint32 reg_addr = SWCORE_BASE + LLDP_CONFIGURATION_OFFSET + portmask_offset;

	ret = jl_apb_reg_read(reg_addr, &val);
	if (ret)
		return ret;

	if (enable == ENABLED)
		SET_BITS(val, 0, 8);
	else if (enable == DISABLED)
		CLR_BITS(val, 0, 8);
	else
		return JL_ERR_PARAM;

	ret = jl_apb_reg_write(reg_addr, val);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_cpu_allow_lldp_enable_get(jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 val = 0;
	jl_uint32 portmask_offset = 5;
	jl_uint32 reg_addr = SWCORE_BASE + LLDP_CONFIGURATION_OFFSET + portmask_offset;

	ret = jl_apb_reg_read(reg_addr, &val);
	if (ret)
		return ret;

	if (GET_BITS(val, 0, 8) == 0x1ff)
		*penable = 1;
	else
		*penable = 0;

	return JL_ERR_OK;
}

