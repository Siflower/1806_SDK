/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_stp.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_stp.h"
#include "fal_dispatch.h"


/**
 * @internal      yt_stp_state_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     stp_id              -stp instance id
 * @param[in]     port                -port num
 * @param[in]     state               -stp state, STP_STATE_LEARN, STP_STATE_DISCARD, STP_STATE_FORWARD
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_stp_state_set(yt_unit_t unit, yt_stp_id_t stp_id, yt_port_t port, yt_stp_state_t state)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((STP_STATE_FORWARD < state || STP_STATE_DISABLE > state), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->stp_state_set(unit, stp_id, port, state);
}

/**
 * @internal      yt_stp_state_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     stp_id              -stp instance id
 * @param[in]     port                -port num
 * @param[out]    pState              -stp state, STP_STATE_LEARN, STP_STATE_DISCARD, STP_STATE_FORWARD
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_stp_state_get(yt_unit_t unit, yt_stp_id_t stp_id, yt_port_t port, yt_stp_state_t *pState)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pState), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->stp_state_get(unit, stp_id, port, pState);
}

/**
 * @internal      yt_stp_instance_map_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     stp_id              -stp instance id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_stp_instance_map_set(yt_unit_t unit, yt_vlan_t vid, yt_stp_id_t stp_id )
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->stp_instance_map_set(unit, vid, stp_id);
}

/**
 * @internal      yt_stp_instance_map_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[out]    pStp_id             -stp instance id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_stp_instance_map_get(yt_unit_t unit, yt_vlan_t vid, yt_stp_id_t *pStp_id )
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->stp_instance_map_get(unit, vid, pStp_id );
}
