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


yt_ret_t yt_stat_mib_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->stat_mib_init(unit);
}

yt_ret_t yt_stat_mib_enable_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->stat_mib_enable_set(unit, enable);
}

yt_ret_t yt_stat_mib_enable_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->stat_mib_enable_get(unit, pEnable);
}

yt_ret_t yt_stat_mib_clear(yt_unit_t unit, yt_port_t port)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->stat_mib_clear(unit, port);
}

yt_ret_t yt_stat_mib_clear_all(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->stat_mib_clear_all(unit);
}

yt_ret_t yt_stat_mib_port_get(yt_unit_t unit, yt_port_t port, yt_stat_mib_port_cnt_t *pCnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pCnt), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->stat_mib_port_get(unit, port, pCnt);
}

yt_ret_t yt_stat_mib_port_singleType_cnt_get(yt_unit_t unit, yt_port_t port, yt_stat_mib_t mibType, uint64 *pCnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((mibType >= YT_STAT_MAX), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pCnt), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->stat_mib_port_singleType_cnt_get(unit, port, mibType, pCnt);
}

yt_ret_t yt_stat_flow_enable_set(yt_unit_t unit, uint32_t flow_id, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->stat_flow_enable_set(unit, flow_id, enable);
}

yt_ret_t yt_stat_flow_enable_get(yt_unit_t unit, uint32_t flow_id, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->stat_flow_enable_get(unit, flow_id, pEnable);
}

yt_ret_t yt_stat_flow_mode_set(yt_unit_t unit, uint32_t flow_id, yt_stat_type_t type, yt_stat_mode_t mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_STATE_TYPE_MAX <= type || YT_STATE_TYPE_FLOW > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_STATE_MODE_PACKET< mode || YT_STATE_MODE_BYTE > mode), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->stat_flow_mode_set(unit, flow_id, type, mode);
}

yt_ret_t yt_stat_flow_mode_get(yt_unit_t unit, uint32_t flow_id, yt_stat_type_t *pType, yt_stat_mode_t *pMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pType), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->stat_flow_mode_get(unit, flow_id, pType, pMode);
}

yt_ret_t yt_stat_flow_count_set(yt_unit_t unit, uint32_t flow_id, uint64 cnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->stat_flow_count_set(unit, flow_id, cnt);
}

yt_ret_t yt_stat_flow_count_get(yt_unit_t unit, uint32_t flow_id, uint64 *pCnt)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pCnt), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->stat_flow_count_get(unit, flow_id, pCnt);
}
#if defined(LINUX_KERNEL_MODE)
EXPORT_SYMBOL(yt_stat_mib_clear);
EXPORT_SYMBOL(yt_stat_mib_port_get);
EXPORT_SYMBOL(yt_stat_mib_clear_all);
EXPORT_SYMBOL(yt_stat_mib_enable_get);
EXPORT_SYMBOL(yt_stat_mib_enable_set);
#endif

