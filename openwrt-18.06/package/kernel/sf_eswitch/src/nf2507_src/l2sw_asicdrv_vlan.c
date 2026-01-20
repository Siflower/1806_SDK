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

#include "l2sw_asicdrv_vlan.h"
#include "l2sw_switch.h"
#include "sal_string.h"

static void _l2sw_VlanMCStUser2Smi(l2sw_uint8 units, l2sw_vlanconfiguser *pVlanCg, l2sw_uint16 *pSmiVlanCfg)
{
    pSmiVlanCfg[0] |= pVlanCg->mbr & 0x07FF;

    pSmiVlanCfg[1] |= pVlanCg->fid_msti & 0x000F;

    pSmiVlanCfg[2] |= pVlanCg->vbpen & 0x0001;
    pSmiVlanCfg[2] |= (pVlanCg->vbpri & 0x0007) << 1;
    pSmiVlanCfg[2] |= (pVlanCg->envlanpol & 0x0001) << 4;
    pSmiVlanCfg[2] |= (pVlanCg->meteridx & 0x003F) << 5;

    pSmiVlanCfg[3] |= pVlanCg->evid & 0x1FFF;
}

static void _l2sw_VlanMCStSmi2User(l2sw_uint8 units, l2sw_uint16 *pSmiVlanCfg, l2sw_vlanconfiguser *pVlanCg)
{
    pVlanCg->mbr            = pSmiVlanCfg[0] & 0x07FF;
    pVlanCg->fid_msti       = pSmiVlanCfg[1] & 0x000F;
    pVlanCg->meteridx       = (pSmiVlanCfg[2] >> 5) & 0x003F;
    pVlanCg->envlanpol      = (pSmiVlanCfg[2] >> 4) & 0x0001;
    pVlanCg->vbpri          = (pSmiVlanCfg[2] >> 1) & 0x0007;
    pVlanCg->vbpen          = pSmiVlanCfg[2] & 0x0001;
    pVlanCg->evid           = pSmiVlanCfg[3] & 0x1FFF;
}

static void _l2sw_Vlan4kStUser2Smi(l2sw_uint8 units, l2sw_user_vlan4kentry *pUserVlan4kEntry, l2sw_uint16 *pSmiVlan4kEntry)
{
    pSmiVlan4kEntry[0] |= (pUserVlan4kEntry->mbr & 0x00FF);
    pSmiVlan4kEntry[0] |= (pUserVlan4kEntry->untag & 0x00FF) << 8;

    pSmiVlan4kEntry[1] |= (pUserVlan4kEntry->fid_msti & 0x000F);
    pSmiVlan4kEntry[1] |= (pUserVlan4kEntry->vbpen & 0x0001) << 4;
    pSmiVlan4kEntry[1] |= (pUserVlan4kEntry->vbpri & 0x0007) << 5;
    pSmiVlan4kEntry[1] |= (pUserVlan4kEntry->envlanpol & 0x0001) << 8;
    pSmiVlan4kEntry[1] |= (pUserVlan4kEntry->meteridx & 0x001F) << 9;
    pSmiVlan4kEntry[1] |= (pUserVlan4kEntry->ivl_svl & 0x0001) << 14;

    pSmiVlan4kEntry[2] |= ((pUserVlan4kEntry->mbr & 0x0700) >> 8);
    pSmiVlan4kEntry[2] |= ((pUserVlan4kEntry->untag & 0x0700) >> 8) << 3;
    pSmiVlan4kEntry[2] |= ((pUserVlan4kEntry->meteridx & 0x0020) >> 5) << 6;
}


static void _l2sw_Vlan4kStSmi2User(l2sw_uint8 units, l2sw_uint16 *pSmiVlan4kEntry, l2sw_user_vlan4kentry *pUserVlan4kEntry)
{
    pUserVlan4kEntry->mbr = (pSmiVlan4kEntry[0] & 0x00FF) | ((pSmiVlan4kEntry[2] & 0x0007) << 8);
    pUserVlan4kEntry->untag = ((pSmiVlan4kEntry[0] & 0xFF00) >> 8) | (((pSmiVlan4kEntry[2] & 0x0038) >> 3) << 8);
    pUserVlan4kEntry->fid_msti = pSmiVlan4kEntry[1] & 0x000F;
    pUserVlan4kEntry->vbpen = (pSmiVlan4kEntry[1] & 0x0010) >> 4;
    pUserVlan4kEntry->vbpri = (pSmiVlan4kEntry[1] & 0x00E0) >> 5;
    pUserVlan4kEntry->envlanpol = (pSmiVlan4kEntry[1] & 0x0100) >> 8;
    pUserVlan4kEntry->meteridx = ((pSmiVlan4kEntry[1] & 0x3E00) >> 9) | (((pSmiVlan4kEntry[2] & 0x0040) >> 6) << 5);
    pUserVlan4kEntry->ivl_svl = (pSmiVlan4kEntry[1] & 0x4000) >> 14;
}

/* Function Name:
 *      l2sw_setAsicVlanMemberConfig
 * Description:
 *      Set 32 VLAN member configurations
 * Input:
 *      index       - VLAN member configuration index (0~31)
 *      pVlanCg - VLAN member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_INPUT                - Invalid input parameter
 *      L2SW_ERR_L2_FID               - Invalid FID
 *      L2SW_ERR_PORT_MASK            - Invalid portmask
 *      L2SW_ERR_FILTER_METER_ID      - Invalid meter
 *      L2SW_ERR_QOS_INT_PRIORITY     - Invalid priority
 *      L2SW_ERR_VLAN_ENTRY_NOT_FOUND - Invalid VLAN member configuration index
 * Note:
 *      None
 */
ret_t l2sw_setAsicVlanMemberConfig(l2sw_uint8 units, l2sw_uint32 index, l2sw_vlanconfiguser *pVlanCg)
{
    ret_t  retVal;
    l2sw_uint32 regAddr;
    l2sw_uint32 regData;
    l2sw_uint16 *tableAddr;
    l2sw_uint32 page_idx;
    l2sw_uint16 smi_vlancfg[L2SW_VLAN_MBRCFG_LEN];

    /* Error Checking  */
    if(index > L2SW_CVIDXMAX)
        return L2SW_ERR_VLAN_ENTRY_NOT_FOUND;

    if(pVlanCg->evid > L2SW_EVIDMAX)
        return L2SW_ERR_INPUT;


    if(pVlanCg->mbr > L2SW_PORTMASK)
        return L2SW_ERR_PORT_MASK;

    if(pVlanCg->fid_msti > L2SW_FIDMAX)
        return L2SW_ERR_L2_FID;

    if(pVlanCg->meteridx > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    if(pVlanCg->vbpri > L2SW_PRIMAX)
        return L2SW_ERR_QOS_INT_PRIORITY;

    MEMSET(smi_vlancfg, 0x00, sizeof(l2sw_uint16) * L2SW_VLAN_MBRCFG_LEN);
    _l2sw_VlanMCStUser2Smi(units, pVlanCg, smi_vlancfg);
    tableAddr = smi_vlancfg;

    for(page_idx = 0; page_idx < 4; page_idx++)  /* 4 pages per VLAN Member Config */
    {
        regAddr = L2SW_VLAN_MEMBER_CONFIGURATION_BASE + (index * 4) + page_idx;
        regData = *tableAddr;

        retVal = l2sw_setAsicReg(units, regAddr, regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        tableAddr++;
    }

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicVlanMemberConfig
 * Description:
 *      Get 32 VLAN member configurations
 * Input:
 *      index       - VLAN member configuration index (0~31)
 *      pVlanCg - VLAN member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_INPUT                - Invalid input parameter
 *      L2SW_ERR_VLAN_ENTRY_NOT_FOUND - Invalid VLAN member configuration index
 * Note:
 *      None
 */
ret_t l2sw_getAsicVlanMemberConfig(l2sw_uint8 units, l2sw_uint32 index, l2sw_vlanconfiguser *pVlanCg)
{
    ret_t  retVal;
    l2sw_uint32 page_idx;
    l2sw_uint32 regAddr;
    l2sw_uint32 regData;
    l2sw_uint16 *tableAddr;
    l2sw_uint16 smi_vlancfg[L2SW_VLAN_MBRCFG_LEN];

    if(index > L2SW_CVIDXMAX)
        return L2SW_ERR_VLAN_ENTRY_NOT_FOUND;

    MEMSET(smi_vlancfg, 0x00, sizeof(l2sw_uint16) * L2SW_VLAN_MBRCFG_LEN);
    tableAddr  = smi_vlancfg;

    for(page_idx = 0; page_idx < 4; page_idx++)  /* 4 pages per VLAN Member Config */
    {
        regAddr = L2SW_VLAN_MEMBER_CONFIGURATION_BASE + (index * 4) + page_idx;

        retVal = l2sw_getAsicReg(units, regAddr, &regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        *tableAddr = (l2sw_uint16)regData;
        tableAddr++;
    }

    _l2sw_VlanMCStSmi2User(units, smi_vlancfg, pVlanCg);
    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicVlan4kEntry
 * Description:
 *      Set VID mapped entry to 4K VLAN table
 * Input:
 *      pVlan4kEntry - 4K VLAN configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_INPUT                - Invalid input parameter
 *      L2SW_ERR_L2_FID               - Invalid FID
 *      L2SW_ERR_VLAN_VID             - Invalid VID parameter (0~4095)
 *      L2SW_ERR_PORT_MASK            - Invalid portmask
 *      L2SW_ERR_FILTER_METER_ID      - Invalid meter
 *      L2SW_ERR_QOS_INT_PRIORITY     - Invalid priority
 * Note:
 *      None
 */
ret_t l2sw_setAsicVlan4kEntry(l2sw_uint8 units, l2sw_user_vlan4kentry *pVlan4kEntry )
{
    l2sw_uint16              vlan_4k_entry[L2SW_VLAN_4KTABLE_LEN];
    l2sw_uint32                  page_idx;
    l2sw_uint16                  *tableAddr;
    ret_t                   retVal;
    l2sw_uint32                  regData;

    if(pVlan4kEntry->vid > L2SW_VIDMAX)
        return L2SW_ERR_VLAN_VID;

    if(pVlan4kEntry->mbr > L2SW_PORTMASK)
        return L2SW_ERR_PORT_MASK;

    if(pVlan4kEntry->untag > L2SW_PORTMASK)
        return L2SW_ERR_PORT_MASK;

    if(pVlan4kEntry->fid_msti > L2SW_FIDMAX)
        return L2SW_ERR_L2_FID;

    if(pVlan4kEntry->meteridx > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    if(pVlan4kEntry->vbpri > L2SW_PRIMAX)
        return L2SW_ERR_QOS_INT_PRIORITY;

    MEMSET(vlan_4k_entry, 0x00, sizeof(l2sw_uint16) * L2SW_VLAN_4KTABLE_LEN);
    _l2sw_Vlan4kStUser2Smi(units, pVlan4kEntry, vlan_4k_entry);

    /* Prepare Data */
    tableAddr = vlan_4k_entry;
    for(page_idx = 0; page_idx < L2SW_VLAN_4KTABLE_LEN; page_idx++)
    {
        regData = *tableAddr;
        retVal = l2sw_setAsicReg(units, L2SW_TABLE_ACCESS_WRDATA_BASE + page_idx, regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        tableAddr++;
    }

    /* Write Address (VLAN_ID) */
    regData = pVlan4kEntry->vid;
    retVal = l2sw_setAsicReg(units, L2SW_TABLE_ACCESS_ADDR_REG, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /* Write Command */
    retVal = l2sw_setAsicRegBits(units, L2SW_TABLE_ACCESS_CTRL_REG, L2SW_TABLE_TYPE_MASK | L2SW_COMMAND_TYPE_MASK,L2SW_TABLE_ACCESS_REG_DATA(TB_OP_WRITE,TB_TARGET_CVLAN));
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicVlan4kEntry
 * Description:
 *      Get VID mapped entry to 4K VLAN table
 * Input:
 *      pVlan4kEntry - 4K VLAN configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_VLAN_VID         - Invalid VID parameter (0~4095)
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - LUT is busy at retrieving
 * Note:
 *      None
 */
ret_t l2sw_getAsicVlan4kEntry(l2sw_uint8 units, l2sw_user_vlan4kentry *pVlan4kEntry )
{
    l2sw_uint16                  vlan_4k_entry[L2SW_VLAN_4KTABLE_LEN];
    l2sw_uint32                  page_idx;
    l2sw_uint16                  *tableAddr;
    ret_t                       retVal;
    l2sw_uint32                  regData;
    l2sw_uint32                  busyCounter;

    if(pVlan4kEntry->vid > L2SW_VIDMAX)
        return L2SW_ERR_VLAN_VID;

    /* Polling status */
    busyCounter = L2SW_VLAN_BUSY_CHECK_NO;
    while(busyCounter)
    {
        retVal = l2sw_getAsicRegBit(units, L2SW_TABLE_ACCESS_STATUS_REG, L2SW_TABLE_LUT_ADDR_BUSY_FLAG_OFFSET,&regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        if(regData == 0)
            break;

        busyCounter --;
        if(busyCounter == 0)
            return L2SW_ERR_BUSYWAIT_TIMEOUT;
    }

    /* Write Address (VLAN_ID) */
    regData = pVlan4kEntry->vid;
    retVal = l2sw_setAsicReg(units, L2SW_TABLE_ACCESS_ADDR_REG, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /* Read Command */
    retVal = l2sw_setAsicRegBits(units, L2SW_TABLE_ACCESS_CTRL_REG, L2SW_TABLE_TYPE_MASK | L2SW_COMMAND_TYPE_MASK, L2SW_TABLE_ACCESS_REG_DATA(TB_OP_READ,TB_TARGET_CVLAN));
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /* Polling status */
    busyCounter = L2SW_VLAN_BUSY_CHECK_NO;
    while(busyCounter)
    {
        retVal = l2sw_getAsicRegBit(units, L2SW_TABLE_ACCESS_STATUS_REG, L2SW_TABLE_LUT_ADDR_BUSY_FLAG_OFFSET,&regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        if(regData == 0)
            break;

        busyCounter --;
        if(busyCounter == 0)
            return L2SW_ERR_BUSYWAIT_TIMEOUT;
    }

    /* Read VLAN data from register */
    tableAddr = vlan_4k_entry;
    for(page_idx = 0; page_idx < L2SW_VLAN_4KTABLE_LEN; page_idx++)
    {
        retVal = l2sw_getAsicReg(units, L2SW_TABLE_ACCESS_RDDATA_BASE + page_idx, &regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        *tableAddr = regData;
        tableAddr++;
    }

    _l2sw_Vlan4kStSmi2User(units, vlan_4k_entry, pVlan4kEntry);

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicVlanAccpetFrameType
 * Description:
 *      Set per-port acceptable frame type
 * Input:
 *      port        - Physical port number (0~10)
 *      frameType   - The acceptable frame type
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                       - Success
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_PORT_ID                  - Invalid port number
 *      L2SW_ERR_VLAN_ACCEPT_FRAME_TYPE   - Invalid frame type
 * Note:
 *      None
 */
ret_t l2sw_setAsicVlanAccpetFrameType(l2sw_uint8 units, l2sw_uint32 port, l2sw_accframetype frameType)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(frameType >= FRAME_TYPE_MAX_BOUND)
        return L2SW_ERR_VLAN_ACCEPT_FRAME_TYPE;

    return l2sw_setAsicRegBits(units, L2SW_VLAN_ACCEPT_FRAME_TYPE_REG(port), L2SW_VLAN_ACCEPT_FRAME_TYPE_MASK(port), frameType);
}
/* Function Name:
 *      l2sw_getAsicVlanAccpetFrameType
 * Description:
 *      Get per-port acceptable frame type
 * Input:
 *      port        - Physical port number (0~10)
 *      pFrameType  - The acceptable frame type
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                       - Success
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_PORT_ID                  - Invalid port number
 *      L2SW_ERR_VLAN_ACCEPT_FRAME_TYPE   - Invalid frame type
 * Note:
 *      None
 */
ret_t l2sw_getAsicVlanAccpetFrameType(l2sw_uint8 units, l2sw_uint32 port, l2sw_accframetype *pFrameType)
{
    l2sw_uint32 regData;
    ret_t  retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if((retVal = l2sw_getAsicRegBits(units, L2SW_VLAN_ACCEPT_FRAME_TYPE_REG(port), L2SW_VLAN_ACCEPT_FRAME_TYPE_MASK(port), &regData)) != L2SW_ERR_OK)
        return retVal;

    *pFrameType = (l2sw_accframetype)regData;
    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicVlanIngressFilter
 * Description:
 *      Set VLAN Ingress Filter
 * Input:
 *      port        - Physical port number (0~10)
 *      enabled     - Enable or disable Ingress filter
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsicVlanIngressFilter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    return l2sw_setAsicRegBit(units, L2SW_VLAN_INGRESS_REG, port, enabled);
}
/* Function Name:
 *      l2sw_getAsicVlanIngressFilter
 * Description:
 *      Get VLAN Ingress Filter
 * Input:
 *      port        - Physical port number (0~10)
 *      pEnable     - Enable or disable Ingress filter
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicVlanIngressFilter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnable)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBit(units, L2SW_VLAN_INGRESS_REG, port, pEnable);
}
/* Function Name:
 *      l2sw_setAsicVlanEgressTagMode
 * Description:
 *      Set CVLAN egress tag mode
 * Input:
 *      port        - Physical port number (0~10)
 *      tagMode     - The egress tag mode. Including Original mode, Keep tag mode and Priority tag mode
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_INPUT    - Invalid input parameter
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsicVlanEgressTagMode(l2sw_uint8 units, l2sw_uint32 port, l2sw_egtagmode tagMode)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(tagMode >= EG_TAG_MODE_END)
        return L2SW_ERR_INPUT;

    return l2sw_setAsicRegBits(units, L2SW_PORT_MISC_CFG_REG(port), L2SW_VLAN_EGRESS_MDOE_MASK, tagMode);
}
/* Function Name:
 *      l2sw_getAsicVlanEgressTagMode
 * Description:
 *      Get CVLAN egress tag mode
 * Input:
 *      port        - Physical port number (0~10)
 *      pTagMode    - The egress tag mode. Including Original mode, Keep tag mode and Priority tag mode
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicVlanEgressTagMode(l2sw_uint8 units, l2sw_uint32 port, l2sw_egtagmode *pTagMode)
{
    l2sw_uint32 regData;
    ret_t  retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if((retVal = l2sw_getAsicRegBits(units, L2SW_PORT_MISC_CFG_REG(port), L2SW_VLAN_EGRESS_MDOE_MASK, &regData)) != L2SW_ERR_OK)
        return retVal;

    *pTagMode = (l2sw_egtagmode)regData;
    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicVlanPortBasedVID
 * Description:
 *      Set port based VID which is indexed to 32 VLAN member configurations
 * Input:
 *      port    - Physical port number (0~10)
 *      index   - Index to VLAN member configuration
 *      pri     - 1Q Port based VLAN priority
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number
 *      L2SW_ERR_QOS_INT_PRIORITY     - Invalid priority
 *      L2SW_ERR_VLAN_ENTRY_NOT_FOUND - Invalid VLAN member configuration index
 * Note:
 *      None
 */
ret_t l2sw_setAsicVlanPortBasedVID(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 index, l2sw_uint32 pri)
{
    l2sw_uint32 regAddr, bit_mask;
    ret_t  retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(index > L2SW_CVIDXMAX)
        return L2SW_ERR_VLAN_ENTRY_NOT_FOUND;

    if(pri > L2SW_PRIMAX)
        return L2SW_ERR_QOS_INT_PRIORITY;

    regAddr = L2SW_VLAN_PVID_CTRL_REG(port);
    bit_mask = L2SW_PORT_VIDX_MASK(port);
    retVal = l2sw_setAsicRegBits(units, regAddr, bit_mask, index);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    regAddr = L2SW_VLAN_PORTBASED_PRIORITY_REG(port);
    bit_mask = L2SW_VLAN_PORTBASED_PRIORITY_MASK(port);
    retVal = l2sw_setAsicRegBits(units, regAddr, bit_mask, pri);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicVlanPortBasedVID
 * Description:
 *      Get port based VID which is indexed to 32 VLAN member configurations
 * Input:
 *      port    - Physical port number (0~10)
 *      pIndex  - Index to VLAN member configuration
 *      pPri    - 1Q Port based VLAN priority
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicVlanPortBasedVID(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pIndex, l2sw_uint32 *pPri)
{
    l2sw_uint32 regAddr,bit_mask;
    ret_t  retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    regAddr = L2SW_VLAN_PVID_CTRL_REG(port);
    bit_mask = L2SW_PORT_VIDX_MASK(port);
    retVal = l2sw_getAsicRegBits(units, regAddr, bit_mask, pIndex);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    regAddr = L2SW_VLAN_PORTBASED_PRIORITY_REG(port);
    bit_mask = L2SW_VLAN_PORTBASED_PRIORITY_MASK(port);
    retVal = l2sw_getAsicRegBits(units, regAddr, bit_mask, pPri);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicVlanProtocolBasedGroupData
 * Description:
 *      Set protocol and port based group database
 * Input:
 *      index       - Index to VLAN member configuration
 *      pPbCfg  - Protocol and port based group database entry
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_INPUT                - Invalid input parameter
 *      L2SW_ERR_VLAN_PROTO_AND_PORT  - Invalid protocol base group database index
 * Note:
 *      None
 */
ret_t l2sw_setAsicVlanProtocolBasedGroupData(l2sw_uint8 units, l2sw_uint32 index, l2sw_protocolgdatacfg *pPbCfg)
{
    l2sw_uint32  frameType;
    l2sw_uint32  etherType;
    ret_t   retVal;

    /* Error Checking */
    if(index > L2SW_PROTOVLAN_GIDX_MAX)
        return L2SW_ERR_VLAN_PROTO_AND_PORT;

    if(pPbCfg->frameType >= PPVLAN_FRAME_TYPE_END )
        return L2SW_ERR_INPUT;

    frameType = pPbCfg->frameType;
    etherType = pPbCfg->etherType;

    /* Frame type */
    retVal = l2sw_setAsicRegBits(units, L2SW_VLAN_PPB_FRAMETYPE_REG(index), L2SW_VLAN_PPB_FRAMETYPE_MASK, frameType);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /* Ether type */
    retVal = l2sw_setAsicReg(units, L2SW_VLAN_PPB_ETHERTYPR_REG(index), etherType);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicVlanProtocolBasedGroupData
 * Description:
 *      Get protocol and port based group database
 * Input:
 *      index       - Index to VLAN member configuration
 *      pPbCfg  - Protocol and port based group database entry
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_INPUT                - Invalid input parameter
 *      L2SW_ERR_VLAN_PROTO_AND_PORT  - Invalid protocol base group database index
 * Note:
 *      None
 */
ret_t l2sw_getAsicVlanProtocolBasedGroupData(l2sw_uint8 units, l2sw_uint32 index, l2sw_protocolgdatacfg *pPbCfg)
{
    l2sw_uint32  frameType;
    l2sw_uint32  etherType;
    ret_t   retVal;

    /* Error Checking */
    if(index > L2SW_PROTOVLAN_GIDX_MAX)
        return L2SW_ERR_VLAN_PROTO_AND_PORT;

    /* Read Frame type */
    retVal = l2sw_getAsicRegBits(units, L2SW_VLAN_PPB_FRAMETYPE_REG(index), L2SW_VLAN_PPB_FRAMETYPE_MASK, &frameType);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /* Read Ether type */
    retVal = l2sw_getAsicReg(units, L2SW_VLAN_PPB_ETHERTYPR_REG(index), &etherType);
    if(retVal != L2SW_ERR_OK)
        return retVal;


    pPbCfg->frameType = frameType;
    pPbCfg->etherType = etherType;
    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicVlanPortAndProtocolBased
 * Description:
 *      Set protocol and port based VLAN configuration
 * Input:
 *      port        - Physical port number (0~10)
 *      index       - Index of protocol and port based database index
 *      pPpbCfg     - Protocol and port based VLAN configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_INPUT                - Invalid input parameter
 *      L2SW_ERR_PORT_ID              - Invalid port number
 *      L2SW_ERR_QOS_INT_PRIORITY     - Invalid priority
 *      L2SW_ERR_VLAN_PROTO_AND_PORT  - Invalid protocol base group database index
 *      L2SW_ERR_VLAN_ENTRY_NOT_FOUND - Invalid VLAN member configuration index
 * Note:
 *      None
 */
ret_t l2sw_setAsicVlanPortAndProtocolBased(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 index, l2sw_protocolvlancfg *pPpbCfg)
{
    l2sw_uint32  reg_addr, bit_mask, bit_value;
    ret_t   retVal;

    /* Error Checking */
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(index > L2SW_PROTOVLAN_GIDX_MAX)
        return L2SW_ERR_VLAN_PROTO_AND_PORT;

    if( (pPpbCfg->valid != L2SW_FALSE) && (pPpbCfg->valid != L2SW_TRUE) )
        return L2SW_ERR_INPUT;

    if(pPpbCfg->vlan_idx > L2SW_CVIDXMAX)
        return L2SW_ERR_VLAN_ENTRY_NOT_FOUND;

    if(pPpbCfg->priority > L2SW_PRIMAX)
        return L2SW_ERR_QOS_INT_PRIORITY;

    /* Valid bit */
    reg_addr  = L2SW_VLAN_PPB_VALID_REG(index);
    bit_mask  = 0x0001 << port;
    bit_value = ((L2SW_TRUE == pPpbCfg->valid) ? 0x1 : 0x0);
    retVal    = l2sw_setAsicRegBits(units, reg_addr, bit_mask, bit_value);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /* Calculate the actual register address for CVLAN index*/
    if(port < 8)
    {
        reg_addr = L2SW_VLAN_PPB_CTRL_REG(index, port);
        bit_mask = L2SW_VLAN_PPB_CTRL_MASK(port);
    }
    else if(port == 8)
    {
        reg_addr = L2SW_REG_VLAN_PPB0_CTRL4;
        bit_mask = L2SW_VLAN_PPB0_CTRL4_PORT8_INDEX_MASK;
    }
    else if(port == 9)
    {
        reg_addr = L2SW_REG_VLAN_PPB0_CTRL4;
        bit_mask = L2SW_VLAN_PPB0_CTRL4_PORT9_INDEX_MASK;
    }
    else if(port == 10)
    {
        reg_addr = L2SW_REG_VLAN_PPB0_CTRL4;
        bit_mask = L2SW_VLAN_PPB0_CTRL4_PORT10_INDEX_MASK;
    }

    bit_value = pPpbCfg->vlan_idx;
    retVal  = l2sw_setAsicRegBits(units, reg_addr, bit_mask, bit_value);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /* write priority */
    reg_addr  = L2SW_VLAN_PPB_PRIORITY_ITEM_REG(port, index);
    bit_mask  = L2SW_VLAN_PPB_PRIORITY_ITEM_MASK(port);
    bit_value = pPpbCfg->priority;
    retVal    = l2sw_setAsicRegBits(units, reg_addr, bit_mask, bit_value);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicVlanPortAndProtocolBased
 * Description:
 *      Get protocol and port based VLAN configuration
 * Input:
 *      port        - Physical port number (0~7)
 *      index       - Index of protocol and port based database index
 *      pPpbCfg     - Protocol and port based VLAN configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_INPUT                - Invalid input parameter
 *      L2SW_ERR_PORT_ID              - Invalid port number
 *      L2SW_ERR_VLAN_PROTO_AND_PORT  - Invalid protocol base group database index
 * Note:
 *      None
 */
ret_t l2sw_getAsicVlanPortAndProtocolBased(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 index, l2sw_protocolvlancfg *pPpbCfg)
{
    l2sw_uint32  reg_addr, bit_mask, bit_value;
    ret_t   retVal;

    /* Error Checking */
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(index > L2SW_PROTOVLAN_GIDX_MAX)
        return L2SW_ERR_VLAN_PROTO_AND_PORT;

    if(pPpbCfg == NULL)
        return L2SW_ERR_INPUT;

    /* Valid bit */
    reg_addr  = L2SW_VLAN_PPB_VALID_REG(index);
    bit_mask  = 0x0001 << port;
    retVal    = l2sw_getAsicRegBits(units, reg_addr, bit_mask, &bit_value);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    pPpbCfg->valid = bit_value;

    /* CVLAN index */
    if(port < 8)
    {
        reg_addr = L2SW_VLAN_PPB_CTRL_REG(index, port);
        bit_mask = L2SW_VLAN_PPB_CTRL_MASK(port);
    }
    else if(port == 8)
    {
        reg_addr = L2SW_REG_VLAN_PPB0_CTRL4;
        bit_mask = L2SW_VLAN_PPB0_CTRL4_PORT8_INDEX_MASK;
    }
    else if(port == 9)
    {
        reg_addr = L2SW_REG_VLAN_PPB0_CTRL4;
        bit_mask = L2SW_VLAN_PPB0_CTRL4_PORT9_INDEX_MASK;
    }
    else if(port == 10)
    {
        reg_addr = L2SW_REG_VLAN_PPB0_CTRL4;
        bit_mask = L2SW_VLAN_PPB0_CTRL4_PORT10_INDEX_MASK;
    }

    retVal = l2sw_getAsicRegBits(units, reg_addr, bit_mask, &bit_value);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    pPpbCfg->vlan_idx = bit_value;


    /* priority */
    reg_addr = L2SW_VLAN_PPB_PRIORITY_ITEM_REG(port,index);
    bit_mask = L2SW_VLAN_PPB_PRIORITY_ITEM_MASK(port);
    retVal = l2sw_getAsicRegBits(units, reg_addr, bit_mask, &bit_value);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    pPpbCfg->priority = bit_value;
    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicVlanFilter
 * Description:
 *      Set enable CVLAN filtering function
 * Input:
 *      enabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicVlanFilter(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_VLAN_CTRL, L2SW_VLAN_CTRL_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicVlanFilter
 * Description:
 *      Get enable CVLAN filtering function
 * Input:
 *      pEnabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicVlanFilter(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_VLAN_CTRL, L2SW_VLAN_CTRL_OFFSET, pEnabled);
}
/* Function Name:
 *      l2sw_setAsicVlanUntagDscpPriorityEn
 * Description:
 *      Set enable Dscp to untag 1Q priority
 * Input:
 *      enabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicVlanUntagDscpPriorityEn(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_UNTAG_DSCP_PRI_CFG, L2SW_UNTAG_DSCP_PRI_CFG_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicVlanUntagDscpPriorityEn
 * Description:
 *      Get enable Dscp to untag 1Q priority
 * Input:
 *      enabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicVlanUntagDscpPriorityEn(l2sw_uint8 units, l2sw_uint32* enabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_UNTAG_DSCP_PRI_CFG, L2SW_UNTAG_DSCP_PRI_CFG_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_setAsicPortBasedFid
 * Description:
 *      Set port based FID
 * Input:
 *      port    - Physical port number (0~10)
 *      fid     - Port based fid
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_L2_FID   - Invalid FID
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortBasedFid(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 fid)
{
    l2sw_uint32  reg_addr;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(fid > L2SW_FIDMAX)
        return L2SW_ERR_L2_FID;

    if(port < 8)
        return l2sw_setAsicReg(units, L2SW_PORT_PBFID_REG(port),fid);
    else {
        reg_addr = L2SW_REG_PORT8_PBFID + port-8;
        return l2sw_setAsicReg(units, reg_addr, fid);
    }

}
/* Function Name:
 *      l2sw_getAsicPortBasedFid
 * Description:
 *      Get port based FID
 * Input:
 *      port    - Physical port number (0~7)
 *      pFid    - Port based fid
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortBasedFid(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pFid)
{
    l2sw_uint32  reg_addr;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 8)
        return l2sw_getAsicReg(units, L2SW_PORT_PBFID_REG(port), pFid);
    else{
        reg_addr = L2SW_REG_PORT8_PBFID + port-8;
        return l2sw_getAsicReg(units, reg_addr, pFid);
    }
}
/* Function Name:
 *      l2sw_setAsicPortBasedFidEn
 * Description:
 *      Set port based FID selection enable
 * Input:
 *      port    - Physical port number (0~10)
 *      enabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortBasedFidEn(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    return l2sw_setAsicRegBit(units, L2SW_REG_PORT_PBFIDEN,port, enabled);
}
/* Function Name:
 *      l2sw_getAsicPortBasedFidEn
 * Description:
 *      Get port based FID selection enable
 * Input:
 *      port    - Physical port number (0~10)
 *      pEnabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortBasedFidEn(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pEnabled)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBit(units, L2SW_REG_PORT_PBFIDEN,port, pEnabled);
}
/* Function Name:
 *      l2sw_setAsicSpanningTreeStatus
 * Description:
 *      Set spanning tree state per each port
 * Input:
 *      port    - Physical port number (0~10)
 *      msti    - Multiple spanning tree instance
 *      state   - Spanning tree state for msti
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_MSTI         - Invalid msti parameter
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_MSTP_STATE   - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsicSpanningTreeStatus(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 msti, l2sw_uint32 state)
{
    l2sw_uint32  reg_addr,bits_msk = 0;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(msti > L2SW_MSTIMAX)
        return L2SW_ERR_MSTI;

    if(state > STPST_FORWARDING)
        return L2SW_ERR_MSTP_STATE;

    if(port < 8)
        return l2sw_setAsicRegBits(units, L2SW_VLAN_MSTI_REG(msti,port), L2SW_VLAN_MSTI_MASK(port),state);
    else{
        reg_addr = L2SW_VLAN_MSTI_REG(msti,port);
        switch(port){
            case 8: bits_msk = L2SW_VLAN_MSTI0_CTRL1_PORT8_STATE_MASK;break;
            case 9: bits_msk = L2SW_VLAN_MSTI0_CTRL1_PORT9_STATE_MASK;break;
            case 10: bits_msk = L2SW_VLAN_MSTI0_CTRL1_PORT10_STATE_MASK;break;
        }
        return l2sw_setAsicRegBits(units, reg_addr, bits_msk,state);
    }
}
/* Function Name:
 *      l2sw_getAsicSpanningTreeStatus
 * Description:
 *      Set spanning tree state per each port
 * Input:
 *      port    - Physical port number (0~10)
 *      msti    - Multiple spanning tree instance
 *      pState  - Spanning tree state for msti
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_MSTI         - Invalid msti parameter
 *      L2SW_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicSpanningTreeStatus(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 msti, l2sw_uint32* pState)
{
    l2sw_uint32  reg_addr,bits_msk = 0;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(msti > L2SW_MSTIMAX)
        return L2SW_ERR_MSTI;

    if(port < 8)
        return l2sw_getAsicRegBits(units, L2SW_VLAN_MSTI_REG(msti,port), L2SW_VLAN_MSTI_MASK(port), pState);
    else{
        reg_addr = L2SW_VLAN_MSTI_REG(msti,port);
        switch(port){
            case 8: bits_msk = L2SW_VLAN_MSTI0_CTRL1_PORT8_STATE_MASK;break;
            case 9: bits_msk = L2SW_VLAN_MSTI0_CTRL1_PORT9_STATE_MASK;break;
            case 10: bits_msk = L2SW_VLAN_MSTI0_CTRL1_PORT10_STATE_MASK;break;
        }
        return l2sw_getAsicRegBits(units, reg_addr, bits_msk, pState);
    }

}

/* Function Name:
 *      l2sw_setAsicVlanTransparent
 * Description:
 *      Set VLAN transparent
 * Input:
 *      port        - Physical port number (0~10)
 *      portmask    - portmask(0~0xFF)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_MASK    - Invalid portmask
 *      L2SW_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsicVlanTransparent(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 portmask)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(portmask > L2SW_PORTMASK)
        return L2SW_ERR_PORT_MASK;

    return l2sw_setAsicRegBits(units, L2SW_REG_VLAN_EGRESS_TRANS_CTRL0 + port, L2SW_VLAN_EGRESS_TRANS_CTRL0_MASK, portmask);
}

/* Function Name:
 *      l2sw_getAsicVlanTransparent
 * Description:
 *      Get VLAN transparent
 * Input:
 *      port        - Physical port number (0~10)
 * Output:
 *      pPortmask   - Ingress port mask
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_MASK    - Invalid portmask
 *      L2SW_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicVlanTransparent(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pPortmask)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBits(units, L2SW_REG_VLAN_EGRESS_TRANS_CTRL0 + port, L2SW_VLAN_EGRESS_TRANS_CTRL0_MASK, pPortmask);
}

/* Function Name:
 *      l2sw_setAsicVlanEgressKeep
 * Description:
 *      Set per egress port VLAN keep mode
 * Input:
 *      port        - Physical port number (0~10)
 *      portmask    - portmask(0~0xFF)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_MASK    - Invalid portmask
 *      L2SW_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsicVlanEgressKeep(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 portmask)
{
    l2sw_uint32 regAddr, bit_mask;
    ret_t  retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(portmask > L2SW_PORTMASK)
        return L2SW_ERR_PORT_MASK;

    if(port < 8){
        retVal = l2sw_setAsicRegBits(units, L2SW_REG_VLAN_EGRESS_KEEP_CTRL0 + (port>>1),L2SW_PORT0_VLAN_KEEP_MASK_MASK<<((port&1)*8),portmask & 0xff);
        if(retVal != L2SW_ERR_OK)
            return retVal;
        regAddr = L2SW_REG_VLAN_EGRESS_KEEP_CTRL0_EXT + (port>>1);
        bit_mask = L2SW_PORT0_VLAN_KEEP_MASK_EXT_MASK;
        bit_mask <<= (port&1)*3;
        retVal = l2sw_setAsicRegBits(units, regAddr, bit_mask, (portmask>>8)&0x7);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else{
        switch(port){
            case 8:
                regAddr = L2SW_REG_VLAN_EGRESS_KEEP_CTRL4;
                bit_mask = L2SW_PORT8_VLAN_KEEP_MASK_MASK;
                retVal = l2sw_setAsicRegBits(units, regAddr, bit_mask, portmask & 0xff);
                if(retVal != L2SW_ERR_OK)
                    return retVal;
                regAddr = L2SW_REG_VLAN_EGRESS_KEEP_CTRL4_EXT;
                bit_mask = L2SW_PORT8_VLAN_KEEP_MASK_EXT_MASK;
                retVal = l2sw_setAsicRegBits(units, regAddr, bit_mask, (portmask>>8)&0x7);
                if(retVal != L2SW_ERR_OK)
                    return retVal;
                break;

            case 9:
                regAddr = L2SW_REG_VLAN_EGRESS_KEEP_CTRL4;
                bit_mask = L2SW_PORT9_VLAN_KEEP_MASK_MASK;
                retVal = l2sw_setAsicRegBits(units, regAddr, bit_mask, portmask & 0xff);
                if(retVal != L2SW_ERR_OK)
                    return retVal;
                regAddr = L2SW_REG_VLAN_EGRESS_KEEP_CTRL4_EXT;
                bit_mask = L2SW_PORT9_VLAN_KEEP_MASK_EXT_MASK;
                retVal = l2sw_setAsicRegBits(units, regAddr, bit_mask, (portmask>>8)&0x7);
                if(retVal != L2SW_ERR_OK)
                    return retVal;
                break;

            case 10:
                regAddr = L2SW_REG_VLAN_EGRESS_KEEP_CTRL5;
                bit_mask = L2SW_VLAN_EGRESS_KEEP_CTRL5_MASK;
                retVal = l2sw_setAsicRegBits(units, regAddr, bit_mask, portmask & 0xff);
                if(retVal != L2SW_ERR_OK)
                    return retVal;
                regAddr = L2SW_REG_VLAN_EGRESS_KEEP_CTRL5_EXT;
                bit_mask = L2SW_VLAN_EGRESS_KEEP_CTRL5_EXT_MASK;
                retVal = l2sw_setAsicRegBits(units, regAddr, bit_mask, (portmask>>8)&0x7);
                if(retVal != L2SW_ERR_OK)
                    return retVal;
                break;
        }
    }

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicVlanEgressKeep
 * Description:
 *      Get per egress port VLAN keep mode
 * Input:
 *      port        - Physical port number (0~7)
 *      pPortmask   - portmask(0~0xFF)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicVlanEgressKeep(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pPortmask)
{
    l2sw_uint32 regAddr, bit_mask, regval_l, regval_h;
    ret_t  retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 8){
        retVal = l2sw_getAsicRegBits(units, L2SW_REG_VLAN_EGRESS_KEEP_CTRL0 + (port>>1),L2SW_PORT0_VLAN_KEEP_MASK_MASK<<((port&1)*8),&regval_l);
        if(retVal != L2SW_ERR_OK)
            return retVal;
        regAddr = L2SW_REG_VLAN_EGRESS_KEEP_CTRL0_EXT + (port>>1);
        bit_mask = L2SW_PORT0_VLAN_KEEP_MASK_EXT_MASK;
        bit_mask <<= (port&1)*3;
        retVal = l2sw_getAsicRegBits(units, regAddr, bit_mask, &regval_h);
        if(retVal != L2SW_ERR_OK)
            return retVal;
        *pPortmask = (regval_h << 8) | regval_l;
    }
    else{
        switch(port){
            case 8:
                regAddr = L2SW_REG_VLAN_EGRESS_KEEP_CTRL4;
                bit_mask = L2SW_PORT8_VLAN_KEEP_MASK_MASK;
                retVal = l2sw_getAsicRegBits(units, regAddr, bit_mask, &regval_l);
                if(retVal != L2SW_ERR_OK)
                    return retVal;
                regAddr = L2SW_REG_VLAN_EGRESS_KEEP_CTRL4_EXT;
                bit_mask = L2SW_PORT8_VLAN_KEEP_MASK_EXT_MASK;
                retVal = l2sw_getAsicRegBits(units, regAddr, bit_mask, &regval_h);
                if(retVal != L2SW_ERR_OK)
                    return retVal;

                *pPortmask = (regval_h << 8) | regval_l;
                break;

            case 9:
                regAddr = L2SW_REG_VLAN_EGRESS_KEEP_CTRL4;
                bit_mask = L2SW_PORT9_VLAN_KEEP_MASK_MASK;
                retVal = l2sw_getAsicRegBits(units, regAddr, bit_mask, &regval_l);
                if(retVal != L2SW_ERR_OK)
                    return retVal;
                regAddr = L2SW_REG_VLAN_EGRESS_KEEP_CTRL4_EXT;
                bit_mask = L2SW_PORT9_VLAN_KEEP_MASK_EXT_MASK;
                retVal = l2sw_getAsicRegBits(units, regAddr, bit_mask, &regval_h);
                if(retVal != L2SW_ERR_OK)
                    return retVal;

                *pPortmask = (regval_h << 8) | regval_l;
                break;

            case 10:
                regAddr = L2SW_REG_VLAN_EGRESS_KEEP_CTRL5;
                bit_mask = L2SW_VLAN_EGRESS_KEEP_CTRL5_MASK;
                retVal = l2sw_getAsicRegBits(units, regAddr, bit_mask, &regval_l);
                if(retVal != L2SW_ERR_OK)
                    return retVal;
                regAddr = L2SW_REG_VLAN_EGRESS_KEEP_CTRL5_EXT;
                bit_mask = L2SW_VLAN_EGRESS_KEEP_CTRL5_EXT_MASK;
                retVal = l2sw_getAsicRegBits(units, regAddr, bit_mask, &regval_h);
                if(retVal != L2SW_ERR_OK)
                    return retVal;

                *pPortmask = (regval_h << 8) | regval_l;
                break;
        }
    }

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setReservedVidAction
 * Description:
 *      Set reserved VID action
 * Input:
 *      vid0Action      - VID 0 action
 *      vid4095Action   - VID 4095 action
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_INPUT    - Error input
 * Note:
 *      None
 */
ret_t l2sw_setReservedVidAction(l2sw_uint8 units, l2sw_uint32 vid0Action, l2sw_uint32 vid4095Action)
{
    ret_t   retVal;

    if(vid0Action >= RES_VID_ACT_END)
        return L2SW_ERR_INPUT;

    if(vid4095Action >= RES_VID_ACT_END)
        return L2SW_ERR_INPUT;

    if((retVal = l2sw_setAsicRegBit(units, L2SW_REG_VLAN_EXT_CTRL, L2SW_VLAN_VID0_TYPE_OFFSET, vid0Action)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicRegBit(units, L2SW_REG_VLAN_EXT_CTRL, L2SW_VLAN_VID4095_TYPE_OFFSET, vid4095Action)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getReservedVidAction
 * Description:
 *      Get reserved VID action
 * Input:
 *      pVid0Action     - VID 0 action
 *      pVid4095Action  - VID 4095 action
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_NULL_POINTER - Null pointer
 * Note:
 *      None
 */
ret_t l2sw_getReservedVidAction(l2sw_uint8 units, l2sw_uint32 *pVid0Action, l2sw_uint32 *pVid4095Action)
{
    ret_t   retVal;

    if(pVid0Action == NULL)
        return L2SW_ERR_NULL_POINTER;

    if(pVid4095Action == NULL)
        return L2SW_ERR_NULL_POINTER;

    if((retVal = l2sw_getAsicRegBit(units, L2SW_REG_VLAN_EXT_CTRL, L2SW_VLAN_VID0_TYPE_OFFSET, pVid0Action)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_getAsicRegBit(units, L2SW_REG_VLAN_EXT_CTRL, L2SW_VLAN_VID4095_TYPE_OFFSET, pVid4095Action)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;

}

/* Function Name:
 *      l2sw_setRealKeepRemarkEn
 * Description:
 *      Set Real Keep Remark
 * Input:
 *      enabled         - 0: 1P remarking is forbidden at real keep packet, 1: 1P remarking is enabled at real keep packet
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_INPUT    - Error input
 * Note:
 *      None
 */
ret_t l2sw_setRealKeepRemarkEn(l2sw_uint8 units, l2sw_uint32 enabled)
{
    ret_t   retVal;

    if((retVal = l2sw_setAsicRegBit(units, L2SW_REG_VLAN_EXT_CTRL, L2SW_VLAN_1P_REMARK_BYPASS_REALKEEP_OFFSET, enabled)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getRealKeepRemarkEn
 * Description:
 *      Get Real Keep Remark
 * Input:
 *      None
 * Output:
 *      pEnabled        - 0: 1P remarking is forbidden at real keep packet, 1: 1P remarking is enabled at real keep packet
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_INPUT    - Error input
 * Note:
 *      None
 */
ret_t l2sw_getRealKeepRemarkEn(l2sw_uint8 units, l2sw_uint32 *pEnabled)
{
    ret_t   retVal;

    if((retVal = l2sw_getAsicRegBit(units, L2SW_REG_VLAN_EXT_CTRL, L2SW_VLAN_1P_REMARK_BYPASS_REALKEEP_OFFSET, pEnabled)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_resetVlan
 * Description:
 *      Reset VLAN table
 * Input:
 *      None.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_resetVlan(l2sw_uint8 units)
{
    ret_t   retVal;

    if((retVal = l2sw_setAsicRegBit(units, L2SW_REG_VLAN_EXT_CTRL2, L2SW_VLAN_EXT_CTRL2_OFFSET, 1)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getresetVlan
 * Description:
 *      Show Reset VLAN table
 * Input:
 *      None.
 * Output:
 *      pEnabled     - State of 1p remarking at real keep packet
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getresetVlan(l2sw_uint8 units, l2sw_uint32 *pEnabled)
{
    ret_t   retVal;

    if((retVal = l2sw_getAsicRegBit(units, L2SW_REG_VLAN_EXT_CTRL2, L2SW_VLAN_EXT_CTRL2_OFFSET, pEnabled)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

