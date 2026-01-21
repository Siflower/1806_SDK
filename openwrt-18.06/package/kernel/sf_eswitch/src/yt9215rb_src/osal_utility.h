#ifndef __OSAL_UTILITY_H__
#define __OSAL_UTILITY_H__
#if defined(__KERNEL__)
#include <linux/delay.h>
#else
#include <unistd.h>
#endif


#if defined(OS_CYGWIN) || defined(OS_LINUX_USR)
#define osal_open                   open
#define osal_close                  close
#define osal_usleep                 usleep
#else
#define osal_open                   (0)
#define osal_close                  (0)
#define osal_usleep
#endif

#if defined(__KERNEL__)
#define osal_mdelay(m)                 mdelay(m)
#else
#define osal_mdelay(m)                 osal_usleep(m * (1000))
#endif

#endif
