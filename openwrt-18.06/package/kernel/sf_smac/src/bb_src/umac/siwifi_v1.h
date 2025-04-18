/**
 ****************************************************************************************
 *
 * @file siwifi_v1.h
 *
 * Copyright (C) Siflower 2018-2025
 *
 ****************************************************************************************
 */

#ifndef _SIWIFI_V1_H_
#define _SIWIFI_V1_H_
#include <linux/platform_device.h>

#include "siwifi_defs.h"
#include "siwifi_factory_info.h"

#define SIWIFI_CONFIG_FW_NAME             "siwifi_settings.ini"
#define SIWIFI_PHY_CONFIG_TRD_NAME        "siwifi_trident.ini"
#define SIWIFI_PHY_CONFIG_AETNENSIS_NAME      "siwifi_aetnensis.ini"
#define SIWIFI_AGC_FW_NAME                "agcram.bin"
#define SIWIFI_AGC_FW_NAME_24G "agcram_24g.bin"
#define SIWIFI_LDPC_FW_NAME               "ldpcram.bin"
#define SIWIFI_DIG_GAIN_TABLE_NAME        "dig_gaintable.ini"
#define SIWIFI_DIG_GAIN_TABLE_EXPA_NAME   "dig_gaintable_expa.ini"
#define SIWIFI_LB_TXPOWER_GAIN_DEFAULT_TABLE_NAME   "default_lb_txpower_table.ini"
#define SIWIFI_HB_TXPOWER_GAIN_DEFAULT_TABLE_NAME   "default_hb_txpower_table.ini"
#ifdef CONFIG_UPDATE_GAIN_TABLE
#define SIWIFI_LB_TXPOWER_GAIN_EXPA_DEFAULT_TABLE_NAME   "default_lb_txpower_table_expa.ini"
#define SIWIFI_HB_TXPOWER_GAIN_EXPA_DEFAULT_TABLE_NAME   "default_hb_txpower_table_expa_low_power.ini"
#else
#define SIWIFI_LB_TXPOWER_GAIN_EXPA_DEFAULT_TABLE_NAME   "default_lb_txpower_table_expa.ini"
#define SIWIFI_HB_TXPOWER_GAIN_EXPA_DEFAULT_TABLE_NAME   "default_hb_txpower_table_expa.ini"
#endif 

#define SIWIFI_LB_TXPOWER_GAIN_TABLE_NAME   "lb_txpower_table.ini"
#define SIWIFI_HB_TXPOWER_GAIN_TABLE_NAME   "hb_txpower_table.ini"
#define SIWIFI_LB_TXPOWER_GAIN_EXPA_TABLE_NAME   "lb_txpower_table_expa.ini"
#define SIWIFI_HB_TXPOWER_GAIN_EXPA_TABLE_NAME   "hb_txpower_table_expa.ini"
#define SIWIFI_RF_GAIN_TABLE_NAME           "rf_gain_table.ini"
#define SIWIFI_TEMPERATURE_CONTROL_NAME        "temperature_control.ini"

//CLK TYPE
enum {
    PLATFORM_CLK = 1 << 1,
    LA_CLK = 1 << 2,
    IQ_TE_CLK = 1 << 3,
};

/**
 * @pci_dev pointer to pci dev
 * @enabled Set if embedded platform has been enabled (i.e. fw loaded and
 *          ipc started)
 * @enable Configure communication with the fw (i.e. configure the transfers
 *         enable and register interrupt)
 * @disable Stop communication with the fw
 * @deinit Free all ressources allocated for the embedded platform
 * @get_address Return the virtual address to access the requested address on
 *              the platform.
 * @ack_irq Acknowledge the irq at link level.
 *
 * @priv Private data for the link driver
 */
struct siwifi_plat {
    //todo: ycchang sync with siwifi_platform.h
#if 1
    struct platform_device *pdev;
#else
    struct pci_dev *pci_dev;
#endif
    bool enabled;

    int (*enable)(struct siwifi_hw *siwifi_hw);
    int (*disable)(struct siwifi_hw *siwifi_hw);
    void (*deinit)(struct siwifi_plat *siwifi_plat);
    u8* (*get_address)(struct siwifi_plat *siwifi_plat, int addr_name,
                       unsigned int offset);
    void (*ack_irq)(struct siwifi_plat *siwifi_plat);

    u8 priv[0] __aligned(sizeof(void *));
};

int siwifi_platform_on(struct siwifi_hw *siwifi_hw);
void siwifi_platform_off(struct siwifi_hw *siwifi_hw);
int siwifi_platform_restart(struct siwifi_hw *siwifi_hw);
//get/set the request clk's frequency, uinit 1MHz
int32_t siwifi_platform_get_clkfreq(struct siwifi_hw *siwifi_hw, int32_t type);
int32_t siwifi_platform_set_clkfreq(struct siwifi_hw *siwifi_hw, int32_t type, int32_t freq);

int siwifi_platform_register_drv(void);
void siwifi_platform_unregister_drv(void);

int siwifi_register_hp_drv(void);
void siwifi_unregister_hp_drv(void);

struct device *siwifi_platform_get_dev(struct siwifi_plat *siwifi_plat);
#endif /* _SIWIFI_V1_H_ */
