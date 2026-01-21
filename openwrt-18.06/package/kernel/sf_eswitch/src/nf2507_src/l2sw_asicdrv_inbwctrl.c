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

#include "l2sw_asicdrv_inbwctrl.h"
/* Function Name:
 *      l2sw_setAsicPortIngressBandwidth
 * Description:
 *      Set per-port total ingress bandwidth
 * Input:
 *      port        - Physical port number (0~7)
 *      bandwidth   - The total ingress bandwidth (unit: 8Kbps), 0x1FFFF:disable
 *      preifg      - Include preamble and IFG, 0:Exclude, 1:Include
 *      enableFC    - Action when input rate exceeds. 0: Drop   1: Flow Control
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
ret_t l2sw_setAsicPortIngressBandwidth(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 bandwidth, l2sw_uint32 preifg, l2sw_uint32 enableFC)
{
    ret_t retVal;
    l2sw_uint32 regData;
    l2sw_uint32 regAddr;

    /* Invalid input parameter */
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    if(bandwidth > L2SW_QOS_GRANULARTY_MAX)
        return L2SW_ERR_OUT_OF_RANGE;

    regAddr = L2SW_INGRESSBW_PORT_RATE_LSB_REG(port);
    regData = bandwidth & L2SW_QOS_GRANULARTY_LSB_MASK;
    retVal = l2sw_setAsicReg(units, regAddr, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    regAddr += 1;
    regData = (bandwidth & L2SW_QOS_GRANULARTY_MSB_MASK) >> L2SW_QOS_GRANULARTY_MSB_OFFSET;
    retVal = l2sw_setAsicRegBits(units, regAddr, L2SW_INGRESSBW_PORT0_RATE_CTRL1_INGRESSBW_RATE16_MASK, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    regAddr = L2SW_PORT_MISC_CFG_REG(port);
    retVal = l2sw_setAsicRegBit(units, regAddr, L2SW_PORT0_MISC_CFG_INGRESSBW_IFG_OFFSET, preifg);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    regAddr = L2SW_PORT_MISC_CFG_REG(port);
    retVal = l2sw_setAsicRegBit(units, regAddr, L2SW_PORT0_MISC_CFG_INGRESSBW_FLOWCTRL_OFFSET, enableFC);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicPortIngressBandwidth
 * Description:
 *      Get per-port total ingress bandwidth
 * Input:
 *      port        - Physical port number (0~7)
 *      pBandwidth  - The total ingress bandwidth (unit: 8Kbps), 0x1FFFF:disable
 *      pPreifg         - Include preamble and IFG, 0:Exclude, 1:Include
 *      pEnableFC   - Action when input rate exceeds. 0: Drop   1: Flow Control
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortIngressBandwidth(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pBandwidth, l2sw_uint32* pPreifg, l2sw_uint32* pEnableFC)
{
    ret_t retVal;
    l2sw_uint32 regData;
    l2sw_uint32 regAddr;

    /* Invalid input parameter */
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    regAddr = L2SW_INGRESSBW_PORT_RATE_LSB_REG(port);
    retVal = l2sw_getAsicReg(units, regAddr, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *pBandwidth = regData;

    regAddr += 1;
    retVal = l2sw_getAsicRegBits(units, regAddr, L2SW_INGRESSBW_PORT0_RATE_CTRL1_INGRESSBW_RATE16_MASK, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *pBandwidth |= (regData << L2SW_QOS_GRANULARTY_MSB_OFFSET);

    regAddr = L2SW_PORT_MISC_CFG_REG(port);
    retVal = l2sw_getAsicRegBit(units, regAddr, L2SW_PORT0_MISC_CFG_INGRESSBW_IFG_OFFSET, pPreifg);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    regAddr = L2SW_PORT_MISC_CFG_REG(port);
    retVal = l2sw_getAsicRegBit(units, regAddr, L2SW_PORT0_MISC_CFG_INGRESSBW_FLOWCTRL_OFFSET, pEnableFC);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicPortIngressBandwidthBypass
 * Description:
 *      Set ingress bandwidth control bypasss 8899, RMA 01-80-C2-00-00-xx and IGMP
 * Input:
 *      enabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortIngressBandwidthBypass(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_SW_DUMMY0, L2SW_INGRESSBW_BYPASS_EN_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicPortIngressBandwidthBypass
 * Description:
 *      Set ingress bandwidth control bypasss 8899, RMA 01-80-C2-00-00-xx and IGMP
 * Input:
 *      pEnabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortIngressBandwidthBypass(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_SW_DUMMY0, L2SW_INGRESSBW_BYPASS_EN_OFFSET, pEnabled);
}

