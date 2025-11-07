/*
 * sf_factory_read_entry.c - Support to read value from factory block.
 * This driver provide an interface between factory block and our net driver(ethernet,wireless),
 * to simplify the process of reading from factory.
 * For example, sf rf driver gets XO config value from factory through MTD.
 *
 * Copyright (C) 2017 Shanghai Siflower Communication Technology Co., Ltd.
 *
 * Author: Zhengjinyang Jiang <star.jiang@siflower.com.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/string.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/mtd/mtd.h>
#include <sfax8_factory_read.h>
#include <linux/etherdevice.h>
#include "sf_factory_read_pl_ref.h"
#ifdef CONFIG_SF16A18_FACTORY_READ_SYSFS_DEBUG
#include "sf_factory_read_sysfs.h"
#endif
struct sfax8_factory_read_context *f_read_ctx = NULL;

static char factory_read_names[31][22] = {
	"mtd-mac-address",
	"mtd-mac-address",
	"mtd-mac-address",
	"mtd-mac-address",
	"mtd-mac-address",
	"mtd-sn-number",
	"mtd-sn-flag",
	"mtd-pcba-boot",
	"mtd-hardware-ver-flag",
	"mtd-hardware-ver",
	"mtd-model-ver-flag",
	"mtd-model-ver",
	"mtd-country-id",
	"mtd-hw-feature",
	"mtd-vender-flag",
	"mtd-vender",
	"mtd-product-key-flag",
	"mtd-product-key",
	"mtd-login-info-flag",
	"mtd-login-info",
	"mtd-rom-type-flag",
	"mtd-rom-type",
	"mtd-wifi-version",
	"mtd-rf-xo-config",
	"mtd-wifi-info",
	"mtd-cooling-temp",
	"mtd-gmac-delay",
	"mtd-default-ssid-lb",
	"mtd-default-ssid-hb",
	"mtd-default-key-lb",
	"mtd-default-key-hb",
};

int get_value_through_mtd(struct device_node *np,
		const char *name, int start_offset, size_t len, unsigned char *buffer)
{
	struct device_node *mtd_np = NULL;
	size_t retlen;
	int size, ret, offset;
	struct mtd_info *mtd;
	const char *part;
	const char part_temp[] = "factory";
	const __be32 *list;
	phandle phandle;
	unsigned char * pmac = NULL;
	uint32_t mac_inc = 0;

	if(name != NULL){
		if(!strcasecmp(name,"mtd-mac-address")){
			pmac = (unsigned char *)buffer;
		}

		if(!np){
			printk("error! device node is null\n");
			return -1;
		}
		list = of_get_property(np, name, &size);
		if (!list || (size != (2 * sizeof(*list))))
			return -2;

		phandle = be32_to_cpup(list++);
		if (phandle)
			mtd_np = of_find_node_by_phandle(phandle);

		if (!mtd_np)
			return -3;

		part = of_get_property(mtd_np, "label", NULL);
		if (!part)
			part = mtd_np->name;
		mtd = get_mtd_device_nm(part);
		offset = be32_to_cpup(list);
	}
	else{
		mtd = get_mtd_device_nm(part_temp);
		offset = start_offset;
	}
	if (IS_ERR(mtd))
		return PTR_ERR(mtd);

	ret = mtd_read(mtd, offset, len, &retlen, (unsigned char *) buffer);
	put_mtd_device(mtd);

	if((name != NULL) && (!strcasecmp(name,"mtd-mac-address"))){
		if (!of_property_read_u32(np, "mtd-mac-address-increment", &mac_inc))
			pmac[5] += mac_inc;
	}
	return ret;
}

static void inc_sf_mac_addr(char *mac, int inc)
{
	unsigned int mac_calc[MACADDR_SIZE] = {0};
	int i = 0;
	for(i = 0 ; i < MACADDR_SIZE ; i++)
	{
		mac_calc[i] = (unsigned int)(mac[i]) & 0xFF;
	}
	mac_calc[MACADDR_SIZE - 1] += inc;
	mac_calc[MACADDR_SIZE - 2] += ((mac_calc[MACADDR_SIZE - 1] & (0x100))  >> 8);
	mac[MACADDR_SIZE - 1] = mac_calc[MACADDR_SIZE - 1] & (0xff);

	mac_calc[MACADDR_SIZE - 3] += ((mac_calc[MACADDR_SIZE - 2] & (0x100))  >> 8);
	mac[MACADDR_SIZE - 2] = mac_calc[MACADDR_SIZE - 2] & (0xff);
	mac[MACADDR_SIZE - 3] = mac_calc[MACADDR_SIZE - 3] & (0xff);
	//the first 3 char is reserved
}

/*
static int is_valid_sf_address(const unsigned char * mac)
{
	//10:16:88 or A8:5A:F3
	return ((mac[0] == 0x10) && (mac[1] == 0x16) && (mac[2] == 0x88)) ||
		((mac[0] == 0xA8) && (mac[1] == 0x5A) && (mac[2] == 0xF3));
}
*/

static int set_sf_address(char * mac)
{
	//10:16:88
	mac[0] = 0x10;
	mac[1] = 0x16;
	mac[2] = 0x88;
	return 0;
}

static int set_mac_address_mtd(struct device_node *np,const char *name,char *mac)
{
	struct device_node *mtd_np = NULL;
	size_t retlen;
	int size, ret, offset;
	struct mtd_info *mtd;
	const char *part;
	const __be32 *list;
	phandle phandle;
	unsigned char *buf = NULL;
	struct erase_info ei;

	if(name != NULL){
		if(!np){
			printk("error! device node is null\n");
			return -1;
		}
		list = of_get_property(np, name, &size);
		if (!list || (size != (2 * sizeof(*list))))
			return -2;

		phandle = be32_to_cpup(list++);
		if (phandle)
			mtd_np = of_find_node_by_phandle(phandle);

		if (!mtd_np)
			return -3;

		part = of_get_property(mtd_np, "label", NULL);
		if (!part)
			part = mtd_np->name;
		mtd = get_mtd_device_nm(part);
		offset = be32_to_cpup(list);
	}else{
		return -1;
	}
	if (IS_ERR(mtd))
		return PTR_ERR(mtd);


	buf = kmalloc(mtd->erasesize, GFP_KERNEL);
	ret = mtd_read(mtd, offset - (offset % mtd->erasesize), mtd->erasesize, &retlen, buf);

	ei.mtd = mtd;
	ei.callback = NULL;
	ei.addr = 0;
	ei.len = mtd->erasesize;
	ei.priv = 0;
	ret = mtd_erase(mtd, &ei);
	memcpy(buf + (offset % mtd->erasesize),mac,MACADDR_SIZE);
	ret = mtd_write(mtd, offset - (offset % mtd->erasesize), mtd->erasesize, &retlen, buf);
	kfree(buf);
	put_mtd_device(mtd);
	return ret;
}

int sfax8_set_int_mtd(int id, uint32_t *ctx_buf, uint32_t val)
{
	const char *name = factory_read_names[id];
	struct device_node *np = NULL, *mtd_np = NULL;
	struct mtd_info *mtd;
	struct erase_info ei;
	phandle phandle;
	size_t retlen;
	int i, size, ret, offset;
	const char *part;
	const __be32 *list;
	unsigned char *buf = NULL;
	unsigned char tmp_buf[4];

	memset(tmp_buf, 0x0, 4);

	if (name != NULL && f_read_ctx != NULL) {
		np = f_read_ctx->np;
		if (!np) {
			printk("error! device node is null\n");
			return -1;
		}
		list = of_get_property(np, name, &size);
		if (!list)
			return -2;

		phandle = be32_to_cpup(list++);
		if (phandle)
			mtd_np = of_find_node_by_phandle(phandle);

		if (!mtd_np)
			return -3;

		part = of_get_property(mtd_np, "label", NULL);
		if (!part)
			part = mtd_np->name;
		mtd = get_mtd_device_nm(part);
		offset = be32_to_cpup(list);
	} else {
		return -1;
	}
	if (IS_ERR(mtd)) {
		return PTR_ERR(mtd);
	}

	buf = kmalloc(mtd->erasesize, GFP_KERNEL);
	ret = mtd_read(mtd, offset - (offset % mtd->erasesize), mtd->erasesize, &retlen, buf);

	ei.mtd = mtd;
	ei.callback = NULL;
	ei.addr = 0;
	ei.len = mtd->erasesize;
	ei.priv = 0;
	ret = mtd_erase(mtd, &ei);

	*ctx_buf = val;
	for (i = 0; i < 4; i++, val >>= 8) {
		tmp_buf[i] = val & 0xff;
	}
	memcpy(buf + (offset % mtd->erasesize), tmp_buf, 4);

	ret = mtd_write(mtd, offset - (offset % mtd->erasesize), mtd->erasesize, &retlen, buf);
	kfree(buf);
	put_mtd_device(mtd);
	printk("set %s: 0x%x\n", name, *ctx_buf);
	return ret;
}
EXPORT_SYMBOL_GPL(sfax8_set_int_mtd);

int sfax8_set_char_mtd(int id, unsigned char *ctx_buf, unsigned char *str, const size_t len)
{
	const char *name = factory_read_names[id];
	struct device_node *np = NULL, *mtd_np = NULL;
	struct mtd_info *mtd;
	struct erase_info ei;
	phandle phandle;
	size_t retlen;
	int i, size, ret, offset;
	const char *part;
	const __be32 *list;
	unsigned char *buf = NULL;
	unsigned char tmp_buf[len];

	memset(tmp_buf, 0x0, len);

	if (name != NULL && f_read_ctx != NULL) {
		np = f_read_ctx->np;
		if (!np) {
			printk("error! device node is null\n");
			return -1;
		}
		list = of_get_property(np, name, &size);
		if (!list)
			return -2;

		phandle = be32_to_cpup(list++);
		if (phandle)
			mtd_np = of_find_node_by_phandle(phandle);

		if (!mtd_np)
			return -3;

		part = of_get_property(mtd_np, "label", NULL);
		if (!part)
			part = mtd_np->name;
		mtd = get_mtd_device_nm(part);
		offset = be32_to_cpup(list);
	} else {
		return -1;
	}
	if (IS_ERR(mtd)) {
		return PTR_ERR(mtd);
	}

	buf = kmalloc(mtd->erasesize, GFP_KERNEL);
	ret = mtd_read(mtd, offset - (offset % mtd->erasesize), mtd->erasesize, &retlen, buf);

	ei.mtd = mtd;
	ei.callback = NULL;
	ei.addr = 0;
	ei.len = mtd->erasesize;
	ei.priv = 0;
	ret = mtd_erase(mtd, &ei);

	for (i = 0; i < len; i++) {
		ctx_buf[i] = str[i];
		tmp_buf[i] = str[i];
	}
	memcpy(buf + (offset % mtd->erasesize), tmp_buf, len);

	ret = mtd_write(mtd, offset - (offset % mtd->erasesize), mtd->erasesize, &retlen, buf);
	kfree(buf);
	put_mtd_device(mtd);
	printk("set %s: %s\n", name, str);
	for (i = 0; i < len; i++) {
		printk("0x%02x\n", str[i]);
	}

	return ret;
}
EXPORT_SYMBOL_GPL(sfax8_set_char_mtd);

int sfax8_set_gmac_delay_mtd(const char *name, unsigned short gmac_delay)
{
	struct device_node *np = NULL, *mtd_np = NULL;
	struct mtd_info *mtd;
	struct erase_info ei;
	phandle phandle;
	size_t retlen;
	int size, ret, offset;
	const char *part;
	const __be32 *list;
	unsigned char *buf = NULL, tmp_buf[GMAC_DELAY_SIZE];

	if(name != NULL && f_read_ctx != NULL){
		np = f_read_ctx->np;
		if(!np){
			printk("error! device node is null\n");
			return -1;
		}
		list = of_get_property(np, name, &size);
		if (!list)
			return -2;

		phandle = be32_to_cpup(list++);
		if (phandle)
			mtd_np = of_find_node_by_phandle(phandle);

		if (!mtd_np)
			return -3;

		part = of_get_property(mtd_np, "label", NULL);
		if (!part)
			part = mtd_np->name;
		mtd = get_mtd_device_nm(part);
		offset = be32_to_cpup(list);
	}else{
		return -1;
	}
	if (IS_ERR(mtd)) {
		return PTR_ERR(mtd);
	}

	// erasesize 65536 here, so mtd_read first
	buf = kmalloc(mtd->erasesize, GFP_KERNEL);
	ret = mtd_read(mtd, offset - (offset % mtd->erasesize), mtd->erasesize, &retlen, buf);

	ei.mtd = mtd;
	ei.callback = NULL;
	ei.addr = 0;
	ei.len = mtd->erasesize;
	ei.priv = 0;
	ret = mtd_erase(mtd, &ei);
	sprintf(tmp_buf, "%04x", gmac_delay);
	memcpy(buf + (offset % mtd->erasesize), tmp_buf, GMAC_DELAY_SIZE);
	// update factory get here
	sprintf(f_read_ctx->gmac_delay, "%04x", gmac_delay);
	ret = mtd_write(mtd, offset - (offset % mtd->erasesize), mtd->erasesize, &retlen, buf);
	kfree(buf);
	put_mtd_device(mtd);
	printk("end %s, set gmac delay:0x%04x\n", __func__, gmac_delay);
	return ret;
}
EXPORT_SYMBOL_GPL(sfax8_set_gmac_delay_mtd);


void handle_macaddr_internal(struct device_node *np,struct sfax8_factory_read_context *priv)
{
	//last char of wifi base address must be 4 aligned in current version
	unsigned int last_char = (unsigned int)priv->macaddr[5];
#ifndef FIXED_MACADDRESS_ALLOC
	unsigned int inc_lb = (SUPPORT_WIFI_VIF_CNT - (last_char % SUPPORT_WIFI_VIF_CNT)) % SUPPORT_WIFI_VIF_CNT;
#endif
	int rc = 0;

	if (!is_valid_ether_addr(priv->macaddr)) {
		//10:16:88
		eth_random_addr(priv->macaddr);
		set_sf_address(priv->macaddr);
		rc = set_mac_address_mtd(np,"mtd-mac-address",priv->macaddr);
		printk("error: generated random MAC address rc=%d!!\n",rc);
	}
	memcpy(priv->macaddr0,priv->macaddr,MACADDR_SIZE);
	memcpy(priv->wifi_lb_macaddr,priv->macaddr,MACADDR_SIZE);
	memcpy(priv->wifi_hb_macaddr,priv->macaddr,MACADDR_SIZE);
	memcpy(priv->lan_macaddr,priv->macaddr,MACADDR_SIZE);
	memcpy(priv->wan_macaddr,priv->macaddr,MACADDR_SIZE);
#ifndef FIXED_MACADDRESS_ALLOC
	//for lb wifi address
	if(inc_lb != 0) inc_sf_mac_addr(priv->wifi_lb_macaddr,inc_lb);
	//for hb wifi address
	inc_sf_mac_addr(priv->wifi_hb_macaddr,inc_lb + SUPPORT_WIFI_VIF_CNT);
	//move eth address to end to avoid conflict with wifi address
	if(inc_lb == 0){
		//pick the last 2 address as lan/wan address
		inc_sf_mac_addr(priv->macaddr,8);
		inc_sf_mac_addr(priv->lan_macaddr,8);
		inc_sf_mac_addr(priv->wan_macaddr,9);
	}else if(inc_lb == 3 || inc_lb == 2){
		//pick the first 2 address as lan/wan address
		inc_sf_mac_addr(priv->wan_macaddr,1);
	}else if(inc_lb == 1){
		//pick the first addres as lan
		//pick the last addres as wan
		inc_sf_mac_addr(priv->wan_macaddr,9);
	}else{
		printk("handle_macaddr_internal should nerver get here %d!!!\n",inc_lb);
	}
#else
	inc_sf_mac_addr(priv->lan_macaddr, MACADDR_LAN_OFFSET);
	inc_sf_mac_addr(priv->wan_macaddr, MACADDR_WAN_OFFSET);
	inc_sf_mac_addr(priv->wifi_lb_macaddr, MACADDR_WIFI_LB_OFFSET);
	inc_sf_mac_addr(priv->wifi_hb_macaddr, MACADDR_WIFI_HB_OFFSET);
#endif
}
EXPORT_SYMBOL_GPL(handle_macaddr_internal);


static void factory_print_string(const char *info, const unsigned char *str, int len)
{
	if (NULL == info || NULL == str || len <= 0)
		return;
	printk(KERN_CONT "%s ", info);
	while (len > 0) {
		printk(KERN_CONT "%#X ", *str);
		str += 1;
		len -= 1;
	}
	printk(KERN_CONT "\n");
}

static int save_value_from_factory_to_host(struct platform_device *pdev,
		struct sfax8_factory_read_context *priv)
{
	int ret = 0;
	unsigned char *buffer;
	struct device_node *np = pdev->dev.of_node;
	// get mac address
	if ((ret = get_value_through_mtd(np, "mtd-mac-address", 0, MACADDR_SIZE,
			     priv->macaddr))) {
		printk("get mac address through mtd failed! ret %d\n",ret);
	} else {
		//handle mac address internal
		handle_macaddr_internal(np,priv);
	}
	priv->exist_flag |= (1 << READ_MAC_ADDRESS);
	factory_print_string("macaddr is:",  priv->macaddr, MACADDR_SIZE);

	// get sn number
	if ((ret = get_value_through_mtd(
			     np, "mtd-sn-number", 0, SN_SIZE, priv->sn)))
		printk("get sn number through mtd failed! ret %d\n", ret);
	priv->exist_flag |= (1 << READ_SN);
	factory_print_string("sn is:",  priv->sn, SN_SIZE);
	// get sn flag
	if ((ret = get_value_through_mtd(np, "mtd-sn-flag", 0, SN_FLAG_SIZE,
			     &priv->sn_flag)))
		printk("get sn flag through mtd failed! ret %d\n", ret);
	priv->exist_flag |= (1 << READ_SN_FLAG);
	printk("sn_flag is 0x%X\n", priv->sn_flag);

	// get pcba boot mark
	if ((ret = get_value_through_mtd(np, "mtd-pcba-boot", 0, PCBA_BOOT_SIZE,
			     priv->pcba_boot)))
		printk("get pcba boot mark through mtd failed! ret %d\n", ret);
	priv->exist_flag |= (1 << READ_PCBA_BOOT);

	// get hardware version flag
	if ((ret = get_value_through_mtd(np, "mtd-hardware-ver-flag", 0,
			     HARDWARE_VER_FLAG_SIZE, priv->hw_ver_flag)))
		printk("get hardware version flag through mtd failed! ret %d\n",
				ret);
	priv->exist_flag |= (1 << READ_HARDWARE_VER_FLAG);
	factory_print_string("hardware version flag is:",  priv->hw_ver_flag, HARDWARE_VER_FLAG_SIZE);
	// get hardware version
	if ((ret = get_value_through_mtd(np, "mtd-hardware-ver", 0,
			     HARDWARE_VER_SIZE, priv->hw_ver)))
		printk("get hardware version through mtd failed! ret %d\n",
				ret);
	priv->exist_flag |= (1 << READ_HARDWARE_VER);
	factory_print_string("hardware version is:", priv->hw_ver, HARDWARE_VER_SIZE);


	// get model version flag
	if ((ret = get_value_through_mtd(np, "mtd-model-ver-flag", 0,
			     MODEL_VER_FLAG_SIZE, priv->model_ver_flag)))
		printk("get model version flag through mtd failed! ret %d\n",
				ret);
	priv->exist_flag |= (1 << READ_MODEL_VER_FLAG);
	factory_print_string("model version flag is:", priv->model_ver_flag , MODEL_VER_FLAG_SIZE);
	// get model version
	if ((ret = get_value_through_mtd(np, "mtd-model-ver", 0, MODEL_VER_SIZE,
			     priv->model_ver)))
		printk("get model version through mtd failed! ret %d\n", ret);
	priv->exist_flag |= (1 << READ_MODEL_VER);
	factory_print_string("model version is:", priv->model_ver, MODEL_VER_SIZE);

	// get counrty id
	if ((ret = get_value_through_mtd(np, "mtd-country-id", 0,
			     COUNTRYID_SIZE, priv->countryID)))
		printk("get country ID through mtd failed! ret %d\n", ret);
	if ((priv->countryID[0] >= 'A') && (priv->countryID[0] <= 'z') &&
			(priv->countryID[1] >= 'A') &&
			(priv->countryID[1] <= 'z')) {
		priv->exist_flag |= (1 << READ_COUNTRY_ID);
	} else {
		factory_print_string("Use default value [CN] because of can not find an vaild country ID:",
				     priv->countryID, COUNTRYID_SIZE);
		priv->countryID[0] = 'C';
		priv->countryID[1] = 'N';
	}
	factory_print_string("countryID is:",  priv->countryID, COUNTRYID_SIZE);

	// get HW feature
	buffer = kmalloc(sizeof(char) * HW_FEATURE_SIZE, GFP_KERNEL);
	if ((ret = get_value_through_mtd(
			     np, "mtd-hw-feature", 0, HW_FEATURE_SIZE, buffer)))
		printk("get HW feature through mtd failed! ret %d\n", ret);
	priv->hw_feature = (buffer[3] << 24) | (buffer[2] << 16) |
			   (buffer[1] << 8) | buffer[0];
	priv->exist_flag |= (1 << READ_HW_FEATURE);
	printk("HW feature is %#x\n", priv->hw_feature);
	kfree(buffer);

	// get vender flag
	if ((ret = get_value_through_mtd(np, "mtd-vender-flag", 0,
			     VENDER_FLAG_SIZE, priv->vender_flag)))
		printk("get vender flag through mtd failed! ret %d\n",
				ret);
	priv->exist_flag |= (1 << READ_VENDER_FLAG);
	factory_print_string("vender flag is:", priv->vender_flag, VENDER_FLAG_SIZE);
	// get vender
	if ((ret = get_value_through_mtd(np, "mtd-vender", 0, VENDER_SIZE,
			     priv->vender)))
		printk("get vender through mtd failed! ret %d\n", ret);
	priv->exist_flag |= (1 << READ_VENDER);
	factory_print_string("vender is:", priv->vender, VENDER_SIZE);

	// get product key flag
	if ((ret = get_value_through_mtd(np, "mtd-product-key-flag", 0,
			     PRODUCT_KEY_FLAG_SIZE, priv->product_key_flag)))
		printk("get product key flag through mtd failed! ret %d\n",
				ret);
	priv->exist_flag |= (1 << READ_PRODUCT_KEY_FLAG);
	factory_print_string("product key flag is:",  priv->product_key_flag, PRODUCT_KEY_FLAG_SIZE);
	// get product key
	if ((ret = get_value_through_mtd(np, "mtd-product-key", 0, PRODUCT_KEY_SIZE,
			     priv->product_key)))
		printk("get product key through mtd failed! ret %d\n", ret);
	priv->exist_flag |= (1 << READ_PRODUCT_KEY);
	factory_print_string("product key is:", priv->product_key, PRODUCT_KEY_SIZE);

	// get login info flag
	if ((ret = get_value_through_mtd(np, "mtd-login-info-flag", 0,
			     LOGIN_INFO_FLAG_SIZE, priv->login_info_flag)))
		printk("get login info flag through mtd failed! ret %d\n",
				ret);
	priv->exist_flag |= (1 << READ_LOGIN_INFO_FLAG);
	factory_print_string("login info flag is:",  priv->login_info_flag, LOGIN_INFO_FLAG_SIZE);
	// get login info
	if ((ret = get_value_through_mtd(np, "mtd-login-info", 0, LOGIN_INFO_SIZE,
			     (unsigned char *)&priv->login_info)))
		printk("get login info through mtd failed! ret %d\n", ret);
	priv->exist_flag |= (1 << READ_LOGIN_INFO);
	printk("login info is %#X\n", priv->login_info);

	// get rom type flag
	if ((ret = get_value_through_mtd(np, "mtd-rom-type-flag", 0,
			     ROM_TYPE_FLAG_SIZE, priv->rom_type_flag)))
		printk("get rom type flag through mtd failed! ret %d\n",
				ret);
	priv->exist_flag |= (1 << READ_ROM_TYPE_FLAG);
	factory_print_string("rom type flag is:", priv->rom_type_flag, ROM_TYPE_FLAG_SIZE);
	// get rom type
	if ((ret = get_value_through_mtd(np, "mtd-rom-type", 0, ROM_TYPE_SIZE,
			     (unsigned char *)&priv->rom_type)))
		printk("get rom type through mtd failed! ret %d\n", ret);
	priv->exist_flag |= (1 << READ_ROM_TYPE);
	printk("rom type is %#x\n", priv->rom_type);

	// get wifi version
	if ((ret = get_value_through_mtd(np, "mtd-wifi-version", 0,
			     WIFI_VERSION_SIZE, priv->wifi_version)))
		printk("get wifi version failed! ret %d\n", ret);
	//check if some value have been saved in flash through a mark value "XO" or "V2"
	if ((priv->wifi_version[0] == 'X' && priv->wifi_version[1] == 'O') ||
			(priv->wifi_version[0] == 'V')) {
		priv->exist_flag |= 1 << READ_WIFI_VERSION;
		factory_print_string("get wifi version:",  priv->wifi_version, WIFI_VERSION_SIZE);
	} else {
		factory_print_string("Do not find XO cali value in factory, mark is:",
				     priv->wifi_version, WIFI_VERSION_SIZE);
	}

    //read wifi info when wifi version is valid
    if(priv->exist_flag & (1 << READ_WIFI_VERSION)){
	    // get wifi info
        if ((ret = get_value_through_mtd(np, "mtd-wifi-info", 0, WIFI_INFO_SIZE_V4,
                        priv->wifi_info)))
            printk("get wifi info through mtd failed! ret %d\n", ret);
        /* XO value is the first two bytes in wifi_info */
        priv->exist_flag |= (1 << READ_RF_XO_CONFIG);
        priv->exist_flag |= (1 << READ_WIFI_INFO);
    }

    // RM#9069 get cooling temp
    if ((ret = get_value_through_mtd(np, "mtd-cooling-temp", 0, COOLING_TEMP_SIZE,
				priv->cooling_temp)))
		printk("get cooling temp through mtd failed! ret %d\n", ret);

	priv->exist_flag |= (1 << READ_COOLING_TEMP);

	// get gmac delay
	if ((ret = get_value_through_mtd(np, "mtd-gmac-delay", 0, GMAC_DELAY_SIZE,
			     priv->gmac_delay)))
		printk("get gmac delay through mtd failed! ret %d\n", ret);
	/* txDelay is the first two bytes in gmac_delay */
	priv->exist_flag |= (1 << READ_GMAC_DELAY);
	factory_print_string("get gmac delay:", priv->gmac_delay, GMAC_DELAY_SIZE);

	// get default_ssid_lb
	if ((ret = get_value_through_mtd(np, "mtd-default-ssid-lb", 0, DEFAULT_SSID_LB_SIZE,
			     priv->default_ssid_lb)))
		printk("get default_ssid_lb through mtd failed! ret %d\n", ret);
	priv->exist_flag |= (1 << READ_DEFAULT_SSID_LB);
	factory_print_string("default_ssid_lb is:", priv->default_ssid_lb, DEFAULT_SSID_LB_SIZE);

	// get default_ssid_hb
	if ((ret = get_value_through_mtd(np, "mtd-default-ssid-hb", 0, DEFAULT_SSID_HB_SIZE,
			     priv->default_ssid_hb)))
		printk("get default_ssid_hb through mtd failed! ret %d\n", ret);
	priv->exist_flag |= (1 << READ_DEFAULT_SSID_HB);
	factory_print_string("default_ssid_hb is:", priv->default_ssid_hb, DEFAULT_SSID_HB_SIZE);

	// get default_key_lb
	if ((ret = get_value_through_mtd(np, "mtd-default-key-lb", 0, DEFAULT_KEY_LB_SIZE,
			     priv->default_key_lb)))
		printk("get default_key_lb through mtd failed! ret %d\n", ret);
	priv->exist_flag |= (1 << READ_DEFAULT_KEY_LB);
	factory_print_string("default_key_lb is:", priv->default_key_lb, DEFAULT_KEY_LB_SIZE);

	// get default_key_hb
	if ((ret = get_value_through_mtd(np, "mtd-default-key-hb", 0, DEFAULT_KEY_HB_SIZE,
			     priv->default_key_hb)))
		printk("get default_key_hb through mtd failed! ret %d\n", ret);
	priv->exist_flag |= (1 << READ_DEFAULT_KEY_HB);
	factory_print_string("default_key_hb is:", priv->default_key_hb, DEFAULT_KEY_HB_SIZE);

	return 0;
}

int sf_get_value_from_factory(enum sfax8_factory_read_action action,
		void *buffer,
		int len)
{
	int length = len;
	if (!buffer) {
		printk("buffer is null\n");
		return -1;
	}
	switch (action) {
	case READ_MAC_ADDRESS:
		if (!(f_read_ctx->exist_flag & (1 << READ_MAC_ADDRESS))) {
			printk("Can not find mac address!\n");
			return -2;
		}
		if (len > MACADDR_SIZE) {
			length = MACADDR_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->macaddr, length);
		break;
	case READ_WAN_MAC_ADDRESS:
		if (!(f_read_ctx->exist_flag & (1 << READ_MAC_ADDRESS))) {
			printk("Can not find mac address!\n");
			return -2;
		}
		if (len > MACADDR_SIZE) {
			length = MACADDR_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->wan_macaddr, length);
		break;
#ifdef CONFIG_SFAX8_HNAT_MULTI_WAN
	case READ_WWAN_MAC_ADDRESS:
		if (!(f_read_ctx->exist_flag & (1 << READ_MAC_ADDRESS))) {
			printk("Can not find mac address!\n");
			return -2;
		}
		if (len > MACADDR_SIZE) {
			length = MACADDR_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->wwan_macaddr, length);
		break;
	case READ_LAN2_MAC_ADDRESS:
		if (!(f_read_ctx->exist_flag & (1 << READ_MAC_ADDRESS))) {
			printk("Can not find mac address!\n");
			return -2;
		}
		if (len > MACADDR_SIZE) {
			length = MACADDR_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->lan2_macaddr, length);
		break;
#endif
	case READ_WIFI_LB_MAC_ADDRESS:
		if (!(f_read_ctx->exist_flag & (1 << READ_MAC_ADDRESS))) {
			printk("Can not find mac address!\n");
			return -2;
		}
		if (len > MACADDR_SIZE) {
			length = MACADDR_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->wifi_lb_macaddr, length);
		break;
	case READ_WIFI_HB_MAC_ADDRESS:
		if (!(f_read_ctx->exist_flag & (1 << READ_MAC_ADDRESS))) {
			printk("Can not find mac address!\n");
			return -2;
		}
		if (len > MACADDR_SIZE) {
			length = MACADDR_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->wifi_hb_macaddr, length);
		break;
	case READ_SN:
		if (!(f_read_ctx->exist_flag & (1 << READ_SN))) {
			printk("Can not find sn number!\n");
			return -2;
		}
		if (len > SN_SIZE) {
			length = SN_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->sn, length);
		break;
	case READ_SN_FLAG:
		if (!(f_read_ctx->exist_flag & (1 << READ_SN_FLAG))) {
			printk("Can not find sn flag!\n");
			return -2;
		}
		if (len > SN_FLAG_SIZE) {
			length = SN_FLAG_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		*(unsigned char *)buffer = f_read_ctx->sn_flag;
		break;
	case READ_PCBA_BOOT:
		if (!(f_read_ctx->exist_flag & (1 << READ_PCBA_BOOT))) {
			printk("Can not find pcab boot mark!\n");
			return -2;
		}
		if (len > PCBA_BOOT_SIZE) {
			length = PCBA_BOOT_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->pcba_boot, length);
		break;
	case READ_HARDWARE_VER_FLAG:
		if (!(f_read_ctx->exist_flag & (1 << READ_HARDWARE_VER_FLAG))) {
			printk("Can not find hardware version flag!\n");
			return -2;
		}
		if (len > HARDWARE_VER_FLAG_SIZE) {
			length = HARDWARE_VER_FLAG_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->hw_ver_flag, length);
		break;
	case READ_HARDWARE_VER:
		if (!(f_read_ctx->exist_flag & (1 << READ_HARDWARE_VER))) {
			printk("Can not find hardware version!\n");
			return -2;
		}
		if (len > HARDWARE_VER_SIZE) {
			length = HARDWARE_VER_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->hw_ver, length);
		break;
	case READ_MODEL_VER_FLAG:
		if (!(f_read_ctx->exist_flag & (1 << READ_MODEL_VER_FLAG))) {
			printk("Can not find model version flag!\n");
			return -2;
		}
		if (len > MODEL_VER_FLAG_SIZE) {
			length = MODEL_VER_FLAG_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->model_ver_flag, length);
		break;
	case READ_MODEL_VER:
		if (!(f_read_ctx->exist_flag & (1 << READ_MODEL_VER))) {
			printk("Can not find model version!\n");
			return -2;
		}
		if (len > MODEL_VER_SIZE) {
			length = MODEL_VER_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->model_ver, length);
		break;
	case READ_COUNTRY_ID:
		if (!(f_read_ctx->exist_flag & (1 << READ_COUNTRY_ID))) {
			printk("Can not find country id!\n");
			return -2;
		}
		if (len > COUNTRYID_SIZE) {
			length = COUNTRYID_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->countryID, length);
		break;
	case READ_HW_FEATURE:
		if (!(f_read_ctx->exist_flag & (1 << READ_HW_FEATURE))) {
			printk("Can not find hw feature!\n");
			return -2;
		}
		if (len > HW_FEATURE_SIZE) {
			length = HW_FEATURE_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		*(unsigned int *)buffer = f_read_ctx->hw_feature;
		break;
	case READ_VENDER_FLAG:
		if (!(f_read_ctx->exist_flag & (1 << READ_VENDER_FLAG))) {
			printk("Can not find vender flag!\n");
			return -2;
		}
		if (len > VENDER_FLAG_SIZE) {
			length = VENDER_FLAG_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->vender_flag, length);
		break;
	case READ_VENDER:
		if (!(f_read_ctx->exist_flag & (1 << READ_VENDER))) {
			printk("Can not find vender!\n");
			return -2;
		}
		if (len > VENDER_SIZE) {
			length = VENDER_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->vender, length);
		break;
	case READ_PRODUCT_KEY_FLAG:
		if (!(f_read_ctx->exist_flag & (1 << READ_PRODUCT_KEY_FLAG))) {
			printk("Can not find product key flag!\n");
			return -2;
		}
		if (len > PRODUCT_KEY_FLAG_SIZE) {
			length = PRODUCT_KEY_FLAG_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->product_key_flag, length);
		break;
	case READ_PRODUCT_KEY:
		if (!(f_read_ctx->exist_flag & (1 << READ_PRODUCT_KEY))) {
			printk("Can not find product key!\n");
			return -2;
		}
		if (len > PRODUCT_KEY_SIZE) {
			length = PRODUCT_KEY_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->product_key, length);
		break;
	case READ_LOGIN_INFO_FLAG:
		if (!(f_read_ctx->exist_flag & (1 << READ_LOGIN_INFO_FLAG))) {
			printk("Can not find log info flag!\n");
			return -2;
		}
		if (len > LOGIN_INFO_FLAG_SIZE) {
			length = LOGIN_INFO_FLAG_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->login_info_flag, length);
		break;
	case READ_LOGIN_INFO:
		if (!(f_read_ctx->exist_flag & (1 << READ_LOGIN_INFO))) {
			printk("Can not find login info!\n");
			return -2;
		}
		if (len > LOGIN_INFO_SIZE) {
			length = LOGIN_INFO_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, &f_read_ctx->login_info, length);
		break;
	case READ_ROM_TYPE_FLAG:
		if (!(f_read_ctx->exist_flag & (1 << READ_ROM_TYPE_FLAG))) {
			printk("Can not find rom type flag!\n");
			return -2;
		}
		if (len > ROM_TYPE_FLAG_SIZE) {
			length = ROM_TYPE_FLAG_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->rom_type_flag, length);
		break;
	case READ_ROM_TYPE:
		if (!(f_read_ctx->exist_flag & (1 << READ_ROM_TYPE))) {
			printk("Can not find rom type!\n");
			return -2;
		}
		if (len > ROM_TYPE_SIZE) {
			length = ROM_TYPE_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, &f_read_ctx->rom_type, length);
		break;
	case READ_WIFI_VERSION:
		if (!(f_read_ctx->exist_flag & (1 << READ_WIFI_VERSION))) {
			printk("Can not find wifi version!\n");
			return -2;
		}
		if (len > WIFI_VERSION_SIZE) {
			length = WIFI_VERSION_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->wifi_version, length);
		break;
	case READ_WIFI_INFO:
		if (!(f_read_ctx->exist_flag & (1 << READ_WIFI_INFO))) {
			printk("Can not find wifi info!\n");
			return -2;
		}
		if (len > WIFI_INFO_SIZE_V4) {
			length = WIFI_INFO_SIZE_V4;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->wifi_info, length);
		break;
	case READ_RF_XO_CONFIG:
		if (!(f_read_ctx->exist_flag & (1 << READ_RF_XO_CONFIG))) {
			printk("Can not find XO config!\n");
			return -2;
		}
		if (len > XO_CONFIG_SIZE) {
			length = XO_CONFIG_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		/* XO value is the first two bytes of wifi_info */
		memcpy(buffer, f_read_ctx->wifi_info, length);
		break;
	case READ_COOLING_TEMP:
		if (!(f_read_ctx->exist_flag & (1 << READ_COOLING_TEMP))) {
			printk("Can not find cooling temperature!\n");
			return -2;
		}
		if (len > COOLING_TEMP_SIZE) {
			length = COOLING_TEMP_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->cooling_temp, length);
		break;
	case READ_GMAC_DELAY:
		if (!(f_read_ctx->exist_flag & (1 << READ_GMAC_DELAY))) {
			printk("Can not find gmac delay!\n");
			return -2;
		}
		if (len > GMAC_DELAY_SIZE) {
			length = GMAC_DELAY_SIZE;
			printk("Your length is larger than max %d\n", length);
		}
		memcpy(buffer, f_read_ctx->gmac_delay, length);
		break;
	default:
		printk("unknow action %d\n", action);
		break;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(sf_get_value_from_factory);

static int sfax8_factory_read_os_resources_get(struct platform_device *pdev,
		struct sfax8_factory_read_context **p_priv)
{
	struct sfax8_factory_read_context *priv;
	int ret;

	priv = devm_kzalloc(&pdev->dev,
			sizeof(struct sfax8_factory_read_context), GFP_KERNEL);
	if (!priv) {
		printk("can not allocate memory!\n");
		ret = -ENOMEM;
		return ret;
	}

	priv->np = pdev->dev.of_node;
	*p_priv = priv;
	f_read_ctx = priv;
	platform_set_drvdata(pdev, priv);
	return 0;
}

static int sfax8_factory_read_os_resources_free(struct platform_device *pdev, struct sfax8_factory_read_context *priv)
{
	devm_kfree(&pdev->dev, priv);
	f_read_ctx = NULL;
	return 0;
}

/*
 *  * func:this is sfax8_factory_read module's probe function, it's do the following things:
 *  1,set the private data to the platform drivers
 *  2,copy value from factory
 *  3,create the dbg fs node
 *  params: struct platform_device *pdev
 *  return:
 *  0 means successful, otherwise failed
 * */
int sfax8_factory_read_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct sfax8_factory_read_context *priv;
	printk("%s...\n", __func__);
	/*Step1:set the platform priv data*/
	if((ret = sfax8_factory_read_os_resources_get(pdev, &priv))){
		printk("sfax8_factory_read_os_resources_get failed, %d!\n", ret);
		return ret;
	}
	/*Step2:copy value from factory*/
	if((ret = save_value_from_factory_to_host(pdev, priv))){
		printk("save_value_from_factory_to_host failed, %d!\n", ret);
		goto ERROR;
	}
#ifdef CONFIG_SF16A18_FACTORY_READ_SYSFS_DEBUG
	/*Step3:create the dbg fs node*/
	if((ret = sf_factory_read_sysfs_register(pdev, "sfax8_factory_read"))){
		printk("save_value_from_factory_to_host failed, %d!\n", ret);
		goto ERROR;
	}
#endif
	return 0;
ERROR:
	sfax8_factory_read_os_resources_free(pdev, priv);
	return ret;
}


int sfax8_factory_read_remove(struct platform_device *pdev)
{
	struct sfax8_factory_read_context *priv;
	printk("%s...\n", __func__);
	priv = (struct sfax8_factory_read_context *)platform_get_drvdata(pdev);

#ifdef CONFIG_SF16A18_FACTORY_READ_SYSFS_DEBUG
	sf_factory_read_sysfs_unregister(pdev);
#endif
	sfax8_factory_read_os_resources_free(pdev, priv);
	return 0;
}

/* *********************************************************************Register Platform Drivers******************************************************/
static const struct of_device_id sfax8_factory_read_of_match[] = {
	{
		.compatible = "siflower,sfax8-factory-read",
	},
	{},
};

static struct platform_driver sfax8_factory_read_driver = {
	.probe      = sfax8_factory_read_probe,
	.remove     = sfax8_factory_read_remove,
	.driver     = {
		.name   = "sfax8_factory_read",
		.of_match_table = sfax8_factory_read_of_match,
	},
};

static int __init sfax8_factory_read_init(void)
{
	return platform_driver_register(&sfax8_factory_read_driver);
}

static void __exit sfax8_factory_read_exit(void)
{
	platform_driver_unregister(&sfax8_factory_read_driver);
}

module_init(sfax8_factory_read_init);
module_exit(sfax8_factory_read_exit);

MODULE_AUTHOR("Zhengjinyang Jiang <star.jiang@siflower.com.cn>");
MODULE_DESCRIPTION("Get value from factory");
MODULE_LICENSE("GPL");
