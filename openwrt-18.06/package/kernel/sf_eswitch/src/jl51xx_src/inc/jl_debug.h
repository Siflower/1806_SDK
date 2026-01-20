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

#ifndef __JL_DEBUG_H__
#define __JL_DEBUG_H__

#ifndef __KERNEL__
#include <stdio.h>
#endif
#include "jl_types.h"
#include "jl_config.h"


#ifdef __KERNEL__
#include <linux/kernel.h>
#define JL_PRINT		printk
#else
#define JL_PRINT		printf
#endif

#ifdef CONFIG_JLFE_DEBUG
#define JL_DBG_MSG(flag, level, ...) \
		do { \
			if (((flag) & JL_DBG_FLAG_MASK) && (level <= CONFIG_JLFE_DBG_LEVEL)) { \
				JL_PRINT(__VA_ARGS__); \
			} \
		} while (0)
#else
#define JL_DBG_MSG(flag, level, ...) \
		do { \
			} while (0)
#endif

#endif /* __JL_DEBUG_H__ */
