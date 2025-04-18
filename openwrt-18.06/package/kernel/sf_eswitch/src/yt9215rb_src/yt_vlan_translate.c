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


/**
 * @internal      yt_vlan_trans_untagPvidIgnore_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_vlan_trans_untagPvidIgnore_set(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t  port,  yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((VLAN_TYPE_SVLAN < type || VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_trans_untagPvidIgnore_set(unit, type, port, enable);
}

/**
 * @internal      yt_vlan_trans_untagPvidIgnore_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_vlan_trans_untagPvidIgnore_get(yt_unit_t unit, yt_vlan_type_t  type,  yt_port_t  port,  yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((VLAN_TYPE_SVLAN < type || VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_trans_untagPvidIgnore_get(unit, type, port, pEnable);
}

/**
 * @internal      yt_vlan_trans_rangeProfile_add
 * @endinternal
 *
 * @brief         Add vlan translate range profile
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vlan_range          -ingress vlan translate profile group info, one of them should bind to the port
 * @param[out]     pProfile_id         -ingress vlan range profile index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_trans_rangeProfile_add(yt_unit_t unit, yt_vlan_range_group_t vlan_range, yt_profile_id_t *pProfile_id)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pProfile_id), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vlan_range.vid_range0_max ||
        YT_VLAN_ID_MAX < vlan_range.vid_range1_max ||
        YT_VLAN_ID_MAX < vlan_range.vid_range2_max ||
        YT_VLAN_ID_MAX < vlan_range.vid_range3_max), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((vlan_range.vid_range0_max < vlan_range.vid_range0_max ||
        vlan_range.vid_range1_max < vlan_range.vid_range1_min ||
        vlan_range.vid_range2_max < vlan_range.vid_range2_min ||
        vlan_range.vid_range3_max < vlan_range.vid_range3_min), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_trans_rangeProfile_add(unit, vlan_range, pProfile_id);
}

/**
 * @internal      yt_vlan_trans_rangeProfile_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     profile_id          -ingress vlan profile index
 * @param[out]    pVlan_range         -ingress vlan translate profile group info, one of them should bind to the port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_trans_rangeProfile_get(yt_unit_t unit, yt_profile_id_t profile_id, yt_vlan_range_group_t *pVlan_range)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pVlan_range), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_trans_rangeProfile_get(unit, profile_id, pVlan_range);
}

/**
 * @internal      yt_vlan_trans_rangeProfile_del
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     profile_id          -ingress vlan profile index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_trans_rangeProfile_del(yt_unit_t unit, yt_profile_id_t profile_id)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->vlan_trans_rangeProfile_del(unit, profile_id);
}

/**
 * @internal      yt_vlan_trans_port_rangeProfileSel_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     profile_id          -ingress vlan profile index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_trans_port_rangeProfileSel_set(yt_unit_t unit, yt_port_t  port, yt_profile_id_t profile_id)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->vlan_trans_port_rangeProfileSel_set(unit, port, profile_id);
}

/**
 * @internal      yt_vlan_trans_port_rangeProfileSel_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pProfile_id         -ingress vlan profile index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_trans_port_rangeProfileSel_get(yt_unit_t unit, yt_port_t  port, yt_profile_id_t *pProfile_id)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pProfile_id), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_trans_port_rangeProfileSel_get(unit, port, pProfile_id);
}

/**
 * @internal      yt_vlan_trans_mode_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     rangeMode           -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_trans_mode_set(yt_unit_t unit, yt_port_t  port, yt_vlan_range_trans_mode_t rangeMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((VLAN_RANGE_TRANS_MODE_SVLAN < rangeMode ||
        VLAN_RANGE_TRANS_MODE_CVLAN > rangeMode), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_trans_mode_set(unit, port, rangeMode);
}

/**
 * @internal      yt_vlan_trans_mode_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pRangeMode          -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_trans_mode_get(yt_unit_t unit, yt_port_t  port, yt_vlan_range_trans_mode_t *pRangeMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pRangeMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_trans_mode_get(unit, port, pRangeMode);
}

/**
 * @internal      yt_vlan_igr_trans_table_add
 * @endinternal
 *
 * @brief         Add ingress vlan translate table and action
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     pRuleTbl            -ingress vlan translate table info
 * @param[in]     pAction             -ingress vlan translate action
 * @param[out]     pEntry_id           -vlan translate table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_TABLE_FULL        -no free table entry
 */
yt_ret_t  yt_vlan_igr_trans_table_add(yt_unit_t unit, yt_vlan_trans_tbl_t *pRuleTbl,  yt_vlan_trans_action_tbl_t *pAction, yt_trans_tbl_id_t *pEntry_id)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pRuleTbl), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pEntry_id), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_igr_trans_table_add(unit, pRuleTbl, pAction, pEntry_id);
}

/**
 * @internal      yt_vlan_igr_trans_table_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     entry_id            -vlan translate table index
 * @param[out]    pRuleTbl            -x
 * @param[out]    pAction             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_igr_trans_table_get(yt_unit_t unit,  yt_trans_tbl_id_t entry_id, yt_vlan_trans_tbl_t *pRuleTbl,  yt_vlan_trans_action_tbl_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pRuleTbl), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_igr_trans_table_get(unit, entry_id, pRuleTbl, pAction);
}

/**
 * @internal      yt_vlan_igr_trans_table_del
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     entry_id            -vlan translate table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_igr_trans_table_del(yt_unit_t unit, yt_trans_tbl_id_t entry_id)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->vlan_igr_trans_table_del(unit, entry_id);
}

/**
 * @internal      yt_vlan_egr_trans_table_add
 * @endinternal
 *
 * @brief         Add engress vlan translate table and action
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     pRuleTbl            -egress vlan translate table info
 * @param[in]     pAction             -egress vlan translate action
 * @param[out]     pEntry_id           -vlan translate table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_TABLE_FULL        -no free table entry
 */
yt_ret_t yt_vlan_egr_trans_table_add(yt_unit_t unit, yt_egr_vlan_trans_tbl_t  *pRuleTbl, yt_egr_vlan_trans_action_tbl_t *pAction, yt_trans_tbl_id_t *pEntry_id)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pRuleTbl), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pEntry_id), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_egr_trans_table_add(unit, pRuleTbl, pAction, pEntry_id);
}

/**
 * @internal      yt_vlan_egr_trans_table_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     entry_idx           -vlan translate table index
 * @param[out]    pRuleTbl            -x
 * @param[out]    pAction             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_vlan_egr_trans_table_get(yt_unit_t unit, yt_trans_tbl_id_t entry_idx, yt_egr_vlan_trans_tbl_t  *pRuleTbl, yt_egr_vlan_trans_action_tbl_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pRuleTbl), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_egr_trans_table_get(unit, entry_idx, pRuleTbl, pAction);
}

/**
 * @internal      yt_vlan_egr_trans_table_del
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     entry_idx           -vlan translate table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_egr_trans_table_del(yt_unit_t unit, yt_trans_tbl_id_t entry_idx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->vlan_egr_trans_table_del(unit, entry_idx);
}
