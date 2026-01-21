/*
* Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#ifndef __YT92XX_H__
#define __YT92XX_H__

#define YT92XX_DFLT_UNITID              0

#define YT92XX_VLAN_NUM	                4096
#define YT92XX_MAX_VID		            4095
#define YT92XX_MIN_VID		            0

/* porting according to platform */
#define YT92XX_SWITCH_ID                    0x0
#define YT92XX_SWITCH_PHY_ADDR              0x1d

void yt92xx_write_wrapper(u32 regAddr, u32 regValue);
void yt92xx_read_wrapper(u32 regAddr, u32 *pRegValue);
int yt92xx_phy_read_wrapper(int addr, u8 reg, u16 *value);
int yt92xx_phy_write_wrapper(int addr, u8 reg, u16 value);

#endif
