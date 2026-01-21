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

#include "l2sw_asicdrv_storm.h"
/* Function Name:
 *      l2sw_setAsicStormFilterBroadcastEnable
 * Description:
 *      Set per-port broadcast storm filter enable/disable
 * Input:
 *      port    - Physical port number (0~7)
 *      enabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterBroadcastEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_setAsicRegBit(units, L2SW_STORM_BCAST_REG, port, enabled);
}
/* Function Name:
 *      l2sw_getAsicStormFilterBroadcastEnable
 * Description:
 *      Get per-port broadcast storm filter enable/disable
 * Input:
 *      port    - Physical port number (0~7)
 *      pEnabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterBroadcastEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnabled)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBit(units, L2SW_STORM_BCAST_REG, port, pEnabled);
}
/* Function Name:
 *      l2sw_setAsicStormFilterBroadcastMeter
 * Description:
 *      Set per-port broadcast storm filter meter
 * Input:
 *      port    - Physical port number (0~7)
 *      meter   - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter index
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterBroadcastMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 meter)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    if(meter > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    return l2sw_setAsicRegBits(units, L2SW_STORM_BCAST_METER_CTRL_REG(port), L2SW_STORM_BCAST_METER_CTRL_MASK(port), meter);
}
/* Function Name:
 *      l2sw_getAsicStormFilterBroadcastMeter
 * Description:
 *      Get per-port broadcast storm filter meter
 * Input:
 *      port    - Physical port number (0~7)
 *      pMeter  - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterBroadcastMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pMeter)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBits(units, L2SW_STORM_BCAST_METER_CTRL_REG(port), L2SW_STORM_BCAST_METER_CTRL_MASK(port), pMeter);
}
/* Function Name:
 *      l2sw_setAsicStormFilterMulticastEnable
 * Description:
 *      Set per-port multicast storm filter enable/disable
 * Input:
 *      port    - Physical port number (0~7)
 *      enabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterMulticastEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_setAsicRegBit(units, L2SW_STORM_MCAST_REG, port, enabled);
}
/* Function Name:
 *      l2sw_getAsicStormFilterMulticastEnable
 * Description:
 *      Get per-port multicast storm filter enable/disable
 * Input:
 *      port    - Physical port number (0~7)
 *      pEnabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterMulticastEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnabled)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBit(units, L2SW_STORM_MCAST_REG, port, pEnabled);
}
/* Function Name:
 *      l2sw_setAsicStormFilterMulticastMeter
 * Description:
 *      Set per-port multicast storm filter meter
 * Input:
 *      port    - Physical port number (0~7)
 *      meter   - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter index
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterMulticastMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 meter)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    if(meter > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    return l2sw_setAsicRegBits(units, L2SW_STORM_MCAST_METER_CTRL_REG(port), L2SW_STORM_MCAST_METER_CTRL_MASK(port), meter);
}
/* Function Name:
 *      l2sw_getAsicStormFilterMulticastMeter
 * Description:
 *      Get per-port multicast storm filter meter
 * Input:
 *      port    - Physical port number (0~7)
 *      pMeter  - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterMulticastMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pMeter)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBits(units, L2SW_STORM_MCAST_METER_CTRL_REG(port), L2SW_STORM_MCAST_METER_CTRL_MASK(port), pMeter);
}
/* Function Name:
 *      l2sw_setAsicStormFilterUnknownMulticastEnable
 * Description:
 *      Set per-port unknown multicast storm filter enable/disable
 * Input:
 *      port    - Physical port number (0~7)
 *      enabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterUnknownMulticastEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_setAsicRegBit(units, L2SW_STORM_UNKNOWN_MCAST_REG, port, enabled);
}
/* Function Name:
 *      l2sw_getAsicStormFilterUnknownMulticastEnable
 * Description:
 *      Get per-port unknown multicast storm filter enable/disable
 * Input:
 *      port    - Physical port number (0~7)
 *      pEnabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterUnknownMulticastEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnabled)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBit(units, L2SW_STORM_UNKNOWN_MCAST_REG, port, pEnabled);
}
/* Function Name:
 *      l2sw_setAsicStormFilterUnknownMulticastMeter
 * Description:
 *      Set per-port unknown multicast storm filter meter
 * Input:
 *      port    - Physical port number (0~7)
 *      meter   - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter index
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterUnknownMulticastMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 meter)
{
    ret_t retVal;

    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    if(meter > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    if(port < 8)
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_STORM_UNMC_METER_CTRL_REG(port), L2SW_STORM_UNMC_METER_CTRL_MASK(port), meter);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_STORM_UNMC_METER_CTRL4 + ((port - 8) >> 1), L2SW_STORM_UNMC_METER_CTRL_MASK(port), meter);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicStormFilterUnknownMulticastMeter
 * Description:
 *      Get per-port unknown multicast storm filter meter
 * Input:
 *      port    - Physical port number (0~7)
 *      pMeter  - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterUnknownMulticastMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pMeter)
{
    ret_t retVal;

    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    if(port < 8)
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_STORM_UNMC_METER_CTRL_REG(port), L2SW_STORM_UNMC_METER_CTRL_MASK(port), pMeter);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_STORM_UNMC_METER_CTRL4 + ((port - 8) >> 1), L2SW_STORM_UNMC_METER_CTRL_MASK(port), pMeter);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicStormFilterUnknownUnicastEnable
 * Description:
 *      Set per-port unknown unicast storm filter enable/disable
 * Input:
 *      port    - Physical port number (0~7)
 *      enabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterUnknownUnicastEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_setAsicRegBit(units, L2SW_STORM_UNKNOWN_UCAST_REG, port, enabled);
}
/* Function Name:
 *      l2sw_getAsicStormFilterUnknownUnicastEnable
 * Description:
 *      get per-port unknown unicast storm filter enable/disable
 * Input:
 *      port    - Physical port number (0~7)
 *      pEnabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterUnknownUnicastEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnabled)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBit(units, L2SW_STORM_UNKNOWN_UCAST_REG, port, pEnabled);
}
/* Function Name:
 *      l2sw_setAsicStormFilterUnknownUnicastMeter
 * Description:
 *      Set per-port unknown unicast storm filter meter
 * Input:
 *      port    - Physical port number (0~7)
 *      meter   - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter index
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterUnknownUnicastMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 meter)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    if(meter > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    return l2sw_setAsicRegBits(units, L2SW_STORM_UNDA_METER_CTRL_REG(port), L2SW_STORM_UNDA_METER_CTRL_MASK(port), meter);
}
/* Function Name:
 *      l2sw_getAsicStormFilterUnknownUnicastMeter
 * Description:
 *      Get per-port unknown unicast storm filter meter
 * Input:
 *      port    - Physical port number (0~7)
 *      pMeter  - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterUnknownUnicastMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pMeter)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBits(units, L2SW_STORM_UNDA_METER_CTRL_REG(port), L2SW_STORM_UNDA_METER_CTRL_MASK(port), pMeter);
}

/* Function Name:
 *      l2sw_setAsicStormFilterExtBroadcastMeter
 * Description:
 *      Set extension broadcast storm filter meter
 * Input:
 *      meter   - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter index
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterExtBroadcastMeter(l2sw_uint8 units, l2sw_uint32 meter)
{
    if(meter > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    return l2sw_setAsicRegBits(units, L2SW_REG_STORM_EXT_MTRIDX_CFG0, L2SW_BC_STORM_EXT_METERIDX_MASK, meter);
}

/* Function Name:
 *      l2sw_getAsicStormFilterExtBroadcastMeter
 * Description:
 *      get extension broadcast storm filter meter
 * Input:
 *      None
 * Output:
 *      pMeter  - meter index (0~31)
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_NULL_POINTER     - Invalid meter index
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterExtBroadcastMeter(l2sw_uint8 units, l2sw_uint32 *pMeter)
{
    if(NULL == pMeter)
        return L2SW_ERR_NULL_POINTER;

    return l2sw_getAsicRegBits(units, L2SW_REG_STORM_EXT_MTRIDX_CFG0, L2SW_BC_STORM_EXT_METERIDX_MASK, pMeter);
}

/* Function Name:
 *      l2sw_setAsicStormFilterExtMulticastMeter
 * Description:
 *      Set extension multicast storm filter meter
 * Input:
 *      meter   - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter index
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterExtMulticastMeter(l2sw_uint8 units, l2sw_uint32 meter)
{
    if(meter > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    return l2sw_setAsicRegBits(units, L2SW_REG_STORM_EXT_MTRIDX_CFG0, L2SW_MC_STORM_EXT_METERIDX_MASK, meter);
}

/* Function Name:
 *      l2sw_getAsicStormFilterExtMulticastMeter
 * Description:
 *      get extension multicast storm filter meter
 * Input:
 *      None
 * Output:
 *      pMeter  - meter index (0~31)
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_NULL_POINTER     - Invalid meter index
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterExtMulticastMeter(l2sw_uint8 units, l2sw_uint32 *pMeter)
{
    if(NULL == pMeter)
        return L2SW_ERR_NULL_POINTER;

    return l2sw_getAsicRegBits(units, L2SW_REG_STORM_EXT_MTRIDX_CFG0, L2SW_MC_STORM_EXT_METERIDX_MASK, pMeter);
}

/* Function Name:
 *      l2sw_setAsicStormFilterExtUnknownMulticastMeter
 * Description:
 *      Set extension unknown multicast storm filter meter
 * Input:
 *      meter   - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter index
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterExtUnknownMulticastMeter(l2sw_uint8 units, l2sw_uint32 meter)
{
    if(meter > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    return l2sw_setAsicRegBits(units, L2SW_REG_STORM_EXT_MTRIDX_CFG1, L2SW_UNMC_STORM_EXT_METERIDX_MASK, meter);
}

/* Function Name:
 *      l2sw_getAsicStormFilterExtUnknownMulticastMeter
 * Description:
 *      get extension unknown multicast storm filter meter
 * Input:
 *      None
 * Output:
 *      pMeter  - meter index (0~31)
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_NULL_POINTER     - Invalid meter index
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterExtUnknownMulticastMeter(l2sw_uint8 units, l2sw_uint32 *pMeter)
{
    if(NULL == pMeter)
        return L2SW_ERR_NULL_POINTER;

    return l2sw_getAsicRegBits(units, L2SW_REG_STORM_EXT_MTRIDX_CFG1, L2SW_UNMC_STORM_EXT_METERIDX_MASK, pMeter);
}

/* Function Name:
 *      l2sw_setAsicStormFilterExtUnknownUnicastMeter
 * Description:
 *      Set extension unknown unicast storm filter meter
 * Input:
 *      meter   - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter index
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterExtUnknownUnicastMeter(l2sw_uint8 units, l2sw_uint32 meter)
{
    if(meter > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    return l2sw_setAsicRegBits(units, L2SW_REG_STORM_EXT_MTRIDX_CFG1, L2SW_UNUC_STORM_EXT_METERIDX_MASK, meter);
}

/* Function Name:
 *      l2sw_getAsicStormFilterExtUnknownUnicastMeter
 * Description:
 *      get extension unknown unicast storm filter meter
 * Input:
 *      None
 * Output:
 *      pMeter  - meter index (0~31)
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_NULL_POINTER     - Invalid meter index
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterExtUnknownUnicastMeter(l2sw_uint8 units, l2sw_uint32 *pMeter)
{
    if(NULL == pMeter)
        return L2SW_ERR_NULL_POINTER;

    return l2sw_getAsicRegBits(units, L2SW_REG_STORM_EXT_MTRIDX_CFG1, L2SW_UNUC_STORM_EXT_METERIDX_MASK, pMeter);
}

/* Function Name:
 *      l2sw_setAsicStormFilterExtBroadcastEnable
 * Description:
 *      Set extension broadcast storm filter state
 * Input:
 *      enabled     - state
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterExtBroadcastEnable(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_STORM_EXT_CFG, L2SW_STORM_BCAST_EXT_EN_OFFSET, enabled);
}

/* Function Name:
 *      l2sw_getAsicStormFilterExtBroadcastEnable
 * Description:
 *      Get extension broadcast storm filter state
 * Input:
 *      None
 * Output:
 *      pEnabled    - state
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_NULL_POINTER     - Null pointer
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterExtBroadcastEnable(l2sw_uint8 units, l2sw_uint32 *pEnabled)
{
    if(NULL == pEnabled)
        return L2SW_ERR_NULL_POINTER;

    return l2sw_getAsicRegBit(units, L2SW_REG_STORM_EXT_CFG, L2SW_STORM_BCAST_EXT_EN_OFFSET, pEnabled);
}

/* Function Name:
 *      l2sw_setAsicStormFilterExtMulticastEnable
 * Description:
 *      Set extension multicast storm filter state
 * Input:
 *      enabled     - state
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterExtMulticastEnable(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_STORM_EXT_CFG, L2SW_STORM_MCAST_EXT_EN_OFFSET, enabled);
}

/* Function Name:
 *      l2sw_getAsicStormFilterExtMulticastEnable
 * Description:
 *      Get extension multicast storm filter state
 * Input:
 *      None
 * Output:
 *      pEnabled    - state
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_NULL_POINTER     - Null pointer
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterExtMulticastEnable(l2sw_uint8 units, l2sw_uint32 *pEnabled)
{
    if(NULL == pEnabled)
        return L2SW_ERR_NULL_POINTER;

    return l2sw_getAsicRegBit(units, L2SW_REG_STORM_EXT_CFG, L2SW_STORM_MCAST_EXT_EN_OFFSET, pEnabled);
}

/* Function Name:
 *      l2sw_setAsicStormFilterExtUnknownMulticastEnable
 * Description:
 *      Set extension unknown multicast storm filter state
 * Input:
 *      enabled     - state
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterExtUnknownMulticastEnable(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_STORM_EXT_CFG, L2SW_STORM_UNKNOWN_MCAST_EXT_EN_OFFSET, enabled);
}

/* Function Name:
 *      l2sw_getAsicStormFilterExtUnknownMulticastEnable
 * Description:
 *      Get extension unknown multicast storm filter state
 * Input:
 *      None
 * Output:
 *      pEnabled    - state
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_NULL_POINTER     - Null pointer
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterExtUnknownMulticastEnable(l2sw_uint8 units, l2sw_uint32 *pEnabled)
{
    if(NULL == pEnabled)
        return L2SW_ERR_NULL_POINTER;

    return l2sw_getAsicRegBit(units, L2SW_REG_STORM_EXT_CFG, L2SW_STORM_UNKNOWN_MCAST_EXT_EN_OFFSET, pEnabled);
}

/* Function Name:
 *      l2sw_setAsicStormFilterExtUnknownUnicastEnable
 * Description:
 *      Set extension unknown unicast storm filter state
 * Input:
 *      enabled     - state
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterExtUnknownUnicastEnable(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_STORM_EXT_CFG, L2SW_STORM_UNKNOWN_UCAST_EXT_EN_OFFSET, enabled);
}

/* Function Name:
 *      l2sw_getAsicStormFilterExtUnknownUnicastEnable
 * Description:
 *      Get extension unknown unicast storm filter state
 * Input:
 *      None
 * Output:
 *      pEnabled    - state
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_NULL_POINTER     - Null pointer
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterExtUnknownUnicastEnable(l2sw_uint8 units, l2sw_uint32 *pEnabled)
{
    if(NULL == pEnabled)
        return L2SW_ERR_NULL_POINTER;

    return l2sw_getAsicRegBit(units, L2SW_REG_STORM_EXT_CFG, L2SW_STORM_UNKNOWN_UCAST_EXT_EN_OFFSET, pEnabled);
}

/* Function Name:
 *      l2sw_setAsicStormFilterExtEnablePortMask
 * Description:
 *      Set extension storm filter port mask
 * Input:
 *      portmask    - port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterExtEnablePortMask(l2sw_uint8 units, l2sw_uint32 portmask)
{
    ret_t retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_STORM_EXT_CFG, L2SW_STORM_EXT_EN_PORTMASK_MASK, portmask & 0x3FF);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_STORM_EXT_CFG, L2SW_STORM_EXT_EN_PORTMASK_EXT_MASK, (portmask >> 10)&1);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicStormFilterExtEnablePortMask
 * Description:
 *      Get extension storm filter port mask
 * Input:
 *      portmask    - port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_NULL_POINTER     - Null pointer
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterExtEnablePortMask(l2sw_uint8 units, l2sw_uint32 *pPortmask)
{
    l2sw_uint32 tmpPmsk;
    ret_t retVal;

    if(NULL == pPortmask)
        return L2SW_ERR_NULL_POINTER;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_STORM_EXT_CFG, L2SW_STORM_EXT_EN_PORTMASK_MASK, &tmpPmsk);
    if(retVal != L2SW_ERR_OK)
        return retVal;
    *pPortmask = tmpPmsk & 0x3ff;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_STORM_EXT_CFG, L2SW_STORM_EXT_EN_PORTMASK_EXT_MASK, &tmpPmsk);
    if(retVal != L2SW_ERR_OK)
        return retVal;
    *pPortmask |= (tmpPmsk & 1) << 10;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicStormFilterEnable
 * Description:
 *      Set storm filter enable/disable
 * Input:
 *      enabled     - 1:enabled, 0:disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicStormFilterEnable(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_STORM_EXT_CFG, L2SW_STORM_EXT_EN_OFFSET, enabled);
}

/* Function Name:
 *      l2sw_getAsicStormFilterEnable
 * Description:
 *      Get storm filter enable/disable
 * Input:
 *      None
 * Output:
 *      pEnabled    - state
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicStormFilterEnable(l2sw_uint8 units, l2sw_uint32 *pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_STORM_EXT_CFG, L2SW_STORM_EXT_EN_OFFSET, pEnabled);
}

