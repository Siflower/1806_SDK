/*
 * FreeRTOS Kernel V10.3.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "yt_mem.h"

/* Stroe alloc attri for each module */
static yt_mem_malloc_attri_t memAllocAttri[YT_MEM_MALLOC_MODULE_MAX];

/******* Macro definition and Structure definition and static global variable for MEMPOOL *******/
/* used to store each entry point*/
static void **pEachEntry[YT_MEM_MALLOC_MODULE_MAX];

/* used to store each fdb entry count*/
static uint32_t eachFreeEntryCount[YT_MEM_MALLOC_MODULE_MAX] = {0};

/******* END:Macro definition and Structure definition and static global variable for MEMPOOL *******/

/******* Macro definition and Structure definition and static global variable for HEAP4 *******/
/* Create a couple of list links to mark the start and end of the list. */
static yt_mem_heap4_block_t memModuleStart[YT_MEM_MALLOC_MODULE_MAX];
static yt_mem_heap4_block_t *pmemModuleEnd[YT_MEM_MALLOC_MODULE_MAX] = {NULL};

/* Keeps track of the number of calls to allocate and free memory as well as the
number of free bytes remaining, but says nothing about fragmentation. */
static size_t memFreeBytesRemaining[YT_MEM_MALLOC_MODULE_MAX] = {0};

/* Gets set to the top bit of an size_t type.  When this bit in the xBlockSize
member of an yt_mem_heap4_block_t structure is set then the block belongs to the
application.  When the bit is free the block is still part of the free heap
space. */
static size_t xBlockAllocatedBit = 0;
/******* END:Macro definition and Structure definition and static global variable for HEAP4 *******/

/*
 * Inserts a block of memory that is being freed into the correct position in
 * the list of free memory blocks.  The block being freed will be merged with
 * the block in front it and/or the block behind it if the memory blocks are
 * adjacent to each other.
 */
static void yt_insert_block_into_free_list( yt_mem_heap4_block_t *pxBlockToInsert, yt_mem_module_t memModule)
{
    yt_mem_heap4_block_t *pxIterator = NULL;
    yt_mem_heap4_block_t *pStartTmp = NULL;
    yt_mem_heap4_block_t *pxEndTmp = NULL;
    uint8_t *puc = NULL;

    pStartTmp = &memModuleStart[memModule];
    pxEndTmp = pmemModuleEnd[memModule];

    /* Iterate through the list until a block is found that has a higher address
    than the block being inserted. */
    for( pxIterator = pStartTmp; pxIterator->pxNextFreeBlock < pxBlockToInsert; pxIterator = pxIterator->pxNextFreeBlock )
    {
        /* Nothing to do here, just iterate to the right position. */
    }

    /* Do the block being inserted, and the block it is being inserted after
    make a contiguous block of memory? */
    puc = ( uint8_t * ) pxIterator;
    if( ( puc + pxIterator->xBlockSize ) == ( uint8_t * ) pxBlockToInsert )
    {
        pxIterator->xBlockSize += pxBlockToInsert->xBlockSize;
        pxBlockToInsert = pxIterator;
    }

    /* Do the block being inserted, and the block it is being inserted before
    make a contiguous block of memory? */
    puc = ( uint8_t * ) pxBlockToInsert;
    if( ( puc + pxBlockToInsert->xBlockSize ) == ( uint8_t * ) pxIterator->pxNextFreeBlock )
    {
        if( pxIterator->pxNextFreeBlock != pxEndTmp )
        {
            /* Form one big block from the two blocks. */
            pxBlockToInsert->xBlockSize += pxIterator->pxNextFreeBlock->xBlockSize;
            pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock->pxNextFreeBlock;
    	}
    	else
    	{
            pxBlockToInsert->pxNextFreeBlock = pxEndTmp;
    	}
    }
    else
    {
        pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
    }

    /* If the block being inserted plugged a gab, so was merged with the block
    before and the block after, then it's pxNextFreeBlock pointer will have
    already been set, and should not be set here as that would make it point
    to itself. */
    if( pxIterator != pxBlockToInsert )
    {
        pxIterator->pxNextFreeBlock = pxBlockToInsert;
    }
}

static void *yt_mem_alloc_by_algo_heap4(size_t xWantedSize,  yt_mem_module_t memModule)
{
    yt_mem_heap4_block_t *pxBlock = NULL;
    yt_mem_heap4_block_t *pxPreviousBlock = NULL;
    yt_mem_heap4_block_t *pxNewBlockLink = NULL;
    yt_mem_heap4_block_t *pStartTmp = NULL;
    yt_mem_heap4_block_t *pxEndTmp = NULL;
    void *pvReturn = NULL;
    size_t *pFreeSizeTmp = NULL;

    /* select param for each module */
    pStartTmp = &memModuleStart[memModule];
    pxEndTmp = pmemModuleEnd[memModule];
    pFreeSizeTmp = &memFreeBytesRemaining[memModule];

    /* Check the requested block size is not so large that the top bit is
    set.  The top bit of the block size member of the yt_mem_heap4_block_t structure
    is used to determine who owns the block - the application or the
    kernel, so it must be free. */
    if( ( xWantedSize & xBlockAllocatedBit ) == 0 )
    {
        /* The wanted size is increased so it can contain a yt_mem_heap4_block_t
        structure in addition to the requested amount of bytes. */
        if( xWantedSize > 0 )
        {
            xWantedSize += xHeapStructSize;

            /* Ensure that blocks are always aligned to the required number
            of bytes. */
            if( ( xWantedSize & PORTBYTE_ALIGNMENT_MASK ) != 0x00 )
            {
                /* Byte alignment required. */
                xWantedSize += ( PORTBYTE_ALIGNMENT - ( xWantedSize & PORTBYTE_ALIGNMENT_MASK ) );
            }
        }

        if( ( xWantedSize > 0 ) && ( xWantedSize <= (*pFreeSizeTmp)) )
        {
            /* Traverse the list from the start	(lowest address) block until
            one	of adequate size is found. */
            pxPreviousBlock = pStartTmp;
            pxBlock = pStartTmp->pxNextFreeBlock;
            while( ( pxBlock->xBlockSize < xWantedSize ) && ( pxBlock->pxNextFreeBlock != NULL ) )
            {
                pxPreviousBlock = pxBlock;
                pxBlock = pxBlock->pxNextFreeBlock;
            }

            /* If the end marker was reached then a block of adequate size
            was	not found. */
            if( pxBlock != pxEndTmp )
            {
                /* Return the memory space pointed to - jumping over the
                yt_mem_heap4_block_t structure at its start. */
                pvReturn = ( void * ) ( ( ( uint8_t * ) pxPreviousBlock->pxNextFreeBlock ) + xHeapStructSize );

                /* This block is being returned for use so must be taken out
                of the list of free blocks. */
                pxPreviousBlock->pxNextFreeBlock = pxBlock->pxNextFreeBlock;

                /* If the block is larger than required it can be split into
                two. */
                if( ( pxBlock->xBlockSize - xWantedSize ) > MINIMUM_BLOCK_SIZE )
                {
                    /* This block is to be split into two.  Create a new
                    block following the number of bytes requested. The void
                    cast is used to prevent byte alignment warnings from the
                    compiler. */
                    pxNewBlockLink = ( void * ) ( ( ( uint8_t * ) pxBlock ) + xWantedSize );

                    /* Calculate the sizes of two blocks split from the
                    single block. */
                    pxNewBlockLink->xBlockSize = pxBlock->xBlockSize - xWantedSize;
                    pxBlock->xBlockSize = xWantedSize;

                    /* Insert the new block into the list of free blocks. */
                    yt_insert_block_into_free_list( pxNewBlockLink, memModule);
                }

                *pFreeSizeTmp -= pxBlock->xBlockSize;

                /* The block is being returned - it is allocated and owned
                by the application and has no "next" block. */
                pxBlock->xBlockSize |= xBlockAllocatedBit;
                pxBlock->pxNextFreeBlock = NULL;
            }
        }
    }

    return pvReturn;
}

static void yt_mem_free_by_algo_heap4( void *pv, yt_mem_module_t memModule)
{
    uint8_t *puc = ( uint8_t * ) pv;
    yt_mem_heap4_block_t *pxLink;
    size_t *pFreeSizeTmp = NULL;

    /* select mux for each module */
    pFreeSizeTmp = &memFreeBytesRemaining[memModule];

    if( pv != NULL )
    {
        /* The memory being freed will have an yt_mem_heap4_block_t structure immediately
        before it. */
        puc -= xHeapStructSize;

        /* This casting is to keep the compiler from issuing warnings. */
        pxLink = ( void * ) puc;

        if( ( pxLink->xBlockSize & xBlockAllocatedBit ) != 0 )
        {
            if( pxLink->pxNextFreeBlock == NULL )
            {
                /* The block is being returned to the heap - it is no longer
                allocated. */
                pxLink->xBlockSize &= ~xBlockAllocatedBit;

                /* Add this block to the list of free blocks. */
                *pFreeSizeTmp += pxLink->xBlockSize;
                yt_insert_block_into_free_list(  ( yt_mem_heap4_block_t * ) pxLink, memModule);
            }
        }
    }
}

static void *yt_mem_alloc_by_algo_mempool(size_t xWantedSize,  yt_mem_module_t memModule)
{
    void *pvReturn = NULL;

    if ((eachFreeEntryCount[memModule] > 0) && (xWantedSize <= memAllocAttri[memModule].allocEachEntrySize))
    {
        pvReturn = pEachEntry[memModule][eachFreeEntryCount[memModule] - 1];
    }

    eachFreeEntryCount[memModule]--;

    return pvReturn;
}

static void yt_mem_free_by_algo_mempool( void *pv, yt_mem_module_t memModule)
{
    eachFreeEntryCount[memModule]++;
    if (eachFreeEntryCount[memModule] > 0)
    {
        pEachEntry[memModule][eachFreeEntryCount[memModule] - 1] = pv;
    }
}

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
yt_ret_t yt_mem_register(yt_mem_malloc_attri_t memAllocAttriSet)
{
    yt_mem_heap4_block_t *pxFirstFreeBlock = NULL;
    uint8_t *pucAlignedHeap = NULL;
    size_t uxAddress = 0;
    size_t xTotalHeapSize = 0;
    uint16_t fdbEntryCount = 0;
    yt_ret_t ret_result = CMM_ERR_FAIL;

    if (memAllocAttriSet.module_id < YT_MEM_MALLOC_MODULE_MAX && memAllocAttriSet.algo == YT_MEM_MALLOC_ALGO_HEAP4)
    {
        if (memAllocAttriSet.allocTotalSize > MINIMUM_BLOCK_SIZE && memAllocAttriSet.pMemForModule != NULL)
        {
            memAllocAttri[memAllocAttriSet.module_id].algo = YT_MEM_MALLOC_ALGO_HEAP4;
            memAllocAttri[memAllocAttriSet.module_id].pMemForModule = memAllocAttriSet.pMemForModule;
            memAllocAttri[memAllocAttriSet.module_id].allocTotalSize = memAllocAttriSet.allocTotalSize;

            /* Ensure the heap starts on a correctly aligned boundary. */
            uxAddress = ( size_t ) memAllocAttriSet.pMemForModule;
            xTotalHeapSize = memAllocAttriSet.allocTotalSize;

            if( ( uxAddress & PORTBYTE_ALIGNMENT_MASK ) != 0 )
            {
                uxAddress += ( PORTBYTE_ALIGNMENT - 1 );
                uxAddress &= ~( ( size_t ) PORTBYTE_ALIGNMENT_MASK );
                xTotalHeapSize -= uxAddress - ( size_t ) memAllocAttriSet.pMemForModule;
            }

            pucAlignedHeap = ( uint8_t * ) uxAddress;

            /* xStart is used to hold a pointer to the first item in the list of free
            blocks.  The void cast is used to prevent compiler warnings. */
            memModuleStart[memAllocAttriSet.module_id].pxNextFreeBlock = ( void * ) pucAlignedHeap;
            memModuleStart[memAllocAttriSet.module_id].xBlockSize = ( size_t ) 0;

            /* pxEnd is used to mark the end of the list of free blocks and is inserted
            at the end of the heap space. */
            uxAddress = ( ( size_t ) pucAlignedHeap ) + xTotalHeapSize;
            uxAddress -= xHeapStructSize;
            uxAddress &= ~( ( size_t ) PORTBYTE_ALIGNMENT_MASK );
            pmemModuleEnd[memAllocAttriSet.module_id] = ( void * ) uxAddress;
            pmemModuleEnd[memAllocAttriSet.module_id]->xBlockSize = 0;
            pmemModuleEnd[memAllocAttriSet.module_id]->pxNextFreeBlock = NULL;

            /* To start with there is a single free block that is sized to take up the
            entire heap space, minus the space taken by pxEnd. */
            pxFirstFreeBlock = ( void * ) pucAlignedHeap;
            pxFirstFreeBlock->xBlockSize = uxAddress - ( size_t ) pxFirstFreeBlock;
            pxFirstFreeBlock->pxNextFreeBlock = pmemModuleEnd[memAllocAttriSet.module_id];

            /* Only one block exists - and it covers the entire usable heap space. */
            memFreeBytesRemaining[memAllocAttriSet.module_id] = pxFirstFreeBlock->xBlockSize;

            ret_result = CMM_ERR_OK;
        }
    }
    else if (memAllocAttriSet.module_id < YT_MEM_MALLOC_MODULE_MAX && memAllocAttriSet.algo == YT_MEM_MALLOC_ALGO_POOL)
    {
        if (memAllocAttriSet.allocTotalSize > MINIMUM_BLOCK_SIZE && memAllocAttriSet.pMemForModule != NULL)
        {
            memAllocAttri[memAllocAttriSet.module_id].algo = YT_MEM_MALLOC_ALGO_POOL;
            memAllocAttri[memAllocAttriSet.module_id].pMemForModule = memAllocAttriSet.pMemForModule;
            memAllocAttri[memAllocAttriSet.module_id].allocTotalSize = memAllocAttriSet.allocTotalSize;
            memAllocAttri[memAllocAttriSet.module_id].pEachEntryPoint = memAllocAttriSet.pEachEntryPoint;
            memAllocAttri[memAllocAttriSet.module_id].allocEntryCount = memAllocAttriSet.allocEntryCount;
            memAllocAttri[memAllocAttriSet.module_id].allocEachEntrySize = memAllocAttriSet.allocEachEntrySize;

            pEachEntry[memAllocAttriSet.module_id] = memAllocAttriSet.pEachEntryPoint;

            pucAlignedHeap = NULL; 
            uxAddress = ( size_t ) memAllocAttriSet.pMemForModule;

            xTotalHeapSize = memAllocAttriSet.allocTotalSize;

            if( ( uxAddress & PORTBYTE_ALIGNMENT_MASK ) != 0 )
            {
                uxAddress += ( PORTBYTE_ALIGNMENT - 1 );
                uxAddress &= ~( ( size_t ) PORTBYTE_ALIGNMENT_MASK );
                xTotalHeapSize -= uxAddress - ( size_t ) memAllocAttriSet.pMemForModule;
            }

            pucAlignedHeap = ( uint8_t * ) uxAddress;

            for (fdbEntryCount = 0; fdbEntryCount < memAllocAttriSet.allocEntryCount; fdbEntryCount++)
            {
                pEachEntry[memAllocAttriSet.module_id][fdbEntryCount] = (void *)(pucAlignedHeap + memAllocAttriSet.allocEachEntrySize*fdbEntryCount);
            }

            eachFreeEntryCount[memAllocAttriSet.module_id] = memAllocAttriSet.allocEntryCount;

            ret_result = CMM_ERR_OK;
        }
    }
    else
    {
        ;
    }

    /* Work out the position of the top bit in a size_t variable. */
    xBlockAllocatedBit = ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * YT_MEM_BITS_PER_BYTE ) - 1 );

    return ret_result;
}

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
void * yt_mem_malloc(size_t xWantedSize, yt_mem_module_t memModule)
{
    void *pMalloc = NULL;

    if ((memModule < YT_MEM_MALLOC_MODULE_MAX) && (memAllocAttri[memModule].pMemForModule != NULL) )
    {
        if (memAllocAttri[memModule].algo == YT_MEM_MALLOC_ALGO_HEAP4)
        {
            pMalloc = yt_mem_alloc_by_algo_heap4(xWantedSize, memModule);
        }
        else if (memAllocAttri[memModule].algo == YT_MEM_MALLOC_ALGO_POOL)
        {
            pMalloc = yt_mem_alloc_by_algo_mempool(xWantedSize, memModule);
        }
        else
        {
            ;
        }
    }

    return pMalloc;
}

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
void yt_mem_free(void * pv, yt_mem_module_t memModule)
{
    if ((memModule < YT_MEM_MALLOC_MODULE_MAX) && (memAllocAttri[memModule].pMemForModule != NULL) )
    {
        if (memAllocAttri[memModule].algo == YT_MEM_MALLOC_ALGO_HEAP4)
        {
            yt_mem_free_by_algo_heap4(pv, memModule);
        }
        else if (memAllocAttri[memModule].algo == YT_MEM_MALLOC_ALGO_POOL)
        {
            yt_mem_free_by_algo_mempool(pv, memModule);
        }
        else
        {
            ;
        }
    }
}

/**
 * @internal      yt_mem_free_size_get
 * @endinternal
 *
 * @brief         get free memory size for each module
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     memModule                -module id
 * @retval        freeSize
 */
size_t yt_mem_free_size_get(yt_mem_module_t memModule)
{
    size_t freeSizeTmp = 0;
    
    if ((memModule < YT_MEM_MALLOC_MODULE_MAX) && (memAllocAttri[memModule].pMemForModule != NULL) )
    {
        if (memAllocAttri[memModule].algo == YT_MEM_MALLOC_ALGO_HEAP4)
        {
            freeSizeTmp = memFreeBytesRemaining[memModule];
        }
        else if (memAllocAttri[memModule].algo == YT_MEM_MALLOC_ALGO_POOL)
        {
            freeSizeTmp = eachFreeEntryCount[memModule]*memAllocAttri[memModule].allocEachEntrySize;
        }
        else
        {
            ;
        }
    }

    return freeSizeTmp;
}
