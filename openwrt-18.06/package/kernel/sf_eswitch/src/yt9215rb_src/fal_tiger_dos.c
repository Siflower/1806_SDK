/******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
*******************************************************************************
*  File Name     : fal_tiger_dos.c
*  Version       : Initial Draft
*  Author        :
*  Created       : 2021/8/11
*  Last Modified :
*  Description   : Denial of Service
*  History:
* 
*       1.  Date            : 2021/8/11
*           Author          :
*           Modification : Created file
*
******************************************************************************/

/**************************************************
 *      Include header files                                               *
 **************************************************/
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_tiger_dos.h"
#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"
/**************************************************
 *      Constants or macros Declaration                          *
 **************************************************/

/**************************************************
 *      Global variables                                                   *
 **************************************************/

/**************************************************
 *      Functions Declaration                                           *
 **************************************************/



/**************************************************
 *      Functions implementations                                   *
 **************************************************/

static uint32_t  fal_tiger_dos_tcp_flagsn_set(yt_unit_t unit, yt_dos_type_t type, yt_enable_t enable)
{
    dos_tcp_flags0_t dos_tcp_flagsn;
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t memid = 0;

    switch(type)
    {
        case DOS_TYPE_TCP_SYNRST_SCAN:
           memid = 4;
           break;
        case DOS_TYPE_TCP_SYNFIN_SCAN:
           memid = 3;
           break;
        case DOS_TYPE_TCP_XMAS_SCAN:
           memid = 0;
           break;
        case DOS_TYPE_TCP_NULL_SCAN:
           memid = 1;
           break;
        case DOS_TYPE_TCP_SYN_PORTLESS1024:
           memid = 2;
           break;
        case DOS_TYPE_TCP_ALLFLAGS_SCAN:
           memid = 5;
           break;
        default:
           return CMM_ERR_NOT_SUPPORT;
    }

    if(enable == YT_ENABLE)
    {
        switch(type)
        {
            case DOS_TYPE_TCP_SYNRST_SCAN:
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORTf, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0f, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGSf, &dos_tcp_flagsn, 0x6);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORT_MASKf, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0_MASKf, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGS_MASKf, &dos_tcp_flagsn, 0x6);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_COPY_TO_CPUf, &dos_tcp_flagsn, 0);
                break;
            case DOS_TYPE_TCP_SYNFIN_SCAN:
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORTf, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0f, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGSf, &dos_tcp_flagsn, 0x3);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORT_MASKf, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0_MASKf, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGS_MASKf, &dos_tcp_flagsn, 0x3);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_COPY_TO_CPUf, &dos_tcp_flagsn, 0);
                break;
            case DOS_TYPE_TCP_XMAS_SCAN:
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORTf, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0f, &dos_tcp_flagsn, 1);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGSf, &dos_tcp_flagsn, 0x29);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORT_MASKf, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0_MASKf, &dos_tcp_flagsn, 1);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGS_MASKf, &dos_tcp_flagsn, 0x29);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_COPY_TO_CPUf, &dos_tcp_flagsn, 0);
                break;
            case DOS_TYPE_TCP_NULL_SCAN:
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORTf, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0f, &dos_tcp_flagsn, 1);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGSf, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORT_MASKf, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0_MASKf, &dos_tcp_flagsn, 1);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGS_MASKf, &dos_tcp_flagsn, 0x3f);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_COPY_TO_CPUf, &dos_tcp_flagsn, 0);
                break;
            case DOS_TYPE_TCP_SYN_PORTLESS1024:
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORTf, &dos_tcp_flagsn, 1);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0f, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGSf, &dos_tcp_flagsn, 0x2);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORT_MASKf, &dos_tcp_flagsn, 1);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0_MASKf, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGS_MASKf, &dos_tcp_flagsn, 0x12);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_COPY_TO_CPUf, &dos_tcp_flagsn, 0);
                break;
            case DOS_TYPE_TCP_ALLFLAGS_SCAN:
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORTf, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0f, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGSf, &dos_tcp_flagsn, 0x3F);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORT_MASKf, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0_MASKf, &dos_tcp_flagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGS_MASKf, &dos_tcp_flagsn, 0x3F);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_COPY_TO_CPUf, &dos_tcp_flagsn, 0);
                break;
            default:
                break;
        }
    }
    else if(enable == YT_DISABLE)
    {
        /* clear setting when disabled */
        HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORTf, &dos_tcp_flagsn, 0);
        HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0f, &dos_tcp_flagsn, 0);
        HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGSf, &dos_tcp_flagsn, 0);
        HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORT_MASKf, &dos_tcp_flagsn, 1);
        HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0_MASKf, &dos_tcp_flagsn, 1);
        HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGS_MASKf, &dos_tcp_flagsn, 0x3f);
        HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_COPY_TO_CPUf, &dos_tcp_flagsn, 0);
    }
    else
    {
        return CMM_ERR_NOT_SUPPORT;
    }
    memid = DOS_TCP_FLAGS0m + memid;

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, memid, 0, sizeof(dos_tcp_flags0_t), &dos_tcp_flagsn), ret);

    return CMM_ERR_OK;
}

static uint32_t  fal_tiger_dos_tcp_flagsn_get(yt_unit_t unit, yt_dos_type_t type, yt_enable_t *pEnable)
{
    dos_tcp_flags0_t dos_tcp_flagsn;
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t memid = 0;
    uint32_t public_tcp_sport;
    uint32_t seq_num;
    uint32_t tcp_flag;
    uint32_t public_tcp_sport_mask;
    uint32_t seq_num_mask;
    uint32_t tcp_flag_mask;
    uint32_t copy_to_cpu;

    switch(type)
    {
        case DOS_TYPE_TCP_SYNRST_SCAN:
           memid = 4;
           break;
        case DOS_TYPE_TCP_SYNFIN_SCAN:
           memid = 3;
           break;
        case DOS_TYPE_TCP_XMAS_SCAN:
           memid = 0;
           break;
        case DOS_TYPE_TCP_NULL_SCAN:
           memid = 1;
           break;
        case DOS_TYPE_TCP_SYN_PORTLESS1024:
           memid = 2;
           break;
        case DOS_TYPE_TCP_ALLFLAGS_SCAN:
           memid = 5;
           break;
        default:
           return CMM_ERR_NOT_SUPPORT;
    }

    memid = DOS_TCP_FLAGS0m + memid;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, memid, 0, sizeof(dos_tcp_flags0_t), &dos_tcp_flagsn), ret);

    HAL_FIELD_GET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORTf, &dos_tcp_flagsn, &public_tcp_sport);
    HAL_FIELD_GET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0f, &dos_tcp_flagsn, &seq_num);
    HAL_FIELD_GET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGSf, &dos_tcp_flagsn, &tcp_flag);
    HAL_FIELD_GET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORT_MASKf, &dos_tcp_flagsn, &public_tcp_sport_mask);
    HAL_FIELD_GET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0_MASKf, &dos_tcp_flagsn, &seq_num_mask);
    HAL_FIELD_GET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGS_MASKf, &dos_tcp_flagsn, &tcp_flag_mask);
    HAL_FIELD_GET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_COPY_TO_CPUf, &dos_tcp_flagsn, &copy_to_cpu);
    if(public_tcp_sport == 0 &&
        seq_num == 0 &&
        tcp_flag == 0 &&
        public_tcp_sport_mask == 1 &&
        seq_num_mask == 1 &&
        tcp_flag_mask == 0x3F)
    {
        *pEnable = YT_DISABLE;
    }
    else
    {
        *pEnable = YT_ENABLE;
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dos_init(yt_unit_t unit)
{
    dos_ctrl_t dos_ctrl;
    cmm_err_t ret = CMM_ERR_OK;

    /*disable part of dos drop by default*/
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_CTRLm, 0, sizeof(dos_ctrl_t), &dos_ctrl), ret);
    HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_TCP_SPORT_EQ_DPORT_DROPf, &dos_ctrl, YT_DISABLE);
    HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_UDP_SPORT_EQ_DPORT_DROPf, &dos_ctrl, YT_DISABLE);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, DOS_CTRLm, 0, sizeof(dos_ctrl_t), &dos_ctrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dos_port_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    dos_ctrl_t dos_ctrl;
    yt_macid_t macid;
    uint32_t port_en_mask;
    cmm_err_t ret = CMM_ERR_OK;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_CTRLm, 0, sizeof(dos_ctrl_t), &dos_ctrl), ret);
    
    HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_PORT_EN_MASKf, &dos_ctrl, &port_en_mask);
    if(enable)
    {
        port_en_mask |= 1<<macid;
    }
    else
    {
        port_en_mask &= ~(1<<macid);
    }
    HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_PORT_EN_MASKf, &dos_ctrl, port_en_mask);
    
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, DOS_CTRLm, 0, sizeof(dos_ctrl_t), &dos_ctrl), ret);
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dos_port_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    dos_ctrl_t dos_ctrl;
    yt_macid_t macid;
    uint32_t port_en_mask;
    cmm_err_t ret = CMM_ERR_OK;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_CTRLm, 0, sizeof(dos_ctrl_t), &dos_ctrl), ret);
    
    HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_PORT_EN_MASKf, &dos_ctrl, &port_en_mask);
    *pEnable = (port_en_mask & (1<<macid)) ? YT_ENABLE : YT_DISABLE;
    
    return CMM_ERR_OK;
}


yt_ret_t fal_tiger_dos_drop_en_set(yt_unit_t unit, yt_dos_type_t type, yt_enable_t enable)
{
    dos_ctrl_t dos_ctrl;
    dos_ctrl1_t dos_ctrl1;
    uint16_t regid = 0;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_CTRLm, 0, sizeof(dos_ctrl_t), &dos_ctrl), ret);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_CTRL1m, 0, sizeof(dos_ctrl1_t), &dos_ctrl1), ret);

    regid = DOS_CTRLm;
    switch (type)
    {
        case DOS_TYPE_DAEQSA:
            HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_MACSA_EQ_MACDA_DROPf, &dos_ctrl, enable);
            break;
        case DOS_TYPE_LAND:
            HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_SIP_EQ_DIP_DROPf, &dos_ctrl, enable);
            break;
        case DOS_TYPE_ICMP_PRAG:
            HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_ICMP_PRAG_DROPf, &dos_ctrl, enable);
            break;
        case DOS_TYPE_TCP_FRAG_OFFSET:
            HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_TCP_FRAG_OFFSET1_DROPf, &dos_ctrl, enable);
            break;
        case DOS_TYPE_LARGE_ICMPV4:
            HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_LARGE_IPV4_ICMP_DROPf, &dos_ctrl, enable);
            break;
        case DOS_TYPE_LARGE_ICMPV6:
            HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_LARGE_IPV6_ICMP_DROPf, &dos_ctrl, enable);
            break;
        case DOS_TYPE_TCP_HEADER_PARTIAL:
            HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_TCP_HEADER_PARTIAL_DROPf, &dos_ctrl, enable);
            break;
        case DOS_TYPE_CFI_MISMATCH_DROP:
            HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_VLAN_CFI1DROP_ENf, &dos_ctrl, enable);
            break;           
        case DOS_TYPE_SA_MC_DROP:
            HAL_FIELD_SET(DOS_CTRL1m, DOS_CTRL1_SA_MC_DROP_ENf, &dos_ctrl1, enable);
            regid = DOS_CTRL1m;
            break;
        case DOS_TYPE_SA_BC_DROP:
            HAL_FIELD_SET(DOS_CTRL1m, DOS_CTRL1_SA_BC_DROP_ENf, &dos_ctrl1, enable);
            regid = DOS_CTRL1m;
            break;
        case DOS_TYPE_SA_ZERO_DROP:
            HAL_FIELD_SET(DOS_CTRL1m, DOS_CTRL1_SA_ZERO_DROP_ENf, &dos_ctrl1, enable);
            regid = DOS_CTRL1m;
            break;
        case DOS_TYPE_DA_ZERO_DROP:
            HAL_FIELD_SET(DOS_CTRL1m, DOS_CTRL1_DA_ZERO_DROP_ENf, &dos_ctrl1, enable);
            regid = DOS_CTRL1m;
            break;
        case DOS_TYPE_TCP_SYNRST_SCAN:
        case DOS_TYPE_TCP_SYNFIN_SCAN:
        case DOS_TYPE_TCP_XMAS_SCAN:
        case DOS_TYPE_TCP_NULL_SCAN:
        case DOS_TYPE_TCP_SYN_PORTLESS1024:
        case DOS_TYPE_TCP_ALLFLAGS_SCAN:
            return fal_tiger_dos_tcp_flagsn_set(unit, type, enable);
        default:
            regid = 0;
            ret = CMM_ERR_NOT_SUPPORT;
                break;
    }

    if(regid == DOS_CTRLm)
    {
        CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, DOS_CTRLm, 0, sizeof(dos_ctrl_t), &dos_ctrl), ret);
    }
    else if(regid == DOS_CTRL1m)
    {
        CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, DOS_CTRL1m, 0, sizeof(dos_ctrl1_t), &dos_ctrl1), ret);
    }

    return ret;
}

yt_ret_t fal_tiger_dos_drop_en_get(yt_unit_t unit, yt_dos_type_t type, yt_enable_t *pEnable)
{
    dos_ctrl_t dos_ctrl;
    dos_ctrl1_t dos_ctrl1;
    uint32_t enable = YT_DISABLE;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_CTRLm, 0, sizeof(dos_ctrl_t), &dos_ctrl), ret);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_CTRL1m, 0, sizeof(dos_ctrl1_t), &dos_ctrl1), ret);

    switch (type)
    {
        case DOS_TYPE_DAEQSA:
            HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_MACSA_EQ_MACDA_DROPf, &dos_ctrl, &enable);
            break;
        case DOS_TYPE_LAND:
            HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_SIP_EQ_DIP_DROPf, &dos_ctrl, &enable);
            break;
        case DOS_TYPE_ICMP_PRAG:
            HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_ICMP_PRAG_DROPf, &dos_ctrl, &enable);
            break;
        case DOS_TYPE_TCP_FRAG_OFFSET:
            HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_TCP_FRAG_OFFSET1_DROPf, &dos_ctrl, &enable);
            break;
        case DOS_TYPE_LARGE_ICMPV4:
            HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_LARGE_IPV4_ICMP_DROPf, &dos_ctrl, &enable);
            break;
        case DOS_TYPE_LARGE_ICMPV6:
            HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_LARGE_IPV6_ICMP_DROPf, &dos_ctrl, &enable);
            break;
        case DOS_TYPE_TCP_HEADER_PARTIAL:
            HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_TCP_HEADER_PARTIAL_DROPf, &dos_ctrl, &enable);
            break;
        case DOS_TYPE_CFI_MISMATCH_DROP:
            HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_VLAN_CFI1DROP_ENf, &dos_ctrl, &enable);
            break;           
        case DOS_TYPE_SA_MC_DROP:
            HAL_FIELD_GET(DOS_CTRL1m, DOS_CTRL1_SA_MC_DROP_ENf, &dos_ctrl1, &enable);
            break;
        case DOS_TYPE_SA_BC_DROP:
            HAL_FIELD_GET(DOS_CTRL1m, DOS_CTRL1_SA_BC_DROP_ENf, &dos_ctrl1, &enable);
            break;
        case DOS_TYPE_SA_ZERO_DROP:
            HAL_FIELD_GET(DOS_CTRL1m, DOS_CTRL1_SA_ZERO_DROP_ENf, &dos_ctrl1, &enable);
            break;
        case DOS_TYPE_DA_ZERO_DROP:
            HAL_FIELD_GET(DOS_CTRL1m, DOS_CTRL1_DA_ZERO_DROP_ENf, &dos_ctrl1, &enable);
            break;
        case DOS_TYPE_TCP_SYNRST_SCAN:
        case DOS_TYPE_TCP_SYNFIN_SCAN:
        case DOS_TYPE_TCP_XMAS_SCAN:
        case DOS_TYPE_TCP_NULL_SCAN:
        case DOS_TYPE_TCP_SYN_PORTLESS1024:
        case DOS_TYPE_TCP_ALLFLAGS_SCAN:
            return fal_tiger_dos_tcp_flagsn_get(unit, type, pEnable);
        default:
            ret = CMM_ERR_NOT_SUPPORT;
            break;
    }

    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return ret;
}

yt_ret_t  fal_tiger_dos_large_icmp_size_set(yt_unit_t unit, yt_dos_icmp_version_t ver, uint16_t size)
{
    dos_large_icmp_ctrl_t dos_large_icmp_ctrl;
    cmm_err_t ret = CMM_ERR_OK;

    if(size > 0x1FFF)
        return CMM_ERR_INPUT;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_LARGE_ICMP_CTRLm, 0, sizeof(dos_large_icmp_ctrl_t), &dos_large_icmp_ctrl), ret);
    
    if(ver == DOS_ICMP_VERSION_4)
    {
        HAL_FIELD_SET(DOS_LARGE_ICMP_CTRLm, DOS_LARGE_ICMP_CTRL_IPV4_ICMP_MAX_SIZEf, &dos_large_icmp_ctrl, size & 0x1FFF);
    }
    else if(ver == DOS_ICMP_VERSION_6)
    {
        HAL_FIELD_SET(DOS_LARGE_ICMP_CTRLm, DOS_LARGE_ICMP_CTRL_IPV6_ICMP_MAX_SIZEf, &dos_large_icmp_ctrl, size & 0x1FFF);
    }
    else
    {
        return CMM_ERR_INPUT;
    }
    
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, DOS_LARGE_ICMP_CTRLm, 0, sizeof(dos_large_icmp_ctrl_t), &dos_large_icmp_ctrl), ret);  
   
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_dos_large_icmp_size_get(yt_unit_t unit, yt_dos_icmp_version_t ver, uint16_t *psize)
{
    dos_large_icmp_ctrl_t dos_large_icmp_ctrl;
    uint32_t size;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_LARGE_ICMP_CTRLm, 0, sizeof(dos_large_icmp_ctrl_t), &dos_large_icmp_ctrl), ret);
    
    if(ver == DOS_ICMP_VERSION_4)
    {
        HAL_FIELD_GET(DOS_LARGE_ICMP_CTRLm, DOS_LARGE_ICMP_CTRL_IPV4_ICMP_MAX_SIZEf, &dos_large_icmp_ctrl, &size);
    }
    else if(ver == DOS_ICMP_VERSION_6)
    {
        HAL_FIELD_GET(DOS_LARGE_ICMP_CTRLm, DOS_LARGE_ICMP_CTRL_IPV6_ICMP_MAX_SIZEf, &dos_large_icmp_ctrl, &size);
    }
    else
    {
        return CMM_ERR_INPUT;
    }

    *psize = size;
    return CMM_ERR_OK;
}
