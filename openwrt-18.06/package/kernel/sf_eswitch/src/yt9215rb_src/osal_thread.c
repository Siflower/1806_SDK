/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "osal_thread.h"
#include "yt_error.h"
#include "yt_util.h"

osal_thread_t osal_thread_create(char *pName, uint32_t thread_prio, uint32_t stack_size, void (*thread_handler)(void *), void *pArg)
{
    osal_thread_t  id;
    osal_pthread_attr_t attr;

    CMM_UNUSED_PARAM(pName);
    CMM_UNUSED_PARAM(thread_prio);

    if(0 != osal_pthread_attr_init(&attr))
    {
        return 0;
    }

    osal_pthread_attr_setstacksize(&attr, stack_size);

    if(0 != osal_pthread_create(&id, NULL, (void *)thread_handler, (void *)pArg))
    {
        return 0;
    }

    /* TODO: relate thread id and name into threadlist,for debug */
    return id;
}

yt_ret_t osal_thread_del(osal_thread_t thread_id)
{
    /* TODO: remove thread info from threadlist by thread id */
    if(osal_thread_self() == thread_id)
    {
        osal_thread_detach(thread_id);
        osal_thread_exit(NULL);
    }
    
    return CMM_ERR_OK;
}

yt_ret_t osal_thread_delay(uint32_t usec)
{
#if 0
    uint32_t sec;
    struct timespec timeout;

    /*TODO: ajust ns according to specific system */
    sec = nsec /10000000;
    timeout.tv_sec = sec;
    timeout.tv_nsec = ((nsec - sec * 10000000) * 1000);

    nanosleep(&timeout, NULL);
#else
    usleep(usec);
#endif
    return CMM_ERR_OK;
}