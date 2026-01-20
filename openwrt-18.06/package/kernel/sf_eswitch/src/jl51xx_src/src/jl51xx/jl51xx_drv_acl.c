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
#include "acl.h"

jl_ret_t jl51xx_acl_check_para(jl_uint8 entry_index, jl_acl_rule_t *acl_rule, jl_acl_action_t *acl_action)
{
	JL_CHECK_POINTER(acl_rule);
	JL_CHECK_POINTER(acl_action);
	JL_CHECK_PORTMASK(acl_rule->ports);
	JL_CHECK_PORT(acl_action->dest_port);

	if (entry_index >= INGRESS_L2_ACL_MATCH_DATA_ENTRIES_NUM)
		return JL_ERR_PARAM;

	if (acl_action->drop_enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	if (acl_action->send_to_cpu_enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	if (acl_action->send_to_port_enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	if (acl_action->force_queue_enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	if (acl_action->update_pcp_enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	if (acl_action->update_vid_enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	if (acl_action->queue_id >= MAX_QUEUE_NUM)
		return JL_ERR_PARAM;

	if (acl_action->new_pcp_value > MAX_PCP)
		return JL_ERR_PARAM;

	if (acl_action->new_vid_value > MAX_VLAN_ID)
		return JL_ERR_PARAM;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_acl_set(jl_uint8 entry_index, jl_acl_rule_t *acl_rule, jl_acl_action_t *acl_action)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 in_l2_acl_match_val[8] = {0};
	jl_uint32 in_l2_acl_result_operation_val[2] = {0};
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = NULL;

	ret = jl51xx_acl_check_para(entry_index, acl_rule,  acl_action);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + entry_index * 8 + INGRESS_L2_ACL_MATCH_DATA_ENTRIES_OFFSET;
	preg_val = (jl_uint8 *)&in_l2_acl_match_val[0];
	jl_write_bits(preg_val, (jl_uint64)acl_rule->compare_ethtype, 0, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->type_of_comparison_ethtype, 1, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->eth_type, 2, 16);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->compare_da, 18, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->type_of_comparison_da, 19, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->da_mac, 20, 48);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->compare_sa, 68, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->type_of_comparison_sa, 69, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->sa_mac, 70, 48);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->compare_vid, 118, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->type_of_comparison_vid, 119, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->vid, 120, 12);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->compare_pcp, 132, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->type_of_comparison_pcp, 133, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->pcp, 134, 3);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->compare_dei, 137, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->type_of_comparison_dei, 138, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->dei, 139, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->compare_has_vlans, 140, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->type_of_comparison_has_vlans, 141, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->has_vlans, 142, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->compare_cstag, 143, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->type_of_comparison_cstag, 144, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_rule->cstag, 145, 1);
	jl_write_bits(preg_val, (jl_uint64)JL_PORTMASK_P2M(acl_rule->ports), 146, 10);
	ret = jl_apb_reg_burst_write(reg_addr, &in_l2_acl_match_val[0], 8);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + entry_index * 2 + INGRESS_L2_ACL_RESULT_OPERATION_ENTRIES_OFFSET;
	preg_val = (jl_uint8 *)&in_l2_acl_result_operation_val[0];
	ret = jl_apb_reg_burst_read(reg_addr, &in_l2_acl_result_operation_val[0], 2);
	if (ret)
		return ret;

	jl_write_bits(preg_val, (jl_uint64)acl_action->drop_enable, 0, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_action->send_to_cpu_enable, 1, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_action->force_queue_enable, 2, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_action->queue_id, 3, 2);
	jl_write_bits(preg_val, (jl_uint64)acl_action->send_to_port_enable, 5, 1);
	jl_write_bits(preg_val, (jl_uint64)JL_PORT_P2M(acl_action->dest_port), 6, 4);
	jl_write_bits(preg_val, (jl_uint64)acl_action->update_pcp_enable, 24, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_action->new_pcp_value, 25, 3);
	jl_write_bits(preg_val, (jl_uint64)acl_action->update_vid_enable, 28, 1);
	jl_write_bits(preg_val, (jl_uint64)acl_action->new_vid_value, 29, 12);
	ret = jl_apb_reg_burst_write(reg_addr, &in_l2_acl_result_operation_val[0], 2);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_acl_get(jl_uint8 entry_index, jl_acl_rule_t *acl_rule, jl_acl_action_t *acl_action)
{
	jl_ret_t ret;
	jl_uint32 in_l2_acl_match_val[8] = {0};
	jl_uint32 in_l2_acl_result_operation_val[2] = {0};
	jl_uint32 reg_addr = 0;
	jl_uint64 val = 0;
	jl_uint8 *preg_val = NULL;

	JL_CHECK_POINTER(acl_rule);
	JL_CHECK_POINTER(acl_action);

	if (entry_index >= INGRESS_L2_ACL_MATCH_DATA_ENTRIES_NUM)
		return JL_ERR_PARAM;

	do {
		reg_addr = SWCORE_BASE + entry_index * 8 + INGRESS_L2_ACL_MATCH_DATA_ENTRIES_OFFSET;
		preg_val = (jl_uint8 *)&in_l2_acl_match_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &in_l2_acl_match_val[0], 8);
		if (ret)
			return ret;

		jl_read_bits(preg_val, &val, 0, 1);
		acl_rule->compare_ethtype = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 1, 1);
		acl_rule->type_of_comparison_ethtype = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 2, 16);
		acl_rule->eth_type = val&0xffff;

		val = 0;
		jl_read_bits(preg_val, &val, 18, 1);
		acl_rule->compare_da = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 19, 1);
		acl_rule->type_of_comparison_da = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 20, 48);
		acl_rule->da_mac = val;

		val = 0;
		jl_read_bits(preg_val, &val, 68, 1);
		acl_rule->compare_sa = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 69, 1);
		acl_rule->type_of_comparison_sa = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 70, 48);
		acl_rule->sa_mac = val;

		val = 0;
		jl_read_bits(preg_val, &val, 118, 1);
		acl_rule->compare_vid = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 119, 1);
		acl_rule->type_of_comparison_vid = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 120, 12);
		acl_rule->vid = val&0xfff;

		val = 0;
		jl_read_bits(preg_val, &val, 132, 1);
		acl_rule->compare_pcp = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 133, 1);
		acl_rule->type_of_comparison_pcp = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 134, 3);
		acl_rule->pcp = val&0x7;

		val = 0;
		jl_read_bits(preg_val, &val, 137, 1);
		acl_rule->compare_dei = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 138, 1);
		acl_rule->type_of_comparison_dei = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 139, 1);
		acl_rule->dei = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 140, 1);
		acl_rule->compare_has_vlans = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 141, 1);
		acl_rule->type_of_comparison_has_vlans = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 142, 1);
		acl_rule->has_vlans = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 143, 1);
		acl_rule->compare_cstag = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 144, 1);
		acl_rule->type_of_comparison_cstag = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 145, 1);
		acl_rule->cstag = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 146, 10);
		acl_rule->ports = JL_PORTMASK_M2P(val&0x3ff);
	} while (0);

	do {
		reg_addr = SWCORE_BASE + entry_index * 2 + INGRESS_L2_ACL_RESULT_OPERATION_ENTRIES_OFFSET;
		preg_val = (jl_uint8 *)&in_l2_acl_result_operation_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &in_l2_acl_result_operation_val[0], 2);
		if (ret)
			return ret;

		val = 0;
		jl_read_bits(preg_val, &val, 0, 1);
		acl_action->drop_enable = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 1, 1);
		acl_action->send_to_cpu_enable = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 2, 1);
		acl_action->force_queue_enable = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 3, 2);
		acl_action->queue_id = val&0x3;

		val = 0;
		jl_read_bits(preg_val, &val, 5, 1);
		acl_action->send_to_port_enable = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 6, 4);
		acl_action->dest_port = JL_PORT_M2P(val&0xf);

		val = 0;
		jl_read_bits(preg_val, &val, 24, 1);
		acl_action->update_pcp_enable = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 25, 3);
		acl_action->new_pcp_value = val&0x7;

		val = 0;
		jl_read_bits(preg_val, &val, 28, 1);
		acl_action->update_vid_enable = val&0x1;

		val = 0;
		jl_read_bits(preg_val, &val, 29, 12);
		acl_action->new_vid_value = val&0xfff;

	} while (0);

	return JL_ERR_OK;
}

