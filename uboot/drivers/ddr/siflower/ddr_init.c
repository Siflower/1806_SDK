#include <common.h>
#include <asm/io.h>
#include <mach/sys.h>
#include "ddr_regs.h"

/* ----------------------local funciton--------------------------------------- */
//delay function
static void delay(u32 count)
{
	return;
}

#if 0
//change clock frequency when UMCTL2_FAST_FREQUENCY_CHANGE = 1
static void fast_change_clock_frequency(struct ddr_info *ddr)
{
	struct ddr_timing *timing = &ddr->timing;
	u32 tmp = 0;
	u16 i = 0, mr[4] = {0};					//value of mr is undetermined

	ddr->freq = 667;						//new frequency

	//new timing parameters
	//nothing to be changed

	//step 1: program shadow timing registers with the timing settings required for the second clock frequency
	//i.e. ddr->freq = 400, then...
	tmp = (DIV_ROUND_UP(timing->trefi / 2, 32) << 16) | (DIV_ROUND_UP(timing->trfc, 2) << 0);
	writel(tmp, (void *)DDR_UMCTL2_RFSHTMG + SHADOW);

	tmp = (((timing->twr > 8) ? DIV_ROUND_UP(timing->twr, 2) : timing->twr - 4) << (9 + 16)) | (0x1 << (8 + 16)) | ((timing->cl - 4) << (4 + 16));
	writel(tmp, (void *)DDR_UMCTL2_INIT3 + SHADOW);
	mr[0] = (tmp & 0xffff0000) >> 16;
	mr[1] = tmp & 0x0000ffff;

	tmp = ((timing->cwl - 5) << (3 + 16));			// the value of t_cwl set by ivan does not match the ddr3 spec perfectly, but does not matter, i think
	writel(tmp, (void *)DDR_UMCTL2_INIT4 + SHADOW);
	mr[2] = (tmp & 0xffff0000) >> 16;
	mr[3] = tmp & 0x0000ffff;

	tmp = (((timing->wl + (ddr->bl / 2) + timing->twr) / 2) << 24) | (DIV_ROUND_UP(timing->tfaw, 2) << 16) | (((((timing->tras_max - 1) / 2) > 0x7f) ? 0x7f : ((timing->tras_max - 1) / 2)) << 8) | (DIV_ROUND_UP(timing->tras_min, 2) << 0);		//the t_ras_max in ddr3 spec and the datasheet are different, use which one?
	writel(tmp, (void *)DDR_UMCTL2_DRAMTMG0 + SHADOW);

	tmp = (DIV_ROUND_UP(timing->txpdll, 2) << 16) | (((timing->al + ((timing->trtp > 4) ? timing->trtp : 4)) / 2) << 8) | (DIV_ROUND_UP(timing->trc, 2) << 0);
	writel(tmp, (void *)DDR_UMCTL2_DRAMTMG1 + SHADOW);

	tmp = (DIV_ROUND_UP(timing->rl + (ddr->bl / 2) + 2 - timing->wl, 2) << 8) | (DIV_ROUND_UP(timing->cwl + (ddr->bl / 2) + timing->twtr, 2) << 0);
	writel(tmp, (void *)DDR_UMCTL2_DRAMTMG2 + SHADOW);

	tmp = (DIV_ROUND_UP(timing->tmrd, 2) << 12) | (DIV_ROUND_UP(timing->tmod, 2) << 0);
	writel(tmp, (void *)DDR_UMCTL2_DRAMTMG3 + SHADOW);

	tmp = (((timing->trcd > timing->al) ? DIV_ROUND_UP(timing->trcd - timing->al, 2) : 0x1) << 24) | (DIV_ROUND_UP(timing->tccd, 2) << 16) | (DIV_ROUND_UP(timing->trrd, 2) << 8) | (((timing->trp / 2) + 1) << 0);
	writel(tmp, (void *)DDR_UMCTL2_DRAMTMG4 + SHADOW);

	tmp = (DIV_ROUND_UP(timing->tcksrx, 2) << 24) | (DIV_ROUND_UP(timing->tcksre, 2) << 16) | (DIV_ROUND_UP(timing->tckesr, 2) << 8) | (DIV_ROUND_UP(timing->tcke, 2) << 0);
	writel(tmp, (void *)DDR_UMCTL2_DRAMTMG5 + SHADOW);

	tmp = (DIV_ROUND_UP(DIV_ROUND_UP(timing->txsdll, 2), 32) << 8) | (DIV_ROUND_UP(DIV_ROUND_UP(timing->txs, 2), 32) << 0);
	writel(tmp, (void *)DDR_UMCTL2_DRAMTMG8 + SHADOW);

	tmp = (0x1 << 31) | (0x1 << 30) | (DIV_ROUND_UP(timing->tzqoper, 2) << 16) | (DIV_ROUND_UP(timing->tzqcs, 2) << 0);
	writel(tmp, (void *)DDR_UMCTL2_ZQCTL0 + SHADOW);

	tmp = (0x2 << 24) | ((((timing->rl % 2) == 0) ? ((timing->rl - 4) / 2) : ((timing->rl - 3) / 2)) << 16) | (0x1 << 8) | ((((timing->wl % 2) == 0) ? ((timing->wl - 4) / 2) : ((timing->wl - 3) / 2)) << 0);
	writel(tmp, (void *)DDR_UMCTL2_DFITMG0 + SHADOW);

	//DFITMG1(SHADOW) use default value

	tmp = (0x6 << 24) | (0x6 << 8) | ((timing->cl - timing->cwl) << 2);
	writel(tmp, (void *)DDR_UMCTL2_ODTCFG + SHADOW);

	//step 2: block traffic on AXI ports
	writel(0x00000000, (void *)DDR_UMCTL2_PCTRL_0);
	writel(0x00000000, (void *)DDR_UMCTL2_PCTRL_1);
	writel(0x00000000, (void *)DDR_UMCTL2_PCTRL_2);
	writel(0x00000000, (void *)DDR_UMCTL2_PCTRL_3);

	//step 3: wait until all AXI ports are idle
	while((readl((void *)DDR_UMCTL2_PSTAT) & 0x000f000f) != 0);				//here we consider all of 4 ports, you can also consider port 0 only

	//step 4: enter the self-refresh mode
	tmp = readl((void *)DDR_UMCTL2_PWRCTL);
	tmp = tmp | (0x1 << 5);
	writel(tmp, (void *)DDR_UMCTL2_PWRCTL);

	//step 5: wait until in self-refresh mode
	while((readl((void *)DDR_UMCTL2_STAT) & 0x3) != 0x3);
	//check if transition to self-refresh was due to software
	while((readl((void *)DDR_UMCTL2_STAT) & 0x30) != 0x20);

	//step 6: do not dequeue any transactions from the CAM
	tmp = readl((void *)DDR_UMCTL2_DBG1);
	tmp = tmp | (0x1 << 0);
	writel(tmp, (void *)DDR_UMCTL2_DBG1);

	//step 7: ensure all outstanding commands have been sent on the DFI
	while((readl((void *)DDR_UMCTL2_DBGCAM) & 0x30000000) != 0x30000000);
	while((readl((void *)DDR_UMCTL2_DBGCAM) & 0x30000000) != 0x30000000);

	//step 8: change the clock frequency to the DWC_ddr_umctl2 core
	//configurate pll system management register
	set_pll_ratio(ddr->freq, 0, 0);

	//step 10: select shadow registers
	tmp = readl((void *)DDR_UMCTL2_MSTR);
	tmp = tmp | (0x1 << 29);
	writel(tmp, (void *)DDR_UMCTL2_MSTR);

	//step 11: if required, trigger the initialization in the PHY
	//to be determined...
	tmp = (0x1 << 0) | (0x1 << 1) | (0x1 << 4) | (0x1 << 5) | (0x1 << 6);
	writel(tmp, (void *)DDR_PHY_PIR);					//trigger delay line calibration
	delay(5);
	while((readl((void *)DDR_PHY_PGSR0) & 0xf) != 0xf)       //wait for delay line calibration done
		delay(1);
	//ddr_training();

	//step 12: exit self-refresh
	tmp = readl((void *)DDR_UMCTL2_PWRCTL);
	tmp = tmp & (~(0x1 << 5));
	writel(tmp, (void *)DDR_UMCTL2_PWRCTL);
	//wait until no longer in self-refresh mode
	while((readl((void *)DDR_UMCTL2_STAT) & 0x3) == 0x3);

	//step 13: update MR register settings of the DRAM
	//select rank 0 only
	tmp = readl((void *)DDR_UMCTL2_MRCTRL0);
	tmp = tmp & (~(0xff << 4));
	tmp = tmp | (0x1 << 4);
	writel(tmp, (void *)DDR_UMCTL2_MRCTRL0);
	//set MR0, MR1, MR2, MR3
	for(i = 0; i < 4; i++)
	{
		while((readl((void *)DDR_UMCTL2_MRSTAT) & 0x1) != 0x0);
		writel(mr[i], (void *)DDR_UMCTL2_MRCTRL1);
		tmp = readl((void *)DDR_UMCTL2_MRCTRL0);
		tmp = tmp & (~(0xf << 12));
		tmp = tmp | (i << 12);
		writel(tmp, (void *)DDR_UMCTL2_MRCTRL0);
		tmp = readl((void *)DDR_UMCTL2_MRCTRL0);
		tmp = tmp | (0x1 << 31);
		writel(tmp, (void *)DDR_UMCTL2_MRCTRL0);
	}

	//step 14: enable dequeuing of CAM commands
	tmp = readl((void *)DDR_UMCTL2_DBG1);
	tmp = tmp & (~(0x1 << 0));
	writel(tmp, (void *)DDR_UMCTL2_DBG1);

	//step 15: reenable traffic on AXI ports
	writel(0x00000001, (void *)DDR_UMCTL2_PCTRL_0);
	writel(0x00000001, (void *)DDR_UMCTL2_PCTRL_1);
	writel(0x00000001, (void *)DDR_UMCTL2_PCTRL_2);
	writel(0x00000001, (void *)DDR_UMCTL2_PCTRL_3);

	ddr_training();
}

static void print_umctl2_log(void)
{
	u32 value;
	int i;
	printf("================UMCTL================\n");
	for (i = 0; i < ARRAY_NUM(umctl2); i++) {
		value = readl((void *)umctl2[i].addr);
        printf("%s: %#010x\n", umctl2[i].name, value);
	}
	printf("==============UMCTL END==============\n");
}

static void print_phy_log(void)
{
	u32 value;
	int i;
	printf("=================PHY=================\n");
	for (i = 0; i < ARRAY_NUM(phy); i++) {
		value = readl((void *)phy[i].addr);
        printf("%s: %#010x\n", phy[i].name, value);
	}
	printf("===============PHY END===============\n");
}
#endif
void ddr_test(void)
{
	void *base = (void *)0xa0000000;
	void *addr;
	#ifdef CONFIG_SFAX8_DDR2
	u32 ddr_size_mask = 0x7fffff0;
	#else
	u32 ddr_size_mask = 0xffffff0;
	#endif
	printf("ddr test\n");

	addr = base + (rand() & ddr_size_mask);
	*(volatile u8*)addr = 0x55;
	printf("DR1BW %p OK\n", addr);

	//print_umctl2_log();
	//print_phy_log();

	if(*(volatile u8*)addr != 0x55) {
		printf("DR1BR FAIL\n");
		//hang();
	}
	else
		printf("DR1BR OK\n");

	addr = base + (rand() & ddr_size_mask);
	*(volatile u16*)addr = 0xaaaa;
	printf("DR2BW %p OK\n", addr);

	if(*(volatile u16*)addr != 0xaaaa) {
		printf("DR2BR FAIL\n");
		//hang();
	}
	else
		printf("DR2BR OK\n");

	addr = base + (rand() & ddr_size_mask);
	*(volatile u32*)addr = 0xaaaa5555;
	printf("DR4BW %p OK\n", addr);

	if(*(volatile u32*)addr != 0xaaaa5555) {
		printf("DR4BR FAIL\n");
		//hang();
	}
	else
		printf("DR4BR OK\n");

	addr = base + (rand() & ddr_size_mask);
	*(volatile u64*)addr = 0xaaaa55555555aaaa;
	printf("DR8BW %p OK\n", addr);

	if(*(volatile u64*)addr != 0xaaaa55555555aaaa) {
		printf("DR8BR FAIL\n");
		//hang();
	}
	else
		printf("DR8BR OK\n");
}

void ddr_init(void)
{
#ifdef CONFIG_SFAX8_DDR2
	struct ddr_info ddr = {
			.size = 128,
			/* This is DDR2 */
			.freq = 400,
			.rank_num = 1,
			.bl = 8,
			.type = ddr2,
	};
	printf("ddr2 init start\n");
#else
	struct ddr_info ddr = {
			.size = 256,
			/* This is DDR3 */
			.freq = 400,
			.rank_num = 1,
			.bl = 8,
			.type = ddr3,
	};
	printf("ddr3 init start\n");
#endif

	// set ddr clock
#ifdef CONFIG_TARGET_SFA18_MPW0
	set_pll_ratio(240, 1, 1);
#else
	//set_pll_ratio(240, 1, 0);
	set_ddr_clock(ddr.freq);
#endif

	ddr_set_timing(&ddr);

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

	set_umctl2_reg(&ddr);
	// deassert core_ddrc_rstn
	writew(0x1, (void *)MEM_RESET); // notice: set bit[1:0] to 3
	writew(0x3, (void *)CPU_RESET); // notice: set bit[1:1] to 1
	writew(0x1, (void *)BUS1_RESET);	// reset port 1
	writew(0x1, (void *)BUS2_RESET);	// reset port 2
	writew(0x1, (void *)BUS3_RESET);	// reset port 3

	ddr_phy_init(&ddr);

#ifdef CONFIG_TARGET_SFA18_MPW0
	set_ddr_clock(ddr.freq);
	// trigger delay line calibration
	writel((0x1 << 0) | (0x1 << 1) | (0x7 << 4), (void *)DDR_PHY_PIR);
	// wait for delay line calibration done
	while ((readl((void *)DDR_PHY_PGSR0) & 0xf) != 0xf)
		;
#endif

	umctl2_init_ddr();

	ddr_training();
	printf("now ddr frequency is %dMHz!!!\n", ddr.freq);
	// print_umctl2_log();
	// print_phy_log();

	ddr_test();
}
