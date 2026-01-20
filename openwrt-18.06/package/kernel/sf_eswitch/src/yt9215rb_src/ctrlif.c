/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "yt_error.h"
#include "yt_types.h"
#include "yt_lock.h"
#include "cal_cmm.h"
#include "ctrlif.h"
#include "spi.h"
#include "smi.h"
#include "i2c.h"

static bool calc_parity(uint32_t data)
{
    bool parity = 0;
    uint32_t len = sizeof(data) * 8;
    uint32_t i;

    for (i = 0; i < len; i++)
    {
        if (data & 0x1)
        {
            parity = !parity;
        }
        data >>= 1;
    }

    return parity;
}

int32_t word_to_bytes(uint32_t data, uint8_t *pOut, uint8_t outLen, bool isAddr, bool bigEndian)
{
    uint32_t i;
    bool parity;

    CMM_PARAM_CHK((!pOut || outLen < sizeof(data)), CMM_ERR_INPUT);

    for (i = 0; i < UINT32_BYTES_NUM; i++)
    {
        if (bigEndian)
        {
            pOut[i] = (data >> ((UINT32_BYTES_NUM - i - 1) * 8)) & 0xff;
        }
        else
        {
            pOut[i] = (data >> (i * 8)) & 0xff;
        }
    }

    if (isAddr)
    {
        parity = calc_parity(data & 0xfffffffc);
        if (bigEndian)
        {
            pOut[UINT32_BYTES_NUM - 1] = (pOut[UINT32_BYTES_NUM - 1] & 0xfc) + parity;
        }
        else
        {
            pOut[0] = (pOut[0] & 0xfc) + parity;
        }
    }

    return CMM_ERR_OK;
}

int32_t bytes_to_word(uint8_t *pData, uint32_t len, uint32_t *regValue, bool bigEndian)
{
    uint32_t i;

    CMM_PARAM_CHK((!pData || !regValue || len > sizeof(*regValue)), CMM_ERR_INPUT);

    *regValue = 0;
    for (i = 0; i < len; i++)
    {
        if (bigEndian)
        {
            *regValue |= pData[i] << ((len - i - 1) * 8);
        }
        else
        {
            *regValue |= pData[i] << (i * 8);
        }
    }

    return CMM_ERR_OK;
}

uint32_t ctrlif_reg_write(uint8_t unit, uint32_t regAddr, uint32_t regValue)
{
    uint32_t ret = CMM_ERR_OK;
    yt_swAccMethod_t method;
    CMM_PARAM_CHK((UNITINFO(unit) == NULL), CMM_ERR_NOT_INIT);
    method = SWITCH_ACCESS_METHOD_ON_UNIT(unit);

    ACCESS_LOCK();
    switch(method)
    {        
        case SWCHIP_ACC_SPI:
            ret = spi_switch_write(unit, regAddr, regValue);
            break;
        
        case SWCHIP_ACC_I2C:
            ret = i2c_switch_write(unit, regAddr, regValue);
            break;
        
        case SWCHIP_ACC_SMI:
            ret = smi_switch_write(unit, regAddr, regValue);
            break;
        
        default:
            ret = CMM_ERR_NOT_SUPPORT;
            break;
    }
    ACCESS_UNLOCK();
    
    return ret;
}

uint32_t ctrlif_reg_read(uint8_t unit, uint32_t regAddr, uint32_t *pRegVale)
{
    uint32_t ret = CMM_ERR_OK;
    yt_swAccMethod_t method;
    CMM_PARAM_CHK((UNITINFO(unit) == NULL), CMM_ERR_NOT_INIT);
    method = SWITCH_ACCESS_METHOD_ON_UNIT(unit);

    ACCESS_LOCK();
    switch(method)
    {
        case SWCHIP_ACC_SMI:
            ret = smi_switch_read(unit, regAddr, pRegVale);
            break;
            
        case SWCHIP_ACC_SPI:
            ret = spi_switch_read(unit, regAddr, pRegVale);
            break;
        
        case SWCHIP_ACC_I2C:
            ret = i2c_switch_read(unit, regAddr, pRegVale);
            break;
            
        default:
            ret = CMM_ERR_NOT_INIT;
            break;
    }
    ACCESS_UNLOCK();
    
    return ret;
}
