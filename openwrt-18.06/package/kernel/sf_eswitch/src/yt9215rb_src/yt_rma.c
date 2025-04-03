/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_rma.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_rma.h"
#include "fal_dispatch.h"


/**
 * @internal      yt_rma_action_set
 * @endinternal
 *
 * @brief         set action of specific reserved multicast address
 * @param[in]     unit                -unit id
 * @param[in]     da               -the last byte of rma mac address,refer to yt_rma_action_t
 * @param[in]     action              -rma packet action,refer to yt_rma_action_t
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_rma_action_set(yt_unit_t unit, yt_rma_da_t  da, yt_rma_action_t action)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->rma_action_set(unit, da, action);
}

/**
 * @internal      yt_rma_action_get
 * @endinternal
 *
 * @brief         get action of specific reserved multicast address
 * @param[in]     unit                -unit id
 * @param[in]     da               -the last byte of rma mac address,refer to yt_rma_action_t
 * @param[out]    pAction             -rma packet action,refer to yt_rma_action_
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_rma_action_get(yt_unit_t unit, yt_rma_da_t  da, yt_rma_action_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rma_action_get(unit, da, pAction);
}

/**
 * @internal      yt_rma_bypass_port_isolation_set
 * @endinternal
 *
 * @brief         set rma passthrough port isolation or not
 * @param[in]     unit                -unit id
 * @param[in]     da               -the last byte of rma mac address,refer to yt_rma_action_t
 * @param[in]     enable                  -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_rma_bypass_port_isolation_set(yt_unit_t unit, yt_rma_da_t  da, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rma_bypass_port_isolation_set(unit, da, enable);
}

/**
 * @internal      yt_rma_bypass_port_isolation_get
 * @endinternal
 *
 * @brief         get the setting of rma passthrough port isolation
 * @param[in]     unit                -unit id
 * @param[in]     da               -the last byte of rma mac address,refer to yt_rma_action_t
 * @param[out]    pEnable                 -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_rma_bypass_port_isolation_get(yt_unit_t unit, yt_rma_da_t  da, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rma_bypass_port_isolation_get(unit, da, pEnable);
}

/**
 * @internal      yt_rma_bypass_vlan_filter_set
 * @endinternal
 *
 * @brief         set rma passthrough vlan igress filter or not
 * @param[in]     unit                -unit id
 * @param[in]     da               -the last byte of rma mac address,refer to yt_rma_action_t
 * @param[in]     enable                  -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_rma_bypass_vlan_filter_set(yt_unit_t unit, yt_rma_da_t da, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rma_bypass_vlan_filter_set(unit, da, enable);
}

/**
 * @internal      yt_rma_bypass_vlan_filter_get
 * @endinternal
 *
 * @brief         get the setting of rma passthrough vlan igress filter
 * @param[in]     unit                -unit id
 * @param[in]     da               -the last byte of rma mac address,refer to yt_rma_action_t
 * @param[out]    pEnable                 -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_rma_bypass_vlan_filter_get(yt_unit_t unit, yt_rma_da_t da, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rma_bypass_vlan_filter_get(unit, da, pEnable);
}
