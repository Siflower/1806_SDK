/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "phy_yt8531.h"
#include "hal_ctrl.h"
#include "yt_error.h"
#include "yt_util.h"

yt_phy_drv_func_t yt8531_drv_func =
{
    .phy_num = 1,
    .phy_init = yt8531_init,
    .phy_restart = yt8531_restart,
    .phy_enable_set = yt8531_enable_set,
    .phy_enable_get = yt8531_enable_get,
    .phy_medium_set = yt8531_medium_set,
    .phy_medium_get = yt8531_medium_get,
    .phy_autoNeg_enable_set = yt8531_autoNeg_enable_set,
    .phy_autoNeg_enable_get = yt8531_autoNeg_enable_get,
    .phy_autoNeg_ability_set = yt8531_autoNeg_ability_set,
    .phy_autoNeg_ability_get = yt8531_autoNeg_ability_get,
    .phy_force_speed_duplex_set = yt8531_force_speed_duplex_set,
    .phy_force_speed_duplex_get = yt8531_force_speed_duplex_get,
    .phy_link_status_get = yt8531_link_status_get,
    .phy_eee_enable_set = yt8531_eee_enable_set,
    .phy_eee_enable_get = yt8531_eee_enable_get,
    .phy_combo_mode_set = yt8531_combo_mode_set,
    .phy_combo_mode_get = yt8531_combo_mode_get,
    .phy_cable_diag = yt8531_cable_diag,
    .phy_interrupt_status_get = yt8531_interrupt_status_get,
    .phy_test_template = yt8531_test_template,
};

yt_phy_drv_t yt8531_drv =
{
    .chip_id = YT_PHY_ID_8531,
    .chip_model = YT_PHY_MODEL_8531,
    .pDrvFunc = &yt8531_drv_func
};

yt_ret_t yt8531_init(yt_unit_t unit)
{
    CMM_UNUSED_PARAM(unit);
    return CMM_ERR_OK;
}

yt_ret_t yt8531_restart(yt_unit_t unit, uint8_t phy_addr)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;
    uint16_t orgData = 0;

    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, PHY_BASE_CTRL_REG_0, &orgData), ret);
    phydata = orgData | 1 << 15;
    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, PHY_BASE_CTRL_REG_0, phydata), ret);
    if(orgData & (1<<11))/*keep power down*/
    {
        HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, PHY_BASE_CTRL_REG_0, orgData);
    }

    return CMM_ERR_OK;
}

yt_ret_t yt8531_enable_set(yt_unit_t unit, uint8_t phy_addr, yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;

    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, PHY_BASE_CTRL_REG_0, &phydata), ret);

    if(YT_ENABLE == enable)
    {
        phydata &= 0xf7ff;
        phydata |= 0x200;
    }
    else
    {
        phydata |= 0x800;
    }
    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, PHY_BASE_CTRL_REG_0, phydata), ret);

    return CMM_ERR_OK;
}

yt_ret_t yt8531_enable_get(yt_unit_t unit, uint8_t phy_addr, yt_enable_t *pEnable)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;
    
    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, PHY_BASE_CTRL_REG_0, &phydata), ret);
    *pEnable = ((phydata >> 11) & 0x1) ? YT_DISABLE : YT_ENABLE;

    return CMM_ERR_OK;
}

yt_ret_t yt8531_medium_set(yt_unit_t unit, uint8_t phy_addr, yt_port_medium_t medium)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(phy_addr);
    CMM_UNUSED_PARAM(medium);
    return CMM_ERR_OK;
}

yt_ret_t yt8531_medium_get(yt_unit_t unit, uint8_t phy_addr, yt_port_medium_t *pMedium)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(phy_addr);
    CMM_UNUSED_PARAM(pMedium);
    return CMM_ERR_OK;
}

yt_ret_t yt8531_autoNeg_enable_set(yt_unit_t unit, uint8_t phy_addr, yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;
    uint16_t phy_reg_0 = 0;
    uint16_t orgData = 0;

    if(YT_ENABLE == enable) /*config AN mode*/
    {
        phy_reg_0 |= 1 << 12;
    }
    else
    {
        phy_reg_0 &= ~(1 << 12); 
    }

    phy_reg_0 |= 1 << 15;
    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, PHY_BASE_CTRL_REG_0, &orgData), ret);
    phydata = (orgData & (~(0x1000))) | phy_reg_0;
    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, PHY_BASE_CTRL_REG_0, phydata), ret);
    if(orgData & (1<<11))/*keep power down*/
    {
        phydata = (phydata & ~(1<<15)) | 1<<11;
        HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, PHY_BASE_CTRL_REG_0, phydata);
    }

    return CMM_ERR_OK;
}

yt_ret_t yt8531_autoNeg_enable_get(yt_unit_t unit, uint8_t phy_addr, yt_enable_t *pEnable)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;

    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, PHY_BASE_CTRL_REG_0, &phydata), ret);
    *pEnable = (phydata >> 12 & 0x01) ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t yt8531_autoNeg_ability_set(yt_unit_t unit, uint8_t phy_addr, yt_port_an_ability_t ability)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;
    uint16_t phy_reg_4 = 0;
    uint16_t phy_reg_9 = 0; 
    uint16_t orgData = 0;

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

    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, PHY_AUTONEG_REG_4, &phydata), ret);
    phydata = (phydata & (~(0xde0))) | phy_reg_4;
    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, PHY_AUTONEG_REG_4, phydata), ret);

    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, PHY_1000BASE_CTRL_REG_9, &phydata), ret);
    phydata = (phydata & (~(0x200))) | phy_reg_9;
    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, PHY_1000BASE_CTRL_REG_9, phydata), ret);

    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, PHY_BASE_CTRL_REG_0, &orgData), ret);
    phydata = orgData | (1 << 15);/*reset*/
    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, PHY_BASE_CTRL_REG_0, phydata), ret);
    if(orgData & (1<<11))/*keep power down*/
    {
        phydata = (phydata & ~(1<<15)) | 1<<11;
        HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, PHY_BASE_CTRL_REG_0, phydata);
    }

    return CMM_ERR_OK;
}

yt_ret_t yt8531_autoNeg_ability_get(yt_unit_t unit, uint8_t phy_addr, yt_port_an_ability_t *pAbility)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phy_reg_4 = 0;
    uint16_t phy_reg_9 = 0;

    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, PHY_AUTONEG_REG_4, &phy_reg_4), ret);
    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, PHY_1000BASE_CTRL_REG_9, &phy_reg_9), ret);

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

yt_ret_t yt8531_force_speed_duplex_set(yt_unit_t unit, uint8_t phy_addr, yt_port_speed_duplex_t speed_dup)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;
    uint16_t phy_reg_0 = 0;
    uint16_t orgData = 0;

    phy_reg_0 &= ~(1 << 12);/* disable an */

    switch(speed_dup)
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
            phy_reg_0 |= 1 << 6;
            phy_reg_0 &= ~(1 << 13);
            phy_reg_0 |= 1 << 8;
            break;
        default:
            return CMM_ERR_INPUT;
    }
    phy_reg_0 |= 1 << 15;

    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, PHY_BASE_CTRL_REG_0, &orgData), ret);
    phydata = (orgData & (~(0xb140))) | phy_reg_0;
    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, PHY_BASE_CTRL_REG_0, phydata), ret);
    if(orgData & (1<<11))/*keep power down*/
    {
        phydata = (phydata & ~(1<<15)) | 1<<11;
        HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, PHY_BASE_CTRL_REG_0, phydata);
    }

    return CMM_ERR_OK;
}

yt_ret_t yt8531_force_speed_duplex_get(yt_unit_t unit, uint8_t phy_addr, yt_port_speed_duplex_t *pSpeedDup)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phy_reg_0 = 0;

    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, PHY_BASE_CTRL_REG_0, &phy_reg_0), ret);

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

yt_ret_t yt8531_link_status_get(yt_unit_t unit, uint8_t phy_addr, yt_port_linkStatus_all_t *pLinkStatus)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t phydata = 0;

    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, PHY_LINK_STATUS_REG_17, &phydata), ret);

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
        return CMM_ERR_FAIL;
    }

    return CMM_ERR_OK;
}

yt_ret_t yt8531_eee_enable_set(yt_unit_t unit, uint8_t phy_addr, yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t regData = 0;

    if(enable == YT_ENABLE)
    {
        regData = 0x6;
    }

    CMM_ERR_CHK(ext_phy_mmd_reg_write(unit, phy_addr, 7, 0x3c, regData), ret);

    return CMM_ERR_OK;
}

yt_ret_t yt8531_eee_enable_get(yt_unit_t unit, uint8_t phy_addr, yt_enable_t *pEnable)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t regData = 0;

    CMM_ERR_CHK(ext_phy_mmd_reg_read(unit, phy_addr, 7, 0x3c, &regData), ret);

    *pEnable = (regData & 0x6) ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t yt8531_combo_mode_set(yt_unit_t unit, uint8_t phy_addr, yt_combo_mode_t mode)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(phy_addr);
    CMM_UNUSED_PARAM(mode);
    return CMM_ERR_OK;
}

yt_ret_t yt8531_combo_mode_get(yt_unit_t unit, uint8_t phy_addr, yt_combo_mode_t *pMode)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(phy_addr);
    CMM_UNUSED_PARAM(pMode);
    return CMM_ERR_OK;
}

yt_ret_t yt8531_cable_diag(yt_unit_t unit, uint8_t phy_addr, yt_port_cableDiag_t *pCableDiagStatus)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t orgSleep = 0;
    uint16_t orgData = 0;
    uint16_t pairStatus = 0;
    uint32_t maxLoop = 10;
    uint16_t oriSelSdsPhy = 0;
    yt_bool_t getResult = 0;
    uint8_t i;

    CMM_ERR_CHK(ext_phy_ext_reg_read(unit, phy_addr, 0xa000, &oriSelSdsPhy), ret); /*select sds or phy*/
    CMM_ERR_CHK(ext_phy_ext_reg_read(unit, phy_addr, 0x27, &orgSleep), ret);/*sleep status*/
    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, PHY_BASE_CTRL_REG_0, &orgData), ret);

    CMM_ERR_CHK(ext_phy_ext_reg_write(unit, phy_addr, 0xa000, (oriSelSdsPhy&(~(1<<1)))), ret);
    CMM_ERR_CHK(ext_phy_ext_reg_write(unit, phy_addr, 0x98, 0xb0a6), ret);
    CMM_ERR_CHK(ext_phy_ext_reg_write(unit, phy_addr, 0x27, (orgSleep&(~(1<<15)))), ret);
    yt8531_restart(unit, phy_addr);

    CMM_ERR_CHK(ext_phy_ext_reg_write(unit, phy_addr, 0x80, 0x924b), ret);/*start*/
    CMM_ERR_CHK(ext_phy_ext_reg_read(unit, phy_addr, 0x84, &pairStatus), ret);
    while(maxLoop--)
    {
        if(!((pairStatus>>15) & 0x1))/*finish*/
        {
            getResult = 1;
            break;
        }
        CMM_ERR_CHK(ext_phy_ext_reg_read(unit, phy_addr, 0x84, &pairStatus), ret);
    }

    if(!getResult)
    {
        /*back to original setting*/
        ext_phy_ext_reg_write(unit, phy_addr, 0x27, orgSleep);
        ext_phy_ext_reg_write(unit, phy_addr, 0xa000, oriSelSdsPhy);
        HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, PHY_BASE_CTRL_REG_0, orgData);
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
            CMM_ERR_CHK(ext_phy_ext_reg_read(unit, phy_addr, 0x87+i, &pCableDiagStatus->pair_length[i]), ret);
        }
    }
    /*TODO:convert OPEN status to OK when all pairs length are same*/

    /*back to original setting*/
    ext_phy_ext_reg_write(unit, phy_addr, 0x27, orgSleep);
    ext_phy_ext_reg_write(unit, phy_addr, 0xa000, oriSelSdsPhy);
    HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, PHY_BASE_CTRL_REG_0, orgData);

    return CMM_ERR_OK;
}

yt_ret_t yt8531_interrupt_status_get(yt_unit_t unit, uint8_t phy_addr, uint16_t *pStatusData)
{
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, 0x13, pStatusData), ret);

    return CMM_ERR_OK;
}

yt_ret_t yt8531_test_template(yt_unit_t unit, uint8_t phy_addr, yt_utp_template_testmode_t mode)
{
    cmm_err_t ret = CMM_ERR_OK;

    if(mode < YT_UTP_TEMPLATE_TMODE_10M_10MSINE || mode > YT_UTP_TEMPLATE_TMODE_1000M_T4)
    {
        return CMM_ERR_INPUT;
    }

    /*for common setting*/
    CMM_ERR_CHK(ext_phy_ext_reg_write(unit, phy_addr, 0x27, 0x2026), ret);/*disable sleep*/
    CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, 0x10, 0x2), ret);/*mdi channel A*/

    if(mode < YT_UTP_TEMPLATE_TMODE_100M_MDI)
    {
        CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, PHY_BASE_CTRL_REG_0, 0x8100), ret);
    }
    else if(mode >= YT_UTP_TEMPLATE_TMODE_1000M_T1 && mode <= YT_UTP_TEMPLATE_TMODE_1000M_T4)
    {
        CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, PHY_BASE_CTRL_REG_0, 0x8140), ret);
    }

    switch(mode)
    {
        case YT_UTP_TEMPLATE_TMODE_10M_10MSINE:
            CMM_ERR_CHK(ext_phy_ext_reg_write(unit, phy_addr, 0xa, 0x209), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_10M_PRANDOM:
            CMM_ERR_CHK(ext_phy_ext_reg_write(unit, phy_addr, 0xa, 0x20a), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_10M_LINKPULSE:
            CMM_ERR_CHK(ext_phy_ext_reg_write(unit, phy_addr, 0xa, 0x20b), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_10M_5MSINE:
            CMM_ERR_CHK(ext_phy_ext_reg_write(unit, phy_addr, 0xa, 0x20c), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_10M_NORMAL:
            CMM_ERR_CHK(ext_phy_ext_reg_write(unit, phy_addr, 0xa, 0x20d), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_100M_MDI:
            CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, PHY_BASE_CTRL_REG_0, 0xa100), ret);
            CMM_ERR_CHK(ext_phy_ext_reg_write(unit, phy_addr, 0x51, 0x4a9), ret);
            CMM_ERR_CHK(ext_phy_ext_reg_write(unit, phy_addr, 0x57, 0x274c), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_100M_MDIX:
            CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, 0x10, 0x22), ret);
            CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, PHY_BASE_CTRL_REG_0, 0xa100), ret);
            CMM_ERR_CHK(ext_phy_ext_reg_write(unit, phy_addr, 0x51, 0x4a9), ret);
            CMM_ERR_CHK(ext_phy_ext_reg_write(unit, phy_addr, 0x57, 0x274c), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_1000M_T1:
            CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, 0x9, 0x2200), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_1000M_T2:
            CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, 0x9, 0x5a00), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_1000M_T3:
            CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, 0x9, 0x7200), ret);
            break;
        case YT_UTP_TEMPLATE_TMODE_1000M_T4:
            CMM_ERR_CHK(HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, 0x9, 0x8200), ret);
            CMM_ERR_CHK(ext_phy_ext_reg_write(unit, phy_addr, 0x51, 0x14a2), ret);
            break;
        default:
            return CMM_ERR_INPUT;
    }

    return CMM_ERR_OK;
}

