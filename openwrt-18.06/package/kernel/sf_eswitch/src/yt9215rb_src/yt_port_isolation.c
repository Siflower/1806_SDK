/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_port_isolation.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_port_isolation.h"
#include "fal_dispatch.h"


/**
 * @internal      yt_port_isolation_set
 * @endinternal
 *
 * @brief         set allowed dest port mask of the port
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     iso_portmask        -the allowed dest portmask of specific port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_port_isolation_set(yt_unit_t unit, yt_port_t port, yt_port_mask_t iso_portmask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit,iso_portmask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->port_isolation_set(unit, port, iso_portmask);
}

/**
 * @internal      yt_port_isolation_get
 * @endinternal
 *
 * @brief         get allowed dest port mask of the port
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pIso_portmask       -the allowed dest portmask of specific port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_port_isolation_get(yt_unit_t unit, yt_port_t port, yt_port_mask_t *pIso_portmask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pIso_portmask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->port_isolation_get(unit, port, pIso_portmask);
}

