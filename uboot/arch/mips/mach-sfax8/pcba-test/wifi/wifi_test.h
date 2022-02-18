#ifndef __WIFI_TEST_H__
#define __WIFI_TEST_H__

enum wifi_test_type {
	WIFI_CHECK = 0,
	WIFI_TX,
	WIFI_RX,
	WIFI_XO,
	WIFI_INIT,
	WIFI_CLK,
};

enum test_status {
	TEST_START = 0,
	TEST_STOP,
	TEST_SET_XO_VALUE,
	TEST_TEMP,
};

struct wifi_test_param {
	enum wifi_test_type test_type;
	int band;
};

struct wifi_ate_tx_param {
	int status;
	int freq;
	int center_freq;
	int bw;
	int phy_bw;
	int mode;
	int rate;
	int txpower;
	int tx_delay;
};

struct wifi_ate_rx_param {
	int freq;
	int center_freq;
	int bw;
	int phy_bw;
	int mode;
	int rate;
	int rx_time_ms;
	int packet_cnt;
};

struct wifi_xo_param {
	int status;
	int xo_value;
};
#endif
