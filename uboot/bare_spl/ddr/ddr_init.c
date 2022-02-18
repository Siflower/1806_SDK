#include <common.h>
#include <io.h>
#include "ddr_regs.h"

/* ----------------------local funciton--------------------------------------- */
static u32 rand()
{
	return 0;
}

void ddr_test(void)
{
	void *base = (void *)0xa0000000;
	void *addr;
	#ifdef DDR2
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
#ifdef DDR2
	struct ddr_info ddr = {
			.size = 128,
			/* This is DDR2 */
#ifdef DDR_DISABLE_ODT
			.freq = 333,
#else
#ifdef hy5ps1g1631c
			.freq = 333,
#else
			.freq = 400,
#endif
#endif
			.bl = 8,
			.bank = 8,
	};
	printf("ddr2 %s init start %dmhz\n", DDR_MODEL, ddr.freq);
#else
	struct ddr_info ddr = {
			.size = 256,
			/* This is DDR3 */
#ifdef PERFORMANCE_MODE
			.freq = 800,
#else
#ifdef SF19A28_FULLMASK
			.freq = 533,
#else
			.freq = 400,
#endif /* SF19A28_FULLMASK */
#endif /* PERFORMANCE_MODE */
			.bl = 8,
			.bank = 8,
	};
	printf("ddr3 %s init start\n", DDR_MODEL);
#endif

	// set ddr clock
#ifdef MPW0
	set_pll_ratio(240, 1, 1);
#else
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

	// deassert presetn
	writew(0x3, (void *)PBUS_RESET); // notice: set bit[1:1] to 1

	set_umctl2_reg(&ddr);
	// deassert core_ddrc_rstn
	writew(0x1, (void *)MEM_RESET); // notice: set bit[1:0] to 3
	writew(0x3, (void *)CPU_RESET); // notice: set bit[1:1] to 1
	writew(0x1, (void *)BUS1_RESET);	// reset port 1
	writew(0x1, (void *)BUS2_RESET);	// reset port 2
	writew(0x1, (void *)BUS3_RESET);	// reset port 3

#ifdef DDR_BIST
	bist();
#endif
	ddr_phy_init(&ddr);

#ifdef MPW0
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
