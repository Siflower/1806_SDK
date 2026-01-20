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

#include "l2sw_asicdrv_fc.h"
/* Function Name:
 *      l2sw_setAsicFlowControlSelect
 * Description:
 *      Set system flow control type
 * Input:
 *      select      - System flow control type 1: Ingress flow control 0:Egress flow control
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlSelect(l2sw_uint8 units, l2sw_uint32 select)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_FLOWCTRL_CTRL0, L2SW_FLOWCTRL_TYPE_OFFSET, select);
}
/* Function Name:
 *      l2sw_getAsicFlowControlSelect
 * Description:
 *      Get system flow control type
 * Input:
 *      pSelect         - System flow control type 1: Ingress flow control 0:Egress flow control
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlSelect(l2sw_uint8 units, l2sw_uint32 *pSelect)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_FLOWCTRL_CTRL0, L2SW_FLOWCTRL_TYPE_OFFSET, pSelect);
}
/* Function Name:
 *      l2sw_setAsicFlowControlJumboMode
 * Description:
 *      Set Jumbo threhsold for flow control
 * Input:
 *      enabled         - Jumbo mode flow control 1: Enable 0:Disable
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlJumboMode(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_FLOWCTRL_JUMBO_SIZE, L2SW_JUMBO_MODE_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicFlowControlJumboMode
 * Description:
 *      Get Jumbo threhsold for flow control
 * Input:
 *      pEnabled        - Jumbo mode flow control 1: Enable 0:Disable
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlJumboMode(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_FLOWCTRL_JUMBO_SIZE, L2SW_JUMBO_MODE_OFFSET, pEnabled);
}
/* Function Name:
 *      l2sw_setAsicFlowControlJumboModeSize
 * Description:
 *      Set Jumbo size for Jumbo mode flow control
 * Input:
 *      size        - Jumbo size 0:3Kbytes 1:4Kbytes 2:6Kbytes 3:9Kbytes
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlJumboModeSize(l2sw_uint8 units, l2sw_uint32 size)
{
    if(size >= FC_JUMBO_SIZE_END)
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_SIZE, L2SW_JUMBO_SIZE_MASK, size);
}
/* Function Name:
 *      l2sw_getAsicFlowControlJumboModeSize
 * Description:
 *      Get Jumbo size for Jumbo mode flow control
 * Input:
 *      pSize       - Jumbo size 0:3Kbytes 1:4Kbytes 2:6Kbytes 3:9Kbytes
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlJumboModeSize(l2sw_uint8 units, l2sw_uint32* pSize)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_SIZE, L2SW_JUMBO_SIZE_MASK, pSize);
}

/* Function Name:
 *      l2sw_setAsicFlowControlQueueEgressEnable
 * Description:
 *      Set flow control ability for each queue
 * Input:
 *      port    - Physical port number (0~7)
 *      qid     - Queue id
 *      enabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 *      L2SW_ERR_QUEUE_ID - Invalid queue id
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlQueueEgressEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 enabled)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(qid > L2SW_QIDMAX)
        return L2SW_ERR_QUEUE_ID;

    return l2sw_setAsicRegBit(units, L2SW_FLOWCTRL_EGRESS_QUEUE_ENABLE_REG(port), L2SW_FLOWCTRL_EGRESS_QUEUE_ENABLE_REG_OFFSET(port)+ qid, enabled);
}
/* Function Name:
 *      l2sw_getAsicFlowControlQueueEgressEnable
 * Description:
 *      Get flow control ability for each queue
 * Input:
 *      port    - Physical port number (0~7)
 *      qid     - Queue id
 *      pEnabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 *      L2SW_ERR_QUEUE_ID - Invalid queue id
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlQueueEgressEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32* pEnabled)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(qid > L2SW_QIDMAX)
        return L2SW_ERR_QUEUE_ID;

    return  l2sw_getAsicRegBit(units, L2SW_FLOWCTRL_EGRESS_QUEUE_ENABLE_REG(port), L2SW_FLOWCTRL_EGRESS_QUEUE_ENABLE_REG_OFFSET(port)+ qid, pEnabled);
}
/* Function Name:
 *      l2sw_setAsicFlowControlDropAll
 * Description:
 *      Set system-based drop parameters
 * Input:
 *      dropall     - Whole system drop threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlDropAll(l2sw_uint8 units, l2sw_uint32 dropall)
{
    if(dropall >= L2SW_PAGE_NUMBER)
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_CTRL0, L2SW_DROP_ALL_THRESHOLD_DATA_MASK, dropall);
}
/* Function Name:
 *      l2sw_getAsicFlowControlDropAll
 * Description:
 *      Get system-based drop parameters
 * Input:
 *      pDropall    - Whole system drop threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlDropAll(l2sw_uint8 units, l2sw_uint32* pDropall)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_CTRL0, L2SW_DROP_ALL_THRESHOLD_DATA_MASK, pDropall);
}
/* Function Name:
 *      l2sw_setAsicFlowControlPauseAll
 * Description:
 *      Set system-based all ports enable flow control parameters
 * Input:
 *      threshold   - Whole system pause all threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlPauseAllThreshold(l2sw_uint8 units, l2sw_uint32 threshold)
{
    if(threshold >= L2SW_PAGE_NUMBER)
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_ALL_ON, L2SW_FLOWCTRL_ALL_ON_THRESHOLD_MASK, threshold);
}
/* Function Name:
 *      l2sw_getAsicFlowControlPauseAllThreshold
 * Description:
 *      Get system-based all ports enable flow control parameters
 * Input:
 *      pThreshold  - Whole system pause all threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlPauseAllThreshold(l2sw_uint8 units, l2sw_uint32 *pThreshold)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_ALL_ON, L2SW_FLOWCTRL_ALL_ON_THRESHOLD_MASK, pThreshold);
}
/* Function Name:
 *      l2sw_setAsicFlowControlSystemThreshold
 * Description:
 *      Set system-based flow control parameters
 * Input:
 *      onThreshold     - Flow control turn ON threshold
 *      offThreshold    - Flow control turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlSystemThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    ret_t retVal;

    if((onThreshold >= L2SW_PAGE_NUMBER) || (offThreshold >= L2SW_PAGE_NUMBER))
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_SYS_OFF, L2SW_FLOWCTRL_SYS_OFF_MASK, offThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_SYS_ON, L2SW_FLOWCTRL_SYS_ON_MASK, onThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicFlowControlSystemThreshold
 * Description:
 *      Get system-based flow control parameters
 * Input:
 *      pOnThreshold    - Flow control turn ON threshold
 *      pOffThreshold   - Flow control turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlSystemThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    ret_t retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_SYS_OFF, L2SW_FLOWCTRL_SYS_OFF_MASK, pOffThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_SYS_ON, L2SW_FLOWCTRL_SYS_ON_MASK, pOnThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_setAsicFlowControlSharedThreshold
 * Description:
 *      Set share-based flow control parameters
 * Input:
 *      onThreshold     - Flow control turn ON threshold
 *      offThreshold    - Flow control turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlSharedThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    ret_t retVal;

    if((onThreshold >= L2SW_PAGE_NUMBER) || (offThreshold >= L2SW_PAGE_NUMBER))
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_SHARE_OFF, L2SW_FLOWCTRL_SHARE_OFF_MASK, offThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_SHARE_ON, L2SW_FLOWCTRL_SHARE_ON_MASK, onThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicFlowControlSharedThreshold
 * Description:
 *      Get share-based flow control parameters
 * Input:
 *      pOnThreshold    - Flow control turn ON threshold
 *      pOffThreshold   - Flow control turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlSharedThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    ret_t retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_SHARE_OFF, L2SW_FLOWCTRL_SHARE_OFF_MASK, pOffThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_SHARE_ON, L2SW_FLOWCTRL_SHARE_ON_MASK, pOnThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_setAsicFlowControlPortThreshold
 * Description:
 *      Set Port-based flow control parameters
 * Input:
 *      onThreshold     - Flow control turn ON threshold
 *      offThreshold    - Flow control turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlPortThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    ret_t retVal;

    if((onThreshold >= L2SW_PAGE_NUMBER) || (offThreshold >= L2SW_PAGE_NUMBER))
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_PORT_OFF, L2SW_FLOWCTRL_PORT_OFF_MASK, offThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_PORT_ON, L2SW_FLOWCTRL_PORT_ON_MASK, onThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicFlowControlPortThreshold
 * Description:
 *      Get Port-based flow control parameters
 * Input:
 *      pOnThreshold    - Flow control turn ON threshold
 *      pOffThreshold   - Flow control turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlPortThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    ret_t retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_PORT_OFF, L2SW_FLOWCTRL_PORT_OFF_MASK, pOffThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_PORT_ON, L2SW_FLOWCTRL_PORT_ON_MASK, pOnThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_setAsicFlowControlPortPrivateThreshold
 * Description:
 *      Set Port-private-based flow control parameters
 * Input:
 *      onThreshold     - Flow control turn ON threshold
 *      offThreshold    - Flow control turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlPortPrivateThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    ret_t retVal;

    if((onThreshold >= L2SW_PAGE_NUMBER) || (offThreshold >= L2SW_PAGE_NUMBER))
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_PORT_PRIVATE_OFF, L2SW_FLOWCTRL_PORT_PRIVATE_OFF_MASK, offThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_PORT_PRIVATE_ON, L2SW_FLOWCTRL_PORT_PRIVATE_ON_MASK, onThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicFlowControlPortPrivateThreshold
 * Description:
 *      Get Port-private-based flow control parameters
 * Input:
 *      pOnThreshold    - Flow control turn ON threshold
 *      pOffThreshold   - Flow control turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlPortPrivateThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    ret_t retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_PORT_PRIVATE_OFF, L2SW_FLOWCTRL_PORT_PRIVATE_OFF_MASK, pOffThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_PORT_PRIVATE_ON, L2SW_FLOWCTRL_PORT_PRIVATE_ON_MASK, pOnThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_setAsicFlowControlSystemDropThreshold
 * Description:
 *      Set system-based drop parameters
 * Input:
 *      onThreshold     - Drop turn ON threshold
 *      offThreshold    - Drop turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlSystemDropThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    ret_t retVal;

    if((onThreshold >= L2SW_PAGE_NUMBER) || (offThreshold >= L2SW_PAGE_NUMBER))
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_FCOFF_SYS_OFF, L2SW_FLOWCTRL_FCOFF_SYS_OFF_MASK, offThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_FCOFF_SYS_ON, L2SW_FLOWCTRL_FCOFF_SYS_ON_MASK, onThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicFlowControlSystemDropThreshold
 * Description:
 *      Get system-based drop parameters
 * Input:
 *      pOnThreshold    - Drop turn ON threshold
 *      pOffThreshold   - Drop turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlSystemDropThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    ret_t retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_FCOFF_SYS_OFF, L2SW_FLOWCTRL_FCOFF_SYS_OFF_MASK, pOffThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_FCOFF_SYS_ON, L2SW_FLOWCTRL_FCOFF_SYS_ON_MASK, pOnThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_setAsicFlowControlSharedDropThreshold
 * Description:
 *      Set share-based fdrop parameters
 * Input:
 *      onThreshold     - Drop turn ON threshold
 *      offThreshold    - Drop turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlSharedDropThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    ret_t retVal;

    if((onThreshold >= L2SW_PAGE_NUMBER) || (offThreshold >= L2SW_PAGE_NUMBER))
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_FCOFF_SHARE_OFF, L2SW_FLOWCTRL_FCOFF_SHARE_OFF_MASK, offThreshold);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_FCOFF_SHARE_ON, L2SW_FLOWCTRL_FCOFF_SHARE_ON_MASK, onThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicFlowControlSharedDropThreshold
 * Description:
 *      Get share-based fdrop parameters
 * Input:
 *      pOnThreshold    - Drop turn ON threshold
 *      pOffThreshold   - Drop turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlSharedDropThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    ret_t retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_FCOFF_SHARE_OFF, L2SW_FLOWCTRL_FCOFF_SHARE_OFF_MASK, pOffThreshold);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_FCOFF_SHARE_ON, L2SW_FLOWCTRL_FCOFF_SHARE_ON_MASK, pOnThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_setAsicFlowControlPortDropThreshold
 * Description:
 *      Set Port-based drop parameters
 * Input:
 *      onThreshold     - Drop turn ON threshold
 *      offThreshold    - Drop turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlPortDropThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    ret_t retVal;

    if((onThreshold >= L2SW_PAGE_NUMBER) || (offThreshold >= L2SW_PAGE_NUMBER))
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_FCOFF_PORT_OFF, L2SW_FLOWCTRL_FCOFF_PORT_OFF_MASK, offThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_FCOFF_PORT_ON, L2SW_FLOWCTRL_FCOFF_PORT_ON_MASK, onThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicFlowControlPortDropThreshold
 * Description:
 *      Get Port-based drop parameters
 * Input:
 *      pOnThreshold    - Drop turn ON threshold
 *      pOffThreshold   - Drop turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlPortDropThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    ret_t retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_FCOFF_PORT_OFF, L2SW_FLOWCTRL_FCOFF_PORT_OFF_MASK, pOffThreshold);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_FCOFF_PORT_ON, L2SW_FLOWCTRL_FCOFF_PORT_ON_MASK, pOnThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_setAsicFlowControlPortPrivateDropThreshold
 * Description:
 *      Set Port-private-based drop parameters
 * Input:
 *      onThreshold     - Drop turn ON threshold
 *      offThreshold    - Drop turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlPortPrivateDropThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    ret_t retVal;

    if((onThreshold >= L2SW_PAGE_NUMBER) || (offThreshold >= L2SW_PAGE_NUMBER))
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_FCOFF_PORT_PRIVATE_OFF, L2SW_FLOWCTRL_FCOFF_PORT_PRIVATE_OFF_MASK, offThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_FCOFF_PORT_PRIVATE_ON, L2SW_FLOWCTRL_FCOFF_PORT_PRIVATE_ON_MASK, onThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicFlowControlPortPrivateDropThreshold
 * Description:
 *      Get Port-private-based drop parameters
 * Input:
 *      pOnThreshold    - Drop turn ON threshold
 *      pOffThreshold   - Drop turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlPortPrivateDropThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    ret_t retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_FCOFF_PORT_PRIVATE_OFF, L2SW_FLOWCTRL_FCOFF_PORT_PRIVATE_OFF_MASK, pOffThreshold);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_FCOFF_PORT_PRIVATE_ON, L2SW_FLOWCTRL_FCOFF_PORT_PRIVATE_ON_MASK, pOnThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_setAsicFlowControlSystemJumboThreshold
 * Description:
 *      Set Jumbo system-based flow control parameters
 * Input:
 *      onThreshold     - Flow control turn ON threshold
 *      offThreshold    - Flow control turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlSystemJumboThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    ret_t retVal;

    if((onThreshold >= L2SW_PAGE_NUMBER) || (offThreshold >= L2SW_PAGE_NUMBER))
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_SYS_OFF, L2SW_FLOWCTRL_JUMBO_SYS_OFF_MASK, offThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_SYS_ON, L2SW_FLOWCTRL_JUMBO_SYS_ON_MASK, onThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicFlowControlSystemJumboThreshold
 * Description:
 *      Get Jumbo system-based flow control parameters
 * Input:
 *      pOnThreshold    - Flow control turn ON threshold
 *      pOffThreshold   - Flow control turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlSystemJumboThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    ret_t retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_SYS_OFF, L2SW_FLOWCTRL_JUMBO_SYS_OFF_MASK, pOffThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_SYS_ON, L2SW_FLOWCTRL_JUMBO_SYS_ON_MASK, pOnThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_setAsicFlowControlSharedJumboThreshold
 * Description:
 *      Set Jumbo share-based flow control parameters
 * Input:
 *      onThreshold     - Flow control turn ON threshold
 *      offThreshold    - Flow control turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlSharedJumboThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    ret_t retVal;

    if((onThreshold >= L2SW_PAGE_NUMBER) || (offThreshold >= L2SW_PAGE_NUMBER))
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_SHARE_OFF, L2SW_FLOWCTRL_JUMBO_SHARE_OFF_MASK, offThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_SHARE_ON, L2SW_FLOWCTRL_JUMBO_SHARE_ON_MASK, onThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicFlowControlSharedJumboThreshold
 * Description:
 *      Get Jumbo share-based flow control parameters
 * Input:
 *      pOnThreshold    - Flow control turn ON threshold
 *      pOffThreshold   - Flow control turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlSharedJumboThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    ret_t retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_SHARE_OFF, L2SW_FLOWCTRL_JUMBO_SHARE_OFF_MASK, pOffThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_SHARE_ON, L2SW_FLOWCTRL_JUMBO_SHARE_ON_MASK, pOnThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_setAsicFlowControlPortJumboThreshold
 * Description:
 *      Set Jumbo Port-based flow control parameters
 * Input:
 *      onThreshold     - Flow control turn ON threshold
 *      offThreshold    - Flow control turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlPortJumboThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    ret_t retVal;

    if((onThreshold >= L2SW_PAGE_NUMBER) || (offThreshold >= L2SW_PAGE_NUMBER))
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_PORT_OFF, L2SW_FLOWCTRL_JUMBO_PORT_OFF_MASK, offThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_PORT_ON, L2SW_FLOWCTRL_JUMBO_PORT_ON_MASK, onThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicFlowControlPortJumboThreshold
 * Description:
 *      Get Jumbo Port-based flow control parameters
 * Input:
 *      pOnThreshold    - Flow control turn ON threshold
 *      pOffThreshold   - Flow control turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlPortJumboThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    ret_t retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_PORT_OFF, L2SW_FLOWCTRL_JUMBO_PORT_OFF_MASK, pOffThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_PORT_ON, L2SW_FLOWCTRL_JUMBO_PORT_ON_MASK, pOnThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_setAsicFlowControlPortPrivateJumboThreshold
 * Description:
 *      Set Jumbo Port-private-based flow control parameters
 * Input:
 *      onThreshold     - Flow control turn ON threshold
 *      offThreshold    - Flow control turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlPortPrivateJumboThreshold(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    ret_t retVal;

    if((onThreshold >= L2SW_PAGE_NUMBER) || (offThreshold >= L2SW_PAGE_NUMBER))
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_PORT_PRIVATE_OFF, L2SW_FLOWCTRL_JUMBO_PORT_PRIVATE_OFF_MASK, offThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_PORT_PRIVATE_ON, L2SW_FLOWCTRL_JUMBO_PORT_PRIVATE_ON_MASK, onThreshold);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicFlowControlPortPrivateJumboThreshold
 * Description:
 *      Get Jumbo Port-private-based flow control parameters
 * Input:
 *      pOnThreshold    - Flow control turn ON threshold
 *      pOffThreshold   - Flow control turn OFF threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlPortPrivateJumboThreshold(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    ret_t retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_PORT_PRIVATE_OFF, L2SW_FLOWCTRL_JUMBO_PORT_PRIVATE_OFF_MASK, pOffThreshold);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_JUMBO_PORT_PRIVATE_ON, L2SW_FLOWCTRL_JUMBO_PORT_PRIVATE_ON_MASK, pOnThreshold);

    return retVal;
}



/* Function Name:
 *      l2sw_setAsicEgressFlowControlQueueDropThreshold
 * Description:
 *      Set Queue-based egress flow control turn on or ingress flow control drop on threshold
 * Input:
 *      qid         - The queue id
 *      threshold   - Queue-based flown control/drop turn ON threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 *      L2SW_ERR_QUEUE_ID     - Invalid queue id
 * Note:
 *      None
 */
ret_t l2sw_setAsicEgressFlowControlQueueDropThreshold(l2sw_uint8 units, l2sw_uint32 qid, l2sw_uint32 threshold)
{
    if( threshold >= L2SW_PAGE_NUMBER)
        return L2SW_ERR_OUT_OF_RANGE;

    if(qid > L2SW_QIDMAX)
        return L2SW_ERR_QUEUE_ID;

    return l2sw_setAsicRegBits(units, L2SW_FLOWCTRL_QUEUE_DROP_ON_REG(qid), L2SW_FLOWCTRL_QUEUE_DROP_ON_MASK, threshold);
}
/* Function Name:
 *      l2sw_getAsicEgressFlowControlQueueDropThreshold
 * Description:
 *      Get Queue-based egress flow control turn on or ingress flow control drop on threshold
 * Input:
 *      qid         - The queue id
 *      pThreshold  - Queue-based flown control/drop turn ON threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_QUEUE_ID     - Invalid queue id
 * Note:
 *      None
 */
ret_t l2sw_getAsicEgressFlowControlQueueDropThreshold(l2sw_uint8 units, l2sw_uint32 qid, l2sw_uint32 *pThreshold)
{
    if(qid > L2SW_QIDMAX)
      return L2SW_ERR_QUEUE_ID;

    return l2sw_getAsicRegBits(units, L2SW_FLOWCTRL_QUEUE_DROP_ON_REG(qid), L2SW_FLOWCTRL_QUEUE_DROP_ON_MASK, pThreshold);
}
/* Function Name:
 *      l2sw_setAsicEgressFlowControlPortDropThreshold
 * Description:
 *      Set port-based egress flow control turn on or ingress flow control drop on threshold
 * Input:
 *      port        - Physical port number (0~7)
 *      threshold   - Queue-based flown control/drop turn ON threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicEgressFlowControlPortDropThreshold(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 threshold)
{
    if(port > L2SW_PORTIDMAX)
      return L2SW_ERR_PORT_ID;

    if(threshold >= L2SW_PAGE_NUMBER)
      return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_setAsicRegBits(units, L2SW_FLOWCTRL_PORT_DROP_ON_REG(port), L2SW_FLOWCTRL_PORT_DROP_ON_MASK, threshold);
}
/* Function Name:
 *      l2sw_setAsicEgressFlowControlPortDropThreshold
 * Description:
 *      Set port-based egress flow control turn on or ingress flow control drop on threshold
 * Input:
 *      port        - Physical port number (0~7)
 *      pThreshold  - Queue-based flown control/drop turn ON threshold
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicEgressFlowControlPortDropThreshold(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pThreshold)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBits(units, L2SW_FLOWCTRL_PORT_DROP_ON_REG(port), L2SW_FLOWCTRL_PORT_DROP_ON_MASK, pThreshold);
}
/* Function Name:
 *      l2sw_setAsicEgressFlowControlPortDropGap
 * Description:
 *      Set port-based egress flow control turn off or ingress flow control drop off gap
 * Input:
 *      gap     - Flow control/drop turn OFF threshold = turn ON threshold - gap
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicEgressFlowControlPortDropGap(l2sw_uint8 units, l2sw_uint32 gap)
{
    if(gap >= L2SW_PAGE_NUMBER)
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_PORT_GAP, L2SW_FLOWCTRL_PORT_GAP_MASK, gap);
}
/* Function Name:
 *      l2sw_getAsicEgressFlowControlPortDropGap
 * Description:
 *      Get port-based egress flow control turn off or ingress flow control drop off gap
 * Input:
 *      pGap    - Flow control/drop turn OFF threshold = turn ON threshold - gap
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicEgressFlowControlPortDropGap(l2sw_uint8 units, l2sw_uint32 *pGap)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_PORT_GAP, L2SW_FLOWCTRL_PORT_GAP_MASK, pGap);
}
/* Function Name:
 *      l2sw_setAsicEgressFlowControlQueueDropGap
 * Description:
 *      Set Queue-based egress flow control turn off or ingress flow control drop off gap
 * Input:
 *      gap     - Flow control/drop turn OFF threshold = turn ON threshold - gap
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicEgressFlowControlQueueDropGap(l2sw_uint8 units, l2sw_uint32 gap)
{
    if(gap >= L2SW_PAGE_NUMBER)
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_setAsicRegBits(units, L2SW_REG_FLOWCTRL_QUEUE_GAP, L2SW_FLOWCTRL_QUEUE_GAP_MASK, gap);
}
/* Function Name:
 *      l2sw_getAsicEgressFlowControlQueueDropGap
 * Description:
 *      Get Queue-based egress flow control turn off or ingress flow control drop off gap
 * Input:
 *      pGap    - Flow control/drop turn OFF threshold = turn ON threshold - gap
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicEgressFlowControlQueueDropGap(l2sw_uint8 units, l2sw_uint32 *pGap)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_QUEUE_GAP, L2SW_FLOWCTRL_QUEUE_GAP_MASK, pGap);
}
/* Function Name:
 *      l2sw_getAsicEgressQueueEmptyPortMask
 * Description:
 *      Get queue empty port mask
 * Input:
 *      pPortmask   -  Queue empty port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicEgressQueueEmptyPortMask(l2sw_uint8 units, l2sw_uint32 *pPortmask)
{
    return l2sw_getAsicReg(units, L2SW_REG_PORT_QEMPTY, pPortmask);
}
/* Function Name:
 *      l2sw_getAsicTotalPage
 * Description:
 *      Get system total page usage number
 * Input:
 *      pPageCount  -  page usage number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicTotalPage(l2sw_uint8 units, l2sw_uint32 *pPageCount)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_TOTAL_PAGE_COUNTER, L2SW_FLOWCTRL_TOTAL_PAGE_COUNTER_MASK, pPageCount);
}
/* Function Name:
 *      l2sw_getAsicPulbicPage
 * Description:
 *      Get system public page usage number
 * Input:
 *      pPageCount  -  page usage number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicPulbicPage(l2sw_uint8 units, l2sw_uint32 *pPageCount)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_PUBLIC_PAGE_COUNTER, L2SW_FLOWCTRL_PUBLIC_PAGE_COUNTER_MASK, pPageCount);
}
/* Function Name:
 *      l2sw_getAsicMaxTotalPage
 * Description:
 *      Get system total page max usage number
 * Input:
 *      pPageCount  -  page usage number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicMaxTotalPage(l2sw_uint8 units, l2sw_uint32 *pPageCount)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_TOTAL_PAGE_MAX, L2SW_FLOWCTRL_TOTAL_PAGE_MAX_MASK, pPageCount);
}
/* Function Name:
 *      l2sw_getAsicPulbicPage
 * Description:
 *      Get system public page max usage number
 * Input:
 *      pPageCount  -  page usage number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicMaxPulbicPage(l2sw_uint8 units, l2sw_uint32 *pPageCount)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_PUBLIC_PAGE_MAX, L2SW_FLOWCTRL_PUBLIC_PAGE_MAX_MASK, pPageCount);
}
/* Function Name:
 *      l2sw_getAsicPortPage
 * Description:
 *      Get per-port page usage number
 * Input:
 *      port        -  Physical port number (0~7)
 *      pPageCount  -  page usage number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortPage(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pPageCount)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 8)
        return l2sw_getAsicRegBits(units, L2SW_FLOWCTRL_PORT_PAGE_COUNTER_REG(port), L2SW_FLOWCTRL_PORT_PAGE_COUNTER_MASK, pPageCount);
    else
        return l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_PORT8_PAGE_COUNTER+port - 8, L2SW_FLOWCTRL_PORT_PAGE_COUNTER_MASK, pPageCount);
}
/* Function Name:
 *      l2sw_getAsicPortPage
 * Description:
 *      Get per-port page max usage number
 * Input:
 *      port        -  Physical port number (0~7)
 *      pPageCount  -  page usage number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortPageMax(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pPageCount)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;
    if(port < 8)
        return l2sw_getAsicRegBits(units, L2SW_FLOWCTRL_PORT_PAGE_MAX_REG(port), L2SW_FLOWCTRL_PORT_PAGE_MAX_MASK, pPageCount);
    else
        return l2sw_getAsicRegBits(units, L2SW_REG_FLOWCTRL_PORT0_PAGE_MAX+port-8, L2SW_FLOWCTRL_PORT_PAGE_MAX_MASK, pPageCount);


}

/* Function Name:
 *      l2sw_setAsicFlowControlEgressPortIndep
 * Description:
 *      Set per-port egress flow control independent
 * Input:
 *      port        - Physical port number (0~7)
 *      enabled     - Egress port flow control usage 1:enable 0:disable.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsicFlowControlEgressPortIndep(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enable)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    return l2sw_setAsicRegBit(units, L2SW_REG_PORT0_MISC_CFG + (port *0x20), L2SW_PORT0_MISC_CFG_FLOWCTRL_INDEP_OFFSET,enable);
}

/* Function Name:
 *      l2sw_getAsicFlowControlEgressPortIndep
 * Description:
 *      Get per-port egress flow control independent
 * Input:
 *      port        - Physical port number (0~7)
 *      enabled     - Egress port flow control usage 1:enable 0:disable.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicFlowControlEgressPortIndep(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnable)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_PORT0_MISC_CFG + (port *0x20),L2SW_PORT0_MISC_CFG_FLOWCTRL_INDEP_OFFSET,pEnable);
}
