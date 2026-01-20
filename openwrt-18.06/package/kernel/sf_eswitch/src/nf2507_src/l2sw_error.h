/**  @file
  *  @brief    brief  description
  *  @author   Netforward
  *  @date     2020.5.7
  *  @version  1.0
  *  @note
  *  detailed  description
  *  $LastChangedDate$
  *  $LastChangedRevision$
  *  $LastChangedBy$
  */

#ifndef __COMMON_RT_ERROR_H__
#define __COMMON_RT_ERROR_H__

/*
 * Include Files
 */

/*
 * Data Type Declaration
 */
typedef enum rt_error_code_e
{
    L2SW_ERR_FAILED = -1,                             /* General Error                                                                    */

    /* 0x0000xxxx for common error code */
    L2SW_ERR_OK = 0,                                  /* 0x00000000, OK                                                                   */
    L2SW_ERR_INPUT,                                   /* 0x00000001, invalid input parameter                                              */
    L2SW_ERR_UNIT_ID,                                 /* 0x00000002, invalid unit id                                                      */
    L2SW_ERR_PORT_ID,                                 /* 0x00000003, invalid port id                                                      */
    L2SW_ERR_PORT_MASK,                               /* 0x00000004, invalid port mask                                                    */
    L2SW_ERR_PORT_LINKDOWN,                           /* 0x00000005, link down port status                                                */
    L2SW_ERR_ENTRY_INDEX,                             /* 0x00000006, invalid entry index                                                  */
    L2SW_ERR_NULL_POINTER,                            /* 0x00000007, input parameter is null pointer                                      */
    L2SW_ERR_QUEUE_ID,                                /* 0x00000008, invalid queue id                                                     */
    L2SW_ERR_QUEUE_NUM,                               /* 0x00000009, invalid queue number                                                 */
    L2SW_ERR_BUSYWAIT_TIMEOUT,                        /* 0x0000000a, busy watting time out                                                */
    L2SW_ERR_MAC,                                     /* 0x0000000b, invalid mac address                                                  */
    L2SW_ERR_OUT_OF_RANGE,                            /* 0x0000000c, input parameter out of range                                         */
    L2SW_ERR_CHIP_NOT_SUPPORTED,                      /* 0x0000000d, functions not supported by this chip model                           */
    L2SW_ERR_SMI,                                     /* 0x0000000e, SMI error                                                            */
    L2SW_ERR_NOT_INIT,                                /* 0x0000000f, The module is not initial                                            */
    L2SW_ERR_CHIP_NOT_FOUND,                          /* 0x00000010, The chip can not found                                               */
    L2SW_ERR_NOT_ALLOWED,                             /* 0x00000011, actions not allowed by the function                                  */
    L2SW_ERR_DRIVER_NOT_FOUND,                        /* 0x00000012, The driver can not found                                             */
    L2SW_ERR_SEM_LOCK_FAILED,                         /* 0x00000013, Failed to lock semaphore                                             */
    L2SW_ERR_SEM_UNLOCK_FAILED,                       /* 0x00000014, Failed to unlock semaphore                                           */
    L2SW_ERR_ENABLE,                                  /* 0x00000015, invalid enable parameter                                             */
    L2SW_ERR_TBL_FULL,                                /* 0x00000016, input table full                                                     */

    /* 0x0001xxxx for vlan */
    L2SW_ERR_VLAN_VID = 0x00010000,                   /* 0x00010000, invalid vid                                                          */
    L2SW_ERR_VLAN_PRIORITY,                           /* 0x00010001, invalid 1p priority                                                  */
    L2SW_ERR_VLAN_EMPTY_ENTRY,                        /* 0x00010002, emtpy entry of vlan table                                            */
    L2SW_ERR_VLAN_ACCEPT_FRAME_TYPE,                  /* 0x00010003, invalid accept frame type                                            */
    L2SW_ERR_VLAN_EXIST,                              /* 0x00010004, vlan is exist                                                        */
    L2SW_ERR_VLAN_ENTRY_NOT_FOUND,                    /* 0x00010005, specified vlan entry not found                                       */
    L2SW_ERR_VLAN_PORT_MBR_EXIST,                     /* 0x00010006, member port exist in the specified vlan                              */
    L2SW_ERR_VLAN_PROTO_AND_PORT,                     /* 0x00010008, invalid protocol and port based vlan                              */

    /* 0x0002xxxx for svlan */
    L2SW_ERR_SVLAN_ENTRY_INDEX = 0x00020000,          /* 0x00020000, invalid svid entry no                                                */
    L2SW_ERR_SVLAN_ETHER_TYPE,                        /* 0x00020001, invalid SVLAN ether type                                             */
    L2SW_ERR_SVLAN_TABLE_FULL,                        /* 0x00020002, no empty entry in SVLAN table                                        */
    L2SW_ERR_SVLAN_ENTRY_NOT_FOUND,                   /* 0x00020003, specified svlan entry not found                                      */
    L2SW_ERR_SVLAN_EXIST,                             /* 0x00020004, SVLAN entry is exist                                                 */
    L2SW_ERR_SVLAN_VID,                               /* 0x00020005, invalid svid                                                         */

    /* 0x0003xxxx for MSTP */
    L2SW_ERR_MSTI = 0x00030000,                       /* 0x00030000, invalid msti                                                         */
    L2SW_ERR_MSTP_STATE,                              /* 0x00030001, invalid spanning tree status                                         */
    L2SW_ERR_MSTI_EXIST,                              /* 0x00030002, MSTI exist                                                           */
    L2SW_ERR_MSTI_NOT_EXIST,                          /* 0x00030003, MSTI not exist                                                       */

    /* 0x0004xxxx for BUCKET */
    L2SW_ERR_TIMESLOT = 0x00040000,                   /* 0x00040000, invalid time slot                                                    */
    L2SW_ERR_TOKEN,                                   /* 0x00040001, invalid token amount                                                 */
    L2SW_ERR_RATE,                                    /* 0x00040002, invalid rate                                                         */
    L2SW_ERR_TICK,                                    /* 0x00040003, invalid tick                                                 */

    /* 0x0005xxxx for RMA */
    L2SW_ERR_RMA_ADDR = 0x00050000,                   /* 0x00050000, invalid rma mac address                                              */
    L2SW_ERR_RMA_ACTION,                              /* 0x00050001, invalid rma action                                                   */

    /* 0x0006xxxx for L2 */
    L2SW_ERR_L2_HASH_KEY = 0x00060000,                /* 0x00060000, invalid L2 Hash key                                                  */
    L2SW_ERR_L2_HASH_INDEX,                           /* 0x00060001, invalid L2 Hash index                                                */
    L2SW_ERR_L2_CAM_INDEX,                            /* 0x00060002, invalid L2 CAM index                                                 */
    L2SW_ERR_L2_ENRTYSEL,                             /* 0x00060003, invalid EntrySel                                                     */
    L2SW_ERR_L2_INDEXTABLE_INDEX,                     /* 0x00060004, invalid L2 index table(=portMask table) index                        */
    L2SW_ERR_LIMITED_L2ENTRY_NUM,                     /* 0x00060005, invalid limited L2 entry number                                      */
    L2SW_ERR_L2_AGGREG_PORT,                          /* 0x00060006, this aggregated port is not the lowest physical
                                                                   port of its aggregation group                                        */
    L2SW_ERR_L2_FID,                                  /* 0x00060007, invalid fid                                                          */
    L2SW_ERR_L2_VID,                                 /* 0x00060008, invalid cvid                                                         */
    L2SW_ERR_L2_NO_EMPTY_ENTRY,                       /* 0x00060009, no empty entry in L2 table                                           */
    L2SW_ERR_L2_ENTRY_NOTFOUND,                       /* 0x0006000a, specified entry not found                                            */
    L2SW_ERR_L2_INDEXTBL_FULL,                        /* 0x0006000b, the L2 index table is full                                           */
    L2SW_ERR_L2_INVALID_FLOWTYPE,                     /* 0x0006000c, invalid L2 flow type                                                 */
    L2SW_ERR_L2_L2UNI_PARAM,                          /* 0x0006000d, invalid L2 unicast parameter                                         */
    L2SW_ERR_L2_L2MULTI_PARAM,                        /* 0x0006000e, invalid L2 multicast parameter                                       */
    L2SW_ERR_L2_IPMULTI_PARAM,                        /* 0x0006000f, invalid L2 ip multicast parameter                                    */
    L2SW_ERR_L2_PARTIAL_HASH_KEY,                     /* 0x00060010, invalid L2 partial Hash key                                          */
    L2SW_ERR_L2_EMPTY_ENTRY,                          /* 0x00060011, the entry is empty(invalid)                                          */
    L2SW_ERR_L2_FLUSH_TYPE,                           /* 0x00060012, the flush type is invalid                                            */
    L2SW_ERR_L2_NO_CPU_PORT,                          /* 0x00060013, CPU port not exist                                                   */

    /* 0x0007xxxx for FILTER (PIE) */
    L2SW_ERR_FILTER_BLOCKNUM = 0x00070000,            /* 0x00070000, invalid block number                                                 */
    L2SW_ERR_FILTER_ENTRYIDX,                         /* 0x00070001, invalid entry index                                                  */
    L2SW_ERR_FILTER_CUTLINE,                          /* 0x00070002, invalid cutline value                                                */
    L2SW_ERR_FILTER_FLOWTBLBLOCK,                     /* 0x00070003, block belongs to flow table                                          */
    L2SW_ERR_FILTER_INACLBLOCK,                       /* 0x00070004, block belongs to ingress ACL                                         */
    L2SW_ERR_FILTER_ACTION,                           /* 0x00070005, action doesn't consist to entry type                                 */
    L2SW_ERR_FILTER_INACL_RULENUM,                    /* 0x00070006, invalid ACL rulenum                                                  */
    L2SW_ERR_FILTER_INACL_TYPE,                       /* 0x00070007, entry type isn't an ingress ACL rule                                 */
    L2SW_ERR_FILTER_INACL_EXIST,                      /* 0x00070008, ACL entry is already exit                                            */
    L2SW_ERR_FILTER_INACL_EMPTY,                      /* 0x00070009, ACL entry is empty                                                   */
    L2SW_ERR_FILTER_FLOWTBL_TYPE,                     /* 0x0007000a, entry type isn't an flow table rule                                  */
    L2SW_ERR_FILTER_FLOWTBL_RULENUM,                  /* 0x0007000b, invalid flow table rulenum                                           */
    L2SW_ERR_FILTER_FLOWTBL_EMPTY,                    /* 0x0007000c, flow table entry is empty                                            */
    L2SW_ERR_FILTER_FLOWTBL_EXIST,                    /* 0x0007000d, flow table entry is already exist                                    */
    L2SW_ERR_FILTER_METER_ID,                         /* 0x0007000e, invalid metering id                                                  */
    L2SW_ERR_FILTER_LOG_ID,                           /* 0x0007000f, invalid log id                                                       */
    L2SW_ERR_FILTER_INACL_NONE_BEGIN_IDX,             /* 0x00070010, entry index is not starting index of a group of rules                */
    L2SW_ERR_FILTER_INACL_ACT_NOT_SUPPORT,            /* 0x00070011, action not support                                                    */
    L2SW_ERR_FILTER_INACL_RULE_NOT_SUPPORT,           /* 0x00070012, rule not support                                                   */

    /* 0x0008xxxx for ACL Rate Limit */
    L2SW_ERR_ACLRL_HTHR = 0x00080000,                 /* 0x00080000, invalid high threshold                                               */
    L2SW_ERR_ACLRL_TIMESLOT,                          /* 0x00080001, invalid time slot                                                    */
    L2SW_ERR_ACLRL_TOKEN,                             /* 0x00080002, invalid token amount                                                 */
    L2SW_ERR_ACLRL_RATE,                              /* 0x00080003, invalid rate                                                         */

    /* 0x0009xxxx for Link aggregation */
    L2SW_ERR_LA_CPUPORT = 0x00090000,                 /* 0x00090000, CPU port can not be aggregated port                                  */
    L2SW_ERR_LA_TRUNK_ID,                             /* 0x00090001, invalid trunk id                                                     */
    L2SW_ERR_LA_PORTMASK,                             /* 0x00090002, invalid port mask                                                    */
    L2SW_ERR_LA_HASHMASK,                             /* 0x00090003, invalid hash mask                                                    */
    L2SW_ERR_LA_DUMB,                                 /* 0x00090004, this API should be used in 802.1ad dumb mode                         */
    L2SW_ERR_LA_PORTNUM_DUMB,                         /* 0x00090005, it can only aggregate at most four ports when 802.1ad dumb mode      */
    L2SW_ERR_LA_PORTNUM_NORMAL,                       /* 0x00090006, it can only aggregate at most eight ports when 802.1ad normal mode   */
    L2SW_ERR_LA_MEMBER_OVERLAP,                       /* 0x00090007, the specified port mask is overlapped with other group               */
    L2SW_ERR_LA_NOT_MEMBER_PORT,                      /* 0x00090008, the port is not a member port of the trunk                           */
    L2SW_ERR_LA_TRUNK_NOT_EXIST,                      /* 0x00090009, the trunk doesn't exist                                              */


    /* 0x000axxxx for storm filter */
    L2SW_ERR_SFC_TICK_PERIOD = 0x000a0000,            /* 0x000a0000, invalid SFC tick period                                              */
    L2SW_ERR_SFC_UNKNOWN_GROUP,                       /* 0x000a0001, Unknown Storm filter group                                           */

    /* 0x000bxxxx for pattern match */
    L2SW_ERR_PM_MASK = 0x000b0000,                    /* 0x000b0000, invalid pattern length. Pattern length should be 8                   */
    L2SW_ERR_PM_LENGTH,                               /* 0x000b0001, invalid pattern match mask, first byte must care                     */
    L2SW_ERR_PM_MODE,                                 /* 0x000b0002, invalid pattern match mode                                           */

    /* 0x000cxxxx for input bandwidth control */
    L2SW_ERR_INBW_TICK_PERIOD = 0x000c0000,           /* 0x000c0000, invalid tick period for input bandwidth control                      */
    L2SW_ERR_INBW_TOKEN_AMOUNT,                       /* 0x000c0001, invalid amount of token for input bandwidth control                  */
    L2SW_ERR_INBW_FCON_VALUE,                         /* 0x000c0002, invalid flow control ON threshold value for input bandwidth control  */
    L2SW_ERR_INBW_FCOFF_VALUE,                        /* 0x000c0003, invalid flow control OFF threshold value for input bandwidth control */
    L2SW_ERR_INBW_FC_ALLOWANCE,                       /* 0x000c0004, invalid allowance of incomming packet for input bandwidth control    */
    L2SW_ERR_INBW_RATE,                               /* 0x000c0005, invalid input bandwidth                                              */

    /* 0x000dxxxx for QoS */
    L2SW_ERR_QOS_1P_PRIORITY = 0x000d0000,            /* 0x000d0000, invalid 802.1P priority                                              */
    L2SW_ERR_QOS_DSCP_VALUE,                          /* 0x000d0001, invalid DSCP value                                                   */
    L2SW_ERR_QOS_INT_PRIORITY,                        /* 0x000d0002, invalid internal priority                                            */
    L2SW_ERR_QOS_SEL_DSCP_PRI,                        /* 0x000d0003, invalid DSCP selection priority                                      */
    L2SW_ERR_QOS_SEL_PORT_PRI,                        /* 0x000d0004, invalid port selection priority                                      */
    L2SW_ERR_QOS_SEL_IN_ACL_PRI,                      /* 0x000d0005, invalid ingress ACL selection priority                               */
    L2SW_ERR_QOS_SEL_CLASS_PRI,                       /* 0x000d0006, invalid classifier selection priority                                */
    L2SW_ERR_QOS_EBW_RATE,                            /* 0x000d0007, invalid egress bandwidth rate                                        */
    L2SW_ERR_QOS_SCHE_TYPE,                           /* 0x000d0008, invalid QoS scheduling type                                          */
    L2SW_ERR_QOS_QUEUE_WEIGHT,                        /* 0x000d0009, invalid Queue weight                                                 */
    L2SW_ERR_QOS_SEL_PRI_SOURCE,                      /* 0x000d000a, invalid selection of priority source                                                 */

    /* 0x000exxxx for port ability */
    L2SW_ERR_PHY_PAGE_ID = 0x000e0000,                /* 0x000e0000, invalid PHY page id                                                  */
    L2SW_ERR_PHY_REG_ID,                              /* 0x000e0001, invalid PHY reg id                                                   */
    L2SW_ERR_PHY_DATAMASK,                            /* 0x000e0002, invalid PHY data mask                                                */
    L2SW_ERR_PHY_AUTO_NEGO_MODE,                      /* 0x000e0003, invalid PHY auto-negotiation mode*/
    L2SW_ERR_PHY_SPEED,                               /* 0x000e0004, invalid PHY speed setting                                            */
    L2SW_ERR_PHY_DUPLEX,                              /* 0x000e0005, invalid PHY duplex setting                                           */
    L2SW_ERR_PHY_FORCE_ABILITY,                       /* 0x000e0006, invalid PHY force mode ability parameter                             */
    L2SW_ERR_PHY_FORCE_1000,                          /* 0x000e0007, invalid PHY force mode 1G speed setting                              */
    L2SW_ERR_PHY_TXRX,                                /* 0x000e0008, invalid PHY tx/rx                                                    */
    L2SW_ERR_PHY_ID,                                  /* 0x000e0009, invalid PHY id                                                       */
    L2SW_ERR_PHY_RTCT_NOT_FINISH,                     /* 0x000e000a, PHY RTCT in progress                                                 */

    /* 0x000fxxxx for mirror */
    L2SW_ERR_MIRROR_DIRECTION = 0x000f0000,           /* 0x000f0000, invalid error mirror direction                                       */
    L2SW_ERR_MIRROR_SESSION_FULL,                     /* 0x000f0001, mirroring session is full                                            */
    L2SW_ERR_MIRROR_SESSION_NOEXIST,                  /* 0x000f0002, mirroring session not exist                                          */
    L2SW_ERR_MIRROR_PORT_EXIST,                       /* 0x000f0003, mirroring port already exists                                        */
    L2SW_ERR_MIRROR_PORT_NOT_EXIST,                   /* 0x000f0004, mirroring port does not exists                                       */
    L2SW_ERR_MIRROR_PORT_FULL,                        /* 0x000f0005, Exceeds maximum number of supported mirroring port                   */

    /* 0x0010xxxx for stat */
    L2SW_ERR_STAT_INVALID_GLOBAL_CNTR = 0x00100000,   /* 0x00100000, Invalid Global Counter                                               */
    L2SW_ERR_STAT_INVALID_PORT_CNTR,                  /* 0x00100001, Invalid Port Counter                                                 */
    L2SW_ERR_STAT_GLOBAL_CNTR_FAIL,                   /* 0x00100002, Could not retrieve/reset Global Counter                              */
    L2SW_ERR_STAT_PORT_CNTR_FAIL,                     /* 0x00100003, Could not retrieve/reset Port Counter                                */
    L2SW_ERR_STAT_INVALID_CNTR,                       /* 0x00100004, Invalid Counter                                                      */
    L2SW_ERR_STAT_CNTR_FAIL,                          /* 0x00100005, Could not retrieve/reset Counter                                     */

    /* 0x0011xxxx for dot1x */
    L2SW_ERR_DOT1X_INVALID_DIRECTION = 0x00110000,    /* 0x00110000, Invalid Authentication Direction                                     */
    L2SW_ERR_DOT1X_PORTBASEDPNEN,                     /* 0x00110001, Port-based enable port error                                         */
    L2SW_ERR_DOT1X_PORTBASEDAUTH,                     /* 0x00110002, Port-based auth port error                                           */
    L2SW_ERR_DOT1X_PORTBASEDOPDIR,                    /* 0x00110003, Port-based opdir error                                               */
    L2SW_ERR_DOT1X_MACBASEDPNEN,                      /* 0x00110004, MAC-based enable port error                                          */
    L2SW_ERR_DOT1X_MACBASEDOPDIR,                     /* 0x00110005, MAC-based opdir error                                                */
    L2SW_ERR_DOT1X_PROC,                              /* 0x00110006, unauthorized behavior error                                          */
    L2SW_ERR_DOT1X_GVLANIDX,                          /* 0x00110007, guest vlan index error                                               */
    L2SW_ERR_DOT1X_GVLANTALK,                         /* 0x00110008, guest vlan OPDIR error                                               */
    L2SW_ERR_DOT1X_MAC_PORT_MISMATCH,                 /* 0x00110009, Auth MAC and port mismatch eror                                      */

    L2SW_ERR_END                                       /* The symbol is the latest symbol                                                  */
} rt_error_code_t;


#endif /* __COMMON_RT_ERROR_H__ */
