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
#include "oam.h"
#include "sal_string.h"
#include "l2sw_asicdrv_oam.h"


/* Module Name : OAM */

static l2sw_api_ret_t _l2sw_oam_init(l2sw_uint8 units)
{
    return L2SW_ERR_OK;
} /* end of l2sw_oam_init */

static l2sw_api_ret_t _l2sw_oam_state_set(l2sw_uint8 units, l2sw_enable_t enabled)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (enabled >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicOamEnable(units, enabled))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_oam_state_get(l2sw_uint8 units, l2sw_enable_t *pEnabled)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((retVal = l2sw_getAsicOamEnable(units, pEnabled))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_oam_parserAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_oam_parser_act_t action)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (action >= OAM_PARSER_ACTION_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicOamParser(units, l2sw_switch_port_L2P_get(units, port), action))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_oam_parserAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_oam_parser_act_t *pAction)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_getAsicOamParser(units, l2sw_switch_port_L2P_get(units, port), pAction))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_oam_multiplexerAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_oam_multiplexer_act_t action)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (action >= OAM_MULTIPLEXER_ACTION_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicOamMultiplexer(units, l2sw_switch_port_L2P_get(units, port), action))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_oam_multiplexerAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_oam_multiplexer_act_t *pAction)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_getAsicOamMultiplexer(units, l2sw_switch_port_L2P_get(units, port), pAction))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

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
l2sw_api_ret_t l2sw_oam_init(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_oam_init(units);    
    L2SW_API_UNLOCK(units);

    return retVal;
} /* end of l2sw_oam_init */


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
l2sw_api_ret_t l2sw_oam_state_set(l2sw_uint8 units, l2sw_enable_t enabled)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_oam_state_set(units, enabled);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_oam_state_get(l2sw_uint8 units, l2sw_enable_t *pEnabled)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_oam_state_get(units, pEnabled);    
    L2SW_API_UNLOCK(units);

    return retVal;
}



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
l2sw_api_ret_t  l2sw_oam_parserAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_oam_parser_act_t action)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_oam_parserAction_set(units, port, action);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_oam_parserAction_set
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
l2sw_api_ret_t  l2sw_oam_parserAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_oam_parser_act_t *pAction)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_oam_parserAction_get(units, port, pAction);    
    L2SW_API_UNLOCK(units);

    return retVal;
}


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
l2sw_api_ret_t  l2sw_oam_multiplexerAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_oam_multiplexer_act_t action)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_oam_multiplexerAction_set(units, port, action);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_oam_parserAction_set
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
l2sw_api_ret_t  l2sw_oam_multiplexerAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_oam_multiplexer_act_t *pAction)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_oam_multiplexerAction_get(units, port, pAction);    
    L2SW_API_UNLOCK(units);

    return retVal;
}


