/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_loopdetect.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_LOOPDETECT_H
#define __FAL_TIGER_LOOPDETECT_H


#include "fal_cmm.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Macro Declaration
 */
#define L2_LOOP_DETECT_FLAG_DUMMY                    0x180814

/**
 * @internal      fal_tiger_loop_detect_enable_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_loop_detect_enable_set(yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      fal_tiger_loop_detect_enable_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_loop_detect_enable_get(yt_unit_t unit, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_loop_detect_tpid_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     tpid                -tpid value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_loop_detect_tpid_set(yt_unit_t unit, yt_tpid_t tpid);


/**
 * @internal      fal_tiger_loop_detect_tpid_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pTpid               -tpid value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_loop_detect_tpid_get(yt_unit_t unit, yt_tpid_t *pTpid);


/**
 * @internal      fal_tiger_loop_detect_generate_way_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     way                 -When a loop occurs, should it be handed over to HW or SW for processing
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_loop_detect_generate_way_set(yt_unit_t unit, yt_generate_way_t way);


/**
 * @internal      fal_tiger_loop_detect_generate_way_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pWay                -When a loop occurs, should it be handed over to HW or SW for processing
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_loop_detect_generate_way_get(yt_unit_t unit, yt_generate_way_t *pWay);


/**
 * @internal      fal_tiger_loop_detect_unitID_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     localID             -local unit id
 * @param[in]     remoteID            -remote unit id, support two remote unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_loop_detect_unitID_set(yt_unit_t unit, yt_local_id_t localID, yt_remote_id_t remoteID);


/**
 * @internal      fal_tiger_loop_detect_unitID_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pLocalID            -local unit id
 * @param[out]    pRemoteID           -remote unit id, support two remote unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_loop_detect_unitID_get(yt_unit_t unit, yt_local_id_t *pLocalID, yt_remote_id_t *pRemoteID);

/**
 * @internal      fal_tiger_loop_detect_loopedPorts_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pPortMask          -pointer to port mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_loop_detect_loopedPorts_get(yt_unit_t unit, yt_port_mask_t *pPortMask);
extern yt_ret_t fal_tiger_loop_detect_interval_set(yt_unit_t unit, uint32_t interval);
extern yt_ret_t fal_tiger_loop_detect_interval_get(yt_unit_t unit, uint32_t *pInterval);
extern yt_ret_t fal_tiger_loop_detect_prevent_enable_set(yt_unit_t unit, yt_enable_t enable);
extern yt_ret_t fal_tiger_loop_detect_prevent_enable_get(yt_unit_t unit, yt_enable_t *pEnable);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
