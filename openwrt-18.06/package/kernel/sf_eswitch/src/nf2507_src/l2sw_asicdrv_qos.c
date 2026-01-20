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

#include "l2sw_asicdrv_qos.h"
/* Function Name:
 *      l2sw_setAsicPriorityDot1qRemapping
 * Description:
 *      Set 802.1Q absolutely priority
 * Input:
 *      srcpriority - Priority value
 *      priority     - Absolute priority value
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                 - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_QOS_INT_PRIORITY    - Invalid priority
 * Note:
 *      None
 */
ret_t l2sw_setAsicPriorityDot1qRemapping(l2sw_uint8 units, l2sw_uint32 srcpriority, l2sw_uint32 priority )
{
    if((srcpriority > L2SW_PRIMAX) || (priority > L2SW_PRIMAX))
        return L2SW_ERR_QOS_INT_PRIORITY;

    return l2sw_setAsicRegBits(units, L2SW_QOS_1Q_PRIORITY_REMAPPING_REG(srcpriority), L2SW_QOS_1Q_PRIORITY_REMAPPING_MASK(srcpriority),priority);
}
/* Function Name:
 *      l2sw_getAsicPriorityDot1qRemapping
 * Description:
 *      Get 802.1Q absolutely priority
 * Input:
 *      srcpriority - Priority value
 *      pPriority     - Absolute priority value
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK     - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicPriorityDot1qRemapping(l2sw_uint8 units, l2sw_uint32 srcpriority, l2sw_uint32 *pPriority )
{
    if(srcpriority > L2SW_PRIMAX )
        return L2SW_ERR_QOS_INT_PRIORITY;

    return l2sw_getAsicRegBits(units, L2SW_QOS_1Q_PRIORITY_REMAPPING_REG(srcpriority), L2SW_QOS_1Q_PRIORITY_REMAPPING_MASK(srcpriority), pPriority);
}
/* Function Name:
 *      l2sw_setAsicPriorityPortBased
 * Description:
 *      Set port based priority
 * Input:
 *      port         - Physical port number (0~7)
 *      priority     - Priority value
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                 - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number
 *      L2SW_ERR_QOS_INT_PRIORITY    - Invalid priority
 * Note:
 *      None
 */
ret_t l2sw_setAsicPriorityPortBased(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 priority )
{
    ret_t retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(priority > L2SW_PRIMAX )
        return L2SW_ERR_QOS_INT_PRIORITY;

    if(port < 8)
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_QOS_PORTBASED_PRIORITY_REG(port), L2SW_QOS_PORTBASED_PRIORITY_MASK(port), priority);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_QOS_PORTBASED_PRIORITY_CTRL2, 0x7 << ((port - 8) << 2), priority);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicPriorityPortBased
 * Description:
 *      Get port based priority
 * Input:
 *      port         - Physical port number (0~7)
 *      pPriority     - Priority value
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK         - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicPriorityPortBased(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pPriority )
{
    ret_t retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 8)
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_QOS_PORTBASED_PRIORITY_REG(port), L2SW_QOS_PORTBASED_PRIORITY_MASK(port), pPriority);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_QOS_PORTBASED_PRIORITY_CTRL2, 0x7 << ((port - 8) << 2), pPriority);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicPriorityDscpBased
 * Description:
 *      Set DSCP-based priority
 * Input:
 *      dscp         - DSCP value
 *      priority     - Priority value
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                 - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_QOS_DSCP_VALUE    - Invalid DSCP value
 *      L2SW_ERR_QOS_INT_PRIORITY    - Invalid priority
 * Note:
 *      None
 */
ret_t l2sw_setAsicPriorityDscpBased(l2sw_uint8 units, l2sw_uint32 dscp, l2sw_uint32 priority )
{
    if(priority > L2SW_PRIMAX )
        return L2SW_ERR_QOS_INT_PRIORITY;

    if(dscp > L2SW_DSCPMAX)
        return L2SW_ERR_QOS_DSCP_VALUE;

    return l2sw_setAsicRegBits(units, L2SW_QOS_DSCP_TO_PRIORITY_REG(dscp), L2SW_QOS_DSCP_TO_PRIORITY_MASK(dscp), priority);
}
/* Function Name:
 *      l2sw_getAsicPriorityDscpBased
 * Description:
 *      Get DSCP-based priority
 * Input:
 *      dscp         - DSCP value
 *      pPriority     - Priority value
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                 - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_QOS_INT_PRIORITY    - Invalid priority
 * Note:
 *      None
 */
ret_t l2sw_getAsicPriorityDscpBased(l2sw_uint8 units, l2sw_uint32 dscp, l2sw_uint32 *pPriority )
{
    if(dscp > L2SW_DSCPMAX)
        return L2SW_ERR_QOS_DSCP_VALUE;

    return l2sw_getAsicRegBits(units, L2SW_QOS_DSCP_TO_PRIORITY_REG(dscp), L2SW_QOS_DSCP_TO_PRIORITY_MASK(dscp), pPriority);
}
/* Function Name:
 *      l2sw_setAsicPriorityDecision
 * Description:
 *      Set priority decision table
 * Input:
 *      prisrc         - Priority decision source
 *      decisionPri - Decision priority assignment
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                     - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_QOS_INT_PRIORITY        - Invalid priority
 *      L2SW_ERR_QOS_SEL_PRI_SOURCE    - Invalid priority decision source parameter
 * Note:
 *      None
 */
ret_t l2sw_setAsicPriorityDecision(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 prisrc, l2sw_uint32 decisionPri)
{
    ret_t retVal;

    if(index >= PRIDEC_IDX_END )
        return L2SW_ERR_ENTRY_INDEX;

    if(prisrc >= PRIDEC_END )
        return L2SW_ERR_QOS_SEL_PRI_SOURCE;

    if(decisionPri > L2SW_DECISIONPRIMAX )
        return L2SW_ERR_QOS_INT_PRIORITY;

    switch(index)
    {
        case PRIDEC_IDX0:
            if((retVal = l2sw_setAsicRegBits(units, L2SW_QOS_INTERNAL_PRIORITY_DECISION_REG(prisrc), L2SW_QOS_INTERNAL_PRIORITY_DECISION_MASK(prisrc), decisionPri))!=  L2SW_ERR_OK)
                return retVal;
            break;
        case PRIDEC_IDX1:
            if((retVal = l2sw_setAsicRegBits(units, L2SW_QOS_INTERNAL_PRIORITY_DECISION2_REG(prisrc), L2SW_QOS_INTERNAL_PRIORITY_DECISION2_MASK(prisrc), decisionPri))!=  L2SW_ERR_OK)
                return retVal;
            break;
        default:
            break;
    };

    return L2SW_ERR_OK;


}

/* Function Name:
 *      l2sw_getAsicPriorityDecision
 * Description:
 *      Get priority decision table
 * Input:
 *      prisrc         - Priority decision source
 *      pDecisionPri - Decision priority assignment
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                     - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_QOS_SEL_PRI_SOURCE    - Invalid priority decision source parameter
 * Note:
 *      None
 */
ret_t l2sw_getAsicPriorityDecision(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 prisrc, l2sw_uint32* pDecisionPri)
{
    ret_t retVal;

    if(index >= PRIDEC_IDX_END )
        return L2SW_ERR_ENTRY_INDEX;

    if(prisrc >= PRIDEC_END )
        return L2SW_ERR_QOS_SEL_PRI_SOURCE;

    switch(index)
    {
        case PRIDEC_IDX0:
            if((retVal = l2sw_getAsicRegBits(units, L2SW_QOS_INTERNAL_PRIORITY_DECISION_REG(prisrc), L2SW_QOS_INTERNAL_PRIORITY_DECISION_MASK(prisrc), pDecisionPri))!=  L2SW_ERR_OK)
                return retVal;
            break;
        case PRIDEC_IDX1:
            if((retVal = l2sw_getAsicRegBits(units, L2SW_QOS_INTERNAL_PRIORITY_DECISION2_REG(prisrc), L2SW_QOS_INTERNAL_PRIORITY_DECISION2_MASK(prisrc), pDecisionPri))!=  L2SW_ERR_OK)
                return retVal;
            break;
        default:
            break;
    };

    return L2SW_ERR_OK;

}

/* Function Name:
 *      l2sw_setAsicPortPriorityDecisionIndex
 * Description:
 *      Set priority decision index for each port
 * Input:
 *      port     - Physical port number (0~7)
 *      index     - Table index
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK             - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_QUEUE_NUM      - Invalid queue number
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortPriorityDecisionIndex(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 index )
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(index >= PRIDEC_IDX_END)
        return L2SW_ERR_ENTRY_INDEX;

    return l2sw_setAsicRegBit(units, L2SW_QOS_INTERNAL_PRIORITY_DECISION_IDX_CTRL, port, index);
}
/* Function Name:
 *      l2sw_getAsicPortPriorityDecisionIndex
 * Description:
 *      Get priority decision index  for each port
 * Input:
 *      port     - Physical port number (0~7)
 *      pIndex     - Table index
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK             - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortPriorityDecisionIndex(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pIndex )
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBit(units, L2SW_QOS_INTERNAL_PRIORITY_DECISION_IDX_CTRL, port, pIndex);
}

/* Function Name:
 *      l2sw_setAsicOutputQueueMappingIndex
 * Description:
 *      Set output queue number for each port
 * Input:
 *      port     - Physical port number (0~7)
 *      index     - Mapping table index
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK             - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_QUEUE_NUM      - Invalid queue number
 * Note:
 *      None
 */
ret_t l2sw_setAsicOutputQueueMappingIndex(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 index )
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(index >= L2SW_QUEUENO)
        return L2SW_ERR_QUEUE_NUM;

    return l2sw_setAsicRegBits(units, L2SW_QOS_PORT_QUEUE_NUMBER_REG(port), L2SW_QOS_PORT_QUEUE_NUMBER_MASK(port), index);
}
/* Function Name:
 *      l2sw_getAsicOutputQueueMappingIndex
 * Description:
 *      Get output queue number for each port
 * Input:
 *      port     - Physical port number (0~7)
 *      pIndex     - Mapping table index
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK             - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicOutputQueueMappingIndex(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pIndex )
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBits(units, L2SW_QOS_PORT_QUEUE_NUMBER_REG(port), L2SW_QOS_PORT_QUEUE_NUMBER_MASK(port), pIndex);
}
/* Function Name:
 *      l2sw_setAsicPriorityToQIDMappingTable
 * Description:
 *      Set priority to QID mapping table parameters
 * Input:
 *      index         - Mapping table index
 *      priority     - The priority value
 *      qid         - Queue id
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                 - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_QUEUE_ID          - Invalid queue id
 *      L2SW_ERR_QUEUE_NUM          - Invalid queue number
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority
 * Note:
 *      None
 */
ret_t l2sw_setAsicPriorityToQIDMappingTable(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 priority, l2sw_uint32 qid )
{
    if(index >= L2SW_QUEUENO)
        return L2SW_ERR_QUEUE_NUM;

    if(priority > L2SW_PRIMAX)
        return L2SW_ERR_QOS_INT_PRIORITY;

    if(qid > L2SW_QIDMAX)
        return L2SW_ERR_QUEUE_ID;

    return l2sw_setAsicRegBits(units, L2SW_QOS_1Q_PRIORITY_TO_QID_REG(index, priority), L2SW_QOS_1Q_PRIORITY_TO_QID_MASK(priority), qid);
}
/* Function Name:
 *      l2sw_getAsicPriorityToQIDMappingTable
 * Description:
 *      Get priority to QID mapping table parameters
 * Input:
 *      index         - Mapping table index
 *      priority     - The priority value
 *      pQid         - Queue id
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                 - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_QUEUE_NUM          - Invalid queue number
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority
 * Note:
 *      None
 */
ret_t l2sw_getAsicPriorityToQIDMappingTable(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 priority, l2sw_uint32* pQid)
{
    if(index >= L2SW_QUEUENO)
        return L2SW_ERR_QUEUE_NUM;

    if(priority > L2SW_PRIMAX)
        return L2SW_ERR_QOS_INT_PRIORITY;

    return l2sw_getAsicRegBits(units, L2SW_QOS_1Q_PRIORITY_TO_QID_REG(index, priority), L2SW_QOS_1Q_PRIORITY_TO_QID_MASK(priority), pQid);
}
/* Function Name:
 *      l2sw_setAsicRemarkingDot1pAbility
 * Description:
 *      Set 802.1p remarking ability
 * Input:
 *      port     - Physical port number (0~7)
 *      enabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK             - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsicRemarkingDot1pAbility(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_PORT_MISC_CFG_REG(port), L2SW_1QREMARK_ENABLE_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicRemarkingDot1pAbility
 * Description:
 *      Get 802.1p remarking ability
 * Input:
 *      port     - Physical port number (0~7)
 *      pEnabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK     - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicRemarkingDot1pAbility(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_PORT_MISC_CFG_REG(port), L2SW_1QREMARK_ENABLE_OFFSET, pEnabled);
}
/* Function Name:
 *      l2sw_setAsicRemarkingDot1pParameter
 * Description:
 *      Set 802.1p remarking parameter
 * Input:
 *      priority     - Priority value
 *      newPriority - New priority value
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                 - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority
 * Note:
 *      None
 */
ret_t l2sw_setAsicRemarkingDot1pParameter(l2sw_uint8 units, l2sw_uint32 priority, l2sw_uint32 newPriority )
{
    if(priority > L2SW_PRIMAX || newPriority > L2SW_PRIMAX)
        return L2SW_ERR_QOS_INT_PRIORITY;

    return l2sw_setAsicRegBits(units, L2SW_QOS_1Q_REMARK_REG(priority), L2SW_QOS_1Q_REMARK_MASK(priority), newPriority);
}
/* Function Name:
 *      l2sw_getAsicRemarkingDot1pParameter
 * Description:
 *      Get 802.1p remarking parameter
 * Input:
 *      priority     - Priority value
 *      pNewPriority - New priority value
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                 - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority
 * Note:
 *      None
 */
ret_t l2sw_getAsicRemarkingDot1pParameter(l2sw_uint8 units, l2sw_uint32 priority, l2sw_uint32 *pNewPriority )
{
    if(priority > L2SW_PRIMAX )
        return L2SW_ERR_QOS_INT_PRIORITY;

    return l2sw_getAsicRegBits(units, L2SW_QOS_1Q_REMARK_REG(priority), L2SW_QOS_1Q_REMARK_MASK(priority), pNewPriority);
}

/* Function Name:
 *      l2sw_setAsicRemarkingDot1pSrc
 * Description:
 *      Set remarking source of 802.1p remarking.
 * Input:
 *      type      - remarking source
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT         - The module is not initial
 *      L2SW_ERR_PORT_ID  - invalid port id
 *      L2SW_ERR_INPUT            - invalid input parameter

 * Note:
 *      The API can configure 802.1p remark functionality to map original DSCP value or internal
 *      priority to TX DSCP value.
 */
ret_t l2sw_setAsicRemarkingDot1pSrc(l2sw_uint8 units, l2sw_uint32 type)
{

    if(type >= DOT1P_PRISEL_END )
        return L2SW_ERR_QOS_SEL_PRI_SOURCE;

    return l2sw_setAsicRegBit(units, L2SW_REG_RMK_CFG_SEL_CTRL, L2SW_RMK_1Q_CFG_SEL_OFFSET, type);
}


/* Function Name:
 *      l2sw_getAsicRemarkingDot1pSrc
 * Description:
 *      Get remarking source of 802.1p remarking.
 * Output:
 *      pType      - remarking source
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT         - The module is not initial
 *      L2SW_ERR_PORT_ID          - invalid port id
 *      L2SW_ERR_INPUT            - invalid input parameter
 *      L2SW_ERR_NULL_POINTER     - input parameter may be null pointer

 * Note:
 *      None
 */
ret_t l2sw_getAsicRemarkingDot1pSrc(l2sw_uint8 units, l2sw_uint32 *pType)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_RMK_CFG_SEL_CTRL, L2SW_RMK_1Q_CFG_SEL_OFFSET, pType);
}





/* Function Name:
 *      l2sw_setAsicRemarkingDscpAbility
 * Description:
 *      Set DSCP remarking ability
 * Input:
 *      enabled     - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK     - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicRemarkingDscpAbility(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REMARKING_CTRL_REG, L2SW_REMARKING_DSCP_ENABLE_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicRemarkingDscpAbility
 * Description:
 *      Get DSCP remarking ability
 * Input:
 *      enabled     - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK     - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicRemarkingDscpAbility(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REMARKING_CTRL_REG, L2SW_REMARKING_DSCP_ENABLE_OFFSET, pEnabled);
}
/* Function Name:
 *      l2sw_setAsicRemarkingDscpParameter
 * Description:
 *      Set DSCP remarking parameter
 * Input:
 *      priority     - Priority value
 *      newDscp     - New DSCP value
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                 - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_QOS_DSCP_VALUE    - Invalid DSCP value
 *      L2SW_ERR_QOS_INT_PRIORITY    - Invalid priority
 * Note:
 *      None
 */
ret_t l2sw_setAsicRemarkingDscpParameter(l2sw_uint8 units, l2sw_uint32 priority, l2sw_uint32 newDscp )
{
    if(priority > L2SW_PRIMAX )
        return L2SW_ERR_QOS_INT_PRIORITY;

    if(newDscp > L2SW_DSCPMAX)
        return L2SW_ERR_QOS_DSCP_VALUE;

    return l2sw_setAsicRegBits(units, L2SW_QOS_DSCP_REMARK_REG(priority), L2SW_QOS_DSCP_REMARK_MASK(priority), newDscp);
}
/* Function Name:
 *      l2sw_getAsicRemarkingDscpParameter
 * Description:
 *      Get DSCP remarking parameter
 * Input:
 *      priority     - Priority value
 *      pNewDscp     - New DSCP value
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                 - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_QOS_INT_PRIORITY    - Invalid priority
 * Note:
 *      None
 */
ret_t l2sw_getAsicRemarkingDscpParameter(l2sw_uint8 units, l2sw_uint32 priority, l2sw_uint32* pNewDscp )
{
    if(priority > L2SW_PRIMAX )
        return L2SW_ERR_QOS_INT_PRIORITY;

    return l2sw_getAsicRegBits(units, L2SW_QOS_DSCP_REMARK_REG(priority), L2SW_QOS_DSCP_REMARK_MASK(priority), pNewDscp);
}

/* Function Name:
 *      l2sw_setAsicRemarkingDscpSrc
 * Description:
 *      Set remarking source of DSCP remarking.
 * Input:
 *      type      - remarking source
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT         - The module is not initial
 *      L2SW_ERR_PORT_ID  - invalid port id
 *      L2SW_ERR_INPUT            - invalid input parameter

 * Note:
 *      The API can configure DSCP remark functionality to map original DSCP value or internal
 *      priority to TX DSCP value.
 */
ret_t l2sw_setAsicRemarkingDscpSrc(l2sw_uint8 units, l2sw_uint32 type)
{

    if(type >= DSCP_PRISEL_END )
        return L2SW_ERR_QOS_SEL_PRI_SOURCE;

    return l2sw_setAsicRegBits(units, L2SW_REG_RMK_CFG_SEL_CTRL, L2SW_RMK_DSCP_CFG_SEL_MASK, type);
}


/* Function Name:
 *      l2sw_getAsicRemarkingDscpSrc
 * Description:
 *      Get remarking source of DSCP remarking.
 * Output:
 *      pType      - remarking source
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT         - The module is not initial
 *      L2SW_ERR_PORT_ID          - invalid port id
 *      L2SW_ERR_INPUT            - invalid input parameter
 *      L2SW_ERR_NULL_POINTER     - input parameter may be null pointer

 * Note:
 *      None
 */
ret_t l2sw_getAsicRemarkingDscpSrc(l2sw_uint8 units, l2sw_uint32 *pType)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_RMK_CFG_SEL_CTRL, L2SW_RMK_DSCP_CFG_SEL_MASK, pType);
}

/* Function Name:
 *      l2sw_setAsicRemarkingDscp2Dscp
 * Description:
 *      Set DSCP to remarked DSCP mapping.
 * Input:
 *      dscp    - DSCP value
 *      rmkDscp - remarked DSCP value
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_UNIT_ID          - Invalid unit id
 *      L2SW_ERR_QOS_DSCP_VALUE   - Invalid dscp value
 * Note:
 *      dscp parameter can be DSCP value or internal priority according to configuration of API
 *      dal_apollomp_qos_dscpRemarkSrcSel_set(), because DSCP remark functionality can map original DSCP
 *      value or internal priority to TX DSCP value.
 */
ret_t l2sw_setAsicRemarkingDscp2Dscp(l2sw_uint8 units, l2sw_uint32 dscp, l2sw_uint32 rmkDscp)
{
    if((dscp > L2SW_DSCPMAX ) || (rmkDscp > L2SW_DSCPMAX))
        return L2SW_ERR_QOS_INT_PRIORITY;

    return l2sw_setAsicRegBits(units, L2SW_QOS_DSCP_TO_DSCP_REG(dscp), L2SW_QOS_DSCP_TO_DSCP_MASK(dscp), rmkDscp);
}

/* Function Name:
 *      l2sw_getAsicRemarkingDscp2Dscp
 * Description:
 *      Get DSCP to remarked DSCP mapping.
 * Input:
 *      dscp    - DSCP value
 * Output:
 *      pRmkDscp   - remarked DSCP value
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_QOS_DSCP_VALUE   - Invalid dscp value
 *      L2SW_ERR_NULL_POINTER     - NULL pointer
 * Note:
 *      None.
 */
ret_t l2sw_getAsicRemarkingDscp2Dscp(l2sw_uint8 units, l2sw_uint32 dscp, l2sw_uint32 *pRmkDscp)
{
    if(dscp > L2SW_DSCPMAX)
        return L2SW_ERR_QOS_DSCP_VALUE;

    return l2sw_getAsicRegBits(units, L2SW_QOS_DSCP_TO_DSCP_REG(dscp), L2SW_QOS_DSCP_TO_DSCP_MASK(dscp), pRmkDscp);

}

