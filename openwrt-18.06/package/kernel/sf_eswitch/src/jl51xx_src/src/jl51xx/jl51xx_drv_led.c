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
#include "jl51xx/reg_apb_top.h"
#include "jl51xx/reg_pinmux.h"
#include "jl51xx/jl51xx_drv_led.h"

static inline jl_uint8 __led_config_to_mask(jl_led_config_t config)
{
	jl_uint8 src_mask = 0;

	switch (config) {
		case LED_CONFIG_LINK:
			src_mask = 0x3F;
			break;
		case LED_CONFIG_ACT:
			src_mask = 0xCF;
			break;
		case LED_CONFIG_LINK_ACT:
			src_mask = 0xF;
			break;
		case LED_CONFIG_SPD10:
			src_mask = 0x37;
			break;
		case LED_CONFIG_SPD100:
			src_mask = 0x3B;
			break;
		case LED_CONFIG_SPD10_ACT:
			src_mask = 0x7;
			break;
		case LED_CONFIG_SPD100_ACT:
			src_mask = 0xB;
			break;
		case LED_CONFIG_DUPLEX:
			src_mask = 0x3D;
			break;
		case LED_CONFIG_COL:
			src_mask = 0xFE;
			break;
		case LED_CONFIG_LOOP:
			src_mask = 0xFF;
			break;
		default:
			src_mask= 0;
			break;
	}
	return src_mask;
}

static inline jl_ret_t __mask_to_led_config(jl_uint8 src_mask, jl_led_config_t *pconfig)
{
	jl_ret_t ret = JL_ERR_OK;

	switch (src_mask) {
		case 0x3F:
			*pconfig = LED_CONFIG_LINK;
			break;
		case 0xCF:
			*pconfig = LED_CONFIG_ACT;
			break;
		case 0xF:
			*pconfig = LED_CONFIG_LINK_ACT;
			break;
		case 0x37:
			*pconfig = LED_CONFIG_SPD10;
			break;
		case 0x3B:
			*pconfig = LED_CONFIG_SPD100;
			break;
		case 0x7:
			*pconfig = LED_CONFIG_SPD10_ACT;
			break;
		case 0xB:
			*pconfig = LED_CONFIG_SPD100_ACT;
			break;
		case 0x3D:
			*pconfig = LED_CONFIG_DUPLEX;
			break;
		case 0xFE:
			*pconfig = LED_CONFIG_COL;
			break;
		case 0xFF:
			*pconfig = LED_CONFIG_LOOP;
			break;
		default:
			ret = JL_ERR_PARAM;
			break;
	}
	return ret;
}

jl_ret_t jl51xx_led_enable_get(jl_led_group_t group, jl_portmask_t *portmask)
{
	jl_ret_t ret;
	jl_uint64 pmask;
	jl_uint32 flip_mask;
	jl_uint32 reg_addr = 0;
	jl_uint32 led_cfg[1];
	jl_uint8 *preg_val = NULL;

	reg_addr = APB_TOP_BASE + LED_CFG_0_OFFSET;
	preg_val = (jl_uint8 *)&led_cfg[0];
	ret = jl_apb_reg_read(reg_addr, &led_cfg[0]);
	if (ret)
		return ret;

	if (group == LED_GROUP0)
		jl_read_bits(preg_val, &pmask, 0, 8); //port0-port7
	else if (group == LED_GROUP1)
		jl_read_bits(preg_val, &pmask, 8, 8); //port0-port7
	else
		return JL_ERR_PARAM;

	flip_mask = (jl_portmask_t)pmask;
	FLIP_BITS(flip_mask, 0, 7);

	flip_mask = JL_PORTMASK_VP2P(flip_mask);

	*portmask = flip_mask;
	JL_DBG_MSG(JL_FLAG_LED, _DBG_ON,
			"%s pmask=0x%llxx, portmask=0x%x, led_cfg=0x%x\n",
			__func__, pmask, *portmask, led_cfg[0]);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_led_enable_set(jl_led_group_t group, jl_portmask_t portmask)
{
	jl_ret_t ret;
	jl_uint32 reg_addr = 0;
	jl_uint64 pmask;
	jl_uint32 led_cfg[1];
	jl_uint8 *preg_val = NULL;
	jl_uint32 flip_pmask = JL_PORTMASK_P2VP(portmask);

	if (portmask >= 0x100) {
		JL_DBG_MSG(JL_FLAG_LED, _DBG_ON,
				"only port0-port7 is allowed to enable led\n");
		return JL_ERR_OK;
	}

	FLIP_BITS(flip_pmask, 0, 7);
	pmask = flip_pmask;

	reg_addr = APB_TOP_BASE + LED_CFG_0_OFFSET;
	preg_val = (jl_uint8 *)&led_cfg[0];
	ret = jl_apb_reg_read(reg_addr, &led_cfg[0]);
	if (ret)
		return ret;

	if (group == LED_GROUP0)
		jl_write_bits(preg_val, pmask, 0, 8); //port0-port7
	else if (group == LED_GROUP1)
		jl_write_bits(preg_val, pmask, 8, 8); //port0-port7
	else
		return JL_ERR_PARAM;
	ret = jl_apb_reg_write(reg_addr, led_cfg[0]);
	if (ret)
		return ret;

	JL_DBG_MSG(JL_FLAG_LED, _DBG_ON,
			"%s portmask=0x%x, flip_pmask=0x%x, led_cfg=0x%x\n",
			__func__, portmask, flip_pmask, led_cfg[0]);
	return ret;
}

jl_ret_t jl51xx_led_perport_config_set(jl_port_t phyport, jl_led_group_t group, jl_led_config_t config)
{
	jl_ret_t ret;
	jl_uint32 reg_addr = 0;
	jl_uint32 val[1];
	jl_uint8 *preg_val = NULL;
	jl_uint64 src_mask;

	src_mask = __led_config_to_mask(config);
	if (src_mask == 0)
		return JL_ERR_PARAM;

	preg_val = (jl_uint8 *)&val[0];
	reg_addr = APB_TOP_BASE + LED_CFG_4_OFFSET + JL_PORT_P2VP(phyport);
	if (group == LED_GROUP1)
		reg_addr += 8; //LED_CFG_12_OFFSET

	ret = jl_apb_reg_read(reg_addr, &val[0]);
	if (ret)
		return ret;

	//do not modify bit8 loop indicator
	jl_write_bits(preg_val, src_mask, 0, 8);
	ret = jl_apb_reg_write(reg_addr, val[0]);

	JL_DBG_MSG(JL_FLAG_LED, _DBG_ON,
			"%s addr=0x%x, src_mask=0x%llx, regval=0x%x\n",
			__func__, reg_addr, src_mask, val[0]);
	return ret;
}

jl_ret_t jl51xx_led_group_config_set(jl_led_group_t group, jl_led_config_t config)
{
	jl_ret_t ret;
	jl_port_t port;

	JL_FOR_EACH_PHY_UTP_PORT(port) {
		ret = jl51xx_led_perport_config_set(
				port,
				group,
				config);
		if (ret)
			return ret;
	}

	return JL_ERR_OK;
}

jl_ret_t jl51xx_led_group_config_get(jl_led_group_t group, jl_led_config_t *pconfig)
{
	jl_ret_t ret;
	jl_uint32 reg_addr = 0;
	jl_uint32 val[1];
	jl_uint8 *preg_val = NULL;
	jl_uint64 src_mask = 0;

	preg_val = (jl_uint8 *)&val[0];
	reg_addr = APB_TOP_BASE + LED_CFG_4_OFFSET; //Each led is same configured
	if (group == LED_GROUP1)
		reg_addr += 8; //LED_CFG_12_OFFSET

	ret = jl_apb_reg_read(reg_addr, &val[0]);
	if (ret)
		return ret;

	//do not modify bit8 loop indicator
	jl_read_bits(preg_val, &src_mask, 0, 8);
	ret = __mask_to_led_config((jl_uint8)src_mask, pconfig);

	JL_DBG_MSG(JL_FLAG_LED, _DBG_ON,
			"%s src_mask=0x%llx, regval=0x%x\n",
			__func__, src_mask, val[0]);
	return ret;
}

jl_ret_t jl51xx_led_onoff_time_set(jl_uint8 on_time, jl_uint8 off_time)
{
	jl_ret_t ret;
	jl_uint32 reg_addr = 0;
	jl_uint32 val[1];
	jl_uint8 *preg_val = NULL;
	jl_uint64 on = on_time, off = off_time;

	preg_val = (jl_uint8 *)&val[0];
	reg_addr = APB_TOP_BASE + LED_CFG_2_OFFSET;

	ret = jl_apb_reg_read(reg_addr, &val[0]);
	if (ret)
		return ret;

	jl_write_bits(preg_val, on, 0, 7);
	jl_write_bits(preg_val, off, 7, 7);

	ret = jl_apb_reg_write(reg_addr, val[0]);

	JL_DBG_MSG(JL_FLAG_LED, _DBG_ON,
			"%s on_time=0x%x, off_time=0x%x, regval=0x%x\n",
			__func__, on_time, off_time, val[0]);
	return ret;
}

jl_ret_t jl51xx_led_onoff_time_get(jl_uint8 *on_time, jl_uint8 *off_time)
{
	jl_ret_t ret;
	jl_uint32 reg_addr = 0;
	jl_uint32 val[1];
	jl_uint8 *preg_val = NULL;
	jl_uint64 on = 0, off = 0;

	preg_val = (jl_uint8 *)&val[0];
	reg_addr = APB_TOP_BASE + LED_CFG_2_OFFSET;

	ret = jl_apb_reg_read(reg_addr, &val[0]);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &on, 0, 7);
	jl_read_bits(preg_val, &off, 7, 7);

	*on_time = (jl_uint8)on;
	*off_time = (jl_uint8)off;

	JL_DBG_MSG(JL_FLAG_LED, _DBG_ON,
			"%s on_time=0x%x, off_time=0x%x, regval=0x%x\n",
			__func__, *on_time, *off_time, val[0]);
	return ret;
}

jl_ret_t jl51xx_led_blink_rate_set(jl_led_blink_rate_t blink_rate)
{
	jl_uint8 on_time;

	if (blink_rate > 0x7F)
		return JL_ERR_OUT_OF_RANGE;

	on_time = (jl_uint8)blink_rate;

	return jl51xx_led_onoff_time_set(on_time, on_time);
}

jl_ret_t jl51xx_led_blink_rate_get(jl_led_blink_rate_t *blink_rate)
{
	jl_ret_t ret;
	jl_uint8 on_time = 0, off_time = 0;

	ret = jl51xx_led_onoff_time_get(&on_time, &off_time);
	if (ret)
		return ret;

	if (on_time != off_time) {
		JL_DBG_MSG(JL_FLAG_LED, _DBG_ERROR,
				"LED blinkrate on_time != off_time!\n");
		return JL_ERR_CONFIG;
	}
	if (on_time > LED_BLINKRATE_2032MS)
		return JL_ERR_OUT_OF_RANGE;

	*blink_rate = on_time;
	return JL_ERR_OK;
}

jl_ret_t jl51xx_led_group_active_high_set(jl_led_group_t group, jl_portmask_t portmask)
{
	jl_ret_t ret;
	jl_uint32 reg_addr = 0;
	jl_uint64 pmask;
	jl_uint32 polarity_cfg[1];
	jl_uint8 *preg_val = NULL;
	jl_uint32 flip_pmask = JL_PORTMASK_P2VP(portmask);

	if (portmask >= 0x100) {
		JL_DBG_MSG(JL_FLAG_LED, _DBG_ERROR,
				"only port0-port7 is allowed to enable led\n");
		return JL_ERR_OK;
	}

	FLIP_BITS(flip_pmask, 0, 7);
	pmask = flip_pmask;

	reg_addr = APB_TOP_BASE + LED_CFG_1_OFFSET;
	preg_val = (jl_uint8 *)&polarity_cfg[0];
	ret = jl_apb_reg_read(reg_addr, &polarity_cfg[0]);
	if (ret)
		return ret;

	if (group == LED_GROUP0)
		jl_write_bits(preg_val, pmask, 0, 8); //port0-port7
	else if (group == LED_GROUP1)
		jl_write_bits(preg_val, pmask, 8, 8); //port0-port7
	else
		return JL_ERR_PARAM;
	ret = jl_apb_reg_write(reg_addr, polarity_cfg[0]);
	if (ret)
		return ret;

	return ret;
}

jl_ret_t jl51xx_led_group_active_high_get(jl_led_group_t group, jl_portmask_t *portmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint64 pmask = 0;
	jl_uint32 flip_mask = 0;
	jl_uint32 reg_addr = 0;
	jl_uint32 polarity_cfg[1] = {0};
	jl_uint8 *preg_val = NULL;

	reg_addr = APB_TOP_BASE + LED_CFG_1_OFFSET;
	preg_val = (jl_uint8 *)&polarity_cfg[0];
	ret = jl_apb_reg_read(reg_addr, &polarity_cfg[0]);
	if (ret)
		return ret;

	if (group == LED_GROUP0)
		jl_read_bits(preg_val, &pmask, 0, 8); //port0-port7
	else if (group == LED_GROUP1)
		jl_read_bits(preg_val, &pmask, 8, 8); //port0-port7
	else
		return JL_ERR_PARAM;

	flip_mask = (jl_portmask_t)pmask;
	FLIP_BITS(flip_mask, 0, 7);

	flip_mask = JL_PORTMASK_VP2P(flip_mask);
	*portmask = flip_mask;
	JL_DBG_MSG(JL_FLAG_LED, _DBG_ON,
			"%s pmask=0x%llx, polarity_cfg=0x%x\n",
			__func__, pmask, polarity_cfg[0]);

	return JL_ERR_OK;
}
