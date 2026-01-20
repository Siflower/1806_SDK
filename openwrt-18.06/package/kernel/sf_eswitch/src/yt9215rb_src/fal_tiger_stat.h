/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_stat.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_STAT_H
#define __FAL_TIGER_STAT_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "fal_cmm.h"
#include "yt_types.h"
#include "yt_stat.h"

#define MIB_CTRL_ADDR   (0xc0000)
#define MIB_OP_ADDR (0xc0004)
#define MIB_TBL_BASE_ADDR0  (0xc0100)
#define MIB_MAX_COUNTER (0x2e)
#define MIB_COUNTER_MAX    (0x2e -5)

/**
 * @internal      fal_tiger_mib_init
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stat_mib_init(yt_unit_t unit);


/**
 * @internal      fal_tiger_stat_mib_enable_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     enable              -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stat_mib_enable_set (yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      fal_tiger_stat_mib_enable_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stat_mib_enable_get (yt_unit_t unit, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_stat_mib_clear
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stat_mib_clear (yt_unit_t unit, yt_port_t port);


/**
 * @internal      fal_tiger_stat_mib_clear_all
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stat_mib_clear_all (yt_unit_t unit);


/**
 * @internal      fal_tiger_stat_mib_port_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pcnt                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stat_mib_port_get (yt_unit_t unit, yt_port_t port, yt_stat_mib_port_cnt_t *pcnt);

extern yt_ret_t fal_tiger_stat_mib_port_index_get (yt_unit_t unit, yt_port_t port, uint32_t index, uint64 *pcnt);

extern yt_ret_t fal_tiger_stat_mib_port_singleType_cnt_get(yt_unit_t unit, yt_port_t port, yt_stat_mib_t mibType, uint64 *pCnt);

/**
 * @internal      fal_tiger_stat_flow_enable_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     flow_id             -x
 * @param[in]     enable              -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stat_flow_enable_set (yt_unit_t unit, uint32_t flow_id, yt_enable_t enable);


/**
 * @internal      fal_tiger_stat_flow_enable_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     flow_id             -x
 * @param[in]     enable              -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stat_flow_enable_get (yt_unit_t unit, uint32_t flow_id, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_stat_flow_mode_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     flow_id             -x
 * @param[in]     type                -x
 * @param[in]     mode                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stat_flow_mode_set (yt_unit_t unit, uint32_t flow_id, yt_stat_type_t type, yt_stat_mode_t mode);


/**
 * @internal      fal_tiger_stat_flow_mode_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     cpu_offset          -x
 * @param[in]     drop_offset         -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stat_flow_mode_get (yt_unit_t unit, uint32_t flow_id, yt_stat_type_t *ptype, yt_stat_mode_t *pmode);


/**
 * @internal      fal_tiger_stat_flow_count_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     flow_id             -x
 * @param[in]     count               -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stat_flow_count_set (yt_unit_t unit, uint32_t flow_id, uint64 count);


/**
 * @internal      fal_tiger_stat_flow_count_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     flow_id             -x
 * @param[out]    count               -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_stat_flow_count_get (yt_unit_t unit, uint32_t flow_id, uint64 *count);




#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
