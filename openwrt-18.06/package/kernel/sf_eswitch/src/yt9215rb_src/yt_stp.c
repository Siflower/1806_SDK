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

yt_ret_t yt_stp_state_set(yt_unit_t unit, yt_stp_id_t stpId, yt_port_t port, yt_stp_state_t state)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((STP_STATE_DISABLE < state || STP_STATE_FORWARD > state), CMM_ERR_EXCEED_RANGE); 

    return YT_DISPATCH(unit)->stp_state_set(unit, stpId, port, state);
}

yt_ret_t yt_stp_state_get(yt_unit_t unit, yt_stp_id_t stpId, yt_port_t port, yt_stp_state_t *pState)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pState), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->stp_state_get(unit, stpId, port, pState);
}

yt_ret_t yt_stp_instance_map_set(yt_unit_t unit, yt_vlan_t vid, yt_stp_id_t stpId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->stp_instance_map_set(unit, vid, stpId);
}

yt_ret_t yt_stp_instance_map_vlanlist_set(yt_unit_t unit, yt_vlan_mask_t *pVlanlist, yt_stp_id_t stpId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pVlanlist), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->stp_instance_map_vlanlist_set(unit, pVlanlist, stpId);
}

yt_ret_t yt_stp_instance_map_get(yt_unit_t unit, yt_vlan_t vid, yt_stp_id_t *pStpId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pStpId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->stp_instance_map_get(unit, vid, pStpId);
}

yt_ret_t yt_stp_bpdu_egr_filter_set(yt_unit_t unit, yt_stp_bpdu_type_t bpduType, yt_port_mask_t portMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, portMask))), CMM_ERR_PORTLIST);
    CMM_PARAM_CHK((bpduType != CUSTOMER_BPDU_TYPE && bpduType != PROVIDER_BPDU_TYPE), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->stp_bpdu_egr_filter_set(unit, bpduType, portMask);
}

yt_ret_t yt_stp_bpdu_egr_filter_get(yt_unit_t unit, yt_stp_bpdu_type_t bpduType, yt_port_mask_t *pPortMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pPortMask), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((bpduType != CUSTOMER_BPDU_TYPE && bpduType != PROVIDER_BPDU_TYPE), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->stp_bpdu_egr_filter_get(unit, bpduType, pPortMask);
}

yt_ret_t yt_stp_bpdu_act_set(yt_unit_t unit, yt_port_t port, yt_stp_bpdu_type_t bpduType, yt_act_type_t action)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((bpduType != CUSTOMER_BPDU_TYPE && bpduType != PROVIDER_BPDU_TYPE), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((action >= ACT_TYPE_TRAP && action < ACT_TYPE_FWD), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->stp_bpdu_act_set(unit, port, bpduType, action);
}

yt_ret_t yt_stp_bpdu_act_get(yt_unit_t unit, yt_port_t port, yt_stp_bpdu_type_t bpduType, yt_act_type_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((bpduType != CUSTOMER_BPDU_TYPE && bpduType != PROVIDER_BPDU_TYPE), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->stp_bpdu_act_get(unit, port, bpduType, pAction);
}

