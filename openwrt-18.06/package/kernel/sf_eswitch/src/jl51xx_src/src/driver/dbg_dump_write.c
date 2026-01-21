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

#include "driver/jl_reg_io.h"

#define JL_REG_DUMP_FILE		"register_write_dump.txt"
#define JL_CMD_WRITE			"WRITE"
#define JL_CMD_BWRITE			"BWRITE"
#define JL_CMD_DELAY			"DELAY"
#define JL_CMD_FLAG_DA			0U
#define JL_CMD_FLAG_IDA			1U
#define MAX_BURST_SIZE			8U

#define JL_DUMP_OVL 1
#define JL_DUMP_EE 0

static FILE *g_dump_fp;
static jl_uint8 g_dump_enable;

jl_api_ret_t jl_dump_file_open(void)
{
	g_dump_fp = fopen(JL_REG_DUMP_FILE, "w+");
	if (g_dump_fp == NULL)
		return JL_ERR_FAIL;

	return JL_ERR_OK;
}

void jl_dump_file_close(void)
{
	if (g_dump_fp != NULL) {
		fclose(g_dump_fp);
		g_dump_fp = NULL;
	}
}

#if JL_DUMP_OVL
jl_api_ret_t jl_dump_direct_write(jl_uint32 phy_id, jl_uint32 page_id, jl_uint32 reg_id, jl_uint16 val)
{
	jl_uint32 addr;

	if (g_dump_fp == NULL)
		return JL_ERR_EXISTS;
	if (!g_dump_enable)
		return JL_ERR_OK;

	addr = ((phy_id & 0x1FU) << 0x15U) | ((page_id & 0x1FU) << 0x5U) | reg_id;

	fprintf(g_dump_fp, "DEVICE(REGFILE_RF_BASE+0x%x) = 0x%x;\n", addr * 4, val);	// DEVICE(REGFILE_APB_BASE + (ADDR_FIXUP_DATA_LO << 2)) = 0xe06f;       fprintf(g_dump_fp, "REG32(0x%x) = 0x%x;\n",

	return JL_ERR_OK;
}

jl_api_ret_t jl_dump_indirect_write(jl_uint32 addr, jl_uint32 val)
{
	if (g_dump_fp == NULL)
		return JL_ERR_EXISTS;
	if (!g_dump_enable)
		return JL_ERR_OK;

	fprintf(g_dump_fp, "DEVICE(REGFILE_APB_BASE+0x%x) = 0x%x;\n", addr * 4, val);

	return JL_ERR_OK;
}

jl_api_ret_t jl_dump_indirect_burst_write(jl_uint32 addr, jl_uint32 size, jl_uint32 * pval)
{
	if (g_dump_fp == NULL)
		return JL_ERR_EXISTS;
	if ((size > MAX_BURST_SIZE) || (pval == NULL))
		return JL_ERR_PARAM;
	if (!g_dump_enable)
		return JL_ERR_OK;

	for (jl_uint8 i = 0; i < size; i++)
		fprintf(g_dump_fp, "DEVICE(REGFILE_APB_BASE+0x%x) = 0x%x;\n", addr * 4 + i * 4, pval[i]);
	return JL_ERR_OK;
}

jl_api_ret_t jl_dump_add_delay(jl_uint32 us)
{
	if (g_dump_fp == NULL)
		return JL_ERR_EXISTS;

	fprintf(g_dump_fp, "%s %d\n", JL_CMD_DELAY, us);

	return JL_ERR_OK;
}

jl_api_ret_t jl_dump_add_comment(char *comment)
{
	if (g_dump_fp == NULL)
		return JL_ERR_EXISTS;

	if (comment == NULL)
		return JL_ERR_PARAM;

	fprintf(g_dump_fp, "//%s\n", comment);

	return JL_ERR_OK;
}

#elif JL_DUMP_EE

jl_api_ret_t jl_dump_direct_write(jl_uint32 phy_id, jl_uint32 page_id, jl_uint32 reg_id, jl_uint16 val)
{
	jl_uint32 addr;

	if (g_dump_fp == NULL)
		return JL_ERR_EXISTS;
	if (!g_dump_enable)
		return JL_ERR_OK;

	addr = ((phy_id & 0x1FU) << 0x15U) | ((page_id & 0x1FU) << 0x5U) | reg_id;

	fprintf(g_dump_fp, "%s %d 0x%x 0x%x\n", JL_CMD_WRITE, JL_CMD_FLAG_DA, addr, val);

	return JL_ERR_OK;
}

jl_api_ret_t jl_dump_indirect_write(jl_uint32 addr, jl_uint32 val)
{
	if (g_dump_fp == NULL)
		return JL_ERR_EXISTS;
	if (!g_dump_enable)
		return JL_ERR_OK;

	fprintf(g_dump_fp, "%s %d 0x%x 0x%x\n", JL_CMD_WRITE, JL_CMD_FLAG_IDA, addr, val);

	return JL_ERR_OK;
}

jl_api_ret_t jl_dump_indirect_burst_write(jl_uint32 addr, jl_uint32 size, jl_uint32 * pval)
{
	if (g_dump_fp == NULL)
		return JL_ERR_EXISTS;
	if ((size > MAX_BURST_SIZE) || (pval == NULL))
		return JL_ERR_PARAM;
	if (!g_dump_enable)
		return JL_ERR_OK;

	fprintf(g_dump_fp, "%s %d 0x%x", JL_CMD_BWRITE, size, addr);
	for (jl_uint8 i = 0; i < size; i++)
		fprintf(g_dump_fp, " 0x%x", pval[i]);
	fprintf(g_dump_fp, "\n");

	return JL_ERR_OK;
}

jl_api_ret_t jl_dump_add_delay(jl_uint32 us)
{
	if (g_dump_fp == NULL)
		return JL_ERR_EXISTS;

	fprintf(g_dump_fp, "%s %d\n", JL_CMD_DELAY, us);

	return JL_ERR_OK;
}

jl_api_ret_t jl_dump_add_comment(char *comment)
{
	if (g_dump_fp == NULL)
		return JL_ERR_EXISTS;

	if (comment == NULL)
		return JL_ERR_PARAM;

	fprintf(g_dump_fp, ";%s\n", comment);

	return JL_ERR_OK;
}

#endif

void jl_dump_enable(void)
{
	g_dump_enable = 1;
}

void jl_dump_disable(void)
{
	g_dump_enable = 0;
}
