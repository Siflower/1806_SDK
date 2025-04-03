/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_stat.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_stat.h"
#include "fal_dispatch.h"


/**
 * @internal      yt_stat_mib_init
 * @endinternal
 *
 * @brief         initial mib function and lock
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_stat_mib_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->stat_mib_init(unit);
}

/**
 * @internal      yt_stat_mib_enable_set
 * @endinternal
 *
 * @brief         enable mib function
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_stat_mib_enable_set (yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->stat_mib_enable_set(unit, enable);
}

/**
 * @internal      yt_stat_mib_enable_get
 * @endinternal
 *
 * @brief         get mib enable value
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_stat_mib_enable_get (yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->stat_mib_enable_get(unit, pEnable);
}

/**
 * @internal      yt_stat_mib_clear
 * @endinternal
 *
 * @brief         clear one port mib data
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_stat_mib_clear (yt_unit_t unit, yt_port_t port)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->stat_mib_clear(unit, port);
}

/**
 * @internal      yt_stat_mib_clear_all
 * @endinternal
 *
 * @brief         clear all ports mib data
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_stat_mib_clear_all (yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->stat_mib_clear_all(unit);
}

/**
 * @internal      yt_stat_mib_port_get
 * @endinternal
 *
 * @brief         get one port all mib info
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pCnt                -mib info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_stat_mib_port_get (yt_unit_t unit, yt_port_t port, yt_stat_mib_port_cnt_t *pCnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pCnt), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->stat_mib_port_get(unit, port, pCnt);
}

/**
 * @internal      yt_stat_flow_enable_set
 * @endinternal
 *
 * @brief         enable flow function with flow id
 * @param[in]     unit                -unit id
 * @param[in]     flow_id             -flow  id
 * @param[in]     enable              -enable value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_stat_flow_enable_set (yt_unit_t unit, uint32_t flow_id, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->stat_flow_enable_set(unit, flow_id, enable);
}

/**
 * @internal      yt_stat_flow_enable_get
 * @endinternal
 *
 * @brief         get flow function with flow id enable value
 * @param[in]     unit                -unit id
 * @param[in]     flow_id             -flow  id
 * @param[in]     pEnable             -enable value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_stat_flow_enable_get (yt_unit_t unit, uint32_t flow_id, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->stat_flow_enable_get(unit, flow_id, pEnable);
}

/**
 * @internal      yt_stat_flow_mode_set
 * @endinternal
 *
 * @brief         set flow id type and mode
 * @param[in]     unit                -unit id
 * @param[in]     flow_id             -flow id
 * @param[in]     type                -type value for port base or flow base
 * @param[in]     mode                -mode for byte or pkt
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_stat_flow_mode_set (yt_unit_t unit, uint32_t flow_id, yt_stat_type_t type, yt_stat_mode_t mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_STATE_TYPE_PORT < type || YT_STATE_TYPE_FLOW > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_STATE_MODE_PACKET< mode || YT_STATE_MODE_BYTE > mode), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->stat_flow_mode_set(unit, flow_id, type, mode);
}

/**
 * @internal      yt_stat_flow_mode_get
 * @endinternal
 *
 * @brief         get flow id type and mode
 * @param[in]     unit                -unit id
 * @param[in]     flow_id             -flow id
 * @param[in]     pType                -type value for port base or flow base
 * @param[in]     pMode                -mode for byte or pkt
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_stat_flow_mode_get (yt_unit_t unit, uint32_t flow_id, yt_stat_type_t *pType, yt_stat_mode_t *pMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pType), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->stat_flow_mode_get(unit, flow_id, pType, pMode);
}

/**
 * @internal      yt_stat_flow_count_set
 * @endinternal
 *
 * @brief         set flow count for clear count
 * @param[in]     unit                -unit id
 * @param[in]     flow_id             -flow id
 * @param[in]     cnt               -counter value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_stat_flow_count_set (yt_unit_t unit, uint32_t flow_id, uint64 cnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->stat_flow_count_set(unit, flow_id, cnt);
}

/**
 * @internal      yt_stat_flow_count_get
 * @endinternal
 *
 * @brief         get flow counter value
 * @param[in]     unit                -unit id
 * @param[in]     flow_id             -flow id
 * @param[out]    pCnt               -count value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_stat_flow_count_get (yt_unit_t unit, uint32_t flow_id, uint64 *pCnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pCnt), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->stat_flow_count_get(unit, flow_id, pCnt);
}
