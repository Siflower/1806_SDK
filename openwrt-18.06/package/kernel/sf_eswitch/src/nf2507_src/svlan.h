/**  @file
  *  @brief    brief  description
  *  @author   Netforward
  *  @date     2020.5.7
  *  @version  1.0
  *  @note
  *  detailed  description
  *  $LastChangedDate$
  *  $LastChangedRevision$
  *  $LastChangedBy$
  */

#ifndef __L2SW_API_SVLAN_H__
#define __L2SW_API_SVLAN_H__

typedef l2sw_uint32 l2sw_svlan_index_t;

typedef struct l2sw_svlan_memberCfg_s{
    l2sw_uint32 svid;
    l2sw_portmask_t memberport;
    l2sw_portmask_t untagport;
    l2sw_uint32 fiden;
    l2sw_uint32 fid;
    l2sw_uint32 priority;
    l2sw_uint32 efiden;
    l2sw_uint32 efid;
}l2sw_svlan_memberCfg_t;

typedef enum l2sw_cpri_e
{
    SVLAN_SPRI_AS_CPRI = 0,
    SVLAN_CPRI_ZERO,
    SVLAN_CPRI_END
} l2sw_cpri_t;

typedef enum l2sw_svlan_pri_ref_e
{
    REF_INTERNAL_PRI = 0,
    REF_CTAG_PRI,
    REF_SVLAN_PRI,
    REF_PB_PRI,
    REF_PRI_END
} l2sw_svlan_pri_ref_t;


typedef l2sw_uint32 l2sw_svlan_tpid_t;

typedef enum l2sw_svlan_untag_action_e
{
    UNTAG_DROP = 0,
    UNTAG_TRAP,
    UNTAG_ASSIGN,
    UNTAG_END
} l2sw_svlan_untag_action_t;

typedef enum l2sw_svlan_unmatch_action_e
{
    UNMATCH_DROP = 0,
    UNMATCH_TRAP,
    UNMATCH_ASSIGN,
    UNMATCH_ASSIGN_KEEP,
    UNMATCH_END
} l2sw_svlan_unmatch_action_t;

typedef enum l2sw_svlan_unassign_action_e
{
    UNASSIGN_PBSVID = 0,
    UNASSIGN_TRAP,
    UNASSIGN_END
} l2sw_svlan_unassign_action_t;


typedef enum l2sw_svlan_lookupType_e
{
    SVLAN_LOOKUP_S64MBRCGF  = 0,
    SVLAN_LOOKUP_C4KVLAN,
    SVLAN_LOOKUP_END,

} l2sw_svlan_lookupType_t;

/* Function Name:
 *      l2sw_svlan_init
 * Description:
 *      Initialize SVLAN Configuration
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      Ether type of S-tag in 802.1ad is 0x88a8 and there are existed ether type 0x9100 and 0x9200 for Q-in-Q SLAN design.
 *      User can set mathced ether type as service provider supported protocol.
 */
extern l2sw_api_ret_t l2sw_svlan_init(l2sw_uint8 units);

/* Function Name:
 *      l2sw_svlan_servicePort_add
 * Description:
 *      Add one service port in the specified device
 * Input:
 *      port - Port id.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API is setting which port is connected to provider switch. All frames receiving from this port must
 *      contain accept SVID in S-tag field.
 */
extern l2sw_api_ret_t l2sw_svlan_servicePort_add(l2sw_uint8 units, l2sw_port_t port);

/* Function Name:
 *      l2sw_svlan_servicePort_get
 * Description:
 *      Get service ports in the specified device.
 * Input:
 *      None
 * Output:
 *      pSvlan_portmask - pointer buffer of svlan ports.
 * Return:
 *      L2SW_ERR_OK          - OK
 *      L2SW_ERR_FAILED      - Failed
 *      L2SW_ERR_SMI         - SMI access error
 * Note:
 *      This API is setting which port is connected to provider switch. All frames receiving from this port must
 *      contain accept SVID in S-tag field.
 */
extern l2sw_api_ret_t l2sw_svlan_servicePort_get(l2sw_uint8 units, l2sw_portmask_t *pSvlan_portmask);

/* Function Name:
 *      l2sw_svlan_servicePort_del
 * Description:
 *      Delete one service port in the specified device
 * Input:
 *      port - Port id.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API is removing SVLAN service port in the specified device.
 */
extern l2sw_api_ret_t l2sw_svlan_servicePort_del(l2sw_uint8 units, l2sw_port_t port);

/* Function Name:
 *      l2sw_svlan_tpidEntry_set
 * Description:
 *      Configure accepted S-VLAN ether type.
 * Input:
 *      svlan_tag_id - Ether type of S-tag frame parsing in uplink ports.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameter.
 * Note:
 *      Ether type of S-tag in 802.1ad is 0x88a8 and there are existed ether type 0x9100 and 0x9200 for Q-in-Q SLAN design.
 *      User can set mathced ether type as service provider supported protocol.
 */
extern l2sw_api_ret_t l2sw_svlan_tpidEntry_set(l2sw_uint8 units, l2sw_uint32 svlan_tag_id);

/* Function Name:
 *      l2sw_svlan_tpidEntry_get
 * Description:
 *      Get accepted S-VLAN ether type setting.
 * Input:
 *      None
 * Output:
 *      pSvlan_tag_id -  Ether type of S-tag frame parsing in uplink ports.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      This API is setting which port is connected to provider switch. All frames receiving from this port must
 *      contain accept SVID in S-tag field.
 */
extern l2sw_api_ret_t l2sw_svlan_tpidEntry_get(l2sw_uint8 units, l2sw_uint32 *pSvlan_tag_id);

/* Function Name:
 *      l2sw_svlan_priorityRef_set
 * Description:
 *      Set S-VLAN upstream priority reference setting.
 * Input:
 *      ref - reference selection parameter.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameter.
 * Note:
 *      The API can set the upstream SVLAN tag priority reference source. The related priority
 *      sources are as following:
 *      - REF_INTERNAL_PRI,
 *      - REF_CTAG_PRI,
 *      - REF_SVLAN_PRI,
 *      - REF_PB_PRI.
 */
extern l2sw_api_ret_t l2sw_svlan_priorityRef_set(l2sw_uint8 units, l2sw_svlan_pri_ref_t ref);

/* Function Name:
 *      l2sw_svlan_priorityRef_get
 * Description:
 *      Get S-VLAN upstream priority reference setting.
 * Input:
 *      None
 * Output:
 *      pRef - reference selection parameter.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      The API can get the upstream SVLAN tag priority reference source. The related priority
 *      sources are as following:
 *      - REF_INTERNAL_PRI,
 *      - REF_CTAG_PRI,
 *      - REF_SVLAN_PRI,
 *      - REF_PB_PRI
 */
extern l2sw_api_ret_t l2sw_svlan_priorityRef_get(l2sw_uint8 units, l2sw_svlan_pri_ref_t *pRef);

/* Function Name:
 *      l2sw_svlan_memberPortEntry_set
 * Description:
 *      Configure system SVLAN member content
 * Input:
 *      svid - SVLAN id
 *      psvlan_cfg - SVLAN member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_INPUT            - Invalid input parameter.
 *      L2SW_ERR_SVLAN_VID        - Invalid SVLAN VID parameter.
 *      L2SW_ERR_PORT_MASK        - Invalid portmask.
 *      L2SW_ERR_SVLAN_TABLE_FULL - SVLAN configuration is full.
 * Note:
 *      The API can set system 64 accepted s-tag frame format. Only 64 SVID S-tag frame will be accpeted
 *      to receiving from uplink ports. Other SVID S-tag frame or S-untagged frame will be droped by default setup.
 *      - l2sw_svlan_memberCfg_t->svid is SVID of SVLAN member configuration.
 *      - l2sw_svlan_memberCfg_t->memberport is member port mask of SVLAN member configuration.
 *      - l2sw_svlan_memberCfg_t->fid is filtering database of SVLAN member configuration.
 *      - l2sw_svlan_memberCfg_t->priority is priority of SVLAN member configuration.
 */
extern l2sw_api_ret_t l2sw_svlan_memberPortEntry_set(l2sw_uint8 units, l2sw_uint32 svid_idx, l2sw_svlan_memberCfg_t *psvlan_cfg);

/* Function Name:
 *      l2sw_svlan_memberPortEntry_get
 * Description:
 *      Get SVLAN member Configure.
 * Input:
 *      svid - SVLAN id
 * Output:
 *      pSvlan_cfg - SVLAN member configuration
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can get system 64 accepted s-tag frame format. Only 64 SVID S-tag frame will be accpeted
 *      to receiving from uplink ports. Other SVID S-tag frame or S-untagged frame will be droped.
 */
extern l2sw_api_ret_t l2sw_svlan_memberPortEntry_get(l2sw_uint8 units, l2sw_uint32 svid_idx, l2sw_svlan_memberCfg_t *pSvlan_cfg);

/* Function Name:
 *      l2sw_svlan_memberPortEntry_adv_set
 * Description:
 *      Configure system SVLAN member by index
 * Input:
 *      idx         - Index (0 ~ 63)
 *      psvlan_cfg  - SVLAN member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_INPUT            - Invalid input parameter.
 *      L2SW_ERR_SVLAN_VID        - Invalid SVLAN VID parameter.
 *      L2SW_ERR_PORT_MASK        - Invalid portmask.
 *      L2SW_ERR_SVLAN_TABLE_FULL - SVLAN configuration is full.
 * Note:
 *      The API can set system 64 accepted s-tag frame format by index.
 *      - l2sw_svlan_memberCfg_t->svid is SVID of SVLAN member configuration.
 *      - l2sw_svlan_memberCfg_t->memberport is member port mask of SVLAN member configuration.
 *      - l2sw_svlan_memberCfg_t->fid is filtering database of SVLAN member configuration.
 *      - l2sw_svlan_memberCfg_t->priority is priority of SVLAN member configuration.
 */
extern l2sw_api_ret_t l2sw_svlan_memberPortEntry_adv_set(l2sw_uint8 units, l2sw_uint32 idx, l2sw_svlan_memberCfg_t *pSvlan_cfg);

/* Function Name:
 *      l2sw_svlan_memberPortEntry_adv_get
 * Description:
 *      Get SVLAN member Configure by index.
 * Input:
 *      idx         - Index (0 ~ 63)
 * Output:
 *      pSvlan_cfg  - SVLAN member configuration
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can get system 64 accepted s-tag frame format. Only 64 SVID S-tag frame will be accpeted
 *      to receiving from uplink ports. Other SVID S-tag frame or S-untagged frame will be droped.
 */
extern l2sw_api_ret_t l2sw_svlan_memberPortEntry_adv_get(l2sw_uint8 units, l2sw_uint32 idx, l2sw_svlan_memberCfg_t *pSvlan_cfg);

/* Function Name:
 *      l2sw_svlan_defaultSvlan_set
 * Description:
 *      Configure default egress SVLAN.
 * Input:
 *      port - Source port
 *      svid - SVLAN id
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_INPUT                    - Invalid input parameter.
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 * Note:
 *      The API can set port n S-tag format index while receiving frame from port n
 *      is transmit through uplink port with s-tag field
 */
extern l2sw_api_ret_t l2sw_svlan_defaultSvlan_set(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_t svid);

/* Function Name:
 *      l2sw_svlan_defaultSvlan_get
 * Description:
 *      Get the configure default egress SVLAN.
 * Input:
 *      port - Source port
 * Output:
 *      pSvid - SVLAN VID
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API can get port n S-tag format index while receiving frame from port n
 *      is transmit through uplink port with s-tag field
 */
extern l2sw_api_ret_t l2sw_svlan_defaultSvlan_get(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_t *pSvid);

/* Function Name:
 *      l2sw_svlan_c2s_add
 * Description:
 *      Configure SVLAN C2S table
 * Input:
 *      vid - VLAN ID
 *      src_port - Ingress Port
 *      svid - SVLAN VID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port ID.
 *      L2SW_ERR_SVLAN_VID    - Invalid SVLAN VID parameter.
 *      L2SW_ERR_VLAN_VID     - Invalid VID parameter.
 *      L2SW_ERR_OUT_OF_RANGE - input out of range.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API can set system C2S configuration. ASIC will check upstream's VID and assign related
 *      SVID to mathed packet. There are 128 SVLAN C2S configurations.
 */
extern l2sw_api_ret_t l2sw_svlan_c2s_add(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_port_t src_port, l2sw_vlan_t svid);

/* Function Name:
 *      l2sw_svlan_c2s_del
 * Description:
 *      Delete one C2S entry
 * Input:
 *      vid - VLAN ID
 *      src_port - Ingress Port
 *      svid - SVLAN VID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_VLAN_VID         - Invalid VID parameter.
 *      L2SW_ERR_PORT_ID          - Invalid port ID.
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 * Note:
 *      The API can delete system C2S configuration. There are 128 SVLAN C2S configurations.
 */
extern l2sw_api_ret_t l2sw_svlan_c2s_del(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_port_t src_port);

/* Function Name:
 *      l2sw_svlan_c2s_get
 * Description:
 *      Get configure SVLAN C2S table
 * Input:
 *      vid - VLAN ID
 *      src_port - Ingress Port
 * Output:
 *      pSvid - SVLAN ID
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port ID.
 *      L2SW_ERR_OUT_OF_RANGE - input out of range.
 * Note:
 *     The API can get system C2S configuration. There are 128 SVLAN C2S configurations.
 */
extern l2sw_api_ret_t l2sw_svlan_c2s_get(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_port_t src_port, l2sw_vlan_t *pSvid);

/* Function Name:
 *      l2sw_svlan_untag_action_set
 * Description:
 *      Configure Action of downstream Un-Stag packet
 * Input:
 *      action  - Action for UnStag
 *      svid    - The SVID assigned to UnStag packet
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can configure action of downstream Un-Stag packet. A SVID assigned
 *      to the un-stag is also supported by this API. The parameter of svid is
 *      only referenced when the action is set to UNTAG_ASSIGN
 */
extern l2sw_api_ret_t l2sw_svlan_untag_action_set(l2sw_uint8 units, l2sw_svlan_untag_action_t action, l2sw_vlan_t svid);

/* Function Name:
 *      l2sw_svlan_untag_action_get
 * Description:
 *      Get Action of downstream Un-Stag packet
 * Input:
 *      None
 * Output:
 *      pAction  - Action for UnStag
 *      pSvid    - The SVID assigned to UnStag packet
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can Get action of downstream Un-Stag packet. A SVID assigned
 *      to the un-stag is also retrieved by this API. The parameter pSvid is
 *      only refernced when the action is UNTAG_ASSIGN
 */
extern l2sw_api_ret_t l2sw_svlan_untag_action_get(l2sw_uint8 units, l2sw_svlan_untag_action_t *pAction, l2sw_vlan_t *pSvid);

/* Function Name:
 *      l2sw_svlan_unmatch_action_set
 * Description:
 *      Configure Action of downstream Unmatch packet
 * Input:
 *      action  - Action for Unmatch
 *      svid    - The SVID assigned to Unmatch packet
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can configure action of downstream Un-match packet. A SVID assigned
 *      to the un-match is also supported by this API. The parameter od svid is
 *      only refernced when the action is set to UNMATCH_ASSIGN
 */
extern l2sw_api_ret_t l2sw_svlan_unmatch_action_set(l2sw_uint8 units, l2sw_svlan_unmatch_action_t action, l2sw_vlan_t svid);

/* Function Name:
 *      l2sw_svlan_unmatch_action_get
 * Description:
 *      Get Action of downstream Unmatch packet
 * Input:
 *      None
 * Output:
 *      pAction  - Action for Unmatch
 *      pSvid    - The SVID assigned to Unmatch packet
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can Get action of downstream Un-match packet. A SVID assigned
 *      to the un-match is also retrieved by this API. The parameter pSvid is
 *      only refernced when the action is UNMATCH_ASSIGN
 */
extern l2sw_api_ret_t l2sw_svlan_unmatch_action_get(l2sw_uint8 units, l2sw_svlan_unmatch_action_t *pAction, l2sw_vlan_t *pSvid);

/* Function Name:
 *      l2sw_svlan_dmac_vidsel_set
 * Description:
 *      Set DMAC CVID selection
 * Input:
 *      port    - Port
 *      enable  - state of DMAC CVID Selection
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      This API can set DMAC CVID Selection state
 */
extern l2sw_api_ret_t l2sw_svlan_dmac_vidsel_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_svlan_dmac_vidsel_get
 * Description:
 *      Get DMAC CVID selection
 * Input:
 *      port    - Port
 * Output:
 *      pEnable - state of DMAC CVID Selection
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      This API can get DMAC CVID Selection state
 */
extern l2sw_api_ret_t l2sw_svlan_dmac_vidsel_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_svlan_ipmc2s_add
 * Description:
 *      add ip multicast address to SVLAN
 * Input:
 *      svid    - SVLAN VID
 *      ipmc    - ip multicast address
 *      ipmcMsk - ip multicast mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can set IP mutlicast to SVID configuration. If upstream packet is IPv4 multicast
 *      packet and DIP is matched MC2S configuration, ASIC will assign egress SVID to the packet.
 *      There are 32 SVLAN multicast configurations for IP and L2 multicast.
 */
extern l2sw_api_ret_t l2sw_svlan_ipmc2s_add(l2sw_uint8 units, ipaddr_t ipmc, ipaddr_t ipmcMsk, l2sw_vlan_t svid);

/* Function Name:
 *      l2sw_svlan_ipmc2s_del
 * Description:
 *      delete ip multicast address to SVLAN
 * Input:
 *      ipmc    - ip multicast address
 *      ipmcMsk - ip multicast mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_SVLAN_VID        - Invalid SVLAN VID parameter.
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 * Note:
 *      The API can delete IP mutlicast to SVID configuration. There are 32 SVLAN multicast configurations for IP and L2 multicast.
 */
extern l2sw_api_ret_t l2sw_svlan_ipmc2s_del(l2sw_uint8 units, ipaddr_t ipmc, ipaddr_t ipmcMsk);

/* Function Name:
 *      l2sw_svlan_ipmc2s_get
 * Description:
 *      Get ip multicast address to SVLAN
 * Input:
 *      ipmc    - ip multicast address
 *      ipmcMsk - ip multicast mask
 * Output:
 *      pSvid - SVLAN VID
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_OUT_OF_RANGE - input out of range.
 * Note:
 *      The API can get IP mutlicast to SVID configuration. There are 32 SVLAN multicast configurations for IP and L2 multicast.
 */
extern l2sw_api_ret_t l2sw_svlan_ipmc2s_get(l2sw_uint8 units, ipaddr_t ipmc, ipaddr_t ipmcMsk, l2sw_vlan_t *pSvid);

/* Function Name:
 *      l2sw_svlan_l2mc2s_add
 * Description:
 *      Add L2 multicast address to SVLAN
 * Input:
 *      mac     - L2 multicast address
 *      macMsk  - L2 multicast address mask
 *      svid    - SVLAN VID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can set L2 Mutlicast to SVID configuration. If upstream packet is L2 multicast
 *      packet and DMAC is matched, ASIC will assign egress SVID to the packet. There are 32
 *      SVLAN multicast configurations for IP and L2 multicast.
 */
extern l2sw_api_ret_t l2sw_svlan_l2mc2s_add(l2sw_uint8 units, l2sw_mac_t mac, l2sw_mac_t macMsk, l2sw_vlan_t svid);

/* Function Name:
 *      l2sw_svlan_l2mc2s_del
 * Description:
 *      delete L2 multicast address to SVLAN
 * Input:
 *      mac     - L2 multicast address
 *      macMsk  - L2 multicast address mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_SVLAN_VID        - Invalid SVLAN VID parameter.
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 * Note:
 *      The API can delete Mutlicast to SVID configuration. There are 32 SVLAN multicast configurations for IP and L2 multicast.
 */
extern l2sw_api_ret_t l2sw_svlan_l2mc2s_del(l2sw_uint8 units, l2sw_mac_t mac, l2sw_mac_t macMsk);

/* Function Name:
 *      l2sw_svlan_l2mc2s_get
 * Description:
 *      Get L2 multicast address to SVLAN
 * Input:
 *      mac     - L2 multicast address
 *      macMsk  - L2 multicast address mask
 * Output:
 *      pSvid   - SVLAN VID
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 * Note:
 *      The API can get L2 mutlicast to SVID configuration. There are 32 SVLAN multicast configurations for IP and L2 multicast.
 */
extern l2sw_api_ret_t l2sw_svlan_l2mc2s_get(l2sw_uint8 units, l2sw_mac_t mac, l2sw_mac_t macMsk, l2sw_vlan_t *pSvid);

/* Function Name:
 *      l2sw_svlan_sp2c_add
 * Description:
 *      Add system SP2C configuration
 * Input:
 *      cvid        - VLAN ID
 *      dst_port    - Destination port of SVLAN to CVLAN configuration
 *      svid        - SVLAN VID
 *
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_SVLAN_VID    - Invalid SVLAN VID parameter.
 *      L2SW_ERR_VLAN_VID     - Invalid VID parameter.
 *      L2SW_ERR_OUT_OF_RANGE - input out of range.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API can add SVID & Destination Port to CVLAN configuration. The downstream frames with assigned
 *      SVID will be add C-tag with assigned CVID if the output port is the assigned destination port.
 *      There are 128 SP2C configurations.
 */
extern l2sw_api_ret_t l2sw_svlan_sp2c_add(l2sw_uint8 units, l2sw_vlan_t svid, l2sw_port_t dst_port, l2sw_vlan_t cvid);

/* Function Name:
 *      l2sw_svlan_sp2c_get
 * Description:
 *      Get configure system SP2C content
 * Input:
 *      svid        - SVLAN VID
 *      dst_port    - Destination port of SVLAN to CVLAN configuration
 * Output:
 *      pCvid - VLAN ID
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_OUT_OF_RANGE - input out of range.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_SVLAN_VID    - Invalid SVLAN VID parameter.
 * Note:
 *     The API can get SVID & Destination Port to CVLAN configuration. There are 128 SP2C configurations.
 */
extern l2sw_api_ret_t l2sw_svlan_sp2c_get(l2sw_uint8 units, l2sw_vlan_t svid, l2sw_port_t dst_port, l2sw_vlan_t *pCvid);

/* Function Name:
 *      l2sw_svlan_sp2c_del
 * Description:
 *      Delete system SP2C configuration
 * Input:
 *      svid        - SVLAN VID
 *      dst_port    - Destination port of SVLAN to CVLAN configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_SVLAN_VID    - Invalid SVLAN VID parameter.
 *      L2SW_ERR_OUT_OF_RANGE - input out of range.
 * Note:
 *      The API can delete SVID & Destination Port to CVLAN configuration. There are 128 SP2C configurations.
 */
extern l2sw_api_ret_t l2sw_svlan_sp2c_del(l2sw_uint8 units, l2sw_vlan_t svid, l2sw_port_t dst_port);


/* Function Name:
 *      l2sw_svlan_lookupType_set
 * Description:
 *      Set lookup type of SVLAN
 * Input:
 *      type        - lookup type
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 * Note:
 *      none
 */
extern l2sw_api_ret_t l2sw_svlan_lookupType_set(l2sw_uint8 units, l2sw_svlan_lookupType_t type);

/* Function Name:
 *      l2sw_svlan_lookupType_get
 * Description:
 *      Get lookup type of SVLAN
 * Input:
 *      pType       - lookup type
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 * Note:
 *      none
 */
extern l2sw_api_ret_t l2sw_svlan_lookupType_get(l2sw_uint8 units, l2sw_svlan_lookupType_t *pType);

/* Function Name:
 *      l2sw_svlan_trapPri_set
 * Description:
 *      Set svlan trap priority
 * Input:
 *      priority - priority for trap packets
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_QOS_INT_PRIORITY
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_svlan_trapPri_set(l2sw_uint8 units, l2sw_pri_t priority);

/* Function Name:
 *      l2sw_svlan_trapPri_get
 * Description:
 *      Get svlan trap priority
 * Input:
 *      None
 * Output:
 *      pPriority - priority for trap packets
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_svlan_trapPri_get(l2sw_uint8 units, l2sw_pri_t *pPriority);

/* Function Name:
 *      l2sw_svlan_unassign_action_set
 * Description:
 *      Configure Action of upstream without svid assign action
 * Input:
 *      action  - Action for Un-assign
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can configure action of upstream Un-assign svid packet. If action is not
 *      trap to CPU, the port-based SVID sure be assign as system need
 */
extern l2sw_api_ret_t l2sw_svlan_unassign_action_set(l2sw_uint8 units, l2sw_svlan_unassign_action_t action);

/* Function Name:
 *      l2sw_svlan_unassign_action_get
 * Description:
 *      Get action of upstream without svid assignment
 * Input:
 *      None
 * Output:
 *      pAction  - Action for Un-assign
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_svlan_unassign_action_get(l2sw_uint8 units, l2sw_svlan_unassign_action_t *pAction);


/* Function Name:
 *      l2sw_svlan_checkAndCreateMbr
 * Description:
 *      Check and create Member configuration and return index
 * Input:
 *      vid  - VLAN id.
 * Output:
 *      pIndex  - Member configuration index
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_VLAN_VID     - Invalid VLAN ID.
 *      L2SW_ERR_TBL_FULL     - Member Configuration table full
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_svlan_checkAndCreateMbr(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_uint32 *pIndex);

/* Function Name:
 *      l2sw_svlan_s2c_add_cpri_set
 * Description:
 *      Set the source of priority field in the new c-tag of S2C downstream
 * Input:
 *      cpri_type   - CPRI value in CTAG
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_SVLAN_VID    - Invalid SVLAN VID parameter.
 *      L2SW_ERR_OUT_OF_RANGE - input out of range.
 * Note:
 *   
 */
l2sw_api_ret_t l2swu_svlan_s2c_add_cpri_set(l2sw_uint8 units, l2sw_cpri_t cpri_type);

/* Function Name:
 *      l2sw_svlan_s2c_add_cpri_get
 * Description:
 *      Get the source of priority field in the new c-tag of S2C downstream
 * Input:
 *      None
 * Output:
 *      pcpri_type   - CPRI value in CTAG
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_SVLAN_VID    - Invalid SVLAN VID parameter.
 *      L2SW_ERR_OUT_OF_RANGE - input out of range.
 * Note:
 *   
 */
l2sw_api_ret_t l2swu_svlan_s2c_add_cpri_get(l2sw_uint8 units, l2sw_cpri_t *pcpri_type);



#endif /* __L2SW_API_SVLAN_H__ */
