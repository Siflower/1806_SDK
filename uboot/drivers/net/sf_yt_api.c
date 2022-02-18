#include "sfa18_gmac.h"
#include "sf_yt_api.h"

int ytphy_read_ext(struct phy_device *phydev, u32 regnum)
{
	int ret;
	int val;

	ret = phy_write(phydev, 0, REG_DEBUG_ADDR_OFFSET, regnum);
	if (ret < 0)
		return ret;

	val = phy_read(phydev, 0, REG_DEBUG_DATA);

	return val;
}

int ytphy_write_ext(struct phy_device *phydev, u32 regnum, u16 val)
{
	int ret;

	ret = phy_write(phydev, 0, REG_DEBUG_ADDR_OFFSET, regnum);
	if (ret < 0)
		return ret;

	ret = phy_write(phydev, 0, REG_DEBUG_DATA, val);

	return ret;
}

int yt8521_config_init(struct phy_device *phydev)
{
	int ret;
	int val;

	//phydev->irq = PHY_POLL;

	ytphy_write_ext(phydev, 0xa000, 0);

	/* disable auto sleep */
	val = ytphy_read_ext(phydev, YT8521_EXTREG_SLEEP_CONTROL1);
	if (val < 0)
		return val;

	val &= (~BIT(YT8521_EN_SLEEP_SW_BIT));

	ret = ytphy_write_ext(phydev, YT8521_EXTREG_SLEEP_CONTROL1, val);
	if (ret < 0)
		return ret;

	/* enable RXC clock when no wire plug */
	ret = ytphy_write_ext(phydev, 0xa000, 0);
	if (ret < 0)
		return ret;

	val = ytphy_read_ext(phydev, 0xc);
	if (val < 0)
		return val;
	val &= ~(1 << 12);

	// select tx delay here, 50 ps clock is appropriately
	val &= 0xff0f;
	val |= 0x20;
	ret = ytphy_write_ext(phydev, 0xc, val);
	if (ret < 0)
		return ret;
	printf ("end %s\n", __func__);
	return ret;
}
