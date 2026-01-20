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

#ifndef __JL51XX_DRV_STORM_H__
#define __JL51XX_DRV_STORM_H__

#include "jl_types.h"
#include "storm.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl51xx_storm_control_enable_set(jl_port_t egress_port, jl_storm_type_t type, jl_enable_t enable);
jl_ret_t jl51xx_storm_control_enable_get(jl_port_t egress_port, jl_storm_type_t type, jl_enable_t *penable);

jl_ret_t jl51xx_storm_control_rate_set(jl_port_t egress_port, jl_storm_type_t type, jl_rate_unit_t unit, jl_rate_t rate);
jl_ret_t jl51xx_storm_control_rate_get(jl_port_t egress_port, jl_storm_type_t type, jl_rate_unit_t *punit, jl_rate_t *prate);

jl_ret_t jl51xx_storm_control_drop_counter_get(jl_port_t egress_port, jl_uint32 *pcntr);
#ifdef __cplusplus
}
#endif

#endif /* __JL51XX_DRV_STORM_H__ */
