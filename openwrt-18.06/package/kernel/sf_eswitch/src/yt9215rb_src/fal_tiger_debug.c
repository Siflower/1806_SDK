/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_debug.c
*
* @brief x
*
********************************************************************************
*/
/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_print.h"
#include "fal_tiger_debug.h"
#include "fal_tiger_port.h"
#include "hal_mem.h"
#include "hal_ctrl.h"

#if defined(DEBUG_INCLUDED) && defined(PORT_INCLUDED)
yt_ret_t fal_tiger_debug_phyTemplate_test_set(yt_unit_t unit, yt_port_t port,  yt_utp_template_testmode_t mode)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t regData;
	yt_phy_comm_cfg_t cfg;

    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, 0x80028, &regData), ret);
    if (INVALID_ID != CAL_YTP_TO_EXTPORT(unit, port))/*for sds test*/
    {
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x80028, (1<<(2+port-5)) | 0x3), ret);
        /*disable serdes interface*/
        regData &= 0x7c;
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x80028, regData), ret);
        if(mode == YT_UTP_TEMPLATE_TMODE_SDS2500M)
        {
            fal_tiger_port_extif_mode_set(unit, port, YT_EXTIF_MODE_BX2500);
        }
        else if(mode == YT_UTP_TEMPLATE_TMODE_SDS1000M)
        {
            fal_tiger_port_extif_mode_set(unit, port, YT_EXTIF_MODE_FIB_1000);
        }
    }
    else /*internal phy port*/
    {
        /*disable serdes interface*/
        regData &= 0x7c;
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x80028, regData), ret);
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x8002c, (1<<(16+port)) | 0xff), ret);
    }

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        CMM_ERR_CHK(yt_phy_test_template(cfg, mode), ret);
        return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}
#endif

yt_ret_t fal_tiger_debug_phyLoopback_test_set(yt_unit_t unit, yt_port_t port, yt_phy_chip_mode_t phyMode, yt_phy_loopback_mode_t mode)
{
    cmm_err_t ret = CMM_ERR_OK;
	yt_phy_comm_cfg_t cfg;

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        CMM_ERR_CHK(yt_phy_loopback_set(cfg, phyMode, mode), ret);
        return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_debug_utp_snr_get(yt_unit_t unit, yt_port_t port, yt_utp_snr_t *pSnrValue)
{
    yt_ret_t ret = CMM_ERR_OK;
    yt_port_linkStatus_all_t allLinkStatus;
    yt_phy_comm_cfg_t cfg;

    osal_memset(&allLinkStatus, sizeof(allLinkStatus), 0, sizeof(allLinkStatus));
    CMM_ERR_CHK(fal_tiger_port_link_status_all_get(unit, port, &allLinkStatus), ret);
    if (allLinkStatus.link_speed != PORT_SPEED_1000M
        && allLinkStatus.link_speed != PORT_SPEED_100M)
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    cfg.unit = unit;
    ret = phy_addr_smi_type_get(unit, port, &cfg.phyAddr, &cfg.smiType);
    if (ret == CMM_ERR_OK)
    {
        yt_phy_utp_snr_get(cfg, allLinkStatus.link_speed, pSnrValue);
        return CMM_ERR_OK;
    }

    return CMM_ERR_FAIL;
}
