/**
 ****************************************************************************************
 *
 * @file siwifi_configparse.c
 *
 * Copyright (C) Siflower 2018-2025
 *
 ****************************************************************************************
 */
#include <linux/firmware.h>
#include <linux/if_ether.h>

#include "siwifi_defs.h"
#include "siwifi_cfgfile.h"
#include "siwifi_msg_tx.h"

/**
 *
 */
static const char *siwifi_find_tag(const u8 *file_data, unsigned int file_size,
                                 const char *tag_name, unsigned int tag_len)
{
    unsigned int curr, line_start = 0, line_size;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Walk through all the lines of the configuration file */
    while (line_start < file_size) {
        /* Search the end of the current line (or the end of the file) */
        for (curr = line_start; curr < file_size; curr++)
            if (file_data[curr] == '\n')
                break;

        /* Compute the line size */
        line_size = curr - line_start;

        /* Check if this line contains the expected tag */
        if ((line_size == (strlen(tag_name) + tag_len)) &&
            (!strncmp(&file_data[line_start], tag_name, strlen(tag_name))))
            return (&file_data[line_start + strlen(tag_name)]);

        /* Move to next line */
        line_start = curr + 1;
    }

    /* Tag not found */
    return NULL;
}

/**
 * Parse the Config file used at init time
 */
int siwifi_parse_configfile(struct siwifi_hw *siwifi_hw, const char *filename,
                          struct siwifi_conf_file *config)
{
    const struct firmware *config_fw;
    u8 dflt_mac[ETH_ALEN] = { 0, 111, 111, 111, 111, 0 };
    int ret;
    const u8 *tag_ptr;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    if ((ret = request_firmware(&config_fw, filename, siwifi_hw->dev))) {
        printk(KERN_CRIT "%s: Failed to get %s (%d)\n", __func__, filename, ret);
        return ret;
    }

    /* Get MAC Address */
    tag_ptr = siwifi_find_tag(config_fw->data, config_fw->size,
                            "MAC_ADDR=", strlen("00:00:00:00:00:00"));
    if (tag_ptr != NULL) {
        u8 *addr = config->mac_addr;
        if (sscanf(tag_ptr,
                   "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
                   addr + 0, addr + 1, addr + 2,
                   addr + 3, addr + 4, addr + 5) != ETH_ALEN)
            memcpy(config->mac_addr, dflt_mac, ETH_ALEN);
    } else
        memcpy(config->mac_addr, dflt_mac, ETH_ALEN);

    SIWIFI_DBG("MAC Address is:\n%pM\n", config->mac_addr);

    /* Release the configuration file */
    release_firmware(config_fw);

    return 0;
}

/**
 * Parse the Config file used at init time
 */
int siwifi_parse_phy_configfile(struct siwifi_hw *siwifi_hw, const char *filename,
                              struct siwifi_phy_conf_file *config, int path)
{
//todo shoule be debuged
#if 0
//#ifdef CONFIG_SIWIFI_TX_POWER_CALI
    const struct firmware *config_fw;
    int ret;
    const u8 *tag_ptr;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    if ((ret = request_firmware(&config_fw, filename, siwifi_hw->dev))) {
        printk(KERN_CRIT "%s: Failed to get %s (%d)\n", __func__, filename, ret);
        return ret;
    }

    /* Get Trident path mapping */
    tag_ptr = siwifi_find_tag(config_fw->data, config_fw->size,
                            "TRD_PATH_MAPPING=", strlen("00"));
    if (tag_ptr != NULL) {
        u8 val;
        if (sscanf(tag_ptr, "%hhx", &val) == 1)
            config->trd.path_mapping = val;
        else
            config->trd.path_mapping = path;
    } else
        config->trd.path_mapping = path;

    SIWIFI_DBG("Trident path mapping is: %d\n", config->trd.path_mapping);

    /* Get DC offset compensation */
    tag_ptr = siwifi_find_tag(config_fw->data, config_fw->size,
                            "TX_DC_OFF_COMP=", strlen("00000000"));
    if (tag_ptr != NULL) {
        if (sscanf(tag_ptr, "%08x", &config->trd.tx_dc_off_comp) != 1)
            config->trd.tx_dc_off_comp = 0;
    } else
        config->trd.tx_dc_off_comp = 0;

    SIWIFI_DBG("TX DC offset compensation is: %08X\n", config->trd.tx_dc_off_comp);

    /* Get aetnensis TX IQ compensation value for path0 on 2.4GHz */
    tag_ptr = siwifi_find_tag(config_fw->data, config_fw->size,
                            "AETNENSIS_TX_IQ_COMP_2_4G_PATH_0=", strlen("00000000"));
    if (tag_ptr != NULL) {
        if (sscanf(tag_ptr, "%08x", &config->aetnensis.tx_iq_comp_2_4G[0]) != 1)
            config->aetnensis.tx_iq_comp_2_4G[0] = 0x01000000;
    } else
        config->aetnensis.tx_iq_comp_2_4G[0] = 0x01000000;

    SIWIFI_DBG("aetnensis TX IQ compensation for path 0 on 2.4GHz is: %08X\n", config->aetnensis.tx_iq_comp_2_4G[0]);

    /* Get aetnensis TX IQ compensation value for path1 on 2.4GHz */
    tag_ptr = siwifi_find_tag(config_fw->data, config_fw->size,
                            "AETNENSIS_TX_IQ_COMP_2_4G_PATH_1=", strlen("00000000"));
    if (tag_ptr != NULL) {
        if (sscanf(tag_ptr, "%08x", &config->aetnensis.tx_iq_comp_2_4G[1]) != 1)
            config->aetnensis.tx_iq_comp_2_4G[1] = 0x01000000;
    } else
        config->aetnensis.tx_iq_comp_2_4G[1] = 0x01000000;

    SIWIFI_DBG("aetnensis TX IQ compensation for path 1 on 2.4GHz is: %08X\n", config->aetnensis.tx_iq_comp_2_4G[1]);

    /* Get aetnensis RX IQ compensation value for path0 on 2.4GHz */
    tag_ptr = siwifi_find_tag(config_fw->data, config_fw->size,
                            "AETNENSIS_RX_IQ_COMP_2_4G_PATH_0=", strlen("00000000"));
    if (tag_ptr != NULL) {
        if (sscanf(tag_ptr, "%08x", &config->aetnensis.rx_iq_comp_2_4G[0]) != 1)
            config->aetnensis.rx_iq_comp_2_4G[0] = 0x01000000;
    } else
        config->aetnensis.rx_iq_comp_2_4G[0] = 0x01000000;

    SIWIFI_DBG("aetnensis RX IQ compensation for path 0 on 2.4GHz is: %08X\n", config->aetnensis.rx_iq_comp_2_4G[0]);

    /* Get aetnensis RX IQ compensation value for path1 on 2.4GHz */
    tag_ptr = siwifi_find_tag(config_fw->data, config_fw->size,
                            "AETNENSIS_RX_IQ_COMP_2_4G_PATH_1=", strlen("00000000"));
    if (tag_ptr != NULL) {
        if (sscanf(tag_ptr, "%08x", &config->aetnensis.rx_iq_comp_2_4G[1]) != 1)
            config->aetnensis.rx_iq_comp_2_4G[1] = 0x01000000;
    } else
        config->aetnensis.rx_iq_comp_2_4G[1] = 0x01000000;

    SIWIFI_DBG("aetnensis RX IQ compensation for path 1 on 2.4GHz is: %08X\n", config->aetnensis.rx_iq_comp_2_4G[1]);

    /* Get aetnensis TX IQ compensation value for path0 on 5GHz */
    tag_ptr = siwifi_find_tag(config_fw->data, config_fw->size,
                            "AETNENSIS_TX_IQ_COMP_5G_PATH_0=", strlen("00000000"));
    if (tag_ptr != NULL) {
        if (sscanf(tag_ptr, "%08x", &config->aetnensis.tx_iq_comp_5G[0]) != 1)
            config->aetnensis.tx_iq_comp_5G[0] = 0x01000000;
    } else
        config->aetnensis.tx_iq_comp_5G[0] = 0x01000000;

    SIWIFI_DBG("aetnensis TX IQ compensation for path 0 on 5GHz is: %08X\n", config->aetnensis.tx_iq_comp_5G[0]);

    /* Get aetnensis TX IQ compensation value for path1 on 5GHz */
    tag_ptr = siwifi_find_tag(config_fw->data, config_fw->size,
                            "AETNENSIS_TX_IQ_COMP_5G_PATH_1=", strlen("00000000"));
    if (tag_ptr != NULL) {
        if (sscanf(tag_ptr, "%08x", &config->aetnensis.tx_iq_comp_5G[1]) != 1)
            config->aetnensis.tx_iq_comp_5G[1] = 0x01000000;
    } else
        config->aetnensis.tx_iq_comp_5G[1] = 0x01000000;

    SIWIFI_DBG("aetnensis TX IQ compensation for path 1 on 5GHz is: %08X\n", config->aetnensis.tx_iq_comp_5G[1]);

    /* Get aetnensis RX IQ compensation value for path0 on 5GHz */
    tag_ptr = siwifi_find_tag(config_fw->data, config_fw->size,
                            "AETNENSIS_RX_IQ_COMP_5G_PATH_0=", strlen("00000000"));
    if (tag_ptr != NULL) {
        if (sscanf(tag_ptr, "%08x", &config->aetnensis.rx_iq_comp_5G[0]) != 1)
            config->aetnensis.rx_iq_comp_5G[0] = 0x01000000;
    } else
        config->aetnensis.rx_iq_comp_5G[0] = 0x01000000;

    SIWIFI_DBG("aetnensis RX IQ compensation for path 0 on 5GHz is: %08X\n", config->aetnensis.rx_iq_comp_5G[0]);

    /* Get aetnensis RX IQ compensation value for path1 on 5GHz */
    tag_ptr = siwifi_find_tag(config_fw->data, config_fw->size,
                            "AETNENSIS_RX_IQ_COMP_5G_PATH_1=", strlen("00000000"));
    if (tag_ptr != NULL) {
        if (sscanf(tag_ptr, "%08x", &config->aetnensis.rx_iq_comp_5G[1]) != 1)
            config->aetnensis.rx_iq_comp_5G[1] = 0x01000000;
    } else
        config->aetnensis.rx_iq_comp_5G[1] = 0x01000000;

    SIWIFI_DBG("aetnensis RX IQ compensation for path 1 on 5GHz is: %08X\n", config->aetnensis.rx_iq_comp_5G[1]);

    /* Get aetnensis default path */
    tag_ptr = siwifi_find_tag(config_fw->data, config_fw->size,
                            "AETNENSIS_DEFAULT_PATH=", strlen("00"));
    if (tag_ptr != NULL) {
        u8 val;
        if (sscanf(tag_ptr, "%hhx", &val) == 1)
            config->aetnensis.path_used = val;
        else
            config->aetnensis.path_used = path;
    } else
        config->aetnensis.path_used = path;

    SIWIFI_DBG("aetnensis default path is: %d\n", config->aetnensis.path_used);

    /* Release the configuration file */
    release_firmware(config_fw);
#endif

    return 0;
}

#if defined(CONFIG_SIWIFI_TEMPERATURE_CONTROL)
void readline_temperature_control_configfile(struct siwifi_hw *siwifi_hw, const struct firmware *config_fw,
                                                const char *tag_name, int i){
    const u8 *tag_ptr;
    int tmp_value;

    tag_ptr = siwifi_find_tag(config_fw->data, config_fw->size, tag_name, strlen("000"));
    if (tag_ptr != NULL) {
        if (sscanf(tag_ptr, "%d", &tmp_value) != 1){
            siwifi_hw->temp_ctl.temp_ctl_parameters[i] = TEMP_CTL_DEFAULT_TRIGGER_TEMP;
        } else {
            siwifi_hw->temp_ctl.temp_ctl_parameters[i] = tmp_value;
        }
    } else {
        if (i < 5)
            siwifi_hw->temp_ctl.temp_ctl_parameters[i] = TEMP_CTL_DEFAULT_TRIGGER_TEMP;
        else
            siwifi_hw->temp_ctl.temp_ctl_parameters[i] = 0;
    }

    switch(i){
        case 0:
            if(siwifi_hw->temp_ctl.temp_ctl_parameters[i] != 1)
                siwifi_hw->temp_ctl.temp_ctl_parameters[i] = 0;
            break;
        case 1:
            if(siwifi_hw->temp_ctl.temp_ctl_parameters[i] < TEMP_CTL_DEFAULT_TRIGGER_TEMP)
                siwifi_hw->temp_ctl.temp_ctl_parameters[i] = TEMP_CTL_DEFAULT_TRIGGER_TEMP;
            break;
        case 2:
        case 3:
        case 4:
            if(siwifi_hw->temp_ctl.temp_ctl_parameters[i] < siwifi_hw->temp_ctl.temp_ctl_parameters[i-1])
                siwifi_hw->temp_ctl.temp_ctl_parameters[i] = siwifi_hw->temp_ctl.temp_ctl_parameters[i-1] + 1;
            break;
        case 5:
            if(siwifi_hw->temp_ctl.temp_ctl_parameters[i] < 0 || siwifi_hw->temp_ctl.temp_ctl_parameters[i] > 100)
                siwifi_hw->temp_ctl.temp_ctl_parameters[i] = 0;
            break;
        default:
            if(siwifi_hw->temp_ctl.temp_ctl_parameters[i] < siwifi_hw->temp_ctl.temp_ctl_parameters[i-1])
                siwifi_hw->temp_ctl.temp_ctl_parameters[i] = siwifi_hw->temp_ctl.temp_ctl_parameters[i-1] + 1;
            if(siwifi_hw->temp_ctl.temp_ctl_parameters[i] > 100)
                siwifi_hw->temp_ctl.temp_ctl_parameters[i] = 100;
            break;
    }
}

int siwifi_parse_temperature_control_configfile(struct siwifi_hw *siwifi_hw, const char *filename){
    const struct firmware *config_fw;
    int ret;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    if ((ret = request_firmware(&config_fw, filename, siwifi_hw->dev))) {
        printk(KERN_CRIT "%s: Failed to get %s (%d)\n", __func__, filename, ret);
        return ret;
    }

    if(siwifi_hw->mod_params->is_hb){
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_HB_ENABLE=",0);
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_HB_TRIGGER_TEMP_LV1=",1);
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_HB_TRIGGER_TEMP_LV2=",2);
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_HB_TRIGGER_TEMP_LV3=",3);
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_HB_TRIGGER_TEMP_LV4=",4);
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_HB_TRIGGER_LIMIT_LV1_PERCENT=",5);
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_HB_TRIGGER_LIMIT_LV2_PERCENT=",6);
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_HB_TRIGGER_LIMIT_LV3_PERCENT=",7);
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_HB_TRIGGER_LIMIT_LV4_PERCENT=",8);
    } else {
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_LB_ENABLE=",0);
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_LB_TRIGGER_TEMP_LV1=",1);
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_LB_TRIGGER_TEMP_LV2=",2);
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_LB_TRIGGER_TEMP_LV3=",3);
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_LB_TRIGGER_TEMP_LV4=",4);
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_LB_TRIGGER_LIMIT_LV1_PERCENT=",5);
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_LB_TRIGGER_LIMIT_LV2_PERCENT=",6);
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_LB_TRIGGER_LIMIT_LV3_PERCENT=",7);
        readline_temperature_control_configfile(siwifi_hw,config_fw,"TEMPCTL_LB_TRIGGER_LIMIT_LV4_PERCENT=",8);
    }

	/* Release the configuration file */
	    release_firmware(config_fw);

	return 0;
}
#endif

#ifdef CONFIG_ENABLE_DIGGAINTABLE
#define DEFAULT_VALUE_DIGTABLE_CONFIGFILE 0x30303030

void readline_digtable_configfile(struct siwifi_hw *siwifi_hw, const struct firmware *config_fw,
								const char *tag_name, int i,uint32_t  max){
	unsigned int digtable[2];
    const u8 *tag_ptr;
    tag_ptr = siwifi_find_tag(config_fw->data, config_fw->size, tag_name, strlen("00000000"));
    if (tag_ptr != NULL) {
        if (sscanf(tag_ptr,"%08x",digtable) != 1){
			if(max){
			siwifi_hw->phy_config.digtable_max[i] = DEFAULT_VALUE_DIGTABLE_CONFIGFILE;
			}else
			siwifi_hw->phy_config.digtable[i] = DEFAULT_VALUE_DIGTABLE_CONFIGFILE;
		}
		else{
			if(max){
			memcpy(&siwifi_hw->phy_config.digtable_max[i],digtable,sizeof(digtable[0]));
			}else
			memcpy(&siwifi_hw->phy_config.digtable[i],digtable,sizeof(digtable[0]));
		}
	} else{
			if(max){
			siwifi_hw->phy_config.digtable_max[i] = DEFAULT_VALUE_DIGTABLE_CONFIGFILE;
			}else
			siwifi_hw->phy_config.digtable[i] = DEFAULT_VALUE_DIGTABLE_CONFIGFILE;
	}
//    SIWIFI_DBG("siwifi_hw->phy_config.digtable[%d]:%x\n", i, siwifi_hw->phy_config.digtable[i]);
	if(!max){
	    printk("siwifi_hw->phy_config.digtable[%d]:%x\n", i, siwifi_hw->phy_config.digtable[i]);
	}
    else
		printk("siwifi_hw->phy_config.digtable_max[%d]:%x\n", i, siwifi_hw->phy_config.digtable_max[i]);
}

/**
 * Parse the Config file used at init time
 */
int siwifi_parse_digtable_configfile(struct siwifi_hw *siwifi_hw, const char *filename){
	const struct firmware *config_fw;
    int ret;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    if ((ret = request_firmware(&config_fw, filename, siwifi_hw->dev))) {
        printk(KERN_CRIT "%s: Failed to get %s (%d)\n", __func__, filename, ret);
        return ret;
    }
    if(siwifi_hw->mod_params->is_hb){
		readline_digtable_configfile(siwifi_hw, config_fw, "HBTX20DIGGAINTABLE0=", 0,0);
		readline_digtable_configfile(siwifi_hw, config_fw, "HBTX20DIGGAINTABLE1=", 1,0);
		readline_digtable_configfile(siwifi_hw, config_fw, "HBTX40DIGGAINTABLE0=", 2,0);
		readline_digtable_configfile(siwifi_hw, config_fw, "HBTX40DIGGAINTABLE1=", 3,0);
		readline_digtable_configfile(siwifi_hw, config_fw, "HBTX80DIGGAINTABLE0=", 4,0);
		readline_digtable_configfile(siwifi_hw, config_fw, "HBTX80DIGGAINTABLE1=", 5,0);
		readline_digtable_configfile(siwifi_hw, config_fw, "HBTX20DIGGAINTABLE0_MAX=", 0,1);
		readline_digtable_configfile(siwifi_hw, config_fw, "HBTX20DIGGAINTABLE1_MAX=", 1,1);
		readline_digtable_configfile(siwifi_hw, config_fw, "HBTX40DIGGAINTABLE0_MAX=", 2,1);
		readline_digtable_configfile(siwifi_hw, config_fw, "HBTX40DIGGAINTABLE1_MAX=", 3,1);
		readline_digtable_configfile(siwifi_hw, config_fw, "HBTX80DIGGAINTABLE0_MAX=", 4,1);
		readline_digtable_configfile(siwifi_hw, config_fw, "HBTX80DIGGAINTABLE1_MAX=", 5,1);
	}
	else{
		readline_digtable_configfile(siwifi_hw, config_fw, "LBTX20DIGGAINTABLE0=", 0,0);
		readline_digtable_configfile(siwifi_hw, config_fw, "LBTX20DIGGAINTABLE1=", 1,0);
		readline_digtable_configfile(siwifi_hw, config_fw, "LBTX40DIGGAINTABLE0=", 2,0);
		readline_digtable_configfile(siwifi_hw, config_fw, "LBTX40DIGGAINTABLE1=", 3,0);
		readline_digtable_configfile(siwifi_hw, config_fw, "LBTX80DIGGAINTABLE0=", 4,0);
		readline_digtable_configfile(siwifi_hw, config_fw, "LBTX80DIGGAINTABLE1=", 5,0);
		readline_digtable_configfile(siwifi_hw, config_fw, "LBTX20DIGGAINTABLE0_MAX=", 0,1);
		readline_digtable_configfile(siwifi_hw, config_fw, "LBTX20DIGGAINTABLE1_MAX=", 1,1);
		readline_digtable_configfile(siwifi_hw, config_fw, "LBTX40DIGGAINTABLE0_MAX=", 2,1);
		readline_digtable_configfile(siwifi_hw, config_fw, "LBTX40DIGGAINTABLE1_MAX=", 3,1);
		readline_digtable_configfile(siwifi_hw, config_fw, "LBTX80DIGGAINTABLE0_MAX=", 4,1);
		readline_digtable_configfile(siwifi_hw, config_fw, "LBTX80DIGGAINTABLE1_MAX=", 5,1);
	}
	readline_digtable_configfile(siwifi_hw, config_fw, "ENABLE_DIGGAINTABLE_MAX=", 6,1);
	readline_digtable_configfile(siwifi_hw, config_fw, "ENABLE_DIGGAINTABLE=", 6,0);

	SIWIFI_DBG("==========%x %x %x %x %x %x %x\n",siwifi_hw->phy_config.digtable[0],siwifi_hw->phy_config.digtable[1],siwifi_hw->phy_config.digtable[2],siwifi_hw->phy_config.digtable[3],siwifi_hw->phy_config.digtable[4],siwifi_hw->phy_config.digtable[5],siwifi_hw->phy_config.digtable[6]);
	SIWIFI_DBG("--------%x %x %x %x %x %x %x\n",siwifi_hw->phy_config.digtable_max[0],siwifi_hw->phy_config.digtable_max[1],siwifi_hw->phy_config.digtable_max[2],siwifi_hw->phy_config.digtable_max[3],siwifi_hw->phy_config.digtable_max[4],siwifi_hw->phy_config.digtable_max[5],siwifi_hw->phy_config.digtable_max[6]);
	/* Release the configuration file */
	   release_firmware(config_fw);

	return 0;

}
#endif

#ifdef CONFIG_ENABLE_TXPOWERTABLE

#define DEFAULT_VALUE_TXPOWER_GAIN_TABLE 0

/**
 * Parse the TXPower to gain Config file used at init time
 */
int siwifi_parse_txpower_gain_table_configfile(struct siwifi_hw *siwifi_hw, int ext_pa){
	const struct firmware *config_fw;
	unsigned int digtable[2];
	const u8 *tag_ptr;
	char tag_name[16];
	const char *filename;
	char filename_path[64];
	struct file *fp = NULL;
    int ret;
	int i;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    if (siwifi_hw->mod_params->is_hb) {
		filename = ext_pa ? SIWIFI_HB_TXPOWER_GAIN_EXPA_TABLE_NAME : SIWIFI_HB_TXPOWER_GAIN_TABLE_NAME;

        sprintf(filename_path, "%s%s", "/lib/firmware/", filename);

		fp = filp_open(filename_path, O_RDONLY , 0);
		if(IS_ERR(fp)) {
			printk(KERN_CRIT "%s: %s not exsist, use default file.\n", __func__, filename);
			filename = ext_pa ? SIWIFI_HB_TXPOWER_GAIN_EXPA_DEFAULT_TABLE_NAME : SIWIFI_HB_TXPOWER_GAIN_DEFAULT_TABLE_NAME;
			printk("[POWER_TABLE_NAME] %s %d %s\n", __func__, __LINE__, filename);
		}else {
			filp_close(fp, NULL);
		}
	} else {
		filename = ext_pa ? SIWIFI_LB_TXPOWER_GAIN_EXPA_TABLE_NAME : SIWIFI_LB_TXPOWER_GAIN_TABLE_NAME;
        sprintf(filename_path, "%s%s", "/lib/firmware/", filename);

		fp = filp_open(filename_path, O_RDONLY , 0);
		if(IS_ERR(fp)) {
			printk(KERN_CRIT "%s: %s not exsist, use default file.\n", __func__, filename);
			filename = ext_pa ? SIWIFI_LB_TXPOWER_GAIN_EXPA_DEFAULT_TABLE_NAME : SIWIFI_LB_TXPOWER_GAIN_DEFAULT_TABLE_NAME;
			printk("[POWER_TABLE_NAME] %s %d %s\n", __func__, __LINE__, filename);
		}else {
			filp_close(fp, NULL);
		}
	}
#ifdef CONFIG_SIWIFI_DBG
	printk(" %s parse file: %s \n", __func__, filename);
#endif

    if ((ret = request_firmware(&config_fw, filename, siwifi_hw->dev))) {
        printk(KERN_CRIT "%s: Failed to get %s (%d)\n", __func__, filename, ret);
        return ret;
    }
	for (i = 0; i < 31; i++) {
        if(siwifi_hw->mod_params->is_hb){
			sprintf(tag_name, "HBTXPOWER%d=", i);
		} else {
			sprintf(tag_name, "LBTXPOWER%d=", i);
		}
		tag_ptr = siwifi_find_tag(config_fw->data, config_fw->size, tag_name, strlen("00"));
		if (tag_ptr != NULL) {
			if (sscanf(tag_ptr,"%d",digtable) != 1){
				if(siwifi_hw->mod_params->is_hb){
					siwifi_hw->phy_config.hb_power_gain_tb[i] = DEFAULT_VALUE_TXPOWER_GAIN_TABLE;
				}else
					siwifi_hw->phy_config.lb_power_gain_tb[i] = DEFAULT_VALUE_TXPOWER_GAIN_TABLE;
			}
			else{
				if(siwifi_hw->mod_params->is_hb){
					memcpy(&siwifi_hw->phy_config.hb_power_gain_tb[i],digtable,sizeof(digtable[0]));
				}else
					memcpy(&siwifi_hw->phy_config.lb_power_gain_tb[i],digtable,sizeof(digtable[0]));
			}
		} else{
			if(siwifi_hw->mod_params->is_hb){
				siwifi_hw->phy_config.hb_power_gain_tb[i] = DEFAULT_VALUE_TXPOWER_GAIN_TABLE;
			}else
				siwifi_hw->phy_config.lb_power_gain_tb[i] = DEFAULT_VALUE_TXPOWER_GAIN_TABLE;
		}
#ifdef CONFIG_SIWIFI_DBG
		if(siwifi_hw->mod_params->is_hb){
			printk("file HB TX power %d dbm gain is %d \n", i, siwifi_hw->phy_config.hb_power_gain_tb[i]);
		} else {
			printk("file LB TX power %d dbm gain is %d \n", i, siwifi_hw->phy_config.lb_power_gain_tb[i]);
		}
#endif
	}

	/* Release the configuration file */
	   release_firmware(config_fw);

	return 0;
}
#endif

// (channel) 13 * (one channel gain num) 28
#define LB_OFFSET_LENGTH 364
// (channel) 25 * (one channel gain num) 53
#define HB_OFFSET_LENGTH 1325

int siwifi_update_txpower_offset_configfile(char *char_txpower_list, int txpower_mode, int lb, struct siwifi_hw *siwifi_hw, int second_antenna){
	const struct firmware *config_fw;
    int ret, i, offset;
	char *gain_file, gain_num, gain_num_next;
	u8 *txpower_list = (u8 *)char_txpower_list;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

	if (second_antenna){
		if ((ret = request_firmware(&config_fw, SIWIFI_TXPOWER_SLEEPMODE_LOW_SECOND_NAME, siwifi_hw->dev))) {
			printk(KERN_CRIT "%s: Failed to get %s (%d)\n", __func__, SIWIFI_TXPOWER_SLEEPMODE_LOW_SECOND_NAME, ret);
			return ret;
		}
	} else{
		if ((ret = request_firmware(&config_fw, SIWIFI_TXPOWER_SLEEPMODE_LOW_NAME, siwifi_hw->dev))) {
			printk(KERN_CRIT "%s: Failed to get %s (%d)\n", __func__, SIWIFI_TXPOWER_SLEEPMODE_LOW_NAME, ret);
			return ret;
		}
	}

	gain_file = (char *)config_fw->data;

	if(txpower_mode){
		for( ; *gain_file != '='; ){
			gain_file = gain_file + 1;
		}
	}

	if(lb){
		for( i = 0; i < LB_OFFSET_LENGTH ; i++){
			for( ; *gain_file != '-'; ){
				gain_file = gain_file + 1;
			}
			gain_file = gain_file + 1;
			gain_num = *gain_file;
			gain_num_next = *(gain_file + 1);
			if(gain_num_next >= '0' && gain_num_next <= '9'){
				//0x30 is tranform char(read as ASCII by request_firmware) to int
				offset = (gain_num - 0x30) * 10 + (gain_num_next - 0x30);
			} else {
				offset = gain_num - 0x30;
			}
			if(offset < (*(txpower_list + i) & 0x1f)){
				*(txpower_list + i) = *(txpower_list + i) - offset;
			}
		}
	}
	else{
		for( i = 0; i < LB_OFFSET_LENGTH ; i++){
			for( ; *gain_file != '-'; ){
				gain_file = gain_file + 1;
			}
			gain_file = gain_file + 1;
		}
		for( i = 0; i < HB_OFFSET_LENGTH ; i++){
			for( ; *gain_file != '-'; ){
				gain_file = gain_file + 1;
			}
			gain_file = gain_file + 1;
			gain_num = *gain_file;
			gain_num_next = *(gain_file + 1);
			if(gain_num_next >= '0' && gain_num_next <= '9'){
				offset = (gain_num - 0x30) * 10 + (gain_num_next - 0x30);
			} else {
				offset = gain_num - 0x30;
			}
			if(offset < (*(txpower_list + i) & 0x1f)){
				*(txpower_list + i) = *(txpower_list + i) - offset;
			}
		}
	}

	/* Release the configuration file */
	release_firmware(config_fw);

	return 0;

}

//update txpower_lvl then send it to lmac
int siwifi_update_txpower_lvl(struct siwifi_hw *siwifi_hw, int txpower_lvl){

	if(txpower_lvl < 0 || txpower_lvl > 2){
		txpower_lvl = 2;
	}

	siwifi_hw->mod_params->txpower_lvl = txpower_lvl;
	siwifi_send_set_power_lvl(siwifi_hw, (u8)txpower_lvl);

	return 0;
}

int siwifi_update_antenna_number(struct siwifi_hw *siwifi_hw, int antenna_number){

	if(antenna_number < 1 || antenna_number > 2){
		antenna_number = 2;
	}

	siwifi_send_set_antenna_number(siwifi_hw, (u8)antenna_number);

	return 0;
}
