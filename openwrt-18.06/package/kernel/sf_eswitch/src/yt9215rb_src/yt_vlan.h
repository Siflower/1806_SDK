/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_vlan.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_VLAN_H
#define __YT_VLAN_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "yt_cmm.h"

#define YT_VLAN_ID_MIN                         (0U)
#define YT_VLAN_ID_MAX                         (4095U)
#define YT_VLAN_PRI_MAX                        (7U)
#define YT_VLAN_FID_ENTRY_MAX_NUM              (128U)
#define YT_VLAN_MAC_IP_SUBNET_VLAN_MAX_NUM     (32U)
#define YT_VLAN_MAC_VLAN_MAX_PREFIX_LEN        (48U)
#define YT_VLAN_IP_SUBNET_VLAN_MAX_PREFIX_LEN  (32U)
#define YT_VLAN_POLICY_MAX_NUM                 (16U)

typedef struct yt_tpid_profiles_s
{
    uint16_t tpid[YT_TPID_PROFILE_NUM];
}yt_tpid_profiles_t;

typedef enum yt_vlan_mac_ip_subnet_pri_e
{
    YT_VLAN_TYPE_MAC_VLAN_FIRST,
    YT_VLAN_TYPE_IPSUB_VLAN_FIRST,
}yt_vlan_mac_ip_subnet_pri_t;

typedef enum yt_vlan_aft_e
{
    YT_VLAN_AFT_ALL,
    YT_VLAN_AFT_TAGGED,
    YT_VLAN_AFT_UNTAGGED,
    YT_VLAN_AFT_NONE,
}yt_vlan_aft_t;

typedef enum yt_egr_tag_mode_e
{
    YT_VLAN_TAG_MODE_UNTAGGED,
    YT_VLAN_TAG_MODE_TAGGED,
    YT_VLAN_TAG_MODE_TAGGED_EXCEPT_DEF_VID,
    YT_VLAN_TAG_MODE_PRIO_TAGGED,
    YT_VLAN_TAG_MODE_KEEP_ALL,
    YT_VLAN_TAG_MODE_KEEP_TAGGED_MODE,
    YT_VLAN_TAG_MODE_ENTRY_BASED
} yt_egr_tag_mode_t;

typedef enum yt_vlan_action_e
{
    YT_VLAN_ACTION_NO_CHANGE,
    YT_VLAN_ACTION_ADD,
    YT_VLAN_ACTION_REPLACE,        
    YT_VLAN_ACTION_COPY,
}yt_vlan_action_t;

typedef enum yt_protocol_vlan_pri_act_e
{
    YT_PROTOCOL_VLAN_PRI_ACT_NO_CHANGE = 0,
    YT_PROTOCOL_VLAN_PRI_ACT_REPLACE,
}yt_protocol_vlan_pri_act_t;

typedef enum yt_macip_vlan_pri_act_e
{
    YT_MACIP_VLAN_PRI_ACT_NO_CHANGE = 0,
    YT_MACIP_VLAN_PRI_ACT_REPLACE,  
    YT_MACIP_VLAN_PRI_ACT_COPY,
}yt_macip_vlan_pri_act_t;

typedef enum yt_vlan_tag_fmt_e
{
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    YT_VLAN_TAG_FMT_NONE,
#endif
    YT_VLAN_TAG_FMT_UNTAGGED,
    YT_VLAN_TAG_FMT_PRIO_TAGGED, 
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    YT_VLAN_TAG_FMT_UNTAGGED_AND_PRIO_TAGGED,
#endif
    YT_VLAN_TAG_FMT_TAGGED,
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    YT_VLAN_TAG_FMT_UNTAGGED_AND_TAGGED,
    YT_VLAN_TAG_FMT_PRIO_TAGGED_AND_TAGGED, 
    YT_VLAN_TAG_FMT_ALL,
#endif
    YT_VLAN_TAG_FMT_MAX,
}yt_vlan_tag_fmt_t;

typedef enum yt_vlan_ip_mc_mode_e
{
    YT_VLAN_IPMC_MODE_MAC,
    YT_VLAN_IPMC_MODE_GIP,
    YT_VLAN_IPMC_MODE_GIP_VID,
    YT_VLAN_IPMC_MODE_SIP_GIP,
    YT_VLAN_IPMC_MODE_SIP_GIP_VID,
} yt_vlan_ip_mc_mode_t;

typedef struct yt_vlan_protocol_key_s 
{
    yt_l2_type_t l2Type;  //use yt_l2_type_t to set value
    uint16_t     ethType;
    yt_enable_t  keyEn;
}yt_vlan_protocol_key_t;

typedef struct yt_vlan_protocol_action_s 
{
    uint16_t         newCvid;
    yt_vlan_action_t cvidAct;
    uint16_t         newSvid;
    yt_vlan_action_t svidAct;
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    uint8_t          newCpri;
    yt_protocol_vlan_pri_act_t cpriAct;
    uint8_t          newSpri;
    yt_protocol_vlan_pri_act_t spriAct;
#endif
}yt_vlan_protocol_action_t;

typedef struct yt_mac_ip_action_s
{
    yt_bool_t       igrVlanFilterBypass;
    yt_act_type_t   fwdAct;
    uint8_t                  newSpri;
    yt_macip_vlan_pri_act_t  spriAct;
    uint8_t                  newCpri;
    yt_macip_vlan_pri_act_t  cpriAct;
    uint16_t                 newSvid;
    yt_vlan_action_t         svidAct;
    uint16_t                 newCvid;
    yt_vlan_action_t         cvidAct;
}yt_mac_vlan_action_t,yt_ip_subnet_vlan_action_t;

typedef struct yt_mac_vlan_rule_s
{
    yt_bool_t   isLagId;
    yt_bool_t   portIncl;
    uint8_t     port;
    uint8_t     saPrefix;
    uint8_t     smac[6];
    yt_vlan_tag_fmt_t stagFmt;
    yt_vlan_tag_fmt_t ctagFmt;
}yt_mac_vlan_rule_t;

typedef struct yt_ip_subnet_vlan_rule_s
{
    yt_bool_t    isLagId;
    yt_bool_t    portIncl;
    uint8_t      port;
    uint8_t      sipPrefix;
    uint32_t     sip;
    yt_vlan_tag_fmt_t stagFmt;
    yt_vlan_tag_fmt_t ctagFmt;
}yt_ip_subnet_vlan_rule_t;

typedef struct yt_vlan_policy_info_s
{
    yt_bool_t disableL2Learn;
    yt_vlan_ip_mc_mode_t ipv4LearnMode;
    yt_vlan_ip_mc_mode_t ipv6LearnMode;
    yt_act_type_t igmpAct;
    yt_act_type_t mldAct;
    yt_act_type_t ipv4McIpRange1Act;    /* IP : 239.x.x.x  */
    yt_act_type_t ipv4McIpRange2Act;    /* IP : 224.0.1.x  */
    yt_act_type_t ipv4McIpRange3Act;    /* IP : 224.0.0.x  */
    yt_act_type_t ipv6McIpRange1Act;    /* IP : FF0X:0:0:DB8:0:0:XXXX:XXXX  */
    yt_act_type_t ipv6McIpRange2Act;    /* IP : FF0X::XXXX:XXXX  */
    yt_act_type_t l2UnknownMcAct;
    yt_act_type_t ipv4UnknownMcAct;
    yt_act_type_t ipv6UnknownMcAct;
    yt_port_mask_t l2UnknownMcPortMask;
    yt_port_mask_t ipv4UnknownMcPortMask;
    yt_port_mask_t ipv6UnknownMcPortMask;
}yt_vlan_policy_info_t;


/**
 * @internal      yt_vlan_init
 * @endinternal
 *
 * @brief         Init vlan
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 */
extern yt_ret_t  yt_vlan_init(yt_unit_t unit);

/**
 * @internal      yt_vlan_port_set
 * @endinternal
 *
 * @brief         Set vlan untag port list and tag port list
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     memberPortmask      -member port bit mask
 * @param[in]     untagPortmask       -untag member port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORTLIST    -portlist err
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_port_set(yt_unit_t unit, yt_vlan_t vid, yt_port_mask_t memberPortmask, yt_port_mask_t untagPortmask);

/**
 * @internal      yt_vlan_port_get
 * @endinternal
 *
 * @brief         Get vlan untag port list and tag port list
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[out]    pMemberPortmask     -member port bit mask
 * @param[in]     pUntagPortmask      -untag member port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_port_get(yt_unit_t unit, yt_vlan_t vid, yt_port_mask_t *pMemberPortmask, yt_port_mask_t *pUntagPortmask);

/**
 * @internal      yt_vlan_svlMode_enable_set
 * @endinternal
 *
 * @brief         Set per vlan SVL mode or IVL mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_svlMode_enable_set(yt_unit_t unit, yt_vlan_t vid, yt_enable_t enable);

/**
 * @internal      yt_vlan_svlMode_enable_get
 * @endinternal
 *
 * @brief         Get per vlan SVL mode or IVL mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_svlMode_enable_get(yt_unit_t unit, yt_vlan_t vid, yt_enable_t *pEnable);

/**
 * @internal      yt_vlan_fid_set
 * @endinternal
 *
 * @brief         Set the fid value corresponding to vlan 
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     fid                 -fid
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_fid_set(yt_unit_t unit, yt_vlan_t vid, yt_fid_t fid);

/**
 * @internal      yt_vlan_fid_get
 * @endinternal
 *
 * @brief         Get the fid value corresponding to vlan
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[out]    pFid                -fid
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_fid_get(yt_unit_t unit, yt_vlan_t vid, yt_fid_t *pFid);

/**
 * @internal      yt_vlan_igrTpid_set
 * @endinternal
 *
 * @brief         Set ingress tpid value
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     tpid                -tpid profile
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 */
extern yt_ret_t  yt_vlan_igrTpid_set(yt_unit_t unit, yt_tpid_profiles_t tpid);

/**
 * @internal      yt_vlan_igrTpid_get
 * @endinternal
 *
 * @brief         Get ingress tpid value
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pTpid               -tpid profile
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_vlan_igrTpid_get(yt_unit_t unit, yt_tpid_profiles_t *pTpid);

/**
 * @internal      yt_vlan_port_igrTpidSel_set
 * @endinternal
 *
 * @brief         Set per port ingress tpid profile mask of SVLAN or CVLAN
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     tpidIdxMask         -tpid profile mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 */
extern yt_ret_t  yt_vlan_port_igrTpidSel_set(yt_unit_t unit, yt_vlan_type_t type, yt_port_t port, yt_tpidprofile_id_mask_t tpidIdxMask);

/**
 * @internal      yt_vlan_port_igrTpidSel_get
 * @endinternal
 *
 * @brief         Get per port ingress tpid profile mask of SVLAN or CVLAN
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pTpidIdxMask        -tpid profile mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_PORT        -port err
 */
extern yt_ret_t  yt_vlan_port_igrTpidSel_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_tpidprofile_id_mask_t *pTpidIdxMask);

/**
 * @internal      yt_vlan_port_igrPvid_set
 * @endinternal
 *
 * @brief         Set per port ingress PVID of SVLAN or CVLAN
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     vid                 -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_EXCEED_RANGE  -input value out of range
 */
extern yt_ret_t  yt_vlan_port_igrPvid_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, yt_vlan_t vid);

/**
 * @internal      yt_vlan_port_igrPvid_get
 * @endinternal
 *
 * @brief         Get per port ingress PVID of SVLAN or CVLAN
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pVid                -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_PORT        -port err
 */
extern yt_ret_t  yt_vlan_port_igrPvid_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t *pVid);

/**
 * @internal      yt_vlan_port_igrDefPri_set
 * @endinternal
 *
 * @brief         Set port ingress default priority for untag packets depend on vlan type (CVLAN or SVLAN)
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     pri                 -priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_vlan_port_igrDefPri_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, uint8_t pri);

/**
 * @internal      yt_vlan_port_igrDefPri_get
 * @endinternal
 *
 * @brief         Get port ingress default priority for untag packets depend on vlan type (CVLAN or SVLAN)
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pPri                -priority
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_vlan_port_igrDefPri_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, uint8_t *pPri);

/**
 * @internal      yt_vlan_port_igrFilter_enable_set
 * @endinternal
 *
 * @brief         Set per port ingress filter to enable or disable
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 */
extern yt_ret_t  yt_vlan_port_igrFilter_enable_set(yt_unit_t unit, yt_port_t  port, yt_enable_t enable);

/**
 * @internal      yt_vlan_port_igrFilter_enable_get
 * @endinternal
 *
 * @brief         Get per port ingress filter whether is enable or disable
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_PORT        -port err
 */
extern yt_ret_t  yt_vlan_port_igrFilter_enable_get(yt_unit_t unit, yt_port_t  port, yt_enable_t *pEnable);

/**
 * @internal      yt_vlan_igrTransparent_set
 * @endinternal
 *
 * @brief         Set ingress vlan transparent,packet from port will bypass egress vlan filter on the ports specified by port_mask
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -ingress port num
 * @param[in]     portMask            -egress port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORTLIST    -portlist err
 * @retval        CMM_ERR_PORT        -port err
 */
extern yt_ret_t  yt_vlan_igrTransparent_set(yt_unit_t unit, yt_port_t  port, yt_port_mask_t portMask);

/**
 * @internal      yt_vlan_igrTransparent_get
 * @endinternal
 *
 * @brief         Get ingress vlan transparent setting by ingress port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -ingress port num
 * @param[out]    pPortMask           -egress port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_vlan_igrTransparent_get(yt_unit_t unit, yt_port_t  port, yt_port_mask_t *pPortMask);

/**
 * @internal      yt_vlan_port_aft_set
 * @endinternal
 *
 * @brief         Set port accept frame type 
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     aft                 -accept frame type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 */
extern yt_ret_t  yt_vlan_port_aft_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, yt_vlan_aft_t aft);

/**
 * @internal      yt_vlan_port_aft_get
 * @endinternal
 *
 * @brief         Get port accept frame type 
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pAft                -accept frame type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_vlan_port_aft_get(yt_unit_t unit, yt_vlan_type_t type, yt_port_t port, yt_vlan_aft_t *pAft);

/**
 * @internal      yt_vlan_port_egrTagMode_set
 * @endinternal
 *
 * @brief         Set port egress tag mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     tagMode             -tag mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 */
extern yt_ret_t  yt_vlan_port_egrTagMode_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port,  yt_egr_tag_mode_t tagMode);

/**
 * @internal      yt_vlan_port_egrTagMode_get
 * @endinternal
 *
 * @brief         Get port egress tag mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pTagMode            -tag mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_vlan_port_egrTagMode_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port,  yt_egr_tag_mode_t *pTagMode);

/**
 * @internal      yt_vlan_port_egrDefVid_set
 * @endinternal
 *
 * @brief         Set egress default VID for egress tag mode YT_VLAN_TAG_MODE_TAGGED_EXCEPT_DEF_VID
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     defaultVid          -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  yt_vlan_port_egrDefVid_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t defaultVid);

/**
 * @internal      yt_vlan_port_egrDefVid_get
 * @endinternal
 *
 * @brief         Get egress default VID for egress tag mode YT_VLAN_TAG_MODE_TAGGED_EXCEPT_DEF_VID
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pDefaultVid         -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_vlan_port_egrDefVid_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t *pDefaultVid);

/**
 * @internal      yt_vlan_egrTpid_set
 * @endinternal
 *
 * @brief        Set egress tpid value 
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     tpids               -tpids
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 */
extern yt_ret_t  yt_vlan_egrTpid_set(yt_unit_t unit,  yt_tpid_profiles_t tpids);

/**
 * @internal      yt_vlan_egrTpid_get
 * @endinternal
 *
 * @brief         Get egress tpid value
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pTpids              -tpids
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_vlan_egrTpid_get(yt_unit_t unit,  yt_tpid_profiles_t *pTpids);

/**
 * @internal      yt_vlan_port_egrTpidSel_set
 * @endinternal
 *
 * @brief         Set per port egress tpid profile index of SVLAN or CVLAN
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[in]     tpidIdx             -tpid profile index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 */
extern yt_ret_t  yt_vlan_port_egrTpidSel_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, uint8_t tpidIdx);

/**
 * @internal      yt_vlan_port_egrTpidSel_get
 * @endinternal
 *
 * @brief         Get per port egress tpid profile index of SVLAN or CVLAN
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -port num
 * @param[out]    pTpidIdx            -tpid profile index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_vlan_port_egrTpidSel_get(yt_unit_t unit, yt_vlan_type_t type, yt_port_t port, uint8_t *pTpidIdx);

/**
 * @internal      yt_vlan_port_egrTransparent_set
 * @endinternal
 *
 * @brief         Set vlan egress transparent state for per port,packet from port will keep all of tag on the ports specified by port_mask
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -egress port num
 * @param[in]     enable              -enable or disable
 * @param[in]     portMask           -ignress port bit mask to transparent
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_PORTLIST     -portlist err
 */
extern yt_ret_t  yt_vlan_port_egrTransparent_set(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t port,  yt_enable_t enable, yt_port_mask_t portMask);

/**
 * @internal      yt_vlan_port_egrTransparent_get
 * @endinternal
 *
 * @brief         Get vlan egress transparent state for per port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @param[in]     port                -egress port num
 * @param[out]    pEnable             -enable or disable
 * @param[out]    pPortMask          -egress port bit mask to transparent
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_vlan_port_egrTransparent_get(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t port,  yt_enable_t *pEnable, yt_port_mask_t *pPortMask);

/**
 * @internal      yt_vlan_port_egrFilter_enable_set
 * @endinternal
 *
 * @brief         Set per port egress filter to enable or disable
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 */
extern yt_ret_t  yt_vlan_port_egrFilter_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_vlan_port_egrFilter_enable_get
 * @endinternal
 *
 * @brief         Get per port egress filter to enable or disable
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable            -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_vlan_port_egrFilter_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_vlan_port_vidTypeSel_set
 * @endinternal
 *
 * @brief         Set forward vlan type
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     type                -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_vlan_port_vidTypeSel_set(yt_unit_t unit, yt_port_t port, yt_vlan_type_t type);

/**
 * @internal      yt_vlan_port_vidTypeSel_get
 * @endinternal
 *
 * @brief         Get forward vlan type
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pType               -YT_VLAN_TYPE_CVLAN or YT_VLAN_TYPE_SVLAN
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_vlan_port_vidTypeSel_get(yt_unit_t unit, yt_port_t port, yt_vlan_type_t *pType);

/**
 * @internal      yt_vlan_protocolBasedVlan_group_set
 * @endinternal
 *
 * @brief         Set protocol based vlan group
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -group id
 * @param[in]     pKey                -protocol key
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  yt_vlan_protocolBasedVlan_group_set(yt_unit_t unit, uint8_t groupId, const yt_vlan_protocol_key_t *pKey);

/**
 * @internal      yt_vlan_protocolBasedVlan_group_get
 * @endinternal
 *
 * @brief         Get protocol based vlan group
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -group id
 * @param[out]    pKey                -protocol key
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  yt_vlan_protocolBasedVlan_group_get(yt_unit_t unit, uint8_t groupId, yt_vlan_protocol_key_t *pKey);

/**
 * @internal      yt_vlan_protocolBasedVlan_table_add
 * @endinternal
 *
 * @brief         Set protocol vlan table action
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     groupId             -group id
 * @param[in]     pAction             -action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  yt_vlan_protocolBasedVlan_table_add(yt_unit_t unit, yt_port_t port, uint8_t groupId, const yt_vlan_protocol_action_t *pAction);

/**
 * @internal      yt_vlan_protocolBasedVlan_table_get
 * @endinternal
 *
 * @brief         Get protocol vlan table action
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     groupId             -group id
 * @param[out]    pAction             -action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  yt_vlan_protocolBasedVlan_table_get(yt_unit_t unit, yt_port_t port, uint8_t  groupId, yt_vlan_protocol_action_t *pAction);

/**
 * @internal      yt_vlan_protocolBasedVlan_table_del
 * @endinternal
 *
 * @brief         Del protocol vlan table action
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     groupId             -group id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  yt_vlan_protocolBasedVlan_table_del(yt_unit_t unit, yt_port_t port, uint8_t  groupId);

/**
 * @internal      yt_vlan_mac_ipSub_tbl_arrange_set
 * @endinternal
 *
 * @brief         arrange table for both of mac vlan and ip subnet vlan
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     macNum              -mac vlan entry number
 * @param[in]     ipSubNum            -ip subnet vlan entry number
 * @param[in]     pri                 -mac vlan priority first or ip subnet vlan priority first
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  yt_vlan_mac_ipSub_tbl_arrange_set(yt_unit_t unit, uint8_t macNum, uint8_t ipSubNum, yt_vlan_mac_ip_subnet_pri_t  pri);

/**
 * @internal      yt_vlan_mac_ipSub_tbl_arrange_get
 * @endinternal
 *
 * @brief          Get table arrange for both of mac vlan and ip subnet vlan
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]     pMacNum            -mac vlan entry number
 * @param[out]     pIpSubNum          -ip subnet vlan entry number
 * @param[out]     pPri               -mac vlan priority first or ip subnet vlan priority first
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_vlan_mac_ipSub_tbl_arrange_get(yt_unit_t unit, uint8_t *pMacNum, uint8_t *pIpSubNum, yt_vlan_mac_ip_subnet_pri_t  *pPri);

/**
 * @internal      yt_vlan_macBasedVlan_table_add
 * @endinternal
 *
 * @brief         Add mac based vlan table and return the table index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     pMacVlanRule        -mac vlan entry rule
 * @param[in]     pMacVlanAction      -mac vlan entry action
 * @param[out]     pTableIdx          -table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  yt_vlan_macBasedVlan_table_add(yt_unit_t unit, const yt_mac_vlan_rule_t *pMacVlanRule, const yt_mac_vlan_action_t *pMacVlanAction, yt_mac_vlan_table_idx_t *pTableIdx);

/**
 * @internal      yt_vlan_macBasedVlan_table_add_by_index
 * @endinternal
 *
 * @brief         Set mac based vlan table by the table index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     tableIdx            -table index
 * @param[in]     pMacVlanRule        -mac vlan entry rule
 * @param[in]     pMacVlanAction      -mac vlan entry action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  yt_vlan_macBasedVlan_table_add_by_index(yt_unit_t unit, yt_mac_vlan_table_idx_t tableIdx, const yt_mac_vlan_rule_t *pMacVlanRule, const yt_mac_vlan_action_t *pMacVlanAction);

/**
 * @internal      yt_vlan_macBasedVlan_table_get
 * @endinternal
 *
 * @brief         Get mac based vlan table by the table index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     tableIdx            -table index
 * @param[in]     pMacVlanRule        -mac vlan entry rule
 * @param[in]     pMacVlanAction      -mac vlan entry action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 */
extern yt_ret_t  yt_vlan_macBasedVlan_table_get(yt_unit_t unit, yt_mac_vlan_table_idx_t tableIdx, yt_mac_vlan_rule_t *pMacVlanRule, yt_mac_vlan_action_t *pMacVlanAction);

/**
 * @internal      yt_vlan_macBasedVlan_table_del
 * @endinternal
 *
 * @brief         Delete mac based vlan table by the table index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     tableIdx            -table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 */
extern yt_ret_t  yt_vlan_macBasedVlan_table_del(yt_unit_t unit, yt_mac_vlan_table_idx_t tableIdx);

/**
 * @internal      yt_vlan_ipSubnetBasedVlan_table_add
 * @endinternal
 *
 * @brief         Set ip subnet based vlan table and return the table index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     pIpSubnetVlanRule   -ip subnet vlan entry rule
 * @param[in]     pIpSubnetVlanAction -ip subnet vlan entry action
 * @param[out]    pTableIdx           -table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  yt_vlan_ipSubnetBasedVlan_table_add(yt_unit_t unit, const yt_ip_subnet_vlan_rule_t *pIpSubnetVlanRule, const yt_ip_subnet_vlan_action_t *pIpSubnetVlanAction, yt_ip_subnet_vlan_table_idx_t *pTableIdx);

/**
 * @internal      yt_vlan_ipSubnetBasedVlan_table_add_by_index
 * @endinternal
 *
 * @brief         Set ip subnet based vlan table by the table index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     tableIdx            -table index
 * @param[in]     pIpSubnetVlanRule   -ip subnet vlan entry rule
 * @param[in]     pIpSubnetVlanAction -ip subnet vlan entry action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORT        -port err
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  yt_vlan_ipSubnetBasedVlan_table_add_by_index(yt_unit_t unit, yt_ip_subnet_vlan_table_idx_t tableIdx, const yt_ip_subnet_vlan_rule_t *pIpSubnetVlanRule, const yt_ip_subnet_vlan_action_t *pIpSubnetVlanAction);

/**
 * @internal      yt_vlan_ipSubnetBasedVlan_table_get
 * @endinternal
 *
 * @brief         Get ip subnet based vlan table by the table index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     tableIdx            -table index
 * @param[in]     pIpSubnetVlanRule   -ip subnet vlan entry rule
 * @param[in]     pIpSubnetVlanAction -ip subnet vlan entry action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 */
extern yt_ret_t  yt_vlan_ipSubnetBasedVlan_table_get(yt_unit_t unit, yt_ip_subnet_vlan_table_idx_t tableIdx, yt_ip_subnet_vlan_rule_t *pIpSubnetVlanRule, yt_ip_subnet_vlan_action_t *pIpSubnetVlanAction);


/**
 * @internal      yt_vlan_ipSubnetBasedVlan_table_del
 * @endinternal
 *
 * @brief         Delete ip subnet based vlan table by the table index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     tableIdx            -table index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 */
extern yt_ret_t  yt_vlan_ipSubnetBasedVlan_table_del(yt_unit_t unit, yt_ip_subnet_vlan_table_idx_t tableIdx);

/**
 * @internal      yt_vlan_ipSubnetBasedVlan_bypass_arp_set
 * @endinternal
 *
 * @brief         Set arp type check for ip subnet based vlan table
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 */
extern yt_ret_t  yt_vlan_ipSubnetBasedVlan_bypass_arp_set(yt_unit_t unit, yt_enable_t enable);

/**
 * @internal      yt_vlan_ipSubnetBasedVlan_bypass_arp_get
 * @endinternal
 *
 * @brief         Get arp type check of ip subnet based vlan table
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 */
extern yt_ret_t  yt_vlan_ipSubnetBasedVlan_bypass_arp_get(yt_unit_t unit, yt_enable_t *pEnable);

/**
 * @internal      yt_vlan_policy_idx_mapping_set
 * @endinternal
 *
 * @brief         set the mapping vlan policy of each vlan
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     vlanPolicyIdx       -vlan policy index,start from 1,0 means none match vlan policy index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  yt_vlan_policy_idx_mapping_set(yt_unit_t unit, yt_vlan_t vid,  uint8_t vlanPolicyIdx);

/**
 * @internal      yt_vlan_policy_idx_mapping_get
 * @endinternal
 *
 * @brief         get the mapping vlan policy of each vlan
 * @note          APPLICABLE DEVICES - Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[pot]    pVlanPolicyIdx      -vlan policy index,start from 1,0 means none match vlan policy index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  yt_vlan_policy_idx_mapping_get(yt_unit_t unit, yt_vlan_t vid,  uint8_t *pVlanPolicyIdx);

/**
 * @internal      yt_vlan_policy_table_add
 * @endinternal
 *
 * @brief         add vlan policy table
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     pVlanPolicyInfo     -vlan policy info
 * @param[out]    pVlanPolicyIdx      -vlan policy index
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORTLIST     -portlist err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  yt_vlan_policy_table_add(yt_unit_t unit, const yt_vlan_policy_info_t *pVlanPolicyInfo,  uint8_t *pVlanPolicyIdx);

/**
 * @internal      yt_vlan_policy_table_add_by_index
 * @endinternal
 *
 * @brief         add vlan policy table by index
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vlanPolicyIdx       -vlan policy index,start from 1
 * @param[in]     pVlanPolicyInfo     -vlan policy info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_PORTLIST    -portlist err
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  yt_vlan_policy_table_add_by_index(yt_unit_t unit, uint8_t vlanPolicyIdx, const yt_vlan_policy_info_t *pVlanPolicyInfo);

/**
 * @internal      yt_vlan_policy_table_get
 * @endinternal
 *
 * @brief         get vlan policy table
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vlanPolicyIdx       -vlan policy index,start from 1
 * @param[out]    pVlanPolicyInfo     -vlan policy info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 * @retval        CMM_ERR_EXCEED_RANGE -input value out of range
 */
extern yt_ret_t  yt_vlan_policy_table_get(yt_unit_t unit, uint8_t vlanPolicyIdx, yt_vlan_policy_info_t *pVlanPolicyInfo);

/**
 * @internal      yt_vlan_policy_table_del
 * @endinternal
 *
 * @brief         delete vlan policy table
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     vlanPolicyIdx       -vlan policy index,start from 1
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input value err
 * @retval        CMM_ERR_NOT_INIT    -not init
 * @retval        CMM_ERR_NULL_POINT  -point is NULL
 */
extern yt_ret_t  yt_vlan_policy_table_del(yt_unit_t unit, uint8_t vlanPolicyIdx);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
