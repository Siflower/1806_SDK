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
#include "jl51xx/jl51xx_drv_qos.h"
#include "qos.h"

jl_ret_t _jl_qos_l3_set(struct qos_l3_config *config)
{
	return jl51xx_qos_l3_set(config);
}

jl_ret_t _jl_qos_l3_get(struct qos_l3_config *config)
{
	return jl51xx_qos_l3_get(config);
}

jl_api_ret_t _jl_qos_l3_port_enable_set(jl_port_t port, jl_uint8 enable)
{
	return jl51xx_qos_l3_port_enable_set(port, enable);
}

jl_api_ret_t _jl_qos_l3_port_enable_get(jl_port_t port, jl_uint8 *enable)
{
	return jl51xx_qos_l3_port_enable_get(port, enable);
}

jl_api_ret_t _jl_qos_port_set(jl_port_t port, struct qos_port_config *config)
{
	return jl51xx_qos_port_set(port, config);
}

jl_api_ret_t _jl_qos_port_get(jl_port_t port, struct qos_port_config *config)
{
	return jl51xx_qos_port_get(port, config);
}

jl_api_ret_t _jl_qos_vlan_set(struct qos_vlan_config *config)
{
	return jl51xx_qos_vlan_set(config);
}

jl_api_ret_t _jl_qos_vlan_get(struct qos_vlan_config *config)
{
	return jl51xx_qos_vlan_get(config);
}

jl_api_ret_t jl_qos_l3_set(struct qos_l3_config *config)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_POINTER(config);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_l3_set(config);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_l3_set);

jl_api_ret_t jl_qos_l3_get(struct qos_l3_config *config)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_POINTER(config);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_l3_get(config);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_l3_get);

jl_api_ret_t jl_qos_l3_port_enable_set(jl_port_t port, jl_uint8 enable)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_l3_port_enable_set(port, enable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_l3_port_enable_set);

jl_api_ret_t jl_qos_l3_port_enable_get(jl_port_t port, jl_uint8 *enable)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(enable);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_l3_port_enable_get(port, enable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_l3_port_enable_get);

jl_api_ret_t jl_qos_port_set(jl_port_t port, struct qos_port_config *config)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(config);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_port_set(port, config);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_port_set);

jl_api_ret_t jl_qos_port_get(jl_port_t port, struct qos_port_config *config)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(config);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_port_get(port, config);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_port_get);

jl_api_ret_t jl_qos_vlan_set(struct qos_vlan_config *config)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_POINTER(config);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_vlan_set(config);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_vlan_set);

jl_api_ret_t jl_qos_vlan_get(struct qos_vlan_config *config)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_POINTER(config);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_vlan_get(config);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_vlan_get);

/*new qos api*/
jl_api_ret_t _jl_qos_init(void)
{
	return jl51xx_qos_init();
}

jl_api_ret_t _jl_qos_deinit(void)
{
	return jl51xx_qos_deinit();
}

jl_api_ret_t _jl_qos_l3_queue_assign_enable_set(jl_port_t port, jl_enable_t enable)
{
	return jl51xx_qos_l3_queue_assign_enable_set(port, enable);
}

jl_api_ret_t _jl_qos_l3_queue_assign_enable_get(jl_port_t port, jl_enable_t *penable)
{
	return jl51xx_qos_l3_queue_assign_enable_get(port, penable);
}

jl_api_ret_t _jl_qos_l3_dscp2queue_map_set(jl_uint32 dscp, jl_uint32 queue)
{
	return jl51xx_qos_l3_dscp2queue_map_set(dscp, queue);
}

jl_api_ret_t _jl_qos_l3_dscp2queue_map_get(jl_uint32 dscp, jl_uint32 *pqueue)
{
	return jl51xx_qos_l3_dscp2queue_map_get(dscp, pqueue);
}

jl_api_ret_t _jl_qos_l3_exp2queue_map_set(jl_uint32 exp, jl_uint32 queue)
{
	return jl51xx_qos_l3_exp2queue_map_set(exp, queue);
}

jl_api_ret_t _jl_qos_l3_exp2queue_map_get(jl_uint32 exp, jl_uint32 *pqueue)
{
	return jl51xx_qos_l3_exp2queue_map_get(exp, pqueue);
}

jl_api_ret_t _jl_qos_l2_pcp2queue_map_set(jl_uint32 vlan_pcp, jl_uint32 queue)
{
	return jl51xx_qos_l2_pcp2queue_map_set(vlan_pcp, queue);
}

jl_api_ret_t _jl_qos_l2_pcp2queue_map_get(jl_uint32 vlan_pcp, jl_uint32 *pqueue)
{
	return jl51xx_qos_l2_pcp2queue_map_get(vlan_pcp, pqueue);
}

jl_api_ret_t _jl_qos_l2_port_pcp_set(jl_port_t port, jl_uint32 pcp)
{
	return jl51xx_qos_l2_port_pcp_set(port, pcp);
}

jl_api_ret_t _jl_qos_l2_port_pcp_get(jl_port_t port, jl_uint32 *ppcp)
{
	return jl51xx_qos_l2_port_pcp_get(port, ppcp);
}

jl_api_ret_t _jl_qos_queue_pri_set(jl_port_t port, jl_uint32 queue, jl_uint32 priority)
{
	return jl51xx_qos_queue_pri_set(port, queue, priority);
}

jl_api_ret_t _jl_qos_queue_pri_get(jl_port_t port, jl_uint32 queue, jl_uint32 *ppriority)
{
	return jl51xx_qos_queue_pri_get(port, queue, ppriority);
}

jl_api_ret_t _jl_qos_queue_dwrr_set(jl_port_t port, jl_uint32 queue, jl_uint32 weight)
{
	return jl51xx_qos_queue_dwrr_set(port, queue, weight);
}

jl_api_ret_t _jl_qos_queue_dwrr_get(jl_port_t port, jl_uint32 queue, jl_uint32 *pweight)
{
	return jl51xx_qos_queue_dwrr_get(port, queue, pweight);
}

jl_api_ret_t _jl_qos_schedule_set(jl_uint8 port, jl_qos_schedule_t *psche)
{
	return jl51xx_qos_schedule_set(port, psche);
}

jl_api_ret_t _jl_qos_schedule_get(jl_uint8 port, jl_qos_schedule_t *psche)
{
	return jl51xx_qos_schedule_get(port, psche);
}

jl_api_ret_t _jl_qos_enqueue_set(jl_uint8 port, jl_uint8 enmask)
{
	return jl51xx_qos_enqueue_set(port, enmask);
}

jl_api_ret_t _jl_qos_enqueue_get(jl_uint8 port, jl_uint8 *penmask)
{
	return jl51xx_qos_enqueue_get(port, penmask);
}

jl_api_ret_t jl_qos_init(void)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_init();
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_init);

jl_api_ret_t jl_qos_deinit(void)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_deinit();
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_deinit);

jl_api_ret_t jl_qos_l3_queue_assign_enable_set(jl_port_t port, jl_enable_t enable)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_l3_queue_assign_enable_set(port, enable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_l3_queue_assign_enable_set);


jl_api_ret_t jl_qos_l3_queue_assign_enable_get(jl_port_t port, jl_enable_t *penable)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(penable);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_l3_queue_assign_enable_get(port, penable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_l3_queue_assign_enable_get);

jl_api_ret_t jl_qos_l3_dscp2queue_map_set(jl_uint32 dscp, jl_uint32 queue)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_l3_dscp2queue_map_set(dscp, queue);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_l3_dscp2queue_map_set);

jl_api_ret_t jl_qos_l3_dscp2queue_map_get(jl_uint32 dscp, jl_uint32 *pqueue)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_POINTER(pqueue);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_l3_dscp2queue_map_get(dscp, pqueue);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_l3_dscp2queue_map_get);

jl_api_ret_t jl_qos_l3_exp2queue_map_set(jl_uint32 exp, jl_uint32 queue)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_l3_exp2queue_map_set(exp, queue);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_l3_exp2queue_map_set);

jl_api_ret_t jl_qos_l3_exp2queue_map_get(jl_uint32 exp, jl_uint32 *pqueue)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_POINTER(pqueue);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_l3_exp2queue_map_get(exp, pqueue);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_l3_exp2queue_map_get);

jl_api_ret_t jl_qos_l2_pcp2queue_map_set(jl_uint32 vlan_pcp, jl_uint32 queue)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_l2_pcp2queue_map_set(vlan_pcp, queue);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_l2_pcp2queue_map_set);

jl_api_ret_t jl_qos_l2_pcp2queue_map_get(jl_uint32 vlan_pcp, jl_uint32 *pqueue)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_POINTER(pqueue);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_l2_pcp2queue_map_get(vlan_pcp, pqueue);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_l2_pcp2queue_map_get);

jl_api_ret_t jl_qos_l2_port_pcp_set(jl_port_t port, jl_uint32 pcp)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_l2_port_pcp_set(port, pcp);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_l2_port_pcp_set);

jl_api_ret_t jl_qos_l2_port_pcp_get(jl_port_t port, jl_uint32 *ppcp)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(ppcp);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_l2_port_pcp_get(port, ppcp);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_l2_port_pcp_get);

jl_api_ret_t jl_qos_queue_pri_set(jl_port_t port, jl_uint32 queue, jl_uint32 priority)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_queue_pri_set(port, queue, priority);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_queue_pri_set);

jl_api_ret_t jl_qos_queue_pri_get(jl_port_t port, jl_uint32 queue, jl_uint32 *ppriority)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(ppriority);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_queue_pri_get(port, queue, ppriority);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_queue_pri_get);

jl_api_ret_t jl_qos_queue_dwrr_set(jl_port_t port, jl_uint32 queue, jl_uint32 weight)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_queue_dwrr_set(port, queue, weight);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_queue_dwrr_set);

jl_api_ret_t jl_qos_queue_dwrr_get(jl_port_t port, jl_uint32 queue, jl_uint32 *pweight)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(pweight);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_queue_dwrr_get(port, queue, pweight);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_queue_dwrr_get);

jl_api_ret_t jl_qos_schedule_set(jl_uint8 port, jl_qos_schedule_t *psche)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(psche);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_schedule_set(port, psche);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_schedule_set);

jl_api_ret_t jl_qos_schedule_get(jl_uint8 port, jl_qos_schedule_t *psche)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(psche);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_schedule_get(port, psche);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_schedule_get);

jl_api_ret_t jl_qos_enqueue_set(jl_uint8 port, jl_uint8 enmask)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_enqueue_set(port, enmask);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_enqueue_set);

jl_api_ret_t jl_qos_enqueue_get(jl_uint8 port, jl_uint8 *penmask)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(penmask);

	JL_API_MUTEX_LOCK();
	ret = _jl_qos_enqueue_get(port, penmask);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_qos_enqueue_get);