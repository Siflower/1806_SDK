/*================================================================
*                                                                             *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.              *
*  Motorcomm Confidential and Proprietary.                                    *
*                                                                             *
================================================================*/

#ifndef _YT_LOCK_H
#define _YT_LOCK_H

#include "yt_types.h"
#include "osal_sem.h"

typedef enum _yt_sem_e
{
    YT_DOT1X_LOCK_ID = 0,
    YT_QOS_LOCK_ID,
    YT_RMA_LOCK_ID,
    YT_L2_LOCK_ID,
    YT_ACL_LOCK_ID,
    YT_VLAN_LOCK_ID,
    YT_PORT_LOCK_ID,
    YT_STAT_LOCK_ID,
    YT_ACCESS_LOCK_ID,
    YT_INT_SMI_LOCK_ID,
    YT_EXT_SMI_LOCK_ID,
    YT_LOCK_ID_MAX
} yt_sem_t;

/* for common use */
#define YT_LOCK_ID(id)              yt_lock[id]
#define YT_LOCK_INIT(id)            osal_mux_init(&YT_LOCK_ID(id), NULL)
#define YT_LOCK(id)                 osal_mux_lock(&YT_LOCK_ID(id))
#define YT_UNLOCK(id)               osal_mux_unlock(&YT_LOCK_ID(id))

/* for dot1x */
#define DOT1X_LOCK_INIT()           YT_LOCK_INIT(YT_DOT1X_LOCK_ID)
#define DOT1X_LOCK()                osal_mux_lock(&YT_LOCK_ID(YT_DOT1X_LOCK_ID))
#define DOT1X_UNLOCK()              osal_mux_unlock(&YT_LOCK_ID(YT_DOT1X_LOCK_ID))

/* for l2 */
#define L2_LOCK_INIT()           YT_LOCK_INIT(YT_L2_LOCK_ID)
#define L2_LOCK()                osal_mux_lock(&YT_LOCK_ID(YT_L2_LOCK_ID))
#define L2_UNLOCK()              osal_mux_unlock(&YT_LOCK_ID(YT_L2_LOCK_ID))

/* for port */
#define PORT_LOCK_INIT()           YT_LOCK_INIT(YT_PORT_LOCK_ID)
#define PORT_LOCK()                osal_mux_lock(&YT_LOCK_ID(YT_PORT_LOCK_ID))
#define PORT_UNLOCK()              osal_mux_unlock(&YT_LOCK_ID(YT_PORT_LOCK_ID))

/* for mib */
#define STAT_LOCK_INIT()           YT_LOCK_INIT(YT_STAT_LOCK_ID)
#define STAT_LOCK()                osal_mux_lock(&YT_LOCK_ID(YT_STAT_LOCK_ID))
#define STAT_UNLOCK()              osal_mux_unlock(&YT_LOCK_ID(YT_STAT_LOCK_ID))

/* for acl */
#define ACL_LOCK_INIT()           YT_LOCK_INIT(YT_ACL_LOCK_ID)
#define ACL_LOCK()                osal_mux_lock(&YT_LOCK_ID(YT_ACL_LOCK_ID))
#define ACL_UNLOCK()              osal_mux_unlock(&YT_LOCK_ID(YT_ACL_LOCK_ID))

/* for switch access */
#define ACCESS_LOCK_INIT()          YT_LOCK_INIT(YT_ACCESS_LOCK_ID)
#define ACCESS_LOCK()               osal_mux_lock(&YT_LOCK_ID(YT_ACCESS_LOCK_ID))
#define ACCESS_UNLOCK()             osal_mux_unlock(&YT_LOCK_ID(YT_ACCESS_LOCK_ID))

/* for smi(mdio) access */
#define INT_SMI_LOCK_INIT()          YT_LOCK_INIT(YT_INT_SMI_LOCK_ID)
#define INT_SMI_LOCK()               osal_mux_lock(&YT_LOCK_ID(YT_INT_SMI_LOCK_ID))
#define INT_SMI_UNLOCK()             osal_mux_unlock(&YT_LOCK_ID(YT_INT_SMI_LOCK_ID))
#define EXT_SMI_LOCK_INIT()          YT_LOCK_INIT(YT_EXT_SMI_LOCK_ID)
#define EXT_SMI_LOCK()               osal_mux_lock(&YT_LOCK_ID(YT_EXT_SMI_LOCK_ID))
#define EXT_SMI_UNLOCK()             osal_mux_unlock(&YT_LOCK_ID(YT_EXT_SMI_LOCK_ID))

extern osal_mux  yt_lock[YT_LOCK_ID_MAX];

#endif //YT_LOCK_H
