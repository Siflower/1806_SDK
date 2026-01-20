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
#include "jl51xx/jl51xx_drv_storm.h"
#include "jl51xx/jl51xx_drv_l2.h"

enum MBSC_MODE {
	MBSC_BYTE_MODE = 0,
	MBSC_PKT_MODE = 1,
};

#define STORM_REG_BASE		(SWCORE_BASE + L2_BROADCAST_STORM_CONTROL_RATE_CONFIGURATION_OFFSET)
#define STORM_BLOCK_SIZE		(31)
#define STORM_RATE_OFFSET		(0)
#define STORM_CAP_OFFSET		(10)
#define STORM_THR_OFFSET		(20)
#define STORM_ENABLE_OFFSET		(30)

#define TICK_NUM				(5)
#define MIN_LEN					(64)
#define SWITCH_SLICE			(1)
//6.25MHz
#define SYSTEM_TICK			(6250000)
//default tick freq(Hz), rounding off
jl_uint32 tick_freq_list[TICK_NUM] = {625000, 62500, 6250, 625, 62};

static jl_uint32 __power(jl_uint8 base, jl_uint8 powerRaised)
{
	if (powerRaised != 0)
		return (base*__power(base, powerRaised-1));
	else
		return 1;
}

static jl_ret_t __calc_tick_freq(jl_uint16 *div, jl_uint8 *step)
{
	jl_ret_t ret;
	jl_uint8 i = 0;
	jl_uint32 result = 0;

	if ((ret = jl51xx_l2_tick_configuration_get(
					div, step)) != JL_ERR_OK)
		return ret;

	//default configure
	if (*div == 0xa && *step == 0xa)
		return JL_ERR_OK;

	for (i = 0; i < TICK_NUM; i++) {
		result = __power(*step, i);
		tick_freq_list[i] = SYSTEM_TICK/((*div) * result);
	}

	return JL_ERR_OK;
}

static jl_ret_t __storm_control_config_set(jl_port_t mac_port, jl_storm_type_t type, jl_uint8 mode, jl_uint16 token_in, jl_uint16 tick_in, jl_uint16 bucket_cap, jl_uint16 bucket_thr)
{
	jl_ret_t ret;
	jl_uint32 reg;
	jl_uint32 val_tab[1];
	jl_uint8 *ptr = NULL;
	jl_uint64 packets_not_bytes = mode;
	jl_uint64 tokens = token_in;
	jl_uint64 tick = tick_in;
	jl_uint64 capability = bucket_cap;
	jl_uint64 threshold = bucket_thr;

	reg = STORM_REG_BASE + STORM_BLOCK_SIZE * type + mac_port;
	//Set storm rate configuration
	do {
		ptr = (jl_uint8 *)&val_tab[0];
		if ((ret = jl_apb_reg_read(reg + STORM_RATE_OFFSET,
					&val_tab[0])) != JL_ERR_OK)
			return ret;

		JL_DBG_MSG(JL_FLAG_ALL, _DBG_ON,
			"%s addr=0x%x, val=0x%x, tick=%llu, tokens=%llu\n",
			__func__, reg + STORM_RATE_OFFSET,
			val_tab[0], tick, tokens);
		jl_write_bits(ptr, packets_not_bytes, 0, 1);
		jl_write_bits(ptr, tokens, 1, 12);
		jl_write_bits(ptr, tick, 13, 3);

		if ((ret = jl_apb_reg_write(reg + STORM_RATE_OFFSET,
					val_tab[0])) != JL_ERR_OK)
			return ret;

	} while(0);

	//Set storm bucket capacity
	do {
		ptr = (jl_uint8 *)&val_tab[0];
		if ((ret = jl_apb_reg_read(reg + STORM_CAP_OFFSET,
					&val_tab[0])) != JL_ERR_OK)
			return ret;

		jl_write_bits(ptr, capability, 0, 16);

		JL_DBG_MSG(JL_FLAG_ALL, _DBG_ON,
			"%s addr=0x%x, val=0x%x, capa=%llu\n",
			__func__, reg + STORM_CAP_OFFSET,
			val_tab[0], capability);

		if ((ret = jl_apb_reg_write(reg + STORM_CAP_OFFSET,
					val_tab[0])) != JL_ERR_OK)
			return ret;
	} while(0);

	do {
		ptr = (jl_uint8 *)&val_tab[0];
		if ((ret = jl_apb_reg_read(reg + STORM_THR_OFFSET,
					&val_tab[0])) != JL_ERR_OK)
			return ret;

		JL_DBG_MSG(JL_FLAG_ALL, _DBG_ON,
			"%s addr=0x%x, val=0x%x, capa=%llu\n",
			__func__, reg + STORM_THR_OFFSET,
			val_tab[0], threshold);

		jl_write_bits(ptr, threshold, 0, 16);

		if ((ret = jl_apb_reg_write(reg + STORM_THR_OFFSET,
					val_tab[0])) != JL_ERR_OK)
			return ret;
	} while(0);

	return JL_ERR_OK;
}

static jl_ret_t __storm_control_config_get(jl_port_t mac_port, jl_storm_type_t type, jl_uint8 *pmode, jl_uint16 *ptoken_in, jl_uint8 *ptick)
{
	jl_ret_t ret;
	jl_uint32 reg;
	jl_uint32 val_tab[1];
	jl_uint8 *ptr = NULL;
	jl_uint64 packets_not_bytes;
	jl_uint64 tokens;
	jl_uint64 tick;

	reg = STORM_REG_BASE + STORM_BLOCK_SIZE * type + mac_port;
	//get storm rate configuration
	do {
		ptr = (jl_uint8 *)&val_tab[0];
		if ((ret = jl_apb_reg_read(reg + STORM_RATE_OFFSET,
					&val_tab[0])) != JL_ERR_OK)
			return ret;

		jl_read_bits(ptr, &packets_not_bytes, 0, 1);
		jl_read_bits(ptr, &tokens, 1, 12);
		jl_read_bits(ptr, &tick, 13, 3);

		JL_DBG_MSG(JL_FLAG_ALL, _DBG_ON,
			"%s addr=0x%x, val=0x%x, tick=%llu\n",
			__func__, reg, val_tab[0], tick);

		*pmode = (jl_uint8)packets_not_bytes;
		*ptoken_in = (jl_uint16)tokens;
		*ptick = (jl_uint8)tick;
	} while(0);

	return JL_ERR_OK;
}

static jl_ret_t _storm_control_byte_rate_set(jl_port_t mac_port, jl_storm_type_t type, jl_rate_t rate)
{
	jl_ret_t ret;
	jl_uint16 div = 0;
	jl_uint8 step = 0, tick = 3, i = 0;
	jl_uint16 byte_token_in = 0;
	jl_uint16 bucket_cap = 0, bucket_thr = 0;
	jl_uint32 byte_rate = 0;

	ret = __calc_tick_freq(&div, &step);
	if (ret)
		return ret;

	byte_token_in = div * SWITCH_SLICE;
	//byte_rate = (rate * 1024)/8;
	byte_rate = rate << 7;

	for (i = 0; i < TICK_NUM; i++) {
		//precision: 5%
#ifndef __KERNEL__
		if (tick_freq_list[i] * byte_token_in <= 1.05 * (byte_rate))
#else
		if (tick_freq_list[i] * byte_token_in <= (105 * (byte_rate) / 100))
#endif
		{
			tick = i;
			byte_token_in = byte_rate/(tick_freq_list[tick]);
			bucket_cap = byte_token_in * 20;
			bucket_thr = byte_token_in * 10;
			break;
		}
	}

	//adjust, if cap is too small
	while (bucket_cap < 0x1000) {
		if (tick == TICK_NUM - 1) {
			bucket_cap = byte_token_in * 300;
			bucket_thr = byte_token_in * 150;
			break;
		}
		tick = tick + 1;
		byte_token_in = byte_rate/(tick_freq_list[tick]);

		bucket_cap = byte_token_in * 20;
		bucket_thr = byte_token_in * 10;
	}

	JL_DBG_MSG(JL_FLAG_ALL, _DBG_ON,
			"byteRate = %u, byteTokenIn=%u, tick=%u",
			byte_rate, byte_token_in, tick);

	return __storm_control_config_set(mac_port, type,
						(jl_uint8)MBSC_BYTE_MODE,
						byte_token_in, tick,
						bucket_cap,
						bucket_thr);
}

static jl_ret_t _storm_control_pkt_rate_set(jl_port_t mac_port, jl_storm_type_t type, jl_rate_t pkt_rate)
{
	jl_ret_t ret;
	jl_uint16 div = 0;
	jl_uint8 step = 0, tick = 0, i = 0;
	jl_uint16 pkt_token_in = 0;
	jl_uint16 bucket_cap = 0, bucket_thr = 0;

	ret = __calc_tick_freq(&div, &step);
	if (ret)
		return ret;

	pkt_token_in = div * SWITCH_SLICE;
	tick = TICK_NUM - 1;
	for (i = 0; i < TICK_NUM; i++) {
#ifndef __KERNEL__
		if (tick_freq_list[i] * pkt_token_in <= 1.05 * pkt_rate)
#else
		if (tick_freq_list[i] * pkt_token_in <= (105 * (pkt_rate) / 100))
#endif
		{
			tick = i;
			break;
		}
	}

	pkt_token_in = pkt_rate/(tick_freq_list[tick]);
	bucket_cap = pkt_token_in * 20;
	bucket_thr = pkt_token_in * 10;

	JL_DBG_MSG(JL_FLAG_ALL, _DBG_ON,
			"pktRate = %u, pktTokenIn=%u, tick=%u",
			pkt_rate, pkt_token_in, tick);

	return __storm_control_config_set(mac_port, type,
						(jl_uint8)MBSC_PKT_MODE,
						pkt_token_in, tick,
						bucket_cap,
						bucket_thr);
}

jl_ret_t jl51xx_storm_control_rate_set(jl_port_t egress_port, jl_storm_type_t type, jl_rate_unit_t unit, jl_rate_t rate)
{
	if (unit == RATE_UNIT_KBPS)
		return _storm_control_byte_rate_set(
				JL_PORT_P2M(egress_port),
				type,
				rate);
	else if (unit == RATE_UNIT_PPS)
		return _storm_control_pkt_rate_set(
				JL_PORT_P2M(egress_port),
				type,
				rate);
	else
		return JL_ERR_PARAM;
}

jl_ret_t jl51xx_storm_control_rate_get(jl_port_t egress_port, jl_storm_type_t type, jl_rate_unit_t *punit, jl_rate_t *prate)
{
	jl_ret_t ret;
	jl_uint8 mode;
	jl_uint16 token;
	jl_uint8 tick;
	jl_uint16 div;
	jl_uint8 step;

	ret = __storm_control_config_get(
				JL_PORT_P2M(egress_port),
				type, &mode, &token, &tick);
	if (ret)
		return ret;

	if ((ret = __calc_tick_freq(&div, &step)) != JL_ERR_OK)
		return ret;

	if (mode == 1) {
		//packet mode
		*punit = RATE_UNIT_PPS;
		*prate = token * tick_freq_list[tick];
	} else if (mode == 0) {
		//byte mode
		*punit = RATE_UNIT_KBPS;
		*prate = (token * tick_freq_list[tick]) >> 7;
	} else
		return JL_ERR_PARAM;

	JL_DBG_MSG(JL_FLAG_ALL, _DBG_ON,
			"mode=%u, token=%u, tick=%u, unit=%u, rate=%u\n",
			mode, token, tick, *punit, *prate);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_storm_control_enable_set(jl_port_t egress_port, jl_storm_type_t type, jl_enable_t enable)
{
	jl_ret_t ret;
	jl_port_t mac_port = JL_PORT_P2M(egress_port);
	jl_uint32 reg_addr;
	jl_uint32 val = 0;

	if (type == STORM_TYPE_UNKNOWN)
		reg_addr = L2_FLOODING_STORM_CONTROL_ENABLE_OFFSET;
	else if (type == STORM_TYPE_MULTICAST)
		reg_addr = L2_MULTICAST_STORM_CONTROL_ENABLE_OFFSET;
	else if (type == STORM_TYPE_BROADCAST)
		reg_addr = L2_BROADCAST_STORM_CONTROL_ENABLE_OFFSET;
	else
		return JL_ERR_PARAM;
	reg_addr += SWCORE_BASE;

	ret = jl_apb_reg_read(reg_addr, &val);
	if (ret)
		return ret;

	if (enable == 1)
		SET_BIT(val, mac_port);
	else if (enable == 0)
		CLR_BIT(val, mac_port);
	else
		return ret;

	JL_DBG_MSG(JL_FLAG_ALL, _DBG_ON,
			"%s addr=0x%x, val=0x%x, mac_port=%u\n",
			__func__, reg_addr, val, mac_port);
	return jl_apb_reg_write(reg_addr, val);
}

jl_ret_t jl51xx_storm_control_enable_get(jl_port_t egress_port, jl_storm_type_t type, jl_enable_t *penable)
{
	jl_ret_t ret;
	jl_port_t mac_port = JL_PORT_P2M(egress_port);
	jl_uint32 reg_addr;
	jl_uint32 val = 0;

	if (type == STORM_TYPE_UNKNOWN)
		reg_addr = L2_FLOODING_STORM_CONTROL_ENABLE_OFFSET;
	else if (type == STORM_TYPE_MULTICAST)
		reg_addr = L2_MULTICAST_STORM_CONTROL_ENABLE_OFFSET;
	else if (type == STORM_TYPE_BROADCAST)
		reg_addr = L2_BROADCAST_STORM_CONTROL_ENABLE_OFFSET;
	else
		return JL_ERR_PARAM;
	reg_addr += SWCORE_BASE;

	ret = jl_apb_reg_read(reg_addr, &val);
	if (ret)
		return ret;

	if (val & BIT(mac_port))
		*penable = 1;
	else
		*penable = 0;

	JL_DBG_MSG(JL_FLAG_ALL, _DBG_ON,
			"%s addr=0x%x, val=0x%x, mac_port=%u\n",
			__func__, reg_addr, val, mac_port);
	return JL_ERR_OK;
}

jl_ret_t jl51xx_storm_control_drop_counter_get(jl_port_t egress_port, jl_uint32 *pcntr)
{
	jl_ret_t ret;
	jl_uint32 reg;
	jl_uint32 val_tab[1];
	jl_uint8 *ptr = NULL;
	jl_uint64 counter = 0;

	reg = SWCORE_BASE + MBSC_DROP_OFFSET + JL_PORT_P2M(egress_port);

	ptr = (jl_uint8 *)&val_tab[0];

	if ((ret = jl_apb_reg_read(reg,
				&val_tab[0])) != JL_ERR_OK)
		return ret;

	jl_read_bits(ptr, &counter, 0, 24);

	*pcntr = (jl_uint32)counter;

	return JL_ERR_OK;
}
