/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_vlan_translate.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_VLAN_TRANSLATE_H__
#define __FAL_TIGER_VLAN_TRANSLATE_H__


#include "fal_cmm.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @internal      fal_tiger_vlan_trans_untagPvidIgnore_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_vlan_trans_untagPvidIgnore_set(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t  port,  yt_enable_t enable);


/**
 * @internal      fal_tiger_vlan_trans_untagPvidIgnore_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_vlan_trans_untagPvidIgnore_get(yt_unit_t unit, yt_vlan_type_t  type,  yt_port_t  port,  yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_vlan_trans_rangeProfile_add
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vlan_range          -ingress vlan translate profile group info, one of them should bind to the port
 * @param[in]     pProfile_id         -ingress vlan profile index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_trans_rangeProfile_add(yt_unit_t unit, yt_profile_id_t profile_id, const yt_vlan_range_group_t *pVlan_range);


/**
 * @internal      fal_tiger_vlan_trans_rangeProfile_get
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
extern yt_ret_t  fal_tiger_vlan_trans_rangeProfile_get(yt_unit_t unit, yt_profile_id_t profile_id, yt_vlan_range_group_t *pVlan_range);


/**
 * @internal      fal_tiger_vlan_trans_rangeProfile_del
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     profile_id          -ingress vlan profile index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_trans_rangeProfile_del(yt_unit_t unit, yt_profile_id_t profile_id);


/**
 * @internal      fal_tiger_vlan_trans_port_rangeProfileSel_set
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
extern yt_ret_t  fal_tiger_vlan_trans_port_rangeProfileSel_set(yt_unit_t unit, yt_port_t  port, yt_profile_id_t profile_id);


/**
 * @internal      fal_tiger_vlan_trans_port_rangeProfileSel_get
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
extern yt_ret_t  fal_tiger_vlan_trans_port_rangeProfileSel_get(yt_unit_t unit, yt_port_t  port, yt_profile_id_t *pProfile_id);


/**
 * @internal      fal_tiger_vlan_trans_mode_set
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
extern yt_ret_t  fal_tiger_vlan_trans_mode_set(yt_unit_t unit, yt_port_t  port, yt_vlan_range_trans_mode_t rangeMode);


/**
 * @internal      fal_tiger_vlan_trans_mode_get
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
extern yt_ret_t  fal_tiger_vlan_trans_mode_get(yt_unit_t unit, yt_port_t  port, yt_vlan_range_trans_mode_t *pRangeMode);


/**
 * @internal      fal_tiger_vlan_igr_trans_table_add
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     pRuleTbl            -x
 * @param[in]     pAction             -x
 * @param[in]     pEntry_id           -vlan translate table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_igr_trans_table_add(yt_unit_t unit, const yt_vlan_trans_tbl_t *pRuleTbl,  const yt_vlan_trans_action_tbl_t *pAction, yt_trans_tbl_id_t *pEntry_id);


/**
 * @internal      fal_tiger_vlan_igr_trans_table_get
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
extern yt_ret_t  fal_tiger_vlan_igr_trans_table_get(yt_unit_t unit,  yt_trans_tbl_id_t entry_id, yt_vlan_trans_tbl_t *pRuleTbl,  yt_vlan_trans_action_tbl_t *pAction);


/**
 * @internal      fal_tiger_vlan_igr_trans_table_del
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     entry_id            -vlan translate table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_igr_trans_table_del(yt_unit_t unit, yt_trans_tbl_id_t entry_id);


/**
 * @internal      fal_tiger_vlan_egr_trans_table_add
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     pRuleTbl            -x
 * @param[in]     pAction             -x
 * @param[in]     pEntry_id           -vlan translate table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_vlan_egr_trans_table_add(yt_unit_t unit, const yt_egr_vlan_trans_tbl_t  *pRuleTbl, const yt_egr_vlan_trans_action_tbl_t *pAction, yt_trans_tbl_id_t *pEntry_id);


/**
 * @internal      fal_tiger_vlan_egr_trans_table_get
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
extern yt_ret_t fal_tiger_vlan_egr_trans_table_get(yt_unit_t unit, yt_trans_tbl_id_t entry_idx, yt_egr_vlan_trans_tbl_t  *pRuleTbl, yt_egr_vlan_trans_action_tbl_t *pAction);


/**
 * @internal      fal_tiger_vlan_egr_trans_table_del
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     entry_idx           -vlan translate table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_egr_trans_table_del(yt_unit_t unit, yt_trans_tbl_id_t entry_idx);

extern yt_ret_t  fal_tiger_vlan_igr_trans_prio_tagPvidIgnore_set(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t  port,  yt_enable_t enable);
extern yt_ret_t  fal_tiger_vlan_igr_trans_prio_tagPvidIgnore_get(yt_unit_t unit, yt_vlan_type_t  type,  yt_port_t  port,  yt_enable_t *pEnable);
extern yt_ret_t  fal_tiger_vlan_igr_trans_lookup_miss_ctrl_set(yt_unit_t unit, yt_port_t  port, yt_vlan_type_t  type,  yt_act_type_t missAction);
extern yt_ret_t  fal_tiger_vlan_igr_trans_lookup_miss_ctrl_get(yt_unit_t unit, yt_port_t  port, yt_vlan_type_t  type,  yt_act_type_t *pMissAction);
extern yt_ret_t  fal_tiger_vlan_igr_trans_table_add_by_index(yt_unit_t unit, yt_trans_tbl_id_t tableIdx, const yt_vlan_trans_tbl_t *pRuleTbl,  const yt_vlan_trans_action_tbl_t *pAction);
extern yt_ret_t  fal_tiger_vlan_egr_trans_lookup_miss_ctrl_set(yt_unit_t unit, yt_port_t  port, yt_vlan_type_t  type,  yt_vlan_egrtrans_lookup_miss_act_t missAction);
extern yt_ret_t  fal_tiger_vlan_egr_trans_lookup_miss_ctrl_get(yt_unit_t unit, yt_port_t  port, yt_vlan_type_t  type,  yt_vlan_egrtrans_lookup_miss_act_t *pMissAction);
extern yt_ret_t  fal_tiger_vlan_egr_trans_rangeProfile_add(yt_unit_t unit, yt_profile_id_t profileId, const yt_vlan_range_group_t *pVlanRange);
extern yt_ret_t  fal_tiger_vlan_egr_trans_rangeProfile_get(yt_unit_t unit, yt_profile_id_t profileId, yt_vlan_range_group_t *pVlanRange);
extern yt_ret_t  fal_tiger_vlan_egr_trans_rangeProfile_del(yt_unit_t unit, yt_profile_id_t profileId);
extern yt_ret_t  fal_tiger_vlan_egr_trans_table_add_by_index(yt_unit_t unit, yt_trans_tbl_id_t tableIdx, const yt_egr_vlan_trans_tbl_t  *pRuleTbl, const yt_egr_vlan_trans_action_tbl_t *pAction);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
