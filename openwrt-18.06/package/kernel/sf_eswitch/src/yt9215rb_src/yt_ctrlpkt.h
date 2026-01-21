/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_ctrlpkt.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_CTRLPKT_H
#define __YT_CTRLPKT_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "yt_cmm.h"

#define YT_CTRL_PKT_BYPASS_VLAN_BIT         BIT(0)
#define YT_CTRL_PKT_BYPASS_PORT_ISO_BIT     BIT(1)
#define YT_CTRL_PKT_BYPASS_STORM_BIT        BIT(2)
#define YT_CTRL_PKT_BYPASS_PORT_METER_BIT   BIT(3)
#define YT_CTRL_PKT_BYPASS_FLOW_METER_BIT   BIT(4)
#define YT_CTRL_PKT_BYPASS_ALL              (YT_CTRL_PKT_BYPASS_VLAN_BIT | YT_CTRL_PKT_BYPASS_PORT_ISO_BIT \
                                            | YT_CTRL_PKT_BYPASS_STORM_BIT | YT_CTRL_PKT_BYPASS_PORT_METER_BIT \
                                            | YT_CTRL_PKT_BYPASS_FLOW_METER_BIT)

typedef enum yt_ctrlpkt_l2_action_e
{
    L2_ACTION_FWD = 0,
    L2_ACTION_TRAP,
    L2_ACTION_DROP,
    L2_ACTION_COPY,
    L2_ACTION_END
}yt_ctrlpkt_l2_action_t;

typedef enum yt_ctrlpkt_type_e
{
    YT_CTRL_PKT_ARP_TYPE = 0,
    YT_CTRL_PKT_ND_TYPE,
    YT_CTRL_PKT_LLDP_TYPE,
#ifdef SWITCH_SERIES_TIGER
    YT_CTRL_PKT_LLDP_EEE_TYPE,
#endif
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    YT_CTRL_PKT_ERPS_TYPE,
    YT_CTRL_PKT_RRPP_TYPE,
    YT_CTRL_PKT_PIM_TYPE,
    YT_CTRL_PKT_IGMP_TYPE,
    YT_CTRL_PKT_MLD_TYPE,
#endif
    YT_CTRL_PKT_END_TYPE,
}yt_ctrlpkt_type_t;

typedef enum yt_ctrlpkt_parse_type_e
{
    YT_CTRL_PKT_PARSE_ARP_TYPE,
    YT_CTRL_PKT_PARSE_PPPOE_TYPE,
    YT_CTRL_PKT_PARSE_END_TYPE
}yt_ctrlpkt_parse_type_t;

typedef struct yt_ctrlpkt_bypass_s
{
    yt_enable_t bypass_vlan;
    yt_enable_t bypass_port_isolation;
    yt_enable_t bypass_storm;
    yt_enable_t bypass_port_meter;
    yt_enable_t bypass_flow_meter;
    uint32_t flag;
}yt_ctrlpkt_bypass_t;

typedef struct yt_ctrlpkt_myMac_act_s
{
    yt_act_type_t action;
    uint16_t mgmtVid;
    yt_bool_t valid;
}yt_ctrlpkt_myMac_act_t;

typedef struct yt_ctrlpkt_rrpp_parse_s
{
    uint16_t etherType;
    uint32_t oui;
}yt_ctrlpkt_rrpp_parse_t;

/**
 * @internal      yt_ctrlpkt_act_set
 * @endinternal
 *
 * @brief         Set action of ctrl-pkt
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pktType             -packet type: ARP/ND/LLDP/LLDP_EEE/ERPS/RRPP/IGMP/MLD/PIM
 * @param[in]     actType             -packet action: FWD/TRAP/DROP/COPY
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -invalid input
 * @retval        CMM_ERR_PORT        -invalid port
 * @retval        CMM_ERR_NOT_INIT    -module not init
 */
extern yt_ret_t yt_ctrlpkt_act_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_act_type_t actType);

/**
 * @internal      yt_ctrlpkt_act_get
 * @endinternal
 *
 * @brief         Get action of ctrl-pkt
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pktType             -packet type: ARP/ND/LLDP/ERPS/RRPP/IGMP/MLD/PIM
 * @param[out]    pActType            -packet action: FWD/TRAP/DROP/COPY
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -invalid input
 * @retval        CMM_ERR_PORT        -invalid port
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 * @retval        CMM_ERR_NOT_INIT    -module not init
 */
extern yt_ret_t yt_ctrlpkt_act_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_act_type_t *pActType);

/**
 * @internal      yt_ctrlpkt_myMac_act_set
 * @endinternal
 *
 * @brief         Set action of my mac packet
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     pActConf            -action config data struct
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_NOT_INIT    -module not init
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 */
extern yt_ret_t yt_ctrlpkt_myMac_act_set(yt_unit_t unit, yt_ctrlpkt_myMac_act_t *pActConf);

/**
 * @internal      yt_ctrlpkt_myMac_act_get
 * @endinternal
 *
 * @brief         Get action of my mac packet
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pActConf            -action config data struct
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_NOT_INIT    -module not init
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 */
extern yt_ret_t yt_ctrlpkt_myMac_act_get(yt_unit_t unit, yt_ctrlpkt_myMac_act_t *pActConf);

/**
 * @internal      yt_ctrlpkt_bypass_set
 * @endinternal
 *
 * @brief         Set bypass state of ctrl-pkt
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pktType             -packet type: ARP/ND/LLDP/ERPS/RRPP/IGMP/MLD/PIM
 * @param[in]     pBypassConf         -config data
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -invalid input
 * @retval        CMM_ERR_PORT        -invalid port
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 * @retval        CMM_ERR_NOT_INIT    -module not init
 */
extern yt_ret_t yt_ctrlpkt_bypass_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_ctrlpkt_bypass_t *pBypassConf);

/**
 * @internal      yt_ctrlpkt_bypass_get
 * @endinternal
 *
 * @brief         Get bypass state of ctrl-pkt
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pktType             -packet type: ARP/ND/LLDP/ERPS/RRPP/IGMP/MLD/PIM
 * @param[out]    pBypassConf         -config data
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -invalid input
 * @retval        CMM_ERR_PORT        -invalid port
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 * @retval        CMM_ERR_NOT_INIT    -module not init
 */
extern yt_ret_t yt_ctrlpkt_bypass_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_ctrlpkt_bypass_t *pBypassConf);

/**
 * @internal      yt_ctrlpkt_bypass_stp_set
 * @endinternal
 *
 * @brief         Set stp bypass state of ctrl-pkt
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pktType             -packet type: ARP/ND/LLDP/ERPS/RRPP/IGMP/MLD/PIM
 * @param[in]     enable              -YT_ENABLE/YT_DISABLE
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -invalid input
 * @retval        CMM_ERR_NOT_INIT    -module not init
 */
extern yt_ret_t yt_ctrlpkt_bypass_stp_set(yt_unit_t unit, yt_ctrlpkt_type_t pktType,
    yt_enable_t enable);

/**
 * @internal      yt_ctrlpkt_bypass_stp_get
 * @endinternal
 *
 * @brief         Get stp bypass state of ctrl-pkt
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pktType             -packet type: ARP/ND/LLDP/ERPS/RRPP/IGMP/MLD/PIM
 * @param[out]    pEnable             -YT_ENABLE/YT_DISABLE
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 * @retval        CMM_ERR_NOT_INIT    -module not init
 */
extern yt_ret_t yt_ctrlpkt_bypass_stp_get(yt_unit_t unit, yt_ctrlpkt_type_t pktType,
    yt_enable_t *pEnable);

/**
 * @internal      yt_ctrlpkt_sa_learn_set
 * @endinternal
 *
 * @brief         Set sa-learning state of ctrl-pkt
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pktType             -packet type: ARP/ND/LLDP/ERPS/RRPP/IGMP/MLD/PIM
 * @param[in]     enable              -YT_ENABLE/YT_DISABLE
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -invalid input
 * @retval        CMM_ERR_PORT        -invalid port
 * @retval        CMM_ERR_NOT_INIT    -module not init
 */
extern yt_ret_t yt_ctrlpkt_sa_learn_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_enable_t enable);

/**
 * @internal      yt_ctrlpkt_sa_learn_get
 * @endinternal
 *
 * @brief         Get sa-learning state of ctrl-pkt
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pktType             -packet type: ARP/ND/LLDP/ERPS/RRPP/IGMP/MLD/PIM
 * @param[out]    pEnable             -YT_ENABLE/YT_DISABLE
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -invalid input
 * @retval        CMM_ERR_PORT        -invalid port
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 * @retval        CMM_ERR_NOT_INIT    -module not init
 */
extern yt_ret_t yt_ctrlpkt_sa_learn_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_enable_t *pEnable);

/**
 * @internal      yt_ctrlpkt_parse_en_set
 * @endinternal
 *
 * @brief         Set parse state of ctrl-pkt
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     parseType           -parse type: ARP/PPPOE
 * @param[in]     enable              -YT_ENABLE/YT_DISABLE
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -invalid input
 * @retval        CMM_ERR_NOT_INIT    -module not init
 */
extern yt_ret_t yt_ctrlpkt_parse_en_set(yt_unit_t unit, yt_ctrlpkt_parse_type_t parseType, yt_enable_t enable);

/**
 * @internal      yt_ctrlpkt_parse_en_get
 * @endinternal
 *
 * @brief         Get parse state of ctrl-pkt
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     parseType           -parse type: ARP/PPPOE
 * @param[out]    pEnable             -YT_ENABLE/YT_DISABLE
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 * @retval        CMM_ERR_NOT_INIT    -module not init
 */
extern yt_ret_t yt_ctrlpkt_parse_en_get(yt_unit_t unit, yt_ctrlpkt_parse_type_t parseType, yt_enable_t *pEnable);

/**
 * @internal      yt_ctrlpkt_rrpp_parse_set
 * @endinternal
 *
 * @brief         Set parse state of rrpp packets
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     pParseConf          -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 * @retval        CMM_ERR_NOT_INIT    -module not init
 */
extern yt_ret_t yt_ctrlpkt_rrpp_parse_set(yt_unit_t unit, yt_ctrlpkt_rrpp_parse_t *pParseConf);

/**
 * @internal      yt_ctrlpkt_rrpp_parse_get
 * @endinternal
 *
 * @brief         Get parse state of rrpp packets
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pParseConf          -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 * @retval        CMM_ERR_NOT_INIT    -module not init
 */
extern yt_ret_t yt_ctrlpkt_rrpp_parse_get(yt_unit_t unit, yt_ctrlpkt_rrpp_parse_t *pParseConf);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
