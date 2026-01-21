/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_port.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_PORT_H__
#define __YT_PORT_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "yt_types.h"
#include "yt_cmm.h"
#include "cal_cmm.h"

#define YT_MAX_CASCADE_PORT_NUM     CAL_MAX_CASCADE_PORT_NUM
#define YT_INVALID_CASCADE_PORT     CAL_INVALID_CASCADE_PORT
#define YT_CASCADE_FWD_PORTMASK     CAL_CASCADE_FWD_PORTMASK
#define YT_JUMBO_SIZE_MAX                  CAL_JUMBO_SIZE_MAX
#define YT_JUMBO_SIZE_MIN                  CAL_JUMBO_SIZE_MIN
#define YT_PKTGAP_SIZE_MAX               CAL_PKTGAP_SIZE_MAX
#define YT_PKTGAP_SIZE_MIN               CAL_PKTGAP_SIZE_MIN
#define YT_EEE_WAKEUP_TIMER_MAX               CAL_EEE_WAKEUP_TIMER_MAX
#define YT_EEE_SLEEP_TIMER_MAX               CAL_EEE_SLEEP_TIMER_MAX

typedef struct yt_extif_force_ctrl_s
{
    yt_port_link_status_t   link;
    yt_port_speed_duplex_t speed_dup;
    yt_bool_t   pause_en;
}yt_extif_force_ctrl_t;

typedef struct yt_port_cascade_s
{
    yt_bool_t valid;
    yt_port_t port_num;
}yt_port_cascade_t;

typedef struct yt_cascade_info_s
{
    yt_enable_t en;
    yt_port_cascade_t ports[YT_MAX_CASCADE_PORT_NUM];
}yt_cascade_info_t;

typedef enum yt_dvddio_power_level_e
{
    POWER18V = 0,
    POWER25V,
    POWER33V,
    POWER33V_OR_25V
}yt_dvddio_power_level_t;

typedef enum yt_dvddio_power_pad_e
{
    POWER_PAD_NORMAL,
    POWER_PAD_RGMII1,
    POWER_PAD_RGMII2
}yt_dvddio_power_pad_t;

typedef struct yt_port_mdio_polling_cfg_s
{
    yt_bool_t port_vid;   /* enable this polling */
    yt_unit_t phy_type; 
    yt_unit_t phy_addr;  
} yt_port_mdio_polling_cfg_t;

/**
 * @internal      yt_port_init
 * @endinternal
 *
 * @brief         initial port module
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_init(yt_unit_t unit);

/**
 * @internal      yt_port_default_cfg_set
 * @endinternal
 *
 * @brief         Configure port according to board profile
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_default_cfg_set(yt_unit_t unit);

/**
 * @internal      yt_port_mac_enable_set
 * @endinternal
 *
 * @brief         disable or enable port,includes mac and phy(if phy exists)
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_mac_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      yt_port_mac_enable_get
 * @endinternal
 *
 * @brief         get port enable status
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_mac_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      yt_port_link_status_get
 * @endinternal
 *
 * @brief         get port link status
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pLinkStatus         -LINK_DOWN or LINK_UP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_link_status_get(yt_unit_t unit, yt_port_t port, yt_port_link_status_t *pLinkStatus);


/**
 * @internal      yt_port_link_status_all_get
 * @endinternal
 *
 * @brief         get full status of port,includes link,speed,duplex etc.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pAllLinkStatus      -all link status
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_link_status_all_get(yt_unit_t unit, yt_port_t port, yt_port_linkStatus_all_t *pAllLinkStatus);


/**
 * @internal      yt_port_backpress_enable_set
 * @endinternal
 *
 * @brief         set half duplex backpressure enable status
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_backpress_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      yt_port_backpress_enable_get
 * @endinternal
 *
 * @brief         get half duplex backpressure enable status
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_backpress_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      yt_port_cascade_set
 * @endinternal
 *
 * @brief         config cascade ports and enable status
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     cascade_info        -cascade enable status and ports configure information
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_cascade_set(yt_unit_t unit, yt_cascade_info_t cascade_info);


/**
 * @internal      yt_port_cascade_get
 * @endinternal
 *
 * @brief         get cascade port info
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pCascade_info       -cascade enable status and ports configure information
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_cascade_get(yt_unit_t unit, yt_cascade_info_t *pCascade_info);


/**
 * @internal      yt_port_pkt_gap_set
 * @endinternal
 *
 * @brief         set port tx packet gap value
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     gap                 -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_pkt_gap_set(yt_unit_t unit, yt_port_t port, uint8_t gap);


/**
 * @internal      yt_port_pkt_gap_get
 * @endinternal
 *
 * @brief         get port tx packet gap value
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pGap                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_pkt_gap_get(yt_unit_t unit, yt_port_t port, uint8_t *pGap);


/**
 * @internal      yt_port_macAutoNeg_enable_set
 * @endinternal
 *
 * @brief         set mac AN enable status
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_macAutoNeg_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      yt_port_macAutoNeg_enable_get
 * @endinternal
 *
 * @brief         get mac AN enable status
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_macAutoNeg_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      yt_port_mac_force_set
 * @endinternal
 *
 * @brief         set mac force control configuration.should disable mac AN first. 
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     port_ctrl           -force control configuration
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_mac_force_set(yt_unit_t unit, yt_port_t port, yt_port_speed_duplex_t port_ctrl);


/**
 * @internal      yt_port_mac_force_get
 * @endinternal
 *
 * @brief         get mac force control configuration
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pPort_ctrl          -force control configuration
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_mac_force_get(yt_unit_t unit, yt_port_t port, yt_port_speed_duplex_t *pPort_ctrl);


/**
 * @internal      yt_port_mac_fc_set
 * @endinternal
 *
 * @brief         force to enable or disable flowcontrol on mac
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     fcAutoNegEnable              -enable or disable
 * @param[in]     rxFcEnable              -enable or disable
 * @param[in]     txFcEnable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_mac_fc_set(yt_unit_t unit, yt_port_t port, yt_enable_t fcAutoNegEnable, yt_enable_t rxFcEnable, yt_enable_t txFcEnable);


/**
 * @internal      yt_port_mac_fc_get
 * @endinternal
 *
 * @brief         get force flowcontrol status on mac
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pFcAutoNegEnable             -enable or disable
 * @param[out]    pRxFcEnable             -enable or disable
 * @param[out]    pTxFcEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_mac_fc_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pFcAutoNegEnable, yt_enable_t *pRxFcEnable, yt_enable_t *pTxFcEnable);


/**
 * @internal      yt_port_mac_block_set
 * @endinternal
 *
 * @brief         set cfg rx/tx mac status on mac 
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     rxMacEnable              -enable or disable
 * @param[in]     txMacEnable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_mac_block_set(yt_unit_t unit, yt_port_t port, yt_enable_t rxMacEnable, yt_enable_t txMacEnable);


/**
 * @internal      yt_port_mac_block_get
 * @endinternal
 *
 * @brief         get cfg rx/tx mac status on mac 
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pRxMacEnable             -enable or disable
 * @param[out]    pTxMacEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_mac_block_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pRxMacEnable, yt_enable_t *pTxMacEnable);


/**
 * @internal      yt_port_mac_eee_enable_set
 * @endinternal
 *
 * @brief         enable EEE on port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_mac_eee_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      yt_port_mac_eee_enable_get
 * @endinternal
 *
 * @brief         get enable status of EEE
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_mac_eee_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      yt_port_mac_eee_timer_set
 * @endinternal
 *
 * @brief         set wakeup and sleep timer
 * @note          APPLICABLE DEVICES  -Tiger,Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     uint32_t         -wakeupTimer
 * @param[in]     uint32_t         -sleepTimer
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_mac_eee_timer_set(yt_unit_t unit, yt_port_t port, uint32_t wakeupTimer, uint32_t sleepTimer);


/**
 * @internal      yt_port_mac_eee_timer_get
 * @endinternal
 *
 * @brief         get wakeup and sleep timer
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pWakeupTimer         -lpi wakeup timer
 * @param[out]    pSleepTimer             -lpi sleep timer
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_mac_eee_timer_get(yt_unit_t unit, yt_port_t port, uint32_t *pWakeupTimer, uint32_t *pSleepTimer);


/**
 * @internal      yt_port_extif_mode_set
 * @endinternal
 *
 * @brief         set ext interface mode,XMII or SGMII etc.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     mode                -MII,RGMII,SGMII etc.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_extif_mode_set(yt_unit_t unit, yt_port_t port, yt_extif_mode_t mode);


/**
 * @internal      yt_port_extif_mode_get
 * @endinternal
 *
 * @brief         get ext interface mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pMode               -MII,RGMII,SGMII etc.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_extif_mode_get(yt_unit_t unit, yt_port_t port, yt_extif_mode_t *pMode);


/**
 * @internal      yt_port_extif_rgmii_delay_set
 * @endinternal
 *
 * @brief         set tx and rx delay value for rgmii.txc delay = txc_2ns_en * 2ns + txc_delay * 0.15ns;rxc delay = rxc_delay * 0.15ns
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     rxc_delay           -tx delay,range(0~15)
 * @param[in]     txc_delay           -rx delay,range(0~15)
 * @param[in]     txc_2ns_en          -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_extif_rgmii_delay_set(yt_unit_t unit, yt_port_t port, uint8_t rxc_delay, uint8_t txc_delay, yt_enable_t txc_2ns_en);


/**
 * @internal      yt_port_extif_rgmii_delay_get
 * @endinternal
 *
 * @brief         get tx and rx delay value of  rgmii
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pRxc_delay          -x
 * @param[out]    pTxc_delay          -x
 * @param[out]    pTxc_2ns_en         -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_extif_rgmii_delay_get(yt_unit_t unit, yt_port_t port, uint8_t *pRxc_delay, uint8_t *pTxc_delay, yt_enable_t *pTxc_2ns_en);


/**
 * @internal      yt_port_phy_enable_set
 * @endinternal
 *
 * @brief         disable or enable phy
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     phyMode              -copper, phy, combo
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phy_enable_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_enable_t enable);


/**
 * @internal      yt_port_phy_enable_get
 * @endinternal
 *
 * @brief         get port enable status phy
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     phyMode              -copper, phy
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phy_enable_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable);


/**
 * @internal      yt_port_phyAutoNeg_enable_set
 * @endinternal
 *
 * @brief         set phy AN enable status
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     phyMode              -copper, phy, combo
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phyAutoNeg_enable_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_enable_t enable);


/**
 * @internal      yt_port_phyAutoNeg_enable_get
 * @endinternal
 *
 * @brief         get phy AN enable status
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     phyMode              -copper, phy
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phyAutoNeg_enable_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable);


/**
 * @internal      yt_port_phyAutoNeg_ability_set
 * @endinternal
 *
 * @brief         set phy AN ability configuration
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     phyMode              -copper, phy, combo
 * @param[in]     ability             -AN ability configuration
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phyAutoNeg_ability_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t ability);


/**
 * @internal      yt_port_phyAutoNeg_ability_get
 * @endinternal
 *
 * @brief         get phy AN ability configuration
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     phyMode              -copper, phy
 * @param[out]    pAbility            -AN ability configuration
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phyAutoNeg_ability_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t *pAbility);


/**
 * @internal      yt_port_phy_force_set
 * @endinternal
 *
 * @brief         set phy force speed and duplex
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     phyMode              -copper, phy, combo
 * @param[in]     speed_dup           -speed and duplex
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phy_force_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t speed_dup);


/**
 * @internal      yt_port_phy_force_get
 * @endinternal
 *
 * @brief         get phy force speed and duplex configuration
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     phyMode              -copper, phy
 * @param[out]    pSpeedDup           -speed and duplex
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phy_force_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t *pSpeedDup);


/**
 * @internal      yt_port_phy_linkstatus_get
 * @endinternal
 *
 * @brief         get phy link status,includes link status,link speed and link duplex
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     phyMode              -copper, phy
 * @param[out]    pLinkStatus         -LINK_DOWN or LINK_UP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phy_linkstatus_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_linkStatus_all_t *pLinkStatus);


/**
 * @internal      yt_port_phy_reg_set
 * @endinternal
 *
 * @brief         set phy register data
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     regAddr             -reg address
 * @param[in]     data                -reg data
 * @param[in]     type                -phy type, internal or external phy
 * @param[in]     regType                -mii, ext, mmd, top ext, sds
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phy_reg_set(yt_unit_t unit, yt_port_t port, uint32_t regAddr, uint16_t data, yt_phy_type_t type, yt_phy_reg_type_t regType);


/**
 * @internal      yt_port_phy_reg_get
 * @endinternal
 *
 * @brief         get external phy register data
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     regAddr             -reg address
 * @param[out]    pData               -reg data
 * @param[in]     type                -phy type, internal or external phy
 * @param[in]     regType                -mii, ext, mmd, top ext, sds
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phy_reg_get(yt_unit_t unit, yt_port_t port, uint32_t regAddr, uint16_t *pData, yt_phy_type_t type, yt_phy_reg_type_t regType);

/**
 * @internal      yt_port_phy_eee_enable_set
 * @endinternal
 *
 * @brief         enable EEE on port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phy_eee_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      yt_port_phy_eee_enable_get
 * @endinternal
 *
 * @brief         get enable status of EEE
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phy_eee_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      yt_port_phy_eee_status_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phy_eee_status_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

#if defined(SWITCH_SERIES_TIGER)
/**
 * @internal      yt_port_jumbo_size_set
 * @endinternal
 *
 * @brief          set jumbo frame size on port,9kB max length by default
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     size              -jumbo frame size
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_jumbo_size_set(yt_unit_t unit, yt_port_t port, uint32_t size);


/**
 * @internal      yt_port_jumbo_size_get
 * @endinternal
 *
 * @brief         get jumbo frame size on port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pSize             -jumbo frame size
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_jumbo_size_get(yt_unit_t unit, yt_port_t port, uint32_t *pSize);


/**
 * @internal      yt_port_jumbo_enable_set
 * @endinternal
 *
 * @brief         enable jumbo frame on port,9kB max length by default
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_jumbo_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      yt_port_jumbo_enable_get
 * @endinternal
 *
 * @brief         get enable status of jumbo frame on port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_jumbo_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


#else


/**
 * @internal      yt_port_jumbo_size_set
 * @endinternal
 *
 * @brief          set jumbo frame size on port,9kB max length by default
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     size              -jumbo frame size
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_jumbo_size_set(yt_unit_t unit, uint32_t size);


/**
 * @internal      yt_port_jumbo_size_get
 * @endinternal
 *
 * @brief         get jumbo frame size on port
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pSize             -jumbo frame size
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_jumbo_size_get(yt_unit_t unit, uint32_t *pSize);


/**
 * @internal      yt_port_jumbo_enable_set
 * @endinternal
 *
 * @brief         enable jumbo frame on port,9kB max length by default
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_jumbo_enable_set(yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      yt_port_jumbo_enable_get
 * @endinternal
 *
 * @brief         get enable status of jumbo frame on port
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_jumbo_enable_get(yt_unit_t unit, yt_enable_t *pEnable);


#endif

/**
 * @internal      yt_port_cable_diag_start
 * @endinternal
 *
 * @brief         start port cable diagnostic
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_cable_diag_start(yt_unit_t unit, yt_port_t port);


/**
 * @internal      yt_port_cable_diag_result_get
 * @endinternal
 *
 * @brief         get cable diagnose result of pairs.it will cause linkdown when do cable diag.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pCableStatus        -cable status
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_cable_diag_result_get(yt_unit_t unit, yt_port_t port, yt_port_cableDiag_t *pCableStatus);


/**
 * @internal      yt_port_phy_chip_mode_set
 * @endinternal
 *
 * @brief         set combo mode
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     phyMode              -copper, phy, combo
 * @param[in]     mode                -combo mode -copper, fiber, combo
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phy_chip_mode_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t mode);


/**
 * @internal      yt_port_phy_chip_mode_get
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
extern yt_ret_t yt_port_phy_chip_mode_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t *pMode);


/**
 * @internal      yt_port_phyCombo_mode_set
 * @endinternal
 *
 * @brief         set combo mode copper/fiber frist
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     mode                -combo mode -fiber_first, copper_first
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phyCombo_mode_set(yt_unit_t unit, yt_port_t port, yt_combo_mode_t mode);


/**
 * @internal      yt_port_phyCombo_mode_get
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
extern yt_ret_t yt_port_phyCombo_mode_get(yt_unit_t unit, yt_port_t port, yt_combo_mode_t *pMode);


/**
 * @internal      yt_port_phyCrossover_mode_set
 * @endinternal
 *
 * @brief         set utp crossover mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     mode                -utp crossover mode--mdi,mdix,auto
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phyCrossover_mode_set(yt_unit_t unit, yt_port_t port, yt_utp_crossover_mode_t mode);


/**
 * @internal      yt_port_phyCrossover_mode_get
 * @endinternal
 *
 * @brief         get utp crossover mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pMode               -utp crossover mode--mdi,mdix,auto
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phyCrossover_mode_get(yt_unit_t unit, yt_port_t port, yt_utp_crossover_mode_t *pMode);


/**
 * @internal      yt_port_phyCrossover_status_get
 * @endinternal
 *
 * @brief         get utp crossover status
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pStatus             -utp crossover status--mdi,mdix
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phyCrossover_status_get(yt_unit_t unit, yt_port_t port, yt_utp_crossover_status_t *pStatus);

/**
 * @internal      yt_port_phy_green_enable_set
 * @endinternal
 *
 * @brief         Enable/Disable phy green function(CLD)
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phy_green_start(yt_unit_t unit, yt_port_t port);

/**
 * @internal      yt_port_phy_green_enable_get
 * @endinternal
 *
 * @brief         get phy green status(CLD)
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pCableStatus             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phy_green_result_get(yt_unit_t unit, yt_port_t port, yt_port_cableDiag_t *pCableStatus);

/**
 * @internal      yt_port_phy_smart_downgrade_set
 * @endinternal
 *
 * @brief         Enable/Disable phy smart speed downgrade
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -YT_ENABLE/YT_DISABLE
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phy_smart_downgrade_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_port_phy_smart_downgrade_get
 * @endinternal
 *
 * @brief         get phy smart speed downgrade state
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pStatus             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_phy_smart_downgrade_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pStatus);

/**
 * @internal      yt_port_extif_xmiiClk_invert_set
 * @endinternal
 *
 * @brief         set extif xmii clk revert state
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    state             -invert state enable/disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_extif_xmiiClk_invert_set(yt_unit_t unit, yt_port_t port, yt_enable_t state);


/**
 * @internal      yt_port_extif_xmiiClk_invert_get
 * @endinternal
 *
 * @brief         get extif xmii clk revert state
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pState             -invert state enable/disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_extif_xmiiClk_invert_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pState);


/**
 * @internal      yt_port_parallel_detection_set
 * @endinternal
 *
 * @brief         set serdes parallel detection state
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    enable             -parallel detection state enable/disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_parallel_detection_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      yt_port_parallel_detection_get
 * @endinternal
 *
 * @brief         get serdes parallel detection state
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -parallel detection state enable/disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_parallel_detection_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      yt_port_dvddio_power_level_set
 * @endinternal
 *
 * @brief         set dvddio power level
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     powePad                -powerpad
 * @param[out]    powerLevel             -1.8v,2.5v,3.3v
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_dvddio_power_level_set(yt_unit_t unit, yt_dvddio_power_pad_t powePad, yt_dvddio_power_level_t powerLevel);


/**
 * @internal      yt_port_dvddio_power_level_get
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
extern yt_ret_t yt_port_dvddio_power_level_get(yt_unit_t unit, yt_dvddio_power_pad_t powePad, yt_dvddio_power_level_t *pPowerLevel);


/**
 * @internal      yt_port_polling_enable_set
 * @endinternal
 *
 * @brief         set to enable/disable port polling
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_polling_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      yt_port_polling_enable_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger, Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_port_polling_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
