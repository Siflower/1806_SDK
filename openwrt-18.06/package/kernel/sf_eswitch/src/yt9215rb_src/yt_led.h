/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_led.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_LED_H
#define __YT_LED_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "yt_cmm.h"

typedef enum yt_led_id_e
{
    LED_ID_0 = 0,
    LED_ID_1,
    LED_ID_2,
    LED_ID_NUM
}yt_led_id_t;

typedef struct yt_led_act_cfg_s {
    uint8_t    spd10m_blink_en;
    uint8_t    spd100m_blink_en;
    uint8_t    spd1000m_blink_en;
    uint8_t    collision_blink_en;
    uint8_t    spd10m_on_en;
    uint8_t    spd100m_on_en;
    uint8_t    spd1000m_on_en;
    uint8_t    rxact_on_en;
    uint8_t    txact_on_en;
    uint8_t    rxact_blink_en;
    uint8_t    txact_blink_en;
    uint8_t    half_duplex_en;
    uint8_t    full_duplex_en;
    uint8_t    active_blink_indicate_en;
    uint8_t    loopdetect_indicate_en;
    uint8_t    eee_indicate_en;
    uint8_t    collision_blink_indicate_en;      /* only for LED0 */
    uint8_t    disable_link_try_en;     /* only for LED0 */
} yt_led_act_cfg_t;

typedef enum yt_led_blink_event_e {
    LED_BLINK_EVENT_EEE = 0,
    LED_BLINK_EVENT_OTHERS,
    LED_BLINK_EVENT_NUM
}yt_led_blink_event_t;

typedef enum yt_led_blink_freq_e {
    LED_BLINK_FREQ_2HZ = 0,
    LED_BLINK_FREQ_4HZ,
    LED_BLINK_FREQ_8HZ,
    LED_BLINK_FREQ_16HZ,
    LED_BLINK_FREQ_NUM
}yt_led_blink_freq_t;

typedef enum yt_led_blink_duty_e {
    LED_BLINK_DUTY_50 = 0,
    LED_BLINK_DUTY_67 = 1,
    LED_BLINK_DUTY_75 = 2,
    LED_BLINK_DUTY_83 = 3,
    LED_BLINK_DUTY_33 = 5,
    LED_BLINK_DUTY_25 = 6,
    LED_BLINK_DUTY_17 = 7
}yt_led_blink_duty_t;

typedef enum yt_led_loopdetect_blink_rate_e {
    LED_LOOPDETECT_RATE_512MS = 0,
    LED_LOOPDETECT_RATE_1024MS,
    LED_LOOPDETECT_RATE_1536MS,
    LED_LOOPDETECT_RATE_2048MS,
    LED_LOOPDETECT_RATE_NUM
}yt_led_loopdetect_blink_rate_t;

typedef enum yt_led_force_mode_e {
    LED_FORCE_MODE_NORMAL = 0,      /* no force */
    LED_FORCE_MODE_BLINK,
    LED_FORCE_MODE_LOW,
    LED_FORCE_MODE_HIGH,
    LED_FORCE_MODE_NUM
}yt_led_force_mode_t;

typedef enum yt_led_force_rate_e {
    LED_FORCE_RATE_512MS = 0,
    LED_FORCE_RATE_1024MS,
    LED_FORCE_RATE_2048MS,
    LED_FORCE_RATE_NORMAL,
    LED_FORCE_RATE_NUM
}yt_led_force_rate_t;

typedef struct yt_led_remapping_s {
    yt_port_t port;
    yt_led_id_t ledId;
}yt_led_remapping_t;

/**
 * @internal      yt_led_enable
 * @endinternal
 *
 * @brief         enabel LED
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_enable(yt_unit_t unit);

/**
 * @internal      yt_led_mode_set
 * @endinternal
 *
 * @brief         select the mode of led
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     mode                -serial or parallel mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_mode_set(yt_unit_t unit, yt_led_mode_t mode);

/**
 * @internal      yt_led_mode_get
 * @endinternal
 *
 * @brief         get the mode of led
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pmode               -serial or parallel mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_mode_get(yt_unit_t unit, yt_led_mode_t *pMode);

/**
 * @internal      yt_led_action_set
 * @endinternal
 *
 * @brief         select the action of LED0~2 per port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[in]     ledActCfg           -config of led action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_action_set(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_act_cfg_t ledActCfg);

/**
 * @internal      yt_led_action_get
 * @endinternal
 *
 * @brief         get the action of LED0~2 per port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[out]    pLedActCfg          -config of led action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_action_get(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_act_cfg_t *pLedActCfg);

/**
 * @internal      yt_led_blink_freq_set
 * @endinternal
 *
 * @brief         select the frequency of blink
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     event               -event associated with blink
 * @param[in]     freq                -frequency of blink
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_blink_freq_set(yt_unit_t unit, yt_port_t port, yt_led_blink_event_t event, yt_led_blink_freq_t freq);

/**
 * @internal      yt_led_blink_freq_get
 * @endinternal
 *
 * @brief         get the frequency of blink
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     event               -event associated with blink
 * @param[out]    pFreq               -frequency of blink
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_blink_freq_get(yt_unit_t unit, yt_port_t port, yt_led_blink_event_t event, yt_led_blink_freq_t *pFreq);

/**
 * @internal      yt_led_blink_duty_set
 * @endinternal
 *
 * @brief         select the duty of blink
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     duty                -duty of blink
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_blink_duty_set(yt_unit_t unit, yt_port_t port, yt_led_blink_duty_t duty);

/**
 * @internal      yt_led_blink_duty_get
 * @endinternal
 *
 * @brief         get the duty of blink
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pDuty               -duty of blink
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_blink_duty_get(yt_unit_t unit, yt_port_t port, yt_led_blink_duty_t *pDuty);

/**
 * @internal      yt_led_loopdetect_blink_rate_set
 * @endinternal
 *
 * @brief         select blinking rate of loopdetect
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     rate                -blinking rate
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_loopdetect_blink_rate_set(yt_unit_t unit, yt_led_loopdetect_blink_rate_t rate);

/**
 * @internal      yt_led_loopdetect_blink_rate_get
 * @endinternal
 *
 * @brief         get blinking rate of loopdetect
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pRate               -blinking rate
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_loopdetect_blink_rate_get(yt_unit_t unit, yt_led_loopdetect_blink_rate_t *pRate);

/**
 * @internal      yt_led_force_mode_set
 * @endinternal
 *
 * @brief         select the cpu force mode
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[in]     mode                -force mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_force_mode_set(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_force_mode_t mode);

/**
 * @internal      yt_led_force_mode_get
 * @endinternal
 *
 * @brief         get cpu force mode of LED
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[out]    pMode               -force mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_force_mode_get(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_force_mode_t *pMode);

/**
 * @internal      yt_led_force_rate_set
 * @endinternal
 *
 * @brief         select the rate of force mode
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[in]     rate                -
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_force_rate_set(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_force_rate_t rate);

/**
 * @internal      yt_led_force_rate_get
 * @endinternal
 *
 * @brief         get the rate of force mode
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[out]    pRate               -rate
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_force_rate_get(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_force_rate_t *pRate);


/**
 * @internal      yt_led_serial_activeMode_set
 * @endinternal
 *
 * @brief         select the active mode of serial LED
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     mode                -active mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_serial_activeMode_set(yt_unit_t unit, yt_sled_activeMode_t mode);


/**
 * @internal      yt_led_serial_activeMode_get
 * @endinternal
 *
 * @brief         get the active mode of serial LED
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pMode               -active mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_serial_activeMode_get(yt_unit_t unit, yt_sled_activeMode_t *pMode);

/**
 * @internal      yt_led_serial_remapping_set
 * @endinternal
 *
 * @brief         select the remapping state of serial LED(dst-->src)
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     index               -index of led_data
 * @param[in]     dstInfo             -the destination information of remapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_serial_remapping_set(yt_unit_t unit, uint8_t index, yt_led_remapping_t dstInfo);
/**
 * @internal      yt_led_serial_remapping_get
 * @endinternal
 *
 * @brief         get the remapping state of serial LED
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     index               -index of led_data
 * @param[out]    pDstInfo            -the destination information of remapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_serial_remapping_get(yt_unit_t unit, uint8_t index, yt_led_remapping_t *pDstInfo);

/**
 * @internal      yt_led_serial_enable_set
 * @endinternal
 *
 * @brief         enabel/disable serial LED
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_serial_enable_set(yt_unit_t unit, yt_enable_t enable);

/**
 * @internal      yt_led_serial_enable_get
 * @endinternal
 *
 * @brief         get enable state of serial LED
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_serial_enable_get(yt_unit_t unit, yt_enable_t *pEnable);

/**
 * @internal      yt_led_parallel_output_set
 * @endinternal
 *
 * @brief         select output port mask of parallel LED
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port_mask           -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_parallel_output_set(yt_unit_t unit, yt_port_mask_t port_mask);

/**
 * @internal      yt_led_parallel_output_get
 * @endinternal
 *
 * @brief         get output port mask of parallel LED
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pport_mask          -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_parallel_output_get(yt_unit_t unit, yt_port_mask_t *pport_mask);

/**
 * @internal      yt_led_parallel_remapping_set
 * @endinternal
 *
 * @brief         select the remapping state of parallel LED(dst-->src)
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -the source information of remapping
 * @param[in]     dstInfo             -the destination information of remapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_parallel_remapping_set(yt_unit_t unit, yt_led_remapping_t srcInfo, yt_led_remapping_t dstInfo);

/**
 * @internal      yt_led_parallel_remapping_get
 * @endinternal
 *
 * @brief         get the remapping state of parallel LED
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -the source information of remapping
 * @param[out]    pDstInfo            -the destination information of remapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_parallel_remapping_get(yt_unit_t unit, yt_led_remapping_t srcInfo, yt_led_remapping_t *pDstInfo);

/**
 * @internal      yt_led_parallel_pos_invert_set
 * @endinternal
 *
 * @brief         invert led_pos signal for parallel mode
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_parallel_pos_invert_set(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_enable_t enable);

/**
 * @internal      yt_led_parallel_pos_invert_get
 * @endinternal
 *
 * @brief         get inverted status of led_pos signal
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[in]     pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_led_parallel_pos_invert_get(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_enable_t *pEnable);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
