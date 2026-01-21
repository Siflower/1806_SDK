/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_lag.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_LAG_H
#define __FAL_TIGER_LAG_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "fal_cmm.h"

#define LAG_MEM_NUM_PERGRP      4

/**
 * @internal      fal_tiger_lag_init
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_lag_init(yt_unit_t unit);


/**
 * @internal      fal_tiger_lag_en_get
 * @endinternal
 *
 * @brief         get lag en state and lag id  based on port
 * @param[in]     unit                -unit id
 * @param[out]    yt_enable_t         -lag en
 * @param[out]    uint8_t         -lag id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_lag_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *lagState, uint8_t *lagId);


/**
 * @internal      fal_tiger_lag_hash_sel_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     hash_mask           -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_lag_hash_sel_set(yt_unit_t unit, uint8_t hash_mask);


/**
 * @internal      fal_tiger_lag_hash_sel_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    p_hash_mask         -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_lag_hash_sel_get(yt_unit_t unit, uint8_t *p_hash_mask);


/**
 * @internal      fal_tiger_lag_group_port_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -x
 * @param[in]     member_portmask     -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_lag_group_port_set(yt_unit_t unit, uint8_t groupId, yt_port_mask_t member_portmask);


/**
 * @internal      fal_tiger_lag_group_port_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -x
 * @param[out]    p_member_portmask   -link aggregation group member portmask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_lag_group_port_get(yt_unit_t unit, uint8_t groupId, yt_port_mask_t *p_member_portmask);

extern yt_ret_t fal_tiger_lag_phyPort_belong_lagPort_get(yt_unit_t unit, yt_port_t port, uint8_t *pGroupId);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
