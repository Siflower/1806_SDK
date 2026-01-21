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
#include "driver/jl_reg_io.h"
#include "jl51xx/reg_ethmac.h"
#include "jl51xx/jl51xx_drv_mib.h"
#include "mib.h"

#define ETHMAC_REG_ADDR(idx, ofs)			(ETHMAC0_BASE + idx * ETHMAC0_BLOCK_SIZE + ofs)
jl_ret_t jl51xx_mib_port_rc_mode_get(jl_port_t port, jl_enable_t *pregval)
{
	jl_ret_t ret;
	jl_uint32 val;

	ret = jl_apb_reg_read(ETHMAC_REG_ADDR(JL_PORT_P2M(port), STATISTIC_MODE_0_OFFSET),
					&val);
	if (ret)
		return ret;

	if (val & BIT(0))
		*pregval = 1;
	else
		*pregval = 0;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_mib_port_rc_mode_set(jl_port_t port, jl_uint32 regval)
{
	jl_ret_t ret;
	jl_uint32 val;

	ret = jl_apb_reg_read(ETHMAC_REG_ADDR(JL_PORT_P2M(port), STATISTIC_MODE_0_OFFSET),
					&val);
	if (ret)
		return ret;

	if (regval == 1)
		SET_BIT(val, 0);
	else if (regval == 0)
		CLR_BIT(val, 0);
	else
		return JL_ERR_PARAM;

	ret = jl_apb_reg_write(
					ETHMAC_REG_ADDR(JL_PORT_P2M(port), STATISTIC_MODE_0_OFFSET),
					val);
	return ret;
}

jl_ret_t jl51xx_mib_port_clear(jl_port_t port, jl_uint16 type)
{
	jl_ret_t ret;
	jl_uint32 val = 0;

	val = (jl_uint32)type << 4;
	val |= BIT(0);

	ret = jl_apb_reg_write(
					ETHMAC_REG_ADDR(JL_PORT_P2M(port), STATISTIC_MODE_1_OFFSET),
					val);
	return ret;
}
#define MIB_TYPE_END		21
jl_ret_t jl51xx_mib_port_get(jl_port_t port, jl_uint16 type, jl_uint64 *pregval)
{
	jl_ret_t ret;
	jl_uint32 burst = 0;
	jl_uint32 buf[2] = {0};
	jl_uint32 *ptr = (jl_uint32 *)pregval;

	jl_uint32 mib_base_addr = ETHMAC_REG_ADDR(JL_PORT_P2M(port), TX_PKT_CNT_OFFSET);
	CONST_T jl_uint8 mib_reg_offset[MIB_TYPE_END] = {
		0, 2, 4, 6, 8, 10, 12, 14, 16, 17,
		18, 20, 22, 24, 26, 28, 30, 32, 34, 35, 36 };

	if (type >= MIB_TYPE_END)
		return JL_ERR_PARAM;

	switch(type) {
		case PORT_MIB_TX_PKT_CNT:
		case PORT_MIB_TX_PKT_BYTE_CNT:
		case PORT_MIB_TX_PKT_UC_CNT:
		case PORT_MIB_TX_PKT_UC_BYTE_CNT:
		case PORT_MIB_TX_PKT_MC_CNT:
		case PORT_MIB_TX_PKT_MC_BYTE_CNT:
		case PORT_MIB_TX_PKT_BC_CNT:
		case PORT_MIB_TX_PKT_BC_BYTE_CNT:
		case PORT_MIB_RX_PKT_CNT:
		case PORT_MIB_RX_PKT_BYTE_CNT:
		case PORT_MIB_RX_PKT_UC_CNT:
		case PORT_MIB_RX_PKT_UC_BYTE_CNT:
		case PORT_MIB_RX_PKT_MC_CNT:
		case PORT_MIB_RX_PKT_MC_BYTE_CNT:
		case PORT_MIB_RX_PKT_BC_CNT:
		case PORT_MIB_RX_PKT_BC_BYTE_CNT:
			burst = 2;
			break;
		case PORT_MIB_TX_PKT_DRP_CNT:
		case PORT_MIB_TX_PKT_ERR_CNT:
		case PORT_MIB_RX_PKT_DRP_CNT:
		case PORT_MIB_RX_PKT_CRC_ERR_CNT:
		case PORT_MIB_RX_FRAG_ERR_CNT:
			burst = 1;
			break;
		default:
			JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ERROR, "mib type overflow!\n");
			break;
	}
	ret = jl_apb_reg_burst_read(mib_base_addr + mib_reg_offset[type], buf, burst);
	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "%s regaddr=0x%4x, buf[0]=0x%4x, buf[1]=0x%4x, burst=%d errmsg:%s!\n",
			__func__, mib_base_addr + mib_reg_offset[type],
			buf[0], buf[1], burst, JL_ERRMSG(ret));

	*ptr++= buf[0];
	*ptr = buf[1];

	return ret;
}

jl_ret_t jl51xx_mib_swc_global_drop_get(jl_mib_swc_glb_drop_t *pport_cnt)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 val = 0;

	JL_CHECK_POINTER(pport_cnt);

	reg_addr = SWCORE_BASE + IPP_BROKEN_PACKETS_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &val, 1);
	if (ret)
		return ret;
	pport_cnt->ipp_broken_pkt = val;

	val = 0;
	reg_addr = SWCORE_BASE + IPP_PM_DROP_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &val, 1);
	if (ret)
		return ret;
	pport_cnt->ipp_pm_drop = val;

	val = 0;
	reg_addr = SWCORE_BASE + EPP_PM_DROP_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &val, 1);
	if (ret)
		return ret;
	pport_cnt->epp_pm_drop = val;

	val = 0;
	reg_addr = SWCORE_BASE + UNKNOWN_INGRESS_DROP_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &val, 1);
	if (ret)
		return ret;
	pport_cnt->unknown_ingress_drop = val;

	val = 0;
	reg_addr = SWCORE_BASE + EMPTY_MASK_DROP_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &val, 1);
	if (ret)
		return ret;
	pport_cnt->empty_mask_drop = val;

	val = 0;
	reg_addr = SWCORE_BASE + INGRESS_SPANNING_TREE_DROP_LISTEN_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &val, 1);
	if (ret)
		return ret;
	pport_cnt->ingress_stp_listen_drop = val;

	val = 0;
	reg_addr = SWCORE_BASE + INGRESS_SPANNING_TREE_DROP_LEARNING_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &val, 1);
	if (ret)
		return ret;
	pport_cnt->ingress_stp_learn_drop = val;

	val = 0;
	reg_addr = SWCORE_BASE + INGRESS_SPANNING_TREE_DROP_BLOCKING_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &val, 1);
	if (ret)
		return ret;
	pport_cnt->ingress_stp_block_drop = val;

	val = 0;
	reg_addr = SWCORE_BASE + L2_FLAG_DROP_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &val, 1);
	if (ret)
		return ret;
	pport_cnt->l2_lookup_drop = val;

	val = 0;
	reg_addr = SWCORE_BASE + INGRESS_L2_ACL_DROP_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &val, 1);
	if (ret)
		return ret;
	pport_cnt->ingress_config_acl_drop = val;

	val = 0;
	reg_addr = SWCORE_BASE + VLAN_MEMBER_DROP_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &val, 1);
	if (ret)
		return ret;
	pport_cnt->vlan_member_drop = val;

	val = 0;
	reg_addr = SWCORE_BASE + MINIMUM_ALLOWED_VLAN_DROP_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &val, 1);
	if (ret)
		return ret;
	pport_cnt->mini_allow_vlan_drop = val;

	val = 0;
	reg_addr = SWCORE_BASE + MAXIMUM_ALLOWED_VLAN_DROP_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &val, 1);
	if (ret)
		return ret;
	pport_cnt->max_allow_vlan_drop = val;

	val = 0;
	reg_addr = SWCORE_BASE + BUFFER_OVERFLOW_DROP_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &val, 1);
	if (ret)
		return ret;
	pport_cnt->buf_overflow_drop = val;

	val = 0;
	reg_addr = SWCORE_BASE + INGRESS_RESOURCE_MANAGER_DROP_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &val, 1);
	if (ret)
		return ret;
	pport_cnt->ingress_res_manager_drop = val;

	val = 0;
	reg_addr = SWCORE_BASE + RE_QUEUE_OVERFLOW_DROP_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &val, 1);
	if (ret)
		return ret;
	pport_cnt->re_queue_overflow_drop = val;

	return ret;
}

jl_ret_t jl51xx_mib_swc_global_drop_clear(void)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 val = 0;

	reg_addr = SWCORE_BASE + IPP_BROKEN_PACKETS_OFFSET;
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + IPP_PM_DROP_OFFSET;
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + EPP_PM_DROP_OFFSET;
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + UNKNOWN_INGRESS_DROP_OFFSET;
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + EMPTY_MASK_DROP_OFFSET;
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + INGRESS_SPANNING_TREE_DROP_LISTEN_OFFSET;
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + INGRESS_SPANNING_TREE_DROP_LEARNING_OFFSET;
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + INGRESS_SPANNING_TREE_DROP_BLOCKING_OFFSET;
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + L2_FLAG_DROP_OFFSET;
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + INGRESS_L2_ACL_DROP_OFFSET;
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + VLAN_MEMBER_DROP_OFFSET;
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + MINIMUM_ALLOWED_VLAN_DROP_OFFSET;
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + MAXIMUM_ALLOWED_VLAN_DROP_OFFSET;
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + BUFFER_OVERFLOW_DROP_OFFSET;
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + INGRESS_RESOURCE_MANAGER_DROP_OFFSET;
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + RE_QUEUE_OVERFLOW_DROP_OFFSET;
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);

	return ret;
}

jl_ret_t jl51xx_mib_swc_port_drop_get(jl_port_t port, jl_mib_swc_port_drop_t *pport_cnt)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 ps_error_val[2] = {0};
	jl_uint8 *preg_val = (jl_uint8 *)&ps_error_val[0];
	jl_uint64 data1 = 0;
	jl_uint64 data2 = 0;

	/* Check Port Valid */
	JL_CHECK_PORT(port);
	JL_CHECK_POINTER(pport_cnt);

	reg_addr = SWCORE_BASE + SP_OVERFLOW_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_read(reg_addr, &pport_cnt->sp_overflow_drop, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + PS_ERROR_COUNTER_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_read(reg_addr, &ps_error_val[0], 2);
	if (ret)
		return ret;
	jl_read_bits(preg_val, &data1, 0, 24);
	jl_read_bits(preg_val, &data2, 24, 24);
	pport_cnt->ps_err_underrun = data1&0xffffff;
	pport_cnt->ps_err_overflow = data2&0xffffff;

	reg_addr = SWCORE_BASE + UNKNOWN_EGRESS_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_read(reg_addr, &pport_cnt->unknown_egress_drop, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + EGRESS_PORT_DISABLED_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_read(reg_addr, &pport_cnt->egress_port_dis_drop, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + QUEUE_OFF_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_read(reg_addr, &pport_cnt->queue_off_drop, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + EGRESS_SPANNING_TREE_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_read(reg_addr, &pport_cnt->egress_stp_drop, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + MBSC_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_read(reg_addr, &pport_cnt->mbsc_drop, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + DRAIN_PORT_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_read(reg_addr, &pport_cnt->drain_port_drop, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + MAC_RX_BROKEN_PACKETS_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_read(reg_addr, &pport_cnt->mac_rx_broken_pkt, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + MAC_RX_SHORT_PACKET_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_read(reg_addr, &pport_cnt->mac_rx_short_pkt_drop, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + EGRESS_RESOURCE_MANAGEMENT_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_read(reg_addr, &pport_cnt->egress_res_manage_drop, 1);

	return ret;
}

jl_ret_t jl51xx_mib_swc_port_drop_clear(jl_port_t port)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg_addr = 0;
	jl_uint32 val = 0;
	jl_uint32 ps_error_val[2] = {0};

	/* Check Port Valid */
	JL_CHECK_PORT(port);

    reg_addr = SWCORE_BASE + SP_OVERFLOW_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

    reg_addr = SWCORE_BASE + PS_ERROR_COUNTER_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_write(reg_addr, &ps_error_val[0], 2);
	if (ret)
		return ret;

    reg_addr = SWCORE_BASE + UNKNOWN_EGRESS_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

    reg_addr = SWCORE_BASE + EGRESS_PORT_DISABLED_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + QUEUE_OFF_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + EGRESS_SPANNING_TREE_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + MBSC_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + DRAIN_PORT_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + MAC_RX_BROKEN_PACKETS_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + MAC_RX_SHORT_PACKET_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);
	if (ret)
		return ret;

	reg_addr = SWCORE_BASE + EGRESS_RESOURCE_MANAGEMENT_DROP_OFFSET + JL_PORT_P2M(port);
	ret = jl_apb_reg_burst_write(reg_addr, &val, 1);

	return ret;
}
