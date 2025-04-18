/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_loopdetect.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_loopdetect.h"
#include "fal_dispatch.h"


/**
 * @internal      yt_loop_detect_enable_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_loop_detect_enable_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->loop_detect_enable_set(unit, enable);
}

/**
 * @internal      yt_loop_detect_enable_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_loop_detect_enable_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->loop_detect_enable_get(unit, pEnable);
}

/**
 * @internal      yt_loop_detect_tpid_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     tpid                -tpid value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_loop_detect_tpid_set(yt_unit_t unit, yt_tpid_t tpid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->loop_detect_tpid_set(unit, tpid);
}

/**
 * @internal      yt_loop_detect_tpid_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pTpid               -tpid value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_loop_detect_tpid_get(yt_unit_t unit, yt_tpid_t *pTpid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pTpid), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->loop_detect_tpid_get(unit, pTpid);
}

/**
 * @internal      yt_loop_detect_generate_way_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     way                 -When a loop occurs, should it be handed over to HW or SW for processing
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_loop_detect_generate_way_set(yt_unit_t unit, yt_generate_way_t way)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((LOOP_DETECT_GENERATE_WAY_SW < way || LOOP_DETECT_GENERATE_WAY_HW > way), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->loop_detect_generate_way_set(unit, way);
}

/**
 * @internal      yt_loop_detect_generate_way_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pWay                -When a loop occurs, should it be handed over to HW or SW for processing
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_loop_detect_generate_way_get(yt_unit_t unit, yt_generate_way_t *pWay)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pWay), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->loop_detect_generate_way_get(unit, pWay);
}

/**
 * @internal      yt_loop_detect_unitID_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     localID             -local unit id
 * @param[in]     remoteID            -remote unit id, support two remote unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_loop_detect_unitID_set(yt_unit_t unit, yt_local_id_t localID, yt_remote_id_t remoteID)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->loop_detect_unitID_set(unit, localID, remoteID);
}

/**
 * @internal      yt_loop_detect_unitID_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pLocalID            -local unit id
 * @param[out]    pRemoteID           -remote unit id, support two remote unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_loop_detect_unitID_get(yt_unit_t unit, yt_local_id_t *pLocalID, yt_remote_id_t *pRemoteID)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pLocalID), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pRemoteID), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->loop_detect_unitID_get(unit, pLocalID, pRemoteID);
}
