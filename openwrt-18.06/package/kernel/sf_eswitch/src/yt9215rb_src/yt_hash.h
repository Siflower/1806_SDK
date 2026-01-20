/*******************************************************************************
*                                                                             *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.              *
*  Motorcomm Confidential and Proprietary.                                    *
*                                                                             *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_hash.h
*
* @brief common use
*
********************************************************************************
*/
#ifndef __YT_HASH_H__
#define __YT_HASH_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "yt_types.h"
#include "yt_error.h"
#include "osal_sem.h"
#include "cal_cmm.h"
#include "osal_mem.h"
#include "yt_mem.h"

#define YT_HASH_BUCKET_MAX_SIZE  0x10000

typedef struct yt_hash_table_node_s 
{
    struct yt_hash_table_node_s *pPrevNode;
    struct yt_hash_table_node_s *pNextNode;
    uint8_t nodeKeyData[0];
}yt_hash_table_node_t;

typedef struct yt_hash_table_bucket_s 
{
    uint32_t   nodeCntUsed;  /* total node count in this bucket */
    yt_hash_table_node_t *pNodeHead;  /* first node head in this bucket */
    osal_mux   bucketLock; /* bucket lock */
}yt_hash_table_bucket_t;

typedef struct yt_hash_table_s
{
    uint32_t   hashBucketCount; /* alloced hashValue table buckets count */
    uint32_t   hashNodeCountMax; /* max nodes in hash table,used for hash bucket index calculate */
    uint32_t   bucketCntUsed;   /* total hashValue table buckets count in use */
    uint32_t   nodeCntUsed;      /* total hashValue table nodes count in use */
    yt_hash_table_bucket_t *pBuckets;
}yt_hash_table_t;

/**
 * @internal      yt_hash_table_init
 * @endinternal
 *
 * @brief         init hash table
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     pHashTable                -hash table pointer
 * @param[in]     hashBucketCount        -hash bucket count
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_hash_table_init(yt_hash_table_t *pHashTable, uint32_t hashBucketCount, uint32_t hashNodeCountMax);

/**
 * @internal      yt_hash_table_destory
 * @endinternal
 *
 * @brief         destroy hash table,when need to destory hashValue table ,must free all locks of bucket and free all nodes first
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     pHashTable                -hash table pointer
 * @retval        NONE
 */
extern void yt_hash_table_destory(yt_hash_table_t *pHashTable);

/**
 * @internal      yt_hash_table_bucket_idx_get
 * @endinternal
 *
 * @brief         get hash table bucket index by pKey
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     pHashTable                -hash table pointer
 * @param[in]     pKey                      -pKey
 * @param[in]     keysize                 -pKey size
 * @retval          hash table bucket index
 */
extern uint32_t yt_hash_table_bucket_idx_get(yt_hash_table_t *pHashTable, uint8_t *pKey, uint32_t keysize);

/**
 * @internal      yt_hash_table_bucket_idx_get_by_u32key
 * @endinternal
 *
 * @brief         get hash table bucket index by u32 keyValue
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     pHashTable                -hash table pointer
 * @param[in]     keyValue                      -key value
 * @retval          hash table bucket index
 */
extern uint32_t yt_hash_table_bucket_idx_get_by_u32key(yt_hash_table_t *pHashTable, uint32_t keyValue);

/**
 * @internal      yt_hash_table_bucket_lock
 * @endinternal
 *
 * @brief         lock hash table bucket by hash bucket index
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     pHashTable                -hash table pointer
 * @param[in]     bucketIdx                   -hash table bucket index
 * @retval        NONE
 */
extern yt_ret_t yt_hash_table_bucket_lock(yt_hash_table_t *pHashTable, uint32_t bucketIdx);

/**
 * @internal      yt_hash_table_bucket_unlock
 * @endinternal
 *
 * @brief         unlock hash table bucket by hash bucket index
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     pHashTable                -hash table pointer
 * @param[in]     bucketIdx                   -hash table bucket index
 * @retval        NONE
 */
extern yt_ret_t yt_hash_table_bucket_unlock(yt_hash_table_t *pHashTable, uint32_t bucketIdx);

/**
 * @internal      yt_hash_table_node_find
 * @endinternal
 *
 * @brief         find hash table node with hash bucket index and pKey
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     pHashTable                -hash table pointer
 * @param[in]     bucketIdx                   -hash table bucket index
 * @param[in]     pKey                      -pKey
 * @param[in]     keysize                 -pKey size
 * @retval          hash table node
 */
extern yt_hash_table_node_t * yt_hash_table_node_find(yt_hash_table_t *pHashTable, uint32_t bucketIdx, void *pKey,  uint32_t keysize);

/**
 * @internal      yt_hash_table_node_find_by_u32key
 * @endinternal
 *
 * @brief         find hash table node with hash bucket index and u32 keyValue
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     pHashTable                -hash table pointer
 * @param[in]     bucketIdx                   -hash table bucket index
 * @param[in]     keyValue                      -key value
 * @retval          hash table node
 */
extern yt_hash_table_node_t * yt_hash_table_node_find_by_u32key(yt_hash_table_t *pHashTable, uint32_t bucketIdx, uint32_t keyValue);

/**
 * @internal      yt_hash_table_node_add
 * @endinternal
 *
 * @brief         add hash table node randomly with hash bucket index
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     pHashTable                -hash table pointer
 * @param[in]     bucketIdx                   -hash table bucket index
 * @param[in]     pNode                 -node
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_hash_table_node_add(yt_hash_table_t *pHashTable, uint32_t bucketIdx, yt_hash_table_node_t *pNode);

/**
 * @internal      yt_hash_table_node_add_sort_by_key
 * @endinternal
 *
 * @brief         when add hash table node,sorted by u32 key and hash bucket index
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     pHashTable                -hash table pointer
 * @param[in]     bucketIdx                   -hash table bucket index
 * @param[in]     pNode                 -node
 * @param[in]     keyValue                 -key value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_hash_table_node_add_sort_by_u32key(yt_hash_table_t *pHashTable, uint32_t bucketIdx, yt_hash_table_node_t *pNode, uint32_t keyValue);

/**
 * @internal      yt_hash_table_node_del
 * @endinternal
 *
 * @brief         del hash table node with hash bucket index
 * @note          APPLICABLE DEVICES  -Shark
 * @param[in]     pHashTable                -hash table pointer
 * @param[in]     bucketIdx                   -hash table bucket index
 * @param[in]     pNode                 -node
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_hash_table_node_del(yt_hash_table_t *pHashTable, uint32_t bucketIdx, yt_hash_table_node_t *pNode);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

