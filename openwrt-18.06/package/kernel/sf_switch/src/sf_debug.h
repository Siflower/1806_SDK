/*************************************************************************
	> File Name: drivers/net/ethernet/siflower/sf_debug.h
	> Author: Qin xia
	> Mail: qin.xia@siflower.com.cn
	> Created Time: 2017年01月21日 星期六 10时48分13秒
 ************************************************************************/

#include <linux/netdevice.h>
#include <linux/kernel.h>
#include "sf_common.h"

enum DEBUG_MODL {
	ETH_INIT_DEBUG,
	ETH_POLL_DEBUG,
	ETH_TX_DEBUG,
	ETH_RX_DEBUG,
	ETH_IRQ_DEBUG,
	SWITCH_INIT_DEBUG,
	SWITCH_DEBUG,
	ETH_RESET_DEBUG,
};

void  print_msg(int mode, struct net_device *netdev, const char* fmt, ...);
#ifdef CONFIG_DEBUG_FS
int sfax8_npu_debug_show(struct seq_file *m, void *v);
int sfax8_npu_debug_open(struct inode *inode, struct file *file);
ssize_t sfax8_npu_debug_read(struct file *file, char __user *buf, size_t count, loff_t *ppos);
ssize_t sfax8_npu_debug_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);

int sfax8_npu_counts_show(struct seq_file *m, void *v);
int sfax8_npu_counts_open(struct inode *inode, struct file *file);
ssize_t sfax8_npu_counts_read(struct file *file, char __user *buf, size_t count, loff_t *ppos);
ssize_t sfax8_npu_counts_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);
#endif

extern int g_i_tx_wake_time;
