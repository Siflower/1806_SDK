/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "phy_int_serdes.h"
#include "hal_ctrl.h"
#include "yt_error.h"
#include "yt_util.h"

const yt_phy_drv_func_t int_serdes_drv_func =
{
    .phy_num = 2,
    .phy_init = int_serdes_init,
    .phy_restart = int_serdes_restart,
    .phy_enable_set = int_serdes_enable_set,
    .phy_enable_get = int_serdes_enable_get,
    .phy_medium_set = int_serdes_medium_set,
    .phy_medium_get = int_serdes_medium_get,
    .phy_autoNeg_enable_set = int_serdes_autoNeg_enable_set,
    .phy_autoNeg_enable_get = int_serdes_autoNeg_enable_get,
    .phy_autoNeg_ability_set = int_serdes_autoNeg_ability_set,
    .phy_autoNeg_ability_get = int_serdes_autoNeg_ability_get,
    .phy_force_speed_duplex_set = int_serdes_force_speed_duplex_set,
    .phy_force_speed_duplex_get = int_serdes_force_speed_duplex_get,
    .phy_link_status_get = int_serdes_link_status_get,
    .phy_eee_enable_set = int_serdes_eee_enable_set,
    .phy_eee_enable_get = int_serdes_eee_enable_get,
    .phy_eee_status_get = int_serdes_eee_status_get,
    .phy_combo_mode_set = int_serdes_combo_mode_set,
    .phy_combo_mode_get = int_serdes_combo_mode_get,
    .phy_cable_diag_start = int_serdes_cable_diag_start,
    .phy_cable_diag_result_get = int_serdes_cable_diag_result_get,
    .phy_interrupt_status_get = int_serdes_interrupt_status_get,
    .phy_test_template = int_serdes_test_template,
    .phy_loopback_set = int_serdes_loopback_set,
    .phy_green_start = int_serdes_green_start,
    .phy_green_result_get = int_serdes_green_result_get,
    .phy_smart_downgrade_set = int_serdes_smart_downgrade_set,
    .phy_smart_downgrade_get = int_serdes_smart_downgrade_get,
    .phy_parallel_detection_set = int_serdes_parallel_detection_set,
    .phy_parallel_detection_get = int_serdes_parallel_detection_get,
    .phy_utp_snr_get = int_serdes_utp_snr_get,
    .phy_chip_mode_set = int_serdes_chip_mode_set,
    .phy_chip_mode_get = int_serdes_chip_mode_get,
    .phy_fc_autoneg_cfg_set = int_serdes_fc_autoneg_cfg_set,
};

const yt_phy_drv_t int_serdes_drv =
{
    .phyChipId = YT_PHY_ID_INTSERDES,
    .phyChipModel = YT_PHY_MODEL_INTSERDES,
    .pDrvFunc = &int_serdes_drv_func
};

yt_ret_t int_serdes_init(yt_phy_comm_cfg_t cfg)
{
    CMM_UNUSED_PARAM(cfg);
    return CMM_ERR_OK;
}

yt_ret_t int_serdes_restart(yt_phy_comm_cfg_t cfg)
{
    CMM_UNUSED_PARAM(cfg);

    return CMM_ERR_OK;
}

yt_ret_t int_serdes_enable_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phyData = 0;

    CMM_UNUSED_PARAM(phyMode);
    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_BASE_CTRL_REG_0, &phyData), ret);
    if(YT_ENABLE == enable)
    {
        CLEAR_BIT(phyData, 11);
        SET_BIT(phyData, 9);
    }
    else
    {
        SET_BIT(phyData, 11);
    }
    CMM_ERR_CHK(phy_mii_reg_write(cfg, PHY_BASE_CTRL_REG_0, phyData), ret);

    return CMM_ERR_OK;
}

yt_ret_t int_serdes_enable_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phyData = 0;

    CMM_UNUSED_PARAM(phyMode);
    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_BASE_CTRL_REG_0, &phyData), ret);
    *pEnable = IS_BIT_SET(phyData, 11) ? YT_DISABLE : YT_ENABLE;

    return CMM_ERR_OK;
}

yt_ret_t int_serdes_medium_set(yt_phy_comm_cfg_t cfg, yt_port_medium_t medium)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(medium);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_medium_get(yt_phy_comm_cfg_t cfg, yt_port_medium_t *pMedium)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(pMedium);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_autoNeg_enable_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t enable)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(enable);
    CMM_UNUSED_PARAM(phyMode);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_autoNeg_enable_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(pEnable);
    CMM_UNUSED_PARAM(phyMode);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_autoNeg_ability_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t ability)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(ability);
    CMM_UNUSED_PARAM(phyMode);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_autoNeg_ability_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t *pAbility)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(pAbility);
    CMM_UNUSED_PARAM(phyMode);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_force_speed_duplex_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t speedDup)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(speedDup);
    CMM_UNUSED_PARAM(phyMode);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_force_speed_duplex_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t *pSpeedDup)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(pSpeedDup);
    CMM_UNUSED_PARAM(phyMode);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_link_status_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_linkStatus_all_t *pLinkStatus)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;

    CMM_UNUSED_PARAM(phyMode);
    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_LINK_STATUS_REG_17, &phydata), ret);
    pLinkStatus->link_duplex = (phydata >> 13) & 0x1;
    pLinkStatus->link_status = (phydata >> 10) & 0x1;
    pLinkStatus->link_speed = (phydata >> 14) & 0x3;
    pLinkStatus->tx_fc_en = (phydata >> 6) & 0x1;
    pLinkStatus->rx_fc_en = (phydata >> 7) & 0x1;
    return CMM_ERR_OK;
}

yt_ret_t int_serdes_eee_enable_set(yt_phy_comm_cfg_t cfg, yt_enable_t enable)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(enable);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_eee_enable_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(pEnable);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_eee_status_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(pEnable);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_combo_mode_set(yt_phy_comm_cfg_t cfg, yt_combo_mode_t mode)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(mode);
    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_combo_mode_get(yt_phy_comm_cfg_t cfg, yt_combo_mode_t *pMode)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(pMode);
    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_cable_diag_start(yt_phy_comm_cfg_t cfg)
{
    CMM_UNUSED_PARAM(cfg);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_cable_diag_result_get(yt_phy_comm_cfg_t cfg, yt_port_cableDiag_t *pCableDiagStatus)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(pCableDiagStatus);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_interrupt_status_get(yt_phy_comm_cfg_t cfg, uint16_t *pStatusData)
{
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(phy_mii_reg_read(cfg, 0x13, pStatusData), ret);

    return CMM_ERR_OK;
}

yt_ret_t int_serdes_test_template(yt_phy_comm_cfg_t cfg, yt_utp_template_testmode_t mode)
{
    cmm_err_t ret = CMM_ERR_OK;

    if(mode < YT_UTP_TEMPLATE_TMODE_SDS2500M || mode > YT_UTP_TEMPLATE_TMODE_SDS1000M)
    {
        return CMM_ERR_INPUT;
    }

    switch(mode)
    {
        case YT_UTP_TEMPLATE_TMODE_SDS2500M:
        case YT_UTP_TEMPLATE_TMODE_SDS1000M:
            /*CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x5, 0xc100), ret);*/
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0xa0, 0x8c00), ret);
            break;
        default:
            return CMM_ERR_INPUT;
    }

    return CMM_ERR_OK;
}

yt_ret_t int_serdes_loopback_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_phy_loopback_mode_t mode)
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
            break;
        case YT_PHY_LOOPBACK_MODE_REMOTE:
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x0, 0x470), ret);
            break;
        case YT_PHY_LOOPBACK_MODE_DISABLE:
            CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_BASE_CTRL_REG_0, &data), ret);
            CLEAR_BIT(data, 14);
            CMM_ERR_CHK(phy_mii_reg_write(cfg, PHY_BASE_CTRL_REG_0, data), ret);

            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x0, 0x450), ret);
            break;
        default:
            return CMM_ERR_INPUT;
    }

    return CMM_ERR_OK;
}

yt_ret_t int_serdes_green_start(yt_phy_comm_cfg_t cfg)
{
    CMM_UNUSED_PARAM(cfg);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_green_result_get(yt_phy_comm_cfg_t cfg, yt_port_cableDiag_t *pStatus)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(pStatus);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_smart_downgrade_set(yt_phy_comm_cfg_t cfg, yt_enable_t enable)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(enable);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_smart_downgrade_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(pEnable);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_parallel_detection_set(yt_phy_comm_cfg_t cfg, yt_enable_t enable)
{
    yt_ret_t ret;
    uint16_t regData = 0;

    CMM_ERR_CHK(phy_ext_reg_read(cfg, 0xdb, &regData), ret);
    if (enable)
    {
        SET_BIT(regData, 15);
    }
    else
    {
        CLEAR_BIT(regData, 15);
    }
    CMM_ERR_CHK(phy_ext_reg_write(cfg, 0xdb, regData), ret);

    return CMM_ERR_OK;
}

yt_ret_t int_serdes_parallel_detection_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable)
{
    yt_ret_t ret;
    uint16_t regData = 0;

    CMM_ERR_CHK(phy_ext_reg_read(cfg, 0xdb, &regData), ret);
    regData = (regData>>15) & 0x1;
    *pEnable = regData ? YT_ENABLE : YT_DISABLE;
    return CMM_ERR_OK;
}

yt_ret_t int_serdes_utp_snr_get(yt_phy_comm_cfg_t cfg, yt_port_speed_t speed, yt_utp_snr_t *pSnrVal)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(speed);
    CMM_UNUSED_PARAM(pSnrVal);
    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_chip_mode_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t mode)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(mode);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_chip_mode_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t *pMode)
{
    CMM_UNUSED_PARAM(cfg);
    CMM_UNUSED_PARAM(pMode);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t int_serdes_fc_autoneg_cfg_set(yt_phy_comm_cfg_t cfg, yt_extif_mode_t extifMode)
{
    yt_ret_t ret = CMM_ERR_OK;

    switch(extifMode)
    {
        case YT_EXTIF_MODE_SG_MAC:
            CMM_ERR_CHK(phy_ext_reg_write(cfg, 0x1, 0x2700), ret);
            break;
        default:
            break;
    }
    CMM_ERR_CHK(int_serdes_restart(cfg), ret);

    return CMM_ERR_OK;
}