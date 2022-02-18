/**
 ****************************************************************************************
 *
 * @file hal_machw.h
 *
 * @brief Definition of the API used to initialize and control the MAC HW
 *
 * File contains the API to perform the functions of the MAC HW initialization and
 * interrupt handling. This module initializes the MAC HW registers and handles the
 * different interrupts raised by MAC HW.
 *
 * Copyright (C) Siflower 2015-2019
 *
 ****************************************************************************************
 */

#ifndef _HAL_MACHW_H_
#define _HAL_MACHW_H_

/**
 *****************************************************************************************
 * @defgroup HAL HAL
 * @ingroup LMAC
 * @brief MAC HW Abstraction Module
 * @{
 *****************************************************************************************
 */

/**
 *****************************************************************************************
 * @defgroup MACHW MACHW
 * @ingroup HAL
 * @brief MAC HW Control Module
 * @{
 *****************************************************************************************
 */
#include "hal_desc.h"
#include "reg_mac_core.h"


/**
 * DEFINES
 ****************************************************************************************
 */

/// Duration between AP TBTT and Beacon Transmission (in us, should be a multiple of 128us)
#define HAL_MACHW_BCN_TX_DELAY_US       (2048)

/**
 * ENUMERATIONS
 ****************************************************************************************
 */

/// For MAC HW States.
enum
{
    /// MAC HW IDLE State.
    HW_IDLE = 0,
    /// MAC HW RESERVED State.
    HW_RESERVED,
    /// MAC HW DOZE State.
    HW_DOZE,
    /// MAC HW ACTIVE State.
    HW_ACTIVE
};

enum
{
    /// 802.ll b
    MODE_802_11B = 0,
    /// 802.11 a
    MODE_802_11A,
    /// 802.11 g
    MODE_802_11G,
    /// 802.11n at 2.4GHz
    MODE_802_11N_2_4,
    /// 802.11n at 5GHz
    MODE_802_11N_5,
    /// Reserved for future use
    MODE_RESERVED,
    /// 802.11ac at 5GHz
    MODE_802_11AC_5
};

/// Mapping of HW timers
enum
{
    /// AC0 TX timeout
    HAL_AC0_TIMER = 0,
    /// AC1 TX timeout
    HAL_AC1_TIMER,
    /// AC2 TX timeout
    HAL_AC2_TIMER,
    /// AC3 TX timeout
    HAL_AC3_TIMER,
    /// BCN TX timeout
    HAL_BCN_TIMER,
    /// Go to IDLE timeout
    HAL_IDLE_TIMER,
    /// RX interrupt mitigation timeout
    HAL_RX_TIMER,
    #if NX_MM_TIMER
    /// MM timeout
    HAL_MM_TIMER,
    #endif
    /// Kernel timer
    HAL_KE_TIMER,

    /// Number of HW timers used
    HAL_TIMER_MAX,
};

/// Bits associated to HW timers
enum
{
    /// AC0 TX timeout bit
    HAL_AC0_TIMER_BIT = CO_BIT(HAL_AC0_TIMER),
    /// AC1 TX timeout bit
    HAL_AC1_TIMER_BIT = CO_BIT(HAL_AC1_TIMER),
    /// AC2 TX timeout bit
    HAL_AC2_TIMER_BIT = CO_BIT(HAL_AC2_TIMER),
    /// AC3 TX timeout bit
    HAL_AC3_TIMER_BIT = CO_BIT(HAL_AC3_TIMER),
    /// BCN TX timeout bit
    HAL_BCN_TIMER_BIT = CO_BIT(HAL_BCN_TIMER),
    /// Go to IDLE timeout bit
    HAL_IDLE_TIMER_BIT = CO_BIT(HAL_IDLE_TIMER),
    /// RX interrupt mitigation timeout bit
    HAL_RX_TIMER_BIT = CO_BIT(HAL_RX_TIMER),
    #if NX_MM_TIMER
    /// MM timeout bit
    HAL_MM_TIMER_BIT = CO_BIT(HAL_MM_TIMER),
    #endif
    /// Kernel timer bit
    HAL_KE_TIMER_BIT = CO_BIT(HAL_KE_TIMER),
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
#ifdef CFG_RWTL
//TODO
uint32_t tl_diff;
#endif

/**
 ****************************************************************************************
 * @brief Get the current HW time.
 *
 * @return The value of nxmac_monotonic_counter_2_lo register.
 ****************************************************************************************
 */
__INLINE uint32_t hal_machw_time(void)
{
    return nxmac_monotonic_counter_2_lo_get();
}

/**
 ****************************************************************************************
 * @brief Compare timer absolute expiration time.
 *
 * @param[in] time1 First time to compare.
 * @param[in] time2 Second time to compare.
 *
 * @return true if time1 is earlier than time2.
 ****************************************************************************************
 */
__INLINE bool hal_machw_time_cmp(uint32_t time1, uint32_t time2)
{
    uint32_t diff = time1 - time2;

    #ifdef CFG_RWTL
    /// Temporary workaround for the TL4 compiler
    tl_diff = diff;
    #endif

    return (((int32_t)diff) < 0);
}

/**
 ****************************************************************************************
 * @brief Compare timer absolute expiration time.
 *
 * @param[in] time Time to compare.
 *
 * @return true if time is in the past, false otherwise
 ****************************************************************************************
 */
__INLINE bool hal_machw_time_past(uint32_t time)
{
    return (hal_machw_time_cmp(time, hal_machw_time()));
}

/**
 ****************************************************************************************
 * @brief Block execution for the given number of us.
 *
 * @param[in] us Number of us to wait for.
 ****************************************************************************************
 */
__INLINE void hal_machw_udelay(uint32_t us)
{
    uint32_t e = hal_machw_time() + us;
    while (!hal_machw_time_past(e));
}

#if NX_MULTI_ROLE
extern const uint8_t rxv2macrate[];

__INLINE uint32_t hal_machw_rx_duration(struct rx_hd *rhd, uint16_t len)
{
    // Fill-in the TimeOnAir parameter registers
    nxmac_ppdu_mcs_index_setf(rxv2macrate[(rhd->recvec1a >> 12) & 0x0F]);
    nxmac_time_on_air_param_1_pack(0, 0, 0, (rhd->recvec1b >> 15) & 0x01, 0, len);

    // Compute the duration
    nxmac_compute_duration_setf(1);
    #ifdef CFG_RWTL
    // Add a fake read to ensure previous write is performed
    hal_machw_time();
    #endif
    while(nxmac_time_on_air_valid_getf() == 0);
    ASSERT_REC_VAL(nxmac_time_on_air_valid_getf() != 0, 500);

    // Retrieve the duration
    return ((uint32_t)nxmac_time_on_air_getf());
}
#endif

#if (RW_BFMEE_EN)
/**
 ****************************************************************************************
 * @brief Check whether BFMEE is supported by the MAC and PHY HW.
 *
 * @return true if supported, false otherwise
 ****************************************************************************************
 */
__INLINE bool hal_machw_bfmee_support(void)
{
    return (nxmac_bfmee_getf() && phy_bfmee_supported()) != 0;
}
#endif //(RW_BFMEE_EN)

#if (RW_BFMER_EN)
/**
 ****************************************************************************************
 * @brief Check whether BFMER is supported by the MAC and PHY HW.
 *
 * @return true if supported, false otherwise
 ****************************************************************************************
 */
__INLINE bool hal_machw_bfmer_support(void)
{
    return (false);
}
#endif //(RW_BFMER_EN)

/**
 ****************************************************************************************
 * @brief Interrupt Controller initialization.
 * This function initializes the interrupt control registers and enables the interrupts.
 ****************************************************************************************
 */
void hal_machw_init(void);

#if NX_DEBUG_DUMP
/**
 ****************************************************************************************
 * @brief Fill-in debug dump buffer with HW diagnostic port state.
 ****************************************************************************************
 */
void hal_machw_get_diag_state(void);
#endif

/**
 ****************************************************************************************
 * @brief MAC HW reset function.
 * This function is part of the recovery mechanism invoked upon an error detection in the
 * LMAC. It resets the MAC HW state machines, which in consequence resets the PHY.
 ****************************************************************************************
 */
void hal_machw_reset(void);

/**
 ****************************************************************************************
 * @brief Function accessing the HW address search engine.
 *
 * @param[in] addr Pointer to the MAC address to search
 *
 * @return The index of the station found in the address list if any, @ref INVALID_STA_IDX
 * otherwise
 ****************************************************************************************
 */
uint8_t hal_machw_search_addr(struct mac_addr *addr);

/**
 ****************************************************************************************
 * @brief MAC HW interrupt disable function.
 * This function is part of the recovery mechanism invoked upon an error detection in the
 * LMAC. It disables the MAC HW interrupts.
 ****************************************************************************************
 */
void hal_machw_disable_int(void);


/**
 ****************************************************************************************
 * @brief Stop the operation of the MAC HW.
 * This function stops the MAC HW operation and resets it.
 ****************************************************************************************
 */
void hal_machw_stop(void);

/**
 ****************************************************************************************
 * @brief Put back the MAC HW in monitoring mode.
 * This function has to be called when the last configured interface is removed.
 ****************************************************************************************
 */
void hal_machw_monitor_mode(void);


/**
 ****************************************************************************************
 * @brief Check if the HW timers don expire too early to go to sleep.
 *
 * @return true if we can sleep, false otherwise.
 ****************************************************************************************
 */
bool hal_machw_sleep_check(void);

/**
 ****************************************************************************************
 * @brief Interrupt Handling.
 * This function handles the general interrupts raised by the MAC HW.
 ****************************************************************************************
 */
int hal_machw_gen_handler(int irq);


/**
 ****************************************************************************************
 * @brief programs the nextState register of the core.
 * This register is present in the stateControl register of the core.
 * Depending upon the currentState of the core the next state is programmed.
 * If the currentState is other than IDLE it will program IDLE into nextState.
 * If the currentState is IDLE it will program the nextState required.
 * The state to which the core has to be changed is passed as parameter.
 * It also waits for the idle interrupt
 * NOTE: you must enable the general interrupt MAC IDLE bit enable before you use this function to change the state
 *
 * @param uint8_t *wait
 *      if wait is null ,mean the user do not user gentime(IDLE) interrupt, and we will set the state to new state and wait 1ms to see
 *      if the current mac state has been changed
 *      if wait is not null, mean the user use a interrupt and the lmac_change_state function will wait 100us, and then check the
 *         *wait to see if *wait has been changed by the interrupt handler, note user must change the *wait value in the interrupt
 *         handler function
 ****************************************************************************************
 */
bool hal_machw_change_state(int new_state, uint8_t wait);
/**
 ****************************************************************************************
 * @brief Ask to HW to move to IDLE state.
 * This function assumes that the HW is not in IDLE state when it is called.
 ****************************************************************************************
 */
void hal_machw_idle_req(void);

/// @}  // end of group HAL_MACHW
/// @}  // end of group HAL
#endif // _HAL_MACHW_H_
