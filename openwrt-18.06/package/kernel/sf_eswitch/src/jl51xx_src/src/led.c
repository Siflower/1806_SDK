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

#include "jl51xx/jl51xx_drv_led.h"
#include "led.h"

static jl_ret_t _jl_led_enable_set(jl_led_group_t group, jl_portmask_t portmask)
{
	if (group >= LED_GROUP_END)
		return JL_ERR_PARAM;
	/* Check Port Mask Valid */
	JL_CHECK_PORTMASK(portmask);

	return jl51xx_led_enable_set(group, portmask);
}

static jl_ret_t _jl_led_enable_get(jl_led_group_t group, jl_portmask_t *portmask)
{
	return jl51xx_led_enable_get(group, portmask);
}

static jl_ret_t _jl_led_group_config_set(jl_led_group_t group, jl_led_config_t config)
{
	return jl51xx_led_group_config_set(group, config);
}

static jl_ret_t _jl_led_group_config_get(jl_led_group_t group, jl_led_config_t *pconfig)
{
	return jl51xx_led_group_config_get(group, pconfig);
}

static jl_ret_t _jl_led_blink_rate_set(jl_led_blink_rate_t blink_rate)
{
	return jl51xx_led_blink_rate_set(blink_rate);
}

static jl_ret_t _jl_led_blink_rate_get(jl_led_blink_rate_t *blink_rate)
{
	return jl51xx_led_blink_rate_get(blink_rate);
}

static jl_ret_t _jl_led_group_active_high_set(jl_led_group_t group, jl_portmask_t portmask)
{
	if (group >= LED_GROUP_END)
		return JL_ERR_PARAM;
	/* Check Port Mask Valid */
	JL_CHECK_PORTMASK(portmask);

	return jl51xx_led_group_active_high_set(group, portmask);
}

static jl_ret_t _jl_led_group_active_high_get(jl_led_group_t group, jl_portmask_t *portmask)
{
	return jl51xx_led_group_active_high_get(group, portmask);
}

jl_api_ret_t jl_led_enable_set(jl_led_group_t group, jl_portmask_t portmask)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_API_MUTEX_LOCK();
	ret = _jl_led_enable_set(group, portmask);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_led_enable_set);

jl_api_ret_t jl_led_enable_get(jl_led_group_t group, jl_portmask_t *portmask)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_POINTER(portmask);

	if (group >= LED_GROUP_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_led_enable_get(group, portmask);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_led_enable_get);

jl_api_ret_t jl_led_group_config_set(jl_led_group_t group, jl_led_config_t config)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	if (config >= LED_CONFIG_END ||
			group >= LED_GROUP_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_led_group_config_set(group, config);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_led_group_config_set);

jl_api_ret_t jl_led_group_config_get(jl_led_group_t group, jl_led_config_t *pconfig)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_POINTER(pconfig);

	if (group >= LED_GROUP_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_led_group_config_get(group, pconfig);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_led_group_config_get);

jl_api_ret_t jl_led_blink_rate_set(jl_led_blink_rate_t blink_rate)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	if (blink_rate >= LED_BLINKRATE_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_led_blink_rate_set(blink_rate);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_led_blink_rate_set);

jl_api_ret_t jl_led_blink_rate_get(jl_led_blink_rate_t *blink_rate)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_POINTER(blink_rate);

	JL_API_MUTEX_LOCK();
	ret = _jl_led_blink_rate_get(blink_rate);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_led_blink_rate_get);

jl_api_ret_t jl_led_group_active_high_set(jl_led_group_t group, jl_portmask_t portmask)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_API_MUTEX_LOCK();
	ret = _jl_led_group_active_high_set(group, portmask);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_led_group_active_high_set);

jl_api_ret_t jl_led_group_active_high_get(jl_led_group_t group, jl_portmask_t *portmask)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_POINTER(portmask);

	if (group >= LED_GROUP_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_led_group_active_high_get(group, portmask);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_led_group_active_high_get);
