/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_storm_ctrl.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_STORM_CTRL_H
#define __YT_STORM_CTRL_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "yt_cmm.h"
#include "yt_rate.h"

typedef enum yt_storm_type_e
{
    YT_STORM_TYPE_BCAST,
    YT_STORM_TYPE_UNKNOWN_UCAST,
    YT_STORM_TYPE_L2_MCAST,
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    YT_STORM_TYPE_IPV4_MCAST,
    YT_STORM_TYPE_IPV6_MCAST,
#endif
    YT_STORM_TYPE_L2_UNKNOWN_MCAST,
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    YT_STORM_TYPE_IPV4_UNKNOWN_MCAST,
    YT_STORM_TYPE_IPV6_UNKNOWN_MCAST,  
#endif
}yt_storm_type_t;

/**
 * @internal      yt_storm_ctrl_init
 * @endinternal
 *
 * @brief         init storm control module
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_storm_ctrl_init(yt_unit_t unit);


/**
 * @internal      yt_storm_ctrl_enable_set
 * @endinternal
 *
 * @brief         enable specific storm control on port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     stormType          -storm type,refer to yt_storm_type_t
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_storm_ctrl_enable_set(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, yt_enable_t enable);


/**
 * @internal      yt_storm_ctrl_enable_get
 * @endinternal
 *
 * @brief         get the storm control enable state on port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     stormType          -storm type,refer to yt_storm_type_t
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_storm_ctrl_enable_get(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, yt_enable_t *pEnable);


/**
 * @internal      yt_storm_ctrl_rate_mode_set
 * @endinternal
 *
 * @brief         set the storm control rate mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     stormType          -storm type,refer to yt_storm_type_t
 * @param[in]     pRateMode           -storm rate mode,bps or pps
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_storm_ctrl_rate_mode_set(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, yt_port_rate_mode_t rateMode);


/**
 * @internal      yt_storm_ctrl_rate_mode_get
 * @endinternal
 *
 * @brief         get the storm control rate mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     stormType          -storm type,refer to yt_storm_type_t
 * @param[out]    pRateMode         -storm rate mode,bps or pps
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_storm_ctrl_rate_mode_get(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, yt_port_rate_mode_t *pRateMode);



/**
 * @internal      yt_storm_ctrl_rate_set
 * @endinternal
 *
 * @brief         set storm control rate value.The min unit is 20kbps(1k=1000) or 4pps.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     stormType          -storm type,refer to yt_storm_type_t
 * @param[in]     rate                -storm rate.the range is (20kbps~4*10^6kbps) or (1~2^9kpps),1k=1000.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_storm_ctrl_rate_set(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, uint32_t rate);


/**
 * @internal      yt_storm_ctrl_rate_get
 * @endinternal
 *
 * @brief         get storm control rate value
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     stormType          -storm type,refer to yt_storm_type_t
 * @param[out]    pRate               -storm rate
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_storm_ctrl_rate_get(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, uint32_t *pRate);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
