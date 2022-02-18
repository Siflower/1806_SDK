/*
 * =====================================================================================
 *
 *       Filename:  wifi_test.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/06/2018 10:19:59 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  austin, austin.xu@siflower.com.cn
 *        Company:  Siflower
 *
 * =====================================================================================
 */
#include <common.h>
#include <asm/io.h>
#include <mach/pcba_test.h>

#include "rwnx_config.h"
#include "rf_access.h"
#include "wifi_ldfw.h"
#include "phy.h"
#include "wifi_test.h"
#include "wifi_ate_test.h"
#include "wifi_xo_test.h"

int wifi_band = WIFI_24G;
static int init_done = 0;

/*
 * Without downloading firmware, and without downloading default values, the
 * script below can be run.
 * This scripts depends on the bootclock being available, if this is a problem
 * we can try to make a version that does not need a bootclock.
 * (In fact you are testing both XO and bootclock now)
 *
 * Depending on the frequency of bootclock, the value written to 0x1060 is 6 (6
 * MHz) or 12 (MHz).
 *
 * When a value of 40+/-1, so a value in range [39 .. 41] is read from 0x1063,
 * then XO is OK.
 **/
static int wifi_clk_test(void)
{
	int value = 0;
	ml_apb_write(0x1001, 0x1fe7);
	ml_apb_write(0x1073, 0x0003);
	udelay(1000);
	ml_apb_write(0x1080, 0x0000);

	ml_apb_write(0x1300, 0x0440);
	udelay(2000);
	ml_apb_write(0x1300, 0x0560);
	ml_apb_write(0x1310, 0x0010);

	ml_apb_write(0x1060, 0x000c);
	ml_apb_write(0x1314, 0x0020);

	ml_apb_write(0x1062, 0x003b);
	udelay(1000);
	value = ml_apb_read(0x1063);
	if (value >= 39 && value <= 41)
		return CMD_SUCCESS;
	else {
		printf("bootclock value is %d\n", value);
		return CMD_FAIL;
	}
}

static int wifi_init(void)
{
	struct phy_cfg_tag phy_conf;
	int ret = 0;

	if (init_done)
		return CMD_SUCCESS;

	// disable ldpc. other params are not used now
	phy_conf.parameters[0] = 0;
	// BW:
	// 0~20M 1~40M 2~80M
	phy_conf.parameters[1] = 0;
	phy_conf.parameters[2] = 1;

	load_rf_pmem_bin();
	ret = rf_app_prepare();
	if (ret < 0)
		return CMD_FAIL;

	wifi_band = WIFI_24G;
	init_aetnensis_for_r20170127(1, 1);
	if (wifi_phy_init(&phy_conf)) {
		printf("trx init error\n");
		return CMD_FAIL;
	}

	wifi_band = WIFI_5G;
	init_aetnensis_for_r20170127(0, 1);
	if (wifi_phy_init(&phy_conf)) {
		printf("trx init error\n");
		return CMD_FAIL;
	}
	init_done = 1;
	return CMD_SUCCESS;
}

int wifi_test(void *data)
{
	int ret = CMD_FAIL;
	struct wifi_test_param *wifi_test_param =
			(struct wifi_test_param *)data;

	wifi_band = wifi_test_param->band;

	switch (wifi_test_param->test_type) {
	case WIFI_TX:
		ret = wifi_tx_test(data + sizeof(*wifi_test_param));
		break;
	case WIFI_RX:
		ret = wifi_rx_test(data + sizeof(*wifi_test_param));
		break;
	case WIFI_XO:
		ret = wifi_xo_test(data + sizeof(*wifi_test_param));
		break;
	case WIFI_INIT:
		ret = wifi_init();
		break;
	case WIFI_CLK:
		ret = wifi_clk_test();
		break;
	default:
		ret = CMD_FAIL;
		break;
	}
	return ret;
}
