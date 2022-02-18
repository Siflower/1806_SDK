#include "sfa18_gmac.h"
#include "sf_nf_api.h"

#define SF_MDC_GPIO			28
#define SF_MDIO_GPIO		29

#define MDIO_DELAY 1
#define MDIO_READ_DELAY 1
#define MDIO_READ 2
#define MDIO_WRITE 1

#define MII_ADDR_C45 (1<<30)
#define MDIO_C45 (1<<15)
#define MDIO_C45_ADDR (MDIO_C45 | 0)
#define MDIO_C45_READ (MDIO_C45 | 3)
#define MDIO_C45_WRITE (MDIO_C45 | 1)

#define MDIO_HIGH            gpio_set_value(SF_MDIO_GPIO, 1)
#define MDIO_LOW             gpio_set_value(SF_MDIO_GPIO, 0)
#define MDC_HIGH             gpio_set_value(SF_MDC_GPIO, 1)
#define MDC_LOW              gpio_set_value(SF_MDC_GPIO, 0)
#define READ_MDIO            gpio_get_value(SF_MDIO_GPIO)



void sf_nf_mdio_init(void)
{
	gpio_request(SF_MDC_GPIO, "mdcGpio");
	gpio_request(SF_MDIO_GPIO, "mdioGpio");

	gpio_direction_output(SF_MDC_GPIO, 1);
	MDC_HIGH;
	gpio_direction_output(SF_MDIO_GPIO, 1);
	MDIO_HIGH;
}

static void mdiobb_send_bit(uint8_t val)
{
	if((val & 0x1) == 0)
		MDIO_LOW;
	else
		MDIO_HIGH;

	udelay(MDIO_DELAY);
	MDC_HIGH;
	udelay(MDIO_READ_DELAY);
	MDC_LOW;
}

//get data on the rising edge of the clock
static uint8_t mdiobb_get_bit(void)
{
	udelay(MDIO_DELAY);
	MDC_HIGH;
	udelay(MDIO_READ_DELAY);
	MDC_LOW;

	return gpio_get_value(SF_MDIO_GPIO);
}

static void mdiobb_send_num(uint16_t val, uint8_t bits)
{
	int i;

	for (i = bits - 1; i >= 0; i--)
		mdiobb_send_bit((val >> i) & 1);
}

static uint16_t mdiobb_get_num(uint32_t bits)
{
	int i;
	uint16_t ret = 0;

	for (i = bits - 1; i >= 0; i--)
	{
		ret <<= 1;
		ret |= mdiobb_get_bit();
	}

	return ret;
}

static void mdiobb_cmd(uint32_t op, uint32_t phy, uint32_t reg)
{
	int i;

	gpio_direction_output(SF_MDIO_GPIO, 1);

	/* set 32bits pre */
	for (i = 0; i < 32; i++)
		mdiobb_send_bit(1);

	/* start: 22 clause 01, 45 clause 00*/
	mdiobb_send_bit(0);
	if (op & MDIO_C45)
		mdiobb_send_bit(0);
	else
		mdiobb_send_bit(1);

	/* send r/w bit */
	mdiobb_send_bit((op >> 1) & 1);
	mdiobb_send_bit((op >> 0) & 1);

	/* send phy_id and reg_addr*/
	mdiobb_send_num(phy, 5);
	mdiobb_send_num(reg, 5);
}

//used in clause 45
static int mdiobb_cmd_addr(uint32_t phy, uint32_t addr)
{
	unsigned int dev_addr = (addr >> 16) & 0x1F;
	unsigned int reg = addr & 0xFFFF;
	mdiobb_cmd(MDIO_C45_ADDR, phy, dev_addr);

	mdiobb_send_bit(1);
	mdiobb_send_bit(0);

	mdiobb_send_num(reg, 16);

	//mdiobb_send_bit(1);
	mdiobb_get_bit();

	return dev_addr;
}

int mdiobb_read(uint32_t phy, uint32_t reg, uint16_t *val)
{
	/* send start and r/w phy_id reg_addr*/
	if(reg & MII_ADDR_C45)
	{
		reg = mdiobb_cmd_addr(phy, reg);
		mdiobb_cmd(MDIO_C45_READ, phy, reg);
	}
	else
		mdiobb_cmd(MDIO_READ, phy, reg);

	gpio_direction_input(SF_MDIO_GPIO);
	/* get ta bit */
	if(mdiobb_get_bit() != 0)
	{
		*val = 0xffff;
		printf("read error!\n");
		return 0;
	}

	/* get value*/
	*val = mdiobb_get_num(16);

	mdiobb_get_bit();

	//printf("val:0x%x\n", *val);
	return 0;
}

int mdiobb_write(uint32_t phy, uint32_t reg, uint16_t val)
{
	/* send start and r/w phy/reg addr*/
	if(reg & MII_ADDR_C45)
	{
		reg = mdiobb_cmd_addr(phy, reg);
		mdiobb_cmd(MDIO_C45_WRITE, phy, reg);

	}
	else
		mdiobb_cmd(MDIO_WRITE, phy, reg);

	/* send ta:10*/
	mdiobb_send_bit(1);
	mdiobb_send_bit(0);

	/* send value*/
	mdiobb_send_num(val, 16);

	gpio_direction_input(SF_MDIO_GPIO);
	mdiobb_get_bit();

	return 0;
}

int l2sw_getAsicReg(uint32_t reg, uint32_t *val)
{
	uint16_t regData;

	mdiobb_write(MDC_MDIO_PHY_ID_0, MDC_MDIO_CTRL0_REG, MDC_MDIO_ADDR_OP);
	mdiobb_write(MDC_MDIO_PHY_ID_0, MDC_MDIO_ADDRESS_REG, reg);
	mdiobb_write(MDC_MDIO_PHY_ID_0, MDC_MDIO_CTRL1_REG, MDC_MDIO_READ_OP);
	mdiobb_read(MDC_MDIO_PHY_ID_0, MDC_MDIO_DATA_READ_REG, &regData);

	*val = regData;
	return 0;
}

int l2sw_setAsicReg(uint32_t reg, uint32_t val)
{
	mdiobb_write(MDC_MDIO_PHY_ID_0, MDC_MDIO_CTRL0_REG, MDC_MDIO_ADDR_OP);
	mdiobb_write(MDC_MDIO_PHY_ID_0, MDC_MDIO_ADDRESS_REG, reg);
	mdiobb_write(MDC_MDIO_PHY_ID_0, MDC_MDIO_DATA_WRITE_REG, val);
	mdiobb_write(MDC_MDIO_PHY_ID_0, MDC_MDIO_CTRL1_REG, MDC_MDIO_WRITE_OP);
	return 0;
}

void nf_switch_init(void)
{
	l2sw_setAsicReg(L2SW_REG_DIGITAL_INTERFACE_SELECT, 0x10);
	l2sw_setAsicReg(L2SW_REG_DIGITAL_INTERFACE_SELECT_1, 0x1);
	l2sw_setAsicReg(L2SW_REG_DIGITAL_INTERFACE2_FORCE, 0x1016);
	l2sw_setAsicReg(L2SW_REG_DIGITAL_INTERFACE1_FORCE, 0x1016);
	l2sw_setAsicReg(L2SW_REG_EXT1_RGMXF, 0x986);
	l2sw_setAsicReg(L2SW_REG_EXT2_RGMXF, 0x986);
	l2sw_setAsicReg(L2SW_REG_ECO_0_REG, 0x22);
	l2sw_setAsicReg(0x3b0c, 0x7);
	l2sw_setAsicReg(0x3b0d, 0x7);
}
