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

#include "l2sw_asicdrv_cputag.h"
/* Function Name:
 *      l2sw_setAsicCputagEnable
 * Description:
 *      Set cpu tag function enable/disable
 * Input:
 *      enabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_ENABLE   - Invalid enable/disable input
 * Note:
 *      If CPU tag function is disabled, CPU tag will not be added to frame
 *      forwarded to CPU port, and all ports cannot parse CPU tag.
 */
ret_t l2sw_setAsicCputagEnable(l2sw_uint8 units, l2sw_uint32 enabled)
{
    if(enabled > 1)
        return L2SW_ERR_ENABLE;

    return l2sw_setAsicRegBit(units, L2SW_REG_CPU_CTRL, L2SW_CPU_EN_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicCputagEnable
 * Description:
 *      Get cpu tag function enable/disable
 * Input:
 *      pEnabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicCputagEnable(l2sw_uint8 units, l2sw_uint32 *pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_CPU_CTRL, L2SW_CPU_EN_OFFSET, pEnabled);
}
/* Function Name:
 *      l2sw_setAsicCputagTrapPort
 * Description:
 *      Set cpu tag trap port
 * Input:
 *      port - port number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *     API can set destination port of trapping frame
 */
ret_t l2sw_setAsicCputagTrapPort(l2sw_uint8 units, l2sw_uint32 port)
{
    ret_t retVal;

    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_CPU_CTRL, L2SW_CPU_TRAP_PORT_MASK, port & 7);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_CPU_CTRL, L2SW_CPU_TRAP_PORT_EXT_MASK, (port>>3) & 1);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicCputagTrapPort
 * Description:
 *      Get cpu tag trap port
 * Input:
 *      pPort - port number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *     None
 */
ret_t l2sw_getAsicCputagTrapPort(l2sw_uint8 units, l2sw_uint32 *pPort)
{
    ret_t retVal;
    l2sw_uint32 tmpPort;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_CPU_CTRL, L2SW_CPU_TRAP_PORT_MASK, &tmpPort);
    if(retVal != L2SW_ERR_OK)
        return retVal;
    *pPort = tmpPort;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_CPU_CTRL, L2SW_CPU_TRAP_PORT_EXT_MASK, &tmpPort);
    if(retVal != L2SW_ERR_OK)
        return retVal;
    *pPort |= (tmpPort & 1) << 3;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicCputagPortmask
 * Description:
 *      Set ports that can parse CPU tag
 * Input:
 *      portmask - port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_MASK    - Invalid portmask
 * Note:
 *     None
 */
ret_t l2sw_setAsicCputagPortmask(l2sw_uint8 units, l2sw_uint32 portmask)
{
    if(portmask > L2SW_PORTMASK)
        return L2SW_ERR_PORT_MASK;

    return l2sw_setAsicReg(units, L2SW_CPU_PORT_MASK_REG, portmask);
}
/* Function Name:
 *      l2sw_getAsicCputagPortmask
 * Description:
 *      Get ports that can parse CPU tag
 * Input:
 *      pPortmask - port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *     None
 */
ret_t l2sw_getAsicCputagPortmask(l2sw_uint8 units, l2sw_uint32 *pPortmask)
{
    return l2sw_getAsicReg(units, L2SW_CPU_PORT_MASK_REG, pPortmask);
}
/* Function Name:
 *      l2sw_setAsicCputagInsertMode
 * Description:
 *      Set CPU-tag insert mode
 * Input:
 *      mode - 0: insert to all packets; 1: insert to trapped packets; 2: don't insert
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_NOT_ALLOWED  - Actions not allowed by the function
 * Note:
 *     None
 */
ret_t l2sw_setAsicCputagInsertMode(l2sw_uint8 units, l2sw_uint32 mode)
{
    if(mode >= CPUTAG_INSERT_END)
        return L2SW_ERR_NOT_ALLOWED;

    return l2sw_setAsicRegBits(units, L2SW_REG_CPU_CTRL, L2SW_CPU_INSERTMODE_MASK, mode);
}
/* Function Name:
 *      l2sw_getAsicCputagInsertMode
 * Description:
 *      Get CPU-tag insert mode
 * Input:
 *      pMode - 0: insert to all packets; 1: insert to trapped packets; 2: don't insert
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *     None
 */
ret_t l2sw_getAsicCputagInsertMode(l2sw_uint8 units, l2sw_uint32 *pMode)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_CPU_CTRL, L2SW_CPU_INSERTMODE_MASK, pMode);
}
/* Function Name:
 *      l2sw_setAsicCputagPriorityRemapping
 * Description:
 *      Set queue assignment of CPU port
 * Input:
 *      srcPri - internal priority (0~7)
 *      newPri - internal priority after remapping (0~7)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_QOS_INT_PRIORITY     - Invalid priority
 * Note:
 *     None
 */
ret_t l2sw_setAsicCputagPriorityRemapping(l2sw_uint8 units, l2sw_uint32 srcPri, l2sw_uint32 newPri)
{
    if((srcPri > L2SW_PRIMAX) || (newPri > L2SW_PRIMAX))
        return L2SW_ERR_QOS_INT_PRIORITY;

    return l2sw_setAsicRegBits(units, L2SW_QOS_PRIPORITY_REMAPPING_IN_CPU_REG(srcPri), L2SW_QOS_PRIPORITY_REMAPPING_IN_CPU_MASK(srcPri), newPri);
}
/* Function Name:
 *      l2sw_getAsicCputagPriorityRemapping
 * Description:
 *      Get queue assignment of CPU port
 * Input:
 *      srcPri - internal priority (0~7)
 *      pNewPri - internal priority after remapping (0~7)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_QOS_INT_PRIORITY     - Invalid priority
 * Note:
 *     None
 */
ret_t l2sw_getAsicCputagPriorityRemapping(l2sw_uint8 units, l2sw_uint32 srcPri, l2sw_uint32 *pNewPri)
{
    if(srcPri > L2SW_PRIMAX)
        return L2SW_ERR_QOS_INT_PRIORITY;

    return l2sw_getAsicRegBits(units, L2SW_QOS_PRIPORITY_REMAPPING_IN_CPU_REG(srcPri), L2SW_QOS_PRIPORITY_REMAPPING_IN_CPU_MASK(srcPri), pNewPri);
}
/* Function Name:
 *      l2sw_setAsicCputagPosition
 * Description:
 *      Set cpu tag insert position
 * Input:
 *      postion - 1: After entire packet(before CRC field), 0: After MAC_SA (Default)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 * Note:
 *     None
 */
ret_t l2sw_setAsicCputagPosition(l2sw_uint8 units, l2sw_uint32 postion)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_CPU_CTRL, L2SW_CPU_TAG_POSITION_OFFSET, postion);
}
/* Function Name:
 *      l2sw_getAsicCputagPosition
 * Description:
 *      Get cpu tag insert position
 * Input:
 *      pPostion - 1: After entire packet(before CRC field), 0: After MAC_SA (Default)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 * Note:
 *     None
 */
ret_t l2sw_getAsicCputagPosition(l2sw_uint8 units, l2sw_uint32* pPostion)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_CPU_CTRL, L2SW_CPU_TAG_POSITION_OFFSET, pPostion);
}

/* Function Name:
 *      l2sw_setAsicCputagMode
 * Description:
 *      Set cpu tag mode
 * Input:
 *      mode - 1: 4bytes mode, 0: 8bytes mode
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_INPUT    - Invalid input parameters
 * Note:
 *      If CPU tag function is disabled, CPU tag will not be added to frame
 *      forwarded to CPU port, and all ports cannot parse CPU tag.
 */
ret_t l2sw_setAsicCputagMode(l2sw_uint8 units, l2sw_uint32 mode)
{
    if(mode > 1)
        return L2SW_ERR_INPUT;

    return l2sw_setAsicRegBit(units, L2SW_REG_CPU_CTRL, L2SW_CPU_TAG_FORMAT_OFFSET, mode);
}
/* Function Name:
 *      l2sw_getAsicCputagMode
 * Description:
 *      Get cpu tag mode
 * Input:
 *      pMode - 1: 4bytes mode, 0: 8bytes mode
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicCputagMode(l2sw_uint8 units, l2sw_uint32 *pMode)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_CPU_CTRL, L2SW_CPU_TAG_FORMAT_OFFSET, pMode);
}
/* Function Name:
 *      l2sw_setAsicCputagRxMinLength
 * Description:
 *      Set cpu tag mode
 * Input:
 *      mode - 1: 64bytes, 0: 72bytes
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_INPUT    - Invalid input parameters
 * Note:
 *      If CPU tag function is disabled, CPU tag will not be added to frame
 *      forwarded to CPU port, and all ports cannot parse CPU tag.
 */
ret_t l2sw_setAsicCputagRxMinLength(l2sw_uint8 units, l2sw_uint32 mode)
{
    if(mode > 1)
        return L2SW_ERR_INPUT;

    return l2sw_setAsicRegBit(units, L2SW_REG_CPU_CTRL, L2SW_CPU_TAG_RXBYTECOUNT_OFFSET, mode);
}
/* Function Name:
 *      l2sw_getAsicCputagRxMinLength
 * Description:
 *      Get cpu tag mode
 * Input:
 *      pMode - 1: 64bytes, 0: 72bytes
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicCputagRxMinLength(l2sw_uint8 units, l2sw_uint32 *pMode)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_CPU_CTRL, L2SW_CPU_TAG_RXBYTECOUNT_OFFSET, pMode);
}

