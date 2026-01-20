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

#ifndef _L2SW_ASICDRV_SCHEDULING_H_
#define _L2SW_ASICDRV_SCHEDULING_H_

#include "l2sw_asicdrv.h"

#define L2SW_QWEIGHTMAX    0x7F
#define L2SW_PORT_QUEUE_METER_INDEX_MAX    7

/* enum for queue type */
enum QUEUETYPE
{
    QTYPE_STRICT = 0,
    QTYPE_WFQ,
};
extern ret_t l2sw_setAsicLeakyBucketParameter(l2sw_uint8 units, l2sw_uint32 tick, l2sw_uint32 token);
extern ret_t l2sw_getAsicLeakyBucketParameter(l2sw_uint8 units, l2sw_uint32 *tick, l2sw_uint32 *token);
extern ret_t l2sw_setAsicAprMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 apridx);
extern ret_t l2sw_getAsicAprMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 *apridx);
extern ret_t l2sw_setAsicPprMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 ppridx);
extern ret_t l2sw_getAsicPprMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 *ppridx);
extern ret_t l2sw_setAsicAprEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 aprEnable);
extern ret_t l2sw_getAsicAprEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *aprEnable);
extern ret_t l2sw_setAsicPprEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 pprEnable);
extern ret_t l2sw_getAsicPprEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pprEnable);

extern ret_t l2sw_setAsicWFQWeight(l2sw_uint8 units, l2sw_uint32, l2sw_uint32 queueid, l2sw_uint32 weight );
extern ret_t l2sw_getAsicWFQWeight(l2sw_uint8 units, l2sw_uint32, l2sw_uint32 queueid, l2sw_uint32 *weight );
extern ret_t l2sw_setAsicWFQBurstSize(l2sw_uint8 units, l2sw_uint32 burstsize);
extern ret_t l2sw_getAsicWFQBurstSize(l2sw_uint8 units, l2sw_uint32 *burstsize);

extern ret_t l2sw_setAsicQueueType(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 queueType);
extern ret_t l2sw_getAsicQueueType(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 *queueType);
extern ret_t l2sw_setAsicQueueRate(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 ppridx, l2sw_uint32 apridx );
extern ret_t l2sw_getAsicQueueRate(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32* ppridx, l2sw_uint32* apridx );
extern ret_t l2sw_setAsicPortEgressRate(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 rate);
extern ret_t l2sw_getAsicPortEgressRate(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *rate);
extern ret_t l2sw_setAsicPortEgressRateIfg(l2sw_uint8 units, l2sw_uint32 ifg);
extern ret_t l2sw_getAsicPortEgressRateIfg(l2sw_uint8 units, l2sw_uint32 *ifg);

#endif /*_L2SW_ASICDRV_SCHEDULING_H_*/

