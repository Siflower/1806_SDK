/**
 ******************************************************************************
 *
 * @file siwifi_fw_dump.c
 *
 * @brief Definition of debug fs entries to process fw dump
 *
 * Copyright (C) Siflower 2018-2025
 *
 ******************************************************************************
 */


#include <linux/kmod.h>
#include <linux/debugfs.h>

#include "siwifi_defs.h"
#include "siwifi_debugfs.h"
#include "reg_la.h"
#include "siwifi_mem.h"

static ssize_t siwifi_dbgfs_rhd_read(struct file *file,
                                   char __user *user_buf,
                                   size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct dbg_debug_dump_tag *dump = priv->dbgdump_elem.buf.addr;
    ssize_t read;

    mutex_lock(&priv->dbgdump_elem.mutex);
    if (!priv->debugfs.trace_prst) {
        mutex_unlock(&priv->dbgdump_elem.mutex);
        return 0;
    }

    read = simple_read_from_buffer(user_buf, count, ppos,
                                   dump->rhd_mem,
                                   dump->dbg_info.rhd_len);

    mutex_unlock(&priv->dbgdump_elem.mutex);
    return read;
}

DEBUGFS_READ_FILE_OPS(rhd);

static ssize_t siwifi_dbgfs_rbd_read(struct file *file,
                                   char __user *user_buf,
                                   size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct dbg_debug_dump_tag *dump = priv->dbgdump_elem.buf.addr;
    ssize_t read;

    mutex_lock(&priv->dbgdump_elem.mutex);
    if (!priv->debugfs.trace_prst) {
        mutex_unlock(&priv->dbgdump_elem.mutex);
        return 0;
    }

    read = simple_read_from_buffer(user_buf, count, ppos,
                                   dump->rbd_mem,
                                   dump->dbg_info.rbd_len);

    mutex_unlock(&priv->dbgdump_elem.mutex);
    return read;
}

DEBUGFS_READ_FILE_OPS(rbd);

static ssize_t siwifi_dbgfs_thdx_read(struct file *file, char __user *user_buf,
                                    size_t count, loff_t *ppos, int idx)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct dbg_debug_dump_tag *dump = priv->dbgdump_elem.buf.addr;
    ssize_t read;

    mutex_lock(&priv->dbgdump_elem.mutex);
    if (!priv->debugfs.trace_prst) {
        mutex_unlock(&priv->dbgdump_elem.mutex);
        return 0;
    }

    read = simple_read_from_buffer(user_buf, count, ppos,
                                   &dump->thd_mem[idx],
                                   dump->dbg_info.thd_len[idx]);

    mutex_unlock(&priv->dbgdump_elem.mutex);
    return read;
}

static ssize_t siwifi_dbgfs_thd0_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    return siwifi_dbgfs_thdx_read(file, user_buf, count, ppos, 0);
}
DEBUGFS_READ_FILE_OPS(thd0);

static ssize_t siwifi_dbgfs_thd1_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    return siwifi_dbgfs_thdx_read(file, user_buf, count, ppos, 1);
}
DEBUGFS_READ_FILE_OPS(thd1);

static ssize_t siwifi_dbgfs_thd2_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    return siwifi_dbgfs_thdx_read(file, user_buf, count, ppos, 2);
}
DEBUGFS_READ_FILE_OPS(thd2);

static ssize_t siwifi_dbgfs_thd3_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    return siwifi_dbgfs_thdx_read(file, user_buf, count, ppos, 3);
}
DEBUGFS_READ_FILE_OPS(thd3);

#if (NX_TXQ_CNT == 5)
static ssize_t siwifi_dbgfs_thd4_read(struct file *file,
                                    char __user *user_buf,
                                    size_t count, loff_t *ppos)
{
    return siwifi_dbgfs_thdx_read(file, user_buf, count, ppos, 4);
}
DEBUGFS_READ_FILE_OPS(thd4);
#endif

static ssize_t siwifi_dbgfs_mactrace_read(struct file *file,
                                        char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    ssize_t read;

    mutex_lock(&priv->dbgdump_elem.mutex);
    if (!priv->debugfs.trace_prst) {
        char msg[64];

        if(test_bit(SIWIFI_DEV_STACK_RESTARTING, &priv->drv_flags) || priv->debugfs.recoverying){
            mutex_unlock(&priv->dbgdump_elem.mutex);
            return 0;
        }

        scnprintf(msg, sizeof(msg), "Force trigger\n");

        siwifi_dbgfs_trigger_fw_dump(priv, msg);

        mutex_unlock(&priv->dbgdump_elem.mutex);
        return 0;
    }

	//change la_mem from mactrace to la0(added by siflower),because we want
	//use cat ...../mactrace for recovery test,
	//we must use la0 instead of mactrace when generate vcd(genvcd.c) accordingly
    //read = simple_read_from_buffer(user_buf, count, ppos, dump->la_mem, dump->dbg_info.la_conf.trace_len);

    mutex_unlock(&priv->dbgdump_elem.mutex);

    return read;
}
DEBUGFS_READ_FILE_OPS(mactrace);

static ssize_t siwifi_dbgfs_la0_read(struct file *file,
                                        char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
#ifdef CFG_LA_ENABLE
    struct dbg_debug_dump_tag *dump = priv->dbgdump_elem.buf.addr;
#endif
    ssize_t read;

    mutex_lock(&priv->dbgdump_elem.mutex);
#ifdef CFG_LA_ENABLE
	if(dump->la_mem_valid)
		read = simple_read_from_buffer(user_buf, count, ppos,
                                  priv->dbgdump_elem.la_mem_addr,
                                  dump->dbg_info.la_conf.trace_len);
	else
#endif
		read = 0;

    mutex_unlock(&priv->dbgdump_elem.mutex);

    return read;
}
DEBUGFS_READ_FILE_OPS(la0);

static ssize_t siwifi_dbgfs_macdiags_read(struct file *file,
                                        char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct dbg_debug_dump_tag *dump = priv->dbgdump_elem.buf.addr;
    ssize_t read;

    mutex_lock(&priv->dbgdump_elem.mutex);
    if (!priv->debugfs.trace_prst) {
        mutex_unlock(&priv->dbgdump_elem.mutex);
        return 0;
    }

    read = simple_read_from_buffer(user_buf, count, ppos,
                                   dump->dbg_info.diags_mac,
                                   DBG_DIAGS_MAC_MAX * 2);

    mutex_unlock(&priv->dbgdump_elem.mutex);
    return read;
}

DEBUGFS_READ_FILE_OPS(macdiags);

static ssize_t siwifi_dbgfs_phydiags_read(struct file *file,
                                        char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct dbg_debug_dump_tag *dump = priv->dbgdump_elem.buf.addr;
    ssize_t read;

    mutex_lock(&priv->dbgdump_elem.mutex);
    if (!priv->debugfs.trace_prst) {
        mutex_unlock(&priv->dbgdump_elem.mutex);
        return 0;
    }

    read = simple_read_from_buffer(user_buf, count, ppos,
                                   dump->dbg_info.diags_phy,
                                   DBG_DIAGS_PHY_MAX * 2);

    mutex_unlock(&priv->dbgdump_elem.mutex);
    return read;
}

DEBUGFS_READ_FILE_OPS(phydiags);

static ssize_t siwifi_dbgfs_hwdiags_read(struct file *file,
                                       char __user *user_buf,
                                       size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct dbg_debug_dump_tag *dump = priv->dbgdump_elem.buf.addr;
    char buf[16];
    int ret;

    mutex_lock(&priv->dbgdump_elem.mutex);
    if (!priv->debugfs.trace_prst) {
        mutex_unlock(&priv->dbgdump_elem.mutex);
        return 0;
    }

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "%08X\n", dump->dbg_info.mac_cur_diag);

    mutex_unlock(&priv->dbgdump_elem.mutex);
    return simple_read_from_buffer(user_buf, count, ppos, buf, ret);
}

DEBUGFS_READ_FILE_OPS(hwdiags);

static ssize_t siwifi_dbgfs_plfdiags_read(struct file *file,
                                       char __user *user_buf,
                                       size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct dbg_debug_dump_tag *dump = priv->dbgdump_elem.buf.addr;
    char buf[16];
    int ret;

    mutex_lock(&priv->dbgdump_elem.mutex);
    if (!priv->debugfs.trace_prst) {
        mutex_unlock(&priv->dbgdump_elem.mutex);
        return 0;
    }

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "%08X\n", dump->dbg_info.la_conf.diag_conf);

    mutex_unlock(&priv->dbgdump_elem.mutex);
    return simple_read_from_buffer(user_buf, count, ppos, buf, ret);
}

DEBUGFS_READ_FILE_OPS(plfdiags);

static ssize_t siwifi_dbgfs_plfdiags1_read(struct file *file,
                                       char __user *user_buf,
                                       size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct dbg_debug_dump_tag *dump = priv->dbgdump_elem.buf.addr;
    char buf[16];
    int ret;

    mutex_lock(&priv->dbgdump_elem.mutex);
    if (!priv->debugfs.trace_prst) {
        mutex_unlock(&priv->dbgdump_elem.mutex);
        return 0;
    }

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "%08X\n", dump->dbg_info.la_conf.diag_conf1);

    mutex_unlock(&priv->dbgdump_elem.mutex);
    return simple_read_from_buffer(user_buf, count, ppos, buf, ret);
}

DEBUGFS_READ_FILE_OPS(plfdiags1);

static ssize_t siwifi_dbgfs_swdiags_read(struct file *file,
                                      char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct dbg_debug_dump_tag *dump = priv->dbgdump_elem.buf.addr;
    ssize_t read;

    mutex_lock(&priv->dbgdump_elem.mutex);
    if (!priv->debugfs.trace_prst) {
        mutex_unlock(&priv->dbgdump_elem.mutex);
        return 0;
    }

    read = simple_read_from_buffer(user_buf, count, ppos,
                                   &dump->dbg_info.sw_diag,
                                   dump->dbg_info.sw_diag_len);

    mutex_unlock(&priv->dbgdump_elem.mutex);
    return read;
}

DEBUGFS_READ_FILE_OPS(swdiags);

static ssize_t siwifi_dbgfs_error_read(struct file *file,
                                     char __user *user_buf,
                                     size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct dbg_debug_dump_tag *dump = priv->dbgdump_elem.buf.addr;
    ssize_t read;

    mutex_lock(&priv->dbgdump_elem.mutex);
    if (!priv->debugfs.trace_prst) {
        mutex_unlock(&priv->dbgdump_elem.mutex);
        return 0;
    }

    read = simple_read_from_buffer(user_buf, count, ppos,
                                   dump->dbg_info.error,
                                   strlen((char *)dump->dbg_info.error));

    mutex_unlock(&priv->dbgdump_elem.mutex);
    return read;
}

DEBUGFS_READ_FILE_OPS(error);

static ssize_t siwifi_dbgfs_rxdesc_read(struct file *file,
                                      char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct dbg_debug_dump_tag *dump = priv->dbgdump_elem.buf.addr;
    char buf[32];
    int ret;
    ssize_t read;

    mutex_lock(&priv->dbgdump_elem.mutex);
    if (!priv->debugfs.trace_prst) {
        mutex_unlock(&priv->dbgdump_elem.mutex);
        return 0;
    }

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "%08X\n%08X\n", dump->dbg_info.rhd,
                    dump->dbg_info.rbd);
    read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

    mutex_unlock(&priv->dbgdump_elem.mutex);
    return read;
}

DEBUGFS_READ_FILE_OPS(rxdesc);

static ssize_t siwifi_dbgfs_txdesc_read(struct file *file,
                                      char __user *user_buf,
                                      size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct dbg_debug_dump_tag *dump = priv->dbgdump_elem.buf.addr;
    char buf[64];
    int len = 0;
    int i;

    mutex_lock(&priv->dbgdump_elem.mutex);
    if (!priv->debugfs.trace_prst) {
        mutex_unlock(&priv->dbgdump_elem.mutex);
        return 0;
    }

    for (i = 0; i < NX_TXQ_CNT; i++) {
        len += scnprintf(&buf[len], min_t(size_t, sizeof(buf) - len - 1, count),
                         "%08X\n", dump->dbg_info.thd[i]);
    }

    mutex_unlock(&priv->dbgdump_elem.mutex);
    return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

DEBUGFS_READ_FILE_OPS(txdesc);

static ssize_t siwifi_dbgfs_macrxptr_read(struct file *file,
                                        char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct dbg_debug_dump_tag *dump = priv->dbgdump_elem.buf.addr;
    ssize_t read;

    mutex_lock(&priv->dbgdump_elem.mutex);
    if (!priv->debugfs.trace_prst) {
        mutex_unlock(&priv->dbgdump_elem.mutex);
        return 0;
    }

    read = simple_read_from_buffer(user_buf, count, ppos,
                                   &dump->dbg_info.rhd_hw_ptr,
                                   2 * sizeof(dump->dbg_info.rhd_hw_ptr));

    mutex_unlock(&priv->dbgdump_elem.mutex);
    return read;
}

DEBUGFS_READ_FILE_OPS(macrxptr);

static ssize_t siwifi_dbgfs_lamacconf_read(struct file *file,
                                         char __user *user_buf,
                                         size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct dbg_debug_dump_tag *dump = priv->dbgdump_elem.buf.addr;
    ssize_t read;

    mutex_lock(&priv->dbgdump_elem.mutex);
    if (!priv->debugfs.trace_prst) {
        mutex_unlock(&priv->dbgdump_elem.mutex);
        return 0;
    }

    read = simple_read_from_buffer(user_buf, count, ppos,
                                   dump->dbg_info.la_conf.conf,
                                   LA_CONF_LEN * 4);

    mutex_unlock(&priv->dbgdump_elem.mutex);
    return read;
}
DEBUGFS_READ_FILE_OPS(lamacconf);

static ssize_t siwifi_dbgfs_chaninfo_read(struct file *file,
                                        char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    struct dbg_debug_dump_tag *dump = priv->dbgdump_elem.buf.addr;
    char buf[4 * 32];
    int ret;

    mutex_lock(&priv->dbgdump_elem.mutex);
    if (!priv->debugfs.trace_prst) {
        mutex_unlock(&priv->dbgdump_elem.mutex);
        return 0;
    }

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "type:          %d\n"
                    "prim20_freq:   %d MHz\n"
                    "center1_freq:  %d MHz\n"
                    "center2_freq:  %d MHz\n",
                    (dump->dbg_info.chan_info.info1 >> 8)  & 0xFF,
                    (dump->dbg_info.chan_info.info1 >> 16) & 0xFFFF,
                    (dump->dbg_info.chan_info.info2 >> 0)  & 0xFFFF,
                    (dump->dbg_info.chan_info.info2 >> 16) & 0xFFFF);

    mutex_unlock(&priv->dbgdump_elem.mutex);
    return simple_read_from_buffer(user_buf, count, ppos, buf, ret);
}

DEBUGFS_READ_FILE_OPS(chaninfo);

static ssize_t siwifi_dbgfs_um_helper_read(struct file *file,
                                         char __user *user_buf,
                                         size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    char buf[sizeof(priv->debugfs.helper_cmd)];
    int ret;

    ret = scnprintf(buf, min_t(size_t, sizeof(buf) - 1, count),
                    "%s", priv->debugfs.helper_cmd);

    return simple_read_from_buffer(user_buf, count, ppos, buf, ret);
}

static ssize_t siwifi_dbgfs_um_helper_write(struct file *file,
                                          const char __user *user_buf,
                                          size_t count, loff_t *ppos)
{
    struct siwifi_hw *priv = private_data_proc_debug(file->private_data);
    int eobuf = min_t(size_t, sizeof(priv->debugfs.helper_cmd) - 1, count);

    priv->debugfs.helper_cmd[eobuf] = '\0';
    if (copy_from_user(priv->debugfs.helper_cmd, user_buf, eobuf))
        return -EFAULT;

    return count;
}

DEBUGFS_READ_WRITE_FILE_OPS(um_helper);

/*
 * Calls a userspace pgm
 */
int siwifi_um_helper(struct siwifi_debugfs *siwifi_debugfs, const char *cmd)
{
    char *envp[] = { "PATH=/sbin:/usr/sbin:/bin:/usr/bin", NULL };
    char **argv;
    int argc, ret;
	char *cmd_buf;
    int cmd_len;
    struct siwifi_hw *siwifi_hw = container_of(siwifi_debugfs, struct siwifi_hw, debugfs);

	cmd_len = strlen((cmd = cmd ? cmd : siwifi_debugfs->helper_cmd));
    if (!siwifi_debugfs->dir ||
        !cmd_len){
        return 0;
	}

	cmd_buf = (char *)siwifi_kmalloc(cmd_len + 8, GFP_ATOMIC);
	if(!cmd_buf)
		return 0;

    if(!siwifi_hw->mod_params->is_hb)
    	sprintf(cmd_buf, "%s %s\n", cmd, "lb");
    else
    	sprintf(cmd_buf, "%s %s\n", cmd, "hb");

    argv = argv_split(in_interrupt() ? GFP_ATOMIC : GFP_KERNEL, cmd_buf, &argc);
    if (!argc)
	{
		siwifi_kfree(cmd_buf);
        return -EINVAL;
	}

    if ((ret = call_usermodehelper(argv[0], argv, envp,
                                   UMH_WAIT_PROC | UMH_KILLABLE)))
        printk("Failed to call %s (%s returned %d)\n",
               argv[0], cmd, ret);
    argv_free(argv);
	siwifi_kfree(cmd_buf);

    return ret;
}

static void siwifi_um_helper_work(struct work_struct *ws)
{
    struct siwifi_debugfs *siwifi_debugfs = container_of(ws, struct siwifi_debugfs,
                                                     helper_work);
    struct siwifi_hw *siwifi_hw = container_of(siwifi_debugfs, struct siwifi_hw,
                                           debugfs);
#ifdef CFG_DBGDUMP
#if defined(CFG_A28_V_LA_CLK_BUG)
#ifdef CONFIG_SF16A18_WIFI_LA_ENABLE
#define LA_MEM_LINE_COUNT (32 * 1024)
#define MEMORY_SIZE_PER_LA_LINE 8
    uint32_t la_mem_addr = ((1 << siwifi_hw->mod_params->is_hb) == HB_MODULE) ? MEM_LB_SHARE_MEM_BASE : MEM_HB_SHARE_MEM_BASE;
	uint8_t *dst;
	uint8_t *src;
	uint32_t len = 0;
	uint8_t data;
	uint32_t remaining = 0;
	uint32_t curr_line = 0;
#endif
#endif

#ifdef CONFIG_SF16A18_WIFI_LA_ENABLE
#if defined(CFG_A28_V_LA_CLK_BUG)
	la_cntrl_set(LA_STOP_BIT | LA_CLK_SEL_BIT);
	data = readb((void *)REG_SYSM_SHARE_RAM_SEL);
    data &= ~(((1 << siwifi_hw->mod_params->is_hb) == LB_MODULE) ? ( 1 << 0 ) : ( 1 << 1 ));
	writeb(data, (void *)REG_SYSM_SHARE_RAM_SEL);

	remaining = la_writeaddr_getf() * MEMORY_SIZE_PER_LA_LINE;
	curr_line = (la_firstsample_get() + 1) % LA_MEM_LINE_COUNT;

	dst = (uint8_t *)siwifi_hw->dbgdump_elem.la_mem_addr;
	if (la_writeaddr_getf() >= LA_MEM_LINE_COUNT)
	{
		remaining = LA_MEM_LINE_COUNT * MEMORY_SIZE_PER_LA_LINE;
		//dump it into two steps
		//1,from first samples to the end
		src = (uint8_t *)(la_mem_addr + curr_line * MEMORY_SIZE_PER_LA_LINE);
		//why ignore the last samples
		len = (LA_MEM_LINE_COUNT - curr_line) * MEMORY_SIZE_PER_LA_LINE;
		ASSERT_ERR(dst != NULL);
		if(len) {
			memcpy(dst, src, len);
			remaining -= len;
		}
	}
	else
	{
		//remaining = curr_line * MEMORY_SIZE_PER_LA_LINE;
	}

	//2, from the 0 to the first samples
	src = (uint8_t *)la_mem_addr;
	dst += len;
	len = remaining;
	if(len) {
		memcpy(dst, src, len);
	}

	data = readb((void *)REG_SYSM_SHARE_RAM_SEL);
	data |= (((1 << CONFIG_BAND_TYPE) == LB_MODULE) ? ( 1 << 0 ) : ( 1 << 1 ));
	writeb(data, (void *)REG_SYSM_SHARE_RAM_SEL);
	la_cntrl_set(LA_START_BIT);
#endif
#endif
#endif

    siwifi_um_helper(siwifi_debugfs, NULL);
    if (!siwifi_debugfs->unregistering)
        siwifi_umh_done(siwifi_hw);
    siwifi_debugfs->helper_scheduled = false;
}

int siwifi_trigger_um_helper(struct siwifi_debugfs *siwifi_debugfs)
{
    struct siwifi_hw *siwifi_hw = container_of(siwifi_debugfs, struct siwifi_hw,
                                           debugfs);

    if (siwifi_debugfs->helper_scheduled == true) {
        dev_err(siwifi_hw->dev, "%s: Already scheduled\n", __func__);
        return -EBUSY;
    }

    spin_lock_bh(&siwifi_debugfs->umh_lock);
    if (siwifi_debugfs->unregistering) {
        spin_unlock_bh(&siwifi_debugfs->umh_lock);
        dev_err(siwifi_hw->dev, "%s: unregistering\n", __func__);
        return -ENOENT;
    }
    siwifi_debugfs->helper_scheduled = true;
    schedule_work(&siwifi_debugfs->helper_work);
    spin_unlock_bh(&siwifi_debugfs->umh_lock);

    return 0;
}

int siwifi_dbgfs_register_fw_dump(struct siwifi_hw *siwifi_hw,
                                struct STRUCT_ENTRY_PROC_DEBUG *dir_drv,
                                struct STRUCT_ENTRY_PROC_DEBUG *dir_diags)
{

    struct siwifi_debugfs *siwifi_debugfs = &siwifi_hw->debugfs;

    BUILD_BUG_ON(sizeof(CONFIG_SIWIFI_UM_HELPER_DFLT) >=
                 sizeof(siwifi_debugfs->helper_cmd));
    strncpy(siwifi_debugfs->helper_cmd,
            CONFIG_SIWIFI_UM_HELPER_DFLT, sizeof(siwifi_debugfs->helper_cmd));
    INIT_WORK(&siwifi_debugfs->helper_work, siwifi_um_helper_work);
    DEBUGFS_ADD_FILE(um_helper, dir_drv, S_IWUSR | S_IRUSR);

    siwifi_debugfs->trace_prst = siwifi_debugfs->helper_scheduled = false;
    spin_lock_init(&siwifi_debugfs->umh_lock);
    DEBUGFS_ADD_FILE(rhd,       dir_diags, S_IRUSR);
    DEBUGFS_ADD_FILE(rbd,       dir_diags, S_IRUSR);
    DEBUGFS_ADD_FILE(thd0,      dir_diags, S_IRUSR);
    DEBUGFS_ADD_FILE(thd1,      dir_diags, S_IRUSR);
    DEBUGFS_ADD_FILE(thd2,      dir_diags, S_IRUSR);
    DEBUGFS_ADD_FILE(thd3,      dir_diags, S_IRUSR);
#if (NX_TXQ_CNT == 5)
    DEBUGFS_ADD_FILE(thd4,      dir_diags, S_IRUSR);
#endif
    DEBUGFS_ADD_FILE(mactrace,  dir_diags, S_IRUSR);
	DEBUGFS_ADD_FILE(la0,		dir_diags, S_IRUSR);
    DEBUGFS_ADD_FILE(macdiags,  dir_diags, S_IRUSR);
    DEBUGFS_ADD_FILE(phydiags,  dir_diags, S_IRUSR);
    DEBUGFS_ADD_FILE(plfdiags,  dir_diags, S_IRUSR);
    DEBUGFS_ADD_FILE(plfdiags1,  dir_diags, S_IRUSR);
    DEBUGFS_ADD_FILE(hwdiags,   dir_diags, S_IRUSR);
    DEBUGFS_ADD_FILE(swdiags,   dir_diags, S_IRUSR);
    DEBUGFS_ADD_FILE(error,     dir_diags, S_IRUSR);
    DEBUGFS_ADD_FILE(rxdesc,    dir_diags, S_IRUSR);
    DEBUGFS_ADD_FILE(txdesc,    dir_diags, S_IRUSR);
    DEBUGFS_ADD_FILE(macrxptr,  dir_diags, S_IRUSR);
    DEBUGFS_ADD_FILE(lamacconf, dir_diags, S_IRUSR);
    DEBUGFS_ADD_FILE(chaninfo,  dir_diags, S_IRUSR);

    return 0;

  err:
    return -1;
}
