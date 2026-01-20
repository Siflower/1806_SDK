/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_oam.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_OAM_H
#define __FAL_TIGER_OAM_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "fal_cmm.h"


/**
 * @internal      fal_tiger_oam_en_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_oam_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_oam_en_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_oam_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_oam_bypass_accessctrl_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_oam_bypass_accessctrl_set(yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      fal_tiger_oam_bypass_accessctrl_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_oam_bypass_accessctrl_get(yt_unit_t unit, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_oam_parser_act_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     action              -oam parser action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_oam_parser_act_set(yt_unit_t unit, yt_port_t port, yt_oam_parser_action_t action);


/**
 * @internal      fal_tiger_oam_parser_act_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    paction             -oam parser action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_oam_parser_act_get(yt_unit_t unit, yt_port_t port, yt_oam_parser_action_t *paction);


/**
 * @internal      fal_tiger_oam_mux_act_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     action              -oam multiplexer action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_oam_mux_act_set(yt_unit_t unit, yt_port_t port, yt_oam_mux_action_t action);


/**
 * @internal      fal_tiger_oam_mux_act_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    paction             -oam multiplexer action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_oam_mux_act_get(yt_unit_t unit, yt_port_t port, yt_oam_mux_action_t *paction);


/**
 * @internal      fal_tiger_oam_mac_swap_en_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_oam_mac_swap_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_oam_mac_swap_en_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_oam_mac_swap_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
