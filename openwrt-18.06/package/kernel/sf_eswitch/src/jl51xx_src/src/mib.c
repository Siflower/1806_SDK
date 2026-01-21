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

#include "jl51xx/jl51xx_drv_port.h"
#include "jl51xx/jl51xx_drv_mib.h"
#include "mib.h"

static jl_ret_t _jl_mib_port_rc_mode_get(jl_port_t port, jl_enable_t *penable)
{
	return jl51xx_mib_port_rc_mode_get(port, penable);
}

static jl_ret_t _jl_mib_port_rc_mode_set(jl_port_t port, jl_enable_t enable)
{
	return jl51xx_mib_port_rc_mode_set(port, enable);
}

static jl_ret_t _jl_mib_port_clear(jl_port_t port, jl_port_mib_type_t type)
{
	return jl51xx_mib_port_clear(port, type);
}

static jl_ret_t _jl_mib_port_clear_all(jl_port_t port)
{
	jl_ret_t ret;
	jl_uint16 type;

	for (type = 0; type < PORT_MIB_TYPE_END; type++) {
		ret = jl51xx_mib_port_clear(port, type);
		if (ret)
			return ret;
	}

	return JL_ERR_OK;
}

static jl_ret_t _jl_mib_port_get(jl_port_t port, jl_port_mib_type_t type, jl_mib_counter_t *pcnt)
{
	return jl51xx_mib_port_get(port, type, pcnt);
}

static jl_ret_t _jl_mib_port_get_all(jl_port_t port, jl_port_mib_cnt_t *pport_cnt)
{
	jl_ret_t ret;
	jl_mib_counter_t counter;
	jl_uint32 *ptr;
	jl_enable_t rc;
	jl_port_mib_type_t idx;
	CONST_T jl_uint8 mib_reg_width[PORT_MIB_TYPE_END] = {
		2, 2, 2, 2, 2, 2, 2, 2, 1, 1,
		2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1 };

	ret = jl51xx_mib_port_rc_mode_get(port, &rc);
	if (ret)
		return ret;
	pport_cnt->rc_enable = rc;
	ptr = (jl_uint32 *)(&(pport_cnt->tx_pkt_cnt));

	for (idx = 0; idx < PORT_MIB_TYPE_END; idx++) {
		counter = 0;
		ret = jl51xx_mib_port_get(port, idx, &counter);
		if (ret) {
			return ret;
		}
		if (mib_reg_width[idx] == 2)
			*((jl_uint64 *)ptr) = counter;
		else if (mib_reg_width[idx] == 1)
			*ptr = (counter & 0xFFFFFFFF);
		else
			return JL_ERR_FAIL;
		ptr += mib_reg_width[idx];
	}

	return ret;
}

jl_api_ret_t jl_mib_port_rc_mode_get(jl_port_t port, jl_enable_t *penable)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_CHECK_PORT(port);

	JL_CHECK_POINTER(penable);

	JL_API_MUTEX_LOCK();
	ret = _jl_mib_port_rc_mode_get(port, penable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_mib_port_rc_mode_get);

jl_api_ret_t jl_mib_port_rc_mode_set(jl_port_t port, jl_enable_t enable)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_CHECK_PORT(port);

	if (enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_mib_port_rc_mode_set(port, enable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_mib_port_rc_mode_set);

jl_api_ret_t jl_mib_port_clear(jl_port_t port, jl_port_mib_type_t type)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_CHECK_PORT(port);

	JL_MIB_CHECK_TYPE(type);

	JL_API_MUTEX_LOCK();
	ret = _jl_mib_port_clear(port, type);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_mib_port_clear);

jl_api_ret_t jl_mib_port_clear_all(jl_port_t port)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_mib_port_clear_all(port);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_mib_port_clear_all);

jl_api_ret_t jl_mib_port_get(jl_port_t port, jl_port_mib_type_t type, jl_mib_counter_t *pcnt)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_CHECK_PORT(port);

	JL_CHECK_POINTER(pcnt);

	JL_MIB_CHECK_TYPE(type);

	JL_API_MUTEX_LOCK();
	ret = _jl_mib_port_get(port, type, pcnt);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_mib_port_get);

jl_api_ret_t jl_mib_port_get_all(jl_port_t port, jl_port_mib_cnt_t *pport_cnt)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_CHECK_PORT(port);

	JL_CHECK_POINTER(pport_cnt);

	JL_API_MUTEX_LOCK();
	ret = _jl_mib_port_get_all(port, pport_cnt);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_mib_port_get_all);

jl_api_ret_t jl_mib_swc_global_drop_get(jl_mib_swc_glb_drop_t *pport_cnt)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_CHECK_POINTER(pport_cnt);

	JL_API_MUTEX_LOCK();
	ret = jl51xx_mib_swc_global_drop_get(pport_cnt);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_mib_swc_global_drop_get);

jl_api_ret_t jl_mib_swc_global_drop_clear(void)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_API_MUTEX_LOCK();
	ret = jl51xx_mib_swc_global_drop_clear();
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_mib_swc_global_drop_clear);

jl_api_ret_t jl_mib_swc_port_drop_get(jl_port_t port, jl_mib_swc_port_drop_t *pport_cnt)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_CHECK_PORT(port);

	JL_CHECK_POINTER(pport_cnt);

	JL_API_MUTEX_LOCK();
	ret = jl51xx_mib_swc_port_drop_get(port, pport_cnt);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_mib_swc_port_drop_get);

jl_api_ret_t jl_mib_swc_port_drop_clear(jl_port_t port)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = jl51xx_mib_swc_port_drop_clear(port);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_mib_swc_port_drop_clear);

