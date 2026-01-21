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

#include "l2sw_asicdrv_rma.h"
/* Function Name:
 *      l2sw_setAsicRma
 * Description:
 *      Set reserved multicast address for CPU trapping
 * Input:
 *      index     - reserved multicast LSB byte, 0x00~0x2F is available value
 *      pRmacfg     - type of RMA for trapping frame type setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK         - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_RMA_ADDR - Invalid RMA address index
 * Note:
 *      None
 */
ret_t l2sw_setAsicRma(l2sw_uint8 units, l2sw_uint32 index, l2sw_rma_t* pRmacfg)
{
    l2sw_uint32 regData = 0;
#ifndef USE_L2SW_SDK
    ret_t retVal;
#endif

    if(index > L2SW_RMAMAX)
        return L2SW_ERR_RMA_ADDR;

    regData |= (pRmacfg->portiso_leaky & 0x0001);
    regData |= ((pRmacfg->vlan_leaky & 0x0001) << 1);
    regData |= ((pRmacfg->keep_format & 0x0001) << 2);
    regData |= ((pRmacfg->trap_priority & 0x0007) << 3);
    regData |= ((pRmacfg->discard_storm_filter & 0x0001) << 6);
    regData |= ((pRmacfg->operation & 0x0003) << 7);

    if( (index >= 0x4 && index <= 0x7) || (index >= 0x9 && index <= 0x0C) || (0x0F == index))
        index = 0x04;
    else if((index >= 0x13 && index <= 0x17) || (0x19 == index) || (index >= 0x1B && index <= 0x1f))
        index = 0x13;
    else if(index >= 0x22 && index <= 0x2F)
        index = 0x22;

#if defined(USE_L2SW_SDK)

#else
    retVal = l2sw_setAsicRegBits(units, L2SW_REG_RMA_CTRL00, L2SW_TRAP_PRIORITY_MASK, pRmacfg->trap_priority);
    if(retVal != L2SW_ERR_OK)
        return retVal;
#endif

    return l2sw_setAsicReg(units, L2SW_REG_RMA_CTRL00+index, regData);
}
/* Function Name:
 *      l2sw_getAsicRma
 * Description:
 *      Get reserved multicast address for CPU trapping
 * Input:
 *      index     - reserved multicast LSB byte, 0x00~0x2F is available value
 *      rmacfg     - type of RMA for trapping frame type setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK         - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_RMA_ADDR - Invalid RMA address index
 * Note:
 *      None
 */
ret_t l2sw_getAsicRma(l2sw_uint8 units, l2sw_uint32 index, l2sw_rma_t* pRmacfg)
{
    ret_t retVal;
    l2sw_uint32 regData;

    if(index > L2SW_RMAMAX)
        return L2SW_ERR_RMA_ADDR;

    if( (index >= 0x4 && index <= 0x7) || (index >= 0x9 && index <= 0x0C) || (0x0F == index))
        index = 0x04;
    else if((index >= 0x13 && index <= 0x17) || (0x19 == index) || (index >= 0x1B && index <= 0x1f))
        index = 0x13;
    else if(index >= 0x22 && index <= 0x2F)
        index = 0x22;

    retVal = l2sw_getAsicReg(units, L2SW_REG_RMA_CTRL00+index, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    pRmacfg->operation = ((regData >> 7) & 0x0003);
    pRmacfg->discard_storm_filter = ((regData >> 6) & 0x0001);
    pRmacfg->trap_priority = ((regData >> 3) & 0x0007);
    pRmacfg->keep_format = ((regData >> 2) & 0x0001);
    pRmacfg->vlan_leaky = ((regData >> 1) & 0x0001);
    pRmacfg->portiso_leaky = (regData & 0x0001);

#if defined(USE_L2SW_SDK)

#else
    retVal = l2sw_getAsicRegBits(units, L2SW_REG_RMA_CTRL00, L2SW_TRAP_PRIORITY_MASK, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    pRmacfg->trap_priority = regData;
#endif

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicRmaCdp
 * Description:
 *      Set CDP(Cisco Discovery Protocol) for CPU trapping
 * Input:
 *      pRmacfg     - type of RMA for trapping frame type setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK         - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_RMA_ADDR - Invalid RMA address index
 * Note:
 *      None
 */
ret_t l2sw_setAsicRmaCdp(l2sw_uint8 units, l2sw_rma_t* pRmacfg)
{
    l2sw_uint32 regData = 0;
#ifndef USE_L2SW_SDK
	ret_t retVal;
#endif
    if(pRmacfg->operation >= RMAOP_END)
        return L2SW_ERR_RMA_ACTION;

    if(pRmacfg->trap_priority > L2SW_PRIMAX)
        return L2SW_ERR_QOS_INT_PRIORITY;

    regData |= (pRmacfg->portiso_leaky & 0x0001);
    regData |= ((pRmacfg->vlan_leaky & 0x0001) << 1);
    regData |= ((pRmacfg->keep_format & 0x0001) << 2);
    regData |= ((pRmacfg->trap_priority & 0x0007) << 3);
    regData |= ((pRmacfg->discard_storm_filter & 0x0001) << 6);
    regData |= ((pRmacfg->operation & 0x0003) << 7);

#if defined(USE_L2SW_SDK)

#else
    retVal = l2sw_setAsicRegBits(units, L2SW_REG_RMA_CTRL00, L2SW_TRAP_PRIORITY_MASK, pRmacfg->trap_priority);
    if(retVal != L2SW_ERR_OK)
        return retVal;
#endif

    return l2sw_setAsicReg(units, L2SW_REG_RMA_CTRL_CDP, regData);
}
/* Function Name:
 *      l2sw_getAsicRmaCdp
 * Description:
 *      Get CDP(Cisco Discovery Protocol) for CPU trapping
 * Input:
 *      None
 * Output:
 *      pRmacfg     - type of RMA for trapping frame type setting
 * Return:
 *      L2SW_ERR_OK         - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_RMA_ADDR - Invalid RMA address index
 * Note:
 *      None
 */
ret_t l2sw_getAsicRmaCdp(l2sw_uint8 units, l2sw_rma_t* pRmacfg)
{
    ret_t retVal;
    l2sw_uint32 regData;

    retVal = l2sw_getAsicReg(units, L2SW_REG_RMA_CTRL_CDP, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    pRmacfg->operation = ((regData >> 7) & 0x0003);
    pRmacfg->discard_storm_filter = ((regData >> 6) & 0x0001);
    pRmacfg->trap_priority = ((regData >> 3) & 0x0007);
    pRmacfg->keep_format = ((regData >> 2) & 0x0001);
    pRmacfg->vlan_leaky = ((regData >> 1) & 0x0001);
    pRmacfg->portiso_leaky = (regData & 0x0001);

#if defined(USE_L2SW_SDK)

#else
    retVal = l2sw_getAsicRegBits(units, L2SW_REG_RMA_CTRL00, L2SW_TRAP_PRIORITY_MASK, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    pRmacfg->trap_priority = regData;
#endif

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicRmaCsstp
 * Description:
 *      Set CSSTP(Cisco Shared Spanning Tree Protocol) for CPU trapping
 * Input:
 *      pRmacfg     - type of RMA for trapping frame type setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK         - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_RMA_ADDR - Invalid RMA address index
 * Note:
 *      None
 */
ret_t l2sw_setAsicRmaCsstp(l2sw_uint8 units, l2sw_rma_t* pRmacfg)
{
    l2sw_uint32 regData = 0;
#ifndef USE_L2SW_SDK
	ret_t retVal;
#endif
    if(pRmacfg->operation >= RMAOP_END)
        return L2SW_ERR_RMA_ACTION;

    if(pRmacfg->trap_priority > L2SW_PRIMAX)
        return L2SW_ERR_QOS_INT_PRIORITY;

    regData |= (pRmacfg->portiso_leaky & 0x0001);
    regData |= ((pRmacfg->vlan_leaky & 0x0001) << 1);
    regData |= ((pRmacfg->keep_format & 0x0001) << 2);
    regData |= ((pRmacfg->trap_priority & 0x0007) << 3);
    regData |= ((pRmacfg->discard_storm_filter & 0x0001) << 6);
    regData |= ((pRmacfg->operation & 0x0003) << 7);

#if defined(USE_L2SW_SDK)

#else
    retVal = l2sw_setAsicRegBits(units, L2SW_REG_RMA_CTRL00, L2SW_TRAP_PRIORITY_MASK, pRmacfg->trap_priority);
    if(retVal != L2SW_ERR_OK)
        return retVal;
#endif

    return l2sw_setAsicReg(units, L2SW_REG_RMA_CTRL_CSSTP, regData);
}
/* Function Name:
 *      l2sw_getAsicRmaCsstp
 * Description:
 *      Get CSSTP(Cisco Shared Spanning Tree Protocol) for CPU trapping
 * Input:
 *      None
 * Output:
 *      pRmacfg     - type of RMA for trapping frame type setting
 * Return:
 *      L2SW_ERR_OK         - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_RMA_ADDR - Invalid RMA address index
 * Note:
 *      None
 */
ret_t l2sw_getAsicRmaCsstp(l2sw_uint8 units, l2sw_rma_t* pRmacfg)
{
    ret_t retVal;
    l2sw_uint32 regData;

    retVal = l2sw_getAsicReg(units, L2SW_REG_RMA_CTRL_CSSTP, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    pRmacfg->operation = ((regData >> 7) & 0x0003);
    pRmacfg->discard_storm_filter = ((regData >> 6) & 0x0001);
    pRmacfg->trap_priority = ((regData >> 3) & 0x0007);
    pRmacfg->keep_format = ((regData >> 2) & 0x0001);
    pRmacfg->vlan_leaky = ((regData >> 1) & 0x0001);
    pRmacfg->portiso_leaky = (regData & 0x0001);

#if defined(USE_L2SW_SDK)

#else
    retVal = l2sw_getAsicRegBits(units, L2SW_REG_RMA_CTRL00, L2SW_TRAP_PRIORITY_MASK, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    pRmacfg->trap_priority = regData;
#endif

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicRmaLldp
 * Description:
 *      Set LLDP for CPU trapping
 * Input:
 *      pRmacfg     - type of RMA for trapping frame type setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK         - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_RMA_ADDR - Invalid RMA address index
 * Note:
 *      None
 */
ret_t l2sw_setAsicRmaLldp(l2sw_uint8 units, l2sw_uint32 enabled, l2sw_rma_t* pRmacfg)
{
    l2sw_uint32 regData = 0;
    ret_t retVal;

    if(enabled > 1)
        return L2SW_ERR_ENABLE;

    if(pRmacfg->operation >= RMAOP_END)
        return L2SW_ERR_RMA_ACTION;

    if(pRmacfg->trap_priority > L2SW_PRIMAX)
        return L2SW_ERR_QOS_INT_PRIORITY;

    retVal = l2sw_setAsicRegBit(units, L2SW_REG_RMA_LLDP_EN, L2SW_RMA_LLDP_EN_OFFSET,enabled);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    regData |= (pRmacfg->portiso_leaky & 0x0001);
    regData |= ((pRmacfg->vlan_leaky & 0x0001) << 1);
    regData |= ((pRmacfg->keep_format & 0x0001) << 2);
    regData |= ((pRmacfg->trap_priority & 0x0007) << 3);
    regData |= ((pRmacfg->discard_storm_filter & 0x0001) << 6);
    regData |= ((pRmacfg->operation & 0x0003) << 7);

#if defined(USE_L2SW_SDK)

#else
    retVal = l2sw_setAsicRegBits(units, L2SW_REG_RMA_CTRL00, L2SW_TRAP_PRIORITY_MASK, pRmacfg->trap_priority);
    if(retVal != L2SW_ERR_OK)
        return retVal;
#endif

    return l2sw_setAsicReg(units, L2SW_REG_RMA_CTRL_LLDP, regData);
}
/* Function Name:
 *      l2sw_getAsicRmaLldp
 * Description:
 *      Get LLDP for CPU trapping
 * Input:
 *      None
 * Output:
 *      pRmacfg     - type of RMA for trapping frame type setting
 * Return:
 *      L2SW_ERR_OK         - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_RMA_ADDR - Invalid RMA address index
 * Note:
 *      None
 */
ret_t l2sw_getAsicRmaLldp(l2sw_uint8 units, l2sw_uint32 *pEnabled, l2sw_rma_t* pRmacfg)
{
    ret_t retVal;
    l2sw_uint32 regData;

    retVal = l2sw_getAsicRegBit(units, L2SW_REG_RMA_LLDP_EN, L2SW_RMA_LLDP_EN_OFFSET,pEnabled);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicReg(units, L2SW_REG_RMA_CTRL_LLDP, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    pRmacfg->operation = ((regData >> 7) & 0x0003);
    pRmacfg->discard_storm_filter = ((regData >> 6) & 0x0001);
    pRmacfg->trap_priority = ((regData >> 3) & 0x0007);
    pRmacfg->keep_format = ((regData >> 2) & 0x0001);
    pRmacfg->vlan_leaky = ((regData >> 1) & 0x0001);
    pRmacfg->portiso_leaky = (regData & 0x0001);

#if defined(USE_L2SW_SDK)

#else
    retVal = l2sw_getAsicRegBits(units, L2SW_REG_RMA_CTRL00, L2SW_TRAP_PRIORITY_MASK, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    pRmacfg->trap_priority = regData;
#endif

    return L2SW_ERR_OK;
}
