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
#include "jl51xx/jl51xx_drv_vlan.h"
#include "vlan.h"

jl_api_ret_t _jl_vlan_init(void)
{
	return jl51xx_vlan_init();
}

jl_api_ret_t jl_vlan_init(void)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_API_MUTEX_LOCK();
	ret = _jl_vlan_init();
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_vlan_init);

jl_api_ret_t _jl_vlan_deinit(void)
{
	return jl51xx_vlan_deinit();
}

jl_api_ret_t jl_vlan_deinit(void)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_API_MUTEX_LOCK();
	ret = _jl_vlan_deinit();
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_vlan_deinit);

jl_api_ret_t _jl_vlan_set(jl_uint32 vlan_index, struct vlan_config *config)
{
	return jl51xx_vlan_set(vlan_index, config);
}

jl_api_ret_t jl_vlan_set(jl_uint32 vlan_index, struct vlan_config *config)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_POINTER(config);

	JL_API_MUTEX_LOCK();
	ret = _jl_vlan_set(vlan_index, config);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_vlan_set);

jl_api_ret_t _jl_vlan_get(jl_uint32 vlan_index, struct vlan_config *config)
{
	return jl51xx_vlan_get(vlan_index, config);
}

jl_api_ret_t jl_vlan_get(jl_uint32 vlan_index, struct vlan_config *config)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_POINTER(config);

	JL_API_MUTEX_LOCK();
	ret = _jl_vlan_get(vlan_index, config);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_vlan_get);

jl_api_ret_t _jl_vlan_set_batch(struct vlan_item *v_list, jl_uint16 size)
{
	return jl51xx_vlan_set_batch(v_list, size);
}

jl_api_ret_t jl_vlan_set_batch(struct vlan_item *v_list, jl_uint16 size)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_POINTER(v_list);

	JL_API_MUTEX_LOCK();
	ret = _jl_vlan_set_batch(v_list, size);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_vlan_set_batch);



/* ---- vlan: port level --------------- */
jl_api_ret_t _jl_port_vlan_set(jl_port_t port, jl_uint32 vid)
{
	return jl51xx_port_vlan_set(port, vid);
}

jl_api_ret_t jl_port_vlan_set(jl_port_t port, jl_uint32 vid)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_vlan_set(port, vid);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_vlan_set);

jl_api_ret_t _jl_port_vlan_get(jl_port_t port, jl_uint32 *vid)
{
	return jl51xx_port_vlan_get(port, vid);
}

jl_api_ret_t jl_port_vlan_get(jl_port_t port, jl_uint32 *vid)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(vid);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_vlan_get(port, vid);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_vlan_get);

jl_api_ret_t _jl_port_vlan_assignment_set(jl_port_t port, enum vlan_assignment opt)
{
	return jl51xx_port_vlan_assignment_set(port, opt);
}

jl_api_ret_t jl_port_vlan_assignment_set(jl_port_t port, enum vlan_assignment opt)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_vlan_assignment_set(port, opt);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_vlan_assignment_set);

jl_api_ret_t _jl_port_vlan_assignment_get(jl_port_t port, enum vlan_assignment *opt)
{
	return jl51xx_port_vlan_assignment_get(port, opt);
}

jl_api_ret_t jl_port_vlan_assignment_get(jl_port_t port, enum vlan_assignment *opt)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(opt);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_vlan_assignment_get(port, opt);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_vlan_assignment_get);

jl_api_ret_t _jl_port_vlan_operation_set(jl_port_t port, jl_port_dir_t dir, enum vlan_operation opt)
{

	if (PORT_DIR_INGRESS == dir)
		return jl51xx_port_vlan_ingress_operation_set(port, opt);
	else
		return jl51xx_port_vlan_egress_operation_set(port, opt);
}

jl_api_ret_t jl_port_vlan_operation_set(jl_port_t port, jl_port_dir_t dir, enum vlan_operation opt)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_vlan_operation_set(port, dir,opt);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_vlan_operation_set);

jl_api_ret_t _jl_port_vlan_operation_get(jl_port_t port, jl_port_dir_t dir,enum vlan_operation *opt)
{
	if (PORT_DIR_INGRESS == dir)
		return jl51xx_port_vlan_ingress_operation_get(port, opt);
	else
		return jl51xx_port_vlan_egress_operation_get(port, opt);
}

jl_api_ret_t jl_port_vlan_operation_get(jl_port_t port, jl_port_dir_t dir, enum vlan_operation *opt)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(opt);

	JL_API_MUTEX_LOCK();
	ret = _jl_port_vlan_operation_get(port, dir, opt);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_port_vlan_operation_get);

jl_api_ret_t _jl_vlan_acpt_frmt_set(jl_port_t port, jl_vlan_acpt_frmt_t frmt)
{
	return jl51xx_vlan_acpt_frmt_set(port, frmt);
}

jl_api_ret_t jl_vlan_acpt_frmt_set(jl_port_t port, jl_vlan_acpt_frmt_t frmt)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_vlan_acpt_frmt_set(port, frmt);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_vlan_acpt_frmt_set);

jl_api_ret_t _jl_vlan_acpt_frmt_get(jl_port_t port, jl_vlan_acpt_frmt_t *pfrmt)
{
	return jl51xx_vlan_acpt_frmt_get(port, pfrmt);
}

jl_api_ret_t jl_vlan_acpt_frmt_get(jl_port_t port, jl_vlan_acpt_frmt_t *pfrmt)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(pfrmt);

	JL_API_MUTEX_LOCK();
	ret = _jl_vlan_acpt_frmt_get(port, pfrmt);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_vlan_acpt_frmt_get);

