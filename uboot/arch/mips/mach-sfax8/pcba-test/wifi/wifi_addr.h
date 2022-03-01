#ifndef __WIFI_ADDR_H__
#define __WIFI_ADDR_H__

#define SIFLOWER_WIFI1_BASE               0xB1000000//wifi 2.4G
#define SIFLOWER_WIFI2_BASE               0xB1400000//wifi 5.0G

enum wifi_bands {
	WIFI_24G = 0,
	WIFI_5G
};

extern int wifi_band;
#define SIFLOWER_CATRF_BASE               0xB1C00000
#define WIFI_BASE_ADDR (wifi_band == WIFI_24G ? SIFLOWER_WIFI1_BASE : SIFLOWER_WIFI2_BASE)

#endif
