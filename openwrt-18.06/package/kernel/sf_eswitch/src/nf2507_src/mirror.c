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
#include "mirror.h"
#include "sal_string.h"
#include "l2sw_asicdrv_mirror.h"

static l2sw_api_ret_t _l2sw_mirror_portBased_set(l2sw_uint8 units, l2sw_port_t mirroring_port, l2sw_portmask_t *pMirrored_rx_portmask, l2sw_portmask_t *pMirrored_tx_portmask)
{
    l2sw_api_ret_t retVal;
    l2sw_enable_t mirRx, mirTx;
    l2sw_uint32 i, pmask;
    l2sw_port_t source_port;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port valid */
    L2SW_CHK_PORT_VALID(units, mirroring_port);

    if(NULL == pMirrored_rx_portmask)
        return L2SW_ERR_NULL_POINTER;

    if(NULL == pMirrored_tx_portmask)
        return L2SW_ERR_NULL_POINTER;

    L2SW_CHK_PORTMASK_VALID(units, pMirrored_rx_portmask);

    L2SW_CHK_PORTMASK_VALID(units, pMirrored_tx_portmask);

    /*Mirror Sorce Port Mask Check*/
    if (pMirrored_tx_portmask->bits[0]!=pMirrored_rx_portmask->bits[0]&&pMirrored_tx_portmask->bits[0]!=0&&pMirrored_rx_portmask->bits[0]!=0)
        return L2SW_ERR_PORT_MASK;

     /*mirror port != source port*/
    if(L2SW_PORTMASK_IS_PORT_SET((*pMirrored_tx_portmask), mirroring_port) || L2SW_PORTMASK_IS_PORT_SET((*pMirrored_rx_portmask), mirroring_port))
        return L2SW_ERR_PORT_MASK;

    source_port = l2sw_switch_maxLogicalPort_get(units);

    L2SW_SCAN_ALL_LOG_PORT(units, i)
    {
        if (pMirrored_tx_portmask->bits[0]&(1<<i))
        {
            source_port = i;
            break;
        }

        if (pMirrored_rx_portmask->bits[0]&(1<<i))
        {
            source_port = i;
            break;
        }
    }

    if ((retVal = l2sw_setAsicPortMirror(units, l2sw_switch_port_L2P_get(units, source_port), l2sw_switch_port_L2P_get(units, mirroring_port))) != L2SW_ERR_OK)
        return retVal;
    if(pMirrored_rx_portmask->bits[0] != 0)
    {
        if ((retVal = l2sw_switch_portmask_L2P_get(units, pMirrored_rx_portmask, &pmask)) != L2SW_ERR_OK)
            return retVal;
        if ((retVal = l2sw_setAsicPortMirrorMask(units, pmask)) != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        if ((retVal = l2sw_switch_portmask_L2P_get(units, pMirrored_tx_portmask, &pmask)) != L2SW_ERR_OK)
            return retVal;
        if ((retVal = l2sw_setAsicPortMirrorMask(units, pmask)) != L2SW_ERR_OK)
            return retVal;
    }


    if (pMirrored_rx_portmask->bits[0])
        mirRx = L2SW_ENABLED;
    else
        mirRx = L2SW_DISABLED;

    if ((retVal = l2sw_setAsicPortMirrorRxFunction(units, mirRx)) != L2SW_ERR_OK)
        return retVal;

    if (pMirrored_tx_portmask->bits[0])
        mirTx = L2SW_ENABLED;
    else
        mirTx = L2SW_DISABLED;

    if ((retVal = l2sw_setAsicPortMirrorTxFunction(units, mirTx)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;

}

static l2sw_api_ret_t _l2sw_mirror_portBased_get(l2sw_uint8 units, l2sw_port_t *pMirroring_port, l2sw_portmask_t *pMirrored_rx_portmask, l2sw_portmask_t *pMirrored_tx_portmask)
{
    l2sw_api_ret_t retVal;
    l2sw_port_t source_port;
    l2sw_enable_t mirRx, mirTx;
    l2sw_uint32 sport, mport, pmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);
    mirRx=0;
    mirTx=0;
    if(NULL == pMirrored_rx_portmask)
        return L2SW_ERR_NULL_POINTER;

    if(NULL == pMirrored_tx_portmask)
        return L2SW_ERR_NULL_POINTER;

    if(NULL == pMirroring_port)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortMirror(units, &sport, &mport)) != L2SW_ERR_OK)
        return retVal;
    source_port = l2sw_switch_port_P2L_get(units, sport);
    *pMirroring_port = l2sw_switch_port_P2L_get(units, mport);

    if ((retVal = l2sw_getAsicPortMirrorRxEnable(units, &mirRx)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicPortMirrorTxEnable(units, &mirTx)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicPortMirrorMask(units, &pmask)) != L2SW_ERR_OK)
        return retVal;

    if (L2SW_DISABLED == mirRx)
        pMirrored_rx_portmask->bits[0]=0;
    else
    {
        if ((retVal = l2sw_switch_portmask_P2L_get(units, pmask, pMirrored_rx_portmask)) != L2SW_ERR_OK)
            return retVal;
        //pMirrored_rx_portmask->bits[0] |= 1<<source_port;
    }

    if (L2SW_DISABLED == mirTx)
        pMirrored_tx_portmask->bits[0]=0;
    else
    {
        if ((retVal = l2sw_switch_portmask_P2L_get(units, pmask, pMirrored_tx_portmask)) != L2SW_ERR_OK)
            return retVal;
        //pMirrored_tx_portmask->bits[0] |= 1<<source_port;
    }

    return L2SW_ERR_OK;

}


static l2sw_api_ret_t _l2sw_mirror_isolationLeaky_set(l2sw_uint8 units, l2sw_enable_t txenable, l2sw_enable_t rxenable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((txenable >= L2SW_ENABLE_END) ||(rxenable >= L2SW_ENABLE_END))
        return L2SW_ERR_ENABLE;

    if ((retVal = l2sw_setAsicPortMirrorIsolationTxLeaky(units, txenable)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicPortMirrorIsolationRxLeaky(units, rxenable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_mirror_isolationLeaky_get(l2sw_uint8 units, l2sw_enable_t *pTxenable, l2sw_enable_t *pRxenable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if( (NULL == pTxenable) || (NULL == pRxenable) )
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortMirrorIsolationTxLeaky(units, pTxenable)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicPortMirrorIsolationRxLeaky(units, pRxenable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_mirror_enable_set(l2sw_uint8 units,l2sw_enable_t txEnable, l2sw_enable_t rxEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (rxEnable >= L2SW_ENABLE_END || txEnable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if ((retVal = l2sw_setAsicPortMirrorRxEnable(units, rxEnable)) != L2SW_ERR_OK){
        //display("l2sw_setAsicPortMirrorRxEnable failed\n");
        return retVal;
    }
    if ((retVal = l2sw_setAsicPortMirrorTxEnable(units, txEnable)) != L2SW_ERR_OK){
        //display("l2sw_setAsicPortMirrorTxEnable failed\n");
        return retVal;
    }
    return L2SW_ERR_OK;


}

static l2sw_api_ret_t _l2sw_mirror_enable_get(l2sw_uint8 units,l2sw_enable_t *pTxEnable, l2sw_enable_t *pRxEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if( (NULL == pTxEnable) || (NULL == pRxEnable) )
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortMirrorRxEnable(units, pRxEnable)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicPortMirrorTxEnable(units, pTxEnable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;


}

static l2sw_api_ret_t _l2sw_mirror_leaky_oam_set(l2sw_uint8 units, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if ((retVal = l2sw_setAsicPortMirrorOamLeaky(units, enable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;

}

static l2sw_api_ret_t _l2sw_mirror_leaky_oam_get(l2sw_uint8 units, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if( NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortMirrorOamLeaky(units, pEnable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;

}

static l2sw_api_ret_t _l2sw_mirror_include_acl_portlist_set(l2sw_uint8 units,l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if ((retVal = l2sw_setAsicPortMirrorIncludeAclPortList(units, enable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;

}

static l2sw_api_ret_t _l2sw_mirror_include_acl_portlist_get(l2sw_uint8 units,l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if( NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortMirrorIncludeAclPortList(units, pEnable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;

}

/* Function Name:
 *      l2sw_mirror_portBased_set
 * Description:
 *      Set port mirror function.
 * Input:
 *      mirroring_port          - Monitor port.
 *      pMirrored_rx_portmask   - Rx mirror port mask.
 *      pMirrored_tx_portmask   - Tx mirror port mask.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_PORT_MASK    - Invalid portmask.
 * Note:
 *      The API is to set mirror function of source port and mirror port.
 *      The mirror port can only be set to one port and the TX and RX mirror ports
 *      should be identical.
 */
l2sw_api_ret_t l2sw_mirror_portBased_set(l2sw_uint8 units, l2sw_port_t mirroring_port, l2sw_portmask_t *pMirrored_rx_portmask, l2sw_portmask_t *pMirrored_tx_portmask)
{
     l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_mirror_portBased_set(units, mirroring_port, pMirrored_rx_portmask, pMirrored_tx_portmask);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_mirror_portBased_get
 * Description:
 *      Get port mirror function.
 * Input:
 *      None
 * Output:
 *      pMirroring_port         - Monitor port.
 *      pMirrored_rx_portmask   - Rx mirror port mask.
 *      pMirrored_tx_portmask   - Tx mirror port mask.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API is to get mirror function of source port and mirror port.
 */
l2sw_api_ret_t l2sw_mirror_portBased_get(l2sw_uint8 units, l2sw_port_t *pMirroring_port, l2sw_portmask_t *pMirrored_rx_portmask, l2sw_portmask_t *pMirrored_tx_portmask)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_mirror_portBased_get(units, pMirroring_port, pMirrored_rx_portmask, pMirrored_tx_portmask);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_mirror_isolationLeaky_set
 * Description:
 *      Set mirror Isolation leaky.
 * Input:
 *      txenable -TX leaky enable.
 *      rxenable - RX leaky enable.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to set mirror VLAN leaky function forwarding packets to miror port.
 */
l2sw_api_ret_t l2sw_mirror_isolationLeaky_set(l2sw_uint8 units, l2sw_enable_t txenable, l2sw_enable_t rxenable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_mirror_isolationLeaky_set(units, txenable, rxenable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_mirror_isolationLeaky_get
 * Description:
 *      Get mirror isolation leaky.
 * Input:
 *      None
 * Output:
 *      pTxenable - TX leaky enable.
 *      pRxenable - RX leaky enable.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API is to get mirror isolation leaky status.
 */
l2sw_api_ret_t l2sw_mirror_isolationLeaky_get(l2sw_uint8 units, l2sw_enable_t *pTxenable, l2sw_enable_t *pRxenable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_mirror_isolationLeaky_get(units, pTxenable, pRxenable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_mirror_enable_set
 * Description:
 *      Set mirror function status.
 * Input:
 *      txenable -TX  enable.
 *      rxenable - RX  enable.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to set mirror enable function.
 */
l2sw_api_ret_t l2sw_mirror_enable_set(l2sw_uint8 units,l2sw_enable_t txEnable, l2sw_enable_t rxEnable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_mirror_enable_set(units, txEnable, rxEnable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_mirror_enable_get
 * Description:
 *      Get mirror function status.
 * Input:
 *      None
 * Output:
 *      pTxEnable - TX leaky enable.
 *      pRxEnable - RX leaky enable.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to get mirror enable function.
 */
l2sw_api_ret_t l2sw_mirror_enable_get(l2sw_uint8 units,l2sw_enable_t *pTxEnable, l2sw_enable_t *pRxEnable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_mirror_enable_get(units, pTxEnable, pRxEnable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_mirror_leaky_oam_set
 * Description:
 *      Set mirror oam packet leaky enable.
 * Input:
 *      enable -  enable.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to set mirror oam packet leaky function.
 */
l2sw_api_ret_t l2sw_mirror_leaky_oam_set(l2sw_uint8 units,l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_mirror_leaky_oam_set(units, enable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_mirror_leaky_oam_get
 * Description:
 *      Get mirror oam packet leaky enable.
 * Input:
 *      None
 * Output:
 *      pEnable
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to set mirror oam packet leaky function.
 */
l2sw_api_ret_t l2sw_mirror_leaky_oam_get(l2sw_uint8 units,l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_mirror_leaky_oam_get(units, pEnable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_mirror_include_acl_portlist_set
 * Description:
 *      Set mirror include acl portlist.
 * Input:
 *      enable -  enable.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to set mirror include acl packet portlist function.
 */
l2sw_api_ret_t l2sw_mirror_include_acl_portlist_set(l2sw_uint8 units,l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_mirror_include_acl_portlist_set(units, enable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_mirror_include_acl_portlist_get
 * Description:
 *      Set mirror include acl portlist.
 * Input:
 *      None
 * Output:
 *      pEnable
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to get mirror include acl packet portlist function.
 */
l2sw_api_ret_t l2sw_mirror_include_acl_portlist_get(l2sw_uint8 units,l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_mirror_include_acl_portlist_get(units, pEnable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

