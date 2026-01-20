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

#include "l2sw_asicdrv_phy.h"

#if !defined(USE_L2SW_SDK)
/* Function Name:
 *      l2sw_setAsicPHYOCPReg
 * Description:
 *      Set PHY OCP registers
 * Input:
 *      phyNo   - Physical port number (0~7)
 *      ocpAddr - OCP address
 *      ocpData - Writing data
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PHY_REG_ID       - invalid PHY address
 *      L2SW_ERR_PHY_ID           - invalid PHY no
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      None
 */
ret_t l2sw_setAsicPHYOCPReg(l2sw_uint8 units, l2sw_uint32 phyNo, l2sw_uint32 ocpAddr, l2sw_uint32 ocpData )
{
    ret_t retVal;
    l2sw_uint32 regAddr;
    l2sw_uint32 ocpAddrPrefix, ocpAddr9_6, ocpAddr5_1;

    /* OCP prefix */
    ocpAddrPrefix = ((ocpAddr & 0xFC00) >> 10);

#if defined(USE_L2SW_SDK)
#else
    if((retVal = l2sw_setAsicRegBits(units, L2SW_REG_GPHY_OCP_MSB_0, L2SW_CFG_CPU_OCPADR_MSB_MASK, ocpAddrPrefix)) != L2SW_ERR_OK)
        return retVal;
#endif

    /*prepare access address*/
    ocpAddr9_6 = ((ocpAddr >> 6) & 0x000F);
    ocpAddr5_1 = ((ocpAddr >> 1) & 0x001F);

#if defined(USE_L2SW_SDK)
    regAddr = (ocpAddr9_6 << 8) | (phyNo << L2SW_PHY_OFFSET) | ocpAddr5_1;
#else
    regAddr = L2SW_PHY_BASE | (ocpAddr9_6 << 8) | (phyNo << L2SW_PHY_OFFSET) | ocpAddr5_1;
#endif

    if((retVal = l2sw_setAsicReg(units, regAddr, ocpData)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicPHYOCPReg
 * Description:
 *      Get PHY OCP registers
 * Input:
 *      phyNo   - Physical port number (0~7)
 *      ocpAddr - PHY address
 *      pRegData - read data
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PHY_REG_ID       - invalid PHY address
 *      L2SW_ERR_PHY_ID           - invalid PHY no
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      None
 */
ret_t l2sw_getAsicPHYOCPReg(l2sw_uint8 units, l2sw_uint32 phyNo, l2sw_uint32 ocpAddr, l2sw_uint32 *pRegData )
{
    ret_t retVal;
    l2sw_uint32 regAddr;
    l2sw_uint32 ocpAddrPrefix, ocpAddr9_6, ocpAddr5_1;
    /* OCP prefix */
    ocpAddrPrefix = ((ocpAddr & 0xFC00) >> 10);

#if defined(USE_L2SW_SDK)
#else
    if((retVal = l2sw_setAsicRegBits(units, L2SW_REG_GPHY_OCP_MSB_0, L2SW_CFG_CPU_OCPADR_MSB_MASK, ocpAddrPrefix)) != L2SW_ERR_OK)
        return retVal;
#endif

    /*prepare access address*/
    ocpAddr9_6 = ((ocpAddr >> 6) & 0x000F);
    ocpAddr5_1 = ((ocpAddr >> 1) & 0x001F);

#if defined(USE_L2SW_SDK)
    regAddr = (ocpAddr9_6 << 8) | (phyNo << L2SW_PHY_OFFSET) | ocpAddr5_1;
#else
    regAddr = L2SW_PHY_BASE | (ocpAddr9_6 << 8) | (phyNo << L2SW_PHY_OFFSET) | ocpAddr5_1;
#endif

    if((retVal = l2sw_getAsicReg(units, regAddr, pRegData)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

#else

/* Function Name:
 *      l2sw_setAsicPHYOCPReg
 * Description:
 *      Set PHY OCP registers
 * Input:
 *      phyNo   - Physical port number (0~7)
 *      ocpAddr - OCP address
 *      ocpData - Writing data
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PHY_REG_ID       - invalid PHY address
 *      L2SW_ERR_PHY_ID           - invalid PHY no
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      None
 */
ret_t l2sw_setAsicPHYOCPReg(l2sw_uint8 units, l2sw_uint32 phyNo, l2sw_uint32 ocpAddr, l2sw_uint32 ocpData )
{
    ret_t retVal;
    l2sw_uint32 regData;
    l2sw_uint32 busyFlag, checkCounter;
    l2sw_uint32 ocpAddrPrefix, ocpAddr9_6, ocpAddr5_1;

    /*Check internal phy access busy or not*/
    /*retVal = l2sw_getAsicRegBit(units, L2SW_REG_INDRECT_ACCESS_STATUS, L2SW_INDRECT_ACCESS_STATUS_OFFSET,&busyFlag);*/
    retVal = l2sw_getAsicReg(units, L2SW_REG_INDRECT_ACCESS_STATUS,&busyFlag);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    if(busyFlag)
        return L2SW_ERR_BUSYWAIT_TIMEOUT;

    /* OCP prefix */
    ocpAddrPrefix = ((ocpAddr & 0xFC00) >> 10);

#if defined(USE_L2SW_SDK)
#else
    if((retVal = l2sw_setAsicRegBits(units, L2SW_REG_GPHY_OCP_MSB_0, L2SW_CFG_CPU_OCPADR_MSB_MASK, ocpAddrPrefix)) != L2SW_ERR_OK)
        return retVal;
#endif

    /*prepare access data*/
    retVal = l2sw_setAsicReg(units, L2SW_REG_INDRECT_ACCESS_WRITE_DATA, ocpData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /*prepare access address*/
    ocpAddr9_6 = ((ocpAddr >> 6) & 0x000F);
    ocpAddr5_1 = ((ocpAddr >> 1) & 0x001F);

#if defined(USE_L2SW_SDK)
    regData = (ocpAddr9_6 << 8) | (phyNo << L2SW_PHY_OFFSET) | ocpAddr5_1;
#else
    regData = L2SW_PHY_BASE | (ocpAddr9_6 << 8) | (phyNo << L2SW_PHY_OFFSET) | ocpAddr5_1;
#endif

    retVal = l2sw_setAsicReg(units, L2SW_REG_INDRECT_ACCESS_ADDRESS, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /*Set WRITE Command*/
    retVal = l2sw_setAsicReg(units, L2SW_REG_INDRECT_ACCESS_CTRL, L2SW_CMD_MASK | L2SW_RW_MASK);

    checkCounter = 100;
    while(checkCounter)
    {
        retVal = l2sw_getAsicReg(units, L2SW_REG_INDRECT_ACCESS_STATUS,&busyFlag);
        if((retVal != L2SW_ERR_OK) || busyFlag)
        {
            checkCounter --;
            if(0 == checkCounter)
                return L2SW_ERR_BUSYWAIT_TIMEOUT;
        }
        else
        {
            checkCounter = 0;
        }
    }

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicPHYOCPReg
 * Description:
 *      Get PHY OCP registers
 * Input:
 *      phyNo   - Physical port number (0~7)
 *      ocpAddr - PHY address
 *      pRegData - read data
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PHY_REG_ID       - invalid PHY address
 *      L2SW_ERR_PHY_ID           - invalid PHY no
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      None
 */
ret_t l2sw_getAsicPHYOCPReg(l2sw_uint8 units, l2sw_uint32 phyNo, l2sw_uint32 ocpAddr, l2sw_uint32 *pRegData )
{
    ret_t retVal;
    l2sw_uint32 regData;
    l2sw_uint32 busyFlag,checkCounter;
    l2sw_uint32 ocpAddrPrefix, ocpAddr9_6, ocpAddr5_1;
    /*Check internal phy access busy or not*/
    /*retVal = l2sw_getAsicRegBit(units, L2SW_REG_INDRECT_ACCESS_STATUS, L2SW_INDRECT_ACCESS_STATUS_OFFSET,&busyFlag);*/
    retVal = l2sw_getAsicReg(units, L2SW_REG_INDRECT_ACCESS_STATUS,&busyFlag);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    if(busyFlag)
        return L2SW_ERR_BUSYWAIT_TIMEOUT;

    /* OCP prefix */
    ocpAddrPrefix = ((ocpAddr & 0xFC00) >> 10);

#if defined(USE_L2SW_SDK)
#else
    if((retVal = l2sw_setAsicRegBits(units, L2SW_REG_GPHY_OCP_MSB_0, L2SW_CFG_CPU_OCPADR_MSB_MASK, ocpAddrPrefix)) != L2SW_ERR_OK)
        return retVal;
#endif

    /*prepare access address*/
    ocpAddr9_6 = ((ocpAddr >> 6) & 0x000F);
    ocpAddr5_1 = ((ocpAddr >> 1) & 0x001F);

#if defined(USE_L2SW_SDK)
    regData = (ocpAddr9_6 << 8) | (phyNo << L2SW_PHY_OFFSET) | ocpAddr5_1;
#else
    regData = L2SW_PHY_BASE | (ocpAddr9_6 << 8) | (phyNo << L2SW_PHY_OFFSET) | ocpAddr5_1;
#endif

    retVal = l2sw_setAsicReg(units, L2SW_REG_INDRECT_ACCESS_ADDRESS, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /*Set READ Command*/
    retVal = l2sw_setAsicReg(units, L2SW_REG_INDRECT_ACCESS_CTRL, L2SW_CMD_MASK );
    if(retVal != L2SW_ERR_OK)
        return retVal;

    checkCounter = 100;
    while(checkCounter)
    {
        retVal = l2sw_getAsicReg(units, L2SW_REG_INDRECT_ACCESS_STATUS,&busyFlag);
        if((retVal != L2SW_ERR_OK) || busyFlag)
        {
            checkCounter --;
            if(0 == checkCounter)
                return L2SW_ERR_FAILED;
        }
        else
        {
            checkCounter = 0;
        }
    }

    /*get PHY register*/
    retVal = l2sw_getAsicReg(units, L2SW_REG_INDRECT_ACCESS_READ_DATA, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *pRegData = regData;

    return L2SW_ERR_OK;
}

#endif

/* Function Name:
 *      l2sw_setAsicPHYReg
 * Description:
 *      Set PHY registers
 * Input:
 *      phyNo   - Physical port number (0~7)
 *      phyAddr - PHY address (0~31)
 *      phyData - Writing data
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PHY_REG_ID       - invalid PHY address
 *      L2SW_ERR_PHY_ID           - invalid PHY no
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      None
 */
ret_t l2sw_setAsicPHYReg(l2sw_uint8 units, l2sw_uint32 phyNo, l2sw_uint32 phyAddr, l2sw_uint32 phyData )
{
    l2sw_uint32 ocp_addr;

    if(phyAddr > L2SW_PHY_REGNOMAX)
        return L2SW_ERR_PHY_REG_ID;

    ocp_addr = 0xa400 + phyAddr*2;

    return l2sw_setAsicPHYOCPReg(units, phyNo, ocp_addr, phyData);
}
/* Function Name:
 *      l2sw_getAsicPHYReg
 * Description:
 *      Get PHY registers
 * Input:
 *      phyNo   - Physical port number (0~7)
 *      phyAddr - PHY address (0~31)
 *      pRegData - Writing data
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PHY_REG_ID       - invalid PHY address
 *      L2SW_ERR_PHY_ID           - invalid PHY no
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      None
 */
ret_t l2sw_getAsicPHYReg(l2sw_uint8 units, l2sw_uint32 phyNo, l2sw_uint32 phyAddr, l2sw_uint32 *pRegData )
{
    l2sw_uint32 ocp_addr;

    if(phyAddr > L2SW_PHY_REGNOMAX)
        return L2SW_ERR_PHY_REG_ID;

    ocp_addr = 0xa400 + phyAddr*2;

    return l2sw_getAsicPHYOCPReg(units, phyNo, ocp_addr, pRegData);
}

#if !defined(USE_L2SW_SDK)

#else
/* Function Name:
 *      l2sw_setAsicSdsReg
 * Description:
 *      Set Serdes registers
 * Input:
 *      sdsId   - sdsid (0~1)
 *      sdsReg - reg address (0~31)
 *      sdsPage - Writing data
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success

 * Note:
 *      None
 */

ret_t l2sw_setAsicSdsReg(l2sw_uint8 units, l2sw_uint32 sdsId, l2sw_uint32 sdsReg, l2sw_uint32 sdsPage,  l2sw_uint32 value)
{
    l2sw_uint32 retVal;

    if( (retVal = l2sw_setAsicReg(units, L2SW_REG_SDS_INDACS_DATA, value)) != L2SW_ERR_OK)
        return retVal;

    if( (retVal = l2sw_setAsicReg(units, L2SW_REG_SDS_INDACS_ADR, (sdsPage<<5) | sdsReg)) != L2SW_ERR_OK)
        return retVal;

    if( (retVal = l2sw_setAsicReg(units, L2SW_REG_SDS_INDACS_CMD, 0x00C0|sdsId)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;

}

/* Function Name:
 *      l2sw_getAiscSdsReg
 * Description:
 *      Get Serdes registers
 * Input:
 *      sdsId   - sdsid (0~1)
 *      sdsReg - reg address (0~31)
 *      sdsPage - Writing data
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success

 * Note:
 *      None
 */
ret_t l2sw_getAsicSdsReg(l2sw_uint8 units, l2sw_uint32 sdsId, l2sw_uint32 sdsReg, l2sw_uint32 sdsPage, l2sw_uint32 *value)
{
    l2sw_uint32 retVal, busy;

    if( (retVal = l2sw_setAsicReg(units, L2SW_REG_SDS_INDACS_ADR, (sdsPage<<5) | sdsReg)) != L2SW_ERR_OK)
        return retVal;

    if( (retVal = l2sw_setAsicReg(units, L2SW_REG_SDS_INDACS_CMD, 0x0080|sdsId)) != L2SW_ERR_OK)
        return retVal;

    while(1)
    {
        if ((retVal = l2sw_getAsicReg(units, L2SW_REG_SDS_INDACS_CMD, &busy))!=L2SW_ERR_OK)
            return retVal;

        if ((busy & 0x100) == 0)
            break;
    }

    if ((retVal = l2sw_getAsicReg(units, L2SW_REG_SDS_INDACS_DATA, value))!=L2SW_ERR_OK)
            return retVal;

    return L2SW_ERR_OK;
}


/* Function Name:
 *      l2sw_setAsicPHYSram
 * Description:
 *      Set PHY registers
 * Input:
 *      phyNo   - Physical port number (0~7)
 *      sramAddr - SRAM address
 *      sramData - Writing data
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PHY_REG_ID       - invalid PHY address
 *      L2SW_ERR_PHY_ID           - invalid PHY no
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      None
 */

ret_t l2sw_setAsicPHYSram(l2sw_uint8 units, l2sw_uint32 phyNo, l2sw_uint32 sramAddr, l2sw_uint32 sramData )
{
    l2sw_uint32 retVal;

    if ((retVal = l2sw_setAsicPHYOCPReg(units, phyNo, 0xa436, sramAddr)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicPHYOCPReg(units, phyNo, 0xa438, sramData)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
#endif


int sf_l2sw_setAsicPHYReg(unsigned int phyNo, unsigned int phyAddr, unsigned int pRegData)
{
	return l2sw_setAsicPHYReg(0, phyNo, phyAddr, pRegData);
}
int sf_l2sw_getAsicPHYReg(unsigned int phyNo, unsigned int phyAddr, unsigned int *pRegData)
{
	return l2sw_getAsicPHYReg(0, phyNo, phyAddr, pRegData);
}
