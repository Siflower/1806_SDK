/*
* =====================================================================================
*
*       Filename:  kernel/sf_gmac/src/yt8521.c
*
*    Description:
*
*        Version:  1.0
*        Created:  09/15/2021 10:04:28 AM
*       Compiler:  gcc
*
*        Company:  Siflower Communication Tenology Co.,Ltd
*
* =====================================================================================
*/

#include "sf_gmac.h"
#include "yt8521.h"

int ytphy_read_ext(struct mii_bus *bus, struct phy_device *phydev, u32 regnum)
{
	int ret;
	int val;

	ret = bus->write(bus, phydev->mdio.addr, REG_DEBUG_ADDR_OFFSET, regnum);
	if (ret < 0)
		return ret;

	val = bus->read(bus, phydev->mdio.addr, REG_DEBUG_DATA);

	return val;
}

int ytphy_write_ext(struct mii_bus *bus, struct phy_device *phydev, u32 regnum, u16 val)
{
	int ret;

	ret = bus->write(bus, phydev->mdio.addr, REG_DEBUG_ADDR_OFFSET, regnum);
	if (ret < 0)
		return ret;

	ret = bus->write(bus, phydev->mdio.addr, REG_DEBUG_DATA, val);

	return ret;
}

int yt8521_config_init(struct mii_bus *bus, struct phy_device *phydev)
{
	int ret;
	int val;

	//phydev->irq = PHY_POLL;

	ytphy_write_ext(bus, phydev, 0xa000, 0);

	/* disable auto sleep */
	val = ytphy_read_ext(bus, phydev, YT8521_EXTREG_SLEEP_CONTROL1);
	if (val < 0)
		return val;

	val &= (~BIT(YT8521_EN_SLEEP_SW_BIT));

	ret = ytphy_write_ext(bus, phydev, YT8521_EXTREG_SLEEP_CONTROL1, val);
	if (ret < 0)
		return ret;

	/* enable RXC clock when no wire plug */
	ret = ytphy_write_ext(bus, phydev, 0xa000, 0);
	if (ret < 0)
		return ret;

	val = ytphy_read_ext(bus, phydev, 0xc);
	if (val < 0)
		return val;

	// To enable gate phy rx_clk_gmii when phy is link down
	val &= ~(1 << 12);
	// select tx delay here, 50 ps clock is appropriately
	val &= 0xff0f;
	val |= 0x20;
	ret = ytphy_write_ext(bus, phydev, 0xc, val);
	if (ret < 0)
		return ret;

	// disable SPEED AUTO DOWNGRADE
	val = ytphy_read_ext(bus, phydev, YT8521_SPEED_AUTO_DOWNGRADE_CTRL);
	if (val < 0)
		return val;

	val &= ~(1 << 5);
	ret = ytphy_write_ext(bus, phydev, YT8521_SPEED_AUTO_DOWNGRADE_CTRL, val);
	if (ret < 0)
		return ret;

	// trigger software reset to take effect
	val = ytphy_read_ext(bus, phydev, 0);
	if (val < 0)
		return val;

	val |= (1 << 15);
	ret = ytphy_write_ext(bus, phydev, 0, val);
	if (ret < 0)
		return ret;

	printk ("end %s\n", __func__);
	return ret;
}
