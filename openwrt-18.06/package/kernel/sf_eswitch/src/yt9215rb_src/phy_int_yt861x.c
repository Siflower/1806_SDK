/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "phy_int_yt861x.h"
#include "hal_ctrl.h"
#include "yt_error.h"
#include "yt_util.h"

static uint8_t intPhy861xSleep = 1;

const yt_phy_drv_func_t int_yt861x_drv_func =
{
    .phy_num = 8,
    .phy_init = int_yt861x_init,
    .phy_restart = int_yt861x_restart,
    .phy_enable_set = int_yt861x_enable_set,
    .phy_enable_get = int_yt861x_enable_get,
    .phy_medium_set = int_yt861x_medium_set,
    .phy_medium_get = int_yt861x_medium_get,
    .phy_autoNeg_enable_set = int_yt861x_autoNeg_enable_set,
    .phy_autoNeg_enable_get = int_yt861x_autoNeg_enable_get,
    .phy_autoNeg_ability_set = int_yt861x_autoNeg_ability_set,
    .phy_autoNeg_ability_get = int_yt861x_autoNeg_ability_get,
    .phy_force_speed_duplex_set = int_yt861x_force_speed_duplex_set,
    .phy_force_speed_duplex_get = int_yt861x_force_speed_duplex_get,
    .phy_link_status_get = int_yt861x_link_status_get,
    .phy_eee_enable_set = int_yt861x_eee_enable_set,
    .phy_eee_enable_get = int_yt861x_eee_enable_get,
    .phy_eee_status_get = int_yt861x_eee_status_get,
    .phy_combo_mode_set = int_yt861x_combo_mode_set,
    .phy_combo_mode_get = int_yt861x_combo_mode_get,
    .phy_cable_diag_start = int_yt861x_cable_diag_start,
    .phy_cable_diag_result_get = int_yt861x_cable_diag_result_get,
    .phy_interrupt_status_get = int_yt861x_interrupt_status_get,
    .phy_test_template = int_yt861x_test_template,
    .phy_crossover_mode_set = int_yt861x_crossover_mode_set,
    .phy_crossover_mode_get = int_yt861x_crossover_mode_get,
    .phy_crossover_status_get = int_yt861x_crossover_status_get,
    .phy_loopback_set = int_yt861x_loopback_set,
    .phy_green_start = int_yt861x_green_start,
    .phy_green_result_get = int_yt861x_green_result_get,
    .phy_smart_downgrade_set = int_yt861x_smart_downgrade_set,
    .phy_smart_downgrade_get = int_yt861x_smart_downgrade_get,
    .phy_parallel_detection_set = int_yt861x_parallel_detection_set,
    .phy_parallel_detection_get = int_yt861x_parallel_detection_get,
    .phy_utp_snr_get = int_yt861x_utp_snr_get,
    .phy_chip_mode_set = int_yt861x_chip_mode_set,
    .phy_chip_mode_get = int_yt861x_chip_mode_get,
    .phy_fc_autoneg_cfg_set = int_yt861x_fc_autoneg_cfg_set,
};

const yt_phy_drv_t int_yt861x_drv =
{
    .phyChipId = YT_PHY_ID_INT861X,
    .phyChipModel = YT_PHY_MODEL_INT861X,
    .pDrvFunc = &int_yt861x_drv_func
};

yt_ret_t int_yt861x_init(yt_phy_comm_cfg_t cfg)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t phy_addr;
    uint16_t regData = 0;
    static yt_bool_t initFlag = 0;

    if(initFlag)
    {
        return CMM_ERR_OK;
    }

#if defined(SWITCH_SERIES_TIGER)
    for(phy_addr = 0; phy_addr < int_yt861x_drv_func.phy_num; phy_addr++)
    {
        /*vga init*/
        cfg.phyAddr = phy_addr; 
        if (CAL_SWCHIP_ID(cfg.unit) != YT_SW_ID_9218)
        {
            CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x50, &regData), ret);
            regData &= 0xF3FF;
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x50, regData), ret);
        }

        /*CSD ca*/
        CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x408, &regData), ret);
        regData &= 0x0FFFE;
        CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x408, regData), ret);

        /*UTP init*/
        CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x29, &regData), ret);
        regData &= (~(0x3F));
        regData |= 0x8;
        CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x29, regData), ret);

        /*Green Init(CLD)*/
        CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x3a9, &regData), ret);
        regData &= (~(0x3F));
        regData |= 0x17;
        CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x3a9, regData), ret);
    }

#elif defined(SWITCH_SERIES_SHARK)
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(ret); 
    CMM_UNUSED_PARAM(phy_addr);
    CMM_UNUSED_PARAM(regData);
#endif

    initFlag = 1;

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_restart(yt_phy_comm_cfg_t cfg)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;
    uint16_t orgData = 0;

    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_BASE_CTRL_REG_0, &orgData), ret);
    phydata = (orgData | 1 << 15);
    CMM_ERR_CHK(phy_mii_reg_write(cfg, PHY_BASE_CTRL_REG_0, phydata), ret);
    if(orgData & (1<<11))/*keep power down*/
    {
        phy_mii_reg_write(cfg, PHY_BASE_CTRL_REG_0, orgData);
    }

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_enable_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;

    CMM_UNUSED_PARAM(phyMode);
    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_BASE_CTRL_REG_0, &phydata), ret);

    if(YT_ENABLE == enable)
    {
        phydata &= 0xf7ff;
        phydata |= 0x200;
    }
    else
    {
        phydata |= 0x800;
    }
    CMM_ERR_CHK(phy_mii_reg_write(cfg, PHY_BASE_CTRL_REG_0, phydata), ret);

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_enable_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;

    CMM_UNUSED_PARAM(phyMode);
    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_BASE_CTRL_REG_0, &phydata), ret);
    *pEnable = ((phydata >> 11) & 0x1) ? YT_DISABLE : YT_ENABLE;

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_medium_set(yt_phy_comm_cfg_t cfg, yt_port_medium_t medium)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(medium);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_yt861x_medium_get(yt_phy_comm_cfg_t cfg, yt_port_medium_t *pMedium)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(pMedium);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_yt861x_autoNeg_enable_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;
    uint16_t phy_reg_0 = 0;
    uint16_t orgData = 0;

    CMM_UNUSED_PARAM(phyMode);
    if(YT_ENABLE == enable) /*config AN mode*/
    {
        phy_reg_0 |= 1 << 12;
    }
    else
    {
        phy_reg_0 &= ~(1 << 12); 
    }

    phy_reg_0 |= 1 << 15;
    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_BASE_CTRL_REG_0, &orgData), ret);
    phydata = (orgData & (~(0x1000))) | phy_reg_0;
    CMM_ERR_CHK(phy_mii_reg_write(cfg, PHY_BASE_CTRL_REG_0, phydata), ret);
    if(orgData & (1<<11))/*keep power down*/
    {
        phydata = (phydata & ~(1<<15)) | 1<<11;
        phy_mii_reg_write(cfg, PHY_BASE_CTRL_REG_0, phydata);
    }

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_autoNeg_enable_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;

    CMM_UNUSED_PARAM(phyMode);
    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_BASE_CTRL_REG_0, &phydata), ret);
    *pEnable = (phydata >> 12 & 0x01) ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_autoNeg_ability_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t ability)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;
    uint16_t phy_reg_4 = 0;
    uint16_t phy_reg_9 = 0; 
    uint16_t orgData = 0;

    CMM_UNUSED_PARAM(phyMode);
    if(ability.asyFC_en == 1) // config asymmetric flow control
    {
        phy_reg_4 |= 1 << 11; 
    }
    else
    {
        phy_reg_4 &= ~(1 << 11); 
    }

    if(ability.fc_en == 1) // config flow control
    {
        phy_reg_4 |= 1 << 10; 
    }
    else
    {
        phy_reg_4 &= ~(1 << 10); 
    }

    if(ability.half_10_en == 1) // config 10 half
    {
        phy_reg_4 |= 1 << 5;
    }
    else
    {
        phy_reg_4 &= ~(1 << 5);
    }

    if(ability.full_10_en == 1) //config 10 full
    {
        phy_reg_4 |= 1 << 6;
    }
    else
    {
        phy_reg_4 &= ~(1 << 6);
    }

    if(ability.half_100_en == 1) //config 100 half
    {
        phy_reg_4 |= 1 << 7;
    }
    else
    {
        phy_reg_4 &= ~(1 << 7);
    }

    if(ability.full_100_en == 1) //config 100 full
    {
        phy_reg_4 |= 1 << 8;
    }
    else
    {
        phy_reg_4 &= ~(1 << 8);
    }

    if(ability.full_1000_en == 1) //config 1000 full
    {
        phy_reg_9 |= 1 << 9;
    }
    else
    {
        phy_reg_9 &= ~(1 << 9);
    }

    if (ability.full_2500_en == 1)
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_AUTONEG_REG_4, &phydata), ret);
    phydata = (phydata & (~(0xde0))) | phy_reg_4;
    CMM_ERR_CHK(phy_mii_reg_write(cfg, PHY_AUTONEG_REG_4, phydata), ret);

    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_1000BASE_CTRL_REG_9, &phydata), ret);
    phydata = (phydata & (~(0x200))) | phy_reg_9;
    CMM_ERR_CHK(phy_mii_reg_write(cfg, PHY_1000BASE_CTRL_REG_9, phydata), ret);

    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_BASE_CTRL_REG_0, &orgData), ret);
    phydata = orgData | (1 << 15);/*reset*/
    CMM_ERR_CHK(phy_mii_reg_write(cfg, PHY_BASE_CTRL_REG_0, phydata), ret);
    if(orgData & (1<<11))/*keep power down*/
    {
        phydata = (phydata & ~(1<<15)) | 1<<11;
        phy_mii_reg_write(cfg, PHY_BASE_CTRL_REG_0, phydata);
    }

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_autoNeg_ability_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t *pAbility)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phy_reg_4 = 0;
    uint16_t phy_reg_9 = 0;

    CMM_UNUSED_PARAM(phyMode);
    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_AUTONEG_REG_4, &phy_reg_4), ret);
    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_1000BASE_CTRL_REG_9, &phy_reg_9), ret);

    if((phy_reg_9 >> 9) & 0x1) 
    {
        pAbility->full_1000_en = 1;
    }
    else
    {
        pAbility->full_1000_en = 0;
    }

    if((phy_reg_4 >> 10) & 0x1) 
    {
        pAbility->fc_en = 1;
    }
    else
    {
        pAbility->fc_en = 0;
    }

    if((phy_reg_4 >> 11) & 0x1) 
    {
        pAbility->asyFC_en = 1;
    }
    else
    {
        pAbility->asyFC_en = 0;
    }

    if((phy_reg_4 >> 8) & 0x1) 
    {
        pAbility->full_100_en = 1;
    }
    else
    {
        pAbility->full_100_en = 0;
    }

    if((phy_reg_4 >> 7) & 0x1) 
    {
        pAbility->half_100_en = 1;
    }
    else
    {
        pAbility->half_100_en = 0;
    }

    if((phy_reg_4 >> 6) & 0x1) 
    {
        pAbility->full_10_en = 1;
    }
    else
    {
        pAbility->full_10_en = 0;
    }

    if((phy_reg_4 >> 5) & 0x1) 
    {
        pAbility->half_10_en = 1;
    }
    else
    {
        pAbility->half_10_en = 0;
    }

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_force_speed_duplex_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t speedDup)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;
    uint16_t phy_reg_0 = 0;
    uint16_t orgData = 0;

    CMM_UNUSED_PARAM(phyMode);
    phy_reg_0 &= ~(1 << 12);/* disable an */

    switch(speedDup)
    {
        case PORT_SPEED_DUP_10HALF:
            phy_reg_0 &= ~(1 << 6);
            phy_reg_0 &= ~(1 << 13);
            phy_reg_0 &= ~(1 << 8);
            break;
        case PORT_SPEED_DUP_10FULL:
            phy_reg_0 &= ~(1 << 6);
            phy_reg_0 &= ~(1 << 13);
            phy_reg_0 |= 1 << 8;
            break;
        case PORT_SPEED_DUP_100HALF:
            phy_reg_0 &= ~(1 << 6);
            phy_reg_0 |= 1 << 13;
            phy_reg_0 &= ~(1 << 8);
            break;
        case PORT_SPEED_DUP_100FULL:
            phy_reg_0 &= ~(1 << 6);
            phy_reg_0 |= 1 << 13;
            phy_reg_0 |= 1 << 8;
            break;
        case PORT_SPEED_DUP_1000FULL:
        default:
            return CMM_ERR_INPUT;
    }
    phy_reg_0 |= 1 << 15;

    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_BASE_CTRL_REG_0, &orgData), ret);
    phydata = (orgData & (~(0xb140))) | phy_reg_0;
    CMM_ERR_CHK(phy_mii_reg_write(cfg, PHY_BASE_CTRL_REG_0, phydata), ret);
    if(orgData & (1<<11))/*keep power down*/
    {
        phydata = (phydata & ~(1<<15)) | 1<<11;
        phy_mii_reg_write(cfg, PHY_BASE_CTRL_REG_0, phydata);
    }

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_force_speed_duplex_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t *pSpeedDup)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phy_reg_0 = 0;

    CMM_UNUSED_PARAM(phyMode);
    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_BASE_CTRL_REG_0, &phy_reg_0), ret);
    if((phy_reg_0 >> 8) & 0x1)
    {
        if(((phy_reg_0 >> 13) & 0x1) && (!((phy_reg_0 >> 6) & 0x1))) 
        {
            *pSpeedDup = PORT_SPEED_DUP_100FULL;
        }
        else if(((phy_reg_0 >> 6) & 0x1) && (!((phy_reg_0 >> 13) & 0x1)))
        {
            *pSpeedDup = PORT_SPEED_DUP_1000FULL;
        }
        else if((!((phy_reg_0 >> 6) & 0x1)) && (!((phy_reg_0 >> 13) & 0x1)))
        {
            *pSpeedDup = PORT_SPEED_DUP_10FULL;
        }
    }
    else
    {
        if(((phy_reg_0 >> 13) & 0x1) && (!((phy_reg_0 >> 6) & 0x1))) 
        {
            *pSpeedDup = PORT_SPEED_DUP_100HALF;
        }
        else if((!((phy_reg_0 >> 6) & 0x1)) && (!((phy_reg_0 >> 13) & 0x1)))
        {
            *pSpeedDup = PORT_SPEED_DUP_10HALF;
        }
    }

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_link_status_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_linkStatus_all_t *pLinkStatus)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;

    CMM_UNUSED_PARAM(phyMode);
    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_LINK_STATUS_REG_17, &phydata), ret);

    if((phydata >> 11) & 0x1)
    {
        pLinkStatus->link_duplex = (phydata >> 13) & 0x1;
        pLinkStatus->link_status = (phydata >> 10) & 0x1;
        pLinkStatus->link_speed = (phydata >> 14) & 0x3;
        pLinkStatus->tx_fc_en = (phydata >> 3) & 0x1;
        pLinkStatus->rx_fc_en = (phydata >> 2) & 0x1;
    }
    else
    {
        pLinkStatus->link_duplex = PORT_DUPLEX_HALF;
        pLinkStatus->link_status = PORT_LINK_DOWN;
        pLinkStatus->link_speed = PORT_SPEED_100M;
        pLinkStatus->tx_fc_en = FALSE;
        pLinkStatus->rx_fc_en = FALSE;
    }

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_eee_enable_set(yt_phy_comm_cfg_t cfg, yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t regData = 0;

    if(enable == YT_ENABLE)
    {
        regData = 0x6;
    }

    CMM_ERR_CHK(phy_mmd_reg_write(cfg, 7, 0x3c, regData), ret);
    CMM_ERR_CHK(int_yt861x_restart(cfg), ret);

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_eee_enable_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t regData = 0;

    CMM_ERR_CHK(phy_mmd_reg_read(cfg, 7, 0x3c, &regData), ret);

    *pEnable = (regData & 0x6) ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_eee_status_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t regData = 0;

    CMM_ERR_CHK(phy_mmd_reg_read(cfg, 7, 0x8000, &regData), ret);

    *pEnable = (regData & 0x6) ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_combo_mode_set(yt_phy_comm_cfg_t cfg, yt_combo_mode_t mode)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(mode);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_yt861x_combo_mode_get(yt_phy_comm_cfg_t cfg, yt_combo_mode_t *pMode)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(pMode);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_yt861x_cable_diag_start(yt_phy_comm_cfg_t cfg)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t regData = 0;

    CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x83, 0x300), ret);
    CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x97, 0x6868), ret);
    CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x408, 0x0), ret);
    CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x27, &regData), ret);
    if (((regData >> 15) & 0x1) == 1)
    {
        intPhy861xSleep = 1;
    }
    else
    {
        intPhy861xSleep = 0;
    }
    CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x27, 0x2029), ret);
    CMM_ERR_CHK(phy_mii_reg_read(cfg, 0x0, &regData), ret);
    SET_BIT(regData, 15);
    CMM_ERR_CHK(phy_mii_reg_write(cfg, 0x0, regData), ret);
    CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x80, 0x920b), ret);
        
    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_cable_diag_result_get(yt_phy_comm_cfg_t cfg, yt_port_cableDiag_t *pCableDiagStatus)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t pairStatus = 0;
    uint16_t tmpData;
    uint8_t i = 0;

    CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x84, &pairStatus), ret);
    if (((pairStatus >> 15) & 0x1) == 0x1)
    {
        /* csd detect not finish */
        return CMM_ERR_FAIL;
    }
    for(i = 0; i < 4; i++)
    {
        pCableDiagStatus->pair_status[i] = ((pairStatus >> 2*i) & 0x3);
        if(pCableDiagStatus->pair_status[i] != PORT_CABLE_STATUS_UNKNOWN)
        {
            pCableDiagStatus->pair_valid[i] = 1;
        }

        if(pCableDiagStatus->pair_status[i] == PORT_CABLE_STATUS_SHORT ||
            pCableDiagStatus->pair_status[i] == PORT_CABLE_STATUS_OPEN)
        {
            CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x87+i, &pCableDiagStatus->pair_length[i]), ret);
        }
    }
    CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x27, &tmpData), ret);
    if (intPhy861xSleep == 1)
    {
        SET_BIT(tmpData, 15);
        CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x27, tmpData), ret);
    }
    
    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_interrupt_status_get(yt_phy_comm_cfg_t cfg, uint16_t *pStatusData)
{
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(phy_mii_reg_read(cfg, 0x13, pStatusData), ret);

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_test_template(yt_phy_comm_cfg_t cfg, yt_utp_template_testmode_t mode)
{
    cmm_err_t ret = CMM_ERR_OK;

    if(mode < YT_UTP_TEMPLATE_TMODE_10M_10MSINE || mode > YT_UTP_TEMPLATE_TMODE_1000M_T4)
    {
        return CMM_ERR_INPUT;
    }

    /*for common setting*/
    CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x27, 0x2026), ret);/*disable sleep*/
    CMM_ERR_CHK(phy_mii_reg_write(cfg, 0x10, 0x2), ret);/*mdi channel A*/

    if(mode < YT_UTP_TEMPLATE_TMODE_100M_MDI)
    {
        CMM_ERR_CHK(phy_mii_reg_write(cfg, PHY_BASE_CTRL_REG_0, 0x8100), ret);
    }
    else if(mode >= YT_UTP_TEMPLATE_TMODE_1000M_T1 && mode <= YT_UTP_TEMPLATE_TMODE_1000M_T4)
    {
        CMM_ERR_CHK(phy_mii_reg_write(cfg, PHY_BASE_CTRL_REG_0, 0x8140), ret);
    }

    switch(mode)
    {
        case YT_UTP_TEMPLATE_TMODE_10M_10MSINE:
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0xa, 0x209), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_10M_PRANDOM:
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0xa, 0x20a), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_10M_LINKPULSE:
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0xa, 0x20b), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_10M_5MSINE:
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0xa, 0x20c), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_10M_NORMAL:
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0xa, 0x20d), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_100M_MDI:
            CMM_ERR_CHK(phy_mii_reg_write(cfg, PHY_BASE_CTRL_REG_0, 0xa100), ret);
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x51, 0x4a9), ret);
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x57, 0x274c), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_100M_MDIX:
            CMM_ERR_CHK(phy_mii_reg_write(cfg, 0x10, 0x22), ret);
            CMM_ERR_CHK(phy_mii_reg_write(cfg, PHY_BASE_CTRL_REG_0, 0xa100), ret);
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x51, 0x4a9), ret);
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x57, 0x274c), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_1000M_T1:
            CMM_ERR_CHK(phy_mii_reg_write(cfg, 0x9, 0x2200), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_1000M_T2:
            CMM_ERR_CHK(phy_mii_reg_write(cfg, 0x9, 0x5a00), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_1000M_T3:
            CMM_ERR_CHK(phy_mii_reg_write(cfg, 0x9, 0x7200), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_1000M_T4:
            CMM_ERR_CHK(phy_mii_reg_write(cfg, 0x9, 0x8200), ret);
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x51, 0x14a2), ret);
            break;
        default:
            return CMM_ERR_INPUT;
    }

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_crossover_mode_set(yt_phy_comm_cfg_t cfg, yt_utp_crossover_mode_t mode)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;

    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_FUNCTION_CTRL_REG_16, &phydata), ret);

    switch(mode)
    {
        case YT_UTP_CROSSOVER_MODE_MDI:
            phydata &= ~(1 << 6);
            phydata &= ~(1 << 5);
            break;
        case YT_UTP_CROSSOVER_MODE_MDIX:
            phydata &= ~(1 << 6);
            phydata |= 1 << 5;
            break;
        case YT_UTP_CROSSOVER_MODE_AUTO:
            phydata |= 1 << 6;
            phydata |= 1 << 5;
            break;
        default:
            return CMM_ERR_INPUT;
        }

    phy_mii_reg_write(cfg, PHY_FUNCTION_CTRL_REG_16, phydata);
    int_yt861x_restart(cfg);

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_crossover_mode_get(yt_phy_comm_cfg_t cfg, yt_utp_crossover_mode_t *pMode)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;

    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_FUNCTION_CTRL_REG_16, &phydata), ret);

    *pMode = (phydata >> 5) & 0x3;

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_crossover_status_get(yt_phy_comm_cfg_t cfg, yt_utp_crossover_status_t *pStatus)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;

    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_LINK_STATUS_REG_17, &phydata), ret);

    if ((phydata >> 11) & 0x1)
    {
        *pStatus = (phydata >> 6) & 0x1;
    }
    else
    {
        return CMM_ERR_FAIL;
    }

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_loopback_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_phy_loopback_mode_t mode)
{
    yt_ret_t ret;
    uint16_t data;

    CMM_UNUSED_PARAM(phyMode);
    switch(mode)
    {
        case YT_PHY_LOOPBACK_MODE_INTERNAL:
            CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_BASE_CTRL_REG_0, &data), ret);
            SET_BIT(data, 14);
            CMM_ERR_CHK(phy_mii_reg_write(cfg, PHY_BASE_CTRL_REG_0, data), ret);
            break;
        case YT_PHY_LOOPBACK_MODE_EXTERNAL:
            /* disable sleep */
            CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x27, &data), ret);
            CLEAR_BIT(data, 15);
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x27, data), ret);

            /* enable external loopback */
            CMM_ERR_CHK(phy_ext_reg_read(cfg, 0xa, &data), ret);
            SET_BIT(data, 4);
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0xa, data), ret);

            /* reset */
            CMM_ERR_CHK(int_yt861x_restart(cfg), ret);
            break;
        case YT_PHY_LOOPBACK_MODE_REMOTE:
            CMM_ERR_CHK(phy_ext_reg_read(cfg, 0xa, &data), ret);
            SET_BIT(data, 5);
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0xa, data), ret);
            break;
        case YT_PHY_LOOPBACK_MODE_DISABLE:
            /* disable internal loopback */
            CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_BASE_CTRL_REG_0, &data), ret);
            CLEAR_BIT(data, 14);
            CMM_ERR_CHK(phy_mii_reg_write(cfg, PHY_BASE_CTRL_REG_0, data), ret);

            /* disable external loopback */
            CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x27, &data), ret);
            SET_BIT(data, 15);
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x27, data), ret);

            CMM_ERR_CHK(phy_ext_reg_read(cfg, 0xa, &data), ret);
            CLEAR_BIT(data, 4);
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0xa, data), ret);

            /* disable remote loopback */
            CMM_ERR_CHK(phy_ext_reg_read(cfg, 0xa, &data), ret);
            CLEAR_BIT(data, 5);
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0xa, data), ret);

            /* reset */
            CMM_ERR_CHK(int_yt861x_restart(cfg), ret);
            break;
        default:
            return CMM_ERR_INPUT;
    }

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_green_start(yt_phy_comm_cfg_t cfg)
{
    yt_ret_t ret;

    CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x3a0, 0xeb24), ret);
    CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x3cc, 0x7001), ret);
    CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x3a9, 0x2f17), ret);
    /* restart auto neg to detect */
    CMM_ERR_CHK(phy_mii_reg_write(cfg, 0x0, 0x9000), ret);

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_green_result_get(yt_phy_comm_cfg_t cfg, yt_port_cableDiag_t *pStatus)
{
    yt_ret_t ret;
    uint16_t regData = 0;
    yt_bool_t pairValid = 0;
    yt_port_cable_status_t cableStatus = 0;
    uint16_t length = 0;
    int32_t index = 0;
    
    CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x3af, &regData), ret);
    regData = regData & 0x1FF;
    if (regData == 0)
    {
        /* CLD not finish, need try again with delay */
        return CMM_ERR_FAIL;
    }

    if (regData >= 0xE1)
    {
        /* incorrect, need CLD one more time */
        length = 0;
        pairValid = 0;
        cableStatus = PORT_CABLE_STATUS_UNKNOWN;
    }
    else
    {
        length = regData;
        pairValid = 1;
        cableStatus = PORT_CABLE_STATUS_OK;
    }
    for(index = 0; index < 4; index++)
    {
        pStatus->pair_status[index] = cableStatus;
        pStatus->pair_valid[index] = pairValid;
        pStatus->pair_length[index] = length * 80;
    }
    
    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_smart_downgrade_set(yt_phy_comm_cfg_t cfg, yt_enable_t enable)
{
    yt_ret_t ret;
    uint16_t phyDataReg14 = 0;

    CMM_ERR_CHK(phy_mii_reg_read(cfg, 0x14, &phyDataReg14), ret);
    if (enable == YT_ENABLE)
    {
        phyDataReg14 |= BIT(5);
    }
    else
    {
        phyDataReg14 &= ~(BIT(5));
    }
    CMM_ERR_CHK(phy_mii_reg_write(cfg, 0x14, phyDataReg14), ret);
    int_yt861x_restart(cfg);
        
    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_smart_downgrade_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable)
{
    yt_ret_t ret;
    uint16_t phyDataReg14 = 0;

    CMM_ERR_CHK(phy_mii_reg_read(cfg, 0x14, &phyDataReg14), ret);
    *pEnable = (phyDataReg14 >> 5 & 0x1) ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_parallel_detection_set(yt_phy_comm_cfg_t cfg, yt_enable_t enable)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(enable);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_yt861x_parallel_detection_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(pEnable);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_yt861x_utp_snr_get(yt_phy_comm_cfg_t cfg, yt_port_speed_t speed, yt_utp_snr_t *pSnrVal)
{
    yt_ret_t ret;
    uint16_t divisor = 0;
    uint16_t regData = 0;
    uint16_t mse0 = 0;
    uint16_t mse1 = 0;
    uint16_t mse2 = 0;
    uint16_t mse3 = 0;
    uint8_t loopTime = 0;
    uint16_t regData1 = 0;
    uint16_t regData2 = 0;

    if (speed == PORT_SPEED_100M)
    {
        divisor = 32768;
    }
    else
    {
        divisor = 29696;
    }

    CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x59, &regData1), ret);
    regData = regData1 | (1<<15);
    CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x59, regData), ret);
    CMM_ERR_CHK(phy_ext_reg_read(cfg, 0xa080, &regData2), ret);
    regData = regData2 & (~(1<<10));
    CMM_ERR_CHK(phy_ext_reg_write(cfg, 0xa080, regData), ret);
    while(loopTime < 100)
    {
        CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x5A, &regData), ret);
        mse0 += (regData & 0x7FFF);
        if (speed == PORT_SPEED_1000M)
        {
            CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x5B, &regData), ret);
            mse1 += (regData & 0x7FFF);
            CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x5C, &regData), ret);
            mse2 += (regData & 0x7FFF);
            CMM_ERR_CHK(phy_ext_reg_read(cfg, 0x5D, &regData), ret);
            mse3 += (regData & 0x7FFF);
        }
        loopTime++;
    }

    if (mse0 != 0)
    {
        pSnrVal->snr[0] = (divisor/(mse0/100));
    }

    if (speed == PORT_SPEED_1000M)
    {
        if (mse1 != 0)
        {
            pSnrVal->snr[1] = (divisor/(mse1/100));
        }

        if (mse2 != 0)
        {
            pSnrVal->snr[2] = (divisor/(mse2/100));
        }

        if (mse3 != 0)
        {
            pSnrVal->snr[3] = (divisor/(mse3/100));
        }
    }

    CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x59, regData1), ret);
    CMM_ERR_CHK(phy_ext_reg_write(cfg, 0xa080, regData2), ret);
    return CMM_ERR_OK;
}

yt_ret_t int_yt861x_chip_mode_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t mode)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(mode);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_yt861x_chip_mode_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t *pMode)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(pMode);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_yt861x_fc_autoneg_cfg_set(yt_phy_comm_cfg_t cfg, yt_extif_mode_t extifMode)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(extifMode);

    return CMM_ERR_OK;
}