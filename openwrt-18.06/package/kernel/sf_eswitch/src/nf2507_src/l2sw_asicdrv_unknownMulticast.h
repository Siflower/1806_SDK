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

#ifndef _L2SW_ASICDRV_UNKNOWNMULTICAST_H_
#define _L2SW_ASICDRV_UNKNOWNMULTICAST_H_

#include "l2sw_asicdrv.h"

enum L2_UNKOWN_MULTICAST_BEHAVE
{
    L2_UNKOWN_MULTICAST_FLOODING = 0,
    L2_UNKOWN_MULTICAST_DROP,
    L2_UNKOWN_MULTICAST_TRAP,
    L2_UNKOWN_MULTICAST_DROP_EXCLUDE_RMA,
    L2_UNKOWN_MULTICAST_END
};

enum L3_UNKOWN_MULTICAST_BEHAVE
{
    L3_UNKOWN_MULTICAST_FLOODING = 0,
    L3_UNKOWN_MULTICAST_DROP,
    L3_UNKOWN_MULTICAST_TRAP,
    L3_UNKOWN_MULTICAST_ROUTER,
    L3_UNKOWN_MULTICAST_END
};

enum MULTICASTTYPE{
    MULTICAST_TYPE_IPV4 = 0,
    MULTICAST_TYPE_IPV6,
    MULTICAST_TYPE_L2,
    MULTICAST_TYPE_END
};

extern ret_t l2sw_setAsicUnknownL2MulticastBehavior(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 behave);
extern ret_t l2sw_getAsicUnknownL2MulticastBehavior(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pBehave);
extern ret_t l2sw_setAsicUnknownIPv4MulticastBehavior(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 behave);
extern ret_t l2sw_getAsicUnknownIPv4MulticastBehavior(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pBehave);
extern ret_t l2sw_setAsicUnknownIPv6MulticastBehavior(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 behave);
extern ret_t l2sw_getAsicUnknownIPv6MulticastBehavior(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pBehave);
extern ret_t l2sw_setAsicUnknownMulticastTrapPriority(l2sw_uint8 units, l2sw_uint32 priority);
extern ret_t l2sw_getAsicUnknownMulticastTrapPriority(l2sw_uint8 units, l2sw_uint32 *pPriority);

#endif /*_L2SW_ASICDRV_UNKNOWNMULTICAST_H_*/


