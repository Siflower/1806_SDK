#ifndef __OSAL_PTHREAD_H__
#define __OSAL_PTHREAD_H__

/*
 * Include Files
 */
#if defined(OS_CYGWIN) || defined(OS_LINUX_USR)
#include <pthread.h>
#include <sys/unistd.h>
#elif defined (__KERNEL__)
#include <linux/mutex.h>
#endif
#include "yt_types.h"

/*
 * Macro Definition
 */
#if defined(OS_CYGWIN) || defined(OS_LINUX_USR)
typedef pthread_t osal_pthread_t;
typedef pthread_attr_t osal_pthread_attr_t;
typedef pthread_mutex_t osal_pthread_mutex_t;
typedef struct sched_param osal_sched_param;
#define osal_pthread_create     pthread_create
#define osal_pthread_detach     pthread_detach
#define osal_pthread_join       pthread_join
#define osal_pthread_self       pthread_self
#define osal_pthread_exit       pthread_exit
#define osal_pthread_cancel     pthread_cancel
#define osal_pthread_attr_init  pthread_attr_init
#define osal_pthread_attr_destroy       pthread_attr_destroy
#define osal_pthread_attr_setstacksize  pthread_attr_setstacksize
#define osal_pthread_attr_setinheritsched   pthread_attr_setinheritsched
#define osal_pthread_attr_setschedpolicy    pthread_attr_setschedpolicy
#define osal_pthread_attr_setschedparam     pthread_attr_setschedparam
#elif defined(__KERNEL__)
typedef int osal_pthread_mutex_t;
typedef int osal_pthread_t;
#else

#if !defined(_SYS_SCHED_H_)
struct sched_param {
        int sched_priority;
};
#endif

typedef int osal_pthread_mutex_t;
typedef int osal_pthread_attr_t;
typedef struct sched_param osal_sched_param;
typedef int osal_pthread_t;
#define osal_pthread_attr_init(a)                   (0)
#define osal_pthread_attr_setstacksize(a, b)        (0)
#define osal_pthread_attr_setinheritsched(a, b)     (0)
#define osal_pthread_attr_setschedpolicy(a, b)      (0)
#define osal_pthread_attr_setschedparam(a, b)       (0)
#define osal_pthread_create(a, b, c, d)             (0)
#define osal_pthread_attr_destroy(a)                (0)
#define sched_get_priority_min(a)                   (0)
#define sched_get_priority_max(a)                   (1)
#define osal_pthread_detach(a)                      (0)
#define osal_pthread_self()                         (0)
#define osal_pthread_exit(a)                        (0)
#define osal_pthread_cancel(a)
#define osal_pthread_join(a, b)

#ifndef SCHED_OTHER
#define SCHED_OTHER                                 (0)
#endif

#ifndef SCHED_FIFO
#define SCHED_FIFO                                  (1)
#endif

#ifndef SCHED_RR
#define SCHED_RR                                    (2)
#endif

#ifndef PTHREAD_EXPLICIT_SCHED
#define PTHREAD_EXPLICIT_SCHED                      (1)
#endif

#endif

#define DEFAULT_STACK_SIZE  10240
#define OSAL_MAX_PTHREAD_NAME_LEN   16
#define OSAL_MAX_TSKS       32

/* Scheduling Algorithm */
#define OS_PTHREAD_LOW_PRIO     gOsal_pthread_prio_min  
#define OS_PTHREAD_HIGH_PRIO    gOsal_pthread_prio_max
#define OSAL_SCHED_RR           (1 << 16)
#define OSAL_SCHED_FIFO         (1 << 17)
#define OSAL_SCHED_OTHER        (1 << 18)
#define OSAL_PTHREAD_LOW_PRIO   0
#define OSAL_PTHREAD_HIGH_PRIO  255
#define OSAL_PRIO_TO_OS_PRIO(osal_prio, os_prio) \
    os_prio = OS_PTHREAD_LOW_PRIO + (((int32_t)(osal_prio) - OSAL_PTHREAD_LOW_PRIO) * \
        (OS_PTHREAD_HIGH_PRIO - OS_PTHREAD_LOW_PRIO) /(OSAL_PTHREAD_HIGH_PRIO - OSAL_PTHREAD_LOW_PRIO));

typedef struct osal_pthread_task_struct
{
    osal_pthread_mutex_t phread_mutex;
    osal_pthread_t pthread_index;
    uint32_t        pthread_prio; /*prio with schedule*/
    uint32_t        pthread_stack_size;
    uint16_t        free;
    char            pthread_name[OSAL_MAX_PTHREAD_NAME_LEN];
}osal_pthread_tsk;


extern yt_ret_t osal_posix_pthread_init(void);
extern uint32_t osal_posix_pthread_create(char *pName, uint32_t thread_prio, uint32_t stack_size, void (*thread_handler)(void *), void *pArg);
extern yt_ret_t osal_posix_pthread_del(uint32_t task_id);
extern yt_ret_t osal_posix_pthread_delay(uint32_t us);
extern yt_ret_t osal_ptask_add(osal_pthread_tsk *pTask);
extern yt_ret_t osal_ptask_del(uint32_t task_id);
extern yt_ret_t osal_ptask_find(char *pName, uint32_t *pTaskId);
#endif

