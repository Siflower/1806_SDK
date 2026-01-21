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

yt_ret_t  fal_tiger_rma_action_set(yt_unit_t unit, yt_rma_da_t  macda, yt_act_type_t action)
{
    rma_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, RMA_CTRLNm, macda, sizeof(rma_ctrln_t), &entry), ret);
    switch(action)
    {
        case FWD_TYPE_FWD:
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_COPY_TO_CPUf, &entry, 0);
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_DROPf, &entry, 0);
            break;
        case FWD_TYPE_TRAP:
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_COPY_TO_CPUf, &entry, 1);
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_DROPf, &entry, 1);
            break;
        case FWD_TYPE_COPY:
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_COPY_TO_CPUf, &entry, 1);
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_DROPf, &entry, 0);
            break;
        case FWD_TYPE_DROP:
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_COPY_TO_CPUf, &entry, 0);
            HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_DROPf, &entry, 1);
            break;
        default:
            return CMM_ERR_INPUT;
    }
    
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, RMA_CTRLNm, macda, sizeof(rma_ctrln_t), &entry), ret); 
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_rma_action_get(yt_unit_t unit, yt_rma_da_t  macda, yt_act_type_t *pAction)
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
            *pAction = ACT_TYPE_TRAP;
        }
        else
        {
            *pAction = ACT_TYPE_COPY;
        }
    }
    else if(copy_to_cpu == 0)
    {
        if(drop == 1)
        {
            *pAction = ACT_TYPE_DROP;
        }
        else
        {
            *pAction = ACT_TYPE_FWD;
        }
    }
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_rma_cpu_code_set(yt_unit_t unit, yt_rma_da_t  macda, yt_rma_cpu_code_t cpuCode)
{
    rma_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    cpuCode = 0x37;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, RMA_CTRLNm, macda, sizeof(rma_ctrln_t), &entry), ret);
    HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_CPU_CODEf, &entry, cpuCode);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, RMA_CTRLNm, macda, sizeof(rma_ctrln_t), &entry), ret); 
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_rma_cpu_code_get(yt_unit_t unit, yt_rma_da_t  macda, yt_rma_cpu_code_t *pCpuCode)
{
    rma_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, RMA_CTRLNm, macda, sizeof(rma_ctrln_t), &entry), ret);
    HAL_FIELD_GET(RMA_CTRLNm, RMA_CTRLN_CPU_CODEf, &entry, pCpuCode);
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_rma_bypass_set(yt_unit_t unit, yt_rma_da_t  macda, yt_rma_bypass_t rmaBypass)
{
    rma_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    l2_mc_unknown_act_ctrl_t mcAct;

    osal_memset(&mcAct, sizeof(l2_mc_unknown_act_ctrl_t), 0, sizeof(l2_mc_unknown_act_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, RMA_CTRLNm, macda, sizeof(rma_ctrln_t), &entry), ret);
    if ((rmaBypass.setBitMask & YT_RMA_BYPASS_PORT_ISO_BIT) != 0)
    {
        HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_PORT_ISOLATE_LEAKYf, &entry, rmaBypass.bypass_port_isolation);
    }
    if ((rmaBypass.setBitMask & YT_RMA_BYPASS_VLAN_FILTER_BIT) != 0)
    {
        HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_VLAN_LEAKYf, &entry, rmaBypass.bypass_vlan_filter);
    }
    if ((rmaBypass.setBitMask & YT_RMA_BYPASS_STORM_BIT) != 0)
    {
        HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_BYPASS_STORMf, &entry, rmaBypass.bypass_storm);
    }
    if ((rmaBypass.setBitMask & YT_RMA_BYPASS_FLOW_METER_BIT) != 0)
    {
        HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_BYPASS_FLOW_METERf, &entry, rmaBypass.bypass_flow_meter);
    }
    if ((rmaBypass.setBitMask & YT_RMA_BYPASS_PORT_METER_BIT) != 0)
    {
        HAL_FIELD_SET(RMA_CTRLNm, RMA_CTRLN_BYPASS_PORT_METERf, &entry, rmaBypass.bypass_port_meter);
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, RMA_CTRLNm, macda, sizeof(rma_ctrln_t), &entry), ret); 

    if((rmaBypass.setBitMask & YT_RMA_BYPASS_UNKNOWN_MC_BIT) != 0)
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_MC_UNKNOWN_ACT_CTRLm, 0, sizeof(l2_mc_unknown_act_ctrl_t), &mcAct), ret);
        HAL_FIELD_SET(L2_MC_UNKNOWN_ACT_CTRLm, L2_MC_UNKNOWN_ACT_CTRL_L2_MC_UNKNOWN_DROP_RMABYPASSf, &mcAct, rmaBypass.bypass_unknown_mc);
        CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_MC_UNKNOWN_ACT_CTRLm, 0, sizeof(l2_mc_unknown_act_ctrl_t), &mcAct), ret);
    }
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_rma_bypass_get(yt_unit_t unit, yt_rma_da_t  macda, yt_rma_bypass_t *pRmaBypass)
{
    rma_ctrln_t entry;
    l2_mc_unknown_act_ctrl_t mcAct;
    uint32_t enable = 0;
    cmm_err_t ret = CMM_ERR_OK;

    osal_memset(&mcAct, sizeof(l2_mc_unknown_act_ctrl_t), 0, sizeof(l2_mc_unknown_act_ctrl_t));
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, RMA_CTRLNm, macda, sizeof(rma_ctrln_t), &entry), ret);
    if ((pRmaBypass->setBitMask & YT_RMA_BYPASS_PORT_ISO_BIT) != 0)
    {
        HAL_FIELD_GET(RMA_CTRLNm, RMA_CTRLN_PORT_ISOLATE_LEAKYf, &entry, &enable);
        pRmaBypass->bypass_port_isolation = enable ? YT_ENABLE : YT_DISABLE;
    }
    if ((pRmaBypass->setBitMask & YT_RMA_BYPASS_VLAN_FILTER_BIT) != 0)
    {
        HAL_FIELD_GET(RMA_CTRLNm, RMA_CTRLN_VLAN_LEAKYf, &entry, &enable);
        pRmaBypass->bypass_vlan_filter = enable ? YT_ENABLE : YT_DISABLE;
    }
    if ((pRmaBypass->setBitMask & YT_RMA_BYPASS_STORM_BIT) != 0)
    {
        HAL_FIELD_GET(RMA_CTRLNm, RMA_CTRLN_BYPASS_STORMf, &entry, &enable);
        pRmaBypass->bypass_storm = enable ? YT_ENABLE : YT_DISABLE;
    }
    if ((pRmaBypass->setBitMask & YT_RMA_BYPASS_FLOW_METER_BIT) != 0)
    {
        HAL_FIELD_GET(RMA_CTRLNm, RMA_CTRLN_BYPASS_FLOW_METERf, &entry, &enable);
        pRmaBypass->bypass_flow_meter = enable ? YT_ENABLE : YT_DISABLE;
    }
    if ((pRmaBypass->setBitMask & YT_RMA_BYPASS_PORT_METER_BIT) != 0)
    {
        HAL_FIELD_GET(RMA_CTRLNm, RMA_CTRLN_BYPASS_PORT_METERf, &entry, &enable);
        pRmaBypass->bypass_port_meter = enable ? YT_ENABLE : YT_DISABLE;
    }
    if ((pRmaBypass->setBitMask & YT_RMA_BYPASS_UNKNOWN_MC_BIT) != 0)
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_MC_UNKNOWN_ACT_CTRLm, 0, sizeof(l2_mc_unknown_act_ctrl_t), &mcAct), ret);
        HAL_FIELD_GET(L2_MC_UNKNOWN_ACT_CTRLm, L2_MC_UNKNOWN_ACT_CTRL_L2_MC_UNKNOWN_DROP_RMABYPASSf, &mcAct, &enable);
        pRmaBypass->bypass_unknown_mc = enable ? YT_ENABLE : YT_DISABLE;
    }
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_rma_sa_learn_dis_set(yt_unit_t unit, yt_rma_da_t macda, yt_enable_t enable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(macda);
    CMM_UNUSED_PARAM(enable);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_rma_sa_learn_dis_get(yt_unit_t unit, yt_rma_da_t macda, yt_enable_t *pEnable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(macda);
    CMM_UNUSED_PARAM(pEnable);

    return CMM_ERR_NOT_SUPPORT;
}