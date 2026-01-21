/**
 * @file led.h
 * @brief LED
 */

/**
 * @defgroup led_group LED
 * @{
 */

/**
 * @example example_led_oper_mode.c
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

#ifndef __JL_API_LED_H__
#define __JL_API_LED_H__

#include "jl_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum jl_led_group_e
 * @brief led group.
 */
enum jl_led_group_e {
	LED_GROUP0 = 0,/**< 0 */
	LED_GROUP1,/**< 1 */
	LED_GROUP_END,/**< Invalid */
};

/**
 * @public typedef jl_led_group_t
 */
typedef enum jl_led_group_e jl_led_group_t;

/**
 * @enum jl_led_blink_rate_e
 * @brief led blink rate.
 */
enum jl_led_blink_rate_e {
	LED_BLINKRATE_16MS = 1,/**< 1 */
	LED_BLINKRATE_32MS = 2,/**< 2 */
	LED_BLINKRATE_48MS = 3,/**< 3 */
	LED_BLINKRATE_64MS = 4,/**< 4 */
	LED_BLINKRATE_96MS = 6,/**< 6 */
	LED_BLINKRATE_128MS = 8,/**< 8 */
	LED_BLINKRATE_256MS = 16,/**< 16 */
	LED_BLINKRATE_512MS = 32,/**< 32 */
	LED_BLINKRATE_1024MS = 64,/**< 64 */
	LED_BLINKRATE_2032MS = 127,/**< 127 */
	LED_BLINKRATE_END,/**< Invalid */
};

/**
 * @public typedef jl_led_blink_rate_t
 */
typedef enum jl_led_blink_rate_e jl_led_blink_rate_t;

/**
 * @enum jl_led_config_e
 * @brief led config.
 */
enum jl_led_config_e {
	LED_CONFIG_LINK = 0,/**< 0 */
	LED_CONFIG_ACT,/**< 1 */
	LED_CONFIG_LINK_ACT,/**< 2 */
	LED_CONFIG_SPD10,/**< 3 */
	LED_CONFIG_SPD100,/**< 4 */
	LED_CONFIG_SPD10_ACT,/**< 5 */
	LED_CONFIG_SPD100_ACT,/**< 6 */
	LED_CONFIG_DUPLEX,/**< 7 */
	LED_CONFIG_COL,/**< 8 */
	LED_CONFIG_LOOP,/**< 9 */
	LED_CONFIG_END,/**< Invalid */
};

/**
 * @public typedef jl_led_config_t
 */
typedef enum jl_led_config_e jl_led_config_t;

/**
 * @enum jl_led_active_e
 * @brief led active mode.
 */
enum jl_led_active_e {
	LED_ACTIVE_HIGH = 0,/**< 0 */
	LED_ACTIVE_LOW,/**< 1 */
	LED_ACTIVE_END,/**< Invalid */
};

/**
 * @public typedef jl_led_active_t
 */
typedef enum jl_led_active_e jl_led_active_t;

/**
 * @brief Set LED enable configuration
 * @details This function can be used to enable LED per port per group. portmask indicates an
 * entry of 10-bit correspond to 10 ports. If set to 1, it means the specified led of
 * this port will be enabled. If set to 0, it means the specified led of this port will be disabled.
 * @param[in] group	LED group index
 * @param[in] portmask	PHY portmask
 * @return set LED enable successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_led_enable_set(jl_led_group_t group, jl_portmask_t portmask);

/**
 * @brief Get LED enable status
 * @param[in]	group		LED group index
 * @param[out]	portmask	PHY portmask
 * @return get LED enable status successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_led_enable_get(jl_led_group_t group, jl_portmask_t *portmask);

/**
 * @brief Set LED configuration
 * @details This function can be used to set the specified led group behavior.
 * @param[in] group	LED group index
 * @param[in] config	led behavior configuration
 * @return set LED configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_led_group_config_set(jl_led_group_t group, jl_led_config_t config);

/**
 * @brief Get LED configuration
 * @details This function can be used to get the specified led group behavior
 * @param[in]	group	LED group index
 * @param[out]	pconfig	LED behavior configuration
 * @return get LED configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_led_group_config_get(jl_led_group_t group, jl_led_config_t *pconfig);

/**
 * @brief Set LED blink rate in millisecond
 * @details This function can be used to set the all the LED blink rate.
 * @param[in]	blink_rate	LED blink rate
 * @return set LED blink rate successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_led_blink_rate_set(jl_led_blink_rate_t blink_rate);

/**
 * @brief Get LED blink rate in millisecond
 * @details This function can be used to get the blink rate of all the LED groups.
 * @param[out]	blink_rate	LED blink rate
 * @return get LED blink rate successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_led_blink_rate_get(jl_led_blink_rate_t *blink_rate);

/**
 * @brief Set LED polarity as active high
 * @details This function can be used to set the specified LED group of the specified portmask as
 * active high triggered. Portmask indicates an entry of 10-bit correspond to 10 ports.
 * If set to 1, it means the specified led of this port will be active high triggered.
 * If set to 0, it means the specified led of this port will be active low triggered.
 * @param[in]	group		LED group index
 * @param[in]	portmask	PHY portmask
 * @return set LED active high successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_led_group_active_high_set(jl_led_group_t group, jl_portmask_t portmask);

/**
 * @brief Get LED active high ports
 * @details This function can be used to get the active high triggered ports of the specified LED group.
 * @param[in]	group		LED group index
 * @param[out]	portmask	PHY portmask
 * @return Get LED active high successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_led_group_active_high_get(jl_led_group_t group, jl_portmask_t *portmask);

#ifdef __cplusplus
}
#endif

#endif /* __JL_API_LED_H__*/
/** @} LED APIs*/
