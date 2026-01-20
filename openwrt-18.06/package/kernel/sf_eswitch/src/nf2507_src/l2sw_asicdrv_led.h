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

#ifndef _L2SW_ASICDRV_LED_H_
#define _L2SW_ASICDRV_LED_H_

#include "l2sw_asicdrv.h"

#define L2SW_LEDGROUPNO                 3
#define L2SW_LEDGROUPMASK               0x7
#define L2SW_LED_FORCE_MODE_BASE        L2SW_REG_CPU_FORCE_LED0_CFG0
#define L2SW_LED_FORCE_CTRL             L2SW_REG_CPU_FORCE_LED_CFG

enum L2SW_LEDOP{

    LEDOP_SCAN0=0,
    LEDOP_SCAN1,
    LEDOP_PARALLEL,
    LEDOP_SERIAL,
    LEDOP_END,
};

enum L2SW_LEDSERACT{

    LEDSERACT_HIGH=0,
    LEDSERACT_LOW,
    LEDSERACT_MAX,
};

enum L2SW_LEDSER{

    LEDSER_16G=0,
    LEDSER_8G,
    LEDSER_MAX,
};

enum L2SW_LEDCOL2SW{

    LEDCOL2SW_LEDOFF=0,
    LEDCOL2SW_DUPCOL,
    LEDCOL2SW_LINK_ACT,
    LEDCOL2SW_SPD1000,
    LEDCOL2SW_SPD100,
    LEDCOL2SW_SPD10,
    LEDCOL2SW_SPD1000ACT,
    LEDCOL2SW_SPD100ACT,
    LEDCOL2SW_SPD10ACT,
    LEDCOL2SW_SPD10010ACT,
    LEDCOL2SW_LOOPDETECT,
    LEDCOL2SW_EEE,
    LEDCOL2SW_LINKRX,
    LEDCOL2SW_LINKTX,
    LEDCOL2SW_MASTER,
    LEDCOL2SW_ACT,
    LEDCOL2SW_END
};

enum L2SW_LEDBLINKRATE{

    LEDBLINKRATE_32MS=0,
    LEDBLINKRATE_64MS,
    LEDBLINKRATE_128MS,
    LEDBLINKRATE_256MS,
    LEDBLINKRATE_512MS,
    LEDBLINKRATE_1024MS,
    LEDBLINKRATE_48MS,
    LEDBLINKRATE_96MS,
    LEDBLINKRATE_END,
};

enum L2SW_LEDFORCEMODE{

    LEDFORCEMODE_NORMAL=0,
    LEDFORCEMODE_BLINK,
    LEDFORCEMODE_OFF,
    LEDFORCEMODE_ON,
    LEDFORCEMODE_END,
};

enum L2SW_LEDFORCERATE{

    LEDFORCERATE_512MS=0,
    LEDFORCERATE_1024MS,
    LEDFORCERATE_2048MS,
    LEDFORCERATE_NORMAL,
    LEDFORCERATE_END,

};

enum L2SW_LEDMODE
{
    L2SW_LED_MODE_0 = 0,
    L2SW_LED_MODE_1,
    L2SW_LED_MODE_2,
    L2SW_LED_MODE_3,
    L2SW_LED_MODE_END
};

extern ret_t l2sw_setAsicLedIndicateInfoConfig(l2sw_uint8 units, l2sw_uint32 ledno, l2sw_uint32 config);
extern ret_t l2sw_getAsicLedIndicateInfoConfig(l2sw_uint8 units, l2sw_uint32 ledno, l2sw_uint32* pConfig);
extern ret_t l2sw_setAsicForceGroupLed(l2sw_uint8 units, l2sw_uint32 groupmask, l2sw_uint32 mode);
extern ret_t l2sw_getAsicForceGroupLed(l2sw_uint8 units, l2sw_uint32* groupmask, l2sw_uint32* pMode);
extern ret_t l2sw_setAsicLedBlinkRate(l2sw_uint8 units, l2sw_uint32 blinkRate);
extern ret_t l2sw_getAsicLedBlinkRate(l2sw_uint8 units, l2sw_uint32* pBlinkRate);
extern ret_t l2sw_setAsicLedForceBlinkRate(l2sw_uint8 units, l2sw_uint32 blinkRate);
extern ret_t l2sw_getAsicLedForceBlinkRate(l2sw_uint8 units, l2sw_uint32* pBlinkRate);
extern ret_t l2sw_setAsicLedGroupEnable(l2sw_uint8 units, l2sw_uint32 group, l2sw_uint32 portmask);
extern ret_t l2sw_getAsicLedGroupEnable(l2sw_uint8 units, l2sw_uint32 group, l2sw_uint32 *portmask);
extern ret_t l2sw_setAsicLedOutputEnable(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicLedOutputEnable(l2sw_uint8 units, l2sw_uint32 *ptr_enabled);


#endif /*#ifndef _L2SW_ASICDRV_LED_H_*/

