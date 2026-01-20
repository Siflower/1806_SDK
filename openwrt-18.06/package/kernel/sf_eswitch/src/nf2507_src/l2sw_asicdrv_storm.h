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

#ifndef _L2SW_ASICDRV_STORM_H_
#define _L2SW_ASICDRV_STORM_H_

#include "l2sw_asicdrv.h"

extern ret_t l2sw_setAsicStormFilterBroadcastEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicStormFilterBroadcastEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnabled);
extern ret_t l2sw_setAsicStormFilterBroadcastMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 meter);
extern ret_t l2sw_getAsicStormFilterBroadcastMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pMeter);
extern ret_t l2sw_setAsicStormFilterMulticastEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicStormFilterMulticastEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnabled);
extern ret_t l2sw_setAsicStormFilterMulticastMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 meter);
extern ret_t l2sw_getAsicStormFilterMulticastMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pMeter);
extern ret_t l2sw_setAsicStormFilterUnknownMulticastEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicStormFilterUnknownMulticastEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnabled);
extern ret_t l2sw_setAsicStormFilterUnknownMulticastMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 meter);
extern ret_t l2sw_getAsicStormFilterUnknownMulticastMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pMeter);
extern ret_t l2sw_setAsicStormFilterUnknownUnicastEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicStormFilterUnknownUnicastEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnabled);
extern ret_t l2sw_setAsicStormFilterUnknownUnicastMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 meter);
extern ret_t l2sw_getAsicStormFilterUnknownUnicastMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pMeter);
extern ret_t l2sw_setAsicStormFilterExtBroadcastMeter(l2sw_uint8 units, l2sw_uint32 meter);
extern ret_t l2sw_getAsicStormFilterExtBroadcastMeter(l2sw_uint8 units, l2sw_uint32 *pMeter);
extern ret_t l2sw_setAsicStormFilterExtMulticastMeter(l2sw_uint8 units, l2sw_uint32 meter);
extern ret_t l2sw_getAsicStormFilterExtMulticastMeter(l2sw_uint8 units, l2sw_uint32 *pMeter);
extern ret_t l2sw_setAsicStormFilterExtUnknownMulticastMeter(l2sw_uint8 units, l2sw_uint32 meter);
extern ret_t l2sw_getAsicStormFilterExtUnknownMulticastMeter(l2sw_uint8 units, l2sw_uint32 *pMeter);
extern ret_t l2sw_setAsicStormFilterExtUnknownUnicastMeter(l2sw_uint8 units, l2sw_uint32 meter);
extern ret_t l2sw_getAsicStormFilterExtUnknownUnicastMeter(l2sw_uint8 units, l2sw_uint32 *pMeter);
extern ret_t l2sw_setAsicStormFilterExtBroadcastEnable(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicStormFilterExtBroadcastEnable(l2sw_uint8 units, l2sw_uint32 *pEnabled);
extern ret_t l2sw_setAsicStormFilterExtMulticastEnable(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicStormFilterExtMulticastEnable(l2sw_uint8 units, l2sw_uint32 *pEnabled);
extern ret_t l2sw_setAsicStormFilterExtUnknownMulticastEnable(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicStormFilterExtUnknownMulticastEnable(l2sw_uint8 units, l2sw_uint32 *pEnabled);
extern ret_t l2sw_setAsicStormFilterExtUnknownUnicastEnable(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicStormFilterExtUnknownUnicastEnable(l2sw_uint8 units, l2sw_uint32 *pEnabled);
extern ret_t l2sw_setAsicStormFilterExtEnablePortMask(l2sw_uint8 units, l2sw_uint32 portmask);
extern ret_t l2sw_getAsicStormFilterExtEnablePortMask(l2sw_uint8 units, l2sw_uint32 *pPortmask);
extern ret_t l2sw_setAsicStormFilterEnable(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicStormFilterEnable(l2sw_uint8 units, l2sw_uint32 *pEnabled);

#endif /*_L2SW_ASICDRV_STORM_H_*/


