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
#include "yt_util.h"
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
#include "phy_drv.h"

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
        case PORT_SPEED_DUP_5GFULL:
            *pSpeed = PORT_SPEED_5G;
            *pDuplex = PORT_DUPLEX_FULL;
            break;
        case PORT_SPEED_DUP_10GFULL:
            *pSpeed = PORT_SPEED_10G;
            *pDuplex = PORT_DUPLEX_FULL;
            break;
        default:
            return CMM_ERR_INPUT;
    }

    return CMM_ERR_OK;
}

static yt_ret_t fal_rgmii_strength_set(yt_unit_t unit, yt_dvddio_power_pad_t powerPad, yt_dvddio_power_level_t powerLevel)
{
    uint32_t reg_data = 0;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t index = 0;

    if (POWER_PAD_NORMAL == powerPad)
    {
        return CMM_ERR_OK;
    }

    index = (POWER_PAD_RGMII1 == powerPad) ? 0 : 1;
    switch(powerLevel)
    {
        case POWER18V:
            CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, CHIP_MODE_SEL_REG, &reg_data), ret);
            SET_BIT(reg_data, 9);
            CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, CHIP_MODE_SEL_REG, reg_data), ret);

            CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, CHIP_PAD_PART_REG(index), &reg_data), ret);
            reg_data &= ~(0x7<<15);
            reg_data |= (0x4<<15);
            CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, CHIP_PAD_PART_REG(index), reg_data), ret);
            break;
        case POWER25V:
        case POWER33V:
            CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, CHIP_MODE_SEL_REG, &reg_data), ret);
            CLEAR_BIT(reg_data, 9);
            CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, CHIP_MODE_SEL_REG, reg_data), ret);

            CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, CHIP_PAD_PART_REG(index), &reg_data), ret);
            reg_data &= ~(0x7<<15);
            reg_data |= (0x3<<15);
            CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, CHIP_PAD_PART_REG(index), reg_data), ret);
            break;
        default:
            return CMM_ERR_NOT_SUPPORT;
    }
    return ret;
}

static yt_ret_t fal_rgmii_vbias_set(yt_unit_t unit, yt_dvddio_power_pad_t powerPad, yt_dvddio_power_level_t powerLevel)
{
    uint32_t reg_data = 0;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t index = 0;

    if (POWER_PAD_NORMAL == powerPad)
    {
        return ret;
    }

    if ((CAL_SWCHIP_ID(unit) == YT_SW_ID_9218 && POWER_PAD_RGMII1 == powerPad) ||
           CAL_SWCHIP_ID(unit) == YT_SW_ID_9215)
    {
        index = 1;
    }
    else if (CAL_SWCHIP_ID(unit) == YT_SW_ID_9218 && POWER_PAD_RGMII2 == powerPad)
    {
        index = 3;
    }
    else
    {
        return CMM_ERR_NOT_SUPPORT;
    }
    switch(powerLevel)
    {
        case POWER18V:
        case POWER25V:
            CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, CHIP_DCC_CALIB_CLK_SEL_REG, &reg_data), ret);
            reg_data &= ~(0x3<<index);
            CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, CHIP_DCC_CALIB_CLK_SEL_REG, reg_data), ret);
            break;
        case POWER33V:
            CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, CHIP_DCC_CALIB_CLK_SEL_REG, &reg_data), ret);
            reg_data &= ~(0x3<<index);
            reg_data |= (0x2<<index);
            CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, CHIP_DCC_CALIB_CLK_SEL_REG, reg_data), ret);
            break;
        default:
            return CMM_ERR_NOT_SUPPORT;
    }

    return ret;
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
        else if(speed == PORT_SPEED_5G)
        {
            *pSpeedDup = PORT_SPEED_DUP_5GFULL;
        }
        else if(speed == PORT_SPEED_10G)
        {
            *pSpeedDup = PORT_SPEED_DUP_10GFULL;
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

static yt_ret_t fal_port_serdes_init(yt_unit_t unit)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t phyAddr;
    uint16_t phyData;
    static yt_bool_t initFlag = 0;
    uint32_t bit = 0;
    yt_phy_comm_cfg_t cfg;

    if(initFlag)
    {
        return CMM_ERR_OK;
    }

    cfg.unit = unit;
    cfg.smiType = YT_PHY_SMITYPE_SW_INT;
    for(phyAddr = 8; phyAddr <= 9; phyAddr++)
    {
        cfg.phyAddr = phyAddr;
        CMM_ERR_CHK(phy_ext_reg_read(cfg, 0xa0, &phyData), ret);
        if (CAL_SWCHIP_ID(unit) == YT_SW_ID_9218)
        {
            bit = 13;
        }
        else
        {
            bit = 14;
        }
        if (IS_BIT_SET(phyData, bit))
        {
            CLEAR_BIT(phyData, bit);
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0xa0, phyData), ret);
        }

        /*vco band init*/
        CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x2e, &phyData), ret);
        SET_BIT(phyData, 9);
        CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x2e, phyData), ret);

        /*serdes as mode */
        CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x1d0, &phyData), ret);
        phyData &= ~(0xff << 0);
        phyData |= (0x80<<0);
        CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x1d0, phyData), ret);
    }

    initFlag = 1;

    return CMM_ERR_OK;
}

static yt_ret_t fal_tiger_port_medium_mode_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t *phyMode)
{
    yt_port_medium_t portMedium = 0;
    yt_ret_t ret = CMM_ERR_OK;
    yt_port_linkStatus_all_t phyStatus;
    yt_combo_mode_t comboMode = COMBO_MODE_COPPER_FIRST;

    portMedium = CAL_PORT_MEDIUM(unit, port);
    if ((portMedium == PORT_MEDI_COMBO_FIBER) || (portMedium == PORT_MEDI_COMBO_COPPER))
    {
        CMM_ERR_CHK(fal_tiger_port_phy_linkstatus_get(unit, port, YT_PHY_CHIP_MODE_COPPER, &phyStatus), ret);
        if (phyStatus.link_status == PORT_LINK_UP)
        {
            *phyMode = YT_PHY_CHIP_MODE_COPPER;
        }
        else
        {
            CMM_ERR_CHK(fal_tiger_port_phy_linkstatus_get(unit, port, YT_PHY_CHIP_MODE_FIBER, &phyStatus), ret);
            if (phyStatus.link_status == PORT_LINK_UP)
            {
                *phyMode = YT_PHY_CHIP_MODE_FIBER;
            }
            else
            {
                if (portMedium == PORT_MEDI_COMBO_FIBER)
                {
                    CMM_ERR_CHK(fal_tiger_port_phyCombo_mode_get(unit, port, &comboMode), ret);
                    if (comboMode == COMBO_MODE_COPPER_FIRST)
                    {
                        *phyMode = YT_PHY_CHIP_MODE_COPPER;
                    }
                    else
                    {
                        *phyMode = YT_PHY_CHIP_MODE_FIBER;
                    }
                }
            }
        }
    }
    else if (portMedium == PORT_MEDI_COPPER)
    {
        *phyMode = YT_PHY_CHIP_MODE_COPPER;
    }
    else if (portMedium == PORT_MEDI_FIBER)
    {
        *phyMode = YT_PHY_CHIP_MODE_FIBER;
    }
    else
    {
        return CMM_ERR_NOT_SUPPORT;
    }
    return ret;
}

yt_ret_t fal_tiger_port_default_cfg_set(yt_unit_t unit)
{
#ifdef PORT_INCLUDED
    yt_port_t port;
    yt_port_attri_t attr;
    yt_extif_mode_t extifMode;
    uint8_t extif_id = 0;
#endif

#ifdef PORT_INCLUDED
    for(port = 0; port < CAL_PORT_NUM_ON_UNIT(unit); port++)
    {
        attr = CAL_PORT_ATTRIBUTE(unit, port);
        switch(attr)
        {
            case PORT_ATTR_PHY:
            case PORT_ATTR_SERDES:
            case PORT_ATTR_XMII:
                /*ext port*/
                if(CAL_PORT_TYPE_EXT == CAL_YTP_PORT_TYPE(unit, port))
                {
                    extifMode = CAL_PORT_EXTIFMODE(unit, port);
                    if (extifMode != INVALID_ID)
                    {
                        fal_tiger_port_extif_mode_set(unit, port, extifMode);
                    }

                    if(CAL_IS_PHY_PORT(unit, port))
                    {
                        if(CAL_IS_YTPHY(unit, port))
                        {
                            extif_id = CAL_YTP_TO_EXTPORT(unit, port);
                            HAL_MEM_DIRECT_WRITE(unit, 0x8035C+4*extif_id, TRUE);/*enable rgmii AN*/
                        }
                        else
                        {
                            /*TODO:set mac force and start polling*/
                        }
                    }
                }
                /*config combo port*/
                if(CAL_IS_COMBO_PORT(unit, port))
                {
                    /*phy combo mode*/
                    if(CAL_IS_PHY_PORT(unit, port))
                    {
                        if(PORT_MEDI_COMBO_FIBER == CAL_PORT_MEDIUM(unit, port))
                        {
                            fal_tiger_port_phyCombo_mode_set(unit, port, COMBO_MODE_FIBER_FIRST);
                        }
                        else if(PORT_MEDI_COMBO_COPPER == CAL_PORT_MEDIUM(unit, port))
                        {
                            fal_tiger_port_phyCombo_mode_set(unit, port, COMBO_MODE_COPPER_FIRST);
                        }
                    }
                    /*TODO:other*/
                }
                break;
            case PORT_ATTR_INT_CPU:
                break;
            default:
                break;
        }
    }
#else
    CMM_UNUSED_PARAM(unit);
#endif
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_init(yt_unit_t unit)
{
    yt_port_t port;
    cmm_err_t ret = CMM_ERR_OK;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    for(port = 0; port < CAL_PORT_NUM_ON_UNIT(unit); port++)
    {
        ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
        if (ret == CMM_ERR_OK)
        {
            yt_phy_init(cfg);
        }

    }
    CMM_ERR_CHK(fal_port_serdes_init(unit), ret);
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_mac_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
    port_ctrl_t port_ctrl;
    yt_macid_t mac_id;
    yt_enable_t orgEnable;
    yt_phy_comm_cfg_t cfg;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    /* if no change, do nothing */
    fal_tiger_port_mac_enable_get(unit, port, &orgEnable);
    if(orgEnable == enable)
    {
        return CMM_ERR_OK;
    }

    CMM_PARAM_CHK(HAL_TBL_REG_READ(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl), ret);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_AN_LINK_ENf, &port_ctrl, enable);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_TXMAC_ENf, &port_ctrl, enable);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_RXMAC_ENf, &port_ctrl, enable);
    CMM_PARAM_CHK(HAL_TBL_REG_WRITE(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl), ret);

    if ((orgEnable == YT_DISABLE) && (enable == YT_ENABLE))
    {
        cfg.unit = unit;
        ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
        if (ret == CMM_ERR_OK)
        {
            yt_phy_restart(cfg);
        }
    }
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_mac_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    cmm_err_t ret = CMM_ERR_OK;
    port_ctrl_t port_ctrl;
    yt_macid_t mac_id;
    uint32_t an_en;
    uint32_t tx_en;
    uint32_t rx_en;

    mac_id = CAL_YTP_TO_MAC(unit, port);

    CMM_PARAM_CHK(HAL_TBL_REG_READ(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl), ret);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_AN_LINK_ENf, &port_ctrl, &an_en);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_TXMAC_ENf, &port_ctrl, &tx_en);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_RXMAC_ENf, &port_ctrl, &rx_en);

    if((an_en || tx_en || rx_en))
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
    osal_memset(&status, sizeof(port_status_t), 0, sizeof(port_status_t));
    
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
    osal_memset(&port_status, sizeof(port_status_t), 0, sizeof(port_status_t));
    
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
    yt_macid_t macId;
    cmm_err_t ret = CMM_ERR_OK;

    macId = CAL_YTP_TO_MAC(unit, port);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_RATE_CTRLNm, macId, sizeof(port_rate_ctrln_t), &entry), ret);
    HAL_FIELD_SET(PORT_RATE_CTRLNm, PORT_RATE_CTRLN_GAP_VALUEf, &entry, gap);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PORT_RATE_CTRLNm, macId, sizeof(port_rate_ctrln_t), &entry), ret);
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_pkt_gap_get(yt_unit_t unit, yt_port_t port, uint8_t *pGap)
{
    port_rate_ctrln_t entry;
    yt_macid_t macId;
    cmm_err_t ret = CMM_ERR_OK;

    if(NULL == pGap)
    {
        return CMM_ERR_NULL_POINT;
    }

    macId = CAL_YTP_TO_MAC(unit, port);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_RATE_CTRLNm, macId, sizeof(port_rate_ctrln_t), &entry), ret);
    HAL_FIELD_GET(PORT_RATE_CTRLNm, PORT_RATE_CTRLN_GAP_VALUEf, &entry, pGap);
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_macAutoNeg_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_enable_t orgEnable = YT_DISABLE;
    port_ctrl_t port_ctrl;
    yt_macid_t mac_id;
    yt_phy_comm_cfg_t cfg;

    mac_id = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&port_ctrl, sizeof(port_ctrl_t), 0, sizeof(port_ctrl_t));
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl), ret);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_AN_LINK_ENf, &port_ctrl, &orgEnable);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_FLOW_LINK_ANf, &port_ctrl, enable);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_AN_LINK_ENf, &port_ctrl, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl), ret);

    /*mac force to auto,need phy tiger*/
    if(orgEnable == YT_DISABLE && enable == YT_ENABLE)
    {
        cfg.unit = unit;
        ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
        if (ret == CMM_ERR_OK)
        {
            yt_phy_restart(cfg);
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
    osal_memset(&port_ctrl, sizeof(port_ctrl_t), 0, sizeof(port_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl), ret);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_AN_LINK_ENf, &port_ctrl, pEnable);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_mac_force_set(yt_unit_t unit, yt_port_t port, yt_port_speed_duplex_t port_ctrl)
{
    port_ctrl_t port_ctrl_entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t mac_id;
    yt_port_speed_t speed = PORT_SPEED_1000M;
    yt_port_speed_mode_t speed_mode = PORT_SPEED_MODE_1000M;
    yt_port_duplex_t duplex_mode = PORT_DUPLEX_FULL;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    osal_memset(&port_ctrl_entry, sizeof(port_ctrl_t), 0, sizeof(port_ctrl_t));

    ret = fal_port_speedDup_split(port_ctrl, &speed, &duplex_mode);
    if(ret != CMM_ERR_OK)
    {
        return ret;
    }

    if(speed == PORT_SPEED_2500M)
    {
        speed_mode = PORT_SPEED_MODE_2500M;
    }
    else if (speed == PORT_SPEED_5G)
    {
        speed_mode = PORT_SPEED_MODE_5G;
    }
    else if (speed == PORT_SPEED_10G)
    {
        speed_mode = PORT_SPEED_MODE_10G;
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
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl_entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_mac_force_get(yt_unit_t unit, yt_port_t port, yt_port_speed_duplex_t *pPort_ctrl)
{
    port_ctrl_t port_ctrl_entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t mac_id;
    yt_port_speed_t speed = PORT_SPEED_1000M;
    yt_port_speed_mode_t speed_mode = PORT_SPEED_MODE_1000M;
    yt_port_duplex_t duplex_mode = PORT_DUPLEX_FULL;

    mac_id = CAL_YTP_TO_MAC(unit,port);
    osal_memset(&port_ctrl_entry, sizeof(port_ctrl_t), 0, sizeof(port_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl_entry), ret);

    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_SPEED_MODEf, &port_ctrl_entry, &speed_mode);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_DUPLEX_MODEf, &port_ctrl_entry, &duplex_mode);

    if(speed_mode == PORT_SPEED_MODE_2500M)
    {
        speed = PORT_SPEED_2500M;
    }
    else if (speed_mode == PORT_SPEED_MODE_5G)
    {
        speed = PORT_SPEED_5G;
    }
    else if (speed_mode == PORT_SPEED_MODE_10G)
    {
        speed = PORT_SPEED_10G;
    }
    else
    {
        speed = (yt_port_speed_t)speed_mode;
    }

    return fal_port_speedDup_combine(speed, duplex_mode, pPort_ctrl);
}

yt_ret_t fal_tiger_port_mac_fc_set(yt_unit_t unit, yt_port_t port, yt_enable_t fcAutoNegEnable, yt_enable_t rxFcEnable, yt_enable_t txFcEnable)
{
    port_ctrl_t port_ctrl_entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    osal_memset(&port_ctrl_entry, sizeof(port_ctrl_t), 0, sizeof(port_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl_entry), ret);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_FLOW_LINK_ANf, &port_ctrl_entry, fcAutoNegEnable);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_RX_FC_ENf, &port_ctrl_entry, rxFcEnable);
    HAL_FIELD_SET(PORT_CTRLm, PORT_CTRL_TX_FC_ENf, &port_ctrl_entry, txFcEnable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl_entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_mac_fc_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pFcAutoNegEnable, yt_enable_t *pRxFcEnable, yt_enable_t *pTxFcEnable)
{
    port_ctrl_t port_ctrl_entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    osal_memset(&port_ctrl_entry, sizeof(port_ctrl_t), 0, sizeof(port_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_CTRLm, mac_id, sizeof(port_ctrl_t), &port_ctrl_entry), ret);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_FLOW_LINK_ANf, &port_ctrl_entry, pFcAutoNegEnable);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_RX_FC_ENf, &port_ctrl_entry, pRxFcEnable);
    HAL_FIELD_GET(PORT_CTRLm, PORT_CTRL_TX_FC_ENf, &port_ctrl_entry, pTxFcEnable);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_mac_block_set(yt_unit_t unit, yt_port_t port, yt_enable_t rxCfgEnable, yt_enable_t txCfgEnable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(rxCfgEnable);
    CMM_UNUSED_PARAM(txCfgEnable);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_port_mac_block_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pRxCfgEnable, yt_enable_t *pTxCfgEnable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(pRxCfgEnable);
    CMM_UNUSED_PARAM(pTxCfgEnable);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_port_mac_eee_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    static uint8_t gEEEStatus = 0;
    yt_macid_t mac_id;
    uint32_t eeeEnable;
    uint32_t eeeData;
    yt_ret_t ret = CMM_ERR_OK;

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

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_mac_eee_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    yt_macid_t macId;
    uint32_t eeeData;
    yt_ret_t ret = CMM_ERR_OK;

    macId = CAL_YTP_TO_MAC(unit, port);
    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, 0xa0000+0x40*macId, &eeeData), ret);
    eeeData &= (1 << 1);
    (*pEnable) = eeeData ? YT_ENABLE : YT_DISABLE;

    return ret;
}

yt_ret_t fal_tiger_port_mac_eee_timer_set(yt_unit_t unit, yt_port_t port, uint32_t wakeupTimer, uint32_t sleepTimer)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(wakeupTimer);
    CMM_UNUSED_PARAM(sleepTimer);
    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_port_mac_eee_timer_get(yt_unit_t unit, yt_port_t port, uint32_t *pWakeupTimer, uint32_t *pSleepTimer)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(pWakeupTimer);
    CMM_UNUSED_PARAM(pSleepTimer);
    return CMM_ERR_NOT_SUPPORT;
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
	yt_phy_comm_cfg_t cfg;
    uint8_t phyType; 
    yt_port_attri_t attribute;

    if (CAL_YTP_PORT_TYPE(unit, port) == CAL_PORT_TYPE_INTPHY)
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    cfg.unit = unit;
    cfg.phyAddr = CAL_YTP_TO_EXTPHYADDR(unit, port);
    cfg.smiType = YT_PHY_SMITYPE_SW_EXT;
    phyType = CAL_YTP_TO_PHYTYPE(unit, port);
    if ((cfg.phyAddr != INVALID_ID) && (phyType & YT_PHY_EXT))
    {
        CMM_ERR_CHK(yt_phy_fc_autoneg_cfg_set(cfg, mode), ret);
    }

    cfg.phyAddr = CAL_YTP_TO_INTPHYADDR(unit, port);
    cfg.smiType = YT_PHY_SMITYPE_SW_INT;
    attribute = CAL_PORT_ATTRIBUTE(unit, port);
    if ((cfg.phyAddr != INVALID_ID) && ((attribute == PORT_ATTR_PHY) || (attribute == PORT_ATTR_SERDES)))
    {
        CMM_ERR_CHK(yt_phy_fc_autoneg_cfg_set(cfg, mode), ret);
    }

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
    phy_swIntPhy_drv_update(unit, port, mode);

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
        if(*pMode > YT_EXTIF_MODE_SGFIB_AS - YT_EXTIF_MODE_SG_MAC)
        {
            *pMode = YT_EXTIF_MODE_SGFIB_AS;
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
    osal_memset(&rg_ctrl_entry, sizeof(mdio_polling_t), 0, sizeof(mdio_polling_t));
    osal_memset(&sg_ctrl_entry, sizeof(sg_phy_t), 0, sizeof(sg_phy_t));

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
        case YT_EXTIF_MODE_SGFIB_RSVD:
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
    osal_memset(&rg_ctrl_entry, sizeof(mdio_polling_t), 0, sizeof(mdio_polling_t));
    osal_memset(&sg_ctrl_entry, sizeof(sg_phy_t), 0, sizeof(sg_phy_t));

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
        case YT_EXTIF_MODE_SGFIB_RSVD:
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
    yt_extif_mode_t mode;
    uint32_t extif_mode_reg;

    if(rxc_delay > 0xF || txc_delay > 0xF)
    {
        return CMM_ERR_INPUT;
    }
    if (CAL_YTP_TO_MAC(unit, port) == 4)
    {
        ret = fal_tiger_port_extif_mode_get(unit, port, &mode);
        if (ret != CMM_ERR_OK)
        {
            return ret;
        }
        if (YT_EXTIF_MODE_RGMII == mode)
        {
            extif_id = 1;
        }
        else
        {
            return CMM_ERR_NOT_SUPPORT;
        }
    }
    else
    {
        CMM_PARAM_CHK((CAL_PORT_TYPE_EXT!=CAL_YTP_PORT_TYPE(unit,port)), CMM_ERR_INPUT);
        extif_id = CAL_YTP_TO_EXTPORT(unit, port);
    }

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
    yt_extif_mode_t mode;
    uint32_t extif_mode_reg;

    if (CAL_YTP_TO_MAC(unit, port) == 4)
    {
        ret = fal_tiger_port_extif_mode_get(unit, port, &mode);
        if (ret != CMM_ERR_OK)
        {
            return ret;
        }
        if (YT_EXTIF_MODE_RGMII == mode)
        {
            extif_id = 1;
        }
        else
        {
            return CMM_ERR_NOT_SUPPORT;
        }
    }
    else
    {
        CMM_PARAM_CHK((CAL_PORT_TYPE_EXT!=CAL_YTP_PORT_TYPE(unit,port)), CMM_ERR_INPUT);
        extif_id = CAL_YTP_TO_EXTPORT(unit, port);
    }

    extif_mode_reg = (extif_id == 0)?EXTIF0_MODEm : EXTIF1_MODEm;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, extif_mode_reg, 0, sizeof(uint32_t), &reg_data), ret);
    HAL_FIELD_GET(extif_mode_reg, EXTIF0_MODE_RGMII_TXC_DELAY_ENf, &reg_data, pTxc_2ns_en);
    HAL_FIELD_GET(extif_mode_reg, EXTIF0_MODE_RGMII_TXC_DELAY_SELf, &reg_data, pTxc_delay);
    HAL_FIELD_GET(extif_mode_reg, EXTIF0_MODE_RGMII_RXC_DELAY_SELf, &reg_data, pRxc_delay);
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_phy_enable_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_enable_t enable)
{
    yt_macid_t mac_id;
    uint32_t regData;
    yt_phy_comm_cfg_t cfg;
    yt_ret_t ret;

    mac_id = CAL_YTP_TO_MAC(unit, port);
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

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        yt_phy_enable_set(cfg, phyMode, enable);
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_phy_enable_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable)
{
    yt_ret_t ret;
    yt_enable_t phy_en = YT_ENABLE;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        yt_phy_enable_get(cfg, phyMode, &phy_en);
    }

    if(phy_en)
    {
        *pEnable = YT_ENABLE;
    }
    else
    {
        *pEnable = YT_DISABLE;
    }

    return CMM_ERR_OK;
}


yt_ret_t fal_tiger_port_phyAutoNeg_enable_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_enable_t enable)
{
    yt_ret_t ret;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        yt_phy_autoNeg_enable_set(cfg, phyMode, enable);
        return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phyAutoNeg_enable_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable)
{
    yt_ret_t ret;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        yt_phy_autoNeg_enable_get(cfg, phyMode, pEnable);
        return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phyAutoNeg_ability_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t ability)
{
    yt_ret_t ret;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        yt_phy_autoNeg_ability_set(cfg, phyMode, ability);
        return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phyAutoNeg_ability_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t *pAbility)
{
    yt_ret_t ret;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        yt_phy_autoNeg_ability_get(cfg, phyMode, pAbility);
        return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phy_force_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t speed_dup)
{
    yt_ret_t ret;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        yt_phy_force_speed_duplex_set(cfg, phyMode, speed_dup);

        return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phy_force_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t *pSpeedDup)
{
    yt_ret_t ret;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        yt_phy_force_speed_duplex_get(cfg, phyMode, pSpeedDup);
        return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phy_linkstatus_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_linkStatus_all_t *pLinkStatus)
{
    yt_ret_t ret;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        yt_phy_link_status_get(cfg, phyMode, pLinkStatus);
		return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phy_interruptStatus_get(yt_unit_t unit, yt_port_t port, uint16_t *pIntStatus)
{
    yt_ret_t ret = CMM_ERR_FAIL;
    yt_phy_comm_cfg_t cfg;
    yt_port_attri_t attribute;
    uint8_t phyType; 

    cfg.unit = unit;
    cfg.phyAddr = CAL_YTP_TO_INTPHYADDR(unit, port);
    cfg.smiType = YT_PHY_SMITYPE_SW_INT;
    attribute = CAL_PORT_ATTRIBUTE(unit, port);
    if ((cfg.phyAddr != INVALID_ID) && ((attribute == PORT_ATTR_PHY) || (attribute == PORT_ATTR_SERDES)))
    {
        ret = yt_phy_interrupt_status_get(cfg, pIntStatus);
    }

    cfg.phyAddr = CAL_YTP_TO_EXTPHYADDR(unit, port);
    cfg.smiType = YT_PHY_SMITYPE_SW_EXT;
    phyType = CAL_YTP_TO_PHYTYPE(unit, port);
    if ((cfg.phyAddr != INVALID_ID) && (phyType & YT_PHY_EXT))
    {
        ret |= yt_phy_interrupt_status_get(cfg, pIntStatus);
    }

    return (ret == CMM_ERR_OK) ? CMM_ERR_OK : CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phy_reg_set(yt_unit_t unit, yt_port_t port, uint32_t regAddr, uint16_t data, yt_phy_type_t type, yt_phy_reg_type_t regType)
{
    yt_port_t phyAddr;
    yt_ret_t ret;
    yt_phy_comm_cfg_t cfg;

    CMM_UNUSED_PARAM(regType);
    if (type == PHY_INTERNAL)
    {
        phyAddr = CAL_YTP_TO_INTPHYADDR(unit, port);
    }
    else
    {
        phyAddr = CAL_YTP_TO_EXTPHYADDR(unit, port);
    }

    if (phyAddr == INVALID_ID)
    {
        return CMM_ERR_FAIL;
    }

    cfg.unit = unit;
    cfg.phyAddr = phyAddr;
    if (type == PHY_EXTERNAL)
    {
        cfg.smiType = YT_PHY_SMITYPE_SW_EXT;
    }
    else if (type == PHY_INTERNAL)
    {
        cfg.smiType = YT_PHY_SMITYPE_SW_INT;
    }
    else
    {
        return CMM_ERR_INPUT;
    }

    ret = phy_mii_reg_write(cfg, regAddr, data);

    return ret;
}

yt_ret_t fal_tiger_port_phy_reg_get(yt_unit_t unit, yt_port_t port, uint32_t regAddr, uint16_t *pData, yt_phy_type_t type, yt_phy_reg_type_t regType)
{
    yt_port_t phyAddr;
    yt_ret_t ret;
    yt_phy_comm_cfg_t cfg;

    CMM_UNUSED_PARAM(regType);
    if (type == PHY_INTERNAL)
    {
        phyAddr = CAL_YTP_TO_INTPHYADDR(unit, port);
    }
    else
    {
        phyAddr = CAL_YTP_TO_EXTPHYADDR(unit, port);
    }

    if (phyAddr == INVALID_ID)
    {
        return CMM_ERR_FAIL;
    }
    cfg.unit = unit;
    cfg.phyAddr = phyAddr;
    if (type == PHY_EXTERNAL)
    {
        cfg.smiType = YT_PHY_SMITYPE_SW_EXT;
    }
    else if (type == PHY_INTERNAL)
    {
        cfg.smiType = YT_PHY_SMITYPE_SW_INT;

    }
    else
    {
        return CMM_ERR_FAIL;
    }

    ret = phy_mii_reg_read(cfg, regAddr, pData);

    return ret;
}

yt_ret_t fal_tiger_port_phy_eee_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    yt_ret_t ret;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        yt_phy_eee_enable_set(cfg, enable);
		return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phy_eee_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    yt_ret_t ret;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        yt_phy_eee_enable_get(cfg, pEnable);
		return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phy_eee_status_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    yt_ret_t ret;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        yt_phy_eee_status_get(cfg, pEnable);
		return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phyCombo_mode_set(yt_unit_t unit, yt_port_t port, yt_combo_mode_t mode)
{
    yt_ret_t ret;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        yt_phy_combo_mode_set(cfg, mode);
        return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phyCombo_mode_get(yt_unit_t unit, yt_port_t port, yt_combo_mode_t *pMode)
{
    yt_ret_t ret;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        yt_phy_combo_mode_get(cfg, pMode);
        return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phy_chip_mode_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t mode)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(mode);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_port_phy_chip_mode_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t *pMode)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(pMode);

    return CMM_ERR_NOT_SUPPORT;
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
    yt_port_speed_duplex_t mac_ctrl;
    yt_port_speed_duplex_t phySpeedDup;
    yt_enable_t fcAutoNegEnable;
    yt_enable_t rxFcEnable;
    yt_enable_t txFcEnable;
    yt_phy_chip_mode_t phyMode = 0;

    CMM_ERR_CHK(fal_tiger_port_medium_mode_get(unit, port, &phyMode), ret);
    CMM_ERR_CHK(fal_tiger_port_macAutoNeg_enable_get(unit, port, &enable), ret);
    /* sync for mac force mode */
    if(YT_ENABLE == enable)
    {
        return CMM_ERR_OK;
    }

    CMM_ERR_CHK(fal_tiger_port_phy_linkstatus_get(unit, port, phyMode, &phyStatus), ret);
    CMM_ERR_CHK(fal_tiger_port_phyAutoNeg_enable_get(unit, port, phyMode, &enable), ret);
    CMM_ERR_CHK(fal_tiger_port_mac_force_get(unit, port, &mac_ctrl), ret);
    CMM_ERR_CHK(fal_tiger_port_mac_fc_get(unit, port, &fcAutoNegEnable, &rxFcEnable, &txFcEnable), ret);
    CMM_ERR_CHK(fal_port_speedDup_combine(phyStatus.link_speed, phyStatus.link_duplex, &phySpeedDup), ret);

    if(phySpeedDup == mac_ctrl)
    {
        if(enable == YT_DISABLE ||
            (enable && phyStatus.rx_fc_en == rxFcEnable &&
            phyStatus.tx_fc_en == txFcEnable))/*if phy force,no need sync pause status*/
        {
            return CMM_ERR_OK;/*no change*/
        }
    }
    mac_ctrl = phySpeedDup;
    if(enable)
    {
        rxFcEnable = phyStatus.rx_fc_en;
        txFcEnable = phyStatus.tx_fc_en;
    }

    CMM_ERR_CHK(fal_tiger_port_mac_force_set(unit, port, mac_ctrl), ret);
    CMM_ERR_CHK(fal_tiger_port_mac_fc_set(unit, port, YT_DISABLE, rxFcEnable, txFcEnable), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_port_jumbo_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t mac_id;
    uint32_t regData;
    yt_enable_t pEnable;

    CMM_ERR_CHK(fal_tiger_port_jumbo_enable_get(unit, port, &pEnable), ret);
    if (pEnable == enable)
    {
        return CMM_ERR_OK;
    }

    mac_id = CAL_YTP_TO_MAC(unit, port);
    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, 0x81008+0x1000*mac_id, &regData), ret);
    if(enable == YT_ENABLE)
    {
        if (((regData&0x3FFF00)>>8) <= CAL_JUMBO_SIZE_MIN)
        {
            regData &= 0xFFC000FF;
            regData |= (CAL_JUMBO_SIZE_MAX<<8);
        }
    }
    else
    {
        regData &= 0xFFC000FF;
        regData |= (CAL_JUMBO_SIZE_MIN<<8);
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
    *pEnable = ((regData&0x3FFF00) <= (CAL_JUMBO_SIZE_MAX<<8) && (regData&0x3FFF00) > (CAL_JUMBO_SIZE_MIN<<8)) ? YT_ENABLE : YT_DISABLE;
    return CMM_ERR_OK;
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

yt_ret_t fal_tiger_port_cable_diag_start(yt_unit_t unit, yt_port_t port)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        CMM_ERR_CHK(yt_phy_cable_diag_start(cfg), ret);
	return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_cable_diag_result_get(yt_unit_t unit, yt_port_t port, yt_port_cableDiag_t *pCableStatus)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        CMM_ERR_CHK(yt_phy_cable_diag_result_get(cfg, pCableStatus), ret);
	return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phyCrossover_mode_set(yt_unit_t unit, yt_port_t port, yt_utp_crossover_mode_t mode)
{
    cmm_err_t ret = CMM_ERR_OK;
	yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        CMM_ERR_CHK(yt_phy_crossover_mode_set(cfg, mode), ret);
		return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phyCrossover_mode_get(yt_unit_t unit, yt_port_t port, yt_utp_crossover_mode_t *pMode)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        CMM_ERR_CHK(yt_phy_crossover_mode_get(cfg, pMode), ret);
		return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phyCrossover_status_get(yt_unit_t unit, yt_port_t port, yt_utp_crossover_status_t *pStatus)
{
    cmm_err_t ret = CMM_ERR_OK;
	yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        CMM_ERR_CHK(yt_phy_crossover_status_get(cfg, pStatus), ret);
		return CMM_ERR_OK;
    }
    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phyGreen_start(yt_unit_t unit, yt_port_t port)
{
    yt_ret_t ret;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        yt_phy_green_start(cfg);
	return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_phyGreen_result_get(yt_unit_t unit, yt_port_t port, yt_port_cableDiag_t *pStatus)
{
    yt_ret_t ret;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        yt_phy_green_result_get(cfg, pStatus);
        return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_port_physmart_downgrade_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(enable);

    return CMM_ERR_NOT_SUPPORT;;
}

yt_ret_t fal_tiger_port_physmart_downgrade_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(pEnable);

    return CMM_ERR_NOT_SUPPORT;;
}

/**
 * @internal      fal_tiger_port_extif_xmiiClk_invert_set
 * @endinternal
 *
 * @brief         set extif xmii clk invert 
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     state                -xmii clk revert enable/disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_port_extif_xmiiClk_invert_set(yt_unit_t unit, yt_port_t port,yt_enable_t state)
{
    cmm_err_t ret = CMM_ERR_OK;
    extif0_mode_t extif_data;
    yt_port_t extif_id;
    uint32_t extif_mode_reg;
    
    if (CAL_YTP_TO_MAC(unit, port) == 4)
    {
        extif_id = 1;
    }
    else
    {
        extif_id = CAL_YTP_TO_EXTPORT(unit, port);
        if(extif_id == INVALID_ID)
        {
            return CMM_ERR_INPUT;
        }
    }
    extif_mode_reg = (extif_id == 0) ? EXTIF0_MODEm : EXTIF1_MODEm;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, extif_mode_reg, 0, sizeof(extif0_mode_t), &extif_data), ret);
    HAL_FIELD_SET(extif_mode_reg, EXTIF0_MODE_XMII_TXC_IN_SELf, &extif_data, state);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, extif_mode_reg, 0, sizeof(extif0_mode_t), &extif_data), ret);
    return CMM_ERR_OK;
}


/**
 * @internal      fal_tiger_port_extif_xmiiClk_invert_get
 * @endinternal
 *
 * @brief         get extif xmii clk invert 
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pState               -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_port_extif_xmiiClk_invert_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pState)
{
    cmm_err_t ret = CMM_ERR_OK;
    extif0_mode_t extif_data;
    yt_port_t extif_id;
    uint32_t extif_mode_reg;

    if (CAL_YTP_TO_MAC(unit, port) == 4)
    {
        extif_id = 1;
    }
    else
    {
        extif_id = CAL_YTP_TO_EXTPORT(unit, port);
        if(extif_id == INVALID_ID)
        {
            return CMM_ERR_INPUT;
        }
    }
    extif_mode_reg = (extif_id == 0) ? EXTIF0_MODEm : EXTIF1_MODEm;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, extif_mode_reg, 0, sizeof(extif0_mode_t), &extif_data), ret);
    
    HAL_FIELD_GET(extif_mode_reg, EXTIF0_MODE_XMII_TXC_IN_SELf, &extif_data, pState);
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_port_parallel_detection_set
 * @endinternal
 *
 * @brief         set serdes parallel detection state
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    enable             -parallel detection state enable/disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_port_parallel_detection_set(yt_unit_t unit, yt_port_t port,yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
	yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (CAL_IS_SERDES(unit, port) && (ret == CMM_ERR_OK))
    {
        yt_phy_parallel_detection_set(cfg, enable);
        return CMM_ERR_OK;
    }

    return CMM_ERR_NOT_SUPPORT;
}

/**
 * @internal      fal_tiger_port_parallel_detection_get
 * @endinternal
 *
 * @brief         get serdes parallel detection state
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -parallel detection state enable/disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_port_parallel_detection_get(yt_unit_t unit, yt_port_t port,yt_enable_t *pEnable)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (CAL_IS_SERDES(unit, port) && (ret == CMM_ERR_OK))
    {
        yt_phy_parallel_detection_get(cfg, pEnable);
        return ret;
    }

    return CMM_ERR_NOT_SUPPORT;
}

/**
 * @internal      fal_tiger_port_dvddio_power_level_set
 * @endinternal
 *
 * @brief         set dvddio power level
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     powePad                -powePad
 * @param[out]    powerLevel             -1.8v,2.5v,3.3v
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_port_dvddio_power_level_set(yt_unit_t unit, yt_dvddio_power_pad_t powerPad, yt_dvddio_power_level_t powerLevel)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t reg_data = 0;
    uint32_t index = 0;
    uint32_t value = 0;

    if (CAL_SWCHIP_ID(unit) == YT_SW_ID_9218)
    {
        if (POWER_PAD_NORMAL ==  powerPad)
        {
            index = 0;
        }
        else if (POWER_PAD_RGMII1 == powerPad)
        {
            index = 2;
        }
        else
        {
            index = 4;
        }
    }
    else if (CAL_SWCHIP_ID(unit) == YT_SW_ID_9215)
    {
        if (POWER_PAD_NORMAL ==  powerPad)
        {
            index = 4;
        }
        else if (POWER_PAD_RGMII1 == powerPad)
        {
            index = 0;
        }
        else
        {
            index = 2;
        }
    }
    else
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    if (POWER_PAD_NORMAL == powerPad)
    {
        if (POWER25V == powerLevel ||POWER33V == powerLevel)
        {
            value = 0x0;
        }
        else
        {
            value = 0x3;
        }
    }
    else
    {
        if (POWER18V == powerLevel)
        {
            value = 0x2;
        }
        else if (POWER25V == powerLevel)
        {
            value = 0x1;
        }
        else
        {
            value = 0x0;
        }
    }
    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, CHIP_IO_LEVEL_MODE_REG, &reg_data), ret);
    reg_data &= ~(0x3<<index);
    reg_data |= (value<<index);
    CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, CHIP_IO_LEVEL_MODE_REG, reg_data), ret);

    ret = fal_rgmii_strength_set(unit, powerPad, powerLevel);
    if (CMM_ERR_OK != ret)
    {
        return ret;
    }
    ret = fal_rgmii_vbias_set(unit, powerPad, powerLevel);

    return ret;
}


/**
 * @internal      fal_tiger_port_dvddio_power_level_get
 * @endinternal
 *
 * @brief         get dvddio power level
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     powePad                -powePad
 * @param[out]    pPowerLevel             -1.8v,2.5v,3.3v
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_port_dvddio_power_level_get(yt_unit_t unit, yt_dvddio_power_pad_t powerPad, yt_dvddio_power_level_t *pPowerLevel)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t reg_data = 0;
    uint8_t index = 0;

    if (CAL_SWCHIP_ID(unit) == YT_SW_ID_9218)
    {
        if (POWER_PAD_NORMAL ==  powerPad)
        {
            index = 0;
        }
        else if (POWER_PAD_RGMII1 == powerPad)
        {
            index = 2;
        }
        else
        {
            index = 4;
        }
    }
    else if (CAL_SWCHIP_ID(unit) == YT_SW_ID_9215)
    {
        if (POWER_PAD_NORMAL ==  powerPad)
        {
            index = 4;
        }
        else if (POWER_PAD_RGMII1 == powerPad)
        {
            index = 0;
        }
        else
        {
            index = 2;
        }
    }
    else
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, CHIP_IO_LEVEL_MODE_REG, &reg_data), ret);    
    reg_data = (0x3) & (reg_data>>index);
    if (POWER_PAD_NORMAL == powerPad)
    {
        if (reg_data == 0)
        {
            *pPowerLevel = POWER33V_OR_25V;
        }
        else
        {
            *pPowerLevel = POWER18V;
        }
    }
    else
    {
        if (reg_data == 0)
        {
            *pPowerLevel = POWER33V;
        }
        else if (reg_data == 1)
        {
            *pPowerLevel = POWER25V;
        }
        else
        {
            *pPowerLevel = POWER18V;
        }
    }

    return ret;
}
