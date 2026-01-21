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

#include "l2sw_asicdrv_dot1x.h"
/* Function Name:
 *      l2sw_setAsic1xPBEnConfig
 * Description:
 *      Set 802.1x port-based port enable configuration
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
ret_t l2sw_setAsic1xPBEnConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_setAsicRegBit(units, L2SW_DOT1X_PORT_ENABLE_REG, port, enabled);
}
/* Function Name:
 *      l2sw_getAsic1xPBEnConfig
 * Description:
 *      Get 802.1x port-based port enable configuration
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
ret_t l2sw_getAsic1xPBEnConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnabled)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBit(units, L2SW_DOT1X_PORT_ENABLE_REG, port, pEnabled);
}
/* Function Name:
 *      l2sw_setAsic1xPBAuthConfig
 * Description:
 *      Set 802.1x port-based authorised port configuration
 * Input:
 *      port    - Physical port number (0~7)
 *      auth    - 1: authorised, 0: non-authorised
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsic1xPBAuthConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 auth)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_setAsicRegBit(units, L2SW_DOT1X_PORT_AUTH_REG, port, auth);
}
/* Function Name:
 *      l2sw_getAsic1xPBAuthConfig
 * Description:
 *      Get 802.1x port-based authorised port configuration
 * Input:
 *      port    - Physical port number (0~7)
 *      pAuth   - 1: authorised, 0: non-authorised
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsic1xPBAuthConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pAuth)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBit(units, L2SW_DOT1X_PORT_AUTH_REG, port, pAuth);
}
/* Function Name:
 *      l2sw_setAsic1xPBOpdirConfig
 * Description:
 *      Set 802.1x port-based operational direction
 * Input:
 *      port    - Physical port number (0~7)
 *      opdir   - Operation direction 1: IN, 0:BOTH
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsic1xPBOpdirConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 opdir)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_setAsicRegBit(units, L2SW_DOT1X_PORT_OPDIR_REG, port, opdir);
}
/* Function Name:
 *      l2sw_getAsic1xPBOpdirConfig
 * Description:
 *      Get 802.1x port-based operational direction
 * Input:
 *      port    - Physical port number (0~7)
 *      pOpdir  - Operation direction 1: IN, 0:BOTH
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsic1xPBOpdirConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pOpdir)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBit(units, L2SW_DOT1X_PORT_OPDIR_REG, port, pOpdir);
}
/* Function Name:
 *      l2sw_setAsic1xMBEnConfig
 * Description:
 *      Set 802.1x mac-based port enable configuration
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
ret_t l2sw_setAsic1xMBEnConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_setAsicRegBit(units, L2SW_DOT1X_MAC_ENABLE_REG, port, enabled);
}
/* Function Name:
 *      l2sw_getAsic1xMBEnConfig
 * Description:
 *      Get 802.1x mac-based port enable configuration
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
ret_t l2sw_getAsic1xMBEnConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnabled)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBit(units, L2SW_DOT1X_MAC_ENABLE_REG, port, pEnabled);
}
/* Function Name:
 *      l2sw_setAsic1xMBOpdirConfig
 * Description:
 *      Set 802.1x mac-based operational direction
 * Input:
 *      opdir       - Operation direction 1: IN, 0:BOTH
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsic1xMBOpdirConfig(l2sw_uint8 units, l2sw_uint32 opdir)
{
    return l2sw_setAsicRegBit(units, L2SW_DOT1X_CFG_REG, L2SW_DOT1X_MAC_OPDIR_OFFSET, opdir);
}
/* Function Name:
 *      l2sw_getAsic1xMBOpdirConfig
 * Description:
 *      Get 802.1x mac-based operational direction
 * Input:
 *      pOpdir      - Operation direction 1: IN, 0:BOTH
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsic1xMBOpdirConfig(l2sw_uint8 units, l2sw_uint32 *pOpdir)
{
    return l2sw_getAsicRegBit(units, L2SW_DOT1X_CFG_REG, L2SW_DOT1X_MAC_OPDIR_OFFSET, pOpdir);
}
/* Function Name:
 *      l2sw_setAsic1xProcConfig
 * Description:
 *      Set 802.1x unauth. behavior configuration
 * Input:
 *      port    - Physical port number (0~7)
 *      proc    - 802.1x unauth. behavior configuration 0:drop 1:trap to CPU 2:Guest VLAN
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_DOT1X_PROC   - Unauthorized behavior error
 * Note:
 *      None
 */
ret_t l2sw_setAsic1xProcConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 proc)
{

    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    if(proc >= DOT1X_UNAUTH_END)
        return L2SW_ERR_DOT1X_PROC;

    if(port < 8)
    {
        return l2sw_setAsicRegBits(units, L2SW_DOT1X_UNAUTH_ACT_BASE, L2SW_DOT1X_UNAUTH_ACT_MASK(port),proc);
    }
    else
    {
        return l2sw_setAsicRegBits(units, L2SW_REG_DOT1X_UNAUTH_ACT_W1, L2SW_DOT1X_UNAUTH_ACT_MASK(port),proc);
    }
}
/* Function Name:
 *      l2sw_getAsic1xProcConfig
 * Description:
 *      Get 802.1x unauth. behavior configuration
 * Input:
 *      port    - Physical port number (0~7)
 *      pProc   - 802.1x unauth. behavior configuration 0:drop 1:trap to CPU 2:Guest VLAN
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsic1xProcConfig(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pProc)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    if(port < 8)
    return l2sw_getAsicRegBits(units, L2SW_DOT1X_UNAUTH_ACT_BASE, L2SW_DOT1X_UNAUTH_ACT_MASK(port),pProc);
    else
        return l2sw_getAsicRegBits(units, L2SW_REG_DOT1X_UNAUTH_ACT_W1, L2SW_DOT1X_UNAUTH_ACT_MASK(port),pProc);
}
/* Function Name:
 *      l2sw_setAsic1xGuestVidx
 * Description:
 *      Set 802.1x guest vlan index
 * Input:
 *      index   - 802.1x guest vlan index (0~31)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_DOT1X_GVLANIDX   - Invalid cvid index
 * Note:
 *      None
 */
ret_t l2sw_setAsic1xGuestVidx(l2sw_uint8 units, l2sw_uint32 index)
{
    if(index >= L2SW_CVIDXNO)
        return L2SW_ERR_DOT1X_GVLANIDX;

    return l2sw_setAsicRegBits(units, L2SW_DOT1X_CFG_REG, L2SW_DOT1X_GVIDX_MASK, index);
}
/* Function Name:
 *      l2sw_getAsic1xGuestVidx
 * Description:
 *      Get 802.1x guest vlan index
 * Input:
 *      pIndex  - 802.1x guest vlan index (0~31)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsic1xGuestVidx(l2sw_uint8 units, l2sw_uint32 *pIndex)
{
    return l2sw_getAsicRegBits(units, L2SW_DOT1X_CFG_REG, L2SW_DOT1X_GVIDX_MASK, pIndex);
}
/* Function Name:
 *      l2sw_setAsic1xGVOpdir
 * Description:
 *      Set 802.1x guest vlan talk to auth. DA
 * Input:
 *      enabled     - 0:disable 1:enable
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsic1xGVOpdir(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_DOT1X_CFG_REG, L2SW_DOT1X_GVOPDIR_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsic1xGVOpdir
 * Description:
 *      Get 802.1x guest vlan talk to auth. DA
 * Input:
 *      pEnabled        - 0:disable 1:enable
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsic1xGVOpdir(l2sw_uint8 units, l2sw_uint32 *pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_DOT1X_CFG_REG, L2SW_DOT1X_GVOPDIR_OFFSET, pEnabled);
}
/* Function Name:
 *      l2sw_setAsic1xTrapPriority
 * Description:
 *      Set 802.1x Trap priority
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
ret_t l2sw_setAsic1xTrapPriority(l2sw_uint8 units, l2sw_uint32 priority)
{
    if(priority > L2SW_PRIMAX)
        return L2SW_ERR_QOS_INT_PRIORITY;

    return l2sw_setAsicRegBits(units, L2SW_REG_QOS_TRAP_PRIORITY0, L2SW_DOT1X_PRIORTY_MASK,priority);
}
/* Function Name:
 *      l2sw_getAsic1xTrapPriority
 * Description:
 *      Get 802.1x Trap priority
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
ret_t l2sw_getAsic1xTrapPriority(l2sw_uint8 units, l2sw_uint32 *pPriority)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_QOS_TRAP_PRIORITY0, L2SW_DOT1X_PRIORTY_MASK, pPriority);
}

