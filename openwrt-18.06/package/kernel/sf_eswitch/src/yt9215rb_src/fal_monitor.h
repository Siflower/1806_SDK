/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#ifndef __FAL_MONITOR_H__
#define __FAL_MONITOR_H__

#include "yt_types.h"
#ifdef OSAL_INCLUDED
#include "osal_pthread.h"
#endif

typedef yt_ret_t (*yt_port_mac_sync_phy_t)(yt_unit_t, yt_port_t);
typedef yt_ret_t (*yt_led_softPoll_sync_phy_t)(yt_unit_t, yt_port_t);

#define DEFAULT_POLLING_INTERVAL    500

/*
    polling port status;
    sync mac with phy;
*/
typedef struct yt_fal_monitor_s
{
    yt_bool_t   polling_thread_en;
    uint32_t    polling_task_id;
    uint32_t    polling_interval;/*ms*/
    yt_port_mask_t  port_polling_portmask[YT_MAX_UNIT];
    yt_port_mask_t  led_polling_portmask[YT_MAX_UNIT];
    yt_port_mac_sync_phy_t mac_sync_phy_func;
    yt_led_softPoll_sync_phy_t led_softPoll_sync_phy_func;               
    /*TODO:status change callback*/
}yt_fal_monitor_t;

extern yt_ret_t fal_monitor_init(void);
extern yt_ret_t fal_monitor_start(void);
extern yt_ret_t fal_monitor_stop(void);
extern yt_ret_t fal_monitor_polling_port_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);
extern yt_ret_t fal_monitor_polling_port_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);
extern yt_ret_t fal_monitor_led_port_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);
extern yt_ret_t fal_monitor_led_port_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

#endif
