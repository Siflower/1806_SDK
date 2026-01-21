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

#include "l2sw_asicdrv_trunking.h"
/* Function Name:
 *      l2sw_setAsicTrunkingMode
 * Description:
 *      Set port trunking mode
 * Input:
 *      mode    - 1:dumb 0:user defined
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicTrunkingMode(l2sw_uint8 units, l2sw_uint32 mode)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_PORT_TRUNK_CTRL, L2SW_PORT_TRUNK_DUMB_OFFSET, mode);
}
/* Function Name:
 *      l2sw_getAsicTrunkingMode
 * Description:
 *      Get port trunking mode
 * Input:
 *      pMode   - 1:dumb 0:user defined
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicTrunkingMode(l2sw_uint8 units, l2sw_uint32* pMode)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_PORT_TRUNK_CTRL, L2SW_PORT_TRUNK_DUMB_OFFSET, pMode);
}
/* Function Name:
 *      l2sw_setAsicTrunkingFc
 * Description:
 *      Set port trunking flow control
 * Input:
 *      group       - Trunk Group ID
 *      enabled     - 0:disable, 1:enable
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicTrunkingFc(l2sw_uint8 units, l2sw_uint32 group, l2sw_uint32 enabled)
{
    ret_t       retVal;

    if(group > L2SW_MAX_TRUNK_GID)
        return L2SW_ERR_LA_TRUNK_ID;

    if((retVal = l2sw_setAsicRegBit(units, L2SW_REG_PORT_TRUNK_DROP_CTRL, L2SW_PORT_TRUNK_DROP_CTRL_OFFSET, L2SW_ENABLED)) != L2SW_ERR_OK)
        return retVal;

    return l2sw_setAsicRegBit(units, L2SW_REG_PORT_TRUNK_FLOWCTRL, (L2SW_EN_FLOWCTRL_TG0_OFFSET + group), enabled);
}
/* Function Name:
 *      l2sw_getAsicTrunkingFc
 * Description:
 *      Get port trunking flow control
 * Input:
 *      group       - Trunk Group ID
 *      pEnabled    - 0:disable, 1:enable
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicTrunkingFc(l2sw_uint8 units, l2sw_uint32 group, l2sw_uint32* pEnabled)
{
    if(group > L2SW_MAX_TRUNK_GID)
        return L2SW_ERR_LA_TRUNK_ID;

    return l2sw_getAsicRegBit(units, L2SW_REG_PORT_TRUNK_FLOWCTRL, (L2SW_EN_FLOWCTRL_TG0_OFFSET + group), pEnabled);
}
/* Function Name:
 *      l2sw_setAsicTrunkingGroup
 * Description:
 *      Set trunking group available port mask
 * Input:
 *      group       - Trunk Group ID
 *      portmask    - Logic trunking enable port mask, max 4 ports
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicTrunkingGroup(l2sw_uint8 units, l2sw_uint32 group, l2sw_uint32 portmask)
{
    if(group > L2SW_MAX_TRUNK_GID)
        return L2SW_ERR_LA_TRUNK_ID;
    return l2sw_setAsicRegBits(units, L2SW_REG_PORT_TRUNK_GROUP_MASK, L2SW_PORT_TRUNK_GROUP0_MASK_MASK << (group * 4), portmask);
}
/* Function Name:
 *      l2sw_getAsicTrunkingGroup
 * Description:
 *      Get trunking group available port mask
 * Input:
 *      group       - Trunk Group ID
 * Output:
 *      pPortmask   - Logic trunking enable port mask, max 4 ports
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicTrunkingGroup(l2sw_uint8 units, l2sw_uint32 group, l2sw_uint32* pPortmask)
{
    if(group > L2SW_MAX_TRUNK_GID)
        return L2SW_ERR_LA_TRUNK_ID;

    return l2sw_getAsicRegBits(units, L2SW_REG_PORT_TRUNK_GROUP_MASK, L2SW_PORT_TRUNK_GROUP0_MASK_MASK << (group * 4), pPortmask);
}
/* Function Name:
 *      l2sw_setAsicTrunkingFlood
 * Description:
 *      Set port trunking flood function
 * Input:
 *      enabled     - Port trunking flooding function 0:disable 1:enable
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_setAsicTrunkingFlood(l2sw_uint8 units, l2sw_uint32 enabled)
{
    return l2sw_setAsicRegBit(units, L2SW_REG_PORT_TRUNK_CTRL, L2SW_PORT_TRUNK_FLOOD_OFFSET, enabled);
}
/* Function Name:
 *      l2sw_getAsicTrunkingFlood
 * Description:
 *      Get port trunking flood function
 * Input:
 *      pEnabled    - Port trunking flooding function 0:disable 1:enable
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicTrunkingFlood(l2sw_uint8 units, l2sw_uint32* pEnabled)
{
    return l2sw_getAsicRegBit(units, L2SW_REG_PORT_TRUNK_CTRL, L2SW_PORT_TRUNK_FLOOD_OFFSET, pEnabled);
}
/* Function Name:
 *      l2sw_setAsicTrunkingHashSelect
 * Description:
 *      Set port trunking hash select sources
 * Input:
 *      hashsel     - hash sources mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      7 bits mask for link aggregation group0 hash parameter selection {DIP, SIP, DMAC, SMAC, SPA}
 *      0b0000001: SPA
 *      0b0000010: SMAC
 *      0b0000100: DMAC
 *      0b0001000: SIP
 *      0b0010000: DIP
 *      0b0100000: TCP/UDP Source Port
 *      0b1000000: TCP/UDP Destination Port
 */
ret_t l2sw_setAsicTrunkingHashSelect(l2sw_uint8 units, l2sw_uint32 hashsel)
{
    return l2sw_setAsicRegBits(units, L2SW_REG_PORT_TRUNK_CTRL, L2SW_PORT_TRUNK_HASH_MASK, hashsel);
}
/* Function Name:
 *      l2sw_getAsicTrunkingHashSelect
 * Description:
 *      Get port trunking hash select sources
 * Input:
 *      pHashsel    - hash sources mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicTrunkingHashSelect(l2sw_uint8 units, l2sw_uint32* pHashsel)
{
    return l2sw_getAsicRegBits(units, L2SW_REG_PORT_TRUNK_CTRL, L2SW_PORT_TRUNK_HASH_MASK, pHashsel);
}
/* Function Name:
 *      l2sw_getAsicQeueuEmptyStatus
 * Description:
 *      Get current output queue if empty status
 * Input:
 *      portmask    - queue empty port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK   - Success
 *      L2SW_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t l2sw_getAsicQeueuEmptyStatus(l2sw_uint8 units, l2sw_uint32* portmask)
{
    return l2sw_getAsicReg(units, L2SW_REG_PORT_QEMPTY, portmask);
}
/* Function Name:
 *      l2sw_setAsicTrunkingHashTable
 * Description:
 *      Set port trunking hash value mapping table
 * Input:
 *      hashval     - hashing value 0-15
 *      portId      - trunking port id 0-3
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_OUT_OF_RANGE - Invalid hashing value (0-15)
 * Note:
 *      None
 */
ret_t l2sw_setAsicTrunkingHashTable(l2sw_uint8 units, l2sw_uint32 hashval, l2sw_uint32 portId)
{
    if(hashval > L2SW_TRUNKING_HASHVALUE_MAX)
        return L2SW_ERR_OUT_OF_RANGE;

    if(portId >= L2SW_TRUNKING_PORTNO)
        return L2SW_ERR_PORT_ID;

    if(hashval >= 8)
        return l2sw_setAsicRegBits(units, L2SW_REG_PORT_TRUNK_HASH_MAPPING_CTRL1, L2SW_PORT_TRUNK_HASH_MAPPING_CTRL1_HASH8_MASK<<((hashval-8)*2), portId);
    else
        return l2sw_setAsicRegBits(units, L2SW_REG_PORT_TRUNK_HASH_MAPPING_CTRL0, L2SW_PORT_TRUNK_HASH_MAPPING_CTRL0_HASH0_MASK<<(hashval*2), portId);
}
/* Function Name:
 *      l2sw_getAsicTrunkingHashTable
 * Description:
 *      Get port trunking hash value mapping table
 * Input:
 *      hashval     - hashing value 0-15
 *      pPortId         - trunking port id 0-3
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - Invalid hashing value (0-15)
 * Note:
 *      None
 */
ret_t l2sw_getAsicTrunkingHashTable(l2sw_uint8 units, l2sw_uint32 hashval, l2sw_uint32* pPortId)
{
    if(hashval > L2SW_TRUNKING_HASHVALUE_MAX)
        return L2SW_ERR_OUT_OF_RANGE;

    if(hashval >= 8)
        return l2sw_getAsicRegBits(units, L2SW_REG_PORT_TRUNK_HASH_MAPPING_CTRL1, L2SW_PORT_TRUNK_HASH_MAPPING_CTRL1_HASH8_MASK<<((hashval-8)*2), pPortId);
    else
        return l2sw_getAsicRegBits(units, L2SW_REG_PORT_TRUNK_HASH_MAPPING_CTRL0, L2SW_PORT_TRUNK_HASH_MAPPING_CTRL0_HASH0_MASK<<(hashval*2), pPortId);
}

/* Function Name:
 *      l2sw_setAsicTrunkingHashTable1
 * Description:
 *      Set port trunking hash value mapping table
 * Input:
 *      hashval     - hashing value 0-15
 *      portId      - trunking port id 0-3
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_OUT_OF_RANGE - Invalid hashing value (0-15)
 * Note:
 *      None
 */
ret_t l2sw_setAsicTrunkingHashTable1(l2sw_uint8 units, l2sw_uint32 hashval, l2sw_uint32 portId)
{
    if(hashval > L2SW_TRUNKING_HASHVALUE_MAX)
        return L2SW_ERR_OUT_OF_RANGE;

    if(portId >= L2SW_TRUNKING1_PORTN0)
        return L2SW_ERR_PORT_ID;

    if(hashval >= 8)
        return l2sw_setAsicRegBits(units, L2SW_REG_PORT_TRUNK_HASH_MAPPING_CTRL3, L2SW_PORT_TRUNK_HASH_MAPPING_CTRL3_HASH8_MASK<<((hashval-8)*2), portId);
    else
        return l2sw_setAsicRegBits(units, L2SW_REG_PORT_TRUNK_HASH_MAPPING_CTRL2, L2SW_PORT_TRUNK_HASH_MAPPING_CTRL2_HASH0_MASK<<(hashval*2), portId);
}
/* Function Name:
 *      l2sw_getAsicTrunkingHashTable1
 * Description:
 *      Get port trunking hash value mapping table
 * Input:
 *      hashval     - hashing value 0-15
 *      pPortId         - trunking port id 0-3
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Success
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE - Invalid hashing value (0-15)
 * Note:
 *      None
 */
ret_t l2sw_getAsicTrunkingHashTable1(l2sw_uint8 units, l2sw_uint32 hashval, l2sw_uint32* pPortId)
{
    if(hashval > L2SW_TRUNKING_HASHVALUE_MAX)
        return L2SW_ERR_OUT_OF_RANGE;

    if(hashval >= 8)
        return l2sw_getAsicRegBits(units, L2SW_REG_PORT_TRUNK_HASH_MAPPING_CTRL3, L2SW_PORT_TRUNK_HASH_MAPPING_CTRL3_HASH8_MASK<<((hashval-8)*2), pPortId);
    else
        return l2sw_getAsicRegBits(units, L2SW_REG_PORT_TRUNK_HASH_MAPPING_CTRL2, L2SW_PORT_TRUNK_HASH_MAPPING_CTRL2_HASH0_MASK<<(hashval*2), pPortId);
}

