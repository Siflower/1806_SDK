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

#ifndef _L2SW_ASICDRV_METER_H_
#define _L2SW_ASICDRV_METER_H_

#include "l2sw_asicdrv.h"


extern ret_t l2sw_setAsicShareMeter(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 rate, l2sw_uint32 ifg);
extern ret_t l2sw_getAsicShareMeter(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 *pRate, l2sw_uint32 *pIfg);
extern ret_t l2sw_setAsicShareMeterBucketSize(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 lbThreshold);
extern ret_t l2sw_getAsicShareMeterBucketSize(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 *pLbThreshold);
extern ret_t l2sw_setAsicShareMeterType(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 type);
extern ret_t l2sw_getAsicShareMeterType(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 *pType);
extern ret_t l2sw_setAsicMeterExceedStatus(l2sw_uint8 units, l2sw_uint32 index);
extern ret_t l2sw_getAsicMeterExceedStatus(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pStatus);

#endif /*_L2SW_ASICDRV_FC_H_*/

