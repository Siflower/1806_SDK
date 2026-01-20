/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_tiger_gpio.h"
#include "hal_mem.h"


yt_ret_t fal_tiger_gpio_init(yt_unit_t unit)
{
    CMM_UNUSED_PARAM(unit);
    /* TODO: init paratemers*/

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_gpio_mdioMaster_pinGroup_set(yt_unit_t unit, yt_mdio_master_t mdiom, yt_enable_t enable)
{
    uint32_t ret;
    uint32_t regVal;
    uint32_t valMask = 0x1c;
    uint32_t valuecfg = 0;

    if(mdiom == YT_MDIO_MASTER_1)
    {
        if(enable == YT_ENABLE)
        {
            valuecfg = 0x4U;
        }
    }
    else if(mdiom == YT_MDIO_MASTER_2)
    {
        if(enable == YT_ENABLE)
        {
            valuecfg = 0x8U;
        }
    }
    else if(mdiom == YT_MDIO_MASTER_3)
    {
        if(enable == YT_ENABLE)
        {
            valuecfg = 0x10U;
        }
    }
    else
    {
        /*nothing to do*/
    }
    
    ret = HAL_MEM_DIRECT_READ(unit, MODE_SEL, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }

    regVal &= (~valMask);
    regVal |= valuecfg;
    HAL_MEM_DIRECT_WRITE(unit, MODE_SEL, regVal);      
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_gpio_mdioMaster_pinGroup_get(yt_unit_t unit, yt_mdio_master_t mdiom, yt_enable_t *pEnable)
{
    uint32_t ret;
    uint32_t regVal;
    
    ret = HAL_MEM_DIRECT_READ(unit, MODE_SEL, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }

    if(mdiom == YT_MDIO_MASTER_1)
    {
        if(((regVal >> 2) & 0x1) == 1)
        {
            *pEnable = YT_ENABLE;
        }
        else
        {
            *pEnable = YT_DISABLE;
        }
    }
    else if(mdiom == YT_MDIO_MASTER_2)
    {
        if(((regVal >> 3) & 0x1) == 1)
        {
            *pEnable = YT_ENABLE;
        }
        else
        {
            *pEnable = YT_DISABLE;
        }
    }
    else if(mdiom == YT_MDIO_MASTER_3)
    {
        if(((regVal >> 4) & 0x1) == 1)
        {
            *pEnable = YT_ENABLE;
        }
        else
        {
            *pEnable = YT_DISABLE;
        }
    }
    else
    {
        /*nothing to do*/
    }
    
    return CMM_ERR_OK;
}