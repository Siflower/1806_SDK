/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_port.c
*
* @brief x
*
********************************************************************************
*/
/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_tiger_l2.h"
#include "hal_mem.h"
#include "hal_ctrl.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"
#include "cal_cmm.h"
#include "fal_tiger_port.h"
#include "fal_tiger_cmm.h"
#include "fal_monitor.h"

static yt_ret_t fal_port_speedDup_split(yt_port_speed_duplex_t speed_dup, yt_port_speed_t *pSpeed, yt_port_duplex_t *pDuplex)
{
    switch(speed_dup)
    {
        case PORT_SPEED_DUP_10HALF:
            *pSpeed  = PORT_SPEED_10M;
            *pDuplex = PORT_DUPLEX_HALF;
            break;
        case PORT_SPEED_DUP_10FULL:
            *pSpeed  = PORT_SPEED_10M;
            *pDuplex = PORT_DUPLEX_FULL;
            break;
        case PORT_SPEED_DUP_100HALF:
            *pSpeed  = PORT_SPEED_100M;
            *pDuplex = PORT_DUPLEX_HALF;
            break;
        case PORT_SPEED_DUP_100FULL:
            *pSpeed  = PORT_SPEED_100M;
            *pDuplex = PORT_DUPLEX_FULL;
            break;
        case PORT_SPEED_DUP_1000FULL:
            *pSpeed  = PORT_SPEED_1000M;
            *pDuplex = PORT_DUPLEX_FULL;
            break;
        case PORT_SPEED_DUP_2500FULL:
            *pSpeed = PORT_SPEED_2500M;
            *pDuplex = PORT_DUPLEX_FULL;
            break;
        default:
            return CMM_ERR_INPUT;
    }

    return CMM_ERR_OK;
}

static yt_ret_t fal_port_speedDup_combine(yt_port_speed_t speed, yt_port_duplex_t duplex, yt_port_speed_duplex_t *pSpeedDup)
{
    if(duplex == PORT_DUPLEX_FULL)
    {
        if(speed == PORT_SPEED_10M)
        {
            *pSpeedDup = PORT_SPEED_DUP_10FULL;
        }
        else if(speed == PORT_SPEED_100M)
        {
            *pSpeedDup = PORT_SPEED_DUP_100FULL;
        }
        else if(speed == PORT_SPEED_1000M)
        {
            *pSpeedDup = PORT_SPEED_DUP_1000FULL;
        }
        else if(speed == PORT_SPEED_2500M)
        {
            *pSpeedDup = PORT_SPEED_DUP_2500FULL;
        }
        else
        {
            return CMM_ERR_INPUT;
        }
    }
    else if(duplex == PORT_DUPLEX_HALF)
    {
        if(speed == PORT_SPEED_10M)
        {
            *pSpeedDup = PORT_SPEED_DUP_10HALF;
        }
        else if(speed == PORT_SPEED_100M)
        {
            *pSpeedDup = PORT_SPEED_DUP_100HALF;
        }
        else
        {
            return CMM_ERR_INPUT;
        }
    }
    else
    {
        return CMM_ERR_INPUT;
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_init(yt_unit_t unit)
{
    global_ctrl1_t global_ctrl_tbl;
    yt_port_t port;
    yt_macid_t mac_id;
    cmm_err_t ret = CMM_ERR_OK;
    /* set ac drop global state enable */
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, GLOBAL_CTRL1m, 0, sizeof(global_ctrl1_t), &global_ctrl_tbl), ret);
    HAL_FIELD_SET(GLOBAL_CTRL1m, GLOBAL_CTRL1_AC_ENf, &global_ctrl_tbl, YT_ENABLE);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, GLOBAL_CTRL1m, 0, sizeof(global_ctrl1_t), &global_ctrl_tbl), ret);

    for(port = 0; port < CAL_PORT_NUM_ON_UNIT(unit); port++)
    {
        mac_id = CAL_YTP_TO_MAC(unit, port);
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            HALPHYDRV_FUNC(unit, mac_id)->phy_init(unit);
        }
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
    port_ctrl_t port_ctrl;
    yt_port_t phy_addr;
    yt_macid_t mac_id;
    yt_enable_t orgEnable;
    uint32_t regData;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    if(YT_ENABLE == enable  &&
        CAL_PORT_TYPE_INTPHY == CAL_YTP_PORT_TYPE(unit, port))
    {
        /*enable phy*/
        HAL_MEM_DIRECT_READ(unit, 0x8002c, &regData);
        if(!(regData & (1<<(mac_id+8))))
        {
            regData = regData | (1<<(mac_id+8)) | 0xff;
            HAL_MEM_DIRECT_WRITE(unit, 0x8002c, regData);
        }
    }

    /* if no change, do nothing */
    fal_tiger_port_enable_get(unit, port, &orgEnable);
    if(orgEnable == enable)
    {
        return CMM_ERR_OK;
    }

    CMM_PARAM_CHK(HAL_TBL_REG_READ(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl), ret);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_AN_LINK_ENf, &port_ctrl, enable);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_TXMAC_ENf, &port_ctrl, enable);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_RXMAC_ENf, &port_ctrl, enable);
    CMM_PARAM_CHK(HAL_TBL_REG_WRITE(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl), ret);

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            HALPHYDRV_FUNC(unit, mac_id)->phy_enable_set(unit, phy_addr, enable);
        }
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    cmm_err_t ret = CMM_ERR_OK;
    port_ctrl_t port_ctrl;
    yt_port_t phy_addr;
    yt_macid_t mac_id;
    uint32_t an_en;
    uint32_t tx_en;
    uint32_t rx_en;
    yt_enable_t phy_en = YT_ENABLE;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    CMM_PARAM_CHK(HAL_TBL_REG_READ(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl), ret);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_AN_LINK_ENf, &port_ctrl, &an_en);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_TXMAC_ENf, &port_ctrl, &tx_en);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_RXMAC_ENf, &port_ctrl, &rx_en);

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            HALPHYDRV_FUNC(unit, mac_id)->phy_enable_get(unit, phy_addr, &phy_en);
        }
    }

    if((an_en || tx_en || rx_en) && phy_en)
    {
        *pEnable = YT_ENABLE;
    }
    else
    {
        *pEnable = YT_DISABLE;
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_link_status_get(yt_unit_t unit, yt_port_t port, yt_port_link_status_t *pLinkStatus)
{
    cmm_err_t ret = CMM_ERR_OK;
    port_status_t   status;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit,port);
    osal_memset(&status, 0, sizeof(port_status_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_STATUSm, mac_id, sizeof(port_status_t), &status), ret);
    HAL_FIELD_GET(PORT_STATUSm, PORT_STATUS_LINKf, &status, pLinkStatus);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_link_status_all_get(yt_unit_t unit, yt_port_t port, yt_port_linkStatus_all_t *pAllLinkStatus)
{
    cmm_err_t ret = CMM_ERR_OK;
    port_status_t   port_status;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit,port);
    osal_memset(&port_status, 0, sizeof(port_status_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_STATUSm, mac_id, sizeof(port_status_t), &port_status), ret);

    HAL_FIELD_GET(PORT_STATUSm, PORT_STATUS_LINKf, &port_status, &pAllLinkStatus->link_status);
    HAL_FIELD_GET(PORT_STATUSm, PORT_STATUS_SPEED_MODEf, &port_status, &pAllLinkStatus->link_speed);
    HAL_FIELD_GET(PORT_STATUSm, PORT_STATUS_DUPLEX_MODEf, &port_status, &pAllLinkStatus->link_duplex);
    HAL_FIELD_GET(PORT_STATUSm, PORT_STATUS_RX_FC_ENf, &port_status, &pAllLinkStatus->rx_fc_en);
    HAL_FIELD_GET(PORT_STATUSm, PORT_STATUS_TX_FC_ENf, &port_status, &pAllLinkStatus->tx_fc_en);

    /*b100 for mac and sds 2.5g*/
    if((yt_port_speed_mode_t)pAllLinkStatus->link_speed == PORT_SPEED_MODE_2500M)
    {
        pAllLinkStatus->link_speed = PORT_SPEED_2500M;
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_backpress_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_t port_mac_id;
    port_ctrl_t    port_ctrl;

    port_mac_id = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_CTRLm, port_mac_id, sizeof(port_ctrl_t), &port_ctrl), ret);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_HALF_FC_ENf, &port_ctrl, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PORT_CTRLm, port_mac_id, sizeof(port_ctrl_t), &port_ctrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_backpress_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_t port_mac_id;
    port_ctrl_t    port_ctrl;
    uint32_t half_fc_enable;

    if(NULL == pEnable)
        return CMM_ERR_NULL_POINT;

    port_mac_id = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_CTRLm, port_mac_id, sizeof(port_ctrl_t), &port_ctrl), ret);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_HALF_FC_ENf, &port_ctrl, &half_fc_enable);

    *pEnable = half_fc_enable == 1 ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_cascade_set(yt_unit_t unit, yt_cascade_info_t cascade_info)
{
    cascade_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t mac_id;
    uint32_t field_id;
    uint32_t i;
    loop_detect_act_ctrl_t act_ctrl_entry;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, CASCADE_CTRLm, 0, sizeof(cascade_ctrl_t), &entry), ret);
    HAL_FIELD_SET(CASCADE_CTRLm, CASCADE_CTRL_ENf, &entry, cascade_info.en);
    act_ctrl_entry.entry_data[0] = YT_CASCADE_FWD_PORTMASK;
    for (i = 0; i < YT_MAX_CASCADE_PORT_NUM; i++)
    {
        if (0 == i)
            field_id = CASCADE_CTRL_PORT0f;
        else if (1 == i)
            field_id = CASCADE_CTRL_PORT1f;
        else
            return CMM_ERR_EXCEED_RANGE;

        if (cascade_info.ports[i].valid)
        {
            mac_id = CAL_YTP_TO_MAC(unit, cascade_info.ports[i].port_num);
            CLEAR_BIT(act_ctrl_entry.entry_data[0], mac_id);
        }
        else
        {
            mac_id = YT_INVALID_CASCADE_PORT;
        }
        HAL_FIELD_SET(CASCADE_CTRLm, field_id, &entry, mac_id);
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, CASCADE_CTRLm, 0, sizeof(cascade_ctrl_t), &entry), ret);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, LOOP_DETECT_ACT_CTRLm, 0, sizeof(loop_detect_act_ctrl_t), &act_ctrl_entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_cascade_get(yt_unit_t unit, yt_cascade_info_t *pCascade_info)
{
    cmm_err_t ret = CMM_ERR_OK;
    cascade_ctrl_t entry;
    yt_macid_t mac_id;
    uint32_t field_id;
    uint32_t i;

    if(NULL == pCascade_info)
        return CMM_ERR_NULL_POINT;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, CASCADE_CTRLm, 0, sizeof(cascade_ctrl_t), &entry), ret);
    HAL_FIELD_GET(CASCADE_CTRLm, CASCADE_CTRL_ENf, &entry, &pCascade_info->en);
    for (i = 0; i < YT_MAX_CASCADE_PORT_NUM; i++)
    {
        if (0 == i)
            field_id = CASCADE_CTRL_PORT0f;
        else if (1 == i)
            field_id = CASCADE_CTRL_PORT1f;
        else
            return CMM_ERR_EXCEED_RANGE;

        HAL_FIELD_GET(CASCADE_CTRLm, field_id, &entry, &mac_id);
        if (YT_INVALID_CASCADE_PORT == mac_id)
        {
            pCascade_info->ports[i].valid = 0;
            pCascade_info->ports[i].port_num = YT_INVALID_CASCADE_PORT;
        }
        else
        {
            pCascade_info->ports[i].valid = 1;
            CAL_MAC_TO_YTP(unit, mac_id, pCascade_info->ports[i].port_num);
        }
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_pkt_gap_set(yt_unit_t unit, yt_port_t port, uint8_t gap)
{
    port_rate_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_RATE_CTRLNm, port, sizeof(port_rate_ctrln_t), &entry), ret);
    HAL_FIELD_SET(PORT_RATE_CTRLNm, PORT_RATE_CTRLN_GAP_VALUEf, &entry, gap);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PORT_RATE_CTRLNm, port, sizeof(port_rate_ctrln_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_pkt_gap_get(yt_unit_t unit, yt_port_t port, uint8_t *pGap)
{
    port_rate_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    if(NULL == pGap)
        return CMM_ERR_NULL_POINT;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_RATE_CTRLNm, port, sizeof(port_rate_ctrln_t), &entry), ret);
    HAL_FIELD_GET(PORT_RATE_CTRLNm, PORT_RATE_CTRLN_GAP_VALUEf, &entry, pGap);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_macAutoNeg_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_enable_t orgEnable = YT_DISABLE;
    port_ctrl_t port_ctrl;
    yt_macid_t mac_id;
    yt_port_t phy_addr;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    osal_memset(&port_ctrl, 0, sizeof(port_ctrl_t));
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl), ret);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_AN_LINK_ENf, &port_ctrl, &orgEnable);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_FLOW_LINK_ANf, &port_ctrl, enable);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_AN_LINK_ENf, &port_ctrl, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl), ret);

    /*mac force to auto,need phy tiger*/
    if(orgEnable == YT_DISABLE && enable == YT_ENABLE)
    {
        if(phy_addr != INVALID_ID)
        {
            if(HALPHYDRV(unit, mac_id) != NULL)
            {
                HALPHYDRV_FUNC(unit, mac_id)->phy_restart(unit, phy_addr);
            }
        }
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_macAutoNeg_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    cmm_err_t ret = CMM_ERR_OK;
    port_ctrl_t port_ctrl;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    osal_memset(&port_ctrl, 0, sizeof(port_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl), ret);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_AN_LINK_ENf, &port_ctrl, pEnable);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_mac_force_set(yt_unit_t unit, yt_port_t port, yt_port_force_ctrl_t port_ctrl)
{
    port_ctrl_t port_ctrl_entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t mac_id;
    yt_port_speed_t speed = PORT_SPEED_1000M;
    yt_port_speed_mode_t speed_mode = PORT_SPEED_MODE_1000M;
    yt_port_duplex_t duplex_mode = PORT_DUPLEX_FULL;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    osal_memset(&port_ctrl_entry, 0, sizeof(port_ctrl_t));

    ret = fal_port_speedDup_split(port_ctrl.speed_dup, &speed, &duplex_mode);
    if(ret != CMM_ERR_OK)
    {
        return ret;
    }

    if(speed == PORT_SPEED_2500M)
    {
        /*b100 for mac and sds 2.5g*/
        speed_mode = PORT_SPEED_MODE_2500M;
    }
    else
    {
        speed_mode = (yt_port_speed_mode_t)speed;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl_entry), ret);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_FLOW_LINK_ANf, &port_ctrl_entry, YT_DISABLE);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_AN_LINK_ENf, &port_ctrl_entry, YT_DISABLE);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_SPEED_MODEf, &port_ctrl_entry, speed_mode);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_DUPLEX_MODEf, &port_ctrl_entry, duplex_mode);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_RX_FC_ENf, &port_ctrl_entry, port_ctrl.rx_fc_en);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_TX_FC_ENf, &port_ctrl_entry, port_ctrl.tx_fc_en);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl_entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_mac_force_get(yt_unit_t unit, yt_port_t port, yt_port_force_ctrl_t *pPort_ctrl)
{
    port_ctrl_t port_ctrl_entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t mac_id;
    yt_port_speed_t speed = PORT_SPEED_1000M;
    yt_port_speed_mode_t speed_mode = PORT_SPEED_MODE_1000M;
    yt_port_duplex_t duplex_mode = PORT_DUPLEX_FULL;

    mac_id = CAL_YTP_TO_MAC(unit,port);
    osal_memset(&port_ctrl_entry, 0, sizeof(port_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl_entry), ret);

    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_SPEED_MODEf, &port_ctrl_entry, &speed_mode);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_DUPLEX_MODEf, &port_ctrl_entry, &duplex_mode);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_RX_FC_ENf, &port_ctrl_entry, &pPort_ctrl->rx_fc_en);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_TX_FC_ENf, &port_ctrl_entry, &pPort_ctrl->tx_fc_en);

    /*b100 for mac and sds 2.5g*/
    if(speed_mode == PORT_SPEED_MODE_2500M)
    {
        speed = PORT_SPEED_2500M;
    }
    else
    {
        speed = (yt_port_speed_t)speed_mode;
    }

    return fal_port_speedDup_combine(speed, duplex_mode, &pPort_ctrl->speed_dup);
}

yt_ret_t fal_tiger_port_mac_fc_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    port_ctrl_t port_ctrl_entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    osal_memset(&port_ctrl_entry, 0, sizeof(port_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl_entry), ret);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_FLOW_LINK_ANf, &port_ctrl_entry, YT_DISABLE);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_RX_FC_ENf, &port_ctrl_entry, enable);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_TX_FC_ENf, &port_ctrl_entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl_entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_mac_fc_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    port_ctrl_t port_ctrl_entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    osal_memset(&port_ctrl_entry, 0, sizeof(port_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl_entry), ret);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_RX_FC_ENf, &port_ctrl_entry, pEnable);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_extif_mode_set(yt_unit_t unit, yt_port_t port, yt_extif_mode_t mode)
{
    cmm_err_t ret = CMM_ERR_OK;
    sg_phy_t sg_data;
    extif0_mode_t extif_data;
    yt_port_t extif_id;
    uint32_t reg_data = 0;
    uint32_t extif_mode_reg;
    yt_port_t extif_bit;

    if (CAL_YTP_TO_MAC(unit, port) == 4)
    {
        if (mode >= YT_EXTIF_MODE_SG_MAC)
        {
            return CMM_ERR_NOT_SUPPORT;
        }
        extif_id = 1;
        extif_bit = 2;
    }
    else
    {
        extif_id = CAL_YTP_TO_EXTPORT(unit, port);
        if(extif_id == INVALID_ID)
        {
            return CMM_ERR_INPUT;
        }
        extif_bit = (extif_id == 0) ? 1 : 0;
    }
    switch(mode)
    {
        case YT_EXTIF_MODE_MII:
        case YT_EXTIF_MODE_REMII:
        case YT_EXTIF_MODE_RMII_MAC:
        case YT_EXTIF_MODE_RMII_PHY:
        case YT_EXTIF_MODE_RGMII:
        case YT_EXTIF_MODE_XMII_DISABLE:
        {
            extif_mode_reg = (extif_id == 0) ? EXTIF0_MODEm : EXTIF1_MODEm;
            /*select xmii mode*/
            CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, CHIP_INTERFACE_SELECT_REG, &reg_data), ret);
            if (mode == YT_EXTIF_MODE_XMII_DISABLE)
            {
                CLEAR_BIT(reg_data, extif_bit);
            }
            else
            {
                SET_BIT(reg_data, extif_bit);
            }
            CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, CHIP_INTERFACE_SELECT_REG, reg_data), ret);
            /*config rgmii or xmii mode*/
            CMM_ERR_CHK(HAL_TBL_REG_READ(unit, extif_mode_reg, 0, sizeof(extif0_mode_t), &extif_data), ret);
            HAL_FIELD_SET(extif_mode_reg, EXTIF0_MODE_XMII_MODEf, &extif_data, mode);
            if (mode == YT_EXTIF_MODE_XMII_DISABLE)
            {
                HAL_FIELD_SET(extif_mode_reg, EXTIF0_MODE_XMII_PORT_ENf, &extif_data, FALSE);
            }
            else
            {
                HAL_FIELD_SET(extif_mode_reg, EXTIF0_MODE_XMII_PORT_ENf, &extif_data, TRUE);
            }
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, extif_mode_reg, 0, sizeof(extif0_mode_t), &extif_data), ret);
        }
            break;
        case YT_EXTIF_MODE_SG_MAC:
        case YT_EXTIF_MODE_SG_PHY:
        case YT_EXTIF_MODE_FIB_1000:
        case YT_EXTIF_MODE_FIB_100:
        case YT_EXTIF_MODE_BX2500:
        case YT_EXTIF_MODE_SGFIB_AS:
        case YT_EXTIF_MODE_SG_DISABLE:
        {
            /*enable serder interface*/
            CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, CHIP_INTERFACE_CTRL_REG, &reg_data), ret);
            reg_data |= 1 << extif_id;
            CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, CHIP_INTERFACE_CTRL_REG, reg_data), ret);
            /*select sg mode*/
            CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, CHIP_INTERFACE_SELECT_REG, &reg_data), ret);
            reg_data &= ~(1 << extif_bit);
            CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, CHIP_INTERFACE_SELECT_REG, reg_data), ret);
            /*config fiber or sg mode*/
            CMM_ERR_CHK(HAL_TBL_REG_READ(unit, SG_PHYm, extif_id, sizeof(sg_phy_t), &sg_data), ret);
            HAL_FIELD_SET(SG_PHYm, SG_PHY_APPLICATION_MODEf, &sg_data, mode - YT_EXTIF_MODE_SG_MAC);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, SG_PHYm, extif_id, sizeof(sg_phy_t), &sg_data), ret);
        }
            break;
        default:
            return CMM_ERR_INPUT;
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_extif_mode_get(yt_unit_t unit, yt_port_t port, yt_extif_mode_t *pMode)
{
    cmm_err_t ret = CMM_ERR_OK;
    sg_phy_t sg_data;
    extif0_mode_t extif_data;
    yt_port_t extif_id;
    uint32_t reg_data = 0;
    uint32_t extif_mode_reg;
    yt_port_t extif_bit;

    if (CAL_YTP_TO_MAC(unit, port) == 4)
    {
        extif_id = 1;
        extif_bit = 2;
    }
    else
    {
        extif_id = CAL_YTP_TO_EXTPORT(unit, port);
        if(extif_id == INVALID_ID)
        {
            return CMM_ERR_INPUT;
        }
        extif_bit = (extif_id == 0) ? 1 : 0;
    }

    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, CHIP_INTERFACE_SELECT_REG, &reg_data), ret);
    if(reg_data & (1 << extif_bit)) /*xmii*/
    {
        extif_mode_reg = (extif_id == 0) ? EXTIF0_MODEm : EXTIF1_MODEm;
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, extif_mode_reg, 0, sizeof(extif0_mode_t), &extif_data), ret);
        HAL_FIELD_GET(extif_mode_reg, EXTIF0_MODE_XMII_MODEf, &extif_data, pMode);
        if(*pMode > YT_EXTIF_MODE_XMII_DISABLE)
        {
            *pMode = YT_EXTIF_MODE_XMII_DISABLE;
            return CMM_ERR_FAIL;
        }
    }
    else
    {
        if (CAL_YTP_TO_MAC(unit, port) == 4)
        {
            *pMode = YT_EXTIF_MODE_XMII_DISABLE;
            return CMM_ERR_OK;
        }

        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, SG_PHYm, extif_id, sizeof(sg_phy_t), &sg_data), ret);
        HAL_FIELD_GET(SG_PHYm, SG_PHY_APPLICATION_MODEf, &sg_data, pMode);
        if(*pMode > YT_EXTIF_MODE_SG_DISABLE - YT_EXTIF_MODE_SG_MAC)
        {
            *pMode = YT_EXTIF_MODE_SG_DISABLE;
            return CMM_ERR_FAIL;
        }
        *pMode = *pMode + YT_EXTIF_MODE_SG_MAC;
    }

    return CMM_ERR_OK;
}

#if 0 /*TODO:for sg_phy*/
static yt_ret_t fal_tiger_port_extif_force_set(yt_unit_t unit, yt_port_t port, yt_extif_force_ctrl_t port_ctrl)
{
    cmm_err_t ret = CMM_ERR_OK;
    mdio_polling_t  rg_ctrl_entry;
    sg_phy_t    sg_ctrl_entry;
    yt_extif_mode_t mode;
    yt_port_t extif_id;
    uint8_t speed_mode = PORT_SPEED_1000M;
    uint8_t duplex_mode = PORT_DUPLEX_FULL;

    CMM_PARAM_CHK((CAL_PORT_TYPE_EXT!=CAL_YTP_PORT_TYPE(unit, port)), CMM_ERR_INPUT);

    extif_id = CAL_YTP_TO_EXTPORT(unit, port);
    osal_memset(&rg_ctrl_entry, 0, sizeof(mdio_polling_t));
    osal_memset(&sg_ctrl_entry, 0, sizeof(sg_phy_t));

    switch(port_ctrl.speed_dup)
    {
        case PORT_SPEED_DUP_10HALF:
            speed_mode = PORT_SPEED_10M;
            duplex_mode = PORT_DUPLEX_HALF;
            break;
        case PORT_SPEED_DUP_10FULL:
            speed_mode = PORT_SPEED_10M;
            duplex_mode = PORT_DUPLEX_FULL;
            break;
        case PORT_SPEED_DUP_100HALF:
            speed_mode = PORT_SPEED_100M;
            duplex_mode = PORT_DUPLEX_HALF;
            break;
        case PORT_SPEED_DUP_100FULL:
            speed_mode = PORT_SPEED_100M;
            duplex_mode = PORT_DUPLEX_FULL;
            break;
        case PORT_SPEED_DUP_1000FULL:
            speed_mode = PORT_SPEED_1000M;
            duplex_mode = PORT_DUPLEX_FULL;
            break;
        case PORT_SPEED_DUP_2500FULL:
            if(mode < YT_EXTIF_MODE_SG_MAC)/*not support for xmii*/
            {
                return CMM_ERR_INPUT;
            }
            speed_mode = 4;
            duplex_mode = PORT_DUPLEX_FULL;
            break;
        default:
            return CMM_ERR_INPUT;
    }

    CMM_ERR_CHK(fal_tiger_port_extif_mode_get(unit, port, &mode), ret);
    switch(mode)
    {
        case YT_EXTIF_MODE_MII:
        case YT_EXTIF_MODE_REMII:
        case YT_EXTIF_MODE_RMII_MAC:
        case YT_EXTIF_MODE_RMII_PHY:
        case YT_EXTIF_MODE_RGMII:
        {
            CMM_ERR_CHK(HAL_TBL_REG_READ(unit, MDIO_POLLINGm, extif_id, sizeof(mdio_polling_t), &rg_ctrl_entry), ret);
            HAL_FIELD_SET(MDIO_POLLINGm, MDIO_POLLING_DUPLEXf, &rg_ctrl_entry, duplex_mode);
            HAL_FIELD_SET(MDIO_POLLINGm, MDIO_POLLING_SPEEDf, &rg_ctrl_entry, speed_mode);
            HAL_FIELD_SET(MDIO_POLLINGm, MDIO_POLLING_LINKf, &rg_ctrl_entry, port_ctrl.link);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, MDIO_POLLINGm, extif_id, sizeof(mdio_polling_t), &rg_ctrl_entry), ret);
        }
            break;
        case YT_EXTIF_MODE_SG_MAC:
        case YT_EXTIF_MODE_SG_PHY:
        case YT_EXTIF_MODE_FIB_1000:
        case YT_EXTIF_MODE_FIB_100:
        case YT_EXTIF_MODE_BX2500:
        case YT_EXTIF_MODE_SGFIB_AS:
        {
            CMM_ERR_CHK(HAL_TBL_REG_READ(unit, SG_PHYm, extif_id, sizeof(sg_phy_t), &sg_ctrl_entry), ret);
            HAL_FIELD_SET(SG_PHYm, SG_PHY_DUPLEX_MODEf, &sg_ctrl_entry, duplex_mode);
            HAL_FIELD_SET(SG_PHYm, SG_PHY_SPEED_MODEf, &sg_ctrl_entry, speed_mode);
            HAL_FIELD_SET(SG_PHYm, SG_PHY_LINKf, &sg_ctrl_entry, port_ctrl.link);
            HAL_FIELD_SET(SG_PHYm, SG_PHY_PAUSEf, &sg_ctrl_entry, port_ctrl.pause_en);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, SG_PHYm, extif_id, sizeof(sg_phy_t), &sg_ctrl_entry), ret);
        }
            break;
        default:
            return CMM_ERR_INPUT;
    }

    return CMM_ERR_OK;
}

static yt_ret_t fal_tiger_port_extif_force_get(yt_unit_t unit, yt_port_t port, yt_extif_force_ctrl_t *pPortCtrl)
{
    cmm_err_t ret = CMM_ERR_OK;
    mdio_polling_t  rg_ctrl_entry;
    sg_phy_t    sg_ctrl_entry;
    yt_extif_mode_t mode;
    yt_port_t extif_id;
    uint8_t speed_mode = PORT_SPEED_1000M;
    uint8_t duplex_mode = PORT_DUPLEX_FULL;

    CMM_PARAM_CHK((CAL_PORT_TYPE_EXT!=CAL_YTP_PORT_TYPE(unit, port)), CMM_ERR_INPUT);

    extif_id = CAL_YTP_TO_EXTPORT(unit, port);
    osal_memset(&rg_ctrl_entry, 0, sizeof(mdio_polling_t));
    osal_memset(&sg_ctrl_entry, 0, sizeof(sg_phy_t));

    CMM_ERR_CHK(fal_tiger_port_extif_mode_get(unit, port, &mode), ret);
    switch(mode)
    {
        case YT_EXTIF_MODE_MII:
        case YT_EXTIF_MODE_REMII:
        case YT_EXTIF_MODE_RMII_MAC:
        case YT_EXTIF_MODE_RMII_PHY:
        case YT_EXTIF_MODE_RGMII:
        {
            CMM_ERR_CHK(HAL_TBL_REG_READ(unit, MDIO_POLLINGm, extif_id, sizeof(mdio_polling_t), &rg_ctrl_entry), ret);
            HAL_FIELD_GET(MDIO_POLLINGm, MDIO_POLLING_DUPLEXf, &rg_ctrl_entry, &duplex_mode);
            HAL_FIELD_GET(MDIO_POLLINGm, MDIO_POLLING_SPEEDf, &rg_ctrl_entry, &speed_mode);
            HAL_FIELD_GET(MDIO_POLLINGm, MDIO_POLLING_LINKf, &rg_ctrl_entry, &pPortCtrl->link);
            pPortCtrl->pause_en = 0;
        }
            break;
        case YT_EXTIF_MODE_SG_MAC:
        case YT_EXTIF_MODE_SG_PHY:
        case YT_EXTIF_MODE_FIB_1000:
        case YT_EXTIF_MODE_FIB_100:
        case YT_EXTIF_MODE_BX2500:
        case YT_EXTIF_MODE_SGFIB_AS:
        {
            CMM_ERR_CHK(HAL_TBL_REG_READ(unit, SG_PHYm, extif_id, sizeof(sg_phy_t), &sg_ctrl_entry), ret);
            HAL_FIELD_GET(SG_PHYm, SG_PHY_DUPLEX_MODEf, &sg_ctrl_entry, &duplex_mode);
            HAL_FIELD_GET(SG_PHYm, SG_PHY_SPEED_MODEf, &sg_ctrl_entry, &speed_mode);
            HAL_FIELD_GET(SG_PHYm, SG_PHY_LINKf, &sg_ctrl_entry, &pPortCtrl->link);
            HAL_FIELD_GET(SG_PHYm, SG_PHY_PAUSEf, &sg_ctrl_entry, &pPortCtrl->pause_en);
        }
            break;
        default:
            return CMM_ERR_INPUT;
    }

    if(duplex_mode == PORT_DUPLEX_FULL)
    {
        if(speed_mode == PORT_SPEED_10M)
        {
            pPortCtrl->speed_dup = PORT_SPEED_DUP_10FULL;
        }
        else if(speed_mode == PORT_SPEED_100M)
        {
            pPortCtrl->speed_dup = PORT_SPEED_DUP_100FULL;
        }
        else if(speed_mode == PORT_SPEED_1000M)
        {
            pPortCtrl->speed_dup = PORT_SPEED_DUP_1000FULL;
        }
        else if(speed_mode == PORT_SPEED_2500M)
        {
            pPortCtrl->speed_dup = PORT_SPEED_DUP_2500FULL;
        }
    }
    else if(duplex_mode == PORT_DUPLEX_HALF)
    {
        if(speed_mode == PORT_SPEED_10M)
        {
            pPortCtrl->speed_dup = PORT_SPEED_DUP_10HALF;
        }
        else if(speed_mode == PORT_SPEED_100M)
        {
            pPortCtrl->speed_dup = PORT_SPEED_DUP_100HALF;
        }
    }

    return CMM_ERR_OK;
}
#endif
/*
txc delay = txc_2ns_en * 2ns + txc_delay * 0.15ns
rxc delay = rxc_delay * 0.15ns
*/
yt_ret_t fal_tiger_port_extif_rgmii_delay_set(yt_unit_t unit, yt_port_t port, uint8_t rxc_delay, uint8_t txc_delay, yt_enable_t txc_2ns_en)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_t extif_id;
    uint32_t reg_data;
    uint32_t extif_mode_reg;

    if(rxc_delay > 0xF || txc_delay > 0xF)
    {
        return CMM_ERR_INPUT;
    }

    CMM_PARAM_CHK((CAL_PORT_TYPE_EXT!=CAL_YTP_PORT_TYPE(unit,port)), CMM_ERR_INPUT);
    extif_id = CAL_YTP_TO_EXTPORT(unit, port);

    extif_mode_reg = (extif_id == 0)?EXTIF0_MODEm : EXTIF1_MODEm;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, extif_mode_reg, 0, sizeof(uint32_t), &reg_data), ret);
    HAL_FIELD_SET(extif_mode_reg, EXTIF0_MODE_RGMII_TXC_DELAY_ENf, &reg_data, txc_2ns_en);
    HAL_FIELD_SET(extif_mode_reg, EXTIF0_MODE_RGMII_TXC_DELAY_SELf, &reg_data, txc_delay);
    HAL_FIELD_SET(extif_mode_reg, EXTIF0_MODE_RGMII_RXC_DELAY_SELf, &reg_data, rxc_delay);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, extif_mode_reg, 0, sizeof(uint32_t), &reg_data), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_extif_rgmii_delay_get(yt_unit_t unit, yt_port_t port, uint8_t *pRxc_delay, uint8_t *pTxc_delay, yt_enable_t *pTxc_2ns_en)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_t extif_id;
    uint32_t reg_data = 0;
    uint32_t extif_mode_reg;

    CMM_PARAM_CHK((CAL_PORT_TYPE_EXT!=CAL_YTP_PORT_TYPE(unit,port)), CMM_ERR_INPUT);
    extif_id = CAL_YTP_TO_EXTPORT(unit, port);

    extif_mode_reg = (extif_id == 0)?EXTIF0_MODEm : EXTIF1_MODEm;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, extif_mode_reg, 0, sizeof(uint32_t), &reg_data), ret);
    HAL_FIELD_GET(extif_mode_reg, EXTIF0_MODE_RGMII_TXC_DELAY_ENf, &reg_data, pTxc_2ns_en);
    HAL_FIELD_GET(extif_mode_reg, EXTIF0_MODE_RGMII_TXC_DELAY_SELf, &reg_data, pTxc_delay);
    HAL_FIELD_GET(extif_mode_reg, EXTIF0_MODE_RGMII_RXC_DELAY_SELf, &reg_data, pRxc_delay);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_phyAutoNeg_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    yt_port_t phy_addr;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            HALPHYDRV_FUNC(unit, mac_id)->phy_autoNeg_enable_set(unit, phy_addr, enable);
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phyAutoNeg_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    yt_port_t phy_addr;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            HALPHYDRV_FUNC(unit, mac_id)->phy_autoNeg_enable_get(unit, phy_addr, pEnable);
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phyAutoNeg_ability_set(yt_unit_t unit, yt_port_t port, yt_port_an_ability_t ability)
{
    yt_port_t phy_addr;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            HALPHYDRV_FUNC(unit, mac_id)->phy_autoNeg_ability_set(unit, phy_addr, ability);
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phyAutoNeg_ability_get(yt_unit_t unit, yt_port_t port, yt_port_an_ability_t *pAbility)
{
    yt_port_t phy_addr;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            HALPHYDRV_FUNC(unit, mac_id)->phy_autoNeg_ability_get(unit, phy_addr, pAbility);
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phy_force_set(yt_unit_t unit, yt_port_t port, yt_port_speed_duplex_t speed_dup)
{
    yt_port_t phy_addr;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            HALPHYDRV_FUNC(unit, mac_id)->phy_force_speed_duplex_set(unit, phy_addr, speed_dup);
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phy_force_get(yt_unit_t unit, yt_port_t port, yt_port_speed_duplex_t *pSpeedDup)
{
    yt_port_t phy_addr;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            HALPHYDRV_FUNC(unit, mac_id)->phy_force_speed_duplex_get(unit, phy_addr, pSpeedDup);
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phy_linkstatus_get(yt_unit_t unit, yt_port_t port, yt_port_linkStatus_all_t *pLinkStatus)
{
    yt_port_t phy_addr;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            HALPHYDRV_FUNC(unit, mac_id)->phy_link_status_get(unit, phy_addr, pLinkStatus);
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phy_interruptStatus_get(yt_unit_t unit, yt_port_t port, uint16_t *pIntStatus)
{
    yt_port_t phy_addr;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            HALPHYDRV_FUNC(unit, mac_id)->phy_interrupt_status_get(unit, phy_addr, pIntStatus);
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phy_reg_set(yt_unit_t unit, yt_port_t port, uint32_t regAddr, uint16_t data, yt_phy_type_t type)
{
    yt_port_t phy_addr;
    yt_ret_t ret;

    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);
    if (phy_addr == INVALID_ID || !HALSWDRV(unit))
    {
        return CMM_ERR_FAIL;
    }

    if (type == PHY_EXTERNAL)
    {
        ret = HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, regAddr, data);
    }
    else if (type == PHY_INTERNAL)
    {
        ret = HALSWDRV_FUNC(unit)->switch_intif_write(unit, phy_addr, regAddr, data);
    }
    else
    {
        return CMM_ERR_INPUT;
    }

    return ret;
}

yt_ret_t fal_tiger_port_phy_reg_get(yt_unit_t unit, yt_port_t port, uint32_t regAddr, uint16_t *pData, yt_phy_type_t type)
{
    yt_port_t phy_addr;
    yt_ret_t ret;

    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);
    if (phy_addr == INVALID_ID || !HALSWDRV(unit))
    {
        return CMM_ERR_FAIL;
    }

    if (type == PHY_EXTERNAL)
    {
        ret = HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, regAddr, pData);
    }
    else if (type == PHY_INTERNAL)
    {
        ret = HALSWDRV_FUNC(unit)->switch_intif_read(unit, phy_addr, regAddr, pData);
    }
    else
    {
        return CMM_ERR_FAIL;
    }

    return ret;
}

yt_ret_t fal_tiger_port_eee_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    yt_ret_t ret = CMM_ERR_OK;
    static uint8_t gEEEStatus = 0;
    yt_port_t phy_addr;
    yt_macid_t mac_id;
    uint32_t eeeEnable;
    uint32_t eeeData;

    /*enable global eee*/
    if(gEEEStatus == 0 && enable == YT_ENABLE)
    {
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, 0x80320, &eeeEnable), ret);
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, 0x80324, &eeeData), ret);
        eeeEnable |= (1<<16);
        eeeData |= (1<<16);
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x80320, eeeEnable), ret);
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x80324, eeeData), ret);
        gEEEStatus = 1;
    }

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, 0xb0000, &eeeEnable), ret);
    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, 0xa0000+0x40*mac_id, &eeeData), ret);
    if(enable == YT_ENABLE)
    {
        eeeEnable |= (1<<mac_id);
        eeeData |= (1<<1);
    }
    else
    {
        eeeEnable &= ~(1<<mac_id);
        eeeData &= ~(1<<1);
    }
    CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0xb0000, eeeEnable), ret);
    CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0xa0000+0x40*mac_id, eeeData), ret);

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            HALPHYDRV_FUNC(unit, mac_id)->phy_eee_enable_set(unit, phy_addr, enable);
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_eee_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    yt_port_t phy_addr;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            HALPHYDRV_FUNC(unit, mac_id)->phy_eee_enable_get(unit, phy_addr, pEnable);
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phyCombo_mode_set(yt_unit_t unit, yt_port_t port, yt_combo_mode_t mode)
{
    yt_port_t phy_addr;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            HALPHYDRV_FUNC(unit, mac_id)->phy_combo_mode_set(unit, phy_addr, mode);
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phyCombo_mode_get(yt_unit_t unit, yt_port_t port, yt_combo_mode_t *pMode)
{
    yt_port_t phy_addr;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            HALPHYDRV_FUNC(unit, mac_id)->phy_combo_mode_get(unit, phy_addr, pMode);
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_polling_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
#ifdef MONITOR_INCLUDED
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(fal_monitor_polling_port_enable_set(unit, port, enable), ret);
#else
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(enable);
#endif
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_polling_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
#ifdef MONITOR_INCLUDED
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(fal_monitor_polling_port_enable_get(unit, port, pEnable), ret);
#else
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(pEnable);
#endif
    return CMM_ERR_OK;
}

uint32_t fal_tiger_port_mac_sync_phy(yt_unit_t unit, yt_port_t port)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_enable_t enable;
    yt_port_linkStatus_all_t phyStatus;
    yt_port_force_ctrl_t mac_ctrl;
    yt_port_speed_duplex_t phySpeedDup;

    CMM_ERR_CHK(fal_tiger_port_macAutoNeg_enable_get(unit, port, &enable), ret);
    /* sync for mac force mode */
    if(YT_ENABLE == enable)
    {
        return CMM_ERR_OK;
    }

    CMM_ERR_CHK(fal_tiger_port_phy_linkstatus_get(unit, port, &phyStatus), ret);
    CMM_ERR_CHK(fal_tiger_port_phyAutoNeg_enable_get(unit, port, &enable), ret);
    CMM_ERR_CHK(fal_tiger_port_mac_force_get(unit, port, &mac_ctrl), ret);
    CMM_ERR_CHK(fal_port_speedDup_combine(phyStatus.link_speed, phyStatus.link_duplex, &phySpeedDup), ret);

    if(phySpeedDup == mac_ctrl.speed_dup)
    {
        if(enable == YT_DISABLE ||
            (enable && phyStatus.rx_fc_en == mac_ctrl.rx_fc_en &&
            phyStatus.tx_fc_en == mac_ctrl.tx_fc_en))/*if phy force,no need sync pause status*/
        {
            return CMM_ERR_OK;/*no change*/
        }
    }
    mac_ctrl.speed_dup = phySpeedDup;
    if(enable)
    {
        mac_ctrl.rx_fc_en = phyStatus.rx_fc_en;
        mac_ctrl.tx_fc_en = phyStatus.tx_fc_en;
    }

    CMM_ERR_CHK(fal_tiger_port_mac_force_set(unit, port, mac_ctrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_jumbo_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t mac_id;
    uint32_t regData;

    mac_id = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, 0x81008+0x1000*mac_id, &regData), ret);
    regData &= 0xFFC000FF;
    if(enable == YT_ENABLE)
    {
        regData |= (0x2400<<8);
    }
    else
    {
        regData |= (0x5EE<<8);
    }

    CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x81008+0x1000*mac_id, regData), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_jumbo_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t mac_id;
    uint32_t regData;

    mac_id = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, 0x81008+0x1000*mac_id, &regData), ret);
    *pEnable = ((regData&0x7FFF00) == (0x2400<<8)) ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_cable_diag(yt_unit_t unit, yt_port_t port, yt_port_cableDiag_t *pCableStatus)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_t phy_addr;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            CMM_ERR_CHK(HALPHYDRV_FUNC(unit, mac_id)->phy_cable_diag(unit, phy_addr, pCableStatus), ret);
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_jumbo_size_set(yt_unit_t unit, yt_port_t port, uint32_t size)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t mac_id;
    uint32_t regData;
    yt_enable_t enable;
    mac_id = CAL_YTP_TO_MAC(unit, port);
    if (!fal_tiger_port_jumbo_enable_get(unit, port, &enable))
    {
        if (YT_ENABLE == enable)
        {
            CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, 0x81008+0x1000*mac_id, &regData), ret);
            regData &= 0xFFC000FF;
            regData |= (size<<8);
            CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x81008+0x1000*mac_id, regData), ret);
        }
        else
        {
            return CMM_ERR_FORBIDDEN;
        }
    }
    else
    {
        return CMM_ERR_FAIL;
    }
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_jumbo_size_get(yt_unit_t unit, yt_port_t port, uint32_t *pSize)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t mac_id;
    uint32_t regData;
    mac_id = CAL_YTP_TO_MAC(unit, port);
    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, 0x81008+0x1000*mac_id, &regData), ret);
    *pSize = ((regData&0x3FFF00)>>8);
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_phyTemplate_test_set(yt_unit_t unit, yt_port_t port, yt_utp_template_testmode_t mode)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_t phy_addr;
    yt_macid_t mac_id;
    uint32_t regData;

    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, 0x80028, &regData), ret);

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    if (INVALID_ID != CAL_YTP_TO_EXTPORT(unit, port))/*for sds test*/
    {
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x80028, (1<<(2+port-5)) | 0x3), ret);
        /*disable serdes interface*/
        regData &= 0x7c;
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x80028, regData), ret);
        if(mode == YT_UTP_TEMPLATE_TMODE_SDS2500M)
        {
            fal_tiger_port_extif_mode_set(unit, port, YT_EXTIF_MODE_BX2500);
        }
        else if(mode == YT_UTP_TEMPLATE_TMODE_SDS1000M)
        {
            fal_tiger_port_extif_mode_set(unit, port, YT_EXTIF_MODE_FIB_1000);
        }
    }
    else /*internal phy port*/
    {
        /*disable serdes interface*/
        regData &= 0x7c;
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x80028, regData), ret);
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x8002c, (1<<(16+port)) | 0xff), ret);
    }

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            CMM_ERR_CHK(HALPHYDRV_FUNC(unit, mac_id)->phy_test_template(unit, phy_addr, mode), ret);
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}
