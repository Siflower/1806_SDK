/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_ctrlpkt.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_ctrlpkt.h"
#include "fal_dispatch.h"


/**
 * @internal      yt_ctrlpkt_unknown_ucast_act_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     act_ctrl            -l2 packet action,FWD,TRAP,DROP,COPY
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_ctrlpkt_unknown_ucast_act_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_l2_action_t act_ctrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((L2_ACTION_COPY < act_ctrl || L2_ACTION_FWD > act_ctrl), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->ctrlpkt_unknown_ucast_act_set(unit, port, act_ctrl);
}

/**
 * @internal      yt_ctrlpkt_unknown_ucast_act_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pAct_ctrl           -l2 packet action,FWD,TRAP,DROP,COPY
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_ctrlpkt_unknown_ucast_act_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_l2_action_t *pAct_ctrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pAct_ctrl), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->ctrlpkt_unknown_ucast_act_get(unit, port, pAct_ctrl);
}

/**
 * @internal      yt_ctrlpkt_unknown_mcast_act_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     act_ctrl            -l2 packet action,FWD,TRAP,DROP,COPY
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_ctrlpkt_unknown_mcast_act_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_l2_action_t act_ctrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((L2_ACTION_COPY < act_ctrl || L2_ACTION_FWD > act_ctrl), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->ctrlpkt_unknown_mcast_act_set(unit, port, act_ctrl);
}

/**
 * @internal      yt_ctrlpkt_unknown_mcast_act_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pAct_ctrl           -l2 packet action,FWD,TRAP,DROP,COPY
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_ctrlpkt_unknown_mcast_act_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_l2_action_t *pAct_ctrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pAct_ctrl), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->ctrlpkt_unknown_mcast_act_get(unit, port, pAct_ctrl);
}

/**
 * @internal      yt_ctrlpkt_arp_act_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     act_ctrl            -l2 packet action,FWD,TRAP,DROP,COPY
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_ctrlpkt_arp_act_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_l2_action_t act_ctrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((L2_ACTION_COPY < act_ctrl || L2_ACTION_FWD > act_ctrl), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->ctrlpkt_arp_act_set(unit, port, act_ctrl);
}

/**
 * @internal      yt_ctrlpkt_arp_act_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pAct_ctrl           -l2 packet action,FWD,TRAP,DROP,COPY
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_ctrlpkt_arp_act_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_l2_action_t *pAct_ctrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pAct_ctrl), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->ctrlpkt_arp_act_get(unit, port, pAct_ctrl);
}

/**
 * @internal      yt_ctrlpkt_nd_act_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     act_ctrl            -l2 packet action,FWD,TRAP,DROP,COPY
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_ctrlpkt_nd_act_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_l2_action_t act_ctrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((L2_ACTION_COPY < act_ctrl || L2_ACTION_FWD > act_ctrl), CMM_ERR_INPUT);
    
    return YT_DISPATCH(unit)->ctrlpkt_nd_act_set(unit, port, act_ctrl);
}


/**
 * @internal      yt_ctrlpkt_nd_act_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pAct_ctrl           -l2 packet action,FWD,TRAP,DROP,COPY
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_ctrlpkt_nd_act_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_l2_action_t *pAct_ctrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pAct_ctrl), CMM_ERR_NULL_POINT);
    
    return YT_DISPATCH(unit)->ctrlpkt_nd_act_get(unit, port, pAct_ctrl);
}


/**
 * @internal      yt_ctrlpkt_lldp_eee_act_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     act_ctrl            -l2 packet action,FWD,TRAP,DROP,COPY
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_ctrlpkt_lldp_eee_act_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_l2_action_t act_ctrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((L2_ACTION_COPY < act_ctrl || L2_ACTION_FWD > act_ctrl), CMM_ERR_INPUT);
        
    return YT_DISPATCH(unit)->ctrlpkt_lldp_eee_act_set(unit, port, act_ctrl);
}


/**
 * @internal      yt_ctrlpkt_lldp_eee_act_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pAct_ctrl           -l2 packet action,FWD,TRAP,DROP,COPY
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_ctrlpkt_lldp_eee_act_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_l2_action_t *pAct_ctrl)
    {
        CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
        CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
        CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
        CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
        CMM_PARAM_CHK((NULL == pAct_ctrl), CMM_ERR_NULL_POINT);
        
        return YT_DISPATCH(unit)->ctrlpkt_lldp_eee_act_get(unit, port, pAct_ctrl);
    }


/**
 * @internal      yt_ctrlpkt_lldp_act_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     act_ctrl            -l2 packet action,FWD,TRAP,DROP,COPY
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_ctrlpkt_lldp_act_set(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_l2_action_t act_ctrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((L2_ACTION_COPY < act_ctrl || L2_ACTION_FWD > act_ctrl), CMM_ERR_INPUT);
        
    return YT_DISPATCH(unit)->ctrlpkt_lldp_act_set(unit, port, act_ctrl);
}


/**
 * @internal      yt_ctrlpkt_lldp_act_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pAct_ctrl           -l2 packet action,FWD,TRAP,DROP,COPY
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_ctrlpkt_lldp_act_get(yt_unit_t unit, yt_port_t port, yt_ctrlpkt_l2_action_t *pAct_ctrl)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pAct_ctrl), CMM_ERR_NULL_POINT);
        
    return YT_DISPATCH(unit)->ctrlpkt_lldp_act_get(unit, port, pAct_ctrl);
}
