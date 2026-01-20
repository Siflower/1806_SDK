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

#include "jl51xx/jl51xx_drv_stp.h"
#include "stp.h"

jl_ret_t _jl_stp_port_ingress_state_set(jl_port_t port, jl_stp_state_t stp_state)
{
	return jl51xx_stp_port_ingress_state_set(port, stp_state);
}

jl_ret_t _jl_stp_port_ingress_state_get(jl_port_t port, jl_stp_state_t *pstp_state)
{
	return jl51xx_stp_port_ingress_state_get(port, pstp_state);
}

jl_ret_t _jl_stp_port_egress_state_set(jl_port_t port, jl_stp_state_t stp_state)
{
	return jl51xx_stp_port_egress_state_set(port, stp_state);
}

jl_ret_t _jl_stp_port_egress_state_get(jl_port_t port, jl_stp_state_t *pstp_state)
{
	return jl51xx_stp_port_egress_state_get(port, pstp_state);
}

jl_ret_t _jl_stp_ingress_drop_counter_get(jl_stp_state_t stp_state, jl_uint32 *pcntr)
{
	return jl51xx_stp_ingress_drop_counter_get(stp_state, pcntr);
}

jl_ret_t _jl_stp_port_egress_drop_counter_get(jl_port_t port, jl_uint32 *pcntr)
{
	return jl51xx_stp_port_egress_drop_counter_get(port, pcntr);
}

jl_api_ret_t jl_stp_port_egress_state_set(jl_port_t port, jl_stp_state_t stp_state)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_PORT(port);

	if (stp_state >= STP_STATE_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_stp_port_egress_state_set(port, stp_state);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_stp_port_egress_state_set);

jl_api_ret_t jl_stp_port_egress_state_get(jl_port_t port, jl_stp_state_t *pstp_state)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_PORT(port);

	JL_CHECK_POINTER(pstp_state);

	JL_API_MUTEX_LOCK();
	ret = _jl_stp_port_egress_state_get(port, pstp_state);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_stp_port_egress_state_get);

jl_api_ret_t jl_stp_port_ingress_state_set(jl_port_t port, jl_stp_state_t stp_state)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_PORT(port);

	if (stp_state >= STP_STATE_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_stp_port_ingress_state_set(port, stp_state);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_stp_port_ingress_state_set);

jl_api_ret_t jl_stp_port_ingress_state_get(jl_port_t port, jl_stp_state_t *pstp_state)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_PORT(port);

	JL_CHECK_POINTER(pstp_state);

	JL_API_MUTEX_LOCK();
	ret = _jl_stp_port_ingress_state_get(port, pstp_state);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_stp_port_ingress_state_get);

jl_api_ret_t jl_stp_ingress_drop_counter_get(jl_stp_state_t stp_state, jl_uint32 *pcntr)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	/* Packets will be forwarded as normal when set to
	 * STP_STATE_DISABLED or STP_STATE_FORWARDING
	 */
	if (stp_state == STP_STATE_DISABLED || \
			stp_state == STP_STATE_FORWARDING)
		return JL_ERR_PARAM;

	JL_CHECK_POINTER(pcntr);

	JL_API_MUTEX_LOCK();
	ret = _jl_stp_ingress_drop_counter_get(stp_state, pcntr);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_stp_ingress_drop_counter_get);

jl_api_ret_t jl_stp_port_egress_drop_counter_get(jl_port_t port, jl_uint32 *pcntr)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_PORT(port);

	JL_CHECK_POINTER(pcntr);

	JL_API_MUTEX_LOCK();
	ret = _jl_stp_port_egress_drop_counter_get(port, pcntr);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_stp_port_egress_drop_counter_get);
