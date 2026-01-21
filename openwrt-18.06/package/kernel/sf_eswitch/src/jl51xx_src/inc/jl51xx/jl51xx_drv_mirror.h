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

#ifndef __JL51XX_DRV_MIRROR_H__
#define __JL51XX_DRV_MIRROR_H__

#include "jl_error.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl51xx_mirror_input_mr_get(jl_port_t mirrored_port,
					jl_port_t *mirroring_port);
jl_ret_t jl51xx_mirror_output_mr_get(jl_port_t mirrored_port,
	      				jl_port_t *mirroring_port);
jl_ret_t jl51xx_mirror_input_mr_set(jl_port_t mirrored_port,
	      				jl_port_t mirroring_port);
jl_ret_t jl51xx_mirror_output_mr_set(jl_port_t mirrored_port,
					jl_port_t mirroring_port);
jl_ret_t jl51xx_mirror_input_mr_clear(jl_port_t mirrored_port);
jl_ret_t jl51xx_mirror_output_mr_clear(jl_port_t mirrored_port);
#ifdef __cplusplus
}
#endif

#endif /* __JL51XX_DRV_MIRROR_H__ */
