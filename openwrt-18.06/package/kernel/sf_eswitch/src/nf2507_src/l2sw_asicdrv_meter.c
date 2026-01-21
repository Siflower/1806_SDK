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

#include "l2sw_asicdrv_meter.h"
/* Function Name:
 *      l2sw_setAsicShareMeter
 * Description:
 *      Set meter configuration
 * Input:
 *      index   - hared meter index (0-31)
 *      rate    - 17-bits rate of share meter, unit is 8Kpbs
 *      ifg     - Including IFG in rate calculation, 1:include 0:exclude
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter
 * Note:
 *      None
 */
ret_t l2sw_setAsicShareMeter(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 rate, l2sw_uint32 ifg)
{
    ret_t retVal;

    if(index > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    if(index < 32)
    {
    /*21-bits Rate*/
        retVal = l2sw_setAsicReg(units, L2SW_METER_RATE_REG(index), rate&0xFFFF);
        if(retVal != L2SW_ERR_OK)
            return retVal;
#if !defined(USE_L2SW_SDK)
        retVal = l2sw_setAsicReg(units, L2SW_METER_RATE_REG(index) + 1, (rate & 0x70000) >> 16);
#else
        retVal = l2sw_setAsicReg(units, L2SW_METER_RATE_REG(index) + 1, (rate & 0x1f0000) >> 16);
#endif
        if(retVal != L2SW_ERR_OK)
            return retVal;

        retVal = l2sw_setAsicRegBit(units, L2SW_METER_IFG_CTRL_REG(index), L2SW_METER_IFG_OFFSET(index), ifg);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
    /*21-bits Rate*/
        retVal = l2sw_setAsicReg(units, L2SW_REG_METER32_RATE_CTRL0 + ((index-32) << 1), rate&0xFFFF);
        if(retVal != L2SW_ERR_OK)
            return retVal;
            
#if !defined(USE_L2SW_SDK)
        retVal = l2sw_setAsicReg(units, L2SW_REG_METER32_RATE_CTRL0 + ((index-32) << 1) + 1, (rate & 0x70000) >> 16);
#else   
        retVal = l2sw_setAsicReg(units, L2SW_REG_METER32_RATE_CTRL0 + ((index-32) << 1) + 1, (rate & 0x1f0000) >> 16);
#endif

        if(retVal != L2SW_ERR_OK)
            return retVal;

        retVal = l2sw_setAsicRegBit(units, L2SW_REG_METER_IFG_CTRL2 + ((index-32) >> 4), L2SW_METER_IFG_OFFSET(index), ifg);
        if(retVal != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicShareMeter
 * Description:
 *      Get meter configuration
 * Input:
 *      index   - hared meter index (0-31)
 *      pRate   - 17-bits rate of share meter, unit is 8Kpbs
 *      pIfg    - Including IFG in rate calculation, 1:include 0:exclude
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter
 * Note:
 *      None
 */
ret_t l2sw_getAsicShareMeter(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 *pRate, l2sw_uint32 *pIfg)
{
    l2sw_uint32 regData;
    l2sw_uint32 regData2;
    ret_t retVal;

    if(index > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    if(index < 32)
    {
    /*17-bits Rate*/
     retVal = l2sw_getAsicReg(units, L2SW_METER_RATE_REG(index), &regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

     retVal = l2sw_getAsicReg(units, L2SW_METER_RATE_REG(index) + 1, &regData2);
        if(retVal != L2SW_ERR_OK)
            return retVal;
            
#if !defined(USE_L2SW_SDK)
    *pRate = ((regData2 << 16) & 0x70000) | regData;
#else
    *pRate = ((regData2 << 16) & 0x1f0000) | regData;
#endif
    /*IFG*/
    retVal = l2sw_getAsicRegBit(units, L2SW_METER_IFG_CTRL_REG(index), L2SW_METER_IFG_OFFSET(index), pIfg);

    return retVal;
    }
    else
    {
    /*17-bits Rate*/
     retVal = l2sw_getAsicReg(units, L2SW_REG_METER32_RATE_CTRL0 + ((index-32) << 1), &regData);
        if(retVal != L2SW_ERR_OK)
            return retVal;

     retVal = l2sw_getAsicReg(units, L2SW_REG_METER32_RATE_CTRL0 + ((index-32) << 1) + 1, &regData2);
        if(retVal != L2SW_ERR_OK)
            return retVal;
            
#if !defined(USE_L2SW_SDK)
    *pRate = ((regData2 << 16) & 0x70000) | regData;
#else
    *pRate = ((regData2 << 16) & 0x1f0000) | regData;
#endif    
    /*IFG*/
    retVal = l2sw_getAsicRegBit(units, L2SW_REG_METER_IFG_CTRL2 + ((index-32) >> 4), L2SW_METER_IFG_OFFSET(index), pIfg);

    return retVal;
    }
}
/* Function Name:
 *      l2sw_setAsicShareMeterBucketSize
 * Description:
 *      Set meter related leaky bucket threshold
 * Input:
 *      index       - hared meter index (0-31)
 *      lbthreshold - Leaky bucket threshold of meter
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter
 * Note:
 *      None
 */
ret_t l2sw_setAsicShareMeterBucketSize(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 lbthreshold)
{
    l2sw_uint32 ret;
    
    if(index > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    if(index < 32) {
        if((ret = l2sw_setAsicReg(units, L2SW_METER_BUCKET_SIZE_REG(index), lbthreshold & 0xffff)) != 0)
            return ret;
        if((ret = l2sw_setAsicRegBits(units, L2SW_REG_METER31_BUCKET_SIZE_H + index, L2SW_METER_BUCKET_MASK, (lbthreshold & 0x1f0000) >> 16)) != 0)
            return ret;
    }
    else {
        if((ret = l2sw_setAsicReg(units, L2SW_REG_METER32_BUCKET_SIZE + index - 32, lbthreshold & 0xffff)) != 0)
            return ret;
        if((ret = l2sw_setAsicRegBits(units, L2SW_REG_METER32_BUCKET_SIZE_H + index - 32, L2SW_METER_BUCKET_MASK, (lbthreshold & 0x1f0000) >> 16)) != 0)
            return ret;
    }

    return L2SW_ERR_OK;
}
/* Function Name:
 *      l2sw_getAsicShareMeterBucketSize
 * Description:
 *      Get meter related leaky bucket threshold
 * Input:
 *      index       - hared meter index (0-31)
 *      pLbthreshold - Leaky bucket threshold of meter
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter
 * Note:
 *      None
 */
ret_t l2sw_getAsicShareMeterBucketSize(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 *pLbthreshold)
{
    l2sw_uint32 ret = 0;
    l2sw_uint32 value = 0;
    
    if(index > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    if(index < 32) {
        if((ret = l2sw_getAsicReg(units, L2SW_METER_BUCKET_SIZE_REG(index), &value)) != 0)
            return ret;
            
        *pLbthreshold = value;
        
        if((ret = l2sw_getAsicRegBits(units, L2SW_REG_METER31_BUCKET_SIZE_H + index, L2SW_METER_BUCKET_MASK, &value)) != 0)
            return ret;
            
        *pLbthreshold |= (value << 16);
    }
    else {
        if((ret = l2sw_getAsicReg(units, L2SW_REG_METER32_BUCKET_SIZE + index - 32, pLbthreshold)) != 0)
            return ret;

        *pLbthreshold = value;
        
        if((ret = l2sw_getAsicRegBits(units, L2SW_REG_METER32_BUCKET_SIZE_H + index - 32, L2SW_METER_BUCKET_MASK, pLbthreshold)) != 0)
            return ret;
            
        *pLbthreshold |= (value << 16);
    }

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_setAsicShareMeterType
 * Description:
 *      Set meter Type
 * Input:
 *      index       - shared meter index (0-31)
 *      Type        - 0: kbps, 1: pps
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter
 * Note:
 *      None
 */
ret_t l2sw_setAsicShareMeterType(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 type)
{
    l2sw_uint32 reg;

    if(index > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    if(index < 32)
        reg = L2SW_REG_METER_MODE_SETTING0 + (index / 16);
    else
        reg = L2SW_REG_METER_MODE_SETTING2 + ((index - 32) / 16);

    return l2sw_setAsicRegBit(units, reg, index % 16, type);
}

/* Function Name:
 *      l2sw_getAsicShareMeterType
 * Description:
 *      Get meter Type
 * Input:
 *      index       - shared meter index (0-31)
 * Output:
 *      pType       - 0: kbps, 1: pps
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter
 * Note:
 *      None
 */
ret_t l2sw_getAsicShareMeterType(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 *pType)
{
    l2sw_uint32 reg;

    if(index > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    if(NULL == pType)
        return L2SW_ERR_NULL_POINTER;

    if(index < 32)
        reg = L2SW_REG_METER_MODE_SETTING0 + (index / 16);
    else
        reg = L2SW_REG_METER_MODE_SETTING2 + ((index - 32) / 16);

    return l2sw_getAsicRegBit(units, reg, index % 16, pType); 
}


/* Function Name:
 *      l2sw_setAsicMeterExceedStatus
 * Description:
 *      Clear shared meter status
 * Input:
 *      index       - hared meter index (0-31)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter
 * Note:
 *      None
 */
ret_t l2sw_setAsicMeterExceedStatus(l2sw_uint8 units, l2sw_uint32 index)
{
    if(index > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    if(index < 32)
        return l2sw_setAsicRegBit(units, L2SW_METER_OVERRATE_INDICATOR_REG(index), L2SW_METER_EXCEED_OFFSET(index), 1);
    else
        return l2sw_setAsicRegBit(units, L2SW_REG_METER_OVERRATE_INDICATOR2 + ((index - 32) >> 4), L2SW_METER_EXCEED_OFFSET(index), 1);

}
/* Function Name:
 *      l2sw_getAsicMeterExceedStatus
 * Description:
 *      Get shared meter status
 * Input:
 *      index   - hared meter index (0-31)
 *      pStatus     - 0: rate doesn't exceed    1: rate exceeds
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - Success
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_METER_ID  - Invalid meter
 * Note:
 *      If rate is over rate*8Kbps of a meter, the state bit of this meter is set to 1.
 */
ret_t l2sw_getAsicMeterExceedStatus(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pStatus)
{
    if(index > L2SW_METERMAX)
        return L2SW_ERR_FILTER_METER_ID;

    if(index < 32)
        return l2sw_getAsicRegBit(units, L2SW_METER_OVERRATE_INDICATOR_REG(index), L2SW_METER_EXCEED_OFFSET(index), pStatus);
    else
        return l2sw_getAsicRegBit(units, L2SW_REG_METER_OVERRATE_INDICATOR2 + ((index - 32) >> 4), L2SW_METER_EXCEED_OFFSET(index), pStatus);
}

