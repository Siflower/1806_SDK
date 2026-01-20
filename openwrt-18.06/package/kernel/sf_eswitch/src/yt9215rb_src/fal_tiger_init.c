/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

/*
 * Include Files
 */
#include "fal_cmm.h"
#include "fal_tiger_init.h"
#include "fal_tiger_port.h"
#include "fal_tiger_led.h"
#include "osal_print.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"
#include "fal_tiger_qos.h"
#include "fal_tiger_sys.h"
#include "phy_drv.h"
#include "yt_util.h"
#include "sw_yt921x.h"

static yt_ret_t fal_tiger_patch_init(yt_unit_t unit);

yt_ret_t fal_tiger_init(yt_unit_t unit)
{
    fal_tiger_patch_init(unit);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_9218_init(yt_unit_t unit)
{
    fal_tiger_init(unit);

    return CMM_ERR_OK;
}

static yt_ret_t fal_tiger_patch_qos_init(yt_unit_t unit)
{
    uint32_t port;
    uint32_t qid;
    uint32_t regAddr;
    uint32_t regData;
    uint32_t regData2;
    int32_t ret = 0;
    uint32_t i;

    for (port = 0; port < FAL_MAX_PORT_NUM; port++)
    {
        for (qid = 0; qid < CAL_MAX_UCAST_QUEUE_NUM(unit); qid++)
        {
            regAddr = QOS_FORCEAC_UCASTQUE_REG(unit, port, qid);
            CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, regAddr, &regData), ret);
            CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, regAddr + 4, &regData2), ret);
            regData &= ~(0xff);
            regData |= 0x20;
            CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, regAddr, regData), ret);
            CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, regAddr + 4, regData2), ret);
        }

        for (qid = 0; qid < CAL_MAX_MCAST_QUEUE_NUM(unit); qid++)
        {
            regAddr = QOS_FORCEAC_MCASTQUE_REG(unit, port, qid);
            CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, regAddr, &regData), ret);
            regData &= ~(0xff);
            regData |= 0xc;
            CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, regAddr, regData), ret);
        }
    }

    for (i = 0; i < QOS_GROUPAC_MAX_NUM; i++)
    {
        regAddr = QOS_GROUPAC_REG(i);
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, regAddr, &regData), ret);
        regData &= ~(1 << 8);
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, regAddr, regData), ret);
    }

    return CMM_ERR_OK;
}

static yt_ret_t fal_tiger_patch_init(yt_unit_t unit)
{
    uint8_t i;
    uint32_t regData;
    uint32_t regData2;
    cmm_err_t ret = CMM_ERR_OK;
    l2_learn_per_port_ctrln_t l2_learn_per_port_ctrl;
    global_ctrl1_t gEntry;
    uint32_t ceiling = 0xdc;
    uint32_t regAddr;

    uint8_t intTuneVal[4] = {0xE1, 0xE2, 0xE3, 0xE4};

    /* flow control */
    for(i = 0; i < FAL_MAX_PORT_NUM; i++)
    {
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, 0x281000+i*8, &regData), ret);
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, 0x281004+i*8, &regData2), ret);
        regData &= ~(0x1ff | (0x7 << 29));
        regData2 &= ~(0xff);
        regData |= ((0x4b & 0x1ff) | ((ceiling & 0x7) << 29));
        regData2 |= ((ceiling >> 3) & 0xff);
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x281000+i*8, regData), ret);
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x281004+i*8, regData2), ret);
    }
    for(i = 0; i < 4; i++)
    {
        if (CAL_SWCHIP_ID(unit) == YT_SW_ID_9215)
        {
            CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, 0x2801D0+i*4, &regData), ret);
            regData &= ~(0x3ff);
            regData |= (0x14a & 0x3ff);
            CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x2801D0+i*4, regData), ret);
        }
        HAL_MEM_DIRECT_WRITE(unit, 0x180904+i*4, intTuneVal[i]);
    }

    if ((CAL_BOARD_ID == BOARD_ID_YT9215RB_YT8531) || (CAL_BOARD_ID == BOARD_ID_YT9215S_YT8531)
            || (CAL_BOARD_ID == BOARD_ID_YT9218MB_YT8531))
    {
        /*select mdio grp1 pin*/
        HAL_MEM_DIRECT_READ(unit, 0x80358, &regData);
        regData |= (1 << 2);
        HAL_MEM_DIRECT_WRITE(unit, 0x80358, regData);
    }

    regAddr = EXT_IF_ACCESS_ADDR_CTRL;
    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, regAddr , &regData), ret);
    regData &= ~(0xfU << 8U);
    regData |= (EXT_IF_FREQUENCY_SEL << 8U);
    CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, regAddr, regData), ret);

    /*disable internal cpu port learn fdb*/
    HAL_TBL_REG_READ(unit, L2_LEARN_PER_PORT_CTRLNm, FAL_INTERNAL_CPU_MACID, sizeof(l2_learn_per_port_ctrl), &l2_learn_per_port_ctrl);
    HAL_FIELD_SET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_LEARN_DISABLEf, &l2_learn_per_port_ctrl, YT_ENABLE);
    HAL_TBL_REG_WRITE(unit, L2_LEARN_PER_PORT_CTRLNm, FAL_INTERNAL_CPU_MACID, sizeof(l2_learn_per_port_ctrl), &l2_learn_per_port_ctrl);

    HAL_MEM_DIRECT_READ(unit, CHIP_CHIP_ID_REG, &regData);
    HAL_TBL_REG_READ(unit, GLOBAL_CTRL1m, 0, sizeof(global_ctrl1_t), &gEntry);
    HAL_FIELD_SET(GLOBAL_CTRL1m, GLOBAL_CTRL1_ACL_ENf, &gEntry, YT_ENABLE);
    if (0x1 == (regData&0x0ffff))
    {
        HAL_FIELD_SET(GLOBAL_CTRL1m, GLOBAL_CTRL1_AC_ENf, &gEntry, YT_ENABLE);
    }
    HAL_TBL_REG_WRITE(unit, GLOBAL_CTRL1m, 0, sizeof(global_ctrl1_t), &gEntry);

    /*
    * disable sds intf,will be enabled when config sds mode
    * set to mdio access mode
    */
    HAL_MEM_DIRECT_READ(unit, CHIP_INTERFACE_CTRL_REG, &regData);
    regData &= ~(0x43<<0);
    HAL_MEM_DIRECT_WRITE(unit, CHIP_INTERFACE_CTRL_REG, regData);

    /*
    * adjust dying gasp for YT_SW_ID_9218
    */
     if (CAL_SWCHIP_ID(unit) == YT_SW_ID_9218)
     {
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, CHIP_TOPBIAS_REG, &regData), ret);
        regData &= ~(0x3<<22);
        regData |= (0x3<<22);
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, CHIP_TOPBIAS_REG, regData), ret);
     }

    /* fix the issue 14437 */
    for(i = 0; i < (FAL_MAX_PORT_NUM - 1); i++)
    {
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, CHIP_GMAC_CTRL1(i), &regData), ret);
        regData &= ~(0xFF<<20);
        regData |= (0x20<<20);
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, CHIP_GMAC_CTRL1(i), regData), ret);
    }

    fal_tiger_patch_qos_init(unit);

    return CMM_ERR_OK;
}
