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

#ifndef __JL51XX_DRV_STP_H__
#define __JL51XX_DRV_STP_H__

#include "jl_types.h"
#include "stp.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl51xx_stp_port_ingress_state_set(jl_port_t port, jl_stp_state_t stp_state);
jl_ret_t jl51xx_stp_port_ingress_state_get(jl_port_t port, jl_stp_state_t *pstp_state);

jl_ret_t jl51xx_stp_port_egress_state_set(jl_port_t port, jl_stp_state_t stp_state);
jl_ret_t jl51xx_stp_port_egress_state_get(jl_port_t port, jl_stp_state_t *pstp_state);

jl_ret_t jl51xx_stp_ingress_drop_counter_get(jl_stp_state_t stp_state, jl_uint32 *pcntr);
jl_ret_t jl51xx_stp_port_egress_drop_counter_get(jl_port_t port, jl_uint32 *pcntr);
#ifdef __cplusplus
}
#endif

#endif /* __JL51XX_DRV_STP_H__ */
