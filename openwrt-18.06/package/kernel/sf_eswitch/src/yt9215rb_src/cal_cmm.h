/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef __CAL_COMMON_H__
#define __CAL_COMMON_H__

#include "yt_types.h"
#include "cal_bprofile.h"
#include "cal_mgm.h"
#include "chipdef.h"
#include "yt_phy.h"
#include "phy_drv.h"

typedef enum cal_port_type_e {
    CAL_PORT_TYPE_INTPHY,
    CAL_PORT_TYPE_EXT,
    CAL_PORT_TYPE_END
}cal_port_type_t;

#define UNITINFO(unit)	gpSwitchUnit[unit]

/* hw board resource */
#define CAL_MAX_UNIT_NUM                gBoardInfo.switch_count
#define CAL_MAX_PORT_NUM_ON_UNIT(unit)                (UNITINFO(unit)->pChipIfInfo->allif_num)
#define CAL_VLAN_NUM_MAX                CHIP_VLAN_NUM_MAX
#define CAL_VLAN_TPID_PROFILE_NUM       CHIP_VLAN_TPID_PROFILE_NUM
#define CAL_REMOTE_NUM_MAX              CHIP_REMOTE_NUM_MAX
#define CAL_FLOW_NUM_MAX                CHIP_FLOW_NUM_MAX
#define CAL_MAX_CASCADE_PORT_NUM        CHIP_MAX_CASCADE_PORT_NUM
#define CAL_INVALID_CASCADE_PORT        CHIP_INVALID_CASCADE_PORT
#define CAL_CASCADE_FWD_PORTMASK        CHIP_CASCADE_FWD_PORTMASK
#define CAL_JUMBO_SIZE_MAX              CHIP_JUMBO_SIZE_MAX
#define CAL_JUMBO_SIZE_MIN              CHIP_JUMBO_SIZE_MIN
#define CAL_PKTGAP_SIZE_MAX           CHIP_PKTGAP_SIZE_MAX
#define CAL_PKTGAP_SIZE_MIN           CHIP_PKTGAP_SIZE_MIN
#define CAL_MIRROR_MAX_GROUP_NUM        CHIP_MIRROR_MAX_GROUP_NUM
#define CAL_MIRROR_INVALID_PORT         CHIP_MIRROR_INVALID_PORT
#define CAL_STP_INSTANCE_NUM            CHIP_STP_INSTANCE_NUM
#define CAL_EEE_WAKEUP_TIMER_MAX    CHIP_EEE_WAKEUP_TIMER_MAX
#define CAL_EEE_SLEEP_TIMER_MAX    CHIP_EEE_SLEEP_TIMER_MAX
#define CAL_ACL_NUM_MAX                 CHIP_ACL_NUM_MAX

#define CAL_UNIT_COUNT  (gBoardInfo.switch_count)
#define CAL_PORT_NUM_ON_UNIT(unit)  (UNITINFO(unit)->port_num)
#define CAL_SWCHIP_MODEL(unit)  UNITINFO(unit)->chip_model
#define CAL_SWCHIP_ID(unit)  UNITINFO(unit)->chip_id
#define CAL_BOARD_ID        (gBoardInfo.pIdentifier->id)

#define CMM_IS_MEMBER_PORT(portmask, port, result) \
    do {\
        uint8_t u1Int32Pos;\
        uint8_t u1BitPos;\
        u1Int32Pos = (uint8_t)(port / YT_PORTS_WORD_SIZE);\
        u1BitPos = (uint8_t)(port % YT_PORTS_WORD_SIZE);\
        if ((u1Int32Pos < YT_PORTS_WORD_NUM) &&\
            (portmask.portbits[u1Int32Pos]  & (1<<u1BitPos)) != 0){\
            result = TRUE;\
        }\
        else{\
            result = FALSE;\
        }\
    }while(0)

#define CMM_SET_MEMBER_PORT(portmask, port) \
do {\
    uint8_t u1Int32Pos;\
    uint8_t u1BitPos;\
    u1Int32Pos = (uint8_t)(port / YT_PORTS_WORD_SIZE);\
    u1BitPos = (uint8_t)(port % YT_PORTS_WORD_SIZE);\
    if (u1Int32Pos < YT_PORTS_WORD_NUM){\
        portmask.portbits[u1Int32Pos] |= (1<<u1BitPos);\
    }\
} while (0)

#define CMM_RESET_MEMBER_PORT(portmask, port) \
do {\
    uint8_t u1Int32Pos;\
    uint8_t u1BitPos;\
    u1Int32Pos = (uint8_t)(port / YT_PORTS_WORD_SIZE);\
    u1BitPos = (uint8_t)(port % YT_PORTS_WORD_SIZE);\
    if (u1Int32Pos < YT_PORTS_WORD_NUM){\
        portmask.portbits[u1Int32Pos] &= ~(1<<u1BitPos);\
    }\
} while (0)

#define CMM_CLEAR_MEMBER_PORT(portmask) \
do {\
    uint8_t u1Pos;\
    for (u1Pos = 0; u1Pos < YT_PORTS_WORD_NUM; u1Pos++){\
        portmask.portbits[u1Pos] = 0;\
    }\
} while (0)


/* for yt api */
#define CMM_PORT_VALID(unit, port)    ((gcal_inited) && (port < UNITINFO(unit)->port_num))
#define CMM_PLIST_VALID(unit, portlist)   (cal_ytportmask_isvalid(unit, portlist, FALSE)==CMM_ERR_OK ? TRUE : FALSE)
#define CMM_PLIST_UPDATE(unit, port, flag) (cal_ytportmask_update(unit, port, flag))
#define CMM_PLIST_CPUPORT_VALID(unit, portlist)   (cal_ytportmask_isvalid(unit, portlist, TRUE)==CMM_ERR_OK ? TRUE : FALSE)

/* for fal api */
/* get mac id from yt port */
#define CAL_YTP_TO_MAC(unit, port)	\
	(CMM_PORT_VALID(unit,port) ? UNITINFO(unit)->pPortDescp[port]->mac_id : INVALID_ID)

/* get phy address by phyType,  int phy get int phy address, intext or ext phy get external phy address */
#define CAL_YTP_TO_PHYADDR(unit, port) \
    cal_phy_addr_get(unit, port)

/* get internal phy address */
#define CAL_YTP_TO_INTPHYADDR(unit, port) \
    ((UNITINFO(unit)->pPortDescp[port]->phy_addr) & 0xff)

/* get external phy address */
#define CAL_YTP_TO_EXTPHYADDR(unit, port) \
    (((UNITINFO(unit)->pPortDescp[port]->phy_addr) >> 8) & 0xff)

#define CAL_YTP_TO_PHYTYPE(unit, port) \
    (UNITINFO(unit)->pPortDescp[port]->phyType)

/* get extend port id from yt port */
#define CAL_YTP_TO_EXTPORT(unit, port) \
    (chipdef_get_extif_by_macid(CAL_YTP_TO_MAC(unit, port), (UNITINFO(unit)->pChipIfInfo)))

/* check yt port is internal phy port or extend port */
#define CAL_YTP_PORT_TYPE(unit, port) \
    (CAL_YTP_TO_EXTPORT(unit, port) == INVALID_ID ? CAL_PORT_TYPE_INTPHY : CAL_PORT_TYPE_EXT)

/* convert yt portmask to mac portmask */
#define CAL_YTPLIST_TO_MLIST(unit, portlist, maclist) \
do {\
    uint8_t _index = 0;\
    uint8_t _ismember = FALSE;\
    CMM_CLEAR_MEMBER_PORT(maclist);\
    for(_index = 0; _index < UNITINFO(unit)->port_num; _index++){\
        CMM_IS_MEMBER_PORT(portlist, _index, _ismember);\
        if(TRUE == _ismember){\
             CMM_SET_MEMBER_PORT(maclist, UNITINFO(unit)->pPortDescp[_index]->mac_id);\
        }\
    }\
} while (0)

/* convert mac port to yt port */
#define CAL_MAC_TO_YTP(unit, macid, ytport) \
do {\
    uint8_t _index;\
    ytport = INVALID_ID;\
    for(_index = 0; _index < UNITINFO(unit)->port_num; _index++){\
        if(UNITINFO(unit)->pPortDescp[_index]->mac_id == macid){\
            ytport = _index;\
            break;\
        }\
    }\
} while (0)

/* convert mac portmask to yt portmask */
#define CAL_MLIST_TO_YTPLIST(unit, maclist, portlist) \
do {\
    uint8_t _index = 0;\
    uint8_t _ismember = FALSE;\
    CMM_CLEAR_MEMBER_PORT(portlist);\
    for(_index = 0; _index < UNITINFO(unit)->port_num; _index++){\
        CMM_IS_MEMBER_PORT(maclist, UNITINFO(unit)->pPortDescp[_index]->mac_id, _ismember);\
        if(TRUE == _ismember){\
            CMM_SET_MEMBER_PORT(portlist, _index);\
        }\
    }\
} while (0)


#define CAL_PHYCHIP_MODEL(unit, ytport) phy_model_get_by_port(unit, ytport)
#define CAL_PORT_ATTRIBUTE(unit, ytport) \
    (CMM_PORT_VALID(unit, ytport) ? UNITINFO(unit)->pPortDescp[ytport]->attribute : INVALID_ID)
#define CAL_IS_SERDES(unit, ytport) phy_is_serdes(unit, port)
#define CAL_IS_COMBO_PORT(unit, ytport) cal_is_combo_port(unit, ytport)
#define CAL_IS_PHY_PORT(unit, ytport) \
    ((YT_PHY_MODEL_END > phy_model_get_by_port(unit, ytport)) ? TRUE : FALSE)
#define CAL_PORT_MEDIUM(unit, ytport) \
    (CMM_PORT_VALID(unit, ytport) ? UNITINFO(unit)->pPortDescp[ytport]->medium : INVALID_ID)
#define CAL_IS_YTPHY(unit, ytport) \
    (YT_PHY_MODEL_END > phy_model_get_by_port(unit, ytport))
#define CAL_PORT_EXTIFMODE(unit, ytport) \
    (CMM_PORT_VALID(unit, ytport) ? UNITINFO(unit)->pPortDescp[ytport]->extifMode : INVALID_ID)

/* board related resources */
#define CAL_YT_ALL_PORT_MASK(unit)              gBoardInfo.portmask[unit]
#define CAL_MAC_ALL_PORT_MASK(unit)             gBoardInfo.macmask[unit]
/* chip related resources */
#define CAL_MAX_UCAST_QUEUE_NUM(unit)           UNITINFO(unit)->pChipCap->max_ucast_queue_num
#define CAL_MAX_MCAST_QUEUE_NUM(unit)           UNITINFO(unit)->pChipCap->max_mcast_queue_num
#define CAL_MAX_METER_ENTRY_NUM(unit)           UNITINFO(unit)->pChipCap->max_meter_entry_num
#define CAL_MAX_VALUE_INT_PRI(unit)             UNITINFO(unit)->pChipCap->max_value_of_int_pri
#define CAL_MAX_VALUE_INT_DROP(unit)            UNITINFO(unit)->pChipCap->max_value_of_int_drop
#define CAL_MAX_VALUE_DSCP(unit)                UNITINFO(unit)->pChipCap->max_value_of_dscp
#define CAL_VLAN_XLATE_ENTRY_NUM(unit)          UNITINFO(unit)->pChipCap->max_vlan_xlate_entry_num
#define CAL_VLAN_EGR_XLATE_TBL_NUM(unit)        UNITINFO(unit)->pChipCap->max_vlan_egr_xlate_tbl_num
#define CAL_PROTOCOL_BASED_VLAN_NUM(unit)       UNITINFO(unit)->pChipCap->max_protocol_vlan_tbl_num
#define CAL_MAX_VALUE_MSTI(unit)                UNITINFO(unit)->pChipCap->max_value_of_msti
#define CAL_MAX_VLAN_RANGE_PROFILE_NUM(unit)    UNITINFO(unit)->pChipCap->max_vlan_range_profile_num
#define CAL_MAX_VLAN_METER_ENTRY_NUM(unit)      UNITINFO(unit)->pChipCap->max_vlan_meter_entry_num
#define CAL_L2_FDB_NUM_MAX(unit)              (UNITINFO(unit)->pChipCap->max_l2_fdb_num)

/*
 * Function Declaration
 */
uint32_t cal_ytportmask_isvalid(yt_unit_t unit, yt_port_mask_t port_mask, yt_bool_t incCpuPort);
uint32_t cal_ytportmask_update(yt_unit_t unit, yt_port_t port, yt_bool_t flag);
yt_bool_t cal_is_combo_port(yt_unit_t unit, yt_port_t port);
uint8_t cal_phy_addr_get(yt_unit_t unit, yt_port_t port);

#endif
