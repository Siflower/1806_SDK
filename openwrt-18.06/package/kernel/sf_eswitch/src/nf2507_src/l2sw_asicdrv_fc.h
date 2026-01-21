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

#ifndef _L2SW_ASICDRV_FC_H_
#define _L2SW_ASICDRV_FC_H_

#include "l2sw_asicdrv.h"

#define L2SW_PAGE_NUMBER    0x800


enum FLOW_CONTROL_TYPE
{
    FC_EGRESS = 0,
    FC_INGRESS,
};

enum FC_JUMBO_SIZE
{
    FC_JUMBO_SIZE_3K = 0,
    FC_JUMBO_SIZE_4K,
    FC_JUMBO_SIZE_6K,
    FC_JUMBO_SIZE_9K,
    FC_JUMBO_SIZE_END,

};


extern ret_t l2sw_setAsicFlowControlSelect(l2sw_uint8 units, l2sw_uint32 select);
extern ret_t l2sw_getAsicFlowControlSelect(l2sw_uint8 units, l2sw_uint32 *pSelect);
extern ret_t l2sw_setAsicFlowControlJumboMode(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicFlowControlJumboMode(l2sw_uint8 units, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicFlowControlJumboModeSize(l2sw_uint8 units, l2sw_uint32 size);
extern ret_t l2sw_getAsicFlowControlJumboModeSize(l2sw_uint8 units, l2sw_uint32* pSize);
extern ret_t l2sw_setAsicFlowControlQueueEgressEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicFlowControlQueueEgressEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicFlowControlDropAll(l2sw_uint8 units, l2sw_uint32 dropall);
extern ret_t l2sw_getAsicFlowControlDropAll(l2sw_uint8 units, l2sw_uint32* pDropall);
extern ret_t l2sw_setAsicFlowControlPauseAllThreshold(l2sw_uint8 units, l2sw_uint32 threshold);
extern ret_t l2sw_getAsicFlowControlPauseAllThreshold(l2sw_uint8 units, l2sw_uint32 *pThreshold);
extern ret_t l2sw_setAsicFlowControlSystemThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);
extern ret_t l2sw_getAsicFlowControlSystemThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);
extern ret_t l2sw_setAsicFlowControlSharedThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);
extern ret_t l2sw_getAsicFlowControlSharedThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);
extern ret_t l2sw_setAsicFlowControlPortThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);
extern ret_t l2sw_getAsicFlowControlPortThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);
extern ret_t l2sw_setAsicFlowControlPortPrivateThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);
extern ret_t l2sw_getAsicFlowControlPortPrivateThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);
extern ret_t l2sw_setAsicFlowControlSystemDropThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);
extern ret_t l2sw_getAsicFlowControlSystemDropThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);
extern ret_t l2sw_setAsicFlowControlSharedDropThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);
extern ret_t l2sw_getAsicFlowControlSharedDropThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);
extern ret_t l2sw_setAsicFlowControlPortDropThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);
extern ret_t l2sw_getAsicFlowControlPortDropThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);
extern ret_t l2sw_setAsicFlowControlPortPrivateDropThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);
extern ret_t l2sw_getAsicFlowControlPortPrivateDropThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);
extern ret_t l2sw_setAsicFlowControlSystemJumboThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);
extern ret_t l2sw_getAsicFlowControlSystemJumboThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);
extern ret_t l2sw_setAsicFlowControlSharedJumboThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);
extern ret_t l2sw_getAsicFlowControlSharedJumboThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);
extern ret_t l2sw_setAsicFlowControlPortJumboThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);
extern ret_t l2sw_getAsicFlowControlPortJumboThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);
extern ret_t l2sw_setAsicFlowControlPortPrivateJumboThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);
extern ret_t l2sw_getAsicFlowControlPortPrivateJumboThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);

extern ret_t l2sw_setAsicEgressFlowControlPortDropGap(l2sw_uint8 units, l2sw_uint32 gap);
extern ret_t l2sw_getAsicEgressFlowControlPortDropGap(l2sw_uint8 units, l2sw_uint32 *pGap);
extern ret_t l2sw_setAsicEgressFlowControlQueueDropGap(l2sw_uint8 units, l2sw_uint32 gap);
extern ret_t l2sw_getAsicEgressFlowControlQueueDropGap(l2sw_uint8 units, l2sw_uint32 *pGap);
extern ret_t l2sw_setAsicEgressFlowControlPortDropThreshold(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 threshold);
extern ret_t l2sw_getAsicEgressFlowControlPortDropThreshold(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pThreshold);
extern ret_t l2sw_setAsicEgressFlowControlQueueDropThreshold(l2sw_uint8 units, l2sw_uint32 qid, l2sw_uint32 threshold);
extern ret_t l2sw_getAsicEgressFlowControlQueueDropThreshold(l2sw_uint8 units, l2sw_uint32 qid, l2sw_uint32 *pThreshold);
extern ret_t l2sw_getAsicEgressQueueEmptyPortMask(l2sw_uint8 units, l2sw_uint32 *pPortmask);
extern ret_t l2sw_getAsicTotalPage(l2sw_uint8 units, l2sw_uint32 *pPageCount);
extern ret_t l2sw_getAsicPulbicPage(l2sw_uint8 units, l2sw_uint32 *pPageCount);
extern ret_t l2sw_getAsicMaxTotalPage(l2sw_uint8 units, l2sw_uint32 *pPageCount);
extern ret_t l2sw_getAsicMaxPulbicPage(l2sw_uint8 units, l2sw_uint32 *pPageCount);
extern ret_t l2sw_getAsicPortPage(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pPageCount);
extern ret_t l2sw_getAsicPortPageMax(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pPageCount);
extern ret_t l2sw_setAsicFlowControlEgressPortIndep(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enable);
extern ret_t l2sw_getAsicFlowControlEgressPortIndep(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnable);

#endif /*_L2SW_ASICDRV_FC_H_*/

