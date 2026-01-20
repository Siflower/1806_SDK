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

#ifndef __L2SW_API_VLAN_H__
#define __L2SW_API_VLAN_H__


/*
 * Data Type Declaration
 */
#define L2SW_MAX_NUM_OF_PROTO_TYPE                   0xFFFF
#define L2SW_MAX_NUM_OF_MSTI                         0xF
#define L2SW_FID_MAX                                 0xF

typedef struct  l2sw_vlan_cfg_s
{
    l2sw_portmask_t  mbr;
    l2sw_portmask_t  untag;
    l2sw_uint16      ivl_en;
    l2sw_uint16      fid_msti;
    l2sw_uint16      envlanpol;
    l2sw_uint16      meteridx;
    l2sw_uint16      vbpen;
    l2sw_uint16      vbpri;
}l2sw_vlan_cfg_t;

typedef struct  l2sw_vlan_mbrcfg_s
{
    l2sw_uint16      evid;
    l2sw_portmask_t  mbr;
    l2sw_uint16      fid_msti;
    l2sw_uint16      envlanpol;
    l2sw_uint16      meteridx;
    l2sw_uint16      vbpen;
    l2sw_uint16      vbpri;
}l2sw_vlan_mbrcfg_t;

typedef l2sw_uint32  l2sw_stp_msti_id_t;     /* MSTI ID  */

typedef enum l2sw_stp_state_e
{
    STP_STATE_DISABLED = 0,
    STP_STATE_BLOCKING,
    STP_STATE_LEARNING,
    STP_STATE_FORWARDING,
    STP_STATE_END
} l2sw_stp_state_t;

typedef l2sw_uint32  l2sw_vlan_proto_type_t;     /* protocol and port based VLAN protocol type  */


typedef enum l2sw_vlan_acceptFrameType_e
{
    ACCEPT_FRAME_TYPE_ALL = 0,             /* untagged, priority-tagged and tagged */
    ACCEPT_FRAME_TYPE_TAG_ONLY,         /* tagged */
    ACCEPT_FRAME_TYPE_UNTAG_ONLY,     /* untagged and priority-tagged */
    ACCEPT_FRAME_TYPE_END
} l2sw_vlan_acceptFrameType_t;


/* frame type of protocol vlan - reference 802.1v standard */
typedef enum l2sw_vlan_protoVlan_frameType_e
{
    FRAME_TYPE_ETHERNET = 0,
    FRAME_TYPE_LLCOTHER,
    FRAME_TYPE_RFC1042,
    FRAME_TYPE_END
} l2sw_vlan_protoVlan_frameType_t;

/* Protocol-and-port-based Vlan structure */
typedef struct l2sw_vlan_protoAndPortInfo_s
{
    l2sw_uint32                         proto_type;
    l2sw_vlan_protoVlan_frameType_t frame_type;
    l2sw_vlan_t                     cvid;
    l2sw_pri_t                     cpri;
}l2sw_vlan_protoAndPortInfo_t;

/* tagged mode of VLAN - reference realtek private specification */
typedef enum l2sw_vlan_tagMode_e
{
    VLAN_TAG_MODE_ORIGINAL = 0,
    VLAN_TAG_MODE_KEEP_FORMAT,
    VLAN_TAG_MODE_PRI,
    VLAN_TAG_MODE_REAL_KEEP_FORMAT,
    VLAN_TAG_MODE_END
} l2sw_vlan_tagMode_t;

typedef enum l2sw_vlan_resVidAction_e
{
    RESVID_ACTION_UNTAG = 0,
    RESVID_ACTION_TAG,
    RESVID_ACTION_END
} l2sw_vlan_resVidAction_t;

/* Function Name:
 *      l2sw_vlan_init
 * Description:
 *      Initialize VLAN.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      VLAN is disabled by default. User has to call this API to enable VLAN before
 *      using it. And It will set a default VLAN(vid 1) including all ports and set
 *      all ports PVID to the default VLAN.
 */
extern l2sw_api_ret_t l2sw_vlan_init(l2sw_uint8 units);

/* Function Name:
 *      l2sw_vlan_set
 * Description:
 *      Set a VLAN entry.
 * Input:
 *      vid - VLAN ID to configure.
 *      pVlanCfg - VLAN Configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 *      L2SW_ERR_L2_FID               - Invalid FID.
 *      L2SW_ERR_VLAN_PORT_MBR_EXIST  - Invalid member port mask.
 *      L2SW_ERR_VLAN_VID             - Invalid VID parameter.
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_vlan_set(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_vlan_cfg_t *pVlanCfg);

/* Function Name:
 *      l2sw_vlan_get
 * Description:
 *      Get a VLAN entry.
 * Input:
 *      vid - VLAN ID to configure.
 * Output:
 *      pVlanCfg - VLAN Configuration
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_VLAN_VID     - Invalid VID parameter.
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_vlan_get(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_vlan_cfg_t *pVlanCfg);

/* Function Name:
 *      l2sw_vlan_egrFilterEnable_set
 * Description:
 *      Set VLAN egress filter.
 * Input:
 *      egrFilter - Egress filtering
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid input parameters.
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_vlan_egrFilterEnable_set(l2sw_uint8 units, l2sw_enable_t egrFilter);

/* Function Name:
 *      l2sw_vlan_egrFilterEnable_get
 * Description:
 *      Get VLAN egress filter.
 * Input:
 *      pEgrFilter - Egress filtering
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_NULL_POINTER - NULL Pointer.
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_vlan_egrFilterEnable_get(l2sw_uint8 units, l2sw_enable_t *pEgrFilter);

/* Function Name:
 *      l2sw_vlan_mbrCfg_set
 * Description:
 *      Set a VLAN Member Configuration entry by index.
 * Input:
 *      idx     - Index of VLAN Member Configuration.
 *      pMbrcfg - VLAN member Configuration.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_VLAN_VID     - Invalid VID parameter.
 * Note:
 *     Set a VLAN Member Configuration entry by index.
 */
extern l2sw_api_ret_t l2sw_vlan_mbrCfg_set(l2sw_uint8 units, l2sw_uint32 idx, l2sw_vlan_mbrcfg_t *pMbrcfg);

/* Function Name:
 *      l2sw_vlan_mbrCfg_get
 * Description:
 *      Get a VLAN Member Configuration entry by index.
 * Input:
 *      idx - Index of VLAN Member Configuration.
 * Output:
 *      pMbrcfg - VLAN member Configuration.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_VLAN_VID     - Invalid VID parameter.
 * Note:
 *     Get a VLAN Member Configuration entry by index.
 */
extern l2sw_api_ret_t l2sw_vlan_mbrCfg_get(l2sw_uint8 units, l2sw_uint32 idx, l2sw_vlan_mbrcfg_t *pMbrcfg);

/* Function Name:
 *     l2sw_vlan_portPvid_set
 * Description:
 *      Set port to specified VLAN ID(PVID).
 * Input:
 *      port - Port id.
 *      pvid - Specified VLAN ID.
 *      priority - 802.1p priority for the PVID.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number.
 *      L2SW_ERR_VLAN_PRIORITY        - Invalid priority.
 *      L2SW_ERR_VLAN_ENTRY_NOT_FOUND - VLAN entry not found.
 *      L2SW_ERR_VLAN_VID             - Invalid VID parameter.
 * Note:
 *       The API is used for Port-based VLAN. The untagged frame received from the
 *       port will be classified to the specified VLAN and assigned to the specified priority.
 */
extern l2sw_api_ret_t l2sw_vlan_portPvid_set(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_t pvid, l2sw_pri_t priority);

/* Function Name:
 *      l2sw_vlan_portPvid_get
 * Description:
 *      Get VLAN ID(PVID) on specified port.
 * Input:
 *      port - Port id.
 * Output:
 *      pPvid - Specified VLAN ID.
 *      pPriority - 802.1p priority for the PVID.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *     The API can get the PVID and 802.1p priority for the PVID of Port-based VLAN.
 */
extern l2sw_api_ret_t l2sw_vlan_portPvid_get(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_t *pPvid, l2sw_pri_t *pPriority);

/* Function Name:
 *      l2sw_vlan_portIgrFilterEnable_set
 * Description:
 *      Set VLAN ingress for each port.
 * Input:
 *      port - Port id.
 *      igr_filter - VLAN ingress function enable status.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The status of vlan ingress filter is as following:
 *      - DISABLED
 *      - ENABLED
 *      While VLAN function is enabled, ASIC will decide VLAN ID for each received frame and get belonged member
 *      ports from VLAN table. If received port is not belonged to VLAN member ports, ASIC will drop received frame if VLAN ingress function is enabled.
 */
extern l2sw_api_ret_t l2sw_vlan_portIgrFilterEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t igr_filter);

/* Function Name:
 *      l2sw_vlan_portIgrFilterEnable_get
 * Description:
 *      Get VLAN Ingress Filter
 * Input:
 *      port        - Port id.
 * Output:
 *      pIgr_filter - VLAN ingress function enable status.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *     The API can Get the VLAN ingress filter status.
 *     The status of vlan ingress filter is as following:
 *     - DISABLED
 *     - ENABLED
 */
extern l2sw_api_ret_t l2sw_vlan_portIgrFilterEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pIgr_filter);

/* Function Name:
 *      l2sw_vlan_portAcceptFrameType_set
 * Description:
 *      Set VLAN accept_frame_type
 * Input:
 *      port                - Port id.
 *      accept_frame_type   - accept frame type
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_PORT_ID                  - Invalid port number.
 *      L2SW_ERR_VLAN_ACCEPT_FRAME_TYPE   - Invalid frame type.
 * Note:
 *      The API is used for checking 802.1Q tagged frames.
 *      The accept frame type as following:
 *      - ACCEPT_FRAME_TYPE_ALL
 *      - ACCEPT_FRAME_TYPE_TAG_ONLY
 *      - ACCEPT_FRAME_TYPE_UNTAG_ONLY
 */
extern l2sw_api_ret_t l2sw_vlan_portAcceptFrameType_set(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_acceptFrameType_t accept_frame_type);

/* Function Name:
 *      l2sw_vlan_portAcceptFrameType_get
 * Description:
 *      Get VLAN accept_frame_type
 * Input:
 *      port - Port id.
 * Output:
 *      pAccept_frame_type - accept frame type
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *     The API can Get the VLAN ingress filter.
 *     The accept frame type as following:
 *     - ACCEPT_FRAME_TYPE_ALL
 *     - ACCEPT_FRAME_TYPE_TAG_ONLY
 *     - ACCEPT_FRAME_TYPE_UNTAG_ONLY
 */
extern l2sw_api_ret_t l2sw_vlan_portAcceptFrameType_get(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_acceptFrameType_t *pAccept_frame_type);

/* Function Name:
 *      l2sw_vlan_tagMode_set
 * Description:
 *      Set CVLAN egress tag mode
 * Input:
 *      port        - Port id.
 *      tag_mode    - The egress tag mode.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_INPUT        - Invalid input parameter.
 *      L2SW_ERR_ENABLE       - Invalid enable input.
 * Note:
 *      The API can set Egress tag mode. There are 4 mode for egress tag:
 *      - VLAN_TAG_MODE_ORIGINAL,
 *      - VLAN_TAG_MODE_KEEP_FORMAT,
 *      - VLAN_TAG_MODE_PRI.
 *      - VLAN_TAG_MODE_REAL_KEEP_FORMAT,
 */
extern l2sw_api_ret_t l2sw_vlan_tagMode_set(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_tagMode_t tag_mode);

/* Function Name:
 *      l2sw_vlan_tagMode_get
 * Description:
 *      Get CVLAN egress tag mode
 * Input:
 *      port - Port id.
 * Output:
 *      pTag_mode - The egress tag mode.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get Egress tag mode. There are 4 mode for egress tag:
 *      - VLAN_TAG_MODE_ORIGINAL,
 *      - VLAN_TAG_MODE_KEEP_FORMAT,
 *      - VLAN_TAG_MODE_PRI.
 *      - VLAN_TAG_MODE_REAL_KEEP_FORMAT,
 */
extern l2sw_api_ret_t l2sw_vlan_tagMode_get(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_tagMode_t *pTag_mode);

/* Function Name:
 *      l2sw_vlan_transparent_set
 * Description:
 *      Set VLAN transparent mode
 * Input:
 *      egr_port        - Egress Port id.
 *      pIgr_pmask      - Ingress Port Mask.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      None.
 */
extern l2sw_api_ret_t l2sw_vlan_transparent_set(l2sw_uint8 units, l2sw_port_t egr_port, l2sw_portmask_t *pIgr_pmask);

/* Function Name:
 *      l2sw_vlan_transparent_get
 * Description:
 *      Get VLAN transparent mode
 * Input:
 *      egr_port        - Egress Port id.
 * Output:
 *      pIgr_pmask      - Ingress Port Mask
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      None.
 */
extern l2sw_api_ret_t l2sw_vlan_transparent_get(l2sw_uint8 units, l2sw_port_t egr_port, l2sw_portmask_t *pIgr_pmask);

/* Function Name:
 *      l2sw_vlan_keep_set
 * Description:
 *      Set VLAN egress keep mode
 * Input:
 *      egr_port        - Egress Port id.
 *      pIgr_pmask      - Ingress Port Mask.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      None.
 */
extern l2sw_api_ret_t l2sw_vlan_keep_set(l2sw_uint8 units, l2sw_port_t egr_port, l2sw_portmask_t *pIgr_pmask);

/* Function Name:
 *      l2sw_vlan_keep_get
 * Description:
 *      Get VLAN egress keep mode
 * Input:
 *      egr_port        - Egress Port id.
 * Output:
 *      pIgr_pmask      - Ingress Port Mask
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      None.
 */
extern l2sw_api_ret_t l2sw_vlan_keep_get(l2sw_uint8 units, l2sw_port_t egr_port, l2sw_portmask_t *pIgr_pmask);

/* Function Name:
 *      l2sw_vlan_stg_set
 * Description:
 *      Set spanning tree group instance of the vlan to the specified device
 * Input:
 *      vid - Specified VLAN ID.
 *      stg - spanning tree group instance.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_MSTI         - Invalid msti parameter
 *      L2SW_ERR_INPUT        - Invalid input parameter.
 *      L2SW_ERR_VLAN_VID     - Invalid VID parameter.
 * Note:
 *      The API can set spanning tree group instance of the vlan to the specified device.
 */
extern l2sw_api_ret_t l2sw_vlan_stg_set(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_stp_msti_id_t stg);

/* Function Name:
 *      l2sw_vlan_stg_get
 * Description:
 *      Get spanning tree group instance of the vlan to the specified device
 * Input:
 *      vid - Specified VLAN ID.
 * Output:
 *      pStg - spanning tree group instance.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_VLAN_VID     - Invalid VID parameter.
 * Note:
 *      The API can get spanning tree group instance of the vlan to the specified device.
 */
extern l2sw_api_ret_t l2sw_vlan_stg_get(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_stp_msti_id_t *pStg);

/* Function Name:
 *      l2sw_vlan_protoAndPortBasedVlan_add
 * Description:
 *      Add the protocol-and-port-based vlan to the specified port of device.
 * Input:
 *      port  - Port id.
 *      pInfo - Protocol and port based VLAN configuration information.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_VLAN_VID         - Invalid VID parameter.
 *      L2SW_ERR_VLAN_PRIORITY    - Invalid priority.
 *      L2SW_ERR_TBL_FULL         - Table is full.
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 * Note:
 *      The incoming packet which match the protocol-and-port-based vlan will use the configure vid for ingress pipeline
 *      The frame type is shown in the following:
 *      - FRAME_TYPE_ETHERNET
 *      - FRAME_TYPE_RFC1042
 *      - FRAME_TYPE_LLCOTHER
 */
extern l2sw_api_ret_t l2sw_vlan_protoAndPortBasedVlan_add(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_protoAndPortInfo_t *pInfo);

/* Function Name:
 *      l2sw_vlan_protoAndPortBasedVlan_get
 * Description:
 *      Get the protocol-and-port-based vlan to the specified port of device.
 * Input:
 *      port - Port id.
 *      proto_type - protocol-and-port-based vlan protocol type.
 *      frame_type - protocol-and-port-based vlan frame type.
 * Output:
 *      pInfo - Protocol and port based VLAN configuration information.
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 *      L2SW_ERR_TBL_FULL         - Table is full.
 * Note:
 *     The incoming packet which match the protocol-and-port-based vlan will use the configure vid for ingress pipeline
 *     The frame type is shown in the following:
 *      - FRAME_TYPE_ETHERNET
 *      - FRAME_TYPE_RFC1042
 *      - FRAME_TYPE_LLCOTHER
 */
extern l2sw_api_ret_t l2sw_vlan_protoAndPortBasedVlan_get(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_proto_type_t proto_type, l2sw_vlan_protoVlan_frameType_t frame_type, l2sw_vlan_protoAndPortInfo_t *pInfo);

/* Function Name:
 *      l2sw_vlan_protoAndPortBasedVlan_del
 * Description:
 *      Delete the protocol-and-port-based vlan from the specified port of device.
 * Input:
 *      port        - Port id.
 *      proto_type  - protocol-and-port-based vlan protocol type.
 *      frame_type  - protocol-and-port-based vlan frame type.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 *      L2SW_ERR_TBL_FULL         - Table is full.
 * Note:
 *     The incoming packet which match the protocol-and-port-based vlan will use the configure vid for ingress pipeline
 *     The frame type is shown in the following:
 *      - FRAME_TYPE_ETHERNET
 *      - FRAME_TYPE_RFC1042
 *      - FRAME_TYPE_LLCOTHER
 */
extern l2sw_api_ret_t l2sw_vlan_protoAndPortBasedVlan_del(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_proto_type_t proto_type, l2sw_vlan_protoVlan_frameType_t frame_type);

/* Function Name:
 *      l2sw_vlan_protoAndPortBasedVlan_delAll
 * Description:
 *     Delete all protocol-and-port-based vlans from the specified port of device.
 * Input:
 *      port - Port id.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 * Note:
 *     The incoming packet which match the protocol-and-port-based vlan will use the configure vid for ingress pipeline
 *     Delete all flow table protocol-and-port-based vlan entries.
 */
extern l2sw_api_ret_t l2sw_vlan_protoAndPortBasedVlan_delAll(l2sw_uint8 units, l2sw_port_t port);

/* Function Name:
 *      l2sw_vlan_portFid_set
 * Description:
 *      Set port-based filtering database
 * Input:
 *      port - Port id.
 *      enable - ebable port-based FID
 *      fid - Specified filtering database.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_L2_FID - Invalid fid.
 *      L2SW_ERR_INPUT - Invalid input parameter.
 *      L2SW_ERR_PORT_ID - Invalid port ID.
 * Note:
 *      The API can set port-based filtering database. If the function is enabled, all input
 *      packets will be assigned to the port-based fid regardless vlan tag.
 */
extern l2sw_api_ret_t l2sw_vlan_portFid_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable, l2sw_fid_t fid);

/* Function Name:
 *      l2sw_vlan_portFid_get
 * Description:
 *      Get port-based filtering database
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - ebable port-based FID
 *      pFid - Specified filtering database.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT - Invalid input parameters.
 *      L2SW_ERR_PORT_ID - Invalid port ID.
 * Note:
 *      The API can get port-based filtering database status. If the function is enabled, all input
 *      packets will be assigned to the port-based fid regardless vlan tag.
 */
extern l2sw_api_ret_t l2sw_vlan_portFid_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable, l2sw_fid_t *pFid);

/* Function Name:
 *      l2sw_vlan_UntagDscpPriorityEnable_set
 * Description:
 *      Set Untag DSCP priority assign
 * Input:
 *      enable - state of Untag DSCP priority assign
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_ENABLE          - Invalid input parameters.
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_vlan_UntagDscpPriorityEnable_set(l2sw_uint8 units, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_vlan_UntagDscpPriorityEnable_get
 * Description:
 *      Get Untag DSCP priority assign
 * Input:
 *      None
 * Output:
 *      pEnable - state of Untag DSCP priority assign
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_NULL_POINTER    - Null pointer
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_vlan_UntagDscpPriorityEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnable);


/*Spanning Tree*/
/* Function Name:
 *      l2sw_stp_mstpState_set
 * Description:
 *      Configure spanning tree state per each port.
 * Input:
 *      port - Port id
 *      msti - Multiple spanning tree instance.
 *      stp_state - Spanning tree state for msti
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_MSTI         - Invalid msti parameter.
 *      L2SW_ERR_MSTP_STATE   - Invalid STP state.
 * Note:
 *      System supports per-port multiple spanning tree state for each msti.
 *      There are four states supported by ASIC.
 *      - STP_STATE_DISABLED
 *      - STP_STATE_BLOCKING
 *      - STP_STATE_LEARNING
 *      - STP_STATE_FORWARDING
 */
extern l2sw_api_ret_t l2sw_stp_mstpState_set(l2sw_uint8 units, l2sw_stp_msti_id_t msti, l2sw_port_t port, l2sw_stp_state_t stp_state);

/* Function Name:
 *      l2sw_stp_mstpState_get
 * Description:
 *      Get spanning tree state per each port.
 * Input:
 *      port - Port id.
 *      msti - Multiple spanning tree instance.
 * Output:
 *      pStp_state - Spanning tree state for msti
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_MSTI         - Invalid msti parameter.
 * Note:
 *      System supports per-port multiple spanning tree state for each msti.
 *      There are four states supported by ASIC.
 *      - STP_STATE_DISABLED
 *      - STP_STATE_BLOCKING
 *      - STP_STATE_LEARNING
 *      - STP_STATE_FORWARDING
 */
extern l2sw_api_ret_t l2sw_stp_mstpState_get(l2sw_uint8 units, l2sw_stp_msti_id_t msti, l2sw_port_t port, l2sw_stp_state_t *pStp_state);

/* Function Name:
 *      l2sw_vlan_checkAndCreateMbr
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
 *      L2SW_ERR_VLAN_ENTRY_NOT_FOUND - VLAN not found
 *      L2SW_ERR_TBL_FULL     - Member Configuration table full
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_vlan_checkAndCreateMbr(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_uint32 *pIndex);

/* Function Name:
 *      l2sw_vlan_reservedVidAction_set
 * Description:
 *      Set Action of VLAN ID = 0 & 4095 tagged packet
 * Input:
 *      action_vid0     - Action for VID 0.
 *      action_vid4095  - Action for VID 4095.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_vlan_reservedVidAction_set(l2sw_uint8 units, l2sw_vlan_resVidAction_t action_vid0, l2sw_vlan_resVidAction_t action_vid4095);

/* Function Name:
 *      l2sw_vlan_reservedVidAction_get
 * Description:
 *      Get Action of VLAN ID = 0 & 4095 tagged packet
 * Input:
 *      pAction_vid0     - Action for VID 0.
 *      pAction_vid4095  - Action for VID 4095.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_NULL_POINTER - NULL Pointer
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_vlan_reservedVidAction_get(l2sw_uint8 units, l2sw_vlan_resVidAction_t *pAction_vid0, l2sw_vlan_resVidAction_t *pAction_vid4095);

/* Function Name:
 *      l2sw_vlan_realKeepRemarkEnable_set
 * Description:
 *      Set Real keep 1p remarking feature
 * Input:
 *      enabled     - State of 1p remarking at real keep packet
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_vlan_realKeepRemarkEnable_set(l2sw_uint8 units, l2sw_enable_t enabled);

/* Function Name:
 *      l2sw_vlan_realKeepRemarkEnable_get
 * Description:
 *      Get Real keep 1p remarking feature
 * Input:
 *      None.
 * Output:
 *      pEnabled     - State of 1p remarking at real keep packet
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_vlan_realKeepRemarkEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnabled);

/* Function Name:
 *      l2sw_vlan_reset
 * Description:
 *      Reset VLAN
 * Input:
 *      None.
 * Output:
 *      pEnabled     - State of 1p remarking at real keep packet
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 *
 */
l2sw_api_ret_t l2sw_vlan_reset(l2sw_uint8 units);

/* Function Name:
 *      l2sw_vlan_reset_show
 * Description:
 *      show Reset VLAN state
 * Input:
 *      None.
 * Output:
 *      pEnabled     - State of vlan_reset
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 *
 */
l2sw_api_ret_t l2sw_vlan_reset_show(l2sw_uint8 units, l2sw_enable_t *pEnabled);

/* Function Name:
 *      l2sw_vlan_protoAndPortBasedVlan_show
 * Description:
 *      Reset VLAN
 * Input:
 *      l2sw_port_t port     - port id 
        l2sw_uint32 index    - register index
 * Output:
 *      pInfo               - protoAndPortBasedVlan info
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_vlan_protoAndPortBasedVlan_show(l2sw_uint8 units, l2sw_port_t port, l2sw_uint32 index, l2sw_vlan_protoAndPortInfo_t *pInfo);


#endif /* __L2SW_API_VLAN_H__ */
