/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_dot1x.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_DOT1X_H
#define __FAL_TIGER_DOT1X_H


#include "fal_cmm.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _dot1x_data_s
{
    uint16_t dot1x_portbase_auth_mask;
    uint16_t dot1x_portbase_dir_mask;
    uint16_t dot1x_macbase_state_mask;
} dot1x_data_t;

/**
 * @internal      fal_tiger_dot1x_init
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_init(yt_unit_t unit);


/**
 * @internal      fal_tiger_dot1x_portBasedEnable_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_portBasedEnable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_dot1x_portBasedEnable_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_portBasedEnable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_dot1x_portBasedAuthStatus_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     port_auth           -yt_dot1x.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_portBasedAuthStatus_set(yt_unit_t unit, yt_port_t port, yt_dot1x_auth_status_t port_auth);


/**
 * @internal      fal_tiger_dot1x_portBasedAuthStatus_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pPort_auth          -yt_dot1x.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_portBasedAuthStatus_get(yt_unit_t unit, yt_port_t port, yt_dot1x_auth_status_t *pPort_auth);


/**
 * @internal      fal_tiger_dot1x_portBasedDirection_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     port_direction      -yt_dot1x.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_portBasedDirection_set(yt_unit_t unit, yt_port_t port, yt_dot1x_direction_t port_direction);


/**
 * @internal      fal_tiger_dot1x_portBasedDirection_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pPort_direction     -yt_dot1x.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_portBasedDirection_get(yt_unit_t unit, yt_port_t port, yt_dot1x_direction_t *pPort_direction);


/**
 * @internal      fal_tiger_dot1x_macBasedEnable_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_macBasedEnable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_dot1x_macBasedEnable_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_macBasedEnable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_dot1x_guest_vlan_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_guest_vlan_set(yt_unit_t unit, yt_vlan_t vid, yt_enable_t enable);


/**
 * @internal      fal_tiger_dot1x_guest_vlan_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_guest_vlan_get(yt_unit_t unit, yt_vlan_t vid, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_dot1x_tx_bypass_bc_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_tx_bypass_bc_set(yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      fal_tiger_dot1x_tx_bypass_bc_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_tx_bypass_bc_get(yt_unit_t unit, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_dot1x_tx_bypass_mc_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_tx_bypass_mc_set(yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      fal_tiger_dot1x_tx_bypass_mc_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_tx_bypass_mc_get(yt_unit_t unit, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_dot1x_rx_bypass_bc_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_rx_bypass_bc_set(yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      fal_tiger_dot1x_rx_bypass_bc_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_rx_bypass_bc_get(yt_unit_t unit, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_dot1x_rx_bypass_mc_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_rx_bypass_mc_set(yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      fal_tiger_dot1x_rx_bypass_mc_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_rx_bypass_mc_get(yt_unit_t unit, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_dot1x_eapol_act_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     act_type            -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_eapol_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t act_type);

/**
 * @internal      fal_tiger_dot1x_eapol_act_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]     pActType            -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_dot1x_eapol_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t* pActType);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
