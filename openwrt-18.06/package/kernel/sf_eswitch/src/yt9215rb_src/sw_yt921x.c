/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#include "sw_yt921x.h"
#include "hal_mem.h"
#include "yt_util.h"
#include "yt_lock.h"

const yt_switch_drv_func_t yt9215_drv_func =
{
    .switch_init = yt9215_init,
    .switch_mac_config = yt9215_mac_config,
    .switch_led_config = yt9215_led_config,
    .switch_intif_read = yt9215_intif_read,
    .switch_intif_write = yt9215_intif_write,
    .switch_extif_read = yt9215_extif_read,
    .switch_extif_write = yt9215_extif_write,
    .switch_extif_polling_config = yt9215_extif_polling_config,
    .switch_edata_read = yt9215_edata_read,
};

const yt_switch_drv_t yt9215_drv =
{
    .chip_id = YT_SW_ID_9215,
    /*.chip_model = YT_SW_MODEL_9215,*/
    .pDrvFunc = &yt9215_drv_func
};

const yt_switch_drv_t yt9218_drv =
{
    .chip_id = YT_SW_ID_9218,
    /*.chip_model = YT_SW_MODEL_9218,*/
    .pDrvFunc = &yt9215_drv_func
};

yt_ret_t yt9215_init(yt_unit_t unit)
{
    CMM_UNUSED_PARAM(unit);
    return CMM_ERR_OK;
}

yt_ret_t yt9215_mac_config(yt_unit_t unit)
{
    CMM_UNUSED_PARAM(unit);
    return CMM_ERR_OK;
}

yt_ret_t yt9215_led_config(yt_unit_t unit)
{
    CMM_UNUSED_PARAM(unit);
    return CMM_ERR_OK;
}

yt_ret_t yt9215_intif_read(yt_unit_t unit, uint8_t intif_addr, uint32_t regAddr, uint16_t *pData)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t base_data;
    uint32_t op_data = 0;
    uint32_t wait_count = 0;

    INT_SMI_LOCK();

    CMM_ERR_UNLOCK(HAL_MEM_DIRECT_READ(unit, INT_IF_ACCESS_ADDR_CTRL, &base_data), YT_INT_SMI_LOCK_ID, ret);
    base_data &= 0xFC00FFF1;
    base_data |= ((intif_addr&0x1f) << 21 | (regAddr&0x1f) << 16 | 2 << 2);
    CMM_ERR_UNLOCK(HAL_MEM_DIRECT_WRITE(unit, INT_IF_ACCESS_ADDR_CTRL, base_data), YT_INT_SMI_LOCK_ID, ret);

    op_data = 1;
    CMM_ERR_UNLOCK(HAL_MEM_DIRECT_WRITE(unit, INT_IF_ACCESS_FRAME_CTRL, op_data), YT_INT_SMI_LOCK_ID, ret);

    while(MAX_BUSYING_WAIT_TIME > wait_count)
    {
        CMM_ERR_UNLOCK(HAL_MEM_DIRECT_READ(unit, INT_IF_ACCESS_FRAME_CTRL, &op_data), YT_INT_SMI_LOCK_ID, ret);

        if(!op_data)
        {
            CMM_ERR_UNLOCK(HAL_MEM_DIRECT_READ(unit, INT_IF_ACCESS_DATA_1_ADDR, &base_data), YT_INT_SMI_LOCK_ID, ret);
            *pData = base_data;
            INT_SMI_UNLOCK();
            return CMM_ERR_OK;
        }
        wait_count ++;
    }

    INT_SMI_UNLOCK();
    return CMM_ERR_BUSYING_TIME;
}

yt_ret_t yt9215_intif_write(yt_unit_t unit, uint8_t intif_addr, uint32_t regAddr, uint16_t data)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t base_data;
    uint32_t op_data = 0;
    uint32_t wait_count = 0;

    INT_SMI_LOCK();

    CMM_ERR_UNLOCK(HAL_MEM_DIRECT_READ(unit, INT_IF_ACCESS_ADDR_CTRL, &base_data), YT_INT_SMI_LOCK_ID, ret);
    base_data &= 0xFC00FFF1;
    base_data |= ((intif_addr&0x1f) << 21 | (regAddr&0x1f) << 16 | 1 << 2);
    CMM_ERR_UNLOCK(HAL_MEM_DIRECT_WRITE(unit, INT_IF_ACCESS_ADDR_CTRL, base_data), YT_INT_SMI_LOCK_ID, ret);
    CMM_ERR_UNLOCK(HAL_MEM_DIRECT_WRITE(unit, INT_IF_ACCESS_DATA_0_ADDR, data), YT_INT_SMI_LOCK_ID, ret);

    op_data = 1;
    CMM_ERR_UNLOCK(HAL_MEM_DIRECT_WRITE(unit, INT_IF_ACCESS_FRAME_CTRL, op_data), YT_INT_SMI_LOCK_ID, ret);

    while(MAX_BUSYING_WAIT_TIME > wait_count)
    {
        CMM_ERR_UNLOCK(HAL_MEM_DIRECT_READ(unit, INT_IF_ACCESS_FRAME_CTRL, &op_data), YT_INT_SMI_LOCK_ID, ret);

        if(!op_data)
        {
            INT_SMI_UNLOCK();
            return CMM_ERR_OK;
        }

        wait_count ++;
    }

    INT_SMI_UNLOCK();
    return CMM_ERR_BUSYING_TIME;
}

yt_ret_t yt9215_extif_read(yt_unit_t unit, uint8_t extif_addr, uint32_t regAddr, uint16_t *pData)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t base_data;
    uint32_t op_data = 0;
    uint32_t wait_count = 0;

    EXT_SMI_LOCK();

    CMM_ERR_UNLOCK(HAL_MEM_DIRECT_READ(unit, EXT_IF_ACCESS_ADDR_CTRL, &base_data), YT_EXT_SMI_LOCK_ID, ret);
    base_data &= 0xFC00FFF1;/*clause 22*/
    base_data |= ((extif_addr&0x1f) << 21 | (regAddr&0x1f) << 16 | 2 << 2);
    CMM_ERR_UNLOCK(HAL_MEM_DIRECT_WRITE(unit, EXT_IF_ACCESS_ADDR_CTRL, base_data), YT_EXT_SMI_LOCK_ID, ret);

    op_data = 1;
    CMM_ERR_UNLOCK(HAL_MEM_DIRECT_WRITE(unit, EXT_IF_ACCESS_FRAME_CTRL, op_data), YT_EXT_SMI_LOCK_ID, ret);

    while(MAX_BUSYING_WAIT_TIME > wait_count)
    {
        CMM_ERR_UNLOCK(HAL_MEM_DIRECT_READ(unit, EXT_IF_ACCESS_FRAME_CTRL, &op_data), YT_EXT_SMI_LOCK_ID, ret);

        if(!op_data)
        {
            CMM_ERR_UNLOCK(HAL_MEM_DIRECT_READ(unit, EXT_IF_ACCESS_DATA_1_ADDR, &base_data), YT_EXT_SMI_LOCK_ID, ret);
            *pData = base_data;
            EXT_SMI_UNLOCK();
            return CMM_ERR_OK;
        }
        wait_count ++;
    }

    EXT_SMI_UNLOCK();
    return CMM_ERR_BUSYING_TIME;
}

yt_ret_t yt9215_extif_write(yt_unit_t unit, uint8_t extif_addr, uint32_t regAddr, uint16_t data)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t base_data;
    uint32_t op_data = 0;
    uint32_t wait_count = 0;

    EXT_SMI_LOCK();

    CMM_ERR_UNLOCK(HAL_MEM_DIRECT_READ(unit, EXT_IF_ACCESS_ADDR_CTRL, &base_data), YT_EXT_SMI_LOCK_ID, ret);
    base_data &= 0xFC00FFF1;/*clause 22*/
    base_data |= ((extif_addr&0x1f) << 21 | (regAddr&0x1f) << 16 | 1 << 2);
    CMM_ERR_UNLOCK(HAL_MEM_DIRECT_WRITE(unit, EXT_IF_ACCESS_ADDR_CTRL, base_data), YT_EXT_SMI_LOCK_ID, ret);
    CMM_ERR_UNLOCK(HAL_MEM_DIRECT_WRITE(unit, EXT_IF_ACCESS_DATA_0_ADDR, data), YT_EXT_SMI_LOCK_ID, ret);

    op_data = 1;
    CMM_ERR_UNLOCK(HAL_MEM_DIRECT_WRITE(unit, EXT_IF_ACCESS_FRAME_CTRL, op_data), YT_EXT_SMI_LOCK_ID, ret);

    while(MAX_BUSYING_WAIT_TIME > wait_count)
    {
        CMM_ERR_UNLOCK(HAL_MEM_DIRECT_READ(unit, EXT_IF_ACCESS_FRAME_CTRL, &op_data), YT_EXT_SMI_LOCK_ID, ret);

        if(!op_data)
        {
            EXT_SMI_UNLOCK();
            return CMM_ERR_OK;
        }

        wait_count ++;
    }

    EXT_SMI_UNLOCK();
    return CMM_ERR_BUSYING_TIME;
}

yt_ret_t yt9215_extif_polling_config(yt_unit_t unit)
{
    CMM_UNUSED_PARAM(unit);
    return CMM_ERR_OK;
}

yt_ret_t yt9215_edata_read(yt_unit_t unit, uint16_t addr, uint8_t *pData)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t op_data = 0;
    uint32_t wait_count = 0;

    op_data = 0x05 | ((addr << 8) & 0xFF00);
    CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, E_OP_CTRL_0, op_data), ret);
    while(MAX_BUSYING_WAIT_TIME > wait_count)
    {
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, E_OP_CTRL_1, &op_data), ret);

        if( (op_data & 0x0F) == 0x03)
        {
            *pData = (op_data >> 24) & 0xFF;
            return CMM_ERR_OK;
        }
        wait_count++;
    }

    return CMM_ERR_BUSYING_TIME;
}
