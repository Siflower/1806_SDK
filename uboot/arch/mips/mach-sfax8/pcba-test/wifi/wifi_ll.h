/**
 ****************************************************************************************
 *
 * @file ll.h
 *
 * @brief Declaration of low level functions.
 *
 * Copyright (C) Siflower 2015-2019
 *
 ****************************************************************************************
 */

#ifndef _WIFI_LL_H_
#define _WIFI_LL_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
// standard integer functions
#include "co_int.h"


/**
 ****************************************************************************************
 * @brief Enable interrupts globally in the system.
 * This macro must be used when the initialization phase is over and the interrupts
 * can start being handled by the system.
 *
 * **************************************************************************************
 */
//TODO
#define GLOBAL_INT_START()                                 \
    do {                                                       \
    } while(0)


/**
 *****************************************************************************************
 * @brief Disable interrupts globally in the system.
 * This macro must be used when the system wants to disable all the interrupts
 * it could handle.
 *
 *****************************************************************************************
 */

#define GLOBAL_INT_STOP()                                      \
    do {                                                       \
    } while(0)


/**
 *****************************************************************************************
 * @brief Disable interrupts globally in the system.
 * This macro must be used in conjunction with the @ref GLOBAL_INT_RESTORE macro since this
 * last one will close the brace that the current macro opens.  This means that both
 * macros must be located at the same scope level.
 *
 *****************************************************************************************
 */

//TODO
#define GLOBAL_INT_DISABLE()                                                \
do {


/**
 *****************************************************************************************
 * @brief Restore interrupts from the previous global disable.
 * @sa GLOBAL_INT_DISABLE
 *
 *****************************************************************************************
 */

    //TODO
#define GLOBAL_INT_RESTORE()                                                \
} while(0);
#endif // _WIFI_LL_H_
