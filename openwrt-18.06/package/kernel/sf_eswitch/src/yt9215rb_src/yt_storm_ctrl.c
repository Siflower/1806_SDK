/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_storm_ctrl.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_storm_ctrl.h"
#include "fal_dispatch.h"


/**
 * @internal      yt_storm_ctrl_init
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_storm_ctrl_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->storm_ctrl_init(unit);
}

/**
 * @internal      yt_storm_ctrl_enable_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     storm_type          -storm type,refer to yt_storm_type_t
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_storm_ctrl_enable_set(yt_unit_t unit, yt_port_t port, yt_storm_type_t storm_type, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->storm_ctrl_enable_set(unit, port, storm_type, enable);
}

/**
 * @internal      yt_storm_ctrl_enable_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     storm_type          -storm type,refer to yt_storm_type_t
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_storm_ctrl_enable_get(yt_unit_t unit, yt_port_t port, yt_storm_type_t storm_type, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->storm_ctrl_enable_get(unit, port, storm_type, pEnable);
}

/**
 * @internal      yt_storm_ctrl_rate_mode_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     storm_type          -storm type,refer to yt_storm_type_t
 * @param[in]     rate_mode           -storm rate mode,byte or packet
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_storm_ctrl_rate_mode_set(yt_unit_t unit, yt_port_t port, yt_storm_type_t storm_type, yt_storm_rate_mode_t rate_mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((STORM_RATE_MODE_PACKET< rate_mode || STORM_RATE_MODE_BYTE > rate_mode), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->storm_ctrl_rate_mode_set(unit, port, storm_type, rate_mode);
}

/**
 * @internal      yt_storm_ctrl_rate_mode_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     storm_type          -storm type,refer to yt_storm_type_t
 * @param[out]    pRate_mode         -storm rate mode,byte or packet
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_storm_ctrl_rate_mode_get(yt_unit_t unit, yt_port_t port, yt_storm_type_t storm_type, yt_storm_rate_mode_t *pRate_mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pRate_mode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->storm_ctrl_rate_mode_get(unit, port, storm_type, pRate_mode);
}

/**
 * @internal      yt_storm_ctrl_rate_include_gap_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     storm_type          -storm type,refer to yt_storm_type_t
 * @param[in]     inc_gap             -storm rate include or exclude packet gap
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_storm_ctrl_rate_include_gap_set(yt_unit_t unit, yt_port_t port, yt_storm_type_t storm_type, yt_storm_rate_gap_t inc_gap)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((STORM_RATE_GAP_INCLUDE < inc_gap || STORM_RATE_GAP_EXCLUDE > inc_gap), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->storm_ctrl_rate_include_gap_set(unit, port, storm_type, inc_gap);
}

/**
 * @internal      yt_storm_ctrl_rate_include_gap_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     storm_type          -storm type,refer to yt_storm_type_t
 * @param[out]    pInc_gap           -storm rate include or exclude packet gap
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_storm_ctrl_rate_include_gap_get(yt_unit_t unit, yt_port_t port, yt_storm_type_t storm_type, yt_storm_rate_gap_t *pInc_gap)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pInc_gap), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->storm_ctrl_rate_include_gap_get(unit, port, storm_type, pInc_gap);
}

/**
 * @internal      yt_storm_ctrl_rate_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     storm_type          -storm type,refer to yt_storm_type_t
 * @param[in]     rate                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_storm_ctrl_rate_set(yt_unit_t unit, yt_port_t port, yt_storm_type_t storm_type, uint32_t rate)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->storm_ctrl_rate_set(unit, port, storm_type, rate);
}

/**
 * @internal      yt_storm_ctrl_rate_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     storm_type          -storm type,refer to yt_storm_type_t
 * @param[out]    pRate               -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_storm_ctrl_rate_get(yt_unit_t unit, yt_port_t port, yt_storm_type_t storm_type, uint32_t *pRate)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pRate), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->storm_ctrl_rate_get(unit, port, storm_type, pRate);
}
