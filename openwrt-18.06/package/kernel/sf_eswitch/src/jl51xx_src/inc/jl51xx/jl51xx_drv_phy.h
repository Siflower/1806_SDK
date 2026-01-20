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

#ifndef __JL51XX_DRV_PHY_H__
#define __JL51XX_DRV_PHY_H__

#include "jl_types.h"
#include "jl51xx/reg_phy.h"
#include "jl51xx/reg_fephy.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PHY_DATA_MASK				((jl_uint32)0x0000FFFFU)
#define PHY_REG_MASK				((jl_uint8)0x1FU)
#define PHY_ID_MASK					((jl_uint8)0x0FU)

jl_ret_t jl51xx_phy_indirect_read(jl_uint32 phy, jl_uint32 regaddr, jl_uint32 *pregval);

jl_ret_t jl51xx_phy_indirect_write(jl_uint32 phy, jl_uint32 regaddr, jl_uint32 regval);

jl_ret_t jl51xx_phy_direct_read_ext(jl_uint32 phy, jl_uint32 page, jl_uint32 reg, jl_uint32 *pregval);

jl_ret_t jl51xx_phy_direct_write_ext(jl_uint32 phy, jl_uint32 page, jl_uint32 reg, jl_uint32 regval);

#ifdef __cplusplus
}
#endif

#endif /* __JL51XX_DRV_PHY_H__ */
