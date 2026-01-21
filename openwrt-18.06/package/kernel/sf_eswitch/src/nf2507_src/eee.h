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

#ifndef __L2SW_API_EEE_H__
#define __L2SW_API_EEE_H__


/* Function Name:
 *      l2sw_eee_init
 * Description:
 *      EEE function initialization.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 *      This API is used to initialize EEE status.
 */
extern l2sw_api_ret_t l2sw_eee_init(l2sw_uint8 units);

/* Function Name:
 *      l2sw_eee_portEnable_set
 * Description:
 *      Set enable status of EEE function.
 * Input:
 *      port - port id.
 *      enable - enable EEE status.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_PORT_ID - Invalid port number.
 *      L2SW_ERR_ENABLE - Invalid enable input.
 * Note:
 *      This API can set EEE function to the specific port.
 *      The configuration of the port is as following:
 *      - DISABLE
 *      - ENABLE
 */
extern l2sw_api_ret_t l2sw_eee_portEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_eee_portEnable_get
 * Description:
 *      Get port admin configuration of the specific port.
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - Back pressure status.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_PORT_ID - Invalid port number.
 * Note:
 *      This API can set EEE function to the specific port.
 *      The configuration of the port is as following:
 *      - DISABLE
 *      - ENABLE
 */
extern l2sw_api_ret_t l2sw_eee_portEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable);


#endif /* __L2SW_API_EEE_H__ */

