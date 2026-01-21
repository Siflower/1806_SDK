/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_debug.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_DEBUG_H__
#define __YT_DEBUG_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "yt_types.h"
#include "yt_cmm.h"
#include "yt_port.h"

/**
 * @internal      yt_debug_phyTemplate_test_set
 * @endinternal
 *
 * @brief         set utp template test mode.
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     mode                -different test mode,refer yt_utp_template_testmode_t for details.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_debug_phyTemplate_test_set(yt_unit_t unit, yt_port_t port, yt_utp_template_testmode_t mode);


/**
 * @internal      yt_debug_phyLoopback_test_set
 * @endinternal
 *
 * @brief         set phy loopback mode.Note:use yt_port_phy_force_set() to set port speed if needed.
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     phyMode              -copper, phy, combo
 * @param[in]     mode                -internal loopback or external loopback or remote loopback.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_debug_phyLoopback_test_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_phy_loopback_mode_t mode);

/**
 * @internal      yt_debug_utp_snr_get
 * @endinternal
 *
 * @brief         get phy utp snr value
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     phyMode              -copper, phy, combo
 * @param[out]    pSnrVal             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_debug_utp_snr_get(yt_unit_t unit, yt_port_t port, yt_utp_snr_t *pSnrVal);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
