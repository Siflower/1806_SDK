#ifndef __OSAL_SEMAPHORE_H__
#define __OSAL_SEMAPHORE_H__
/*
 * Include Files
 */
#if defined(OS_CYGWIN) || defined(OS_LINUX_USR)
#include <pthread.h>
#elif defined (__KERNEL__)
#include <linux/mutex.h>
#elif defined (OS_FREERTOS)
#include "FreeRTOS.h"
#include "semphr.h"
#include "yt_error.h"
#elif defined (OS_LITEOS)
#include "los_mux.h"
#include "los_compiler.h"
#include "los_task.h"
#else
#endif

/*
 * Symbol Definition
 */

/*
 * Macro Definition
 */
#if defined(OS_CYGWIN) || defined(OS_LINUX_USR)
#define osal_mux			pthread_mutex_t
#define osal_mux_init		pthread_mutex_init
#define osal_mux_destroy	pthread_mutex_destroy
#define osal_mux_lock		pthread_mutex_lock
#define osal_mux_unlock		pthread_mutex_unlock
#elif defined(__KERNEL__)
#define osal_mux                struct mutex
#define osal_mux_init(a, b)     mutex_init((a))
#define osal_mux_destroy        
#define osal_mux_lock		    mutex_lock
#define osal_mux_unlock		    mutex_unlock
#elif defined (OS_FREERTOS)
#define osal_mux            SemaphoreHandle_t
#define osal_mux_init(pMuxHandle, attr) \
    do \
    { \
        if(NULL == (osal_mux *)(pMuxHandle)) \
        { \
            return CMM_ERR_NULL_POINT; \
        } \
        *(osal_mux *)(pMuxHandle) = xSemaphoreCreateMutex(); \
        if (NULL == *(osal_mux *)(pMuxHandle)) \
        { \
            return CMM_ERR_FAIL; \
        } \
    }while(0)
    
#define osal_mux_destroy(pMuxHandle) \
    do \
    { \
        if((NULL == (osal_mux *)(pMuxHandle)) || (NULL == *(osal_mux *)(pMuxHandle))) \
        { \
            return CMM_ERR_NULL_POINT; \
        } \
        vSemaphoreDelete(*(osal_mux *)(pMuxHandle)); \
    }while(0)
    
#define osal_mux_lock(pMuxHandle) \
    do \
    { \
        if((NULL == (osal_mux *)(pMuxHandle)) || (NULL == *(osal_mux *)(pMuxHandle))) \
        { \
            return CMM_ERR_NULL_POINT; \
        } \
        xSemaphoreTake(*(osal_mux *)(pMuxHandle), portMAX_DELAY); \
    }while(0)
    
#define osal_mux_unlock(pMuxHandle) \
    do \
    { \
        if((NULL == (osal_mux *)(pMuxHandle)) || (NULL == *(osal_mux *)(pMuxHandle))) \
        { \
            return CMM_ERR_NULL_POINT; \
        } \
        xSemaphoreGive(*(osal_mux *)(pMuxHandle)); \
    }while(0)
#elif defined (OS_LITEOS)
#define osal_mux            UINT32
#define osal_mux_init(pMuxHandle, attr) \
    do \
    { \
        if(NULL == (UINT32 *)(pMuxHandle)) \
        { \
            return CMM_ERR_NULL_POINT; \
        } \
        UINT32 ret = LOS_MuxCreate((UINT32 *)pMuxHandle); \
        if (LOS_OK != ret) \
        { \
            return CMM_ERR_FAIL; \
        } \
    }while(0)
    
#define osal_mux_destroy(pMuxHandle) \
    do \
    { \
        if(NULL == (UINT32 *)(pMuxHandle))\
        { \
            return CMM_ERR_NULL_POINT; \
        } \
        UINT32 ret = LOS_MuxDelete(*(UINT32 *)(pMuxHandle)); \
        if (LOS_OK != ret) \
        { \
            return CMM_ERR_FAIL; \
        } \
    }while(0)
    
#define osal_mux_lock(pMuxHandle) \
    do \
    { \
        if(NULL == (UINT32 *)(pMuxHandle)) \
        { \
            return CMM_ERR_NULL_POINT; \
        } \
        UINT32 ret = LOS_MuxPend(*(UINT32 *)(pMuxHandle), LOS_WAIT_FOREVER); \
        if (LOS_OK != ret) \
        { \
            return CMM_ERR_FAIL; \
        } \
    }while(0)
    
#define osal_mux_unlock(pMuxHandle) \
    do \
    { \
        if(NULL == (UINT32 *)(pMuxHandle)) \
        { \
            return CMM_ERR_NULL_POINT; \
        } \
        UINT32 ret = LOS_MuxPost(*(UINT32 *)(pMuxHandle)); \
        if (LOS_OK != ret) \
        { \
            return CMM_ERR_FAIL; \
        } \
    }while(0)
#else
#define osal_mux                int
#define osal_mux_init(a, b)     (0)
#define osal_mux_destroy
#define osal_mux_lock
#define osal_mux_unlock
#endif


/*
 * Data Type Declaration
 */

/*
 * Function Declaration
 */

#endif

