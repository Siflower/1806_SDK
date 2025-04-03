/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef __YT_SYS_H__
#define __YT_SYS_H__

/*
 * Include Files
 */
#include "yt_cmm.h"

typedef enum yt_switch_chip_e
{
    SWCHIP_YT9215SC = 92151,
    SWCHIP_YT9215S,
    SWCHIP_YT9215RB,
    SWCHIP_YT9218N = 92180,
    SWCHIP_YT9218M
}yt_switch_chip_t;

/**
 * @internal      yt_sys_mac_get
 * @endinternal
 *
 * @brief         get mac address of system
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]     pSys_mac            -mac address
 * @retval        CMM_ERR_INPUT        -input parameter error
 * @retval        CMM_ERR_PORT        -input port error
 */
extern yt_ret_t yt_sys_mac_get(yt_unit_t unit, yt_mac_addr_t *pSys_mac);


/**
 * @internal      yt_sys_chip_reset
 * @endinternal
 *
 * @brief         reset chip reg value to default
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_sys_chip_reset(yt_unit_t unit);


/**
 * @internal      yt_sys_database_reset
 * @endinternal
 *
 * @brief         reset software database
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_sys_database_reset(yt_unit_t unit);


/**
 * @internal      yt_sys_version_get
 * @endinternal
 *
 * @brief         get sdk version info
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]   pVerStr                -version info in string format
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_sys_version_get(yt_unit_t unit, char *pVerStr);


/**
 * @internal      yt_sys_register_value_get
 * @endinternal
 *
 * @brief         get specific register value
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     regAddr          -register address
 * @param[out]   pVal                -register value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_sys_register_value_get(yt_unit_t unit, uint32_t regAddr, uint32_t *pVal);


/**
 * @internal      yt_sys_register_value_set
 * @endinternal
 *
 * @brief         set specific register value
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     regAddr          -register address
 * @param[in]     value              -register value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_sys_register_value_set(yt_unit_t unit, uint32_t regAddr, uint32_t value);


/**
 * @internal      yt_sys_chipInfo_get
 * @endinternal
 *
 * @brief          get switch chip information
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pChip               -switch chip mode,refer to yt_switch_chip_t
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_sys_chipInfo_get(yt_unit_t unit, yt_switch_chip_t *pChip);


#endif
