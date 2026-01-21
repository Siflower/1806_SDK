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

#ifndef _L2SW_ASICDRV_INBWCTRL_H_
#define _L2SW_ASICDRV_INBWCTRL_H_

#include "l2sw_asicdrv.h"

extern ret_t l2sw_setAsicPortIngressBandwidth(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 bandwidth, l2sw_uint32 preifg, l2sw_uint32 enableFC);
extern ret_t l2sw_getAsicPortIngressBandwidth(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pBandwidth, l2sw_uint32* pPreifg, l2sw_uint32* pEnableFC );
extern ret_t l2sw_setAsicPortIngressBandwidthBypass(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicPortIngressBandwidthBypass(l2sw_uint8 units, l2sw_uint32* pEnabled);


#endif /*_L2SW_ASICDRV_INBWCTRL_H_*/

