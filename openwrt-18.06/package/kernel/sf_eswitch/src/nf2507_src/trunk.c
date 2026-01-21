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
#include "trunk.h"
#include "sal_string.h"
#include "l2sw_asicdrv_trunking.h"

static l2sw_api_ret_t _l2sw_trunk_port_set(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_portmask_t *pTrunk_member_portmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmsk;
    l2sw_uint32 regValue, type, tmp;
#if !defined(USE_L2SW_SDK)
    if((retVal = l2sw_setAsicReg(units, 0x13C2, 0x0249)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_getAsicReg(units, 0x1300, &regValue)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicReg(units, 0x13C2, 0x0000)) != L2SW_ERR_OK)
        return retVal;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            type = 0;
            break;
        case 0x0652:
        case 0x6368:
            type = 1;
            break;
        case 0x0801:
        case 0x6511:
            type = 2;
            break;
        default:
            return L2SW_ERR_FAILED;
    }

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Trunk Group Valid */
    L2SW_CHK_TRUNK_GROUP_VALID(units, trk_gid);

    L2SW_CHK_PORTMASK_VALID(units, pTrunk_member_portmask);

    if((retVal = l2sw_switch_portmask_L2P_get(units, pTrunk_member_portmask, &pmsk)) != L2SW_ERR_OK)
        return retVal;

    if((type == 0) || (type == 1))
    {
        if ((pmsk | L2SW_PORT_TRUNK_GROUP_MASK_MASK(trk_gid)) != (l2sw_uint32)L2SW_PORT_TRUNK_GROUP_MASK_MASK(trk_gid))
            return L2SW_ERR_PORT_MASK;

        pmsk = (pmsk & L2SW_PORT_TRUNK_GROUP_MASK_MASK(trk_gid)) >> L2SW_PORT_TRUNK_GROUP_MASK_OFFSET(trk_gid);
    }
    else if(type == 2)
    {
        tmp = 0;

        if(pmsk & 0x2)
            tmp |= 1;
        if(pmsk & 0x8)
            tmp |=2;
        if(pmsk & 0x80)
            tmp |=8;

        pmsk = tmp;
    }
#endif  /* !defined(USE_L2SW_SDK) */

    if(NULL == pTrunk_member_portmask)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_setAsicTrunkingGroup(units, trk_gid, pTrunk_member_portmask->bits[0])) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trunk_port_get(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_portmask_t *pTrunk_member_portmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmsk;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Trunk Group Valid */
    L2SW_CHK_TRUNK_GROUP_VALID(units, trk_gid);

    if ((retVal = l2sw_getAsicTrunkingGroup(units, trk_gid, &pmsk)) != L2SW_ERR_OK)
        return retVal;

    pmsk = pmsk << L2SW_PORT_TRUNK_GROUP_MASK_OFFSET(trk_gid);

    if((retVal = l2sw_switch_portmask_P2L_get(units, pmsk, pTrunk_member_portmask)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trunk_distributionAlgorithm_set(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_uint32 algo_bitmask)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (trk_gid != L2SW_WHOLE_SYSTEM)
        return L2SW_ERR_LA_TRUNK_ID;

    if (algo_bitmask >= 128)
        return L2SW_ERR_LA_HASHMASK;

    if ((retVal = l2sw_setAsicTrunkingHashSelect(units, algo_bitmask)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trunk_distributionAlgorithm_get(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_uint32 *pAlgo_bitmask)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (trk_gid != L2SW_WHOLE_SYSTEM)
        return L2SW_ERR_LA_TRUNK_ID;

    if(NULL == pAlgo_bitmask)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicTrunkingHashSelect(units, (l2sw_uint32 *)pAlgo_bitmask)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trunk_trafficSeparate_set(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_trunk_separateType_t separateType)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 enabled;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (trk_gid != L2SW_WHOLE_SYSTEM)
        return L2SW_ERR_LA_TRUNK_ID;

    if(separateType >= SEPARATE_END)
        return L2SW_ERR_INPUT;

    enabled = (separateType == SEPARATE_FLOOD) ? L2SW_ENABLED : L2SW_DISABLED;
    if ((retVal = l2sw_setAsicTrunkingFlood(units, enabled)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trunk_trafficSeparate_get(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_trunk_separateType_t *pSeparateType)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 enabled;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (trk_gid != L2SW_WHOLE_SYSTEM)
        return L2SW_ERR_LA_TRUNK_ID;

    if(NULL == pSeparateType)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicTrunkingFlood(units, &enabled)) != L2SW_ERR_OK)
        return retVal;

    *pSeparateType = (enabled == L2SW_ENABLED) ? SEPARATE_FLOOD : SEPARATE_NONE;
    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trunk_mode_set(l2sw_uint8 units, l2sw_trunk_mode_t mode)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(mode >= TRUNK_MODE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicTrunkingMode(units, (l2sw_uint32)mode)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trunk_mode_get(l2sw_uint8 units, l2sw_trunk_mode_t *pMode)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pMode)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicTrunkingMode(units, (l2sw_uint32 *)pMode)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trunk_trafficPause_set(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Trunk Group Valid */
    L2SW_CHK_TRUNK_GROUP_VALID(units, trk_gid);

    if(enable >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicTrunkingFc(units, (l2sw_uint32)trk_gid, (l2sw_uint32)enable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trunk_trafficPause_get(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Trunk Group Valid */
    L2SW_CHK_TRUNK_GROUP_VALID(units, trk_gid);

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicTrunkingFc(units, (l2sw_uint32)trk_gid, (l2sw_uint32 *)pEnable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trunk_hashMappingTable_set(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_trunk_hashVal2Port_t *pHash2Port_array)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 hashValue;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Trunk Group Valid */
    L2SW_CHK_TRUNK_GROUP_VALID(units, trk_gid);

    if(NULL == pHash2Port_array)
        return L2SW_ERR_NULL_POINTER;

    if(trk_gid <= TRUNK_GROUP1)
    {
        for(hashValue = 0; hashValue < L2SW_MAX_NUM_OF_TRUNK_HASH_VAL; hashValue++)
        {
            if ((retVal = l2sw_setAsicTrunkingHashTable(units, hashValue, pHash2Port_array->value[hashValue])) != L2SW_ERR_OK)
                return retVal;
        }
    }
    else
    {
        for(hashValue = 0; hashValue < L2SW_MAX_NUM_OF_TRUNK_HASH_VAL; hashValue++)
        {
            if ((retVal = l2sw_setAsicTrunkingHashTable1(units, hashValue, pHash2Port_array->value[hashValue])) != L2SW_ERR_OK)
                return retVal;
        }
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trunk_hashMappingTable_get(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_trunk_hashVal2Port_t *pHash2Port_array)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 hashValue;
    l2sw_uint32 hashPort;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Trunk Group Valid */
    L2SW_CHK_TRUNK_GROUP_VALID(units, trk_gid);

    if(NULL == pHash2Port_array)
        return L2SW_ERR_NULL_POINTER;

    if(trk_gid <= TRUNK_GROUP1)
    {
        for(hashValue = 0; hashValue < L2SW_MAX_NUM_OF_TRUNK_HASH_VAL; hashValue++)
        {
            if ((retVal = l2sw_getAsicTrunkingHashTable(units, hashValue, &hashPort)) != L2SW_ERR_OK)
                return retVal;

            pHash2Port_array->value[hashValue] = hashPort;
        }
    }
    else
    {
        for(hashValue = 0; hashValue < L2SW_MAX_NUM_OF_TRUNK_HASH_VAL; hashValue++)
        {
            if ((retVal = l2sw_getAsicTrunkingHashTable1(units, hashValue, &hashPort)) != L2SW_ERR_OK)
                return retVal;

            pHash2Port_array->value[hashValue] = hashPort;
        }
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_trunk_portQueueEmpty_get(l2sw_uint8 units, l2sw_portmask_t *pEmpty_portmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pEmpty_portmask)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicQeueuEmptyStatus(units, &pmask)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_switch_portmask_P2L_get(units, pmask, pEmpty_portmask)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_trunk_port_set
 * Description:
 *      Set trunking group available port mask
 * Input:
 *      trk_gid                 - trunk group id
 *      pTrunk_member_portmask  - Logic trunking member port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_LA_TRUNK_ID  - Invalid trunking group
 *      L2SW_ERR_PORT_MASK    - Invalid portmask.
 * Note:
 *      The API can set port trunking group port mask. Each port trunking group has max 4 ports.
 *      If enabled port mask has less than 2 ports available setting, then this trunking group function is disabled.
 */
l2sw_api_ret_t l2sw_trunk_port_set(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_portmask_t *pTrunk_member_portmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trunk_port_set(units, trk_gid, pTrunk_member_portmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_trunk_port_get
 * Description:
 *      Get trunking group available port mask
 * Input:
 *      trk_gid - trunk group id
 * Output:
 *      pTrunk_member_portmask - Logic trunking member port mask
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_LA_TRUNK_ID  - Invalid trunking group
 * Note:
 *      The API can get 2 port trunking group.
 */
l2sw_api_ret_t l2sw_trunk_port_get(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_portmask_t *pTrunk_member_portmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trunk_port_get(units, trk_gid, pTrunk_member_portmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_trunk_distributionAlgorithm_set
 * Description:
 *      Set port trunking hash select sources
 * Input:
 *      trk_gid         - trunk group id
 *      algo_bitmask   - Bitmask of the distribution algorithm
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_LA_TRUNK_ID  - Invalid trunking group
 *      L2SW_ERR_LA_HASHMASK  - Hash algorithm selection error.
 *      L2SW_ERR_PORT_MASK    - Invalid portmask.
 * Note:
 *      The API can set port trunking hash algorithm sources.
 *      7 bits mask for link aggregation group0 hash parameter selection {DIP, SIP, DMAC, SMAC, SPA}
 *      - 0b0000001: SPA
 *      - 0b0000010: SMAC
 *      - 0b0000100: DMAC
 *      - 0b0001000: SIP
 *      - 0b0010000: DIP
 *      - 0b0100000: TCP/UDP Source Port
 *      - 0b1000000: TCP/UDP Destination Port
 *      Example:
 *      - 0b0000011: SMAC & SPA
 *      - Note that it could be an arbitrary combination or independent set
 */
l2sw_api_ret_t l2sw_trunk_distributionAlgorithm_set(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_uint32 algo_bitmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trunk_distributionAlgorithm_set(units, trk_gid, algo_bitmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_trunk_distributionAlgorithm_get
 * Description:
 *      Get port trunking hash select sources
 * Input:
 *      trk_gid - trunk group id
 * Output:
 *      pAlgo_bitmask -  Bitmask of the distribution algorithm
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_LA_TRUNK_ID  - Invalid trunking group
 * Note:
 *      The API can get port trunking hash algorithm sources.
 */
l2sw_api_ret_t l2sw_trunk_distributionAlgorithm_get(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_uint32 *pAlgo_bitmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trunk_distributionAlgorithm_get(units, trk_gid, pAlgo_bitmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_trunk_trafficSeparate_set
 * Description:
 *      Set the traffic separation setting of a trunk group from the specified device.
 * Input:
 *      trk_gid      - trunk group id
 *      separateType     - traffic separation setting
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_UNIT_ID     - invalid unit id
 *      L2SW_ERR_LA_TRUNK_ID - invalid trunk ID
 *      L2SW_ERR_LA_HASHMASK - invalid hash mask
 * Note:
 *      SEPARATE_NONE: disable traffic separation
 *      SEPARATE_FLOOD: trunk MSB link up port is dedicated to TX flooding (L2 lookup miss) traffic
 */
l2sw_api_ret_t l2sw_trunk_trafficSeparate_set(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_trunk_separateType_t separateType)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trunk_trafficSeparate_set(units, trk_gid, separateType);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_trunk_trafficSeparate_get
 * Description:
 *      Get the traffic separation setting of a trunk group from the specified device.
 * Input:
 *      trk_gid        - trunk group id
 * Output:
 *      pSeparateType   - pointer separated traffic type
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_UNIT_ID      - invalid unit id
 *      L2SW_ERR_LA_TRUNK_ID  - invalid trunk ID
 *      L2SW_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      SEPARATE_NONE: disable traffic separation
 *      SEPARATE_FLOOD: trunk MSB link up port is dedicated to TX flooding (L2 lookup miss) traffic
 */
l2sw_api_ret_t l2sw_trunk_trafficSeparate_get(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_trunk_separateType_t *pSeparateType)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trunk_trafficSeparate_get(units, trk_gid, pSeparateType);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_trunk_mode_set
 * Description:
 *      Set the trunk mode to the specified device.
 * Input:
 *      mode - trunk mode
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_INPUT   - invalid input parameter
 * Note:
 *      The enum of the trunk mode as following
 *      - TRUNK_MODE_NORMAL
 *      - TRUNK_MODE_DUMB
 */
l2sw_api_ret_t l2sw_trunk_mode_set(l2sw_uint8 units, l2sw_trunk_mode_t mode)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trunk_mode_set(units, mode);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_trunk_mode_get
 * Description:
 *      Get the trunk mode from the specified device.
 * Input:
 *      None
 * Output:
 *      pMode - pointer buffer of trunk mode
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      The enum of the trunk mode as following
 *      - TRUNK_MODE_NORMAL
 *      - TRUNK_MODE_DUMB
 */
l2sw_api_ret_t l2sw_trunk_mode_get(l2sw_uint8 units, l2sw_trunk_mode_t *pMode)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trunk_mode_get(units, pMode);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_trunk_trafficPause_set
 * Description:
 *      Set the traffic pause setting of a trunk group.
 * Input:
 *      trk_gid      - trunk group id
 *      enable       - traffic pause state
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_LA_TRUNK_ID - invalid trunk ID
 * Note:
 *      None.
 */
l2sw_api_ret_t l2sw_trunk_trafficPause_set(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trunk_trafficPause_set(units, trk_gid, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_trunk_trafficPause_get
 * Description:
 *      Get the traffic pause setting of a trunk group.
 * Input:
 *      trk_gid        - trunk group id
 * Output:
 *      pEnable        - pointer of traffic pause state.
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_LA_TRUNK_ID  - invalid trunk ID
 *      L2SW_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None.
 */
l2sw_api_ret_t l2sw_trunk_trafficPause_get(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trunk_trafficPause_get(units, trk_gid, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_trunk_hashMappingTable_set
 * Description:
 *      Set hash value to port array in the trunk group id from the specified device.
 * Input:
 *      trk_gid          - trunk group id
 *      pHash2Port_array - ports associate with the hash value
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_UNIT_ID            - invalid unit id
 *      L2SW_ERR_LA_TRUNK_ID        - invalid trunk ID
 *      L2SW_ERR_NULL_POINTER       - input parameter may be null pointer
 *      L2SW_ERR_LA_TRUNK_NOT_EXIST - the trunk doesn't exist
 *      L2SW_ERR_LA_NOT_MEMBER_PORT - the port is not a member port of the trunk
 *      L2SW_ERR_LA_CPUPORT         - CPU port can not be aggregated port
 * Note:
 *      Trunk group 0 & 1 shares the same hash mapping table.
 *      Trunk group 2 uses a independent table.
 */
l2sw_api_ret_t l2sw_trunk_hashMappingTable_set(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_trunk_hashVal2Port_t *pHash2Port_array)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trunk_hashMappingTable_set(units, trk_gid, pHash2Port_array);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_trunk_hashMappingTable_get
 * Description:
 *      Get hash value to port array in the trunk group id from the specified device.
 * Input:
 *      trk_gid          - trunk group id
 * Output:
 *      pHash2Port_array - pointer buffer of ports associate with the hash value
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_UNIT_ID      - invalid unit id
 *      L2SW_ERR_LA_TRUNK_ID  - invalid trunk ID
 *      L2SW_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      Trunk group 0 & 1 shares the same hash mapping table.
 *      Trunk group 2 uses a independent table.
 */
l2sw_api_ret_t l2sw_trunk_hashMappingTable_get(l2sw_uint8 units, l2sw_trunk_group_t trk_gid, l2sw_trunk_hashVal2Port_t *pHash2Port_array)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trunk_hashMappingTable_get(units, trk_gid, pHash2Port_array);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_trunk_portQueueEmpty_get
 * Description:
 *      Get the port mask which all queues are empty.
 * Input:
 *      None.
 * Output:
 *      pEmpty_portmask   - pointer empty port mask
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None.
 */
l2sw_api_ret_t l2sw_trunk_portQueueEmpty_get(l2sw_uint8 units, l2sw_portmask_t *pEmpty_portmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_trunk_portQueueEmpty_get(units, pEmpty_portmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}


