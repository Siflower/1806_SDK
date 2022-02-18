#include <io.h>
#include <sf_mmap.h>
#include <clk.h>

#ifdef SFA18_CRYSTAL_6M
static int clk_use_crystal_6m(void)
{
	int i, para = 0;

	for (i = 0; i < 4; i++) {
		/* calculate new parameters */
		para = readb(CPU_PLL_PARA70 + i * 0x40);
		para += readb(CPU_PLL_PARA158 + i * 0x40) << 8;
		para *= 2;

		if (i == 0) {
			/* switch cpu clk to cmn pll */
			writeb(9, CPU_CLK_DIV);
			writeb(2, CPU_CLK_CONFIG);
		}

		if (i == 2) {
			/* switch pbus clk to cpu pll */
			writeb(0, PBUS_CLK_CONFIG);
		}

		/*
		 * 1.power down pll
		 * 2.clear load bit
		 * 3.set pll parameters
		 * 4.set load bit
		 * 5.clear power down bit
		 * 6.check lock & stable bit
		 */
		writeb(0x3b, CPU_PLL_POWER + i * 0x40);
		writeb(0x18, CPU_PLL_CONFIG + i * 0x40);
		writeb(para & 0xff, CPU_PLL_PARA70 + i * 0x40);
		writeb(para >> 8, CPU_PLL_PARA158 + i * 0x40);
		writeb(0x38, CPU_PLL_CONFIG + i * 0x40);
		writeb(0x3a, CPU_PLL_POWER + i * 0x40);
		while(!readb(CPU_PLL_POWER + i * 0x40 + 0x20));

		if (i == 2) {
			/* switch pbus clk back to cmn pll */
			writeb(2, PBUS_CLK_CONFIG);
		}

		if (i == 0) {
			/* switch cpu clk back to cpu pll */
			writeb(0, CPU_CLK_CONFIG);
			writeb(1, CPU_CLK_DIV);
		}

	}

	return 0;
}
#endif

int clk_gating_init(void)
{
	writeb(readb(CPU_PLL_CONFIG) | (1 << 4) | (1 << 3), CPU_PLL_CONFIG);
	writeb(readb(DDR_PLL_CONFIG) | (1 << 4) | (1 << 3), DDR_PLL_CONFIG);
	writeb(readb(CMN_PLL_CONFIG) | (1 << 4) | (1 << 3), CMN_PLL_CONFIG);
#ifndef SF19A28
	writeb(readb(SPC_PLL_CONFIG) | (1 << 4) | (1 << 3), SPC_PLL_CONFIG);
#endif
#ifdef SFA18_CRYSTAL_6M
	clk_use_crystal_6m();
#endif
	writeb(0xFF, BUS1_XN_CLK_ENABLE);
	writeb(0xFF, BUS2_XN_CLK_ENABLE);
	writeb(0xFF, BUS3_XN_CLK_ENABLE);
	writeb(0xFF, CPU_CLK_ENABLE);
	writeb(0xFF, PBUS_CLK_ENABLE);
	writeb(0x4, MEM_PHY_CLK_DIV);
	writeb(0xFF, MEM_PHY_CLK_ENABLE);
	writeb(0xFF, UART_EXTCLK_ENABLE);
#ifndef SF19A28
	writeb(0xFF, AUDIO_EXTCLK_ENABLE);
	writeb(0xFF, SPDIF_EXTCLK_ENABLE);
	writeb(0xFF, SDHC_EXT_CLK_ENABLE);
	writeb(0xFF, EMMC_EXT_CLK_ENABLE);
	writeb(0xFF, CRYPTO_CLK_ENABLE);
#endif
	// writeb(0xFF, GDU0_CLK_ENABLE);
	// writeb(0xFF, GDU0_EITF_CLK_ENABLE);
	// writeb(0xFF, TVIF0_CLK_ENABLE);
	writeb(0xFF, ETH_REF_CLK_ENABLE);
	writeb(0xFF, ETH_BYP_REF_CLK_ENABLE);
	writeb(0xFF, ETH_TSU_CLK_ENABLE);
#ifdef SF19A28_FULLMASK
	writeb(0xFF, GMAC_BYP_REF_CLK_ENABLE);
#endif
	#ifndef MPW0
	writeb(0xFF, NPU_PE_CLK_ENABLE);
	#endif
	/* set 50MHZ clock of ptp clock */
	writeb(0x1D, ETH_TSU_CLK_DIV);
	#ifdef ENH_EXT_OMINI_PHY
	/* for ext phy only support which need 50MHZ clock */
	writeb(0x1D, ETH_BYP_REF_CLK_DIV);
	#endif
	writeb(0xFF, ETH_BYP_REF_CLK_ENABLE);
	writeb(0xFF, ETH_TSU_CLK_ENABLE);
	writeb(0xFF, WLAN24_MAC_WT_CLK_ENABLE);
	writeb(0xFF, WLAN5_MAC_WT_CLK_ENABLE);
	writeb(0xFF, USBPHY_REF_CLK_ENABLE);
	/* close tclk to avoid clk output in GPIO62. */
	writeb(0x00, TCLK_ENABLE);

	return 0;
}

#ifndef MPW0
struct clk_info {
	int offset;
	int src;
	int ratio;
	int enable;
};

int set_pll_ratio_with_params(int pll_type, unsigned long long pll_para)
{
	unsigned int i, reg, value, offset;

	offset = pll_type;

	//enabe all power down--->close clk
	reg = PLL_BASE + offset*PLL_OFFSET + 0*PLL_REG_WID;
	value = readl(reg);
	value = value | (PLL_PD_ALL_PD);
	writeb(value ,reg);

	//clear load bit
	reg = PLL_BASE + offset*PLL_OFFSET + 7*PLL_REG_WID;
	value = readl(reg);
	value = value & (~PLL_CFG_LOAD);
	writeb(value, reg);

	for(i=0;i<6;i++) {
		writeb(pll_para,CPU_PLL_PARA70 + 4*i + offset*PLL_OFFSET);
		pll_para = pll_para >> 8;
	}

	//set load bit
	reg = PLL_BASE + offset*PLL_OFFSET + 7*PLL_REG_WID;
	value = readl(reg);
	value = value | (PLL_CFG_LOAD);
#if SF19A28_FULLMASK && !CRYSTAL_40M
	value &= ~PLL_CFG_SRC_SEL;
#endif
	writeb(value, reg);

	//clear all powerdown bit
	reg = PLL_BASE + offset*PLL_OFFSET + 0*PLL_REG_WID;
	value = readl(reg);
	value = value & (~(PLL_PD_ALL_PD));
	writeb(value, reg);

	//wait for pll locked
	reg = PLL_BASE + offset*PLL_OFFSET + 8*PLL_REG_WID;
	value = readl(reg);
	while(!value)
		value = readl(reg);

	return 0;
}

#ifdef SF19A28
int clk_update(void)
{
#ifdef SF19A28_FULLMASK
#ifdef PERFORMANCE_MODE
#ifdef CRYSTAL_40M
	/* cpupll -> 1600MHz */
	writel(1, CPU_CLK_CONFIG);
	set_pll_ratio_with_params(0, 0x49000000028);
	writel(0, CPU_CLK_CONFIG);

	/* cmnpll -> 1500MHz */
	writel(1, PBUS_CLK_CONFIG);
	set_pll_ratio_with_params(2, 0x4a00000004b);
	writel(2, PBUS_CLK_CONFIG);
#else /* !CRYSTAL_40M */
	writel(1, CPU_CLK_CONFIG);
	set_pll_ratio_with_params(0, 0x49000000085);
	writel(0, CPU_CLK_CONFIG);

	writel(1, PBUS_CLK_CONFIG);
	set_pll_ratio_with_params(2, 0x4a0000000fa);
	writel(2, PBUS_CLK_CONFIG);
#endif /* CRYSTAL_40M */
	/* bus1, bus2 -> 300MHz, bus3 -> 375MHz */
	writel(4, BUS1_XN_CLK_DIV);
	writel(4, BUS2_XN_CLK_DIV);
	writel(3, BUS3_XN_CLK_DIV);

	/* m6251_0, m6251_1 -> 500MHz */
	writel(2, M6251_0_CLK_DIV);
	writel(2, M6251_1_CLK_DIV);

	/* wlan24, wlan5 -> 300MHz */
	writel(4, WLAN24_MAC_WT_CLK_DIV);
	writel(4, WLAN5_MAC_WT_CLK_DIV);

#else /* !PERFORMANCE_MODE */

#ifdef CRYSTAL_40M
	/* cpupll -> 1600MHz */
	writel(1, CPU_CLK_CONFIG);
	set_pll_ratio_with_params(0, 0x49000000028);
	writel(0, CPU_CLK_CONFIG);

	/* cmnpll -> 1500MHz */
	writel(1, PBUS_CLK_CONFIG);
	set_pll_ratio_with_params(2, 0x4a00000004b);
	writel(2, PBUS_CLK_CONFIG);
#else /* !CRYSTAL_40M */
	writel(1, CPU_CLK_CONFIG);
	set_pll_ratio_with_params(0, 0x4a0000000e9);
	writel(0, CPU_CLK_CONFIG);

	/* cmnpll -> 1500MHz */
	writel(1, PBUS_CLK_CONFIG);
	set_pll_ratio_with_params(2, 0x4a0000000fa);
	writel(2, PBUS_CLK_CONFIG);
#endif /* CRYSTAL_40M */
	/* bus1, bus2 -> 300MHz */
	/* bus3 -> 375MHz */
	writel(4, BUS1_XN_CLK_DIV);
	writel(4, BUS2_XN_CLK_DIV);
	writel(3, BUS3_XN_CLK_DIV);

	/* m6251_0, m6251_1 -> 500MHz */
	writel(2, M6251_0_CLK_DIV);
	writel(2, M6251_1_CLK_DIV);

	/* wlan24, wlan5 -> 375MHz */
	writel(3, WLAN24_MAC_WT_CLK_DIV);
	writel(3, WLAN5_MAC_WT_CLK_DIV);
#endif /* PERFORMANCE_MODE */
#else /* !SF19A28_FULLMASK */

#ifdef CRYSTAL_40M
	/* update pll parameters to fit 40MHz osc_clk */
	writel(1, CPU_CLK_CONFIG);
	set_pll_ratio_with_params(0, 0x4a000000043);
	writel(0, CPU_CLK_CONFIG);

	set_pll_ratio_with_params(1, 0x4a000000028);

	writel(1, PBUS_CLK_CONFIG);
	set_pll_ratio_with_params(2, 0x4a00000004b);
	writel(2, PBUS_CLK_CONFIG);

	writel(readl(CPU_PLL_CONFIG) | 1 << 1, CPU_PLL_CONFIG);
	writel(readl(DDR_PLL_CONFIG) | 1 << 1, DDR_PLL_CONFIG);
	writel(readl(CMN_PLL_CONFIG) | 1 << 1, CMN_PLL_CONFIG);
#endif /* CRYSTAL_40M */
	/* set pbus to 100MHz */
	writel(15 - 1, PBUS_CLK_DIV);
#endif /* SF19A28_FULLMASK */

	return 0;
}
#else
int clk_update(void)
{
	int i;
#ifdef FULLMASK
	struct clk_info clk[] = {
		{  0, 3, 4, 1 }, /* BUS1_CLK */
		{ 20, 0, 1, 1 }, /* M6251_0_CLK */
		{ 21, 0, 1, 1 }, /* M6251_1_CLK */
		{ -1, -1, -1, -1 },
	};
#else
	struct clk_info clk[] = {
		{ 20, 0, 3, 1 }, /* M6251_0_CLK */
		{ 21, 0, 3, 1 }, /* M6251_1_CLK */
		{ 22, 3, 5, 1 }, /* WLAN_24_CLK */
		{ 23, 3, 5, 1 }, /* WLAN_5_CLK */
		{ 27, 2, 4, 1 }, /* GDU_CLK */
		{ 28, 2, 4, 1 }, /* GDU_EITF_CLK */
		{ 29, 2, 4, 1 }, /* TVIF_CLK */
		{ -1, -1, -1, -1 },
	};
#endif

	for (i = 0; clk[i].offset != -1; i++) {
		if (clk[i].enable) {
			writel(clk[i].ratio, CLK_BASE
						+ clk[i].offset * CLK_OFFSET
						+ CLK_REG_WID);
			writel(clk[i].src, CLK_BASE + clk[i].offset * CLK_OFFSET);
		}
	}

#ifndef FULLMASK
	/* switch cpu clk to cmn pll */
	writeb(9, CPU_CLK_DIV);
	writeb(2, CPU_CLK_CONFIG);

	/*
	 * 1.power down pll
	 * 2.clear load bit
	 * 3.set pll parameters
	 * 4.set load bit
	 * 5.clear power down bit
	 * 6.check lock & stable bit
	 */
	writeb(0x3b, CPU_PLL_POWER);
	writeb(0x18, CPU_PLL_CONFIG);
	writeb(0xfa, CPU_PLL_PARA70); /* 1000MHZ */
	writeb(0x00, CPU_PLL_PARA158);
	writeb(0x38, CPU_PLL_CONFIG);
	writeb(0x3a, CPU_PLL_POWER);
#ifndef FPGA
	while(!readb(CPU_PLL_POWER));
#endif
	/* switch cpu clk back to cpu pll */
	writeb(0, CPU_CLK_CONFIG);
	writeb(1, CPU_CLK_DIV); /* 1:2 */
#endif /* FULLMASK */
	return 0;
}
#endif /* SF19A28_MPW0 */
#endif
