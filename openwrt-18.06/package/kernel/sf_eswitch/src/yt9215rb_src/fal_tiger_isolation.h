/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_port_isolation.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_PORT_ISOLATION_H
#define __FAL_TIGER_PORT_ISOLATION_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "fal_cmm.h"


/**
 * @internal      fal_tiger_isolation_port_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     isoPortMask        -Port Bit Mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_isolation_port_set(yt_unit_t unit, yt_port_t port, yt_port_mask_t isoPortMask);


/**
 * @internal      fal_tiger_isolation_port_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pIsoPortMask       -Port Bit Mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_isolation_port_get(yt_unit_t unit, yt_port_t port, yt_port_mask_t *pIsoPortMask);


/**
 * @internal      fal_tiger_isolation_vlan_add
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     isoInfo             -vlan isolation info(min vid/max vid/isolation member port)
 * @param[out]    idx                 -the index of the vlan isolation entry, idx start at 0
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_isolation_vlan_add(yt_unit_t unit, yt_iso_vlan_info_t isoInfo, uint16_t *idx);

/**
 * @internal      fal_tiger_isolation_vlan_del_by_index
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -the index of the vlan isolation entry, idx start at 0
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_isolation_vlan_del_by_index(yt_unit_t unit, uint16_t idx);

/**
 * @internal      fal_tiger_isolation_vlan_set_by_index
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -the index of the vlan isolation entry, idx start at 0
 * @param[in]     isoInfo             -vlan isolation info(min vid/max vid/isolation member port)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_isolation_vlan_set_by_index(yt_unit_t unit, uint16_t idx, yt_iso_vlan_info_t isoInfo);

/**
 * @internal      fal_tiger_isolation_vlan_get_by_index
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -the index of the vlan isolation entry, idx start at 0
 * @param[out]    isoInfo             -vlan isolation info(min vid/max vid/isolation member port)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_isolation_vlan_get_by_index(yt_unit_t unit, uint16_t idx, yt_iso_vlan_info_t *pIsoInfo);


/**
 * @internal      fal_tiger_isolation_vlan_type_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_isolation_vlan_type_set(yt_unit_t unit, yt_iso_vlan_type_t type);

/**
 * @internal      fal_tiger_isolation_vlan_type_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pType               -the type of VLAN isolation
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_isolation_vlan_type_get(yt_unit_t unit, yt_iso_vlan_type_t *pType);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
