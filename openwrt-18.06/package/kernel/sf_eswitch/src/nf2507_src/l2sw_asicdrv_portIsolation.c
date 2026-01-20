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

#include "l2sw_asicdrv_portIsolation.h"
/* Function Name:
 *      l2sw_setAsicPortIsolationPermittedPortmask
 * Description:
 *      Set permitted port isolation portmask
 * Input:
 *      port            - Physical port number (0~10)
 *      permitPortmask  - port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_PORT_MASK    - Invalid portmask
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortIsolationPermittedPortmask(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 permitPortmask)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    if( permitPortmask > L2SW_PORTMASK)
        return L2SW_ERR_PORT_MASK;

    return l2sw_setAsicReg(units, L2SW_PORT_ISOLATION_PORT_MASK_REG(port), permitPortmask);
}
/* Function Name:
 *      l2sw_getAsicPortIsolationPermittedPortmask
 * Description:
 *      Get permitted port isolation portmask
 * Input:
 *      port                - Physical port number (0~10)
 *      pPermitPortmask     - port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortIsolationPermittedPortmask(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pPermitPortmask)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicReg(units, L2SW_PORT_ISOLATION_PORT_MASK_REG(port), pPermitPortmask);
}
/* Function Name:
 *      l2sw_setAsicPortIsolationEfid
 * Description:
 *      Set port isolation EFID
 * Input:
 *      port    - Physical port number (0~10)
 *      efid    - EFID (0~7)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_OUT_OF_RANGE - Input parameter out of range
 * Note:
 *      EFID is used in individual learning in filtering database
 */
ret_t l2sw_setAsicPortIsolationEfid(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 efid)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    if( efid > L2SW_EFIDMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_setAsicRegBits(units, L2SW_PORT_EFID_REG(port), L2SW_PORT_EFID_MASK(port), efid);
}
/* Function Name:
 *      l2sw_getAsicPortIsolationEfid
 * Description:
 *      Get port isolation EFID
 * Input:
 *      port    - Physical port number (0~10)
 *      pEfid   - EFID (0~7)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortIsolationEfid(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEfid)
{
    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    return l2sw_getAsicRegBits(units, L2SW_PORT_EFID_REG(port), L2SW_PORT_EFID_MASK(port), pEfid);
}

