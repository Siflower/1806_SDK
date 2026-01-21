/*================================================================
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
================================================================*/


#ifndef _FAL_CMM_H_
#define _FAL_CMM_H_

#include "yt_cmm.h"
#include "fal_dispatch.h"

#if defined(SWITCH_SERIES_TIGER)
#include "fal_tiger_cmm.h"
#endif
#if defined(SWITCH_SERIES_SHARK)
#include "fal_shark_cmm.h"
#endif

#include "hal_mem.h"

#define IS_MAC_ADDR_EQUAL(x,y)          (!osal_memcmp((x), (y), 6))
#define FAL_MAX_PORT_NUM                CHIP_MAX_PORT_NUM
#define FAL_MAX_LAG_NUM                 CHIP_MAX_LAG_NUM
#define FAL_MAX_MULTICAST_VLAN_NUM      CHIP_MAX_MULTICAST_VLAN_NUM
#define FAL_INTERNAL_CPU_MACID      CHIP_INTERNAL_CPU_MACID

#endif //FAL_CMM_H
