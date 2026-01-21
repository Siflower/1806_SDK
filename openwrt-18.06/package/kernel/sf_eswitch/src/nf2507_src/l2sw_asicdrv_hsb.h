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

#ifndef _L2SW_ASICDRV__HSB_H_
#define _L2SW_ASICDRV__HSB_H_

#include "l2sw_asicdrv.h"

#define L2SW_FIELDSEL_FORMAT_NUMBER      (16)
#define L2SW_FIELDSEL_MAX_OFFSET         (255)

enum FIELDSEL_FORMAT_FORMAT
{
    FIELDSEL_FORMAT_DEFAULT = 0,
    FIELDSEL_FORMAT_RAW,
    FIELDSEL_FORMAT_LLC,
    FIELDSEL_FORMAT_IPV4,
    FIELDSEL_FORMAT_ARP,
    FIELDSEL_FORMAT_IPV6,
    FIELDSEL_FORMAT_IPPAYLOAD,
    FIELDSEL_FORMAT_L4PAYLOAD,
    FIELDSEL_FORMAT_END
};

extern ret_t l2sw_setAsicFieldSelector(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 format, l2sw_uint32 offset);
extern ret_t l2sw_getAsicFieldSelector(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pFormat, l2sw_uint32* pOffset);

#endif /*_L2SW_ASICDRV__HSB_H_*/

