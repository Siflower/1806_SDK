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
#include "vlan.h"
#include "jl51xx/jl51xx_drv_l2.h"
#include "l2.h"

static jl_ret_t _jl_l2_aging_learning_get(jl_enable_t *penable_aging, jl_enable_t *penable_learning)
{
	return jl51xx_l2_aging_learning_get(penable_aging,
					penable_learning);
}

static jl_ret_t _jl_l2_aging_learning_set(jl_enable_t enable_aging, jl_enable_t enable_learning)
{
	return jl51xx_l2_aging_learning_set(enable_aging,
					enable_learning);
}

static jl_ret_t _jl_l2_aging_time_get(jl_uint32 *ptime, jl_uint8 *punit)
{
	return jl51xx_l2_aging_time_get(ptime, punit);
}

static jl_ret_t _jl_l2_aging_time_set(jl_uint32 time, jl_uint8 unit)
{
	return jl51xx_l2_aging_time_set(time, unit);
}
static jl_ret_t _jl_l2_fib_col_clear(void)
{
	return jl51xx_l2_fib_col_clear();
}

static jl_ret_t _jl_l2_fib_hash_clear(void)
{
	return jl51xx_l2_fib_hash_clear();
}

static jl_ret_t _jl_l2_mc_port_mask_get(jl_l2_table_entry_idx_t index, jl_uint32 *pport_mask)
{

	return jl51xx_l2_mc_port_mask_get(index, pport_mask);
}

static jl_ret_t _jl_l2_mc_port_mask_set(jl_l2_table_entry_idx_t index, jl_uint32 port_mask)
{
	return jl51xx_l2_mc_port_mask_set(index, port_mask);
}

static jl_ret_t _jl_l2_mc_port_learning_set(jl_port_t port, jl_enable_t en_learning)
{
	return jl51xx_l2_mc_port_learning_set(port, en_learning);
}

static jl_ret_t _jl_l2_mc_port_learning_get(jl_port_t port, jl_enable_t *pen_learning)
{
	return jl51xx_l2_mc_port_learning_get(port, pen_learning);
}

static jl_ret_t _jl_l2_fib_config_get(jl_l2_fib_key_t *pkey, jl_l2_fib_config_t *pcfg)
{
	return jl51xx_l2_fib_config_get(pkey, pcfg);
}

static jl_ret_t _jl_l2_fib_config_get_by_index(jl_uint32 entry_index, jl_l2_fib_key_t *pkey, jl_l2_fib_config_t *pcfg)
{
	return jl51xx_l2_fib_config_get_by_index(entry_index, pkey, pcfg);
}

jl_ret_t _jl_l2_fib_config_set_auto(jl_l2_fib_key_t *key, jl_l2_fib_config_t *cfg)
{
	return jl51xx_l2_fib_config_set_auto(key, cfg);
}

jl_api_ret_t jl_l2_aging_learning_get(jl_enable_t *penable_aging, jl_enable_t *penable_learning)
{
	jl_api_ret_t  ret;

	JL_CHECK_STAT();

	JL_CHECK_POINTER(penable_aging);

	JL_CHECK_POINTER(penable_learning);

	JL_API_MUTEX_LOCK();
	ret = _jl_l2_aging_learning_get(penable_aging, penable_learning);
	JL_API_MUTEX_UNLOCK();
	return ret;
}
EXPORT_SYMBOL(jl_l2_aging_learning_get);

jl_api_ret_t jl_l2_aging_learning_set(jl_enable_t enable_aging, jl_enable_t enable_learning)
{
	jl_api_ret_t  ret;

	JL_CHECK_STAT();

	if (enable_aging > JL_ENABLE_END || enable_learning > JL_ENABLE_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_l2_aging_learning_set(enable_aging, enable_learning);
	JL_API_MUTEX_UNLOCK();
	return ret;
}
EXPORT_SYMBOL(jl_l2_aging_learning_set);

jl_api_ret_t jl_l2_mc_port_mask_get(jl_l2_table_entry_idx_t index, jl_uint32 *pport_mask)
{
	jl_api_ret_t  ret;

	JL_CHECK_STAT();

	JL_CHECK_POINTER(pport_mask);

	JL_API_MUTEX_LOCK();
	ret = _jl_l2_mc_port_mask_get(index, pport_mask);
	JL_API_MUTEX_UNLOCK();
	return ret;
}
EXPORT_SYMBOL(jl_l2_mc_port_mask_get);

jl_api_ret_t jl_l2_mc_port_mask_set(jl_l2_table_entry_idx_t index, jl_uint32 port_mask)
{
	jl_api_ret_t  ret;

	JL_CHECK_STAT();

	JL_API_MUTEX_LOCK();
	ret = _jl_l2_mc_port_mask_set(index, port_mask);
	JL_API_MUTEX_UNLOCK();
	return ret;
}
EXPORT_SYMBOL(jl_l2_mc_port_mask_set);

jl_api_ret_t jl_l2_mc_port_learning_get(jl_port_t port, jl_enable_t *pen_learning)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pen_learning);

	JL_CHECK_STAT();

	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_l2_mc_port_learning_get(port, pen_learning);
	JL_API_MUTEX_UNLOCK();
	return ret;
}
EXPORT_SYMBOL(jl_l2_mc_port_learning_get);

jl_api_ret_t jl_l2_mc_port_learning_set(jl_port_t port, jl_enable_t en_learning)
{
	jl_api_ret_t ret;

	if (en_learning >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	JL_CHECK_STAT();

	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_l2_mc_port_learning_set(port, en_learning);
	JL_API_MUTEX_UNLOCK();
	return ret;
}
EXPORT_SYMBOL(jl_l2_mc_port_learning_set);

jl_api_ret_t jl_l2_fib_config_get_by_index(jl_uint32 entry_index, jl_l2_fib_key_t *pkey, jl_l2_fib_config_t *pcfg)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_CHECK_POINTER(pkey);

	JL_CHECK_POINTER(pcfg);

	if (entry_index >= JL_L2_DA_TABLE_SIZE)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_l2_fib_config_get_by_index(entry_index, pkey, pcfg);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_l2_fib_config_get_by_index);

jl_api_ret_t jl_l2_fib_config_get(jl_l2_fib_key_t *pkey, jl_l2_fib_config_t *pcfg)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_CHECK_POINTER(pcfg);

	if (pkey == NULL || pkey->fid > VLAN_IDX_MAX)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_l2_fib_config_get(pkey, pcfg);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_l2_fib_config_get);

jl_api_ret_t jl_l2_fib_config_set_auto(jl_l2_fib_key_t *pkey, jl_l2_fib_config_t *pcfg)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_CHECK_POINTER(pkey);

	JL_CHECK_POINTER(pcfg);

	JL_API_MUTEX_LOCK();
	ret = _jl_l2_fib_config_set_auto(pkey, pcfg);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_l2_fib_config_set_auto);

jl_api_ret_t jl_l2_fib_clear(jl_bool is_collision)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_API_MUTEX_LOCK();
	if (is_collision == TRUE)
		ret = _jl_l2_fib_col_clear();
	else if (is_collision == FALSE)
		ret = _jl_l2_fib_hash_clear();
	else
		ret = JL_ERR_PARAM;
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_l2_fib_clear);

jl_api_ret_t jl_l2_fib_clear_all(void)
{
	jl_api_ret_t ret;

	JL_API_MUTEX_LOCK();
	ret = _jl_l2_fib_col_clear();
	ret += _jl_l2_fib_hash_clear();
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_l2_fib_clear_all);

jl_api_ret_t jl_l2_fib_fast_clear_dynamic(void)
{
	jl_uint32 cur_tick_cnt = 0;
	jl_uint8 cur_tick_index = 0;
	jl_ret_t ret;
	jl_uint16 delay;

	JL_CHECK_STAT();

	JL_API_MUTEX_LOCK();
	(void)_jl_l2_aging_time_get(&cur_tick_cnt, &cur_tick_index);

	(void)_jl_l2_aging_time_set(0, 1);
	for (delay = 1000; delay > 0; delay--) {
		/* wait for timeout */
	}

	ret = _jl_l2_aging_time_set(cur_tick_cnt, cur_tick_index);

	JL_API_MUTEX_UNLOCK();
	return ret;
}
EXPORT_SYMBOL(jl_l2_fib_fast_clear_dynamic);

jl_api_ret_t jl_l2_aging_time_set(jl_uint32 time, jl_uint8 unit)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_API_MUTEX_LOCK();
	ret = _jl_l2_aging_time_set(time, unit);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_l2_aging_time_set);

jl_api_ret_t jl_l2_aging_time_get(jl_uint32 *ptime, jl_uint8 *punit)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_CHECK_POINTER(ptime);

	JL_CHECK_POINTER(punit);

	JL_API_MUTEX_LOCK();
	ret = _jl_l2_aging_time_get(ptime, punit);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_l2_aging_time_get);
