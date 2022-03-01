/*
 * =====================================================================================
 *
 *       Filename:  wifi_xo_test.c
 *
 *    Description:  wifi test xo
 *
 *        Version:  1.0
 *        Created:  10/05/2018 07:26:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  austin, austin.xu@siflower.com.cn
 *        Company:  Siflower Communication Tenology Co.,Ltd
 *
 * =====================================================================================
 */
#include <common.h>
#include <asm/io.h>
#include <mach/pcba_test.h>

#include "phy.h"
#include "reg_macbypass.h"
#include "reg_mdm_cfg.h"
#include "reg_rc.h"
#include "rf_access.h"
#include "wifi_test.h"

static int xo_test_started = 0;

int wifi_xo_test(void *data)
{
	struct wifi_xo_param *xo_param = (struct wifi_xo_param *)data;
	int value;

	/* TEST_START and TEST_STOP is for 5G tone XO test, and now we use TX
	 * test and TEST_SET_XO_VALUE to do XO test. So we do not use TEST_START
	 * and TEST_STOP now. */
	if (xo_param->status == TEST_START) {
		ml_apb_write(XO_AUX_RSBUS_CTRL1, xo_param->xo_value);
		if (!xo_test_started) {
			ml_apb_write(HB_CMD_IF_BASE, 0x0031);
			ml_apb_write(HB_CMD_IF_BASE + 1, 0x0600 | 31);
			mdelay(1);
			ml_apb_write(HB_CMD_IF_BASE, 0x80e1);
			ml_apb_write(HB_CMD_IF_BASE + 0x1, 0x0000);
			udelay(100);
			value = ml_apb_read(HB_CMD_IF_BASE + 0xB);
			if ((value & 0x0F00) != 0x0600) {
				printf("start xo test err, value = %04x, except = 0x06xx \n",
						value);
				return CMD_FAIL;
			}
			ml_apb_write(0x380, 0x000D);
			// Enable lb_tx_test_tone
			ml_apb_write(0x0280, 0x0100);
			// Set TTgen Frequency
			ml_apb_write(0x0285, 0);
			// Set TTgen Amplitude
			ml_apb_write(0x0289, 0x24EA);
			xo_test_started = 1;
		}
	} else if (xo_param->status == TEST_STOP) {
		ml_apb_write(0x0280, 0x0);
		//switch mode to rx
		ml_apb_write(HB_CMD_IF_BASE, 0x0031);
		ml_apb_write(HB_CMD_IF_BASE + 0x1, 0x053f);
		mdelay(1);
		ml_apb_write(HB_CMD_IF_BASE, 0x80e1);
		ml_apb_write(HB_CMD_IF_BASE + 0x1, 0x0000);
		udelay(100);
		value = ml_apb_read(HB_CMD_IF_BASE + 0xB);
		if ((value & 0x0F00) != 0x0500) {
			printf("stop xo test err, value = %04x, except = 0x05xx \n", value);
			return CMD_FAIL;
		}
		xo_test_started = 0;
	} else if (xo_param->status == TEST_SET_XO_VALUE) {
		ml_apb_write(XO_AUX_RSBUS_CTRL1, xo_param->xo_value);
		printf("wifi set xo value 0x%x\n", xo_param->xo_value);
	} else if (xo_param->status == TEST_TEMP) {
		ml_apb_send_0_params_cmd(1, HB_CMD_IF_BASE, 0x82b0);
		value = ml_apb_read(HB_CMD_IF_BASE + 0xB) & 0xFF;
		printf("wifi temperature test value %d\n", value);
		return value;
	}
	return CMD_SUCCESS;
}
