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
typedef pthread_t osal_thread_t;
typedef pthread_attr_t osal_pthread_attr_t;
#define osal_pthread_create     pthread_create
#define osal_thread_detach     pthread_detach
#define osal_thread_join       pthread_join
#define osal_thread_self       pthread_self
#define osal_thread_exit       pthread_exit
#define osal_pthread_attr_init      pthread_attr_init
#define osal_pthread_attr_setstacksize  pthread_attr_setstacksize
#elif defined(__KERNEL__)
typedef int osal_thread_t;
#else
typedef int osal_thread_t;
#endif

#define DEFAULT_STACK_SIZE  10240

extern osal_thread_t osal_thread_create(char *pName, uint32_t thread_prio, uint32_t stack_size, void (*thread_handler)(void *), void *pArg);
extern yt_ret_t osal_thread_del(osal_thread_t thread_id);
extern yt_ret_t osal_thread_delay(uint32_t us);

#endif

