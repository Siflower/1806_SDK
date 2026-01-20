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
#include "jl51xx/reg_swcore.h"
#include "jl51xx/jl51xx_drv_stp.h"

jl_ret_t jl51xx_stp_port_egress_state_get(jl_port_t port, jl_stp_state_t *pstp_state)
{
	jl_ret_t ret;
	jl_uint64 stp;
	jl_uint32 src_port_tab_val[1];
	jl_uint32 reg_addr = SWCORE_BASE + EGRESS_SPANNING_TREE_STATE_OFFSET;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];
	jl_uint32 bit_offset = 3 * JL_PORT_P2M(port);

	ret = jl_apb_reg_read(reg_addr, &src_port_tab_val[0]);
	if (ret)
		return ret;

	//check the states
	jl_read_bits(preg_val, &stp, bit_offset, 3);

	*pstp_state = stp;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON,
			"%s reg=0x%x, val[0]=0x%x, stp=0x%x\n",
			__func__, reg_addr, src_port_tab_val[0], *pstp_state);
	return JL_ERR_OK;
}

jl_ret_t jl51xx_stp_port_egress_state_set(jl_port_t port, jl_stp_state_t stp_state)
{
	jl_ret_t ret;
	jl_uint64 stp;
	jl_uint32 src_port_tab_val[1];
	jl_uint32 reg_addr = SWCORE_BASE + EGRESS_SPANNING_TREE_STATE_OFFSET;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];
	jl_uint32 bit_offset = 3 * JL_PORT_P2M(port);

	ret = jl_apb_reg_read(reg_addr, &src_port_tab_val[0]);
	if (ret)
		return ret;

	stp = stp_state;
	jl_write_bits(preg_val, stp, bit_offset, 3);

	ret = jl_apb_reg_write(reg_addr, src_port_tab_val[0]);
	if (ret)
		return ret;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON,
			"%s reg=0x%x, val[0]=0x%x, counter=0x%x\n",
			__func__, reg_addr, src_port_tab_val[0], stp_state);
	return JL_ERR_OK;
}

jl_ret_t jl51xx_stp_port_ingress_state_set(jl_port_t port, jl_stp_state_t stp_state)
{
	jl_ret_t ret;
	jl_uint64 stp;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	stp = stp_state;
	jl_write_bits(preg_val, stp, 40, 3);

	ret = jl_apb_reg_burst_write(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON,
			"%s reg=0x%x, val[0]=0x%x, val[1]=0x%x, stp=0x%x\n",
			__func__, reg_addr, src_port_tab_val[0], src_port_tab_val[1], stp_state);
	return JL_ERR_OK;
}

jl_ret_t jl51xx_stp_port_ingress_state_get(jl_port_t port, jl_stp_state_t *pstp_state)
{
	jl_ret_t ret;
	jl_uint64 stp;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &stp, 40, 3);
	*pstp_state = (jl_stp_state_t)stp;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON,
			"%s reg=0x%x, val[0]=0x%x, val[1]=0x%x, stp=0x%x\n",
			__func__, reg_addr, src_port_tab_val[0], src_port_tab_val[1], *pstp_state);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_stp_ingress_drop_counter_get(jl_stp_state_t stp_state, jl_uint32 *pcntr)
{
	jl_ret_t ret;
	jl_uint64 counter;
	jl_uint32 src_port_tab_val[1];
	jl_uint32 reg_addr;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	if (stp_state == STP_STATE_BLOCKING)
		reg_addr = SWCORE_BASE + INGRESS_SPANNING_TREE_DROP_BLOCKING_OFFSET;
	else if (stp_state == STP_STATE_LISTENING)
		reg_addr = SWCORE_BASE + INGRESS_SPANNING_TREE_DROP_LISTEN_OFFSET;
	else if (stp_state == STP_STATE_LEARNING)
		reg_addr = SWCORE_BASE + INGRESS_SPANNING_TREE_DROP_LEARNING_OFFSET;
	else
		return JL_ERR_PARAM;

	ret = jl_apb_reg_read(reg_addr, &src_port_tab_val[0]);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &counter, 0, 24);
	*pcntr = (jl_uint32)counter;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON,
			"%s reg=0x%x, val[0]=0x%x, counter=0x%x\n",
			__func__, reg_addr, src_port_tab_val[0], *pcntr);
	return JL_ERR_OK;
}

jl_ret_t jl51xx_stp_port_egress_drop_counter_get(jl_port_t port, jl_uint32 *pcntr)
{
	jl_ret_t ret;
	jl_uint64 counter;
	jl_uint32 src_port_tab_val[1];
	jl_uint32 reg_addr = SWCORE_BASE + EGRESS_SPANNING_TREE_DROP_OFFSET + JL_PORT_P2M(port);
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_read(reg_addr, &src_port_tab_val[0]);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &counter, 0, 24);
	*pcntr = (jl_uint32)counter;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON,
			"%s reg=0x%x, val[0]=0x%x, counter=0x%x\n",
			__func__, reg_addr, src_port_tab_val[0], *pcntr);

	return JL_ERR_OK;
}
