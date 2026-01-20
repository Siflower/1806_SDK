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

#include "l2sw_switch.h"
#include "sal_string.h"
#include "l2sw_asicdrv_fc.h"


/* Function Name:
 *      l2sw_Flow_Control_Select_set
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
l2sw_api_ret_t l2sw_Flow_Control_Select_set(l2sw_uint8 units, l2sw_uint32 select)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlSelect(units, select);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_Select_get
 * Description:
 *      Get system flow control type
 * Input:
 *      pSelect     - System flow control type 1: Ingress flow control 0:Egress flow control
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_Flow_Control_Select_get(l2sw_uint8 units, l2sw_uint32 *pSelect)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlSelect(units, pSelect);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_JumboMode_set
 * Description:
 *      Set Jumbo threhsold for flow control
 * Input:
 *      enabled     - Jumbo mode flow control 1: Enable 0:Disable
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_Flow_Control_JumboMode_set(l2sw_uint8 units, l2sw_uint32 enabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlJumboMode(units, enabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_JumboMode_get
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
l2sw_api_ret_t l2sw_Flow_Control_JumboMode_get(l2sw_uint8 units, l2sw_uint32 *pEnabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlJumboMode(units, pEnabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_JumboMode_Size_set
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
l2sw_api_ret_t l2sw_Flow_Control_JumboMode_Size_set(l2sw_uint8 units, l2sw_uint32 size)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlJumboModeSize(units, size);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_JumboMode_Size_get
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
l2sw_api_ret_t l2sw_Flow_Control_JumboMode_Size_get(l2sw_uint8 units, l2sw_uint32 *pSize)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlJumboModeSize(units, pSize);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_Queue_Egress_Enable_set
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
l2sw_api_ret_t l2sw_Flow_Control_Queue_Egress_Enable_set(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 enabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlQueueEgressEnable(units, port, qid, enabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_Queue_Egress_Enable_get
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
l2sw_api_ret_t l2sw_Flow_Control_Queue_Egress_Enable_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 *pEabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlQueueEgressEnable(units, port, qid, pEabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_Drop_All_set
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
l2sw_api_ret_t l2sw_Flow_Control_Drop_All_set(l2sw_uint8 units, l2sw_uint32 dropall)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlDropAll(units, dropall);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_Drop_All_get
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
l2sw_api_ret_t l2sw_Flow_Control_Drop_All_get(l2sw_uint8 units, l2sw_uint32 *pDropall)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlDropAll(units, pDropall);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_Pause_All_set
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
l2sw_api_ret_t l2sw_Flow_Control_Pause_All_set(l2sw_uint8 units, l2sw_uint32 threshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlPauseAllThreshold(units, threshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_Pause_All_get
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
l2sw_api_ret_t l2sw_Flow_Control_Pause_All_get(l2sw_uint8 units, l2sw_uint32 *pThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlPauseAllThreshold(units, pThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_System_Threshold_set
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
l2sw_api_ret_t l2sw_Flow_Control_System_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlSystemThreshold(units, onThreshold, offThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_System_Threshold_get
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
l2sw_api_ret_t l2sw_Flow_Control_System_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlSystemThreshold(units, pOnThreshold, pOffThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_Shared_Threshold_set
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
l2sw_api_ret_t l2sw_Flow_Control_Shared_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlSharedThreshold(units, onThreshold, offThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_Shared_Threshold_get
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
l2sw_api_ret_t l2sw_Flow_Control_Shared_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlSharedThreshold(units, pOnThreshold, pOffThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_Port_Threshold_set
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
l2sw_api_ret_t l2sw_Flow_Control_Port_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlPortThreshold(units, onThreshold, offThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_Port_Threshold_get
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
l2sw_api_ret_t l2sw_Flow_Control_Port_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlPortThreshold(units, pOnThreshold, pOffThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_PortPrivate_Threshold_set
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
l2sw_api_ret_t l2sw_Flow_Control_PortPrivate_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlPortPrivateThreshold(units, onThreshold, offThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_PortPrivate_Threshold_get
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
l2sw_api_ret_t l2sw_Flow_Control_PortPrivate_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlPortPrivateThreshold(units, pOnThreshold, pOffThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_SystemDrop_Threshold_set
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
l2sw_api_ret_t l2sw_Flow_Control_SystemDrop_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlSystemDropThreshold(units, onThreshold, offThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_SystemDrop_Threshold_get
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
l2sw_api_ret_t l2sw_Flow_Control_SystemDrop_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlSystemDropThreshold(units, pOnThreshold, pOffThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}


/* Function Name:
 *      l2sw_Flow_Control_SharedDrop_Threshold_set
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
l2sw_api_ret_t l2sw_Flow_Control_SharedDrop_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlSharedDropThreshold(units, onThreshold, offThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_SharedDrop_Threshold_get
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
l2sw_api_ret_t l2sw_Flow_Control_SharedDrop_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlSharedDropThreshold(units, pOnThreshold, pOffThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_PortDrop_Threshold_set
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
l2sw_api_ret_t l2sw_Flow_Control_PortDrop_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlPortDropThreshold(units, onThreshold, offThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_PortDrop_Threshold_get
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
l2sw_api_ret_t l2sw_Flow_Control_PortDrop_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlPortDropThreshold(units, pOnThreshold, pOffThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_PortPrivate_Drop_Threshold_set
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
l2sw_api_ret_t l2sw_Flow_Control_PortPrivate_Drop_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlPortPrivateDropThreshold(units, onThreshold, offThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_PortPrivate_Drop_Threshold_get
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
l2sw_api_ret_t l2sw_Flow_Control_PortPrivate_Drop_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlPortPrivateDropThreshold(units, pOnThreshold, pOffThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_SystemJumbo_Threshold_set
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
l2sw_api_ret_t l2sw_Flow_Control_SystemJumbo_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlSystemJumboThreshold(units, onThreshold, offThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_SystemJumbo_Threshold_get
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
l2sw_api_ret_t l2sw_Flow_Control_SystemJumbo_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlSystemJumboThreshold(units, pOnThreshold, pOffThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}


/* Function Name:
 *      l2sw_Flow_Control_SharedJumbo_Threshold_set
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
l2sw_api_ret_t l2sw_Flow_Control_SharedJumbo_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlSharedJumboThreshold(units, onThreshold, offThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_SharedJumbo_Threshold_get
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
l2sw_api_ret_t l2sw_Flow_Control_SharedJumbo_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlSharedJumboThreshold(units, pOnThreshold, pOffThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_PortJumbo_Threshold_set
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
l2sw_api_ret_t l2sw_Flow_Control_PortJumbo_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlPortJumboThreshold(units, onThreshold, offThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_PortJumbo_Threshold_get
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
l2sw_api_ret_t l2sw_Flow_Control_PortJumbo_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlPortJumboThreshold(units, pOnThreshold, pOffThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_PortPrivate_Jumbo_Threshold_set
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
l2sw_api_ret_t l2sw_Flow_Control_PortPrivate_Jumbo_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlPortPrivateJumboThreshold(units, onThreshold, offThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_PortPrivate_Jumbo_Threshold_get
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
l2sw_api_ret_t l2sw_Flow_Control_PortPrivate_Jumbo_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlPortPrivateJumboThreshold(units, pOnThreshold, pOffThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_EgressFlow_Control_Queue_Drop_Threshold_set
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
l2sw_api_ret_t l2sw_EgressFlow_Control_Queue_Drop_Threshold_set(l2sw_uint8 units, l2sw_uint32 qid, l2sw_uint32 threshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicEgressFlowControlQueueDropThreshold(units, qid, threshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_EgressFlow_Control_Queue_Drop_Threshold_get
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
l2sw_api_ret_t l2sw_EgressFlow_Control_Queue_Drop_Threshold_get(l2sw_uint8 units, l2sw_uint32 qid, l2sw_uint32 *pThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicEgressFlowControlQueueDropThreshold(units, qid, pThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_EgressFlow_Control_Port_Drop_Threshold_set
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
l2sw_api_ret_t l2sw_EgressFlow_Control_Port_Drop_Threshold_set(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 threshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicEgressFlowControlPortDropThreshold(units, port, threshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_EgressFlow_Control_Port_Drop_Threshold_get
 * Description:
 *      Get port-based egress flow control turn on or ingress flow control drop on threshold
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
l2sw_api_ret_t l2sw_EgressFlow_Control_Port_Drop_Threshold_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pThreshold)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicEgressFlowControlPortDropThreshold(units, port, pThreshold);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_EgressFlow_Control_Port_Drop_Gap_set
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
l2sw_api_ret_t l2sw_EgressFlow_Control_Port_Drop_Gap_set(l2sw_uint8 units, l2sw_uint32 gap)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicEgressFlowControlPortDropGap(units, gap);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_EgressFlow_Control_Port_Drop_Gap_get
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
l2sw_api_ret_t l2sw_EgressFlow_Control_Port_Drop_Gap_get(l2sw_uint8 units, l2sw_uint32 *pGap)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicEgressFlowControlPortDropGap(units, pGap);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_EgressFlow_Control_Queue_Drop_Gap_set
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
l2sw_api_ret_t l2sw_EgressFlow_Control_Queue_Drop_Gap_set(l2sw_uint8 units, l2sw_uint32 gap)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicEgressFlowControlQueueDropGap(units, gap);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_EgressFlow_Control_Queue_Drop_Gap_get
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
l2sw_api_ret_t l2sw_EgressFlow_Control_Queue_Drop_Gap_get(l2sw_uint8 units, l2sw_uint32 *pGap)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicEgressFlowControlQueueDropGap(units, pGap);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Egress_QueueEmpty_PortMask_get
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
l2sw_api_ret_t l2sw_Egress_QueueEmpty_PortMask_get(l2sw_uint8 units, l2sw_uint32 *pPortmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicEgressQueueEmptyPortMask(units, pPortmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Total_Page_get
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
l2sw_api_ret_t l2sw_Total_Page_get(l2sw_uint8 units, l2sw_uint32 *pPageCount)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicTotalPage(units, pPageCount);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Pulbic_Page_get
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
l2sw_api_ret_t l2sw_Pulbic_Page_get(l2sw_uint8 units, l2sw_uint32 *pPageCount)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicPulbicPage(units, pPageCount);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_MaxTotal_Page_get
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
l2sw_api_ret_t l2sw_MaxTotal_Page_get(l2sw_uint8 units, l2sw_uint32 *pPageCount)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicMaxTotalPage(units, pPageCount);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_MaxPulbic_Page_get
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
l2sw_api_ret_t l2sw_MaxPulbic_Page_get(l2sw_uint8 units, l2sw_uint32 *pPageCount)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicMaxPulbicPage(units, pPageCount);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Port_Page_get
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
l2sw_api_ret_t l2sw_Port_Page_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pPageCount)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicPortPage(units, port, pPageCount);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Port_PageMax_get
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
l2sw_api_ret_t l2sw_Port_PageMax_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pPageCount)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicPortPageMax(units, port, pPageCount);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_Flow_Control_Egress_Port_Indep_set
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
l2sw_api_ret_t l2sw_Flow_Control_Egress_Port_Indep_set(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_setAsicFlowControlEgressPortIndep(units, port, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}


/* Function Name:
 *      l2sw_Flow_Control_Egress_Port_Indep_get
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
l2sw_api_ret_t l2sw_Flow_Control_Egress_Port_Indep_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicFlowControlEgressPortIndep(units, port, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

