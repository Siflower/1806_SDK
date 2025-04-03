/*******************************************************************************
*                                                                             *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.              *
*  Motorcomm Confidential and Proprietary.                                    *
*                                                                             *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_util.h
*
* @brief common use
*
********************************************************************************
*/
#ifndef __YT_UTIL_H__
#define __YT_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define IS_BIT_SET(value, bit)  (((value) >> (bit)) & 1l)
#define SET_BIT(value, bit)     ((value) = ((value) | (1l << (bit))))
#define CLEAR_BIT(value, bit)   ((value) = ((value) & ~(1l << (bit))))

#define CMM_UNUSED_PARAM(x) ((void)x)

#define IS_MCAST_ADDR(pAddr)    (((pAddr[0] & 0x01) != 0) ? TRUE : FALSE)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
