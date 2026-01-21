/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_wol.h
*
* @brief x
*
********************************************************************************
*/
#ifndef YT_WOL_H
#define YT_WOL_H

#include "yt_cmm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @internal      yt_wol_switch_enable_set
 * @endinternal
 *
 * @brief         Set switch wol ctrl state
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_wol_switch_enable_set(yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      yt_wol_switch_enable_get
 * @endinternal
 *
 * @brief         Get switch wol ctrl state
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_wol_switch_enable_get(yt_unit_t unit, yt_enable_t *pEnable);


/**
 * @internal      yt_wol_switch_etherType_set
 * @endinternal
 *
 * @brief         Set switch wol ethertype
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     ethType             -tpid value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_wol_switch_etherType_set(yt_unit_t unit, yt_tpid_t ethType);


/**
 * @internal      yt_wol_switch_etherType_get
 * @endinternal
 *
 * @brief         Get switch wol ethertype
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pEthType           -tpid value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_wol_switch_etherType_get(yt_unit_t unit, yt_tpid_t *pEthType);

/**
 * @internal      yt_wol_port_enable_set
 * @endinternal
 *
 * @brief         Set port wol ctrl state
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_wol_port_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_wol_port_enable_get
 * @endinternal
 *
 * @brief         Get port wol ctrl state
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_wol_port_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_wol_port_macAddr_set
 * @endinternal
 *
 * @brief         Set port wol ctrl MAC address for identification
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port
 * @param[in]     macAddr             -mac address
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_wol_port_macAddr_set(yt_unit_t unit, yt_port_t port, yt_mac_addr_t macAddr);

/**
 * @internal      yt_wol_port_macAddr_get
 * @endinternal
 *
 * @brief         Get port wol ctrl MAC address for identification
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port
 * @param[out]    pMacAddr            -mac address
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_wol_port_macAddr_get(yt_unit_t unit, yt_port_t port, yt_mac_addr_t *pMacAddr);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
