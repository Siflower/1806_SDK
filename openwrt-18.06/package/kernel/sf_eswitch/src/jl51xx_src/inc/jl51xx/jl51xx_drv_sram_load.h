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

#ifndef __JL51XX_DRV_SRAM_LOAD_H__
#define __JL51XX_DRV_SRAM_LOAD_H__

#include "jl_base.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SRAM_START_ADDR (0x800)
#define BLOCK_SIZE (12)

jl_ret_t jl51xx_drv_sram_load(void);

#ifdef __cplusplus
}
#endif

#endif /* __JL51XX_DRV_SRAM_LOAD_H__ */
