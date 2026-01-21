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

#ifndef _L2SW_BASE_H_
#define _L2SW_BASE_H_

#include "l2sw_reg.h"

/* (16'h0000) port_reg */

#define    L2SW_PORT_SPECIAL_CONGEST_MODE_TIMER_BASE        L2SW_REG_PKTGEN_PORT0_TIMER
#define    L2SW_PORT_SPECIAL_CONGEST_MODE_TIMER_REG(port)    (L2SW_PORT_SPECIAL_CONGEST_MODE_TIMER_BASE + ((port) << 5))

#define    L2SW_PORT_MISC_CFG_BASE                            L2SW_REG_PORT0_MISC_CFG
#define    L2SW_PORT_MISC_CFG_REG(port)                        (L2SW_PORT_MISC_CFG_BASE + ((port) << 5))
#define    L2SW_1QREMARK_ENABLE_OFFSET                         L2SW_PORT0_MISC_CFG_DOT1Q_REMARK_ENABLE_OFFSET
#define    L2SW_1QREMARK_ENABLE_MASK                        L2SW_PORT0_MISC_CFG_DOT1Q_REMARK_ENABLE_MASK

#define    L2SW_INGRESSBW_PORT_IFG_MASK                        L2SW_PORT0_MISC_CFG_INGRESSBW_IFG_MASK
#define    L2SW_VLAN_EGRESS_MDOE_MASK                        L2SW_PORT0_MISC_CFG_VLAN_EGRESS_MODE_MASK
#define    L2SW_SPECIALCONGEST_SUSTAIN_TIMER_MASK            L2SW_PORT0_MISC_CFG_CONGESTION_SUSTAIN_TIME_MASK

#define    L2SW_INGRESSBW_PORT_RATE_LSB_BASE                L2SW_REG_INGRESSBW_PORT0_RATE_CTRL0
#define    L2SW_INGRESSBW_PORT_RATE_LSB_REG(port)            (L2SW_INGRESSBW_PORT_RATE_LSB_BASE + ((port) << 5))

#define    L2SW_PORT_SMALL_IPG_REG(port)                    (L2SW_REG_PORT0_MISC_CFG + ((port)*0x20))

#define    L2SW_PORT_EEE_CFG_BASE                           L2SW_REG_PORT0_EEECFG
#define    L2SW_PORT_EEE_CFG_REG(port)                      (L2SW_REG_PORT0_EEECFG + ((port) << 5))
#define    L2SW_PORT_EEE_100M_OFFSET                        L2SW_PORT0_EEECFG_EEE_100M_OFFSET
#define    L2SW_PORT_EEE_100M_MASK                          L2SW_PORT0_EEECFG_EEE_100M_MASK
#define    L2SW_PORT_EEE_GIGA_OFFSET                        L2SW_PORT0_EEECFG_EEE_GIGA_500M_OFFSET
#define    L2SW_PORT_EEE_GIGA_MASK                          L2SW_PORT0_EEECFG_EEE_GIGA_500M_MASK


/* (16'h0200) outq_reg */

#define    L2SW_FLOWCTRL_QUEUE_DROP_ON_BASE                    L2SW_REG_FLOWCTRL_QUEUE0_DROP_ON
#define    L2SW_FLOWCTRL_QUEUE_DROP_ON_REG(queue)            (L2SW_FLOWCTRL_QUEUE_DROP_ON_BASE + queue)
#define    L2SW_FLOWCTRL_QUEUE_DROP_ON_MASK                    L2SW_FLOWCTRL_QUEUE0_DROP_ON_MASK

#define    L2SW_FLOWCTRL_PORT_DROP_ON_BASE                    L2SW_REG_FLOWCTRL_PORT0_DROP_ON
#define    L2SW_FLOWCTRL_PORT_DROP_ON_REG(PORT)                (L2SW_FLOWCTRL_PORT_DROP_ON_BASE + PORT)
#define    L2SW_FLOWCTRL_PORT_DROP_ON_MASK                    L2SW_FLOWCTRL_PORT0_DROP_ON_MASK

#define    L2SW_FLOWCTRL_PORT_GAP_REG                        L2SW_REG_FLOWCTRL_PORT_GAP
#define    L2SW_FLOWCTRL_QUEUE_GAP_REG                        L2SW_REG_FLOWCTRL_QUEUE_GAP
#define    L2SW_FLOWCTRL_PORT_QEMPTY_REG                    L2SW_REG_PORT_QEMPTY

/* (16'h0300) sch_reg */

#define    L2SW_SCHEDULE_WFQ_BURST_SIZE_REG                    L2SW_REG_SCHEDULE_WFQ_BURST_SIZE

#define    L2SW_SCHEDULE_QUEUE_TYPE_BASE                    L2SW_REG_SCHEDULE_QUEUE_TYPE_CTRL0
#define    L2SW_SCHEDULE_QUEUE_TYPE_REG(port)                (L2SW_SCHEDULE_QUEUE_TYPE_BASE + (port >> 1))
#define    L2SW_SCHEDULE_QUEUE_TYPE_OFFSET(port, queue)        ((((port) & 0x1) << 3) + queue)
#define    L2SW_SCHEDULE_QUEUE_TYPE_MASK(port, queue)         L2SW_SCHEDULE_QUEUE_TYPE_OFFSET((port), queue)

#define    L2SW_SCHEDULE_PORT_QUEUE_WFQ_WEIGHT_BASE            L2SW_REG_SCHEDULE_PORT0_QUEUE0_WFQ_WEIGHT
#define    L2SW_SCHEDULE_PORT_QUEUE_WFQ_WEIGHT_REG(port, queue)    (L2SW_SCHEDULE_PORT_QUEUE_WFQ_WEIGHT_BASE + ((port) << 3) + queue)

#define    L2SW_SCHEDULE_APR_CTRL_REG                       L2SW_REG_SCHEDULE_APR_CTRL0
#define    L2SW_SCHEDULE_APR_CTRL_OFFSET(port)                (port)
#define    L2SW_SCHEDULE_APR_CTRL_MASK(port)                (1 << L2SW_SCHEDULE_APR_CTRL_OFFSET((port)))

#define    L2SW_SCHEDULE_PORT_APR_METER_BASE                L2SW_REG_SCHEDULE_PORT0_APR_METER_CTRL0
#define    L2SW_SCHEDULE_PORT_APR_METER_REG(port, queue)    (L2SW_SCHEDULE_PORT_APR_METER_BASE + ((port) << 2) + (queue / 5))
#define    L2SW_SCHEDULE_PORT_APR_METER_OFFSET(queue)        (3 * (queue % 5))
#define    L2SW_SCHEDULE_PORT_APR_METER_MASK(queue)            (L2SW_SCHEDULE_PORT0_APR_METER_CTRL0_QUEUE0_APR_METER_MASK << L2SW_SCHEDULE_PORT_APR_METER_OFFSET(queue))

#define    L2SW_PORT_EGRESSBW_LSB_BASE                        L2SW_REG_PORT0_EGRESSBW_CTRL0
#define    L2SW_PORT_EGRESSBW_LSB_REG(port)                    (L2SW_PORT_EGRESSBW_LSB_BASE + ((port) << 1))

#define    L2SW_PORT_EGRESSBW_MSB_BASE                        L2SW_REG_PORT0_EGRESSBW_CTRL1
#define    L2SW_PORT_EGRESSBW_MSB_REG(port)                    (L2SW_PORT_EGRESSBW_MSB_BASE + ((port) << 1))

/* (16'h0500) table_reg */

#define    L2SW_TABLE_ACCESS_CTRL_REG                        L2SW_REG_TABLE_ACCESS_CTRL

#define    L2SW_TABLE_ACCESS_ADDR_REG                        L2SW_REG_TABLE_ACCESS_ADDR

#define    L2SW_TABLE_ACCESS_STATUS_REG                        L2SW_REG_TABLE_LUT_ADDR

#define    L2SW_TABLE_ACCESS_WRDATA_BASE                    L2SW_REG_TABLE_WRITE_DATA0
#define    L2SW_TABLE_ACCESS_WRDATA_REG(index)                (L2SW_TABLE_ACCESS_WRDATA_BASE + index)

#define    L2SW_TABLE_ACCESS_RDDATA_BASE                    L2SW_REG_TABLE_READ_DATA0
#define    L2SW_TABLE_ACCESS_RDDATA_REG(index)                (L2SW_TABLE_ACCESS_RDDATA_BASE + index)



/* (16'h0600) acl_reg */

#define    L2SW_ACL_RULE_TEMPLATE_CTRL_BASE                    L2SW_REG_ACL_RULE_TEMPLATE0_CTRL0
#define    L2SW_ACL_RULE_TEMPLATE_CTRL_REG(template)        (L2SW_ACL_RULE_TEMPLATE_CTRL_BASE + template * 0x4)
#define    L2SW_ACL_TEMPLATE_FIELD_OFFSET(field)            ((field & 0x01) <<3)
#define    L2SW_ACL_TEMPLATE_FIELD_MASK(field)                (0x3F << L2SW_ACL_TEMPLATE_FIELD_OFFSET(field))

#define    L2SW_ACL_ACTION_CTRL_BASE                        L2SW_REG_ACL_ACTION_CTRL0
#define    L2SW_ACL_ACTION_CTRL_REG(rule)                   (L2SW_ACL_ACTION_CTRL_BASE + (rule >> 1))
#define    L2SW_ACL_ACTION_CTRL2_BASE                        L2SW_REG_ACL_ACTION_CTRL32
#define    L2SW_ACL_ACTION_CTRL2_REG(rule)                  (L2SW_ACL_ACTION_CTRL2_BASE + ((rule-64) >> 1))

#define    L2SW_ACL_OP_NOT_OFFSET(rule)                        (6 + ((rule & 0x1) << 3))
#define    L2SW_ACL_OP_NOT_MASK(rule)                        (1 << L2SW_ACL_OP_NOT_OFFSET(rule))

#define    L2SW_ACL_OP_ACTION_OFFSET(rule)                    ((rule & 0x1) << 3)
#if defined(USE_L2SW_SDK)
#define    L2SW_ACL_OP_ACTION_MASK(rule)                    (0xBF << L2SW_ACL_OP_ACTION_OFFSET(rule)) 
#else
#define    L2SW_ACL_OP_ACTION_MASK(rule)                    (0x3F << L2SW_ACL_OP_ACTION_OFFSET(rule))
#endif

#define    L2SW_ACL_ENABLE_REG                                L2SW_REG_ACL_ENABLE
#define    L2SW_ACL_UNMATCH_PERMIT_REG                        L2SW_REG_ACL_UNMATCH_PERMIT

/* (16'h0700) cvlan_reg */

#define    L2SW_VLAN_PVID_CTRL_BASE                            L2SW_REG_VLAN_PVID_CTRL0
#define    L2SW_VLAN_PVID_CTRL_REG(port)                    (L2SW_VLAN_PVID_CTRL_BASE + ((port) >> 1))
#define    L2SW_PORT_VIDX_OFFSET(port)                        (((port) &1)<<3)
#define    L2SW_PORT_VIDX_MASK(port)                        (L2SW_PORT0_VIDX_MASK << L2SW_PORT_VIDX_OFFSET((port)))

#define    L2SW_VLAN_PPB_VALID_BASE                            L2SW_REG_VLAN_PPB0_VALID
#define    L2SW_VLAN_PPB_VALID_REG(item)                    (L2SW_VLAN_PPB_VALID_BASE + (item << 3))

#define    L2SW_VLAN_PPB_CTRL_BASE                            L2SW_REG_VLAN_PPB0_CTRL0
#define    L2SW_VLAN_PPB_CTRL_REG(item, port)               (L2SW_VLAN_PPB_CTRL_BASE + (item << 3) + ((port) / 3) )
#define    L2SW_VLAN_PPB_CTRL_OFFSET(port)                    (((port) % 3) * 5)
#define    L2SW_VLAN_PPB_CTRL_MASK(port)                    (L2SW_VLAN_PPB0_CTRL0_PORT0_INDEX_MASK << L2SW_VLAN_PPB_CTRL_OFFSET((port)))

#define    L2SW_VLAN_PPB_FRAMETYPE_BASE                    L2SW_REG_VLAN_PPB0_CTRL2
#define    L2SW_VLAN_PPB_FRAMETYPE_REG(item)               (L2SW_VLAN_PPB_FRAMETYPE_BASE + (item << 3))
#define    L2SW_VLAN_PPB_FRAMETYPE_MASK                    L2SW_VLAN_PPB0_CTRL2_FRAME_TYPE_MASK

#define    L2SW_VLAN_PPB_ETHERTYPR_BASE                        L2SW_REG_VLAN_PPB0_CTRL3
#define    L2SW_VLAN_PPB_ETHERTYPR_REG(item)                (L2SW_VLAN_PPB_ETHERTYPR_BASE + (item << 3))

#define    L2SW_VLAN_MEMBER_CONFIGURATION_BASE                L2SW_REG_VLAN_MEMBER_CONFIGURATION0_CTRL0


#define    L2SW_VLAN_CTRL_REG                                L2SW_REG_VLAN_CTRL

#define    L2SW_VLAN_INGRESS_REG                            L2SW_REG_VLAN_INGRESS

#define    L2SW_VLAN_ACCEPT_FRAME_TYPE_BASE                    L2SW_REG_VLAN_ACCEPT_FRAME_TYPE_CTRL0
#define    L2SW_VLAN_ACCEPT_FRAME_TYPE_REG(port)            (L2SW_VLAN_ACCEPT_FRAME_TYPE_BASE + ((port) >> 3))
#define    L2SW_VLAN_ACCEPT_FRAME_TYPE_MASK(port)           (L2SW_PORT0_FRAME_TYPE_MASK << (((port) & 0x7) << 1))

#define    L2SW_PORT_EFID_BASE                                L2SW_REG_PORT_EFID_CTRL0
#define    L2SW_PORT_EFID_REG(port)                            (L2SW_PORT_EFID_BASE + ((port) >> 2))
#define    L2SW_PORT_EFID_OFFSET(port)                         (((port) & 0x3) << 2)
#define    L2SW_PORT_EFID_MASK(port)                        (L2SW_PORT0_EFID_MASK << L2SW_PORT_EFID_OFFSET(port))

#define    L2SW_PORT_PBFIDEN_REG                            L2SW_REG_PORT_PBFIDEN

#define    L2SW_PORT_PBFID_BASE                             L2SW_REG_PORT0_PBFID
#define    L2SW_PORT_PBFID_REG(port)                        (L2SW_PORT_PBFID_BASE + port)

/* (16'h0800) dpm_reg */

#define    L2SW_RMA_CTRL_BASE                                L2SW_REG_RMA_CTRL00


#define    L2SW_VLAN_PORTBASED_PRIORITY_BASE                L2SW_REG_VLAN_PORTBASED_PRIORITY_CTRL0
#define    L2SW_VLAN_PORTBASED_PRIORITY_REG(port)            (L2SW_VLAN_PORTBASED_PRIORITY_BASE + ((port) >> 2))
#define    L2SW_VLAN_PORTBASED_PRIORITY_OFFSET(port)        (((port) & 0x3) << 2)
#define    L2SW_VLAN_PORTBASED_PRIORITY_MASK(port)            (0x7 << L2SW_VLAN_PORTBASED_PRIORITY_OFFSET(port))

#define    L2SW_VLAN_PPB_PRIORITY_ITEM_BASE                    L2SW_REG_VLAN_PPB_PRIORITY_ITEM0_CTRL0
#define    L2SW_VLAN_PPB_PRIORITY_ITEM_REG(port, item)        (L2SW_VLAN_PPB_PRIORITY_ITEM_BASE + (item << 2)+ ((port)>>2))
#define    L2SW_VLAN_PPB_PRIORITY_ITEM_OFFSET(port)            (((port) & 0x3) <<2)
#define    L2SW_VLAN_PPB_PRIORITY_ITEM_MASK(port)            (L2SW_VLAN_PPB_PRIORITY_ITEM0_CTRL0_PORT0_PRIORITY_MASK << L2SW_VLAN_PPB_PRIORITY_ITEM_OFFSET(port))

#define    L2SW_QOS_1Q_PRIORITY_REMAPPING_BASE                L2SW_REG_QOS_1Q_PRIORITY_REMAPPING_CTRL0
#define    L2SW_QOS_1Q_PRIORITY_REMAPPING_REG(pri)            (L2SW_QOS_1Q_PRIORITY_REMAPPING_BASE + (pri >> 2))
#define    L2SW_QOS_1Q_PRIORITY_REMAPPING_OFFSET(pri)        ((pri & 0x3) << 2)
#define    L2SW_QOS_1Q_PRIORITY_REMAPPING_MASK(pri)            (0x7 << L2SW_QOS_1Q_PRIORITY_REMAPPING_OFFSET(pri))

#define    L2SW_QOS_DSCP_TO_PRIORITY_BASE                    L2SW_REG_QOS_DSCP_TO_PRIORITY_CTRL0
#define    L2SW_QOS_DSCP_TO_PRIORITY_REG(dscp)                (L2SW_QOS_DSCP_TO_PRIORITY_BASE + (dscp >> 2))
#define    L2SW_QOS_DSCP_TO_PRIORITY_OFFSET(dscp)            ((dscp & 0x3) << 2)
#define    L2SW_QOS_DSCP_TO_PRIORITY_MASK(dscp)                (0x7 << L2SW_QOS_DSCP_TO_PRIORITY_OFFSET(dscp))

#define    L2SW_QOS_PORTBASED_PRIORITY_BASE                    L2SW_REG_QOS_PORTBASED_PRIORITY_CTRL0
#define    L2SW_QOS_PORTBASED_PRIORITY_REG(port)            (L2SW_QOS_PORTBASED_PRIORITY_BASE + ((port) >> 2))
#define    L2SW_QOS_PORTBASED_PRIORITY_OFFSET(port)            (((port) & 0x3) << 2)
#define    L2SW_QOS_PORTBASED_PRIORITY_MASK(port)            (0x7 << L2SW_QOS_PORTBASED_PRIORITY_OFFSET(port))

#define    L2SW_QOS_INTERNAL_PRIORITY_DECISION_BASE            L2SW_REG_QOS_INTERNAL_PRIORITY_DECISION_CTRL0
#define    L2SW_QOS_INTERNAL_PRIORITY_DECISION_REG(src)        (L2SW_QOS_INTERNAL_PRIORITY_DECISION_BASE + (src >> 1))
#define    L2SW_QOS_INTERNAL_PRIORITY_DECISION_OFFSET(src)  ((src & 1) << 3)
#define    L2SW_QOS_INTERNAL_PRIORITY_DECISION_MASK(src)    (L2SW_QOS_INTERNAL_PRIORITY_DECISION_CTRL0_QOS_PORT_WEIGHT_MASK << L2SW_QOS_INTERNAL_PRIORITY_DECISION2_OFFSET(src))

#define    L2SW_QOS_INTERNAL_PRIORITY_DECISION2_BASE            L2SW_REG_QOS_INTERNAL_PRIORITY_DECISION2_CTRL0
#define    L2SW_QOS_INTERNAL_PRIORITY_DECISION2_REG(src)        (L2SW_QOS_INTERNAL_PRIORITY_DECISION2_BASE + (src >> 1))
#define    L2SW_QOS_INTERNAL_PRIORITY_DECISION2_OFFSET(src)  ((src & 1) << 3)
#define    L2SW_QOS_INTERNAL_PRIORITY_DECISION2_MASK(src)    (L2SW_QOS_INTERNAL_PRIORITY_DECISION2_CTRL0_QOS_PORT_WEIGHT_MASK << L2SW_QOS_INTERNAL_PRIORITY_DECISION2_OFFSET(src))

#define    L2SW_QOS_INTERNAL_PRIORITY_DECISION_IDX_CTRL            L2SW_REG_QOS_INTERNAL_PRIORITY_DECISION_IDX
#define    L2SW_QOS_INTERNAL_PRIORITY_DECISION_IDX(port)  (1 << (port))

#define    L2SW_QOS_PRIPORITY_REMAPPING_IN_CPU_BASE            L2SW_REG_QOS_PRIORITY_REMAPPING_IN_CPU_CTRL0
#define    L2SW_QOS_PRIPORITY_REMAPPING_IN_CPU_REG(pri)        (L2SW_QOS_PRIPORITY_REMAPPING_IN_CPU_BASE + (pri >> 2))
#define    L2SW_QOS_PRIPORITY_REMAPPING_IN_CPU_OFFSET(pri)  ((pri & 0x3) << 2)
#define    L2SW_QOS_PRIPORITY_REMAPPING_IN_CPU_MASK(pri)    (L2SW_QOS_PRIORITY_REMAPPING_IN_CPU_CTRL0_PRIORITY0_MASK << L2SW_QOS_PRIPORITY_REMAPPING_IN_CPU_OFFSET(pri))

#define    L2SW_QOS_TRAP_PRIORITY_CTRL0_REG                L2SW_REG_QOS_TRAP_PRIORITY0

#define    L2SW_QOS_TRAP_PRIORITY_CTRL1_REG                L2SW_REG_QOS_TRAP_PRIORITY1

#define    L2SW_QOS_DSCP_TO_DSCP_BASE                             L2SW_REG_QOS_DSCP_REMARK_DSCP_CTRL0
#define    L2SW_QOS_DSCP_TO_DSCP_REG(dscp)                     (L2SW_REG_QOS_DSCP_REMARK_DSCP_CTRL0 + (dscp >> 1))
#define    L2SW_QOS_DSCP_TO_DSCP_OFFSET(dscp)                ((dscp & 0x1) << 8)
#define    L2SW_QOS_DSCP_TO_DSCP_MASK(dscp)                   (0x3F << L2SW_QOS_DSCP_TO_DSCP_OFFSET(dscp))

#define    L2SW_UNUCAST_FLOADING_PMSK_REG                    L2SW_REG_UNDA_FLOODING_PMSK

#define    L2SW_UNMCAST_FLOADING_PMSK_REG                    L2SW_REG_UNMCAST_FLOADING_PMSK

#define    L2SW_BCAST_FLOADING_PMSK_REG                        L2SW_REG_BCAST_FLOADING_PMSK

#define    L2SW_PORT_ISOLATION_PORT_MASK_BASE                L2SW_REG_PORT_ISOLATION_PORT0_MASK
#define    L2SW_PORT_ISOLATION_PORT_MASK_REG(port)            (L2SW_PORT_ISOLATION_PORT_MASK_BASE + (port))

#define    L2SW_FORCE_CTRL_REG                                L2SW_REG_FORCE_CTRL

#define    L2SW_SOURCE_PORT_BLOCK_REG                        L2SW_REG_SOURCE_PORT_PERMIT

#define    L2SW_IPMCAST_VLAN_LEAKY_REG                        L2SW_REG_IPMCAST_VLAN_LEAKY

#define    L2SW_IPMCAST_PORTISO_LEAKY_REG                    L2SW_REG_IPMCAST_PORTISO_LEAKY

#define    L2SW_PORT_SECURIT_CTRL_REG                        L2SW_REG_PORT_SECURITY_CTRL

#define    L2SW_UNKNOWN_IPV4_MULTICAST_BASE                    L2SW_REG_UNKNOWN_IPV4_MULTICAST_CTRL0
#define    L2SW_UNKNOWN_IPV4_MULTICAST_REG(port)            (L2SW_UNKNOWN_IPV4_MULTICAST_BASE + ((port) >> 3))
#define    L2SW_UNKNOWN_IPV4_MULTICAST_OFFSET(port)            (((port) & 0x7) << 1)
#define    L2SW_UNKNOWN_IPV4_MULTICAST_MASK(port)            (L2SW_PORT0_UNKNOWN_IP4_MCAST_MASK << L2SW_UNKNOWN_IPV4_MULTICAST_OFFSET(port))

#define    L2SW_UNKNOWN_IPV6_MULTICAST_BASE                    L2SW_REG_UNKNOWN_IPV6_MULTICAST_CTRL0
#define    L2SW_UNKNOWN_IPV6_MULTICAST_REG(port)            (L2SW_UNKNOWN_IPV6_MULTICAST_BASE + ((port) >> 3))
#define    L2SW_UNKNOWN_IPV6_MULTICAST_OFFSET(port)            (((port) & 0x7) << 1)
#define    L2SW_UNKNOWN_IPV6_MULTICAST_MASK(port)            (L2SW_PORT0_UNKNOWN_IP4_MCAST_MASK << L2SW_UNKNOWN_IPV6_MULTICAST_OFFSET(port))

#define    L2SW_UNKNOWN_L2_MULTICAST_BASE                    L2SW_REG_UNKNOWN_L2_MULTICAST_CTRL0
#define    L2SW_UNKNOWN_L2_MULTICAST_REG(port)                (L2SW_UNKNOWN_L2_MULTICAST_BASE + ((port) >> 3))
#define    L2SW_UNKNOWN_L2_MULTICAST_OFFSET(port)            (((port) & 0x7) << 1)
#define    L2SW_UNKNOWN_L2_MULTICAST_MASK(port)                (L2SW_PORT0_UNKNOWN_L2_MCAST_MASK << L2SW_UNKNOWN_L2_MULTICAST_OFFSET(port))

#define    L2SW_PORT_TRUNK_CTRL_REG                            L2SW_REG_PORT_TRUNK_CTRL
#define    L2SW_PORT_TRUNK_HASH_MASK                           0x007F

#define    L2SW_PORT_TRUNK_GROUP_MASK_REG    L2SW_REG_PORT_TRUNK_GROUP_MASK
#define    L2SW_PORT_TRUNK_GROUP_MASK_OFFSET(group)    (group << 2)
#define    L2SW_PORT_TRUNK_GROUP_MASK_MASK(group)    (L2SW_PORT_TRUNK_GROUP0_MASK_MASK << L2SW_PORT_TRUNK_GROUP_MASK_OFFSET(group))

#define    L2SW_PORT_TRUNK_FLOWCTRL_REG                        L2SW_REG_PORT_TRUNK_FLOWCTRL

#define    L2SW_QOS_PORT_QUEUE_NUMBER_BASE                    L2SW_REG_QOS_PORT_QUEUE_NUMBER_CTRL0
#define    L2SW_QOS_PORT_QUEUE_NUMBER_REG(port)                (L2SW_QOS_PORT_QUEUE_NUMBER_BASE + ((port) >> 2))
#define    L2SW_QOS_PORT_QUEUE_NUMBER_OFFSET(port)            (((port) & 0x3) << 2)
#define    L2SW_QOS_PORT_QUEUE_NUMBER_MASK(port)            (0x7 << L2SW_QOS_PORT_QUEUE_NUMBER_OFFSET(port))

#define    L2SW_QOS_1Q_PRIORITY_TO_QID_BASE                    L2SW_REG_QOS_1Q_PRIORITY_TO_QID_CTRL0
#define    L2SW_QOS_1Q_PRIORITY_TO_QID_REG(index, pri)        (L2SW_QOS_1Q_PRIORITY_TO_QID_BASE + (index << 1) + (pri >> 2))
#define    L2SW_QOS_1Q_PRIORITY_TO_QID_OFFSET(pri)            ((pri & 0x3) << 2)
#define    L2SW_QOS_1Q_PRIORITY_TO_QID_MASK(pri)            (L2SW_QOS_1Q_PRIORITY_TO_QID_CTRL0_PRIORITY0_TO_QID_MASK << L2SW_QOS_1Q_PRIORITY_TO_QID_OFFSET(pri))

#define    L2SW_DEBUG_INFO_BASE                                L2SW_REG_PORT_DEBUG_INFO_CTRL0
#define    L2SW_DEBUG_INFO_REG(port)                        (L2SW_DEBUG_INFO_BASE + ((port) >>1))
#define    L2SW_DEBUG_INFO_OFFSET(port)                        (((port)&1)<<3)
#define    L2SW_DEBUG_INFO_MASK(port)                        (L2SW_PORT0_DEBUG_INFO_MASK << L2SW_DEBUG_INFO_OFFSET(port))

/* (16'h0a00) l2_reg */

#define    L2SW_VLAN_MSTI_BASE                                L2SW_REG_VLAN_MSTI0_CTRL0
#define    L2SW_VLAN_MSTI_REG(tree, port)                    (L2SW_VLAN_MSTI_BASE + (tree << 1) + ((port) >> 3))
#define    L2SW_VLAN_MSTI_OFFSET(port)                        (((port) & 0x7) << 1)
#define    L2SW_VLAN_MSTI_MASK(port)                        (L2SW_VLAN_MSTI0_CTRL0_PORT0_STATE_MASK << L2SW_VLAN_MSTI_OFFSET(port))

#define    L2SW_LUT_PORT_LEARN_LIMITNO_BASE                    L2SW_REG_LUT_PORT0_LEARN_LIMITNO
#define    L2SW_LUT_PORT_LEARN_LIMITNO_REG(port)            (L2SW_LUT_PORT_LEARN_LIMITNO_BASE + (port))

#define    L2SW_LUT_PORT_LEARN_LIMIT_BASE                      L2SW_REG_LEARN_SPEED_LIMIT
#define    L2SW_LUT_PORT_LEARN_LIMIT_REG(port)              (L2SW_LUT_PORT_LEARN_LIMIT_BASE + (port))

#define    L2SW_LUT_CFG_REG                                    L2SW_REG_LUT_CFG

#define    L2SW_LUT_AGEOUT_CTRL_REG                            L2SW_REG_LUT_AGEOUT_CTRL

#define    L2SW_FORCE_FLUSH_REG                                L2SW_REG_FORCE_FLUSH

#define    L2SW_STORM_BCAST_REG                                L2SW_REG_STORM_BCAST

#define    L2SW_STORM_MCAST_REG                                L2SW_REG_STORM_MCAST

#define    L2SW_STORM_UNKNOWN_UCAST_REG                        L2SW_REG_STORM_UNKOWN_UCAST

#define    L2SW_STORM_UNKNOWN_MCAST_REG                        L2SW_REG_STORM_UNKOWN_MCAST

#define    L2SW_STORM_BCAST_METER_CTRL_BASE                    L2SW_REG_STORM_BCAST_METER_CTRL0
#define    L2SW_STORM_BCAST_METER_CTRL_REG(port)            (L2SW_STORM_BCAST_METER_CTRL_BASE + ((port) >> 1))
#define    L2SW_STORM_BCAST_METER_CTRL_OFFSET(port)            (((port) & 0x1) << 3)
#define    L2SW_STORM_BCAST_METER_CTRL_MASK(port)            (0xFF << L2SW_STORM_BCAST_METER_CTRL_OFFSET(port))

#define    L2SW_STORM_MCAST_METER_CTRL_BASE                    L2SW_REG_STORM_MCAST_METER_CTRL0
#define    L2SW_STORM_MCAST_METER_CTRL_REG(port)            (L2SW_STORM_MCAST_METER_CTRL_BASE + ((port) >> 1))
#define    L2SW_STORM_MCAST_METER_CTRL_OFFSET(port)            (((port) & 0x1) << 3)
#define    L2SW_STORM_MCAST_METER_CTRL_MASK(port)            (0xFF << L2SW_STORM_MCAST_METER_CTRL_OFFSET(port))

#define    L2SW_STORM_UNDA_METER_CTRL_BASE                    L2SW_REG_STORM_UNDA_METER_CTRL0
#define    L2SW_STORM_UNDA_METER_CTRL_REG(port)                (L2SW_STORM_UNDA_METER_CTRL_BASE + ((port) >> 1))
#define    L2SW_STORM_UNDA_METER_CTRL_OFFSET(port)            (((port) & 0x1) << 3)
#define    L2SW_STORM_UNDA_METER_CTRL_MASK(port)            (0xFF << L2SW_STORM_UNDA_METER_CTRL_OFFSET(port))

#define    L2SW_STORM_UNMC_METER_CTRL_BASE                    L2SW_REG_STORM_UNMC_METER_CTRL0
#define    L2SW_STORM_UNMC_METER_CTRL_REG(port)                (L2SW_STORM_UNMC_METER_CTRL_BASE + ((port) >> 1))
#define    L2SW_STORM_UNMC_METER_CTRL_OFFSET(port)            (((port) & 0x1) << 3)
#define    L2SW_STORM_UNMC_METER_CTRL_MASK(port)            (0xFF << L2SW_STORM_UNMC_METER_CTRL_OFFSET(port))

#define    L2SW_OAM_PARSER_OFFSET(port)                        ((port)*2)
#define    L2SW_OAM_PARSER_MASK(port)                        (L2SW_PORT0_PARACT_MASK << L2SW_OAM_PARSER_OFFSET(port))

#define    L2SW_OAM_MULTIPLEXER_OFFSET(port)                ((port)*2)
#define    L2SW_OAM_MULTIPLEXER_MASK(port)                    (L2SW_PORT0_PARACT_MASK << L2SW_OAM_MULTIPLEXER_OFFSET(port))

#define    L2SW_OAM_CTRL_REG                                L2SW_REG_OAM_CTRL

#define    L2SW_DOT1X_PORT_ENABLE_REG                        L2SW_REG_DOT1X_PORT_ENABLE

#define    L2SW_DOT1X_MAC_ENABLE_REG                        L2SW_REG_DOT1X_MAC_ENABLE

#define    L2SW_DOT1X_PORT_AUTH_REG                            L2SW_REG_DOT1X_PORT_AUTH

#define    L2SW_DOT1X_PORT_OPDIR_REG                        L2SW_REG_DOT1X_PORT_OPDIR

#define    L2SW_DOT1X_UNAUTH_ACT_BASE                        L2SW_REG_DOT1X_UNAUTH_ACT_W0
#define    L2SW_DOT1X_UNAUTH_ACT_OFFSET(port)                (((port) & 0x7) << 1)
#define    L2SW_DOT1X_UNAUTH_ACT_MASK(port)                    (L2SW_DOT1X_PORT0_UNAUTHBH_MASK << L2SW_DOT1X_UNAUTH_ACT_OFFSET(port))

#define    L2SW_DOT1X_CFG_REG                                L2SW_REG_DOT1X_CFG

#define    L2SW_REG_L2_LRN_CNT_BASE                            L2SW_REG_L2_LRN_CNT_CTRL0
#define    L2SW_REG_L2_LRN_CNT_REG(port)                    (L2SW_REG_L2_LRN_CNT_BASE + (port))

/* (16'h0b00) mltvlan_reg */

#define    L2SW_SVLAN_MCAST2S_ENTRY_BASE_REG(index)        (L2SW_REG_SVLAN_MCAST2S_ENTRY0_CTRL0 + index*5)

/* (16'h0c00) svlan_reg */

#define    L2SW_SVLAN_MEMBERCFG_BASE_REG(index)                (L2SW_REG_SVLAN_MEMBERCFG0_CTRL1 + index*3)
#define    L2SW_SVLAN_C2SCFG_BASE_REG(index)                  (L2SW_REG_SVLAN_C2SCFG0_CTRL0+ index*3)
#define    L2SW_SVLAN_CFG_REG                                L2SW_REG_SVLAN_CFG

/* (16'h0f00) hsactrl_reg */

#define    L2SW_SVLAN_S2C_ENTRY_BASE_REG(index)                (L2SW_REG_SVLAN_SP2C_ENTRY0_CTRL0 + index*2)

/* (16'h1000) mib_reg */

#define    L2SW_MIB_COUNTER_BASE_REG                        L2SW_REG_MIB_COUNTER0

#define    L2SW_MIB_ADDRESS_REG                                L2SW_REG_MIB_ADDRESS

#define    L2SW_MIB_CTRL_REG                                L2SW_REG_MIB_CTRL0
#define    L2SW_MIB_PORT07_MASK                                (0xFF<<L2SW_PORT0_RESET_OFFSET)

/* (16'h1100) intrpt_reg */

#define    L2SW_INTR_CTRL_REG                                L2SW_REG_INTR_CTRL

#define    L2SW_INTR_IMR_REG                                L2SW_REG_INTR_IMR

#define    L2SW_INTR_IMS_REG                                L2SW_REG_INTR_IMS

#define    L2SW_INTR_INDICATOR_BASED                        L2SW_REG_LEARN_OVER_INDICATOR
#define    L2SW_LEARN_OVER_INDICATOR_REG                    L2SW_REG_LEARN_OVER_INDICATOR

#define    L2SW_SPEED_CHANGE_INDICATOR_REG                    L2SW_REG_SPEED_CHANGE_INDICATOR

#define    L2SW_PORT_LINKDOWN_INDICATOR_REG                    L2SW_REG_PORT_LINKDOWN_INDICATOR

#define    L2SW_PORT_LINKUP_INDICATOR_REG                    L2SW_REG_PORT_LINKUP_INDICATOR

#define    L2SW_REG_METER_EXCEED_INDICATOR_BASE                L2SW_REG_METER_EXCEED_INDICATOR0
#define    L2SW_REG_METER_EXCEED_INDICATOR_REG(meter)        (L2SW_REG_METER_EXCEED_INDICATOR_BASE + (meter >> 4))
#define    L2SW_REG_METER_EXCEED_INDICATOR_OFFSET(meter)    (meter & 0xF)

/* (16'h1200) swcore_reg */

#define    L2SW_VS_TPID_REG                                    L2SW_REG_VS_TPID

#define    L2SW_SWITCH_MAC_BASE                                L2SW_REG_SWITCH_MAC0

#define    L2SW_REMARKING_CTRL_REG                            L2SW_REG_SWITCH_CTRL0

#define    L2SW_QOS_DSCP_REMARK_BASE                        L2SW_REG_QOS_DSCP_REMARK_CTRL0
#define    L2SW_QOS_DSCP_REMARK_REG(pri)                    (L2SW_QOS_DSCP_REMARK_BASE + (pri >> 1))
#define    L2SW_QOS_DSCP_REMARK_OFFSET(pri)                    (((pri) & 0x1) << 3)
#define    L2SW_QOS_DSCP_REMARK_MASK(pri)                    (0x3F << L2SW_QOS_DSCP_REMARK_OFFSET(pri))

#define    L2SW_QOS_1Q_REMARK_BASE                            L2SW_REG_QOS_1Q_REMARK_CTRL0
#define    L2SW_QOS_1Q_REMARK_REG(pri)                        (L2SW_QOS_1Q_REMARK_BASE + (pri >> 2))
#define    L2SW_QOS_1Q_REMARK_OFFSET(pri)                    ((pri & 0x3) << 2)
#define    L2SW_QOS_1Q_REMARK_MASK(pri)                        (0x7 << L2SW_QOS_1Q_REMARK_OFFSET(pri))

#define    L2SW_PTKGEN_PAYLOAD_CTRL0_REG                    L2SW_REG_PTKGEN_PAYLOAD_CTRL0

#define    L2SW_PTKGEN_PAYLOAD_CTRL1_REG                    L2SW_REG_PTKGEN_PAYLOAD_CTRL1

#define    L2SW_SVLAN_UPLINK_PORTMASK_REG                    L2SW_REG_SVLAN_UPLINK_PORTMASK

#define    L2SW_CPU_PORT_MASK_REG                            L2SW_REG_CPU_PORT_MASK

#define    L2SW_CPU_CTRL_REG                                L2SW_REG_CPU_CTRL

#define    L2SW_MIRROR_CTRL_REG                                L2SW_REG_MIRROR_CTRL


#define    L2SW_FLOWCTRL_EGRESS_QUEUE_ENABLE_BASE            L2SW_REG_FLOWCTRL_EGRESS_QUEUE_ENABLE_CTRL0
#define    L2SW_FLOWCTRL_EGRESS_QUEUE_ENABLE_REG(port)        (L2SW_FLOWCTRL_EGRESS_QUEUE_ENABLE_BASE + (port >> 1))
#define    L2SW_FLOWCTRL_EGRESS_QUEUE_ENABLE_REG_OFFSET(port)    ((port & 0x1) << 3)
#define    L2SW_FLOWCTRL_EGRESS_QUEUE_ENABLE_REG_MASK(port)    (L2SW_PORT0_QUEUE_MASK_MASK << L2SW_FLOWCTRL_EGRESS_QUEUE_ENABLE_REG_OFFSET(port))


#define    L2SW_FLOWCTRL_PORT_PAGE_COUNTER_BASE                L2SW_REG_FLOWCTRL_PORT0_PAGE_COUNTER
#define    L2SW_FLOWCTRL_PORT_PAGE_COUNTER_REG(port)        (L2SW_FLOWCTRL_PORT_PAGE_COUNTER_BASE + (port))
#define    L2SW_FLOWCTRL_PORT_PAGE_COUNTER_MASK                L2SW_FLOWCTRL_PORT0_PAGE_COUNTER_MASK

#define    L2SW_FLOWCTRL_PORT_PAGE_MAX_BASE                    L2SW_REG_FLOWCTRL_PORT0_PAGE_MAX
#define    L2SW_FLOWCTRL_PORT_PAGE_MAX_REG(port)            (L2SW_FLOWCTRL_PORT_PAGE_MAX_BASE + (port))
#define    L2SW_FLOWCTRL_PORT_PAGE_MAX_MASK                    L2SW_FLOWCTRL_PORT0_PAGE_MAX_MASK

#define    L2SW_FIELD_SELECTOR_REG(index)                    (L2SW_REG_FIELD_SELECTOR0 + index)
#define    L2SW_FIELD_SELECTOR_ENABLE_OFFSET                 L2SW_FIELD_SELECTOR0_ENABLE_OFFSET
#define    L2SW_FIELD_SELECTOR_ENABLE_MASK                    L2SW_FIELD_SELECTOR0_ENABLE_MASK
#define    L2SW_FIELD_SELECTOR_FORMAT_OFFSET                L2SW_FIELD_SELECTOR0_FORMAT_OFFSET
#define    L2SW_FIELD_SELECTOR_FORMAT_MASK                    L2SW_FIELD_SELECTOR0_FORMAT_MASK
#define    L2SW_FIELD_SELECTOR_OFFSET_OFFSET                  L2SW_FIELD_SELECTOR0_OFFSET_OFFSET
#define    L2SW_FIELD_SELECTOR_OFFSET_MASK                    L2SW_FIELD_SELECTOR0_OFFSET_MASK

/* (16'h1300) chip_reg*/

/* (16'h1400) mtrpool_reg */
#define    L2SW_METER_RATE_BASE                                L2SW_REG_METER0_RATE_CTRL0
#define    L2SW_METER_RATE_REG(meter)                        ((meter << 1) + L2SW_METER_RATE_BASE)

#define    L2SW_METER_BUCKET_SIZE_BASE                        L2SW_REG_METER0_BUCKET_SIZE
#define    L2SW_METER_BUCKET_SIZE_REG(meter)                (L2SW_METER_BUCKET_SIZE_BASE + meter)

#define    L2SW_LEAKY_BUCKET_TICK_REG                        L2SW_REG_METER_CTRL0
#define    L2SW_LEAKY_BUCKET_TICK_OFFSET                    L2SW_METER_TICK_OFFSET
#define    L2SW_LEAKY_BUCKET_TICK_MASK                        L2SW_METER_TICK_MASK

#define    L2SW_LEAKY_BUCKET_TOKEN_REG                        L2SW_REG_METER_CTRL1
#define    L2SW_LEAKY_BUCKET_TOKEN_OFFSET                    L2SW_METER_CTRL1_OFFSET
#define    L2SW_LEAKY_BUCKET_TOKEN_MASK                        L2SW_METER_CTRL1_MASK

#define    L2SW_METER_OVERRATE_INDICATOR_BASE                L2SW_REG_METER_OVERRATE_INDICATOR0
#define    L2SW_METER_OVERRATE_INDICATOR_REG(meter)            (L2SW_METER_OVERRATE_INDICATOR_BASE + (meter >> 4))
#define    L2SW_METER_EXCEED_OFFSET(meter)                    (meter & 0xF)
#define    L2SW_METER_EXCEED_MASK(meter)                    (1 << L2SW_METER_EXCEED_OFFSET(meter))

#define    L2SW_METER_IFG_CTRL_BASE                            L2SW_REG_METER_IFG_CTRL0
#define    L2SW_METER_IFG_CTRL_REG(meter)                    (L2SW_METER_IFG_CTRL_BASE + (meter >> 4))
#define    L2SW_METER_IFG_OFFSET(meter)                        (meter & 0xF)
#define    L2SW_METER_IFG_MASK(meter)                        (1 << L2SW_METER_IFG_OFFSET(meter))

#define    L2SW_FLOWCTRL_CTRL_REG                            L2SW_REG_FLOWCTRL_CTRL0

/* (16'h1800)8051_RLDP_EEE_reg */
#define    L2SW_EEELLDP_CTRL0_REG                            L2SW_REG_EEELLDP_CTRL0

#define    L2SW_EEELLDP_CTRL1_REG                            L2SW_REG_EEELLDP_CTRL1

#define    L2SW_EEELLDP_PMSK_REG                            L2SW_REG_EEELLDP_PMSK

#define    L2SW_EEELLDP_TX_FRAMEU_REG_BASE                    L2SW_REG_EEELLDP_FRAMEU00

#define    L2SW_EEELLDP_TX_CAP_FRAMEL_REG_BASE                L2SW_REG_EEELLDP_CAP_FRAMEL00

#define    L2SW_EEELLDP_RX_VALUE_PORT_BASE                    L2SW_REG_EEELLDP_RX_VALUE_P00_00
#define    L2SW_EEELLDP_RX_VALUE_PORT_REG(port)                (L2SW_EEELLDP_RX_VALUE_PORT_BASE + ((port) * 9))

#define    L2SW_RLDP_CTRL0_REG                                L2SW_REG_RLDP_CTRL0
#define    L2SW_RLDP_MODE_OFFSET    14

#define    L2SW_RLDP_RETRY_COUNT_REG                        L2SW_REG_RLDP_CTRL1

#define    L2SW_RLDP_RETRY_PERIOD_LOOPSTATE_REG                L2SW_REG_RLDP_CTRL2

#define    L2SW_RLDP_RETRY_PERIOD_CHKSTATE_REG                L2SW_REG_RLDP_CTRL3

#define    L2SW_RLDP_TX_PMSK_REG                            L2SW_REG_RLDP_CTRL4

#define    L2SW_RLDP_RAND_NUM_REG_BASE                        L2SW_REG_RLDP_RAND_NUM0

#define    L2SW_RLDP_MAGIC_NUM_REG_BASE                        L2SW_REG_RLDP_MAGIC_NUM0

#define    L2SW_RLDP_LOOP_PMSK_REG                            L2SW_REG_RLDP_LOOPSTATUS_INDICATOR

#define    L2SW_RLDP_LOOP_PORT_BASE                            L2SW_REG_RLDP_LOOP_PORT_REG0
#define    L2SW_RLDP_LOOP_PORT_REG(port)                    (L2SW_RLDP_LOOP_PORT_BASE + ((port) >> 1))
#define    L2SW_RLDP_LOOP_PORT_OFFSET(port)                    (((port) & 0x1) << 3)
#define    L2SW_RLDP_LOOP_PORT_MASK(port)                    (L2SW_RLDP_LOOP_PORT_00_MASK << L2SW_RLDP_LOOP_PORT_OFFSET(port))

#define    L2SW_PAGEMETER_PORT_BASE                            L2SW_REG_PAGEMETER_PORT0_CTRL0
#define    L2SW_PAGEMETER_PORT_REG(port)                    (L2SW_PAGEMETER_PORT_BASE + 0x20*(port))

#define    L2SW_HIGHPRI_INDICATOR_REG                        L2SW_REG_HIGHPRI_INDICATOR
#define    L2SW_PORT_INDICATOR_OFFSET(port)                    (port)
#define    L2SW_PORT_INDICATOR_MASK(port)                    (L2SW_PORT0_INDICATOR_MASK << L2SW_PORT_INDICATOR_OFFSET(port))

#define    L2SW_HIGHPRI_CFG_REG                                L2SW_REG_HIGHPRI_CFG

#define    L2SW_EAV_PRIORITY_REMAPPING_BASE                    L2SW_REG_EAV_CTRL1
#define    L2SW_EAV_PRIORITY_REMAPPING_REG(pri)                (L2SW_EAV_PRIORITY_REMAPPING_BASE + (pri >> 2))
#define    L2SW_EAV_PRIORITY_REMAPPING_OFFSET(pri)            ((pri & 0x3) * L2SW_REMAP_EAV_PRI1_REGEN_OFFSET)
#define    L2SW_EAV_PRIORITY_REMAPPING_MASK(pri)            (L2SW_REMAP_EAV_PRI0_REGEN_MASK << L2SW_EAV_PRIORITY_REMAPPING_OFFSET(pri))

#define    L2SW_EEEP_CFG_BASE                                L2SW_REG_PORT0_EEECFG
#define    L2SW_EEEP_CFG_REG(port)                            (L2SW_EEEP_CFG_BASE + (port*0x20))

#define    L2SW_PKG_CFG_BASE                                L2SW_REG_PKTGEN_PORT0_CTRL
#define    L2SW_PKG_CFG_REG(port)                            (L2SW_PKG_CFG_BASE + (port*0x20))

#define    L2SW_PKG_DA_BASE                                    L2SW_REG_PKTGEN_PORT0_DA0
#define    L2SW_PKG_DA_REG(port)                            (L2SW_PKG_DA_BASE + (port*0x20))

#define    L2SW_PKG_SA_BASE                                    L2SW_REG_PKTGEN_PORT0_SA0
#define    L2SW_PKG_SA_REG(port)                            (L2SW_PKG_SA_BASE + (port*0x20))

#define    L2SW_PKG_NUM_BASE                                L2SW_REG_PKTGEN_PORT0_COUNTER0
#define    L2SW_PKG_NUM_REG(port)                            (L2SW_PKG_NUM_BASE + (port*0x20))

#define    L2SW_PKG_LENGTH_BASE                                L2SW_REG_PKTGEN_PORT0_TX_LENGTH
#define    L2SW_PKG_LENGTH_REG(port)                        (L2SW_PKG_LENGTH_BASE + (port*0x20))

/* (16'h1c00)IGMP_MLD_reg */
#define    L2SW_IGMP_GROUP_USAGE_BASE                       L2SW_REG_IGMP_GROUP_USAGE_LIST0
#define    L2SW_IGMP_GROUP_USAGE_REG(idx)                   (L2SW_IGMP_GROUP_USAGE_BASE + (idx / 16))

#define    L2SW_FALLBACK_BASE                               L2SW_REG_FALLBACK_PORT0_CFG0
#define    L2SW_FALLBACK_PORT_CFG_REG(port)                 (L2SW_FALLBACK_BASE + (port * 4))
#define    L2SW_FALLBACK_PORT_MON_CNT_REG(port)             (L2SW_FALLBACK_BASE + 1 + (port * 4))
#define    L2SW_FALLBACK_PORT_ERR_CNT_REG(port)             (L2SW_FALLBACK_BASE + 3 + (port * 4))


/* (16'h6400)timer_1588 */
#define    L2SW_EAV_CFG_BASE                                              L2SW_REG_P0_EAV_CFG
#define    L2SW_EAV_PORT_CFG_REG(port)                              (L2SW_EAV_CFG_BASE + (port *0x10))
#define    L2SW_EAV_CFG_PTP_PHY_EN_EN_OFFSET                 L2SW_P0_EAV_CFG_PTP_PHY_EN_EN_OFFSET
#define    L2SW_EAV_CFG_RX_PDELAY_RESP_OFFSET                L2SW_P0_EAV_CFG_RX_PDELAY_RESP_OFFSET
#define    L2SW_EAV_CFG_RX_PDELAY_REQ_OFFSET                 L2SW_P0_EAV_CFG_RX_PDELAY_REQ_OFFSET
#define    L2SW_EAV_CFG_RX_DELAY_REQ_OFFSET                   L2SW_P0_EAV_CFG_RX_DELAY_REQ_OFFSET
#define    L2SW_EAV_CFG_RX_SYNC_OFFSET                            L2SW_P0_EAV_CFG_RX_SYNC_OFFSET
#define    L2SW_EAV_CFG_TX_PDELAY_RESP_OFFSET                L2SW_P0_EAV_CFG_TX_PDELAY_RESP_OFFSET
#define    L2SW_EAV_CFG_TX_PDELAY_REQ_OFFSET                 L2SW_P0_EAV_CFG_TX_PDELAY_REQ_OFFSET
#define    L2SW_EAV_CFG_TX_DELAY_REQ_OFFSET                   L2SW_P0_EAV_CFG_TX_DELAY_REQ_OFFSET
#define    L2SW_EAV_CFG_TX_SYNC_OFFSET                            L2SW_P0_EAV_CFG_TX_SYNC_OFFSET

#define    L2SW_REG_TX_SYNC_SEQ_ID_BASE                       L2SW_REG_P0_TX_SYNC_SEQ_ID
#define    L2SW_REG_TX_SYNC_SEQ_ID(port)                        (L2SW_REG_TX_SYNC_SEQ_ID_BASE + (port *0x10))
#define    L2SW_REG_SEQ_ID(port, type)                              (L2SW_REG_TX_SYNC_SEQ_ID_BASE + type + (port *0x10))

#define    L2SW_REG_TX_DELAY_REQ_SEQ_ID_BASE              L2SW_REG_P0_TX_DELAY_REQ_SEQ_ID
#define    L2SW_REG_TX_PDELAY_REQ_SEQ_ID_BASE          L2SW_REG_P0_TX_PDELAY_REQ_SEQ_ID
#define    L2SW_REG_TX_PDELAY_RESP_SEQ_ID_BASE        L2SW_REG_P0_TX_PDELAY_RESP_SEQ_ID
#define    L2SW_REG_RX_SYNC_SEQ_ID_BASE                        L2SW_REG_P0_RX_SYNC_SEQ_ID
#define    L2SW_REG_RX_DELAY_REQ_SEQ_ID_BASE            L2SW_REG_P0_RX_DELAY_REQ_SEQ_ID
#define    L2SW_REG_RX_PDELAY_REQ_SEQ_ID_BASE        L2SW_REG_P0_RX_PDELAY_REQ_SEQ_ID
#define    L2SW_REG_RX_PDELAY_RESP_SEQ_ID_BASE        L2SW_REG_P0_RX_PDELAY_RESP_SEQ_ID

#define    L2SW_REG_PORT_NSEC_L_BASE                            L2SW_REG_P0_PORT_NSEC_15_0
#define    L2SW_REG_PORT_NSEC_L(port)                            (L2SW_REG_PORT_NSEC_L_BASE + ((port) *0x10))
#define    L2SW_REG_PORT_NSEC_H_BASE                            L2SW_REG_P0_PORT_NSEC_26_16
#define    L2SW_REG_PORT_NSEC_H(port)                            (L2SW_REG_PORT_NSEC_H_BASE + ((port) *0x10))
#define    L2SW_PORT_NSEC_H_OFFSET                                L2SW_P0_PORT_NSEC_26_16_OFFSET
#define    L2SW_PORT_NSEC_H_MASK                                   L2SW_P0_PORT_NSEC_26_16_MASK

#define    L2SW_REG_PORT_SEC_L_BASE                                L2SW_REG_P0_PORT_SEC_15_0
#define    L2SW_REG_PORT_SEC_L(port)                            (L2SW_REG_PORT_SEC_L_BASE + ((port) *0x10))
#define    L2SW_REG_PORT_SEC_H_BASE                            L2SW_REG_P0_PORT_SEC_31_16
#define    L2SW_REG_PORT_SEC_H(port)                            (L2SW_REG_PORT_SEC_H_BASE + ((port) *0x10))

#endif /*#ifndef _L2SW_BASE_H_*/



