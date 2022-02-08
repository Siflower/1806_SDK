/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 1999,2000 MIPS Technologies, Inc.  All rights reserved.
 */
#include <linux/io.h>
#include <linux/pm.h>
#include <asm/reboot.h>
#include <asm/delay.h>
//kmalloc/kfree
#include <linux/slab.h>

#ifdef CONFIG_SOC_SF16A18
#include <sf16a18.h>
#elif defined CONFIG_SOC_SF19A28
#include <sf19a28.h>
#endif
#include "include/sf_mmap.h"

#define LATENCY					1

#define CLK_EN_OFFSET			0x04
#define	BOE_OFFSET				0x0C
#define	MEMCTL_OFFSET				0x14
#define IRAM_SOFT_RESET1_OFFSET	0x1C
#define IRAM_CLK1_EN_OFFSET		0x20


#define OMINI_PHY1_ADDR (0x10)
#define OMINI_PHY2_ADDR (0x11)
#define OMINI_PHY3_ADDR (0x12)
#define OMINI_PHY4_ADDR (0x13)
#define OMINI_PHY5_ADDR (0x14)

//PHY ID Number[21:0]
#define OMINI_PHY_ID_1  (0x0F1F1F)
#define OMINI_PHY_ID_2  (0x0F2F2F)
#define OMINI_PHY_ID_3  (0x0F3F3F)
#define OMINI_PHY_ID_4  (0x0F4F5F)
#define OMINI_PHY_ID_5  (0x0F5F5F)

//Manufacturer’s Model Number[5:0]
#define OMINI_PHY_MODEL_NUMBER (0x1F)
//Manufacturer’s Revision Number[3:0]
#define OMINI_PHY_REV_NUMBER (0x01)

//OMINIPHY cfg_mode defination
#define OMINI_PHY_CFG_MODE_AUTO_FULL 0xE0
#define OMINI_PHY_CFG_MODE_10M_FULL 0x20
#define OMINI_PHY_CFG_MODE_100M_FULL 0x60
#define OMINI_PHY_CFG_MODE_10M_HALF 0x00
#define OMINI_PHY_CFG_MODE_100M_HALF 0x40
#define OMINI_PHY_CFG_MODE_AUTO_HALF 0x80
#define OMINI_PHY_CFG_MODE_REPEATER_100M_HALF 0xa0
#define OMINI_PHY_CFG_MODE_POWERDOWN 0xc0
#define OMINI_PHY_DUPLEX_FULL 0
#define OMINI_PHY_DUPLEX_HALF 1
#define OMINI_SPEED_100   0
#define OMINI_SPEED_10    1
#define OMINI_SPEED_AUTO  2

#define GMAC_PHY_INTF_SEL_GMII_MII 0x0
#define GMAC_PHY_INTF_SEL_RGMII    0x1
#define GMAC_PHY_INTF_SEL_SGMII    0x2
#define GMAC_PHY_INTF_SEL_TBI      0x3
#define GMAC_PHY_INTF_SEL_RMII     0x4
#define GMAC_PHY_INTF_SEL_RTBI     0x5
#define GMAC_PHY_INTF_SEL_SMII     0x6
#define GMAC_PHY_INTF_SEL_REVMII   0x7
#define GMAC_PHY_INTF_SEL_MASK     0x7
#define CLK_RMII_OEN           (1 << 5)

struct reset{
	unsigned int soft_reset;
	unsigned int sysm_reset;
};

static struct reset resets[] =
{
	{
		.soft_reset = SF_IRAM_SOFT_RESET,
		.sysm_reset = SF_IRAM_SYSM_RESET,
	},
	{
		.soft_reset = SF_NPU_SOFT_RESET,
		.sysm_reset = SF_NPU_SYSM_RESET,
	},
	{
		.soft_reset = SF_EMAC_SOFT_RESET,
		.sysm_reset = SF_EMAC_SYSM_RESET,
	},
#ifndef CONFIG_SF16A18_MPW0
	{
		.soft_reset = SF_GDU_SOFT_RESET,
		.sysm_reset = SF_GDU_SYSM_RESET,
	},
	{
		.soft_reset = SF_CRYPTO_SOFT_RESET,
		.sysm_reset = SF_CRYPTO_SYSM_RESET,
	},
#endif
	{
		.soft_reset = SF_WIFI_1_SOFT_RESET,
		.sysm_reset = SF_WIFI_1_SYSM_RESET,
	},
	{
		.soft_reset = SF_WIFI_2_SOFT_RESET,
		.sysm_reset = SF_WIFI_2_SYSM_RESET,
	},
	{
		.soft_reset = SF_GDMA_SOFT_RESET,
		.sysm_reset = SF_GDMA_SYSM_RESET,
	},
	{
		.soft_reset = SF_SCDMA_SOFT_RESET,
		.sysm_reset = SF_SCDMA_SYSM_RESET,
	},
	{
		.soft_reset = SF_USB_SOFT_RESET,
		.sysm_reset = SF_USB_SYSM_RESET,
	},
	{
		.soft_reset = SF_EMMC_SOFT_RESET,
		.sysm_reset = SF_EMMC_SYSM_RESET,
	},
	{
		.soft_reset = SF_SDIO_SOFT_RESET,
		.sysm_reset = SF_SDIO_SYSM_RESET,
	},
	{
		.soft_reset = SF_CATIP_SOFT_RESET,
		.sysm_reset = SF_CATIP_SYSM_RESET,
	},
	{
		.soft_reset = SF_IIS_SOFT_RESET,
		.sysm_reset = SF_IIS_SYSM_RESET,
	},
	{
		.soft_reset = SF_IIC_SOFT_RESET,
		.sysm_reset = SF_IIC_SYSM_RESET,
	},
	{
		.soft_reset = SF_SPI_SOFT_RESET,
		.sysm_reset = SF_SPI_SYSM_RESET,
	},
	{
		.soft_reset = SF_UART_SOFT_RESET,
		.sysm_reset = SF_UART_SYSM_RESET,
	},
	{
		.soft_reset = SF_PCM_SOFT_RESET,
		.sysm_reset = SF_PCM_SYSM_RESET,
	},
	{
		.soft_reset = SF_PWM_SOFT_RESET,
		.sysm_reset = SF_PWM_SYSM_RESET,
	},
	{
		.soft_reset = SF_TIMER_SOFT_RESET,
		.sysm_reset = SF_TIMER_SYSM_RESET,
	},
	{
		.soft_reset = SF_WDT_SOFT_RESET,
		.sysm_reset = SF_WDT_SYSM_RESET,
	},
	{
		.soft_reset = SF_SPDIF_SOFT_RESET,
		.sysm_reset = SF_SPDIF_SYSM_RESET,
	},
};
static void mips_machine_restart(char *command)
{
#ifdef CONFIG_SF16A18_MPW0
	/* workaround:20170322 SW Reset peration only trig cpu core reset
	   Don't reinit system manager, So we should resume iram mode
	   Otherwise cpu can't access iram durning irom booting */
	__raw_writel(0x0, (void *)SIFLOWER_WIFI_SHARE_RAM_SEL);
	pr_info("MPW0 soft reboot workaround done!\n");
#endif

	dump_stack();
	__raw_writel(SIFLOWER_SW_TRIG_RESET, (void *)SIFLOWER_SW_RESET);
}

static void mips_machine_halt(void)
{
	while (true);
}

static void mips_machine_power_off(void)
{
	//TODO use pmu to power off all soc by i2c interface.
	pr_info("Failed to power down, resetting\n");
	mips_machine_restart(NULL);
}

#ifndef CONFIG_SF16A18_MPW0
int print_reboot_event(void)
{
	int soft_reboot, wdt_reboot, catip_reboot;

	soft_reboot = __raw_readl((void *)SOFT_EVENTS_RO);
	if (soft_reboot) {
		pr_info("Reboot from soft reboot sys-manager\n");
		return soft_reboot;
	}

	wdt_reboot = __raw_readl((void *)SYS_EVTS6356);
	catip_reboot = wdt_reboot & (1 << 6);
	if (catip_reboot) {
		pr_info("Reboot from catip\n");
		return catip_reboot;
	}

	wdt_reboot = wdt_reboot & (1 << 7);
	if (wdt_reboot) {
		pr_info("Reboot from watchdog\n");
		return catip_reboot;
	}

	pr_info("Reboot with unknown events!\n");

	return 0;
}
EXPORT_SYMBOL(print_reboot_event);
#endif

static int __init mips_reboot_setup(void)
{
	_machine_restart = mips_machine_restart;
	_machine_halt = mips_machine_halt;
	pm_power_off = mips_machine_power_off;

#ifndef CONFIG_SF16A18_MPW0
	print_reboot_event();
#endif
	return 0;
}
arch_initcall(mips_reboot_setup);

static LIST_HEAD(rst_list_head);
struct sf_rst_offset {
	struct list_head list;
	unsigned int rst_offset;
	int cnt; //the num of module
	spinlock_t    lock;
};

static struct sf_rst_offset *get_rst_by_offset(unsigned int offset)
{
	struct sf_rst_offset *cur;
	struct list_head *head;

	list_for_each(head, &rst_list_head) {
		cur = list_entry(head, struct sf_rst_offset, list);
		if(cur->rst_offset == offset) {
			return cur;
		}
	}

	return NULL;
}
unsigned int sf_get_sysm_offset(unsigned int offset)
{
	int i = 0;
	for( ; i < ARRAY_SIZE(resets); i++)
		if(resets[i].soft_reset == offset)
			return resets[i].sysm_reset;
	return 0;
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to config ethernet ominiphy                                                          *
 *                                                                                                 *
 ***************************************************************************************************/
static void sys_omini_phy_reset(void)
{
	int phy_cfg_mode, phy_duplex, phy_speed;
	//emac2_cfg_np_msg_code[2:0]
	//emac[5-1]_cfg_power_down[4:0] Default value of True power down bit (Reg24.0)
	writeb(0x0,(void*)(OMINI_PHY_CFG_POWERDOWN));
	//emac3_cfg_np_msg_code[2:0]
	//emac[5-1]_cfg_powerup_reset[4:0] Power Up Reset bit(Reg24.1)
	writeb(0x0,(void*)(OMINI_PHY_CFG_POWERUP_RESET));
	//emac1_cfg_mode[2:0] emac1_cfg_phy_addr[4:0]
	//001 - 10Base-T, Full Duplex, Auto negotiation disabled 0x20
	//111 - All capable, Full Duplex, 10 & 100 BT, Auto negotiation enabled AutoMDIX enable --0xE0
	//011 - 100Base-TX, Full Duplex, Auto-negotiation disabled --0x60
	//000 - 10BaseT, Half Duplex, Auto negotiation disabled
	//010 - 100Base-TX, Half Duplex, Auto-negotiation disabled
	//100 - 100Base-Tx, Half Duplex, Auto-negotaition Enabled
	//101 - Repeater mode,100Base-Tx, Half Duplex,Auto-negotiation Enabled
	//110 - Power down mode,In this mode phy wake up in power down mode

	phy_cfg_mode = OMINI_PHY_CFG_MODE_AUTO_FULL;
#ifdef NPU_ETH_DUPLEX
	phy_duplex = NPU_ETH_DUPLEX;
#else
	phy_duplex = OMINI_PHY_DUPLEX_FULL;
#endif

#ifdef NPU_ETH_SPEED
	phy_speed = NPU_ETH_SPEED;
#else
	phy_speed = OMINI_SPEED_AUTO;
#endif

	if(phy_duplex == OMINI_PHY_DUPLEX_FULL){
		if(phy_speed == OMINI_SPEED_100) phy_cfg_mode = OMINI_PHY_CFG_MODE_100M_FULL;
		if(phy_speed == OMINI_SPEED_10) phy_cfg_mode = OMINI_PHY_CFG_MODE_10M_FULL;
		if(phy_speed == OMINI_SPEED_AUTO) phy_cfg_mode = OMINI_PHY_CFG_MODE_AUTO_FULL;
	}else{
		if(phy_speed == OMINI_SPEED_100) phy_cfg_mode = OMINI_PHY_CFG_MODE_100M_HALF;
		if(phy_speed == OMINI_SPEED_10) phy_cfg_mode = OMINI_PHY_CFG_MODE_10M_HALF;
		if(phy_speed == OMINI_SPEED_AUTO) phy_cfg_mode = OMINI_PHY_CFG_MODE_AUTO_HALF;
	}

	writeb(OMINI_PHY1_ADDR | phy_cfg_mode,(void*)(OMINI_PHY1_CFG_ADDR));
	writeb(OMINI_PHY2_ADDR | phy_cfg_mode,(void*)(OMINI_PHY2_CFG_ADDR));
	writeb(OMINI_PHY3_ADDR | phy_cfg_mode,(void*)(OMINI_PHY3_CFG_ADDR));
	writeb(OMINI_PHY4_ADDR | phy_cfg_mode,(void*)(OMINI_PHY4_CFG_ADDR));
	writeb(OMINI_PHY5_ADDR | phy_cfg_mode,(void*)(OMINI_PHY5_CFG_ADDR));
	//OMINI_PHY1_CFG_FX_PLL_MODE
	/*
	   {
	   emac2_cfg_automdix_en, 1
	   emac1_cfg_en_high,  0
	   emac1_cfg_fx_mode,  0
	   emac1_cfg_adc_bp,   0            bypass mode. (Reg18.8)
	   emac1_cfg_pll_bp,   0            bypass mode (Reg18.9)
	   emac1_cfg_smii_source_sync, 0    smii source sync register Only relevant for SMII mode
	   emac1_cfg_mii_mode[1:0],  00           “00” for MII mode,"01" for RMII mode,"10" for SMII,"11" reserved
	   }
	   */
	writeb(0x80,(void*)(OMINI_PHY1_CFG_FX_PLL_MODE));
	writeb(0x80,(void*)(OMINI_PHY2_CFG_FX_PLL_MODE));
	writeb(0x80,(void*)(OMINI_PHY3_CFG_FX_PLL_MODE));
	writeb(0x80,(void*)(OMINI_PHY4_CFG_FX_PLL_MODE));
	writeb(0x80,(void*)(OMINI_PHY5_CFG_FX_PLL_MODE));
	//emac4_cfg_np_msg_code[2:0]
	//emac[5-1]_cfg_clk_freq
	//‘0’ for 25 MHz clock input; ‘1’ for 50 MHz clock input.
	writeb(0x00,(void*)(OMINI_PHY_CFG_CLK_FREQ));
	//emac5_cfg_np_msg_code[2:0]
	//emac[5-1]_cfg_ref_clk_sel(NA now)
	writeb(0x00,(void*)(OMINI_PHY_CFG_CLK_REF_SEL));

	//emac1_cfg_phy_id[7:0]
	writeb(OMINI_PHY_ID_1 & 0xFF,(void*)(OMINI_PHY1_CFG_PHY_ID_LOW8));
	//emac1_cfg_phy_id[15:8]
	writeb(((OMINI_PHY_ID_1 & 0xFF00) >> 8),(void*)(OMINI_PHY1_CFG_PHY_ID_HIGH8));
	//{emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]}
	writeb(((OMINI_PHY_ID_1 & 0x3F0000) >> 16) | ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6),(void*)(OMINI_PHY1_CFG_MODEL_NR));
	//{emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]}
	writeb((OMINI_PHY_REV_NUMBER << 4) | ((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2),(void*)(OMINI_PHY1_CFG_REV_NR));


	//emac1_cfg_phy_id[7:0]
	writeb(OMINI_PHY_ID_2 & 0xFF,(void*)(OMINI_PHY2_CFG_PHY_ID_LOW8));
	//emac1_cfg_phy_id[15:8]
	writeb(((OMINI_PHY_ID_2 & 0xFF00) >> 8),(void*)(OMINI_PHY2_CFG_PHY_ID_HIGH8));
	//{emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]}
	writeb(((OMINI_PHY_ID_2 & 0x3F0000) >> 16) | ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6),(void*)(OMINI_PHY2_CFG_MODEL_NR));
	//{emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]}
	writeb((OMINI_PHY_REV_NUMBER << 4) | ((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2),(void*)(OMINI_PHY2_CFG_REV_NR));


	//emac1_cfg_phy_id[7:0]
	writeb(OMINI_PHY_ID_3 & 0xFF,(void*)(OMINI_PHY3_CFG_PHY_ID_LOW8));
	//emac1_cfg_phy_id[15:8]
	writeb(((OMINI_PHY_ID_3 & 0xFF00) >> 8),(void*)(OMINI_PHY3_CFG_PHY_ID_HIGH8));
	//{emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]}
	writeb(((OMINI_PHY_ID_3 & 0x3F0000) >> 16) | ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6),(void*)(OMINI_PHY3_CFG_MODEL_NR));
	//{emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]}
	writeb((OMINI_PHY_REV_NUMBER << 4) | ((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2),(void*)(OMINI_PHY3_CFG_REV_NR));

	//emac1_cfg_phy_id[7:0]
	writeb(OMINI_PHY_ID_4 & 0xFF,(void*)(OMINI_PHY4_CFG_PHY_ID_LOW8));
	//emac1_cfg_phy_id[15:8]
	writeb(((OMINI_PHY_ID_4 & 0xFF00) >> 8),(void*)(OMINI_PHY4_CFG_PHY_ID_HIGH8));
	//{emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]}
	writeb(((OMINI_PHY_ID_4 & 0x3F0000) >> 16) | ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6),(void*)(OMINI_PHY4_CFG_MODEL_NR));
	//{emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]}
	writeb((OMINI_PHY_REV_NUMBER << 4) | ((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2),(void*)(OMINI_PHY4_CFG_REV_NR));

	//emac1_cfg_phy_id[7:0]
	writeb(OMINI_PHY_ID_5 & 0xFF,(void*)(OMINI_PHY5_CFG_PHY_ID_LOW8));
	//emac1_cfg_phy_id[15:8]
	writeb(((OMINI_PHY_ID_5 & 0xFF00) >> 8),(void*)(OMINI_PHY5_CFG_PHY_ID_HIGH8));
	//{emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]}
	writeb(((OMINI_PHY_ID_5 & 0x3F0000) >> 16) | ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6),(void*)(OMINI_PHY5_CFG_MODEL_NR));
	//{emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]}
	writeb((OMINI_PHY_REV_NUMBER << 4) | ((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2),(void*)(OMINI_PHY5_CFG_REV_NR));
}

static void sys_init_dma(void)
{
	writew(0x1,(void*)( DMA_BOOT_MANAGER_NS));
	//writew(0xFF,(void*)( DMA_BOOT_FROM_PC));
	writew(0xFF,(void*)( DMA_BOOT_IRQ_NS70));
	writew(0xFF,(void*)( DMA_BOOT_IRQ_NS158));
	writew(0xFF,(void*)( DMA_BOOT_IRQ_NS2316));
	writew(0xFF,(void*)( DMA_BOOT_PERIPH_NS70));
	writew(0xFF,(void*)( DMA_BOOT_PERIPH_NS158));
	writew(0xFF,(void*)( DMA_BOOT_PERIPH_NS2316));
	//writew(0xFF,(void*)( DMA_BOOT_ADDR70));
	//writew(0xFF,(void*)( DMA_BOOT_ADDR158));
	//writew(0xFF,(void*)( DMA_BOOT_ADDR2316));
	//writew(0xFF,(void*)( DMA_BOOT_ADDR3124));
}

static void sys_init_emac(unsigned int initvalue)
{
#ifndef CONFIG_SF16A18_MPW0
	/* select gmac module */
	writew(readw((void *)SYS_MISC_CTRL) & ~(1 << 2), (void *)SYS_MISC_CTRL);
#endif
	if(initvalue == 0){
		//for RMII init
		writew((GMAC_PHY_INTF_SEL_RMII & GMAC_PHY_INTF_SEL_MASK) |
			CLK_RMII_OEN,
			(void *)EMAC_PHY_INTF_SEL_I);
	}else if(initvalue == 1){
		//for RGMII Init
		writew((GMAC_PHY_INTF_SEL_RGMII & GMAC_PHY_INTF_SEL_MASK),
			(void *)EMAC_PHY_INTF_SEL_I);
	}else if(initvalue == 2){
		//for RGMII Init
		writew((GMAC_PHY_INTF_SEL_GMII_MII & GMAC_PHY_INTF_SEL_MASK),
			(void *)EMAC_PHY_INTF_SEL_I);
	}else{
		WARN_ON(1);
	}
}

static int node_init(struct sf_rst_offset **rst_node, unsigned int offset)
{
	*rst_node = kmalloc(sizeof(struct sf_rst_offset), GFP_KERNEL);
	if (!*rst_node)
		return -ENOMEM;

	spin_lock_init(&(*rst_node)->lock);
	(*rst_node)->rst_offset = offset;
	(*rst_node)->cnt = 0;
	list_add(&(*rst_node)->list, &rst_list_head);

	return 0;
}

//sometime we need initvalue by some module init
int release_reset_with_value(unsigned int offset,unsigned int initvalue)
{
	struct sf_rst_offset *rst_node;

	rst_node = get_rst_by_offset(offset);
	if (rst_node) {
		if (rst_node->cnt)
			goto done;
	} else {
		if (node_init(&rst_node, offset) != 0)
			return -ENOMEM;
	}

	spin_lock(&rst_node->lock);
	switch (rst_node->rst_offset) {
		case SF_NPU_SOFT_RESET:
		case SF_EMAC_SOFT_RESET:
		case SF_CATIP_SOFT_RESET:
		case SF_WIFI_1_SOFT_RESET:
		case SF_WIFI_2_SOFT_RESET:
		case SF_USB_SOFT_RESET:
			/* clear internal memory shutdown/sleep */
			writel(0, (void*)(SIFLOWER_SYSCTL_BASE + rst_node->rst_offset + MEMCTL_OFFSET));
			break;
		default:
			break;
	}
	if (rst_node->rst_offset == SF_GDMA_SOFT_RESET)
		sys_init_dma();
	writel(0xFF, (void*)(SIFLOWER_SYSCTL_BASE + rst_node->rst_offset + CLK_EN_OFFSET));
	if (rst_node->rst_offset == SF_IRAM_SOFT_RESET){
		writel(0xFF, (void*)(SIFLOWER_SYSCTL_BASE + rst_node->rst_offset + IRAM_CLK1_EN_OFFSET));
	}
	udelay(LATENCY);
	if (rst_node->rst_offset == SF_NPU_SOFT_RESET){
		writeb(0x1F,(void*)(OMINI_PHY_ENABLE));
		udelay(LATENCY);
		sys_omini_phy_reset();
	}
	if (rst_node->rst_offset == SF_EMAC_SOFT_RESET)
		sys_init_emac(initvalue);
	writel(0x0, (void*)(SIFLOWER_SYSCTL_BASE + rst_node->rst_offset));
	if (rst_node->rst_offset == SF_IRAM_SOFT_RESET){
		writel(0x0, (void*)(SIFLOWER_SYSCTL_BASE + rst_node->rst_offset + IRAM_SOFT_RESET1_OFFSET));
	}
	udelay(LATENCY);
	writel(0xF, (void*)(SIFLOWER_SYSCTL_BASE + rst_node->rst_offset + BOE_OFFSET));
	spin_unlock(&rst_node->lock);

#ifndef CONFIG_SF16A18_MPW0
	if (rst_node->rst_offset == SF_CATIP_SOFT_RESET)
		writel(0x0, (void*)(SIFLOWER_SYSCTL_BASE + CATIP_MAIN_CLK_DEBUG_EN));
#endif
done:
	rst_node->cnt++;
	return 0;
}
EXPORT_SYMBOL(release_reset_with_value);

int release_reset(unsigned int offset)
{
	//don't care about init value
	return release_reset_with_value(offset,0);
}
EXPORT_SYMBOL(release_reset);

int hold_reset(unsigned int offset)
{
	struct sf_rst_offset *rst_node;

	rst_node = get_rst_by_offset(offset);
	if (rst_node) {
		/* cnt = 0 or 1 will do hold reset */
		if (rst_node->cnt > 0)
			rst_node->cnt--;

		if (rst_node->cnt != 0)
			return 0;
	} else {
		if (node_init(&rst_node, offset) != 0)
			return -ENOMEM;
	}

	spin_lock(&rst_node->lock);
	switch (rst_node->rst_offset) {
		case SF_NPU_SOFT_RESET:
		case SF_EMAC_SOFT_RESET:
		case SF_CATIP_SOFT_RESET:
		case SF_WIFI_1_SOFT_RESET:
		case SF_WIFI_2_SOFT_RESET:
		case SF_USB_SOFT_RESET:
			/* set internal memory shutdown */
			writel(BIT(1), (void*)(SIFLOWER_SYSCTL_BASE + rst_node->rst_offset + MEMCTL_OFFSET));
			break;
		default:
			break;
	}
	writel(0xFF, (void*)(SIFLOWER_SYSCTL_BASE + rst_node->rst_offset));
	if (rst_node->rst_offset == SF_IRAM_SOFT_RESET) {
		writel(0xFF, (void*)(SIFLOWER_SYSCTL_BASE + rst_node->rst_offset + IRAM_SOFT_RESET1_OFFSET));
	}

	writel(0x0, (void*)(SIFLOWER_SYSCTL_BASE + rst_node->rst_offset + CLK_EN_OFFSET));
	if (rst_node->rst_offset == SF_IRAM_SOFT_RESET) {
		writel(0x0, (void*)(SIFLOWER_SYSCTL_BASE + rst_node->rst_offset + IRAM_CLK1_EN_OFFSET));
	}

	writel(0x0, (void*)(SIFLOWER_SYSCTL_BASE + rst_node->rst_offset + BOE_OFFSET));
	spin_unlock(&rst_node->lock);

	return 0;
}
EXPORT_SYMBOL(hold_reset);

u32 get_module_clk_gate(u32 soft_reset_offset, bool is_iram_clk1)
{
	if(!is_iram_clk1)
		return readl((void*)(SIFLOWER_SYSCTL_BASE + soft_reset_offset + CLK_EN_OFFSET));
	else if(soft_reset_offset == SF_IRAM_SOFT_RESET)
		return readl((void*)(SIFLOWER_SYSCTL_BASE + soft_reset_offset + IRAM_CLK1_EN_OFFSET));
	else
		printk(KERN_ERR "Bad usage of get_module_clk_gate, the 1st parameter must be SF_IRAM_SOFT_RESET" \
				"when the 2nd parameter is true.\n");
	return 0;
}

EXPORT_SYMBOL(get_module_clk_gate);

void set_module_clk_gate(u32 soft_reset_offset, u32 val, bool is_iram_clk1)
{
	if(!is_iram_clk1)
		writel((val & 0xFF), (void *)(SIFLOWER_SYSCTL_BASE + soft_reset_offset + CLK_EN_OFFSET));
	else if(soft_reset_offset == SF_IRAM_SOFT_RESET)
		writel((val & 0xFF), (void *)(SIFLOWER_SYSCTL_BASE + soft_reset_offset + IRAM_CLK1_EN_OFFSET));
	else
		printk(KERN_ERR "Bad usage of get_module_clk_gate, the 1st parameter must be SF_IRAM_SOFT_RESET" \
				"when the 3rd parameter is true.\n");
}
EXPORT_SYMBOL(set_module_clk_gate);
