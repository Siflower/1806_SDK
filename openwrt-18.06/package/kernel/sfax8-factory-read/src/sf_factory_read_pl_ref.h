/*
 * =====================================================================================
 *
 *       Filename:  sf_factory_read_pl_ref.h
 *
 *    Description:  all statement reference with the platform are defined here
 *        Version:  1.0
 *        Created:  12/30/2016 02:34:36 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  franklin (), franklin.wang@siflower.com.cn
 *        Company:  Siflower Communication Tenology Co.,Ltd
 *
 * =====================================================================================
 */

#ifndef _SF_FFACTORY_READ_PL_REF_H_
#define _SF_FFACTORY_READ_PL_REF_H_

#include <linux/device.h>
#include <sfax8_factory_read.h>

#define SUPPORT_WIFI_VIF_CNT 4

#ifdef CONFIG_VENDOR_HSG
#define FIXED_MACADDRESS_ALLOC
#define MACADDR_LAN_OFFSET 1
#define MACADDR_WAN_OFFSET 0
#define MACADDR_WIFI_LB_OFFSET 2
#define MACADDR_WIFI_HB_OFFSET 6
#else
#define FIXED_MACADDRESS_ALLOC
#define MACADDR_LAN_OFFSET 0
#define MACADDR_WAN_OFFSET 1
#define MACADDR_WIFI_LB_OFFSET 2
#define MACADDR_WIFI_HB_OFFSET 6
#endif

/*
 * SF factory_read PRIVATE PLATFORM DRIVER DATA
 * */
struct sfax8_factory_read_context {
	//main lan eth macaddr
	unsigned char macaddr[MACADDR_SIZE];
	unsigned char sn[SN_SIZE];
	unsigned char sn_flag;
	unsigned char pcba_boot[PCBA_BOOT_SIZE];
	unsigned char hw_ver_flag[HARDWARE_VER_FLAG_SIZE];
	unsigned char hw_ver[HARDWARE_VER_SIZE];
	unsigned char model_ver_flag[MODEL_VER_FLAG_SIZE];
	unsigned char model_ver[MODEL_VER_SIZE];
	unsigned char countryID[COUNTRYID_SIZE];
	uint32_t hw_feature;
	unsigned char vender_flag[VENDER_FLAG_SIZE];
	unsigned char vender[VENDER_SIZE];
	unsigned char product_key_flag[PRODUCT_KEY_FLAG_SIZE];
	unsigned char product_key[PRODUCT_KEY_SIZE];
	unsigned char login_info_flag[LOGIN_INFO_FLAG_SIZE];
	uint32_t login_info;
	unsigned char rom_type_flag[ROM_TYPE_FLAG_SIZE];
	uint32_t rom_type;
	unsigned char wifi_version[WIFI_VERSION_SIZE];
	unsigned char wifi_info[WIFI_INFO_SIZE_V4];
	unsigned int exist_flag;
	bool cali_exist;
	unsigned char wifi_lb_macaddr[MACADDR_SIZE];
	unsigned char wifi_hb_macaddr[MACADDR_SIZE];
	unsigned char lan_macaddr[MACADDR_SIZE];
	unsigned char wan_macaddr[MACADDR_SIZE];
	unsigned char macaddr0[MACADDR_SIZE];
#ifdef CONFIG_SFAX8_HNAT_MULTI_WAN
	unsigned char lan2_macaddr[MACADDR_SIZE];
	unsigned char wwan_macaddr[MACADDR_SIZE];
#endif
	unsigned char cooling_temp[COOLING_TEMP_SIZE];
	unsigned char gmac_delay[GMAC_DELAY_SIZE];
	unsigned char default_ssid_lb[DEFAULT_SSID_LB_SIZE];
	unsigned char default_ssid_hb[DEFAULT_SSID_HB_SIZE];
	unsigned char default_key_lb[DEFAULT_KEY_LB_SIZE];
	unsigned char default_key_hb[DEFAULT_KEY_HB_SIZE];

	//the debug fs root node
	struct dentry *debugfs;

	//the fs root node
	uint32_t start_offset;
	uint32_t len;

	struct device_node *np;
};
#endif
