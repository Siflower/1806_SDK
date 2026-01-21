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

#ifndef _L2SW_ASICDRV_INTERRUPT_H_
#define _L2SW_ASICDRV_INTERRUPT_H_

#include "l2sw_asicdrv.h"

typedef enum L2SW_INTR_IMRS_E
{
    IMRS_LINK_CHANGE,
    IMRS_METER_EXCEED,
    IMRS_L2_LEARN,
    IMRS_SPEED_CHANGE,
    IMRS_SPECIAL_CONGESTION,
    IMRS_GREEN_FEATURE,
    IMRS_LOOP_DETECTION,
    IMRS_8051,
    IMRS_CABLE_DIAG,
    IMRS_ACL,
    IMRS_RESERVED, /* Unused */
    IMRS_SLIENT,
    IMRS_END,
}L2SW_INTR_IMRS;

typedef enum L2SW_INTR_INDICATOR_E
{
    INTRST_L2_LEARN = 0,
    INTRST_SPEED_CHANGE,
    INTRST_SPECIAL_CONGESTION,
    INTRST_PORT_LINKDOWN,
    INTRST_PORT_LINKUP,
    INTRST_METER0_15,
    INTRST_METER16_31,
    INTRST_RLDP_LOOPED,
    INTRST_RLDP_RELEASED,
    INTRST_SYS_LEARN,
    INTRST_END,
}L2SW_INTR_INDICATOR;

extern ret_t l2sw_setAsicInterruptPolarity(l2sw_uint8 units, l2sw_uint32 polarity);
extern ret_t l2sw_getAsicInterruptPolarity(l2sw_uint8 units, l2sw_uint32* pPolarity);
extern ret_t l2sw_setAsicInterruptMask(l2sw_uint8 units, l2sw_uint32 imr);
extern ret_t l2sw_getAsicInterruptMask(l2sw_uint8 units, l2sw_uint32* pImr);
extern ret_t l2sw_setAsicInterruptStatus(l2sw_uint8 units, l2sw_uint32 ims);
extern ret_t l2sw_getAsicInterruptStatus(l2sw_uint8 units, l2sw_uint32* pIms);
extern ret_t l2sw_setAsicInterruptRelatedStatus(l2sw_uint8 units, l2sw_uint32 type, l2sw_uint32 status);
extern ret_t l2sw_getAsicInterruptRelatedStatus(l2sw_uint8 units, l2sw_uint32 type, l2sw_uint32* pStatus);


#endif /*#ifndef _L2SW_ASICDRV_INTERRUPT_H_*/

