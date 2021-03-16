#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/debugfs.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

#include <linux/sysfs.h>
#include <linux/device.h>
#include <linux/sfax8_factory_read.h>

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

static ssize_t sf_factory_read_dbgfs_memory_read(struct file *file,
        char __user *user_buf,
        size_t count,
        loff_t *ppos)
{
    struct sfax8_factory_read_context *fr_ctx = file->private_data;
    char *buf;
	unsigned char *data;
    int res, i;
    ssize_t read;
    size_t bufsz;

    /*everything is read out in one go*/
    if(*ppos)
        return 0;
    if(!fr_ctx)
        return 0;
	if((!fr_ctx->start_offset) && (!fr_ctx->len)){
		printk("do not find start point and length!\n");
		return 0;
	}

	bufsz = 16;

    buf = kzalloc(fr_ctx->len, GFP_ATOMIC);
    if(buf == NULL)
        return 0;

    res = 0;
	data = kzalloc(fr_ctx->len, GFP_ATOMIC);
	if(data == NULL){
		printk("data is null!\n");
	}

	if(fr_ctx->np == NULL){
		printk("device node is null!\n");
		return 0;
	}

	get_value_through_mtd(NULL, NULL, fr_ctx->start_offset, fr_ctx->len, data);
	for(i = 0; i < fr_ctx->len; i++){
		res += scnprintf(&buf[res], min_t(size_t, bufsz - 1, count - res),
						"[%d]:%x\n", i, data[i]);
    }

    read = simple_read_from_buffer(user_buf, count, ppos, buf, res);
    kfree(data);
	kfree(buf);

    return read;
}
DEBUGFS_READ_FILE_OPS(memory);

static ssize_t sf_factory_read_dbgfs_start_len_read(struct file *file,
        char __user *user_buf,
        size_t count,
        loff_t *ppos)
{
    struct sfax8_factory_read_context *fr_ctx = file->private_data;
    char *buf;
    int res;
    ssize_t read;
    size_t bufsz = 32;
    /*everything is read out in one go*/
    if(*ppos)
        return 0;
    if(!fr_ctx)
        return 0;

    bufsz = min_t(size_t, bufsz, count);
    buf = kmalloc(bufsz, GFP_ATOMIC);
    if(buf == NULL)
        return 0;

    bufsz--;

    res = scnprintf(buf, bufsz, "start:%d len:%d\n", fr_ctx->start_offset, fr_ctx->len);

    read = simple_read_from_buffer(user_buf, count, ppos, buf, res);
    kfree(buf);

    return read;
}

static ssize_t sf_factory_read_dbgfs_start_len_write(struct file *file,
                                        const char __user *user_buf,
                                        size_t count, loff_t *ppos)
{
    struct sfax8_factory_read_context *fr_ctx = file->private_data;

    char buf[32];
    int val,length;
    size_t len = min_t(size_t, count, sizeof(buf) - 1);

    if (copy_from_user(buf, user_buf, len))
        return -EFAULT;
    if(!fr_ctx)
        return -EFAULT;

    buf[len] = '\0';

    if (sscanf(buf, "%d %d", &val, &length)){
        printk("%d %d\n",val,length);
		fr_ctx->start_offset = val;
		fr_ctx->len = length;
    }else{
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

static ssize_t sf_factory_read_macaddr_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n",fr_ctx->macaddr[0], fr_ctx->macaddr[1], fr_ctx->macaddr[2],\
										fr_ctx->macaddr[3],fr_ctx->macaddr[4], fr_ctx->macaddr[5]);
}

static ssize_t sf_factory_read_macaddr0_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n",fr_ctx->macaddr0[0], fr_ctx->macaddr0[1], fr_ctx->macaddr0[2],\
										fr_ctx->macaddr0[3],fr_ctx->macaddr0[4], fr_ctx->macaddr0[5]);
}

static ssize_t sf_factory_read_macaddr_wan_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n",fr_ctx->wan_macaddr[0], fr_ctx->wan_macaddr[1], fr_ctx->wan_macaddr[2],\
										fr_ctx->wan_macaddr[3],fr_ctx->wan_macaddr[4], fr_ctx->wan_macaddr[5]);
}

static ssize_t sf_factory_read_macaddr_lb_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n",fr_ctx->wifi_lb_macaddr[0], fr_ctx->wifi_lb_macaddr[1], fr_ctx->wifi_lb_macaddr[2],\
										fr_ctx->wifi_lb_macaddr[3],fr_ctx->wifi_lb_macaddr[4], fr_ctx->wifi_lb_macaddr[5]);
}

static ssize_t sf_factory_read_macaddr_hb_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n",fr_ctx->wifi_hb_macaddr[0], fr_ctx->wifi_hb_macaddr[1], fr_ctx->wifi_hb_macaddr[2],\
										fr_ctx->wifi_hb_macaddr[3],fr_ctx->wifi_hb_macaddr[4], fr_ctx->wifi_hb_macaddr[5]);
}

static ssize_t sf_factory_read_sn_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x\n",\
						fr_ctx->sn[0], fr_ctx->sn[1], fr_ctx->sn[2],fr_ctx->sn[3],\
						fr_ctx->sn[4], fr_ctx->sn[5], fr_ctx->sn[6], fr_ctx->sn[7],\
						fr_ctx->sn[8], fr_ctx->sn[9],fr_ctx->sn[10], fr_ctx->sn[11],\
						fr_ctx->sn[12], fr_ctx->sn[13],fr_ctx->sn[14], fr_ctx->sn[15]);
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

static ssize_t sf_factory_read_hw_ver_flag_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}

	return sprintf(buf, "%.2s\n", fr_ctx->hw_ver_flag);
}

static ssize_t sf_factory_read_hw_ver_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%.32s\n", fr_ctx->hw_ver);
}

static ssize_t sf_factory_read_model_ver_flag_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%.2s\n", fr_ctx->model_ver_flag);
}

static ssize_t sf_factory_read_model_ver_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%.32s\n", fr_ctx->model_ver);
}

static ssize_t sf_factory_read_vender_flag_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%.2s\n", fr_ctx->vender_flag);
}

static ssize_t sf_factory_read_vender_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%.16s\n", fr_ctx->vender);
}

static ssize_t sf_factory_read_product_key_flag_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%.2s\n", fr_ctx->product_key_flag);
}

static ssize_t sf_factory_read_login_info_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%#x\n", fr_ctx->login_info);
}

static ssize_t sf_factory_read_login_info_flag_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%.2s\n", fr_ctx->login_info_flag);
}

static ssize_t sf_factory_read_rom_type_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%#x\n", fr_ctx->rom_type);
}

static ssize_t sf_factory_read_rom_type_flag_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%.2s\n", fr_ctx->rom_type_flag);
}

static ssize_t sf_factory_read_product_key_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%.32s\n", fr_ctx->product_key);
}
static ssize_t sf_factory_read_hw_feature_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sfax8_factory_read_context *fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(to_platform_device(dev));
	if (!fr_ctx) {
		printk("fr_ctx is null!!!\n");
		return 0;
	}
	return sprintf(buf, "%#x\n",fr_ctx->hw_feature);
}

static DEVICE_ATTR(countryid, S_IRUSR, sf_factory_read_countryid_show, NULL);
static DEVICE_ATTR(macaddr, S_IRUSR, sf_factory_read_macaddr_show, NULL);
static DEVICE_ATTR(macaddr_wan, S_IRUSR, sf_factory_read_macaddr_wan_show, NULL);
static DEVICE_ATTR(macaddr_lb, S_IRUSR, sf_factory_read_macaddr_lb_show, NULL);
static DEVICE_ATTR(macaddr_hb, S_IRUSR, sf_factory_read_macaddr_hb_show, NULL);
static DEVICE_ATTR(macaddr0, S_IRUSR, sf_factory_read_macaddr0_show, NULL);
static DEVICE_ATTR(sn, S_IRUSR, sf_factory_read_sn_show, NULL);
static DEVICE_ATTR(sn_flag, S_IRUSR, sf_factory_read_sn_flag_show, NULL);
static DEVICE_ATTR(hw_ver_flag, S_IRUSR, sf_factory_read_hw_ver_flag_show, NULL);
static DEVICE_ATTR(hw_ver, S_IRUSR, sf_factory_read_hw_ver_show, NULL);
static DEVICE_ATTR(model_ver_flag, S_IRUSR, sf_factory_read_model_ver_flag_show, NULL);
static DEVICE_ATTR(model_ver, S_IRUSR, sf_factory_read_model_ver_show, NULL);
static DEVICE_ATTR(hw_feature, S_IRUSR, sf_factory_read_hw_feature_show, NULL);
static DEVICE_ATTR(vender_flag, S_IRUSR, sf_factory_read_vender_flag_show, NULL);
static DEVICE_ATTR(vender, S_IRUSR, sf_factory_read_vender_show, NULL);
static DEVICE_ATTR(product_key_flag, S_IRUSR, sf_factory_read_product_key_flag_show, NULL);
static DEVICE_ATTR(product_key, S_IRUSR, sf_factory_read_product_key_show, NULL);
static DEVICE_ATTR(login_info_flag, S_IRUSR, sf_factory_read_login_info_flag_show, NULL);
static DEVICE_ATTR(login_info, S_IRUSR, sf_factory_read_login_info_show, NULL);
static DEVICE_ATTR(rom_type_flag, S_IRUSR, sf_factory_read_rom_type_flag_show, NULL);
static DEVICE_ATTR(rom_type, S_IRUSR, sf_factory_read_rom_type_show, NULL);

static struct attribute *factory_read_attr[] = {
	&dev_attr_countryid.attr,
	&dev_attr_macaddr.attr,
	&dev_attr_macaddr_wan.attr,
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
	NULL,
};

static const struct attribute_group factory_read_attribute_group = {
	.attrs = factory_read_attr,
};

int sf_factory_read_sysfs_register(struct platform_device *pdev, char *parent)
{
	struct sfax8_factory_read_context *fr_ctx;
	struct dentry *dir_drv;

	fr_ctx = (struct sfax8_factory_read_context *)platform_get_drvdata(
			pdev);
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
