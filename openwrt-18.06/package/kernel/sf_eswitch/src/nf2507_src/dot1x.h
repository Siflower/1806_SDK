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

#ifndef __L2SW_API_DOT1X_H__
#define __L2SW_API_DOT1X_H__


/* Type of port-based dot1x auth/unauth*/
typedef enum l2sw_dot1x_auth_status_e
{
    UNAUTH = 0,
    AUTH,
    AUTH_STATUS_END
} l2sw_dot1x_auth_status_t;

typedef enum l2sw_dot1x_direction_e
{
    DIR_BOTH = 0,
    DIR_IN,
    DIRECTION_END
} l2sw_dot1x_direction_t;

/* unauth pkt action */
typedef enum l2sw_dot1x_unauth_action_e
{
    DOT1X_ACTION_DROP = 0,
    DOT1X_ACTION_TRAP2CPU,
    DOT1X_ACTION_GUESTVLAN,
    DOT1X_ACTION_END
} l2sw_dot1x_unauth_action_t;

/* Function Name:
 *      l2sw_dot1x_unauthPacketOper_set
 * Description:
 *      Set 802.1x unauth action configuration.
 * Input:
 *      port            - Port id.
 *      unauth_action   - 802.1X unauth action.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_INPUT        - Invalid input parameter.
 * Note:
 *      This API can set 802.1x unauth action configuration.
 *      The unauth action is as following:
 *      - DOT1X_ACTION_DROP
 *      - DOT1X_ACTION_TRAP2CPU
 *      - DOT1X_ACTION_GUESTVLAN
 */
extern l2sw_api_ret_t l2sw_dot1x_unauthPacketOper_set(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_unauth_action_t unauth_action);

/* Function Name:
 *      l2sw_dot1x_unauthPacketOper_get
 * Description:
 *      Get 802.1x unauth action configuration.
 * Input:
 *      port - Port id.
 * Output:
 *      pUnauth_action - 802.1X unauth action.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API can get 802.1x unauth action configuration.
 *      The unauth action is as following:
 *      - DOT1X_ACTION_DROP
 *      - DOT1X_ACTION_TRAP2CPU
 *      - DOT1X_ACTION_GUESTVLAN
 */
extern l2sw_api_ret_t l2sw_dot1x_unauthPacketOper_get(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_unauth_action_t *pUnauth_action);

/* Function Name:
 *      l2sw_dot1x_eapolFrame2CpuEnable_set
 * Description:
 *      Set 802.1x EAPOL packet trap to CPU configuration
 * Input:
 *      enable - The status of 802.1x EAPOL packet.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid enable input.
 * Note:
 *      To support 802.1x authentication functionality, EAPOL frame (ether type = 0x888E) has to
 *      be trapped to CPU.
 *      The status of EAPOL frame trap to CPU is as following:
 *      - DISABLED
 *      - ENABLED
 */
extern l2sw_api_ret_t l2sw_dot1x_eapolFrame2CpuEnable_set(l2sw_uint8 units, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_dot1x_eapolFrame2CpuEnable_get
 * Description:
 *      Get 802.1x EAPOL packet trap to CPU configuration
 * Input:
 *      None
 * Output:
 *      pEnable - The status of 802.1x EAPOL packet.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      To support 802.1x authentication functionality, EAPOL frame (ether type = 0x888E) has to
 *      be trapped to CPU.
 *      The status of EAPOL frame trap to CPU is as following:
 *      - DISABLED
 *      - ENABLED
 */
extern l2sw_api_ret_t l2sw_dot1x_eapolFrame2CpuEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_dot1x_portBasedEnable_set
 * Description:
 *      Set 802.1x port-based enable configuration
 * Input:
 *      port - Port id.
 *      enable - The status of 802.1x port.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number.
 *      L2SW_ERR_ENABLE               - Invalid enable input.
 *      L2SW_ERR_DOT1X_PORTBASEDPNEN  - 802.1X port-based enable error
 * Note:
 *      The API can update the port-based port enable register content. If a port is 802.1x
 *      port based network access control "enabled", it should be authenticated so packets
 *      from that port won't be dropped or trapped to CPU.
 *      The status of 802.1x port-based network access control is as following:
 *      - DISABLED
 *      - ENABLED
 */
extern l2sw_api_ret_t l2sw_dot1x_portBasedEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_dot1x_portBasedEnable_get
 * Description:
 *      Get 802.1x port-based enable configuration
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - The status of 802.1x port.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get the 802.1x port-based port status.
 */
extern l2sw_api_ret_t l2sw_dot1x_portBasedEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_dot1x_portBasedAuthStatus_set
 * Description:
 *      Set 802.1x port-based auth. port configuration
 * Input:
 *      port - Port id.
 *      port_auth - The status of 802.1x port.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number.
 *     L2SW_ERR_DOT1X_PORTBASEDAUTH   - 802.1X port-based auth error
 * Note:
 *      The authenticated status of 802.1x port-based network access control is as following:
 *      - UNAUTH
 *      - AUTH
 */
extern l2sw_api_ret_t l2sw_dot1x_portBasedAuthStatus_set(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_auth_status_t port_auth);

/* Function Name:
 *      l2sw_dot1x_portBasedAuthStatus_get
 * Description:
 *      Get 802.1x port-based auth. port configuration
 * Input:
 *      port - Port id.
 * Output:
 *      pPort_auth - The status of 802.1x port.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get 802.1x port-based port auth.information.
 */
extern l2sw_api_ret_t l2sw_dot1x_portBasedAuthStatus_get(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_auth_status_t *pPort_auth);

/* Function Name:
 *      l2sw_dot1x_portBasedDirection_set
 * Description:
 *      Set 802.1x port-based operational direction configuration
 * Input:
 *      port            - Port id.
 *      port_direction  - Operation direction
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number.
 *      L2SW_ERR_DOT1X_PORTBASEDOPDIR - 802.1X port-based operation direction error
 * Note:
 *      The operate controlled direction of 802.1x port-based network access control is as following:
 *      - BOTH
 *      - IN
 */
extern l2sw_api_ret_t l2sw_dot1x_portBasedDirection_set(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_direction_t port_direction);

/* Function Name:
 *      l2sw_dot1x_portBasedDirection_get
 * Description:
 *      Get 802.1X port-based operational direction configuration
 * Input:
 *      port - Port id.
 * Output:
 *      pPort_direction - Operation direction
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get 802.1x port-based operational direction information.
 */
extern l2sw_api_ret_t l2sw_dot1x_portBasedDirection_get(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_direction_t *pPort_direction);

/* Function Name:
 *      l2sw_dot1x_macBasedEnable_set
 * Description:
 *      Set 802.1x mac-based port enable configuration
 * Input:
 *      port - Port id.
 *      enable - The status of 802.1x port.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number.
 *      L2SW_ERR_ENABLE               - Invalid enable input.
 *      L2SW_ERR_DOT1X_MACBASEDPNEN   - 802.1X mac-based enable error
 * Note:
 *      If a port is 802.1x MAC based network access control "enabled", the incoming packets should
 *       be authenticated so packets from that port won't be dropped or trapped to CPU.
 *      The status of 802.1x MAC-based network access control is as following:
 *      - DISABLED
 *      - ENABLED
 */
extern l2sw_api_ret_t l2sw_dot1x_macBasedEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_dot1x_macBasedEnable_get
 * Description:
 *      Get 802.1x mac-based port enable configuration
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - The status of 802.1x port.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      If a port is 802.1x MAC based network access control "enabled", the incoming packets should
 *      be authenticated so packets from that port wont be dropped or trapped to CPU.
 *      The status of 802.1x MAC-based network access control is as following:
 *      - DISABLED
 *      - ENABLED
 */
extern l2sw_api_ret_t l2sw_dot1x_macBasedEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_dot1x_macBasedAuthMac_add
 * Description:
 *      Add an authenticated MAC to ASIC
 * Input:
 *      port        - Port id.
 *      pAuth_mac   - The authenticated MAC.
 *      fid         - filtering database.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number.
 *      L2SW_ERR_ENABLE               - Invalid enable input.
 *      L2SW_ERR_DOT1X_MACBASEDPNEN   - 802.1X mac-based enable error
 * Note:
 *      The API can add a 802.1x authenticated MAC address to port. If the MAC does not exist in LUT,
 *      user can't add this MAC to auth status.
 */
extern l2sw_api_ret_t l2sw_dot1x_macBasedAuthMac_add(l2sw_uint8 units, l2sw_port_t port, l2sw_mac_t *pAuth_mac, l2sw_fid_t fid);

/* Function Name:
 *      l2sw_dot1x_macBasedAuthMac_del
 * Description:
 *      Delete an authenticated MAC to ASIC
 * Input:
 *      port - Port id.
 *      pAuth_mac - The authenticated MAC.
 *      fid - filtering database.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_MAC          - Invalid MAC address.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can delete a 802.1x authenticated MAC address to port. It only change the auth status of
 *      the MAC and won't delete it from LUT.
 */
extern l2sw_api_ret_t l2sw_dot1x_macBasedAuthMac_del(l2sw_uint8 units, l2sw_port_t port, l2sw_mac_t *pAuth_mac, l2sw_fid_t fid);

/* Function Name:
 *      l2sw_dot1x_macBasedAuthMac_get
 * Description:
 *      Get an authenticated MAC to ASIC
 * Input:
 *      port - Port id.
 *      pAuth_mac - The authenticated MAC.
 *      fid - filtering database.
*       pauth - The status of 802.1x mac.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_MAC          - Invalid MAC address.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get 802.1x mac-based operational port and auth-state information
 */
extern l2sw_api_ret_t l2sw_dot1x_macBasedAuthMac_get(l2sw_uint8 units, l2sw_port_t *port, l2sw_mac_t *pAuth_mac, l2sw_fid_t fid, l2sw_uint16 *pauth);

/* Function Name:
 *      l2sw_dot1x_macBasedDirection_set
 * Description:
 *      Set 802.1x mac-based operational direction configuration
 * Input:
 *      mac_direction - Operation direction
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_INPUT                - Invalid input parameter.
 *      L2SW_ERR_DOT1X_MACBASEDOPDIR  - 802.1X mac-based operation direction error
 * Note:
 *      The operate controlled direction of 802.1x mac-based network access control is as following:
 *      - BOTH
 *      - IN
 */
extern l2sw_api_ret_t l2sw_dot1x_macBasedDirection_set(l2sw_uint8 units, l2sw_dot1x_direction_t mac_direction);

/* Function Name:
 *      l2sw_dot1x_macBasedDirection_get
 * Description:
 *      Get 802.1x mac-based operational direction configuration
 * Input:
 *      None
 * Output:
 *      pMac_direction - Operation direction
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API can get 802.1x mac-based operational direction information.
 */
extern l2sw_api_ret_t l2sw_dot1x_macBasedDirection_get(l2sw_uint8 units, l2sw_dot1x_direction_t *pMac_direction);

/* Function Name:
 *      l2sw_dot1x_guestVlan_set
 * Description:
 *      Set 802.1x mac-based operational direction configuration
 * Input:
 *      vid - 802.1x guest VLAN ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameter.
 * Note:
 *      The operate controlled 802.1x guest VLAN
 */
extern l2sw_api_ret_t l2sw_dot1x_guestVlan_set(l2sw_uint8 units, l2sw_vlan_t vid);

/* Function Name:
 *      l2sw_dot1x_guestVlan_get
 * Description:
 *      Get 802.1x guest VLAN configuration
 * Input:
 *      None
 * Output:
 *      pVid - 802.1x guest VLAN ID
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API can get 802.1x guest VLAN information.
 */
extern l2sw_api_ret_t l2sw_dot1x_guestVlan_get(l2sw_uint8 units, l2sw_vlan_t *pVid);

/* Function Name:
 *      l2sw_dot1x_guestVlan2Auth_set
 * Description:
 *      Set 802.1x guest VLAN to auth host configuration
 * Input:
 *      enable - The status of guest VLAN to auth host.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameter.
 * Note:
 *      The operational direction of 802.1x guest VLAN to auth host control is as following:
 *      - ENABLED
 *      - DISABLED
 */
extern l2sw_api_ret_t l2sw_dot1x_guestVlan2Auth_set(l2sw_uint8 units, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_dot1x_guestVlan2Auth_get
 * Description:
 *      Get 802.1x guest VLAN to auth host configuration
 * Input:
 *      None
 * Output:
 *      pEnable - The status of guest VLAN to auth host.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API can get 802.1x guest VLAN to auth host information.
 */
extern l2sw_api_ret_t l2sw_dot1x_guestVlan2Auth_get(l2sw_uint8 units, l2sw_enable_t *pEnable);


#endif /* __L2SW_API_DOT1X_H__ */

