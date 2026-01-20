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

static uint32_t  fal_tiger_dos_tcp_flagsn_set(yt_unit_t unit, yt_dos_type_t dosType, yt_enable_t enable)
{
    dos_tcp_flags0_t dosTcpFlagsn;
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t memId = 0;

    switch(dosType)
    {
        case YT_DOS_TYPE_TCP_SYNRST_SCAN:
           memId = 4;
           break;
        case YT_DOS_TYPE_TCP_SYNFIN_SCAN:
           memId = 3;
           break;
        case YT_DOS_TYPE_TCP_XMAS_SCAN:
           memId = 0;
           break;
        case YT_DOS_TYPE_TCP_NULL_SCAN:
           memId = 1;
           break;
        case YT_DOS_TYPE_TCP_SYN_PORTLESS1024:
           memId = 2;
           break;
        case YT_DOS_TYPE_TCP_ALLFLAGS_SCAN:
           memId = 5;
           break;
        default:
           return CMM_ERR_NOT_SUPPORT;
    }

    if(enable == YT_ENABLE)
    {
        switch(dosType)
        {
            case YT_DOS_TYPE_TCP_SYNRST_SCAN:
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORTf, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0f, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGSf, &dosTcpFlagsn, 0x6);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORT_MASKf, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0_MASKf, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGS_MASKf, &dosTcpFlagsn, 0x6);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_COPY_TO_CPUf, &dosTcpFlagsn, 0);
                break;
            case YT_DOS_TYPE_TCP_SYNFIN_SCAN:
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORTf, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0f, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGSf, &dosTcpFlagsn, 0x3);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORT_MASKf, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0_MASKf, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGS_MASKf, &dosTcpFlagsn, 0x3);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_COPY_TO_CPUf, &dosTcpFlagsn, 0);
                break;
            case YT_DOS_TYPE_TCP_XMAS_SCAN:
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORTf, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0f, &dosTcpFlagsn, 1);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGSf, &dosTcpFlagsn, 0x29);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORT_MASKf, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0_MASKf, &dosTcpFlagsn, 1);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGS_MASKf, &dosTcpFlagsn, 0x29);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_COPY_TO_CPUf, &dosTcpFlagsn, 0);
                break;
            case YT_DOS_TYPE_TCP_NULL_SCAN:
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORTf, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0f, &dosTcpFlagsn, 1);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGSf, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORT_MASKf, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0_MASKf, &dosTcpFlagsn, 1);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGS_MASKf, &dosTcpFlagsn, 0x3f);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_COPY_TO_CPUf, &dosTcpFlagsn, 0);
                break;
            case YT_DOS_TYPE_TCP_SYN_PORTLESS1024:
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORTf, &dosTcpFlagsn, 1);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0f, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGSf, &dosTcpFlagsn, 0x2);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORT_MASKf, &dosTcpFlagsn, 1);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0_MASKf, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGS_MASKf, &dosTcpFlagsn, 0x12);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_COPY_TO_CPUf, &dosTcpFlagsn, 0);
                break;
            case YT_DOS_TYPE_TCP_ALLFLAGS_SCAN:
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORTf, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0f, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGSf, &dosTcpFlagsn, 0x3F);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORT_MASKf, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0_MASKf, &dosTcpFlagsn, 0);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGS_MASKf, &dosTcpFlagsn, 0x3F);
                HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_COPY_TO_CPUf, &dosTcpFlagsn, 0);
                break;
            default:
                break;
        }
    }
    else if(enable == YT_DISABLE)
    {
        /* clear setting when disabled */
        HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORTf, &dosTcpFlagsn, 0);
        HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0f, &dosTcpFlagsn, 0);
        HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGSf, &dosTcpFlagsn, 0);
        HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORT_MASKf, &dosTcpFlagsn, 1);
        HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0_MASKf, &dosTcpFlagsn, 1);
        HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGS_MASKf, &dosTcpFlagsn, 0x3f);
        HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_COPY_TO_CPUf, &dosTcpFlagsn, 0);
    }
    else
    {
        return CMM_ERR_NOT_SUPPORT;
    }
    memId = DOS_TCP_FLAGS0m + memId;
    HAL_FIELD_SET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_DROP_EN, &dosTcpFlagsn, enable);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, memId, 0, sizeof(dos_tcp_flags0_t), &dosTcpFlagsn), ret);

    return CMM_ERR_OK;
}

static uint32_t  fal_tiger_dos_tcp_flagsn_get(yt_unit_t unit, yt_dos_type_t dosType, yt_enable_t *pEnable)
{
    dos_tcp_flags0_t dosTcpFlagsn;
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t memId = 0;
    uint32_t publicTcpSport;
    uint32_t seqNum;
    uint32_t tcpFlag;
    uint32_t publicTcpSportMask;
    uint32_t seqNumMask;
    uint32_t tcpFlagMask;
    uint32_t copyToCpu;
    uint32_t enable = YT_DISABLE;

    switch(dosType)
    {
        case YT_DOS_TYPE_TCP_SYNRST_SCAN:
           memId = 4;
           break;
        case YT_DOS_TYPE_TCP_SYNFIN_SCAN:
           memId = 3;
           break;
        case YT_DOS_TYPE_TCP_XMAS_SCAN:
           memId = 0;
           break;
        case YT_DOS_TYPE_TCP_NULL_SCAN:
           memId = 1;
           break;
        case YT_DOS_TYPE_TCP_SYN_PORTLESS1024:
           memId = 2;
           break;
        case YT_DOS_TYPE_TCP_ALLFLAGS_SCAN:
           memId = 5;
           break;
        default:
           return CMM_ERR_NOT_SUPPORT;
    }

    memId = DOS_TCP_FLAGS0m + memId;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, memId, 0, sizeof(dos_tcp_flags0_t), &dosTcpFlagsn), ret);

    HAL_FIELD_GET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORTf, &dosTcpFlagsn, &publicTcpSport);
    HAL_FIELD_GET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0f, &dosTcpFlagsn, &seqNum);
    HAL_FIELD_GET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGSf, &dosTcpFlagsn, &tcpFlag);
    HAL_FIELD_GET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_PUBLIC_TCP_SPORT_MASKf, &dosTcpFlagsn, &publicTcpSportMask);
    HAL_FIELD_GET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_SEQ_NUM0_MASKf, &dosTcpFlagsn, &seqNumMask);
    HAL_FIELD_GET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_TCP_FLAGS_MASKf, &dosTcpFlagsn, &tcpFlagMask);
    HAL_FIELD_GET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_COPY_TO_CPUf, &dosTcpFlagsn, &copyToCpu);
    HAL_FIELD_GET(DOS_TCP_FLAGS0m, DOS_TCP_FLAGS0_DROP_EN, &dosTcpFlagsn, &enable);
    if(publicTcpSport == 0 &&
        seqNum == 0 &&
        tcpFlag == 0 &&
        publicTcpSportMask == 1 &&
        seqNumMask == 1 &&
        tcpFlagMask == 0x3F &&
        enable == YT_DISABLE)
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
    dos_ctrl_t dosCtrl;
    cmm_err_t ret = CMM_ERR_OK;

    /*disable part of dos drop by default*/
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_CTRLm, 0, sizeof(dos_ctrl_t), &dosCtrl), ret);
    HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_TCP_SPORT_EQ_DPORT_DROPf, &dosCtrl, YT_DISABLE);
    HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_UDP_SPORT_EQ_DPORT_DROPf, &dosCtrl, YT_DISABLE);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, DOS_CTRLm, 0, sizeof(dos_ctrl_t), &dosCtrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dos_port_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    dos_ctrl_t dosCtrl;
    yt_macid_t macId;
    uint32_t portEnMask;
    cmm_err_t ret = CMM_ERR_OK;

    macId = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_CTRLm, 0, sizeof(dos_ctrl_t), &dosCtrl), ret);
    
    HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_PORT_EN_MASKf, &dosCtrl, &portEnMask);
    if(enable)
    {
        portEnMask |= 1<<macId;
    }
    else
    {
        portEnMask &= ~(1<<macId);
    }
    HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_PORT_EN_MASKf, &dosCtrl, portEnMask);
    
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, DOS_CTRLm, 0, sizeof(dos_ctrl_t), &dosCtrl), ret);
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dos_port_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    dos_ctrl_t dosCtrl;
    yt_macid_t macId;
    uint32_t portEnMask;
    cmm_err_t ret = CMM_ERR_OK;

    macId = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_CTRLm, 0, sizeof(dos_ctrl_t), &dosCtrl), ret);
    
    HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_PORT_EN_MASKf, &dosCtrl, &portEnMask);
    *pEnable = (portEnMask & (1<<macId)) ? YT_ENABLE : YT_DISABLE;
    
    return CMM_ERR_OK;
}


yt_ret_t fal_tiger_dos_drop_en_set(yt_unit_t unit, yt_dos_type_t dosType, yt_enable_t enable)
{
    dos_ctrl_t dosCtrl;
    dos_ctrl1_t dosCtrl1;
    uint16_t regId = 0;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_CTRLm, 0, sizeof(dos_ctrl_t), &dosCtrl), ret);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_CTRL1m, 0, sizeof(dos_ctrl1_t), &dosCtrl1), ret);

    regId = DOS_CTRLm;
    switch (dosType)
    {
        case YT_DOS_TYPE_SA_EQ_DA:
            HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_MACSA_EQ_MACDA_DROPf, &dosCtrl, enable);
            break;
        case YT_DOS_TYPE_LAND:
            HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_SIP_EQ_DIP_DROPf, &dosCtrl, enable);
            break;
        case YT_DOS_TYPE_ICMP_FRAG:
            HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_ICMP_PRAG_DROPf, &dosCtrl, enable);
            break;
        case YT_DOS_TYPE_TCP_FRAG_NOT_FIRST:
            HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_TCP_FRAG_OFFSET1_DROPf, &dosCtrl, enable);
            break;
        case YT_DOS_TYPE_LARGE_IPV4_ICMP:
            HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_LARGE_IPV4_ICMP_DROPf, &dosCtrl, enable);
            break;
        case YT_DOS_TYPE_LARGE_IPV6_ICMP:
            HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_LARGE_IPV6_ICMP_DROPf, &dosCtrl, enable);
            break;
        case YT_DOS_TYPE_TCP_HEADER_PARTIAL:
            HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_TCP_HEADER_PARTIAL_DROPf, &dosCtrl, enable);
            break;
        case YT_DOS_TYPE_CFI_MISMATCH:
            HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_VLAN_CFI1DROP_ENf, &dosCtrl, enable);
            break;           
        case YT_DOS_TYPE_UDP_SPORT_EQ_DPORT:
			if (CAL_SWCHIP_ID(unit) == YT_SW_ID_9218)
            {
                HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_UDP_SPORT_EQ_DPORT_DROPf, &dosCtrl, enable);
                regId = DOS_CTRLm;
            }
            else
            {
                regId = 0;
                ret = CMM_ERR_NOT_SUPPORT;
            }
            break;
        case YT_DOS_TYPE_TCP_SPORT_EQ_DPORT:
			if (CAL_SWCHIP_ID(unit) == YT_SW_ID_9218)
            {
                HAL_FIELD_SET(DOS_CTRLm, DOS_CTRL_TCP_SPORT_EQ_DPORT_DROPf, &dosCtrl, enable);
                regId = DOS_CTRLm;
            }
            else
            {
                regId = 0;
                ret = CMM_ERR_NOT_SUPPORT;
            }
            break;
        case YT_DOS_TYPE_SA_MC:
            HAL_FIELD_SET(DOS_CTRL1m, DOS_CTRL1_SA_MC_DROP_ENf, &dosCtrl1, enable);
            regId = DOS_CTRL1m;
            break;
        case YT_DOS_TYPE_SA_BC:
            HAL_FIELD_SET(DOS_CTRL1m, DOS_CTRL1_SA_BC_DROP_ENf, &dosCtrl1, enable);
            regId = DOS_CTRL1m;
            break;
        case YT_DOS_TYPE_SA_ZERO:
            HAL_FIELD_SET(DOS_CTRL1m, DOS_CTRL1_SA_ZERO_DROP_ENf, &dosCtrl1, enable);
            regId = DOS_CTRL1m;
            break;
        case YT_DOS_TYPE_DA_ZERO:
            HAL_FIELD_SET(DOS_CTRL1m, DOS_CTRL1_DA_ZERO_DROP_ENf, &dosCtrl1, enable);
            regId = DOS_CTRL1m;
            break;
        case YT_DOS_TYPE_TCP_SYNRST_SCAN:
        case YT_DOS_TYPE_TCP_SYNFIN_SCAN:
        case YT_DOS_TYPE_TCP_XMAS_SCAN:
        case YT_DOS_TYPE_TCP_NULL_SCAN:
        case YT_DOS_TYPE_TCP_SYN_PORTLESS1024:
        case YT_DOS_TYPE_TCP_ALLFLAGS_SCAN:
            return fal_tiger_dos_tcp_flagsn_set(unit, dosType, enable);
        default:
            regId = 0;
            ret = CMM_ERR_NOT_SUPPORT;
                break;
    }

    if(regId == DOS_CTRLm)
    {
        CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, DOS_CTRLm, 0, sizeof(dos_ctrl_t), &dosCtrl), ret);
    }
    else if(regId == DOS_CTRL1m)
    {
        CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, DOS_CTRL1m, 0, sizeof(dos_ctrl1_t), &dosCtrl1), ret);
    }

    return ret;
}

yt_ret_t fal_tiger_dos_drop_en_get(yt_unit_t unit, yt_dos_type_t dosType, yt_enable_t *pEnable)
{
    dos_ctrl_t dosCtrl;
    dos_ctrl1_t dosCtrl1;
    uint32_t enable = YT_DISABLE;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_CTRLm, 0, sizeof(dos_ctrl_t), &dosCtrl), ret);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_CTRL1m, 0, sizeof(dos_ctrl1_t), &dosCtrl1), ret);

    switch (dosType)
    {
        case YT_DOS_TYPE_SA_EQ_DA:
            HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_MACSA_EQ_MACDA_DROPf, &dosCtrl, &enable);
            break;
        case YT_DOS_TYPE_LAND:
            HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_SIP_EQ_DIP_DROPf, &dosCtrl, &enable);
            break;
        case YT_DOS_TYPE_ICMP_FRAG:
            HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_ICMP_PRAG_DROPf, &dosCtrl, &enable);
            break;
        case YT_DOS_TYPE_TCP_FRAG_NOT_FIRST:
            HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_TCP_FRAG_OFFSET1_DROPf, &dosCtrl, &enable);
            break;
        case YT_DOS_TYPE_LARGE_IPV4_ICMP:
            HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_LARGE_IPV4_ICMP_DROPf, &dosCtrl, &enable);
            break;
        case YT_DOS_TYPE_LARGE_IPV6_ICMP:
            HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_LARGE_IPV6_ICMP_DROPf, &dosCtrl, &enable);
            break;
        case YT_DOS_TYPE_TCP_HEADER_PARTIAL:
            HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_TCP_HEADER_PARTIAL_DROPf, &dosCtrl, &enable);
            break;
        case YT_DOS_TYPE_CFI_MISMATCH:
            HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_VLAN_CFI1DROP_ENf, &dosCtrl, &enable);
            break;           
        case YT_DOS_TYPE_UDP_SPORT_EQ_DPORT:
			if (CAL_SWCHIP_ID(unit) == YT_SW_ID_9218)
            {
                HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_UDP_SPORT_EQ_DPORT_DROPf, &dosCtrl, &enable);
            }
            else if (CAL_SWCHIP_MODEL(unit) == YT_SW_MODEL_9215)
            {
                ret = CMM_ERR_NOT_SUPPORT;
            }
            break;
        case YT_DOS_TYPE_TCP_SPORT_EQ_DPORT:
			if (CAL_SWCHIP_ID(unit) == YT_SW_ID_9218)
            {
                HAL_FIELD_GET(DOS_CTRLm, DOS_CTRL_TCP_SPORT_EQ_DPORT_DROPf, &dosCtrl, &enable);
            }
            else
            {
                ret = CMM_ERR_NOT_SUPPORT;
            }
            break;
        case YT_DOS_TYPE_SA_MC:
            HAL_FIELD_GET(DOS_CTRL1m, DOS_CTRL1_SA_MC_DROP_ENf, &dosCtrl1, &enable);
            break;
        case YT_DOS_TYPE_SA_BC:
            HAL_FIELD_GET(DOS_CTRL1m, DOS_CTRL1_SA_BC_DROP_ENf, &dosCtrl1, &enable);
            break;
        case YT_DOS_TYPE_SA_ZERO:
            HAL_FIELD_GET(DOS_CTRL1m, DOS_CTRL1_SA_ZERO_DROP_ENf, &dosCtrl1, &enable);
            break;
        case YT_DOS_TYPE_DA_ZERO:
            HAL_FIELD_GET(DOS_CTRL1m, DOS_CTRL1_DA_ZERO_DROP_ENf, &dosCtrl1, &enable);
            break;
        case YT_DOS_TYPE_TCP_SYNRST_SCAN:
        case YT_DOS_TYPE_TCP_SYNFIN_SCAN:
        case YT_DOS_TYPE_TCP_XMAS_SCAN:
        case YT_DOS_TYPE_TCP_NULL_SCAN:
        case YT_DOS_TYPE_TCP_SYN_PORTLESS1024:
        case YT_DOS_TYPE_TCP_ALLFLAGS_SCAN:
            return fal_tiger_dos_tcp_flagsn_get(unit, dosType, pEnable);
        default:
            ret = CMM_ERR_NOT_SUPPORT;
            break;
    }

    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return ret;
}

yt_ret_t fal_tiger_dos_large_packet_size_set(yt_unit_t unit, yt_dos_packet_type_t packType, yt_dos_packet_version_t ver, uint16_t size)
{
    dos_large_icmp_ctrl_t dosLargeIcmpCtrl;
    cmm_err_t ret = CMM_ERR_OK;

    if(size > 0x1FFF)
    {
        return CMM_ERR_INPUT;
    }
    if (YT_LARGE_ICMP == packType)
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_LARGE_ICMP_CTRLm, 0, sizeof(dos_large_icmp_ctrl_t), &dosLargeIcmpCtrl), ret);
        if (YT_DOS_VERSION_4 == ver)
        {
            HAL_FIELD_SET(DOS_LARGE_ICMP_CTRLm, DOS_LARGE_ICMP_CTRL_IPV4_ICMP_MAX_SIZEf, &dosLargeIcmpCtrl, size & 0x1FFF);
        }
        else if (YT_DOS_VERSION_6 == ver)
        {
            HAL_FIELD_SET(DOS_LARGE_ICMP_CTRLm, DOS_LARGE_ICMP_CTRL_IPV6_ICMP_MAX_SIZEf, &dosLargeIcmpCtrl, size & 0x1FFF);
        }
        else
        {
            return CMM_ERR_INPUT;
        }
        CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, DOS_LARGE_ICMP_CTRLm, 0, sizeof(dos_large_icmp_ctrl_t), &dosLargeIcmpCtrl), ret);  
    }
    else
    {
        return CMM_ERR_NOT_SUPPORT;
    }
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dos_large_packet_size_get(yt_unit_t unit, yt_dos_packet_type_t packType, yt_dos_packet_version_t ver, uint16_t *pSize)
{
    dos_large_icmp_ctrl_t dosLargeIcmpCtrl;
    uint32_t size;
    cmm_err_t ret = CMM_ERR_OK;

    if (YT_LARGE_ICMP == packType)
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DOS_LARGE_ICMP_CTRLm, 0, sizeof(dos_large_icmp_ctrl_t), &dosLargeIcmpCtrl), ret);
        if(YT_DOS_VERSION_4 == ver)
        {
            HAL_FIELD_GET(DOS_LARGE_ICMP_CTRLm, DOS_LARGE_ICMP_CTRL_IPV4_ICMP_MAX_SIZEf, &dosLargeIcmpCtrl, &size);
        }
        else if(YT_DOS_VERSION_6 == ver)
        {
            HAL_FIELD_GET(DOS_LARGE_ICMP_CTRLm, DOS_LARGE_ICMP_CTRL_IPV6_ICMP_MAX_SIZEf, &dosLargeIcmpCtrl, &size);
        }
        else
        {
            return CMM_ERR_INPUT;
        }
        *pSize = size; 
    }
    else
    {
        return CMM_ERR_NOT_SUPPORT;
    }
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dos_action_set(yt_unit_t unit, yt_dos_type_t dosType, yt_act_type_t action)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(dosType);
    CMM_UNUSED_PARAM(action);
    return CMM_ERR_NOT_SUPPORT;
}
yt_ret_t fal_tiger_dos_action_get(yt_unit_t unit, yt_dos_type_t dosType, yt_act_type_t *pAction)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(dosType);
    CMM_UNUSED_PARAM(pAction);
    return CMM_ERR_NOT_SUPPORT;
}
yt_ret_t fal_tiger_dos_salearn_dis_set(yt_unit_t unit, yt_dos_type_t dosType, yt_enable_t enable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(dosType);
    CMM_UNUSED_PARAM(enable);
    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_dos_salearn_dis_get(yt_unit_t unit, yt_dos_type_t dosType, yt_enable_t *pEnable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(dosType);
    CMM_UNUSED_PARAM(pEnable);
    return CMM_ERR_NOT_SUPPORT;
}
