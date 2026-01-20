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
#include "jl_switch.h"
#include "storm.h"
#include "jl51xx/jl51xx_drv_storm.h"

static jl_ret_t _jl_storm_control_drop_counter_get(jl_port_t egress_port, jl_uint32 *pcntr)
{
	return jl51xx_storm_control_drop_counter_get(egress_port, pcntr);
}

static jl_ret_t _jl_storm_control_rate_set(jl_port_t egress_port, jl_storm_type_t type, jl_rate_unit_t unit, jl_rate_t rate)
{
	return jl51xx_storm_control_rate_set(egress_port,
					type,
					unit,
					rate);
}

static jl_ret_t _jl_storm_control_rate_get(jl_port_t egress_port, jl_storm_type_t type, jl_rate_unit_t *punit, jl_rate_t *prate)
{
	return jl51xx_storm_control_rate_get(egress_port,
					type,
					punit,
					prate);
}

static jl_ret_t _jl_storm_control_enable_set(jl_port_t egress_port, jl_storm_type_t type, jl_enable_t enable)
{
	return jl51xx_storm_control_enable_set(egress_port, type, enable);
}

static jl_ret_t _jl_storm_control_enable_get(jl_port_t egress_port, jl_storm_type_t type, jl_enable_t *penable)
{
	return jl51xx_storm_control_enable_get(egress_port, type, penable);
}

jl_api_ret_t jl_storm_control_enable_set(jl_port_t egress_port, jl_storm_type_t type, jl_enable_t enable)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_PORT(egress_port);

	if (type >= STORM_TYPE_END || enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_storm_control_enable_set(egress_port, type, enable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_storm_control_enable_set);

jl_api_ret_t jl_storm_control_enable_get(jl_port_t egress_port, jl_storm_type_t type, jl_enable_t *penable)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_PORT(egress_port);

	JL_CHECK_POINTER(penable);

	if (type >= STORM_TYPE_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_storm_control_enable_get(egress_port, type, penable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_storm_control_enable_get);

jl_api_ret_t jl_storm_control_rate_set(jl_port_t egress_port, jl_storm_type_t type, jl_rate_unit_t unit, jl_rate_t rate)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_PORT(egress_port);

	if (type >= STORM_TYPE_END || unit >= RATE_UNIT_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_storm_control_rate_set(
				egress_port,
				type,
				unit,
				rate);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_storm_control_rate_set);

jl_api_ret_t jl_storm_control_rate_get(jl_port_t egress_port, jl_storm_type_t type, jl_rate_unit_t *punit, jl_rate_t *prate)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_PORT(egress_port);

	if (type >= STORM_TYPE_END)
		return JL_ERR_PARAM;

	JL_CHECK_POINTER(punit);

	JL_CHECK_POINTER(prate);

	JL_API_MUTEX_LOCK();
	ret = _jl_storm_control_rate_get(
				egress_port,
				type,
				punit,
				prate);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_storm_control_rate_get);

jl_api_ret_t jl_storm_control_drop_counter_get(jl_port_t egress_port, jl_uint32 *pcntr)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_PORT(egress_port);

	JL_CHECK_POINTER(pcntr);

	JL_API_MUTEX_LOCK();
	ret = _jl_storm_control_drop_counter_get(
				egress_port,
				pcntr);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_storm_control_drop_counter_get);
