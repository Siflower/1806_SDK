/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_tiger_rma.h"
#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"

yt_ret_t  fal_tiger_rma_action_set(yt_unit_t unit, yt_rma_da_t  macda, yt_rma_action_t action)
{
    rma_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, RMA_CTRLNm, macda, sizeof(rma_ctrln_t), &entry), ret);
    switch(action)
    {
        case RMA_ACTION_FWD:
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_COPY_TO_CPUf, &entry, 0);
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_DROPf, &entry, 0);
            break;
        case RMA_ACTION_TRAP:
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_COPY_TO_CPUf, &entry, 1);
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_DROPf, &entry, 1);
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_CPU_CODEf, &entry, 0x3f);
            break;
        case RMA_ACTION_COPY:
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_COPY_TO_CPUf, &entry, 1);
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_DROPf, &entry, 0);
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_CPU_CODEf, &entry, 0x3f);
            break;
        case RMA_ACTION_DROP:
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_COPY_TO_CPUf, &entry, 0);
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_DROPf, &entry, 1);
            break;
        default:
            return CMM_ERR_INPUT;
    }
    
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, RMA_CTRLNm, macda, sizeof(rma_ctrln_t), &entry), ret); 
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_rma_action_get(yt_unit_t unit, yt_rma_da_t  macda, yt_rma_action_t *pAction)
{
    rma_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t copy_to_cpu;
    uint32_t drop;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, RMA_CTRLNm, macda, sizeof(rma_ctrln_t), &entry), ret);
    HAL_FIELD_GET(RMA_CTRLNm, RMA_CTRLN_COPY_TO_CPUf, &entry, &copy_to_cpu);
    HAL_FIELD_GET(RMA_CTRLNm, RMA_CTRLN_DROPf, &entry, &drop);
    if(copy_to_cpu == 1)
    {
        if(drop == 1)
        {
            *pAction = RMA_ACTION_TRAP;
        }
        else
        {
            *pAction = RMA_ACTION_COPY;
        }
    }
    else if(copy_to_cpu == 0)
    {
        if(drop == 1)
        {
            *pAction = RMA_ACTION_DROP;
        }
        else
        {
            *pAction = RMA_ACTION_FWD;
        }
    }

    return CMM_ERR_OK;
}


yt_ret_t fal_tiger_rma_bypass_port_isolation_set(yt_unit_t unit, yt_rma_da_t  macda, yt_enable_t enable)
{
    rma_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, RMA_CTRLNm, macda, sizeof(rma_ctrln_t), &entry), ret);
    HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_PORT_ISOLATE_LEAKYf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, RMA_CTRLNm, macda, sizeof(rma_ctrln_t), &entry), ret);
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rma_bypass_port_isolation_get(yt_unit_t unit, yt_rma_da_t  macda, yt_enable_t *pEnable)
{
    rma_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t enable;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, RMA_CTRLNm, macda, sizeof(rma_ctrln_t), &entry), ret);
    HAL_FIELD_GET(RMA_CTRLNm, RMA_CTRLN_PORT_ISOLATE_LEAKYf, &entry, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_rma_bypass_vlan_filter_set(yt_unit_t unit, yt_rma_da_t macda, yt_enable_t enable)
{
    rma_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, RMA_CTRLNm, macda, sizeof(rma_ctrln_t), &entry), ret);
    HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_VLAN_LEAKYf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, RMA_CTRLNm, macda, sizeof(rma_ctrln_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_rma_bypass_vlan_filter_get(yt_unit_t unit, yt_rma_da_t macda, yt_enable_t *pEnable)
{
    rma_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t enable;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, RMA_CTRLNm, macda, sizeof(rma_ctrln_t), &entry), ret);
    HAL_FIELD_GET(RMA_CTRLNm, RMA_CTRLN_VLAN_LEAKYf, &entry, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}
