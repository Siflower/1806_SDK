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

#include "l2sw_asicdrv_eee.h"
#include "l2sw_asicdrv_phy.h"

/*
@func ret_t | l2sw_setAsicEee100M | Set eee force mode function enable/disable.
@parm l2sw_uint32 | port | The port number.
@parm l2sw_uint32 | enabled | 1: enabled, 0: disabled.
@rvalue L2SW_ERR_OK | Success.
@rvalue L2SW_ERR_SMI | SMI access error.
@rvalue L2SW_ERR_INPUT | Invalid input parameter.
@comm
    This API set the 100M EEE enable function.

*/
ret_t l2sw_setAsicEee100M(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enable)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32      regData;

    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    if (enable > 1)
        return L2SW_ERR_INPUT;

    if((retVal = l2sw_getAsicPHYOCPReg(units, port, EEE_OCP_PHY_ADDR, &regData)) != L2SW_ERR_OK)
        return retVal;

    if(enable)
        regData |= (0x0001 << 1);
    else
        regData &= ~(0x0001 << 1);

    if((retVal = l2sw_setAsicPHYOCPReg(units, port, EEE_OCP_PHY_ADDR, regData)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_getAsicReg(units, L2SW_PORT_EEE_CFG_REG(port), &regData)) != L2SW_ERR_OK)
        return retVal;

    if(enable)
        regData |= (0x0001 << 11);
    else
        regData &= ~(0x0001 << 11);

    if((retVal = l2sw_setAsicReg(units, L2SW_PORT_EEE_CFG_REG(port),regData)) != L2SW_ERR_OK)
        return retVal; 

    return L2SW_ERR_OK;
}

/*
@func ret_t | l2sw_getAsicEee100M | Get 100M eee enable/disable.
@parm l2sw_uint32 | port | The port number.
@parm l2sw_uint32* | enabled | 1: enabled, 0: disabled.
@rvalue L2SW_ERR_OK | Success.
@rvalue L2SW_ERR_SMI | SMI access error.
@rvalue L2SW_ERR_INPUT | Invalid input parameter.
@comm
    This API get the 100M EEE function.
*/
ret_t l2sw_getAsicEee100M(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *enable)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32      regData;

    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    if((retVal = l2sw_getAsicPHYOCPReg(units, port, EEE_OCP_PHY_ADDR, &regData)) != L2SW_ERR_OK)
        return retVal;

    *enable = (regData & (0x0001 << 1)) ? L2SW_ENABLED : L2SW_DISABLED;
    return L2SW_ERR_OK;
}

/*
@func ret_t | l2sw_setAsicEeeGiga | Set eee force mode function enable/disable.
@parm l2sw_uint32 | port | The port number.
@parm l2sw_uint32 | enabled | 1: enabled, 0: disabled.
@rvalue L2SW_ERR_OK | Success.
@rvalue L2SW_ERR_SMI | SMI access error.
@rvalue L2SW_ERR_INPUT | Invalid input parameter.
@comm
    This API set the 100M EEE enable function.

*/
ret_t l2sw_setAsicEeeGiga(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enable)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32      regData;

    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    if (enable > 1)
        return L2SW_ERR_INPUT;

    if((retVal = l2sw_getAsicPHYOCPReg(units, port, EEE_OCP_PHY_ADDR, &regData)) != L2SW_ERR_OK)
        return retVal;

    if(enable)
        regData |= (0x0001 << 2);
    else
        regData &= ~(0x0001 << 2);

    if((retVal = l2sw_setAsicPHYOCPReg(units, port, EEE_OCP_PHY_ADDR, regData)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_getAsicReg(units, L2SW_PORT_EEE_CFG_REG(port), &regData)) != L2SW_ERR_OK)
        return retVal;

    if(enable)
        regData |= (0x0001 << 10);
    else
        regData &= ~(0x0001 << 10);

    if((retVal = l2sw_setAsicReg(units, L2SW_PORT_EEE_CFG_REG(port),regData)) != L2SW_ERR_OK)
        return retVal; 

    return L2SW_ERR_OK;
}

/*
@func ret_t | l2sw_getAsicEeeGiga | Get 100M eee enable/disable.
@parm l2sw_uint32 | port | The port number.
@parm l2sw_uint32* | enabled | 1: enabled, 0: disabled.
@rvalue L2SW_ERR_OK | Success.
@rvalue L2SW_ERR_SMI | SMI access error.
@rvalue L2SW_ERR_INPUT | Invalid input parameter.
@comm
    This API get the 100M EEE function.
*/
ret_t l2sw_getAsicEeeGiga(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *enable)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32      regData;

    if(port >= L2SW_PORTNO)
        return L2SW_ERR_PORT_ID;

    if((retVal = l2sw_getAsicPHYOCPReg(units, port, EEE_OCP_PHY_ADDR, &regData)) != L2SW_ERR_OK)
        return retVal;

    *enable = (regData & (0x0001 << 2)) ? L2SW_ENABLED : L2SW_DISABLED;
    return L2SW_ERR_OK;
}
