/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_rate.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_rate.h"
#include "fal_dispatch.h"

yt_ret_t yt_rate_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->rate_init(unit);
}

yt_ret_t yt_rate_igrBandwidthCtrlEnable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rate_igrBandwidthCtrlEnable_set(unit, port, enable);
}

yt_ret_t yt_rate_igrBandwidthCtrlEnable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rate_igrBandwidthCtrlEnable_get(unit, port, pEnable);
}

yt_ret_t yt_rate_igrBandwidthCtrlMode_set(yt_unit_t unit, yt_port_t port, yt_port_rate_mode_t *pRateMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pRateMode), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(YT_RATE_BPS_GAP_INCLUDE < pRateMode->gapMode, CMM_ERR_INPUT);
    CMM_PARAM_CHK(YT_RATE_MODE_PPS < pRateMode->rateMode, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rate_igrBandwidthCtrlMode_set(unit, port, pRateMode);
}

yt_ret_t yt_rate_igrBandwidthCtrlMode_get(yt_unit_t unit, yt_port_t port, yt_port_rate_mode_t *pRateMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pRateMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rate_igrBandwidthCtrlMode_get(unit, port, pRateMode);
}

yt_ret_t yt_rate_igrBandwidthCtrlRate_set(yt_unit_t unit, yt_port_t port, yt_rate_t rate)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->rate_igrBandwidthCtrlRate_set(unit, port, rate);
}

yt_ret_t yt_rate_igrBandwidthCtrlRate_get(yt_unit_t unit, yt_port_t port, yt_rate_t *pRate)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pRate), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rate_igrBandwidthCtrlRate_get(unit, port, pRate);
}

yt_ret_t yt_rate_meter_vlan_enable_set(yt_unit_t unit, yt_vlan_t vid, yt_meterid_t meterId, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_VLAN_METER_ENTRY_NUM(unit) <= meterId, CMM_ERR_NOT_SUPPORT);

    return YT_DISPATCH(unit)->rate_meter_vlan_enable_set(unit, vid, meterId, enable);
}

yt_ret_t yt_rate_meter_vlan_enable_get(yt_unit_t unit, yt_vlan_t vid, yt_meterid_t *pMeter_id, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pMeter_id), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rate_meter_vlan_enable_get(unit, vid, pMeter_id, pEnable);
}

yt_ret_t yt_rate_meter_enable_set(yt_unit_t unit, yt_meterid_t meterId, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_METER_ENTRY_NUM(unit) <= meterId, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rate_meter_enable_set(unit, meterId, enable);
}

yt_ret_t yt_rate_meter_enable_get(yt_unit_t unit, yt_meterid_t meterId, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(CAL_MAX_METER_ENTRY_NUM(unit) <= meterId, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rate_meter_enable_get(unit, meterId, pEnable);
}

yt_ret_t yt_rate_meter_mode_set(yt_unit_t unit, yt_meterid_t meterId, yt_rate_meter_mode_t *pMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(CAL_MAX_METER_ENTRY_NUM(unit) <= meterId, CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pMode), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((0 == pMode->flags), CMM_ERR_INPUT);
    if (pMode->flags & YT_RATE_METER_FLAG_RATEMODE)
    {
        CMM_PARAM_CHK(YT_RATE_MODE_PPS < pMode->rateMode, CMM_ERR_INPUT);
    }
    if (pMode->flags & YT_RATE_METER_FLAG_METERMODE)
    {
        CMM_PARAM_CHK(YT_METER_MODE_RFC2698 < pMode->meterMode, CMM_ERR_INPUT);
    }
    if (pMode->flags & YT_RATE_METER_FLAG_GAP)
    {
        CMM_PARAM_CHK(YT_RATE_BPS_GAP_INCLUDE < pMode->gapMode, CMM_ERR_INPUT);
    }
    if (pMode->flags & YT_RATE_METER_FLAG_DROPCOLOR)
    {
        CMM_PARAM_CHK(YT_DROP_COLOR_NONE < pMode->dropColor, CMM_ERR_INPUT);
    }
    if (pMode->flags & YT_RATE_METER_FLAG_COLORMODE)
    {
        CMM_PARAM_CHK(YT_COLOR_BLIND < pMode->colorMode, CMM_ERR_INPUT);
    }
    if (pMode->flags & YT_RATE_METER_FLAG_CFMODE)
    {
        CMM_PARAM_CHK(YT_CF_MODE_LEAKY < pMode->cfMode, CMM_ERR_INPUT);
    }
    return YT_DISPATCH(unit)->rate_meter_mode_set(unit, meterId, pMode);
}

yt_ret_t yt_rate_meter_mode_get(yt_unit_t unit, yt_meterid_t meterId, yt_rate_meter_mode_t *pMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pMode), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((0 == pMode->flags), CMM_ERR_INPUT);
    CMM_PARAM_CHK(CAL_MAX_METER_ENTRY_NUM(unit) <= meterId, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rate_meter_mode_get(unit, meterId, pMode);
}

yt_ret_t yt_rate_meter_rate_set(yt_unit_t unit, yt_meterid_t meterId, yt_qos_two_rate_t *pRate)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pRate), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(CAL_MAX_METER_ENTRY_NUM(unit) <= meterId, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rate_meter_rate_set(unit, meterId, pRate);
}

yt_ret_t yt_rate_meter_rate_get(yt_unit_t unit, yt_meterid_t meterId, yt_qos_two_rate_t *pRate)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pRate), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(CAL_MAX_METER_ENTRY_NUM(unit) <= meterId, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rate_meter_rate_get(unit, meterId, pRate);
}

yt_ret_t yt_rate_shaping_port_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rate_shaping_port_enable_set(unit, port, enable);
}

yt_ret_t yt_rate_shaping_port_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rate_shaping_port_enable_get(unit, port, pEnable);
}

yt_ret_t yt_rate_shaping_port_mode_set(yt_unit_t unit, yt_port_t port, yt_shaping_mode_t *pShpMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pShpMode), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(YT_RATE_BPS_GAP_INCLUDE < pShpMode->schMode, CMM_ERR_INPUT);
    CMM_PARAM_CHK(YT_RATE_MODE_PPS < pShpMode->shpMode, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rate_shaping_port_mode_set(unit, port, pShpMode);
}

yt_ret_t yt_rate_shaping_port_mode_get(yt_unit_t unit, yt_port_t port, yt_shaping_mode_t *pShpMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pShpMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rate_shaping_port_mode_get(unit, port, pShpMode);
}

yt_ret_t yt_rate_shaping_port_rate_set(yt_unit_t unit, yt_port_t port, yt_rate_t rate)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->rate_shaping_port_rate_set(unit, port, rate);
}

yt_ret_t yt_rate_shaping_port_rate_get(yt_unit_t unit, yt_port_t port, yt_rate_t *pRate)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pRate), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rate_shaping_port_rate_get(unit, port, pRate);
}

yt_ret_t yt_rate_shaping_queue_enable_set(yt_unit_t unit, yt_qid_t qinfo, yt_enable_t cShpEn, yt_enable_t eShpEn)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < cShpEn), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ENABLE < eShpEn), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rate_shaping_queue_enable_set(unit, qinfo, cShpEn, eShpEn);
}

yt_ret_t yt_rate_shaping_queue_enable_get(yt_unit_t unit, yt_qid_t qinfo, yt_enable_t *pCshpEn, yt_enable_t *pEshpEn)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pCshpEn), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pEshpEn), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rate_shaping_queue_enable_get(unit, qinfo, pCshpEn, pEshpEn);
}

yt_ret_t yt_rate_shaping_queue_mode_set(yt_unit_t unit, yt_qid_t qinfo, yt_shaping_mode_t *pShpMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pShpMode), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(YT_RATE_BPS_GAP_INCLUDE < pShpMode->schMode, CMM_ERR_INPUT);
    CMM_PARAM_CHK(YT_RATE_MODE_PPS < pShpMode->shpMode, CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->rate_shaping_queue_mode_set(unit, qinfo, pShpMode);
}

yt_ret_t yt_rate_shaping_queue_mode_get(yt_unit_t unit, yt_qid_t qinfo, yt_shaping_mode_t *pShpMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pShpMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rate_shaping_queue_mode_get(unit, qinfo, pShpMode);
}

yt_ret_t yt_rate_shaping_queue_rate_set(yt_unit_t unit, yt_qid_t qinfo, yt_qos_two_rate_t *pRate)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pRate), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rate_shaping_queue_rate_set(unit, qinfo, pRate);
}

yt_ret_t yt_rate_shaping_queue_rate_get(yt_unit_t unit, yt_qid_t qinfo, yt_qos_two_rate_t *pRate)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pRate), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->rate_shaping_queue_rate_get(unit, qinfo, pRate);
}

