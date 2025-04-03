/**
 ******************************************************************************
 *
 * @file siwifi_factory_info.c
 *
 * @brief siwifi driver use factory info func
 *
 * Copyright (C) Siflower 2018-2025
 *
 ******************************************************************************
 */

#include "siwifi_defs.h"
#include "siwifi_factory_info.h"
#include "siwifi_lmac_glue.h"
#include "siwifi_cfgfile.h"
#ifdef CONFIG_SFAX8_FACTORY_READ
#include <sfax8_factory_read.h>
#else
#define XO_CONFIG_SIZE 2
#define WIFI_INFO_SIZE 2046
#define WIFI_INFO_SIZE_V4 3380
#endif
#include "siwifi_mem.h"

/* WIFI Factory Info Description:
 * For each board, we wil record some info according to factory testing, and in
 * the driver, we use it to get a better performance.
 * Now wifi info in factory include XO value, txpower calibration value:
 * XO value is used to adjust the clk in rf driver.
 * Txpower calibration value is used to modify the txpower of each tx frame to
 * pass the EVM or follow the regulatory domain.
 *
 * Whole Txpower Calibration List:
 * A whole txpower calibration list include the txpower value of each channel
 * and each rate:
 * In 2.4G, we have 4 11b rates, 8 11g rates, 8 11n 20MHz rates and 8 11n 40MHz
 * rates, total 28(4 + 8 + 8 + 8) rates in one channel, and 13 channels(channel
 * 1-13), so the list has 384(13 * 28) bytes to record each channel and each
 * value.
 * In 5G, we have 8 11a rates, 8 11n 20MHz rates, 8 11n 40MHz rates, 9 11ac
 * 20MHz rates, 10 11ac 40MHz rates and 10 11ac 80MHz rates, total 53(8 + 8 + 8
 * + 9 + 10 + 10) rates in one channel, and 25 channels(channel 36-64, 100-144,
 * 149-165), so the list has 1325(25 * 53) bytes to record each channel and each
 * value.
 * When we use a whole txpower calibration list in lmac, we just need to
 * calculate the offset, and we can quickly find the txpower value in the list.
 *
 * TX Power Mode:
 * mac80211 has a txpower interface BSS_CHANGED_TXPOWER. It will pass an
 * txpower(DB) to the driver. But now we don't use it.
 * SIWIFI driver use txpower_lvl to indictae TX power mode. It has three levels:
 * low, standard and high, and lmac will use different txpower lists when at
 * different levels. In different factory info verions, the list may have
 * different meanings.
 *
 * Factory WIFI Info Version: now we have verion 1 and 2.
 * Version 1:
 * Version 1 WIFI factory info only has one lb/hb whole txpower calibration
 * list. So lmac will use the same list whatever the txpower_lvl is.
 * Version 1 table in factory:
 * offset          cnt                 description
 * 0               2                   magic value 'X''O'
 * 2               2                   XO value
 * 4               364(LB_LIST_LEN)    lb txpower calibration list
 * 368             1325(HB_LIST_LEN)   hb txpower calibration list
 *
 * Version 2:
 * Version 2 WIFI factory info has two lb/hb whole txpower calibration lists:
 * normal list and high list. The high list means the value in it is the max
 * txpower when the EVM is pass and we ingore the regulatory domain. The normal
 * list means the value in it is the max txpower when the EVM is pass and it is
 * also following the regulatory domain. The max factory wifi info size is 2K
 * and it can't record two full lists, so we record the full normal list and
 * only one channel for lb/hb high list. In this driver, we use this channel
 * high list to get the whole high list. When the txpower_lvl is
 * SIWIFI_TXPOWER_LVL_HIGH, we use the high list; when the txpower_lvl is
 * SIWIFI_TXPOWER_LVL_LOW or SIWIFI_TXPOWER_LVL_STANDARD, we use the normal list in
 * lmac.
 * Version 2 table in factory:
 * offset          cnt                 description
 * 0               2                   magic value 'V''2'
 * 2               2                   XO value
 * 4               364(LB_LIST_LEN)    lb txpower calibration normal list
 * 368             1325(HB_LIST_LEN)   hb txpower calibration normal list
 * 1693            1                   magic value 24
 * 1694            28                  channel 1 txpower calibration high value
 * 1722            1                   magic value 5
 * 1723            53                  channel 36 txpower calibration high value
 *
 * */

static int sf_wifi_init_normal_txpower_list(
                struct siwifi_factory_info *factory_info,
                int lb,
                char *wifi_info_buf)
{
    if (lb) {
        factory_info->list_len = LB_LIST_LEN;
        factory_info->normal_txpower_list = (char *)siwifi_kzalloc(
                        factory_info->list_len, GFP_KERNEL);
        if (!factory_info->normal_txpower_list)
            return -1;
        memcpy(factory_info->normal_txpower_list,
                        wifi_info_buf + XO_CONFIG_SIZE,
                        factory_info->list_len);
    } else {
        factory_info->list_len = HB_LIST_LEN;
        factory_info->normal_txpower_list = (char *)siwifi_kzalloc(
                        factory_info->list_len, GFP_KERNEL);
        if (!factory_info->normal_txpower_list)
            return -1;
        memcpy(factory_info->normal_txpower_list,
                        wifi_info_buf + XO_CONFIG_SIZE + LB_LIST_LEN,
                        factory_info->list_len);
    }
    return 0;
}

static int sf_wifi_init_sleepmode_txpower_list(struct siwifi_factory_info *factory_info, int lb, struct siwifi_hw *siwifi_hw)
{
	factory_info->sleepmode_txpower_list = (char *)siwifi_kzalloc(factory_info->list_len, GFP_KERNEL);
	if (!factory_info->sleepmode_txpower_list)
		return -1;
	//copy normal table to sleepmode table,then update it
	memcpy(factory_info->sleepmode_txpower_list,factory_info->normal_txpower_list,factory_info->list_len);
	siwifi_update_txpower_offset_configfile(factory_info->sleepmode_txpower_list, 0, lb, siwifi_hw, 0);
	return 0;
}

static int sf_wifi_init_low_txpower_list(struct siwifi_factory_info *factory_info, int lb, struct siwifi_hw *siwifi_hw)
{
	factory_info->low_txpower_list = (char *)siwifi_kzalloc(factory_info->list_len, GFP_KERNEL);
	if (!factory_info->low_txpower_list)
		return -1;
	//copy normal table to low table,then update it
	memcpy(factory_info->low_txpower_list,factory_info->normal_txpower_list,factory_info->list_len);
	siwifi_update_txpower_offset_configfile(factory_info->low_txpower_list, 1, lb, siwifi_hw, 0);
    return 0;
}

#ifdef CFG_DUAL_ANTENNA_CALIBRATE
static int sf_wifi_init_normal_txpower_list_second_antenna(
                struct siwifi_factory_info *factory_info,
                int lb,
                char *wifi_info_buf)
{
    if (lb) {
        factory_info->normal_txpower_list_second_antenna = (char *)siwifi_kzalloc(
                        factory_info->list_len, GFP_KERNEL);
        if (!factory_info->normal_txpower_list_second_antenna)
            return -1;
        memcpy(factory_info->normal_txpower_list_second_antenna,
                        wifi_info_buf + XO_CONFIG_SIZE + LB_LIST_LEN + HB_LIST_LEN,
                        factory_info->list_len);
    } else {
        factory_info->normal_txpower_list_second_antenna = (char *)siwifi_kzalloc(
                        factory_info->list_len, GFP_KERNEL);
        if (!factory_info->normal_txpower_list_second_antenna)
            return -1;
        memcpy(factory_info->normal_txpower_list_second_antenna,
                        wifi_info_buf + XO_CONFIG_SIZE + LB_LIST_LEN + LB_LIST_LEN + HB_LIST_LEN,
                        factory_info->list_len);
    }
    return 0;
}

static int sf_wifi_init_sleepmode_txpower_list_second_antenna(struct siwifi_factory_info *factory_info, int lb, struct siwifi_hw *siwifi_hw)
{
	factory_info->sleepmode_txpower_list_second_antenna = (char *)siwifi_kzalloc(factory_info->list_len, GFP_KERNEL);
	if (!factory_info->sleepmode_txpower_list)
		return -1;
	//copy normal table to sleepmode table,then update it
	memcpy(factory_info->sleepmode_txpower_list_second_antenna,factory_info->normal_txpower_list_second_antenna,factory_info->list_len);
	siwifi_update_txpower_offset_configfile(factory_info->sleepmode_txpower_list_second_antenna, 0, lb, siwifi_hw, 1);
	return 0;
}

static int sf_wifi_init_low_txpower_list_second_antenna(struct siwifi_factory_info *factory_info, int lb, struct siwifi_hw *siwifi_hw)
{
	factory_info->low_txpower_list_second_antenna = (char *)siwifi_kzalloc(factory_info->list_len, GFP_KERNEL);
	if (!factory_info->low_txpower_list_second_antenna)
		return -1;
	//copy normal table to low table,then update it
	memcpy(factory_info->low_txpower_list_second_antenna,factory_info->normal_txpower_list_second_antenna,factory_info->list_len);
	siwifi_update_txpower_offset_configfile(factory_info->low_txpower_list_second_antenna, 1, lb, siwifi_hw, 1);
	return 0;
}
#endif

/* How to get high txpower list in Version 2?
 * 2.4G: The regulatory domain of each channel is the same 20db in CN. We test
 * channel 1 and copy it to each channel.
 * 5G: The regulatory domain of channel 36-64 is 23db, and the regulatory domain
 * of channel 149-165 is 30db in CN. we can reach 23db but can't reach 30db. So
 * we test channel 36 and copy it to channel 36-64, and copy the value of
 * channel 149-165 in normal list to high list.
 */
static int sf_wifi_init_high_txpower_list(
                struct siwifi_factory_info *factory_info,
                int lb,
                char *wifi_info_buf)
{
    int i = 0;
	/* Now if the starting value of the high table is wrong, the higgh table will
	 * be filled with 0 instead of thinking that the entire calibration table is
	 * unavailable
	 */
	int wrong_start_value = 0;
    if (lb) {
        char *lb_chan_gain = wifi_info_buf + XO_CONFIG_SIZE +
                             LB_LIST_LEN + HB_LIST_LEN;
        if (lb_chan_gain[0] != 24) {
            printk("the magic key of lb v2 more info is not "
                   "matched. %d vs 24\n",
                            lb_chan_gain[0]);
            wrong_start_value = 1;
        }
        factory_info->high_txpower_list = (char *)siwifi_kzalloc(
                        factory_info->list_len, GFP_KERNEL);
        if (!factory_info->high_txpower_list)
            goto err;

		if (wrong_start_value) {
				memset(factory_info->high_txpower_list, 0, factory_info->list_len);
		} else {
			// copy the value of channel 1 to each channel.
			for (i = 0; i <= LB_CHANNEL_COUNT; i++) {
				memcpy(factory_info->high_txpower_list + i * LB_ONE_CHAN_GAIN_NUM,
						lb_chan_gain + LB_V2_MAGIC_INFO_SIZE, LB_ONE_CHAN_GAIN_NUM);
			}
		}
    } else {
        char *hb_chan_gain = wifi_info_buf + XO_CONFIG_SIZE +
                             LB_LIST_LEN + HB_LIST_LEN + LB_V2_MORE_INFO_SIZE;
        if (hb_chan_gain[0] != 5) {
            printk("the magic key of hb v2 more info is not "
                   "matched. %d vs 5\n",
                            hb_chan_gain[0]);
             wrong_start_value = 1;
        }
        factory_info->high_txpower_list = (char *)siwifi_kzalloc(
                        factory_info->list_len, GFP_KERNEL);
        if (!factory_info->high_txpower_list)
            goto err;

		if (wrong_start_value) {
				memset(factory_info->high_txpower_list, 0, factory_info->list_len);
		}else{
			// firstly, copy normal power list to high power list, because
			// most of them are the same
			memcpy(factory_info->high_txpower_list,
						factory_info->normal_txpower_list,
						factory_info->list_len);

			// only copy the value to 7 channels: channel 36 to 64.
			for (i = 0; i <= 7; i++) {
				memcpy(factory_info->high_txpower_list + i * HB_ONE_CHAN_GAIN_NUM,
						hb_chan_gain + HB_V2_MAGIC_INFO_SIZE, HB_ONE_CHAN_GAIN_NUM);
			}
		}
    }
    return 0;
err:
    if (factory_info->high_txpower_list)
        siwifi_kfree(factory_info->high_txpower_list);
    return -1;
}

//the wifi version should be "XO" or "V2" or "V3"
int sf_wifi_check_calibration_table_available(char *buf){
	if (buf[0] == 'X' && buf[1] == 'O')
        return 1;
    else if (buf[0] == 'V' && buf[1] == '2')
        return 2;
    else if (buf[0] == 'V' && buf[1] == '3')
        return 3;
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
    else if (buf[0] == 'V' && buf[1] == '4')
		return 4;
#endif
    else {
        return 0;
    }
}

int sf_wifi_init_wifi_factory_info(struct siwifi_hw *siwifi_hw)
{
    int ret = 0;
#ifdef CONFIG_SFAX8_FACTORY_READ
    char buf[WIFI_VERSION_SIZE];
#endif
    struct v1_plat_data *priv =
                    (struct v1_plat_data *)&siwifi_hw->plat->priv;
    int lb = priv->band & LB_MODULE;
    struct siwifi_factory_info *factory_info = &siwifi_hw->factory_info;
    char *wifi_info_buf = NULL;
    uint16_t xo_value = 0;
	//the size of txpower calibration table is 4096 bytes
	unsigned char *txpower_table_value;
	u32 size_calitable = 4096;
	u32 wifi_info_size = WIFI_INFO_SIZE;
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
	int dual_antenna_calibrate = 0;
#endif
	struct file *fp_read = NULL;
	char fisrt_txpower_table_file[] = "/usr/bin/txpower_calibrate_table.bin";
	char second_txpower_table_file[] = "/lib/firmware/default_txpower_calibrate_table.bin";
	struct phy_aetnensis_cfg_tag *phy_tag;
	char second_txpower_expa_table_file[] = "/lib/firmware/default_txpower_calibrate_expa_table.bin";

	phy_tag=(struct phy_aetnensis_cfg_tag *)&siwifi_hw->phy_config;

#ifdef CFG_DUAL_ANTENNA_CALIBRATE
	//update some data when dual antenna calibration is on
	if ((phy_tag->flag & DUAL_ANTENNA_CALIBRATE)){
		//printk("------read the flag DUAL_ANTENNA_CALIBRATE\n");
		dual_antenna_calibrate = 1;
		size_calitable = 5430;
		wifi_info_size = WIFI_INFO_SIZE_V4;
	}
#endif

	//RM#8000 support reading calibration table from file
	txpower_table_value = (unsigned char *)siwifi_kzalloc(size_calitable, GFP_ATOMIC);
	if(!txpower_table_value){
		printk("malloc txpower_table_value fail\n");
		goto err;
	}

	wifi_info_buf = (char *)siwifi_kzalloc(wifi_info_size, GFP_KERNEL);
	if (!wifi_info_buf) {
		printk("can not malloc wifi info buf\n");
		goto err;
	}

	//check txpower_calibrate_table.bin
	fp_read = filp_open(fisrt_txpower_table_file, O_RDONLY , 0);
	if(!IS_ERR(fp_read)){
		memset(txpower_table_value, 0, size_calitable);
		kernel_read(fp_read, txpower_table_value, size_calitable, &fp_read->f_pos);
		filp_close(fp_read, NULL);
		factory_info->version = sf_wifi_check_calibration_table_available((char *)&txpower_table_value[2048]);
		if(factory_info->version == 0){
			printk("factory_info->version in txpower_calibrate_table.bin is wrong!\n");
		}
		else{
			printk("txpower calibration table use txpower_calibrate_table.bin\n");
			memcpy(wifi_info_buf, &txpower_table_value[2050], wifi_info_size);
			goto read_xo_value;
		}
	}
	else{
		//printk("txpower_calibrate_table.bin is not exist!\n");
	}

	//check factory
#ifdef CONFIG_SFAX8_FACTORY_READ
    ret = sf_get_value_from_factory(
                    READ_WIFI_VERSION, buf, WIFI_VERSION_SIZE);
    if (ret) {
        factory_info->version = 0;
    } else {
        factory_info->version = sf_wifi_check_calibration_table_available(buf);
    }

	if (sf_get_value_from_factory(
                        READ_WIFI_INFO, wifi_info_buf, wifi_info_size)) {
        printk("can not get READ_LB_MORE_INFO from factory\n");
        factory_info->version = 0;
    }else{
		printk("txpower calibration table use factory info\n");
		goto read_xo_value;
	}
#else
    factory_info->version = 0;
#endif

	//check default_txpower_calibrate_table.bin when factory is empty
	if(factory_info->version == 0){
		if ((phy_tag->flag & RF_EXTERNAL_PA_ENABLE)) {
			fp_read = filp_open(second_txpower_expa_table_file, O_RDONLY , 0);
		}
		else{
			fp_read = filp_open(second_txpower_table_file, O_RDONLY , 0);
		}
		if(!IS_ERR(fp_read)){
			memset(txpower_table_value, 0, size_calitable);
			kernel_read(fp_read, txpower_table_value, size_calitable, &fp_read->f_pos);
			filp_close(fp_read, NULL);
			factory_info->version = sf_wifi_check_calibration_table_available((char *)&txpower_table_value[2048]);
			if(factory_info->version == 0){
				printk("factory_info->version in default_txpower_calibrate_table.bin is wrong!\n");
				goto err;
			}
			else{
				memcpy(wifi_info_buf, &txpower_table_value[2050], wifi_info_size);
				printk("txpower calibration table use default_txpower_calibrate_table.bin\n");
			}
		}
		else{
			//printk("default_txpower_calibrate_table.bin is not exist!\n");
			goto err;
		}
	}

read_xo_value:
#ifdef CONFIG_SFAX8_FACTORY_READ
	if (sf_get_value_from_factory(
                        READ_RF_XO_CONFIG, &xo_value, XO_CONFIG_SIZE)) {
        //printk("get XO config from sf_factory_read failed\n");
#endif
		//try to get XO value from deautlt_txpower_calibrate.bin
		fp_read = filp_open(second_txpower_table_file, O_RDONLY , 0);
		if(!IS_ERR(fp_read)){
			memset(txpower_table_value, 0, size_calitable);
			kernel_read(fp_read, txpower_table_value, size_calitable, &fp_read->f_pos);
			filp_close(fp_read, NULL);
			factory_info->version = sf_wifi_check_calibration_table_available((char *)&txpower_table_value[2048]);
			if(factory_info->version == 0){
				goto err;
			}
			else{
				printk("get XO config from deautlt_txpower_calibrate.bin\n");
				memcpy(&xo_value, &txpower_table_value[2050], XO_CONFIG_SIZE);
			}
		}
		else{
			goto err;
		}
#ifdef CONFIG_SFAX8_FACTORY_READ
    }
#endif
	factory_info->xo_value = xo_value;
	//printk("xo_value is %x\n",factory_info->xo_value);

    switch (factory_info->version) {
    case 1:
    // fall through
    case 2:
    case 3:
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
	case 4:
#endif
        ret = sf_wifi_init_normal_txpower_list(
                        factory_info, lb, wifi_info_buf);
        if (ret)
            goto err;

		if(sf_wifi_init_sleepmode_txpower_list(factory_info, lb, siwifi_hw))
			goto err;

		if(sf_wifi_init_low_txpower_list(factory_info, lb, siwifi_hw))
			goto err;

        if ((factory_info->version == 2) || (factory_info->version == 3)) {
            ret = sf_wifi_init_high_txpower_list(
                            factory_info, lb, wifi_info_buf);
            if (ret)
                goto err;
        }
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
		if ((dual_antenna_calibrate == 1) && (factory_info->version == 4)){
			if(sf_wifi_init_normal_txpower_list_second_antenna(factory_info, lb, wifi_info_buf))
				goto err;

			if(sf_wifi_init_sleepmode_txpower_list_second_antenna(factory_info, lb, siwifi_hw))
				goto err;

			if(sf_wifi_init_low_txpower_list_second_antenna(factory_info, lb, siwifi_hw))
				goto err;
		}
#endif
		break;
    case 0:
    default:
        break;
    }
    siwifi_kfree(wifi_info_buf);
	siwifi_kfree(txpower_table_value);
    return 0;
err:
    sf_wifi_deinit_wifi_factory_info(siwifi_hw);
    if (wifi_info_buf){
        siwifi_kfree(wifi_info_buf);
	}
	if (txpower_table_value){
		siwifi_kfree(txpower_table_value);
	}
    return -1;
}

void sf_wifi_deinit_wifi_factory_info(struct siwifi_hw *siwifi_hw)
{
    struct siwifi_factory_info *factory_info = &siwifi_hw->factory_info;
    if (factory_info->sleepmode_txpower_list)
        siwifi_kfree(factory_info->sleepmode_txpower_list);
    if (factory_info->low_txpower_list)
        siwifi_kfree(factory_info->low_txpower_list);
    if (factory_info->normal_txpower_list)
        siwifi_kfree(factory_info->normal_txpower_list);
    if (factory_info->high_txpower_list)
        siwifi_kfree(factory_info->high_txpower_list);
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
    if (factory_info->sleepmode_txpower_list_second_antenna)
        siwifi_kfree(factory_info->sleepmode_txpower_list_second_antenna);
    if (factory_info->low_txpower_list_second_antenna)
        siwifi_kfree(factory_info->low_txpower_list_second_antenna);
    if (factory_info->normal_txpower_list_second_antenna)
        siwifi_kfree(factory_info->normal_txpower_list_second_antenna);
#endif
    memset(&siwifi_hw->factory_info, 0, sizeof(struct siwifi_factory_info));
}
