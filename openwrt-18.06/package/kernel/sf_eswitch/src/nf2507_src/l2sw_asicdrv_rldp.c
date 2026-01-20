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

#include "l2sw_asicdrv_rldp.h"
/* Function Name:
 *      l2sw_setAsicRldp
 * Description:
 *      Set RLDP function enable/disable
 * Input:
 *      enabled     - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicRldp(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_RLDP_CTRL0, L2SW_RLDP_ENABLE_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicRldp
 * Description:
 *      Get RLDP function enable/disable
 * Input:
 *      pEnabled    - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicRldp(l2sw_uint8 units, l2sw_uint32 *pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_RLDP_CTRL0, L2SW_RLDP_ENABLE_OFFSET, pEnabled);
}
/* Function Name:
 *      l2sw_setAsicRldpEnable8051
 * Description:
 *      Set RLDP function handled by ASIC or 8051
 * Input:
 *      enabled     - 1: enabled 8051, 0: disabled 8051 (RLDP is handled by ASIC)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicRldpEnable8051(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_RLDP_CTRL0, L2SW_RLDP_8051_ENABLE_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_setAsicRldl2sw_getAsicRldpEnable8051pEnable8051
 * Description:
 *      Get RLDP function handled by ASIC or 8051
 * Input:
 *      pEnabled    - 1: enabled 8051, 0: disabled 8051 (RLDP is handled by ASIC)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicRldpEnable8051(l2sw_uint8 units, l2sw_uint32 *pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_RLDP_CTRL0, L2SW_RLDP_8051_ENABLE_OFFSET, pEnabled);
}
/* Function Name:
 *      l2sw_setAsicRldpCompareRandomNumber
 * Description:
 *      Set enable compare the random number field and seed field of RLDP frame
 * Input:
 *      enabled     - 1: enabled comparing random number, 0: disabled comparing random number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicRldpCompareRandomNumber(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_RLDP_CTRL0, L2SW_RLDP_COMP_ID_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicRldpCompareRandomNumber
 * Description:
 *      Get enable compare the random number field and seed field of RLDP frame
 * Input:
 *      pEnabled    - 1: enabled comparing random number, 0: disabled comparing random number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicRldpCompareRandomNumber(l2sw_uint8 units, l2sw_uint32 *pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_RLDP_CTRL0, L2SW_RLDP_COMP_ID_OFFSET, pEnabled);
}
/* Function Name:
 *      l2sw_setAsicRldpIndicatorSource
 * Description:
 *      Set buzzer and LED source when detecting a loop
 * Input:
 *      src     - 0: ASIC, 1: 8051
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicRldpIndicatorSource(l2sw_uint8 units, l2sw_uint32 src)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_RLDP_CTRL0, L2SW_RLDP_INDICATOR_SOURCE_OFFSET, src);
}
/* Function Name:
 *      l2sw_getAsicRldpIndicatorSource
 * Description:
 *      Get buzzer and LED source when detecting a loop
 * Input:
 *      pSrc    - 0: ASIC, 1: 8051
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicRldpIndicatorSource(l2sw_uint8 units, l2sw_uint32 *pSrc)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_RLDP_CTRL0, L2SW_RLDP_INDICATOR_SOURCE_OFFSET, pSrc);
}
/* Function Name:
 *      l2sw_setAsicRldpCheckingStatePara
 * Description:
 *      Set retry count and retry period of checking state
 * Input:
 *      retryCount  - 0~0xFF (times)
 *      retryPeriod - 0~0xFFFF (ms)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicRldpCheckingStatePara(l2sw_uint8 units, l2sw_uint32 retryCount, l2sw_uint32 retryPeriod)
{
    ret_t retVal;

    if(retryCount > 0xFF)
        return L2SW_ERR_OUT_OF_RANGE;
    if(retryPeriod > L2SW_REGDATAMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicRegBits(units, L2SW_RLDP_RETRY_COUNT_REG, L2SW_RLDP_RETRY_COUNT_CHKSTATE_MASK, retryCount);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return l2sw_setAsicReg(units, L2SW_RLDP_RETRY_PERIOD_CHKSTATE_REG, retryPeriod);
}
/* Function Name:
 *      l2sw_getAsicRldpCheckingStatePara
 * Description:
 *      Get retry count and retry period of checking state
 * Input:
 *      pRetryCount     - 0~0xFF (times)
 *      pRetryPeriod    - 0~0xFFFF (ms)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_getAsicRldpCheckingStatePara(l2sw_uint8 units, l2sw_uint32 *pRetryCount, l2sw_uint32 *pRetryPeriod)
{
    ret_t retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_RLDP_RETRY_COUNT_REG, L2SW_RLDP_RETRY_COUNT_CHKSTATE_MASK, pRetryCount);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return l2sw_getAsicReg(units, L2SW_RLDP_RETRY_PERIOD_CHKSTATE_REG, pRetryPeriod);
}
/* Function Name:
 *      l2sw_setAsicRldpLoopStatePara
 * Description:
 *      Set retry count and retry period of loop state
 * Input:
 *      retryCount  - 0~0xFF (times)
 *      retryPeriod - 0~0xFFFF (ms)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicRldpLoopStatePara(l2sw_uint8 units, l2sw_uint32 retryCount, l2sw_uint32 retryPeriod)
{
    ret_t retVal;

    if(retryCount > 0xFF)
        return L2SW_ERR_OUT_OF_RANGE;

    if(retryPeriod > L2SW_REGDATAMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicRegBits(units, L2SW_RLDP_RETRY_COUNT_REG, L2SW_RLDP_RETRY_COUNT_LOOPSTATE_MASK, retryCount);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return l2sw_setAsicReg(units, L2SW_RLDP_RETRY_PERIOD_LOOPSTATE_REG, retryPeriod);
}
/* Function Name:
 *      l2sw_getAsicRldpLoopStatePara
 * Description:
 *      Get retry count and retry period of loop state
 * Input:
 *      pRetryCount     - 0~0xFF (times)
 *      pRetryPeriod    - 0~0xFFFF (ms)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_getAsicRldpLoopStatePara(l2sw_uint8 units, l2sw_uint32 *pRetryCount, l2sw_uint32 *pRetryPeriod)
{
    ret_t retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_RLDP_RETRY_COUNT_REG, L2SW_RLDP_RETRY_COUNT_LOOPSTATE_MASK, pRetryCount);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return l2sw_getAsicReg(units, L2SW_RLDP_RETRY_PERIOD_LOOPSTATE_REG, pRetryPeriod);
}
/* Function Name:
 *      l2sw_setAsicRldpTxPortmask
 * Description:
 *      Set portmask that send/forward RLDP frame
 * Input:
 *      portmask    - 0~0xFF
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_MASK    - Invalid portmask
 * Note:
 *      None
 */
ret_t l2sw_setAsicRldpTxPortmask(l2sw_uint8 units, l2sw_uint32 portmask)
{
    if(portmask > L2SW_PORTMASK)
        return L2SW_ERR_PORT_MASK;

    return l2sw_setAsicReg(units, L2SW_RLDP_TX_PMSK_REG, portmask);
}
/* Function Name:
 *      l2sw_getAsicRldpTxPortmask
 * Description:
 *      Get portmask that send/forward RLDP frame
 * Input:
 *      pPortmask   - 0~0xFF
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicRldpTxPortmask(l2sw_uint8 units, l2sw_uint32 *pPortmask)
{
    return l2sw_getAsicReg(units, L2SW_RLDP_TX_PMSK_REG, pPortmask);
}
/* Function Name:
 *      l2sw_setAsicRldpMagicNum
 * Description:
 *      Set Random seed of RLDP
 * Input:
 *      seed    - MAC
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicRldpMagicNum(l2sw_uint8 units, ether_addr_t seed)
{
    ret_t retVal;
    l2sw_uint32 regData;
    l2sw_uint16 *accessPtr;
    l2sw_uint32 i;

    accessPtr = (l2sw_uint16*)&seed;

    for (i = 0; i < 3; i++)
    {
        regData = *accessPtr;
        retVal = l2sw_setAsicReg(units, L2SW_RLDP_MAGIC_NUM_REG_BASE + i, regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        accessPtr++;
    }

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicRldpMagicNum
 * Description:
 *      Get Random seed of RLDP
 * Input:
 *      pSeed   - MAC
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicRldpMagicNum(l2sw_uint8 units, ether_addr_t *pSeed)
{
    ret_t retVal;
    l2sw_uint32 regData;
    l2sw_uint16 *accessPtr;
    l2sw_uint32 i;

    accessPtr = (l2sw_uint16*)pSeed;

    for(i = 0; i < 3; i++)
    {
        retVal = l2sw_getAsicReg(units, L2SW_RLDP_MAGIC_NUM_REG_BASE + i, &regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        *accessPtr = regData;
        accessPtr++;
    }

    return retVal;
}

/* Function Name:
 *      l2sw_getAsicRldpLoopedPortmask
 * Description:
 *      Get looped portmask
 * Input:
 *      pPortmask   - 0~0xFF
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicRldpLoopedPortmask(l2sw_uint8 units, l2sw_uint32 *pPortmask)
{
    return l2sw_getAsicReg(units, L2SW_RLDP_LOOP_PMSK_REG, pPortmask);
}
/* Function Name:
 *      l2sw_getAsicRldpRandomNumber
 * Description:
 *      Get Random number of RLDP
 * Input:
 *      pRandNumber     - MAC
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicRldpRandomNumber(l2sw_uint8 units, ether_addr_t *pRandNumber)
{
    ret_t retVal;
    l2sw_uint32 regData;
    l2sw_int16 accessPtr[3];
    l2sw_uint32 i;

    for(i = 0; i < 3; i++)
    {
        retVal = l2sw_getAsicReg(units, L2SW_RLDP_RAND_NUM_REG_BASE+ i, &regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        accessPtr[i] = regData;
    }

    MEMCPY(pRandNumber, accessPtr, 6);
    return retVal;
}
/* Function Name:
 *      l2sw_getAsicRldpLoopedPortmask
 * Description:
 *      Get port number of looped pair
 * Input:
 *      port        - Physical port number (0~7)
 *      pLoopedPair     - port (0~7)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicRldpLoopedPortPair(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pLoopedPair)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 8)
        return l2sw_getAsicRegBits(units, L2SW_RLDP_LOOP_PORT_REG(port), L2SW_RLDP_LOOP_PORT_MASK(port), pLoopedPair);
    else
        return l2sw_getAsicRegBits(units, L2SW_REG_RLDP_LOOP_PORT_REG4 + ((port - 8) >> 1), L2SW_RLDP_LOOP_PORT_MASK(port), pLoopedPair);
}
/* Function Name:
 *      l2sw_setAsicRlppTrap8051
 * Description:
 *      Set trap RLPP packet to 8051
 * Input:
 *      enabled     - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicRlppTrap8051(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_RLDP_CTRL0, L2SW_RLPP_8051_TRAP_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicRlppTrap8051
 * Description:
 *      Get trap RLPP packet to 8051
 * Input:
 *      pEnabled    - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicRlppTrap8051(l2sw_uint8 units, l2sw_uint32 *pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_RLDP_CTRL0, L2SW_RLPP_8051_TRAP_OFFSET, pEnabled);
}
/* Function Name:
 *      l2sw_setAsicRldpLeaveLoopedPortmask
 * Description:
 *      Clear leaved looped portmask
 * Input:
 *      portmask    - 0~0xFF
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicRldpLeaveLoopedPortmask(l2sw_uint8 units, l2sw_uint32 portmask)
{
    return l2sw_setAsicReg(units, L2SW_REG_RLDP_RELEASED_INDICATOR, portmask);
}
/* Function Name:
 *      l2sw_getAsicRldpLeaveLoopedPortmask
 * Description:
 *      Get leaved looped portmask
 * Input:
 *      pPortmask   - 0~0xFF
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicRldpLeaveLoopedPortmask(l2sw_uint8 units, l2sw_uint32 *pPortmask)
{
    return l2sw_getAsicReg(units, L2SW_REG_RLDP_RELEASED_INDICATOR, pPortmask);
}
/* Function Name:
 *      l2sw_setAsicRldpEnterLoopedPortmask
 * Description:
 *      Clear enter loop portmask
 * Input:
 *      portmask    - 0~0xFF
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicRldpEnterLoopedPortmask(l2sw_uint8 units, l2sw_uint32 portmask)
{
    return l2sw_setAsicReg(units, L2SW_REG_RLDP_LOOPED_INDICATOR, portmask);
}
/* Function Name:
 *      l2sw_getAsicRldpEnterLoopedPortmask
 * Description:
 *      Get enter loop portmask
 * Input:
 *      pPortmask   - 0~0xFF
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicRldpEnterLoopedPortmask(l2sw_uint8 units, l2sw_uint32 *pPortmask)
{
    return l2sw_getAsicReg(units, L2SW_REG_RLDP_LOOPED_INDICATOR, pPortmask);
}

/* Function Name:
 *      l2sw_setAsicRldpTriggerMode
 * Description:
 *      Set trigger RLDP mode
 * Input:
 *      mode    - 1: Periodically, 0: SA moving
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicRldpTriggerMode(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_RLDP_CTRL0, L2SW_RLDP_TRIGGER_MODE_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicRldpTriggerMode
 * Description:
 *      Get trigger RLDP mode
 * Input:
 *      pMode   - - 1: Periodically, 0: SA moving
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicRldpTriggerMode(l2sw_uint8 units, l2sw_uint32 *pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_RLDP_CTRL0, L2SW_RLDP_TRIGGER_MODE_OFFSET, pEnabled);
}

/* Function Name:
 *      l2sw_setAsicRldp8051Portmask
 * Description:
 *      Set 8051/CPU configured looped portmask
 * Input:
 *      portmask    - 0~0xFF
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_MASK    - Invalid portmask
 * Note:
 *      None
 */
ret_t l2sw_setAsicRldp8051Portmask(l2sw_uint8 units, l2sw_uint32 portmask)
{
    ret_t retVal;
    if(portmask > L2SW_PORTMASK)
        return L2SW_ERR_PORT_MASK;

    retVal = l2sw_setAsicRegBits(units, L2SW_RLDP_CTRL0_REG,L2SW_RLDP_8051_LOOP_PORTMSK_MASK,portmask & 0xff);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_RLDP_CTRL5,L2SW_RLDP_CTRL5_MASK,(portmask >> 8) & 7);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicRldp8051Portmask
 * Description:
 *      Get 8051/CPU configured looped portmask
 * Input:
 *      pPortmask   - 0~0xFF
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicRldp8051Portmask(l2sw_uint8 units, l2sw_uint32 *pPortmask)
{
    l2sw_uint32 tmpPmsk;
    ret_t retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_RLDP_CTRL0_REG,L2SW_RLDP_8051_LOOP_PORTMSK_MASK,&tmpPmsk);
    if(retVal != L2SW_ERR_OK)
        return retVal;
    *pPortmask = tmpPmsk & 0xff;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_RLDP_CTRL5,L2SW_RLDP_CTRL5_MASK,&tmpPmsk);
    if(retVal != L2SW_ERR_OK)
        return retVal;
    *pPortmask |= (tmpPmsk & 7) <<8;

    return L2SW_ERR_OK;
}

