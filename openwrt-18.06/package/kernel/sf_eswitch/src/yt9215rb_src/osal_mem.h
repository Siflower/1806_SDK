#ifndef __OSAL_MEM_H__
#define __OSAL_MEM_H__
#if defined(__KERNEL__)
#include <linux/vmalloc.h>
#include <linux/string.h>
#endif
#include "yt_mem.h"

/*
 * Macro Definition
 */
#if defined(__KERNEL__)
#if defined (MEM_ALLOC_BY_CUSTOM_FUNC)
#define osal_malloc(x, y)     yt_mem_malloc(x, y)
#define osal_free(x, y)       yt_mem_free(x, y)
#else
#define osal_malloc(x, y)     vmalloc(x)
#define osal_free(x, y)       vfree(x)
#endif
#else
#if defined (MEM_ALLOC_BY_CUSTOM_FUNC)
#define osal_malloc(x, y)     yt_mem_malloc(x, y)
#define osal_free(x, y)       yt_mem_free(x, y)
#else
#define osal_malloc(x, y)     malloc(x)
#define osal_free(x, y)       free(x)
#endif
#endif

#if defined (BOUNDSCHECK_FOR_FUNCS)
#define osal_memset(x, y, w, z)     memset_s(x, y, w, z)
#define osal_memcpy(x, y, w, z)     memcpy_s(x, y, w, z)
#define osal_memmove(x, y, w, z)    memmove_s(x, y, w, z)
#else
#define osal_memset(x, y, w, z)     memset(x, w, z)
#define osal_memcpy(x, y, w, z)     memcpy(x, w, z)
#define osal_memmove(x, y, w, z)    memmove(x, w, z)
#endif

#define osal_read             read
#define osal_write            write
#define osal_memcmp     memcmp
#endif
