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

#ifndef __JL51XX_DRV_LPD_H__
#define __JL51XX_DRV_LPD_H__

#include "jl_error.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl51xx_lpd_ctrl_get(void *lpd_config);
jl_ret_t jl51xx_lpd_ctrl_set(void *lpd_config);
jl_ret_t jl51xx_lpd_disable(void);
jl_ret_t jl51xx_lpd_enable(void);
jl_ret_t jl51xx_lpd_lp_stat_clear(void);
jl_ret_t jl51xx_lpd_ports_get(jl_portmask_t *port_mask);
jl_ret_t jl51xx_lpd_trigger_check(void);
jl_ret_t jl51xx_lpd_trigger_once(jl_port_t port);

#ifdef __cplusplus
}
#endif

#endif /* __JL51XX_DRV_LPD_H__ */
