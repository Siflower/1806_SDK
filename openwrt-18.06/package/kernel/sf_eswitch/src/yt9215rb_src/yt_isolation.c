/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_isolation.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_isolation.h"
#include "fal_dispatch.h"


/**
 * @internal      yt_isolation_port_set
 * @endinternal
 *
 * @brief         set allowed dest port mask of the port
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     iso_portmask        -the allowed dest portmask of specific port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_isolation_port_set(yt_unit_t unit, yt_port_t port, yt_port_mask_t isoPortMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit,isoPortMask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->isolation_port_set(unit, port, isoPortMask);
}

/**
 * @internal      yt_isolation_port_get
 * @endinternal
 *
 * @brief         get allowed dest port mask of the port
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pIsoPortMask       -the allowed dest portmask of specific port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_isolation_port_get(yt_unit_t unit, yt_port_t port, yt_port_mask_t *pIsoPortMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pIsoPortMask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->isolation_port_get(unit, port, pIsoPortMask);
}

/**
 * @internal      yt_isolation_vlan_add
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     isoInfo             -vlan isolation info(min vid/max vid/isolation member port)
 * @param[out]    idx                 -the index of the vlan isolation entry, idx start at 0
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_isolation_vlan_add(yt_unit_t unit, yt_iso_vlan_info_t isoInfo, uint16_t *idx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, isoInfo.iso_msk))), CMM_ERR_PORTLIST);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < isoInfo.min_vid || isoInfo.min_vid <= YT_VLAN_ID_MIN), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < isoInfo.max_vid || isoInfo.max_vid <= YT_VLAN_ID_MIN), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(isoInfo.min_vid > isoInfo.max_vid, CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == idx), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->isolation_vlan_add(unit, isoInfo, idx);
}

/**
 * @internal      yt_isolation_vlan_del_by_index
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -the index of the vlan isolation entry, idx start at 0
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_isolation_vlan_del_by_index(yt_unit_t unit, uint16_t idx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->isolation_vlan_del_by_index(unit, idx);
}

/**
 * @internal      yt_isolation_vlan_set_by_index
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -the index of the vlan isolation entry, idx start at 0
 * @param[in]     isoInfo             -vlan isolation info(min vid/max vid/isolation member port)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_isolation_vlan_set_by_index(yt_unit_t unit, uint16_t idx, yt_iso_vlan_info_t isoInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, isoInfo.iso_msk))), CMM_ERR_PORTLIST);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < isoInfo.min_vid || isoInfo.min_vid <= YT_VLAN_ID_MIN), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < isoInfo.max_vid || isoInfo.max_vid <= YT_VLAN_ID_MIN), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(isoInfo.min_vid > isoInfo.max_vid, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->isolation_vlan_set_by_index(unit, idx, isoInfo);
}

/**
 * @internal      yt_isolation_vlan_get_by_index
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -the index of the vlan isolation entry, idx start at 0
 * @param[out]    isoInfo             -vlan isolation info(min vid/max vid/isolation member port)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_isolation_vlan_get_by_index(yt_unit_t unit, uint16_t idx, yt_iso_vlan_info_t *pIsoInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(NULL == pIsoInfo, CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->isolation_vlan_get_by_index(unit, idx, pIsoInfo);
}

/**
 * @internal      yt_isolation_vlan_type_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     type                -the type of VLAN isolation
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_isolation_vlan_type_set(yt_unit_t unit, yt_iso_vlan_type_t type)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((type > YT_ISO_VLAN_TYPE_SVID || type < YT_ISO_VLAN_TYPE_FVID), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->isolation_vlan_type_set(unit, type);
}

/**
 * @internal      yt_isolation_vlan_type_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[out]    pType               -the type of VLAN isolation
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_isolation_vlan_type_get(yt_unit_t unit, yt_iso_vlan_type_t *pType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == YT_DISPATCH(unit)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pType), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->isolation_vlan_type_get(unit, pType);
}

