/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_debug.c
*
* @brief x
*
********************************************************************************
*/
#include "fal_dispatch.h"

/**
 * @internal      yt_debug_phyTemplate_test_set
 * @endinternal
 *
 * @brief         set utp template test mode.
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     mode                -different test mode,refer yt_utp_template_testmode_t for details.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_debug_phyTemplate_test_set(yt_unit_t unit, yt_port_t port, yt_utp_template_testmode_t mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->debug_phyTemplate_test_set(unit, port, mode);
}


/**
 * @internal      yt_debug_phyLoopback_test_set
 * @endinternal
 *
 * @brief         set phy loopback mode.
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     phyMode              -copper, phy, combo
 * @param[in]     mode                -internal loopback or external loopback or remote loopback.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_debug_phyLoopback_test_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_phy_loopback_mode_t mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((mode > YT_PHY_LOOPBACK_MODE_DISABLE || mode < YT_PHY_LOOPBACK_MODE_INTERNAL), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->debug_phyLoopback_test_set(unit, port, phyMode, mode);
}

