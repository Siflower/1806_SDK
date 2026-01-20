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

#ifndef _L2SW_ASICDRV_RLDP_H_
#define _L2SW_ASICDRV_RLDP_H_

#include "l2sw_asicdrv.h"

#include "sal_string.h"

extern ret_t l2sw_setAsicRldp(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicRldp(l2sw_uint8 units, l2sw_uint32 *pEnabled);
extern ret_t l2sw_setAsicRldpEnable8051(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicRldpEnable8051(l2sw_uint8 units, l2sw_uint32 *pEnabled);
extern ret_t l2sw_setAsicRldpCompareRandomNumber(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicRldpCompareRandomNumber(l2sw_uint8 units, l2sw_uint32 *pEnabled);
extern ret_t l2sw_setAsicRldpIndicatorSource(l2sw_uint8 units, l2sw_uint32 src);
extern ret_t l2sw_getAsicRldpIndicatorSource(l2sw_uint8 units, l2sw_uint32 *pSrc);
extern ret_t l2sw_setAsicRldpCheckingStatePara(l2sw_uint8 units, l2sw_uint32 retryCount, l2sw_uint32 retryPeriod);
extern ret_t l2sw_getAsicRldpCheckingStatePara(l2sw_uint8 units, l2sw_uint32 *pRetryCount, l2sw_uint32 *pRetryPeriod);
extern ret_t l2sw_setAsicRldpLoopStatePara(l2sw_uint8 units, l2sw_uint32 retryCount, l2sw_uint32 retryPeriod);
extern ret_t l2sw_getAsicRldpLoopStatePara(l2sw_uint8 units, l2sw_uint32 *pRetryCount, l2sw_uint32 *pRetryPeriod);
extern ret_t l2sw_setAsicRldpTxPortmask(l2sw_uint8 units, l2sw_uint32 portmask);
extern ret_t l2sw_getAsicRldpTxPortmask(l2sw_uint8 units, l2sw_uint32 *pPortmask);
extern ret_t l2sw_setAsicRldpMagicNum(l2sw_uint8 units, ether_addr_t seed);
extern ret_t l2sw_getAsicRldpMagicNum(l2sw_uint8 units, ether_addr_t *pSeed);
extern ret_t l2sw_getAsicRldpLoopedPortmask(l2sw_uint8 units, l2sw_uint32 *pPortmask);
extern ret_t l2sw_setAsicRldp8051Portmask(l2sw_uint8 units, l2sw_uint32 portmask);
extern ret_t l2sw_getAsicRldp8051Portmask(l2sw_uint8 units, l2sw_uint32 *pPortmask);


extern ret_t l2sw_getAsicRldpRandomNumber(l2sw_uint8 units, ether_addr_t *pRandNumber);
extern ret_t l2sw_getAsicRldpLoopedPortPair(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pLoopedPair);
extern ret_t l2sw_setAsicRlppTrap8051(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicRlppTrap8051(l2sw_uint8 units, l2sw_uint32 *pEnabled);

extern ret_t l2sw_setAsicRldpLeaveLoopedPortmask(l2sw_uint8 units, l2sw_uint32 portmask);
extern ret_t l2sw_getAsicRldpLeaveLoopedPortmask(l2sw_uint8 units, l2sw_uint32 *pPortmask);

extern ret_t l2sw_setAsicRldpEnterLoopedPortmask(l2sw_uint8 units, l2sw_uint32 portmask);
extern ret_t l2sw_getAsicRldpEnterLoopedPortmask(l2sw_uint8 units, l2sw_uint32 *pPortmask);

extern ret_t l2sw_setAsicRldpTriggerMode(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicRldpTriggerMode(l2sw_uint8 units, l2sw_uint32 *pEnabled);

#endif /*_L2SW_ASICDRV_RLDP_H_*/

