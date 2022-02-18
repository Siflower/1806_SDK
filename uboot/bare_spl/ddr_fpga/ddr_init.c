#include <types.h>
#include <io.h>
#include <sf_mmap.h>
#include <clk.h>
#include <sys_manager.h>
#include "ddr_init.h"

/* this code is just for ddr3-1333 */
/* ----------------------local variable--------------------------------------- */
//some basic info about sdram, you can add or modify them here
//describe some infomation of ddr3
static struct ddr_info ddr3 = {
        .size = 256,							//256MB, 2Gb
        .freq = 667,							//667MHz
        .rank_num = 1,							//rank number
        .bl = 8,								//burst length
        .timing = {
        },
};

/* ----------------------local funciton--------------------------------------- */

/* division function to realize rounding up
 * @v1: dividend
 * @v2: divisor
 * return quotient
 */
static u32 division(u32 v1, u32 v2)
{
	u32 ret = 0;

	ret = v1 / v2;
	if((ret * v2) < v1)
	{
		ret = ret + 1;
	}

	return ret;
}

//set parameters in ddr_timing struct for ddr timing setting
static void ddr_set_timing(void)
{
	struct ddr_timing *timing = &ddr3.timing;

	timing->cl = 9;													//cl = 9
	timing->al = 0;
	timing->cwl = 7;												//cwl = 7
	timing->wl = timing->al + timing->cwl;							//wl = 7
	timing->rl = timing->al + timing->cl;							//rl = 9

	timing->trefi = division(7800 * ddr3.freq, 1000) - 1;			//t_refi <= 7.8us
	timing->tfaw = division(30 * ddr3.freq, 1000) + 1;				//t_faw >= 30ns
	timing->trc = division(49500 * ddr3.freq, 1000000) + 1;			//t_rc >= 49.5ns
	timing->twr = division(15 * ddr3.freq, 1000) + 1;				//t_wr >= 15ns
	timing->trfc = division(160 * ddr3.freq, 1000) + 1;				//t_rfc >= 160ns
	timing->trcd = division(13500 * ddr3.freq, 1000000) + 1;		//t_rcd >= 13.5ns
	timing->trp = division(13500 * ddr3.freq, 1000000) + 1;			//t_rp >= 13.5ns
	timing->tras_min = division(36 * ddr3.freq, 1000) + 1;			//t_ras_min = 36ns
	timing->tras_max = division(9 * 7800 * ddr3.freq, 1000) - 1;	//t_ras_max = 9 * t_refi
	timing->tras = division(100 * ddr3.freq, 1000);					//t_ras = 100ns, notice!!!
	timing->twlo = division(9 * ddr3.freq, 1000) / 2;				//0 <= t_wlo <= 9ns
	timing->twlmrd = 40 + 1;										//t_wlmrd >= 40
	timing->tmrd = 4 + 1;											//t_mrd >= 4
	timing->tmod = ((12 > division(15 * ddr3.freq, 1000)) ? 12 : division(15 * ddr3.freq, 1000)) + 1;				//t_mod >= max(12, 15ns)
	timing->tccd = 4 + 1;																							//t_ccd >= 4
	timing->tdllk = 512 + 1;																						//t_dllk >= 512
	timing->txs = ((5 > division((160 + 10) * ddr3.freq, 1000)) ? 5 : division((160 + 10) * ddr3.freq, 1000)) + 1;	//t_xs >= max(5, (t_rfc + 10ns))
	timing->txpr = ((5 > division((160 + 10) * ddr3.freq, 1000)) ? 5 : division((160 + 10) * ddr3.freq, 1000)) + 1;	//t_xpr >= max(5, (t_rfc + 10ns))
	timing->txsdll = 512 + 1;																						//t_xsdll >= t_dllk
	timing->tzqinit = ((512 > division(640 * ddr3.freq, 1000)) ? 512 : division(640 * ddr3.freq, 1000)) + 1;		//t_zqinit >= max(512, 640ns)
	timing->tzqoper = ((256 > division(320 * ddr3.freq, 1000)) ? 256 : division(320 * ddr3.freq, 1000)) + 1;		//t_zqoper >= max(256, 320ns)
	timing->tzqcs = ((64 > division(80 * ddr3.freq, 1000)) ? 64 : division(80 * ddr3.freq, 1000)) + 1;				//t_zqcs >= max(64, 80ns)
	timing->txpdll = ((10 > division(24 * ddr3.freq, 1000)) ? 10 : division(24 * ddr3.freq, 1000)) + 1;				//t_xpdll >= max(10, 24ns)
	timing->txp = ((3 > division(6 * ddr3.freq, 1000)) ? 3 : division(6 * ddr3.freq, 1000)) + 1;					//t_xp >= max(3, 6ns)
	timing->tcksre = ((5 > division(10 * ddr3.freq, 1000)) ? 5 : division(10 * ddr3.freq, 1000)) + 1;				//t_cksre >= max(5, 10ns)
	timing->tcksrx = ((5 > division(10 * ddr3.freq, 1000)) ? 5 : division(10 * ddr3.freq, 1000)) + 1;				//t_cksrx >= max(5 ,10ns)
	timing->trrd = ((4 > division(7500 * ddr3.freq, 1000000)) ? 4 : division(7500 * ddr3.freq, 1000000)) + 1;		//t_rrd >= max(4, 7.5ns)
	timing->tcke = ((3 > division(5625 * ddr3.freq, 1000000)) ? 3 : division(5625 * ddr3.freq, 1000000)) + 1;		//t_cke >= max(3, 5ns)
	timing->twtr = ((4 > division(7500 * ddr3.freq, 1000000)) ? 4 : division(7500 * ddr3.freq, 1000000)) + 1;		//t_wtr >= max(4, 7.5ns)
	timing->trtp = ((4 > division(7500 * ddr3.freq, 1000000)) ? 4 : division(7500 * ddr3.freq, 1000000)) + 1;		//t_rtp >= max(4, 7.5ns)
	timing->tckesr = ((3 > division(5625 * ddr3.freq, 1000000)) ? 3 : division(5625 * ddr3.freq, 1000000)) + 2;		//t_ckesr >= t_cke + 1
}

//ATTENTION!!! ddr training remains to be perfected
static void ddr_training(void)
{
	u32 tmp;

	//program DTCR
	tmp = readl(DDR_PHY_DTCR);
	tmp = tmp & (~((0xf << 24) | (0x1 << 22) | (0xf << 0)));
	tmp = tmp | (0x1 << 24) | (0x1 << 22) | (0xf << 0);
	writel(tmp, DDR_PHY_DTCR);

	writel(0x1, DDR_UMCTL2_RFSHCTL3);       //disable auto-refresh

	//specify which training steps to run, NOTE: just for DDR PHY on FPGA
	tmp = (0x1 << 10) | (0x1 << 0);
	writel(tmp, DDR_PHY_PIR);

	while((readl(DDR_PHY_PGSR0) & ((0x1 << 6) | (0x1 << 0))) != ((0x1 << 6) | (0x1 << 0)));       //wait for completion of training sequence

	writel(0x0, DDR_UMCTL2_RFSHCTL3);       //disable auto-refresh
}

static void set_phy_reg(void)
{
	//NOTE: set phy reg, such as DXnGCR, DCR, PTR*, MR*, DTPR*, etc.

	struct ddr_timing *timing = &ddr3.timing;
	u32 tmp = 0;

	writel(0xb, DDR_PHY_DCR);			//8-bank and ddr mode is ddr3

	tmp = (((timing->twr > 8) ? division(timing->twr, 2) : (timing->twr - 4)) << 9) | (0x1 << 8) | ((timing->cl - 4) << 4);
	writel(tmp, DDR_PHY_MR0);

	//tmp = (0x1 << 6) | (0x1 << 2);		//Rtt_Nom = RZQ/6
	tmp = (0x0 << 0);
	writel(tmp, DDR_PHY_MR1);

	//tmp = (0x1 << 9) | (timing->cwl - 5) << 3;			//Rtt_WR = RZQ/4
	tmp = (timing->cwl - 5) << 3;
	writel(tmp, DDR_PHY_MR2);

	writel(0x0, DDR_PHY_MR3);

	tmp = (timing->trtp << 0) | (timing->twtr << 4) | (timing->trp << 8) | (timing->trcd << 12) | (timing->tras << 16) | (timing->trrd << 22) | (timing->trc << 26);
	writel(tmp, DDR_PHY_DTPR0);

	tmp = ((timing->tmrd - 4) << 0) | ((timing->tmod - 12) << 2) | (timing->tfaw << 5) | (timing->trfc << 11) | (timing->twlmrd << 20) | (timing->twlo << 26);
	writel(tmp, DDR_PHY_DTPR1);

	tmp = (((timing->txs > timing->txsdll) ? timing->txs : timing->txsdll) << 0) | (((timing->txp > timing->txpdll) ? timing->txp : timing->txpdll) << 10) | (timing->tckesr << 15) | (timing->tdllk << 19);
	writel(tmp, DDR_PHY_DTPR2);

	tmp = (division(16 * ddr3.freq, 1066) << 0) | (division(4 * ddr3.freq, 2) << 6) | (division(1 * ddr3.freq, 2) << 21);
	writel(tmp, DDR_PHY_PTR0);

	tmp = (division(3 * ddr3.freq, 2) << 0) | (division(100 * ddr3.freq, 2) << 16);
	writel(tmp, DDR_PHY_PTR1);

	//PTR2 uses default value

	tmp = ((500 * ddr3.freq) << 0) | (timing->txpr << 20);
	writel(tmp, DDR_PHY_PTR3);

	tmp = ((200 * ddr3.freq) << 0) | ((1 * ddr3.freq) << 18);
	writel(tmp, DDR_PHY_PTR4);

	//set IO mode
	tmp = readl(DDR_PHY_PGCR1);
	tmp = tmp & (~(0x3 << 7));
	tmp = tmp | (0x1 << 7);
	writel(tmp, DDR_PHY_PGCR1);

	//check if there is some regs have not be configed?
	tmp = readl(DDR_PHY_PGCR2);
	tmp = tmp & 0xf00c0000;
	tmp = tmp | (0xff << 20) | (((9 * division(7800 * ddr3.freq, 1000)) - 400) << 0);
	writel(tmp, DDR_PHY_PGCR2);
}

static void set_umctl2_reg(void)
{
	struct ddr_timing *timing = &ddr3.timing;
	u32 tmp = 0;

	writel(0x0, DDR_UMCTL2_SWCTL);			//set sw_done = 0
	writel(0x0, DDR_UMCTL2_DFIMISC);		//config quasi dynamic register DFIMISC.dif_init_complete_en = 0
	writel(0x1, DDR_UMCTL2_SWCTL);			//set sw_done = 1
	while((readl(DDR_UMCTL2_SWSTAT) & 0x1) != 0x1);		//wait for sw_done_ack = 1

	tmp = (0x1 << 0) | ((ddr3.bl / 2) << 16);
	writel(tmp, DDR_UMCTL2_MSTR);

	tmp = (division(timing->trefi / 2, 32) << 16) | (division(timing->trfc, 2) << 0);
	writel(tmp, DDR_UMCTL2_RFSHTMG);

	tmp = (division(division(timing->txpr, 2), 1024) << 16) | (division(division(500 * ddr3.freq, 2), 1024) << 0);
	writel(tmp, DDR_UMCTL2_INIT0);

	tmp = (division(division(200 * ddr3.freq, 2), 1024) << 16);			//if need to be divided by 2?
	writel(tmp, DDR_UMCTL2_INIT1);

	//tmp = (((timing->twr > 8) ? division(timing->twr, 2) : timing->twr - 4) << (9 + 16)) | (0x1 << (8 + 16)) | ((timing->cl - 4) << (4 + 16)) | (0x1 << 6) | (0x1 << 2);
	tmp = (((timing->twr > 8) ? division(timing->twr, 2) : timing->twr - 4) << (9 + 16)) | (0x1 << (8 + 16)) | ((timing->cl - 4) << (4 + 16));
	writel(tmp, DDR_UMCTL2_INIT3);

	//tmp = (0x1 << (9 + 16)) | ((timing->cwl - 5) << (3 + 16));			// the value of t_cwl set by ivan does not match the ddr3 spec perfectly, but does not matter, i think
	tmp = (timing->cwl - 5) << (3 + 16);
	writel(tmp, DDR_UMCTL2_INIT4);

	tmp = (division(timing->tzqinit, 32) << 16);
	writel(tmp, DDR_UMCTL2_INIT5);

	tmp = (((timing->wl + (ddr3.bl / 2) + timing->twr) / 2) << 24) | (division(timing->tfaw, 2) << 16) | (((((timing->tras_max - 1) / 2) > 0x7f) ? 0x7f : ((timing->tras_max - 1) / 2)) << 8) | (division(timing->tras_min, 2) << 0);		//the t_ras_max in ddr3 spec and the datasheet are different, use which one?
	writel(tmp, DDR_UMCTL2_DRAMTMG0);

	tmp = (division(timing->txpdll, 2) << 16) | (((timing->al + ((timing->trtp > 4) ? timing->trtp : 4)) / 2) << 8) | (division(timing->trc, 2) << 0);
	writel(tmp, DDR_UMCTL2_DRAMTMG1);

	tmp = (division(timing->rl + (ddr3.bl / 2) + 2 - timing->wl, 2) << 8) | (division(timing->cwl + (ddr3.bl / 2) + timing->twtr, 2) << 0);
	writel(tmp, DDR_UMCTL2_DRAMTMG2);

	tmp = (division(timing->tmrd, 2) << 12) | (division(timing->tmod, 2) << 0);
	writel(tmp, DDR_UMCTL2_DRAMTMG3);

	tmp = (((timing->trcd > timing->al) ? division(timing->trcd - timing->al, 2) : 0x1) << 24) | (division(timing->tccd, 2) << 16) | (division(timing->trrd, 2) << 8) | (((timing->trp / 2) + 1) << 0);
	writel(tmp, DDR_UMCTL2_DRAMTMG4);

	tmp = (division(timing->tcksrx, 2) << 24) | (division(timing->tcksre, 2) << 16) | (division(timing->tckesr, 2) << 8) | (division(timing->tcke, 2) << 0);
	writel(tmp, DDR_UMCTL2_DRAMTMG5);

	tmp = (division(division(timing->txsdll, 2), 32) << 8) | (division(division(timing->txs, 2), 32) << 0);
	writel(tmp, DDR_UMCTL2_DRAMTMG8);

	tmp = (division(timing->tzqoper, 2) << 16) | (division(timing->tzqcs, 2) << 0);
	//tmp = (0x1 << 31) | (0x1 << 30) | (division(timing->tzqoper, 2) << 16) | (division(timing->tzqcs, 2) << 0);
	writel(tmp, DDR_UMCTL2_ZQCTL0);
	//ZQCTL1 use default value

	tmp = (0x2 << 24) | ((((timing->rl % 2) == 0) ? ((timing->rl - 4) / 2) : ((timing->rl - 3) / 2)) << 16) | (0x1 << 8) | ((((timing->wl % 2) == 0) ? ((timing->wl - 4) / 2) : ((timing->wl - 3) / 2)) << 0);
	writel(tmp, DDR_UMCTL2_DFITMG0);
	//DFITMG1 use default value

	writel(0x07002020, DDR_UMCTL2_DFILPCFG0);
	writel(0x00400003, DDR_UMCTL2_DFIUPD0);
	//writel(0xc0400003, DDR_UMCTL2_DFIUPD0);
	//important timing, and no good reason
	writel(0x00010020, DDR_UMCTL2_DFIUPD1);
	writel(0x80000000, DDR_UMCTL2_DFIUPD2);
	//writel(0x00000000, DDR_UMCTL2_DFIUPD2);

#if 1
	writel(0x00080808, (void *)DDR_UMCTL2_ADDRMAP1);			//bank 0~2 in HIF bit[10~12]
	writel(0x00000000, (void *)DDR_UMCTL2_ADDRMAP2);			//column 2~5 in HIF bit[2~5], column 0~1 in HIF bit[0~1] and this cann't be modified
	writel(0x00000000, (void *)DDR_UMCTL2_ADDRMAP3);			//column 6~9 in HIF bit[6~9]
	writel(0x00000f0f, (void *)DDR_UMCTL2_ADDRMAP4);			//column bit12 bit13 not used
	writel(0x07070707, (void *)DDR_UMCTL2_ADDRMAP5);			//row 0~11 in HIF bit[13~24]
	writel(0x0f0f0707, (void *)DDR_UMCTL2_ADDRMAP6);			//row 12~13 in HIF bit[25~26], row 14~15 not used
#else
	writel(0x00020202, (void *)DDR_UMCTL2_ADDRMAP1);			//bank 0~2 in HIF bit[4~6]
	writel(0x03030000, (void *)DDR_UMCTL2_ADDRMAP2);			//column 2~3 in HIF bit[2~3], column 4~5 in HIF bit[7~8], column 0~1 in HIF bit[0~1] and this cann't be modified
	writel(0x03030303, (void *)DDR_UMCTL2_ADDRMAP3);			//column 6~9 in HIF bit[9~12]
	writel(0x00000f0f, (void *)DDR_UMCTL2_ADDRMAP4);			//column bit12 bit13 not used
	writel(0x07070707, (void *)DDR_UMCTL2_ADDRMAP5);			//row 0~11 in HIF bit[13~24]
	writel(0x0f0f0707, (void *)DDR_UMCTL2_ADDRMAP6);			//row 12~13 in HIF bit[25~26], row 14~15 not used
#endif

	tmp = (0x6 << 24) | (0x6 << 8) | ((timing->cl - timing->cwl) << 2);
	writel(tmp, DDR_UMCTL2_ODTCFG);					//ODTCFG:

	tmp = (0x1 << 4) | (0x1 << 0);
	writel(tmp, DDR_UMCTL2_ODTMAP);					//rank otd config, ODTMAP:

	//SCHED and SCHED1 use default value
	//PERFHPR1, PERFLPR1 and PERFWR1 use default value
	//DBG0 and DBG1 use default value

	//port config
	writel(0x00000000, DDR_UMCTL2_PCCFG);
	//cpu
	writel(0x00004014, DDR_UMCTL2_PCFGR_0);
	writel(0x00004018, DDR_UMCTL2_PCFGW_0);
	writel(0x00000001, DDR_UMCTL2_PCTRL_0);
	//npu and gmac
	writel(0x00004024, DDR_UMCTL2_PCFGR_1);
	writel(0x00004028, DDR_UMCTL2_PCFGW_1);
	writel(0x00000001, DDR_UMCTL2_PCTRL_1);
	//gdma and wifi
	writel(0x00004034, DDR_UMCTL2_PCFGR_2);
	writel(0x00004038, DDR_UMCTL2_PCFGW_2);
	writel(0x00000001, DDR_UMCTL2_PCTRL_2);
	//sdio and usb
	writel(0x00004044, DDR_UMCTL2_PCFGR_3);
	writel(0x00004048, DDR_UMCTL2_PCFGW_3);
	writel(0x00000001, DDR_UMCTL2_PCTRL_3);
}

static void ddr_phy_init(void)
{
	u16 tmp;

	set_phy_reg();

	tmp = readw(MEM_RESET);
	tmp = tmp & (~(0x1 << 1));
	writew(tmp, MEM_RESET);
	tmp = tmp | (0x1 << 1);
	writew(tmp, MEM_RESET);

	set_phy_reg();

    writel(0x41, DDR_PHY_PIR);
	while((readl(DDR_PHY_PGSR0) & 0x1) != 0x1);

    writel(0x40001, DDR_PHY_PIR);
    while((readl(DDR_PHY_PGSR0) & 0x1) != 0x1);
}


static void umctl2_init_ddr(void)
{
	u32 umctl2_stat = 0;

	writel(0x0, DDR_UMCTL2_SWCTL);			//set SWCTL.sw_done to 0
	writel(0x1, DDR_UMCTL2_DFIMISC);		//indicates phy init complete and trigger sdram init
	writel(0x1, DDR_UMCTL2_SWCTL);			//set SWCTL.sw_done to 1
	//wait for SWSTAT.sw_done_ack to become 1
	while((readl(DDR_UMCTL2_SWSTAT) & 0x1) != 0x1);

	umctl2_stat = readl(DDR_UMCTL2_STAT);	//get the umctl2 status

	//detect if umctl2 is in normal mode for sdram
	while((umctl2_stat & 0x3) != 0x1)
	{
		umctl2_stat = readl(DDR_UMCTL2_STAT);		//if not, get status again, until umctl2 in normal mode for sdram
	}

	writel(0x0, DDR_UMCTL2_PWRCTL);		//diasble low power function
	writel(0x0, DDR_UMCTL2_SWCTL);          //set SWCTL.sw_done to 0
	writel(0x0, DDR_UMCTL2_DFIMISC);	//clear sdram-init-trigger bit
	writel(0x1, DDR_UMCTL2_SWCTL);          //set SWCTL.sw_done to 1
	//wait for SWSTAT.sw_done_ack to become 1
	while((readl(DDR_UMCTL2_SWSTAT) & 0x1) != 0x1);
	writel(0x0, DDR_UMCTL2_SWCTL);          //set SWCTL.sw_done to 0

	//here should be ddr training
	ddr_training();
}

void ddr_init(void)
{
	//set ddr clock
        writeb(0x4, MEM_PHY_CLK_DIV);

	//assert presetn, core_ddrc_rstn, aresetn
	writew(0x1, (void *)CPU_RESET);					//notice: set bit[1:1] to 0
	writew(0x0, BUS1_RESET);						//reset port 1
	writew(0x0, BUS2_RESET);						//reset port 2
	writew(0x0, BUS3_RESET);						//reset port 3
	writew(0x0, (void *)MEM_RESET);                 //notice: set bit[1:0] to 0
	writew(0x1, (void *)PBUS_RESET);				//notice: set bit[1:1] to 0

	//deassert presetn
	writew(0x3, (void *)PBUS_RESET);                //notice: set bit[1:1] to 1

	ddr_set_timing();

	set_umctl2_reg();

	//deassert core_ddrc_rstn
	writew(0x1, (void *)MEM_RESET);                 //notice: set bit[1:0] to 3
	writew(0x3, (void *)CPU_RESET);                 //notice: set bit[1:1] to 1
	writew(0x1, BUS1_RESET);						//reset port 1
	writew(0x1, BUS2_RESET);						//reset port 2
	writew(0x1, BUS3_RESET);						//reset port 3

	ddr_phy_init();

	umctl2_init_ddr();

}
