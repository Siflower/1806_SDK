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

#ifndef _L2SW_ASICDRV_MIRROR_H_
#define _L2SW_ASICDRV_MIRROR_H_

#include "l2sw_asicdrv.h"

extern ret_t l2sw_setAsicPortMirror(l2sw_uint8 units, l2sw_uint32 source, l2sw_uint32 monitor);
extern ret_t l2sw_getAsicPortMirror(l2sw_uint8 units, l2sw_uint32 *pSource, l2sw_uint32 *pMonitor);
extern ret_t l2sw_setAsicPortMirrorRxFunction(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicPortMirrorRxFunction(l2sw_uint8 units, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicPortMirrorTxFunction(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicPortMirrorTxFunction(l2sw_uint8 units, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicPortMirrorPriority(l2sw_uint8 units, l2sw_uint32 priority);
extern ret_t l2sw_getAsicPortMirrorPriority(l2sw_uint8 units, l2sw_uint32* pPriority);
extern ret_t l2sw_setAsicPortMirrorMask(l2sw_uint8 units, l2sw_uint32 SourcePortmask);
extern ret_t l2sw_getAsicPortMirrorMask(l2sw_uint8 units, l2sw_uint32 *pSourcePortmask);
extern ret_t l2sw_setAsicPortMirrorIsolationRxLeaky(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicPortMirrorIsolationRxLeaky(l2sw_uint8 units, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicPortMirrorIsolationTxLeaky(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicPortMirrorIsolationTxLeaky(l2sw_uint8 units, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicPortMirrorRxEnable(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicPortMirrorRxEnable(l2sw_uint8 units, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicPortMirrorTxEnable(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicPortMirrorTxEnable(l2sw_uint8 units, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicPortMirrorOamLeaky(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicPortMirrorOamLeaky(l2sw_uint8 units, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicPortMirrorIncludeAclPortList(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicPortMirrorIncludeAclPortList(l2sw_uint8 units, l2sw_uint32* pEnabled);

#endif /*#ifndef _L2SW_ASICDRV_MIRROR_H_*/

