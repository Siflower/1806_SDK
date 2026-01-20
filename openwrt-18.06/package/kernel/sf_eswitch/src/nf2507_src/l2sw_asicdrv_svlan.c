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

#include "l2sw_asicdrv_svlan.h"
#include "sal_string.h"

static void _l2sw_svlanConfStUser2Smi(l2sw_uint8 units,  l2sw_svlan_memconf_t *pUserSt, l2sw_uint16 *pSmiSt)
{
    pSmiSt[0] |= (pUserSt->vs_member & 0x00FF);
    pSmiSt[0] |= (pUserSt->vs_untag & 0x00FF) << 8;

    pSmiSt[1] |= (pUserSt->vs_fid_msti & 0x000F);
    pSmiSt[1] |= (pUserSt->vs_priority & 0x0007) << 4;
    pSmiSt[1] |= (pUserSt->vs_force_fid & 0x0001) << 7;

    pSmiSt[2] |= (pUserSt->vs_svid & 0x0FFF);
    pSmiSt[2] |= (pUserSt->vs_efiden & 0x0001) << 12;
    pSmiSt[2] |= (pUserSt->vs_efid & 0x0007) << 13;

    pSmiSt[3] |= ((pUserSt->vs_member & 0x0700) >> 8);
    pSmiSt[3] |= ((pUserSt->vs_untag & 0x0700) >> 8) << 3;
}

static void _l2sw_svlanConfStSmi2User(l2sw_uint8 units,  l2sw_svlan_memconf_t *pUserSt, l2sw_uint16 *pSmiSt)
{

    pUserSt->vs_member = (pSmiSt[0] & 0x00FF) | ((pSmiSt[3] & 0x0007) << 8);
    pUserSt->vs_untag = ((pSmiSt[0] & 0xFF00) >> 8) | (((pSmiSt[3] & 0x0038) >> 3) << 8);

    pUserSt->vs_fid_msti = (pSmiSt[1] & 0x000F);
    pUserSt->vs_priority = (pSmiSt[1] & 0x0070) >> 4;
    pUserSt->vs_force_fid = (pSmiSt[1] & 0x0080) >> 7;

    pUserSt->vs_svid = (pSmiSt[2] & 0x0FFF);
    pUserSt->vs_efiden = (pSmiSt[2] & 0x1000) >> 12;
    pUserSt->vs_efid = (pSmiSt[2] & 0xE000) >> 13;
}

static void _l2sw_svlanMc2sStUser2Smi(l2sw_uint8 units, l2sw_svlan_mc2s_t *pUserSt, l2sw_uint16 *pSmiSt)
{
    pSmiSt[0] |= (pUserSt->svidx & 0x003F);
    pSmiSt[0] |= (pUserSt->format & 0x0001) << 6;
    pSmiSt[0] |= (pUserSt->valid & 0x0001) << 7;

    pSmiSt[1] = (l2sw_uint16)(pUserSt->smask & 0x0000FFFF);
    pSmiSt[2] = (l2sw_uint16)((pUserSt->smask & 0xFFFF0000) >> 16);

    pSmiSt[3] = (l2sw_uint16)(pUserSt->sdata & 0x0000FFFF);
    pSmiSt[4] = (l2sw_uint16)((pUserSt->sdata & 0xFFFF0000) >> 16);
}

static void _l2sw_svlanMc2sStSmi2User(l2sw_uint8 units, l2sw_svlan_mc2s_t *pUserSt, l2sw_uint16 *pSmiSt)
{
    pUserSt->svidx = (pSmiSt[0] & 0x003F);
    pUserSt->format = (pSmiSt[0] & 0x0040) >> 6;
    pUserSt->valid = (pSmiSt[0] & 0x0080) >> 7;

    pUserSt->smask = pSmiSt[1] | (pSmiSt[2] << 16);
    pUserSt->sdata = pSmiSt[3] | (pSmiSt[4] << 16);
}

static void _l2sw_svlanSp2cStUser2Smi(l2sw_uint8 units, l2sw_svlan_s2c_t *pUserSt, l2sw_uint16 *pSmiSt)
{
    pSmiSt[0] |= (pUserSt->dstport & 0x0007);
    pSmiSt[0] |= (pUserSt->svidx & 0x003F) << 3;
    pSmiSt[0] |= ((pUserSt->dstport & 0x0008) >> 3) << 9;

    pSmiSt[1] |= (pUserSt->vid & 0x0FFF);
    pSmiSt[1] |= (pUserSt->valid & 0x0001) << 12;
}

static void _l2sw_svlanSp2cStSmi2User(l2sw_uint8 units, l2sw_svlan_s2c_t *pUserSt, l2sw_uint16 *pSmiSt)
{
    pUserSt->dstport = (((pSmiSt[0] & 0x0200) >> 9) << 3) | (pSmiSt[0] & 0x0007);
    pUserSt->svidx   = (pSmiSt[0] & 0x01F8) >> 3;
    pUserSt->vid     = (pSmiSt[1] & 0x0FFF);
    pUserSt->valid   = (pSmiSt[1] & 0x1000) >> 12;
}

/* Function Name:
 *      l2sw_setAsicSvlanUplinkPortMask
 * Description:
 *      Set uplink ports mask
 * Input:
 *      portMask    - Uplink port mask setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicSvlanUplinkPortMask(l2sw_uint8 units, l2sw_uint32 portMask)
{
    return l2sw_setAsicReg(units, L2SW_REG_SVLAN_UPLINK_PORTMASK, portMask);
}
/* Function Name:
 *      l2sw_getAsicSvlanUplinkPortMask
 * Description:
 *      Get uplink ports mask
 * Input:
 *      pPortmask   - Uplink port mask setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicSvlanUplinkPortMask(l2sw_uint8 units, l2sw_uint32* pPortmask)
{
    return l2sw_getAsicReg(units, L2SW_REG_SVLAN_UPLINK_PORTMASK, pPortmask);
}
/* Function Name:
 *      l2sw_setAsicSvlanTpid
 * Description:
 *      Set accepted S-VLAN ether type. The default ether type of S-VLAN is 0x88a8
 * Input:
 *      protocolType    - Ether type of S-tag frame parsing in uplink ports
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      Ether type of S-tag in 802.1ad is 0x88a8 and there are existed ether type 0x9100 and 0x9200
 *      for Q-in-Q SLAN design. User can set mathced ether type as service provider supported protocol
 */
ret_t l2sw_setAsicSvlanTpid(l2sw_uint8 units, l2sw_uint32 protocolType)
{
    return l2sw_setAsicReg(units, L2SW_REG_VS_TPID, protocolType);
}
/* Function Name:
 *      l2sw_getAsicReg
 * Description:
 *      Get accepted S-VLAN ether type. The default ether type of S-VLAN is 0x88a8
 * Input:
 *      pProtocolType   - Ether type of S-tag frame parsing in uplink ports
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicSvlanTpid(l2sw_uint8 units, l2sw_uint32* pProtocolType)
{
    return l2sw_getAsicReg(units, L2SW_REG_VS_TPID, pProtocolType);
}
/* Function Name:
 *      l2sw_setAsicSvlanPrioritySel
 * Description:
 *      Set SVLAN priority field setting
 * Input:
 *      priSel  - S-priority assignment method, 0:internal priority 1:C-tag priority 2:using Svlan member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_INPUT    - Invalid input parameter
 * Note:
 *      None
 */
ret_t l2sw_setAsicSvlanPrioritySel(l2sw_uint8 units, l2sw_uint32 priSel)
{
    if(priSel >= SPRISEL_END)
        return L2SW_ERR_QOS_INT_PRIORITY;

    return l2sw_setAsicRegBits(units, L2SW_REG_SVLAN_CFG, L2SW_VS_SPRISEL_MASK, priSel);
}
/* Function Name:
 *      l2sw_getAsicSvlanPrioritySel
 * Description:
 *      Get SVLAN priority field setting
 * Input:
 *      pPriSel     - S-priority assignment method, 0:internal priority 1:C-tag priority 2:using Svlan member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicSvlanPrioritySel(l2sw_uint8 units, l2sw_uint32* pPriSel)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_SVLAN_CFG, L2SW_VS_SPRISEL_MASK, pPriSel);
}
/* Function Name:
 *      l2sw_setAsicSvlanTrapPriority
 * Description:
 *      Set trap to CPU priority assignment
 * Input:
 *      priority    - Priority assignment
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority
 * Note:
 *      None
 */
ret_t l2sw_setAsicSvlanTrapPriority(l2sw_uint8 units, l2sw_uint32 priority)
{
    if(priority > L2SW_PRIMAX)
        return L2SW_ERR_QOS_INT_PRIORITY;

    return l2sw_setAsicRegBits(units, L2SW_REG_QOS_TRAP_PRIORITY0, L2SW_SVLAN_PRIOIRTY_MASK, priority);
}
/* Function Name:
 *      l2sw_getAsicSvlanTrapPriority
 * Description:
 *      Get trap to CPU priority assignment
 * Input:
 *      pPriority   - Priority assignment
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicSvlanTrapPriority(l2sw_uint8 units, l2sw_uint32* pPriority)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_QOS_TRAP_PRIORITY0, L2SW_SVLAN_PRIOIRTY_MASK, pPriority);
}
/* Function Name:
 *      l2sw_setAsicSvlanDefaultVlan
 * Description:
 *      Set default egress SVLAN
 * Input:
 *      port    - Physical port number (0~10)
 *      index   - index SVLAN member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number
 *      L2SW_ERR_SVLAN_ENTRY_INDEX    - Invalid SVLAN index parameter
 * Note:
 *      None
 */
ret_t l2sw_setAsicSvlanDefaultVlan(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 index)
{
    ret_t retVal = 0;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(index > L2SW_SVIDXMAX)
        return L2SW_ERR_SVLAN_ENTRY_INDEX;

    if(port < 8){
        if(port & 1)
            retVal = l2sw_setAsicRegBits(units, L2SW_REG_SVLAN_PORTBASED_SVIDX_CTRL0 + (port >> 1), L2SW_VS_PORT1_SVIDX_MASK,index);
        else
            retVal = l2sw_setAsicRegBits(units, L2SW_REG_SVLAN_PORTBASED_SVIDX_CTRL0 + (port >> 1), L2SW_VS_PORT0_SVIDX_MASK,index);
    }else{
        switch(port){
            case 8:
                retVal = l2sw_setAsicRegBits(units, L2SW_REG_SVLAN_PORTBASED_SVIDX_CTRL4, L2SW_VS_PORT8_SVIDX_MASK,index);
                break;

            case 9:
                retVal = l2sw_setAsicRegBits(units, L2SW_REG_SVLAN_PORTBASED_SVIDX_CTRL4, L2SW_VS_PORT9_SVIDX_MASK,index);
                break;

            case 10:
                retVal = l2sw_setAsicRegBits(units, L2SW_REG_SVLAN_PORTBASED_SVIDX_CTRL5, L2SW_SVLAN_PORTBASED_SVIDX_CTRL5_MASK,index);
                break;
        }
    }

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicSvlanDefaultVlan
 * Description:
 *      Get default egress SVLAN
 * Input:
 *      port    - Physical port number (0~7)
 *      pIndex  - index SVLAN member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicSvlanDefaultVlan(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pIndex)
{
    ret_t retVal = 0;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 8){
        if(port & 1)
            retVal = l2sw_getAsicRegBits(units, L2SW_REG_SVLAN_PORTBASED_SVIDX_CTRL0 + (port >> 1), L2SW_VS_PORT1_SVIDX_MASK,pIndex);
        else
            retVal = l2sw_getAsicRegBits(units, L2SW_REG_SVLAN_PORTBASED_SVIDX_CTRL0 + (port >> 1), L2SW_VS_PORT0_SVIDX_MASK,pIndex);
    }else{
        switch(port){
            case 8:
                retVal = l2sw_getAsicRegBits(units, L2SW_REG_SVLAN_PORTBASED_SVIDX_CTRL4, L2SW_VS_PORT8_SVIDX_MASK,pIndex);
                break;

            case 9:
                retVal = l2sw_getAsicRegBits(units, L2SW_REG_SVLAN_PORTBASED_SVIDX_CTRL4, L2SW_VS_PORT9_SVIDX_MASK,pIndex);
                break;

            case 10:
                retVal = l2sw_getAsicRegBits(units, L2SW_REG_SVLAN_PORTBASED_SVIDX_CTRL5, L2SW_SVLAN_PORTBASED_SVIDX_CTRL5_MASK,pIndex);
                break;
        }
    }

    return retVal;

}
/* Function Name:
 *      l2sw_setAsicSvlanIngressUntag
 * Description:
 *      Set action received un-Stag frame from unplink port
 * Input:
 *      mode        - 0:Drop 1:Trap 2:Assign SVLAN
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicSvlanIngressUntag(l2sw_uint8 units, l2sw_uint32 mode)
{
    return l2sw_setAsicRegBits(units, L2SW_REG_SVLAN_CFG, L2SW_VS_UNTAG_MASK, mode);
}
/* Function Name:
 *      l2sw_getAsicSvlanIngressUntag
 * Description:
 *      Get action received un-Stag frame from unplink port
 * Input:
 *      pMode       - 0:Drop 1:Trap 2:Assign SVLAN
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicSvlanIngressUntag(l2sw_uint8 units, l2sw_uint32* pMode)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_SVLAN_CFG, L2SW_VS_UNTAG_MASK, pMode);
}
/* Function Name:
 *      l2sw_setAsicSvlanIngressUnmatch
 * Description:
 *      Set action received unmatched Stag frame from unplink port
 * Input:
 *      mode        - 0:Drop 1:Trap 2:Assign SVLAN
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicSvlanIngressUnmatch(l2sw_uint8 units, l2sw_uint32 mode)
{
    return l2sw_setAsicRegBits(units, L2SW_REG_SVLAN_CFG, L2SW_VS_UNMAT_MASK, mode);
}
/* Function Name:
 *      l2sw_getAsicSvlanIngressUnmatch
 * Description:
 *      Get action received unmatched Stag frame from unplink port
 * Input:
 *      pMode       - 0:Drop 1:Trap 2:Assign SVLAN
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicSvlanIngressUnmatch(l2sw_uint8 units, l2sw_uint32* pMode)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_SVLAN_CFG, L2SW_VS_UNMAT_MASK, pMode);

}
/* Function Name:
 *      l2sw_setAsicSvlanEgressUnassign
 * Description:
 *      Set unplink stream without egress SVID action
 * Input:
 *      enabled     - 1:Trap egress unassigned frames to CPU, 0: Use SVLAN setup in VS_CPSVIDX as egress SVID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicSvlanEgressUnassign(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_SVLAN_CFG, L2SW_VS_UIFSEG_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicSvlanEgressUnassign
 * Description:
 *      Get unplink stream without egress SVID action
 * Input:
 *      pEnabled    - 1:Trap egress unassigned frames to CPU, 0: Use SVLAN setup in VS_CPSVIDX as egress SVID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicSvlanEgressUnassign(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_SVLAN_CFG, L2SW_VS_UIFSEG_OFFSET, pEnabled);
}


/* Function Name:
 *      l2sw_setAsicSvlanMemberConfiguration
 * Description:
 *      Set system 64 S-tag content
 * Input:
 *      index           - index of 64 s-tag configuration
 *      pSvlanMemCfg    - SVLAN member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_SVLAN_ENTRY_INDEX    - Invalid SVLAN index parameter
 * Note:
 *      None
 */
ret_t l2sw_setAsicSvlanMemberConfiguration(l2sw_uint8 units, l2sw_uint32 index, l2sw_svlan_memconf_t* pSvlanMemCfg)
{
    ret_t retVal;
    l2sw_uint32 regAddr = 0, regData;
    l2sw_uint16 *accessPtr;
    l2sw_uint32 i;
    l2sw_uint16 smiSvlanMemConf[L2SW_SVLAN_MEMCOL2SW_LEN];

    if(index > L2SW_SVIDXMAX)
        return L2SW_ERR_SVLAN_ENTRY_INDEX;

    MEMSET(smiSvlanMemConf, 0x00, sizeof(l2sw_uint16) * L2SW_SVLAN_MEMCOL2SW_LEN);
    _l2sw_svlanConfStUser2Smi(units, pSvlanMemCfg, smiSvlanMemConf);

    accessPtr = smiSvlanMemConf;

    regData = *accessPtr;
    for(i = 0; i < 3; i++)
    {
        retVal = l2sw_setAsicReg(units, L2SW_SVLAN_MEMBERCFG_BASE_REG(index) + i, regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        accessPtr ++;
        regData = *accessPtr;
    }

    if(index < 63)
        regAddr = L2SW_REG_SVLAN_MEMBERCFG0_CTRL4+index;
    else if(index == 63)
        regAddr = L2SW_REG_SVLAN_MEMBERCFG63_CTRL4;

    retVal = l2sw_setAsicReg(units, regAddr, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;

}
/* Function Name:
 *      l2sw_getAsicSvlanMemberConfiguration
 * Description:
 *      Get system 64 S-tag content
 * Input:
 *      index           - index of 64 s-tag configuration
 *      pSvlanMemCfg    - SVLAN member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_SVLAN_ENTRY_INDEX    - Invalid SVLAN index parameter
 * Note:
 *      None
 */
ret_t l2sw_getAsicSvlanMemberConfiguration(l2sw_uint8 units, l2sw_uint32 index,l2sw_svlan_memconf_t* pSvlanMemCfg)
{
    ret_t retVal;
    l2sw_uint32 regAddr = 0,regData;
    l2sw_uint16 *accessPtr;
    l2sw_uint32 i;
    l2sw_uint16 smiSvlanMemConf[L2SW_SVLAN_MEMCOL2SW_LEN];

    if(index > L2SW_SVIDXMAX)
        return L2SW_ERR_SVLAN_ENTRY_INDEX;

    MEMSET(smiSvlanMemConf, 0x00, sizeof(l2sw_uint16) * L2SW_SVLAN_MEMCOL2SW_LEN);

    accessPtr = smiSvlanMemConf;

    for(i = 0; i < 3; i++)
    {
        retVal = l2sw_getAsicReg(units, L2SW_SVLAN_MEMBERCFG_BASE_REG(index) + i, &regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        *accessPtr = regData;

        accessPtr ++;
    }

    if(index < 63)
        regAddr = L2SW_REG_SVLAN_MEMBERCFG0_CTRL4+index;
    else if(index == 63)
        regAddr = L2SW_REG_SVLAN_MEMBERCFG63_CTRL4;

    retVal = l2sw_getAsicReg(units, regAddr, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *accessPtr = regData;

    _l2sw_svlanConfStSmi2User(units, pSvlanMemCfg, smiSvlanMemConf);

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicSvlanC2SConf
 * Description:
 *      Set SVLAN C2S table
 * Input:
 *      index   - index of 128 Svlan C2S configuration
 *      evid    - Enhanced VID
 *      portmask    - available c2s port mask
 *      svidx   - index of 64 Svlan member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENTRY_INDEX  - Invalid entry index
 * Note:
 *      ASIC will check upstream's VID and assign related SVID to mathed packet
 */
ret_t l2sw_setAsicSvlanC2SConf(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 evid, l2sw_uint32 portmask, l2sw_uint32 svidx)
{
    ret_t retVal;

    if(index > L2SW_C2SIDXMAX)
        return L2SW_ERR_ENTRY_INDEX;

    retVal = l2sw_setAsicReg(units, L2SW_SVLAN_C2SCFG_BASE_REG(index), svidx);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicReg(units, L2SW_SVLAN_C2SCFG_BASE_REG(index) + 1, portmask);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicReg(units, L2SW_SVLAN_C2SCFG_BASE_REG(index) + 2, evid);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicSvlanC2SConf
 * Description:
 *      Get SVLAN C2S table
 * Input:
 *      index   - index of 128 Svlan C2S configuration
 *      pEvid   - Enhanced VID
 *      pPortmask   - available c2s port mask
 *      pSvidx  - index of 64 Svlan member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENTRY_INDEX  - Invalid entry index
 * Note:
 *      None
 */
ret_t l2sw_getAsicSvlanC2SConf(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pEvid, l2sw_uint32* pPortmask, l2sw_uint32* pSvidx)
{
    ret_t retVal;

    if(index > L2SW_C2SIDXMAX)
        return L2SW_ERR_ENTRY_INDEX;

    retVal = l2sw_getAsicReg(units, L2SW_SVLAN_C2SCFG_BASE_REG(index), pSvidx);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicReg(units, L2SW_SVLAN_C2SCFG_BASE_REG(index) + 1, pPortmask);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicReg(units, L2SW_SVLAN_C2SCFG_BASE_REG(index) + 2, pEvid);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicSvlanMC2SConf
 * Description:
 *      Set system MC2S content
 * Input:
 *      index           - index of 32 SVLAN 32 MC2S configuration
 *      pSvlanMc2sCfg   - SVLAN Multicast to SVLAN member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENTRY_INDEX  - Invalid entry index
 * Note:
 *      If upstream packet is L2 multicast or IPv4 multicast packet and DMAC/DIP is matched MC2S
 *      configuration, ASIC will assign egress SVID to the packet
 */
ret_t l2sw_setAsicSvlanMC2SConf(l2sw_uint8 units, l2sw_uint32 index,l2sw_svlan_mc2s_t* pSvlanMc2sCfg)
{
    ret_t retVal;
    l2sw_uint32 regData;
    l2sw_uint16 *accessPtr;
    l2sw_uint32 i;
    l2sw_uint16 smiSvlanMC2S[L2SW_SVLAN_MC2S_LEN];

    if(index > L2SW_MC2SIDXMAX)
        return L2SW_ERR_ENTRY_INDEX;

    MEMSET(smiSvlanMC2S, 0x00, sizeof(l2sw_uint16) * L2SW_SVLAN_MC2S_LEN);
    _l2sw_svlanMc2sStUser2Smi(units, pSvlanMc2sCfg, smiSvlanMC2S);

    accessPtr = smiSvlanMC2S;

    for(i = 0; i < 5; i++)
    {
        regData = *(accessPtr + i);
        retVal = l2sw_setAsicReg(units, L2SW_SVLAN_MCAST2S_ENTRY_BASE_REG(index) + i, regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicSvlanMC2SConf
 * Description:
 *      Get system MC2S content
 * Input:
 *      index           - index of 32 SVLAN 32 MC2S configuration
 *      pSvlanMc2sCfg   - SVLAN Multicast to SVLAN member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENTRY_INDEX  - Invalid entry index
 * Note:
 *      None
 */
ret_t l2sw_getAsicSvlanMC2SConf(l2sw_uint8 units, l2sw_uint32 index, l2sw_svlan_mc2s_t* pSvlanMc2sCfg)
{
    ret_t retVal;
    l2sw_uint32 regData;
    l2sw_uint16 *accessPtr;
    l2sw_uint32 i;
    l2sw_uint16 smiSvlanMC2S[L2SW_SVLAN_MC2S_LEN];

    if(index > L2SW_MC2SIDXMAX)
        return L2SW_ERR_ENTRY_INDEX;

    MEMSET(smiSvlanMC2S, 0x00, sizeof(l2sw_uint16) * L2SW_SVLAN_MC2S_LEN);

    accessPtr = smiSvlanMC2S;

    for(i = 0; i < 5; i++)
    {
        retVal = l2sw_getAsicReg(units, L2SW_SVLAN_MCAST2S_ENTRY_BASE_REG(index) + i, &regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        *accessPtr = regData;
        accessPtr ++;
    }


    _l2sw_svlanMc2sStSmi2User(units, pSvlanMc2sCfg, smiSvlanMC2S);

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicSvlanSP2CConf
 * Description:
 *      Set system 128 SP2C content
 * Input:
 *      index           - index of 128 SVLAN & Port to CVLAN configuration
 *      pSvlanSp2cCfg   - SVLAN & Port to CVLAN configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENTRY_INDEX  - Invalid entry index
 * Note:
 *      None
 */
ret_t l2sw_setAsicSvlanSP2CConf(l2sw_uint8 units, l2sw_uint32 index, l2sw_svlan_s2c_t* pSvlanSp2cCfg)
{
    ret_t retVal;
    l2sw_uint32 regData;
    l2sw_uint16 *accessPtr;
    l2sw_uint32 i;
    l2sw_uint16 smiSvlanSP2C[L2SW_SVLAN_SP2C_LEN];

    if(index > L2SW_SP2CMAX)
        return L2SW_ERR_ENTRY_INDEX;

    MEMSET(smiSvlanSP2C, 0x00, sizeof(l2sw_uint16) * L2SW_SVLAN_SP2C_LEN);
    _l2sw_svlanSp2cStUser2Smi(units, pSvlanSp2cCfg,smiSvlanSP2C);

    accessPtr = smiSvlanSP2C;

    for(i = 0; i < 2; i++)
    {
        regData = *(accessPtr + i);
        retVal = l2sw_setAsicReg(units, L2SW_SVLAN_S2C_ENTRY_BASE_REG(index) + i, regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicSvlanSP2CConf
 * Description:
 *      Get system 128 SP2C content
 * Input:
 *      index           - index of 128 SVLAN & Port to CVLAN configuration
 *      pSvlanSp2cCfg   - SVLAN & Port to CVLAN configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENTRY_INDEX  - Invalid entry index
 * Note:
 *      None
 */
ret_t l2sw_getAsicSvlanSP2CConf(l2sw_uint8 units, l2sw_uint32 index,l2sw_svlan_s2c_t* pSvlanSp2cCfg)
{
    ret_t retVal;
    l2sw_uint32 regData;
    l2sw_uint16 *accessPtr;
    l2sw_uint32 i;
    l2sw_uint16 smiSvlanSP2C[L2SW_SVLAN_SP2C_LEN];

    if(index > L2SW_SP2CMAX)
        return L2SW_ERR_ENTRY_INDEX;

    MEMSET(smiSvlanSP2C, 0x00, sizeof(l2sw_uint16) * L2SW_SVLAN_SP2C_LEN);

    accessPtr = smiSvlanSP2C;

    for(i = 0; i < 2; i++)
    {
        retVal = l2sw_getAsicReg(units, L2SW_SVLAN_S2C_ENTRY_BASE_REG(index) + i, &regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        *accessPtr = regData;

        accessPtr ++;
    }

    _l2sw_svlanSp2cStSmi2User(units, pSvlanSp2cCfg, smiSvlanSP2C);

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicSvlanDmacCvidSel
 * Description:
 *      Set downstream CVID decision by DMAC
 * Input:
 *      port        - Physical port number (0~7)
 *      enabled     - 0:disabled, 1:enabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsicSvlanDmacCvidSel(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 8)
        return l2sw_setAsicRegBit(units, L2SW_REG_SVLAN_CFG, L2SW_VS_PORT0_DMACVIDSEL_OFFSET + port, enabled);
    else
        return l2sw_setAsicRegBit(units, L2SW_REG_SVLAN_CFG_EXT, L2SW_VS_PORT8_DMACVIDSEL_OFFSET + (port-8), enabled);
}
/* Function Name:
 *      l2sw_getAsicSvlanDmacCvidSel
 * Description:
 *      Get downstream CVID decision by DMAC
 * Input:
 *      port        - Physical port number (0~7)
 *      pEnabled    - 0:disabled, 1:enabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicSvlanDmacCvidSel(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pEnabled)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 8)
        return l2sw_getAsicRegBit(units, L2SW_REG_SVLAN_CFG, L2SW_VS_PORT0_DMACVIDSEL_OFFSET + port, pEnabled);
    else
        return l2sw_getAsicRegBit(units, L2SW_REG_SVLAN_CFG_EXT, L2SW_VS_PORT8_DMACVIDSEL_OFFSET + (port-8), pEnabled);
}
/* Function Name:
 *      l2sw_setAsicSvlanUntagVlan
 * Description:
 *      Set default ingress untag SVLAN
 * Input:
 *      index   - index SVLAN member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_SVLAN_ENTRY_INDEX    - Invalid SVLAN index parameter
 * Note:
 *      None
 */
ret_t l2sw_setAsicSvlanUntagVlan(l2sw_uint8 units, l2sw_uint32 index)
{
    if(index > L2SW_SVIDXMAX)
        return L2SW_ERR_SVLAN_ENTRY_INDEX;

    return l2sw_setAsicRegBits(units, L2SW_REG_SVLAN_UNTAG_UNMAT_CFG, L2SW_VS_UNTAG_SVIDX_MASK, index);
}
/* Function Name:
 *      l2sw_getAsicSvlanUntagVlan
 * Description:
 *      Get default ingress untag SVLAN
 * Input:
 *      pIndex  - index SVLAN member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicSvlanUntagVlan(l2sw_uint8 units, l2sw_uint32* pIndex)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_SVLAN_UNTAG_UNMAT_CFG, L2SW_VS_UNTAG_SVIDX_MASK, pIndex);
}

/* Function Name:
 *      l2sw_setAsicSvlanUnmatchVlan
 * Description:
 *      Set default ingress unmatch SVLAN
 * Input:
 *      index   - index SVLAN member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_SVLAN_ENTRY_INDEX    - Invalid SVLAN index parameter
 * Note:
 *      None
 */
ret_t l2sw_setAsicSvlanUnmatchVlan(l2sw_uint8 units, l2sw_uint32 index)
{
    if(index > L2SW_SVIDXMAX)
        return L2SW_ERR_SVLAN_ENTRY_INDEX;

    return l2sw_setAsicRegBits(units, L2SW_REG_SVLAN_UNTAG_UNMAT_CFG, L2SW_VS_UNMAT_SVIDX_MASK, index);
}
/* Function Name:
 *      l2sw_getAsicSvlanUnmatchVlan
 * Description:
 *      Get default ingress unmatch SVLAN
 * Input:
 *      pIndex  - index SVLAN member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicSvlanUnmatchVlan(l2sw_uint8 units, l2sw_uint32* pIndex)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_SVLAN_UNTAG_UNMAT_CFG, L2SW_VS_UNMAT_SVIDX_MASK, pIndex);
}


/* Function Name:
 *      l2sw_setAsicSvlanLookupType
 * Description:
 *      Set svlan lookup table selection
 * Input:
 *      type    - lookup type
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicSvlanLookupType(l2sw_uint8 units, l2sw_uint32 type)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_SVLAN_LOOKUP_TYPE, L2SW_SVLAN_LOOKUP_TYPE_OFFSET, type);
}

/* Function Name:
 *      l2sw_svlan_s2c_add_cpri_set
 * Description:
 *      Set the source of priority field in the new c-tag of S2C downstream
 * Input:
 *      cpri_type   - CPRI value in CTAG
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_svlan_s2c_add_cpri_set(l2sw_uint8 units, l2sw_uint32 cpri_type)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_EVLAN_PRIVATE_CTRL, L2SW_EVLAN_S2C_CPRI_ADD_SEL_OFFSET, cpri_type);
}

/* Function Name:
 *      l2sw_svlan_s2c_add_cpri_get
 * Description:
 *      Get the source of priority field in the new c-tag of S2C downstream
 * Input:
 *      None
 * Output:
 *      pcpri_type  - CPRI value in CTAG
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_svlan_s2c_add_cpri_get(l2sw_uint8 units, l2sw_uint32 *pcpri_type)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_EVLAN_PRIVATE_CTRL, L2SW_EVLAN_S2C_CPRI_ADD_SEL_OFFSET, pcpri_type);
}



