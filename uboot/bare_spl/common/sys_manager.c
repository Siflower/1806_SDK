#include <types.h>
#include <io.h>
#include <sf_mmap.h>
#include "sys_manager.h"
#include <errorno.h>

int sys_apb_reset_i2s(void)
{
	writew(0x0, IIS_SYSM_RESET);
	writew(0x1, IIS_SYSM_RESET);

	writew(0xFF, IIS_SOFT_CLK_EN);

	writew(0xFF, IIS_SOFT_RESET);
	writew(0x00, IIS_SOFT_RESET);

	writew(0x0F, IIS_SOFT_BOE);

	return 0;
}

int sys_apb_reset_i2c(void)
{
	writew(0x0, IIC_SYSM_RESET);
	writew(0x1, IIC_SYSM_RESET);

	writew(0xFF, IIC_SOFT_CLK_EN);

	writew(0xFF, IIC_SOFT_RESET);
	writew(0x00, IIC_SOFT_RESET);

	writew(0x0F, IIC_SOFT_BOE);

	return 0;
}

int sys_apb_reset_spi(void)
{
	writew(0x0, SPI_SYSM_RESET);
	writew(0x1, SPI_SYSM_RESET);

	writew(0xFF, SPI_SOFT_CLK_EN);

	writew(0xFF, SPI_SOFT_RESET);
	writew(0x00, SPI_SOFT_RESET);

	writew(0x0F, SPI_SOFT_BOE);

	return 0;
}

int sys_apb_reset_uart(void)
{
	writew(0x0, UART_SYSM_RESET);
	writew(0x1, UART_SYSM_RESET);

	writew(0xFF, UART_SOFT_CLK_EN);

	writew(0xFF, UART_SOFT_RESET);
	writew(0x00, UART_SOFT_RESET);

	writew(0x0F, UART_SOFT_BOE);

	return 0;
}

int sys_apb_reset_pcm(void)
{
	writew(0x0, PCM_SYSM_RESET);
	writew(0x1, PCM_SYSM_RESET);

	writew(0xFF, PCM_SOFT_CLK_EN);

	writew(0xFF, PCM_SOFT_RESET);
	writew(0x00, PCM_SOFT_RESET);

	writew(0x0F, PCM_SOFT_BOE);

	return 0;
}

int sys_apb_reset_pwm(void)
{
	writew(0x0, PWM_SYSM_RESET);
	writew(0x1, PWM_SYSM_RESET);

	writew(0xFF, PWM_SOFT_CLK_EN);

	writew(0xFF, PWM_SOFT_RESET);
	writew(0x00, PWM_SOFT_RESET);

	writew(0x0F, PWM_SOFT_BOE);

	return 0;
}

int sys_apb_reset_timer(void)
{
	writew(0x0, TIMER_SYSM_RESET);
	writew(0x1, TIMER_SYSM_RESET);

	writew(0xFF, TIMER_SOFT_CLK_EN);

	writew(0xFF, TIMER_SOFT_RESET);
	writew(0x00, TIMER_SOFT_RESET);

	writew(0x0F, TIMER_SOFT_BOE);

	return 0;
}

int sys_apb_reset_watchdog(void)
{
	writew(0x0, WDT_SYSM_RESET);
	writew(0x1, WDT_SYSM_RESET);

	writew(0xFF, WDT_SOFT_CLK_EN);

	writew(0xFF, WDT_SOFT_RESET);
	writew(0x00, WDT_SOFT_RESET);

	writew(0x0F, WDT_SOFT_BOE);

	return 0;
}

int sys_apb_reset_spdif(void)
{
	writew(0x0, SPDIF_SYSM_RESET);
	writew(0x1, SPDIF_SYSM_RESET);

	writew(0xFF, SPDIF_SOFT_CLK_EN);

	writew(0xFF, SPDIF_SOFT_RESET);
	writew(0x00, SPDIF_SOFT_RESET);

	writew(0x0F, SPDIF_SOFT_BOE);

	return 0;
}

int sys_gpio_reset(void)
{
	writew(0x0, GPIO_SYSM_RESET);
	writew(0x1, GPIO_SYSM_RESET);

	writew(0xFF, GPIO_SOFT_CLK_EN);

	writew(0xFF, GPIO_SOFT_RESET);
	writew(0x00, GPIO_SOFT_RESET);

	writew(0x0F, GPIO_SOFT_BOE);

	return 0;
}

int sys_apb_bus_reset(void)
{
	writew(0x0, PBUS_SYSM_RESET);
	writew(0x1, PBUS_SYSM_RESET);

	return 0;
}

int sys_apb_modules_reset(void)
{
	sys_apb_bus_reset();
#ifndef SF19A28
	sys_apb_reset_i2s();
	sys_apb_reset_pcm();
	sys_apb_reset_spdif();
#endif
	sys_apb_reset_i2c();
	sys_apb_reset_spi();
	sys_apb_reset_uart();
	sys_apb_reset_pwm();
	sys_apb_reset_timer();
	sys_apb_reset_watchdog();

	return 0;
}

int sys_dma_reset(void)
{
	writew(0x0, DMA_SYSM_RESET);
	writew(0x1, DMA_SYSM_RESET);

	writew(0x1, DMA_BOOT_MANAGER_NS);
	writew(0xFF, DMA_BOOT_IRQ_NS70);
	writew(0xFF, DMA_BOOT_IRQ_NS158);
	writew(0xFF, DMA_BOOT_IRQ_NS2316);
	writew(0xFF, DMA_BOOT_PERIPH_NS70);
	writew(0xFF, DMA_BOOT_PERIPH_NS158);
	writew(0xFF, DMA_BOOT_PERIPH_NS2316);

	writew(0xFF, DMA_SOFT_CLK_EN);

	writew(0xFF, DMA_SOFT_RESET);
	writew(0x00, DMA_SOFT_RESET);

	writew(0x0F, DMA_SOFT_BOE);

	return 0;
}

enum bus_sel {
	gmac,
	gdu,
};

void sys_module_select(enum bus_sel module)
{
	u32 tmp = readl(SYS_MISC_CTRL);

	switch (module) {
	case gdu:
		tmp |= 1 << 2;
		break;
	case gmac:
		tmp &= ~(1 << 2);
		break;
	default:
		break;
	}
	writel(tmp, SYS_MISC_CTRL);
}

#define GMAC_PHY_INTF_SEL_GMII_MII 0x0
#define GMAC_PHY_INTF_SEL_RGMII    0x1
#define GMAC_PHY_INTF_SEL_SGMII    0x2
#define GMAC_PHY_INTF_SEL_TBI      0x3
#define GMAC_PHY_INTF_SEL_RMII     0x4
#define GMAC_PHY_INTF_SEL_RTBI     0x5
#define GMAC_PHY_INTF_SEL_SMII     0x6
#define GMAC_PHY_INTF_SEL_REVMII   0x7
#define GMAC_PHY_INTF_SEL_MASK     0x7
#define CLK_RMII_OEN		(1 << 5)

int sys_gmac_reset(void)
{
#define RGMII	1
	sys_module_select(gmac);

	writew(0x0, EMAC_SYSM_RESETN);
	writew(0x1, EMAC_SYSM_RESETN);

	writew(0xFF, EMAC_SOFT_CLKGATE_EN);

	writew(0xFF, EMAC_SOFT_RESET);

#ifdef GMII
	writew((GMAC_PHY_INTF_SEL_GMII_MII & GMAC_PHY_INTF_SEL_MASK),
			EMAC_PHY_INTF_SEL_I);
#elif defined(RMII)
	writew((GMAC_PHY_INTF_SEL_RMII & GMAC_PHY_INTF_SEL_MASK) | CLK_RMII_OEN,
			EMAC_PHY_INTF_SEL_I);
#elif defined(RGMII)
	writew((GMAC_PHY_INTF_SEL_RGMII & GMAC_PHY_INTF_SEL_MASK),
			EMAC_PHY_INTF_SEL_I);
#endif

/* should be the same with linux-3.18.29/arch/mips/siflower/reset.c
 * sys_init_emac func. */
#ifdef RGMII
	// set TX/RX CLK delay
	// x(1 <= x <= 256) means to delay (x * 0.04)ns
	writew(0x30, EMAC_CLK_TX_I_DLY);
	writew(0x40, EMAC_CLK_PHY_RX_I_DLY);
	writew(0x1, EMAC_CLK_PHY_RX_I_DLY_EN);
#endif
	writew(0x00, EMAC_SOFT_RESET);
	writew(0x0F, EMAC_SOFT_BOE);

	return 0;
}

int sys_usb_reset(void)
{
	writew(0x0, USB_SYSM_RESET);
	writew(0x1, USB_SYSM_RESET);

	writew(0xFF, USB_SOFT_CLK_EN);

	writew(0xFF, USB_SOFT_RESET);
	writew(0x00, USB_SOFT_RESET);

	writew(0x0F, USB_SOFT_BOE);

	return 0;
}

int sys_emmc_reset(void)
{
	writew(0x0, EMMC_SYSM_RESET);
	writew(0x1, EMMC_SYSM_RESET);

	writew(0xFF, EMMC_SOFT_CLK_EN);

	writew(0xFF, EMMC_SOFT_RESET);
	writew(0x00, EMMC_SOFT_RESET);

	writew(0x0F, EMMC_SOFT_BOE);

	return 0;
}

int sys_sdio_reset(void)
{
	writew(0x0, SDIO_SYSM_RESET);
	writew(0x1, SDIO_SYSM_RESET);

	writew(0xFF, SDIO_SOFT_CLK_EN);

	writew(0xFF, SDIO_SOFT_RESET);
	writew(0x00, SDIO_SOFT_RESET);

	writew(0x0F, SDIO_SOFT_BOE);
#ifdef MPW0
	writew(0xFF, SDIO_SW_MSC_REG);
#else
	writew(0, SDIO_SW_MSC_REG);
#endif
	return 0;
}

int sys_gdu_reset(void)
{
	sys_module_select(gdu);

	writew(0x0, BUS1_BM3_SYSM_RESET);
	writew(0x1, BUS1_BM3_SYSM_RESET);

	writew(0xFF, GDU_SOFT_CLK_EN);

	writew(0xFF, GDU_SOFT_RESET);
	writew(0x00, GDU_SOFT_RESET);

	writew(0x0F, GDU_SOFT_BOE);

	return 0;
}

int sys_crypto_reset(void)
{
	writew(0x0, CRYPTO_SYSM_RESET);
	writew(0x1, CRYPTO_SYSM_RESET);

	writew(0xFF, CRYPTO_SOFT_CLK_EN);

	writew(0xFF, CRYPTO_SOFT_RESET);
	writew(0x00, CRYPTO_SOFT_RESET);

	writew(0x0F, CRYPTO_SOFT_BOE);

	return 0;
}

int sys_wifi_scdma_reset(void)
{
#ifdef MPW0
	writew(0x0, SCDMA_SYSM_RESET);
	writew(0x1, SCDMA_SYSM_RESET);

	writew(0xFF, SCDMA_SOFT_CLK_EN);

	writew(0xFF, SCDMA_SOFT_RESET);
	writew(0x00, SCDMA_SOFT_RESET);

	writew(0x0F, SCDMA_SOFT_BOE);
#endif
	return 0;
}

int sys_wifi_1_reset(void)
{
	writew(0x0, WIFI_1_SYSM_RESET);
	writew(0x1, WIFI_1_SYSM_RESET);

	writew(0xFF, WIFI_1_SOFT_CLK_EN);

	writew(0xFF, WIFI_1_SOFT_RESET);
	writew(0x00, WIFI_1_SOFT_RESET);

	writew(0x0F, WIFI_1_SOFT_BOE);

	return 0;
}

int sys_wifi_2_reset(void)
{
	writew(0x0, WIFI_2_SYSM_RESET);
	writew(0x1, WIFI_2_SYSM_RESET);

	writew(0xFF, WIFI_2_SOFT_CLK_EN);

	writew(0xFF, WIFI_2_SOFT_RESET);
	writew(0x00, WIFI_2_SOFT_RESET);

	writew(0x0F, WIFI_2_SOFT_BOE);

	return 0;
}

int sys_catip_reset(void)
{
	writew(0x0, CATIP_SYSM_RESET);
	writew(0x1, CATIP_SYSM_RESET);

	writew(0xFF, CATIP_SOFT_CLK_EN);

	writew(0xFF, CATIP_SOFT_RESET);
	writew(0x00, CATIP_SOFT_RESET);

	writew(0x0F, CATIP_SOFT_BOE);

#ifndef MPW0
	//disable the main clk output debug enable
	writel(0x0, (void*)CATIP_MAIN_CLK_DEBUG_EN);
#endif

	return 0;
}

int sys_wifi_reset(void)
{
	sys_wifi_scdma_reset();
	sys_wifi_1_reset();
	sys_wifi_2_reset();
	sys_catip_reset();

	return 0;
}

int memory_soft_reset(void)
{
	/* reset ddr reg, bit 1 */
	writew(0x1, PBUS_RESET);
	/* reset bus cpu to ddr, bit 1 */
	writew(0x1, CPU_RESET);
	/* reset memory */
	writew(0x0, MEM_RESET);
	writew(0x1, MEM_RESET);
	writew(0x3, CPU_RESET);
	writew(0x3, PBUS_RESET);

	return 0;
}

/**************************************************************************************************
*                                                                                                 *
* -Description:                                                                                   *
*    This part is used to reset bus.                                                           *
*                                                                                                 *
***************************************************************************************************/
int bus_soft_reset(void)
{
	writew(0x0, BUS1_RESET);
	writew(0x1, BUS1_RESET);

	writew(0x0, BUS2_RESET);
	writew(0x1, BUS2_RESET);

	writew(0x0, BUS3_RESET);
	writew(0x1, BUS3_RESET);

	return 0;
}

int io_soft_reset(void)
{
	writew(0x0, IO_SYSM_RESET);
	writew(0x1, IO_SYSM_RESET);

	return 0;
}

static inline int sys_remap_n_enable(void)
{
	/* used to access block devices(except sdio) + npu */
	writel(readl(SYS_MISC_CTRL) | 0x1, SYS_MISC_CTRL);

	return 0;
}

//[4:0] PHY ADD register field
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
// e0 means auto mode
#define OMINI_PHY_CFG_MODE 0xE0

int sys_omini_phy_reset(void)
{
	/*
	 * emac2_cfg_np_msg_code[2:0]
	 * emac[5-1]_cfg_power_down[4:0]
	 * Default value of True power down bit (Reg24.0)
	 */
	writeb(0x0, OMINI_PHY_CFG_POWERDOWN);

	/*
	 * emac3_cfg_np_msg_code[2:0]
	 * emac[5-1]_cfg_powerup_reset[4:0]
	 * Power Up Reset bit(Reg24.1)
	 */
	writeb(0x0, OMINI_PHY_CFG_POWERUP_RESET);

	/*
	 * emac1_cfg_mode[2:0] emac1_cfg_phy_addr[4:0]
	 * emac1_cfg_mode[2:0]
	 *	001 - 10Base-T, Full Duplex, Auto negotiation disabled 0x20
	 * emac1_cfg_mode[2:0]
	 *	111 All capable, Full Duplex, 10 & 100 BT,
	 *	Auto negotiation enabled AutoMDIX enable --0xE0
	 * emac1_cfg_mode[2:0]
	 *	011 100Base-TX, Full Duplex, Auto-negotiation disabled --0x60
	 */
	writeb(OMINI_PHY1_ADDR | OMINI_PHY_CFG_MODE, OMINI_PHY1_CFG_ADDR);
	writeb(OMINI_PHY2_ADDR | OMINI_PHY_CFG_MODE, OMINI_PHY2_CFG_ADDR);
	writeb(OMINI_PHY3_ADDR | OMINI_PHY_CFG_MODE, OMINI_PHY3_CFG_ADDR);
	writeb(OMINI_PHY4_ADDR | OMINI_PHY_CFG_MODE, OMINI_PHY4_CFG_ADDR);
	writeb(OMINI_PHY5_ADDR | OMINI_PHY_CFG_MODE, OMINI_PHY5_CFG_ADDR);
	writeb(0x80, OMINI_PHY1_CFG_FX_PLL_MODE);
	writeb(0x80, OMINI_PHY2_CFG_FX_PLL_MODE);
	writeb(0x80, OMINI_PHY3_CFG_FX_PLL_MODE);
	writeb(0x80, OMINI_PHY4_CFG_FX_PLL_MODE);
	writeb(0x80, OMINI_PHY5_CFG_FX_PLL_MODE);

	/*
	 * emac4_cfg_np_msg_code[2:0]
	 * emac[5-1]_cfg_clk_freq
	 * ‘0’ for 25 MHz clock input; ‘1’ for 50 MHz clock input.
	 */
	writeb(0x00, OMINI_PHY_CFG_CLK_FREQ);

	/*
	 * emac5_cfg_np_msg_code[2:0]
	 * emac[5-1]_cfg_ref_clk_sel(NA now)
	 */
	writeb(0x00, OMINI_PHY_CFG_CLK_REF_SEL);

	/* mac1_cfg_phy_id[7:0] */
	writeb(OMINI_PHY_ID_1 & 0xFF, OMINI_PHY1_CFG_PHY_ID_LOW8);

	/* mac1_cfg_phy_id[15:8] */
	writeb(((OMINI_PHY_ID_1 & 0xFF00) >> 8), OMINI_PHY1_CFG_PHY_ID_HIGH8);

	/* {emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]} */
	writeb(((OMINI_PHY_ID_1 & 0x3F0000) >> 16) |
	       ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6), OMINI_PHY1_CFG_MODEL_NR);

	/* {emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]} */
	writeb((OMINI_PHY_REV_NUMBER << 4) |
		((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2), OMINI_PHY1_CFG_REV_NR);


	/* emac1_cfg_phy_id[7:0] */
	writeb(OMINI_PHY_ID_2 & 0xFF, OMINI_PHY2_CFG_PHY_ID_LOW8);

	/* emac1_cfg_phy_id[15:8] */
	writeb(((OMINI_PHY_ID_2 & 0xFF00) >> 8), OMINI_PHY2_CFG_PHY_ID_HIGH8);

	/* {emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]} */
	writeb(((OMINI_PHY_ID_2 & 0x3F0000) >> 16) |
	       ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6), OMINI_PHY2_CFG_MODEL_NR);

	/* {emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]} */
	writeb((OMINI_PHY_REV_NUMBER << 4) |
		((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2), OMINI_PHY2_CFG_REV_NR);


	/* emac1_cfg_phy_id[7:0] */
	writeb(OMINI_PHY_ID_3 & 0xFF, OMINI_PHY3_CFG_PHY_ID_LOW8);

	/* emac1_cfg_phy_id[15:8] */
	writeb(((OMINI_PHY_ID_3 & 0xFF00) >> 8), OMINI_PHY3_CFG_PHY_ID_HIGH8);

	/* {emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]} */
	writeb(((OMINI_PHY_ID_3 & 0x3F0000) >> 16) |
	       ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6), OMINI_PHY3_CFG_MODEL_NR);

	/* {emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]} */
	writeb((OMINI_PHY_REV_NUMBER << 4) |
		((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2), OMINI_PHY3_CFG_REV_NR);

	/* emac1_cfg_phy_id[7:0] */
	writeb(OMINI_PHY_ID_4 & 0xFF, OMINI_PHY4_CFG_PHY_ID_LOW8);

	/* emac1_cfg_phy_id[15:8] */
	writeb(((OMINI_PHY_ID_4 & 0xFF00) >> 8), OMINI_PHY4_CFG_PHY_ID_HIGH8);

	/* {emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]} */
	writeb(((OMINI_PHY_ID_4 & 0x3F0000) >> 16) |
	       ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6), OMINI_PHY4_CFG_MODEL_NR);

	/* {emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]} */
	writeb((OMINI_PHY_REV_NUMBER << 4) |
		((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2), OMINI_PHY4_CFG_REV_NR);

	/* emac1_cfg_phy_id[7:0] */
	writeb(OMINI_PHY_ID_5 & 0xFF, OMINI_PHY5_CFG_PHY_ID_LOW8);

	/* emac1_cfg_phy_id[15:8] */
	writeb(((OMINI_PHY_ID_5 & 0xFF00) >> 8), OMINI_PHY5_CFG_PHY_ID_HIGH8);

	/* {emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]} */
	writeb(((OMINI_PHY_ID_5 & 0x3F0000) >> 16) |
	       ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6), OMINI_PHY5_CFG_MODEL_NR);

	/* {emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]} */
	writeb((OMINI_PHY_REV_NUMBER << 4) |
		((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2), OMINI_PHY5_CFG_REV_NR);

	return 0;
}


int sys_npu_reset(void)
{
	writew(0x0, NPU_SYSM_RESET);
	writew(0x1, NPU_SYSM_RESET);

	writew(0xFF, NPU_SOFT_CLK_EN);

	writew(0xFF, NPU_SOFT_RESET);
#ifndef FPGA
	/* emac[5-1]_phy_enable[4:0] */
	writeb(0xFF, OMINI_PHY_ENABLE);
	sys_omini_phy_reset();
#endif
	writew(0x00, NPU_SOFT_RESET);

	writew(0x0F, NPU_SOFT_BOE);

	return 0;
}

static inline int set_clk_ratio(void)
{
	/* FIXME: some fullmask board need some delay
	 * before memory_soft_reset()
	 */
	volatile int i = (1<< 10);

	writel(4, CORE_L2_CLK_RATIO);

	while(i--)
		;

	writel(1, CORE_L2_SET_CLK_RATIO);

	return 0;
}

int sys_reboot(void)
{
	writew(0x01, SOFT_REBOOT);

	return 0;
}

/*
 * TODO: cut down code size.
 */
int sys_init(void)
{
#ifdef SF19A28
	/*
	* record_en from 0 to 1, the sys_events will
	* be record. It's not from 1->0 in irom code
	* when reboot with soft reset or watchdog
	 * */
	u8 record_en;
	record_en = readb(SYS_MISC_CTRL);
	record_en &= ~(0x1 << 1);
	writew(record_en, SYS_MISC_CTRL);
#endif

	/* set clock divider to 1:3 */
	set_clk_ratio();

	memory_soft_reset();
	bus_soft_reset();
	io_soft_reset();
	sys_apb_modules_reset();
	sys_dma_reset();
	sys_npu_reset();
	sys_wifi_reset();
	sys_usb_reset();
	sys_gmac_reset();
#ifndef SF19A28
	sys_emmc_reset();
	sys_sdio_reset();
	// sys_gdu_reset();
	sys_crypto_reset();
#endif
	sys_gpio_reset();
	sys_remap_n_enable();

	return 0;
}
