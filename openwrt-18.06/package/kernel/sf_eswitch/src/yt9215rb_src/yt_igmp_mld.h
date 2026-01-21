/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_igmp_mld.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_IGMP_MLD_H
#define __YT_IGMP_MLD_H

#include "yt_cmm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(SWITCH_SERIES_TIGER)
#define YT_MAX_MULTICAST_ROUTER_PORT    (2U)

typedef enum yt_igmp_mld_op_mode_e
{
    YT_MULTI_OP_MODE_LEARN,
    YT_MULTI_OP_MODE_FLOOD,
    YT_MULTI_OP_MODE_TRAP,
    YT_MULTI_OP_MODE_LEARN_AND_COPY_TO_CPU,
} yt_igmp_mld_op_mode_t;

typedef struct yt_igmp_mld_learn_bypass_range_s
{
    uint8_t    bypass_239_255_255_x_en;
    uint8_t    bypass_224_0_1_x_en;
    uint8_t    bypass_224_0_0_x_en;
    uint8_t    bypass_ipv6_00xx_en;
} yt_igmp_mld_learn_bypass_range_t;

typedef struct yt_igmp_mld_router_port_s
{
    uint8_t    valid[YT_MAX_MULTICAST_ROUTER_PORT];
    yt_port_t  port[YT_MAX_MULTICAST_ROUTER_PORT];
} yt_igmp_mld_router_port_t;

#endif

#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)

#define YT_IGMP_ROUTER_PORT_AGING_MAXTIME  1024
#define YT_IGMP_DYNAMIC_ROUTER_GROUP_NUM  8
#define YT_IGMP_STATIC_ROUTER_GROUP_NUM  4
#define YT_MLD_ROUTER_PORT_AGING_MAXTIME  1024
#define YT_MLD_DYNAMIC_ROUTER_GROUP_NUM  8
#define YT_MLD_STATIC_ROUTER_GROUP_NUM  4
#define YT_IGMP_MLD_MULTICAST_GROUP_NUM  256
#define YT_IGMP_MLD_MULTICAST_PER_PORT_GROUP_NUM  128
#define YT_IGMP_MLD_MEMBER_PORT_AGING_MAXTIME  1024

typedef enum yt_igmp_version_s {
    YT_IGMP_V1V2,
    YT_IGMP_V3,
} yt_igmp_version_t;

typedef enum yt_igmp_v1v2_ctrlPkt_type_s {
    YT_IGMP_V1V2_QUERY,
    YT_IGMP_V1V2_REPORT,
    YT_IGMP_V2_LEAVE,
} yt_igmp_v1v2_ctrlPkt_type_t;

typedef enum yt_igmp_v3_ctrlPkt_type_s {
    YT_IGMP_V3_QUERY,
    YT_IGMP_V3_REPORT,
} yt_igmp_v3_ctrlPkt_type_t;

typedef enum yt_mld_version_s {
    YT_MLD_V1,
    YT_MLD_V2,
} yt_mld_version_t;

typedef enum yt_mld_v1_ctrlPkt_type_s {
    YT_MLD_V1_QUERY,
    YT_MLD_V1_REPORT,
    YT_MLD_V1_LEAVE,
} yt_mld_v1_ctrlPkt_type_t;

typedef enum yt_mld_v2_ctrlPkt_type_s {
    YT_MLD_V2_QUERY,
    YT_MLD_V2_REPORT,
} yt_mld_v2_ctrlPkt_type_t;

typedef enum yt_igmp_mld_fwd_ports_s {
    YT_IGMP_MLD_FLOOD,
    YT_IGMP_MLD_ROUTER_PORTS,
} yt_igmp_mld_fwd_ports_t;

typedef enum yt_igmp_mld_leave_aging_s {
    YT_IGMP_MLD_2S,
    YT_IGMP_MLD_4S,
    YT_IGMP_MLD_8S,
    YT_IGMP_MLD_10S,
} yt_igmp_mld_leave_aging_t;
#endif

#if defined(SWITCH_SERIES_TIGER)
/**
 * @internal      yt_igmp_opmode_set
 * @endinternal
 *
 * @brief         set operaction mode for igmp packet
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     mode                -IGMP/MLD operation mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_opmode_set(yt_unit_t unit,  yt_igmp_mld_op_mode_t mode);

/**
 * @internal      yt_igmp_opmode_get
 * @endinternal
 *
 * @brief         get operaction mode for igmp packet
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pmode               -IGMP/MLD operation mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_opmode_get(yt_unit_t unit,  yt_igmp_mld_op_mode_t *pmode);

/**
 * @internal      yt_mld_opmode_set
 * @endinternal
 *
 * @brief         set operaction mode for mld packet
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     mode                -IGMP/MLD operation mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_mld_opmode_set(yt_unit_t unit,  yt_igmp_mld_op_mode_t mode);

/**
 * @internal      yt_mld_opmode_get
 * @endinternal
 *
 * @brief         get operaction mode for mld packet
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pmode               -IGMP/MLD operation mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_mld_opmode_get(yt_unit_t unit,  yt_igmp_mld_op_mode_t *pmode);

/**
 * @internal      yt_igmp_mld_port_report_allow_set
 * @endinternal
 *
 * @brief         enable igmp/mld report on port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     en                  -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_port_report_allow_set(yt_unit_t unit, yt_port_t port, yt_enable_t en);

/**
 * @internal      yt_igmp_mld_port_report_allow_get
 * @endinternal
 *
 * @brief         get enable state of igmp/mld report on port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pen                 -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_port_report_allow_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pen);

/**
 * @internal      yt_igmp_mld_port_leave_allow_set
 * @endinternal
 *
 * @brief         enable igmp/mld leave on port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     en                  -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_port_leave_allow_set(yt_unit_t unit, yt_port_t port, yt_enable_t en);

/**
 * @internal      yt_igmp_mld_port_leave_allow_get
 * @endinternal
 *
 * @brief         get enable state of igmp/mld leave on port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pen                 -enable or not
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_port_leave_allow_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pen);

/**
 * @internal      yt_igmp_mld_port_query_allow_set
 * @endinternal
 *
 * @brief         enable igmp/mld query on port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     en                  -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_port_query_allow_set(yt_unit_t unit, yt_port_t port, yt_enable_t en);

/**
 * @internal      yt_igmp_mld_port_query_allow_get
 * @endinternal
 *
 * @brief         get enable state of igmp/mld query on port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pen                 -enable or not
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_port_query_allow_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pen);

/**
 * @internal      yt_igmp_mld_fastleave_set
 * @endinternal
 *
 * @brief         enable fastleave
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     en                  -enable or not
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_fastleave_set(yt_unit_t unit, yt_enable_t en);

/**
 * @internal      yt_igmp_mld_fastleave_get
 * @endinternal
 *
 * @brief         get enable state of fastleave
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pen                 -enable or not
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_fastleave_get(yt_unit_t unit, yt_enable_t *pen);

/**
 * @internal      yt_igmp_mld_learnlimit_en_set
 * @endinternal
 *
 * @brief         enable multicast group limit
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     en                  -enable or not
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_learnlimit_en_set(yt_unit_t unit, yt_enable_t en);

/**
 * @internal      yt_igmp_mld_learnlimit_en_get
 * @endinternal
 *
 * @brief         get enable state of multicast group limit
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pen                 -enable or not
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_learnlimit_en_get(yt_unit_t unit, yt_enable_t *pen);

/**
 * @internal      yt_igmp_mld_learnlimit_maxgroup_set
 * @endinternal
 *
 * @brief         set max multicast group number
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     maxgroup            -max group number
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_learnlimit_maxgroup_set(yt_unit_t unit, uint32_t  maxgroup);

/**
 * @internal      yt_igmp_mld_learnlimit_maxgroup_get
 * @endinternal
 *
 * @brief         get max multicast group number
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pmaxgroup           -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_learnlimit_maxgroup_get(yt_unit_t unit, uint32_t  *pmaxgroup);

/**
 * @internal      yt_igmp_mld_fwd_routerport_only_set
 * @endinternal
 *
 * @brief         enable forward igmp/mld packet to router port only
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     en                  -enable or not
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_fwd_routerport_only_set(yt_unit_t unit, yt_enable_t en);

/**
 * @internal      yt_igmp_mld_fwd_routerport_only_get
 * @endinternal
 *
 * @brief         get enable state of forward igmp/mld packet to router port only
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pen                 -enable or not
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_fwd_routerport_only_get(yt_unit_t unit, yt_enable_t *pen);

/**
 * @internal      yt_igmp_mld_fwd_routerport_primary_set
 * @endinternal
 *
 * @brief         enable forward igmp/mld packet to router port only if router port exist,otherwire flood
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     en                  -enable or not
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_fwd_routerport_primary_set(yt_unit_t unit, yt_enable_t en);

/**
 * @internal      yt_igmp_mld_fwd_routerport_primary_get
 * @endinternal
 *
 * @brief         get enable state of forward igmp/mld packet to router port only if router port exist
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pen                 -enable or not
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_fwd_routerport_primary_get(yt_unit_t unit, yt_enable_t *pen);

/**
 * @internal      yt_igmp_mld_bypass_grouprange_set
 * @endinternal
 *
 * @brief         set ip range that won't be learnt to multicast group
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     bypass              -Multicast IP range that bypass learn
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_igmp_mld_bypass_grouprange_set(yt_unit_t unit, yt_igmp_mld_learn_bypass_range_t bypass);

/**
 * @internal      yt_igmp_mld_bypass_grouprange_get
 * @endinternal
 *
 * @brief         get ip range that won't be learnt to multicast group
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pbypass             -Multicast IP range that bypass learn
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_igmp_mld_bypass_grouprange_get(yt_unit_t unit, yt_igmp_mld_learn_bypass_range_t *pbypass);

/**
 * @internal      yt_igmp_mld_dynamic_routerport_get
 * @endinternal
 *
 * @brief         get dynamic multicast router ports
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    prouter_port        -Multicast router port info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_dynamic_routerport_get(yt_unit_t unit, yt_igmp_mld_router_port_t *prouter_port);

/**
 * @internal      yt_igmp_mld_dynamic_routerport_allow_set
 * @endinternal
 *
 * @brief         enable dynamic router port learning on ports
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     en                  -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_igmp_mld_dynamic_routerport_allow_set(yt_unit_t unit, yt_port_t port, yt_enable_t en);

/**
 * @internal      yt_igmp_mld_dynamic_routerport_allow_get
 * @endinternal
 *
 * @brief         get the enable state of dynamic router port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pen                 -enable or not
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_igmp_mld_dynamic_routerport_allow_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pen);

/**
 * @internal      yt_igmp_mld_dynamic_routerport_agingtime_set
 * @endinternal
 *
 * @brief         set dynamic router port aging time
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     sec                 -second
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_igmp_mld_dynamic_routerport_agingtime_set(yt_unit_t unit, uint32_t sec);

/**
 * @internal      yt_igmp_mld_dynamic_routerport_agingtime_get
 * @endinternal
 *
 * @brief         get dynamic router port aging time
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    psec                -second
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_igmp_mld_dynamic_routerport_agingtime_get(yt_unit_t unit, uint32_t *psec);

/**
 * @internal      yt_igmp_mld_static_routerport_set
 * @endinternal
 *
 * @brief         add static multicast router ports
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port_mask           -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_static_routerport_set(yt_unit_t unit, yt_port_mask_t port_mask);

/**
 * @internal      yt_igmp_mld_static_routerport_get
 * @endinternal
 *
 * @brief         get static multicast router ports
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pport_mask          -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_static_routerport_get(yt_unit_t unit, yt_port_mask_t *pport_mask);

/**
 * @internal      yt_igmp_mld_bypass_portIsolation_set
 * @endinternal
 *
 * @brief         enable igmp passthrough port isolation
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     en                  -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_igmp_mld_bypass_portIsolation_set(yt_unit_t unit, yt_port_t port, yt_enable_t en);

/**
 * @internal      yt_igmp_mld_bypass_portIsolation_get
 * @endinternal
 *
 * @brief         get enable state of igmp passthrough port isolation
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pen                 -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_igmp_mld_bypass_portIsolation_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pen);

/**
 * @internal      yt_igmp_mld_ipmc_bypass_portIsolation_set
 * @endinternal
 *
 * @brief         enable ip multicast passthrough port isolation
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     en                  -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_igmp_mld_ipmc_bypass_portIsolation_set(yt_unit_t unit, yt_enable_t en);

/**
 * @internal      yt_igmp_mld_ipmc_bypass_portIsolation_get
 * @endinternal
 *
 * @brief         get the enable state of ip multicast passthrough port isolation
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pen                 -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_igmp_mld_ipmc_bypass_portIsolation_get(yt_unit_t unit, yt_enable_t *pen);

/**
 * @internal      yt_igmp_mld_bypass_vlanIngrfilter_en_set
 * @endinternal
 *
 * @brief         Set port vlan igmp bypass ingress filter state
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enabled             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_bypass_vlanIngrfilter_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enabled);

/**
 * @internal      yt_igmp_mld_bypass_vlanIngrfilter_en_get
 * @endinternal
 *
 * @brief         Get port vlan igmp bypass ingress filter state
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnabled            -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_igmp_mld_bypass_vlanIngrfilter_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnabled);
#endif

#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)

/**
 * @internal      yt_igmp_mld_init
 * @endinternal
 *
 * @brief         Init igmp mld
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 */
extern yt_ret_t  yt_igmp_mld_init(yt_unit_t unit);

/**
 * @internal      yt_igmp_dynamicRouterPort_aging_time_set
 * @endinternal
 *
 * @brief         IGMP dynamic router port aging time set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     sec                 -aging time
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_dynamicRouterPort_aging_time_set(yt_unit_t unit,  uint32_t sec);

/**
 * @internal      yt_igmp_dynamicRouterPort_aging_time_get
 * @endinternal
 *
 * @brief         IGMP dynamic router port aging time get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[out]    pSec                -aging time
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_dynamicRouterPort_aging_time_get(yt_unit_t unit,  uint32_t *pSec);

/**
 * @internal      yt_igmp_dynamicRouterPort_get
 * @endinternal
 *
 * @brief         IGMP dynamic router port get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -dynamic group idx
 * @param[out]    pVid                -vid
 * @param[out]    pPort               -port id
 * @param[out]    pTime               -remaining aging time
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_dynamicRouterPort_get(yt_unit_t unit,  uint8_t idx, yt_vlan_t *pVid, yt_port_t *pPort, uint32_t *pTime);

/**
 * @internal      yt_igmp_forbidden_dynamicRouterPort_en_set
 * @endinternal
 *
 * @brief         forbid dynamic router port set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_forbidden_dynamicRouterPort_en_set(yt_unit_t unit,  yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_igmp_forbidden_dynamicRouterPort_en_get
 * @endinternal
 *
 * @brief         forbid dynamic router port get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_forbidden_dynamicRouterPort_en_get(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_igmp_staticRouterPort_add_by_idx
 * @endinternal
 *
 * @brief         add static router port by group idx
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -static group idx
 * @param[in]     vid                 -vid
 * @param[in]     portMask            -portMask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_staticRouterPort_add_by_idx(yt_unit_t unit,  uint8_t idx, yt_vlan_t vid, yt_port_mask_t portMask);

/**
 * @internal      yt_igmp_staticRouterPort_get
 * @endinternal
 *
 * @brief         static router port get by group idx
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -static group idx
 * @param[out]    pVid                -vid
 * @param[out]    pPortMask           -portMask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_staticRouterPort_get(yt_unit_t unit,  uint8_t idx, yt_vlan_t *pVid, yt_port_mask_t *pPortMask);

/**
 * @internal      yt_igmp_staticRouterPort_del
 * @endinternal
 *
 * @brief         static router port delete by group idx
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -static group idx
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_staticRouterPort_del(yt_unit_t unit,  uint8_t idx);

/**
 * @internal      yt_igmp_fast_leave_en_set
 * @endinternal
 *
 * @brief         fast leave enable set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id or lag group id
 * @param[in]     isLag               -lag group or not
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_fast_leave_en_set(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t enable);

/**
 * @internal      yt_igmp_fast_leave_en_get
 * @endinternal
 *
 * @brief         fast leave enable get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id or lag group id
 * @param[in]     isLag               -lag group or not
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_fast_leave_en_get(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t *pEnable);

/**
 * @internal      yt_igmp_report_leave_fwd_ctrl_set
 * @endinternal
 *
 * @brief         report and leave packet forward dest ports set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     version             -YT_IGMP_V1V2, YT_IGMP_V3
 * @param[in]     fwdCtrl             -YT_IGMP_MLD_FLOOD, YT_IGMP_MLD_ROUTER_PORTS
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_report_leave_fwd_ctrl_set(yt_unit_t unit, yt_igmp_version_t version, yt_igmp_mld_fwd_ports_t fwdCtrl);

/**
 * @internal      yt_igmp_report_leave_fwd_ctrl_get
 * @endinternal
 *
 * @brief         report and leave packet forward dest ports get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     version             -YT_IGMP_V1V2, YT_IGMP_V3
 * @param[out]    pFwdCtrl            -YT_IGMP_MLD_FLOOD, YT_IGMP_MLD_ROUTER_PORTS
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_report_leave_fwd_ctrl_get(yt_unit_t unit, yt_igmp_version_t version, yt_igmp_mld_fwd_ports_t *pFwdCtrl);

/**
 * @internal      yt_igmp_ctrlPkt_act_set
 * @endinternal
 *
 * @brief         IGMP packet action set
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id
 * @param[in]     actType             -ACT_TYPE_FWD, ACT_TYPE_DROP, ACT_TYPE_COPY, ACT_TYPE_TRAP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_ctrlPkt_act_set(yt_unit_t unit,  yt_port_t port, yt_act_type_t actType);

/**
 * @internal      yt_igmp_ctrlPkt_act_get
 * @endinternal
 *
 * @brief         IGMP packet action get
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id
 * @param[out]    pActType            -ACT_TYPE_FWD, ACT_TYPE_DROP, ACT_TYPE_COPY, ACT_TYPE_TRAP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_ctrlPkt_act_get(yt_unit_t unit,  yt_port_t port, yt_act_type_t *pActType);

/**
 * @internal      yt_igmp_v1v2_ctrlPkt_act_set
 * @endinternal
 *
 * @brief         IGMPv1v2 packet action set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     ctrlPkt             -YT_IGMP_V1V2_QUERY, YT_IGMP_V1V2_REPORT, YT_IGMP_V2_LEAVE
 * @param[in]     port                -port id
 * @param[in]     actType             -ACT_TYPE_FWD, ACT_TYPE_DROP, ACT_TYPE_COPY, ACT_TYPE_TRAP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_v1v2_ctrlPkt_act_set(yt_unit_t unit,  yt_igmp_v1v2_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t actType);

/**
 * @internal      yt_igmp_v1v2_ctrlPkt_act_get
 * @endinternal
 *
 * @brief         IGMPv1v2 packet action get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     ctrlPkt             -YT_IGMP_V1V2_QUERY, YT_IGMP_V1V2_REPORT, YT_IGMP_V2_LEAVE
 * @param[in]     port                -port id
 * @param[out]    pActType            -ACT_TYPE_FWD, ACT_TYPE_DROP, ACT_TYPE_COPY, ACT_TYPE_TRAP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_v1v2_ctrlPkt_act_get(yt_unit_t unit,  yt_igmp_v1v2_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t *pActType);

/**
 * @internal      yt_igmp_v3_ctrlPkt_act_set
 * @endinternal
 *
 * @brief         IGMPv3 packet action set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     ctrlPkt             -YT_IGMP_V3_QUERY, YT_IGMP_V3_REPORT
 * @param[in]     port                -port id
 * @param[in]     actType             -ACT_TYPE_FWD, ACT_TYPE_DROP, ACT_TYPE_COPY, ACT_TYPE_TRAP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_v3_ctrlPkt_act_set(yt_unit_t unit,  yt_igmp_v3_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t actType);

/**
 * @internal      yt_igmp_v3_ctrlPkt_act_get
 * @endinternal
 *
 * @brief         IGMPv3 packet action get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     ctrlPkt             -YT_IGMP_V3_QUERY, YT_IGMP_V3_REPORT
 * @param[in]     port                -port id
 * @param[out]    pActType            -ACT_TYPE_FWD, ACT_TYPE_DROP, ACT_TYPE_COPY, ACT_TYPE_TRAP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_v3_ctrlPkt_act_get(yt_unit_t unit,  yt_igmp_v3_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t *pActType);

/**
 * @internal      yt_igmp_v1v2_port_learn_en_set
 * @endinternal
 *
 * @brief         IGMPv1v2 learn enable set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id or lag group id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_v1v2_port_learn_en_set(yt_unit_t unit,  yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_igmp_v1v2_port_learn_en_get
 * @endinternal
 *
 * @brief         IGMPv1v2 learn enable get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id or lag group id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_v1v2_port_learn_en_get(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_igmp_v3_port_learn_en_set
 * @endinternal
 *
 * @brief         IGMPv3 learn enable set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id or lag group id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_v3_port_learn_en_set(yt_unit_t unit,  yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_igmp_v3_port_learn_en_get
 * @endinternal
 *
 * @brief         IGMPv3 learn enable get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id or lag group id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_v3_port_learn_en_get(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_igmp_routerPort_dataFwd_en_set
 * @endinternal
 *
 * @brief         data forward router port or not set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_routerPort_dataFwd_en_set(yt_unit_t unit, yt_enable_t enable);

/**
 * @internal      yt_igmp_routerPort_dataFwd_en_get
 * @endinternal
 *
 * @brief         data forward router port or not get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_routerPort_dataFwd_en_get(yt_unit_t unit, yt_enable_t *pEnable);

/**
 * @internal      yt_mld_dynamicRouterPort_aging_time_set
 * @endinternal
 *
 * @brief         MLD dynamic router port aging time set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     sec                 -aging time
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_dynamicRouterPort_aging_time_set(yt_unit_t unit,  uint32_t sec);

/**
 * @internal      yt_mld_dynamicRouterPort_aging_time_get
 * @endinternal
 *
 * @brief         MLD dynamic router port aging time get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[out]    pSec                -aging time
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_dynamicRouterPort_aging_time_get(yt_unit_t unit,  uint32_t *pSec);

/**
 * @internal      yt_mld_dynamicRouterPort_get
 * @endinternal
 *
 * @brief         MLD dynamic router port get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -dynamic group idx
 * @param[out]    pVid                -vid
 * @param[out]    pPort               -port id
 * @param[out]    pTime               -remaining aging time
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_dynamicRouterPort_get(yt_unit_t unit,  uint8_t idx, yt_vlan_t *pVid, yt_port_t *pPort, uint32_t *pTime);

/**
 * @internal      yt_mld_forbidden_dynamicRouterPort_en_set
 * @endinternal
 *
 * @brief         forbid dynamic router port set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_forbidden_dynamicRouterPort_en_set(yt_unit_t unit,  yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_mld_forbidden_dynamicRouterPort_en_get
 * @endinternal
 *
 * @brief         forbid dynamic router port get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_forbidden_dynamicRouterPort_en_get(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_mld_staticRouterPort_add_by_idx
 * @endinternal
 *
 * @brief         add static router port by group idx
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -static group idx
 * @param[in]     vid                 -vid
 * @param[in]     portMask            -portMask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_staticRouterPort_add_by_idx(yt_unit_t unit,  uint8_t idx, yt_vlan_t vid, yt_port_mask_t portMask);

/**
 * @internal      yt_mld_staticRouterPort_get
 * @endinternal
 *
 * @brief         static router port get by group idx
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -static group idx
 * @param[out]    pVid                -vid
 * @param[out]    pPortMask           -portMask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_staticRouterPort_get(yt_unit_t unit,  uint8_t idx, yt_vlan_t *pVid, yt_port_mask_t *pPortMask);

/**
 * @internal      yt_mld_staticRouterPort_del
 * @endinternal
 *
 * @brief         static router port delete by group idx
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -static group idx
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_staticRouterPort_del(yt_unit_t unit,  uint8_t idx);

/**
 * @internal      yt_mld_fast_leave_en_set
 * @endinternal
 *
 * @brief         fast leave enable set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id or lag group id
 * @param[in]     isLag               -lag group or not
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_fast_leave_en_set(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t enable);

/**
 * @internal      yt_mld_fast_leave_en_get
 * @endinternal
 *
 * @brief         fast leave enable get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id or lag group id
 * @param[in]     isLag               -lag group or not
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_fast_leave_en_get(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t *pEnable);

/**
 * @internal      yt_mld_report_leave_fwd_ctrl_set
 * @endinternal
 *
 * @brief         report and leave packet forward dest ports set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     version             -YT_MLD_V1, YT_MLD_V2
 * @param[in]     fwdCtrl             -YT_IGMP_MLD_FLOOD, YT_IGMP_MLD_ROUTER_PORTS
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_report_leave_fwd_ctrl_set(yt_unit_t unit, yt_mld_version_t version, yt_igmp_mld_fwd_ports_t fwdCtrl);

/**
 * @internal      yt_mld_report_leave_fwd_ctrl_get
 * @endinternal
 *
 * @brief         report and leave packet forward dest ports get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     version             -YT_MLD_V1, YT_MLD_V2
 * @param[out]    pFwdCtrl            -YT_IGMP_MLD_FLOOD, YT_IGMP_MLD_ROUTER_PORTS
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_report_leave_fwd_ctrl_get(yt_unit_t unit, yt_mld_version_t version, yt_igmp_mld_fwd_ports_t *pFwdCtrl);

/**
 * @internal      yt_mld_ctrlPkt_act_set
 * @endinternal
 *
 * @brief         MLD packet action set
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id
 * @param[in]     actType             -ACT_TYPE_FWD, ACT_TYPE_DROP, ACT_TYPE_COPY, ACT_TYPE_TRAP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_ctrlPkt_act_set(yt_unit_t unit,  yt_port_t port, yt_act_type_t actType);

/**
 * @internal      yt_mld_ctrlPkt_act_get
 * @endinternal
 *
 * @brief         MLD packet action get
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id
 * @param[out]    pActType            -ACT_TYPE_FWD, ACT_TYPE_DROP, ACT_TYPE_COPY, ACT_TYPE_TRAP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_ctrlPkt_act_get(yt_unit_t unit,  yt_port_t port, yt_act_type_t *pActType);

/**
 * @internal      yt_mld_v1_ctrlPkt_act_set
 * @endinternal
 *
 * @brief         MLDv1 packet action set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     ctrlPkt             -YT_MLD_V1_QUERY, YT_MLD_V1_REPORT, YT_MLD_V1_LEAVE
 * @param[in]     port                -port id
 * @param[in]     actType             -ACT_TYPE_FWD, ACT_TYPE_DROP, ACT_TYPE_COPY, ACT_TYPE_TRAP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_v1_ctrlPkt_act_set(yt_unit_t unit,  yt_mld_v1_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t actType);

/**
 * @internal      yt_mld_v1_ctrlPkt_act_get
 * @endinternal
 *
 * @brief         MLDv1 packet action get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     ctrlPkt             -YT_MLD_V1_QUERY, YT_MLD_V1_REPORT, YT_MLD_V1_LEAVE
 * @param[in]     port                -port id
 * @param[out]    pActType            -ACT_TYPE_FWD, ACT_TYPE_DROP, ACT_TYPE_COPY, ACT_TYPE_TRAP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_v1_ctrlPkt_act_get(yt_unit_t unit,  yt_mld_v1_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t *pActType);

/**
 * @internal      yt_mld_v2_ctrlPkt_act_set
 * @endinternal
 *
 * @brief         MLDv2 packet action set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     ctrlPkt             -YT_MLD_V2_QUERY, YT_MLD_V2_REPORT
 * @param[in]     port                -port id
 * @param[in]     actType             -ACT_TYPE_FWD, ACT_TYPE_DROP, ACT_TYPE_COPY, ACT_TYPE_TRAP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_v2_ctrlPkt_act_set(yt_unit_t unit,  yt_mld_v2_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t actType);

/**
 * @internal      yt_mld_v2_ctrlPkt_act_get
 * @endinternal
 *
 * @brief         MLDv2 packet action get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     ctrlPkt             -YT_MLD_V2_QUERY, YT_MLD_V2_REPORT
 * @param[in]     port                -port id
 * @param[out]    pActType            -ACT_TYPE_FWD, ACT_TYPE_DROP, ACT_TYPE_COPY, ACT_TYPE_TRAP
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_v2_ctrlPkt_act_get(yt_unit_t unit,  yt_mld_v2_ctrlPkt_type_t ctrlPkt, yt_port_t port, yt_act_type_t *pActType);

/**
 * @internal      yt_mld_v1_port_learn_en_set
 * @endinternal
 *
 * @brief         MLDv1 learn enable set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id or lag group id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_v1_port_learn_en_set(yt_unit_t unit,  yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_mld_v1_port_learn_en_get
 * @endinternal
 *
 * @brief         MLDv1 learn enable get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id or lag group id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_v1_port_learn_en_get(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_mld_v2_port_learn_en_set
 * @endinternal
 *
 * @brief         MLDv2 learn enable set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id or lag group id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_v2_port_learn_en_set(yt_unit_t unit,  yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_mld_v2_port_learn_en_get
 * @endinternal
 *
 * @brief         MLDv2 learn enable get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id or lag group id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_v2_port_learn_en_get(yt_unit_t unit,  yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_mld_routerPort_dataFwd_en_set
 * @endinternal
 *
 * @brief         data forward router port or not set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_routerPort_dataFwd_en_set(yt_unit_t unit, yt_enable_t enable);

/**
 * @internal      yt_mld_routerPort_dataFwd_en_get
 * @endinternal
 *
 * @brief         data forward router port or not get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_mld_routerPort_dataFwd_en_get(yt_unit_t unit, yt_enable_t *pEnable);

/**
 * @internal      yt_igmp_mld_system_learnlimit_set
 * @endinternal
 *
 * @brief         IGMP and MLD group learn limit set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @param[in]     maxNum              -learn limit number
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_mld_system_learnlimit_set(yt_unit_t unit, yt_enable_t enable, uint16_t maxNum);

/**
 * @internal      yt_igmp_mld_system_learnlimit_get
 * @endinternal
 *
 * @brief         IGMP and MLD group learn limit get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @param[out]    pMaxNum             -learn limit number
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_mld_system_learnlimit_get(yt_unit_t unit, yt_enable_t *pEnable, uint16_t *pMaxNum);

/**
 * @internal      yt_igmp_mld_system_learnCnt_get
 * @endinternal
 *
 * @brief         IGMP and MLD group learn count get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[out]    pCnt                -count
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_mld_system_learnCnt_get(yt_unit_t unit, uint16_t *pCnt);

/**
 * @internal      yt_igmp_mld_port_learnlimit_set
 * @endinternal
 *
 * @brief         IGMP and MLD group learn limit set by per port
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id or lag group id
 * @param[in]     isLag               -lag group or not
 * @param[in]     enable              -enable or disable
 * @param[in]     maxNum              -learn limit number
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_mld_port_learnlimit_set(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t enable, uint16_t maxNum);

/**
 * @internal      yt_igmp_mld_port_learnlimit_get
 * @endinternal
 *
 * @brief         IGMP and MLD group learn limit get by per port
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id or lag group id
 * @param[in]     isLag               -lag group or not
 * @param[out]    pEnable             -enable or disable
 * @param[out]    pMaxNum             -learn limit number
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_mld_port_learnlimit_get(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, yt_enable_t *pEnable, uint16_t *pMaxNum);

/**
 * @internal      yt_igmp_mld_port_learnCnt_get
 * @endinternal
 *
 * @brief         IGMP and MLD group learn count get by per port
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port id or lag group id
 * @param[in]     isLag               -lag group or not
 * @param[out]    pCnt                -count
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_mld_port_learnCnt_get(yt_unit_t unit, yt_port_t port, yt_bool_t isLag, uint16_t *pCnt);

/**
 * @internal      yt_igmp_mld_memberports_aging_time_set
 * @endinternal
 *
 * @brief         IGMP and MLD member port aging time set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     sec                 -aging time
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_mld_memberports_aging_time_set(yt_unit_t unit,  uint32_t sec);

/**
 * @internal      yt_igmp_mld_memberports_aging_time_get
 * @endinternal
 *
 * @brief         IGMP and MLD member port aging time get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[out]    pSec                -aging time
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_mld_memberports_aging_time_get(yt_unit_t unit,  uint32_t *pSec);

/**
 * @internal      yt_igmp_mld_memberport_time_get
 * @endinternal
 *
 * @brief         IGMP and MLD member port remaining aging time get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -group id
 * @param[in]     port                -port id
 * @param[out]    pTime               -remaining aging time
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_mld_memberport_time_get(yt_unit_t unit,  uint8_t idx, yt_port_t port, uint32_t *pTime);

/**
 * @internal      yt_igmp_mld_leaveports_aging_time_set
 * @endinternal
 *
 * @brief         IGMP and MLD last member port aging time set
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     agingEnum           -YT_IGMP_MLD_2S, YT_IGMP_MLD_4S, YT_IGMP_MLD_8S, YT_IGMP_MLD_10S
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_mld_leaveports_aging_time_set(yt_unit_t unit,  yt_igmp_mld_leave_aging_t agingEnum);

/**
 * @internal      yt_igmp_mld_leaveports_aging_time_get
 * @endinternal
 *
 * @brief         IGMP and MLD last member port aging time get
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[out]    pAgingEnum          -YT_IGMP_MLD_2S, YT_IGMP_MLD_4S, YT_IGMP_MLD_8S, YT_IGMP_MLD_10S
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_igmp_mld_leaveports_aging_time_get(yt_unit_t unit,  yt_igmp_mld_leave_aging_t *pAgingEnum);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
