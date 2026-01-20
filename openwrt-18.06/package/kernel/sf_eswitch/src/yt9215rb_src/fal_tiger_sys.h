/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#ifndef __FAL_TIGER_SYS_H__
#define __FAL_TIGER_SYS_H__


#include "yt_types.h"
#include "yt_cmm.h"
#include "yt_sys.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @internal      fal_tiger_sys_mac_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     psys_mac            -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_sys_mac_get(yt_unit_t unit, yt_mac_addr_t *psys_mac);


/**
 * @internal      fal_tiger_sys_chip_reset
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_sys_chip_reset(yt_unit_t unit);


/**
 * @internal      fal_tiger_sys_database_reset
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_sys_database_reset(yt_unit_t unit);


/**
 * @internal      fal_tiger_sys_chipInfo_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pChip               -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_sys_chipInfo_get(yt_unit_t unit, yt_switch_chip_t *pChip);




#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

