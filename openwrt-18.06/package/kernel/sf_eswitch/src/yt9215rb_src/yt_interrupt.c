/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_interrupt.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_interrupt.h"
#include "fal_dispatch.h"

yt_ret_t yt_int_polarity_set(yt_unit_t unit, yt_int_polarity_t type)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((INT_POLAR_HIGH < type || INT_POLAR_LOW > type), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->int_polarity_set(unit, type);
}

yt_ret_t yt_int_polarity_get(yt_unit_t unit, yt_int_polarity_t *pType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pType), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->int_polarity_get(unit, pType);
}

yt_ret_t yt_int_control_set(yt_unit_t unit, yt_int_type_t type, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((INT_TYPE_END <= type || 0 > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->int_control_set(unit, type, enable);
}

yt_ret_t yt_int_control_get(yt_unit_t unit, yt_int_type_t type, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((INT_TYPE_END <= type || 0 > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->int_control_get(unit, type, pEnable);
}

yt_ret_t yt_int_status_get(yt_unit_t unit, yt_intr_status_t *pIntStatus)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pIntStatus), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->int_status_get(unit, pIntStatus);
}

yt_ret_t yt_int_typeStatus_get(yt_unit_t unit, yt_int_type_t type, yt_intr_status_t intStatus, yt_bool_t *pActive)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pActive), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->int_typeStatus_get(unit, type, intStatus, pActive);
}

yt_ret_t yt_int_port_int_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->int_port_int_set(unit, port, enable);
}

yt_ret_t yt_int_port_int_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->int_port_int_get(unit, port, pEnable);
}

yt_ret_t yt_int_allPortIntStatus_get(yt_unit_t unit, yt_intr_status_t *pIntStatus)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pIntStatus), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->int_allPortIntStatus_get(unit, pIntStatus);
}

yt_ret_t yt_int_portIntStatus_get(yt_unit_t unit, yt_port_t port, yt_intr_status_t intStatus, yt_bool_t *pActive)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pActive), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->int_portIntStatus_get(unit, port, intStatus, pActive);
}

yt_ret_t yt_int_wol_signalOutputType_set(yt_unit_t unit, yt_int_wol_outputType_t outputType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_INT_OUTPUT_TYPE_LOW_PULSE < outputType || YT_INT_OUTPUT_TYPE_HIGH_LEVEL > outputType), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->int_wol_signalOutputType_set(unit, outputType);
}

yt_ret_t yt_int_wol_signalOutputType_get(yt_unit_t unit, yt_int_wol_outputType_t *pOutputType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pOutputType), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->int_wol_signalOutputType_get(unit, pOutputType);
}

yt_ret_t yt_int_wol_port_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->int_wol_port_enable_set(unit, port, enable);
}

yt_ret_t yt_int_wol_port_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->int_wol_port_enable_get(unit, port, pEnable);
}

yt_ret_t yt_int_wol_allPortIntStatus_get(yt_unit_t unit, yt_intr_status_t *pIntStatus)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pIntStatus), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->int_wol_allPortIntStatus_get(unit, pIntStatus);
}

