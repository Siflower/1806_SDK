/*******************************************************************************
*                                                                              *
*  Copyright (c), 2025, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef BPROFILE_DEFAULT_DEMO_H__
#define BPROFILE_DEFAULT_DEMO_H__
#include "yt_types.h"

#ifdef SWITCH_SERIES_SHARK
#if defined(BOARD_YT9232_24G_4GC_DEMO)  || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9232_24g_4gc_register(void);
#endif
#if defined(BOARD_YT9232_24G_4GE_DEMO)  || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9232_24g_4ge_register(void);
#endif
#if defined(BOARD_YT9232_24G_DEMO)  || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9232_24g_register(void);
#endif
#if defined(BOARD_YT9232_24G_4FIB_DEMO)  || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9232_24g_4fib_register(void);
#endif
#if defined(BOARD_YT9232_24G_2GE_2FIB_DEMO)  || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9232_24g_2ge_2fib_register(void);
#endif
#if defined(BOARD_YT9232_24G_2FIB_DEMO)  || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9232_24g_2fib_register(void);
#endif
#if defined(BOARD_YT9232_16G_DEMO)  || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9232_16g_register(void);
#endif
#if defined(BOARD_YT9232_16G_2GC_DEMO)  || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9232_16g_2gc_register(void);
#endif
#if defined(BOARD_YT9232_16G_2FIB_DEMO)  || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9232_16g_2fib_register(void);
#endif
#if defined(BOARD_YT9231_DEFAULT_DEMO)  || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9231_register(void);
#endif
#if defined(BOARD_YT9231_6FIB_DEMO)  || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9231_6fib_register(void);
#endif
#if defined(BOARD_YT9230_DEFAULT_DEMO)  || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9230_register(void);
#endif
#endif


#ifdef SWITCH_SERIES_TIGER
#if defined(BOARD_YT9218N_DEFAULT_DEMO) || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9218n_register(void);
#endif
#if defined(BOARD_YT9218MB_YT8531_DEMO) || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9218mb_yt8531_register(void);
#endif
#if defined(BOARD_YT9218MB_DEFAULT_DEMO) || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9218mb_register(void);
#endif
#if defined(BOARD_YT9215SC_DEFAULT_DEMO) || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9215sc_register(void);
#endif
#if defined(BOARD_YT9215S_YT8531_FIB_DEMO) || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9215s_yt8531_fib_register(void);
#endif
#if defined(BOARD_YT9215S_FIB_DEMO) || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9215s_fib_register(void);
#endif
#if defined(BOARD_YT9215RB_YT8531_DEMO) || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9215rb_yt8531_register(void);
#endif
#if defined(BOARD_YT9215RB_DEFAULT_DEMO) || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9215rb_register(void);
#endif
#if defined(BOARD_YT9215_DEFAULT_DEMO) || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9215_register(void);
#endif
#if defined(BOARD_YT9214NB_DEFAULT_DEMO) || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9214nb_register(void);
#endif
#if defined(BOARD_YT9213NB_DEFAULT_DEMO) || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9213nb_register(void);
#endif
#if defined(BOARD_YT921X_MULTI_SW_DEFAULT_DEMO) || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt921x_multi_sw_register(void);
#endif
#endif


#ifdef SWITCH_SERIES_WHALE
#if defined(BOARD_YT9228_DEFAULT_DEMO) || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9228_register(void);
#endif
#if defined(BOARD_YT9224_DEFAULT_DEMO) || defined(BOARD_SELECT_ALL)
yt_ret_t yt_board_profile_yt9224_register(void);
#endif
#endif

#endif