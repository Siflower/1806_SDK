/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_lag.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_LAG_H
#define __YT_LAG_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "yt_cmm.h"

#define YT_AGG_HASH_SEL_SRC_PORT      BIT(0U)
#define YT_AGG_HASH_SEL_MAC_DA          BIT(1U)
#define YT_AGG_HASH_SEL_MAC_SA           BIT(2U)
#define YT_AGG_HASH_SEL_IP_DEST           BIT(3U)
#define YT_AGG_HASH_SEL_IP_SRC             BIT(4U)
#define YT_AGG_HASH_SEL_IP_PROTO       BIT(5U)
#define YT_AGG_HASH_SEL_L4_DPORT      BIT(6U)
#define YT_AGG_HASH_SEL_L4_SPORT       BIT(7U)
#define YT_LAG_AGG_HASH_SEL_MAX       (8U)

/**
 * @internal      yt_lag_init
 * @endinternal
 *
 * @brief         init lag hash sel
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_lag_init(yt_unit_t unit);

/**
 * @internal      yt_lag_en_get
 * @endinternal
 *
 * @brief         get lag en state and lag id  based on port
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    yt_enable_t         -lag en
 * @param[out]    uint8_t         -lag id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_lag_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *lagState, uint8_t *lagId);

/**
 * @internal      yt_lag_hash_sel_set
 * @endinternal
 *
 * @brief         set link aggregation hash algorithm bitmask
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     hashMask           -link aggregation hash algorithm bitmask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_lag_hash_sel_set(yt_unit_t unit, uint16_t hashMask);


/**
 * @internal      yt_lag_hash_sel_get
 * @endinternal
 *
 * @brief         get link aggregation hash algorithm bitmask
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pHashMask         -link aggregation hash algorithm bitmask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_lag_hash_sel_get(yt_unit_t unit, uint16_t *pHashMask);


/**
 * @internal      yt_lag_group_port_set
 * @endinternal
 *
 * @brief         set lag group member ports
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -lag group index
 * @param[in]     memberPortMask     -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_SAMEENTRY_EXIST   - member port conflict with other lag
 */
extern yt_ret_t yt_lag_group_port_set(yt_unit_t unit, uint8_t groupId, yt_port_mask_t memberPortMask);


/**
 * @internal      yt_lag_group_port_get
 * @endinternal
 *
 * @brief         get lag group member portmask
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -lag group index
 * @param[out]    pMemberPortmask   -link aggregation group portmask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_lag_group_port_get(yt_unit_t unit, uint8_t groupId, yt_port_mask_t *pMemberPortmask);



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
