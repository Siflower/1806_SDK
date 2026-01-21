/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_lag.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_lag.h"
#include "fal_dispatch.h"

yt_ret_t yt_lag_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->lag_init(unit);
}

yt_ret_t yt_lag_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *lagState, uint8_t *lagId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == lagState), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == lagId), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->lag_en_get(unit, port, lagState, lagId);
}

yt_ret_t yt_lag_hash_sel_set(yt_unit_t unit, uint16_t hashMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((0 == hashMask || 0x0FF < hashMask), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->lag_hash_sel_set(unit, (uint8_t)hashMask);
}

yt_ret_t yt_lag_hash_sel_get(yt_unit_t unit, uint16_t *pHashMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pHashMask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->lag_hash_sel_get(unit, (uint8_t *)pHashMask);
}

yt_ret_t yt_lag_group_port_set(yt_unit_t unit, uint8_t groupId, yt_port_mask_t memberPortMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit,memberPortMask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->lag_group_port_set(unit, groupId, memberPortMask);
}

yt_ret_t yt_lag_group_port_get(yt_unit_t unit, uint8_t groupId, yt_port_mask_t *pMemberPortmask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pMemberPortmask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->lag_group_port_get(unit, groupId, pMemberPortmask);
}

