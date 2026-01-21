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

#ifndef _L2SW_ASICDRV_PHY_H_
#define _L2SW_ASICDRV_PHY_H_

#include "l2sw_asicdrv.h"

#define L2SW_PHY_REGNOMAX           0x1F
#define L2SW_PHY_EXTERNALMAX        0x7

#define L2SW_PHY_BASE               0x2000
#define L2SW_PHY_EXT_BASE           0xA000

#define L2SW_PHY_OFFSET             5
#define L2SW_PHY_EXT_OFFSET         9

#define L2SW_PHY_PAGE_ADDRESS       31


#define L2SW_PHY_AUTONEG_ADV_REG            0x04
#define L2SW_PHY_FLOW_CONTROL_EN           (1 << 10)
#define L2SW_PHY_RESET_EN                  (1 << 15)

extern ret_t l2sw_setAsicPHYReg(l2sw_uint8 units, l2sw_uint32 phyNo, l2sw_uint32 phyAddr, l2sw_uint32 regData );
extern ret_t l2sw_getAsicPHYReg(l2sw_uint8 units, l2sw_uint32 phyNo, l2sw_uint32 phyAddr, l2sw_uint32* pRegData );
extern ret_t l2sw_setAsicPHYOCPReg(l2sw_uint8 units, l2sw_uint32 phyNo, l2sw_uint32 ocpAddr, l2sw_uint32 ocpData );
extern ret_t l2sw_getAsicPHYOCPReg(l2sw_uint8 units, l2sw_uint32 phyNo, l2sw_uint32 ocpAddr, l2sw_uint32 *pRegData );

#if !defined(USE_L2SW_SDK)

#else
extern ret_t l2sw_setAsicSdsReg(l2sw_uint8 units, l2sw_uint32 sdsId, l2sw_uint32 sdsReg, l2sw_uint32 sdsPage,  l2sw_uint32 value);
extern ret_t l2sw_getAsicSdsReg(l2sw_uint8 units, l2sw_uint32 sdsId, l2sw_uint32 sdsReg, l2sw_uint32 sdsPage, l2sw_uint32 *value);
extern ret_t l2sw_setAsicPHYSram(l2sw_uint8 units, l2sw_uint32 phyNo, l2sw_uint32 sramAddr, l2sw_uint32 sramData );
#endif

int sf_l2sw_setAsicPHYReg(unsigned int phyNo, unsigned int phyAddr, unsigned int pRegData);
int sf_l2sw_getAsicPHYReg(unsigned int phyNo, unsigned int phyAddr, unsigned int *pRegData);
#endif /*#ifndef _L2SW_ASICDRV_PHY_H_*/
