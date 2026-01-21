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

#ifndef _L2SW_ASICDRV_OAM_H_
#define _L2SW_ASICDRV_OAM_H_

#include "l2sw_asicdrv.h"

enum OAMPARACT
{
    OAM_PARFWD = 0,
    OAM_PARLB,
    OAM_PARDISCARD,
    OAM_PARFWDCPU
};

enum OAMMULACT
{
    OAM_MULFWD = 0,
    OAM_MULDISCARD,
    OAM_MULCPU
};

extern ret_t l2sw_setAsicOamParser(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 parser);
extern ret_t l2sw_getAsicOamParser(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pParser);
extern ret_t l2sw_setAsicOamMultiplexer(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 multiplexer);
extern ret_t l2sw_getAsicOamMultiplexer(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pMultiplexer);
extern ret_t l2sw_setAsicOamCpuPri(l2sw_uint8 units, l2sw_uint32 priority);
extern ret_t l2sw_getAsicOamCpuPri(l2sw_uint8 units, l2sw_uint32 *pPriority);
extern ret_t l2sw_setAsicOamEnable(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicOamEnable(l2sw_uint8 units, l2sw_uint32 *pEnabled);
#endif /*_L2SW_ASICDRV_OAM_H_*/

