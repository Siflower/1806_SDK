/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "phy_drv.h"
#include "yt_phy.h"

extern const yt_phy_drv_t yt8614_drv;

yt_ret_t yt8614_init(yt_phy_comm_cfg_t cfg);
yt_ret_t yt8614_restart(yt_phy_comm_cfg_t cfg);
yt_ret_t yt8614_enable_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t enable);
yt_ret_t yt8614_enable_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable);
yt_ret_t yt8614_medium_set(yt_phy_comm_cfg_t cfg, yt_port_medium_t medium);
yt_ret_t yt8614_medium_get(yt_phy_comm_cfg_t cfg, yt_port_medium_t *pMedium);
yt_ret_t yt8614_autoNeg_enable_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t enable);
yt_ret_t yt8614_autoNeg_enable_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable);
yt_ret_t yt8614_autoNeg_ability_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t ability);
yt_ret_t yt8614_autoNeg_ability_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t *pAbility);
yt_ret_t yt8614_force_speed_duplex_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t speedDup);
yt_ret_t yt8614_force_speed_duplex_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t *pSpeedDup);
yt_ret_t yt8614_link_status_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_linkStatus_all_t *pLinkStatus);
yt_ret_t yt8614_eee_enable_set(yt_phy_comm_cfg_t cfg, yt_enable_t enable);
yt_ret_t yt8614_eee_enable_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable);
yt_ret_t yt8614_eee_status_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable);
yt_ret_t yt8614_combo_mode_set(yt_phy_comm_cfg_t cfg, yt_combo_mode_t mode);
yt_ret_t yt8614_combo_mode_get(yt_phy_comm_cfg_t cfg, yt_combo_mode_t *pMode);
yt_ret_t yt8614_cable_diag_start(yt_phy_comm_cfg_t cfg);
yt_ret_t yt8614_cable_diag_result_get(yt_phy_comm_cfg_t cfg, yt_port_cableDiag_t *pCableDiagStatus);
yt_ret_t yt8614_interrupt_status_get(yt_phy_comm_cfg_t cfg, uint16_t *pStatusData);
yt_ret_t yt8614_test_template(yt_phy_comm_cfg_t cfg, yt_utp_template_testmode_t mode);
yt_ret_t yt8614_crossover_mode_set(yt_phy_comm_cfg_t cfg, yt_utp_crossover_mode_t mode);
yt_ret_t yt8614_crossover_mode_get(yt_phy_comm_cfg_t cfg, yt_utp_crossover_mode_t *pMode);
yt_ret_t yt8614_crossover_status_get(yt_phy_comm_cfg_t cfg, yt_utp_crossover_status_t *pStatus);
yt_ret_t yt8614_loopback_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_phy_loopback_mode_t mode);
yt_ret_t yt8614_green_start(yt_phy_comm_cfg_t cfg);
yt_ret_t yt8614_green_result_get(yt_phy_comm_cfg_t cfg, yt_port_cableDiag_t *pStatus);
yt_ret_t yt8614_smart_downgrade_set(yt_phy_comm_cfg_t cfg, yt_enable_t enable);
yt_ret_t yt8614_smart_downgrade_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable);
yt_ret_t yt8614_parallel_detection_set(yt_phy_comm_cfg_t cfg, yt_enable_t enable);
yt_ret_t yt8614_parallel_detection_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable);
yt_ret_t yt8614_utp_snr_get(yt_phy_comm_cfg_t cfg, yt_port_speed_t speed, yt_utp_snr_t *pSnrVal);
yt_ret_t yt8614_chip_mode_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t mode);
yt_ret_t yt8614_chip_mode_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t *pMode);
yt_ret_t yt8614_fc_autoneg_cfg_set(yt_phy_comm_cfg_t cfg, yt_extif_mode_t extifMode);