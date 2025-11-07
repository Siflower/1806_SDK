#ifndef _SF_ESWITCH_H_
#define _SF_ESWITCH_H_

#ifdef CONFIG_SWCONFIG
#include <linux/switch.h>
#endif

#include <linux/phy.h>


#include <linux/platform_device.h>

#define SF_VERSION					"1.0"

#define YT9215RB_ID					0x9002
#define YT9215RB_MODE					0x3
#define YT9215S_MODE					0x2
#define YT9215SC_MODE				    0x1
#define RTK8367C_ID					0x6367
#define INTEL7084_ID					0x2003
#define INTEL7082_ID					0x3003
#define AN8855_ID					0xc0ff0410
/* Description: 'PHY Address' */
#define PHY_ADDR_0_ADDR_OFFSET				0xF415
#define PHY_ADDR_0_ADDR_SHIFT				0
#define PHY_ADDR_0_ADDR_SIZE				5

#define SWITCH_PORT_LIST				0x1f
#define RGMII_PORT0					0x5
#define RTK_PHY_PORT_NUM				5
#define INTEL_SWITCH_PORT_NUM				7
#define INTEL_PHY_PORT_NUM				5
#define PHY_SWITCH_PORT_NUM				5
#define AN8855_NUM_PORTS					6
#define AN8855_PHY_PORT_NUM					4
#define AN8855_CPU_PORT						5
#define YT9215RB_NUM_PORTS				6
#define YT9215S_NUM_PORTS 				7
#define YT9215SC_NUM_PORTS 				7
#define YT9215RB_PHY_PORT_NUM				5
#define YT9215RB_NUM_CPU_PORTS				5
#define YT9215S_NUM_CPU_PORTS				6
#define YT9215SC_NUM_CPU_PORTS			6

#define PHY_CTRL_ENABLE_POWER_DOWN			(1 << 11)
#define PHY_IDENTIFY_1					0x02
#define PHY_IDENTIFY_2					0x03

// extern spinlock_t	mdio_lock;
extern struct mutex op_switch_lock;
// #define SF_MDIO_LOCK()				spin_lock(&mdio_lock)
// #define SF_MDIO_UNLOCK()				spin_unlock(&mdio_lock)
#define SF_MDIO_LOCK()					mutex_lock(&op_switch_lock);
#define SF_MDIO_UNLOCK()				mutex_unlock(&op_switch_lock);

enum sf_eswitch_model {
	UNKNOWN = 0,
	RTK8367C,
	INTEL7084,
	INTEL7082,
	RESERVE2,
	YT9215RB,
	YT9215S,
	AN8855,
	YT9215SC,
};

enum led_mode {
	LED_NORMAL = 0, // led on when link 10/100/1000Mbps, blink when tx/rx
	LED_ALL_ON,
	LED_ALL_OFF,
	LED_ALL_BLINK
};

struct vlan_entry {
	u16 vid;
	u32 member;
	u32 untag;
	struct list_head entry_list;
};

struct sf_eswitch_priv {
#ifdef CONFIG_SWCONFIG
	struct switch_dev	swdev;
#endif

	int model;
	int port_list;
	int phy_status[PHY_SWITCH_PORT_NUM];
	struct sf_eswitch_api_t *pesw_api;

#ifdef CONFIG_DEBUG_FS
	struct dentry *esw_debug;
#endif

	struct task_struct	*monitor_thread;
	int (*init)(struct platform_device *pdev);
	int (*deinit)(struct platform_device *pdev);
	unsigned char (*init_swdev)(struct platform_device *pdev, struct mii_bus* pmii_bus);
	void (*deinit_swdev)(struct platform_device *pdev);
	void (*write_phy)(struct sf_eswitch_priv* priv, int phyNo, int phyReg, int phyData);
	unsigned int (*read_phy)(struct sf_eswitch_priv* priv , int phyNo, int phyReg);
	void (*get_vlan)(struct sf_eswitch_priv * pesw_priv,struct vlan_entry* backup_vlan_entries);
	void (*set_vlan)(struct sf_eswitch_priv * pesw_priv,struct vlan_entry* backup_vlan_entries);
};

struct sf_eswitch_api_t {
#ifdef CONFIG_SWCONFIG
	struct switch_dev_ops *ops;
#endif
	void (*vender_init)( struct sf_eswitch_priv *eswitch_priv);
	void (*vender_deinit)( struct sf_eswitch_priv *eswitch_priv);
	void (*led_init)( int led_mode);
	void (*ifg_init)(void);
	void (*enable_all_phy)(struct sf_eswitch_priv *pesw_priv);
	void (*disable_all_phy)(void);
	int (*check_phy_linkup)(int port);
	u32 (*get_cpu_port_rx_mib)(void);
	int (*set_cpu_port_self_mirror)(struct sf_eswitch_priv *pesw_priv, int port, int enable);
	int (*getAsicReg)(unsigned int reg, unsigned int *pValue);
	int (*setAsicReg)(unsigned int reg, unsigned int pValue);
	int (*getAsicPHYReg)(unsigned int phyNo, unsigned int phyAddr, unsigned int *pRegData);
	int (*setAsicPHYReg)(unsigned int phyNo, unsigned int phyAddr, unsigned int pRegData);
	void (*dump_mac)(char *macaddr, int port);
};

#endif
