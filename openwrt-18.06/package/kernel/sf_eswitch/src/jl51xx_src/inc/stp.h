/**
 * @file stp.h
 * @brief Spanning Tree Protocol
 */

/**
 * @defgroup spanning_tree_protocol_group Spanning Tree Protocol
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

#ifndef __JL_API_STP_H__
#define __JL_API_STP_H__

#include "jl_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum jl_stp_state_e
 * @brief stp state.
 */
enum jl_stp_state_e {
	STP_STATE_DISABLED = 0,/**< 0 */
	STP_STATE_BLOCKING,/**< 1 */
	STP_STATE_LISTENING,/**< 2 */
	STP_STATE_LEARNING,/**< 3 */
	STP_STATE_FORWARDING,/**< 4 */
	STP_STATE_END,/**< Invalid */
};

/**
 * @public typedef jl_stp_state_t
 */
typedef enum jl_stp_state_e jl_stp_state_t;

/**
 * @brief Configure ingress spanning tree state per each port
 * @param[in]	port		port id
 * @param[in]	stp_state	ingress spanning tree state
 * @return set ingress stp state successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *		System supports per-port ingress spanning tree state.
 *		There are five states supported by ASIC.
 *		- STP_STATE_DISABLED
 *		- STP_STATE_BLOCKING
 *		- STP_STATE_LISTENING
 *		- STP_STATE_LEARNING    
 *		- STP_STATE_FORWARDING
 */
JL_API jl_api_ret_t jl_stp_port_ingress_state_set(jl_port_t port, jl_stp_state_t stp_state);

/**
 * @brief Get ingress spanning tree state per each port
 * @param[in]	port		port id
 * @param[out]	pstp_state	the returned ingress spanning tree state
 * @return get ingress stp state successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *		System supports per-port ingress spanning tree state.
 *		There are five states supported by ASIC.
 *		- STP_STATE_DISABLED
 *		- STP_STATE_BLOCKING
 *		- STP_STATE_LISTENING
 *		- STP_STATE_LEARNING    
 *		- STP_STATE_FORWARDING
 */
JL_API jl_api_ret_t jl_stp_port_ingress_state_get(jl_port_t port, jl_stp_state_t *pstp_state);

/**
 * @brief Configure egress spanning tree state per each port
 * @param[in]	port		port id
 * @param[in]	stp_state	egress spanning tree state
 * @return set egress stp state successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *		System supports per-port egress spanning tree state.
 *		There are five states supported by ASIC.
 *		- STP_STATE_DISABLED
 *		- STP_STATE_BLOCKING
 *		- STP_STATE_LISTENING
 *		- STP_STATE_LEARNING    
 *		- STP_STATE_FORWARDING
 */
JL_API jl_api_ret_t jl_stp_port_egress_state_set(jl_port_t port, jl_stp_state_t stp_state);

/**
 * @brief Get egress spanning tree state per each port
 * @param[in]	port		port id
 * @param[out]	pstp_state	egress spanning tree state
 * @return get egress stp state successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *		System supports per-port egress spanning tree state.
 *		There are five states supported by ASIC.
 *		- STP_STATE_DISABLED
 *		- STP_STATE_BLOCKING
 *		- STP_STATE_LISTENING
 *		- STP_STATE_LEARNING    
 *		- STP_STATE_FORWARDING
 */
JL_API jl_api_ret_t jl_stp_port_egress_state_get(jl_port_t port, jl_stp_state_t *pstp_state);

/**
 * @brief Get drop counter due to the specified ingress stp state
 * @param[in] stp_state ingress spanning tree state
 * @param[out] pcntr counter
 * @return get ingress stp drop counter successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 * @note
 *		Drop counter will be calculated per ingress spanning tree state not per port.
 *		Packets will be dropped when stp state is set as the following three states:
 *		- STP_STATE_BLOCKING
 *		- STP_STATE_LISTENING
 *		- STP_STATE_LEARNING
 *		While the ingress spanning tree state is set as the following two states,
 *		packets will be forwarded as normal.
 *		- STP_STATE_FORWARDING
 *		- STP_STATE_DISABLED
 */
JL_API jl_api_ret_t jl_stp_ingress_drop_counter_get(jl_stp_state_t stp_state, jl_uint32 *pcntr);

/**
 * @brief Get per-port drop counter due to egress spanning tree state.
 * @param[in] port port id
 * @param[out] pcntr drop counter
 * @return get egress stp drop counter successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_stp_port_egress_drop_counter_get(jl_port_t port, jl_uint32 *pcntr);

#ifdef __cplusplus
}
#endif

#endif /* __JL_API_STP_H__*/
/** @} Spanning Tree Protocol APIs*/
