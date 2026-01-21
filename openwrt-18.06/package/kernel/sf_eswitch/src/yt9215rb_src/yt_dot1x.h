/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_dot1x.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_DOT1X_H
#define __YT_DOT1X_H

#include "yt_cmm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Type of port-based dot1x auth/unauth*/
typedef enum yt_dot1x_auth_status_e
{
    YT_DOT1X_AUTH_STATUS_UNAUTH = 0,
    YT_DOT1X_AUTH_STATUS_AUTH,
    YT_DOT1X_AUTH_STATUS_END
} yt_dot1x_auth_status_t;

typedef enum yt_dot1x_direction_e
{
    YT_DOT1X_AUTH_DIR_BOTH = 0,
    YT_DOT1X_AUTH_DIR_IN,
    YT_DOT1X_AUTH_DIR_END
} yt_dot1x_direction_t;


/**
 * @internal      yt_dot1x_init
 * @endinternal
 *
 * @brief         dot1x module init api
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_init(yt_unit_t unit);


/**
 * @internal      yt_dot1x_portBasedEnable_set
 * @endinternal
 *
 * @brief         dot1x port-based state set
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_portBasedEnable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      yt_dot1x_portBasedEnable_get
 * @endinternal
 *
 * @brief         dot1x port-based state get
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_portBasedEnable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      yt_dot1x_portBasedAuthStatus_set
 * @endinternal
 *
 * @brief         dot1x port-based auth state set
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     port_auth           -auth status, include auth, unauth
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_portBasedAuthStatus_set(yt_unit_t unit, yt_port_t port, yt_dot1x_auth_status_t port_auth);


/**
 * @internal      yt_dot1x_portBasedAuthStatus_get
 * @endinternal
 *
 * @brief         dot1x port-based auth state get
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pPort_auth          -auth status, include auth, unauth
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_portBasedAuthStatus_get(yt_unit_t unit, yt_port_t port, yt_dot1x_auth_status_t *pPort_auth);


/**
 * @internal      yt_dot1x_portBasedDirection_set
 * @endinternal
 *
 * @brief        dot1x  port-based direction set
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     port_direction      -port direction, include both, in
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_portBasedDirection_set(yt_unit_t unit, yt_port_t port, yt_dot1x_direction_t port_direction);


/**
 * @internal      yt_dot1x_portBasedDirection_get
 * @endinternal
 *
 * @brief         dot1x port-based direction get
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pPort_direction     -port direction, include both, in
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_portBasedDirection_get(yt_unit_t unit, yt_port_t port, yt_dot1x_direction_t *pPort_direction);

/**
 * @internal      yt_dot1x_macBasedEnable_set
 * @endinternal
 *
 * @brief         dot1x mac-based state set
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_macBasedEnable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      yt_dot1x_macBasedEnable_get
 * @endinternal
 *
 * @brief         dot1x mac-based state get
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_macBasedEnable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      yt_dot1x_guest_vlan_set
 * @endinternal
 *
 * @brief         dot1x guest vlan state set, work in port-based mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_guest_vlan_set(yt_unit_t unit, yt_vlan_t vid, yt_enable_t enable);


/**
 * @internal      yt_dot1x_guest_vlan_get
 * @endinternal
 *
 * @brief         dot1x guest vlan state get
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_guest_vlan_get(yt_unit_t unit, yt_vlan_t vid, yt_enable_t *pEnable);


/**
 * @internal      yt_dot1x_tx_bypass_bc_set
 * @endinternal
 *
 * @brief         dot1x tx bypass bcast set, work in port-based mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_tx_bypass_bc_set(yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      yt_dot1x_tx_bypass_bc_get
 * @endinternal
 *
 * @brief         dot1x tx bypass bcast get, work in port-based mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_tx_bypass_bc_get(yt_unit_t unit, yt_enable_t *pEnable);


/**
 * @internal      yt_dot1x_tx_bypass_mc_set
 * @endinternal
 *
 * @brief         dot1x tx bypass mcast set, work in port-based mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_tx_bypass_mc_set(yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      yt_dot1x_tx_bypass_mc_get
 * @endinternal
 *
 * @brief         dot1x tx bypass mcast get, work in port-based mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_tx_bypass_mc_get(yt_unit_t unit, yt_enable_t *pEnable);


/**
 * @internal      yt_dot1x_rx_bypass_bc_set
 * @endinternal
 *
 * @brief         dot1x rx bypass bcast set, work in port-based mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_rx_bypass_bc_set(yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      yt_dot1x_rx_bypass_bc_get
 * @endinternal
 *
 * @brief         dot1x rx bypass bcast get, work in port-based mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_rx_bypass_bc_get(yt_unit_t unit, yt_enable_t *pEnable);


/**
 * @internal      yt_dot1x_rx_bypass_mc_set
 * @endinternal
 *
 * @brief         dot1x rx bypass mcast set, work in port-based mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_rx_bypass_mc_set(yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      yt_dot1x_rx_bypass_mc_get
 * @endinternal
 *
 * @brief         dot1x rx bypass mcast get, work in port-based mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]     pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_rx_bypass_mc_get(yt_unit_t unit, yt_enable_t *pEnable);

/**
 * @internal      yt_dot1x_eapol_act_set
 * @endinternal
 *
 * @brief         dot1x eapol act set
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     actType            -act type, include forward, drop, copy, trap
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_eapol_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t actType);

/**
 * @internal      yt_dot1x_eapol_act_get
 * @endinternal
 *
 * @brief         dot1x eapol act get
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pActType            -act type, include forward, drop, copy, trap
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dot1x_eapol_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t* pActType);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
