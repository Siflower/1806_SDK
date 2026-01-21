/**  @file
  *  @brief    brief  description
  *  @author   
  *  @date     2020.5.7
  *  @version  1.0
  *  @note
  *  detailed  description
  *  $LastChangedDate$
  *  $LastChangedRevision$
  *  $LastChangedBy$
  */

#ifndef __L2SW_API_ACL_H__
#define __L2SW_API_ACL_H__

/*
 * Data Type Declaration
 */
#define L2SW_FILTER_RAW_FIELD_NUMBER                8

#define ACL_DEFAULT_ABILITY                         0
#define ACL_DEFAULT_UNMATCH_PERMIT                  1

#define ACL_RULE_FREE                               0
#define ACL_RULE_INAVAILABLE                        1
#define ACL_RULE_CARETAG_MASK                       0x1F
#define FILTER_POLICING_MAX                         4
#define FILTER_LOGGING_MAX                          8
#define FILTER_PATTERN_MAX                          4

#define FILTER_ENACT_CVLAN_MASK         0x01
#define FILTER_ENACT_SVLAN_MASK         0x02
#define FILTER_ENACT_PRIORITY_MASK      0x04
#define FILTER_ENACT_POLICING_MASK      0x08
#define FILTER_ENACT_FWD_MASK           0x10
#define FILTER_ENACT_INTGPIO_MASK       0x20
#define FILTER_ENACT_COUNTER_MASK       0x80
#define FILTER_ENACT_INIT_MASK          0x3F

typedef enum l2sw_filter_act_cactext_e
{
    FILTER_ENACT_CACTEXT_VLANONLY=0,
    FILTER_ENACT_CACTEXT_BOTHVLANTAG,
    FILTER_ENACT_CACTEXT_TAGONLY,
    FILTER_ENACT_CACTEXT_END,


}l2sw_filter_act_cactext_t;

typedef enum l2sw_filter_act_ctagfmt_e
{
    FILTER_CTAGFMT_UNTAG=0,
    FILTER_CTAGFMT_TAG,
    FILTER_CTAGFMT_KEEP,
    FILTER_CTAGFMT_KEEP1PRMK,


}l2sw_filter_act_ctag_t;





#define L2SW_MAX_NUM_OF_FILTER_TYPE                  5
#define L2SW_MAX_NUM_OF_FILTER_FIELD                 8

#define L2SW_DOT_1AS_TIMESTAMP_UNIT_IN_WORD_LENGTH   3UL
#define L2SW_IPV6_ADDR_WORD_LENGTH                   4UL

#define FILTER_ENACT_CVLAN_TYPE(type)   (type - FILTER_ENACT_CVLAN_INGRESS)
#define FILTER_ENACT_SVLAN_TYPE(type)   (type - FILTER_ENACT_SVLAN_INGRESS)
#define FILTER_ENACT_FWD_TYPE(type)     (type - FILTER_ENACT_ADD_DSTPORT)
#define FILTER_ENACT_PRI_TYPE(type)     (type - FILTER_ENACT_PRIORITY)

#define L2SW_FILTER_FIELD_USED_MAX                   8
#define L2SW_FILTER_FIELD_INDEX(template, index)     ((template << 4) + index)

typedef enum l2sw_filter_act_type_e
{
    /* CVLAN */
    FILTER_ENACT_CVLAN_INGRESS = 0,
    FILTER_ENACT_CVLAN_EGRESS,
    FILTER_ENACT_CVLAN_SVID,
    FILTER_ENACT_POLICING_1,

    /* SVLAN */
    FILTER_ENACT_SVLAN_INGRESS,
    FILTER_ENACT_SVLAN_EGRESS,
    FILTER_ENACT_SVLAN_CVID,
    FILTER_ENACT_POLICING_2,

    /* Policing and Logging */
    FILTER_ENACT_POLICING_0,

    /* Forward */
    FILTER_ENACT_COPY_CPU,
    FILTER_ENACT_DROP = 0xa,
    FILTER_ENACT_ADD_DSTPORT,
    FILTER_ENACT_REDIRECT,
    FILTER_ENACT_MIRROR,
    FILTER_ENACT_TRAP_CPU,
    FILTER_ENACT_ISOLATION,

    /* QoS */
    FILTER_ENACT_PRIORITY = 0x10,
    FILTER_ENACT_DSCP_REMARK,
    FILTER_ENACT_1P_REMARK,
    FILTER_ENACT_POLICING_3,

    /* Interrutp and GPO */
    FILTER_ENACT_INTERRUPT,
    FILTER_ENACT_GPO,

    /*VLAN tag*/
    FILTER_ENACT_EGRESSCTAG_UNTAG = 0x16,
    FILTER_ENACT_EGRESSCTAG_TAG,
    FILTER_ENACT_EGRESSCTAG_KEEP,
    FILTER_ENACT_EGRESSCTAG_KEEPAND1PRMK,

    /* counter*/
    FILTER_ENACT_COUNTER,

    /*not set any action*/
    FILTER_ENACT_NOACTION,
    FILTER_ENACT_END,
}l2sw_filter_act_type_t;

typedef enum l2sw_filter_act_enable_e
{
    FILTER_ACT_DISABLE = 0,
    FILTER_ACT_ENABLE,
    FILTER_ACT_END,
} l2sw_filter_act_enable_t;


typedef struct
{
    l2sw_filter_act_enable_t actEnable[FILTER_ENACT_END];

    /* CVLAN acton */
    l2sw_uint32      filterCvlanVid;
    l2sw_uint32      filterCvlanIdx;
    /* SVLAN action */
    l2sw_uint32      filterSvlanVid;
    l2sw_uint32      filterSvlanIdx;

    /* Policing action */
    l2sw_uint32      filterPolicingIdx[FILTER_POLICING_MAX];

    /* Forwarding action */
    l2sw_portmask_t  filterPortmask;

    /* QOS action */
    l2sw_uint32      filterPriority;

    /*GPO*/
    l2sw_uint32      filterPin;

	/* counter action */
    l2sw_uint32      filterCounterIdx;

} l2sw_filter_action_t;

typedef struct l2sw_filter_flag_s
{
    l2sw_uint32 value;
    l2sw_uint32 mask;
} l2sw_filter_flag_t;

typedef enum l2sw_filter_care_tag_index_e
{
    CARE_TAG_CTAG = 0,
    CARE_TAG_STAG,
    CARE_TAG_PPPOE,
    CARE_TAG_IPV4,
    CARE_TAG_IPV6,
    CARE_TAG_TCP,
    CARE_TAG_UDP,
    CARE_TAG_ARP,
    CARE_TAG_RSV1,
    CARE_TAG_RSV2,
    CARE_TAG_ICMP,
    CARE_TAG_IGMP,
    CARE_TAG_LLC,
    CARE_TAG_RSV3,
    CARE_TAG_HTTP,
    CARE_TAG_RSV4,
    CARE_TAG_RSV5,
    CARE_TAG_DHCP,
    CARE_TAG_DHCPV6,
    CARE_TAG_SNMP,
    CARE_TAG_OAM,
    CARE_TAG_END,
} l2sw_filter_care_tag_index_t;

typedef struct l2sw_filter_care_tag_s
{
    l2sw_filter_flag_t tagType[CARE_TAG_END];
} l2sw_filter_care_tag_t;

typedef struct l2sw_filter_field l2sw_filter_field_t;

typedef struct
{
    l2sw_uint32 value[L2SW_DOT_1AS_TIMESTAMP_UNIT_IN_WORD_LENGTH];
} l2sw_filter_dot1as_timestamp_t;

typedef enum l2sw_filter_field_data_type_e
{
    FILTER_FIELD_DATA_MASK = 0,
    FILTER_FIELD_DATA_RANGE,
    FILTER_FIELD_DATA_END ,
} l2sw_filter_field_data_type_t;

typedef struct l2sw_filter_ip_s
{
    l2sw_uint32 dataType;
    l2sw_uint32 rangeStart;
    l2sw_uint32 rangeEnd;
    l2sw_uint32 value;
    l2sw_uint32 mask;
} l2sw_filter_ip_t;

typedef struct l2sw_filter_mac_s
{
    l2sw_uint32 dataType;
    l2sw_mac_t value;
    l2sw_mac_t mask;
    l2sw_mac_t rangeStart;
    l2sw_mac_t rangeEnd;
} l2sw_filter_mac_t;

typedef l2sw_uint32 l2sw_filter_op_t;

typedef struct l2sw_filter_value_s
{
    l2sw_uint32 dataType;
    l2sw_uint32 value;
    l2sw_uint32 mask;
    l2sw_uint32 rangeStart;
    l2sw_uint32 rangeEnd;

} l2sw_filter_value_t;

typedef struct l2sw_filter_activeport_s
{
    l2sw_portmask_t value;
    l2sw_portmask_t mask;

} l2sw_filter_activeport_t;



typedef struct l2sw_filter_tag_s
{
    l2sw_filter_value_t pri;
    l2sw_filter_flag_t cfi;
    l2sw_filter_value_t vid;
} l2sw_filter_tag_t;

typedef struct l2sw_filter_ipFlag_s
{
    l2sw_filter_flag_t xf;
    l2sw_filter_flag_t mf;
    l2sw_filter_flag_t df;
} l2sw_filter_ipFlag_t;

typedef struct
{
    l2sw_uint32 addr[L2SW_IPV6_ADDR_WORD_LENGTH];
} l2sw_filter_ip6_addr_t;

typedef struct
{
    l2sw_uint32 dataType;
    l2sw_filter_ip6_addr_t value;
    l2sw_filter_ip6_addr_t mask;
    l2sw_filter_ip6_addr_t rangeStart;
    l2sw_filter_ip6_addr_t rangeEnd;
} l2sw_filter_ip6_t;

typedef l2sw_uint32 l2sw_filter_number_t;

typedef struct l2sw_filter_pattern_s
{
    l2sw_uint32 value[FILTER_PATTERN_MAX];
    l2sw_uint32 mask[FILTER_PATTERN_MAX];
} l2sw_filter_pattern_t;

typedef struct l2sw_filter_tcpFlag_s
{
    l2sw_filter_flag_t urg;
    l2sw_filter_flag_t ack;
    l2sw_filter_flag_t psh;
    l2sw_filter_flag_t rst;
    l2sw_filter_flag_t syn;
    l2sw_filter_flag_t fin;
    l2sw_filter_flag_t ns;
    l2sw_filter_flag_t cwr;
    l2sw_filter_flag_t ece;
} l2sw_filter_tcpFlag_t;

typedef l2sw_uint32 l2sw_filter_field_raw_t;

typedef enum l2sw_filter_field_temple_input_e
{
    FILTER_FIELD_TEMPLE_INPUT_TYPE = 0,
    FILTER_FIELD_TEMPLE_INPUT_INDEX,
    FILTER_FIELD_TEMPLE_INPUT_MAX ,
} l2sw_filter_field_temple_input_t;

struct l2sw_filter_field
{
    l2sw_uint32 fieldType;

    union
    {
        /* L2 struct */
        l2sw_filter_mac_t       dmac;
        l2sw_filter_mac_t       smac;
        l2sw_filter_value_t     etherType;
        l2sw_filter_tag_t       ctag;
        l2sw_filter_tag_t       relayCtag;
        l2sw_filter_tag_t       stag;
        l2sw_filter_tag_t       l2tag;
        l2sw_filter_dot1as_timestamp_t dot1asTimeStamp;
        l2sw_filter_mac_t       mac;

        /* L3 struct */
        l2sw_filter_ip_t      sip;
        l2sw_filter_ip_t      dip;
        l2sw_filter_ip_t      ip;
        l2sw_filter_value_t   protocol;
        l2sw_filter_value_t   ipTos;
        l2sw_filter_ipFlag_t  ipFlag;
        l2sw_filter_value_t   ipOffset;
        l2sw_filter_ip6_t     sipv6;
        l2sw_filter_ip6_t     dipv6;
        l2sw_filter_ip6_t     ipv6;
        l2sw_filter_value_t   ipv6TrafficClass;
        l2sw_filter_value_t   ipv6NextHeader;
        l2sw_filter_value_t   flowLabel;

        /* L4 struct */
        l2sw_filter_value_t   tcpSrcPort;
        l2sw_filter_value_t   tcpDstPort;
        l2sw_filter_tcpFlag_t tcpFlag;
        l2sw_filter_value_t   tcpSeqNumber;
        l2sw_filter_value_t   tcpAckNumber;
        l2sw_filter_value_t   udpSrcPort;
        l2sw_filter_value_t   udpDstPort;
        l2sw_filter_value_t   icmpCode;
        l2sw_filter_value_t   icmpType;
        l2sw_filter_value_t   igmpType;

        /* pattern match */
        l2sw_filter_pattern_t pattern;

        l2sw_filter_value_t   inData;

    } filter_pattern_union;

    l2sw_uint32 fieldTemplateNo;
    l2sw_uint32 fieldTemplateIdx[L2SW_FILTER_FIELD_USED_MAX];

    struct l2sw_filter_field *next;
};

typedef enum l2sw_filter_field_type_e
{
    FILTER_FIELD_DMAC = 0,
    FILTER_FIELD_SMAC,
    FILTER_FIELD_ETHERTYPE,
    FILTER_FIELD_CTAG,
    FILTER_FIELD_STAG,

    FILTER_FIELD_IPV4_SIP,
    FILTER_FIELD_IPV4_DIP,
    FILTER_FIELD_IPV4_TOS,
    FILTER_FIELD_IPV4_PROTOCOL,
    FILTER_FIELD_IPV4_FLAG,
    FILTER_FIELD_IPV4_OFFSET,
    FILTER_FIELD_IPV6_SIPV6,
    FILTER_FIELD_IPV6_DIPV6,
    FILTER_FIELD_IPV6_TRAFFIC_CLASS,
    FILTER_FIELD_IPV6_NEXT_HEADER,

    FILTER_FIELD_TCP_SPORT,
    FILTER_FIELD_TCP_DPORT,
    FILTER_FIELD_TCP_FLAG,
    FILTER_FIELD_UDP_SPORT,
    FILTER_FIELD_UDP_DPORT,
    FILTER_FIELD_ICMP_CODE,
    FILTER_FIELD_ICMP_TYPE,
    FILTER_FIELD_IGMP_TYPE,

    FILTER_FIELD_VID_RANGE,
    FILTER_FIELD_IP_RANGE,
    FILTER_FIELD_PORT_RANGE,

    FILTER_FIELD_USER_DEFINED00,
    FILTER_FIELD_USER_DEFINED01,
    FILTER_FIELD_USER_DEFINED02,
    FILTER_FIELD_USER_DEFINED03,
    FILTER_FIELD_USER_DEFINED04,
    FILTER_FIELD_USER_DEFINED05,
    FILTER_FIELD_USER_DEFINED06,
    FILTER_FIELD_USER_DEFINED07,
    FILTER_FIELD_USER_DEFINED08,
    FILTER_FIELD_USER_DEFINED09,
    FILTER_FIELD_USER_DEFINED10,
    FILTER_FIELD_USER_DEFINED11,
    FILTER_FIELD_USER_DEFINED12,
    FILTER_FIELD_USER_DEFINED13,
    FILTER_FIELD_USER_DEFINED14,
    FILTER_FIELD_USER_DEFINED15,

    FILTER_FIELD_PATTERN_MATCH,

    FILTER_FIELD_END,
} l2sw_filter_field_type_t;


typedef enum l2sw_filter_field_type_raw_e
{
    FILTER_FIELD_RAW_UNUSED = 0,
    FILTER_FIELD_RAW_DMAC_15_0,
    FILTER_FIELD_RAW_DMAC_31_16,
    FILTER_FIELD_RAW_DMAC_47_32,
    FILTER_FIELD_RAW_SMAC_15_0,
    FILTER_FIELD_RAW_SMAC_31_16,
    FILTER_FIELD_RAW_SMAC_47_32,
    FILTER_FIELD_RAW_ETHERTYPE,
    FILTER_FIELD_RAW_STAG,
    FILTER_FIELD_RAW_CTAG,

    FILTER_FIELD_RAW_IPV4_SIP_15_0 = 0x10,
    FILTER_FIELD_RAW_IPV4_SIP_31_16,
    FILTER_FIELD_RAW_IPV4_DIP_15_0,
    FILTER_FIELD_RAW_IPV4_DIP_31_16,


    FILTER_FIELD_RAW_IPV6_SIP_15_0 = 0x20,
    FILTER_FIELD_RAW_IPV6_SIP_31_16,
    FILTER_FIELD_RAW_IPV6_DIP_15_0 = 0x28,
    FILTER_FIELD_RAW_IPV6_DIP_31_16,

    FILTER_FIELD_RAW_VIDRANGE = 0x30,
    FILTER_FIELD_RAW_IPRANGE,
    FILTER_FIELD_RAW_PORTRANGE,
    FILTER_FIELD_RAW_FIELD_VALID,

    FILTER_FIELD_RAW_FIELD_SELECT00 = 0x40,
    FILTER_FIELD_RAW_FIELD_SELECT01,
    FILTER_FIELD_RAW_FIELD_SELECT02,
    FILTER_FIELD_RAW_FIELD_SELECT03,
    FILTER_FIELD_RAW_FIELD_SELECT04,
    FILTER_FIELD_RAW_FIELD_SELECT05,
    FILTER_FIELD_RAW_FIELD_SELECT06,
    FILTER_FIELD_RAW_FIELD_SELECT07,
    FILTER_FIELD_RAW_FIELD_SELECT08,
    FILTER_FIELD_RAW_FIELD_SELECT09,
    FILTER_FIELD_RAW_FIELD_SELECT10,
    FILTER_FIELD_RAW_FIELD_SELECT11,
    FILTER_FIELD_RAW_FIELD_SELECT12,
    FILTER_FIELD_RAW_FIELD_SELECT13,
    FILTER_FIELD_RAW_FIELD_SELECT14,
    FILTER_FIELD_RAW_FIELD_SELECT15,

    FILTER_FIELD_RAW_TCPSPORT = 0x80,
    FILTER_FIELD_RAW_TCPDPORT,
    FILTER_FIELD_RAW_TCPFLAG,

    FILTER_FIELD_RAW_UDPSPORT,
    FILTER_FIELD_RAW_UDPDPORT,

    FILTER_FIELD_RAW_ICMPCODETYPE,
    FILTER_FIELD_RAW_IGMPTYPE,

    FILTER_FIELD_RAW_SPORT,
    FILTER_FIELD_RAW_DPORT,

    FILTER_FIELD_RAW_IP4TOSPORTO,
    FILTER_FIELD_RAW_IP4FLAGOFFSET,

    FILTER_FIELD_RAW_TRAFFICCLASS_NEXTHEADER,
    
    FILTER_FIELD_RAW_CPUTAG,
    FILTER_FIELD_RAW_L2PAYLOAD,
    
    FILTER_FIELD_RAW_IP6SIP0,
    FILTER_FIELD_RAW_IP6SIP1,
    FILTER_FIELD_RAW_IP6SIP2,
    FILTER_FIELD_RAW_IP6SIP3,
    FILTER_FIELD_RAW_IP6SIP4,
    FILTER_FIELD_RAW_IP6SIP5,
    FILTER_FIELD_RAW_IP6SIP6,
    FILTER_FIELD_RAW_IP6SIP7,
    
    FILTER_FIELD_RAW_IP6DIP0,
    FILTER_FIELD_RAW_IP6DIP1,
    FILTER_FIELD_RAW_IP6DIP2,
    FILTER_FIELD_RAW_IP6DIP3,
    FILTER_FIELD_RAW_IP6DIP4,
    FILTER_FIELD_RAW_IP6DIP5,
    FILTER_FIELD_RAW_IP6DIP6,
    FILTER_FIELD_RAW_IP6DIP7,
    FILTER_FIELD_RAW_PKTFORMAT,

    FILTER_FIELD_RAW_END,
} l2sw_filter_field_type_raw_t;

typedef enum l2sw_filter_flag_care_type_e
{
    FILTER_FLAG_CARE_DONT_CARE = 0,
    FILTER_FLAG_CARE_1,
    FILTER_FLAG_CARE_0,
    FILTER_FLAG_END
} l2sw_filter_flag_care_type_t;

typedef l2sw_uint32  l2sw_filter_id_t;    /* filter id type */

typedef enum l2sw_filter_invert_e
{
    FILTER_INVERT_DISABLE = 0,
    FILTER_INVERT_ENABLE,
    FILTER_INVERT_END,
} l2sw_filter_invert_t;

typedef l2sw_uint32 l2sw_filter_state_t;

typedef l2sw_uint32 l2sw_filter_unmatch_action_t;

typedef enum l2sw_filter_unmatch_action_e
{
    FILTER_UNMATCH_DROP = 0,
    FILTER_UNMATCH_PERMIT,
    FILTER_UNMATCH_END,
} l2sw_filter_unmatch_action_type_t;

typedef struct
{
    l2sw_filter_field_t      *fieldHead;
    l2sw_filter_care_tag_t   careTag;
    l2sw_filter_activeport_t activeport;

    l2sw_filter_invert_t     invert;
} l2sw_filter_cfg_t;

typedef struct
{
    l2sw_filter_field_raw_t      dataFieldRaw[L2SW_FILTER_RAW_FIELD_NUMBER];
    l2sw_filter_field_raw_t      careFieldRaw[L2SW_FILTER_RAW_FIELD_NUMBER];
    l2sw_filter_field_type_raw_t fieldRawType[L2SW_FILTER_RAW_FIELD_NUMBER];
    l2sw_filter_care_tag_t       careTag;
    l2sw_filter_activeport_t     activeport;

    l2sw_filter_invert_t         invert;
    l2sw_enable_t                valid;
} l2sw_filter_cfg_raw_t;

typedef struct
{
    l2sw_uint32 index;
    l2sw_filter_field_type_raw_t fieldType[L2SW_FILTER_RAW_FIELD_NUMBER];
} l2sw_filter_template_t;

typedef enum l2sw_field_sel_e
{
    FORMAT_DEFAULT = 0,
    FORMAT_RAW,
    FORMAT_LLC,
    FORMAT_IPV4,
    FORMAT_ARP,
    FORMAT_IPV6,
    FORMAT_IPPAYLOAD,
    FORMAT_L4PAYLOAD,
    FORMAT_END
}l2sw_field_sel_t;

typedef enum l2sw_filter_iprange_e
{
    IPRANGE_UNUSED = 0,
    IPRANGE_IPV4_SIP,
    IPRANGE_IPV4_DIP,
    IPRANGE_IPV6_SIP,
    IPRANGE_IPV6_DIP,
    IPRANGE_IPv6_SIP_127_96,
    IPRANGE_IPv6_DIP_127_96,
    IPRANGE_END
}l2sw_filter_iprange_t;

typedef enum l2sw_filter_vidrange_e
{
    VIDRANGE_UNUSED = 0,
    VIDRANGE_CVID,
    VIDRANGE_SVID,
    VIDRANGE_END
}l2sw_filter_vidrange_t;

typedef enum l2sw_filter_portrange_e
{
    PORTRANGE_UNUSED = 0,
    PORTRANGE_SPORT,
    PORTRANGE_DPORT,
    PORTRANGE_END
}l2sw_filter_portrange_t;

/* Function Name:
 *      l2sw_filter_igrAcl_init
 * Description:
 *      ACL initialization function
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_NULL_POINTER - Pointer pFilter_field or pFilter_cfg point to NULL.
 * Note:
 *      This function enable and intialize ACL function
 */
extern l2sw_api_ret_t l2sw_filter_igrAcl_init(l2sw_uint8 units);

/* Function Name:
 *      l2sw_filter_igrAcl_field_add
 * Description:
 *      Add comparison rule to an ACL configuration
 * Input:
 *      pFilter_cfg     - The ACL configuration that this function will add comparison rule
 *      pFilter_field   - The comparison rule that will be added.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_NULL_POINTER     - Pointer pFilter_field or pFilter_cfg point to NULL.
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 * Note:
 *      This function add a comparison rule (*pFilter_field) to an ACL configuration (*pFilter_cfg).
 *      Pointer pFilter_cfg points to an ACL configuration structure, this structure keeps multiple ACL
 *      comparison rules by means of linked list. Pointer pFilter_field will be added to linked
 *      list keeped by structure that pFilter_cfg points to.
 */
extern l2sw_api_ret_t l2sw_filter_igrAcl_field_add(l2sw_uint8 units, l2sw_filter_cfg_t *pFilter_cfg, l2sw_filter_field_t *pFilter_field);

/* Function Name:
 *      l2sw_filter_igrAcl_cfg_add
 * Description:
 *      Add an ACL configuration to ASIC
 * Input:
 *      filter_id       - Start index of ACL configuration.
 *      pFilter_cfg     - The ACL configuration that this function will add comparison rule
 *      pFilter_action  - Action(s) of ACL configuration.
 * Output:
 *      ruleNum - number of rules written in acl table
 * Return:
 *      L2SW_ERR_OK                               - OK
 *      L2SW_ERR_FAILED                           - Failed
 *      L2SW_ERR_SMI                              - SMI access error
 *      L2SW_ERR_NULL_POINTER                     - Pointer pFilter_field or pFilter_cfg point to NULL.
 *      L2SW_ERR_INPUT                            - Invalid input parameters.
 *      L2SW_ERR_ENTRY_INDEX                      - Invalid filter_id .
 *      L2SW_ERR_NULL_POINTER                     - Pointer pFilter_action or pFilter_cfg point to NULL.
 *      L2SW_ERR_FILTER_INACL_ACT_NOT_SUPPORT     - Action is not supported in this chip.
 *      L2SW_ERR_FILTER_INACL_RULE_NOT_SUPPORT    - Rule is not supported.
 * Note:
 *      This function store pFilter_cfg, pFilter_action into ASIC. The starting
 *      index(es) is filter_id.
 */
extern l2sw_api_ret_t l2sw_filter_igrAcl_cfg_add(l2sw_uint8 units, l2sw_filter_id_t filter_id, l2sw_filter_cfg_t *pFilter_cfg, l2sw_filter_action_t *pAction, l2sw_filter_number_t *ruleNum);

/* Function Name:
 *      l2sw_filter_igrAcl_cfg_del
 * Description:
 *      Delete an ACL configuration from ASIC
 * Input:
 *      filter_id   - Start index of ACL configuration.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_ENTRYIDX  - Invalid filter_id.
 * Note:
 *      This function delete a group of ACL rules starting from filter_id.
 */
extern l2sw_api_ret_t l2sw_filter_igrAcl_cfg_del(l2sw_uint8 units, l2sw_filter_id_t filter_id);

/* Function Name:
 *      l2sw_filter_igrAcl_cfg_delAll
 * Description:
 *      Delete all ACL entries from ASIC
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      This function delete all ACL configuration from ASIC.
 */
extern l2sw_api_ret_t l2sw_filter_igrAcl_cfg_delAll(l2sw_uint8 units);

/* Function Name:
 *      l2sw_filter_igrAcl_cfg_get
 * Description:
 *      Get one ingress acl configuration from ASIC.
 * Input:
 *      filter_id       - Start index of ACL configuration.
 * Output:
 *      pFilter_cfg     - buffer pointer of ingress acl data
 *      pFilter_action  - buffer pointer of ingress acl action
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_NULL_POINTER     - Pointer pFilter_action or pFilter_cfg point to NULL.
 *      L2SW_ERR_FILTER_ENTRYIDX  - Invalid entry index.
 * Note:
 *      This function delete all ACL configuration from ASIC.
 */
extern l2sw_api_ret_t l2sw_filter_igrAcl_cfg_get(l2sw_uint8 units, l2sw_filter_id_t filter_id, l2sw_filter_cfg_raw_t *pFilter_cfg, l2sw_filter_action_t *pAction);

/* Function Name:
 *      l2sw_filter_igrAcl_entry_add
 * Description:
 *      Add an ACL configuration to ASIC
 * Input:
 *      filter_id       - Start index of ACL configuration.
 *      templete_id     - templete id
 *      pFilter_cfg     - The ACL configuration that this function will add comparison rule
 *      pFilter_action  - Action(s) of ACL configuration.
 * Output:
 *      ruleNum - number of rules written in acl table
 * Return:
 *      L2SW_ERR_OK                               - OK
 *      L2SW_ERR_FAILED                           - Failed
 *      L2SW_ERR_SMI                              - SMI access error
 *      L2SW_ERR_NULL_POINTER                     - Pointer pFilter_field or pFilter_cfg point to NULL.
 *      L2SW_ERR_INPUT                            - Invalid input parameters.
 *      L2SW_ERR_ENTRY_INDEX                      - Invalid filter_id .
 *      L2SW_ERR_NULL_POINTER                     - Pointer pFilter_action or pFilter_cfg point to NULL.
 *      L2SW_ERR_FILTER_INACL_ACT_NOT_SUPPORT     - Action is not supported in this chip.
 *      L2SW_ERR_FILTER_INACL_RULE_NOT_SUPPORT    - Rule is not supported.
 * Note:
 *      This function store pFilter_cfg, pFilter_action into ASIC. The starting
 *      index(es) is filter_id.
 */
extern l2sw_api_ret_t l2sw_filter_igrAcl_entry_add(l2sw_uint8 units, l2sw_filter_id_t filter_id, l2sw_uint32 templete_id, l2sw_filter_cfg_raw_t *pFilter_cfg, l2sw_filter_action_t *pAction);

/* Function Name:
 *      l2sw_filter_igrAcl_unmatchAction_set
 * Description:
 *      Set action to packets when no ACL configuration match
 * Input:
 *      port    - Port id.
 *      action  - Action.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port id.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This function sets action of packets when no ACL configruation matches.
 */
extern l2sw_api_ret_t l2sw_filter_igrAcl_unmatchAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_filter_unmatch_action_t action);

/* Function Name:
 *      l2sw_filter_igrAcl_unmatchAction_get
 * Description:
 *      Get action to packets when no ACL configuration match
 * Input:
 *      port    - Port id.
 * Output:
 *      pAction - Action.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port id.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This function gets action of packets when no ACL configruation matches.
 */
extern l2sw_api_ret_t l2sw_filter_igrAcl_unmatchAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_filter_unmatch_action_t* action);

/* Function Name:
 *      l2sw_filter_igrAcl_state_set
 * Description:
 *      Set state of ingress ACL.
 * Input:
 *      port    - Port id.
 *      state   - Ingress ACL state.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port id.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This function gets action of packets when no ACL configruation matches.
 */
extern l2sw_api_ret_t l2sw_filter_igrAcl_state_set(l2sw_uint8 units, l2sw_port_t port, l2sw_filter_state_t state);

/* Function Name:
 *      l2sw_filter_igrAcl_state_get
 * Description:
 *      Get state of ingress ACL.
 * Input:
 *      port    - Port id.
 * Output:
 *      pState  - Ingress ACL state.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port id.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This function gets action of packets when no ACL configruation matches.
 */
extern l2sw_api_ret_t l2sw_filter_igrAcl_state_get(l2sw_uint8 units, l2sw_port_t port, l2sw_filter_state_t* state);

/* Function Name:
 *      l2sw_filter_igrAcl_template_set
 * Description:
 *      Set template of ingress ACL.
 * Input:
 *      aclTemplate - Ingress ACL template
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Invalid input parameters.
 * Note:
 *      This function set ACL template.
 */
extern l2sw_api_ret_t l2sw_filter_igrAcl_template_set(l2sw_uint8 units, l2sw_filter_template_t *aclTemplate);

/* Function Name:
 *      l2sw_filter_igrAcl_template_get
 * Description:
 *      Get template of ingress ACL.
 * Input:
 *      None
 * Output:
 *      aclTemplate - Ingress ACL template
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 *      This function gets template of ACL.
 */
extern l2sw_api_ret_t l2sw_filter_igrAcl_template_get(l2sw_uint8 units, l2sw_filter_template_t *aclTemplate);

/* Function Name:
 *      l2sw_filter_igrAcl_field_sel_set
 * Description:
 *      Set user defined field selectors in HSB
 * Input:
 *      index       - index of field selector 0-15
 *      format      - Format of field selector
 *      offset      - Retrieving data offset
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 *      System support 16 user defined field selctors.
 *      Each selector can be enabled or disable.
 *      User can defined retrieving 16-bits in many predefiend
 *      standard l2/l3/l4 payload.
 */
extern l2sw_api_ret_t l2sw_filter_igrAcl_field_sel_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_field_sel_t format, l2sw_uint32 offset);

/* Function Name:
 *      l2sw_filter_igrAcl_field_sel_get
 * Description:
 *      Get user defined field selectors in HSB
 * Input:
 *      index       - index of field selector 0-15
 * Output:
 *      pFormat     - Format of field selector
 *      pOffset     - Retrieving data offset
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 *      None.
 */
extern l2sw_api_ret_t l2sw_filter_igrAcl_field_sel_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_field_sel_t *pFormat, l2sw_uint32 *pOffset);

/* Function Name:
 *      l2sw_filter_iprange_set
 * Description:
 *      Set IP Range check
 * Input:
 *      index       - index of IP Range 0-15
 *      type        - IP Range check type, 0:Delete a entry, 1: IPv4_SIP, 2: IPv4_DIP, 3:IPv6_SIP, 4:IPv6_DIP
 *      upperIp     - The upper bound of IP range
 *      lowerIp     - The lower Bound of IP range
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE    - The parameter is out of range
 *      L2SW_ERR_INPUT           - Input error
 * Note:
 *      upperIp must be larger or equal than lowerIp.
 */
extern l2sw_api_ret_t l2sw_filter_iprange_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_iprange_t type, ipaddr_t upperIp, ipaddr_t lowerIp);

/* Function Name:
 *      l2sw_filter_iprange_get
 * Description:
 *      Set IP Range check
 * Input:
 *      index       - index of IP Range 0-15
 * Output:
 *      pType        - IP Range check type, 0:Delete a entry, 1: IPv4_SIP, 2: IPv4_DIP, 3:IPv6_SIP, 4:IPv6_DIP
 *      pUpperIp     - The upper bound of IP range
 *      pLowerIp     - The lower Bound of IP range
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE    - The parameter is out of range
 * Note:
 *      upperIp must be larger or equal than lowerIp.
 */
extern l2sw_api_ret_t l2sw_filter_iprange_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_iprange_t *pType, ipaddr_t *pUpperIp, ipaddr_t *pLowerIp);

/* Function Name:
 *      l2sw_filter_vidrange_set
 * Description:
 *      Set VID Range check
 * Input:
 *      index       - index of VID Range 0-15
 *      type        - IP Range check type, 0:Delete a entry, 1: CVID, 2: SVID
 *      upperVid    - The upper bound of VID range
 *      lowerVid    - The lower Bound of VID range
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE    - The parameter is out of range
 *      L2SW_ERR_INPUT           - Input error
 * Note:
 *      upperVid must be larger or equal than lowerVid.
 */
extern l2sw_api_ret_t l2sw_filter_vidrange_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_vidrange_t type, l2sw_uint32 upperVid, l2sw_uint32 lowerVid);

/* Function Name:
 *      l2sw_filter_vidrange_get
 * Description:
 *      Get VID Range check
 * Input:
 *      index       - index of VID Range 0-15
 * Output:
 *      pType        - IP Range check type, 0:Unused, 1: CVID, 2: SVID
 *      pUpperVid    - The upper bound of VID range
 *      pLowerVid    - The lower Bound of VID range
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE    - The parameter is out of range
 * Note:
 *      None.
 */
extern l2sw_api_ret_t l2sw_filter_vidrange_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_vidrange_t *pType, l2sw_uint32 *pUpperVid, l2sw_uint32 *pLowerVid);

/* Function Name:
 *      l2sw_filter_portrange_set
 * Description:
 *      Set Port Range check
 * Input:
 *      index       - index of Port Range 0-15
 *      type        - IP Range check type, 0:Delete a entry, 1: Source Port, 2: Destnation Port
 *      upperPort   - The upper bound of Port range
 *      lowerPort   - The lower Bound of Port range
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE    - The parameter is out of range
 *      L2SW_ERR_INPUT           - Input error
 * Note:
 *      upperPort must be larger or equal than lowerPort.
 */
extern l2sw_api_ret_t l2sw_filter_portrange_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_portrange_t type, l2sw_uint32 upperPort, l2sw_uint32 lowerPort);

/* Function Name:
 *      l2sw_filter_portrange_get
 * Description:
 *      Set Port Range check
 * Input:
 *      index       - index of Port Range 0-15
 * Output:
 *      pType       - IP Range check type, 0:Delete a entry, 1: Source Port, 2: Destnation Port
 *      pUpperPort  - The upper bound of Port range
 *      pLowerPort  - The lower Bound of Port range
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE    - The parameter is out of range
 *      L2SW_ERR_INPUT           - Input error
 * Note:
 *      None.
 */
extern l2sw_api_ret_t l2sw_filter_portrange_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_portrange_t *pType, l2sw_uint32 *pUpperPort, l2sw_uint32 *pLowerPort);

/* Function Name:
 *      l2sw_filter_igrAclPolarity_set
 * Description:
 *      Set ACL Goip control palarity
 * Input:
 *      polarity - 1: High, 0: Low
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
extern l2sw_api_ret_t l2sw_filter_igrAclPolarity_set(l2sw_uint8 units, l2sw_uint32 polarity);

/* Function Name:
 *      l2sw_filter_igrAclPolarity_get
 * Description:
 *      Get ACL Goip control palarity
 * Input:
 *      None
 * Output:
 *      pPolarity - 1: High, 0: Low
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
extern l2sw_api_ret_t l2sw_filter_igrAclPolarity_get(l2sw_uint8 units, l2sw_uint32* pPolarity);

/* Function Name:
 *      l2sw_acl_globalstate_set
 * Description:
 *      Set ACL global state
 * Input:
 *      enable - 1: enable, 0: disable
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
extern l2sw_api_ret_t l2sw_acl_globalstate_set(l2sw_uint8 units, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_acl_globalstate_get
 * Description:
 *      Get ACL global state
 * Input:
 *      None
 * Output:
 *      enable - 1: enable, 0: disable
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
extern l2sw_api_ret_t l2sw_acl_globalstate_get(l2sw_uint8 units, l2sw_enable_t *enable);

/* Function Name:
 *      l2sw_acl_entry_matchnot_set
 * Description:
 *      Set ACL entry matchnot
 * Input:
 *      enable - 1: enable, 0: disable
 *      index  - acl table index
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
extern l2sw_api_ret_t l2sw_acl_entry_matchnot_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_acl_entry_matchnot_get
 * Description:
 *      Get ACL entry matchnot
 * Input:
 *      index  -acl table index
 * Output:
 *      enable - 1: enable, 0: disable
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
extern l2sw_api_ret_t l2sw_acl_entry_matchnot_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_enable_t *enable);

/* Function Name:
 *      l2sw_acl_entry_action_set
 * Description:
 *      Set ACL entry action
 * Input:
 *      enable - 1: enable, 0: disable
 *      action  - acl table index action
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
extern l2sw_api_ret_t l2sw_acl_entry_action_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 action);

/* Function Name:
 *      l2sw_acl_entry_action_get
 * Description:
 *      Get ACL entry action
 * Input:
 *      index  -acl table index action
 * Output:
 *      action  - acl table index action
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
extern l2sw_api_ret_t l2sw_acl_entry_action_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 *action);

/* Function Name:
 *      l2sw_acl_defualt_template_get
 * Description:
 *      Get ACL defualt template
 * Input:
 *      index  -acl table index action
 * Output:
 *      acltemp -acl defualt template
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
extern l2sw_api_ret_t l2sw_acl_defualt_template_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_template_t *acltemp);


#endif /* __L2SW_API_ACL_H__ */
