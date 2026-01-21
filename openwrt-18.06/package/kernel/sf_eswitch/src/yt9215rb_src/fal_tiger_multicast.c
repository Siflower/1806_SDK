/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_multicast.c
*
* @brief x
*
********************************************************************************
*/
/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_tiger_multicast.h"
#include "fal_tiger_l2.h"

#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"

/**
 * @internal      fal_tiger_multicast_vlan_add
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_multicast_vlan_add(yt_unit_t unit, yt_vlan_t vid)
{
    cmm_err_t ret           = CMM_ERR_OK;
    multi_vlan_tbl_t multi_vlan_tbl;
    uint8_t i, valid_num = 0;
    uint32_t mulit_en0;
    uint32_t mulit_en1;
    uint32_t fid0;
    uint32_t fid1;

    for(i = 0; (FAL_MAX_MULTICAST_VLAN_NUM/2) > i; i++)
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, MULTI_VLAN_TBLm, i, sizeof(multi_vlan_tbl_t), &multi_vlan_tbl), ret);
        HAL_FIELD_GET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_MULTI_EN0f, &multi_vlan_tbl, &mulit_en0);
        HAL_FIELD_GET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_MULTI_EN1f, &multi_vlan_tbl, &mulit_en1);
        HAL_FIELD_GET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_FID0f, &multi_vlan_tbl, &fid0);
        HAL_FIELD_GET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_FID1f, &multi_vlan_tbl, &fid1);
        
        if((mulit_en0 && (vid == fid0))
            || (mulit_en1 && (vid == fid1)))
        {
            return CMM_ERR_SAMEENTRY_EXIST;
        }

        if(!mulit_en0 || !mulit_en1)
        {
            break;
        }
        
        if(mulit_en0) {valid_num++;}
        if(mulit_en1) {valid_num++;}
    }

    if(FAL_MAX_MULTICAST_VLAN_NUM == valid_num)
    {
        return CMM_ERR_ENTRY_FULL;
    }

    if(!mulit_en0)
    {
        mulit_en0 = TRUE;
        fid0      = vid;
    }
    else if(!mulit_en1)
    {
        mulit_en1 = TRUE;
        fid1      = vid;
    }
    HAL_FIELD_SET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_MULTI_EN0f, &multi_vlan_tbl, mulit_en0);
    HAL_FIELD_SET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_MULTI_EN1f, &multi_vlan_tbl, mulit_en1);
    HAL_FIELD_SET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_FID0f, &multi_vlan_tbl, fid0);
    HAL_FIELD_SET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_FID1f, &multi_vlan_tbl, fid1);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, MULTI_VLAN_TBLm, i, sizeof(multi_vlan_tbl_t), &multi_vlan_tbl), ret);

    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_multicast_vlan_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pvlanarray          -Multicast vlan info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_multicast_vlan_get(yt_unit_t unit, yt_multi_vlan_t *pvlanarray)
{
    cmm_err_t ret           = CMM_ERR_OK;
    multi_vlan_tbl_t multi_vlan_tbl;
    uint8_t i, valid_num = 0;
    uint32_t mulit_en0;
    uint32_t mulit_en1;
    uint32_t fid0;
    uint32_t fid1;

    for(i = 0; (FAL_MAX_MULTICAST_VLAN_NUM/2) > i; i++)
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, MULTI_VLAN_TBLm, i, sizeof(multi_vlan_tbl_t), &multi_vlan_tbl), ret);
        HAL_FIELD_GET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_MULTI_EN0f, &multi_vlan_tbl, &mulit_en0);
        HAL_FIELD_GET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_MULTI_EN1f, &multi_vlan_tbl, &mulit_en1);
        HAL_FIELD_GET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_FID0f, &multi_vlan_tbl, &fid0);
        HAL_FIELD_GET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_FID1f, &multi_vlan_tbl, &fid1);
        
        if(mulit_en0)
        {
            pvlanarray->vlanid[valid_num++] = fid0;
        }
        if(mulit_en1)
        {
            pvlanarray->vlanid[valid_num++] = fid1;
        }
    }

    if(0 == valid_num)
    {
        return CMM_ERR_ENTRY_NOT_FOUND;
    }

    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_multicast_vlan_del
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_multicast_vlan_del(yt_unit_t unit, yt_vlan_t vid)
{
    cmm_err_t ret           = CMM_ERR_OK;
    multi_vlan_tbl_t multi_vlan_tbl;
    uint8_t i,  valid_num = 0;
    uint32_t mulit_en0;
    uint32_t mulit_en1;
    uint32_t fid0;
    uint32_t fid1;

    for(i = 0; (FAL_MAX_MULTICAST_VLAN_NUM/2) > i; i++)
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, MULTI_VLAN_TBLm, i, sizeof(multi_vlan_tbl_t), &multi_vlan_tbl), ret);
        HAL_FIELD_GET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_MULTI_EN0f, &multi_vlan_tbl, &mulit_en0);
        HAL_FIELD_GET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_MULTI_EN1f, &multi_vlan_tbl, &mulit_en1);
        HAL_FIELD_GET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_FID0f, &multi_vlan_tbl, &fid0);
        HAL_FIELD_GET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_FID1f, &multi_vlan_tbl, &fid1);
        
        if((mulit_en0 && (vid == fid0))
            || (mulit_en1 && (vid == fid1)))
        {
            valid_num++;
            break;
        }
    }

    if(0 == valid_num)
    {
        return CMM_ERR_ENTRY_NOT_FOUND;
    }

    if(mulit_en0 && (vid == fid0))
    {
        mulit_en0 = FALSE;
        fid0      = 0;
    }
    else if(mulit_en1 && (vid == fid1))
    {
        mulit_en1 = FALSE;
        fid1      = 0;
    }

    HAL_FIELD_SET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_MULTI_EN0f, &multi_vlan_tbl, mulit_en0);
    HAL_FIELD_SET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_MULTI_EN1f, &multi_vlan_tbl, mulit_en1);
    HAL_FIELD_SET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_FID0f, &multi_vlan_tbl, fid0);
    HAL_FIELD_SET(MULTI_VLAN_TBLm, MULTI_VLAN_TBL_FID1f, &multi_vlan_tbl, fid1);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, MULTI_VLAN_TBLm, i, sizeof(multi_vlan_tbl_t), &multi_vlan_tbl), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_multicast_ipv4_add(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv4_info_t *pIpinfo, yt_port_mask_t portMask)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(mode);
    CMM_UNUSED_PARAM(pIpinfo);
    CMM_UNUSED_PARAM(portMask);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_ipv6_add(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv6_info_t *pIpinfo, yt_port_mask_t portMask)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(mode);
    CMM_UNUSED_PARAM(pIpinfo);
    CMM_UNUSED_PARAM(portMask);

    return CMM_ERR_NOT_SUPPORT;
}


yt_ret_t fal_tiger_multicast_ipv4_del(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv4_info_t *pIpinfo)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(mode);
    CMM_UNUSED_PARAM(pIpinfo);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_ipv6_del(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv6_info_t *pIpinfo)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(mode);
    CMM_UNUSED_PARAM(pIpinfo);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_ipv4_withIpAndVid_get(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv4_info_t *pIpinfo,  yt_mcast_ipv4_data_t *pData)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(mode);
    CMM_UNUSED_PARAM(pIpinfo);
    CMM_UNUSED_PARAM(pData);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_ipv4_withindex_get(yt_unit_t unit, uint16_t index, yt_mcast_ipv4_data_t *pData)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(index);
    CMM_UNUSED_PARAM(pData);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_ipv4_withindex_getnext(yt_unit_t unit, uint16_t index, uint16_t *pNextIdx, yt_mcast_ipv4_data_t *pData)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(index);
    CMM_UNUSED_PARAM(pNextIdx);
    CMM_UNUSED_PARAM(pData);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_ipv6_withIpAndVid_get(yt_unit_t unit, yt_mcast_mode_t mode, const yt_mcast_ipv6_info_t *pIpinfo,  yt_mcast_ipv6_data_t *pData)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(mode);
    CMM_UNUSED_PARAM(pIpinfo);
    CMM_UNUSED_PARAM(pData);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_ipv6_withindex_get(yt_unit_t unit, uint16_t index, yt_mcast_ipv6_data_t *pData)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(index);
    CMM_UNUSED_PARAM(pData);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_ipv6_withindex_getnext(yt_unit_t unit, uint16_t index, uint16_t *pNextIdx, yt_mcast_ipv6_data_t *pData)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(index);
    CMM_UNUSED_PARAM(pNextIdx);
    CMM_UNUSED_PARAM(pData);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_macaddr_withMacAndVid_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_mcast_mac_data_t *pData)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(vid);
    CMM_UNUSED_PARAM(macAddr);
    CMM_UNUSED_PARAM(pData);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_macaddr_withindex_get(yt_unit_t unit, uint16_t index, yt_mcast_mac_data_t *pData)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(index);
    CMM_UNUSED_PARAM(pData);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_macaddr_withindex_getnext(yt_unit_t unit, uint16_t index,  uint16_t *pNextIdx, yt_mcast_mac_data_t *pData)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(index);
    CMM_UNUSED_PARAM(pNextIdx);
    CMM_UNUSED_PARAM(pData);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_all_ipmc_flush(yt_unit_t unit)
{
    CMM_UNUSED_PARAM(unit);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_vid_ipmc_flush(yt_unit_t unit, yt_vlan_t vid)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(vid);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_hwmc_flush_all(yt_unit_t unit, yt_mcast_hw_mode_t mode)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(mode);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_hwmc_flush_by_vid(yt_unit_t unit, yt_mcast_hw_mode_t mode, yt_vlan_t vid)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(mode);
    CMM_UNUSED_PARAM(vid);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_hwmc_flush_by_portmask(yt_unit_t unit, yt_mcast_hw_mode_t mode, yt_port_mask_t portMask)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(mode);
    CMM_UNUSED_PARAM(portMask);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_hwmc_mac_del(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_port_mask_t portMask)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(vid);
    CMM_UNUSED_PARAM(macAddr);
    CMM_UNUSED_PARAM(portMask);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_multicast_hwmc_ipv4_del(yt_unit_t unit, yt_vlan_t vid, yt_ip_addr_t gip, yt_port_mask_t portMask)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(vid);
    CMM_UNUSED_PARAM(gip);
    CMM_UNUSED_PARAM(portMask);

    return CMM_ERR_NOT_SUPPORT;
}
