/**
 ******************************************************************************
 *
 * @file siwifi_debugfs.c
 *
 * @brief Definition of debugfs entries
 *
 * Copyright (C) Siflower 2018-2025
 *
 ******************************************************************************
 */


#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/debugfs.h>
#include <linux/string.h>
#include <linux/sort.h>
#include <linux/rtc.h>

#include "siwifi_debugfs.h"
#include "siwifi_msg_tx.h"
#include "siwifi_radar.h"
#include "siwifi_tx.h"
#include "lmac_msg.h"
#include "reg_mdm_cfg.h"
#include "siwifi_v1.h"
#include "siwifi_lmac_glue.h"
#include "siwifi_cfgfile.h"
#include "ipc_host.h"
#include "siwifi_mem.h"
#include "hal_machw_mib.h"
#include "reg_access.h"
#include "siwifi_version.h"
#if defined CONFIG_SIWIFI_REPEATER
#include "siwifi_repeater.h"
#endif
#if defined CONFIG_SIWIFI_IGMP
#include "siwifi_igmp.h"
#endif
#include "siwifi_frame.h"
#ifdef CONFIG_SIWIFI_IQENGINE
#include "siwifi_iqengine.h"
#endif

#ifdef CONFIG_SIWIFI_PROCFS
int siwifi_proc_show(struct seq_file *m, void *v)
{
    return 0;
}

int siwifi_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, siwifi_proc_show, PDE_DATA(inode));
}

static ssize_t siwifi_dbgfs_bool_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
    int value;
    char buf[32];
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    /* Get the content of the file */
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;
    buf[len] = '\0';
    sscanf(buf, "%d\n", &value);
    printk("%d\n", value);
    return len;
}

static ssize_t siwifi_dbgfs_bool_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
    char buf[32];
    ssize_t read;
    struct seq_file *seq = file->private_data;
    struct siwifi_hw * priv = seq->private;
    ssize_t ret = 0;
    u64 val;

    val = *(int *)priv;
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1, count), "%lld\n", val);
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_x32_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
    u32 value;
    char buf[32];
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    /* Get the content of the file */
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;
    buf[len] = '\0';
    sscanf(buf, "0x%08x\n", &value);
    printk("0x%08x\n", value);
    return len;
}

static ssize_t siwifi_dbgfs_x32_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
    char buf[32];
    ssize_t read;
    struct seq_file *seq = file->private_data;
    struct siwifi_hw * priv = seq->private;
    ssize_t ret = 0;
    u64 val;

    val = *(u32 *)priv;
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1, count), "0x%08llx\n", val);
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_u32_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
    u32 value;
    char buf[32];
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    /* Get the content of the file */
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;
    buf[len] = '\0';
    sscanf(buf, "%u\n", &value);
    printk("%u\n", value);
    return len;
}

static ssize_t siwifi_dbgfs_u32_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
    char buf[32];
    ssize_t read;
    struct seq_file *seq = file->private_data;
    struct siwifi_hw * priv = seq->private;
    ssize_t ret = 0;
    u64 val;

    val = *(u32 *)priv;
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1, count), "%llu\n", val);
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_x64_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
    u64 value;
    char buf[32];
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    /* Get the content of the file */
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;
    buf[len] = '\0';
    sscanf(buf, "0x%016llx\n", &value);
    printk("0x%016llx\n", value);
    return len;
}


static ssize_t siwifi_dbgfs_x64_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
    char buf[32];
    ssize_t read;
    struct seq_file *seq = file->private_data;
    struct siwifi_hw * priv = seq->private;
    ssize_t ret = 0;
    u64 val;

    val = *(u64 *)priv;
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1, count), "0x%016llx\n", val);
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_u64_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
    u64 value;
    char buf[32];
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    /* Get the content of the file */
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;
    buf[len] = '\0';
    sscanf(buf, "%llu\n", &value);
    printk("%llu\n", value);
    return len;
}


static ssize_t siwifi_dbgfs_u64_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
    char buf[32];
    ssize_t read;
    struct seq_file *seq = file->private_data;
    struct siwifi_hw * priv = seq->private;
    ssize_t ret = 0;
    u64 val;

    val = *(u64 *)priv;
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1, count), "%llu\n", val);
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}
#endif

#if defined CONFIG_VDR_HW
#include "hw_interface.h"

static void siwifi_debug_hw_interface(struct siwifi_hw *siwifi_hw)
{
    struct net_device *dev;
    struct siwifi_sta *sta;
    struct siwifi_vif *siwifi_vif, *__siwifi_vif;
    struct vap_status_diag_info vap_info_resp;
    struct sta_status_diag_info sta_info_resp;

    spin_lock_bh(&siwifi_hw->cb_lock);
    list_for_each_entry_safe(siwifi_vif, __siwifi_vif, &siwifi_hw->vifs, list) {
        dev = siwifi_vif->ndev;
        vendor_get_vap_status(dev, &vap_info_resp);
        list_for_each_entry(sta, &siwifi_vif->ap.sta_list, list) {
            vendor_get_sta_status(dev, sta, &sta_info_resp);
        }
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);
}

static ssize_t siwifi_dbgfs_vendor_stat_write(struct file *file,
                                                  const char __user *user_buf,
                                                  size_t count, loff_t *ppos)
{
    int value;
    char buf[32];
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    /* Get the content of the file */
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';
    sscanf(buf, "%d\n", &value);
    printk("%d\n", value);
    return len;
}

static ssize_t siwifi_dbgfs_vendor_stat_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    char buf[32];
    int ret;
    ssize_t read;

    if (*ppos == 0) {
        siwifi_debug_hw_interface(file->private_data);
    }
    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count), "%d\n", 0);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

DEBUGFS_READ_WRITE_FILE_OPS(vendor_stat);
#endif /* CONFIG_VDR_HW */

static ssize_t siwifi_dbgfs_mpinfo_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    char buf[256];
    int ret, code = -1;
    ssize_t read;
    struct siwifi_hw *siwifi_hw = (struct siwifi_hw *)file->private_data;
    struct siwifi_vif *siwifi_vif, *__siwifi_vif;
    struct dbg_get_vendor_mp_info_cfm cfm;

    list_for_each_entry_safe(siwifi_vif, __siwifi_vif, &siwifi_hw->vifs, list) {
        if ((siwifi_vif == NULL) || (siwifi_hw == NULL)) {
            printk(" no vif or siwifi hw\n");
            code = -1;
            break;
        }
        memset(&cfm, 0, sizeof(struct dbg_get_vendor_mp_info_cfm));
        if (siwifi_send_dbg_get_vendor_mp_info_req(siwifi_hw, &cfm) == 0) {
            code = 0;
            break;
        } else {
            printk("siwifi_send_dbg_get_vendor_mp_info_req send msg fail\n");
            code = -1;
            break;
        }
    }

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
            "ret=%d \ntotal=%d us\ncca=%d us\ncca20=%d us\ncca40=%d us\ntx=%d us\nrx=%d us\nnoise0=%d dbm\nnoise1=%d dbm\ncpuidle=%d/100\n",
            code, cfm.last_cal_time_us, cfm.cca_busy_time_us, cfm.cca20_busy_time_us, cfm.cca40_busy_time_us,
            cfm.tx_frame_time_us, cfm.rx_frame_time_us, cfm.last_noise0_dbm,cfm.last_noise1_dbm,cfm.cpu_idle_time_us*100/cfm.last_cal_time_us);
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

DEBUGFS_READ_FILE_OPS(mpinfo);

#ifdef CONFIG_SIWIFI_TRX_STAT
static ssize_t siwifi_dbgfs_lmacrx_read(struct file *file,
                                     char __user *user_buf,
                                     size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char *buf;
    int ret = 0;
    ssize_t read;
    struct dbg_get_rx_stat_cfm cfm;
    struct dbg_rx_stat *dbg_trx_stat;

    int bufsz = 2048;
    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (!buf)
        return 0;
    if (siwifi_send_dbg_get_rx_stat_req(priv, &cfm, 0)) {
        printk("can not send rx stat cmd\n");
        goto DONE;
    }
    dbg_trx_stat = (struct dbg_rx_stat *)priv->dbg_trx_stat_elem.addr;

    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n RX mib              total                 uploads\n");

    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
            "%10d          %10u         %10u\n", dbg_trx_stat->mib, dbg_trx_stat->total, dbg_trx_stat->upload);

    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  Host buffer not available     %10d\n", dbg_trx_stat->hostbuf_not_available);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  Rx amsdu buf cnt              %10d\n", dbg_trx_stat->amsdu);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  Key-invalid  spuirous         %10d\n", dbg_trx_stat->key_invalid_spurious_data);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  Key-invalid  duplicate        %10d\n", dbg_trx_stat->key_invalid_duplicate_frame);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  Key-invalid  spu-mgmt         %10d\n", dbg_trx_stat->key_invalid_spurious_mgmt);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  Key-invalid  mgmt handled     %10d\n", dbg_trx_stat->key_invalid_mgmt_handled);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  Key-invalid  mgmt upload      %10d\n", dbg_trx_stat->key_invalid_mgmt_upload);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  Sta is not registered         %10d\n", dbg_trx_stat->sta_nonregister);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  Encryption failed             %10d\n", dbg_trx_stat->encrypted_fail);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  RX ctrl_frame                 %10d\n", dbg_trx_stat->ctrl_frame);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  RX mgmt frame                 %10d\n", dbg_trx_stat->mgt_frame);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  RX data frame                 %10u\n", dbg_trx_stat->data_frame);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  Total data frame discards: \n");
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  Reord discard                 %10d\n", dbg_trx_stat->data_frame_reord_discard);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  Reord discard dup             %10d\n", dbg_trx_stat->data_frame_reord_duplicate_discard);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  Reord discard pn              %10d\n", dbg_trx_stat->data_frame_reord_pn_discard);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  Frame discard own             %10d\n", dbg_trx_stat->data_frame_own_sa_discard);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  mpdu cnt1 discard             %10d\n", dbg_trx_stat->data_frame_mpdu1_discard);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  duplicate frame discard       %10d\n", dbg_trx_stat->data_frame_duplicate_discard);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n  pn check failed discard       %10d\n", dbg_trx_stat->data_frame_pn_failed_discard);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n defrag_frame                   %10d\n", dbg_trx_stat->defrag_frame);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n rx pd dma used idx             %10u\n", dbg_trx_stat->rx_pddesc_used_idx);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n rx pd dma free idx             %10u\n", dbg_trx_stat->rx_pddesc_free_idx);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n rx max msdu cnt                %10u\n", dbg_trx_stat->max_msdu_idx);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n rx max dma cnt                 %10u\n", dbg_trx_stat->max_dma_idx);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n rx max used pd cnt             %10u\n", dbg_trx_stat->max_used_pddesc);

    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n fa 26140                       %10d\n", dbg_trx_stat->cve_2020_26140);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n fa 24588                       %10d\n", dbg_trx_stat->cve_2020_24588);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n fa 24587                       %10d\n", dbg_trx_stat->cve_2020_24587);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n fa 26146                       %10d\n", dbg_trx_stat->cve_2020_26146);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n jumbo frame                    %10d\n", dbg_trx_stat->jumbo_frame);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
		     "\n  pn check failed upload       %10d\n", dbg_trx_stat->data_frame_pn_failed_upload);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n reord_alloc_cnt                    %10d\n", dbg_trx_stat->reord_alloc_cnt);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n reord_release_cnt                    %10d\n", dbg_trx_stat->reord_release_cnt);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n reord_hostid_nozero_cnt                    %10d\n", dbg_trx_stat->reord_hostid_nozero_cnt);
DONE:
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    siwifi_kfree(buf);
    return read;
}

static ssize_t siwifi_dbgfs_lmacrx_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct dbg_get_rx_stat_cfm cfm;

    if (siwifi_send_dbg_get_rx_stat_req(priv, &cfm, 1))
        printk("can not send rx stat cmd\n");

    return count;
}

DEBUGFS_READ_WRITE_FILE_OPS(lmacrx);



static ssize_t siwifi_dbgfs_lmactx_read(struct file *file,
                                     char __user *user_buf,
                                     size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char *buf;
    int ret = 0;
    int i, skipped;
    ssize_t read;
    int j;
    struct dbg_get_tx_stat_cfm cfm;
    struct dbg_tx_stat *dbg_trx_stat;
    //int bufsz = 7 * 50 + 32 * 4 * 30 + 1024;
    int bufsz = 5124;

    const char agg_finish_reason_string[8][32] = {
        " Not compatible",
        "       Bw drop1",
        "     Max length",
        "      Max count",
        "      Hw empty1",
        "      Hw empty2",
        "      Hw empty3",
        "      int frame"
    };

    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (buf == NULL)
        return 0;

    ///get tx statistics from lmac
    if (siwifi_send_dbg_get_tx_stat_req(priv, &cfm, 0)){
        printk("can not send tx stat cmd\n");
        goto DONE;
    }
    dbg_trx_stat = (struct dbg_tx_stat *)priv->dbg_trx_stat_elem.addr;

    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n LMAC           single MPDU      successfull       rate(%%)\n");
    for (i = 0; i < AC_MAX; i++)
        ret += scnprintf(&buf[ret],
            min_t(size_t, bufsz - ret - 1, count - ret),
            "   AC%2d        %10d       %10d        %10d\n", i, dbg_trx_stat->s_mpdu_tx[i], dbg_trx_stat->s_mpdu_successful[i],
            (dbg_trx_stat->s_mpdu_tx[i]) ? ((dbg_trx_stat->s_mpdu_successful[i] * 100)/dbg_trx_stat->s_mpdu_tx[i]) : 0);

    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n AMPDU group         total       successfull       rate(%%)\n");
	//for debugfs can not print too much info,only care AC_BE
    //for (i = 0; i < AC_MAX; i++) {
	for (i = AC_BE; i <= AC_BE; i++) {
        ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "AC: %d\n", i);
        for (j = 0, skipped = 0; j < 64; j++) {
            if (!dbg_trx_stat->a_mpdu_count[i][j]) {
               skipped = 1;
               continue;
            }
            if (skipped) {
                ret += scnprintf(&buf[ret],
                                 min_t(size_t, bufsz - ret - 1, count - ret),
                             "   * * *            %10d    %10d    %10d\n", 0, 0, 0);
                skipped = 0;
            }
            ret += scnprintf(&buf[ret],
                         min_t(size_t, bufsz - ret - 1, count - ret),
                         "   mpdu#%2d         %10d    %10d    %10d\n", j + 1, dbg_trx_stat->a_mpdu_count[i][j],
                            dbg_trx_stat->a_mpdu_successful[i][j], (dbg_trx_stat->a_mpdu_count[i][j]) ? (dbg_trx_stat->a_mpdu_successful[i][j] * 100) / dbg_trx_stat->a_mpdu_count[i][j] : 0);

        }
        if (skipped)
            ret += scnprintf(&buf[ret],
                         min_t(size_t, bufsz - ret - 1, count - ret),
                         "   * * *            %10d    %10d    %10d\n", 0, 0, 0);
    }

    //aggregation finish reason
    for (i = 0, j = 0; i < 8; i++) {
        j += dbg_trx_stat->agg_finish_reason[i];
    }
    if (!j) {
        ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                "\n no aggregation\n");
    } else {
        ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                        "\n agg finish reason         count          percentage(%%)\n");
        for (i = 0; i < 8; i++) {
            ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                        " %d  %s         %10d     %10d\n", i, agg_finish_reason_string[i],
                        dbg_trx_stat->agg_finish_reason[i],
                        dbg_trx_stat->agg_finish_reason[i] ? dbg_trx_stat->agg_finish_reason[i] * 100 / j : 0);
        }
    }

    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n beacon tx failed cnt %d\n", dbg_trx_stat->bcn_failed_cnt);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n tx pkt cnt %d\n", dbg_trx_stat->pkt_cnt);
	ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\n LMAC          tran cnt        cfm-cnt      trans-cnt     cfm-cnt      discard-cnt     t-trans-cnt   t-cfm-cnt         t-discard-cnt       t-flush-cnt\n");
    for (i = 0; i < AC_MAX; i++)
        ret += scnprintf(&buf[ret],
            min_t(size_t, bufsz - ret - 1, count - ret),
            "   AC%2d %10d     %10d    %10d   %10d    %10d       %10d       %10d       %10d       %10d\n", i, dbg_trx_stat->transmitting_list_cnt[i], dbg_trx_stat->cfm_list_cnt[i],
			dbg_trx_stat->transmitting_list_add_cnt[i], dbg_trx_stat->cfmlist_add_cnt[i], dbg_trx_stat->discard_cnt[i], dbg_trx_stat->total_transmitting_list_add_cnt[i], dbg_trx_stat->total_cfmlist_add_cnt[i], dbg_trx_stat->total_discard_cnt[i], dbg_trx_stat->total_flush_cnt[i]);

    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\nHAL ASSERT AC1 %d;AC3 %d;PT %d BCN: %d\n",dbg_trx_stat->record_cnt[0], dbg_trx_stat->record_cnt[1], dbg_trx_stat->record_cnt[2], dbg_trx_stat->record_cnt[3]);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "\nbw drop to 20MHz cnt %d\n", dbg_trx_stat->record_cnt[4]);
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "bw drop to 40MHz cnt %d\n", dbg_trx_stat->record_cnt[5]);
DONE:
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    siwifi_kfree(buf);

    return read;
}

static ssize_t siwifi_dbgfs_lmactx_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct dbg_get_tx_stat_cfm cfm;

    if (siwifi_send_dbg_get_tx_stat_req(priv, &cfm, 1))
        printk("can not send tx stat cmd\n");

    return count;
}

DEBUGFS_READ_WRITE_FILE_OPS(lmactx);

#endif

static ssize_t siwifi_dbgfs_src_filter_read(struct file *file,
                                     char __user *user_buf,
                                     size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = file->private_data;
    struct siwifi_vif *siwifi_vif = NULL;
    struct siwifi_src_filter *src_filter = NULL;
    char *buf = NULL;
    size_t bufsz = 2048;
    ssize_t read;
    int ret = 0;
    int i = 0;
    buf = kzalloc(bufsz, GFP_ATOMIC);
    if (buf == NULL)
        return 0;
    list_for_each_entry(siwifi_vif, &priv->vifs, list) {
        if (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_STATION) {
            ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                                    "siwifi_vif  %s  %pM current %lu\n",
                                    siwifi_vif->ndev->name,
                                    siwifi_vif->ndev->dev_addr,
                                    jiffies);
            ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                                    "TX_src_macaddr     drop_count last_use\n");
            for (i = 0; i < STA_HASH_SIZE; i++) {
                src_filter = (struct siwifi_src_filter *)(siwifi_vif->src_filter_hash[i]);
                while (src_filter != NULL) {
                    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                                      "%pM  %10d %lu\n",
                                      src_filter->src_macaddr,
                                      src_filter->drop_count,
                                      src_filter->last_use
                                      );
                    src_filter = src_filter->next;
                }
            }
        }
    }
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    kfree(buf);
    return read;
}
static ssize_t siwifi_dbgfs_src_filter_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct siwifi_vif *siwifi_vif = NULL;
    char buf[32];
    int val = 0;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0) {
        if(val == 0) {
            list_for_each_entry(siwifi_vif, &priv->vifs, list) {
                if (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_STATION) {
                    siwifi_src_filter_hash_free(siwifi_vif);
                }
            }
        }
    }
    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(src_filter);

#ifdef CONFIG_SIWIFI_IQENGINE
static ssize_t siwifi_dbgfs_iq_help_read(struct file *file,
                                           char __user *user_buf,
                                           size_t count, loff_t *ppos)
{
    char buf[512];
    int ret = 0;
    ssize_t read;
    ret += scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "Usage:\n");
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                    "    mode:  [0:NONE] [1:BB PLAYER] [2:RF PLAYER] [3:BB RECORDER] [4:RF RECORDER]\n");
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                    "    length:  iq recode or player file size, in byte.\n");
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                    "    enable:  [0:start] [1:stop]\n");
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                    "    1,echo 4 > iq_mode \n");
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                    "    2,echo 5000 > iq_length  \n");
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                    "    3,echo 1 > iq_enable\n");
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    return read;
}
DEBUGFS_READ_FILE_OPS(iq_help);
static ssize_t siwifi_dbgfs_iq_mode_read(struct file *file,
                                           char __user *user_buf,
                                           size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[128];
    ssize_t read;
    int ret = 0;
    ret += scnprintf(&buf[ret], min_t(size_t, 128 - ret - 1, count - ret),
            "IQENGINE mode=%d \n", priv->iqe.mode);
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    return read;
}
static ssize_t siwifi_dbgfs_iq_mode_write(struct file *file,
                                            const char __user *user_buf,
                                            size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;
    buf[len] = '\0';
    if (sscanf(buf, "%d", &val) > 0) {
        printk("User set iq mode %d\n", val);
        if ((val < IQE_MODE_NONE) || (val > IQE_RF_RECORDER)) {
            printk("Invalid iq mode\n");
        } else {
            priv->iqe.mode = val;
        }
    }
    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(iq_mode);
static ssize_t siwifi_dbgfs_iq_length_read(struct file *file,
                                           char __user *user_buf,
                                           size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[128];
    ssize_t read;
    int ret = 0;
    ret += scnprintf(&buf[ret], min_t(size_t, 128 - ret - 1, count - ret),
            "IQENGINE length=%d (byte)\n", priv->iqe.iq_buffer_len);
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    return read;
}
static ssize_t siwifi_dbgfs_iq_length_write(struct file *file,
                                            const char __user *user_buf,
                                            size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;
    buf[len] = '\0';
    if (sscanf(buf, "%d", &val) > 0) {
        printk("User set iq length %d\n", val);
        if (val > 0 ) {
            priv->iqe.iq_buffer_len = val;
        } else {
            printk("Invalid iq length\n");
            return 0;
        }
    }
    if (priv->iqe.mode > 0) {
        iqe_init(priv, priv->iqe.iq_buffer_len, priv->iqe.mode);
    } else {
        printk("Please set iq mode first!\n");
    }
    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(iq_length);
static ssize_t siwifi_dbgfs_iq_enable_write(struct file *file,
                                            const char __user *user_buf,
                                            size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;
    buf[len] = '\0';
    if (sscanf(buf, "%d", &val) > 0) {
        printk("User set iq enable %d\n", val);
        if (val == 0 ) {
            iqe_enable(priv, val);
        } else {
            iqe_enable(priv, 1);
        }
    }
    return count;
}
DEBUGFS_WRITE_FILE_OPS(iq_enable);
#endif

static ssize_t siwifi_dbgfs_stats_read(struct file *file,
                                     char __user *user_buf,
                                     size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char *buf;
    int ret;
#ifdef CONFIG_SIWIFI_SPLIT_TX_BUF
    int per;
#endif
    int i, skipped;
    ssize_t read;
    int bufsz = (NX_TXQ_CNT) * 20 + (ARRAY_SIZE(priv->stats.amsdus_rx) + 1) * 40
        + (ARRAY_SIZE(priv->stats.ampdus_tx) * 30) + 64;

    if (*ppos)
        return 0;

    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (buf == NULL)
        return 0;

    ret = scnprintf(buf, bufsz, "TXQs CFM balances ");
    for (i = 0; i < NX_TXQ_CNT; i++)
        ret += scnprintf(&buf[ret], bufsz - ret,
                         "  [%1d]:%3d", i,
                         priv->stats.cfm_balance[i]);

    ret += scnprintf(&buf[ret], bufsz - ret, "\n");
    ret += scnprintf(&buf[ret], bufsz - ret,
                         " queue stop [%d] start [%d] \n", priv->stats.queues_stops,
                         priv->stats.queues_starts);
#ifdef CONFIG_SIWIFI_SPLIT_TX_BUF
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "\nAMSDU[len]       done         failed   received\n");
    for (i = skipped = 0; i < NX_TX_PAYLOAD_MAX; i++) {
        if (priv->stats.amsdus[i].done) {
            per = DIV_ROUND_UP((priv->stats.amsdus[i].failed) *
                               100, priv->stats.amsdus[i].done);
        } else if (priv->stats.amsdus_rx[i]) {
            per = 0;
        } else {
            per = 0;
            skipped = 1;
            continue;
        }
        if (skipped) {
            ret += scnprintf(&buf[ret], bufsz - ret, "   ...\n");
            skipped = 0;
        }

        ret += scnprintf(&buf[ret], bufsz - ret,
                         "   [%2d]    %10d %8d(%3d%%) %10d\n",  i ? i + 1 : i,
                         priv->stats.amsdus[i].done,
                         priv->stats.amsdus[i].failed, per,
                         priv->stats.amsdus_rx[i]);
    }

    for (; i < ARRAY_SIZE(priv->stats.amsdus_rx); i++) {
        if (!priv->stats.amsdus_rx[i]) {
            skipped = 1;
            continue;
        }
        if (skipped) {
            ret += scnprintf(&buf[ret], bufsz - ret, "   ...\n");
            skipped = 0;
        }

        ret += scnprintf(&buf[ret], bufsz - ret,
                         "   [%2d]                              %10d\n",
                         i + 1, priv->stats.amsdus_rx[i]);
    }
#else
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "\nAMSDU[len]   received\n");
    for (i = skipped = 0; i < ARRAY_SIZE(priv->stats.amsdus_rx); i++) {
        if (!priv->stats.amsdus_rx[i]) {
            skipped = 1;
            continue;
        }
        if (skipped) {
            ret += scnprintf(&buf[ret], bufsz - ret,
                             "   ...\n");
            skipped = 0;
        }

        ret += scnprintf(&buf[ret], bufsz - ret,
                         "   [%2d]    %10d\n",
                         i + 1, priv->stats.amsdus_rx[i]);
    }

#endif /* CONFIG_SIWIFI_SPLIT_TX_BUF */

    ret += scnprintf(&buf[ret], bufsz - ret,
                     "\nAMPDU[len]     done  received\n");
    for (i = skipped = 0; i < ARRAY_SIZE(priv->stats.ampdus_tx); i++) {
        if (!priv->stats.ampdus_tx[i] && !priv->stats.ampdus_rx[i]) {
            skipped = 1;
            continue;
        }
        if (skipped) {
            ret += scnprintf(&buf[ret], bufsz - ret,
                             "    ...\n");
            skipped = 0;
        }

        ret += scnprintf(&buf[ret], bufsz - ret,
                         "   [%2d]   %9d %9d\n", i ? i + 1 : i,
                         priv->stats.ampdus_tx[i], priv->stats.ampdus_rx[i]);
    }

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    siwifi_kfree(buf);

    return read;
}

static ssize_t siwifi_dbgfs_stats_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);

    /* Prevent from interrupt preemption as these statistics are updated under
     * interrupt */
    spin_lock_bh(&priv->tx_lock);

    memset(&priv->stats, 0, sizeof(priv->stats));

    spin_unlock_bh(&priv->tx_lock);

    return count;
}

DEBUGFS_READ_WRITE_FILE_OPS(stats);

static ssize_t siwifi_dbgfs_txpower_lvl_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "txpower_lvl=%d\n", (int)priv->mod_params->txpower_lvl);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_txpower_lvl_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if ((sscanf(buf, "%d", &val) > 0) && (val >= 0) && (val <= 2)){
        printk("user want to set txpower_lvl %d\n", val);
//		siwifi_send_set_power_lvl(siwifi_hw, (u8)siwifi_hw->mod_params->txpower_lvl);
		siwifi_update_txpower_lvl(priv, val);
	}

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(txpower_lvl);

static ssize_t siwifi_dbgfs_fixed_gain_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;
    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "%d\n", (int)priv->fixed_gain);
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    return read;
}
static ssize_t siwifi_dbgfs_fixed_gain_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int gain = 0;
    int temp_ctrl_enable = 0;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;
    buf[len] = '\0';
    if ((sscanf(buf, "%d %d", &gain, &temp_ctrl_enable) > 0)){
        printk("set fixed gain %d tempctrl %d\n", gain, temp_ctrl_enable);
        //use -1 to clear fixed gain in lmac
        if(gain < -1 || gain > 0xFF){
            printk("invalid gain index\n");
        }else{
            priv->fixed_gain = gain;
    		siwifi_send_set_fixed_gain(priv, gain, temp_ctrl_enable);
        }
	}
    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(fixed_gain);

#if defined CONFIG_SIWIFI_IGMP
static ssize_t siwifi_dbgfs_multicast_group_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char *buf;
    int bufsz;
    int ret = 0;
    ssize_t read;
    struct multicast_group * tmp_mul_group;
    int i;

    bufsz = 32 * MAX_MEMBER_IN_MULTICAST * siwifi_get_mc_group_number(priv) + 192;
    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);

    ret += scnprintf(&buf[ret], bufsz - ret,"######## help info ########\n");
    ret += scnprintf(&buf[ret], bufsz - ret,"special multicast group ip\n");
    ret += scnprintf(&buf[ret], bufsz - ret,"224. 0 . 0 .251:mDNS\n");
    ret += scnprintf(&buf[ret], bufsz - ret,"224. 0 . 0 .252:LLMNR\n");
    ret += scnprintf(&buf[ret], bufsz - ret,"239.255.255.250:SSDP\n");
    ret += scnprintf(&buf[ret], bufsz - ret,"###########################\n");

    ret += scnprintf(&buf[ret], bufsz - ret,"multicast group info:\n");

    tmp_mul_group = priv->mul_group;
    while(tmp_mul_group != NULL) {
        ret += scnprintf(&buf[ret], bufsz - ret,
                        "group:%d.%d.%d.%d\n",
                        NIPQUAD(tmp_mul_group->multicast_addr));
        for(i = 0; i < MAX_MEMBER_IN_MULTICAST; i++) {
            if(tmp_mul_group->member[i].member_addr != 0){
                ret += scnprintf(&buf[ret], bufsz - ret,
                                "  member:%d.%d.%d.%d",
                                NIPQUAD(tmp_mul_group->member[i].member_addr));
                ret += scnprintf(&buf[ret], bufsz - ret,
                                "(%x:%x:%x:%x:%x:%x)\n",
                                NMACQUAD(tmp_mul_group->member[i].member_source[0]));
            }
        }
        tmp_mul_group = tmp_mul_group->next_mcg;
    }

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    siwifi_kfree(buf);
    return read;
}

static ssize_t siwifi_dbgfs_multicast_group_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    //struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0){
        printk("do nothing\n");
	}

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(multicast_group);
#endif

#if defined CONFIG_SIWIFI_REPEATER
static ssize_t siwifi_dbgfs_repeater_info_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    extern struct list_head g_rp_info_list;
    struct repeater_info *rp_info;
    struct repeater_sta *sta;
    char *buf;
    int bufsz;
    int ret = 0;
    ssize_t read;
    int i;

    bufsz = 2048;
    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);

    ret += scnprintf(&buf[ret], bufsz - ret,"Repeater Debug Info:\n");

    list_for_each_entry(rp_info, &g_rp_info_list, list) {
        ret += scnprintf(&buf[ret], bufsz - ret,
                "vif_dev %s if_name %pM sta_num %d max_sta %d cur_time %lu\n",
                rp_info->vif_dev->name,rp_info->if_mac, rp_info->sta_num, rp_info->max_sta, jiffies);

        ret += scnprintf(&buf[ret], bufsz - ret, "-----------------  MAC HASH TABLE  -----------------------\n");
        for (i = 0; i < rp_info->arr_len; i++) {
            for (sta = rp_info->mac_head[i]; sta; sta = sta->mac_link) {
#ifdef CONFIG_SIWIFI_REPEATER_IPV6
                ret += scnprintf(&buf[ret], bufsz - ret,
                    "hash_idx %2d mac %pM ip %pi4 ipv6 %pI6 ipv6_lla %pI6 last_use %lu\n",
                            i, &sta->mac, &sta->ip[IPVER_4], &sta->ip[IPVER_6], &sta->ip[IPVER_6_LLA], sta->last_use);

#else
                ret += scnprintf(&buf[ret], bufsz - ret,
                    "hash_idx %2d mac %pM ip %pI4 last_use %lu\n", i, &sta->mac, &sta->ip[IPVER_4], sta->last_use);
#endif
            }
        }

        ret += scnprintf(&buf[ret], bufsz - ret, "-----------------   IP HASH TABLE  -----------------------\n");
        for (i = 0; i < rp_info->arr_len; i++) {
            for (sta = rp_info->ip_head[IPVER_4][i]; sta; sta = sta->ip_link[IPVER_4]) {
                ret += scnprintf(&buf[ret], bufsz - ret,
                    "hash_idx %2d mac %pM ip %pI4 last_use %lu\n", i, &sta->mac, &sta->ip[IPVER_4], sta->last_use);
            }
        }
#ifdef CONFIG_SIWIFI_REPEATER_IPV6
        ret += scnprintf(&buf[ret], bufsz - ret, "----------------- IPv6 HASH TABLE  -----------------------\n");
        for (i = 0; i < rp_info->arr_len; i++) {
            for (sta = rp_info->ip_head[IPVER_6][i]; sta; sta = sta->ip_link[IPVER_6]) {
                ret += scnprintf(&buf[ret], bufsz - ret,
                    "hash_idx %2d mac %pM ipv6 %pI6 last_use %lu\n", i, &sta->mac, &sta->ip[IPVER_6], sta->last_use);
            }
        }
        ret += scnprintf(&buf[ret], bufsz - ret, "----------------- IPv6 HASH TABLE (LLA)  -----------------\n");
        for (i = 0; i < rp_info->arr_len; i++) {
            for (sta = rp_info->ip_head[IPVER_6_LLA][i]; sta; sta = sta->ip_link[IPVER_6_LLA]) {
                ret += scnprintf(&buf[ret], bufsz - ret,
                    "hash_idx %2d mac %pM ip %pI4 last_use %lu\n", i, &sta->mac, &sta->ip[IPVER_6_LLA], sta->last_use);
            }
        }
#endif
    }

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    siwifi_kfree(buf);
    return read;
}

static ssize_t siwifi_dbgfs_repeater_info_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0){
        printk("do nothing\n");
	}

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(repeater_info);
#endif

static ssize_t siwifi_dbgfs_antenna_number_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
//    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "antenna_number=...\n");

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_antenna_number_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if ((sscanf(buf, "%d", &val) > 0) && (val >= 1) && (val <= 2)){
        printk("user want to set antenna_number %d\n", val);
		siwifi_update_antenna_number(priv, val);
	}

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(antenna_number);

extern int set_bcn_ies(struct net_device *dev, int ie_len, void *ie);
static ssize_t siwifi_dbgfs_beacon_insert_info_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    char buf[300];
    int ret = 0;
    ssize_t read;
    size_t bufsz = sizeof(buf)-1;
    int i;

    ret += scnprintf(&buf[ret], bufsz - ret,
            "private_lenth:%d \n", siwifi_hw->beacon_insert_info_len);
    ret += scnprintf(&buf[ret], bufsz - ret, "private_info:");
    for (i = 0; i < siwifi_hw->beacon_insert_info_len; i++) {
        ret += scnprintf(&buf[ret], bufsz - ret, "%02x", siwifi_hw->beacon_insert_info[i]);
    }

    ret += scnprintf(&buf[ret], bufsz - ret, "\n");
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_beacon_insert_info_write(struct file *file,
                                        const char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    struct siwifi_vif *siwifi_vif;
    struct net_device *dev;
    char ie_str[64];
    char ie_fragment[3];
    char info[64];
    int length = 0;
    int  i;
    size_t len = min_t(size_t, count, sizeof(ie_str) - 1);

    memset(ie_str, 0, sizeof(ie_str));
    ie_str[len] = '\0';
    memset(ie_fragment, '\0', sizeof(ie_fragment));
    if (copy_from_user(ie_str, user_buf, len))
        return -EFAULT;

    length = strlen(ie_str) / 2;
    //siwifi_hw->beacon_insert_info_len = strlen(ie_str) / 2;
    for(i = 0; i < length; i++) {
        strncpy(ie_fragment, ie_str + (i * 2), 2);
        sscanf(ie_fragment, "%hhx", &info[i]);
    }

    list_for_each_entry(siwifi_vif, &siwifi_hw->vifs, list) {
        dev = siwifi_vif->ndev;
        if((length > 0) && (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_AP)){
            set_bcn_ies(dev, length, info);
        }
    }

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(beacon_insert_info);

static ssize_t siwifi_dbgfs_assoc_insert_info_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    char buf[300];
    int ret = 0;
    ssize_t read;
    size_t bufsz = sizeof(buf)-1;
    int i;

    ret += scnprintf(&buf[ret], bufsz - ret,
            "private_lenth:%d \n", siwifi_hw->assoc_insert.info_dmalength);
    ret += scnprintf(&buf[ret], bufsz - ret, "private_info:");
    for (i = 0; i < siwifi_hw->assoc_insert.info_dmalength; i++) {
        ret += scnprintf(&buf[ret], bufsz - ret, "%02x", siwifi_hw->assoc_insert_info[i]);
    }

    ret += scnprintf(&buf[ret], bufsz - ret, "\n");
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}
static ssize_t siwifi_dbgfs_assoc_insert_info_write(struct file *file,
                                        const char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    char ie_str[64];
    char info[64];
    char ie_fragment[3];
    int length = 0;
    int  i;
    size_t len = min_t(size_t, count, sizeof(ie_str) - 1);
    memset(ie_str, 0, sizeof(ie_str));
    ie_str[len] = '\0';

    memset(ie_fragment,'\0',sizeof(ie_fragment));
    if (copy_from_user(ie_str, user_buf, len))
        return -EFAULT;

    length = strlen(ie_str) / 2;
    for(i = 0; i < length; i++) {
        strncpy(ie_fragment, ie_str + (i * 2), 2);
        sscanf(ie_fragment, "%hhx", &info[i]);
    }

    siwifi_set_assoc_insert_info(siwifi_hw, info, length);

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(assoc_insert_info);

static ssize_t siwifi_dbgfs_auth_insert_info_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    char buf[300];
    int ret = 0;
    ssize_t read;
    size_t bufsz = sizeof(buf)-1;
    int i;

    ret += scnprintf(&buf[ret], bufsz - ret,
            "private_lenth:%d \n", siwifi_hw->auth_insert.info_dmalength);
    ret += scnprintf(&buf[ret], bufsz - ret, "private_info:");
    for (i = 0; i < siwifi_hw->auth_insert.info_dmalength; i++) {
        ret += scnprintf(&buf[ret], bufsz - ret, "%02x", siwifi_hw->auth_insert_info[i]);
    }

    ret += scnprintf(&buf[ret], bufsz - ret, "\n");
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_auth_insert_info_write(struct file *file,
                                        const char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    char ie_str[64];
    char info[64];
    char ie_fragment[3];
    int length = 0;
    int  i;
    size_t len = min_t(size_t, count, sizeof(ie_str) - 1);

    memset(ie_str, 0, sizeof(ie_str));
    ie_str[len] = '\0';
    memset(ie_fragment,'\0',sizeof(ie_fragment));
    if (copy_from_user(ie_str, user_buf, len))
        return -EFAULT;

    length = strlen(ie_str) / 2;
    for(i = 0; i < length; i++) {
        strncpy(ie_fragment, ie_str + (i * 2), 2);
        sscanf(ie_fragment, "%hhx", &info[i]);
    }

    siwifi_set_auth_insert_info(siwifi_hw, info, length);

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(auth_insert_info);

static ssize_t siwifi_dbgfs_probe_insert_info_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    char buf[300];
    int ret = 0;
    ssize_t read;
    size_t bufsz = sizeof(buf)-1;
    int i;

    ret += scnprintf(&buf[ret], bufsz - ret,
            "private_lenth:%d \n", siwifi_hw->probe_insert_info_len);
    ret += scnprintf(&buf[ret], bufsz - ret, "private_info:");
    for (i = 0; i < siwifi_hw->probe_insert_info_len; i++) {
        ret += scnprintf(&buf[ret], bufsz - ret, "%02x", siwifi_hw->probe_insert_info[i]);
    }

    ret += scnprintf(&buf[ret], bufsz - ret, "\n");
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_probe_insert_info_write(struct file *file,
                                        const char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    char ie_str[64];
    char info[64];
    char ie_fragment[3];
    int length = 0;
    int  i;
    size_t len = min_t(size_t, count, sizeof(ie_str) - 1);

    memset(ie_str, 0, sizeof(ie_str));
    ie_str[len] = '\0';
    memset(ie_fragment,'\0',sizeof(ie_fragment));
    if (copy_from_user(ie_str, user_buf, len))
        return -EFAULT;

    length = strlen(ie_str) / 2;
    for(i = 0; i < length; i++) {
        strncpy(ie_fragment, ie_str + (i * 2), 2);
        sscanf(ie_fragment, "%hhx", &info[i]);
    }

    siwifi_set_probe_insert_info(siwifi_hw, info, length);

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(probe_insert_info);

static ssize_t siwifi_dbgfs_clear_insert_info_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    char buf[64];
    int clear;
    struct net_device *dev;
    struct siwifi_vif * siwifi_vif;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';
    if (sscanf(buf, "%d", &clear) > 0) {
        if(clear){
            list_for_each_entry(siwifi_vif, &siwifi_hw->vifs, list) {
                dev = siwifi_vif->ndev;
                if((SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_AP)){
                    set_bcn_ies(dev, 0, NULL);
                }
            }
            siwifi_set_auth_insert_info(siwifi_hw, NULL, 0);
            siwifi_set_assoc_insert_info(siwifi_hw, NULL, 0);
            siwifi_set_probe_insert_info(siwifi_hw, NULL, 0);
        }
    }

    return count;
}
DEBUGFS_WRITE_FILE_OPS(clear_insert_info);

static ssize_t siwifi_dbgfs_rssi_inbandpower_20_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    //get rssi form inband power 20p
    char buf[100];
    int rssi_20p_ant0,rssi_20p_ant1;
    int ret=0;
    size_t bufsz = sizeof(buf)-1;
    ssize_t read;
    uint32_t *inbandpower20p_addr = (unsigned int *)RIU_AGCINBDPOW20PSTAT_ADDR(priv->mod_params->is_hb);

  //  printk("choose bw = %s addr %p power=%x\n",buf,inbandpower20p_addr,*inbandpower20p_addr);
    rssi_20p_ant0=(char)(((*inbandpower20p_addr) & RIU_INBDPOW20PDBM0_MASK) >> RIU_INBDPOW20PDBM0_LSB);
    rssi_20p_ant1=(char)(((*inbandpower20p_addr) & RIU_INBDPOW20PDBM1_MASK) >> RIU_INBDPOW20PDBM1_LSB);
    if (!priv->mod_params->is_hb)
     ret += scnprintf(&buf[ret], bufsz - ret,"rssi1_inbandpower20p(dBm)=%d rssi2_inbandpower20p(dBm)=%d\n",rssi_20p_ant0 - 5,rssi_20p_ant1 - 5);//lb
    else
    ret += scnprintf(&buf[ret], bufsz - ret,"rssi1_inbandpower20p(dBm)=%d rssi2_inbandpower20p(dBm)=%d\n",rssi_20p_ant0 - 10,rssi_20p_ant1 - 10);//hb

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    return read;
}
DEBUGFS_READ_FILE_OPS(rssi_inbandpower_20);

static ssize_t siwifi_dbgfs_rssi_inbandpower_40_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    //get rssi form inband power 20p
    char buf[100];
    int rssi_20p_ant0,rssi_20p_ant1;
    int ret=0;
    size_t bufsz = sizeof(buf)-1;
    ssize_t read;
    uint32_t *inbandpower20p_addr = (unsigned int *)RIU_AGCINBDPOW20PSTAT_ADDR(priv->mod_params->is_hb);

    rssi_20p_ant0=(char)(((*inbandpower20p_addr) & RIU_INBDPOW20PDBM0_MASK) >> RIU_INBDPOW20PDBM0_LSB);
    rssi_20p_ant1=(char)(((*inbandpower20p_addr) & RIU_INBDPOW20PDBM1_MASK) >> RIU_INBDPOW20PDBM1_LSB);
    if (!priv->mod_params->is_hb)
     ret += scnprintf(&buf[ret], bufsz - ret,"rssi1_inbandpower40(dBm)=%d rssi2_inbandpower40(dBm)=%d\n",rssi_20p_ant0 - 2,rssi_20p_ant1 - 2);//lb
    else
    ret += scnprintf(&buf[ret], bufsz - ret,"rssi1_inbandpower40(dBm)=%d rssi2_inbandpower40(dBm)=%d\n",rssi_20p_ant0 - 7,rssi_20p_ant1 - 7);//hb

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    return read;
}
DEBUGFS_READ_FILE_OPS(rssi_inbandpower_40);

static ssize_t siwifi_dbgfs_rssi_inbandpower_80_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    //get rssi form inband power 20p
    char buf[100];
    int rssi_20p_ant0,rssi_20p_ant1;
    int ret=0;
    size_t bufsz = sizeof(buf)-1;
    ssize_t read;
    uint32_t *inbandpower20p_addr = (unsigned int *)RIU_AGCINBDPOW20PSTAT_ADDR(priv->mod_params->is_hb);

    rssi_20p_ant0=(char)(((*inbandpower20p_addr) & RIU_INBDPOW20PDBM0_MASK) >> RIU_INBDPOW20PDBM0_LSB);
    rssi_20p_ant1=(char)(((*inbandpower20p_addr) & RIU_INBDPOW20PDBM1_MASK) >> RIU_INBDPOW20PDBM1_LSB);
    if (!priv->mod_params->is_hb)
     ret += scnprintf(&buf[ret], bufsz - ret,"lb max  bw is 40M");//lb
    else
    ret += scnprintf(&buf[ret], bufsz - ret,"rssi1_inbandpower80(dBm)=%d rssi2_inbandpower80(dBm)=%d\n",rssi_20p_ant0 - 4,rssi_20p_ant1 - 4);//hb

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    return read;
}
DEBUGFS_READ_FILE_OPS(rssi_inbandpower_80);



static ssize_t siwifi_dbgfs_txpower_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;
    struct siwifi_vif *vif;
    vif = list_first_entry(&priv->vifs, struct siwifi_vif, list);
    if(!vif){
        return 0;
    }

#ifdef CONFIG_SIWIFI_COOLING_TEMP
    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "txpower=%d max=%d\n", vif->txpower_idx,priv->max_txpower_idx);
#else
    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "txpower=%d\n", vif->txpower_idx);
#endif

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

extern int siwifi_debug_set_tx_power(struct wiphy *wiphy, struct wireless_dev *wdev,
                                   enum nl80211_tx_power_setting type, int mbm);

static ssize_t siwifi_dbgfs_txpower_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct siwifi_vif *vif;
    char buf[32];
    int val;
    int ret = 0;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if ((sscanf(buf, "%d", &val) > 0) && (val >= 0) && (val <= 31)){
        printk("user want to set %d power index to rf\n", val);
        list_for_each_entry(vif, &priv->vifs, list) {
            vif->txpower_idx = val;
            ret = siwifi_debug_set_tx_power(priv->wiphy, NULL, NL80211_TX_POWER_FIXED, val * 100);
        }
    }
    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(txpower);

#ifdef CONFIG_SIWIFI_TX_POWER_CALI
static ssize_t siwifi_dbgfs_txpower_cali_enable_read(struct file *file,
                char __user *user_buf,
                size_t count,
                loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct phy_aetnensis_cfg_tag *phy_tag =
                    (struct phy_aetnensis_cfg_tag *)&priv->phy_config;
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count), "power_cali_flag=%d\n", phy_tag->flag);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_txpower_cali_enable_write(struct file *file,
                const char __user *user_buf,
                size_t count,
                loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct phy_aetnensis_cfg_tag *phy_tag = (struct phy_aetnensis_cfg_tag *)&priv->phy_config;
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (!sscanf(buf, "%d", &val)) {
        printk("can not sscanf the buf!\n");
        return -EFAULT;
    }

    phy_tag->flag = val;
    siwifi_send_txpower_cali_enable_req(priv);
    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(txpower_cali_enable);
#endif

static ssize_t siwifi_dbgfs_recovery_enable_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "recovery=%d\n", priv->debugfs.recovery);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_recovery_enable_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0) {
        printk("set the recovery %d\n", val);
        priv->debugfs.recovery = !!val;
    }
    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(recovery_enable);

static ssize_t siwifi_dbgfs_drop_multicast_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "drop_multicast=%d\n", priv->debugfs.drop_multicast);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_drop_multicast_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0) {
        printk("set drop_multicast %d\n", val);
        priv->debugfs.drop_multicast = !!val;
    }
    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(drop_multicast);

#ifdef CONFIG_HEART_BEAT
static ssize_t siwifi_dbgfs_recovery_hb_read(struct file *file,
                char __user *user_buf,
                size_t count,
                loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "recovery_num=%d\n", priv->debugfs.recovery_hb_count);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}
DEBUGFS_READ_FILE_OPS(recovery_hb);
#endif

static ssize_t siwifi_dbgfs_skb_alloc_fail_read(struct file *file,
                char __user *user_buf,
                size_t count,
                loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[128];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "skb-aloc-fail=%d drop=%d\n", priv->debugfs.skb_alloc_fail_cnt, priv->debugfs.skb_drop_cnt);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}
DEBUGFS_READ_FILE_OPS(skb_alloc_fail);

static ssize_t siwifi_dbgfs_run_state_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;
    int run_state = 0;

    if (priv->debugfs.recoverying)
        run_state |= (0x1 << 0);
    if (priv->debugfs.trace_prst)
        run_state |= (0x1 << 1);

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "%d\n", run_state);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}
DEBUGFS_READ_FILE_OPS(run_state);

#ifdef CONFIG_SIWIFI_TX_POWER_CALI
static ssize_t siwifi_dbgfs_cali_table_read(struct file *file,
                char __user *user_buf,
                size_t count,
                loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct phy_aetnensis_cfg_tag *phy_tag = (struct phy_aetnensis_cfg_tag *)&priv->phy_config;
    struct v1_plat_data *plat_data = (struct v1_plat_data *)&priv->plat->priv;
    struct siwifi_factory_info *factory_info = &priv->factory_info;
    uint8_t *tx_pow_cali_gain_list = NULL;
    int channel_lb[13] = {2412, 2417, 2422, 2427, 2432, 2437, 2442, 2447, 2452, 2457, 2462, 2467, 2472};
    int channel_hb[25] = {5180, 5200, 5220, 5240, 5260, 5280, 5300, 5320, 5500, 5520, 5540, 5560, 5580, 5600, 5620, 5640, 5660, 5680, 5700, 5720, 5745, 5765, 5785, 5805, 5825};
    char *mode[3] = {"NOHT", "__HT", "_VHT"};
    int bw[] = {20, 40, 80};
    int rate[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    uint8_t *buf = NULL;
    int ret = 0;
    int bufsz = 0;
    int offset = 0;
    ssize_t read;
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
	//Number of calibration tables
    int cali_table_number = 0;
    int print_calitable_times = 0;
    int rate_number_noht = 12;
    int rate_number_ht = 8;
    int rate_number_vht = 10;
    int rate_number_vht_20 = 9;
    int bw_number_ht = 2;
    int bw_number_vht = 3;
    int mode_number_lb = 2;
    int mode_number_hb = 3;
    int low_band_channel_bufz = 6596;
    int high_band_channel_bufz = 23872;

    cali_table_number = phy_tag->version;
	if(cali_table_number > 2)
		cali_table_number = 2;

    if (plat_data->band & LB_MODULE) {
        bufsz = low_band_channel_bufz * cali_table_number;
        buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
        if (buf == NULL) {
            printk("can't get buf\r\n");
            return 0;
        }
    } else {
        bufsz = high_band_channel_bufz * cali_table_number;
        buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
        if (buf == NULL) {
            printk("can't get buf \r\n");
            return 0;
        }
    }

    while (cali_table_number > 0) {
        tx_pow_cali_gain_list = (uint8_t *)factory_info->normal_txpower_list;
        if (print_calitable_times == 1){
			if(phy_tag->flag & DUAL_ANTENNA_CALIBRATE){
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
				tx_pow_cali_gain_list = (uint8_t *)factory_info->normal_txpower_list_second_antenna;
#endif
			}
			else{
				tx_pow_cali_gain_list = (uint8_t *)factory_info->high_txpower_list;
			}
		}
		if(tx_pow_cali_gain_list == NULL ){
            siwifi_kfree(buf);
			return 0;
		}
        offset = 0;
        if (plat_data->band & LB_MODULE) {
            ret += scnprintf(&buf[ret], bufsz - ret, "CHANNEL  MODE    BW        MCS0  MCS1  MCS2  MCS3  MCS4  MCS5  MCS6  MCS7  MCS8  MCS9  MCS10  MCS11\n");
            for (i = 0; i < sizeof(channel_lb) / sizeof(channel_lb[0]); i++) {
                for (j = 0; j < mode_number_lb; j++) {
                    if (!strncmp(mode[j], "NOHT", 4)) {
                        for (l = 0; l < rate_number_noht; l++) {
                            if (rate[l] == rate[0]) {
                                ret += scnprintf(&buf[ret], bufsz - ret, "%7d  %4s  %3dM  (gain)%4d",
                                                channel_lb[i], mode[0], bw[0], (int)tx_pow_cali_gain_list[offset]);
                                offset++;
                            } else if (rate[l] == rate[11]) {
                                ret += scnprintf(&buf[ret], bufsz - ret, "  %4d\n", (int)tx_pow_cali_gain_list[offset]);
                                offset++;
                            } else {
                                ret += scnprintf(&buf[ret], bufsz - ret, "  %4d", (int)tx_pow_cali_gain_list[offset]);
                                offset++;
                            }
                        }
                    } else if (!strncmp(mode[j], "__HT", 2)) {
                        for (k = 0; k < bw_number_ht; k++) {
                            for (l = 0; l < rate_number_ht; l++) {
                                if (rate[l] == rate[0]) {
                                    ret += scnprintf(&buf[ret], bufsz - ret, "%7d  %4s  %3dM  (gain)%4d",
                                                    channel_lb[i], mode[1], bw[k], (int)tx_pow_cali_gain_list[offset]);
                                    offset++;
                                } else if (rate[l] == rate[7]) {
                                    ret += scnprintf(&buf[ret], bufsz - ret, "  %4d\n", (int)tx_pow_cali_gain_list[offset]);
                                    offset++;
                                } else {
                                    ret += scnprintf(&buf[ret], bufsz - ret, "  %4d", (int)tx_pow_cali_gain_list[offset]);
                                    offset++;
                                }
                            }
                        }
                    }
                }
            }
        } else {
            ret += scnprintf(&buf[ret], bufsz - ret, "CHANNEL  MODE    BW        MCS0  MCS1  MCS2  MCS3  MCS4  MCS5  MCS6  MCS7  MCS8  MCS9\n");
            for (i = 0; i < sizeof(channel_hb) / sizeof(channel_hb[0]); i++) {
                for (j = 0; j < mode_number_hb; j++) {
                    if (!strncmp(mode[j], "NOHT", 4)) {
                        for (l = 0; l < rate_number_ht; l++) {
                            if (rate[l] == rate[0]) {
                                ret += scnprintf(&buf[ret], bufsz - ret, "%7d  %4s  %3dM  (gain)%4d",
                                                channel_hb[i], mode[0], bw[0], (int)tx_pow_cali_gain_list[offset]);
                                offset++;
                            } else if (rate[l] == rate[7]) {
                                ret += scnprintf(&buf[ret], bufsz - ret, "  %4d\n", (int)tx_pow_cali_gain_list[offset]);
                                offset++;
                            } else {
                                ret += scnprintf(&buf[ret], bufsz - ret, "  %4d", (int)tx_pow_cali_gain_list[offset]);
                                offset++;
                            }
                        }
                    } else if (!strncmp(mode[j], "__HT", 2)) {
                        for (k = 0; k < bw_number_ht; k++) {
                            for (l = 0; l < rate_number_ht; l++) {
                                if (rate[l] == rate[0]) {
                                    ret += scnprintf(&buf[ret], bufsz - ret, "%7d  %4s  %3dM  (gain)%4d",
                                                    channel_hb[i], mode[j], bw[k], (int)tx_pow_cali_gain_list[offset]);
                                    offset++;
                                } else if (rate[l] == rate[7]) {
                                    ret += scnprintf(&buf[ret], bufsz - ret, "  %4d\n", (int)tx_pow_cali_gain_list[offset]);
                                    offset++;
                                } else {
                                    ret += scnprintf(&buf[ret], bufsz - ret, "  %4d", (int)tx_pow_cali_gain_list[offset]);
                                    offset++;
                                }
                            }
                        }
                    } else {
                        for (k = 0; k < bw_number_vht; k++) {
                            if (bw[k] == 20) {
                                for (l = 0; l < rate_number_vht_20; l++) {
                                    if (rate[l] == rate[0]) {
                                        ret += scnprintf(&buf[ret], bufsz - ret, "%7d  %4s  %3dM  (gain)%4d",
                                                        channel_hb[i], mode[j], bw[k], (int)tx_pow_cali_gain_list[offset]);
                                        offset++;
                                    } else if (rate[l] == rate[8]) {
                                        ret += scnprintf(&buf[ret], bufsz - ret, "  %4d\n", (int)tx_pow_cali_gain_list[offset]);
                                        offset++;
                                    } else {
                                        ret += scnprintf(&buf[ret], bufsz - ret, "  %4d", (int)tx_pow_cali_gain_list[offset]);
                                        offset++;
                                    }
                                }
                            } else {
                                for (l = 0; l < rate_number_vht; l++) {
                                    if (rate[l] == rate[0]) {
                                        ret += scnprintf(&buf[ret], bufsz - ret, "%7d  %4s  %3dM  (gain)%4d",
                                                        channel_hb[i], mode[j], bw[k], (int)tx_pow_cali_gain_list[offset]);
                                        offset++;
                                    } else if (rate[l] == rate[9]) {
                                        ret += scnprintf(&buf[ret], bufsz - ret, "  %4d\n", (int)tx_pow_cali_gain_list[offset]);
                                        offset++;
                                    } else {
                                        ret += scnprintf(&buf[ret], bufsz - ret, "  %4d", (int)tx_pow_cali_gain_list[offset]);
                                        offset++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        cali_table_number--;
        print_calitable_times++;
    }
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    siwifi_kfree(buf);
    return read;
}
DEBUGFS_READ_FILE_OPS(cali_table);
#endif

static ssize_t siwifi_dbgfs_band_type_read(struct file *file,
                char __user *user_buf,
                size_t count,
                loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    if (!priv->mod_params->is_hb)
        ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count), "lb\n");
    else
        ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count), "hb\n");

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}
DEBUGFS_READ_FILE_OPS(band_type);

void siwifi_dump_lmac_debug_info(struct ipc_shared_env_tag *shared_env_ptr)
{
    int i;
    if(!shared_env_ptr) return;
    printk("dump lmac debug info\n");
    printk("irq runing %d\n", shared_env_ptr->debug_info.lmac_irq_running);
    printk("irq count %d\n", shared_env_ptr->debug_info.lmac_irq_count);
    printk("irq index %d\n", shared_env_ptr->debug_info.lmac_irq);
    printk("evt runing  %d\n", shared_env_ptr->debug_info.lmac_evt_running);
    printk("evt count %d\n", shared_env_ptr->debug_info.lmac_evt_count);
    printk("evt index %d\n", shared_env_ptr->debug_info.lmac_evt);
    printk("msg id %d dst %d src %d\n", shared_env_ptr->debug_info.lmac_msg_id, shared_env_ptr->debug_info.lmac_dest_id, shared_env_ptr->debug_info.lmac_src_id);
    printk("evt record index %d\n", shared_env_ptr->debug_info.lmac_evt_idx);
    for (i = 0; i < MAX_DBG_EVT_CNT/ 5; i++)
    {
        printk("%d %d %d %d %d\n",
                shared_env_ptr->debug_info.lmac_evt_record[i * 5], shared_env_ptr->debug_info.lmac_evt_record[i * 5 + 1],
                shared_env_ptr->debug_info.lmac_evt_record[i * 5 + 2], shared_env_ptr->debug_info.lmac_evt_record[i * 5 + 3], shared_env_ptr->debug_info.lmac_evt_record[i * 5 + 4]);
    }
}
static ssize_t siwifi_dbgfs_dump_lmac_debuginfo_read(struct file *file,
                char __user *user_buf,
                size_t count,
                loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct ipc_host_env_tag *env = NULL;
    struct ipc_shared_env_tag *shared_env_ptr = NULL;
    char buf[32];
    int ret;
    ssize_t read;
    if (*ppos)
        return 0;
    env = priv->ipc_env;
    if(!env)
        return 0;
    shared_env_ptr = env->shared;
    if(!shared_env_ptr)
        return 0;
    if (!priv->mod_params->is_hb)
        ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count), "lb\n");
    else
        ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count), "hb\n");
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    siwifi_dump_lmac_debug_info(shared_env_ptr);
    return read;
}
DEBUGFS_READ_FILE_OPS(dump_lmac_debuginfo);

#ifdef CONFIG_DEBUG_TXQ_STOP
extern void siwifi_print_txq_trace(void);
static ssize_t siwifi_dbgfs_txq_status_read(struct file *file,
                char __user *user_buf,
                size_t count,
                loff_t *ppos)
{
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count), "txq stas:\n");

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    siwifi_print_txq_trace();
    return read;
}
DEBUGFS_READ_FILE_OPS(txq_status);
#endif

static ssize_t siwifi_dbgfs_version_read(struct file *file,
                char __user *user_buf,
                size_t count,
                loff_t *ppos)
{
    char buf[64];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count), "%d.%d.%03d\n", SIWIFI_VERSION_MAIN_HED, SIWIFI_VERSION_MAIN_MID, SIWIFI_VERSION_MAIN_SUB);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}
DEBUGFS_READ_FILE_OPS(version);

static ssize_t siwifi_dbgfs_compile_time_read(struct file *file,
                char __user *user_buf,
                size_t count,
                loff_t *ppos)
{
    char buf[64];
    int ret;
    ssize_t read;
    char compile_time[64];
    struct rtc_time tm;
    //CST = UTC +8:00
    unsigned long raw_time = CONFIG_COMPILE_TIME + 8 * 3600;

    rtc_time_to_tm(raw_time, &tm);
    sprintf(compile_time, "%04d-%02d-%02d %02d:%02d:%02d",tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count), "version:%d.%d.%03d, time:%s\n", SIWIFI_VERSION_MAIN_HED, SIWIFI_VERSION_MAIN_MID, SIWIFI_VERSION_MAIN_SUB,compile_time);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}
DEBUGFS_READ_FILE_OPS(compile_time);

#define PRINT_MIB_ATTR_ARRAY(band,MIB_ATTR_NAME, STR) \
    scnprintf(&buf[ret], bufsz - ret, "%s(0x%04x)                    %u %u %u %u %u %u %u %u\n", STR, REG_MIB_BASE(band) - REG_MAC_CORE_BASE_ADDR(band) + offsetof(struct machw_mib_tag, MIB_ATTR_NAME), mib->MIB_ATTR_NAME[0], mib->MIB_ATTR_NAME[1], \
             mib->MIB_ATTR_NAME[2],  mib->MIB_ATTR_NAME[3],  mib->MIB_ATTR_NAME[4],  mib->MIB_ATTR_NAME[5],  mib->MIB_ATTR_NAME[6], mib->MIB_ATTR_NAME[7])

#define PRINT_MIB_ATTR(band,MIB_ATTR_NAME, STR) \
    scnprintf(&buf[ret], bufsz - ret, "%s(0x%04x)          %10u\n", STR, REG_MIB_BASE(band) - REG_MAC_CORE_BASE_ADDR(band) + offsetof(struct machw_mib_tag, MIB_ATTR_NAME), mib->MIB_ATTR_NAME)

static ssize_t siwifi_dbgfs_mib_read(struct file *file,
                char __user *user_buf,
                size_t count,
                loff_t *ppos)
{
    char *buf;
    int ret;
    ssize_t read;
    size_t bufsz = 4096;
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct machw_mib_tag *mib = (struct machw_mib_tag*)(REG_MIB_BASE(priv->mod_params->is_hb));

    /* everything is read in one go */
    if (*ppos)
        return 0;

    bufsz = min_t(size_t, bufsz, count * 2);
    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (buf == NULL)
        return 0;
    bufsz--;

    ret = scnprintf(buf, bufsz, "mib table\n");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_wep_excluded_count, "dot11_wep_excluded_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_fcs_error_count, "dot11_fcs_error_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,nx_rx_phy_error_count, "nx_rx_phy_error_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,nx_rd_fifo_overflow_count, "nx_rd_fifo_overflow_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,nx_tx_underun_count, "nx_tx_underun_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,nx_rx_mpif_overflow_count, "nx_rx_mpif_overflow_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,nx_qos_utransmitted_mpdu_count, "nx_qos_utransmitted_mpdu_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,nx_qos_gtransmitted_mpdu_count, "nx_qos_gtransmitted_mpdu_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,dot11_qos_failed_count, "dot11_qos_failed_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,dot11_qos_retry_count, "dot11_qos_retry_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,dot11_qos_rts_success_count, "dot11_qos_rts_success_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,dot11_qos_rts_failure_count, "dot11_qos_rts_failure_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,nx_qos_ack_failure_count, "nx_qos_ack_failure_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,nx_qos_ureceived_mpdu_count, "nx_qos_ureceived_mpdu_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,nx_qos_greceived_mpdu_count, "nx_qos_greceived_mpdu_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,nx_qos_ureceived_other_mpdu, "nx_qos_ureceived_other_mpdu");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,dot11_qos_retries_received_count, "dot11_qos_retries_received_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,nx_utransmitted_amsdu_count, "nx_utransmitted_amsdu_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,nx_gtransmitted_amsdu_count, "nx_gtransmitted_amsdu_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,dot11_failed_amsdu_count, "dot11_failed_amsdu_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,dot11_retry_amsdu_count, "dot11_retry_amsdu_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,dot11_transmitted_octets_in_amsdu, "dot11_transmitted_octets_in_amsdu");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,dot11_amsdu_ack_failure_count, "dot11_amsdu_ack_failure_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,nx_ureceived_amsdu_count, "nx_ureceived_amsdu_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,nx_greceived_amsdu_count, "nx_greceived_amsdu_count");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,nx_ureceived_other_amsdu, "nx_ureceived_other_amsdu");
    ret += PRINT_MIB_ATTR_ARRAY(priv->mod_params->is_hb,dot11_received_octets_in_amsdu_count, "dot11_received_octets_in_amsdu_count");

    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_transmitted_ampdu_count, "dot11_transmitted_ampdu_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_transmitted_mpdus_in_ampdu_count, "dot11_transmitted_mpdus_in_ampdu_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_transmitted_octets_in_ampdu_count, "dot11_transmitted_octets_in_ampdu_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,wnlu_ampdu_received_count, "wnlu_ampdu_received_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,nx_gampdu_received_count, "nx_gampdu_received_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,nx_other_ampdu_received_count, "nx_other_ampdu_received_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_mpdu_in_received_ampdu_count, "dot11_mpdu_in_received_ampdu_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_received_octets_in_ampdu_count, "dot11_received_octets_in_ampdu_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_ampdu_delimiter_crc_error_count, "dot11_ampdu_delimiter_crc_error_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_implicit_bar_failure_count, "dot11_implicit_bar_failure_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_explicit_bar_failure_count, "dot11_explicit_bar_failure_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_20mhz_frame_transmitted_count, "dot11_20mhz_frame_transmitted_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_40mhz_frame_transmitted_count, "dot11_40mhz_frame_transmitted_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_80mhz_frame_transmitted_count, "dot11_80mhz_frame_transmitted_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_20mhz_frame_received_count, "dot11_20mhz_frame_received_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_40mhz_frame_received_count, "dot11_40mhz_frame_received_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_80mhz_frame_received_count, "dot11_80mhz_frame_received_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,nx_failed_20mhz_txop, "nx_failed_20mhz_txop");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,nx_successful_txops_20, "nx_successful_txops_20");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,nx_failed_40mhz_txop, "nx_failed_40mhz_txop");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,nx_successful_txops_40, "nx_successful_txops_40");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,nx_failed_80mhz_txop, "nx_failed_80mhz_txop");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,nx_successful_txops_80, "nx_successful_txops_80");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_stbc_cts_success_count, "dot11_stbc_cts_success_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_stbc_cts_failure_count, "dot11_stbc_cts_failure_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_non_stbc_cts_success_count, "dot11_non_stbc_cts_success_count");
    ret += PRINT_MIB_ATTR(priv->mod_params->is_hb,dot11_non_stbc_cts_failure_count, "dot11_non_stbc_cts_failure_count");

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    siwifi_kfree(buf);
    return read;
}
DEBUGFS_READ_FILE_OPS(mib);

static ssize_t siwifi_dbgfs_cde_info_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    /* Forward the information to the LMAC */
    siwifi_send_read_cde_info(priv);
    return 0;
}
DEBUGFS_READ_FILE_OPS(cde_info);

#define TXQ_STA_PREF "tid|"
#define TXQ_STA_PREF_FMT "%3d|"

#define TXQ_VIF_PREF "type|"
#define TXQ_VIF_PREF_FMT "%4s|"

#ifdef KEEP_EARLY_SKB
#define TXQ_HDR     "idx| status  |credit|ready|retry|last_stop|pushed|early|ps_drop1|ps_drop2|ps_drop3| address|   total|  success|   stuck"
#define TXQ_HDR_FMT "%3d|%s%s%s%s%s%s%s%s%s|%6d|%5d|%5d|%9d|%6d|%5d|%8d|%8d|%8d|%p|%8d|%8d|%8d"
#elif defined(CONFIG_BRIDGE_ACCELERATE)
#define TXQ_HDR "idx| status  |credit|ready|retry|last_stop|pushed|fastready|ps_drop1|ps_drop2|ps_drop3| address|   total| success|   stuck"
#define TXQ_HDR_FMT "%3d|%s%s%s%s%s%s%s%s%s|%6d|%5d|%5d|%9d|%6d|%9d|%8d|%8d|%8d|%p|%8d|%8d|%8d"
#else
#define TXQ_HDR "idx| status  |credit|ready|retry|last_stop|pushed|ps_drop1|ps_drop2|ps_drop3| address|   total|  success|   stuck"
#define TXQ_HDR_FMT "%3d|%s%s%s%s%s%s%s%s%s|%6d|%5d|%5d|%9d|%6d|%8d|%8d|%8d|%p|%8d|%8d|%8d"
#endif

#ifdef CONFIG_SIWIFI_AMSDUS_TX

#define TXQ_HDR_SUFF "|amsdu|max_nb"
#define TXQ_HDR_SUFF_FMT "|%5d|%5d"
#else
#define TXQ_HDR_SUFF ""
#define TXQ_HDR_SUF_FMT ""
#endif /* CONFIG_SIWIFI_AMSDUS_TX */

#define TXQ_HDR_MAX_LEN (sizeof(TXQ_STA_PREF) + sizeof(TXQ_HDR) + sizeof(TXQ_HDR_SUFF) + 1)


#define PS_HDR  "Legacy PS: ready=%d, sp=%d / UAPSD: ready=%d, sp=%d"
#define PS_HDR_LEGACY "Legacy PS: ready=%d, sp=%d"
#define PS_HDR_UAPSD  "UAPSD: ready=%d, sp=%d"
#define PS_HDR_MAX_LEN  sizeof("Legacy PS: ready=xxx, sp=xxx / UAPSD: ready=xxx, sp=xxx\n")

#define STA_HDR "** STA %d (%pM)\n"
#define STA_HDR_MAX_LEN (sizeof("- STA xx (xx:xx:xx:xx:xx:xx)\n") + PS_HDR_MAX_LEN)


#define VIF_HDR "* VIF [%d] %s\n"
#define VIF_HDR_MAX_LEN sizeof(VIF_HDR) + IFNAMSIZ

#ifdef CONFIG_SIWIFI_AMSDUS_TX


#define VIF_SEP "---------------------------------------\n"

#else /* ! CONFIG_SIWIFI_AMSDUS_TX */
#define VIF_SEP "---------------------------------\n"
#endif /* CONFIG_SIWIFI_AMSDUS_TX*/

#define VIF_SEP_LEN sizeof(VIF_SEP)

#define CAPTION "status: L=in hwq list, F=stop full, P=stop sta PS, V=stop vif PS, C=stop channel, S=stop CSA, M=stop MU, W=flow ctrl, R=Memory ctrl"
#define CAPTION_LEN sizeof(CAPTION)

#define UPDATE_IDLETIME_CNT_LEN 35

#define STA_TXQ 0
#define VIF_TXQ 1

static int siwifi_dbgfs_txq(char *buf, size_t size, struct siwifi_txq *txq, int type, int tid, char *name)
{
    int res, idx = 0;

    if (type == STA_TXQ) {
        res = scnprintf(&buf[idx], size, TXQ_STA_PREF_FMT, tid);
        idx += res;
        size -= res;
    } else {
        res = scnprintf(&buf[idx], size, TXQ_VIF_PREF_FMT, name);
        idx += res;
        size -= res;
    }

    res = scnprintf(&buf[idx], size, TXQ_HDR_FMT, txq->idx,
                    (txq->status & SIWIFI_TXQ_IN_HWQ_LIST) ? "L" : " ",
                    (txq->status & SIWIFI_TXQ_STOP_FULL) ? "F" : " ",
                    (txq->status & SIWIFI_TXQ_STOP_STA_PS) ? "P" : " ",
                    (txq->status & SIWIFI_TXQ_STOP_VIF_PS) ? "V" : " ",
                    (txq->status & SIWIFI_TXQ_STOP_CHAN) ? "C" : " ",
                    (txq->status & SIWIFI_TXQ_STOP_CSA) ? "S" : " ",
                    (txq->status & SIWIFI_TXQ_STOP_MU_POS) ? "M" : " ",
                    (txq->status & SIWIFI_TXQ_NDEV_FLOW_CTRL) ? "W" : " ",
                    (txq->status & SIWIFI_TXQ_MEMORY_CTRL) ? "R" : " ",
                    txq->credits, skb_queue_len(&txq->sk_list),
                    txq->nb_retry,
                    txq->last_stop_pos,
                    txq->pkt_pushed[0],
#ifdef KEEP_EARLY_SKB
                    skb_queue_len(&txq->early_sk_list),
#endif
#ifdef CONFIG_BRIDGE_ACCELERATE
                    skb_queue_len(&txq->accel_sk_list),
#endif
                    txq->ps_on_drop,
                    txq->ps_active_drop,
                    txq->ps_off_drop,
                    txq,
                    txq->pkt_send_total,
                    txq->pkt_send_success,
                    txq->stuck_time);
    idx += res;
    size -= res;

#ifdef CONFIG_SIWIFI_AMSDUS_TX
    if (type == STA_TXQ) {
        res = scnprintf(&buf[idx], size, TXQ_HDR_SUFF_FMT,
                        txq->amsdu_len, txq->amsdu_maxnb
                        );
        idx += res;
        size -= res;
    }
#endif

    res = scnprintf(&buf[idx], size, "\n");
    idx += res;
    size -= res;

    return idx;
}

static int siwifi_dbgfs_txq_sta(char *buf, size_t size, struct siwifi_sta *siwifi_sta,
                              struct siwifi_hw *siwifi_hw)
{
    int tid, res, idx = 0;
    struct siwifi_txq *txq;

    res = scnprintf(&buf[idx], size, "\n" STA_HDR,
                    siwifi_sta->sta_idx,
                    siwifi_sta->mac_addr
                    );
    idx += res;
    size -= res;


    if (siwifi_sta->ps.active) {
        if (siwifi_sta->uapsd_tids &&
            (siwifi_sta->uapsd_tids == ((1 << NX_NB_TXQ_PER_STA) - 1)))
            res = scnprintf(&buf[idx], size, PS_HDR_UAPSD "\n",
                            siwifi_sta->ps.pkt_ready[UAPSD_ID],
                            siwifi_sta->ps.sp_cnt[UAPSD_ID]);
        else if (siwifi_sta->uapsd_tids)
            res = scnprintf(&buf[idx], size, PS_HDR "\n",
                            siwifi_sta->ps.pkt_ready[LEGACY_PS_ID],
                            siwifi_sta->ps.sp_cnt[LEGACY_PS_ID],
                            siwifi_sta->ps.pkt_ready[UAPSD_ID],
                            siwifi_sta->ps.sp_cnt[UAPSD_ID]);
        else
            res = scnprintf(&buf[idx], size, PS_HDR_LEGACY "\n",
                            siwifi_sta->ps.pkt_ready[LEGACY_PS_ID],
                            siwifi_sta->ps.sp_cnt[LEGACY_PS_ID]);
        idx += res;
        size -= res;
    } else {
        res = scnprintf(&buf[idx], size, "\n");
        idx += res;
        size -= res;
    }

    res = scnprintf(&buf[idx], size, "update idletime count:%u\n", siwifi_sta->update_time_count);
    idx += res;
    size -= res;
    res = scnprintf(&buf[idx], size, TXQ_STA_PREF TXQ_HDR TXQ_HDR_SUFF "\n");
    idx += res;
    size -= res;


    foreach_sta_txq(siwifi_sta, txq, tid, siwifi_hw) {
        if(txq->pkt_send_total == 0) continue;
        res = siwifi_dbgfs_txq(&buf[idx], size, txq, STA_TXQ, tid, NULL);
        idx += res;
        size -= res;
    }

    return idx;
}

static int siwifi_dbgfs_txq_vif(char *buf, size_t size, struct siwifi_vif *siwifi_vif,
                              struct siwifi_hw *siwifi_hw)
{
    int res, idx = 0;
    struct siwifi_txq *txq;
    struct siwifi_sta *siwifi_sta;


    if (!siwifi_vif->ndev || !siwifi_vif->up)
        return 0;
    res = scnprintf(&buf[idx], size, VIF_HDR, siwifi_vif->vif_index, siwifi_vif->ndev->name);
    idx += res;
    size -= res;
    if (SIWIFI_VIF_TYPE(siwifi_vif) ==  NL80211_IFTYPE_AP ||
        SIWIFI_VIF_TYPE(siwifi_vif) ==  NL80211_IFTYPE_P2P_GO ||
        SIWIFI_VIF_TYPE(siwifi_vif) ==  NL80211_IFTYPE_MESH_POINT) {
        res = scnprintf(&buf[idx], size, TXQ_VIF_PREF TXQ_HDR "\n");
        idx += res;
        size -= res;
        txq = siwifi_txq_vif_get(siwifi_vif, NX_UNK_TXQ_TYPE);
        res = siwifi_dbgfs_txq(&buf[idx], size, txq, VIF_TXQ, 0, "UNK");
        idx += res;
        size -= res;
        txq = siwifi_txq_vif_get(siwifi_vif, NX_BCMC_TXQ_TYPE);
        res = siwifi_dbgfs_txq(&buf[idx], size, txq, VIF_TXQ, 0, "BCMC");
        idx += res;
        size -= res;
        siwifi_sta = &siwifi_hw->sta_table[siwifi_vif->ap.bcmc_index];
        if (siwifi_sta->ps.active) {
            res = scnprintf(&buf[idx], size, PS_HDR_LEGACY "\n",
                            siwifi_sta->ps.pkt_ready[LEGACY_PS_ID],
                            siwifi_sta->ps.sp_cnt[LEGACY_PS_ID]);
            idx += res;
            size -= res;
        } else {
            res = scnprintf(&buf[idx], size, "\n");
            idx += res;
            size -= res;
        }

        list_for_each_entry(siwifi_sta, &siwifi_vif->ap.sta_list, list) {
            res = siwifi_dbgfs_txq_sta(&buf[idx], size, siwifi_sta, siwifi_hw);
            idx += res;
            size -= res;
        }
    } else if (SIWIFI_VIF_TYPE(siwifi_vif) ==  NL80211_IFTYPE_STATION ||
               SIWIFI_VIF_TYPE(siwifi_vif) ==  NL80211_IFTYPE_P2P_CLIENT) {
        if (siwifi_vif->sta.ap) {
            res = siwifi_dbgfs_txq_sta(&buf[idx], size, siwifi_vif->sta.ap, siwifi_hw);
            idx += res;
            size -= res;
        }
    }
    return idx;
}

void siwifi_txq_dump_info(struct siwifi_hw *siwifi_hw, char *buf, size_t size, int *idx)
{
    int res;
    size_t bufsz;
    struct siwifi_vif *vif;

    bufsz = size;
    *idx = 0;

    res = scnprintf(&buf[*idx], bufsz, CAPTION);
    *idx += res;
    bufsz -= res;

    //spin_lock_bh(&siwifi_hw->tx_lock);
    list_for_each_entry(vif, &siwifi_hw->vifs, list) {
        res = scnprintf(&buf[*idx], bufsz, "\n"VIF_SEP);
        *idx += res;
        bufsz -= res;
        res = siwifi_dbgfs_txq_vif(&buf[*idx], bufsz, vif, siwifi_hw);
        *idx += res;
        bufsz -= res;
        res = scnprintf(&buf[*idx], bufsz, VIF_SEP);
        *idx += res;
        bufsz -= res;
        if (bufsz <= 0)
        {
            printk("need to increase txq dump buffer size\n");
            break;
        }
    }
    //spin_unlock_bh(&siwifi_hw->tx_lock);
}


static ssize_t siwifi_dbgfs_txq_read(struct file *file ,
                                   char __user *user_buf,
                                   size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    char *buf;
    int idx;
    ssize_t read;
    size_t bufsz = ((NX_VIRT_DEV_MAX * (VIF_HDR_MAX_LEN + 2 * VIF_SEP_LEN)) +
                    (NX_REMOTE_STA_MAX * STA_HDR_MAX_LEN) +
                    ((NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX + NX_NB_TXQ) *
                     TXQ_HDR_MAX_LEN) + CAPTION_LEN + UPDATE_IDLETIME_CNT_LEN);

    /* everything is read in one go */
    if (*ppos)
        return 0;
    printk("sz= %d\n", bufsz);
    bufsz = min_t(size_t, bufsz, count);
    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (buf == NULL)
        return 0;

    bufsz--;
    siwifi_txq_dump_info(siwifi_hw, buf, bufsz, &idx);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, idx);
    siwifi_kfree(buf);

    return read;
}
DEBUGFS_READ_FILE_OPS(txq);

static ssize_t siwifi_dbgfs_repeater_status_read(struct file *file ,
                                   char __user *user_buf,
                                   size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct siwifi_vif *siwifi_vif, *__siwifi_vif;
    char buf[128];
    int ret = 0;
    ssize_t read;
    size_t bufsz = sizeof(buf)-1;

    list_for_each_entry_safe(siwifi_vif, __siwifi_vif, &priv->vifs, list) {
        if (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_STATION) {
            switch (siwifi_vif->repeater_status)
            {
                case REPEATER_VIF_UP:
                    ret += scnprintf(&buf[ret], bufsz - ret,
                            "repeater sta_vif up!\n");
                    break;
                case REPEATER_AUTH:
                    ret += scnprintf(&buf[ret], bufsz - ret,
                            "repeater auth!\n");
                    break;
                case REPEATER_ASSOC:
                    ret += scnprintf(&buf[ret], bufsz - ret,
                            "repeater assoc!\n");
                    break;
                case REPEATER_EAPOL:
                    ret += scnprintf(&buf[ret], bufsz - ret,
                            "repeater eapol!\n");
                    break;
                default:
                    ret += scnprintf(&buf[ret], bufsz - ret,
                            "repeater fail\n");
                    break;
            }
        }
    }

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}
DEBUGFS_READ_FILE_OPS(repeater_status);

static ssize_t siwifi_dbgfs_siwifi_static_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
    char *buf;
    int ret = 0;
    ssize_t read;
    int bufsz = 4096;

    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (!buf)
        return -ENOMEM;

    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "nothing \n");

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    siwifi_kfree(buf);
    return read;
}

static ssize_t siwifi_dbgfs_siwifi_static_write(struct file *file, const char __user *user_buf,
                                          size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = file->private_data;
    char buf[255];
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    int value, result;

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    result = sscanf(buf, "%d", &value);
    if (result) {
        if (value == 1) {
            siwifi_static_timer_clear(priv);
            siwifi_static_timer_start(priv);
        } else if (value >= 100) {
            siwifi_static_timer_print(priv, (value - 100));
        }
    }

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(siwifi_static);

static ssize_t siwifi_dbgfs_enable_atf_read(struct file *file,
                                   char __user *user_buf,
                                   size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "atf_enable = %d\n", priv->atf.enable);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_enable_atf_write(struct file *file,
                                    const char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val = 0;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0) {
        if(val == 1) {
            printk("enable atf(airtime fairness)\n");
            priv->atf.enable = 1;
        } else {
            printk("disable atf(airtime fairness)\n");
            priv->atf.enable = 0;
        }
    }
    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(enable_atf);

static ssize_t siwifi_dbgfs_atf_read(struct file *file ,
                                   char __user *user_buf,
                                   size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    char *buf = NULL;
    int idx = 0;
    int res = 0;
    ssize_t read;
    size_t bufsz = 64 * NX_REMOTE_STA_MAX * NX_NB_TID_PER_STA;
    struct siwifi_vif *vif = NULL;
    struct siwifi_sta *siwifi_sta = NULL;
    int tid = 0;
    struct siwifi_txq *txq = NULL;
    /* everything is read in one go */
    if (*ppos)
        return 0;
    printk("sz= %d\n", bufsz);
    bufsz = min_t(size_t, bufsz, count);
    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (buf == NULL)
        return 0;

    bufsz--;

    res = scnprintf(&buf[idx], bufsz, "max_rateinfo %8u update_cnt %8u\n",
                                    siwifi_hw->atf.max_rateinfo,
                                    siwifi_hw->atf.update_cnt);
    idx += res;
    bufsz -= res;

    list_for_each_entry(vif, &siwifi_hw->vifs, list) {
        if (SIWIFI_VIF_TYPE(vif) ==  NL80211_IFTYPE_AP){
            list_for_each_entry(siwifi_sta, &vif->ap.sta_list, list) {
                res = scnprintf(&buf[idx], bufsz, "\nsta %d(%pM)\n", siwifi_sta->sta_idx, siwifi_sta->mac_addr);
                idx += res;
                bufsz -= res;
                res = scnprintf(&buf[idx], bufsz, "idx atf_record get_token skip_token last_get last_skip have_sent\n");
                idx += res;
                bufsz -= res;
                foreach_sta_txq(siwifi_sta, txq, tid, siwifi_hw) {
                    if (txq->atf.rateinfo == 0 || txq->atf.enable == 0)
                        continue;
                    res = scnprintf(&buf[idx], bufsz, "%3d   %8u  %8u   %8u %8u  %8u  %8u\n",
                                    txq->idx,
                                    txq->atf.record_rateinfo,
                                    txq->atf.debug_get_token_cnt,
                                    txq->atf.debug_skip_token_cnt,
                                    txq->atf.debug_get_token_cnt_last,
                                    txq->atf.debug_skip_token_cnt_last,
                                    txq->atf.have_sent
                                    );
                    idx += res;
                    bufsz -= res;
                }
            }
        }
    }

    read = simple_read_from_buffer(user_buf, count, ppos, buf, idx);
    siwifi_kfree(buf);

    return read;
}
DEBUGFS_READ_FILE_OPS(atf);

static ssize_t siwifi_dbgfs_txq_time_stat_read(struct file *file ,
                                   char __user *user_buf,
                                   size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    char *buf = NULL;
    int idx = 0;
    int res = 0;
    ssize_t read;
    size_t bufsz = 64 * NX_REMOTE_STA_MAX * NX_NB_TID_PER_STA;
    struct siwifi_vif *vif = NULL;
    struct siwifi_sta *siwifi_sta = NULL;
    int tid = 0;
    struct siwifi_txq *txq = NULL;
    /* everything is read in one go */
    if (*ppos)
        return 0;
    printk("sz= %d\n", bufsz);
    bufsz = min_t(size_t, bufsz, count);
    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (buf == NULL)
        return 0;

    bufsz--;

    list_for_each_entry(vif, &siwifi_hw->vifs, list) {
        if (SIWIFI_VIF_TYPE(vif) ==  NL80211_IFTYPE_AP){
            list_for_each_entry(siwifi_sta, &vif->ap.sta_list, list) {
                res = scnprintf(&buf[idx], bufsz, "\nsta %d(%pM)\n", siwifi_sta->sta_idx, siwifi_sta->mac_addr);
                idx += res;
                bufsz -= res;
                res = scnprintf(&buf[idx], bufsz, "idx  0ms   ~  10ms  ~  20ms  ~  50ms  ~  100ms+  .. total    retry come_xmit free_xmit\n");
                idx += res;
                bufsz -= res;
                res = scnprintf(&buf[idx], bufsz, "last timer\n");
                idx += res;
                bufsz -= res;
                foreach_sta_txq(siwifi_sta, txq, tid, siwifi_hw) {
                    if (txq->last_timer_time_stat.inlmac_total == 0)
                        continue;
                    res = scnprintf(&buf[idx], bufsz, "%3d %8u %8u %8u %8u %8u %8u %8u  %8u  %8u\n",
                                    txq->idx,
                                    txq->last_timer_time_stat.inlmac_0ms,
                                    txq->last_timer_time_stat.inlmac_10ms,
                                    txq->last_timer_time_stat.inlmac_20ms,
                                    txq->last_timer_time_stat.inlmac_50ms,
                                    txq->last_timer_time_stat.inlmac_100ms,
                                    txq->last_timer_time_stat.inlmac_total,
                                    txq->last_timer_time_stat.inlmac_retry,
                                    txq->last_timer_time_stat.come_xmit,
                                    txq->last_timer_time_stat.free_xmit);
                    idx += res;
                    bufsz -= res;
                }
                res = scnprintf(&buf[idx], bufsz, "total\n");
                idx += res;
                bufsz -= res;
                foreach_sta_txq(siwifi_sta, txq, tid, siwifi_hw) {
                    if (txq->time_stat.inlmac_total == 0)
                        continue;
                    res = scnprintf(&buf[idx], bufsz, "%3d %8u %8u %8u %8u %8u %8u %8u  %8u  %8u\n",
                                    txq->idx,
                                    txq->time_stat.inlmac_0ms,
                                    txq->time_stat.inlmac_10ms,
                                    txq->time_stat.inlmac_20ms,
                                    txq->time_stat.inlmac_50ms,
                                    txq->time_stat.inlmac_100ms,
                                    txq->time_stat.inlmac_total,
                                    txq->time_stat.inlmac_retry,
                                    txq->time_stat.come_xmit,
                                    txq->time_stat.free_xmit
                                    );
                    idx += res;
                    bufsz -= res;
                }
            }
        }
    }

    read = simple_read_from_buffer(user_buf, count, ppos, buf, idx);
    siwifi_kfree(buf);

    return read;
}
DEBUGFS_READ_FILE_OPS(txq_time_stat);

static int siwifi_dbgfs_ndevq_vif(char *buf, size_t size, struct siwifi_vif *siwifi_vif,
                              struct siwifi_hw *siwifi_hw)
{
    int res, idx = 0;
	struct netdev_queue *ndevq = NULL;
	int i = 0;

	if (!siwifi_vif->up || siwifi_vif->ndev == NULL)
        return idx;
    res = scnprintf(&buf[idx], size, VIF_HDR, siwifi_vif->vif_index, siwifi_vif->ndev->name);
    idx += res;
    size -= res;

	for (i = 0; i < siwifi_hw->tx_queue_num; i++)
	{
		ndevq = netdev_get_tx_queue(siwifi_vif->ndev, i);
        res = scnprintf(&buf[idx], size,
                         "  [%1d]:%3lu", i,
                         ndevq->state);
		idx += res;
		size -= res;
	}

    return idx;
}

static ssize_t siwifi_dbgfs_ndevq_read(struct file *file ,
                                   char __user *user_buf,
                                   size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    struct siwifi_vif *vif;
    char *buf;
    int idx, res;
    ssize_t read;
    size_t bufsz = 1024;

    /* everything is read in one go */
    if (*ppos)
        return 0;

    bufsz = min_t(size_t, bufsz, count);
    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (buf == NULL)
        return 0;

    bufsz--;
    idx = 0;

    res = scnprintf(&buf[idx], bufsz, CAPTION);
    idx += res;
    bufsz -= res;

    //spin_lock_bh(&siwifi_hw->tx_lock);
    list_for_each_entry(vif, &siwifi_hw->vifs, list) {
        res = scnprintf(&buf[idx], bufsz, "\n"VIF_SEP);
        idx += res;
        bufsz -= res;
        res = siwifi_dbgfs_ndevq_vif(&buf[idx], bufsz, vif, siwifi_hw);
        idx += res;
        bufsz -= res;
        res = scnprintf(&buf[idx], bufsz, "\n"VIF_SEP);
        idx += res;
        bufsz -= res;
    }
    //spin_unlock_bh(&siwifi_hw->tx_lock);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, idx);
    siwifi_kfree(buf);

    return read;
}
DEBUGFS_READ_FILE_OPS(ndevq);

static ssize_t siwifi_dbgfs_trx_stats_read(struct file *file,
                                     char __user *user_buf,
                                     size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char *buf;
    int ret;
    struct siwifi_vif *vif;
    ssize_t read;
    int bufsz = 4096;
    if (*ppos)
        return 0;

    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (buf == NULL)
        return 0;

    ret = scnprintf(buf, bufsz, "dump trx status\n");


	ret += scnprintf(&buf[ret], bufsz - ret,
                     "mpdu last rx      %lu\n",
                     priv->stats.last_rx);
	ret += scnprintf(&buf[ret], bufsz - ret,
                     "mgmt rx           %9d\n",
                     priv->stats.mgmt_rx);
	ret += scnprintf(&buf[ret], bufsz - ret,
                     "spurious rx       %9d\n",
                     priv->stats.spurious_rx);
	ret += scnprintf(&buf[ret], bufsz - ret,
                     "delete rx         %9d\n",
                     priv->stats.delete_rx);
	ret += scnprintf(&buf[ret], bufsz - ret,
                     "monitor rx        %9d\n",
                     priv->stats.monitor_rx);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "mon_vif close rx  %9d\n",
                     priv->stats.rx_monitor_interface_close);
	ret += scnprintf(&buf[ret], bufsz - ret,
                     "len_update rx     %9d\n",
                     priv->stats.len_update_rx);
	ret += scnprintf(&buf[ret], bufsz - ret,
                     "rxdataind         %9llu\n",
                     priv->stats.rxind_cnts);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "rx mpdu missed    %9u\n",
                     priv->stats.ampdus_rx_miss);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "rx ldpc           %9u\n",
                     priv->stats.ldpc_rx);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "rx bcc            %9u\n",
                     priv->stats.bcc_rx);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "rx stbc           %9u\n",
                     priv->stats.stbc_rx);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "ipc pending cnt   %9u\n",
                     priv->stats.ipc_pending_cnt);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "rx buf alloc fail %9u\n",
                     priv->rx_skb_alloc_fail_cnt);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "rx pkt            %9llu\n",
                     priv->stats.total_rx);

    ret += scnprintf(&buf[ret], bufsz - ret,
                     "mpdu tx retry     %9d\n",
                     priv->stats.tx_retry);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "single tx retry   %9d\n",
                     priv->stats.single_retry);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "mpdu tx failed    %9d\n",
                     priv->stats.tx_failed);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "mpdu tx flush     %9d\n",
                     priv->stats.tx_flush);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "mpdu tx discard   %9d\n",
                     priv->stats.tx_discard);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "tx copy expand    %9d\n",
                     priv->stats.tx_copy_expand);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "tx drop full      %9d\n",
                     priv->stats.tx_drop_full);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "tx drop sta null  %9d\n",
                     priv->stats.tx_drop_sta_null);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "tx drop multicast  %9d\n",
                     priv->stats.tx_drop_multicast);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "tx drop inactive  %9d\n",
                     priv->stats.tx_drop_txq_inactive);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "tx drop hdr fail  %9d\n",
                     priv->stats.tx_drop_hdr_alloc_fail);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "tx drop prepare   %9d\n",
                     priv->stats.tx_drop_prep_tx);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "tx pkt            %9llu\n",
                     priv->stats.tx_pkt);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "reord_alloc_cnt   %9d\n",
                     priv->reord_alloc_cnt);
    ret += scnprintf(&buf[ret], bufsz - ret,
                     "reord_release_cnt   %9d\n",
                     priv->reord_release_cnt);
    //dump vif
    list_for_each_entry(vif, &priv->vifs, list) {
        ret += scnprintf(&buf[ret], bufsz - ret, "VIF[%d]: %s\n", vif->vif_index, vif->ndev->name);
	    ret += scnprintf(&buf[ret], bufsz - ret,
                     "  RX bcn:%9d non:%9d ft:%9d mgmt: %9d\n", vif->mgmt_stats.rx_bcn_cnts,
                     vif->mgmt_stats.rx_non_cnts, vif->mgmt_stats.rx_ft_cnts, vif->mgmt_stats.rx_mgmt_cnts);
	    ret += scnprintf(&buf[ret], bufsz - ret,
                     "  mgmt-tx:     %9d \n  mgmt-tx-drop:%9d \n", vif->mgmt_stats.tx_mgmt, vif->mgmt_stats.tx_mgmt_drop);
    }
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);


    siwifi_kfree(buf);

    return read;
}
DEBUGFS_READ_FILE_OPS(trx_stats);


static ssize_t siwifi_dbgfs_rxdesc_read(struct file *file,
                                     char __user *user_buf,
                                     size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct ipc_host_env_tag *env = NULL;
    struct ipc_shared_env_tag *shared_env_ptr = NULL;
    char *buf;
    int ret, i;
    ssize_t read;
    int bufsz = 4096;

    if(!priv)
        return 0;
    if (*ppos)
        return 0;
    env = priv->ipc_env;
    if(!env)
        return 0;
    shared_env_ptr = env->shared;
    if(!shared_env_ptr)
        return 0;
    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (buf == NULL)
        return 0;

    ret = scnprintf(buf, bufsz, "dump rx desc\n");

    printk("dump ipc_shared host_rxbuf>>>>>>>ipc_host_rxbuf_idx=%d\n", env->ipc_host_rxbuf_idx);
    for(i = 0; i < IPC_RXBUF_CNT; i++)
    {
        printk("host_rxbuf[%d].hostid=%x .dma_addr=%x\n", i , shared_env_ptr->host_rxbuf[i].hostid,
                shared_env_ptr->host_rxbuf[i].dma_addr);
    }

    printk("dump ipc_shared host_rxdesc>>>>>>>>>ipc_host_rxdesc_idx=%d\n", env->ipc_host_rxdesc_idx);
    for(i = 0; i < IPC_RXDESC_CNT; i++)
    {
        printk("host_rxdesc[%d].hostid=%p dma_addr=%x shared_dma_addr=%x\n", i , env->ipc_host_rxdesc_array[i].hostid,
                env->ipc_host_rxdesc_array[i].dma_addr, shared_env_ptr->host_rxdesc[i].dma_addr);
    }
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    siwifi_kfree(buf);
    return read;
}
DEBUGFS_READ_FILE_OPS(rxdesc);

static ssize_t siwifi_dbgfs_tx_ctrl_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    sscanf(buf, "%d", &val);

    /*RM#1003479 in order to ensure throughput, the test does not need to close an antenna,
    *if need restore  can change 65 to NX_TXDESC_CNT0 
	*/
    if (val == 65) {
        siwifi_update_antenna_number(priv, 1);
    } else if (0 <= val && val < NX_TXDESC_CNT0) {
        //siwifi_update_antenna_number(priv, 2);
        printk("[DEBUG] ---reach limit temp, start ctrl--- \n");
    } else {
        printk("Input error !\n");
        return count;
    }

    priv->tx_ctrl = val;
    spin_lock_bh(&priv->tx_lock);
    siwifi_adjust_hwq_credits(priv, (s8)val);
    spin_unlock_bh(&priv->tx_lock);

    return count;
}

static ssize_t siwifi_dbgfs_tx_ctrl_read(struct file *file,
        char __user *user_buf,
        size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char *buf;
    int ret;
    ssize_t read;
    int bufsz = 64;

    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (buf == NULL)
        return 0;

    ret = scnprintf(buf, bufsz, "%u\n", priv->tx_ctrl);
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    siwifi_kfree(buf);

    return read;
}
DEBUGFS_READ_WRITE_FILE_OPS(tx_ctrl);

static ssize_t siwifi_dbgfs_rx_ctrl_write(struct file *file,
        const char __user *user_buf,
        size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    sscanf(buf, "%d", &val);
    if (val < 0) {
        priv->rx_ctrl = 0;
        REG_PL_WR(MDM_RXCTRL1_ADDR(priv->mod_params->is_hb), 0x0C300C30);
        REG_PL_WR(MDM_RXCTRL3_ADDR(priv->mod_params->is_hb), 0x0C300C30);
    } else if (val >= 100) {
        priv->rx_ctrl = 99;
        REG_PL_WR(MDM_RXCTRL1_ADDR(priv->mod_params->is_hb), 0x00200020);
        REG_PL_WR(MDM_RXCTRL3_ADDR(priv->mod_params->is_hb), 0x00200020);
    } else {
        priv->rx_ctrl = val;
        if (priv->mod_params->is_hb) {
            REG_PL_WR(MDM_RXCTRL1_ADDR(priv->mod_params->is_hb), ((3120 * (100 - val)) / 100) | (((3120 * (100 - val)) / 100) << 16));
            REG_PL_WR(MDM_RXCTRL3_ADDR(priv->mod_params->is_hb), ((3120 * (100 - val)) / 100) | (((3120 * (100 - val)) / 100) << 16));
        } else {
            REG_PL_WR(MDM_RXCTRL1_ADDR(priv->mod_params->is_hb), ((1080 * (100 - val)) / 100) | (((1080 * (100 - val)) / 100) << 16));
            REG_PL_WR(MDM_RXCTRL3_ADDR(priv->mod_params->is_hb), ((1080 * (100 - val)) / 100) | (((1080 * (100 - val)) / 100) << 16));
        }
    }

    return count;
}

static ssize_t siwifi_dbgfs_rx_ctrl_read(struct file *file,
        char __user *user_buf,
        size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char *buf;
    int ret;
    ssize_t read;
    int bufsz = 128;

    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (buf == NULL)
        return 0;

    if (priv->mod_params->is_hb)
        ret = scnprintf(buf, bufsz, "The limiting rate is %d%% of the maximum rate (866 Mbits/s).\n", 100 - priv->rx_ctrl);
    else
        ret = scnprintf(buf, bufsz, "The limiting rate is %d%% of the maximum rate (300 Mbits/s).\n", 100 - priv->rx_ctrl);
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    siwifi_kfree(buf);

    return read;
}
DEBUGFS_READ_WRITE_FILE_OPS(rx_ctrl);

#if defined CONFIG_SIWIFI_CH_OFFSET
static ssize_t siwifi_dbgfs_ch_offset_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    char buf[32];
    int ch_offset;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    struct mm_set_channel_cfm cfm;
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &ch_offset) > 0){
        printk("[debug] ready to set channel offset %d\n", ch_offset);
        siwifi_hw->ch_offset = ch_offset;
        siwifi_send_set_channel_offset(siwifi_hw, 0, &cfm, ch_offset);
    }

    return count;
}

static ssize_t siwifi_dbgfs_ch_offset_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    char buf[32];
    int ret;
    ssize_t read;
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                "ch_offset %d\n", siwifi_hw->ch_offset);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

DEBUGFS_READ_WRITE_FILE_OPS(ch_offset);

#endif

//debug purpos
void siwifi_hwq_dump_info(struct siwifi_hw *siwifi_hw, char *buf, size_t size, int *idx)
{
    int bufsz = size;
    int id, res;
    struct siwifi_hwq *hwq;

    *idx = 0;
    for (id = ARRAY_SIZE(siwifi_hw->hwq) - 1; id >= 0 ; id--) {
        hwq = &siwifi_hw->hwq[id];
        res = scnprintf(&buf[*idx], bufsz, "hwq[%d] informations: \n", id);
        *idx += res;
        bufsz -= res;

        res = scnprintf(&buf[*idx], bufsz,"        credits : %d\n"
                                          "        size : %d\n"
                                          "        need_processing : %d\n",
                                          hwq->credits[0], hwq->size, hwq->need_processing);
        *idx += res;
        bufsz -= res;
#ifdef TOKEN_ENABLE
        res = scnprintf(&buf[*idx], bufsz,"        token: %d\n"
                                          "        status 0 : %d\n"
                                          "        status 1 : %d\n",
                                          hwq->outstanding_tokens, hwq->token_status[0], hwq->token_status[1]);
        *idx += res;
        bufsz -= res;
#endif
		res = scnprintf(&buf[*idx], bufsz,"        cfm cnt: %d\n"
										  "        push_cnt : %d\n",
                                          hwq->cfm_cnt, hwq->push_cnt);
        *idx += res;
        bufsz -= res;
        if (bufsz <= 0)
        {
            printk("increase hwq buffer size\n");
            break;
        }
    }
    return ;
}


static ssize_t siwifi_dbgfs_hwq_read(struct file *file ,
                                   char __user *user_buf,
                                   size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    char *buf;
    int idx;
    ssize_t read;
    size_t bufsz = 1024;

    /* everything is read in one go */
    if (*ppos)
        return 0;

    bufsz = min_t(size_t, bufsz, count);
    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (buf == NULL)
        return 0;

    bufsz--;
    siwifi_hwq_dump_info(siwifi_hw, buf, bufsz, &idx);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, idx);
    siwifi_kfree(buf);

    return read;
}
DEBUGFS_READ_FILE_OPS(hwq);

#ifdef CONFIG_ERROR_DUMP
static ssize_t siwifi_dbgfs_error_dump_read(struct file *file,
                                     char __user *user_buf,
                                     size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    ssize_t read = 0;
    int i,j,k;
    int bufsz = 0;
    int ret = 0;
    int show_num = 0;
    char *buf;
    unsigned long time;
    mutex_lock(&priv->dbgdump_elem.mutex);
    if(priv->dbgdump_elem.err_total_count == 0){
        mutex_unlock(&priv->dbgdump_elem.mutex);
        return read;
    }

    for (i = 0; i < ERROR_INFO_MAX_NUM; i ++) {
        if(priv->dbgdump_elem.errorinfo[i]->count != 0){
            show_num += priv->dbgdump_elem.errorinfo[i]->count;
        }
    }
    bufsz = show_num * (ERROR_BUF_MAX_SIZE + 512);
    buf = siwifi_kzalloc(bufsz, GFP_ATOMIC);
    if(!buf) {
        printk("alloc err buffer fail(%d)\n",priv->mod_params->is_hb);
        goto ERROR;
    }

    for (i = 0; i < ERROR_INFO_MAX_NUM; i ++) {
        if(priv->dbgdump_elem.errorinfo[i]->count != 0){
            ret += scnprintf(&buf[ret],bufsz - ret, "[%d]:%d\n", i,priv->dbgdump_elem.errorinfo[i]->count);
        }
    }
    ret += scnprintf(&buf[ret],bufsz - ret, "Total error:%d,show the last %d\n", priv->dbgdump_elem.err_total_count, show_num);

    j = 0;
    if (priv->dbgdump_elem.err_total_count > show_num) {
        for (i = priv->dbgdump_elem.err_cur_idx + 1; i < ERROR_INFO_MAX_NUM; i++) {
            if (!priv->dbgdump_elem.errorinfo[i]){
                printk("dbgdump_elem.errorinfo[%d] is null\n",i);
                mutex_unlock(&priv->dbgdump_elem.mutex);
                goto ERROR;
            }
            if (!priv->dbgdump_elem.errorinfo[i]->buf){
                printk("dbgdump_elem.errorinfo[%d]->buf is null\n",i);
                mutex_unlock(&priv->dbgdump_elem.mutex);
                goto ERROR;
            }
            for(k = 0; k < priv->dbgdump_elem.errorinfo[i]->count; k++){
                time = jiffies_to_msecs(priv->dbgdump_elem.errorinfo[i]->time[k]);
                ret += scnprintf(&buf[ret],bufsz - ret, "[%lu.%lu] %s\n",time / 1000, time % 1000, priv->dbgdump_elem.errorinfo[i]->buf);
                j++;
            }
        }
    }

    for (i = 0; i < priv->dbgdump_elem.err_cur_idx + 1; i++){
        if (!priv->dbgdump_elem.errorinfo[i]){
            printk("dbgdump_elem.errorinfo[%d] is null\n",i);
            mutex_unlock(&priv->dbgdump_elem.mutex);
            goto ERROR;
        }
        if (!priv->dbgdump_elem.errorinfo[i]->buf){
            printk("dbgdump_elem.errorinfo[%d]->buf is null\n",i);
            mutex_unlock(&priv->dbgdump_elem.mutex);
            goto ERROR;
        }
        for(k = 0; k < priv->dbgdump_elem.errorinfo[i]->count; k++){
            time = jiffies_to_msecs(priv->dbgdump_elem.errorinfo[i]->time[k]);
            ret += scnprintf(&buf[ret],bufsz - ret, "[%lu.%lu] %s\n",time / 1000, time % 1000, priv->dbgdump_elem.errorinfo[i]->buf);
            j++;
        }
    }
    mutex_unlock(&priv->dbgdump_elem.mutex);
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
ERROR:
    if(buf)
        siwifi_kfree(buf);
    return read;
}
DEBUGFS_READ_FILE_OPS(error_dump);
#endif

static ssize_t siwifi_dbgfs_acsinfo_read(struct file *file,
                                           char __user *user_buf,
                                           size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct wiphy *wiphy = priv->wiphy;
    char *buf;
    ssize_t ret;
    int bufsz = (SCAN_CHANNEL_MAX + 1) * 43;
    int survey_cnt = 0;
    int len = 0;
    int band, chan_cnt;

    buf = (char *)siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (!buf)
        return 0;
    mutex_lock(&priv->dbgdump_elem.mutex);

    len += scnprintf(buf, min_t(size_t, bufsz - 1, count),
                     "FREQ    TIME(ms)    BUSY(ms)    NOISE(dBm)\n");

    for (band = NL80211_BAND_2GHZ; band <= NL80211_BAND_5GHZ; band++) {
        if(!wiphy->bands[band])
            continue;
        for (chan_cnt = 0; chan_cnt < wiphy->bands[band]->n_channels; chan_cnt++) {
            struct siwifi_survey_info *p_survey_info = &priv->survey[survey_cnt];
            struct ieee80211_channel *p_chan = &wiphy->bands[band]->channels[chan_cnt];

            if (p_survey_info->filled) {
                len += scnprintf(&buf[len], min_t(size_t, bufsz - len - 1, count),
                                 "%d    %03d         %03d         %d\n",
                                 p_chan->center_freq,
                                 p_survey_info->chan_time_ms,
                                 p_survey_info->chan_time_busy_ms,
                                 p_survey_info->noise_dbm);
            } else {
                len += scnprintf(&buf[len], min_t(size_t, bufsz -len -1, count),
                                 "%d    NOT AVAILABLE\n",
                                 p_chan->center_freq);
            }

            survey_cnt++;
        }
    }

    mutex_unlock(&priv->dbgdump_elem.mutex);
    ret = simple_read_from_buffer(user_buf, count, ppos, buf, len);
    siwifi_kfree(buf);
    return ret;
}
DEBUGFS_READ_FILE_OPS(acsinfo);

static ssize_t siwifi_dbgfs_fw_dbg_read(struct file *file,
                                           char __user *user_buf,
                                           size_t count, loff_t *ppos)
{
    char help[]="usage: [MOD:<ALL|KE|DBG|IPC|DMA|MM|TX|RX|PHY>]* "
        "[DBG:<NONE|CRT|ERR|WRN|INF|VRB>]\n";

    return simple_read_from_buffer(user_buf, count, ppos, help, sizeof(help));
}


static ssize_t siwifi_dbgfs_fw_dbg_write(struct file *file,
                                            const char __user *user_buf,
                                            size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int idx = 0;
    u32 mod = 0;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;
    buf[len] = '\0';

#define SIWIFI_MOD_TOKEN(str, val)                                        \
    if (strncmp(&buf[idx], str, sizeof(str) - 1 ) == 0) {               \
        idx += sizeof(str) - 1;                                         \
        mod |= val;                                                     \
        continue;                                                       \
    }

#define SIWIFI_DBG_TOKEN(str, val)                                \
    if (strncmp(&buf[idx], str, sizeof(str) - 1) == 0) {        \
        idx += sizeof(str) - 1;                                 \
        dbg = val;                                              \
        goto dbg_done;                                          \
    }

    while ((idx + 4) < len) {
        if (strncmp(&buf[idx], "MOD:", 4) == 0) {
            idx += 4;
            SIWIFI_MOD_TOKEN("ALL", 0xffffffff);
            SIWIFI_MOD_TOKEN("KE",  BIT(0));
            SIWIFI_MOD_TOKEN("DBG", BIT(1));
            SIWIFI_MOD_TOKEN("IPC", BIT(2));
            SIWIFI_MOD_TOKEN("DMA", BIT(3));
            SIWIFI_MOD_TOKEN("MM",  BIT(4));
            SIWIFI_MOD_TOKEN("TX",  BIT(5));
            SIWIFI_MOD_TOKEN("RX",  BIT(6));
            SIWIFI_MOD_TOKEN("PHY", BIT(7));
            idx++;
        } else if (strncmp(&buf[idx], "DBG:", 4) == 0) {
            u32 dbg = 0;
            idx += 4;
            SIWIFI_DBG_TOKEN("NONE", 0);
            SIWIFI_DBG_TOKEN("CRT",  1);
            SIWIFI_DBG_TOKEN("ERR",  2);
            SIWIFI_DBG_TOKEN("WRN",  3);
            SIWIFI_DBG_TOKEN("INF",  4);
            SIWIFI_DBG_TOKEN("VRB",  5);
            idx++;
            continue;
          dbg_done:
            siwifi_send_dbg_set_sev_filter_req(priv, dbg);
        } else {
            idx++;
        }
    }

    if (mod) {
        siwifi_send_dbg_set_mod_filter_req(priv, mod);
    }

    return count;
}

DEBUGFS_READ_WRITE_FILE_OPS(fw_dbg);

static ssize_t siwifi_dbgfs_sys_stats_read(struct file *file,
                                         char __user *user_buf,
                                         size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[3*64];
    int len = 0;
    ssize_t read;
    int error = 0;
    struct dbg_get_sys_stat_cfm cfm;
    u32 sleep_int, sleep_frac, doze_int, doze_frac;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Get the information from the FW */
    if ((error = siwifi_send_dbg_get_sys_stat_req(priv, &cfm)))
        return error;

    if (cfm.stats_time == 0)
        return 0;

    sleep_int = ((cfm.cpu_sleep_time * 100) / cfm.stats_time);
    sleep_frac = (((cfm.cpu_sleep_time * 100) % cfm.stats_time) * 10) / cfm.stats_time;
    doze_int = ((cfm.doze_time * 100) / cfm.stats_time);
    doze_frac = (((cfm.doze_time * 100) % cfm.stats_time) * 10) / cfm.stats_time;

    len += scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                     "\nSystem statistics:\n");
    len += scnprintf(&buf[len], min_t(size_t, sizeof(buf) - 1, count),
                     "  CPU sleep [%%]: %d.%d\n", sleep_int, sleep_frac);
    len += scnprintf(&buf[len], min_t(size_t, sizeof(buf) - 1, count),
                     "  Doze      [%%]: %d.%d\n", doze_int, doze_frac);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, len);

    return read;
}

DEBUGFS_READ_FILE_OPS(sys_stats);

#ifdef CONFIG_SIWIFI_MUMIMO_TX
static ssize_t siwifi_dbgfs_mu_group_read(struct file *file,
                                        char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    struct siwifi_mu_info *mu = &siwifi_hw->mu;
    struct siwifi_mu_group *group;
    size_t bufsz = NX_MU_GROUP_MAX * sizeof("xx = (xx - xx - xx - xx)\n") + 50;
    char *buf;
    int j, res, idx = 0;

    if (*ppos)
        return 0;

    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (buf == NULL)
        return 0;

    res = scnprintf(&buf[idx], bufsz, "MU Group list (%d groups, %d users max)\n",
                    NX_MU_GROUP_MAX, CONFIG_USER_MAX);
    idx += res;
    bufsz -= res;

    list_for_each_entry(group, &mu->active_groups, list) {
        if (group->user_cnt) {
            res = scnprintf(&buf[idx], bufsz, "%2d = (", group->group_id);
            idx += res;
            bufsz -= res;
            for (j = 0; j < (CONFIG_USER_MAX - 1) ; j++) {
                if (group->users[j])
                    res = scnprintf(&buf[idx], bufsz, "%2d - ",
                                    group->users[j]->sta_idx);
                else
                    res = scnprintf(&buf[idx], bufsz, ".. - ");

                idx += res;
                bufsz -= res;
            }

            if (group->users[j])
                res = scnprintf(&buf[idx], bufsz, "%2d)\n",
                                group->users[j]->sta_idx);
            else
                res = scnprintf(&buf[idx], bufsz, "..)\n");

            idx += res;
            bufsz -= res;
        }
    }

    res = simple_read_from_buffer(user_buf, count, ppos, buf, idx);
    siwifi_kfree(buf);

    return res;
}

DEBUGFS_READ_FILE_OPS(mu_group);
#endif

#ifdef CONFIG_SIWIFI_P2P_DEBUGFS
static ssize_t siwifi_dbgfs_oppps_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *rw_hw = file->private_data;
    struct siwifi_vif *rw_vif;
    char buf[32];
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    int ctw;

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;
    buf[len] = '\0';

    /* Read the written CT Window (provided in ms) value */
    if (sscanf(buf, "ctw=%d", &ctw) > 0) {
        /* Check if at least one VIF is configured as P2P GO */
        list_for_each_entry(rw_vif, &rw_hw->vifs, list) {
            if (SIWIFI_VIF_TYPE(rw_vif) == NL80211_IFTYPE_P2P_GO) {
                struct mm_set_p2p_oppps_cfm cfm;

                /* Forward request to the embedded and wait for confirmation */
                siwifi_send_p2p_oppps_req(rw_hw, rw_vif, (u8)ctw, &cfm);

                break;
            }
        }
    }

    return count;
}

DEBUGFS_WRITE_FILE_OPS(oppps);

static ssize_t siwifi_dbgfs_noa_write(struct file *file,
                                    const char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *rw_hw = file->private_data;
    struct siwifi_vif *rw_vif;
    char buf[64];
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    int noa_count, interval, duration, dyn_noa;

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;
    buf[len] = '\0';

    /* Read the written NOA information */
    if (sscanf(buf, "count=%d interval=%d duration=%d dyn=%d",
               &noa_count, &interval, &duration, &dyn_noa) > 0) {
        /* Check if at least one VIF is configured as P2P GO */
        list_for_each_entry(rw_vif, &rw_hw->vifs, list) {
            if (SIWIFI_VIF_TYPE(rw_vif) == NL80211_IFTYPE_P2P_GO) {
                struct mm_set_p2p_noa_cfm cfm;

                /* Forward request to the embedded and wait for confirmation */
                siwifi_send_p2p_noa_req(rw_hw, rw_vif, noa_count, interval,
                                      duration, (dyn_noa > 0),  &cfm);

                break;
            }
        }
    }

    return count;
}

DEBUGFS_WRITE_FILE_OPS(noa);
#endif /* CONFIG_SIWIFI_P2P_DEBUGFS */

#ifdef CONFIG_SIWIFI_FW_TRACE
struct siwifi_dbgfs_fw_trace {
    struct siwifi_fw_trace_local_buf lbuf;
    struct siwifi_fw_trace *trace;
};

static int siwifi_dbgfs_fw_trace_open(struct inode *inode, struct file *file)
{
    struct siwifi_dbgfs_fw_trace *ltrace = siwifi_kmalloc(sizeof(*ltrace), GFP_KERNEL);
    struct siwifi_hw *priv = inode->i_private;

    if (!ltrace)
        return -ENOMEM;

    if (siwifi_fw_trace_alloc_local(&ltrace->lbuf, 5120)) {
        siwifi_kfree(ltrace);
    }

    ltrace->trace = &priv->debugfs.fw_trace;
    file->private_data = ltrace;
    return 0;
}

static int siwifi_dbgfs_fw_trace_release(struct inode *inode, struct file *file)
{
    struct siwifi_dbgfs_fw_trace *ltrace = file->private_data;

    if (ltrace) {
        siwifi_fw_trace_free_local(&ltrace->lbuf);
        siwifi_kfree(ltrace);
    }

    return 0;
}

static ssize_t siwifi_dbgfs_fw_trace_read(struct file *file,
                                        char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_dbgfs_fw_trace *ltrace = file->private_data;
    return siwifi_fw_trace_read(ltrace->trace, &ltrace->lbuf,
                              (file->f_flags & O_NONBLOCK), user_buf, count);
}

static ssize_t siwifi_dbgfs_fw_trace_write(struct file *file,
                                         const char __user *user_buf,
                                         size_t count, loff_t *ppos)
{
    struct siwifi_dbgfs_fw_trace *ltrace = file->private_data;
    int ret;

    ret = _siwifi_fw_trace_reset(ltrace->trace, true);
    if (ret)
        return ret;

    return count;
}

DEBUGFS_READ_WRITE_OPEN_RELEASE_FILE_OPS(fw_trace);

static ssize_t siwifi_dbgfs_fw_trace_level_read(struct file *file,
                                              char __user *user_buf,
                                              size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    return siwifi_fw_trace_level_read(&priv->debugfs.fw_trace, user_buf,
                                    count, ppos);
}

static ssize_t siwifi_dbgfs_fw_trace_level_write(struct file *file,
                                               const char __user *user_buf,
                                               size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    return siwifi_fw_trace_level_write(&priv->debugfs.fw_trace, user_buf, count);
}
DEBUGFS_READ_WRITE_FILE_OPS(fw_trace_level);
#endif


#ifdef CONFIG_SIWIFI_RADAR
static ssize_t siwifi_dbgfs_pulses_read(struct file *file,
                                      char __user *user_buf,
                                      size_t count, loff_t *ppos,
                                      int rd_idx)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char *buf;
    int len = 0;
    int bufsz;
    int i;
    int index;
    struct siwifi_radar_pulses *p = &priv->radar.pulses[rd_idx];
    ssize_t read;

    if (*ppos != 0)
        return 0;

    /* Prevent from interrupt preemption */
    spin_lock_bh(&priv->radar.lock);
    bufsz = p->count * 34 + 51;
    bufsz += siwifi_radar_dump_pattern_detector(NULL, 0, &priv->radar, rd_idx);
    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (buf == NULL) {
        spin_unlock_bh(&priv->radar.lock);
        return 0;
    }

    if (p->count) {
        len += scnprintf(&buf[len], bufsz - len,
                         " PRI     WIDTH     FOM     FREQ\n");
        index = p->index;
        for (i = 0; i < p->count; i++) {
            struct radar_pulse *pulse;

            if (index > 0)
                index--;
            else
                index = SIWIFI_RADAR_PULSE_MAX - 1;

            pulse = (struct radar_pulse *) &p->buffer[index];

            len += scnprintf(&buf[len], bufsz - len,
                             "%05dus  %03dus     %2d%%    %+3dMHz\n", pulse->rep,
                             2 * pulse->len, 6 * pulse->fom, 2*pulse->freq);
        }
    }

    len += siwifi_radar_dump_pattern_detector(&buf[len], bufsz - len,
                                            &priv->radar, rd_idx);

    spin_unlock_bh(&priv->radar.lock);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, len);

    siwifi_kfree(buf);

    return read;
}

static ssize_t siwifi_dbgfs_pulses_prim_read(struct file *file,
                                           char __user *user_buf,
                                           size_t count, loff_t *ppos)
{
    return siwifi_dbgfs_pulses_read(file, user_buf, count, ppos, 0);
}

DEBUGFS_READ_FILE_OPS(pulses_prim);

static ssize_t siwifi_dbgfs_pulses_sec_read(struct file *file,
                                          char __user *user_buf,
                                          size_t count, loff_t *ppos)
{
    return siwifi_dbgfs_pulses_read(file, user_buf, count, ppos, 1);
}

DEBUGFS_READ_FILE_OPS(pulses_sec);

static ssize_t siwifi_dbgfs_detected_read(struct file *file,
                                        char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char *buf;
    int bufsz,len = 0;
    ssize_t read;

    if (*ppos != 0)
        return 0;

    bufsz = 5; // RIU:\n
    bufsz += siwifi_radar_dump_radar_detected(NULL, 0, &priv->radar,
                                            SIWIFI_RADAR_RIU);

    if (priv->phy_cnt > 1) {
        bufsz += 5; // FCU:\n
        bufsz += siwifi_radar_dump_radar_detected(NULL, 0, &priv->radar,
                                                SIWIFI_RADAR_FCU);
    }

    buf = siwifi_kmalloc(bufsz, GFP_KERNEL);
    if (buf == NULL) {
        return 0;
    }

    len = scnprintf(&buf[len], bufsz, "RIU:\n");
    len += siwifi_radar_dump_radar_detected(&buf[len], bufsz - len, &priv->radar,
                                            SIWIFI_RADAR_RIU);

    if (priv->phy_cnt > 1) {
        len += scnprintf(&buf[len], bufsz - len, "FCU:\n");
        len += siwifi_radar_dump_radar_detected(&buf[len], bufsz - len,
                                              &priv->radar, SIWIFI_RADAR_FCU);
    }

    read = simple_read_from_buffer(user_buf, count, ppos, buf, len);

    siwifi_kfree(buf);

    return read;
}

DEBUGFS_READ_FILE_OPS(detected);

static ssize_t siwifi_dbgfs_enable_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "RIU=%d FCU=%d\n", priv->radar.dpd[SIWIFI_RADAR_RIU]->enabled,
                    priv->radar.dpd[SIWIFI_RADAR_FCU]->enabled);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_enable_write(struct file *file,
                                     const char __user *user_buf,
                                     size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "RIU=%d", &val) > 0)
        siwifi_radar_detection_enable(&priv->radar, val, SIWIFI_RADAR_RIU);

    if (sscanf(buf, "FCU=%d", &val) > 0)
        siwifi_radar_detection_enable(&priv->radar, val, SIWIFI_RADAR_FCU);

    return count;
}

DEBUGFS_READ_WRITE_FILE_OPS(enable);

static ssize_t siwifi_dbgfs_band_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "BAND=%d\n", priv->sec_phy_chan.band);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_band_write(struct file *file,
                                     const char __user *user_buf,
                                     size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if ((sscanf(buf, "%d", &val) > 0) && (val >= 0) && (val <= NL80211_BAND_5GHZ))
        priv->sec_phy_chan.band = val;

    return count;
}

DEBUGFS_READ_WRITE_FILE_OPS(band);

static ssize_t siwifi_dbgfs_type_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "TYPE=%d\n", priv->sec_phy_chan.type);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_type_write(struct file *file,
                                     const char __user *user_buf,
                                     size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if ((sscanf(buf, "%d", &val) > 0) && (val >= PHY_CHNL_BW_20) &&
        (val <= PHY_CHNL_BW_80P80))
        priv->sec_phy_chan.type = val;

    return count;
}

DEBUGFS_READ_WRITE_FILE_OPS(type);

static ssize_t siwifi_dbgfs_prim20_read(struct file *file,
                                      char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "PRIM20=%dMHz\n", priv->sec_phy_chan.prim20_freq);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_prim20_write(struct file *file,
                                       const char __user *user_buf,
                                       size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0)
        priv->sec_phy_chan.prim20_freq = val;

    return count;
}

DEBUGFS_READ_WRITE_FILE_OPS(prim20);

static ssize_t siwifi_dbgfs_center1_read(struct file *file,
                                       char __user *user_buf,
                                       size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "CENTER1=%dMHz\n", priv->sec_phy_chan.center_freq1);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_center1_write(struct file *file,
                                        const char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0)
        priv->sec_phy_chan.center_freq1 = val;

    return count;
}

DEBUGFS_READ_WRITE_FILE_OPS(center1);

static ssize_t siwifi_dbgfs_center2_read(struct file *file,
                                       char __user *user_buf,
                                       size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "CENTER2=%dMHz\n", priv->sec_phy_chan.center_freq2);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_center2_write(struct file *file,
                                        const char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0)
        priv->sec_phy_chan.center_freq2 = val;

    return count;
}

DEBUGFS_READ_WRITE_FILE_OPS(center2);


static ssize_t siwifi_dbgfs_set_read(struct file *file,
                                   char __user *user_buf,
                                   size_t count, loff_t *ppos)
{
    return 0;
}

static ssize_t siwifi_dbgfs_set_write(struct file *file,
                                    const char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);

    siwifi_send_set_channel(priv, 1, NULL);
    siwifi_radar_detection_enable(&priv->radar, SIWIFI_RADAR_DETECT_ENABLE,
                                SIWIFI_RADAR_FCU);

    return count;
}

DEBUGFS_READ_WRITE_FILE_OPS(set);

static ssize_t siwifi_dbgfs_radar_debugmode_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "radar_debugmode=%d\n", (int)priv->mod_params->radar_debugmode);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_radar_debugmode_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if ((sscanf(buf, "%d", &val) > 0) && (val >= 0) && (val <= 2)){
        printk("user want to set radar debugmode %d\n", val);
		siwifi_hw->mod_params->radar_debugmode = val;
	}

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(radar_debugmode);

static ssize_t siwifi_dbgfs_radar_debug_printk_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "radar_debug_printk=%d\n", (int)priv->mod_params->radar_debug_printk);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_radar_debug_printk_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if ((sscanf(buf, "%d", &val) > 0) && (val >= 0) && (val <= 2)){
        printk("user want to set radar debug printk %d\n", val);
		siwifi_hw->mod_params->radar_debug_printk = val;
	}

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(radar_debug_printk);

extern int freq_delta;
static ssize_t siwifi_dbgfs_radar_freq_delta_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "radar_freq_delta=%d\n", freq_delta);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_radar_freq_delta_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if ((sscanf(buf, "%d", &val) > 0)){
        printk("user want to set radar freq delta %d\n", val);
		freq_delta = val;
	}

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(radar_freq_delta);

static ssize_t siwifi_dbgfs_radar_event_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    ssize_t read = 0;

    siwifi_radar_detected(siwifi_hw);

    return read;
}

DEBUGFS_READ_FILE_OPS(radar_event);

#endif /* CONFIG_SIWIFI_RADAR */

static ssize_t siwifi_dbgfs_ave_txq_read(struct file *file,
                                              char __user *user_buf,
                                              size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = file->private_data;
    struct siwifi_vif *vif = NULL;
    struct siwifi_sta *sta = NULL;
    struct siwifi_txq *txq = NULL;
    int tid = 0, print_title = 0;

    char buf[32];
    int ret = 0;
    ssize_t read;
    u32 pkt_tid = 0, pkt_tid_max = 0;

    printk("cnt %d min %d max %d \n",
            priv->ave_speed_cnt_thres,
            priv->ave_speed_credits_low,
            priv->ave_speed_credits_up
            );
    printk("hwq enable [%d %d %d %d %d]\n",
            priv->hwq[0].ave_speed.ave_speed_enable,
            priv->hwq[1].ave_speed.ave_speed_enable,
            priv->hwq[2].ave_speed.ave_speed_enable,
            priv->hwq[3].ave_speed.ave_speed_enable,
            priv->hwq[4].ave_speed.ave_speed_enable
            );
    printk("hwq  total [%d %d %d %d %d]\n",
            priv->hwq[0].ave_speed.as_ave_pkt_total,
            priv->hwq[1].ave_speed.as_ave_pkt_total,
            priv->hwq[2].ave_speed.as_ave_pkt_total,
            priv->hwq[3].ave_speed.as_ave_pkt_total,
            priv->hwq[4].ave_speed.as_ave_pkt_total
            );
    list_for_each_entry (vif, &priv->vifs, list) {
        if (SIWIFI_VIF_TYPE(vif) != NL80211_IFTYPE_AP)
            continue;
        printk("vif[%d] %pM\n", vif->vif_index, vif->ndev->dev_addr);
        list_for_each_entry (sta, &vif->ap.sta_list, list) {
            printk("sta[%2d] %pM\n", sta->sta_idx, sta->mac_addr);
            print_title = 0;
            pkt_tid_max = 0;
            foreach_sta_txq(sta, txq, tid, priv) {
                pkt_tid = txq->as_enable_pkt + txq->as_disable_pkt;
                if (pkt_tid > pkt_tid_max)
                    pkt_tid_max = pkt_tid;
                if (!pkt_tid)
                    continue;
                if (pkt_tid * 100 < pkt_tid_max)
                    continue;

                if (!print_title) {
                    printk("  txq   enable  disable hwq[ ] apkt rdy acc\n");
                    //     "  --- -------- --------   -[-] ---- --- ---"
                    print_title = 1;
                }

                printk("  %3d %8d %8d   %s[%d] %4d %3d %3d\n",
                        txq->idx,
                        txq->as_enable_pkt,
                        txq->as_disable_pkt,
                        (txq->status & SIWIFI_TXQ_IN_HWQ_LIST) ? "Y" : " ",
                        txq->hwq->id,
                        txq->as_ave_pkt,
                        skb_queue_len(&txq->sk_list),
                        skb_queue_len(&txq->accel_sk_list)
                        );
            }
            printk("\n");
        }
        printk("\n");
    }


    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_ave_txq_write(struct file *file,
                                              const char __user *user_buf,
                                              size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = file->private_data;
    char buf[32];
    int val = 0, val1 = 0, val2 = 0, val3 = 0;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    struct siwifi_vif *vif = NULL;
    struct siwifi_sta *sta = NULL;
    struct siwifi_txq *txq = NULL;
    int tid = 0;

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0) {
        if (val == 0) {
            printk("clear enable & disable pkt\n");
            list_for_each_entry (vif, &priv->vifs, list) {
                if (SIWIFI_VIF_TYPE(vif) != NL80211_IFTYPE_AP)
                    continue;
                list_for_each_entry (sta, &vif->ap.sta_list, list) {
                    foreach_sta_txq(sta, txq, tid, priv) {
                        txq->as_enable_pkt = 0;
                        txq->as_disable_pkt = 0;
                    }
                }
            }
        } else if (val == 1) {
            if (sscanf(buf, "%d %d %d %d", &val, &val1, &val2, &val3) > 0) {
                printk("set cnt %d min %d max %d \n", val1, val2, val3);
                priv->ave_speed_cnt_thres   = val1;
                priv->ave_speed_credits_low = val2;
                priv->ave_speed_credits_up  = val3;
            }
        }
    }

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(ave_txq);

static ssize_t siwifi_dbgfs_tx_memory_limit_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct siwifi_vif *vif;
    char buf[512];
    int ret;
    ssize_t read;
    int i;

    ret = 0;
    list_for_each_entry(vif, &priv->vifs, list) {
        for (i = 0; i < priv->tx_queue_num; i++) {
            ret += scnprintf(&buf[ret], min_t(size_t, 512 - ret - 1, count - ret),
                        "vif[%s] tx_memory_limit[%d]=%d \n", vif->ndev->name, i, vif->lm_ctl[i].tx_memory_limit);
        }
    }

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    return read;
}

static ssize_t siwifi_dbgfs_tx_memory_limit_write(struct file *file,
                                    const char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);

    char buf[32];
    int queue, val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d %d", &queue, &val) > 0) {
        struct siwifi_vif *vif;
        printk("User want set memory limit %d to queue %d\n", val, queue);
        if (queue >= priv->tx_queue_num) {
            printk("invalid queue number : %d\n", queue);
            return -EFAULT;
        }
        list_for_each_entry(vif, &priv->vifs, list) {
            vif->lm_ctl[queue].tx_memory_limit = val;
        }
    }

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(tx_memory_limit);


static ssize_t siwifi_dbgfs_tx_cnt_limit_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct siwifi_vif *vif;
    char buf[512];
    int ret;
    ssize_t read;
    int i;

    ret = 0;
    list_for_each_entry(vif, &priv->vifs, list) {
        for (i = 0; i < priv->tx_queue_num; i++) {
            ret += scnprintf(&buf[ret], min_t(size_t, 512 - ret - 1, count - ret),
                        "vif[%s] tx_cnt_limit[%d]=%d \n", vif->ndev->name, i, vif->lm_ctl[i].tx_cnt_limit);
        }
    }

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    return read;
}

static ssize_t siwifi_dbgfs_tx_cnt_limit_write(struct file *file,
                                    const char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);

    char buf[32];
    int queue, val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d %d", &queue, &val) > 0) {
        struct siwifi_vif *vif;
        printk("User want set memory cnt limit %d to queue %d\n", val, queue);
        if (queue >= priv->tx_queue_num) {
            printk("invalid queue number : %d\n", queue);
            return -EFAULT;
        }
        list_for_each_entry(vif, &priv->vifs, list) {
            vif->lm_ctl[queue].tx_cnt_limit = val;
        }
    }

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(tx_cnt_limit);


static ssize_t siwifi_dbgfs_lm_enable_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[128];
    ssize_t read;
    int ret = 0;

    ret += scnprintf(&buf[ret], min_t(size_t, 128 - ret - 1, count - ret),
                    "lm enable=%d \n", priv->lm_ctrl_enable);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    return read;
}

static ssize_t siwifi_dbgfs_lm_enable_write(struct file *file,
                                    const char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);

    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0) {
        printk("User set lm_enable %d\n", val);
        priv->lm_ctrl_enable = !!val;
    }

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(lm_enable);

static ssize_t siwifi_dbgfs_lm_stats_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct siwifi_vif *vif;
    ssize_t read;
    char *buf;
    int i;
    int ret = 0;
    int bufsz = 1024;
    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (!buf)
        return -EFAULT;
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
            "vif       txq_mem    amsdu_mem    limit_mem      txq_cnt    amsdu_cnt    limit_cnt\n");

    list_for_each_entry(vif, &priv->vifs, list) {
        for (i = 0; i < priv->tx_queue_num; i++) {
            ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                             "%-8s %8d     %8d     %8d     %8d     %8d     %8d\n",
                             vif->ndev->name,
                             vif->lm_ctl[i].tx_memory_usage,
                             vif->lm_ctl[i].amsdu_tx_memory_usage,
                             vif->lm_ctl[i].tx_memory_limit,
                             vif->lm_ctl[i].tx_cnt,
                             vif->lm_ctl[i].amsdu_tx_cnt,
                             vif->lm_ctl[i].tx_cnt_limit);
        }
    }

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    siwifi_kfree(buf);
    return read;
}
DEBUGFS_READ_FILE_OPS(lm_stats);

#ifdef CONFIG_SIWIFI_TEMPERATURE_CONTROL
static ssize_t siwifi_dbgfs_temp_ctl_state_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[512];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "TEMPCTL_ENABLE=%d\n", priv->temp_ctl.temp_ctl_parameters[0]);
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                    "TEMPCTL_TRIGGER_TEMP_LV1=%d\n", priv->temp_ctl.temp_ctl_parameters[1]);
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                    "TEMPCTL_TRIGGER_TEMP_LV2=%d\n", priv->temp_ctl.temp_ctl_parameters[2]);
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                    "TEMPCTL_TRIGGER_TEMP_LV3=%d\n", priv->temp_ctl.temp_ctl_parameters[3]);
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                    "TEMPCTL_TRIGGER_TEMP_LV4=%d\n", priv->temp_ctl.temp_ctl_parameters[4]);
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                    "Current temperature control level=%d\n", priv->temp_ctl.temp_ctl_level);
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
            "TEMPCTL_TRIGGER_LIMIT_LV1_PERCENT=%d\n", priv->temp_ctl.temp_ctl_parameters[5]);
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
            "TEMPCTL_TRIGGER_LIMIT_LV2_PERCENT=%d\n", priv->temp_ctl.temp_ctl_parameters[6]);
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
            "TEMPCTL_TRIGGER_LIMIT_LV3_PERCENT=%d\n", priv->temp_ctl.temp_ctl_parameters[7]);
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
            "TEMPCTL_TRIGGER_LIMIT_LV4_PERCENT=%d\n", priv->temp_ctl.temp_ctl_parameters[8]);
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    return read;
}

static ssize_t siwifi_dbgfs_temp_ctl_state_write(struct file *file,
                                    const char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);

    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0 && val == 1)
        siwifi_parse_temperature_control_configfile(priv, SIWIFI_TEMPERATURE_CONTROL_NAME);

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(temp_ctl_state);
#endif

static ssize_t siwifi_dbgfs_mem_info_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    siwifi_print_mem_info();
    return 0;
}
DEBUGFS_READ_FILE_OPS(mem_info);

#ifdef CONFIG_WIFI_RX_THREAD
static ssize_t siwifi_dbgfs_rxthread_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    printk("rx thread               : %p\n", priv->rx_kthread);
    printk("rx_pending_count        : %d\n", atomic_read(&priv->rx_pending_count));
    printk("rx element cur cnt      : %d\n", priv->rx_thread_dbg.elt_cur_cnt);
    printk("rx element max cnt      : %d\n", priv->rx_thread_dbg.elt_max_cnt);
    printk("rx skb cur cnt          : %d\n", priv->rx_thread_dbg.skb_cur_cnt);
    printk("rx skb max cnt          : %d\n", priv->rx_thread_dbg.skb_max_cnt);
    printk("rx skb max per element  : %d\n", priv->rx_thread_dbg.skb_max_per_elt);
    printk("rx process max loop     : %d\n", priv->rx_thread_dbg.elt_loop_max_cnt);

    return 0;
}
DEBUGFS_READ_FILE_OPS(rxthread);
#endif

#ifdef CONFIG_PRIV_RX_BUFFER_POOL
extern uint8_t g_rx_skb_pool_enable;
static ssize_t siwifi_dbgfs_rxbuf_pool_enable_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    printk("g_rx_skb_pool_enable = %d\n", g_rx_skb_pool_enable);
    return 0;
}

static ssize_t siwifi_dbgfs_rxbuf_pool_enable_write(struct file *file,
                                    const char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0 && val == 1)
        g_rx_skb_pool_enable = 1;
    else
        g_rx_skb_pool_enable = 0;

    printk("user set g_rx_skb_pool_enable to %d\n", g_rx_skb_pool_enable);

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(rxbuf_pool_enable);
#endif

static ssize_t siwifi_dbgfs_dump_vif_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    siwifi_send_vif_dump(priv);
    return 0;
}
DEBUGFS_READ_FILE_OPS(dump_vif);

#ifdef CONFIG_SIWIFI_TRACE_SKB
static ssize_t siwifi_dbgfs_debug_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[512];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "echo $index > snoop_sta  ////index indicates the sta->idx in txq\n");
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                    "DEBUG ICMP:                echo 1 > debug\n");
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                    "DEBUG ALL PKTS:            echo 2 > debug\n");
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                    "DEBUG MGMT PKTS:           echo 4 > debug\n");
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                    "DEBUG RECORDS  :           echo $index > record\n");
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                    "DEBUG EAPOL    :           echo 16 > debug\n");
    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                    "current sta idx=%d flags=%x\n", priv->trace_ctx.debug_sta_idx, priv->trace_ctx.debug_flag);
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    return read;
}

static ssize_t siwifi_dbgfs_debug_write(struct file *file,
                                    const char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);

    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0)
        priv->trace_ctx.debug_flag = val;

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(debug);

static ssize_t siwifi_dbgfs_debug_result_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[64];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "debug_icmp_tx_num=%d, debug_icmp_tx_ok_num=%d reply=%d\n",
                    priv->trace_ctx.debug_icmp_tx_num, priv->trace_ctx.debug_icmp_tx_ok_num, priv->trace_ctx.debug_icmp_reply);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    return read;
}

static ssize_t siwifi_dbgfs_debug_result_write(struct file *file,
                                    const char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);

    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0) {
        priv->trace_ctx.debug_icmp_tx_ok_num = 0;
        priv->trace_ctx.debug_icmp_tx_num = 0;
    }

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(debug_result);

const char siwifi_record_event_string[8][6] = {
        "XMIT",
        "CFM ",
        "RX  ",
        "PUSH",
        "HWQ "
};

static ssize_t siwifi_dbgfs_record_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[4000];
    int ret, index, first_index;
    ssize_t read;
    unsigned long rem_nsec;
    struct siwifi_trace_sta_record *record;
    struct siwifi_trace_sta_record *pool;
    u64 ts;

    ts = local_clock();
    rem_nsec = do_div(ts, 1000000000);
    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "trace current sta idx=%d flags=%x now %5lu.%03lu\n", priv->trace_ctx.debug_sta_idx, priv->trace_ctx.debug_flag, (unsigned long)ts, rem_nsec / 1000000);

    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
            "   TIME    EVT  STA TID  LEN SKB STATUS\n");
    if(priv->trace_ctx.debug_sta_record_buf){
        pool = (struct siwifi_trace_sta_record *)(priv->trace_ctx.debug_sta_record_buf);
        first_index = priv->trace_ctx.debug_sta_record_pos;
        index = first_index;
        while(index < (first_index + SIWIFI_TRACE_RECORD_NUM)){
            record = (struct siwifi_trace_sta_record *)(&pool[index % SIWIFI_TRACE_RECORD_NUM]);
            if(record->skb_len != 0){
                ts = record->ts_nsec;
                rem_nsec = do_div(ts, 1000000000);
                ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
                        "-%5lu.%03lu %s %3d %2d %4d %04x %02x\n", (unsigned long)ts, rem_nsec / 1000000, siwifi_record_event_string[(int)record->event],
                        record->sta_idx, record->tid, record->skb_len, record->skb_addr & 0xFFFF, record->status);
            }
            index++;
        }
    }

    ret += scnprintf(&buf[ret], min_t(size_t, sizeof(buf) - 1 - ret, count),
            "\n--end\n");
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    return read;
}

static ssize_t siwifi_dbgfs_record_write(struct file *file,
                                    const char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);

    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0) {
        priv->trace_ctx.debug_sta_idx = val;
        siwifi_trace_record_reset(priv);
        priv->trace_ctx.debug_flag = SIWIFI_SKB_DEBUG_FLAG_RECORD;
    }

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(record);
#endif ///CONFIG_SIWIFI_TRACE_SKB

static ssize_t siwifi_dbgfs_max_rx_process_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "max rx process=%d \n", priv->ipc_env->max_rx_process_time);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    return read;
}

static ssize_t siwifi_dbgfs_max_rx_process_write(struct file *file,
                                    const char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);

    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0)
        priv->ipc_env->max_rx_process_time = val;

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(max_rx_process);

#ifdef NEW_SCHEDULE
static ssize_t siwifi_dbgfs_burstinfo_read(struct file *file,
                                   char __user *user_buf,
                                   size_t count, loff_t *ppos)
{
    return 0;
}

static ssize_t siwifi_dbgfs_burstinfo_write(struct file *file,
                                    const char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) <= 0)
        val = -1;

    siwifi_send_dbg_print_burst_info(priv, val);

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(burstinfo);
#endif


static ssize_t siwifi_dbgfs_force_trigger_type_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "force_trigger_type=%d\n", priv->debugfs.force_trigger_type);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_force_trigger_type_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0) {
        printk("set the force_trigger-- %d\n", val);
        priv->debugfs.force_trigger_type = val;
    }
    return count;
}

DEBUGFS_READ_WRITE_FILE_OPS(force_trigger_type);

static ssize_t siwifi_dbgfs_hwq_credit_dec_read(struct file *file,
                char __user *user_buf,
                size_t count,
                loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "hwq credit_dec=%d\n", priv->hwq_credits_dec[0]);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_hwq_credit_dec_write(struct file *file,
                const char __user *user_buf,
                size_t count,
                loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int val;
    int ret;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0) {
        spin_lock_bh(&priv->tx_lock);
        ret = siwifi_adjust_hwq_credits(priv, (s8)(val));
        spin_unlock_bh(&priv->tx_lock);
        printk("set the hwq_credit_dec-- %d ret=%d\n", val, ret);
    }
    return count;
}

DEBUGFS_READ_WRITE_FILE_OPS(hwq_credit_dec);

static ssize_t siwifi_dbgfs_la_clk_read(struct file *file,
                                       char __user *user_buf,
                                       size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[16];
    int ret;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "%d\n", siwifi_platform_get_clkfreq(priv, LA_CLK));

    return simple_read_from_buffer(user_buf, count, ppos, buf, ret);
}
DEBUGFS_READ_FILE_OPS(la_clk);

#define MCS_INDEX_TX_RCX_OFT    0
#define MCS_INDEX_TX_RCX_MASK   (0X7F << MCS_INDEX_TX_RCX_OFT)
#define BW_TX_RCX_OFT           7
#define BW_TX_RCX_MASK          (0X3 << BW_TX_RCX_OFT)
#define SHORT_GI_TX_RCX_OFT     9
#define SHORT_GI_TX_RCX_MASK    (0x1 << SHORT_GI_TX_RCX_OFT)
#define PRE_TYPE_TX_RCX_OFT     10
#define PRE_TYPE_TX_RCX_MASK    (0x1 << PRE_TYPE_TX_RCX_OFT)
#define FORMAT_MOD_TX_RCX_OFT   11
#define FORMAT_MOD_TX_RCX_MASK  (0X7 << FORMAT_MOD_TX_RCX_OFT)
#define VHT_NSS_OFT             4
#define VHT_NSS_MASK            (0x7 << VHT_NSS_OFT)
#define VHT_MCS_OFT             0
#define VHT_MCS_MASK            (0xF << VHT_MCS_OFT)
#define HT_NSS_OFT              3
#define HT_NSS_MASK             (0x3 << HT_NSS_OFT)
#define HT_MCS_OFT              0
#define HT_MCS_MASK             (0x7 << HT_MCS_OFT)
#define FORMATMOD_NON_HT        0
#define FORMATMOD_NON_HT_DUP_OFDM 1
#define FORMATMOD_HT_MF         2
#define FORMATMOD_HT_GF         3
#define FORMATMOD_VHT           4
//redinfed conflict with hal_desc.h
//#define N_CCK                   8
//#define N_OFDM                  8
//#define N_HT                    (8*2*2*4)
//#define N_VHT                   (10*4*2*8)
#define LINE_MAX_SZ 150

struct st {
    char line[LINE_MAX_SZ + 1];
    unsigned int r_idx;
};

static int compare_idx(const void *st1, const void *st2)
{
    int index1 = ((struct st *)st1)->r_idx;
    int index2 = ((struct st *)st2)->r_idx;

    if (index1 > index2) return 1;
    if (index1 < index2) return -1;

    return 0;
}

static int print_rate(char *buf, int size, int format, int nss, int mcs, int bw,
                      int sgi, int pre, int *r_idx)
{
    int res = 0;
    int bitrates_cck[4] = { 10, 20, 55, 110 };
    int bitrates_ofdm[8] = { 6, 9, 12, 18, 24, 36, 48, 54};
    char he_gi[3][4] = {"0.8", "1.6", "3.2"};

    if (format < FORMATMOD_HT_MF) {
        if (mcs < 4) {
            if (r_idx) {
                *r_idx = (mcs * 2) + pre;
                res = scnprintf(buf, size - res, "%3d ", *r_idx);
            }
            res += scnprintf(&buf[res], size - res, "L-CCK/%cP   %2u.%1uM    ",
                             pre > 0 ? 'L' : 'S',
                             bitrates_cck[mcs] / 10,
                             bitrates_cck[mcs] % 10);
        } else {
            mcs -= 4;
            if (r_idx) {
                *r_idx = N_CCK + mcs;
                res = scnprintf(buf, size - res, "%3d ", *r_idx);
            }
            res += scnprintf(&buf[res], size - res, "L-OFDM     %2u.0M    ",
                             bitrates_ofdm[mcs]);
        }
    } else if (format < FORMATMOD_VHT) {
        if (r_idx) {
            *r_idx = N_CCK + N_OFDM + nss * 32 + mcs * 4 + bw * 2 + sgi;
            res = scnprintf(buf, size - res, "%3d ", *r_idx);
        }
        mcs += nss * 8;
        res += scnprintf(&buf[res], size - res, "HT%d/%cGI    MCS%-2d   ",
                         20 * (1 << bw), sgi ? 'S' : 'L', mcs);
    } else if (format == FORMATMOD_VHT){
        if (r_idx) {
            *r_idx = N_CCK + N_OFDM + N_HT + nss * 80 + mcs * 8 + bw * 2 + sgi;
            res = scnprintf(buf, size - res, "%3d ", *r_idx);
        }
        res += scnprintf(&buf[res], size - res, "VHT%d/%cGI%*cMCS%d/%1d  ",
                         20 * (1 << bw), sgi ? 'S' : 'L', bw > 2 ? 2 : 3, ' ',
                         mcs, nss + 1);
    } else {
        if (r_idx) {
            *r_idx = N_CCK + N_OFDM + N_HT + N_VHT + nss * 144 + mcs * 12 + bw * 3 + sgi;
            res = scnprintf(buf, size - res, "%3d ", *r_idx);
        }
        res += scnprintf(&buf[res], size - res, "HE%d/GI%s%*cMCS%d/%1d%*c",
                         20 * (1 << bw), he_gi[sgi], bw > 2 ? 1 : 2, ' ',
                         mcs, nss + 1, mcs > 9 ? 1 : 2, ' ');

    }

    return res;
}

static int print_rate_from_cfg(char *buf, int size, u32 rate_config, int *r_idx)
{
    union siwifi_rate_ctrl_info *r_cfg = (union siwifi_rate_ctrl_info *)&rate_config;
    union siwifi_mcs_index *mcs_index = (union siwifi_mcs_index *)&rate_config;
    unsigned int ft, pre, gi, bw, nss, mcs, len;

    ft = r_cfg->formatModTx;
    pre = r_cfg->giAndPreTypeTx >> 1;
    if (ft == FORMATMOD_HE_SU) {
        mcs = mcs_index->he.mcs;
        nss = mcs_index->he.nss;
    } else if (ft == FORMATMOD_VHT) {
        mcs = mcs_index->vht.mcs;
        nss = mcs_index->vht.nss;
    } else if (ft >= FORMATMOD_HT_MF) {
        mcs = mcs_index->ht.mcs;
        nss = mcs_index->ht.nss;
    } else {
        mcs = mcs_index->legacy;
        nss = 0;
    }
    gi = r_cfg->giAndPreTypeTx;
    bw = r_cfg->bwTx;

    len = print_rate(buf, size, ft, nss, mcs, bw, gi, pre, r_idx);
    return len;
}

static void idx_to_rate_cfg(int idx, union siwifi_rate_ctrl_info *r_cfg)
{
    r_cfg->value = 0;
    if (idx < N_CCK)
    {
        r_cfg->formatModTx = FORMATMOD_NON_HT;
        r_cfg->giAndPreTypeTx = (idx & 1) << 1;
        r_cfg->mcsIndexTx = idx / 2;
    }
    else if (idx < (N_CCK + N_OFDM))
    {
        r_cfg->formatModTx = FORMATMOD_NON_HT;
        r_cfg->mcsIndexTx =  idx - N_CCK + 4;
    }
    else if (idx < (N_CCK + N_OFDM + N_HT))
    {
        union siwifi_mcs_index *r = (union siwifi_mcs_index *)r_cfg;

        idx -= (N_CCK + N_OFDM);
        r_cfg->formatModTx = FORMATMOD_HT_MF;
        r->ht.nss = idx / (8*2*2);
        r->ht.mcs = (idx % (8*2*2)) / (2*2);
        r_cfg->bwTx = ((idx % (8*2*2)) % (2*2)) / 2;
        r_cfg->giAndPreTypeTx = idx & 1;
    }
    else if (idx < (N_CCK + N_OFDM + N_HT + N_VHT))
    {
        union siwifi_mcs_index *r = (union siwifi_mcs_index *)r_cfg;

        idx -= (N_CCK + N_OFDM + N_HT);
        r_cfg->formatModTx = FORMATMOD_VHT;
        r->vht.nss = idx / (10*4*2);
        r->vht.mcs = (idx % (10*4*2)) / (4*2);
        r_cfg->bwTx = ((idx % (10*4*2)) % (4*2)) / 2;
        r_cfg->giAndPreTypeTx = idx & 1;
    }
    else
    {
        union siwifi_mcs_index *r = (union siwifi_mcs_index *)r_cfg;

        idx -= (N_CCK + N_OFDM + N_HT + N_VHT);
        r_cfg->formatModTx = FORMATMOD_HE_SU;
        r->vht.nss = idx / (12*4*3);
        r->vht.mcs = (idx % (12*4*3)) / (4*3);
        r_cfg->bwTx = ((idx % (12*4*3)) % (4*3)) / 3;
        r_cfg->giAndPreTypeTx = idx % 3;
    }
}

static ssize_t siwifi_dbgfs_rc_stats_read(struct file *file,
                                        char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_sta *sta = NULL;
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char *buf;
    int bufsz, len = 0;
    ssize_t read;
    int i = 0;
    int error = 0;
    struct me_rc_stats_cfm me_rc_stats_cfm;
    unsigned int no_samples;
    struct st *st;
    u8 mac[6];

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* everything should fit in one call */
    if (*ppos)
        return 0;

    /* Get the station index from MAC address */
    sscanf(file->f_path.dentry->d_parent->d_iname, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
            &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    if (mac == NULL)
        return 0;
    sta = siwifi_get_sta(priv, mac);
    if (sta == NULL)
        return 0;

    /* Forward the information to the LMAC */
    if ((error = siwifi_send_me_rc_stats(priv, sta->sta_idx, &me_rc_stats_cfm)))
        return error;

    no_samples = me_rc_stats_cfm.no_samples;
    if (no_samples == 0)
        return 0;

    bufsz = no_samples * LINE_MAX_SZ + 500;

    buf = siwifi_kmalloc(bufsz + 1, GFP_ATOMIC);
    if (buf == NULL)
        return 0;

    st = siwifi_kmalloc(sizeof(struct st) * no_samples, GFP_ATOMIC);
    if (st == NULL)
    {
        siwifi_kfree(buf);
        return 0;
    }

    for (i = 0; i < no_samples; i++)
    {
        unsigned int tp, eprob;
        len = print_rate_from_cfg(st[i].line, LINE_MAX_SZ,
                                  me_rc_stats_cfm.rate_stats[i].rate_config,
                                  &st[i].r_idx);

        if (me_rc_stats_cfm.sw_retry_step != 0)
        {
            len += scnprintf(&st[i].line[len], LINE_MAX_SZ - len,  "%c",
                    me_rc_stats_cfm.retry_step_idx[me_rc_stats_cfm.sw_retry_step] == i ? '*' : ' ');
        }
        else
        {
            len += scnprintf(&st[i].line[len], LINE_MAX_SZ - len, " ");
        }
        len += scnprintf(&st[i].line[len], LINE_MAX_SZ - len, "%c",
                me_rc_stats_cfm.retry_step_idx[0] == i ? 'T' : ' ');
        len += scnprintf(&st[i].line[len], LINE_MAX_SZ - len, "%c",
                me_rc_stats_cfm.retry_step_idx[1] == i ? 't' : ' ');
        len += scnprintf(&st[i].line[len], LINE_MAX_SZ - len, "%c ",
                me_rc_stats_cfm.retry_step_idx[2] == i ? 'P' : ' ');

        tp = me_rc_stats_cfm.tp[i] / 10;
        len += scnprintf(&st[i].line[len], LINE_MAX_SZ - len, " %4u.%1u",
                         tp / 10, tp % 10);

        eprob = ((me_rc_stats_cfm.rate_stats[i].probability * 1000) >> 16) + 1;
        len += scnprintf(&st[i].line[len],LINE_MAX_SZ - len,
                         "  %4u.%1u %5u(%6u)  %6u %6u",
                         eprob / 10, eprob % 10,
                         me_rc_stats_cfm.rate_stats[i].success,
                         me_rc_stats_cfm.rate_stats[i].attempts,
                         me_rc_stats_cfm.rate_stats[i].sample_skipped,
                         me_rc_stats_cfm.rate_stats[i].n_retry & 0x1F);
    }
    len = scnprintf(buf, bufsz ,
                     "\nTX rate info for %02X:%02X:%02X:%02X:%02X:%02X:\n",
                     mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    len += scnprintf(&buf[len], bufsz - len,
            " #  type        rate             tpt   eprob    ok(   tot)   skipped  nRetry\n");

    // add sorted statistics to the buffer
    sort(st, no_samples, sizeof(st[0]), compare_idx, NULL);
    for (i = 0; i < no_samples; i++)
    {
        len += scnprintf(&buf[len], bufsz - len, "%s\n", st[i].line);
    }
    len += scnprintf(&buf[len], bufsz - len, "\n MPDUs AMPDUs AvLen trialP");
    len += scnprintf(&buf[len], bufsz - len, "\n%6u %6u %3d.%1d %6u\n",
                     me_rc_stats_cfm.ampdu_len,
                     me_rc_stats_cfm.ampdu_packets,
                     me_rc_stats_cfm.avg_ampdu_len >> 16,
                     ((me_rc_stats_cfm.avg_ampdu_len * 10) >> 16) % 10,
                     me_rc_stats_cfm.sample_wait);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, len);

    siwifi_kfree(buf);
    siwifi_kfree(st);

    return read;
}

DEBUGFS_READ_FILE_OPS(rc_stats);

static ssize_t siwifi_dbgfs_rc_fixed_rate_idx_write(struct file *file,
                                                  const char __user *user_buf,
                                                  size_t count, loff_t *ppos)
{
    struct siwifi_sta *sta = NULL;
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    u8 mac[6];
    char buf[10];
    int fixed_rate_idx = -1;
    union siwifi_rate_ctrl_info rate_config;
    int error = 0;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* Get the station index from MAC address */
    sscanf(file->f_path.dentry->d_parent->d_iname, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
            &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    if (mac == NULL)
        return 0;
    sta = siwifi_get_sta(priv, mac);
    if (sta == NULL)
        return 0;

    /* Get the content of the file */
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;
    buf[len] = '\0';
    sscanf(buf, "%i\n", &fixed_rate_idx);

    /* Convert rate index into rate configuration */
    if ((fixed_rate_idx < 0) || (fixed_rate_idx >= (N_CCK + N_OFDM + N_HT + N_VHT + N_HE)))
    {
        // disable fixed rate
        rate_config.value = (u32)-1;
    }
    else
    {
        idx_to_rate_cfg(fixed_rate_idx, &rate_config);
    }

    // Forward the request to the LMAC
    if ((error = siwifi_send_me_rc_set_rate(priv, sta->sta_idx,
                                          (u16)rate_config.value)) != 0)
    {
        return error;
    }

    priv->debugfs.rc_config[sta->sta_idx] = (int)rate_config.value;
    return len;
}

DEBUGFS_WRITE_FILE_OPS(rc_fixed_rate_idx);

static ssize_t siwifi_dbgfs_rc_set_no_ss_write(struct file *file,
                                        const char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    u8 val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%hhu", &val) > 0) {
        printk("siwifi_dbgfs_rc_set_no_ss_write %hhu", val);
        if(val == 0 || val == 1)
            siwifi_send_me_rc_set_no_ss(priv, val);
        else
            return 0;
	}

    return count;
}

DEBUGFS_WRITE_FILE_OPS(rc_set_no_ss);

static ssize_t siwifi_dbgfs_disable_wmm_edca_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[32];
    int ret = 0;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                     "disable_wmm_edca=%d\n", priv->disable_wmm_edca);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_disable_wmm_edca_write(struct file *file,
                                        const char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    int value;
    char buf[32];
    char msg[64];
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';
    if ((sscanf(buf, "%d", &value) > 0) && (value >= 0) && (value <= 1)){
        if (value != priv->disable_wmm_edca) {
            scnprintf(msg, sizeof(msg), "Force trigger\n");
            siwifi_send_dbg_trigger_req(priv, msg, 8);
        }
        spin_lock_bh(&priv->tx_lock);
        priv->disable_wmm_edca = value;
        spin_unlock_bh(&priv->tx_lock);
    }

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(disable_wmm_edca);

static ssize_t siwifi_dbgfs_last_rx_read(struct file *file,
                                       char __user *user_buf,
                                       size_t count, loff_t *ppos)
{
    struct siwifi_sta *sta = NULL;
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct siwifi_rx_rate_stats *rate_stats;
    char *buf;
    int bufsz, i, len = 0;
    ssize_t read;
    unsigned int fmt, pre, bw, nss, mcs, gi;
    u8 mac[6];
    struct rx_vector_1 *last_rx;
    char hist[] = "##################################################";
    int hist_len = sizeof(hist) - 1;
    u8 nrx;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* everything should fit in one call */
    if (*ppos)
        return 0;

    /* Get the station index from MAC address */
    sscanf(file->f_path.dentry->d_parent->d_iname, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
            &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    if (mac == NULL)
        return 0;
    sta = siwifi_get_sta(priv, mac);
    if (sta == NULL)
        return 0;

    rate_stats = &sta->stats.rx_rate;
    bufsz = (rate_stats->size * ( 50 * hist_len) + 200);
    buf = siwifi_kmalloc(bufsz + 1, GFP_ATOMIC);
    if (buf == NULL)
        return 0;

    // Get number of RX paths
    nrx = (priv->version_cfm.version_phy_1 & MDM_NRX_MASK) >> MDM_NRX_LSB;

    len += scnprintf(buf, bufsz,
                     "\nRX rate info for %02X:%02X:%02X:%02X:%02X:%02X:\n",
                     mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    // Display Statistics
    for (i = 0 ; i < rate_stats->size ; i++ )
    {
        if (rate_stats->table[i]) {

#define MY_DO_DIV(a,b)  ({\
    u64 tmp = a;    \
    do_div(tmp, b); \
    tmp; })
            union siwifi_rate_ctrl_info rate_config;
            uint32_t percent = (uint32_t)MY_DO_DIV((rate_stats->table[i] * 1000), rate_stats->cpt);
            uint32_t p;

            idx_to_rate_cfg(i, &rate_config);
            len += print_rate_from_cfg(&buf[len], bufsz - len,
                                       rate_config.value, NULL);
            p = (percent * hist_len) / 1000;
            len += scnprintf(&buf[len], bufsz - len, ": %8lld(%3d.%1d%%)%.*s\n",
                             rate_stats->table[i],
                             percent / 10, percent % 10, p, hist);
#undef MY_DO_DIV

        }
    }

    // Display detailed info of the last received rate
    last_rx = &sta->stats.last_rx.rx_vect1;

    len += scnprintf(&buf[len], bufsz - len,"\nLast received rate\n"
                     "  type      rate    LDPC STBC BEAMFM %s\n",
                     (nrx > 1) ? "rssi1(dBm) rssi2(dBm)" : "rssi(dBm)");

    fmt = last_rx->format_mod;
    bw = last_rx->ch_bw;
    pre = last_rx->pre_type;
#ifdef CONFIG_SFA28_FULLMASK
    if (fmt == FORMATMOD_VHT) {
        mcs = last_rx->mcs & 0x0F;
        nss = last_rx->stbc ? last_rx->n_sts / 2 : last_rx->n_sts;
        gi = last_rx->short_gi;
    } else if (fmt >= FORMATMOD_HT_MF) {
        mcs = last_rx->mcs % 8;
        nss = last_rx->mcs / 8;
        gi = last_rx->short_gi;
    } else {
        BUG_ON((mcs = legrates_lut[last_rx->leg_rate]) == -1);
        nss = 0;
        gi = 0;
    }
    len += print_rate(&buf[len], bufsz - len, fmt, nss, mcs, bw, gi, pre, NULL);
    if (fmt == FORMATMOD_VHT) {
        len += scnprintf(&buf[len], bufsz - len, "  %c    %c     %c",
                last_rx->fec_coding ? 'L' : ' ',
                last_rx->stbc ? 'S' : ' ',
                !last_rx->smoothing ? 'B' : ' ');
    } else if (fmt >= FORMATMOD_HT_MF) {
        len += scnprintf(&buf[len], bufsz - len, "  %c    %c      ",
                         last_rx->fec_coding ? 'L' : ' ',
                         last_rx->stbc ? 'S' : ' ');
    } else {
        len += scnprintf(&buf[len], bufsz - len, "              ");
    }

#else
    if (fmt == FORMATMOD_HE_SU) {
        mcs = last_rx->he.mcs;
        nss = last_rx->he.nss;
        gi = last_rx->he.gi_type;
    } else if (fmt == FORMATMOD_VHT) {
        mcs = last_rx->vht.mcs;
        nss = last_rx->vht.nss;
        gi = last_rx->vht.short_gi;
    } else if (fmt >= FORMATMOD_HT_MF) {
        mcs = last_rx->ht.mcs % 8;
        nss = last_rx->ht.mcs / 8;;
        gi = last_rx->ht.short_gi;
    } else {
        BUG_ON((mcs = legrates_lut[last_rx->leg_rate]) == -1);
        nss = 0;
        gi = 0;
    }

    len += print_rate(&buf[len], bufsz - len, fmt, nss, mcs, bw, gi, pre, NULL);

    /* flags for HT/VHT/HE */
    if (fmt == FORMATMOD_HE_SU) {
        len += scnprintf(&buf[len], bufsz - len, "  %c    %c     %c",
                         last_rx->he.fec ? 'L' : ' ',
                         last_rx->he.stbc ? 'S' : ' ',
                         last_rx->he.beamformed ? 'B' : ' ');
    } else if (fmt == FORMATMOD_VHT) {
        len += scnprintf(&buf[len], bufsz - len, "  %c    %c     %c",
                         last_rx->vht.fec ? 'L' : ' ',
                         last_rx->vht.stbc ? 'S' : ' ',
                         last_rx->vht.beamformed ? 'B' : ' ');
    } else if (fmt >= FORMATMOD_HT_MF) {
        len += scnprintf(&buf[len], bufsz - len, "  %c    %c      ",
                         last_rx->ht.fec ? 'L' : ' ',
                         last_rx->ht.stbc ? 'S' : ' ');
    } else {
        len += scnprintf(&buf[len], bufsz - len, "              ");
    }
#endif
    if (nrx > 1) {
        len += scnprintf(&buf[len], bufsz - len, "      %-4d       %d\n",
                         last_rx->rssi1, last_rx->rssi1);
    } else {
        len += scnprintf(&buf[len], bufsz - len, "     %d\n", last_rx->rssi1);
    }
    len += scnprintf(&buf[len], bufsz - len,"\nrssi :%d\n",sta->stats.last_rx.rx_vect1.rssi1);
    len += scnprintf(&buf[len], bufsz - len,"tx_packets :%u\n",sta->stats.tx_packets);
    len += scnprintf(&buf[len], bufsz - len,"rx_packets :%u\n",sta->stats.rx_packets);
    len += scnprintf(&buf[len], bufsz - len,"NSS :%u\n",sta->stats.last_rx.rx_vect1.n_sts);
    len += scnprintf(&buf[len], bufsz - len,"MCS :%u\n",sta->stats.last_rx.rx_vect1.mcs);
    if (sta->stats.last_rx.rx_vect1.short_gi)
    {
        len += scnprintf(&buf[len], bufsz - len,"GI : 400ns\n");
    }
    else
    {
        len += scnprintf(&buf[len], bufsz - len,"GI : 800ns\n");
    }
    /* to judge the working mode */
    if (sta && sta->rec_info.params)
    {
        if (sta->vht)
        {
            len += scnprintf(&buf[len], bufsz - len,"the working mode :5g_ac\n");
        }
        else if (sta->rec_info.params->supported_rates_len == 8)
        {
            if (sta->ht)
            {
                len += scnprintf(&buf[len], bufsz - len,"the working mode :5g_n\n");
            }
            else
            {
                len += scnprintf(&buf[len], bufsz - len,"the working mode :5g_a\n");
            }
        }
        else if (sta->ht)
        {
            len += scnprintf(&buf[len], bufsz - len,"the working mode :2.4g_n\n");
        }
        else if (sta->rec_info.params->supported_rates_len == 4)
        {

            len += scnprintf(&buf[len], bufsz - len,"the working mode :2.4g_b\n");
        }
        else
        {
            len += scnprintf(&buf[len], bufsz - len,"the working mode :2.4g_g\n");
        }
    }
    read = simple_read_from_buffer(user_buf, count, ppos, buf, len);

    siwifi_kfree(buf);
    return read;
}

static ssize_t siwifi_dbgfs_last_rx_write(struct file *file,
                                        const char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_sta *sta = NULL;
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    u8 mac[6];

    /* Get the station index from MAC address */
    sscanf(file->f_path.dentry->d_parent->d_iname, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
           &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    if (mac == NULL)
        return 0;
    sta = siwifi_get_sta(priv, mac);
    if (sta == NULL)
        return 0;

    /* Prevent from interrupt preemption as these statistics are updated under
     * interrupt */
    spin_lock_bh(&priv->tx_lock);
    memset(sta->stats.rx_rate.table, 0,
           sta->stats.rx_rate.size * sizeof(sta->stats.rx_rate.table[0]));
    sta->stats.rx_rate.cpt = 0;
    spin_unlock_bh(&priv->tx_lock);

    return count;
}

DEBUGFS_READ_WRITE_FILE_OPS(last_rx);

#ifdef CONFIG_SIWIFI_FW_TRACE
/*
 * trace helper
 */
void siwifi_fw_trace_dump(struct siwifi_hw *siwifi_hw)
{
    /* may be called before siwifi_dbgfs_register */
    if (siwifi_hw->plat->enabled && !siwifi_hw->debugfs.fw_trace.buf.data) {
        siwifi_fw_trace_buf_init(&siwifi_hw->debugfs.fw_trace.buf,
                               siwifi_ipc_fw_trace_desc_get(siwifi_hw));
    }

    if (!siwifi_hw->debugfs.fw_trace.buf.data)
        return;

    _siwifi_fw_trace_dump(&siwifi_hw->debugfs.fw_trace.buf);
}

void siwifi_fw_trace_reset(struct siwifi_hw *siwifi_hw)
{
    _siwifi_fw_trace_reset(&siwifi_hw->debugfs.fw_trace, true);
}
#endif

void siwifi_dbgfs_trigger_fw_dump(struct siwifi_hw *siwifi_hw, char *reason)
{
    if(!siwifi_send_dbg_trigger_req(siwifi_hw, reason, siwifi_hw->debugfs.force_trigger_type)
            && siwifi_hw->debugfs.recovery && siwifi_hw->debugfs.force_trigger_type == 1){
        //if msg send success and recovery and force_trigger_type is set, ignore messages until restart finish
        siwifi_hw->debugfs.recoverying = true;
    }
}

static void siwifi_rc_stat_work(struct work_struct *ws)
{
    struct siwifi_debugfs *siwifi_debugfs = container_of(ws, struct siwifi_debugfs,
                                                     rc_stat_work);
    struct siwifi_hw *siwifi_hw = container_of(siwifi_debugfs, struct siwifi_hw,
                                           debugfs);
    struct siwifi_sta *sta;
    uint8_t ridx, sta_idx, sta_action;

    ridx = siwifi_debugfs->rc_read;
    sta_idx = siwifi_debugfs->rc_sta[ridx];
    sta_action = siwifi_debugfs->rc_sta_action[ridx];
    if (sta_idx >= (NX_REMOTE_STA_MAX)) {
        WARN(1, "Invalid sta index %d", sta_idx);
        return;
    }

    siwifi_debugfs->rc_sta[ridx] = 0xFF;
    siwifi_debugfs->rc_sta_action[ridx] = RC_IDLE;
    ridx = (ridx + 1) % ARRAY_SIZE(siwifi_debugfs->rc_sta);
    siwifi_debugfs->rc_read = ridx;
    sta = &siwifi_hw->sta_table[sta_idx];
    //printk("siwifi_rc_stat_work, sta_idx : %d, mac :%pM\n", sta_idx, sta->mac_addr);

    if (sta_action == RC_REGISTER && siwifi_debugfs->dir_sta[sta_idx] == NULL) {
        /* register the sta */
        struct STRUCT_ENTRY_PROC_DEBUG *dir_rc = siwifi_debugfs->dir_rc;
        struct STRUCT_ENTRY_PROC_DEBUG *dir_sta;
        struct STRUCT_ENTRY_PROC_DEBUG *file;
        char sta_name[18];
        struct siwifi_rx_rate_stats *rate_stats = &sta->stats.rx_rate;
        int nb_rx_rate = N_CCK + N_OFDM;
        struct siwifi_rc_config_save *rc_cfg, *next;

        if (sta->sta_idx >= NX_REMOTE_STA_MAX) {
            scnprintf(sta_name, sizeof(sta_name), "bc_mc");
        } else {
            scnprintf(sta_name, sizeof(sta_name), "%pM", sta->mac_addr);
        }

        if (!(dir_sta = DEBUGFS_CREATE_DIR(sta_name, dir_rc))) {
            printk("can not create sta_name : %s\n", sta_name);
            goto error;
        }

        siwifi_debugfs->dir_sta[sta->sta_idx] = dir_sta;

#ifdef CONFIG_SIWIFI_DEBUGFS
        file = debugfs_create_file("stats", S_IRUSR, dir_sta, siwifi_hw,
                                   &siwifi_dbgfs_rc_stats_ops);
#elif defined CONFIG_SIWIFI_PROCFS
        file = proc_create_data("stats", S_IRUSR, dir_sta,
                &siwifi_dbgfs_rc_stats_ops, siwifi_hw);
#endif
        if (IS_ERR_OR_NULL(file)) {
            printk("can not create 'stats'\n");
            goto error_after_dir;
        }

#ifdef CONFIG_SIWIFI_DEBUGFS
        file = debugfs_create_file("fixed_rate_idx", S_IWUSR , dir_sta, siwifi_hw,
                                   &siwifi_dbgfs_rc_fixed_rate_idx_ops);
#elif defined CONFIG_SIWIFI_PROCFS
        file = proc_create_data("fixed_rate_idx", S_IRUSR, dir_sta,
                &siwifi_dbgfs_rc_fixed_rate_idx_ops, siwifi_hw);
#endif
        if (IS_ERR_OR_NULL(file)) {
            printk("can not create 'fixed_rate_idx'\n");
            goto error_after_dir;
        }

#ifdef CONFIG_SIWIFI_DEBUGFS
        file = debugfs_create_file("rx_rate", S_IRUSR | S_IWUSR, dir_sta, siwifi_hw,
                                   &siwifi_dbgfs_last_rx_ops);
#elif defined CONFIG_SIWIFI_PROCFS
        file = proc_create_data("rx_rate", S_IRUSR, dir_sta,
                &siwifi_dbgfs_last_rx_ops, siwifi_hw);
#endif
        if (IS_ERR_OR_NULL(file)) {
            printk("can not create 'rx_rate'\n");
            goto error_after_dir;
        }

        if (siwifi_hw->mod_params->ht_on)
            nb_rx_rate += N_HT;

        if (siwifi_hw->mod_params->vht_on)
            nb_rx_rate += N_VHT;

        if (siwifi_hw->mod_params->he_on)
            nb_rx_rate += N_HE;

        rate_stats->table = siwifi_kzalloc(nb_rx_rate * sizeof(rate_stats->table[0]),
                                    GFP_KERNEL);
        if (!rate_stats->table)
            goto error_after_dir;

        rate_stats->size = nb_rx_rate;
        rate_stats->cpt = 0;

        /* By default enable rate contoller */
        siwifi_debugfs->rc_config[sta_idx] = -1;

        /* Unless we already fix the rate for this station */
        list_for_each_entry_safe(rc_cfg, next, &siwifi_debugfs->rc_config_save, list) {
            if (jiffies_to_msecs(jiffies - rc_cfg->timestamp) > RC_CONFIG_DUR) {
                list_del(&rc_cfg->list);
                siwifi_kfree(rc_cfg);
            } else if (!memcmp(rc_cfg->mac_addr, sta->mac_addr, ETH_ALEN)) {
                siwifi_debugfs->rc_config[sta_idx] = rc_cfg->rate;
                list_del(&rc_cfg->list);
                siwifi_kfree(rc_cfg);
                break;
            }
        }

        if ((siwifi_debugfs->rc_config[sta_idx] >= 0) &&
            siwifi_send_me_rc_set_rate(siwifi_hw, sta_idx,
                                     (u16)siwifi_debugfs->rc_config[sta_idx]))
            siwifi_debugfs->rc_config[sta_idx] = -1;

    } else if (sta_action == RC_UNREGISTER && siwifi_debugfs->dir_sta[sta_idx] != NULL) {
        /* unregister the sta */
        if (sta->stats.rx_rate.table) {
            siwifi_kfree(sta->stats.rx_rate.table);
            sta->stats.rx_rate.table = NULL;
        }
        sta->stats.rx_rate.size = 0;
        sta->stats.rx_rate.cpt  = 0;

        /* If fix rate was set for this station, save the configuration in case
           we reconnect to this station within RC_CONFIG_DUR msec */
        if (siwifi_debugfs->rc_config[sta_idx] >= 0) {
            struct siwifi_rc_config_save *rc_cfg;
            rc_cfg = siwifi_kmalloc(sizeof(*rc_cfg), GFP_KERNEL);
            if (rc_cfg) {
                rc_cfg->rate = siwifi_debugfs->rc_config[sta_idx];
                rc_cfg->timestamp = jiffies;
                memcpy(rc_cfg->mac_addr, sta->mac_addr, ETH_ALEN);
                list_add_tail(&rc_cfg->list, &siwifi_debugfs->rc_config_save);
            }
        }

        DEBUGFS_REMOVE_DIR(siwifi_debugfs->dir_sta[sta_idx]);
        siwifi_debugfs->dir_sta[sta->sta_idx] = NULL;
    }

    return;

  error_after_dir:
    DEBUGFS_REMOVE_DIR(siwifi_debugfs->dir_sta[sta_idx]);
    siwifi_debugfs->dir_sta[sta->sta_idx] = NULL;
  error:
    dev_err(siwifi_hw->dev,
            "Error while (un)registering debug entry for sta %d\n", sta_idx);
}

void _siwifi_dbgfs_rc_stat_write(struct siwifi_debugfs *siwifi_debugfs, uint8_t sta_idx, int is_register)
{
    uint8_t widx = siwifi_debugfs->rc_write;
    if (siwifi_debugfs->rc_sta[widx] != 0XFF) {
        WARN(1, "Overlap in debugfs rc_sta table\n");
        return;
    }

    if (siwifi_debugfs->unregistering)
        return;

    siwifi_debugfs->rc_sta[widx] = sta_idx;
    if (is_register) {
        siwifi_debugfs->rc_sta_action[widx] = RC_REGISTER;
    } else {
        siwifi_debugfs->rc_sta_action[widx] = RC_UNREGISTER;
    }

    widx = (widx + 1) % ARRAY_SIZE(siwifi_debugfs->rc_sta);
    siwifi_debugfs->rc_write = widx;

    schedule_work(&siwifi_debugfs->rc_stat_work);
}

void siwifi_dbgfs_register_rc_stat(struct siwifi_hw *siwifi_hw, struct siwifi_sta *sta)
{
    if(siwifi_hw->enable_rc)
        _siwifi_dbgfs_rc_stat_write(&siwifi_hw->debugfs, sta->sta_idx, 1);
}

void siwifi_dbgfs_unregister_rc_stat(struct siwifi_hw *siwifi_hw, struct siwifi_sta *sta)
{
    if(siwifi_hw->enable_rc)
        _siwifi_dbgfs_rc_stat_write(&siwifi_hw->debugfs, sta->sta_idx, 0);
}

#ifdef CONFIG_BRIDGE_ACCELERATE
extern u64 accelerate_cnt;
extern u64 accel_pkt_in;
extern int accel_enable;

static ssize_t siwifi_dbgfs_accel_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    //struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char *buf;
    int ret = 0;
    ssize_t read;
    int bufsz = 4096;

    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (buf == NULL)
        return 0;

#if 1
    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
                     "accel enable=%d accel %lld pktin %lld\n", accel_enable, accelerate_cnt, accel_pkt_in);
#endif
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    siwifi_kfree(buf);

    return read;
}

static ssize_t siwifi_dbgfs_accel_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    //struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[64];
    int val;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d", &val) > 0) {
        printk("clear accel stat %d\n", val);
        accel_enable = (val == 0) ? 0 : 1;
        accelerate_cnt = 0;
        accel_pkt_in = 0;
    }
    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(accel);
#endif /* CONFIG_BRIDGE_ACCELERATE */

static ssize_t siwifi_dbgfs_mgmt_info_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    char *buf;
    int ret = 0;
    ssize_t read;
    int bufsz = 2048;
    struct dbg_get_mgmt_info_cfm cfm;
    struct dbg_mgmt_info *dbg_mgmt_info;
    struct siwifi_hw *siwifi_hw = (struct siwifi_hw *)file->private_data;

    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (!buf) {
        return 0;
    }

    if (siwifi_send_dbg_get_mgmt_info_req(siwifi_hw, &cfm, 0)) {
        printk("can not send get_mgmt_info_req\n");
        goto DONE;
    }

    dbg_mgmt_info = (struct dbg_mgmt_info *)siwifi_hw->dbg_mgmt_info_elem.addr;

    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
            "\n | packets   | TX           | RX\n"
            " | assocreq  | %-12d | %-12d\n"
            " | assocrsp  | %-12d | %-12d\n"
            " | probereq  | %-12d | %-12d\n"
            " | probersp  | %-12d | %-12d\n"
            " | beacon    | %-12lld | %-12lld\n"
            " | auth      | %-12d | %-12d\n"
            " | deauth    | %-12d | %-12d\n"
            " | disassoc  | %-12d | %-12d\n"
            " | action    | %-12d | %-12d\n\n",
            dbg_mgmt_info->assocreq_tx_packets, dbg_mgmt_info->assocreq_rx_packets,
            dbg_mgmt_info->assocrsp_tx_packets, dbg_mgmt_info->assocrsp_rx_packets,
            dbg_mgmt_info->probereq_tx_packets, dbg_mgmt_info->probereq_rx_packets,
            dbg_mgmt_info->probersp_tx_packets, dbg_mgmt_info->probersp_rx_packets,
            dbg_mgmt_info->beacon_tx_packets, dbg_mgmt_info->beacon_rx_packets,
            dbg_mgmt_info->authent_tx_packets, dbg_mgmt_info->authent_rx_packets,
            dbg_mgmt_info->deauthent_tx_packets, dbg_mgmt_info->deauthent_rx_packets,
            dbg_mgmt_info->disassoc_tx_packets, dbg_mgmt_info->disassoc_rx_packets,
            dbg_mgmt_info->action_tx_packets, dbg_mgmt_info->action_rx_packets);
DONE:
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    siwifi_kfree(buf);
    return read;
}
DEBUGFS_READ_FILE_OPS(mgmt_info);

#define COUNT_MIB_RTS_ARRAY(MIB_ATTR_NAME) \
	mib->MIB_ATTR_NAME[0] + mib->MIB_ATTR_NAME[1] + mib->MIB_ATTR_NAME[2] + mib->MIB_ATTR_NAME[3] + \
	mib->MIB_ATTR_NAME[5] + mib->MIB_ATTR_NAME[6] + mib->MIB_ATTR_NAME[7]
static ssize_t siwifi_dbgfs_ctrl_info_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    char *buf;
    int ret = 0;
    ssize_t read;
    int bufsz = 2048;
    struct dbg_get_ctrl_info_cfm cfm;
    struct dbg_ctrl_info *dbg_ctrl_info;
    struct siwifi_hw *siwifi_hw = (struct siwifi_hw *)file->private_data;
    struct machw_mib_tag *mib = (struct machw_mib_tag*)(REG_MIB_BASE(siwifi_hw->mod_params->is_hb));

    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (!buf) {
        return 0;
    }

    if (siwifi_send_dbg_get_ctrl_info_req(siwifi_hw, &cfm, 0)) {
        printk("can not send get_ctrl_info_req\n");
        goto DONE;
    }

    dbg_ctrl_info = (struct dbg_ctrl_info *)siwifi_hw->dbg_ctrl_info_elem.addr;

    ret += scnprintf(&buf[ret], min_t(size_t, bufsz - ret - 1, count - ret),
            "\n | packets         | TX           | RX\n"
            " | ba              | %-12d | %-12d\n"
            " | bar             | %-12d | %-12d\n"
            " | pspoll          | %-12d | %-12d\n"
            " | rts             | %-12d | %-12d\n"
            " | cts             | %-12d | %-12d\n"
            " | ack             | %-12d | %-12d\n"
            " | cfend           | %-12d | %-12d\n\n",
            dbg_ctrl_info->ba_tx_packets, dbg_ctrl_info->ba_rx_packets,
            dbg_ctrl_info->bar_tx_packets, dbg_ctrl_info->bar_rx_packets,
            dbg_ctrl_info->pspoll_tx_packets, dbg_ctrl_info->pspoll_rx_packets,
            (COUNT_MIB_RTS_ARRAY(dot11_qos_rts_success_count) +  COUNT_MIB_RTS_ARRAY(dot11_qos_rts_failure_count)), dbg_ctrl_info->rts_rx_packets,
            dbg_ctrl_info->cts_tx_packets, dbg_ctrl_info->cts_rx_packets,
            dbg_ctrl_info->ack_tx_packets, dbg_ctrl_info->ack_rx_packets,
            dbg_ctrl_info->cfend_tx_packets, dbg_ctrl_info->cfend_rx_packets);
DONE:
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    siwifi_kfree(buf);
    return read;
}
DEBUGFS_READ_FILE_OPS(ctrl_info);

static ssize_t siwifi_dbgfs_set_cca_parameter_write(struct file *file,
                                        const char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    char buf[128];
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    uint32_t param[12];

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    sscanf(buf, "%d %d %d %d %d %d %x %x %d %d %d %d",
            &param[0], &param[1], &param[2], &param[3], &param[4], &param[5],
            &param[6], &param[7], &param[8], &param[9], &param[10], &param[11]);

    siwifi_send_dbg_set_cca_parameter_req(siwifi_hw, param);

    return count;
}
DEBUGFS_WRITE_FILE_OPS(set_cca_parameter);

extern int siwifi_fast_channel_switch(struct siwifi_hw *siwifi_hw);
static ssize_t siwifi_dbgfs_fast_channel_switch_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    char *buf;
    int ret = 0;
    ssize_t read;
    int bufsz = 4096;
    buf = siwifi_kmalloc(bufsz, GFP_ATOMIC);
    if (buf == NULL)
        return 0;
    siwifi_fast_channel_switch(siwifi_hw);
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
    siwifi_kfree(buf);
    return read;
}
DEBUGFS_READ_FILE_OPS(fast_channel_switch);

static ssize_t siwifi_dbgfs_send_frame_custom_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = file->private_data;
    char buf[256];
    int ret;
    ssize_t read;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "Usage: \n"
                    "1. First Parameter : [FRAME_TYPE]\n"
                    "2. Subsequent Parameters :\n"
                    "Fixed frame: [VIF_NAME] [MAC_ADDR]\n"
                    "Custom frame: [VIF_IDX] [AC] [BW] [MCS] [NSS] [FORMAT_MOD] "
                    "[GI] [LDPC] [EXPECT_ACK] [ABSOLUTE FILE PATH]\n"
                    "last custom_frame statinfo: 0x%x\n", priv->debug_frame_statinfo);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}
// ACK frame for example
u8 siwifi_example_payload[] = {
    0xD4, 0x00, 0x00, 0x00, 0xA8, 0x5A, 0xF3, 0xFF, 0x00, 0xA4
};

static void siwifi_prase_hex_pkt(char *buf, size_t size, uint8_t *payload)
{
    int i;
    for (i = 0; i < size / 3; i++) {
        sscanf(&((char *)buf)[i * 3], "%hhx", &payload[i]);
    }
}

static ssize_t siwifi_dbgfs_send_frame_custom_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct siwifi_vif *vif;
    struct sk_buff *ret_skb = NULL;
    struct mm_send_debug_frame_req *param = NULL;
    char default_path[16] = "/root/pkt.txt";
    char pkt_path[64];
    char frame_name[20];
    void *pkt_buf = NULL;
    uint8_t *payload = NULL;
    loff_t pkt_bufsize = 0x1000;
    loff_t pkt_filesize = 0;
    int ret;
    int error;
    int filelen;
    char *data = NULL;
    uint8_t mac[6];
    char buf[128];
    char vif_name[10];

    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;

    buf[len] = '\0';

    if((sscanf(buf, "%s", frame_name))) {
        if(!strcmp(frame_name, "custom_frame")) {
            param = siwifi_kmalloc(sizeof(struct mm_send_debug_frame_req), GFP_ATOMIC);
            if (!param)
                return -ENOMEM;
            if ((sscanf(buf + strlen(frame_name) + 1, "%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %s",
                        &param->vif_idx,&param->ac, &param->bw, &param->mcs, &param->nss, &param->format_mod,
                        &param->gi, &param->ldpc, &param->expect_ack, pkt_path) > 0)) {
                if (!(param->ac < AC_MAX) && (param->bw <= PHY_CHNL_BW_160) && (param->mcs <= 15) &&
                        (param->nss <= 1) && (param->format_mod < 8) && (param->gi < 3) &&
                        (param->ldpc <= 1)) {
                    printk("invalid configuration");
                    if (param)
                        siwifi_kfree(param);
                    return -EINVAL;
                }

                // Not a valid path, use default
                if (pkt_path[0] != '/') {
                    memcpy(pkt_path, default_path, sizeof(default_path));
                    printk("invalid path");
                }
                if ((ret = kernel_read_file_from_path(pkt_path, &pkt_buf, &pkt_filesize, pkt_bufsize, READING_MODULE)) >= 0) {
                    filelen = pkt_filesize;
                    if ((filelen / 3) == 0) {
                        printk("payload length is 0, use example_payload");
                        goto example_payload;
                    }
                    data = pkt_buf;
                    payload = siwifi_kmalloc(filelen / 3, GFP_ATOMIC);

                    if (!payload) {
                        goto end;
                    }
                    siwifi_prase_hex_pkt(data, pkt_filesize, payload);
                    param->frmlen = filelen / 3;
                    if ((error = siwifi_send_debug_frame(priv, param, payload))) {
                        goto end;
                    } else {
                        printk("send custom_frame with payload in %s\n", pkt_path);
                    }
                } else {
                    printk("Failed to read file %s with error code %d, use example payload", pkt_path, ret);
                    goto example_payload;
                }
            }
        }
        else if((sscanf(buf + strlen(frame_name) + 1, "%s %hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
                vif_name, &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) > 0)) {
            list_for_each_entry(vif, &priv->vifs, list) {
                if(!strcmp(vif_name, vif->ndev->name)) {
                    if(!vif->ndev || !vif->up) {
                        printk("vif %s can not use\n", vif_name);
                        goto end;
                    }
                    printk("%s send %s frame to %hhx:%hhx:%hhx:%hhx:%hhx:%hhx\n",
                            vif_name, frame_name, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
                    if(!strcmp(frame_name, "beacon")) {
                        ret_skb = siwifi_create_beacon(vif, mac);
                        if (ret_skb)
                            siwifi_frame_send_mgmt(vif, ret_skb);
                    } else if(!strcmp(frame_name, "auth_req")) {
                        ret_skb = siwifi_create_auth_req(vif, mac);
                        if (ret_skb)
                            siwifi_frame_send_mgmt(vif, ret_skb);
                    } else if(!strcmp(frame_name, "auth_rsp")) {
                        ret_skb = siwifi_create_auth_rsp(vif, mac);
                        if (ret_skb)
                            siwifi_frame_send_mgmt(vif, ret_skb);
                    } else if(!strcmp(frame_name, "deauth")) {
                        ret_skb = siwifi_create_deauth(vif, mac);
                        if (ret_skb)
                            siwifi_frame_send_mgmt(vif, ret_skb);
                    } else if(!strcmp(frame_name, "assoc_req")) {
                        ret_skb = siwifi_create_assoc_req(vif, mac);
                        if(ret_skb)
                            siwifi_frame_send_mgmt(vif, ret_skb);
                    } else if(!strcmp(frame_name, "assoc_rsp")) {
                        ret_skb = siwifi_create_assoc_rsp(vif, mac);
                        if(ret_skb)
                            siwifi_frame_send_mgmt(vif, ret_skb);
                    } else if(!strcmp(frame_name, "probe_rsp")) {
                        ret_skb = siwifi_create_probe_rsp(vif, mac);
                        if(ret_skb)
                            siwifi_frame_send_mgmt(vif, ret_skb);
                    } else {
                        printk("ERROR frame type!\n");
                        return -EFAULT;
                    }
                }
            }
        }
    }

end:
    if(param)
        siwifi_kfree(param);
    if(payload)
        siwifi_kfree(payload);
    if(pkt_buf)
        vfree(pkt_buf);

    return count;

example_payload:

    param->frmlen = sizeof(siwifi_example_payload);
    if ((error = siwifi_send_debug_frame(priv, param, siwifi_example_payload)))
    {
        goto out;
    }

out:
    if (param)
        siwifi_kfree(param);
    if (pkt_buf)
        vfree(pkt_buf);

    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(send_frame_custom);

static ssize_t siwifi_dbgfs_sta_tid_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = file->private_data;
    char buf[256];
    int ret = 0;
    ssize_t read;
    struct siwifi_vif *siwifi_vif;
    struct siwifi_sta *siwifi_sta;

    list_for_each_entry(siwifi_vif, &siwifi_hw->vifs, list) {
        if (SIWIFI_VIF_TYPE(siwifi_vif) ==  NL80211_IFTYPE_AP) {
            list_for_each_entry(siwifi_sta, &siwifi_vif->ap.sta_list, list) {
                if(siwifi_sta->user_tid >= 0 && siwifi_sta->user_tid != SIWIFI_USER_TID_NOT_SET) {
                    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                            "sta %d(%pM) tid:%d\n", siwifi_sta->sta_idx, siwifi_sta->mac_addr, siwifi_sta->user_tid);
                }
            }
        }
    }

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_sta_tid_write(struct file *file,
                                    const char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    struct siwifi_vif *siwifi_vif;
    struct siwifi_sta *siwifi_sta;
    char buf[128];
    size_t len = min_t(size_t, count, sizeof(buf) - 1);
    u8 mac[6];
    int tid;

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;
    buf[len] = '\0';
    if(sscanf(buf, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx %d", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5], &tid) > 0) {
        list_for_each_entry(siwifi_vif, &siwifi_hw->vifs, list) {
            if (SIWIFI_VIF_TYPE(siwifi_vif) ==  NL80211_IFTYPE_AP) {
                list_for_each_entry(siwifi_sta, &siwifi_vif->ap.sta_list, list) {
                    if(!memcmp(mac, siwifi_sta->mac_addr, 6)) {
                        if(tid < 0){
                            siwifi_sta->user_tid = SIWIFI_USER_TID_NOT_SET;
                        } else {
                            siwifi_sta->user_tid = tid;
                        }
                        break;
                    }
                }
            }
        }
    }
    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(sta_tid)

#if defined CONFIG_SIWIFI_IGMP
static ssize_t siwifi_dbgfs_enable_multicast_to_unicast_ssid_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    struct siwifi_vif *siwifi_vif;
    char buf[256];
    ssize_t read;
    int ret = 0;
    size_t bufsz = sizeof(buf)-1;

    list_for_each_entry(siwifi_vif, &siwifi_hw->vifs, list) {
        ret += snprintf(buf + ret, bufsz - ret, "%s : %u\n",
                        siwifi_vif->wdev.ssid, siwifi_vif->enable_multicast_to_unicast);
    }

    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    return read;
}

static ssize_t siwifi_dbgfs_enable_multicast_to_unicast_ssid_write(struct file *file,
                                      const char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *siwifi_hw = private_data_proc_debug(file->private_data);
    struct siwifi_vif * siwifi_vif;
    char buf[64];
    uint32_t enable;
    u8 mac[6];
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if(copy_from_user(buf,user_buf,len))
        return -EFAULT;
    buf[len] = '\0';
    if(sscanf(buf, "%u,%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
        &enable, &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) > 0) {
        list_for_each_entry(siwifi_vif, &siwifi_hw->vifs, list) {
            if(!memcmp(mac, siwifi_vif->ndev->dev_addr, 6)) {
                siwifi_vif->enable_multicast_to_unicast = enable;
                break;
            }
        }
    }
    return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(enable_multicast_to_unicast_ssid);
#endif /*CONFIG_SIWIFI_DEBUGFS && CONFIG_SIWIFI_IGMP*/

int siwifi_dbgfs_register(struct siwifi_hw *siwifi_hw, const char *name)
{
#ifdef CONFIG_SIWIFI_DEBUGFS
    struct STRUCT_ENTRY_PROC_DEBUG *phyd = siwifi_hw->wiphy->debugfsdir;
#elif defined CONFIG_SIWIFI_PROCFS
    struct STRUCT_ENTRY_PROC_DEBUG *phyd = siwifi_hw->procfsdir;
#endif /*CONFIG_SIWIFI_DEBUGFS*/
    struct STRUCT_ENTRY_PROC_DEBUG *dir_rc;
    struct STRUCT_ENTRY_PROC_DEBUG *dir_throughput;
    struct STRUCT_ENTRY_PROC_DEBUG *dir_lm;
#ifdef CONFIG_SIWIFI_TEMPERATURE_CONTROL
    struct STRUCT_ENTRY_PROC_DEBUG *dir_temp_ctl;
#endif

    struct siwifi_debugfs *siwifi_debugfs = &siwifi_hw->debugfs;
    struct STRUCT_ENTRY_PROC_DEBUG *dir_drv;
#ifdef CONFIG_DBG_DUMP
    struct STRUCT_ENTRY_PROC_DEBUG *dir_diags;
#endif

    if (!(dir_drv = DEBUGFS_CREATE_DIR(name, phyd)))
        return -ENOMEM;

    siwifi_debugfs->dir = dir_drv;
    siwifi_debugfs->unregistering = false;

    siwifi_debugfs->force_trigger_type = 0;
    siwifi_debugfs->recovery = 1;
#ifdef CONFIG_HEART_BEAT
    siwifi_debugfs->recovery_hb_count = 0;
#endif
    siwifi_debugfs->skb_alloc_fail_cnt = 0;
#ifdef CONFIG_DBG_DUMP
    if (!(dir_diags = DEBUGFS_CREATE_DIR("diags", dir_drv)))
        goto err;
#endif

    if (!(dir_throughput = DEBUGFS_CREATE_DIR("throughput_ctrl", dir_drv)))
        goto err;
    if (!(dir_rc = DEBUGFS_CREATE_DIR("rc", dir_drv)))
        goto err;
    siwifi_debugfs->dir_rc = dir_rc;
    INIT_WORK(&siwifi_debugfs->rc_stat_work, siwifi_rc_stat_work);
    INIT_LIST_HEAD(&siwifi_debugfs->rc_config_save);
    siwifi_debugfs->rc_write = siwifi_debugfs->rc_read = 0;
    memset(siwifi_debugfs->rc_sta, 0xFF, sizeof(siwifi_debugfs->rc_sta));
    memset(siwifi_debugfs->rc_sta_action, RC_IDLE, sizeof(siwifi_debugfs->rc_sta_action));
#ifdef CONFIG_BRIDGE_ACCELERATE
    DEBUGFS_ADD_FILE(accel,  dir_drv, S_IWUSR | S_IRUSR);
#endif
    DEBUGFS_ADD_FILE(trx_stats,  dir_drv, S_IRUSR);
    DEBUGFS_ADD_FILE(rxdesc,  dir_drv, S_IRUSR);
    DEBUGFS_ADD_FILE(tx_ctrl, dir_throughput, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(rx_ctrl, dir_throughput, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_U32(enable_rc, dir_drv, &siwifi_hw->enable_rc,
                    S_IWUSR | S_IRUSR);
#if defined CONFIG_SIWIFI_IGMP
    DEBUGFS_ADD_U32(enable_multicast_to_unicast, dir_drv, &siwifi_hw->enable_multicast_to_unicast,
                    S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(enable_multicast_to_unicast_ssid, dir_drv, S_IWUSR | S_IRUSR);    
#endif
#ifdef CONFIG_SIWIFI_CH_OFFSET
    DEBUGFS_ADD_FILE(ch_offset, dir_drv, S_IWUSR | S_IRUSR);
#endif
#ifdef CONFIG_SIWIFI_COOLING_TEMP
    DEBUGFS_ADD_U32(temp_disable, dir_drv, &siwifi_hw->temp_disable,
                    S_IWUSR | S_IRUSR);
#endif
    DEBUGFS_ADD_U32(debug_get_survey, dir_drv, &siwifi_hw->debug_get_survey,
                S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(disable_wmm_edca, dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(fast_channel_switch, dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_U32(wmm_edca_interval, dir_drv, &siwifi_hw->wmm_edca_interval,
            S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_U32(wmm_edca_counter_drop, dir_drv, &siwifi_hw->wmm_edca_counter_drop,
            S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(mgmt_info, dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(ctrl_info, dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(sta_tid, dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(set_cca_parameter, dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_U32(wmm_edca_pkt_threshold, dir_drv, &siwifi_hw->wmm_edca_pkt_threshold,
            S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_U32(disable_cca_channel_switch, dir_drv, &siwifi_hw->disable_cca_channel_switch,
            S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(siwifi_static, dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_U32(tcp_check, dir_drv, &siwifi_hw->siwifi_static_enable_tcp_check, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_U32(scan_timeout, dir_drv, &siwifi_hw->scan_timeout,
            S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_U32(amsdu_threshold, dir_drv, &siwifi_hw->amsdu_threshold,
            S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_U32(wmm_debug_enable, dir_drv, &siwifi_hw->wmm_debug_enable,
			S_IWUSR | S_IRUSR);
#ifdef CONFIG_SIWIFI_TRACE_SKB
    DEBUGFS_ADD_U32(snoop_sta, dir_drv, &siwifi_hw->trace_ctx.debug_sta_idx,
                    S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(debug,  dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(debug_result,  dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(record,  dir_drv, S_IWUSR | S_IRUSR);
#endif

    DEBUGFS_ADD_U32(tcp_pacing_shift, dir_drv, &siwifi_hw->tcp_pacing_shift,
                    S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(band_type,  dir_drv, S_IRUSR);
    DEBUGFS_ADD_FILE(dump_lmac_debuginfo,  dir_drv, S_IRUSR);
#ifdef CONFIG_DEBUG_TXQ_STOP
    DEBUGFS_ADD_FILE(txq_status,  dir_drv, S_IRUSR);
#endif
    DEBUGFS_ADD_FILE(version,  dir_drv, S_IRUSR);
    DEBUGFS_ADD_FILE(compile_time,  dir_drv, S_IRUSR);
    DEBUGFS_ADD_FILE(mib,  dir_drv, S_IRUSR);
    DEBUGFS_ADD_U32(max_process_time, dir_drv, &siwifi_hw->task_max_process_time,
                    S_IWUSR | S_IRUSR);
#ifdef CONFIG_SIWIFI_TX_POWER_CALI
    DEBUGFS_ADD_FILE(cali_table,  dir_drv, S_IRUSR);
#endif
    DEBUGFS_ADD_FILE(stats,  dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(sys_stats, dir_drv,  S_IRUSR);
#ifdef CONFIG_SIWIFI_TRX_STAT
    DEBUGFS_ADD_FILE(lmactx,  dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(lmacrx,  dir_drv, S_IWUSR | S_IRUSR);
#endif
    DEBUGFS_ADD_FILE(cde_info,  dir_drv, S_IRUSR);
    DEBUGFS_ADD_FILE(txpower,  dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(txpower_lvl,  dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(antenna_number,  dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(rssi_inbandpower_20,  dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(rssi_inbandpower_40,  dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(rssi_inbandpower_80,  dir_drv, S_IWUSR | S_IRUSR);
#if defined CONFIG_SIWIFI_IGMP
    DEBUGFS_ADD_FILE(multicast_group,  dir_drv, S_IWUSR | S_IRUSR);
#endif
#if defined CONFIG_SIWIFI_REPEATER
    DEBUGFS_ADD_FILE(repeater_info,  dir_drv, S_IWUSR | S_IRUSR);
#endif
#ifdef CONFIG_SIWIFI_TX_POWER_CALI
    DEBUGFS_ADD_FILE(txpower_cali_enable, dir_drv, S_IWUSR | S_IRUSR);
#endif
    DEBUGFS_ADD_FILE(recovery_enable,  dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(drop_multicast,  dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(run_state,  dir_drv, S_IRUSR);
    DEBUGFS_ADD_FILE(fixed_gain,  dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(beacon_insert_info, dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(assoc_insert_info, dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(auth_insert_info, dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(probe_insert_info, dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(clear_insert_info, dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(repeater_status, dir_drv, S_IRUSR);
    DEBUGFS_ADD_FILE(ndevq, dir_drv, S_IRUSR);
    DEBUGFS_ADD_FILE(src_filter,  dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(txq_time_stat, dir_drv, S_IRUSR);
    {
        struct STRUCT_ENTRY_PROC_DEBUG *dir_atf;
        if (!(dir_atf = DEBUGFS_CREATE_DIR("airtime_fairness", dir_drv)))
            goto err;
        DEBUGFS_ADD_FILE(atf, dir_atf, S_IRUSR);
        DEBUGFS_ADD_FILE(enable_atf, dir_atf, S_IWUSR | S_IRUSR);
    }

    // enable IQ engine player or recoder
#ifdef CONFIG_SIWIFI_IQENGINE
    {
        struct STRUCT_ENTRY_PROC_DEBUG *dir_iqengine;
        if (!(dir_iqengine = DEBUGFS_CREATE_DIR("iqengine", dir_drv)))
            goto err;
        DEBUGFS_ADD_FILE(iq_mode, dir_iqengine, S_IWUSR | S_IRUSR);
        DEBUGFS_ADD_FILE(iq_length,     dir_iqengine, S_IWUSR | S_IRUSR);
        DEBUGFS_ADD_FILE(iq_enable,     dir_iqengine, S_IWUSR);
        DEBUGFS_ADD_FILE(iq_help,     dir_iqengine, S_IRUSR);
    }
#endif

    DEBUGFS_ADD_FILE(txq, dir_drv, S_IRUSR);
    DEBUGFS_ADD_FILE(hwq, dir_drv, S_IRUSR);
    DEBUGFS_ADD_FILE(acsinfo, dir_drv, S_IRUSR);
#ifdef NEW_SCHEDULE
    DEBUGFS_ADD_FILE(burstinfo, dir_drv, S_IWUSR | S_IRUSR);
#endif
#ifdef CONFIG_SIWIFI_MUMIMO_TX
    DEBUGFS_ADD_FILE(mu_group, dir_drv, S_IRUSR);
#endif

#ifdef CONFIG_HEART_BEAT
    DEBUGFS_ADD_FILE(recovery_hb, dir_drv, S_IRUSR);
#endif
    DEBUGFS_ADD_FILE(skb_alloc_fail, dir_drv, S_IRUSR);
#ifdef CONFIG_ERROR_DUMP
    DEBUGFS_ADD_FILE(error_dump, dir_drv, S_IRUSR);
#endif
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
	DEBUGFS_ADD_BOOL(ate_stats, dir_drv, &siwifi_hw->ate_env.ate_start);
	DEBUGFS_ADD_BOOL(ate_macbyp_tx_stats,  dir_drv,&siwifi_hw->ate_env.tx_macbypass_start);
	DEBUGFS_ADD_BOOL(ate_tx_stats, dir_drv, &siwifi_hw->ate_env.tx_frame_start);
#endif
#ifdef CONFIG_SIWIFI_P2P_DEBUGFS
    {
        /* Create a p2p directory */
        struct STRUCT_ENTRY_PROC_DEBUG *dir_p2p;
        if (!(dir_p2p = DEBUGFS_CREATE_DIR("p2p", dir_drv)))
            goto err;

        /* Add file allowing to control Opportunistic PS */
        DEBUGFS_ADD_FILE(oppps, dir_p2p, S_IRUSR);
        /* Add file allowing to control Notice of Absence */
        DEBUGFS_ADD_FILE(noa, dir_p2p, S_IRUSR);
    }
#endif /* CONFIG_SIWIFI_P2P_DEBUGFS */

#ifdef CONFIG_DBG_DUMP
    if (siwifi_dbgfs_register_fw_dump(siwifi_hw, dir_drv, dir_diags))
        goto err;
    DEBUGFS_ADD_FILE(fw_dbg, dir_diags, S_IWUSR | S_IRUSR);

#ifdef CONFIG_SIWIFI_FW_TRACE
    if (!siwifi_fw_trace_init(&siwifi_hw->debugfs.fw_trace,
                            siwifi_ipc_fw_trace_desc_get(siwifi_hw))) {
        DEBUGFS_ADD_FILE(fw_trace, dir_diags, S_IWUSR | S_IRUSR);
        if (siwifi_hw->debugfs.fw_trace.buf.nb_compo)
            DEBUGFS_ADD_FILE(fw_trace_level, dir_diags, S_IWUSR | S_IRUSR);
    } else {
        siwifi_debugfs->fw_trace.buf.data = NULL;
    }
#endif

    DEBUGFS_ADD_FILE(force_trigger_type, dir_diags, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(hwq_credit_dec,     dir_diags, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(la_clk,    dir_diags,          S_IRUSR);
#endif
#ifdef CONFIG_SIWIFI_RADAR
    {
        struct STRUCT_ENTRY_PROC_DEBUG *dir_radar, *dir_sec;
        if (!(dir_radar = DEBUGFS_CREATE_DIR("radar", dir_drv)))
            goto err;

        DEBUGFS_ADD_FILE(pulses_prim, dir_radar, S_IRUSR);
        DEBUGFS_ADD_FILE(detected,    dir_radar, S_IRUSR);
        DEBUGFS_ADD_FILE(enable,      dir_radar, S_IRUSR);
        DEBUGFS_ADD_FILE(radar_event, dir_radar, S_IRUSR);
        DEBUGFS_ADD_FILE(radar_debugmode, dir_radar, S_IRUSR);
        DEBUGFS_ADD_FILE(radar_debug_printk, dir_radar, S_IRUSR);
        DEBUGFS_ADD_FILE(radar_freq_delta, dir_radar, S_IRUSR);

        if (siwifi_hw->phy_cnt == 2) {
            DEBUGFS_ADD_FILE(pulses_sec, dir_radar, S_IRUSR);

            if (!(dir_sec = DEBUGFS_CREATE_DIR("sec", dir_radar)))
                goto err;

            DEBUGFS_ADD_FILE(band,    dir_sec, S_IWUSR | S_IRUSR);
            DEBUGFS_ADD_FILE(type,    dir_sec, S_IWUSR | S_IRUSR);
            DEBUGFS_ADD_FILE(prim20,  dir_sec, S_IWUSR | S_IRUSR);
            DEBUGFS_ADD_FILE(center1, dir_sec, S_IWUSR | S_IRUSR);
            DEBUGFS_ADD_FILE(center2, dir_sec, S_IWUSR | S_IRUSR);
            DEBUGFS_ADD_FILE(set,     dir_sec, S_IWUSR | S_IRUSR);
        }
    }
#endif /* CONFIG_SIWIFI_RADAR */

    if (!(dir_lm = DEBUGFS_CREATE_DIR("lite_mem", dir_drv)))
        goto err;
    DEBUGFS_ADD_FILE(tx_memory_limit, dir_lm, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(tx_cnt_limit, dir_lm, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(lm_stats, dir_lm, S_IRUSR);
    DEBUGFS_ADD_FILE(lm_enable, dir_lm, S_IRUSR);
    DEBUGFS_ADD_FILE(mpinfo,  dir_drv, S_IRUSR);
#if defined CONFIG_SIWIFI_TEMPERATURE_CONTROL
    if (!(dir_temp_ctl = DEBUGFS_CREATE_DIR("temp_ctl", dir_drv)))
        goto err;
    DEBUGFS_ADD_FILE(temp_ctl_state, dir_temp_ctl, S_IWUSR | S_IRUSR);
#endif
    DEBUGFS_ADD_FILE(max_rx_process, dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(mem_info, dir_drv, S_IRUSR);
#ifdef CONFIG_WIFI_RX_THREAD
    DEBUGFS_ADD_FILE(rxthread, dir_drv, S_IRUSR);
#endif
#ifdef CONFIG_PRIV_RX_BUFFER_POOL
    DEBUGFS_ADD_FILE(rxbuf_pool_enable, dir_drv, S_IRUSR | S_IWUSR);
#endif
    DEBUGFS_ADD_FILE(dump_vif, dir_drv, S_IRUSR);

#if defined CONFIG_VDR_HW
    DEBUGFS_ADD_FILE(vendor_stat, dir_drv, S_IWUSR | S_IRUSR);
#endif
    DEBUGFS_ADD_FILE(send_frame_custom, dir_drv, S_IWUSR | S_IRUSR);
    DEBUGFS_ADD_FILE(rc_set_no_ss, dir_drv, S_IWUSR);
    DEBUGFS_ADD_FILE(ave_txq, dir_drv, S_IWUSR | S_IRUSR);
    return 0;

err:
    siwifi_dbgfs_unregister(siwifi_hw);
    return -ENOMEM;
}

void siwifi_dbgfs_unregister(struct siwifi_hw *siwifi_hw)
{
    struct siwifi_debugfs *siwifi_debugfs = &siwifi_hw->debugfs;

    struct siwifi_rc_config_save *cfg, *next;
    list_for_each_entry_safe(cfg, next, &siwifi_debugfs->rc_config_save, list) {
        list_del(&cfg->list);
        siwifi_kfree(cfg);
    }

#ifdef CONFIG_SIWIFI_FW_TRACE
    siwifi_fw_trace_deinit(&siwifi_hw->debugfs.fw_trace);
#endif

    if (!siwifi_hw->debugfs.dir)
        return;

    siwifi_debugfs->unregistering = true;
    flush_work(&siwifi_debugfs->helper_work);
    flush_work(&siwifi_debugfs->rc_stat_work);
    DEBUGFS_REMOVE_DIR(siwifi_hw->debugfs.dir);
    siwifi_hw->debugfs.dir = NULL;
}
