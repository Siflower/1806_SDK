/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_led.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_led.h"
#include "fal_dispatch.h"

/**
 * @internal      yt_led_enable
 * @endinternal
 *
 * @brief         enabel LED
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_enable(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->led_enable(unit);
}

/*
 * Function Declaration
 */
/**
 * @internal      yt_led_mode_set
 * @endinternal
 *
 * @brief         select the mode of led
 * @param[in]     unit                -unit id
 * @param[in]     mode                -serial or parallel mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_mode_set(yt_unit_t unit, yt_led_mode_t mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->led_mode_set(unit, mode);
}

/**
 * @internal      yt_led_mode_get
 * @endinternal
 *
 * @brief         get the mode of led
 * @param[in]     unit                -unit id
 * @param[out]    pmode               -serial or parallel mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_mode_get(yt_unit_t unit, yt_led_mode_t *pMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->led_mode_get(unit, pMode);
}

/**
 * @internal      yt_led_action_set
 * @endinternal
 *
 * @brief         select the action of LED0~2 per port
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[in]     ledActCfg           -config of led action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_action_set(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_act_cfg_t ledActCfg)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->led_action_set(unit, port, ledId, ledActCfg);
}

/**
 * @internal      yt_led_action_get
 * @endinternal
 *
 * @brief         get the action of LED0~2 per port
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[out]    pLedActCfg          -config of led action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_action_get(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_act_cfg_t *pLedActCfg)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pLedActCfg), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->led_action_get(unit, port, ledId, pLedActCfg);
}

/**
 * @internal      yt_led_mode_set
 * @endinternal
 *
 * @brief         select the frequency of blink
 * @param[in]     unit                -unit id
 * @param[in]     event               -event associated with blink
 * @param[in]     freq                -frequency of blink
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_blink_freq_set(yt_unit_t unit, yt_port_t port, yt_led_blink_event_t event, yt_led_blink_freq_t freq)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->led_blink_freq_set(unit, port, event, freq);
}

/**
 * @internal      yt_led_blink_freq_get
 * @endinternal
 *
 * @brief         get the frequency of blink
 * @param[in]     unit                -unit id
 * @param[in]     event               -event associated with blink
 * @param[out]    pFreq               -frequency of blink
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_blink_freq_get(yt_unit_t unit, yt_port_t port, yt_led_blink_event_t event, yt_led_blink_freq_t *pFreq)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pFreq), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->led_blink_freq_get(unit, port, event, pFreq);
}

/**
 * @internal      yt_led_blink_duty_set
 * @endinternal
 *
 * @brief         select the duty of blink
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     duty                -duty of blink
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_blink_duty_set(yt_unit_t unit, yt_port_t port, yt_led_blink_duty_t duty)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->led_blink_duty_set(unit, port, duty);
}

/**
 * @internal      yt_led_blink_duty_get
 * @endinternal
 *
 * @brief         get the duty of blink
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pDuty               -duty of blink
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_blink_duty_get(yt_unit_t unit, yt_port_t port, yt_led_blink_duty_t *pDuty)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pDuty), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->led_blink_duty_get(unit, port, pDuty);
}

/**
 * @internal      yt_led_loopdetect_blink_rate_set
 * @endinternal
 *
 * @brief         select blinking rate of loopdetect
 * @param[in]     unit                -unit id
 * @param[in]     rate                -blinking rate
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_loopdetect_blink_rate_set(yt_unit_t unit, yt_led_loopdetect_blink_rate_t rate)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->led_loopdetect_blink_rate_set(unit, rate);
}

/**
 * @internal      yt_led_loopdetect_blink_rate_get
 * @endinternal
 *
 * @brief         get blinking rate of loopdetect
 * @param[in]     unit                -unit id
 * @param[out]    pRate               -blinking rate
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_loopdetect_blink_rate_get(yt_unit_t unit, yt_led_loopdetect_blink_rate_t *pRate)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->led_loopdetect_blink_rate_get(unit, pRate);
}


/**
 * @internal      yt_led_force_mode_set
 * @endinternal
 *
 * @brief         select the cpu force mode
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[in]     mode                -force mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_force_mode_set(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_force_mode_t mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->led_force_mode_set(unit, port, ledId, mode);
}

/**
 * @internal      yt_led_force_mode_get
 * @endinternal
 *
 * @brief         get cpu force mode of LED
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[out]    pMode               -force mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_force_mode_get(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_force_mode_t *pMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->led_force_mode_get(unit, port, ledId, pMode);
}

/**
 * @internal      yt_led_blink_duty_set
 * @endinternal
 *
 * @brief         select the rate of force mode
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[in]     rate                -rate
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_force_rate_set(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_force_rate_t rate)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->led_force_rate_set(unit, port, ledId, rate);
}

/**
 * @internal      yt_led_force_rate_get
 * @endinternal
 *
 * @brief         get the rate of force mode
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[out]    pRate               -rate
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_force_rate_get(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_force_rate_t *pRate)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pRate), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->led_force_rate_get(unit, port, ledId, pRate);
}

/**
 * @internal      yt_led_serial_activeMode_set
 * @endinternal
 *
 * @brief         select the active mode of serial LED
 * @param[in]     unit                -unit id
 * @param[in]     mode                -active mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_serial_activeMode_set(yt_unit_t unit, yt_sled_activeMode_t mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->led_serial_activeMode_set(unit, mode);
}


/**
 * @internal      yt_led_serial_activeMode_get
 * @endinternal
 *
 * @brief         get the active mode of serial LED
 * @param[in]     unit                -unit id
 * @param[out]    pMode               -active mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_serial_activeMode_get(yt_unit_t unit, yt_sled_activeMode_t *pMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->led_serial_activeMode_get(unit, pMode);
}

/**
 * @internal      yt_led_serial_remapping_set
 * @endinternal
 *
 * @brief         select the remapping state of serial LED
 * @param[in]     unit                -unit id
 * @param[in]     index               -index of led_data
 * @param[in]     dstInfo             -the destination information of remapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_serial_remapping_set(yt_unit_t unit, uint8_t index, yt_led_remapping_t dstInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, dstInfo.port))), CMM_ERR_PORT);
    
    return YT_DISPATCH(unit)->led_serial_remapping_set(unit, index, dstInfo);
}

/**
 * @internal      yt_led_serial_remapping_get
 * @endinternal
 *
 * @brief         get the remapping state of serial LED
 * @param[in]     unit                -unit id
 * @param[in]     index               -index of led_data
 * @param[out     pDstInfo            -the destination information of remapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_serial_remapping_get(yt_unit_t unit, uint8_t index, yt_led_remapping_t *pDstInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pDstInfo), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->led_serial_remapping_get(unit, index, pDstInfo);
}

/**
 * @internal      yt_led_serial_enable_set
 * @endinternal
 *
 * @brief         enabel/disable serial LED
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_serial_enable_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->led_serial_enable_set(unit, enable);
}

/**
 * @internal      yt_led_serial_enable_get
 * @endinternal
 *
 * @brief         get enable state of serial LED
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_serial_enable_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->led_serial_enable_get(unit, pEnable);
}

/**
 * @internal      yt_led_parallel_output_set
 * @endinternal
 *
 * @brief         select output port mask of parallel LED
 * @param[in]     unit                -unit id
 * @param[in]     port_mask           -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_parallel_output_set(yt_unit_t unit, yt_port_mask_t port_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, port_mask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->led_parallel_output_set(unit, port_mask);
}

/**
 * @internal      yt_led_parallel_output_get
 * @endinternal
 *
 * @brief         get output port mask of parallel LED
 * @param[in]     unit                -unit id
 * @param[out]    pport_mask          -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_led_parallel_output_get(yt_unit_t unit, yt_port_mask_t *pport_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pport_mask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->led_parallel_output_get(unit, pport_mask);
}

/**
 * @internal      yt_led_parallel_remapping_set
 * @endinternal
 *
 * @brief         select the remapping state of parallel LED
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -the source information of remapping
 * @param[in]     dstInfo             -the destination information of remapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_parallel_remapping_set(yt_unit_t unit, yt_led_remapping_t srcInfo, yt_led_remapping_t dstInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, srcInfo.port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, dstInfo.port))), CMM_ERR_PORT);
    
    return YT_DISPATCH(unit)->led_parallel_remapping_set(unit, srcInfo, dstInfo);
}

/**
 * @internal      yt_led_parallel_remapping_get
 * @endinternal
 *
 * @brief         get the remapping state of parallel LED
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -the source information of remapping
 * @param[out]    pDstInfo            -the destination information of remapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_parallel_remapping_get(yt_unit_t unit, yt_led_remapping_t srcInfo, yt_led_remapping_t *pDstInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, srcInfo.port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pDstInfo), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->led_parallel_remapping_get(unit, srcInfo, pDstInfo);
}

/**
 * @internal      yt_led_parallel_pos_invert_set
 * @endinternal
 *
 * @brief         invert led_pos signal for parallel mode
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_parallel_pos_invert_set(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
	CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->led_parallel_pos_invert_set(unit, port, ledId, enable);
}

/**
 * @internal      yt_led_parallel_pos_invert_get
 * @endinternal
 *
 * @brief         get inverted status of led_pos signal
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[in]     pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_led_parallel_pos_invert_get(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
	CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->led_parallel_pos_invert_get(unit, port, ledId, pEnable);
}
