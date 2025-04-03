/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_qos.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_qos.h"
#include "fal_dispatch.h"


/**
 * @internal      yt_qos_intPri_map_weight_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     pri_tbl             -internal priority select, The higher the value, the higher the priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intPri_map_weight_set(yt_unit_t unit, yt_port_t port, yt_qos_intPri_map_weight_t pri_tbl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->qos_intPri_map_weight_set(unit, port, pri_tbl);
}

/**
 * @internal      yt_qos_intPri_map_weight_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pri_tbl             -internal priority select, The higher the value, the higher the priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intPri_map_weight_get(yt_unit_t unit, yt_port_t port, yt_qos_intPri_map_weight_t *pri_tbl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pri_tbl), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intPri_map_weight_get(unit, port, pri_tbl);
}

/**
 * @internal      yt_qos_intPri_portDefPri_set  
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intPri_portDefPri_set  (yt_unit_t unit, yt_port_t port, yt_enable_t enable, yt_pri_t pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intPri_portDefPri_set (unit, port, enable, pri);
}

/**
 * @internal      yt_qos_intPri_portDefPri_get 
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_qos_intPri_portDefPri_get (yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable, yt_pri_t *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pPri), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intPri_portDefPri_get (unit, port, pEnable, pPri);
}

/**
 * @internal      yt_qos_intPri_cpri_map_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     cpri                -tag control information exclude vid
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intPri_cpri_map_set(yt_unit_t unit, yt_port_t port, yt_qos_pmap_tci_t cpri, yt_pri_t pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->qos_intPri_cpri_map_set(unit, port, cpri, pri);
}

/**
 * @internal      yt_qos_intPri_cpri_map_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     cpri                -tag control information exclude vid
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intPri_cpri_map_get(yt_unit_t unit, yt_port_t port, yt_qos_pmap_tci_t cpri, yt_pri_t *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pPri), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intPri_cpri_map_get(unit, port, cpri, pPri);
}

/**
 * @internal      yt_qos_intPri_spri_map_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     spri                -tag control information exclude vid
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intPri_spri_map_set(yt_unit_t unit, yt_port_t port, yt_qos_pmap_tci_t spri, yt_pri_t pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->qos_intPri_spri_map_set(unit, port, spri, pri);
}

/**
 * @internal      yt_qos_intPri_spri_map_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     spri                -tag control information exclude vid
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intPri_spri_map_get(yt_unit_t unit, yt_port_t port, yt_qos_pmap_tci_t spri, yt_pri_t *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pPri), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intPri_spri_map_get(unit, port, spri, pPri);
}

/**
 * @internal      yt_qos_intPri_dscp_map_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     dscp                -the dscp value (0 - MAX_DSCP_VALUE)
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intPri_dscp_map_set(yt_unit_t unit, yt_dscp_t dscp, yt_pri_t pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->qos_intPri_dscp_map_set(unit, dscp, pri);
}

/**
 * @internal      yt_qos_intPri_dscp_map_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     dscp                -the dscp value (0 - MAX_DSCP_VALUE)
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intPri_dscp_map_get(yt_unit_t unit, yt_dscp_t dscp, yt_pri_t *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pPri), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intPri_dscp_map_get(unit, dscp, pPri);
}

/**
 * @internal      yt_qos_intPri_vlan_map_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     enable              -enable or disable
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_qos_intPri_vlan_map_set(yt_unit_t unit, yt_vlan_t vid, yt_enable_t enable, yt_pri_t pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intPri_vlan_map_set(unit, vid, enable, pri);
}

/**
 * @internal      yt_qos_intPri_vlan_map_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[out]    pEnable             -enable or disable
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_qos_intPri_vlan_map_get(yt_unit_t unit, yt_vlan_t vid, yt_enable_t *pEnable, yt_pri_t *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pPri), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intPri_vlan_map_get(unit, vid, pEnable, pPri);
}

/**
 * @internal      yt_qos_intPri_map_igrMirror_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intPri_map_igrMirror_set(yt_unit_t unit, yt_enable_t enable, yt_pri_t pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intPri_map_igrMirror_set(unit, enable, pri);
}

/**
 * @internal      yt_qos_intPri_map_igrMirror_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intPri_map_igrMirror_get(yt_unit_t unit, yt_enable_t *pEnable, yt_pri_t *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pPri), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intPri_map_igrMirror_get(unit, pEnable, pPri);
}

/**
 * @internal      yt_qos_intPri_map_egrMirror_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @param[in]     pri                 -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intPri_map_egrMirror_set(yt_unit_t unit, yt_enable_t enable, yt_pri_t pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_intPri_map_egrMirror_set(unit, enable, pri);
}

/**
 * @internal      yt_qos_intPri_map_egrMirror_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @param[out]    pPri                -internal priority or vlan tag priority (0 - MAX_PRIORITY)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intPri_map_egrMirror_get(yt_unit_t unit, yt_enable_t *pEnable, yt_pri_t *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pPri), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intPri_map_egrMirror_get(unit, pEnable, pPri);
}

/**
 * @internal      yt_qos_intDP_cpri_map_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     cpri                -tag control information exclude vid
 * @param[in]     dp                  -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intDP_cpri_map_set(yt_unit_t unit, yt_port_t port, yt_qos_pmap_tci_t cpri, yt_dp_t dp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->qos_intDP_cpri_map_set(unit, port, cpri, dp);
}

/**
 * @internal      yt_qos_intDP_cpri_map_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     cpri                -tag control information exclude vid
 * @param[out]    pDp                 -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intDP_cpri_map_get(yt_unit_t unit, yt_port_t port, yt_qos_pmap_tci_t cpri, yt_dp_t *pDp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pDp), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intDP_cpri_map_get(unit, port, cpri, pDp);
}

/**
 * @internal      yt_qos_intDP_spri_map_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     spri                -tag control information exclude vid
 * @param[in]     dp                  -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intDP_spri_map_set(yt_unit_t unit, yt_port_t port, yt_qos_pmap_tci_t spri, yt_dp_t dp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->qos_intDP_spri_map_set(unit, port, spri, dp);
}

/**
 * @internal      yt_qos_intDP_spri_map_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     spri                -tag control information exclude vid
 * @param[out]    pDp                 -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intDP_spri_map_get(yt_unit_t unit, yt_port_t port, yt_qos_pmap_tci_t spri, yt_dp_t *pDp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pDp), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intDP_spri_map_get(unit, port, spri, pDp);
}

/**
 * @internal      yt_qos_intDP_dscp_map_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     dscp                -the dscp value (0 - MAX_DSCP_VALUE)
 * @param[in]     dp                  -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intDP_dscp_map_set(yt_unit_t unit, yt_dscp_t dscp, yt_dp_t dp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->qos_intDP_dscp_map_set(unit, dscp, dp);
}

/**
 * @internal      yt_qos_intDP_dscp_map_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     dscp                -the dscp value (0 - MAX_DSCP_VALUE)
 * @param[out]    pDp                 -drop priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_intDP_dscp_map_get(yt_unit_t unit, yt_dscp_t dscp, yt_dp_t *pDp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pDp), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_intDP_dscp_map_get(unit, dscp, pDp);
}

/**
 * @internal      yt_qos_que_map_ucast_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     qmap_tbl            -internal priority to queue mapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_que_map_ucast_set(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t qmap_tbl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->qos_que_map_ucast_set(unit, port, qmap_tbl);
}

/**
 * @internal      yt_qos_que_map_ucast_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    qmap_tbl            -internal priority to queue mapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_que_map_ucast_get(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *qmap_tbl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == qmap_tbl), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_que_map_ucast_get(unit, port, qmap_tbl);
}

/**
 * @internal      yt_qos_que_map_mcast_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     qmap_tbl            -internal priority to queue mapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_que_map_mcast_set(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t qmap_tbl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->qos_que_map_mcast_set(unit, port, qmap_tbl);
}

/**
 * @internal      yt_qos_que_map_mcast_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    qmap_tbl            -internal priority to queue mapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_que_map_mcast_get(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *qmap_tbl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == qmap_tbl), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_que_map_mcast_get(unit, port, qmap_tbl);
}

/**
 * @internal      yt_qos_que_forceDrop_enable_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_que_forceDrop_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_que_forceDrop_enable_get(unit, port, pEnable);
}

/**
 * @internal      yt_qos_que_forceDrop_enable_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_que_forceDrop_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->qos_que_forceDrop_enable_set(unit, port, enable);
}

/**
 * @internal      yt_qos_remark_port_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     rmark_en            -remark states of different type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_remark_port_set(yt_unit_t unit, yt_port_t port, yt_qos_remark_en_t rmark_en)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->qos_remark_port_set(unit, port, rmark_en);
}

/**
 * @internal      yt_qos_remark_port_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pRmark_en           -remark states of different type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_remark_port_get(yt_unit_t unit, yt_port_t port, yt_qos_remark_en_t *pRmark_en)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pRmark_en), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_remark_port_get(unit, port, pRmark_en);
}

/**
 * @internal      yt_qos_remark_dscp_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -as a key value that remark to priority and dscp
 * @param[in]     new_dscp            -the dscp value (0 - MAX_DSCP_VALUE)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_remark_dscp_set(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_dscp_t new_dscp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->qos_remark_dscp_set(unit, srcInfo, new_dscp);
}

/**
 * @internal      yt_qos_remark_dscp_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -the priority and dscp for remark(src or dest)
 * @param[out]    pNew_dscp           -the dscp value (0 - MAX_DSCP_VALUE)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_remark_dscp_get(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_dscp_t *pNew_dscp)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pNew_dscp), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_remark_dscp_get(unit, srcInfo, pNew_dscp);
}

/**
 * @internal      yt_qos_remark_cpri_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     srcInfo             -as a key value that remark to priority and dscp
 * @param[in]     dstInfo             -tag control information exclude vid
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_remark_cpri_set(yt_unit_t unit, yt_port_t port, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t dstInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->qos_remark_cpri_set(unit, port, srcInfo, dstInfo);
}

/**
 * @internal      yt_qos_remark_cpri_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     srcInfo             -as a key value that remark to priority and dscp
 * @param[out]    pDstInfo            -tag control information exclude vid
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_remark_cpri_get(yt_unit_t unit, yt_port_t port, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t *pDstInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pDstInfo), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_remark_cpri_get(unit, port, srcInfo, pDstInfo);
}

/**
 * @internal      yt_qos_remark_spri_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     srcInfo             -as a key value that remark to priority and dscp
 * @param[in]     dstInfo             -tag control information exclude vid
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_remark_spri_set(yt_unit_t unit, yt_port_t port, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t dstInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->qos_remark_spri_set(unit, port, srcInfo, dstInfo);
}

/**
 * @internal      yt_qos_remark_spri_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     srcInfo             -as a key value that remark to priority and dscp
 * @param[out]    pDstInfo            -tag control information exclude vid
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_remark_spri_get(yt_unit_t unit, yt_port_t port, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t *pDstInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pDstInfo), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_remark_spri_get(unit, port, srcInfo, pDstInfo);
}

/**
 * @internal      yt_qos_schedule_sp_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[in]     qpri                -queue priority (0 - MAX_QUEUE_ID)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_schedule_sp_set(yt_unit_t unit, yt_qid_t qinfo, yt_queue_pri_t qpri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, qinfo.port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->qos_schedule_sp_set(unit, qinfo, qpri);
}

/**
 * @internal      yt_qos_schedule_sp_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[out]    pQpri               -queue priority (0 - MAX_QUEUE_ID)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_schedule_sp_get(yt_unit_t unit, yt_qid_t qinfo, yt_queue_pri_t *pQpri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, qinfo.port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pQpri), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_schedule_sp_get(unit, qinfo, pQpri);
}

/**
 * @internal      yt_qos_schedule_dwrr_mode_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[in]     dwrr_cmode          -bbs and pps
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_schedule_dwrr_mode_set(yt_unit_t unit, yt_qid_t qinfo, yt_rate_mode_t dwrr_cmode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, qinfo.port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->qos_schedule_dwrr_mode_set(unit, qinfo, dwrr_cmode);
}

/**
 * @internal      yt_qos_schedule_dwrr_mode_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[out]    pDwrr_cmode         -bbs and pps
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_schedule_dwrr_mode_get(yt_unit_t unit, yt_qid_t qinfo, yt_rate_mode_t *pDwrr_cmode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, qinfo.port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pDwrr_cmode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_schedule_dwrr_mode_get(unit, qinfo, pDwrr_cmode);
}

/**
 * @internal      yt_qos_schedule_dwrr_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[in]     qweight             -queue weight in dwrr mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_schedule_dwrr_set(yt_unit_t unit, yt_qid_t qinfo, yt_queue_weight_t qweight)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, qinfo.port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->qos_schedule_dwrr_set(unit, qinfo, qweight);
}

/**
 * @internal      yt_qos_schedule_dwrr_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     qinfo               -queue id, port, and ucast or mcast type config
 * @param[out]    pQweight            -queue weight in dwrr mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_qos_schedule_dwrr_get(yt_unit_t unit, yt_qid_t qinfo, yt_queue_weight_t *pQweight)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, qinfo.port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pQweight), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->qos_schedule_dwrr_get(unit, qinfo, pQweight);
}
