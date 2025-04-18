/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_acl.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_acl.h"
#include "fal_dispatch.h"


/**
 * @internal      yt_acl_init
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_acl_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->acl_init(unit);
}

/**
 * @internal      yt_acl_port_en_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_acl_port_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->acl_port_en_set(unit, port, enable);
}

/**
 * @internal      yt_acl_port_en_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable            -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_acl_port_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->acl_port_en_get(unit, port, pEnable);
}

/**
 * @internal      yt_acl_unmatch_permit_en_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_acl_unmatch_permit_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->acl_unmatch_permit_en_set(unit, port, enable);
}

/**
 * @internal      yt_acl_unmatch_permit_en_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable            -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_acl_unmatch_permit_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->acl_unmatch_permit_en_get(unit, port, pEnable);
}

/**
 * @internal      yt_acl_udf_rule_set
 * @endinternal
 *
 * @brief         set user define rule to specific acl udf table.
 * @brief         every table will take 2 bytes data,X*2 and X*2+1 udf table used for YT_IGRACL_TEMPLATE_UDF_X.
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     index               -acl udf table index,range (0~15).
 * @param[in]     type                -acl user define data type
 * @param[in]     offset              -offset start from specific type,range (0~127)
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_acl_udf_rule_set(yt_unit_t unit, uint8_t index, yt_acl_udf_type_t type, uint8_t offset)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((ACL_UDF_TYPE_L4 < type || ACL_UDF_TYPE_RAW > type), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->acl_udf_rule_set(unit, index, type, offset);
}

/**
 * @internal      yt_acl_rule_init
 * @endinternal
 *
 * @brief         start to init rule,must init before add rule key
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_acl_rule_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->acl_rule_init(unit);
}

/**
 * @internal      yt_acl_rule_reset
 * @endinternal
 *
 * @brief         clear acl rule key and action,used to recreate rule before rule active
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_acl_rule_reset(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->acl_rule_reset(unit);
}

/**
 * @internal      yt_acl_rule_key_add
 * @endinternal
 *
 * @brief         add key to list,prepare to create rule.max 16 keys
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                -acl key type,refer to yt_igrAcl_key_type_t define
 * @param[in]     pKey_data           -point to different key data,must according to key type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_NOT_INIT        -did not init rule yet
 * @retval        CMM_ERR_NOT_SUPPORT        -unknown key type
 * @retval        CMM_ERR_ENTRY_FULL        -reach to max num of keys
 * @retval        CMM_ERR_SAMEENTRY_EXIST        -already has same key
 */
yt_ret_t yt_acl_rule_key_add(yt_unit_t unit, yt_igrAcl_key_type_t type, yt_comm_key_t *pKey_data)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pKey_data), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_IGRACL_TEMPLATE_MAX < type || YT_IGRACL_TEMPLATE_MAC_DA > type), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->acl_rule_key_add(unit, type, pKey_data);
}

/**
 * @internal      yt_acl_rule_action_add
 * @endinternal
 *
 * @brief         add acl action before acl rule active
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                  -acl action type,refer to yt_acl_action_type_t define
 * @param[in]     pAction             -point to acl action settings,must according to action type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_NOT_INIT        -did not init rule yet
 * @retval        CMM_ERR_NOT_SUPPORT        -unknown action type
 */
yt_ret_t yt_acl_rule_action_add(yt_unit_t unit, yt_acl_action_type_t type, yt_comm_act_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((ACL_ACT_TYPE_MAX < type || ACL_ACT_TYPE_FWD > type), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->acl_rule_action_add(unit, type, pAction);
}

/**
 * @internal      yt_acl_rule_create
 * @endinternal
 *
 * @brief         create acl rule after add rule key data
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     rulePri             -acl rule priority,range 0~511
 * @param[in]     ruleReverse         -if reverse rule logic
 * @param[out]    pId                 -acl rule id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_acl_rule_create(yt_unit_t unit, uint16_t rulePri, yt_bool_t ruleReverse, uint32_t *pId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pId), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->acl_rule_create(unit, rulePri, ruleReverse, pId);
}

/**
 * @internal      yt_acl_rule_active
 * @endinternal
 *
 * @brief         active acl rule and related action
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     id                  -acl rule id created by yt_acl_rule_create
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_acl_rule_active(yt_unit_t unit, uint32_t id)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->acl_rule_active(unit, id);
}

/**
 * @internal      yt_acl_rule_del
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     id                  -acl rule id created by yt_acl_rule_create
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_acl_rule_del(yt_unit_t unit, uint32_t id)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->acl_rule_del(unit, id);
}
