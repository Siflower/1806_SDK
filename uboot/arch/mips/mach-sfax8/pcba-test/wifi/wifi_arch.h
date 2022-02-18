/**
 ****************************************************************************************
 *
 * @file arch.h
 *
 * @brief This file contains the definitions of the macros and functions that are
 * architecture dependent.  The implementation of those is implemented in the
 * appropriate architecture directory.
 *
 * Copyright (C) Siflower 2015-2019
 *
 ****************************************************************************************
 */

#ifndef _WIFI_ARCH_H_
#define _WIFI_ARCH_H_

/**
 ****************************************************************************************
 * @defgroup PLATFORM_DRIVERS PLATFORM_DRIVERS
 * @ingroup MACSW
 * @brief Declaration of the platform API.
 * @{
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @defgroup WIFI_ARCH_APS3 WIFI_ARCH_APS3
 * @ingroup PLATFORM_DRIVERS
 * @brief Declaration of the APS3 platform architecture API.
 * @{
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "wifi_ll.h"
#include "wifi_reg_access.h"

#ifdef CLIB
#include <stdio.h>
#endif

#ifdef CFG_RWTL
#define RWTL_LOG_ADDR SIFLOWER_SIM_LOG_BASE
#define RWTL_LOG_LAST_STATUS_ADDR SIFLOWER_SIM_LOG_TRIG
#endif
#define RWTL_LOG_FAILED 0x77
#define RWTL_LOG_SUCCESS 0x88
#define RWTL_LOG_TIMEOUT 0x99
#define RWTL_LOG_ROOM_SIZE 31

#define SHARE_MEM_BASE (WIFI_BASE_ADDR+0x00000000)

#ifdef CFG_RWTL
	#define dbg(fmt, ...)   do {} while (0)
#else
	#define dbg(fmt, ...) printf(fmt, ## __VA_ARGS__)
#endif
/*
 * ASSERTION CHECK
 ****************************************************************************************
 */
void force_trigger(const char *msg);
void assert_rec(const char *condition, const char * file, int line);
void assert_warn(const char *condition, const char * file, int line);
static inline void assert_err(const char *condition, const char *file, int line)
{
	printf("%s line:%d %s\n", file, line, condition);
};

#if (NX_DEBUG > 1)
#define ASSERT(type, cond) assert_##type(cond, __func__, __LINE__)
#elif (NX_DEBUG > 0)
#define ASSERT(type, cond) assert_##type("", __func__, __LINE__)
#else
#define ASSERT(type, cond) assert_##type("", "", 0)
#endif

#if NX_DEBUG
/// Assertions showing a critical error that could require a full system reset
#define ASSERT_ERR(cond)                              \
    do {                                              \
        if (!(cond)) {                                \
            ASSERT(err, #cond);                       \
        }                                             \
    } while(0)

/// Assertions showing a critical error that could require a full system reset
#define ASSERT_ERR2(cond, param0, param1) ASSERT_ERR(cond)

/// Assertions showing a non-critical problem that has to be fixed by the SW
#define ASSERT_WARN(cond)                             \
    do {                                              \
        if (!(cond)) {                                \
            ASSERT(warn, #cond);                      \
        }                                             \
    } while(0)
#else
#define ASSERT_ERR(cond)
#define ASSERT_ERR2(cond, param0, param1)
#define ASSERT_WARN(cond)
#endif

#if NX_RECOVERY
/// Assertions that trigger the automatic recovery mechanism and return void
#define ASSERT_REC(cond)                              \
    ({                                                \
        if (!(cond)) {                                \
            ASSERT(rec, #cond);                       \
            return;                                   \
        }                                             \
    })

/// Assertions that trigger the automatic recovery mechanism and return a value
#define ASSERT_REC_VAL(cond, ret)                     \
    ({                                                \
        if (!(cond)) {                                \
            ASSERT(rec, #cond);                       \
            return (ret);                             \
        }                                             \
    })

/// Assertions that trigger the automatic recovery mechanism and do not return
#define ASSERT_REC_NO_RET(cond)                       \
    ({                                                \
        if (!(cond)) {                                \
            ASSERT(rec, #cond);                       \
        }                                             \
    })

#else
/// Assertions that trigger the automatic recovery mechanism and return void
#define ASSERT_REC(cond)             ASSERT_ERR(cond)

/// Assertions that trigger the automatic recovery mechanism and return a value
#define ASSERT_REC_VAL(cond, ret)    ASSERT_ERR(cond)

/// Assertions that trigger the automatic recovery mechanism and do not return
#define ASSERT_REC_NO_RET(cond)      ASSERT_ERR(cond)
#endif

/// Object allocated in shared memory - check linker script
#define __SHARED __attribute__ ((section("shram")))

/// Macro checking if a pointer is part of the shared RAM
#define TST_SHRAM_PTR(ptr) ((((uint32_t)(ptr)) < (uint32_t)_sshram) ||                   \
                            (((uint32_t)(ptr)) >= (uint32_t)_eshram))

/// Macro checking if a pointer is part of the shared RAM
#define CHK_SHRAM_PTR(ptr) { if (TST_SHRAM_PTR(ptr)) return;}

#ifndef CLIB
extern void *memcpy (void *dst0, const void *src0, size_t len0);
extern void __attribute__((optimize("O0"))) * memset (void *m, int c, size_t n);
#endif

void wifi_sleep_us(int count);
void wifi_sleep_ms(int count);
void wifi_sleep_s(int count);
void wifi_mem_set(uint8_t *p, uint8_t value, uint32_t size);
void wifi_mem_cpy(uint8_t *dst, uint8_t *src, uint32_t size);
void wifi_mem_cpy32(uint32_t *dst, uint32_t *src, uint32_t size);
void wifi_record_error(char *msg, uint8_t error_size, void *error_buf);


void echo_byte(int offset, char value);
void echo_int(int offset, int value);
char read_byte(int offset);
void wifi_test_end(void);

//set wifi iram select mode: enb=1 for LA mem mode enabled, enb=0 for normal iram mode
void wifi_iram_la_mode_enable(int enb);

void wifi_sim_hwmem_op_save(uint32_t start_addr, uint32_t length);
void wifi_sim_hwmem_op_load(uint32_t start_addr, uint32_t length);
void wifi_sim_hwmem_op_reset(void);
/// @}
/// @}
#endif // _WIFI_ARCH_H_
