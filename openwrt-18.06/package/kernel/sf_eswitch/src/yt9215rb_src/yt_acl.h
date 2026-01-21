/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_acl.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_ACL_H__
#define __YT_ACL_H__


#include "yt_cmm.h"
#include "yt_vlan_translate.h"

#ifdef SWITCH_SERIES_TIGER

#define YT_ACL_UDF_DATA_LEN_MAX 4
#define YT_ACL_UDF_NUM_MAX  16
#define YT_ACL_BITWID   32
#define YT_ACL_KEY_BIT_SIZE(max)    ((max+31)/YT_ACL_BITWID)

#ifdef SWITCH_MAX_ACL_NUM
#define YT_ACL_IDX_MAX SWITCH_MAX_ACL_NUM
#else
#define YT_ACL_IDX_MAX 384
#endif

typedef enum yt_igrAcl_key_type_e
{
    YT_IGRACL_TEMPLATE_MAC_DA=1,
    YT_IGRACL_TEMPLATE_MAC_SA,
    YT_IGRACL_TEMPLATE_L2_TYPE,
    YT_IGRACL_TEMPLATE_L3_TYPE,
    YT_IGRACL_TEMPLATE_CDEI,
    YT_IGRACL_TEMPLATE_CPRI=6,
    YT_IGRACL_TEMPLATE_CTAG_FMT,
    YT_IGRACL_TEMPLATE_SDEI,
    YT_IGRACL_TEMPLATE_SPRI,
    YT_IGRACL_TEMPLATE_STAG_FMT,
    YT_IGRACL_TEMPLATE_SVID=11,
    YT_IGRACL_TEMPLATE_CVID,
    YT_IGRACL_TEMPLATE_IPV4_DA,
    YT_IGRACL_TEMPLATE_IPV4_SA,
    YT_IGRACL_TEMPLATE_L4_DPORT,
    YT_IGRACL_TEMPLATE_L4_SPORT=16,
    YT_IGRACL_TEMPLATE_L4_TYPE,
    YT_IGRACL_TEMPLATE_IP_FRAGMENT,
    YT_IGRACL_TEMPLATE_IP_1ST_FRAGMENT,
    YT_IGRACL_TEMPLATE_IPV6_DA,
    YT_IGRACL_TEMPLATE_IPV6_SA=21,
    YT_IGRACL_TEMPLATE_IP_OPTION,
    YT_IGRACL_TEMPLATE_PPPOE_FLAG,
    YT_IGRACL_TEMPLATE_TCP_FLAGS,
    YT_IGRACL_TEMPLATE_IP_PROTOCOL,
    YT_IGRACL_TEMPLATE_TOS=26,
    YT_IGRACL_TEMPLATE_IS_IGMP,
    YT_IGRACL_TEMPLATE_UDF_0,
    YT_IGRACL_TEMPLATE_UDF_1,
    YT_IGRACL_TEMPLATE_UDF_2,
    YT_IGRACL_TEMPLATE_UDF_3=31,
    YT_IGRACL_TEMPLATE_UDF_4,
    YT_IGRACL_TEMPLATE_UDF_5,
    YT_IGRACL_TEMPLATE_UDF_6,
    YT_IGRACL_TEMPLATE_UDF_7,
    YT_IGRACL_TEMPLATE_INNER_CVID=36,
    YT_IGRACL_TEMPLATE_INNER_SVID,
    YT_IGRACL_TEMPLATE_INNER_SPRI,
    YT_IGRACL_TEMPLATE_INNER_CPRI,
    YT_IGRACL_TEMPLATE_INNER_SDEI,
    YT_IGRACL_TEMPLATE_INNER_CDEI=41,
    YT_IGRACL_TEMPLATE_ETHER_TYPE,
    YT_IGRACL_TEMPLATE_SRC_PORTMASK,
    YT_IGRACL_TEMPLATE_MAX,
}yt_igrAcl_key_type_t;

typedef enum yt_acl_l3_type_e
{
    YT_ACL_L3_NONE, /*except all below*/
    YT_ACL_L3_IPV4,
    YT_ACL_L3_IPV6,
    YT_ACL_L3_IPV4ARP,
    YT_ACL_L3_LLDP,
    YT_ACL_L3_EAPOL,
    YT_ACL_L3_ERP,
    YT_ACL_L3_SLOW_PROTOCOL,
}yt_acl_l3_type_t;

typedef enum yt_acl_l4_type_e
{
    YT_ACL_L4_NONE,/*except all below*/
    YT_ACL_L4_TCP,
    YT_ACL_L4_UDP,
    YT_ACL_L4_UDP_LITE,
    YT_ACL_L4_ICMP,
    YT_ACL_L4_IGMP,
    YT_ACL_L4_MLD,
    YT_ACL_L4_ND,
}yt_acl_l4_type_t;

typedef enum yt_acl_udf_type_e
{
    YT_ACL_UDF_TYPE_RAW,
    YT_ACL_UDF_TYPE_L3,
    YT_ACL_UDF_TYPE_L4,
}yt_acl_udf_type_t;

typedef enum yt_acl_fwd_type_e
{
    YT_ACL_FWD_TYPE_FWD,
    YT_ACL_FWD_TYPE_COPY,
    YT_ACL_FWD_TYPE_REDIRECT,
    YT_ACL_FWD_TYPE_TRAP,
    YT_ACL_FWD_TYPE_DROP,
}yt_acl_fwd_type_t;

typedef struct yt_acl_key_mac_s
{
    yt_mac_addr_t mac_data;
    yt_mac_addr_t mac_mask;
}yt_acl_key_mac_t;/*for s_mac, d_mac*/

typedef struct yt_acl_key_etherType_s
{
    uint16_t type_data_min;
    uint16_t type_mask_max;
    yt_bool_t range_en;
}yt_acl_key_etherType_t;

typedef struct yt_acl_key_l2Type_s
{
    yt_l2_type_t type_data;
    uint8_t type_mask;
}yt_acl_key_l2Type_t;

typedef struct yt_acl_key_l3Type_s
{
    yt_acl_l3_type_t type_data;
    uint8_t type_mask;
}yt_acl_key_l3Type_t;

typedef struct yt_acl_key_l4Type_s
{
    yt_acl_l4_type_t type_data;
    uint8_t type_mask;
}yt_acl_key_l4Type_t;

typedef struct yt_acl_key_dei_s
{
    yt_bool_t dei_data;
    yt_bool_t dei_mask;
}yt_acl_key_dei_t;

typedef struct yt_acl_key_pri_s
{
    yt_pri_t pri_data;
    uint8_t pri_mask;
}yt_acl_key_pri_t;

typedef struct yt_acl_key_vlanFmt_s
{
    yt_vlan_format_t tagfmt_data;
    uint8_t tagfmt_mask;
}yt_acl_key_vlanFmt_t;

typedef struct yt_acl_key_vid_s
{
    uint16_t vid_data_min;/*as min value if range_en*/
    uint16_t vid_mask_max;/*as max value if range_en*/
    yt_bool_t range_en;
}yt_acl_key_vid_t;

typedef struct yt_acl_key_ipv4_s
{
    uint32_t ip_data_min;/*as min value if range_en*/
    uint32_t ip_mask_max;/*as max value if range_en*/
    yt_bool_t range_en;
}yt_acl_key_ipv4_t;

typedef struct yt_acl_key_l4Port_s
{
    uint16_t port_data_min;
    uint16_t port_mask_max;
    yt_bool_t range_en;
}yt_acl_key_l4Port_t;

typedef struct yt_acl_key_isPppoe_s
{
    yt_bool_t is_pppoe_data;
    yt_bool_t is_pppoe_mask;
}yt_acl_key_isPppoe_t;

typedef struct yt_acl_key_tcpFlag_s
{
    uint8_t flag_data;
    uint8_t flag_mask;
}yt_acl_key_tcpFlag_t;

typedef struct yt_acl_key_isIgmp_s
{
    yt_bool_t is_igmp_data;
    yt_bool_t is_igmp_mask;
}yt_acl_key_isIgmp_t;

typedef struct yt_acl_key_ipFrag_s
{
    yt_bool_t is_frag_data;
    yt_bool_t is_frag_mask;
}yt_acl_key_ipFrag_t;

typedef struct yt_acl_key_1stIpFrag_s
{
    yt_bool_t is_1stFrag_data;
    yt_bool_t is_1stFrag_mask;
}yt_acl_key_1stIpFrag_t;

typedef struct yt_acl_key_ipOption_s
{
    yt_bool_t is_option_data;
    yt_bool_t is_option_mask;
}yt_acl_key_ipOption_t;

typedef struct yt_acl_key_ipProtocol_s
{
    uint8_t protocol_data;
    uint8_t protocol_mask;
}yt_acl_key_ipProtocol_t;

typedef struct yt_acl_key_ipTOS_s
{
    uint8_t tos_data;
    uint8_t tos_mask;
}yt_acl_key_ipTOS_t;

typedef struct yt_acl_key_udf_s
{
    uint8_t udf_data[YT_ACL_UDF_DATA_LEN_MAX];
    uint8_t udf_mask[YT_ACL_UDF_DATA_LEN_MAX];
}yt_acl_key_udf_t;

typedef struct yt_acl_key_ipv6_s
{
    uint8_t ipv6_data_min[IP6_ADDR_LEN];
    uint8_t ipv6_mask_max[IP6_ADDR_LEN];
    yt_bool_t range_en;
}yt_acl_key_ipv6_t;

typedef yt_port_mask_t yt_acl_key_srcPortmask_t;
typedef void yt_acl_comm_key_t;
typedef void yt_acl_comm_act_t;

typedef enum yt_acl_vlan_assign_mode_e
{
	YT_ACL_VLANASSIGN_MODE_INVAILD,
	YT_ACL_VLANASSIGN_MODE_UNTAG,
	YT_ACL_VLANASSIGN_MODE_TAG,
	YT_ACL_VLANASSIGN_MODE_KEEP,
}yt_acl_vlan_assign_mode_t;

typedef enum yt_acl_action_type_e
{
	YT_ACL_ACT_TYPE_FWD,
	YT_ACL_ACT_TYPE_INTPRI_MAP,
	YT_ACL_ACT_TYPE_VID_REPLACE,
	YT_ACL_ACT_TYPE_PRI_REPLACE,
	YT_ACL_ACT_TYPE_VLAN_ASSIGN,
	YT_ACL_ACT_TYPE_DSCP_REPLACE,
	YT_ACL_ACT_TYPE_METER_ASSIGN,
	YT_ACL_ACT_TYPE_FLOWSTAT,
	YT_ACL_ACT_TYPE_MIRROR_ENABLE,
	YT_ACL_ACT_TYPE_BYPASS_ENABLE,
	YT_ACL_ACT_TYPE_MAX
}yt_acl_action_type_t;

typedef struct yt_acl_action_fwd_s
{
    yt_bool_t fwd_en;
    yt_acl_fwd_type_t fwd_type;
    yt_port_mask_t dst_portmask;/*used for FWD and REDIRECT,all zero for DROP type*/
}yt_acl_action_fwd_t;

typedef struct yt_acl_action_internalPriMap_s
{
    yt_bool_t int_dp_en; /*enable internal drop priority map*/
    yt_bool_t int_pri_en; /*enable internal priority map*/
    uint8 int_dp; /*internal drop priority,0~3*/
    uint8 int_pri; /*internal priority,0~7*/
}yt_acl_action_internalPriMap_t;

typedef struct yt_acl_action_vid_replace_s
{
    yt_bool_t cvid_replace_en;
    yt_bool_t svid_replace_en;
    yt_bool_t fwd_cvid_en;
    yt_bool_t fwd_svid_en;
    yt_vlan_t cvid;
    yt_vlan_t svid;
}yt_acl_action_vid_replace_t;

typedef struct yt_acl_action_pri_replace_s
{
    yt_bool_t cpri_replace_en;
    yt_bool_t cdei_replace_en;
    yt_pri_t cpri;
    yt_bool_t cdei;
    yt_bool_t spri_replace_en;
    yt_bool_t sdei_replace_en;
    yt_pri_t spri;
    yt_bool_t sdei;
}yt_acl_action_pri_replace_t;

typedef struct yt_acl_action_vlan_assign_s
{
	yt_acl_vlan_assign_mode_t ctag_assign_mode;
	yt_acl_vlan_assign_mode_t stag_assign_mode;
}yt_acl_action_vlan_assign_t;

typedef struct yt_acl_action_dscp_replace_s
{
    yt_bool_t dscp_replace_en;
    yt_dscp_t dscp;
}yt_acl_action_dscp_replace_t;

typedef struct yt_acl_action_meter_assign_s
{
    yt_bool_t meter_en;
    yt_meterid_t meter_id;
}yt_acl_action_meter_assign_t;

typedef struct yt_acl_action_flowStats_s
{
    yt_bool_t flow_stats_en;
    uint8_t flow_stats_id; /*flow stat table id*/
}yt_acl_action_flowStats_t;

typedef struct yt_acl_action_mirrorEn_s
{
    yt_bool_t mirror_en;
}yt_acl_action_mirrorEn_t;

typedef struct yt_acl_action_bypass_s
{
    yt_bool_t cpu_ctrl_bypass_en;
}yt_acl_action_bypass_t;

/**
 * @internal      yt_acl_init
 * @endinternal
 *
 * @brief         acl module init,enable unmatch permit on port by default
 * @note          APPLICABLE DEVICES    -Tiger
 * @param[in]     unit                  -unit id
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 */
extern yt_ret_t yt_acl_init(yt_unit_t unit);

/**
 * @internal      yt_acl_port_en_set
 * @endinternal
 *
 * @brief         enable or disable acl function on port
 * @note          APPLICABLE DEVICES    -Tiger
 * @param[in]     unit                  -unit id
 * @param[in]     port                  -port num
 * @param[in]     enable                -enable or disable
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_PORT          -input port err
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_acl_port_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_acl_port_en_get
 * @endinternal
 *
 * @brief         get acl enable state on port
 * @note          APPLICABLE DEVICES    -Tiger
 * @param[in]     unit                  -unit id
 * @param[in]     port                  -port num
 * @param[out]    pEnable               -enable or disable
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_PORT          -input port err
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 */
extern yt_ret_t yt_acl_port_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_acl_unmatch_permit_en_set
 * @endinternal
 *
 * @brief         enable or disable forward permit when unmatch acl rule on port
 * @note          APPLICABLE DEVICES    -Tiger
 * @param[in]     unit                  -unit id
 * @param[in]     port                  -port num
 * @param[in]     enable                -enable or disable
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_PORT          -input port err
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_acl_unmatch_permit_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_acl_unmatch_permit_en_get
 * @endinternal
 *
 * @brief         get enable state of forward permit when unmatch acl rule on port
 * @note          APPLICABLE DEVICES    -Tiger
 * @param[in]     unit                  -unit id
 * @param[in]     port                  -port num
 * @param[out]    pEnable               -enable or disable
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_PORT          -input port err
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 */
extern yt_ret_t  yt_acl_unmatch_permit_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_acl_udf_rule_set
 * @endinternal
 *
 * @brief         set user define rule to specific acl udf table.
 * @brief         every table will take 2 bytes data,X*2 and X*2+1 udf table used for YT_IGRACL_TEMPLATE_UDF_X.
 * @note          APPLICABLE DEVICES    -Tiger
 * @param[in]     unit                  -unit id
 * @param[in]     index                 -acl udf table index,range (0~15).
 * @param[in]     type                  -acl user define data type
 * @param[in]     offset                -offset start from specific type,range (0~127)
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_acl_udf_rule_set(yt_unit_t unit, uint8_t  index, yt_acl_udf_type_t type, uint8_t offset);

/**
 * @internal      yt_acl_rule_init
 * @endinternal
 *
 * @brief         start to init rule,must init before add rule key
 * @note          APPLICABLE DEVICES    -Tiger
 * @param[in]     unit                  -unit id
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 */
extern yt_ret_t yt_acl_rule_init(yt_unit_t unit);

/**
 * @internal      yt_acl_rule_reset
 * @endinternal
 *
 * @brief         clear acl rule key and action,used to recreate rule before rule active
 * @note          APPLICABLE DEVICES    -Tiger
 * @param[in]     unit                  -unit id
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 */
extern yt_ret_t yt_acl_rule_reset(yt_unit_t unit);

/**
 * @internal      yt_acl_rule_key_add
 * @endinternal
 *
 * @brief         add key to list,prepare to create rule.max 16 keys
 * @note          APPLICABLE DEVICES    -Tiger
 * @param[in]     unit                  -unit id
 * @param[in]     type                  -acl key type,refer to yt_igrAcl_key_type_t define
 * @param[in]     pKeyData              -point to different key data,must according to key type
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_NOT_SUPPORT   -current type not support
 * @retval        CMM_ERR_PORTLIST      -error src port mask
 * @retval        CMM_ERR_ENTRY_FULL    -full acl entry based on different key type
 * @retval        CMM_ERR_SAMEENTRY_EXIST    -exist same acl key type
 */
extern yt_ret_t yt_acl_rule_key_add(yt_unit_t unit, yt_igrAcl_key_type_t type, yt_acl_comm_key_t *pKeyData);

/**
 * @internal      yt_acl_rule_action_add
 * @endinternal
 *
 * @brief         add acl action before acl rule active
 * @note          APPLICABLE DEVICES    -Tiger
 * @param[in]     unit                  -unit id
 * @param[in]     type                  -acl action type,refer to yt_acl_action_type_t define
 * @param[in]     pAction               -point to acl action settings,must according to action type
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_NOT_SUPPORT   -current type not support
 */
extern yt_ret_t yt_acl_rule_action_add(yt_unit_t unit, yt_acl_action_type_t type, yt_acl_comm_act_t *pAction);

/**
 * @internal      yt_acl_rule_create
 * @endinternal
 *
 * @brief         create acl rule after add rule key data
 * @note          APPLICABLE DEVICES    -Tiger
 * @param[in]     unit                  -unit id
 * @param[in]     rulePri               -acl rule priority,range 0~511
 * @param[in]     ruleReverse           -if reverse rule logic
 * @param[out]    pId                   -acl rule id
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_TOO_LESS_INFO -on too less info in the rule
 * @retval        CMM_ERR_ENTRY_FULL    -full acl entry based on different key type
 * @retval        CMM_ERR_SAMEENTRY_EXIST    -exist same acl key type
 */
extern yt_ret_t yt_acl_rule_create(yt_unit_t unit, uint16_t rulePri, yt_bool_t ruleReverse, uint32_t *pId);

/**
 * @internal      yt_acl_rule_active
 * @endinternal
 *
 * @brief         active acl rule and related action
 * @note          APPLICABLE DEVICES    -Tiger
 * @param[in]     unit                  -unit id
 * @param[in]     id                    -acl rule id created by yt_acl_rule_create
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_ENTRY_NOT_FOUND   -rule not found
 */
extern yt_ret_t yt_acl_rule_active(yt_unit_t unit, uint32_t id);

/**
 * @internal      yt_acl_rule_del
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES    -Tiger
 * @param[in]     unit                  -unit id
 * @param[in]     id                    -acl rule id created by yt_acl_rule_create
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_ENTRY_NOT_FOUND   -rule not found
 */
extern yt_ret_t yt_acl_rule_del(yt_unit_t unit, uint32_t id);
#endif

#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
#define YT_ACL_IP_RANGE_DEPTH   4
#define YT_ACL_UDF_DATA_LEN_MAX 4
#define YT_ACL_UDF_NUM_MAX      16
#define YT_ACL_ENTRY_NUM_MAX    4
#define YT_ACL_IDX_MAX          YT_ACL_NUM_MAX
#define YT_ACL_INSTANCE_ID_BASE     0
#define YT_ACL_INSTANCE_NUM_MAX     16
#define YT_ACL_INSTANCE_INVALID_ID  0xFF
#define YT_ACL_INSTANCE_PATTERN_NUM_MAX 4
#define YT_ACL_BIN_ID_BASE          0
#define YT_ACL_BIN_NUM_MAX          16
#define YT_ACL_BITWID               32
#define YT_ACL_KEY_BIT_SIZE(max)    (max/YT_ACL_BITWID)
#define YT_ACLS_BIT_SIZE(max)       ((max + 31)/YT_ACL_BITWID)
#define YT_ACTS_BIT_SIZE            5
#define YT_ACL_SW_BIN_SIZE          (YT_ACL_IDX_MAX/YT_ACL_BIN_NUM_MAX)
#define YT_ACL_HW_BIN_SIZE          (YT_ACL_SW_BIN_SIZE*2)
#define YT_ACL_ACTION_EXIST_FLAG   (1<<0)
#define YT_ACL_INSTALL_HW_FALG     (1<<1)
#define YT_ACL_ENTRY_MODIFY_FLAG   (1<<2)
#define YT_ACL_ACTION_MODIFY_FLAG  (1<<3)
#define YT_ACL_ACT_INVALID_ID       0xFFF
#define YT_ACL_SW_ID_GET(instanceId, entryId)   (instanceId*YT_ACL_IDX_MAX + entryId)
#define YT_ACL_ENTRY_ID_GET(swEntryId)          (swEntryId%YT_ACL_IDX_MAX)
#define YT_ACL_INSTANCE_ID_GET(swEntryId)       (swEntryId/YT_ACL_IDX_MAX)

typedef void yt_acl_comm_key_t;
typedef enum yt_acl_udf_type_e
{
    YT_ACL_UDF_TYPE_RAW,
    YT_ACL_UDF_TYPE_L3,
    YT_ACL_UDF_TYPE_L4,
}yt_acl_udf_type_t;

typedef enum yt_acl_com_range_type_e
{
    YT_ACL_RANGE_TYPE_CVID,
    YT_ACL_RANGE_TYPE_SVID,
    YT_ACL_RANGE_TYPE_ORG_CVID,
    YT_ACL_RANGE_TYPE_ORG_SVID,
    YT_ACL_RANGE_TYPE_SPORT,
    YT_ACL_RANGE_TYPE_DPORT,
    YT_ACL_RANGE_TYPE_PKT_LEN,
    YT_ACL_RANGE_TYPE_LOOKUP_VID,
    YT_ACL_RANGE_TYPE_TTL,
    YT_ACL_RANGE_TYPE_CPRI,
    YT_ACL_RANGE_TYPE_SPRI,
    YT_ACL_RANGE_TYPE_ORG_CPRI,
    YT_ACL_RANGE_TYPE_ORG_SPRI,
}yt_acl_com_range_type_t;

typedef enum yt_acl_ip_range_type_e
{
    YT_ACL_RANGE_TYPE_IPV4_DA,
    YT_ACL_RANGE_TYPE_IPV4_SA,
    YT_ACL_RANGE_TYPE_IPV6_DA,
    YT_ACL_RANGE_TYPE_IPV6_DA_HALF,
    YT_ACL_RANGE_TYPE_IPV6_DA_ALL,
    YT_ACL_RANGE_TYPE_IPV6_SA,
    YT_ACL_RANGE_TYPE_IPV6_SA_HALF,
    YT_ACL_RANGE_TYPE_IPV6_SA_ALL,
}yt_acl_ip_range_type_t;

typedef enum yt_acl_range_type_e{
    YT_ACL_COMMON_RANGE_TYPE,
    YT_ACL_IP_RANGE_TYPE,
    YT_ACL_RANGE_TYPE_MAX,
}yt_acl_range_type_t;

typedef enum yt_acl_l3_type_e
{
    YT_ACL_L3_NONE, /*except all below*/
    YT_ACL_L3_IPV4,
    YT_ACL_L3_IPV6,
    YT_ACL_L3_IPV4ARP,
    YT_ACL_L3_LLDP,
    YT_ACL_L3_EAPOL,
    YT_ACL_L3_RRPP,
    YT_ACL_L3_SLOW_PROTOCOL,
    YT_ACL_L3_PPPOE,
    YT_ACL_L3_CFM_APS,
}yt_acl_l3_type_t;

typedef enum yt_acl_l4_type_e
{
    YT_ACL_L4_NONE,/*except all below*/
    YT_ACL_L4_TCP,
    YT_ACL_L4_UDP,
    YT_ACL_L4_UDP_LITE,
    YT_ACL_L4_ICMP,
    YT_ACL_L4_IGMP,
    YT_ACL_L4_MLD,
    YT_ACL_L4_ND,
}yt_acl_l4_type_t;

typedef enum yt_acl_ttl_type_e
{
    YT_ACL_TTL_0,
    YT_ACL_TTL_1,
    YT_ACL_TTL_2_254,
    YT_ACL_TTL_255,
}yt_acl_ttl_type_t;

typedef enum yt_acl_fwd_type_e
{
    YT_ACL_FWD_TYPE_FWD,
    YT_ACL_FWD_TYPE_COPY,
    YT_ACL_FWD_TYPE_REDIRECT,
    YT_ACL_FWD_TYPE_TRAP,
    YT_ACL_FWD_TYPE_DROP,
    YT_ACL_FWD_TYPE_PERMIT,
}yt_acl_fwd_type_t;

typedef enum yt_acl_vlan_assign_mode_e
{
	YT_ACL_VLANASSIGN_MODE_INVAILD,
	YT_ACL_VLANASSIGN_MODE_UNTAG,
	YT_ACL_VLANASSIGN_MODE_TAG,
	YT_ACL_VLANASSIGN_MODE_KEEP,
}yt_acl_vlan_assign_mode_t;

typedef struct yt_acl_udf_s{
    yt_acl_udf_type_t type;
    uint16_t offset;
    uint16_t data;
    uint16_t mask;
}yt_acl_udf_t;

typedef struct yt_acl_rangeKey_s{
    yt_acl_com_range_type_t com_range_type;
    yt_acl_ip_range_type_t ip_range_type;
    uint16_t com_min;
    uint16_t com_max;
    uint32_t ip_min[YT_ACL_IP_RANGE_DEPTH];
    uint32_t ip_max[YT_ACL_IP_RANGE_DEPTH];
}yt_acl_rangeKey_t;

typedef enum yt_acl_instance_mode_e{
    YT_ACL_INSTANCE_SINGLE_MODE = 0,
    YT_ACL_INSTANCE_COMBINE2_MODE,
    YT_ACL_INSTANCE_COMBINE4_MODE,
    YT_ACL_INSTANCE_MODE_MAX
}yt_acl_instance_mode_t;

typedef enum yt_acl_instance_pri_e{
    YT_ACL_INSTANCE_PRI0 = 0,
    YT_ACL_INSTANCE_PRI1,
    YT_ACL_INSTANCE_PRI2,
    YT_ACL_INSTANCE_PRI3,
    YT_ACL_INSTANCE_PRI4,
    YT_ACL_INSTANCE_PRI5,
    YT_ACL_INSTANCE_PRI6,
    YT_ACL_INSTANCE_PRI7,
    YT_ACL_INSTANCE_PRI8,
    YT_ACL_INSTANCE_PRI9,
    YT_ACL_INSTANCE_PRI10,
    YT_ACL_INSTANCE_PRI11,
    YT_ACL_INSTANCE_PRI12,
    YT_ACL_INSTANCE_PRI13,
    YT_ACL_INSTANCE_PRI14,
    YT_ACL_INSTANCE_PRI15,
    YT_ACL_INSTANCE_PRI_MAX
}yt_acl_instance_pri_t;

/* ACL Key Define*/
typedef enum yt_acl_rule_key_e{
    YT_ACL_KEY_SELECT_LAG_PORT_FLAG = 0,
    YT_ACL_KEY_SELECT_SINGLE_PORT = 1,
    YT_ACL_KEY_SELECT_SPM = 2,
    YT_ACL_KEY_SELECT_DMAC = 3,
    YT_ACL_KEY_SELECT_SMAC = 4,
    YT_ACL_KEY_SELECT_ETHER_TYPE = 5,
    YT_ACL_KEY_SELECT_INNER_VLAN_ID = 6,
    YT_ACL_KEY_SELECT_INNER_VLAN_PRI = 7,
    YT_ACL_KEY_SELECT_INNER_VLAN_CFI = 8,
    YT_ACL_KEY_SELECT_ORI_INNER_VLAN_ID = 9,
    YT_ACL_KEY_SELECT_ORI_INNER_VLAN_PRI = 10,
    YT_ACL_KEY_SELECT_ORI_INNER_VLAN_CFI = 11,
    YT_ACL_KEY_SELECT_OUTER_VLAN_ID = 12,
    YT_ACL_KEY_SELECT_OUTER_VLAN_PRI = 13,
    YT_ACL_KEY_SELECT_OUTER_VLAN_CFI = 14,
    YT_ACL_KEY_SELECT_ORI_OUTER_VLAN_ID = 15,
    YT_ACL_KEY_SELECT_ORI_OUTER_VLAN_PRI = 16,
    YT_ACL_KEY_SELECT_ORI_OUTER_VLAN_CFI = 17,
    YT_ACL_KEY_SELECT_DIP = 18,
    YT_ACL_KEY_SELECT_SIP = 19,
    YT_ACL_KEY_SELECT_DIP6 = 20,
    YT_ACL_KEY_SELECT_DIP6_HIGH = 21,
    YT_ACL_KEY_SELECT_DIP6_LOW = 22,
    YT_ACL_KEY_SELECT_SIP6 = 23,
    YT_ACL_KEY_SELECT_SIP6_HIGH = 24,
    YT_ACL_KEY_SELECT_SIP6_LOW = 25,
    YT_ACL_KEY_SELECT_L4_DPORT = 26,
    YT_ACL_KEY_SELECT_L4_SPORT = 27,
    YT_ACL_KEY_SELECT_L2_TYPE = 28,
    YT_ACL_KEY_SELECT_L3_TYPE = 29,
    YT_ACL_KEY_SELECT_L4_TYPE = 30,
    YT_ACL_KEY_SELECT_IP_PRECEDENCE = 31,
    YT_ACL_KEY_SELECT_TOS = 32,
    YT_ACL_KEY_SELECT_TRAFFIC_CLASS = 33,
    YT_ACL_KEY_SELECT_DSCP = 34,
    YT_ACL_KEY_SELECT_IP_PROTOCOL = 35,
    YT_ACL_KEY_SELECT_TCP_FLAGS = 36,
    YT_ACL_KEY_SELECT_STAG_FRAME = 37,
    YT_ACL_KEY_SELECT_CTAG_FRAME = 38,
    YT_ACL_KEY_SELECT_IP_FRAG = 39,
    YT_ACL_KEY_SELECT_IP_FIRST_FRAG = 40,
    YT_ACL_KEY_SELECT_IP_OPTION = 41,
    YT_ACL_KEY_SELECT_COMMON_RANGE = 42,
    YT_ACL_KEY_SELECT_IP_RANGE = 43,
    YT_ACL_KEY_SELECT_ICMP_TYPE = 44,
    YT_ACL_KEY_SELECT_ICMP_CODE = 45,
    YT_ACL_KEY_SELECT_ICMP6_TYPE = 46,
    YT_ACL_KEY_SELECT_ICMP6_CODE = 47,
    YT_ACL_KEY_SELECT_FWD_VID = 48,
    YT_ACL_KEY_SELECT_PPPOE_SESSION = 49,
    YT_ACL_KEY_SELECT_L3_HDERROR = 50,
    YT_ACL_KEY_SELECT_L4_HDERROR = 51,
    YT_ACL_KEY_SELECT_UDF = 52,
    YT_ACL_KEY_SELECT_IS_MGNT_VLAN = 53,
    YT_ACL_KEY_SELECT_L4_UNKNOWN_TYPE = 54,
    YT_ACL_KEY_SELECT_TTL_TYPE = 55,
    YT_ACL_KEY_SELECT_MAX
}yt_acl_rule_key_t;

typedef enum yt_acl_pattern_type_e
{
    YT_ACL_TEMPLATE_TYPE_IPV4,
    YT_ACL_TEMPLATE_TYPE_IPV6,
    YT_ACL_TEMPLATE_TYPE_NON_IP,
    YT_ACL_TEMPLATE_TYPE_MIX,
    YT_ACL_TEMPLATE_TYPE_ALL,
}yt_acl_pattern_type_t;

typedef struct yt_acl_key_mac_s
{
    yt_mac_addr_t mac_data;
    yt_mac_addr_t mac_mask;
}yt_acl_key_mac_t;

typedef struct yt_acl_key_etherType_s
{
    uint16_t type_data;
    uint16_t type_mask;
}yt_acl_key_etherType_t;

typedef yt_port_mask_t yt_acl_key_spm_t;

typedef struct yt_acl_key_l2Type_s
{
    yt_l2_type_t type_data;
    uint8_t type_mask;
}yt_acl_key_l2Type_t;

typedef struct yt_acl_key_l3Type_s
{
    yt_acl_l3_type_t type_data;
    uint8_t type_mask;
}yt_acl_key_l3Type_t;

typedef struct yt_acl_key_l4Type_s
{
    yt_acl_l4_type_t type_data;
    uint8_t type_mask;
}yt_acl_key_l4Type_t;

typedef struct yt_acl_key_isLag_s
{
    yt_bool_t is_lag_data;
    yt_bool_t is_lag_mask;
}yt_acl_key_isLag_t;

typedef struct yt_acl_key_port_s
{
    uint8_t port_data;
    uint8_t port_mask;
}yt_acl_key_port_t;

typedef struct yt_acl_key_ttl_s
{
    yt_acl_ttl_type_t type_data;
    uint8_t type_mask;
}yt_acl_key_ttl_t;

typedef struct yt_acl_key_l4UnKnowType_s
{
    yt_bool_t l4_unknow_data;
    yt_bool_t l4_unknow_mask;
}yt_acl_key_l4UnKnowType_t;

typedef struct yt_acl_key_dei_s
{
    yt_bool_t dei_data;
    yt_bool_t dei_mask;
}yt_acl_key_dei_t;

typedef struct yt_acl_key_pri_s
{
    yt_pri_t pri_data;
    uint8_t pri_mask;
}yt_acl_key_pri_t;

typedef struct yt_acl_key_vid_s
{
    uint16_t vid_data;
    uint16_t vid_mask;
}yt_acl_key_vid_t;

typedef struct yt_acl_key_vlanFmt_s
{
    yt_vlan_format_t tagfmt_data;
    uint8_t tagfmt_mask;
}yt_acl_key_vlanFmt_t;

typedef struct yt_acl_key_ipv4_s
{
    uint32_t ip_data;
    uint32_t ip_mask;
}yt_acl_key_ipv4_t;

typedef struct yt_acl_key_l4Port_s
{
    uint16_t port_data;
    uint16_t port_mask;
}yt_acl_key_l4Port_t;

typedef struct yt_acl_key_icmp_type_s
{
    uint8_t icmp_type_data;
    uint8_t icmp_type_mask;
}yt_acl_key_icmp_type_t;

typedef struct yt_acl_key_icmp_code_s
{
    uint8_t icmp_code_data;
    uint8_t icmp_code_mask;
}yt_acl_key_icmp_code_t;

typedef struct yt_acl_key_isPppoe_s
{
    yt_bool_t is_pppoe_data;
    yt_bool_t is_pppoe_mask;
}yt_acl_key_isPppoe_t;

typedef struct yt_acl_key_isMgntVlan_s
{
    yt_bool_t is_mgnt_data;
    yt_bool_t is_mgnt_mask;
}yt_acl_key_isMgntVlan_t;

typedef struct yt_acl_key_tcpFlag_s
{
    uint8_t flag_data;
    uint8_t flag_mask;
}yt_acl_key_tcpFlag_t;

typedef struct yt_acl_key_isIgmp_s
{
    yt_bool_t is_igmp_data;
    yt_bool_t is_igmp_mask;
}yt_acl_key_isIgmp_t;

typedef struct yt_acl_key_ipFrag_s
{
    yt_bool_t is_frag_data;
    yt_bool_t is_frag_mask;
}yt_acl_key_ipFrag_t;

typedef struct yt_acl_key_1stIpFrag_s
{
    yt_bool_t is_1stFrag_data;
    yt_bool_t is_1stFrag_mask;
}yt_acl_key_1stIpFrag_t;

typedef struct yt_acl_key_ipOption_s
{
    yt_bool_t is_option_data;
    yt_bool_t is_option_mask;
}yt_acl_key_ipOption_t;

typedef struct yt_acl_key_ipProtocol_s
{
    uint8_t protocol_data;
    uint8_t protocol_mask;
}yt_acl_key_ipProtocol_t;

typedef struct yt_acl_key_ipTOS_s
{
    uint8_t tos_data;
    uint8_t tos_mask;
}yt_acl_key_ipTOS_t;

typedef struct yt_acl_key_traffic_s
{
    uint8_t traffic_data;
    uint8_t traffic_mask;
}yt_acl_key_traffic_t;

typedef struct yt_acl_key_dscp_s
{
    uint8_t dscp_data;
    uint8_t dscp_mask;
}yt_acl_key_dscp_t;

typedef struct yt_acl_key_ip_prece_s
{
    uint8_t ip_prece_data;
    uint8_t ip_prece_mask;
}yt_acl_key_ip_prece_t;

typedef struct yt_acl_key_hderror_s
{
    yt_bool_t hderror_data;
    yt_bool_t hderror_mask;
}yt_acl_key_hderror_t;

typedef struct yt_acl_key_udf_s
{
    uint16_t udf_data;
    uint16_t udf_mask;
}yt_acl_key_udf_t;

typedef struct yt_acl_key_ipv6_s
{
    uint8_t ipv6_data[IP6_ADDR_LEN];
    uint8_t ipv6_mask[IP6_ADDR_LEN];
}yt_acl_key_ipv6_t;

typedef struct yt_acl_key_range_s
{
    uint16_t range_data;
    uint16_t range_mask;
}yt_acl_key_range_t;

typedef struct yt_acl_hw_entry_s
{
    uint32_t data[YT_ACL_ENTRY_NUM_MAX];
    uint32_t mask[YT_ACL_ENTRY_NUM_MAX];
    uint8_t portId;
}yt_acl_hw_entry_t;

typedef struct yt_acl_action_forward_s
{

#ifdef SWITCH_SERIES_SHARK
    yt_acl_fwd_type_t fwd_decision_type;
    yt_port_mask_t dest_port_mask;
    uint8_t port_index_id;
    yt_bool_t dest_port_type;
    yt_bool_t is_lag;
    yt_bool_t fwd_decision_en;
#else
    yt_acl_fwd_type_t fwd_decision_type;
    yt_port_mask_t dest_port_mask;
    yt_bool_t fwd_decision_en;
#endif
}yt_acl_action_forward_t;

typedef struct yt_acl_action_qos_s
{
    uint8_t int_pri;
    uint8_t int_dp;
    yt_bool_t int_pri_valid;
    yt_bool_t int_dp_valid;
}yt_acl_action_qos_t;

typedef struct yt_acl_action_remark_s
{
    uint16_t cvid;
    uint16_t svid;
    uint8_t cpri;
    uint8_t spri;
    uint8_t dscp;
    yt_bool_t cdei;
    yt_bool_t sdei;
    yt_bool_t dscp_replace_en;
    yt_bool_t cpri_replace_en;
    yt_bool_t spri_replace_en;
    yt_bool_t cdei_replace_en;
    yt_bool_t sdei_replace_en;
    yt_bool_t cvid_replace_en;
    yt_bool_t svid_replace_en;
}yt_acl_action_remark_t;

typedef struct yt_acl_action_bypass_s
{
    yt_bool_t cpu_ctrl_bypass;
    yt_bool_t storm_bypass;
    yt_bool_t port_meter_bypass;
    yt_bool_t igr_vlan_filter_bypass;
    yt_bool_t stp_bypass;
    yt_bool_t remark_bypass;
    yt_bool_t egress_translation_bypass;
}yt_acl_action_bypass_t;

typedef struct yt_acl_action_vlan_s
{
    yt_acl_vlan_assign_mode_t ctag_assign;
    yt_acl_vlan_assign_mode_t stag_assign;
    uint8_t ctpid_index;
    uint8_t stpid_index;
    yt_bool_t cvid_fwd_en;
    yt_bool_t svid_fwd_en;
    yt_bool_t ctpid_en;
    yt_bool_t stpid_en;
}yt_acl_action_vlan_t;

typedef struct yt_acl_action_meter_s
{
    uint8_t flow_meter_id;
    yt_bool_t flow_meter_en;
}yt_acl_action_meter_t;

typedef struct yt_acl_action_stats_s
{
    uint8_t flow_stats_id;
    yt_bool_t flow_stats_en;
}yt_acl_action_stats_t;

typedef struct yt_acl_action_mirror_s
{
    uint8_t flow_mirror_group_id;
    yt_bool_t flow_mirror_en;
}yt_acl_action_mirror_t;

typedef struct yt_acl_action_s
{
    yt_acl_action_forward_t forward;
    yt_acl_action_vlan_t vlan;
    yt_acl_action_qos_t qos;
    yt_acl_action_remark_t remark;
    yt_acl_action_bypass_t bypass;
    yt_acl_action_meter_t meter;
    yt_acl_action_stats_t stats;
    yt_acl_action_mirror_t mirror;
}yt_acl_action_t;

typedef struct yt_acl_action_mask_s
{
    uint32_t bitMap[YT_ACTS_BIT_SIZE];
}yt_acl_action_mask_t;

typedef struct yt_acl_key_mask_s
{
    uint32_t bitMap[YT_ACLS_BIT_SIZE(YT_ACL_KEY_SELECT_MAX)];
}yt_acl_key_mask_t;

typedef struct yt_acl_entry_s
{
    uint32_t prio;
    yt_acl_entry_id_t swEntryId;
    uint16_t hwEntryId;
    uint16_t actionPorkMaskId;
    uint16_t installed;
    uint16_t actionId;
    struct yt_acl_hw_entry_s hwEntry;
    struct yt_acl_action_s action;
    struct yt_acl_instance_s *pInstance;
    struct yt_acl_bin_s *pBin;
}yt_acl_entry_t;

typedef struct yt_acl_bin_s
{
    yt_acl_phase_id_t phaseId;
    yt_acl_bin_id_t binId;
    uint16_t entryCount;
    struct yt_acl_bin_s *pNext;
    struct yt_acl_bin_s *pPrev;
    struct yt_acl_entry_s **pEntries; /*index is install hw entry id*/
}yt_acl_bin_t;

typedef struct yt_acl_instance_s
{
    uint8_t instanceId;
    uint8_t binCount;
    uint16_t entrySize;
    uint16_t entryBinCount;
    uint16_t entryCount;
    uint16_t hitStartId;
    uint16_t hitEndId;
    yt_acl_instance_pri_t prio;
    yt_acl_instance_mode_t mode;
    yt_acl_key_mask_t keyMask;
    yt_acl_pattern_id_t patternIds[YT_ACL_INSTANCE_PATTERN_NUM_MAX];
    struct yt_acl_bin_s *pBin;
    struct yt_acl_entry_s **pEntries; /*index only for sw pri sorting*/
    struct yt_acl_instance_s *pNext;
}yt_acl_instance_t;

typedef struct acl_instance_inter_pri_s
{
    yt_acl_instance_pri_t instancePrio;
    uint8_t instanceId;
}acl_instance_inter_pri_t;

typedef struct yt_acl_instance_list_s
{
    yt_bool_t init;
    struct yt_acl_instance_s *pHead;
}yt_acl_instance_list_t;

/**
 * @internal      yt_acl_init
 * @endinternal
 *
 * @brief         Init acl module, create sw database header and enable unmatch-fwd & redirect state
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 */
extern yt_ret_t yt_acl_init(yt_unit_t unit);

/**
 * @internal      yt_acl_deinit
 * @endinternal
 *
 * @brief         clear sw database and hw data, free mem
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 */
extern yt_ret_t yt_acl_deinit(yt_unit_t unit);

/**
 * @internal      yt_acl_global_en_set
 * @endinternal
 *
 * @brief         Enable/Disable acl global state
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     enable                -enable or disable
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_acl_global_en_set(yt_unit_t unit, yt_enable_t enable);

/**
 * @internal      yt_acl_global_en_get
 * @endinternal
 *
 * @brief         Get the acl global state
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[out]    pEnable               -enable or disable
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 */
extern yt_ret_t yt_acl_global_en_get(yt_unit_t unit, yt_enable_t *pEnable);

/**
 * @internal      yt_acl_port_en_set
 * @endinternal
 *
 * @brief         Enable/Disable the acl port state
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     port                  -port num
 * @param[in]     enable                -enable or disable
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_PORT          -input port err
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_acl_port_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_acl_portlist_en_set
 * @endinternal
 *
 * @brief         Enable/Disable the acl port state
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     portlist              -portlist
 * @param[in]     enable                -enable or disable
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_acl_portlist_en_set(yt_unit_t unit, yt_port_mask_t portMask, yt_enable_t enable);

/**
 * @internal      yt_acl_port_en_get
 * @endinternal
 *
 * @brief         Get the acl port state
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     port                  -port num
 * @param[out]    pEnable               -enable or disable
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_PORT          -input port err
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 */
extern yt_ret_t yt_acl_port_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_acl_unmatch_fwd_en_set
 * @endinternal
 *
 * @brief         Enable/Disable acl port unmatch fwd state
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     port                  -port num
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_PORT          -input port err
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_acl_unmatch_fwd_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_acl_unmatch_fwd_en_get
 * @endinternal
 *
 * @brief         Get the acl port unmatch state
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     port                  -port num
 * @param[out]    pEnable               -enable or disable
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_PORT          -input port err
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 */
extern yt_ret_t yt_acl_unmatch_fwd_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_acl_redirect_force_en_set
 * @endinternal
 *
 * @brief         Enable/Disable force state of redirect action
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     enable                -enable or disable
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_acl_redirect_force_en_set(yt_unit_t unit, yt_enable_t enable);

/**
 * @internal      yt_acl_redirect_force_en_get
 * @endinternal
 *
 * @brief         Get force state of redirect action
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[out]    pEnable               -enable or disable
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 */
extern yt_ret_t yt_acl_redirect_force_en_get(yt_unit_t unit, yt_enable_t *pEnable);

/**
 * @internal      yt_acl_instance_create
 * @endinternal
 *
 * @brief         Create acl instance based on keyMask, auto assign instance index
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     mode                  -instance mode
 * @param[in]     prio                  -instance priority
 * @param[in]     keyMask               -key mask on instance
 * @param[out]    pInstanceId           -auto assign instance id
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ACL_INSTANCE_FULL     -acl instance is full
 * @retval        CMM_ERR_ACL_BIN_NOT_ENOUGH    -no enougn bin for acl instance
 * @retval        CMM_ERR_ACL_KEY_SELECT_FAIL   -acl key mask select fail
 */
extern yt_ret_t yt_acl_instance_create(yt_unit_t unit, yt_acl_instance_pri_t prio,
    yt_acl_key_mask_t keyMask, yt_acl_instance_id_t *pInstanceId);

/**
 * @internal      yt_acl_instance_get
 * @endinternal
 *
 * @brief         Get acl instance by index
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -instance id
 * @param[out]    pInsData              -instance data
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ACL_INSTANCE_NOT_FOUND    -instance not found
 */
extern yt_ret_t yt_acl_instance_get(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_instance_t **pInsData);

/**
 * @internal      yt_acl_instance_exist_check
 * @endinternal
 *
 * @brief         Check instance exist or not
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -instance id
 * @param[out]    pStatus               -exist status
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_acl_instance_exist_check(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_bool_t *pStatus);

/**
 * @internal      yt_acl_instance_delete
 * @endinternal
 *
 * @brief         Delete acl instance by index
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -instance id
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ACL_INSTANCE_NOT_FOUND    -instance not found
 */
extern yt_ret_t yt_acl_instance_delete(yt_unit_t unit, yt_acl_instance_id_t instanceId);

/**
 * @internal      yt_acl_entry_create
 * @endinternal
 *
 * @brief         Create acl entry based on instancce
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -instance id
 * @param[out]    pEntryId              -auto assign entry id
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ACL_INSTANCE_NOT_FOUND    -instance not found
 * @retval        CMM_ERR_ACL_ENTRY_NOT_FOUND    -acl entry not fount
 */
extern yt_ret_t yt_acl_entry_create(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t *pEntryId);

/**
 * @internal      yt_acl_entry_delete
 * @endinternal
 *
 * @brief         Delete acl sw entry
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -instance id
 * @param[in]     entryId               -entry id
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ACL_ENTRY_NOT_FOUND   -acl entry not fount
 */
extern yt_ret_t yt_acl_entry_delete(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId);

/**
 * @internal      yt_acl_entry_get
 * @endinternal
 *
 * @brief         Get acl sw entry
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -instance id
 * @param[in]     entryId               -entry id
 * @param[out]    pEntry                -acl entry data
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ACL_ENTRY_NOT_FOUND   -acl entry not exist
 */
extern yt_ret_t yt_acl_entry_get(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId, yt_acl_entry_t **pEntry);


/**
 * @internal      yt_acl_entry_destory
 * @endinternal
 *
 * @brief         Delete acl sw && hw entry
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -instance id
 * @param[in]     entryId               -entry id
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ACL_ENTRY_NOT_FOUND   -acl entry not exist
 */
extern yt_ret_t yt_acl_entry_destory(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId);

/**
 * @internal      fal_shark_acl_entry_active
 * @endinternal
 *
 * @brief         Install acl entry into hw
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -instance id
 * @param[in]     entryId               -entry id
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ACL_ENTRY_NOT_FOUND   -acl entry not exist
 */
extern yt_ret_t yt_acl_entry_active(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId);

/**
 * @internal      yt_acl_entry_inactive
 * @endinternal
 *
 * @brief         Remove acl entry from hw
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -instance id
 * @param[in]     entryId               -entry id
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ACL_ENTRY_NOT_FOUND   -acl entry not exist
 */
extern yt_ret_t yt_acl_entry_inactive(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId);

/**
 * @internal      yt_acl_entry_pri_set
 * @endinternal
 *
 * @brief         Set acl entry prio
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -instance id
 * @param[in]     entryId               -entry id
 * @param[in]     prio                  -entry priority
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ACL_ENTRY_NOT_FOUND   -acl entry not exist
 */
extern yt_ret_t yt_acl_entry_pri_set(yt_unit_t unit, yt_acl_instance_id_t instanceId,
   yt_acl_entry_id_t entryId, uint32_t prio);

/**
 * @internal      yt_acl_key_set
 * @endinternal
 *
 * @brief         Set acl key data && mask
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -instance id
 * @param[in]     entryId               -entry id
 * @param[in]     keyType               -acl key type
 * @param[in]     pKeyData              -acl common key data based on different key type
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ACL_ENTRY_NOT_FOUND   -acl entry not exist
 * @retval        CMM_ERR_ACL_INVALID_KEY_ON_INSTANCE   -invalid acl key in current instance
 */
extern yt_ret_t yt_acl_key_set(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId, yt_acl_rule_key_t keyType,
    yt_acl_comm_key_t *pKeyData);

/**
 * @internal      yt_acl_key_get
 * @endinternal
 *
 * @brief         Set acl key data && mask
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -instance id
 * @param[in]     entryId               -entry id
 * @param[in]     keyType               -acl key type
 * @param[out]    pKeyData              -acl common key data based on different key type
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ACL_ENTRY_NOT_FOUND   -acl entry not exist
 * @retval        CMM_ERR_ACL_INVALID_KEY_ON_INSTANCE   -invalid acl key in current instance
 */
extern yt_ret_t yt_acl_key_get(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId, yt_acl_rule_key_t keyType,
    yt_acl_comm_key_t *pKeyData);

/**
 * @internal      yt_acl_key_clear
 * @endinternal
 *
 * @brief         clear acl key data && mask
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -instance id
 * @param[in]     entryId               -entry id
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ACL_ENTRY_NOT_FOUND   -acl entry not exist
 */
extern yt_ret_t yt_acl_key_clear(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId);

/**
 * @internal      yt_acl_action_add
 * @endinternal
 *
 * @brief         Add action on acl entry
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -instance id
 * @param[in]     entryId               -entry id
 * @param[in]     pAction               -acl action info
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ACL_ENTRY_NOT_FOUND   -acl entry not exist
 */
extern yt_ret_t yt_acl_action_add(yt_unit_t unit, yt_acl_instance_id_t instanceId, 
    yt_acl_entry_id_t entryId, yt_acl_action_t *pAction);

/**
 * @internal      yt_acl_action_get
 * @endinternal
 *
 * @brief         Get acl action
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -instance id
 * @param[in]     entryId               -entry id
 * @param[out]    pAction               -acl action info
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ACL_ENTRY_NOT_FOUND   -acl entry not exist
 */
extern yt_ret_t yt_acl_action_get(yt_unit_t unit, yt_acl_instance_id_t instanceId, 
    yt_acl_entry_id_t entryId, yt_acl_action_t *pAction);

/**
 * @internal      yt_acl_action_del
 * @endinternal
 *
 * @brief         Remove action from acl entry
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -instance id
 * @param[in]     entryId               -entry id
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ACL_ENTRY_NOT_FOUND   -acl entry not exist
 */
extern yt_ret_t yt_acl_action_del(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId);

/**
 * @internal      yt_acl_range_key_add
 * @endinternal
 *
 * @brief         Create acl (IP/COMMON)range key by type
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     rangeType             -range profile type
 * @param[in]     pRangeKeyData         -range key data
 * @param[out]    pGenKey               -range profile indexs bit map
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_acl_range_key_add(yt_unit_t unit, yt_acl_range_type_t rangeType,
    yt_acl_rangeKey_t *pRangeKeyData, uint16_t *pGenKey);

/**
 * @internal      yt_acl_range_key_get
 * @endinternal
 *
 * @brief         Create acl (IP/COMMON)range key by type
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     rangeType             -range profile type
 * @param[in]     genKey                -range profile indexs bit map
 * @param[out]    pRangeKeyData         -range key data
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ENTRY_NOT_FOUND   -acl range entry not found
 */
extern yt_ret_t yt_acl_range_key_get(yt_unit_t unit, yt_acl_range_type_t rangeType,
    uint16_t genKey, yt_acl_rangeKey_t *pRangeKeyData);

/**
 * @internal      yt_acl_range_key_del
 * @endinternal
 *
 * @brief         Create acl (IP/COMMON)range key by type
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     rangeType             -range profile type
 * @param[in]     genKey                -range profile indexs bit map
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 * @retval        CMM_ERR_ENTRY_NOT_FOUND   -acl range entry not found
 */
extern yt_ret_t yt_acl_range_key_del(yt_unit_t unit, yt_acl_range_type_t rangeType,
    uint16_t genKey);

/**
 * @internal      yt_acl_udf_add
 * @endinternal
 *
 * @brief         Create acl udf key
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     pUdfSet               -udf set data
 * @param[out]    pGenKey                -bitMap of assign index
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 */
extern yt_ret_t yt_acl_udf_add(yt_unit_t unit, yt_acl_udf_t *pUdfSet, uint16_t *pGenKey);

/**
 * @internal      yt_acl_udf_get
 * @endinternal
 *
 * @brief         Get acl udf key
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     genKey                -bitmap of assign index
 * @param[out]    pUdfSet               -udf set data
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_ENTRY_NOT_FOUND   -acl udf entry not found
 */
extern yt_ret_t yt_acl_udf_get(yt_unit_t unit, uint16_t genKey, yt_acl_udf_t *pUdfSet);

/**
 * @internal      yt_acl_udf_exist_check
 * @endinternal
 *
 * @brief         Check udf key exist
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     genKey                -bitmap of assign index
 * @param[out]    pStatus               -exist status
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_ENTRY_NOT_FOUND   -acl udf entry not found
 */
extern yt_ret_t yt_acl_udf_exist_check(yt_unit_t unit, uint16_t genKey, yt_bool_t *pStatus);

/**
 * @internal      yt_acl_udf_del
 * @endinternal
 *
 * @brief         Delete acl udf key
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     genKey                -bitmap of assign index
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_ENTRY_NOT_FOUND   -acl udf entry not found
 */
extern yt_ret_t yt_acl_udf_del(yt_unit_t unit, uint16_t genKey);

#ifdef SWITCH_SERIES_SHARK
/**
 * @internal      yt_acl_hit_en_set
 * @endinternal
 *
 * @brief         Set enable state of acl hit, max num is 512 hw-entries
 * @note          APPLICABLE DEVICES    -Shark
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -acl instance id
 * @param[in]     startEntryId          -start acl entry id
 * @param[in]     endEntryId            -end acl entry id
 * @param[in]     enable                -enable or disable
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_TABLE_FULL    -acl hit state full
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_acl_hit_en_set(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t startEntryId, yt_acl_entry_id_t endEntryId, yt_enable_t enable);

/**
 * @internal      yt_acl_hit_en_get
 * @endinternal
 *
 * @brief         Get enable state of acl hit 
 * @note          APPLICABLE DEVICES    -Shark
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -acl instance id
 * @param[in]     entryId               -acl entry id
 * @param[out]    pEnable               -enable or disable
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_acl_hit_en_get(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_acl_entry_id_t entryId, yt_enable_t *pEnable);
#else
/**
 * @internal      yt_acl_hit_en_set
 * @endinternal
 *
 * @brief         Set enable state of acl hit, max num is 512 hw-entries
 * @note          APPLICABLE DEVICES    -Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -acl instance id
 * @param[in]     enable                -enable or disable
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_TABLE_FULL    -acl hit state full
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_acl_hit_en_set(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_enable_t enable);

/**
 * @internal      yt_acl_hit_en_get
 * @endinternal
 *
 * @brief         Get enable state of acl hit 
 * @note          APPLICABLE DEVICES    -Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -acl instance id
 * @param[out]    pEnable               -enable or disable
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t yt_acl_hit_en_get(yt_unit_t unit, yt_acl_instance_id_t instanceId,
    yt_enable_t *pEnable);
#endif

/**
 * @internal      yt_acl_hit_get
 * @endinternal
 *
 * @brief         Get acl entry hit status
 * @note          APPLICABLE DEVICES    -Shark/Whale
 * @param[in]     unit                  -unit id
 * @param[in]     instanceId            -instance id
 * @param[in]     entryId               -entry id
 * @param[out]    pHitStatus            -entry hit status
 * @retval        CMM_ERR_OK            -on success
 * @retval        CMM_ERR_FAIL          -on fail
 * @retval        CMM_ERR_INPUT         -input value err
 * @retval        CMM_ERR_NOT_INIT      -not init
 * @retval        CMM_ERR_NULL_POINT    -null pointer
 */
extern yt_ret_t yt_acl_hit_get(yt_unit_t unit, yt_acl_instance_id_t instanceId, 
    yt_acl_entry_id_t entryId, yt_bool_t *pHitStatus);

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
