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

#ifndef __L2SW_RLDP_H__
#define __L2SW_RLDP_H__


/*
 * Include Files
 */


/*
 * Symbol Definition
 */
typedef enum l2sw_rldp_trigger_e
{
    L2SW_RLDP_TRIGGER_SAMOVING = 0,
    L2SW_RLDP_TRIGGER_PERIOD,
    L2SW_RLDP_TRIGGER_END
} l2sw_rldp_trigger_t;

typedef enum l2sw_rldp_cmpType_e
{
    L2SW_RLDP_CMPTYPE_MAGIC = 0,     /* Compare the RLDP with magic only */
    L2SW_RLDP_CMPTYPE_MAGIC_ID,      /* Compare the RLDP with both magic + ID */
    L2SW_RLDP_CMPTYPE_END
} l2sw_rldp_cmpType_t;

typedef enum l2sw_rldp_loopStatus_e
{
    L2SW_RLDP_LOOPSTS_NONE = 0,
    L2SW_RLDP_LOOPSTS_LOOPING,
    L2SW_RLDP_LOOPSTS_END
} l2sw_rldp_loopStatus_t;

typedef enum l2sw_rlpp_trapType_e
{
    L2SW_RLPP_TRAPTYPE_NONE = 0,
    L2SW_RLPP_TRAPTYPE_CPU,
    L2SW_RLPP_TRAPTYPE_END
} l2sw_rlpp_trapType_t;

typedef struct l2sw_rldp_config_s
{
    l2sw_enable_t        rldp_enable;
    l2sw_rldp_trigger_t trigger_mode;
    l2sw_mac_t           magic;
    l2sw_rldp_cmpType_t  compare_type;
    l2sw_uint32              interval_check; /* Checking interval for check state */
    l2sw_uint32              num_check;      /* Checking number for check state */
    l2sw_uint32              interval_loop;  /* Checking interval for loop state */
    l2sw_uint32              num_loop;       /* Checking number for loop state */
} l2sw_rldp_config_t;

typedef struct l2sw_rldp_portConfig_s
{
    l2sw_enable_t        tx_enable;
} l2sw_rldp_portConfig_t;

typedef struct l2sw_rldp_status_s
{
    l2sw_mac_t           id;
} l2sw_rldp_status_t;

typedef struct l2sw_rldp_portStatus_s
{
    l2sw_rldp_loopStatus_t   loop_status;
    l2sw_rldp_loopStatus_t   loop_enter;
    l2sw_rldp_loopStatus_t   loop_leave;
} l2sw_rldp_portStatus_t;

/*
 * Data Declaration
 */


/*
 * Macro Declaration
 */

#define L2SW_RLDP_INTERVAL_MAX  0xffff
#define L2SW_RLDP_NUM_MAX       0xff


/*
 * Function Declaration
 */

/* Module Name : RLDP */


/* Function Name:
 *      l2sw_rldp_config_set
 * Description:
 *      Set RLDP module configuration
 * Input:
 *      pConfig - configuration structure of RLDP
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_rldp_config_set(l2sw_uint8 units, l2sw_rldp_config_t *pConfig);


/* Function Name:
 *      l2sw_rldp_config_get
 * Description:
 *      Get RLDP module configuration
 * Input:
 *      None
 * Output:
 *      pConfig - configuration structure of RLDP
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_rldp_config_get(l2sw_uint8 units, l2sw_rldp_config_t *pConfig);


/* Function Name:
 *      l2sw_rldp_portConfig_set
 * Description:
 *      Set per port RLDP module configuration
 * Input:
 *      port   - port number to be configured
 *      pPortConfig - per port configuration structure of RLDP
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_rldp_portConfig_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rldp_portConfig_t *pPortConfig);


/* Function Name:
 *      l2sw_rldp_portConfig_get
 * Description:
 *      Get per port RLDP module configuration
 * Input:
 *      port    - port number to be get
 * Output:
 *      pPortConfig - per port configuration structure of RLDP
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_rldp_portConfig_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rldp_portConfig_t *pPortConfig);


/* Function Name:
 *      l2sw_rldp_status_get
 * Description:
 *      Get RLDP module status
 * Input:
 *      None
 * Output:
 *      pStatus - status structure of RLDP
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_rldp_status_get(l2sw_uint8 units, l2sw_rldp_status_t *pStatus);


/* Function Name:
 *      l2sw_rldp_portStatus_get
 * Description:
 *      Get RLDP module status
 * Input:
 *      port    - port number to be get
 * Output:
 *      pPortStatus - per port status structure of RLDP
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_rldp_portStatus_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rldp_portStatus_t *pPortStatus);


/* Function Name:
 *      l2sw_rldp_portStatus_set
 * Description:
 *      Clear RLDP module status
 * Input:
 *      port    - port number to be clear
 *      pPortStatus - per port status structure of RLDP
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      Clear operation effect loop_enter and loop_leave only, other field in
 *      the structure are don't care
 */
extern l2sw_api_ret_t l2sw_rldp_portStatus_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rldp_portStatus_t *pPortStatus);


/* Function Name:
 *      l2sw_rldp_portLoopPair_get
 * Description:
 *      Get RLDP port loop pairs
 * Input:
 *      port    - port number to be get
 * Output:
 *      pPortmask - per port related loop ports
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_rldp_portLoopPair_get(l2sw_uint8 units, l2sw_port_t port, l2sw_portmask_t *pPortmask);


/* Function Name:
 *      l2sw_rldp_enable_set
 * Description:
 *      Set RLDP enable state
 * Input:
 *      enable - rldp enable state 
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_rldp_enable_set(l2sw_uint8 units,l2sw_enable_t enable);


/* Function Name:
 *      l2sw_rldp_enable_get
 * Description:
 *      Get RLDP enable state
 * Input:
 *      None
 * Output:
 *      pEnable - rldp enable state 
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_rldp_enable_get(l2sw_uint8 units,l2sw_enable_t *pEnable);


/* Function Name:
 *      l2sw_rldp_trap_set
 * Description:
 *      Set RLDP trap enable state
 * Input:
 *      enable - rldp trap enable state 
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_rldp_trap_set(l2sw_uint8 units,l2sw_enable_t enable);

/* Function Name:
 *      l2sw_rldp_enable_get
 * Description:
 *      Get RLDP trap enable state
 * Input:
 *      None
 * Output:
 *      pEnable - rldp trap enable state 
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT
 *      L2SW_ERR_NULL_POINTER
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_rldp_trap_get(l2sw_uint8 units,l2sw_enable_t *pEnable);


#endif /* __L2SW_RLDP_H__ */

