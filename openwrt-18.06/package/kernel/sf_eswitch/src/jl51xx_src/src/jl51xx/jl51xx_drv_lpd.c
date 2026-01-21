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

#ifndef __KERNEL__
#include <string.h>
#else
#include <linux/string.h>
#endif
#include "jl_base.h"

typedef struct _jl_lpd_config_s {
	jl_mac_t src_mac;
	jl_uint16 eth_type;
	jl_uint16 op_code;
	jl_uint16 ttl;
} _jl_lpd_config_t;


jl_ret_t jl51xx_lpd_ctrl_set(void *lpd_config)
{
	jl_ret_t ret;
	jl_port_t mac_port;
	_jl_lpd_config_t *config = (_jl_lpd_config_t *)lpd_config;
	jl_uint64 tx_flow, lp_frm_etype, lp_frm_opcode, lp_frm_ttl;
	jl_uint32 txloop_frm_ctrl_val[2] = {0};
	jl_uint32 mac_addr_val[2] = {0};
	jl_uint32 ctrl_moder_val[1] = {0};
	jl_uint32 reg_addr = 0;
	jl_uint8 * preg_val = NULL;

	JL_FOR_EACH_MAC_PORT(mac_port) {
		reg_addr = ETHMAC0_BASE + ETHMAC0_BLOCK_SIZE * mac_port + CTRL_MODER_OFFSET;
		ret = jl_apb_reg_burst_read(reg_addr, &ctrl_moder_val[0], 1);
		if (ret)
			return ret;
		preg_val = (jl_uint8 *)&ctrl_moder_val[0];
		tx_flow = 1;
		jl_write_bits(preg_val, tx_flow, 2, 1);
		ret = jl_apb_reg_burst_write(reg_addr, &ctrl_moder_val[0], 1);
		if (ret)
			return ret;

		reg_addr = ETHMAC0_BASE + ETHMAC0_BLOCK_SIZE * mac_port + TXLOOP_FRAME_CONTROL_OFFSET;
		ret = jl_apb_reg_burst_read(reg_addr, &txloop_frm_ctrl_val[0], 2);
		if (ret)
			return ret;
		preg_val = (jl_uint8 *)&txloop_frm_ctrl_val[0];
		lp_frm_etype = config->eth_type;
		lp_frm_opcode = config->op_code;
		lp_frm_ttl = config->ttl;
		jl_write_bits(preg_val, lp_frm_etype, 0, 16);
		jl_write_bits(preg_val, lp_frm_opcode, 16, 16);
		jl_write_bits(preg_val, lp_frm_ttl, 32, 16);
		ret = jl_apb_reg_burst_write(reg_addr, &txloop_frm_ctrl_val[0], 2);
		if (ret)
			return ret;

		reg_addr = ETHMAC0_BASE + ETHMAC0_BLOCK_SIZE * mac_port + MAC_ADDR_OFFSET;
		preg_val = (jl_uint8 *)&mac_addr_val[0];
		preg_val[0]=config->src_mac.octet[5];
		preg_val[1]=config->src_mac.octet[4];
		preg_val[2]=config->src_mac.octet[3];
		preg_val[3]=config->src_mac.octet[2];
		preg_val[4]=config->src_mac.octet[1];
		preg_val[5]=config->src_mac.octet[0];
		ret = jl_apb_reg_burst_write(reg_addr, &mac_addr_val[0], 2);
		if (ret)
			return ret;
	}

	return JL_ERR_OK;
}

jl_ret_t jl51xx_lpd_ctrl_get(void *lpd_config)
{
	jl_ret_t ret;
	jl_port_t mac_port;
	_jl_lpd_config_t *config = (_jl_lpd_config_t *)lpd_config;
	jl_uint64 lp_frm_etype, lp_frm_opcode, lp_frm_ttl;
	jl_uint32 txloop_frm_ctrl_val[2] = {0};
	jl_uint32 mac_addr_val[2] = {0};
	jl_uint32 reg_addr = 0;
	jl_uint8 * preg_val = NULL;

	JL_FOR_EACH_MAC_PORT(mac_port) {
		reg_addr = ETHMAC0_BASE + ETHMAC0_BLOCK_SIZE * mac_port + TXLOOP_FRAME_CONTROL_OFFSET;
		ret = jl_apb_reg_burst_read(reg_addr, &txloop_frm_ctrl_val[0], 2);
		if (ret)
			return ret;
		preg_val = (jl_uint8 *)&txloop_frm_ctrl_val[0];
		jl_read_bits(preg_val, &lp_frm_etype, 0, 16);
		jl_read_bits(preg_val, &lp_frm_opcode, 16, 16);
		jl_read_bits(preg_val, &lp_frm_ttl, 32, 16);
		config->eth_type = (jl_uint16)lp_frm_etype;
		config->op_code = (jl_uint16)lp_frm_opcode;
		config->ttl = (jl_uint16)lp_frm_ttl;

		reg_addr = ETHMAC0_BASE + ETHMAC0_BLOCK_SIZE * mac_port + MAC_ADDR_OFFSET;
		ret = jl_apb_reg_burst_read(reg_addr, &mac_addr_val[0], 2);
		if (ret)
			return ret;
		memcpy(&config->src_mac.octet[0], &mac_addr_val[0], ETHER_ADDR_LEN);

		break;
	}

	return JL_ERR_OK;
}

jl_ret_t __jl51xx_lpd_enable(jl_uint8 enable)
{
	jl_ret_t ret;
	jl_uint64 lp_detect_en;
	jl_uint32 lpd_ctrl_val[1];
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = (jl_uint8 *)&lpd_ctrl_val[0];

	reg_addr = MAG_BASE + LOOP_DETECT_CONTROL_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &lpd_ctrl_val[0], 1);
	if (ret)
		return ret;

	if (enable)
		lp_detect_en = JL_MAC_PORTMASK;
	else
		lp_detect_en = 0;
	jl_write_bits(preg_val, lp_detect_en, 0, 10);

	ret = jl_apb_reg_burst_write(reg_addr, &lpd_ctrl_val[0], 1);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_lpd_enable(void)
{
	return __jl51xx_lpd_enable(1);
}
jl_ret_t jl51xx_lpd_disable(void)
{
	return __jl51xx_lpd_enable(0);
}

jl_ret_t jl51xx_lpd_trigger_check(void)
{
	jl_ret_t ret;
	jl_uint64 lp_detect_chk;
	jl_uint32 lpd_ctrl_val[1];
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = (jl_uint8 *)&lpd_ctrl_val[0];

	reg_addr = MAG_BASE + LOOP_DETECT_CONTROL_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &lpd_ctrl_val[0], 1);
	if (ret)
		return ret;

	lp_detect_chk = 1;
	jl_write_bits(preg_val, lp_detect_chk, 12, 1);

	ret = jl_apb_reg_burst_write(reg_addr, &lpd_ctrl_val[0], 1);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_lpd_trigger_once(jl_port_t port)
{
	jl_ret_t ret;
	jl_uint8 count = 10;
	jl_uint64 tx_pause_en, tx_pause_rq;
	jl_uint32 txpause_frm_ctrl_val[1];
	jl_uint32 reg_addr = ETHMAC0_BASE + ETHMAC0_BLOCK_SIZE * JL_PORT_P2M(port) + TXPAUSE_FRAME_CONTROL_OFFSET;
	jl_uint8 * preg_val = (jl_uint8 *)&txpause_frm_ctrl_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &txpause_frm_ctrl_val[0], 1);
	if (ret)
		return ret;

	/*
	 * insertion control frame selection: tx_pause_en
	 * 0 for loop frame
	 * 1 for pause control frame
	 */
	tx_pause_rq = 1;
	tx_pause_en = 0;
	jl_write_bits(preg_val, tx_pause_rq, 16, 1);
	jl_write_bits(preg_val, tx_pause_en, 17, 1);

	ret = jl_apb_reg_burst_write(reg_addr, &txpause_frm_ctrl_val[0], 1);
	if (ret)
		return ret;

	/* wait loop frame send successfully */
	while (--count) {
		port_udelay(1);
		ret = jl_apb_reg_burst_read(reg_addr, &txpause_frm_ctrl_val[0], 1);
		if (ret)
			return ret;

		jl_read_bits(preg_val, &tx_pause_rq, 16, 1);

		if (!tx_pause_rq)
			break;
	}

	if (!count)
		return JL_ERR_TIMEOUT;

	/* delay 1us before loop detect check */
	port_udelay(1);
	jl51xx_lpd_trigger_check();

	return JL_ERR_OK;
}

jl_ret_t jl51xx_lpd_ports_get(jl_portmask_t *port_mask)
{
	jl_ret_t ret;
	jl_uint64 lp_detect;
	jl_uint32 lpd_stat_val[1];
	jl_uint32 reg_addr = MAG_BASE + LOOP_DETECT_STATUS_OFFSET;
	jl_uint8 *preg_val = (jl_uint8 *)&lpd_stat_val[0];

	ret = jl_apb_reg_burst_read(reg_addr,
				&lpd_stat_val[0], 1);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &lp_detect, 0, 10);

	*port_mask = JL_PORTMASK_M2P(lp_detect);

	return JL_ERR_OK;
}
