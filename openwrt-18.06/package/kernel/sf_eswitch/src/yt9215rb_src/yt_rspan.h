/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_rspan.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_RSPAN_H__
#define __YT_RSPAN_H__

#include "yt_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define YT_RSPAN_VLAN_FLAG_TPID     BIT(0)
#define YT_RSPAN_VLAN_FLAG_VID      BIT(1)
#define YT_RSPAN_VLAN_FLAG_PRI      BIT(2)
#define YT_RSPAN_VLAN_FLAG_DEI      BIT(3)
#define YT_RSPAN_VLAN_FLAG_ALL      (YT_RSPAN_VLAN_FLAG_TPID | YT_RSPAN_VLAN_FLAG_VID \
                                    | YT_RSPAN_VLAN_FLAG_PRI | YT_RSPAN_VLAN_FLAG_DEI)

typedef struct yt_rspan_vlan_info_s
{
    uint32_t flags;
    yt_tpid_t tpid;
    yt_vlan_t vid;
    yt_pri_t pri;
    uint8_t dei;
}yt_rspan_vlan_info_t;

typedef enum yt_rspan_src_dev_tag_op_e
{
    YT_RSPAN_SRC_DEV_ADD_TAG,
    YT_RSPAN_SRC_DEV_REPLACE_CTAG,
    YT_RSPAN_SRC_DEV_REPLACE_STAG,
    YT_RSPAN_SRC_DEV_REPLACE_CSTAG,
}yt_rspan_src_dev_tag_op_t;

typedef enum yt_rspan_dst_dev_tag_op_e
{
    YT_RSPAN_DST_DEV_KEEP_TAG,
    YT_RSPAN_DST_DEV_REMOVE_TAG,
}yt_rspan_dst_dev_tag_op_t;

/**
 * @internal      yt_rspan_vlan_info_set
 * @endinternal
 *
 * @brief         Set RSPAN tag content(TPID/VLAN ID/priority/dei) according to flags
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -group id
 * @param[in]     pVlanInfo           -pointer to mirror entry
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rspan_vlan_info_set(yt_unit_t unit, yt_mirror_group_t groupId, yt_rspan_vlan_info_t *pVlanInfo);

/**
 * @internal      yt_rspan_vlan_info_get
 * @endinternal
 *
 * @brief         Get RSPAN tag content(TPID/VLAN ID/priority/dei) according to flags.
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -group id
 * @param[out]    pVlanInfo           -pointer to mirror entry
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rspan_vlan_info_get(yt_unit_t unit, yt_mirror_group_t groupId, yt_rspan_vlan_info_t *pVlanInfo);

/**
 * @internal      yt_rspan_smac_learn_set
 * @endinternal
 *
 * @brief         Configure whether SMAC learning is required for RSPAN packets
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -group id
 * @param[in]     enable              -enable or disable smac learning
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rspan_smac_learn_set(yt_unit_t unit, yt_mirror_group_t groupId, yt_enable_t enable);

/**
 * @internal      yt_rspan_smac_learn_get
 * @endinternal
 *
 * @brief         Get SMAC learning enable status for RSPAN packets
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -group id
 * @param[out]    pEnable             -enable or disable smac learning
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rspan_smac_learn_get(yt_unit_t unit, yt_mirror_group_t groupId, yt_enable_t *pEnable);

/**
 * @internal      yt_rspan_src_dev_tag_op_set
 * @endinternal
 *
 * @brief         Set the tag behavior when the source device sends RSPAN packets
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -group id
 * @param[in]     tagOp               -tag behavior(add tag/replace ctag/replace stag/replace ctag and stag)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rspan_src_dev_tag_op_set(yt_unit_t unit, yt_mirror_group_t groupId, yt_rspan_src_dev_tag_op_t tagOp);

/**
 * @internal      yt_rspan_src_dev_tag_op_get
 * @endinternal
 *
 * @brief         Get the tag behavior when the source device sends RSPAN packets
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -group id
 * @param[out]    pTagOp              -tag behavior(add tag/replace ctag/replace stag/replace ctag and stag)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rspan_src_dev_tag_op_get(yt_unit_t unit, yt_mirror_group_t groupId, yt_rspan_src_dev_tag_op_t *pTagOp);

/**
 * @internal      yt_rspan_dst_dev_tag_op_set
 * @endinternal
 *
 * @brief         Set the tag behavior when the destination device sends RSPAN packets
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -group id
 * @param[in]     tagOp               -tag behavior(remove tag/keep tag)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rspan_dst_dev_tag_op_set(yt_unit_t unit, yt_mirror_group_t groupId, yt_rspan_dst_dev_tag_op_t tagOp);

/**
 * @internal      yt_rspan_dst_dev_tag_op_get
 * @endinternal
 *
 * @brief         Get the tag behavior when the destination device sends RSPAN packets
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -group id
 * @param[out]    pTagOp              -tag behavior(remove tag/keep tag)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rspan_dst_dev_tag_op_get(yt_unit_t unit, yt_mirror_group_t groupId, yt_rspan_dst_dev_tag_op_t *pTagOp);

/**
 * @internal      yt_rspan_egr_enable_set
 * @endinternal
 *
 * @brief         Enable/disable egress RSPAN, usually call on source and destination device
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -group id
 * @param[in]     enable              -enable/disable RSPAN
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rspan_egr_enable_set(yt_unit_t unit, yt_mirror_group_t groupId, yt_enable_t enable);

/**
 * @internal      yt_rspan_egr_enable_get
 * @endinternal
 *
 * @brief         Get RSPAN enable status
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -group id
 * @param[out]    pEnable             -enable/disable RSPAN
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rspan_egr_enable_get(yt_unit_t unit, yt_mirror_group_t groupId, yt_enable_t *pEnable);

/**
 * @internal      yt_rspan_igr_enable_set
 * @endinternal
 *
 * @brief         Enable/disable ingress RSPAN, usually call on destination device
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -the port number of the RSPAN packet received
 * @param[in]     enable              -enable/disable RSPAN
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rspan_igr_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_rspan_igr_enable_get
 * @endinternal
 *
 * @brief         Get ingress RSPAN enable status
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -the port number of the RSPAN packet received
 * @param[out]    pEnable             -enable/disable RSPAN
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rspan_igr_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_rspan_fwd_pkt_filter_enable_set
 * @endinternal
 *
 * @brief         whether to drop RSPAN packets that are normally forwarded on destination device
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable/disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rspan_fwd_pkt_filter_enable_set(yt_unit_t unit, yt_enable_t enable);

/**
 * @internal      yt_rspan_fwd_pkt_filter_enable_get
 * @endinternal
 *
 * @brief         Get the drop status of normally forwarded RSPAN packets on destination device
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable/disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rspan_fwd_pkt_filter_enable_get(yt_unit_t unit, yt_enable_t *pEnable);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
