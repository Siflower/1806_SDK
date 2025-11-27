/**
 ******************************************************************************
 *
 * @file siwifi_ioctl.c
 *
 * @brief Entry point of the ioctl dispatcher of SIWIFI DRIVERS
 *
 * Copyright (C) Siflower 2018-2025
 *
 ******************************************************************************
 */

#include <linux/errno.h>
#include <linux/wireless.h>
#include <linux/mtd/mtd.h>
#include <net/mac80211.h>
#include <net/cfg80211.h>

#include "siwifi_msg_tx.h"
#include "siwifi_defs.h"
#include "reg_access.h"
#include "siwifi_ioctl.h"
#include "siwifi_tx.h"
#include "siwifi_rx.h"
#include "lmac_msg.h"
#include "hal_desc.h"
#include "ipc_host.h"
#include "siwifi_compat.h"
#include "siwifi_mem.h"

//riu reg
#define RIU_SIWIFIFECTRL0_ADDR   (REG_RIU_BASE_ADDR+0xB100)
#define RIU_SIWIFIFECTRL1_ADDR   (REG_RIU_BASE_ADDR+0xB104)

//macbypass base addr
#define REG_MACBYPASS_BASE_ADDR(band) (WIFI_BASE_ADDR(band)+0x00160000)

//macbypass reg
#define MACBYP_CTRL_ADDR(band) (REG_MACBYPASS_BASE_ADDR(band)+0x0000)
#define MACBYP_PAYLOAD_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0004)
#define MACBYP_TRIGGER_ADDR(band)   (REG_MACBYPASS_BASE_ADDR(band)+0x0008)
#define MACBYP_CLKEN_ADDR(band)    (REG_MACBYPASS_BASE_ADDR(band)+0x000C)
#define MACBYP_FRAMEPERBURST_ADDR    (REG_MACBYPASS_BASE_ADDR+0x0010)
#define MACBYP_INTERFRAME_DELAY_ADDR(band)    (REG_MACBYPASS_BASE_ADDR(band)+0x0048)
#define MACBYP_STAT_FRAME_OK_ADDR(band)   (REG_MACBYPASS_BASE_ADDR(band)+0x0080)
#define MACBYP_STAT_FRAME_BAD_ADDR(band)   (REG_MACBYPASS_BASE_ADDR(band)+0x0084)
#define MACBYP_STAT_RXEND_ADDR(band)   (REG_MACBYPASS_BASE_ADDR(band)+0x0088)
#define MACBYP_STAT_RXERROR_ADDR(band)   (REG_MACBYPASS_BASE_ADDR(band)+0x008C)
#define MACBYP_STAT_EVM_ADDR    (REG_MACBYPASS_BASE_ADDR+0x0090)
#define MACBYP_STAT_RSSI_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0094)
#define MACBYP_STAT_PHYERR_ADDR  (REG_MACBYPASS_BASE_ADDR+0x0098)
#define MACBYP_INT_CTRL_ADDR   (REG_MACBYPASS_BASE_ADDR+0x00A0)
#define MACBYP_TRIG_FORMATMOD_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0100)
#define MACBYP_TRIG_MCS_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0104)
#define MACBYP_TRIG_SGI_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0108)
#define MACBYP_TXV0_ADDR(band)   (REG_MACBYPASS_BASE_ADDR(band)+0x0200)
#define MACBYP_TXV1_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0204)
#define MACBYP_TXV2_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0208)
#define MACBYP_TXV3_ADDR   (REG_MACBYPASS_BASE_ADDR+0x020c)
#define MACBYP_TXV4_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0210)
#define MACBYP_TXV5_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0214)
#define MACBYP_TXV6_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0218)
#define MACBYP_TXV7_ADDR   (REG_MACBYPASS_BASE_ADDR+0x021C)
#define MACBYP_TXV8_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0220)
#define MACBYP_TXV9_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0224)
#define MACBYP_TXV10_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0228)
#define MACBYP_TXV11_ADDR   (REG_MACBYPASS_BASE_ADDR+0x022C)
#define MACBYP_TXV12_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0230)
#define MACBYP_TXV13_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0234)
#define MACBYP_TXV14_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0238)
#define MACBYP_TXV15_ADDR   (REG_MACBYPASS_BASE_ADDR+0x023C)
#define MACBYP_VERSION_ADDR   (REG_MACBYPASS_BASE_ADDR+0x03FC)
#define DOT1120MHZFRAMERECEIVEDCOUNT_ADDR(band) (WIFI_BASE_ADDR(band) + 0x80B80)
#define DOT1140MHZFRAMERECEIVEDCOUNT_ADDR(band) (WIFI_BASE_ADDR(band) + 0x80B84)
#define DOT1180MHZFRAMERECEIVEDCOUNT_ADDR(band) (WIFI_BASE_ADDR(band) + 0x80B88)
#define DOT11FCSERRORCOUNT_ADDR(band) (WIFI_BASE_ADDR(band) + 0x80804)
#define RWRXPHYERRORCOUNT_ADDR(band) (WIFI_BASE_ADDR(band) + 0x80808)
#define RWQOSURECEIVEDOTHERMPDU0_ADDR (WIFI_BASE_ADDR + 0x80950)
#define RWQOSURECEIVEDOTHERMPDU1_ADDR (WIFI_BASE_ADDR + 0x80954)
#define RWQOSURECEIVEDMPDUCOUNT0_ADDR(band) (WIFI_BASE_ADDR(band) + 0x80910)
#define RWQOSURECEIVEDMPDUCOUNT1_ADDR(band) (WIFI_BASE_ADDR(band) + 0x80914)
#define RWQOSURECEIVEDMPDUCOUNT2_ADDR(band) (WIFI_BASE_ADDR(band) + 0x80918)
#define RWQOSURECEIVEDMPDUCOUNT3_ADDR(band) (WIFI_BASE_ADDR(band) + 0x8091c)
#define RWQOSURECEIVEDMPDUCOUNT4_ADDR(band) (WIFI_BASE_ADDR(band) + 0x80920)
#define RWQOSURECEIVEDMPDUCOUNT5_ADDR(band) (WIFI_BASE_ADDR(band) + 0x80924)
#define RWQOSURECEIVEDMPDUCOUNT6_ADDR(band) (WIFI_BASE_ADDR(band) + 0x80928)
#define RWQOSURECEIVEDMPDUCOUNT7_ADDR(band) (WIFI_BASE_ADDR(band) + 0x8092c)
#define RWQOSURECEIVEDMPDUCOUNT10_ADDR(band) (WIFI_BASE_ADDR(band) + 0x80930)
#define RWQOSURECEIVEDMPDUCOUNT11_ADDR(band) (WIFI_BASE_ADDR(band) + 0x80934)
#define RWQOSURECEIVEDMPDUCOUNT12_ADDR(band) (WIFI_BASE_ADDR(band) + 0x80938)
#define RWQOSURECEIVEDMPDUCOUNT13_ADDR(band) (WIFI_BASE_ADDR(band) + 0x8093c)
#define RWQOSURECEIVEDMPDUCOUNT14_ADDR(band) (WIFI_BASE_ADDR(band) + 0x80940)
#define RWQOSURECEIVEDMPDUCOUNT15_ADDR(band) (WIFI_BASE_ADDR(band) + 0x80944)
#define RWQOSURECEIVEDMPDUCOUNT16_ADDR(band) (WIFI_BASE_ADDR(band) + 0x80948)
#define RWQOSURECEIVEDMPDUCOUNT17_ADDR(band) (WIFI_BASE_ADDR(band) + 0x8094c)
#define NXMAC_MAC_CNTRL_1_ADDR(band)   (REG_MAC_CORE_BASE_ADDR(band) + 0x004C)
__INLINE void nxmac_mib_table_reset_setf(struct siwifi_hw *siwifi_hw,uint8_t mibtablereset)
{
    ASSERT_ERR((((uint32_t)mibtablereset << 12) & ~((uint32_t)0x00001000)) == 0);
    REG_PL_WR(NXMAC_MAC_CNTRL_1_ADDR(siwifi_hw->mod_params->is_hb), (REG_PL_RD(NXMAC_MAC_CNTRL_1_ADDR(siwifi_hw->mod_params->is_hb)) & ~((uint32_t)0x00001000)) | ((uint32_t)mibtablereset << 12));
}

#define NXMAC_RX_CNTRL_ADDR(band)   (REG_MAC_CORE_BASE_ADDR(band) + 0x0060)
__INLINE uint32_t nxmac_rx_cntrl_get(struct siwifi_hw *siwifi_hw)
{
    return REG_PL_RD(NXMAC_RX_CNTRL_ADDR(siwifi_hw->mod_params->is_hb));
}

#define NXMAC_MAX_RX_LENGTH_ADDR(band)   (REG_MAC_CORE_BASE_ADDR(band) + 0x0150)
__INLINE void nxmac_max_rx_length_set(struct siwifi_hw *siwifi_hw,uint32_t value)
{
    REG_PL_WR(NXMAC_MAX_RX_LENGTH_ADDR(siwifi_hw->mod_params->is_hb), value);
}

#define NXMAC_DSSS_MAX_PWR_LEVEL_LSB    8
#define NXMAC_OFDM_MAX_PWR_LEVEL_LSB    0

#define DEBUG_MAC_HEAD 0
#define DEBUG_WHOLE_FRAME_MAC_HEAD 0
#define HB_COMMANDIF_BASE_ADDR 0x1400
#define LB_COMMANDIF_BASE_ADDR 0x1440
#define TXQ_IDX_BCMC 360
#define TXQ_IDX_UKS 364

#define RF_BASE_ADDR    0xB1C00000

#define SFCFG_CMD_ATE_TXVECTOR_PARAM 0x20
#define TXVECTOR_PARAM_MAX 0xF


static struct {

    //txvector1  setting

    uint8_t tx_power :8;

    //txvector2 setting
    uint8_t sounding :1;
    uint8_t feccoding :1;
    uint8_t reserved1	:1;
    uint8_t smoothing	:1;
    uint8_t continuousTx :1;
    uint8_t reserved2	:1;
    uint8_t chbw		:2;//00=20M 01=40M 10=80m 11=160m
    //txvector3 setting
    uint8_t antennaSet  :8;
    //txvector4
    uint8_t smmindex	:8;
    //txvector5
    uint8_t mcs			:7;
    uint8_t pretype		:1;
    //txvector6
    uint8_t formatmod :3;
    uint8_t numExtnSS :2;
    uint8_t nsts :3;
    //7
    uint8_t	legLength :8;
    //8
    uint8_t leglength2 :4;
    uint8_t legrate :4;
    //9
    uint8_t service :8;
    //10
    uint8_t service2 :8;
    //11
    uint8_t htlength1 :8;
    //12
    uint8_t htlength2 :8;
    //13
    uint8_t htlength3 :4;
    uint8_t aggregation :1;
    uint8_t dozeallowed :1;
    uint8_t shortgi :1;
    uint8_t disambiguityBit :1;
    //14
    uint8_t	ntx :3;
    uint8_t stbc :2;
    uint8_t reserved3 :2;
    uint8_t beamformed :1;
    //15
    uint8_t partialid :8;
    //16
    uint8_t partialid1 :1;
    uint8_t grounpid :6;
    uint8_t reserved :1;
}txvector = {
    2,
    //1
    0,//no sounding
    0,//bcc
    0,
    0,//use smoothing
    0,
    0,
    0,//20M
    //2
    3,//2*2  us 2 aetnna
    //3
    2,//beanmforming
    //4
    0,0,//long preamble
    //5
    0,
    0,
    0,
    //6
    0,
    //7
    0,0,
    //8
    0,
    //9
    0,
    //10
    0,
    //11
    0,
    //12
    1,0,0,0,0,
    //13
    1,0,0,0,
    //14
    0,
    //15
    0,0,0
};


extern int8_t rf_set_xo_value(uint32_t xo_value);
extern int8_t rf_get_temperature(bool from_lmac );
extern int8_t  rf_switch_aet_trx(int ate_num);
extern int32_t rf_trx_status_change(int txpower_idx,uint32_t txrxid,bool txrx);
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
extern int ate_aetset;
#endif

typedef int (*siwifi_ioctl_cmd_handler)(
        struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr);

static int siwifi_ioctl_do_ate_tx_frame_start(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr);

static int siwifi_ioctl_do_ate_rx_frame_start(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr);

static void siwifi_ate_rx_cb(struct siwifi_hw *siwifi_hw);

/*func:Return the results of cfg to userspace. Help user to check the status of ioctl.
 * params:*cfg, *iwr
 * return:0
 * */
static int siwifi_ioctl_response_to_user(struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr,
        int status)
{
    cfg->status = status;
    iwr->u.data.length = sizeof(struct siwifi_ioctl_cfg);
    if (copy_to_user(iwr->u.data.pointer, cfg, iwr->u.data.length)) {
        printk("copy_to_user() fail in %s\n", __func__);
        return -EFAULT;
    }
    return 0;
}

/*func:Return the results about all setting to userspace.
 *     User can check the status of setting.
 *     used by siwifi_ioctl_do_get_info.
 * params:*user,*iwr
 * return:
 * */
static int siwifi_response_info_to_user(struct siwifi_ioctl_ate_dump_info *user,
        struct iwreq *iwr)
{
    iwr->u.data.length = sizeof(struct siwifi_ioctl_ate_dump_info);

    if (copy_to_user(iwr->u.data.pointer, user, iwr->u.data.length)) {
        printk("copy_to_user() fail in %s\n", __func__);
        return -EFAULT;
    }
    return 0;
}
/*func:Convert the freq into channel idex.
 *params:*renx_hw,freq
 *return:channel idex
 * */
int siwifi_freq_to_idx(struct siwifi_hw *siwifi_hw, int freq)
{
    struct ieee80211_supported_band *sband;
    int band, ch, idx = 0;

    for (band = NL80211_BAND_2GHZ; band < NUM_NL80211_BANDS; band++) {
        sband = siwifi_hw->wiphy->bands[band];
        if (!sband) {
            continue;
        }
        printk("sband->n_channels = %d\n", sband->n_channels);
        for (ch = 0, idx = 0; ch < sband->n_channels; ch++, idx++) {
            if (sband->channels[ch].center_freq == freq) {
                goto exit;
            }
        }
    }
    idx = -1;
    if ((freq >= 4900) && (freq <= 5825)) {
        idx = 0;
    }
exit:
    // Channel has been found, return the index
    return idx;
}

/* Convert string(char) into decimal int*/
static int siwifi_char_to_int(char *tmp_data)
{
    int tmp_int = 0;

    while (*tmp_data != 0) {
        if (*tmp_data >= '0' && *tmp_data <= '9') {
            tmp_int = tmp_int * 10 + (*tmp_data - '0');
            tmp_data++;
        } else {
            return -1;
        }
    }
    return tmp_int;
}

//change char to Hexadecimal int
unsigned char siwifi_char_to_value(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';

    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;

    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    return '0';
}

unsigned char siwifi_twochar_to_byte(char h,char l)
{
    return (unsigned char)(siwifi_char_to_value(h) * 16 + siwifi_char_to_value(l));
}

unsigned int siwifi_fourchar_to_byte(char h,char l,char j,char k)
{
    return (unsigned int)(siwifi_char_to_value(h) * 4096 + siwifi_char_to_value(l) * 256 + siwifi_char_to_value(j) * 16 + siwifi_char_to_value(k));
}

/* Convert type of the address from cfg->data to the type of mac address.
 * eg.
 *    change unit8_t b="11:22:33:44:55:66" to unit8_t b={0x11,0x22,0x33,0x44,0x55,0x66}
 */
static char *siwifi_str_to_str16(char *data,char *str)
{
    int cnt = 0;
    int i = 0;
    while (cnt < 6) {
        data[cnt] = siwifi_twochar_to_byte(str[3 * cnt], str[3 * cnt + 1]);
        cnt++;
        i++;
        i++;
    }
    return data;
}

/* Convert type of the string from cfg->data to the type of Hexadecimal int.
 * eg.
 *    change unit8_t b="6566" to int b={0x65,0x66}
 */
static char *siwifi_str_to_twoint16(char *data,char *str)
{
    int cnt = 0;
    int i = 0;
    while (cnt < ((sizeof(str) - 1) / 2)) {
        data[cnt] = siwifi_twochar_to_byte(str[2 * cnt], str[2 * cnt + 1]);
        cnt++;
        i++;
        i++;
    }
    return data;
}

/* Convert string into hexadecimal int*/
static int siwifi_str_to_int16(char *tmp_data,uint32_t datalength)
{
    int tmp_int = 0x00000000;
    int length =0;
    while (length < datalength) {
        tmp_int = tmp_int * 16 + siwifi_char_to_value(*tmp_data);
        tmp_data++;
        length++;
    }
    return tmp_int;
}


static int *siwifi_str4_to_int16(int *data,char *str)
{
    data[0] = siwifi_fourchar_to_byte(str[2], str[3], str[4], str[5]);
    //printk("data[0] = 0x%0x\n",data[0]);
    return data;
}

static void siwifi_ml_apb_write(uint16_t addr, uint16_t data){

    REG_PL_WR16(RF_BASE_ADDR + (addr << 1), data);
    if (addr < 0x1300 || addr > 0x13EF)
        return;

    udelay(50);
    return;
}

static uint16_t siwifi_ml_apb_read(uint16_t addr){
    if (addr < 0x1300 || addr > 0x13EF)
        return REG_PL_RD16(RF_BASE_ADDR + (addr << 1));

    REG_PL_RD16(RF_BASE_ADDR + (addr << 1));
    udelay(1);
    return REG_PL_RD16(RF_BASE_ADDR + (0x13F1 << 1));
}

static void us_sleep(uint32_t us)
{
#define NXMAC_MAC_CNTRL_2_ADDR 0xB1080120
    uint32_t e = REG_PL_RD(NXMAC_MAC_CNTRL_2_ADDR) + us;
    do {
        volatile int n = 1 << 10; // relax
        while (n--)
            ;
    } while ((int32_t)(REG_PL_RD(NXMAC_MAC_CNTRL_2_ADDR) - e) < 0);
}
#if 0
//ABCD
static void siwifi_ate_txq_deinit(struct siwifi_hw *siwifi_hw, struct siwifi_txq *txq)
{
    spin_lock_bh(&siwifi_hw->tx_lock);
    siwifi_txq_del_from_hw_list(txq);
    txq->idx = TXQ_INACTIVE;
    siwifi_txq_flush(siwifi_hw, txq);
    spin_unlock_bh(&siwifi_hw->tx_lock);
}

static void siwifi_ate_txq_sta_deinit(struct siwifi_hw *siwifi_hw, struct siwifi_sta *siwifi_sta)
{
    struct siwifi_txq *txq;
    printk("tid is %d\n",siwifi_hw->ate_env.params->tid);
    txq = siwifi_txq_sta_get(siwifi_sta, siwifi_hw->ate_env.params->tid, NULL);

    siwifi_ate_txq_deinit(siwifi_hw, txq);
}

static int siwifi_ate_sta_remove(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
        struct ieee80211_sta *sta)
{
    struct siwifi_hw *siwifi_hw;
    struct siwifi_sta *siwifi_sta;
    int error = 0;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    siwifi_hw = hw->priv;
    siwifi_sta = (struct siwifi_sta *)sta->drv_priv;

    spin_lock_bh(&siwifi_hw->cb_lock);
    list_del(&siwifi_sta->list);

    siwifi_ate_txq_sta_deinit(siwifi_hw, siwifi_sta);

    spin_unlock_bh(&siwifi_hw->cb_lock);
    error = siwifi_send_sta_del(siwifi_hw, siwifi_sta->sta_idx);
    return error;
}
#endif //ABCD

#if 0
static int siwifi_hardware_status_save(struct siwifi_hw *siwifi_hw)
{
    //TODO
    struct mm_read_reg_cfm *read_cfm;
    int ret = 0;
    read_cfm = siwifi_kzalloc(sizeof(struct mm_read_reg_cfm), GFP_KERNEL);
    //save the value of current resigter
    if (siwifi_send_read_reg(siwifi_hw, 0, NXMAC_RX_CNTRL_ADDR, read_cfm)) {
        ret = -1;
    }
    siwifi_hw->ate_env.reg.origi_val1 = read_cfm->reg_val;

    return ret;
}

static int siwifi_hardware_status_restore(struct siwifi_hw *siwifi_hw)
{
    //TODO
    int ret = 0;
    //recover the original value of the register
    if (siwifi_send_write_reg(siwifi_hw, 0, NXMAC_RX_CNTRL_ADDR, siwifi_hw->ate_env.reg.origi_val1)) {
        ret = -1;
    }

    return ret;
}
#endif


/*
 * func:when ate test start, user must call this function.In ATE Mode, we will do:
 *  1,record all the neccessary register, when we finish the ate test, switch it back
 *  2,reset all ated statistics
 *  3,disconnect all the clients
 *  4,reset all the RF/PHY register settingsto the ated mode
 *  5,disable tx/rx until start tx/rx cmd comming in
 *
 *  now, because we are not fimiliar with our hardware feature, so make the things easy
 *  please do the manully software and reset before the ate test
 * params:
 * return:
 * */
static int siwifi_ioctl_do_ate_start(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
    int dual_antenna_calibrate = 0;
#endif
    struct siwifi_vif *siwifi_vif;
    struct phy_aetnensis_cfg_tag *phy_tag;
    siwifi_hw->ate_env.ate_start = true;
#ifdef CONFIG_SIWIFI_EASYMESH
    if (timer_pending(&siwifi_hw->sta_timer_info.timer)) {
        del_timer_sync(&siwifi_hw->sta_timer_info.timer);
    }
#endif
#if 0
    if(siwifi_hardware_status_save(siwifi_hw)){//save the status before "ate start".
        status = SIWIFI_IOCTL_RET_FAILURE;
    }
#endif
    phy_tag = (struct phy_aetnensis_cfg_tag *)&siwifi_hw->phy_config;
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
    if (phy_tag->flag & DUAL_ANTENNA_CALIBRATE)
        dual_antenna_calibrate = 1;
#endif
    if (phy_tag->flag & RF_EXTERNAL_PA_ENABLE)
        phy_tag->flag = RF_EXTERNAL_PA_ENABLE;
    else if (phy_tag->flag & RF_ENHANCE_LOWRATE_POWER)
        phy_tag->flag = RF_ENHANCE_LOWRATE_POWER;
    phy_tag->flag |= ATE_TEST_ON;
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
    if (dual_antenna_calibrate == 1)
        phy_tag->flag |= DUAL_ANTENNA_CALIBRATE;
#endif
#ifdef CONFIG_SIWIFI_TX_POWER_CALI
    siwifi_send_txpower_cali_enable_req(siwifi_hw);
#endif

    siwifi_vif = list_first_entry(&siwifi_hw->vifs, struct siwifi_vif, list);
    if (!siwifi_vif) {
        printk("there is no virtual interface on this hardware!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
    }
    siwifi_hw->ate_env.siwifi_vif = siwifi_vif;
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/*
 * func:stop ATE test,restore the status before "ate start".
 * params:*siwifi_hw,*cfg,*iwr
 * return:status */
static int siwifi_ioctl_do_ate_stop(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    //stop any test process
    siwifi_hw->ate_env.ate_start = false;
#if 0
    if(siwifi_hardware_status_restore(siwifi_hw)){
        status = SIWIFI_IOCTL_RET_FAILURE;
    }
#endif
    if (siwifi_hw->ate_env.vif_ctx_flag) {
        struct me_ate_tools_op_cfm op_cfm;
        if (siwifi_send_me_ate_tools_op(siwifi_hw, NULL, &op_cfm, DELETE_CHANTX_OP)) {
            printk("can not remove channel context!\n");
            goto stop_done;
        }
        siwifi_hw->ate_env.vif_ctx_flag = false;
    }
stop_done:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/* func:start tx,send frame.
 * params:*siwifi_hw,*cfg,*iwr
 * return:status
 * */
static int siwifi_ioctl_do_ate_tx_start(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate stoped, can not start tx directly!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto tx_start_done;
    }
#ifdef CONFIG_SIWIFI_AGG_TX
    if (strncasecmp(cfg->data, "true", 4)) {
        printk("agg_on\n");
        siwifi_hw->ate_env.agg_tx = true;
    } else
#endif
    {
        siwifi_hw->ate_env.agg_tx = false;
    }
    siwifi_ioctl_do_ate_tx_frame_start(siwifi_hw, cfg, iwr);

tx_start_done:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/* func: stop tx,clear frame count.
 * params:*siwifi_hw,*cfg,*iwr
 * return:status
 * */
static int siwifi_ioctl_do_ate_tx_stop(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate stoped, can not stop tx directly!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto tx_start_done;
    }

    if (siwifi_hw->ate_env.tx_frame_start) {
        siwifi_hw->ate_env.tx_frame_start = false;
        siwifi_hw->ate_env.conf.pkg.cont_flag = false;
        printk("force stop tx_frame,send tx=%d\n", siwifi_hw->ate_env.tx_count);
        printk("force stop tx_frame,successful tx=%d\n", siwifi_hw->ate_env.tx_successful_count);
        printk("force stop tx_frame,retry tx=%d\n", siwifi_hw->ate_env.tx_retry_count);
        siwifi_hw->ate_env.tx_left_count = 0;
    } else {
        printk("tx_frame already stopped, do nothing!\n");
    }
#if 0
    if(siwifi_hw->ate_env.agg_action){
        printk("remove sta\n");
        siwifi_ate_sta_remove(siwifi_hw->hw, siwifi_hw->ate_env.siwifi_vif->vif, siwifi_hw->ate_env.control->sta);
        //stop ampdu action
        siwifi_hw->ate_env.params->action = IEEE80211_AMPDU_TX_STOP_FLUSH;
        if (siwifi_hw->ate_env.ate_ops.ampdu_action(siwifi_hw->hw, siwifi_hw->ate_env.siwifi_vif->vif, siwifi_hw->ate_env.params)) {
            printk("ampdu action IEEE80211_AMPDU_TX_STOP_FLUSH fail!\n");
        }
        siwifi_kfree(siwifi_hw->ate_env.control->sta);
        siwifi_kfree(siwifi_hw->ate_env.control);
        siwifi_kfree(siwifi_hw->ate_env.params);
        siwifi_hw->ate_env.agg_action = false;
    }
#endif
    if (siwifi_hw->ate_env.vif_ctx_flag) {
        struct me_ate_tools_op_cfm op_cfm;
        if (siwifi_send_me_ate_tools_op(siwifi_hw, NULL, &op_cfm, DELETE_CHANTX_OP)) {
            printk("can not remove channel context!\n");
            goto tx_start_done;
        }
        siwifi_hw->ate_env.vif_ctx_flag = false;
        siwifi_txq_vif_deinit(siwifi_hw, siwifi_hw->ate_env.siwifi_vif);
    }

tx_start_done:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/* func:start Rx to receive data.
 * params:*siwifi_hw,*cfg,*iwr
 * return:status
 * */
static int siwifi_ioctl_do_ate_rx_start(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ATE not started!!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto rx_start_done;
    }

    if (siwifi_hw->ate_env.tx_frame_start) {
        printk("Tx not stoped! Stoping now...\n");
        siwifi_hw->ate_env.tx_frame_start = false;
        printk("Stop tx, OK! Rx Started!\n");
    }
    siwifi_hw->ate_env.rec_rx_count = 0;
    siwifi_hw->ate_env.all_err = 0;
    siwifi_hw->ate_env.phy_err = 0;
    siwifi_hw->ate_env.per = 0;
    siwifi_hw->ate_env.fcs_err = 0;
    nxmac_mib_table_reset_setf(siwifi_hw,1);
    siwifi_ioctl_do_ate_rx_frame_start(siwifi_hw, cfg, iwr);
    siwifi_hw->ate_env.vif_ctx_flag = true;
rx_start_done:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/* func:stop rx.
 * params:*siwifi_hw,*cfg,*iwr
 * return:status
 * */
static int siwifi_ioctl_do_ate_rx_stop(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ATE not started!!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto rx_stop_done;
    }
    if (siwifi_hw->ate_env.rx_frame_start) {
        siwifi_hw->ate_env.rx_frame_start = false;
        //recover the original value of the register
#if 0
        REG_PL_WR(NXMAC_RX_CNTRL_ADDR,siwifi_hw->ate_env.reg.origi_val1);
#endif
    }
    if (siwifi_hw->ate_env.vif_ctx_flag) {
        struct me_ate_tools_op_cfm op_cfm;
        if (siwifi_send_me_ate_tools_op(siwifi_hw, NULL, &op_cfm, DELETE_CHANTX_OP)) {
            printk("can not remove channel context!\n");
            goto rx_stop_done;
        }
         if (siwifi_send_me_ate_tools_op(siwifi_hw, NULL, &op_cfm, SET_HW_IDLE)) {
            printk("can not set mac hw idle!\n");
            goto rx_stop_done;
         }
        siwifi_hw->ate_env.vif_ctx_flag = false;
    }

rx_stop_done:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

static int siwifi_ioctl_do_ate_whole_frame_machdr(struct siwifi_hw *siwifi_hw,
        char *macheadr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
#ifdef DEBUG_PRINTK
    printk("%s: set beacon mac header\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    //memcpy(&siwifi_hw->ate_env.conf.machdr, macheadr, 24);

    memcpy(&siwifi_hw->ate_env.conf.machdr.fc, macheadr, sizeof(siwifi_hw->ate_env.conf.machdr.fc));
    memcpy(&siwifi_hw->ate_env.conf.machdr.dur, macheadr + 2, sizeof(siwifi_hw->ate_env.conf.machdr.dur));
    memcpy(&siwifi_hw->ate_env.conf.machdr.siwifi_bssid, macheadr + 4, sizeof(siwifi_hw->ate_env.conf.machdr.siwifi_bssid));
    memcpy(&siwifi_hw->ate_env.conf.machdr.siwifi_da, macheadr + 10, sizeof(siwifi_hw->ate_env.conf.machdr.siwifi_da));
    memcpy(&siwifi_hw->ate_env.conf.machdr.siwifi_sa, macheadr + 16, sizeof(siwifi_hw->ate_env.conf.machdr.siwifi_sa));
    memcpy(&siwifi_hw->ate_env.conf.machdr.seqc, macheadr + 22, sizeof(siwifi_hw->ate_env.conf.machdr.seqc));

#if DEBUG_WHOLE_FRAME_MAC_HEAD
    int i = 0;
    //printk whole frame mac header
    printk("machdr.fc = 0x%04x\n", siwifi_hw->ate_env.conf.machdr.fc & 0x0000FFFF);
    printk("machdr.dur = 0x%04x\n", siwifi_hw->ate_env.conf.machdr.dur & 0x0000FFFF);

    for (i = 0; i < 6; i++) {
        printk("mac_hdr.addr1[%d]=0x%02x\n", i, siwifi_hw->ate_env.conf.machdr.siwifi_bssid[i] & 0x000000FF);
    }

    for (i = 0; i < 6; i++) {
        printk("mac_hdr.addr2[%d]=0x%02x\n", i, siwifi_hw->ate_env.conf.machdr.siwifi_da[i] & 0x000000FF);
    }

    for (i = 0; i < 6; i++) {
        printk("mac_hdr.addr3[%d]=0x%02x\n", i, siwifi_hw->ate_env.conf.machdr.siwifi_sa[i] & 0x000000FF);
    }

    printk("machdr.seqc = 0x%04x\n", siwifi_hw->ate_env.conf.machdr.seqc & 0x0000FFFF);
#endif

DONE:
    return status;
}

static int siwifi_ioctl_do_ate_whole_frame_payload(struct siwifi_hw *siwifi_hw,
        char *payload,
        int length)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    char *tmp_payload = (char *)(payload);
#ifdef DEBUG_PRINTK
    printk("%s: set beacon payload\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    memcpy(&siwifi_hw->ate_env.conf.pkg.payload, tmp_payload, length);

DONE:
    return status;
}

static struct ieee80211_hdr_3addr *siwifi_ioctl_prepare_skb_macheader(struct siwifi_hw *siwifi_hw,
        struct ieee80211_hdr_3addr *hdr)
{
    //printk("%s: prepare skb data\n", __func__);

    memcpy(hdr, &siwifi_hw->ate_env.conf.machdr, sizeof(siwifi_hw->ate_env.conf.machdr));

#if DEBUG_MAC_HEAD
    //printk frame mac header
    printk("%s: ++== %d line ==++ hdr->frame_control = 0x%x\n", __func__, __LINE__, hdr->frame_control);
    printk("%s: hdr->duration_id = 0x%x\n", __func__, hdr->duration_id);
    printk("%s: addr1 = 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\n", __func__,
            hdr->addr1[0], hdr->addr1[1], hdr->addr1[2], hdr->addr1[3], hdr->addr1[4], hdr->addr1[5]);
    printk("%s: addr2 = 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\n", __func__,
            hdr->addr2[0], hdr->addr2[1], hdr->addr2[2], hdr->addr2[3], hdr->addr2[4], hdr->addr2[5]);
    printk("%s: addr3 = 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\n", __func__,
            hdr->addr3[0], hdr->addr3[1], hdr->addr3[2], hdr->addr3[3], hdr->addr3[4], hdr->addr3[5]);
    printk("%s: hdr->seq_ctrl = 0x%x\n", __func__, hdr->seq_ctrl);
#endif

    return hdr;
}

static struct ieee80211_qos_hdr *siwifi_ioctl_prepare_skb_qos_macheader(struct siwifi_hw *siwifi_hw,
        struct ieee80211_qos_hdr *qos_hdr,
        uint16_t seq)
{
    //printk("%s: prepare skb data\n", __func__);
    siwifi_hw->ate_env.conf.qos_machdr.fc = 0x0088;
    siwifi_hw->ate_env.conf.qos_machdr.qosc = 0x0000;
    siwifi_hw->ate_env.conf.qos_machdr.seqc = (uint16_t)(seq << 4);
    memcpy(qos_hdr, &siwifi_hw->ate_env.conf.qos_machdr, (sizeof(siwifi_hw->ate_env.conf.qos_machdr)));

#if DEBUG_MAC_HEAD
    //printk frame mac header
    printk("%s: ++== %d line ==++ qos_hdr->frame_control = 0x%x\n", __func__, __LINE__, qos_hdr->frame_control);
    printk("%s: qos_hdr->duration_id = 0x%x\n", __func__, qos_hdr->duration_id);
    printk("%s: addr1 = 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\n", __func__,
            qos_hdr->addr1[0], qos_hdr->addr1[1], qos_hdr->addr1[2], qos_hdr->addr1[3], qos_hdr->addr1[4], qos_hdr->addr1[5]);
    printk("%s: addr2 = 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\n", __func__,
            qos_hdr->addr2[0], qos_hdr->addr2[1], qos_hdr->addr2[2], qos_hdr->addr2[3], qos_hdr->addr2[4], qos_hdr->addr2[5]);
    printk("%s: addr3 = 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\n", __func__,
            qos_hdr->addr3[0], qos_hdr->addr3[1], qos_hdr->addr3[2], qos_hdr->addr3[3], qos_hdr->addr3[4], qos_hdr->addr3[5]);
    printk("%s: hdr->seq_ctrl = 0x%x\n", __func__, qos_hdr->seq_ctrl);
#endif

    return qos_hdr;
}

static struct sk_buff *siwifi_ioctl_internal_build_skb(struct siwifi_hw *siwifi_hw,
        struct siwifi_vif *siwifi_vif,
        uint16_t seq)
{
    struct sk_buff *skb;
    struct ieee80211_tx_info *info;
    struct ieee80211_tx_rate *rate;
    struct siwifi_ate_package_params *ate_pack;
    struct cfg80211_chan_def *chandef;
    struct siwifi_ate_rate_params *ate_rate;
    struct ethhdr *eth;
    int i, extra_headroom;
    int payload_flag;
    char *tmp;
    uint8_t len_hdr = 14;

    //printk("%s: bulid skb\n",__func__);
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ATE not started!\n");
        return NULL;
    }
    if (!siwifi_hw->ate_env.tx_frame_start) {
        printk("Tx stop!\n");
        return NULL;
    }
    //set this according to user configuration
    ate_pack = &siwifi_hw->ate_env.conf.pkg;
    chandef = &siwifi_hw->ate_env.conf.chandef;
    ate_rate = &siwifi_hw->ate_env.conf.rate;
    //now just consider that the payload's length to be ate_pack->frame_len
    extra_headroom = 0;
    skb = dev_alloc_skb(extra_headroom + len_hdr + ate_pack->frame_len);
    if (!skb)
        return NULL;
    skb_reserve(skb, extra_headroom);

    eth = (struct ethhdr *)skb_put(skb, len_hdr);
    memset(eth, 0, len_hdr);
    memcpy(eth->h_dest, siwifi_hw->ate_env.conf.qos_machdr.siwifi_da, ETH_ALEN);
    //hdr = (struct siwifi_ate_machdr_params *) skb_put(skb, len_hdr);
    //memset(hdr, 0, len_hdr);
    //prepare skb data for fill structure of the hdr
    //memcpy(hdr->siwifi_da, &siwifi_hw->ate_env.conf.machdr.siwifi_da, ETH_ALEN);
    //siwifi_ioctl_prepare_skb_macheader(siwifi_hw, hdr);

    //copy the user defined payload to skb
    tmp = skb_put(skb, ate_pack->frame_len);

    payload_flag = siwifi_hw->ate_env.conf.pkg.payload_mode;

    switch (payload_flag) {
        case 1:
            //set the content of payload,such as frame of beacon
            memcpy(tmp, &siwifi_hw->ate_env.conf.pkg.payload, ate_pack->frame_len);
            printk("%s: == %d line == fill whole frame payload from array\n", __func__, __LINE__);
        default:
            for (i = 0; i < ate_pack->frame_len; i++) {
                tmp[i] = i % 50;
            }
    }

    //fill the skb's prority and control info
    skb->priority = 0xFF;
    skb->queue_mapping = NX_BCMC_TXQ_NDEV_IDX;

    info = IEEE80211_SKB_CB(skb);
    info->band = chandef->chan->band;
    info->hw_queue = siwifi_hw->ate_env.hw_queue;
    //info->control.vif = siwifi_vif->vif;
    info->flags |= IEEE80211_TX_CTL_NO_ACK;
    info->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
    if (siwifi_hw->ate_env.agg_tx) {
        info->flags |= IEEE80211_TX_CTL_AMPDU;
    }
#if 1
    //set the tx rate
    rate = &info->control.rates[0];
    //rate index config
    rate->idx = ate_rate->rate_idx;

    //rate flags config
    rate->flags = 0;
    /*
       IEEE80211_TX_RC_USE_RTS_CTS                  = BIT(0),
       IEEE80211_TX_RC_USE_CTS_PROTECT              = BIT(1),
       IEEE80211_TX_RC_USE_SHORT_PREAMBLE           = BIT(2),
       IEEE80211_TX_RC_MCS                          = BIT(3),
       IEEE80211_TX_RC_GREEN_FIELD                  = BIT(4),
       IEEE80211_TX_RC_40_MHZ_WIDTH                 = BIT(5),
       IEEE80211_TX_RC_DUP_DATA                     = BIT(6),
       IEEE80211_TX_RC_SHORT_GI                     = BIT(7),
       IEEE80211_TX_RC_VHT_MCS                      = BIT(8),
       IEEE80211_TX_RC_80_MHZ_WIDTH                 = BIT(9),
       IEEE80211_TX_RC_160_MHZ_WIDTH                = BIT(10),
     */

    if (VHT_MODE == ate_rate->mode) {
        rate->flags |= IEEE80211_TX_RC_VHT_MCS;
    } else if (HT_MM_MODE == ate_rate->mode) {
        rate->flags |= IEEE80211_TX_RC_MCS;
    } else if (HT_GF_MODE == ate_rate->mode) {
        rate->flags |= IEEE80211_TX_RC_MCS | IEEE80211_TX_RC_GREEN_FIELD;
        //siwifi_hw->mod_params->gf_on = true;softmac
    } else if (NON_HT_DUP_MODE == ate_rate->mode) {
        rate->flags |= IEEE80211_TX_RC_DUP_DATA;
    } else {
        ;
    }

    if (ate_rate->use_short_preamble) {
        if (ate_rate->rate_idx <= 3 && NON_HT_MODE == ate_rate->mode) {
            rate->flags |= IEEE80211_TX_RC_USE_SHORT_PREAMBLE;
        }
    }

    if (ate_rate->use_short_gi) {
        if (NON_HT_MODE != ate_rate->mode) {
            rate->flags |= IEEE80211_TX_RC_SHORT_GI;
        }
    }
    //config frame width ,
    //dini do not support 160MHz bandwidth.
    if (NL80211_CHAN_WIDTH_80 == ate_pack->frame_bw) {
        rate->flags |= IEEE80211_TX_RC_80_MHZ_WIDTH;
    } else if (NL80211_CHAN_WIDTH_40 == ate_pack->frame_bw) {
        rate->flags |= IEEE80211_TX_RC_40_MHZ_WIDTH;
    } else {
        ;
    }
#endif

    return skb;
}

static int siwifi_build_trx_test_environment(struct siwifi_hw *siwifi_hw)
{
    struct siwifi_vif *siwifi_vif;
    struct me_ate_tools_op_cfm op_cfm;
    struct siwifi_sta *sta;
    //struct station_parameters *params;
    //u8 mac_addr[ETH_ALEN] = {0x11,0x66,0x88,0xFF,0xFF,0xFF};
    int i = 0;
    u8 txq_status = 0;

    //step1.get a vif
    siwifi_vif = siwifi_hw->ate_env.siwifi_vif;
    if (!siwifi_vif) {
        printk("there is no virtual interface on this hardware!\n");
        return -1;
    }
    printk("SIWIFI_VIF_TYPE(siwifi_vif):%d\n", SIWIFI_VIF_TYPE(siwifi_vif));
    if(SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_STATION) {
        siwifi_src_filter_hash_free(siwifi_vif);
        del_timer(&(siwifi_vif->src_filter_timer));
    }
    SIWIFI_VIF_TYPE(siwifi_vif) = NL80211_IFTYPE_AP;
    //step3.creat chantx & add chanctx
    siwifi_vif->roc_tdls = 1;
    if (siwifi_hw->ate_env.vif_ctx_flag) {
        printk("There is a ontext now!,just update it.\n");
        if (siwifi_send_me_ate_tools_op(siwifi_hw, &siwifi_hw->ate_env.conf.chandef, &op_cfm, UPDATE_CHANTX_OP)) {
            printk("can't not update the chanctx!\n");
            return -2;
        }

        siwifi_txq_vif_deinit(siwifi_hw, siwifi_vif);
        siwifi_txq_vif_init(siwifi_hw, siwifi_vif, SIWIFI_TXQ_STOP_CHAN);
        siwifi_txq_vif_start(siwifi_vif, SIWIFI_TXQ_STOP_CHAN, siwifi_hw);
        siwifi_vif->roc_tdls = 0;
    } else {
        if (siwifi_send_me_ate_tools_op(siwifi_hw, &siwifi_hw->ate_env.conf.chandef, &op_cfm, CREATE_CHANTX_OP)) {
            printk("can't not create a channel context in LMAC!\n");
            return -3;
        }
        if (siwifi_send_me_ate_tools_op(siwifi_hw, &siwifi_hw->ate_env.conf.chandef, &op_cfm,SET_HW_ACTIVE)){
            printk("SET HW ACTIVE OVERTIME!\n");
            return -3;
        }
        if (op_cfm.status == CO_OK) {
            //step4.create station
            siwifi_vif->ap.bcmc_index = NX_REMOTE_STA_MAX;
            siwifi_vif->ap.flags = 0;
            sta = &siwifi_hw->sta_table[NX_REMOTE_STA_MAX];
            sta->valid = true;
            sta->aid = 0;
            sta->sta_idx = NX_REMOTE_STA_MAX;
            sta->ch_idx = op_cfm.ch_idx;
            sta->vif_idx = siwifi_vif->vif_index;
            sta->qos = false;
            sta->acm = 0;
            sta->ps.active = false;
            siwifi_mu_group_sta_init(sta, NULL);
            spin_lock_bh(&siwifi_hw->cb_lock);
            //step5.init txq
            //selece a txq of this vif
            //TODO:should be selected by app
            siwifi_hw->ate_env.hw_queue = SIWIFI_HWQ_BE;
            siwifi_hw->ate_env.tx_queue = TXQ_IDX_BCMC;
            siwifi_hw->cur_chanctx = op_cfm.ch_idx;
            siwifi_txq_vif_init(siwifi_hw, siwifi_vif, txq_status);
            spin_unlock_bh(&siwifi_hw->cb_lock);
            netif_tx_start_all_queues(siwifi_vif->ndev);
            //step6.wait siwifi_rx_chan_switch_ind
            while (i++ < 1000) {
                msleep(10);
                if (siwifi_hw->ate_env.vif_ctx_flag)
                    break;
            }
            if (i >= 1000) {
                printk("Wait siwifi_rx_chan_switch_ind timeout\n");
                return -4;
            }
        } else {
            printk("Add a chanctx in LMAC failed!\n");
            return -5;
        }
        siwifi_vif->roc_tdls = 0;
        //step5:link the chanctx to the test vif
        siwifi_chanctx_link(siwifi_vif, op_cfm.ch_idx, &siwifi_hw->ate_env.conf.chandef);
        printk("op_cfm.ch_idx:%d\n", op_cfm.ch_idx);
    }

    //params = siwifi_kzalloc(sizeof(struct station_parameters) , GFP_KERNEL);
    //params->sta_flags_set = NL80211_STA_FLAG_AUTHORIZED | NL80211_STA_FLAG_AUTHENTICATED | NL80211_STA_FLAG_ASSOCIATED;
    //params->supported_rates_len = 12;
    //params->aid = 1;
    //params->supported_rates = NULL;
    //if(siwifi_hw->ate_env.ate_ops.add_station(siwifi_hw->wiphy,siwifi_vif->ndev,(u8 *)(&mac_addr),params)){
    //printk("can't add station!\n");
    //}
    //siwifi_txq_vif_start(siwifi_vif, SIWIFI_TXQ_IN_HWQ_LIST, siwifi_hw);
    return 0;
}

/* func:send number of frames, the frame number is in the iwreq, the frame type and payload is
 *      set by the other interface
 *      send frame steps:
 *      1,initialize all statements that for tx frames
 *      2,stop the upper layer stack queue
 *      3,flush the chosen AC queue's skbs(?? maybe do not need)
 * params:
 * return:
 * */
static int siwifi_ioctl_do_ate_tx_frame_start(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    //this is for a example that send a frame using siwifi_ops_tx
    int status = SIWIFI_IOCTL_RET_SUCCESS;
#if 0
#ifdef CONFIG_SIWIFI_AGG_TX
    struct ieee80211_tx_control *control;
    struct ieee80211_ampdu_params *params;
    int bufs = 32;
    char *tmp_num = (char *)(&cfg->data[0]);
#endif
#endif
#ifdef DEBUG_PRINTK
    printk("@@@%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto tx_frame_start_done;
    }
    if (siwifi_hw->ate_env.tx_frame_start) {
        printk("another tx frame has started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto tx_frame_start_done;
    }
#if 0
#ifdef CONFIG_SIWIFI_AGG_TX
    if (siwifi_hw->ate_env.agg_action) {
        siwifi_ate_sta_remove(siwifi_hw->hw, siwifi_hw->ate_env.siwifi_vif->vif, siwifi_hw->ate_env.control->sta);
        //stop ampdu action
        siwifi_hw->ate_env.params->action = IEEE80211_AMPDU_TX_STOP_FLUSH;
        if (siwifi_hw->ate_env.ate_ops.ampdu_action(siwifi_hw->hw, siwifi_hw->ate_env.siwifi_vif->vif, siwifi_hw->ate_env.params)) {
            printk("ampdu action IEEE80211_AMPDU_TX_STOP_FLUSH fail!\n");
            goto tx_frame_start_done;
        }
        siwifi_kfree(siwifi_hw->ate_env.control->sta);
        siwifi_kfree(siwifi_hw->ate_env.control);
        siwifi_kfree(siwifi_hw->ate_env.params);
        siwifi_hw->ate_env.agg_action = false;
    }
#endif
#endif
    if (siwifi_build_trx_test_environment(siwifi_hw)) {
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto tx_frame_start_done;
    }
#if 0
#ifdef CONFIG_SIWIFI_AGG_TX
    //step5 add station for agg_tx and start ampdu action
    if (siwifi_hw->ate_env.agg_tx) {
        printk("create a new station\n");
        control = siwifi_kzalloc(sizeof(*control), GFP_KERNEL);
        control->sta = siwifi_kzalloc(sizeof(struct ieee80211_sta), GFP_KERNEL);
        //2.4G
        if (siwifi_chanctx->ctx->def.chan->band == 0) {
            control->sta->supp_rates[0] = 0xfff;
            control->sta->supp_rates[1] = 0x00;
            control->sta->ht_cap.cap = 0x21;
            control->sta->bandwidth = 0;
        } else { //5G
            control->sta->supp_rates[0] = 0x00;
            control->sta->supp_rates[1] = 0xff;
            control->sta->ht_cap.cap = 0x63;
            control->sta->vht_cap.vht_supported = 1;
            control->sta->vht_cap.cap = 0xf800032;
            control->sta->vht_cap.vht_mcs.tx_mcs_map = 0xfffe;
            control->sta->bandwidth = 2;
        }
        control->sta->ht_cap.ht_supported = 1;
        control->sta->ht_cap.ampdu_factor = 0x3;
        control->sta->ht_cap.ampdu_density = 0x5;
        control->sta->wme = 1;
        control->sta->rx_nss = 1;
        control->sta->smps_mode = 2;
        if (siwifi_hw->ate_env.ate_ops.sta_add(siwifi_hw->hw, siwifi_vif->vif, control->sta)) {
            printk("can't add station!\n");
            goto tx_frame_start_done;
        }
        siwifi_hw->ate_env.control = control;
        siwifi_hw->ate_env.control->sta = control->sta;

        //start ampdu action
        params = siwifi_kzalloc(sizeof(*params), GFP_KERNEL);
        params->action = IEEE80211_AMPDU_TX_START;
        params->sta = siwifi_hw->ate_env.control->sta;
        params->tid = 0;
        params->ssn = 0;
        bufs = 2 * siwifi_char_to_int(tmp_num);
        printk("buf size is %d\n", bufs);
        params->buf_size = bufs;
        params->amsdu = 0;
        if (siwifi_hw->ate_env.ate_ops.ampdu_action(siwifi_hw->hw, siwifi_vif->vif, params)) {
            printk("ampdu action IEEE80211_AMPDU_TX_START fail!\n");
            goto tx_frame_start_done;
        }
        siwifi_hw->ate_env.params = params;
        siwifi_hw->ate_env.agg_action = true;
        params->action = IEEE80211_AMPDU_TX_OPERATIONAL;
        if (siwifi_hw->ate_env.ate_ops.ampdu_action(siwifi_hw->hw, siwifi_vif->vif, params)) {
            printk("ampdu action IEEE80211_AMPDU_TX_OPERATIONAL fail!\n");
            goto tx_frame_start_done;
        }
        siwifi_hw->stats.agg_done = 0;
        siwifi_hw->stats.agg_retries = 0;
        siwifi_hw->stats.agg_died = 0;
        siwifi_hw->ate_env.tx_agg_retry = 0;
        siwifi_hw->ate_env.tx_agg_done = 0;
    }
#endif
#endif
    //step6: do test
    siwifi_hw->ate_env.tx_total_count = siwifi_hw->ate_env.conf.pkg.frame_num;
    siwifi_hw->ate_env.tx_left_count = siwifi_hw->ate_env.tx_total_count;
    siwifi_hw->ate_env.tx_count = 0;
    siwifi_hw->ate_env.tx_retry_count = 0;
    siwifi_hw->ate_env.tx_successful_count = 0;

    printk("%s, siwifi_hw->ate_env.tx_left_count =%d\n", __func__, siwifi_hw->ate_env.tx_left_count);
    siwifi_hw->ate_env.tx_frame_start = true;

    //schedule the tx_task to burst send frames
    tasklet_schedule(&siwifi_hw->ate_env.tx_task);
tx_frame_start_done:
    return status;
}

/************************************rx*********************************/
/* func:RX Start
 * params:
 * return:status
 * */
static int siwifi_ioctl_do_ate_rx_frame_start(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    uint32_t mpdu_offset = sizeof(struct hw_rxhdr) + 2;
    printk("@@@%s\n", __func__);

    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto rx_frame_start_done;
    }

    if (siwifi_hw->ate_env.rx_frame_start) {
        printk("another rx frame has started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto rx_frame_start_done;
    }

    siwifi_hw->ate_env.reg.origi_val1 = nxmac_rx_cntrl_get(siwifi_hw);
    //limit the rx length
    nxmac_max_rx_length_set(siwifi_hw, siwifi_hw->ipc_env->rx_bufsz - mpdu_offset);

    //set filter to accept Error Frames and accept Unicast
    //siwifi_send_set_filter(siwifi_hw,FIF_PROMISC_IN_BSS|FIF_ALLMULTI|FIF_FCSFAIL|
    //FIF_PLCPFAIL|FIF_OTHER_BSS);

    //siwifi_send_set_filter(siwifi_hw,FIF_PROMISC_IN_BSS|FIF_FCSFAIL|FIF_PLCPFAIL|FIF_OTHER_BSS);
    //siwifi_send_set_filter(siwifi_hw,1|FIF_OTHER_BSS);
    REG_PL_WR(0xB1080060, 0x150286D8);
    REG_PL_WR(0xB1480060, 0x150286D8);
    //support ldpc
    siwifi_hw->mod_params->ldpc_on = true;

    if (siwifi_build_trx_test_environment(siwifi_hw)) {
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto rx_frame_start_done;
    }

    siwifi_hw->ate_env.rx_frame_start = true;
rx_frame_start_done:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/* func:load the setting from &cfg->ata, set phy bandwidth into struct siwifi_hw->ate_env
 * params:  enum nl80211_chan_width {
 NL80211_CHAN_WIDTH_20_NOHT,
 NL80211_CHAN_WIDTH_20,
 NL80211_CHAN_WIDTH_40,
 NL80211_CHAN_WIDTH_80,
 NL80211_CHAN_WIDTH_80p80,
 NL80211_CHAN_WIDTH_160,
 };
 * return:status
 * */
static int siwifi_ioctl_do_set_phy_bandwidth(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    struct cfg80211_chan_def *chandef;
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    int tmp_data = 0;
    char *tmp_num = (char *)(&cfg->data[0]);
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    tmp_data = siwifi_char_to_int(tmp_num);
#ifdef DEBUG_PRINTK
    printk("^^^^^^^tmp_data : %d\n", tmp_data);
#endif
    if (tmp_data < 0 || (tmp_data > 4)) {
        printk("!!!BW Error\n");
        status = SIWIFI_IOCTL_RET_INVALID_DATA;
        goto DONE;
    }

    chandef = &siwifi_hw->ate_env.conf.chandef;
    //check if it is a new value
    if (chandef->width == tmp_data) {
        printk("same setting,do nothing.\n");
        goto DONE;
    }
    chandef->width = tmp_data;
DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/* func:load the setting from &cfg->ata, set mac bandwidth into struct siwifi_hw->ate_env
 * params:  enum nl80211_chan_width {
 NL80211_CHAN_WIDTH_20_NOHT,
 NL80211_CHAN_WIDTH_20,
 NL80211_CHAN_WIDTH_40,
 NL80211_CHAN_WIDTH_80,
 NL80211_CHAN_WIDTH_80p80,
 NL80211_CHAN_WIDTH_160,
 };
 * return:status
 * */
static int siwifi_ioctl_do_set_frame_bandwidth(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    int tmp_data = 0;
    char *tmp_num = (char *)(&cfg->data[0]);
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }
    tmp_data = siwifi_char_to_int(tmp_num);
#ifdef	DEBUG_PRINTK
    printk("^^^^^^^tmp_data : %d\n", tmp_data);
#endif
    if (tmp_data < 0 || (tmp_data > 4)) {
        printk("!!!BW Error\n");
        status = SIWIFI_IOCTL_RET_INVALID_DATA;
        goto DONE;
    }
    //check if it is a new value
    if (siwifi_hw->ate_env.conf.pkg.frame_bw == tmp_data) {
        printk("same setting,do nothing.\n");
        goto DONE;
    }
    siwifi_hw->ate_env.conf.pkg.frame_bw = tmp_data;
DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}
/* func:
 * params:
 * return:status
 * */
static int siwifi_ioctl_do_set_macbypass_interval(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    int tmp_data = 0;
    char *tmp_cou = (char *)(&cfg->data[0]);
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    tmp_data = siwifi_char_to_int(tmp_cou);
#ifdef DEBUG_PRINTK
    printk("set---macbypass-cant=%d\n", tmp_data);
#endif
    if (tmp_data<100){
        printk("you should set more frame interval !!!!!\n");
    }
    siwifi_hw->ate_env.conf.pkg.macbypass_interval = tmp_data;

DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}
/* func:
 * params:
 * return:status
 * */
static int siwifi_ioctl_do_set_xo_value(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    int tmp_data = 0;
    char *tmp_xo_value = (char *)(&cfg->data[0]);
    uint8_t xo_value = 0;
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    tmp_data = siwifi_char_to_int(tmp_xo_value);
#ifdef DEBUG_PRINTK
    printk("Set XO_VALUE  =%d\n", tmp_data);
#endif
    xo_value = tmp_data & 0xff;
    status = rf_set_xo_value(xo_value);
DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}
/* func:
 * params:
 * return:status
 * */
static int siwifi_ioctl_do_set_aet_num(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    int tmp_data = 0;
    char *tmp_confg = (char *)(&cfg->data[0]);
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
    ate_aetset = 1;
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }
    sscanf(tmp_confg,"%x",&tmp_data);
    //#ifdef DEBUG_PRINTK
    printk("Set aet_setting  =0x%x\n", tmp_data);
    //#endif
    status = rf_switch_aet_trx(tmp_data);
DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
    ate_aetset = 0;
#endif
    return status;
}

/* func:load the setting from &cfg->data, set prim_freq into struct siwifi_hw->ate_env
 * params:
 *    2G Band idx0-2412, idx1-2417, idx2-2422
 idx3-2427, idx4-2432, idx5-2437, idx6-2442
 idx7-2447, idx8-2452, idx9-2457, idx10-2462
 idx11-2467, idx12-2472, idx13-2484
 *  //5G band idx0~27
//5180    5190    5200    5210    5220    5230
//5240    5260    5280    5300    5320    5500
//5520    5540    5560    5580    5600    5620
//5640    5660    5680    5700    5720    5745
//5765    5785    5805    5825
 * return:status
 * */
static int siwifi_ioctl_do_set_channel(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    struct cfg80211_chan_def *chandef;
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    int tmp_data = 0;
    char *tmp_freq = (char *)&cfg->data[0];
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    //change string to int
    tmp_data = siwifi_char_to_int(tmp_freq);
#ifdef DEBUG_PRINTK
    printk("^^^^^^^tmp_data : %d\n", tmp_data);
#endif
    if (tmp_data < 0) {
        printk("Freq Error\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    chandef = &siwifi_hw->ate_env.conf.chandef;
    //check if it is a new value
    if (!chandef->chan) {
        chandef->chan = siwifi_kzalloc(sizeof(struct ieee80211_channel), GFP_KERNEL);
    }
    if (chandef->chan->center_freq == tmp_data) {
        printk("same setting,do nothing.\n");
        goto DONE;
    }
    chandef->chan->center_freq = tmp_data;
    //set band according to freq
    if (chandef->chan->center_freq <= 2484) {
        chandef->chan->band = NL80211_BAND_2GHZ;
    } else if (chandef->chan->center_freq > 4895 && chandef->chan->center_freq < 5835) {
        chandef->chan->band = NL80211_BAND_5GHZ;
    } else {
        printk("error band!\n");
        status = SIWIFI_IOCTL_RET_INVALID_DATA;
        goto DONE;
    }
    if (chandef->center_freq1 == 0) {
        chandef->center_freq1 = chandef->chan->center_freq;
        chandef->center_freq2 = 0;
    }
DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/* func:load the setting from &cfg->data, set center_freq1 into struct siwifi_hw->ate_env
 * params:
 *    2G Band idx0-2412, idx1-2417, idx2-2422
 idx3-2427, idx4-2432, idx5-2437, idx6-2442
 idx7-2447, idx8-2452, idx9-2457, idx10-2462
 idx11-2467, idx12-2472, idx13-2484
 *  //5G band idx0~27
//5180    5190    5200    5210    5220    5230
//5240    5260    5280    5300    5320    5500
//5520    5540    5560    5580    5600    5620
//5640    5660    5680    5700    5720    5745
//5765    5785    5805    5825
 * return:status
 * */
static int siwifi_ioctl_do_set_center_freq1(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    struct cfg80211_chan_def *chandef;
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    int tmp_data = 0;
    char *tmp_freq = (char *)&cfg->data[0];
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    //change string to int
    tmp_data = siwifi_char_to_int(tmp_freq);
#ifdef DEBUG_PRINTK
    printk("^^^^^^^tmp_data : %d\n", tmp_data);
#endif

    if (!(((tmp_data >= 2412) && (tmp_data <= 2484)) || ((tmp_data >= 4900) && (tmp_data <= 5825)))) {
        printk("Freq Error\n");
        status = SIWIFI_IOCTL_RET_INVALID_DATA;
        goto DONE;
    }

    chandef = &siwifi_hw->ate_env.conf.chandef;
    //check if it is a new value
    if (chandef->center_freq1 == tmp_data) {
        printk("same setting,do nothing.\n");
        goto DONE;
    }

    chandef->center_freq1 = tmp_data;
    chandef->center_freq2 = 0;
DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}


/*func:
 *
 *
 * */
static int siwifi_ioctl_do_read_from_mtd(struct siwifi_hw *siwifi_hw, struct siwifi_ioctl_cfg *cfg,struct iwreq *iwr){
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    int xo_config_offset = 2048;
    unsigned char *buf = NULL;
    struct mtd_info *mtd = NULL;
    const char part[] = "factory";
    int retlen = 0;
    int length = cfg->length;
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif

    printk("enter the read from mtd\r\n");
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    mtd = get_mtd_device_nm(part);
    if (IS_ERR(mtd))
        return PTR_ERR(mtd);
    buf = siwifi_kmalloc(mtd->erasesize, GFP_KERNEL);
    if (!buf)
        return -ENOMEM;
    mtd_read(mtd, 0, mtd->erasesize, &retlen, buf);

    //   if ((buf[xo_config_offset] == 'X' &&buf[xo_config_offset+1]=='O')||(buf[xo_config_offset] == 'V'&&buf[xo_config_offset+1]=='2')){
    memcpy(&cfg->data[0],&buf[xo_config_offset],length);
    printk("memcpy end\n");
    //  }
    //  else{
    //      snprintf(&cfg->data[0], 20, "no cali_table");
    //  }
DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    put_mtd_device(mtd);
    siwifi_kfree(buf);
    return status;
}


/*
 * func:save the value to mtd block3.
 * params:
 * return:0 success, otherwise failure
 * */
static int siwifi_ioctl_do_save_data_to_mtd(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    unsigned char *buf = NULL;
    struct erase_info ei;
    struct mtd_info *mtd = NULL;
    const char part[] = "factory";
    uint32_t offset = 0;
    uint16_t length = cfg->length;
    char p_offset[4] = "0000";
    char *p_data = (char *)(&cfg->data[4]);
    size_t retlen;
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif

    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        siwifi_ioctl_response_to_user(cfg, iwr, SIWIFI_IOCTL_RET_FAILURE);
        return SIWIFI_IOCTL_RET_FAILURE;
    }
    mtd = get_mtd_device_nm(part);
    if (IS_ERR(mtd))
        return PTR_ERR(mtd);
    buf = siwifi_kmalloc(mtd->erasesize, GFP_KERNEL);
    if (!buf)
        return -ENOMEM;
    mtd_read(mtd, 0, mtd->erasesize, &retlen, buf);

    ei.mtd = mtd;
    ei.callback = NULL;
    ei.addr = 0;
    ei.len = mtd->erasesize;
    ei.priv = 0;
    mtd_erase(mtd, &ei);

    memcpy(p_offset,(&cfg->data[0]),4);
    offset = siwifi_char_to_int(&p_offset[0]);

    printk("in ioctl offset is %d,data's length is %d data is %d\n",offset,length,cfg->data[4]);
    memcpy(&buf[offset],p_data,length);

    //write the config to mtd clock
    if (mtd_write(mtd, 0, mtd->erasesize, &retlen, buf)) {
        printk("get config from mtd error\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
    }
    put_mtd_device(mtd);
    siwifi_kfree(buf);
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}


/*func:get factory power form mtd3 and return to ate main.c
 *params:channel,rate,mode,mcs
 *return:power
 */

static int _siwifi_ioctl_do_get_factory_power(struct siwifi_hw *siwifi_hw, struct siwifi_ioctl_cfg *cfg, struct iwreq *iwr)
{
    char *tmp_data = (char *)(&cfg->data[0]);
    int channel, mode, rate, bw;
    int power;
    char part[] = "factory";
    struct mtd_info *mtd;
    unsigned char *buf = NULL;
    int tmp_power = 0;
    int retlen;
    int offset = 0;

    //printk("enter into the ioctl  get factory power\r\n");
    sscanf(tmp_data, "%d %d %d %d", &channel, &bw, &mode, &rate);
    //printk("channel : %d,bw :%d,mode :%d,rate:%d", channel, bw, mode, rate);
    if (channel < 3000) {
        offset = (channel - 2412) / 5 * 28;
        switch (mode) {
            case 0:
                offset += rate;
                break;
            case 2:
                if (bw == 1)
                    offset += 12 + rate;
                else
                    offset += 20 + rate;
                break;
        }
    } else {
        switch (channel) {
            case 5180:
                offset = 364 + 53 * 0;
                break;
            case 5200:
                offset = 364 + 53 * 1;
                break;
            case 5220:
                offset = 364 + 53 * 2;
                break;
            case 5240:
                offset = 364 + 53 * 3;
                break;
            case 5260:
                offset = 364 + 53 * 4;
                break;
            case 5280:
                offset = 364 + 53 * 5;
                break;
            case 5300:
                offset = 364 + 53 * 6;
                break;
            case 5320:
                offset = 364 + 53 * 7;
                break;
            case 5500:
                offset = 364 + 53 * 8;
                break;
            case 5520:
                offset = 364 + 53 * 9;
                break;
            case 5540:
                offset = 364 + 53 * 10;
                break;
            case 5560:
                offset = 364 + 53 * 11;
                break;
            case 5580:
                offset = 364 + 53 * 12;
                break;
            case 5600:
                offset = 364 + 53 * 13;
                break;
            case 5620:
                offset = 364 + 53 * 14;
                break;
            case 5640:
                offset = 364 + 53 * 15;
                break;
            case 5660:
                offset = 364 + 53 * 16;
                break;
            case 5680:
                offset = 364 + 53 * 17;
                break;
            case 5700:
                offset = 364 + 53 * 18;
                break;
            case 5720:
                offset = 364 + 53 * 19;
                break;
            case 5745:
                offset = 364 + 53 * 20;
                break;
            case 5765:
                offset = 364 + 53 * 21;
                break;
            case 5785:
                offset = 364 + 53 * 22;
                break;
            case 5805:
                offset = 364 + 53 * 23;
                break;
            case 5825:
                offset = 364 + 53 * 24;
                break;
        }
        switch (mode) {
            case 0:
                offset += rate;
                break;
            case 2:
                if (bw == 1)
                    offset += 8 + rate;
                else
                    offset += 16 + rate;
                break;
            case 4:
                if (bw == 1)
                    offset += 24 + rate;
                else if (bw == 2)
                    offset += 33 + rate;
                else
                    offset += 43 + rate;
                break;
        }
    }
    offset += 2052;
    //get related mtd block
    mtd = get_mtd_device_nm(part);
    if (IS_ERR(mtd))
        return PTR_ERR(mtd);
    buf = siwifi_kmalloc(mtd->erasesize, GFP_KERNEL);
    if (!buf)
        return -ENOMEM;
    mtd_read(mtd, 0, mtd->erasesize, &retlen, buf);
    power = buf[offset];

    put_mtd_device(mtd);
    siwifi_kfree(buf);

    tmp_power = power;
    printk("power in ioctl is %d\r\n", tmp_power);

    iwr->u.data.length = sizeof(power);
    if (copy_to_user(iwr->u.data.pointer, &tmp_power, 4)) {
        printk("return power fail\r\n");
        return -1;
    }

    return 0;
}

static int siwifi_ioctl_do_get_factory_power(struct siwifi_hw *siwifi_hw, struct siwifi_ioctl_cfg *cfg, struct iwreq *iwr)
{
    char *tmp_data = (char *)(&cfg->data[0]);
    //gain_mode:0-normal(from factory) 1-low 2-sleep 3-normal(from siwifi_hw, usually the same as factory)
    int channel, mode, rate, bw, gain_mode, ant;
    int power = 0, channel_number = 0;
    unsigned char *buf = NULL;
    int tmp_power = 0;
    unsigned int offset = 0;

    printk("enter into the ioctl  get gain power\r\n");
    sscanf(tmp_data, "%d %d %d %d %d %d", &channel, &bw, &mode, &rate, &gain_mode, &ant);
    printk("channel:%d, bw:%d, mode:%d, rate:%d, gain_mode:%d, ant:%d\r\n", channel, bw, mode, rate, gain_mode, ant);
    //Call the old interface directly when reading the factory table
    if(gain_mode != 1 && gain_mode != 2 && gain_mode !=3) {
        _siwifi_ioctl_do_get_factory_power(siwifi_hw,cfg,iwr);
        return 0;
    }else{
        //2.4g: 13 channels * 28 rates
        if (channel < 3000) {
            offset = (channel - 2412) / 5 * 28;
            switch (mode) {
                //no_ht
                case 0:
                    offset += rate;
                    break;
                    //ht
                case 2:
                    if (bw == 1)
                        offset += 12 + rate;
                    else
                        offset += 20 + rate;
                    break;
            }
            if (offset > 364) {
                printk("offset(%d) is out of lb range\n",offset);
                return -1;
            }
        } else {
            //5g: 25 channels * 53 rates
            //channel 36~64; total 8
            if (channel < 5321) {
                channel_number = (channel - 5180) / 20;
            }
            //channel 100~144; total 12
            else if (channel < 5721) {
                channel_number = (channel - 5500) / 20 + 8;
            }
            //channel 149~165; total 5
            else if (channel < 5826) {
                channel_number = (channel - 5745) / 20 + 20;
            }
            offset = channel_number * 53;
            switch (mode) {
                //no_ht
                case 0:
                    offset += rate;
                    break;
                    //ht
                case 2:
                    if (bw == 1)
                        offset += 8 + rate;
                    else
                        offset += 16 + rate;
                    break;
                    //vht
                case 4:
                    if (bw == 1)
                        offset += 24 + rate;
                    else if (bw == 2)
                        offset += 33 + rate;
                    else
                        offset += 43 + rate;
                    break;
            }
        }

        if (offset > 1325){
            printk("offset(%d) is out of hb range\n",offset);
            return -1;
        }

        if (gain_mode == 1) {
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
            if (ant == 2)
                buf = siwifi_hw->factory_info.low_txpower_list_second_antenna;
            else
#endif
                buf = siwifi_hw->factory_info.low_txpower_list;
        } else if (gain_mode == 2){
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
            if (ant == 2)
                buf = siwifi_hw->factory_info.sleepmode_txpower_list_second_antenna;
            else
#endif
                buf = siwifi_hw->factory_info.sleepmode_txpower_list;
        } else {
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
            if (ant == 2)
                buf = siwifi_hw->factory_info.normal_txpower_list_second_antenna;
            else
#endif
                buf = siwifi_hw->factory_info.normal_txpower_list;
        }

        if (buf){
            power = buf[offset];
        }
        tmp_power = power;
        printk("power in ioctl is %d\r\n", tmp_power);

        iwr->u.data.length = sizeof(power);
        if (copy_to_user(iwr->u.data.pointer, &tmp_power, 4)) {
            printk("return power fail\r\n");
            return -1;
        }
    }
    return 0;
}

/*func:get factory data form mtd3 and return to ate main.c
 *params:offset
 *return:data
 */

static int siwifi_ioctl_do_get_data_from_factory(struct siwifi_hw *siwifi_hw, struct siwifi_ioctl_cfg *cfg, struct iwreq *iwr)
{
    char part[] = "factory";
    struct mtd_info *mtd;
    unsigned char *buf = NULL;
    int data = 0;
    int retlen;
	int i = 0;
    uint32_t offset = 0;
    char p_offset[4] = "0000";
    memcpy(p_offset,(&cfg->data[0]),4);
	for(i = 0; i < 4; i ++) {
		if(p_offset[i] >= '0' && p_offset[i] <= '9') {
			offset = offset * 10 + (p_offset[i]  - '0');
		}
	}
	if(offset < 0) {
		printk("get data faile\r\n");
		return -1;
	}
    //get related mtd block
    mtd = get_mtd_device_nm(part);
    if (IS_ERR(mtd))
        return PTR_ERR(mtd);
    buf = siwifi_kmalloc(mtd->erasesize, GFP_KERNEL);
    mtd_read(mtd, 0, mtd->erasesize, &retlen, buf);
    data = buf[offset];

    put_mtd_device(mtd);
    siwifi_kfree(buf);

    if (copy_to_user(iwr->u.data.pointer, &data, 4)) {
        printk("return data fail\r\n");
        return -1;
    }

    return 0;
}


/* func: load the setting from &cfg->data, set Mode into struct siwifi_hw->ate_env
 * params: //  enum format_mode {
 NON_HT_MODE = 0,
 NON_HT_DUP_MODE,
 HT_MM_MODE,
 HT_GF_MODE,
 VHT_MODE,
 };
 * return:status
 * */
static int siwifi_ioctl_do_set_phy_mode(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    int tmp_data = 0;
    char *tmp_mod = (char *)(&cfg->data[0]);
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    tmp_data = siwifi_char_to_int(tmp_mod);
#ifdef DEBUG_PRINTK
    printk("^^^^^^^tmp_data : %d\n", tmp_data);
#endif

    if (tmp_data < 0 || (tmp_data > 4)) {
        printk("!!!Mode Error\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }
    siwifi_hw->ate_env.conf.rate.mode = tmp_data;
DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/*
 * func:load the setting from &cfg->data, set rate idx into struct siwifi_hw->ate_env
 * params:// when mode = NON_HT_MODE,
// 2G:rate_idx 0~11 means legency rate 1Mbps~54Mbps
// 5G:rate_idx 0~7 means legency rate 6Mbps~54Mbps
// when mode = HT_MM_MODE,
// rate_idx 0~7 means mcs0~7 for HT Nss=1
// rate 6.5Mbps~65.0Mbps   @non SGI @20MHz BW
// rate 7.2Mbps~72.2Mbps   @use SGI @20MHz BW
// rate 13.5Mbps~135.0Mbps @use SGI @40MHz BW
// rate 15.0Mbps~150.0Mbps @use SGI @40MHz BW
// HT_GF_MODE rate is same with HT_MM_MODE, but can't use SGI
// when mode = VHT_MODE,
// rate_idx 0~8 means mcs0~8 for VHT Nss=1 @20MHz BW
// rate 6.5Mbps~78.0Mbps   @non SGI @20MHz BW
// rate 7.2Mbps~86.7Mbps   @use SGI @20MHz BW
// rate_idx 0~9 means mcs0~9 for VHT Nss=1 @40MHz BW
// rate 13.5Mbps~180.0Mbps @use SGI @40MHz BW
// rate 15.0Mbps~200.0Mbps @use SGI @40MHz BW
// rate_idx 0~9 means mcs0~9 for VHT Nss=1 @80MHz BW
// rate 29.3Mbps~390.0Mbps @use SGI @80MHz BW
// rate 32.5Mbps~433.3Mbps @use SGI @80MHz BW
 * return:status
 * */
static int siwifi_ioctl_do_set_rate(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    int tmp_data = 0;
    char *tmp_rate = (char *)(&cfg->data[0]);
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }
    tmp_data = siwifi_char_to_int(tmp_rate);
#ifdef DEBUG_PRINTK
    printk("^^^^^^^tmp_data : %d\n", tmp_data);
#endif
    if (tmp_data < 0 || (tmp_data > 15)) {
        printk("Rate Error\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }
    siwifi_hw->ate_env.conf.rate.rate_idx = tmp_data;

DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/*
 * func:load the setting from &cfg->data, set short preamble (or not) into struct siwifi_hw->ate_env
 *  // use_short_preamble is only valid @NON_HT_MODE rate 1~3
 * params: 0 , 1
 * return:status
 * */
static int siwifi_ioctl_do_set_preamble(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    int tmp_data = 0;
    char *tmp_pre = (char *)(&cfg->data[0]);
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    tmp_data = siwifi_char_to_int(tmp_pre);
#ifdef DEBUG_PRINTK
    printk("^^^^^^^tmp_data : %d\n", tmp_data);
#endif

    if (tmp_data < 0 || (tmp_data > 1)) {
        printk("!!!short preamble Error\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    siwifi_hw->ate_env.conf.rate.use_short_preamble = tmp_data;
DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/*
 * func:load the setting from &cfg->data, set short GI (or not) into struct siwifi_hw->ate_env
// use_short_gi is only valid in HT_MM_MODE VHT_MODE
 * params:0,1
 * return:status
 * */
static int siwifi_ioctl_do_set_gi(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    int tmp_data = 0;
    char *tmp_gi = (char *)(&cfg->data[0]);
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    tmp_data = siwifi_char_to_int(tmp_gi);
#ifdef DEBUG_PRINTK
    printk("^^^^^^^tmp_data : %d\n", tmp_data);
#endif

    if (tmp_data < 0 || (tmp_data > 1)) {
        printk("!!!Short GI Error\n");
        status = SIWIFI_IOCTL_RET_INVALID_DATA;
        goto DONE;
    }
    siwifi_hw->ate_env.conf.rate.use_short_gi = tmp_data;
DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}
/*function for set tx vector
 *
 *
 */
static int siwifi_set_txvector(uint32_t base_addr,int idx,uint8_t value){

    uint32_t txvec_offset = 0x0200 + 4*idx;
    int status=SIWIFI_IOCTL_RET_SUCCESS;
    REG_PL_WR(base_addr + 0x00160000 + txvec_offset, value);
    return status;
}

/*
 * func:load the setting from &cfg->data, set power into struct siwifi_hw->ate_env
 * params:0~31
 * return:status
 * */
static int siwifi_ioctl_do_set_tx_power(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    int tmp_data = 0;
    char *tmp_pow = (char *)(&cfg->data[0]);
    uint32_t base_addr=WIFI_BASE_ADDR((siwifi_hw->mod_params->is_hb));
    uint8_t power=0x0;
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    tmp_data = siwifi_char_to_int(tmp_pow);
#ifdef DEBUG_PRINTK
    printk("^^^^^^^tmp_data : %d\n", tmp_data);
#endif
    if (tmp_data < 0 || (tmp_data > 0xff)) {
        printk("Power Error\n");
        status = SIWIFI_IOCTL_RET_INVALID_DATA;
        goto DONE;
    }

    /*LMAC use OFDM_MAX_PWR_LEVEL value to config RF register, range is 0~31*/
    siwifi_hw->ate_env.conf.pkg.tx_power = tmp_data << NXMAC_OFDM_MAX_PWR_LEVEL_LSB;
    /*Though LMAC not use DSSS_MAX_PWR_LEVEL value to config RF register,
     *but it really set into MACHW register 0x60B000A0, so ensure that
     *OFDM_MAX_PWR_LEVEL = DSSS_MAX_PWR_LEVEL =  ate_pkg->tx_power*/
    siwifi_hw->ate_env.conf.pkg.tx_power |= tmp_data << NXMAC_DSSS_MAX_PWR_LEVEL_LSB;

    siwifi_hw->ate_env.conf.chandef.chan->max_power = tmp_data;
    siwifi_hw->ate_env.siwifi_vif->txpower = tmp_data;
    siwifi_hw->ate_env.siwifi_vif->txpower_idx = tmp_data;
    /*add  for set tx power during tx*/
    if (siwifi_hw->ate_env.tx_macbypass_start || siwifi_hw->ate_env.tx_frame_start){
        power= tmp_data & 0xff;
        siwifi_set_txvector(base_addr,0,power);
        printk("set tx power during tx : %x\n",power);
    }
DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/*
 * func:the tx frame are configured by userspace, and we ignore all
 *     the other frame setting by cmd group3
 * params:
 * return:
 * */
static int siwifi_ioctl_do_set_whole_frame(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    char *macheadr = (char *)(&cfg->data[0]);
    char *payload = macheadr + 24;
    int length = cfg->length - 24;
#ifdef DEBUG_PRINTK
    printk("%s: cfg->length = %d, length = %d,\n", __func__, cfg->length, length);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    //set tx beacon flag
    siwifi_hw->ate_env.conf.pkg.payload_mode = 1;

    //fill beacon mac header
    siwifi_ioctl_do_ate_whole_frame_machdr(siwifi_hw, macheadr);
    //fill beacon payload
    siwifi_ioctl_do_ate_whole_frame_payload(siwifi_hw, payload, length);

DONE:
    return status;
}

/*
 * func:set the number of TX frame. When the setting of count = 0, it can send frame continuously!
 * params:1~...
 * return:status
 * */
static int siwifi_ioctl_do_set_tx_count(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    int tmp_data = 0;
    char *tmp_cou = (char *)(&cfg->data[0]);
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }
    tmp_data = siwifi_char_to_int(tmp_cou);
#ifdef DEBUG_PRINTK
    printk("^^^^^^^tmp_data : %d\n", tmp_data);
#endif
    if (tmp_data == 0) {
        printk("Number Zero\n");
        //status = SIWIFI_IOCTL_RET_INVALID_DATA;
        //goto DONE;
        siwifi_hw->ate_env.conf.pkg.cont_flag == true;
        tmp_data = 1;
    }
    if (tmp_data < 0) {
        printk("Number Error!\n");
        status = SIWIFI_IOCTL_RET_INVALID_DATA;
        goto DONE;
    }

    siwifi_hw->ate_env.conf.pkg.frame_num = tmp_data;

DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/*
 * func:set the length of TX payload
 * params:11b,a,g:1024,11n,ac:4096,...
 * return:
 * */
static int siwifi_ioctl_do_set_payload_length(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    int tmp_data = 0;
    char *tmp_len = (char *)(&cfg->data[0]);
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    tmp_data = siwifi_char_to_int(tmp_len);
#ifdef DEBUG_PRINTK
    printk("^^^^^^^tmp_data : %d\n", tmp_data);
#endif
    if (tmp_data < 0) {
        printk(" Length Error\n");
        status = SIWIFI_IOCTL_RET_INVALID_DATA;
        goto DONE;
    }
    siwifi_hw->ate_env.conf.pkg.frame_len = tmp_data;

DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

static int siwifi_ioctl_do_set_frame_control(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    char *tmp_pre = (char *)(&cfg->data[0]);
    int tmp_data[] = {0x0080};

    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    if (!cfg->data) {
        printk("freamble not set!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    siwifi_str4_to_int16(tmp_data, tmp_pre);

    siwifi_hw->ate_env.conf.machdr.fc = tmp_data[0];
#ifdef  DEBUG_PRINTK
    printk("%s: == ate_env.conf.machdr.fc = 0x%x\n", __func__, siwifi_hw->ate_env.conf.machdr.fc);
#endif
DONE:
    return status;
}

static int siwifi_ioctl_do_set_duration_id(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    char *tmp_pre = (char *)(&cfg->data[0]);
    int tmp_data[] = {0x0000};

    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    if (!cfg->data) {
        printk("freamble not set!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    siwifi_str4_to_int16(tmp_data, tmp_pre);

    siwifi_hw->ate_env.conf.machdr.dur = tmp_data[0];
#ifdef DEBUG_PRINTK
    printk("%s: == ate_env.conf.machdr.dur = 0x%x\n", __func__, siwifi_hw->ate_env.conf.machdr.dur);
#endif
DONE:
    return status;
}

/*
 * func:set the BSSID
 * params:"XX:XX:XX:XX:XX:XX"
 * return:status
 * */
static int siwifi_ioclt_do_set_tx_bssid(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    char *tmp_bssid = (char *)(&cfg->data[0]);
    static uint8_t siwifi_set_bssid[] = {0x11, 0x66, 0x88, 0xFF, 0xFF, 0xFF};
#ifdef DEBUG_PRINTK
    printk("%s: set tx bssid\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    //change the string to type of mac address. eg:"00:00:00:00:00:00" to {0x00,0x00,0x00,0x00,0x00,0x00}
    siwifi_str_to_str16(siwifi_set_bssid, tmp_bssid);

    memcpy(&siwifi_hw->ate_env.conf.machdr.siwifi_bssid, siwifi_set_bssid, sizeof(siwifi_hw->ate_env.conf.machdr.siwifi_bssid));
    memcpy(&siwifi_hw->ate_env.conf.qos_machdr.siwifi_bssid, siwifi_set_bssid, sizeof(siwifi_hw->ate_env.conf.qos_machdr.siwifi_bssid));
    //printk("hw:~~~0x%02x\n",siwifi_hw->ate_env.conf.add.bssid[0]);
    printk("set_bssid: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n",
            siwifi_set_bssid[0], siwifi_set_bssid[1], siwifi_set_bssid[2],
            siwifi_set_bssid[3], siwifi_set_bssid[4], siwifi_set_bssid[5]);

DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/*
 * func:set the da
 * params:"XX:XX:XX:XX:XX:XX"
 * return:status
 * */
static int siwifi_ioclt_do_set_tx_da(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    char *tmp_da = (char *)(&cfg->data[0]);
    static uint8_t siwifi_set_da[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
#ifdef DEBUG_PRINTK
    printk("%s: set tx da\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    //change the string to type of mac address. eg:"00:00:00:00:00:00" to {0x00,0x00,0x00,0x00,0x00,0x00}
    siwifi_str_to_str16(siwifi_set_da, tmp_da);

    //memcpy(&siwifi_hw->ate_env.conf.add.da,siwifi_set_da,sizeof(siwifi_hw->ate_env.conf.add.da));
    memcpy(&siwifi_hw->ate_env.conf.machdr.siwifi_da, siwifi_set_da, sizeof(siwifi_hw->ate_env.conf.machdr.siwifi_da));
    memcpy(&siwifi_hw->ate_env.conf.qos_machdr.siwifi_da, siwifi_set_da, sizeof(siwifi_hw->ate_env.conf.qos_machdr.siwifi_da));
    printk("set_da: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n",
            siwifi_set_da[0], siwifi_set_da[1], siwifi_set_da[2], siwifi_set_da[3], siwifi_set_da[4], siwifi_set_da[5]);

DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/*
 * func:set the sa
 * params:"XX:XX:XX:XX:XX:XX"
 * return:status
 * */
static int siwifi_ioclt_do_set_tx_sa(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    char *tmp_sa = (char *)(&cfg->data[0]);
    static uint8_t siwifi_set_sa[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
#ifdef DEBUG_PRINTK
    printk("%s: set tx sa\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    //change the string to type of mac address. eg:"00:00:00:00:00:00" to {0x00,0x00,0x00,0x00,0x00,0x00}
    siwifi_str_to_str16(siwifi_set_sa, tmp_sa);

    //memcpy(&siwifi_hw->ate_env.conf.add.sa,siwifi_set_sa,sizeof(siwifi_hw->ate_env.conf.add.sa));
    memcpy(&siwifi_hw->ate_env.conf.machdr.siwifi_sa, siwifi_set_sa, sizeof(siwifi_hw->ate_env.conf.machdr.siwifi_sa));
    memcpy(&siwifi_hw->ate_env.conf.qos_machdr.siwifi_sa, siwifi_set_sa, sizeof(siwifi_hw->ate_env.conf.qos_machdr.siwifi_sa));
    printk("set_sa: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n",
            siwifi_set_sa[0], siwifi_set_sa[1], siwifi_set_sa[2], siwifi_set_sa[3], siwifi_set_sa[4], siwifi_set_sa[5]);

DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

static int siwifi_ioctl_do_set_seq_ctrl(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    char *tmp_pre = (char *)(&cfg->data[0]);
    int tmp_data[] = {0x5e20};

    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    if (!cfg->data) {
        printk("freamble not set!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    siwifi_str4_to_int16(tmp_data, tmp_pre);

    siwifi_hw->ate_env.conf.machdr.seqc = tmp_data[0];
#ifdef DEBUG_PRINTK
    printk("%s: == ate_env.conf.machdr.seqc = 0x%x\n", __func__, siwifi_hw->ate_env.conf.machdr.seqc);
#endif
DONE:
    return status;
}

/*
 * func:save the value to mtd block3.
 * params:
 * return:0 success, otherwise failure
 * */
static int siwifi_ioctl_do_save_to_mtd(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    int save_xo = 1;
    int from = 2048 + 2;
    int xo_config_offset = 2048;
    unsigned char *buf = NULL;
    unsigned char data[4] = {0};
    struct erase_info ei;
    struct mtd_info *mtd = NULL;
    const char part[] = "factory";
    size_t retlen;
    uint8_t value_1 = 0;
    uint8_t value_2 = 0;
    uint8_t size = 0;
    uint8_t offset = 0;
    char *tmp_data = (char *)(&cfg->data[2]);
    char *tmp_datapower = (char *)(&cfg->data[1]);
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif

    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        siwifi_ioctl_response_to_user(cfg, iwr, SIWIFI_IOCTL_RET_FAILURE);
        return SIWIFI_IOCTL_RET_FAILURE;
    }

    if (cfg->data[0] == '1') {
        if (tmp_datapower[0] == '0') {
            memcpy(data, &tmp_datapower[1], 1);
        } else {
            memcpy(data, &tmp_datapower[0], 2);
        }
        data[3] = '\0';
        if (kstrtou8(data, 10, &value_2))
            printk("[%d]kstrtou8 error\n", __LINE__);
        save_xo = 0;
        if (kstrtoint(&tmp_datapower[3], 10, &from))
            printk("[%d]kstrtoint error\n", __LINE__);
        printk("from:%d\n", from);
    } else {                      //save config
        size = strlen(cfg->data); //size=5:0x404; size=6:0x0404;
        if (size == 6)
            offset = 2;
        else if (size == 5)
            offset = 1;
        else if (size == 4 || size == 3)
            offset = 0;
        else {
            printk("error input data!");
            siwifi_ioctl_response_to_user(cfg, iwr, SIWIFI_IOCTL_RET_INVALID_DATA);
            return SIWIFI_IOCTL_RET_INVALID_DATA;
        }

        if (size > 4) {
            memcpy(data, tmp_data, offset);
            data[offset] = '\0';
            if (kstrtou8(data, 16, &value_1))
                printk("[%d]kstrtou8 error\n", __LINE__);
        } else {
            value_1 = 0;
        }
        memcpy(data, &tmp_data[offset], 2);
        data[2] = '\0';
        if (kstrtou8(data, 16, &value_2))
            printk("[%d]kstrtou8 error\n", __LINE__);
    }
    printk("value_1 is 0x%x,value_2 is 0x%x,size:%d\n", value_1, value_2, size);
    //get related mtd block
    mtd = get_mtd_device_nm(part);
    if (IS_ERR(mtd))
        return PTR_ERR(mtd);
    buf = siwifi_kmalloc(mtd->erasesize, GFP_KERNEL);
    if (!buf)
        return -ENOMEM;
    mtd_read(mtd, 0, mtd->erasesize, &retlen, buf);

    ei.mtd = mtd;
    ei.callback = NULL;
    ei.addr = 0;
    ei.len = mtd->erasesize;
    ei.priv = 0;
    mtd_erase(mtd, &ei);

    if (!((buf[xo_config_offset] == 'X' && buf[xo_config_offset + 1] == 'O') || (buf[xo_config_offset] == 'V' && buf[xo_config_offset + 1] == '2')||(buf[xo_config_offset] == 'V' && buf[xo_config_offset + 1] == '3')||(buf[xo_config_offset] == 'V' && buf[xo_config_offset + 1] == '4'))) {
        buf[xo_config_offset] = 'X';
        buf[xo_config_offset + 1] = 'O';
    }
    if (save_xo){
        buf[from] = value_1;
        buf[from+1] = value_2;
    }
    if (!save_xo)
        buf[from] = value_2;
    printk("from is :%d", from);
    //write the config to mtd clock
    if (mtd_write(mtd, 0, mtd->erasesize, &retlen, buf)) {
        printk("get config from mtd error\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
    }
    put_mtd_device(mtd);

    siwifi_kfree(buf);

    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/*
 * func:get all set info
 * params:
 * return:status
 * */
static int siwifi_ioctl_do_get_info(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    struct siwifi_ioctl_ate_dump_info *user;
    //initialisation
    user = siwifi_kmalloc(sizeof(struct siwifi_ioctl_ate_dump_info), GFP_KERNEL);

    if (!user) {
        printk("oom happen!\n");
        return -ENOMEM;
    }
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }
    //TX
    user->bandwidth = siwifi_hw->ate_env.conf.chandef.width;
    user->frame_bandwidth = siwifi_hw->ate_env.conf.pkg.frame_bw;
    user->band = siwifi_hw->ate_env.conf.chandef.chan->band;
    user->freq = siwifi_hw->ate_env.conf.chandef.chan->center_freq;
    user->freq1 = siwifi_hw->ate_env.conf.chandef.center_freq1;
    user->rate = siwifi_hw->ate_env.conf.rate.rate_idx;
    user->mode = siwifi_hw->ate_env.conf.rate.mode;
    user->gi = siwifi_hw->ate_env.conf.rate.use_short_gi;
    user->pre = siwifi_hw->ate_env.conf.rate.use_short_preamble;
    user->power = siwifi_hw->ate_env.conf.pkg.tx_power;
    user->len = siwifi_hw->ate_env.conf.pkg.frame_len;
    user->count = siwifi_hw->ate_env.tx_total_count;
    user->tx_cont = siwifi_hw->ate_env.tx_count;
    user->tx_successful = siwifi_hw->ate_env.tx_successful_count;
    user->tx_retry = siwifi_hw->ate_env.tx_retry_count;
    memcpy(&user->bssid, &siwifi_hw->ate_env.conf.machdr.siwifi_bssid, sizeof(user->bssid));
    memcpy(&user->da, &siwifi_hw->ate_env.conf.machdr.siwifi_da, sizeof(user->da));
    memcpy(&user->sa, &siwifi_hw->ate_env.conf.machdr.siwifi_sa, sizeof(user->sa));
    //RX
    if ((siwifi_hw->ate_env.rx_frame_start) || (siwifi_hw->ate_env.rx_macbypass_start)) {
        siwifi_ate_rx_cb(siwifi_hw);
    }
    //  user->per = siwifi_hw->ate_env.per;
    user->fcs_err = siwifi_hw->ate_env.fcs_err;
    user->rec_rx_count = siwifi_hw->ate_env.rec_rx_count;
    user->phy_err = siwifi_hw->ate_env.phy_err;
    user->fcs_ok=siwifi_hw->ate_env.fcs_ok;
    user->rssi = siwifi_hw->ate_env.rssi;
    user->fcs_ok_for_macaddr=siwifi_hw->ate_env.fcs_ok_for_macaddr;
	user->fcs_group = siwifi_hw->ate_env.fcs_group;

    user->reg_val = siwifi_hw->ate_env.reg.read_reg_val;
    user->reg_status = siwifi_hw->ate_env.reg.status;
    if (siwifi_response_info_to_user(user, iwr)) {
        printk("siwifi_ioctl_response_to_user() fail!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
    }

DONE:
    siwifi_kfree(user);
    return status;
}

/*
 * func:read val from the register,and response to user.
 * params:,type,addr
 * return:status
 * */
static int siwifi_ioctl_do_read_reg(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    char *tmp_addr = (char *)(&cfg->data[0]);
    unsigned int siwifi_reg_addr = 0x60B00000;
    unsigned int reg_val = 0x16A18ABC;
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    //check the input value
    if ((strlen(tmp_addr) != 10) && (strlen(tmp_addr) != 6)) {
        status = SIWIFI_IOCTL_RET_INVALID_DATA;
        printk("Invalid length(%d)of register addr,must be 0x00000000 or 0x0000\n", strlen(tmp_addr));
        goto DONE;
    }

    siwifi_reg_addr = siwifi_str_to_int16((tmp_addr + 2),strlen(tmp_addr +2));
    printk("***read:siwifi_reg_addr = 0x%x\n", siwifi_reg_addr);
    if (!siwifi_reg_addr) {
        status = SIWIFI_IOCTL_RET_INVALID_DATA;
        printk("Invalid register addr\n");
        goto DONE;
    }
    if (siwifi_reg_addr >> 16) {
        reg_val = REG_PL_RD(siwifi_reg_addr);
    } else {
        reg_val = REG_PL_RD16(0xB7A00000 + (siwifi_reg_addr << 1));
        if ((siwifi_reg_addr >= 0x1300) && (siwifi_reg_addr <= 0x137F)) {
            udelay(1);
            reg_val = REG_PL_RD16(0xB7A00000 + (0x13F1 << 1));
        }
    }
    if (reg_val == 0x16A18ABC) {
        siwifi_hw->ate_env.reg.status = 1;
        printk("read val error!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
    } else {
        printk("***read :reg_val = 0x%x,status = %d\n",
                reg_val, siwifi_hw->ate_env.reg.status);
        siwifi_hw->ate_env.reg.read_reg_val = reg_val;
    }
DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/*
 * func:write val into the register.
 * params:addr,val
 * return:status
 * */
static int siwifi_ioctl_do_write_reg(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    char tmp_addr[9] = {0};
    char tmp_val[9] = {0};
    unsigned int siwifi_reg_addr = 0x60B00000;
    unsigned int siwifi_reg_val = 0x00000000;
    uint16_t check_val = 0;
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }
    //check input
    if (strlen(cfg->data) >= 14 && strlen(cfg->data) <= 21) {
        strncpy(tmp_addr, &(cfg->data[2]), 8);
        strncpy(tmp_val, &(cfg->data[13]), 8);
    } else if (strlen(cfg->data) >= 10 && strlen(cfg->data) <= 13) {
        strncpy(tmp_addr, &(cfg->data[2]), 4);
        strncpy(tmp_val, &(cfg->data[9]), 4);
    } else {
        status = SIWIFI_IOCTL_RET_INVALID_DATA;
        printk("Invalid length(%d) of input,must be 0x00000000 0x00000000 or 0x0000 0x0000\n", strlen(cfg->data));
        goto DONE;
    }
    siwifi_reg_addr = siwifi_str_to_int16(tmp_addr,strlen((char *)tmp_addr));
    siwifi_reg_val = siwifi_str_to_int16(tmp_val,strlen((char *)tmp_val));

    printk("write_reg:siwifi_reg_addr=0x%x;siwifi_reg_val=0x%x\n", siwifi_reg_addr, siwifi_reg_val);

    if (!siwifi_reg_addr) {
        status = SIWIFI_IOCTL_RET_INVALID_DATA;
        printk("Invalid register addr\n");
        goto DONE;
    }
    if (siwifi_reg_addr >> 16) {
        REG_PL_WR(siwifi_reg_addr, siwifi_reg_val);
        if (REG_PL_RD(siwifi_reg_addr) != siwifi_reg_val) {
            status = SIWIFI_IOCTL_RET_FAILURE;
            printk("Maybe write reg error!\n");
        }
    } else {
        siwifi_ml_apb_write(siwifi_reg_addr, siwifi_reg_val);
        check_val = siwifi_ml_apb_read(siwifi_reg_addr);

        if (check_val != siwifi_reg_val) {
            status = SIWIFI_IOCTL_RET_FAILURE;
            printk("Write reg error!\n");
        }
    }
DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/* func:set the content of payload
 * params:
 * return:status
 * */
static int siwifi_ioctl_do_set_payload(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    char *tmp_payload = (char *)(&cfg->data[0]);
    uint8_t payload[sizeof(tmp_payload) / 2 - 1] = "0";
#ifdef DEBUG_PRINTK
    printk("%s: fill payload\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    siwifi_str_to_twoint16(payload, tmp_payload);
    printk("payload:0x%02x\n", payload[0]);
    siwifi_hw->ate_env.conf.pkg.payload_mode = 1;
    memcpy(&siwifi_hw->ate_env.conf.pkg.payload, payload, sizeof(siwifi_hw->ate_env.conf.pkg.payload));

DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}
/*function
 * prepare tx vector*/
static int siwifi_prepare_txvector(struct siwifi_hw *siwifi_hw,uint8_t *txvec){


    int status;
    txvec[0]=txvector.tx_power =siwifi_hw->ate_env.siwifi_vif->txpower;

    if (siwifi_hw->ate_env.conf.pkg.frame_bw>0){
        txvector.chbw=siwifi_hw->ate_env.conf.pkg.frame_bw-1;
    }
    else{
        txvector.chbw=siwifi_hw->ate_env.conf.pkg.frame_bw;
    }
    txvec[1]=(txvector.sounding) | (txvector.feccoding << 1 ) | (txvector.smoothing << 3 ) | (txvector.chbw << 6 );
    txvec[2]=txvector.antennaSet;
    if (txvector.beamformed){
        txvec[3]=txvector.smmindex;
    }
    else{
        txvec[3]=0;
    }
    txvector.mcs=siwifi_hw->ate_env.conf.rate.rate_idx & 0xFF;
    txvec[4]=(txvector.mcs ) |(txvector.pretype << 7);

    txvector.formatmod =siwifi_hw->ate_env.conf.rate.mode & 0xFF;
    txvec[5]=(txvector.formatmod)|(txvector.numExtnSS <<3) |(txvector.nsts <<5);

    txvector.legLength=siwifi_hw->ate_env.conf.pkg.frame_len &0xFF;
    txvec[6]=txvector.legLength;

    txvector.leglength2=(siwifi_hw->ate_env.conf.pkg.frame_len >>8) & 0xF;
    if (siwifi_hw->ate_env.conf.rate.mode == 0){
        if (siwifi_hw->ate_env.conf.chandef.chan->band){
            siwifi_hw->ate_env.conf.rate.rate_idx = siwifi_hw->ate_env.conf.rate.rate_idx + 4;
        }
        switch(siwifi_hw->ate_env.conf.rate.rate_idx){
            case 0:
            case 1:
            case 2:
            case 3:
                txvector.legrate=siwifi_hw->ate_env.conf.rate.rate_idx;
                break;
            case 4:
                txvector.legrate=0xb;
                break;
            case 5:
                txvector.legrate=0xf;
                break;
            case 6:
                txvector.legrate=0xa;
                break;
            case 7:
                txvector.legrate=0xe;
                break;
            case 8:
                txvector.legrate=0x9;
                break;
            case 9:
                txvector.legrate=0xd;
                break;
            case 10:
                txvector.legrate=0x8;
                break;
            case 11:
                txvector.legrate=0xc;
                break;
            default :
                printk(" rate is wrong,please check your setting");
                return status = SIWIFI_IOCTL_RET_FAILURE;
        }
    }
    else{
        txvector.legrate=0xb;//when mode is ht or vht ,setting the legrate filed 4b'1011;
    }
    txvec[7]=txvector.leglength2 | (txvector.legrate <<4);
    txvec[8]=txvector.service;
    txvec[9]=txvector.service2;

    txvector.htlength1 = siwifi_hw->ate_env.conf.pkg.frame_len & 0xFF;
    txvec[10]=txvector.htlength1;

    txvector.htlength2 = (siwifi_hw->ate_env.conf.pkg.frame_len>>8) & 0xFF;
    txvec[11]=txvector.htlength2;

    txvector.htlength3 =(siwifi_hw->ate_env.conf.pkg.frame_len>>16) & 0xF;
    txvector.shortgi=siwifi_hw->ate_env.conf.rate.use_short_gi;
    txvec[12]=txvector.htlength3 | (txvector.shortgi << 6)|(txvector.aggregation<<4);

    txvec[13]=txvector.ntx | (txvector.stbc <<3) | (txvector.beamformed <<7);

    txvec[14]=txvector.partialid;

    txvec[15]=txvector.partialid1 | txvector.grounpid << 1;
    return status = SIWIFI_IOCTL_RET_SUCCESS;
}

/* func:sent tx continuously through macbypass
 * params:
 * return:status
 * */
#define SFAX8_THERMAL_DIGGAINMASK 0xE0
#define SFAX8_THERMAL_POWERMASK 0x1F
#define SFAX8_THERMAL_POWERMAX_FOR_SINGLE 31
#define SFAX8_THERMAL_POWERMAX_FOR_DUAL 15
#define SFAX8_THERMAL_POWERMIN_1  0


static int siwifi_ioctl_do_ate_macbypass_tx_start(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    uint32_t base_addr = 0xB1000000;
    struct cfg80211_chan_def *chandef;
    uint8_t txvec[16] = {0};
    uint32_t txvec_idx_max = 15;
    uint32_t txvec_idx=0;
    uint32_t delay =1000;
    struct mm_set_channel_cfm cfm;
#ifdef CONFIG_SIWIFI_COOLING_TEMP
    uint32_t temp_disable = siwifi_hw->temp_disable;
    int txpower_tmp=0;
    int change_power= siwifi_hw->change_power;
    int change_power_trend = siwifi_hw->change_power_trend;
#endif
#ifdef CONFIG_SIWIFI_COOLING_TEMP
    int SFAX8_THERMAL_POWERMAX = SFAX8_THERMAL_POWERMAX_FOR_SINGLE;
    int SFAX8_THERMAL_POWERMIN = SFAX8_THERMAL_POWERMIN_1;
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
    struct phy_aetnensis_cfg_tag *phy_tag;
    phy_tag=(struct phy_aetnensis_cfg_tag *)&siwifi_hw->phy_config;
    if ((phy_tag->flag & DUAL_ANTENNA_CALIBRATE)){
	    SFAX8_THERMAL_POWERMAX = SFAX8_THERMAL_POWERMAX_FOR_DUAL;
    }
#endif
#endif
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    chandef = &siwifi_hw->ate_env.conf.chandef;

    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    if (siwifi_hw->ate_env.tx_frame_start || siwifi_hw->ate_env.tx_macbypass_start) {
        printk("another tx frame has started! you should stop tx first!!!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    if(siwifi_send_set_channel_for_macbypass_tx(siwifi_hw, 0, &cfm, chandef))
        return -EIO;

#ifdef CONFIG_SFA28_FULLMASK
    base_addr=WIFI_BASE_ADDR(siwifi_hw->mod_params->is_hb);
    mdelay(100);
    REG_PL_WR(MACBYP_CLKEN_ADDR(siwifi_hw->mod_params->is_hb),1);
    REG_PL_WR(MACBYP_CTRL_ADDR(siwifi_hw->mod_params->is_hb), 0);               //macbyp_ctrl_set(0);
    REG_PL_WR(REG_MDM_CFG_BASE_ADDR(siwifi_hw->mod_params->is_hb) + 0x0888, 0x1111); //mdm_swreset_set(0x1111);
    udelay(100);
    REG_PL_WR(REG_MDM_CFG_BASE_ADDR(siwifi_hw->mod_params->is_hb) + 0x0888, 0); //mdm_swreset_set(0);

    if (siwifi_hw->ate_env.conf.pkg.macbypass_interval != 0)
        delay = siwifi_hw->ate_env.conf.pkg.macbypass_interval;
    REG_PL_WR(MACBYP_INTERFRAME_DELAY_ADDR(siwifi_hw->mod_params->is_hb), delay);  //set tx delay between frame 1/clk*interval

    //setting txvector0a
    siwifi_prepare_txvector(siwifi_hw,txvec);
    for (txvec_idx=0;txvec_idx<txvec_idx_max;txvec_idx ++){
        siwifi_set_txvector(base_addr,txvec_idx,txvec[txvec_idx]);
        printk("txvector[%d]=%x\n",txvec_idx,txvec[txvec_idx]);
    }
    //	siwifi_dynamic_adjustment_digital_gian(siwifi_hw);

#ifdef CONFIG_SIWIFI_COOLING_TEMP
    txpower_tmp = txvec[0];
    if(!temp_disable){
        txpower_tmp = txvec[0] & SFAX8_THERMAL_POWERMASK;
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

        }
        else{
            if((siwifi_hw->ate_env.siwifi_vif->txpower_idx & SFAX8_THERMAL_DIGGAINMASK) == 0)
            {
                if(change_power_trend  >= 0)
                    txpower_tmp += (change_power & SFAX8_THERMAL_POWERMASK);
                else
                    txpower_tmp -= ((change_power & SFAX8_THERMAL_POWERMASK) - 1);
            }
            else
            {
                if(change_power_trend  >= 0)
                    txpower_tmp += ((change_power & SFAX8_THERMAL_POWERMASK) - 1);
                else
                    txpower_tmp -= (change_power & SFAX8_THERMAL_POWERMASK);
            }

            if (txpower_tmp > SFAX8_THERMAL_POWERMAX )
                txpower_tmp = SFAX8_THERMAL_POWERMAX;
            else if (txpower_tmp < SFAX8_THERMAL_POWERMIN)
                txpower_tmp = SFAX8_THERMAL_POWERMIN;
            txpower_tmp = txpower_tmp^0x80^(siwifi_hw->ate_env.siwifi_vif->txpower_idx & ~SFAX8_THERMAL_POWERMASK);
        }
    }
    REG_PL_WR(MACBYP_TXV0_ADDR(siwifi_hw->mod_params->is_hb), txpower_tmp);
    printk("change_power :%x\n trend=%d txpower_tmp %d\n",siwifi_hw->change_power,change_power_trend,txpower_tmp);
#else
    REG_PL_WR(MACBYP_TXV0_ADDR(siwifi_hw->mod_params->is_hb), txvec[0]);
#endif
    REG_PL_WR(MACBYP_CTRL_ADDR(siwifi_hw->mod_params->is_hb), 0x300);        //macbyp_ctrl_set(0x300);
    udelay(100);
    REG_PL_WR(MACBYP_CTRL_ADDR(siwifi_hw->mod_params->is_hb), 0x301); //macbyp_ctrl_set(0x301);
    siwifi_hw->ate_env.tx_macbypass_start = true;
#else
    //step5: do test
    mdelay(100);
    if (chandef->chan->band) {
        base_addr = 0xB1400000;
    }

    REG_PL_WR(base_addr + 0x00160000, 0);               //macbyp_ctrl_set(0);
    REG_PL_WR(base_addr + 0x00100000 + 0x0888, 0x1111); //mdm_swreset_set(0x1111);
    udelay(100);
    REG_PL_WR(base_addr + 0x00100000 + 0x0888, 0);                                  //mdm_swreset_set(0);
    REG_PL_WR(base_addr + 0x00160000 + 0x0048, 0x300);                              //macbyp_pre_tx_delay_set(0x300);
    REG_PL_WR(base_addr + 0x00160000 + 0x0200, siwifi_hw->ate_env.siwifi_vif->txpower); //macbyp_txv0_set(pow);
    switch (siwifi_hw->ate_env.conf.pkg.frame_bw) {
        case 0:
            REG_PL_WR(base_addr + 0x00160000 + 0x0204, 0x00); //macbyp_txv1_set(0x00);//20M:0x00;40M:0x40;80M:0x80
            break;
        case 1:
            REG_PL_WR(base_addr + 0x00160000 + 0x0204, 0x00); //macbyp_txv1_set(0x00);//20M:0x00;40M:0x40;80M:0x80
            break;
        case 2:
            REG_PL_WR(base_addr + 0x00160000 + 0x0204, 0x40); //macbyp_txv1_set(0x00);//20M:0x00;40M:0x40;80M:0x80
            break;
        case 3:
            REG_PL_WR(base_addr + 0x00160000 + 0x0204, 0x80); //macbyp_txv1_set(0x00);//20M:0x00;40M:0x40;80M:0x80
            break;
        default:
            status = SIWIFI_IOCTL_RET_FAILURE;
            break;
    }
    REG_PL_WR(base_addr + 0x00160000 + 0x0208, 0x3);                                        //macbyp_txv2_set(0x3);
    REG_PL_WR(base_addr + 0x00160000 + 0x020C, 0);                                          //macbyp_txv3_set(0x0);
    REG_PL_WR(base_addr + 0x00160000 + 0x0210, siwifi_hw->ate_env.conf.rate.rate_idx);        //macbyp_txv4_set(0x7);//mcs
    REG_PL_WR(base_addr + 0x00160000 + 0x0214, siwifi_hw->ate_env.conf.rate.mode);            //macbyp_txv5_set(0x2);//NO-HT:0x0;HT-MM:0x2;VHT:0x4
    REG_PL_WR(base_addr + 0x00160000 + 0x0218, 0xff & siwifi_hw->ate_env.conf.pkg.frame_len); //macbyp_txv6_set(0xf0);
    if (base_addr == 0xB1000000) {
        if (siwifi_hw->ate_env.conf.rate.rate_idx <= 3) {
            REG_PL_WR(base_addr + 0x00160000 + 0x021C, ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8) | (siwifi_hw->ate_env.conf.rate.rate_idx << 4)); //macbyp_txv7_set(0xcf);//54Mbps//0xcf
        } else {
            switch (siwifi_hw->ate_env.conf.rate.rate_idx) {
                case 4:
                    REG_PL_WR(base_addr + 0x00160000 + 0x021C, 0xb0 | ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8));
                    break;
                case 5:
                    REG_PL_WR(base_addr + 0x00160000 + 0x021C, 0xf0 | ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8));
                    break;
                case 6:
                    REG_PL_WR(base_addr + 0x00160000 + 0x021C, 0xa0 | ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8));
                    break;
                case 7:
                    REG_PL_WR(base_addr + 0x00160000 + 0x021C, 0xe0 | ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8));
                    break;
                case 8:
                    REG_PL_WR(base_addr + 0x00160000 + 0x021C, 0x90 | ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8));
                    break;
                case 9:
                    REG_PL_WR(base_addr + 0x00160000 + 0x021C, 0xd0 | ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8));
                    break;
                case 10:
                    REG_PL_WR(base_addr + 0x00160000 + 0x021C, 0x80 | ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8));
                    break;
                case 11:
                    REG_PL_WR(base_addr + 0x00160000 + 0x021C, 0xc0 | ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8));
                    break;
                default:
                    printk("2.4G rate error\n");
                    status = SIWIFI_IOCTL_RET_FAILURE;
                    goto DONE;
            }
        }
    } else {
        switch (siwifi_hw->ate_env.conf.rate.rate_idx) {
            case 0:
                REG_PL_WR(base_addr + 0x00160000 + 0x021C, 0xb0 | ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8));
                break;
            case 1:
                REG_PL_WR(base_addr + 0x00160000 + 0x021C, 0xf0 | ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8));
                break;
            case 2:
                REG_PL_WR(base_addr + 0x00160000 + 0x021C, 0xa0 | ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8));
                break;
            case 3:
                REG_PL_WR(base_addr + 0x00160000 + 0x021C, 0xe0 | ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8));
                break;
            case 4:
                REG_PL_WR(base_addr + 0x00160000 + 0x021C, 0x90 | ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8));
                break;
            case 5:
                REG_PL_WR(base_addr + 0x00160000 + 0x021C, 0xd0 | ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8));
                break;
            case 6:
                REG_PL_WR(base_addr + 0x00160000 + 0x021C, 0x80 | ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8));
                break;
            case 7:
                REG_PL_WR(base_addr + 0x00160000 + 0x021C, 0xc0 | ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8));
                break;
            default:
                REG_PL_WR(base_addr + 0x00160000 + 0x021C, 0xc0 | ((0x0f00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8));
                break;
        }
    }
    REG_PL_WR(base_addr + 0x00160000 + 0x0220, 0);                                                                                                                 //macbyp_txv8_set(0x00);
    REG_PL_WR(base_addr + 0x00160000 + 0x0224, 0);                                                                                                                 //macbyp_txv9_set(0x00);
    REG_PL_WR(base_addr + 0x00160000 + 0x0228, 0xff & siwifi_hw->ate_env.conf.pkg.frame_len);                                                                        //macbyp_txv10_set(0xf0);//HTlength
    REG_PL_WR(base_addr + 0x00160000 + 0x022C, (0xff00 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 8);                                                               //macbyp_txv11_set(0xf0);//HTlength
    REG_PL_WR(base_addr + 0x00160000 + 0x0230, ((0xf0000 & siwifi_hw->ate_env.conf.pkg.frame_len) >> 16) | (0x40 & (siwifi_hw->ate_env.conf.rate.use_short_gi << 6))); //macbyp_txv12_set(0x00);//VHTlength
    REG_PL_WR(base_addr + 0x00160000 + 0x0234, 0x1);                                                                                                               //macbyp_txv13_set(0x1);
    REG_PL_WR(base_addr + 0x00160000 + 0x0238, 0x0);                                                                                                               //macbyp_txv14_set(0x0);
    REG_PL_WR(base_addr + 0x00160000 + 0x023C, 0x0);                                                                                                               //macbyp_txv14_set(0x0);
    REG_PL_WR(base_addr + 0x00160000, 0x300);                                                                                                                      //macbyp_ctrl_set(0x300);
    udelay(100);
    REG_PL_WR(base_addr + 0x00160000, 0x301); //macbyp_ctrl_set(0x301);
#endif
DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/* func:stop tx continuously through macbypass
 * params:
 * return:status
 * */
static int siwifi_ioctl_do_ate_macbypass_tx_stop(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    //uint32_t base_addr = 0xB1000000;
    struct cfg80211_chan_def *chandef;
    chandef = &siwifi_hw->ate_env.conf.chandef;
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }
#ifdef CONFIG_SFA28_FULLMASK
    //base_addr=WIFI_BASE_ADDR;
    REG_PL_WR(MACBYP_CTRL_ADDR(siwifi_hw->mod_params->is_hb), 0);
    //reset sw
    REG_PL_WR(REG_MACBYPASS_BASE_ADDR(siwifi_hw->mod_params->is_hb) + 0x0888, 0x1111); //mdm_swreset_set(0x1111);
    udelay(100);
    REG_PL_WR(REG_MACBYPASS_BASE_ADDR(siwifi_hw->mod_params->is_hb) + 0x0888, 0); //mdm_swreset_set(0)
#else
    if (chandef->chan->band)
        base_addr = 0xB1400000;

    REG_PL_WR(base_addr + 0x00160000, 0);               //macbyp_ctrl_set(0);
    REG_PL_WR(base_addr + 0x00100000 + 0x0888, 0x1111); //mdm_swreset_set(0x1111);
    udelay(100);
    REG_PL_WR(base_addr + 0x00100000 + 0x0888, 0); //mdm_swreset_set(0);
#endif
    //	if (siwifi_hw->ate_env.vif_ctx_flag) {
    //		struct me_ate_tools_op_cfm op_cfm;
    //		if (siwifi_send_me_ate_tools_op(siwifi_hw, NULL, &op_cfm, DELETE_CHANTX_OP)) {
    //			printk("can not remove channel context!\n");
    //			goto DONE;
    //		}
    //		siwifi_hw->ate_env.vif_ctx_flag = false;
    //	}

    //	if (chandef->chan->band) {
    //		//switch mode to rx
    //	} else {
    //		//switch mode to rx
    //	}
    siwifi_hw->ate_env.tx_macbypass_start = false;
DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/*FUNCTION RX BY MACBYPASS
 * PARAM:
 * RETURN :STATUS
 * */
static int siwifi_ioctl_do_ate_macbypass_rx_start(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    struct cfg80211_chan_def *chandef;
    chandef = &siwifi_hw->ate_env.conf.chandef;
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    if (siwifi_hw->ate_env.tx_frame_start) {
        printk("another tx frame has started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    if (siwifi_build_trx_test_environment(siwifi_hw)) {
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    mdelay(100);
    siwifi_hw->ate_env.rec_rx_count = 0;
    siwifi_hw->ate_env.all_err = 0;
    siwifi_hw->ate_env.phy_err = 0;
    siwifi_hw->ate_env.per = 0;
    siwifi_hw->ate_env.fcs_err = 0;

    REG_PL_WR(MACBYP_CLKEN_ADDR(siwifi_hw->mod_params->is_hb),1);
    REG_PL_WR(MACBYP_TRIGGER_ADDR(siwifi_hw->mod_params->is_hb), 0x00000012); //macbyp_trigger_set(0x00000012);
    udelay(10);
    REG_PL_WR(MACBYP_CTRL_ADDR(siwifi_hw->mod_params->is_hb), 0x10); //macbyp_ctrl_set(0x10);
    udelay(10);
    REG_PL_WR(MACBYP_CTRL_ADDR(siwifi_hw->mod_params->is_hb), 0x102); //macbyp_ctrl_set(0x101);
    mdelay(10);
    siwifi_hw->ate_env.rx_macbypass_start = 1;
DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;

}


/* func:stop Rx  macbypass
 * params:
 * return:status
 * */
static int siwifi_ioctl_do_ate_macbypass_rx_stop(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    struct cfg80211_chan_def *chandef;
    chandef = &siwifi_hw->ate_env.conf.chandef;
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }



    if (!siwifi_hw->ate_env.rx_macbypass_start) {
        printk("rx macbypass do not start\n");
    } else {
        REG_PL_WR(MACBYP_CTRL_ADDR(siwifi_hw->mod_params->is_hb), 0);            //macbyp_ctrl_set(0);
        REG_PL_WR(REG_MDM_CFG_BASE_ADDR(siwifi_hw->mod_params->is_hb) + 0x0888, 0x1); //mdm_swreset_set(0x1);
        udelay(100);
        REG_PL_WR(REG_MDM_CFG_BASE_ADDR(siwifi_hw->mod_params->is_hb) + 0x0888, 0); //mdm_swreset_set(0);
        siwifi_hw->ate_env.fcs_err = 0;
        siwifi_hw->ate_env.phy_err = 0;
        siwifi_hw->ate_env.all_err = 0;
        siwifi_hw->ate_env.rec_rx_count = 0;
        siwifi_hw->ate_env.fcs_ok = 0;
    }
    siwifi_hw->ate_env.rx_macbypass_start = false;

    if (siwifi_hw->ate_env.vif_ctx_flag) {
        struct me_ate_tools_op_cfm op_cfm;
        if (siwifi_send_me_ate_tools_op(siwifi_hw, NULL, &op_cfm, DELETE_CHANTX_OP)) {
            printk("can not remove channel context!\n");
            goto DONE;
        }
        siwifi_hw->ate_env.vif_ctx_flag = false;
    }

    if (chandef->chan->band) {
        //switch mode to rx
    } else {
        //switch mode to rx
    }
    siwifi_hw->ate_env.rx_macbypass_start = false;

DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}



#ifdef CONFIG_SFA28_FULLMASK
extern int8_t rf_set_test_dc(bool band,uint8_t path,bool status,uint16_t dcre,uint16_t dcim);
#endif
/* func:start tx test tone
 * params:
 * return:status
 * */
static int siwifi_ioctl_do_ate_tx_tone_start(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    if (siwifi_hw->ate_env.tx_frame_start) {
        printk("another tx frame has started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }

    if (siwifi_build_trx_test_environment(siwifi_hw)) {
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }
#ifdef CONFIG_SFA28_FULLMASK
    printk("path set %d\n",txvector.antennaSet);
    if (siwifi_hw->ate_env.conf.chandef.chan->center_freq <= 3072) {
        rf_trx_status_change( siwifi_hw->ate_env.siwifi_vif->txpower_idx,1,0);
        rf_set_test_dc(1,txvector.antennaSet,1,200,0);
    }
    else if(siwifi_hw->ate_env.conf.chandef.chan->center_freq > 4870 && siwifi_hw->ate_env.conf.chandef.chan->center_freq < 6145) {
        rf_trx_status_change( siwifi_hw->ate_env.siwifi_vif->txpower_idx,0,0);
        rf_set_test_dc(0,txvector.antennaSet,1,200,0);
    }
#endif
DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

/* func:stop tx test tone
 * params:
 * return:status
 * */
static int siwifi_ioctl_do_ate_tx_tone_stop(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
#ifdef DEBUG_PRINTK
    printk("%s\n", __func__);
#endif
    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        status = SIWIFI_IOCTL_RET_FAILURE;
        goto DONE;
    }
#ifdef CONFIG_SFA28_FULLMASK
    if (siwifi_hw->ate_env.conf.chandef.chan->center_freq <= 3072) {
        rf_set_test_dc(1,txvector.antennaSet,0,0,0);
        rf_trx_status_change( siwifi_hw->ate_env.siwifi_vif->txpower_idx,1,1);
    }
    else if (siwifi_hw->ate_env.conf.chandef.chan->center_freq > 4870 && siwifi_hw->ate_env.conf.chandef.chan->center_freq < 6145) {
        rf_set_test_dc(0,txvector.antennaSet,0,0,0);
        rf_trx_status_change( siwifi_hw->ate_env.siwifi_vif->txpower_idx,0,1);
    }
#endif
    if (siwifi_hw->ate_env.vif_ctx_flag) {
        struct me_ate_tools_op_cfm op_cfm;
        if (siwifi_send_me_ate_tools_op(siwifi_hw, NULL, &op_cfm, DELETE_CHANTX_OP)) {
            printk("can not remove channel context!\n");
            goto DONE;
        }
        siwifi_hw->ate_env.vif_ctx_flag = false;
    }

DONE:
    siwifi_ioctl_response_to_user(cfg, iwr, status);
    return status;
}

//the first CMD type is for test flow control
static siwifi_ioctl_cmd_handler siwifi_ioctl_cmd_set1[] =
{
    /* cmd id start from 0x0 */
    siwifi_ioctl_do_ate_start,/* 0x0000 */
    siwifi_ioctl_do_ate_stop,/* 0x0001 */
    siwifi_ioctl_do_ate_tx_start,/* 0x0002 */
    siwifi_ioctl_do_ate_tx_stop,/* 0x0003 */
    siwifi_ioctl_do_ate_rx_start,/* 0x0004 */
    siwifi_ioctl_do_ate_rx_stop,/* 0x0005 */
    siwifi_ioctl_do_ate_tx_frame_start,/* 0x0006 */
    //  siwifi_ioctl_do_ate_tx_cont_start,/* 0x0007 */
    siwifi_ioctl_do_ate_rx_frame_start,/* 0x0007 */
    siwifi_ioctl_do_ate_macbypass_tx_start,/* 0x0008 */
    siwifi_ioctl_do_ate_macbypass_tx_stop,/* 0x0009 */
    siwifi_ioctl_do_ate_tx_tone_start,/* 0x000A */
    siwifi_ioctl_do_ate_tx_tone_stop,/* 0x000B */
    siwifi_ioctl_do_ate_macbypass_rx_start,/* 0x000c */
    siwifi_ioctl_do_ate_macbypass_rx_stop,/* 0x000d */

};


//the second CMD group is for setting tx/rx parameters
static siwifi_ioctl_cmd_handler siwifi_ioctl_cmd_set2[] =
{
    /* cmd id start from 0x100 */
    siwifi_ioctl_do_set_phy_bandwidth,/* 0x100 */
    siwifi_ioctl_do_set_channel,/* 0x101 */
    siwifi_ioctl_do_set_phy_mode,/*0x102 */
    siwifi_ioctl_do_set_rate,/* 0x103 */
    siwifi_ioctl_do_set_preamble,/* 0x104 */
    siwifi_ioctl_do_set_gi,/* 0x105 */
    siwifi_ioctl_do_set_tx_power,/* 0x106 */
    siwifi_ioctl_do_get_info,/* 0x107 */
    siwifi_ioctl_do_save_to_mtd,/* 0x108 */
    siwifi_ioctl_do_set_center_freq1,/* 0x109 */
    [11] = siwifi_ioctl_do_read_from_mtd,/* 0x10b */
    [12] = siwifi_ioctl_do_save_data_to_mtd,/* 0x10c */
	[13] = siwifi_ioctl_do_get_data_from_factory,/*0x10d*/
    [16] = siwifi_ioctl_do_get_factory_power,/*0x110*/
#if 0
    siwifi_ioctl_do_select_antenna,/* 0x10a */
#endif
};


//the third CMD group is for MAC frame content set
static siwifi_ioctl_cmd_handler siwifi_ioctl_cmd_set3[]=
{
    /*cmd id start from 0x1000*/
    siwifi_ioctl_do_set_whole_frame,/* 0x1000 */
    siwifi_ioctl_do_set_tx_count,/* 0x1001 */
    siwifi_ioctl_do_set_payload_length,/* 0x1002 */
    siwifi_ioctl_do_set_frame_control,/*  0x1003 */
    siwifi_ioctl_do_set_duration_id,/*  0x1004  */
    siwifi_ioclt_do_set_tx_bssid,/*  0x1005 */
    siwifi_ioclt_do_set_tx_da,/*  0x1006 */
    siwifi_ioclt_do_set_tx_sa,/*  0x1007 */
    siwifi_ioctl_do_set_seq_ctrl,/*  0x1008 */
    siwifi_ioctl_do_set_payload,/*  0x1009 */
    siwifi_ioctl_do_set_frame_bandwidth,/* 0x100a */
    siwifi_ioctl_do_set_macbypass_interval,/* 0x100b */
    siwifi_ioctl_do_set_xo_value, /* 0X100C */
    siwifi_ioctl_do_set_aet_num, /*0x100D*/
};

//the fourth CMD group is for MAC/PHY/RF Registers read/write
//also include flash/eeprom read
static siwifi_ioctl_cmd_handler siwifi_ioctl_cmd_set4[]=
{
    /*cmd id start from 0x10000*/
    siwifi_ioctl_do_read_reg,/* 0x10000 */
    siwifi_ioctl_do_write_reg,/* 0x10001 */
};
#if 0
//the fifth CMD group is for getting statistics
static siwifi_ioctl_cmd_handler siwifi_ioctl_cmd_set5[]=
{
    /*cmd id start from 0x100000*/
};
#endif

struct siwifi_ioctl_cmd_table {
    siwifi_ioctl_cmd_handler *cmd_set;
    int cmd_set_size;
    int cmd_offset;
};


static struct siwifi_ioctl_cmd_table siwifi_ioctl_cmd_tables[]={
    {
        siwifi_ioctl_cmd_set1,
        sizeof(siwifi_ioctl_cmd_set1) / sizeof(siwifi_ioctl_cmd_handler),
        0x0,
    },
    {
        siwifi_ioctl_cmd_set2,
        sizeof(siwifi_ioctl_cmd_set2) / sizeof(siwifi_ioctl_cmd_handler),
        0x100,
    },
    {
        siwifi_ioctl_cmd_set3,
        sizeof(siwifi_ioctl_cmd_set3) / sizeof(siwifi_ioctl_cmd_handler),
        0x1000,
    },
    {
        siwifi_ioctl_cmd_set4,
        sizeof(siwifi_ioctl_cmd_set4) / sizeof(siwifi_ioctl_cmd_handler),
        0x10000,
    },
#if 0
    {
        siwifi_ioctl_cmd_set5,
        sizeof(siwifi_ioctl_cmd_set5) / sizeof(siwifi_ioctl_cmd_set5),
        0x100000,
    }
#endif

};


//define new command by command_type
//and set txvector by the index from command_id
static uint32_t siwifi_ioctl_do_set_txvector(uint32_t command_id,struct siwifi_ioctl_cfg *cfg){
    uint32_t index=0;
    char *txvector_param = &(cfg->data[0]);
    uint8_t txvector_val=0;

    index= siwifi_char_to_int(txvector_param) & 0xF0;
    txvector_val=siwifi_char_to_int(txvector_param) & 0x0F;
    switch(index){
        case 0x10:
            txvector.sounding = txvector_val;
            break;
        case 0x20:
            txvector.feccoding = txvector_val;
            break;
        case 0x30:
            txvector.smoothing = txvector_val;
            break;
        case 0x40:
            txvector.antennaSet = txvector_val;
            break;
        case 0x50:
            txvector.pretype = txvector_val;
            break;
        case 0x60:
            txvector.nsts = txvector_val;
            break;
        case 0x70:
            txvector.ntx = txvector_val;
            break;
        case 0x80:
            txvector.beamformed = txvector_val;
            break;
        case 0x90:
        default:
            printk("no this txvector,please check your setting");
            return SIWIFI_IOCTL_RET_FAILURE;

    }

    printk("txvector param %x = %x",index,txvector_val);
    return SIWIFI_IOCTL_RET_SUCCESS;
}
static int siwifi_ioctl_cmd_traversal(struct siwifi_hw *siwifi_hw,
        struct siwifi_ioctl_cfg *cfg,
        struct iwreq *iwr)
{
    int status = SIWIFI_IOCTL_RET_SUCCESS;
    uint32_t command_id;
    uint32_t table_index = 0;
    uint32_t command_type;
    command_id = cfg->command_id;
    command_type = cfg->command_type;
#ifdef DEBUG_PRINTK
    printk("\n%s: Command_Id = 0x%04x !\n", __func__, command_id);
#endif
    switch (command_type){
        case SFCFG_CMD_ATE_TXVECTOR_PARAM:
            status=siwifi_ioctl_do_set_txvector(command_id,cfg);
            break;
        default:
            break;
    }
    if (command_type != SFCFG_CMD_ATE_TXVECTOR_PARAM){
        while (table_index < (sizeof(siwifi_ioctl_cmd_tables) / sizeof(struct siwifi_ioctl_cmd_table))) {
            int cmd_index = 0;
            cmd_index = command_id - siwifi_ioctl_cmd_tables[table_index].cmd_offset;
            if ((cmd_index >= 0) && (cmd_index < siwifi_ioctl_cmd_tables[table_index].cmd_set_size)) {
                siwifi_ioctl_cmd_handler *target_handler =
                    siwifi_ioctl_cmd_tables[table_index].cmd_set;

                if (target_handler[cmd_index] != NULL)
                    status = (*target_handler[cmd_index])(siwifi_hw, cfg, iwr);
                break;
            }
            table_index++;
        }
    }
    return status;
}

void siwifi_ate_tx_cb(struct siwifi_hw *siwifi_hw, struct sk_buff *skb)
{
    struct siwifi_txhdr *txhdr;
    union siwifi_hw_txstatus *siwifi_txst;
    txhdr = (struct siwifi_txhdr *)skb->data;
    siwifi_txst = (union siwifi_hw_txstatus *)(&(txhdr->hw_hdr.cfm.status));
    //TODO, try to find the skb in the send list
#ifdef CONFIG_SIWIFI_AGG_TX
    if (siwifi_hw->ate_env.agg_tx) {
        siwifi_hw->ate_env.tx_agg_done = siwifi_hw->stats.agg_done;
        siwifi_hw->ate_env.tx_agg_retry = siwifi_hw->stats.agg_retries;
    }
#endif
    if (siwifi_txst->tx_done)
        siwifi_hw->ate_env.tx_successful_count++;
    if (siwifi_txst->retry_required)
        siwifi_hw->ate_env.tx_retry_count++;
}



void siwifi_ate_rx_cb(struct siwifi_hw *siwifi_hw)
{
    if ((!siwifi_hw->ate_env.rx_frame_start) && (!siwifi_hw->ate_env.rx_macbypass_start)) {
        printk("rx do not start\n");
        return;
    }

    if (siwifi_hw->ate_env.rx_macbypass_start) {
        siwifi_hw->ate_env.fcs_err = REG_PL_RD(MACBYP_STAT_RXERROR_ADDR(siwifi_hw->mod_params->is_hb));
        siwifi_hw->ate_env.phy_err = REG_PL_RD(MACBYP_STAT_FRAME_BAD_ADDR(siwifi_hw->mod_params->is_hb));
        siwifi_hw->ate_env.all_err = siwifi_hw->ate_env.fcs_err + siwifi_hw->ate_env.phy_err;
        siwifi_hw->ate_env.fcs_ok = REG_PL_RD(MACBYP_STAT_FRAME_OK_ADDR(siwifi_hw->mod_params->is_hb));
        siwifi_hw->ate_env.rec_rx_count = REG_PL_RD(MACBYP_STAT_RXEND_ADDR(siwifi_hw->mod_params->is_hb));
    } else {

        //record info about error frame
        siwifi_hw->ate_env.fcs_err = REG_PL_RD(DOT11FCSERRORCOUNT_ADDR(siwifi_hw->mod_params->is_hb));
        siwifi_hw->ate_env.phy_err = REG_PL_RD(RWRXPHYERRORCOUNT_ADDR(siwifi_hw->mod_params->is_hb));
        siwifi_hw->ate_env.all_err = siwifi_hw->ate_env.fcs_err + siwifi_hw->ate_env.phy_err;
        siwifi_hw->ate_env.fcs_ok_for_macaddr =  REG_PL_RD(RWQOSURECEIVEDMPDUCOUNT0_ADDR(siwifi_hw->mod_params->is_hb)) + REG_PL_RD(RWQOSURECEIVEDMPDUCOUNT1_ADDR(siwifi_hw->mod_params->is_hb))+ REG_PL_RD(RWQOSURECEIVEDMPDUCOUNT2_ADDR(siwifi_hw->mod_params->is_hb)) + REG_PL_RD(RWQOSURECEIVEDMPDUCOUNT3_ADDR(siwifi_hw->mod_params->is_hb)) + REG_PL_RD(RWQOSURECEIVEDMPDUCOUNT4_ADDR(siwifi_hw->mod_params->is_hb))+ REG_PL_RD(RWQOSURECEIVEDMPDUCOUNT5_ADDR(siwifi_hw->mod_params->is_hb)) + REG_PL_RD(RWQOSURECEIVEDMPDUCOUNT6_ADDR(siwifi_hw->mod_params->is_hb)) + REG_PL_RD(RWQOSURECEIVEDMPDUCOUNT7_ADDR(siwifi_hw->mod_params->is_hb));
        siwifi_hw->ate_env.fcs_group =  REG_PL_RD(RWQOSURECEIVEDMPDUCOUNT10_ADDR(siwifi_hw->mod_params->is_hb)) + REG_PL_RD(RWQOSURECEIVEDMPDUCOUNT11_ADDR(siwifi_hw->mod_params->is_hb))+ REG_PL_RD(RWQOSURECEIVEDMPDUCOUNT12_ADDR(siwifi_hw->mod_params->is_hb)) + REG_PL_RD(RWQOSURECEIVEDMPDUCOUNT13_ADDR(siwifi_hw->mod_params->is_hb)) + REG_PL_RD(RWQOSURECEIVEDMPDUCOUNT14_ADDR(siwifi_hw->mod_params->is_hb))+ REG_PL_RD(RWQOSURECEIVEDMPDUCOUNT15_ADDR(siwifi_hw->mod_params->is_hb)) + REG_PL_RD(RWQOSURECEIVEDMPDUCOUNT16_ADDR(siwifi_hw->mod_params->is_hb)) + REG_PL_RD(RWQOSURECEIVEDMPDUCOUNT17_ADDR(siwifi_hw->mod_params->is_hb));
        switch (siwifi_hw->ate_env.conf.chandef.width) {
            case 0:
            case 1:
                siwifi_hw->ate_env.rec_rx_count = REG_PL_RD(DOT1120MHZFRAMERECEIVEDCOUNT_ADDR(siwifi_hw->mod_params->is_hb)) + siwifi_hw->ate_env.all_err;
                siwifi_hw->ate_env.fcs_ok = REG_PL_RD(DOT1120MHZFRAMERECEIVEDCOUNT_ADDR(siwifi_hw->mod_params->is_hb));
                break;
            case 2:
                siwifi_hw->ate_env.fcs_ok = REG_PL_RD(DOT1140MHZFRAMERECEIVEDCOUNT_ADDR(siwifi_hw->mod_params->is_hb));
                siwifi_hw->ate_env.rec_rx_count = REG_PL_RD(DOT1140MHZFRAMERECEIVEDCOUNT_ADDR(siwifi_hw->mod_params->is_hb)) + siwifi_hw->ate_env.all_err;
                break;
            case 3:
                siwifi_hw->ate_env.fcs_ok = REG_PL_RD(DOT1180MHZFRAMERECEIVEDCOUNT_ADDR(siwifi_hw->mod_params->is_hb));
                siwifi_hw->ate_env.rec_rx_count = REG_PL_RD(DOT1180MHZFRAMERECEIVEDCOUNT_ADDR(siwifi_hw->mod_params->is_hb)) + siwifi_hw->ate_env.all_err;
                break;
            default:
                siwifi_hw->ate_env.fcs_ok =  REG_PL_RD(DOT1120MHZFRAMERECEIVEDCOUNT_ADDR(siwifi_hw->mod_params->is_hb));
                siwifi_hw->ate_env.rec_rx_count = REG_PL_RD(DOT1120MHZFRAMERECEIVEDCOUNT_ADDR(siwifi_hw->mod_params->is_hb)) + siwifi_hw->ate_env.all_err;
                break;
        }
    }
}

void siwifi_ate_rx_cb_rssi(struct siwifi_hw *siwifi_hw, struct sk_buff *skb)
{
    struct hw_rxhdr *hw_rxhdr;
    hw_rxhdr = (struct hw_rxhdr *)skb->data;

    //record rssi
    siwifi_hw->ate_env.rssi = hw_rxhdr->hwvect.rx_vect1.rssi1;
}

/*
 * func:this function is for siwifi ioctl internal use,could be called:
 *      1,siwifi_ioctl_do_ate_tx_frame
 *      2,siwifi_ioctl_do_ate_tx_cont_frame
 *      3,siwifi_task, when there is a free buffer after release
 * params:
 * return:
 * */
uint32_t busy_count = 0;
void siwifi_ate_task(unsigned long data)
{
    struct siwifi_hw *siwifi_hw = (struct siwifi_hw *)data;
    struct siwifi_txq *txq;
    int tx_count;
    int i = 0;
    struct sk_buff *skb, *tmp;
    struct sk_buff_head skbs;
#ifdef CONFIG_SIWIFI_AGG_TX
    struct siwifi_sta *siwifi_sta;
#endif
    uint16_t seq = 0;
    //maybe we should add a protect for this vifs
    //think about that if somebody delete the interface while we still do the ate test,
    //crash will happen!
    //Now,the solution is to check the siwifi_hw->vifs.
    struct siwifi_vif *siwifi_vif = siwifi_hw->ate_env.siwifi_vif;
    //printk("%s &siwifi_vif=0x%x, vif_index=0x%x, &ieee80211_vif=0x%x, type=0x%x, addr=%pM\n",
    //__func__, siwifi_vif, siwifi_vif->vif_index, siwifi_vif->vif, siwifi_vif->vif->type, siwifi_vif->vif->addr);
    if (!siwifi_vif) {
        printk("no any vifs exist, tx abort!\n");
        return;
    }

    if (!siwifi_hw->ate_env.ate_start) {
        printk("ate not started!\n");
        return;
    }

    if (!siwifi_hw->ate_env.tx_frame_start) {
        printk("TX not started!\n");
        return;
    }
    /*if we transmit all the frames, we should exit
      and enable the up layer stack's tx queue;otherwise
      we should do:
      1,stop the upper layer's tx queue when we first enter into the task,this has been done by siwifi_ioctl_do_ate_tx_frame
      2,check how many tx seats for this times
      3,exit
     */
    //maybe we need to add a lock when we access ate_env and the upper layer tx queue
    if ((siwifi_hw->ate_env.tx_left_count <= 0) && (siwifi_hw->ate_env.conf.pkg.cont_flag == false)) {
        spin_lock(&siwifi_hw->tx_lock);
        siwifi_txq_stop(&siwifi_hw->txq[siwifi_hw->ate_env.tx_queue], SIWIFI_TXQ_STOP_FULL, SIWIFI_TXQ_STOP_POS_IOCTL);
        siwifi_hw->ate_env.tx_frame_start = false;
        spin_unlock(&siwifi_hw->tx_lock);
        return;
    }

    //here get the numble of hw tx seats for this tx time
    txq = &siwifi_hw->txq[siwifi_hw->ate_env.tx_queue];
#if 0
#ifdef CONFIG_SIWIFI_AGG_TX
    if (siwifi_hw->ate_env.agg_tx) {
        siwifi_sta = (struct siwifi_sta *)siwifi_hw->ate_env.control->sta->drv_priv;
        txq = &siwifi_sta->txqs[siwifi_hw->ate_env.params->tid];
    }
#endif
#endif
    spin_lock(&siwifi_hw->tx_lock);
    tx_count = txq->credits;
    spin_unlock(&siwifi_hw->tx_lock);
    //printk("tx_count is %d\n",tx_count);

    if (tx_count <= 0) {
        //printk("this time no free tx desc for us to use\n");
        us_sleep(100);
        busy_count++;
        if (busy_count > 10000) {
            printk("channel is busy!\n");
            return;
        }
        tasklet_schedule(&siwifi_hw->ate_env.tx_task);
        return;
    }
    busy_count = 0;
    //reset tx_count to ate_env.tx_left_count
    if (siwifi_hw->ate_env.tx_left_count < tx_count) {
        tx_count = siwifi_hw->ate_env.tx_left_count;
    }
    //start to transmit tx_count frames by siwifi_start_xmit loop
#ifdef CONFIG_SIWIFI_AGG_TX
    if (siwifi_hw->ate_env.agg_tx)
        seq = siwifi_hw->ate_env.tx_total_count - siwifi_hw->ate_env.tx_left_count;
#endif
#if 1
    //build skbs
    __skb_queue_head_init(&skbs);
    while (tx_count--) {
        skb = siwifi_ioctl_internal_build_skb(siwifi_hw, siwifi_vif, seq);
        __skb_queue_tail(&skbs, skb);
        i++;
        seq++;
    }
    if (skb_queue_empty(&skbs)) {
        printk("No skbs, error!!!!!!!!!!!!!\n");
        return;
    }
    skb_queue_walk_safe(&skbs, skb, tmp)
    {
        if (list_empty(&siwifi_hw->vifs)) {
            printk("vif have been deleted!!!!!\n");
            return;
        }
        if (!siwifi_hw->ate_env.vif_ctx_flag) {
            printk("no channel context!\n");
            return;
        }
        siwifi_start_xmit(skb, siwifi_vif->ndev);
    }
#else
    //single MPDU
    while (tx_count--) {
        skb = siwifi_ioctl_internal_build_skb(siwifi_hw, siwifi_vif, 0);
        if (!skb) {
            printk("No skb, error!!!!!!!!!!!!!!!\n");
            return;
        }
        //to avoid the crash when the vifs are deleted
        if (list_empty(&siwifi_hw->vifs)) {
            printk("vif have been deleted!!!!!\n");
            return;
        }
        if (!siwifi_hw->ate_env.vif_ctx_flag) {
            printk("no channel context!\n");
            return;
        }
        siwifi_start_xmit(skb, siwifi_vif->ndev);
        i++;
    }
#endif
    siwifi_hw->ate_env.tx_count += i;
    //renew ate_env.tx_left_count before tx start
    if (siwifi_hw->ate_env.conf.pkg.cont_flag == false) {
        siwifi_hw->ate_env.tx_left_count -= i;
        if (siwifi_hw->ate_env.agg_tx && (siwifi_hw->ate_env.tx_left_count % 100 == 0)) {
            printk("*****total=%d left=%d, send=%d, retry=%d, successful mpdu=%d Ampdu==%d*****\n",
                    siwifi_hw->ate_env.tx_total_count, siwifi_hw->ate_env.tx_left_count,
                    siwifi_hw->ate_env.tx_count + siwifi_hw->ate_env.tx_retry_count + siwifi_hw->ate_env.tx_agg_retry,
                    siwifi_hw->ate_env.tx_retry_count + siwifi_hw->ate_env.tx_agg_retry,
                    siwifi_hw->ate_env.tx_successful_count, siwifi_hw->ate_env.tx_agg_done);
        } else if (siwifi_hw->ate_env.tx_left_count % 1000 == 0) {
            printk("*****total=%d left=%d, send=%d, retry=%d, successful=%d *****\n",
                    siwifi_hw->ate_env.tx_total_count, siwifi_hw->ate_env.tx_left_count,
                    siwifi_hw->ate_env.tx_count, siwifi_hw->ate_env.tx_retry_count, siwifi_hw->ate_env.tx_successful_count);
        }
    } else {
        if (siwifi_hw->ate_env.tx_count % 1000 == 0) {
            printk("*****continuous mode, send=%d, retry=%d, successful=%d*****\n",
                    siwifi_hw->ate_env.tx_count, siwifi_hw->ate_env.tx_retry_count, siwifi_hw->ate_env.tx_successful_count);
        }
    }

    tasklet_schedule(&siwifi_hw->ate_env.tx_task);
    return;
}
//sync with ate_server.c
#define SFCFG_PRIV_IOCTL_ATE     (SIOCIWFIRSTPRIV + 0x08)
int siwifi_ops_ioctl(struct siwifi_hw *siwifi_hw,
        struct ifreq *rq,
        int cmd)
{
    struct siwifi_ioctl_cfg *cfg;
    struct iwreq *iwr = (struct iwreq *)rq;
    int status = 0;
    unsigned int magic_num;
    int ret = 0;
// we only support  ate cmd
	if(cmd != SFCFG_PRIV_IOCTL_ATE){
		printk("[SIWIFI]  got unsupport  cmd  0x%x ifname %s\n", cmd, rq->ifr_ifrn.ifrn_name);
		return -1;
	}

    cfg = siwifi_kzalloc(sizeof(struct siwifi_ioctl_cfg), GFP_KERNEL);
    if (!cfg) {
        printk("alloc ioctl cfg fail!\n");
        return -ENOMEM;
    }

	ret = copy_from_user(cfg, iwr->u.data.pointer, iwr->u.data.length);
	// printk("[debug] ifname %s wifi iwrr ioctl ptr 0x%p  len %d, ret %d cmd %d \n", rq->ifr_ifrn.ifrn_name,iwr->u.data.pointer, iwr->u.data.length,ret, cmd);
    if (ret) {
        printk("can not copy the ifreq from userspace! %d \n", ret);
        status = -EFAULT;
        goto DONE;
    }
#if 0
    printk("siwifi_ops_ioctl+++++++++++++++++++++++++++++++++++++++++++++\n");
    printk("cmd = 0x%x\n", cmd);
    printk("magic_no = 0x%x\n", cfg->magic_no);
    printk("command_type = 0x%x\n", cfg->command_type);
    printk("command_id = 0x%x\n", cfg->command_id);
    printk("length = %d\n", cfg->length);
    printk("sequence = %d\n", cfg->sequence);
    printk("data : %s\n", (char *)(cfg->data));
#endif
    //TODO:
    //for each cmds, need some SanityCheck?

    //try to traveral all cmds' handlers ,if match, then we will use it to process this cmds
    //magic_num = ntohl(cfg->magic_no);
    magic_num = cfg->magic_no;
    switch (magic_num) {
        case SFCFG_MAGIC_NO:
            status = siwifi_ioctl_cmd_traversal(siwifi_hw, cfg, iwr);
            break;
        default:
            status = SIWIFI_IOCTL_RET_FAILURE;
            printk("unknown magic number!\n");
            break;
    }
    //unified all the return value to outside

    if (status)
        status = -1;
DONE:
    siwifi_kfree(cfg);
    return status;
}
