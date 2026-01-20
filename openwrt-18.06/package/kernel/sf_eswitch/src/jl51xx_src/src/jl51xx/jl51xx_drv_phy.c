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
#include "jl51xx/jl51xx_drv_phy.h"
#include "jl51xx/reg_phy.h"

jl_ret_t jl51xx_phy_indirect_read(jl_uint32 phy, jl_uint32 regaddr, jl_uint32 *pregval)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 fephy_id = JL_PORT_P2VP(phy);
	jl_uint32 addr;

	addr = FEPHY0_BASE + fephy_id * FEPHY0_BLOCK_SIZE + regaddr;

	ret = jl_apb_reg_read(addr, pregval);
	if (ret)
		JL_DBG_MSG(JL_FLAG_PHY, _DBG_ERROR,
					"%s addr=0x%x regaddr=0x%x regval=0x%x fail: %s\n",
					__func__, addr, regaddr, *pregval, JL_ERRMSG(ret));
	return ret;
}

jl_ret_t jl51xx_phy_indirect_write(jl_uint32 phy, jl_uint32 regaddr, jl_uint32 regval)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 fephy_id = JL_PORT_P2VP(phy);
	jl_uint32 addr;

	addr = FEPHY0_BASE + fephy_id * FEPHY0_BLOCK_SIZE + regaddr;

	ret = jl_apb_reg_write(addr, regval);
	if (ret)
		JL_DBG_MSG(JL_FLAG_PHY, _DBG_ERROR,
					"%s addr=0x%x regaddr=0x%x regval=0x%x fail: %s\n",
					__func__, addr, regaddr, regval, JL_ERRMSG(ret));
	return ret;
}

jl_ret_t jl51xx_phy_direct_read_ext(jl_uint32 phy, jl_uint32 page, jl_uint32 reg, jl_uint32 *pregval)
{
	if ((page > PHY_REG_MASK) || (reg > PHY_REG_MASK))
		return JL_ERR_PARAM;

	*pregval = jl_phy_reg_read_ext(phy, page, reg);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_phy_direct_write_ext(jl_uint32 phy, jl_uint32 page, jl_uint32 reg, jl_uint32 regval)
{
	if ((page > PHY_REG_MASK) || (reg > PHY_REG_MASK))
		return JL_ERR_PARAM;

	jl_phy_reg_write_ext(phy, page, reg, (jl_uint16)(regval & PHY_DATA_MASK));

	return JL_ERR_OK;
}
