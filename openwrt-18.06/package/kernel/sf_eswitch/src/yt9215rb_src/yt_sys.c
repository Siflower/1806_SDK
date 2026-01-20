/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_sys.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_sys.h"
#include "fal_dispatch.h"
#include "version.h"
#include "hal_mem.h"


/**
 * @internal      yt_sys_mac_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     psys_mac            -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_sys_mac_get(yt_unit_t unit, yt_mac_addr_t *pSys_mac)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pSys_mac), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->sys_mac_get(unit, pSys_mac);
}

/**
 * @internal      yt_sys_chip_reset
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_sys_chip_reset(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->sys_chip_reset(unit);
}

/**
 * @internal      yt_sys_database_reset
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_sys_database_reset(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->sys_database_reset(unit);
}

/**
 * @internal      yt_sys_version_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]   pVerStr                -version info in string format
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_sys_version_get(yt_unit_t unit, char *pVerStr)
{
    CMM_UNUSED_PARAM(unit);

    CMM_PARAM_CHK((NULL == pVerStr), CMM_ERR_NULL_POINT);

    osal_strcpy(pVerStr, osal_strlen(pVerStr)+1, YT_SDK_VERSION);

    return CMM_ERR_OK;
}

/**
 * @internal      yt_sys_register_value_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     regAddr          -register address
 * @param[out]   pVal                -register value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_sys_register_value_get(yt_unit_t unit, uint32_t regAddr, uint32_t *pVal)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pVal), CMM_ERR_NULL_POINT);

    return HAL_MEM_DIRECT_READ(unit, regAddr, pVal);
}

/**
 * @internal      yt_sys_register_value_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     regAddr          -register address
 * @param[in]     value              -register value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_sys_register_value_set(yt_unit_t unit, uint32_t regAddr, uint32_t value)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);

    return HAL_MEM_DIRECT_WRITE(unit, regAddr, value);
}

/**
 * @internal      yt_sys_chipInfo_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pChip               -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_sys_chipInfo_get(yt_unit_t unit, yt_switch_chip_t *pChip)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pChip), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->sys_chipInfo_get(unit, pChip);
}

#if defined(LINUX_KERNEL_MODE)
EXPORT_SYMBOL(yt_sys_chip_reset);
#endif