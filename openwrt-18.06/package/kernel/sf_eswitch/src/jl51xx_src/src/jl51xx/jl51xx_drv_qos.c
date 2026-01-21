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
#include "qos.h"
#include "jl51xx/jl51xx_drv_qos.h"

#define QOS_L3_PROR_MAX_IDX	3
#define QOS_L3_TOS_MAX_IDX	256
#define QOS_L3_EXP_MAX_IDX	8
#define QOS_L3_UNKNOWN_MAX_IDX	1

jl_qos_res_mng_t g_res_mngt;

static jl_ret_t jl51xx_qos_resource_info_get(void);
static jl_ret_t jl51xx_qos_drain_port(jl_uint16 l_mask);
static jl_ret_t jl51xx_qos_recover_port(jl_uint16 l_mask);
static jl_ret_t jl51xx_qos_check_port_empty(jl_uint16 l_mask);
static jl_ret_t jl51xx_qos_dwrr_res_check(void);


static jl_ret_t __check_l3_args(struct qos_l3_config *config)
{
	switch (config->type) {
	case QOS_L3_IPV4:
		if (config->qos_l3.ipv4.priority > QOS_L3_PROR_MAX_IDX)
			return JL_ERR_PARAM;
		if (config->qos_l3.ipv4.tos >= QOS_L3_TOS_MAX_IDX)
			return JL_ERR_PARAM;
		break;
	case QOS_L3_IPV6:
		if (config->qos_l3.ipv6.priority > QOS_L3_PROR_MAX_IDX)
			return JL_ERR_PARAM;
		if (config->qos_l3.ipv6.tos >= QOS_L3_TOS_MAX_IDX)
			return JL_ERR_PARAM;
		break;
	case QOS_L3_MPLS:
		if (config->qos_l3.mpls.priority > QOS_L3_PROR_MAX_IDX)
			return JL_ERR_PARAM;
		if (config->qos_l3.mpls.exp >= QOS_L3_EXP_MAX_IDX)
			return JL_ERR_PARAM;
		break;
	case QOS_L3_UNKNOWN:
		if (config->qos_l3.unknown.priority > QOS_L3_PROR_MAX_IDX)
			return JL_ERR_PARAM;
		break;
	default:
		return JL_ERR_PARAM;
	}

	return JL_ERR_OK;
}

/* L3 Based assign */
jl_ret_t jl51xx_qos_l3_set(struct qos_l3_config *config)
{
	jl_ret_t ret;
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = NULL;

	if (__check_l3_args(config))
		return JL_ERR_PARAM;

	switch (config->type) {
	case QOS_L3_IPV4:
		{
		jl_uint32 ipv4_egress_val[1] = {0};
		jl_uint64 p_queue;

		reg_addr = SWCORE_BASE + IP_TOS_FIELD_TO_EGRESS_QUEUE_MAPPING_TABLE_OFFSET + config->qos_l3.ipv4.tos * 1;
		preg_val = (jl_uint8 *)&ipv4_egress_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &ipv4_egress_val[0], 1);
		if (ret)
			return ret;
		p_queue = config->qos_l3.ipv4.priority;
		jl_write_bits(preg_val, p_queue, 0, 2);
		ret = jl_apb_reg_burst_write(reg_addr, &ipv4_egress_val[0], 1);
		if (ret)
			return ret;
		break;
		}
	case QOS_L3_IPV6:
		{
		jl_uint32 ipv6_egress_val[1] = {0};
		jl_uint64 p_queue;

		reg_addr = SWCORE_BASE + IP_TOS_FIELD_TO_EGRESS_QUEUE_MAPPING_TABLE_OFFSET + config->qos_l3.ipv6.tos * 1;
		preg_val = (jl_uint8 *)&ipv6_egress_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &ipv6_egress_val[0], 1);
		if (ret)
			return ret;
		p_queue = config->qos_l3.ipv6.priority;
		jl_write_bits(preg_val, p_queue, 0, 2);
		ret = jl_apb_reg_burst_write(reg_addr, &ipv6_egress_val[0], 1);
		if (ret)
			return ret;
		break;
		}
	case QOS_L3_MPLS:
		{
		jl_uint32 mplsmap_val[1] = {0};
		jl_uint64 p_queue;

		reg_addr = SWCORE_BASE + MPLS_EXP_FIELD_TO_EGRESS_QUEUE_MAPPING_TABLE_OFFSET + config->qos_l3.mpls.exp * 1;
		preg_val = (jl_uint8 *)&mplsmap_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &mplsmap_val[0], 1);
		if (ret)
			return ret;
		p_queue = config->qos_l3.mpls.priority;
		jl_write_bits(preg_val, p_queue, 0, 2);
		ret = jl_apb_reg_burst_write(reg_addr, &mplsmap_val[0], 1);
		if (ret)
			return ret;
		break;
		}
	case QOS_L3_UNKNOWN:
		{
		jl_uint32 unknown_type_val[1] = {0};
		jl_uint64 force_queue;
		jl_uint64 e_queue;

		reg_addr = SWCORE_BASE + FORCE_UNKNOWN_L3_PACKET_TO_SPECIFIC_EGRESS_QUEUE_OFFSET;
		preg_val = (jl_uint8 *)&unknown_type_val[0];
		force_queue = config->qos_l3.unknown.force_en;
		e_queue = config->qos_l3.unknown.priority;
		jl_write_bits(preg_val, force_queue, 0, 1);
		jl_write_bits(preg_val, e_queue, 1, 2);
		ret = jl_apb_reg_burst_write(reg_addr, &unknown_type_val[0], 1);
		if (ret)
			return ret;
		break;
		}
	default:
		return JL_ERR_PARAM;
	}

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_l3_get(struct qos_l3_config *config)
{
	jl_ret_t ret;
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = NULL;

	if (__check_l3_args(config))
		return JL_ERR_PARAM;

	do {
		jl_uint32 ipv4_egress_val[1];
		jl_uint64 p_queue;

		reg_addr = SWCORE_BASE + IP_TOS_FIELD_TO_EGRESS_QUEUE_MAPPING_TABLE_OFFSET + config->qos_l3.ipv4.tos * 1;
		preg_val = (jl_uint8 *)&ipv4_egress_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &ipv4_egress_val[0], 1);
		if (ret)
			return ret;
		jl_read_bits(preg_val, &p_queue, 0, 2);
		config->qos_l3.ipv4.priority = p_queue;
	} while (0);

	do {
		jl_uint32 ipv6_egress_val[1];
		jl_uint64 p_queue;

		reg_addr = SWCORE_BASE + IP_TOS_FIELD_TO_EGRESS_QUEUE_MAPPING_TABLE_OFFSET + config->qos_l3.ipv6.tos * 1;
		preg_val = (jl_uint8 *)&ipv6_egress_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &ipv6_egress_val[0], 1);
		if (ret)
			return ret;
		jl_read_bits(preg_val, &p_queue, 0, 2);
		config->qos_l3.ipv6.priority = p_queue;
	} while (0);

	do {
		jl_uint32 mplsmap_val[1];
		jl_uint64 p_queue;

		reg_addr = SWCORE_BASE + MPLS_EXP_FIELD_TO_EGRESS_QUEUE_MAPPING_TABLE_OFFSET + config->qos_l3.mpls.exp * 1;
		preg_val = (jl_uint8 *)&mplsmap_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &mplsmap_val[0], 1);
		if (ret)
			return ret;
		jl_read_bits(preg_val, &p_queue, 0, 2);
		config->qos_l3.mpls.priority = p_queue;
	} while (0);

	do {
		jl_uint32 unknown_type_val[1];
		jl_uint64 force_queue;
		jl_uint64 e_queue;

		reg_addr = SWCORE_BASE + FORCE_UNKNOWN_L3_PACKET_TO_SPECIFIC_EGRESS_QUEUE_OFFSET;
		preg_val = (jl_uint8 *)&unknown_type_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &unknown_type_val[0], 1);
		if (ret)
			return ret;
		jl_read_bits(preg_val, &force_queue, 0, 1);
		jl_read_bits(preg_val, &e_queue, 1, 2);
		config->qos_l3.unknown.force_en = force_queue;
		config->qos_l3.unknown.priority = e_queue;
	} while (0);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_l3_port_enable_set(jl_port_t port, jl_uint8 enable)
{
	jl_ret_t ret;
	jl_uint64 prio_from_l3;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	prio_from_l3 = enable;
	jl_write_bits(preg_val, prio_from_l3, 0, 1);

	ret = jl_apb_reg_burst_write(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_l3_port_enable_get(jl_port_t port, jl_uint8 *enable)
{
	jl_ret_t ret;
	jl_uint64 prio_from_l3;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &prio_from_l3, 0, 1);
	*enable = prio_from_l3;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_port_set(jl_port_t port, struct qos_port_config *config)
{
	jl_ret_t ret;
	jl_uint64 default_pcp;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	default_pcp = config->pcp_priority;
	jl_write_bits(preg_val, default_pcp, 27, 3);

	ret = jl_apb_reg_burst_write(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_port_get(jl_port_t port, struct qos_port_config *config)
{
	jl_ret_t ret;
	jl_uint64 default_pcp;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &default_pcp, 27, 3);
	config->pcp_priority = default_pcp;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_vlan_set(struct qos_vlan_config *config)
{
	jl_ret_t ret;
	jl_uint64 p_queue;
	jl_uint32 vlan_pcp_map_val[1];
	jl_uint32 reg_addr = SWCORE_BASE + VLAN_PCP_TO_EGRESS_QUEUE_MAPPING_TABLE_OFFSET + config->pcp * 1;
	jl_uint8 *preg_val = (jl_uint8 *)&vlan_pcp_map_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &vlan_pcp_map_val[0], 1);
	if (ret)
		return ret;

	p_queue = config->vlan_priority;
	jl_write_bits(preg_val, p_queue, 0, 2);

	ret = jl_apb_reg_burst_write(reg_addr, &vlan_pcp_map_val[0], 1);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_vlan_get(struct qos_vlan_config *config)
{
	jl_ret_t ret;
	jl_uint64 p_queue;
	jl_uint32 vlan_pcp_map_val[1];
	jl_uint32 reg_addr = SWCORE_BASE + VLAN_PCP_TO_EGRESS_QUEUE_MAPPING_TABLE_OFFSET + config->pcp * 1;
	jl_uint8 *preg_val = (jl_uint8 *)&vlan_pcp_map_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &vlan_pcp_map_val[0], 1);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &p_queue, 0, 2);
	config->vlan_priority = p_queue;

	return JL_ERR_OK;
}

/*new qos api*/
jl_ret_t jl51xx_qos_table_init(void)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t port = UTP_PORT0;
	jl_uint32 dscp = 0;
	jl_uint32 exp = 0;
	jl_uint32 pcp = 0;
	jl_uint32 queue = 0;

	jl_qos_schedule_t t_sche;

	for (queue = 0; queue < QOS_QUEUE_NUM; queue++) {
		t_sche.type[queue] = QOS_SCHE_SP;
	}

	JL_FOR_EACH_PHY_PORT(port) {
		/*set default l3pri disable*/
		ret = jl51xx_qos_l3_queue_assign_enable_set(port, DISABLED);
		JL_CHECK_RETURN(ret);

		/*set default port pcp 0*/
		ret = jl51xx_qos_l2_port_pcp_set(port, 0);
		JL_CHECK_RETURN(ret);

		/*set default Egress Queue weight 1 */
		for (queue = 0; queue < MAX_QUEUE_NUM; queue++) {
			ret = jl51xx_qos_queue_dwrr_set(port, queue, 1);
			JL_CHECK_RETURN(ret);
		}

		ret = jl51xx_qos_schedule_set(port, &t_sche);
		JL_CHECK_RETURN(ret);
	}

	for (dscp = 0; dscp <= MAX_DSCP; dscp++) {
		ret = jl51xx_qos_l3_dscp2queue_map_set(dscp, 1);
		JL_CHECK_RETURN(ret);
	}

	for (exp = 0; exp <= MAX_EXP; exp++) {
		ret = jl51xx_qos_l3_exp2queue_map_set(exp, 1);
		JL_CHECK_RETURN(ret);
	}

	for (pcp = 0; pcp <= MAX_PCP; pcp++) {
		ret = jl51xx_qos_l2_pcp2queue_map_set(pcp, 1);
		JL_CHECK_RETURN(ret);
	}


	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_init(void)
{
	jl_ret_t ret = JL_ERR_OK;

	ret = jl51xx_qos_resource_info_get();
	JL_CHECK_RETURN(ret);

	ret = jl51xx_qos_table_init();
	JL_CHECK_RETURN(ret);

	return JL_ERR_OK;
}


jl_ret_t jl51xx_qos_deinit(void)
{
	jl_ret_t ret = JL_ERR_OK;

	ret = jl51xx_qos_table_init();
	JL_CHECK_RETURN(ret);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_buf_manage_set(jl_port_t port, jl_uint32 mode)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 reg_val = 0;

	JL_CHECK_PORT(port);

	reg_addr = SWCORE_BASE + EGRESS_RESOURCE_MANAGEMENT_MODE_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_read(reg_addr, &reg_val);
	if (ret)
		return ret;

	/*limit mode:0; Guarantee mode:1*/
	WRITE_BIT(reg_val, 0, mode);
	ret = jl_apb_reg_write(reg_addr, reg_val);
	if (ret)
		return ret;

	g_res_mngt.mng_mode[port] = mode & 0x1;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_buf_manage_get(jl_port_t port, jl_uint32 *pmode)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 reg_val = 0;

	JL_CHECK_PORT(port);

	reg_addr = SWCORE_BASE + EGRESS_RESOURCE_MANAGEMENT_MODE_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_read(reg_addr, &reg_val);
	if (ret)
		return ret;

	/*limit mode:0; Guarantee mode:1*/
	*pmode = GET_BIT(reg_val, 0);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_queue_guarantee_set(jl_uint32 index, jl_uint32 cells)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 reg_val = 0;

	JL_ASSERT(index <= 3);
	JL_ASSERT(cells <= 0x3ff);

	reg_addr = SWCORE_BASE + EGRESS_QUEUE_GUARANTEE_SET_OFFSET + index;
	ret = jl_apb_reg_read(reg_addr, &reg_val);
	if (ret)
		return ret;

	WRITE_BITS(reg_val, 0, 9, cells);
	ret = jl_apb_reg_write(reg_addr, reg_val);
	if (ret)
		return ret;

	g_res_mngt.guarantee[index] = cells;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_queue_guarantee_get(jl_uint32 index, jl_uint32 *pcells)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 reg_val = 0;

	JL_ASSERT(index <= 3);
	JL_CHECK_POINTER(pcells);

	reg_addr = SWCORE_BASE + EGRESS_QUEUE_GUARANTEE_SET_OFFSET + index;
	ret = jl_apb_reg_read(reg_addr, &reg_val);
	if (ret)
		return ret;

	/*limit mode:0; Guarantee mode:1*/
	*pcells = GET_BITS(reg_val, 0, 9);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_l3_queue_assign_enable_set(jl_port_t port, jl_enable_t enable)
{
	jl_ret_t ret;
	jl_uint64 prio_from_l3;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	JL_ASSERT(enable < JL_ENABLE_END);

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	prio_from_l3 = enable;
	jl_write_bits(preg_val, prio_from_l3, 0, 1);

	ret = jl_apb_reg_burst_write(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_l3_queue_assign_enable_get(jl_port_t port, jl_enable_t *penable)
{
	jl_ret_t ret;
	jl_uint64 prio_from_l3;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &prio_from_l3, 0, 1);
	*penable = prio_from_l3;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_l3_dscp2queue_map_set(jl_uint32 dscp, jl_uint32 queue)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 reg_val = 0;

	JL_ASSERT(dscp <= MAX_DSCP);
	JL_ASSERT(queue < MAX_QUEUE_NUM);

	reg_addr = SWCORE_BASE + IP_TOS_FIELD_TO_EGRESS_QUEUE_MAPPING_TABLE_OFFSET + (dscp << 2);
	ret = jl_apb_reg_read(reg_addr, &reg_val);
	if (ret)
		return ret;

	WRITE_BITS(reg_val, 0, 1, queue);
	ret = jl_apb_reg_write(reg_addr, reg_val);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_l3_dscp2queue_map_get(jl_uint32 dscp, jl_uint32 *pqueue)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 reg_val = 0;

	JL_ASSERT(dscp <= MAX_DSCP);
	JL_CHECK_POINTER(pqueue);

	reg_addr = SWCORE_BASE + IP_TOS_FIELD_TO_EGRESS_QUEUE_MAPPING_TABLE_OFFSET + (dscp << 2);
	ret = jl_apb_reg_read(reg_addr, &reg_val);
	if (ret)
		return ret;

	*pqueue = GET_BITS(reg_val, 0, 1);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_l3_exp2queue_map_set(jl_uint32 exp, jl_uint32 queue)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 reg_val = 0;

	JL_ASSERT(exp <= MAX_EXP);
	JL_ASSERT(queue < MAX_QUEUE_NUM);

	reg_addr = SWCORE_BASE + MPLS_EXP_FIELD_TO_EGRESS_QUEUE_MAPPING_TABLE_OFFSET + exp;
	ret = jl_apb_reg_read(reg_addr, &reg_val);
	if (ret)
		return ret;

	WRITE_BITS(reg_val, 0, 1, queue);
	ret = jl_apb_reg_write(reg_addr, reg_val);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_l3_exp2queue_map_get(jl_uint32 exp, jl_uint32 *pqueue)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 reg_val = 0;

	JL_ASSERT(exp <= MAX_EXP);
	JL_CHECK_POINTER(pqueue);

	reg_addr = SWCORE_BASE + MPLS_EXP_FIELD_TO_EGRESS_QUEUE_MAPPING_TABLE_OFFSET + exp;
	ret = jl_apb_reg_read(reg_addr, &reg_val);
	if (ret)
		return ret;

	*pqueue = GET_BITS(reg_val, 0, 1);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_l2_pcp2queue_map_set(jl_uint32 vlan_pcp, jl_uint32 queue)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 reg_val = 0;

	JL_ASSERT(vlan_pcp <= MAX_PCP);
	JL_ASSERT(queue < MAX_QUEUE_NUM);

	reg_addr = SWCORE_BASE + VLAN_PCP_TO_EGRESS_QUEUE_MAPPING_TABLE_OFFSET + vlan_pcp;
	ret = jl_apb_reg_read(reg_addr, &reg_val);
	if (ret)
		return ret;

	WRITE_BITS(reg_val, 0, 1, queue);
	ret = jl_apb_reg_write(reg_addr, reg_val);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_l2_pcp2queue_map_get(jl_uint32 vlan_pcp, jl_uint32 *pqueue)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 reg_val = 0;

	JL_ASSERT(vlan_pcp <= MAX_PCP);
	JL_CHECK_POINTER(pqueue);

	reg_addr = SWCORE_BASE + VLAN_PCP_TO_EGRESS_QUEUE_MAPPING_TABLE_OFFSET + vlan_pcp;
	ret = jl_apb_reg_read(reg_addr, &reg_val);
	if (ret)
		return ret;

	*pqueue = GET_BITS(reg_val, 0, 1);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_l2_port_pcp_set(jl_port_t port, jl_uint32 pcp)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint64 default_pcp = 0;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	JL_ASSERT(pcp <= MAX_PCP);

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	default_pcp = pcp;
	jl_write_bits(preg_val, default_pcp, 27, 3);

	ret = jl_apb_reg_burst_write(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_l2_port_pcp_get(jl_port_t port, jl_uint32 *ppcp)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint64 default_pcp = 0;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	JL_CHECK_POINTER(ppcp);

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &default_pcp, 27, 3);
	*ppcp = default_pcp;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_queue_pri_set(jl_port_t port, jl_uint32 queue, jl_uint32 priority)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 reg_val = 0;

	JL_ASSERT(queue < MAX_QUEUE_NUM);
	JL_ASSERT(priority < MAX_QUEUE_NUM); //four priority,same as queue

	reg_addr = SWCORE_BASE + MAP_QUEUE_TO_PRIORITY_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_read(reg_addr, &reg_val);
	JL_CHECK_RETURN(ret);

	WRITE_BITS(reg_val, (queue*2 + 0), (queue*2 + 1), priority);

	ret = jl_apb_reg_write(reg_addr, reg_val);
	JL_CHECK_RETURN(ret);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_queue_pri_get(jl_port_t port, jl_uint32 queue, jl_uint32 *ppriority)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 reg_val = 0;

	JL_ASSERT(queue < MAX_QUEUE_NUM);
	JL_CHECK_POINTER(ppriority);

	reg_addr = SWCORE_BASE + MAP_QUEUE_TO_PRIORITY_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_read(reg_addr, &reg_val);
	JL_CHECK_RETURN(ret);

	*ppriority = GET_BITS(reg_val, (queue*2 + 0), (queue*2 + 1));

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_queue_dwrr_set(jl_port_t port, jl_uint32 queue, jl_uint32 weight)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 reg_val = 0;

	JL_ASSERT(queue < MAX_QUEUE_NUM);
	JL_ASSERT(weight <= MAX_QUEUE_WEIGHT);

	reg_addr = SWCORE_BASE + DWRR_WEIGHT_CONFIGURATION_OFFSET + JL_PORT_P2M(port)*4 + queue;
	ret = jl_apb_reg_read(reg_addr, &reg_val);
	JL_CHECK_RETURN(ret);

	WRITE_BITS(reg_val, 0, 7, weight);

	ret = jl_apb_reg_write(reg_addr, reg_val);
	JL_CHECK_RETURN(ret);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_queue_dwrr_get(jl_port_t port, jl_uint32 queue, jl_uint32 *pweight)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 reg_val = 0;

	JL_ASSERT(queue < MAX_QUEUE_NUM);
	JL_CHECK_POINTER(pweight);

	reg_addr = SWCORE_BASE + DWRR_WEIGHT_CONFIGURATION_OFFSET + JL_PORT_P2M(port)*4 + queue;
	ret = jl_apb_reg_read(reg_addr, &reg_val);
	JL_CHECK_RETURN(ret);

	*pweight = GET_BITS(reg_val, 0, 7);

	return JL_ERR_OK;
}



static jl_ret_t jl51xx_qos_resource_info_get(void)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_ret_t ret2 = JL_ERR_OK;
	jl_uint8 port, idx;
	jl_uint32 reg_addr = 0;
	jl_uint32 reg_val = 0;

	for (idx = 0; idx < 4; idx++)
	{
		reg_addr = SWCORE_BASE + EGRESS_QUEUE_GUARANTEE_SET_OFFSET + idx;
		ret = jl_apb_reg_read(reg_addr, &reg_val);
		JL_CHECK_RETURN(ret);

		g_res_mngt.guarantee[idx] = reg_val;
	}

	JL_FOR_EACH_PHY_PORT(port) {
		reg_addr = SWCORE_BASE + EGRESS_RESOURCE_MANAGEMENT_MODE_OFFSET + JL_PORT_P2M(port);
		ret = jl_apb_reg_read(reg_addr, &reg_val);
		JL_CHECK_RETURN(ret);

		g_res_mngt.mng_mode[port] = reg_val & 0x1;
		g_res_mngt.grt_pointer[port] = (reg_val >> 1) & 0x3;

		reg_addr = SWCORE_BASE + ENABLE_ENQUEUE_TO_PORTS_AND_QUEUES_OFFSET + JL_PORT_P2M(port);
		ret = jl_apb_reg_read(reg_addr, &reg_val);
		JL_CHECK_RETURN(ret);

		g_res_mngt.en_mask[port] = reg_val & 0xf;
	}

	reg_addr = SWCORE_BASE + EGRESS_RESOURCE_MANAGEMENT_MODE_OFFSET + QOS_MULTICAST_RES_MANAGEMENT_ID;
	ret = jl_apb_reg_read(reg_addr, &reg_val);
	JL_CHECK_RETURN(ret);

	g_res_mngt.mng_mode[QOS_MULTICAST_RES_MANAGEMENT_ID] = reg_val & 0x1;
	g_res_mngt.grt_pointer[QOS_MULTICAST_RES_MANAGEMENT_ID] = (reg_val >> 1) & 0x3;

	reg_addr = SWCORE_BASE + EGRESS_RESOURCE_MANAGEMENT_MARGIN_OFFSET;
	ret = jl_apb_reg_read(reg_addr, &reg_val);
	JL_CHECK_RETURN(ret);

	g_res_mngt.global_margin = reg_val & 0x3ff;
	g_res_mngt.ffa_margin = (reg_val >> 10) & 0x3ff;


	if (g_res_mngt.guarantee[0] < 60)
	{
		ret = jl51xx_qos_drain_port(0x3ff);
		JL_CHECK_RETURN(ret);

		ret2 = jl51xx_qos_check_port_empty(0x3ff);
		if (ret2)
			goto recover;

		ret2 = jl51xx_qos_queue_guarantee_set(0, 60); //only index 0 is needed in this patch

recover:
		ret = jl51xx_qos_recover_port(0x3ff);
		JL_CHECK_RETURN(ret);
	}

	return ret2;
}


static jl_ret_t jl51xx_qos_drain_port(jl_uint16 l_mask)
{
	jl_uint32 reg_addr = 0;
	jl_uint32 c_mask = 0;
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t lport;

	//disable queues
	JL_FOR_EACH_PHY_PORT(lport) {
		if (CHECK_BIT(l_mask, lport))
		{
			reg_addr = SWCORE_BASE + ENABLE_ENQUEUE_TO_PORTS_AND_QUEUES_OFFSET + JL_PORT_P2M(lport);
			ret = jl_apb_reg_write(reg_addr, 0);
			JL_CHECK_RETURN(ret);
		}
	}
	//drain ports
	reg_addr = SWCORE_BASE + DRAIN_PORT_OFFSET;

	c_mask = JL_PORTMASK_P2M(l_mask);

	ret = jl_apb_reg_write(reg_addr, c_mask);
	JL_CHECK_RETURN(ret);

	return JL_ERR_OK;
}

static jl_ret_t jl51xx_qos_recover_port(jl_uint16 l_mask)
{
	jl_uint32 reg_addr = 0;
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t lport;

	//disable drain
	reg_addr = SWCORE_BASE + DRAIN_PORT_OFFSET;
	ret = jl_apb_reg_write(reg_addr, 0);
	JL_CHECK_RETURN(ret);

	//enable queues
	JL_FOR_EACH_PHY_PORT(lport) {
		if (CHECK_BIT(l_mask, lport))
		{
			reg_addr = SWCORE_BASE + ENABLE_ENQUEUE_TO_PORTS_AND_QUEUES_OFFSET + JL_PORT_P2M(lport);
			ret = jl_apb_reg_write(reg_addr, g_res_mngt.en_mask[lport]);
			JL_CHECK_RETURN(ret);
		}
	}

	return JL_ERR_OK;
}

static jl_ret_t jl51xx_qos_check_port_empty(jl_uint16 l_mask)
{
	jl_uint32 reg_addr = 0;
	jl_uint32 reg_val = 0;
	jl_portmask_t c_mask = 0;
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 i;

	reg_addr = SWCORE_BASE + PACKET_BUFFER_STATUS_OFFSET;

	c_mask = JL_PORTMASK_P2M(l_mask);

	for (i = 0; i < 10; i++) {
		ret = jl_apb_reg_read(reg_addr, &reg_val);
		JL_CHECK_RETURN(ret);

		if (((~reg_val) & c_mask) == 0) {
			return JL_ERR_OK;
		}
		port_udelay(50000);
	}

	return JL_ERR_BUSY;
}

static jl_ret_t jl51xx_qos_dwrr_res_check(void)
{
	jl_uint16 now_res = 0;
	jl_uint16 need_res = 0;
	jl_uint8 port;

	JL_FOR_EACH_PHY_PORT(port) {
		if (g_res_mngt.mng_mode[port] == QOS_RES_GUARANTEE) {
			now_res += (QOS_QUEUE_NUM * g_res_mngt.guarantee[0]);
		}
	}

	if (g_res_mngt.mng_mode[QOS_MULTICAST_RES_MANAGEMENT_ID] == QOS_RES_GUARANTEE) {
		now_res += g_res_mngt.guarantee[g_res_mngt.grt_pointer[QOS_MULTICAST_RES_MANAGEMENT_ID]];
	}

	need_res += (QOS_QUEUE_NUM * g_res_mngt.guarantee[0]);

	if ((now_res + need_res) > (QOS_RES_CELLS - g_res_mngt.global_margin - g_res_mngt.ffa_margin))
		return JL_ERR_RESOURCE;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_schedule_set(jl_uint8 port, jl_qos_schedule_t *psche)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_ret_t ret2 = JL_ERR_OK;
	jl_uint16 port_mask = 0;
	jl_uint8 i, dwrr_num = 0, ori_dwrr = 0;
	jl_uint8 q_prio = 0, is_change = 0;
	jl_uint8 crt_pri[QOS_QUEUE_NUM];
	jl_qos_schedule_t ori_sche;

	JL_CHECK_POINTER(psche);

	ret = jl51xx_qos_schedule_get(port, &ori_sche);
	JL_CHECK_RETURN(ret);

	for (i = 0; i < QOS_QUEUE_NUM; i++)
	{
		if (psche->type[i] > QOS_SCHE_DWRR)
			return JL_ERR_PARAM;

		if (ori_sche.type[i] != psche->type[i])
			is_change = 1;

		if (ori_sche.type[i] == QOS_SCHE_DWRR)
			ori_dwrr = 1;

		if (psche->type[i] == QOS_SCHE_DWRR)
		{
			crt_pri[i] = 3;
			dwrr_num++;
		}
		else
			crt_pri[i] = q_prio++;
	}

	/*check wether is invalid config*/
	if (dwrr_num == 1)
		return JL_ERR_CONFIG;

	/*check wether config is changed*/
	if (is_change == 0)
		return JL_ERR_OK;

	SET_BIT(port_mask,port);

	/*resource check is needed when change to guarantee*/
	if (dwrr_num && !ori_dwrr)
	{
		ret = jl51xx_qos_dwrr_res_check();
		JL_CHECK_RETURN(ret);
	}

	ret = jl51xx_qos_drain_port(port_mask);
	JL_CHECK_RETURN(ret);

	/*waiting for port empty*/
	ret2 = jl51xx_qos_check_port_empty(port_mask);

	if (ret2 == JL_ERR_OK)
	{
		/*when change to guarantee*/
		if (dwrr_num && !ori_dwrr)
		{
			ret2 = jl51xx_qos_buf_manage_set(port, QOS_RES_GUARANTEE);
			if (ret2)
				goto recover;
		}
		else if (!dwrr_num && ori_dwrr)  /*when change to limiter*/
		{
			ret2 = jl51xx_qos_buf_manage_set(port, QOS_RES_LIMITER);
			if (ret2)
				goto recover;
		}

		for (i = 0; i < QOS_QUEUE_NUM; i++)
		{
			ret2 = jl51xx_qos_queue_pri_set(port, i, crt_pri[i]);
			if (ret2)
				break;
		}
	}

recover:
	ret = jl51xx_qos_recover_port(port_mask);
	JL_CHECK_RETURN(ret);

	return ret2;
}


jl_ret_t jl51xx_qos_schedule_get(jl_uint8 port, jl_qos_schedule_t *psche)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32  crt_pri[QOS_QUEUE_NUM];
	jl_uint8 i = 0, dwrr_num = 0;

	JL_CHECK_POINTER(psche);

	for (i = 0; i < QOS_QUEUE_NUM; i++)
	{
		ret = jl51xx_qos_queue_pri_get(port, i, &crt_pri[i]);
		if (ret)
			return ret;
	}

	for (i = 0; i < QOS_QUEUE_NUM; i++)
	{
		psche->type[i] = QOS_SCHE_SP;

		if (crt_pri[i] == 3)
		{
			psche->type[i] = QOS_SCHE_DWRR;
			dwrr_num++;

			if ((i == 3) && (dwrr_num == 1))
				psche->type[i] = QOS_SCHE_SP;
		}
	}
	return JL_ERR_OK;
}



jl_ret_t jl51xx_qos_enqueue_set(jl_uint8 port,
								jl_uint8 enmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 val = 0;

	if (enmask > 0xf)
		return JL_ERR_PARAM;

	reg_addr = SWCORE_BASE + ENABLE_ENQUEUE_TO_PORTS_AND_QUEUES_OFFSET + JL_PORT_P2M(port);
	val = enmask;

	ret = jl_apb_reg_write(reg_addr, val);
	JL_CHECK_RETURN(ret);

	g_res_mngt.en_mask[port] = enmask;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_qos_enqueue_get(jl_uint8 port,
								jl_uint8 *penmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 val = 0;

	JL_CHECK_POINTER(penmask);

	reg_addr = SWCORE_BASE + ENABLE_ENQUEUE_TO_PORTS_AND_QUEUES_OFFSET + JL_PORT_P2M(port);

	ret = jl_apb_reg_read(reg_addr, &val);
	JL_CHECK_RETURN(ret);

	*penmask = val & 0xf;

	return JL_ERR_OK;
}
