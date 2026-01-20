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
#include "rldp.h"
#include "l2sw_asicdrv_rldp.h"

/*
 * Symbol Definition
 */


/*
 * Data Declaration
 */


/*
 * Macro Declaration
 */


/*
 * Function Declaration
 */

/* Module Name : RLDP */

static l2sw_api_ret_t _l2sw_rldp_config_set(l2sw_uint8 units, l2sw_rldp_config_t *pConfig)
{
    l2sw_api_ret_t retVal;
    ether_addr_t magic;
    l2sw_uint32 pmsk;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (pConfig->rldp_enable >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if (pConfig->trigger_mode >= L2SW_RLDP_TRIGGER_END)
        return L2SW_ERR_INPUT;

    if (pConfig->compare_type >= L2SW_RLDP_CMPTYPE_END)
        return L2SW_ERR_INPUT;

    if (pConfig->num_check >= L2SW_RLDP_NUM_MAX)
        return L2SW_ERR_INPUT;

    if (pConfig->interval_check >= L2SW_RLDP_INTERVAL_MAX)
        return L2SW_ERR_INPUT;

    if (pConfig->num_loop >= L2SW_RLDP_NUM_MAX)
        return L2SW_ERR_INPUT;

    if (pConfig->interval_loop >= L2SW_RLDP_INTERVAL_MAX)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_getAsicRldpTxPortmask(units, &pmsk))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicRldpTxPortmask(units, 0x00))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicRldpTxPortmask(units, pmsk))!=L2SW_ERR_OK)
        return retVal;
#if 0
    if ((retVal = l2sw_setAsicRldp(units, pConfig->rldp_enable))!=L2SW_ERR_OK)
        return retVal;
#else
    /* L2SW_ENABLED: first DISABLED, then MODIFY, last ENABLED. */
    /* L2SW_DISABLED: first DISABLED, then MODIFY. */
    if ((retVal = l2sw_setAsicRldp(units, L2SW_DISABLED))!=L2SW_ERR_OK)
        return retVal;
#endif
    if ((retVal = l2sw_setAsicRldpTriggerMode(units, pConfig->trigger_mode))!=L2SW_ERR_OK)
        return retVal;

    MEMCPY(&magic, &pConfig->magic, sizeof(ether_addr_t));
    if ((retVal = l2sw_setAsicRldpMagicNum(units, magic))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicRldpCompareRandomNumber(units, pConfig->compare_type))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicRldpCompareRandomNumber(units, pConfig->compare_type))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicRldpCheckingStatePara(units, pConfig->num_check, pConfig->interval_check))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicRldpLoopStatePara(units, pConfig->num_loop, pConfig->interval_loop))!=L2SW_ERR_OK)
        return retVal;
#if 1
    if (pConfig->rldp_enable == L2SW_ENABLED)
    {
        if ((retVal = l2sw_setAsicRldp(units, L2SW_ENABLED))!=L2SW_ERR_OK)
            return retVal;
    }
#endif
    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rldp_config_get(l2sw_uint8 units, l2sw_rldp_config_t *pConfig)
{
    l2sw_api_ret_t retVal;
    ether_addr_t magic;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((retVal = l2sw_getAsicRldp(units, &pConfig->rldp_enable))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicRldpTriggerMode(units, &pConfig->trigger_mode))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicRldpMagicNum(units, &magic))!=L2SW_ERR_OK)
        return retVal;
    MEMCPY(&pConfig->magic, &magic, sizeof(ether_addr_t));

    if ((retVal = l2sw_getAsicRldpCompareRandomNumber(units, &pConfig->compare_type))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicRldpCompareRandomNumber(units, &pConfig->compare_type))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicRldpCheckingStatePara(units, &pConfig->num_check, &pConfig->interval_check))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicRldpLoopStatePara(units, &pConfig->num_loop, &pConfig->interval_loop))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rldp_portConfig_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rldp_portConfig_t *pPortConfig)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmsk;
    l2sw_uint32 phy_port;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (pPortConfig->tx_enable>= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    phy_port = l2sw_switch_port_L2P_get(units, port);

    if ((retVal = l2sw_getAsicRldpTxPortmask(units, &pmsk))!=L2SW_ERR_OK)
        return retVal;

    if (pPortConfig->tx_enable)
    {
         pmsk |=(1<<phy_port);
    }
    else
    {
         pmsk &= ~(1<<phy_port);
    }

    if ((retVal = l2sw_setAsicRldpTxPortmask(units, pmsk))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;

}

static l2sw_api_ret_t _l2sw_rldp_portConfig_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rldp_portConfig_t *pPortConfig)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmsk;
    l2sw_portmask_t logicalPmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_getAsicRldpTxPortmask(units, &pmsk))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_switch_portmask_P2L_get(units, pmsk, &logicalPmask)) != L2SW_ERR_OK)
        return retVal;


    if (logicalPmask.bits[0] & (1<<port))
    {
         pPortConfig->tx_enable = L2SW_ENABLED;
    }
    else
    {
         pPortConfig->tx_enable = L2SW_DISABLED;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rldp_status_get(l2sw_uint8 units, l2sw_rldp_status_t *pStatus)
{
    l2sw_api_ret_t retVal;
    ether_addr_t seed;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((retVal = l2sw_getAsicRldpRandomNumber(units, &seed))!=L2SW_ERR_OK)
        return retVal;
    MEMCPY(&pStatus->id, &seed, sizeof(ether_addr_t));

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rldp_portStatus_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rldp_portStatus_t *pPortStatus)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmsk;
    l2sw_portmask_t logicalPmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_getAsicRldpLoopedPortmask(units, &pmsk))!=L2SW_ERR_OK)
        return retVal;
    if ((retVal = l2sw_switch_portmask_P2L_get(units, pmsk, &logicalPmask)) != L2SW_ERR_OK)
        return retVal;

    if (logicalPmask.bits[0] & (1<<port))
    {
         pPortStatus->loop_status = L2SW_RLDP_LOOPSTS_LOOPING;
    }
    else
    {
         pPortStatus->loop_status  = L2SW_RLDP_LOOPSTS_NONE;
    }

    if ((retVal = l2sw_getAsicRldpEnterLoopedPortmask(units, &pmsk))!=L2SW_ERR_OK)
        return retVal;
    if ((retVal = l2sw_switch_portmask_P2L_get(units, pmsk, &logicalPmask)) != L2SW_ERR_OK)
        return retVal;

    if (logicalPmask.bits[0] & (1<<port))
    {
         pPortStatus->loop_enter = L2SW_RLDP_LOOPSTS_LOOPING;
    }
    else
    {
         pPortStatus->loop_enter  = L2SW_RLDP_LOOPSTS_NONE;
    }

    if ((retVal = l2sw_getAsicRldpLeaveLoopedPortmask(units, &pmsk))!=L2SW_ERR_OK)
        return retVal;
    if ((retVal = l2sw_switch_portmask_P2L_get(units, pmsk, &logicalPmask)) != L2SW_ERR_OK)
        return retVal;

    if (logicalPmask.bits[0] & (1<<port))
    {
         pPortStatus->loop_leave = L2SW_RLDP_LOOPSTS_LOOPING;
    }
    else
    {
         pPortStatus->loop_leave  = L2SW_RLDP_LOOPSTS_NONE;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rldp_portStatus_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rldp_portStatus_t *pPortStatus)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmsk;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    pmsk = (pPortStatus->loop_enter)<<l2sw_switch_port_L2P_get(units, port);
    if ((retVal = l2sw_setAsicRldpEnterLoopedPortmask(units, pmsk))!=L2SW_ERR_OK)
        return retVal;

    pmsk = (pPortStatus->loop_leave)<<l2sw_switch_port_L2P_get(units, port);
    if ((retVal = l2sw_setAsicRldpLeaveLoopedPortmask(units, pmsk))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rldp_portLoopPair_get(l2sw_uint8 units, l2sw_port_t port, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmsk;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_getAsicRldpLoopedPortPair(units, l2sw_switch_port_L2P_get(units, port), &pmsk))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_switch_portmask_P2L_get(units, pmsk, pPortmask)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rldp_enable_set(l2sw_uint8 units,l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);


    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;
    
    if ((retVal = l2sw_setAsicRldpEnable8051(units, enable)) != L2SW_ERR_OK)
        return retVal;
    
    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rldp_enable_get(l2sw_uint8 units,l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);


    if (NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicRldpEnable8051(units, (l2sw_uint32 *)pEnable)) != L2SW_ERR_OK)
        return retVal;
    
    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rldp_trap_set(l2sw_uint8 units,l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);


    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;
    
    if ((retVal = l2sw_setAsicRlppTrap8051(units, enable)) != L2SW_ERR_OK)
        return retVal;
    
    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rldp_trap_get(l2sw_uint8 units,l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);


    if (NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicRlppTrap8051(units, (l2sw_uint32 *)pEnable)) != L2SW_ERR_OK)
        return retVal;
    
    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_rldp_config_set
 * Description:
 *      Set RLDP module configuration
 * Input:
 *      pConfig - configuration structure of RLDP
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_rldp_config_set(l2sw_uint8 units, l2sw_rldp_config_t *pConfig)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rldp_config_set(units, pConfig);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rldp_config_get
 * Description:
 *      Get RLDP module configuration
 * Input:
 *      None
 * Output:
 *      pConfig - configuration structure of RLDP
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_rldp_config_get(l2sw_uint8 units, l2sw_rldp_config_t *pConfig)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rldp_config_get(units, pConfig);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rldp_portConfig_set
 * Description:
 *      Set per port RLDP module configuration
 * Input:
 *      port   - port number to be configured
 *      pPortConfig - per port configuration structure of RLDP
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_rldp_portConfig_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rldp_portConfig_t *pPortConfig)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rldp_portConfig_set(units, port, pPortConfig);
    L2SW_API_UNLOCK(units);

    return retVal;
} /* end of l2sw_rldp_portConfig_set */


/* Function Name:
 *      l2sw_rldp_portConfig_get
 * Description:
 *      Get per port RLDP module configuration
 * Input:
 *      port    - port number to be get
 * Output:
 *      pPortConfig - per port configuration structure of RLDP
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_rldp_portConfig_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rldp_portConfig_t *pPortConfig)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rldp_portConfig_get(units, port, pPortConfig);
    L2SW_API_UNLOCK(units);

    return retVal;
} /* end of l2sw_rldp_portConfig_get */


/* Function Name:
 *      l2sw_rldp_status_get
 * Description:
 *      Get RLDP module status
 * Input:
 *      None
 * Output:
 *      pStatus - status structure of RLDP
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_rldp_status_get(l2sw_uint8 units, l2sw_rldp_status_t *pStatus)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rldp_status_get(units, pStatus);
    L2SW_API_UNLOCK(units);

    return retVal;
} /* end of l2sw_rldp_status_get */


/* Function Name:
 *      l2sw_rldp_portStatus_get
 * Description:
 *      Get RLDP module status
 * Input:
 *      port    - port number to be get
 * Output:
 *      pPortStatus - per port status structure of RLDP
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_rldp_portStatus_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rldp_portStatus_t *pPortStatus)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rldp_portStatus_get(units, port, pPortStatus);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rldp_portStatus_set
 * Description:
 *      Clear RLDP module status
 * Input:
 *      port    - port number to be clear
 *      pPortStatus - per port status structure of RLDP
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      Clear operation effect loop_enter and loop_leave only, other field in
 *      the structure are don't care. Loop status cab't be clean.
 */
l2sw_api_ret_t l2sw_rldp_portStatus_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rldp_portStatus_t *pPortStatus)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rldp_portStatus_set(units, port, pPortStatus);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rldp_portLoopPair_get
 * Description:
 *      Get RLDP port loop pairs
 * Input:
 *      port    - port number to be get
 * Output:
 *      pPortmask - per port related loop ports
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_rldp_portLoopPair_get(l2sw_uint8 units, l2sw_port_t port, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rldp_portLoopPair_get(units, port, pPortmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rldp_enable_set
 * Description:
 *      Set RLDP enable state
 * Input:
 *      enable - rldp enable state 
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_rldp_enable_set(l2sw_uint8 units,l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rldp_enable_set(units,enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rldp_enable_get
 * Description:
 *      Get RLDP enable state
 * Input:
 *      None
 * Output:
 *      pEnable - rldp enable state 
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_rldp_enable_get(l2sw_uint8 units,l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rldp_enable_get(units,pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rldp_trap_set
 * Description:
 *      Set RLDP trap enable state
 * Input:
 *      enable - rldp trap enable state 
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_rldp_trap_set(l2sw_uint8 units,l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rldp_trap_set(units,enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rldp_enable_get
 * Description:
 *      Get RLDP trap enable state
 * Input:
 *      None
 * Output:
 *      pEnable - rldp trap enable state 
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_rldp_trap_get(l2sw_uint8 units,l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rldp_trap_get(units,pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}




