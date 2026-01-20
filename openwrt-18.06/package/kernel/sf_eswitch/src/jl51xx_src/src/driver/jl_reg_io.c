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
#include "jl_config.h"
#include "driver/jl_reg_io.h"

#define APB_REG_BUSRT_SIZE_PG0_MAX	6
#define APB_REG_BUSRT_SIZE_MAX	8

#define REG_NO_OPR_CTRL_0	16
#define REG_NO_OPR_CTRL_1	17
#define REG_NO_OPR_CTRL_2	18
#define REG_NO_OPR_DATA0_L	19
#define REG_NO_OPR_DATA6_L	16


jl_ret_t __apb_reg_read(jl_uint32 reg, jl_uint32 *buf, jl_uint8 size)
{
	/* access apb registers indirectly via SMI on domain "phy 0 & page 0" */
	jl_uint32 phy_0 = 0;
	/* page selection register */
	jl_uint32 phy_reg = 31;
	jl_uint32 page_0 = 0;
	jl_uint32 page_1 = 1;
	jl_uint32 reg_l_14b = 0;
	jl_uint32 reg_h_16b = 0;
	jl_uint16 try = 1000;
	jl_uint32 tmp = 0;
	jl_uint8 pg0_burst = 0;
	jl_uint8 pg1_burst = 0;
	jl_uint16 wide_en = 0;
	jl_uint16 burst_ctrl = 0;
	jl_int32 i, j;
	jl_api_ret_t ret = 0;

	if (size > APB_REG_BUSRT_SIZE_MAX) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Brust size overflow, max burst size is 8\n");
		ret = JL_ERR_PARAM;
		return ret;
	}

#ifdef CONFIG_JLFE_APB_MUTEX
	port_mutex_lock(g_smi_dev.apb_mutex);
#endif

	/* set page 0 */
	jl_smi_write(phy_0, phy_reg, page_0);

	/* 1. set burst size to 0(1 word)
	 * '0' is same as '1', but would not enable the wide range
	 */
	wide_en = ((size > 1) ? 1: 0);
	burst_ctrl = (size << 2) | (wide_en << 1);
	jl_smi_write(phy_0, REG_NO_OPR_CTRL_2, burst_ctrl);

	/* 2. set register address && issue read operation */
	reg_l_14b = ((reg << 2) & 0x0000fffc) | 1;
	reg_h_16b = (reg >> 14) & 0x000003fff;
	jl_smi_write(phy_0, REG_NO_OPR_CTRL_1, reg_h_16b);
	jl_smi_write(phy_0, REG_NO_OPR_CTRL_0, reg_l_14b);

	/* 4. check operation done & status */
	while (--try) {
		tmp = jl_smi_read(phy_0, REG_NO_OPR_CTRL_0);
		if (!(tmp & 1))
			break;
	}

	if (!try) {
#ifdef CONFIG_JLFE_APB_MUTEX
		port_mutex_unlock(g_smi_dev.apb_mutex);
#endif
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "read apb register timeout\n");
		ret = JL_ERR_TIMEOUT;
		return ret;
	}

	tmp = jl_smi_read(phy_0, REG_NO_OPR_CTRL_2);
	if ((tmp & 1)) {
#ifdef CONFIG_JLFE_APB_MUTEX
		port_mutex_unlock(g_smi_dev.apb_mutex);
#endif
		ret = JL_ERR_FAIL;
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "read apb register fail\n");
		return ret;
	}

	/* 5. read data */
	pg0_burst = size;
	if (size > APB_REG_BUSRT_SIZE_PG0_MAX) {
		pg0_burst = APB_REG_BUSRT_SIZE_PG0_MAX;
		pg1_burst = size - APB_REG_BUSRT_SIZE_PG0_MAX;
	}
	for (i = 0; i < pg0_burst; i++) {
		buf[i] = jl_smi_read(phy_0, REG_NO_OPR_DATA0_L + (i * 2));
		tmp = jl_smi_read(phy_0, REG_NO_OPR_DATA0_L + (i * 2) + 1);
		buf[i] |= tmp << 16;
	}
	if (size > APB_REG_BUSRT_SIZE_PG0_MAX) {
		/* set page 1 */
		jl_smi_write(phy_0, phy_reg, page_1);
		for (j = 0; j < pg1_burst; j++) {
			buf[i + j] = jl_smi_read(phy_0, REG_NO_OPR_DATA6_L + (j * 2));
			tmp = jl_smi_read(phy_0, REG_NO_OPR_DATA6_L + (j * 2) + 1);
			buf[i + j] |= tmp << 16;
		}
	}

#ifdef CONFIG_JLFE_APB_MUTEX
	port_mutex_unlock(g_smi_dev.apb_mutex);
#endif

	return ret;
}

jl_ret_t __apb_reg_write(jl_uint32 reg, jl_uint32 *buf, jl_uint8 size)
{
	/* access apb registers indirectly via SMI on domain "phy 0 & page 0" */
	jl_uint32 phy_0 = 0;
	/* page selection register */
	jl_uint32 phy_reg = 31;
	jl_uint32 page_0 = 0;
	jl_uint32 page_1 = 1;
	jl_uint32 reg_l_14b = 0;
	jl_uint32 reg_h_16b = 0;
	jl_uint16 try = 1000;
	jl_uint16 tmp = 0;
	jl_uint8 pg0_burst = 0;
	jl_uint8 pg1_burst = 0;
	jl_uint16 wide_en = 0;
	jl_uint16 burst_ctrl = 0;
	jl_int32 i, j;
	jl_ret_t ret = 0;

	if (size > APB_REG_BUSRT_SIZE_MAX) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Brust size overflow, max burst size is 8\n");
		ret = JL_ERR_PARAM;
		return ret;
	}

#ifdef CONFIG_JLFE_DUMP_REG_WRITE
	if (size == 1) {
		ret = jl_dump_indirect_write(reg, *buf);
		if (ret)
			JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Dump smi write fail!!!\n");
	} else {
		ret = jl_dump_indirect_burst_write(reg, size, buf);
		if (ret)
			JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Dump smi write fail!!!\n");
	}
	ret = 0;
#endif

#ifdef CONFIG_JLFE_APB_MUTEX
	port_mutex_lock(g_smi_dev.apb_mutex);
#endif

	/* set page 0 */
	jl_smi_write(phy_0, phy_reg, page_0);

	/* 1. write data */
	pg0_burst = size;
	if (size > APB_REG_BUSRT_SIZE_PG0_MAX) {
		pg0_burst = APB_REG_BUSRT_SIZE_PG0_MAX;
		pg1_burst = size - APB_REG_BUSRT_SIZE_PG0_MAX;
	}
	for (i = 0; i < pg0_burst; i++) {
		jl_smi_write(phy_0, REG_NO_OPR_DATA0_L + (i * 2), (buf[i] & 0x0000ffff));
		jl_smi_write(phy_0, REG_NO_OPR_DATA0_L + (i * 2 + 1), ((buf[i] >> 16) & 0x0000ffff));
	}
	if (size > APB_REG_BUSRT_SIZE_PG0_MAX) {
		/* set page 1 */
		jl_smi_write(phy_0, phy_reg, page_1);
		for (j = 0; j < pg1_burst; j++) {
			jl_smi_write(phy_0, REG_NO_OPR_DATA6_L + (j * 2), (buf[i + j] & 0x0000ffff));
			jl_smi_write(phy_0, REG_NO_OPR_DATA6_L + (j * 2 + 1), ((buf[i + j] >> 16) & 0x0000ffff));
		}
		/* set page 0 */
		jl_smi_write(phy_0, phy_reg, page_0);
	}

	/* 2. set burst size to 0(1 word)
	 * '0' is same as '1', but would not enable the wide range
	 */
	wide_en = ((size > 1) ? 1: 0);
	burst_ctrl = (size << 2) | (wide_en << 1);
	jl_smi_write(phy_0, REG_NO_OPR_CTRL_2, burst_ctrl);

	/* 3. set register address && issue read operation */
	reg_l_14b = ((reg << 2) & 0x0000fffc) | 3;
	reg_h_16b = (reg >> 14) & 0x000003fff;
	jl_smi_write(phy_0, REG_NO_OPR_CTRL_1, reg_h_16b);
	jl_smi_write(phy_0, REG_NO_OPR_CTRL_0, reg_l_14b);

	/* 4. check operation done & status */
	while (--try) {
		tmp = jl_smi_read(phy_0, REG_NO_OPR_CTRL_0);
		if (!(tmp & 1))
			break;
	}

	if (!try) {
#ifdef CONFIG_JLFE_APB_MUTEX
		port_mutex_unlock(g_smi_dev.apb_mutex);
#endif
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "write apb register timeout\n");
		ret = JL_ERR_TIMEOUT;
		return ret;
	}
	tmp = jl_smi_read(phy_0, REG_NO_OPR_CTRL_2);
	if ((tmp & 1)) {
#ifdef CONFIG_JLFE_APB_MUTEX
		port_mutex_unlock(g_smi_dev.apb_mutex);
#endif
		ret = JL_ERR_FAIL;
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "write apb register fail\n");
		return ret;
	}

#ifdef CONFIG_JLFE_APB_MUTEX
	port_mutex_unlock(g_smi_dev.apb_mutex);
#endif

	return ret;
}

void jl_read_bits(jl_uint8 *bit_buf, jl_uint64 *bits, jl_uint8 start, jl_uint8 count)
{
	jl_uint64 i_bits = 0;
	jl_uint8 i_count = 0;
	jl_uint32 i_index = (start) / 8;
	jl_uint8 i_mask = 1 << ((start) % 8);

	while (i_count < count) {
		if (bit_buf[i_index] & i_mask)
			i_bits |= (1ULL << i_count);
		if (i_mask == 0x80) {
			i_index++;
			i_mask = 0x01;
		} else
			i_mask <<= 1;
		i_count++;
	}
	*bits = i_bits;
};

void jl_write_bits(jl_uint8 *bit_buf, jl_uint64 bits, jl_uint8 start, jl_uint8 count)
{
	jl_uint64 i_bits = bits;
	jl_uint8 i_count = 0;
	jl_uint32 i_index = (start) / 8;
	jl_uint8 i_mask = 1 << ((start) % 8);

	while (i_count < count) {
		if ((i_bits >> i_count) & 0x01)
			bit_buf[i_index] |= i_mask;
		else
			bit_buf[i_index] &= ~i_mask;
		if (i_mask == 0x80) {
			i_index++;
			i_mask = 0x01;
		} else
			i_mask <<= 1;
		i_count++;
	}
};

jl_uint16 jl_phy_reg_read_ext(jl_uint32 phy, jl_uint32 page, jl_uint32 reg)
{
	jl_smi_write(phy, 0x1f, page);

	return jl_smi_read(phy, reg);
}

void jl_phy_reg_write_ext(jl_uint32 phy, jl_uint32 page, jl_uint32 reg, jl_uint16 val)
{
	jl_smi_write(phy, 0x1f, page);
	jl_smi_write(phy, reg, val);

#ifdef CONFIG_JLFE_DUMP_REG_WRITE
	jl_ret_t ret;

	ret = jl_dump_direct_write(phy, page, reg, val);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Dump smi write fail!!!\n");
#endif
}

jl_api_ret_t jl_apb_reg_read(jl_uint32 reg, jl_uint32 *val)
{
	return __apb_reg_read(reg, val, 1);
}

jl_api_ret_t jl_apb_reg_burst_read(jl_uint32 reg, jl_uint32 *buf, jl_uint32 size)
{
	return __apb_reg_read(reg, buf, size);
}

jl_api_ret_t jl_apb_reg_write(jl_uint32 reg, jl_uint32 val)
{
	return __apb_reg_write(reg, &val, 1);
}

jl_api_ret_t jl_apb_reg_burst_write(jl_uint32 reg, jl_uint32 *buf, jl_uint32 size)
{
	return __apb_reg_write(reg, buf, size);
}

// jl_api_ret_t jl_reg_io_init(void)
// {
// #ifdef CONFIG_JLFE_DUMP_REG_WRITE
// 	jl_dump_file_open();
// #endif

// 	return jl_smi_init();
// }

// jl_api_ret_t jl_reg_io_deinit(void)
// {
// #ifdef CONFIG_JLFE_DUMP_REG_WRITE
// 	jl_dump_file_close();
// #endif

// 	return jl_smi_deinit();
// }
