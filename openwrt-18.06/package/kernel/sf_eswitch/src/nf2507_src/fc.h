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

#ifndef __L2SW_API_FC_H__
#define __L2SW_API_FC_H__

#include "l2sw_asicdrv.h"

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
extern l2sw_api_ret_t l2sw_Flow_Control_Select_set(l2sw_uint8 units, l2sw_uint32 select);


/* Function Name:
 *      l2sw_Flow_Control_Select_get
 * Description:
 *      Get system flow control type
 * Input:
 *      None
 * Output:
 *      pSelect     - System flow control type 1: Ingress flow control 0:Egress flow control
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_Select_get(l2sw_uint8 units, l2sw_uint32 *pSelect);

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
extern l2sw_api_ret_t l2sw_Flow_Control_JumboMode_set(l2sw_uint8 units, l2sw_uint32 enabled);

/* Function Name:
 *      l2sw_Flow_Control_JumboMode_get
 * Description:
 *      Get Jumbo threhsold for flow control
 * Input:
 *      None
 * Output:
 *      pEnabled        - Jumbo mode flow control 1: Enable 0:Disable
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_JumboMode_get(l2sw_uint8 units, l2sw_uint32 *pEnabled);

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
extern l2sw_api_ret_t l2sw_Flow_Control_JumboMode_Size_set(l2sw_uint8 units, l2sw_uint32 size);

/* Function Name:
 *      l2sw_Flow_Control_JumboMode_Size_get
 * Description:
 *      Get Jumbo size for Jumbo mode flow control
 * Input:
 *      None
 * Output:
 *      pSize       - Jumbo size 0:3Kbytes 1:4Kbytes 2:6Kbytes 3:9Kbytes
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_JumboMode_Size_get(l2sw_uint8 units, l2sw_uint32 *pSize);

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
extern l2sw_api_ret_t l2sw_Flow_Control_Queue_Egress_Enable_set(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 enabled);

/* Function Name:
 *      l2sw_Flow_Control_Queue_Egress_Enable_get
 * Description:
 *      Get flow control ability for each queue
 * Input:
 *      port    - Physical port number (0~7)
 *      qid     - Queue id
 * Output:
 *      pEnabled - 1: enabled, 0: disabled
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 *      L2SW_ERR_QUEUE_ID - Invalid queue id
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_Queue_Egress_Enable_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 *pEabled);

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
extern l2sw_api_ret_t l2sw_Flow_Control_Drop_All_set(l2sw_uint8 units, l2sw_uint32 dropall);

/* Function Name:
 *      l2sw_Flow_Control_Drop_All_get
 * Description:
 *      Get system-based drop parameters
 * Input:
 *      None
 * Output:
 *      pDropall    - Whole system drop threshold
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_Drop_All_get(l2sw_uint8 units, l2sw_uint32 *pDropall);

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
extern l2sw_api_ret_t l2sw_Flow_Control_Pause_All_set(l2sw_uint8 units, l2sw_uint32 threshold);

/* Function Name:
 *      l2sw_Flow_Control_Pause_All_get
 * Description:
 *      Get system-based all ports enable flow control parameters
 * Input:
 *      None
 * Output:
 *      pThreshold  - Whole system pause all threshold
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_Pause_All_get(l2sw_uint8 units, l2sw_uint32 *pThreshold);

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
extern l2sw_api_ret_t l2sw_Flow_Control_System_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);

/* Function Name:
 *      l2sw_Flow_Control_System_Threshold_get
 * Description:
 *      Get system-based flow control parameters
 * Input:
 *      None
 * Output:
 *      pOnThreshold    - Flow control turn ON threshold
 *      pOffThreshold   - Flow control turn OFF threshold
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_System_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);

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
extern l2sw_api_ret_t l2sw_Flow_Control_Shared_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);

/* Function Name:
 *      l2sw_Flow_Control_Shared_Threshold_get
 * Description:
 *      Get share-based flow control parameters
 * Input:
 *      None
 * Output:
 *      pOnThreshold    - Flow control turn ON threshold
 *      pOffThreshold   - Flow control turn OFF threshold
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_Shared_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);

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
extern l2sw_api_ret_t l2sw_Flow_Control_Port_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);

/* Function Name:
 *      l2sw_Flow_Control_Port_Threshold_get
 * Description:
 *      Get Port-based flow control parameters
 * Input:
 *      None
 * Output:
 *      pOnThreshold    - Flow control turn ON threshold
 *      pOffThreshold   - Flow control turn OFF threshold
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_Port_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);

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
extern l2sw_api_ret_t l2sw_Flow_Control_PortPrivate_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);

/* Function Name:
 *      l2sw_Flow_Control_PortPrivate_Threshold_get
 * Description:
 *      Get Port-private-based flow control parameters
 * Input:
 *      None
 * Output:
 *      pOnThreshold    - Flow control turn ON threshold
 *      pOffThreshold   - Flow control turn OFF threshold
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_PortPrivate_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);

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
extern l2sw_api_ret_t l2sw_Flow_Control_SystemDrop_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);

/* Function Name:
 *      l2sw_Flow_Control_SystemDrop_Threshold_get
 * Description:
 *      Get system-based drop parameters
 * Input:
 *      None
 * Output:
 *      pOnThreshold    - Drop turn ON threshold
 *      pOffThreshold   - Drop turn OFF threshold
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_SystemDrop_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);

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
extern l2sw_api_ret_t l2sw_Flow_Control_SharedDrop_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);

/* Function Name:
 *      l2sw_Flow_Control_SharedDrop_Threshold_get
 * Description:
 *      Get share-based fdrop parameters
 * Input:
 *      None
 * Output:
 *      pOnThreshold    - Drop turn ON threshold
 *      pOffThreshold   - Drop turn OFF threshold
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_SharedDrop_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);

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
extern l2sw_api_ret_t l2sw_Flow_Control_PortDrop_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);

/* Function Name:
 *      l2sw_Flow_Control_PortDrop_Threshold_get
 * Description:
 *      Get Port-based drop parameters
 * Input:
 *      None
 * Output:
 *      pOnThreshold    - Drop turn ON threshold
 *      pOffThreshold   - Drop turn OFF threshold
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_PortDrop_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);

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
extern l2sw_api_ret_t l2sw_Flow_Control_PortPrivate_Drop_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);

/* Function Name:
 *      l2sw_Flow_Control_PortPrivate_Drop_Threshold_get
 * Description:
 *      Get Port-private-based drop parameters
 * Input:
 *      None
 * Output:
 *      pOnThreshold    - Drop turn ON threshold
 *      pOffThreshold   - Drop turn OFF threshold
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_PortPrivate_Drop_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);

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
extern l2sw_api_ret_t l2sw_Flow_Control_SystemJumbo_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);

/* Function Name:
 *      l2sw_Flow_Control_SystemJumbo_Threshold_get
 * Description:
 *      Get Jumbo system-based flow control parameters
 * Input:
 *      None
 * Output:
 *      pOnThreshold    - Flow control turn ON threshold
 *      pOffThreshold   - Flow control turn OFF threshold
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_SystemJumbo_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);

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
extern l2sw_api_ret_t l2sw_Flow_Control_SharedJumbo_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);

/* Function Name:
 *      l2sw_Flow_Control_SharedJumbo_Threshold_get
 * Description:
 *      Get Jumbo share-based flow control parameters
 * Input:
 *      None
 * Output:
 *      pOnThreshold    - Flow control turn ON threshold
 *      pOffThreshold   - Flow control turn OFF threshold
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_SharedJumbo_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);

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
extern l2sw_api_ret_t l2sw_Flow_Control_PortJumbo_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);

/* Function Name:
 *      l2sw_Flow_Control_PortJumbo_Threshold_get
 * Description:
 *      Get Jumbo Port-based flow control parameters
 * Input:
 *      None
 * Output:
 *      pOnThreshold    - Flow control turn ON threshold
 *      pOffThreshold   - Flow control turn OFF threshold
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_PortJumbo_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);

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
extern l2sw_api_ret_t l2sw_Flow_Control_PortPrivate_Jumbo_Threshold_set(l2sw_uint8 units, l2sw_uint32 onThreshold, l2sw_uint32 offThreshold);

/* Function Name:
 *      l2sw_Flow_Control_PortPrivate_Jumbo_Threshold_get
 * Description:
 *      Get Jumbo Port-private-based flow control parameters
 * Input:
 *      None
 * Output:
 *      pOnThreshold    - Flow control turn ON threshold
 *      pOffThreshold   - Flow control turn OFF threshold
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_PortPrivate_Jumbo_Threshold_get(l2sw_uint8 units, l2sw_uint32 *pOnThreshold, l2sw_uint32 *pOffThreshold);

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
extern l2sw_api_ret_t l2sw_EgressFlow_Control_Queue_Drop_Threshold_set(l2sw_uint8 units, l2sw_uint32 qid, l2sw_uint32 threshold);

/* Function Name:
 *      l2sw_EgressFlow_Control_Queue_Drop_Threshold_get
 * Description:
 *      Get Queue-based egress flow control turn on or ingress flow control drop on threshold
 * Input:
 *      qid         - The queue id
 * Output:
 *      pThreshold  - Queue-based flown control/drop turn ON threshold
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_QUEUE_ID     - Invalid queue id
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_EgressFlow_Control_Queue_Drop_Threshold_get(l2sw_uint8 units, l2sw_uint32 qid, l2sw_uint32 *pThreshold);

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
extern l2sw_api_ret_t l2sw_EgressFlow_Control_Port_Drop_Threshold_set(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 threshold);

/* Function Name:
 *      l2sw_EgressFlow_Control_Port_Drop_Threshold_get
 * Description:
 *      Get port-based egress flow control turn on or ingress flow control drop on threshold
 * Input:
 *      port        - Physical port number (0~7)
 * Output:
 *      pThreshold  - Queue-based flown control/drop turn ON threshold
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_EgressFlow_Control_Port_Drop_Threshold_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pThreshold);

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
extern l2sw_api_ret_t l2sw_EgressFlow_Control_Port_Drop_Gap_set(l2sw_uint8 units, l2sw_uint32 gap);

/* Function Name:
 *      l2sw_EgressFlow_Control_Port_Drop_Gap_get
 * Description:
 *      Get port-based egress flow control turn off or ingress flow control drop off gap
 * Input:
 *      None
 * Output:
 *      pGap    - Flow control/drop turn OFF threshold = turn ON threshold - gap
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_EgressFlow_Control_Port_Drop_Gap_get(l2sw_uint8 units, l2sw_uint32 *pGap);

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
extern l2sw_api_ret_t l2sw_EgressFlow_Control_Queue_Drop_Gap_set(l2sw_uint8 units, l2sw_uint32 gap);

/* Function Name:
 *      l2sw_EgressFlow_Control_Queue_Drop_Gap_get
 * Description:
 *      Get Queue-based egress flow control turn off or ingress flow control drop off gap
 * Input:
 *      None
 * Output:
 *      pGap    - Flow control/drop turn OFF threshold = turn ON threshold - gap
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_EgressFlow_Control_Queue_Drop_Gap_get(l2sw_uint8 units, l2sw_uint32 *pGap);

/* Function Name:
 *      l2sw_Egress_QueueEmpty_PortMask_get
 * Description:
 *      Get queue empty port mask
 * Input:
 *      None
 * Output:
 *      pPortmask   -  Queue empty port mask
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Egress_QueueEmpty_PortMask_get(l2sw_uint8 units, l2sw_uint32 *pPortmask);

/* Function Name:
 *      l2sw_Total_Page_get
 * Description:
 *      Get system total page usage number
 * Input:
 *      None
 * Output:
 *      pPageCount  -  page usage number
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Total_Page_get(l2sw_uint8 units, l2sw_uint32 *pPageCount);

/* Function Name:
 *      l2sw_Pulbic_Page_get
 * Description:
 *      Get system public page usage number
 * Input:
 *      None
 * Output:
 *      pPageCount  -  page usage number
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Pulbic_Page_get(l2sw_uint8 units, l2sw_uint32 *pPageCount);

/* Function Name:
 *      l2sw_MaxTotal_Page_get
 * Description:
 *      Get system total page max usage number
 * Input:
 *      None
 * Output:
 *      pPageCount  -  page usage number
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_MaxTotal_Page_get(l2sw_uint8 units, l2sw_uint32 *pPageCount);

/* Function Name:
 *      l2sw_MaxPulbic_Page_get
 * Description:
 *      Get system public page max usage number
 * Input:
 *      None
 * Output:
 *      pPageCount  -  page usage number
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_MaxPulbic_Page_get(l2sw_uint8 units, l2sw_uint32 *pPageCount);

/* Function Name:
 *      l2sw_Port_Page_get
 * Description:
 *      Get per-port page usage number
 * Input:
 *      port        -  Physical port number (0~7)
 * Output:
 *      pPageCount  -  page usage number
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Port_Page_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pPageCount);

/* Function Name:
 *      l2sw_Port_PageMax_get
 * Description:
 *      Get per-port page max usage number
 * Input:
 *      port        -  Physical port number (0~7)
 * Output:
 *      pPageCount  -  page usage number
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Port_PageMax_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pPageCount);

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
extern l2sw_api_ret_t l2sw_Flow_Control_Egress_Port_Indep_set(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enable);

/* Function Name:
 *      l2sw_Flow_Control_Egress_Port_Indep_get
 * Description:
 *      Set per-port egress flow control independent
 * Input:
 *      port        - Physical port number (0~7)
 * Output:
 *      enabled     - Egress port flow control usage 1:enable 0:disable.
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_Flow_Control_Egress_Port_Indep_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnable);


#endif /* __L2SW_API_FC_H__ */
