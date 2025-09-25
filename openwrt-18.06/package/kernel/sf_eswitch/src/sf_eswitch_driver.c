#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kthread.h>
#include <linux/bitmap.h>
#include <linux/delay.h>
#include <linux/of_mdio.h>
#include <linux/platform_device.h>
#include "sf_eswitch.h"
#include "realtek8367c_src/rtk_types.h"
#include "realtek8367c_src/rtk_switch.h"
#include "realtek8367c_src/rtl8367c_asicdrv.h"
#include "intel7084_src/src/gsw_sw_init.h"
#include "yt9215rb_src/sf_yt9215rb_ops.h"
#include "yt9215rb_src/chipdef_tiger.h"
#ifdef CONFIG_DEBUG_FS
#include "sf_eswitch_debug.h"
#endif

#ifdef CONFIG_SFAX8_GENL
#include "sfax8_netlink.h"
#endif

struct mutex op_switch_lock;
// spinlock_t	mdio_lock;
struct mii_bus *gp_mii_bus = NULL;
struct vlan_entry vlan_entries;
extern struct sf_eswitch_api_t an8855_api;
extern ethsw_api_dev_t *pedev0[GSW_DEV_MAX];
int rtk_phy_id;
int rtk_rgmii_port;
int air_an8855_phy_id_get(void);
#ifdef CONFIG_SFAX8_GENL
sf_nlfamily dps_family;
struct sfax8_netlink *genl_priv = NULL;
#endif

static struct of_device_id mdio_gpio_of_match[] = {
	{.compatible = "virtual,mdio-gpio",},
	{}
};

#ifdef CONFIG_SFAX8_GENL
int notify_link_event(struct sf_eswitch_priv *pesw_priv, int port, int updown, char *ifname)
{
	struct sk_buff *skb;
	int ret = 0;
	void *msg_head;
	struct genl_family *family = &(dps_family.family);
	char macaddr[20] = {0};

	skb = genlmsg_new(MAX_MSG_SIZE, GFP_KERNEL);
	if (!skb)
		return -ENOMEM;
	msg_head = genlmsg_put(skb, 0, 0, family, 0, SF_CMD_GENERIC);
	if (!msg_head) {
		printk("%s : add genlmsg header error!\n", __func__);
		ret = -ENOMEM;
		goto err;
	}
	ret = nla_put_u32(skb, SF_ETH_CMD_ATTR_DPS_PORT, port);
	if (ret < 0)
		goto err;

	ret = nla_put_u32(skb, SF_ETH_CMD_ATTR_DPS_LINK, updown);
	if (ret < 0)
		goto err;

	if (updown > 0) {
		pesw_priv->pesw_api->dump_mac(macaddr, port);
		ret = nla_put_string(skb, SF_ETH_CMD_ATTR_DPS_MAC, macaddr);
		if (ret < 0)
			goto err;
	}

	ret = nla_put_string(skb, SF_ETH_CMD_ATTR_DPS_IFNAME, ifname);
	if (ret < 0)
		goto err;

	genlmsg_end(skb, msg_head);
	ret = genlmsg_multicast(family, skb, 0, 0, GFP_KERNEL);
	return 0;
err:
	nlmsg_free(skb);
	return ret;
}
#endif



#ifdef CONFIG_DEBUG_FS
static struct file_operations esw_debug_ops = {
	.owner = THIS_MODULE,
	.open  = sf_eswitch_debug_open,
	.read  = sf_eswitch_debug_read,
	.write  = sf_eswitch_debug_write,
	.release  = sf_eswitch_debug_release,
	.llseek  = default_llseek,
	.unlocked_ioctl = sf_eswitch_debug_ioctl
};
#endif

int check_port_in_portlist(struct sf_eswitch_priv *pesw_priv, int port)
{
	return (1 << port) & pesw_priv->port_list ? 1 : 0;
}

static int old_phy_status[PHY_SWITCH_PORT_NUM] = {0};
static int phy_monitor_thread(void *data)
{
	struct sf_eswitch_priv *pesw_priv = data;
	struct sf_eswitch_api_t *pesw_api = pesw_priv->pesw_api;
	int i, updown, max_port = 0;

	if (pesw_priv->model == RTK8367C)
		max_port = RTK_SWITCH_PORT_NUM;
	else if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082)
		max_port = INTEL_SWITCH_PORT_NUM;
	else if (pesw_priv->model == YT9215RB || pesw_priv->model == YT9215S || pesw_priv->model == YT9215SC)
		max_port = YT9215RB_PHY_PORT_NUM;

	for (i = 0; i < max_port; i++) {
		if (!check_port_in_portlist(pesw_priv, i))
			continue;

		if (pesw_api->check_phy_linkup(i))
			updown = 1;
		else
			updown = 0;

#ifdef CONFIG_SFAX8_GENL
		if (updown && old_phy_status[i] != pesw_priv->phy_status[i])
			notify_link_event(pesw_priv, i, updown, "eth0");
#endif
	old_phy_status[i] = pesw_priv->phy_status[i] = updown;
	}

	while (!kthread_should_stop()) {
		msleep(2000);
		for (i = 0; i < max_port; i++) {
			if (!check_port_in_portlist(pesw_priv, i))
				continue;

			pesw_priv->phy_status[i] = pesw_api->check_phy_linkup(i);
#ifdef CONFIG_SFAX8_GENL
			if (old_phy_status[i] != pesw_priv->phy_status[i])
				notify_link_event(pesw_priv, i,
						pesw_priv->phy_status[i], "eth0");
#endif
			old_phy_status[i] = pesw_priv->phy_status[i];
		}
	}
	return 0;
}

void sf_eswitch_deinit_swdev(struct platform_device *pdev)
{
	struct sf_eswitch_priv *pesw_priv = platform_get_drvdata(pdev);

#ifdef CONFIG_SFAX8_GENL
	if (genl_priv)
		genl_priv->genl_family_exit(&(dps_family.family));
#endif
	pesw_priv->pesw_api->vender_deinit(pesw_priv);
#ifdef CONFIG_SWCONFIG
	unregister_switch(&pesw_priv->swdev);
#endif
	return;
}

unsigned char sf_eswitch_init_swdev(struct platform_device *pdev, struct mii_bus* pmii_bus)
{
	struct sf_eswitch_priv *pesw_priv = platform_get_drvdata(pdev);
	struct device_node *mdio_node = NULL;
#ifdef CONFIG_SWCONFIG
	struct switch_dev *pswdev;
	int ret = 0;
#endif
	unsigned int chip_id = 0, retry_times = 0, chip_mode = 0;
	gp_mii_bus = pmii_bus;

#ifdef CONFIG_MDIO_GPIO
	mdio_node = of_find_matching_node(NULL, mdio_gpio_of_match);
	if (mdio_node) {
		gp_mii_bus = of_mdio_find_bus(mdio_node);
	}
#endif

	if (gp_mii_bus == NULL) {
		return UNKNOWN;
	}

#ifdef CONFIG_SWCONFIG
	pswdev = &pesw_priv->swdev;
#endif

	do {
		// chip id to read realtek 8367c
		rtk_phy_id = 0;
		rtl8367c_setAsicReg(0x13C2, 0x0249);
		rtl8367c_getAsicReg(0x1300, &chip_id);

		if (chip_id == RTK8367C_ID) {
			pesw_priv->model =  RTK8367C;
			pesw_priv->pesw_api = &rtk8367c_api;
			pesw_priv->port_list = SWITCH_PORT_LIST;
			rtk_rgmii_port = EXT_PORT0;
#ifdef CONFIG_SWCONFIG
			pswdev->ports = RTK_SWITCH_PORT_NUM;
			pswdev->cpu_port = EXT_PORT0;
#endif
			break;
		}

		// chip id to read realtek 8367s
		rtk_phy_id = 29;
		rtl8367c_setAsicReg(0x13C2, 0x0249);
		rtl8367c_getAsicReg(0x1300, &chip_id);

		if (chip_id == RTK8367C_ID) {
			pesw_priv->model =  RTK8367C;
			pesw_priv->pesw_api = &rtk8367c_api;
			pesw_priv->port_list = SWITCH_PORT_LIST;
			rtk_rgmii_port = EXT_PORT1;
#ifdef CONFIG_SWCONFIG
			pswdev->ports = RTK_SWITCH_PORT_NUM;
			pswdev->cpu_port = EXT_PORT1;
#endif
			break;
		}

		// chip id to read an8855
		chip_id = air_an8855_phy_id_get();
		if (chip_id == AN8855_ID) {
			pesw_priv->model = AN8855;
			pesw_priv->pesw_api = &an8855_api;
			pesw_priv->port_list = SWITCH_PORT_LIST;
#ifdef CONFIG_SWCONFIG
			pswdev->ports = AN8855_NUM_PORTS;
			pswdev->cpu_port = AN8855_CPU_PORT;
#endif
			break;
		}
		
		//chip id to read intel
		ethsw_init_pedev0();
		intel7084_mdio_rd(0xFA11, 0, 16, &chip_id);
		if (chip_id == INTEL7084_ID) {
			pesw_priv->model = INTEL7084;
			pesw_priv->pesw_api = &intel7084_api;
			pesw_priv->port_list = SWITCH_PORT_LIST;
#ifdef CONFIG_SWCONFIG
			pswdev->ports = INTEL_SWITCH_PORT_NUM;
			pswdev->cpu_port = RGMII_PORT0;
#endif
			break;
		}
		//chip id to read intel7082 mdio_addr 0x1f
		pedev0[0]->mdio_id = 0x1;
		pedev0[0]->mdio_addr = 0x1F;
		intel7084_mdio_rd(0xFA11, 0, 16, &chip_id);
		if (chip_id == INTEL7082_ID) {
			pesw_priv->model = INTEL7082;
			pesw_priv->pesw_api = &intel7084_api;
			pesw_priv->port_list = SWITCH_PORT_LIST;
#ifdef CONFIG_SWCONFIG
			pswdev->ports = INTEL_SWITCH_PORT_NUM;
			pswdev->cpu_port = RGMII_PORT0;
#endif
			break;
		}

		//chip id to read intel7082 mdio_addr 0x4
		pedev0[0]->mdio_addr = 0x4;
		intel7084_mdio_rd(0xFA11, 0, 16, &chip_id);
		if(chip_id == INTEL7082_ID){
			pesw_priv->model = INTEL7082;
			pesw_priv->pesw_api = &intel7084_api;
			pesw_priv->port_list = SWITCH_PORT_LIST;
#ifdef CONFIG_SWCONFIG
			pswdev->ports = INTEL_SWITCH_PORT_NUM;
			pswdev->cpu_port = RGMII_PORT0;
#endif
			break;
		}

		// chip id to read yt9215rb | yt9215s
		yt9215rb_getAsicReg(CHIP_CHIP_ID_REG, &chip_id);
		yt9215rb_getAsicReg(CHIP_CHIP_MODE_REG, &chip_mode);

		if ((chip_id >> 16 & 0xffff) == YT9215RB_ID &&
			(chip_mode & 0x3) == YT9215RB_MODE) {

			pesw_priv->model = YT9215RB;
			pesw_priv->pesw_api = &yt9215rb_api;
			pesw_priv->port_list = SWITCH_PORT_LIST;
			yt9215_port_nums = YT9215RB_NUM_PORTS;
			yt9215_cpu_port = YT9215RB_NUM_CPU_PORTS;
#ifdef CONFIG_SWCONFIG
			pswdev->ports = YT9215RB_NUM_PORTS;
			pswdev->cpu_port = YT9215RB_NUM_CPU_PORTS;
#endif
			break;
		} else if ((chip_id >> 16 & 0xffff) == YT9215RB_ID &&
		 (chip_mode & 0x3) == YT9215S_MODE) {
			pesw_priv->model = YT9215S;
			pesw_priv->pesw_api = &yt9215rb_api;
			pesw_priv->port_list = SWITCH_PORT_LIST;
			yt9215_port_nums = YT9215S_NUM_PORTS;
			yt9215_cpu_port = YT9215S_NUM_CPU_PORTS;
#ifdef CONFIG_SWCONFIG
			pswdev->ports = YT9215S_NUM_PORTS;
			pswdev->cpu_port = YT9215S_NUM_CPU_PORTS;
#endif
			break;
		} else if ((chip_id >> 16 & 0xffff) == YT9215RB_ID &&
		 (chip_mode & 0x3) == YT9215SC_MODE) {
			pesw_priv->model = YT9215SC;
			pesw_priv->pesw_api = &yt9215rb_api;
			pesw_priv->port_list = SWITCH_PORT_LIST;
			yt9215_port_nums = YT9215SC_NUM_PORTS;
			yt9215_cpu_port = YT9215SC_NUM_CPU_PORTS;
#ifdef CONFIG_SWCONFIG
			pswdev->ports = YT9215SC_NUM_PORTS;
			pswdev->cpu_port = YT9215SC_NUM_CPU_PORTS;
#endif
			break;
        }

		retry_times++;
		printk("unknown switch type! retry times:%d\n", retry_times);

	} while (retry_times < 3);

#ifdef CONFIG_SWCONFIG
	pswdev->ops = pesw_priv->pesw_api->ops;
#endif

	memset(&vlan_entries, 0, sizeof(struct vlan_entry));
	if (!vlan_entries.entry_list.prev)
		INIT_LIST_HEAD(&(vlan_entries.entry_list));
	// init eswitch hw
	pesw_priv->pesw_api->vender_init(pesw_priv);
	// init eswitch led mode
	pesw_priv->pesw_api->led_init(LED_NORMAL);

#ifdef CONFIG_SFAX8_GENL
	if (genl_priv) {
		genl_priv->genl_family_init(&dps_family, "DPS_NL", "updown");
		printk("init dps family with id %d!\n", dps_family.family.id);
	}
#endif
#ifdef CONFIG_SWCONFIG
	ret = register_switch(pswdev, NULL);
	if (ret) {

		pesw_priv->pesw_api->vender_deinit(pesw_priv);
		printk("failed to register sfax8\n");
	}
#endif
	return pesw_priv->model;
}

unsigned int sf_eswitch_read_phy_reg(struct sf_eswitch_priv* priv , int phyNo, int phyReg)
{
	GSW_MDIO_data_t parm;
	unsigned int phyData = 0;

	SF_MDIO_LOCK();
	if (priv->model == RTK8367C) {
		rtl8367c_getAsicPHYReg(phyNo, phyReg, &phyData);
	}
	else if (priv->model == INTEL7084 || priv->model == INTEL7082) {
		parm.nAddressDev = phyNo;
		parm.nAddressReg = phyReg;
		intel7084_phy_rd(&parm);
		phyData = parm.nData;
	} else if (priv->model == YT9215RB || priv->model == YT9215S || priv->model == YT9215SC) {
		yt9215rb_getAsicPHYReg(phyNo, phyReg, &phyData);
	}

	SF_MDIO_UNLOCK();

	return phyData;
}

void sf_eswitch_write_phy_reg(struct sf_eswitch_priv* priv, int phyNo, int phyReg, int phyData)
{
	GSW_MDIO_data_t parm;

	SF_MDIO_LOCK();
	if (priv->model == RTK8367C) {
		rtl8367c_setAsicPHYReg(phyNo, phyReg, phyData);
	}
	else if (priv->model == INTEL7084 || priv->model == INTEL7082) {
		parm.nAddressDev = phyNo;
		parm.nAddressReg = phyReg;
		parm.nData = phyData;
		intel7084_phy_wr(&parm);
	} else if (priv->model == YT9215RB || priv->model == YT9215S || priv->model == YT9215SC) {
		yt9215rb_setAsicPHYReg(phyNo, phyReg, phyData);
	}

	SF_MDIO_UNLOCK();

	return;
}

int mdio_read_ext(int phyaddr, int phyreg, int *phydata)
{
	struct mii_bus *pmii_bus ;
	int phy_value= 0;
	pmii_bus = gp_mii_bus;
	if (!pmii_bus) {
		printk("mdio bus not found\n");
		return -1;
	}

	phy_value = pmii_bus->read(pmii_bus, phyaddr, phyreg);
	*phydata = phy_value;
	return 0;
}

int mdio_write_ext(int phyaddr, int phyreg, int phydata)
{
	struct mii_bus *pmii_bus;
	pmii_bus = gp_mii_bus;
	if (!pmii_bus) {
		printk("mdio bus not found\n");
		return -1;
	}
	pmii_bus->write(pmii_bus, phyaddr, phyreg, phydata);
	return 0;
}

int air_an8855_phy_id_get(void)
{
	int phydata = 0, phy_id;
	mdio_read_ext(1, 2, &phydata);
	phy_id = (phydata & 0xffff) << 16;
	mdio_read_ext(1, 3, &phydata);
	phy_id |= phydata & 0xffff;
	return phy_id;
}

int sf_eswitch_init(struct platform_device *pdev)
{
#ifdef CONFIG_SWCONFIG
	struct switch_dev *pswdev;
#endif
	struct sf_eswitch_priv *pesw_priv = (struct sf_eswitch_priv *)platform_get_drvdata(pdev);


#ifdef CONFIG_SWCONFIG
	pswdev = &pesw_priv->swdev;
	pesw_priv->monitor_thread = kthread_run(phy_monitor_thread, pesw_priv, "%s",
			pswdev->name);
#else
	pesw_priv->monitor_thread = kthread_run(phy_monitor_thread, pesw_priv, "%s",
			"sf_eswitch");
#endif

	if (IS_ERR(pesw_priv->monitor_thread)) {
		printk("failed to alloc kthread\n");
		goto err_out_thread;
	}

	// if (pesw_priv->model == INTEL7084)
	// 	sf_intel7084_qos_register();

	return 0;

err_out_thread:
	// pesw_priv->pesw_api->vender_deinit(pesw_priv);
	printk("eswitch init fail\n");
	return -1;
}

static int sf_eswitch_deinit(struct platform_device *pdev)
{
	struct sf_eswitch_priv *pesw_priv = platform_get_drvdata(pdev);
	//	struct sf_eswitch_api_t *pesw_api = pesw_priv->pesw_api;
	if (!pesw_priv) {
		printk("eswitch is null deinit fail\n");
		return -1;
	}

	// if (pesw_priv->model == INTEL7084)
	// 	sf_intel7084_qos_unregister();

	kthread_stop(pesw_priv->monitor_thread);

	// pesw_api->vender_deinit(pesw_priv);

	return 0;
}

static int sf_eswitch_probe(struct platform_device *pdev)
{
	struct sf_eswitch_priv *pesw_priv;
#ifdef CONFIG_SFAX8_GENL
	struct platform_device *genl_pdev = NULL;
#endif

	pesw_priv = kzalloc(sizeof(struct sf_eswitch_priv), GFP_KERNEL);
	if (!pesw_priv) {
		dev_err(&pdev->dev, "no memory for eswitch data\n");
		return -ENOMEM;
	}

	// spin_lock_init(&mdio_lock);
	mutex_init(&op_switch_lock);
	platform_set_drvdata(pdev, pesw_priv);
#ifdef CONFIG_SWCONFIG
	pesw_priv->swdev.alias = "sfax8_eswitch";
	pesw_priv->swdev.name = "sfax8_eswitch";
	pesw_priv->swdev.vlans = 4096;
#endif
	pesw_priv->init = sf_eswitch_init;
	pesw_priv->deinit = sf_eswitch_deinit;
	pesw_priv->init_swdev = sf_eswitch_init_swdev;
	pesw_priv->deinit_swdev = sf_eswitch_deinit_swdev;
	pesw_priv->write_phy = sf_eswitch_write_phy_reg;
	pesw_priv->read_phy = sf_eswitch_read_phy_reg;

#ifdef CONFIG_SFAX8_GENL
	genl_pdev = platform_device_register_simple("sf_genl",PLATFORM_DEVID_AUTO,NULL,0);
	if (genl_pdev == NULL)
		goto err_out_pdev;

	printk("genl dev name %s\n", dev_name(&genl_pdev->dev));
	genl_priv = platform_get_drvdata(genl_pdev);
	if (genl_priv == NULL) {
		printk("probe genl fail\n");
		goto err_genl_priv;
	}
#endif

#ifdef CONFIG_DEBUG_FS
	pesw_priv->esw_debug = debugfs_create_file("esw_debug", 0777, NULL,
			(void *)pesw_priv, &esw_debug_ops);
	if (pesw_priv->esw_debug == NULL) {
		goto err_out_debugfs;
	}
#endif

	printk("eswitch probe success\n");
	return 0;

#ifdef CONFIG_DEBUG_FS
err_out_debugfs:
#endif
#ifdef CONFIG_SFAX8_GENL
err_genl_priv:
	platform_device_unregister(genl_pdev);
err_out_pdev:
#endif
	platform_set_drvdata(pdev, NULL);
	kfree(pesw_priv);
	printk("eswitch probe fail\n");
	return -1;
}


static int sf_eswitch_remove(struct platform_device *pdev)
{
	struct sf_eswitch_priv *pesw_priv = platform_get_drvdata(pdev);

	if (!pesw_priv) {
		printk("eswitch is null, remove fail\n");
		return -1;
	}

	if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082)
		intel7084_bridge_redirect_disable();

#ifdef CONFIG_DEBUG_FS
	debugfs_remove(pesw_priv->esw_debug);
#endif
	platform_set_drvdata(pdev, NULL);
	kfree(pesw_priv);

	mutex_destroy(&op_switch_lock);
	printk("eswitch remove success\n");
	return 0;
}

static struct platform_driver sf_eswitch_driver = {
	.driver = {
		.name = "sf_eswitch",
		.owner = THIS_MODULE,
	},
	.probe = sf_eswitch_probe,
	.remove = sf_eswitch_remove,
};

static int __init sf_eswitch_module_init(void)
{
	return platform_driver_register(&sf_eswitch_driver);
}
module_init(sf_eswitch_module_init);

static void __exit sf_eswitch_module_exit(void)
{
	platform_driver_unregister(&sf_eswitch_driver);
}
module_exit(sf_eswitch_module_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Gigabit switch driver for sfax8");
MODULE_VERSION(SF_VERSION);
