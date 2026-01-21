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
#include "led.h"
#include "sal_string.h"
#include "l2sw_asicdrv_led.h"

static l2sw_api_ret_t _l2sw_led_enable_set(l2sw_uint8 units, l2sw_led_group_t group, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmask;
    l2sw_port_t port;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (group >= LED_GROUP_END)
        return L2SW_ERR_INPUT;

    L2SW_CHK_PORTMASK_VALID(units, pPortmask);

    L2SW_PORTMASK_SCAN((*pPortmask), port)
    {
        if(l2sw_switch_isCPUPort(units, port) == L2SW_ERR_OK)
            return L2SW_ERR_PORT_MASK;
    }

    if((retVal = l2sw_switch_portmask_L2P_get(units, pPortmask, &pmask)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicLedGroupEnable(units, group, pmask)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_led_enable_get(l2sw_uint8 units, l2sw_led_group_t group, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (group >= LED_GROUP_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_getAsicLedGroupEnable(units, group, &pmask)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_switch_portmask_P2L_get(units, pmask, pPortmask)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;

}


static l2sw_api_ret_t _l2sw_led_blinkRate_set(l2sw_uint8 units, l2sw_led_blink_rate_t blinkRate)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (blinkRate >= LED_BLINKRATE_END)
        return L2SW_ERR_FAILED;

    if ((retVal = l2sw_setAsicLedBlinkRate(units, blinkRate)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_led_blinkRate_get(l2sw_uint8 units, l2sw_led_blink_rate_t *pBlinkRate)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pBlinkRate)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicLedBlinkRate(units, pBlinkRate)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_led_groupConfig_set(l2sw_uint8 units, l2sw_led_group_t group, l2sw_led_congig_t config)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (LED_GROUP_END <= group)
        return L2SW_ERR_FAILED;

    if (LED_CONFIG_END <= config)
        return L2SW_ERR_FAILED;

    if ((retVal = l2sw_setAsicLedIndicateInfoConfig(units, group, config)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_led_groupConfig_get(l2sw_uint8 units, l2sw_led_group_t group, l2sw_led_congig_t *pConfig)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (LED_GROUP_END <= group)
        return L2SW_ERR_FAILED;

    if(NULL == pConfig)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicLedIndicateInfoConfig(units, group, pConfig)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_led_OutputEnable_set(l2sw_uint8 units, l2sw_enable_t state)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (state >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicLedOutputEnable(units, state))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_led_OutputEnable_get(l2sw_uint8 units, l2sw_enable_t *pState)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(pState == NULL)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicLedOutputEnable(units, pState))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;

}

/* Function Name:
 *      l2sw_led_enable_set
 * Description:
 *      Set Led enable congiuration
 * Input:
 *      group       - LED group id.
 *      pPortmask   - LED enable port mask.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_MASK    - Error portmask
 * Note:
 *      The API can be used to enable LED per port per group.
 */
l2sw_api_ret_t l2sw_led_enable_set(l2sw_uint8 units, l2sw_led_group_t group, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_led_enable_set(units, group, pPortmask);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_led_enable_get
 * Description:
 *      Get Led enable congiuration
 * Input:
 *      group - LED group id.
 * Output:
 *      pPortmask - LED enable port mask.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API can be used to get LED enable status.
 */
l2sw_api_ret_t l2sw_led_enable_get(l2sw_uint8 units, l2sw_led_group_t group, l2sw_portmask_t *pPortmask)
{
     l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_led_enable_get(units, group, pPortmask);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_led_blinkRate_set
 * Description:
 *      Set LED blinking rate
 * Input:
 *      blinkRate - blinking rate.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      ASIC support 6 types of LED blinking rates at 43ms, 84ms, 120ms, 170ms, 340ms and 670ms.
 */
l2sw_api_ret_t l2sw_led_blinkRate_set(l2sw_uint8 units, l2sw_led_blink_rate_t blinkRate)
{
     l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_led_blinkRate_set(units, blinkRate);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_led_blinkRate_get
 * Description:
 *      Get LED blinking rate at mode 0 to mode 3
 * Input:
 *      None
 * Output:
 *      pBlinkRate - blinking rate.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      There are  6 types of LED blinking rates at 43ms, 84ms, 120ms, 170ms, 340ms and 670ms.
 */
l2sw_api_ret_t l2sw_led_blinkRate_get(l2sw_uint8 units, l2sw_led_blink_rate_t *pBlinkRate)
{
     l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_led_blinkRate_get(units, pBlinkRate);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_led_groupConfig_set
 * Description:
 *      Set per group Led to congiuration mode
 * Input:
 *      group   - LED group.
 *      config  - LED configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API can set LED indicated information configuration for each LED group with 1 to 1 led mapping to each port.
 *      - Definition  LED Statuses      Description
 *      - 0000        LED_Off           LED pin Tri-State.
 *      - 0001        Dup/Col           Collision, Full duplex Indicator.
 *      - 0010        Link/Act          Link, Activity Indicator.
 *      - 0011        Spd1000           1000Mb/s Speed Indicator.
 *      - 0100        Spd100            100Mb/s Speed Indicator.
 *      - 0101        Spd10             10Mb/s Speed Indicator.
 *      - 0110        Spd1000/Act       1000Mb/s Speed/Activity Indicator.
 *      - 0111        Spd100/Act        100Mb/s Speed/Activity Indicator.
 *      - 1000        Spd10/Act         10Mb/s Speed/Activity Indicator.
 *      - 1001        Spd100 (10)/Act   10/100Mb/s Speed/Activity Indicator.
 *      - 1010        LoopDetect        LoopDetect Indicator.
 *      - 1011        EEE               EEE Indicator.
 *      - 1100        Link/Rx           Link, Activity Indicator.
 *      - 1101        Link/Tx           Link, Activity Indicator.
 *      - 1110        Master            Link on Master Indicator.
 *      - 1111        Act               Activity Indicator. Low for link established.
 */
l2sw_api_ret_t l2sw_led_groupConfig_set(l2sw_uint8 units, l2sw_led_group_t group, l2sw_led_congig_t config)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_led_groupConfig_set(units, group, config);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_led_groupConfig_get
 * Description:
 *      Get Led group congiuration mode
 * Input:
 *      group - LED group.
 * Output:
 *      pConfig - LED configuration.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *       The API can get LED indicated information configuration for each LED group.
 */
l2sw_api_ret_t l2sw_led_groupConfig_get(l2sw_uint8 units, l2sw_led_group_t group, l2sw_led_congig_t *pConfig)
{
     l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_led_groupConfig_get(units, group, pConfig);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_led_OutputEnable_set
 * Description:
 *      This API set LED I/O state.
 * Input:
 *      enabled     - LED I/O state
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Error parameter
 * Note:
 *      This API set LED I/O state.
 */
l2sw_api_ret_t l2sw_led_OutputEnable_set(l2sw_uint8 units, l2sw_enable_t state)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_led_OutputEnable_set(units, state);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_led_OutputEnable_get
 * Description:
 *      This API get LED I/O state.
 * Input:
 *      None.
 * Output:
 *      pEnabled        - LED I/O state
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Error parameter
 * Note:
 *      This API set current LED I/O  state.
 */
l2sw_api_ret_t l2sw_led_OutputEnable_get(l2sw_uint8 units, l2sw_enable_t *pState)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_led_OutputEnable_get(units, pState);    
    L2SW_API_UNLOCK(units);

    return retVal;
}
