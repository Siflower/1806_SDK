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

#include "l2sw_asicdrv_misc.h"
/* Function Name:
 *      l2sw_setAsicMacAddress
 * Description:
 *      Set switch MAC address
 * Input:
 *      mac     - switch mac
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicMacAddress(l2sw_uint8 units, ether_addr_t mac)
{
    ret_t retVal;
    l2sw_uint32 regData;
    l2sw_uint8 *accessPtr;
    l2sw_uint32 i;

    accessPtr =  (l2sw_uint8*)&mac;

    for(i = 0; i <=2; i++)
    {
        regData = (*(accessPtr + (i*2)) << 8) | *(accessPtr + (i*2) + 1);
        retVal = l2sw_setAsicReg(units, L2SW_REG_SWITCH_MAC2 - i, regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicMacAddress
 * Description:
 *      Get switch MAC address
 * Input:
 *      pMac    - switch mac
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicMacAddress(l2sw_uint8 units, ether_addr_t *pMac)
{
    ret_t retVal;
    l2sw_uint32 regData;
    l2sw_uint8 *accessPtr;
    l2sw_uint32 i;


    accessPtr = (l2sw_uint8*)pMac;

    for(i = 0; i <= 2; i++)
    {
        retVal = l2sw_getAsicReg(units, L2SW_REG_SWITCH_MAC2 - i, &regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        *accessPtr = (regData & 0xFF00) >> 8;
        accessPtr ++;
        *accessPtr = regData & 0xFF;
        accessPtr ++;
    }

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicDebugInfo
 * Description:
 *      Get per-port packet forward debugging information
 * Input:
 *      port        - Physical port number (0~7)
 *      pDebugifo   - per-port packet trap/drop/forward reason
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicDebugInfo(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pDebugifo)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBits(units, L2SW_DEBUG_INFO_REG(port), L2SW_DEBUG_INFO_MASK(port), pDebugifo);
}
/* Function Name:
 *      l2sw_setAsicPortJamMode
 * Description:
 *      Set half duplex flow control setting
 * Input:
 *      mode    - 0: Back-Pressure 1: DEFER
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortJamMode(l2sw_uint8 units, l2sw_uint32 mode)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_CFG_BACKPRESSURE, L2SW_LONGTXE_OFFSET,mode);
}
/* Function Name:
 *      l2sw_getAsicPortJamMode
 * Description:
 *      Get half duplex flow control setting
 * Input:
 *      pMode   - 0: Back-Pressure 1: DEFER
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortJamMode(l2sw_uint8 units, l2sw_uint32* pMode)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_CFG_BACKPRESSURE, L2SW_LONGTXE_OFFSET, pMode);
}

#if !defined(USE_L2SW_SDK)

#else
/* Function Name:
 *      l2sw_setAsicMaxLengthCfg
 * Description:
 *      Set Max packet length configuration
 * Input:
 *      cfgId       - Configuration ID
 *      maxLength   - Max Length
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicMaxLengthCfg(l2sw_uint8 units, l2sw_uint32 cfgId, l2sw_uint32 maxLength)
{
    return l2sw_setAsicRegBits(units, L2SW_REG_MAX_LEN_RX_TX_CFG0 + cfgId, L2SW_MAX_LEN_RX_TX_CFG0_MASK, maxLength);
}

/* Function Name:
 *      l2sw_getAsicMaxLengthCfg
 * Description:
 *      Get Max packet length configuration
 * Input:
 *      cfgId       - Configuration ID
 *      maxLength   - Max Length
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicMaxLengthCfg(l2sw_uint8 units, l2sw_uint32 cfgId, l2sw_uint32 *pMaxLength)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_MAX_LEN_RX_TX_CFG0 + cfgId, L2SW_MAX_LEN_RX_TX_CFG0_MASK, pMaxLength);
}

/* Function Name:
 *      l2sw_setAsicMaxLength
 * Description:
 *      Set Max packet length
 * Input:
 *      port        - port ID
 *      type        - 0: 10M/100M speed, 1: giga speed
 *      cfgId       - Configuration ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicMaxLength(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 type, l2sw_uint32 cfgId)
{
    ret_t retVal;

    if(port < 8)
    {
        retVal = l2sw_setAsicRegBit(units, L2SW_REG_MAX_LENGTH_CFG, (type * 8) + port, cfgId);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_setAsicRegBit(units, L2SW_REG_MAX_LENGTH_CFG_EXT, (type * 3) + port - 8, cfgId);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicMaxLength
 * Description:
 *      Get Max packet length
 * Input:
 *      port        - port ID
 *      type        - 0: 10M/100M speed, 1: giga speed
 *      cfgId       - Configuration ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicMaxLength(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 type, l2sw_uint32 *pCfgId)
{
    ret_t retVal;

    if(port < 8)
    {
        retVal = l2sw_getAsicRegBit(units, L2SW_REG_MAX_LENGTH_CFG, (type * 8) + port, pCfgId);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_getAsicRegBit(units, L2SW_REG_MAX_LENGTH_CFG_EXT, (type * 3) + port - 8, pCfgId);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    return L2SW_ERR_OK;
}
#endif