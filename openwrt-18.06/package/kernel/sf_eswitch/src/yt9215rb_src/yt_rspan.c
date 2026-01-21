/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_rspan.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_rspan.h"
#include "fal_dispatch.h"

yt_ret_t yt_rspan_vlan_info_set(yt_unit_t unit, yt_mirror_group_t groupId, yt_rspan_vlan_info_t *pVlanInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MIRROR_MAX_GROUP_NUM <= groupId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pVlanInfo), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((0 == pVlanInfo->flags), CMM_ERR_INPUT);
    if (pVlanInfo->flags & YT_RSPAN_VLAN_FLAG_VID)
    {
        CMM_PARAM_CHK((YT_VLAN_ID_MAX < pVlanInfo->vid), CMM_ERR_EXCEED_RANGE);
    }
    if (pVlanInfo->flags & YT_RSPAN_VLAN_FLAG_PRI)
    {
        CMM_PARAM_CHK((CAL_MAX_VALUE_INT_PRI(unit) < pVlanInfo->pri), CMM_ERR_INPUT);
    }
    if (pVlanInfo->flags & YT_RSPAN_VLAN_FLAG_DEI)
    {
        CMM_PARAM_CHK((TRUE < pVlanInfo->dei), CMM_ERR_INPUT);
    }

    return YT_DISPATCH(unit)->rspan_vlan_info_set(unit, groupId, pVlanInfo);
}

yt_ret_t yt_rspan_vlan_info_get(yt_unit_t unit, yt_mirror_group_t groupId, yt_rspan_vlan_info_t *pVlanInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MIRROR_MAX_GROUP_NUM <= groupId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pVlanInfo), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((0 == pVlanInfo->flags), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rspan_vlan_info_get(unit, groupId, pVlanInfo);
}

yt_ret_t yt_rspan_smac_learn_set(yt_unit_t unit, yt_mirror_group_t groupId, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MIRROR_MAX_GROUP_NUM <= groupId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((enable > YT_ENABLE || enable < YT_DISABLE), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rspan_smac_learn_set(unit, groupId, enable);
}

yt_ret_t yt_rspan_smac_learn_get(yt_unit_t unit, yt_mirror_group_t groupId, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MIRROR_MAX_GROUP_NUM <= groupId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rspan_smac_learn_get(unit, groupId, pEnable);
}

yt_ret_t yt_rspan_src_dev_tag_op_set(yt_unit_t unit, yt_mirror_group_t groupId, yt_rspan_src_dev_tag_op_t tagOp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MIRROR_MAX_GROUP_NUM <= groupId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((tagOp > YT_RSPAN_SRC_DEV_REPLACE_CSTAG || tagOp < YT_RSPAN_SRC_DEV_ADD_TAG), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rspan_src_dev_tag_op_set(unit, groupId, tagOp);
}

yt_ret_t yt_rspan_src_dev_tag_op_get(yt_unit_t unit, yt_mirror_group_t groupId, yt_rspan_src_dev_tag_op_t *pTagOp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MIRROR_MAX_GROUP_NUM <= groupId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pTagOp), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rspan_src_dev_tag_op_get(unit, groupId, pTagOp);
}

yt_ret_t yt_rspan_dst_dev_tag_op_set(yt_unit_t unit, yt_mirror_group_t groupId, yt_rspan_dst_dev_tag_op_t tagOp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MIRROR_MAX_GROUP_NUM <= groupId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((tagOp < YT_RSPAN_DST_DEV_KEEP_TAG || tagOp > YT_RSPAN_DST_DEV_REMOVE_TAG), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rspan_dst_dev_tag_op_set(unit, groupId, tagOp);
}

yt_ret_t yt_rspan_dst_dev_tag_op_get(yt_unit_t unit, yt_mirror_group_t groupId, yt_rspan_dst_dev_tag_op_t *pTagOp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MIRROR_MAX_GROUP_NUM <= groupId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pTagOp), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rspan_dst_dev_tag_op_get(unit, groupId, pTagOp);
}

yt_ret_t yt_rspan_egr_enable_set(yt_unit_t unit, yt_mirror_group_t groupId, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MIRROR_MAX_GROUP_NUM <= groupId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((enable > YT_ENABLE || enable < YT_DISABLE), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rspan_egr_enable_set(unit, groupId, enable);
}

yt_ret_t yt_rspan_egr_enable_get(yt_unit_t unit, yt_mirror_group_t groupId, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_MIRROR_MAX_GROUP_NUM <= groupId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rspan_egr_enable_get(unit, groupId, pEnable);
}

yt_ret_t yt_rspan_igr_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((enable > YT_ENABLE || enable < YT_DISABLE), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rspan_igr_enable_set(unit, port, enable);
}

yt_ret_t yt_rspan_igr_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rspan_igr_enable_get(unit, port, pEnable);
}

yt_ret_t yt_rspan_fwd_pkt_filter_enable_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((enable > YT_ENABLE || enable < YT_DISABLE), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rspan_fwd_pkt_filter_enable_set(unit, enable);
}

yt_ret_t yt_rspan_fwd_pkt_filter_enable_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rspan_fwd_pkt_filter_enable_get(unit, pEnable);
}