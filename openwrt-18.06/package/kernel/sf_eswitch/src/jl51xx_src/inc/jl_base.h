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

#ifndef __JL_BASE_H__
#define __JL_BASE_H__

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
  #error "Don't support big endian in current version"
#endif

/* Differentiate between built-in compile and out-of-tree compile for
 * Linux kernel, for normal build, always include jl autoconf */
#ifdef __KERNEL__
#ifdef MODULE
#include <linux/kconfig.h>
#else
#include "jl_autoconf.h"
#endif /* MODULE */
#else /* KERNEL */
#include "jl_autoconf.h"
#endif

#include "jl_config.h"
#include "jl_types.h"
#include "jl_debug.h"
#include "jl_error.h"
#include "portable/jl_portable.h"
#include "jl_switch.h"
#include "driver/jl_reg_io.h"

void jl_smi_write(u8 phy, u8 reg, u32 val);
int jl_smi_read(u8 phy, u8 reg);

#define JL_ARRAY_SIZE(x)		(sizeof(x)/sizeof((x)[0]))

#endif /* __JL_BASE_H__ */
