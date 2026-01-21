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
#include "yt_error.h"
#include "yt_types.h"
#include "yt_init.h"
#include "yt_vlan.h"
#include "cal_mgm.h"
#include "fal_dispatch.h"
#include "hal_cmm.h"
#include "drv.h"
#include "fal_init.h"
#ifdef OSAL_INCLUDED
#include "osal_pthread.h"
#endif
#ifdef YT_PKTGEN_EN
#include "yt_pkt_gen.h"
#endif
#ifdef ACC_UART
#include "uart.h"
#endif

yt_ret_t  yt_basic_init(void)
{
    cmm_err_t ret = CMM_ERR_OK;
    CMM_ERR_CHK(cal_mgm_init(), ret);
    CMM_ERR_CHK(hal_init(), ret);
    CMM_ERR_CHK(fal_dispatch_init(), ret);
    CMM_ERR_CHK(yt_drv_init(), ret);
#ifdef OSAL_INCLUDED
    CMM_ERR_CHK(osal_posix_pthread_init(), ret);
#endif
    phy_smi_init();

    return CMM_ERR_OK;
}

yt_ret_t  yt_device_init(yt_unit_t unit, uint8_t *dev)
{
    cmm_err_t ret = CMM_ERR_OK;

    CMM_UNUSED_PARAM(unit);
#ifdef ACC_UART
    CMM_ERR_CHK(uart_init(dev), ret);
#else
    CMM_UNUSED_PARAM(ret);
    CMM_UNUSED_PARAM(dev);
#endif

    return CMM_ERR_OK;
}

yt_ret_t  yt_device_close(yt_unit_t unit)
{
    cmm_err_t ret = CMM_ERR_OK;
    CMM_UNUSED_PARAM(unit);
    CMM_ERR_CHK(drv_close(),ret);
    return CMM_ERR_OK;
}

yt_ret_t yt_modules_init(void)
{
    yt_unit_t unit;
    cmm_err_t ret = CMM_ERR_OK;
    
    for(unit = 0; unit < YT_UNIT_NUM; unit++)
    {
#ifdef L2_INCLUDED
        CMM_ERR_CHK(yt_l2_init(unit),ret);
#endif
#ifdef PORT_INCLUDED
        CMM_ERR_CHK(yt_port_init(unit),ret);
#endif
#ifdef VLAN_INCLUDED
        CMM_ERR_CHK(yt_vlan_init(unit),ret);
#endif
#ifdef QOS_INCLUDED
        CMM_ERR_CHK(yt_qos_init(unit), ret);
#endif
#ifdef RATE_INCLUDED
        CMM_ERR_CHK(yt_rate_init(unit),ret);
#endif
#ifdef STORM_CTRL_INCLUDED
        CMM_ERR_CHK(yt_storm_ctrl_init(unit),ret);
#endif
#ifdef ACL_INCLUDED
        CMM_ERR_CHK(yt_acl_init(unit),ret);
#endif
#ifdef DOT1X_INCLUDED
        CMM_ERR_CHK(yt_dot1x_init(unit),ret);
#endif
#ifdef STAT_INCLUDED
        CMM_ERR_CHK(yt_stat_mib_init(unit),ret);
#endif
#ifdef NIC_INCLUDED
        CMM_ERR_CHK(yt_nic_init(unit),ret);
#endif
#ifdef DOS_INCLUDED
        CMM_ERR_CHK(yt_dos_init(unit),ret);
#endif
#ifdef PORT_MIRROR_INCLUDED
        CMM_ERR_CHK(yt_mirror_init(unit), ret);
#endif
#ifdef LAG_INCLUDED
        CMM_ERR_CHK(yt_lag_init(unit), ret);
#endif
#ifdef IGMP_MLD_INCLUDED
#ifdef SWITCH_SERIES_WHALE
        CMM_ERR_CHK(yt_igmp_mld_init(unit),ret);
#endif
#endif
    }

    return CMM_ERR_OK;
}

yt_ret_t  yt_phy_drv_load(void)
{
    yt_ret_t ret;

    CMM_ERR_CHK(phy_drv_load(), ret);

    return CMM_ERR_OK;
}

yt_ret_t  yt_port_default_cfg_init(void)
{
    yt_unit_t unit;
    cmm_err_t ret = CMM_ERR_OK;

    for(unit = 0; unit < YT_UNIT_NUM; unit++)
    {
        CMM_ERR_CHK(yt_port_default_cfg_set(unit), ret);
    }

    return CMM_ERR_OK;
}

yt_ret_t  yt_patch_init(void)
{
    yt_ret_t ret;

    CMM_ERR_CHK(fal_init(), ret);

    return CMM_ERR_OK;
}

yt_ret_t  yt_init(void)
{
    cmm_err_t ret = CMM_ERR_OK;
    
    CMM_ERR_CHK(yt_basic_init(),ret);
    CMM_ERR_CHK(yt_patch_init(), ret);
    CMM_ERR_CHK(yt_phy_drv_load(), ret);
    CMM_ERR_CHK(yt_port_default_cfg_init(), ret);
    CMM_ERR_CHK(yt_modules_init(), ret);

    return CMM_ERR_OK;
}

yt_ret_t  yt_drv_init(void)
{
    yt_unit_t unit;
    cmm_err_t ret = CMM_ERR_OK;

    for(unit = 0; unit < YT_UNIT_NUM; unit++)
    {
        CMM_ERR_CHK(drv_init(unit), ret);
    }

    return CMM_ERR_OK;
}

#if defined(LINUX_KERNEL_MODE)
EXPORT_SYMBOL(yt_init);
#endif

