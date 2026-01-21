/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_mirror.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_MIRROR_H__
#define __YT_MIRROR_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "yt_types.h"

typedef struct yt_mirror_entry_s
{
    uint32 flags;
    yt_port_t mirrorPort;
    yt_bool_t isLag;
    yt_port_mask_t rxPortMask;
    yt_port_mask_t txPortMask;
} yt_mirror_entry_t;

/* flags for mirror_group_set/get */
#define YT_MIRROR_FLAG_PORT_INGRESS BIT(0)
#define YT_MIRROR_FLAG_PORT_EGRESS  BIT(1)
#define YT_MIRROR_FLAG_MIRROR_PORT  BIT(2)
#define YT_MIRROR_FLAG_ALL (YT_MIRROR_FLAG_PORT_INGRESS | YT_MIRROR_FLAG_PORT_EGRESS | YT_MIRROR_FLAG_MIRROR_PORT)

/**
 * @internal      yt_mirror_init
 * @endinternal
 *
 * @brief         Init mirror module
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_mirror_init(yt_unit_t unit);

/**
 * @internal      yt_mirror_group_set
 * @endinternal
 *
 * @brief         Set mirror group entry, set mirrorPort = YT_MIRROR_INVALID_PORT to disable mirror function.
 *                Tiger not support mirror port is lag port.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     grpId               -group id
 * @param[in]     pMirrorEntry        -pointer to mirror entry
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_mirror_group_set(yt_unit_t unit, yt_mirror_group_t grpId, yt_mirror_entry_t *pMirrorEntry);

/**
 * @internal      yt_mirror_group_get
 * @endinternal
 *
 * @brief         Get mirror group entry.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     grpId               -group id
 * @param[out]     pMirrorEntry        -pointer to mirror entry
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_mirror_group_get(yt_unit_t unit, yt_mirror_group_t grpId, yt_mirror_entry_t *pMirrorEntry);




#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
