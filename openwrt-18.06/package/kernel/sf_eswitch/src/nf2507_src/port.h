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

#ifndef __L2SW_API_PORT_H__
#define __L2SW_API_PORT_H__

/*
 * Data Type Declaration
 */

#define PHY_CONTROL_REG                             0
#define PHY_STATUS_REG                              1
#define PHY_AN_ADVERTISEMENT_REG                    4
#define PHY_AN_LINKPARTNER_REG                      5
#define PHY_1000_BASET_CONTROL_REG                  9
#define PHY_1000_BASET_STATUS_REG                   10
#define PHY_RESOLVED_REG                            26

#define L2SW_EFID_MAX                                0x7

#define L2SW_FIBER_FORCE_1000M                       3
#define L2SW_FIBER_FORCE_100M                        5
#define L2SW_FIBER_FORCE_100M1000M                   7

#define RTK_INDRECT_ACCESS_CRTL                     0x1f00
#define L2SW_INDRECT_ACCESS_STATUS                  0x1f01
#define L2SW_INDRECT_ACCESS_ADDRESS                 0x1f02
#define L2SW_INDRECT_ACCESS_WRITE_DATA              0x1f03
#define L2SW_INDRECT_ACCESS_READ_DATA               0x1f04
#define L2SW_INDRECT_ACCESS_DELAY                   0x1f80
#define L2SW_INDRECT_ACCESS_BURST                   0x1f81
#define L2SW_RW_MASK                                0x2
#define L2SW_CMD_MASK                               0x1
#define L2SW_PHY_BUSY_OFFSET                        2


#define PHY0_DEFAULT_VALUE                          0x1140
#define PHY0_DUPLEX_BIT                             0x100
#define PHY0_AN_BIT                                 0x1000
#define PHY0_LOOPBACK_BIT                           0x4000
#define PHY0_SPEED_10M_BIT                          0x2040
#define PHY0_SPEED_100M_BIT                         0x2000
#define PHY0_SPEED_1000M_BIT                        0x40
#define PHY0_POWER_DOWN_BIT                         0x800

#define PHY9_ENABLE_M_S_CONFIG_BIT                  0x1000
#define PHY9_MASTER_BIT                             0x800

#define PHY10_MASTER_BIT                            0x4000

#define MDIO_WRITE_OPECODE 0x1
#define MDIO_READ_OPECODE 0x2

#define MII_22 0x0 
#define MDIO_45 0x1

#define UNIT0_PORT6_PHY_ID 1
#define UNIT0_PORT7_PHY_ID 2

#define UNIT1_PORT6_PHY_ID 0
#define UNIT1_PORT7_PHY_ID 2

typedef enum l2sw_mode_ext_e
{
    MODE_EXT_DISABLE = 0,
    MODE_EXT_RGMII,
    MODE_EXT_MII_MAC,
    MODE_EXT_MII_PHY,
    MODE_EXT_TMII_MAC,
    MODE_EXT_TMII_PHY,
    MODE_EXT_GMII,
    MODE_EXT_RMII_MAC,
    MODE_EXT_RMII_PHY,
    MODE_EXT_SGMII,
    MODE_EXT_HSGMII,
    MODE_EXT_1000X_100FX,
    MODE_EXT_1000X,
    MODE_EXT_100FX,
    MODE_EXT_RGMII_2,
    MODE_EXT_MII_MAC_2,
    MODE_EXT_MII_PHY_2,
    MODE_EXT_TMII_MAC_2,
    MODE_EXT_TMII_PHY_2,
    MODE_EXT_RMII_MAC_2,
    MODE_EXT_RMII_PHY_2,
    MODE_EXT_END
} l2sw_mode_ext_t;

typedef enum l2sw_port_duplex_e
{
    PORT_HALF_DUPLEX = 0,
    PORT_FULL_DUPLEX,
    PORT_DUPLEX_END
} l2sw_port_duplex_t;

typedef enum l2sw_port_master_e
{
    PORT_SLAVE = 0,
    PORT_MASTER = 1,
    PORT_MASTER_CONFIG_DISABLE = 2,
} l2sw_port_master_t;

typedef enum l2sw_port_linkStatus_e
{
    PORT_LINKDOWN = 0,
    PORT_LINKUP,
    PORT_LINKSTATUS_END
} l2sw_port_linkStatus_t;

typedef struct  l2sw_port_mac_ability_s
{
    l2sw_uint32 forcemode;
    l2sw_uint32 speed;
    l2sw_uint32 duplex;
    l2sw_uint32 link;
    l2sw_uint32 nway;
    l2sw_uint32 txpause;
    l2sw_uint32 rxpause;
}l2sw_port_mac_ability_t;

typedef struct l2sw_port_phy_ability_s
{
    l2sw_uint32    AutoNegotiation;  /*PHY register 0.12 setting for auto-negotiation process*/
    l2sw_uint32    Half_10;          /*PHY register 4.5 setting for 10BASE-TX half duplex capable*/
    l2sw_uint32    Full_10;          /*PHY register 4.6 setting for 10BASE-TX full duplex capable*/
    l2sw_uint32    Half_100;         /*PHY register 4.7 setting for 100BASE-TX half duplex capable*/
    l2sw_uint32    Full_100;         /*PHY register 4.8 setting for 100BASE-TX full duplex capable*/
    l2sw_uint32    Full_1000;        /*PHY register 9.9 setting for 1000BASE-T full duplex capable*/
    l2sw_uint32    FC;               /*PHY register 4.10 setting for flow control capability*/
    l2sw_uint32    AsyFC;            /*PHY register 4.11 setting for  asymmetric flow control capability*/
} l2sw_port_phy_ability_t;

typedef l2sw_uint32  l2sw_port_phy_data_t;     /* phy page  */

typedef enum l2sw_port_phy_mdix_mode_e
{
    PHY_AUTO_CROSSOVER_MODE= 0,
    PHY_FORCE_MDI_MODE,
    PHY_FORCE_MDIX_MODE,
    PHY_FORCE_MODE_END
} l2sw_port_phy_mdix_mode_t;

typedef enum l2sw_port_phy_mdix_status_e
{
    PHY_STATUS_AUTO_MDI_MODE= 0,
    PHY_STATUS_AUTO_MDIX_MODE,
    PHY_STATUS_FORCE_MDI_MODE,
    PHY_STATUS_FORCE_MDIX_MODE,
    PHY_STATUS_FORCE_MODE_END
} l2sw_port_phy_mdix_status_t;

typedef l2sw_uint32  l2sw_port_phy_page_t;     /* phy page  */

typedef enum l2sw_port_phy_reg_e
{
    PHY_REG_CONTROL             = 0,
    PHY_REG_STATUS,
    PHY_REG_IDENTIFIER_1,
    PHY_REG_IDENTIFIER_2,
    PHY_REG_AN_ADVERTISEMENT,
    PHY_REG_AN_LINKPARTNER,
    PHY_REG_1000_BASET_CONTROL  = 9,
    PHY_REG_1000_BASET_STATUS,
    PHY_REG_PHY_16              = 16,
    PHY_REG_PHY_18              = 18,
    PHY_REG_PHY_24              = 24,
    PHY_REG_LINK_STATUS         = 26,
    PHY_REG_PHY_28              = 28,
    PHY_REG_PHY_29              = 29,
    PHY_REG_PAGE                = 31,
    PHY_REG_END                 = 32
} l2sw_port_phy_reg_t;

typedef enum l2sw_port_phy_test_mode_e
{
    PHY_TEST_MODE_NORMAL= 0,
    PHY_TEST_MODE_1,
    PHY_TEST_MODE_2,
    PHY_TEST_MODE_3,
    PHY_TEST_MODE_4,
    PHY_TEST_MODE_END
} l2sw_port_phy_test_mode_t;

typedef enum l2sw_port_speed_e
{
    PORT_SPEED_10M = 0,
    PORT_SPEED_100M,
    PORT_SPEED_1000M,
    PORT_SPEED_500M,
    PORT_SPEED_2500M,
    PORT_SPEED_END
} l2sw_port_speed_t;

typedef enum l2sw_port_media_e
{
    PORT_MEDIA_COPPER = 0,
    PORT_MEDIA_FIBER,
    PORT_MEDIA_END
}l2sw_port_media_t;

typedef enum l2sw_port_mtu_e
{
    PORT_MTU_1522 = 0,
    PORT_MTU_1536,
    PORT_MTU_1552,
    PORT_MTU_10K,
    PORT_MTU_END
}l2sw_port_mtu_t;

typedef struct l2sw_rtctResult_s
{
    l2sw_port_speed_t    linkType;
    union
    {
        struct fe_result_s
        {
            l2sw_uint32      isRxShort;
            l2sw_uint32      isTxShort;
            l2sw_uint32      isRxOpen;
            l2sw_uint32      isTxOpen;
            l2sw_uint32      isRxMismatch;
            l2sw_uint32      isTxMismatch;
            l2sw_uint32      isRxLinedriver;
            l2sw_uint32      isTxLinedriver;
            l2sw_uint32      rxLen;
            l2sw_uint32      txLen;
        } fe_result;

        struct ge_result_s
        {
            l2sw_uint32      channelAShort;
            l2sw_uint32      channelBShort;
            l2sw_uint32      channelCShort;
            l2sw_uint32      channelDShort;

            l2sw_uint32      channelAOpen;
            l2sw_uint32      channelBOpen;
            l2sw_uint32      channelCOpen;
            l2sw_uint32      channelDOpen;

            l2sw_uint32      channelAMismatch;
            l2sw_uint32      channelBMismatch;
            l2sw_uint32      channelCMismatch;
            l2sw_uint32      channelDMismatch;

            l2sw_uint32      channelALinedriver;
            l2sw_uint32      channelBLinedriver;
            l2sw_uint32      channelCLinedriver;
            l2sw_uint32      channelDLinedriver;

            l2sw_uint32      channelALen;
            l2sw_uint32      channelBLen;
            l2sw_uint32      channelCLen;
            l2sw_uint32      channelDLen;
        } ge_result;
    }result;
} l2sw_rtctResult_t;

/* Function Name:
 *      l2sw_port_phyAutoNegoAbility_set
 * Description:
 *      Set ethernet PHY auto-negotiation desired ability.
 * Input:
 *      port        - port id.
 *      pAbility    - Ability structure
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_PHY_REG_ID       - Invalid PHY address
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      If Full_1000 bit is set to 1, the AutoNegotiation will be automatic set to 1. While both AutoNegotiation and Full_1000 are set to 0, the PHY speed and duplex selection will
 *      be set as following 100F > 100H > 10F > 10H priority sequence.
 */
extern l2sw_api_ret_t l2sw_port_phyAutoNegoAbility_set(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_ability_t *pAbility);

/* Function Name:
 *      l2sw_port_phyAutoNegoAbility_get
 * Description:
 *      Get PHY ability through PHY registers.
 * Input:
 *      port - Port id.
 * Output:
 *      pAbility - Ability structure
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_PHY_REG_ID       - Invalid PHY address
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      Get the capablity of specified PHY.
 */
extern l2sw_api_ret_t l2sw_port_phyAutoNegoAbility_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_ability_t *pAbility);

/* Function Name:
 *      l2sw_port_phyForceModeAbility_set
 * Description:
 *      Set the port speed/duplex mode/pause/asy_pause in the PHY force mode.
 * Input:
 *      port        - port id.
 *      pAbility    - Ability structure
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_PHY_REG_ID       - Invalid PHY address
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      While both AutoNegotiation and Full_1000 are set to 0, the PHY speed and duplex selection will
 *      be set as following 100F > 100H > 10F > 10H priority sequence.
 */
extern l2sw_api_ret_t l2sw_port_phyForceModeAbility_set(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_ability_t *pAbility);

/* Function Name:
 *      l2sw_port_phyForceModeAbility_get
 * Description:
 *      Get PHY ability through PHY registers.
 * Input:
 *      port - Port id.
 * Output:
 *      pAbility - Ability structure
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_PHY_REG_ID       - Invalid PHY address
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      Get the capablity of specified PHY.
 */
extern l2sw_api_ret_t l2sw_port_phyForceModeAbility_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_ability_t *pAbility);

/* Function Name:
 *      l2sw_port_phyStatus_get
 * Description:
 *      Get ethernet PHY linking status
 * Input:
 *      port - Port id.
 * Output:
 *      linkStatus  - PHY link status
 *      speed       - PHY link speed
 *      duplex      - PHY duplex mode
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_PHY_REG_ID       - Invalid PHY address
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      API will return auto negotiation status of phy.
 */
extern l2sw_api_ret_t l2sw_port_phyStatus_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_linkStatus_t *pLinkStatus, l2sw_port_speed_t *pSpeed, l2sw_port_duplex_t *pDuplex);

/* Function Name:
 *      l2sw_port_macForceLink_set
 * Description:
 *      Set port force linking configuration.
 * Input:
 *      port            - port id.
 *      pPortability    - port ability configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API can set Port/MAC force mode properties.
 */
extern l2sw_api_ret_t l2sw_port_macForceLink_set(l2sw_uint8 units, l2sw_port_t port, l2sw_port_mac_ability_t *pPortability);

/* Function Name:
 *      l2sw_port_macForceLink_get
 * Description:
 *      Get port force linking configuration.
 * Input:
 *      port - Port id.
 * Output:
 *      pPortability - port ability configuration
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can get Port/MAC force mode properties.
 */
extern l2sw_api_ret_t l2sw_port_macForceLink_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_mac_ability_t *pPortability);

/* Function Name:
 *      l2sw_port_macForceLinkExt_set
 * Description:
 *      Set external interface force linking configuration.
 * Input:
 *      port            - external port ID
 *      mode            - external interface mode
 *      pPortability    - port ability configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can set external interface force mode properties.
 *      The external interface can be set to:
 *      - MODE_EXT_DISABLE,
 *      - MODE_EXT_RGMII,
 *      - MODE_EXT_MII_MAC,
 *      - MODE_EXT_MII_PHY,
 *      - MODE_EXT_TMII_MAC,
 *      - MODE_EXT_TMII_PHY,
 *      - MODE_EXT_GMII,
 *      - MODE_EXT_RMII_MAC,
 *      - MODE_EXT_RMII_PHY,
 *      - MODE_EXT_SGMII,
 *      - MODE_EXT_HSGMII,
 *      - MODE_EXT_1000X_100FX,
 *      - MODE_EXT_1000X,
 *      - MODE_EXT_100FX,
 */
extern l2sw_api_ret_t l2sw_port_macForceLinkExt_set(l2sw_uint8 units, l2sw_port_t port, l2sw_mode_ext_t mode, l2sw_port_mac_ability_t *pPortability);

/* Function Name:
 *      l2sw_port_macForceLinkExt_get
 * Description:
 *      Set external interface force linking configuration.
 * Input:
 *      port            - external port ID
 * Output:
 *      pMode           - external interface mode
 *      pPortability    - port ability configuration
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can get external interface force mode properties.
 */
extern l2sw_api_ret_t l2sw_port_macForceLinkExt_get(l2sw_uint8 units, l2sw_port_t port, l2sw_mode_ext_t *pMode, l2sw_port_mac_ability_t *pPortability);

/* Function Name:
 *      l2sw_port_macStatus_get
 * Description:
 *      Get port link status.
 * Input:
 *      port - Port id.
 * Output:
 *      pPortstatus - port ability configuration
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API can get Port/PHY properties.
 */
extern l2sw_api_ret_t l2sw_port_macStatus_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_mac_ability_t *pPortstatus);

/* Function Name:
 *      l2sw_port_macLocalLoopbackEnable_set
 * Description:
 *      Set Port Local Loopback. (Redirect TX to RX.)
 * Input:
 *      port    - Port id.
 *      enable  - Loopback state, 0:disable, 1:enable
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API can enable/disable Local loopback in MAC.
 *      For UTP port, This API will also enable the digital
 *      loopback bit in PHY register for sync of speed between
 *      PHY and MAC. For EXT port, users need to force the
 *      link state by themself.
 */
extern l2sw_api_ret_t l2sw_port_macLocalLoopbackEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_port_macLocalLoopbackEnable_get
 * Description:
 *      Get Port Local Loopback. (Redirect TX to RX.)
 * Input:
 *      port    - Port id.
 * Output:
 *      pEnable  - Loopback state, 0:disable, 1:enable
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      None.
 */
extern l2sw_api_ret_t l2sw_port_macLocalLoopbackEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_port_phyReg_set
 * Description:
 *      Set PHY register data of the specific port.
 * Input:
 *      port    - port id.
 *      reg     - Register id
 *      regData - Register data
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_PHY_REG_ID       - Invalid PHY address
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      This API can set PHY register data of the specific port.
 */
extern l2sw_api_ret_t l2sw_port_phyReg_set(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_reg_t reg, l2sw_port_phy_data_t value);

/* Function Name:
 *      l2sw_port_phyReg_get
 * Description:
 *      Get PHY register data of the specific port.
 * Input:
 *      port    - Port id.
 *      reg     - Register id
 * Output:
 *      pData   - Register data
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_PHY_REG_ID       - Invalid PHY address
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      This API can get PHY register data of the specific port.
 */
extern l2sw_api_ret_t l2sw_port_phyReg_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_reg_t reg, l2sw_port_phy_data_t *pData);

/* Function Name:
 *      l2sw_port_backpressureEnable_set
 * Description:
 *      Set the half duplex backpressure enable status of the specific port.
 * Input:
 *      port    - port id.
 *      enable  - Back pressure status.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_ENABLE       - Invalid enable input.
 * Note:
 *      This API can set the half duplex backpressure enable status of the specific port.
 *      The half duplex backpressure enable status of the port is as following:
 *      - DISABLE
 *      - ENABLE
 */
extern l2sw_api_ret_t l2sw_port_backpressureEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_port_backpressureEnable_get
 * Description:
 *      Get the half duplex backpressure enable status of the specific port.
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - Back pressure status.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API can get the half duplex backpressure enable status of the specific port.
 *      The half duplex backpressure enable status of the port is as following:
 *      - DISABLE
 *      - ENABLE
 */
extern l2sw_api_ret_t l2sw_port_backpressureEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_port_adminEnable_set
 * Description:
 *      Set port admin configuration of the specific port.
 * Input:
 *      port    - port id.
 *      enable  - Back pressure status.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_ENABLE       - Invalid enable input.
 * Note:
 *      This API can set port admin configuration of the specific port.
 *      The port admin configuration of the port is as following:
 *      - DISABLE
 *      - ENABLE
 */
extern l2sw_api_ret_t l2sw_port_adminEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_port_adminEnable_get
 * Description:
 *      Get port admin configurationof the specific port.
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - Back pressure status.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API can get port admin configuration of the specific port.
 *      The port admin configuration of the port is as following:
 *      - DISABLE
 *      - ENABLE
 */
extern l2sw_api_ret_t l2sw_port_adminEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_port_isolation_set
 * Description:
 *      Set permitted port isolation portmask
 * Input:
 *      port         - port id.
 *      pPortmask    - Permit port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_PORT_MASK    - Invalid portmask.
 * Note:
 *      This API set the port mask that a port can trasmit packet to of each port
 *      A port can only transmit packet to ports included in permitted portmask
 */
extern l2sw_api_ret_t l2sw_port_isolation_set(l2sw_uint8 units, l2sw_port_t port, l2sw_portmask_t *pPortmask);

/* Function Name:
 *      l2sw_port_isolation_get
 * Description:
 *      Get permitted port isolation portmask
 * Input:
 *      port - Port id.
 * Output:
 *      pPortmask - Permit port mask
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API get the port mask that a port can trasmit packet to of each port
 *      A port can only transmit packet to ports included in permitted portmask
 */
extern l2sw_api_ret_t l2sw_port_isolation_get(l2sw_uint8 units, l2sw_port_t port, l2sw_portmask_t *pPortmask);

/* Function Name:
 *      l2sw_port_rgmiiDelayExt_set
 * Description:
 *      Set RGMII interface delay value for TX and RX.
 * Input:
 *      port    - port id
 *      txDelay - TX delay value, 1 for delay 2ns and 0 for no-delay
 *      rxDelay - RX delay value, 0~7 for delay setup.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can set external interface 2 RGMII delay.
 *      In TX delay, there are 2 selection: no-delay and 2ns delay.
 *      In RX dekay, there are 8 steps for delay tunning. 0 for no-delay, and 7 for maximum delay.
 *      Note. This API should be called before l2sw_port_macForceLinkExt_set(units).
 */
extern l2sw_api_ret_t l2sw_port_rgmiiDelayExt_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t rx_reverse, l2sw_enable_t tx_reverse, l2sw_data_t txDelay, l2sw_data_t rxDelay);

/* Function Name:
 *      l2sw_port_rgmiiDelayExt_get
 * Description:
 *      Get RGMII interface delay value for TX and RX.
 * Input:
 *      None
 * Output:
 *      port     - port id
 *      pTxDelay - TX delay value
 *      pRxDelay - RX delay value
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can set external interface 2 RGMII delay.
 *      In TX delay, there are 2 selection: no-delay and 2ns delay.
 *      In RX dekay, there are 8 steps for delay tunning. 0 for n0-delay, and 7 for maximum delay.
 */
extern l2sw_api_ret_t l2sw_port_rgmiiDelayExt_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *rx_reverse, l2sw_enable_t *tx_reverse, l2sw_data_t *pTxDelay, l2sw_data_t *pRxDelay);

/* Function Name:
 *      l2sw_port_phyEnableAll_set
 * Description:
 *      Set all PHY enable status.
 * Input:
 *      enable - PHY Enable State.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid enable input.
 * Note:
 *      This API can set all PHY status.
 *      The configuration of all PHY is as following:
 *      - DISABLE
 *      - ENABLE
 */
extern l2sw_api_ret_t l2sw_port_phyEnableAll_set(l2sw_uint8 units, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_port_phyEnableAll_get
 * Description:
 *      Get all PHY enable status.
 * Input:
 *      None
 * Output:
 *      pEnable - PHY Enable State.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      This API can set all PHY status.
 *      The configuration of all PHY is as following:
 *      - DISABLE
 *      - ENABLE
 */
extern l2sw_api_ret_t l2sw_port_phyEnableAll_get(l2sw_uint8 units, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_port_efid_set
 * Description:
 *      Set port-based enhanced filtering database
 * Input:
 *      port - Port id.
 *      efid - Specified enhanced filtering database.
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
 *      The API can set port-based enhanced filtering database.
 */
extern l2sw_api_ret_t l2sw_port_efid_set(l2sw_uint8 units, l2sw_port_t port, l2sw_data_t efid);

/* Function Name:
 *      l2sw_port_efid_get
 * Description:
 *      Get port-based enhanced filtering database
 * Input:
 *      port - Port id.
 * Output:
 *      pEfid - Specified enhanced filtering database.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT - Invalid input parameters.
 *      L2SW_ERR_PORT_ID - Invalid port ID.
 * Note:
 *      The API can get port-based enhanced filtering database status.
 */
extern l2sw_api_ret_t l2sw_port_efid_get(l2sw_uint8 units, l2sw_port_t port, l2sw_data_t *pEfid);

/* Function Name:
 *      l2sw_port_mtu_set
 * Description:
 *      Set Max packet length
 * Input:
 *      mtu        - Max packet length (0:1522, 1:1536, 2:1552, 3:10k) 
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can set Max packet length
 */
extern l2sw_api_ret_t l2sw_port_mtu_set(l2sw_uint8 units, l2sw_port_mtu_t mtu);

/* Function Name:
 *      l2sw_port_mtu_get
 * Description:
 *      Get Max packet length
 * Input:
 *      None
 * Output:
 *      mtu        - Max packet length (0:1522, 1:1536, 2:1552, 3:10k) 
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can set Max packet length
 */
extern l2sw_api_ret_t l2sw_port_mtu_get(l2sw_uint8 units, l2sw_port_mtu_t *mtu);

/* Function Name:
 *      l2sw_port_ipg_set
 * Description:
 *      Set Interframe Gap length
 * Input:
 *      ipg        - Interframe Gap <4-32>
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can set Interframe Gap length
 */
extern l2sw_api_ret_t l2sw_port_ipg_set(l2sw_uint8 units, l2sw_uint32 ipg);

/* Function Name:
 *      l2sw_port_ipg_get
 * Description:
 *      Get Interframe Gap length
 * Input:
 *      None
 * Output:
 *      ipg        - Interframe Gap <4-32>
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can get Interframe Gap length
 */
extern l2sw_api_ret_t l2sw_port_ipg_get(l2sw_uint8 units, l2sw_uint32 *ipg, l2sw_uint32 *mac_ipg);

/* Function Name:
 *      l2sw_port_mode_set
 * Description:
 *      Set utp port mode
 * Input:
 *      port - port id
 *      mode - copper,fiber
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can set utp port mode
 */
extern l2sw_api_ret_t l2sw_port_mode_set(l2sw_uint8 units, l2sw_uint32 port, l2sw_port_media_t mode);

/* Function Name:
 *      l2sw_port_mode_get
 * Description:
 *      Get utp port mode
 * Input:
 *      port - port id
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can get utp port mode
 */
extern l2sw_api_ret_t l2sw_port_mode_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_port_media_t *mode);

/* Function Name:
 *      l2sw_gmacport_phy_set
 * Description:
 *      Set gmacport's phy value 
 * Input:
 *      port - port id
 *      reg - phy reg
 *      value
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can set gmacport's phy value
 */
extern l2sw_api_ret_t l2sw_port_gmac_phy_set(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint16 reg, l2sw_uint16 value);

/* Function Name:
 *      l2sw_gmacport_phy_get
 * Description:
 *      Get gmacport's phy value 
 * Input:
 *      port - port id
 *      reg - phy reg
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can get gmacport's phy value 
 */
extern l2sw_api_ret_t l2sw_port_gmac_phy_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint16 reg, l2sw_uint16 *value);

/* Function Name:
 *      l2sw_port_master_slave_set
 * Description:
 *      Set utpport's master slave mode
 * Input:
 *      port - port id
 *      m_s - master or slave
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can set utpport's phy slave or master 
 */
extern l2sw_api_ret_t l2sw_port_master_slave_set(l2sw_uint8 units, l2sw_uint32 port, l2sw_port_master_t m_s);

/* Function Name:
 *      l2sw_port_master_slave_get
 * Description:
 *      Get utpport's master slave mode
 * Input:
 *      port - port id
 * Output:
 *   m_s - master or slave
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can get utpport's phy slave or master 
 */
extern l2sw_api_ret_t l2sw_port_master_slave_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_enable_t *enable, l2sw_port_master_t *m_s);

/* Function Name:
 *      l2sw_port_powerdown
 * Description:
 *      Set power down port
 * Input:
 *      pbmp - port bitmap
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can set utpport's power down
 */
extern l2sw_api_ret_t l2sw_port_power_down_set(l2sw_uint8 units, l2sw_uint8 pbmp);

#endif /* __L2SW_API_PORT_H__ */


