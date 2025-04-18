#ifndef __OSAL_SEMAPHORE_H__
#define __OSAL_SEMAPHORE_H__
/*
 * Include Files
 */
#if defined(OS_CYGWIN) || defined(OS_LINUX_USR)
#include <pthread.h>
#elif defined (__KERNEL__)
#include <linux/mutex.h>
#else
#include "FreeRTOS.h"
#include "semphr.h"
#include "yt_error.h"
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
#else
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
#endif


/*
 * Data Type Declaration
 */

/*
 * Function Declaration
 */

#endif
