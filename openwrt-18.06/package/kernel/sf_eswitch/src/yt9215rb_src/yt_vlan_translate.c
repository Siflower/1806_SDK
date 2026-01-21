/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_vlan_translate.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_vlan_translate.h"
#include "fal_dispatch.h"

yt_ret_t yt_vlan_igr_trans_untagPvidIgnore_set(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t  port,  yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_igr_trans_untagPvidIgnore_set(unit, type, port, enable);
}

yt_ret_t yt_vlan_igr_trans_untagPvidIgnore_get(yt_unit_t unit, yt_vlan_type_t  type,  yt_port_t  port,  yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_igr_trans_untagPvidIgnore_get(unit, type, port, pEnable);
}

yt_ret_t yt_vlan_igr_trans_prio_tagPvidIgnore_set(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t  port,  yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_igr_trans_prio_tagPvidIgnore_set(unit, type, port, enable);
}

yt_ret_t yt_vlan_igr_trans_prio_tagPvidIgnore_get(yt_unit_t unit, yt_vlan_type_t  type,  yt_port_t  port,  yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_igr_trans_prio_tagPvidIgnore_get(unit, type, port, pEnable);
}

yt_ret_t  yt_vlan_igr_trans_lookup_miss_ctrl_set(yt_unit_t unit, yt_port_t  port, yt_vlan_type_t  type,  yt_act_type_t missAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((ACT_TYPE_TRAP < missAction || ACT_TYPE_FWD > missAction), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_igr_trans_lookup_miss_ctrl_set(unit, port, type, missAction);
}

yt_ret_t  yt_vlan_igr_trans_lookup_miss_ctrl_get(yt_unit_t unit, yt_port_t  port, yt_vlan_type_t  type,  yt_act_type_t *pMissAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pMissAction), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_igr_trans_lookup_miss_ctrl_get(unit, port, type, pMissAction);
}

yt_ret_t  yt_vlan_igr_trans_rangeProfile_add(yt_unit_t unit, yt_profile_id_t profileId, const yt_vlan_range_group_t *pVlanRange)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_IGRTRANS_PROFILE_MAX_NUM < profileId || 1 > profileId), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pVlanRange), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_igr_trans_rangeProfile_add(unit, profileId, pVlanRange);
}

yt_ret_t  yt_vlan_igr_trans_rangeProfile_get(yt_unit_t unit, yt_profile_id_t profileId, yt_vlan_range_group_t *pVlanRange)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pVlanRange), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_VLAN_IGRTRANS_PROFILE_MAX_NUM < profileId || 1 > profileId), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_igr_trans_rangeProfile_get(unit, profileId, pVlanRange);
}

yt_ret_t  yt_vlan_igr_trans_rangeProfile_del(yt_unit_t unit, yt_profile_id_t profileId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_IGRTRANS_PROFILE_MAX_NUM < profileId || 1 > profileId), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_igr_trans_rangeProfile_del(unit, profileId);
}

yt_ret_t  yt_vlan_igr_trans_port_rangeProfileSel_set(yt_unit_t unit, yt_port_t  port, yt_profile_id_t profileId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_IGRTRANS_PROFILE_MAX_NUM < profileId || 1 > profileId), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_igr_trans_port_rangeProfileSel_set(unit, port, profileId);
}

yt_ret_t  yt_vlan_igr_trans_port_rangeProfileSel_get(yt_unit_t unit, yt_port_t  port, yt_profile_id_t *pProfileId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pProfileId), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_igr_trans_port_rangeProfileSel_get(unit, port, pProfileId);
}

yt_ret_t  yt_vlan_igr_trans_mode_set(yt_unit_t unit, yt_port_t  port, yt_vlan_range_trans_mode_t rangeMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_RANGE_TRANS_MODE_SVLAN < rangeMode ||
        YT_VLAN_RANGE_TRANS_MODE_CVLAN > rangeMode), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_igr_trans_mode_set(unit, port, rangeMode);
}

yt_ret_t  yt_vlan_igr_trans_mode_get(yt_unit_t unit, yt_port_t  port, yt_vlan_range_trans_mode_t *pRangeMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pRangeMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_igr_trans_mode_get(unit, port, pRangeMode);
}

yt_ret_t  yt_vlan_igr_trans_table_add(yt_unit_t unit, const yt_vlan_trans_tbl_t *pRuleTbl,  const yt_vlan_trans_action_tbl_t *pAction, yt_trans_tbl_id_t *pTableIdx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pRuleTbl), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pTableIdx), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_VLAN_TAG_FMT_MAX <= pRuleTbl->stagFormat), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_TAG_FMT_MAX <= pRuleTbl->ctagFormat), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, pRuleTbl->srcPortMaskNeedCare))), CMM_ERR_PORTLIST);

    CMM_PARAM_CHK((YT_VLAN_TRANS_ACTION_MAX <= pAction->svidAction || YT_VLAN_TRANS_ACTION_NO_CHANGE > pAction->svidAction), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_TRANS_ACTION_MAX <= pAction->cvidAction || YT_VLAN_TRANS_ACTION_NO_CHANGE > pAction->cvidAction), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pAction->assignSvid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pAction->assignCvid), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_igr_trans_table_add(unit, pRuleTbl, pAction, pTableIdx);
}

yt_ret_t  yt_vlan_igr_trans_table_add_by_index(yt_unit_t unit, yt_trans_tbl_id_t tableIdx, const yt_vlan_trans_tbl_t *pRuleTbl, const yt_vlan_trans_action_tbl_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pRuleTbl), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_VLAN_TAG_FMT_MAX <= pRuleTbl->stagFormat), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_TAG_FMT_MAX <= pRuleTbl->ctagFormat), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, pRuleTbl->srcPortMaskNeedCare))), CMM_ERR_PORTLIST);

    CMM_PARAM_CHK((YT_VLAN_TRANS_ACTION_MAX <= pAction->svidAction || YT_VLAN_TRANS_ACTION_NO_CHANGE > pAction->svidAction), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_TRANS_ACTION_MAX <= pAction->cvidAction || YT_VLAN_TRANS_ACTION_NO_CHANGE > pAction->cvidAction), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pAction->assignSvid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pAction->assignCvid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_IGR_TRANS_MAX_ENTRY_NUM(unit) < tableIdx), CMM_ERR_EXCEED_RANGE);


    return YT_DISPATCH(unit)->vlan_igr_trans_table_add_by_index(unit, tableIdx, pRuleTbl, pAction);
}

yt_ret_t  yt_vlan_igr_trans_table_get(yt_unit_t unit,  yt_trans_tbl_id_t tableIdx, yt_vlan_trans_tbl_t *pRuleTbl,  yt_vlan_trans_action_tbl_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pRuleTbl), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_VLAN_IGR_TRANS_MAX_ENTRY_NUM(unit) < tableIdx), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_igr_trans_table_get(unit, tableIdx, pRuleTbl, pAction);
}

yt_ret_t  yt_vlan_igr_trans_table_del(yt_unit_t unit, yt_trans_tbl_id_t tableIdx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_IGR_TRANS_MAX_ENTRY_NUM(unit) < tableIdx), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_igr_trans_table_del(unit, tableIdx);
}

yt_ret_t  yt_vlan_egr_trans_lookup_miss_ctrl_set(yt_unit_t unit, yt_port_t  port, yt_vlan_type_t  type,  yt_vlan_egrtrans_lookup_miss_act_t missAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_EGRTRANS_LOOKUP_MISS_FWD > missAction 
                                || YT_VLAN_EGRTRANS_LOOKUP_MISS_DROP < missAction), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_egr_trans_lookup_miss_ctrl_set(unit, port, type, missAction);
}

yt_ret_t  yt_vlan_egr_trans_lookup_miss_ctrl_get(yt_unit_t unit, yt_port_t  port, yt_vlan_type_t  type,  yt_vlan_egrtrans_lookup_miss_act_t *pMissAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pMissAction), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_egr_trans_lookup_miss_ctrl_get(unit, port, type, pMissAction);
}

yt_ret_t  yt_vlan_egr_trans_rangeProfile_add(yt_unit_t unit, yt_profile_id_t profileId, const yt_vlan_range_group_t *pVlanRange)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pVlanRange), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_VLAN_EGRTRANS_PROFILE_MAX_NUM < profileId || 1 > profileId), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_egr_trans_rangeProfile_add(unit, profileId, pVlanRange);
}

yt_ret_t  yt_vlan_egr_trans_rangeProfile_get(yt_unit_t unit, yt_profile_id_t profileId, yt_vlan_range_group_t *pVlanRange)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pVlanRange), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_VLAN_EGRTRANS_PROFILE_MAX_NUM < profileId || 1 > profileId), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_egr_trans_rangeProfile_get(unit, profileId, pVlanRange);
}

yt_ret_t  yt_vlan_egr_trans_rangeProfile_del(yt_unit_t unit, yt_profile_id_t profileId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_EGRTRANS_PROFILE_MAX_NUM < profileId || 1 > profileId), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_egr_trans_rangeProfile_del(unit, profileId);
}

yt_ret_t yt_vlan_egr_trans_table_add(yt_unit_t unit, const yt_egr_vlan_trans_tbl_t  *pRuleTbl, const yt_egr_vlan_trans_action_tbl_t *pAction, yt_trans_tbl_id_t *pTableIdx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pRuleTbl), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pTableIdx), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, pRuleTbl->dstPortMaskNeedCare))), CMM_ERR_PORTLIST);

    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pAction->assignSvid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pAction->assignCvid), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_egr_trans_table_add(unit, pRuleTbl, pAction, pTableIdx);
}

yt_ret_t yt_vlan_egr_trans_table_add_by_index(yt_unit_t unit, yt_trans_tbl_id_t tableIdx, const yt_egr_vlan_trans_tbl_t  *pRuleTbl, const yt_egr_vlan_trans_action_tbl_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pRuleTbl), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, pRuleTbl->dstPortMaskNeedCare))), CMM_ERR_PORTLIST);

    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pAction->assignSvid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pAction->assignCvid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_EGR_TRANS_MAX_ENTRY_NUM(unit) < tableIdx), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_egr_trans_table_add_by_index(unit, tableIdx, pRuleTbl, pAction);
}

yt_ret_t yt_vlan_egr_trans_table_get(yt_unit_t unit, yt_trans_tbl_id_t tableIdx, yt_egr_vlan_trans_tbl_t  *pRuleTbl, yt_egr_vlan_trans_action_tbl_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pRuleTbl), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_VLAN_EGR_TRANS_MAX_ENTRY_NUM(unit) < tableIdx), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_egr_trans_table_get(unit, tableIdx, pRuleTbl, pAction);
}

yt_ret_t  yt_vlan_egr_trans_table_del(yt_unit_t unit, yt_trans_tbl_id_t tableIdx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_EGR_TRANS_MAX_ENTRY_NUM(unit) < tableIdx), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_egr_trans_table_del(unit, tableIdx);
}

