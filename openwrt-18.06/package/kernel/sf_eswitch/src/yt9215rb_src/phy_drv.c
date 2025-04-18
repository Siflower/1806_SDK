/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#include "phy_drv.h"
#include "hal_ctrl.h"
#include "phy_int_yt861x.h"
#include "phy_yt8531.h"
#include "phy_int_serdes.h"

yt_phy_drv_t *gpPhyDrvList[] =
{
    [YT_PHY_MODEL_INT861X] = &int_yt861x_drv,
#ifdef PHY_YT8531_INCLUDED
    [YT_PHY_MODEL_8531] = &yt8531_drv,
#else
    [YT_PHY_MODEL_8531] = NULL,
#endif
#if defined(BOARD_YT9215SC_DEFAULT_DEMO) || defined(BOARD_YT9215S_FIB_DEMO) || defined(BOARD_YT9215S_YT8531_FIB_DEMO)
    [YT_PHY_MODEL_INTSERDES] = &int_serdes_drv,
#else
    [YT_PHY_MODEL_INTSERDES] = NULL,
#endif
};

yt_ret_t int_phy_mmd_reg_read(yt_unit_t unit, uint8_t phy_addr, uint8_t mmd_id, uint16_t regAddr, uint16_t *pData)
{
    if(HALSWDRV(unit) == NULL || pData == NULL)
    {
        return CMM_ERR_FAIL;
    }

    HALSWDRV_FUNC(unit)->switch_intif_write(unit, phy_addr, 0xd, mmd_id);
    HALSWDRV_FUNC(unit)->switch_intif_write(unit, phy_addr, 0xe, regAddr);
    HALSWDRV_FUNC(unit)->switch_intif_write(unit, phy_addr, 0xd, 0x4000+mmd_id);
    HALSWDRV_FUNC(unit)->switch_intif_read(unit, phy_addr, 0xe, pData);

    return CMM_ERR_OK;
}

yt_ret_t int_phy_mmd_reg_write(yt_unit_t unit, uint8_t phy_addr, uint8_t mmd_id, uint16_t regAddr, uint16_t data)
{
    if(HALSWDRV(unit) == NULL)
    {
        return CMM_ERR_FAIL;
    }

    HALSWDRV_FUNC(unit)->switch_intif_write(unit, phy_addr, 0xd, mmd_id);
    HALSWDRV_FUNC(unit)->switch_intif_write(unit, phy_addr, 0xe, regAddr);
    HALSWDRV_FUNC(unit)->switch_intif_write(unit, phy_addr, 0xd, 0x4000+mmd_id);
    HALSWDRV_FUNC(unit)->switch_intif_write(unit, phy_addr, 0xe, data);

    return CMM_ERR_OK;
}

yt_ret_t int_phy_ext_reg_read(yt_unit_t unit, uint8_t phy_addr, uint16_t regAddr, uint16_t *pData)
{
    if(HALSWDRV(unit) == NULL || pData == NULL)
    {
        return CMM_ERR_FAIL;
    }

    HALSWDRV_FUNC(unit)->switch_intif_write(unit, phy_addr, 0x1e, regAddr);
    HALSWDRV_FUNC(unit)->switch_intif_read(unit, phy_addr, 0x1f, pData);

    return CMM_ERR_OK;
}

yt_ret_t int_phy_ext_reg_write(yt_unit_t unit, uint8_t phy_addr, uint16_t regAddr, uint16_t data)
{
    if(HALSWDRV(unit) == NULL)
    {
        return CMM_ERR_FAIL;
    }

    HALSWDRV_FUNC(unit)->switch_intif_write(unit, phy_addr, 0x1e, regAddr);
    HALSWDRV_FUNC(unit)->switch_intif_write(unit, phy_addr, 0x1f, data);

    return CMM_ERR_OK;
}

yt_ret_t ext_phy_mmd_reg_read(yt_unit_t unit, uint8_t phy_addr, uint8_t mmd_id, uint16_t regAddr, uint16_t *pData)
{
    if(HALSWDRV(unit) == NULL || pData == NULL)
    {
        return CMM_ERR_FAIL;
    }

    HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, 0xd, mmd_id);
    HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, 0xe, regAddr);
    HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, 0xd, 0x4000+mmd_id);
    HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, 0xe, pData);

    return CMM_ERR_OK;
}

yt_ret_t ext_phy_mmd_reg_write(yt_unit_t unit, uint8_t phy_addr, uint8_t mmd_id, uint16_t regAddr, uint16_t data)
{
    if(HALSWDRV(unit) == NULL)
    {
        return CMM_ERR_FAIL;
    }

    HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, 0xd, mmd_id);
    HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, 0xe, regAddr);
    HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, 0xd, 0x4000+mmd_id);
    HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, 0xe, data);

    return CMM_ERR_OK;
}

yt_ret_t ext_phy_ext_reg_read(yt_unit_t unit, uint8_t phy_addr, uint16_t regAddr, uint16_t *pData)
{
    if(HALSWDRV(unit) == NULL || pData == NULL)
    {
        return CMM_ERR_FAIL;
    }

    HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, 0x1e, regAddr);
    HALSWDRV_FUNC(unit)->switch_extif_read(unit, phy_addr, 0x1f, pData);

    return CMM_ERR_OK;
}

yt_ret_t ext_phy_ext_reg_write(yt_unit_t unit, uint8_t phy_addr, uint16_t regAddr, uint16_t data)
{
    if(HALSWDRV(unit) == NULL)
    {
        return CMM_ERR_FAIL;
    }

    HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, 0x1e, regAddr);
    HALSWDRV_FUNC(unit)->switch_extif_write(unit, phy_addr, 0x1f, data);

    return CMM_ERR_OK;
}
