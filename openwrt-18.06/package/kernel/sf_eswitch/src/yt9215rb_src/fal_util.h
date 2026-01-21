/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#ifndef __FAL_UTIL_H__
#define __FAL_UTIL_H__

#include "yt_error.h"
#include "yt_types.h"

extern uint32_t fal_bits_1_count_get(uint32_t value);
extern uint8_t fal_32bits_1_count_get(uint32_t value, uint32_t *pCount);
extern uint8_t fal_16bits_1_count_get(uint16_t value, uint32_t *pCount);
extern uint8_t fal_8bits_1_count_get(uint8_t value, uint32_t *pCount);
extern uint8_t fal_8bits_right_get(uint8_t value, uint32_t *pIndex);
extern uint8_t fal_16bits_right_get(uint16_t value, uint32_t *pIndex);
#endif

