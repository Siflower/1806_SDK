/**
 ******************************************************************************
 *
 * @file siwifi_debugfs.h
 *
 * @brief Miscellaneous utility function definitions
 *
 * Copyright (C) Siflower 2018-2025
 *
 ******************************************************************************
 */


#ifndef _SIWIFI_DEBUGFS_H_
#define _SIWIFI_DEBUGFS_H_

#include <linux/workqueue.h>
#include <linux/proc_fs.h>
#include <linux/if_ether.h>
#include "siwifi_fw_trace.h"

struct siwifi_hw;
struct siwifi_sta;

/* some macros taken from iwlwifi */
/* TODO: replace with generic read and fill read buffer in open to avoid double
 * reads */
/*The macro to create file*/
#if defined (CONFIG_SIWIFI_DEBUGFS)
#define DEBUGFS_ADD_FILE(name, parent, mode) do {               \
    if (!debugfs_create_file(#name, mode, parent, siwifi_hw,      \
                &siwifi_dbgfs_##name##_ops))                      \
    goto err;                                                   \
} while (0)
#define STRUCT_ENTRY_PROC_DEBUG dentry
#define DEBUGFS_CREATE_DIR(name, parent) debugfs_create_dir(name, parent)
#define DEBUGFS_REMOVE_DIR(dir) debugfs_remove_recursive(dir)
#define private_data_proc_debug(private_data) private_data

#elif defined CONFIG_SIWIFI_PROCFS
#define DEBUGFS_ADD_FILE(name, parent, mode) do {               \
    if (!proc_create_data(#name, mode, parent,                  \
                &siwifi_dbgfs_##name##_ops, siwifi_hw))         \
        goto err;                                               \
} while (0)
#define STRUCT_ENTRY_PROC_DEBUG proc_dir_entry
#define DEBUGFS_CREATE_DIR(name, parent) proc_mkdir(name, parent)
#define DEBUGFS_REMOVE_DIR(dir) proc_remove(dir)
#define private_data_proc_debug(private_data) (struct siwifi_hw *)(((struct seq_file *) private_data)->private)
#endif

#if defined (CONFIG_SIWIFI_DEBUGFS)
#define DEBUGFS_ADD_BOOL(name, parent, ptr) do {                \
    struct STRUCT_ENTRY_PROC_DEBUG *__tmp;                                       \
    __tmp = debugfs_create_bool(#name, S_IWUSR | S_IRUSR,       \
            parent, ptr);                                       \
    if (IS_ERR(__tmp) || !__tmp)                                \
    goto err;                                                   \
} while (0)

#define DEBUGFS_ADD_X64(name, parent, ptr) do {                 \
    struct STRUCT_ENTRY_PROC_DEBUG *__tmp;                                       \
    __tmp = debugfs_create_x64(#name, S_IWUSR | S_IRUSR,        \
            parent, ptr);                                       \
    if (IS_ERR(__tmp) || !__tmp)                                \
    goto err;                                                   \
} while (0)

#define DEBUGFS_ADD_U64(name, parent, ptr, mode) do {           \
    struct STRUCT_ENTRY_PROC_DEBUG *__tmp;                                       \
    __tmp = debugfs_create_u64(#name, mode,                     \
            parent, ptr);                                       \
    if (IS_ERR(__tmp) || !__tmp)                                \
    goto err;                                                   \
} while (0)

#define DEBUGFS_ADD_X32(name, parent, ptr) do {                 \
    struct STRUCT_ENTRY_PROC_DEBUG *__tmp;                                       \
    __tmp = debugfs_create_x32(#name, S_IWUSR | S_IRUSR,        \
            parent, ptr);                                       \
    if (IS_ERR(__tmp) || !__tmp)                                \
    goto err;                                                   \
} while (0)

#define DEBUGFS_ADD_U32(name, parent, ptr, mode) do {           \
    struct STRUCT_ENTRY_PROC_DEBUG *__tmp;                                       \
    __tmp = debugfs_create_u32(#name, mode,                     \
            parent, ptr);                                       \
    if (IS_ERR(__tmp) || !__tmp)                                \
    goto err;                                                   \
} while (0)

#elif defined CONFIG_SIWIFI_PROCFS                              /*procfs */
#define DEBUGFS_ADD_BOOL(name, parent, ptr) do {           \
static const struct file_operations siwifi_dbgfs_##name##_ops = {   \
    .write  = siwifi_dbgfs_bool_write,                              \
    .read   = siwifi_dbgfs_bool_read,                               \
    .open   = siwifi_proc_open,                                     \
    .llseek = seq_lseek,                                            \
};                                                                  \
    if(!proc_create_data(#name, S_IWUSR | S_IRUSR,                  \
                parent, &siwifi_dbgfs_##name##_ops, ptr))	        \
    goto err;                                                       \
} while (0)

#define DEBUGFS_ADD_X32(name, parent, ptr, mode) do {               \
static const struct file_operations siwifi_dbgfs_##name##_ops = {   \
    .write  = siwifi_dbgfs_x32_write,                               \
    .read   = siwifi_dbgfs_x32_read,                                \
    .open   = siwifi_proc_open,                                     \
    .llseek = seq_lseek,                                            \
};                                                                  \
    if(!proc_create_data(#name, mode,                               \
                parent, &siwifi_dbgfs_##name##_ops, ptr))	        \
    goto err;                                                       \
} while (0)

#define DEBUGFS_ADD_X64(name, parent, ptr) do {               \
static const struct file_operations siwifi_dbgfs_##name##_ops = {   \
    .write  = siwifi_dbgfs_x64_write,                               \
    .read   = siwifi_dbgfs_x64_read,                                \
    .open   = siwifi_proc_open,                                     \
    .llseek = seq_lseek,                                            \
};                                                                  \
    if(!proc_create_data(#name, S_IWUSR | S_IRUSR,                               \
                parent, &siwifi_dbgfs_##name##_ops, ptr))	        \
    goto err;                                                       \
} while (0)

#define DEBUGFS_ADD_U32(name, parent, ptr, mode) do {               \
static const struct file_operations siwifi_dbgfs_##name##_ops = {   \
    .write  = siwifi_dbgfs_u32_write,                               \
    .read   = siwifi_dbgfs_u32_read,                                \
    .open   = siwifi_proc_open,                                     \
    .llseek = seq_lseek,                                            \
};                                                                  \
    if(!proc_create_data(#name, mode,                  \
                parent, &siwifi_dbgfs_##name##_ops, ptr))	        \
    goto err;                                                       \
} while (0)

#define DEBUGFS_ADD_U64(name, parent, ptr, mode) do {               \
static const struct file_operations siwifi_dbgfs_##name##_ops = {   \
    .write  = siwifi_dbgfs_u64_write,                               \
    .read   = siwifi_dbgfs_u64_read,                                \
    .open   = siwifi_proc_open,                                     \
    .llseek = seq_lseek,                                            \
};                                                                  \
    if(!proc_create_data(#name, mode,                               \
                parent, &siwifi_dbgfs_##name##_ops, ptr))	        \
    goto err;                                                       \
} while (0)
#endif

/* file operation */
#define DEBUGFS_READ_FUNC(name)                                         \
    static ssize_t siwifi_dbgfs_##name##_read(struct file *file,          \
                                            char __user *user_buf,      \
                                            size_t count, loff_t *ppos);

#define DEBUGFS_WRITE_FUNC(name)                                         \
    static ssize_t siwifi_dbgfs_##name##_write(struct file *file,          \
                                             const char __user *user_buf,\
                                             size_t count, loff_t *ppos);

#define DEBUGFS_OPEN_FUNC(name)                              \
    static int siwifi_dbgfs_##name##_open(struct inode *inode, \
                                        struct file *file);

#define DEBUGFS_RELEASE_FUNC(name)                              \
    static int siwifi_dbgfs_##name##_release(struct inode *inode, \
                                           struct file *file);

/*debugfs*/
#if defined (CONFIG_SIWIFI_DEBUGFS)
#define DEBUGFS_READ_FILE_OPS(name)                             \
    DEBUGFS_READ_FUNC(name);                                    \
static const struct file_operations siwifi_dbgfs_##name##_ops = { \
    .read   = siwifi_dbgfs_##name##_read,                         \
    .open   = simple_open,                                      \
    .llseek = generic_file_llseek,                              \
};

#define DEBUGFS_WRITE_FILE_OPS(name)                            \
    DEBUGFS_WRITE_FUNC(name);                                   \
static const struct file_operations siwifi_dbgfs_##name##_ops = { \
    .write  = siwifi_dbgfs_##name##_write,                        \
    .open   = simple_open,                                      \
    .llseek = generic_file_llseek,                              \
};

#define DEBUGFS_READ_WRITE_FILE_OPS(name)                       \
    DEBUGFS_READ_FUNC(name);                                    \
    DEBUGFS_WRITE_FUNC(name);                                   \
static const struct file_operations siwifi_dbgfs_##name##_ops = { \
    .write  = siwifi_dbgfs_##name##_write,                        \
    .read   = siwifi_dbgfs_##name##_read,                         \
    .open   = simple_open,                                      \
    .llseek = generic_file_llseek,                              \
};

#define DEBUGFS_READ_WRITE_OPEN_RELEASE_FILE_OPS(name)              \
    DEBUGFS_READ_FUNC(name);                                        \
    DEBUGFS_WRITE_FUNC(name);                                       \
    DEBUGFS_OPEN_FUNC(name);                                        \
    DEBUGFS_RELEASE_FUNC(name);                                     \
static const struct file_operations siwifi_dbgfs_##name##_ops = {     \
    .write   = siwifi_dbgfs_##name##_write,                           \
    .read    = siwifi_dbgfs_##name##_read,                            \
    .open    = siwifi_dbgfs_##name##_open,                            \
    .release = siwifi_dbgfs_##name##_release,                         \
    .llseek  = generic_file_llseek,                                 \
};

#elif defined CONFIG_SIWIFI_PROCFS                                  /*procfs*/
#define DEBUGFS_READ_FILE_OPS(name)                             \
    DEBUGFS_READ_FUNC(name);                                    \
static struct file_operations siwifi_dbgfs_##name##_ops = { \
    .owner = THIS_MODULE,                                          \
    .read   = siwifi_dbgfs_##name##_read,                         \
    .open   = siwifi_proc_open,                                      \
    .llseek = seq_lseek,                              \
};

#define DEBUGFS_WRITE_FILE_OPS(name)                            \
    DEBUGFS_WRITE_FUNC(name);                                   \
static struct file_operations siwifi_dbgfs_##name##_ops = { \
    .owner = THIS_MODULE,                                          \
    .write  = siwifi_dbgfs_##name##_write,                        \
    .open   = siwifi_proc_open,                                      \
    .llseek = seq_lseek,                              \
};

#define DEBUGFS_READ_WRITE_FILE_OPS(name)                       \
    DEBUGFS_READ_FUNC(name);                                    \
    DEBUGFS_WRITE_FUNC(name);                                   \
static struct file_operations siwifi_dbgfs_##name##_ops = { \
    .owner = THIS_MODULE,                                          \
    .write  = siwifi_dbgfs_##name##_write,                        \
    .read   = siwifi_dbgfs_##name##_read,                         \
    .open   = siwifi_proc_open,                                      \
    .llseek = seq_lseek,                              \
};

#define DEBUGFS_READ_WRITE_OPEN_RELEASE_FILE_OPS(name)              \
    DEBUGFS_READ_FUNC(name);                                        \
    DEBUGFS_WRITE_FUNC(name);                                       \
    DEBUGFS_OPEN_FUNC(name);                                        \
    DEBUGFS_RELEASE_FUNC(name);                                     \
static struct file_operations siwifi_dbgfs_##name##_ops = {     \
    .owner = THIS_MODULE,                                          \
    .write   = siwifi_dbgfs_##name##_write,                           \
    .read    = siwifi_dbgfs_##name##_read,                            \
    .open    = siwifi_dbgfs_##name##_open,                            \
    .release = siwifi_dbgfs_##name##_release,                         \
    .llseek = generic_file_llseek,                              \
};
#endif

#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)

enum {
    RC_IDLE,
    RC_REGISTER,
    RC_UNREGISTER
};

struct siwifi_debugfs {
    unsigned long long rateidx;
    struct STRUCT_ENTRY_PROC_DEBUG *dir;
    bool trace_prst;
    bool recoverying;

    char helper_cmd[64];
    struct work_struct helper_work;
    bool helper_scheduled;
    spinlock_t umh_lock;
    bool unregistering;

    struct siwifi_fw_trace fw_trace;

#ifdef CONFIG_HEART_BEAT
    int recovery_hb_count;
#endif
    uint8_t force_trigger_type;
    uint8_t recovery;
    struct work_struct rc_stat_work;
    uint8_t rc_sta[NX_REMOTE_STA_MAX * 4];
    uint8_t rc_sta_action[NX_REMOTE_STA_MAX * 4];
    uint8_t rc_write;
    uint8_t rc_read;
    struct STRUCT_ENTRY_PROC_DEBUG *dir_rc;
    struct STRUCT_ENTRY_PROC_DEBUG *dir_sta[NX_REMOTE_STA_MAX];
    int rc_config[NX_REMOTE_STA_MAX];
    struct list_head rc_config_save;
    unsigned int skb_alloc_fail_cnt;
    unsigned int skb_drop_cnt;
};


// Max duration in msecs to save rate config for a sta after disconnection
#define RC_CONFIG_DUR 600000

struct siwifi_rc_config_save {
    struct list_head list;
    unsigned long timestamp;
    int rate;
    u8 mac_addr[ETH_ALEN];
};

int siwifi_dbgfs_register(struct siwifi_hw *siwifi_hw, const char *name);
void siwifi_dbgfs_unregister(struct siwifi_hw *siwifi_hw);
int siwifi_um_helper(struct siwifi_debugfs *siwifi_debugfs, const char *cmd);
int siwifi_trigger_um_helper(struct siwifi_debugfs *siwifi_debugfs);
void siwifi_dbgfs_register_rc_stat(struct siwifi_hw *siwifi_hw, struct siwifi_sta *sta);
void siwifi_dbgfs_unregister_rc_stat(struct siwifi_hw *siwifi_hw, struct siwifi_sta *sta);
#ifdef CONFIG_SIWIFI_PROCFS
int siwifi_proc_open(struct inode *inode, struct file *file);
#endif

int siwifi_dbgfs_register_fw_dump(struct siwifi_hw *siwifi_hw,
                                struct STRUCT_ENTRY_PROC_DEBUG *dir_drv,
                                struct STRUCT_ENTRY_PROC_DEBUG *dir_diags);
void siwifi_dbgfs_trigger_fw_dump(struct siwifi_hw *siwifi_hw, char *reason);

void siwifi_fw_trace_dump(struct siwifi_hw *siwifi_hw);
void siwifi_fw_trace_reset(struct siwifi_hw *siwifi_hw);

void siwifi_hwq_dump_info(struct siwifi_hw *hw, char *buf, size_t size, int *idx);
void siwifi_txq_dump_info(struct siwifi_hw *siwifi_hw, char *buf, size_t size, int *idx);

#else

struct siwifi_debugfs {
};

static inline int siwifi_dbgfs_register(struct siwifi_hw *siwifi_hw, const char *name) { return 0; }
static inline void siwifi_dbgfs_unregister(struct siwifi_hw *siwifi_hw) {}
static inline int siwifi_um_helper(struct siwifi_debugfs *siwifi_debugfs, const char *cmd) { return 0; }
static inline int siwifi_trigger_um_helper(struct siwifi_debugfs *siwifi_debugfs) { return 0; }
static inline void siwifi_dbgfs_register_rc_stat(struct siwifi_hw *siwifi_hw, struct siwifi_sta *sta)  {}
static inline void siwifi_dbgfs_unregister_rc_stat(struct siwifi_hw *siwifi_hw, struct siwifi_sta *sta)  {}

void siwifi_fw_trace_dump(struct siwifi_hw *siwifi_hw) {}
void siwifi_fw_trace_reset(struct siwifi_hw *siwifi_hw) {}

void siwifi_hwq_dump_info(struct siwifi_hw *hw, char *buf, size_t size, int *idx) {}
void siwifi_txq_dump_info(struct siwifi_hw *siwifi_hw, char *buf, size_t size, int *idx) {}

#endif /* CONFIG_SIWIFI_DEBUGFS */

#endif /* _SIWIFI_DEBUGFS_H_ */
