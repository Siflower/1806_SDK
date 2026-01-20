/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_vlan_translate.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_VLAN_TRANSLATE_H__
#define __YT_VLAN_TRANSLATE_H__


#include "yt_cmm.h"
#include "yt_vlan.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(SWITCH_SERIES_TIGER)
#define YT_VLAN_IGRTRANS_PROFILE_MAX_NUM      (9U)
#endif
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
#define YT_VLAN_IGRTRANS_PROFILE_MAX_NUM      (16U)
#endif
#define YT_VLAN_EGRTRANS_PROFILE_MAX_NUM      (16U)


typedef enum yt_vlan_trans_action_e
{
    YT_VLAN_TRANS_ACTION_NO_CHANGE,  /*no action*/
    YT_VLAN_TRANS_ACTION_ADD,        /*add assign vid if untag*/
    YT_VLAN_TRANS_ACTION_REPLACE,/*replace original vid with assgin vid*/
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    YT_VLAN_TRANS_ACTION_COPY,/*copy svlan to cvlan or copy cvlan to svlan*/
#endif
    YT_VLAN_TRANS_ACTION_MAX
}yt_vlan_trans_action_t;

typedef enum yt_prio_trans_action_e
{
    YT_VLAN_PRIO_ACT_NO_CHANGE,  /*no action*/
    YT_VLAN_PRIO_ACT_REPLACE, /*replace original priority with assgin priority*/
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    YT_VLAN_PRIO_ACT_COPY, /*copy spri to cpri or copy cpri to spri*/
#endif
    YT_VLAN_PRIO_ACT_MAX
}yt_prio_trans_action_t;

typedef enum yt_vlan_range_trans_mode_e
{
    YT_VLAN_RANGE_TRANS_MODE_CVLAN,
    YT_VLAN_RANGE_TRANS_MODE_SVLAN,
}yt_vlan_range_trans_mode_t;

typedef enum yt_vlan_match_type_e
{
    YT_VLAN_MATCH_TYPE_VID,
    YT_VLAN_MATCH_TYPE_RANGE,
}yt_vlan_match_type_t;

typedef enum yt_vlan_egrtrans_lookup_miss_act_e 
{
    YT_VLAN_EGRTRANS_LOOKUP_MISS_FWD,
    YT_VLAN_EGRTRANS_LOOKUP_MISS_DROP,
}yt_vlan_egrtrans_lookup_miss_act_t;

typedef enum yt_vlan_format_e
{
    YT_VLAN_FMT_UNTAGGED,
    YT_VLAN_FMT_PRIO_TAGGED,
    YT_VLAN_FMT_TAGGED,
}yt_vlan_format_t;

typedef struct yt_vlan_range_group_e
{
#if defined(SWITCH_SERIES_TIGER)
    uint16_t vidRange0Min;
    uint16_t vidRange0Max;
    uint16_t vidRange1Min;
    uint16_t vidRange1Max;
    uint16_t vidRange2Min;
    uint16_t vidRange2Max;
    uint16_t vidRange3Min;
    uint16_t vidRange3Max;
#endif
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    uint16_t vidMin;
    uint16_t vidMax;
    yt_vlan_type_t vidType;    /* YT_VLAN_TYPE_CVLAN:just match cvlan; YT_VLAN_TYPE_SVLAN:just match svlan */
#endif
}yt_vlan_range_group_t;

typedef struct yt_vlan_trans_tbl_s
{
    yt_bool_t                 svidNeedCare;    /*false:do not need to care svid   true:need to care svid*/
    yt_bool_t                 cvidNeedCare;    /*false:do not need to care cvid   true:need to care cvid*/
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    yt_profile_id_t           sVidRangeId0;
    yt_profile_id_t           sVidRangeId1;
    yt_profile_id_t           sVidRangeId2;
    uint16_t                  svid;
    yt_vlan_match_type_t svidMatchType;
    yt_profile_id_t           cVidRangeId0;
    yt_profile_id_t           cVidRangeId1;
    yt_profile_id_t           cVidRangeId2;
    uint16_t                  cvid;
    yt_vlan_match_type_t cvidMatchType;
#endif
#if defined(SWITCH_SERIES_TIGER)
    yt_bool_t                 stagFormatNeedCare;
    uint16_t                  svidOrVidRangeId;
    yt_bool_t                 ctagFormatNeedCare;
    uint16_t                  cvidOrVidRangeId;
#endif
    yt_vlan_tag_fmt_t         stagFormat;
    yt_vlan_tag_fmt_t         ctagFormat;
    yt_port_mask_t            srcPortMaskNeedCare;  /* src portmask which need to care */
}yt_vlan_trans_tbl_t;

typedef struct yt_vlan_trans_action_tbl_s
{
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    yt_bool_t      igrVlanFilterBypass;
    yt_act_type_t  fwdAct;
    uint8_t                 assignSpri;
    yt_prio_trans_action_t  spriAction;
    uint8_t                 assignCpri;
    yt_prio_trans_action_t  cpriAction;
#endif
    uint16_t                assignSvid;
    yt_vlan_trans_action_t  svidAction;
    uint16_t                assignCvid;
    yt_vlan_trans_action_t  cvidAction;
}yt_vlan_trans_action_tbl_t;

typedef struct yt_egr_vlan_trans_tbl_s
{
    yt_bool_t                  svidNeedCare;    /*false:do not need to care svid   true:need to care svid*/
    yt_bool_t                  cvidNeedCare;    /*false:do not need to care cvid   true:need to care cvid*/
    yt_bool_t                  mvrNeedCare;    /*if check multicast vlan hit or not*/
#if defined(SWITCH_SERIES_TIGER)
    yt_vlan_range_trans_mode_t vidRangeMode;/*select range check works on CVLAN or SVLAN*/
    uint16_t    vidRangeMin;
    uint16_t    vidRangeMax;
    uint16_t    vid;
    yt_bool_t   originalCtagFormatNeedCare;/*if check original cvlan is tagged*/
    yt_bool_t   originalStagFormatNeedCare;/*if check original svlan is tagged*/
#endif
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    yt_profile_id_t           sVidRangeId0;
    yt_profile_id_t           sVidRangeId1;
    yt_profile_id_t           sVidRangeId2;
    uint16_t                  svid;
    yt_vlan_match_type_t      svidMatchType;
    yt_vlan_tag_fmt_t         stagFormat;
    yt_profile_id_t           cVidRangeId0;
    yt_profile_id_t           cVidRangeId1;
    yt_profile_id_t           cVidRangeId2;
    uint16_t                  cvid;
    yt_vlan_match_type_t      cvidMatchType;
    yt_vlan_tag_fmt_t         ctagFormat;
#endif
    yt_port_mask_t            dstPortMaskNeedCare; /* dst portmask which need to care */
} yt_egr_vlan_trans_tbl_t;

typedef struct yt_egr_vlan_trans_data_s
{
    yt_bool_t     svidEnable;  /*false:do not change svid   true:need to change svid*/
    yt_bool_t     cvidEnable;/*false:do not change cvid   true:need to change cvid*/
    uint16_t      assignSvid;
    uint16_t      assignCvid;
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    yt_bool_t     spriEnable;  /*false:do not change svid   true:need to change svid*/
    yt_bool_t     cpriEnable;/*false:do not change cvid   true:need to change cvid*/
    uint8_t       assignSpri;
    uint8_t       assignCpri;
#endif
} yt_egr_vlan_trans_action_tbl_t;


/**
 * @internal      yt_vlan_igr_trans_untagPvidIgnore_set
 * @endinternal
 *
 * @brief         Set pvid value to cvlan of svlan to match ingress trans table for untag packet
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 */
extern yt_ret_t yt_vlan_igr_trans_untagPvidIgnore_set(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t  port,  yt_enable_t enable);

/**
 * @internal      yt_vlan_igr_trans_untagPvidIgnore_get
 * @endinternal
 *
 * @brief         Get whether pvid value is set to cvlan of svlan to match ingress trans table for untag packet
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t yt_vlan_igr_trans_untagPvidIgnore_get(yt_unit_t unit, yt_vlan_type_t  type,  yt_port_t  port,  yt_enable_t *pEnable);

/**
 * @internal      yt_vlan_igr_trans_prio_tagPvidIgnore_set
 * @endinternal
 *
 * @brief         Set pvid value to cvlan of svlan to match ingress trans table for priority tag packet
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 */
extern yt_ret_t yt_vlan_igr_trans_prio_tagPvidIgnore_set(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t  port,  yt_enable_t enable);

/**
 * @internal      yt_vlan_igr_trans_prio_tagPvidIgnore_get
 * @endinternal
 *
 * @brief         Get whether pvid value is set to cvlan of svlan to match ingress trans table for priority tag packet
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t yt_vlan_igr_trans_prio_tagPvidIgnore_get(yt_unit_t unit, yt_vlan_type_t  type,  yt_port_t  port,  yt_enable_t *pEnable);

/**
 * @internal      yt_vlan_igr_trans_lookup_miss_ctrl_set
 * @endinternal
 *
 * @brief         Set loop miss ctrl for ingress trans table match
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     missAction          -miss drop action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_igr_trans_lookup_miss_ctrl_set(yt_unit_t unit, yt_port_t  port, yt_vlan_type_t  type,  yt_act_type_t missAction);

/**
 * @internal      yt_vlan_igr_trans_lookup_miss_ctrl_get
 * @endinternal
 *
 * @brief         Get loop miss ctrl for ingress trans table match
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[out]    pMissAction         -miss drop action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_vlan_igr_trans_lookup_miss_ctrl_get(yt_unit_t unit, yt_port_t  port, yt_vlan_type_t  type,  yt_act_type_t *pMissAction);

/**
 * @internal      yt_vlan_igr_trans_rangeProfile_add
 * @endinternal
 *
 * @brief         Set vlan range profile for ingress trans table match,it need to be binded to port for Tiger
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     profileId           -ingress vlan profile index,start from 1
 * @param[in]     pVlanRange          -ingress vlan translate profile group info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_igr_trans_rangeProfile_add(yt_unit_t unit, yt_profile_id_t profileId, const yt_vlan_range_group_t *pVlanRange);

/**
 * @internal      yt_vlan_igr_trans_rangeProfile_get
 * @endinternal
 *
 * @brief         Get vlan range profile for ingress trans table match
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     profileId           -ingress vlan profile index,start from 1
 * @param[out]    pVlanRange          -ingress vlan translate profile group info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_igr_trans_rangeProfile_get(yt_unit_t unit, yt_profile_id_t profileId, yt_vlan_range_group_t *pVlanRange);

/**
 * @internal      yt_vlan_igr_trans_rangeProfile_del
 * @endinternal
 *
 * @brief         Delete vlan range profile for ingress trans table match
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     profileId           -ingress vlan profile index,start from 1
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_igr_trans_rangeProfile_del(yt_unit_t unit, yt_profile_id_t profileId);

/**
 * @internal      yt_vlan_igr_trans_port_rangeProfileSel_set
 * @endinternal
 *
 * @brief         Set port to bind vlan range profile
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     profileId           -ingress vlan profile index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_igr_trans_port_rangeProfileSel_set(yt_unit_t unit, yt_port_t  port, yt_profile_id_t profileId);

/**
 * @internal      yt_vlan_igr_trans_port_rangeProfileSel_get
 * @endinternal
 *
 * @brief         Get the bind vlan range profile of port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pProfileId          -ingress vlan profile index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_vlan_igr_trans_port_rangeProfileSel_get(yt_unit_t unit, yt_port_t  port, yt_profile_id_t *pProfileId);

/**
 * @internal      yt_vlan_igr_trans_mode_set
 * @endinternal
 *
 * @brief         Set vlan range profile whether is belong to cvid or svid
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     rangeMode           -range mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 */
extern yt_ret_t  yt_vlan_igr_trans_mode_set(yt_unit_t unit, yt_port_t  port, yt_vlan_range_trans_mode_t rangeMode);

/**
 * @internal      yt_vlan_igr_trans_mode_get
 * @endinternal
 *
 * @brief         Get vlan range profile whether is belong to cvid or svid
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pRangeMode          -range mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_vlan_igr_trans_mode_get(yt_unit_t unit, yt_port_t  port, yt_vlan_range_trans_mode_t *pRangeMode);

/**
 * @internal      yt_vlan_igr_trans_table_add
 * @endinternal
 *
 * @brief         Add ingress vlan translate table and action
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     pRuleTbl            -ingress vlan translate table info
 * @param[in]     pAction             -ingress vlan translate action
 * @param[out]     pTableIdx          -vlan translate table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_TABLE_FULL  -no free table entry
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_PORTLIST    -portlist err
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_igr_trans_table_add(yt_unit_t unit, const yt_vlan_trans_tbl_t *pRuleTbl,  const yt_vlan_trans_action_tbl_t *pAction, yt_trans_tbl_id_t *pTableIdx);

/**
 * @internal      yt_vlan_igr_trans_table_add_by_index
 * @endinternal
 *
 * @brief         Add ingress vlan translate table and action by index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     tableIdx            -vlan translate table index
 * @param[in]     pRuleTbl            -ingress vlan translate table info
 * @param[in]     pAction             -ingress vlan translate action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_TABLE_FULL  -no free table entry
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_PORTLIST    -portlist err
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_igr_trans_table_add_by_index(yt_unit_t unit, yt_trans_tbl_id_t tableIdx, const yt_vlan_trans_tbl_t *pRuleTbl,  const yt_vlan_trans_action_tbl_t *pAction);

/**
 * @internal      yt_vlan_igr_trans_table_get
 * @endinternal
 *
 * @brief         Get ingress vlan translate table and action
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     tableIdx            -vlan translate table index
 * @param[out]    pRuleTbl            -x
 * @param[out]    pAction             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_igr_trans_table_get(yt_unit_t unit,  yt_trans_tbl_id_t tableIdx, yt_vlan_trans_tbl_t *pRuleTbl,  yt_vlan_trans_action_tbl_t *pAction);

/**
 * @internal      yt_vlan_igr_trans_table_del
 * @endinternal
 *
 * @brief         Delete ingress vlan translate table and action
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     tableIdx            -vlan translate table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_igr_trans_table_del(yt_unit_t unit, yt_trans_tbl_id_t tableIdx);

/**
 * @internal      yt_vlan_egr_trans_lookup_miss_ctrl_enable_set
 * @endinternal
 *
 * @brief         Set loop miss ctrl for egress trans table match
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     missAction          -lookup miss action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_egr_trans_lookup_miss_ctrl_set(yt_unit_t unit, yt_port_t  port, yt_vlan_type_t  type,  yt_vlan_egrtrans_lookup_miss_act_t missAction);

/**
 * @internal      yt_vlan_egr_trans_lookup_miss_ctrl_get
 * @endinternal
 *
 * @brief         Get loop miss ctrl for egress trans table match
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[out]    pMissAction         -lookup miss action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_vlan_egr_trans_lookup_miss_ctrl_get(yt_unit_t unit, yt_port_t  port, yt_vlan_type_t  type,  yt_vlan_egrtrans_lookup_miss_act_t *pMissAction);

/**
 * @internal      yt_vlan_egr_trans_rangeProfile_add
 * @endinternal
 *
 * @brief         Set vlan range profile for egress trans table match
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     profileId           -egress vlan profile index
 * @param[in]     pVlanRange          -egress vlan translate profile group info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_egr_trans_rangeProfile_add(yt_unit_t unit, yt_profile_id_t profileId, const yt_vlan_range_group_t *pVlanRange);

/**
 * @internal      yt_vlan_egr_trans_rangeProfile_get
 * @endinternal
 *
 * @brief         Get vlan range profile for egress trans table match
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     profileId           -ingress vlan profile index
 * @param[out]    pVlanRange          -ingress vlan translate profile group info, one of them should bind to the port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_egr_trans_rangeProfile_get(yt_unit_t unit, yt_profile_id_t profileId, yt_vlan_range_group_t *pVlanRange);

/**
 * @internal      yt_vlan_egr_trans_rangeProfile_del
 * @endinternal
 *
 * @brief         Delete vlan range profile for egress trans table match
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     profileId           -ingress vlan profile index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_egr_trans_rangeProfile_del(yt_unit_t unit, yt_profile_id_t profileId);

/**
 * @internal      yt_vlan_egr_trans_table_add
 * @endinternal
 *
 * @brief         Add egress vlan translate table and action
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     pRuleTbl            -egress vlan translate table info
 * @param[in]     pAction             -egress vlan translate action
 * @param[out]    pTableIdx           -vlan translate table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_TABLE_FULL  -no free table entry
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_PORTLIST    -portlist err
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_vlan_egr_trans_table_add(yt_unit_t unit, const yt_egr_vlan_trans_tbl_t  *pRuleTbl, const yt_egr_vlan_trans_action_tbl_t *pAction, yt_trans_tbl_id_t *pTableIdx);

/**
 * @internal      yt_vlan_egr_trans_table_add_by_index
 * @endinternal
 *
 * @brief         Add egress vlan translate table and action by index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     pRuleTbl            -egress vlan translate table info
 * @param[in]     pAction             -egress vlan translate action
 * @param[out]    tableIdx            -vlan translate table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_TABLE_FULL  -no free table entry
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_PORTLIST    -portlist err
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_vlan_egr_trans_table_add_by_index(yt_unit_t unit, yt_trans_tbl_id_t tableIdx, const yt_egr_vlan_trans_tbl_t  *pRuleTbl, const yt_egr_vlan_trans_action_tbl_t *pAction);

/**
 * @internal      yt_vlan_egr_trans_table_get
 * @endinternal
 *
 * @brief         Get egress vlan translate table and action
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     tableIdx            -vlan translate table index
 * @param[out]    pRuleTbl            -rule tbale
 * @param[out]    pAction             -action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_vlan_egr_trans_table_get(yt_unit_t unit, yt_trans_tbl_id_t tableIdx, yt_egr_vlan_trans_tbl_t  *pRuleTbl, yt_egr_vlan_trans_action_tbl_t *pAction);

/**
 * @internal      yt_vlan_egr_trans_table_del
 * @endinternal
 *
 * @brief         Delete egress vlan translate table and action
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     tableIdx            -vlan translate table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_egr_trans_table_del(yt_unit_t unit, yt_trans_tbl_id_t tableIdx);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
