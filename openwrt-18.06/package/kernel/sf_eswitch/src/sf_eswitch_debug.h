#ifndef _SF_ESWICTH_DEBUG_H_
#define _SF_ESWICTH_DEBUG_H_
/*
* Description
*
* Copyright (C) 2016-2019 Qin.Xia <qin.xia@siflower.com.cn>
*
* Siflower software
*/

#ifdef CONFIG_DEBUG_FS
#include <linux/debugfs.h>
#endif
#include <sys/ioctl.h>

struct sfax8_debug_info {
    char *debug_buffer;
    void *i_private;
    int buffer_len;
};


long sf_eswitch_debug_ioctl (struct file * fil, unsigned int a, unsigned long b);
int sf_eswitch_debug_open(struct inode *inode, struct file *file);
int sf_eswitch_debug_release(struct inode *inode, struct file *file);
ssize_t sf_eswitch_debug_read(struct file *file, char __user *user_buf,
		size_t count, loff_t *ppos);
ssize_t sf_eswitch_debug_write(struct file *file, const char __user *user_buf,
		size_t count, loff_t *ppos);
#ifdef CONFIG_SFAX8_GENL
int notify_link_event(struct sf_eswitch_priv *pesw_priv, int port, int updown, char *ifname, uint8_t *mac, uint16_t vlan_id, bool flag);
#endif
#endif
