/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_vlan.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_VLAN_H
#define __FAL_TIGER_VLAN_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "fal_cmm.h"

#define VLAN_PROTOCOl_MAX_NUM                   4

typedef enum  yt_l2_type_bitmap_e
{
    L2_TYPE_BITMAP_ETHV2 = 1,
    L2_TYPE_BITMAP_ETHSAP = 2,
    L2_TYPE_BITMAP_ETHSNAP = 4
}yt_l2_type_bitmap_t;

/**
 * @internal      fal_tiger_vlan_init
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_init(yt_unit_t unit);

extern yt_ret_t  fal_tiger_vlan_port_set(yt_unit_t unit,  yt_vlan_t vid,  yt_port_mask_t  member_portmask, yt_port_mask_t  untag_portmask);

extern yt_ret_t  fal_tiger_vlan_port_get(yt_unit_t unit,  yt_vlan_t vid,  yt_port_mask_t  *pMember_portmask, yt_port_mask_t  *pUntag_portmask);

extern yt_ret_t  fal_tiger_vlan_svlMode_enable_set(yt_unit_t unit, yt_vlan_t vid,  yt_enable_t enable);

extern yt_ret_t  fal_tiger_vlan_svlMode_enable_get(yt_unit_t unit, yt_vlan_t vid,  yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_vlan_fid_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     fid                 -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_fid_set(yt_unit_t unit, yt_vlan_t vid,  yt_fid_t fid);


/**
 * @internal      fal_tiger_vlan_fid_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[out]    pFid                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_fid_get(yt_unit_t unit, yt_vlan_t vid,  yt_fid_t *pFid);


/**
 * @internal      fal_tiger_vlan_ingrTpid_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     tpid                -yt_vlan.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_ingrTpid_set(yt_unit_t unit,  yt_tpid_profiles_t tpid);


/**
 * @internal      fal_tiger_vlan_ingrTpid_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pTpid               -yt_vlan.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_ingrTpid_get(yt_unit_t unit,  yt_tpid_profiles_t *pTpid);


/**
 * @internal      fal_tiger_vlan_port_ingrTpidMask_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     tpidIdxMask         -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_port_ingrTpidMask_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_tpidprofile_id_mask_t tpidIdxMask);


/**
 * @internal      fal_tiger_vlan_port_ingrTpidMask_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pTpidIdxMask        -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_port_ingrTpidMask_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_tpidprofile_id_mask_t *pTpidIdxMask);


/**
 * @internal      fal_tiger_vlan_port_ingrDefaultVlan_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     vid                 -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_port_ingrDefaultVlan_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t vid);


/**
 * @internal      fal_tiger_vlan_port_ingrDefaultVlan_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pVid                -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_port_ingrDefaultVlan_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t *pVid);

/**
 * @internal      fal_tiger_vlan_port_igrDefPri_set
 * @endinternal
 *
 * @brief         Set port ingress default priority for untag packets depend on vlan type (CVLAN or SVLAN)
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     pri                 -priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_port_igrDefPri_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, uint8_t pri);

/**
 * @internal      fal_tiger_vlan_port_igrDefPri_get
 * @endinternal
 *
 * @brief         Get port ingress default priority for untag packets depend on vlan type (CVLAN or SVLAN)
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                -VLAN_TYPE_CVLAN or VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pPri                -priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_port_igrDefPri_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, uint8_t *pPri);

/**
 * @internal      fal_tiger_vlan_port_ingrFilter_enable_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enabled             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_port_ingrFilter_enable_set(yt_unit_t unit, yt_port_t  port, yt_enable_t enabled);


/**
 * @internal      fal_tiger_vlan_port_ingrFilter_enable_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnabled            -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_port_ingrFilter_enable_get(yt_unit_t unit, yt_port_t  port, yt_enable_t *pEnabled);

/**
 * @internal      fal_tiger_vlan_ingrTransparent_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     port_mask           -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_ingrTransparent_set(yt_unit_t unit, yt_port_t  port, yt_port_mask_t port_mask);


/**
 * @internal      fal_tiger_vlan_ingrTransparent_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pPort_mask          -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_ingrTransparent_get(yt_unit_t unit, yt_port_t  port, yt_port_mask_t *pPort_mask);


/**
 * @internal      fal_tiger_vlan_port_aft_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     tagAFT              -yt_vlan.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_port_aft_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, yt_vlan_aft_t tagAFT);


/**
 * @internal      fal_tiger_vlan_port_aft_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pTagAFT             -yt_vlan.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_port_aft_get(yt_unit_t unit, yt_vlan_type_t type, yt_port_t port, yt_vlan_aft_t *pTagAFT);


/**
 * @internal      fal_tiger_vlan_port_egrTagMode_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     tagMode             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_vlan_port_egrTagMode_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port,  yt_egr_tag_mode_t tagMode);


/**
 * @internal      fal_tiger_vlan_port_egrTagMode_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pTagMode            -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_vlan_port_egrTagMode_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port,  yt_egr_tag_mode_t *pTagMode);


/**
 * @internal      fal_tiger_vlan_port_egrDefaultVid_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     default_vid         -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_vlan_port_egrDefaultVid_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t default_vid);


/**
 * @internal      fal_tiger_vlan_port_egrDefaultVid_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pDefault_vid        -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_vlan_port_egrDefaultVid_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t *pDefault_vid);


/**
 * @internal      fal_tiger_vlan_egrTpid_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     tpids               -yt_vlan.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_vlan_egrTpid_set(yt_unit_t unit,  yt_tpid_profiles_t tpids);


/**
 * @internal      fal_tiger_vlan_egrTpid_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pTpids              -yt_vlan.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_vlan_egrTpid_get(yt_unit_t unit,  yt_tpid_profiles_t *pTpids);


/**
 * @internal      fal_tiger_vlan_port_egrTpidIdx_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     tpidIdx             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_vlan_port_egrTpidIdx_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, uint8_t tpidIdx);


/**
 * @internal      fal_tiger_vlan_port_egrTpidIdx_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pTpidIdx            -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_vlan_port_egrTpidIdx_get(yt_unit_t unit, yt_vlan_type_t type, yt_port_t port, uint8_t *pTpidIdx);


/**
 * @internal      fal_tiger_vlan_port_egrTransparent_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @param[in]     port_mask           -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_vlan_port_egrTransparent_set(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t port,  yt_enable_t enable, yt_port_mask_t port_mask);


/**
 * @internal      fal_tiger_vlan_port_egrTransparent_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @param[out]    pPort_mask          -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_vlan_port_egrTransparent_get(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t port,  yt_enable_t *pEnable, yt_port_mask_t *pPort_mask);


/**
 * @internal      fal_tiger_vlan_port_egrFilter_en_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enabled             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_port_egrFilter_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enabled);


/**
 * @internal      fal_tiger_vlan_port_egrFilter_en_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnabled            -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_vlan_port_egrFilter_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnabled);


/**
 * @internal      fal_tiger_port_vidTypeSel_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     mode                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_port_vidTypeSel_set(yt_unit_t unit, yt_port_t port, yt_vlan_type_t mode);


/**
 * @internal      fal_tiger_port_vidTypeSel_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pMode               -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_port_vidTypeSel_get(yt_unit_t unit, yt_port_t port, yt_vlan_type_t *pMode);

/**
 * @internal      fal_tiger_vlan_protocolBasedVlan_group_set
 * @endinternal
 *
 * @brief         Set protocol based vlan group
 * @note          APPLICABLE DEVICES  -Tiger&Shark
 * @param[in]     unit                -unit id
 * @param[in]     groupId           -group id
 * @param[in]    pKey                -protocol key
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  fal_tiger_vlan_protocolBasedVlan_group_set(yt_unit_t unit, uint8_t groupId, const yt_vlan_protocol_key_t *pKey);

/**
 * @internal      fal_tiger_vlan_protocolBasedVlan_group_get
 * @endinternal
 *
 * @brief         Get protocol based vlan group
 * @note          APPLICABLE DEVICES  -Tiger&Shark
 * @param[in]     unit                -unit id
 * @param[in]     groupId           -group id
 * @param[out]    pKey                -protocol key
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 * @retval        CMM_ERR_NULL_POINT   -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  fal_tiger_vlan_protocolBasedVlan_group_get(yt_unit_t unit, uint8_t groupId, yt_vlan_protocol_key_t *pKey);

/**
 * @internal      fal_tiger_vlan_protocolBasedVlan_table_add
 * @endinternal
 *
 * @brief         Set protocol vlan table action
 * @note          APPLICABLE DEVICES  -Tiger&Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     groupId           -group id
 * @param[in]     pAction              -action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 * @retval        CMM_ERR_PORT         -port err
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  fal_tiger_vlan_protocolBasedVlan_table_add(yt_unit_t unit, yt_port_t port, uint8_t groupId, const yt_vlan_protocol_action_t *pAction);

/**
 * @internal      fal_tiger_vlan_protocolBasedVlan_table_get
 * @endinternal
 *
 * @brief         Get protocol vlan table action
 * @note          APPLICABLE DEVICES  -Tiger&Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     groupId           -group id
 * @param[out]    pAction             -action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 * @retval        CMM_ERR_PORT         -port err
 * @retval        CMM_ERR_NULL_POINT   -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  fal_tiger_vlan_protocolBasedVlan_table_get(yt_unit_t unit, yt_port_t port, uint8_t  groupId, yt_vlan_protocol_action_t *pAction);

/**
 * @internal      fal_tiger_vlan_protocolBasedVlan_table_del
 * @endinternal
 *
 * @brief         Del protocol vlan table action
 * @note          APPLICABLE DEVICES  -Tiger&Shark
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     groupId           -group id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 * @retval        CMM_ERR_PORT         -port err
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  fal_tiger_vlan_protocolBasedVlan_table_del(yt_unit_t unit, yt_port_t port, uint8_t  groupId);

/**
 * @internal      fal_tiger_vlan_mac_ipSub_tbl_arrange_set
 * @endinternal
 *
 * @brief          allocate resource for both of mac vlan and ip subnet vlan
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                   -unit id
 * @param[in]     macNum             -mac vlan entry number
 * @param[in]     ipSubNum          -ip subnet vlan entry number
 * @param[in]     pri                       -mac vlan priority first or ip subnet vlan priority first
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  fal_tiger_vlan_mac_ipSub_tbl_arrange_set(yt_unit_t unit, uint8_t macNum, uint8_t ipSubNum, yt_vlan_mac_ip_subnet_pri_t  pri);

/**
 * @internal      fal_tiger_vlan_mac_ipSub_tbl_arrange_get
 * @endinternal
 *
 * @brief          Get allocated resource for both of mac vlan and ip subnet vlan
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                   -unit id
 * @param[out]     pMacNum             -mac vlan entry number
 * @param[out]     pIpSubNum         -ip subnet vlan entry number
 * @param[out]     pPri                       -mac vlan priority first or ip subnet vlan priority first
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 * @retval        CMM_ERR_NULL_POINT   -point is NULL
 */
extern yt_ret_t  fal_tiger_vlan_mac_ipSub_tbl_arrange_get(yt_unit_t unit, uint8_t *pMacNum, uint8_t *pIpSubNum, yt_vlan_mac_ip_subnet_pri_t  *pPri);

/**
 * @internal      fal_tiger_vlan_macBasedVlan_table_add
 * @endinternal
 *
 * @brief         Add mac based vlan table and return the table index
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     pMacVlanRule    -mac vlan entry rule
 * @param[in]     pMacVlanAction  -mac vlan entry action
 * @param[out]     pTableIdx       -table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 * @retval        CMM_ERR_PORT         -port err
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  fal_tiger_vlan_macBasedVlan_table_add(yt_unit_t unit, const yt_mac_vlan_rule_t *pMacVlanRule, const yt_mac_vlan_action_t *pMacVlanAction, yt_mac_vlan_table_idx_t *pTableIdx);

/**
 * @internal      fal_tiger_vlan_macBasedVlan_table_add_by_index
 * @endinternal
 *
 * @brief         Set mac based vlan table by the table index
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     tableIdx          - table index
 * @param[in]     pMacVlanRule    -mac vlan entry rule
 * @param[in]     pMacVlanAction  -mac vlan entry action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 * @retval        CMM_ERR_PORT         -port err
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  fal_tiger_vlan_macBasedVlan_table_add_by_index(yt_unit_t unit, yt_mac_vlan_table_idx_t tableIdx, const yt_mac_vlan_rule_t *pMacVlanRule, const yt_mac_vlan_action_t *pMacVlanAction);

/**
 * @internal      fal_tiger_vlan_macBasedVlan_table_get
 * @endinternal
 *
 * @brief         Get mac based vlan table by the table index
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     tableIdx          - table index
 * @param[in]     pMacVlanRule    -mac vlan entry rule
 * @param[in]     pMacVlanAction  -mac vlan entry action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 */
extern yt_ret_t  fal_tiger_vlan_macBasedVlan_table_get(yt_unit_t unit, yt_mac_vlan_table_idx_t tableIdx, yt_mac_vlan_rule_t *pMacVlanRule, yt_mac_vlan_action_t *pMacVlanAction);

/**
 * @internal      fal_tiger_vlan_macBasedVlan_table_del
 * @endinternal
 *
 * @brief         Delete mac based vlan table by the table index
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     tableIdx          - table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 */
extern yt_ret_t  fal_tiger_vlan_macBasedVlan_table_del(yt_unit_t unit, yt_mac_vlan_table_idx_t tableIdx);

/**
 * @internal      fal_tiger_vlan_ipSubnetBasedVlan_table_add
 * @endinternal
 *
 * @brief         Set ip subnet based vlan table and return the table index
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     pIpSubnetVlanRule    -ip subnet vlan entry rule
 * @param[in]     pIpSubnetVlanAction  -ip subnet vlan entry action
 * @param[out]    pTableIdx       -table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 * @retval        CMM_ERR_PORT         -port err
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  fal_tiger_vlan_ipSubnetBasedVlan_table_add(yt_unit_t unit, const yt_ip_subnet_vlan_rule_t *pIpSubnetVlanRule, const yt_ip_subnet_vlan_action_t *pIpSubnetVlanAction, yt_ip_subnet_vlan_table_idx_t *pTableIdx);

/**
 * @internal      fal_tiger_vlan_ipSubnetBasedVlan_table_add_by_index
 * @endinternal
 *
 * @brief         Set ip subnet based vlan table by the table index
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     tableIdx          - table index
 * @param[in]     pIpSubnetVlanRule    -ip subnet vlan entry rule
 * @param[in]     pIpSubnetVlanAction  -ip subnet vlan entry action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 * @retval        CMM_ERR_PORT         -port err
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  fal_tiger_vlan_ipSubnetBasedVlan_table_add_by_index(yt_unit_t unit, yt_ip_subnet_vlan_table_idx_t tableIdx, const yt_ip_subnet_vlan_rule_t *pIpSubnetVlanRule, const yt_ip_subnet_vlan_action_t *pIpSubnetVlanAction);

/**
 * @internal      fal_tiger_vlan_ipSubnetBasedVlan_table_get
 * @endinternal
 *
 * @brief         Get ip subnet based vlan table by the table index
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     tableIdx          - table index
 * @param[in]     pIpSubnetVlanRule    -ip subnet vlan entry rule
 * @param[in]     pIpSubnetVlanAction  -ip subnet vlan entry action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 */
extern yt_ret_t  fal_tiger_vlan_ipSubnetBasedVlan_table_get(yt_unit_t unit, yt_ip_subnet_vlan_table_idx_t tableIdx, yt_ip_subnet_vlan_rule_t *pIpSubnetVlanRule, yt_ip_subnet_vlan_action_t *pIpSubnetVlanAction);


/**
 * @internal      fal_tiger_vlan_ipSubnetBasedVlan_table_del
 * @endinternal
 *
 * @brief         Delete ip subnet based vlan table by the table index
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     tableIdx          - table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 */
extern yt_ret_t  fal_tiger_vlan_ipSubnetBasedVlan_table_del(yt_unit_t unit, yt_ip_subnet_vlan_table_idx_t tableIdx);

/**
 * @internal      fal_tiger_vlan_ipSubnetBasedVlan_bypass_arp_set
 * @endinternal
 *
 * @brief         Set arp type check for ip subnet based vlan table
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 */
extern yt_ret_t  fal_tiger_vlan_ipSubnetBasedVlan_bypass_arp_set(yt_unit_t unit, yt_enable_t enable);

/**
 * @internal      fal_tiger_vlan_ipSubnetBasedVlan_bypass_arp_get
 * @endinternal
 *
 * @brief         Get arp type check of ip subnet based vlan table
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
  * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 */
extern yt_ret_t  fal_tiger_vlan_ipSubnetBasedVlan_bypass_arp_get(yt_unit_t unit, yt_enable_t *pEnable);

/**
 * @internal      fal_tiger_vlan_policy_idx_mapping_set
 * @endinternal
 *
 * @brief         set the mapping vlan policy of each vlan
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     vlanPolicyIdx     -vlan policy index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  fal_tiger_vlan_policy_idx_mapping_set(yt_unit_t unit, yt_vlan_t vid,  uint8_t vlanPolicyIdx);

/**
 * @internal      fal_tiger_vlan_policy_idx_mapping_get
 * @endinternal
 *
 * @brief         get the mapping vlan policy of each vlan
 * @note          APPLICABLE DEVICES - Shark
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[pot]   pVlanPolicyIdx     -vlan policy index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 * @retval        CMM_ERR_NULL_POINT   -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  fal_tiger_vlan_policy_idx_mapping_get(yt_unit_t unit, yt_vlan_t vid,  uint8_t *pVlanPolicyIdx);

/**
 * @internal      fal_tiger_vlan_policy_table_add
 * @endinternal
 *
 * @brief         add vlan policy table
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     pVlanPolicyInfo    -vlan policy info
 * @param[out]     pVlanPolicyIdx     -vlan policy index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 * @retval        CMM_ERR_PORTLIST     -portlist err
 * @retval        CMM_ERR_NULL_POINT   -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  fal_tiger_vlan_policy_table_add(yt_unit_t unit, const yt_vlan_policy_info_t *pVlanPolicyInfo,  uint8_t *pVlanPolicyIdx);

/**
 * @internal      fal_tiger_vlan_policy_table_add_by_index
 * @endinternal
 *
 * @brief         add vlan policy table by index
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     vlanPolicyIdx     -vlan policy index
 * @param[in]     pVlanPolicyInfo    -vlan policy info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 * @retval        CMM_ERR_PORTLIST     -portlist err
 * @retval        CMM_ERR_NULL_POINT   -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  fal_tiger_vlan_policy_table_add_by_index(yt_unit_t unit, uint8_t vlanPolicyIdx, const yt_vlan_policy_info_t *pVlanPolicyInfo);

/**
 * @internal      fal_tiger_vlan_policy_table_get
 * @endinternal
 *
 * @brief         get vlan policy table
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     vlanPolicyIdx     -vlan policy index
 * @param[out]     pVlanPolicyInfo    -vlan policy info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 * @retval        CMM_ERR_NULL_POINT   -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  fal_tiger_vlan_policy_table_get(yt_unit_t unit, uint8_t vlanPolicyIdx, yt_vlan_policy_info_t *pVlanPolicyInfo);

/**
 * @internal      fal_tiger_vlan_policy_table_del
 * @endinternal
 *
 * @brief         delete vlan policy table
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     unit                -unit id
 * @param[in]     vlanPolicyIdx     -vlan policy index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input value err
 * @retval        CMM_ERR_NOT_INIT     -not init
 * @retval        CMM_ERR_NULL_POINT   -point is NULL
 */
extern yt_ret_t  fal_tiger_vlan_policy_table_del(yt_unit_t unit, uint8_t vlanPolicyIdx);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
