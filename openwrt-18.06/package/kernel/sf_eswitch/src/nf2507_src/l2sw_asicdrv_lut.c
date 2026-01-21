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

#include "l2sw_asicdrv_lut.h"
#include "sal_string.h"
static void _l2sw_fdbStUser2Smi(l2sw_uint8 units, l2sw_luttb *pLutSt, l2sw_uint16 *pFdbSmi)
{
    /* L3 lookup */
    if(pLutSt->l3lookup)
    {
        if(pLutSt->l3vidlookup)
        {
            pFdbSmi[0] = (pLutSt->sip & 0x0000FFFF);
            pFdbSmi[1] = (pLutSt->sip & 0xFFFF0000) >> 16;

            pFdbSmi[2] = (pLutSt->dip & 0x0000FFFF);
            pFdbSmi[3] = (pLutSt->dip & 0x0FFF0000) >> 16;

            pFdbSmi[3] |= (pLutSt->l3lookup & 0x0001) << 12;
            pFdbSmi[3] |= (pLutSt->l3vidlookup & 0x0001) << 13;
            pFdbSmi[3] |= ((pLutSt->mbr & 0x0300) >> 8) << 14;

            pFdbSmi[4] |= (pLutSt->mbr & 0x00FF);
            pFdbSmi[4] |= (pLutSt->l3_vid & 0x00FF) << 8;

            pFdbSmi[5] |= ((pLutSt->l3_vid & 0x0F00) >> 8);
            pFdbSmi[5] |= (pLutSt->nosalearn & 0x0001) << 5;
            pFdbSmi[5] |= ((pLutSt->mbr & 0x0400) >> 10) << 7;
        }
        else
        {
            pFdbSmi[0] = (pLutSt->sip & 0x0000FFFF);
            pFdbSmi[1] = (pLutSt->sip & 0xFFFF0000) >> 16;

            pFdbSmi[2] = (pLutSt->dip & 0x0000FFFF);
            pFdbSmi[3] = (pLutSt->dip & 0x0FFF0000) >> 16;

            pFdbSmi[3] |= (pLutSt->l3lookup & 0x0001) << 12;
            pFdbSmi[3] |= (pLutSt->l3vidlookup & 0x0001) << 13;
            pFdbSmi[3] |= ((pLutSt->mbr & 0x0300) >> 8) << 14;

            pFdbSmi[4] |= (pLutSt->mbr & 0x00FF);
            pFdbSmi[4] |= (pLutSt->igmpidx & 0x00FF) << 8;

            pFdbSmi[5] |= (pLutSt->igmp_asic & 0x0001);
            pFdbSmi[5] |= (pLutSt->lut_pri & 0x0007) << 1;
            pFdbSmi[5] |= (pLutSt->fwd_en & 0x0001) << 4;
            pFdbSmi[5] |= (pLutSt->nosalearn & 0x0001) << 5;
            pFdbSmi[5] |= ((pLutSt->mbr & 0x0400) >> 10) << 7;
        }
    }
    else if(pLutSt->mac.octet[0] & 0x01) /*Multicast L2 Lookup*/
    {
        pFdbSmi[0] |= pLutSt->mac.octet[5];
        pFdbSmi[0] |= pLutSt->mac.octet[4] << 8;

        pFdbSmi[1] |= pLutSt->mac.octet[3];
        pFdbSmi[1] |= pLutSt->mac.octet[2] << 8;

        pFdbSmi[2] |= pLutSt->mac.octet[1];
        pFdbSmi[2] |= pLutSt->mac.octet[0] << 8;

        pFdbSmi[3] |= pLutSt->cvid_fid;
        pFdbSmi[3] |= (pLutSt->l3lookup & 0x0001) << 12;
        pFdbSmi[3] |= (pLutSt->ivl_svl & 0x0001) << 13;
        pFdbSmi[3] |= ((pLutSt->mbr & 0x0300) >> 8) << 14;

        pFdbSmi[4] |= (pLutSt->mbr & 0x00FF);
        pFdbSmi[4] |= (pLutSt->igmpidx & 0x00FF) << 8;

        pFdbSmi[5] |= pLutSt->igmp_asic;
        pFdbSmi[5] |= (pLutSt->lut_pri & 0x0007) << 1;
        pFdbSmi[5] |= (pLutSt->fwd_en & 0x0001) << 4;
        pFdbSmi[5] |= (pLutSt->nosalearn & 0x0001) << 5;
        pFdbSmi[5] |= ((pLutSt->mbr & 0x0400) >> 10) << 7;
    }
    else /*Asic auto-learning*/
    {
        pFdbSmi[0] |= pLutSt->mac.octet[5];
        pFdbSmi[0] |= pLutSt->mac.octet[4] << 8;

        pFdbSmi[1] |= pLutSt->mac.octet[3];
        pFdbSmi[1] |= pLutSt->mac.octet[2] << 8;

        pFdbSmi[2] |= pLutSt->mac.octet[1];
        pFdbSmi[2] |= pLutSt->mac.octet[0] << 8;

        pFdbSmi[3] |= pLutSt->cvid_fid;
        pFdbSmi[3] |= (pLutSt->l3lookup & 0x0001) << 12;
        pFdbSmi[3] |= (pLutSt->ivl_svl & 0x0001) << 13;
        pFdbSmi[3] |= ((pLutSt->spa & 0x0008) >> 3) << 15;

        pFdbSmi[4] |= pLutSt->efid;
        pFdbSmi[4] |= (pLutSt->fid & 0x000F) << 3;
        pFdbSmi[4] |= (pLutSt->sa_en & 0x0001) << 7;
        pFdbSmi[4] |= (pLutSt->spa & 0x0007) << 8;
        pFdbSmi[4] |= (pLutSt->age & 0x0007) << 11;
        pFdbSmi[4] |= (pLutSt->auth & 0x0001) << 14;
        pFdbSmi[4] |= (pLutSt->sa_block & 0x0001) << 15;

        pFdbSmi[5] |= pLutSt->da_block;
        pFdbSmi[5] |= (pLutSt->lut_pri & 0x0007) << 1;
        pFdbSmi[5] |= (pLutSt->fwd_en & 0x0001) << 4;
        pFdbSmi[5] |= (pLutSt->nosalearn & 0x0001) << 5;
    }
}


static void _l2sw_fdbStSmi2User(l2sw_uint8 units, l2sw_luttb *pLutSt, l2sw_uint16 *pFdbSmi)
{
    /*L3 lookup*/
    if(pFdbSmi[3] & 0x1000)
    {
        if(pFdbSmi[3] & 0x2000)
        {
            pLutSt->sip             = pFdbSmi[0] | (pFdbSmi[1] << 16);
            pLutSt->dip             = 0xE0000000 | pFdbSmi[2] | ((pFdbSmi[3] & 0x0FFF) << 16);

            pLutSt->mbr             = (pFdbSmi[4] & 0x00FF) | (((pFdbSmi[3] & 0xC000) >> 14) << 8) | (((pFdbSmi[5] & 0x0080) >> 7) << 10);
            pLutSt->l3_vid          = ((pFdbSmi[4] & 0xFF00) >> 8) | (pFdbSmi[5] & 0x000F);

            pLutSt->l3lookup        = (pFdbSmi[3] & 0x1000) >> 12;
            pLutSt->l3vidlookup     = (pFdbSmi[3] & 0x2000) >> 13;
            pLutSt->nosalearn       = (pFdbSmi[5] & 0x0020) >> 5;
        }
        else
        {
            pLutSt->sip             = pFdbSmi[0] | (pFdbSmi[1] << 16);
            pLutSt->dip             = 0xE0000000 | pFdbSmi[2] | ((pFdbSmi[3] & 0x0FFF) << 16);

            pLutSt->lut_pri         = (pFdbSmi[5] & 0x000E) >> 1;
            pLutSt->fwd_en          = (pFdbSmi[5] & 0x0010) >> 4;

            pLutSt->mbr             = (pFdbSmi[4] & 0x00FF) | (((pFdbSmi[3] & 0xC000) >> 14) << 8) | (((pFdbSmi[5] & 0x0080) >> 7) << 10);
            pLutSt->igmpidx         = (pFdbSmi[4] & 0xFF00) >> 8;

            pLutSt->igmp_asic       = (pFdbSmi[5] & 0x0001);
            pLutSt->l3lookup        = (pFdbSmi[3] & 0x1000) >> 12;
            pLutSt->nosalearn       = (pFdbSmi[5] & 0x0020) >> 5;
        }
    }
    else if(pFdbSmi[2] & 0x0100) /*Multicast L2 Lookup*/
    {
        pLutSt->mac.octet[0]    = (pFdbSmi[2] & 0xFF00) >> 8;
        pLutSt->mac.octet[1]    = (pFdbSmi[2] & 0x00FF);
        pLutSt->mac.octet[2]    = (pFdbSmi[1] & 0xFF00) >> 8;
        pLutSt->mac.octet[3]    = (pFdbSmi[1] & 0x00FF);
        pLutSt->mac.octet[4]    = (pFdbSmi[0] & 0xFF00) >> 8;
        pLutSt->mac.octet[5]    = (pFdbSmi[0] & 0x00FF);

        pLutSt->cvid_fid        = pFdbSmi[3] & 0x0FFF;
        pLutSt->lut_pri         = (pFdbSmi[5] & 0x000E) >> 1;
        pLutSt->fwd_en          = (pFdbSmi[5] & 0x0010) >> 4;

        pLutSt->mbr             = (pFdbSmi[4] & 0x00FF) | (((pFdbSmi[3] & 0xC000) >> 14) << 8) | (((pFdbSmi[5] & 0x0080) >> 7) << 10);
        pLutSt->igmpidx         = (pFdbSmi[4] & 0xFF00) >> 8;

        pLutSt->igmp_asic       = (pFdbSmi[5] & 0x0001);
        pLutSt->l3lookup        = (pFdbSmi[3] & 0x1000) >> 12;
        pLutSt->ivl_svl         = (pFdbSmi[3] & 0x2000) >> 13;
        pLutSt->nosalearn       = (pFdbSmi[5] & 0x0020) >> 5;
    }
    else /*Asic auto-learning*/
    {
        pLutSt->mac.octet[0]    = (pFdbSmi[2] & 0xFF00) >> 8;
        pLutSt->mac.octet[1]    = (pFdbSmi[2] & 0x00FF);
        pLutSt->mac.octet[2]    = (pFdbSmi[1] & 0xFF00) >> 8;
        pLutSt->mac.octet[3]    = (pFdbSmi[1] & 0x00FF);
        pLutSt->mac.octet[4]    = (pFdbSmi[0] & 0xFF00) >> 8;
        pLutSt->mac.octet[5]    = (pFdbSmi[0] & 0x00FF);

        pLutSt->cvid_fid        = pFdbSmi[3] & 0x0FFF;
        pLutSt->lut_pri         = (pFdbSmi[5] & 0x000E) >> 1;
        pLutSt->fwd_en          = (pFdbSmi[5] & 0x0010) >> 4;

        pLutSt->sa_en           = (pFdbSmi[4] & 0x0080) >> 7;
        pLutSt->auth            = (pFdbSmi[4] & 0x4000) >> 14;
        pLutSt->spa             = ((pFdbSmi[4] & 0x0700) >> 8) | (((pFdbSmi[3] & 0x8000) >> 15) << 3);
        pLutSt->age             = (pFdbSmi[4] & 0x3800) >> 11;
        pLutSt->fid             = (pFdbSmi[4] & 0x0078) >> 3;
        pLutSt->efid            = (pFdbSmi[4] & 0x0007);
        pLutSt->sa_block        = (pFdbSmi[4] & 0x8000) >> 15;

        pLutSt->da_block        = (pFdbSmi[5] & 0x0001);
        pLutSt->l3lookup        = (pFdbSmi[3] & 0x1000) >> 12;
        pLutSt->ivl_svl         = (pFdbSmi[3] & 0x2000) >> 13;
        pLutSt->nosalearn       = (pFdbSmi[5] & 0x0020) >> 5;
    }
}

/* Function Name:
 *      l2sw_setAsicLutIpMulticastLookup
 * Description:
 *      Set Lut IP multicast lookup function
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
ret_t l2sw_setAsicLutIpMulticastLookup(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_LUT_CFG, L2SW_LUT_IPMC_HASH_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicLutIpMulticastLookup
 * Description:
 *      Get Lut IP multicast lookup function
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
ret_t l2sw_getAsicLutIpMulticastLookup(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_LUT_CFG, L2SW_LUT_IPMC_HASH_OFFSET, pEnabled);
}

/* Function Name:
 *      l2sw_setAsicLutIpMulticastLookup
 * Description:
 *      Set Lut IP multicast + VID lookup function
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
ret_t l2sw_setAsicLutIpMulticastVidLookup(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_LUT_CFG2, L2SW_LUT_IPMC_VID_HASH_OFFSET, enabled);
}

/* Function Name:
 *      l2sw_getAsicLutIpMulticastVidLookup
 * Description:
 *      Get Lut IP multicast lookup function
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
ret_t l2sw_getAsicLutIpMulticastVidLookup(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_LUT_CFG2, L2SW_LUT_IPMC_VID_HASH_OFFSET, pEnabled);
}

/* Function Name:
 *      l2sw_setAsicLutIpLookupMethod
 * Description:
 *      Set Lut IP lookup hash with DIP or {DIP,SIP} pair
 * Input:
 *      type - 1: When DIP can be found in IPMC_GROUP_TABLE, use DIP+SIP Hash, otherwise, use DIP+(SIP=0.0.0.0) Hash.
 *             0: When DIP can be found in IPMC_GROUP_TABLE, use DIP+(SIP=0.0.0.0) Hash, otherwise use DIP+SIP Hash.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicLutIpLookupMethod(l2sw_uint8 units, l2sw_uint32 type)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_LUT_CFG, L2SW_LUT_IPMC_LOOKUP_OP_OFFSET, type);
}
/* Function Name:
 *      l2sw_getAsicLutIpLookupMethod
 * Description:
 *      Get Lut IP lookup hash with DIP or {DIP,SIP} pair
 * Input:
 *      pType - 1: When DIP can be found in IPMC_GROUP_TABLE, use DIP+SIP Hash, otherwise, use DIP+(SIP=0.0.0.0) Hash.
 *              0: When DIP can be found in IPMC_GROUP_TABLE, use DIP+(SIP=0.0.0.0) Hash, otherwise use DIP+SIP Hash.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicLutIpLookupMethod(l2sw_uint8 units, l2sw_uint32* pType)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_LUT_CFG, L2SW_LUT_IPMC_LOOKUP_OP_OFFSET, pType);
}
/* Function Name:
 *      l2sw_setAsicLutAgeTimerSpeed
 * Description:
 *      Set LUT agging out speed
 * Input:
 *      timer - Agging out timer 0:Has been aged out
 *      speed - Agging out speed 0-fastest 3-slowest
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicLutAgeTimerSpeed(l2sw_uint8 units, l2sw_uint32 timer, l2sw_uint32 speed)
{
    if(timer>L2SW_LUT_AGETIMERMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    if(speed >L2SW_LUT_AGESPEEDMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_setAsicRegBits(units, L2SW_REG_LUT_CFG, L2SW_AGE_TIMER_MASK | L2SW_AGE_SPEED_MASK, (timer << L2SW_AGE_TIMER_OFFSET) | (speed << L2SW_AGE_SPEED_OFFSET));
}
/* Function Name:
 *      l2sw_getAsicLutAgeTimerSpeed
 * Description:
 *      Get LUT agging out speed
 * Input:
 *      pTimer - Agging out timer 0:Has been aged out
 *      pSpeed - Agging out speed 0-fastest 3-slowest
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_getAsicLutAgeTimerSpeed(l2sw_uint8 units, l2sw_uint32* pTimer, l2sw_uint32* pSpeed)
{
    l2sw_uint32 regData;
    ret_t retVal;

    retVal = l2sw_getAsicReg(units, L2SW_REG_LUT_CFG, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *pTimer =  (regData & L2SW_AGE_TIMER_MASK) >> L2SW_AGE_TIMER_OFFSET;

    *pSpeed =  (regData & L2SW_AGE_SPEED_MASK) >> L2SW_AGE_SPEED_OFFSET;

    return L2SW_ERR_OK;

}
/* Function Name:
 *      l2sw_setAsicLutCamTbUsage
 * Description:
 *      Configure Lut CAM table usage
 * Input:
 *      enabled - L2 CAM table usage 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicLutCamTbUsage(l2sw_uint8 units, l2sw_uint32 enabled)
{
    ret_t retVal;

    retVal = l2sw_setAsicRegBit(units, L2SW_REG_LUT_CFG, L2SW_BCAM_DISABLE_OFFSET, enabled ? 0 : 1);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicLutCamTbUsage
 * Description:
 *      Get Lut CAM table usage
 * Input:
 *      pEnabled - L2 CAM table usage 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicLutCamTbUsage(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    ret_t       retVal;
    l2sw_uint32  regData;

    if ((retVal = l2sw_getAsicRegBit(units, L2SW_REG_LUT_CFG, L2SW_BCAM_DISABLE_OFFSET, &regData)) != L2SW_ERR_OK)
        return retVal;

    *pEnabled = regData ? 0 : 1;
    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicLutLearnLimitNo
 * Description:
 *      Set per-Port auto learning limit number
 * Input:
 *      port    - Physical port number (0~7)
 *      number  - ASIC auto learning entries limit number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number
 *      L2SW_ERR_LIMITED_L2ENTRY_NUM  - Invalid auto learning limit number
 * Note:
 *      None
 */
   /*�޸�: L2SW_PORTIDMAX, L2SW_LUT_LEARNLIMITMAX, L2SW_LUT_PORT_LEARN_LIMITNO_REG*/
ret_t l2sw_setAsicLutLearnLimitNo(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 number)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(number > L2SW_LUT_LEARNLIMITMAX)
        return L2SW_ERR_LIMITED_L2ENTRY_NUM;

    if(port < 8)
     return l2sw_setAsicReg(units, L2SW_LUT_PORT_LEARN_LIMITNO_REG(port), number);
    else
        return l2sw_setAsicReg(units, L2SW_REG_LUT_PORT8_LEARN_LIMITNO+port-8, number);

}
/* Function Name:
 *      l2sw_getAsicLutLearnLimitNo
 * Description:
 *      Get per-Port auto learning limit number
 * Input:
 *      port    - Physical port number (0~7)
 *      pNumber     - ASIC auto learning entries limit number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
  /*�޸�: L2SW_PORTIDMAX, L2SW_LUT_PORT_LEARN_LIMITNO_REG*/
ret_t l2sw_getAsicLutLearnLimitNo(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pNumber)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 8)
     return l2sw_getAsicReg(units, L2SW_LUT_PORT_LEARN_LIMITNO_REG(port), pNumber);
    else
        return l2sw_getAsicReg(units, L2SW_REG_LUT_PORT8_LEARN_LIMITNO+port-8, pNumber);
}

/* Function Name:
 *      l2sw_setAsicSystemLutLearnLimitNo
 * Description:
 *      Set system auto learning limit number
 * Input:
 *      number  - ASIC auto learning entries limit number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number
 *      L2SW_ERR_LIMITED_L2ENTRY_NUM  - Invalid auto learning limit number
 * Note:
 *      None
 */
  /*�޸�: L2SW_LUT_LEARNLIMITMAX*/
ret_t l2sw_setAsicSystemLutLearnLimitNo(l2sw_uint8 units, l2sw_uint32 number)
{
    if(number > L2SW_LUT_LEARNLIMITMAX)
        return L2SW_ERR_LIMITED_L2ENTRY_NUM;

    return l2sw_setAsicReg(units, L2SW_REG_LUT_SYS_LEARN_LIMITNO, number);
}

/* Function Name:
 *      l2sw_getAsicSystemLutLearnLimitNo
 * Description:
 *      Get system auto learning limit number
 * Input:
 *      port    - Physical port number (0~7)
 *      pNumber     - ASIC auto learning entries limit number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicSystemLutLearnLimitNo(l2sw_uint8 units, l2sw_uint32 *pNumber)
{
    if(NULL == pNumber)
        return L2SW_ERR_NULL_POINTER;

    return l2sw_getAsicReg(units, L2SW_REG_LUT_SYS_LEARN_LIMITNO, pNumber);
}

/* Function Name:
 *      l2sw_setAsicLutLearnOverAct
 * Description:
 *      Set auto learn over limit number action
 * Input:
 *      action  - Learn over action 0:normal, 1:drop 2:trap
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_NOT_ALLOWED  - Invalid learn over action
 * Note:
 *      None
 */
ret_t l2sw_setAsicLutLearnOverAct(l2sw_uint8 units, l2sw_uint32 action)
{
    if(action >= LRNOVERACT_END)
        return L2SW_ERR_NOT_ALLOWED;

    return l2sw_setAsicRegBits(units, L2SW_REG_PORT_SECURITY_CTRL, L2SW_LUT_LEARN_OVER_ACT_MASK, action);
}
/* Function Name:
 *      l2sw_getAsicLutLearnOverAct
 * Description:
 *      Get auto learn over limit number action
 * Input:
 *      pAction     - Learn over action 0:normal, 1:drop 2:trap
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicLutLearnOverAct(l2sw_uint8 units, l2sw_uint32* pAction)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_PORT_SECURITY_CTRL, L2SW_LUT_LEARN_OVER_ACT_MASK, pAction);
}

/* Function Name:
 *      l2sw_setAsicSystemLutLearnOverAct
 * Description:
 *      Set system auto learn over limit number action
 * Input:
 *      action  - Learn over action 0:normal, 1:drop, 2:trap
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_NOT_ALLOWED  - Invalid learn over action
 * Note:
 *      None
 */
ret_t l2sw_setAsicSystemLutLearnOverAct(l2sw_uint8 units, l2sw_uint32 action)
{
    if(action >= LRNOVERACT_END)
        return L2SW_ERR_NOT_ALLOWED;

    return l2sw_setAsicRegBits(units, L2SW_REG_LUT_LRN_SYS_LMT_CTRL, L2SW_LUT_SYSTEM_LEARN_OVER_ACT_MASK, action);
}

/* Function Name:
 *      l2sw_getAsicSystemLutLearnOverAct
 * Description:
 *      Get system auto learn over limit number action
 * Input:
 *      pAction     - Learn over action 0:normal, 1:drop 2:trap
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicSystemLutLearnOverAct(l2sw_uint8 units, l2sw_uint32 *pAction)
{
    if(NULL == pAction)
        return L2SW_ERR_NULL_POINTER;

    return l2sw_getAsicRegBits(units, L2SW_REG_LUT_LRN_SYS_LMT_CTRL, L2SW_LUT_SYSTEM_LEARN_OVER_ACT_MASK, pAction);
}

/* Function Name:
 *      l2sw_setAsicSystemLutLearnPortMask
 * Description:
 *      Set system auto learn limit port mask
 * Input:
 *      portmask    - port mask of system learning limit
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_MASK    - Error port mask
 * Note:
 *      None
 */
  /*�޸�: L2SW_LUT_SYSTEM_LEARN_PMASK_MASK*/
ret_t l2sw_setAsicSystemLutLearnPortMask(l2sw_uint8 units, l2sw_uint32 portmask)
{
    ret_t retVal;

    if(portmask > L2SW_PORTMASK)
        return L2SW_ERR_PORT_MASK;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_LUT_LRN_SYS_LMT_CTRL, L2SW_LUT_SYSTEM_LEARN_PMASK_MASK, portmask & 0xff);
    if(retVal != L2SW_ERR_OK)
        return retVal;
    retVal = l2sw_setAsicRegBits(units, L2SW_REG_LUT_LRN_SYS_LMT_CTRL, L2SW_LUT_SYSTEM_LEARN_PMASK1_MASK, (portmask>>8) & 0x7);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;

}

/* Function Name:
 *      l2sw_getAsicSystemLutLearnPortMask
 * Description:
 *      Get system auto learn limit port mask
 * Input:
 *      None
 * Output:
 *      pPortmask   - port mask of system learning limit
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_NULL_POINTER - NULL pointer
 * Note:
 *      None
 */
 /*�޸�: L2SW_LUT_SYSTEM_LEARN_PMASK_MASK*/
ret_t l2sw_getAsicSystemLutLearnPortMask(l2sw_uint8 units, l2sw_uint32 *pPortmask)
{
    l2sw_uint32 tmpmask;
    ret_t retVal;

    if(NULL == pPortmask)
        return L2SW_ERR_NULL_POINTER;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_LUT_LRN_SYS_LMT_CTRL, L2SW_LUT_SYSTEM_LEARN_PMASK_MASK, &tmpmask);
    if(retVal != L2SW_ERR_OK)
        return retVal;
    *pPortmask = tmpmask & 0xff;
    retVal = l2sw_getAsicRegBits(units, L2SW_REG_LUT_LRN_SYS_LMT_CTRL, L2SW_LUT_SYSTEM_LEARN_PMASK1_MASK, &tmpmask);
    if(retVal != L2SW_ERR_OK)
        return retVal;
    *pPortmask |= (tmpmask & 0x7) << 8;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicL2LookupTb
 * Description:
 *      Set filtering database entry
 * Input:
 *      pL2Table    - L2 table entry writing to 8K+64 filtering database
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicL2LookupTb(l2sw_uint8 units, l2sw_luttb *pL2Table)
{
    ret_t retVal;
    l2sw_uint32 regData;
    l2sw_uint16 *accessPtr;
    l2sw_uint32 i;
    l2sw_uint16 smil2Table[L2SW_LUT_TABLE_SIZE];
    l2sw_uint32 tblCmd;
    l2sw_uint32 busyCounter;

    MEMSET(smil2Table, 0x00, sizeof(l2sw_uint16) * L2SW_LUT_TABLE_SIZE);
    _l2sw_fdbStUser2Smi(units, pL2Table, smil2Table);

    if(pL2Table->wait_time == 0)
        busyCounter = L2SW_LUT_BUSY_CHECK_NO;
    else
        busyCounter = pL2Table->wait_time;

    while(busyCounter)
    {
        retVal = l2sw_getAsicRegBit(units, L2SW_TABLE_ACCESS_STATUS_REG, L2SW_TABLE_LUT_ADDR_BUSY_FLAG_OFFSET,&regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        pL2Table->lookup_busy = regData;
        if(!regData)
            break;

        busyCounter --;
        if(busyCounter == 0)
            return L2SW_ERR_BUSYWAIT_TIMEOUT;
    }

    accessPtr = smil2Table;

    for(i = 0; i < L2SW_LUT_ENTRY_SIZE; i++)
    {
        regData = *(accessPtr + i);
        retVal = l2sw_setAsicReg(units, L2SW_TABLE_ACCESS_WRDATA_BASE + i, regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    tblCmd = (L2SW_TABLE_ACCESS_REG_DATA(TB_OP_WRITE,TB_TARGET_L2)) & (L2SW_TABLE_TYPE_MASK  | L2SW_COMMAND_TYPE_MASK);
    /* Write Command */
    retVal = l2sw_setAsicReg(units, L2SW_TABLE_ACCESS_CTRL_REG, tblCmd);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    if(pL2Table->wait_time == 0)
        busyCounter = L2SW_LUT_BUSY_CHECK_NO;
    else
        busyCounter = pL2Table->wait_time;

    while(busyCounter)
    {
        retVal = l2sw_getAsicRegBit(units, L2SW_TABLE_ACCESS_STATUS_REG, L2SW_TABLE_LUT_ADDR_BUSY_FLAG_OFFSET,&regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        pL2Table->lookup_busy = regData;
        if(!regData)
            break;

        busyCounter --;
        if(busyCounter == 0)
            return L2SW_ERR_BUSYWAIT_TIMEOUT;
    }

    /*Read access status*/
    retVal = l2sw_getAsicRegBit(units, L2SW_TABLE_ACCESS_STATUS_REG, L2SW_HIT_STATUS_OFFSET, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    pL2Table->lookup_hit = regData;
    if(!pL2Table->lookup_hit)
        return L2SW_ERR_FAILED;

    /*Read access address*/
    /*
    retVal = l2sw_getAsicRegBits(units, L2SW_TABLE_ACCESS_STATUS_REG, L2SW_TABLE_LUT_ADDR_TYPE_MASK | L2SW_TABLE_LUT_ADDR_ADDRESS_MASK,&regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    pL2Table->address = regData;*/

    retVal = l2sw_getAsicReg(units, L2SW_TABLE_ACCESS_STATUS_REG, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    //pL2Table->address = (regData & 0x7ff) | ((regData & 0x4000) >> 3) | ((regData & 0x800) << 1);
    pL2Table->address = regData & 0xfff;
    pL2Table->lookup_busy = 0;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicL2LookupTb
 * Description:
 *      Get filtering database entry
 * Input:
 *      pL2Table    - L2 table entry writing to 2K+64 filtering database
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_INPUT            - Invalid input parameter
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - LUT is busy at retrieving
 * Note:
 *      None
 */
ret_t l2sw_getAsicL2LookupTb(l2sw_uint8 units, l2sw_uint32 method, l2sw_luttb *pL2Table)
{
    ret_t retVal;
    l2sw_uint32 regData;
    l2sw_uint16* accessPtr;
    l2sw_uint32 i;
    l2sw_uint16 smil2Table[L2SW_LUT_TABLE_SIZE];
    l2sw_uint32 busyCounter;
    l2sw_uint32 tblCmd;

    if(pL2Table->wait_time == 0)
        busyCounter = L2SW_LUT_BUSY_CHECK_NO;
    else
        busyCounter = pL2Table->wait_time;

    while(busyCounter)
    {
        retVal = l2sw_getAsicRegBit(units, L2SW_TABLE_ACCESS_STATUS_REG, L2SW_TABLE_LUT_ADDR_BUSY_FLAG_OFFSET,&regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        pL2Table->lookup_busy = regData;
        if(!pL2Table->lookup_busy)
            break;

        busyCounter --;
        if(busyCounter == 0)
            return L2SW_ERR_BUSYWAIT_TIMEOUT;
    }


    tblCmd = (method << L2SW_ACCESS_METHOD_OFFSET) & L2SW_ACCESS_METHOD_MASK;

    switch(method)
    {
        case LUTREADMETHOD_ADDRESS:
        case LUTREADMETHOD_NEXT_ADDRESS:
        case LUTREADMETHOD_NEXT_L2UC:
        case LUTREADMETHOD_NEXT_L2MC:
        case LUTREADMETHOD_NEXT_L3MC:
        case LUTREADMETHOD_NEXT_L2L3MC:
            retVal = l2sw_setAsicReg(units, L2SW_TABLE_ACCESS_ADDR_REG, pL2Table->address);
            if(retVal != L2SW_ERR_OK)
                return retVal;
            break;
        case LUTREADMETHOD_MAC:
            MEMSET(smil2Table, 0x00, sizeof(l2sw_uint16) * L2SW_LUT_TABLE_SIZE);
            _l2sw_fdbStUser2Smi(units, pL2Table, smil2Table);

            accessPtr = smil2Table;
            regData = *accessPtr;
            for(i=0; i<L2SW_LUT_ENTRY_SIZE; i++)
            {
                retVal = l2sw_setAsicReg(units, L2SW_TABLE_ACCESS_WRDATA_BASE + i, regData);
                if(retVal != L2SW_ERR_OK)
                    return retVal;

                accessPtr ++;
                regData = *accessPtr;

            }
            break;
        case LUTREADMETHOD_NEXT_L2UCSPA:
            retVal = l2sw_setAsicReg(units, L2SW_TABLE_ACCESS_ADDR_REG, pL2Table->address);
            if(retVal != L2SW_ERR_OK)
                return retVal;

            tblCmd = tblCmd | ((pL2Table->spa << L2SW_TABLE_ACCESS_CTRL_SPA_OFFSET) & L2SW_TABLE_ACCESS_CTRL_SPA_MASK);

            break;
        default:
            return L2SW_ERR_INPUT;
    }

    tblCmd = tblCmd | ((L2SW_TABLE_ACCESS_REG_DATA(TB_OP_READ,TB_TARGET_L2)) & (L2SW_TABLE_TYPE_MASK  | L2SW_COMMAND_TYPE_MASK));
    /* Read Command */
    retVal = l2sw_setAsicReg(units, L2SW_TABLE_ACCESS_CTRL_REG, tblCmd);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    if(pL2Table->wait_time == 0)
        busyCounter = L2SW_LUT_BUSY_CHECK_NO;
    else
        busyCounter = pL2Table->wait_time;

    while(busyCounter)
    {
        retVal = l2sw_getAsicRegBit(units, L2SW_TABLE_ACCESS_STATUS_REG, L2SW_TABLE_LUT_ADDR_BUSY_FLAG_OFFSET,&regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        pL2Table->lookup_busy = regData;
        if(!pL2Table->lookup_busy)
            break;

        busyCounter --;
        if(busyCounter == 0)
            return L2SW_ERR_BUSYWAIT_TIMEOUT;
    }

    retVal = l2sw_getAsicRegBit(units, L2SW_TABLE_ACCESS_STATUS_REG, L2SW_HIT_STATUS_OFFSET,&regData);
    if(retVal != L2SW_ERR_OK)
            return retVal;
    pL2Table->lookup_hit = regData;
    if(!pL2Table->lookup_hit)
        return L2SW_ERR_L2_ENTRY_NOTFOUND;

    /*Read access address*/
    //retVal = l2sw_getAsicRegBits(units, L2SW_TABLE_ACCESS_STATUS_REG, L2SW_TABLE_LUT_ADDR_TYPE_MASK | L2SW_TABLE_LUT_ADDR_ADDRESS_MASK,&regData);
    retVal = l2sw_getAsicReg(units, L2SW_TABLE_ACCESS_STATUS_REG, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    //pL2Table->address = (regData & 0x7ff) | ((regData & 0x4000) >> 3) | ((regData & 0x800) << 1);
    pL2Table->address = regData & 0xfff;

    /*read L2 entry */
    MEMSET(smil2Table, 0x00, sizeof(l2sw_uint16) * L2SW_LUT_TABLE_SIZE);

    accessPtr = smil2Table;

    for(i = 0; i < L2SW_LUT_ENTRY_SIZE; i++)
    {
        retVal = l2sw_getAsicReg(units, L2SW_TABLE_ACCESS_RDDATA_BASE + i, &regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        *accessPtr = regData;

        accessPtr ++;
    }

    _l2sw_fdbStSmi2User(units, pL2Table, smil2Table);

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicLutLearnLimitPerSec
 * Description:
 *      Get per-Port auto learning number per-Second
 * Input:
 *      port      - Physical port number (0~7)
 *      enabled   - Port Status
 *      number    - ASIC auto learning entries number per-Seconds
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
 /*�޸�L2SW_PORTIDMAX, L2SW_REG_L2_LRN_CNT_REG, port10 reg is not contnious, wait for updating of base.h*/
ret_t l2sw_setAsicLutLearnLimitPerSec(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled, l2sw_uint32 number)
{
    ret_t retVal;

    if(port > L2SW_PORTIDMAX - 3)
        return L2SW_ERR_PORT_ID;

    retVal = l2sw_setAsicRegBit(units, L2SW_REG_LEARN_SPEED_DISABLE, port, enabled);
    if (retVal != L2SW_ERR_OK)
        return retVal;    
    
    if(port < 8)
    {
        retVal = l2sw_setAsicReg(units, L2SW_LUT_PORT_LEARN_LIMIT_REG(port), number);
        if (retVal != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicLutLearnLimitPerSec
 * Description:
 *      Get per-Port auto learning number
 * Input:
 *      port      - Physical port number (0~7)
 * Output:
 *      pEnabled  - Port Status
 *      pNumber   - ASIC auto learning entries number per-Second
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
 /*�޸�L2SW_PORTIDMAX, L2SW_REG_L2_LRN_CNT_REG, port10 reg is not contnious, wait for updating of base.h*/
ret_t l2sw_getAsicLutLearnLimitPerSec(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnabled, l2sw_uint32 *pNumber)
{
    ret_t retVal;
    
    if(port > L2SW_PORTIDMAX - 3)
        return L2SW_ERR_PORT_ID;

    retVal = l2sw_getAsicRegBit(units, L2SW_REG_LEARN_SPEED_DISABLE, port, pEnabled);
    if (retVal != L2SW_ERR_OK)
        return retVal;    
    
    retVal = l2sw_getAsicReg(units, L2SW_LUT_PORT_LEARN_LIMIT_REG(port), pNumber);
    if (retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicLutLearnNo
 * Description:
 *      Get per-Port auto learning number
 * Input:
 *      port    - Physical port number (0~7)
 *      pNumber     - ASIC auto learning entries number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
 /*�޸�L2SW_PORTIDMAX, L2SW_REG_L2_LRN_CNT_REG, port10 reg is not contnious, wait for updating of base.h*/
ret_t l2sw_getAsicLutLearnNo(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pNumber)
{
    ret_t retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(port < 10)
    {
     retVal = l2sw_getAsicReg(units, L2SW_REG_L2_LRN_CNT_REG(port), pNumber);
        if (retVal != L2SW_ERR_OK)
         return retVal;
    }
    else
    {
        retVal = l2sw_getAsicReg(units, L2SW_REG_L2_LRN_CNT_CTRL10, pNumber);
        if (retVal != L2SW_ERR_OK)
         return retVal;
    }

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicLutFlushAll
 * Description:
 *      Flush all entries in LUT. Includes static & dynamic entries
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicLutFlushAll(l2sw_uint8 units)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_L2_FLUSH_CTRL3, L2SW_L2_FLUSH_CTRL3_OFFSET, 1);
}

/* Function Name:
 *      l2sw_getAsicLutFlushAllStatus
 * Description:
 *      Get Flush all status, 1:Busy, 0 normal
 * Input:
 *      None
 * Output:
 *      pBusyStatus - Busy state
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_NULL_POINTER - Null pointer
 * Note:
 *      None
 */
ret_t l2sw_getAsicLutFlushAllStatus(l2sw_uint8 units, l2sw_uint32 *pBusyStatus)
{
    if(NULL == pBusyStatus)
        return L2SW_ERR_NULL_POINTER;

    return l2sw_getAsicRegBit(units, L2SW_REG_L2_FLUSH_CTRL3, L2SW_L2_FLUSH_CTRL3_OFFSET, pBusyStatus);
}

/* Function Name:
 *      l2sw_setAsicLutForceFlush
 * Description:
 *      Set per port force flush setting
 * Input:
 *      portmask    - portmask(0~0xFF)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_MASK    - Invalid portmask
 * Note:
 *      None
 */
 /*port8~port10������������һ��register, wait for updating of base.h, reg.h*/
ret_t l2sw_setAsicLutForceFlush(l2sw_uint8 units, l2sw_uint32 portmask)
{
    ret_t retVal;

    if(portmask > L2SW_PORTMASK)
        return L2SW_ERR_PORT_MASK;

    retVal = l2sw_setAsicRegBits(units, L2SW_FORCE_FLUSH_REG, L2SW_FORCE_FLUSH_PORTMASK_MASK, portmask & 0xff);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_FORCE_FLUSH1, L2SW_PORTMASK1_MASK, (portmask >> 8) & 0x7);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicLutForceFlushStatus
 * Description:
 *      Get per port force flush status
 * Input:
 *      pPortmask   - portmask(0~0xFF)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
 /*port8~port10������������һ��register, wait for updating of base.h, reg.h*/
ret_t l2sw_getAsicLutForceFlushStatus(l2sw_uint8 units, l2sw_uint32 *pPortmask)
{
    l2sw_uint32 tmpMask;
    ret_t retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_FORCE_FLUSH_REG, L2SW_BUSY_STATUS_MASK,&tmpMask);
    if(retVal != L2SW_ERR_OK)
        return retVal;
    *pPortmask = tmpMask & 0xff;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_FORCE_FLUSH1, L2SW_BUSY_STATUS1_MASK,&tmpMask);
    if(retVal != L2SW_ERR_OK)
        return retVal;
    *pPortmask |= (tmpMask & 7) << 8;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicLutFlushMode
 * Description:
 *      Set user force L2 pLutSt table flush mode
 * Input:
 *      mode    - 0:Port based 1: Port + VLAN based 2:Port + FID/MSTI based
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_NOT_ALLOWED  - Actions not allowed by the function
 * Note:
 *      None
 */
ret_t l2sw_setAsicLutFlushMode(l2sw_uint8 units, l2sw_uint32 mode)
{
    if( mode >= FLUSHMDOE_END )
        return L2SW_ERR_NOT_ALLOWED;

    return l2sw_setAsicRegBits(units, L2SW_REG_L2_FLUSH_CTRL2, L2SW_LUT_FLUSH_MODE_MASK, mode);
}
/* Function Name:
 *      l2sw_getAsicLutFlushMode
 * Description:
 *      Get user force L2 pLutSt table flush mode
 * Input:
 *      pMode   - 0:Port based 1: Port + VLAN based 2:Port + FID/MSTI based
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicLutFlushMode(l2sw_uint8 units, l2sw_uint32* pMode)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_L2_FLUSH_CTRL2, L2SW_LUT_FLUSH_MODE_MASK, pMode);
}
/* Function Name:
 *      l2sw_setAsicLutFlushType
 * Description:
 *      Get L2 LUT flush type
 * Input:
 *      type    - 0: dynamice unicast; 1: both dynamic and static unicast entry
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicLutFlushType(l2sw_uint8 units, l2sw_uint32 type)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_L2_FLUSH_CTRL2, L2SW_LUT_FLUSH_TYPE_OFFSET,type);
}
/* Function Name:
 *      l2sw_getAsicLutFlushType
 * Description:
 *      Set L2 LUT flush type
 * Input:
 *      pType   - 0: dynamice unicast; 1: both dynamic and static unicast entry
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicLutFlushType(l2sw_uint8 units, l2sw_uint32* pType)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_L2_FLUSH_CTRL2, L2SW_LUT_FLUSH_TYPE_OFFSET,pType);
}


/* Function Name:
 *      l2sw_setAsicLutFlushVid
 * Description:
 *      Set VID of Port + VID pLutSt flush mode
 * Input:
 *      vid     - Vid (0~4095)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_VLAN_VID - Invalid VID parameter (0~4095)
 * Note:
 *      None
 */
ret_t l2sw_setAsicLutFlushVid(l2sw_uint8 units, l2sw_uint32 vid)
{
    if( vid > L2SW_VIDMAX )
        return L2SW_ERR_VLAN_VID;
    
    return l2sw_setAsicRegBits(units, L2SW_REG_L2_FLUSH_CTRL1, L2SW_LUT_FLUSH_VID_MASK, vid);
}
/* Function Name:
 *      l2sw_getAsicLutFlushVid
 * Description:
 *      Get VID of Port + VID pLutSt flush mode
 * Input:
 *      pVid    - Vid (0~4095)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicLutFlushVid(l2sw_uint8 units, l2sw_uint32* pVid)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_L2_FLUSH_CTRL1, L2SW_LUT_FLUSH_VID_MASK, pVid);
}
/* Function Name:
 *      l2sw_setAsicPortFlusdFid
 * Description:
 *      Set FID of Port + FID pLutSt flush mode
 * Input:
 *      fid     - FID/MSTI for force flush
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_L2_FID   - Invalid FID (0~15)
 * Note:
 *      None
 */
ret_t l2sw_setAsicLutFlushFid(l2sw_uint8 units, l2sw_uint32 fid)
{
    if( fid > L2SW_FIDMAX )
        return L2SW_ERR_L2_FID;

    return l2sw_setAsicRegBits(units, L2SW_REG_L2_FLUSH_CTRL1, L2SW_LUT_FLUSH_FID_MASK, fid);
}
/* Function Name:
 *      l2sw_getAsicLutFlushFid
 * Description:
 *      Get FID of Port + FID pLutSt flush mode
 * Input:
 *      pFid    - FID/MSTI for force flush
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicLutFlushFid(l2sw_uint8 units, l2sw_uint32* pFid)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_L2_FLUSH_CTRL1, L2SW_LUT_FLUSH_FID_MASK, pFid);
}
/* Function Name:
 *      l2sw_setAsicLutDisableAging
 * Description:
 *      Set L2 LUT aging per port setting
 * Input:
 *      port    - Physical port number (0~7)
 *      disabled    - 0: enable aging; 1: disabling aging
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
 /*�޸�L2SW_PORTIDMAX*/
ret_t l2sw_setAsicLutDisableAging(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 disabled)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;
    
    return l2sw_setAsicRegBit(units, L2SW_LUT_AGEOUT_CTRL_REG, port, disabled);
}
/* Function Name:
 *      l2sw_getAsicLutDisableAging
 * Description:
 *      Get L2 LUT aging per port setting
 * Input:
 *      port    - Physical port number (0~7)
 *      pDisabled - 0: enable aging; 1: disabling aging
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
 /*�޸�L2SW_PORTIDMAX*/
ret_t l2sw_getAsicLutDisableAging(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pDisabled)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBit(units, L2SW_LUT_AGEOUT_CTRL_REG, port, pDisabled);
}

/* Function Name:
 *      l2sw_setAsicLutIPMCGroup
 * Description:
 *      Set IPMC Group Table
 * Input:
 *      index       - the entry index in table (0 ~ 63)
 *      group_addr  - the multicast group address (224.0.0.0 ~ 239.255.255.255)
 *      vid         - VLAN ID
 *      pmask       - portmask
 *      valid       - valid bit
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_INPUT    - Invalid parameter
 * Note:
 *      None
 */
ret_t l2sw_setAsicLutIPMCGroup(l2sw_uint8 units, l2sw_uint32 index, ipaddr_t group_addr, l2sw_uint32 vid, l2sw_uint32 pmask, l2sw_uint32 valid)
{
    l2sw_uint32  regAddr, regData, bitoffset;
    ipaddr_t    ipData;
    ret_t       retVal;

    if(index > L2SW_LUT_IPMCGRP_TABLE_MAX)
        return L2SW_ERR_INPUT;

    if (vid > L2SW_VIDMAX)
        return L2SW_ERR_VLAN_VID;

    ipData = group_addr;

    if( (ipData & 0xF0000000) != 0xE0000000)    /* not in 224.0.0.0 ~ 239.255.255.255 */
        return L2SW_ERR_INPUT;

    /* Group Address */
    regAddr = L2SW_REG_IPMC_GROUP_ENTRY0_H + (index * 2);
    regData = ((ipData & 0x0FFFFFFF) >> 16);

    if( (retVal = l2sw_setAsicReg(units, regAddr, regData)) != L2SW_ERR_OK)
        return retVal;

    regAddr++;
    regData = (ipData & 0x0000FFFF);

    if( (retVal = l2sw_setAsicReg(units, regAddr, regData)) != L2SW_ERR_OK)
        return retVal;

    /* VID */
    regAddr = L2SW_REG_IPMC_GROUP_VID_00 + index;
    regData = vid;

    if( (retVal = l2sw_setAsicReg(units, regAddr, regData)) != L2SW_ERR_OK)
        return retVal;

    /* portmask */
    regAddr = L2SW_REG_IPMC_GROUP_PMSK_00 + index;
    regData = pmask;

    if( (retVal = l2sw_setAsicReg(units, regAddr, regData)) != L2SW_ERR_OK)
        return retVal;

    /* valid */
    regAddr = L2SW_REG_IPMC_GROUP_VALID_15_0 + (index / 16);
    bitoffset = index % 16;
    if( (retVal = l2sw_setAsicRegBit(units, regAddr, bitoffset, valid)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_getAsicLutIPMCGroup
 * Description:
 *      Set IPMC Group Table
 * Input:
 *      index       - the entry index in table (0 ~ 63)
 * Output:
 *      pGroup_addr - the multicast group address (224.0.0.0 ~ 239.255.255.255)
 *      pVid        - VLAN ID
 *      pPmask      - portmask
 *      pValid      - Valid bit
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_INPUT    - Invalid parameter
 * Note:
 *      None
 */
ret_t l2sw_getAsicLutIPMCGroup(l2sw_uint8 units, l2sw_uint32 index, ipaddr_t *pGroup_addr, l2sw_uint32 *pVid, l2sw_uint32 *pPmask, l2sw_uint32 *pValid)
{
    l2sw_uint32      regAddr, regData, bitoffset;
    ipaddr_t    ipData;
    ret_t       retVal;

    if(index > L2SW_LUT_IPMCGRP_TABLE_MAX)
        return L2SW_ERR_INPUT;

    if (NULL == pGroup_addr)
        return L2SW_ERR_NULL_POINTER;

    if (NULL == pVid)
        return L2SW_ERR_NULL_POINTER;

    if (NULL == pPmask)
        return L2SW_ERR_NULL_POINTER;

    /* Group address */
    regAddr = L2SW_REG_IPMC_GROUP_ENTRY0_H + (index * 2);
    if( (retVal = l2sw_getAsicReg(units, regAddr, &regData)) != L2SW_ERR_OK)
        return retVal;

    *pGroup_addr = (((regData & 0x00000FFF) << 16) | 0xE0000000);

    regAddr++;
    if( (retVal = l2sw_getAsicReg(units, regAddr, &regData)) != L2SW_ERR_OK)
        return retVal;

    ipData = (*pGroup_addr | (regData & 0x0000FFFF));
    *pGroup_addr = ipData;

    /* VID */
    regAddr = L2SW_REG_IPMC_GROUP_VID_00 + index;
    if( (retVal = l2sw_getAsicReg(units, regAddr, &regData)) != L2SW_ERR_OK)
        return retVal;

    *pVid = regData;

    /* portmask */
    regAddr = L2SW_REG_IPMC_GROUP_PMSK_00 + index;
    if( (retVal = l2sw_getAsicReg(units, regAddr, &regData)) != L2SW_ERR_OK)
        return retVal;

    *pPmask = regData;

    /* valid */
    regAddr = L2SW_REG_IPMC_GROUP_VALID_15_0 + (index / 16);
    bitoffset = index % 16;
    if( (retVal = l2sw_getAsicRegBit(units, regAddr, bitoffset, &regData)) != L2SW_ERR_OK)
        return retVal;

    *pValid = regData;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicLutLinkDownForceAging
 * Description:
 *       Set LUT link down aging setting.
 * Input:
 *      enable      - link down aging setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_ENABLE    - Invalid parameter
 * Note:
 *      None
 */
ret_t l2sw_setAsicLutLinkDownForceAging(l2sw_uint8 units, l2sw_uint32 enable)
{
    if(enable > 1)
        return L2SW_ERR_ENABLE;

    return l2sw_setAsicRegBit(units, L2SW_REG_LUT_CFG, L2SW_LINKDOWN_AGEOUT_OFFSET, enable ? 0 : 1);
}

/* Function Name:
 *      l2sw_getAsicLutLinkDownForceAging
 * Description:
 *       Get LUT link down aging setting.
 * Input:
 *      pEnable         - link down aging setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_ENABLE    - Invalid parameter
 * Note:
 *      None
 */
ret_t l2sw_getAsicLutLinkDownForceAging(l2sw_uint8 units, l2sw_uint32 *pEnable)
{
    l2sw_uint32  value;
    ret_t   retVal;

    if ((retVal = l2sw_getAsicRegBit(units, L2SW_REG_LUT_CFG, L2SW_LINKDOWN_AGEOUT_OFFSET, &value)) != L2SW_ERR_OK)
        return retVal;

    *pEnable = value ? 0 : 1;
    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicLutIpmcFwdRouterPort
 * Description:
 *       Set IPMC packet forward to rounter port also or not
 * Input:
 *      enable      - 1: Inlcude router port, 0, exclude router port
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_ENABLE     Invalid parameter
 * Note:
 *      None
 */
ret_t l2sw_setAsicLutIpmcFwdRouterPort(l2sw_uint8 units, l2sw_uint32 enable)
{
    if(enable > 1)
        return L2SW_ERR_ENABLE;

    return l2sw_setAsicRegBit(units, L2SW_REG_LUT_CFG2, L2SW_LUT_IPMC_FWD_RPORT_OFFSET, enable);
}

/* Function Name:
 *      l2sw_getAsicLutIpmcFwdRouterPort
 * Description:
 *       Get IPMC packet forward to rounter port also or not
 * Input:
 *      None
 * Output:
 *      pEnable         - 1: Inlcude router port, 0, exclude router port
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_NULL_POINTER     - Null pointer
 * Note:
 *      None
 */
ret_t l2sw_getAsicLutIpmcFwdRouterPort(l2sw_uint8 units, l2sw_uint32 *pEnable)
{
    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    return l2sw_getAsicRegBit(units, L2SW_REG_LUT_CFG2, L2SW_LUT_IPMC_FWD_RPORT_OFFSET, pEnable);
}

