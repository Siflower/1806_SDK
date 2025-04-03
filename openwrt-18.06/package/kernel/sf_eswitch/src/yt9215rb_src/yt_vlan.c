/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_vlan.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_vlan.h"
#include "fal_dispatch.h"


/**
 * @internal      yt_vlan_init
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->vlan_init(unit);
}

/**
 * @internal      yt_vlan_port_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     member_portmask           -member port bit mask
 * @param[in]     untag_portmask           -untag member port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_port_set(yt_unit_t unit,  yt_vlan_t vid,  yt_port_mask_t  member_portmask, yt_port_mask_t  untag_portmask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, member_portmask))), CMM_ERR_PORTLIST);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, untag_portmask))), CMM_ERR_PORTLIST);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_port_set(unit, vid, member_portmask, untag_portmask);
}

/**
 * @internal      yt_vlan_port_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[out]    pMember_portmask          -member port bit mask
 * @param[in]     pUntag_portmask           -untag member port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_port_get(yt_unit_t unit,  yt_vlan_t vid,  yt_port_mask_t  *pMember_portmask, yt_port_mask_t  *pUntag_portmask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pMember_portmask), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pUntag_portmask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_get(unit, vid, pMember_portmask, pUntag_portmask);
}

/**
 * @internal      yt_vlan_svlMode_enable_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     enable                -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_svlMode_enable_set(yt_unit_t unit, yt_vlan_t vid,  yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_svlMode_enable_set(unit, vid, enable);
}

/**
 * @internal      yt_vlan_svlMode_enable_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     pEnable                -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_svlMode_enable_get(yt_unit_t unit, yt_vlan_t vid,  yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_svlMode_enable_get(unit, vid, pEnable);
}

/**
 * @internal      yt_vlan_fid_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     fid                 -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_fid_set(yt_unit_t unit, yt_vlan_t vid,  yt_fid_t fid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_fid_set(unit, vid, fid);
}

/**
 * @internal      yt_vlan_fid_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[out]    pFid                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_fid_get(yt_unit_t unit, yt_vlan_t vid,  yt_fid_t *pFid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pFid), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_fid_get(unit, vid, pFid);
}


/**
 * @internal      yt_vlan_igrTpid_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     tpid                -yt_vlan.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_igrTpid_set(yt_unit_t unit,  yt_tpid_profiles_t tpid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->vlan_igrTpid_set(unit, tpid);
}

/**
 * @internal      yt_vlan_igrTpid_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pTpid               -yt_vlan.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_igrTpid_get(yt_unit_t unit,  yt_tpid_profiles_t *pTpid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pTpid), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_igrTpid_get(unit, pTpid);
}

/**
 * @internal      yt_vlan_port_igrTpidSel_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     tpidIdxMask         -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_port_igrTpidSel_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_tpidprofile_id_mask_t tpidIdxMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((VLAN_TYPE_SVLAN < type || VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_port_igrTpidSel_set(unit, type, port, tpidIdxMask);
}

/**
 * @internal      yt_vlan_port_igrTpidSel_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pTpidIdxMask        -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_port_igrTpidSel_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_tpidprofile_id_mask_t *pTpidIdxMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((VLAN_TYPE_SVLAN < type || VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pTpidIdxMask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_igrTpidSel_get(unit, type, port, pTpidIdxMask);
}

/**
 * @internal      yt_vlan_port_igrPvid_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     vid                 -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_port_igrPvid_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t vid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((VLAN_TYPE_SVLAN < type || VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_port_igrDefVlan_set(unit, type, port, vid);
}

/**
 * @internal      yt_vlan_port_igrPvid_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pVid                -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_port_igrPvid_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t *pVid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((VLAN_TYPE_SVLAN < type || VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pVid), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_igrDefVlan_get(unit, type, port, pVid);
}

/**
 * @internal      yt_vlan_port_igrFilter_enable_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_port_igrFilter_enable_set(yt_unit_t unit, yt_port_t  port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_port_igrFilter_enable_set(unit, port, enable);
}

/**
 * @internal      yt_vlan_port_igrFilter_enable_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable            -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_port_igrFilter_enable_get(yt_unit_t unit, yt_port_t  port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_igrFilter_enable_get(unit, port, pEnable);
}

/**
 * @internal      yt_vlan_igrTransparent_set
 * @endinternal
 *
 * @brief         Set ingress vlan transparent,packet from port will bypass egress vlan filter on the ports specified by port_mask
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -ignress port num
 * @param[in]     port_mask           -egress port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_igrTransparent_set(yt_unit_t unit, yt_port_t  port, yt_port_mask_t port_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, port_mask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->vlan_igrTransparent_set(unit, port, port_mask);
}

/**
 * @internal      yt_vlan_igrTransparent_get
 * @endinternal
 *
 * @brief         Get ingress vlan transparent setting by ingress port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -ignress port num
 * @param[out]    pPort_mask          -egress port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_igrTransparent_get(yt_unit_t unit, yt_port_t  port, yt_port_mask_t *pPort_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pPort_mask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_igrTransparent_get(unit, port, pPort_mask);
}

/**
 * @internal      yt_vlan_port_aft_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     tagAFT              -yt_vlan.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_port_aft_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, yt_vlan_aft_t aft)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((VLAN_TYPE_SVLAN < type || VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((VLAN_AFT_UNTAGGED < aft || VLAN_AFT_ALL > aft), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_port_aft_set(unit, type, port, aft);
}

/**
 * @internal      yt_vlan_port_aft_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pTagAFT             -yt_vlan.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_port_aft_get(yt_unit_t unit, yt_vlan_type_t type, yt_port_t port, yt_vlan_aft_t *pAft)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((VLAN_TYPE_SVLAN < type || VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pAft), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_aft_get(unit, type, port, pAft);
}

/**
 * @internal      yt_vlan_port_egrTagMode_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     tagMode             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_vlan_port_egrTagMode_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port,  yt_egr_tag_mode_t tagMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((VLAN_TYPE_SVLAN < type || VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((VLAN_TAG_MODE_ENTRY_BASED < tagMode || VLAN_TAG_MODE_UNTAGGED > tagMode), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_port_egrTagMode_set(unit, type, port, tagMode);
}

/**
 * @internal      yt_vlan_port_egrTagMode_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pTagMode            -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_vlan_port_egrTagMode_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port,  yt_egr_tag_mode_t *pTagMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((VLAN_TYPE_SVLAN < type || VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pTagMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_egrTagMode_get(unit, type, port, pTagMode);
}

/**
 * @internal      yt_vlan_port_egrDefVid_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     default_vid         -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_vlan_port_egrDefVid_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t default_vid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((VLAN_TYPE_SVLAN < type || VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < default_vid), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_port_egrDefVid_set(unit, type, port, default_vid);
}

/**
 * @internal      yt_vlan_port_egrDefVid_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pDefault_vid        -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_vlan_port_egrDefVid_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t *pDefault_vid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((VLAN_TYPE_SVLAN < type || VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pDefault_vid), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_egrDefVid_get(unit, type, port, pDefault_vid);
}

/**
 * @internal      yt_vlan_egrTpid_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     tpids               -yt_vlan.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_vlan_egrTpid_set(yt_unit_t unit,  yt_tpid_profiles_t tpids)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->vlan_egrTpid_set(unit, tpids);
}

/**
 * @internal      yt_vlan_egrTpid_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pTpids              -yt_vlan.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_vlan_egrTpid_get(yt_unit_t unit,  yt_tpid_profiles_t *pTpids)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pTpids), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_egrTpid_get(unit, pTpids);
}

/**
 * @internal      yt_vlan_port_egrTpidSel_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     tpidIdx             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_vlan_port_egrTpidSel_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, uint8_t tpidIdx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((VLAN_TYPE_SVLAN < type || VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_TPID_PROFILE_NUM <= tpidIdx), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_port_egrTpidSel_set(unit, type, port, tpidIdx);
}

/**
 * @internal      yt_vlan_port_egrTpidSel_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pTpidIdx            -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_vlan_port_egrTpidSel_get(yt_unit_t unit, yt_vlan_type_t type, yt_port_t port, uint8_t *pTpidIdx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((VLAN_TYPE_SVLAN < type || VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pTpidIdx), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_egrTpidSel_get(unit, type, port, pTpidIdx);
}

/**
 * @internal      yt_vlan_port_egrTransparent_set
 * @endinternal
 *
 * @brief         Set vlan egress transparent state for per port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -egress port num
 * @param[in]     enable              -enable or disable
 * @param[in]     port_mask           -ignress port bit mask to transparent
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_vlan_port_egrTransparent_set(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t port,  yt_enable_t enable, yt_port_mask_t port_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, port_mask))), CMM_ERR_PORTLIST);
    CMM_PARAM_CHK((VLAN_TYPE_SVLAN < type || VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_port_egrTransparent_set(unit, type, port, enable, port_mask);
}

/**
 * @internal      yt_vlan_port_egrTransparent_get
 * @endinternal
 *
 * @brief         Get vlan egress transparent state for per port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -egress port num
 * @param[out]    pEnable             -enable or disable
 * @param[out]    pPort_mask          -ignress port bit mask to transparent
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_vlan_port_egrTransparent_get(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t port,  yt_enable_t *pEnable, yt_port_mask_t *pPort_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((VLAN_TYPE_SVLAN < type || VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pPort_mask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_egrTransparent_get(unit, type, port, pEnable, pPort_mask);
}

/**
 * @internal      yt_vlan_port_egrFilter_enable_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_port_egrFilter_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_port_egrFilter_enable_set(unit, port, enable);
}

/**
 * @internal      yt_vlan_port_egrFilter_enable_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable            -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_port_egrFilter_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_egrFilter_enable_get(unit, port, pEnable);
}

/**
 * @internal      yt_vlan_port_vidTypeSel_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_port_vidTypeSel_set(yt_unit_t unit, yt_port_t port, yt_vlan_type_t type)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((VLAN_TYPE_SVLAN < type || VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_port_vidTypeSel_set(unit, port, type);
}

/**
 * @internal      yt_vlan_port_vidTypeSel_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pType               -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_vlan_port_vidTypeSel_get(yt_unit_t unit, yt_port_t port, yt_vlan_type_t *pType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pType), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_vidTypeSel_get(unit, port, pType);
}
