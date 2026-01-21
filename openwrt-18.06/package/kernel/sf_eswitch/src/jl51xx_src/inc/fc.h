/**
 * @file fc.h
 * @brief Flow Control
 */

/**
 * @defgroup fc_group Flow Control
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


#ifndef __JL_API_FC_H__
#define __JL_API_FC_H__

#include "jl_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Set enable or disable port flow control
 * @details  This function can enable or disable flow control in the specified port.
 * @param[in]  port      Port ID
 * @param[in]  enable    Enable or disable
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_ret_t jl_port_fc_enable_set(jl_port_t port, jl_enable_t enable);

/**
 * @brief Get port flow control status
 * @details  This function can get flow control status in the specified port.
 * @param[in]  port      Port ID
 * @param[out]  penable    Get enable status
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_ret_t jl_port_fc_enable_get(jl_port_t port, jl_enable_t *penable);

/**
 * @brief Set port flow control threshold
 * @details  This function can set flow control threshold in the specified port.
 * @param[in]  port      Port ID
 * @param[in]  threshold_on   On threshold for the number of used free-for-all cells for this port,range[0,0x3ff]
 * @param[in]  threshold_off  Off threshold for the number of used free-for-all cells for this port,range[0,0x3ff]
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 * @note  The value of threshold_off should be greater than the value of threshold_on
 */
JL_API jl_ret_t jl_port_fc_threshold_set(jl_port_t port,
	jl_uint32 threshold_on, jl_uint32 threshold_off);

/**
 * @brief Get port flow control threshold
 * @details  This function can get flow control threshold in the specified port.
 * @param[in]  port      Port ID
 * @param[out]  pthreshold_on   Get On threshold for this port
 * @param[out]  pthreshold_off  Get Off threshold for this port
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_ret_t jl_port_fc_threshold_get(jl_port_t port,
	jl_uint32 *pthreshold_on, jl_uint32 *pthreshold_off);

/**
 * @brief Set port flow control tx pause time
 * @details  This function can set flow control tx pause time in the specified port.
 * @param[in]  port      Port ID
 * @param[in]  pause_time   tx pause time,range[0,65535]
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 * @note   Default value is 65535.
 */
JL_API jl_ret_t jl_port_fc_pause_time_set(jl_port_t port, jl_uint32 pause_time);

/**
 * @brief Get port flow control tx pause time
 * @details  This function can get flow control tx pause time in the specified port.
 * @param[in]  port      Port ID
 * @param[in]  ppause_time   get tx pause time
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_ret_t jl_port_fc_pause_time_get(jl_port_t port, jl_uint32 *ppause_time);

/**
 * @brief Set enable or disable tail drop of the specified port
 * @param[in]  phyport  port number
 * @param[in]  enable  1: enable tail drop; 0: disable tail drop
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_taildrop_enable_set(jl_port_t phyport, jl_enable_t enable);

/**
 * @brief Get tail drop enable status per source port
 * @param[in]   phyport  port number
 * @param[out]  penable  the return enable status, 1: enable tail drop; 0: disable tail drop
 * @return Get successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_taildrop_enable_get(jl_port_t phyport, jl_enable_t *penable);

/**
 * @brief Get the drop counters due to taildrop
 * @param[out]  pcntr  drop counter
 * @return Get successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_taildrop_counter_get(jl_uint32 *pcntr);


#ifdef __cplusplus
}
#endif

#endif /* __JL_API_FC_H__*/
/** @} Flow Control APIs*/

