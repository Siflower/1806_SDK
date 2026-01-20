/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef __PHY_DRV_H__
#define __PHY_DRV_H__

#include "phy_chipdef.h"
#include "yt_error.h"
#include "yt_phy.h"
#include "yt_board.h"

#define PHY_MII_REG_2   (2)
#define PHY_MII_REG_3   (3)

typedef struct yt_phy_drv_func_s
{
    uint8_t phy_num; /* max interface num */
    yt_ret_t (*phy_init)(yt_phy_comm_cfg_t cfg);
    yt_ret_t (*phy_restart)(yt_phy_comm_cfg_t cfg);
    yt_ret_t (*phy_enable_set)(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t, yt_enable_t);
    yt_ret_t (*phy_enable_get)(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable);
    yt_ret_t (*phy_medium_set)(yt_phy_comm_cfg_t cfg, yt_port_medium_t medium);
    yt_ret_t (*phy_medium_get)(yt_phy_comm_cfg_t cfg, yt_port_medium_t *pMedium);
    yt_ret_t (*phy_autoNeg_enable_set)(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t enable);
    yt_ret_t (*phy_autoNeg_enable_get)(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable);
    yt_ret_t (*phy_autoNeg_ability_set)(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t ability);
    yt_ret_t (*phy_autoNeg_ability_get)(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t *pAbility);
    yt_ret_t (*phy_force_speed_duplex_set)(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t speedDup);
    yt_ret_t (*phy_force_speed_duplex_get)(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t *pSpeedDup);
    yt_ret_t (*phy_link_status_get)(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_linkStatus_all_t *pLinkStatus);
    yt_ret_t (*phy_eee_enable_set)(yt_phy_comm_cfg_t cfg, yt_enable_t enable);
    yt_ret_t (*phy_eee_enable_get)(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable);
    yt_ret_t (*phy_eee_status_get)(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable);
    yt_ret_t (*phy_combo_mode_set)(yt_phy_comm_cfg_t cfg, yt_combo_mode_t mode);
    yt_ret_t (*phy_combo_mode_get)(yt_phy_comm_cfg_t cfg, yt_combo_mode_t *pMode);
    yt_ret_t (*phy_cable_diag_start)(yt_phy_comm_cfg_t cfg);
    yt_ret_t (*phy_cable_diag_result_get)(yt_phy_comm_cfg_t cfg, yt_port_cableDiag_t *pCableDiagStatus);
    yt_ret_t (*phy_interrupt_status_get)(yt_phy_comm_cfg_t cfg, uint16_t *pStatusData);
    yt_ret_t (*phy_test_template)(yt_phy_comm_cfg_t cfg, yt_utp_template_testmode_t mode);
    yt_ret_t (*phy_crossover_mode_set)(yt_phy_comm_cfg_t cfg, yt_utp_crossover_mode_t mode);
    yt_ret_t (*phy_crossover_mode_get)(yt_phy_comm_cfg_t cfg, yt_utp_crossover_mode_t *pMode);
    yt_ret_t (*phy_crossover_status_get)(yt_phy_comm_cfg_t cfg, yt_utp_crossover_status_t *pStatus);
    yt_ret_t (*phy_loopback_set)(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_phy_loopback_mode_t mode);
    yt_ret_t (*phy_green_start)(yt_phy_comm_cfg_t cfg);
    yt_ret_t (*phy_green_result_get)(yt_phy_comm_cfg_t cfg, yt_port_cableDiag_t *pStatus);
    yt_ret_t (*phy_smart_downgrade_set)(yt_phy_comm_cfg_t cfg, yt_enable_t enable);
    yt_ret_t (*phy_smart_downgrade_get)(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable);
    yt_ret_t (*phy_parallel_detection_set)(yt_phy_comm_cfg_t cfg, yt_enable_t enable);
    yt_ret_t (*phy_parallel_detection_get)(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable);
    yt_ret_t (*phy_utp_snr_get)(yt_phy_comm_cfg_t cfg, yt_port_speed_t speed, yt_utp_snr_t *pSnrVal);
    yt_ret_t (*phy_fc_autoneg_cfg_set)(yt_phy_comm_cfg_t cfg, yt_extif_mode_t extifMode);
    yt_ret_t (*phy_chip_mode_set)(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t mode);
    yt_ret_t (*phy_chip_mode_get)(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t *pMode);
}yt_phy_drv_func_t;

typedef enum yt_phy_reg_type_e
{
    YT_PHY_REG_TYPE_TOP_EXT,
    YT_PHY_REG_TYPE_PHY_TOP_EXT,
    YT_PHY_REG_TYPE_PHY_MII,
    YT_PHY_REG_TYPE_PHY_EXT,
    YT_PHY_REG_TYPE_PHY_MMD1,
    YT_PHY_REG_TYPE_PHY_MMD3,
    YT_PHY_REG_TYPE_PHY_MMD7,
    YT_PHY_REG_TYPE_SDS_TOP_EXT,
    YT_PHY_REG_TYPE_SG_MII,
    YT_PHY_REG_TYPE_SG_EXT,
    YT_PHY_REG_TYPE_USXG_MII,
    YT_PHY_REG_TYPE_USXG_EXT,
}yt_phy_reg_type_t;

typedef struct yt_phy_drv_s
{
    yt_phy_chip_id_t    phyChipId;
    yt_phy_chip_model_t phyChipModel;
    const yt_phy_drv_func_t   *pDrvFunc;
}yt_phy_drv_t;

typedef struct yt_phy_desc_s {
    yt_phy_chip_model_t phyChipModel;
    uint8_t phyBaseAddr;
    const yt_phy_drv_func_t *phyDrv;
}yt_phy_desc_t;

typedef struct yt_phy_smi_bus_s {
    smi_cl22_write smiWrite;
    smi_cl22_read smiRead;
    /* switch internal phy smi ignore autoLoad */
    uint8_t autoLoad;
    uint32_t phyAddrMask;
    yt_phy_desc_t phyDesc[YT_PHY_MAX_ADDR];
}yt_phy_smi_bus_t;

extern const yt_phy_drv_t *gpPhyDrvList[];

yt_phy_smi_bus_t *phy_smi_bus_get(yt_unit_t unit, yt_phy_smi_type_t smiType);
yt_ret_t phy_mii_reg_read(yt_phy_comm_cfg_t cfg, uint16_t regAddr, uint16_t *pRegValue);
yt_ret_t phy_mii_reg_write(yt_phy_comm_cfg_t cfg, uint16_t regAddr, uint16_t regValue);
yt_ret_t phy_ext_reg_read(yt_phy_comm_cfg_t cfg, uint16_t regAddr, uint16_t *pRegValue);
yt_ret_t phy_ext_reg_write(yt_phy_comm_cfg_t cfg, uint16_t regAddr, uint16_t regValue);
yt_ret_t phy_mmd_reg_read(yt_phy_comm_cfg_t cfg, uint8_t mmdId, uint16_t regAddr, uint16_t *pRegValue);
yt_ret_t phy_mmd_reg_write(yt_phy_comm_cfg_t cfg, uint8_t mmdId, uint16_t regAddr, uint16_t regValue);
uint8_t phy_baseAddr_get(yt_phy_comm_cfg_t cfg);
yt_phy_chip_model_t phy_model_get_by_port(yt_unit_t unit, yt_port_t port);
yt_ret_t phy_swIntPhy_drv_update(yt_unit_t unit, yt_port_t port, yt_extif_mode_t mode);
uint8_t phy_addr_smi_type_get(yt_unit_t unit, yt_port_t port, uint8_t *pPhyAddr, yt_phy_smi_type_t *pSmiType);
yt_bool_t phy_is_serdes(yt_unit_t unit, yt_port_t port);
void phy_smi_init(void);
yt_ret_t phy_drv_load(void);
yt_ret_t phy_comm_param_check(yt_phy_comm_cfg_t cfg);
void phy_external_smi_phyAddrMask_set(uint32_t phyAddrMask);
yt_ret_t phy_external_smi_cl22_register(smi_cl22_read smiRead, smi_cl22_write smiWrite);

#endif
