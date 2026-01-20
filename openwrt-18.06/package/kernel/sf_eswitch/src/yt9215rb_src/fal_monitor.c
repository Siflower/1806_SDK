/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "yt_error.h"
#include "yt_cmm.h"
#include "fal_monitor.h"

#ifdef SWITCH_SERIES_TIGER
#include "fal_tiger_port.h"
#endif

yt_fal_monitor_t gFal_monitor =
{
    .polling_thread_en = YT_DISABLE,
    .polling_thread_id = 0,
    .polling_interval = 0,
#ifdef SWITCH_SERIES_TIGER
    .mac_sync_phy_func = fal_tiger_port_mac_sync_phy,
#else
    .mac_sync_phy_func = NULL
#endif
};

static void fal_monitor_polling_thread(void);

yt_ret_t fal_monitor_init(void)
{
    gFal_monitor.polling_interval = DEFAULT_POLLING_INTERVAL;

    if(gFal_monitor.polling_thread_id == 0)
    {
        gFal_monitor.polling_thread_id = osal_thread_create("YT Monitor", 0, DEFAULT_STACK_SIZE, (void *)fal_monitor_polling_thread, NULL);

        if(gFal_monitor.polling_thread_id == 0)
        {
            return CMM_ERR_FAIL;
        }
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_monitor_start(void)
{
    gFal_monitor.polling_thread_en = YT_ENABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_monitor_stop(void)
{
    gFal_monitor.polling_thread_en = YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_monitor_polling_port_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    if(YT_ENABLE == enable)
    {
        CMM_SET_MEMBER_PORT(gFal_monitor.polling_portmask[unit], port);
    }
    else
    {
        CMM_RESET_MEMBER_PORT(gFal_monitor.polling_portmask[unit], port);
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_monitor_polling_port_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_IS_MEMBER_PORT(gFal_monitor.polling_portmask[unit], port, *pEnable);

    return CMM_ERR_OK;
}

static void fal_monitor_polling_thread(void)
{
    yt_unit_t unit;
    yt_port_t port;
    yt_bool_t isEnable = FALSE;

    while(1)
    {
        osal_thread_delay(1000*gFal_monitor.polling_interval);

        if(!gFal_monitor.polling_thread_en)
        {
            continue;
        }

        if(gFal_monitor.mac_sync_phy_func == NULL)
        {
            continue;
        }
        for(unit = 0; unit < CAL_MAX_UNIT_NUM; unit++)
        {
            for(port = 0; port < CAL_PORT_NUM_ON_UNIT(unit); port++)
            {
                CMM_IS_MEMBER_PORT(gFal_monitor.polling_portmask[unit], port, isEnable);
                if(isEnable)
                {
                    gFal_monitor.mac_sync_phy_func(unit, port);
                }
            }
        }
    }
}

