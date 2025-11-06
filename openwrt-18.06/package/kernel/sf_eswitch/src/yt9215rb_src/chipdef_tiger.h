/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#ifndef __CHIP_DEFINE_TIGER_H__
#define __CHIP_DEFINE_TIGER_H__

/* chip capacity macro */
#define CHIP_VLAN_NUM_MAX                  4096
#define CHIP_VLAN_TPID_PROFILE_NUM         4
#define CHIP_L2_FDB_NUM_MAX                4096
#define CHIP_FLOW_NUM_MAX                  64
#define CHIP_MAX_PORT_NUM		11
#define CHIP_MAX_LAG_NUM		2
#define CHIP_MAX_MULTICAST_VLAN_NUM	16
#define CHIP_INTERNAL_CPU_MACID		10
#define CHIP_MAX_CASCADE_PORT_NUM           2
#define CHIP_INVALID_CASCADE_PORT           (0xF)
#define CHIP_CASCADE_FWD_PORTMASK           ((1UL << CHIP_MAX_PORT_NUM) - 1)
#define CHIP_JUMBO_SIZE_MAX                 (0x2400)
#define CHIP_JUMBO_SIZE_MIN                 (0x5EE)

#define CHIP_INTERFACE_SELECT_REG   (0x80394)
#define CHIP_INTERFACE_CTRL_REG   (0x80028)
#define CHIP_CHIP_ID_REG   (0x80008)
#define CHIP_CHIP_MODE_REG   (0x80388)

#endif
