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

#include "jl_types.h"
#include "jl_switch.h"
#include "jl51xx/jl51xx_mac_uctrl.h"
#include "jl51xx/jl51xx_drv_phy.h"

jl_ret_t jl51xx_mac_uctrl_read(jl_port_t port, jl_uint32 *pregval)
{
	jl_ret_t ret = JL_ERR_OK;

	if (pregval == NULL)
		return JL_ERR_PARAM;

	if ((ret = jl51xx_phy_direct_read_ext(0, MAC_UCTRL_PAGE_ID,
					MAC_UCTRL_REG_ID(JL_PORT_P2M(port)),
					pregval)) != JL_ERR_OK)
		return ret;

	JL_DBG_MSG(JL_FLAG_MAC_UCTRL, _DBG_ON,
				"read port=0x%x, regval=0x%x errmsg: %s\n",
				port, *pregval, JL_ERRMSG(ret));
	return ret;
}

jl_ret_t jl51xx_mac_uctrl_write(jl_port_t port, jl_uint32 regval)
{
	jl_ret_t ret;

	if (port >= JL_PORT_MAX) {
		ret = JL_ERR_PARAM;

		return ret;
	}

	if ((ret = jl51xx_phy_direct_write_ext(0, MAC_UCTRL_PAGE_ID,
					MAC_UCTRL_REG_ID(JL_PORT_P2M(port)),
					regval)) != JL_ERR_OK)
		return ret;

	JL_DBG_MSG(JL_FLAG_MAC_UCTRL, _DBG_ON, "write port=0x%x, regval=0x%x errmsg:%s\n",
			port, regval, JL_ERRMSG(ret));
	return JL_ERR_OK;
}

jl_ret_t jl51xx_sw_rsvd_read(jl_port_t port, jl_uint32 *pregval)
{
	jl_ret_t ret = JL_ERR_OK;

	if (pregval == NULL)
		return JL_ERR_PARAM;

	if ((ret = jl51xx_phy_direct_read_ext(0, SW_RSVD_PAGE_ID,
					SW_RSVD_REG_ID(JL_PORT_P2M(port)),
					pregval)) != JL_ERR_OK)
		return ret;

	JL_DBG_MSG(JL_FLAG_MAC_UCTRL, _DBG_ON,
				"read port=0x%x, regval=0x%x errmsg: %s\n",
				port, *pregval, JL_ERRMSG(ret));
	return ret;
}

jl_ret_t jl51xx_sw_rsvd_write(jl_port_t port, jl_uint32 regval)
{
	jl_ret_t ret;

	if (port >= JL_PORT_MAX) {
		ret = JL_ERR_PARAM;

		return ret;
	}

	if ((ret = jl51xx_phy_direct_write_ext(0, SW_RSVD_PAGE_ID,
					SW_RSVD_REG_ID(JL_PORT_P2M(port)),
					regval)) != JL_ERR_OK)
		return ret;

	JL_DBG_MSG(JL_FLAG_MAC_UCTRL, _DBG_ON,
				"write port=0x%x, regval=0x%x errmsg:%s\n",
				port, regval, JL_ERRMSG(ret));
	return JL_ERR_OK;
}
