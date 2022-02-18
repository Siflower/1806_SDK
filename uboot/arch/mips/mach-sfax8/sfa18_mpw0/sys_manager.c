#include <common.h>
#include <asm/io.h>
#include <mach/sfax8.h>

#if defined(CONFIG_SPL_BUILD)
/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset & enabel clk for i2s.                                             *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_apb_reset_i2s(void)
{
	//reset i2s module manager
	writew(0x0,(void*)(IIS_SYSM_RESET));
	writew(0x1,(void*)(IIS_SYSM_RESET));

	//enable i2s module clk
	writew(0xFF,(void*)(IIS_SOFT_CLK_EN));

	//reset i2s module
	writew(0xFF,(void*)(IIS_SOFT_RESET));
	writew(0x00,(void*)(IIS_SOFT_RESET));

	//set i2s boe
	writew(0x0F,(void*)(IIS_SOFT_BOE));
}
/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset & enabel clk for i2c.                                             *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_apb_reset_i2c(void)
{
	//reset i2c module manager
	writew(0x0,(void*)(IIC_SYSM_RESET));
	writew(0x1,(void*)(IIC_SYSM_RESET));

	//enable i2c module clk
	writew(0xFF,(void*)(IIC_SOFT_CLK_EN));

	//reset i2c module
	writew(0xFF,(void*)(IIC_SOFT_RESET));
	writew(0x00,(void*)(IIC_SOFT_RESET));

	//set i2c boe
	writew(0x0F,(void*)(IIC_SOFT_BOE));
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset & enabel clk for spi.                                             *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_apb_reset_spi(void)
{
	//reset spi module manager
	writew(0x0,(void*)(SPI_SYSM_RESET));
	writew(0x1,(void*)(SPI_SYSM_RESET));

	//enable spi module clk
	writew(0xFF,(void*)(SPI_SOFT_CLK_EN));

	//reset spi module
	writew(0xFF,(void*)(SPI_SOFT_RESET));
	writew(0x00,(void*)(SPI_SOFT_RESET));

	//set spi boe
	writew(0x0F,(void*)(SPI_SOFT_BOE));
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset & enabel clk for uart.                                            *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_apb_reset_uart(void)
{
	//reset uart module manager
	writew(0x0,(void*)(UART_SYSM_RESET));
	writew(0x1,(void*)(UART_SYSM_RESET));

	//enable uart module clk
	writew(0xFF,(void*)(UART_SOFT_CLK_EN));

	//reset uart module
	writew(0xFF,(void*)(UART_SOFT_RESET));
	writew(0x00,(void*)(UART_SOFT_RESET));

	//set uart boe
	writew(0x0F,(void*)(UART_SOFT_BOE));
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset & enabel clk for  pcm.                                            *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_apb_reset_pcm(void)
{
	//reset pcm module manager
	writew(0x0,(void*)(PCM_SYSM_RESET));
	writew(0x1,(void*)(PCM_SYSM_RESET));

	//enable pcm module clk
	writew(0xFF,(void*)(PCM_SOFT_CLK_EN));

	//reset pcm module
	writew(0xFF,(void*)(PCM_SOFT_RESET));
	writew(0x00,(void*)(PCM_SOFT_RESET));

	//set pcm boe
	writew(0x0F,(void*)(PCM_SOFT_BOE));
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset & enabel clk for  pwm.                                            *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_apb_reset_pwm(void)
{
	//reset pwm module manager
	writew(0x0,(void*)(PWM_SYSM_RESET));
	writew(0x1,(void*)(PWM_SYSM_RESET));

	//enable pwm module clk
	writew(0xFF,(void*)(PWM_SOFT_CLK_EN));

	//reset pwm module
	writew(0xFF,(void*)(PWM_SOFT_RESET));
	writew(0x00,(void*)(PWM_SOFT_RESET));

	//set pwm boe
	writew(0x0F,(void*)(PWM_SOFT_BOE));
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset & enabel clk for timer.                                           *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_apb_reset_timer(void)
{
	//reset timer module manager
	writew(0x0,(void*)(TIMER_SYSM_RESET));
	writew(0x1,(void*)(TIMER_SYSM_RESET));

	//enable timer module clk
	writew(0xFF,(void*)(TIMER_SOFT_CLK_EN));

	//reset timerB module
	writew(0xFF,(void*)(TIMER_SOFT_RESET));
	writew(0x00,(void*)(TIMER_SOFT_RESET));

	//set timer boe
	writew(0x0F,(void*)(TIMER_SOFT_BOE));
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset & enabel clk for watchdog.                                        *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_apb_reset_watchdog(void)
{
	//reset watchdog module manager
	writew(0x0,(void*)(WDT_SYSM_RESET));
	writew(0x1,(void*)(WDT_SYSM_RESET));

	//enable watchdog module clk
	writew(0xFF,(void*)(WDT_SOFT_CLK_EN));

	//reset watchdog module
	writew(0xFF,(void*)(WDT_SOFT_RESET));
	writew(0x00,(void*)(WDT_SOFT_RESET));

	//set watchdog boe
	writew(0x0F,(void*)(WDT_SOFT_BOE));

	//clear irom mcu watchdog event mask
	//writel(0x7f&(readl((const volatile void *)SYS_EVTS_MASK6356)),(volatile void *)SYS_EVTS_MASK6356);
}
/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset & enabel clk for spdif.                                           *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_apb_reset_spdif(void)
{
	//reset spdif module manager
	writew(0x0,(void*)(SPDIF_SYSM_RESET));
	writew(0x1,(void*)(SPDIF_SYSM_RESET));

	//enable spdif module clk
	writew(0xFF,(void*)(SPDIF_SOFT_CLK_EN));

	//reset spdif module
	writew(0xFF,(void*)(SPDIF_SOFT_RESET));
	writew(0x00,(void*)(SPDIF_SOFT_RESET));

	//set spdif boe
	writew(0x0F,(void*)(SPDIF_SOFT_BOE));
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset & enabel clk for GPIO.                                             *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_gpio_reset(void)
{
	//reset gpio module manager
	writew(0x0,(void*)(GPIO_SYSM_RESET));
	writew(0x1,(void*)(GPIO_SYSM_RESET));

	//enable gpio module clk
	writew(0xFF,(void*)(GPIO_SOFT_CLK_EN));

	//reset gpio module
	writew(0xFF,(void*)(GPIO_SOFT_RESET));
	writew(0x00,(void*)(GPIO_SOFT_RESET));

	//set gpio boe
	writew(0x0F,(void*)(GPIO_SOFT_BOE));
}
/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset apb bus.                                                          *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_apb_bus_reset(void)
{
	//reset APB bus module
	//writew(0x0,(void*)(PBUS_RESET));
	//writew(0x1,(void*)(PBUS_RESET));

	//reset APB bus
	writew(0x0,(void*)(PBUS_SYSM_RESET));
	writew(0x1,(void*)(PBUS_SYSM_RESET));
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset & enabel clk for all modules which connect to apb bus.            *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_apb_modules_reset(void)
{
	//reset apb bus
	sys_apb_bus_reset();

	sys_apb_reset_i2s();
	sys_apb_reset_i2c();
	sys_apb_reset_spi();
	sys_apb_reset_uart();
	sys_apb_reset_pcm();
	sys_apb_reset_pwm();
	sys_apb_reset_timer();
	sys_apb_reset_watchdog();  //irom code have reseted watchdog module
	sys_apb_reset_spdif();
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
/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset GDMA.                                                          *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_dma_reset(void)
{
	//reset dma system manager module
	writew(0x0,(void*)(DMA_SYSM_RESET));
	writew(0x1,(void*)(DMA_SYSM_RESET));

	//init dma default config
	sys_init_dma();

	//enable dma clk
	writew(0xFF,(void*)(DMA_SOFT_CLK_EN));

	//hardward reset dma
	writew(0xFF,(void*)(DMA_SOFT_RESET));
	writew(0x00,(void*)(DMA_SOFT_RESET));

	//set dma boe
	writew(0x0F,(void*)(DMA_SOFT_BOE));
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
#define CLK_RMII_OPEN           (1 << 5)

#define RMII 1

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset gmac.                                                              *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_gmac_reset(void)
{

	//system reset gmac
	writew(0x0,(void*)(EMAC_SYSM_RESETN));
	writew(0x1,(void*)(EMAC_SYSM_RESETN));

	//enable gmac clk
	writew(0xFF,(void*)(EMAC_SOFT_CLKGATE_EN));

	//hardward reset gmac
	writew(0xFF,(void*)(EMAC_SOFT_RESET));
	//PHY Interface Select phy_intf_sel_i[2:0]


	// PHY Interface Select phy_intf_sel_i[2:0]
#ifdef RMII
	// for ext ominiphy only in simulation only support RMII interface
	// so in this mode we only support 10m/100M
	writew((GMAC_PHY_INTF_SEL_RMII & GMAC_PHY_INTF_SEL_MASK) | CLK_RMII_OPEN,
			(void*)EMAC_PHY_INTF_SEL_I);
#elif RGMII
	writew((GMAC_PHY_INTF_SEL_RGMII & GMAC_PHY_INTF_SEL_MASK),
			(void*)EMAC_PHY_INTF_SEL_I);
#endif

	writew(0x00,(void*)(EMAC_SOFT_RESET));

	//set gmac boe
	writew(0x0F,(void*)(EMAC_SOFT_BOE));
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset usb.                                                              *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_usb_reset(void)
{
	//reset usb system manager module
	writew(0x0,(void*)(USB_SYSM_RESET));
	writew(0x1,(void*)(USB_SYSM_RESET));

	//enable usb
	writew(0xFF,(void*)(USB_SOFT_CLK_EN));

	//hardward reset usb
	writew(0xFF,(void*)(USB_SOFT_RESET));
	writew(0x00,(void*)(USB_SOFT_RESET));

	//set usb boe
	writew(0x0F,(void*)(USB_SOFT_BOE));
}

/***************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset emmc.                                                             *
 *                                                                                                 *
 ***************************************************************************************************/

void sys_emmc_reset(void)
{
	//reset emmc system manager module
	writew(0x0,(void*)(EMMC_SYSM_RESET));
	writew(0x1,(void*)(EMMC_SYSM_RESET));

	//enable emmc
	writew(0xFF,(void*)(EMMC_SOFT_CLK_EN));

	//hardward reset emmc
	writew(0xFF,(void*)(EMMC_SOFT_RESET));
	writew(0x00,(void*)(EMMC_SOFT_RESET));

	//set emmc boe
	writew(0x0F,(void*)(EMMC_SOFT_BOE));

}

/***************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset sdio.                                                             *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_sdio_reset(void)
{
	//reset emmc system manager module
	writew(0x0,(void*)(SDIO_SYSM_RESET));
	writew(0x1,(void*)(SDIO_SYSM_RESET));

	//enable emmc
	writew(0xFF,(void*)(SDIO_SOFT_CLK_EN));

	//hardward reset emmc
	writew(0xFF,(void*)(SDIO_SOFT_RESET));
	writew(0x00,(void*)(SDIO_SOFT_RESET));

	//set emmc boe
	writew(0x0F,(void*)(SDIO_SOFT_BOE));

	//change the voltage from 3.3 to 1.8.
	writew(0xFF,(void*)(SW_MSC_REG));
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset WIFI DMA.                                                         *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_wifi_scdma_reset(void)
{
	//reset wifi system manager module
	writew(0x0,(void*)(SCDMA_SYSM_RESET));
	writew(0x1,(void*)(SCDMA_SYSM_RESET));

	//enable wifi clk
	writew(0xFF,(void*)(SCDMA_SOFT_CLK_EN));

	//hardward reset wifi
	writew(0xFF,(void*)(SCDMA_SOFT_RESET));
	writew(0x00,(void*)(SCDMA_SOFT_RESET));

	//set wifi boe
	writew(0x0F,(void*)(SCDMA_SOFT_BOE));
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset WIFI 1.                                                           *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_wifi_1_reset(void)
{
	//reset wifi system manager module
	writew(0x0,(void*)(WIFI_1_SYSM_RESET));
	writew(0x1,(void*)(WIFI_1_SYSM_RESET));

	//enable wifi clk
	writew(0xFF,(void*)(WIFI_1_SOFT_CLK_EN));

	//hardward reset wifi
	writew(0xFF,(void*)(WIFI_1_SOFT_RESET));
	writew(0x00,(void*)(WIFI_1_SOFT_RESET));

	//set wifi boe
	writew(0x0F,(void*)(WIFI_1_SOFT_BOE));
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset WIFI 2.                                                           *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_wifi_2_reset(void)
{
	//reset wifi system manager module
	writew(0x0,(void*)(WIFI_2_SYSM_RESET));
	writew(0x1,(void*)(WIFI_2_SYSM_RESET));

	//enable wifi clk
	writew(0xFF,(void*)(WIFI_2_SOFT_CLK_EN));

	//hardward reset wifi
	writew(0xFF,(void*)(WIFI_2_SOFT_RESET));
	writew(0x00,(void*)(WIFI_2_SOFT_RESET));

	//set wifi boe
	writew(0x0F,(void*)(WIFI_2_SOFT_BOE));
}

//catena rf module reset
void sys_catip_reset(void)
{
	//reset wifi system manager module
	writew(0x0,(void*)(CATIP_SYSM_RESET));
	writew(0x1,(void*)(CATIP_SYSM_RESET));

	//enable wifi clk
	writew(0xFF,(void*)(CATIP_SOFT_CLK_EN));

	//hardward reset wifi
	writew(0xFF,(void*)(CATIP_SOFT_RESET));
	writew(0x00,(void*)(CATIP_SOFT_RESET));

	//set wifi boe
	writew(0x0F,(void*)(CATIP_SOFT_BOE));
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset WIFI.                                                             *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_wifi_reset(void)
{
	sys_wifi_scdma_reset(); //dma in wifi module
	sys_wifi_1_reset();
	sys_wifi_2_reset();
	sys_catip_reset();
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset cpu.                                                              *
 *                                                                                                 *
 ***************************************************************************************************/
void cpu_soft_reset(void)
{
	//reset cpu
	writew(0x0,(void*)(CPU_RESET));
	writew(0x1,(void*)(CPU_RESET));
}
/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset iram.                                                              *
 *                                                                                                 *
 ***************************************************************************************************/
void iram_soft_reset(void)
{
	//reset iram system manager module
	writew(0x0,(void*)(IRAM_SYSM_RESET));
	writew(0x1,(void*)(IRAM_SYSM_RESET));

	//enable iram clk
	writew(0xFF,(void*)(IRAM_SOFT_CLK_EN));
	writew(0xFF,(void*)(IRAM_SOFT_CLK_EN1));

	//hardward reset sata
	writew(0xFF,(void*)(IRAM_SOFT_RESET));
	writew(0x00,(void*)(IRAM_SOFT_RESET));
	writew(0xFF,(void*)(IRAM_SOFT_RESET1));
	writew(0x00,(void*)(IRAM_SOFT_RESET1));

	//set sata boe
	writew(0x0F,(void*)(IRAM_SOFT_BOE));
}
/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset memory.                                                           *
 *                                                                                                 *
 ***************************************************************************************************/
void memory_soft_reset(void)
{
	//reset ddr system manager module,this operation move to irom fix code.
	//writew(0x0,(void*)(MPU_SYSM_RESET));
	//writew(0x1,(void*)(MPU_SYSM_RESET));

	//reset ddr reg, bit 1
	writew(0x1,(void*)( PBUS_RESET));
	//reset bus cpu to ddr, bit 1
	writew(0x1,(void*)( CPU_RESET));
	//reset memory
	writew(0x0,(void*)(MEM_RESET));
	writew(0x1,(void*)(MEM_RESET));

	writew(0x3,(void*)( CPU_RESET));

	writew(0x3,(void*)( PBUS_RESET));
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset bus.                                                           *
 *                                                                                                 *
 ***************************************************************************************************/
void bus_soft_reset(void)
{
	//reset bus1
	writew(0x0,(void*)(BUS1_RESET));
	writew(0x1,(void*)(BUS1_RESET));

	//reset bus2
	writew(0x0,(void*)(BUS2_RESET));
	writew(0x1,(void*)(BUS2_RESET));

	//reset bus3
	writew(0x0,(void*)(BUS3_RESET));
	writew(0x1,(void*)(BUS3_RESET));

	//reset bus4
	//writew(0x0,(void*)(BUS4_RESET));
	//writew(0x1,(void*)(BUS4_RESET));

	//reset bus5
	//writew(0x0,(void*)(BUS5_RESET));
	//writew(0x1,(void*)(BUS5_RESET));

	//reset bus6
	//writew(0x0,(void*)(BUS6_RESET));
	//writew(0x1,(void*)(BUS6_RESET));

	//reset bus7
	//writew(0x0,(void*)(BUS7_RESET));
	//writew(0x1,(void*)(BUS7_RESET));

	//reset bus8
	//writew(0x0,(void*)(BUS8_RESET));
	//writew(0x1,(void*)(BUS8_RESET));
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to clock manager.                                                           *
 *                                                                                                 *
 ***************************************************************************************************/
void clock_manager_soft_reset(void)
{
	//reset clock manager module
	writew(0x0,(void*)(CM_SYSM_RESET));
	writew(0x1,(void*)(CM_SYSM_RESET));
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to io manager.                                                             *
 *                                                                                                 *
 ***************************************************************************************************/
void io_soft_reset(void)
{
	//reset IO manager module
	writew(0x0,(void*)(IO_SYSM_RESET));
	writew(0x1,(void*)(IO_SYSM_RESET));
}

/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to remap access space.                                                     *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_remap_n_enable(void)
{
	//if not set
	//can't access block devices(except sdio) + npu
	writel(0x1,(void*)( REMAP_N));
}
#endif

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


/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to config ethernet ominiphy                                                          *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_omini_phy_reset(void)
{
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

	int phy_cfg_mode = OMINI_PHY_CFG_MODE_AUTO_FULL;
#ifdef NPU_ETH_DUPLEX
	int phy_duplex = NPU_ETH_DUPLEX;
#else
	int phy_duplex = OMINI_PHY_DUPLEX_FULL;
#endif

#ifdef NPU_ETH_SPEED
	int phy_speed = NPU_ETH_SPEED;
#else
	int phy_speed = OMINI_SPEED_AUTO;
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


/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset NPU.                                                          *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_npu_reset(void)
{
	//reset npu system manager module
	writew(0x0,(void*)(NPU_SYSM_RESET));
	writew(0x1,(void*)(NPU_SYSM_RESET));

	//enable npu clk
	writew(0xFF,(void*)(NPU_SOFT_CLK_EN));

	//hardward reset npu
	writew(0xFF,(void*)(NPU_SOFT_RESET));
#ifndef FPGA
	//emac[5-1]_phy_enable[4:0]
	writeb(0x1F,(void*)(OMINI_PHY_ENABLE));
	sys_omini_phy_reset();
#endif
	writew(0x00,(void*)(NPU_SOFT_RESET));

	//we can hardware reset the phy by single with the 4 bit of NPU_SOFT_RESET
#if 0
	writew(0x10,(void*)(NPU_SOFT_RESET));
	timer_count_down(0, 0, USER, 0, 10, 0);
	timer_count_down_over(0, 0);
	timer_disable(0, 0);
	writew(0x00,(void*)(NPU_SOFT_RESET));
	timer_count_down(0, 0, USER, 0, 10, 0);
	timer_count_down_over(0, 0);
	timer_disable(0, 0);
#endif

	//set npu boe
	writew(0x0F,(void*)(NPU_SOFT_BOE));
}

#if defined(CONFIG_SPL_BUILD)
static int set_clk_ratio(void)
{
	writel(4,(void*)(CORE_L2_CLK_RATIO));
	writel(1,(void*)(CORE_L2_SET_CLK_RATIO));

	return 0;
}
/**************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset all module for sf1688.                                            *
 *                                                                                                 *
 ***************************************************************************************************/
void sys_reset_all_module(void)
{
	//cpu reset
	//cpu_soft_reset();

	//iram reset code move to copy_c2_ram.S
	//iram_soft_reset();

	//set Clock Divider to 1:3
	set_clk_ratio();

	//memory reset
	memory_soft_reset();
	//reset clock manager module
	//clock_manager_soft_reset();

	//reset sys bus
	bus_soft_reset();

	//reset IO manager module
	io_soft_reset();

	//reset apb modules
	sys_apb_modules_reset();

	//reset dam
	sys_dma_reset();
	//reset npu
	sys_npu_reset();
	//reset wifi
	sys_wifi_reset();
	sys_usb_reset();
	sys_gmac_reset();
	sys_emmc_reset();
	sys_sdio_reset();
	sys_gpio_reset();
	sys_remap_n_enable();
}
#endif

/**************************************************************************************************
 *	software reboot the system by setting system manager.
 ***************************************************************************************************/
void sys_reboot(void)
{
	printf("Running %s() ...\n",__func__);
	writew(0x01,(void*)(SOFT_REBOOT));
}
