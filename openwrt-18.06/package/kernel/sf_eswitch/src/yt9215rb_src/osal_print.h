#ifndef __OSAL_PRINT_H__
#define __OSAL_PRINT_H__

#if defined(__KERNEL__)
#include <linux/string.h>
#endif
#include "yt_util.h"

/*add other space later */
#if defined(__KERNEL__)
#define osal_printf                    printk
#else
#define osal_printf                    printf
#endif
#define osal_strncmp              strncmp
#define osal_strlen                  strlen
#define osal_strchr                 strchr
#define osal_strspn                 strspn
#define osal_strtol                  strtol
#define osal_strcmp               strcmp

#if defined (BOUNDSCHECK_FOR_FUNCS)
#define osal_strcpy(x, y, z)            strcpy_s(x, y, z)
#define osal_strncpy(x, y, w, z)      strncpy_s(x, y, w, z)
#define osal_strcat(x, y, z)             strcat_s(x, y, z)
#define osal_sprintf(x, y, z, ARGS...)       sprintf_s(x, y, z, ## ARGS)
#define osal_vsnprintf(x, y, w, z, k) vsnprintf_s(x, y, w, z, k)
#define osal_strtok(x, y, z)             strtok_s(x, y, z)
#else
#define osal_strcpy(x, y, z)            strcpy(x, z)
#define osal_strncpy(x, y, w, z)      strncpy(x, w, z)
#define osal_strcat(x, y, z)             strcat(x, z)
#define osal_sprintf(x, y, z, ARGS...)   sprintf(x, z, ## ARGS)
#define osal_vsnprintf(x, y, w, z, k)  vsnprintf(x, w, z, k)
#define osal_strtok(x, y, z)              strtok(x, y);  \
                                                     CMM_UNUSED_PARAM(z)
#endif

#if defined(__KERNEL__)
#define osal_strtoul                simple_strtoul
#else
#define osal_strtoul                strtoul
#endif
#if defined(__KERNEL__)
#define osal_strtou32            simple_strtoul
#else
#define osal_strtou32            strtoul
#endif
#define osal_qsort                  qsort
#define osal_atoi                    atoi

#define osal_isdigit                 isdigit
#define osal_isxdigit               isxdigit
#define osal_isspace               isspace


#define OSAL_DEBUG(DEBUG_ENABLE, msg) \
    if (DEBUG_ENABLE) \
    { \
        osal_printf msg; \
    }

#endif
