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

jl_ret_t jl51xx_mirror_input_mr_get(jl_port_t mirrored_port,
						jl_port_t *mirroring_port)
{
	jl_ret_t ret;
	jl_uint64 inputmirrorenabled, destinputmirror;
	jl_uint32 src_port_tab_val[2] = {0};
	jl_uint32 src_port_tab_addr = SOURCE_PORT_TABLE_OFFSET + (JL_PORT_P2M(mirrored_port) * 2) + SWCORE_BASE;
	jl_uint8 * preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(src_port_tab_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &inputmirrorenabled, 35, 1);
	jl_read_bits(preg_val, &destinputmirror, 36, 4);

	if (inputmirrorenabled) {
		*mirroring_port = JL_PORT_M2P(destinputmirror);

		return JL_ERR_OK;
	}

	*mirroring_port = UNDEF_PORT;

	return JL_ERR_UNAVAIL;
}

jl_ret_t jl51xx_mirror_output_mr_get(jl_port_t mirrored_port,
						jl_port_t *mirroring_port)
{
	jl_ret_t ret;
	jl_uint64 outputmirrorenabled, outputmirrorport;
	jl_uint32 output_mr_tab_val[1] = {0};
	jl_uint32 output_mr_tab_addr = OUTPUT_MIRRORING_TABLE_OFFSET + (JL_PORT_P2M(mirrored_port) * 1) + SWCORE_BASE;

	ret = jl_apb_reg_burst_read(output_mr_tab_addr, &output_mr_tab_val[0], 1);
	if (ret)
		return ret;

	jl_read_bits((jl_uint8 *)&output_mr_tab_val[0], &outputmirrorenabled, 0, 1);
	jl_read_bits((jl_uint8 *)&output_mr_tab_val[0], &outputmirrorport, 1, 4);

	if (outputmirrorenabled) {
		*mirroring_port = JL_PORT_M2P(outputmirrorport);

		return JL_ERR_OK;
	}

	*mirroring_port = UNDEF_PORT;

	return JL_ERR_UNAVAIL;
}

jl_ret_t jl51xx_mirror_input_mr_set(jl_port_t mirrored_port,
						jl_port_t mirroring_port)
{
	jl_ret_t ret;
	jl_uint64 inputmirrorenabled, destinputmirror;
	jl_uint32 src_port_tab_val[2] = {0};
	jl_uint32 src_port_tab_addr = SOURCE_PORT_TABLE_OFFSET + (JL_PORT_P2M(mirrored_port) * 2) + SWCORE_BASE;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(src_port_tab_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	destinputmirror = JL_PORT_P2M(mirroring_port);
	inputmirrorenabled = 1;
	jl_write_bits(preg_val, inputmirrorenabled, 35, 1);
	jl_write_bits(preg_val, destinputmirror, 36, 4);

	ret = jl_apb_reg_burst_write(src_port_tab_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_mirror_input_mr_clear(jl_port_t mirrored_port)
{
	jl_ret_t ret;
	jl_uint64 inputmirrorenabled;
	jl_uint32 src_port_tab_val[2] = {0};
	jl_uint32 src_port_tab_addr = SOURCE_PORT_TABLE_OFFSET + (JL_PORT_P2M(mirrored_port) * 2) + SWCORE_BASE;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(src_port_tab_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	inputmirrorenabled = 0;
	jl_write_bits(preg_val, inputmirrorenabled, 35, 1);

	ret = jl_apb_reg_burst_write(src_port_tab_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_mirror_output_mr_set(jl_port_t mirrored_port,
						jl_port_t mirroring_port)
{
	jl_ret_t ret;
	jl_uint64 outputmirrorenabled, outputmirrorport;
	jl_uint32 output_mr_tab_val[1] = {0};
	jl_uint32 output_mr_tab_addr = OUTPUT_MIRRORING_TABLE_OFFSET + (JL_PORT_P2M(mirrored_port) * 1) + SWCORE_BASE;
	jl_uint8 *preg_val = (jl_uint8 *)&output_mr_tab_val[0];

	ret = jl_apb_reg_burst_read(output_mr_tab_addr, &output_mr_tab_val[0], 1);
	if (ret)
		return ret;

	outputmirrorport = JL_PORT_P2M(mirroring_port);
	outputmirrorenabled = 1;
	jl_write_bits(preg_val, outputmirrorenabled, 0, 1);
	jl_write_bits(preg_val, outputmirrorport, 1, 4);

	ret = jl_apb_reg_burst_write(output_mr_tab_addr, &output_mr_tab_val[0], 1);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_mirror_output_mr_clear(jl_port_t mirrored_port)
{
	jl_ret_t ret;
	jl_uint64 outputmirrorenabled;
	jl_uint32 output_mr_tab_val[1] = {0};
	jl_uint32 output_mr_tab_addr = OUTPUT_MIRRORING_TABLE_OFFSET + (JL_PORT_P2M(mirrored_port) * 1) + SWCORE_BASE;
	jl_uint8 *preg_val = (jl_uint8 *)&output_mr_tab_val[0];

	ret = jl_apb_reg_burst_read(output_mr_tab_addr, &output_mr_tab_val[0], 1);
	if (ret)
		return ret;

	outputmirrorenabled = 0;
	jl_write_bits(preg_val, outputmirrorenabled, 0, 1);

	ret = jl_apb_reg_burst_write(output_mr_tab_addr, &output_mr_tab_val[0], 1);
	if (ret)
		return ret;

	return JL_ERR_OK;
}
