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

#include "l2sw_asicdrv_oam.h"
/* Function Name:
 *      l2sw_setAsicOamParser
 * Description:
 *      Set OAM parser state
 * Input:
 *      port    - Physical port number (0~7)
 *      parser  - Per-Port OAM parser state
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_NOT_ALLOWED  - Invalid paser state
 * Note:
 *      None
 */
ret_t l2sw_setAsicOamParser(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 parser)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    if(parser > OAM_PARFWDCPU)
        return L2SW_ERR_NOT_ALLOWED;

    return l2sw_setAsicRegBits(units, L2SW_REG_OAM_PARSER_CTRL0 + port/8, L2SW_OAM_PARSER_MASK(port % 8), parser);
}
/* Function Name:
 *      l2sw_getAsicOamParser
 * Description:
 *      Get OAM parser state
 * Input:
 *      port    - Physical port number (0~7)
 *      pParser     - Per-Port OAM parser state
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicOamParser(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pParser)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBits(units, L2SW_REG_OAM_PARSER_CTRL0 + port/8, L2SW_OAM_PARSER_MASK(port%8), pParser);
}
/* Function Name:
 *      l2sw_setAsicOamMultiplexer
 * Description:
 *      Set OAM multiplexer state
 * Input:
 *      port        - Physical port number (0~7)
 *      multiplexer - Per-Port OAM multiplexer state
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_NOT_ALLOWED  - Invalid multiplexer state
 * Note:
 *      None
 */
ret_t l2sw_setAsicOamMultiplexer(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 multiplexer)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    if(multiplexer > OAM_MULCPU)
        return L2SW_ERR_NOT_ALLOWED;

    return l2sw_setAsicRegBits(units, L2SW_REG_OAM_MULTIPLEXER_CTRL0 + port/8, L2SW_OAM_MULTIPLEXER_MASK(port%8), multiplexer);
}
/* Function Name:
 *      l2sw_getAsicOamMultiplexer
 * Description:
 *      Get OAM multiplexer state
 * Input:
 *      port        - Physical port number (0~7)
 *      pMultiplexer - Per-Port OAM multiplexer state
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicOamMultiplexer(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pMultiplexer)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBits(units, L2SW_REG_OAM_MULTIPLEXER_CTRL0 + port/8, L2SW_OAM_MULTIPLEXER_MASK(port%8), pMultiplexer);
}
/* Function Name:
 *      l2sw_setAsicOamCpuPri
 * Description:
 *      Set trap priority for OAM packet
 * Input:
 *      priority    - priority (0~7)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - Success
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_QOS_INT_PRIORITY     - Invalid priority
 * Note:
 *      None
 */
ret_t l2sw_setAsicOamCpuPri(l2sw_uint8 units, l2sw_uint32 priority)
{
    if(priority > L2SW_PRIMAX)
        return L2SW_ERR_QOS_INT_PRIORITY;

    return l2sw_setAsicRegBits(units, L2SW_REG_QOS_TRAP_PRIORITY0, L2SW_OAM_PRIOIRTY_MASK, priority);
}
/* Function Name:
 *      l2sw_getAsicOamCpuPri
 * Description:
 *      Get trap priority for OAM packet
 * Input:
 *      pPriority   - priority (0~7)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicOamCpuPri(l2sw_uint8 units, l2sw_uint32 *pPriority)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_QOS_TRAP_PRIORITY0, L2SW_OAM_PRIOIRTY_MASK, pPriority);
}
/* Function Name:
 *      l2sw_setAsicOamEnable
 * Description:
 *      Set OAM function state
 * Input:
 *      enabled     - OAM function usage 1:enable, 0:disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicOamEnable(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_OAM_CTRL, L2SW_OAM_CTRL_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicOamEnable
 * Description:
 *      Get OAM function state
 * Input:
 *      pEnabled    - OAM function usage 1:enable, 0:disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicOamEnable(l2sw_uint8 units, l2sw_uint32 *pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_OAM_CTRL, L2SW_OAM_CTRL_OFFSET, pEnabled);
}
