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

#ifndef _L2SW_ASICDRV_RMA_H_
#define _L2SW_ASICDRV_RMA_H_

#include "l2sw_asicdrv.h"

#define L2SW_RMAMAX                     0x2F

enum L2SW_RMAOP
{
    RMAOP_FORWARD = 0,
    RMAOP_TRAP_TO_CPU,
    RMAOP_DROP,
    RMAOP_FORWARD_EXCLUDE_CPU,
    RMAOP_END
};


typedef struct  l2sw_rma_s{

    l2sw_uint16 operation;
    l2sw_uint16 discard_storm_filter;
    l2sw_uint16 trap_priority;
    l2sw_uint16 keep_format;
    l2sw_uint16 vlan_leaky;
    l2sw_uint16 portiso_leaky;

}l2sw_rma_t;


extern ret_t l2sw_setAsicRma(l2sw_uint8 units, l2sw_uint32 index, l2sw_rma_t* pRmacfg);
extern ret_t l2sw_getAsicRma(l2sw_uint8 units, l2sw_uint32 index, l2sw_rma_t* pRmacfg);
extern ret_t l2sw_setAsicRmaCdp(l2sw_uint8 units, l2sw_rma_t* pRmacfg);
extern ret_t l2sw_getAsicRmaCdp(l2sw_uint8 units, l2sw_rma_t* pRmacfg);
extern ret_t l2sw_setAsicRmaCsstp(l2sw_uint8 units, l2sw_rma_t* pRmacfg);
extern ret_t l2sw_getAsicRmaCsstp(l2sw_uint8 units, l2sw_rma_t* pRmacfg);
extern ret_t l2sw_setAsicRmaLldp(l2sw_uint8 units, l2sw_uint32 enabled, l2sw_rma_t* pRmacfg);
extern ret_t l2sw_getAsicRmaLldp(l2sw_uint8 units, l2sw_uint32 *pEnabled, l2sw_rma_t* pRmacfg);

#endif /*#ifndef _L2SW_ASICDRV_RMA_H_*/

