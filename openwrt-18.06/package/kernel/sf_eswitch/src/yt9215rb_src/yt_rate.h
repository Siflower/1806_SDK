/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_rate.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_RATE_H
#define __YT_RATE_H
#include "yt_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum yt_rate_bps_gap_e
{
    YT_RATE_BPS_GAP_EXCLUDE,
    YT_RATE_BPS_GAP_INCLUDE,
}yt_rate_bps_gap_t;

typedef struct yt_port_rate_mode_s
{
    /* bps or pps */
    yt_rate_mode_t    rateMode;
    /* excGAP or incGAP */
    yt_rate_bps_gap_t    gapMode;
}yt_port_rate_mode_t;

typedef struct yt_shaping_mode_s
{
    yt_rate_mode_t shpMode;
    yt_rate_bps_gap_t schMode;
}yt_shaping_mode_t;

typedef struct
{
     /* bps or pps */
    yt_rate_t    eir;
    /* bps or pps */
    yt_rate_t    cir;
}yt_qos_two_rate_t;

typedef enum yt_meter_mode_e
{
    YT_METER_MODE_RFC4115,
    YT_METER_MODE_RFC2698
} yt_meter_mode_t;

typedef enum yt_color_mode_e
{
    YT_COLOR_AWARE,
    YT_COLOR_BLIND
} yt_color_mode_t;

typedef enum yt_cf_mode_e
{
    YT_CF_MODE_NONE,
    YT_CF_MODE_LEAKY
}yt_cf_mode_t;

typedef enum yt_drop_color_e
{
    YT_DROP_COLOR_GYR = 0, /* drop color is GREEN YELLOW RED */
    YT_DROP_COLOR_YR,      /* drop color is YELLOW RED */
    YT_DROP_COLOR_R,       /* drop color is RED */
    YT_DROP_COLOR_NONE,    /* no drop color */
}yt_drop_color_t;

#define YT_RATE_METER_FLAG_METERMODE        BIT(0)
#define YT_RATE_METER_FLAG_RATEMODE         BIT(1)
#define YT_RATE_METER_FLAG_COLORMODE        BIT(2)
#define YT_RATE_METER_FLAG_DROPCOLOR        BIT(3)
#define YT_RATE_METER_FLAG_GAP              BIT(4)
#define YT_RATE_METER_FLAG_CFMODE           BIT(5)
#define YT_RATE_METER_FLAG_ALL              (YT_RATE_METER_FLAG_METERMODE | YT_RATE_METER_FLAG_RATEMODE \
                                            | YT_RATE_METER_FLAG_COLORMODE | YT_RATE_METER_FLAG_DROPCOLOR \
                                            | YT_RATE_METER_FLAG_GAP | YT_RATE_METER_FLAG_CFMODE)

typedef struct yt_rate_meter_mode_s
{
    uint32_t flags;
    /* RFC4115 or RFC2698 */
    yt_meter_mode_t    meterMode;
    /* bps or pps */
    yt_rate_mode_t    rateMode;
    /* aware or blind */
    yt_color_mode_t    colorMode;
    yt_drop_color_t    dropColor;
    /* excGAP or incGAP */
    yt_rate_bps_gap_t    gapMode;
    /* carry overflow Green buckets to Yellow when meter mode is RFC4115 */
    yt_cf_mode_t    cfMode;
}yt_rate_meter_mode_t;

/**
 * @internal      yt_rate_init
 * @endinternal
 *
 * @brief         Init rate module
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_init(yt_unit_t unit);


/**
 * @internal      yt_rate_igrBandwidthCtrlEnable_set
 * @endinternal
 *
 * @brief         Config port ingress bandwidth control state
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_igrBandwidthCtrlEnable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      yt_rate_igrBandwidthCtrlEnable_get
 * @endinternal
 *
 * @brief         Get port ingress bandwidth control state
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_igrBandwidthCtrlEnable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      yt_rate_igrBandwidthCtrlMode_set
 * @endinternal
 *
 * @brief         Config port ingress bandwidth control mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pRateMode           -config rate mode(bps or pps) and gap mode(include or exclude gap)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_igrBandwidthCtrlMode_set(yt_unit_t unit, yt_port_t port, yt_port_rate_mode_t *pRateMode);


/**
 * @internal      yt_rate_igrBandwidthCtrlMode_get
 * @endinternal
 *
 * @brief         Get port ingress bandwidth control mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pRateMode           -config rate mode(bps or pps) and gap mode(include or exclude gap)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_igrBandwidthCtrlMode_get(yt_unit_t unit, yt_port_t port, yt_port_rate_mode_t *pRateMode);


/**
 * @internal      yt_rate_igrBandwidthCtrlRate_set
 * @endinternal
 *
 * @brief         Config port ingress bandwidth control rate value.
 *                The min unit of tiger is 32kbps(1k=1000) or 32pps.
 *                The min unit of shark is 16kbps(1k=1000) or 16pps.
 *                The min unit of Whale is 64kbps(1k=1000) or 64pps.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     rate                -rate setting.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_igrBandwidthCtrlRate_set(yt_unit_t unit, yt_port_t port, yt_rate_t rate);


/**
 * @internal      yt_rate_igrBandwidthCtrlRate_get
 * @endinternal
 *
 * @brief         Get port ingress bandwidth control rate value
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pRate               -rate setting
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_igrBandwidthCtrlRate_get(yt_unit_t unit, yt_port_t port, yt_rate_t *pRate);


/**
 * @internal      yt_rate_meter_vlan_enable_set
 * @endinternal
 *
 * @brief         Config vlan based meter state
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     meterId             -meter id, can access meter tbl by it. The range is 0~31.
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_meter_vlan_enable_set(yt_unit_t unit, yt_vlan_t vid, yt_meterid_t meterId, yt_enable_t enable);


/**
 * @internal      yt_rate_meter_vlan_enable_get
 * @endinternal
 *
 * @brief         Get vlan based meter state
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[out]    pMeter_id           -meter id, can access meter tbl by it
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_meter_vlan_enable_get(yt_unit_t unit, yt_vlan_t vid, yt_meterid_t *pMeter_id, yt_enable_t *pEnable);


/**
 * @internal      yt_rate_meter_enable_set
 * @endinternal
 *
 * @brief         Config meter entry state
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     meterId             -meter id, can access meter tbl by it
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_meter_enable_set(yt_unit_t unit, yt_meterid_t meterId, yt_enable_t enable);


/**
 * @internal      yt_rate_meter_enable_get
 * @endinternal
 *
 * @brief         Get meter entry state
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     meterId             -meter id, can access meter tbl by it
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_meter_enable_get(yt_unit_t unit, yt_meterid_t meterId, yt_enable_t *pEnable);


/**
 * @internal      yt_rate_meter_mode_set
 * @endinternal
 *
 * @brief         Config meter entry mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     meterId             -meter id, can access meter tbl by it
 * @param[in]     pMode               - meter mode config
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_meter_mode_set(yt_unit_t unit, yt_meterid_t meterId, yt_rate_meter_mode_t *pMode);


/**
 * @internal      yt_rate_meter_mode_get
 * @endinternal
 *
 * @brief         Get meter entry mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     meterId             -meter id, can access meter tbl by it
 * @param[out]    pMode               - meter mode config
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_meter_mode_get(yt_unit_t unit, yt_meterid_t meterId, yt_rate_meter_mode_t *pMode);


/**
 * @internal      yt_rate_meter_rate_set
 * @endinternal
 *
 * @brief         Config meter entry rate value.
 *                The min unit of tiger is 32kbps(1k=1000) or 32pps.
 *                The min unit of shark is 16kbps(1k=1000) or 16pps.
 *                The min unit of Whale is 64kbps(1k=1000) or 64pps.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     meterId             -meter id
 * @param[in]     pRate               -CIR, EIR config.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_meter_rate_set(yt_unit_t unit, yt_meterid_t meterId, yt_qos_two_rate_t *pRate);


/**
 * @internal      yt_rate_meter_rate_get
 * @endinternal
 *
 * @brief         Get meter entry rate value
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     meterId             -meter id, can access meter tbl by it
 * @param[out]    pRate               -CIR, EIR config
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_meter_rate_get(yt_unit_t unit, yt_meterid_t meterId, yt_qos_two_rate_t *pRate);


/**
 * @internal      yt_rate_shaping_port_enable_set
 * @endinternal
 *
 * @brief         Config egress port shaping state
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_shaping_port_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      yt_rate_shaping_port_enable_get
 * @endinternal
 *
 * @brief         Get egress port shaping state
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_shaping_port_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      yt_rate_shaping_port_mode_set
 * @endinternal
 *
 * @brief         Config egress port shaping mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pShpMode            -shaping mode config
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_shaping_port_mode_set(yt_unit_t unit, yt_port_t port, yt_shaping_mode_t *pShpMode);


/**
 * @internal      yt_rate_shaping_port_mode_get
 * @endinternal
 *
 * @brief         Get egress port shaping mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pShpMode            -shaping mode config
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_shaping_port_mode_get(yt_unit_t unit, yt_port_t port, yt_shaping_mode_t *pShpMode);


/**
 * @internal      yt_rate_shaping_port_rate_set
 * @endinternal
 *
 * @brief         Config egress port shaping rate value.
 *                The min unit of tiger is 32kbps(1k=1000) or 32pps.
 *                The min unit of shark is 16kbps(1k=1000) or 16pps.
 *                The min unit of Whale is 64kbps(1k=1000) or 64pps.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     rate                -rate setting.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_shaping_port_rate_set(yt_unit_t unit, yt_port_t port, yt_rate_t rate);


/**
 * @internal      yt_rate_shaping_port_rate_get
 * @endinternal
 *
 * @brief         Get egress port shaping rate value
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pRate               -rate setting
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_shaping_port_rate_get(yt_unit_t unit, yt_port_t port, yt_rate_t *pRate);


/**
 * @internal      yt_rate_shaping_queue_enable_set
 * @endinternal
 *
 * @brief         Set status of egress bandwidth control on specified queue.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[in]     cShpEn              -enable or disable
 * @param[in]     eShpEn              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_shaping_queue_enable_set(yt_unit_t unit, yt_qid_t qinfo, yt_enable_t cShpEn, yt_enable_t eShpEn);


/**
 * @internal      yt_rate_shaping_queue_enable_get
 * @endinternal
 *
 * @brief         Get state of egress bandwidth control on specified queue.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[out]    pCshpEn             -enable or disable
 * @param[out]    pEshpEn             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_shaping_queue_enable_get(yt_unit_t unit, yt_qid_t qinfo, yt_enable_t *pCshpEn, yt_enable_t *pEshpEn);


/**
 * @internal      yt_rate_shaping_queue_mode_set
 * @endinternal
 *
 * @brief         Set mode of egress bandwidth control on specified queue.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[in]     pShpMode            -shaping mode config
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_shaping_queue_mode_set(yt_unit_t unit, yt_qid_t qinfo, yt_shaping_mode_t *pShpMode);


/**
 * @internal      yt_rate_shaping_queue_mode_get
 * @endinternal
 *
 * @brief         Get mode of egress bandwidth control on specified queue.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[out]    pShpMode            -shaping mode config
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_shaping_queue_mode_get(yt_unit_t unit, yt_qid_t qinfo, yt_shaping_mode_t *pShpMode);


/**
 * @internal      yt_rate_shaping_queue_rate_set
 * @endinternal
 *
 * @brief         Set rate value of egress bandwidth control rate on specified queue.
 *                The min unit of tiger is 32kbps(1k=1000) or 32pps.
 *                The min unit of shark is 16kbps(1k=1000) or 16pps.
 *                The min unit of Whale is 64kbps(1k=1000) or 64pps.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[in]     pRate               -CIR, EIR config.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_shaping_queue_rate_set(yt_unit_t unit, yt_qid_t qinfo, yt_qos_two_rate_t *pRate);


/**
 * @internal      yt_rate_shaping_queue_rate_get
 * @endinternal
 *
 * @brief         Get rate of egress bandwidth control on specified queue.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[out]    pRate               -CIR, EIR config
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rate_shaping_queue_rate_get(yt_unit_t unit, yt_qid_t qinfo, yt_qos_two_rate_t *pRate);




#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
