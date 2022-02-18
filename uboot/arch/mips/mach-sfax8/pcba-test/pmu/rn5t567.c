/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <mach/sfax8.h>
#include <mach/pcba_test.h>
#include <asm/io.h>
#include <malloc.h>
#include <errno.h>


#define PMU_ADDR		(0x36)	//the slave addr to i2c for pmu rn5t567
#define TEST_REG		(0x00)	//test reg addr for pmu rn5t567
#define PMU_RST_STATUS	(0x13)	// reset button status for pmu rn5t567
#define PMU_RST_MASK	(1 << 0) // reset button status mask


extern int i2c_write(u8 *buf, u8 len, u8 addr, int n);
extern int i2c_read(u8 addr, u8 reg, u8 *buf, u8 len, int n);


/*
*config pmu interface
*@reg: pmu reg addr
*@val: config value
*return 0:success; else:i2c transmit time out
*/
static int pmu_write(u8 reg, u8 val)
{
	u8 buf[] = { reg, val };
	return i2c_write(buf, sizeof(buf), PMU_ADDR, 0);
}

/*
*get pmu config val
*@reg: pmu reg addr
*@val: store readback data
*return 0:success; else; i2c transmit time out
*/
static int pmu_read(u8 reg, u8 *val)
{
	return i2c_read(PMU_ADDR, reg, val, 1, 0);
}

/*
* get pmu reset button status
* @val: store the status& STAUS_MASK
* @return 0:success,get the result form val
*		 else: get status failed.
*/
int get_pmu_rst_status(u8 *val)
{
	int ret;
	u8 tmp = 0;
	ret = pmu_read(PMU_RST_STATUS, &tmp);
	if(ret)
		return ret;
	*val = (tmp & PMU_RST_MASK);
	if(*val)
		ret = pmu_write(PMU_RST_STATUS, 0);
	return 0;

}

int pmu_test(void *data)
{

	u8 tmp;
	int ret;
	ret = pmu_read(TEST_REG, &tmp);
	if(ret)
		goto err;
	printf("pmu test ok!\n");
	return CMD_SUCCESS;
err:
	printf("pmu test failed!\n");
	return CMD_TIMEOUT;
}
