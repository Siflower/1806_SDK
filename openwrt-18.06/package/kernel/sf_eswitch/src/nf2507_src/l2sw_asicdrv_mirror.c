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

#include "l2sw_asicdrv_mirror.h"
#define L2SW_MIRROR_RX_MAGIC_NUM_VALUE 0x7
#define L2SW_MIRROR_TX_MAGIC_NUM_VALUE 0xE

/* Function Name:
 *      l2sw_setAsicPortMirror
 * Description:
 *      Set port mirror function
 * Input:
 *      source  - Source port
 *      monitor - Monitor (destination) port
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortMirror(l2sw_uint8 units, l2sw_uint32 source, l2sw_uint32 monitor)
{

    if((source > L2SW_PORTIDMAX) || (monitor > L2SW_PORTIDMAX))
        return L2SW_ERR_PORT_ID;

    return l2sw_setAsicRegBits(units, L2SW_MIRROR_CTRL_REG, L2SW_MIRROR_MONITOR_PORT_MASK, monitor);
}
/* Function Name:
 *      l2sw_getAsicPortMirror
 * Description:
 *      Get port mirror function
 * Input:
 *      pSource     - Source port
 *      pMonitor - Monitor (destination) port
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortMirror(l2sw_uint8 units, l2sw_uint32 *pSource, l2sw_uint32 *pMonitor)
{  
    *pSource = 0X0;
    return l2sw_getAsicRegBits(units, L2SW_MIRROR_CTRL_REG, L2SW_MIRROR_MONITOR_PORT_MASK, pMonitor);
}
/* Function Name:
 *      l2sw_setAsicPortMirrorRxFunction
 * Description:
 *      Set the mirror function on RX of the mirrored
 * Input:
 *      enabled     - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortMirrorRxFunction(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_MIRROR_CTRL_REG, L2SW_MIRROR_RX_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicPortMirrorRxFunction
 * Description:
 *      Get the mirror function on RX of the mirrored
 * Input:
 *      pEnabled    - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortMirrorRxFunction(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_MIRROR_CTRL_REG, L2SW_MIRROR_RX_OFFSET, pEnabled);
}

/* Function Name:
 *      l2sw_setAsicPortMirrorTxFunction
 * Description:
 *      Set the mirror function on TX of the mirrored
 * Input:
 *      enabled     - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortMirrorTxFunction(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_MIRROR_CTRL_REG, L2SW_MIRROR_TX_OFFSET, enabled);
}

/* Function Name:
 *      l2sw_getAsicPortMirrorTxFunction
 * Description:
 *      Get the mirror function on TX of the mirrored
 * Input:
 *      pEnabled    - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortMirrorTxFunction(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_MIRROR_CTRL_REG, L2SW_MIRROR_TX_OFFSET, pEnabled);
}

/* Function Name:
 *      l2sw_setAsicPortMirrorMask
 * Description:
 *      Set mirror source port mask
 * Input:
 *      SourcePortmask  - Source Portmask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_MASK- Port Mask Error
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortMirrorMask(l2sw_uint8 units, l2sw_uint32 SourcePortmask)
{
    if( SourcePortmask > L2SW_PORTMASK)
        return L2SW_ERR_PORT_MASK;

    return l2sw_setAsicRegBits(units, L2SW_REG_MIRROR_SRC_PMSK, L2SW_MIRROR_SRC_PMSK_MASK, SourcePortmask);
}

/* Function Name:
 *      l2sw_getAsicPortMirrorMask
 * Description:
 *      Get mirror source port mask
 * Input:
 *      None
 * Output:
 *      pSourcePortmask     - Source Portmask
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_MASK- Port Mask Error
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortMirrorMask(l2sw_uint8 units, l2sw_uint32 *pSourcePortmask)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_MIRROR_SRC_PMSK, L2SW_MIRROR_SRC_PMSK_MASK, pSourcePortmask);
}

/* Function Name:
 *      l2sw_setAsicPortMirrorIsolationRxLeaky
 * Description:
 *      Set the mirror function of  Isolation RX leaky
 * Input:
 *      enabled     - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortMirrorIsolationRxLeaky(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_MIRROR_CTRL2, L2SW_MIRROR_RX_ISOLATION_LEAKY_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicPortMirrorIsolationRxLeaky
 * Description:
 *      Get the mirror function of VLAN RX leaky
 * Input:
 *      pEnabled    - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortMirrorIsolationRxLeaky(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_MIRROR_CTRL2, L2SW_MIRROR_RX_ISOLATION_LEAKY_OFFSET, pEnabled);
}

/* Function Name:
 *      l2sw_setAsicPortMirrorIsolationTxLeaky
 * Description:
 *      Set the mirror function of Isolation TX leaky
 * Input:
 *      enabled     - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortMirrorIsolationTxLeaky(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_MIRROR_CTRL2, L2SW_MIRROR_TX_ISOLATION_LEAKY_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicPortMirrorIsolationTxLeaky
 * Description:
 *      Get the mirror function of VLAN TX leaky
 * Input:
 *      None
 * Output:
 *      pEnabled    - 1: enabled, 0: disabled
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortMirrorIsolationTxLeaky(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_MIRROR_CTRL2, L2SW_MIRROR_TX_ISOLATION_LEAKY_OFFSET, pEnabled);
}

/* Function Name:
 *      l2sw_setAsicPortMirrorRxEnable
 * Description:
 *      Set the mirror function Status
 * Input:
 *      enabled    - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortMirrorRxEnable(l2sw_uint8 units, l2sw_uint32 enabled)
{
    if(enabled == L2SW_ENABLED)
        return l2sw_setAsicRegBits(units, L2SW_MIRROR_CTRL_REG, L2SW_MIRROR_RX_MAGIC_NUM_MASK, L2SW_MIRROR_RX_MAGIC_NUM_VALUE);
    else
        return l2sw_setAsicRegBits(units, L2SW_MIRROR_CTRL_REG, L2SW_MIRROR_RX_MAGIC_NUM_MASK, 0x0);
}

/* Function Name:
 *      l2sw_getAsicPortMirrorRxEnable
 * Description:
 *      Get the mirror function Status
 * Input:
 *      None
 * Output:
 *      pEnabled    - 1: enabled, 0: disabled
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortMirrorRxEnable(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    int ret = 0;
    ret = l2sw_getAsicRegBits(units, L2SW_MIRROR_CTRL_REG, L2SW_MIRROR_RX_MAGIC_NUM_MASK, pEnabled);
    if(L2SW_MIRROR_RX_MAGIC_NUM_VALUE == *pEnabled)
        *pEnabled = L2SW_ENABLED;
    else
        *pEnabled = L2SW_DISABLED;
    return ret;
}

/* Function Name:
 *      l2sw_setAsicPortMirrorTxEnable
 * Description:
 *      Set the mirror function Status
 * Input:
 *      enabled    - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortMirrorTxEnable(l2sw_uint8 units, l2sw_uint32 enabled)
{
    if(enabled == L2SW_ENABLED)
        return l2sw_setAsicRegBits(units, L2SW_MIRROR_CTRL_REG, L2SW_MIRROR_TX_MAGIC_NUM_MASK, L2SW_MIRROR_TX_MAGIC_NUM_VALUE);
    else
        return l2sw_setAsicRegBits(units, L2SW_MIRROR_CTRL_REG, L2SW_MIRROR_TX_MAGIC_NUM_MASK, 0x0);
}

/* Function Name:
 *      l2sw_getAsicPortMirrorTxEnable
 * Description:
 *      Get the mirror function Status
 * Input:
 *      None
 * Output:
 *      pEnabled    - 1: enabled, 0: disabled
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortMirrorTxEnable(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    int ret=0;
    ret = l2sw_getAsicRegBits(units, L2SW_MIRROR_CTRL_REG, L2SW_MIRROR_TX_MAGIC_NUM_MASK, pEnabled);
    if(L2SW_MIRROR_TX_MAGIC_NUM_VALUE == *pEnabled)
        *pEnabled = L2SW_ENABLED;
    else
        *pEnabled = L2SW_DISABLED;
    return ret;
}

/* Function Name:
 *      l2sw_setAsicPortMirrorOamLeaky
 * Description:
 *      Set mirror oam packet leaky enable
 * Input:
 *      enabled    - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortMirrorOamLeaky(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_MIRROR_CTRL2, L2SW_MIRROR_IGNORE_OAM_LOOPBACK_EN_OFFSET, enabled);
}

/* Function Name:
 *      l2sw_getAsicPortMirrorOamLeaky
 * Description:
 *      Get mirror oam packet leaky enable
 * Input:
 *      None
 * Output:
 *      pEnabled    - 1: enabled, 0: disabled
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortMirrorOamLeaky(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_MIRROR_CTRL2, L2SW_MIRROR_IGNORE_OAM_LOOPBACK_EN_OFFSET, pEnabled);
}

/* Function Name:
 *      l2sw_setAsicPortMirrorIncludeAclPortList
 * Description:
 *      Set mirror include acl portlist enable
 * Input:
 *      enabled    - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortMirrorIncludeAclPortList(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_MIRROR_CTRL_REG, L2SW_INCLUDE_ACL_PORTLIST_OFFSET, enabled);
}

/* Function Name:
 *      l2sw_getAsicPortMirrorIncludeAclPortList
 * Description:
 *      Get mirror include acl portlist enable
 * Input:
 *      None
 * Output:
 *      pEnabled    - 1: enabled, 0: disabled
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortMirrorIncludeAclPortList(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_MIRROR_CTRL_REG, L2SW_INCLUDE_ACL_PORTLIST_OFFSET, pEnabled);
}

