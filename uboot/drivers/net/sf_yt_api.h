#ifndef _SF_YT_API_H_
#define _SF_YT_API_H_

#define REG_DEBUG_ADDR_OFFSET			0x1e
#define REG_DEBUG_DATA					0x1f
#define YT8521_EXTREG_SLEEP_CONTROL1    0x27
#define YT8521_EN_SLEEP_SW_BIT			15


int ytphy_read_ext(struct phy_device *phydev, u32 regnum);
int ytphy_write_ext(struct phy_device *phydev, u32 regnum, u16 val);
int yt8521_config_init(struct phy_device *phydev);
#endif
