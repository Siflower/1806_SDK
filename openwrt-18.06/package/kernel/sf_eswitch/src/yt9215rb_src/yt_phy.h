/*******************************************************************************
*                                                                              *
*  Copyright (c), 2025, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_phy.h
*
* @brief x
*
********************************************************************************
*/

#ifndef YT_PHY_H
#define YT_PHY_H
#include "yt_types.h"
#include "yt_board.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define YT_PHY_MAX_ADDR     (32)
#define YT_PHYADDR_ALLMASK  (0xFFFFFFFF)

typedef enum yt_phy_smi_type_e {
    YT_PHY_SMITYPE_SW_INT,
    YT_PHY_SMITYPE_SW_EXT,
    YT_PHY_SMITYPE_EXTERNAL,
}yt_phy_smi_type_t;

typedef struct yt_phy_comm_cfg_s {
    yt_phy_smi_type_t smiType;
    uint8_t phyAddr;
    yt_unit_t unit;
}yt_phy_comm_cfg_t; 

/**
 * @internal      yt_phy_external_smi_phyAddrMask_set
 * @endinternal
 *
 * @brief         Set phy address mask for external SMI, use this API to specify phy address mask that is currentyly in use.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     phyAddrMask         -phy address mask
 * @retval        none                -no return value
 */
extern void yt_phy_external_smi_phyAddrMask_set(uint32_t phyAddrMask);

/**
 * @internal      yt_phy_external_smi_cl22_register
 * @endinternal
 *
 * @brief         Set clause 22 read/write function for external smi bus.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     smiRead             -read function
 * @param[in]     smiWrite            -write function
 * @retval        CMM_ERR_OK          -on success
 */
extern yt_ret_t yt_phy_external_smi_cl22_register(smi_cl22_read smiRead, smi_cl22_write smiWrite);

/**
 * @internal      yt_phy_init
 * @endinternal
 *
 * @brief         Init phy
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_init(yt_phy_comm_cfg_t cfg);

/**
 * @internal      yt_phy_restart
 * @endinternal
 *
 * @brief         Restart phy
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_restart(yt_phy_comm_cfg_t cfg);

/**
 * @internal      yt_phy_enable_set
 * @endinternal
 *
 * @brief         Enable or disable phy
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     phyMode             -phy chip mode.
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_enable_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t enable);

/**
 * @internal      yt_phy_enable_get
 * @endinternal
 *
 * @brief         Get the enable status
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     phyMode             -phy chip mode.
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_enable_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable);

/**
 * @internal      yt_phy_medium_set
 * @endinternal
 *
 * @brief         set the medium type of phy
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     medium              -medium type.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_medium_set(yt_phy_comm_cfg_t cfg, yt_port_medium_t medium);

/**
 * @internal      yt_phy_medium_get
 * @endinternal
 *
 * @brief         Get the medium type of phy
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[out]    pMedium             -medium type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_medium_get(yt_phy_comm_cfg_t cfg, yt_port_medium_t *pMedium);

/**
 * @internal      yt_phy_autoNeg_enable_set
 * @endinternal
 *
 * @brief         Enable or disable phy AN
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     phyMode             -phy chip mode.
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_autoNeg_enable_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t enable);

/**
 * @internal      yt_phy_autoNeg_enable_get
 * @endinternal
 *
 * @brief         Enable or disable phy AN
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     phyMode             -phy chip mode.
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_autoNeg_enable_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable);

/**
 * @internal      yt_phy_autoNeg_ability_set
 * @endinternal
 *
 * @brief         Configure phy AN ability
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     phyMode             -phy chip mode.
 * @param[in]     ability             -AN ability
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_autoNeg_ability_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t ability);

/**
 * @internal      yt_phy_autoNeg_ability_get
 * @endinternal
 *
 * @brief         Get phy AN ability
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     phyMode             -phy chip mode.
 * @param[out]    pAbility            -AN ability
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_autoNeg_ability_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t *pAbility);

/**
 * @internal      yt_phy_force_speed_duplex_set
 * @endinternal
 *
 * @brief         Set phy force speed and duplex
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     phyMode             -phy chip mode.
 * @param[in]     speedDup            -speed and duplex
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_force_speed_duplex_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t speedDup);

/**
 * @internal      yt_phy_force_speed_duplex_get
 * @endinternal
 *
 * @brief         Get phy force speed and duplex
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     phyMode             -phy chip mode.
 * @param[out]    pSpeedDup           -speed and duplex
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_force_speed_duplex_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t *pSpeedDup);

/**
 * @internal      yt_phy_link_status_get
 * @endinternal
 *
 * @brief         Get phy link status
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     phyMode             -phy chip mode.
 * @param[out]    pLinkStatus         -link status
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_link_status_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_port_linkStatus_all_t *pLinkStatus);

/**
 * @internal      yt_phy_eee_enable_set
 * @endinternal
 *
 * @brief         Enable or disable phy EEE
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_eee_enable_set(yt_phy_comm_cfg_t cfg, yt_enable_t enable);

/**
 * @internal      yt_phy_eee_enable_get
 * @endinternal
 *
 * @brief         Get enable status of phy EEE
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_eee_enable_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable);

/**
 * @internal      yt_phy_eee_status_get
 * @endinternal
 *
 * @brief         Get EEE status
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[out]    pEnable             -EEE status
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_eee_status_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable);

/**
 * @internal      yt_phy_combo_mode_set
 * @endinternal
 *
 * @brief         Configure combo mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     mode                -combo mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_combo_mode_set(yt_phy_comm_cfg_t cfg, yt_combo_mode_t mode);

/**
 * @internal      yt_phy_combo_mode_get
 * @endinternal
 *
 * @brief         Get combo mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[out]    pMode               -combo mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_combo_mode_get(yt_phy_comm_cfg_t cfg, yt_combo_mode_t *pMode);

/**
 * @internal      yt_phy_cable_diag_start
 * @endinternal
 *
 * @brief         Start cable diagnostic
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_cable_diag_start(yt_phy_comm_cfg_t cfg);

/**
 * @internal      yt_phy_cable_diag_result_get
 * @endinternal
 *
 * @brief          Get status of pairs.it will cause linkdown when do cable diag.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[out]    pCableDiagStatus    -cable status
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_cable_diag_result_get(yt_phy_comm_cfg_t cfg, yt_port_cableDiag_t *pCableDiagStatus);

/**
 * @internal      yt_phy_interrupt_status_get
 * @endinternal
 *
 * @brief         Get interrupt status.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[out]    pStatusData         -interrupt status
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_interrupt_status_get(yt_phy_comm_cfg_t cfg, uint16_t *pStatusData);

/**
 * @internal      yt_phy_test_template
 * @endinternal
 *
 * @brief         Set utp template test mode.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     mode                -test mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_test_template(yt_phy_comm_cfg_t cfg, yt_utp_template_testmode_t mode);

/**
 * @internal      yt_phy_crossover_mode_set
 * @endinternal
 *
 * @brief         Set utp crossover mode.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     mode                -utp crossover mode--mdi,mdix,auto
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_crossover_mode_set(yt_phy_comm_cfg_t cfg, yt_utp_crossover_mode_t mode);

/**
 * @internal      yt_phy_crossover_mode_get
 * @endinternal
 *
 * @brief         Get utp crossover mode.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[out]    pMode               -utp crossover mode--mdi,mdix,auto
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_crossover_mode_get(yt_phy_comm_cfg_t cfg, yt_utp_crossover_mode_t *pMode);

/**
 * @internal      yt_phy_crossover_status_get
 * @endinternal
 *
 * @brief         Get utp crossover status
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[out]    pStatus             -utp crossover status--mdi,mdix
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_crossover_status_get(yt_phy_comm_cfg_t cfg, yt_utp_crossover_status_t *pStatus);

/**
 * @internal      yt_phy_loopback_set
 * @endinternal
 *
 * @brief         set phy loopback mode.Note:use yt_port_phy_force_set() to set port speed if needed.
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     phyMode             -copper, phy, combo
 * @param[in]     mode                -internal loopback or external loopback or remote loopback.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_loopback_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t phyMode, yt_phy_loopback_mode_t mode);

/**
 * @internal      yt_phy_green_start
 * @endinternal
 *
 * @brief         Start phy green function
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_green_start(yt_phy_comm_cfg_t cfg);

/**
 * @internal      yt_phy_green_result_get
 * @endinternal
 *
 * @brief         Get phy green status
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_green_result_get(yt_phy_comm_cfg_t cfg, yt_port_cableDiag_t *pStatus);

/**
 * @internal      yt_phy_smart_downgrade_set
 * @endinternal
 *
 * @brief         Enable/Disable phy smart speed downgrade
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_smart_downgrade_set(yt_phy_comm_cfg_t cfg, yt_enable_t enable);

/**
 * @internal      yt_phy_smart_downgrade_get
 * @endinternal
 *
 * @brief         get phy smart speed downgrade state
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_smart_downgrade_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable);

/**
 * @internal      yt_phy_parallel_detection_set
 * @endinternal
 *
 * @brief         Set serdes parallel detection state
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_parallel_detection_set(yt_phy_comm_cfg_t cfg, yt_enable_t enable);

/**
 * @internal      yt_phy_parallel_detection_get
 * @endinternal
 *
 * @brief         Get serdes parallel detection state
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_parallel_detection_get(yt_phy_comm_cfg_t cfg, yt_enable_t *pEnable);

/**
 * @internal      yt_phy_utp_snr_get
 * @endinternal
 *
 * @brief         get phy utp snr value
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     speed               -speed
 * @param[out]    pSnrVal             -snr
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_utp_snr_get(yt_phy_comm_cfg_t cfg, yt_port_speed_t speed, yt_utp_snr_t *pSnrVal);

/**
 * @internal      yt_phy_fc_autoneg_cfg_set
 * @endinternal
 *
 * @brief         
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     extifMode           -extif mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_fc_autoneg_cfg_set(yt_phy_comm_cfg_t cfg, yt_extif_mode_t extifMode);

/**
 * @internal      yt_phy_chip_mode_set
 * @endinternal
 *
 * @brief         Set phy chip mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[in]     mode                -phy chip mode.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_chip_mode_set(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t mode);

/**
 * @internal      yt_phy_chip_mode_get
 * @endinternal
 *
 * @brief         Get phy chip mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     cfg                 -common parameters, include unit, phy address, smi type.
 * @param[out]    pMode               -phy chip mode.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_phy_chip_mode_get(yt_phy_comm_cfg_t cfg, yt_phy_chip_mode_t *pMode);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif