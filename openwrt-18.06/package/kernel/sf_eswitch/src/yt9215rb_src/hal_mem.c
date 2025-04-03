/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/*
 * Include Files
 */
#include "osal_mem.h"
#include "osal_sem.h"
#include "osal_print.h"
#include "yt_types.h"
#include "hal_mem.h"
#include "yt_util.h"

#ifdef ACC_UART
#include "uart.h"
#elif defined ACC_MEM
#include "mem.h"
#else
#include "ctrlif.h"
#endif

#ifdef MEM_MODE_CMODEL
#include "hal_mem_cmodel.h"
#endif

/*
 * Data Declaration
 */
osal_mux g_cfgmux;

uint8_t ghal_mem32_init      = FALSE;
uint8_t ghal_reg_table_init  = FALSE;
uint32 DEBUG_MODE_K = 0;

#ifdef MEM_MODE_CMODEL
hal_reg_tbl_mode_t greg_tbl_mode = HAL_REG_TBL_MODE_CMODEL;
#else
hal_reg_tbl_mode_t greg_tbl_mode = HAL_REG_TBL_MODE_NORMAL;
#endif

/*
 * Symbol Definition
 */

/*
 * Macro Declaration
 */

#define DRV_MEM_DS_VALID_CHK(mem_id, idx) \
do {\
    CMM_PARAM_CHK((NUM_MEMS <= mem_id), CMM_ERR_INPUT);\
    if((sizeof(tbl_reg_list) / sizeof(tbl_reg_list[0])) < NUM_MEMS) \
    {\
        osal_printf("Mem size %u is less than %u\r\n", (uint32_t)(sizeof(tbl_reg_list)/sizeof(tbl_reg_list[0])), NUM_MEMS);\
        return CMM_ERR_REG_TABLE_NUM;\
    }\
    if(tbl_reg_list[mem_id].entry_number<= idx )\
    {\
        osal_printf("Mem %u entry num %u is less than %u\r\n", mem_id, tbl_reg_list[mem_id].entry_number , idx);\
        return CMM_ERR_REG_TABLE_IDX;\
    }\
} while(0)

/*
 * Function Declaration
 */
static uint32_t hal_table_reg_op(uint8_t unit, uint8_t is_write, uint32 mem_id, uint32_t idx, uint32_t len, void *pvalue);

static uint32_t hal_table_reg_normal_write(uint8_t unit, uint32 mem_id, uint32_t idx, uint16_t len, void *pvalue);
static uint32_t hal_table_reg_normal_read(uint8_t unit, uint32 mem_id, uint32_t idx, uint16_t len, void *pvalue);


uint32_t hal_mem32_init(void)
{
    ghal_mem32_init = TRUE;
    return CMM_ERR_OK;
}


uint32_t hal_table_reg_init(void)
{
    osal_mux_init(&g_cfgmux, NULL);
    
#ifdef MEM_MODE_CMODEL
    hal_table_reg_cmodel_init();
#endif

    ghal_reg_table_init = TRUE;

    return CMM_ERR_OK;
}

uint32_t hal_mem32_write(yt_unit_t unit, uint32_t addr, uint32_t Val)
{
    CMM_UNUSED_PARAM(unit);
#ifdef ACC_UART
    return uart_switch_write( addr,  Val);
#elif defined ACC_MEM
    return mem_switch_write( addr,  Val);
#else
    return ctrlif_reg_write(unit, addr, Val);
#endif

    return CMM_ERR_OK;
}
uint32_t hal_mem32_read(yt_unit_t unit, uint32_t addr, uint32_t *pVal)
{
        CMM_UNUSED_PARAM(unit);
        
#ifdef ACC_UART
        return uart_switch_read(addr,  pVal);
#elif defined ACC_MEM
        return mem_switch_read( addr,  pVal);
#else
        return ctrlif_reg_read(unit, addr, pVal);
#endif
}


uint32  hal_tbl_reg_field_get(uint32_t mem_id, uint32_t field_id,  void *entry, void *data)
{
    uint32 word, low, width;
    uint32 value_hi, value_lo, value;
    uint32 *ptbl = (uint32_t *)entry;
    uint32 *pdata = (uint32_t *)data;

    if (mem_id >= NUM_MEMS) return CMM_ERR_FAIL; //tbl_reg not found
    if (field_id >= tbl_reg_list[mem_id].fields_num) return CMM_ERR_FAIL; //field not found

    word = tbl_reg_list[mem_id].fields[field_id].word_offset; 
    low = tbl_reg_list[mem_id].fields[field_id].bit_offset;
    width = tbl_reg_list[mem_id].fields[field_id].width;

    if (low + width < 32)
    {
        value = ptbl[word];
        *pdata = (uint32)GET_FIELD(value, low, width);
    }
    else
    {    
        if (low == 0 && width == 32)
        {
            *pdata = ptbl[word];
        }
        else
        {
            value_hi = ptbl[word + 1];
            value_lo = ptbl[word];
            value_hi = (uint32)GET_FIELD(value_hi, 0, (width - (32 - low)));
            value_lo = (uint32)GET_FIELD(value_lo, low, (32 - low));
            *pdata = (value_hi << (32 - low)) + value_lo;
        }
    }

    return CMM_ERR_OK;
}	

uint32  hal_tbl_reg_field_set(uint32_t mem_id, uint32_t field_id,  void *entry, uint32_t data)
{
    uint32 word, low, width, value;
    uint32 *ptbl = (uint32_t *)entry;

    if (mem_id >= NUM_MEMS) return CMM_ERR_FAIL; //tbl_reg not found
    if (field_id >= tbl_reg_list[mem_id].fields_num) return CMM_ERR_FAIL; //field not found

    word = tbl_reg_list[mem_id].fields[field_id].word_offset; 
    low = tbl_reg_list[mem_id].fields[field_id].bit_offset;
    width = tbl_reg_list[mem_id].fields[field_id].width;

    value = ptbl[word];
    value = (uint32)CLR_FIELD(value, low, width);
    if(width == 32)
    {
        value = 0;
    }
    value |= (data << low);

    ptbl[word] = value;

    return CMM_ERR_OK;
}	

static uint32_t hal_table_reg_op(uint8_t unit, uint8_t is_write, uint32 mem_id, uint32_t idx, uint32_t len, void *pvalue)
{
    uint32 i;
    cmm_err_t ret;

    /*Use 32bits addr now*/
    uint32 addr;
    uint32 *pvalue_tmp = (uint32*)pvalue;
    
    CMM_UNUSED_PARAM(len);
    
    CMM_PARAM_CHK((NULL == pvalue), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((FALSE == ghal_mem32_init), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((FALSE == ghal_reg_table_init), CMM_ERR_NOT_INIT);

  
    DRV_MEM_DS_VALID_CHK(mem_id, idx);
    osal_mux_lock(&g_cfgmux);
    
    addr = tbl_reg_list[mem_id].asic_offset + tbl_reg_list[mem_id].max_entry_size*idx ;

    for(i = 0; (tbl_reg_list[mem_id].entry_real_size/(sizeof(uint32))) > i; i++)
    {
        if(NULL == (pvalue_tmp + i))
        {
            osal_mux_unlock(&g_cfgmux);
            return CMM_ERR_NULL_POINT;
        }
        if(is_write)
        {    
            ret = hal_mem32_write(unit, addr, *(pvalue_tmp + i));
            if(DEBUG_MODE_K == 1)
            {
                  osal_printf("Write addr[0x%x] = 0x%x\n",addr,*(uint32 *)(pvalue_tmp + i));
            }
        }
        else
        {
            ret = hal_mem32_read(unit, addr, pvalue_tmp + i);
        }

        if(CMM_ERR_OK != ret)
        {
            osal_mux_unlock(&g_cfgmux);
            return ret;
        }

        if(is_write == 0)
       {
          if(DEBUG_MODE_K == 1)
          {
               osal_printf("Read addr[0x%x] = 0x%x\n",addr,*(uint32 *)(pvalue_tmp + i));
          }
       }

        addr += (sizeof(uint32));
    }


    osal_mux_unlock(&g_cfgmux);

	return CMM_ERR_OK;
}

static uint32_t hal_table_reg_normal_write(uint8_t unit, uint32 mem_id, uint32_t idx, uint16_t len, void *pvalue)
{
    cmm_err_t ret;

    CMM_UNUSED_PARAM(unit);
    
    CMM_PARAM_CHK((NULL == pvalue), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((FALSE == ghal_mem32_init), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((FALSE == ghal_reg_table_init), CMM_ERR_NOT_INIT);
    
    CMM_ERR_CHK(hal_table_reg_op(unit, TRUE, mem_id, idx, len, pvalue), ret);
    
    return CMM_ERR_OK;
}

static uint32_t hal_table_reg_normal_read(uint8_t unit, uint32 mem_id, uint32_t idx, uint16_t len, void *pvalue)
{
    cmm_err_t ret;

    CMM_PARAM_CHK((NULL == pvalue), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((FALSE == ghal_mem32_init), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((FALSE == ghal_reg_table_init), CMM_ERR_NOT_INIT);

#ifdef INTERNAL_MSG_DEBUG
    if (MEM_FLAG_MSG == tbl_reg_list[mem_id].flag)
    {
        CMM_ERR_CHK(hal_dbg_msg_read(unit, mem_id, pvalue), ret);
    }
    else
#endif
    {
        CMM_ERR_CHK(hal_table_reg_op(unit, FALSE, mem_id, idx, len, pvalue), ret);
    }

    return CMM_ERR_OK;
}


uint32 hal_table_reg_write(uint8_t unit, uint32 mem_id, uint32_t idx, uint16_t len, void *pvalue)
{
    cmm_err_t ret;

#ifdef MEM_MODE_CMODEL 
    if ((HAL_REG_TBL_MODE_CMODEL == greg_tbl_mode) || (HAL_REG_TBL_MODE_BOTH == greg_tbl_mode))
    { 
        CMM_ERR_CHK(hal_table_reg_cmodel_write(unit, mem_id, idx, len, pvalue), ret);
    }
#endif

    if ((HAL_REG_TBL_MODE_NORMAL == greg_tbl_mode) || (HAL_REG_TBL_MODE_BOTH == greg_tbl_mode))
    {
        CMM_ERR_CHK(hal_table_reg_normal_write(unit, mem_id, idx, len, pvalue), ret);
    }
    return CMM_ERR_OK;
}


uint32 hal_table_reg_read(uint8_t unit, uint32 mem_id, uint32_t idx,uint16_t len, void *pvalue)
{
    cmm_err_t ret;

#ifdef MEM_MODE_CMODEL 
    if ((HAL_REG_TBL_MODE_CMODEL == greg_tbl_mode) || (HAL_REG_TBL_MODE_BOTH == greg_tbl_mode))
    {
        CMM_ERR_CHK(hal_table_reg_cmodel_read(unit, mem_id, idx, len, pvalue), ret);
    }
#endif

    if ((HAL_REG_TBL_MODE_NORMAL == greg_tbl_mode) || (HAL_REG_TBL_MODE_BOTH == greg_tbl_mode))
    {
        CMM_ERR_CHK(hal_table_reg_normal_read(unit, mem_id, idx, len, pvalue), ret);
    }

    return CMM_ERR_OK;
}


uint32 hal_table_reg_mode_set(uint8_t unit, hal_reg_tbl_mode_t mode)
{
    CMM_UNUSED_PARAM(unit);
    
    greg_tbl_mode = mode;
    
    return CMM_ERR_OK;
}


uint32 hal_table_reg_mode_get(uint8_t unit, hal_reg_tbl_mode_t *pmode)
{
    CMM_UNUSED_PARAM(unit);
    CMM_PARAM_CHK((NULL == pmode), CMM_ERR_NULL_POINT);
    
    *pmode = greg_tbl_mode;
    
    return CMM_ERR_OK;
}


uint32 hal_table_reg_reset(uint8_t unit, hal_reg_tbl_mode_t mode)
{
    cmm_err_t ret = CMM_ERR_OK;
    
    if(HAL_REG_TBL_MODE_CMODEL == mode || HAL_REG_TBL_MODE_BOTH == mode)
    {
#ifdef MEM_MODE_CMODEL 
        CMM_ERR_CHK(hal_table_reg_cmodel_reset(unit), ret);
#else
        CMM_UNUSED_PARAM(unit);
        CMM_UNUSED_PARAM(ret);
#endif
    }

    return CMM_ERR_OK;
}


uint32_t hal_table_reg_exit(void)
{
    if(!ghal_reg_table_init)
    {
        return CMM_ERR_OK;
    }
    
#ifdef MEM_MODE_CMODEL
    hal_table_reg_cmodel_exit();
#endif

#ifndef __KERNEL__
    osal_mux_destroy(&g_cfgmux);
#endif
    ghal_reg_table_init = FALSE;
    
    return CMM_ERR_OK;
}
