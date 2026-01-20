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
#include "l2sw_error.h"
#include "storm.h"
#include "rate.h"
#include "sal_string.h"
#include "l2sw_asicdrv.h"
#include "l2sw_asicdrv_storm.h"
#include "l2sw_asicdrv_meter.h"
#include "l2sw_asicdrv_rma.h"
#include "l2sw_asicdrv_igmp.h"

static l2sw_api_ret_t _l2sw_rate_stormControlMeterIdx_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_storm_group_t stormType, l2sw_uint32 index)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (stormType >= STORM_GROUP_END)
        return L2SW_ERR_SFC_UNKNOWN_GROUP;

    if (index > L2SW_MAX_METER_ID)
        return L2SW_ERR_FILTER_METER_ID;

    switch (stormType)
    {
        case STORM_GROUP_UNKNOWN_UNICAST:
            if ((retVal = l2sw_setAsicStormFilterUnknownUnicastMeter(units, l2sw_switch_port_L2P_get(units, port), index))!=L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_UNKNOWN_MULTICAST:
            if ((retVal = l2sw_setAsicStormFilterUnknownMulticastMeter(units, l2sw_switch_port_L2P_get(units, port), index))!=L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_MULTICAST:
            if ((retVal = l2sw_setAsicStormFilterMulticastMeter(units, l2sw_switch_port_L2P_get(units, port), index))!=L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_BROADCAST:
            if ((retVal = l2sw_setAsicStormFilterBroadcastMeter(units, l2sw_switch_port_L2P_get(units, port), index))!=L2SW_ERR_OK)
                return retVal;
            break;
        default:
            break;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_stormControlMeterIdx_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_storm_group_t stormType, l2sw_uint32 *pIndex)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (stormType >= STORM_GROUP_END)
        return L2SW_ERR_SFC_UNKNOWN_GROUP;

    if (NULL == pIndex )
        return L2SW_ERR_NULL_POINTER;

    switch (stormType)
    {
        case STORM_GROUP_UNKNOWN_UNICAST:
            if ((retVal = l2sw_getAsicStormFilterUnknownUnicastMeter(units, l2sw_switch_port_L2P_get(units, port), pIndex))!=L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_UNKNOWN_MULTICAST:
            if ((retVal = l2sw_getAsicStormFilterUnknownMulticastMeter(units, l2sw_switch_port_L2P_get(units, port), pIndex))!=L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_MULTICAST:
            if ((retVal = l2sw_getAsicStormFilterMulticastMeter(units, l2sw_switch_port_L2P_get(units, port), pIndex))!=L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_BROADCAST:
            if ((retVal = l2sw_getAsicStormFilterBroadcastMeter(units, l2sw_switch_port_L2P_get(units, port), pIndex))!=L2SW_ERR_OK)
                return retVal;
            break;
        default:
            break;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_stormControlPortEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_storm_group_t stormType, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (stormType >= STORM_GROUP_END)
        return L2SW_ERR_SFC_UNKNOWN_GROUP;

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    switch (stormType)
    {
        case STORM_GROUP_UNKNOWN_UNICAST:
            if ((retVal = l2sw_setAsicStormFilterUnknownUnicastEnable(units, l2sw_switch_port_L2P_get(units, port), enable)) != L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_UNKNOWN_MULTICAST:
            if ((retVal = l2sw_setAsicStormFilterUnknownMulticastEnable(units, l2sw_switch_port_L2P_get(units, port), enable)) != L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_MULTICAST:
            if ((retVal = l2sw_setAsicStormFilterMulticastEnable(units, l2sw_switch_port_L2P_get(units, port), enable)) != L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_BROADCAST:
            if ((retVal = l2sw_setAsicStormFilterBroadcastEnable(units, l2sw_switch_port_L2P_get(units, port), enable)) != L2SW_ERR_OK)
                return retVal;
            break;
        default:
            break;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_stormControlPortEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_storm_group_t stormType, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (stormType >= STORM_GROUP_END)
        return L2SW_ERR_SFC_UNKNOWN_GROUP;

    if (NULL == pEnable)
        return L2SW_ERR_ENABLE;

    switch (stormType)
    {
        case STORM_GROUP_UNKNOWN_UNICAST:
            if ((retVal = l2sw_getAsicStormFilterUnknownUnicastEnable(units, l2sw_switch_port_L2P_get(units, port), pEnable)) != L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_UNKNOWN_MULTICAST:
            if ((retVal = l2sw_getAsicStormFilterUnknownMulticastEnable(units, l2sw_switch_port_L2P_get(units, port), pEnable)) != L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_MULTICAST:
            if ((retVal = l2sw_getAsicStormFilterMulticastEnable(units, l2sw_switch_port_L2P_get(units, port), pEnable)) != L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_BROADCAST:
            if ((retVal = l2sw_getAsicStormFilterBroadcastEnable(units, l2sw_switch_port_L2P_get(units, port), pEnable)) != L2SW_ERR_OK)
                return retVal;
            break;
        default:
            break;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_storm_bypass_set(l2sw_uint8 units, l2sw_storm_bypass_t type, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;
    l2sw_rma_t rmacfg;
    l2sw_uint32 tmp;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= BYPASS_END)
        return L2SW_ERR_INPUT;

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if (type >= 0 && type <= BYPASS_UNDEF_GARP_2F)
    {
        if ((retVal = l2sw_getAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.discard_storm_filter = enable;

        if ((retVal = l2sw_setAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if(type == BYPASS_IGMP)
    {
        if ((retVal = l2sw_setAsicIGMPBypassStormCTRL(units, enable)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (type == BYPASS_CDP)
    {
        if ((retVal = l2sw_getAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.discard_storm_filter = enable;

        if ((retVal = l2sw_setAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (type  == BYPASS_CSSTP)
    {
        if ((retVal = l2sw_getAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.discard_storm_filter = enable;

        if ((retVal = l2sw_setAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (type  == BYPASS_LLDP)
    {
        if ((retVal = l2sw_getAsicRmaLldp(units, &tmp, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.discard_storm_filter = enable;

        if ((retVal = l2sw_setAsicRmaLldp(units, tmp, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else
        return L2SW_ERR_INPUT;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_storm_bypass_get(l2sw_uint8 units, l2sw_storm_bypass_t type, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    l2sw_rma_t rmacfg;
    l2sw_uint32 tmp;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= BYPASS_END)
        return L2SW_ERR_INPUT;

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if (type >= 0 && type <= BYPASS_UNDEF_GARP_2F)
    {
        if ((retVal = l2sw_getAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = rmacfg.discard_storm_filter;
    }
    else if(type == BYPASS_IGMP)
    {
        if ((retVal = l2sw_getAsicIGMPBypassStormCTRL(units, pEnable)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (type == BYPASS_CDP)
    {
        if ((retVal = l2sw_getAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = rmacfg.discard_storm_filter;
    }
    else if (type == BYPASS_CSSTP)
    {
        if ((retVal = l2sw_getAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = rmacfg.discard_storm_filter;
    }
    else if (type == BYPASS_LLDP)
    {
        if ((retVal = l2sw_getAsicRmaLldp(units, &tmp,&rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = rmacfg.discard_storm_filter;
    }
    else
        return L2SW_ERR_INPUT;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_stormControlExtPortmask_set(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pPortmask)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_switch_portmask_L2P_get(units, pPortmask, &pmask)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicStormFilterExtEnablePortMask(units, pmask)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_stormControlExtPortmask_get(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pPortmask)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicStormFilterExtEnablePortMask(units, &pmask)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_switch_portmask_P2L_get(units, pmask, pPortmask)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_stormControlExtEnable_set(l2sw_uint8 units, l2sw_rate_storm_group_t stormType, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (stormType >= STORM_GROUP_END)
        return L2SW_ERR_SFC_UNKNOWN_GROUP;

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    switch (stormType)
    {
        case STORM_GROUP_UNKNOWN_UNICAST:
            if ((retVal = l2sw_setAsicStormFilterExtUnknownUnicastEnable(units, enable)) != L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_UNKNOWN_MULTICAST:
            if ((retVal = l2sw_setAsicStormFilterExtUnknownMulticastEnable(units, enable)) != L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_MULTICAST:
            if ((retVal = l2sw_setAsicStormFilterExtMulticastEnable(units, enable)) != L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_BROADCAST:
            if ((retVal = l2sw_setAsicStormFilterExtBroadcastEnable(units, enable)) != L2SW_ERR_OK)
                return retVal;
            break;
        default:
            break;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_stormControlExtEnable_get(l2sw_uint8 units, l2sw_rate_storm_group_t stormType, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (stormType >= STORM_GROUP_END)
        return L2SW_ERR_SFC_UNKNOWN_GROUP;

    if (NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    switch (stormType)
    {
        case STORM_GROUP_UNKNOWN_UNICAST:
            if ((retVal = l2sw_getAsicStormFilterExtUnknownUnicastEnable(units, (l2sw_uint32 *)pEnable)) != L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_UNKNOWN_MULTICAST:
            if ((retVal = l2sw_getAsicStormFilterExtUnknownMulticastEnable(units, (l2sw_uint32 *)pEnable)) != L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_MULTICAST:
            if ((retVal = l2sw_getAsicStormFilterExtMulticastEnable(units, (l2sw_uint32 *)pEnable)) != L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_BROADCAST:
            if ((retVal = l2sw_getAsicStormFilterExtBroadcastEnable(units, (l2sw_uint32 *)pEnable)) != L2SW_ERR_OK)
                return retVal;
            break;
        default:
            break;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_stormControlExtMeterIdx_set(l2sw_uint8 units, l2sw_rate_storm_group_t stormType, l2sw_uint32 index)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (stormType >= STORM_GROUP_END)
        return L2SW_ERR_SFC_UNKNOWN_GROUP;

    if (index > L2SW_MAX_METER_ID)
        return L2SW_ERR_FILTER_METER_ID;

    switch (stormType)
    {
        case STORM_GROUP_UNKNOWN_UNICAST:
            if ((retVal = l2sw_setAsicStormFilterExtUnknownUnicastMeter(units, index))!=L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_UNKNOWN_MULTICAST:
            if ((retVal = l2sw_setAsicStormFilterExtUnknownMulticastMeter(units, index))!=L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_MULTICAST:
            if ((retVal = l2sw_setAsicStormFilterExtMulticastMeter(units, index))!=L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_BROADCAST:
            if ((retVal = l2sw_setAsicStormFilterExtBroadcastMeter(units, index))!=L2SW_ERR_OK)
                return retVal;
            break;
        default:
            break;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_stormControlExtMeterIdx_get(l2sw_uint8 units, l2sw_rate_storm_group_t stormType, l2sw_uint32 *pIndex)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (stormType >= STORM_GROUP_END)
        return L2SW_ERR_SFC_UNKNOWN_GROUP;

    if(NULL == pIndex)
        return L2SW_ERR_NULL_POINTER;

    switch (stormType)
    {
        case STORM_GROUP_UNKNOWN_UNICAST:
            if ((retVal = l2sw_getAsicStormFilterExtUnknownUnicastMeter(units, pIndex))!=L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_UNKNOWN_MULTICAST:
            if ((retVal = l2sw_getAsicStormFilterExtUnknownMulticastMeter(units, pIndex))!=L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_MULTICAST:
            if ((retVal = l2sw_getAsicStormFilterExtMulticastMeter(units, pIndex))!=L2SW_ERR_OK)
                return retVal;
            break;
        case STORM_GROUP_BROADCAST:
            if ((retVal = l2sw_getAsicStormFilterExtBroadcastMeter(units, pIndex))!=L2SW_ERR_OK)
                return retVal;
            break;
        default:
            break;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_stormControlEnable_set(l2sw_uint8 units, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;
    
    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;
    
    if ((retVal = l2sw_setAsicStormFilterEnable(units, enable))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_stormControlEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    
    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);
    
    if ((retVal = l2sw_getAsicStormFilterEnable(units, pEnable))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_rate_stormControlMeterIdx_set
 * Description:
 *      Set the storm control meter index.
 * Input:
 *      port       - port id
 *      storm_type - storm group type
 *      index       - storm control meter index.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_PORT_ID - Invalid port id
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter
 * Note:
 *
 */
l2sw_api_ret_t l2sw_rate_stormControlMeterIdx_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_storm_group_t stormType, l2sw_uint32 index)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_stormControlMeterIdx_set(units, port, stormType, index);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rate_stormControlMeterIdx_get
 * Description:
 *      Get the storm control meter index.
 * Input:
 *      port       - port id
 *      storm_type - storm group type
 * Output:
 *      pIndex     - storm control meter index.
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_PORT_ID - Invalid port id
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter
 * Note:
 *
 */
l2sw_api_ret_t l2sw_rate_stormControlMeterIdx_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_storm_group_t stormType, l2sw_uint32 *pIndex)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_stormControlMeterIdx_get(units, port, stormType, pIndex);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rate_stormControlPortEnable_set
 * Description:
 *      Set enable status of storm control on specified port.
 * Input:
 *      port       - port id
 *      stormType  - storm group type
 *      enable     - enable status of storm control
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_PORT_ID           - invalid port id
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
l2sw_api_ret_t l2sw_rate_stormControlPortEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_storm_group_t stormType, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_stormControlPortEnable_set(units, port, stormType, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rate_stormControlPortEnable_set
 * Description:
 *      Set enable status of storm control on specified port.
 * Input:
 *      port       - port id
 *      stormType  - storm group type
 * Output:
 *      pEnable     - enable status of storm control
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_PORT_ID           - invalid port id
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
l2sw_api_ret_t l2sw_rate_stormControlPortEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_storm_group_t stormType, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_stormControlPortEnable_get(units, port, stormType, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_storm_bypass_set
 * Description:
 *      Set bypass storm filter control configuration.
 * Input:
 *      type    - Bypass storm filter control type.
 *      enable  - Bypass status.
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
 *      This API can set per-port bypass stomr filter control frame type including RMA and igmp.
 *      The bypass frame type is as following:
 *      - BYPASS_BRG_GROUP,
 *      - BYPASS_FD_PAUSE,
 *      - BYPASS_SP_MCAST,
 *      - BYPASS_1X_PAE,
 *      - BYPASS_UNDEF_BRG_04,
 *      - BYPASS_UNDEF_BRG_05,
 *      - BYPASS_UNDEF_BRG_06,
 *      - BYPASS_UNDEF_BRG_07,
 *      - BYPASS_PROVIDER_BRIDGE_GROUP_ADDRESS,
 *      - BYPASS_UNDEF_BRG_09,
 *      - BYPASS_UNDEF_BRG_0A,
 *      - BYPASS_UNDEF_BRG_0B,
 *      - BYPASS_UNDEF_BRG_0C,
 *      - BYPASS_PROVIDER_BRIDGE_GVRP_ADDRESS,
 *      - BYPASS_8021AB,
 *      - BYPASS_UNDEF_BRG_0F,
 *      - BYPASS_BRG_MNGEMENT,
 *      - BYPASS_UNDEFINED_11,
 *      - BYPASS_UNDEFINED_12,
 *      - BYPASS_UNDEFINED_13,
 *      - BYPASS_UNDEFINED_14,
 *      - BYPASS_UNDEFINED_15,
 *      - BYPASS_UNDEFINED_16,
 *      - BYPASS_UNDEFINED_17,
 *      - BYPASS_UNDEFINED_18,
 *      - BYPASS_UNDEFINED_19,
 *      - BYPASS_UNDEFINED_1A,
 *      - BYPASS_UNDEFINED_1B,
 *      - BYPASS_UNDEFINED_1C,
 *      - BYPASS_UNDEFINED_1D,
 *      - BYPASS_UNDEFINED_1E,
 *      - BYPASS_UNDEFINED_1F,
 *      - BYPASS_GMRP,
 *      - BYPASS_GVRP,
 *      - BYPASS_UNDEF_GARP_22,
 *      - BYPASS_UNDEF_GARP_23,
 *      - BYPASS_UNDEF_GARP_24,
 *      - BYPASS_UNDEF_GARP_25,
 *      - BYPASS_UNDEF_GARP_26,
 *      - BYPASS_UNDEF_GARP_27,
 *      - BYPASS_UNDEF_GARP_28,
 *      - BYPASS_UNDEF_GARP_29,
 *      - BYPASS_UNDEF_GARP_2A,
 *      - BYPASS_UNDEF_GARP_2B,
 *      - BYPASS_UNDEF_GARP_2C,
 *      - BYPASS_UNDEF_GARP_2D,
 *      - BYPASS_UNDEF_GARP_2E,
 *      - BYPASS_UNDEF_GARP_2F,
 *      - BYPASS_IGMP.
 *      - BYPASS_CDP.
 *      - BYPASS_CSSTP.
 *      - BYPASS_LLDP.
 */
l2sw_api_ret_t l2sw_storm_bypass_set(l2sw_uint8 units, l2sw_storm_bypass_t type, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_storm_bypass_set(units, type, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_storm_bypass_get
 * Description:
 *      Get bypass storm filter control configuration.
 * Input:
 *      type - Bypass storm filter control type.
 * Output:
 *      pEnable - Bypass status.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can get per-port bypass stomr filter control frame type including RMA and igmp.
 *      The bypass frame type is as following:
 *      - BYPASS_BRG_GROUP,
 *      - BYPASS_FD_PAUSE,
 *      - BYPASS_SP_MCAST,
 *      - BYPASS_1X_PAE,
 *      - BYPASS_UNDEF_BRG_04,
 *      - BYPASS_UNDEF_BRG_05,
 *      - BYPASS_UNDEF_BRG_06,
 *      - BYPASS_UNDEF_BRG_07,
 *      - BYPASS_PROVIDER_BRIDGE_GROUP_ADDRESS,
 *      - BYPASS_UNDEF_BRG_09,
 *      - BYPASS_UNDEF_BRG_0A,
 *      - BYPASS_UNDEF_BRG_0B,
 *      - BYPASS_UNDEF_BRG_0C,
 *      - BYPASS_PROVIDER_BRIDGE_GVRP_ADDRESS,
 *      - BYPASS_8021AB,
 *      - BYPASS_UNDEF_BRG_0F,
 *      - BYPASS_BRG_MNGEMENT,
 *      - BYPASS_UNDEFINED_11,
 *      - BYPASS_UNDEFINED_12,
 *      - BYPASS_UNDEFINED_13,
 *      - BYPASS_UNDEFINED_14,
 *      - BYPASS_UNDEFINED_15,
 *      - BYPASS_UNDEFINED_16,
 *      - BYPASS_UNDEFINED_17,
 *      - BYPASS_UNDEFINED_18,
 *      - BYPASS_UNDEFINED_19,
 *      - BYPASS_UNDEFINED_1A,
 *      - BYPASS_UNDEFINED_1B,
 *      - BYPASS_UNDEFINED_1C,
 *      - BYPASS_UNDEFINED_1D,
 *      - BYPASS_UNDEFINED_1E,
 *      - BYPASS_UNDEFINED_1F,
 *      - BYPASS_GMRP,
 *      - BYPASS_GVRP,
 *      - BYPASS_UNDEF_GARP_22,
 *      - BYPASS_UNDEF_GARP_23,
 *      - BYPASS_UNDEF_GARP_24,
 *      - BYPASS_UNDEF_GARP_25,
 *      - BYPASS_UNDEF_GARP_26,
 *      - BYPASS_UNDEF_GARP_27,
 *      - BYPASS_UNDEF_GARP_28,
 *      - BYPASS_UNDEF_GARP_29,
 *      - BYPASS_UNDEF_GARP_2A,
 *      - BYPASS_UNDEF_GARP_2B,
 *      - BYPASS_UNDEF_GARP_2C,
 *      - BYPASS_UNDEF_GARP_2D,
 *      - BYPASS_UNDEF_GARP_2E,
 *      - BYPASS_UNDEF_GARP_2F,
 *      - BYPASS_IGMP.
 *      - BYPASS_CDP.
 *      - BYPASS_CSSTP.
 *      - BYPASS_LLDP.
 */
l2sw_api_ret_t l2sw_storm_bypass_get(l2sw_uint8 units, l2sw_storm_bypass_t type, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_storm_bypass_get(units, type, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rate_stormControlExtPortmask_set
 * Description:
 *      Set externsion storm control port mask
 * Input:
 *      pPortmask  - port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
l2sw_api_ret_t l2sw_rate_stormControlExtPortmask_set(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_stormControlExtPortmask_set(units, pPortmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rate_stormControlExtPortmask_get
 * Description:
 *      Set externsion storm control port mask
 * Input:
 *      None
 * Output:
 *      pPortmask  - port mask
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
l2sw_api_ret_t l2sw_rate_stormControlExtPortmask_get(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_stormControlExtPortmask_get(units, pPortmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rate_stormControlExtEnable_set
 * Description:
 *      Set externsion storm control state
 * Input:
 *      stormType   - storm group type
 *      enable      - externsion storm control state
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
l2sw_api_ret_t l2sw_rate_stormControlExtEnable_set(l2sw_uint8 units, l2sw_rate_storm_group_t stormType, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_stormControlExtEnable_set(units, stormType, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rate_stormControlExtEnable_get
 * Description:
 *      Get externsion storm control state
 * Input:
 *      stormType   - storm group type
 * Output:
 *      pEnable     - externsion storm control state
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
l2sw_api_ret_t l2sw_rate_stormControlExtEnable_get(l2sw_uint8 units, l2sw_rate_storm_group_t stormType, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_stormControlExtEnable_get(units, stormType, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rate_stormControlExtMeterIdx_set
 * Description:
 *      Set externsion storm control meter index
 * Input:
 *      stormType   - storm group type
 *      index       - externsion storm control state
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
l2sw_api_ret_t l2sw_rate_stormControlExtMeterIdx_set(l2sw_uint8 units, l2sw_rate_storm_group_t stormType, l2sw_uint32 index)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_stormControlExtMeterIdx_set(units, stormType, index);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rate_stormControlExtMeterIdx_get
 * Description:
 *      Get externsion storm control meter index
 * Input:
 *      stormType   - storm group type
 *      pIndex      - externsion storm control state
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
l2sw_api_ret_t l2sw_rate_stormControlExtMeterIdx_get(l2sw_uint8 units, l2sw_rate_storm_group_t stormType, l2sw_uint32 *pIndex)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_stormControlExtMeterIdx_get(units, stormType, pIndex);
    L2SW_API_UNLOCK(units);

    return retVal;
}


/* Function Name:
 *      _l2sw_rate_stormControlEnable_set
 * Description:
 *      Set enable status of storm control function
 * Input:
 *      enable     - enable status of storm control
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
l2sw_api_ret_t l2sw_rate_stormControlEnable_set(l2sw_uint8 units, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_stormControlEnable_set(units, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}


/* Function Name:
 *      l2sw_rate_stormControlEnable_get
 * Description:
 *      Get enable status of storm control function
 * Input:
 *      None
 * Output:
 *      pEnable     - enable status of storm control
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
l2sw_api_ret_t l2sw_rate_stormControlEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = l2sw_getAsicStormFilterEnable(units, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}


