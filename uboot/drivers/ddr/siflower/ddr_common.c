#include <common.h>
#include <asm/io.h>
#include <mach/sys.h>

#include "ddr_regs.h"

static void delay(u32 count)
{
	#if 0
	volatile u32 i = 0, j = 0;

	while (j < count) {
		for (i = 0; i < 10; i++)
			;
		j++;
	}
	#else
	return;
	#endif
}

static inline void set_quasi_dynamic_reg(u32 value, u32 addr)
{
	//set sw_done = 0
	writel(0x0, (void *)DDR_UMCTL2_SWCTL);
	/* config register */
	writel(value, (void *)addr);
	//set sw_done = 1
	writel(0x1, (void *)DDR_UMCTL2_SWCTL);
	//wait for sw_done_ack = 1
	while (!(readl((void *)DDR_UMCTL2_SWSTAT)))
		;
}

void set_umctl2_reg(struct ddr_info *ddr)
{
	struct ddr_timing *t = &ddr->timing;
	enum ddr_type type = ddr->type;
        u32 tck = t->tck;
	u32 tmp = 0;

	set_quasi_dynamic_reg(0x0, DDR_UMCTL2_DFIMISC);

	tmp = (ddr->bl / 2) << 16;
	if (type == ddr3)
		tmp |= (0x1 << 0);
	writel(tmp, (void *)DDR_UMCTL2_MSTR);

	//disable hardware low power function
	writel(0, (void *)DDR_UMCTL2_HWLPCTL);

	// set selfref_to_x32 = 0x40, powerdown_to_x32 = 0x10
	tmp = (0x40 << 16) | (0x10);
	writel(tmp, (void *)DDR_UMCTL2_PWRTMG);

	//refresh burst = 8
	tmp = (0x2 << 20) | (0x10 << 12) | (7 << 4);
	writel(tmp, (void *)DDR_UMCTL2_RFSHCTL0);

	tmp = ((t->trefi / 2 / 32) << 16) | (tck_to_uctl2(t->trfc) << 0);
	writel(tmp, (void *)DDR_UMCTL2_RFSHTMG);

	if (type == ddr2)
		tmp = (2 << 16) | (tck_to_x1024(us_to_tck(240)) << 0);
	else if(type == ddr3)
		tmp = (tck_to_x1024(t->txpr) << 16) | (tck_to_x1024(us_to_tck(500)) << 0);
	writel(tmp, (void *)DDR_UMCTL2_INIT0);

	//TODO: whether should be set to 1?
	tmp = (tck_to_x1024(us_to_tck(200)) << 16); //if need to be divided by 2?
	writel(tmp, (void *)DDR_UMCTL2_INIT1);

	if (type == ddr2) {
		t->mr[0] = 3 | (t->cl << 4) | ((t->twr - 1) << 9); //BL=8
		//DLL fast exit
		t->mr[0] |= 1 << 12;
		t->mr[1] = (t->al << 3) | (1 << 6);
	} else if (type == ddr3) {
		if (t->twr > 8)
			t->mr[0] = ((t->twr / 2) & 0x7) << 9;
		else
			t->mr[0] = (t->twr - 4) << 9;

		if (t->cl < 12)
			t->mr[0] |= (t->cl - 4) << 4;
		else if (t->cl == 12)
			t->mr[0] |= 1 << 2;
		else if (t->cl == 13)
			t->mr[0] |= (1 << 2) | (1 << 4);
		t->mr[0] |= 1 << 8;
		if ((t->cl - t->al) < 3)
			t->mr[1] = (t->cl - t->al) << 3;
		t->mr[1] |= (1 << 2) | (1 << 6);
	}
	writel((t->mr[0] << 16) | t->mr[1], (void *)DDR_UMCTL2_INIT3);

	if (type == ddr2) {
		t->mr[2] = t->mr[3] = 0;
	} else if (type == ddr3) {
		t->mr[2] = ((t->cwl - 5) << 3) | (1 << 6);
		//t->mr[2] |= 1 << 9;
		t->mr[3] = 0;
	}
	writel((t->mr[2] << 16) | t->mr[3], (void *)DDR_UMCTL2_INIT4);

	tmp = DIV_ROUND_UP(t->tzqinit, 32) << 16;
	writel(tmp, (void *)DDR_UMCTL2_INIT5);

	tmp = (((t->wl + (ddr->bl / 2) + t->twr) / 2) << 24) | (tck_to_uctl2(t->tfaw) << 16) | (((t->tras_max - 1) / 2 / 1024) << 8) | (tck_to_uctl2(t->tras_min) << 0);
	writel(tmp, (void *)DDR_UMCTL2_DRAMTMG0);

	tmp = tck_to_uctl2(t->trc) << 0;
	if (type == ddr2)
		tmp |= (tck_to_uctl2(t->txp) << 16) | (((t->al + (ddr->bl / 2) + max(t->trtp, (u32)2) - 2) / 2) << 8);
	else if (type == ddr3)
		tmp |= (tck_to_uctl2(t->txpdll) << 16) | (((t->al + max(t->trtp, (u32)4)) / 2) << 8);
	writel(tmp, (void *)DDR_UMCTL2_DRAMTMG1);

	tmp = (tck_to_uctl2(t->rl + ddr->bl / 2 + 2 - t->wl) << 8) | (tck_to_uctl2(t->cwl + ddr->bl / 2 + t->twtr) << 0);
	writel(tmp, (void *)DDR_UMCTL2_DRAMTMG2);

	tmp = (tck_to_uctl2(t->tmrd) << 12) | (tck_to_uctl2(t->tmod) << 0);
	writel(tmp, (void *)DDR_UMCTL2_DRAMTMG3);

	if (t->trcd > t->al)
		tmp = tck_to_uctl2(t->trcd - t->al) << 24;
	else
		tmp = 1 << 24;
	tmp |= (tck_to_uctl2(t->tccd) << 16) | (tck_to_uctl2(t->trrd) << 8) | ((t->trp / 2 + 1) << 0);
	writel(tmp, (void *)DDR_UMCTL2_DRAMTMG4);

	tmp = (tck_to_uctl2(t->tcksrx) << 24) | (tck_to_uctl2(t->tcksre) << 16) | (tck_to_uctl2(t->tckesr) << 8) | (tck_to_uctl2(t->tcke) << 0);
	writel(tmp, (void *)DDR_UMCTL2_DRAMTMG5);

	tmp = (tck_to_x32(t->txsdll) << 8) | (tck_to_x32(t->txs) << 0);
	writel(tmp, (void *)DDR_UMCTL2_DRAMTMG8);

	//tmp = (division(t->tzqoper, 2) << 16) | (division(t->tzqcs, 2) << 0);
	tmp = (0x1 << 31) | (0x1 << 30) | (tck_to_uctl2(t->tzqoper) << 16) | (tck_to_uctl2(t->tzqcs) << 0);
	writel(tmp, (void *)DDR_UMCTL2_ZQCTL0);
	//ZQCTL1 use default value

	tmp = (0x2 << 24) | (((t->rl + (t->rl & 1) - 4) / 2) << 16) | (0x1 << 8) | (((t->wl + (t->wl & 1) - 4) / 2) << 0);
	writel(tmp, (void *)DDR_UMCTL2_DFITMG0);
	//DFITMG1 use default value
	//tmp = (1 << 16) | (1 << 8) | (1 << 0);
	//writel(tmp, (void *)DDR_UMCTL2_DFITMG1);

	writel(0x07002021, (void *)DDR_UMCTL2_DFILPCFG0);
	//writel(0x00400003, (void *)DDR_UMCTL2_DFIUPD0);
	writel(0x00400003, (void *)DDR_UMCTL2_DFIUPD0);
	//important timing, and no good reason
	writel(0x000800ff, (void *)DDR_UMCTL2_DFIUPD1);
	//writel(0x80000000, (void *)DDR_UMCTL2_DFIUPD2);
	writel(0x00000000, (void *)DDR_UMCTL2_DFIUPD2);

#if 1
	writel(0x00080808, (void *)DDR_UMCTL2_ADDRMAP1); //bank 0~2 in HIF bit[10~12]
	writel(0x00000000, (void *)DDR_UMCTL2_ADDRMAP2); //column 2~5 in HIF bit[2~5], column 0~1 in HIF bit[0~1] and this cann't be modified
	writel(0x00000000, (void *)DDR_UMCTL2_ADDRMAP3); //column 6~9 in HIF bit[6~9]
	writel(0x00000f0f, (void *)DDR_UMCTL2_ADDRMAP4); //column bit12 bit13 not used
	writel(0x07070707, (void *)DDR_UMCTL2_ADDRMAP5); //row 0~11 in HIF bit[13~24]
	writel(0x0f0f0707, (void *)DDR_UMCTL2_ADDRMAP6); //row 12~13 in HIF bit[25~26], row 14~15 not used
#else
	writel(0x00020202, (void *)DDR_UMCTL2_ADDRMAP1); //bank 0~2 in HIF bit[4~6]
	writel(0x03030000, (void *)DDR_UMCTL2_ADDRMAP2); //column 2~3 in HIF bit[2~3], column 4~5 in HIF bit[7~8], column 0~1 in HIF bit[0~1] and this cann't be modified
	writel(0x03030303, (void *)DDR_UMCTL2_ADDRMAP3); //column 6~9 in HIF bit[9~12]
	writel(0x00000f0f, (void *)DDR_UMCTL2_ADDRMAP4); //column bit12 bit13 not used
	writel(0x07070707, (void *)DDR_UMCTL2_ADDRMAP5); //row 0~11 in HIF bit[13~24]
	writel(0x0f0f0707, (void *)DDR_UMCTL2_ADDRMAP6); //row 12~13 in HIF bit[25~26], row 14~15 not used
#endif
	tmp = 0;
	if (type == ddr2) {
		if (ddr->freq == 533)
			tmp = (7 << 24) | ((t->cwl + t->al - 5) << 16) | (7 << 8) | ((t->cl + t->al - 5) << 2);
		else if (ddr->freq == 400)
			tmp = (6 << 24) | ((t->cwl + t->al - 4) << 16) | (6 << 8) | ((t->cl + t->al - 4) << 2);
		else if (ddr->freq < 400)
			tmp = (5 << 24) | ((t->cwl + t->al - 3) << 16) | (6 << 8) | ((t->cl + t->al - 4) << 2);
	} else if (type == ddr3)
		tmp = (0x6 << 24) | (0x6 << 8) | ((t->cl - t->cwl) << 2);
	writel(tmp, (void *)DDR_UMCTL2_ODTCFG); //ODTCFG:

	tmp = (0x1 << 4) | (0x1 << 0);
	writel(tmp, (void *)DDR_UMCTL2_ODTMAP); //rank otd config, ODTMAP:

	//SCHED and SCHED1 use default value
	//PERFHPR1, PERFLPR1 and PERFWR1 use default value
	//DBG0 and DBG1 use default value

	//port config
	writel(0x00000000, (void *)DDR_UMCTL2_PCCFG);
	//cpu
	writel(0x00004014, (void *)DDR_UMCTL2_PCFGR_0);
	writel(0x00004018, (void *)DDR_UMCTL2_PCFGW_0);
	writel(0x00000001, (void *)DDR_UMCTL2_PCTRL_0);
	//npu and gmac
	writel(0x00004024, (void *)DDR_UMCTL2_PCFGR_1);
	writel(0x00004028, (void *)DDR_UMCTL2_PCFGW_1);
	writel(0x00000001, (void *)DDR_UMCTL2_PCTRL_1);
	//gdma and wifi
	writel(0x00004034, (void *)DDR_UMCTL2_PCFGR_2);
	writel(0x00004038, (void *)DDR_UMCTL2_PCFGW_2);
	writel(0x00000001, (void *)DDR_UMCTL2_PCTRL_2);
	//sdio and usb
	writel(0x00004044, (void *)DDR_UMCTL2_PCFGR_3);
	writel(0x00004048, (void *)DDR_UMCTL2_PCFGW_3);
	writel(0x00000001, (void *)DDR_UMCTL2_PCTRL_3);
}

static void set_phy_reg(struct ddr_info *ddr)
{
	//NOTE: set phy reg, such as DXnGCR, DCR, PTR*, MR*, DTPR*, etc.
	struct ddr_timing *t = &ddr->timing;
	u32 tck = t->tck;
	u32 freq = ddr->freq;
	u32 tmp = 0;

	tmp = readl((void *)DDR_PHY_PLLCR) & (~(3 << 18));
	if (freq < 450)
		tmp |= 3 << 18;
	else if (freq < 670)
		tmp |= 1 << 18;
	writel(tmp, (void *)DDR_PHY_PLLCR);

	if (ddr->type == ddr2)
		tmp = 0x2;
	else if (ddr->type == ddr3)
		tmp = 0x3;
	else {
		error("wrong ddr type!!!\n");
	}
	//FIXME: is 8 bank? Don't set bytemask
	tmp |= 1 << 3;
	writel(tmp, (void *)DDR_PHY_DCR); //8-bank

	writel(t->mr[0], (void *)DDR_PHY_MR0);
	writel(t->mr[1], (void *)DDR_PHY_MR1);
	writel(t->mr[2], (void *)DDR_PHY_MR2);
	writel(t->mr[3], (void *)DDR_PHY_MR3);

	tmp = (t->trtp << 0) | (t->twtr << 4) | (t->trp << 8) | (t->trcd << 12) | (t->tras << 16) | (t->trrd << 22) | (t->trc << 26);
	writel(tmp, (void *)DDR_PHY_DTPR0);

	tmp = (t->tfaw << 5) | (t->trfc << 11);
	if (ddr->type == ddr2)
		tmp |= (t->tmrd << 0) | (t->tmod << 2) | (40 << 20) | (8 << 26) | (0 << 30);
	else if (ddr->type == ddr3)
		tmp |= ((t->tmrd - 4) << 0) | ((t->tmod - 12) << 2) | (t->twlmrd << 20) | (t->twlo << 26);
	writel(tmp, (void *)DDR_PHY_DTPR1);

	if (ddr->type == ddr2)
		tmp = (t->txs << 0) | (t->txp << 10) | (t->tcke << 15) | (t->tdllk << 19) | (0 << 31);
	else if (ddr->type == ddr3)
		tmp = (max(t->txs, t->txsdll) << 0) | (max(t->txp, t->txpdll) << 10) | (t->tckesr << 15) | (t->tdllk << 19) | ((t->tccd - 4) << 31);
	writel(tmp, (void *)DDR_PHY_DTPR2);

	tmp = (DIV_ROUND_UP(freq * 16, 1066) << 0) | (DIV_ROUND_UP(freq * 2134, 1066) << 6) | (DIV_ROUND_UP(534 * freq, 1066) << 21);
	writel(tmp, (void *)DDR_PHY_PTR0);

	tmp = ((DIV_ROUND_UP(4800 * freq, 1066)) << 0) | ((DIV_ROUND_UP(freq * 53334, 1066)) << 16);
	writel(tmp, (void *)DDR_PHY_PTR1);

	//PTR2 uses default value
	if (ddr->type == ddr2)
		tmp = (us_to_tck(200) << 0) | (ns_to_tck(400) << 20);
	else if (ddr->type == ddr3)
		tmp = (us_to_tck(500) << 0) | (t->txpr << 20);
	writel(tmp, (void *)DDR_PHY_PTR3);

	tmp = (us_to_tck(200) << 0) | (us_to_tck(1) << 18);
	writel(tmp, (void *)DDR_PHY_PTR4);

	//set IO mode
	tmp = readl((void *)DDR_PHY_PGCR1);
	tmp &= ~(0x3 << 7);
	if (ddr->type == ddr3)
		tmp = tmp | (0x1 << 7);
	writel(tmp, (void *)DDR_PHY_PGCR1);

	//check if there is some regs have not be configed?
	tmp = readl((void *)DDR_PHY_PGCR2);
	tmp = tmp & 0xfffc0000;
	tmp |= (9 * t->trefi - 400) << 0;
	writel(tmp, (void *)DDR_PHY_PGCR2);

	tmp = readl((void *)DDR_PHY_DSGCR);
	tmp &= ~((1 << 0) | (1 << 2));
	writel(tmp, (void *)DDR_PHY_DSGCR);
}

void ddr_phy_init(struct ddr_info *ddr)
{
	writew((1 << 1) | (1 << 0), (void *)MEM_RESET);

	//wait phy init done
	while ((readl((void *)DDR_PHY_PGSR0) & 0xf) != 0xf) {
		delay(1);
	}

        set_phy_reg(ddr);
	//trigger sdram init performed by ddr controller
	writel(0x40001, (void *)DDR_PHY_PIR);
	while (!(readl((void *)DDR_PHY_PGSR0) & 0x1))
		;
}

void umctl2_init_ddr(void)
{
	set_quasi_dynamic_reg(0x1, DDR_UMCTL2_DFIMISC);

	//wait for controller to move to "normal" mode
	while ((readl((void *)DDR_UMCTL2_STAT) & 0x3) != 0x1)
		;
}

void ddr_training(void)
{
	u32 tmp;

	writel(0x0, (void *)DDR_UMCTL2_PWRCTL);   //diasble low power function
	writel(0x1, (void *)DDR_UMCTL2_RFSHCTL3); //disable auto-refresh
	set_quasi_dynamic_reg(0x0, DDR_UMCTL2_DFIMISC);

	//program DTCR
	tmp = readl((void *)DDR_PHY_DTCR);
	tmp &= ~((0xff << 24) | (0x1 << 22));
	tmp |= (0x8 << 28) | (0x1 << 24);
	writel(tmp, (void *)DDR_PHY_DTCR);

	//printf("ddr training start!!!\n");
	//specify which training steps to run
	#ifdef CONFIG_SFAX8_DDR2
	tmp = (0x7a << 9) | (0x1 << 18) | (0x1 << 0);
	#else
	tmp = (0x7f << 9) | (0x1 << 18) | (0x1 << 0);
	#endif
	writel(tmp, (void *)DDR_PHY_PIR);

	while (!(readl((void *)DDR_PHY_PGSR0) & 0x1)) {
		delay(10);
	}
	//wait for completion of training sequence
	tmp = readl((void *)DDR_PHY_PGSR0);
	#ifdef CONFIG_SFAX8_DDR2
	if ((tmp & 0x0ff00f41) != 0x00000f41) {
	#else
	if ((tmp & 0x0ff00fe1) != 0x00000fe1) {
	#endif
		error("DDR training error(%#x)!!!\n", tmp);
	} else
		printf("DDR training success\n");

	writel(0x0, (void *)DDR_UMCTL2_RFSHCTL3); //enable auto-refresh
	//Enable power save: auto power-down mode.
	tmp = (1 << 1);
	writel(tmp, (void *)DDR_UMCTL2_PWRCTL);
}

typedef struct clk_cfg_t
{
	u32 cfg;
	u32 div;
}clk_cfg;

static volatile clk_cfg clks[8] = {{0, 0}};

static u8 saved = 0;
//ddr clk config, now just for ddr3-1333, to be updated
void set_pll_ratio(u32 freq, u8 need_save, u8 push_phase)
{
	u32 i;
	u32 reg;
	u32 value;
	u32 offset = 1;
	long long pll_para;

	if (saved) {
		//resume clks prament to default pll
		writel(clks[0].div, (void *)CPU_CLK_DIV);
		writel(clks[1].div, (void *)PBUS_CLK_DIV);
		writel(clks[2].div, (void *)MEM_PHY_CLK_DIV);
		writel(clks[3].div, (void *)BUS1_XN_CLK_DIV);
		writel(clks[4].div, (void *)BUS2_XN_CLK_DIV);
		writel(clks[5].div, (void *)BUS3_XN_CLK_DIV);

		writel(clks[0].cfg, (void *)CPU_CLK_CONFIG);
		writel(clks[1].cfg, (void *)PBUS_CLK_CONFIG);
		writel(clks[2].cfg, (void *)MEM_PHY_CLK_CONFIG);
		writel(clks[3].cfg, (void *)BUS1_XN_CLK_CONFIG);
		writel(clks[4].cfg, (void *)BUS2_XN_CLK_CONFIG);
		writel(clks[5].cfg, (void *)BUS3_XN_CLK_CONFIG);

		//disable ddr pll bypass,use pll frequency,change to 1596M
		value = readl((void *)DDR_PLL_CONFIG);
		value = value & (~PLL_CFG_BYPASS);
		writel(value, (void *)DDR_PLL_CONFIG);
	}

	//enabe all power down--->close clk
	reg = PLL_BASE + offset * PLL_OFFSET + 0 * PLL_REG_WID;
	//pll red read test
	value = readl((void *)reg) | (PLL_PD_ALL_PD);
	writeb(value, (void *)reg);

	//clear load bit
	reg = PLL_BASE + offset * PLL_OFFSET + 7 * PLL_REG_WID;
	value = readl((void *)reg) & (~PLL_CFG_LOAD);
	writeb(value, (void *)reg);

//for 6MHz OSC, use pll_para "0x510000001BC" to set ddr_pll to 1332MHz
#ifdef SFA18_CRYSTAL_6M //for crystal 6M
	switch (freq) {
	case 240:
		pll_para = 0x49000000028;
		break;
	case 667:
	case 333:
		pll_para = 0x490000000DE;
		break;
	default:
		pll_para = 0x4900000010A;
		break;
	};
#else //for crystal 12M
	switch (freq) {
	case 240:
		pll_para = 0x49000000014;
		break;
	case 667:
	case 333:
		pll_para = 0x4900000006F;
		break;
	default:
		pll_para = 0x49000000085;
		break;
	};
#endif

	for (i = 0; i < 6; i++) {
		writeb(pll_para, (void *)CPU_PLL_PARA70 + 4 * i + offset * PLL_OFFSET);
		pll_para = pll_para >> 8;
	}

	//set load bit
	reg = PLL_BASE + offset * PLL_OFFSET + 7 * PLL_REG_WID;
	value = readl((void *)reg) | (PLL_CFG_LOAD);
	writeb(value, (void *)reg);

	//clear all powerdown bit
	reg = PLL_BASE + offset * PLL_OFFSET + 0 * PLL_REG_WID;
	value = readl((void *)reg) & (~(PLL_PD_ALL_PD));
	writeb(value, (void *)reg);

	if (push_phase) {
		writeb(0x32, (void *)DDR_PLL_POWER);
		//0x19E0_1100 set to 0x20, ddr controller clock as 120MHz, 0 degree phase
		//0x19E0_1100 set to 0x30, ddr controller clock as 120MHz, 90 degree phase
		//0x19E0_1100 set to 0x40, ddr controller clock as 120MHz, 180 degree phase
		//0x19E0_1100 set to 0x50, ddr controller clock as 120MHz, 270 degree phase
		writeb(0x30, (void *)0xb9E01100); //phase clk = pll_ output / 2
	}

	if (need_save) {
		//change clks prament to ddr pll
		clks[0].cfg = readl((void *)CPU_CLK_CONFIG);
		clks[1].cfg = readl((void *)PBUS_CLK_CONFIG);
		clks[2].cfg = readl((void *)MEM_PHY_CLK_CONFIG);
		clks[3].cfg = readl((void *)BUS1_XN_CLK_CONFIG);
		clks[4].cfg = readl((void *)BUS2_XN_CLK_CONFIG);
		clks[5].cfg = readl((void *)BUS3_XN_CLK_CONFIG);
		writel(1, (void *)CPU_CLK_CONFIG);
		writel(1, (void *)PBUS_CLK_CONFIG);
		writel(1, (void *)MEM_PHY_CLK_CONFIG);
		writel(1, (void *)BUS1_XN_CLK_CONFIG);
		writel(1, (void *)BUS2_XN_CLK_CONFIG);
		writel(1, (void *)BUS3_XN_CLK_CONFIG);

		clks[0].div = readl((void *)CPU_CLK_DIV);
		clks[1].div = readl((void *)PBUS_CLK_DIV);
		clks[2].div = readl((void *)MEM_PHY_CLK_DIV);
		clks[3].div = readl((void *)BUS1_XN_CLK_DIV);
		clks[4].div = readl((void *)BUS2_XN_CLK_DIV);
		clks[5].div = readl((void *)BUS3_XN_CLK_DIV);
		writel(0, (void *)CPU_CLK_DIV);
		writel(19, (void *)PBUS_CLK_DIV);
		writel(19, (void *)MEM_PHY_CLK_DIV);
		writel(19, (void *)BUS1_XN_CLK_DIV);
		writel(19, (void *)BUS2_XN_CLK_DIV);
		writel(19, (void *)BUS3_XN_CLK_DIV);

		saved = 1;
	}
	//waiting for pll locked
	reg = PLL_BASE + offset * PLL_OFFSET + 8 * PLL_REG_WID;
	while (!readl((void *)reg));

	return;
}

void set_ddr_clock(u32 freq)
{
	u32 div;

	set_pll_ratio(freq, 0, 0);

	if (freq == 667)
		div = 2;
	else if (freq == 333)
		div = 4;
	else
		div = DIV_ROUND_UP(1596, freq);
#ifndef CONFIG_TARGET_SFA18_MPW0
	div *= 2;
#endif
	writel(div - 1, (void *)MEM_PHY_CLK_DIV);
	printf("MEM_PHY_CLK_DIV = %#x\n", readl((void *)MEM_PHY_CLK_DIV));
}

void ddr_common_init(struct ddr_info *ddr)
{
	// assert presetn, core_ddrc_rstn, aresetn
	writew(0x1, (void *)CPU_RESET);  // notice: set bit[1:1] to 0
	writew(0x0, (void *)BUS1_RESET);	 // reset port 1
	writew(0x0, (void *)BUS2_RESET);	 // reset port 2
	writew(0x0, (void *)BUS3_RESET);	 // reset port 3
	writew(0x0, (void *)MEM_RESET);  // notice: set bit[1:0] to 0
	writew(0x1, (void *)PBUS_RESET); // notice: set bit[1:1] to 0
        delay(1);

	// deassert presetn
	writew(0x3, (void *)PBUS_RESET); // notice: set bit[1:1] to 1
        delay(10);

	set_umctl2_reg(ddr);
	// deassert core_ddrc_rstn
	writew(0x1, (void *)MEM_RESET); // notice: set bit[1:0] to 3
	writew(0x3, (void *)CPU_RESET); // notice: set bit[1:1] to 1
	writew(0x1, (void *)BUS1_RESET);	// reset port 1
	writew(0x1, (void *)BUS2_RESET);	// reset port 2
	writew(0x1, (void *)BUS3_RESET);	// reset port 3

	ddr_phy_init(ddr);

	umctl2_init_ddr();

	ddr_training();
}
