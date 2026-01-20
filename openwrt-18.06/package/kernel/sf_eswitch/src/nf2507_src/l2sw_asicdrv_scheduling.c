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

#include "l2sw_asicdrv_scheduling.h"
/* Function Name:
 *      l2sw_setAsicLeakyBucketParameter
 * Description:
 *      Set Leaky Bucket Paramters
 * Input:
 *      tick    - Tick is used for time slot size unit
 *      token   - Token is used for adding budget in each time slot
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_TICK     - Invalid TICK
 *      L2SW_ERR_TOKEN    - Invalid TOKEN
 * Note:
 *      None
 */
ret_t l2sw_setAsicLeakyBucketParameter(l2sw_uint8 units, l2sw_uint32 tick, l2sw_uint32 token)
{
    ret_t retVal;

    if(tick > 0xFF)
        return L2SW_ERR_TICK;

    if(token > 0xFF)
        return L2SW_ERR_TOKEN;

    retVal = l2sw_setAsicRegBits(units, L2SW_LEAKY_BUCKET_TICK_REG, L2SW_LEAKY_BUCKET_TICK_MASK, tick);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_setAsicRegBits(units, L2SW_LEAKY_BUCKET_TOKEN_REG, L2SW_LEAKY_BUCKET_TOKEN_MASK, token);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicLeakyBucketParameter
 * Description:
 *      Get Leaky Bucket Paramters
 * Input:
 *      tick    - Tick is used for time slot size unit
 *      token   - Token is used for adding budget in each time slot
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicLeakyBucketParameter(l2sw_uint8 units, l2sw_uint32 *tick, l2sw_uint32 *token)
{
    ret_t retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_LEAKY_BUCKET_TICK_REG, L2SW_LEAKY_BUCKET_TICK_MASK, tick);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    retVal = l2sw_getAsicRegBits(units, L2SW_LEAKY_BUCKET_TOKEN_REG, L2SW_LEAKY_BUCKET_TOKEN_MASK, token);

    return retVal;
}
/* Function Name:
 *      l2sw_setAsicAprMeter
 * Description:
 *      Set per-port per-queue APR shared meter index
 * Input:
 *      port    - Physical port number (0~10)
 *      qid     - Queue id
 *      apridx  - dedicated shared meter index for APR (0~7)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number
 *      L2SW_ERR_QUEUE_ID         - Invalid queue id
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter
 * Note:
 *      None
 */
ret_t l2sw_setAsicAprMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 apridx)
{
    ret_t retVal;
    l2sw_uint32 regAddr;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(qid > L2SW_QIDMAX)
        return L2SW_ERR_QUEUE_ID;

    if(apridx > L2SW_PORT_QUEUE_METER_INDEX_MAX)
        return L2SW_ERR_FILTER_METER_ID;

    if(port < 8)
        retVal = l2sw_setAsicRegBits(units, L2SW_SCHEDULE_PORT_APR_METER_REG(port, qid), L2SW_SCHEDULE_PORT_APR_METER_MASK(qid), apridx);
    else {
        regAddr = L2SW_REG_SCHEDULE_PORT8_APR_METER_CTRL0 + ((port-8) << 1) + (qid / 5);
        retVal = l2sw_setAsicRegBits(units, regAddr, L2SW_SCHEDULE_PORT_APR_METER_MASK(qid), apridx);
    }

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicAprMeter
 * Description:
 *      Get per-port per-queue APR shared meter index
 * Input:
 *      port    - Physical port number (0~10)
 *      qid     - Queue id
 *      apridx  - dedicated shared meter index for APR (0~7)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 *      L2SW_ERR_QUEUE_ID - Invalid queue id
 * Note:
 *      None
 */
ret_t l2sw_getAsicAprMeter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 *apridx)
{
    ret_t retVal;
    l2sw_uint32 regAddr;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(qid > L2SW_QIDMAX)
        return L2SW_ERR_QUEUE_ID;

    if(port < 8)
        retVal = l2sw_getAsicRegBits(units, L2SW_SCHEDULE_PORT_APR_METER_REG(port, qid), L2SW_SCHEDULE_PORT_APR_METER_MASK(qid), apridx);
    else {
        regAddr = L2SW_REG_SCHEDULE_PORT8_APR_METER_CTRL0 + ((port-8) << 1) + (qid / 5);
        retVal = l2sw_getAsicRegBits(units, regAddr, L2SW_SCHEDULE_PORT_APR_METER_MASK(qid), apridx);
    }

    return retVal;
}
/* Function Name:
 *      l2sw_setAsicAprEnable
 * Description:
 *      Set per-port APR enable
 * Input:
 *      port        - Physical port number (0~7)
 *      aprEnable   - APR enable seting 1:enable 0:disable
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_setAsicAprEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 aprEnable)
{
    ret_t retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    retVal = l2sw_setAsicRegBit(units, L2SW_SCHEDULE_APR_CTRL_REG, L2SW_SCHEDULE_APR_CTRL_OFFSET(port), aprEnable);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicAprEnable
 * Description:
 *      Get per-port APR enable
 * Input:
 *      port        - Physical port number (0~7)
 *      aprEnable   - APR enable seting 1:enable 0:disable
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicAprEnable(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *aprEnable)
{
    ret_t retVal;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    retVal = l2sw_getAsicRegBit(units, L2SW_SCHEDULE_APR_CTRL_REG, L2SW_SCHEDULE_APR_CTRL_OFFSET(port), aprEnable);

    return retVal;
}
/* Function Name:
 *      l2sw_setAsicWFQWeight
 * Description:
 *      Set weight  of a queue
 * Input:
 *      port    - Physical port number (0~10)
 *      qid     - The queue ID wanted to set
 *      qWeight - The weight value wanted to set (valid:0~127)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number
 *      L2SW_ERR_QUEUE_ID         - Invalid queue id
 *      L2SW_ERR_QOS_QUEUE_WEIGHT - Invalid queue weight
 * Note:
 *      None
 */
ret_t l2sw_setAsicWFQWeight(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 qWeight)
{
    ret_t retVal;

    /* Invalid input parameter */
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(qid > L2SW_QIDMAX)
        return L2SW_ERR_QUEUE_ID;

    if(qWeight > L2SW_QWEIGHTMAX && qid > 0)
        return L2SW_ERR_QOS_QUEUE_WEIGHT;

    retVal = l2sw_setAsicReg(units, L2SW_SCHEDULE_PORT_QUEUE_WFQ_WEIGHT_REG(port, qid), qWeight);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicWFQWeight
 * Description:
 *      Get weight  of a queue
 * Input:
 *      port    - Physical port number (0~10)
 *      qid     - The queue ID wanted to set
 *      qWeight - The weight value wanted to set (valid:0~127)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number
 *      L2SW_ERR_QUEUE_ID         - Invalid queue id
 * Note:
 *      None
 */
ret_t l2sw_getAsicWFQWeight(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 *qWeight)
{
    ret_t retVal;


    /* Invalid input parameter */
    if(port  > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(qid > L2SW_QIDMAX)
        return L2SW_ERR_QUEUE_ID;


    retVal = l2sw_getAsicReg(units, L2SW_SCHEDULE_PORT_QUEUE_WFQ_WEIGHT_REG(port, qid), qWeight);

    return retVal;
}
/* Function Name:
 *      l2sw_setAsicWFQBurstSize
 * Description:
 *      Set WFQ leaky bucket burst size
 * Input:
 *      burstsize   - Leaky bucket burst size, unit byte
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicWFQBurstSize(l2sw_uint8 units, l2sw_uint32 burstsize)
{
    ret_t retVal;

    retVal = l2sw_setAsicReg(units, L2SW_SCHEDULE_WFQ_BURST_SIZE_REG, burstsize);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicWFQBurstSize
 * Description:
 *      Get WFQ leaky bucket burst size
 * Input:
 *      burstsize   - Leaky bucket burst size, unit byte
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicWFQBurstSize(l2sw_uint8 units, l2sw_uint32 *burstsize)
{
    ret_t retVal;

    retVal = l2sw_getAsicReg(units, L2SW_SCHEDULE_WFQ_BURST_SIZE_REG, burstsize);

    return retVal;
}
/* Function Name:
 *      l2sw_setAsicQueueType
 * Description:
 *      Set type of a queue
 * Input:
 *      port        - Physical port number (0~10)
 *      qid         - The queue ID wanted to set
 *      queueType   - The specified queue type. 0b0: Strict priority, 0b1: WFQ
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 *      L2SW_ERR_QUEUE_ID - Invalid queue id
 * Note:
 *      None
 */
ret_t l2sw_setAsicQueueType(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 queueType)
{
    ret_t retVal;

    /* Invalid input parameter */
    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(qid > L2SW_QIDMAX)
        return L2SW_ERR_QUEUE_ID;

    /* Set Related Registers */
    retVal = l2sw_setAsicRegBit(units, L2SW_SCHEDULE_QUEUE_TYPE_REG(port), L2SW_SCHEDULE_QUEUE_TYPE_OFFSET(port, qid),queueType);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicQueueType
 * Description:
 *      Get type of a queue
 * Input:
 *      port        - Physical port number (0~7)
 *      qid         - The queue ID wanted to set
 *      queueType   - The specified queue type. 0b0: Strict priority, 0b1: WFQ
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 *      L2SW_ERR_PORT_ID  - Invalid port number
 *      L2SW_ERR_QUEUE_ID - Invalid queue id
 * Note:
 *      None
 */
ret_t l2sw_getAsicQueueType(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 qid, l2sw_uint32 *queueType)
{
    ret_t retVal;

    /* Invalid input parameter */
    if(port  > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(qid > L2SW_QIDMAX)
        return L2SW_ERR_QUEUE_ID;

    retVal = l2sw_getAsicRegBit(units, L2SW_SCHEDULE_QUEUE_TYPE_REG(port), L2SW_SCHEDULE_QUEUE_TYPE_OFFSET(port, qid),queueType);

    return retVal;
}
/* Function Name:
 *      l2sw_setAsicPortEgressRate
 * Description:
 *      Set per-port egress rate
 * Input:
 *      port        - Physical port number (0~10)
 *      rate        - Egress rate
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_QOS_EBW_RATE - Invalid bandwidth/rate
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortEgressRate(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 rate)
{
    ret_t retVal;
    l2sw_uint32 regAddr, regData;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    if(rate > L2SW_QOS_GRANULARTY_MAX)
        return L2SW_ERR_QOS_EBW_RATE;

    regAddr = L2SW_PORT_EGRESSBW_LSB_REG(port);
    regData = L2SW_QOS_GRANULARTY_LSB_MASK & rate;

    retVal = l2sw_setAsicReg(units, regAddr, regData);

    if(retVal != L2SW_ERR_OK)
        return retVal;

    regAddr = L2SW_PORT_EGRESSBW_MSB_REG(port);
    regData = (L2SW_QOS_GRANULARTY_MSB_MASK & rate) >> L2SW_QOS_GRANULARTY_MSB_OFFSET;

    retVal = l2sw_setAsicRegBits(units, regAddr, L2SW_PORT6_EGRESSBW_CTRL1_MASK, regData);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicPortEgressRate
 * Description:
 *      Get per-port egress rate
 * Input:
 *      port        - Physical port number (0~10)
 *      rate        - Egress rate
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortEgressRate(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *rate)
{
    ret_t retVal;
    l2sw_uint32 regAddr, regData,regData2;

    if(port > L2SW_PORTIDMAX)
        return L2SW_ERR_PORT_ID;

    regAddr = L2SW_PORT_EGRESSBW_LSB_REG(port);

    retVal = l2sw_getAsicReg(units, regAddr, &regData);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    regAddr = L2SW_PORT_EGRESSBW_MSB_REG(port);
    retVal = l2sw_getAsicRegBits(units, regAddr, L2SW_PORT6_EGRESSBW_CTRL1_MASK, &regData2);
    if(retVal != L2SW_ERR_OK)
        return retVal;

    *rate = regData | (regData2 << L2SW_QOS_GRANULARTY_MSB_OFFSET);

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_setAsicPortEgressRateIfg
 * Description:
 *      Set per-port egress rate calculate include/exclude IFG
 * Input:
 *      ifg     - 1:include IFG 0:exclude IFG
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicPortEgressRateIfg(l2sw_uint8 units, l2sw_uint32 ifg)
{
    ret_t retVal;

    retVal = l2sw_setAsicRegBit(units, L2SW_REG_SCHEDULE_WFQ_CTRL, L2SW_SCHEDULE_WFQ_CTRL_OFFSET, ifg);

    return retVal;
}
/* Function Name:
 *      l2sw_getAsicPortEgressRateIfg
 * Description:
 *      Get per-port egress rate calculate include/exclude IFG
 * Input:
 *      ifg     - 1:include IFG 0:exclude IFG
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicPortEgressRateIfg(l2sw_uint8 units, l2sw_uint32 *ifg)
{
    ret_t retVal;

    retVal = l2sw_getAsicRegBit(units, L2SW_REG_SCHEDULE_WFQ_CTRL, L2SW_SCHEDULE_WFQ_CTRL_OFFSET, ifg);

    return retVal;
}
