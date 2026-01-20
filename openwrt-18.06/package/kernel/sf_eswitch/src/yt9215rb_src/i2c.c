/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "yt_error.h"
#include "yt_util.h"
#include "i2c.h"
#include "ctrlif.h"

uint32_t i2c_switch_write(uint8_t unit, uint32_t regAddr, uint32_t regValue)
{
    uint8_t addr[UINT32_BYTES_NUM];
    uint8_t value[UINT32_BYTES_NUM];
    int32_t ret;
    yt_i2c_mode_t i2cMode = SWITCH_I2C_CONTROLLER_ON_UNIT(unit).i2c_mode;
    bool isBigEndian = (i2cMode == YT_I2C_SIMPLE) ? 0 : 1;

    CMM_PARAM_CHK(NULL == SWITCH_I2C_CONTROLLER_ON_UNIT(unit).i2c_write, CMM_ERR_NOT_INIT);

    word_to_bytes(regAddr, addr, sizeof(addr), 1, isBigEndian);
    isBigEndian = (i2cMode == YT_I2C_STD_MSB) ? 1 : 0;
    word_to_bytes(regValue, value, sizeof(value), 0, isBigEndian);
    CMM_ERR_CHK(SWITCH_I2C_CONTROLLER_ON_UNIT(unit).i2c_write(addr, sizeof(addr), value, sizeof(value)), ret);

    return CMM_ERR_OK;
}

uint32_t i2c_switch_read(uint8_t unit, uint32_t regAddr, uint32_t *pRegValue)
{
    uint8_t addr[UINT32_BYTES_NUM] = {0};
    uint8_t value[UINT32_BYTES_NUM] = {0};
    int32_t ret;
    yt_i2c_mode_t i2cMode = SWITCH_I2C_CONTROLLER_ON_UNIT(unit).i2c_mode;
    bool isBigEndian = (i2cMode == YT_I2C_SIMPLE) ? 0 : 1;

    CMM_PARAM_CHK(NULL == pRegValue, CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(NULL == SWITCH_I2C_CONTROLLER_ON_UNIT(unit).i2c_read, CMM_ERR_NOT_INIT);

    word_to_bytes(regAddr, addr, sizeof(addr), 1, isBigEndian);
    CMM_ERR_CHK(SWITCH_I2C_CONTROLLER_ON_UNIT(unit).i2c_read(addr, sizeof(addr), value, sizeof(value)), ret);

    isBigEndian = (i2cMode == YT_I2C_STD_MSB) ? 1 : 0;
    CMM_ERR_CHK(bytes_to_word(value, sizeof(value), pRegValue, isBigEndian), ret);

    return CMM_ERR_OK;
}

