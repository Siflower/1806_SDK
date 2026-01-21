/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_acl.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_ACL_H__
#define __FAL_TIGER_ACL_H__


#include "fal_cmm.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

enum acl_rule_type_e
{
    ACL_RULE_MAC_DA0 = 1,
    ACL_RULE_MAC_SA0,
    ACL_RULE_MAC_DA1_SA1,
    ACL_RULE_VLAN_TAG,
    ACL_RULE_VID_PRI_DEI,
    ACL_RULE_IPV4_DA,
    ACL_RULE_IPV4_SA,
    ACL_RULE_IPV6_DA0,
    ACL_RULE_IPV6_DA1,
    ACL_RULE_IPV6_DA2,
    ACL_RULE_IPV6_DA3,
    ACL_RULE_IPV6_SA0,
    ACL_RULE_IPV6_SA1,
    ACL_RULE_IPV6_SA2,
    ACL_RULE_IPV6_SA3,
    ACL_RULE_MISC,
    ACL_RULE_L4_PORT,
    ACL_RULE_UDF0,
    ACL_RULE_UDF1,
    ACL_RULE_UDF2,
    ACL_RULE_UDF3,
    ACL_RULE_UDF4,
    ACL_RULE_UDF5,
    ACL_RULE_UDF6,
    ACL_RULE_UDF7,
    ACL_RULE_ETHERTYPE_VALUE,
    ACL_RULE_MAX,
};

#define ACL_LINE_NUM    48
#define EXT_NUM         8
#define ACL_ENTRY_WIDTH_PER32BITS   2
#define ACL_MASK_WIDTH_PER32BITS    ACL_ENTRY_WIDTH_PER32BITS
#define ACL_IDX_INVALID             0xFFFF

#define ACL_KEY_MAX_NUM     8

#define ACL_ONE_DATA_LEN_MAX 16
#define ACL_MASK_OFFSET (ACL_ONE_DATA_LEN_MAX+4)

#define ACL_1_BIT_MASK      (0x1)
#define ACL_2_BIT_MASK      (0x3)
#define ACL_3_BIT_MASK      (0x7)
#define ACL_4_BIT_MASK      (0xF)
#define ACL_8_BIT_MASK      (0xFF)
#define ACL_12_BIT_MASK     (0xFFF)


#define ACL_VLAN_DEI_BIT_MASK   ACL_1_BIT_MASK
#define ACL_VLAN_FMT_BIT_MASK   ACL_2_BIT_MASK
#define ACL_VLAN_PRI_BIT_MASK   ACL_3_BIT_MASK
#define ACL_VLAN_ID_BIT_MASK    ACL_12_BIT_MASK
#define ACL_L2_TYPE_BIT_MASK    ACL_3_BIT_MASK
#define ACL_L3_TYPE_BIT_MASK    ACL_4_BIT_MASK
#define ACL_L4_TYPE_BIT_MASK    ACL_3_BIT_MASK
#define ACL_IP_FRAG_BIT_MASK    ACL_1_BIT_MASK
#define ACL_IP_FIR_FRAG_BIT_MASK     ACL_1_BIT_MASK
#define ACL_IP_OPTION_BIT_MASK  ACL_1_BIT_MASK
#define ACL_TCP_FLAG_BIT_MASK   ACL_8_BIT_MASK
#define ACL_IP_PROT_BIT_MASK    ACL_8_BIT_MASK
#define ACL_IP_TOS_BIT_MASK     ACL_8_BIT_MASK
#define ACL_PPPOE_FLAG_BIT_MASK ACL_1_BIT_MASK
#define ACL_IS_IGMP_BIT_MASK    ACL_1_BIT_MASK



typedef struct data{
    uint8_t grpID;
    uint8_t entryIdx;
    uint8_t rule_type;
    uint32 data_entry[ACL_ENTRY_WIDTH_PER32BITS];
    uint32 data_mask[ACL_ENTRY_WIDTH_PER32BITS];
    struct data *next;
}data_store_t;

typedef struct data_head_s
{
    uint8_t size;
    struct data *head;
}data_head_t;

typedef struct{
    uint8_t isUsed;
    uint32_t id;
} acl_id_t;

typedef struct igrAcl_data_s
{
    yt_igrAcl_key_type_t  type;
    uint8_t    data[ACL_ONE_DATA_LEN_MAX+4];// more 4 for range
    uint8_t    mask[ACL_ONE_DATA_LEN_MAX];
    struct     igrAcl_data_s *pNext;
}igrAcl_data_t;

typedef struct acl_action_s {
    uint32    INTR_EN;
    uint32    CPU_CTRL_BYPASS;
    uint32    FWD_CVID_EN;
    uint32    FWD_SVID_EN;
    uint32    RESERVED0;
    uint32    GPIO_EN;
    uint32    FLOW_STATS_PTR;
    uint32    FLOW_STATS_EN;
    uint32    MIRROR_EN;
    uint32    METER_EN;
    uint32    METER_ID;
    uint32    DSCP_REPLACE_EN;
    uint32    DSCP;
    uint32    INT_DP_VALID;
    uint32    INT_DP;
    uint32    INT_PRI_VALID;
    uint32    INT_PRI;
    uint32    CVID_REPLACE_EN;
    uint32    CVID;
    uint32    CPRI_REPLACE_EN;
    uint32    CPRI;
    uint32    CDEI_REPLACE_EN;
    uint32    CDEI;
    uint32    SVID_REPLACE_EN;
    uint32    SVID;
    uint32    SPRI_REPLACE_EN;
    uint32    SPRI;
    uint32    SDEI_REPLACE_EN;
    uint32    SDEI;
    uint32    FWD_DECISION_EN;
    uint32    DEST_PORT_MASK;
    uint32    FWD_DECISION_TYPE;
    uint32    CTAG_ASSIGN;
    uint32    STAG_ASSIGN;
}acl_action_t;

typedef struct yt_igrAcl_data_tlv_s
{
    yt_igrAcl_key_type_t  type;
    union{
        struct {
            uint8_t    mac_data[MAC_ADDR_LEN];
            uint8_t    offsets[ACL_MASK_OFFSET-MAC_ADDR_LEN];
            uint8_t    mac_mask[MAC_ADDR_LEN];
            uint8_t    reserved[ACL_ONE_DATA_LEN_MAX-MAC_ADDR_LEN];
        }mac;  /*for s_mac, d_mac*/

        struct {
            uint16_t    type_data;
            uint8_t      range_en;
            uint8_t      offsets[ACL_MASK_OFFSET-3];
            uint16_t    type_mask;
            uint8_t      reserved[ACL_ONE_DATA_LEN_MAX-2];
        }ether_type;

        struct {
            uint8_t    type_data;
            uint8_t    offsets[ACL_MASK_OFFSET-1];
            uint8_t    type_mask;
            uint8_t    reserved[ACL_ONE_DATA_LEN_MAX-1];
        }l2_3_4_type;

        struct {
            uint8_t    dei_data;
            uint8_t    offsets[ACL_MASK_OFFSET-1];
            uint8_t    dei_mask;
            uint8_t    reserved[ACL_ONE_DATA_LEN_MAX-1];
        }dei;

        struct {
            uint8_t    pri_data;
            uint8_t    offsets[ACL_MASK_OFFSET-1];
            uint8_t    pri_mask;
            uint8_t    reserved[ACL_ONE_DATA_LEN_MAX-1];
        }pri;

        struct {
            uint8_t    tagfmt_data;
            uint8_t    offsets[ACL_MASK_OFFSET-1];
            uint8_t    tagfmt_mask;
            uint8_t    reserved[ACL_ONE_DATA_LEN_MAX-1];
        }tag_format;

        struct {
            uint16_t    vid_data;
            uint8_t      range_en;
            uint8_t      offsets[ACL_MASK_OFFSET-3];
            uint16_t    vid_mask;
            uint8_t      reserved[ACL_ONE_DATA_LEN_MAX-2];
        }vid;

        struct {
            uint32_t    ip_data;
            uint8_t      range_en;
            uint8_t      offsets[ACL_MASK_OFFSET-5];
            uint32_t    ip_mask;
            uint8_t      reserved[ACL_ONE_DATA_LEN_MAX-4];
        }ipv4;

        struct {
            uint16_t    port_data;
            uint8_t      range_en;
            uint8_t      offsets[ACL_MASK_OFFSET-3];
            uint16_t    port_mask;
            uint8_t      reserved[ACL_ONE_DATA_LEN_MAX-2];
        }l4_port;

        struct {
            uint8_t    flag_data;
            uint8_t    offsets[ACL_MASK_OFFSET-1];
            uint8_t    flag_mask;
            uint8_t    reserved[ACL_ONE_DATA_LEN_MAX-1];
        }is_flag;/*pppoe,igmp,frag,1stFrag,ip option*/

        struct {
            uint8_t    flag_data;
            uint8_t    offsets[ACL_MASK_OFFSET-1];
            uint8_t    flag_mask;
            uint8_t    reserved[ACL_ONE_DATA_LEN_MAX-1];
        }tcp_flag;

        struct {
            uint8_t    protocol_data;
            uint8_t    offsets[ACL_MASK_OFFSET-1];
            uint8_t    protocol_mask;
            uint8_t    reserved[ACL_ONE_DATA_LEN_MAX-1];
        }ip_protocol;

        struct {
            uint8_t    tos_data;
            uint8_t    offsets[ACL_MASK_OFFSET-1];
            uint8_t    tos_mask;
            uint8_t    reserved[ACL_ONE_DATA_LEN_MAX-1];
        }ip_tos;

        struct {
            uint8_t     udf_data[YT_ACL_UDF_DATA_LEN_MAX];
            uint8_t     offsets[ACL_MASK_OFFSET-YT_ACL_UDF_DATA_LEN_MAX];
            uint8_t     udf_mask[YT_ACL_UDF_DATA_LEN_MAX];
            uint8_t     reserved[ACL_ONE_DATA_LEN_MAX-YT_ACL_UDF_DATA_LEN_MAX];
        }udf;

        struct {
            uint8_t    ipv6_data[IP6_ADDR_LEN];
            uint8_t    range_en;
            uint8_t    offsets[ACL_MASK_OFFSET-IP6_ADDR_LEN-1];
            uint8_t    ipv6_mask[IP6_ADDR_LEN];
        }ipv6;
    }data;
    struct yt_igrAcl_data_tlv_s *pNext;
}yt_igrAcl_data_tlv_t;

/**
 * @internal      fal_tiger_acl_init
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_acl_init(yt_unit_t unit);


/**
 * @internal      fal_tiger_acl_port_en_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable             -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_acl_port_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_acl_port_en_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable            -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_acl_port_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_acl_unmatch_permit_en_set
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
extern yt_ret_t  fal_tiger_acl_unmatch_permit_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_acl_unmatch_permit_en_get
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
extern yt_ret_t  fal_tiger_acl_unmatch_permit_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_acl_udf_rule_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     index               -x
 * @param[in]     type                -acl user define data type
 * @param[in]     offset              -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_acl_udf_rule_set(yt_unit_t unit, uint8_t index, yt_acl_udf_type_t type, uint8_t offset);


/**
 * @internal      fal_tiger_acl_rule_init
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_acl_rule_init(yt_unit_t unit);


/**
 * @internal      fal_tiger_acl_rule_reset
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_acl_rule_reset(yt_unit_t unit);


/**
 * @internal      fal_tiger_acl_rule_key_add
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                -x
 * @param[in]     pKeyData            -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_acl_rule_key_add(yt_unit_t unit, yt_igrAcl_key_type_t type, yt_acl_comm_key_t *pKeyData);


/**
 * @internal      fal_tiger_acl_rule_action_add
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                -x
 * @param[in]     pAction             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_acl_rule_action_add(yt_unit_t unit, yt_acl_action_type_t type, yt_acl_comm_act_t *pAction);


/**
 * @internal      fal_tiger_acl_rule_create
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     rulePri             -x
 * @param[in]     ruleReverse         -yt_types.h
 * @param[out]    pId                 -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_acl_rule_create(yt_unit_t unit, uint16_t rulePri, yt_bool_t ruleReverse, uint32_t *pId);


/**
 * @internal      fal_tiger_acl_rule_active
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     id                  -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_acl_rule_active(yt_unit_t unit, uint32_t id);


/**
 * @internal      fal_tiger_acl_rule_del
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     id                  -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_acl_rule_del(yt_unit_t unit, uint32_t id);


extern uint32_t fal_tiger_acl_reset(yt_unit_t unit);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
