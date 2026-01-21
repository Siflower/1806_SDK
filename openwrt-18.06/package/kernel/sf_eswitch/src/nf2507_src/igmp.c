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
#include "igmp.h"
#include "sal_string.h"
#include "l2sw_asicdrv.h"
#include "l2sw_asicdrv_igmp.h"
#include "l2sw_asicdrv_lut.h"

static l2sw_api_ret_t _l2sw_igmp_init(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;
    l2sw_port_t port;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((retVal = l2sw_setAsicLutIpMulticastLookup(units, L2SW_ENABLED))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicLutIpLookupMethod(units, 1))!=L2SW_ERR_OK)
        return retVal;

    L2SW_SCAN_ALL_PHY_PORTMASK(units, port)
    {
        if ((retVal = l2sw_setAsicIGMPv1Opeartion(units, port, PROTOCOL_OP_ASIC))!=L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_setAsicIGMPv2Opeartion(units, port, PROTOCOL_OP_ASIC))!=L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_setAsicIGMPv3Opeartion(units, port, PROTOCOL_OP_FLOOD))!=L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_setAsicMLDv1Opeartion(units, port, PROTOCOL_OP_ASIC))!=L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_setAsicMLDv2Opeartion(units, port, PROTOCOL_OP_FLOOD))!=L2SW_ERR_OK)
            return retVal;
    }

    if ((retVal = l2sw_setAsicIGMPAllowDynamicRouterPort(units, l2sw_switch_phyPortMask_get(units)))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicIGMPFastLeaveEn(units, L2SW_ENABLED))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicIGMPReportLeaveFlood(units, 1))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicIgmp(units, L2SW_ENABLED))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_state_set(l2sw_uint8 units, l2sw_enable_t enabled)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (enabled >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicIgmp(units, enabled))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_state_get(l2sw_uint8 units, l2sw_enable_t *pEnabled)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(pEnabled == NULL)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIgmp(units, pEnabled))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_static_router_port_set(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Valid port mask */
    if(pPortmask == NULL)
        return L2SW_ERR_NULL_POINTER;

    L2SW_CHK_PORTMASK_VALID(units, pPortmask);

    if ((retVal = l2sw_switch_portmask_L2P_get(units, pPortmask, &pmask))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicIGMPStaticRouterPort(units, pmask))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_static_router_port_get(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(pPortmask == NULL)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIGMPStaticRouterPort(units, &pmask))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_switch_portmask_P2L_get(units, pmask, pPortmask))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_protocol_set(l2sw_uint8 units, l2sw_port_t port, l2sw_igmp_protocol_t protocol, l2sw_igmp_action_t action)
{
    l2sw_uint32      operation;
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(protocol >= PROTOCOL_END)
        return L2SW_ERR_INPUT;

    if(action >= IGMP_ACTION_END)
        return L2SW_ERR_INPUT;

    switch(action)
    {
        case IGMP_ACTION_FORWARD:
            operation = PROTOCOL_OP_FLOOD;
            break;
        case IGMP_ACTION_TRAP2CPU:
            operation = PROTOCOL_OP_TRAP;
            break;
        case IGMP_ACTION_DROP:
            operation = PROTOCOL_OP_DROP;
            break;
        case IGMP_ACTION_ASIC:
            operation = PROTOCOL_OP_ASIC;
            break;
        default:
            return L2SW_ERR_INPUT;
    }

    switch(protocol)
    {
        case PROTOCOL_IGMPv1:
            if ((retVal = l2sw_setAsicIGMPv1Opeartion(units, l2sw_switch_port_L2P_get(units, port), operation))!=L2SW_ERR_OK)
                return retVal;

            break;
        case PROTOCOL_IGMPv2:
            if ((retVal = l2sw_setAsicIGMPv2Opeartion(units, l2sw_switch_port_L2P_get(units, port), operation))!=L2SW_ERR_OK)
                return retVal;

            break;
        case PROTOCOL_IGMPv3:
            if ((retVal = l2sw_setAsicIGMPv3Opeartion(units, l2sw_switch_port_L2P_get(units, port), operation))!=L2SW_ERR_OK)
                return retVal;

            break;
        case PROTOCOL_MLDv1:
            if ((retVal = l2sw_setAsicMLDv1Opeartion(units, l2sw_switch_port_L2P_get(units, port), operation))!=L2SW_ERR_OK)
                return retVal;

            break;
        case PROTOCOL_MLDv2:
            if ((retVal = l2sw_setAsicMLDv2Opeartion(units, l2sw_switch_port_L2P_get(units, port), operation))!=L2SW_ERR_OK)
                return retVal;

            break;
        default:
            return L2SW_ERR_INPUT;

    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_protocol_get(l2sw_uint8 units, l2sw_port_t port, l2sw_igmp_protocol_t protocol, l2sw_igmp_action_t *pAction)
{
    l2sw_uint32      operation;
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(protocol >= PROTOCOL_END)
        return L2SW_ERR_INPUT;

    if(pAction == NULL)
        return L2SW_ERR_NULL_POINTER;

    switch(protocol)
    {
        case PROTOCOL_IGMPv1:
            if ((retVal = l2sw_getAsicIGMPv1Opeartion(units, l2sw_switch_port_L2P_get(units, port), &operation))!=L2SW_ERR_OK)
                return retVal;

            break;
        case PROTOCOL_IGMPv2:
            if ((retVal = l2sw_getAsicIGMPv2Opeartion(units, l2sw_switch_port_L2P_get(units, port), &operation))!=L2SW_ERR_OK)
                return retVal;

            break;
        case PROTOCOL_IGMPv3:
            if ((retVal = l2sw_getAsicIGMPv3Opeartion(units, l2sw_switch_port_L2P_get(units, port), &operation))!=L2SW_ERR_OK)
                return retVal;

            break;
        case PROTOCOL_MLDv1:
            if ((retVal = l2sw_getAsicMLDv1Opeartion(units, l2sw_switch_port_L2P_get(units, port), &operation))!=L2SW_ERR_OK)
                return retVal;

            break;
        case PROTOCOL_MLDv2:
            if ((retVal = l2sw_getAsicMLDv2Opeartion(units, l2sw_switch_port_L2P_get(units, port), &operation))!=L2SW_ERR_OK)
                return retVal;

            break;
        default:
            return L2SW_ERR_INPUT;

    }

    switch(operation)
    {
        case PROTOCOL_OP_FLOOD:
            *pAction = IGMP_ACTION_FORWARD;
            break;
        case PROTOCOL_OP_TRAP:
            *pAction = IGMP_ACTION_TRAP2CPU;
            break;
        case PROTOCOL_OP_DROP:
            *pAction = IGMP_ACTION_DROP;
            break;
        case PROTOCOL_OP_ASIC:
            *pAction = IGMP_ACTION_ASIC;
            break;
        default:
            return L2SW_ERR_FAILED;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_fastLeave_set(l2sw_uint8 units, l2sw_enable_t state)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(state >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicIGMPFastLeaveEn(units, (l2sw_uint32)state))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_fastLeave_get(l2sw_uint8 units, l2sw_enable_t *pState)
{
    l2sw_uint32      fast_leave;
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(pState == NULL)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIGMPFastLeaveEn(units, &fast_leave))!=L2SW_ERR_OK)
        return retVal;

    *pState = ((fast_leave == 1) ? L2SW_ENABLED : L2SW_DISABLED);
    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_maxGroup_set(l2sw_uint8 units, l2sw_port_t port, l2sw_uint32 group)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(group > L2SW_IGMP_MAX_GOUP)
        return L2SW_ERR_OUT_OF_RANGE;

    if ((retVal = l2sw_setAsicIGMPPortMAXGroup(units, l2sw_switch_port_L2P_get(units, port), group))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_maxGroup_get(l2sw_uint8 units, l2sw_port_t port, l2sw_uint32 *pGroup)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(pGroup == NULL)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIGMPPortMAXGroup(units, l2sw_switch_port_L2P_get(units, port), pGroup))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_currentGroup_get(l2sw_uint8 units, l2sw_port_t port, l2sw_uint32 *pGroup)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(pGroup == NULL)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIGMPPortCurrentGroup(units, l2sw_switch_port_L2P_get(units, port), pGroup))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_tableFullAction_set(l2sw_uint8 units, l2sw_igmp_tableFullAction_t action)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(action >= IGMP_TABLE_FULL_OP_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicIGMPTableFullOP(units, (l2sw_uint32)action))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_tableFullAction_get(l2sw_uint8 units, l2sw_igmp_tableFullAction_t *pAction)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pAction)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIGMPTableFullOP(units, (l2sw_uint32 *)pAction))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_checksumErrorAction_set(l2sw_uint8 units, l2sw_igmp_checksumErrorAction_t action)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(action >= IGMP_CRC_ERR_OP_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicIGMPCRCErrOP(units, (l2sw_uint32)action))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_checksumErrorAction_get(l2sw_uint8 units, l2sw_igmp_checksumErrorAction_t *pAction)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pAction)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIGMPCRCErrOP(units, (l2sw_uint32 *)pAction))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}


static l2sw_api_ret_t _l2sw_igmp_queryInterval_set(l2sw_uint8 units, l2sw_uint32 interval)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(interval > L2SW_MAX_QUERY_INT)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicIGMPQueryInterval(units, interval))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_queryInterval_get(l2sw_uint8 units, l2sw_uint32 *pInterval)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pInterval)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIGMPQueryInterval(units, pInterval))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_dynamicRouterPort_queryInterval_set(l2sw_uint8 units, l2sw_uint32 interval)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(interval > L2SW_MAX_QUERY_INT)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicIGMPDynamicRouterPortQueryInterval(units, interval))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_dynamicRouterPort_queryInterval_get(l2sw_uint8 units, l2sw_uint32 *pInterval)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pInterval)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIGMPDynamicRouterPortQueryInterval(units, pInterval))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_robustness_set(l2sw_uint8 units, l2sw_uint32 robustness)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(robustness > L2SW_MAX_ROB_VAR)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicIGMPRobVar(units, robustness))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_robustness_get(l2sw_uint8 units, l2sw_uint32 *pRobustness)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pRobustness)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIGMPRobVar(units, pRobustness))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_dynamicRouterPortAllow_set(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32 pmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pPortmask)
        return L2SW_ERR_NULL_POINTER;

    L2SW_CHK_PORTMASK_VALID(units, pPortmask);

    if ((retVal = l2sw_switch_portmask_L2P_get(units, pPortmask, &pmask))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicIGMPAllowDynamicRouterPort(units, pmask))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_dynamicRouterPortAllow_get(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32 pmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pPortmask)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIGMPAllowDynamicRouterPort(units, &pmask))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_switch_portmask_P2L_get(units, pmask, pPortmask))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_dynamicRouterPort_get(l2sw_uint8 units, l2sw_igmp_dynamicRouterPort_t *pDynamicRouterPort)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32 port;
    l2sw_uint32 timer;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pDynamicRouterPort)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIGMPdynamicRouterPort1(units, &port, &timer))!= L2SW_ERR_OK)
        return retVal;

    if (port == L2SW_ROUTER_PORT_INVALID)
    {
        pDynamicRouterPort->dynamicRouterPort0Valid = L2SW_DISABLED;
        pDynamicRouterPort->dynamicRouterPort0      = 0;
        pDynamicRouterPort->dynamicRouterPort0Timer = 0;
    }
    else
    {
        pDynamicRouterPort->dynamicRouterPort0Valid = L2SW_ENABLED;
        pDynamicRouterPort->dynamicRouterPort0      = l2sw_switch_port_P2L_get(units, port);
        pDynamicRouterPort->dynamicRouterPort0Timer = timer;
    }

    if ((retVal = l2sw_getAsicIGMPdynamicRouterPort2(units, &port, &timer))!= L2SW_ERR_OK)
        return retVal;

    if (port == L2SW_ROUTER_PORT_INVALID)
    {
        pDynamicRouterPort->dynamicRouterPort1Valid = L2SW_DISABLED;
        pDynamicRouterPort->dynamicRouterPort1      = 0;
        pDynamicRouterPort->dynamicRouterPort1Timer = 0;
    }
    else
    {
        pDynamicRouterPort->dynamicRouterPort1Valid = L2SW_ENABLED;
        pDynamicRouterPort->dynamicRouterPort1      = l2sw_switch_port_P2L_get(units, port);
        pDynamicRouterPort->dynamicRouterPort1Timer = timer;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_suppressionEnable_set(l2sw_uint8 units, l2sw_enable_t reportSuppression, l2sw_enable_t leaveSuppression)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(reportSuppression >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if(leaveSuppression >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicIGMPSuppression(units, (l2sw_uint32)reportSuppression, (l2sw_uint32)leaveSuppression))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_suppressionEnable_get(l2sw_uint8 units, l2sw_enable_t *pReportSuppression, l2sw_enable_t *pLeaveSuppression)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pReportSuppression)
        return L2SW_ERR_NULL_POINTER;

    if(NULL == pLeaveSuppression)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIGMPSuppression(units, (l2sw_uint32 *)pReportSuppression, (l2sw_uint32 *)pLeaveSuppression))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_portRxPktEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_igmp_rxPktEnable_t *pRxCfg)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pRxCfg)
        return L2SW_ERR_NULL_POINTER;

    if(pRxCfg->rxQuery >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if(pRxCfg->rxReport >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if(pRxCfg->rxLeave >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if(pRxCfg->rxMRP >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if(pRxCfg->rxMcast >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicIGMPQueryRX(units, l2sw_switch_port_L2P_get(units, port), (l2sw_uint32)pRxCfg->rxQuery))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicIGMPReportRX(units, l2sw_switch_port_L2P_get(units, port), (l2sw_uint32)pRxCfg->rxReport))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicIGMPLeaveRX(units, l2sw_switch_port_L2P_get(units, port), (l2sw_uint32)pRxCfg->rxLeave))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicIGMPMRPRX(units, l2sw_switch_port_L2P_get(units, port), (l2sw_uint32)pRxCfg->rxMRP))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicIGMPMcDataRX(units, l2sw_switch_port_L2P_get(units, port), (l2sw_uint32)pRxCfg->rxMcast))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_portRxPktEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_igmp_rxPktEnable_t *pRxCfg)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pRxCfg)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIGMPQueryRX(units, l2sw_switch_port_L2P_get(units, port), (l2sw_uint32 *)&(pRxCfg->rxQuery)))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicIGMPReportRX(units, l2sw_switch_port_L2P_get(units, port), (l2sw_uint32 *)&(pRxCfg->rxReport)))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicIGMPLeaveRX(units, l2sw_switch_port_L2P_get(units, port), (l2sw_uint32 *)&(pRxCfg->rxLeave)))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicIGMPMRPRX(units, l2sw_switch_port_L2P_get(units, port), (l2sw_uint32 *)&(pRxCfg->rxMRP)))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicIGMPMcDataRX(units, l2sw_switch_port_L2P_get(units, port), (l2sw_uint32 *)&(pRxCfg->rxMcast)))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_groupInfo_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_igmp_groupInfo_t *pGroup)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32      valid;
    l2sw_igmpgroup  grp;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check index */
    if(index > L2SW_IGMP_MAX_GOUP)
        return L2SW_ERR_INPUT;

    if(NULL == pGroup)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIGMPGroup(units, index, &valid, &grp))!=L2SW_ERR_OK)
        return retVal;

    MEMSET(pGroup, 0x00, sizeof(l2sw_igmp_groupInfo_t));
    pGroup->valid = valid;
    pGroup->reportSuppFlag = grp.report_supp_flag;

    if(grp.p0_timer != 0)
    {
        L2SW_PORTMASK_PORT_SET((pGroup->member), l2sw_switch_port_P2L_get(units, 0));
        pGroup->timer[l2sw_switch_port_P2L_get(units, 0)] = grp.p0_timer;
    }

    if(grp.p1_timer != 0)
    {
        L2SW_PORTMASK_PORT_SET((pGroup->member), l2sw_switch_port_P2L_get(units, 1));
        pGroup->timer[l2sw_switch_port_P2L_get(units, 1)] = grp.p1_timer;
    }

    if(grp.p2_timer != 0)
    {
        L2SW_PORTMASK_PORT_SET((pGroup->member), l2sw_switch_port_P2L_get(units, 2));
        pGroup->timer[l2sw_switch_port_P2L_get(units, 2)] = grp.p2_timer;
    }

    if(grp.p3_timer != 0)
    {
        L2SW_PORTMASK_PORT_SET((pGroup->member), l2sw_switch_port_P2L_get(units, 3));
        pGroup->timer[l2sw_switch_port_P2L_get(units, 3)] = grp.p3_timer;
    }

    if(grp.p4_timer != 0)
    {
        L2SW_PORTMASK_PORT_SET((pGroup->member), l2sw_switch_port_P2L_get(units, 4));
        pGroup->timer[l2sw_switch_port_P2L_get(units, 4)] = grp.p4_timer;
    }

    if(grp.p5_timer != 0)
    {
        L2SW_PORTMASK_PORT_SET((pGroup->member), l2sw_switch_port_P2L_get(units, 5));
        pGroup->timer[l2sw_switch_port_P2L_get(units, 5)] = grp.p5_timer;
    }

    if(grp.p6_timer != 0)
    {
        L2SW_PORTMASK_PORT_SET((pGroup->member), l2sw_switch_port_P2L_get(units, 6));
        pGroup->timer[l2sw_switch_port_P2L_get(units, 6)] = grp.p6_timer;
    }

    if(grp.p7_timer != 0)
    {
        L2SW_PORTMASK_PORT_SET((pGroup->member), l2sw_switch_port_P2L_get(units, 7));
        pGroup->timer[l2sw_switch_port_P2L_get(units, 7)] = grp.p7_timer;
    }

    if(grp.p8_timer != 0)
    {
        L2SW_PORTMASK_PORT_SET((pGroup->member), l2sw_switch_port_P2L_get(units, 8));
        pGroup->timer[l2sw_switch_port_P2L_get(units, 8)] = grp.p8_timer;
    }

    if(grp.p9_timer != 0)
    {
        L2SW_PORTMASK_PORT_SET((pGroup->member), l2sw_switch_port_P2L_get(units, 9));
        pGroup->timer[l2sw_switch_port_P2L_get(units, 9)] = grp.p9_timer;
    }

    if(grp.p10_timer != 0)
    {
        L2SW_PORTMASK_PORT_SET((pGroup->member), l2sw_switch_port_P2L_get(units, 10));
        pGroup->timer[l2sw_switch_port_P2L_get(units, 10)] = grp.p10_timer;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_ReportLeaveFwdAction_set(l2sw_uint8 units, l2sw_igmp_ReportLeaveFwdAct_t action)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32      regData;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    switch(action)
    {
        case IGMP_REPORT_LEAVE_TO_ROUTER:
            regData = 1;
            break;
        case IGMP_REPORT_LEAVE_TO_ALLPORT:
            regData = 2;
            break;
        case IGMP_REPORT_LEAVE_TO_ROUTER_PORT_ADV:
            regData = 3;
            break;
        default:
            return L2SW_ERR_INPUT;
    }

    if ((retVal = l2sw_setAsicIGMPReportLeaveFlood(units, regData))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_ReportLeaveFwdAction_get(l2sw_uint8 units, l2sw_igmp_ReportLeaveFwdAct_t *pAction)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32      regData;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pAction)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIGMPReportLeaveFlood(units, &regData))!=L2SW_ERR_OK)
        return retVal;

    switch(regData)
    {
        case 1:
            *pAction = IGMP_REPORT_LEAVE_TO_ROUTER;
            break;
        case 2:
            *pAction = IGMP_REPORT_LEAVE_TO_ALLPORT;
            break;
        case 3:
            *pAction = IGMP_REPORT_LEAVE_TO_ROUTER_PORT_ADV;
            break;
        default:
            return L2SW_ERR_FAILED;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_dropLeaveZeroEnable_set(l2sw_uint8 units, l2sw_enable_t enabled)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(enabled >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicIGMPDropLeaveZero(units, enabled))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;

}

static l2sw_api_ret_t _l2sw_igmp_dropLeaveZeroEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnabled)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pEnabled)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIGMPDropLeaveZero(units, (l2sw_uint32 *)pEnabled))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;

}

static l2sw_api_ret_t _l2sw_igmp_bypassGroupRange_set(l2sw_uint8 units, l2sw_igmp_bypassGroup_t group, l2sw_enable_t enabled)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(group >= IGMP_BYPASS_GROUP_END)
        return L2SW_ERR_INPUT;

    if(enabled >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicIGMPBypassGroup(units, group, enabled))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_igmp_bypassGroupRange_get(l2sw_uint8 units, l2sw_igmp_bypassGroup_t group, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(group >= IGMP_BYPASS_GROUP_END)
        return L2SW_ERR_INPUT;

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicIGMPBypassGroup(units, group, pEnable))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}


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
l2sw_api_ret_t l2sw_igmp_init(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_init(units);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_state_set(l2sw_uint8 units, l2sw_enable_t enabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_state_set(units, enabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_state_get(l2sw_uint8 units, l2sw_enable_t *pEnabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_state_get(units, pEnabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_static_router_port_set(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_static_router_port_set(units, pPortmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_static_router_port_get(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_static_router_port_get(units, pPortmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_protocol_set(l2sw_uint8 units, l2sw_port_t port, l2sw_igmp_protocol_t protocol, l2sw_igmp_action_t action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_protocol_set(units, port, protocol, action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_igmp_protocol_get
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
l2sw_api_ret_t l2sw_igmp_protocol_get(l2sw_uint8 units, l2sw_port_t port, l2sw_igmp_protocol_t protocol, l2sw_igmp_action_t *pAction)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_protocol_get(units, port, protocol, pAction);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_fastLeave_set(l2sw_uint8 units, l2sw_enable_t state)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_fastLeave_set(units, state);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_fastLeave_get(l2sw_uint8 units, l2sw_enable_t *pState)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_fastLeave_get(units, pState);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_maxGroup_set(l2sw_uint8 units, l2sw_port_t port, l2sw_uint32 group)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_maxGroup_set(units, port, group);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_maxGroup_get(l2sw_uint8 units, l2sw_port_t port, l2sw_uint32 *pGroup)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_maxGroup_get(units, port, pGroup);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_currentGroup_get(l2sw_uint8 units, l2sw_port_t port, l2sw_uint32 *pGroup)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_currentGroup_get(units, port, pGroup);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_tableFullAction_set(l2sw_uint8 units, l2sw_igmp_tableFullAction_t action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_tableFullAction_set(units, action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_tableFullAction_get(l2sw_uint8 units, l2sw_igmp_tableFullAction_t *pAction)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_tableFullAction_get(units, pAction);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_checksumErrorAction_set(l2sw_uint8 units, l2sw_igmp_checksumErrorAction_t action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_checksumErrorAction_set(units, action);
    L2SW_API_UNLOCK(units);

    return retVal;
}


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
l2sw_api_ret_t l2sw_igmp_checksumErrorAction_get(l2sw_uint8 units, l2sw_igmp_checksumErrorAction_t *pAction)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_checksumErrorAction_get(units, pAction);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_queryInterval_set(l2sw_uint8 units, l2sw_uint32 interval)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_queryInterval_set(units, interval);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_queryInterval_get(l2sw_uint8 units, l2sw_uint32 *pInterval)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_queryInterval_get(units, pInterval);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_dynamicRouterPort_queryInterval_set(l2sw_uint8 units, l2sw_uint32 interval)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_dynamicRouterPort_queryInterval_set(units, interval);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_dynamicRouterPort_queryInterval_get(l2sw_uint8 units, l2sw_uint32 *pInterval)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_dynamicRouterPort_queryInterval_get(units, pInterval);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_robustness_set(l2sw_uint8 units, l2sw_uint32 robustness)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_robustness_set(units, robustness);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_robustness_get(l2sw_uint8 units, l2sw_uint32 *pRobustness)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_robustness_get(units, pRobustness);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_igmp_dynamicRouterRortAllow_set
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
l2sw_api_ret_t l2sw_igmp_dynamicRouterPortAllow_set(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_dynamicRouterPortAllow_set(units, pPortmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_igmp_dynamicRouterRortAllow_get
 * Description:
 *      Get dynamic router port allow option
 * Input:
 *      None.
 * Output:
 *      pPortmask    - Dynamic Port allow mask
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_NULL_POINTER    - Null pointer
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_PORT_MASK       - Error parameter
 * Note:
 *
 */
l2sw_api_ret_t l2sw_igmp_dynamicRouterPortAllow_get(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_dynamicRouterPortAllow_get(units, pPortmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_dynamicRouterPort_get(l2sw_uint8 units, l2sw_igmp_dynamicRouterPort_t *pDynamicRouterPort)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_dynamicRouterPort_get(units, pDynamicRouterPort);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_suppressionEnable_set(l2sw_uint8 units, l2sw_enable_t reportSuppression, l2sw_enable_t leaveSuppression)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_suppressionEnable_set(units, reportSuppression, leaveSuppression);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_suppressionEnable_get(l2sw_uint8 units, l2sw_enable_t *pReportSuppression, l2sw_enable_t *pLeaveSuppression)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_suppressionEnable_get(units, pReportSuppression, pLeaveSuppression);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_portRxPktEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_igmp_rxPktEnable_t *pRxCfg)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_portRxPktEnable_set(units, port, pRxCfg);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_igmp_portRxPktEnable_get
 * Description:
 *      Get IGMP/MLD RX Packet configuration
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
l2sw_api_ret_t l2sw_igmp_portRxPktEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_igmp_rxPktEnable_t *pRxCfg)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_portRxPktEnable_get(units, port, pRxCfg);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_groupInfo_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_igmp_groupInfo_t *pGroup)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_groupInfo_get(units, index, pGroup);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_ReportLeaveFwdAction_set(l2sw_uint8 units, l2sw_igmp_ReportLeaveFwdAct_t action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_ReportLeaveFwdAction_set(units, action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_igmp_ReportLeaveFwdAction_get
 * Description:
 *      Get Report Leave packet forwarding action
 * Input:
 *      action      - Action
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Error Input
 *      L2SW_ERR_NULL_POINTER    - Null Pointer
 * Note:
 *
 */
l2sw_api_ret_t l2sw_igmp_ReportLeaveFwdAction_get(l2sw_uint8 units, l2sw_igmp_ReportLeaveFwdAct_t *pAction)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_ReportLeaveFwdAction_get(units, pAction);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_dropLeaveZeroEnable_set(l2sw_uint8 units, l2sw_enable_t enabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_dropLeaveZeroEnable_set(units, enabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_dropLeaveZeroEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_dropLeaveZeroEnable_get(units, pEnabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_bypassGroupRange_set(l2sw_uint8 units, l2sw_igmp_bypassGroup_t group, l2sw_enable_t enabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_bypassGroupRange_set(units, group, enabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_igmp_bypassGroupRange_get(l2sw_uint8 units, l2sw_igmp_bypassGroup_t group, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_igmp_bypassGroupRange_get(units, group, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

