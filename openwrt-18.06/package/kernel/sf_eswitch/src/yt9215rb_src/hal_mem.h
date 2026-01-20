/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#ifndef __HAL_MEM_H__
#define __HAL_MEM_H__

/*
 * Include Files
 */
#include "yt_error.h" 
#include "osal_sem.h"
#ifdef SWITCH_SERIES_TIGER
#include "fal_tiger_mem.h"
#endif
#ifdef SWITCH_SERIES_SHARK
#include "fal_shark_mem.h"
#endif

#ifdef SWITCH_SERIES_WHALE
#include "fal_whale_mem.h"
#endif

/*
 * Symbol Definition
 */
extern osal_mux g_cfgmux;
extern uint8_t ghal_mem32_init;
extern uint8_t ghal_reg_table_init;

/*
 * Macro Definition
 */


#define HAL_TBL_REG_WRITE(unit,mem,idx,len,pvlaue)   hal_table_reg_write(unit,mem,idx,len,pvlaue)
#define HAL_TBL_REG_READ(unit,mem,idx, len, pvlaue)  hal_table_reg_read(unit,mem,idx,len,pvlaue)

#define HAL_MEM_DIRECT_READ(unit, memaddr, pvalue)   hal_mem32_read(unit, memaddr, pvalue)
#define HAL_MEM_DIRECT_WRITE(unit, memaddr, value)   hal_mem32_write(unit, memaddr, value)

#define GET_FIELD(value, low_bit, width) (((value) >> (low_bit)) & ((1UL<<(width))-1))
#define CLR_FIELD(value, low_bit, width) ((value) & (~(((1UL<<(width))-1)<<(low_bit))))

#define HAL_FIELD_SET(mem, field, entry, data)   hal_tbl_reg_field_set(mem, field, entry, data)
#define HAL_FIELD_GET(mem, field, entry, pdata) \
do {\
    uint32_t _tmpData_; \
    hal_tbl_reg_field_get(mem, field, entry, &_tmpData_); \
    *pdata = _tmpData_; \
}while(0)

#define MEM_FLAG_REG   0x1
#define MEM_FLAG_TBL 0x2
#define MEM_FLAG_MSG   0x4
#define MEM_FLAG_KEY   0x8

#define FIELD_NORMAL 0x1

extern uint32 DEBUG_MODE_K;

struct field_id_s {
    uint8_t field_id;
    uint8_t width;
    uint8_t word_offset;
    uint8_t bit_offset;
};
typedef struct field_id_s field_id_t;

struct tbl_reg_info_s {
    uint32_t asic_offset;
    uint32_t max_entry_size;
    uint32_t entry_real_size;
    uint32_t entry_number;
    uint32_t fields_num;
    const field_id_t *fields;
};

typedef struct tbl_reg_info_s tbl_reg_info_t;

extern const tbl_reg_info_t tbl_reg_list[NUM_MEMS];

/*
 * Function Declaration
 */

uint32_t hal_mem32_write(yt_unit_t unit, uint32_t addr, uint32_t Val);
uint32_t hal_mem32_read(yt_unit_t unit, uint32_t addr, uint32_t *pVal);

extern uint32_t hal_mem32_init(void);
extern uint32_t hal_table_reg_init(void);
extern uint32_t hal_table_reg_exit(void);

extern uint32 hal_table_reg_write(uint8_t unit, uint32 mem_id, uint32_t idx, uint16_t len, void *pvalue);
extern uint32 hal_table_reg_read(uint8_t unit, uint32 mem_id, uint32_t idx,uint16_t len, void *pvalue);

extern uint32  hal_tbl_reg_field_set(uint32_t mem_id, uint32_t field_id, void *entry, uint32_t data);
extern uint32  hal_tbl_reg_field_get(uint32_t mem_id, uint32_t field_id, void *entry, void *data);

#endif
