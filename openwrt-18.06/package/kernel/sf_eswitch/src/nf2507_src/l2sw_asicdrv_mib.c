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

#include "l2sw_asicdrv_mib.h"

/* Function Name:
 *      l2sw_setAsicMIBsCounterReset
 * Description:
 *      Reset global/queue manage or per-port MIB counter
 * Input:
 *      greset  - Global reset
 *      qmreset - Queue maganement reset
 *      portmask    - Port reset mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicMIBsCounterReset(l2sw_uint8 units, l2sw_uint32 greset, l2sw_uint32 qmreset, l2sw_uint32 portmask)
{
    ret_t retVal;
    l2sw_uint32 regData;
    l2sw_uint32 regBits;

    regBits = L2SW_GLOBAL_RESET_MASK |
                L2SW_QM_RESET_MASK |
                    L2SW_MIB_PORT07_MASK |
                    ((l2sw_uint32)0x7 << 13);
    regData = ((greset << L2SW_GLOBAL_RESET_OFFSET) & L2SW_GLOBAL_RESET_MASK) |
                ((qmreset << L2SW_QM_RESET_OFFSET) & L2SW_QM_RESET_MASK) |
                (((portmask & 0xFF) << L2SW_PORT0_RESET_OFFSET) & L2SW_MIB_PORT07_MASK) |
                (((portmask >> 8)&0x7) << 13);


    retVal = l2sw_setAsicRegBits(units, L2SW_REG_MIB_CTRL0, regBits, (regData >> L2SW_PORT0_RESET_OFFSET));

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicMIBsCounter
 * Description:
 *      Get MIBs counter
 * Input:
 *      port        - Physical port number (0~7)
 *      mibIdx      - MIB counter index
 *      pCounter    - MIB retrived counter
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - MIB is busy at retrieving
 *      L2SW_ERR_STAT_CNTR_FAIL   - MIB is resetting
 * Note:
 *      Before MIBs counter retrieving, writting accessing address to ASIC at first and check the MIB
 *      control register status. If busy bit of MIB control is set, that means MIB counter have been
 *      waiting for preparing, then software must wait atfer this busy flag reset by ASIC. This driver
 *      did not recycle reading user desired counter. Software must use driver again to get MIB counter
 *      if return value is not L2SW_ERR_OK.
 */
ret_t l2sw_getAsicMIBsCounter(l2sw_uint8 units, l2sw_uint32 port, L2SW_MIBCOUNTER mibIdx, l2sw_uint64* pCounter)
{
    ret_t retVal;
    l2sw_uint32 regAddr;
    l2sw_uint32 regData;
    l2sw_uint32 mibAddr;
    l2sw_uint32 mibOff=0;

    /* address offset to MIBs counter */
    CONST l2sw_uint16 mibLength[L2SW_MIBS_NUMBER]= {
        4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        4,2,2,2,2,2,2,2,2,
        4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
    /*port 5 address offset*/
    CONST l2sw_uint16 mibLength1[L2SW_MIBCOUNTER_NUMBER]= {
        2,2,2,2,
        1,0,1,0,2,
        1,0,1,0,1,0,1,
        1,0,1,0,1,0,1,
        1,0,1,0,2,
        1,0,1,0,1,0,1,0,
        1,3};
    l2sw_uint16 i = 0;
    l2sw_uint64 mibCounter;


    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(mibIdx >= L2SW_MIBS_NUMBER)
        return L2SW_ERR_STAT_INVALID_CNTR;

    if(dot1dTpLearnedEntryDiscards == mibIdx)
    {
        mibAddr = L2SW_MIB_LEARNENTRYDISCARD_OFFSET;
    }
    else if(port == 5 || port == 15){
        i = 0;
        mibOff = L2SW_MIB_PORT_OFFSET * UTP_PORT5;

        while(i < mibIdx)
        {
            mibOff += mibLength1[i];
            i++;
        }

        mibAddr = mibOff;

    }
    else
    {
        i = 0;
        mibOff = L2SW_MIB_PORT_OFFSET * port;

        if(port > 7)
            mibOff = mibOff + 68;

        while(i < mibIdx)
        {
            mibOff += mibLength[i];
            i++;
        }

        mibAddr = mibOff;
    }

    /* Read MIB addr before writing */
    retVal = l2sw_getAsicReg(units, L2SW_REG_MIB_ADDRESS, &regData);
    if(retVal != L2SW_ERR_OK)
    {
        return retVal;
    }


    if (regData == (mibAddr >> 2))
    {
        /* Write MIB addr to an alternate value */
        retVal = l2sw_setAsicReg(units, L2SW_REG_MIB_ADDRESS, (mibAddr >> 2) + 1);
        if(retVal != L2SW_ERR_OK)
        {
            return retVal;
        }
        while(1)
        {
            retVal = l2sw_getAsicReg(units, L2SW_REG_MIB_ADDRESS, &regData);
            if(retVal != L2SW_ERR_OK)
                return retVal;

            if(regData == ((mibAddr >> 2) + 1))
            {
                break;
            }

            retVal = l2sw_setAsicReg(units, L2SW_REG_MIB_ADDRESS, (mibAddr >> 2) + 1);
            if(retVal != L2SW_ERR_OK)
            {
                return retVal;
            }
        }

        /* polling busy flag */
        i = 100;
        while(i > 0)
        {
            /*read MIB control register*/
            retVal = l2sw_getAsicReg(units, L2SW_MIB_CTRL_REG,&regData);
            if(retVal != L2SW_ERR_OK)
            {
                return retVal;
            }


            if((regData & L2SW_MIB_CTRL0_BUSY_FLAG_MASK) == 0)
            {
                break;
            }

            i--;
        }

        if(regData & L2SW_MIB_CTRL0_BUSY_FLAG_MASK)
        {
            return L2SW_ERR_BUSYWAIT_TIMEOUT;
        }


        if(regData & L2SW_RESET_FLAG_MASK)
        {
            return L2SW_ERR_STAT_CNTR_FAIL;
        }

    }

    /*writing access counter address first*/
    /*This address is SRAM address, and SRAM address = MIB register address >> 2*/
    /*then ASIC will prepare 64bits counter wait for being retrived*/
    /*Write Mib related address to access control register*/
    if(mibLength1[i]==0 && (mibAddr >> 2) == 0xa2){
 ////       mibAddr=0x287;
    }

    retVal = l2sw_setAsicReg(units, L2SW_REG_MIB_ADDRESS, (mibAddr >> 2));

    if(retVal != L2SW_ERR_OK)
    {
        return retVal;
    }


    /* polling MIB Addr register */
    while(1)
    {
        retVal = l2sw_getAsicReg(units, L2SW_REG_MIB_ADDRESS, &regData);
        if(retVal != L2SW_ERR_OK)
        {
            return retVal;
        }

        if(regData == (mibAddr >> 2))
        {
            break;
        }

        retVal = l2sw_setAsicReg(units, L2SW_REG_MIB_ADDRESS, (mibAddr >> 2));
        
        if(retVal != L2SW_ERR_OK)
        {
            return retVal;
        }            
    }

    /* polling busy flag */
    i = 100;
    while(i > 0)
    {
        /*read MIB control register*/
        retVal = l2sw_getAsicReg(units, L2SW_MIB_CTRL_REG,&regData);
        if(retVal != L2SW_ERR_OK)
        {
            return retVal;
        }
            

        if((regData & L2SW_MIB_CTRL0_BUSY_FLAG_MASK) == 0)
        {
            break;
        }

        i--;
    }

    if(regData & L2SW_MIB_CTRL0_BUSY_FLAG_MASK)
    {
        return L2SW_ERR_BUSYWAIT_TIMEOUT;
    }
        

    if(regData & L2SW_RESET_FLAG_MASK)
    {
        return L2SW_ERR_STAT_CNTR_FAIL;
    }
        

    mibCounter = 0;
    /*port 5*/
    if(port == 5)
    {
        i = mibLength1[mibIdx];
        if(3 == i){
            *pCounter = mibCounter;
            return L2SW_ERR_OK;

        }else if (1 == i){
            regAddr = L2SW_MIB_COUNTER_BASE_REG + (mibOff % 4);
            retVal = l2sw_getAsicReg(units, regAddr, &regData);
            if(retVal != L2SW_ERR_OK)
                return retVal;

            mibCounter = regData & 0xFF;

        }else if (0 == i){
            regAddr = L2SW_MIB_COUNTER_BASE_REG + ((mibOff - 1) % 4);
            retVal = l2sw_getAsicReg(units, regAddr, &regData);
            if(retVal != L2SW_ERR_OK)
                return retVal;
            mibCounter = (regData >> 8) & 0xFF;
        }
        else{
            regAddr = L2SW_MIB_COUNTER_BASE_REG + ((mibOff + 1) % 4);

            while(i)
            {
                retVal = l2sw_getAsicReg(units, regAddr, &regData);
                if(retVal != L2SW_ERR_OK)
                    return retVal;               
                mibCounter = (mibCounter << 16) | (regData & 0xFFFF);

                regAddr --;
                i --;
            }
        }
        
    }
    /*other ports*/
    else
    {
        i = mibLength[mibIdx];
        if(4 == i)
            regAddr = L2SW_MIB_COUNTER_BASE_REG + 3;
        else
            regAddr = L2SW_MIB_COUNTER_BASE_REG + ((mibOff + 1) % 4);
        
        while(i)
        {
            retVal = l2sw_getAsicReg(units, regAddr, &regData);
            if(retVal != L2SW_ERR_OK)
                return retVal;

            mibCounter = (mibCounter << 16) | (regData & 0xFFFF);

            regAddr --;
            i --;

        }
    }

    *pCounter = mibCounter;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicMIBsLogCounter
 * Description:
 *      Get MIBs Loggin counter
 * Input:
 *      index       - The index of 32 logging counter (0 ~ 31)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_ENTRY_INDEX      - Wrong index
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - MIB is busy at retrieving
 *      L2SW_ERR_STAT_CNTR_FAIL   - MIB is resetting
 * Note:
 *      This API get 32 logging counter
 */
ret_t l2sw_getAsicMIBsLogCounter(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 *pCounter)
{
    ret_t retVal;
    l2sw_uint32 regAddr;
    l2sw_uint32 regData;
    l2sw_uint32 mibAddr;
    l2sw_uint16 i;
    l2sw_uint64 mibCounter;

    if(index > L2SW_MIB_MAX_LOG_CNT_IDX)
        return L2SW_ERR_ENTRY_INDEX;

    mibAddr = L2SW_MIB_LOG_CNT_OFFSET + ((index / 2) * 4);

    retVal = l2sw_setAsicReg(units, L2SW_REG_MIB_ADDRESS, (mibAddr >> 2));
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /*read MIB control register*/
    retVal = l2sw_getAsicReg(units, L2SW_MIB_CTRL_REG, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    if(regData & L2SW_MIB_CTRL0_BUSY_FLAG_MASK)
        return L2SW_ERR_BUSYWAIT_TIMEOUT;

    if(regData & L2SW_RESET_FLAG_MASK)
        return L2SW_ERR_STAT_CNTR_FAIL;

    mibCounter = 0;
    if((index % 2) == 1)
        regAddr = L2SW_MIB_COUNTER_BASE_REG + 3;
    else
        regAddr = L2SW_MIB_COUNTER_BASE_REG + 1;

    for(i = 0; i <= 1; i++)
    {
        retVal = l2sw_getAsicReg(units, regAddr, &regData);

        if(retVal != L2SW_ERR_OK)
            return retVal;

        mibCounter = (mibCounter << 16) | (regData & 0xFFFF);

        regAddr --;
    }

    *pCounter = mibCounter;
    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicMIBsControl
 * Description:
 *      Get MIB control register
 * Input:
 *      pMask       - MIB control status mask bit[0]-busy bit[1]
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 * Note:
 *      Software need to check this control register atfer doing port resetting or global resetting
 */
ret_t l2sw_getAsicMIBsControl(l2sw_uint8 units, l2sw_uint32* pMask)
{
    ret_t retVal;
    l2sw_uint32 regData;

    retVal = l2sw_getAsicReg(units, L2SW_MIB_CTRL_REG, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *pMask = regData & (L2SW_MIB_CTRL0_BUSY_FLAG_MASK | L2SW_RESET_FLAG_MASK);

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicMIBsResetValue
 * Description:
 *      Reset all counter to 0 or 1
 * Input:
 *      value           - Reset to value 0 or 1
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicMIBsResetValue(l2sw_uint8 units, l2sw_uint32 value)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_MIB_CTRL0, L2SW_RESET_VALUE_OFFSET, value);
}
/* Function Name:
 *      l2sw_getAsicMIBsResetValue
 * Description:
 *      Reset all counter to 0 or 1
 * Input:
 *      value           - Reset to value 0 or 1
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicMIBsResetValue(l2sw_uint8 units, l2sw_uint32* value)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_MIB_CTRL0, L2SW_RESET_VALUE_OFFSET, value);
}

/* Function Name:
 *      l2sw_setAsicMIBsUsageMode
 * Description:
 *      MIB update mode
 * Input:
 *      mode            - 1: latch all MIBs by timer 0:normal free run counting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicMIBsUsageMode(l2sw_uint8 units, l2sw_uint32 mode)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_MIB_CTRL4, L2SW_MIB_USAGE_MODE_OFFSET, mode);
}
/* Function Name:
 *      l2sw_getAsicMIBsUsageMode
 * Description:
 *      MIB update mode
 * Input:
 *      pMode           - 1: latch all MIBs by timer 0:normal free run counting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicMIBsUsageMode(l2sw_uint8 units, l2sw_uint32* pMode)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_MIB_CTRL4, L2SW_MIB_USAGE_MODE_OFFSET, pMode);
}

/* Function Name:
 *      l2sw_setAsicMIBsTimer
 * Description:
 *      MIB latching timer
 * Input:
 *      timer           - latch timer, unit 1 second
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicMIBsTimer(l2sw_uint8 units, l2sw_uint32 timer)
{
    return l2sw_setAsicRegBits(units, L2SW_REG_MIB_CTRL4, L2SW_MIB_TIMER_MASK, timer);
}
/* Function Name:
 *      l2sw_getAsicMIBsTimer
 * Description:
 *      MIB latching timer
 * Input:
 *      pTimer          - latch timer, unit 1 second
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicMIBsTimer(l2sw_uint8 units, l2sw_uint32* pTimer)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_MIB_CTRL4, L2SW_MIB_TIMER_MASK, pTimer);
}
/* Function Name:
 *      l2sw_setAsicMIBsLoggingMode
 * Description:
 *      MIB logging counter mode
 * Input:
 *      index   - logging counter mode index (0~15)
 *      mode    - 0:32-bits mode 1:64-bits mode
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicMIBsLoggingMode(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 mode)
{
    if(index > L2SW_MIB_MAX_LOG_MODE_IDX)
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_setAsicRegBit(units, L2SW_REG_MIB_CTRL3, index,mode);
}
/* Function Name:
 *      l2sw_getAsicMIBsLoggingMode
 * Description:
 *      MIB logging counter mode
 * Input:
 *      index   - logging counter mode index (0~15)
 *      pMode   - 0:32-bits mode 1:64-bits mode
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_getAsicMIBsLoggingMode(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pMode)
{
    if(index > L2SW_MIB_MAX_LOG_MODE_IDX)
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_getAsicRegBit(units, L2SW_REG_MIB_CTRL3, index,pMode);
}

/* Function Name:
 *      l2sw_setAsicMIBsLoggingType
 * Description:
 *      MIB logging counter type
 * Input:
 *      index   - logging counter mode index (0~15)
 *      type    - 0:Packet count 1:Byte count
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicMIBsLoggingType(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 type)
{
    if(index > L2SW_MIB_MAX_LOG_MODE_IDX)
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_setAsicRegBit(units, L2SW_REG_MIB_CTRL5, index,type);
}

/* Function Name:
 *      l2sw_getAsicMIBsLoggingType
 * Description:
 *      MIB logging counter type
 * Input:
 *      index   - logging counter mode index (0~15)
 *      pType   - 0:Packet count 1:Byte count
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_getAsicMIBsLoggingType(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pType)
{
    if(index > L2SW_MIB_MAX_LOG_MODE_IDX)
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_getAsicRegBit(units, L2SW_REG_MIB_CTRL5, index,pType);
}

/* Function Name:
 *      l2sw_setAsicMIBsResetLoggingCounter
 * Description:
 *      MIB logging counter type
 * Input:
 *      index   - logging counter index (0~31)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicMIBsResetLoggingCounter(l2sw_uint8 units, l2sw_uint32 index)
{
    ret_t retVal;

    if(index > L2SW_MIB_MAX_LOG_CNT_IDX)
        return L2SW_ERR_OUT_OF_RANGE;

    if(index < 16)
        retVal = l2sw_setAsicReg(units, L2SW_REG_MIB_CTRL1, 1<<index);
    else
        retVal = l2sw_setAsicReg(units, L2SW_REG_MIB_CTRL2, 1<<(index-16));

    return retVal;
}

/* Function Name:
 *      l2sw_setAsicMIBsLength
 * Description:
 *      Set MIB length couting mode
 * Input:
 *      txLengthMode    - 0: tag length doesn't be counted. 1: tag length is counted.
 *      rxLengthMode    - 0: tag length doesn't be counted. 1: tag length is counted.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicMIBsLength(l2sw_uint8 units, l2sw_uint32 txLengthMode, l2sw_uint32 rxLengthMode)
{
    ret_t retVal;

    if( (retVal = l2sw_setAsicRegBit(units, L2SW_REG_MIB_RMON_LEN_CTRL, L2SW_TX_LENGTH_CTRL_OFFSET, txLengthMode)) != L2SW_ERR_OK)
        return retVal;

    if( (retVal = l2sw_setAsicRegBit(units, L2SW_REG_MIB_RMON_LEN_CTRL, L2SW_RX_LENGTH_CTRL_OFFSET, rxLengthMode)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicMIBsLength
 * Description:
 *      Set MIB length couting mode
 * Input:
 *      None.
 * Output:
 *      pTxLengthMode - 0: tag length doesn't be counted. 1: tag length is counted.
 *      pRxLengthMode - 0: tag length doesn't be counted. 1: tag length is counted.
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_getAsicMIBsLength(l2sw_uint8 units, l2sw_uint32 *pTxLengthMode, l2sw_uint32 *pRxLengthMode)
{
    ret_t retVal;

    if( (retVal = l2sw_getAsicRegBit(units, L2SW_REG_MIB_RMON_LEN_CTRL, L2SW_TX_LENGTH_CTRL_OFFSET, pTxLengthMode)) != L2SW_ERR_OK)
        return retVal;

    if( (retVal = l2sw_getAsicRegBit(units, L2SW_REG_MIB_RMON_LEN_CTRL, L2SW_RX_LENGTH_CTRL_OFFSET, pRxLengthMode)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

