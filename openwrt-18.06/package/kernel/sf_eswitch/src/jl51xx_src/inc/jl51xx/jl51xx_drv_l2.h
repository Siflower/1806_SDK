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

#ifndef __JL51XX_DRV_L2_H__
#define __JL51XX_DRV_L2_H__

#include "jl_types.h"
#include "jl51xx/reg_swcore.h"
#include "l2.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SWCORE_L2_REG_ADDR(ofs)								(SWCORE_BASE + ofs)
#define SWCORE_L2_REG_ENTRY_ADDR(ofs, idx, width)			(SWCORE_BASE + ofs + idx * width)

typedef jl_uint32 reg_addr_t;

JL_API jl_ret_t jl51xx_l2_aging_learning_get(jl_enable_t *penable_aging, jl_enable_t *penable_learning);
jl_ret_t jl51xx_l2_aging_learning_set(jl_enable_t enable_aging, jl_enable_t enable_learning);

jl_ret_t jl51xx_l2_mc_port_mask_get(jl_uint32 index, jl_uint32 *pregval);
jl_ret_t jl51xx_l2_mc_port_mask_set(jl_uint32 index, jl_uint32 regval);

jl_ret_t jl51xx_l2_mc_port_learning_get(jl_port_t port, jl_enable_t *pen_learning);
jl_ret_t jl51xx_l2_mc_port_learning_set(jl_port_t port, jl_enable_t en_learning);

jl_ret_t jl51xx_l2_fib_config_set_auto(jl_l2_fib_key_t *key, jl_l2_fib_config_t *cfg);

JL_API jl_ret_t jl51xx_l2_fib_config_get(jl_l2_fib_key_t *key, jl_l2_fib_config_t *cfg);
JL_API jl_ret_t jl51xx_l2_fib_config_get_by_index(jl_uint32 entry_index, jl_l2_fib_key_t *pkey, jl_l2_fib_config_t *cfg);

jl_ret_t jl51xx_l2_fib_hash_clear(void);
jl_ret_t jl51xx_l2_fib_col_clear(void);

jl_ret_t jl51xx_l2_aging_time_set(jl_uint32 time, jl_uint8 unit);
JL_API jl_ret_t jl51xx_l2_aging_time_get(jl_uint32 *ptime, jl_uint8 *punit);

jl_ret_t jl51xx_l2_tick_configuration_get(jl_uint16 *pdiv, jl_uint8 *pstep);

#ifdef __cplusplus
}
#endif

#endif /* __JL51XX_DRV_L2_H__ */
