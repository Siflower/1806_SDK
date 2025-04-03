/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_sensor.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_sensor.h"
#include "fal_dispatch.h"


/**
 * @internal      yt_sensor_temp_enable_set
 * @endinternal
 *
 * @brief         enable or disable temperature sensor
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_sensor_temp_enable_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->sensor_temp_enable_set(unit, enable);
}

/**
 * @internal      yt_sensor_temp_enable_get
 * @endinternal
 *
 * @brief         get enable status of temperature sensor
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_sensor_temp_enable_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->sensor_temp_enable_get(unit, pEnable);
}

/**
 * @internal      yt_sensor_temp_value_get
 * @endinternal
 *
 * @brief         get temperature value,in centigrade*100 unit.if negative is true,temp=-absolutevalue/100 centigrade
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pIsNegative         -negative flag
 * @param[out]    pAbsoluteValue      -absolute value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_sensor_temp_value_get(yt_unit_t unit, yt_bool_t *pIsNegative, uint16_t *pAbsoluteValue)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pIsNegative), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pAbsoluteValue), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->sensor_temp_value_get(unit, pIsNegative, pAbsoluteValue);
}
