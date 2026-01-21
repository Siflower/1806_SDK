/**
 * @file sram_load.h
 * @brief Sram Load
 */

/**
 * @defgroup sram_load_group Sram Load
 * @{
 */

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

#ifndef __JL_API_SRAM_LOAD_H__
#define __JL_API_SRAM_LOAD_H__

#include "jl_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Perform a transfer to sram
 * @details This api is a tool, it can copy datas of bin to the sram for the firmware to use
 * @return set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 */
JL_API jl_api_ret_t jl_sram_load(void);

#ifdef __cplusplus
}
#endif
#endif /* __JL_API_SRAM_LOAD_H__*/
/** @} SRAM_LOAD APIs*/

