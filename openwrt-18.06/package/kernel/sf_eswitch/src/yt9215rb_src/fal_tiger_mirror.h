/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_mirror.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_MIRROR_H
#define __FAL_TIGER_MIRROR_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "fal_cmm.h"

/**
 * @internal      fal_tiger_mirror_init
 * @endinternal
 *
 * @brief         Description
 *                Mirror init API.
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_mirror_init(yt_unit_t unit);

/**
 * @internal      fal_tiger_mirror_group_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     grpId               -group id
 * @param[in]     pMirrorEntry        -pointer to mirror entry
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_mirror_group_set(yt_unit_t unit, yt_mirror_group_t grpId,
                                    yt_mirror_entry_t *pMirrorEntry);


/**
 * @internal      fal_tiger_mirror_group_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     grpId               -group id
 * @param[out]     pMirrorEntry       -pointer to mirror entry
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_mirror_group_get(yt_unit_t unit, yt_mirror_group_t grpId,
                                    yt_mirror_entry_t *pMirrorEntry);




#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
