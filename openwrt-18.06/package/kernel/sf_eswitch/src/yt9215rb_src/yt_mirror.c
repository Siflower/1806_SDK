/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_mirror.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_mirror.h"
#include "fal_dispatch.h"

yt_ret_t yt_mirror_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->mirror_init(unit);
}

yt_ret_t yt_mirror_group_set(yt_unit_t unit, yt_mirror_group_t grpId,
                                     yt_mirror_entry_t *pMirrorEntry)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MIRROR_MAX_GROUP_NUM <= grpId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pMirrorEntry), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((0 == pMirrorEntry->flags), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->mirror_group_set(unit, grpId, pMirrorEntry);
}

yt_ret_t yt_mirror_group_get(yt_unit_t unit, yt_mirror_group_t grpId,
                                     yt_mirror_entry_t *pMirrorEntry)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MIRROR_MAX_GROUP_NUM <= grpId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pMirrorEntry), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((0 == pMirrorEntry->flags), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->mirror_group_get(unit, grpId, pMirrorEntry);
}
