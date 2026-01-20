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

#ifndef _L2SW_ASICDRV_QOS_H_
#define _L2SW_ASICDRV_QOS_H_

#include "l2sw_asicdrv.h"

#define L2SW_DECISIONPRIMAX    0xFF

/* enum Priority Selection Types */
enum PRIDECISION
{
    PRIDEC_PORT = 0,
    PRIDEC_ACL,
    PRIDEC_DSCP,
    PRIDEC_1Q,
    PRIDEC_1AD,
    PRIDEC_CVLAN,
    PRIDEC_DA,
    PRIDEC_SA,
    PRIDEC_END,
};

/* enum Priority Selection Index */
enum L2SW_PRIDEC_TABLE
{
    PRIDEC_IDX0 = 0,
    PRIDEC_IDX1,
    PRIDEC_IDX_END,
};

enum L2SW_DOT1P_PRISEL
{
    DOT1P_PRISEL_USER =  0,
    DOT1P_PRISEL_TAG,
    DOT1P_PRISEL_END
};

enum L2SW_DSCP_PRISEL
{
    DSCP_PRISEL_INTERNAL =  0,
    DSCP_PRISEL_DSCP,
    DSCP_PRISEL_USER ,
    DSCP_PRISEL_END
};


extern ret_t l2sw_setAsicRemarkingDot1pAbility(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicRemarkingDot1pAbility(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicRemarkingDot1pParameter(l2sw_uint8 units, l2sw_uint32 priority, l2sw_uint32 newPriority );
extern ret_t l2sw_getAsicRemarkingDot1pParameter(l2sw_uint8 units, l2sw_uint32 priority, l2sw_uint32 *pNewPriority );
extern  ret_t l2sw_setAsicRemarkingDot1pSrc(l2sw_uint8 units, l2sw_uint32 type);
extern  ret_t l2sw_getAsicRemarkingDot1pSrc(l2sw_uint8 units, l2sw_uint32 *pType);
extern ret_t l2sw_setAsicRemarkingDscpAbility(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicRemarkingDscpAbility(l2sw_uint8 units, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicRemarkingDscpParameter(l2sw_uint8 units, l2sw_uint32 priority, l2sw_uint32 newDscp );
extern ret_t l2sw_getAsicRemarkingDscpParameter(l2sw_uint8 units, l2sw_uint32 priority, l2sw_uint32* pNewDscp );

extern ret_t l2sw_setAsicPriorityDot1qRemapping(l2sw_uint8 units, l2sw_uint32 srcpriority, l2sw_uint32 priority );
extern ret_t l2sw_getAsicPriorityDot1qRemapping(l2sw_uint8 units, l2sw_uint32 srcpriority, l2sw_uint32 *pPriority );
extern ret_t l2sw_setAsicPriorityDscpBased(l2sw_uint8 units, l2sw_uint32 dscp, l2sw_uint32 priority );
extern ret_t l2sw_getAsicPriorityDscpBased(l2sw_uint8 units, l2sw_uint32 dscp, l2sw_uint32 *pPriority );
extern ret_t l2sw_setAsicPriorityPortBased(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 priority );
extern ret_t l2sw_getAsicPriorityPortBased(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pPriority );
extern ret_t l2sw_setAsicPriorityDecision(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 prisrc, l2sw_uint32 decisionPri);
extern ret_t l2sw_getAsicPriorityDecision(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 prisrc, l2sw_uint32* pDecisionPri);
extern ret_t l2sw_setAsicPriorityToQIDMappingTable(l2sw_uint8 units, l2sw_uint32 qnum, l2sw_uint32 priority, l2sw_uint32 qid );
extern ret_t l2sw_getAsicPriorityToQIDMappingTable(l2sw_uint8 units, l2sw_uint32 qnum, l2sw_uint32 priority, l2sw_uint32* pQid);
extern ret_t l2sw_setAsicOutputQueueMappingIndex(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qnum );
extern ret_t l2sw_getAsicOutputQueueMappingIndex(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pQnum );

extern ret_t l2sw_setAsicRemarkingDscpSrc(l2sw_uint8 units, l2sw_uint32 type);
extern ret_t l2sw_getAsicRemarkingDscpSrc(l2sw_uint8 units, l2sw_uint32 *pType);
extern ret_t l2sw_setAsicRemarkingDscp2Dscp(l2sw_uint8 units, l2sw_uint32 dscp, l2sw_uint32 rmkDscp);
extern ret_t l2sw_getAsicRemarkingDscp2Dscp(l2sw_uint8 units, l2sw_uint32 dscp, l2sw_uint32 *pRmkDscp);

extern ret_t l2sw_setAsicPortPriorityDecisionIndex(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 index );
extern ret_t l2sw_getAsicPortPriorityDecisionIndex(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pIndex );

#endif /*#ifndef _L2SW_ASICDRV_QOS_H_*/

