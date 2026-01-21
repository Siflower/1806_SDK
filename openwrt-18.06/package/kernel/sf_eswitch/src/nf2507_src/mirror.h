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

#ifndef __L2SW_API_MIRROR_H__
#define __L2SW_API_MIRROR_H__

typedef enum l2sw_mirror_keep_e
{
    MIRROR_FOLLOW_VLAN = 0,
    MIRROR_KEEP_ORIGINAL,
    MIRROR_KEEP_END
}l2sw_mirror_keep_t;


/* Function Name:
 *      l2sw_mirror_portBased_set
 * Description:
 *      Set port mirror function.
 * Input:
 *      mirroring_port          - Monitor port.
 *      pMirrored_rx_portmask   - Rx mirror port mask.
 *      pMirrored_tx_portmask   - Tx mirror port mask.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_PORT_MASK    - Invalid portmask.
 * Note:
 *      The API is to set mirror function of source port and mirror port.
 *      The mirror port can only be set to one port and the TX and RX mirror ports
 *      should be identical.
 */
extern l2sw_api_ret_t l2sw_mirror_portBased_set(l2sw_uint8 units, l2sw_port_t mirroring_port, l2sw_portmask_t *pMirrored_rx_portmask, l2sw_portmask_t *pMirrored_tx_portmask);

/* Function Name:
 *      l2sw_mirror_portBased_get
 * Description:
 *      Get port mirror function.
 * Input:
 *      None
 * Output:
 *      pMirroring_port         - Monitor port.
 *      pMirrored_rx_portmask   - Rx mirror port mask.
 *      pMirrored_tx_portmask   - Tx mirror port mask.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API is to get mirror function of source port and mirror port.
 */
extern l2sw_api_ret_t l2sw_mirror_portBased_get(l2sw_uint8 units, l2sw_port_t* pMirroring_port, l2sw_portmask_t *pMirrored_rx_portmask, l2sw_portmask_t *pMirrored_tx_portmask);


/* Function Name:
 *      l2sw_mirror_isolationLeaky_set
 * Description:
 *      Set mirror Isolation leaky.
 * Input:
 *      txenable -TX leaky enable.
 *      rxenable - RX leaky enable.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to set mirror VLAN leaky function forwarding packets to miror port.
 */
extern l2sw_api_ret_t l2sw_mirror_isolationLeaky_set(l2sw_uint8 units, l2sw_enable_t txenable, l2sw_enable_t rxenable);

/* Function Name:
 *      l2sw_mirror_isolationLeaky_get
 * Description:
 *      Get mirror isolation leaky.
 * Input:
 *      None
 * Output:
 *      pTxenable - TX leaky enable.
 *      pRxenable - RX leaky enable.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API is to get mirror isolation leaky status.
 */
extern l2sw_api_ret_t l2sw_mirror_isolationLeaky_get(l2sw_uint8 units, l2sw_enable_t *pTxenable, l2sw_enable_t *pRxenable);

/* Function Name:
 *      l2sw_mirror_enable_set
 * Description:
 *      Set mirror function status.
 * Input:
 *      txenable -TX  enable.
 *      rxenable - RX  enable.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to set mirror enable function.
 */
extern l2sw_api_ret_t l2sw_mirror_enable_set(l2sw_uint8 units,l2sw_enable_t txEnable, l2sw_enable_t rxEnable);

/* Function Name:
 *      l2sw_mirror_enable_get
 * Description:
 *      Get mirror function status.
 * Input:
 *      None
 * Output:
 *      pTxEnable - TX leaky enable.
 *      pRxEnable - RX leaky enable.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to get mirror enable function.
 */
extern l2sw_api_ret_t l2sw_mirror_enable_get(l2sw_uint8 units,l2sw_enable_t *pTxEnable, l2sw_enable_t *pRxEnable);

/* Function Name:
 *      l2sw_mirror_leaky_oam_set
 * Description:
 *      Set mirror oam packet leaky enable.
 * Input:
 *      enable -  enable.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to set mirror oam packet leaky function.
 */
extern l2sw_api_ret_t l2sw_mirror_leaky_oam_set(l2sw_uint8 units,l2sw_enable_t enable);

/* Function Name:
 *      l2sw_mirror_leaky_oam_get
 * Description:
 *      Get mirror oam packet leaky enable.
 * Input:
 *      None
 * Output:
 *      pEnable
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to set mirror oam packet leaky function.
 */
 extern l2sw_api_ret_t l2sw_mirror_leaky_oam_get(l2sw_uint8 units,l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_mirror_include_acl_portlist_set
 * Description:
 *      Set mirror include acl portlist.
 * Input:
 *      enable -  enable.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to set mirror include acl packet portlist function.
 */
 extern l2sw_api_ret_t l2sw_mirror_include_acl_portlist_set(l2sw_uint8 units,l2sw_enable_t enable);

/* Function Name:
 *      l2sw_mirror_include_acl_portlist_get
 * Description:
 *      Set mirror include acl portlist.
 * Input:
 *      None
 * Output:
 *      pEnable
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to get mirror include acl packet portlist function.
 */
 extern l2sw_api_ret_t l2sw_mirror_include_acl_portlist_get(l2sw_uint8 units,l2sw_enable_t *pEnable);


#endif /* __L2SW_API_MIRROR_H__ */

