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

#ifndef __L2SW_API_STORM_H__
#define __L2SW_API_STORM_H__

#define STORM_UNUC_INDEX                            28
#define STORM_UNMC_INDEX                            29
#define STORM_MC_INDEX                              30
#define STORM_BC_INDEX                              31

typedef enum l2sw_rate_storm_group_e
{
    STORM_GROUP_UNKNOWN_UNICAST = 0,
    STORM_GROUP_UNKNOWN_MULTICAST,
    STORM_GROUP_MULTICAST,
    STORM_GROUP_BROADCAST,
    STORM_GROUP_END
} l2sw_rate_storm_group_t;

typedef enum l2sw_storm_bypass_e
{
    BYPASS_BRG_GROUP = 0,
    BYPASS_FD_PAUSE,
    BYPASS_SP_MCAST,
    BYPASS_1X_PAE,
    BYPASS_UNDEF_BRG_04,
    BYPASS_UNDEF_BRG_05,
    BYPASS_UNDEF_BRG_06,
    BYPASS_UNDEF_BRG_07,
    BYPASS_PROVIDER_BRIDGE_GROUP_ADDRESS,
    BYPASS_UNDEF_BRG_09,
    BYPASS_UNDEF_BRG_0A,
    BYPASS_UNDEF_BRG_0B,
    BYPASS_UNDEF_BRG_0C,
    BYPASS_PROVIDER_BRIDGE_GVRP_ADDRESS,
    BYPASS_8021AB,
    BYPASS_UNDEF_BRG_0F,
    BYPASS_BRG_MNGEMENT,
    BYPASS_UNDEFINED_11,
    BYPASS_UNDEFINED_12,
    BYPASS_UNDEFINED_13,
    BYPASS_UNDEFINED_14,
    BYPASS_UNDEFINED_15,
    BYPASS_UNDEFINED_16,
    BYPASS_UNDEFINED_17,
    BYPASS_UNDEFINED_18,
    BYPASS_UNDEFINED_19,
    BYPASS_UNDEFINED_1A,
    BYPASS_UNDEFINED_1B,
    BYPASS_UNDEFINED_1C,
    BYPASS_UNDEFINED_1D,
    BYPASS_UNDEFINED_1E,
    BYPASS_UNDEFINED_1F,
    BYPASS_GMRP,
    BYPASS_GVRP,
    BYPASS_UNDEF_GARP_22,
    BYPASS_UNDEF_GARP_23,
    BYPASS_UNDEF_GARP_24,
    BYPASS_UNDEF_GARP_25,
    BYPASS_UNDEF_GARP_26,
    BYPASS_UNDEF_GARP_27,
    BYPASS_UNDEF_GARP_28,
    BYPASS_UNDEF_GARP_29,
    BYPASS_UNDEF_GARP_2A,
    BYPASS_UNDEF_GARP_2B,
    BYPASS_UNDEF_GARP_2C,
    BYPASS_UNDEF_GARP_2D,
    BYPASS_UNDEF_GARP_2E,
    BYPASS_UNDEF_GARP_2F,
    BYPASS_IGMP,
    BYPASS_CDP,
    BYPASS_CSSTP,
    BYPASS_LLDP,
    BYPASS_END,
}l2sw_storm_bypass_t;

/* Function Name:
 *      l2sw_rate_stormControlMeterIdx_set
 * Description:
 *      Set the storm control meter index.
 * Input:
 *      port       - port id
 *      storm_type - storm group type
 *      index       - storm control meter index.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_PORT_ID - Invalid port id
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_rate_stormControlMeterIdx_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_storm_group_t stormType, l2sw_uint32 index);

/* Function Name:
 *      l2sw_rate_stormControlMeterIdx_get
 * Description:
 *      Get the storm control meter index.
 * Input:
 *      port       - port id
 *      storm_type - storm group type
 * Output:
 *      pIndex     - storm control meter index.
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_PORT_ID - Invalid port id
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_rate_stormControlMeterIdx_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_storm_group_t stormType, l2sw_uint32 *pIndex);

/* Function Name:
 *      l2sw_rate_stormControlPortEnable_set
 * Description:
 *      Set enable status of storm control on specified port.
 * Input:
 *      port       - port id
 *      stormType  - storm group type
 *      enable     - enable status of storm control
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_PORT_ID           - invalid port id
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_rate_stormControlPortEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_storm_group_t stormType, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_rate_stormControlPortEnable_get
 * Description:
 *      Set enable status of storm control on specified port.
 * Input:
 *      port       - port id
 *      stormType  - storm group type
 * Output:
 *      pEnable     - enable status of storm control
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_PORT_ID           - invalid port id
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_rate_stormControlPortEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_storm_group_t stormType, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_storm_bypass_set
 * Description:
 *      Set bypass storm filter control configuration.
 * Input:
 *      type    - Bypass storm filter control type.
 *      enable  - Bypass status.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_ENABLE       - Invalid IFG parameter
 * Note:
 *
 *      This API can set per-port bypass stomr filter control frame type including RMA and igmp.
 *      The bypass frame type is as following:
 *      - BYPASS_BRG_GROUP,
 *      - BYPASS_FD_PAUSE,
 *      - BYPASS_SP_MCAST,
 *      - BYPASS_1X_PAE,
 *      - BYPASS_UNDEF_BRG_04,
 *      - BYPASS_UNDEF_BRG_05,
 *      - BYPASS_UNDEF_BRG_06,
 *      - BYPASS_UNDEF_BRG_07,
 *      - BYPASS_PROVIDER_BRIDGE_GROUP_ADDRESS,
 *      - BYPASS_UNDEF_BRG_09,
 *      - BYPASS_UNDEF_BRG_0A,
 *      - BYPASS_UNDEF_BRG_0B,
 *      - BYPASS_UNDEF_BRG_0C,
 *      - BYPASS_PROVIDER_BRIDGE_GVRP_ADDRESS,
 *      - BYPASS_8021AB,
 *      - BYPASS_UNDEF_BRG_0F,
 *      - BYPASS_BRG_MNGEMENT,
 *      - BYPASS_UNDEFINED_11,
 *      - BYPASS_UNDEFINED_12,
 *      - BYPASS_UNDEFINED_13,
 *      - BYPASS_UNDEFINED_14,
 *      - BYPASS_UNDEFINED_15,
 *      - BYPASS_UNDEFINED_16,
 *      - BYPASS_UNDEFINED_17,
 *      - BYPASS_UNDEFINED_18,
 *      - BYPASS_UNDEFINED_19,
 *      - BYPASS_UNDEFINED_1A,
 *      - BYPASS_UNDEFINED_1B,
 *      - BYPASS_UNDEFINED_1C,
 *      - BYPASS_UNDEFINED_1D,
 *      - BYPASS_UNDEFINED_1E,
 *      - BYPASS_UNDEFINED_1F,
 *      - BYPASS_GMRP,
 *      - BYPASS_GVRP,
 *      - BYPASS_UNDEF_GARP_22,
 *      - BYPASS_UNDEF_GARP_23,
 *      - BYPASS_UNDEF_GARP_24,
 *      - BYPASS_UNDEF_GARP_25,
 *      - BYPASS_UNDEF_GARP_26,
 *      - BYPASS_UNDEF_GARP_27,
 *      - BYPASS_UNDEF_GARP_28,
 *      - BYPASS_UNDEF_GARP_29,
 *      - BYPASS_UNDEF_GARP_2A,
 *      - BYPASS_UNDEF_GARP_2B,
 *      - BYPASS_UNDEF_GARP_2C,
 *      - BYPASS_UNDEF_GARP_2D,
 *      - BYPASS_UNDEF_GARP_2E,
 *      - BYPASS_UNDEF_GARP_2F,
 *      - BYPASS_IGMP.
 */
extern l2sw_api_ret_t l2sw_storm_bypass_set(l2sw_uint8 units, l2sw_storm_bypass_t type, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_storm_bypass_get
 * Description:
 *      Get bypass storm filter control configuration.
 * Input:
 *      type - Bypass storm filter control type.
 * Output:
 *      pEnable - Bypass status.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can get per-port bypass stomr filter control frame type including RMA and igmp.
 *      The bypass frame type is as following:
 *      - BYPASS_BRG_GROUP,
 *      - BYPASS_FD_PAUSE,
 *      - BYPASS_SP_MCAST,
 *      - BYPASS_1X_PAE,
 *      - BYPASS_UNDEF_BRG_04,
 *      - BYPASS_UNDEF_BRG_05,
 *      - BYPASS_UNDEF_BRG_06,
 *      - BYPASS_UNDEF_BRG_07,
 *      - BYPASS_PROVIDER_BRIDGE_GROUP_ADDRESS,
 *      - BYPASS_UNDEF_BRG_09,
 *      - BYPASS_UNDEF_BRG_0A,
 *      - BYPASS_UNDEF_BRG_0B,
 *      - BYPASS_UNDEF_BRG_0C,
 *      - BYPASS_PROVIDER_BRIDGE_GVRP_ADDRESS,
 *      - BYPASS_8021AB,
 *      - BYPASS_UNDEF_BRG_0F,
 *      - BYPASS_BRG_MNGEMENT,
 *      - BYPASS_UNDEFINED_11,
 *      - BYPASS_UNDEFINED_12,
 *      - BYPASS_UNDEFINED_13,
 *      - BYPASS_UNDEFINED_14,
 *      - BYPASS_UNDEFINED_15,
 *      - BYPASS_UNDEFINED_16,
 *      - BYPASS_UNDEFINED_17,
 *      - BYPASS_UNDEFINED_18,
 *      - BYPASS_UNDEFINED_19,
 *      - BYPASS_UNDEFINED_1A,
 *      - BYPASS_UNDEFINED_1B,
 *      - BYPASS_UNDEFINED_1C,
 *      - BYPASS_UNDEFINED_1D,
 *      - BYPASS_UNDEFINED_1E,
 *      - BYPASS_UNDEFINED_1F,
 *      - BYPASS_GMRP,
 *      - BYPASS_GVRP,
 *      - BYPASS_UNDEF_GARP_22,
 *      - BYPASS_UNDEF_GARP_23,
 *      - BYPASS_UNDEF_GARP_24,
 *      - BYPASS_UNDEF_GARP_25,
 *      - BYPASS_UNDEF_GARP_26,
 *      - BYPASS_UNDEF_GARP_27,
 *      - BYPASS_UNDEF_GARP_28,
 *      - BYPASS_UNDEF_GARP_29,
 *      - BYPASS_UNDEF_GARP_2A,
 *      - BYPASS_UNDEF_GARP_2B,
 *      - BYPASS_UNDEF_GARP_2C,
 *      - BYPASS_UNDEF_GARP_2D,
 *      - BYPASS_UNDEF_GARP_2E,
 *      - BYPASS_UNDEF_GARP_2F,
 *      - BYPASS_IGMP.
 */
extern l2sw_api_ret_t l2sw_storm_bypass_get(l2sw_uint8 units, l2sw_storm_bypass_t type, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_rate_stormControlExtPortmask_set
 * Description:
 *      Set externsion storm control port mask
 * Input:
 *      pPortmask  - port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_rate_stormControlExtPortmask_set(l2sw_uint8 units, l2sw_portmask_t *pPortmask);

/* Function Name:
 *      l2sw_rate_stormControlExtPortmask_get
 * Description:
 *      Set externsion storm control port mask
 * Input:
 *      None
 * Output:
 *      pPortmask  - port mask
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_rate_stormControlExtPortmask_get(l2sw_uint8 units, l2sw_portmask_t *pPortmask);

/* Function Name:
 *      l2sw_rate_stormControlExtEnable_set
 * Description:
 *      Set externsion storm control state
 * Input:
 *      stormType   - storm group type
 *      enable      - externsion storm control state
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_rate_stormControlExtEnable_set(l2sw_uint8 units, l2sw_rate_storm_group_t stormType, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_rate_stormControlExtEnable_get
 * Description:
 *      Get externsion storm control state
 * Input:
 *      stormType   - storm group type
 * Output:
 *      pEnable     - externsion storm control state
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_rate_stormControlExtEnable_get(l2sw_uint8 units, l2sw_rate_storm_group_t stormType, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_rate_stormControlExtMeterIdx_set
 * Description:
 *      Set externsion storm control meter index
 * Input:
 *      stormType   - storm group type
 *      index       - externsion storm control state
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_rate_stormControlExtMeterIdx_set(l2sw_uint8 units, l2sw_rate_storm_group_t stormType, l2sw_uint32 index);

/* Function Name:
 *      l2sw_rate_stormControlExtMeterIdx_get
 * Description:
 *      Get externsion storm control meter index
 * Input:
 *      stormType   - storm group type
 * Output:
 *      pIndex      - externsion storm control state
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_rate_stormControlExtMeterIdx_get(l2sw_uint8 units, l2sw_rate_storm_group_t stormType, l2sw_uint32 *pIndex);

/* Function Name:
 *      l2sw_rate_stormControlEnable_set
 * Description:
 *      Set enable status of storm control function
 * Input:
 *      enable     - enable status of storm control
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_rate_stormControlEnable_set(l2sw_uint8 units, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_rate_stormControlEnable_get
 * Description:
 *      Get enable status of storm control function
 * Input:
 *      None
 * Output:
 *      pEnable     - enable status of storm control
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT          - The module is not initial
 *      L2SW_ERR_INPUT             - invalid input parameter
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_rate_stormControlEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnable);


#endif /* __L2SW_API_STORM_H__ */
