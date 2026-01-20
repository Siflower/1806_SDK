/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_rate.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_RATE_H
#define __FAL_TIGER_RATE_H

#include "fal_cmm.h"
#include "yt_qos.h"
#include "fal_tiger_qos.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define FAL_TIGER_DEFAULT_TOKEN_UNIT                       0x5
#define FAL_TIGER_DEFAULT_METER_TIME_SLOT                  0x50
#define FAL_TIGER_DEFAULT_PSHAP_TIME_SLOT                  0x50
#define FAL_TIGER_DEFAULT_QSHAP_TIME_SLOT                  0x84
#define FAL_TIGER_DEFAULT_BURST_SIZE                       0x3800
#define FAL_SYS_CLK_REG                                    0xE0040

#define FAL_TIGER_RATE_WORD_SIZE                           (sizeof(uint32_t) * 8)
#define FAL_TIGER_RATE_FLOWMETER_WORD_NUM                  (2)
#define FAL_TIGER_RATE_QSHP_WORD_NUM                       (5)

extern uint8_t g_cycle_time[YT_MAX_UNIT];
#define FAL_CHIP_CYCLE_TIME(unit) g_cycle_time[unit]

extern yt_ret_t fal_tiger_rate_init(yt_unit_t unit);
extern yt_ret_t fal_tiger_rate_igrBandwidthCtrlEnable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);
extern yt_ret_t fal_tiger_rate_igrBandwidthCtrlEnable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);
extern yt_ret_t fal_tiger_rate_igrBandwidthCtrlMode_set(yt_unit_t unit, yt_port_t port, yt_port_rate_mode_t *pRateMode);
extern yt_ret_t fal_tiger_rate_igrBandwidthCtrlMode_get(yt_unit_t unit, yt_port_t port, yt_port_rate_mode_t *pRateMode);
extern yt_ret_t fal_tiger_rate_igrBandwidthCtrlRate_set(yt_unit_t unit, yt_port_t port, yt_rate_t rate);
extern yt_ret_t fal_tiger_rate_igrBandwidthCtrlRate_get(yt_unit_t unit, yt_port_t port, yt_rate_t *pRate);
extern yt_ret_t fal_tiger_rate_meter_vlan_enable_set(yt_unit_t unit, yt_vlan_t vid, yt_meterid_t meterId, yt_enable_t enable);
extern yt_ret_t fal_tiger_rate_meter_vlan_enable_get(yt_unit_t unit, yt_vlan_t vid, yt_meterid_t *pMeter_id, yt_enable_t *pEnable);
extern yt_ret_t fal_tiger_rate_meter_enable_set(yt_unit_t unit, yt_meterid_t meterId, yt_enable_t enable);
extern yt_ret_t fal_tiger_rate_meter_enable_get(yt_unit_t unit, yt_meterid_t meterId, yt_enable_t *pEnable);
extern yt_ret_t fal_tiger_rate_meter_mode_set(yt_unit_t unit, yt_meterid_t meterId, yt_rate_meter_mode_t *pMode);
extern yt_ret_t fal_tiger_rate_meter_mode_get(yt_unit_t unit, yt_meterid_t meterId, yt_rate_meter_mode_t *pMode);
extern yt_ret_t fal_tiger_rate_meter_rate_set(yt_unit_t unit, yt_meterid_t meterId, yt_qos_two_rate_t *pRate);
extern yt_ret_t fal_tiger_rate_meter_rate_get(yt_unit_t unit, yt_meterid_t meterId, yt_qos_two_rate_t *pRate);
extern yt_ret_t fal_tiger_rate_shaping_port_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);
extern yt_ret_t fal_tiger_rate_shaping_port_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);
extern yt_ret_t fal_tiger_rate_shaping_port_mode_set(yt_unit_t unit, yt_port_t port, yt_shaping_mode_t *pShpMode);
extern yt_ret_t fal_tiger_rate_shaping_port_mode_get(yt_unit_t unit, yt_port_t port, yt_shaping_mode_t *pShpMode);
extern yt_ret_t fal_tiger_rate_shaping_port_rate_set(yt_unit_t unit, yt_port_t port, yt_rate_t rate);
extern yt_ret_t fal_tiger_rate_shaping_port_rate_get(yt_unit_t unit, yt_port_t port, yt_rate_t *pRate);
extern yt_ret_t fal_tiger_rate_shaping_queue_enable_set(yt_unit_t unit, yt_qid_t qinfo, yt_enable_t cShpEn, yt_enable_t eShpEn);
extern yt_ret_t fal_tiger_rate_shaping_queue_enable_get(yt_unit_t unit, yt_qid_t qinfo, yt_enable_t *pCshpEn, yt_enable_t *pEshpEn);
extern yt_ret_t fal_tiger_rate_shaping_queue_mode_set(yt_unit_t unit, yt_qid_t qinfo, yt_shaping_mode_t *pShpMode);
extern yt_ret_t fal_tiger_rate_shaping_queue_mode_get(yt_unit_t unit, yt_qid_t qinfo, yt_shaping_mode_t *pShpMode);
extern yt_ret_t fal_tiger_rate_shaping_queue_rate_set(yt_unit_t unit, yt_qid_t qinfo, yt_qos_two_rate_t *pRate);
extern yt_ret_t fal_tiger_rate_shaping_queue_rate_get(yt_unit_t unit, yt_qid_t qinfo, yt_qos_two_rate_t *pRate);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
