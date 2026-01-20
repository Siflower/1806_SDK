/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_stp.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_STP_H
#define __FAL_TIGER_STP_H


#include "fal_cmm.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @internal      fal_tiger_stp_state_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     stpId               -stp instance id
 * @param[in]     port                -port num
 * @param[in]     state               -stp state, STP_STATE_LEARN, STP_STATE_DISCARD, STP_STATE_FORWARD
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stp_state_set(yt_unit_t unit, yt_stp_id_t stpId, yt_port_t port, yt_stp_state_t state);

/**
 * @internal      fal_tiger_stp_state_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     stpId               -stp instance id
 * @param[in]     port                -port num
 * @param[out]    pState              -stp state, STP_STATE_LEARN, STP_STATE_DISCARD, STP_STATE_FORWARD
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stp_state_get(yt_unit_t unit, yt_stp_id_t stpId, yt_port_t port, yt_stp_state_t *pState);

/**
 * @internal      fal_tiger_stp_instance_map_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     stpId               -stp instance id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */

extern yt_ret_t fal_tiger_stp_instance_map_set(yt_unit_t unit, yt_vlan_t vid, yt_stp_id_t stpId);

/**
 * @internal      fal_tiger_stp_instance_map_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[out]    pStpId              -stp instance id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stp_instance_map_get(yt_unit_t unit, yt_vlan_t vid, yt_stp_id_t *pStpId);

/**
 * @internal      fal_tiger_stp_instance_map_vlanlist_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     pVlanlist           -vlan list
 * @param[in]     stpId               -stp instance id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stp_instance_map_vlanlist_set(yt_unit_t unit, yt_vlan_mask_t *pVlanlist, yt_stp_id_t stpId);

/**
 * @internal      fal_tiger_stp_bpdu_act_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     bpduType            -Customer bpdu or Provider bpdu type
 * @param[in]     action              -Fwd/Drop/Copy/Trap
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stp_bpdu_act_set(yt_unit_t unit, yt_port_t port,
    yt_stp_bpdu_type_t bpduType, yt_act_type_t action);

/**
 * @internal      fal_tiger_stp_bpdu_act_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     bpduType            -Customer bpdu or Provider bpdu type
 * @param[in]     action              -Fwd/Drop/Copy/Trap
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */

extern yt_ret_t fal_tiger_stp_bpdu_act_get(yt_unit_t unit, yt_port_t port,
    yt_stp_bpdu_type_t bpduType, yt_act_type_t *pAction);

/**
 * @internal      fal_tiger_stp_bpdu_egr_filter_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     bpduType            -Customer bpdu or Provider bpdu type
 * @param[in]     portMask            -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stp_bpdu_egr_filter_set(yt_unit_t unit, yt_stp_bpdu_type_t bpduType,
    yt_port_mask_t portMask);

/**
 * @internal      fal_tiger_stp_bpdu_egr_filter_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     bpduType            -Customer bpdu or Provider bpdu type
 * @param[out]    pPortMask           -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stp_bpdu_egr_filter_get(yt_unit_t unit, yt_stp_bpdu_type_t bpduType,
    yt_port_mask_t *pPortMask);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
