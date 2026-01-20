#ifndef __CMM_ERROR_H__
#define __CMM_ERROR_H__
#if defined(__KERNEL__)
#include <linux/kernel.h>
#endif

/*
 * Include Files
 */
#include "yt_types.h"
#ifndef OS_FREERTOS
#include "yt_lock.h"
#endif

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
        "FDB operation busy",         /* CMM_ERR_FDB_OP_BUSY */ \
        "Invalid port",               /* CMM_ERR_PORT */ \
        "Invalid portlist",           /* CMM_ERR_PORTLIST */ \
        "Access  phy register busy",  /*CMM_ERR_BUSYING_TIME*/ \
        "Exceed allowed range",       /*CMM_ERR_EXCEED_RANGE*/ \
        "Port has been in lag port",  /*CMM_ERR_PORT_BEEN_IN_LAG*/ \
        "Too less info",              /*CMM_ERR_TOO_LESS_INFO*/ \
        "ACL module not init",        /*CMM_ERR_ACL_NOT_INIT*/ \
        "ACL instance not found",     /*CMM_ERR_ACL_INSTANCE_NOT_FOUND*/ \
        "ACL instance is full",       /*CMM_ERR_ACL_INSTANCE_FULL*/ \
        "ACL keymask not support",    /*CMM_ERR_ACL_KEY_SELECT_FAIL*/ \
        "ACL entry not found",        /*CMM_ERR_ACL_ENTRY_NOT_FOUND*/ \
        "ACL bin not enough",         /*CMM_ERR_ACL_BIN_NOT_ENOUGH*/ \
        "ACL invalid key on current instance",   /*CMM_ERR_ACL_INVALID_KEY_ON_INSTANCE*/ \
        "ACL operation is busy",      /*CMM_ERR_ACL_OP_BUSY*/ \
        "Forbidden operation",        /*CMM_ERR_FORBIDDEN*/ \
        "Entry over lapping",         /*CMM_ERR_ENTRY_OVERLAPPING*/ \
        "Entry invalid",              /*CMM_ERR_ENTRY_INVALID*/ \
        "Unknown error"               /* CMM_ERR_MAX */ \
}

typedef enum _yt_debug_level_e
{
    YT_DEBUG_DEBUG = 0,
    YT_DEBUG_INFO,
    YT_DEBUG_WARN,
    YT_DEBUG_ERROR,
    YT_DEBUG_MAX
} yt_debug_level_t;

#define _YT_PROMPT_MSG       \
{                         \
        "Debug: ",                  /* YT_DEBUG_DEBUG  */ \
        "Info: ",                /* YT_DEBUG_INFO */ \
        "Warning : ",        /* YT_DEBUG_WARN */ \
        "Error : ",           /* YT_DEBUG_ERROR */ \
        "Unknown level"      /* YT_DEBUG_MAX */ \
}


/* errorcode to str */
extern char * const _yt_errmsg[];

#define yt_errmsg(rv)          \
    _yt_errmsg[rv]

/* for debug */
extern uint8_t yt_debug_level;
extern char * const _yt_prompt_msg[];

#define YT_PRINT(trace_level, err_code)\
do {\
    yt_printf("%s%s\r\n", \
              _yt_prompt_msg[(0 <= trace_level && YT_DEBUG_MAX > trace_level) ? trace_level : YT_DEBUG_MAX], \
              _yt_errmsg[(0 <= err_code && CMM_ERR_MAX > err_code) ? err_code : CMM_ERR_MAX]); \
} while (0)

#define YT_DEBUG(trace_level, err_code)\
do {\
    if (trace_level >= yt_debug_level) {\
        YT_PRINT(trace_level, err_code); \
    }\
} while (0)

#define CMM_PARAM_CHK(expr, err_code)\
do {\
    if ((uint32_t)(expr)) {\
        YT_DEBUG(YT_DEBUG_ERROR, err_code); \
	printk("%s %d \n", __func__, __LINE__); \
        return err_code; \
    }\
} while (0)

#define CMM_ERR_HDL(op, err_handle, ret)\
do {\
    if ((ret = (op)) != CMM_ERR_OK)\
    goto err_handle;\
} while(0)

#define CMM_ERR_UNLOCK(op, lockid, ret)\
do {\
    if ((ret = (op)) != CMM_ERR_OK) {\
        if (lockid < YT_LOCK_ID_MAX) {\
            osal_mux_unlock(&YT_LOCK_ID(lockid));\
        }\
        return ret;\
    }\
} while(0)

#define CMM_ERR_CHK(op, ret)\
do {\
    if ((ret = (op)) != CMM_ERR_OK)\
    return ret;\
} while(0)

#define CMM_ACL_ERR_CHK_WITH_FREE_INSTANCE(op, lockid, ret, instance)\
do {\
    if ((ret = (op)) != CMM_ERR_OK){\
        if (instance != NULL){\
            osal_free(instance, YT_MEM_MALLOC_MODULE_ACL);\
            instance = NULL;\
        }\
        if (lockid < YT_LOCK_ID_MAX) {\
            osal_mux_unlock(&YT_LOCK_ID(lockid));\
        }\
        return ret;\
    }\
} while(0)

#define CMM_ACL_ERR_CHK_WITH_FREE_BIN(op, ret, bin)\
do {\
    if ((ret = (op)) != CMM_ERR_OK){\
        if (bin != NULL){\
            osal_free(bin, YT_MEM_MALLOC_MODULE_ACL);\
            bin = NULL;\
        }\
        return ret;\
    }\
} while(0)

#define CMM_ACL_ERR_CHK_WITH_FREE_INSTANCE_BIN(op, lockid, ret, instance, bin)\
do {\
    if ((ret = (op)) != CMM_ERR_OK){\
        if (instance != NULL){\
            osal_free(instance, YT_MEM_MALLOC_MODULE_ACL);\
            instance = NULL;\
        }\
        if (bin != NULL){\
            osal_free(bin, YT_MEM_MALLOC_MODULE_ACL);\
            bin = NULL;\
        }\
        if (lockid < YT_LOCK_ID_MAX) {\
            osal_mux_unlock(&YT_LOCK_ID(lockid));\
        }\
        return ret;\
    }\
} while(0)

#define CMM_ACL_ERR_CHK_WITH_FREE_ENTRY(op, lockid, ret, entry)\
do {\
    if ((ret = (op)) != CMM_ERR_OK){\
        if (entry != NULL){\
            osal_free(entry, YT_MEM_MALLOC_MODULE_ACL);\
            entry = NULL;\
        }\
        if (lockid < YT_LOCK_ID_MAX) {\
            osal_mux_unlock(&YT_LOCK_ID(lockid));\
        }\
        return ret;\
    }\
} while(0)

#endif

