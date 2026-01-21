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

#ifndef _L2SW_ASICDRV_IGMP_H_
#define _L2SW_ASICDRV_IGMP_H_

/****************************************************************/
/* Header File inclusion                                        */
/****************************************************************/
#include "l2sw_asicdrv.h"

#define L2SW_MAX_LEAVE_TIMER        (7)
#define L2SW_MAX_QUERY_INT          (0xFFFF)
#define L2SW_MAX_ROB_VAR            (7)

#define L2SW_IGMP_GOUP_NO           (256)
#define L2SW_IGMP_MAX_GOUP          (0xFF)
#define L2SW_IGMP_GRP_BLEN          (3)
#define L2SW_ROUTER_PORT_INVALID    (0xF)

enum L2SW_IGMPTABLE_FULL_OP
{
    TABLE_FULL_FORWARD = 0,
    TABLE_FULL_DROP,
    TABLE_FULL_TRAP,
    TABLE_FULL_OP_END
};

enum L2SW_CRC_ERR_OP
{
    CRC_ERR_DROP = 0,
    CRC_ERR_TRAP,
    CRC_ERR_FORWARD,
    CRC_ERR_NORMAL,
    CRC_ERR_OP_END
};

enum L2SW_IGMP_MLD_PROTOCOL_OP
{
    PROTOCOL_OP_ASIC = 0,
    PROTOCOL_OP_FLOOD,
    PROTOCOL_OP_TRAP,
    PROTOCOL_OP_DROP,
    PROTOCOL_OP_END
};

enum L2SW_IGMP_MLD_BYPASS_GROUP
{
    BYPASS_224_0_0_X = 0,
    BYPASS_224_0_1_X,
    BYPASS_239_255_255_X,
    BYPASS_IPV6_00XX,
    BYPASS_GROUP_END
};

typedef struct
{
    l2sw_uint32 p0_timer;
    l2sw_uint32 p1_timer;
    l2sw_uint32 p2_timer;
    l2sw_uint32 p3_timer;
    l2sw_uint32 p4_timer;
    l2sw_uint32 p5_timer;
    l2sw_uint32 p6_timer;
    l2sw_uint32 p7_timer;
    l2sw_uint32 p8_timer;
    l2sw_uint32 p9_timer;
    l2sw_uint32 p10_timer;
    l2sw_uint32 report_supp_flag;

}l2sw_igmpgroup;


ret_t l2sw_setAsicIgmp(l2sw_uint8 units, l2sw_uint32 enabled);
ret_t l2sw_getAsicIgmp(l2sw_uint8 units, l2sw_uint32 *pEnabled);
ret_t l2sw_setAsicIpMulticastVlanLeaky(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled );
ret_t l2sw_getAsicIpMulticastVlanLeaky(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnabled );
ret_t l2sw_setAsicIGMPTableFullOP(l2sw_uint8 units, l2sw_uint32 operation);
ret_t l2sw_getAsicIGMPTableFullOP(l2sw_uint8 units, l2sw_uint32 *pOperation);
ret_t l2sw_setAsicIGMPCRCErrOP(l2sw_uint8 units, l2sw_uint32 operation);
ret_t l2sw_getAsicIGMPCRCErrOP(l2sw_uint8 units, l2sw_uint32 *pOperation);
ret_t l2sw_setAsicIGMPFastLeaveEn(l2sw_uint8 units, l2sw_uint32 enabled);
ret_t l2sw_getAsicIGMPFastLeaveEn(l2sw_uint8 units, l2sw_uint32 *pEnabled);
ret_t l2sw_setAsicIGMPQueryInterval(l2sw_uint8 units, l2sw_uint32 interval);
ret_t l2sw_getAsicIGMPQueryInterval(l2sw_uint8 units, l2sw_uint32 *pInterval);
ret_t l2sw_setAsicIGMPDynamicRouterPortQueryInterval(l2sw_uint8 units, l2sw_uint32 interval);
ret_t l2sw_getAsicIGMPDynamicRouterPortQueryInterval(l2sw_uint8 units, l2sw_uint32 *pinterval);
ret_t l2sw_setAsicIGMPRobVar(l2sw_uint8 units, l2sw_uint32 rob_var);
ret_t l2sw_getAsicIGMPRobVar(l2sw_uint8 units, l2sw_uint32 *pRob_var);
ret_t l2sw_setAsicIGMPStaticRouterPort(l2sw_uint8 units, l2sw_uint32 pmsk);
ret_t l2sw_getAsicIGMPStaticRouterPort(l2sw_uint8 units, l2sw_uint32 *pMsk);
ret_t l2sw_setAsicIGMPAllowDynamicRouterPort(l2sw_uint8 units, l2sw_uint32 pmsk);
ret_t l2sw_getAsicIGMPAllowDynamicRouterPort(l2sw_uint8 units, l2sw_uint32 *pPmsk);
ret_t l2sw_getAsicIGMPdynamicRouterPort1(l2sw_uint8 units, l2sw_uint32 *pPort, l2sw_uint32 *pTimer);
ret_t l2sw_getAsicIGMPdynamicRouterPort2(l2sw_uint8 units, l2sw_uint32 *pPort, l2sw_uint32 *pTimer);
ret_t l2sw_setAsicIGMPSuppression(l2sw_uint8 units, l2sw_uint32 report_supp_enabled, l2sw_uint32 leave_supp_enabled);
ret_t l2sw_getAsicIGMPSuppression(l2sw_uint8 units, l2sw_uint32 *pReport_supp_enabled, l2sw_uint32 *pLeave_supp_enabled);
ret_t l2sw_setAsicIGMPQueryRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 allow_query);
ret_t l2sw_getAsicIGMPQueryRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pAllow_query);
ret_t l2sw_setAsicIGMPReportRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 allow_report);
ret_t l2sw_getAsicIGMPReportRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pAllow_report);
ret_t l2sw_setAsicIGMPLeaveRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 allow_leave);
ret_t l2sw_getAsicIGMPLeaveRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pAllow_leave);
ret_t l2sw_setAsicIGMPMRPRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 allow_mrp);
ret_t l2sw_getAsicIGMPMRPRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pAllow_mrp);
ret_t l2sw_setAsicIGMPMcDataRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 allow_mcdata);
ret_t l2sw_getAsicIGMPMcDataRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pAllow_mcdata);
ret_t l2sw_setAsicIGMPv1Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 igmpv1_op);
ret_t l2sw_getAsicIGMPv1Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pIgmpv1_op);
ret_t l2sw_setAsicIGMPv2Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 igmpv2_op);
ret_t l2sw_getAsicIGMPv2Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pIgmpv2_op);
ret_t l2sw_setAsicIGMPv3Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 igmpv3_op);
ret_t l2sw_getAsicIGMPv3Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pIgmpv3_op);
ret_t l2sw_setAsicMLDv1Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 mldv1_op);
ret_t l2sw_getAsicMLDv1Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pMldv1_op);
ret_t l2sw_setAsicMLDv2Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 mldv2_op);
ret_t l2sw_getAsicMLDv2Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pMldv2_op);
ret_t l2sw_setAsicIGMPPortMAXGroup(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 max_group);
ret_t l2sw_getAsicIGMPPortMAXGroup(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pMax_group);
ret_t l2sw_getAsicIGMPPortCurrentGroup(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pCurrent_group);
ret_t l2sw_getAsicIGMPGroup(l2sw_uint8 units, l2sw_uint32 idx, l2sw_uint32 *pValid, l2sw_igmpgroup *pGrp);
ret_t l2sw_setAsicIpMulticastPortIsoLeaky(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled);
ret_t l2sw_getAsicIpMulticastPortIsoLeaky(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnabled);
ret_t l2sw_setAsicIGMPReportLeaveFlood(l2sw_uint8 units, l2sw_uint32 flood);
ret_t l2sw_getAsicIGMPReportLeaveFlood(l2sw_uint8 units, l2sw_uint32 *pFlood);
ret_t l2sw_setAsicIGMPDropLeaveZero(l2sw_uint8 units, l2sw_uint32 drop);
ret_t l2sw_getAsicIGMPDropLeaveZero(l2sw_uint8 units, l2sw_uint32 *pDrop);
ret_t l2sw_setAsicIGMPBypassStormCTRL(l2sw_uint8 units, l2sw_uint32 bypass);
ret_t l2sw_getAsicIGMPBypassStormCTRL(l2sw_uint8 units, l2sw_uint32 *pBypass);
ret_t l2sw_setAsicIGMPIsoLeaky(l2sw_uint8 units, l2sw_uint32 leaky);
ret_t l2sw_getAsicIGMPIsoLeaky(l2sw_uint8 units, l2sw_uint32 *pLeaky);
ret_t l2sw_setAsicIGMPVLANLeaky(l2sw_uint8 units, l2sw_uint32 leaky);
ret_t l2sw_getAsicIGMPVLANLeaky(l2sw_uint8 units, l2sw_uint32 *pLeaky);
ret_t l2sw_setAsicIGMPBypassGroup(l2sw_uint8 units, l2sw_uint32 bypassType, l2sw_uint32 enabled);
ret_t l2sw_getAsicIGMPBypassGroup(l2sw_uint8 units, l2sw_uint32 bypassType, l2sw_uint32 *pEnabled);

#endif /*#ifndef _L2SW_ASICDRV_IGMP_H_*/

