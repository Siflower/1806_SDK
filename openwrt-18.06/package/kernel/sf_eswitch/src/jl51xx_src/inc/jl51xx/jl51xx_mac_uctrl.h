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

#ifndef __JL51XX_MAC_UCTRL_H__
#define __JL51XX_MAC_UCTRL_H__

#include "jl_types.h"
#include "driver/jl_reg_io.h"
#include "jl51xx/reg_top.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAC_UCTRL_DATA_MASK						((jl_uint16)(0xFFFFU))
#define MAC_UCTRL_PAGE_ID						(MAC0_USER_CTRL_OFFSET >> 5)
#define MAC_UCTRL_REG_ID(id)					((MAC0_USER_CTRL_OFFSET & 0x1F) + id)
#define MAC_UCTRL_LINK_UP						BIT(0)
#define MAC_UCTRL_FULL_DUPLEX					BIT(1)
#define MAC_UCTRL_100M_SPEED					BIT(2)

#define SW_RSVD_DATA_MASK						((jl_uint16)(0xFFFFU))
#define SW_RSVD_PAGE_ID							(SW_RSVD1_OFFSET >> 5)
#define SW_RSVD_REG_ID(id)						((SW_RSVD1_OFFSET & 0x1F) + id)
jl_ret_t jl51xx_mac_uctrl_read(jl_port_t phyid, jl_uint32 *pregval);

jl_ret_t jl51xx_mac_uctrl_write(jl_port_t phyid, jl_uint32 regval);

jl_ret_t jl51xx_sw_rsvd_read(jl_port_t phyid, jl_uint32 *pregval);

jl_ret_t jl51xx_sw_rsvd_write(jl_port_t phyid, jl_uint32 regval);

#ifdef __cplusplus
}
#endif

#endif /* __JL51XX_MAC_UCTRL_H__ */
