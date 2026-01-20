/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_qos.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_QOS_H
#define __FAL_TIGER_QOS_H


#include "fal_cmm.h"
#include "yt_qos.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DONE_CHECK_NUMBER                           1000
#define FAL_PER_PORT_MAX_QUEUE_PRI(unit)            (CAL_MAX_UCAST_QUEUE_NUM(unit) + CAL_MAX_MCAST_QUEUE_NUM(unit) - 1)
#define QOS_FORCEAC_UCASTQUE_REG(unit, macid, qid)  (0x301000 + ((macid) * CAL_MAX_UCAST_QUEUE_NUM(unit) * 8) + ((qid) * 8))
#define QOS_FORCEAC_MCASTQUE_REG(unit, macid, qid)  (0x302000 + ((macid) * CAL_MAX_MCAST_QUEUE_NUM(unit) * 4) + ((qid) * 4))
#define QOS_GROUPAC_REG(idx)                        (0x303000 + ((idx) * 4))
#define QOS_GROUPAC_MAX_NUM                         (10)

/**
 * @internal      fal_tiger_qos_init
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_init(yt_unit_t unit);

/**
 * @internal      fal_tiger_qos_intPri_sel_precedence_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pPriTbl             -internal priority select, The higher the value, the higher the priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intPri_sel_precedence_set(yt_unit_t unit, yt_port_t port, yt_qos_intPri_sel_precedence_t *pPriTbl);


/**
 * @internal      fal_tiger_qos_intPri_sel_precedence_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pPriTbl             -internal priority select, The higher the value, the higher the priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intPri_sel_precedence_get(yt_unit_t unit, yt_port_t port, yt_qos_intPri_sel_precedence_t *pPriTbl);


/**
 * @internal      fal_tiger_qos_intPri_portDefPri_set 
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intPri_portDefPri_set (yt_unit_t unit, yt_port_t port, yt_enable_t enable, yt_pri_t pri);


/**
 * @internal      fal_tiger_qos_intPri_portDefPri_get 
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @param[out]    pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_qos_intPri_portDefPri_get (yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable, yt_pri_t *pri);

/**
 * @internal      fal_tiger_qos_intPri_intCpri_map_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     cpri                -internal vlan priority (0 - MAX_PRIORITY)
 * @param[in]     pri                 -internal priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intPri_intCpri_map_set(yt_unit_t unit, yt_pri_t cpri, yt_pri_t pri);

/**
 * @internal      fal_tiger_qos_intPri_intCpri_map_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     cpri                -internal vlan priority (0 - MAX_PRIORITY)
 * @param[out]    pPri                -internal priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intPri_intCpri_map_get(yt_unit_t unit, yt_pri_t cpri, yt_pri_t *pPri);

/**
 * @internal      fal_tiger_qos_intPri_intSpri_map_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     spri                -internal vlan priority (0 - MAX_PRIORITY)
 * @param[in]     pri                 -internal priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intPri_intSpri_map_set(yt_unit_t unit, yt_pri_t spri, yt_pri_t pri);

/**
 * @internal      fal_tiger_qos_intPri_intSpri_map_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     spri                -internal vlan priority (0 - MAX_PRIORITY)
 * @param[out]    pPri                -internal priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intPri_intSpri_map_get(yt_unit_t unit, yt_pri_t spri, yt_pri_t *pPri);

/**
 * @internal      fal_tiger_qos_intPri_cpri_map_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     cpri                -tag control information exclude vid
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intPri_cpri_map_set(yt_unit_t unit, yt_qos_pmap_tci_t cpri, yt_pri_t pri);


/**
 * @internal      fal_tiger_qos_intPri_cpri_map_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     cpri                -tag control information exclude vid
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intPri_cpri_map_get(yt_unit_t unit, yt_qos_pmap_tci_t cpri, yt_pri_t *pPri);


/**
 * @internal      fal_tiger_qos_intPri_spri_map_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     spri                -tag control information exclude vid
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intPri_spri_map_set(yt_unit_t unit, yt_qos_pmap_tci_t spri, yt_pri_t pri);


/**
 * @internal      fal_tiger_qos_intPri_spri_map_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     spri                -tag control information exclude vid
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intPri_spri_map_get(yt_unit_t unit, yt_qos_pmap_tci_t spri, yt_pri_t *pPri);


/**
 * @internal      fal_tiger_qos_intPri_dscp_map_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     dscp                -the dscp value (0 - MAX_DSCP_VALUE)
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intPri_dscp_map_set(yt_unit_t unit, yt_dscp_t dscp, yt_pri_t pri);


/**
 * @internal      fal_tiger_qos_intPri_dscp_map_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     dscp                -the dscp value (0 - MAX_DSCP_VALUE)
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intPri_dscp_map_get(yt_unit_t unit, yt_dscp_t dscp, yt_pri_t *pPri);


/**
 * @internal      fal_tiger_qos_intPri_vlan_map_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     enable              -enable or disable
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_qos_intPri_vlan_map_set(yt_unit_t unit, yt_vlan_t vid, yt_enable_t enable, yt_pri_t pri);


/**
 * @internal      fal_tiger_qos_intPri_vlan_map_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[out]    pEnable             -enable or disable
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_qos_intPri_vlan_map_get(yt_unit_t unit, yt_vlan_t vid, yt_enable_t *pEnable, yt_pri_t *pPri);


/**
 * @internal      fal_tiger_qos_intPri_map_igrMirror_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     grpId               -mirror group id
 * @param[in]     enable              -enable or disable
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intPri_map_igrMirror_set(yt_unit_t unit, yt_mirror_group_t grpId, yt_enable_t enable, yt_pri_t pri);


/**
 * @internal      fal_tiger_qos_intPri_map_igrMirror_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     grpId               -mirror group id
 * @param[out]    pEnable             -enable or disable
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intPri_map_igrMirror_get(yt_unit_t unit, yt_mirror_group_t grpId, yt_enable_t *pEnable, yt_pri_t *pPri);


/**
 * @internal      fal_tiger_qos_intPri_map_egrMirror_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     grpId               -mirror group id
 * @param[in]     enable              -enable or disable
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intPri_map_egrMirror_set(yt_unit_t unit, yt_mirror_group_t grpId, yt_enable_t enable, yt_pri_t pri);


/**
 * @internal      fal_tiger_qos_intPri_map_egrMirror_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     grpId               -mirror group id
 * @param[out]    pEnable             -enable or disable
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intPri_map_egrMirror_get(yt_unit_t unit, yt_mirror_group_t grpId, yt_enable_t *pEnable, yt_pri_t *pPri);

/**
 * @internal      fal_tiger_qos_intDP_cpri_map_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     cpri                -tag control information exclude vid
 * @param[in]     dp                  -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intDP_cpri_map_set(yt_unit_t unit, yt_qos_pmap_tci_t cpri, yt_dp_t dp);


/**
 * @internal      fal_tiger_qos_intDP_cpri_map_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     cpri                -tag control information exclude vid
 * @param[out]    pDp                 -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intDP_cpri_map_get(yt_unit_t unit, yt_qos_pmap_tci_t cpri, yt_dp_t *pDp);


/**
 * @internal      fal_tiger_qos_intDP_spri_map_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     spri                -tag control information exclude vid
 * @param[in]     dp                  -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intDP_spri_map_set(yt_unit_t unit, yt_qos_pmap_tci_t spri, yt_dp_t dp);


/**
 * @internal      fal_tiger_qos_intDP_spri_map_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     spri                -tag control information exclude vid
 * @param[out]    pDp                 -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intDP_spri_map_get(yt_unit_t unit, yt_qos_pmap_tci_t spri, yt_dp_t *pDp);


/**
 * @internal      fal_tiger_qos_intDP_dscp_map_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     dscp                -the dscp value (0 - MAX_DSCP_VALUE)
 * @param[in]     dp                  -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intDP_dscp_map_set(yt_unit_t unit, yt_dscp_t dscp, yt_dp_t dp);


/**
 * @internal      fal_tiger_qos_intDP_dscp_map_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     dscp                -the dscp value (0 - MAX_DSCP_VALUE)
 * @param[out]    pDp                 -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_intDP_dscp_map_get(yt_unit_t unit, yt_dscp_t dscp, yt_dp_t *pDp);


/**
 * @internal      fal_tiger_qos_que_map_ucast_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pQmap               -internal priority to queue mapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_que_map_ucast_set(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *pQmap);


/**
 * @internal      fal_tiger_qos_que_map_ucast_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pQmap               -internal priority to queue mapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_que_map_ucast_get(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *pQmap);


/**
 * @internal      fal_tiger_qos_que_map_mcast_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pQmap               -internal priority to queue mapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_que_map_mcast_set(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *pQmap);


/**
 * @internal      fal_tiger_qos_que_map_mcast_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pQmap               -internal priority to queue mapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_que_map_mcast_get(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *pQmap);

/**
 * @internal      fal_tiger_qos_que_forceDropPerQue_enable_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_que_forceDropPerQue_enable_set(yt_unit_t unit, yt_qid_t qinfo, yt_enable_t enable);

/**
 * @internal      fal_tiger_qos_que_forceDropPerQue_enable_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_que_forceDropPerQue_enable_get(yt_unit_t unit, yt_qid_t qinfo, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_qos_que_forceDrop_enable_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_que_forceDrop_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_qos_que_forceDrop_enable_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_que_forceDrop_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      fal_tiger_qos_remark_port_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pRemarkCtrl            -remark states of different type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_remark_port_set(yt_unit_t unit, yt_port_t port, const yt_qos_remark_ctrl_t *pRemarkCtrl);


/**
 * @internal      fal_tiger_qos_remark_port_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pRemarkCtrl           -remark states of different type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_remark_port_get(yt_unit_t unit, yt_port_t port, yt_qos_remark_ctrl_t *pRemarkCtrl);


/**
 * @internal      fal_tiger_qos_remark_dscp_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -as a key value that remark to priority and dscp
 * @param[in]     newDscp            -the dscp value (0 - MAX_DSCP_VALUE)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_remark_dscp_set(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_dscp_t newDscp);


/**
 * @internal      fal_tiger_qos_remark_dscp_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -the priority and dscp for remark(src or dest)
 * @param[out]    pNewDscp           -the dscp value (0 - MAX_DSCP_VALUE)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_remark_dscp_get(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_dscp_t *pNewDscp);


/**
 * @internal      fal_tiger_qos_remark_cpri_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -as a key value that remark to priority and dscp
 * @param[in]     dstInfo             -tag control information exclude vid
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_remark_cpri_set(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t dstInfo);


/**
 * @internal      fal_tiger_qos_remark_cpri_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -as a key value that remark to priority and dscp
 * @param[out]    pDstInfo            -tag control information exclude vid
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_remark_cpri_get(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t *pDstInfo);


/**
 * @internal      fal_tiger_qos_remark_spri_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -as a key value that remark to priority and dscp
 * @param[in]     dstInfo             -tag control information exclude vid
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_remark_spri_set(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t dstInfo);


/**
 * @internal      fal_tiger_qos_remark_spri_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -as a key value that remark to priority and dscp
 * @param[out]    pDstInfo            -tag control information exclude vid
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_remark_spri_get(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t *pDstInfo);


/**
 * @internal      fal_tiger_qos_schedule_queue_pri_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[in]     qpri                -queue priority (0 - MAX_QUEUE_ID)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_schedule_queue_pri_set(yt_unit_t unit, yt_qid_t qinfo, yt_queue_pri_t qpri);


/**
 * @internal      fal_tiger_qos_schedule_queue_pri_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[out]    pQpri               -queue priority (0 - MAX_QUEUE_ID)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_schedule_queue_pri_get(yt_unit_t unit, yt_qid_t qinfo, yt_queue_pri_t *pQpri);


/**
 * @internal      fal_tiger_qos_schedule_dwrr_mode_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[in]     dwrrMode          -bbs and pps
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_schedule_dwrr_mode_set(yt_unit_t unit, yt_qid_t qinfo, yt_rate_mode_t dwrrMode);


/**
 * @internal      fal_tiger_qos_schedule_dwrr_mode_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[out]    pDwrrMode         -bbs and pps
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_schedule_dwrr_mode_get(yt_unit_t unit, yt_qid_t qinfo, yt_rate_mode_t *pDwrrMode);


/**
 * @internal      fal_tiger_qos_schedule_dwrr_weight_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[in]     qweight             -queue weight in dwrr mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_schedule_dwrr_weight_set(yt_unit_t unit, yt_qid_t qinfo, yt_queue_weight_t qweight);


/**
 * @internal      fal_tiger_qos_schedule_dwrr_weight_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[out]    pQweight            -queue weight in dwrr mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_qos_schedule_dwrr_weight_get(yt_unit_t unit, yt_qid_t qinfo, yt_queue_weight_t *pQweight);


extern uint32_t fal_tiger_qos_queue_id_get(yt_unit_t unit, yt_qid_t qinfo, uint8_t *pId);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
