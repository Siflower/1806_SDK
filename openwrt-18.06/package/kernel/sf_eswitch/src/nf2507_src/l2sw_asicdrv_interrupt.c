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

#include "l2sw_asicdrv_interrupt.h"
/* Function Name:
 *      l2sw_setAsicInterruptPolarity
 * Description:
 *      Set interrupt trigger polarity
 * Input:
 *      polarity    - 0:pull high 1: pull low
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicInterruptPolarity(l2sw_uint8 units, l2sw_uint32 polarity)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_INTR_CTRL, L2SW_INTR_CTRL_OFFSET, polarity);
}
/* Function Name:
 *      l2sw_getAsicInterruptPolarity
 * Description:
 *      Get interrupt trigger polarity
 * Input:
 *      pPolarity   - 0:pull high 1: pull low
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicInterruptPolarity(l2sw_uint8 units, l2sw_uint32* pPolarity)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_INTR_CTRL, L2SW_INTR_CTRL_OFFSET, pPolarity);
}
/* Function Name:
 *      l2sw_setAsicInterruptMask
 * Description:
 *      Set interrupt enable mask
 * Input:
 *      imr     - Interrupt mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicInterruptMask(l2sw_uint8 units, l2sw_uint32 imr)
{
    return l2sw_setAsicReg(units, L2SW_REG_INTR_IMR, imr);
}
/* Function Name:
 *      l2sw_getAsicInterruptMask
 * Description:
 *      Get interrupt enable mask
 * Input:
 *      pImr    - Interrupt mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicInterruptMask(l2sw_uint8 units, l2sw_uint32* pImr)
{
    return l2sw_getAsicReg(units, L2SW_REG_INTR_IMR, pImr);
}
/* Function Name:
 *      l2sw_setAsicInterruptMask
 * Description:
 *      Clear interrupt enable mask
 * Input:
 *      ims     - Interrupt status mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      This API can be used to clear ASIC interrupt status and register will be cleared by writting 1.
 *      [0]:Link change,
 *      [1]:Share meter exceed,
 *      [2]:Learn number overed,
 *      [3]:Speed Change,
 *      [4]:Tx special congestion
 *      [5]:1 second green feature
 *      [6]:loop detection
 *      [7]:interrupt from 8051
 *      [8]:Cable diagnostic finish
 *      [9]:ACL action interrupt trigger
 *      [11]: Silent Start
 */
ret_t l2sw_setAsicInterruptStatus(l2sw_uint8 units, l2sw_uint32 ims)
{
    return l2sw_setAsicReg(units, L2SW_REG_INTR_IMS, ims);
}
/* Function Name:
 *      l2sw_getAsicInterruptStatus
 * Description:
 *      Get interrupt enable mask
 * Input:
 *      pIms    - Interrupt status mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicInterruptStatus(l2sw_uint8 units, l2sw_uint32* pIms)
{
    return l2sw_getAsicReg(units, L2SW_REG_INTR_IMS, pIms);
}
/* Function Name:
 *      l2sw_setAsicInterruptRelatedStatus
 * Description:
 *      Clear interrupt status
 * Input:
 *      type    - per port Learn over, per-port speed change, per-port special congest, share meter exceed status
 *      status  - exceed status, write 1 to clear
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_setAsicInterruptRelatedStatus(l2sw_uint8 units, l2sw_uint32 type, l2sw_uint32 status)
{
    CONST l2sw_uint32 indicatorAddress[INTRST_END] = {L2SW_REG_LEARN_OVER_INDICATOR,
                                                    L2SW_REG_SPEED_CHANGE_INDICATOR,
                                                    L2SW_REG_SPECIAL_CONGEST_INDICATOR,
                                                    L2SW_REG_PORT_LINKDOWN_INDICATOR,
                                                    L2SW_REG_PORT_LINKUP_INDICATOR,
                                                    L2SW_REG_METER_OVERRATE_INDICATOR0,
                                                    L2SW_REG_METER_OVERRATE_INDICATOR1,
                                                    L2SW_REG_RLDP_LOOPED_INDICATOR,
                                                    L2SW_REG_RLDP_RELEASED_INDICATOR,
                                                    L2SW_REG_SYSTEM_LEARN_OVER_INDICATOR};

    if(type >= INTRST_END )
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_setAsicReg(units, indicatorAddress[type], status);
}
/* Function Name:
 *      l2sw_getAsicInterruptRelatedStatus
 * Description:
 *      Get interrupt status
 * Input:
 *      type    - per port Learn over, per-port speed change, per-port special congest, share meter exceed status
 *      pStatus     - exceed status, write 1 to clear
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t l2sw_getAsicInterruptRelatedStatus(l2sw_uint8 units, l2sw_uint32 type, l2sw_uint32* pStatus)
{
    CONST l2sw_uint32 indicatorAddress[INTRST_END] = {L2SW_REG_LEARN_OVER_INDICATOR,
                                                    L2SW_REG_SPEED_CHANGE_INDICATOR,
                                                    L2SW_REG_SPECIAL_CONGEST_INDICATOR,
                                                    L2SW_REG_PORT_LINKDOWN_INDICATOR,
                                                    L2SW_REG_PORT_LINKUP_INDICATOR,
                                                    L2SW_REG_METER_OVERRATE_INDICATOR0,
                                                    L2SW_REG_METER_OVERRATE_INDICATOR1,
                                                    L2SW_REG_RLDP_LOOPED_INDICATOR,
                                                    L2SW_REG_RLDP_RELEASED_INDICATOR,
                                                    L2SW_REG_SYSTEM_LEARN_OVER_INDICATOR};

    if(type >= INTRST_END )
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_getAsicReg(units, indicatorAddress[type], pStatus);
}

