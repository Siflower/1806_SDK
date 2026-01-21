/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_debug.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_DEBUG_H__
#define __FAL_TIGER_DEBUG_H__


#include "yt_types.h"
#include "yt_debug.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @internal      fal_tiger_debug_phyTemplate_test_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    mode        -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_debug_phyTemplate_test_set(yt_unit_t unit, yt_port_t port, yt_utp_template_testmode_t mode);


/**
 * @internal      fal_tiger_debug_phyLoopback_test_set
 * @endinternal
 *
 * @brief         set phy loopback mode.
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     phyMode              -copper, phy, combo
 * @param[in]     mode                -internal loopback or external loopback or remote loopback.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_debug_phyLoopback_test_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_phy_loopback_mode_t mode);

/**
 * @internal      fal_tiger_debug_utp_snr_get
 * @endinternal
 *
 * @brief         get utp snr value based on port id
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pSnrValue           -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_debug_utp_snr_get(yt_unit_t unit, yt_port_t port, yt_utp_snr_t *pSnrValue);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
