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
#include "fc.h"


jl_api_ret_t _jl_port_fc_enable_set(jl_port_t port, jl_enable_t enable)
{
	return jl51xx_port_fc_enable_set(port, enable);
}

jl_api_ret_t _jl_port_fc_enable_get(jl_port_t port, jl_enable_t *penable)
{
	return jl51xx_port_fc_enable_get(port, penable);
}

jl_api_ret_t _jl_port_fc_threshold_set(jl_port_t port,
			jl_uint32 threshold_on, jl_uint32 threshold_off)
{
	return jl51xx_port_fc_threshold_set(port, threshold_on, threshold_off);
}

jl_api_ret_t _jl_port_fc_threshold_get(jl_port_t port,
			jl_uint32 *pthreshold_on, jl_uint32 *pthreshold_off)
{
	return jl51xx_port_fc_threshold_get(port, pthreshold_on, pthreshold_off);
}

jl_api_ret_t _jl_port_fc_pause_time_set(jl_port_t port, jl_uint32 pause_time)
{
	return jl51xx_port_fc_pause_time_set(port, pause_time);
}

jl_api_ret_t _jl_port_fc_pause_time_get(jl_port_t port, jl_uint32 *ppause_time)
{
	return jl51xx_port_fc_pause_time_get(port, ppause_time);
}

jl_api_ret_t _jl_taildrop_enable_set(jl_port_t port, jl_enable_t enable)
{
	return jl51xx_taildrop_enable_set(port, enable);
}

jl_api_ret_t _jl_taildrop_enable_get(jl_port_t port, jl_enable_t *penable)
{
	return jl51xx_taildrop_enable_get(port, penable);
}

jl_api_ret_t _jl_taildrop_counter_get(jl_uint32 *pcntr)
{
	return jl51xx_ingress_resource_manager_drop_counter_get(pcntr);
}


jl_ret_t jl_port_fc_enable_set(jl_port_t port, jl_enable_t enable)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_fc_enable_set(port, enable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_fc_enable_set);

jl_ret_t jl_port_fc_enable_get(jl_port_t port, jl_enable_t *penable)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(penable);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_fc_enable_get(port, penable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_fc_enable_get);

jl_ret_t jl_port_fc_threshold_set(jl_port_t port,
			jl_uint32 threshold_on, jl_uint32 threshold_off)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_fc_threshold_set(port, threshold_on, threshold_off);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_fc_threshold_set);

jl_ret_t jl_port_fc_threshold_get(jl_port_t port,
			jl_uint32 *pthreshold_on, jl_uint32 *pthreshold_off)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(pthreshold_on);
	JL_CHECK_POINTER(pthreshold_off);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_fc_threshold_get(port, pthreshold_on, pthreshold_off);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_fc_threshold_get);

jl_ret_t jl_port_fc_pause_time_set(jl_port_t port, jl_uint32 pause_time)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_fc_pause_time_set(port, pause_time);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_fc_pause_time_set);

jl_ret_t jl_port_fc_pause_time_get(jl_port_t port, jl_uint32 *ppause_time)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(ppause_time);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_fc_pause_time_get(port, ppause_time);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_fc_pause_time_get);

jl_api_ret_t jl_taildrop_enable_set(jl_port_t port, jl_enable_t enable)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_PORT(port);

	if (enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_taildrop_enable_set(port, enable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_taildrop_enable_set);

jl_api_ret_t jl_taildrop_enable_get(jl_port_t port, jl_enable_t *penable)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_PORT(port);

	JL_CHECK_POINTER(penable);

	JL_API_MUTEX_LOCK();
	ret = _jl_taildrop_enable_get(port, penable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_taildrop_enable_get);

jl_api_ret_t jl_taildrop_counter_get(jl_uint32 *pcntr)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_POINTER(pcntr);

	JL_API_MUTEX_LOCK();
	ret = _jl_taildrop_counter_get(pcntr);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_taildrop_counter_get);

