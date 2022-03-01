/*
 * =====================================================================================
 *
 *       Filename:  wifi_ate_test.c
 *
 *    Description:  wifi use macbypass to tx/rx test
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

enum sf_bw {
	CHAN_WIDTH_20_NOHT = 0,
	CHAN_WIDTH_20,
	CHAN_WIDTH_40,
	CHAN_WIDTH_80
};

static int bw_to_macbyp_bw(int bw)
{
	int macbyp_bw;
	switch (bw) {
	case CHAN_WIDTH_20_NOHT:
	case CHAN_WIDTH_20:
		macbyp_bw = 0;
		break;
	case CHAN_WIDTH_40:
		macbyp_bw = 0x40;
		break;
	case CHAN_WIDTH_80:
		macbyp_bw = 0x80;
		break;
	default:
		macbyp_bw = 0;
		break;
	}
	return macbyp_bw;
}

int wifi_tx_test(void *data)
{
	struct wifi_ate_tx_param *tx_param = (struct wifi_ate_tx_param *)data;

	if (tx_param->status == TEST_STOP) {
		printf("wifi tx test stop, band %d\n", wifi_band);
		int value = 0;
		macbyp_ctrl_set(0x1);
		mdm_swreset_set(0x1111);
		mdm_swreset_set(0x0);

		if (wifi_band == WIFI_5G) {
			// switch mode to rx
			ml_apb_write(HB_CMD_IF_BASE, 0x0031);
			ml_apb_write(HB_CMD_IF_BASE + 0x1, 0x053f);
			mdelay(1);
			ml_apb_write(HB_CMD_IF_BASE, 0x80e1);
			ml_apb_write(HB_CMD_IF_BASE + 0x1, 0x0000);
			udelay(100);
			value = ml_apb_read(HB_CMD_IF_BASE + 0xB);
			if ((value & 0x0F00) != 0x0500) {
				printf("stop tx test err, value = %04x,except "
				       "= 0x05xx \n",
						value);
				return CMD_FAIL;
			}
		} else {
			// switch mode to rx
			ml_apb_write(LB_CMD_IF_BASE, 0x0031);
			ml_apb_write(LB_CMD_IF_BASE + 0x1, 0x153f);
			mdelay(1);
			ml_apb_write(LB_CMD_IF_BASE, 0x80e1);
			ml_apb_write(LB_CMD_IF_BASE + 0x1, 0x1000);
			udelay(100);
			value = ml_apb_read(LB_CMD_IF_BASE + 0xB);
			if ((value & 0x0F00) != 0x0500) {
				printf("stop tx test err, value = %04x,except "
				       "= 0x05xx \n",
						value);
				return CMD_FAIL;
			}
		}
		return CMD_SUCCESS;
	}

	phy_hw_set_channel(wifi_band, tx_param->freq, tx_param->center_freq,
			tx_param->phy_bw, 0);
	printf("%s, wifi_band %d, freq %d, center_freq %d, bw %d, mode %d, rate %d, "
	       "txpower_idx %d\n",
			__func__, wifi_band, tx_param->freq, tx_param->center_freq,
			tx_param->bw, tx_param->mode, tx_param->rate,
			tx_param->txpower);

	/* set tx param */
	macbyp_ctrl_set(0);
	mdm_swreset_set(0x1111);
	mdm_swreset_set(0x0);
	macbyp_pre_tx_delay_set(tx_param->tx_delay);
	macbyp_txv0_set(tx_param->txpower);
	// 20M:0x00;40M:0x40;80M:0x80
	macbyp_txv1_set((1 << 3) | bw_to_macbyp_bw(tx_param->bw));
	macbyp_txv2_set(0x1);
	macbyp_txv3_set(0x0);
	macbyp_txv4_set(tx_param->rate);
	macbyp_txv5_set(tx_param->mode);
	/* len = 0x400 v6[0:7] and v7[0:3] */
	macbyp_txv6_set(0x00);
	if (tx_param->mode != 0)
		// for HT VHT
		macbyp_txv7_set(0xb4);
	else {
		if (wifi_band == WIFI_24G) {
			if (tx_param->rate <= 3) {
				macbyp_txv7_set(0x4 | (tx_param->rate << 4));
			} else {
				switch (tx_param->rate) {
				case 4:
					macbyp_txv7_set(0xb4);
					break;
				case 5:
					macbyp_txv7_set(0xf4);
					break;
				case 6:
					macbyp_txv7_set(0xa4);
					break;
				case 7:
					macbyp_txv7_set(0xe4);
					break;
				case 8:
					macbyp_txv7_set(0x94);
					break;
				case 9:
					macbyp_txv7_set(0xd4);
					break;
				case 10:
					macbyp_txv7_set(0x84);
					break;
				case 11:
					macbyp_txv7_set(0xc4);
					break;
				default:
					macbyp_txv7_set(0xb4);
					break;
				}
			}
		} else {
			switch (tx_param->rate) {
			case 1:
				macbyp_txv7_set(0xb4);
				break;
			case 2:
				macbyp_txv7_set(0xf4);
				break;
			case 3:
				macbyp_txv7_set(0xa4);
				break;
			case 4:
				macbyp_txv7_set(0xe4);
				break;
			case 5:
				macbyp_txv7_set(0x94);
				break;
			case 6:
				macbyp_txv7_set(0xd4);
				break;
			case 7:
				macbyp_txv7_set(0x84);
				break;
			case 8:
				macbyp_txv7_set(0xc4);
				break;
			default:
				macbyp_txv7_set(0xb4);
				break;
			}
		}
	}
	macbyp_txv8_set(0x0);
	macbyp_txv9_set(0x0);
	macbyp_txv10_set(0x0); // HT length
	macbyp_txv11_set(0x4); // HT Length
	macbyp_txv12_set(0x0);
	macbyp_txv13_set(0x0);
	macbyp_txv14_set(0x0);
	macbyp_txv15_set(0x0);

	macbyp_ctrl_set(0x300);
	udelay(100);
	macbyp_ctrl_set(0x301);

	return CMD_SUCCESS;
}

int wifi_rx_test(void *data)
{
	struct wifi_ate_rx_param *rx_param = (struct wifi_ate_rx_param *)data;
	uint32_t frame_all = 0;
	uint32_t frame_bad = 0;
	uint32_t frame_err = 0;
	int ret = CMD_FAIL;
	int timeout = rx_param->rx_time_ms;
	int rx_pkt_cnt = rx_param->packet_cnt;

	printf("%s, band %d, freq %d, center_freq %d, bw %d, phy_bw %d, mode %d, rate %d\n",
			__func__, wifi_band, rx_param->freq, rx_param->center_freq,
			rx_param->bw, rx_param->phy_bw, rx_param->mode, rx_param->rate);
	phy_hw_set_channel(wifi_band, rx_param->freq, rx_param->center_freq,
			rx_param->phy_bw, 0);

	macbyp_trigger_set(0x00000012);
	macbyp_ctrl_set(0x11);
	macbyp_ctrl_set(0x101);
	do {
		mdelay(20);
		frame_all = macbyp_stat_rxend_get();
		timeout -= 20;
	} while(timeout > 0 && frame_all < rx_pkt_cnt);
	frame_bad = macbyp_stat_frame_bad_get();
	frame_err = macbyp_stat_rxerror_get();

	macbyp_ctrl_set(0);
	mdm_swreset_set(0x1111);
	mdm_swreset_set(0x0);
	/* Some low rate send very slow, and frame_all can't reach rx_pkt_cnt,
	   so here we just ask frame_all > 100. */
	if (frame_all < 100)
		ret = CMD_TIMEOUT;
	/* If bad and err frames more than 10%, then test fail. */
	else if ((frame_bad + frame_err) * 10 < frame_all)
		ret = CMD_SUCCESS;
	else
		ret = CMD_FAIL;
	printf("%s wifi_band %d, frame_bad %d, frame_err %d, frame_all %d, ret %d\n",
			__func__, wifi_band, frame_bad, frame_err, frame_all, ret);
	return ret;
}
