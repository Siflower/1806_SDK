/**
 ****************************************************************************************
 *
 * @file gcc/compiler.h
 *
 * @brief Definitions of compiler specific directives.
 *
 * Copyright (C) Siflower 2015-2019
 *
 ****************************************************************************************
 */

#ifndef _WIFI_COMPILER_H_
#define _WIFI_COMPILER_H_

/// define the force inlining attribute for this compiler
#define __INLINE static __attribute__((__always_inline__)) inline

/// define the interrupt handler attribute for this compiler
#if 0
#define __IRQ __attribute__((interrupt))
#else
#define __IRQ
#endif

/// define the BT IRQ handler attribute for this compiler
#define __BTIRQ

/// define the BLE IRQ handler attribute for this compiler
#define __BLEIRQ

/// define the interrupt handler attribute for this compiler
#define __FIQ

#define __MODULE__ ""
/// Pack a structure field
#define __PACKED16 __attribute__ ((__packed__))
#define __PACKED __attribute__ ((__packed__))

#define __UNUSED __attribute__ ((unused))

/// Align instantiated lvalue or struct member on 4 bytes
#define __ALIGN4 __attribute__((aligned(4)))
/// Align instantiated lvalue or struct member on 8 bytes
#define __ALIGN8 __attribute__((aligned(8)))

/// function returns struct in registers (4 words max, var with gnuarm)
#define __VIR

// Mapping of these different elements is already handled in the map.txt file, so no need
// to define anything here
#define __SHAREDRAMIPC
#define __SHAREDRAM
#define __LARAMMAC
#define __LARAMMAC
#define __LARAMPHY
#define __MIB

#endif // _WIFI_COMPILER_H_
