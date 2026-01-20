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
#define YT_PORT_NUM_ON_UNIT(unit)                 CAL_MAX_PORT_NUM_ON_UNIT(unit)
#define YT_VLAN_NUM_MAX             CAL_VLAN_NUM_MAX
#define YT_L2_FDB_NUM_MAX(unit)       CAL_L2_FDB_NUM_MAX(unit)
#define YT_LOG_ALL_PORT_MASK(unit)        CAL_YT_ALL_PORT_MASK(unit)
#define YT_MAC_ALL_PORT_MASK(unit)        CAL_MAC_ALL_PORT_MASK(unit)
#define YT_TPID_PROFILE_NUM             CAL_VLAN_TPID_PROFILE_NUM
#define YT_PROTOCOL_BASED_VLAN_NUM(unit)      CAL_PROTOCOL_BASED_VLAN_NUM(unit)
#define YT_VLAN_IGR_TRANS_MAX_ENTRY_NUM(unit)      CAL_VLAN_XLATE_ENTRY_NUM(unit)
#define YT_VLAN_EGR_TRANS_MAX_ENTRY_NUM(unit)      CAL_VLAN_EGR_XLATE_TBL_NUM(unit)
#define YT_LAG_NUM_MAX       CHIP_MAX_LAG_NUM
#define YT_MIRROR_MAX_GROUP_NUM                     CAL_MIRROR_MAX_GROUP_NUM
#define YT_MIRROR_INVALID_PORT                      CAL_MIRROR_INVALID_PORT
#define YT_REMOTE_NUM_MAX                           CAL_REMOTE_NUM_MAX
#define YT_STP_INSTANCE_NUM                         CAL_STP_INSTANCE_NUM
#define YT_ACL_NUM_MAX             CAL_ACL_NUM_MAX

#endif

