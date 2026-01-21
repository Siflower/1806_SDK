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

#include "l2sw_asicdrv_led.h"
/* Function Name:
 *      l2sw_setAsicLedIndicateInfoConfig
 * Description:
 *      Set Leds indicated information mode
 * Input:
 *      ledno   - LED group number. There are 1 to 1 led mapping to each port in each led group
 *      config  - Support 16 types configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      The API can set LED indicated information configuration for each LED group with 1 to 1 led mapping to each port.
 *      Definition        LED Statuses            Description
 *      0000        LED_Off                LED pin Tri-State.
 *      0001        Dup/Col                Collision, Full duplex Indicator. Blinking every 43ms when collision happens. Low for full duplex, and high for half duplex mode.
 *      0010        Link/Act               Link, Activity Indicator. Low for link established. Link/Act Blinks every 43ms when the corresponding port is transmitting or receiving.
 *      0011        Spd1000                1000Mb/s Speed Indicator. Low for 1000Mb/s.
 *      0100        Spd100                 100Mb/s Speed Indicator. Low for 100Mb/s.
 *      0101        Spd10                  10Mb/s Speed Indicator. Low for 10Mb/s.
 *      0110        Spd1000/Act            1000Mb/s Speed/Activity Indicator. Low for 1000Mb/s. Blinks every 43ms when the corresponding port is transmitting or receiving.
 *      0111        Spd100/Act             100Mb/s Speed/Activity Indicator. Low for 100Mb/s. Blinks every 43ms when the corresponding port is transmitting or receiving.
 *      1000        Spd10/Act              10Mb/s Speed/Activity Indicator. Low for 10Mb/s. Blinks every 43ms when the corresponding port is transmitting or receiving.
 *      1001        Spd100 (10)/Act        10/100Mb/s Speed/Activity Indicator. Low for 10/100Mb/s. Blinks every 43ms when the corresponding port is transmitting or receiving.
 *      1010        Fiber                  Fiber link Indicator. Low for Fiber.
 *      1011        Fault                  Auto-negotiation     Fault Indicator. Low for Fault.
 *      1100        Link/Rx                Link, Activity Indicator. Low for link established. Link/Rx Blinks every 43ms when the corresponding port is transmitting.
 *      1101        Link/Tx                Link, Activity Indicator. Low for link established. Link/Tx Blinks every 43ms when the corresponding port is receiving.
 *      1110        Master                 Link on Master Indicator. Low for link Master established.
 *      1111        LED_Force              Force LED output, LED output value reference
 */
ret_t l2sw_setAsicLedIndicateInfoConfig(l2sw_uint8 units, l2sw_uint32 ledno, l2sw_uint32 config)
{
    ret_t   retVal;
    CONST l2sw_uint16 bits[L2SW_LEDGROUPNO] = {L2SW_LED0_CFG_MASK, L2SW_LED1_CFG_MASK, L2SW_LED2_CFG_MASK};
    
    if(ledno >= L2SW_LEDGROUPNO)
        return L2SW_ERR_OUT_OF_RANGE;
    
    if(config >= LEDCOL2SW_END)
        return L2SW_ERR_OUT_OF_RANGE;
    
    retVal = l2sw_setAsicRegBit(units, L2SW_REG_LED_CONFIGURATION, L2SW_LED_CONFIG_SEL_OFFSET, 0);
    if(retVal != L2SW_ERR_OK)
        return retVal;
    
    return l2sw_setAsicRegBits(units, L2SW_REG_LED_CONFIGURATION, bits[ledno], config);
}
/* Function Name:
 *      l2sw_getAsicLedIndicateInfoConfig
 * Description:
 *      Get Leds indicated information mode
 * Input:
 *      ledno   - LED group number. There are 1 to 1 led mapping to each port in each led group
 *      pConfig     - Support 16 types configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_getAsicLedIndicateInfoConfig(l2sw_uint8 units, l2sw_uint32 ledno, l2sw_uint32* pConfig)
{
    CONST l2sw_uint16 bits[L2SW_LEDGROUPNO]= {L2SW_LED0_CFG_MASK, L2SW_LED1_CFG_MASK, L2SW_LED2_CFG_MASK};

    if(ledno >= L2SW_LEDGROUPNO)
        return L2SW_ERR_OUT_OF_RANGE;

    /* Get register value */
    return l2sw_getAsicRegBits(units, L2SW_REG_LED_CONFIGURATION, bits[ledno], pConfig);
}

/* Function Name:
 *      l2sw_setAsicForceGroupLed
 * Description:
 *      Turn on/off Led of all ports
 * Input:
 *      group   - LED group number
 *      mode    - 0b00:normal mode, 0b01:force blink, 0b10:force off, 0b11:force on
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicForceGroupLed(l2sw_uint8 units, l2sw_uint32 groupmask, l2sw_uint32 mode)
{
    ret_t retVal;
    l2sw_uint32 i,bitmask;
    CONST l2sw_uint16 bits[3]= {0x0004,0x0010,0x0040};

    /* Invalid input parameter */
    if(groupmask > L2SW_LEDGROUPMASK)
        return L2SW_ERR_OUT_OF_RANGE;

    if(mode >= LEDFORCEMODE_END)
        return L2SW_ERR_OUT_OF_RANGE;

    bitmask = 0;
    for(i = 0; i <  L2SW_LEDGROUPNO; i++)
    {
        if(groupmask & (1 << i))
        {
            bitmask = bitmask | bits[i];
        }

    }

    retVal = l2sw_setAsicRegBits(units, L2SW_LED_FORCE_CTRL, L2SW_LED_FORCE_MODE_MASK, bitmask);

    retVal = l2sw_setAsicRegBits(units, L2SW_LED_FORCE_CTRL, L2SW_FORCE_MODE_MASK, mode);

    if(LEDFORCEMODE_NORMAL == mode)
        retVal = l2sw_setAsicRegBits(units, L2SW_LED_FORCE_CTRL, L2SW_LED_FORCE_MODE_MASK, 0);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicForceGroupLed
 * Description:
 *      Turn on/off Led of all ports
 * Input:
 *      group   - LED group number
 *      pMode   - 0b00:normal mode, 0b01:force blink, 0b10:force off, 0b11:force on
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicForceGroupLed(l2sw_uint8 units, l2sw_uint32* groupmask, l2sw_uint32* pMode)
{
    ret_t retVal;
    l2sw_uint32 i,regData;
    CONST l2sw_uint16 bits[3] = {0x0004,0x0010,0x0040};

    /* Get Related Registers */
    if((retVal = l2sw_getAsicRegBits(units, L2SW_LED_FORCE_CTRL, L2SW_LED_FORCE_MODE_MASK, &regData)) != L2SW_ERR_OK)
        return retVal;

    for(i = 0; i< L2SW_LEDGROUPNO; i++)
    {
        if((regData & bits[i]) == bits[i])
        {
            *groupmask = *groupmask | (1 << i);
        }
    }

    return l2sw_getAsicRegBits(units, L2SW_LED_FORCE_CTRL, L2SW_FORCE_MODE_MASK, pMode);
}
/* Function Name:
 *      l2sw_setAsicLedBlinkRate
 * Description:
 *      Set led blinking rate at mode 0 to mode 3
 * Input:
 *      blinkRate   - Support 6 blink rates
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      LED blink rate can be at 43ms, 84ms, 120ms, 170ms, 340ms and 670ms
 */
ret_t l2sw_setAsicLedBlinkRate(l2sw_uint8 units, l2sw_uint32 blinkRate)
{
    if(blinkRate >= LEDBLINKRATE_END)
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_setAsicRegBits(units, L2SW_REG_LED_MODE, L2SW_SEL_LEDRATE_MASK, blinkRate);
}
/* Function Name:
 *      l2sw_getAsicLedBlinkRate
 * Description:
 *      Get led blinking rate at mode 0 to mode 3
 * Input:
 *      pBlinkRate  - Support 6 blink rates
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicLedBlinkRate(l2sw_uint8 units, l2sw_uint32* pBlinkRate)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_LED_MODE, L2SW_SEL_LEDRATE_MASK, pBlinkRate);
}
/* Function Name:
 *      l2sw_setAsicLedForceBlinkRate
 * Description:
 *      Set LEd blinking rate for force mode led
 * Input:
 *      blinkRate   - Support 6 blink rates
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicLedForceBlinkRate(l2sw_uint8 units, l2sw_uint32 blinkRate)
{
    if(blinkRate >= LEDFORCERATE_END)
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_setAsicRegBits(units, L2SW_REG_LED_MODE, L2SW_FORCE_RATE_MASK, blinkRate);
}
/* Function Name:
 *      l2sw_getAsicLedForceBlinkRate
 * Description:
 *      Get LED blinking rate for force mode led
 * Input:
 *      pBlinkRate  - Support 6 blink rates
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicLedForceBlinkRate(l2sw_uint8 units, l2sw_uint32* pBlinkRate)
{
     return l2sw_getAsicRegBits(units, L2SW_REG_LED_MODE, L2SW_FORCE_RATE_MASK, pBlinkRate);
}

/*
@func ret_t | l2sw_setAsicLedGroupEnable | Turn on/off Led of all system ports
@parm l2sw_uint32 | group | LED group id.
@parm l2sw_uint32 | portmask | LED port mask.
@rvalue L2SW_ERR_OK | Success.
@rvalue L2SW_ERR_SMI | SMI access error.
@rvalue L2SW_ERR_PORT_ID | Invalid port number.
@rvalue L2SW_ERR_INPUT | Invalid input value.
@comm
    The API can turn on/off leds of dedicated port while indicated information configuration of LED group is set to force mode.
 */
ret_t l2sw_setAsicLedGroupEnable(l2sw_uint8 units, l2sw_uint32 group, l2sw_uint32 portmask)
{
    ret_t retVal;
    l2sw_uint32 regAddr;
    l2sw_uint32 regDataMask;

    if ( group >= L2SW_LEDGROUPNO )
        return L2SW_ERR_INPUT;

    regAddr = L2SW_REG_PARA_LED_IO_EN1 + group/2;
    regDataMask = 0xFF << ((group%2)*8);
    retVal = l2sw_setAsicRegBits(units, regAddr, regDataMask, portmask&0xff);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    regAddr = L2SW_REG_PARA_LED_IO_EN3;
    regDataMask = 0x3 << (group*2);
    retVal = l2sw_setAsicRegBits(units, regAddr, regDataMask, (portmask>>8)&0x7);
    if(retVal != L2SW_ERR_OK)
        return retVal;


    return L2SW_ERR_OK;
}

/*
@func ret_t | l2sw_getAsicLedGroupEnable | Get on/off status of Led of all system ports
@parm l2sw_uint32 | group | LED group id.
@parm l2sw_uint32 | *portmask | LED port mask.
@rvalue L2SW_ERR_OK | Success.
@rvalue L2SW_ERR_SMI | SMI access error.
@rvalue L2SW_ERR_PORT_ID | Invalid port number.
@rvalue L2SW_ERR_INPUT | Invalid input value.
@comm
    The API can turn on/off leds of dedicated port while indicated information configuration of LED group is set to force mode.
 */
ret_t l2sw_getAsicLedGroupEnable(l2sw_uint8 units, l2sw_uint32 group, l2sw_uint32 *portmask)
{
    ret_t retVal;
    l2sw_uint32 regAddr;
    l2sw_uint32 regDataMask,regData;

    if ( group >= L2SW_LEDGROUPNO )
        return L2SW_ERR_INPUT;

    regAddr = L2SW_REG_PARA_LED_IO_EN1 + group/2;
    regDataMask = 0xFF << ((group%2)*8);
    retVal = l2sw_getAsicRegBits(units, regAddr, regDataMask, portmask);
    if(retVal != L2SW_ERR_OK)
        return retVal;


    regAddr = L2SW_REG_PARA_LED_IO_EN3;
    regDataMask = 0x3 << (group*2);
    retVal = l2sw_getAsicRegBits(units, regAddr, regDataMask, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *portmask = (regData << 8) | *portmask;

    return L2SW_ERR_OK;
}

/*
@func ret_t | l2sw_setAsicLedOutputEnable | Set LED output enable
@parm l2sw_uint32 | enabled | enable or disalbe.
@rvalue L2SW_ERR_OK | Success.
@rvalue L2SW_ERR_SMI | SMI access error.
@rvalue L2SW_ERR_INPUT | Invalid input value.
@comm
    The API can turn on/off LED output Enable
 */
ret_t l2sw_setAsicLedOutputEnable(l2sw_uint8 units, l2sw_uint32 enabled)
{
    ret_t retVal;
    l2sw_uint32 regdata;

    if (enabled == 1)
        regdata = 0;
    else
        regdata = 1;

    /* Enable/Disable H/W IGMP/MLD */
    retVal = l2sw_setAsicRegBit(units, L2SW_REG_LED_SYS_CONFIG, L2SW_LED_IO_DISABLE_OFFSET, regdata);

    return retVal;
}


/*
@func ret_t | l2sw_getAsicLedOutputEnable | Get LED serial mode setup
@parm l2sw_uint32*| active | Active High or Low.
@rvalue L2SW_ERR_OK | Success.
@rvalue L2SW_ERR_SMI | SMI access error.
@rvalue L2SW_ERR_INPUT | Invalid input value.
@comm
    The API can get LED serial mode setup and get signal active high/low.
 */
ret_t l2sw_getAsicLedOutputEnable(l2sw_uint8 units, l2sw_uint32 *ptr_enabled)
{
    ret_t retVal;
    l2sw_uint32 regdata;

    retVal = l2sw_getAsicRegBit(units, L2SW_REG_LED_SYS_CONFIG, L2SW_LED_IO_DISABLE_OFFSET, &regdata);
    if (retVal != L2SW_ERR_OK)
        return retVal;

    if (regdata == 1)
        *ptr_enabled = 0;
    else
        *ptr_enabled = 1;

    return L2SW_ERR_OK;
}

