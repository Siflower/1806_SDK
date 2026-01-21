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

#ifndef __JL51XX_DRV_FC_H__
#define __JL51XX_DRV_FC_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl51xx_taildrop_global_ffa_threshold_set(jl_enable_t enable, jl_uint32 cells);
jl_ret_t jl51xx_taildrop_global_ffa_threshold_get(jl_enable_t *penable, jl_uint32 *pcells);

jl_ret_t jl51xx_taildrop_port_ffa_threshold_set(jl_port_t phyport, jl_enable_t enable, jl_uint32 cells);
jl_ret_t jl51xx_taildrop_port_ffa_threshold_get(jl_port_t phyport, jl_enable_t *penable, jl_uint32 *pcells);

jl_ret_t jl51xx_taildrop_enable_set(jl_port_t phyport, jl_enable_t enable);
jl_ret_t jl51xx_taildrop_enable_get(jl_port_t phyport, jl_enable_t *penable);

jl_ret_t jl51xx_taildrop_enable_set(jl_port_t port, jl_enable_t enable);
jl_ret_t jl51xx_taildrop_enable_get(jl_port_t port, jl_enable_t *penable);

jl_ret_t jl51xx_ingress_resource_manager_drop_counter_get(jl_uint32 *pcntr);

jl_ret_t jl51xx_port_fc_enable_set(jl_port_t port, jl_enable_t enable);
jl_ret_t jl51xx_port_fc_enable_get(jl_port_t port, jl_enable_t *penable);

jl_ret_t jl51xx_port_fc_threshold_set(jl_port_t port,
			jl_uint32 threshold_on, jl_uint32 threshold_off);
jl_ret_t jl51xx_port_fc_threshold_get(jl_port_t port,
			jl_uint32 *pthreshold_on, jl_uint32 *pthreshold_off);

jl_ret_t jl51xx_port_fc_pause_time_set(jl_port_t port, jl_uint32 pause_time);
jl_ret_t jl51xx_port_fc_pause_time_get(jl_port_t port, jl_uint32 *ppause_time);

#ifdef __cplusplus
}
#endif

#endif /* __JL51XX_DRV_FC_H__ */
