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

#ifndef __L2SW_API_L2_H__
#define __L2SW_API_L2_H__

/*
 * Data Type Declaration
 */
#define L2SW_MAX_NUM_OF_LEARN_LIMIT(units)           (l2sw_switch_maxLutAddrNumber_get(units))

#define L2SW_MAC_ADDR_LEN                            6
#define L2SW_MAX_LUT_ADDRESS(units)                  (L2SW_MAX_NUM_OF_LEARN_LIMIT(units))
#define L2SW_MAX_LUT_ADDR_ID(units)                  (L2SW_MAX_LUT_ADDRESS(units) - 1)

#define L2SW_LEARN_LIMIT                             100
#define L2SW_OVER_LEARN_LIMIT                        0x00000020

typedef l2sw_uint32 l2sw_l2_age_time_t;

typedef enum l2sw_l2_flood_type_e
{
    FLOOD_UNKNOWNDA = 0,
    FLOOD_UNKNOWNMC,
    FLOOD_BC,
    FLOOD_END
} l2sw_l2_flood_type_t;

typedef l2sw_uint32 l2sw_l2_flushItem_t;

typedef enum l2sw_l2_flushType_e
{
    FLUSH_TYPE_BY_PORT = 0,       /* physical port       */
    FLUSH_TYPE_BY_PORT_VID,       /* physical port + VID */
    FLUSH_TYPE_BY_PORT_FID,       /* physical port + FID */
    FLUSH_TYPE_END
} l2sw_l2_flushType_t;

typedef struct l2sw_l2_flushCfg_s
{
    l2sw_enable_t    flushByVid;
    l2sw_vlan_t      vid;
    l2sw_enable_t    flushByFid;
    l2sw_uint32      fid;
    l2sw_enable_t    flushByPort;
    l2sw_port_t      port;
    l2sw_enable_t    flushByMac;
    l2sw_mac_t       ucastAddr;
    l2sw_enable_t    flushStaticAddr;
    l2sw_enable_t    flushAddrOnAllPorts; /* this is used when flushByVid */
} l2sw_l2_flushCfg_t;

typedef enum l2sw_l2_read_method_e{

    READMETHOD_MAC = 0,
    READMETHOD_ADDRESS,
    READMETHOD_NEXT_ADDRESS,
    READMETHOD_NEXT_L2UC,
    READMETHOD_NEXT_L2MC,
    READMETHOD_NEXT_L3MC,
    READMETHOD_NEXT_L2L3MC,
    READMETHOD_NEXT_L2UCSPA,
    READMETHOD_END
}l2sw_l2_read_method_t;

/* l2 limit learning count action */
typedef enum l2sw_l2_limitLearnCntAction_e
{
    LIMIT_LEARN_CNT_ACTION_DROP = 0,
    LIMIT_LEARN_CNT_ACTION_FORWARD,
    LIMIT_LEARN_CNT_ACTION_TO_CPU,
    LIMIT_LEARN_CNT_ACTION_END
} l2sw_l2_limitLearnCntAction_t;

typedef enum l2sw_l2_ipmc_lookup_type_e
{
    LOOKUP_MAC = 0,
    LOOKUP_IP,
    LOOKUP_IP_VID,
    LOOKUP_END
} l2sw_l2_ipmc_lookup_type_t;

/* l2 address table - unicast data structure */
typedef struct l2sw_l2_ucastAddr_s
{
    l2sw_mac_t       mac;
    l2sw_uint32      ivl;
    l2sw_uint32      cvid;
    l2sw_uint32      fid;
    l2sw_uint32      efid;
    l2sw_uint32      port;
    l2sw_uint32      sa_block;
    l2sw_uint32      da_block;
    l2sw_uint32      auth;
    l2sw_uint32      is_static;
    l2sw_uint32      priority;
    l2sw_uint32      sa_pri_en;
    l2sw_uint32      fwd_pri_en;
    l2sw_uint32      address;
}l2sw_l2_ucastAddr_t;

/* l2 address table - multicast data structure */
typedef struct l2sw_l2_mcastAddr_s
{
    l2sw_uint32      vid;
    l2sw_mac_t       mac;
    l2sw_uint32      fid;
    l2sw_portmask_t  portmask;
    l2sw_uint32      ivl;
    l2sw_uint32      priority;
    l2sw_uint32      fwd_pri_en;
    l2sw_uint32      igmp_asic;
    l2sw_uint32      igmp_index;
    l2sw_uint32      address;
}l2sw_l2_mcastAddr_t;

/* l2 address table - ip multicast data structure */
typedef struct l2sw_l2_ipMcastAddr_s
{
    ipaddr_t        dip;
    ipaddr_t        sip;
    l2sw_portmask_t  portmask;
    l2sw_uint32      priority;
    l2sw_uint32      fwd_pri_en;
    l2sw_uint32      igmp_asic;
    l2sw_uint32      igmp_index;
    l2sw_uint32      address;
}l2sw_l2_ipMcastAddr_t;

/* l2 address table - ip VID multicast data structure */
typedef struct l2sw_l2_ipVidMcastAddr_s
{
    ipaddr_t        dip;
    ipaddr_t        sip;
    l2sw_uint32      vid;
    l2sw_portmask_t  portmask;
    l2sw_uint32      address;
}l2sw_l2_ipVidMcastAddr_t;

typedef struct l2sw_l2_addr_table_s
{
    l2sw_uint32  index;
    ipaddr_t    sip;
    ipaddr_t    dip;
    l2sw_mac_t   mac;
    l2sw_uint32  sa_block;
    l2sw_uint32  auth;
    l2sw_portmask_t  portmask;
    l2sw_uint32  age;
    l2sw_uint32  ivl;
    l2sw_uint32  cvid;
    l2sw_uint32  fid;
    l2sw_uint32  is_ipmul;
    l2sw_uint32  is_static;
    l2sw_uint32  is_ipvidmul;
    l2sw_uint32  l3_vid;
}l2sw_l2_addr_table_t;

typedef enum l2sw_l2_clearStatus_e
{
    L2_CLEAR_STATE_FINISH = 0,
    L2_CLEAR_STATE_BUSY,
    L2_CLEAR_STATE_END
}l2sw_l2_clearStatus_t;

/* Function Name:
 *      l2sw_l2_init
 * Description:
 *      Initialize l2 module of the specified device.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK          - OK
 *      L2SW_ERR_FAILED      - Failed
 *      L2SW_ERR_SMI         - SMI access error
 * Note:
 *      Initialize l2 module before calling any l2 APIs.
 */
extern l2sw_api_ret_t l2sw_l2_init(l2sw_uint8 units);

/* Function Name:
 *      l2sw_l2_addr_add
 * Description:
 *      Add LUT unicast entry.
 * Input:
 *      pMac - 6 bytes unicast(I/G bit is 0) mac address to be written into LUT.
 *      pL2_data - Unicast entry parameter
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_MAC              - Invalid MAC address.
 *      L2SW_ERR_L2_FID           - Invalid FID .
 *      L2SW_ERR_L2_INDEXTBL_FULL - hashed index is full of entries.
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 * Note:
 *      If the unicast mac address already existed in LUT, it will udpate the status of the entry.
 *      Otherwise, it will find an empty or asic auto learned entry to write. If all the entries
 *      with the same hash value can't be replaced, ASIC will return a L2SW_ERR_L2_INDEXTBL_FULL error.
 */
extern l2sw_api_ret_t l2sw_l2_addr_add(l2sw_uint8 units, l2sw_mac_t *pMac, l2sw_l2_ucastAddr_t *pL2_data);

/* Function Name:
 *      l2sw_l2_addr_get
 * Description:
 *      Get LUT unicast entry.
 * Input:
 *      pMac    - 6 bytes unicast(I/G bit is 0) mac address to be written into LUT.
 * Output:
 *      pL2_data - Unicast entry parameter
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number.
 *      L2SW_ERR_MAC                  - Invalid MAC address.
 *      L2SW_ERR_L2_FID               - Invalid FID .
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      If the unicast mac address existed in LUT, it will return the port and fid where
 *      the mac is learned. Otherwise, it will return a L2SW_ERR_L2_ENTRY_NOTFOUND error.
 */
extern l2sw_api_ret_t l2sw_l2_addr_get(l2sw_uint8 units, l2sw_mac_t *pMac, l2sw_l2_ucastAddr_t *pL2_data);

/* Function Name:
 *      l2sw_l2_addr_next_get
 * Description:
 *      Get Next LUT unicast entry.
 * Input:
 *      read_method     - The reading method.
 *      port            - The port number if the read_metohd is READMETHOD_NEXT_L2UCSPA
 *      pAddress        - The Address ID
 * Output:
 *      pL2_data - Unicast entry parameter
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number.
 *      L2SW_ERR_MAC                  - Invalid MAC address.
 *      L2SW_ERR_L2_FID               - Invalid FID .
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      Get the next unicast entry after the current entry pointed by pAddress.
 *      The address of next entry is returned by pAddress. User can use (address + 1)
 *      as pAddress to call this API again for dumping all entries is LUT.
 */
extern l2sw_api_ret_t l2sw_l2_addr_next_get(l2sw_uint8 units, l2sw_l2_read_method_t read_method, l2sw_port_t port, l2sw_uint32 *pAddress, l2sw_l2_ucastAddr_t *pL2_data);

/* Function Name:
 *      l2sw_l2_addr_del
 * Description:
 *      Delete LUT unicast entry.
 * Input:
 *      pMac     - 6 bytes unicast(I/G bit is 0) mac address to be written into LUT.
 *      pL2_data - Unicast entry parameter
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number.
 *      L2SW_ERR_MAC                  - Invalid MAC address.
 *      L2SW_ERR_L2_FID               - Invalid FID .
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      If the mac has existed in the LUT, it will be deleted. Otherwise, it will return L2SW_ERR_L2_ENTRY_NOTFOUND.
 */
extern l2sw_api_ret_t l2sw_l2_addr_del(l2sw_uint8 units, l2sw_mac_t *pMac, l2sw_l2_ucastAddr_t *pL2_data);

/* Function Name:
 *      l2sw_l2_mcastAddr_add
 * Description:
 *      Add LUT multicast entry.
 * Input:
 *      pMcastAddr  - L2 multicast entry structure
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_MAC              - Invalid MAC address.
 *      L2SW_ERR_L2_FID           - Invalid FID .
 *      L2SW_ERR_L2_VID           - Invalid VID .
 *      L2SW_ERR_L2_INDEXTBL_FULL - hashed index is full of entries.
 *      L2SW_ERR_PORT_MASK        - Invalid portmask.
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 * Note:
 *      If the multicast mac address already existed in the LUT, it will udpate the
 *      port mask of the entry. Otherwise, it will find an empty or asic auto learned
 *      entry to write. If all the entries with the same hash value can't be replaced,
 *      ASIC will return a L2SW_ERR_L2_INDEXTBL_FULL error.
 */
extern l2sw_api_ret_t l2sw_l2_mcastAddr_add(l2sw_uint8 units, l2sw_l2_mcastAddr_t *pMcastAddr);

/* Function Name:
 *      l2sw_l2_mcastAddr_get
 * Description:
 *      Get LUT multicast entry.
 * Input:
 *      pMcastAddr  - L2 multicast entry structure
 * Output:
 *      pMcastAddr  - L2 multicast entry structure
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_MAC                  - Invalid MAC address.
 *      L2SW_ERR_L2_FID               - Invalid FID .
 *      L2SW_ERR_L2_VID               - Invalid VID .
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      If the multicast mac address existed in the LUT, it will return the port where
 *      the mac is learned. Otherwise, it will return a L2SW_ERR_L2_ENTRY_NOTFOUND error.
 */
extern l2sw_api_ret_t l2sw_l2_mcastAddr_get(l2sw_uint8 units, l2sw_l2_mcastAddr_t *pMcastAddr);

/* Function Name:
 *      l2sw_l2_mcastAddr_next_get
 * Description:
 *      Get Next L2 Multicast entry.
 * Input:
 *      pAddress        - The Address ID
 * Output:
 *      pMcastAddr  - L2 multicast entry structure
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      Get the next L2 multicast entry after the current entry pointed by pAddress.
 *      The address of next entry is returned by pAddress. User can use (address + 1)
 *      as pAddress to call this API again for dumping all multicast entries is LUT.
 */
extern l2sw_api_ret_t l2sw_l2_mcastAddr_next_get(l2sw_uint8 units, l2sw_uint32 *pAddress, l2sw_l2_mcastAddr_t *pMcastAddr);

/* Function Name:
 *      l2sw_l2_mcastAddr_del
 * Description:
 *      Delete LUT multicast entry.
 * Input:
 *      pMcastAddr  - L2 multicast entry structure
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_MAC                  - Invalid MAC address.
 *      L2SW_ERR_L2_FID               - Invalid FID .
 *      L2SW_ERR_L2_VID               - Invalid VID .
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      If the mac has existed in the LUT, it will be deleted. Otherwise, it will return L2SW_ERR_L2_ENTRY_NOTFOUND.
 */
extern l2sw_api_ret_t l2sw_l2_mcastAddr_del(l2sw_uint8 units, l2sw_l2_mcastAddr_t *pMcastAddr);

/* Function Name:
 *      l2sw_l2_ipMcastAddr_add
 * Description:
 *      Add Lut IP multicast entry
 * Input:
 *      pIpMcastAddr    - IP Multicast entry
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_L2_INDEXTBL_FULL - hashed index is full of entries.
 *      L2SW_ERR_PORT_MASK        - Invalid portmask.
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 * Note:
 *      System supports L2 entry with IP multicast DIP/SIP to forward IP multicasting frame as user
 *      desired. If this function is enabled, then system will be looked up L2 IP multicast entry to
 *      forward IP multicast frame directly without flooding.
 */
extern l2sw_api_ret_t l2sw_l2_ipMcastAddr_add(l2sw_uint8 units, l2sw_l2_ipMcastAddr_t *pIpMcastAddr);

/* Function Name:
 *      l2sw_l2_ipMcastAddr_get
 * Description:
 *      Get LUT IP multicast entry.
 * Input:
 *      pIpMcastAddr    - IP Multicast entry
 * Output:
 *      pIpMcastAddr    - IP Multicast entry
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      The API can get Lut table of IP multicast entry.
 */
extern l2sw_api_ret_t l2sw_l2_ipMcastAddr_get(l2sw_uint8 units, l2sw_l2_ipMcastAddr_t *pIpMcastAddr);

/* Function Name:
 *      l2sw_l2_ipMcastAddr_next_get
 * Description:
 *      Get Next IP Multicast entry.
 * Input:
 *      pAddress        - The Address ID
 * Output:
 *      pIpMcastAddr    - IP Multicast entry
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      Get the next IP multicast entry after the current entry pointed by pAddress.
 *      The address of next entry is returned by pAddress. User can use (address + 1)
 *      as pAddress to call this API again for dumping all IP multicast entries is LUT.
 */
extern l2sw_api_ret_t l2sw_l2_ipMcastAddr_next_get(l2sw_uint8 units, l2sw_uint32 *pAddress, l2sw_l2_ipMcastAddr_t *pIpMcastAddr);

/* Function Name:
 *      l2sw_l2_ipMcastAddr_del
 * Description:
 *      Delete a ip multicast address entry from the specified device.
 * Input:
 *      pIpMcastAddr    - IP Multicast entry
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      The API can delete a IP multicast address entry from the specified device.
 */
extern l2sw_api_ret_t l2sw_l2_ipMcastAddr_del(l2sw_uint8 units, l2sw_l2_ipMcastAddr_t *pIpMcastAddr);

/* Function Name:
 *      l2sw_l2_ipVidMcastAddr_add
 * Description:
 *      Add Lut IP multicast+VID entry
 * Input:
 *      pIpVidMcastAddr - IP & VID multicast Entry
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_L2_INDEXTBL_FULL - hashed index is full of entries.
 *      L2SW_ERR_PORT_MASK        - Invalid portmask.
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_l2_ipVidMcastAddr_add(l2sw_uint8 units, l2sw_l2_ipVidMcastAddr_t *pIpVidMcastAddr);

/* Function Name:
 *      l2sw_l2_ipVidMcastAddr_get
 * Description:
 *      Get LUT IP multicast+VID entry.
 * Input:
 *      pIpVidMcastAddr - IP & VID multicast Entry
 * Output:
 *      pIpVidMcastAddr - IP & VID multicast Entry
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_l2_ipVidMcastAddr_get(l2sw_uint8 units, l2sw_l2_ipVidMcastAddr_t *pIpVidMcastAddr);

/* Function Name:
 *      l2sw_l2_ipVidMcastAddr_next_get
 * Description:
 *      Get Next IP Multicast+VID entry.
 * Input:
 *      pAddress        - The Address ID
 * Output:
 *      pIpVidMcastAddr - IP & VID multicast Entry
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      Get the next IP multicast entry after the current entry pointed by pAddress.
 *      The address of next entry is returned by pAddress. User can use (address + 1)
 *      as pAddress to call this API again for dumping all IP multicast entries is LUT.
 */
extern l2sw_api_ret_t l2sw_l2_ipVidMcastAddr_next_get(l2sw_uint8 units, l2sw_uint32 *pAddress, l2sw_l2_ipVidMcastAddr_t *pIpVidMcastAddr);

/* Function Name:
 *      l2sw_l2_ipVidMcastAddr_del
 * Description:
 *      Delete a ip multicast+VID address entry from the specified device.
 * Input:
 *      pIpVidMcastAddr - IP & VID multicast Entry
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_l2_ipVidMcastAddr_del(l2sw_uint8 units, l2sw_l2_ipVidMcastAddr_t *pIpVidMcastAddr);

/* Function Name:
 *      l2sw_l2_ucastAddr_flush
 * Description:
 *      Flush L2 mac address by type in the specified device (both dynamic and static).
 * Input:
 *      pConfig - flush configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_VLAN_VID     - Invalid VID parameter.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      flushByVid          - 1: Flush by VID, 0: Don't flush by VID
 *      vid                 - VID (0 ~ 4095)
 *      flushByFid          - 1: Flush by FID, 0: Don't flush by FID
 *      fid                 - FID (0 ~ 15)
 *      flushByPort         - 1: Flush by Port, 0: Don't flush by Port
 *      port                - Port ID
 *      flushByMac          - Not Supported
 *      ucastAddr           - Not Supported
 *      flushStaticAddr     - 1: Flush both Static and Dynamic entries, 0: Flush only Dynamic entries
 *      flushAddrOnAllPorts - 1: Flush VID-matched entries at all ports, 0: Flush VID-matched entries per port.
 */
extern l2sw_api_ret_t l2sw_l2_ucastAddr_flush(l2sw_uint8 units, l2sw_l2_flushCfg_t *pConfig);

/* Function Name:
 *      l2sw_l2_table_clear
 * Description:
 *      Flush all static & dynamic entries in LUT.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_l2_table_clear(l2sw_uint8 units);

/* Function Name:
 *      l2sw_l2_table_clearStatus_get
 * Description:
 *      Get table clear status
 * Input:
 *      None
 * Output:
 *      pStatus - Clear status, 1:Busy, 0:finish
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_l2_table_clearStatus_get(l2sw_uint8 units, l2sw_l2_clearStatus_t *pStatus);

/* Function Name:
 *      l2sw_l2_flushLinkDownPortAddrEnable_set
 * Description:
 *      Set HW flush linkdown port mac configuration of the specified device.
 * Input:
 *      port - Port id.
 *      enable - link down flush status
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_ENABLE       - Invalid enable input.
 * Note:
 *      The status of flush linkdown port address is as following:
 *      - DISABLED
 *      - ENABLED
 */
extern l2sw_api_ret_t l2sw_l2_flushLinkDownPortAddrEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_l2_flushLinkDownPortAddrEnable_get
 * Description:
 *      Get HW flush linkdown port mac configuration of the specified device.
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - link down flush status
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The status of flush linkdown port address is as following:
 *      - DISABLED
 *      - ENABLED
 */
extern l2sw_api_ret_t l2sw_l2_flushLinkDownPortAddrEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_l2_agingEnable_set
 * Description:
 *      Set L2 LUT aging status per port setting.
 * Input:
 *      port    - Port id.
 *      enable  - Aging status
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_ENABLE       - Invalid enable input.
 * Note:
 *      This API can be used to set L2 LUT aging status per port.
 */
extern l2sw_api_ret_t l2sw_l2_agingEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_l2_agingEnable_get
 * Description:
 *      Get L2 LUT aging status per port setting.
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - Aging status
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API can be used to get L2 LUT aging function per port.
 */
extern l2sw_api_ret_t l2sw_l2_agingEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_l2_limitLearningCnt_set
 * Description:
 *      Set per-Port auto learning limit number
 * Input:
 *      port    - Port id.
 *      mac_cnt - Auto learning entries limit number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number.
 *      L2SW_ERR_LIMITED_L2ENTRY_NUM  - Invalid auto learning limit number
 * Note:
 *      The API can set per-port ASIC auto learning limit number from 0(disable learning)
 *      to 8k.
 */
extern l2sw_api_ret_t l2sw_l2_limitLearningCnt_set(l2sw_uint8 units, l2sw_port_t port, l2sw_mac_cnt_t mac_cnt);

/* Function Name:
 *      l2sw_l2_limitLearningCnt_get
 * Description:
 *      Get per-Port auto learning limit number
 * Input:
 *      port - Port id.
 * Output:
 *      pMac_cnt - Auto learning entries limit number
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get per-port ASIC auto learning limit number.
 */
extern l2sw_api_ret_t l2sw_l2_limitLearningCnt_get(l2sw_uint8 units, l2sw_port_t port, l2sw_mac_cnt_t *pMac_cnt);

/* Function Name:
 *      l2sw_l2_limitSystemLearningCnt_set
 * Description:
 *      Set System auto learning limit number
 * Input:
 *      mac_cnt - Auto learning entries limit number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_LIMITED_L2ENTRY_NUM  - Invalid auto learning limit number
 * Note:
 *      The API can set system ASIC auto learning limit number from 0(disable learning)
 *      to 2112.
 */
extern l2sw_api_ret_t l2sw_l2_limitSystemLearningCnt_set(l2sw_uint8 units, l2sw_mac_cnt_t mac_cnt);

/* Function Name:
 *      l2sw_l2_limitSystemLearningCnt_get
 * Description:
 *      Get System auto learning limit number
 * Input:
 *      None
 * Output:
 *      pMac_cnt - Auto learning entries limit number
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get system ASIC auto learning limit number.
 */
extern l2sw_api_ret_t l2sw_l2_limitSystemLearningCnt_get(l2sw_uint8 units, l2sw_mac_cnt_t *pMac_cnt);

/* Function Name:
 *      l2sw_l2_limitLearningCntAction_set
 * Description:
 *      Configure auto learn over limit number action.
 * Input:
 *      port   - Port id.
 *      action - Learn over action
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_NOT_ALLOWED  - Invalid learn over action
 * Note:
 *      The API can set SA unknown packet action while auto learn limit number is over
 *      The action symbol as following:
 *      - LIMIT_LEARN_CNT_ACTION_DROP,
 *      - LIMIT_LEARN_CNT_ACTION_FORWARD,
 *      - LIMIT_LEARN_CNT_ACTION_TO_CPU,
 */
extern l2sw_api_ret_t l2sw_l2_limitLearningCntAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_l2_limitLearnCntAction_t action);

/* Function Name:
 *      l2sw_l2_limitLearningCntAction_get
 * Description:
 *      Get auto learn over limit number action.
 * Input:
 *      port - Port id.
 * Output:
 *      pAction - Learn over action
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get SA unknown packet action while auto learn limit number is over
 *      The action symbol as following:
 *      - LIMIT_LEARN_CNT_ACTION_DROP,
 *      - LIMIT_LEARN_CNT_ACTION_FORWARD,
 *      - LIMIT_LEARN_CNT_ACTION_TO_CPU,
 */
extern l2sw_api_ret_t l2sw_l2_limitLearningCntAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_l2_limitLearnCntAction_t *pAction);

/* Function Name:
 *      l2sw_l2_limitSystemLearningCntAction_set
 * Description:
 *      Configure system auto learn over limit number action.
 * Input:
 *      port   - Port id.
 *      action - Learn over action
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_NOT_ALLOWED  - Invalid learn over action
 * Note:
 *      The API can set SA unknown packet action while auto learn limit number is over
 *      The action symbol as following:
 *      - LIMIT_LEARN_CNT_ACTION_DROP,
 *      - LIMIT_LEARN_CNT_ACTION_FORWARD,
 *      - LIMIT_LEARN_CNT_ACTION_TO_CPU,
 */
extern l2sw_api_ret_t l2sw_l2_limitSystemLearningCntAction_set(l2sw_uint8 units, l2sw_l2_limitLearnCntAction_t action);

/* Function Name:
 *      l2sw_l2_limitSystemLearningCntAction_get
 * Description:
 *      Get system auto learn over limit number action.
 * Input:
 *      None.
 * Output:
 *      pAction - Learn over action
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get SA unknown packet action while auto learn limit number is over
 *      The action symbol as following:
 *      - LIMIT_LEARN_CNT_ACTION_DROP,
 *      - LIMIT_LEARN_CNT_ACTION_FORWARD,
 *      - LIMIT_LEARN_CNT_ACTION_TO_CPU,
 */
extern l2sw_api_ret_t l2sw_l2_limitSystemLearningCntAction_get(l2sw_uint8 units, l2sw_l2_limitLearnCntAction_t *pAction);

/* Function Name:
 *      l2sw_l2_limitSystemLearningCntPortMask_set
 * Description:
 *      Configure system auto learn portmask
 * Input:
 *      pPortmask - Port Mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_MASK    - Invalid port mask.
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_l2_limitSystemLearningCntPortMask_set(l2sw_uint8 units, l2sw_portmask_t *pPortmask);

/* Function Name:
 *      l2sw_l2_limitSystemLearningCntPortMask_get
 * Description:
 *      get system auto learn portmask
 * Input:
 *      None
 * Output:
 *      pPortmask - Port Mask
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_NULL_POINTER - Null pointer.
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_l2_limitSystemLearningCntPortMask_get(l2sw_uint8 units, l2sw_portmask_t *pPortmask);

/* Function Name:
 *      l2sw_l2_limitlearningSpeed_set
 * Description:
 *      Set per-Port per-Second current auto learning number 
 * Input:
 *      port      - Port id.
 *      enabled   - Port Status.
 *      mac_cnt   - Auto learning entries limit number per-Second.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_OVER_LEARN_LIMIT     - Invalid auto learning limit number.
 * Note:
 *      The API can set per-Second per-port ASIC auto learning number enable or disable
 */
extern l2sw_api_ret_t l2sw_l2_limitlearningSpeed_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enabled, l2sw_mac_cnt_t mac_cnt);

/* Function Name:
 *      l2sw_l2_limitlearningSpeed_get
 * Description:
 *      Get per-Port per-Second current auto learning number 
 * Input:
 *      port    - Port id.
 * Output:
 *      pMac_cnt  - Learning counts per-Second.   
 *      pEnabled  - Port Status.
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_NULL_POINTER     - NULL Points.
 * Note:
 *      The API can set per-Second per-port ASIC auto learning number enable or disable
 */
extern l2sw_api_ret_t l2sw_l2_limitlearningSpeed_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnabled, l2sw_mac_cnt_t *pMac_cnt);

/* Function Name:
 *      l2sw_l2_learningCnt_get
 * Description:
 *      Get per-Port current auto learning number
 * Input:
 *      port - Port id.
 * Output:
 *      pMac_cnt - ASIC auto learning entries number
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get per-port ASIC auto learning number
 */
extern l2sw_api_ret_t l2sw_l2_learningCnt_get(l2sw_uint8 units, l2sw_port_t port, l2sw_mac_cnt_t *pMac_cnt);

/* Function Name:
 *      l2sw_l2_floodPortMask_set
 * Description:
 *      Set flooding portmask
 * Input:
 *      type - flooding type.
 *      pFlood_portmask - flooding porkmask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_MASK    - Invalid portmask.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can set the flooding mask.
 *      The flooding type is as following:
 *      - FLOOD_UNKNOWNDA
 *      - FLOOD_UNKNOWNMC
 *      - FLOOD_BC
 */
extern l2sw_api_ret_t l2sw_l2_floodPortMask_set(l2sw_uint8 units, l2sw_l2_flood_type_t floood_type, l2sw_portmask_t *pFlood_portmask);

/* Function Name:
 *      l2sw_l2_floodPortMask_get
 * Description:
 *      Get flooding portmask
 * Input:
 *      type - flooding type.
 * Output:
 *      pFlood_portmask - flooding porkmask
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API can get the flooding mask.
 *      The flooding type is as following:
 *      - FLOOD_UNKNOWNDA
 *      - FLOOD_UNKNOWNMC
 *      - FLOOD_BC
 */
extern l2sw_api_ret_t l2sw_l2_floodPortMask_get(l2sw_uint8 units, l2sw_l2_flood_type_t floood_type, l2sw_portmask_t *pFlood_portmask);

/* Function Name:
 *      l2sw_l2_localPktPermit_set
 * Description:
 *      Set permittion of frames if source port and destination port are the same.
 * Input:
 *      port - Port id.
 *      permit - permittion status
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_ENABLE       - Invalid permit value.
 * Note:
 *      This API is setted to permit frame if its source port is equal to destination port.
 */
extern l2sw_api_ret_t l2sw_l2_localPktPermit_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t permit);

/* Function Name:
 *      l2sw_l2_localPktPermit_get
 * Description:
 *      Get permittion of frames if source port and destination port are the same.
 * Input:
 *      port - Port id.
 * Output:
 *      pPermit - permittion status
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API is to get permittion status for frames if its source port is equal to destination port.
 */
extern l2sw_api_ret_t l2sw_l2_localPktPermit_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pPermit);

/* Function Name:
 *      l2sw_l2_aging_set
 * Description:
 *      Set LUT aging out speed
 * Input:
 *      aging_time - Aging out time.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 * Note:
 *      The API can set LUT agging out period for each entry and the range is from 14s to 800s.
 */
extern l2sw_api_ret_t l2sw_l2_aging_set(l2sw_uint8 units, l2sw_l2_age_time_t aging_time);

/* Function Name:
 *      l2sw_l2_aging_get
 * Description:
 *      Get LUT aging out time
 * Input:
 *      None
 * Output:
 *      pAging_time - Aging out time.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get LUT agging out period for each entry.
 */
extern l2sw_api_ret_t l2sw_l2_aging_get(l2sw_uint8 units, l2sw_l2_age_time_t *pAging_time);

/* Function Name:
 *      l2sw_l2_ipMcastAddrLookup_set
 * Description:
 *      Set Lut IP multicast lookup function
 * Input:
 *      type - Lookup type for IPMC packet.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK          - OK
 *      L2SW_ERR_FAILED      - Failed
 *      L2SW_ERR_SMI         - SMI access error
 * Note:
 *      This API can work with l2sw_l2_ipMcastAddrLookupException_add.
 *      If users set the lookup type to DIP, the group in exception table
 *      will be lookup by DIP+SIP
 *      If users set the lookup type to DIP+SIP, the group in exception table
 *      will be lookup by only DIP
 */
extern l2sw_api_ret_t l2sw_l2_ipMcastAddrLookup_set(l2sw_uint8 units, l2sw_l2_ipmc_lookup_type_t type);

/* Function Name:
 *      l2sw_l2_ipMcastAddrLookup_get
 * Description:
 *      Get Lut IP multicast lookup function
 * Input:
 *      None.
 * Output:
 *      pType - Lookup type for IPMC packet.
 * Return:
 *      L2SW_ERR_OK          - OK
 *      L2SW_ERR_FAILED      - Failed
 *      L2SW_ERR_SMI         - SMI access error
 * Note:
 *      None.
 */
extern l2sw_api_ret_t l2sw_l2_ipMcastAddrLookup_get(l2sw_uint8 units, l2sw_l2_ipmc_lookup_type_t *pType);

/* Function Name:
 *      l2sw_l2_ipMcastForwardRouterPort_set
 * Description:
 *      Set IPMC packet forward to rounter port also or not
 * Input:
 *      enabled - 1: Inlcude router port, 0, exclude router port
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK          - OK
 *      L2SW_ERR_FAILED      - Failed
 *      L2SW_ERR_SMI         - SMI access error
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_l2_ipMcastForwardRouterPort_set(l2sw_uint8 units, l2sw_enable_t enabled);

/* Function Name:
 *      l2sw_l2_ipMcastForwardRouterPort_get
 * Description:
 *      Get IPMC packet forward to rounter port also or not
 * Input:
 *      None.
 * Output:
 *      pEnabled    - 1: Inlcude router port, 0, exclude router port
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_NULL_POINTER - Null pointer
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_l2_ipMcastForwardRouterPort_get(l2sw_uint8 units, l2sw_enable_t *pEnabled);

/* Function Name:
 *      l2sw_l2_ipMcastGroupEntry_add
 * Description:
 *      Add an IP Multicast entry to group table
 * Input:
 *      ip_addr     - IP address
 *      vid         - VLAN ID
 *      pPortmask   - portmask
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK          - OK
 *      L2SW_ERR_FAILED      - Failed
 *      L2SW_ERR_SMI         - SMI access error
 *      L2SW_ERR_TBL_FULL    - Table Full
 * Note:
 *      Add an entry to IP Multicast Group table.
 */
extern l2sw_api_ret_t l2sw_l2_ipMcastGroupEntry_add(l2sw_uint8 units, ipaddr_t ip_addr, l2sw_uint32 vid, l2sw_portmask_t *pPortmask);

/* Function Name:
 *      l2sw_l2_ipMcastGroupEntry_del
 * Description:
 *      Delete an entry from IP Multicast group table
 * Input:
 *      ip_addr     - IP address
 *      vid         - VLAN ID
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK          - OK
 *      L2SW_ERR_FAILED      - Failed
 *      L2SW_ERR_SMI         - SMI access error
 *      L2SW_ERR_TBL_FULL    - Table Full
 * Note:
 *      Delete an entry from IP Multicast group table.
 */
extern l2sw_api_ret_t l2sw_l2_ipMcastGroupEntry_del(l2sw_uint8 units, ipaddr_t ip_addr, l2sw_uint32 vid);

/* Function Name:
 *      l2sw_l2_ipMcastGroupEntry_get
 * Description:
 *      get an entry from IP Multicast group table
 * Input:
 *      ip_addr     - IP address
 *      vid         - VLAN ID
 * Output:
 *      pPortmask   - member port mask
 * Return:
 *      L2SW_ERR_OK          - OK
 *      L2SW_ERR_FAILED      - Failed
 *      L2SW_ERR_SMI         - SMI access error
 *      L2SW_ERR_TBL_FULL    - Table Full
 * Note:
 *      Delete an entry from IP Multicast group table.
 */
extern l2sw_api_ret_t l2sw_l2_ipMcastGroupEntry_get(l2sw_uint8 units, ipaddr_t ip_addr, l2sw_uint32 vid, l2sw_portmask_t *pPortmask);

/* Function Name:
 *      l2sw_l2_entry_get
 * Description:
 *      Get LUT unicast entry.
 * Input:
 *      pL2_entry - Index field in the structure.
 * Output:
 *      pL2_entry - other fields such as MAC, port, age...
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_L2_EMPTY_ENTRY   - Empty LUT entry.
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 * Note:
 *      This API is used to get address by index from 0~2111.
 */
extern l2sw_api_ret_t l2sw_l2_entry_get(l2sw_uint8 units, l2sw_l2_addr_table_t *pL2_entry);


#endif /* __L2SW_API_L2_H__ */

