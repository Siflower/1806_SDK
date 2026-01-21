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

#include "l2sw_asicdrv_hsb.h"
/* Function Name:
 *      l2sw_setAsicFieldSelector
 * Description:
 *      Set user defined field selectors in HSB
 * Input:
 *      index       - index of field selector 0-15
 *      format      - Format of field selector
 *      offset      - Retrieving data offset
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      System support 16 user defined field selctors.
 *      Each selector can be enabled or disable. User can defined retrieving 16-bits in many predefiend
 *      standard l2/l3/l4 payload.
 */
ret_t l2sw_setAsicFieldSelector(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 format, l2sw_uint32 offset)
{
    l2sw_uint32 regData;

    if(index > L2SW_FIELDSEL_FORMAT_NUMBER)
        return L2SW_ERR_OUT_OF_RANGE;

    if(format >= FIELDSEL_FORMAT_END)
        return L2SW_ERR_OUT_OF_RANGE;

    regData = (((format << L2SW_FIELD_SELECTOR_FORMAT_OFFSET) & L2SW_FIELD_SELECTOR_FORMAT_MASK ) |
               ((offset << L2SW_FIELD_SELECTOR_OFFSET_OFFSET) & L2SW_FIELD_SELECTOR_OFFSET_MASK ));

    return l2sw_setAsicReg(units, L2SW_FIELD_SELECTOR_REG(index), regData);
}
/* Function Name:
 *      l2sw_getAsicFieldSelector
 * Description:
 *      Get user defined field selectors in HSB
 * Input:
 *      index       - index of field selector 0-15
 *      pFormat     - Format of field selector
 *      pOffset     - Retrieving data offset
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicFieldSelector(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pFormat, l2sw_uint32* pOffset)
{
    ret_t retVal;
    l2sw_uint32 regData;

    retVal = l2sw_getAsicReg(units, L2SW_FIELD_SELECTOR_REG(index), &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *pFormat    = ((regData & L2SW_FIELD_SELECTOR_FORMAT_MASK) >> L2SW_FIELD_SELECTOR_FORMAT_OFFSET);
    *pOffset    = ((regData & L2SW_FIELD_SELECTOR_OFFSET_MASK) >> L2SW_FIELD_SELECTOR_OFFSET_OFFSET);

    return L2SW_ERR_OK;
}
