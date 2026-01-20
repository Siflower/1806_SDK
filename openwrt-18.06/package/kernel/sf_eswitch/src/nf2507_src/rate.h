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

#ifndef __L2SW_API_RATE_H__
#define __L2SW_API_RATE_H__

/*
 * Include Files
 */
//#include "l2sw_types.h"

/*
 * Data Type Declaration
 */
#define L2SW_MAX_METER_ID            64//(l2sw_switch_maxMeterId_get(units))
#define L2SW_METER_NUM               (L2SW_MAX_METER_ID + 1)

typedef enum l2sw_meter_type_e{
    METER_TYPE_KBPS = 0,    /* Kbps */
    METER_TYPE_PPS,         /* Packet per second */
    METER_TYPE_END
}l2sw_meter_type_t;


/*
 * Function Declaration
 */

 /* Rate */
/* Function Name:
 *      l2sw_rate_shareMeter_set
 * Description:
 *      Set meter configuration
 * Input:
 *      index       - shared meter index
 *      type        - shared meter type
 *      rate        - rate of share meter
 *      ifg_include - include IFG or not, ENABLE:include DISABLE:exclude
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter
 *      L2SW_ERR_RATE             - Invalid rate
 *      L2SW_ERR_INPUT            - Invalid input parameters
 * Note:
 *      The API can set shared meter rate and ifg include for each meter.
 *      The rate unit is 1 kbps and the range is from 8k to 1048568k if type is METER_TYPE_KBPS and
 *      the granularity of rate is 8 kbps.
 *      The rate unit is packets per second and the range is 1 ~ 0x1FFF if type is METER_TYPE_PPS.
 *      The ifg_include parameter is used
 *      for rate calculation with/without inter-frame-gap and preamble.
 */
l2sw_api_ret_t l2sw_rate_shareMeter_set(l2sw_uint8 units, l2sw_meter_id_t index, l2sw_meter_type_t type, l2sw_rate_t rate, l2sw_enable_t ifg_include);

/* Function Name:
 *      l2sw_rate_shareMeter_get
 * Description:
 *      Get meter configuration
 * Input:
 *      index        - shared meter index
 * Output:
 *      pType        - Meter Type
 *      pRate        - pointer of rate of share meter
 *      pIfg_include - include IFG or not, ENABLE:include DISABLE:exclude
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter
 * Note:
 *
 */
l2sw_api_ret_t l2sw_rate_shareMeter_get(l2sw_uint8 units, l2sw_meter_id_t index, l2sw_meter_type_t *pType, l2sw_rate_t *pRate, l2sw_enable_t *pIfg_include);

/* Function Name:
 *      l2sw_rate_shareMeterBucket_set
 * Description:
 *      Set meter Bucket Size
 * Input:
 *      index        - shared meter index
 *      bucket_size  - Bucket Size
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_INPUT            - Error Input
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter
 * Note:
 *      The API can set shared meter bucket size.
 */
extern l2sw_api_ret_t l2sw_rate_shareMeterBucket_set(l2sw_uint8 units, l2sw_meter_id_t index, l2sw_uint32 bucket_size);

/* Function Name:
 *      l2sw_rate_shareMeterBucket_get
 * Description:
 *      Get meter Bucket Size
 * Input:
 *      index        - shared meter index
 * Output:
 *      pBucket_size - Bucket Size
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter
 * Note:
 *      The API can get shared meter bucket size.
 */
extern l2sw_api_ret_t l2sw_rate_shareMeterBucket_get(l2sw_uint8 units, l2sw_meter_id_t index, l2sw_uint32 *pBucket_size);

/* Function Name:
 *      l2sw_rate_igrBandwidthCtrlRate_set
 * Description:
 *      Set port ingress bandwidth control
 * Input:
 *      port        - Port id
 *      rate        - Rate of share meter
 *      ifg_include - include IFG or not, ENABLE:include DISABLE:exclude
 *      fc_enable   - enable flow control or not, ENABLE:use flow control DISABLE:drop
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_ENABLE       - Invalid IFG parameter.
 *      L2SW_ERR_INBW_RATE    - Invalid ingress rate parameter.
 * Note:
 *      The rate unit is 1 kbps and the range is from 8k to 1048568k. The granularity of rate is 8 kbps.
 *      The ifg_include parameter is used for rate calculation with/without inter-frame-gap and preamble.
 */
extern l2sw_api_ret_t l2sw_rate_igrBandwidthCtrlRate_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_t rate,  l2sw_enable_t ifg_include, l2sw_enable_t fc_enable);

/* Function Name:
 *      l2sw_rate_igrBandwidthCtrlRate_get
 * Description:
 *      Get port ingress bandwidth control
 * Input:
 *      port - Port id
 * Output:
 *      pRate           - Rate of share meter
 *      pIfg_include    - Rate's calculation including IFG, ENABLE:include DISABLE:exclude
 *      pFc_enable      - enable flow control or not, ENABLE:use flow control DISABLE:drop
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *     The rate unit is 1 kbps and the range is from 8k to 1048568k. The granularity of rate is 8 kbps.
 *     The ifg_include parameter is used for rate calculation with/without inter-frame-gap and preamble.
 */
extern l2sw_api_ret_t l2sw_rate_igrBandwidthCtrlRate_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_t *pRate, l2sw_enable_t *pIfg_include, l2sw_enable_t *pFc_enable);

/* Function Name:
 *      l2sw_rate_egrBandwidthCtrlRate_set
 * Description:
 *      Set port egress bandwidth control
 * Input:
 *      port        - Port id
 *      rate        - Rate of egress bandwidth
 *      ifg_include - include IFG or not, ENABLE:include DISABLE:exclude
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_QOS_EBW_RATE - Invalid egress bandwidth/rate
 * Note:
 *     The rate unit is 1 kbps and the range is from 8k to 1048568k. The granularity of rate is 8 kbps.
 *     The ifg_include parameter is used for rate calculation with/without inter-frame-gap and preamble.
 */
extern l2sw_api_ret_t l2sw_rate_egrBandwidthCtrlRate_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_t rate,  l2sw_enable_t ifg_includ);

/* Function Name:
 *      l2sw_rate_egrBandwidthCtrlRate_get
 * Description:
 *      Get port egress bandwidth control
 * Input:
 *      port - Port id
 * Output:
 *      pRate           - Rate of egress bandwidth
 *      pIfg_include    - Rate's calculation including IFG, ENABLE:include DISABLE:exclude
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *     The rate unit is 1 kbps and the range is from 8k to 1048568k. The granularity of rate is 8 kbps.
 *     The ifg_include parameter is used for rate calculation with/without inter-frame-gap and preamble.
 */
extern l2sw_api_ret_t l2sw_rate_egrBandwidthCtrlRate_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_t *pRate, l2sw_enable_t *pIfg_include);

/* Function Name:
 *      l2sw_rate_egrQueueBwCtrlEnable_set
 * Description:
 *      Set enable status of egress bandwidth control on specified queue.
 * Input:
 *      port   - port id
 *      queue  - queue id
 *      enable - enable status of egress queue bandwidth control
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_PORT_ID          - invalid port id
 *      L2SW_ERR_QUEUE_ID         - invalid queue id
 *      L2SW_ERR_INPUT            - invalid input parameter
 * Note:
 *      None
 */
extern l2sw_api_ret_t l2sw_rate_egrQueueBwCtrlEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_qid_t queue, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_rate_egrQueueBwCtrlEnable_get
 * Description:
 *      Get rate of egress bandwidth control on specified queue.
 * Input:
 *      port  - port id
 *      queue - queue id
 *      pIndex - shared meter index
 * Output:
 *      pRate - pointer to rate of egress queue bandwidth control
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_PORT_ID          - invalid port id
 *      L2SW_ERR_QUEUE_ID         - invalid queue id
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter id
 * Note:
 *    None.
 */
extern l2sw_api_ret_t l2sw_rate_egrQueueBwCtrlEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_qid_t queue, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_rate_egrQueueBwCtrlRate_set
 * Description:
 *      Set rate of egress bandwidth control on specified queue.
 * Input:
 *      port  - port id
 *      queue - queue id
 *      index - shared meter index
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_PORT_ID          - invalid port id
 *      L2SW_ERR_QUEUE_ID         - invalid queue id
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter id
 * Note:
 *    The actual rate control is set in shared meters.
 *    The unit of granularity is 8Kbps.
 */
extern l2sw_api_ret_t l2sw_rate_egrQueueBwCtrlRate_set(l2sw_uint8 units, l2sw_port_t port, l2sw_qid_t queue, l2sw_meter_id_t index);

/* Function Name:
 *      l2sw_rate_egrQueueBwCtrlRate_get
 * Description:
 *      Get rate of egress bandwidth control on specified queue.
 * Input:
 *      port  - port id
 *      queue - queue id
 *      pIndex - shared meter index
 * Output:
 *      pRate - pointer to rate of egress queue bandwidth control
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_PORT_ID          - invalid port id
 *      L2SW_ERR_QUEUE_ID         - invalid queue id
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter id
 * Note:
 *    The actual rate control is set in shared meters.
 *    The unit of granularity is 8Kbps.
 */
extern l2sw_api_ret_t l2sw_rate_egrQueueBwCtrlRate_get(l2sw_uint8 units, l2sw_port_t port, l2sw_qid_t queue, l2sw_meter_id_t *pIndex);

/* Function Name:
 *      l2sw_rate_PortIngressBandwidthBypass_set
 * Description:
 *      Set global enable / disable special packets to bypass the bandwidth limit of the incoming port.
 * Input:
 *      enabled - 1 :enabled or 0 :disabled
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 * Note:
 *     
 */
extern l2sw_api_ret_t l2sw_rate_PortIngressBandwidthBypass_set(l2sw_uint8 units, l2sw_uint32 enabled);

/* Function Name:
 *      l2sw_rate_PortIngressBandwidthBypass_get
 * Description:
 *      Get global enable / disable special packets to bypass the bandwidth limit of the incoming port.
 * Input:
 *      None
 * Output:
 *      pEnabled - enabled status
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 * Note:
 *     
 */
extern l2sw_api_ret_t l2sw_rate_PortIngressBandwidthBypass_get(l2sw_uint8 units, l2sw_uint32* pEnabled);

#endif /* __L2SW_API_RATE_H__ */

