/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#ifndef __CAL_BOARD_PROFILE_H__
#define __CAL_BOARD_PROFILE_H__

/*
 * Include Files
 */
#include "yt_types.h"
#include "yt_util.h"
#include "chipdef.h"
#include "phy_chipdef.h"
#include "yt_board.h"

/*
 * Symbol Definition
 */

/* profile identifier */
#define BOARD_PROFILE_MAX_NUM		    (32)	/* how many profiles the code would use */
#define BOARD_PROFILE_NAME_MAXLEN		32	/* profile name max length */
#define BOARD_NONE						(0xFF)
#define INVALID_ID						(0xFF)
#define INVALID_PHYID_ALL				(0xFFFF)
#define CFG_PHYADDR(intAddr, extAddr)   (((extAddr) << 8) | (intAddr))
#define INVALID_BOARD_ID                (0xFFFFFFFF)

/*
 * Data Type Definition
 */

typedef struct board_profile_identify_s
{
    uint32_t	id;
    char		name[BOARD_PROFILE_NAME_MAXLEN];
}board_profile_identify_t;

/* Switch chip description structure */
typedef struct yt_swDescp_s
{
    yt_switch_chip_id_t chip_id;
    yt_switch_chip_model_t chip_model;	/* chip model */
    yt_switch_access_t sw_access;   /* the switch registers accessor,only for chip mode */
    uint8_t port_num;
    yt_portDescp_t *pPortDescp[YT_MAX_PORT_PER_UNIT];  /* port */
    const yt_swchip_cap_t *pChipCap;  /*capability of every unit*/
    const yt_swchip_if_info_t *pChipIfInfo; /* interface info */
}yt_swDescp_t;

/* board profile structure */
typedef struct yt_hwProfile_info_s
{
    const board_profile_identify_t	*pIdentifier;	/* identifier of this board profile */
    uint8_t				switch_count; /* number of switch chip on board */
    yt_swDescp_t	*pSwDescp[YT_MAX_UNIT];    /* switch description */
    yt_port_mask_t		portmask[YT_MAX_UNIT];  /*yt port mask of every unit*/
    yt_port_mask_t		macmask[YT_MAX_UNIT];  /*mac id mask of every unit*/
    yt_port_mask_t		allportmask[YT_MAX_UNIT];  /*yt port mask of every unit, include CPUPORT*/
}yt_hwProfile_info_t;

/* profile identify and init data */
typedef struct yt_hwProfile_s
{
    const board_profile_identify_t		*pIdentifier;	/* identifier of this board profile */
    uint32_t (*profile_init)(yt_hwProfile_info_t *);	/* user defined api to init profile data according to user defined */
}yt_hwProfile_t;

typedef struct yt_cfgBoardInfo_s
{
    uint8_t valid;
    board_profile_identify_t boardIdentify;
    uint8_t	switchCnt; /* number of switch chip on board */ 
    yt_swDescp_t swDesc[YT_MAX_UNIT];
    yt_portDescp_t portDesc[YT_MAX_UNIT][YT_MAX_PORT_PER_UNIT];
}yt_cfgBoardInfo_t;

extern yt_hwProfile_info_t	gBoardInfo;
extern const yt_swDescp_t	*gpSwitchUnit[YT_MAX_UNIT];

/*
 * Function Declaration
 */
uint32_t cal_board_profile_init(void);
yt_ret_t cal_board_ext_phy_addr_set(yt_unit_t unit, yt_port_t port, uint8_t phyAddr);
yt_ret_t cal_board_ext_phy_addr_get(yt_unit_t unit, yt_port_t port, uint8_t *pPhyAddr);
yt_ret_t cal_board_port_attribute_set(yt_unit_t unit, yt_port_t port, yt_port_attri_t attribute);
yt_ret_t cal_board_port_num_get(yt_unit_t unit, uint32_t *pMaxPorts);
yt_ret_t cal_board_profile_register(const yt_hwProfile_t *profile);
void cal_board_profile_clear(void);
yt_ret_t cal_board_profile_set(yt_board_desc_cfg_t *pBoardDesc);
yt_ret_t cal_board_profile_unregister(uint32_t boardId);

#endif /*end of __CAL_BOARD_PROFILE_H__*/
