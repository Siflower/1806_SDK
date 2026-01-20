/*******************************************************************************
*                                                                             *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.              *
*  Motorcomm Confidential and Proprietary.                                    *
*                                                                             *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_mem.h
*
* @brief common use
*
********************************************************************************
*/
#ifndef __YT_MEM_H__
#define __YT_MEM_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "yt_types.h"
#include "yt_error.h"
#include "osal_sem.h"
#include "cal_cmm.h"

/* align config */
#define PORTBYTE_ALIGNMENT          4

#if defined(PORTBYTE_ALIGNMENT) && (PORTBYTE_ALIGNMENT == 32)
    #define PORTBYTE_ALIGNMENT_MASK ( 0x001f )
#elif defined(PORTBYTE_ALIGNMENT) && (PORTBYTE_ALIGNMENT == 16)
    #define PORTBYTE_ALIGNMENT_MASK ( 0x000f )
#elif defined(PORTBYTE_ALIGNMENT) && (PORTBYTE_ALIGNMENT == 8)
    #define PORTBYTE_ALIGNMENT_MASK ( 0x0007 )
#elif defined(PORTBYTE_ALIGNMENT) && (PORTBYTE_ALIGNMENT == 4)
    #define PORTBYTE_ALIGNMENT_MASK	( 0x0003 )
#else /* default is 4 */
    #define PORTBYTE_ALIGNMENT_MASK	( 0x0003 )
#endif

/* malloc attribute of config algo and module*/
typedef enum yt_mem_module_e
{
    YT_MEM_MALLOC_MODULE_DEFAULT,
    YT_MEM_MALLOC_MODULE_CLI,
    YT_MEM_MALLOC_MODULE_ACL,
    YT_MEM_MALLOC_MODULE_FDB,
    YT_MEM_MALLOC_MODULE_MAX
}yt_mem_module_t;

typedef enum yt_mem_algo_e
{
    YT_MEM_MALLOC_ALGO_NONE,
    YT_MEM_MALLOC_ALGO_HEAP4,
    YT_MEM_MALLOC_ALGO_POOL
}yt_mem_algo_t;

typedef struct yt_mem_malloc_attri_s
{
    yt_mem_module_t module_id;      /* module id */
    yt_mem_algo_t algo;                   /* mem alloc algo */
    void *pMemForModule;           /* mempool and  heap4 algo all need to set it,module mem */
    uint32_t allocTotalSize;               /* mempool and  heap4 algo all need to set it,alloc total size */
    void **pEachEntryPoint;           /* only mempool algo need to set it,each entry point */
    uint32_t allocEntryCount;              /* only mempool algo need to set it,alloc max count at the same moment */
    uint32_t allocEachEntrySize;        /* only mempool algo need to set it,each memory pool block size*/
} yt_mem_malloc_attri_t;

/* Define the linked list structure.  This is used to link free blocks in order
of their memory address. */
typedef struct yt_mem_heap4_block_s
{
    struct yt_mem_heap4_block_s *pxNextFreeBlock;	/*<< The next free block in the list. */
    size_t xBlockSize;						/*<< The size of the free block. */
} yt_mem_heap4_block_t;

/* The size of the structure placed at the beginning of each allocated memory
block must by correctly byte aligned. */
static const size_t xHeapStructSize	= ( sizeof( yt_mem_heap4_block_t ) + ( ( size_t ) ( PORTBYTE_ALIGNMENT - 1 ) ) ) & (~( ( size_t ) PORTBYTE_ALIGNMENT_MASK ));
/* Block sizes must not get too small. */
#define MINIMUM_BLOCK_SIZE	( ( size_t ) ( xHeapStructSize << 1 ) )
/* Assumes 8bit bytes! */
#define YT_MEM_BITS_PER_BYTE		(( size_t )8)

/**
 * @internal      yt_mem_register
 * @endinternal
 *
 * @brief         register mem for each module 
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     memAllocAttriSet                -each module alloc attribute
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_mem_register(yt_mem_malloc_attri_t memAllocAttri);

/**
 * @internal      yt_mem_malloc
 * @endinternal
 *
 * @brief         alloc memory for each module
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     xWantedSize                -alloc size
 * @param[in]     memModule                -module id
 * @retval        mem ptr
 */
extern void * yt_mem_malloc(size_t xWantedSize, yt_mem_module_t memModule);

/**
 * @internal      yt_mem_free
 * @endinternal
 *
 * @brief         free memory for each module
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     xWantedSize                -free memory ptr
 * @param[in]     memModule                -module id
 * @retval        NONE
 */
extern void yt_mem_free(void * pv, yt_mem_module_t memModule);

/**
 * @internal      yt_mem_free_size_get
 * @endinternal
 *
 * @brief         get free memory size for each module
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     memModule                -module id
 * @retval        freeSize
 */
extern size_t yt_mem_free_size_get(yt_mem_module_t memModule);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

