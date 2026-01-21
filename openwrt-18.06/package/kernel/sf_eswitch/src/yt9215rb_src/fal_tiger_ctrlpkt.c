/******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
*******************************************************************************
*  File Name     : fal_tiger_ctrlpkt.c
*  Version       : Initial Draft
*  Created       : 2022/3/1
*  Last Modified :
*  Description   : To get set control packet action
*
******************************************************************************/
/**************************************************
 *      Include header files                       *
 **************************************************/
#include "fal_tiger_ctrlpkt.h"
#include "yt_error.h"
#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"
/**************************************************
 *      Constants or macros Declaration            *
 **************************************************/

/**************************************************
 *      Global variables                           *
 **************************************************/

/**************************************************
 *      Functions Declaration                      *
 **************************************************/



/**************************************************
 *      Functions implementations                  *
 **************************************************/
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
yt_ret_t fal_tiger_ctrlpkt_act_mask_get(yt_macid_t macid, yt_act_type_t actType,
    uint32_t *pCopyMask, uint32_t *pDropMask)
{
    CMM_PARAM_CHK(pDropMask == NULL || pCopyMask == NULL, CMM_ERR_NULL_POINT);
    switch(actType)
    {
        case ACT_TYPE_FWD:
            *pCopyMask = (uint32_t)CLR_FIELD((*pCopyMask), macid, 1);
            *pDropMask = (uint32_t)CLR_FIELD((*pDropMask), macid, 1);
            break;

        case ACT_TYPE_TRAP:
            *pCopyMask |= (uint32_t)(1UL<<macid);
            *pDropMask |= (uint32_t)(1UL<<macid);
            break;

        case ACT_TYPE_DROP:
            *pCopyMask = (uint32_t)CLR_FIELD((*pCopyMask), macid, 1);
            *pDropMask |= (uint32_t)(1UL<<macid);
            break;

        case ACT_TYPE_COPY:
            *pCopyMask |= (uint32_t)(1UL<<macid);
            *pDropMask = (uint32_t)CLR_FIELD((*pDropMask), macid, 1);;
            break;

        default:
            return CMM_ERR_NOT_SUPPORT;
    }

    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_ctrlpkt_act_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pktType             -packet type: ARP/ND/LLDP/LLDP_EEE
 * @param[in]     actType             -packet action: FWD/TRAP/DROP/COPY
 * @retval        CMM_ERR_NOT_SUPPORT -not support pkt type
 */
yt_ret_t fal_tiger_ctrlpkt_act_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_act_type_t actType)
{
    l2_arp_bcast_per_port_ctrl_t arpEntry;
    l2_nd_per_port_ctrl_t ndEntry;
    l2_lldp_eee_per_port_ctrl_t lldpEeeEntry;
    l2_lldp_per_port_ctrl_t lldpEntry;
    yt_macid_t macid;
    uint32_t copyMask = 0;
    uint32_t dropMask = 0;
    cmm_err_t ret = CMM_ERR_OK;

    macid = CAL_YTP_TO_MAC(unit,port);
    switch (pktType)
    {
        case YT_CTRL_PKT_ARP_TYPE:
            osal_memset(&arpEntry, sizeof(l2_arp_bcast_per_port_ctrl_t), 0, sizeof(l2_arp_bcast_per_port_ctrl_t));
            CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_ARP_BCAST_PER_PORT_CTRLm, 0, sizeof(l2_arp_bcast_per_port_ctrl_t), &arpEntry), ret);
            HAL_FIELD_GET(L2_ARP_BCAST_PER_PORT_CTRLm, L2_ARP_BCAST_PER_PORT_CTRL_COPY_TO_CPUf, &arpEntry, &copyMask);
            HAL_FIELD_GET(L2_ARP_BCAST_PER_PORT_CTRLm, L2_ARP_BCAST_PER_PORT_CTRL_DROPf, &arpEntry, &dropMask);
            CMM_ERR_CHK(fal_tiger_ctrlpkt_act_mask_get(macid, actType, &copyMask, &dropMask), ret);
            HAL_FIELD_SET(L2_ARP_BCAST_PER_PORT_CTRLm, L2_ARP_BCAST_PER_PORT_CTRL_COPY_TO_CPUf, &arpEntry, copyMask);
            HAL_FIELD_SET(L2_ARP_BCAST_PER_PORT_CTRLm, L2_ARP_BCAST_PER_PORT_CTRL_DROPf, &arpEntry, dropMask);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_ARP_BCAST_PER_PORT_CTRLm, 0, sizeof(l2_arp_bcast_per_port_ctrl_t), &arpEntry), ret);  
            break;

        case YT_CTRL_PKT_ND_TYPE:
            osal_memset(&ndEntry, sizeof(l2_nd_per_port_ctrl_t), 0, sizeof(l2_nd_per_port_ctrl_t));
            CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_ND_PER_PORT_CTRLm, 0, sizeof(l2_nd_per_port_ctrl_t), &ndEntry), ret);
            HAL_FIELD_GET(L2_ND_PER_PORT_CTRLm, L2_ND_PER_PORT_CTRL_COPY_TO_CPUf, &ndEntry, &copyMask);
            HAL_FIELD_GET(L2_ND_PER_PORT_CTRLm, L2_ND_PER_PORT_CTRL_DROPf, &ndEntry, &dropMask);
            CMM_ERR_CHK(fal_tiger_ctrlpkt_act_mask_get(macid, actType, &copyMask, &dropMask), ret);
            HAL_FIELD_SET(L2_ND_PER_PORT_CTRLm, L2_ND_PER_PORT_CTRL_COPY_TO_CPUf, &ndEntry, copyMask);
            HAL_FIELD_SET(L2_ND_PER_PORT_CTRLm, L2_ND_PER_PORT_CTRL_DROPf, &ndEntry, dropMask);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_ND_PER_PORT_CTRLm, 0, sizeof(l2_nd_per_port_ctrl_t), &ndEntry), ret);
            break;

        case YT_CTRL_PKT_LLDP_TYPE:
            osal_memset(&lldpEntry, sizeof(l2_lldp_per_port_ctrl_t), 0, sizeof(l2_lldp_per_port_ctrl_t));
            CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LLDP_PER_PORT_CTRLm, 0, sizeof(l2_lldp_per_port_ctrl_t), &lldpEntry), ret);
            HAL_FIELD_GET(L2_LLDP_PER_PORT_CTRLm, L2_LLDP_PER_PORT_CTRL_COPY_TO_CPUf, &lldpEntry, &copyMask);
            HAL_FIELD_GET(L2_LLDP_PER_PORT_CTRLm, L2_LLDP_PER_PORT_CTRL_DROPf, &lldpEntry, &dropMask);
            CMM_ERR_CHK(fal_tiger_ctrlpkt_act_mask_get(macid, actType, &copyMask, &dropMask), ret);
            HAL_FIELD_SET(L2_LLDP_PER_PORT_CTRLm, L2_LLDP_PER_PORT_CTRL_COPY_TO_CPUf, &lldpEntry, copyMask);
            HAL_FIELD_SET(L2_LLDP_PER_PORT_CTRLm, L2_LLDP_PER_PORT_CTRL_DROPf, &lldpEntry, dropMask);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_LLDP_PER_PORT_CTRLm, 0, sizeof(l2_lldp_per_port_ctrl_t), &lldpEntry), ret);
            break;

        case YT_CTRL_PKT_LLDP_EEE_TYPE:
            osal_memset(&lldpEeeEntry, sizeof(l2_lldp_eee_per_port_ctrl_t), 0, sizeof(l2_lldp_eee_per_port_ctrl_t));
            CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LLDP_EEE_PER_PORT_CTRLm, 0, sizeof(l2_lldp_eee_per_port_ctrl_t), &lldpEeeEntry), ret);
            HAL_FIELD_GET(L2_LLDP_EEE_PER_PORT_CTRLm, L2_LLDP_EEE_PER_PORT_CTRL_COPY_TO_CPUf, &lldpEeeEntry, &copyMask);
            HAL_FIELD_GET(L2_LLDP_EEE_PER_PORT_CTRLm, L2_LLDP_EEE_PER_PORT_CTRL_DROPf, &lldpEeeEntry, &dropMask);
            CMM_ERR_CHK(fal_tiger_ctrlpkt_act_mask_get(macid, actType, &copyMask, &dropMask), ret);
            HAL_FIELD_SET(L2_LLDP_EEE_PER_PORT_CTRLm, L2_LLDP_EEE_PER_PORT_CTRL_COPY_TO_CPUf, &lldpEeeEntry, copyMask);
            HAL_FIELD_SET(L2_LLDP_EEE_PER_PORT_CTRLm, L2_LLDP_EEE_PER_PORT_CTRL_DROPf, &lldpEeeEntry, dropMask);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_LLDP_EEE_PER_PORT_CTRLm, 0, sizeof(l2_lldp_eee_per_port_ctrl_t), &lldpEeeEntry), ret);
            break;

        default:
            return CMM_ERR_NOT_SUPPORT;
    }

    return CMM_ERR_OK;
}

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
yt_ret_t fal_tiger_ctrlpkt_act_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_act_type_t *pActType)
{
    l2_arp_bcast_per_port_ctrl_t arpEntry;
    l2_nd_per_port_ctrl_t ndEntry;
    l2_lldp_eee_per_port_ctrl_t lldpEeeEntry;
    l2_lldp_per_port_ctrl_t lldpEntry;
    yt_macid_t macid;
    uint32_t copyMask = 0;
    uint32_t dropMask = 0;
    cmm_err_t ret = CMM_ERR_OK;

    macid = CAL_YTP_TO_MAC(unit,port);
    switch (pktType)
    {
        case YT_CTRL_PKT_ARP_TYPE:
            osal_memset(&arpEntry, sizeof(l2_arp_bcast_per_port_ctrl_t), 0, sizeof(l2_arp_bcast_per_port_ctrl_t));
            CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_ARP_BCAST_PER_PORT_CTRLm, 0, sizeof(l2_arp_bcast_per_port_ctrl_t), &arpEntry), ret);
            HAL_FIELD_GET(L2_ARP_BCAST_PER_PORT_CTRLm, L2_ARP_BCAST_PER_PORT_CTRL_COPY_TO_CPUf, &arpEntry, &copyMask);
            HAL_FIELD_GET(L2_ARP_BCAST_PER_PORT_CTRLm, L2_ARP_BCAST_PER_PORT_CTRL_DROPf, &arpEntry, &dropMask);
            break;

        case YT_CTRL_PKT_ND_TYPE:
            osal_memset(&ndEntry, sizeof(l2_nd_per_port_ctrl_t), 0, sizeof(l2_nd_per_port_ctrl_t));
            CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_ND_PER_PORT_CTRLm, 0, sizeof(l2_nd_per_port_ctrl_t), &ndEntry), ret);
            HAL_FIELD_GET(L2_ND_PER_PORT_CTRLm, L2_ND_PER_PORT_CTRL_COPY_TO_CPUf, &ndEntry, &copyMask);
            HAL_FIELD_GET(L2_ND_PER_PORT_CTRLm, L2_ND_PER_PORT_CTRL_DROPf, &ndEntry, &dropMask);
            break;

        case YT_CTRL_PKT_LLDP_TYPE:
            osal_memset(&lldpEntry, sizeof(l2_lldp_per_port_ctrl_t), 0, sizeof(l2_lldp_per_port_ctrl_t));
            CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LLDP_PER_PORT_CTRLm, 0, sizeof(l2_lldp_per_port_ctrl_t), &lldpEntry), ret);
            HAL_FIELD_GET(L2_LLDP_PER_PORT_CTRLm, L2_LLDP_PER_PORT_CTRL_COPY_TO_CPUf, &lldpEntry, &copyMask);
            HAL_FIELD_GET(L2_LLDP_PER_PORT_CTRLm, L2_LLDP_PER_PORT_CTRL_DROPf, &lldpEntry, &dropMask);
            break;

        case YT_CTRL_PKT_LLDP_EEE_TYPE:
            osal_memset(&lldpEeeEntry, sizeof(l2_lldp_eee_per_port_ctrl_t), 0, sizeof(l2_lldp_eee_per_port_ctrl_t));
            CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LLDP_EEE_PER_PORT_CTRLm, 0, sizeof(l2_lldp_eee_per_port_ctrl_t), &lldpEeeEntry), ret);
            HAL_FIELD_GET(L2_LLDP_EEE_PER_PORT_CTRLm, L2_LLDP_EEE_PER_PORT_CTRL_COPY_TO_CPUf, &lldpEeeEntry, &copyMask);
            HAL_FIELD_GET(L2_LLDP_EEE_PER_PORT_CTRLm, L2_LLDP_EEE_PER_PORT_CTRL_DROPf, &lldpEeeEntry, &dropMask);
            break;

        default:
            return CMM_ERR_NOT_SUPPORT;
    }

    if (copyMask & (1UL<<macid))
    {
        if (dropMask & (1UL<<macid))
        {
            *pActType = ACT_TYPE_TRAP;
        }
        else
        {
            *pActType = ACT_TYPE_COPY;
        }
    }
    else
    {
        if (dropMask & (1UL<<macid))
        {
            *pActType = ACT_TYPE_DROP;
        }
        else
        {
            *pActType = ACT_TYPE_FWD;
        }
    }

    return CMM_ERR_OK;
}

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
yt_ret_t fal_tiger_ctrlpkt_myMac_act_set(yt_unit_t unit, yt_ctrlpkt_myMac_act_t *pActConf)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pActConf);
    return CMM_ERR_NOT_SUPPORT;
}

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
yt_ret_t fal_tiger_ctrlpkt_myMac_act_get(yt_unit_t unit, yt_ctrlpkt_myMac_act_t *pActConf)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pActConf);
    return CMM_ERR_NOT_SUPPORT;
}

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
yt_ret_t fal_tiger_ctrlpkt_bypass_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_ctrlpkt_bypass_t *pBypassConf)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(pktType);
    CMM_UNUSED_PARAM(pBypassConf);
    return CMM_ERR_NOT_SUPPORT;
}

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
yt_ret_t fal_tiger_ctrlpkt_bypass_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_ctrlpkt_bypass_t *pBypassConf)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(pktType);
    CMM_UNUSED_PARAM(pBypassConf);
    return CMM_ERR_NOT_SUPPORT;
}

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
yt_ret_t fal_tiger_ctrlpkt_bypass_stp_set(yt_unit_t unit, yt_ctrlpkt_type_t pktType, yt_enable_t enable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pktType);
    CMM_UNUSED_PARAM(enable);
    return CMM_ERR_NOT_SUPPORT;
}

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
yt_ret_t fal_tiger_ctrlpkt_bypass_stp_get(yt_unit_t unit, yt_ctrlpkt_type_t pktType, yt_enable_t *pEnable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pktType);
    CMM_UNUSED_PARAM(pEnable);
    return CMM_ERR_NOT_SUPPORT;
}

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
yt_ret_t fal_tiger_ctrlpkt_sa_learn_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_enable_t enable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(pktType);
    CMM_UNUSED_PARAM(enable);
    return CMM_ERR_NOT_SUPPORT;
}

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
yt_ret_t fal_tiger_ctrlpkt_sa_learn_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_type_t pktType,
    yt_enable_t *pEnable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(pktType);
    CMM_UNUSED_PARAM(pEnable);
    return CMM_ERR_NOT_SUPPORT;
}

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
yt_ret_t fal_tiger_ctrlpkt_parse_en_set(yt_unit_t unit, yt_ctrlpkt_parse_type_t parseType, yt_enable_t enable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(parseType);
    CMM_UNUSED_PARAM(enable);
    return CMM_ERR_NOT_SUPPORT;
}

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
yt_ret_t fal_tiger_ctrlpkt_parse_en_get(yt_unit_t unit, yt_ctrlpkt_parse_type_t parseType, yt_enable_t *pEnable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(parseType);
    CMM_UNUSED_PARAM(pEnable);
    return CMM_ERR_NOT_SUPPORT;
}

/**
 * @internal      fal_tiger_ctrlpkt_rrpp_parse_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     pParseConf           -x
 * @retval        CMM_ERR_NOT_SUPPORT -not support pkt type
 */
yt_ret_t fal_tiger_ctrlpkt_rrpp_parse_set(yt_unit_t unit, yt_ctrlpkt_rrpp_parse_t *pParseConf)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pParseConf);
    return CMM_ERR_NOT_SUPPORT;
}

/**
 * @internal      fal_tiger_ctrlpkt_rrpp_parse_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pParseConf          -x
 * @retval        CMM_ERR_NOT_SUPPORT -not support pkt type
 */
yt_ret_t fal_tiger_ctrlpkt_rrpp_parse_get(yt_unit_t unit, yt_ctrlpkt_rrpp_parse_t *pParseConf)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pParseConf);
    return CMM_ERR_NOT_SUPPORT;
}

