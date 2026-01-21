/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_dos.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_DOS_H
#define __FAL_TIGER_DOS_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "fal_cmm.h"

extern yt_ret_t fal_tiger_dos_init(yt_unit_t unit);
extern yt_ret_t fal_tiger_dos_port_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);
extern yt_ret_t fal_tiger_dos_port_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);
extern yt_ret_t fal_tiger_dos_drop_en_set(yt_unit_t unit, yt_dos_type_t dosType, yt_enable_t enable);
extern yt_ret_t fal_tiger_dos_drop_en_get(yt_unit_t unit, yt_dos_type_t dosType, yt_enable_t *pEnable);
extern yt_ret_t fal_tiger_dos_large_packet_size_set(yt_unit_t unit, yt_dos_packet_type_t packType, yt_dos_packet_version_t ver, uint16_t size);
extern yt_ret_t fal_tiger_dos_large_packet_size_get(yt_unit_t unit, yt_dos_packet_type_t packType, yt_dos_packet_version_t ver, uint16_t *psize);
extern yt_ret_t fal_tiger_dos_action_set(yt_unit_t unit, yt_dos_type_t dosType, yt_act_type_t action);
extern yt_ret_t fal_tiger_dos_action_get(yt_unit_t unit, yt_dos_type_t dosType, yt_act_type_t *pAction);
extern yt_ret_t fal_tiger_dos_salearn_dis_set(yt_unit_t unit, yt_dos_type_t dosType, yt_enable_t enable);
extern yt_ret_t fal_tiger_dos_salearn_dis_get(yt_unit_t unit, yt_dos_type_t dosType, yt_enable_t *pEnable);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
