/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_dos.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_DOS_H
#define __YT_DOS_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "yt_cmm.h"

typedef enum yt_dos_type_e 
{
    YT_DOS_TYPE_UDP_SPORT_EQ_DPORT,      /* SW_ID_9215 not spport, SW_ID_9218 support */
    YT_DOS_TYPE_TCP_SPORT_EQ_DPORT,     /* SW_ID_9215 not spport, SW_ID_9218 support */
    YT_DOS_TYPE_TCP_HEADER_PARTIAL,
    YT_DOS_TYPE_TCP_FRAG_NOT_FIRST,
    YT_DOS_TYPE_ICMP_FRAG,
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    YT_DOS_TYPE_LARGE_IPV4_PAYLOAD,
    YT_DOS_TYPE_LARGE_IPV6_PAYLOAD,
#endif
    YT_DOS_TYPE_LARGE_IPV4_ICMP,
    YT_DOS_TYPE_LARGE_IPV6_ICMP,
    YT_DOS_TYPE_LAND,
    YT_DOS_TYPE_DA_ZERO,
    YT_DOS_TYPE_SA_MC,
    YT_DOS_TYPE_SA_BC,
    YT_DOS_TYPE_SA_ZERO,
    YT_DOS_TYPE_SA_EQ_DA,
    YT_DOS_TYPE_TCP_XMAS_SCAN, /*TCP sequence number is zero and the FIN, URG and PSH bits are set*/
    YT_DOS_TYPE_TCP_NULL_SCAN, /*TCP sequence number is zero and all control bits are zeros*/
    YT_DOS_TYPE_TCP_SYN_PORTLESS1024,  /*SYN bit is 1, ACK bit is 0, and Source-Port is 0~1023*/
    YT_DOS_TYPE_TCP_SYNFIN_SCAN,   /*SYN and FIN bits are set*/
    YT_DOS_TYPE_TCP_SYNRST_SCAN,   /*SYN and RST bits are set*/
    YT_DOS_TYPE_TCP_ALLFLAGS_SCAN, /*All control bits are set*/
    YT_DOS_TYPE_CFI_MISMATCH,
    YT_DOS_TYPE_MAX,
}yt_dos_type_t;

typedef enum yt_dos_packet_version_e
{
    YT_DOS_VERSION_4,
    YT_DOS_VERSION_6,
}yt_dos_packet_version_t;

typedef enum yt_dos_packet_type_e
{
    YT_LARGE_ICMP,
#if defined(SWITCH_SERIES_SHARK) || defined(SWITCH_SERIES_WHALE)
    YT_LARGE_IP_PAYLOAD,
#endif
    YT_LARGE_TYPE_MAX,
}yt_dos_packet_type_t;

/**
 * @internal      yt_dos_init
 * @endinternal
 *
 * @brief         init dos function module
 * @note          APPLICABLE DEVICES  -Tiger,Shark, Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dos_init(yt_unit_t unit);

/**
 * @internal      yt_dos_port_en_set
 * @endinternal
 *
 * @brief         enable or disable dos on specific port
 * @note          APPLICABLE DEVICES  -Tiger,Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dos_port_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      yt_dos_port_en_get
 * @endinternal
 *
 * @brief         get enable state of dos on specific port
 * @note          APPLICABLE DEVICES  -Tiger,Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dos_port_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      yt_dos_drop_en_set
 * @endinternal
 *
 * @brief         enable drop for specific dos
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     dosType                -dos type
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dos_drop_en_set(yt_unit_t unit, yt_dos_type_t dosType, yt_enable_t enable);


/**
 * @internal      yt_dos_drop_en_get
 * @endinternal
 *
 * @brief         get drop state of specific dos
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     type                -type defined of dos
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dos_drop_en_get(yt_unit_t unit, yt_dos_type_t dosType, yt_enable_t *pEnable);


/**
 * @internal      yt_dos_action_set
 * @endinternal
 *
 * @brief         set action of specific dos type
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     dosType                -dos type
 * @param[in]     action              -action of dos
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dos_action_set(yt_unit_t unit, yt_dos_type_t dosType, yt_act_type_t action);


/**
 * @internal      yt_dos_action_get
 * @endinternal
 *
 * @brief         get action of specific dos type
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     dosType                -dos type
 * @param[in]     actionMap              -action of dos
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dos_action_get(yt_unit_t unit, yt_dos_type_t dosType, yt_act_type_t *pAction);


/**
 * @internal      yt_dos_salearn_dis_set
 * @endinternal
 *
 * @brief         enable/disable SA learning for specific dos type 
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     dosType                -type defined of dos
 * @param[in]     enable             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dos_salearn_dis_set(yt_unit_t unit, yt_dos_type_t dosType, yt_enable_t enable);


/**
 * @internal      yt_dos_salearn_dis_get
 * @endinternal
 *
 * @brief         get enable/disable SA learning for specific dos type 
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     dosType                -dos type
 * @param[in]     pEnable             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_dos_salearn_dis_get(yt_unit_t unit, yt_dos_type_t dosType, yt_enable_t *pEnable);


/**
 * @internal      yt_dos_large_packet_size_set
 * @endinternal
 *
 * @brief         set largest size of icmp packet
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     packtype       - icmp/ip payload
 * @param[in]     ver                 -icmp version
 * @param[in]     size                -largest size
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
 extern yt_ret_t yt_dos_large_packet_size_set(yt_unit_t unit, yt_dos_packet_type_t packType, yt_dos_packet_version_t ver, uint16_t size);


/**
 * @internal      yt_dos_large_packet_size_get
 * @endinternal
 *
 * @brief         get largest size of icmp packet
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     packtype       - icmp/ip payload
 * @param[in]     ver                 -icmp version
 * @param[in]     size                -size
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
 extern yt_ret_t yt_dos_large_packet_size_get(yt_unit_t unit, yt_dos_packet_type_t packType, yt_dos_packet_version_t ver, uint16_t *pSize);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
