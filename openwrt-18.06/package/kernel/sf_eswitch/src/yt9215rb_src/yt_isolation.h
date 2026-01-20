/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_isolation.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_ISOLATION_H
#define __YT_ISOLATION_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "yt_cmm.h"

#define YT_ISO_VLAN_NUM_MAX    (16)

typedef enum yt_iso_vlan_type_e
{
    YT_ISO_VLAN_TYPE_FVID = 0,
    YT_ISO_VLAN_TYPE_CVID,
    YT_ISO_VLAN_TYPE_SVID
}yt_iso_vlan_type_t;

/**
 * @internal      yt_isolation_port_set
 * @endinternal
 *
 * @brief         set isolation dest port mask of the port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     isoPortMask        -the isolation dest portmask of specific port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_isolation_port_set(yt_unit_t unit, yt_port_t port, yt_port_mask_t isoPortMask);


/**
 * @internal      yt_isolation_port_get
 * @endinternal
 *
 * @brief         get isolation dest port mask of the port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pIsoPortMask       -the isolation dest portmask of specific port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_isolation_port_get(yt_unit_t unit, yt_port_t port, yt_port_mask_t *pIsoPortMask);


/**
 * @internal      yt_isolation_vlan_add
 * @endinternal
 *
 * @brief         add isolation vlan info
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     isoInfo             -vlan isolation info(min vid/max vid/isolation member port)
 * @param[out]    idx                 -the index of the vlan isolation entry, idx start at 0
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_isolation_vlan_add(yt_unit_t unit, yt_iso_vlan_info_t isoInfo, uint16_t *idx);


/**
 * @internal      yt_isolation_vlan_del_by_index
 * @endinternal
 *
 * @brief         delete isolation vlan by index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -the index of the vlan isolation entry, idx start at 0
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_isolation_vlan_del_by_index(yt_unit_t unit, uint16_t idx);

/**
 * @internal      yt_isolation_vlan_set_by_index
 * @endinternal
 *
 * @brief         set isolation vlan by index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -the index of the vlan isolation entry, idx start at 0
 * @param[in]     isoInfo             -vlan isolation info(min vid/max vid/isolation member port)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_isolation_vlan_set_by_index(yt_unit_t unit, uint16_t idx, yt_iso_vlan_info_t isoInfo);

/**
 * @internal      yt_isolation_vlan_get_by_index
 * @endinternal
 *
 * @brief         get isolation vlan by index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     idx                 -the index of the vlan isolation entry, idx start at 0
 * @param[out]    isoInfo             -vlan isolation info(min vid/max vid/isolation member port)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_isolation_vlan_get_by_index(yt_unit_t unit, uint16_t idx, yt_iso_vlan_info_t *pIsoInfo);

/**
 * @internal      yt_isolation_vlan_type_set
 * @endinternal
 *
 * @brief         set isolation vlan type
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -the type of VLAN isolation
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_isolation_vlan_type_set(yt_unit_t unit, yt_iso_vlan_type_t type);

/**
 * @internal      yt_isolation_vlan_type_get
 * @endinternal
 *
 * @brief         get isolation vlan type
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pType               -the type of VLAN isolation
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_isolation_vlan_type_get(yt_unit_t unit, yt_iso_vlan_type_t *pType);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
