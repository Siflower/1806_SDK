/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_port.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_port.h"
#include "fal_dispatch.h"

yt_ret_t yt_port_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->port_init(unit);
}

yt_ret_t yt_port_default_cfg_set(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->port_default_cfg_set(unit);
}

yt_ret_t yt_port_mac_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_mac_enable_set(unit, port, enable);
}

yt_ret_t yt_port_mac_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_mac_enable_get(unit, port, pEnable);
}

yt_ret_t yt_port_link_status_get(yt_unit_t unit, yt_port_t port, yt_port_link_status_t *pLinkStatus)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pLinkStatus), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_link_status_get(unit, port, pLinkStatus);
}

yt_ret_t yt_port_link_status_all_get(yt_unit_t unit, yt_port_t port, yt_port_linkStatus_all_t *pAllLinkStatus)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pAllLinkStatus), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_link_status_all_get(unit, port, pAllLinkStatus);
}

yt_ret_t yt_port_backpress_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_backpress_enable_set(unit, port, enable);
}

yt_ret_t yt_port_backpress_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_backpress_enable_get(unit, port, pEnable);
}

yt_ret_t yt_port_cascade_set(yt_unit_t unit, yt_cascade_info_t cascade_info)
{
    uint32_t i;

    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < cascade_info.en || YT_DISABLE > cascade_info.en), CMM_ERR_INPUT);
    for (i = 0; i < YT_MAX_CASCADE_PORT_NUM; i++)
    {
        if (cascade_info.ports[i].valid)
            CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, cascade_info.ports[i].port_num))), CMM_ERR_PORT);
    }

    return YT_DISPATCH(unit)->port_cascade_set(unit, cascade_info);
}

yt_ret_t yt_port_cascade_get(yt_unit_t unit, yt_cascade_info_t *pCascade_info)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pCascade_info), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_cascade_get(unit, pCascade_info);
}

yt_ret_t yt_port_pkt_gap_set(yt_unit_t unit, yt_port_t port, uint8_t gap)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_PKTGAP_SIZE_MAX < gap), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_pkt_gap_set(unit, port, gap);
}

yt_ret_t yt_port_pkt_gap_get(yt_unit_t unit, yt_port_t port, uint8_t *pGap)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pGap), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_pkt_gap_get(unit, port, pGap);
}

yt_ret_t yt_port_macAutoNeg_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_macAutoNeg_enable_set(unit, port, enable);
}

yt_ret_t yt_port_macAutoNeg_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_macAutoNeg_enable_get(unit, port, pEnable);
}

yt_ret_t yt_port_mac_force_set(yt_unit_t unit, yt_port_t port, yt_port_speed_duplex_t port_ctrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->port_mac_force_set(unit, port, port_ctrl);
}

yt_ret_t yt_port_mac_force_get(yt_unit_t unit, yt_port_t port, yt_port_speed_duplex_t *pPort_ctrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pPort_ctrl), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_mac_force_get(unit, port, pPort_ctrl);
}

yt_ret_t yt_port_mac_fc_set(yt_unit_t unit, yt_port_t port, yt_enable_t fcAutoNegEnable, yt_enable_t rxFcEnable, yt_enable_t txFcEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < fcAutoNegEnable || YT_DISABLE > fcAutoNegEnable), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ENABLE < rxFcEnable || YT_DISABLE > rxFcEnable), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ENABLE < txFcEnable || YT_DISABLE > txFcEnable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_mac_fc_set(unit, port, fcAutoNegEnable, rxFcEnable, txFcEnable);
}

yt_ret_t yt_port_mac_fc_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pFcAutoNegEnable, yt_enable_t *pRxFcEnable, yt_enable_t *pTxFcEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pFcAutoNegEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pRxFcEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pTxFcEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_mac_fc_get(unit, port, pFcAutoNegEnable, pRxFcEnable, pTxFcEnable);
}

yt_ret_t yt_port_mac_block_set(yt_unit_t unit, yt_port_t port, yt_enable_t rxMacEnable, yt_enable_t txMacEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < rxMacEnable || YT_DISABLE > rxMacEnable), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ENABLE < txMacEnable || YT_DISABLE > txMacEnable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_mac_block_set(unit, port, rxMacEnable, txMacEnable);
}

yt_ret_t yt_port_mac_block_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pRxMacEnable, yt_enable_t *pTxMacEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pRxMacEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pTxMacEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_mac_block_get(unit, port, pRxMacEnable, pTxMacEnable);
}

yt_ret_t yt_port_mac_eee_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_mac_eee_enable_set(unit, port, enable);
}

yt_ret_t yt_port_mac_eee_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_mac_eee_enable_get(unit, port, pEnable);
}

yt_ret_t yt_port_mac_eee_timer_set(yt_unit_t unit, yt_port_t port, uint32_t wakeupTimer, uint32_t sleepTimer)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_EEE_WAKEUP_TIMER_MAX <= wakeupTimer ||YT_EEE_SLEEP_TIMER_MAX <= sleepTimer), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_mac_eee_timer_set(unit, port, wakeupTimer, sleepTimer);
}

yt_ret_t yt_port_mac_eee_timer_get(yt_unit_t unit, yt_port_t port, uint32_t *pWakeupTimer, uint32_t *pSleepTimer)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pWakeupTimer ||NULL == pSleepTimer), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_mac_eee_timer_get(unit, port, pWakeupTimer, pSleepTimer);
}

yt_ret_t yt_port_extif_mode_set(yt_unit_t unit, yt_port_t port, yt_extif_mode_t mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_EXTIF_MODE_USXGMII_10G < mode || YT_EXTIF_MODE_MII > mode), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_extif_mode_set(unit, port, mode);
}

yt_ret_t yt_port_extif_mode_get(yt_unit_t unit, yt_port_t port, yt_extif_mode_t *pMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_extif_mode_get(unit, port, pMode);
}

yt_ret_t yt_port_extif_rgmii_delay_set(yt_unit_t unit, yt_port_t port, uint8_t rxc_delay, uint8_t txc_delay, yt_enable_t txc_2ns_en)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < txc_2ns_en || YT_DISABLE > txc_2ns_en), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_extif_rgmii_delay_set(unit, port, rxc_delay, txc_delay, txc_2ns_en);
}

yt_ret_t yt_port_extif_rgmii_delay_get(yt_unit_t unit, yt_port_t port, uint8_t *pRxc_delay, uint8_t *pTxc_delay, yt_enable_t *pTxc_2ns_en)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pRxc_delay), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pTxc_delay), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pTxc_2ns_en), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_extif_rgmii_delay_get(unit, port, pRxc_delay, pTxc_delay, pTxc_2ns_en);
}

yt_ret_t yt_port_phy_enable_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);
    CMM_PARAM_CHK(((YT_PHY_CHIP_MODE_COMBO < phyMode) || (YT_PHY_CHIP_MODE_COPPER > phyMode)), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_phy_enable_set(unit, port, phyMode, enable);
}

yt_ret_t yt_port_phy_enable_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(((YT_PHY_CHIP_MODE_FIBER < phyMode) || (YT_PHY_CHIP_MODE_COPPER > phyMode)), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_phy_enable_get(unit, port, phyMode, pEnable);
}

yt_ret_t yt_port_phyAutoNeg_enable_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);
    CMM_PARAM_CHK(((YT_PHY_CHIP_MODE_COMBO < phyMode) || (YT_PHY_CHIP_MODE_COPPER > phyMode)), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_phyAutoNeg_enable_set(unit, port, phyMode, enable);
}

yt_ret_t yt_port_phyAutoNeg_enable_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(((YT_PHY_CHIP_MODE_FIBER < phyMode) || (YT_PHY_CHIP_MODE_COPPER > phyMode)), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_phyAutoNeg_enable_get(unit, port, phyMode, pEnable);
}

yt_ret_t yt_port_phyAutoNeg_ability_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t ability)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK(((YT_PHY_CHIP_MODE_COMBO < phyMode) || (YT_PHY_CHIP_MODE_COPPER > phyMode)), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_phyAutoNeg_ability_set(unit, port, phyMode, ability);
}

yt_ret_t yt_port_phyAutoNeg_ability_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_an_ability_t *pAbility)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pAbility), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(((YT_PHY_CHIP_MODE_FIBER < phyMode) || (YT_PHY_CHIP_MODE_COPPER > phyMode)), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_phyAutoNeg_ability_get(unit, port, phyMode, pAbility);
}

yt_ret_t yt_port_phy_force_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t speed_dup)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((PORT_SPEED_DUP_10GFULL < speed_dup || PORT_SPEED_DUP_10HALF > speed_dup), CMM_ERR_INPUT);
    CMM_PARAM_CHK(((YT_PHY_CHIP_MODE_COMBO < phyMode) || (YT_PHY_CHIP_MODE_COPPER > phyMode)), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_phy_force_set(unit, port, phyMode, speed_dup);
}

yt_ret_t yt_port_phy_force_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_speed_duplex_t *pSpeedDup)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pSpeedDup), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(((YT_PHY_CHIP_MODE_FIBER < phyMode) || (YT_PHY_CHIP_MODE_COPPER > phyMode)), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_phy_force_get(unit, port, phyMode, pSpeedDup);
}

yt_ret_t yt_port_phy_linkstatus_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_port_linkStatus_all_t *pLinkStatus)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pLinkStatus), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(((YT_PHY_CHIP_MODE_FIBER < phyMode) || (YT_PHY_CHIP_MODE_COPPER > phyMode)), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_phy_linkstatus_get(unit, port, phyMode, pLinkStatus);
}

yt_ret_t yt_port_phy_reg_set(yt_unit_t unit, yt_port_t port, uint32_t regAddr, uint16_t data, yt_phy_type_t type, yt_phy_reg_type_t regType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((PHY_EXTERNAL < type || PHY_INTERNAL > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_PHY_REG_TYPE_USXG_EXT < regType || YT_PHY_REG_TYPE_TOP_EXT > regType), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_phy_reg_set(unit, port, regAddr, data, type, regType);
}

yt_ret_t yt_port_phy_reg_get(yt_unit_t unit, yt_port_t port, uint32_t regAddr, uint16_t *pData, yt_phy_type_t type, yt_phy_reg_type_t regType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pData), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((PHY_EXTERNAL < type || PHY_INTERNAL > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_PHY_REG_TYPE_USXG_EXT < regType || YT_PHY_REG_TYPE_TOP_EXT > regType), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_phy_reg_get(unit, port, regAddr, pData, type, regType);
}

yt_ret_t yt_port_phy_eee_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);
    
    return YT_DISPATCH(unit)->port_phy_eee_enable_set(unit, port, enable);
}

yt_ret_t yt_port_phy_eee_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_phy_eee_enable_get(unit, port, pEnable);
}

yt_ret_t yt_port_phy_eee_status_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
   
    return YT_DISPATCH(unit)->port_phy_eee_status_get(unit, port, pEnable);
}

#if defined(SWITCH_SERIES_TIGER)

yt_ret_t yt_port_jumbo_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_jumbo_enable_set(unit, port, enable);
}

yt_ret_t yt_port_jumbo_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_jumbo_enable_get(unit, port, pEnable);
}

yt_ret_t yt_port_jumbo_size_set(yt_unit_t unit, yt_port_t port, uint32_t size)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_JUMBO_SIZE_MAX < size || size < YT_JUMBO_SIZE_MIN), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->port_jumbo_size_set(unit, port, size);
}

yt_ret_t yt_port_jumbo_size_get(yt_unit_t unit, yt_port_t port, uint32_t *pSize)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pSize), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_jumbo_size_get(unit, port, pSize);
}

#else

yt_ret_t yt_port_jumbo_enable_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_jumbo_enable_set(unit, enable);
}

yt_ret_t yt_port_jumbo_enable_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_jumbo_enable_get(unit, pEnable);
}

yt_ret_t yt_port_jumbo_size_set(yt_unit_t unit, uint32_t size)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_JUMBO_SIZE_MAX < size || size < YT_JUMBO_SIZE_MIN), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->port_jumbo_size_set(unit, size);
}

yt_ret_t yt_port_jumbo_size_get(yt_unit_t unit, uint32_t *pSize)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pSize), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_jumbo_size_get(unit, pSize);
}

#endif

yt_ret_t yt_port_cable_diag_start(yt_unit_t unit, yt_port_t port)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->port_cable_diag_start(unit, port);
}

yt_ret_t yt_port_cable_diag_result_get(yt_unit_t unit, yt_port_t port, yt_port_cableDiag_t *pCableStatus)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pCableStatus), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_cable_diag_result_get(unit, port, pCableStatus);
}

yt_ret_t yt_port_phyCrossover_mode_set(yt_unit_t unit, yt_port_t port, yt_utp_crossover_mode_t mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    
    return YT_DISPATCH(unit)->port_phyCrossover_mode_set(unit, port,  mode);
}

yt_ret_t yt_port_phyCrossover_mode_get(yt_unit_t unit, yt_port_t port, yt_utp_crossover_mode_t *pMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pMode), CMM_ERR_NULL_POINT);
   
    return YT_DISPATCH(unit)->port_phyCrossover_mode_get(unit, port, pMode);
}

yt_ret_t yt_port_phy_chip_mode_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_PHY_CHIP_MODE_RESERVED < mode || mode < YT_PHY_CHIP_MODE_COPPER), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->port_phy_chip_mode_set(unit, port, mode);
}

yt_ret_t yt_port_phy_chip_mode_get(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t *pMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_phy_chip_mode_get(unit, port, pMode);
}

yt_ret_t yt_port_phyCombo_mode_set(yt_unit_t unit, yt_port_t port, yt_combo_mode_t mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((COMBO_MODE_FIBER_FIRST < mode || mode < COMBO_MODE_COPPER_FIRST), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->port_phyCombo_mode_set(unit, port, mode);
}

yt_ret_t yt_port_phyCombo_mode_get(yt_unit_t unit, yt_port_t port, yt_combo_mode_t *pMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_phyCombo_mode_get(unit, port, pMode);
}

yt_ret_t yt_port_phyCrossover_status_get(yt_unit_t unit, yt_port_t port, yt_utp_crossover_status_t *pStatus)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pStatus), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_phyCrossover_status_get(unit, port, pStatus);
}

yt_ret_t yt_port_phy_smart_downgrade_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

     return YT_DISPATCH(unit)->port_phy_smart_downgrade_set(unit, port, enable);
}

 yt_ret_t yt_port_phy_smart_downgrade_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pStatus)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pStatus), CMM_ERR_NULL_POINT);

     return YT_DISPATCH(unit)->port_phy_smart_downgrade_get(unit, port, pStatus);
}

yt_ret_t yt_port_phy_green_start(yt_unit_t unit, yt_port_t port)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    
     return YT_DISPATCH(unit)->port_phy_green_start(unit, port);
}

 yt_ret_t yt_port_phy_green_result_get(yt_unit_t unit, yt_port_t port, yt_port_cableDiag_t *pStatus)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pStatus), CMM_ERR_NULL_POINT);
   
     return YT_DISPATCH(unit)->port_phy_green_result_get(unit, port, pStatus);
}

yt_ret_t yt_port_extif_xmiiClk_invert_set(yt_unit_t unit, yt_port_t port, yt_enable_t state)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < state || YT_DISABLE > state), CMM_ERR_INPUT);

     return YT_DISPATCH(unit)->port_extif_xmiiClk_invert_set(unit, port, state);
}

yt_ret_t yt_port_extif_xmiiClk_invert_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pState)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pState), CMM_ERR_NULL_POINT);

     return YT_DISPATCH(unit)->port_extif_xmiiClk_invert_get(unit, port, pState);
}

yt_ret_t yt_port_parallel_detection_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_parallel_detection_set(unit, port, enable);
}

yt_ret_t yt_port_parallel_detection_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_parallel_detection_get(unit, port, pEnable);
}

yt_ret_t yt_port_dvddio_power_level_set(yt_unit_t unit, yt_dvddio_power_pad_t powePad, yt_dvddio_power_level_t powerLevel)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((2 < powePad), CMM_ERR_INPUT);
    CMM_PARAM_CHK((POWER33V < powerLevel || POWER18V > powerLevel), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->port_dvddio_power_level_set(unit, powePad, powerLevel);
}

yt_ret_t yt_port_dvddio_power_level_get(yt_unit_t unit, yt_dvddio_power_pad_t powePad, yt_dvddio_power_level_t *pPowerLevel)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((2 < powePad), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pPowerLevel), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_dvddio_power_level_get(unit, powePad, pPowerLevel);
}

yt_ret_t yt_port_polling_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

     return YT_DISPATCH(unit)->port_polling_enable_set(unit, port, enable);
}

yt_ret_t yt_port_polling_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

     return YT_DISPATCH(unit)->port_polling_enable_get(unit, port, pEnable);
}

#if defined(LINUX_KERNEL_MODE)
EXPORT_SYMBOL(yt_port_phy_force_set);
EXPORT_SYMBOL(yt_port_phyAutoNeg_enable_get);
EXPORT_SYMBOL(yt_port_phy_linkstatus_get);
EXPORT_SYMBOL(yt_port_phyAutoNeg_enable_set);
EXPORT_SYMBOL(yt_port_phy_enable_set);
EXPORT_SYMBOL(yt_port_phy_enable_get);
#endif