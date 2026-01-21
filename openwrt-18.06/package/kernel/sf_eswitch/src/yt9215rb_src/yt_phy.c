/*******************************************************************************
*                                                                              *
*  Copyright (c), 2025, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_phy.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_types.h"
#include "yt_error.h"
#include "yt_cmm.h"
#include "yt_phy.h"
#include "hal_ctrl.h"

void yt_phy_external_smi_phyAddrMask_set(uint32_t phyAddrMask)
{
    phy_external_smi_phyAddrMask_set(phyAddrMask);

    return;
}

yt_ret_t yt_phy_external_smi_cl22_register(smi_cl22_read smiRead, smi_cl22_write smiWrite)
{
    return phy_external_smi_cl22_register(smiRead, smiWrite);
}

yt_ret_t yt_phy_init(yt_phy_comm_cfg_t cfg)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_init(cfg);   
}


yt_ret_t yt_phy_restart(yt_phy_comm_cfg_t cfg)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    
    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_restart(cfg);   
}

yt_ret_t yt_phy_enable_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t enable)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_PHY_CHIP_MODE_RESERVED < phyMode || phyMode < YT_PHY_CHIP_MODE_COPPER), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_enable_set(cfg, phyMode, enable);
}

yt_ret_t yt_phy_enable_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_PHY_CHIP_MODE_RESERVED < phyMode || phyMode < YT_PHY_CHIP_MODE_COPPER), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_enable_get(cfg, phyMode, pEnable);
}

yt_ret_t yt_phy_medium_set(yt_phy_comm_cfg_t cfg, yt_port_medium_t medium)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((PORT_MEDI_END <= medium || medium < PORT_MEDI_COPPER), CMM_ERR_EXCEED_RANGE);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_medium_set(cfg, medium);  
}

yt_ret_t yt_phy_medium_get(yt_phy_comm_cfg_t cfg, yt_port_medium_t *pMedium)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((pMedium == NULL), CMM_ERR_NULL_POINT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_medium_get(cfg, pMedium);  
}

yt_ret_t yt_phy_autoNeg_enable_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t enable)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_PHY_CHIP_MODE_RESERVED < phyMode || phyMode < YT_PHY_CHIP_MODE_COPPER), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_autoNeg_enable_set(cfg, phyMode, enable);
}

yt_ret_t yt_phy_autoNeg_enable_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_PHY_CHIP_MODE_RESERVED < phyMode || phyMode < YT_PHY_CHIP_MODE_COPPER), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_autoNeg_enable_get(cfg, phyMode, pEnable);
}

yt_ret_t yt_phy_autoNeg_ability_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t ability)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_PHY_CHIP_MODE_RESERVED < phyMode || phyMode < YT_PHY_CHIP_MODE_COPPER), CMM_ERR_EXCEED_RANGE);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_autoNeg_ability_set(cfg, phyMode, ability);
}

yt_ret_t yt_phy_autoNeg_ability_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t *pAbility)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_PHY_CHIP_MODE_RESERVED < phyMode || phyMode < YT_PHY_CHIP_MODE_COPPER), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pAbility), CMM_ERR_NULL_POINT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_autoNeg_ability_get(cfg, phyMode, pAbility);
}

yt_ret_t yt_phy_force_speed_duplex_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t speedDup)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_PHY_CHIP_MODE_RESERVED < phyMode || phyMode < YT_PHY_CHIP_MODE_COPPER), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((PORT_SPEED_DUP_END <= speedDup || PORT_SPEED_DUP_10HALF > speedDup), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_force_speed_duplex_set(cfg, phyMode, speedDup);
}

yt_ret_t yt_phy_force_speed_duplex_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t *pSpeedDup)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_PHY_CHIP_MODE_RESERVED < phyMode || phyMode < YT_PHY_CHIP_MODE_COPPER), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((pSpeedDup == NULL), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_force_speed_duplex_get(cfg, phyMode, pSpeedDup);
}

yt_ret_t yt_phy_link_status_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_linkStatus_all_t *pLinkStatus)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_PHY_CHIP_MODE_RESERVED < phyMode || phyMode < YT_PHY_CHIP_MODE_COPPER), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((pLinkStatus == NULL), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_link_status_get(cfg, phyMode, pLinkStatus);
}

yt_ret_t yt_phy_eee_enable_set(yt_phy_comm_cfg_t cfg, yt_enable_t enable)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_eee_enable_set(cfg, enable);
}

yt_ret_t yt_phy_eee_enable_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((pEnable == NULL), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_eee_enable_get(cfg, pEnable);
}

yt_ret_t yt_phy_eee_status_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((pEnable == NULL), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_eee_status_get(cfg, pEnable);
}

yt_ret_t yt_phy_combo_mode_set(yt_phy_comm_cfg_t cfg, yt_combo_mode_t mode)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((mode > COMBO_MODE_FIBER_FIRST || mode < COMBO_MODE_COPPER_FIRST), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_combo_mode_set(cfg, mode);
}

yt_ret_t yt_phy_combo_mode_get(yt_phy_comm_cfg_t cfg, yt_combo_mode_t *pMode)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((pMode == NULL), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_combo_mode_get(cfg, pMode);
}

yt_ret_t yt_phy_cable_diag_start(yt_phy_comm_cfg_t cfg)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_cable_diag_start(cfg);
}

yt_ret_t yt_phy_cable_diag_result_get(yt_phy_comm_cfg_t cfg, yt_port_cableDiag_t *pCableDiagStatus)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((pCableDiagStatus == NULL), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_cable_diag_result_get(cfg, pCableDiagStatus);
}

yt_ret_t yt_phy_interrupt_status_get(yt_phy_comm_cfg_t cfg, uint16_t *pStatusData)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((pStatusData == NULL), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_interrupt_status_get(cfg, pStatusData);
}

yt_ret_t yt_phy_test_template(yt_phy_comm_cfg_t cfg, yt_utp_template_testmode_t mode)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((mode > YT_UTP_TEMPLATE_TMODE_SDS1000M || mode < YT_UTP_TEMPLATE_TMODE_10M_10MSINE), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_test_template(cfg, mode);
}

yt_ret_t yt_phy_crossover_mode_set(yt_phy_comm_cfg_t cfg, yt_utp_crossover_mode_t mode)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((mode > YT_UTP_CROSSOVER_MODE_AUTO || mode < YT_UTP_CROSSOVER_MODE_MDI), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_crossover_mode_set(cfg, mode);
}

yt_ret_t yt_phy_crossover_mode_get(yt_phy_comm_cfg_t cfg, yt_utp_crossover_mode_t *pMode)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((pMode == NULL), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_crossover_mode_get(cfg, pMode);
}

yt_ret_t yt_phy_crossover_status_get(yt_phy_comm_cfg_t cfg, yt_utp_crossover_status_t *pStatus)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((pStatus == NULL), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_crossover_status_get(cfg, pStatus);
}

yt_ret_t yt_phy_loopback_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_phy_loopback_mode_t mode)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_PHY_CHIP_MODE_RESERVED < phyMode || phyMode < YT_PHY_CHIP_MODE_COPPER), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((mode > YT_PHY_LOOPBACK_MODE_DISABLE || mode < YT_PHY_LOOPBACK_MODE_INTERNAL), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_loopback_set(cfg, phyMode, mode);
}

yt_ret_t yt_phy_green_start(yt_phy_comm_cfg_t cfg)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_green_start(cfg);
}

yt_ret_t yt_phy_green_result_get(yt_phy_comm_cfg_t cfg, yt_port_cableDiag_t *pStatus)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((pStatus == NULL), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_green_result_get(cfg, pStatus);
}

yt_ret_t yt_phy_smart_downgrade_set(yt_phy_comm_cfg_t cfg, yt_enable_t enable)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_smart_downgrade_set(cfg, enable);
}

yt_ret_t yt_phy_smart_downgrade_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((pEnable == NULL), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_smart_downgrade_get(cfg, pEnable);
}

yt_ret_t yt_phy_parallel_detection_set(yt_phy_comm_cfg_t cfg, yt_enable_t enable)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_parallel_detection_set(cfg, enable);
}

yt_ret_t yt_phy_parallel_detection_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((pEnable == NULL), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_parallel_detection_get(cfg, pEnable);
}

yt_ret_t yt_phy_utp_snr_get(yt_phy_comm_cfg_t cfg, yt_port_speed_t speed, yt_utp_snr_t *pSnrVal)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((PORT_SPEED_END <= speed || PORT_SPEED_10M > speed), CMM_ERR_INPUT);
    CMM_PARAM_CHK((pSnrVal == NULL), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_utp_snr_get(cfg, speed, pSnrVal);
}

yt_ret_t yt_phy_fc_autoneg_cfg_set(yt_phy_comm_cfg_t cfg, yt_extif_mode_t extifMode)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_EXTIF_MODE_USXGMII_10G < extifMode || YT_EXTIF_MODE_MII > extifMode), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_fc_autoneg_cfg_set(cfg, extifMode);
}

yt_ret_t yt_phy_chip_mode_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t mode)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_PHY_CHIP_MODE_RESERVED <= mode || YT_PHY_CHIP_MODE_COPPER > mode), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_chip_mode_set(cfg, mode);
}

yt_ret_t yt_phy_chip_mode_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t *pMode)
{
    yt_phy_smi_bus_t *smiBus = NULL;

    CMM_PARAM_CHK((phy_comm_param_check(cfg) != CMM_ERR_OK), CMM_ERR_INPUT);
    CMM_PARAM_CHK((pMode == NULL), CMM_ERR_INPUT);

    smiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    CMM_PARAM_CHK((!smiBus || !smiBus->phyDesc[cfg.phyAddr].phyDrv), CMM_ERR_NULL_POINT);

    return smiBus->phyDesc[cfg.phyAddr].phyDrv->phy_chip_mode_get(cfg, pMode);
}
