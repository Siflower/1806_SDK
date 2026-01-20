/**  @file *  @brief    brief  description
  *  @author   
  *  @date     2020.5.7
  *  @version  1.0
  *  @note
  *  detailed  description
  *  $LastChangedDate$
  *  $LastChangedRevision$
  *  $LastChangedBy$
  */

#ifndef __L2SW_API_IGMP_H__
#define __L2SW_API_IGMP_H__

/*
 * Data Type Declaration
 */
typedef enum l2sw_igmp_type_e
{
    IGMP_IPV4 = 0,
    IGMP_PPPOE_IPV4,
    IGMP_MLD,
    IGMP_PPPOE_MLD,
    IGMP_TYPE_END
} l2sw_igmp_type_t;

typedef enum l2sw_trap_igmp_action_e
{
    IGMP_ACTION_FORWARD = 0,
    IGMP_ACTION_TRAP2CPU,
    IGMP_ACTION_DROP,
    IGMP_ACTION_ASIC,
    IGMP_ACTION_END
} l2sw_igmp_action_t;

typedef enum l2sw_igmp_protocol_e
{
    PROTOCOL_IGMPv1 = 0,
    PROTOCOL_IGMPv2,
    PROTOCOL_IGMPv3,
    PROTOCOL_MLDv1,
    PROTOCOL_MLDv2,
    PROTOCOL_END
} l2sw_igmp_protocol_t;

typedef enum l2sw_igmp_tableFullAction_e
{
    IGMP_TABLE_FULL_FORWARD = 0,
    IGMP_TABLE_FULL_DROP,
    IGMP_TABLE_FULL_TRAP,
    IGMP_TABLE_FULL_OP_END
}l2sw_igmp_tableFullAction_t;

typedef enum l2sw_igmp_checksumErrorAction_e
{
    IGMP_CRC_ERR_DROP = 0,
    IGMP_CRC_ERR_TRAP,
    IGMP_CRC_ERR_FORWARD,
    IGMP_CRC_ERR_NORMAL,
    IGMP_CRC_ERR_OP_END
}l2sw_igmp_checksumErrorAction_t;

typedef enum l2sw_igmp_bypassGroup_e
{
    IGMP_BYPASS_224_0_0_X = 0,
    IGMP_BYPASS_224_0_1_X,
    IGMP_BYPASS_239_255_255_X,
    IGMP_BYPASS_GROUP_END
}l2sw_igmp_bypassGroup_t;


typedef struct l2sw_igmp_dynamicRouterPort_s
{
    l2sw_enable_t    dynamicRouterPort0Valid;
    l2sw_port_t      dynamicRouterPort0;
    l2sw_uint32      dynamicRouterPort0Timer;
    l2sw_enable_t    dynamicRouterPort1Valid;
    l2sw_port_t      dynamicRouterPort1;
    l2sw_uint32      dynamicRouterPort1Timer;

}l2sw_igmp_dynamicRouterPort_t;

typedef struct l2sw_igmp_rxPktEnable_s
{
    l2sw_enable_t rxQuery;
    l2sw_enable_t rxReport;
    l2sw_enable_t rxLeave;
    l2sw_enable_t rxMRP;
    l2sw_enable_t rxMcast;
}l2sw_igmp_rxPktEnable_t;

typedef struct l2sw_igmp_groupInfo_s
{
    l2sw_enable_t    valid;
    l2sw_portmask_t  member;
    l2sw_uint32      timer[L2SW_PORT_MAX];
    l2sw_uint32      reportSuppFlag;
}l2sw_igmp_groupInfo_t;

typedef enum l2sw_igmp_ReportLeaveFwdAct_e
{
    IGMP_REPORT_LEAVE_TO_ROUTER = 0,
    IGMP_REPORT_LEAVE_TO_ALLPORT,
    IGMP_REPORT_LEAVE_TO_ROUTER_PORT_ADV,
    IGMP_REPORT_LEAVE_ACT_END
}l2sw_igmp_ReportLeaveFwdAct_t;

/* Function Name:
 *      l2sw_igmp_init
 * Description:
 *      This API enables H/W IGMP and set a default initial configuration.
 * Input:
 *      None.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 *      This API enables H/W IGMP and set a default initial configuration.
 */
extern l2sw_api_ret_t l2sw_igmp_init(l2sw_uint8 units);

/* Function Name:
 *      l2sw_igmp_state_set
 * Description:
 *      This API set H/W IGMP state.
 * Input:
 *      enabled     - H/W IGMP state
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Error parameter
 * Note:
 *      This API set H/W IGMP state.
 */
extern l2sw_api_ret_t l2sw_igmp_state_set(l2sw_uint8 units, l2sw_enable_t enabled);

/* Function Name:
 *      l2sw_igmp_state_get
 * Description:
 *      This API get H/W IGMP state.
 * Input:
 *      None.
 * Output:
 *      pEnabled        - H/W IGMP state
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Error parameter
 * Note:
 *      This API set current H/W IGMP state.
 */
extern l2sw_api_ret_t l2sw_igmp_state_get(l2sw_uint8 units, l2sw_enable_t *pEnabled);

/* Function Name:
 *      l2sw_igmp_static_router_port_set
 * Description:
 *      Configure static router port
 * Input:
 *      pPortmask    - Static Port mask
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_PORT_MASK       - Error parameter
 * Note:
 *      This API set static router port
 */
extern l2sw_api_ret_t l2sw_igmp_static_router_port_set(l2sw_uint8 units, l2sw_portmask_t *pPortmask);

/* Function Name:
 *      l2sw_igmp_static_router_port_get
 * Description:
 *      Get static router port
 * Input:
 *      None.
 * Output:
 *      pPortmask       - Static port mask
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_PORT_MASK       - Error parameter
 * Note:
 *      This API get static router port
 */
extern l2sw_api_ret_t l2sw_igmp_static_router_port_get(l2sw_uint8 units, l2sw_portmask_t *pPortmask);

/* Function Name:
 *      l2sw_igmp_protocol_set
 * Description:
 *      set IGMP/MLD protocol action
 * Input:
 *      port        - Port ID
 *      protocol    - IGMP/MLD protocol
 *      action      - Per-port and per-protocol IGMP action seeting
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_PORT_MASK       - Error parameter
 * Note:
 *      This API set IGMP/MLD protocol action
 */
extern l2sw_api_ret_t l2sw_igmp_protocol_set(l2sw_uint8 units, l2sw_port_t port, l2sw_igmp_protocol_t protocol, l2sw_igmp_action_t action);

/* Function Name:
 *      l2sw_igmp_protocol_get
 * Description:
 *      set IGMP/MLD protocol action
 * Input:
 *      port        - Port ID
 *      protocol    - IGMP/MLD protocol
 * Output:
 *      pAction      - Per-port and per-protocol IGMP action seeting
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_PORT_MASK       - Error parameter
 * Note:
 *      This API set IGMP/MLD protocol action
 */
extern l2sw_api_ret_t l2sw_igmp_protocol_get(l2sw_uint8 units, l2sw_port_t port, l2sw_igmp_protocol_t protocol, l2sw_igmp_action_t *pAction);

/* Function Name:
 *      l2sw_igmp_fastLeave_set
 * Description:
 *      set IGMP/MLD FastLeave state
 * Input:
 *      state       - ENABLED: Enable FastLeave, DISABLED: disable FastLeave
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_INPUT           - Error Input
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 *      This API set IGMP/MLD FastLeave state
 */
extern l2sw_api_ret_t l2sw_igmp_fastLeave_set(l2sw_uint8 units, l2sw_enable_t state);

/* Function Name:
 *      l2sw_igmp_fastLeave_get
 * Description:
 *      get IGMP/MLD FastLeave state
 * Input:
 *      None
 * Output:
 *      pState      - ENABLED: Enable FastLeave, DISABLED: disable FastLeave
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_NULL_POINTER    - NULL pointer
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 *      This API get IGMP/MLD FastLeave state
 */
extern l2sw_api_ret_t l2sw_igmp_fastLeave_get(l2sw_uint8 units, l2sw_enable_t *pState);

/* Function Name:
 *      l2sw_igmp_maxGroup_set
 * Description:
 *      Set per port multicast group learning limit.
 * Input:
 *      port        - Port ID
 *      group       - The number of multicast group learning limit.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_PORT_ID         - Error Port ID
 *      L2SW_ERR_OUT_OF_RANGE    - parameter out of range
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 *      This API set per port multicast group learning limit.
 */
extern l2sw_api_ret_t l2sw_igmp_maxGroup_set(l2sw_uint8 units, l2sw_port_t port, l2sw_uint32 group);

/* Function Name:
 *      l2sw_igmp_maxGroup_get
 * Description:
 *      Get per port multicast group learning limit.
 * Input:
 *      port        - Port ID
 * Output:
 *      pGroup      - The number of multicast group learning limit.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_PORT_ID         - Error Port ID
 *      L2SW_ERR_NULL_POINTER    - Null pointer
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 *      This API get per port multicast group learning limit.
 */
extern l2sw_api_ret_t l2sw_igmp_maxGroup_get(l2sw_uint8 units, l2sw_port_t port, l2sw_uint32 *pGroup);

/* Function Name:
 *      l2sw_igmp_currentGroup_get
 * Description:
 *      Get per port multicast group learning count.
 * Input:
 *      port        - Port ID
 * Output:
 *      pGroup      - The number of multicast group learning count.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_PORT_ID         - Error Port ID
 *      L2SW_ERR_NULL_POINTER    - Null pointer
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 *      This API get per port multicast group learning count.
 */
extern l2sw_api_ret_t l2sw_igmp_currentGroup_get(l2sw_uint8 units, l2sw_port_t port, l2sw_uint32 *pGroup);

/* Function Name:
 *      l2sw_igmp_tableFullAction_set
 * Description:
 *      set IGMP/MLD Table Full Action
 * Input:
 *      action      - Table Full Action
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_INPUT           - Error Input
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 */
extern l2sw_api_ret_t l2sw_igmp_tableFullAction_set(l2sw_uint8 units, l2sw_igmp_tableFullAction_t action);

/* Function Name:
 *      l2sw_igmp_tableFullAction_get
 * Description:
 *      get IGMP/MLD Table Full Action
 * Input:
 *      None
 * Output:
 *      pAction     - Table Full Action
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_NULL_POINTER    - Null pointer
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 */
extern l2sw_api_ret_t l2sw_igmp_tableFullAction_get(l2sw_uint8 units, l2sw_igmp_tableFullAction_t *pAction);

/* Function Name:
 *      l2sw_igmp_checksumErrorAction_set
 * Description:
 *      set IGMP/MLD Checksum Error Action
 * Input:
 *      action      - Checksum error Action
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_INPUT           - Error Input
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 */
extern l2sw_api_ret_t l2sw_igmp_checksumErrorAction_set(l2sw_uint8 units, l2sw_igmp_checksumErrorAction_t action);

/* Function Name:
 *      l2sw_igmp_checksumErrorAction_get
 * Description:
 *      get IGMP/MLD Checksum Error Action
 * Input:
 *      None
 * Output:
 *      pAction     - Checksum error Action
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_NULL_POINTER    - Null pointer
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 */
extern l2sw_api_ret_t l2sw_igmp_checksumErrorAction_get(l2sw_uint8 units, l2sw_igmp_checksumErrorAction_t *pAction);


/* Function Name:
 *      l2sw_igmp_queryInterval_set
 * Description:
 *      set IGMP/MLD Query Interval
 * Input:
 *      interval     - Query Interval
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_INPUT           - Error Input
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 */
extern l2sw_api_ret_t l2sw_igmp_queryInterval_set(l2sw_uint8 units, l2sw_uint32 interval);

/* Function Name:
 *      l2sw_igmp_queryInterval_get
 * Description:
 *      get IGMP/MLD Query Interval
 * Input:
 *      None.
 * Output:
 *      pInterval   - Query Interval
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_NULL_POINTER    - Null pointer
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 */
extern l2sw_api_ret_t l2sw_igmp_queryInterval_get(l2sw_uint8 units, l2sw_uint32 *pInterval);

/* Function Name:
 *      l2sw_igmp_dynamicRouterPort_queryInterval_set
 * Description:
 *      set IGMP/MLD dynamic Router Port Query Interval
 * Input:
 *      interval     - Query Interval
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_INPUT           - Error Input
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 */

extern l2sw_api_ret_t l2sw_igmp_dynamicRouterPort_queryInterval_set(l2sw_uint8 units, l2sw_uint32 interval);

/* Function Name:
 *      l2sw_igmp_dynamicRouterPort_queryInterval_get
 * Description:
 *      get IGMP/MLD dynamic Router Port Query Interval
 * Input:
 *      None.
 * Output:
 *      pInterval   - Query Interval
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_NULL_POINTER    - Null pointer
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 */

extern l2sw_api_ret_t l2sw_igmp_dynamicRouterPort_queryInterval_get(l2sw_uint8 units, l2sw_uint32 *pInterval);


/* Function Name:
 *      l2sw_igmp_robustness_set
 * Description:
 *      set IGMP/MLD Robustness value
 * Input:
 *      robustness     - Robustness value
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_INPUT           - Error Input
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 */
extern l2sw_api_ret_t l2sw_igmp_robustness_set(l2sw_uint8 units, l2sw_uint32 robustness);

/* Function Name:
 *      l2sw_igmp_robustness_get
 * Description:
 *      get IGMP/MLD Robustness value
 * Input:
 *      None
 * Output:
 *      pRobustness     - Robustness value.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_NULL_POINTER    - Null pointer
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 */
extern l2sw_api_ret_t l2sw_igmp_robustness_get(l2sw_uint8 units, l2sw_uint32 *pRobustness);

/* Function Name:
 *      l2sw_igmp_dynamicRouterPortAllow_set
 * Description:
 *      Configure dynamic router port allow option
 * Input:
 *      pPortmask    - Dynamic Port allow mask
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_PORT_MASK       - Error parameter
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_igmp_dynamicRouterPortAllow_set(l2sw_uint8 units, l2sw_portmask_t *pPortmask);

/* Function Name:
 *      l2sw_igmp_dynamicRouterPortAllow_get
 * Description:
 *      Get dynamic router port allow option
 * Input:
 *      None.
 * Output:
 *      pPortmask    - Dynamic Port allow mask
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_PORT_MASK       - Error parameter
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_igmp_dynamicRouterPortAllow_get(l2sw_uint8 units, l2sw_portmask_t *pPortmask);

/* Function Name:
 *      l2sw_igmp_dynamicRouterPort_get
 * Description:
 *      Get dynamic router port
 * Input:
 *      None.
 * Output:
 *      pDynamicRouterPort    - Dynamic Router Port
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_NULL_POINTER    - Null pointer
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_PORT_MASK       - Error parameter
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_igmp_dynamicRouterPort_get(l2sw_uint8 units, l2sw_igmp_dynamicRouterPort_t *pDynamicRouterPort);

/* Function Name:
 *      l2sw_igmp_suppressionEnable_set
 * Description:
 *      Configure IGMPv1/v2 & MLDv1 Report/Leave/Done suppression
 * Input:
 *      reportSuppression   - Report suppression
 *      leaveSuppression    - Leave suppression
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Error Input
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_igmp_suppressionEnable_set(l2sw_uint8 units, l2sw_enable_t reportSuppression, l2sw_enable_t leaveSuppression);

/* Function Name:
 *      l2sw_igmp_suppressionEnable_get
 * Description:
 *      Get IGMPv1/v2 & MLDv1 Report/Leave/Done suppression
 * Input:
 *      None
 * Output:
 *      pReportSuppression  - Report suppression
 *      pLeaveSuppression   - Leave suppression
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_NULL_POINTER    - Null pointer
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_igmp_suppressionEnable_get(l2sw_uint8 units, l2sw_enable_t *pReportSuppression, l2sw_enable_t *pLeaveSuppression);

/* Function Name:
 *      l2sw_igmp_portRxPktEnable_set
 * Description:
 *      Configure IGMP/MLD RX Packet configuration
 * Input:
 *      port       - Port ID
 *      pRxCfg     - RX Packet Configuration
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Error Input
 *      L2SW_ERR_NULL_POINTER    - Null pointer
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_igmp_portRxPktEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_igmp_rxPktEnable_t *pRxCfg);

/* Function Name:
 *      l2sw_igmp_portRxPktEnable_get
 * Description:
 *      Get IGMP/MLD RX Packet configuration
 * Input:
 *      port       - Port ID
 * Output:
 *      pRxCfg     - RX Packet Configuration
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Error Input
 *      L2SW_ERR_NULL_POINTER    - Null pointer
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_igmp_portRxPktEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_igmp_rxPktEnable_t *pRxCfg);

/* Function Name:
 *      l2sw_igmp_groupInfo_get
 * Description:
 *      Get IGMP/MLD Group database
 * Input:
 *      indes       - Index (0~255)
 * Output:
 *      pGroup      - Group database information.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Error Input
 *      L2SW_ERR_NULL_POINTER    - Null pointer
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_igmp_groupInfo_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_igmp_groupInfo_t *pGroup);

/* Function Name:
 *      l2sw_igmp_ReportLeaveFwdAction_set
 * Description:
 *      Set Report Leave packet forwarding action
 * Input:
 *      action      - Action
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Error Input
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_igmp_ReportLeaveFwdAction_set(l2sw_uint8 units, l2sw_igmp_ReportLeaveFwdAct_t action);

/* Function Name:
 *      l2sw_igmp_ReportLeaveFwdAction_get
 * Description:
 *      Get Report Leave packet forwarding action
 * Input:
 *      None.
 * Output:
 *      pAction      - Action
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Error Input
 *      L2SW_ERR_NULL_POINTER    - Null Pointer
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_igmp_ReportLeaveFwdAction_get(l2sw_uint8 units, l2sw_igmp_ReportLeaveFwdAct_t *pAction);

/* Function Name:
 *      l2sw_igmp_dropLeaveZeroEnable_set
 * Description:
 *      Set the function of droppping Leave packet with group IP = 0.0.0.0
 * Input:
 *      enabled      - Action 1: drop, 0:pass
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Error Input
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_igmp_dropLeaveZeroEnable_set(l2sw_uint8 units, l2sw_enable_t enabled);

/* Function Name:
 *      l2sw_igmp_dropLeaveZeroEnable_get
 * Description:
 *      Get the function of droppping Leave packet with group IP = 0.0.0.0
 * Input:
 *      None
 * Output:
 *      pEnabled.   - Action 1: drop, 0:pass
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Error Input
 *      L2SW_ERR_NULL_POINTER    - Null Pointer
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_igmp_dropLeaveZeroEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnabled);

/* Function Name:
 *      l2sw_igmp_bypassGroupRange_set
 * Description:
 *      Set Bypass group
 * Input:
 *      group       - bypassed group
 *      enabled     - enabled 1: Bypassed, 0: not bypass
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Error Input
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_igmp_bypassGroupRange_set(l2sw_uint8 units, l2sw_igmp_bypassGroup_t group, l2sw_enable_t enabled);

/* Function Name:
 *      l2sw_igmp_bypassGroupRange_get
 * Description:
 *      get Bypass group
 * Input:
 *      group       - bypassed group
 * Output:
 *      pEnable     - enabled 1: Bypassed, 0: not bypass
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Error Input
 *      L2SW_ERR_NULL_POINTER    - Null Pointer
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_igmp_bypassGroupRange_get(l2sw_uint8 units, l2sw_igmp_bypassGroup_t group, l2sw_enable_t *pEnable);

#endif /* __L2SW_API_IGMP_H__ */
