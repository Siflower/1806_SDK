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

#include "l2sw_switch.h"
#include "dot1x.h"
#ifdef L2SW_LINUX_KERNEL
#include "linux/kernel.h"
#endif
#include "sal_string.h"
#include "vlan.h"
#include "l2sw_asicdrv_dot1x.h"
#include "l2sw_asicdrv_rma.h"
#include "l2sw_asicdrv_lut.h"
#include "l2sw_asicdrv_vlan.h"

static l2sw_api_ret_t _l2sw_dot1x_unauthPacketOper_set(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_unauth_action_t unauth_action)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (unauth_action >= DOT1X_ACTION_END)
        return L2SW_ERR_DOT1X_PROC;

    if ((retVal = l2sw_setAsic1xProcConfig(units, l2sw_switch_port_L2P_get(units, port), unauth_action)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_dot1x_unauthPacketOper_get(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_unauth_action_t *pUnauth_action)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pUnauth_action)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsic1xProcConfig(units, l2sw_switch_port_L2P_get(units, port), pUnauth_action)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_dot1x_eapolFrame2CpuEnable_set(l2sw_uint8 units, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;
    l2sw_rma_t rmacfg;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if ((retVal = l2sw_getAsicRma(units, 3, &rmacfg)) != L2SW_ERR_OK)
        return retVal;

    if (L2SW_ENABLED == enable)
        rmacfg.operation = RMAOP_TRAP_TO_CPU;
    else if (L2SW_DISABLED == enable)
        rmacfg.operation = RMAOP_FORWARD;

    if ((retVal = l2sw_setAsicRma(units, 3, &rmacfg)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_dot1x_eapolFrame2CpuEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    l2sw_rma_t rmacfg;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicRma(units, 3, &rmacfg)) != L2SW_ERR_OK)
        return retVal;

    if (RMAOP_TRAP_TO_CPU == rmacfg.operation)
        *pEnable = L2SW_ENABLED;
    else
        *pEnable = L2SW_DISABLED;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_dot1x_portBasedEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if ((retVal = l2sw_setAsic1xPBEnConfig(units, l2sw_switch_port_L2P_get(units, port),enable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_dot1x_portBasedEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsic1xPBEnConfig(units, l2sw_switch_port_L2P_get(units, port), pEnable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_dot1x_portBasedAuthStatus_set(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_auth_status_t port_auth)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, port);

     if (port_auth >= AUTH_STATUS_END)
        return L2SW_ERR_DOT1X_PORTBASEDAUTH;

    if ((retVal = l2sw_setAsic1xPBAuthConfig(units, l2sw_switch_port_L2P_get(units, port), port_auth)) != L2SW_ERR_OK)
        return retVal;


    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_dot1x_portBasedAuthStatus_get(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_auth_status_t *pPort_auth)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pPort_auth)
        return L2SW_ERR_NULL_POINTER;

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_getAsic1xPBAuthConfig(units, l2sw_switch_port_L2P_get(units, port), pPort_auth)) != L2SW_ERR_OK)
        return retVal;
    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_dot1x_portBasedDirection_set(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_direction_t port_direction)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (port_direction >= DIRECTION_END)
        return L2SW_ERR_DOT1X_PORTBASEDOPDIR;

    if ((retVal = l2sw_setAsic1xPBOpdirConfig(units, l2sw_switch_port_L2P_get(units, port), port_direction)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_dot1x_portBasedDirection_get(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_direction_t *pPort_direction)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pPort_direction)
        return L2SW_ERR_NULL_POINTER;

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_getAsic1xPBOpdirConfig(units, l2sw_switch_port_L2P_get(units, port), pPort_direction)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_dot1x_macBasedEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if ((retVal = l2sw_setAsic1xMBEnConfig(units, l2sw_switch_port_L2P_get(units, port),enable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_dot1x_macBasedEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_getAsic1xMBEnConfig(units, l2sw_switch_port_L2P_get(units, port), pEnable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_dot1x_macBasedAuthMac_add(l2sw_uint8 units, l2sw_port_t port, l2sw_mac_t *pAuth_mac, l2sw_fid_t fid)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 method;
    l2sw_luttb l2Table;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* must be unicast address */
    if ((pAuth_mac == NULL) || (pAuth_mac->octet[0] & 0x1))
        return L2SW_ERR_MAC;

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (fid > L2SW_FIDMAX)
        return L2SW_ERR_L2_FID;

    MEMSET(&l2Table, 0, sizeof(l2sw_luttb));

    /* fill key (MAC,FID) to get L2 entry */
    MEMCPY(l2Table.mac.octet, pAuth_mac->octet, ETHER_ADDR_LEN);
    l2Table.fid = fid;
    method = LUTREADMETHOD_MAC;
    retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table);
    if ( L2SW_ERR_OK == retVal)
    {
        if (l2Table.spa != l2sw_switch_port_L2P_get(units, port)){           
			return L2SW_ERR_DOT1X_MAC_PORT_MISMATCH;
        }
        MEMCPY(l2Table.mac.octet, pAuth_mac->octet, ETHER_ADDR_LEN);
        l2Table.fid = fid;
        l2Table.efid = 0;
        l2Table.auth = 1;
        retVal = l2sw_setAsicL2LookupTb(units, &l2Table);
        return retVal;
    }
    else

        return retVal;

}

static l2sw_api_ret_t _l2sw_dot1x_macBasedAuthMac_del(l2sw_uint8 units, l2sw_port_t port, l2sw_mac_t *pAuth_mac, l2sw_fid_t fid)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 method;
    l2sw_luttb l2Table;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* must be unicast address */
    if ((pAuth_mac == NULL) || (pAuth_mac->octet[0] & 0x1))
        return L2SW_ERR_MAC;

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (fid > L2SW_FIDMAX)
        return L2SW_ERR_L2_FID;

    MEMSET(&l2Table, 0, sizeof(l2sw_luttb));

    /* fill key (MAC,FID) to get L2 entry */
    MEMCPY(l2Table.mac.octet, pAuth_mac->octet, ETHER_ADDR_LEN);
    l2Table.fid = fid;
    method = LUTREADMETHOD_MAC;
    retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table);
    if (L2SW_ERR_OK == retVal)
    {
        if (l2Table.spa != l2sw_switch_port_L2P_get(units, port))
            return L2SW_ERR_DOT1X_MAC_PORT_MISMATCH;

        MEMCPY(l2Table.mac.octet, pAuth_mac->octet, ETHER_ADDR_LEN);
        l2Table.fid = fid;
        l2Table.auth = 0;
        retVal = l2sw_setAsicL2LookupTb(units, &l2Table);
        return retVal;
    }
    else
        return retVal;

}

static l2sw_api_ret_t _l2sw_dot1x_macBasedAuthMac_get(l2sw_uint8 units, l2sw_port_t *port, l2sw_mac_t *pAuth_mac, l2sw_fid_t fid,l2sw_uint16 *pauth)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 method;
    l2sw_luttb l2Table;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* must be unicast address */
    if ((pAuth_mac == NULL) || (pAuth_mac->octet[0] & 0x1))
        return L2SW_ERR_MAC;

    if (fid > L2SW_FIDMAX)
        return L2SW_ERR_L2_FID;

    MEMSET(&l2Table, 0, sizeof(l2sw_luttb));

    /* fill key (MAC,FID) to get L2 entry */
    MEMCPY(l2Table.mac.octet, pAuth_mac->octet, ETHER_ADDR_LEN);
    l2Table.fid = fid;
    method = LUTREADMETHOD_MAC;
    retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table);
    if (L2SW_ERR_OK == retVal)
    {   
        *port = (l2sw_port_t)l2sw_switch_port_P2L_get(units, l2Table.spa);
        *pauth = l2Table.auth;
    }
    
    return retVal;
}

static l2sw_api_ret_t _l2sw_dot1x_macBasedDirection_set(l2sw_uint8 units, l2sw_dot1x_direction_t mac_direction)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (mac_direction >= DIRECTION_END)
        return L2SW_ERR_DOT1X_MACBASEDOPDIR;

    if ((retVal = l2sw_setAsic1xMBOpdirConfig(units, mac_direction)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_dot1x_macBasedDirection_get(l2sw_uint8 units, l2sw_dot1x_direction_t *pMac_direction)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pMac_direction)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsic1xMBOpdirConfig(units, pMac_direction)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_dot1x_guestVlan_set(l2sw_uint8 units, l2sw_vlan_t vid)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 index;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* vid must be 0~4095 */
    if (vid > L2SW_VIDMAX)
        return L2SW_ERR_VLAN_VID;

    if((retVal = l2sw_vlan_checkAndCreateMbr(units, vid, &index)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsic1xGuestVidx(units, index)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_dot1x_guestVlan_get(l2sw_uint8 units, l2sw_vlan_t *pVid)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 gvidx;
    l2sw_vlanconfiguser vlanMC;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pVid)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsic1xGuestVidx(units, &gvidx)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicVlanMemberConfig(units, gvidx, &vlanMC)) != L2SW_ERR_OK)
        return retVal;

    *pVid = vlanMC.evid;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_dot1x_guestVlan2Auth_set(l2sw_uint8 units, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if ((retVal = l2sw_setAsic1xGVOpdir(units, enable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_dot1x_guestVlan2Auth_get(l2sw_uint8 units, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsic1xGVOpdir(units, pEnable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}


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
l2sw_api_ret_t l2sw_dot1x_unauthPacketOper_set(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_unauth_action_t unauth_action)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_unauthPacketOper_set(units, port, unauth_action);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_dot1x_unauthPacketOper_get(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_unauth_action_t *pUnauth_action)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_unauthPacketOper_get(units, port, pUnauth_action);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_dot1x_eapolFrame2CpuEnable_set(l2sw_uint8 units, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_eapolFrame2CpuEnable_set(units, enable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_dot1x_eapolFrame2CpuEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_eapolFrame2CpuEnable_get(units, pEnable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_dot1x_portBasedEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_portBasedEnable_set(units, port, enable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_dot1x_portBasedEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_portBasedEnable_get(units, port, pEnable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_dot1x_portBasedAuthStatus_set(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_auth_status_t port_auth)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_portBasedAuthStatus_set(units, port, port_auth);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_dot1x_portBasedAuthStatus_get(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_auth_status_t *pPort_auth)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_portBasedAuthStatus_get(units, port, pPort_auth);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_dot1x_portBasedDirection_set(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_direction_t port_direction)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_portBasedDirection_set(units, port, port_direction);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_dot1x_portBasedDirection_get(l2sw_uint8 units, l2sw_port_t port, l2sw_dot1x_direction_t *pPort_direction)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_portBasedDirection_get(units, port, pPort_direction);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_dot1x_macBasedEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_macBasedEnable_set(units, port, enable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_dot1x_macBasedEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_macBasedEnable_get(units, port, pEnable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_dot1x_macBasedAuthMac_add(l2sw_uint8 units, l2sw_port_t port, l2sw_mac_t *pAuth_mac, l2sw_fid_t fid)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_macBasedAuthMac_add(units, port, pAuth_mac, fid);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_dot1x_macBasedAuthMac_del(l2sw_uint8 units, l2sw_port_t port, l2sw_mac_t *pAuth_mac, l2sw_fid_t fid)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_macBasedAuthMac_del(units, port, pAuth_mac, fid);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_dot1x_macBasedAuthMac_get(l2sw_uint8 units, l2sw_port_t *port, l2sw_mac_t *pAuth_mac, l2sw_fid_t fid,l2sw_uint16 *pauth)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_macBasedAuthMac_get(units, port, pAuth_mac, fid,pauth);    
    L2SW_API_UNLOCK(units);

    return retVal;
}


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
l2sw_api_ret_t l2sw_dot1x_macBasedDirection_set(l2sw_uint8 units, l2sw_dot1x_direction_t mac_direction)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_macBasedDirection_set(units, mac_direction);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_dot1x_macBasedDirection_get
 * Description:
 *      Get 802.1x mac-based operational direction configuration
 * Input:
 *      port - Port id.
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
l2sw_api_ret_t l2sw_dot1x_macBasedDirection_get(l2sw_uint8 units, l2sw_dot1x_direction_t *pMac_direction)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_macBasedDirection_get(units, pMac_direction);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      Set 802.1x guest VLAN configuration
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
l2sw_api_ret_t l2sw_dot1x_guestVlan_set(l2sw_uint8 units, l2sw_vlan_t vid)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_guestVlan_set(units, vid);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_dot1x_guestVlan_get(l2sw_uint8 units, l2sw_vlan_t *pVid)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_guestVlan_get(units, pVid);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_dot1x_guestVlan2Auth_set(l2sw_uint8 units, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_guestVlan2Auth_set(units, enable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_dot1x_guestVlan2Auth_get(l2sw_uint8 units, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_dot1x_guestVlan2Auth_get(units, pEnable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}


