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

#ifdef L2SW_LINUX_KERNEL
#include "linux/kernel.h"
#endif
#include "sal_string.h"
#include "l2sw_switch.h"
#include "l2sw_asicdrv.h"

#if defined(L2SW_X86_ASICDRV)
#include "I2Clib.h"
#else
#include "smi.h"
#endif


#if defined(COL2SWIG_L2SW865X_CLE) || defined (L2SW_LINUX_KERNEL)
l2sw_uint32 cleDebuggingDisplay;
#endif

#ifdef EMBEDDED_SUPPORT
extern void setReg(l2sw_uint8 units, l2sw_uint16, l2sw_uint16);
extern l2sw_uint16 getReg(l2sw_uint8 units, l2sw_uint16);
#endif

/* Function Name:
 *      l2sw_setAsicRegBit
 * Description:
 *      Set a bit value of a specified register
 * Input:
 *      reg     - register's address
 *      bit     - bit location
 *      value   - value to set. It can be value 0 or 1.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_INPUT    - Invalid input parameter
 * Note:
 *      Set a bit of a specified register to 1 or 0.
 */
ret_t l2sw_setAsicRegBit(l2sw_uint8 units, l2sw_uint32 reg, l2sw_uint32 bit, l2sw_uint32 value)
{

#if defined(L2SW_X86_ASICDRV)
    l2sw_uint32 regData;
    ret_t retVal;

    if(bit >= L2SW_REGBITLENGTH)
        return L2SW_ERR_INPUT;

    retVal = Access_Read(reg, 2, &regData);
    if(L2SW_TRUE != retVal)
        return L2SW_ERR_SMI;

    if(0x8367B == cleDebuggingDisplay)
        PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);

    if(value)
        regData = regData | (1 << bit);
    else
        regData = regData & (~(1 << bit));

    retVal = Access_Write(reg, 2, regData);
    if(L2SW_TRUE != retVal)
        return L2SW_ERR_SMI;

    if(0x8367B == cleDebuggingDisplay)
        PRINT("W[0x%4.4x]=0x%4.4x\n", reg, regData);

#elif defined(EMBEDDED_SUPPORT)
    l2sw_uint16 tmp;

    if(reg > L2SW_REGDATAMAX || value > 1)
        return L2SW_ERR_INPUT;

    tmp = getReg(units, reg);
    tmp &= (1 << bitIdx);
    tmp |= (value << bitIdx);
    setReg(units, reg, tmp);

#else
    l2sw_uint32 regData;
    ret_t retVal;

    if(bit >= L2SW_REGBITLENGTH) {
        //debug(ERROR, "ERR: set reg 0x%x bit %d to %d, errCode=%d\n", reg, bit, value, L2SW_ERR_INPUT);
        return L2SW_ERR_INPUT;
    }
#ifdef L2SW_LINUX_KERNEL
    retVal = l2sw_smi_read(units, reg, &regData);
#else
    retVal = l2sw_smi_read(units, reg, &regData);
#endif
    if(retVal != L2SW_ERR_OK) {
        //debug(ERROR, "ERR: set reg 0x%x bit %d to %d, errCode=%d\n", reg, bit, value, L2SW_ERR_SMI);
        return L2SW_ERR_SMI;
    }

  #ifdef COL2SWIG_L2SW865X_CLE
    if(0x8367B == cleDebuggingDisplay)
        PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);
  #endif
    if(value)
        regData = regData | (1 << bit);
    else
        regData = regData & (~(1 << bit));
#ifdef L2SW_LINUX_KERNEL
    retVal = l2sw_smi_write(units, reg, regData);
#else
    retVal = l2sw_smi_write(units, reg, regData);
#endif
    if(retVal != L2SW_ERR_OK) {
        //debug(ERROR, "ERR: set reg 0x%x bit %d to %d, errCode=%d\n", reg, bit, value, L2SW_ERR_SMI);
        return L2SW_ERR_SMI;
    }

  #ifdef COL2SWIG_L2SW865X_CLE
    if(0x8367B == cleDebuggingDisplay)
        PRINT("W[0x%4.4x]=0x%4.4x\n", reg, regData);
  #endif

#endif
    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicRegBit
 * Description:
 *      Get a bit value of a specified register
 * Input:
 *      reg     - register's address
 *      bit     - bit location
 *      value   - value to get.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_INPUT    - Invalid input parameter
 * Note:
 *      None
 */
ret_t l2sw_getAsicRegBit(l2sw_uint8 units, l2sw_uint32 reg, l2sw_uint32 bit, l2sw_uint32 *pValue)
{

#if defined(L2SW_X86_ASICDRV)

    l2sw_uint32 regData;
    ret_t retVal;

    if(bit >= L2SW_REGBITLENGTH)
        return L2SW_ERR_INPUT;

    retVal = Access_Read(reg, 2, &regData);
    if(L2SW_TRUE != retVal)
        return L2SW_ERR_SMI;

    *pValue = (regData & (0x1 << bit)) >> bit;

    if(0x8367B == cleDebuggingDisplay)
        PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);

#elif defined(EMBEDDED_SUPPORT)
    l2sw_uint16 tmp;

    if(reg > L2SW_REGDATAMAX )
        return L2SW_ERR_INPUT;

    tmp = getReg(units, reg);
    tmp = tmp >> bitIdx;
    tmp &= 1;
    *value = tmp;
#else
    l2sw_uint32 regData;
    ret_t retVal;
#ifdef L2SW_LINUX_KERNEL
    retVal = l2sw_smi_read(units, reg, &regData);
#else
	retVal = l2sw_smi_read(units, reg, &regData);
#endif
    if(retVal != L2SW_ERR_OK) {
        //debug(ERROR, "ERR: get reg 0x%x bit %d, errCode=%d\n", reg, bit, L2SW_ERR_SMI);
        return L2SW_ERR_SMI;
    }

  #ifdef COL2SWIG_L2SW865X_CLE
    if(0x8367B == cleDebuggingDisplay)
        PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);
  #endif

    *pValue = (regData & (0x1 << bit)) >> bit;

#endif
    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicRegBits
 * Description:
 *      Set bits value of a specified register
 * Input:
 *      reg     - register's address
 *      bits    - bits mask for setting
 *      value   - bits value for setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_INPUT    - Invalid input parameter
 * Note:
 *      Set bits of a specified register to value. Both bits and value are be treated as bit-mask
 */
ret_t l2sw_setAsicRegBits(l2sw_uint8 units, l2sw_uint32 reg, l2sw_uint32 bits, l2sw_uint32 value)
{

#if defined(L2SW_X86_ASICDRV)

    l2sw_uint32 regData;
    ret_t retVal;
    l2sw_uint32 bitsShift;
    l2sw_uint32 valueShifted;

    if(bits >= (1 << L2SW_REGBITLENGTH) )
        return L2SW_ERR_INPUT;

    bitsShift = 0;
    while(!(bits & (1 << bitsShift)))
    {
        bitsShift++;
        if(bitsShift >= L2SW_REGBITLENGTH)
            return L2SW_ERR_INPUT;
    }

    valueShifted = value << bitsShift;
    if(valueShifted > L2SW_REGDATAMAX)
        return L2SW_ERR_INPUT;

    retVal = Access_Read(reg, 2, &regData);
    if(L2SW_TRUE != retVal)
        return L2SW_ERR_SMI;

    if(0x8367B == cleDebuggingDisplay)
        PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);

    regData = regData & (~bits);
    regData = regData | (valueShifted & bits);

    retVal = Access_Write(reg,2, regData);
    if(L2SW_TRUE != retVal)
        return L2SW_ERR_SMI;

    if(0x8367B == cleDebuggingDisplay)
        PRINT("W[0x%4.4x]=0x%4.4x\n", reg, regData);

#elif defined(EMBEDDED_SUPPORT)
    l2sw_uint32 regData;
    l2sw_uint32 bitsShift;
    l2sw_uint32 valueShifted;

    if(reg > L2SW_REGDATAMAX )
        return L2SW_ERR_INPUT;

    if(bits >= (1 << L2SW_REGBITLENGTH) )
        return L2SW_ERR_INPUT;

    bitsShift = 0;
    while(!(bits & (1 << bitsShift)))
    {
        bitsShift++;
        if(bitsShift >= L2SW_REGBITLENGTH)
            return L2SW_ERR_INPUT;
    }

    valueShifted = value << bitsShift;
    if(valueShifted > L2SW_REGDATAMAX)
        return L2SW_ERR_INPUT;

    regData = getReg(reg);
    regData = regData & (~bits);
    regData = regData | (valueShifted & bits);

    setReg(units, reg, regData);

#else
    l2sw_uint32 regData;
    ret_t retVal;
    l2sw_uint32 bitsShift;
    l2sw_uint32 valueShifted;

    if(bits >= (1 << L2SW_REGBITLENGTH) ) {
        //debug(ERROR, "ERR: set reg 0x%x bits 0x%x to 0x%x, errCode=%d\n", reg, bits, value, L2SW_ERR_INPUT);
        return L2SW_ERR_INPUT;
    }

    bitsShift = 0;
    while(!(bits & (1 << bitsShift)))
    {
        bitsShift++;
        if(bitsShift >= L2SW_REGBITLENGTH) {
            //debug(ERROR, "ERR: set reg 0x%x bits 0x%x to 0x%x, errCode=%d\n", reg, bits, value, L2SW_ERR_INPUT);
            return L2SW_ERR_INPUT;
        }
    }
    valueShifted = value << bitsShift;

    if(valueShifted > L2SW_REGDATAMAX) {
        //debug(ERROR, "ERR: set reg 0x%x bits 0x%x to 0x%x, errCode=%d\n", reg, bits, value, L2SW_ERR_INPUT);
        return L2SW_ERR_INPUT;
    }
#ifdef L2SW_LINUX_KERNEL
    retVal = l2sw_smi_read(units, reg, &regData);
#else
    retVal = l2sw_smi_read(units, reg, &regData);
#endif
    if(retVal != L2SW_ERR_OK) {
        //debug(ERROR, "ERR: set reg 0x%x bits 0x%x to 0x%x, errCode=%d\n", reg, bits, value, L2SW_ERR_SMI);
        return L2SW_ERR_SMI;
    }
  #ifdef COL2SWIG_L2SW865X_CLE
    if(0x8367B == cleDebuggingDisplay)
        PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);
  #endif

    regData = regData & (~bits);
    regData = regData | (valueShifted & bits);
#ifdef L2SW_LINUX_KERNEL
    retVal = l2sw_smi_write(units, reg, regData);
#else
	retVal = l2sw_smi_write(units, reg, regData);
#endif
    if(retVal != L2SW_ERR_OK) {
        //debug(ERROR, "ERR: set reg 0x%x bits 0x%x to 0x%x, errCode=%d\n", reg, bits, value, L2SW_ERR_SMI);
        return L2SW_ERR_SMI;
    }
  #ifdef COL2SWIG_L2SW865X_CLE
    if(0x8367B == cleDebuggingDisplay)
        PRINT("W[0x%4.4x]=0x%4.4x\n", reg, regData);
  #endif
#endif
    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicRegBits
 * Description:
 *      Get bits value of a specified register
 * Input:
 *      reg     - register's address
 *      bits    - bits mask for setting
 *      value   - bits value for setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_INPUT    - Invalid input parameter
 * Note:
 *      None
 */
ret_t l2sw_getAsicRegBits(l2sw_uint8 units, l2sw_uint32 reg, l2sw_uint32 bits, l2sw_uint32 *pValue)
{

#if defined(L2SW_X86_ASICDRV)

    l2sw_uint32 regData;
    ret_t retVal;
    l2sw_uint32 bitsShift;

    if(bits >= (1 << L2SW_REGBITLENGTH) )
        return L2SW_ERR_INPUT;

    bitsShift = 0;
    while(!(bits & (1 << bitsShift)))
    {
        bitsShift++;
        if(bitsShift >= L2SW_REGBITLENGTH)
            return L2SW_ERR_INPUT;
    }

    retVal = Access_Read(reg, 2, &regData);
    if(L2SW_TRUE != retVal)
        return L2SW_ERR_SMI;

    *pValue = (regData & bits) >> bitsShift;

    if(0x8367B == cleDebuggingDisplay)
        PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);

#elif defined(EMBEDDED_SUPPORT)
    l2sw_uint32 regData;
    l2sw_uint32 bitsShift;

    if(reg > L2SW_REGDATAMAX )
        return L2SW_ERR_INPUT;

    if(bits >= (1UL << L2SW_REGBITLENGTH) )
        return L2SW_ERR_INPUT;

    bitsShift = 0;
    while(!(bits & (1UL << bitsShift)))
    {
        bitsShift++;
        if(bitsShift >= L2SW_REGBITLENGTH)
            return L2SW_ERR_INPUT;
    }

    regData = getReg(units, reg);
    *value = (regData & bits) >> bitsShift;

#else
    l2sw_uint32 regData;
    ret_t retVal;
    l2sw_uint32 bitsShift;

    if(bits>= (1<<L2SW_REGBITLENGTH) ) {
        //debug(ERROR, "ERR: get reg 0x%x bits 0x%x, errCode=%d\n", reg, bits, L2SW_ERR_INPUT);
        return L2SW_ERR_INPUT;
    }

    bitsShift = 0;
    while(!(bits & (1 << bitsShift)))
    {
        bitsShift++;
        if(bitsShift >= L2SW_REGBITLENGTH) {
            //debug(ERROR, "ERR: get reg 0x%x bits 0x%x, errCode=%d\n", reg, bits, L2SW_ERR_INPUT);
            return L2SW_ERR_INPUT;
        }
    }
#ifdef L2SW_LINUX_KERNEL
    retVal = l2sw_smi_read(units, reg, &regData);
#else
    retVal = l2sw_smi_read(units, reg, &regData);
#endif
    if(retVal != L2SW_ERR_OK) {
        //debug(ERROR, "ERR: get reg 0x%x bits 0x%x, errCode=%d\n", reg, bits, L2SW_ERR_SMI);
        return L2SW_ERR_SMI;
    }
    *pValue = (regData & bits) >> bitsShift;
  #ifdef COL2SWIG_L2SW865X_CLE
    if(0x8367B == cleDebuggingDisplay)
        PRINT("R[0x%4.4x]=0x%4.4x\n",reg, regData);
  #endif

#endif
    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicReg
 * Description:
 *      Set content of asic register
 * Input:
 *      reg     - register's address
 *      value   - Value setting to register
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      The value will be set to ASIC mapping address only and it is always return L2SW_ERR_OK while setting un-mapping address registers
 */
ret_t l2sw_setAsicReg(l2sw_uint8 units, l2sw_uint32 reg, l2sw_uint32 value)
{
#if defined(L2SW_X86_ASICDRV)/*L2SW-CNSD2-NickWu-20061222: for x86 compile*/

    ret_t retVal;

    retVal = Access_Write(reg,2,value);
    if(L2SW_TRUE != retVal) return L2SW_ERR_SMI;

    if(0x8367B == cleDebuggingDisplay)
        PRINT("W[0x%4.4x]=0x%4.4x\n",reg,value);

#else
    ret_t retVal;
#ifdef L2SW_LINUX_KERNEL
    retVal = l2sw_smi_write(units, reg, value);
#else
	retVal = l2sw_smi_write(units, reg, value);
#endif
    if(retVal != L2SW_ERR_OK) {
        //debug(ERROR, "ERR: set reg 0x%x to 0x%x, errCode=%d\n", reg, value, L2SW_ERR_SMI);
        return L2SW_ERR_SMI;
    }
  #ifdef COL2SWIG_L2SW865X_CLE
    if(0x8367B == cleDebuggingDisplay)
        PRINT("W[0x%4.4x]=0x%4.4x\n",reg,value);
  #endif

#endif

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicReg
 * Description:
 *      Get content of asic register
 * Input:
 *      reg     - register's address
 *      value   - Value setting to register
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      Value 0x0000 will be returned for ASIC un-mapping address
 */
ret_t l2sw_getAsicReg(l2sw_uint8 units, l2sw_uint32 reg, l2sw_uint32 *pValue)
{

#if defined(L2SW_X86_ASICDRV)

    l2sw_uint32 regData;
    ret_t retVal;

    retVal = Access_Read(reg, 2, &regData);
    if(L2SW_TRUE != retVal)
        return L2SW_ERR_SMI;

    *pValue = regData;

    if(0x8367B == cleDebuggingDisplay)
        PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);

#elif defined(EMBEDDED_SUPPORT)
    if(reg > L2SW_REGDATAMAX)
        return L2SW_ERR_INPUT;

    *value = getReg(units, reg);

#else
    l2sw_uint32 regData;
    ret_t retVal;
#ifdef L2SW_LINUX_KERNEL
    retVal = l2sw_smi_read(units, reg, &regData);
#else
	retVal = l2sw_smi_read(units, reg, &regData);
#endif
    if(retVal != L2SW_ERR_OK) {
        //debug(ERROR, "ERR: get reg 0x%x, errCode=%d\n", reg, L2SW_ERR_SMI);
        return L2SW_ERR_SMI;
    }

    *pValue = regData;
  #ifdef COL2SWIG_L2SW865X_CLE
    if(0x8367B == cleDebuggingDisplay)
        PRINT("R[0x%4.4x]=0x%4.4x\n", reg, regData);
  #endif

#endif

    return L2SW_ERR_OK;
}

int sf_l2sw_setAsicReg(unsigned int reg, unsigned int pValue)
{
	return l2sw_setAsicReg(0, reg, pValue);
}

int sf_l2sw_getAsicReg(unsigned int reg, unsigned int *pValue)
{
	return l2sw_getAsicReg(0, reg, pValue);
}
