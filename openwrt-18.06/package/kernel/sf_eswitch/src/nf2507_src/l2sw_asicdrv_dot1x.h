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

#ifndef _L2SW_ASICDRV_DOT1X_H_
#define _L2SW_ASICDRV_DOT1X_H_

#include "l2sw_asicdrv.h"

enum DOT1X_UNAUTH_BEHAV
{
    DOT1X_UNAUTH_DROP = 0,
    DOT1X_UNAUTH_TRAP,
    DOT1X_UNAUTH_GVLAN,
    DOT1X_UNAUTH_END
};

extern ret_t l2sw_setAsic1xPBEnConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled);
extern ret_t l2sw_getAsic1xPBEnConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnabled);
extern ret_t l2sw_setAsic1xPBAuthConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 auth);
extern ret_t l2sw_getAsic1xPBAuthConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pAuth);
extern ret_t l2sw_setAsic1xPBOpdirConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 opdir);
extern ret_t l2sw_getAsic1xPBOpdirConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pOpdir);
extern ret_t l2sw_setAsic1xMBEnConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled);
extern ret_t l2sw_getAsic1xMBEnConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnabled);
extern ret_t l2sw_setAsic1xMBOpdirConfig(l2sw_uint8 units, l2sw_uint32 opdir);
extern ret_t l2sw_getAsic1xMBOpdirConfig(l2sw_uint8 units, l2sw_uint32 *pOpdir);
extern ret_t l2sw_setAsic1xProcConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 proc);
extern ret_t l2sw_getAsic1xProcConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pProc);
extern ret_t l2sw_setAsic1xGuestVidx(l2sw_uint8 units, l2sw_uint32 index);
extern ret_t l2sw_getAsic1xGuestVidx(l2sw_uint8 units, l2sw_uint32 *pIndex);
extern ret_t l2sw_setAsic1xGVOpdir(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsic1xGVOpdir(l2sw_uint8 units, l2sw_uint32 *pEnabled);
extern ret_t l2sw_setAsic1xTrapPriority(l2sw_uint8 units, l2sw_uint32 priority);
extern ret_t l2sw_getAsic1xTrapPriority(l2sw_uint8 units, l2sw_uint32 *pPriority);


#endif /*_L2SW_ASICDRV_DOT1X_H_*/

