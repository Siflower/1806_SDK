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

#include "l2sw_asicdrv_unknownMulticast.h"
/* Function Name:
 *      l2sw_setAsicUnknownL2MulticastBehavior
 * Description:
 *      Set behavior of L2 multicast
 * Input:
 *      port    - Physical port number (0~7)
 *      behave  - 0: flooding, 1: drop, 2: trap
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_NOT_ALLOWED  - Invalid operation
 * Note:
 *      None
 */
ret_t l2sw_setAsicUnknownL2MulticastBehavior(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 behave)
{
    ret_t retVal;

    if(port >  L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(behave >= L2_UNKOWN_MULTICAST_END)
        return L2SW_ERR_NOT_ALLOWED;
    if(port < 8)
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_UNKNOWN_L2_MULTICAST_REG(port), L2SW_UNKNOWN_L2_MULTICAST_MASK(port), behave);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_UNKNOWN_L2_MULTICAST_CTRL1, 3 << ((port - 8) << 1), behave);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicUnknownL2MulticastBehavior
 * Description:
 *      Get behavior of L2 multicast
 * Input:
 *      port    - Physical port number (0~7)
 *      pBehave     - 0: flooding, 1: drop, 2: trap
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicUnknownL2MulticastBehavior(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pBehave)
{
    ret_t retVal;

    if(port >  L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 8)
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_UNKNOWN_L2_MULTICAST_REG(port), L2SW_UNKNOWN_L2_MULTICAST_MASK(port), pBehave);
        if (retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_UNKNOWN_L2_MULTICAST_CTRL1, 3 << ((port - 8) << 1), pBehave);
        if (retVal != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicUnknownIPv4MulticastBehavior
 * Description:
 *      Set behavior of IPv4 multicast
 * Input:
 *      port    - Physical port number (0~7)
 *      behave  - 0: flooding, 1: drop, 2: trap
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_NOT_ALLOWED  - Invalid operation
 * Note:
 *      None
 */
ret_t l2sw_setAsicUnknownIPv4MulticastBehavior(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 behave)
{
    if(port >  L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(behave >= L3_UNKOWN_MULTICAST_END)
        return L2SW_ERR_NOT_ALLOWED;

    return l2sw_setAsicRegBits(units, L2SW_UNKNOWN_IPV4_MULTICAST_REG(port), L2SW_UNKNOWN_IPV4_MULTICAST_MASK(port), behave);
}
/* Function Name:
 *      l2sw_getAsicUnknownIPv4MulticastBehavior
 * Description:
 *      Get behavior of IPv4 multicast
 * Input:
 *      port    - Physical port number (0~7)
 *      pBehave     - 0: flooding, 1: drop, 2: trap
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicUnknownIPv4MulticastBehavior(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pBehave)
{
    if(port >  L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBits(units, L2SW_UNKNOWN_IPV4_MULTICAST_REG(port), L2SW_UNKNOWN_IPV4_MULTICAST_MASK(port), pBehave);
}
/* Function Name:
 *      l2sw_setAsicUnknownIPv6MulticastBehavior
 * Description:
 *      Set behavior of IPv6 multicast
 * Input:
 *      port    - Physical port number (0~7)
 *      behave  - 0: flooding, 1: drop, 2: trap
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_NOT_ALLOWED  - Invalid operation
 * Note:
 *      None
 */
ret_t l2sw_setAsicUnknownIPv6MulticastBehavior(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 behave)
{
    if(port >  L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(behave >= L3_UNKOWN_MULTICAST_END)
        return L2SW_ERR_NOT_ALLOWED;

    return l2sw_setAsicRegBits(units, L2SW_UNKNOWN_IPV6_MULTICAST_REG(port), L2SW_UNKNOWN_IPV6_MULTICAST_MASK(port), behave);
}
/* Function Name:
 *      l2sw_getAsicUnknownIPv6MulticastBehavior
 * Description:
 *      Get behavior of IPv6 multicast
 * Input:
 *      port    - Physical port number (0~7)
 *      pBehave     - 0: flooding, 1: drop, 2: trap
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicUnknownIPv6MulticastBehavior(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pBehave)
{
    if(port >  L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBits(units, L2SW_UNKNOWN_IPV6_MULTICAST_REG(port), L2SW_UNKNOWN_IPV6_MULTICAST_MASK(port), pBehave);
}
/* Function Name:
 *      l2sw_setAsicUnknownMulticastTrapPriority
 * Description:
 *      Set trap priority of unknown multicast frame
 * Input:
 *      priority    - priority (0~7)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority
 * Note:
 *      None
 */
ret_t l2sw_setAsicUnknownMulticastTrapPriority(l2sw_uint8 units, l2sw_uint32 priority)
{
    if(priority > L2SW_PRIMAX)
        return L2SW_ERR_QOS_INT_PRIORITY;

    return l2sw_setAsicRegBits(units, L2SW_QOS_TRAP_PRIORITY_CTRL0_REG, L2SW_UNKNOWN_MC_PRIORTY_MASK, priority);
}
/* Function Name:
 *      l2sw_getAsicUnknownMulticastTrapPriority
 * Description:
 *      Get trap priority of unknown multicast frame
 * Input:
 *      pPriority   - priority (0~7)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicUnknownMulticastTrapPriority(l2sw_uint8 units, l2sw_uint32 *pPriority)
{
    return l2sw_getAsicRegBits(units, L2SW_QOS_TRAP_PRIORITY_CTRL0_REG, L2SW_UNKNOWN_MC_PRIORTY_MASK, pPriority);
}
