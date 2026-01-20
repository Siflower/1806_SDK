/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_ctrlpkt.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_ctrlpkt.h"
#include "fal_dispatch.h"

yt_ret_t yt_ctrlpkt_act_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_act_type_t actType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_CTRL_PKT_ARP_TYPE > pktType || YT_CTRL_PKT_END_TYPE <= pktType), CMM_ERR_INPUT);
    CMM_PARAM_CHK((ACT_TYPE_FWD > actType || ACT_TYPE_TRAP < actType), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->ctrlpkt_act_set(unit, port, pktType, actType);
}

yt_ret_t yt_ctrlpkt_act_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_act_type_t *pActType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_CTRL_PKT_ARP_TYPE > pktType || YT_CTRL_PKT_END_TYPE <= pktType), CMM_ERR_INPUT);
    CMM_PARAM_CHK(pActType == NULL, CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->ctrlpkt_act_get(unit, port, pktType, pActType);
}

yt_ret_t yt_ctrlpkt_myMac_act_set(yt_unit_t unit, yt_ctrlpkt_myMac_act_t *pActConf)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(pActConf == NULL, CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->ctrlpkt_myMac_act_set(unit, pActConf);
}

yt_ret_t yt_ctrlpkt_myMac_act_get(yt_unit_t unit, yt_ctrlpkt_myMac_act_t *pActConf)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(pActConf == NULL, CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->ctrlpkt_myMac_act_get(unit, pActConf);
}

yt_ret_t  yt_ctrlpkt_bypass_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_ctrlpkt_bypass_t *pBypassConf)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_CTRL_PKT_ARP_TYPE > pktType || YT_CTRL_PKT_END_TYPE <= pktType), CMM_ERR_INPUT);
    CMM_PARAM_CHK(pBypassConf == NULL, CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->ctrlpkt_bypass_set(unit, port, pktType, pBypassConf);
}

yt_ret_t yt_ctrlpkt_bypass_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_ctrlpkt_bypass_t *pBypassConf)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_CTRL_PKT_ARP_TYPE > pktType || YT_CTRL_PKT_END_TYPE <= pktType), CMM_ERR_INPUT);
    CMM_PARAM_CHK(pBypassConf == NULL, CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->ctrlpkt_bypass_get(unit, port, pktType, pBypassConf);
}

yt_ret_t yt_ctrlpkt_bypass_stp_set(yt_unit_t unit, yt_ctrlpkt_type_t pktType,
    yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_CTRL_PKT_ARP_TYPE > pktType || YT_CTRL_PKT_END_TYPE <= pktType), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_DISABLE > enable || YT_ENABLE < enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->ctrlpkt_bypass_stp_set(unit, pktType, enable);
}

yt_ret_t yt_ctrlpkt_bypass_stp_get(yt_unit_t unit, yt_ctrlpkt_type_t pktType,
    yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_CTRL_PKT_ARP_TYPE > pktType || YT_CTRL_PKT_END_TYPE <= pktType), CMM_ERR_INPUT);
    CMM_PARAM_CHK(pEnable == NULL, CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->ctrlpkt_bypass_stp_get(unit, pktType, pEnable);
}

yt_ret_t  yt_ctrlpkt_sa_learn_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_CTRL_PKT_ARP_TYPE > pktType || YT_CTRL_PKT_END_TYPE <= pktType), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_DISABLE > enable || YT_ENABLE < enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->ctrlpkt_sa_learn_set(unit, port, pktType, enable);
}

yt_ret_t yt_ctrlpkt_sa_learn_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_CTRL_PKT_ARP_TYPE > pktType || YT_CTRL_PKT_END_TYPE <= pktType), CMM_ERR_INPUT);
    CMM_PARAM_CHK(pEnable == NULL, CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->ctrlpkt_sa_learn_get(unit, port, pktType, pEnable);
}

yt_ret_t yt_ctrlpkt_parse_en_set(yt_unit_t unit, yt_ctrlpkt_parse_type_t parseType, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_CTRL_PKT_PARSE_ARP_TYPE > parseType || YT_CTRL_PKT_PARSE_END_TYPE <= parseType), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_DISABLE > enable || YT_ENABLE < enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->ctrlpkt_parse_en_set(unit, parseType, enable);
}

yt_ret_t yt_ctrlpkt_parse_en_get(yt_unit_t unit, yt_ctrlpkt_parse_type_t parseType, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_CTRL_PKT_PARSE_ARP_TYPE > parseType || YT_CTRL_PKT_PARSE_END_TYPE <= parseType), CMM_ERR_INPUT);
    CMM_PARAM_CHK(pEnable == NULL, CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->ctrlpkt_parse_en_get(unit, parseType, pEnable);
}

yt_ret_t yt_ctrlpkt_rrpp_parse_set(yt_unit_t unit, yt_ctrlpkt_rrpp_parse_t *pParseConf)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(pParseConf == NULL, CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->ctrlpkt_rrpp_parse_set(unit, pParseConf);
}

yt_ret_t  yt_ctrlpkt_rrpp_parse_get(yt_unit_t unit, yt_ctrlpkt_rrpp_parse_t *pParseConf)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(pParseConf == NULL, CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->ctrlpkt_rrpp_parse_get(unit, pParseConf);
}

