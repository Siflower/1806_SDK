/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_stp.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_STP_H
#define __YT_STP_H


#include "yt_cmm.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum yt_stp_state_e
{
    STP_STATE_FORWARD = 0,
    STP_STATE_LEARN,
    STP_STATE_DISCARD,
    STP_STATE_DISABLE,
}yt_stp_state_t;

typedef enum yt_stp_bpdu_type_e
{
    CUSTOMER_BPDU_TYPE,
    PROVIDER_BPDU_TYPE,
}yt_stp_bpdu_type_t;

/**
 * @internal      yt_stp_state_set
 * @endinternal
 *
 * @brief         Set stp state based on port index
 * @note          APPLICABLE DEVICES  -Tiger,Shark
 * @param[in]     unit                -unit id
 * @param[in]     stpId               -stp instance id
 * @param[in]     port                -port num
 * @param[in]     state               -stp state, STP_STATE_LEARN, STP_STATE_DISCARD, STP_STATE_FORWARD
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port is not valid
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_stp_state_set(yt_unit_t unit, yt_stp_id_t stpId, yt_port_t port, yt_stp_state_t state);

/**
 * @internal      yt_stp_state_get
 * @endinternal
 *
 * @brief         Get stp state based on port index
 * @note          APPLICABLE DEVICES  -Tiger,Shark
 * @param[in]     unit                -unit id
 * @param[in]     stpId               -stp instance id
 * @param[in]     port                -port num
 * @param[out]    pState              -stp state, STP_STATE_LEARN, STP_STATE_DISCARD, STP_STATE_FORWARD
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 */
extern yt_ret_t yt_stp_state_get(yt_unit_t unit, yt_stp_id_t stpId, yt_port_t port, yt_stp_state_t *pState);

/**
 * @internal      yt_stp_instance_map_set
 * @endinternal
 *
 * @brief         Map vlan id to instance id
 * @note          APPLICABLE DEVICES  -Tiger,Shark
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     stpId               -stp instance id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_stp_instance_map_set(yt_unit_t unit, yt_vlan_t vid, yt_stp_id_t stpId);

/**
 * @internal      yt_stp_instance_map_vlanlist_set
 * @endinternal
 *
 * @brief         Map vlan list to instance id
 * @note          APPLICABLE DEVICES  -Tiger,Shark
 * @param[in]     unit                -unit id
 * @param[in]     pVlanlist           -vlan list
 * @param[in]     stpId               -stp instance id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 */
extern yt_ret_t yt_stp_instance_map_vlanlist_set(yt_unit_t unit, yt_vlan_mask_t *pVlanlist, yt_stp_id_t stpId);

/**
 * @internal      yt_stp_instance_map_get
 * @endinternal
 *
 * @brief         Get instance id based on vlan id
 * @note          APPLICABLE DEVICES  -Tiger,Shark
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[out]    pStpId              -stp instance id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 */
extern yt_ret_t yt_stp_instance_map_get(yt_unit_t unit, yt_vlan_t vid, yt_stp_id_t *pStpId);

/**
 * @internal      yt_stp_bpdu_egr_filter_set
 * @endinternal
 *
 * @brief         Set egress filter portmask based on bpdu type
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     bpduType            -Customer bpdu or Provider bpdu type
 * @param[in]     portMask            -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORTLIST    -portlist is not valid
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_stp_bpdu_egr_filter_set(yt_unit_t unit, yt_stp_bpdu_type_t bpduType, yt_port_mask_t portMask);

/**
 * @internal      yt_stp_bpdu_egr_filter_get
 * @endinternal
 *
 * @brief         Get egress filter portmask based on bpdu type
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     bpduType            -Customer bpdu or Provider bpdu type
 * @param[out]    pPortMask           -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_stp_bpdu_egr_filter_get(yt_unit_t unit, yt_stp_bpdu_type_t bpduType, yt_port_mask_t *pPortMask);

/**
 * @internal      yt_stp_bpdu_act_set
 * @endinternal
 *
 * @brief         Set bpdu action based on port
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     bpduType            -Customer bpdu or Provider bpdu type
 * @param[in]     action              -Fwd/Drop/Copy/Trap
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_stp_bpdu_act_set(yt_unit_t unit, yt_port_t port, yt_stp_bpdu_type_t bpduType, yt_act_type_t action);

/**
 * @internal      yt_stp_bpdu_act_get
 * @endinternal
 *
 * @brief         Get bpdu action based on port
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     bpduType            -Customer bpdu or Provider bpdu type
 * @param[in]     action              -Fwd/Drop/Copy/Trap
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -on port is not valid
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 */
extern yt_ret_t yt_stp_bpdu_act_get(yt_unit_t unit, yt_port_t port, yt_stp_bpdu_type_t bpduType, yt_act_type_t *pAction);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
