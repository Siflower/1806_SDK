/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_ctrlpkt.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_CTRLPKT_H
#define __FAL_TIGER_CTRLPKT_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "fal_cmm.h"

/**
 * @internal      fal_tiger_ctrlpkt_act_mask_get
 * @endinternal
 *
 * @brief         get act mask based on drop & copy mask
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     macid               -mac index
 * @param[in]     actType             -packet action: FWD/TRAP/DROP/COPY
 * @param[out]    pCopyMask           -copy mask
 * @param[out]    pDropMask           -drop mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 * @retval        CMM_ERR_NOT_SUPPORT -current action type not support
 */
extern yt_ret_t fal_tiger_ctrlpkt_act_mask_get(yt_macid_t macid, yt_act_type_t actType,
    uint32_t *pCopyMask, uint32_t *pDropMask);

/**
 * @internal      fal_tiger_ctrlpkt_act_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pktType             -packet type: ARP/ND/LLDP/ERPS/RRPP/IGMP/MLD/PIM
 * @param[in]     actType             -packet action: FWD/TRAP/DROP/COPY
 * @retval        CMM_ERR_NOT_SUPPORT -not support pkt type
 */
extern yt_ret_t fal_tiger_ctrlpkt_act_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_act_type_t actType);

/**
 * @internal      fal_tiger_ctrlpkt_act_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pktType             -packet type: ARP/ND/LLDP/ERPS/RRPP/IGMP/MLD/PIM
 * @param[out]    pActType            -packet action: FWD/TRAP/DROP/COPY
 * @retval        CMM_ERR_NOT_SUPPORT -not support pkt type
 */
extern yt_ret_t fal_tiger_ctrlpkt_act_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_act_type_t *pActType);

/**
 * @internal      fal_tiger_ctrlpkt_myMac_act_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     pActConf            -action config data struct
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_NULL_POINT  -null pointer
 */
extern yt_ret_t fal_tiger_ctrlpkt_myMac_act_set(yt_unit_t unit, yt_ctrlpkt_myMac_act_t *pActConf);

/**
 * @internal      fal_tiger_ctrlpkt_myMac_act_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pActConf            -action config data struct
 * @retval        CMM_ERR_NOT_SUPPORT -not support pkt type
 */
extern yt_ret_t fal_tiger_ctrlpkt_myMac_act_get(yt_unit_t unit, yt_ctrlpkt_myMac_act_t *pActConf);

/**
 * @internal      fal_tiger_ctrlpkt_bypass_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pktType             -packet type: ARP/ND/LLDP/ERPS/RRPP/IGMP/MLD/PIM
 * @param[in]     pBypassConf         -config data
 * @retval        CMM_ERR_NOT_SUPPORT -not support pkt type
 */
extern yt_ret_t fal_tiger_ctrlpkt_bypass_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_ctrlpkt_bypass_t *pBypassConf);

/**
 * @internal      fal_tiger_ctrlpkt_bypass_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pktType             -packet type: ARP/ND/LLDP/ERPS/RRPP/IGMP/MLD/PIM
 * @param[out]    pBypassConf         -config data
 * @retval        CMM_ERR_NOT_SUPPORT -not support pkt type
 */
extern yt_ret_t fal_tiger_ctrlpkt_bypass_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_ctrlpkt_bypass_t *pBypassConf);

/**
 * @internal      fal_tiger_ctrlpkt_bypass_stp_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     pktType             -packet type: ARP/ND/LLDP/ERPS/RRPP/IGMP/MLD
 * @param[in]     enable              -YT_ENABLE/YT_DISABLE
 * @retval        CMM_ERR_NOT_SUPPORT -not support pkt type
 */
extern yt_ret_t fal_tiger_ctrlpkt_bypass_stp_set(yt_unit_t unit, yt_ctrlpkt_type_t pktType, yt_enable_t enable);

/**
 * @internal      fal_tiger_ctrlpkt_bypass_stp_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     pktType             -packet type: ARP/ND/LLDP/ERPS/RRPP/IGMP/MLD
 * @param[out]    pEnable             -YT_ENABLE/YT_DISABLE
 * @retval        CMM_ERR_NOT_SUPPORT -not support pkt type
 */
extern yt_ret_t fal_tiger_ctrlpkt_bypass_stp_get(yt_unit_t unit, yt_ctrlpkt_type_t pktType, yt_enable_t *pEnable);

/**
 * @internal      fal_tiger_ctrlpkt_sa_learn_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pktType             -packet type: ARP/ND/LLDP/ERPS/RRPP/IGMP/MLD
 * @param[in]     enable              -YT_ENABLE/YT_DISABLE
 * @retval        CMM_ERR_NOT_SUPPORT -not support pkt type
 */
extern yt_ret_t fal_tiger_ctrlpkt_sa_learn_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_enable_t enable);

/**
 * @internal      fal_tiger_ctrlpkt_sa_learn_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pktType             -packet type: ARP/ND/LLDP/ERPS/RRPP/IGMP/MLD
 * @param[out]    pEnable             -YT_ENABLE/YT_DISABLE
 * @retval        CMM_ERR_NOT_SUPPORT -not support pkt type
 */
extern yt_ret_t fal_tiger_ctrlpkt_sa_learn_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_enable_t *pEnable);

/**
 * @internal      fal_tiger_ctrlpkt_parse_en_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     parseType           -ARP|PPPOE
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_NOT_SUPPORT -not support pkt type
 */
extern yt_ret_t fal_tiger_ctrlpkt_parse_en_set(yt_unit_t unit, yt_ctrlpkt_parse_type_t parseType, yt_enable_t enable);

/**
 * @internal      fal_tiger_ctrlpkt_parse_en_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     parseType           -ARP|PPPOE
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_NOT_SUPPORT -not support pkt type
 */
extern yt_ret_t fal_tiger_ctrlpkt_parse_en_get(yt_unit_t unit, yt_ctrlpkt_parse_type_t parseType, yt_enable_t *pEnable);

/**
 * @internal      fal_tiger_ctrlpkt_rrpp_parse_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     pEigenConf           -x
 * @retval        CMM_ERR_NOT_SUPPORT -not support pkt type
 */
extern yt_ret_t fal_tiger_ctrlpkt_rrpp_parse_set(yt_unit_t unit, yt_ctrlpkt_rrpp_parse_t *pParseConf);

/**
 * @internal      fal_tiger_ctrlpkt_rrpp_parse_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pEigenConf          -x
 * @retval        CMM_ERR_NOT_SUPPORT -not support pkt type
 */
extern yt_ret_t fal_tiger_ctrlpkt_rrpp_parse_get(yt_unit_t unit, yt_ctrlpkt_rrpp_parse_t *pParseConf);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
