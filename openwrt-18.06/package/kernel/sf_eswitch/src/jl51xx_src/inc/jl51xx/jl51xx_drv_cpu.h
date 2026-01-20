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

#ifndef __JL51XX_DRV_CPU_H__
#define __JL51XX_DRV_CPU_H__

#include "jl_types.h"
#include "cpu.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl51xx_cpu_allow_bpdu_enable_set(jl_enable_t enable);
jl_ret_t jl51xx_cpu_allow_bpdu_enable_get(jl_enable_t *penable);

jl_ret_t jl51xx_cpu_allow_rst_bpdu_enable_set(jl_enable_t enable);
jl_ret_t jl51xx_cpu_allow_rst_bpdu_enable_get(jl_enable_t *penable);

jl_ret_t jl51xx_to_cpu_tag_enable_get(jl_enable_t *penable);
jl_ret_t jl51xx_to_cpu_tag_enable_set(jl_enable_t enable);

jl_ret_t jl51xx_cpu_allow_lldp_enable_set(jl_enable_t enable);
jl_ret_t jl51xx_cpu_allow_lldp_enable_get(jl_enable_t *penable);

#ifdef __cplusplus
}
#endif

#endif /* __JL51XX_DRV_CPU_H__ */
