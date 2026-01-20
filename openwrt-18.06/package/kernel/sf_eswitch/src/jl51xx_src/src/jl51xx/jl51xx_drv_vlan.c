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

#include "jl51xx/jl51xx_drv_vlan.h"

jl_ret_t __vlan_table_reset(void)
{
	jl_ret_t ret;
	jl_uint32 v;
	jl_uint64 valid, in_vmask, in_vid, v_portmask, gid;
	jl_uint32 vlan_tab_val[2];
	jl_uint32 v2v_tab_tcam_val[1];
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = NULL;

	/* create Default VLAN Entry & Drop VLAN Entry */
	for (v = 0; v <= VLAN_IDX_MAX; v++) {
		if (v == VLAN_IDX_DEFAULT) {
			valid = 1;
			in_vid = DEFAULT_VLAN_ID;
			in_vmask = 0xfff;
			v_portmask = 0x3ff;
		} else if (v == VLAN_IDX_DROP) {
			valid = 1;
			in_vid = 0;
			in_vmask = 0;
			v_portmask = 0;
		} else {
			valid = 0;
			in_vid = 0;
			in_vmask = 0xfff;
			v_portmask = 0;
		}

		gid = 0;

		do {
			reg_addr = SWCORE_BASE + VLAN_TABLE_OFFSET + v * 2;
			preg_val = (jl_uint8 *)&vlan_tab_val[0];
			ret = jl_apb_reg_burst_read(reg_addr, &vlan_tab_val[0], 2);
			if (ret)
				return ret;

			jl_write_bits(preg_val, v_portmask, 0, 10);
			jl_write_bits(preg_val, gid, 10, 4);

			ret = jl_apb_reg_burst_write(reg_addr, &vlan_tab_val[0], 2);
			if (ret)
				return ret;
		} while (0);

		do {
			reg_addr = SWCORE_BASE + VID_TO_VLAN_TABLE_TCAM_OFFSET + v * 1;
			preg_val = (jl_uint8 *)&v2v_tab_tcam_val[0];
			ret = jl_apb_reg_burst_read(reg_addr, &v2v_tab_tcam_val[0], 1);
			if (ret)
				return ret;

			jl_write_bits(preg_val, valid, 0, 1);
			jl_write_bits(preg_val, in_vmask, 1, 12);
			jl_write_bits(preg_val, in_vid, 13, 12);

			ret = jl_apb_reg_burst_write(reg_addr, &v2v_tab_tcam_val[0], 1);
			if (ret)
				return ret;
		} while (0);
	}

	return JL_ERR_OK;
}

static jl_ret_t __vlan_prepare(void)
{
	jl_ret_t ret;
	jl_port_t port;
	jl_uint32 v;
	jl_uint64 use_lookup, miss_vid, answer;
	jl_uint32 in_vid_trans_val[1];
	jl_uint32 vid2vtab_tcam_answer_val[1];
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = NULL;

	for (port = 0; port < SWITCH_PORT_NUM; port++) {
		do {
			reg_addr = SWCORE_BASE + INGRESS_VID_TRANSLATION_ACTION_OFFSET + port * 1;
			preg_val = (jl_uint8 *)&in_vid_trans_val[0];
			ret = jl_apb_reg_burst_read(reg_addr, &in_vid_trans_val[0], 1);
			if (ret)
				return ret;

			use_lookup = 1;
			miss_vid = VLAN_IDX_DROP;
			jl_write_bits(preg_val, use_lookup, 0, 1);
			jl_write_bits(preg_val, miss_vid, 1, 12);

			ret = jl_apb_reg_burst_write(reg_addr, &in_vid_trans_val[0], 1);
			if (ret)
				return ret;
		} while (0);
	}

	/* bind vlan-index to TCAMAnswer table*/
	for (v = 0; v <= VLAN_IDX_MAX; v++) {
		do {
			reg_addr = SWCORE_BASE + VID_TO_VLAN_TABLE_TCAM_ANSWER_OFFSET + v * 1 ;
			preg_val = (jl_uint8 *)&vid2vtab_tcam_answer_val[0];
			ret = jl_apb_reg_burst_read(reg_addr, &vid2vtab_tcam_answer_val[0], 1);
			if (ret)
				return ret;

			answer = v;
			jl_write_bits(preg_val, answer, 0, 4);

			ret = jl_apb_reg_burst_write(reg_addr, &vid2vtab_tcam_answer_val[0], 1);
			if (ret)
				return ret;
		} while (0);
	}

	return JL_ERR_OK;
}

static jl_ret_t __check_vlan_index(jl_uint32 vlan_index, jl_uint8 is_setting)
{
	if (vlan_index > VLAN_IDX_MAX)
		return JL_ERR_PARAM;

	if (!is_setting)
		return JL_ERR_OK;

	/* not allow to change index for drop vidx */
	if (vlan_index == VLAN_IDX_DROP)
		return JL_ERR_PARAM;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_vlan_init(void)
{
	jl_ret_t ret;
	jl_port_t port;

	ret = __vlan_prepare();
	if (ret)
		return ret;

	ret = __vlan_table_reset();
	if (ret)
		return ret;

	/* set all ports default vid = 1 */
	JL_FOR_EACH_PHY_PORT(port) {
		ret = jl51xx_port_vlan_set(port, DEFAULT_VLAN_ID);
		if (ret)
			return ret;
	}

	return JL_ERR_OK;
}

jl_ret_t jl51xx_vlan_deinit(void)
{
	jl_ret_t ret;
	jl_port_t port;
	jl_uint64 answer;
	jl_uint32 vid2vtab_tcam_answer_val[1];
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = NULL;

	JL_FOR_EACH_PHY_PORT(port) {
		/* reset vlan assignment */
		ret = jl51xx_port_vlan_assignment_set(port, VLAN_ASSIGN_PKT);
		if (ret)
			return ret;

		/* reset vlan ingress operation */
		ret = jl51xx_port_vlan_ingress_operation_set(port, VLAN_OPERATION_NONE);
		if (ret)
			return ret;

		/* reset vlan egress operation */
		ret = jl51xx_port_vlan_egress_operation_set(port, VLAN_OPERATION_NONE);
		if (ret)
			return ret;

		/* set all ports default vid = 1 */
		ret = jl51xx_port_vlan_set(port, DEFAULT_VLAN_ID);
		if (ret)
			return ret;
	}

	ret = __vlan_table_reset();
	if (ret)
		return ret;

	/* disable vlan */
	do {
		reg_addr = SWCORE_BASE + VID_TO_VLAN_TABLE_TCAM_ANSWER_OFFSET + VLAN_IDX_DROP * 1 ;
		preg_val = (jl_uint8 *)&vid2vtab_tcam_answer_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &vid2vtab_tcam_answer_val[0], 1);
		if (ret)
			return ret;

		answer = VLAN_IDX_DEFAULT;
		jl_write_bits(preg_val, answer, 0, 4);

		ret = jl_apb_reg_burst_write(reg_addr, &vid2vtab_tcam_answer_val[0], 1);
		if (ret)
			return ret;
	} while (0);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_vlan_set(jl_uint32 vlan_index, struct vlan_config *config)
{
	jl_ret_t ret;
	jl_uint64 valid, in_vid, fid, vportmask, operation, vid_sel, new_vid;
	jl_uint32 vid2vtab_tcam_val[1];
	jl_uint32 vtab_val[2];
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = NULL;

	JL_CHECK_POINTER(config);
	JL_CHECK_PORTMASK(config->port_mask);
	JL_ASSERT(config->operation < VLAN_OPERATION_END);
	JL_ASSERT(config->vid_sel < VLANID_INHERIT_FROM_END);
	JL_ASSERT(config->vid <= MAX_VLAN_ID);
	JL_ASSERT(config->new_vid <= MAX_VLAN_ID);

	if (config->fid > VLAN_IDX_MAX)
		return JL_ERR_PARAM;

	if (__check_vlan_index(vlan_index, 1))
		return JL_ERR_PARAM;

	do {
		reg_addr = SWCORE_BASE + VLAN_TABLE_OFFSET + vlan_index * 2;
		preg_val = (jl_uint8 *)&vtab_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &vtab_val[0], 2);
		if (ret)
			return ret;

		vportmask = JL_PORTMASK_P2M(config->port_mask);
		fid = config->fid;
		jl_write_bits(preg_val, vportmask, 0, 10);
		jl_write_bits(preg_val, fid, 10, 4);

		operation = config->operation;
		vid_sel = config->vid_sel;
		new_vid = config->new_vid;
		jl_write_bits(preg_val, operation, 14, 3);
		jl_write_bits(preg_val, vid_sel, 17, 2);
		jl_write_bits(preg_val, new_vid, 25, 12);

		ret = jl_apb_reg_burst_write(reg_addr, &vtab_val[0], 2);
		if (ret)
			return ret;
	} while (0);

	do {
		reg_addr = SWCORE_BASE + VID_TO_VLAN_TABLE_TCAM_OFFSET + vlan_index * 1;
		preg_val = (jl_uint8 *)&vid2vtab_tcam_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &vid2vtab_tcam_val[0], 1);
		if (ret)
			return ret;

		valid = config->valid;
		in_vid = config->vid;
		jl_write_bits(preg_val, valid, 0, 1);
		jl_write_bits(preg_val, in_vid, 13, 12);

		ret = jl_apb_reg_burst_write(reg_addr, &vid2vtab_tcam_val[0], 1);
		if (ret)
			return ret;
	} while (0);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_vlan_get(jl_uint32 vlan_index, struct vlan_config *config)
{
	jl_ret_t ret;
	jl_uint64 vid, in_vid, fid, vportmask, operation, vid_sel, new_vid;
	jl_uint32 vid2vtab_tcam_val[1];
	jl_uint32 vtab_val[2];
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = NULL;

	if (__check_vlan_index(vlan_index, 0))
		return JL_ERR_PARAM;

	do {
		reg_addr = SWCORE_BASE + VID_TO_VLAN_TABLE_TCAM_OFFSET + vlan_index * 1;
		preg_val = (jl_uint8 *)&vid2vtab_tcam_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &vid2vtab_tcam_val[0], 1);
		if (ret)
			return ret;

		jl_read_bits(preg_val, &vid, 0, 1);
		jl_read_bits(preg_val, &in_vid, 13, 12);

		config->valid = vid;
		config->vid = in_vid;
	} while (0);

	do {
		reg_addr = SWCORE_BASE + VLAN_TABLE_OFFSET + vlan_index * 2;
		preg_val = (jl_uint8 *)&vtab_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &vtab_val[0], 2);
		if (ret)
			return ret;

		jl_read_bits(preg_val, &vportmask, 0, 10);
		jl_read_bits(preg_val, &fid, 10, 4);
		config->port_mask = JL_PORTMASK_M2P(vportmask);
		config->fid = fid;

		jl_read_bits(preg_val, &operation, 14, 3);
		jl_read_bits(preg_val, &vid_sel, 17, 2);
		jl_read_bits(preg_val, &new_vid, 25, 12);
		config->operation = operation;
		config->vid_sel = vid_sel;
		config->new_vid = new_vid;
	} while (0);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_vlan_set_batch(struct vlan_item *v_list, uint16_t size)
{
	jl_ret_t ret;
	jl_uint32 i;

	for (i = 0; i < size; i++) {
		jl_uint32 idx = v_list[i].index;
		struct vlan_config *cfg = &v_list[i].config;

		ret = jl51xx_vlan_set(idx, cfg);
		if (ret)
			return ret;
	}

	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_vlan_set(jl_port_t port, jl_uint32 vid)
{
	jl_ret_t ret;
	jl_uint64 default_vid;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	default_vid = vid;
	jl_write_bits(preg_val, default_vid, 14, 12);

	ret = jl_apb_reg_burst_write(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_vlan_get(jl_port_t port, jl_uint32 *vid)
{
	jl_ret_t ret;
	jl_uint64 default_vid;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &default_vid, 14, 12);
	*vid = default_vid;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_vlan_assignment_set(jl_port_t port, enum vlan_assignment opt)
{
	jl_ret_t ret;
	jl_uint64 vlan_assign;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	if (opt > VLAN_ASSIGN_MIXED)
		return JL_ERR_PARAM;

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	vlan_assign = opt;
	jl_write_bits(preg_val, vlan_assign, 12, 2);

	ret = jl_apb_reg_burst_write(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_vlan_assignment_get(jl_port_t port, enum vlan_assignment *opt)
{
	jl_ret_t ret;
	jl_uint64 vlan_assign;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &vlan_assign, 12, 2);
	*opt = vlan_assign;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_vlan_ingress_operation_set(jl_uint32 port, enum vlan_operation opt)
{
	jl_ret_t ret;
	jl_uint64 vlan_single_op, vidsel;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	if (opt > VLAN_OPERATION_REMOVE_ALL)
		return JL_ERR_PARAM;

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	vlan_single_op = opt;
	vidsel = 1; // From source port table entry’s defaultVid.
	jl_write_bits(preg_val, vlan_single_op, 1, 3);
	jl_write_bits(preg_val, vidsel, 4, 2);

	ret = jl_apb_reg_burst_write(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_vlan_ingress_operation_get(jl_uint32 port, enum vlan_operation *opt)
{
	jl_ret_t ret;
	jl_uint64 vlan_single_op;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &vlan_single_op, 1, 3);
	*opt = vlan_single_op;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_vlan_egress_operation_set(jl_uint32 port, enum vlan_operation opt)
{
	jl_ret_t ret;
	jl_uint64 vlan_single_op;
	jl_uint32 egress_port_tab_val[1];
	jl_uint32 reg_addr = SWCORE_BASE + EGRESS_PORT_CONFIGURATION_OFFSET + JL_PORT_P2M(port)*1;
	jl_uint8 *preg_val = (jl_uint8 *)&egress_port_tab_val[0];

	if ((opt  ==  VLAN_OPERATION_SWAP) || (opt  ==  VLAN_OPERATION_PUSH))
		return JL_ERR_PARAM;

	ret = jl_apb_reg_burst_read(reg_addr, &egress_port_tab_val[0], 1);
	if (ret)
		return ret;

	vlan_single_op = opt;
	jl_write_bits(preg_val, vlan_single_op, 0, 3);

	ret = jl_apb_reg_burst_write(reg_addr, &egress_port_tab_val[0], 1);
	if (ret)
		return ret;


	return JL_ERR_OK;
}

jl_ret_t jl51xx_port_vlan_egress_operation_get(jl_uint32 port, enum vlan_operation *opt)
{
	jl_ret_t ret;
	jl_uint64 vlan_single_op;
	jl_uint32 egress_port_tab_val[1];
	jl_uint32 reg_addr = SWCORE_BASE + EGRESS_PORT_CONFIGURATION_OFFSET + JL_PORT_P2M(port) * 1;
	jl_uint8 *preg_val = (jl_uint8 *)&egress_port_tab_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &egress_port_tab_val[0], 1);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &vlan_single_op, 0, 3);
	*opt = vlan_single_op;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_vlan_acpt_frmt_set(jl_uint32 port, jl_vlan_acpt_frmt_t frmt)
{
	jl_ret_t ret;
	jl_uint64 min_allowed_vlans;
	jl_uint64 max_allowed_vlans;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	if (frmt == VLAN_ACCEPT_FRAME_TYPE_ALL) {
		min_allowed_vlans = 0;  /*0 = All packets are accepted*/
		max_allowed_vlans = 2;  /*2 = Any number of VLANs are accepted*/
	} else if (frmt == VLAN_ACCEPT_FRAME_TYPE_TAG_ONLY) {
		min_allowed_vlans = 1;  /*1 = 1 or more tags are accepted*/
		max_allowed_vlans = 2;  /*2 = Any number of VLANs are accepted*/
	} else if (frmt == VLAN_ACCEPT_FRAME_TYPE_UNTAG_ONLY) {
		min_allowed_vlans = 0;  /*0 = All packets are accepted*/
		max_allowed_vlans = 0;  /*0 = Only untagged packets are accepted*/
	} else
		return JL_ERR_PARAM;

	jl_write_bits(preg_val, min_allowed_vlans, 30, 2);
	jl_write_bits(preg_val, max_allowed_vlans, 32, 2);

	ret = jl_apb_reg_burst_write(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_vlan_acpt_frmt_get(jl_uint32 port, jl_vlan_acpt_frmt_t *pfrmt)
{
	jl_ret_t ret;
	jl_uint64 min_allowed_vlans;
	jl_uint64 max_allowed_vlans;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &min_allowed_vlans, 30, 2);
	jl_read_bits(preg_val, &max_allowed_vlans, 32, 2);

	if ((min_allowed_vlans == 0) && (max_allowed_vlans == 2)) {
		*pfrmt = VLAN_ACCEPT_FRAME_TYPE_ALL;
	} else if ((min_allowed_vlans == 1) && (max_allowed_vlans == 2)) {
		*pfrmt = VLAN_ACCEPT_FRAME_TYPE_TAG_ONLY;
	} else if ((min_allowed_vlans == 0) && (max_allowed_vlans == 0)) {
		*pfrmt = VLAN_ACCEPT_FRAME_TYPE_UNTAG_ONLY;
	} else {
		*pfrmt = VLAN_ACCEPT_FRAME_TYPE_END;
		return JL_ERR_PARAM;
	}

	return JL_ERR_OK;
}
