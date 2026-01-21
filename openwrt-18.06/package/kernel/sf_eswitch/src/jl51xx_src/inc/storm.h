/**
 * @file storm.h
 * @brief Multicast Broadcast Storm Control reference
 */

/**
 * @defgroup storm_group Storm
 * @{
 */

/**
 * @example example_storm_rate_ctrl.c
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

#ifndef __JL_API_STORM_H__
#define __JL_API_STORM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "jl_types.h"

/**
 * @public typedef jl_rate_t
 */
typedef jl_uint32	jl_rate_t;

/**
 * @enum jl_storm_type_e
 * @brief storm type.
 */
enum jl_storm_type_e {
	STORM_TYPE_BROADCAST = 0,/**< 0 */
	STORM_TYPE_MULTICAST,/**< 1 */
	STORM_TYPE_UNKNOWN,/**< 2 */
	STORM_TYPE_END,/**< Invalid */
};

/**
 * @public typedef jl_storm_type_t
 */
typedef enum jl_storm_type_e jl_storm_type_t;

/**
 * @enum jl_rate_unit_e
 * @brief rate unit.
 */
enum jl_rate_unit_e {
	RATE_UNIT_KBPS = 0,/**< 0 */
	RATE_UNIT_PPS,/**< 1 */
	RATE_UNIT_END,/**< Invalid */
};

/**
 * @public typedef jl_rate_unit_t
 */
typedef enum jl_rate_unit_e jl_rate_unit_t;

/**
 * @brief Set storm control enable configuration
 * @details This function can be used to enable storm control per egress port per traffic type.
 * @param[in]	egress_port	the specified egress port
 * @param[in]	type		the traffic type to be set
 * @param[in]	enable		the enable status to be set
 * @return set storm control enable successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_storm_control_enable_set(jl_port_t egress_port, jl_storm_type_t type, jl_enable_t enable);

/**
 * @brief Get storm control enable configuration
 * @details This function can be used to get the specified traffic type enable status of the specified egress port
 * @param[in]	egress_port	the specified egress port
 * @param[in]	type		the traffic type
 * @param[out]	penable		the returned enable status
 * @return get storm control enable successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_storm_control_enable_get(jl_port_t egress_port, jl_storm_type_t type, jl_enable_t *penable);

/**
 * @brief Set storm control rate configuration
 * @details This function can set storm control rate for each egress port each traffic type.
 * The rate unit is 1 kbps and the range is from 8k to 1048568k if type is RATE_UNIT_KBPS
 * and the granularity of rate is 8 kbps.
 * The rate unit is packets per second and the range is 1 ~ 0x1FFF if type is RATE_UNIT_PPS.\n
 * The IFG is included by default.
 * @param[in]	egress_port	the specified egress port
 * @param[in]	type		the traffic type to be set
 * @param[in]	unit		the rate unit to be set
 * @param[out]	rate		storm control rate
 * @return set storm control enable successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_storm_control_rate_set(jl_port_t egress_port, jl_storm_type_t type, jl_rate_unit_t unit, jl_rate_t rate);

/**
 * @brief Get storm control rate configuration
 * @details This function can get storm control rate for each egress port each traffic type.
 * The rate unit is 1 kbps and the range is from 8k to 1048568k if type is RATE_UNIT_KBPS
 * and the granularity of rate is 8 kbps.
 * The rate unit is packets per second and the range is 1 ~ 0x1FFF if type is RATE_UNIT_PPS.\n
 * The IFG is included by default.
 * @param[in]	egress_port	the specified egress port
 * @param[in]	type		the traffic type to be set
 * @param[out]	punit		the returned rate unit
 * @param[out]	prate		the returned storm control rate
 * @return get storm control enable successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_storm_control_rate_get(jl_port_t egress_port, jl_storm_type_t type, jl_rate_unit_t *punit, jl_rate_t *prate);

/**
 * @brief Get storm control drop counter
 * @details This function can get storm control drop counter of the specified port
 * @param[in]	egress_port	the specified egress port
 * @param[out]	pcntr		the returned drop counter
 * @return get storm control drop counter successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_storm_control_drop_counter_get(jl_port_t egress_port, jl_uint32 *pcntr);

#ifdef __cplusplus
}
#endif

#endif /* __JL_API_STORM_H__ */
/** @} Storm APIs*/
