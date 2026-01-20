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
#include "rate.h"
#include "qos.h"
#include "sal_string.h"
#include "l2sw_asicdrv_meter.h"
#include "l2sw_asicdrv_inbwctrl.h"
#include "l2sw_asicdrv_scheduling.h"

static l2sw_api_ret_t _l2sw_rate_shareMeter_set(l2sw_uint8 units, l2sw_meter_id_t index, l2sw_meter_type_t type, l2sw_rate_t rate, l2sw_enable_t ifg_include)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (index > L2SW_MAX_METER_ID)
        return L2SW_ERR_FILTER_METER_ID;

    if (type >= METER_TYPE_END)
        return L2SW_ERR_INPUT;

    if (ifg_include >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if (0 == rate)
	{
		/*恢复默认值，不限�?*/
		if ((retVal = l2sw_setAsicShareMeter(units, index, rate, ifg_include)) != L2SW_ERR_OK)
		{
			return retVal;
		}
	}
	else
	{
	    switch (type)
	    {
	        case METER_TYPE_KBPS:
	            if (rate > L2SW_QOS_RATE_INPUT_MAX || rate < L2SW_QOS_RATE_INPUT_MIN)
	                return L2SW_ERR_RATE ;

            if ((retVal = l2sw_setAsicShareMeter(units, index, rate >> 3, ifg_include)) != L2SW_ERR_OK)
                return retVal;

            break;
            case METER_TYPE_PPS:
            if (rate > L2SW_QOS_PPS_INPUT_MAX || rate < L2SW_QOS_PPS_INPUT_MIN)
                return L2SW_ERR_RATE ;

            if ((retVal = l2sw_setAsicShareMeter(units, index, rate, ifg_include)) != L2SW_ERR_OK)
                return retVal;

            break;
	        default:
	            return L2SW_ERR_INPUT;
	    }
	}

    /* Set Type */
    if ((retVal = l2sw_setAsicShareMeterType(units, index, (l2sw_uint32)type)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_shareMeter_get(l2sw_uint8 units, l2sw_meter_id_t index, l2sw_meter_type_t *pType, l2sw_rate_t *pRate, l2sw_enable_t *pIfg_include)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 regData;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (index > L2SW_MAX_METER_ID)
        return L2SW_ERR_FILTER_METER_ID;

    if(NULL == pType)
        return L2SW_ERR_NULL_POINTER;

    if(NULL == pRate)
        return L2SW_ERR_NULL_POINTER;

    if(NULL == pIfg_include)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicShareMeter(units, index, &regData, pIfg_include)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicShareMeterType(units, index, (l2sw_uint32 *)pType)) != L2SW_ERR_OK)
        return retVal;

    if(*pType == METER_TYPE_KBPS)
        *pRate = regData<<3;
    else
        *pRate = regData;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_shareMeterBucket_set(l2sw_uint8 units, l2sw_meter_id_t index, l2sw_uint32 bucket_size)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (index > L2SW_MAX_METER_ID)
        return L2SW_ERR_FILTER_METER_ID;

    if(bucket_size > L2SW_METERBUCKETSIZEMAX)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicShareMeterBucketSize(units, index, bucket_size)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_shareMeterBucket_get(l2sw_uint8 units, l2sw_meter_id_t index, l2sw_uint32 *pBucket_size)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (index > L2SW_MAX_METER_ID)
        return L2SW_ERR_FILTER_METER_ID;

    if(NULL == pBucket_size)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicShareMeterBucketSize(units, index, pBucket_size)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_igrBandwidthCtrlRate_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_t rate, l2sw_enable_t ifg_include, l2sw_enable_t fc_enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(ifg_include >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if(fc_enable >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if (0 == rate)
	{
	    if ((retVal = l2sw_setAsicPortIngressBandwidth(units, l2sw_switch_port_L2P_get(units, port), rate, ifg_include,fc_enable)) != L2SW_ERR_OK)
	        return retVal;
	}
	else
	{
	    if(l2sw_switch_isHsgPort(units, port) == L2SW_ERR_OK)
	    {
	        if ((rate > L2SW_QOS_RATE_INPUT_MAX_HSG) || (rate < L2SW_QOS_RATE_INPUT_MIN))
	            return L2SW_ERR_QOS_EBW_RATE ;
	    }
	    else
	    {
	        if ((rate > L2SW_QOS_RATE_INPUT_MAX) || (rate < L2SW_QOS_RATE_INPUT_MIN))
	            return L2SW_ERR_QOS_EBW_RATE ;
	    }

        if ((retVal = l2sw_setAsicPortIngressBandwidth(units, l2sw_switch_port_L2P_get(units, port), rate>>3, ifg_include,fc_enable)) != L2SW_ERR_OK)
	        return retVal;
	}


    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_igrBandwidthCtrlRate_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_t *pRate, l2sw_enable_t *pIfg_include, l2sw_enable_t *pFc_enable)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 regData;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pIfg_include)
        return L2SW_ERR_NULL_POINTER;

    if(NULL == pFc_enable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortIngressBandwidth(units, l2sw_switch_port_L2P_get(units, port), &regData, pIfg_include, pFc_enable)) != L2SW_ERR_OK)
        return retVal;

    *pRate = regData<<3;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_egrBandwidthCtrlRate_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_t rate,  l2sw_enable_t ifg_include)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (0 == rate)
    {
        if ((retVal = l2sw_setAsicPortEgressRate(units, l2sw_switch_port_L2P_get(units, port), rate)) != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        if(l2sw_switch_isHsgPort(units, port) == L2SW_ERR_OK)
        {
            if ((rate > L2SW_QOS_RATE_INPUT_MAX_HSG) || (rate < L2SW_QOS_RATE_INPUT_MIN))
                return L2SW_ERR_QOS_EBW_RATE ;
        }
        else
        {
            if ((rate > L2SW_QOS_RATE_INPUT_MAX) || (rate < L2SW_QOS_RATE_INPUT_MIN))
                return L2SW_ERR_QOS_EBW_RATE ;
        }
    
        if (ifg_include >= L2SW_ENABLE_END)
            return L2SW_ERR_ENABLE;

        if ((retVal = l2sw_setAsicPortEgressRate(units, l2sw_switch_port_L2P_get(units, port), rate>>3)) != L2SW_ERR_OK)
            return retVal;
    }
    
    if ((retVal = l2sw_setAsicPortEgressRateIfg(units, ifg_include)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_egrBandwidthCtrlRate_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_t *pRate, l2sw_enable_t *pIfg_include)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 regData;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pRate)
        return L2SW_ERR_NULL_POINTER;

    if(NULL == pIfg_include)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortEgressRate(units, l2sw_switch_port_L2P_get(units, port), &regData)) != L2SW_ERR_OK)
        return retVal;

    *pRate = regData << 3;

    if ((retVal = l2sw_getAsicPortEgressRateIfg(units, (l2sw_uint32*)pIfg_include)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_egrQueueBwCtrlEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_qid_t queue, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    /*for whole port function, the queue value should be 0xFF*/
    if (queue != L2SW_WHOLE_SYSTEM)
        return L2SW_ERR_QUEUE_ID;

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicAprEnable(units, l2sw_switch_port_L2P_get(units, port),pEnable))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_egrQueueBwCtrlEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_qid_t queue, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    /*for whole port function, the queue value should be 0xFF*/
    if (queue != L2SW_WHOLE_SYSTEM)
        return L2SW_ERR_QUEUE_ID;

    if (enable>=L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicAprEnable(units, l2sw_switch_port_L2P_get(units, port), enable))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_egrQueueBwCtrlRate_get(l2sw_uint8 units, l2sw_port_t port, l2sw_qid_t queue, l2sw_meter_id_t *pIndex)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 offset_idx;
    l2sw_uint32 phy_port;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (queue >= L2SW_MAX_NUM_OF_QUEUE)
        return L2SW_ERR_QUEUE_ID;

    if(NULL == pIndex)
        return L2SW_ERR_NULL_POINTER;

    phy_port = l2sw_switch_port_L2P_get(units, port);
    if ((retVal=l2sw_getAsicAprMeter(units, phy_port, queue,&offset_idx))!=L2SW_ERR_OK)
        return retVal;

    *pIndex = offset_idx + (phy_port*8);

     return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_egrQueueBwCtrlRate_set(l2sw_uint8 units, l2sw_port_t port, l2sw_qid_t queue, l2sw_meter_id_t index)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 offset_idx;
    l2sw_uint32 phy_port;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (queue >= L2SW_MAX_NUM_OF_QUEUE)
        return L2SW_ERR_QUEUE_ID;

    if (index > L2SW_MAX_METER_ID)
        return L2SW_ERR_FILTER_METER_ID;

    phy_port = l2sw_switch_port_L2P_get(units, port);
    if (index < (phy_port*8) ||  index > (7 + (phy_port*8)))
        return L2SW_ERR_FILTER_METER_ID;

    offset_idx = index - (phy_port*8);

    if ((retVal=l2sw_setAsicAprMeter(units, phy_port,queue,offset_idx))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_PortIngressBandwidthBypass_set(l2sw_uint8 units, l2sw_uint32 enabled)
{
    l2sw_api_ret_t retVal;

    if ((retVal=l2sw_setAsicPortIngressBandwidthBypass(units, enabled))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_rate_PortIngressBandwidthBypass_get(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    l2sw_api_ret_t retVal;

    if ((retVal=l2sw_getAsicPortIngressBandwidthBypass(units, pEnabled))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

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
l2sw_api_ret_t l2sw_rate_shareMeter_set(l2sw_uint8 units, l2sw_meter_id_t index, l2sw_meter_type_t type, l2sw_rate_t rate, l2sw_enable_t ifg_include)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_shareMeter_set(units, index, type, rate, ifg_include);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_rate_shareMeter_get(l2sw_uint8 units, l2sw_meter_id_t index, l2sw_meter_type_t *pType, l2sw_rate_t *pRate, l2sw_enable_t *pIfg_include)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_shareMeter_get(units, index, pType, pRate, pIfg_include);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_rate_shareMeterBucket_set(l2sw_uint8 units, l2sw_meter_id_t index, l2sw_uint32 bucket_size)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_shareMeterBucket_set(units, index, bucket_size);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_rate_shareMeterBucket_get(l2sw_uint8 units, l2sw_meter_id_t index, l2sw_uint32 *pBucket_size)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_shareMeterBucket_get(units, index, pBucket_size);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_rate_igrBandwidthCtrlRate_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_t rate, l2sw_enable_t ifg_include, l2sw_enable_t fc_enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_igrBandwidthCtrlRate_set(units, port, rate, ifg_include, fc_enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_rate_igrBandwidthCtrlRate_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_t *pRate, l2sw_enable_t *pIfg_include, l2sw_enable_t *pFc_enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_igrBandwidthCtrlRate_get(units, port, pRate, pIfg_include, pFc_enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_rate_egrBandwidthCtrlRate_set(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_t rate,  l2sw_enable_t ifg_include)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_egrBandwidthCtrlRate_set(units, port, rate, ifg_include);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_rate_egrBandwidthCtrlRate_get(l2sw_uint8 units, l2sw_port_t port, l2sw_rate_t *pRate, l2sw_enable_t *pIfg_include)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_egrBandwidthCtrlRate_get(units, port, pRate, pIfg_include);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rate_egrQueueBwCtrlEnable_get
 * Description:
 *      Get enable status of egress bandwidth control on specified queue.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      queue   - queue id
 * Output:
 *      pEnable - Pointer to enable status of egress queue bandwidth control
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_PORT_ID          - invalid port id
 *      L2SW_ERR_QUEUE_ID         - invalid queue id
 *      L2SW_ERR_NULL_POINTER     - input parameter may be null pointer
 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_rate_egrQueueBwCtrlEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_qid_t queue, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_egrQueueBwCtrlEnable_get(units, port, queue, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_rate_egrQueueBwCtrlEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_qid_t queue, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_egrQueueBwCtrlEnable_set(units, port, queue, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_rate_egrQueueBwCtrlRate_get(l2sw_uint8 units, l2sw_port_t port, l2sw_qid_t queue, l2sw_meter_id_t *pIndex)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_egrQueueBwCtrlRate_get(units, port, queue, pIndex);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_rate_egrQueueBwCtrlRate_set(l2sw_uint8 units, l2sw_port_t port, l2sw_qid_t queue, l2sw_meter_id_t index)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_egrQueueBwCtrlRate_set(units, port, queue, index);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_rate_PortIngressBandwidthBypass_set(l2sw_uint8 units, l2sw_uint32 enabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_PortIngressBandwidthBypass_set(units, enabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_rate_PortIngressBandwidthBypass_set
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
l2sw_api_ret_t l2sw_rate_PortIngressBandwidthBypass_get(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_rate_PortIngressBandwidthBypass_get(units, pEnabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}

