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

#include "jl_base.h"
#include "jl51xx/jl51xx_drv_mirror.h"
#include "mirror.h"

jl_api_ret_t _jl_mirror_input_mr_get(jl_port_t mirrored_port, jl_port_t *mirroring_port)
{
	return jl51xx_mirror_input_mr_get(mirrored_port, mirroring_port);
}

jl_api_ret_t _jl_mirror_output_mr_get(jl_port_t mirrored_port, jl_port_t *mirroring_port)
{
	return jl51xx_mirror_output_mr_get(mirrored_port, mirroring_port);
}

jl_api_ret_t _jl_mirror_input_mr_set(jl_port_t mirrored_port, jl_port_t mirroring_port)
{
	return jl51xx_mirror_input_mr_set(mirrored_port, mirroring_port);
}

jl_api_ret_t _jl_mirror_input_mr_clear(jl_port_t mirrored_port)
{
	return jl51xx_mirror_input_mr_clear(mirrored_port);
}

jl_api_ret_t _jl_mirror_output_mr_set(jl_port_t mirrored_port, jl_port_t mirroring_port)
{
	return jl51xx_mirror_output_mr_set(mirrored_port, mirroring_port);
}

jl_api_ret_t _jl_mirror_output_mr_clear(jl_port_t mirrored_port)
{
	return jl51xx_mirror_output_mr_clear(mirrored_port);
}

jl_api_ret_t jl_mirror_input_mr_get(jl_port_t mirrored_port, jl_port_t *mirroring_port)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(mirrored_port);
	JL_CHECK_POINTER(mirroring_port);

	JL_API_MUTEX_LOCK();
	ret = _jl_mirror_input_mr_get(mirrored_port, mirroring_port);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_mirror_input_mr_get);

jl_api_ret_t jl_mirror_output_mr_get(jl_port_t mirrored_port, jl_port_t *mirroring_port)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(mirrored_port);
	JL_CHECK_POINTER(mirroring_port);

	JL_API_MUTEX_LOCK();
	ret = _jl_mirror_output_mr_get(mirrored_port, mirroring_port);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_mirror_output_mr_get);

jl_api_ret_t jl_mirror_input_mr_set(jl_port_t mirrored_port, jl_port_t mirroring_port)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(mirrored_port);
	JL_CHECK_PORT(mirroring_port);

	JL_API_MUTEX_LOCK();
	ret = _jl_mirror_input_mr_set(mirrored_port, mirroring_port);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_mirror_input_mr_set);

jl_api_ret_t jl_mirror_input_mr_clear(jl_port_t mirrored_port)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(mirrored_port);

	JL_API_MUTEX_LOCK();
	ret = _jl_mirror_input_mr_clear(mirrored_port);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_mirror_input_mr_clear);

jl_api_ret_t jl_mirror_output_mr_set(jl_port_t mirrored_port, jl_port_t mirroring_port)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(mirrored_port);
	JL_CHECK_PORT(mirroring_port);

	JL_API_MUTEX_LOCK();
	ret = _jl_mirror_output_mr_set(mirrored_port, mirroring_port);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_mirror_output_mr_set);

jl_api_ret_t jl_mirror_output_mr_clear(jl_port_t mirrored_port)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_PORT(mirrored_port);

	JL_API_MUTEX_LOCK();
	ret = _jl_mirror_output_mr_clear(mirrored_port);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_mirror_output_mr_clear);
