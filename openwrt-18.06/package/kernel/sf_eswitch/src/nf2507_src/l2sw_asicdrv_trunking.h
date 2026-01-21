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

#ifndef _L2SW_ASICDRV_TRUNKING_H_
#define _L2SW_ASICDRV_TRUNKING_H_

#include "l2sw_asicdrv.h"

#define L2SW_MAX_TRUNK_GID              (2)
#define L2SW_TRUNKING_PORTNO            (4)
#define L2SW_TRUNKING1_PORTN0           (2)
#define L2SW_TRUNKING_HASHVALUE_MAX     (15)

extern ret_t l2sw_setAsicTrunkingGroup(l2sw_uint8 units, l2sw_uint32 group, l2sw_uint32 portmask);
extern ret_t l2sw_getAsicTrunkingGroup(l2sw_uint8 units, l2sw_uint32 group, l2sw_uint32* pPortmask);
extern ret_t l2sw_setAsicTrunkingFlood(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicTrunkingFlood(l2sw_uint8 units, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicTrunkingHashSelect(l2sw_uint8 units, l2sw_uint32 hashsel);
extern ret_t l2sw_getAsicTrunkingHashSelect(l2sw_uint8 units, l2sw_uint32* pHashsel);

extern ret_t l2sw_getAsicQeueuEmptyStatus(l2sw_uint8 units, l2sw_uint32* pPortmask);

extern ret_t l2sw_setAsicTrunkingMode(l2sw_uint8 units, l2sw_uint32 mode);
extern ret_t l2sw_getAsicTrunkingMode(l2sw_uint8 units, l2sw_uint32* pMode);
extern ret_t l2sw_setAsicTrunkingFc(l2sw_uint8 units, l2sw_uint32 group, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicTrunkingFc(l2sw_uint8 units, l2sw_uint32 group, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicTrunkingHashTable(l2sw_uint8 units, l2sw_uint32 hashval, l2sw_uint32 portId);
extern ret_t l2sw_getAsicTrunkingHashTable(l2sw_uint8 units, l2sw_uint32 hashval, l2sw_uint32* pPortId);
extern ret_t l2sw_setAsicTrunkingHashTable1(l2sw_uint8 units, l2sw_uint32 hashval, l2sw_uint32 portId);
extern ret_t l2sw_getAsicTrunkingHashTable1(l2sw_uint8 units, l2sw_uint32 hashval, l2sw_uint32* pPortId);

#endif /*_L2SW_ASICDRV_TRUNKING_H_*/

