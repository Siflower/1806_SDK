/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_tiger_stat.h"
#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"

yt_ret_t fal_tiger_stat_mib_init(yt_unit_t unit)
{
    CMM_UNUSED_PARAM(unit);
    STAT_LOCK_INIT();

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_stat_mib_enable_set (yt_unit_t unit, yt_enable_t enable)
{
    global_ctrl1_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    
    osal_memset(&entry, 0, sizeof(global_ctrl1_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, GLOBAL_CTRL1m, 0, sizeof(global_ctrl1_t), &entry), ret);
    HAL_FIELD_SET(GLOBAL_CTRL1m, GLOBAL_CTRL1_MIB_ENf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, GLOBAL_CTRL1m, 0, sizeof(global_ctrl1_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_stat_mib_enable_get (yt_unit_t unit, yt_enable_t *pEnable)
{
    global_ctrl1_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t enable;

    if(NULL == pEnable)
        return CMM_ERR_NULL_POINT;

    osal_memset(&entry, 0, sizeof(global_ctrl1_t));
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, GLOBAL_CTRL1m, 0, sizeof(global_ctrl1_t), &entry), ret);
    HAL_FIELD_GET(GLOBAL_CTRL1m, GLOBAL_CTRL1_MIB_ENf, &entry, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;   

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_stat_mib_clear (yt_unit_t unit, yt_port_t port)
{
    uint32_t ctrl_data = 0;
    yt_port_t port_mac_id;
    cmm_err_t ret = CMM_ERR_OK;

    port_mac_id = CAL_YTP_TO_MAC(unit,port);
    
    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_OP_ADDR, &ctrl_data), ret);
    ctrl_data = port_mac_id << 3 | 0x2 << 0;
    CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, MIB_OP_ADDR, ctrl_data), ret);
    ctrl_data |= 1 << 30;
    CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, MIB_OP_ADDR, ctrl_data), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_stat_mib_clear_all (yt_unit_t unit)
{
    uint32_t ctrl_data = 0;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_OP_ADDR, &ctrl_data), ret);
    ctrl_data = 0;
    CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, MIB_OP_ADDR, ctrl_data), ret);
    ctrl_data |= 1 << 30;
    CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, MIB_OP_ADDR, ctrl_data), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_stat_mib_port_get (yt_unit_t unit, yt_port_t port, yt_stat_mib_port_cnt_t *pcnt)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint32 *port_cnt;
    uint32_t mib_index = 0;
    uint32 mib_data = 0;
    uint32 tmp_data = 0;
    uint64 result_data = 0;
    yt_port_t port_mac_id;

    if(NULL == pcnt)
        return CMM_ERR_NULL_POINT;

    port_mac_id = CAL_YTP_TO_MAC(unit,port);
    port_cnt = (uint32 *)pcnt;
    for(mib_index = 0; mib_index < YT_STAT_MAX; mib_index ++ )
    {
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * mib_index, &mib_data), ret);
        if(mib_index == RX_OKBYTE || mib_index == RX_NOT_OKBYTE || mib_index == TX_OKBYTE)
        {
            CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 *(mib_index + 1), &tmp_data), ret);
            result_data = tmp_data;

            *(uint64 *)port_cnt = (result_data << 32) |mib_data;
            mib_index += 1;
            result_data = 0;
            port_cnt += 2;
        }
        else if(mib_index == RX_UNICAST)
        {
            *port_cnt = pcnt->RX_64B + pcnt->RX_65_127B + pcnt->RX_128_255B + pcnt->RX_256_511B + pcnt->RX_512_1023B 
                            + pcnt->RX_1024_1518B + pcnt->RX_JUMBO - pcnt->RX_MULTICAST - pcnt->RX_BROADCAST - pcnt->RX_PAUSE - pcnt->RX_OAM_COUNTER;
            port_cnt += 1;
        }
        else if(mib_index == TX_UNICAST)
        {
            *port_cnt = pcnt->TX_OK_PKT - pcnt->TX_MULTICAST - pcnt->TX_BROADCAST - pcnt->TX_PAUSE - pcnt->TX_OAM_COUNTER;
            port_cnt += 1;
        }
        else if(mib_index == RX_JUMBO)
        {
            *port_cnt = mib_data;
            port_cnt += 1;
            mib_index += 1;
        }
        else if(mib_index == TX_JUMBO)
        {
            *port_cnt = mib_data;
            port_cnt += 1;
            mib_index += 1;
        }
        else
        {
            *port_cnt = mib_data;
            port_cnt += 1;
        }
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_stat_mib_port_index_get (yt_unit_t unit, yt_port_t port, uint32_t index, uint64 *pcnt)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint32 mib_data = 0;
    uint32 tmp_data = 0;
    uint64 result_data = 0;
    yt_port_t port_mac_id;

    if(NULL == pcnt)
        return CMM_ERR_NULL_POINT;

    if(index >= YT_STAT_MAX)
        return CMM_ERR_NOT_SUPPORT;

    port_mac_id = CAL_YTP_TO_MAC(unit,port);

    if (index == RX_UNICAST)
    {
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * RX_64B, &mib_data), ret);
        result_data += mib_data;
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * RX_65_127B, &mib_data), ret);
        result_data += mib_data;
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * RX_128_255B, &mib_data), ret);
        result_data += mib_data;
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * RX_256_511B, &mib_data), ret);
        result_data += mib_data;
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * RX_512_1023B, &mib_data), ret);
        result_data += mib_data;
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * RX_1024_1518B, &mib_data), ret);
        result_data += mib_data;
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * RX_JUMBO, &mib_data), ret);
        result_data += mib_data;
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * RX_BROADCAST, &mib_data), ret);
        result_data -= mib_data;
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * RX_MULTICAST, &mib_data), ret);
        result_data -= mib_data;
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * RX_PAUSE, &mib_data), ret);
        result_data -= mib_data;
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * RX_OAM_COUNTER, &mib_data), ret);
        result_data -= mib_data;
        *pcnt = result_data;
    } 
    else if (index == TX_UNICAST)
    {
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * TX_OK_PKT, &mib_data), ret);
        result_data += mib_data;
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * TX_BROADCAST, &mib_data), ret);
        result_data -= mib_data;
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * TX_MULTICAST, &mib_data), ret);
        result_data -= mib_data;
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * TX_PAUSE, &mib_data), ret);
        result_data -= mib_data;
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * TX_OAM_COUNTER, &mib_data), ret);
        result_data -= mib_data;
        *pcnt = result_data;
    } 
    else
    {
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * index, &mib_data), ret);
        if(index == RX_OKBYTE || index == RX_NOT_OKBYTE || index == TX_OKBYTE)
        {
            CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, MIB_TBL_BASE_ADDR0 + 0x100 * port_mac_id + 0x4 * (index + 1), &tmp_data), ret);
            result_data |= tmp_data;
        }
        *pcnt = (result_data << 32) | mib_data;
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_stat_flow_enable_set (yt_unit_t unit, uint32_t flow_id, yt_enable_t enable)
{
    flow_stats_cfg_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_PARAM_CHK((CAL_FLOW_NUM_MAX <= flow_id), CMM_ERR_INPUT);
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, FLOW_STATS_CFG_TBLm, flow_id, sizeof(flow_stats_cfg_tbl_t), &entry), ret);
    HAL_FIELD_SET(FLOW_STATS_CFG_TBLm, FLOW_STATS_CFG_TBL_ENf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, FLOW_STATS_CFG_TBLm, flow_id, sizeof(flow_stats_cfg_tbl_t), &entry), ret);
   
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_stat_flow_enable_get (yt_unit_t unit, uint32_t flow_id, yt_enable_t *pEnable)
{
    flow_stats_cfg_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t enable;

    CMM_PARAM_CHK((CAL_FLOW_NUM_MAX <= flow_id), CMM_ERR_INPUT);
    
    if(NULL == pEnable)
        return CMM_ERR_NULL_POINT;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, FLOW_STATS_CFG_TBLm, flow_id, sizeof(flow_stats_cfg_tbl_t), &entry), ret);
    HAL_FIELD_GET(FLOW_STATS_CFG_TBLm, FLOW_STATS_CFG_TBL_ENf, &entry, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;
   
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_stat_flow_mode_set (yt_unit_t unit, uint32_t flow_id, yt_stat_type_t type, yt_stat_mode_t mode)
{
    flow_stats_cfg_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_PARAM_CHK((CAL_FLOW_NUM_MAX <= flow_id), CMM_ERR_INPUT);
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, FLOW_STATS_CFG_TBLm, flow_id, sizeof(flow_stats_cfg_tbl_t), &entry), ret);
    HAL_FIELD_SET(FLOW_STATS_CFG_TBLm, FLOW_STATS_CFG_TBL_SELf, &entry, type);
    HAL_FIELD_SET(FLOW_STATS_CFG_TBLm, FLOW_STATS_CFG_TBL_MODEf, &entry, mode);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, FLOW_STATS_CFG_TBLm, flow_id, sizeof(flow_stats_cfg_tbl_t), &entry), ret);
   
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_stat_flow_mode_get (yt_unit_t unit, uint32_t flow_id, yt_stat_type_t *ptype, yt_stat_mode_t *pmode)
{
    flow_stats_cfg_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t type;
    uint32_t mode;

    CMM_PARAM_CHK((CAL_FLOW_NUM_MAX <= flow_id), CMM_ERR_INPUT);
    
    if(NULL == ptype || NULL == pmode)
        return CMM_ERR_NULL_POINT;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, FLOW_STATS_CFG_TBLm, flow_id, sizeof(flow_stats_cfg_tbl_t), &entry), ret); 
    HAL_FIELD_GET(FLOW_STATS_CFG_TBLm, FLOW_STATS_CFG_TBL_SELf, &entry, &type);
    HAL_FIELD_GET(FLOW_STATS_CFG_TBLm, FLOW_STATS_CFG_TBL_MODEf, &entry, &mode);
    *ptype = type;
    *pmode = mode;
   
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_stat_flow_count_set (yt_unit_t unit, uint32_t flow_id, uint64 count)
{
    flow_stats_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_PARAM_CHK((CAL_FLOW_NUM_MAX <= flow_id), CMM_ERR_INPUT);
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, FLOW_STATS_TBLm, flow_id, sizeof(flow_stats_tbl_t), &entry), ret);
    HAL_FIELD_SET(FLOW_STATS_TBLm, FLOW_STATS_TBL_CNT1f, &entry, (count >> 32) & 0xFFFFFFFF);
    HAL_FIELD_SET(FLOW_STATS_TBLm, FLOW_STATS_TBL_CNT0f, &entry, count & 0xFFFFFFFF);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, FLOW_STATS_TBLm, flow_id, sizeof(flow_stats_tbl_t), &entry), ret);
   
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_stat_flow_count_get (yt_unit_t unit, uint32_t flow_id, uint64 *count)
{
    flow_stats_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t cnt0;
    uint32_t cnt1;

    CMM_PARAM_CHK((CAL_FLOW_NUM_MAX <= flow_id), CMM_ERR_INPUT);
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, FLOW_STATS_TBLm, flow_id, sizeof(flow_stats_tbl_t), &entry), ret);  
    HAL_FIELD_GET(FLOW_STATS_TBLm, FLOW_STATS_TBL_CNT1f, &entry, &cnt1);
    HAL_FIELD_GET(FLOW_STATS_TBLm, FLOW_STATS_TBL_CNT0f, &entry, &cnt0);
    *count = ((uint64)cnt1 << 32) | cnt0;
   
    return CMM_ERR_OK;
}
