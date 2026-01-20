/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_port.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_PORT_H__
#define __FAL_TIGER_PORT_H__


#include "yt_types.h"
#include "yt_port.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum yt_port_speed_mode_e
{
    PORT_SPEED_MODE_10M = 0,
    PORT_SPEED_MODE_100M,
    PORT_SPEED_MODE_1000M,
    PORT_SPEED_MODE_10G,
    PORT_SPEED_MODE_2500M,
    PORT_SPEED_MODE_5G,
    PORT_SPEED_MODE_END
}yt_port_speed_mode_t;

/**
 * @internal      fal_tiger_port_init
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_init(yt_unit_t unit);

/**
 * @internal      fal_tiger_port_default_cfg_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_default_cfg_set(yt_unit_t unit);

/**
 * @internal      fal_tiger_port_mac_enable_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_mac_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_port_mac_enable_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_mac_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_port_link_status_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pLinkStatus         -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_link_status_get(yt_unit_t unit, yt_port_t port, yt_port_link_status_t *pLinkStatus);


/**
 * @internal      fal_tiger_port_link_status_all_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pAllLinkStatus      -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_link_status_all_get(yt_unit_t unit, yt_port_t port, yt_port_linkStatus_all_t *pAllLinkStatus);


/**
 * @internal      fal_tiger_port_backpress_enable_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_backpress_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_port_backpress_enable_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_backpress_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_port_cascade_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     cascade_info        -cascade enable status and ports configure information
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_cascade_set(yt_unit_t unit, yt_cascade_info_t cascade_info);


/**
 * @internal      fal_tiger_port_cascade_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pCascade_info       -cascade enable status and ports configure information
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_cascade_get(yt_unit_t unit, yt_cascade_info_t *pCascade_info);


/**
 * @internal      fal_tiger_port_pkt_gap_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     gap                 -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_pkt_gap_set(yt_unit_t unit, yt_port_t port, uint8_t gap);


/**
 * @internal      fal_tiger_port_pkt_gap_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pGap                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_pkt_gap_get(yt_unit_t unit, yt_port_t port, uint8_t *pGap);


/**
 * @internal      fal_tiger_port_macAutoNeg_enable_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_macAutoNeg_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_port_macAutoNeg_enable_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_macAutoNeg_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_port_mac_force_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     port_ctrl           -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_mac_force_set(yt_unit_t unit, yt_port_t port, yt_port_speed_duplex_t port_ctrl);


/**
 * @internal      fal_tiger_port_mac_force_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pPort_ctrl          -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_mac_force_get(yt_unit_t unit, yt_port_t port, yt_port_speed_duplex_t *pPort_ctrl);


/**
 * @internal      fal_tiger_port_mac_fc_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     fcAutoNegEnable              -enable or disable
 * @param[in]     rxFcEnable              -enable or disable
 * @param[in]     txFcEnable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_mac_fc_set(yt_unit_t unit, yt_port_t port, yt_enable_t fcAutoNegEnable, yt_enable_t rxFcEnable, yt_enable_t txFcEnable);


/**
 * @internal      fal_tiger_port_mac_fc_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pFcAutoNegEnable             -enable or disable
 * @param[out]    pRxFcEnable             -enable or disable
 * @param[out]    pTxFcEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_mac_fc_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pFcAutoNegEnable, yt_enable_t *pRxFcEnable, yt_enable_t *pTxFcEnable);


/**
 * @internal      fal_tiger_port_mac_block_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     rxCfgEnable              -enable or disable
 * @param[in]     txCfgEnable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_mac_block_set(yt_unit_t unit, yt_port_t port, yt_enable_t rxCfgEnable, yt_enable_t txCfgEnable);


/**
 * @internal      fal_tiger_port_mac_block_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pRxCfgEnable             -enable or disable
 * @param[out]    pTxCfgEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_mac_block_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pRxCfgEnable, yt_enable_t *pTxCfgEnable);


/**
 * @internal      fal_tiger_port_mac_eee_enable_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_mac_eee_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_port_mac_eee_enable_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_mac_eee_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_port_mac_eee_timer_set
 * @endinternal
 *
 * @brief         set wakeup and sleep timer
 * @note          APPLICABLE DEVICES  -Tiger,Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     uint32_t         -wakeupTimer
 * @param[in]     uint32_t         -sleepTimer
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_mac_eee_timer_set(yt_unit_t unit, yt_port_t port, uint32_t wakeupTimer, uint32_t sleepTimer);


/**
 * @internal      fal_tiger_port_mac_eee_timer_get
 * @endinternal
 *
 * @brief         get wakeup and sleep timer
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pWakeupTimer         -lpi wakeup timer
 * @param[out]    pSleepTimer             -lpi sleep timer
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_mac_eee_timer_get(yt_unit_t unit, yt_port_t port, uint32_t *pWakeupTimer, uint32_t *pSleepTimer);


/**
 * @internal      fal_tiger_port_extif_mode_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     mode                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_extif_mode_set(yt_unit_t unit, yt_port_t port, yt_extif_mode_t mode);


/**
 * @internal      fal_tiger_port_extif_mode_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pMode               -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_extif_mode_get(yt_unit_t unit, yt_port_t port, yt_extif_mode_t *pMode);


/**
 * @internal      fal_tiger_port_extif_rgmii_delay_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     rxc_delay           -x
 * @param[in]     txc_delay           -x
 * @param[in]     txc_2ns_en          -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_extif_rgmii_delay_set(yt_unit_t unit, yt_port_t port, uint8_t rxc_delay, uint8_t txc_delay, yt_enable_t txc_2ns_en);


/**
 * @internal      fal_tiger_port_extif_rgmii_delay_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pRxc_delay          -x
 * @param[out]    pTxc_delay          -x
 * @param[out]    pTxc_2ns_en         -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_extif_rgmii_delay_get(yt_unit_t unit, yt_port_t port, uint8_t *pRxc_delay, uint8_t *pTxc_delay, yt_enable_t *pTxc_2ns_en);


/**
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phy_enable_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_enable_t enable);


/**
 * @internal      fal_tiger_port_mac_enable_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phy_enable_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_port_phyAutoNeg_enable_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phyAutoNeg_enable_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_enable_t enable);


/**
 * @internal      fal_tiger_port_phyAutoNeg_enable_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phyAutoNeg_enable_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_port_phyAutoNeg_ability_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ability             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phyAutoNeg_ability_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t ability);


/**
 * @internal      fal_tiger_port_phyAutoNeg_ability_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pAbility            -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phyAutoNeg_ability_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t *pAbility);


/**
 * @internal      fal_tiger_port_phy_force_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     speed_dup           -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phy_force_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t speed_dup);


/**
 * @internal      fal_tiger_port_phy_force_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pSpeedDup           -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phy_force_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t *pSpeedDup);


/**
 * @internal      fal_tiger_port_phy_linkstatus_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pLinkStatus         -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phy_linkstatus_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_linkStatus_all_t *pLinkStatus);


/**
 * @internal      fal_tiger_port_phy_interruptStatus_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pIntStatus         -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phy_interruptStatus_get(yt_unit_t unit, yt_port_t port, uint16_t *pIntStatus);


/**
 * @internal      fal_tiger_port_phy_reg_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     regAddr             -x
 * @param[in]     data                -x
 * @param[in]     type                -phy type, internal or external phy
 * @param[in]     regType                -mii, ext, mmd, top ext, sds
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phy_reg_set(yt_unit_t unit, yt_port_t port, uint32_t regAddr, uint16_t data, yt_phy_type_t type, yt_phy_reg_type_t regType);


/**
 * @internal      fal_tiger_port_phy_reg_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     regAddr             -x
 * @param[out]    pData               -x
 * @param[in]     type                -phy type, internal or external phy
 * @param[in]     regType                -mii, ext, mmd, top ext, sds
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phy_reg_get(yt_unit_t unit, yt_port_t port, uint32_t regAddr, uint16_t *pData, yt_phy_type_t type, yt_phy_reg_type_t regType);


/**
 * @internal      fal_tiger_port_phy_eee_enable_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phy_eee_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_port_phy_eee_enable_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phy_eee_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      fal_tiger_port_phy_eee_status_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phy_eee_status_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_port_phyCombo_mode_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     mode                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phyCombo_mode_set(yt_unit_t unit, yt_port_t port, yt_combo_mode_t mode);


/**
 * @internal      fal_tiger_port_phyCombo_mode_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pMode               -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phyCombo_mode_get(yt_unit_t unit, yt_port_t port, yt_combo_mode_t *pMode);

/**
 * @internal      fal_tiger_port_polling_enable_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_polling_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_port_polling_enable_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_polling_enable_get(yt_unit_t unit, yt_port_t port,  yt_enable_t *pEnable);


extern uint32_t fal_tiger_port_mac_sync_phy(yt_unit_t unit, yt_port_t port);


/**
 * @internal      fal_tiger_port_jumbo_enable_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_jumbo_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_port_jumbo_enable_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_jumbo_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      fal_tiger_port_jumbo_size_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     size              -jumbo size
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_jumbo_size_set(yt_unit_t unit, yt_port_t port, uint32_t size);


/**
 * @internal      fal_tiger_port_jumbo_size_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pSize             -jumbo size
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_jumbo_size_get(yt_unit_t unit, yt_port_t port, uint32_t *pSize);

/**
 * @internal      fal_tiger_port_cable_diag_start
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_cable_diag_start(yt_unit_t unit, yt_port_t port);


/**
 * @internal      fal_tiger_port_cable_diag_result_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pCableStatus        -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_cable_diag_result_get(yt_unit_t unit, yt_port_t port, yt_port_cableDiag_t *pCableStatus);


/**
 * @internal      fal_tiger_port_phyCrossover_mode_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     mode                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phyCrossover_mode_set(yt_unit_t unit, yt_port_t port, yt_utp_crossover_mode_t mode);


/**
 * @internal      fal_tiger_port_phyCrossover_mode_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pMode               -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phyCrossover_mode_get(yt_unit_t unit, yt_port_t port, yt_utp_crossover_mode_t *pMode);


/**
 * @internal      fal_tiger_port_phyCrossover_status_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pStatus             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phyCrossover_status_get(yt_unit_t unit, yt_port_t port, yt_utp_crossover_status_t *pStatus);

/**
 * @internal      fal_tiger_port_phyGreen_start
 * @endinternal
 *
 * @brief         Enable/Disable phy green function
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -YT_ENABLE/YT_DISABLE
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phyGreen_start(yt_unit_t unit, yt_port_t port);

/**
 * @internal      fal_tiger_port_phyGreen_result_get
 * @endinternal
 *
 * @brief         get phy green status
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pStatus             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phyGreen_result_get(yt_unit_t unit, yt_port_t port, yt_port_cableDiag_t *pStatus);


/**
 * @internal      fal_tiger_port_physmart_downgrade_set
 * @endinternal
 *
 * @brief         Enable/Disable phy smart speed downgrade
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -YT_ENABLE/YT_DISABLE
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_physmart_downgrade_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);

/**
 * @internal      fal_tiger_port_physmart_downgrade_get
 * @endinternal
 *
 * @brief         get phy smart speed downgrade state
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pStatus             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_physmart_downgrade_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_port_extif_xmiiClk_invert_set
 * @endinternal
 *
 * @brief         set extif xmii clk revert 
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     state                -xmii clk revert enable/disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_extif_xmiiClk_invert_set(yt_unit_t unit, yt_port_t port,yt_enable_t state);


/**
 * @internal      fal_tiger_port_extif_xmiiClk_invert_get
 * @endinternal
 *
 * @brief         get extif xmii clk revert 
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pState               -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_extif_xmiiClk_invert_get(yt_unit_t unit, yt_port_t port,yt_enable_t *pState);


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
extern yt_ret_t fal_tiger_port_parallel_detection_set(yt_unit_t unit, yt_port_t port,yt_enable_t enable);


/**
 * @internal      fal_tiger_port_parallel_detection_get
 * @endinternal
 *
 * @brief         get serdes parallel detection state
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable            -parallel detection state enable/disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_parallel_detection_get(yt_unit_t unit, yt_port_t port,yt_enable_t *pEnable);


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
extern yt_ret_t fal_tiger_port_dvddio_power_level_set(yt_unit_t unit, yt_dvddio_power_pad_t powerPad, yt_dvddio_power_level_t powerLevel);


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
extern yt_ret_t fal_tiger_port_dvddio_power_level_get(yt_unit_t unit, yt_dvddio_power_pad_t powerPad, yt_dvddio_power_level_t *pPowerLevel);


/**
 * @internal      fal_tiger_port_phy_chip_mode_set
 * @endinternal
 *
 * @brief         set combo mode
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     mode                -combo mode -copper, fiber, combo
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phy_chip_mode_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t mode);


/**
 * @internal      fal_tiger_port_phy_chip_mode_get
 * @endinternal
 *
 * @brief         get combo mode
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pMode               -combo mode -copper, fiber, combo
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phy_chip_mode_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t *pMode);


/**
 * @internal      fal_tiger_port_phyCombo_mode_set
 * @endinternal
 *
 * @brief         set combo mode copper/fiber frist
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     mode                -combo mode -fiber_first, copper_first
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phyCombo_mode_set(yt_unit_t unit, yt_port_t port, yt_combo_mode_t mode);


/**
 * @internal      fal_tiger_port_phyCombo_mode_get
 * @endinternal
 *
 * @brief         get combo mode copper/fiber frist
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pMode               -combo mode -fiber_first, copper_first
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phyCombo_mode_get(yt_unit_t unit, yt_port_t port, yt_combo_mode_t *pMode);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
