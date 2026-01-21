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

#ifndef _L2SW_ASICDRV_CPUTAG_H_
#define _L2SW_ASICDRV_CPUTAG_H_

#include "l2sw_asicdrv.h"

enum CPUTAG_INSERT_MODE
{
    CPUTAG_INSERT_TO_ALL = 0,
    CPUTAG_INSERT_TO_TRAPPING,
    CPUTAG_INSERT_TO_NO,
    CPUTAG_INSERT_END
};

extern ret_t l2sw_setAsicCputagEnable(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicCputagEnable(l2sw_uint8 units, l2sw_uint32 *pEnabled);
extern ret_t l2sw_setAsicCputagTrapPort(l2sw_uint8 units, l2sw_uint32 port);
extern ret_t l2sw_getAsicCputagTrapPort(l2sw_uint8 units, l2sw_uint32 *pPort);
extern ret_t l2sw_setAsicCputagPortmask(l2sw_uint8 units, l2sw_uint32 portmask);
extern ret_t l2sw_getAsicCputagPortmask(l2sw_uint8 units, l2sw_uint32 *pPmsk);
extern ret_t l2sw_setAsicCputagInsertMode(l2sw_uint8 units, l2sw_uint32 mode);
extern ret_t l2sw_getAsicCputagInsertMode(l2sw_uint8 units, l2sw_uint32 *pMode);
extern ret_t l2sw_setAsicCputagPriorityRemapping(l2sw_uint8 units, l2sw_uint32 srcPri, l2sw_uint32 newPri);
extern ret_t l2sw_getAsicCputagPriorityRemapping(l2sw_uint8 units, l2sw_uint32 srcPri, l2sw_uint32 *pNewPri);
extern ret_t l2sw_setAsicCputagPosition(l2sw_uint8 units, l2sw_uint32 postion);
extern ret_t l2sw_getAsicCputagPosition(l2sw_uint8 units, l2sw_uint32* pPostion);
extern ret_t l2sw_setAsicCputagMode(l2sw_uint8 units, l2sw_uint32 mode);
extern ret_t l2sw_getAsicCputagMode(l2sw_uint8 units, l2sw_uint32 *pMode);
extern ret_t l2sw_setAsicCputagRxMinLength(l2sw_uint8 units, l2sw_uint32 mode);
extern ret_t l2sw_getAsicCputagRxMinLength(l2sw_uint8 units, l2sw_uint32 *pMode);

#endif /*#ifndef _L2SW_ASICDRV_CPUTAG_H_*/

