/**
 ****************************************************************************************
 *
 * @file siwifi_cfgfile.h
 *
 * Copyright (C) Siflower 2018-2025
 *
 ****************************************************************************************
 */

#ifndef _SIWIFI_CFGFILE_H_
#define _SIWIFI_CFGFILE_H_

/*
 * Structure used to retrieve information from the Config file used at Initialization time
 */
struct siwifi_conf_file {
    u8 mac_addr[ETH_ALEN];
};

/*
 * Structure used to retrieve information from the PHY Config file used at Initialization time
 */
struct siwifi_phy_conf_file {
    struct phy_trd_cfg_tag trd;
    struct phy_aetnensis_cfg_tag aetnensis;
};
#ifdef CONFIG_SIWIFI_CFGFILE

int siwifi_parse_configfile(struct siwifi_hw *siwifi_hw, const char *filename,
                          struct siwifi_conf_file *config);

int siwifi_parse_phy_configfile(struct siwifi_hw *siwifi_hw, const char *filename,
                              struct siwifi_phy_conf_file *config, int path);

#ifdef CONFIG_SIWIFI_TEMPERATURE_CONTROL
int siwifi_parse_temperature_control_configfile(struct siwifi_hw *siwifi_hw, const char *filename);
#endif
#ifdef CONFIG_ENABLE_DIGGAINTABLE
int siwifi_parse_digtable_configfile(struct siwifi_hw *siwifi_hw, const char *filename);
#endif

#ifdef CONFIG_ENABLE_TXPOWERTABLE
int siwifi_parse_txpower_gain_table_configfile(struct siwifi_hw *siwifi_hw, int ext_pa);
#endif

int siwifi_update_txpower_offset_configfile(char *txpower_list, int txpower_mode, int lb, struct siwifi_hw *siwifi_hw, int second_antenna);
int siwifi_update_txpower_lvl(struct siwifi_hw *siwifi_hw, int txpower_lvl);
int siwifi_update_antenna_number(struct siwifi_hw *siwifi_hw, int antenna_number);

#else
static inline int siwifi_parse_configfile(struct siwifi_hw *siwifi_hw, const char *filename,
                          struct siwifi_conf_file *config) { return 0; }

static inline int siwifi_parse_phy_configfile(struct siwifi_hw *siwifi_hw, const char *filename,
                              struct siwifi_phy_conf_file *config, int path) { return 0; }
#endif

#endif /* _SIWIFI_CFGFILE_H_ */
