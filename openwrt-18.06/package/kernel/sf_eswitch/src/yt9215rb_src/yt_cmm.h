#ifndef __YT_CMM_H__
#define __YT_CMM_H__

#include "yt_types.h"
#include "yt_util.h"
#include "yt_error.h"
#include "yt_lock.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "cal_cmm.h"

#define YT_UNIT_NUM                 CAL_MAX_UNIT_NUM
#define YT_PORT_NUM                 CAL_MAX_PORT_NUM
#define YT_VLAN_NUM_MAX             CAL_VLAN_NUM_MAX
#define YT_L2_FDB_NUM_MAX       CAL_L2_FDB_NUM_MAX
#define YT_LOG_ALL_PORT_MASK(unit)        CAL_YT_ALL_PORT_MASK(unit)
#define YT_MAC_ALL_PORT_MASK(unit)        CAL_MAC_ALL_PORT_MASK(unit)

typedef enum yt_fwd_type_e {
    FWD_TYPE_FWD,
    FWD_TYPE_DROP,
    FWD_TYPE_COPY,
    FWD_TYPE_TRAP,
}yt_fwd_type_t;

typedef enum yt_enable_e {
    YT_DISABLE = 0,
    YT_ENABLE = 1,
}yt_enable_t;

typedef enum  yt_l2_type_e
{
    L2_ETHV2 = 1,
    L2_ETHSAP,
    L2_ETHSNAP,
    L2_ETHMAX
}yt_l2_type_t;


#endif
