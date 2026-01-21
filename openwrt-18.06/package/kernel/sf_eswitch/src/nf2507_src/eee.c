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
#include "eee.h"
#ifdef L2SW_LINUX_KERNEL
#include "linux/kernel.h"
#endif
#include "sal_string.h"
#include "l2sw_asicdrv_eee.h"
#include "l2sw_asicdrv_phy.h"
#include "port.h"

static l2sw_api_ret_t _l2sw_eee_init(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if((retVal = l2sw_setAsicRegBit(units, 0x0018, 10, 1)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicRegBit(units, 0x0018, 11, 1)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_eee_portEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32      phyregData;
    l2sw_uint32      macregData;
    l2sw_uint32      regData;
    l2sw_uint32    phy_port;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

#if defined(USE_L2SW_SDK)
    L2SW_CHK_PORT_VALID(units, port);
#else 
    /* Check port is UTP port */
    L2SW_CHK_PORT_IS_UTP(units, port);
#endif

    if (enable>=L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    phy_port = l2sw_switch_port_L2P_get(units, port);
    
#if defined(USE_L2SW_SDK)
    if((retVal = l2sw_getAsicReg(units, L2SW_PORT_EEE_CFG_REG(phy_port), &regData)) != L2SW_ERR_OK)
        return retVal;   
    /* set bit0 and bit1 to 1 or 0 */
    if(enable == L2SW_ENABLED) {
        regData &= ~0x1;
        regData |= 0x2;
    }
    else { 
        regData |= 0x1;
        regData &= ~0x2;
    }
    
    if ((retVal = l2sw_getAsicPHYReg(units, phy_port, PHY_RESOLVED_REG, &phyregData)) != L2SW_ERR_OK)
        return retVal;
    
    /* set bit2-8 to 3mhz or 25mhz or 125mhz by speed */    
    if(((phyregData&0x0030) >> 4) == PORT_SPEED_10M) {
        regData &= 0xfffffe03;
        regData |= 0xc;
    }
    else if(((phyregData&0x0030) >> 4) == PORT_SPEED_100M) {
        regData &= 0xfffffe03;
        regData |= 0x64;
    }
    else if(((phyregData&0x0030) >> 4) == PORT_SPEED_1000M) {
        regData &= 0xfffffe03;
        regData |= 0x1f4;
    }
    
    if((retVal = l2sw_setAsicReg(units, L2SW_PORT_EEE_CFG_REG(phy_port), regData)) != L2SW_ERR_OK)
        return retVal; 
#else
/*
    if ((retVal = l2sw_setAsicEee100M(units, phy_port,enable))!=L2SW_ERR_OK)
        return retVal;
    if ((retVal = l2sw_setAsicEeeGiga(units, phy_port,enable))!=L2SW_ERR_OK)
        return retVal;
    if ((retVal = l2sw_setAsicPHYReg(units, phy_port, L2SW_PHY_PAGE_ADDRESS, 0))!=L2SW_ERR_OK)
        return retVal;
    if ((retVal = l2sw_getAsicPHYReg(units, phy_port, 0, &regData))!=L2SW_ERR_OK)
        return retVal;
    regData |= 0x0200;
    if ((retVal = l2sw_setAsicPHYReg(units, phy_port, 0, regData))!=L2SW_ERR_OK)
        return retVal;*/
#endif

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_eee_portEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 regData1;
    l2sw_uint32    phy_port;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);
    
#if defined(USE_L2SW_SDK)
    L2SW_CHK_PORT_VALID(units, port);
#else 
    /* Check port is UTP port */
    L2SW_CHK_PORT_IS_UTP(units, port);
#endif

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    phy_port = l2sw_switch_port_L2P_get(units, port);
    
#if defined(USE_L2SW_SDK)
    if((retVal = l2sw_getAsicReg(units, L2SW_PORT_EEE_CFG_REG(phy_port), &regData1)) != L2SW_ERR_OK)
        return retVal;  

    /* get bit9 */
    if((regData1 & 0x200) != 0)
        *pEnable = L2SW_ENABLED;
    else
        *pEnable = L2SW_DISABLED;

#else
/*
    if ((retVal = l2sw_getAsicEee100M(units, phy_port,&regData1))!=L2SW_ERR_OK)
        return retVal;
    if ((retVal = l2sw_getAsicEeeGiga(units, phy_port,&regData2))!=L2SW_ERR_OK)
        return retVal;

    if (regData1==1&&regData2==1)
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;*/
#endif

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_eee_init
 * Description:
 *      EEE function initialization.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 *      This API is used to initialize EEE status.
 */
l2sw_api_ret_t l2sw_eee_init(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_eee_init(units);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_eee_portEnable_set
 * Description:
 *      Set enable status of EEE function.
 * Input:
 *      port - port id.
 *      enable - enable EEE status.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_PORT_ID - Invalid port number.
 *      L2SW_ERR_ENABLE - Invalid enable input.
 * Note:
 *      This API can set EEE function to the specific port.
 *      The configuration of the port is as following:
 *      - DISABLE
 *      - ENABLE
 */
l2sw_api_ret_t l2sw_eee_portEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_eee_portEnable_set(units, port, enable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_eee_portEnable_get
 * Description:
 *      Get enable status of EEE function
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - Back pressure status.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_PORT_ID - Invalid port number.
 * Note:
 *      This API can get EEE function to the specific port.
 *      The configuration of the port is as following:
 *      - DISABLE
 *      - ENABLE
 */

l2sw_api_ret_t l2sw_eee_portEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_eee_portEnable_get(units, port, pEnable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}


