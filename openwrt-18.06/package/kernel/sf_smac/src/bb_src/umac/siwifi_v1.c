/**
 ****************************************************************************************
 *
 * @file siwifi_v1.c
 *
 * Copyright (C) Siflower 2018-2025
 *
 ****************************************************************************************
 */
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/kthread.h>
#include <linux/interrupt.h>
#include <linux/firmware.h>
#include <linux/of_net.h>
#include <linux/mtd/mtd.h>
#include <linux/clk.h>
#include <sf19a28.h>
#include <linux/of.h>
#include <linux/of_device.h>
#ifdef CONFIG_SFA28_FULLMASK
#include <sf19a28.h>
#endif
#ifdef CONFIG_SFAX8_FACTORY_READ
#include <sfax8_factory_read.h>
#endif
#ifdef CONFIG_SF16A18_WIFI_RF
#include "rf_pl_ref.h"
#endif
#if (defined CONFIG_SIWIFI_RF_RECALI) || (defined CONFIG_SIWIFI_RF_CMD_DUMP)
#include "siwifi_msg_tx.h"
#endif

#include "reg_mdm_riu.h"
#include "siwifi_v1.h"
#include "siwifi_prof.h"
#include "siwifi_tx.h"
#include "siwifi_main.h"
#include "ipc_host.h"
#include "siwifi_lmac_glue.h"
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
#include "siwifi_ioctl.h"
#endif

#ifdef CONFIG_SIWIFI_TX_POWER_CALI
#include "siwifi_factory_info.h"
#endif

#include "siwifi_mem.h"
#include "siwifi_cfgfile.h"
#include "siwifi_traffic.h"

struct siwifi_reprobe {
    struct device *dev;
    struct work_struct work;
};

#define SIWIFI_RESET_WIFI_USE_UM 1

#ifdef SIWIFI_RESET_WIFI_USE_UM

int siwifi_call_um_wifi_reload(void)
{
    char *envp[] = {"PATH=/sbin:/usr/sbin:/bin:/usr/bin", NULL};
    char **argv;
    int argc, ret;
    char *cmd_buf;
    int cmd_len;
    const char *cmd = "/sbin/wifi";

    cmd_len = strlen(cmd);
    cmd_buf = (char *)siwifi_kmalloc(cmd_len + 8, GFP_ATOMIC);
    if (!cmd_buf)
        return 0;
    sprintf(cmd_buf, "%s up\n", cmd);
    argv = argv_split(in_interrupt() ? GFP_ATOMIC : GFP_KERNEL, cmd_buf, &argc);
    if (!argc) {
        siwifi_kfree(cmd_buf);
        return -EINVAL;
    }

    if ((ret = call_usermodehelper(argv[0], argv, envp,
                    UMH_WAIT_PROC | UMH_KILLABLE)))
        printk(KERN_CRIT "Failed to call %s (%s returned %d)\n",
                argv[0], cmd, ret);
    argv_free(argv);
    siwifi_kfree(cmd_buf);

    return ret;
}

#endif

void siwifi_reprobe_work(struct work_struct *wk)
{
    struct siwifi_reprobe *reprobe;
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    reprobe = container_of(wk, struct siwifi_reprobe, work);
    if (device_reprobe(reprobe->dev))
        dev_err(reprobe->dev, "reprobe failed!\n");
    siwifi_kfree(reprobe);
    module_put(THIS_MODULE);
#ifdef SIWIFI_RESET_WIFI_USE_UM
    //now we depend on umhelper script to bring wifi up after reprobe
    //TODO: remove it latter case we get a proper methord to re-build the chanel-ctx after probe
    siwifi_call_um_wifi_reload();
#endif
    SIWIFI_DBG(SIWIFI_FN_EXIT_STR);
}

void siwifi_reprobe_driver(struct siwifi_hw *siwifi_hw)
{
    struct siwifi_reprobe *reprobe;
    printk("siwifi_reprobe_driver\n");

    //now we are going to restart the driver,set bit to bypassing msgs from mac80211
    set_bit(SIWIFI_DEV_HW_DEAD, &siwifi_hw->drv_flags);
    /*
     * get a module reference to avoid doing this while unloading
     * anyway and to avoid scheduling a work with code that's
     * being removed.
     */
    if (!try_module_get(THIS_MODULE)) {
        printk(KERN_CRIT "%s: Module is being unloaded - abort\n", __func__);
        return;
    }
    reprobe = siwifi_kzalloc(sizeof(*reprobe), GFP_ATOMIC);
    if (!reprobe) {
        module_put(THIS_MODULE);
        return;
    }
    reprobe->dev = siwifi_hw->dev;
    INIT_WORK(&reprobe->work, siwifi_reprobe_work);
    schedule_work(&reprobe->work);
}

#ifdef CONFIG_SF16A18_WIFI_RF
/* external function declare */
extern int sf_wifi_rf_bb_register(int8_t band_type, int (*callback)(void *data, uint32_t event, uint32_t flags, void *parameter), void *data);
extern int sf_wifi_rf_bb_unregister(int8_t band_type);
extern int sf_wifi_rf_get_feature(int8_t band);

#define SFAX8_THERMAL_DIGGAINMASK 0xE0
#define SFAX8_THERMAL_POWERMASK 0x1F

#ifdef CFG_DUAL_ANTENNA_CALIBRATE
#define SFAX8_THERMAL_POWERMAX_DUAL 15
#endif
#define SFAX8_THERMAL_POWERMAX_SINGLE 31
#define SFAX8_THERMAL_POWERMIN_1  0

#ifdef CONFIG_SIWIFI_TEMPERATURE_CONTROL
#define TEMP_CTL_MAX_LIMIT 64
void siwifi_set_temp_ctl_level(struct siwifi_hw *siwifi_hw){
    int hwq_limit = 0;
    switch(siwifi_hw->temp_ctl.temp_ctl_level){
        case 1:
            hwq_limit = TEMP_CTL_MAX_LIMIT * siwifi_hw->temp_ctl.temp_ctl_parameters[5] / 100;
            break;
        case 2:
            hwq_limit = TEMP_CTL_MAX_LIMIT * siwifi_hw->temp_ctl.temp_ctl_parameters[6] / 100;
            break;
        case 3:
            hwq_limit = TEMP_CTL_MAX_LIMIT * siwifi_hw->temp_ctl.temp_ctl_parameters[7] / 100;
            break;
        case 4:
            hwq_limit = TEMP_CTL_MAX_LIMIT * siwifi_hw->temp_ctl.temp_ctl_parameters[8] / 100;
            break;
        case 0:
        default:
            hwq_limit = 0;
    }

    if(siwifi_hw->temp_ctl.temp_ctl_level == 4) {
        siwifi_update_antenna_number(siwifi_hw, 1);
        siwifi_update_txpower_lvl(siwifi_hw, 0);
    } else {
        siwifi_update_antenna_number(siwifi_hw, 2);
        siwifi_update_txpower_lvl(siwifi_hw, 2);
    }
    if (siwifi_hw->temp_ctl.temp_ctl_level == 0)
        printk("temperature control level %d, hwq limit percent %d%%, current temp %d\n", siwifi_hw->temp_ctl.temp_ctl_level,
                0, siwifi_hw->temp_ctl.last_change_temp);
    else
        printk("temperature control level %d, hwq limit percent %d%%, current temp %d\n", siwifi_hw->temp_ctl.temp_ctl_level,
                siwifi_hw->temp_ctl.temp_ctl_parameters[siwifi_hw->temp_ctl.temp_ctl_level + 4], siwifi_hw->temp_ctl.last_change_temp);
    spin_lock_bh(&siwifi_hw->tx_lock);
    siwifi_adjust_hwq_credits(siwifi_hw, (s8)hwq_limit);
    spin_unlock_bh(&siwifi_hw->tx_lock);
    return;
}
#endif // CONFIG_SIWIFI_TEMPERATURE_CONTROL

static int sf_wifi_rf_callback(void *data, uint32_t event, uint32_t flags, void *parameter)
{
#ifdef CONFIG_SIWIFI_RF_RECALI
    struct siwifi_hw *siwifi_hw = NULL;
    int error;
    struct v1_plat_data *priv;
    int cnt = 0;
#endif
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    //printk("event : 0x%x, flags : 0x%x\n", event, flags);

    if (!data) {
        printk("driver context passed in is null!\n");
        return RF_RET_FAILED;
    }
#ifdef CONFIG_SIWIFI_RF_RECALI
    siwifi_hw = (struct siwifi_hw *)data;
    priv = (struct v1_plat_data *)&siwifi_hw->plat->priv;
    //method to implement:
    //1, stop business, rf send stop communication
    //  do not let use to configure at this time, so block all ops
    //  1)lock the mutex
    //  2)send msg to lmac, lmac request idle and stop the channel switch mechanism
    //  3)return to rf
    //
    //2, restart business, rf send start communication
    //  1)send msg to lmac, lmac restore the idle state and enable the channel switch mechanism again
    //  2)unlock the mutex
    //  3)return to rf
    //
    switch (event) {
        case RF_EVENT_TRX_RECALI:
            {
                if (test_bit(SIWIFI_DEV_STACK_RESTARTING, &siwifi_hw->drv_flags)) {
                    printk("drv is busy for restarting.. OK\n");
                    return RF_RET_OK;
                }
                if (!test_bit(SIWIFI_DEV_STARTED, &siwifi_hw->drv_flags)) {
                    SIWIFI_DBG("drv is not started yet.. OK\n");
                    return RF_RET_OK;
                }
                error = siwifi_send_rf_calibrate(siwifi_hw, parameter ? (uint16_t)(*((uint16_t *)parameter)) : 0xFFFF);
                if (error) {
                    printk("siwifi_send_rf_calibrate failed, error: %d\n", error);
                    return RF_RET_FAILED;
                }
                return RF_RET_OK;
            }
        case RF_EVENT_NEED_RESTART:
            {
                spin_lock_bh(&siwifi_hw->cb_lock);
                if (
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
                        siwifi_hw->debugfs.recovery &&
#endif
                        !test_bit(SIWIFI_DEV_STACK_RESTARTING, &siwifi_hw->drv_flags)) {
                    siwifi_trigger_restart(siwifi_hw);
                }
                spin_unlock_bh(&siwifi_hw->cb_lock);
                return RF_RET_OK;
            }
        case RF_EVENT_STOP_BUSINESS:
            {
                struct siwifi_vif *siwifi_vif;
                //if the relicating is ongoing, return
                if (test_bit(SIWIFI_DEV_CALIBRATING, &siwifi_hw->drv_flags)) {
                    return RF_RET_OK;
                }
                //if the driver is during restarting, return again
                //rf should release the bb_mtx lock, and reschedule this action later on
                if (test_bit(SIWIFI_DEV_STACK_RESTARTING, &siwifi_hw->drv_flags)) {
                    printk("rf is busy for restarting..,call this later\n");
                    return RF_RET_AGAIN;
                }
                //wait for scan finish
                cnt = 0;
                while(siwifi_hw->scaning && cnt < 1000){
                    msleep(10);
                    cnt++;
                };
                if(cnt == 1000){
                    return RF_RET_FAILED;
                }
                //stop all txq because of recalibrtion
                spin_lock_bh(&siwifi_hw->cb_lock);
                list_for_each_entry(siwifi_vif, &siwifi_hw->vifs, list) {
                    siwifi_txq_vif_stop(siwifi_vif, SIWIFI_TXQ_STOP_RECALI, siwifi_hw);
                }
                spin_unlock_bh(&siwifi_hw->cb_lock);

                error = siwifi_send_rf_ctrl(siwifi_hw, MM_RF_CTRL_STOP_COMMUNICATION);
                if (error) {
                    printk("siwifi_send_rf_ctrl failed, error: %d\n", error);
                    WARN_ON(1);
                    return RF_RET_AGAIN;
                }

                set_bit(SIWIFI_DEV_CALIBRATING, &siwifi_hw->drv_flags);
                return RF_RET_OK;
            }
        case RF_EVENT_START_BUSINESS:
            {
                struct siwifi_vif *siwifi_vif;
                //if the relicating is ongoing, return
                if (!test_bit(SIWIFI_DEV_CALIBRATING, &siwifi_hw->drv_flags)) {
                    return RF_RET_OK;
                }
                clear_bit(SIWIFI_DEV_CALIBRATING, &siwifi_hw->drv_flags);
                if (test_bit(SIWIFI_DEV_STACK_RESTARTING, &siwifi_hw->drv_flags)) {
                    //this should not happen, because during stop/start rf commnunication , there should not
                    //be any process that make lmac enter into error state
                    //FIXME
                    WARN_ON(1);
                    return RF_RET_OK;
                }
                spin_lock_bh(&siwifi_hw->cb_lock);
                list_for_each_entry(siwifi_vif, &siwifi_hw->vifs, list) {
                    siwifi_txq_vif_start(siwifi_vif, SIWIFI_TXQ_STOP_RECALI, siwifi_hw);
                }
                spin_unlock_bh(&siwifi_hw->cb_lock);

                if ((error = siwifi_send_rf_ctrl(siwifi_hw, MM_RF_CTRL_START_COMMUNICATION)))
                {
                    printk("siwifi_send_rf_trx_calibration failed, error : %d!\n", error);
                    return RF_RET_FAILED;
                }
                return RF_RET_OK;
            }
#ifdef CONFIG_SIWIFI_RF_CMD_DUMP
        case RF_EVENT_DUMP_CMD:
            {
                struct mm_rf_cmd_dump_cfm cfm;
                if (test_bit(SIWIFI_DEV_STACK_RESTARTING, &siwifi_hw->drv_flags)) {
                    printk("drv is busy for restarting.. OK\n");
                    return RF_RET_OK;
                }
                if (!test_bit(SIWIFI_DEV_STARTED, &siwifi_hw->drv_flags)) {
                    SIWIFI_DBG("drv is not started yet.. OK\n");
                    return RF_RET_OK;
                }
                if ((error = siwifi_send_rf_cmd_dump(siwifi_hw, &cfm,
                                (((struct rf_cmd_dump_set *)parameter)->address),
                                (((struct rf_cmd_dump_set *)parameter)->size),
                                (((struct rf_cmd_dump_set *)parameter)->flag)))) {
                    printk("siwifi_send_rf_cmd_dump failed, error : %d!\n", error);
                    return RF_RET_FAILED;
                }
                if (cfm.status) {
                    printk("mm_rf_cmd_dump_cfm status error!\n");
                    return RF_RET_FAILED;
                }
                ((struct rf_cmd_dump_set *)parameter)->count = cfm.count;
                return RF_RET_OK;
            }
#endif
        case RF_EVENT_GET_TEMP:
            {
                struct mm_rf_get_temp_cfm cfm;
                if (test_bit(SIWIFI_DEV_STACK_RESTARTING, &siwifi_hw->drv_flags)) {
                    printk("drv is busy for restarting.. OK\n");
                    return RF_RET_FAILED;
                }
                if (!test_bit(SIWIFI_DEV_STARTED, &siwifi_hw->drv_flags)) {
                    SIWIFI_DBG("drv is not started yet.. OK\n");
                    return RF_RET_FAILED;
                }
                if ((error = siwifi_send_rf_get_temperature(siwifi_hw, (&cfm)))) {
                    printk("siwifi_send_rf_get_temp failed, error : %d!\n", error);
                    return RF_RET_FAILED;
                }
                ((struct rf_get_temp *)parameter)->value = cfm.value;
                return RF_RET_OK;
            }
#ifdef CONFIG_SIWIFI_TEMPERATURE_CONTROL
        case RF_EVENT_TEMP_CTL:
            {
                struct rf_cooling_temp_set * temp_set = (struct rf_cooling_temp_set *)parameter;
                int temp_ctl_level = 0;

                if (parameter == NULL) {
                    printk("Error: parameter is null!\n");
                    return RF_RET_FAILED;
                }

                if (siwifi_hw->temp_ctl.temp_ctl_parameters[0] != 1)
                    return RF_RET_OK;

                if (abs(temp_set->cur_temp - siwifi_hw->temp_ctl.last_change_temp) < TEMP_CTL_DEFAULT_JUMP_LIMIT)
                    return RF_RET_OK;

                if (temp_set->cur_temp >= siwifi_hw->temp_ctl.temp_ctl_parameters[4]) {
                    temp_ctl_level = 4;
                } else if (temp_set->cur_temp >= siwifi_hw->temp_ctl.temp_ctl_parameters[3]) {
                    if(siwifi_hw->temp_ctl.temp_ctl_level == 4)
                        temp_ctl_level = 4;
                    else
                        temp_ctl_level = 3;
                } else if (temp_set->cur_temp >= siwifi_hw->temp_ctl.temp_ctl_parameters[2]) {
                    temp_ctl_level = 2;
                } else if (temp_set->cur_temp >= siwifi_hw->temp_ctl.temp_ctl_parameters[1]) {
                    temp_ctl_level = 1;
                } else {
                    temp_ctl_level = 0;
                }

                if (siwifi_hw->temp_ctl.temp_ctl_level == temp_ctl_level)
                    return RF_RET_OK;

                siwifi_hw->temp_ctl.temp_ctl_level = temp_ctl_level;
                siwifi_hw->temp_ctl.last_change_temp = temp_set->cur_temp;
                siwifi_set_temp_ctl_level(siwifi_hw);

                return RF_RET_OK;
            }
#endif // CONFIG_SIWIFI_TEMPERATURE_CONTROL
#ifdef CONFIG_SIWIFI_COOLING_TEMP
        case RF_EVENT_COOLING_TEMP:
            if (parameter == NULL) {
                printk("Error: parameter is null!\n");
                return RF_RET_FAILED;
            }
            {
#ifdef CONFIG_SFA28_FULLMASK
                int change_power;
                int change_power_trend;
                if(siwifi_hw->mod_params->is_hb){
                    change_power = ((struct rf_cooling_temp_set *)parameter)->change_power[1];
                    change_power_trend = ((struct rf_cooling_temp_set *)parameter)->change_power_trend[1];
                }
                else{
                    change_power = ((struct rf_cooling_temp_set *)parameter)->change_power[0];
                    change_power_trend = ((struct rf_cooling_temp_set *)parameter)->change_power_trend[0];
                }
                siwifi_hw->change_power = change_power;
                siwifi_hw->change_power_trend = change_power_trend;
                if (test_bit(SIWIFI_DEV_STARTED, &siwifi_hw->drv_flags)) {
                    if(siwifi_send_change_power_req(siwifi_hw,change_power,change_power_trend)) {
                        printk("set max idx to LMAC failed!\n");
                        return RF_RET_FAILED;
                    }
                } else {
                    return RF_RET_OK;
                }
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS

                if (siwifi_hw->ate_env.tx_macbypass_start && (!(siwifi_hw->temp_disable))) {
                    int txpower_tmp = siwifi_hw->ate_env.siwifi_vif->txpower_idx & SFAX8_THERMAL_POWERMASK;
                    int SFAX8_THERMAL_POWERMAX = SFAX8_THERMAL_POWERMAX_SINGLE;
                    int SFAX8_THERMAL_POWERMIN = SFAX8_THERMAL_POWERMIN_1;
                    if (((struct rf_cooling_temp_set *)parameter)->dual_cali_enable)
                        SFAX8_THERMAL_POWERMAX = SFAX8_THERMAL_POWERMAX_DUAL;
                    if ((change_power & SFAX8_THERMAL_DIGGAINMASK) == 0){
                        if (change_power_trend  >= 0)
                            txpower_tmp +=  change_power;
                        else
                            txpower_tmp -=  change_power;
                        if (txpower_tmp > SFAX8_THERMAL_POWERMAX )
                            txpower_tmp = SFAX8_THERMAL_POWERMAX;
                        else if (txpower_tmp < SFAX8_THERMAL_POWERMIN)
                            txpower_tmp = SFAX8_THERMAL_POWERMIN;
                        txpower_tmp = txpower_tmp^(siwifi_hw->ate_env.siwifi_vif->txpower_idx & ~SFAX8_THERMAL_POWERMASK);
                    } else {
                        if ((siwifi_hw->ate_env.siwifi_vif->txpower_idx & SFAX8_THERMAL_DIGGAINMASK) == 0) {
                            if (change_power_trend  >= 0)
                                txpower_tmp += (change_power & SFAX8_THERMAL_POWERMASK);
                            else
                                txpower_tmp -= ((change_power & SFAX8_THERMAL_POWERMASK) - 1);
                        } else {
                            if (change_power_trend  >= 0)
                                txpower_tmp +=  ((change_power & SFAX8_THERMAL_POWERMASK) - 1);
                            else
                                txpower_tmp -= (change_power & SFAX8_THERMAL_POWERMASK);
                        }

                        if (txpower_tmp > SFAX8_THERMAL_POWERMAX )
                            txpower_tmp = SFAX8_THERMAL_POWERMAX;
                        else if (txpower_tmp < SFAX8_THERMAL_POWERMIN)
                            txpower_tmp = SFAX8_THERMAL_POWERMIN;
                        txpower_tmp = txpower_tmp^0x80^(siwifi_hw->ate_env.siwifi_vif->txpower_idx & ~SFAX8_THERMAL_POWERMASK);
                    }
                    //   printk("WRITE POWER : %d\n change_power=%d ",txpower_tmp,change_power);
                    REG_PL_WR((priv->base + 0x00160000 + 0x0200),txpower_tmp);
                }
#endif
#endif
                return RF_RET_OK;
            }
#endif // CONFIG_SIWIFI_COOLING_TEMP
        default:
            {
                printk("unknown event from RF!\n");
                return RF_RET_FAILED;
            }
    }
#endif //CONFIG_SIWIFI_RF_RECALI
    return RF_RET_OK;
}
#endif


#ifdef CONFIG_SIWIFI_TX_POWER_CALI
static void sf_wifi_unload_cali_conf(struct siwifi_hw *siwifi_hw)
{
    struct phy_aetnensis_cfg_tag *phy_tag =
        (struct phy_aetnensis_cfg_tag *)&siwifi_hw->phy_config;
    if (siwifi_hw->factory_info.normal_txpower_list &&
            phy_tag->normal_address)
        dma_unmap_single(siwifi_hw->dev, phy_tag->normal_address,
                phy_tag->mapping_len, DMA_BIDIRECTIONAL);
    if (siwifi_hw->factory_info.sleepmode_txpower_list && phy_tag->sleepmode_address)
        dma_unmap_single(siwifi_hw->dev, phy_tag->sleepmode_address,
                phy_tag->mapping_len, DMA_BIDIRECTIONAL);
    if (siwifi_hw->factory_info.low_txpower_list && phy_tag->low_address)
        dma_unmap_single(siwifi_hw->dev, phy_tag->low_address,
                phy_tag->mapping_len, DMA_BIDIRECTIONAL);
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
    if (siwifi_hw->factory_info.normal_txpower_list_second_antenna &&
            phy_tag->normal_address_second)
        dma_unmap_single(siwifi_hw->dev, phy_tag->normal_address_second,
                phy_tag->mapping_len, DMA_BIDIRECTIONAL);
    if (siwifi_hw->factory_info.sleepmode_txpower_list_second_antenna && phy_tag->sleepmode_address_second)
        dma_unmap_single(siwifi_hw->dev, phy_tag->sleepmode_address_second,
                phy_tag->mapping_len, DMA_BIDIRECTIONAL);
    if (siwifi_hw->factory_info.low_txpower_list_second_antenna && phy_tag->low_address_second)
        dma_unmap_single(siwifi_hw->dev, phy_tag->low_address_second,
                phy_tag->mapping_len, DMA_BIDIRECTIONAL);
#endif
    if (siwifi_hw->factory_info.high_txpower_list && phy_tag->high_address)
        dma_unmap_single(siwifi_hw->dev, phy_tag->high_address,
                phy_tag->mapping_len, DMA_BIDIRECTIONAL);
    memset(phy_tag, 0, sizeof(struct phy_aetnensis_cfg_tag));
    sf_wifi_deinit_wifi_factory_info(siwifi_hw);
}

/* phy_tag is set in this func and factory_info is set in
 * sf_wifi_init_wifi_factory_info func */
static int sf_wifi_load_cali_conf(struct siwifi_hw *siwifi_hw)
{
    int ret, rf_feature;
    uint32_t chan_num, one_chan_gain_num;
    dma_addr_t dma_addr = 0;
    dma_addr_t sleepmode_list_dma_addr = 0;
    dma_addr_t low_list_dma_addr = 0;
    dma_addr_t high_list_dma_addr = 0;
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
    dma_addr_t dma_second_addr = 0;
    dma_addr_t sleepmode_list_second_dma_addr = 0;
    dma_addr_t low_list_second_dma_addr = 0;
#endif
    struct phy_aetnensis_cfg_tag *phy_tag =
        (struct phy_aetnensis_cfg_tag *)&siwifi_hw->phy_config;
    struct v1_plat_data *priv =
        (struct v1_plat_data *)&siwifi_hw->plat->priv;
    struct siwifi_factory_info *factory_info =
        &siwifi_hw->factory_info;

    rf_feature = sf_wifi_rf_get_feature(priv->band);

    if (rf_feature & RF_FEATURE_SUPPORT_ENHANCE_LOWRATE_POWER)
        phy_tag->flag |= RF_ENHANCE_LOWRATE_POWER;

    if (priv->band & LB_MODULE) {
        if (rf_feature & RF_FEATURE_SUPPORT_EXTERNAL_PA_LB)
            phy_tag->flag |= RF_EXTERNAL_PA_ENABLE;
        chan_num = LB_CHANNEL_COUNT;
        one_chan_gain_num = LB_ONE_CHAN_GAIN_NUM;
    }
    else {
        if (rf_feature & RF_FEATURE_SUPPORT_EXTERNAL_PA_HB)
            phy_tag->flag |= RF_EXTERNAL_PA_ENABLE;
        chan_num = HB_CHANNEL_COUNT;
        one_chan_gain_num = HB_ONE_CHAN_GAIN_NUM;
    }

#ifdef CFG_DUAL_ANTENNA_CALIBRATE
    if(rf_feature & RF_FEATURE_DUAL_ANTENNA_CALIBRATE) {
        phy_tag->flag |= DUAL_ANTENNA_CALIBRATE;
    }
#endif

    ret = sf_wifi_init_wifi_factory_info(siwifi_hw);
    if (ret) {
        printk("can not get wifi info from factory\n");
        return 0;
    }

    if (factory_info->normal_txpower_list) {
        dma_addr = dma_map_single(siwifi_hw->dev, factory_info->normal_txpower_list, factory_info->list_len, DMA_BIDIRECTIONAL);
        if (dma_mapping_error(siwifi_hw->dev, dma_addr)) {
            dma_addr = 0;
            printk("dma_mapping ERROR\n");
            goto ERROR;
        }
    }

    if (factory_info->sleepmode_txpower_list) {
        sleepmode_list_dma_addr = dma_map_single(siwifi_hw->dev, factory_info->sleepmode_txpower_list, factory_info->list_len, DMA_BIDIRECTIONAL);
        if (dma_mapping_error(siwifi_hw->dev, sleepmode_list_dma_addr)) {
            sleepmode_list_dma_addr = 0;
            printk("dma_mapping ERROR\n");
            goto ERROR;
        }
    }

    if (factory_info->low_txpower_list) {
        low_list_dma_addr = dma_map_single(siwifi_hw->dev, factory_info->low_txpower_list, factory_info->list_len, DMA_BIDIRECTIONAL);
        if (dma_mapping_error(siwifi_hw->dev, low_list_dma_addr)) {
            low_list_dma_addr = 0;
            printk("dma_mapping ERROR\n");
            goto ERROR;
        }
    }

#ifdef CFG_DUAL_ANTENNA_CALIBRATE
    if (factory_info->normal_txpower_list_second_antenna) {
        dma_second_addr = dma_map_single(siwifi_hw->dev, factory_info->normal_txpower_list_second_antenna, factory_info->list_len, DMA_BIDIRECTIONAL);
        if (dma_mapping_error(siwifi_hw->dev, dma_second_addr)) {
            dma_second_addr = 0;
            printk("dma_mapping ERROR\n");
            goto ERROR;
        }
    }

    if (factory_info->sleepmode_txpower_list_second_antenna) {
        sleepmode_list_second_dma_addr = dma_map_single(siwifi_hw->dev, factory_info->sleepmode_txpower_list_second_antenna, factory_info->list_len, DMA_BIDIRECTIONAL);
        if (dma_mapping_error(siwifi_hw->dev, sleepmode_list_second_dma_addr)) {
            sleepmode_list_second_dma_addr = 0;
            printk("dma_mapping ERROR\n");
            goto ERROR;
        }
    }

    if (factory_info->low_txpower_list_second_antenna) {
        low_list_second_dma_addr = dma_map_single(siwifi_hw->dev, factory_info->low_txpower_list_second_antenna, factory_info->list_len, DMA_BIDIRECTIONAL);
        if (dma_mapping_error(siwifi_hw->dev, low_list_second_dma_addr)) {
            low_list_second_dma_addr = 0;
            printk("dma_mapping ERROR\n");
            goto ERROR;
        }
    }
#endif

    if (factory_info->high_txpower_list) {
        high_list_dma_addr = dma_map_single(siwifi_hw->dev, factory_info->high_txpower_list, factory_info->list_len, DMA_BIDIRECTIONAL);
        if (dma_mapping_error(siwifi_hw->dev, high_list_dma_addr)) {
            high_list_dma_addr = 0;
            printk("dma_mapping ERROR\n");
            goto ERROR;
        }
    }

    // set phy_tag by factory_info, phy_tag will be used in lmac.
    phy_tag->version = factory_info->version;
    phy_tag->mapping_len = factory_info->list_len;
    phy_tag->normal_address = dma_addr;
    // when initialized, we use the normal power address as the base address for calibration
    phy_tag->address = dma_addr;

    if (sleepmode_list_dma_addr){
        phy_tag->sleepmode_address = sleepmode_list_dma_addr;
    }else{
        phy_tag->sleepmode_address = dma_addr;
    }

    if (low_list_dma_addr){
        phy_tag->low_address = low_list_dma_addr;
    }else{
        phy_tag->low_address = dma_addr;
    }

    if (high_list_dma_addr){
        phy_tag->high_address = high_list_dma_addr;
    }else{
        phy_tag->high_address = dma_addr;
    }

#ifdef CFG_DUAL_ANTENNA_CALIBRATE
    if (dma_second_addr){
        phy_tag->normal_address_second = dma_second_addr;
    }else{
        phy_tag->normal_address_second = dma_addr;
    }

    phy_tag->address = phy_tag->normal_address_second;

    if (sleepmode_list_second_dma_addr){
        phy_tag->sleepmode_address_second = sleepmode_list_second_dma_addr;
    }else{
        phy_tag->sleepmode_address_second = dma_addr;
    }

    if (low_list_second_dma_addr){
        phy_tag->low_address_second = low_list_second_dma_addr;
    }else{
        phy_tag->low_address_second = dma_addr;
    }
#endif

    phy_tag->gain_num = one_chan_gain_num;
    phy_tag->chan_num = chan_num;
    phy_tag->flag |= TXPOWER_CALI_TABLE;
    printk("band %d: wifi txpower table version %d, flag %d, normal list %p, sleepmode list %p, low list %p, high list %p\n",
            siwifi_hw->mod_params->is_hb, phy_tag->version, phy_tag->flag, factory_info->normal_txpower_list, factory_info->sleepmode_txpower_list,
            factory_info->low_txpower_list, factory_info->high_txpower_list);
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
    if(phy_tag->flag & DUAL_ANTENNA_CALIBRATE) {
        printk("normal second list %p, sleepmode second list %p, low second list %p\n",factory_info->normal_txpower_list_second_antenna, factory_info->sleepmode_txpower_list_second_antenna, factory_info->low_txpower_list_second_antenna);
    }
#endif
    return 0;

ERROR:
    if (dma_addr)
        dma_unmap_single(siwifi_hw->dev, dma_addr, factory_info->list_len, DMA_BIDIRECTIONAL);
    if (sleepmode_list_dma_addr)
        dma_unmap_single(siwifi_hw->dev, sleepmode_list_dma_addr, factory_info->list_len, DMA_BIDIRECTIONAL);
    if (low_list_dma_addr)
        dma_unmap_single(siwifi_hw->dev, low_list_dma_addr, factory_info->list_len, DMA_BIDIRECTIONAL);
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
    if (dma_second_addr)
        dma_unmap_single(siwifi_hw->dev, dma_second_addr, factory_info->list_len, DMA_BIDIRECTIONAL);
    if (sleepmode_list_second_dma_addr)
        dma_unmap_single(siwifi_hw->dev, sleepmode_list_second_dma_addr, factory_info->list_len, DMA_BIDIRECTIONAL);
    if (low_list_second_dma_addr)
        dma_unmap_single(siwifi_hw->dev, low_list_second_dma_addr, factory_info->list_len, DMA_BIDIRECTIONAL);
#endif
    if (high_list_dma_addr)
        dma_unmap_single(siwifi_hw->dev, high_list_dma_addr, factory_info->list_len, DMA_BIDIRECTIONAL);
    return -1;
}
#endif

static void sf_wifi_lmac_share_ram_take(struct v1_plat_data *priv)
{
    uint8_t tmp = 0;
    //NOTE after configuare this, we use 0xB1000000 & 0xB1400000 to access share memory range
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    printk("Now take band %s\n", priv->band == 1 ? "2.4G" : "5G");
    release_reset(SF_IRAM_SOFT_RESET);
    tmp = readb((void *)REG_SYSM_SHARE_RAM_SEL);
    //in a28,bit0 select 5g sharememory as 24g la,bit1 select 24g sharememory as 5g la
#ifdef CONFIG_SFA28_FULLMASK
    if (priv->band & LB_MODULE)
        tmp &= ~(1 << 1);
    else
        tmp &= ~(1 << 0);
#endif
    writeb(tmp, (void *)REG_SYSM_SHARE_RAM_SEL);
}

static void sf_wifi_lmac_share_ram_leave(struct v1_plat_data *priv)
{
#ifdef CONFIG_SFA28_FULLMASK
    uint8_t tmp = 0;
#endif
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    printk("Now leave band %s\n", priv->band == 1 ? "2.4G" : "5G");
#ifdef CONFIG_SFA28_FULLMASK
    tmp = readb((void *)REG_SYSM_SHARE_RAM_SEL);

    if (priv->band & LB_MODULE)
        tmp &= ~(1 << 0);
    else
        tmp &= ~(1 << 1);
    writeb(tmp, (void *)REG_SYSM_SHARE_RAM_SEL);
#endif
    hold_reset(SF_IRAM_SOFT_RESET);
}

/*
 * func:
 *      reset the system manager to reset the bus;
 *      enable the clk output from bus clk;
 *      generate reset to the wifi module
 *      set the wifi boe
 * params:
 * return:
 * */
static void sf_wifi_lmac_platform_reset(struct siwifi_hw *siwifi_hw, struct v1_plat_data *priv, uint8_t init, uint8_t system_reset)
{
    uint32_t offset;
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    //TODO, I think we should not direcly reset bus2 here
    //because on the bus2, there are not only WIFI, but also have other modules
    //if we reset here , it will cause other block worked unnormally
    //maybe there is a system bus reset standard API to be called
    /*
       writeb(0x00, REG_SYSM_BUS2_RESET);
       writeb(0x01, REG_SYSM_BUS2_RESET);
     */
    //this is located in b2m1 or b2m2
    offset = (priv->band & LB_MODULE) ? SF_WIFI_1_SOFT_RESET : SF_WIFI_2_SOFT_RESET;
    if (init) {
        uint32_t value;
        //require system reset when recovery if necessary
        if (system_reset) {
            if (!siwifi_hw->mod_params->is_hb) {
                writew(0x0, (void *)(SIFLOWER_SYSCTL_BASE + SF_WIFI_1_SYSM_RESET));
                writew(0x1, (void *)(SIFLOWER_SYSCTL_BASE + SF_WIFI_1_SYSM_RESET));
#if (defined(CONFIG_SF16A18_WIFI_LA_ENABLE) && (defined(CFG_A28_V_LA_CLK_BUG) || defined(CFG_A28_FULLMASK_LA_BUG)))
                writew(0x0, (void *)(SIFLOWER_SYSCTL_BASE + SF_WIFI_2_SYSM_RESET));
                writew(0x1, (void *)(SIFLOWER_SYSCTL_BASE + SF_WIFI_2_SYSM_RESET));
#endif
            } else {
                writew(0x0, (void *)(SIFLOWER_SYSCTL_BASE + SF_WIFI_2_SYSM_RESET));
                writew(0x1, (void *)(SIFLOWER_SYSCTL_BASE + SF_WIFI_2_SYSM_RESET));
#if (defined(CONFIG_SF16A18_WIFI_LA_ENABLE) && (defined(CFG_A28_V_LA_CLK_BUG) || defined(CFG_A28_FULLMASK_LA_BUG)))
                writew(0x0, (void *)(SIFLOWER_SYSCTL_BASE + SF_WIFI_1_SYSM_RESET));
                writew(0x1, (void *)(SIFLOWER_SYSCTL_BASE + SF_WIFI_1_SYSM_RESET));
#endif
            }
        }
        release_reset(offset);
#if (defined(CONFIG_SF16A18_WIFI_LA_ENABLE) && (defined(CFG_A28_V_LA_CLK_BUG) || defined(CFG_A28_FULLMASK_LA_BUG)))
        release_reset((priv->band & LB_MODULE) ? SF_WIFI_2_SOFT_RESET : SF_WIFI_1_SOFT_RESET);
#endif

        //gating some unused modules by default
        //shutdown la clk by default, configure the la to use the ext_la_clk, and then gating this clk
        writeb(0x2, (priv->band & LB_MODULE) ? REG_SYSM_WIFI1_LA_CLK_SEL : REG_SYSM_WIFI2_LA_CLK_SEL);
        value = get_module_clk_gate(offset, 0);
        value &= 0xF7;
        set_module_clk_gate(offset, value, 0);

#if (defined(CONFIG_SF16A18_WIFI_LA_ENABLE) && (defined(CFG_A28_V_LA_CLK_BUG) || defined(CFG_A28_FULLMASK_LA_BUG)))
        value = get_module_clk_gate((priv->band & LB_MODULE) ? SF_WIFI_2_SOFT_RESET : SF_WIFI_1_SOFT_RESET, 0);
        value &= 0xF7;
        set_module_clk_gate((priv->band & LB_MODULE) ? SF_WIFI_2_SOFT_RESET : SF_WIFI_1_SOFT_RESET, value, 0);
#endif
    } else {
        hold_reset(offset);
#if (defined(CONFIG_SF16A18_WIFI_LA_ENABLE) && (defined(CFG_A28_V_LA_CLK_BUG) || defined(CFG_A28_FULLMASK_LA_BUG)))
        hold_reset((priv->band & LB_MODULE) ? SF_WIFI_2_SOFT_RESET : SF_WIFI_1_SOFT_RESET);
#endif
    }
}

#define LDPC_RAM_ADDR_OFFSET (0x00100000 + 0x9000)
//24g ldpc memory depth 245,width 29
#define LB_LDPC_MEMORY_SIZE (245 * 4)
static int sf_wifi_lmac_ldpc_fw_load(struct siwifi_hw *siwifi_hw, void *base_addr, char *filename, int is_lb)
{
    const struct firmware *fw;
    uint32_t size;
    unsigned int *src, *dst;
    int32_t bist_timeout = 100;
    uint32_t i = 0;
    int ret = 0;
    int err = 0;

    //1.Download buffer in the address 0x9000
    err = request_firmware(&fw, filename, siwifi_hw->dev);
    if (err) {
        printk("# Failed to get %s, with error: %d!\n", filename, err);
        return -1;
    }
    /* Copy the file on the Embedded side */
    //    SIWIFI_DBG
    printk("Now copy %s firmware, @ = 0x%-.8x\n", filename,
            (uint32_t)(base_addr + LDPC_RAM_ADDR_OFFSET));

    src = (unsigned int *)fw->data;
    dst = (unsigned int *)(base_addr + LDPC_RAM_ADDR_OFFSET);
    if(is_lb)
        size = LB_LDPC_MEMORY_SIZE;
    else
        size = (unsigned int)fw->size;
    //    SIWIFI_DBG("\n### size = %d\n", size);
    printk("size=%d, is_lb=%d\n", size, is_lb);
    /* check potential platform bug on multiple stores vs memcpy */
    for (i = 0; i < size; i += 4) {
        writel(*src++, dst);
        dst++;
    }

    i = 0;
    while (i < bist_timeout) {
        //check if agc binary load success
        if (!memcmp((void *)fw->data, (void *)(base_addr + LDPC_RAM_ADDR_OFFSET), size))
            break;
        msleep(10);
        i++;
    }
    //    SIWIFI_DBG
    printk("load ldpc cost %d cnt loop\n", i);
    if (i == bist_timeout) {
        printk("%s load ldpc bist timeout!\n", __func__);
        ret = -2;
        goto DONE;
    }
DONE:
    release_firmware(fw);
    //3.At the end of FSM microcode downloading,
    return ret;
}

#define AGC_MEMORY_ADDR_OFFSET (0x00100000 + 0xA000)
static int sf_wifi_lmac_agc_fw_load(struct siwifi_hw *siwifi_hw, void *base_addr, char *filename)
{
    const struct firmware *fw;
    uint32_t size;
    unsigned int *src, *dst;
    int32_t bist_timeout = 100;
    uint32_t i = 0;
    int ret = 0;
    int err = 0;

    //open AGC CLK
    mdm_agcclkforce_setf(siwifi_hw, 1);

    //1.AGC memory clock must be switched to AHB clock domain, set register AGCMEMCLKCTRL=0
    riu_agcfsmreset_setf(siwifi_hw, 1);

    //2.Download buffer in the address 0xA000
    err = request_firmware(&fw, filename, siwifi_hw->dev);
    if (err) {
        printk("# Failed to get %s, with error: %d!\n", filename, err);
        return -1;
    }

    /* Copy the file on the Embedded side */
    SIWIFI_DBG("Now copy %s firmware, @ = 0x%-.8x\n", filename,
            (uint32_t)(base_addr + AGC_MEMORY_ADDR_OFFSET));

    src = (unsigned int *)fw->data;
    dst = (unsigned int *)(base_addr + AGC_MEMORY_ADDR_OFFSET);
    size = (unsigned int)fw->size;
    SIWIFI_DBG("size = %d\n", size);
    /* check potential platform bug on multiple stores vs memcpy */
    for (i = 0; i < size; i += 4) {
        writel(*src++, dst);
        dst++;
    }

    i = 0;
    while (i < bist_timeout) {
        //check if agc binary load success
        if (!memcmp((void *)fw->data, (void *)(base_addr + AGC_MEMORY_ADDR_OFFSET), size))
            break;
        msleep(10);
        i++;
    }
    SIWIFI_DBG("load agc cost %d cnt loop\n", i);
    if (i == bist_timeout) {
        printk("%s load agc bist timeout!\n", __func__);
        ret = -2;
        goto DONE;
    }
DONE:
    release_firmware(fw);
    //3.At the end of FSM microcode downloading,
    //AGC memory clock and control must be switched back to default AGC clock domain,
    //set the register AGCMEMCLKCTRL=1
    riu_agcfsmreset_setf(siwifi_hw, 0);
    return ret;
}

static int sf_wifi_lmac_fw_load(struct siwifi_hw *siwifi_hw, struct v1_plat_data *priv)
{
    //1,load agc mem
    int ret = 0;

    ret = sf_wifi_lmac_agc_fw_load(siwifi_hw, (void *)priv->base,  (priv->band & LB_MODULE) ? SIWIFI_AGC_FW_NAME_24G : SIWIFI_AGC_FW_NAME);
    if (ret) {
        printk("sf_wifi_lmac_agc_fw_load failed\n");
        return ret;
    }
    return 0;
}

/* ************************************************************************* */
/*siwifi task to process the heavy task caused by user or interruption*/
void siwifi_task(unsigned long data)
{
    struct siwifi_hw *siwifi_hw = (struct siwifi_hw *)data;
    struct v1_plat_data *priv = (struct v1_plat_data *)&siwifi_hw->plat->priv;
    u32 status, statuses = 0, rx_status = 0;
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    unsigned long now = jiffies;
#endif
    unsigned long start = jiffies;
    unsigned long cost_time = 0;

    REG_SW_SET_PROFILING(siwifi_hw, SW_PROF_SIWIFI_IPC_IRQ_HDLR);

    if (ipc_host_get_status(siwifi_hw->ipc_env) & IPC_IRQ_E2A_RXDESC) {
        rx_status |= IPC_IRQ_E2A_RXDESC;
        ipc_host_rxdesc_handler(siwifi_hw->ipc_env);
    }

    while ((status = ipc_host_get_status(siwifi_hw->ipc_env) & (~IPC_IRQ_E2A_RXDESC))) {
        statuses |= status;
        status &= ~IPC_IRQ_E2A_RXDESC;
        /* All kinds of IRQs will be handled in one shot (RX, MSG, DBG, ...)
         * this will ack IPC irqs not the cfpga irqs */
        ipc_host_irq(siwifi_hw->ipc_env, status);
    }

#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    if (statuses & IPC_IRQ_E2A_RXDESC)
        siwifi_hw->stats.last_rx = now;
    if (statuses & IPC_IRQ_E2A_TXCFM)
        siwifi_hw->stats.last_tx = now;
#endif

    spin_lock(&siwifi_hw->tx_lock);
    siwifi_hwq_process_all(siwifi_hw);
    spin_unlock(&siwifi_hw->tx_lock);

    if (siwifi_hw->rx_skb_alloc_fail_cnt){
        spin_lock_bh(&siwifi_hw->cb_lock);
        siwifi_ipc_rxbuf_elem_fail_check(siwifi_hw);
        spin_unlock_bh(&siwifi_hw->cb_lock);
    }
    if (siwifi_hw->ipc_env->ipc_pending_status) {
        siwifi_hw->stats.ipc_pending_cnt ++;
    } else {
        // Clear rx desc status.
        if (rx_status) {
            ipc_host_status_clear(siwifi_hw->ipc_env, rx_status);
            // Make sure rx irq not miss while clear.
            ipc_host_rxdesc_handler(siwifi_hw->ipc_env);
        }
    }
    cost_time = jiffies - start;
    if(siwifi_hw->task_max_process_time < cost_time){
        siwifi_hw->task_max_process_time = cost_time;
    }
    enable_irq(priv->umac_irq);
    REG_SW_CLEAR_PROFILING(siwifi_hw, SW_PROF_SIWIFI_IPC_IRQ_HDLR);
}

int siwifi_tasks_create(struct siwifi_hw *siwifi_hw, struct v1_plat_data *priv)
{
    //first, creat lmac task
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    //TODO, this task should not init in the platform private code
    //make it become the public sw
    tasklet_init(&siwifi_hw->task, siwifi_task, (unsigned long)siwifi_hw);
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
    tasklet_init(&siwifi_hw->ate_env.tx_task, siwifi_ate_task, (unsigned long)siwifi_hw);
#endif
    return 0;
}

void siwifi_tasks_destory(struct siwifi_hw *siwifi_hw, struct v1_plat_data *priv)
{
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
    tasklet_kill(&siwifi_hw->ate_env.tx_task);
#endif
    tasklet_kill(&siwifi_hw->task);
    lmac_glue_stop(siwifi_hw, priv);
}

/*
 * c.f Modem UM (AGC/CCA initialization)
 */
static int siwifi_agc_load(struct siwifi_hw *siwifi_hw, struct v1_plat_data *priv)
{
    //TODO use standard API to request firmware
    //we only have one phy
    siwifi_hw->phy_cnt = 1;

    return sf_wifi_lmac_fw_load(siwifi_hw, priv);
}
/*
 * c.f ldpc bin load
 */
static int siwifi_ldpc_load(struct siwifi_hw *siwifi_hw, struct v1_plat_data *priv)
{

    int ret = 0;
    //TODO use standard API to request firmware
    //we only have one phy
    siwifi_hw->phy_cnt = 1;
    //1,load agc mem
    ret = sf_wifi_lmac_ldpc_fw_load(siwifi_hw, (void *)priv->base, SIWIFI_LDPC_FW_NAME, (priv->band & LB_MODULE) ? 1 : 0);

    if (ret) {
        printk("sf_wifi_lmac_ldpc_fw_load failed\n");
        return ret;
    }
    return 0;
}

static void siwifi_irqs_deinit(struct siwifi_hw *siwifi_hw, struct v1_plat_data *priv)
{
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    ipc_host_disable_irq(siwifi_hw->ipc_env, IPC_IRQ_E2A_ALL);
#ifdef CONFIG_SMP
    irq_set_affinity_hint(priv->umac_irq, NULL);
#endif
    free_irq(priv->umac_irq, siwifi_hw);
}

/**
 *
 */
static irqreturn_t siwifi_host_irq_hdlr(int irq, void *dev_id)
{
    struct siwifi_hw *siwifi_hw = (struct siwifi_hw *)dev_id;
    disable_irq_nosync(irq);
    tasklet_schedule(&siwifi_hw->task);
    return IRQ_HANDLED;
}

/*
 * */
static int siwifi_irqs_init(struct siwifi_hw *siwifi_hw, struct v1_plat_data *priv)
{
    int ret;
#ifdef CONFIG_SMP
    struct cpumask lmac_irq_affi;
    unsigned int cpu = 0;
#endif

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    //enable the host_irq
    /* sched_setscheduler on ONESHOT threaded irq handler for BCNs ? */
    if (!siwifi_hw->mod_params->is_hb)
        ret = request_irq(priv->umac_irq, siwifi_host_irq_hdlr, IRQF_TRIGGER_HIGH, "umac-lb", siwifi_hw);
    else
        ret = request_irq(priv->umac_irq, siwifi_host_irq_hdlr, IRQF_TRIGGER_HIGH, "umac-hb", siwifi_hw);

    if (ret) {
        printk("can not request the irq %d, return %d!\n", priv->umac_irq, ret);
        return ret;
    }
#ifdef CONFIG_SMP
    ret = of_property_read_u32(siwifi_hw->dev->of_node, "smp-affinity", &cpu);
    if (ret != 0)
        cpu = (!siwifi_hw->mod_params->is_hb) ? (CONFIG_NR_CPUS - 2) : (CONFIG_NR_CPUS - 1);
    if (cpu < NR_CPUS) {
        memset(&lmac_irq_affi, 0, sizeof(struct cpumask));
        cpumask_set_cpu(cpu, &lmac_irq_affi);
        ret = irq_set_affinity_hint(priv->umac_irq, &lmac_irq_affi);
        if (ret) {
            printk("can not set the affinity for irq : %d\n", priv->umac_irq);
            return ret;
        }
    }
#endif
    siwifi_ipc_start(siwifi_hw);

    return 0;
}

static int siwifi_sysm_enable(struct siwifi_hw *siwifi_hw, struct v1_plat_data *priv, uint8_t init)
{
    int ret = 0;
    //1, enable the top pll clk gating
    if (init) {
#ifdef CONFIG_SFA28_FULLMASK
        ret = clk_set_rate(priv->pl_clk, 375000000);
#endif //CONFIG_SFA28_FULLMASK
        if (ret) {
            printk("clk_set_rate failed!, ret : %d\n", ret);
        }

        ret = clk_prepare_enable(priv->pl_clk);
        if (ret) {
            printk("Failed to enable pl clk\n");
            return ret;
        }
        printk("the default platform clk rate is %lu\n", clk_get_rate(priv->pl_clk));

        ret = clk_prepare_enable(priv->bus_clk);
        if (ret) {
            printk("Failed to enable wifi bus clk\n");
            return ret;
        }
#if (defined(CONFIG_SF16A18_WIFI_LA_ENABLE) && (defined(CFG_A28_V_LA_CLK_BUG) || defined(CFG_A28_FULLMASK_LA_BUG)))
        ret = clk_prepare_enable(priv->other_band_pl_clk);
        if (ret) {
            printk("Failed to enable other band pl clk\n");
            return ret;
        }
        printk("the default other band platform clk rate is %lu\n", clk_get_rate(priv->pl_clk));

        ret = clk_prepare_enable(priv->other_band_bus_clk);
        if (ret) {
            printk("Failed to enable wifi other band bus clk\n");
            return ret;
        }
#endif

#ifdef CONFIG_SF16A18_LMAC_USE_M_SFDSP
        ret = clk_prepare_enable(priv->m_SFDSP_clk);
        if (ret) {
            printk("Failed to enable _SFDSP clk\n");
            return ret;
        }
#endif
        if(!siwifi_hw->mod_params->is_hb)
        {
            clk_set_rate(priv->pl_clk, 300000000);
            clk_set_rate(priv->bus_clk, 300000000);
            clk_set_rate(priv->m_SFDSP_clk, 375000000);
        }
    }
#ifdef CONFIG_SF16A18_LMAC_USE_M_SFDSP
    //TODO: now we have to depend on b2m1/b2m2 reset to reset _SFDSP core
    //because it has some problem to boot m_SFDSP the second time
    sf_wifi_lmac_platform_reset(siwifi_hw, priv, init, 1);
#else
    sf_wifi_lmac_platform_reset(siwifi_hw, priv, init, 0);
#endif

    if (!init) {
#ifdef CONFIG_SF16A18_LMAC_USE_M_SFDSP
        clk_disable_unprepare(priv->m_SFDSP_clk);
#endif
        clk_disable_unprepare(priv->pl_clk);
        clk_disable_unprepare(priv->bus_clk);
#if (defined(CONFIG_SF16A18_WIFI_LA_ENABLE) && (defined(CFG_A28_V_LA_CLK_BUG) || defined(CFG_A28_FULLMASK_LA_BUG)))
        clk_disable_unprepare(priv->other_band_pl_clk);
        clk_disable_unprepare(priv->other_band_bus_clk);
#endif
    }

    return ret;
}

/*
 * siwifi_platform_reset
 * reset the lmac task
 * reset the ipc memory
 * reset the irq
 * */
int siwifi_platform_restart(struct siwifi_hw *siwifi_hw)
{
    int ret;
    struct v1_plat_data *priv;
    u8 *shared_ram;
    unsigned long now = jiffies;
    static struct {
        unsigned long last;
        unsigned int cnt;
        unsigned int max_cnt;
    } restart_recs = { .last = 0, .cnt = 0, .max_cnt = 5 };

    priv = (struct v1_plat_data *)&siwifi_hw->plat->priv;

    if (!priv->on) {
        printk("platform has already been turned off!\n");
        return 0;
    }

    mutex_lock(&siwifi_hw->dbgdump_elem.mutex);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    siwifi_hw->debugfs.trace_prst = false;
#endif
    /*Step1: unregister IRQ & disable irq*/
    siwifi_irqs_deinit(siwifi_hw, priv);

    /*Step2: disable & destroy the LMAC task*/
    siwifi_tasks_destory(siwifi_hw, priv);

    /*Step3: deinit ipc*/
    siwifi_ipc_deinit(siwifi_hw);

    /*Step4: reset the platform*/
    sf_wifi_lmac_platform_reset(siwifi_hw, priv, 0, 0);

    priv->on = false;
    printk("platform shutdown : %d\n", siwifi_hw->mod_params->is_hb);
    mutex_unlock(&siwifi_hw->dbgdump_elem.mutex);

    if (restart_recs.cnt
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
            && !siwifi_hw->debugfs.force_trigger_type
#endif
       ) {
        if (restart_recs.cnt > (restart_recs.max_cnt + 1)) {
            //reset restart cnt when we exit from force_trigger recovery mode
            printk(KERN_CRIT "%s: reset restart cnt %d\n", __func__, restart_recs.cnt);
            restart_recs.cnt = 0;
        } else if (jiffies_to_msecs(now - restart_recs.last) > 3000) {
            restart_recs.cnt = 0;
        } else if (restart_recs.cnt > restart_recs.max_cnt) {
            printk(KERN_CRIT "%s: Too many failures .. aborting\n", __func__);
            return -EPERM;
        }
    }
    restart_recs.cnt++;
    restart_recs.last = now;

    /*Step5: platform module software reset, clken. boe reset*/
    sf_wifi_lmac_platform_reset(siwifi_hw, priv, 1, 1);

    /*Step6: init ipc*/
    //this must be called before start the lmac task.
    //host will init the share memory env firstly, then lmac use it
    shared_ram = lmac_glue_share_mem_init(priv);
    if ((ret = siwifi_ipc_init(siwifi_hw, shared_ram, priv->base))) {
        printk("siwifi_ipc_init failed!, ret = %d!\n", ret);
        return ret;
    }
    //realloc rx buffers case it's dealloced in ipc_deinit
#if defined(CFG_DEAGG)
    if ((ret = siwifi_ipc_rxbuf_init(siwifi_hw, RX_AMSDU_MAX_SKB_SIZE))) {
        printk("siwifi Cannot allocate the RX buffers ret=%d\n", ret);
        goto error_ipc_init;
    }
#else
    if ((ret = siwifi_ipc_rxbuf_init(siwifi_hw,
                    (4 * (siwifi_hw->mod_params->amsdu_rx_max + 1) + 1) * 1024))) {
        printk("siwifi Cannot allocate the RX buffers ret=%d\n", ret);
        goto error_ipc_init;
    }
#endif

    /*Step7: create the lmac task & enable it*/
    if ((ret = siwifi_tasks_create(siwifi_hw, priv))) {
        printk("can not create lmac task!, return %d\n", ret);
        goto error_ipc_init;
    }

    /*Step8: register & enable irq*/
    if ((ret = siwifi_irqs_init(siwifi_hw, priv))) {
        printk("siwifi_irq_init failed, ret = %d!\n", ret);
        goto error_ipc_init;
    }

    lmac_glue_start(siwifi_hw, priv);

    //RM#7889 load agc ram when recovery to avoid RIU_IRQMACCCATIMEOUTMASKED_BIT
    if ((ret = siwifi_agc_load(siwifi_hw, priv))) {
        printk("siwifi_agc_load failed, ret = %d!\n", ret);
        goto error_register_irqs;
    }

    priv->on = true;
    SIWIFI_DBG(SIWIFI_FN_EXIT_STR);
    return 0;

error_register_irqs:
    siwifi_irqs_deinit(siwifi_hw, priv);
error_ipc_init:
    siwifi_ipc_deinit(siwifi_hw);

#ifdef CONFIG_SF16A18_WIFI_RF
    //here we do not firstly unregister, it is safe here
    //because rf will not take the bb_mtx for a long time
    sf_wifi_rf_bb_unregister(priv->band);
#endif
    printk("unsuccessfully reset platform!\n");
    return ret;
}


int32_t siwifi_platform_get_clkfreq(struct siwifi_hw *siwifi_hw, int32_t clk_type)
{

    struct v1_plat_data *priv;
    priv = (struct v1_plat_data *)&siwifi_hw->plat->priv;

    //need some protection??TODO

    if (!priv->on)
        return -1;

    switch (clk_type)
    {
        case PLATFORM_CLK:
            //todo
            return 150;
        case LA_CLK:
            return priv->la_clk;

        case IQ_TE_CLK:
            //TODO:according the workmode
            return 150;
        default:
            printk("unknown clk type : %d\n", clk_type);
    }

    return 0;
}

int32_t siwifi_platform_set_clkfreq(struct siwifi_hw *siwifi_hw, int32_t clk_type, int32_t freq)
{

    struct v1_plat_data *priv;
    priv = (struct v1_plat_data *)&siwifi_hw->plat->priv;


    if (!priv->on)
        return 0;

    //TODO

    return 0;
}


static int siwifi_check_fw_compatibility(struct siwifi_hw *siwifi_hw)
{
    struct ipc_shared_env_tag *shared = siwifi_hw->ipc_env->shared;
    struct wiphy *wiphy = siwifi_hw->wiphy;
    int ipc_shared_version = 11;
    int res = 0;

    if(shared->comp_info.ipc_shared_version != ipc_shared_version)
    {
        wiphy_err(wiphy, "Different versions of IPC shared version between driver and FW (%d != %d)\n ",
                ipc_shared_version, shared->comp_info.ipc_shared_version);
        res = -1;
    }

    if(shared->comp_info.radarbuf_cnt != IPC_RADARBUF_CNT)
    {
        wiphy_err(wiphy, "Different number of host buffers available for Radar events handling "\
                "between driver and FW (%d != %d)\n", IPC_RADARBUF_CNT,
                shared->comp_info.radarbuf_cnt);
        res = -1;
    }

    if(shared->comp_info.unsuprxvecbuf_cnt != IPC_UNSUPRXVECBUF_CNT)
    {
        wiphy_err(wiphy, "Different number of host buffers available for unsupported Rx vectors "\
                "handling between driver and FW (%d != %d)\n", IPC_UNSUPRXVECBUF_CNT,
                shared->comp_info.unsuprxvecbuf_cnt);
        res = -1;
    }

    if(shared->comp_info.rxdesc_cnt != IPC_RXDESC_CNT)
    {
        wiphy_err(wiphy, "Different number of shared descriptors available for Data RX handling "\
                "between driver and FW (%d != %d)\n", IPC_RXDESC_CNT,
                shared->comp_info.rxdesc_cnt);
        res = -1;
    }

    if(shared->comp_info.rxbuf_cnt != IPC_RXBUF_CNT)
    {
        wiphy_err(wiphy, "Different number of host buffers available for Data Rx handling "\
                "between driver and FW (%d != %d)\n", IPC_RXBUF_CNT,
                shared->comp_info.rxbuf_cnt);
        res = -1;
    }

    if(shared->comp_info.msge2a_buf_cnt != IPC_MSGE2A_BUF_CNT)
    {
        wiphy_err(wiphy, "Different number of host buffers available for Emb->App MSGs "\
                "sending between driver and FW (%d != %d)\n", IPC_MSGE2A_BUF_CNT,
                shared->comp_info.msge2a_buf_cnt);
        res = -1;
    }

    if(shared->comp_info.dbgbuf_cnt != IPC_DBGBUF_CNT)
    {
        wiphy_err(wiphy, "Different number of host buffers available for debug messages "\
                "sending between driver and FW (%d != %d)\n", IPC_DBGBUF_CNT,
                shared->comp_info.dbgbuf_cnt);
        res = -1;
    }

    if(shared->comp_info.bk_txq != NX_TXDESC_CNT0)
    {
        wiphy_err(wiphy, "Driver and FW have different sizes of BK TX queue (%d != %d)\n",
                NX_TXDESC_CNT0, shared->comp_info.bk_txq);
        res = -1;
    }

    if(shared->comp_info.be_txq != NX_TXDESC_CNT1)
    {
        wiphy_err(wiphy, "Driver and FW have different sizes of BE TX queue (%d != %d)\n",
                NX_TXDESC_CNT1, shared->comp_info.be_txq);
        res = -1;
    }

    if(shared->comp_info.vi_txq != NX_TXDESC_CNT2)
    {
        wiphy_err(wiphy, "Driver and FW have different sizes of VI TX queue (%d != %d)\n",
                NX_TXDESC_CNT2, shared->comp_info.vi_txq);
        res = -1;
    }

    if(shared->comp_info.vo_txq != NX_TXDESC_CNT3)
    {
        wiphy_err(wiphy, "Driver and FW have different sizes of VO TX queue (%d != %d)\n",
                NX_TXDESC_CNT3, shared->comp_info.vo_txq);
        res = -1;
    }

#if NX_TXQ_CNT == 5
    if(shared->comp_info.bcn_txq != NX_TXDESC_CNT4)
    {
        wiphy_err(wiphy, "Driver and FW have different sizes of BCN TX queue (%d != %d)\n",
                NX_TXDESC_CNT4, shared->comp_info.bcn_txq);
        res = -1;
    }
#else
    if (shared->comp_info.bcn_txq > 0)
    {
        wiphy_err(wiphy, "BCMC enabled in firmware but disabled in driver\n");
        res = -1;
    }
#endif /* NX_TXQ_CNT == 5 */

    if(shared->comp_info.ipc_shared_size != sizeof(ipc_shared_env))
    {
        wiphy_err(wiphy, "Different sizes of IPC shared between driver and FW (%zd != %d)\n",
                sizeof(ipc_shared_env), shared->comp_info.ipc_shared_size);
        res = -1;
    }

    if(shared->comp_info.msg_api != MSG_API_VER)
    {
        wiphy_warn(wiphy, "WARNING: Different supported message API versions between "\
                "driver and FW (%d != %d)\n", MSG_API_VER, shared->comp_info.msg_api);
    }

    return res;
}

/*
   1 platform module software reset, clken, boe reset)
   2 load agc firmware
   3 init ipc
   4 register & enable irq(SIFLOWER_WIFI_INTC_IPC_HOST)
   5 register rf client
   6 create LMAC task and enable it
   7 enable the WIFI LED timer task
 */
int siwifi_platform_on(struct siwifi_hw *siwifi_hw)
{
    int ret;
    struct v1_plat_data *priv;

    u8 *shared_ram;
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    priv = (struct v1_plat_data *)&siwifi_hw->plat->priv;
    //get shared env
    if (priv->on) {
        printk("the v1 platform has already been turned on!\n");
        return 0;
    }

    /*Step1: enable all the sysm, clk gating, module clk gating, reset...*/
    if ((ret = siwifi_sysm_enable(siwifi_hw, priv, 1))) {
        printk("siwifi_sysm_enable failed\n");
        return ret;
    }

#ifdef CONFIG_SIWIFI_TX_POWER_CALI
    if ((ret = sf_wifi_load_cali_conf(siwifi_hw))) {
        printk("sf_wifi_load_cali_conf failed!\n");
        goto error_sysm_enable;
    }
#endif

    /*Step2: call lmac to pre-allocate some platform resources*/
    //this must before ipc init
    shared_ram = lmac_glue_share_mem_init(priv);

    /*Step3: init ipc*/
    //this must be called before start the lmac task.
    //host will init the share memory env firstly, then lmac use it
    if ((ret = siwifi_ipc_init(siwifi_hw, shared_ram, priv->base))) {
        printk("siwifi_ipc_init failed!, ret = %d!\n", ret);
        goto error_sysm_enable;
    }

    /*Step4: create the lmac task & enable it*/
    if ((ret = siwifi_tasks_create(siwifi_hw, priv))) {
        printk("can not create lmac task!, return %d\n", ret);
		goto error_ipc_init;
	}

    /*Step5: register & enable irq*/
    if ((ret = siwifi_irqs_init(siwifi_hw, priv))) {
        printk("siwifi_irq_init failed, ret = %d!\n", ret);
        goto error_ipc_init;
    }

    lmac_glue_start(siwifi_hw, priv);

#ifdef CONFIG_SF16A18_WIFI_RF
    /*Step6: register rf client*/
    if ((ret = sf_wifi_rf_bb_register(priv->band, &sf_wifi_rf_callback, (void *)siwifi_hw))) {
        printk("can not register rf client, ret : %d\n", ret);
        goto error_register_irqs;
    }
#endif
    /*Step7: load agc firmware*/
    if ((ret = siwifi_agc_load(siwifi_hw, priv))) {
        printk("siwifi_agc_load failed, ret = %d!\n", ret);
#ifdef CONFIG_SF16A18_WIFI_RF
        goto error_register_rfclient;
#else
        goto error_register_irqs;
#endif
    }
    if((ret = siwifi_ldpc_load(siwifi_hw, priv))){
        printk("siwifi_lapc_load failed, ret = %d!\n", ret);
    }

    if ((ret = siwifi_check_fw_compatibility(siwifi_hw))) {
#ifdef CONFIG_SF16A18_WIFI_RF
        goto error_register_rfclient;
#else
        goto error_register_irqs;
#endif
    }

#ifdef CONFIG_ERROR_DUMP
    /*Step8: create a memory to save the last 20 assert info*/
    if ((ret = siwifi_errorinfo_allocs(siwifi_hw))) {
        printk("can not alloc a memory for error dump!, return %d\n", ret);
#ifdef CONFIG_SF16A18_WIFI_RF
        goto error_register_rfclient;
#else
        goto error_register_irqs;
#endif
    }
#endif
    priv->on = true;
    printk("successfully turn on platform %d!\n", siwifi_hw->mod_params->is_hb);
    SIWIFI_DBG(SIWIFI_FN_EXIT_STR);
    return 0;

#ifdef CONFIG_SF16A18_WIFI_RF
error_register_rfclient:
    sf_wifi_rf_bb_unregister(priv->band);
#endif
error_register_irqs:
    siwifi_irqs_deinit(siwifi_hw, priv);
error_ipc_init:
    siwifi_ipc_deinit(siwifi_hw);
error_sysm_enable:
    siwifi_sysm_enable(siwifi_hw, priv, 0);

    printk("unsuccessfully turn on platform %d!\n", siwifi_hw->mod_params->is_hb);
    return ret;
}

/*
   1 disable the WIFI LED timer task
   2 disable & destroy the LMAC task
   3 unregister RF client
   4 unregister IRQ & disable irq
   5 deinit ipc
   6 platform module software reset, boe reset, clkdisable
 */
void siwifi_platform_off(struct siwifi_hw *siwifi_hw)
{
    struct v1_plat_data *priv;
    priv = (struct v1_plat_data *)&siwifi_hw->plat->priv;

    if (!priv->on) {
        printk("platform has already been turned off!\n");
        return;
    }

    /*Step1: unregister IRQ & disable irq*/
    siwifi_irqs_deinit(siwifi_hw, priv);

    /*Step2: disable & destroy the LMAC task*/
    //TODO, to check if it is any possibility to have a deadlock
    siwifi_tasks_destory(siwifi_hw, priv);

#ifdef CONFIG_SF16A18_WIFI_RF
    /*Step3: unregister RF client*/
    sf_wifi_rf_bb_unregister(priv->band);
#endif

    /*Step4: deinit ipc*/
    siwifi_ipc_deinit(siwifi_hw);

#ifdef CONFIG_SIWIFI_TX_POWER_CALI
    sf_wifi_unload_cali_conf(siwifi_hw);
#endif
#ifdef CONFIG_ERROR_DUMP
    siwifi_errorinfo_deallocs(siwifi_hw);
#endif
    /*Step5: reset the platform*/
    siwifi_sysm_enable(siwifi_hw, priv, 0);
    priv->on = false;
    printk("successfully turn off platform %d!\n", siwifi_hw->mod_params->is_hb);
}

/*
 *1 init platform context for lmac
 *2 configure iram to share memory & init share memory for dynamicly allocation later on
 *3 init wifi led resources
 */
static int siwifi_platform_init(struct siwifi_plat **siwifi_pl, struct platform_device *pdev)
{

    struct resource *res;
    uint32_t priv_size = 0;
    struct v1_plat_data *priv = NULL;
    struct siwifi_plat *siwifi_plat = NULL;
    struct siwifi_mod_params *mod_params = (struct siwifi_mod_params *)of_device_get_match_data(&pdev->dev);
    int ret;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    *siwifi_pl = NULL;

    priv_size = ALIGN(sizeof(struct siwifi_plat), sizeof(void *)) + sizeof(struct v1_plat_data);

    siwifi_plat = siwifi_kzalloc(priv_size, GFP_KERNEL);
    if (!siwifi_plat) {
        printk("can not allocate struct siwifi_plat\n");
        return -ENOMEM;
    }
    siwifi_plat->pdev = pdev;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res) {
        printk("no resource in dts file!\n");
        ret = -ENODEV;
        goto error_plat;
    }

    if (!request_mem_region(res->start, resource_size(res), pdev->name)) {
        printk("can not request mem region from 0x%x with the size 0x%x\n", res->start, resource_size(res));
        ret = -EBUSY;
        goto error_plat;
    }

    priv = (struct v1_plat_data *)&siwifi_plat->priv;
    priv->base = ioremap(res->start, resource_size(res));
    if (!priv->base) {
        printk("can not remap the sources!\n");
        ret = -EINVAL;
        goto error_resources;
    }

    priv->umac_irq = platform_get_irq(pdev, 1);
    if (priv->umac_irq == -ENXIO) {
        printk("can not get the umac_irq from system!\n");
        ret = -ENXIO;
        goto error_io;
    }

    //TODO, get this from siwifi_hw structure or somethings
    if (!mod_params->is_hb)
        priv->band = LB_MODULE;
    else
        priv->band = HB_MODULE;

    if (!mod_params->is_hb){
        //priv->pl_clk = devm_clk_get(&pdev->dev, "wifi_lb0");
        priv->pl_clk = devm_clk_get(&pdev->dev, "wlan_clk");
        printk("sucessful platform get priv->lb_band: %d\n",priv->band);
    }
    else{
        //priv->pl_clk = devm_clk_get(&pdev->dev, "wifi_lb3");
        priv->pl_clk = devm_clk_get(&pdev->dev, "wlan_clk");
        printk("sucessful platform get priv->hb_band: %d\n",priv->band);
    }


    if (IS_ERR(priv->pl_clk)) {
        dev_err(&pdev->dev, "Failed to get wlan platform clk\n");
        ret = -EINVAL;
        goto error_io;
    }

#ifdef CONFIG_SFA28_FULLMASK
#if (defined(CONFIG_SF16A18_WIFI_LA_ENABLE) && (defined(CFG_A28_V_LA_CLK_BUG) || defined(CFG_A28_FULLMASK_LA_BUG)))
    if (!mod_params->is_hb)
    {
        priv->bus_clk = devm_clk_get(&pdev->dev, "bus2_clk");
        priv->other_band_bus_clk = devm_clk_get(&pdev->dev, "bus3_clk");
        priv->other_band_pl_clk = devm_clk_get(&pdev->dev, "wlan5_clk");

    }
    else
    {
        priv->bus_clk = devm_clk_get(&pdev->dev, "bus3_clk");
        priv->other_band_bus_clk = devm_clk_get(&pdev->dev, "bus2_clk");
        priv->other_band_pl_clk = devm_clk_get(&pdev->dev, "wlan24_clk");
    }
#else
    if (!mod_params->is_hb)
    {
        priv->bus_clk = devm_clk_get(&pdev->dev, "bus2_clk");
    }
    else
    {
        priv->bus_clk = devm_clk_get(&pdev->dev, "bus3_clk");
    }
#endif //CFG_A28_V_LA_CLK_BUG
#endif //CONFIG_SFA28_FULLMASK

    if (IS_ERR(priv->bus_clk)) {
        dev_err(&pdev->dev, "Failed to get wlan bus clk\n");
        ret = -EINVAL;
        goto error_io;
    }
#if (defined(CONFIG_SF16A18_WIFI_LA_ENABLE) && (defined(CFG_A28_V_LA_CLK_BUG) || defined(CFG_A28_FULLMASK_LA_BUG)))
    if (IS_ERR(priv->other_band_bus_clk)) {
        dev_err(&pdev->dev, "Failed to get other band bus clk\n");
        ret = -EINVAL;
        goto error_io;
    }

    if (IS_ERR(priv->other_band_pl_clk)) {
        dev_err(&pdev->dev, "Failed to get other band pl clk\n");
        ret = -EINVAL;
        goto error_io;
    }
#endif

#ifdef CONFIG_SF16A18_LMAC_USE_M_SFDSP
    if (!mod_params->is_hb){
        priv->m_SFDSP_clk = devm_clk_get(&pdev->dev, "m_SFDSPclk");
        printk("lb_m_SFDSPclk\n");
     }
    else{
        priv->m_SFDSP_clk = devm_clk_get(&pdev->dev, "m_SFDSPclk");
        printk("hb_m_SFDSPclk\n");
    }
    if (IS_ERR(priv->m_SFDSP_clk)) {
        if (priv->m_SFDSP_clk)
            printk("adafafanull\n");
        printk("sucessful platform get priv->m_SFDSP_clk %lu\n",(unsigned long)priv->m_SFDSP_clk);
        dev_err(&pdev->dev, "Failed to get _SFDSP clk\n");
        ret = -EINVAL;
        goto error_io;
    }
#endif

    /*Step1: configure iram and init share memory*/
    sf_wifi_lmac_share_ram_take(priv);

    //Step2: init lmac
    if (lmac_glue_init(priv, &pdev->dev)) {
        printk("lmac_glue_init failed!\n");
        ret = -EINVAL;
        goto error_fw;
    }

    printk("%s, priv->base : %p\n", __func__, priv->base);

    *siwifi_pl = siwifi_plat;
    return 0;

error_fw:
    sf_wifi_lmac_share_ram_leave(priv);
error_io:
    iounmap(priv->base);
error_resources:
    release_mem_region(res->start, resource_size(res));
error_plat:
    siwifi_kfree(siwifi_plat);
    *siwifi_pl = NULL;
    mod_params = NULL;
    return ret;
}

/*
   1 release led resources
   2 configure share memory to iram mode
   3 destroy any platform context resource that alloc in the probe steps
 */
static void siwifi_platform_deinit(struct siwifi_plat *siwifi_plat)
{

    struct resource *res;
    struct v1_plat_data *priv;
    struct platform_device *pdev;

    priv = (struct v1_plat_data *)&siwifi_plat->priv;
    pdev = siwifi_plat->pdev;

    /*Step1: reelease the lmac resources*/
    lmac_glue_deinit(priv);

    /*Step2: configure share memory to iram mode*/
    sf_wifi_lmac_share_ram_leave(priv);

    /*Step3: destroy platform context resource*/
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (res)
        release_mem_region(res->start, resource_size(res));

    iounmap(priv->base);

    siwifi_kfree(siwifi_plat);
}

#ifndef CONFIG_SF16A18_WIFI_LB_LA_ENABLE
struct siwifi_mod_params hb_mod_param = {
    .is_hb = true,
    .ht_on = true,
    .vht_on = true,
    .he_on = true,
    .mcs_map = IEEE80211_VHT_MCS_SUPPORT_0_9,
    .he_mcs_map = IEEE80211_HE_MCS_SUPPORT_0_11,
    .ldpc_on = true,
    .stbc_on = true,
    .phy_cfg = 0,
    .uapsd_timeout = 0,
    .ap_uapsd_on = false,
    .sgi = true,
    .sgi80 = true,
    .use_2040 = true,
    .nss = 2,
    .amsdu_rx_max = 1,
    .bfmee = false,
    .bfmer = false,
    .mesh = false,
    .murx = false,
    .mutx = false,
    .mutx_on = true,
    .use_80 = true,
    .custregd = false,
    .roc_dur_max = 500,
    .listen_itv = 5,
    .listen_bcmc = true,
    .lp_clk_ppm = 1000,
    .ps_on = false,
    .tx_lft = SIWIFI_TX_LIFETIME_MS,
    .amsdu_maxnb = NX_TX_PAYLOAD_MAX,
    .amsdu_int = 100,
    .ampdu_density = 7,
    .uapsd_queues = IEEE80211_WMM_IE_STA_QOSINFO_AC_VO,
    .tdls = true,
    .uf = false,
    .addr_maskall = false,
    .not_send_null = false,
    .ampdu_max_cnt = 32,
    .rts_cts_change = 2,
    .txpower_lvl = 2,
    .led_status = 1,
    .ant_div = true,
    .tx_queue_num = 5,
};
#endif

#ifndef CONFIG_SF16A18_WIFI_HB_LA_ENABLE
struct siwifi_mod_params lb_mod_param = {
    .is_hb = false,
    .ht_on = true,
    .vht_on = true,
    .he_on = true,
    .mcs_map = IEEE80211_VHT_MCS_SUPPORT_0_9,
    .he_mcs_map = IEEE80211_HE_MCS_SUPPORT_0_11,
    .ldpc_on = true,
    .stbc_on = true,
    .phy_cfg = 0,
    .uapsd_timeout = 0,
    .ap_uapsd_on = false,
    .sgi = true,
    .sgi80 = true,
    .use_2040 = true,
    .nss = 2,
    .amsdu_rx_max = 1,
    .bfmee = false,
    .bfmer = false,
    .mesh = false,
    .murx = false,
    .mutx = false,
    .mutx_on = true,
    .use_80 = true,
    .custregd = false,
    .roc_dur_max = 500,
    .listen_itv = 5,
    .listen_bcmc = true,
    .lp_clk_ppm = 1000,
    .ps_on = false,
    .tx_lft = SIWIFI_TX_LIFETIME_MS,
    .amsdu_maxnb = NX_TX_PAYLOAD_MAX,
    .amsdu_int = 100,
    .ampdu_density = 7,
    .uapsd_queues = IEEE80211_WMM_IE_STA_QOSINFO_AC_VO,
    .tdls = true,
    .uf = false,
    .addr_maskall = false,
    .not_send_null = false,
    .ampdu_max_cnt = 32,
    .rts_cts_change = 2,
    .txpower_lvl = 2,
    .led_status = 1,
    .ant_div = true,
    .tx_queue_num = 5,
};
#endif

/*
 * func: probe the hostport umac wifi drivers to linux system, this function will mainly do the things as followed:
 * 1,get all resource from dts file, including IRQ, BASE_ADDRESS,CLK
 *   TODO: now we use a platform_ref.h to cover this issue
 * 2,register driver to mac80211 or cfg80211
 *  2.1 alloc ieee80211_mac
 *  2.2 platform resource init
 *      2.2.1 init platform context for lmac
 *      2.2.2 configure iram to share memory & init share memory for dynamicly allocation later on
 *      2.2.3 init wifi led resources
 *  2.3 turn on the lmac platform
 *      2.3.1 platform module software reset, clken, boe reset)
 *      2.3.2 load agc firmware
 *      2.3.3 init ipc
 *      2.3.4 register & enable irq(SIFLOWER_WIFI_INTC_IPC_HOST)
 *      2.3.5 register rf client
 *      2.3.6 create LMAC task and enable it
 *      2.3.7 enable the WIFI LED timer task
 * params:
 *  struct platform_device *pdev the platform device struct of linux system
 * return:
 *  0 means succussful, otherwise failed
 *
 * */
static int siwifi_hp_probe(struct platform_device *pdev)
{
    int ret;
    struct siwifi_plat *siwifi_plat = NULL;
    void *drvdata = NULL;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    if (siwifi_platform_init(&siwifi_plat, pdev)) {
        printk("siwifi_platform_init failed!\n");
        return -EINVAL;
    }
    if ((ret = siwifi_cfg80211_init(siwifi_plat, &drvdata))) {
        printk("siwifi_cfg80211_init failed, return %d!\n", ret);
        goto out_80211_init;
    }
    platform_set_drvdata(pdev, drvdata);
    return 0;

out_80211_init:
    siwifi_platform_deinit(siwifi_plat);
    return ret;
}

/*
 * func: remove the hostport umac wifi drivers from system, this function will mainly do the things as followed:
 * 1,unregister drivers from mac80211 or cfg80211
 * 2,turn off the lmac platform
 *  2.1 disable the WIFI LED timer task
 *  2.2 disable & destroy the LMAC task
 *  2.3 unregister RF client
 *  2.4 unregister IRQ & disable irq
 *  2.5 deinit ipc
 *  2.6 platform module software reset, boe reset, clkdisable
 * 3,platform resource release
 *  3.1 release led resources
 *  3.2 configure share memory to iram mode
 *  3.3 destroy any platform context resource that alloc in the probe steps
 *
 * 4,unmap the resources which map in the probe step
 * params:
 * return:
 * */
static int siwifi_hp_remove(struct platform_device *pdev)
{
    struct siwifi_hw *siwifi_hw;
    struct siwifi_plat *siwifi_plat;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    siwifi_hw = platform_get_drvdata(pdev);
    siwifi_plat = siwifi_hw->plat;
#ifdef CONFIG_BRIDGE_ACCELERATE
    if(siwifi_hw->mod_params->is_hb) {
        siwifi_hook_xmit_unreigster();
    }
#endif
    siwifi_cfg80211_deinit(siwifi_hw);
    //deinit platform
    siwifi_platform_deinit(siwifi_plat);

    platform_set_drvdata(pdev, NULL);

    return 0;
}

/************************************platform driver register*********************** */
static const char sf_wifi_module_name[16] = {
    "fmac"
};

static const struct of_device_id sf_wifi_of_match[] = {
#ifndef CONFIG_SF16A18_WIFI_HB_LA_ENABLE
	{ .compatible = "siflower,sf16a18-wifi-lb", .data = &lb_mod_param},
#endif
#ifndef CONFIG_SF16A18_WIFI_LB_LA_ENABLE
	{ .compatible = "siflower,sf16a18-wifi-hb", .data = &hb_mod_param},
#endif
	{},
};
MODULE_DEVICE_TABLE(of, sf_wifi_of_match);

/*
static struct platform_driver siwifi_hp_drv = {
	.probe		= siwifi_hp_probe,
	.remove		= siwifi_hp_remove,
	.driver = {
		.owner		= THIS_MODULE,
		.of_match_table	= sf_wifi_of_match,
	},
};
*/

static struct platform_driver siwifi_hp_drv;

int siwifi_register_hp_drv(void)
{
    int ret = 0;
    memset(&siwifi_hp_drv, 0, sizeof(struct platform_driver));
    siwifi_hp_drv.probe = siwifi_hp_probe;
    siwifi_hp_drv.remove = siwifi_hp_remove;
    siwifi_hp_drv.driver.owner =  THIS_MODULE;
    siwifi_hp_drv.driver.name = sf_wifi_module_name;
    siwifi_hp_drv.driver.of_match_table = of_match_ptr(sf_wifi_of_match);

    ret = platform_driver_register(&siwifi_hp_drv);
    SIWIFI_DBG("siwifi_register_hp_drv--ret=%d name=%s\n",ret, siwifi_hp_drv.driver.name);
    return ret;
}

void siwifi_unregister_hp_drv(void)
{
    platform_driver_unregister(&siwifi_hp_drv);
}

struct device *siwifi_platform_get_dev(struct siwifi_plat *siwifi_plat)
{
    return &(siwifi_plat->pdev->dev);
}

int siwifi_platform_register_drv(void)
{
    return siwifi_register_hp_drv();
}

void siwifi_platform_unregister_drv(void)
{
    siwifi_unregister_hp_drv();
}
