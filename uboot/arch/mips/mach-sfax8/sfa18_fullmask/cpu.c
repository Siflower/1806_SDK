#include <common.h>
#include <asm/io.h>
#include <spl.h>
#include <mach/sfax8.h>

#include <mach/sys.h>

extern void ddr_init(void);
extern void sys_reset_all_module(void);
extern char __bss_start[], __bss_end[];


/**************************************************************************************************
*                                                                                                 *
* -Description:                                                                                   *
*    Set pll ratio to fit 6M crystal.                                                             *
*    pll_type: 0 -> cpu_pll 6M                                                                    *
*              1 -> ddr_pll 6M                                                                    *
*              2 -> cmn_pll 6M                                                                    *
*              3 -> spc_pll 6M                                                                    *
*              4 -> cmn_pll 12M                                                                   *
*                                                                                                 *
***************************************************************************************************/
static int set_pll_ratio(int pll_type, unsigned long long pll_para)
{
	unsigned int i, reg, value, offset;

	if(pll_type > 100)
		return -1;

	offset = pll_type%4;

	//enabe all power down--->close clk
	reg = PLL_BASE + offset*PLL_OFFSET + 0*PLL_REG_WID;
	//pll red read test
	value = readl((void*)reg);
	value = value | (PLL_PD_ALL_PD);
	writeb(value ,(void*)reg);

	//clear load bit
	reg = PLL_BASE + offset*PLL_OFFSET + 7*PLL_REG_WID;
	value = readl((void*)reg);
	value = value & (~PLL_CFG_LOAD);
	writeb(value, (void*)reg);

	if(pll_para == 0) {
		switch(pll_type)
		{
			case 0:
				pll_para = CRYSTAL_6M_CPU_PARA;
				break;
			case 1:
				pll_para = CRYSTAL_6M_DDR_PARA;
				break;
			case 2:
				pll_para = CRYSTAL_6M_CMN_PARA;
				break;
			case 3:
				pll_para = CRYSTAL_6M_SPC_PARA;
				break;
			case 6:
				pll_para = CRYSTAL_12M_CMN_PARA;
				break;
			default:
				return 0;
		}
	}

	for(i=0;i<6;i++) {
		writeb(pll_para,(void*)(CPU_PLL_PARA70 + 4*i + offset*PLL_OFFSET));
		pll_para = pll_para >> 8;
	}

	//set load bit
	reg = PLL_BASE + offset*PLL_OFFSET + 7*PLL_REG_WID;
	value = readl((void*)reg);
	value = value | (PLL_CFG_LOAD);
	writeb(value, (void*)reg);

	//clear all powerdown bit
	reg = PLL_BASE + offset*PLL_OFFSET + 0*PLL_REG_WID;
	value = readl((void*)reg);
	value = value & (~(PLL_PD_ALL_PD));
	writeb(value, (void*)reg);

	reg = PLL_BASE + offset*PLL_OFFSET + 8*PLL_REG_WID;
	value = readl((void*)reg);
	while(!value)
	{
		value = readl((void*)reg);
	}

	return 0;
}

/**************************************************************************************************
*                                                                                                 *
* -Description:                                                                                   *
*    if the board use 6m ctystal,we need call this function;                                      *
*    to realize all clks frequency equal to 12M crytal.                                           *
*                                                                                                 *
***************************************************************************************************/
#ifdef SFA18_CRYSTAL_6M
static void clk_use_crystal_6m(void)
{
	unsigned int reg;

	set_pll_ratio(1, 0);
	set_pll_ratio(3, 0);

	//change pbus clk to cpu pll.
	reg = CLK_BASE + 9*CLK_OFFSET + 0*CLK_REG_WID;
	writel(0, (void*)reg);

	//set cmn pll to 12M parameter
	set_pll_ratio(6, 0);

	//change pbus clk back to cmn pll.
	reg = CLK_BASE + 9*CLK_OFFSET + 0*CLK_REG_WID;
	writel(2, (void*)reg);

	//change cpu clk to cmn pll.
	reg = CLK_BASE + 8*CLK_OFFSET + 0*CLK_REG_WID;
	writel(2, (void*)reg);

	set_pll_ratio(0, 0);

	//change cpu clk back to cpu pll.
	reg = CLK_BASE + 8*CLK_OFFSET + 0*CLK_REG_WID;
	writel(0, (void*)reg);

	//change pbus clk to cpu pll.
	reg = CLK_BASE + 9*CLK_OFFSET + 0*CLK_REG_WID;
	writel(0, (void*)reg);

	set_pll_ratio(2, 0);

	//change pbus clk back to cmn pll.
	reg = CLK_BASE + 9*CLK_OFFSET + 0*CLK_REG_WID;
	writel(2, (void*)reg);

	return;
}
#endif

void sf_clk_gating_enable(void)
{

#ifdef SFA18_CRYSTAL_6M
	clk_use_crystal_6m();
#endif

	writeb(readb((void *)(CPU_PLL_CONFIG)) |(1 << 4) | (1 << 3),(void*)(CPU_PLL_CONFIG));
	writeb(readb((void *)(DDR_PLL_CONFIG)) |(1 << 4) | (1 << 3),(void*)(DDR_PLL_CONFIG));
	writeb(readb((void *)(CMN_PLL_CONFIG)) |(1 << 4) | (1 << 3),(void*)(CMN_PLL_CONFIG));
	writeb(readb((void *)(SPC_PLL_CONFIG)) |(1 << 4) | (1 << 3),(void*)(SPC_PLL_CONFIG));

	writeb(0xFF,(void*)( BUS1_XN_CLK_ENABLE));

	writeb(0xFF,(void*)( BUS2_XN_CLK_ENABLE));

	writeb(0xFF,(void*)( BUS3_XN_CLK_ENABLE));

	//writeb(0xFF,(void*)( BUS4_XN_CLK_ENABLE));
	//writeb(0xFF,(void*)( BUS5_XN_CLK_ENABLE));
	//writeb(0xFF,(void*)( BUS6_XN_CLK_ENABLE));
	//writeb(0xFF,(void*)( BUS7_XN_CLK_ENABLE));
	//writeb(0xFF,(void*)( BUS8_XN_CLK_ENABLE));

	writeb(0xFF,(void*)( CPU_CLK_ENABLE));

	writeb(0xFF,(void*)( PBUS_CLK_ENABLE));

	writeb(0xFF,(void*)( MEM_PHY_CLK_ENABLE));
	writeb(0x4,(void*)( MEM_PHY_CLK_DIV));//default 798-> 532M

	writeb(0xFF,(void*)( AUDIO_EXTCLK_ENABLE));

	writeb(0xFF,(void*)( UART_EXTCLK_ENABLE));

	writeb(0xFF,(void*)( SPDIF_EXTCLK_ENABLE));

	writeb(0xFF,(void*)( SDHC_EXT_CLK_ENABLE));

	writeb(0xFF,(void*)( EMMC_EXT_CLK_ENABLE));

	// writeb(0xFF,(void*)GDU0_CLK_ENABLE);
	// writeb(0xFF,(void*)GDU0_EITF_CLK_ENABLE);
	// writeb(0xFF,(void*)TVIF0_CLK_ENABLE);

	writeb(0xFF,(void*)( ETH_REF_CLK_ENABLE));
	writeb(0xFF,(void*)( ETH_BYP_REF_CLK_ENABLE));
	writeb(0xFF,(void*)( ETH_TSU_CLK_ENABLE));
	writeb(0xFF,(void*)( NPU_PE_CLK_ENABLE));

	writeb(0xFF,(void*)( ETH_BYP_REF_CLK_ENABLE));
	writeb(0xFF,(void*)( ETH_TSU_CLK_ENABLE));
	//set 50MHZ clock of ptp clock
	writeb(0x1D,(void*)(ETH_TSU_CLK_DIV));

	writeb(0xFF, (void*)M6251_0_CLK_ENABLE);
	writeb(0xFF, (void*)M6251_1_CLK_ENABLE);

	// NOTE: when we configure sharemem as fram mode, the plf_clk is working clock
	// so we should close the plf_clk when system boot and the wifi not open yet timeing
	// when wifi open and load it's driver, the driver will do open this clock.
	writeb(0x0,(void*)( WLAN24_MAC_WT_CLK_ENABLE));
	writeb(0x0,(void*)( WLAN5_MAC_WT_CLK_ENABLE));

	writeb(0xFF,(void*)( USBPHY_REF_CLK_ENABLE));

	writeb(0xFF,(void*)( TCLK_ENABLE));
}


#define SF_DEBUG

#include <mach/sfax8_debug.h>

/*
 * Update pll/clk to sign-off frequency.
 */
int sf_clk_update(void)
{
	unsigned int reg, div, freq = SF_CPU_PLL_CLK;
	unsigned long long pll_para = (CRYSTAL_12M_CPU_PARA >> 16) << 16;

	//cpu_pll & cpu_clk
	//set cpu_clk div to 2
	reg = CLK_BASE + 8*CLK_OFFSET + 1*CLK_REG_WID;
	writel(1, (void*)reg); //896

	//change cpu clk to cmn pll.
	reg = CLK_BASE + 8*CLK_OFFSET + 0*CLK_REG_WID;
	writel(2, (void*)reg);

	//12M osc -> divided by 4, 6M divided by 2
	div = freq/1000000/4;
	set_pll_ratio(0, pll_para + div);

	//change cpu clk back to cpu pll.
	reg = CLK_BASE + 8*CLK_OFFSET + 0*CLK_REG_WID;
	writel(0, (void*)reg);

	//gdu0_clk
	reg = CLK_BASE + 27*CLK_OFFSET + 1*CLK_REG_WID;
	writel(4, (void*)reg); //237.6

	//gdu0_eitf_clk
	reg = CLK_BASE + 28*CLK_OFFSET + 1*CLK_REG_WID;
	writel(4, (void*)reg); //237.6

	//tvif0_clk
	reg = CLK_BASE + 29*CLK_OFFSET + 1*CLK_REG_WID;
	writel(4, (void*)reg); //237.6

	//m5251_x_clk
	reg = CLK_BASE + 20*CLK_OFFSET + 1*CLK_REG_WID;
	writel(3, (void*)reg); //448

	reg = CLK_BASE + 21*CLK_OFFSET + 1*CLK_REG_WID;
	writel(3, (void*)reg); //448

	//change m6251 clk to cpu pll.
	reg = CLK_BASE + 20*CLK_OFFSET + 0*CLK_REG_WID;
	writel(0, (void*)reg);

	reg = CLK_BASE + 21*CLK_OFFSET + 0*CLK_REG_WID;
	writel(0, (void*)reg);

	/*
	 * SD & EMMC clk default freq is 150MHZ.
	//sdhc_clk & emmc_clk
	reg = CLK_BASE + 14*CLK_OFFSET + 1*CLK_REG_WID;
	writel(19, (void*)reg); //75

	reg = CLK_BASE + 15*CLK_OFFSET + 1*CLK_REG_WID;
	writel(14, (void*)reg); //100
	*/

	//wlan_24 & wlan_5
	reg = CLK_BASE + 22*CLK_OFFSET + 1*CLK_REG_WID;
	writel(5, (void*)reg); //250

	reg = CLK_BASE + 23*CLK_OFFSET + 1*CLK_REG_WID;
	writel(5, (void*)reg); //250

	return 0;
}

/* NOTE: set the sharemem mode default to fram mode for wifi use
 * here we conside the power save when system not open wifi timeing*/
int sf_sharemem_mode_init(void)
{
	writeb(0x03, (void*)(WIFI_SHARE_RAM_SEL));
	return 0;
}


/* arch specific CPU init */
int lowlevel_init(void)
{
#if defined(CONFIG_SPL_BUILD)
	memset(__bss_start, 0, __bss_end - __bss_start);

	sf_clk_gating_enable();

	sf_clk_update();

	sf_sharemem_mode_init();

	sys_reset_all_module();

	led_onoff(1);
	spl_init();
	preloader_console_init();
	ddr_init();
	led_onoff(0);
#endif
	return 0;
}
