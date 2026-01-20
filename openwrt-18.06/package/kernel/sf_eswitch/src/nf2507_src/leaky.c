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
#include "leaky.h"
#include "sal_string.h"
#include "l2sw_asicdrv_portIsolation.h"
#include "l2sw_asicdrv_rma.h"
#include "l2sw_asicdrv_igmp.h"

static l2sw_api_ret_t _l2sw_leaky_vlan_set(l2sw_uint8 units, l2sw_leaky_type_t type, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 port;
    l2sw_rma_t rmacfg;
    l2sw_uint32 tmp;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= LEAKY_END)
        return L2SW_ERR_INPUT;

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if (type >= 0 && type <= LEAKY_UNDEF_GARP_2F)
    {
        if ((retVal = l2sw_getAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.vlan_leaky = enable;

        if ((retVal = l2sw_setAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (LEAKY_IPMULTICAST == type)
    {
        for (port = 0; port <= L2SW_PORT_ID_MAX; port++)
        {
            if ((retVal = l2sw_setAsicIpMulticastVlanLeaky(units, port,enable)) != L2SW_ERR_OK)
                return retVal;
        }
    }
    else if (LEAKY_IGMP == type)
    {
        if ((retVal = l2sw_setAsicIGMPVLANLeaky(units, enable)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (LEAKY_CDP == type)
    {
        if ((retVal = l2sw_getAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.vlan_leaky = enable;

        if ((retVal = l2sw_setAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (LEAKY_CSSTP == type)
    {
        if ((retVal = l2sw_getAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.vlan_leaky = enable;

        if ((retVal = l2sw_setAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (LEAKY_LLDP == type)
    {
        if ((retVal = l2sw_getAsicRmaLldp(units, &tmp,&rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.vlan_leaky = enable;

        if ((retVal = l2sw_setAsicRmaLldp(units, tmp, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_leaky_vlan_get(l2sw_uint8 units, l2sw_leaky_type_t type, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 port,tmp;
    l2sw_rma_t rmacfg;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= LEAKY_END)
        return L2SW_ERR_INPUT;

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if (type >= 0 && type <= LEAKY_UNDEF_GARP_2F)
    {
        if ((retVal = l2sw_getAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = rmacfg.vlan_leaky;

    }
    else if (LEAKY_IPMULTICAST == type)
    {
        for (port = 0; port <= L2SW_PORT_ID_MAX; port++)
        {
            if ((retVal = l2sw_getAsicIpMulticastVlanLeaky(units, port, &tmp)) != L2SW_ERR_OK)
                return retVal;
            if (port>0&&(tmp!=*pEnable))
                return L2SW_ERR_FAILED;
            *pEnable = tmp;
        }
    }
    else if (LEAKY_IGMP == type)
    {
        if ((retVal = l2sw_getAsicIGMPVLANLeaky(units, &tmp)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = tmp;
    }
    else if (LEAKY_CDP == type)
    {
        if ((retVal = l2sw_getAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = rmacfg.vlan_leaky;
    }
    else if (LEAKY_CSSTP == type)
    {
        if ((retVal = l2sw_getAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = rmacfg.vlan_leaky;
    }
    else if (LEAKY_LLDP == type)
    {
        if ((retVal = l2sw_getAsicRmaLldp(units, &tmp, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = rmacfg.vlan_leaky;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_leaky_portIsolation_set(l2sw_uint8 units, l2sw_leaky_type_t type, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 port;
    l2sw_rma_t rmacfg;
    l2sw_uint32 tmp;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= LEAKY_END)
        return L2SW_ERR_INPUT;

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if (type >= 0 && type <= LEAKY_UNDEF_GARP_2F)
    {
        if ((retVal = l2sw_getAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.portiso_leaky = enable;

        if ((retVal = l2sw_setAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (LEAKY_IPMULTICAST == type)
    {
        for (port = 0; port < L2SW_MAX_NUM_OF_PORT; port++)
        {
            if ((retVal = l2sw_setAsicIpMulticastPortIsoLeaky(units, port, enable)) != L2SW_ERR_OK)
                return retVal;
        }
    }
    else if (LEAKY_IGMP == type)
    {
        if ((retVal = l2sw_setAsicIGMPIsoLeaky(units, enable)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (LEAKY_CDP == type)
    {
        if ((retVal = l2sw_getAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.portiso_leaky = enable;

        if ((retVal = l2sw_setAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (LEAKY_CSSTP == type)
    {
        if ((retVal = l2sw_getAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.portiso_leaky = enable;

        if ((retVal = l2sw_setAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }
    else if (LEAKY_LLDP == type)
    {
        if ((retVal = l2sw_getAsicRmaLldp(units, &tmp, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        rmacfg.portiso_leaky = enable;

        if ((retVal = l2sw_setAsicRmaLldp(units, tmp, &rmacfg)) != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_leaky_portIsolation_get(l2sw_uint8 units, l2sw_leaky_type_t type, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 port, tmp;
    l2sw_rma_t rmacfg;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= LEAKY_END)
        return L2SW_ERR_INPUT;

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if (type >= 0 && type <= LEAKY_UNDEF_GARP_2F)
    {
        if ((retVal = l2sw_getAsicRma(units, type, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = rmacfg.portiso_leaky;

    }
    else if (LEAKY_IPMULTICAST == type)
    {
        for (port = 0; port < L2SW_MAX_NUM_OF_PORT; port++)
        {
            if ((retVal = l2sw_getAsicIpMulticastPortIsoLeaky(units, port, &tmp)) != L2SW_ERR_OK)
                return retVal;
            if (port > 0 &&(tmp != *pEnable))
                return L2SW_ERR_FAILED;
            *pEnable = tmp;
        }
    }
    else if (LEAKY_IGMP == type)
    {
        if ((retVal = l2sw_getAsicIGMPIsoLeaky(units, &tmp)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = tmp;
    }
    else if (LEAKY_CDP == type)
    {
        if ((retVal = l2sw_getAsicRmaCdp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = rmacfg.portiso_leaky;
    }
    else if (LEAKY_CSSTP == type)
    {
        if ((retVal = l2sw_getAsicRmaCsstp(units, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = rmacfg.portiso_leaky;
    }
    else if (LEAKY_LLDP == type)
    {
        if ((retVal = l2sw_getAsicRmaLldp(units, &tmp, &rmacfg)) != L2SW_ERR_OK)
            return retVal;

        *pEnable = rmacfg.portiso_leaky;
    }


    return L2SW_ERR_OK;
}


/* Function Name:
 *      l2sw_leaky_vlan_set
 * Description:
 *      Set VLAN leaky.
 * Input:
 *      type - Packet type for VLAN leaky.
 *      enable - Leaky status.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      This API can set VLAN leaky for RMA ,IGMP/MLD, CDP, CSSTP, and LLDP packets.
 *      The leaky frame types are as following:
 *      - LEAKY_BRG_GROUP,
 *      - LEAKY_FD_PAUSE,
 *      - LEAKY_SP_MCAST,
 *      - LEAKY_1X_PAE,
 *      - LEAKY_UNDEF_BRG_04,
 *      - LEAKY_UNDEF_BRG_05,
 *      - LEAKY_UNDEF_BRG_06,
 *      - LEAKY_UNDEF_BRG_07,
 *      - LEAKY_PROVIDER_BRIDGE_GROUP_ADDRESS,
 *      - LEAKY_UNDEF_BRG_09,
 *      - LEAKY_UNDEF_BRG_0A,
 *      - LEAKY_UNDEF_BRG_0B,
 *      - LEAKY_UNDEF_BRG_0C,
 *      - LEAKY_PROVIDER_BRIDGE_GVRP_ADDRESS,
 *      - LEAKY_8021AB,
 *      - LEAKY_UNDEF_BRG_0F,
 *      - LEAKY_BRG_MNGEMENT,
 *      - LEAKY_UNDEFINED_11,
 *      - LEAKY_UNDEFINED_12,
 *      - LEAKY_UNDEFINED_13,
 *      - LEAKY_UNDEFINED_14,
 *      - LEAKY_UNDEFINED_15,
 *      - LEAKY_UNDEFINED_16,
 *      - LEAKY_UNDEFINED_17,
 *      - LEAKY_UNDEFINED_18,
 *      - LEAKY_UNDEFINED_19,
 *      - LEAKY_UNDEFINED_1A,
 *      - LEAKY_UNDEFINED_1B,
 *      - LEAKY_UNDEFINED_1C,
 *      - LEAKY_UNDEFINED_1D,
 *      - LEAKY_UNDEFINED_1E,
 *      - LEAKY_UNDEFINED_1F,
 *      - LEAKY_GMRP,
 *      - LEAKY_GVRP,
 *      - LEAKY_UNDEF_GARP_22,
 *      - LEAKY_UNDEF_GARP_23,
 *      - LEAKY_UNDEF_GARP_24,
 *      - LEAKY_UNDEF_GARP_25,
 *      - LEAKY_UNDEF_GARP_26,
 *      - LEAKY_UNDEF_GARP_27,
 *      - LEAKY_UNDEF_GARP_28,
 *      - LEAKY_UNDEF_GARP_29,
 *      - LEAKY_UNDEF_GARP_2A,
 *      - LEAKY_UNDEF_GARP_2B,
 *      - LEAKY_UNDEF_GARP_2C,
 *      - LEAKY_UNDEF_GARP_2D,
 *      - LEAKY_UNDEF_GARP_2E,
 *      - LEAKY_UNDEF_GARP_2F,
 *      - LEAKY_IGMP,
 *      - LEAKY_IPMULTICAST.
 *      - LEAKY_CDP,
 *      - LEAKY_CSSTP,
 *      - LEAKY_LLDP.
 */
l2sw_api_ret_t l2sw_leaky_vlan_set(l2sw_uint8 units, l2sw_leaky_type_t type, l2sw_enable_t enable)
{
     l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_leaky_vlan_set(units, type,  enable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_leaky_vlan_get
 * Description:
 *      Get VLAN leaky.
 * Input:
 *      type - Packet type for VLAN leaky.
 * Output:
 *      pEnable - Leaky status.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can get VLAN leaky status for RMA ,IGMP/MLD, CDP, CSSTP, and LLDP  packets.
 *      The leaky frame types are as following:
 *      - LEAKY_BRG_GROUP,
 *      - LEAKY_FD_PAUSE,
 *      - LEAKY_SP_MCAST,
 *      - LEAKY_1X_PAE,
 *      - LEAKY_UNDEF_BRG_04,
 *      - LEAKY_UNDEF_BRG_05,
 *      - LEAKY_UNDEF_BRG_06,
 *      - LEAKY_UNDEF_BRG_07,
 *      - LEAKY_PROVIDER_BRIDGE_GROUP_ADDRESS,
 *      - LEAKY_UNDEF_BRG_09,
 *      - LEAKY_UNDEF_BRG_0A,
 *      - LEAKY_UNDEF_BRG_0B,
 *      - LEAKY_UNDEF_BRG_0C,
 *      - LEAKY_PROVIDER_BRIDGE_GVRP_ADDRESS,
 *      - LEAKY_8021AB,
 *      - LEAKY_UNDEF_BRG_0F,
 *      - LEAKY_BRG_MNGEMENT,
 *      - LEAKY_UNDEFINED_11,
 *      - LEAKY_UNDEFINED_12,
 *      - LEAKY_UNDEFINED_13,
 *      - LEAKY_UNDEFINED_14,
 *      - LEAKY_UNDEFINED_15,
 *      - LEAKY_UNDEFINED_16,
 *      - LEAKY_UNDEFINED_17,
 *      - LEAKY_UNDEFINED_18,
 *      - LEAKY_UNDEFINED_19,
 *      - LEAKY_UNDEFINED_1A,
 *      - LEAKY_UNDEFINED_1B,
 *      - LEAKY_UNDEFINED_1C,
 *      - LEAKY_UNDEFINED_1D,
 *      - LEAKY_UNDEFINED_1E,
 *      - LEAKY_UNDEFINED_1F,
 *      - LEAKY_GMRP,
 *      - LEAKY_GVRP,
 *      - LEAKY_UNDEF_GARP_22,
 *      - LEAKY_UNDEF_GARP_23,
 *      - LEAKY_UNDEF_GARP_24,
 *      - LEAKY_UNDEF_GARP_25,
 *      - LEAKY_UNDEF_GARP_26,
 *      - LEAKY_UNDEF_GARP_27,
 *      - LEAKY_UNDEF_GARP_28,
 *      - LEAKY_UNDEF_GARP_29,
 *      - LEAKY_UNDEF_GARP_2A,
 *      - LEAKY_UNDEF_GARP_2B,
 *      - LEAKY_UNDEF_GARP_2C,
 *      - LEAKY_UNDEF_GARP_2D,
 *      - LEAKY_UNDEF_GARP_2E,
 *      - LEAKY_UNDEF_GARP_2F,
 *      - LEAKY_IGMP,
 *      - LEAKY_IPMULTICAST.
 *      - LEAKY_CDP,
 *      - LEAKY_CSSTP,
 *      - LEAKY_LLDP.
 */
l2sw_api_ret_t l2sw_leaky_vlan_get(l2sw_uint8 units, l2sw_leaky_type_t type, l2sw_enable_t *pEnable)
{
	l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_leaky_vlan_get(units, type, pEnable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_leaky_portIsolation_set
 * Description:
 *      Set port isolation leaky.
 * Input:
 *      type - Packet type for port isolation leaky.
 *      enable - Leaky status.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      This API can set port isolation leaky for RMA ,IGMP/MLD, CDP, CSSTP, and LLDP  packets.
 *      The leaky frame types are as following:
 *      - LEAKY_BRG_GROUP,
 *      - LEAKY_FD_PAUSE,
 *      - LEAKY_SP_MCAST,
 *      - LEAKY_1X_PAE,
 *      - LEAKY_UNDEF_BRG_04,
 *      - LEAKY_UNDEF_BRG_05,
 *      - LEAKY_UNDEF_BRG_06,
 *      - LEAKY_UNDEF_BRG_07,
 *      - LEAKY_PROVIDER_BRIDGE_GROUP_ADDRESS,
 *      - LEAKY_UNDEF_BRG_09,
 *      - LEAKY_UNDEF_BRG_0A,
 *      - LEAKY_UNDEF_BRG_0B,
 *      - LEAKY_UNDEF_BRG_0C,
 *      - LEAKY_PROVIDER_BRIDGE_GVRP_ADDRESS,
 *      - LEAKY_8021AB,
 *      - LEAKY_UNDEF_BRG_0F,
 *      - LEAKY_BRG_MNGEMENT,
 *      - LEAKY_UNDEFINED_11,
 *      - LEAKY_UNDEFINED_12,
 *      - LEAKY_UNDEFINED_13,
 *      - LEAKY_UNDEFINED_14,
 *      - LEAKY_UNDEFINED_15,
 *      - LEAKY_UNDEFINED_16,
 *      - LEAKY_UNDEFINED_17,
 *      - LEAKY_UNDEFINED_18,
 *      - LEAKY_UNDEFINED_19,
 *      - LEAKY_UNDEFINED_1A,
 *      - LEAKY_UNDEFINED_1B,
 *      - LEAKY_UNDEFINED_1C,
 *      - LEAKY_UNDEFINED_1D,
 *      - LEAKY_UNDEFINED_1E,
 *      - LEAKY_UNDEFINED_1F,
 *      - LEAKY_GMRP,
 *      - LEAKY_GVRP,
 *      - LEAKY_UNDEF_GARP_22,
 *      - LEAKY_UNDEF_GARP_23,
 *      - LEAKY_UNDEF_GARP_24,
 *      - LEAKY_UNDEF_GARP_25,
 *      - LEAKY_UNDEF_GARP_26,
 *      - LEAKY_UNDEF_GARP_27,
 *      - LEAKY_UNDEF_GARP_28,
 *      - LEAKY_UNDEF_GARP_29,
 *      - LEAKY_UNDEF_GARP_2A,
 *      - LEAKY_UNDEF_GARP_2B,
 *      - LEAKY_UNDEF_GARP_2C,
 *      - LEAKY_UNDEF_GARP_2D,
 *      - LEAKY_UNDEF_GARP_2E,
 *      - LEAKY_UNDEF_GARP_2F,
 *      - LEAKY_IGMP,
 *      - LEAKY_IPMULTICAST.
 *      - LEAKY_CDP,
 *      - LEAKY_CSSTP,
 *      - LEAKY_LLDP.
 */
l2sw_api_ret_t l2sw_leaky_portIsolation_set(l2sw_uint8 units, l2sw_leaky_type_t type, l2sw_enable_t enable)
{
	l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_leaky_portIsolation_set(units, type, enable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_leaky_portIsolation_get
 * Description:
 *      Get port isolation leaky.
 * Input:
 *      type - Packet type for port isolation leaky.
 * Output:
 *      pEnable - Leaky status.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can get port isolation leaky status for RMA ,IGMP/MLD, CDP, CSSTP, and LLDP  packets.
 *      The leaky frame types are as following:
 *      - LEAKY_BRG_GROUP,
 *      - LEAKY_FD_PAUSE,
 *      - LEAKY_SP_MCAST,
 *      - LEAKY_1X_PAE,
 *      - LEAKY_UNDEF_BRG_04,
 *      - LEAKY_UNDEF_BRG_05,
 *      - LEAKY_UNDEF_BRG_06,
 *      - LEAKY_UNDEF_BRG_07,
 *      - LEAKY_PROVIDER_BRIDGE_GROUP_ADDRESS,
 *      - LEAKY_UNDEF_BRG_09,
 *      - LEAKY_UNDEF_BRG_0A,
 *      - LEAKY_UNDEF_BRG_0B,
 *      - LEAKY_UNDEF_BRG_0C,
 *      - LEAKY_PROVIDER_BRIDGE_GVRP_ADDRESS,
 *      - LEAKY_8021AB,
 *      - LEAKY_UNDEF_BRG_0F,
 *      - LEAKY_BRG_MNGEMENT,
 *      - LEAKY_UNDEFINED_11,
 *      - LEAKY_UNDEFINED_12,
 *      - LEAKY_UNDEFINED_13,
 *      - LEAKY_UNDEFINED_14,
 *      - LEAKY_UNDEFINED_15,
 *      - LEAKY_UNDEFINED_16,
 *      - LEAKY_UNDEFINED_17,
 *      - LEAKY_UNDEFINED_18,
 *      - LEAKY_UNDEFINED_19,
 *      - LEAKY_UNDEFINED_1A,
 *      - LEAKY_UNDEFINED_1B,
 *      - LEAKY_UNDEFINED_1C,
 *      - LEAKY_UNDEFINED_1D,
 *      - LEAKY_UNDEFINED_1E,
 *      - LEAKY_UNDEFINED_1F,
 *      - LEAKY_GMRP,
 *      - LEAKY_GVRP,
 *      - LEAKY_UNDEF_GARP_22,
 *      - LEAKY_UNDEF_GARP_23,
 *      - LEAKY_UNDEF_GARP_24,
 *      - LEAKY_UNDEF_GARP_25,
 *      - LEAKY_UNDEF_GARP_26,
 *      - LEAKY_UNDEF_GARP_27,
 *      - LEAKY_UNDEF_GARP_28,
 *      - LEAKY_UNDEF_GARP_29,
 *      - LEAKY_UNDEF_GARP_2A,
 *      - LEAKY_UNDEF_GARP_2B,
 *      - LEAKY_UNDEF_GARP_2C,
 *      - LEAKY_UNDEF_GARP_2D,
 *      - LEAKY_UNDEF_GARP_2E,
 *      - LEAKY_UNDEF_GARP_2F,
 *      - LEAKY_IGMP,
 *      - LEAKY_IPMULTICAST.
 *      - LEAKY_CDP,
 *      - LEAKY_CSSTP,
 *      - LEAKY_LLDP.
 */
l2sw_api_ret_t l2sw_leaky_portIsolation_get(l2sw_uint8 units, l2sw_leaky_type_t type, l2sw_enable_t *pEnable)
{
	l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_leaky_portIsolation_get(units, type, pEnable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}
