/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_oam.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_OAM_H
#define __YT_OAM_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "yt_cmm.h"

typedef enum yt_oam_parser_action_e {
    YT_OAM_PAR_ACTION_FORWARD,
    YT_OAM_PAR_ACTION_DROP,
    YT_OAM_PAR_ACTION_LOOPBACK
}yt_oam_parser_action_t;

typedef enum yt_oam_mux_action_e {
    YT_OAM_MUX_ACTION_FORWARD,
    YT_OAM_MUX_ACTION_DROP,
    YT_OAM_MUX_ACTION_DROP_EXCEPT_CPU
}yt_oam_mux_action_t;


/**
 * @internal      yt_oam_en_set
 * @endinternal
 *
 * @brief         enable oam on port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_oam_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      yt_oam_en_get
 * @endinternal
 *
 * @brief         get enable state of oam on port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_oam_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      yt_oam_bypass_accessctrl_set
 * @endinternal
 *
 * @brief         enable oam packet passthrough ingress access control
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_oam_bypass_accessctrl_set(yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      yt_oam_bypass_accessctrl_get
 * @endinternal
 *
 * @brief         get enable state of oam packet passthrough ingress access control
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_oam_bypass_accessctrl_get(yt_unit_t unit, yt_enable_t *pEnable);


/**
 * @internal      yt_oam_parser_act_set
 * @endinternal
 *
 * @brief         set ingress parser action for oam on port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     action              -oam parser action, include forward, drop, loopback
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_oam_parser_act_set(yt_unit_t unit, yt_port_t port, yt_oam_parser_action_t action);


/**
 * @internal      yt_oam_parser_act_get
 * @endinternal
 *
 * @brief         get parser action of oam on port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pAction             -oam parser action, include forward, drop, loopback
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_oam_parser_act_get(yt_unit_t unit, yt_port_t port, yt_oam_parser_action_t *pAction);


/**
 * @internal      yt_oam_mux_act_set
 * @endinternal
 *
 * @brief         set egress multiplexer action for oam on port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     action              -oam multiplexer action, include forward, drop, drop except cpu
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_oam_mux_act_set(yt_unit_t unit, yt_port_t port, yt_oam_mux_action_t action);


/**
 * @internal      yt_oam_mux_act_get
 * @endinternal
 *
 * @brief         get multiplexer action of oam on port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pAction             -oam multiplexer action, include forward, drop, drop except cpu
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_oam_mux_act_get(yt_unit_t unit, yt_port_t port, yt_oam_mux_action_t *pAction);


/**
 * @internal      yt_oam_mac_swap_en_set
 * @endinternal
 *
 * @brief         set oam mac swap state
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_oam_mac_swap_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      yt_oam_mac_swap_en_get
 * @endinternal
 *
 * @brief         get oam mac swap state
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_oam_mac_swap_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
