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

#include "l2sw_asicdrv_igmp.h"
/* Function Name:
 *      l2sw_setAsicIgmp
 * Description:
 *      Set IGMP/MLD state
 * Input:
 *      enabled     - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicIgmp(l2sw_uint8 units, l2sw_uint32 enabled)
{
    ret_t retVal;

    /* Enable/Disable H/W IGMP/MLD */
    retVal = l2sw_setAsicRegBit(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_IGMP_MLD_EN_OFFSET, enabled);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicIgmp
 * Description:
 *      Get IGMP/MLD state
 * Input:
 *      enabled     - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIgmp(l2sw_uint8 units, l2sw_uint32 *ptr_enabled)
{
    ret_t retVal;

    retVal = l2sw_getAsicRegBit(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_IGMP_MLD_EN_OFFSET, ptr_enabled);
    return retVal;
}
/* Function Name:
 *      l2sw_setAsicIpMulticastVlanLeaky
 * Description:
 *      Set IP multicast VLAN Leaky function
 * Input:
 *      port        - Physical port number (0~7)
 *      enabled     - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      When enabling this function,
 *      if the lookup result(forwarding portmap) of IP Multicast packet is over VLAN boundary,
 *      the packet can be forwarded across VLAN
 */
ret_t l2sw_setAsicIpMulticastVlanLeaky(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled)
{
    ret_t  retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    retVal = l2sw_setAsicRegBit(units, L2SW_REG_IPMCAST_VLAN_LEAKY, port, enabled);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicIpMulticastVlanLeaky
 * Description:
 *      Get IP multicast VLAN Leaky function
 * Input:
 *      port        - Physical port number (0~7)
 *      enabled     - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicIpMulticastVlanLeaky(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *ptr_enabled)
{
    ret_t  retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    retVal = l2sw_getAsicRegBit(units, L2SW_REG_IPMCAST_VLAN_LEAKY, port, ptr_enabled);

    return retVal;
}

/* Function Name:
 *      l2sw_setAsicIGMPTableFullOP
 * Description:
 *      Set Table Full operation
 * Input:
 *      operation   - The operation should be taken when the IGMP table is full.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter is out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPTableFullOP(l2sw_uint8 units, l2sw_uint32 operation)
{
    ret_t  retVal;

    if(operation >= TABLE_FULL_OP_END)
        return L2SW_ERR_OUT_OF_RANGE;

    /* Table full Operation */
    retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_MLD_CFG1, L2SW_TABLE_FULL_OP_MASK, operation);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPTableFullOP
 * Description:
 *      Get Table Full operation
 * Input:
 *      None
 * Output:
 *      poperation  - The operation should be taken when the IGMP table is full.
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPTableFullOP(l2sw_uint8 units, l2sw_uint32 *poperation)
{
    ret_t   retVal;
    l2sw_uint32  value;

    /* Table full Operation */
    retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_MLD_CFG1, L2SW_TABLE_FULL_OP_MASK, &value);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *poperation = value;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPCRCErrOP
 * Description:
 *      Set the operation when ASIC receive a Checksum error packet
 * Input:
 *      operation   -The operation when ASIC receive a Checksum error packet
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter is out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPCRCErrOP(l2sw_uint8 units, l2sw_uint32 operation)
{
    ret_t  retVal;

    if(operation >= CRC_ERR_OP_END)
        return L2SW_ERR_OUT_OF_RANGE;

    /* CRC Error Operation */
    retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_CKS_ERR_OP_MASK, operation);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPCRCErrOP
 * Description:
 *      Get the operation when ASIC receive a Checksum error packet
 * Input:
 *      None
 * Output:
 *      poperation  - The operation of Checksum error packet
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPCRCErrOP(l2sw_uint8 units, l2sw_uint32 *poperation)
{
    ret_t   retVal;
    l2sw_uint32  value;

    /* CRC Error Operation */
    retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_CKS_ERR_OP_MASK, &value);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *poperation = value;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPFastLeaveEn
 * Description:
 *      Enable/Disable Fast Leave
 * Input:
 *      enabled - 1:enable Fast Leave; 0:disable Fast Leave
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPFastLeaveEn(l2sw_uint8 units, l2sw_uint32 enabled)
{
    ret_t  retVal;

    /* Fast Leave */
    retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_FAST_LEAVE_EN_MASK, (enabled >= 1) ? 1 : 0);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPFastLeaveEn
 * Description:
 *      Get Fast Leave state
 * Input:
 *      None
 * Output:
 *      penabled        - 1:enable Fast Leave; 0:disable Fast Leave
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPFastLeaveEn(l2sw_uint8 units, l2sw_uint32 *penabled)
{
    ret_t   retVal;
    l2sw_uint32  value;

    /* Fast Leave */
    retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_FAST_LEAVE_EN_MASK, &value);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *penabled = value;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPQueryInterval
 * Description:
 *      Set Query Interval of IGMP/MLD
 * Input:
 *      interval    - Query Interval
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter is out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPQueryInterval(l2sw_uint8 units, l2sw_uint32 interval)
{
    ret_t  retVal;

    if(interval > L2SW_MAX_QUERY_INT)
        return L2SW_ERR_OUT_OF_RANGE;

    /* Query Interval */
    retVal = l2sw_setAsicReg(units, L2SW_REG_IGMP_MLD_CFG2, interval);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPQueryInterval
 * Description:
 *      Get Query Interval of IGMP/MLD
 * Input:
 *      None
 * Output:
 *      pinterval       - Query Interval
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPQueryInterval(l2sw_uint8 units, l2sw_uint32 *pinterval)
{
    ret_t   retVal;
    l2sw_uint32  value;

    /* Query Interval */
    retVal = l2sw_getAsicReg(units, L2SW_REG_IGMP_MLD_CFG2, &value);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *pinterval = value;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPDynamicRouterPortQueryInterval
 * Description:
 *      Set Dynamic Router Port Query Interval of IGMP/MLD
 * Input:
 *      interval    - Query Interval
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter is out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPDynamicRouterPortQueryInterval(l2sw_uint8 units, l2sw_uint32 interval)
{
    ret_t  retVal;

    if(interval > L2SW_MAX_QUERY_INT)
        return L2SW_ERR_OUT_OF_RANGE;

    /* Query Interval */
    retVal = l2sw_setAsicReg(units, L2SW_REG_ROUTER_PORT_INTERVAL, interval);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPDynamicRouterPortQueryInterval
 * Description:
 *      Get Query Interval of IGMP/MLD
 * Input:
 *      None
 * Output:
 *      pinterval       - Query Interval
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPDynamicRouterPortQueryInterval(l2sw_uint8 units, l2sw_uint32 *pinterval)
{
    ret_t   retVal;
    l2sw_uint32  value;

    /* Query Interval */
    retVal = l2sw_getAsicReg(units, L2SW_REG_ROUTER_PORT_INTERVAL, &value);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *pinterval = value;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPRobVar
 * Description:
 *      Set Robustness Variable of IGMP/MLD
 * Input:
 *      rob_var     - Robustness Variable
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter is out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPRobVar(l2sw_uint8 units, l2sw_uint32 rob_var)
{
    ret_t  retVal;

    if(rob_var > L2SW_MAX_ROB_VAR)
        return L2SW_ERR_OUT_OF_RANGE;

    /* Bourstness variable */
    retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_ROBURSTNESS_VAR_MASK, rob_var);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPRobVar
 * Description:
 *      Get Robustness Variable of IGMP/MLD
 * Input:
 *      none
 * Output:
 *      prob_var     - Robustness Variable
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPRobVar(l2sw_uint8 units, l2sw_uint32 *prob_var)
{
    ret_t   retVal;
    l2sw_uint32  value;

    /* Bourstness variable */
    retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_ROBURSTNESS_VAR_MASK, &value);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *prob_var = value;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPStaticRouterPort
 * Description:
 *      Set IGMP static router port mask
 * Input:
 *      pmsk    - Static portmask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_MASK    - Invalid port mask
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPStaticRouterPort(l2sw_uint8 units, l2sw_uint32 pmsk)
{
    if(pmsk > L2SW_PORTMASK)
        return L2SW_ERR_PORT_MASK;

    return l2sw_setAsicRegBits(units, L2SW_REG_IGMP_STATIC_ROUTER_PORT, L2SW_IGMP_STATIC_ROUTER_PORT_MASK, pmsk);
}

/* Function Name:
 *      l2sw_getAsicIGMPStaticRouterPort
 * Description:
 *      Get IGMP static router port mask
 * Input:
 *      pmsk    - Static portmask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPStaticRouterPort(l2sw_uint8 units, l2sw_uint32 *pmsk)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_IGMP_STATIC_ROUTER_PORT, L2SW_IGMP_STATIC_ROUTER_PORT_MASK, pmsk);
}

/* Function Name:
 *      l2sw_setAsicIGMPAllowDynamicRouterPort
 * Description:
 *      Set IGMP dynamic router port allow mask
 * Input:
 *      pmsk    - Allow dynamic router port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_MASK    - Invalid port mask
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPAllowDynamicRouterPort(l2sw_uint8 units, l2sw_uint32 pmsk)
{
    return l2sw_setAsicReg(units, L2SW_REG_IGMP_MLD_CFG4, pmsk);
}

/* Function Name:
 *      l2sw_getAsicIGMPAllowDynamicRouterPort
 * Description:
 *      Get IGMP dynamic router port allow mask
 * Input:
 *      None.
 * Output:
 *      pPmsk   - Allow dynamic router port mask
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_MASK    - Invalid port mask
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPAllowDynamicRouterPort(l2sw_uint8 units, l2sw_uint32 *pPmsk)
{
    return l2sw_getAsicReg(units, L2SW_REG_IGMP_MLD_CFG4, pPmsk);
}

/* Function Name:
 *      l2sw_getAsicIGMPdynamicRouterPort1
 * Description:
 *      Get 1st dynamic router port and timer
 * Input:
 *      port    - Physical port number (0~7)
 *      timer   - router port timer
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPdynamicRouterPort1(l2sw_uint8 units, l2sw_uint32 *port, l2sw_uint32 *timer)
{
    ret_t   retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_DYNAMIC_ROUTER_PORT, L2SW_D_ROUTER_PORT_1_MASK, port);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_DYNAMIC_ROUTER_PORT, L2SW_D_ROUTER_PORT_TMR_1_MASK, timer);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPdynamicRouterPort2
 * Description:
 *      Get 2nd dynamic router port and timer
 * Input:
 *      port    - Physical port number (0~7)
 *      timer   - router port timer
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPdynamicRouterPort2(l2sw_uint8 units, l2sw_uint32 *port, l2sw_uint32 *timer)
{
    ret_t   retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_DYNAMIC_ROUTER_PORT, L2SW_D_ROUTER_PORT_2_MASK, port);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_DYNAMIC_ROUTER_PORT, L2SW_D_ROUTER_PORT_TMR_2_MASK, timer);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPSuppression
 * Description:
 *      Set the suppression function
 * Input:
 *      report_supp_enabled     - Report suppression, 1:Enable, 0:disable
 *      leave_supp_enabled      - Leave suppression, 1:Enable, 0:disable
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPSuppression(l2sw_uint8 units, l2sw_uint32 report_supp_enabled, l2sw_uint32 leave_supp_enabled)
{
    ret_t   retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_REPORT_SUPPRESSION_MASK, report_supp_enabled);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_LEAVE_SUPPRESSION_MASK, leave_supp_enabled);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPSuppression
 * Description:
 *      Get the suppression function
 * Input:
 *      report_supp_enabled     - Report suppression, 1:Enable, 0:disable
 *      leave_supp_enabled      - Leave suppression, 1:Enable, 0:disable
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPSuppression(l2sw_uint8 units, l2sw_uint32 *report_supp_enabled, l2sw_uint32 *leave_supp_enabled)
{
    ret_t   retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_REPORT_SUPPRESSION_MASK, report_supp_enabled);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_LEAVE_SUPPRESSION_MASK, leave_supp_enabled);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPQueryRX
 * Description:
 *      Set port-based Query packet RX allowance
 * Input:
 *      port            - port number
 *      allow_query     - allowance of Query packet RX, 1:Allow, 0:Drop
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPQueryRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 allow_query)
{
    ret_t   retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    /* Allow Query */
    if (port < 8)
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_ALLOW_QUERY_MASK, allow_query);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_ALLOW_QUERY_MASK, allow_query);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPQueryRX
 * Description:
 *      Get port-based Query packet RX allowance
 * Input:
 *      port            - port number
 * Output:
 *      allow_query     - allowance of Query packet RX, 1:Allow, 0:Drop
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPQueryRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *allow_query)
{
    ret_t   retVal;
    l2sw_uint32  value;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    /* Allow Query */
    if (port < 8)
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_ALLOW_QUERY_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_ALLOW_QUERY_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    *allow_query = value;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPReportRX
 * Description:
 *      Set port-based Report packet RX allowance
 * Input:
 *      port            - port number
 *      allow_report    - allowance of Report packet RX, 1:Allow, 0:Drop
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPReportRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 allow_report)
{
    ret_t   retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 8)
    {
    /* Allow Report */
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_ALLOW_REPORT_MASK, allow_report);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_ALLOW_REPORT_MASK, allow_report);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPReportRX
 * Description:
 *      Get port-based Report packet RX allowance
 * Input:
 *      port            - port number
 * Output:
 *      allow_report    - allowance of Report packet RX, 1:Allow, 0:Drop
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPReportRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *allow_report)
{
    ret_t   retVal;
    l2sw_uint32  value;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 8)
    {
        /* Allow Report */
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_ALLOW_REPORT_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_ALLOW_REPORT_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    *allow_report = value;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPLeaveRX
 * Description:
 *      Set port-based Leave packet RX allowance
 * Input:
 *      port            - port number
 *      allow_leave     - allowance of Leave packet RX, 1:Allow, 0:Drop
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPLeaveRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 allow_leave)
{
    ret_t   retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 8)
    {
        /* Allow Leave */
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_ALLOW_LEAVE_MASK, allow_leave);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_ALLOW_LEAVE_MASK, allow_leave);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPLeaveRX
 * Description:
 *      Get port-based Leave packet RX allowance
 * Input:
 *      port            - port number
 * Output:
 *      allow_leave     - allowance of Leave packet RX, 1:Allow, 0:Drop
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPLeaveRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *allow_leave)
{
    ret_t   retVal;
    l2sw_uint32  value;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 8)
    {
    /* Allow Leave */
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_ALLOW_LEAVE_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_ALLOW_LEAVE_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    *allow_leave = value;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPMRPRX
 * Description:
 *      Set port-based Multicast Routing Protocol packet RX allowance
 * Input:
 *      port            - port number
 *      allow_mrp       - allowance of Multicast Routing Protocol packet RX, 1:Allow, 0:Drop
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPMRPRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 allow_mrp)
{
    ret_t   retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 8)
    {
    /* Allow Multicast Routing Protocol */
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_ALLOW_MRP_MASK, allow_mrp);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_ALLOW_MRP_MASK, allow_mrp);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPMRPRX
 * Description:
 *      Get port-based Multicast Routing Protocol packet RX allowance
 * Input:
 *      port            - port number
 * Output:
 *      allow_mrp       - allowance of Multicast Routing Protocol packet RX, 1:Allow, 0:Drop
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPMRPRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *allow_mrp)
{
    ret_t   retVal;
    l2sw_uint32  value;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    /* Allow Multicast Routing Protocol */
    if(port < 8)
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_ALLOW_MRP_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_ALLOW_MRP_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    *allow_mrp = value;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPMcDataRX
 * Description:
 *      Set port-based Multicast data packet RX allowance
 * Input:
 *      port            - port number
 *      allow_mcdata    - allowance of Multicast data packet RX, 1:Allow, 0:Drop
 * Output:
 *      none
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPMcDataRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 allow_mcdata)
{
    ret_t   retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    /* Allow Multicast Data */
    if(port < 8)
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_ALLOW_MC_DATA_MASK, allow_mcdata);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_ALLOW_MC_DATA_MASK, allow_mcdata);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPMcDataRX
 * Description:
 *      Get port-based Multicast data packet RX allowance
 * Input:
 *      port            - port number
 * Output:
 *      allow_mcdata    - allowance of Multicast data packet RX, 1:Allow, 0:Drop
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPMcDataRX(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *allow_mcdata)
{
    ret_t   retVal;
    l2sw_uint32  value;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    /* Allow Multicast data */
    if(port < 8)
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_ALLOW_MC_DATA_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_ALLOW_MC_DATA_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    *allow_mcdata = value;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPv1Opeartion
 * Description:
 *      Set port-based IGMPv1 Control packet action
 * Input:
 *      port            - port number
 *      igmpv1_op       - IGMPv1 control packet action
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPv1Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 igmpv1_op)
{
    ret_t   retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(igmpv1_op >= PROTOCOL_OP_END)
        return L2SW_ERR_INPUT;

    /* IGMPv1 operation */
    if(port < 8)
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_IGMPV1_OP_MASK, igmpv1_op);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_IGMPV1_OP_MASK, igmpv1_op);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPv1Opeartion
 * Description:
 *      Get port-based IGMPv1 Control packet action
 * Input:
 *      port            - port number
 * Output:
 *      igmpv1_op       - IGMPv1 control packet action
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPv1Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *igmpv1_op)
{
    ret_t   retVal;
    l2sw_uint32  value;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    /* IGMPv1 operation */
    if(port < 8)
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_IGMPV1_OP_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_IGMPV1_OP_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    *igmpv1_op = value;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPv2Opeartion
 * Description:
 *      Set port-based IGMPv2 Control packet action
 * Input:
 *      port            - port number
 *      igmpv2_op       - IGMPv2 control packet action
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPv2Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 igmpv2_op)
{
    ret_t   retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(igmpv2_op >= PROTOCOL_OP_END)
        return L2SW_ERR_INPUT;

    /* IGMPv2 operation */
    if(port < 8)
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_IGMPV2_OP_MASK, igmpv2_op);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_IGMPV2_OP_MASK, igmpv2_op);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPv2Opeartion
 * Description:
 *      Get port-based IGMPv2 Control packet action
 * Input:
 *      port            - port number
 * Output:
 *      igmpv2_op       - IGMPv2 control packet action
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPv2Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *igmpv2_op)
{
    ret_t   retVal;
    l2sw_uint32  value;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    /* IGMPv2 operation */
    if(port < 8)
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_IGMPV2_OP_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_IGMPV2_OP_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    *igmpv2_op = value;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPv3Opeartion
 * Description:
 *      Set port-based IGMPv3 Control packet action
 * Input:
 *      port            - port number
 *      igmpv3_op       - IGMPv3 control packet action
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPv3Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 igmpv3_op)
{
    ret_t   retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(igmpv3_op >= PROTOCOL_OP_END)
        return L2SW_ERR_INPUT;

    /* IGMPv3 operation */
    if(port < 8)
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_IGMPV3_OP_MASK, igmpv3_op);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_IGMPV3_OP_MASK, igmpv3_op);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPv3Opeartion
 * Description:
 *      Get port-based IGMPv3 Control packet action
 * Input:
 *      port            - port number
 * Output:
 *      igmpv3_op       - IGMPv3 control packet action
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPv3Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *igmpv3_op)
{
    ret_t   retVal;
    l2sw_uint32  value;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    /* IGMPv3 operation */
    if(port < 8)
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_IGMPV3_OP_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_IGMPV3_OP_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    *igmpv3_op = value;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicMLDv1Opeartion
 * Description:
 *      Set port-based MLDv1 Control packet action
 * Input:
 *      port            - port number
 *      mldv1_op        - MLDv1 control packet action
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicMLDv1Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 mldv1_op)
{
    ret_t   retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(mldv1_op >= PROTOCOL_OP_END)
        return L2SW_ERR_INPUT;

    /* MLDv1 operation */
    if(port < 8)
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_MLDv1_OP_MASK, mldv1_op);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_MLDv1_OP_MASK, mldv1_op);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicMLDv1Opeartion
 * Description:
 *      Get port-based MLDv1 Control packet action
 * Input:
 *      port            - port number
 * Output:
 *      mldv1_op        - MLDv1 control packet action
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicMLDv1Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *mldv1_op)
{
    ret_t   retVal;
    l2sw_uint32  value;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    /* MLDv1 operation */
    if(port < 8)
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_MLDv1_OP_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_MLDv1_OP_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    *mldv1_op = value;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicMLDv2Opeartion
 * Description:
 *      Set port-based MLDv2 Control packet action
 * Input:
 *      port            - port number
 *      mldv2_op        - MLDv2 control packet action
 * Output:
 *      none
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicMLDv2Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 mldv2_op)
{
    ret_t   retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(mldv2_op >= PROTOCOL_OP_END)
        return L2SW_ERR_INPUT;

    /* MLDv2 operation */
    if(port < 8)
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_MLDv2_OP_MASK, mldv2_op);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_MLDv2_OP_MASK, mldv2_op);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicMLDv2Opeartion
 * Description:
 *      Get port-based MLDv2 Control packet action
 * Input:
 *      port            - port number
 * Output:
 *      mldv2_op        - MLDv2 control packet action
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_PORT_ID  - Error PORT ID
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicMLDv2Opeartion(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *mldv2_op)
{
    ret_t   retVal;
    l2sw_uint32  value;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    /* MLDv2 operation */
    if(port < 8)
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT0_CONTROL + port, L2SW_IGMP_PORT0_CONTROL_MLDv2_OP_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT8_CONTROL + port - 8, L2SW_IGMP_PORT0_CONTROL_MLDv2_OP_MASK, &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    *mldv2_op = value;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPPortMAXGroup
 * Description:
 *      Set per-port Max group number
 * Input:
 *      port        - Physical port number (0~7)
 *      max_group   - max IGMP group
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPPortMAXGroup(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 max_group)
{
    ret_t retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(max_group > L2SW_IGMP_MAX_GOUP)
        return L2SW_ERR_OUT_OF_RANGE;

    if(port < 8)
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT01_MAX_GROUP + (port/2), L2SW_PORT0_MAX_GROUP_MASK << (L2SW_PORT1_MAX_GROUP_OFFSET * (port%2)), max_group);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_PORT89_MAX_GROUP + (port/2), L2SW_PORT0_MAX_GROUP_MASK << (L2SW_PORT1_MAX_GROUP_OFFSET * (port%2)), max_group);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicIGMPPortMAXGroup
 * Description:
 *      Get per-port Max group number
 * Input:
 *      port        - Physical port number (0~7)
 *      max_group   - max IGMP group
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPPortMAXGroup(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *max_group)
{
    ret_t   retVal;
    l2sw_uint32  value;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 8)
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT01_MAX_GROUP + (port/2), L2SW_PORT0_MAX_GROUP_MASK << (L2SW_PORT1_MAX_GROUP_OFFSET * (port%2)), &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT89_MAX_GROUP + (port/2), L2SW_PORT0_MAX_GROUP_MASK << (L2SW_PORT1_MAX_GROUP_OFFSET * (port%2)), &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    *max_group = value;
    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicIGMPPortCurrentGroup
 * Description:
 *      Get per-port current group number
 * Input:
 *      port            - Physical port number (0~7)
 *      current_group   - current IGMP group
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPPortCurrentGroup(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *current_group)
{
    ret_t   retVal;
    l2sw_uint32  value;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 8)
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT01_CURRENT_GROUP + (port/2), L2SW_PORT0_CURRENT_GROUP_MASK << (L2SW_PORT1_CURRENT_GROUP_OFFSET * (port%2)), &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_PORT89_CURRENT_GROUP + ((port - 8)/2), L2SW_PORT0_CURRENT_GROUP_MASK << (L2SW_PORT1_CURRENT_GROUP_OFFSET * (port%2)), &value);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    *current_group = value;
    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicIGMPGroup
 * Description:
 *      Get IGMP group
 * Input:
 *      idx     - Group index (0~255)
 *      valid   - valid bit
 *      grp     - IGMP group
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - Group index is out of range
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPGroup(l2sw_uint8 units, l2sw_uint32 idx, l2sw_uint32 *valid, l2sw_igmpgroup *grp)
{
    ret_t   retVal;
    l2sw_uint32  regAddr, regData;
    l2sw_uint32  i;
    l2sw_uint32  groupIl2swo = 0;

    if(idx > L2SW_IGMP_MAX_GOUP)
        return L2SW_ERR_OUT_OF_RANGE;

    /* Write ACS_ADR register for data bits */
    regAddr = L2SW_TABLE_ACCESS_ADDR_REG;
    regData = idx;
    retVal = l2sw_setAsicReg(units, regAddr, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /* Write ACS_CMD register */
    regAddr = L2SW_TABLE_ACCESS_CTRL_REG;
    regData = L2SW_TABLE_ACCESS_REG_DATA(TB_OP_READ, TB_TARGET_IGMP_GROUP);
    retVal = l2sw_setAsicRegBits(units, regAddr, L2SW_TABLE_TYPE_MASK | L2SW_COMMAND_TYPE_MASK, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /* Read Data Bits */
    regAddr = L2SW_TABLE_ACCESS_RDDATA_BASE;
    for(i = 0 ;i <= 1; i++)
    {
        retVal = l2sw_getAsicReg(units, regAddr, &regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        groupIl2swo |= ((regData & 0xFFFF) << (i * 16));
        regAddr ++;
    }

    grp->p0_timer = groupIl2swo & 0x00000007;
    grp->p1_timer = (groupIl2swo >> 3) & 0x00000007;
    grp->p2_timer = (groupIl2swo >> 6) & 0x00000007;
    grp->p3_timer = (groupIl2swo >> 9) & 0x00000007;
    grp->p4_timer = (groupIl2swo >> 12) & 0x00000007;
    grp->p5_timer = (groupIl2swo >> 15) & 0x00000007;
    grp->p6_timer = (groupIl2swo >> 18) & 0x00000007;
    grp->p7_timer = (groupIl2swo >> 21) & 0x00000007;
    grp->report_supp_flag = (groupIl2swo >> 24) & 0x00000001;
    grp->p8_timer = (groupIl2swo >> 25) & 0x00000007;
    grp->p9_timer = (groupIl2swo >> 28) & 0x00000007;
    grp->p10_timer = (groupIl2swo >> 31) & 0x00000001;

    regAddr = L2SW_TABLE_ACCESS_RDDATA_BASE + 2;
    retVal = l2sw_getAsicReg(units, regAddr, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    grp->p10_timer |= (regData & 0x00000003) << 1;

    /* Valid bit */
    retVal = l2sw_getAsicReg(units, L2SW_IGMP_GROUP_USAGE_REG(idx), &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *valid = ((regData & (0x0001 << (idx %16))) != 0) ? 1 : 0;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicIpMulticastPortIsoLeaky
 * Description:
 *      Set IP multicast Port Isolation leaky
 * Input:
 *      port        - Physical port number (0~7)
 *      enabled     - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsicIpMulticastPortIsoLeaky(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled)
{
    ret_t   retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    retVal = l2sw_setAsicRegBits(units, L2SW_IPMCAST_PORTISO_LEAKY_REG, (0x0001 << port), enabled);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIpMulticastPortIsoLeaky
 * Description:
 *      Get IP multicast Port Isolation leaky
 * Input:
 *      port        - Physical port number (0~7)
 *      enabled     - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIpMulticastPortIsoLeaky(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *enabled)
{
    ret_t   retVal;
    l2sw_uint32  regData;

    retVal = l2sw_getAsicRegBits(units, L2SW_IPMCAST_PORTISO_LEAKY_REG, (0x0001 << port), &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *enabled = regData;
    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPReportLeaveFlood
 * Description:
 *      Set IGMP/MLD Report/Leave flood
 * Input:
 *      flood   - 0: Reserved, 1: flooding to router ports, 2: flooding to all ports, 3: flooding to router port or to all ports if there is no router port
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPReportLeaveFlood(l2sw_uint8 units, l2sw_uint32 flood)
{
    ret_t   retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_IGMP_MLD_CFG3, L2SW_REPORT_LEAVE_FORWARD_MASK, flood);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPReportLeaveFlood
 * Description:
 *      Get IGMP/MLD Report/Leave flood
 * Input:
 *      None
 * Output:
 *      pflood  - 0: Reserved, 1: flooding to router ports, 2: flooding to all ports, 3: flooding to router port or to all ports if there is no router port
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPReportLeaveFlood(l2sw_uint8 units, l2sw_uint32 *pFlood)
{
    ret_t   retVal;
    l2sw_uint32  regData;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_IGMP_MLD_CFG3, L2SW_REPORT_LEAVE_FORWARD_MASK, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *pFlood = regData;
    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPDropLeaveZero
 * Description:
 *      Set the function of droppping Leave packet with group IP = 0.0.0.0
 * Input:
 *      drop    - 1: Drop, 0:Bypass
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPDropLeaveZero(l2sw_uint8 units, l2sw_uint32 drop)
{
    ret_t   retVal;

    retVal = l2sw_setAsicRegBit(units, L2SW_REG_IGMP_MLD_CFG1, L2SW_DROP_LEAVE_ZERO_OFFSET, drop);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPDropLeaveZero
 * Description:
 *      Get the function of droppping Leave packet with group IP = 0.0.0.0
 * Input:
 *      None
 * Output:
 *      pDrop    - 1: Drop, 0:Bypass
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPDropLeaveZero(l2sw_uint8 units, l2sw_uint32 *pDrop)
{
    ret_t   retVal;
    l2sw_uint32  regData;

    retVal = l2sw_getAsicRegBit(units, L2SW_REG_IGMP_MLD_CFG1, L2SW_DROP_LEAVE_ZERO_OFFSET, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *pDrop = regData;
    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPBypassStormCTRL
 * Description:
 *      Set the function of bypass strom control for IGMP/MLD packet
 * Input:
 *      bypass    - 1: Bypass, 0:not bypass
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPBypassStormCTRL(l2sw_uint8 units, l2sw_uint32 bypass)
{
    ret_t   retVal;

    retVal = l2sw_setAsicRegBit(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_IGMP_MLD_DISCARD_STORM_FILTER_OFFSET, bypass);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPBypassStormCTRL
 * Description:
 *      Set the function of bypass strom control for IGMP/MLD packet
 * Input:
 *      None
 * Output:
 *      pBypass    - 1: Bypass, 0:not bypass
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPBypassStormCTRL(l2sw_uint8 units, l2sw_uint32 *pBypass)
{
    ret_t   retVal;
    l2sw_uint32  regData;

    retVal = l2sw_getAsicRegBit(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_IGMP_MLD_DISCARD_STORM_FILTER_OFFSET, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *pBypass = regData;
    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPIsoLeaky
 * Description:
 *      Set Port Isolation leaky for IGMP/MLD packet
 * Input:
 *      leaky    - 1: Leaky, 0:not leaky
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPIsoLeaky(l2sw_uint8 units, l2sw_uint32 leaky)
{
    ret_t   retVal;

    retVal = l2sw_setAsicRegBit(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_IGMP_MLD_PORTISO_LEAKY_OFFSET, leaky);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPIsoLeaky
 * Description:
 *      Get Port Isolation leaky for IGMP/MLD packet
 * Input:
 *      Noen
 * Output:
 *      pLeaky    - 1: Leaky, 0:not leaky
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPIsoLeaky(l2sw_uint8 units, l2sw_uint32 *pLeaky)
{
    ret_t   retVal;
    l2sw_uint32  regData;

    retVal = l2sw_getAsicRegBit(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_IGMP_MLD_PORTISO_LEAKY_OFFSET, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *pLeaky = regData;
    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPVLANLeaky
 * Description:
 *      Set VLAN leaky for IGMP/MLD packet
 * Input:
 *      leaky    - 1: Leaky, 0:not leaky
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPVLANLeaky(l2sw_uint8 units, l2sw_uint32 leaky)
{
    ret_t   retVal;

    retVal = l2sw_setAsicRegBit(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_IGMP_MLD_VLAN_LEAKY_OFFSET, leaky);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPVLANLeaky
 * Description:
 *      Get VLAN leaky for IGMP/MLD packet
 * Input:
 *      Noen
 * Output:
 *      pLeaky    - 1: Leaky, 0:not leaky
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPVLANLeaky(l2sw_uint8 units, l2sw_uint32 *pLeaky)
{
    ret_t   retVal;
    l2sw_uint32  regData;

    retVal = l2sw_getAsicRegBit(units, L2SW_REG_IGMP_MLD_CFG0, L2SW_IGMP_MLD_VLAN_LEAKY_OFFSET, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *pLeaky = regData;
    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicIGMPBypassGroup
 * Description:
 *      Set IGMP/MLD Bypass group
 * Input:
 *      bypassType  - Bypass type
 *      enabled     - enabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicIGMPBypassGroup(l2sw_uint8 units, l2sw_uint32 bypassType, l2sw_uint32 enabled)
{
    ret_t   retVal;
    l2sw_uint32 offset;

    switch(bypassType)
    {
        case BYPASS_224_0_0_X:
            offset = L2SW_IGMP_MLD_IP4_BYPASS_224_0_0_OFFSET;
            break;
        case BYPASS_224_0_1_X:
            offset = L2SW_IGMP_MLD_IP4_BYPASS_224_0_1_OFFSET;
            break;
        case BYPASS_239_255_255_X:
            offset = L2SW_IGMP_MLD_IP4_BYPASS_239_255_255_OFFSET;
            break;
        case BYPASS_IPV6_00XX:
            offset = L2SW_IGMP_MLD_IP6_BYPASS_OFFSET;
            break;
        default:
            return L2SW_ERR_INPUT;
    }

    retVal = l2sw_setAsicRegBit(units, L2SW_REG_IGMP_MLD_CFG3, offset, enabled);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicIGMPBypassGroup
 * Description:
 *      Get IGMP/MLD Bypass group
 * Input:
 *      bypassType  - Bypass type
 * Output:
 *      pEnabled    - enabled
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicIGMPBypassGroup(l2sw_uint8 units, l2sw_uint32 bypassType, l2sw_uint32 *pEnabled)
{
    ret_t   retVal;
    l2sw_uint32 offset;

    switch(bypassType)
    {
        case BYPASS_224_0_0_X:
            offset = L2SW_IGMP_MLD_IP4_BYPASS_224_0_0_OFFSET;
            break;
        case BYPASS_224_0_1_X:
            offset = L2SW_IGMP_MLD_IP4_BYPASS_224_0_1_OFFSET;
            break;
        case BYPASS_239_255_255_X:
            offset = L2SW_IGMP_MLD_IP4_BYPASS_239_255_255_OFFSET;
            break;
        case BYPASS_IPV6_00XX:
            offset = L2SW_IGMP_MLD_IP6_BYPASS_OFFSET;
            break;
        default:
            return L2SW_ERR_INPUT;
    }

    retVal = l2sw_getAsicRegBit(units, L2SW_REG_IGMP_MLD_CFG3, offset, pEnabled);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

