/**
 * @file mirror.h
 * @brief Port mirror reference
 */

/**
 * @defgroup mirror_group Mirror
 * @{
 */

/**
 * @example example_mirror_port_in.c
 * @example example_mirror_port_out.c
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

#ifndef __JL_API_MIRROR_H__
#define __JL_API_MIRROR_H__

#include "jl_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get input mirror port
 * @param[in]	mirrored_port	mirrored port(source port)
 * @param[out]	mirroring_port	mirroring port(monitor port)
 * @return get mirroring port successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 *		@retval JL_ERR_UNAVAIL		unavailable Port id
 */
JL_API jl_api_ret_t jl_mirror_input_mr_get(jl_port_t mirrored_port,
		    			jl_port_t *mirroring_port);

/**
 * @brief Get output mirror port
 * @param[in]	mirrored_port	mirrored port(source port)
 * @param[out]	mirroring_port	mirroring port(monitor port)
 * @return get mirroring port successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 *		@retval JL_ERR_UNAVAIL		unavailable Port id
 */
JL_API jl_api_ret_t jl_mirror_output_mr_get(jl_port_t mirrored_port,
		    			jl_port_t *mirroring_port);

/**
 * @brief Set input mirror port
 * @param[in]	mirrored_port	mirrored port(source port)
 * @param[out]	mirroring_port	mirroring port(monitor port)
 * @return  set mirroring port successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_mirror_input_mr_set(jl_port_t mirrored_port,
		    			jl_port_t mirroring_port);


/**
 * @brief Set output mirror port
 * @param[in]	mirrored_port	mirrored port(source port)
 * @param[out]	mirroring_port	mirroring port(monitor port)
 * @return  set mirroring port successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_mirror_output_mr_set(jl_port_t mirrored_port,
						jl_port_t mirroring_port);

/**
 * @brief Clear input mirror port
 * @param[in]   mirrored_port   mirrored port(source port)
 * @return  clear mirrored_port port successfully or not
 *	       @retval JL_ERR_OK	       Ok
 *	       @retval JL_ERR_INIT	       Check Switch State Fail
 *	       @retval JL_ERR_PARAM	       PARAM Fail
 *	       @retval JL_ERR_TIMEOUT	       Read/Write Reg Timeout
 *	       @retval JL_ERR_FAIL	       Read/Write Reg Fail
 *	       @retval JL_ERR_PORT	       Error Port id
 */
JL_API jl_api_ret_t jl_mirror_input_mr_clear(jl_port_t mirrored_port);

/**
 * @brief Clear output mirror port
 * @param[in]   mirrored_port   mirrored port(source port)
 * @return  clear mirrored_port port successfully or not
 *	       @retval JL_ERR_OK	       Ok
 *	       @retval JL_ERR_INIT	       Check Switch State Fail
 *	       @retval JL_ERR_PARAM	       PARAM Fail
 *	       @retval JL_ERR_TIMEOUT	       Read/Write Reg Timeout
 *	       @retval JL_ERR_FAIL	       Read/Write Reg Fail
 *	       @retval JL_ERR_PORT	       Error Port id
 */
JL_API jl_api_ret_t jl_mirror_output_mr_clear(jl_port_t mirrored_port);

#ifdef __cplusplus
}
#endif

#endif /* __JL_API_MIRROR_H__ */
/** @} Mirror APIs*/
