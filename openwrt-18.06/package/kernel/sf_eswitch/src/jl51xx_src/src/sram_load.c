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

#include "jl51xx/jl51xx_drv_sram_load.h"
#include "sram_load.h"


static jl_ret_t _jl_sram_load(void)
{
	return jl51xx_drv_sram_load();
}

jl_api_ret_t jl_sram_load(void)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();

	JL_API_MUTEX_LOCK();
	ret = _jl_sram_load();
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_sram_load);
