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

#include "jl51xx/reg_clkgen.h"
#include "jl51xx/reg_top.h"
#include "jl51xx/jl51xx_drv_phy.h"
#include "jl51xx/jl51xx_drv_sram_load.h"
#include "jl51xx/jl51xx_drv_sram_load_patch.h"

static jl_ret_t _enter_sram_mode(void)
{
	jl_uint32 try = 1000;
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 clkgen0_val = 0;
	jl_uint32 clkgen0_addr = CLKGEN_BASE + CLKGEN_CTL_0_OFFSET;
	jl_uint32 load_go_val = 0;

	ret = jl_apb_reg_read(clkgen0_addr, &clkgen0_val);
	if (ret)
		goto err;
	CLR_BIT(clkgen0_val, 1); //mcu_rstn
	ret = jl_apb_reg_write(clkgen0_addr, clkgen0_val);
	if (ret)
		goto err;

	ret = jl51xx_phy_direct_write_ext(0, LOAD_GO_OFFSET >> 5, LOAD_GO_OFFSET & 0x1f, 0);
	if (ret)
		goto err;
	port_udelay(5000); //5ms
	ret = jl51xx_phy_direct_write_ext(0, LOAD_DATA0_OFFSET >> 5, LOAD_DATA0_OFFSET & 0x1f, 0xe0c6);
	if (ret)
		goto err;
	port_udelay(5000); //5ms
	ret = jl_apb_reg_read(clkgen0_addr, &clkgen0_val);
	if (ret)
		goto err;
	SET_BIT(clkgen0_val, 1); //mcu_rstn
	ret = jl_apb_reg_write(clkgen0_addr, clkgen0_val);
	if (ret)
		goto err;

	ret = jl51xx_phy_direct_write_ext(0, LOAD_GO_OFFSET >> 5, LOAD_GO_OFFSET & 0x1f, 0x8000);
	if (ret)
		goto err;

	while (--try) {
		ret = jl51xx_phy_direct_read_ext(0, LOAD_GO_OFFSET >> 5, LOAD_GO_OFFSET & 0x1f, &load_go_val);
		if (ret)
			goto err;
		if (load_go_val == 0)
			break;
	}

	if (!try) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Timeout get load_go_val failed !!!\n");
		return JL_ERR_TIMEOUT;
	}
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Switch enter sram load mode ok[%d]!!!\n", ret);
	return ret;
err:
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Switch enter sram load mode failed[%d]!!!\n", ret);
	return ret;
}

static jl_ret_t _patch_load(jl_uint8 *patch_data, jl_uint32 patch_size)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 try = 100;
	jl_uint32 i = 0;
	jl_uint32 block = 0;
	jl_uint32 data[8] = {0};
	jl_uint32 load_go_val = 0;

	JL_CHECK_POINTER(patch_data);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Switch enter patch load!!!\n");
	for (block = 0; block < patch_size/BLOCK_SIZE; block++) {
		//address high
		data[0] = ((SRAM_START_ADDR + block*BLOCK_SIZE) & 0xffff0000) >> 16;
		//address low
		data[1] = (SRAM_START_ADDR + block*BLOCK_SIZE) & 0x0000ffff;
		//data
		for (i = 0; i < 6; i++) {
			data[i+2] = ((patch_data[block*BLOCK_SIZE+(2*i+1)]) << 8) + patch_data[block*BLOCK_SIZE+2*i];
		}
		for (i = 0; i < 8; i++) {
			ret = jl51xx_phy_direct_write_ext(0, LOAD_DATA0_OFFSET >> 5, (LOAD_DATA0_OFFSET & 0x1f) + i, data[i]);
			if (ret)
				goto err;
		}

		ret = jl51xx_phy_direct_write_ext(0, LOAD_GO_OFFSET >> 5, (LOAD_GO_OFFSET & 0x1f), 0x8006);
		if (ret)
			goto err;

		/* check if ready to receive image data */
		try = 100;
		while (--try) {
			ret = jl51xx_phy_direct_read_ext(0, LOAD_GO_OFFSET >> 5, LOAD_GO_OFFSET & 0x1f, &load_go_val);
			if (ret)
				goto err;
			if (load_go_val == 0)
				break;
		}

		if (!try) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Timeout check receive data failed !!!\n");
			return JL_ERR_TIMEOUT;
		}
	}
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Switch patch load ok[%d]!!!\n", ret);
	return ret;

err:
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "patch load reg read/write fail[%d]!!!\n", ret);

	return ret;
}

static jl_ret_t _patch_boot(void)
{
	jl_ret_t ret = JL_ERR_OK;
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Switch enter patch boot!!!\n");
	/*deliver execute patch instruction*/
	ret = jl51xx_phy_direct_write_ext(0, LOAD_DATA0_OFFSET >> 5, LOAD_DATA0_OFFSET & 0x1f, 0);
	if (ret)
		goto err;
	ret = jl51xx_phy_direct_write_ext(0, LOAD_DATA0_OFFSET >> 5, (LOAD_DATA0_OFFSET & 0x1f) + 1, SRAM_START_ADDR);
	if (ret)
		goto err;
	ret = jl51xx_phy_direct_write_ext(0, LOAD_GO_OFFSET >> 5, LOAD_GO_OFFSET & 0x1f, 0x4000);
	if (ret)
		goto err;
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Switch patch boot ok[%d]!!!\n", ret);
	return ret;

err:
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "patch boat reg read/write fail[%d]!!!\n", ret);

	return ret;
}

jl_ret_t jl51xx_drv_sram_load(void)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 patch_size = 0;
	jl_uint8 *patch_data = NULL;

	patch_size = JL_ARRAY_SIZE(sram_firmware_bin);
	patch_data = sram_firmware_bin;

	if (patch_size == 0) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "patch_size is zero!\n");
		return JL_ERR_OK;
	}
	if ((patch_size % BLOCK_SIZE) != 0) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Abort,patch size error!\n");
		return JL_ERR_PARAM;
	}
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "patch size[%d]bytes\n", patch_size);

	ret = _enter_sram_mode();
	if (ret)
		goto err;

	ret = _patch_load(patch_data, patch_size);
	if (ret)
		goto err;

	ret = _patch_boot();
	if (ret)
		goto err;

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Switch sram load patch finish.\n");

	return ret;

err:
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Switch sram load patch failed[%d]!!!\n", ret);

	return ret;
}
