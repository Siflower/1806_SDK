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

#include "l2sw_asicdrv_acl.h"
#include "l2sw_switch.h"
#include "sal_string.h"
/*
    Exchange structure type define with MMI and SMI
*/
static void _l2sw_aclRuleStSmi2User(l2sw_uint8 units, l2sw_aclrule *pAclUser, l2sw_aclrulesmi *pAclSmi)
{
    l2sw_uint8 *care_ptr, *data_ptr;
    l2sw_uint8 care_tmp, data_tmp;
    l2sw_uint32 i;

    pAclUser->data_bits.active_portmsk = (((pAclSmi->data_bits_ext.rule_info >> 1) & 0x0007) << 8) | ((pAclSmi->data_bits.rule_info >> 8) & 0x00FF);
    pAclUser->data_bits.type = (pAclSmi->data_bits.rule_info & 0x0007);
    pAclUser->data_bits.tag_exist = (pAclSmi->data_bits.rule_info & 0x00F8) >> 3;

    care_ptr = (l2sw_uint8*)&pAclSmi->care_bits;
    data_ptr = (l2sw_uint8*)&pAclSmi->data_bits;

    for ( i = 0; i < sizeof(struct acl_rule_smi_st); i++)
    {
        care_tmp = *(care_ptr + i) ^ (*(data_ptr + i));
        data_tmp = *(data_ptr + i);

        *(care_ptr + i) = care_tmp;
        *(data_ptr + i) = data_tmp;
    }

    care_ptr = (l2sw_uint8*)&pAclSmi->care_bits_ext;
    data_ptr = (l2sw_uint8*)&pAclSmi->data_bits_ext;
    care_tmp = (*care_ptr) ^ (*data_ptr);
    data_tmp = (*data_ptr);
    *care_ptr = care_tmp;
    *data_ptr = data_tmp;

    for(i = 0; i < L2SW_ACLRULEFIELDNO; i++)
        pAclUser->data_bits.field[i] = pAclSmi->data_bits.field[i];

    pAclUser->valid = pAclSmi->valid;

    pAclUser->care_bits.active_portmsk = (((pAclSmi->care_bits_ext.rule_info >> 1) & 0x0007) << 8) | ((pAclSmi->care_bits.rule_info >> 8) & 0x00FF);
    pAclUser->care_bits.type = (pAclSmi->care_bits.rule_info & 0x0007);
    pAclUser->care_bits.tag_exist = (pAclSmi->care_bits.rule_info & 0x00F8) >> 3;

    for(i = 0; i < L2SW_ACLRULEFIELDNO; i++)
        pAclUser->care_bits.field[i] = pAclSmi->care_bits.field[i];
}

/*
    Exchange structure type define with MMI and SMI
*/
static void _l2sw_aclRuleStUser2Smi(l2sw_uint8 units, l2sw_aclrule *pAclUser, l2sw_aclrulesmi *pAclSmi)
{
    l2sw_uint8 *care_ptr, *data_ptr;
    l2sw_uint8 care_tmp, data_tmp;
    l2sw_uint32 i;

    pAclSmi->data_bits_ext.rule_info = ((pAclUser->data_bits.active_portmsk >> 8) & 0x7) << 1;
    pAclSmi->data_bits.rule_info = ((pAclUser->data_bits.active_portmsk & 0xff) << 8) | ((pAclUser->data_bits.tag_exist & 0x1F) << 3) | (pAclUser->data_bits.type & 0x07);

    for(i = 0;i < L2SW_ACLRULEFIELDNO; i++)
        pAclSmi->data_bits.field[i] = pAclUser->data_bits.field[i];

    pAclSmi->valid = pAclUser->valid;

    pAclSmi->care_bits_ext.rule_info = ((pAclUser->care_bits.active_portmsk >> 8) & 0x7) << 1;
    pAclSmi->care_bits.rule_info = ((pAclUser->care_bits.active_portmsk & 0xff) << 8) | ((pAclUser->care_bits.tag_exist & 0x1F) << 3) | (pAclUser->care_bits.type & 0x07);

    for(i = 0; i < L2SW_ACLRULEFIELDNO; i++)
        pAclSmi->care_bits.field[i] = pAclUser->care_bits.field[i];

    care_ptr = (l2sw_uint8*)&pAclSmi->care_bits;
    data_ptr = (l2sw_uint8*)&pAclSmi->data_bits;

    for ( i = 0; i < sizeof(struct acl_rule_smi_st); i++)
    {
        care_tmp = *(care_ptr + i) & ~(*(data_ptr + i));
        data_tmp = *(care_ptr + i) & *(data_ptr + i);

        *(care_ptr + i) = care_tmp;
        *(data_ptr + i) = data_tmp;
    }

    care_ptr = (l2sw_uint8*)&pAclSmi->care_bits_ext;
    data_ptr = (l2sw_uint8*)&pAclSmi->data_bits_ext;
    care_tmp = *care_ptr & ~(*data_ptr);
    data_tmp = *care_ptr & *data_ptr;

    *care_ptr = care_tmp;
    *data_ptr = data_tmp;
}

/*
    Exchange structure type define with MMI and SMI
*/
static void _l2sw_aclActStSmi2User(l2sw_uint8 units, l2sw_acl_act_t *pAclUser, l2sw_uint16 *pAclSmi)
{
    pAclUser->cact = (pAclSmi[0] & 0x00C0) >> 6;
    pAclUser->cvidx_cact = (pAclSmi[0] & 0x003F) | (((pAclSmi[3] & 0x0008) >> 3) << 6);

    pAclUser->sact = (pAclSmi[0] & 0xC000) >> 14;
    pAclUser->svidx_sact = ((pAclSmi[0] & 0x3F00) >> 8) | (((pAclSmi[3] & 0x0010) >> 4) << 6);

    pAclUser->aclmeteridx = (pAclSmi[1] & 0x003F) | (((pAclSmi[3] & 0x0020) >> 5) << 6);

    pAclUser->fwdact = (pAclSmi[1] & 0xC000) >> 14;
    pAclUser->fwdpmask = ((pAclSmi[1] & 0x3FC0) >> 6) | (((pAclSmi[3] & 0x01C0) >> 6) << 8);

    pAclUser->priact = (pAclSmi[2] & 0x00C0) >> 6;
    pAclUser->pridx = (pAclSmi[2] & 0x003F) | (((pAclSmi[3] & 0x0200) >> 9) << 6);

    pAclUser->aclint = (pAclSmi[2] & 0x2000) >> 13;
    pAclUser->gpio_en = (pAclSmi[2] & 0x1000) >> 12;
    pAclUser->gpio_pin = (pAclSmi[2] & 0x0F00) >> 8;

    pAclUser->cact_ext = (pAclSmi[2] & 0xC000) >> 14;
    pAclUser->tag_fmt = (pAclSmi[3] & 0x0003);
    pAclUser->fwdact_ext = (pAclSmi[3] & 0x0004) >> 2;
}

/*
    Exchange structure type define with MMI and SMI
*/
static void _l2sw_aclActStUser2Smi(l2sw_uint8 units, l2sw_acl_act_t *pAclUser, l2sw_uint16 *pAclSmi)
{
    pAclSmi[0] |= (pAclUser->cvidx_cact & 0x003F);
    pAclSmi[0] |= (pAclUser->cact & 0x0003) << 6;
    pAclSmi[0] |= (pAclUser->svidx_sact & 0x003F) << 8;
    pAclSmi[0] |= (pAclUser->sact & 0x0003) << 14;

    pAclSmi[1] |= (pAclUser->aclmeteridx & 0x003F);
    pAclSmi[1] |= (pAclUser->fwdpmask & 0x00FF) << 6;
    pAclSmi[1] |= (pAclUser->fwdact & 0x0003) << 14;

    pAclSmi[2] |= (pAclUser->pridx & 0x003F);
    pAclSmi[2] |= (pAclUser->priact & 0x0003) << 6;
    pAclSmi[2] |= (pAclUser->gpio_pin & 0x000F) << 8;
    pAclSmi[2] |= (pAclUser->gpio_en & 0x0001) << 12;
    pAclSmi[2] |= (pAclUser->aclint & 0x0001) << 13;
    pAclSmi[2] |= (pAclUser->cact_ext & 0x0003) << 14;

    pAclSmi[3] |= (pAclUser->tag_fmt & 0x0003);
    pAclSmi[3] |= (pAclUser->fwdact_ext & 0x0001) << 2;
    pAclSmi[3] |= (pAclUser->aclcounteridx & 0x0003F) << 3;
	#if 0
    pAclSmi[3] |= ((pAclUser->cvidx_cact & 0x0040) >> 6) << 3;
    pAclSmi[3] |= ((pAclUser->svidx_sact & 0x0040) >> 6) << 4;
    pAclSmi[3] |= ((pAclUser->aclmeteridx & 0x0040) >> 6) << 5;
    pAclSmi[3] |= ((pAclUser->fwdpmask & 0x0700) >> 8) << 6;
    pAclSmi[3] |= ((pAclUser->pridx & 0x0040) >> 6) << 9;
	#endif
}

/* Function Name:
 *      l2sw_setAsicAcl
 * Description:
 *      Set port acl function enable/disable
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
ret_t l2sw_setAsicAcl(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    return l2sw_setAsicRegBit(units, L2SW_ACL_ENABLE_REG, port, enabled);
}
/* Function Name:
 *      l2sw_getAsicAcl
 * Description:
 *      Get port acl function enable/disable
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
ret_t l2sw_getAsicAcl(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pEnabled)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBit(units, L2SW_ACL_ENABLE_REG, port, pEnabled);
}
/* Function Name:
 *      l2sw_setAsicAclUnmatchedPermit
 * Description:
 *      Set port acl function unmatched permit action
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
ret_t l2sw_setAsicAclUnmatchedPermit(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    return l2sw_setAsicRegBit(units, L2SW_ACL_UNMATCH_PERMIT_REG, port, enabled);
}
/* Function Name:
 *      l2sw_getAsicAclUnmatchedPermit
 * Description:
 *      Get port acl function unmatched permit action
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
ret_t l2sw_getAsicAclUnmatchedPermit(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pEnabled)
{
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBit(units, L2SW_ACL_UNMATCH_PERMIT_REG, port, pEnabled);
}

/* Function Name:
 *      l2sw_setAsicAclRule
 * Description:
 *      Set acl rule content
 * Input:
 *      index   - ACL rule index (0-95) of 96 ACL rules
 *      pAclRule - ACL rule stucture for setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - Invalid ACL rule index (0-95)
 * Note:
 *      System supported 95 shared 289-bit ACL ingress rule. Index was available at range 0-95 only.
 *      If software want to modify ACL rule, the ACL function should be disable at first or unspecify
 *      acl action will be executed.
 *      One ACL rule structure has three parts setting:
 *      Bit 0-147       Data Bits of this Rule
 *      Bit 148     Valid Bit
 *      Bit 149-296 Care Bits of this Rule
 *      There are four kinds of field in Data Bits and Care Bits: Active Portmask, Type, Tag Exist, and 8 fields
 */
ret_t l2sw_setAsicAclRule(l2sw_uint8 units, l2sw_uint32 index, l2sw_aclrule* pAclRule)
{
    l2sw_aclrulesmi aclRuleSmi;
    l2sw_uint16* tableAddr;
    l2sw_uint32 regAddr;
    l2sw_uint32  regData;
    l2sw_uint32 i;
    ret_t retVal;

    if(index > L2SW_ACLRULEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    MEMSET(&aclRuleSmi, 0x00, sizeof(l2sw_aclrulesmi));

    _l2sw_aclRuleStUser2Smi(units, pAclRule, &aclRuleSmi);


	/* Write valid bit = 0  for databit*/
    regAddr = L2SW_TABLE_ACCESS_ADDR_REG;
#if (l2sw_flag == 1)
/*0-95 for carebit, 96-191 for databit,*/
    regData = L2SW_ACLRULETBADDR_DATABIT(index);
    retVal = l2sw_setAsicReg(units, regAddr,regData);
#else
	if(index >= 64)
        regData = L2SW_ACLRULETBADDR2(DATABITS, index);
    else
        regData = L2SW_ACLRULETBADDR(DATABITS, index);
    retVal = l2sw_setAsicReg(units, regAddr,regData);
#endif
	if(retVal !=L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_TABLE_ACCESS_WRDATA_REG(L2SW_ACLRULETBLEN), 0x1, 0);
    if(retVal !=L2SW_ERR_OK)
        return retVal;

    regAddr = L2SW_TABLE_ACCESS_CTRL_REG;
    regData = L2SW_TABLE_ACCESS_REG_DATA(TB_OP_WRITE, TB_TARGET_ACLRULE);
    retVal = l2sw_setAsicReg(units, regAddr, regData);
    if(retVal !=L2SW_ERR_OK)
        return retVal;
	
#if (l2sw_flag == 1)
/*l2sw need write carebit bit144 to 0(valid==1) or 1(valid==0).*/	
/* Write valid bit = 0   for carebit. */
    regAddr = L2SW_TABLE_ACCESS_ADDR_REG;

	/*0-95 for carebit, 96-191 for databit,*/
	regData = L2SW_ACLRULETBADDR_CAREBIT(index);
	retVal = l2sw_setAsicReg(units, regAddr,regData);
	if(retVal !=L2SW_ERR_OK)
		return retVal;

	retVal = l2sw_setAsicRegBits(units, L2SW_TABLE_ACCESS_WRDATA_REG(L2SW_ACLRULETBLEN), 0x1, 1);
	if(retVal !=L2SW_ERR_OK)
		return retVal;

	regAddr = L2SW_TABLE_ACCESS_CTRL_REG;
	regData = L2SW_TABLE_ACCESS_REG_DATA(TB_OP_WRITE, TB_TARGET_ACLRULE);
	retVal = l2sw_setAsicReg(units, regAddr, regData);
	if(retVal !=L2SW_ERR_OK)
		return retVal;
#endif
	
    /* Write ACS_ADR register */
    regAddr = L2SW_TABLE_ACCESS_ADDR_REG;
#if (l2sw_flag == 1)
	/*0-95 for carebit, 96-191 for databit,*/
    regData = L2SW_ACLRULETBADDR_CAREBIT(index);
    retVal = l2sw_setAsicReg(units, regAddr, regData);
#else
    if(index >= 64)
        regData = L2SW_ACLRULETBADDR2(CAREBITS, index);
    else
        regData = L2SW_ACLRULETBADDR(CAREBITS, index);
    retVal = l2sw_setAsicReg(units, regAddr, regData);
#endif
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /* Write Care Bits to ACS_DATA registers */
    tableAddr = (l2sw_uint16*)&aclRuleSmi.care_bits;
    regAddr = L2SW_TABLE_ACCESS_WRDATA_BASE;

    for(i = 0; i < L2SW_ACLRULETBLEN; i++)
    {
        regData = *tableAddr;
        retVal = l2sw_setAsicReg(units, regAddr, regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        regAddr++;
        tableAddr++;
    }	
/*l2sw need write carebit bit144 to 0(valid==1) or 1(valid==0).*/	
#if (l2sw_flag == 1)
    if (1 == aclRuleSmi.valid)
    {
    	retVal = l2sw_setAsicRegBits(units, L2SW_TABLE_ACCESS_WRDATA_REG(L2SW_ACLRULETBLEN), 0x1, 0);
    }
	else
	{
    	retVal = l2sw_setAsicRegBits(units, L2SW_TABLE_ACCESS_WRDATA_REG(L2SW_ACLRULETBLEN), 0x1, 1);
	}	
    if(retVal !=L2SW_ERR_OK)
        return retVal;
#endif
/*set portlist8-11,l2sw不需要设置 */
#if (l2sw_flag == 0)
    retVal = l2sw_setAsicRegBits(units, L2SW_TABLE_ACCESS_WRDATA_REG(L2SW_ACLRULETBLEN), (0x0007 << 1), (aclRuleSmi.care_bits_ext.rule_info >> 1) & 0x0007);
    if(retVal != L2SW_ERR_OK)
        return retVal;
#endif

    /* Write ACS_CMD register */

    regAddr = L2SW_TABLE_ACCESS_CTRL_REG;
    regData = L2SW_TABLE_ACCESS_REG_DATA(TB_OP_WRITE, TB_TARGET_ACLRULE);
    retVal = l2sw_setAsicRegBits(units, regAddr, L2SW_TABLE_TYPE_MASK | L2SW_COMMAND_TYPE_MASK,regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;



    /* Write ACS_ADR register for data bits */
    regAddr = L2SW_TABLE_ACCESS_ADDR_REG;
#if (l2sw_flag == 1)
		/*0-95 for carebit, 96-191 for databit,*/
		regData = L2SW_ACLRULETBADDR_DATABIT(index);
#else
    if(index >= 64)
        regData = L2SW_ACLRULETBADDR2(DATABITS, index);
    else
        regData = L2SW_ACLRULETBADDR(DATABITS, index);
#endif
    retVal = l2sw_setAsicReg(units, regAddr, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /* Write Data Bits to ACS_DATA registers */
    tableAddr = (l2sw_uint16*)&aclRuleSmi.data_bits;
    regAddr = L2SW_TABLE_ACCESS_WRDATA_BASE;

    for(i = 0; i < L2SW_ACLRULETBLEN; i++)
    {
        regData = *tableAddr;
        retVal = l2sw_setAsicReg(units, regAddr, regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        regAddr++;
        tableAddr++;
    }

    retVal = l2sw_setAsicRegBit(units, L2SW_TABLE_ACCESS_WRDATA_REG(L2SW_ACLRULETBLEN), 0, aclRuleSmi.valid);
    if(retVal != L2SW_ERR_OK)
        return retVal;	
	
	/*set portlist8-11,l2sw不需要设置 */
#if (l2sw_flag == 0)	
    retVal = l2sw_setAsicRegBits(units, L2SW_TABLE_ACCESS_WRDATA_REG(L2SW_ACLRULETBLEN), (0x0007 << 1), (aclRuleSmi.data_bits_ext.rule_info >> 1) & 0x0007);
    if(retVal != L2SW_ERR_OK)
        return retVal;
#endif
    /* Write ACS_CMD register for care bits*/
    regAddr = L2SW_TABLE_ACCESS_CTRL_REG;
    regData = L2SW_TABLE_ACCESS_REG_DATA(TB_OP_WRITE, TB_TARGET_ACLRULE);
    retVal = l2sw_setAsicRegBits(units, regAddr, L2SW_TABLE_TYPE_MASK | L2SW_COMMAND_TYPE_MASK, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicAclRule
 * Description:
 *      Get acl rule content
 * Input:
 *      index   - ACL rule index (0-63) of 64 ACL rules
 *      pAclRule - ACL rule stucture for setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - Invalid ACL rule index (0-63)
  * Note:
 *      None
 */
ret_t l2sw_getAsicAclRule(l2sw_uint8 units, l2sw_uint32 index, l2sw_aclrule *pAclRule)
{
    l2sw_aclrulesmi aclRuleSmi;
    l2sw_uint32 regAddr, regData;
    ret_t retVal;
    l2sw_uint16* tableAddr;
    l2sw_uint32 i, valid_databit, valid_carebit;

    if(index > L2SW_ACLRULEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    MEMSET(&aclRuleSmi, 0x00, sizeof(l2sw_aclrulesmi));

    /* Write ACS_ADR register for data bits */
    regAddr = L2SW_TABLE_ACCESS_ADDR_REG;
	
#if (l2sw_flag == 1)
	/*0-95 for carebit, 96-191 for databit,*/
		regData = L2SW_ACLRULETBADDR_DATABIT(index);
#else
    if(index >= 64)
        regData = L2SW_ACLRULETBADDR2(DATABITS, index);
    else
        regData = L2SW_ACLRULETBADDR(DATABITS, index);
#endif
    retVal = l2sw_setAsicReg(units, regAddr, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;


    /* Write ACS_CMD register */
    regAddr = L2SW_TABLE_ACCESS_CTRL_REG;
    regData = L2SW_TABLE_ACCESS_REG_DATA(TB_OP_READ, TB_TARGET_ACLRULE);
    retVal = l2sw_setAsicRegBits(units, regAddr, L2SW_TABLE_TYPE_MASK | L2SW_COMMAND_TYPE_MASK, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /* Read Data Bits */
    regAddr = L2SW_TABLE_ACCESS_RDDATA_BASE;
    tableAddr = (l2sw_uint16*)&aclRuleSmi.data_bits;
    for(i = 0; i < L2SW_ACLRULETBLEN; i++)
    {
        retVal = l2sw_getAsicReg(units, regAddr, &regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        *tableAddr = regData;

        regAddr ++;
        tableAddr ++;
    }

    /* Read Valid Bit */
    retVal = l2sw_getAsicRegBit(units, L2SW_TABLE_ACCESS_RDDATA_REG(L2SW_ACLRULETBLEN), 0, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;
   
#if (l2sw_flag == 1)
    valid_databit = regData & 0x1;
#else
    aclRuleSmi.valid = regData & 0x1;
#endif

	/*get portlist8-11,l2sw不需要get*/
#if (l2sw_flag == 0)
    /* Read active_portmsk_ext Bits */
    retVal = l2sw_getAsicRegBits(units, L2SW_TABLE_ACCESS_RDDATA_REG(L2SW_ACLRULETBLEN), 0x7<<1, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;
    aclRuleSmi.data_bits_ext.rule_info = (regData % 0x0007) << 1;
#endif

    /* Write ACS_ADR register for carebits*/
    regAddr = L2SW_TABLE_ACCESS_ADDR_REG;
#if (l2sw_flag == 1)
	/*0-95 for carebit, 96-191 for databit,*/
	regData = L2SW_ACLRULETBADDR_CAREBIT(index);
#else
    if(index >= 64)
        regData = L2SW_ACLRULETBADDR2(CAREBITS, index);
    else
        regData = L2SW_ACLRULETBADDR(CAREBITS, index);
#endif
    retVal = l2sw_setAsicReg(units, regAddr, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /* Write ACS_CMD register */
    regAddr = L2SW_TABLE_ACCESS_CTRL_REG;
    regData = L2SW_TABLE_ACCESS_REG_DATA(TB_OP_READ, TB_TARGET_ACLRULE);
    retVal = l2sw_setAsicRegBits(units, regAddr, L2SW_TABLE_TYPE_MASK | L2SW_COMMAND_TYPE_MASK, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /* Read Care Bits */
    regAddr = L2SW_TABLE_ACCESS_RDDATA_BASE;
    tableAddr = (l2sw_uint16*)&aclRuleSmi.care_bits;
    for(i = 0; i < L2SW_ACLRULETBLEN; i++)
    {
        retVal = l2sw_getAsicReg(units, regAddr, &regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        *tableAddr = regData;

        regAddr ++;
        tableAddr ++;
    }	
	
#if (l2sw_flag == 1)
	/* Read Valid Bit */
    retVal = l2sw_getAsicRegBit(units, L2SW_TABLE_ACCESS_RDDATA_REG(L2SW_ACLRULETBLEN), 0, &regData);
	if(retVal != L2SW_ERR_OK)
		return retVal;
	
	valid_carebit = regData & 0x1;
	
	if ((valid_carebit == 0) && (valid_databit == 1))
	{
		aclRuleSmi.valid = 0x1;
	}
	else
	{
		aclRuleSmi.valid = 0x0;
	}
 #endif
	
	/*get portlist8-11,l2sw不需要get*/
#if (l2sw_flag == 0)
    /* Read active_portmsk_ext care Bits */
    retVal = l2sw_getAsicRegBits(units, L2SW_TABLE_ACCESS_RDDATA_REG(L2SW_ACLRULETBLEN), 0x7<<1, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;
	
    aclRuleSmi.care_bits_ext.rule_info = (regData & 0x0007) << 1;
#endif

     _l2sw_aclRuleStSmi2User(units, pAclRule, &aclRuleSmi);

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicAclNot
 * Description:
 *      Set rule comparison result inversion / no inversion
 * Input:
 *      index   - ACL rule index (0-95) of 96 ACL rules
 *      not     - 1: inverse, 0: don't inverse
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - Invalid ACL rule index (0-95)
 * Note:
 *      None
 */
ret_t l2sw_setAsicAclNot(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 not)
{
    if(index > L2SW_ACLRULEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    if(index < 64)
        return l2sw_setAsicRegBit(units, L2SW_ACL_ACTION_CTRL_REG(index), L2SW_ACL_OP_NOT_OFFSET(index), not);
    else
        return l2sw_setAsicRegBit(units, L2SW_ACL_ACTION_CTRL2_REG(index), L2SW_ACL_OP_NOT_OFFSET(index), not);

}
/* Function Name:
 *      l2sw_getAsicAcl
 * Description:
 *      Get rule comparison result inversion / no inversion
 * Input:
 *      index   - ACL rule index (0-95) of 95 ACL rules
 *      pNot    - 1: inverse, 0: don't inverse
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - Invalid ACL rule index (0-95)
 * Note:
 *      None
 */
ret_t l2sw_getAsicAclNot(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pNot)
{
    if(index > L2SW_ACLRULEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    if(index < 64)
        return l2sw_getAsicRegBit(units, L2SW_ACL_ACTION_CTRL_REG(index), L2SW_ACL_OP_NOT_OFFSET(index), pNot);
    else
        return l2sw_getAsicRegBit(units, L2SW_ACL_ACTION_CTRL2_REG(index), L2SW_ACL_OP_NOT_OFFSET(index), pNot);

}
/* Function Name:
 *      l2sw_setAsicAclTemplate
 * Description:
 *      Set fields of a ACL Template
 * Input:
 *      index   - ACL template index(0~4)
 *      pAclType - ACL type stucture for setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - Invalid ACL template index(0~4)
 * Note:
 *      The API can set type field of the 5 ACL rule templates.
 *      Each type has 8 fields. One field means what data in one field of a ACL rule means
 *      8 fields of ACL rule 0~95 is descripted by one type in ACL group
 */
ret_t l2sw_setAsicAclTemplate(l2sw_uint8 units, l2sw_uint32 index, l2sw_acltemplate_t* pAclType)
{
    ret_t retVal;
    l2sw_uint32 i;
    l2sw_uint32 regAddr, regData;

    if(index >= L2SW_ACLTEMPLATENO)
        return L2SW_ERR_OUT_OF_RANGE;

    regAddr = L2SW_ACL_RULE_TEMPLATE_CTRL_REG(index);

    for(i = 0; i < (L2SW_ACLRULEFIELDNO/2); i++)
    {
        regData = pAclType->field[i*2+1];
        regData = regData << 8 | pAclType->field[i*2];
        retVal = l2sw_setAsicReg(units, regAddr + i, regData);

        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicAclTemplate
 * Description:
 *      Get fields of a ACL Template
 * Input:
 *      index   - ACL template index(0~4)
 *      pAclType - ACL type stucture for setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - Invalid ACL template index(0~4)
 * Note:
 *      None
 */
ret_t l2sw_getAsicAclTemplate(l2sw_uint8 units, l2sw_uint32 index, l2sw_acltemplate_t *pAclType)
{
    ret_t retVal;
    l2sw_uint32 i;
    l2sw_uint32 regData, regAddr;

    if(index >= L2SW_ACLTEMPLATENO)
        return L2SW_ERR_OUT_OF_RANGE;

    regAddr = L2SW_ACL_RULE_TEMPLATE_CTRL_REG(index);

    for(i = 0; i < (L2SW_ACLRULEFIELDNO/2); i++)
    {
        retVal = l2sw_getAsicReg(units, regAddr + i,&regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        pAclType->field[i*2] = regData & 0xFF;
        pAclType->field[i*2 + 1] = (regData >> 8) & 0xFF;
    }

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicAclAct
 * Description:
 *      Set ACL rule matched Action
 * Input:
 *      index   - ACL rule index (0-95) of 96 ACL rules
 *      pAclAct     - ACL action stucture for setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - Invalid ACL rule index (0-95)
 * Note:
 *      None
 */
ret_t l2sw_setAsicAclAct(l2sw_uint8 units, l2sw_uint32 index, l2sw_acl_act_t* pAclAct)
{
    l2sw_uint16 aclActSmi[L2SW_ACL_ACT_TABLE_LEN];
    ret_t retVal;
    l2sw_uint32 regAddr, regData;
    l2sw_uint16* tableAddr;
    l2sw_uint32 i;

    if(index > L2SW_ACLRULEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    MEMSET(aclActSmi, 0x00, sizeof(l2sw_uint16) * L2SW_ACL_ACT_TABLE_LEN);
     _l2sw_aclActStUser2Smi(units, pAclAct, aclActSmi);

    /* Write ACS_ADR register for data bits */
    regAddr = L2SW_TABLE_ACCESS_ADDR_REG;
    regData = index;
    retVal = l2sw_setAsicReg(units, regAddr, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /* Write Data Bits to ACS_DATA registers */
    tableAddr = aclActSmi;
    regAddr = L2SW_TABLE_ACCESS_WRDATA_BASE;

    for(i = 0; i < L2SW_ACLACTTBLEN; i++)
    {
        regData = *tableAddr;
        retVal = l2sw_setAsicReg(units, regAddr, regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        regAddr++;
        tableAddr++;
    }

    /* Write ACS_CMD register for care bits*/
    regAddr = L2SW_TABLE_ACCESS_CTRL_REG;
    regData = L2SW_TABLE_ACCESS_REG_DATA(TB_OP_WRITE, TB_TARGET_ACLACT);
    retVal = l2sw_setAsicRegBits(units, regAddr, L2SW_TABLE_TYPE_MASK | L2SW_COMMAND_TYPE_MASK, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicAclAct
 * Description:
 *      Get ACL rule matched Action
 * Input:
 *      index   - ACL rule index (0-95) of 96 ACL rules
 *      pAclAct     - ACL action stucture for setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - Invalid ACL rule index (0-95)
  * Note:
 *      None
 */
ret_t l2sw_getAsicAclAct(l2sw_uint8 units, l2sw_uint32 index, l2sw_acl_act_t *pAclAct)
{
    l2sw_uint16 aclActSmi[L2SW_ACL_ACT_TABLE_LEN];
    ret_t retVal;
    l2sw_uint32 regAddr, regData;
    l2sw_uint16 *tableAddr;
    l2sw_uint32 i;

    if(index > L2SW_ACLRULEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    MEMSET(aclActSmi, 0x00, sizeof(l2sw_uint16) * L2SW_ACL_ACT_TABLE_LEN);

    /* Write ACS_ADR register for data bits */
    regAddr = L2SW_TABLE_ACCESS_ADDR_REG;
    regData = index;
    retVal = l2sw_setAsicReg(units, regAddr, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /* Write ACS_CMD register */
    regAddr = L2SW_TABLE_ACCESS_CTRL_REG;
    regData = L2SW_TABLE_ACCESS_REG_DATA(TB_OP_READ, TB_TARGET_ACLACT);
    retVal = l2sw_setAsicRegBits(units, regAddr, L2SW_TABLE_TYPE_MASK | L2SW_COMMAND_TYPE_MASK, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    /* Read Data Bits */
    regAddr = L2SW_TABLE_ACCESS_RDDATA_BASE;
    tableAddr = aclActSmi;
    for(i = 0; i < L2SW_ACLACTTBLEN; i++)
    {
        retVal = l2sw_getAsicReg(units, regAddr, &regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

        *tableAddr = regData;

        regAddr ++;
        tableAddr ++;
    }

    _l2sw_aclActStSmi2User(units, pAclAct, aclActSmi);

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicAclActCtrl
 * Description:
 *      Set ACL rule matched Action Control Bits
 * Input:
 *      index       - ACL rule index (0-95) of 96 ACL rules
 *      aclActCtrl  - 6 ACL Control Bits
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - Invalid ACL rule index (0-95)
 * Note:
 *      ACL Action Control Bits Indicate which actions will be take when a rule matches
 */
ret_t l2sw_setAsicAclActCtrl(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 aclActCtrl)
{
    ret_t retVal;

    if(index > L2SW_ACLRULEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    if(index >= 64)
        retVal = l2sw_setAsicRegBits(units, L2SW_ACL_ACTION_CTRL2_REG(index), L2SW_ACL_OP_ACTION_MASK(index), aclActCtrl);
    else
        retVal = l2sw_setAsicRegBits(units, L2SW_ACL_ACTION_CTRL_REG(index), L2SW_ACL_OP_ACTION_MASK(index), aclActCtrl);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicAclActCtrl
 * Description:
 *      Get ACL rule matched Action Control Bits
 * Input:
 *      index       - ACL rule index (0-95) of 96 ACL rules
 *      pAclActCtrl     - 6 ACL Control Bits
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - Invalid ACL rule index (0-95)
 * Note:
 *      None
 */
ret_t l2sw_getAsicAclActCtrl(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 *pAclActCtrl)
{
    ret_t retVal;
    l2sw_uint32 regData;

    if(index > L2SW_ACLRULEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    if(index >= 64)
        retVal = l2sw_getAsicRegBits(units, L2SW_ACL_ACTION_CTRL2_REG(index), L2SW_ACL_OP_ACTION_MASK(index), &regData);
    else
        retVal = l2sw_getAsicRegBits(units, L2SW_ACL_ACTION_CTRL_REG(index), L2SW_ACL_OP_ACTION_MASK(index), &regData);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    *pAclActCtrl = regData;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicAclPortRange
 * Description:
 *      Set ACL TCP/UDP range check
 * Input:
 *      index       - TCP/UDP port range check table index
 *      type        - Range check type
 *      upperPort   - TCP/UDP port range upper bound
 *      lowerPort   - TCP/UDP port range lower bound
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - Invalid TCP/UDP port range check table index
 * Note:
 *      None
 */
ret_t l2sw_setAsicAclPortRange(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 type, l2sw_uint32 upperPort, l2sw_uint32 lowerPort)
{
    ret_t retVal;

    if(index > L2SW_ACLRANGEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_ACL_SDPORT_RANGE_ENTRY0_CTRL2 + index*3, L2SW_ACL_SDPORT_RANGE_ENTRY0_CTRL2_MASK, type);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicReg(units, L2SW_REG_ACL_SDPORT_RANGE_ENTRY0_CTRL1 + index*3, upperPort);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicReg(units, L2SW_REG_ACL_SDPORT_RANGE_ENTRY0_CTRL0 + index*3, lowerPort);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicAclPortRange
 * Description:
 *      Get ACL TCP/UDP range check
 * Input:
 *      index       - TCP/UDP port range check table index
 *      pType       - Range check type
 *      pUpperPort  - TCP/UDP port range upper bound
 *      pLowerPort  - TCP/UDP port range lower bound
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - Invalid TCP/UDP port range check table index
 * Note:
 *      None
 */
ret_t l2sw_getAsicAclPortRange(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pType, l2sw_uint32* pUpperPort, l2sw_uint32* pLowerPort)
{
    ret_t retVal;

    if(index > L2SW_ACLRANGEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_ACL_SDPORT_RANGE_ENTRY0_CTRL2 + index*3, L2SW_ACL_SDPORT_RANGE_ENTRY0_CTRL2_MASK, pType);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicReg(units, L2SW_REG_ACL_SDPORT_RANGE_ENTRY0_CTRL1 + index*3, pUpperPort);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicReg(units, L2SW_REG_ACL_SDPORT_RANGE_ENTRY0_CTRL0 + index*3, pLowerPort);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicAclVidRange
 * Description:
 *      Set ACL VID range check
 * Input:
 *      index       - ACL VID range check index(0~15)
 *      type        - Range check type
 *      upperVid    - VID range upper bound
 *      lowerVid    - VID range lower bound
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - Invalid ACL  VID range check index(0~15)
 * Note:
 *      None
 */
ret_t l2sw_setAsicAclVidRange(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 type, l2sw_uint32 upperVid, l2sw_uint32 lowerVid)
{
    ret_t retVal;
    l2sw_uint32 regData;

    if(index > L2SW_ACLRANGEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    regData = ((type << L2SW_ACL_VID_RANGE_ENTRY0_CTRL1_CHECK0_TYPE_OFFSET) & L2SW_ACL_VID_RANGE_ENTRY0_CTRL1_CHECK0_TYPE_MASK) |
                (upperVid & L2SW_ACL_VID_RANGE_ENTRY0_CTRL1_CHECK0_HIGH_MASK);

    retVal = l2sw_setAsicReg(units, L2SW_REG_ACL_VID_RANGE_ENTRY0_CTRL1 + index*2, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicReg(units, L2SW_REG_ACL_VID_RANGE_ENTRY0_CTRL0 + index*2, lowerVid);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicAclVidRange
 * Description:
 *      Get ACL VID range check
 * Input:
 *      index       - ACL VID range check index(0~15)
 *      pType       - Range check type
 *      pUpperVid   - VID range upper bound
 *      pLowerVid   - VID range lower bound
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - Invalid ACL VID range check index(0~15)
 * Note:
 *      None
 */
ret_t l2sw_getAsicAclVidRange(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pType, l2sw_uint32* pUpperVid, l2sw_uint32* pLowerVid)
{
    ret_t retVal;
    l2sw_uint32 regData;

    if(index > L2SW_ACLRANGEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_getAsicReg(units, L2SW_REG_ACL_VID_RANGE_ENTRY0_CTRL1 + index*2, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *pType = (regData & L2SW_ACL_VID_RANGE_ENTRY0_CTRL1_CHECK0_TYPE_MASK) >> L2SW_ACL_VID_RANGE_ENTRY0_CTRL1_CHECK0_TYPE_OFFSET;
    *pUpperVid = regData & L2SW_ACL_VID_RANGE_ENTRY0_CTRL1_CHECK0_HIGH_MASK;

    retVal = l2sw_getAsicReg(units, L2SW_REG_ACL_VID_RANGE_ENTRY0_CTRL0 + index*2, pLowerVid);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicAclIpRange
 * Description:
 *      Set ACL IP range check
 * Input:
 *      index       - ACL IP range check index(0~15)
 *      type        - Range check type
 *      upperIp     - IP range upper bound
 *      lowerIp     - IP range lower bound
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - Invalid ACL IP range check index(0~15)
 * Note:
 *      None
 */
ret_t l2sw_setAsicAclIpRange(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 type, ipaddr_t upperIp, ipaddr_t lowerIp)
{
    ret_t retVal;
    l2sw_uint32 regData;
    ipaddr_t ipData;

    if(index > L2SW_ACLRANGEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicRegBits(units, L2SW_REG_ACL_IP_RANGE_ENTRY0_CTRL4 + index*5, L2SW_ACL_IP_RANGE_ENTRY0_CTRL4_MASK, type);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    ipData = upperIp;

    regData = ipData & 0xFFFF;
    retVal = l2sw_setAsicReg(units, L2SW_REG_ACL_IP_RANGE_ENTRY0_CTRL2 + index*5, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    regData = (ipData>>16) & 0xFFFF;
    retVal = l2sw_setAsicReg(units, L2SW_REG_ACL_IP_RANGE_ENTRY0_CTRL3 + index*5, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    ipData = lowerIp;

    regData = ipData & 0xFFFF;
    retVal = l2sw_setAsicReg(units, L2SW_REG_ACL_IP_RANGE_ENTRY0_CTRL0 + index*5, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    regData = (ipData>>16) & 0xFFFF;
    retVal = l2sw_setAsicReg(units, L2SW_REG_ACL_IP_RANGE_ENTRY0_CTRL1 + index*5, regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicAclIpRange
 * Description:
 *      Get ACL IP range check
 * Input:
 *      index       - ACL IP range check index(0~15)
 *      pType       - Range check type
 *      pUpperIp    - IP range upper bound
 *      pLowerIp    - IP range lower bound
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - Invalid ACL IP range check index(0~15)
 * Note:
 *      None
 */
ret_t l2sw_getAsicAclIpRange(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pType, ipaddr_t* pUpperIp, ipaddr_t* pLowerIp)
{
    ret_t retVal;
    l2sw_uint32 regData;
    ipaddr_t ipData;

    if(index > L2SW_ACLRANGEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_getAsicRegBits(units, L2SW_REG_ACL_IP_RANGE_ENTRY0_CTRL4 + index*5, L2SW_ACL_IP_RANGE_ENTRY0_CTRL4_MASK, pType);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicReg(units, L2SW_REG_ACL_IP_RANGE_ENTRY0_CTRL2 + index*5, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;
    ipData = regData;


    retVal = l2sw_getAsicReg(units, L2SW_REG_ACL_IP_RANGE_ENTRY0_CTRL3 + index*5, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    ipData = (regData <<16) | ipData;
    *pUpperIp = ipData;


    retVal = l2sw_getAsicReg(units, L2SW_REG_ACL_IP_RANGE_ENTRY0_CTRL0 + index*5, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;
    ipData = regData;


    retVal = l2sw_getAsicReg(units, L2SW_REG_ACL_IP_RANGE_ENTRY0_CTRL1 + index*5, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    ipData = (regData << 16) | ipData;
    *pLowerIp = ipData;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicAclGpioPolarity
 * Description:
 *      Set ACL Goip control palarity
 * Input:
 *      polarity - 1: High, 0: Low
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
ret_t l2sw_setAsicAclGpioPolarity(l2sw_uint8 units, l2sw_uint32 polarity)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_ACL_GPIO_POLARITY, L2SW_ACL_GPIO_POLARITY_OFFSET, polarity);
}
/* Function Name:
 *      l2sw_getAsicAclGpioPolarity
 * Description:
 *      Get ACL Goip control palarity
 * Input:
 *      pPolarity - 1: High, 0: Low
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
ret_t l2sw_getAsicAclGpioPolarity(l2sw_uint8 units, l2sw_uint32* pPolarity)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_ACL_GPIO_POLARITY, L2SW_ACL_GPIO_POLARITY_OFFSET, pPolarity);
}

