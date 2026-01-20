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

#ifndef _L2SW_ASICDRV_MISC_H_
#define _L2SW_ASICDRV_MISC_H_

#include "l2sw_asicdrv.h"
 
extern ret_t l2sw_setAsicMacAddress(l2sw_uint8 units, ether_addr_t mac);
extern ret_t l2sw_getAsicMacAddress(l2sw_uint8 units, ether_addr_t *pMac);
extern ret_t l2sw_getAsicDebugInfo(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pDebugifo);
extern ret_t l2sw_setAsicPortJamMode(l2sw_uint8 units, l2sw_uint32 mode);
extern ret_t l2sw_getAsicPortJamMode(l2sw_uint8 units, l2sw_uint32* pMode);

#if !defined(USE_L2SW_SDK)

#else
extern ret_t l2sw_setAsicMaxLengthCfg(l2sw_uint8 units, l2sw_uint32 cfgId, l2sw_uint32 maxLength);
extern ret_t l2sw_getAsicMaxLengthCfg(l2sw_uint8 units, l2sw_uint32 cfgId, l2sw_uint32 *pMaxLength);
extern ret_t l2sw_setAsicMaxLength(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 type, l2sw_uint32 cfgId);
extern ret_t l2sw_getAsicMaxLength(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 type, l2sw_uint32 *pCfgId);
#endif

#endif /*_L2SW_ASICDRV_MISC_H_*/

