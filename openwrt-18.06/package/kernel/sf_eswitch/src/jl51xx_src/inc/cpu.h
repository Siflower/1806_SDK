/**
 * @file cpu.h
 * @brief cpu
 */

/**
 * @defgroup cpu_group CPU
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

#ifndef __JL_API_CPU_H__
#define __JL_API_CPU_H__

#include "jl_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Set enable allow bpdu packets send to CPU port or not
 * @param enable: enable state
 * @return set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		This API can be used to set allow bpdu packets send to CPU port or not.
 *		If set then packets that have the destination MAC address equal to
 *		01:80:C2:00:00:00 are sent to the CPU port. Otherwise, the packets
 *		that have thedestination MAC address equal to 01:80:C2:00:00:00 are
 *		broadcast to all the ports.
 */
JL_API jl_api_ret_t jl_cpu_allow_bpdu_enable_set(jl_enable_t enable);

/**
 * @brief Get allow bpdu packets send to CPU port is enabled or not
 * @param penable: enable state
 * @return Get successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		This API can be used to get allow bpdu packets send to CPU port or not.
 *		If set then packets that have the destination MAC address equal to
 *		01:80:C2:00:00:00 are sent to the CPU port. Otherwise, the packets
 *		that have the destination MAC address equal to 01:80:C2:00:00:00 are
 *		broadcast to all the ports.
 */
JL_API jl_api_ret_t jl_cpu_allow_bpdu_enable_get(jl_enable_t *penable);

/**
 * @brief Set allow rst-bpdu packets send to CPU port or not
 * @param enable: enable state
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		This API can be used to set allow rst bpdu packets send to CPU port or not.
 *		If set then packets that have the destination MAC address equal to
 *		01:00:0C:CC:CC:CD are sent to the CPU port.
 */
JL_API jl_api_ret_t jl_cpu_allow_rst_bpdu_enable_set(jl_enable_t enable);

/**
 * @brief Get allow rst-bpdu packets send to CPU port or not
 * @param penable: enable state
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		This API can be used to get allow rst bpdu packets send to CPU port or not.
 *		If the packets's destination MAC address is equal to
 *		01:00:0C:CC:CC:CD, then they are allowed to send to the CPU port.
 */
JL_API jl_api_ret_t jl_cpu_allow_rst_bpdu_enable_get(jl_enable_t *penable);

/**
 * @brief Enable or disable To CPU Port Tag
 * @param enable: enable state
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		This API can be used to enable the To CPU Port Tag.
 */
JL_API jl_api_ret_t jl_cpu_tag_enable_set(jl_enable_t enable);

/**
 * @brief Get To CPU Port Tag is enabled or not
 * @param penable: enable state
 * @return Get successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		This API can be used to get the To CPU Port Tag is enabled or not.
 */
JL_API jl_api_ret_t jl_cpu_tag_enable_get(jl_enable_t *penable);

/**
 * @brief Set enable lldp packets to send to CPU port or not
 * @param enable: 1:send to cpu; 0:Transparent
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		This API can enable lldp packets To CPU or Transparent.
 *		LLDP is as following definition:
 *		DMAC = 01:80:c2:00:00:0e ethertype = 0x88CC
 *		DMAC = 01:80:c2:00:00:03 ethertype = 0x88CC
 *		DMAC = 01:80:c2:00:00:00 ethertype = 0x88CC
 */
JL_API jl_api_ret_t jl_cpu_allow_lldp_enable_set(jl_enable_t enable);

/**
 * @brief Get lldp packets send to CPU port is enabled or not
 * @param penable: enable status, 1:send to cpu; 0:Transparent
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		This API can get lldp packets To CPU is enabled or not.
 *		LLDP is as following definition:
 *		DMAC = 01:80:c2:00:00:0e ethertype = 0x88CC
 *		DMAC = 01:80:c2:00:00:03 ethertype = 0x88CC
 *		DMAC = 01:80:c2:00:00:00 ethertype = 0x88CC
 */
JL_API jl_api_ret_t jl_cpu_allow_lldp_enable_get(jl_enable_t *penable);

#ifdef __cplusplus
}
#endif
#endif /* __JL_API_CPU_H__*/
/** @} CPU APIs*/

