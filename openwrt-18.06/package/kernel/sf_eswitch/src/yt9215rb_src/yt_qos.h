/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_qos.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_QOS_H
#define __YT_QOS_H
#include "yt_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define YT_QOS_DWRR_MAX_WEIGHT         (256)
#define YT_QOS_MAX_QUEUE_NUM            (8)

#ifdef SWITCH_SERIES_TIGER
#define YT_QOS_QUEUE_MAX_PRI           (15)
#else
#define YT_QOS_QUEUE_MAX_PRI           (7)
#endif

typedef enum yt_pri_index_e
{
    YT_CPRI_MAP_INDEX,
    YT_SPRI_MAP_INDEX
}yt_pri_index_t;

typedef enum yt_remark_index_e
{
    YT_CPRI_REMARK_INDEX,
    YT_SPRI_REMARK_INDEX
}yt_remark_index_t;

typedef enum yt_dp_e
{
    YT_DROP_PRIO_GREEN = 0,
    YT_DROP_PRIO_YELLOW,
    YT_DROP_PRIO_RED
}yt_dp_t;

#define YT_QOS_SEL_PRECEDENCE_FLAG_INTCPRI      BIT(0)
#define YT_QOS_SEL_PRECEDENCE_FLAG_INTSPRI      BIT(1)
#define YT_QOS_SEL_PRECEDENCE_FLAG_SMAC         BIT(2)
#define YT_QOS_SEL_PRECEDENCE_FLAG_DMAC         BIT(3)
#define YT_QOS_SEL_PRECEDENCE_FLAG_VLAN         BIT(4)
#define YT_QOS_SEL_PRECEDENCE_FLAG_ACL          BIT(5)
#define YT_QOS_SEL_PRECEDENCE_FLAG_DSCP         BIT(6)
#define YT_QOS_SEL_PRECEDENCE_FLAG_CPRI         BIT(7)
#define YT_QOS_SEL_PRECEDENCE_FLAG_SPRI         BIT(8)
#define YT_QOS_SEL_PRECEDENCE_FLAG_PORT         BIT(9)
#ifdef SWITCH_SERIES_TIGER
#define YT_QOS_SEL_PRECEDENCE_FLAG_ALL  (YT_QOS_SEL_PRECEDENCE_FLAG_SMAC | YT_QOS_SEL_PRECEDENCE_FLAG_DMAC \
                                        | YT_QOS_SEL_PRECEDENCE_FLAG_VLAN | YT_QOS_SEL_PRECEDENCE_FLAG_ACL \
                                        | YT_QOS_SEL_PRECEDENCE_FLAG_DSCP | YT_QOS_SEL_PRECEDENCE_FLAG_CPRI \
                                        | YT_QOS_SEL_PRECEDENCE_FLAG_SPRI | YT_QOS_SEL_PRECEDENCE_FLAG_PORT)
#else
#define YT_QOS_SEL_PRECEDENCE_FLAG_ALL  (YT_QOS_SEL_PRECEDENCE_FLAG_INTCPRI | YT_QOS_SEL_PRECEDENCE_FLAG_INTSPRI \
                                        | YT_QOS_SEL_PRECEDENCE_FLAG_DSCP | YT_QOS_SEL_PRECEDENCE_FLAG_ACL \
                                        | YT_QOS_SEL_PRECEDENCE_FLAG_SPRI | YT_QOS_SEL_PRECEDENCE_FLAG_CPRI \
                                        | YT_QOS_SEL_PRECEDENCE_FLAG_PORT)
#endif

typedef struct yt_qos_intPri_sel_precedence_s
{
    uint32_t flags;
    /* the precedence of internal 802.1p priority map to internal priority */
    uint8_t    intCpri;
    /* the precedence of internal 802.1ad priority map to internal priority */
    uint8_t    intSpri;
    uint8_t    smacPri;
    uint8_t    dmacPri;
    uint8_t    vlanPri;
    uint8_t    aclPri;
    uint8_t    dscpPri;
    /* the precedence of packet's 802.1p priority map to internal priority */
    uint8_t    cpri;
    /* the precedence of packet's 802.1ad priority map to internal priority */
    uint8_t    spri;
    uint8_t    portPri;
}yt_qos_intPri_sel_precedence_t;

typedef struct yt_qos_pmap_tci_s
{
    uint8_t    dei;
    uint8_t    prio;
}yt_qos_pmap_tci_t;

#define YT_QOS_QMAP_FLAG_PRI0_QID         BIT(0)
#define YT_QOS_QMAP_FLAG_PRI1_QID         BIT(1)
#define YT_QOS_QMAP_FLAG_PRI2_QID         BIT(2)
#define YT_QOS_QMAP_FLAG_PRI3_QID         BIT(3)
#define YT_QOS_QMAP_FLAG_PRI4_QID         BIT(4)
#define YT_QOS_QMAP_FLAG_PRI5_QID         BIT(5)
#define YT_QOS_QMAP_FLAG_PRI6_QID         BIT(6)
#define YT_QOS_QMAP_FLAG_PRI7_QID         BIT(7)
#define YT_QOS_QMAP_FLAG_ALL              (YT_QOS_QMAP_FLAG_PRI0_QID | YT_QOS_QMAP_FLAG_PRI1_QID \
                                          | YT_QOS_QMAP_FLAG_PRI2_QID | YT_QOS_QMAP_FLAG_PRI3_QID \
                                          | YT_QOS_QMAP_FLAG_PRI4_QID | YT_QOS_QMAP_FLAG_PRI5_QID \
                                          | YT_QOS_QMAP_FLAG_PRI6_QID | YT_QOS_QMAP_FLAG_PRI7_QID)
typedef struct yt_qos_qmap_s
{
    uint32_t flags;
    uint8_t qid[YT_QOS_MAX_QUEUE_NUM];
}yt_qos_qmap_t;

typedef enum yt_qos_remark_src_type_e
{
    YT_REMARK_SRC_TYPE_INT_PRI,
    YT_REMARK_SRC_TYPE_PKT_PRI
}yt_qos_remark_src_type_t;

#define YT_QOS_REMARK_FLAG_CPRI         BIT(0)
#define YT_QOS_REMARK_FLAG_CPRITYPE     BIT(1)
#define YT_QOS_REMARK_FLAG_CFI          BIT(2)
#define YT_QOS_REMARK_FLAG_SPRI         BIT(3)
#define YT_QOS_REMARK_FLAG_SPRITYPE     BIT(4)
#define YT_QOS_REMARK_FLAG_SDEI         BIT(5)
#define YT_QOS_REMARK_FLAG_DSCP         BIT(6)
#ifdef SWITCH_SERIES_TIGER
#define YT_QOS_REMARK_FLAG_ALL          (YT_QOS_REMARK_FLAG_CPRI | YT_QOS_REMARK_FLAG_CFI \
                                        | YT_QOS_REMARK_FLAG_SPRI | YT_QOS_REMARK_FLAG_SDEI \
                                        | YT_QOS_REMARK_FLAG_DSCP)
#else
#define YT_QOS_REMARK_FLAG_ALL          (YT_QOS_REMARK_FLAG_CPRI | YT_QOS_REMARK_FLAG_CPRITYPE \
                                        | YT_QOS_REMARK_FLAG_CFI | YT_QOS_REMARK_FLAG_SPRI \
                                        | YT_QOS_REMARK_FLAG_SPRITYPE | YT_QOS_REMARK_FLAG_SDEI \
                                        | YT_QOS_REMARK_FLAG_DSCP)
#endif

typedef struct yt_qos_remark_ctrl_s
{
    uint32_t flags;
    yt_qos_remark_src_type_t cpriSrcType;
    yt_qos_remark_src_type_t spriSrcType;
    uint8_t    spriEn;
    uint8_t    sdeiEn;
    uint8_t    cpriEn;
    uint8_t    ccfiEn;
    uint8_t    dscpEn;
}yt_qos_remark_ctrl_t;

typedef struct yt_qos_remark_info_s
{
    yt_dp_t     dp;
    yt_pri_t    prio;
}yt_qos_remark_info_t;

/**
 * @internal      yt_qos_init
 * @endinternal
 *
 * @brief         Init qos module
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_init(yt_unit_t unit);

/**
 * @internal      yt_qos_intPri_sel_precedence_set
 * @endinternal
 *
 * @brief         Config the priority between different priority mechanism.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pPriTbl             -internal priority select, The higher the value, the higher the priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intPri_sel_precedence_set(yt_unit_t unit, yt_port_t port, yt_qos_intPri_sel_precedence_t *pPriTbl);


/**
 * @internal      yt_qos_intPri_sel_precedence_get
 * @endinternal
 *
 * @brief         Get the priority configuration between different priority mechanism.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pPriTbl             -internal priority select, The higher the value, the higher the priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intPri_sel_precedence_get(yt_unit_t unit, yt_port_t port, yt_qos_intPri_sel_precedence_t *pPriTbl);

/**
 * @internal      yt_qos_intPri_portDefPri_set
 * @endinternal
 *
 * @brief         Map port to internal priority.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intPri_portDefPri_set (yt_unit_t unit, yt_port_t port, yt_enable_t enable, yt_pri_t pri);


/**
 * @internal      yt_qos_intPri_portDefPri_get 
 * @endinternal
 *
 * @brief         Get port to internal priority.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_qos_intPri_portDefPri_get (yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable, yt_pri_t *pPri);


/**
 * @internal      yt_qos_intPri_intCpri_map_set
 * @endinternal
 *
 * @brief         Map internal 802.1p priority to internal priority
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     cpri                -internal vlan priority (0 - MAX_PRIORITY)
 * @param[in]     pri                 -internal priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intPri_intCpri_map_set(yt_unit_t unit, yt_pri_t cpri, yt_pri_t pri);


/**
 * @internal      yt_qos_intPri_intCpri_map_get
 * @endinternal
 *
 * @brief         Get internal 802.1p priority to internal priority
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     cpri                -internal vlan priority (0 - MAX_PRIORITY)
 * @param[out]    pPri                -internal priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intPri_intCpri_map_get(yt_unit_t unit, yt_pri_t cpri, yt_pri_t *pPri);

/**
 * @internal      yt_qos_intPri_intSpri_map_set
 * @endinternal
 *
 * @brief         Map internal 802.1ad priority to internal priority
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     spri                -internal vlan priority (0 - MAX_PRIORITY)
 * @param[in]     pri                 -internal priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intPri_intSpri_map_set(yt_unit_t unit, yt_pri_t spri, yt_pri_t pri);

/**
 * @internal      yt_qos_intPri_intSpri_map_get
 * @endinternal
 *
 * @brief         Get internal 802.1ad priority to internal priority
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     spri                -internal vlan priority (0 - MAX_PRIORITY)
 * @param[out]    pPri                -internal priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intPri_intSpri_map_get(yt_unit_t unit, yt_pri_t spri, yt_pri_t *pPri);

/**
 * @internal      yt_qos_intPri_cpri_map_set
 * @endinternal
 *
 * @brief         Map 802.1p priority value to internal priority.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     cpri                -tag control information exclude vid
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intPri_cpri_map_set(yt_unit_t unit, yt_qos_pmap_tci_t cpri, yt_pri_t pri);

/**
 * @internal      yt_qos_intPri_cpri_map_get
 * @endinternal
 *
 * @brief         Get 802.1p priority value to internal priority.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     cpri                -tag control information exclude vid
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intPri_cpri_map_get(yt_unit_t unit, yt_qos_pmap_tci_t cpri, yt_pri_t *pPri);


/**
 * @internal      yt_qos_intPri_spri_map_set
 * @endinternal
 *
 * @brief         Map 802.1ad priority value to internal priority.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     spri                -tag control information exclude vid
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intPri_spri_map_set(yt_unit_t unit, yt_qos_pmap_tci_t spri, yt_pri_t pri);


/**
 * @internal      yt_qos_intPri_spri_map_get
 * @endinternal
 *
 * @brief         Get 802.1ad priority value to internal priority.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     spri                -tag control information exclude vid
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intPri_spri_map_get(yt_unit_t unit, yt_qos_pmap_tci_t spri, yt_pri_t *pPri);

/**
 * @internal      yt_qos_intPri_dscp_map_set
 * @endinternal
 *
 * @brief         Map dscp value to internal priority.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     dscp                -the dscp value (0 - MAX_DSCP_VALUE)
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intPri_dscp_map_set(yt_unit_t unit, yt_dscp_t dscp, yt_pri_t pri);

/**
 * @internal      yt_qos_intPri_dscp_map_get
 * @endinternal
 *
 * @brief         Get dscp value to internal priority.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     dscp                -the dscp value (0 - MAX_DSCP_VALUE)
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intPri_dscp_map_get(yt_unit_t unit, yt_dscp_t dscp, yt_pri_t *pPri);

/**
 * @internal      yt_qos_intPri_vlan_map_set
 * @endinternal
 *
 * @brief         Set vlan mapping internal priority and state
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     enable              -enable or disable
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_qos_intPri_vlan_map_set(yt_unit_t unit, yt_vlan_t vid, yt_enable_t enable, yt_pri_t pri);

/**
 * @internal      yt_qos_intPri_vlan_map_get
 * @endinternal
 *
 * @brief         Get vlan mapping internal priority and state
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[out]    pEnable             -enable or disable
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_qos_intPri_vlan_map_get(yt_unit_t unit, yt_vlan_t vid, yt_enable_t *pEnable, yt_pri_t *pPri);

/**
 * @internal      yt_qos_intPri_map_igrMirror_set
 * @endinternal
 *
 * @brief         Config ingress mirror to internal priority and state.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     grpId               -mirror group id
 * @param[in]     enable              -enable or disable
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intPri_map_igrMirror_set(yt_unit_t unit, yt_mirror_group_t grpId, yt_enable_t enable, yt_pri_t pri);


/**
 * @internal      yt_qos_intPri_map_igrMirror_get
 * @endinternal
 *
 * @brief         Get ingress mirror to internal priority and state.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     grpId               -mirror group id
 * @param[out]    pEnable             -enable or disable
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intPri_map_igrMirror_get(yt_unit_t unit, yt_mirror_group_t grpId, yt_enable_t *pEnable, yt_pri_t *pPri);


/**
 * @internal      yt_qos_intPri_map_egrMirror_set
 * @endinternal
 *
 * @brief         Config engress mirror to internal priority and state.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     grpId               -mirror group id
 * @param[in]     enable              -enable or disable
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intPri_map_egrMirror_set(yt_unit_t unit, yt_mirror_group_t grpId, yt_enable_t enable, yt_pri_t pri);


/**
 * @internal      yt_qos_intPri_map_egrMirror_get
 * @endinternal
 *
 * @brief         Get engress mirror to internal priority and state.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     grpId               -mirror group id
 * @param[out]    pEnable             -enable or disable
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intPri_map_egrMirror_get(yt_unit_t unit, yt_mirror_group_t grpId, yt_enable_t *pEnable, yt_pri_t *pPri);


/**
 * @internal      yt_qos_intDP_cpri_map_set
 * @endinternal
 *
 * @brief         Map 802.1p priority value to internal drop precedent
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     cpri                -tag control information exclude vid
 * @param[in]     dp                  -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intDP_cpri_map_set(yt_unit_t unit, yt_qos_pmap_tci_t cpri, yt_dp_t dp);

/**
 * @internal      yt_qos_intDP_cpri_map_get
 * @endinternal
 *
 * @brief         Get 802.1p priority value to internal drop precedent
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     cpri                -as a key value that maps to int_pri and int_dp
 * @param[out]    pDp                 -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intDP_cpri_map_get(yt_unit_t unit, yt_qos_pmap_tci_t cpri, yt_dp_t *pDp);

/**
 * @internal      yt_qos_intDP_spri_map_set
 * @endinternal
 *
 * @brief         Map 802.1ad priority value to internal drop precedent.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     spri                -tag control information exclude vid
 * @param[in]     dp                  -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intDP_spri_map_set(yt_unit_t unit, yt_qos_pmap_tci_t spri, yt_dp_t dp);

/**
 * @internal      yt_qos_intDP_spri_map_get
 * @endinternal
 *
 * @brief         Get 802.1ad priority value to internal drop precedent.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     spri                -tag control information exclude vid
 * @param[out]    pDp                 -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intDP_spri_map_get(yt_unit_t unit, yt_qos_pmap_tci_t spri, yt_dp_t *pDp);

/**
 * @internal      yt_qos_intDP_dscp_map_set
 * @endinternal
 *
 * @brief         Map dscp value to internal drop precedent.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     dscp                -the dscp value (0 - MAX_DSCP_VALUE)
 * @param[in]     dp                  -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intDP_dscp_map_set(yt_unit_t unit, yt_dscp_t dscp, yt_dp_t dp);


/**
 * @internal      yt_qos_intDP_dscp_map_get
 * @endinternal
 *
 * @brief         Get dscp value to internal drop precedent.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     dscp                -the dscp value (0 - MAX_DSCP_VALUE)
 * @param[out]    pDp                 -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_intDP_dscp_map_get(yt_unit_t unit, yt_dscp_t dscp, yt_dp_t *pDp);

/**
 * @internal      yt_qos_que_map_ucast_set
 * @endinternal
 *
 * @brief         Internal priority to ucast queue mapping about port.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pQmap               -internal priority to queue mapping,qid range(0~7)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_que_map_ucast_set(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *pQmap);


/**
 * @internal      yt_qos_que_map_ucast_get
 * @endinternal
 *
 * @brief         Get internal priority to ucast queue mapping about port.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pQmap               -internal priority to queue mapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_que_map_ucast_get(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *pQmap);


/**
 * @internal      yt_qos_que_map_mcast_set
 * @endinternal
 *
 * @brief         Internal priority to mcast queue mapping about port.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pQmap               -internal priority to queue mapping,qid range(0~3)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_que_map_mcast_set(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *pQmap);


/**
 * @internal      yt_qos_que_map_mcast_get
 * @endinternal
 *
 * @brief         Get internal priority to mcast queue mapping about port.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pQmap               -internal priority to queue mapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_que_map_mcast_get(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *pQmap);

/**
 * @internal      yt_qos_que_forceDrop_enable_set
 * @endinternal
 *
 * @brief         Config force drop for port.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_que_forceDrop_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      yt_qos_que_forceDrop_enable_get
 * @endinternal
 *
 * @brief         Get force drop for port.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_que_forceDrop_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_qos_remark_port_set
 * @endinternal
 *
 * @brief         Config port remark state
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pRemarkCtrl            -remark states of different type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_remark_port_set(yt_unit_t unit, yt_port_t port, const yt_qos_remark_ctrl_t *pRemarkCtrl);


/**
 * @internal      yt_qos_remark_port_get
 * @endinternal
 *
 * @brief         Get port remark state
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pRemarkCtrl           -remark states of different type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_remark_port_get(yt_unit_t unit, yt_port_t port, yt_qos_remark_ctrl_t *pRemarkCtrl);


/**
 * @internal      yt_qos_remark_dscp_set
 * @endinternal
 *
 * @brief         Config internal priority and drop precedent to dscp mapping
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -internal priority and drop precedent
 * @param[in]     newDscp             -the dscp value (0 - MAX_DSCP_VALUE)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_remark_dscp_set(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_dscp_t newDscp);


/**
 * @internal      yt_qos_remark_dscp_get
 * @endinternal
 *
 * @brief         Get internal priority and drop precedent to dscp mapping
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -internal priority and drop precedent
 * @param[out]    pNewDscp            -the dscp value (0 - MAX_DSCP_VALUE)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_remark_dscp_get(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_dscp_t *pNewDscp);


/**
 * @internal      yt_qos_remark_cpri_set
 * @endinternal
 *
 * @brief         Config cvlan internal priority and drop precedent to priority and cfi mapping
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -as a key value that remark to priority and dscp
 * @param[in]     dstInfo             -tag control information exclude vid
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_remark_cpri_set(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t dstInfo);


/**
 * @internal      yt_qos_remark_cpri_get
 * @endinternal
 *
 * @brief         Get cvlan internal priority and drop precedent to priority and cfi mapping
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -as a key value that remark to priority and dscp
 * @param[out]    pDstInfo            -tag control information exclude vid
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_remark_cpri_get(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t *pDstInfo);

/**
 * @internal      yt_qos_remark_spri_set
 * @endinternal
 *
 * @brief         Config svlan internal priority and drop precedent to priority and cfi mapping
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -as a key value that remark to priority and dscp
 * @param[in]     dstInfo             -tag control information exclude vid
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_remark_spri_set(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t dstInfo);


/**
 * @internal      yt_qos_remark_spri_get
 * @endinternal
 *
 * @brief         Get svlan internal priority and drop precedent to priority and cfi mapping
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -as a key value that remark to priority and dscp
 * @param[out]    pDstInfo            -tag control information exclude vid
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_remark_spri_get(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t *pDstInfo);


/**
 * @internal      yt_qos_schedule_queue_pri_set
 * @endinternal
 *
 * @brief         Set the queue priority
 *                use DWRR scheduling when the queue priorities are the same
 *                use SP scheduling when the queue priorities are different
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[in]     qpri                -queue priority (0 - MAX_QUEUE_ID)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_schedule_queue_pri_set(yt_unit_t unit, yt_qid_t qinfo, yt_queue_pri_t qpri);


/**
 * @internal      yt_qos_schedule_queue_pri_get
 * @endinternal
 *
 * @brief         Get the queue priority
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[out]    pQpri               -queue priority (0 - MAX_QUEUE_ID)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_schedule_queue_pri_get(yt_unit_t unit, yt_qid_t qinfo, yt_queue_pri_t *pQpri);


/**
 * @internal      yt_qos_schedule_dwrr_mode_set
 * @endinternal
 *
 * @brief         Config queue schedule dwrr mode(bps or pps)
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[in]     dwrrMode            -bbs and pps
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_schedule_dwrr_mode_set(yt_unit_t unit, yt_qid_t qinfo, yt_rate_mode_t dwrrMode);


/**
 * @internal      yt_qos_schedule_dwrr_mode_get
 * @endinternal
 *
 * @brief         Get queue schedule dwrr mode weight(bps or pps)
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[out]    pDwrrMode           -bbs and pps
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_schedule_dwrr_mode_get(yt_unit_t unit, yt_qid_t qinfo, yt_rate_mode_t *pDwrrMode);


/**
 * @internal      yt_qos_schedule_dwrr_weight_set
 * @endinternal
 *
 * @brief         Config queue schedule dwrr mode weight
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[in]     qweight             -queue weight in dwrr mode,(1~0x3FFF)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_schedule_dwrr_weight_set(yt_unit_t unit, yt_qid_t qinfo, yt_queue_weight_t qweight);


/**
 * @internal      yt_qos_schedule_dwrr_weight_get
 * @endinternal
 *
 * @brief         Get queue schedule dwrr mode weight
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[out]    pQweight            -queue weight in dwrr mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_qos_schedule_dwrr_weight_get(yt_unit_t unit, yt_qid_t qinfo, yt_queue_weight_t *pQweight);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
