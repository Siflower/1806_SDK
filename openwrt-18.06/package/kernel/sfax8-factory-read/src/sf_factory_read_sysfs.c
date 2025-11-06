#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/debugfs.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

#include <linux/sysfs.h>
#include <linux/device.h>
#include <sfax8_factory_read.h>

#include "sf_factory_read_sysfs.h"
#include "sf_factory_read_pl_ref.h"

/* some macros taken from iwlwifi */
#define DEBUGFS_ADD_FILE(name, parent, mode) do {               \
    if (!debugfs_create_file(#name, mode, parent, fr_ctx,      \
                &sf_factory_read_dbgfs_##name##_ops))           \
    goto err;                                                   \
} while (0)

#define DEBUGFS_ADD_BOOL(name, parent, ptr) do {                \
    struct dentry *__tmp;                                       \
    __tmp = debugfs_create_bool(#name, S_IWUSR | S_IRUSR,       \
            parent, ptr);                                       \
    if (IS_ERR(__tmp) || !__tmp)                                \
    goto err;                                                   \
} while (0)

#define DEBUGFS_ADD_X64(name, parent, ptr) do {                 \
    struct dentry *__tmp;                                       \
    __tmp = debugfs_create_x64(#name, S_IWUSR | S_IRUSR,        \
            parent, ptr);                                       \
    if (IS_ERR(__tmp) || !__tmp)                                \
    goto err;                                                   \
} while (0)

#define DEBUGFS_ADD_U64(name, parent, ptr, mode) do {           \
    struct dentry *__tmp;                                       \
    __tmp = debugfs_create_u64(#name, mode,                     \
            parent, ptr);                                       \
    if (IS_ERR(__tmp) || !__tmp)                                \
    goto err;                                                   \
} while (0)

#define DEBUGFS_ADD_X32(name, parent, ptr) do {                 \
    struct dentry *__tmp;                                       \
    __tmp = debugfs_create_x32(#name, S_IWUSR | S_IRUSR,        \
            parent, ptr);                                       \
    if (IS_ERR(__tmp) || !__tmp)                                \
    goto err;                                                   \
} while (0)

#define DEBUGFS_ADD_U32(name, parent, ptr, mode) do {           \
    struct dentry *__tmp;                                       \
    __tmp = debugfs_create_u32(#name, mode,                     \
            parent, ptr);                                       \
    if (IS_ERR(__tmp) || !__tmp)                                \
    goto err;                                                   \
} while (0)

/* file operation */
#define DEBUGFS_READ_FUNC(name)											   \
    static ssize_t sf_factory_read_dbgfs_##name##_read(struct file *file,  \
            char __user *user_buf,                              \
            size_t count, loff_t *ppos);

#define DEBUGFS_WRITE_FUNC(name)                                \
    static ssize_t sf_factory_read_dbgfs_##name##_write(struct file *file, \
            const char __user *user_buf,                        \
            size_t count, loff_t *ppos);


#define DEBUGFS_READ_FILE_OPS(name)                             \
    DEBUGFS_READ_FUNC(name);                                    \
static const struct file_operations sf_factory_read_dbgfs_##name##_ops = { \
    .read   = sf_factory_read_dbgfs_##name##_read,                         \
    .open   = simple_open,                                      \
    .llseek = generic_file_llseek,                              \
};

#define DEBUGFS_WRITE_FILE_OPS(name)                            \
    DEBUGFS_WRITE_FUNC(name);                                   \
static const struct file_operations sf_factory_read_dbgfs_##name##_ops = { \
    .write  = sf_factory_read_dbgfs_##name##_write,                        \
    .open   = simple_open,                                      \
    .llseek = generic_file_llseek,                              \
};


#define DEBUGFS_READ_WRITE_FILE_OPS(name)                       \
    DEBUGFS_READ_FUNC(name);                                    \
    DEBUGFS_WRITE_FUNC(name);                                   \
static const struct file_operations sf_factory_read_dbgfs_##name##_ops = { \
    .write  = sf_factory_read_dbgfs_##name##_write,                        \
    .read   = sf_factory_read_dbgfs_##name##_read,                         \
    .open   = simple_open,                                      \
    .llseek = generic_file_llseek,                              \
};

#define MACADDR_HDR "macaddress : %2x %2x %2x %2x %2x %2x\n"
#define MACADDR_HDR_MAX_LEN  (sizeof(MACADDR_HDR) + 16)

#define SN_HDR "SN : %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x\n"
#define SN_HDR_MAX_LEN (sizeof(SN_HDR) + 16)

#define SN_FLAG_HDR "SN flag: %2x\n"
#define SN_FLAG_HDR_MAX_LEN (sizeof(SN_FLAG_HDR) + 16)

#define PCBA_BOOT_HDR "PCBA value : %.4s\n"
#define PCBA_BOOT_HDR_MAX_LEN (sizeof(PCBA_BOOT_HDR) + 16)

#define COUNTRYID_HDR "Country ID : %.2s\n"
#define COUNTRYID_HDR_MAX_LEN (sizeof(COUNTRYID_HDR) + 16)

#define HW_VER_HDR "Hardware version : %.32s\n"
#define HW_VER_HDR_MAX_LEN (sizeof(HW_VER_HDR) + 16)

#define HW_FEATURE_HDR "Hardware feature : %#x\n"
#define HW_FEATURE_HDR_MAX_LEN (sizeof(HW_FEATURE_HDR) + 16)

#define EXIST_HDR "Exist flag : %8x\n"
#define EXIST_HDR_MAX_LEN (sizeof(EXIST_HDR) + 16)

extern int get_value_through_mtd(struct device_node *np,const char *name, int start_offset, size_t len, unsigned char *buffer);
extern void handle_macaddr_internal(struct device_node *np,struct sfax8_factory_read_context *priv);
extern int sfax8_set_gmac_delay_mtd(const char *name, unsigned short gmac_delay);
extern int sfax8_set_char_mtd(int id, unsigned char *ctx_buf, unsigned char *str, const size_t len);
extern int sfax8_set_int_mtd(int id, uint32_t *ctx_buf, uint32_t val);

static int fr_ctx_all_ff(unsigned char* fr_ctx_member, int count)
{
	int all_ff = 1;
	int i;

	if (!fr_ctx_member) {
		printk("fr_ctx_member is null!!!\n");
		return 0;
	}
	for (i = 0; i < count; i++) {
		if (fr_ctx_member[i] != 0xff) {
			all_ff = 0;
			break;
		}
	}
	return all_ff;
}

static ssize_t sf_factory_read_dbgfs_stats_read(struct file *file,
		char __user *user_buf,
		size_t count,
		loff_t *ppos)
{
	struct sfax8_factory_read_context *fr_ctx = file->private_data;
	char *buf;
	int res;
	ssize_t read;
	size_t bufsz = (MACADDR_HDR_MAX_LEN + SN_HDR_MAX_LEN +
		SN_FLAG_HDR_MAX_LEN + COUNTRYID_HDR_MAX_LEN +
		EXIST_HDR_MAX_LEN +
		HW_VER_HDR_MAX_LEN + HW_FEATURE_HDR_MAX_LEN);

	/*everything is read out in one go*/
	if (*ppos)
		return 0;
	if (!fr_ctx)
		return 0;

	bufsz = min_t(size_t, bufsz, count);
	buf = kzalloc(bufsz, GFP_ATOMIC);
	if (buf == NULL)
		return 0;

	bufsz--;

	res = scnprintf(buf, bufsz, MACADDR_HDR SN_HDR COUNTRYID_HDR HW_VER_HDR
						    HW_FEATURE_HDR "\n",
			fr_ctx->macaddr[0], fr_ctx->macaddr[1],
			fr_ctx->macaddr[2], fr_ctx->macaddr[3],
			fr_ctx->macaddr[4], fr_ctx->macaddr[5], fr_ctx->sn[0],
			fr_ctx->sn[1], fr_ctx->sn[2], fr_ctx->sn[3],
			fr_ctx->sn[4], fr_ctx->sn[5], fr_ctx->sn[6],
			fr_ctx->sn[7], fr_ctx->sn[8], fr_ctx->sn[9],
			fr_ctx->sn[10], fr_ctx->sn[11], fr_ctx->sn[12],
			fr_ctx->sn[13], fr_ctx->sn[14], fr_ctx->sn[15],
			fr_ctx->countryID, fr_ctx->hw_ver, fr_ctx->hw_feature);

	read = simple_read_from_buffer(user_buf, count, ppos, buf, res);
	kfree(buf);

	return read;
}
DEBUGFS_READ_FILE_OPS(stats);

static ssize_t sf_factory_read_dbgfs_memory_read(struct file *file, char __user *user_buf,
							size_t count, loff_t *ppos)
{
	struct sfax8_factory_read_context *fr_ctx = file->private_data;
	char *buf;
	unsigned char *data;
	int res, i;
	ssize_t read;
	size_t bufsz;

	/*everything is read out in one go*/
	if (*ppos)
		return 0;
	if (!fr_ctx)
		return 0;
	if ((!fr_ctx->start_offset) && (!fr_ctx->len)) {
		printk("do not find start point and length!\n");
		return 0;
	}

	bufsz = 16;

	buf = kzalloc(fr_ctx->len, GFP_ATOMIC);
	if (buf == NULL)
		return 0;

	res = 0;
	data = kzalloc(fr_ctx->len, GFP_ATOMIC);
	if (data == NULL) {
		printk("data is null!\n");
	}

	if (fr_ctx->np == NULL) {
		printk("device node is null!\n");
		return 0;
	}

	get_value_through_mtd(NULL, NULL, fr_ctx->start_offset, fr_ctx->len, data);
	for (i = 0; i < fr_ctx->len; i++) {
		res += scnprintf(&buf[res], min_t(size_t, bufsz - 1, count - res),
						"[%d]:%x\n", i, data[i]);
	}

	read = simple_read_from_buffer(user_buf, count, ppos, buf, res);
	kfree(data);
	kfree(buf);

	return read;
}
DEBUGFS_READ_FILE_OPS(memory);

static ssize_t sf_factory_read_dbgfs_start_len_read(struct file *file, char __user *user_buf,
							size_t count, loff_t *ppos)
{
	struct sfax8_factory_read_context *fr_ctx = file->private_data;
	char *buf;
	int res;
	ssize_t read;
	size_t bufsz = 32;
	/*everything is read out in one go*/
	if (*ppos)
		return 0;
	if (!fr_ctx)
		return 0;

	bufsz = min_t(size_t, bufsz, count);
	buf = kmalloc(bufsz, GFP_ATOMIC);
	if (buf == NULL)
		return 0;

	bufsz--;

	res = scnprintf(buf, bufsz, "start:%d len:%d\n", fr_ctx->start_offset, fr_ctx->len);

	read = simple_read_from_buffer(user_buf, count, ppos, buf, res);
	kfree(buf);

	return read;
}

static ssize_t sf_factory_read_dbgfs_start_len_write(struct file *file, const char __user *user_buf,
							size_t count, loff_t *ppos)
{
	struct sfax8_factory_read_context *fr_ctx = file->private_data;

	char buf[32];
	int val, length;
	size_t len = min_t(size_t, count, sizeof(buf) - 1);

	if (copy_from_user(buf, user_buf, len))
		return -EFAULT;
	if (!fr_ctx)
		return -EFAULT;

	buf[len] = '\0';

	if (sscanf(buf, "%d %d", &val, &length)) {
		printk("%d %d\n", val, length);
		fr_ctx->start_offset = val;
		fr_ctx->len = length;
	} else {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	return count;
}
DEBUGFS_READ_WRITE_FILE_OPS(start_len);

static ssize_t sf_factory_read_countryid_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	return sprintf(buf, "%c%c\n",fr_ctx->countryID[0], fr_ctx->countryID[1]);
}

static ssize_t sf_factory_read_countryid_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char ID[COUNTRYID_SIZE];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(ID, 0x0, COUNTRYID_SIZE);
	ret = sscanf(buf, "%c%c", &ID[0], &ID[1]);
	if (ret < COUNTRYID_SIZE) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	if (ID[0] >= 'A' && ID[0] <= 'Z' && \
	    ID[1] >= 'A' && ID[1] <= 'Z') {
		sfax8_set_char_mtd(READ_COUNTRY_ID, fr_ctx->countryID, ID, COUNTRYID_SIZE);
	} else {
		printk("invalid country ID!\n");
		return -EFAULT;
	}

	return count;
}

static ssize_t sf_factory_read_macaddr_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x\n",fr_ctx->macaddr[0], fr_ctx->macaddr[1], fr_ctx->macaddr[2],\
										fr_ctx->macaddr[3],fr_ctx->macaddr[4], fr_ctx->macaddr[5]);
}

static ssize_t sf_factory_read_macaddr_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char addr[MACADDR_SIZE];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(addr, 0x0, MACADDR_SIZE);
	ret = sscanf(buf, "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx", &addr[0], &addr[1], &addr[2], \
								&addr[3], &addr[4], &addr[5]);
	if (ret < MACADDR_SIZE) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	sfax8_set_char_mtd(READ_MAC_ADDRESS, fr_ctx->macaddr, addr, MACADDR_SIZE);
	handle_macaddr_internal(fr_ctx->np, fr_ctx);
	return count;
}

static ssize_t sf_factory_read_macaddr0_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x\n",fr_ctx->macaddr0[0], fr_ctx->macaddr0[1], fr_ctx->macaddr0[2],\
										fr_ctx->macaddr0[3],fr_ctx->macaddr0[4], fr_ctx->macaddr0[5]);
}

static ssize_t sf_factory_read_macaddr0_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char addr[MACADDR_SIZE];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(addr, 0x0, MACADDR_SIZE);
	ret = sscanf(buf, "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx", &addr[0], &addr[1], &addr[2], \
								&addr[3], &addr[4], &addr[5]);
	if (ret < MACADDR_SIZE) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	sfax8_set_char_mtd(READ_MAC_ADDRESS, fr_ctx->macaddr0, addr, MACADDR_SIZE);
	handle_macaddr_internal(fr_ctx->np, fr_ctx);
	return count;
}

static ssize_t sf_factory_read_macaddr_wan_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x\n",fr_ctx->wan_macaddr[0], fr_ctx->wan_macaddr[1], fr_ctx->wan_macaddr[2],\
										fr_ctx->wan_macaddr[3],fr_ctx->wan_macaddr[4], fr_ctx->wan_macaddr[5]);
}

#ifdef CONFIG_SFAX8_HNAT_MULTI_WAN
static ssize_t sf_factory_read_macaddr_wwan_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n",fr_ctx->wwan_macaddr[0], fr_ctx->wwan_macaddr[1], fr_ctx->wwan_macaddr[2],\
										fr_ctx->wwan_macaddr[3],fr_ctx->wwan_macaddr[4], fr_ctx->wwan_macaddr[5]);
}

static ssize_t sf_factory_read_macaddr_lan2_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n",fr_ctx->lan2_macaddr[0], fr_ctx->lan2_macaddr[1], fr_ctx->lan2_macaddr[2],\
										fr_ctx->lan2_macaddr[3],fr_ctx->lan2_macaddr[4], fr_ctx->lan2_macaddr[5]);
}
#endif

static ssize_t sf_factory_read_macaddr_wan_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char addr[MACADDR_SIZE];
	int ret = 0, i = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(addr, 0x0, MACADDR_SIZE);
	ret = sscanf(buf, "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx", &addr[0], &addr[1], &addr[2], \
								&addr[3], &addr[4], &addr[5]);
	if (ret < MACADDR_SIZE) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	for (i = 0; i < MACADDR_SIZE; i++) {
		fr_ctx->wan_macaddr[i] = addr[i];
		printk("0x%02x\n", addr[i]);
	}

	handle_macaddr_internal(fr_ctx->np, fr_ctx);
	return count;
}

static ssize_t sf_factory_read_macaddr_lan_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x\n",fr_ctx->lan_macaddr[0], fr_ctx->lan_macaddr[1], fr_ctx->lan_macaddr[2],\
										fr_ctx->lan_macaddr[3],fr_ctx->lan_macaddr[4], fr_ctx->lan_macaddr[5]);
}

static ssize_t sf_factory_read_macaddr_lan_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char addr[MACADDR_SIZE];
	int ret = 0, i = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(addr, 0x0, MACADDR_SIZE);
	ret = sscanf(buf, "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx", &addr[0], &addr[1], &addr[2], \
								&addr[3], &addr[4], &addr[5]);
	if (ret < MACADDR_SIZE) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	for (i = 0; i < MACADDR_SIZE; i++) {
		fr_ctx->lan_macaddr[i] = addr[i];
		printk("0x%02x\n", addr[i]);
	}

	handle_macaddr_internal(fr_ctx->np, fr_ctx);
	return count;
}

static ssize_t sf_factory_read_macaddr_lb_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x\n",fr_ctx->wifi_lb_macaddr[0], fr_ctx->wifi_lb_macaddr[1], fr_ctx->wifi_lb_macaddr[2],\
										fr_ctx->wifi_lb_macaddr[3],fr_ctx->wifi_lb_macaddr[4], fr_ctx->wifi_lb_macaddr[5]);
}

static ssize_t sf_factory_read_macaddr_lb_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char addr[MACADDR_SIZE];
	int ret = 0, i = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(addr, 0x0, MACADDR_SIZE);
	ret = sscanf(buf, "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx", &addr[0], &addr[1], &addr[2], \
								&addr[3], &addr[4], &addr[5]);
	if (ret < MACADDR_SIZE) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	for (i = 0; i < MACADDR_SIZE; i++) {
		fr_ctx->wifi_lb_macaddr[i] = addr[i];
		printk("0x%02x\n", addr[i]);
	}

	handle_macaddr_internal(fr_ctx->np, fr_ctx);
	return count;
}

static ssize_t sf_factory_read_macaddr_hb_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x\n",fr_ctx->wifi_hb_macaddr[0], fr_ctx->wifi_hb_macaddr[1], fr_ctx->wifi_hb_macaddr[2],\
										fr_ctx->wifi_hb_macaddr[3],fr_ctx->wifi_hb_macaddr[4], fr_ctx->wifi_hb_macaddr[5]);
}

static ssize_t sf_factory_read_macaddr_hb_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char addr[MACADDR_SIZE];
	int ret = 0, i = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(addr, 0x0, MACADDR_SIZE);
	ret = sscanf(buf, "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx", &addr[0], &addr[1], &addr[2], \
								&addr[3], &addr[4], &addr[5]);
	if (ret < MACADDR_SIZE) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	for (i = 0; i < MACADDR_SIZE; i++) {
		fr_ctx->wifi_hb_macaddr[i] = addr[i];
		printk("0x%02x\n", addr[i]);
	}

	handle_macaddr_internal(fr_ctx->np, fr_ctx);
	return count;
}

static ssize_t sf_factory_read_sn_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	if (!fr_ctx_all_ff(fr_ctx->sn, 16)) {
		return sprintf(buf, "%.16s\n", fr_ctx->sn);
	} else {
		return sprintf(buf, "\n");
	}
}

static ssize_t sf_factory_read_sn_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char sn[SN_SIZE + 1];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(sn, 0x0, SN_SIZE + 1);
	ret = sscanf(buf, "%16s", sn);
	if (ret < 1) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	sfax8_set_char_mtd(READ_SN, fr_ctx->sn, sn, SN_SIZE);

	return count;
}

static ssize_t sf_factory_read_sn_flag_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "0x%02x\n",fr_ctx->sn_flag);
}

static ssize_t sf_factory_read_sn_flag_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char flag = 0x0;
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	ret = sscanf(buf, "0x%2hhx", &flag);
	if (ret < 1) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	sfax8_set_char_mtd(READ_SN_FLAG, &fr_ctx->sn_flag, &flag, 1);

	return count;
}

static ssize_t sf_factory_read_hw_ver_flag_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	if (!fr_ctx_all_ff(fr_ctx->hw_ver_flag, 2)) {
		return sprintf(buf, "%.2s\n", fr_ctx->hw_ver_flag);
	} else {
		return sprintf(buf, "\n");
	}
}

static ssize_t sf_factory_read_hw_ver_flag_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char flag[HARDWARE_VER_FLAG_SIZE];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(flag, 0x0, HARDWARE_VER_FLAG_SIZE);
	ret = sscanf(buf, "%c%c", &flag[0], &flag[1]);
	if (ret < HARDWARE_VER_FLAG_SIZE) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	if (fr_ctx->hw_ver[0] != 0xff && flag[0] == 'h' && flag[1] == 'v') {
		sfax8_set_char_mtd(READ_HARDWARE_VER_FLAG, fr_ctx->hw_ver_flag, flag, HARDWARE_VER_FLAG_SIZE);
	} else {
		printk("only when hw_ver is valid can fill in \"hv\"!\n");
		return -EFAULT;
	}

	return count;
}

static ssize_t sf_factory_read_hw_ver_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	if(!fr_ctx_all_ff(fr_ctx->hw_ver, 32)) {
		return sprintf(buf, "%.32s\n", fr_ctx->hw_ver);
	} else {
		return sprintf(buf, "\n");
	}
}

static ssize_t sf_factory_read_hw_ver_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char ver[HARDWARE_VER_SIZE + 1];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(ver, 0x0, HARDWARE_VER_SIZE + 1);
	ret = sscanf(buf, "%32s", ver);
	if (ret < 1) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	sfax8_set_char_mtd(READ_HARDWARE_VER, fr_ctx->hw_ver, ver, HARDWARE_VER_SIZE);

	return count;
}

static ssize_t sf_factory_read_model_ver_flag_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	if(!fr_ctx_all_ff(fr_ctx->model_ver_flag, 2)) {
		return sprintf(buf, "%.2s\n", fr_ctx->model_ver_flag);
	} else {
		return sprintf(buf, "\n");
	}
}

static ssize_t sf_factory_read_model_ver_flag_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char flag[MODEL_VER_FLAG_SIZE];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(flag, 0x0, MODEL_VER_FLAG_SIZE);
	ret = sscanf(buf, "%c%c", &flag[0], &flag[1]);
	if (ret < MODEL_VER_FLAG_SIZE) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	if (fr_ctx->model_ver[0] != 0xff && flag[0] == 'm' && flag[1] == 'v') {
		sfax8_set_char_mtd(READ_MODEL_VER_FLAG, fr_ctx->model_ver_flag, flag, MODEL_VER_FLAG_SIZE);
	} else {
		printk("only when model_ver is valid can fill in \"mv\"!\n");
		return -EFAULT;
	}

	return count;
}

static ssize_t sf_factory_read_model_ver_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	if(!fr_ctx_all_ff(fr_ctx->model_ver, 32)) {
		return sprintf(buf, "%.32s\n", fr_ctx->model_ver);
	} else {
		return sprintf(buf, "\n");
	}
}

static ssize_t sf_factory_read_model_ver_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char ver[MODEL_VER_SIZE + 1];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(ver, 0x0, MODEL_VER_SIZE + 1);
	ret = sscanf(buf, "%32s", ver);
	if (ret < 1) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	sfax8_set_char_mtd(READ_MODEL_VER, fr_ctx->model_ver, ver, MODEL_VER_SIZE);
	return count;
}

static ssize_t sf_factory_read_vender_flag_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	if(!fr_ctx_all_ff(fr_ctx->vender_flag, 2)) {
		return sprintf(buf, "%.2s\n", fr_ctx->vender_flag);
	} else {
		return sprintf(buf, "\n");
	}
}

static ssize_t sf_factory_read_vender_flag_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char flag[VENDER_FLAG_SIZE];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(flag, 0x0, VENDER_FLAG_SIZE);
	ret = sscanf(buf, "%c%c", &flag[0], &flag[1]);
	if (ret < VENDER_FLAG_SIZE) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	if (fr_ctx->vender[0] != 0xff && flag[0] == 'v' && flag[1] == 'd') {
		sfax8_set_char_mtd(READ_VENDER_FLAG, fr_ctx->vender_flag, flag, VENDER_FLAG_SIZE);
	} else {
		printk("only when vender is valid can fill in \"vd\"!\n");
		return -EFAULT;
	}

	return count;
}

static ssize_t sf_factory_read_vender_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	if(!fr_ctx_all_ff(fr_ctx->vender, 16)) {
		return sprintf(buf, "%.16s\n", fr_ctx->vender);
	} else {
		return sprintf(buf, "\n");
	}
}

static ssize_t sf_factory_read_vender_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char vender[VENDER_SIZE + 1];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(vender, 0x0, VENDER_SIZE + 1);
	ret = sscanf(buf, "%16s", vender);
	if (ret < 1) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	sfax8_set_char_mtd(READ_VENDER, fr_ctx->vender, vender, VENDER_SIZE);

	return count;
}

static ssize_t sf_factory_read_product_key_flag_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	if(!fr_ctx_all_ff(fr_ctx->product_key_flag, 2)) {
		return sprintf(buf, "%.2s\n", fr_ctx->product_key_flag);
	} else {
		return sprintf(buf, "\n");
	}
}

static ssize_t sf_factory_read_product_key_flag_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char flag[PRODUCT_KEY_FLAG_SIZE];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(flag, 0x0, PRODUCT_KEY_FLAG_SIZE);
	ret = sscanf(buf, "%c%c", &flag[0], &flag[1]);
	if (ret < PRODUCT_KEY_FLAG_SIZE) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	if (fr_ctx->product_key[0] != 0xff && flag[0] == 'p' && flag[1] == 'k') {
		sfax8_set_char_mtd(READ_PRODUCT_KEY_FLAG, fr_ctx->product_key_flag, flag, PRODUCT_KEY_FLAG_SIZE);
	} else {
		printk("only when product_key is valid can fill in \"pk\"!\n");
		return -EFAULT;
	}

	return count;
}

static ssize_t sf_factory_read_login_info_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "0x%x\n", fr_ctx->login_info);
}

static ssize_t sf_factory_read_login_info_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	uint32_t info = 0x0;
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	ret = sscanf(buf, "0x%x", &info);
	if (ret < 1) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	sfax8_set_int_mtd(READ_LOGIN_INFO, &fr_ctx->login_info, info);

	return count;
}

static ssize_t sf_factory_read_login_info_flag_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	if(!fr_ctx_all_ff(fr_ctx->login_info_flag, 2)) {
		return sprintf(buf, "%.2s\n", fr_ctx->login_info_flag);
	} else {
		return sprintf(buf, "\n");
	}
}

static ssize_t sf_factory_read_login_info_flag_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char flag[LOGIN_INFO_FLAG_SIZE];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(flag, 0x0, LOGIN_INFO_FLAG_SIZE);
	ret = sscanf(buf, "%c%c", &flag[0], &flag[1]);
	if (ret < LOGIN_INFO_FLAG_SIZE) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	if (flag[0] == 'l' && flag[1] == 'i') {
		sfax8_set_char_mtd(READ_LOGIN_INFO_FLAG, fr_ctx->login_info_flag, flag, LOGIN_INFO_FLAG_SIZE);
	} else {
		printk("only when login_info is valid can fill in \"li\"!\n");
		return -EFAULT;
	}

	return count;
}

static ssize_t sf_factory_read_rom_type_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "0x%x\n", fr_ctx->rom_type);
}

static ssize_t sf_factory_read_rom_type_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	uint32_t type = 0x0;
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	ret = sscanf(buf, "0x%x", &type);
	if (ret < 1) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	sfax8_set_int_mtd(READ_ROM_TYPE, &fr_ctx->rom_type, type);

	return count;
}

static ssize_t sf_factory_read_rom_type_flag_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	if(!fr_ctx_all_ff(fr_ctx->rom_type_flag, 2)) {
		return sprintf(buf, "%.2s\n", fr_ctx->rom_type_flag);
	} else {
		return sprintf(buf, "\n");
	}
}

static ssize_t sf_factory_read_rom_type_flag_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char flag[ROM_TYPE_FLAG_SIZE];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(flag, 0x0, ROM_TYPE_FLAG_SIZE);
	ret = sscanf(buf, "%c%c", &flag[0], &flag[1]);
	if (ret < ROM_TYPE_FLAG_SIZE) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	if (fr_ctx->rom_type != 0xffffffff && flag[0] == 'r' && flag[1] == 't') {
		sfax8_set_char_mtd(READ_ROM_TYPE_FLAG, fr_ctx->rom_type_flag, flag, ROM_TYPE_FLAG_SIZE);
	} else {
		printk("only when rom_type is valid can fill in \"rt\"!\n");
		return -EFAULT;
	}

	return count;
}

static ssize_t sf_factory_read_gmac_delay_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "0x%.2s\n0x%.2s\n", &fr_ctx->gmac_delay[0], &fr_ctx->gmac_delay[2]);
}

static ssize_t sf_factory_read_gmac_delay_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned short tx_delay = 0, rx_delay = 0;
	int ret = 0;

	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	ret = sscanf(buf, "%2hx%2hx", &tx_delay, &rx_delay);
	if (ret < 2) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	sfax8_set_gmac_delay_mtd("mtd-gmac-delay", (tx_delay << 8) | rx_delay);

	return count;
}

static ssize_t sf_factory_read_product_key_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	if(!fr_ctx_all_ff(fr_ctx->product_key, 32)) {
		return sprintf(buf, "%.32s\n", fr_ctx->product_key);
	} else {
		return sprintf(buf, "\n");
	}
}

static ssize_t sf_factory_read_product_key_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char key[PRODUCT_KEY_SIZE + 1];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(key, 0x0, PRODUCT_KEY_SIZE + 1);
	ret = sscanf(buf, "%32s", key);
	if (ret < 1) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	sfax8_set_char_mtd(READ_PRODUCT_KEY, fr_ctx->product_key, key, PRODUCT_KEY_SIZE);

	return count;
}

static ssize_t sf_factory_read_hw_feature_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "0x%x\n", fr_ctx->hw_feature);
}

static ssize_t sf_factory_read_hw_feature_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	uint32_t feature = 0x0;
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	ret = sscanf(buf, "0x%x", &feature);
	if (ret < 1) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	sfax8_set_int_mtd(READ_HW_FEATURE, &fr_ctx->hw_feature, feature);

	return count;
}

static ssize_t sf_factory_read_default_ssid_lb_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	if(!fr_ctx_all_ff(fr_ctx->default_ssid_lb, 32)) {
		return sprintf(buf, "%.32s\n",fr_ctx->default_ssid_lb);
	} else {
		return sprintf(buf, "\n");
	}
}

static ssize_t sf_factory_read_default_ssid_lb_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char ssid[DEFAULT_SSID_LB_SIZE + 1];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(ssid, 0x0, DEFAULT_SSID_LB_SIZE + 1);
	ret = sscanf(buf, "%32s", ssid);
	if (ret < 1) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	sfax8_set_char_mtd(READ_DEFAULT_SSID_LB, fr_ctx->default_ssid_lb, ssid, DEFAULT_SSID_LB_SIZE);

	return count;
}

static ssize_t sf_factory_read_default_ssid_hb_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	if(!fr_ctx_all_ff(fr_ctx->default_ssid_hb, 32)) {
		return sprintf(buf, "%.32s\n",fr_ctx->default_ssid_hb);
	} else {
		return sprintf(buf, "\n");
	}
}

static ssize_t sf_factory_read_default_ssid_hb_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char ssid[DEFAULT_SSID_HB_SIZE + 1];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(ssid, 0x0, DEFAULT_SSID_HB_SIZE + 1);
	ret = sscanf(buf, "%32s", ssid);
	if (ret < 1) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	sfax8_set_char_mtd(READ_DEFAULT_SSID_HB, fr_ctx->default_ssid_hb, ssid, DEFAULT_SSID_HB_SIZE);

	return count;
}

static ssize_t sf_factory_read_default_key_lb_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	if(!fr_ctx_all_ff(fr_ctx->default_key_lb, 16)) {
		return sprintf(buf, "%.16s\n",fr_ctx->default_key_lb);
	} else {
		return sprintf(buf, "\n");
	}
}

static ssize_t sf_factory_read_default_key_lb_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char key[DEFAULT_KEY_LB_SIZE + 1];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(key, 0x0, DEFAULT_KEY_LB_SIZE + 1);
	ret = sscanf(buf, "%16s", key);
	if (ret < 1) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	sfax8_set_char_mtd(READ_DEFAULT_KEY_LB, fr_ctx->default_key_lb, key, DEFAULT_KEY_LB_SIZE);

	return count;
}

static ssize_t sf_factory_read_default_key_hb_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	if(!fr_ctx_all_ff(fr_ctx->default_key_hb, 16)) {
		return sprintf(buf, "%.16s\n",fr_ctx->default_key_hb);
	} else {
		return sprintf(buf, "\n");
	}
}

static ssize_t sf_factory_read_default_key_hb_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	unsigned char key[DEFAULT_KEY_HB_SIZE + 1];
	int ret = 0;
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	memset(key, 0x0, DEFAULT_KEY_HB_SIZE + 1);
	ret = sscanf(buf, "%16s", key);
	if (ret < 1) {
		printk("can not sscanf the buf!\n");
		return -EFAULT;
	}

	sfax8_set_char_mtd(READ_DEFAULT_KEY_HB, fr_ctx->default_key_hb, key, DEFAULT_KEY_HB_SIZE);

	return count;
}

static DEVICE_ATTR(countryid,        S_IRUSR | S_IWUSR, sf_factory_read_countryid_show, sf_factory_read_countryid_store);
static DEVICE_ATTR(macaddr,          S_IRUSR | S_IWUSR, sf_factory_read_macaddr_show, sf_factory_read_macaddr_store);
static DEVICE_ATTR(macaddr0,         S_IRUSR | S_IWUSR, sf_factory_read_macaddr0_show, sf_factory_read_macaddr0_store);
static DEVICE_ATTR(macaddr_wan,      S_IRUSR | S_IWUSR, sf_factory_read_macaddr_wan_show, sf_factory_read_macaddr_wan_store);
static DEVICE_ATTR(macaddr_lan,      S_IRUSR | S_IWUSR, sf_factory_read_macaddr_lan_show, sf_factory_read_macaddr_lan_store);
static DEVICE_ATTR(macaddr_lb,       S_IRUSR | S_IWUSR, sf_factory_read_macaddr_lb_show, sf_factory_read_macaddr_lb_store);
static DEVICE_ATTR(macaddr_hb,       S_IRUSR | S_IWUSR, sf_factory_read_macaddr_hb_show, sf_factory_read_macaddr_hb_store);
static DEVICE_ATTR(sn,               S_IRUSR | S_IWUSR, sf_factory_read_sn_show, sf_factory_read_sn_store);
static DEVICE_ATTR(sn_flag,          S_IRUSR | S_IWUSR, sf_factory_read_sn_flag_show, sf_factory_read_sn_flag_store);
static DEVICE_ATTR(hw_ver,           S_IRUSR | S_IWUSR, sf_factory_read_hw_ver_show, sf_factory_read_hw_ver_store);
static DEVICE_ATTR(hw_ver_flag,      S_IRUSR | S_IWUSR, sf_factory_read_hw_ver_flag_show, sf_factory_read_hw_ver_flag_store);
static DEVICE_ATTR(model_ver,        S_IRUSR | S_IWUSR, sf_factory_read_model_ver_show, sf_factory_read_model_ver_store);
static DEVICE_ATTR(model_ver_flag,   S_IRUSR | S_IWUSR, sf_factory_read_model_ver_flag_show, sf_factory_read_model_ver_flag_store);
static DEVICE_ATTR(vender,           S_IRUSR | S_IWUSR, sf_factory_read_vender_show, sf_factory_read_vender_store);
static DEVICE_ATTR(vender_flag,      S_IRUSR | S_IWUSR, sf_factory_read_vender_flag_show, sf_factory_read_vender_flag_store);
static DEVICE_ATTR(product_key,      S_IRUSR | S_IWUSR, sf_factory_read_product_key_show, sf_factory_read_product_key_store);
static DEVICE_ATTR(product_key_flag, S_IRUSR | S_IWUSR, sf_factory_read_product_key_flag_show, sf_factory_read_product_key_flag_store);
static DEVICE_ATTR(login_info,       S_IRUSR | S_IWUSR, sf_factory_read_login_info_show, sf_factory_read_login_info_store);
static DEVICE_ATTR(login_info_flag,  S_IRUSR | S_IWUSR, sf_factory_read_login_info_flag_show, sf_factory_read_login_info_flag_store);
static DEVICE_ATTR(rom_type,         S_IRUSR | S_IWUSR, sf_factory_read_rom_type_show, sf_factory_read_rom_type_store);
static DEVICE_ATTR(rom_type_flag,    S_IRUSR | S_IWUSR, sf_factory_read_rom_type_flag_show, sf_factory_read_rom_type_flag_store);
static DEVICE_ATTR(gmac_delay,       S_IRUSR | S_IWUSR, sf_factory_read_gmac_delay_show, sf_factory_read_gmac_delay_store);
static DEVICE_ATTR(hw_feature,       S_IRUSR | S_IWUSR, sf_factory_read_hw_feature_show, sf_factory_read_hw_feature_store);
static DEVICE_ATTR(default_ssid_lb,  S_IRUSR | S_IWUSR, sf_factory_read_default_ssid_lb_show, sf_factory_read_default_ssid_lb_store);
static DEVICE_ATTR(default_ssid_hb,  S_IRUSR | S_IWUSR, sf_factory_read_default_ssid_hb_show, sf_factory_read_default_ssid_hb_store);
static DEVICE_ATTR(default_key_lb,   S_IRUSR | S_IWUSR, sf_factory_read_default_key_lb_show, sf_factory_read_default_key_lb_store);
static DEVICE_ATTR(default_key_hb,   S_IRUSR | S_IWUSR, sf_factory_read_default_key_hb_show, sf_factory_read_default_key_hb_store);

static struct attribute *factory_read_attr[] = {
	&dev_attr_countryid.attr,
	&dev_attr_macaddr.attr,
	&dev_attr_macaddr_wan.attr,
	&dev_attr_macaddr_lan.attr,
#ifdef CONFIG_SFAX8_HNAT_MULTI_WAN
	&dev_attr_macaddr_wwan.attr,
	&dev_attr_macaddr_lan2.attr,
#endif
	&dev_attr_macaddr_lb.attr,
	&dev_attr_macaddr_hb.attr,
	&dev_attr_macaddr0.attr,
	&dev_attr_sn.attr,
	&dev_attr_sn_flag.attr,
	&dev_attr_hw_ver_flag.attr,
	&dev_attr_hw_ver.attr,
	&dev_attr_model_ver_flag.attr,
	&dev_attr_model_ver.attr,
	&dev_attr_hw_feature.attr,
	&dev_attr_vender_flag.attr,
	&dev_attr_vender.attr,
	&dev_attr_product_key_flag.attr,
	&dev_attr_product_key.attr,
	&dev_attr_login_info_flag.attr,
	&dev_attr_login_info.attr,
	&dev_attr_rom_type_flag.attr,
	&dev_attr_rom_type.attr,
	&dev_attr_gmac_delay.attr,
	&dev_attr_default_ssid_lb.attr,
	&dev_attr_default_ssid_hb.attr,
	&dev_attr_default_key_lb.attr,
	&dev_attr_default_key_hb.attr,
	NULL,
};

static const struct attribute_group factory_read_attribute_group = {
	.attrs = factory_read_attr,
};

int sf_factory_read_sysfs_register(struct platform_device *pdev, char *parent)
{
	struct sfax8_factory_read_context *fr_ctx;
	struct dentry *dir_drv;

	fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(pdev);
	// 1.register debugfs
	printk("%s, parent :%s\n", __func__,
			(parent == NULL) ? "NULL" : parent);
	if (!parent)
		dir_drv = debugfs_create_dir("sfax8_factory_read", NULL);
	else
		dir_drv = debugfs_create_dir(parent, NULL);
	if (!dir_drv) {
		printk("debug fs create directory failed!\n");
		goto err;
	}
	fr_ctx->debugfs = dir_drv;

	DEBUGFS_ADD_FILE(stats, dir_drv, S_IRUSR);
	DEBUGFS_ADD_FILE(memory, dir_drv, S_IRUSR);
	DEBUGFS_ADD_FILE(start_len, dir_drv, S_IWUSR | S_IRUSR);
	// 2.register sysfs
	return sysfs_create_group(
			&(pdev->dev.kobj), &factory_read_attribute_group);

err:
	if (dir_drv)
		debugfs_remove_recursive(dir_drv);
	fr_ctx->debugfs = NULL;
	return -1;
}

int sf_factory_read_sysfs_unregister(struct platform_device *pdev)
{
	struct sfax8_factory_read_context *fr_ctx =
			(struct sfax8_factory_read_context *)
					platform_get_drvdata(pdev);
	if (fr_ctx == NULL) {
		printk("invalid platform driver private data!\n");
		return -1;
	}
	if (!fr_ctx->debugfs) {
		printk("already removed!\n");
		return -2;
	}
	debugfs_remove_recursive(fr_ctx->debugfs);

	sysfs_remove_group(&(pdev->dev.kobj), &factory_read_attribute_group);
	return 0;
}
