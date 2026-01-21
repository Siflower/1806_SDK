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
#include "trap.h"
#include "sal_string.h"
#include "l2sw_asicdrv_port.h"
#include "l2sw_asicdrv_igmp.h"
#include "l2sw_asicdrv_rma.h"
#include "l2sw_asicdrv_oam.h"
#include "l2sw_asicdrv_svlan.h"
#include "l2sw_asicdrv_unknownMulticast.h"
#include "l2sw_asicdrv_dot1x.h"

static l2sw_api_ret_t _l2sw_trap_unknownUnicastPktGlobalAction_set(l2sw_uint8 units, l2sw_trap_ucast_action_t ucast_action)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (ucast_action >= UCAST_ACTION_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicRegBits(units, L2SW_PORT_SECURIT_CTRL_REG, L2SW_UNKNOWN_UNICAST_DA_BEHAVE_MASK, (l2sw_uint32)ucast_action)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_unknownUnicastPktGlobalAction_get(l2sw_uint8 units, l2sw_trap_ucast_action_t *pUcast_action)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (NULL == pUcast_action)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicRegBits(units, L2SW_PORT_SECURIT_CTRL_REG, L2SW_UNKNOWN_UNICAST_DA_BEHAVE_MASK, (l2sw_uint32 *)pUcast_action)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_unknownUnicastPktAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_trap_ucast_action_t ucast_action)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (ucast_action >= UCAST_ACTION_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicPortUnknownDaBehavior(units, l2sw_switch_port_L2P_get(units, port), ucast_action)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_unknownUnicastPktAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_trap_ucast_action_t *pUcast_action)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (NULL == pUcast_action)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortUnknownDaBehavior(units, l2sw_switch_port_L2P_get(units, port), pUcast_action)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_unknownMacPktAction_set(l2sw_uint8 units, l2sw_trap_ucast_action_t ucast_action)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (ucast_action >= UCAST_ACTION_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicPortUnknownSaBehavior(units, ucast_action)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_unknownMacPktAction_get(l2sw_uint8 units, l2sw_trap_ucast_action_t *pUcast_action)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pUcast_action)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortUnknownSaBehavior(units, pUcast_action)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_unmatchMacPktAction_set(l2sw_uint8 units, l2sw_trap_ucast_action_t ucast_action)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (ucast_action >= UCAST_ACTION_FLOODING)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicPortUnmatchedSaBehavior(units, ucast_action)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_unmatchMacPktAction_get(l2sw_uint8 units, l2sw_trap_ucast_action_t *pUcast_action)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pUcast_action)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortUnmatchedSaBehavior(units, pUcast_action)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_unmatchMacMoving_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(enable >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicPortUnmatchedSaMoving(units, l2sw_switch_port_L2P_get(units, port), enable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_unmatchMacMoving_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortUnmatchedSaMoving(units, l2sw_switch_port_L2P_get(units, port), pEnable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_unknownMcastPktAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_mcast_type_t type, l2sw_trap_mcast_action_t mcast_action)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 rawAction;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (type >= MCAST_END)
        return L2SW_ERR_INPUT;

    if (mcast_action >= MCAST_ACTION_END)
        return L2SW_ERR_INPUT;


    switch (type)
    {
        case MCAST_L2:
            if (MCAST_ACTION_ROUTER_PORT == mcast_action)
                return L2SW_ERR_INPUT;
            else if(MCAST_ACTION_DROP_EX_RMA == mcast_action)
                rawAction = L2_UNKOWN_MULTICAST_DROP_EXCLUDE_RMA;
            else
                rawAction = (l2sw_uint32)mcast_action;

            if ((retVal = l2sw_setAsicUnknownL2MulticastBehavior(units, l2sw_switch_port_L2P_get(units, port), rawAction)) != L2SW_ERR_OK)
                return retVal;

            break;
        case MCAST_IPV4:
            if (MCAST_ACTION_DROP_EX_RMA == mcast_action)
                return L2SW_ERR_INPUT;
            else
                rawAction = (l2sw_uint32)mcast_action;

            if ((retVal = l2sw_setAsicUnknownIPv4MulticastBehavior(units, l2sw_switch_port_L2P_get(units, port), rawAction)) != L2SW_ERR_OK)
                return retVal;

            break;
        case MCAST_IPV6:
            if (MCAST_ACTION_DROP_EX_RMA == mcast_action)
                return L2SW_ERR_INPUT;
            else
                rawAction = (l2sw_uint32)mcast_action;

            if ((retVal = l2sw_setAsicUnknownIPv6MulticastBehavior(units, l2sw_switch_port_L2P_get(units, port), rawAction)) != L2SW_ERR_OK)
                return retVal;

            break;
        default:
            break;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_unknownMcastPktAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_mcast_type_t type, l2sw_trap_mcast_action_t *pMcast_action)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 rawAction;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (type >= MCAST_END)
        return L2SW_ERR_INPUT;

    if(NULL == pMcast_action)
        return L2SW_ERR_NULL_POINTER;

    switch (type)
    {
        case MCAST_L2:
            if ((retVal = l2sw_getAsicUnknownL2MulticastBehavior(units, l2sw_switch_port_L2P_get(units, port), &rawAction)) != L2SW_ERR_OK)
                return retVal;

            if(L2_UNKOWN_MULTICAST_DROP_EXCLUDE_RMA == rawAction)
                *pMcast_action = MCAST_ACTION_DROP_EX_RMA;
            else
                *pMcast_action = (l2sw_trap_mcast_action_t)rawAction;

            break;
        case MCAST_IPV4:
            if ((retVal = l2sw_getAsicUnknownIPv4MulticastBehavior(units, l2sw_switch_port_L2P_get(units, port), &rawAction)) != L2SW_ERR_OK)
                return retVal;

            *pMcast_action = (l2sw_trap_mcast_action_t)rawAction;
            break;
        case MCAST_IPV6:
            if ((retVal = l2sw_getAsicUnknownIPv6MulticastBehavior(units, l2sw_switch_port_L2P_get(units, port), &rawAction)) != L2SW_ERR_OK)
                return retVal;

            *pMcast_action = (l2sw_trap_mcast_action_t)rawAction;
            break;
        default:
            break;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_lldpEnable_set(l2sw_uint8 units, l2sw_enable_t enabled)
{
    l2sw_api_ret_t retVal;
    l2sw_rma_t rmacfg;
    l2sw_enable_t tmp;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (enabled >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if ((retVal = l2sw_getAsicRmaLldp(units, &tmp, &rmacfg)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicRmaLldp(units, enabled, &rmacfg)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_lldpEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnabled)
{
    l2sw_api_ret_t retVal;
    l2sw_rma_t rmacfg;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pEnabled)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicRmaLldp(units, pEnabled, &rmacfg)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_reasonTrapToCpuPriority_set(l2sw_uint8 units, l2sw_trap_reason_type_t type, l2sw_pri_t priority)
{
    l2sw_api_ret_t retVal;
    l2sw_rma_t rmacfg;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= TRAP_REASON_END)
        return L2SW_ERR_INPUT;

    if (priority > L2SW_PRIMAX)
        return  L2SW_ERR_QOS_INT_PRIORITY;

    switch (type)
    {
        case TRAP_REASON_RMA:
            if ((retVal = l2sw_getAsicRma(units, 0, &rmacfg)) != L2SW_ERR_OK)
                return retVal;
            rmacfg.trap_priority= priority;
            if ((retVal = l2sw_setAsicRma(units, 0, &rmacfg)) != L2SW_ERR_OK)
                return retVal;

            break;
        case TRAP_REASON_OAM:
            if ((retVal = l2sw_setAsicOamCpuPri(units, priority)) != L2SW_ERR_OK)
                return retVal;

            break;
        case TRAP_REASON_1XUNAUTH:
            if ((retVal = l2sw_setAsic1xTrapPriority(units, priority)) != L2SW_ERR_OK)
                return retVal;

            break;
        case TRAP_REASON_VLANSTACK:
            if ((retVal = l2sw_setAsicSvlanTrapPriority(units, priority)) != L2SW_ERR_OK)
                return retVal;

            break;
        case TRAP_REASON_UNKNOWNMC:
            if ((retVal = l2sw_setAsicUnknownMulticastTrapPriority(units, priority)) != L2SW_ERR_OK)
                return retVal;

            break;
        default:
            return L2SW_ERR_CHIP_NOT_SUPPORTED;
    }


    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_reasonTrapToCpuPriority_get(l2sw_uint8 units, l2sw_trap_reason_type_t type, l2sw_pri_t *pPriority)
{
    l2sw_api_ret_t retVal;
    l2sw_rma_t rmacfg;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= TRAP_REASON_END)
        return L2SW_ERR_INPUT;

    if(NULL == pPriority)
        return L2SW_ERR_NULL_POINTER;

    switch (type)
    {
        case TRAP_REASON_RMA:
            if ((retVal = l2sw_getAsicRma(units, 0, &rmacfg)) != L2SW_ERR_OK)
                return retVal;
            *pPriority = rmacfg.trap_priority;

            break;
        case TRAP_REASON_OAM:
            if ((retVal = l2sw_getAsicOamCpuPri(units, pPriority)) != L2SW_ERR_OK)
                return retVal;

            break;
        case TRAP_REASON_1XUNAUTH:
            if ((retVal = l2sw_getAsic1xTrapPriority(units, pPriority)) != L2SW_ERR_OK)
                return retVal;

            break;
        case TRAP_REASON_VLANSTACK:
            if ((retVal = l2sw_getAsicSvlanTrapPriority(units, pPriority)) != L2SW_ERR_OK)
                return retVal;

            break;
        case TRAP_REASON_UNKNOWNMC:
            if ((retVal = l2sw_getAsicUnknownMulticastTrapPriority(units, pPriority)) != L2SW_ERR_OK)
                return retVal;

            break;
        default:
            return L2SW_ERR_CHIP_NOT_SUPPORTED;

    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_rmaAction_set(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_trap_rma_action_t rma_action)
{
    l2sw_api_ret_t retVal;
    l2sw_rma_t rmacfg;
    l2sw_uint32 tmp;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= TRAP_END)
        return L2SW_ERR_INPUT;

    if (rma_action >= RMA_ACTION_END)
        return L2SW_ERR_RMA_ACTION;

    if (type >= 0 && type <= TRAP_UNDEF_GARP_2F)
    {
        if ((retVal = l2sw_getAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.operation = rma_action;

        if ((retVal = l2sw_setAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (type == TRAP_CDP)
    {
        if ((retVal = l2sw_getAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.operation = rma_action;

        if ((retVal = l2sw_setAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (type  == TRAP_CSSTP)
    {
        if ((retVal = l2sw_getAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.operation = rma_action;

        if ((retVal = l2sw_setAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (type  == TRAP_LLDP)
    {
        if ((retVal = l2sw_getAsicRmaLldp(units, &tmp, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.operation = rma_action;

        if ((retVal = l2sw_setAsicRmaLldp(units, tmp, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else
        return L2SW_ERR_INPUT;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_rmaAction_get(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_trap_rma_action_t *pRma_action)
{
    l2sw_api_ret_t retVal;
    l2sw_rma_t rmacfg;
    l2sw_uint32 tmp;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= TRAP_END)
        return L2SW_ERR_INPUT;

    if(NULL == pRma_action)
        return L2SW_ERR_NULL_POINTER;

    if (type >= 0 && type <= TRAP_UNDEF_GARP_2F)
    {
        if ((retVal = l2sw_getAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pRma_action = rmacfg.operation;
    }
    else if (type == TRAP_CDP)
    {
        if ((retVal = l2sw_getAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pRma_action = rmacfg.operation;
    }
    else if (type == TRAP_CSSTP)
    {
        if ((retVal = l2sw_getAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pRma_action = rmacfg.operation;
    }
    else if (type == TRAP_LLDP)
    {
        if ((retVal = l2sw_getAsicRmaLldp(units, &tmp,&rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pRma_action = rmacfg.operation;
    }
    else
        return L2SW_ERR_INPUT;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_rmaKeepFormat_set(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;
    l2sw_rma_t rmacfg;
    l2sw_uint32 tmp;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= TRAP_END)
        return L2SW_ERR_INPUT;

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if (type >= 0 && type <= TRAP_UNDEF_GARP_2F)
    {
        if ((retVal = l2sw_getAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.keep_format = enable;

        if ((retVal = l2sw_setAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (type == TRAP_CDP)
    {
        if ((retVal = l2sw_getAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.keep_format = enable;

        if ((retVal = l2sw_setAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (type  == TRAP_CSSTP)
    {
        if ((retVal = l2sw_getAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.keep_format = enable;

        if ((retVal = l2sw_setAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (type  == TRAP_LLDP)
    {
        if ((retVal = l2sw_getAsicRmaLldp(units, &tmp, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.keep_format = enable;

        if ((retVal = l2sw_setAsicRmaLldp(units, tmp, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else
        return L2SW_ERR_INPUT;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_rmaKeepFormat_get(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    l2sw_rma_t rmacfg;
    l2sw_uint32 tmp;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= TRAP_END)
        return L2SW_ERR_INPUT;

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if (type >= 0 && type <= TRAP_UNDEF_GARP_2F)
    {
        if ((retVal = l2sw_getAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = rmacfg.keep_format;
    }
    else if (type == TRAP_CDP)
    {
        if ((retVal = l2sw_getAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = rmacfg.keep_format;
    }
    else if (type == TRAP_CSSTP)
    {
        if ((retVal = l2sw_getAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = rmacfg.keep_format;
    }
    else if (type == TRAP_LLDP)
    {
        if ((retVal = l2sw_getAsicRmaLldp(units, &tmp,&rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = rmacfg.keep_format;
    }
    else
        return L2SW_ERR_INPUT;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_rmaTrapToCpuPri_set(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_pri_t pri)
{
    l2sw_api_ret_t retVal;
    l2sw_rma_t rmacfg;
    l2sw_uint32 tmp;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= TRAP_END)
        return L2SW_ERR_INPUT;

    if (pri > L2SW_PRIMAX)
        return L2SW_ERR_QOS_INT_PRIORITY;

    if (type >= 0 && type <= TRAP_UNDEF_GARP_2F)
    {
        if ((retVal = l2sw_getAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.trap_priority = pri;

        if ((retVal = l2sw_setAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (type == TRAP_CDP)
    {
        if ((retVal = l2sw_getAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.trap_priority = pri;

        if ((retVal = l2sw_setAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (type  == TRAP_CSSTP)
    {
        if ((retVal = l2sw_getAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.trap_priority = pri;

        if ((retVal = l2sw_setAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (type  == TRAP_LLDP)
    {
        if ((retVal = l2sw_getAsicRmaLldp(units, &tmp, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.trap_priority = pri;

        if ((retVal = l2sw_setAsicRmaLldp(units, tmp, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else
        return L2SW_ERR_INPUT;
    
    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_rmaTrapToCpuPri_get(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_pri_t *pri)
{
    l2sw_api_ret_t retVal;
    l2sw_rma_t rmacfg;
    l2sw_uint32 tmp;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= TRAP_END)
        return L2SW_ERR_INPUT;

    if(NULL == pri)
        return L2SW_ERR_NULL_POINTER;

    if (type >= 0 && type <= TRAP_UNDEF_GARP_2F)
    {
        if ((retVal = l2sw_getAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pri = rmacfg.trap_priority;
    }
    else if (type == TRAP_CDP)
    {
        if ((retVal = l2sw_getAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pri = rmacfg.trap_priority;
    }
    else if (type == TRAP_CSSTP)
    {
        if ((retVal = l2sw_getAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pri = rmacfg.trap_priority;
    }
    else if (type == TRAP_LLDP)
    {
        if ((retVal = l2sw_getAsicRmaLldp(units, &tmp,&rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pri = rmacfg.trap_priority;
    }
    else
        return L2SW_ERR_INPUT;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_rldpPri_set(l2sw_uint8 units, l2sw_pri_t pri)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((retVal = l2sw_setAsicRegBits(units, L2SW_REG_QOS_TRAP_PRIORITY1,L2SW_DW8051_TRAP_PRI_MASK,pri)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trap_rldpPri_get(l2sw_uint8 units, l2sw_pri_t *pri)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((retVal = l2sw_getAsicRegBits(units, L2SW_REG_QOS_TRAP_PRIORITY1,L2SW_DW8051_TRAP_PRI_MASK,pri)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}


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
l2sw_api_ret_t l2sw_trap_unknownUnicastPktGlobalAction_set(l2sw_uint8 units, l2sw_trap_ucast_action_t ucast_action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_unknownUnicastPktGlobalAction_set(units, ucast_action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      f_trap_unknownUnicastPktGlobalAction_get
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
l2sw_api_ret_t l2sw_trap_unknownUnicastPktGlobalAction_get(l2sw_uint8 units, l2sw_trap_ucast_action_t *pUcast_action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_unknownUnicastPktGlobalAction_get(units, pUcast_action);
    L2SW_API_UNLOCK(units);

    return retVal;
}


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
l2sw_api_ret_t l2sw_trap_unknownUnicastPktAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_trap_ucast_action_t ucast_action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_unknownUnicastPktAction_set(units, port, ucast_action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_trap_unknownUnicastPktAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_trap_ucast_action_t *pUcast_action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_unknownUnicastPktAction_get(units, port, pUcast_action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_trap_unknownMacPktAction_set(l2sw_uint8 units, l2sw_trap_ucast_action_t ucast_action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_unknownMacPktAction_set(units, ucast_action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_trap_unknownMacPktAction_get(l2sw_uint8 units, l2sw_trap_ucast_action_t *pUcast_action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_unknownMacPktAction_get(units, pUcast_action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_trap_unmatchMacPktAction_set(l2sw_uint8 units, l2sw_trap_ucast_action_t ucast_action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_unmatchMacPktAction_set(units, ucast_action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_trap_unmatchMacPktAction_get(l2sw_uint8 units, l2sw_trap_ucast_action_t *pUcast_action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_unmatchMacPktAction_get(units, pUcast_action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_trap_unmatchMacMoving_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_unmatchMacMoving_set(units, port, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_trap_unmatchMacMoving_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_unmatchMacMoving_get(units, port, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
 *      L2SW_ERR_PORT_ID         - Invalid port number.
 *      L2SW_ERR_NOT_ALLOWED     - Invalid action.
 *      L2SW_ERR_INPUT         - Invalid input parameters.
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
l2sw_api_ret_t l2sw_trap_unknownMcastPktAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_mcast_type_t type, l2sw_trap_mcast_action_t mcast_action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_unknownMcastPktAction_set(units, port, type, mcast_action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_trap_unknownMcastPktAction_get
 * Description:
 *      Get behavior of unknown multicast
 * Input:
 *      type - unknown multicast packet type.
 * Output:
 *      pMcast_action - unknown multicast action.
 * Return:
 *      L2SW_ERR_OK                  - OK
 *      L2SW_ERR_FAILED              - Failed
 *      L2SW_ERR_SMI                 - SMI access error
 *      L2SW_ERR_PORT_ID             - Invalid port number.
 *      L2SW_ERR_NOT_ALLOWED         - Invalid operation.
 *      L2SW_ERR_INPUT             - Invalid input parameters.
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
l2sw_api_ret_t l2sw_trap_unknownMcastPktAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_mcast_type_t type, l2sw_trap_mcast_action_t *pMcast_action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_unknownMcastPktAction_get(units, port, type, pMcast_action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_trap_lldpEnable_set
 * Description:
 *      Set LLDP enable.
 * Input:
 *      enabled - LLDP enable, 0: follow RMA, 1: use LLDP action.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                  - OK
 *      L2SW_ERR_FAILED              - Failed
 *      L2SW_ERR_SMI                 - SMI access error
 *      L2SW_ERR_NOT_ALLOWED         - Invalid action.
 *      L2SW_ERR_INPUT             - Invalid input parameters.
 * Note:
 *      - DMAC                                                 Assignment
 *      - 01:80:c2:00:00:0e ethertype = 0x88CC    LLDP
 *      - 01:80:c2:00:00:03 ethertype = 0x88CC
 *      - 01:80:c2:00:00:00 ethertype = 0x88CC

 */
l2sw_api_ret_t l2sw_trap_lldpEnable_set(l2sw_uint8 units, l2sw_enable_t enabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_lldpEnable_set(units, enabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
 *      L2SW_ERR_INPUT         - Invalid input parameters.
 * Note:
 *      LLDP is as following definition.
 *      - DMAC                                                 Assignment
 *      - 01:80:c2:00:00:0e ethertype = 0x88CC    LLDP
 *      - 01:80:c2:00:00:03 ethertype = 0x88CC
 *      - 01:80:c2:00:00:00 ethertype = 0x88CC
 */
l2sw_api_ret_t l2sw_trap_lldpEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_lldpEnable_get(units, pEnabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_trap_reasonTrapToCpuPriority_set(l2sw_uint8 units, l2sw_trap_reason_type_t type, l2sw_pri_t priority)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_reasonTrapToCpuPriority_set(units, type, priority);
    L2SW_API_UNLOCK(units);

    return retVal;
}


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
l2sw_api_ret_t l2sw_trap_reasonTrapToCpuPriority_get(l2sw_uint8 units, l2sw_trap_reason_type_t type, l2sw_pri_t *pPriority)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_reasonTrapToCpuPriority_get(units, type, pPriority);
    L2SW_API_UNLOCK(units);

    return retVal;
}



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
l2sw_api_ret_t l2sw_trap_rmaAction_set(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_trap_rma_action_t rma_action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_rmaAction_set(units, type, rma_action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_trap_rmaAction_get(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_trap_rma_action_t *pRma_action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_rmaAction_get(units, type, pRma_action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_trap_rmaKeepFormat_set(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_rmaKeepFormat_set(units, type, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_trap_rmaKeepFormat_get(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_rmaKeepFormat_get(units, type, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_trap_rmaTrapToCpuPri_set(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_pri_t pri)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_rmaTrapToCpuPri_set(units, type, pri);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_trap_rmaTrapToCpuPri_get(l2sw_uint8 units, l2sw_trap_type_t type, l2sw_pri_t *pri)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_rmaTrapToCpuPri_get(units, type, pri);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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

l2sw_api_ret_t l2sw_trap_rldpPri_set(l2sw_uint8 units, l2sw_pri_t pri)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_rldpPri_set(units, pri);
    L2SW_API_UNLOCK(units);

    return retVal;

}

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

l2sw_api_ret_t l2sw_trap_rldpPri_get(l2sw_uint8 units, l2sw_pri_t *pri)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trap_rldpPri_get(units, pri);
    L2SW_API_UNLOCK(units);

    return retVal;

}

