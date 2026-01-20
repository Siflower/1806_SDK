/**
 * @file lpd.h
 * @brief Loop Detection
 */

/**
 * @defgroup loop_detection_group Loop Detection
 * @{
 */

/**
 * @example example_lpd.c
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

#ifndef __JL_API_LPD_H__
#define __JL_API_LPD_H__

#include "jl_base.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @struct jl_lpd_config_s
 * @brief loop detection configuration.
 * @note "src_mac" should be distinct for each switch using JLSemi Loop Detection Function \n
 * "eth_type" "op_code" should be set to the same value for switches using JLSemi Loop Detection Function
 */
struct jl_lpd_config_s {
	jl_mac_t src_mac; /**< loop frame src mac address (a random address) */
	jl_uint16 eth_type; /**< ethernet type (a given value) */
	jl_uint16 op_code; /**< operation code (a given value)*/
	jl_uint16 ttl; /**< loop detection frame ttl (to avoid broadcast storm)*/
};

/**
 * @public typedef jl_lpd_config_t
 */
typedef struct jl_lpd_config_s jl_lpd_config_t;

/**
 * @brief Get "Loop Detection" configuration
 * @param[out] lpd_config Loop Detection Structure
 * @return loop detection configuration
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_lpd_config_get(jl_lpd_config_t *lpd_config);

/**
 * @brief Set "Loop Detection" configuration
 * @param[in] lpd_config Loop Detection Structure
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_lpd_config_set(jl_lpd_config_t *lpd_config);

/**
 * @brief Enable "Loop Detection"
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_lpd_enable(void);

/**
 * @brief Disable "Loop Detection"
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_lpd_disable(void);

/**
 * @brief Get "Loop Detection" ports in loop
 * @param[out] port_mask ports(in loop) mask
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_lpd_lp_ports_get(jl_portmask_t *port_mask);

/**
 * @brief Trigger one "Loop Detection" process
 * @details Send a loop detection frame automatically and start to check whether loop is detected. \n
 * If loop is detected the port(in loop) LEDS and LED(Buzzer) on loop indication pin will be drived to work.
 * @param[in] port to send the loop detection frame
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_lpd_trigger_once(jl_port_t port);

#ifdef __cplusplus
}
#endif

#endif /* __JL_API_LPD */
/** @} Loop Detection APIs*/
