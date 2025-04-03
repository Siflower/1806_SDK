#ifndef __CMM_ERROR_H__
#define __CMM_ERROR_H__
#if defined(__KERNEL__)
#include <linux/kernel.h>
#endif

/*
 * Include Files
 */
#include "yt_types.h"

/*
 * Symbol Definition
 */

/*
 * Macro Definition
 */
#ifdef __KERNEL__
#define yt_printf printk
#else
#define yt_printf printf
#endif

typedef enum cmm_err_e
{
    CMM_ERR_OK                   = 0,
    CMM_ERR_FAIL                 = 1,
    CMM_ERR_NULL_POINT  = 2,
    CMM_ERR_NOT_SUPPORT,
    CMM_ERR_NOT_INIT,
    CMM_ERR_INPUT = 5,
    CMM_ERR_REG_TABLE_NUM,
    CMM_ERR_REG_TABLE_OP,
    CMM_ERR_TABLE_FULL,
    CMM_ERR_ENTRY_NOT_FOUND,
    CMM_ERR_REG_TABLE_IDX = 10,
    CMM_ERR_SAMEENTRY_EXIST,
    CMM_ERR_ENTRY_FULL,
    CMM_ERR_FDB_OP_BUSY,
    CMM_ERR_PORT,
    CMM_ERR_PORTLIST = 15,
    CMM_ERR_BUSYING_TIME,
    CMM_ERR_EXCEED_RANGE,
    CMM_ERR_TOO_LESS_INFO,
    CMM_ERR_FORBIDDEN,
    CMM_ERR_MAX,
} cmm_err_t;

#define _YT_ERRMSG        \
{                         \
        "Ok",                         /* CMM_ERR_OK */ \
        "Internal error",             /* CMM_ERR_FAIL */ \
        "NULL point error",           /* CMM_ERR_NULL_POINT */ \
        "Not support",                /* CMM_ERR_NOT_SUPPORT */ \
        "Not init error",             /* CMM_ERR_NOT_INIT */ \
        "Invalid input",              /* CMM_ERR_INPUT */ \
        "Table num error",            /* CMM_ERR_REG_TABLE_NUM */ \
        "Invalid operation",          /* CMM_ERR_REG_TABLE_OP */ \
        "Table full",                 /* CMM_ERR_TABLE_FULL */ \
        "Operation not found",        /* CMM_ERR_ENTRY_NOT_FOUND */ \
        "Table index error",          /* CMM_ERR_REG_TABLE_IDX */ \
        "Entry has been exist",       /* CMM_ERR_SAMEENTRY_EXIST */ \
        "Entry full",                 /* CMM_ERR_ENTRY_FULL */ \
        "FDB operation busy",                   /* CMM_ERR_FDB_OP_BUSY */ \
        "Invalid port",               /* CMM_ERR_PORT */ \
        "Invalid portlist",           /* CMM_ERR_PORTLIST */ \
        "Access  phy register busy", /*CMM_ERR_BUSYING_TIME*/ \
        "Exceed allowed range", /*CMM_ERR_EXCEED_RANGE*/ \
        "Unknown error"               /* CMM_ERR_MAX */ \
}

typedef enum _yt_debug_level_e
{
    YT_DEBUG_NONE = 0,
    YT_DEBUG_WARN,
    YT_DEBUG_ERROR,
    YT_DEBUG_MAX
} yt_debug_level_t;

#define _YT_PROMPT_MSG       \
{                         \
        "",                  /* YT_DEBUG_NONE  */ \
        "Warning : ",        /* YT_DEBUG_WARN */ \
        "Error : ",           /* YT_DEBUG_ERROR */ \
        "Unknown level"      /* YT_DEBUG_MAX */ \
}


/* errorcode to str */
extern char *_yt_errmsg[];

#define yt_errmsg(rv)          \
    _yt_errmsg[rv]

/* for debug */
extern uint8_t yt_debug_level;
extern char *_yt_prompt_msg[];

#define YT_PRINT(trace_level, err_code)\
do {\
    yt_printf("%s%s\r\n", \
              _yt_prompt_msg[(0 <= trace_level && YT_DEBUG_MAX > trace_level) ? trace_level : YT_DEBUG_MAX], \
              _yt_errmsg[(0 <= err_code && CMM_ERR_MAX > err_code) ? err_code : CMM_ERR_MAX]); \
} while (0)

#define YT_DEBUG(trace_level, err_code)\
do {\
    if (trace_level <= yt_debug_level) {\
        YT_PRINT(trace_level, err_code); \
    }\
} while (0)

#define CMM_PARAM_CHK(expr, err_code)\
do {\
    if ((uint32_t)(expr)) {\
        YT_DEBUG(YT_DEBUG_ERROR, err_code); \
        return err_code; \
    }\
} while (0)

#define CMM_ERR_HDL(op, err_handle, ret)\
do {\
    if ((ret = (op)) != CMM_ERR_OK)\
    goto err_handle;\
} while(0)

#define CMM_ERR_CHK(op, ret)\
do {\
    if ((ret = (op)) != CMM_ERR_OK)\
    return ret;\
} while(0)

#endif
