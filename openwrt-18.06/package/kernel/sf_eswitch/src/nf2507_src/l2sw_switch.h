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

#ifndef __L2SW_SWITCH_H__
#define __L2SW_SWITCH_H__

#include "l2sw_types.h"
#include "port.h"

#if defined(L2SW_LINUX_KERNEL)
#include "linux/mutex.h"
#endif

#define ENABLED		1
#define DISABLED	0

#define L2SW_MAX_NUM_OF_UNIT   2

#define UNDEFINE_PHY_PORT   (0xFF)
#define L2SW_SWITCH_PORT_NUM (32)

#define MAXPKTLEN_CFG_ID_MAX (1)

#define L2SW_SWITCH_MAX_PKTLEN (0x3FFF)

#define AHB_READ_BIT    0x0
#define AHB_WRITE_BIT   0x1
#define AHB_START_BIT   0x2

#if defined(L2SW_LINUX_KERNEL)
extern struct mutex api_mutex[L2SW_MAX_NUM_OF_UNIT];
#define L2SW_API_LOCK(units)      mutex_lock(&api_mutex[units])
#define L2SW_API_UNLOCK(units)    mutex_unlock(&api_mutex[units])
#else
#define L2SW_API_LOCK(units)
#define L2SW_API_UNLOCK(units)
#endif
#define IQ_INDEX_MIN 0
#define IQ_INDEX_MAX 31
#define OQ_INDEX_MIN 0
#define OQ_INDEX_MAX 15
#define DROP_INDEX_MIN 0
#define DROP_INDEX_MAX 31
#define IQ_MODEL 0
#define OQ_MODEL 0x2000
#define DROP_MODEL 0x4000
#define CB_CFG8_r 0X3208
#define L2SW_PHY_PORT_NUM 5
typedef enum init_state_e
{
    INIT_NOT_COMPLETED = 0,
    INIT_COMPLETED,
    INIT_STATE_END
} init_state_t;

typedef enum switch_chip_e
{
    CHIP_SF2507 = 0,
    /*CHIP_NF8370B,
    CHIP_NF8364B,
    CHIP_NF8363SC_VB,*/
    CHIP_END
}switch_chip_t;

typedef enum port_type_e
{
    UTP_PORT = 0,
    EXT_PORT,
    UNKNOWN_PORT = 0xFF,
    PORT_TYPE_END
}port_type_t;

typedef struct l2sw_switch_halCtrl_s
{
    switch_chip_t   switch_type;
    l2sw_uint32      l2p_port[L2SW_SWITCH_PORT_NUM];
    l2sw_uint32      p2l_port[L2SW_SWITCH_PORT_NUM];
    port_type_t     log_port_type[L2SW_SWITCH_PORT_NUM];
    l2sw_uint32      ptp_port[L2SW_SWITCH_PORT_NUM];
    l2sw_uint32      valid_portmask;
    l2sw_uint32      valid_utp_portmask;
    l2sw_uint32      valid_ext_portmask;
    l2sw_uint32      valid_cpu_portmask;
    l2sw_uint32      min_phy_port;
    l2sw_uint32      max_phy_port;
    l2sw_uint32      phy_portmask;
    l2sw_uint32      combo_logical_port;
    l2sw_uint32      hsg_logical_port;
    l2sw_uint32      sg_logical_portmask;
    l2sw_uint32      max_meter_id;
    l2sw_uint32      max_lut_addr_num;
    l2sw_uint32      trunk_group_mask;

}l2sw_switch_halCtrl_t;

typedef enum l2sw_switch_maxPktLen_linkSpeed_e {
     MAXPKTLEN_LINK_SPEED_FE = 0,
     MAXPKTLEN_LINK_SPEED_GE,
     MAXPKTLEN_LINK_SPEED_END,
} l2sw_switch_maxPktLen_linkSpeed_t;


/* UTIL MACRO */
#define L2SW_CHK_INIT_STATE(units)                                \
    do                                                      \
    {                                                       \
        if(l2sw_switch_initialState_get(units) != INIT_COMPLETED) \
        {                                                   \
            return L2SW_ERR_NOT_INIT;                         \
        }                                                   \
    }while(0)

#define L2SW_CHK_PORT_VALID(units, __port__)                            \
    do                                                          \
    {                                                           \
        if(l2sw_switch_logicalPortCheck(units, __port__) != L2SW_ERR_OK)  \
        {                                                       \
            return L2SW_ERR_PORT_ID;                              \
        }                                                       \
    }while(0)

#define L2SW_CHK_PORT_IS_UTP(units, __port__)                           \
    do                                                          \
    {                                                           \
        if(l2sw_switch_isUtpPort(units, __port__) != L2SW_ERR_OK)         \
        {                                                       \
            return L2SW_ERR_PORT_ID;                              \
        }                                                       \
    }while(0)

#define L2SW_CHK_PORT_IS_EXT(units, __port__)                           \
    do                                                          \
    {                                                           \
        if(l2sw_switch_isExtPort(units, __port__) != L2SW_ERR_OK)         \
        {                                                       \
            return L2SW_ERR_PORT_ID;                              \
        }                                                       \
    }while(0)

#define L2SW_CHK_PORT_IS_COMBO(units, __port__)                         \
    do                                                          \
    {                                                           \
        if(l2sw_switch_isComboPort(units, __port__) != L2SW_ERR_OK)       \
        {                                                       \
            return L2SW_ERR_PORT_ID;                              \
        }                                                       \
    }while(0)

#define L2SW_CHK_PORT_IS_PTP(units, __port__)                           \
    do                                                          \
    {                                                           \
        if(l2sw_switch_isPtpPort(units, __port__) != L2SW_ERR_OK)         \
        {                                                       \
            return L2SW_ERR_PORT_ID;                              \
        }                                                       \
    }while(0)

#define L2SW_CHK_PORTMASK_VALID(units, __portmask__)                        \
    do                                                              \
    {                                                               \
        if(l2sw_switch_isPortMaskValid(units, __portmask__) != L2SW_ERR_OK)   \
        {                                                           \
            return L2SW_ERR_PORT_MASK;                                \
        }                                                           \
    }while(0)

#define L2SW_CHK_PORTMASK_VALID_ONLY_UTP(units, __portmask__)               \
    do                                                              \
    {                                                               \
        if(l2sw_switch_isPortMaskUtp(units, __portmask__) != L2SW_ERR_OK)     \
        {                                                           \
            return L2SW_ERR_PORT_MASK;                                \
        }                                                           \
    }while(0)

#define L2SW_CHK_PORTMASK_VALID_ONLY_EXT(units, __portmask__)               \
    do                                                              \
    {                                                               \
        if(l2sw_switch_isPortMaskExt(units, __portmask__) != L2SW_ERR_OK)     \
        {                                                           \
            return L2SW_ERR_PORT_MASK;                                \
        }                                                           \
    }while(0)

#define L2SW_CHK_TRUNK_GROUP_VALID(units, __grpId__)                        \
    do                                                              \
    {                                                               \
        if(l2sw_switch_isValidTrunkGrpId(units, __grpId__) != L2SW_ERR_OK)    \
        {                                                           \
            return L2SW_ERR_LA_TRUNK_ID;                              \
        }                                                           \
    }while(0)

#define L2SW_PORTMASK_IS_PORT_SET(__portmask__, __port__)    (((__portmask__).bits[0] & (0x00000001 << __port__)) ? 1 : 0)
#define L2SW_PORTMASK_IS_EMPTY(__portmask__)                 (((__portmask__).bits[0] == 0) ? 1 : 0)
#define L2SW_PORTMASK_CLEAR(__portmask__)                    ((__portmask__).bits[0] = 0)
#define L2SW_PORTMASK_PORT_SET(__portmask__, __port__)       ((__portmask__).bits[0] |= (0x00000001 << __port__))
#define L2SW_PORTMASK_PORT_CLEAR(__portmask__, __port__)     ((__portmask__).bits[0] &= ~(0x00000001 << __port__))
#define L2SW_PORTMASK_ALLPORT_SET(units, __portmask__)              (l2sw_switch_logPortMask_get(units, &__portmask__))
#define L2SW_PORTMASK_SCAN(__portmask__, __port__)           for(__port__ = 0; __port__ < L2SW_SWITCH_PORT_NUM; __port__++)  if(L2SW_PORTMASK_IS_PORT_SET(__portmask__, __port__))
#define L2SW_PORTMASK_COMPARE(__portmask_A__, __portmask_B__)    ((__portmask_A__).bits[0] - (__portmask_B__).bits[0])

#define L2SW_SCAN_ALL_PHY_PORTMASK(units, __port__)                 for(__port__ = 0; __port__ < L2SW_SWITCH_PORT_NUM; __port__++)  if( (l2sw_switch_phyPortMask_get(units) & (0x00000001 << __port__)))
#define L2SW_SCAN_ALL_LOG_PORT(units, __port__)                     for(__port__ = 0; __port__ < L2SW_SWITCH_PORT_NUM; __port__++)  if( l2sw_switch_logicalPortCheck(units, __port__) == L2SW_ERR_OK)
#define L2SW_SCAN_ALL_LOG_PORTMASK(units, __portmask__)             for((__portmask__).bits[0] = 0; (__portmask__).bits[0] < 0x7FFFF; (__portmask__).bits[0]++)  if( l2sw_switch_isPortMaskValid(units, &__portmask__) == L2SW_ERR_OK)

/* Port mask defination */
#define L2SW_PHY_PORTMASK_ALL(units)                                (l2sw_switch_phyPortMask_get(units))

/* Port defination*/
#define L2SW_MAX_LOGICAL_PORT_ID(units)                             (l2sw_switch_maxLogicalPort_get(units))

/* Function Name:
 *      l2sw_switch_probe
 * Description:
 *      Probe switch
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Switch probed
 *      L2SW_ERR_FAILED   - Switch Unprobed.
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_switch_probe(l2sw_uint8 units, switch_chip_t *pSwitchChip);

/* Function Name:
 *      l2sw_switch_initialState_set
 * Description:
 *      Set initial status
 * Input:
 *      state   - Initial state;
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Initialized
 *      L2SW_ERR_FAILED   - Uninitialized
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_switch_initialState_set(l2sw_uint8 units, init_state_t state);

/* Function Name:
 *      l2sw_switch_initialState_get
 * Description:
 *      Get initial status
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      INIT_COMPLETED     - Initialized
 *      INIT_NOT_COMPLETED - Uninitialized
 * Note:
 *
 */
extern init_state_t l2sw_switch_initialState_get(l2sw_uint8 units);

/* Function Name:
 *      l2sw_switch_logicalPortCheck
 * Description:
 *      Check logical port ID.
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Port ID is correct
 *      L2SW_ERR_FAILED   - Port ID is not correct
 *      L2SW_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_switch_logicalPortCheck(l2sw_uint8 units, l2sw_port_t logicalPort);

/* Function Name:
 *      l2sw_switch_isUtpPort
 * Description:
 *      Check is logical port a UTP port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Port ID is a UTP port
 *      L2SW_ERR_FAILED   - Port ID is not a UTP port
 *      L2SW_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_switch_isUtpPort(l2sw_uint8 units, l2sw_port_t logicalPort);

/* Function Name:
 *      l2sw_switch_isExtPort
 * Description:
 *      Check is logical port a Extension port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Port ID is a EXT port
 *      L2SW_ERR_FAILED   - Port ID is not a EXT port
 *      L2SW_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_switch_isExtPort(l2sw_uint8 units, l2sw_port_t logicalPort);

/* Function Name:
 *      l2sw_switch_isHsgPort
 * Description:
 *      Check is logical port a HSG port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Port ID is a HSG port
 *      L2SW_ERR_FAILED   - Port ID is not a HSG port
 *      L2SW_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_switch_isHsgPort(l2sw_uint8 units, l2sw_port_t logicalPort);

/* Function Name:
 *      l2sw_switch_isSgmiiPort
 * Description:
 *      Check is logical port a SGMII port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Port ID is a SGMII port
 *      L2SW_ERR_FAILED   - Port ID is not a SGMII port
 *      L2SW_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_switch_isSgmiiPort(l2sw_uint8 units, l2sw_port_t logicalPort);

/* Function Name:
 *      l2sw_switch_isCPUPort
 * Description:
 *      Check is logical port a CPU port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Port ID is a CPU port
 *      L2SW_ERR_FAILED   - Port ID is not a CPU port
 *      L2SW_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_switch_isCPUPort(l2sw_uint8 units, l2sw_port_t logicalPort);

/* Function Name:
 *      l2sw_switch_isComboPort
 * Description:
 *      Check is logical port a Combo port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Port ID is a combo port
 *      L2SW_ERR_FAILED   - Port ID is not a combo port
 *      L2SW_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_switch_isComboPort(l2sw_uint8 units, l2sw_port_t logicalPort);

/* Function Name:
 *      l2sw_switch_ComboPort_get
 * Description:
 *      Get Combo port ID
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      Port ID of combo port
 * Note:
 *
 */
extern l2sw_uint32 l2sw_switch_ComboPort_get(l2sw_uint8 units);

/* Function Name:
 *      l2sw_switch_isPtpPort
 * Description:
 *      Check is logical port a PTP port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Port ID is a PTP port
 *      L2SW_ERR_FAILED   - Port ID is not a PTP port
 *      L2SW_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_switch_isPtpPort(l2sw_uint8 units, l2sw_port_t logicalPort);

/* Function Name:
 *      l2sw_switch_port_L2P_get
 * Description:
 *      Get physical port ID
 * Input:
 *      logicalPort       - logical port ID
 * Output:
 *      None
 * Return:
 *      Physical port ID
 * Note:
 *
 */
extern l2sw_uint32 l2sw_switch_port_L2P_get(l2sw_uint8 units, l2sw_port_t logicalPort);

/* Function Name:
 *      l2sw_switch_port_P2L_get
 * Description:
 *      Get logical port ID
 * Input:
 *      physicalPort       - physical port ID
 * Output:
 *      None
 * Return:
 *      logical port ID
 * Note:
 *
 */
extern l2sw_port_t l2sw_switch_port_P2L_get(l2sw_uint8 units, l2sw_uint32 physicalPort);

/* Function Name:
 *      l2sw_switch_isPortMaskValid
 * Description:
 *      Check portmask is valid or not
 * Input:
 *      pPmask       - logical port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - port mask is valid
 *      L2SW_ERR_FAILED       - port mask is not valid
 *      L2SW_ERR_NOT_INIT     - Not Initialize
 *      L2SW_ERR_NULL_POINTER - Null pointer
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_switch_isPortMaskValid(l2sw_uint8 units, l2sw_portmask_t *pPmask);

/* Function Name:
 *      l2sw_switch_isPortMaskUtp
 * Description:
 *      Check all ports in portmask are only UTP port
 * Input:
 *      pPmask       - logical port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Only UTP port in port mask
 *      L2SW_ERR_FAILED       - Not only UTP port in port mask
 *      L2SW_ERR_NOT_INIT     - Not Initialize
 *      L2SW_ERR_NULL_POINTER - Null pointer
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_switch_isPortMaskUtp(l2sw_uint8 units, l2sw_portmask_t *pPmask);

/* Function Name:
 *      l2sw_switch_isPortMaskExt
 * Description:
 *      Check all ports in portmask are only EXT port
 * Input:
 *      pPmask       - logical port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Only EXT port in port mask
 *      L2SW_ERR_FAILED       - Not only EXT port in port mask
 *      L2SW_ERR_NOT_INIT     - Not Initialize
 *      L2SW_ERR_NULL_POINTER - Null pointer
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_switch_isPortMaskExt(l2sw_uint8 units, l2sw_portmask_t *pPmask);

/* Function Name:
 *      l2sw_switch_portmask_L2P_get
 * Description:
 *      Get physicl portmask from logical portmask
 * Input:
 *      pLogicalPmask       - logical port mask
 * Output:
 *      pPhysicalPortmask   - physical port mask
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_NOT_INIT     - Not Initialize
 *      L2SW_ERR_NULL_POINTER - Null pointer
 *      L2SW_ERR_PORT_MASK    - Error port mask
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_switch_portmask_L2P_get(l2sw_uint8 units, l2sw_portmask_t *pLogicalPmask, l2sw_uint32 *pPhysicalPortmask);

/* Function Name:
 *      l2sw_switch_portmask_P2L_get
 * Description:
 *      Get logical portmask from physical portmask
 * Input:
 *      physicalPortmask    - physical port mask
 * Output:
 *      pLogicalPmask       - logical port mask
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_NOT_INIT     - Not Initialize
 *      L2SW_ERR_NULL_POINTER - Null pointer
 *      L2SW_ERR_PORT_MASK    - Error port mask
 * Note:
 *
 */
extern l2sw_api_ret_t l2sw_switch_portmask_P2L_get(l2sw_uint8 units, l2sw_uint32 physicalPortmask, l2sw_portmask_t *pLogicalPmask);

/* Function Name:
 *      l2sw_switch_phyPortMask_get
 * Description:
 *      Get physical portmask
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      0x00                - Not Initialize
 *      Other value         - Physical port mask
 * Note:
 *
 */
l2sw_uint32 l2sw_switch_phyPortMask_get(l2sw_uint8 units);

/* Function Name:
 *      l2sw_switch_logPortMask_get
 * Description:
 *      Get Logical portmask
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_NOT_INIT     - Not Initialize
 *      L2SW_ERR_NULL_POINTER - Null pointer
 * Note:
 *
 */
l2sw_api_ret_t l2sw_switch_logPortMask_get(l2sw_uint8 units, l2sw_portmask_t *pPortmask);

/* Function Name:
 *      l2sw_switch_init
 * Description:
 *      Set chip to default configuration enviroment
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      The API can set chip registers to default configuration for different release chip model.
 */
extern l2sw_api_ret_t l2sw_switch_init(l2sw_uint8 units);

/* Function Name:
 *      l2sw_switch_portMaxPktLen_set
 * Description:
 *      Set Max packet length
 * Input:
 *      port    - Port ID
 *      speed   - Speed
 *      cfgId   - Configuration ID
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 */
extern l2sw_api_ret_t l2sw_switch_portMaxPktLen_set(l2sw_uint8 units, l2sw_port_t port, l2sw_switch_maxPktLen_linkSpeed_t speed, l2sw_uint32 cfgId);

/* Function Name:
 *      l2sw_switch_portMaxPktLen_get
 * Description:
 *      Get Max packet length
 * Input:
 *      port    - Port ID
 *      speed   - Speed
 * Output:
 *      pCfgId  - Configuration ID
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 */
extern l2sw_api_ret_t l2sw_switch_portMaxPktLen_get(l2sw_uint8 units, l2sw_port_t port, l2sw_switch_maxPktLen_linkSpeed_t speed, l2sw_uint32 *pCfgId);

/* Function Name:
 *      l2sw_switch_maxPktLenCfg_set
 * Description:
 *      Set Max packet length configuration
 * Input:
 *      cfgId   - Configuration ID
 *      pktLen  - Max packet length
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 */
extern l2sw_api_ret_t l2sw_switch_maxPktLenCfg_set(l2sw_uint8 units, l2sw_uint32 cfgId, l2sw_uint32 pktLen);

/* Function Name:
 *      l2sw_switch_maxPktLenCfg_get
 * Description:
 *      Get Max packet length configuration
 * Input:
 *      cfgId   - Configuration ID
 *      pPktLen - Max packet length
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 */
extern l2sw_api_ret_t l2sw_switch_maxPktLenCfg_get(l2sw_uint8 units, l2sw_uint32 cfgId, l2sw_uint32 *pPktLen);

/* Function Name:
 *      l2sw_switch_greenEthernet_set
 * Description:
 *      Set all Ports Green Ethernet state.
 * Input:
 *      enable - Green Ethernet state.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - OK
 *      L2SW_ERR_FAILED   - Failed
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_ENABLE   - Invalid enable input.
 * Note:
 *      This API can set all Ports Green Ethernet state.
 *      The configuration is as following:
 *      - DISABLE
 *      - ENABLE
 */
extern l2sw_api_ret_t l2sw_switch_greenEthernet_set(l2sw_uint8 units, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_switch_greenEthernet_get
 * Description:
 *      Get all Ports Green Ethernet state.
 * Input:
 *      None
 * Output:
 *      pEnable - Green Ethernet state.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 *      This API can get Green Ethernet state.
 */
extern l2sw_api_ret_t l2sw_switch_greenEthernet_get(l2sw_uint8 units, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_switch_maxLogicalPort_get
 * Description:
 *      Get Max logical port ID
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      Max logical port
 * Note:
 *      This API can get max logical port
 */
extern l2sw_port_t l2sw_switch_maxLogicalPort_get(l2sw_uint8 units);

/* Function Name:
 *      l2sw_switch_maxMeterId_get
 * Description:
 *      Get Max Meter ID
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      0x00                - Not Initialize
 *      Other value         - Max Meter ID
 * Note:
 *
 */
extern l2sw_uint32 l2sw_switch_maxMeterId_get(l2sw_uint8 units);

/* Function Name:
 *      l2sw_switch_maxLutAddrNumber_get
 * Description:
 *      Get Max LUT Address number
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      0x00                - Not Initialize
 *      Other value         - Max LUT Address number
 * Note:
 *
 */
extern l2sw_uint32 l2sw_switch_maxLutAddrNumber_get(l2sw_uint8 units);

/* Function Name:
 *      l2sw_switch_isValidTrunkGrpId
 * Description:
 *      Check if trunk group is valid or not
 * Input:
 *      grpId       - Group ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Trunk Group ID is valid
 *      L2SW_ERR_LA_TRUNK_ID  - Trunk Group ID is not valid
 * Note:
 *
 */
extern l2sw_uint32 l2sw_switch_isValidTrunkGrpId(l2sw_uint8 units, l2sw_uint32 grpId);

/* Function Name:
 *      l2sw_switch_ahb_read
 * Description:
 *      Read soc memery by apb to ahb
 * Input:
 *      addr       - soc addr
 * Output:
 *      data
 * Return:
 *      L2SW_ERR_OK
 * Note:
 *
 */
extern l2sw_uint32 l2sw_switch_ahb_read(l2sw_uint8 units, l2sw_uint32 addr, l2sw_uint32 *data);

/* Function Name:
 *      l2sw_switch_ahb_write
 * Description:
 *      Write soc memery by apb to ahb
 * Input:
 *      addr       - soc addr
 *      data       - Data to write
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 * Note:
 *
 */
extern l2sw_uint32 l2sw_switch_ahb_write(l2sw_uint8 units, l2sw_uint32 addr, l2sw_uint32 data);

/* Function Name:
 *      l2sw_switch_mdio_master_init
 * Description:
 *      Init switch mdio master
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 * Note:
 *
 */
extern l2sw_uint32 l2sw_switch_mdio_master_init(l2sw_uint8 units);

/* Function Name:
 *      l2sw_switch_reset_init
 * Description:
 *      Rest init switch
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 * Note:
 *
 */
extern l2sw_uint32 l2sw_switch_reset_init(l2sw_uint8 units, l2sw_mode_ext_t mode);


#endif
