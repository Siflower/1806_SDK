/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_dos.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_dos.h"
#include "fal_dispatch.h"

yt_ret_t yt_dos_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->dos_init(unit);
}

yt_ret_t yt_dos_port_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->dos_port_en_set(unit, port, enable);
}

yt_ret_t yt_dos_port_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->dos_port_en_get(unit, port, pEnable);
}

yt_ret_t yt_dos_drop_en_set(yt_unit_t unit, yt_dos_type_t type, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_DOS_TYPE_MAX <= type || YT_DOS_TYPE_UDP_SPORT_EQ_DPORT > type), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->dos_drop_en_set(unit, type, enable);
}

yt_ret_t yt_dos_drop_en_get(yt_unit_t unit, yt_dos_type_t type, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_DOS_TYPE_MAX <= type || YT_DOS_TYPE_UDP_SPORT_EQ_DPORT > type), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->dos_drop_en_get(unit, type, pEnable);
}

yt_ret_t yt_dos_action_set(yt_unit_t unit, yt_dos_type_t dosType, yt_act_type_t action)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((ACT_TYPE_TRAP < action || ACT_TYPE_FWD > action), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_DOS_TYPE_MAX <= dosType || YT_DOS_TYPE_UDP_SPORT_EQ_DPORT > dosType), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->dos_action_set(unit, dosType, action);
}

yt_ret_t yt_dos_action_get(yt_unit_t unit, yt_dos_type_t dosType, yt_act_type_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_DOS_TYPE_MAX <= dosType || YT_DOS_TYPE_UDP_SPORT_EQ_DPORT > dosType), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->dos_action_get(unit, dosType, pAction);
}

yt_ret_t yt_dos_salearn_dis_set(yt_unit_t unit, yt_dos_type_t dosType, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_DOS_TYPE_MAX <= dosType || YT_DOS_TYPE_UDP_SPORT_EQ_DPORT > dosType), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->dos_salearn_dis_set(unit, dosType, enable);
}

yt_ret_t yt_dos_salearn_dis_get(yt_unit_t unit, yt_dos_type_t dosType, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_DOS_TYPE_MAX <= dosType || YT_DOS_TYPE_UDP_SPORT_EQ_DPORT > dosType), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->dos_salearn_dis_get(unit, dosType, pEnable);
}

 yt_ret_t yt_dos_large_packet_size_set(yt_unit_t unit, yt_dos_packet_type_t packType, yt_dos_packet_version_t ver, uint16_t size)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_DOS_VERSION_6 < ver || YT_DOS_VERSION_4 > ver), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_LARGE_TYPE_MAX <= packType || YT_LARGE_ICMP > packType), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->dos_large_packet_size_set(unit, packType, ver, size);
}

 yt_ret_t yt_dos_large_packet_size_get(yt_unit_t unit, yt_dos_packet_type_t packType, yt_dos_packet_version_t ver, uint16_t *pSize)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_DOS_VERSION_6 < ver || YT_DOS_VERSION_4 > ver), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_LARGE_TYPE_MAX <= packType || YT_LARGE_ICMP > packType), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pSize), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->dos_large_packet_size_get(unit, packType, ver, pSize);
}
