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
 * @internal      fal_tiger_port_enable_set
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
extern yt_ret_t fal_tiger_port_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_port_enable_get
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
extern yt_ret_t fal_tiger_port_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


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
extern yt_ret_t fal_tiger_port_mac_force_set(yt_unit_t unit, yt_port_t port, yt_port_force_ctrl_t port_ctrl);


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
extern yt_ret_t fal_tiger_port_mac_force_get(yt_unit_t unit, yt_port_t port, yt_port_force_ctrl_t *pPort_ctrl);


/**
 * @internal      fal_tiger_port_mac_fc_set
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
extern yt_ret_t fal_tiger_port_mac_fc_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_port_mac_fc_get
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
extern yt_ret_t fal_tiger_port_mac_fc_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


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
extern yt_ret_t fal_tiger_port_phyAutoNeg_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


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
extern yt_ret_t fal_tiger_port_phyAutoNeg_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


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
extern yt_ret_t fal_tiger_port_phyAutoNeg_ability_set(yt_unit_t unit, yt_port_t port, yt_port_an_ability_t ability);


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
extern yt_ret_t fal_tiger_port_phyAutoNeg_ability_get(yt_unit_t unit, yt_port_t port, yt_port_an_ability_t *pAbility);


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
extern yt_ret_t fal_tiger_port_phy_force_set(yt_unit_t unit, yt_port_t port, yt_port_speed_duplex_t speed_dup);


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
extern yt_ret_t fal_tiger_port_phy_force_get(yt_unit_t unit, yt_port_t port, yt_port_speed_duplex_t *pSpeedDup);


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
extern yt_ret_t fal_tiger_port_phy_linkstatus_get(yt_unit_t unit, yt_port_t port, yt_port_linkStatus_all_t *pLinkStatus);


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
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phy_reg_set(yt_unit_t unit, yt_port_t port, uint32_t regAddr, uint16_t data, yt_phy_type_t type);


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
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phy_reg_get(yt_unit_t unit, yt_port_t port, uint32_t regAddr, uint16_t *pData, yt_phy_type_t type);


/**
 * @internal      fal_tiger_port_eee_enable_set
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
extern yt_ret_t fal_tiger_port_eee_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_port_eee_enable_get
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
extern yt_ret_t fal_tiger_port_eee_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


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
extern yt_ret_t fal_tiger_port_polling_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


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
 * @internal      fal_tiger_port_cable_diag
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
extern yt_ret_t fal_tiger_port_cable_diag(yt_unit_t unit, yt_port_t port, yt_port_cableDiag_t *pCableStatus);


/**
 * @internal      fal_tiger_port_phyTemplate_test_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    mode        -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_port_phyTemplate_test_set(yt_unit_t unit, yt_port_t port, yt_utp_template_testmode_t mode);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
