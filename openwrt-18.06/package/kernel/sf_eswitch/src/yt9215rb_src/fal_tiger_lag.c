/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "yt_lag.h"
#include "fal_tiger_lag.h"
#include "hal_mem.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_mem.h"

/*
 * Symbol Definition
 */

/*
 * Macro Declaration
 */

/*
 * Data Declaration
 */

/*
 * Function Declaration
 */

yt_ret_t fal_tiger_lag_hash_sel_set(yt_unit_t unit, uint8_t hash_mask)
{
    link_agg_hash_ctrl_t  entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, LINK_AGG_HASH_CTRLm, 0, sizeof(link_agg_hash_ctrl_t), &entry), ret);
    HAL_FIELD_SET (LINK_AGG_HASH_CTRLm, LINK_AGG_HASH_CTRL_HASH_FIELD_SELf, &entry, hash_mask);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, LINK_AGG_HASH_CTRLm, 0, sizeof(link_agg_hash_ctrl_t), &entry), ret);
   
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_lag_hash_sel_get(yt_unit_t unit, uint8_t *p_hash_mask)
{
    link_agg_hash_ctrl_t  entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, LINK_AGG_HASH_CTRLm, 0, sizeof(link_agg_hash_ctrl_t), &entry), ret);
    HAL_FIELD_GET(LINK_AGG_HASH_CTRLm, LINK_AGG_HASH_CTRL_HASH_FIELD_SELf, &entry, p_hash_mask);
   
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_lag_group_port_set(yt_unit_t unit, uint8_t groupId, yt_port_mask_t member_portmask)
{
    link_agg_groupn_t group_entry;
    link_agg_membern_t member_entry[LAG_MEM_NUM_PERGRP];
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t index = 0;
    uint8_t macid = 0;
    uint32_t  mask;
    yt_port_mask_t macmask;
    uint8_t idx_offset = 0;
    yt_link_agg_group_t laginfo;

    CMM_PARAM_CHK((FAL_MAX_LAG_NUM - 1< groupId), CMM_ERR_INPUT);

    /*check if port mask conflic with another lag*/
    fal_tiger_lag_group_info_get(unit, (groupId == 1 ? 0:1), &laginfo);
    if(member_portmask.portbits[0] & laginfo.member_portmask)
    {
        return CMM_ERR_SAMEENTRY_EXIST;
    }

    osal_memset(member_entry, 0, sizeof(member_entry));
    CAL_YTPLIST_TO_MLIST(unit, member_portmask, macmask);
    
    mask = macmask.portbits[0];

    while(mask)
    {
        if((mask & 0x1) != 0)
        {
            if(index >= LAG_MEM_NUM_PERGRP)
            {
                return CMM_ERR_TABLE_FULL;
            }
            HAL_FIELD_SET(LINK_AGG_MEMBERNm, LINK_AGG_MEMBERN_PORTf, &member_entry[index], macid);
            index++;
        }
        macid++;
        mask >>= 1;
    }

    HAL_FIELD_SET(LINK_AGG_GROUPNm, LINK_AGG_GROUPN_MEMBER_NUMf, &group_entry, index);

    idx_offset = (groupId == 1) ? LAG_MEM_NUM_PERGRP : 0;
    
    for(index=0; index<LAG_MEM_NUM_PERGRP; index++)
    {
        CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, LINK_AGG_MEMBERNm, index+idx_offset, sizeof(link_agg_membern_t), &member_entry[index]), ret); 
    }
    
    HAL_FIELD_SET(LINK_AGG_GROUPNm, LINK_AGG_GROUPN_PORT_MASKf, &group_entry, macmask.portbits[0]);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, LINK_AGG_GROUPNm, groupId, sizeof(link_agg_groupn_t), &group_entry), ret);  
   
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_lag_group_info_get(yt_unit_t unit, uint8_t groupId, yt_link_agg_group_t *p_laginfo)
{
    link_agg_groupn_t group_entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_mask_t macmask;
    yt_port_mask_t portmask;

    CMM_PARAM_CHK((FAL_MAX_LAG_NUM - 1< groupId), CMM_ERR_INPUT);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, LINK_AGG_GROUPNm, groupId, sizeof(link_agg_groupn_t), &group_entry), ret); 
    HAL_FIELD_GET(LINK_AGG_GROUPNm, LINK_AGG_GROUPN_MEMBER_NUMf, &group_entry, &(p_laginfo->member_num));
    HAL_FIELD_GET(LINK_AGG_GROUPNm, LINK_AGG_GROUPN_PORT_MASKf, &group_entry, macmask.portbits);
    CAL_MLIST_TO_YTPLIST(unit, macmask, portmask);
    p_laginfo->member_portmask = portmask.portbits[0];
   
    return CMM_ERR_OK;
}
