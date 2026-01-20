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

#ifndef _L2SW_ASICDRV_PORTISOLATION_H_
#define _L2SW_ASICDRV_PORTISOLATION_H_

#include "l2sw_asicdrv.h"

extern ret_t l2sw_setAsicPortIsolationPermittedPortmask(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 permitPortmask);
extern ret_t l2sw_getAsicPortIsolationPermittedPortmask(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pPermitPortmask);
extern ret_t l2sw_setAsicPortIsolationEfid(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 efid);
extern ret_t l2sw_getAsicPortIsolationEfid(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEfid);

#endif /*_L2SW_ASICDRV_PORTISOLATION_H_*/
