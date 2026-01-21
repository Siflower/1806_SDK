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

#ifndef __L2SW_API_TRAP_H__
#define __L2SW_API_TRAP_H__


typedef enum l2sw_trap_type_e
{
    TRAP_BRG_GROUP = 0,
    TRAP_FD_PAUSE,
    TRAP_SP_MCAST,
    TRAP_1X_PAE,
    TRAP_UNDEF_BRG_04,
    TRAP_UNDEF_BRG_05,
    TRAP_UNDEF_BRG_06,
    TRAP_UNDEF_BRG_07,
    TRAP_PROVIDER_BRIDGE_GROUP_ADDRESS,
    TRAP_UNDEF_BRG_09,
    TRAP_UNDEF_BRG_0A,
    TRAP_UNDEF_BRG_0B,
    TRAP_UNDEF_BRG_0C,
    TRAP_PROVIDER_BRIDGE_GVRP_ADDRESS,
    TRAP_8021AB,
    TRAP_UNDEF_BRG_0F,
    TRAP_BRG_MNGEMENT,
    TRAP_UNDEFINED_11,
    TRAP_UNDEFINED_12,
    TRAP_UNDEFINED_13,
    TRAP_UNDEFINED_14,
    TRAP_UNDEFINED_15,
    TRAP_UNDEFINED_16,
    TRAP_UNDEFINED_17,
    TRAP_UNDEFINED_18,
    TRAP_UNDEFINED_19,
    TRAP_UNDEFINED_1A,
    TRAP_UNDEFINED_1B,
    TRAP_UNDEFINED_1C,
    TRAP_UNDEFINED_1D,
    TRAP_UNDEFINED_1E,
    TRAP_UNDEFINED_1F,
    TRAP_GMRP,
    TRAP_GVRP,
    TRAP_UNDEF_GARP_22,
    TRAP_UNDEF_GARP_23,
    TRAP_UNDEF_GARP_24,
    TRAP_UNDEF_GARP_25,
    TRAP_UNDEF_GARP_26,
    TRAP_UNDEF_GARP_27,
    TRAP_UNDEF_GARP_28,
    TRAP_UNDEF_GARP_29,
    TRAP_UNDEF_GARP_2A,
    TRAP_UNDEF_GARP_2B,
    TRAP_UNDEF_GARP_2C,
    TRAP_UNDEF_GARP_2D,
    TRAP_UNDEF_GARP_2E,
    TRAP_UNDEF_GARP_2F,
    TRAP_CDP,
    TRAP_CSSTP,
    TRAP_LLDP,
    TRAP_END,
}l2sw_trap_type_t;


typedef enum l2sw_mcast_type_e
{
    MCAST_L2 = 0,
    MCAST_IPV4,
    MCAST_IPV6,
    MCAST_END
} l2sw_mcast_type_t;

typedef enum l2sw_trap_mcast_action_e
{
    MCAST_ACTION_FORWARD = 0,
    MCAST_ACTION_DROP,
    MCAST_ACTION_TRAP2CPU,
    MCAST_ACTION_ROUTER_PORT,
    MCAST_ACTION_DROP_EX_RMA,
    MCAST_ACTION_END
} l2sw_trap_mcast_action_t;

typedef enum l2sw_trap_rma_action_e
{
    RMA_ACTION_FORWARD = 0,
    RMA_ACTION_TRAP2CPU,
    RMA_ACTION_DROP,
    RMA_ACTION_FORWARD_EXCLUDE_CPU,
    RMA_ACTION_END
} l2sw_trap_rma_action_t;

typedef enum l2sw_trap_ucast_action_e
{
    UCAST_ACTION_FORWARD_PMASK = 0,
    UCAST_ACTION_DROP,
    UCAST_ACTION_TRAP2CPU,
    UCAST_ACTION_FLOODING,
    UCAST_ACTION_END
} l2sw_trap_ucast_action_t;

typedef enum l2sw_trap_ucast_type_e
{
    UCAST_UNKNOWNDA = 0,
    UCAST_UNKNOWNSA,
    UCAST_UNMATCHSA,
    UCAST_END
} l2sw_trap_ucast_type_t;

typedef enum l2sw_trap_reason_type_e
{
    TRAP_REASON_RMA = 0,
    TRAP_REASON_OAM,
    TRAP_REASON_1XUNAUTH,
    TRAP_REASON_VLANSTACK,
    TRAP_REASON_UNKNOWNMC,
    TRAP_REASON_END,
} l2sw_trap_reason_type_t;


/* Function Name:
 *      l2sw_trap_unknownUnicastPktGlobalAction_set
 * Description:
 *      Set unknown unicast packet global action configuration.
 * Input:
 *      ucast_action    - Unknown unicast action.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                  - OK
 *      L2SW_ERR_FAILED              - Failed
 *      L2SW_ERR_SMI                 - SMI access error
 *      L2SW_ERR_NOT_ALLOWED         - Invalid action.
 *      L2SW_ERR_INPUT               - Invalid input parameters.
 * Note:
 *      This API can set unknown unicast packet action configuration.
 *      The unknown unicast action is as following:
 *          - UCAST_ACTION_FORWARD_PMASK
 *          - UCAST_ACTION_DROP
 *          - UCAST_ACTION_TRAP2CPU
 *          - UCAST_ACTION_FLOODING
 */
l2sw_api_ret_t l2sw_trap_unknownUnicastPktGlobalAction_set(l2sw_uint8 units, l2sw_trap_ucast_action_t ucast_action);

/* Function Name:
 *      l2sw_trap_unknownUnicastPktGlobalAction_get
 * Description:
 *      Get unknown unicast packet global action configuration.
 * Input:
 *      none
 * Output:
 *      pUcast_action   - Unknown unicast action.
 * Return:
 *      L2SW_ERR_OK                  - OK
 *      L2SW_ERR_FAILED              - Failed
 *      L2SW_ERR_SMI                 - SMI access error
 *      L2SW_ERR_NOT_ALLOWED         - Invalid action.
 *      L2SW_ERR_INPUT               - Invalid input parameters.
 *      L2SW_ERR_NULL_POINTER        - Null pointer
 * Note:
 *      This API can get unknown unicast packet action configuration.
 *      The unknown unicast action is as following:
 *          - UCAST_ACTION_FORWARD_PMASK
 *          - UCAST_ACTION_DROP
 *          - UCAST_ACTION_TRAP2CPU
 *          - UCAST_ACTION_FLOODING
 */
l2sw_api_ret_t l2sw_trap_unknownUnicastPktGlobalAction_get(l2sw_uint8 units, l2sw_trap_ucast_action_t *pUcast_action);

/* Function Name:
 *      rtk_trap_unknownUnicastPktAction_set
 * Description:
 *      Set unknown unicast packet action configuration.
 * Input:
 *      port            - ingress port ID for unknown unicast packet
 *      ucast_action    - Unknown unicast action.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                  - OK
 *      L2SW_ERR_FAILED              - Failed
 *      L2SW_ERR_SMI                 - SMI access error
 *      L2SW_ERR_NOT_ALLOWED         - Invalid action.
 *      L2SW_ERR_INPUT               - Invalid input parameters.
 * Note:
 *      This API can set unknown unicast packet action configuration.
 *      The unknown unicast action is as following:
 *          - UCAST_ACTION_FORWARD_PMASK
 *          - UCAST_ACTION_DROP
 *          - UCAST_ACTION_TRAP2CPU
 *          - UCAST_ACTION_FLOODING
 */
l2sw_api_ret_t l2sw_trap_unknownUnicastPktAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_trap_ucast_action_t ucast_action);

/* Function Name:
 *      l2sw_trap_unknownUnicastPktAction_get
 * Description:
 *      Get unknown unicast packet action configuration.
 * Input:
 *      port            - ingress port ID for unknown unicast packet
 * Output:
 *      pUcast_action   - Unknown unicast action.
 * Return:
 *      L2SW_ERR_OK                  - OK
 *      L2SW_ERR_FAILED              - Failed
 *      L2SW_ERR_SMI                 - SMI access error
 *      L2SW_ERR_NOT_ALLOWED         - Invalid action.
 *      L2SW_ERR_INPUT               - Invalid input parameters.
 *      L2SW_ERR_NULL_POINTER        - Null pointer
 * Note:
 *      This API can get unknown unicast packet action configuration.
 *      The unknown unicast action is as following:
 *          - UCAST_ACTION_FORWARD_PMASK
 *          - UCAST_ACTION_DROP
 *          - UCAST_ACTION_TRAP2CPU
 *          - UCAST_ACTION_FLOODING
 */
l2sw_api_ret_t l2sw_trap_unknownUnicastPktAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_trap_ucast_action_t *pUcast_action);

/* Function Name:
 *      l2sw_trap_unknownMacPktAction_set
 * Description:
 *      Set unknown source MAC packet action configuration.
 * Input:
 *      ucast_action    - Unknown source MAC action.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                  - OK
 *      L2SW_ERR_FAILED              - Failed
 *      L2SW_ERR_SMI                 - SMI access error
 *      L2SW_ERR_NOT_ALLOWED         - Invalid action.
 *      L2SW_ERR_INPUT               - Invalid input parameters.
 * Note:
 *      This API can set unknown unicast packet action configuration.
 *      The unknown unicast action is as following:
 *          - UCAST_ACTION_FORWARD_PMASK
 *          - UCAST_ACTION_DROP
 *          - UCAST_ACTION_TRAP2CPU
 */
extern l2sw_api_ret_t l2sw_trap_unknownMacPktAction_set(l2sw_uint8 units, l2sw_trap_ucast_action_t ucast_action);

/* Function Name:
 *      l2sw_trap_unknownMacPktAction_get
 * Description:
 *      Get unknown source MAC packet action configuration.
 * Input:
 *      None.
 * Output:
 *      pUcast_action   - Unknown source MAC action.
 * Return:
 *      L2SW_ERR_OK                  - OK
 *      L2SW_ERR_FAILED              - Failed
 *      L2SW_ERR_SMI                 - SMI access error
 *      L2SW_ERR_NULL_POINTER        - Null Pointer.
 *      L2SW_ERR_INPUT               - Invalid input parameters.
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_trap_unknownMacPktAction_get(l2sw_uint8 units, l2sw_trap_ucast_action_t *pUcast_action);

/* Function Name:
 *      l2sw_trap_unmatchMacPktAction_set
 * Description:
 *      Set unmatch source MAC packet action configuration.
 * Input:
 *      ucast_action    - Unknown source MAC action.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                  - OK
 *      L2SW_ERR_FAILED              - Failed
 *      L2SW_ERR_SMI                 - SMI access error
 *      L2SW_ERR_NOT_ALLOWED         - Invalid action.
 *      L2SW_ERR_INPUT               - Invalid input parameters.
 * Note:
 *      This API can set unknown unicast packet action configuration.
 *      The unknown unicast action is as following:
 *          - UCAST_ACTION_FORWARD_PMASK
 *          - UCAST_ACTION_DROP
 *          - UCAST_ACTION_TRAP2CPU
 */
extern l2sw_api_ret_t l2sw_trap_unmatchMacPktAction_set(l2sw_uint8 units, l2sw_trap_ucast_action_t ucast_action);

/* Function Name:
 *      l2sw_trap_unmatchMacPktAction_get
 * Description:
 *      Get unmatch source MAC packet action configuration.
 * Input:
 *      None.
 * Output:
 *      pUcast_action   - Unknown source MAC action.
 * Return:
 *      L2SW_ERR_OK                  - OK
 *      L2SW_ERR_FAILED              - Failed
 *      L2SW_ERR_SMI                 - SMI access error
 *      L2SW_ERR_NOT_ALLOWED         - Invalid action.
 *      L2SW_ERR_INPUT               - Invalid input parameters.
 * Note:
 *      This API can set unknown unicast packet action configuration.
 *      The unknown unicast action is as following:
 *          - UCAST_ACTION_FORWARD_PMASK
 *          - UCAST_ACTION_DROP
 *          - UCAST_ACTION_TRAP2CPU
 */
extern l2sw_api_ret_t l2sw_trap_unmatchMacPktAction_get(l2sw_uint8 units, l2sw_trap_ucast_action_t *pUcast_action);

/* Function Name:
 *      l2sw_trap_unmatchMacMoving_set
 * Description:
 *      Set unmatch source MAC packet moving state.
 * Input:
 *      port        - Port ID.
 *      enable      - ENABLED: allow SA moving, DISABLE: don't allow SA moving.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                  - OK
 *      L2SW_ERR_FAILED              - Failed
 *      L2SW_ERR_SMI                 - SMI access error
 *      L2SW_ERR_NOT_ALLOWED         - Invalid action.
 *      L2SW_ERR_INPUT               - Invalid input parameters.
 * Note:
 */
extern l2sw_api_ret_t l2sw_trap_unmatchMacMoving_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_trap_unmatchMacMoving_get
 * Description:
 *      Set unmatch source MAC packet moving state.
 * Input:
 *      port        - Port ID.
 * Output:
 *      pEnable     - ENABLED: allow SA moving, DISABLE: don't allow SA moving.
 * Return:
 *      L2SW_ERR_OK                  - OK
 *      L2SW_ERR_FAILED              - Failed
 *      L2SW_ERR_SMI                 - SMI access error
 *      L2SW_ERR_NOT_ALLOWED         - Invalid action.
 *      L2SW_ERR_INPUT               - Invalid input parameters.
 * Note:
 */
extern l2sw_api_ret_t l2sw_trap_unmatchMacMoving_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_trap_unknownMcastPktAction_set
 * Description:
 *      Set behavior of unknown multicast
 * Input:
 *      port            - Port id.
 *      type            - unknown multicast packet type.
 *      mcast_action    - unknown multicast action.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_NOT_ALLOWED  - Invalid action.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      When receives an unknown multicast packet, switch may trap, drop or flood this packet
 *      (1) The unknown multicast packet type is as following:
 *          - MCAST_L2
 *          - MCAST_IPV4
 *          - MCAST_IPV6
 *      (2) The unknown multicast action is as following:
 *          - MCAST_ACTION_FORWARD
 *          - MCAST_ACTION_DROP
 *          - MCAST_ACTION_TRAP2CPU
 */
extern l2sw_api_ret_t l2sw_trap_unknownMcastPktAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_mcast_type_t type, l2sw_trap_mcast_action_t mcast_action);

/* Function Name:
 *      l2sw_trap_unknownMcastPktAction_get
 * Description:
 *      Get behavior of unknown multicast
 * Input:
 *      type - unknown multicast packet type.
 * Output:
 *      pMcast_action - unknown multicast action.
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_NOT_ALLOWED      - Invalid operation.
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 * Note:
 *      When receives an unknown multicast packet, switch may trap, drop or flood this packet
 *      (1) The unknown multicast packet type is as following:
 *          - MCAST_L2
 *          - MCAST_IPV4
 *          - MCAST_IPV6
 *      (2) The unknown multicast action is as following:
 *          - MCAST_ACTION_FORWARD
 *          - MCAST_ACTION_DROP
 *          - MCAST_ACTION_TRAP2CPU
 */
extern l2sw_api_ret_t l2sw_trap_unknownMcastPktAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_mcast_type_t type, l2sw_trap_mcast_action_t *pMcast_action);

/* Function Name:
 *      l2sw_trap_lldpEnable_set
 * Description:
 *      Set LLDP enable.
 * Input:
 *      enabled - LLDP enable, 0: follow RMA, 1: use LLDP action.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_NOT_ALLOWED      - Invalid action.
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 * Note:
 *      - DMAC                                                 Assignment
 *      - 01:80:c2:00:00:0e ethertype = 0x88CC    LLDP
 *      - 01:80:c2:00:00:03 ethertype = 0x88CC
 *      - 01:80:c2:00:00:00 ethertype = 0x88CC

 */
extern l2sw_api_ret_t l2sw_trap_lldpEnable_set(l2sw_uint8 units, l2sw_enable_t enabled);

/* Function Name:
 *      l2sw_trap_lldpEnable_get
 * Description:
 *      Get LLDP status.
 * Input:
 *      None
 * Output:
 *      pEnabled - LLDP enable, 0: follow RMA, 1: use LLDP action.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      LLDP is as following definition.
 *      - DMAC                                                 Assignment
 *      - 01:80:c2:00:00:0e ethertype = 0x88CC    LLDP
 *      - 01:80:c2:00:00:03 ethertype = 0x88CC
 *      - 01:80:c2:00:00:00 ethertype = 0x88CC
 */
extern l2sw_api_ret_t l2sw_trap_lldpEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnabled);

/* Function Name:
 *      l2sw_trap_reasonTrapToCpuPriority_set
 * Description:
 *      Set priority value of a packet that trapped to CPU port according to specific reason.
 * Input:
 *      type     - reason that trap to CPU port.
 *      priority - internal priority that is going to be set for specific trap reason.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT - The module is not initial
 *      L2SW_ERR_INPUT    - Invalid input parameter
 * Note:
 *      Currently the trap reason that supported are listed as follows:
 *      - TRAP_REASON_RMA
 *      - TRAP_REASON_OAM
 *      - TRAP_REASON_1XUNAUTH
 *      - TRAP_REASON_VLANSTACK
 *      - TRAP_REASON_UNKNOWNMC
 */
extern l2sw_api_ret_t l2sw_trap_reasonTrapToCpuPriority_set(l2sw_uint8 units, l2sw_trap_reason_type_t type, l2sw_pri_t priority);

/* Function Name:
 *      l2sw_trap_reasonTrapToCpuPriority_get
 * Description:
 *      Get priority value of a packet that trapped to CPU port according to specific reason.
 * Input:
 *      type      - reason that trap to CPU port.
 * Output:
 *      pPriority - configured internal priority for such reason.
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT     - The module is not initial
 *      L2SW_ERR_INPUT        - Invalid input parameter
 *      L2SW_ERR_NULL_POINTER - NULL pointer
 * Note:
 *      Currently the trap reason that supported are listed as follows:
 *      - TRAP_REASON_RMA
 *      - TRAP_REASON_OAM
 *      - TRAP_REASON_1XUNAUTH
 *      - TRAP_REASON_VLANSTACK
 *      - TRAP_REASON_UNKNOWNMC
 */
extern l2sw_api_ret_t l2sw_trap_reasonTrapToCpuPriority_get(l2sw_uint8 units, l2sw_trap_reason_type_t type, l2sw_pri_t *pPriority);

/* Function Name:
 *      l2sw_trap_rmaAction_set
 * Description:
 *      Set Reserved multicast address action configuration.
 * Input:
 *      type    - rma type.
 *      rma_action - RMA action.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_ENABLE       - Invalid IFG parameter
 * Note:
 *
 *      There are 48 types of Reserved Multicast Address frame for application usage.
 *      (1)They are as following definition.
 *      - TRAP_BRG_GROUP,
 *      - TRAP_FD_PAUSE,
 *      - TRAP_SP_MCAST,
 *      - TRAP_1X_PAE,
 *      - TRAP_UNDEF_BRG_04,
 *      - TRAP_UNDEF_BRG_05,
 *      - TRAP_UNDEF_BRG_06,
 *      - TRAP_UNDEF_BRG_07,
 *      - TRAP_PROVIDER_BRIDGE_GROUP_ADDRESS,
 *      - TRAP_UNDEF_BRG_09,
 *      - TRAP_UNDEF_BRG_0A,
 *      - TRAP_UNDEF_BRG_0B,
 *      - TRAP_UNDEF_BRG_0C,
 *      - TRAP_PROVIDER_BRIDGE_GVRP_ADDRESS,
 *      - TRAP_8021AB,
 *      - TRAP_UNDEF_BRG_0F,
 *      - TRAP_BRG_MNGEMENT,
 *      - TRAP_UNDEFINED_11,
 *      - TRAP_UNDEFINED_12,
 *      - TRAP_UNDEFINED_13,
 *      - TRAP_UNDEFINED_14,
 *      - TRAP_UNDEFINED_15,
 *      - TRAP_UNDEFINED_16,
 *      - TRAP_UNDEFINED_17,
 *      - TRAP_UNDEFINED_18,
 *      - TRAP_UNDEFINED_19,
 *      - TRAP_UNDEFINED_1A,
 *      - TRAP_UNDEFINED_1B,
 *      - TRAP_UNDEFINED_1C,
 *      - TRAP_UNDEFINED_1D,
 *      - TRAP_UNDEFINED_1E,
 *      - TRAP_UNDEFINED_1F,
 *      - TRAP_GMRP,
 *      - TRAP_GVRP,
 *      - TRAP_UNDEF_GARP_22,
 *      - TRAP_UNDEF_GARP_23,
 *      - TRAP_UNDEF_GARP_24,
 *      - TRAP_UNDEF_GARP_25,
 *      - TRAP_UNDEF_GARP_26,
 *      - TRAP_UNDEF_GARP_27,
 *      - TRAP_UNDEF_GARP_28,
 *      - TRAP_UNDEF_GARP_29,
 *      - TRAP_UNDEF_GARP_2A,
 *      - TRAP_UNDEF_GARP_2B,
 *      - TRAP_UNDEF_GARP_2C,
 *      - TRAP_UNDEF_GARP_2D,
 *      - TRAP_UNDEF_GARP_2E,
 *      - TRAP_UNDEF_GARP_2F,
 *      - TRAP_CDP.
 *      - TRAP_CSSTP.
 *      - TRAP_LLDP.
 *      (2) The RMA action is as following:
 *      - RMA_ACTION_FORWARD
 *      - RMA_ACTION_TRAP2CPU
 *      - RMA_ACTION_DROP
 *      - RMA_ACTION_FORWARD_EXCLUDE_CPU
 */
extern l2sw_api_ret_t l2sw_trap_rmaAction_set(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_trap_rma_action_t rma_action);

/* Function Name:
 *      l2sw_trap_rmaAction_get
 * Description:
 *      Get Reserved multicast address action configuration.
 * Input:
 *      type - rma type.
 * Output:
 *      pRma_action - RMA action.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      There are 48 types of Reserved Multicast Address frame for application usage.
 *      (1)They are as following definition.
 *      - TRAP_BRG_GROUP,
 *      - TRAP_FD_PAUSE,
 *      - TRAP_SP_MCAST,
 *      - TRAP_1X_PAE,
 *      - TRAP_UNDEF_BRG_04,
 *      - TRAP_UNDEF_BRG_05,
 *      - TRAP_UNDEF_BRG_06,
 *      - TRAP_UNDEF_BRG_07,
 *      - TRAP_PROVIDER_BRIDGE_GROUP_ADDRESS,
 *      - TRAP_UNDEF_BRG_09,
 *      - TRAP_UNDEF_BRG_0A,
 *      - TRAP_UNDEF_BRG_0B,
 *      - TRAP_UNDEF_BRG_0C,
 *      - TRAP_PROVIDER_BRIDGE_GVRP_ADDRESS,
 *      - TRAP_8021AB,
 *      - TRAP_UNDEF_BRG_0F,
 *      - TRAP_BRG_MNGEMENT,
 *      - TRAP_UNDEFINED_11,
 *      - TRAP_UNDEFINED_12,
 *      - TRAP_UNDEFINED_13,
 *      - TRAP_UNDEFINED_14,
 *      - TRAP_UNDEFINED_15,
 *      - TRAP_UNDEFINED_16,
 *      - TRAP_UNDEFINED_17,
 *      - TRAP_UNDEFINED_18,
 *      - TRAP_UNDEFINED_19,
 *      - TRAP_UNDEFINED_1A,
 *      - TRAP_UNDEFINED_1B,
 *      - TRAP_UNDEFINED_1C,
 *      - TRAP_UNDEFINED_1D,
 *      - TRAP_UNDEFINED_1E,
 *      - TRAP_UNDEFINED_1F,
 *      - TRAP_GMRP,
 *      - TRAP_GVRP,
 *      - TRAP_UNDEF_GARP_22,
 *      - TRAP_UNDEF_GARP_23,
 *      - TRAP_UNDEF_GARP_24,
 *      - TRAP_UNDEF_GARP_25,
 *      - TRAP_UNDEF_GARP_26,
 *      - TRAP_UNDEF_GARP_27,
 *      - TRAP_UNDEF_GARP_28,
 *      - TRAP_UNDEF_GARP_29,
 *      - TRAP_UNDEF_GARP_2A,
 *      - TRAP_UNDEF_GARP_2B,
 *      - TRAP_UNDEF_GARP_2C,
 *      - TRAP_UNDEF_GARP_2D,
 *      - TRAP_UNDEF_GARP_2E,
 *      - TRAP_UNDEF_GARP_2F,
 *      - TRAP_CDP.
 *      - TRAP_CSSTP.
 *      - TRAP_LLDP.
 *      (2) The RMA action is as following:
 *      - RMA_ACTION_FORWARD
 *      - RMA_ACTION_TRAP2CPU
 *      - RMA_ACTION_DROP
 *      - RMA_ACTION_FORWARD_EXCLUDE_CPU
 */
extern l2sw_api_ret_t l2sw_trap_rmaAction_get(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_trap_rma_action_t *pRma_action);

/* Function Name:
 *      l2sw_trap_rmaKeepFormat_set
 * Description:
 *      Set Reserved multicast address keep format configuration.
 * Input:
 *      type    - rma type.
 *      enable - enable keep format.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_ENABLE       - Invalid IFG parameter
 * Note:
 *
 *      There are 48 types of Reserved Multicast Address frame for application usage.
 *      They are as following definition.
 *      - TRAP_BRG_GROUP,
 *      - TRAP_FD_PAUSE,
 *      - TRAP_SP_MCAST,
 *      - TRAP_1X_PAE,
 *      - TRAP_UNDEF_BRG_04,
 *      - TRAP_UNDEF_BRG_05,
 *      - TRAP_UNDEF_BRG_06,
 *      - TRAP_UNDEF_BRG_07,
 *      - TRAP_PROVIDER_BRIDGE_GROUP_ADDRESS,
 *      - TRAP_UNDEF_BRG_09,
 *      - TRAP_UNDEF_BRG_0A,
 *      - TRAP_UNDEF_BRG_0B,
 *      - TRAP_UNDEF_BRG_0C,
 *      - TRAP_PROVIDER_BRIDGE_GVRP_ADDRESS,
 *      - TRAP_8021AB,
 *      - TRAP_UNDEF_BRG_0F,
 *      - TRAP_BRG_MNGEMENT,
 *      - TRAP_UNDEFINED_11,
 *      - TRAP_UNDEFINED_12,
 *      - TRAP_UNDEFINED_13,
 *      - TRAP_UNDEFINED_14,
 *      - TRAP_UNDEFINED_15,
 *      - TRAP_UNDEFINED_16,
 *      - TRAP_UNDEFINED_17,
 *      - TRAP_UNDEFINED_18,
 *      - TRAP_UNDEFINED_19,
 *      - TRAP_UNDEFINED_1A,
 *      - TRAP_UNDEFINED_1B,
 *      - TRAP_UNDEFINED_1C,
 *      - TRAP_UNDEFINED_1D,
 *      - TRAP_UNDEFINED_1E,
 *      - TRAP_UNDEFINED_1F,
 *      - TRAP_GMRP,
 *      - TRAP_GVRP,
 *      - TRAP_UNDEF_GARP_22,
 *      - TRAP_UNDEF_GARP_23,
 *      - TRAP_UNDEF_GARP_24,
 *      - TRAP_UNDEF_GARP_25,
 *      - TRAP_UNDEF_GARP_26,
 *      - TRAP_UNDEF_GARP_27,
 *      - TRAP_UNDEF_GARP_28,
 *      - TRAP_UNDEF_GARP_29,
 *      - TRAP_UNDEF_GARP_2A,
 *      - TRAP_UNDEF_GARP_2B,
 *      - TRAP_UNDEF_GARP_2C,
 *      - TRAP_UNDEF_GARP_2D,
 *      - TRAP_UNDEF_GARP_2E,
 *      - TRAP_UNDEF_GARP_2F,
 *      - TRAP_CDP.
 *      - TRAP_CSSTP.
 *      - TRAP_LLDP.
 */
extern l2sw_api_ret_t l2sw_trap_rmaKeepFormat_set(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_trap_rmaKeepFormat_get
 * Description:
 *      Get Reserved multicast address action configuration.
 * Input:
 *      type - rma type.
 * Output:
 *      pEnable - keep format status.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      There are 48 types of Reserved Multicast Address frame for application usage.
 *      They are as following definition.
 *      - TRAP_BRG_GROUP,
 *      - TRAP_FD_PAUSE,
 *      - TRAP_SP_MCAST,
 *      - TRAP_1X_PAE,
 *      - TRAP_UNDEF_BRG_04,
 *      - TRAP_UNDEF_BRG_05,
 *      - TRAP_UNDEF_BRG_06,
 *      - TRAP_UNDEF_BRG_07,
 *      - TRAP_PROVIDER_BRIDGE_GROUP_ADDRESS,
 *      - TRAP_UNDEF_BRG_09,
 *      - TRAP_UNDEF_BRG_0A,
 *      - TRAP_UNDEF_BRG_0B,
 *      - TRAP_UNDEF_BRG_0C,
 *      - TRAP_PROVIDER_BRIDGE_GVRP_ADDRESS,
 *      - TRAP_8021AB,
 *      - TRAP_UNDEF_BRG_0F,
 *      - TRAP_BRG_MNGEMENT,
 *      - TRAP_UNDEFINED_11,
 *      - TRAP_UNDEFINED_12,
 *      - TRAP_UNDEFINED_13,
 *      - TRAP_UNDEFINED_14,
 *      - TRAP_UNDEFINED_15,
 *      - TRAP_UNDEFINED_16,
 *      - TRAP_UNDEFINED_17,
 *      - TRAP_UNDEFINED_18,
 *      - TRAP_UNDEFINED_19,
 *      - TRAP_UNDEFINED_1A,
 *      - TRAP_UNDEFINED_1B,
 *      - TRAP_UNDEFINED_1C,
 *      - TRAP_UNDEFINED_1D,
 *      - TRAP_UNDEFINED_1E,
 *      - TRAP_UNDEFINED_1F,
 *      - TRAP_GMRP,
 *      - TRAP_GVRP,
 *      - TRAP_UNDEF_GARP_22,
 *      - TRAP_UNDEF_GARP_23,
 *      - TRAP_UNDEF_GARP_24,
 *      - TRAP_UNDEF_GARP_25,
 *      - TRAP_UNDEF_GARP_26,
 *      - TRAP_UNDEF_GARP_27,
 *      - TRAP_UNDEF_GARP_28,
 *      - TRAP_UNDEF_GARP_29,
 *      - TRAP_UNDEF_GARP_2A,
 *      - TRAP_UNDEF_GARP_2B,
 *      - TRAP_UNDEF_GARP_2C,
 *      - TRAP_UNDEF_GARP_2D,
 *      - TRAP_UNDEF_GARP_2E,
 *      - TRAP_UNDEF_GARP_2F,
 *      - TRAP_CDP.
 *      - TRAP_CSSTP.
 *      - TRAP_LLDP.
 */
extern l2sw_api_ret_t l2sw_trap_rmaKeepFormat_get(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_trap_rmaTrapToCpuPri_set
 * Description:
 *      Set Reserved multicast address trap to cpu pri.
 * Input:
 *      type    - rma type.
 *      pri - trap to cpu pri.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_ENABLE       - Invalid IFG parameter
 * Note:
 *
 *      There are 48 types of Reserved Multicast Address frame for application usage.
 *      They are as following definition.
 *      - TRAP_BRG_GROUP,
 *      - TRAP_FD_PAUSE,
 *      - TRAP_SP_MCAST,
 *      - TRAP_1X_PAE,
 *      - TRAP_UNDEF_BRG_04,
 *      - TRAP_UNDEF_BRG_05,
 *      - TRAP_UNDEF_BRG_06,
 *      - TRAP_UNDEF_BRG_07,
 *      - TRAP_PROVIDER_BRIDGE_GROUP_ADDRESS,
 *      - TRAP_UNDEF_BRG_09,
 *      - TRAP_UNDEF_BRG_0A,
 *      - TRAP_UNDEF_BRG_0B,
 *      - TRAP_UNDEF_BRG_0C,
 *      - TRAP_PROVIDER_BRIDGE_GVRP_ADDRESS,
 *      - TRAP_8021AB,
 *      - TRAP_UNDEF_BRG_0F,
 *      - TRAP_BRG_MNGEMENT,
 *      - TRAP_UNDEFINED_11,
 *      - TRAP_UNDEFINED_12,
 *      - TRAP_UNDEFINED_13,
 *      - TRAP_UNDEFINED_14,
 *      - TRAP_UNDEFINED_15,
 *      - TRAP_UNDEFINED_16,
 *      - TRAP_UNDEFINED_17,
 *      - TRAP_UNDEFINED_18,
 *      - TRAP_UNDEFINED_19,
 *      - TRAP_UNDEFINED_1A,
 *      - TRAP_UNDEFINED_1B,
 *      - TRAP_UNDEFINED_1C,
 *      - TRAP_UNDEFINED_1D,
 *      - TRAP_UNDEFINED_1E,
 *      - TRAP_UNDEFINED_1F,
 *      - TRAP_GMRP,
 *      - TRAP_GVRP,
 *      - TRAP_UNDEF_GARP_22,
 *      - TRAP_UNDEF_GARP_23,
 *      - TRAP_UNDEF_GARP_24,
 *      - TRAP_UNDEF_GARP_25,
 *      - TRAP_UNDEF_GARP_26,
 *      - TRAP_UNDEF_GARP_27,
 *      - TRAP_UNDEF_GARP_28,
 *      - TRAP_UNDEF_GARP_29,
 *      - TRAP_UNDEF_GARP_2A,
 *      - TRAP_UNDEF_GARP_2B,
 *      - TRAP_UNDEF_GARP_2C,
 *      - TRAP_UNDEF_GARP_2D,
 *      - TRAP_UNDEF_GARP_2E,
 *      - TRAP_UNDEF_GARP_2F,
 *      - TRAP_CDP.
 *      - TRAP_CSSTP.
 *      - TRAP_LLDP.
 */
extern l2sw_api_ret_t l2sw_trap_rmaTrapToCpuPri_set(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_pri_t pri);

/* Function Name:
 *      l2sw_trap_rmaTrapToCpuPri_get
 * Description:
 *      Get Reserved multicast address trap to cpu pri.
 * Input:
 *      type - rma type.
 * Output:
 *      pri - trap to cpu pri.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      There are 48 types of Reserved Multicast Address frame for application usage.
 *      They are as following definition.
 *      - TRAP_BRG_GROUP,
 *      - TRAP_FD_PAUSE,
 *      - TRAP_SP_MCAST,
 *      - TRAP_1X_PAE,
 *      - TRAP_UNDEF_BRG_04,
 *      - TRAP_UNDEF_BRG_05,
 *      - TRAP_UNDEF_BRG_06,
 *      - TRAP_UNDEF_BRG_07,
 *      - TRAP_PROVIDER_BRIDGE_GROUP_ADDRESS,
 *      - TRAP_UNDEF_BRG_09,
 *      - TRAP_UNDEF_BRG_0A,
 *      - TRAP_UNDEF_BRG_0B,
 *      - TRAP_UNDEF_BRG_0C,
 *      - TRAP_PROVIDER_BRIDGE_GVRP_ADDRESS,
 *      - TRAP_8021AB,
 *      - TRAP_UNDEF_BRG_0F,
 *      - TRAP_BRG_MNGEMENT,
 *      - TRAP_UNDEFINED_11,
 *      - TRAP_UNDEFINED_12,
 *      - TRAP_UNDEFINED_13,
 *      - TRAP_UNDEFINED_14,
 *      - TRAP_UNDEFINED_15,
 *      - TRAP_UNDEFINED_16,
 *      - TRAP_UNDEFINED_17,
 *      - TRAP_UNDEFINED_18,
 *      - TRAP_UNDEFINED_19,
 *      - TRAP_UNDEFINED_1A,
 *      - TRAP_UNDEFINED_1B,
 *      - TRAP_UNDEFINED_1C,
 *      - TRAP_UNDEFINED_1D,
 *      - TRAP_UNDEFINED_1E,
 *      - TRAP_UNDEFINED_1F,
 *      - TRAP_GMRP,
 *      - TRAP_GVRP,
 *      - TRAP_UNDEF_GARP_22,
 *      - TRAP_UNDEF_GARP_23,
 *      - TRAP_UNDEF_GARP_24,
 *      - TRAP_UNDEF_GARP_25,
 *      - TRAP_UNDEF_GARP_26,
 *      - TRAP_UNDEF_GARP_27,
 *      - TRAP_UNDEF_GARP_28,
 *      - TRAP_UNDEF_GARP_29,
 *      - TRAP_UNDEF_GARP_2A,
 *      - TRAP_UNDEF_GARP_2B,
 *      - TRAP_UNDEF_GARP_2C,
 *      - TRAP_UNDEF_GARP_2D,
 *      - TRAP_UNDEF_GARP_2E,
 *      - TRAP_UNDEF_GARP_2F,
 *      - TRAP_CDP.
 *      - TRAP_CSSTP.
 *      - TRAP_LLDP.
 */
extern l2sw_api_ret_t l2sw_trap_rmaTrapToCpuPri_get(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_pri_t *pri);

/* Function Name:
 *      l2sw_trap_rldpPri_get
 * Description:
 *      Get rldp priority value of a packet that trapped to CPU port.
 * Input:
 *      None
 * Output:
 *      pri      - priority.
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 * Note:
 */
extern l2sw_api_ret_t l2sw_trap_rldpPri_get(l2sw_uint8 units, l2sw_pri_t *pri);

/* Function Name:
 *      l2sw_trap_rldpPri_set
 * Description:
 *      Set rldp priority value of a packet that trapped to CPU port.
 * Input:
 *      pri      - priority.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 * Note:
 */
extern l2sw_api_ret_t l2sw_trap_rldpPri_set(l2sw_uint8 units, l2sw_pri_t pri);
#endif /* __L2SW_API_TRAP_H__ */


