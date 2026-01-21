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

#ifdef SWITCH_SERIES_TIGER

yt_ret_t yt_acl_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->acl_init(unit);
}

yt_ret_t  yt_acl_port_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_port_en_set(unit, port, enable);
}

yt_ret_t  yt_acl_port_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->acl_port_en_get(unit, port, pEnable);
}

yt_ret_t  yt_acl_unmatch_permit_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_unmatch_permit_en_set(unit, port, enable);
}

yt_ret_t  yt_acl_unmatch_permit_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->acl_unmatch_permit_en_get(unit, port, pEnable);
}

yt_ret_t yt_acl_udf_rule_set(yt_unit_t unit, uint8_t index, yt_acl_udf_type_t type, uint8_t offset)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ACL_UDF_TYPE_L4 < type || YT_ACL_UDF_TYPE_RAW > type), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_udf_rule_set(unit, index, type, offset);
}

yt_ret_t yt_acl_rule_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->acl_rule_init(unit);
}

yt_ret_t yt_acl_rule_reset(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->acl_rule_reset(unit);
}

yt_ret_t yt_acl_rule_key_add(yt_unit_t unit, yt_igrAcl_key_type_t type, yt_acl_comm_key_t *pKeyData)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pKeyData), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_IGRACL_TEMPLATE_MAX < type || YT_IGRACL_TEMPLATE_MAC_DA > type), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_rule_key_add(unit, type, pKeyData);
}

yt_ret_t yt_acl_rule_action_add(yt_unit_t unit, yt_acl_action_type_t type, yt_acl_comm_act_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_ACL_ACT_TYPE_MAX < type || YT_ACL_ACT_TYPE_FWD > type), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_rule_action_add(unit, type, pAction);
}

yt_ret_t yt_acl_rule_create(yt_unit_t unit, uint16_t rulePri, yt_bool_t ruleReverse, uint32_t *pId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pId), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->acl_rule_create(unit, rulePri, ruleReverse, pId);
}

yt_ret_t yt_acl_rule_active(yt_unit_t unit, uint32_t id)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->acl_rule_active(unit, id);
}

yt_ret_t yt_acl_rule_del(yt_unit_t unit, uint32_t id)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->acl_rule_del(unit, id);
}
#endif

#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)

yt_ret_t yt_acl_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->acl_init(unit);
}

yt_ret_t yt_acl_deinit(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->acl_deinit(unit);
}

yt_ret_t yt_acl_global_en_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_global_en_set(unit, enable);
}

yt_ret_t yt_acl_global_en_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->acl_global_en_get(unit, pEnable);
}

yt_ret_t yt_acl_port_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_port_en_set(unit, port, enable);
}

yt_ret_t yt_acl_portlist_en_set(yt_unit_t unit, yt_port_mask_t portMask, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_portlist_en_set(unit, portMask, enable);
}

yt_ret_t yt_acl_port_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->acl_port_en_get(unit, port, pEnable);
}

yt_ret_t yt_acl_unmatch_fwd_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_unmatch_fwd_en_set(unit, port, enable);
}

yt_ret_t yt_acl_unmatch_fwd_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->acl_unmatch_fwd_en_get(unit, port, pEnable);
}

yt_ret_t yt_acl_redirect_force_en_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_redirect_force_en_set(unit, enable);
}

yt_ret_t yt_acl_redirect_force_en_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->acl_redirect_force_en_get(unit, pEnable);
}

yt_ret_t yt_acl_instance_create(yt_unit_t unit, yt_acl_instance_pri_t prio,
    yt_acl_key_mask_t keyMask, yt_acl_instance_id_t *pInstanceId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_PRI_MAX <= prio || YT_ACL_INSTANCE_PRI0 > prio), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((pInstanceId == NULL), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->acl_instance_create(unit, prio, keyMask, pInstanceId);
}

yt_ret_t yt_acl_instance_get(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_instance_t **pInsData)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pInsData), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_instance_get(unit, instanceId, pInsData);
}

yt_ret_t yt_acl_instance_exist_check(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_bool_t *pStatus)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pStatus), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_instance_exist_check(unit, instanceId, pStatus);
}

yt_ret_t yt_acl_instance_delete(yt_unit_t unit, yt_acl_instance_id_t instanceId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_instance_delete(unit, instanceId);
}

yt_ret_t yt_acl_entry_create(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t *pEntryId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((pEntryId == NULL), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->acl_entry_create(unit, instanceId, pEntryId);
}

yt_ret_t yt_acl_entry_delete(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId || entryId >= YT_ACL_IDX_MAX), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_entry_delete(unit, instanceId, entryId);
}

yt_ret_t yt_acl_entry_get(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId, yt_acl_entry_t **pEntry)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId || entryId >= YT_ACL_IDX_MAX), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pEntry), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->acl_entry_get(unit, instanceId, entryId, pEntry);
}

yt_ret_t yt_acl_entry_destory(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId || entryId >= YT_ACL_IDX_MAX), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_entry_destory(unit, instanceId, entryId);
}

yt_ret_t yt_acl_entry_active(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId)

{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId || entryId >= YT_ACL_IDX_MAX), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_entry_active(unit, instanceId, entryId);
}

yt_ret_t yt_acl_entry_inactive(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId)

{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId || entryId >= YT_ACL_IDX_MAX), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_entry_inactive(unit, instanceId, entryId);
}

yt_ret_t yt_acl_entry_pri_set(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId, uint32_t prio)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId || entryId >= YT_ACL_IDX_MAX), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_entry_pri_set(unit, instanceId, entryId, prio);
}

yt_ret_t yt_acl_key_set(yt_unit_t unit, yt_acl_instance_id_t instanceId,
   yt_acl_entry_id_t entryId, yt_acl_rule_key_t keyType,
    yt_acl_comm_key_t *pKeyData)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId || entryId >= YT_ACL_IDX_MAX), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_ACL_KEY_SELECT_MAX <= keyType || YT_ACL_KEY_SELECT_LAG_PORT_FLAG > keyType), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((pKeyData == NULL), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->acl_key_set(unit, instanceId, entryId, keyType, pKeyData);
}

yt_ret_t yt_acl_key_get(yt_unit_t unit, yt_acl_instance_id_t instanceId,
   yt_acl_entry_id_t entryId, yt_acl_rule_key_t keyType,
    yt_acl_comm_key_t *pKeyData)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId || entryId >= YT_ACL_IDX_MAX), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ACL_KEY_SELECT_MAX <= keyType || YT_ACL_KEY_SELECT_LAG_PORT_FLAG > keyType), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((pKeyData == NULL), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->acl_key_get(unit, instanceId, entryId, keyType, pKeyData);
}

yt_ret_t yt_acl_key_clear(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId || entryId >= YT_ACL_IDX_MAX), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_key_clear(unit, instanceId, entryId);
}

yt_ret_t yt_acl_action_add(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId, yt_acl_action_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((pAction == NULL), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId || entryId >= YT_ACL_IDX_MAX), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_action_add(unit, instanceId, entryId, pAction);
}

yt_ret_t yt_acl_action_get(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId, yt_acl_action_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((pAction == NULL), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId || entryId >= YT_ACL_IDX_MAX), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_action_get(unit, instanceId, entryId, pAction);
}

yt_ret_t yt_acl_action_del(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId || entryId >= YT_ACL_IDX_MAX), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_action_del(unit, instanceId, entryId);
}

yt_ret_t yt_acl_range_key_add(yt_unit_t unit, yt_acl_range_type_t rangeType,
    yt_acl_rangeKey_t *pRangeKeyData, uint16_t *pGenKey)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((rangeType >= YT_ACL_RANGE_TYPE_MAX), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((pRangeKeyData == NULL || pGenKey == NULL), CMM_ERR_NULL_POINT);
    return YT_DISPATCH(unit)->acl_range_key_add(unit, rangeType, pRangeKeyData, pGenKey);
}

yt_ret_t yt_acl_range_key_get(yt_unit_t unit, yt_acl_range_type_t rangeType,
    uint16_t genKey, yt_acl_rangeKey_t *pRangeKeyData)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((rangeType >= YT_ACL_RANGE_TYPE_MAX), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((pRangeKeyData == NULL), CMM_ERR_NULL_POINT);
    return YT_DISPATCH(unit)->acl_range_key_get(unit, rangeType, genKey, pRangeKeyData);
}

yt_ret_t yt_acl_range_key_del(yt_unit_t unit, yt_acl_range_type_t rangeType,
    uint16_t genKey)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((rangeType >= YT_ACL_RANGE_TYPE_MAX), CMM_ERR_EXCEED_RANGE);
    return YT_DISPATCH(unit)->acl_range_key_del(unit, rangeType, genKey);
}

yt_ret_t yt_acl_udf_add(yt_unit_t unit, yt_acl_udf_t *pUdfSet,
    uint16_t *pGenKey)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((pUdfSet == NULL || pGenKey == NULL), CMM_ERR_NULL_POINT);
    return YT_DISPATCH(unit)->acl_udf_add(unit, pUdfSet, pGenKey);
}

yt_ret_t yt_acl_udf_get(yt_unit_t unit, uint16_t genKey, yt_acl_udf_t *pUdfSet)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((pUdfSet == NULL), CMM_ERR_EXCEED_RANGE);
    return YT_DISPATCH(unit)->acl_udf_get(unit, genKey, pUdfSet);
}

yt_ret_t yt_acl_udf_exist_check(yt_unit_t unit, uint16_t genKey, yt_bool_t *pStatus)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(pStatus == NULL, CMM_ERR_NULL_POINT);
    return YT_DISPATCH(unit)->acl_udf_check_exist(unit, genKey, pStatus);
}

yt_ret_t yt_acl_udf_del(yt_unit_t unit, uint16_t genKey)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    return YT_DISPATCH(unit)->acl_udf_del(unit, genKey);
}

#ifdef SWITCH_SERIES_SHARK
yt_ret_t yt_acl_hit_en_set(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t startEntryId, yt_acl_entry_id_t endEntryId,
    yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId || startEntryId >= YT_ACL_IDX_MAX || endEntryId >= YT_ACL_IDX_MAX), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((startEntryId > endEntryId), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_hit_en_set(unit, instanceId, startEntryId, endEntryId, enable);
}

yt_ret_t yt_acl_hit_en_get(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId || entryId >= YT_ACL_IDX_MAX), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_hit_en_get(unit, instanceId, entryId, pEnable);
}
#else
yt_ret_t yt_acl_hit_en_set(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_hit_en_set(unit, instanceId, enable);
}

yt_ret_t yt_acl_hit_en_get(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_hit_en_get(unit, instanceId, pEnable);
}
#endif

yt_ret_t yt_acl_hit_get(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId, yt_bool_t *pHitStatus)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(pHitStatus == NULL, CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ACL_INSTANCE_NUM_MAX <= instanceId || entryId >= YT_ACL_IDX_MAX), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->acl_hit_get(unit, instanceId, entryId, pHitStatus);
}

#endif

