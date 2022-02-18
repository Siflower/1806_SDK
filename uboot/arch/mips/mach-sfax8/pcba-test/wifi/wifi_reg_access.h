/**
 ****************************************************************************************
 *
 * @file reg_access.h
 *
 * @brief File implementing the basic primitives for register accesses
 *
 * Copyright (C) Siflower 2015-2019
 *
 ****************************************************************************************
 */

#ifndef _WIFI_REG_ACCESS_H_
#define _WIFI_REG_ACCESS_H_

/**
 ****************************************************************************************
 * @defgroup REG REG
 * @ingroup PLATFORM_DRIVERS
 *
 * @brief Basic primitives for register access.
 *
 * @{
 ****************************************************************************************
 */
#include "co_utils.h"

#include "wifi_addr.h"
/*
 * MACROS
 ****************************************************************************************
 */
/* This macro will open the printf msg for every write register operation*/
// #define DEBUG_BY_REG

#define REG_CATRF_BASE_ADDR     SIFLOWER_CATRF_BASE

#define IPC_REG_BASE_ADDR                (WIFI_BASE_ADDR + 0x000C0000)
/* Macros for IPC registers access */
#define REG_IPC_APP_RD(env, INDEX) ( *(volatile u32*)( IPC_REG_BASE_ADDR + 4*(INDEX) ))

#define REG_IPC_APP_WR(env, INDEX, value) ( (*(volatile uint32_t *)(IPC_REG_BASE_ADDR + 4*(INDEX))) = (value) )

/// Macro to read a platform register(default 32bits)
#define REG_PL_RD(addr)              (*(volatile uint32_t *)(HW2CPU(addr)))

/// Macro to write a platform register(default 32bits)
#ifdef DEBUG_BY_REG
#define REG_PL_WR(addr, value)      do { \
	(*(volatile uint32_t *)(HW2CPU(addr))) = (value); \
	printf("REG_PL_WR(0x%08x, 0x%08x);\n", addr, value); \
} while(0)
#else
#define REG_PL_WR(addr, value)       (*(volatile uint32_t *)(HW2CPU(addr))) = (value)
#endif

/// Macro to read a 16 bits width platform register, bb access rf
#define REG_PL_RD16(addr)              (*(volatile uint16_t *)(HW2CPU(addr)))

/// Macro to write a 16 bits width platform register, bb access rf
#ifdef DEBUG_BY_REG
#define REG_PL_WR16(addr, value)      do { \
	(*(volatile uint16_t *)(HW2CPU(addr))) = (value); \
	printf("REG_PL_WR16(0x%08x, 0x%04x);\n", addr, value); \
} while(0)
#else
#define REG_PL_WR16(addr, value)       (*(volatile uint16_t *)(HW2CPU(addr))) = (value)
#endif

/// Macro to read a 8 bits width platform register, access system manager
#define REG_PL_RD8(addr)              (*(volatile uint8_t *)(HW2CPU(addr)))

/// Macro to write a 8 bits width platform register, access system manager
#ifdef DEBUG_BY_REG
#define REG_PL_WR8(addr, value)      do { \
	(*(volatile uint8_t *)(HW2CPU(addr))) = (value); \
	printf("REG_PL_WR8(0x%08x, 0x%02x);\n", addr, value); \
} while(0)
#else
#define REG_PL_WR8(addr, value)       (*(volatile uint8_t *)(HW2CPU(addr))) = (value)
#endif

#define REG_RW                  0xFFFFFFFF
#define REG_RWU					0xFFFFFFF0
#define REG_R					0x0
#define REG_TIMER               0x1
#define REG_COUNTER             0x2
#define REG_S					0x4
#define REG_AUTO_RESET          0xF0000000
typedef struct _RegTestData{
	uint32_t reg_offset;
	uint32_t reg_reset_val;
	uint32_t reg_type;
	uint32_t reg_mask;
}RegTestData;
/// @} REG

#endif // _WIFI_REG_ACCESS_H_
