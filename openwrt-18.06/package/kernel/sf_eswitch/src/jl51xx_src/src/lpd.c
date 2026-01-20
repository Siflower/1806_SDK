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
#include "jl51xx/jl51xx_drv_lpd.h"
#include "lpd.h"

jl_api_ret_t _jl_lpd_config_set(jl_lpd_config_t *lpd_config)
{
	return jl51xx_lpd_ctrl_set(lpd_config);
}

jl_api_ret_t _jl_lpd_config_get(jl_lpd_config_t *lpd_config)
{
	return jl51xx_lpd_ctrl_get(lpd_config);
}

jl_api_ret_t _jl_lpd_enable(void)
{
	return jl51xx_lpd_enable();
}

jl_api_ret_t _jl_lpd_disable(void)
{
	return jl51xx_lpd_disable();
}

jl_api_ret_t _jl_lpd_trigger_once(jl_port_t port)
{
	return jl51xx_lpd_trigger_once(port);
}

jl_api_ret_t _jl_lpd_lp_ports_get(jl_portmask_t *port_mask)
{
	return jl51xx_lpd_ports_get(port_mask);
}

jl_api_ret_t jl_lpd_config_set(jl_lpd_config_t *lpd_config)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_POINTER(lpd_config);

	JL_API_MUTEX_LOCK();
	ret = _jl_lpd_config_set(lpd_config);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_lpd_config_set);

jl_api_ret_t jl_lpd_config_get(jl_lpd_config_t *lpd_config)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_POINTER(lpd_config);

	JL_API_MUTEX_LOCK();
	ret = _jl_lpd_config_get(lpd_config);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_lpd_config_get);

jl_api_ret_t jl_lpd_enable(void)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_API_MUTEX_LOCK();
	ret = _jl_lpd_enable();
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_lpd_enable);

jl_api_ret_t jl_lpd_disable(void)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_API_MUTEX_LOCK();
	ret = _jl_lpd_disable();
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_lpd_disable);

jl_api_ret_t jl_lpd_trigger_once(jl_port_t port)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_lpd_trigger_once(port);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_lpd_trigger_once);

jl_api_ret_t jl_lpd_lp_ports_get(jl_portmask_t *port_mask)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_POINTER(port_mask);

	JL_API_MUTEX_LOCK();
	ret = _jl_lpd_lp_ports_get(port_mask);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_lpd_lp_ports_get);
