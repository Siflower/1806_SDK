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

#ifndef _L2SW_ASICDRV_EEE_H_
#define _L2SW_ASICDRV_EEE_H_

#include "l2sw_asicdrv.h"

#define EEE_OCP_PHY_ADDR    (0xA5D0)

extern ret_t l2sw_setAsicEee100M(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enable);
extern ret_t l2sw_getAsicEee100M(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *enable);
extern ret_t l2sw_setAsicEeeGiga(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enable);
extern ret_t l2sw_getAsicEeeGiga(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *enable);


#endif /*_L2SW_ASICDRV_EEE_H_*/
