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

#ifndef __JL51XX_DRV_MIB_H__
#define __JL51XX_DRV_MIB_H__

#include "jl_error.h"
#include "mib.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl51xx_mib_port_rc_mode_get(jl_port_t port, jl_enable_t *pregval);
jl_ret_t jl51xx_mib_port_rc_mode_set(jl_port_t port, jl_uint32 regval);
jl_ret_t jl51xx_mib_port_clear(jl_port_t port, jl_uint16 type);
jl_ret_t jl51xx_mib_port_get(jl_port_t port, jl_uint16 type, jl_uint64 *pregval);
jl_ret_t jl51xx_mib_swc_global_drop_get(jl_mib_swc_glb_drop_t *pport_cnt);
jl_ret_t jl51xx_mib_swc_global_drop_clear(void);
jl_ret_t jl51xx_mib_swc_port_drop_get(jl_port_t port, jl_mib_swc_port_drop_t *pport_cnt);
jl_ret_t jl51xx_mib_swc_port_drop_clear(jl_port_t port);

#ifdef __cplusplus
}
#endif

#endif /* __JL51XX_DRV_MIB_H__ */
