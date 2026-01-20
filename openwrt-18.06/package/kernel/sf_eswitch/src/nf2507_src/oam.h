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

#ifndef __L2SW_OAM_H__
#define __L2SW_OAM_H__

/*
 * Symbol Definition
 */


/*
 * Data Declaration
 */


/*
 * Macro Declaration
 */

typedef enum l2sw_oam_parser_act_e
{
    OAM_PARSER_ACTION_FORWARD = 0,
    OAM_PARSER_ACTION_LOOPBACK,
    OAM_PARSER_ACTION_DISCARD,
    OAM_PARSER_ACTION_END,

} l2sw_oam_parser_act_t;

typedef enum l2sw_oam_multiplexer_act_e
{
    OAM_MULTIPLEXER_ACTION_FORWARD = 0,
    OAM_MULTIPLEXER_ACTION_DISCARD,
    OAM_MULTIPLEXER_ACTION_CPUONLY,
    OAM_MULTIPLEXER_ACTION_END,

} l2sw_oam_multiplexer_act_t;


/*
 * Function Declaration
 */

/* Function Name:
 *      l2sw_oam_init
 * Description:
 *      Initialize oam module.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 * Note:
 *      Must initialize oam module before calling any oam APIs.
 */
extern l2sw_api_ret_t l2sw_oam_init(l2sw_uint8 units);

/* Function Name:
 *      l2sw_oam_state_set
 * Description:
 *      This API set OAM state.
 * Input:
 *      enabled     -OAMstate
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Error parameter
 * Note:
 *      This API set OAM state.
 */
extern l2sw_api_ret_t l2sw_oam_state_set(l2sw_uint8 units, l2sw_enable_t enabled);

/* Function Name:
 *      l2sw_oam_state_get
 * Description:
 *      This API get OAM state.
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
 *      This API set current OAM state.
 */
extern l2sw_api_ret_t l2sw_oam_state_get(l2sw_uint8 units, l2sw_enable_t *pEnabled);


/* Module Name : OAM */

/* Function Name:
 *      l2sw_oam_parserAction_set
 * Description:
 *      Set OAM parser action
 * Input:
 *      port    - port id
 *      action  - parser action
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_PORT_ID      - invalid port id
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_oam_parserAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_oam_parser_act_t action);

/* Function Name:
 *      l2sw_oam_parserAction_get
 * Description:
 *      Get OAM parser action
 * Input:
 *      port    - port id
 * Output:
 *      pAction  - parser action
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_PORT_ID      - invalid port id
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_oam_parserAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_oam_parser_act_t *pAction);


/* Function Name:
 *      l2sw_oam_multiplexerAction_set
 * Description:
 *      Set OAM multiplexer action
 * Input:
 *      port    - port id
 *      action  - parser action
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_PORT_ID      - invalid port id
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_oam_multiplexerAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_oam_multiplexer_act_t action);

/* Function Name:
 *      l2sw_oam_multiplexerAction_get
 * Description:
 *      Get OAM multiplexer action
 * Input:
 *      port    - port id
 * Output:
 *      pAction  - parser action
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_PORT_ID      - invalid port id
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_oam_multiplexerAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_oam_multiplexer_act_t *pAction);


#endif /* __L2SW_OAM_H__ */

