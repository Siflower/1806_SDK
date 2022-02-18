#include <common.h>
#include <io.h>
#include "ddr_regs.h"


static struct ddr_timing ddr_1066Mbps = {
#undef Tck
#define Tck 1875
        .tck = 1875,

        .cl = 7, // cl = 4
	.trcd = ps_to_tck(13125), // t_rcd >= 15ns
	.al = 0,

	.trefi = ns_to_tck(7800),     // t_refi <= 0x20
	.tfaw = ns_to_tck(45),	// t_faw = 14
	.trc = ps_to_tck(58125),      // t_rc >= 55ns
	.twr = ns_to_tck(15),		// t_wr = 4
	.trfc = ns_to_tck(105),    // t_rfc = 0x22
	.trp = ps_to_tck(13125),      // t_rp >= 15ns
	.tras_min = ns_to_tck(45),    // t_ras_min = 40ns
	.tras_max = ns_to_tck(70000), // t_ras_max = 70k ns
	.tmrd = 2,			// t_mrd >= 4,
	.tmod = 0xc, // t_mod not needed by DDR2, set to umctl2 default value
	.tccd = 2, // t_ccd = bl/2
	.tdllk = 512,	// t_dllk >= 512(not found in DDR2)
	.txpr = ns_to_tck(400),	// not used. t_xpr >= MAX(5, (t_rfc + 10ns))
	.txsdll = 200,		 // t_isxr:t_xsdll =t_is
	.txp = 2, // t_xp >= MAX(3, 6ns),t_xard
	.tcksre = 1,			 // always 1
	.tcksrx = 1,			 // always 1
	.trrd = ns_to_tck(10),	 // t_rrd >= MAX(4, 7.5ns)
	.tcke = 3,			 // all 3
	.twtr = ps_to_tck(7500),       // t_wtr >= MAX(4, 7.5ns)
	.trtp = ps_to_tck(7500),       // t_rtp >= MAX(4, 7.5ns)
};

static struct ddr_timing ddr_800Mbps = {
#undef Tck
#define Tck 2500
        .tck = 2500,

        .cl = 5, // cl = 4
	.trcd = ps_to_tck(12500), // t_rcd >= 15ns
	.al = 0,

	.trefi = ns_to_tck(7800),     // t_refi <= 0x20
	.tfaw = ns_to_tck(45),	// not found
	.trc = ps_to_tck(57500),      // t_rc >= 55ns
	.twr = ns_to_tck(15),		// t_wr = 4
	.trfc = ns_to_tck(105),    // t_rfc = 0x22
	.trp = ps_to_tck(12500),      // t_rp >= 15ns
	.tras_min = ns_to_tck(45),    // t_ras_min = 40ns
	.tras_max = ns_to_tck(70000), // t_ras_max = 70k ns
	.tmrd = 2,			// t_mrd >= 4,
	.tmod = ns_to_tck(12), // t_mod not needed by DDR2, set to umctl2 default value
	.tccd = 2, // t_ccd = bl/2
	.tdllk = 512,	// t_dllk >= 512(not found in DDR2)
	.txpr = ns_to_tck(400),	 // not used. t_xpr >= MAX(5, (t_rfc + 10ns))
	.txsdll = 200,		 // t_xsdll >= t_dllk
	.txp = 2, // t_xp >= MAX(3, 6ns),t_xard
	.tcksre = 1,			 // t_cksre >= MAX(5, 10ns)
	.tcksrx = 1,			 // t_cksrx >= MAX(5 ,10ns)
	.trrd = ns_to_tck(10),	 // t_rrd >= MAX(4, 7.5ns)
	.tcke = 3,			 // t_cke >= MAX(3, 5ns)
	.twtr = ps_to_tck(7500),       // t_wtr >= MAX(4, 7.5ns)
	.trtp = ps_to_tck(7500),       // t_rtp >= MAX(4, 7.5ns)
};


static struct ddr_timing ddr_667Mbps = {
#undef Tck
#define Tck 3000
        .tck = 3000,

        .cl = 5, // cl = 4
	.trcd = ns_to_tck(15), // t_rcd >= 15ns
	.al = 0,

	.trefi = ns_to_tck(7800),     // t_refi <= 0x20
	.tfaw = ns_to_tck(50),	// not found
	.trc = ns_to_tck(60),      // t_rc >= 55ns
	.twr = ns_to_tck(15),		// t_wr = 4
	.trfc = ns_to_tck(105),    // t_rfc = 0x22
	.trp = ns_to_tck(15),      // t_rp >= 15ns
	.tras_min = ns_to_tck(45),    // t_ras_min = 40ns
	.tras_max = ns_to_tck(70000), // t_ras_max = 70k ns
	.tmrd = 2,			// t_mrd >= 4,
	.tmod = ns_to_tck(12), // t_mod not needed by DDR2, set to umctl2 default value
	.tccd = 2, // t_ccd = bl/2
	.tdllk = 512,	// t_dllk >= 512(not found in DDR2)
	.txpr = ns_to_tck(400),	 // not used. t_xpr >= MAX(5, (t_rfc + 10ns))
	.txsdll = 200,		 // t_xsdll >= t_dllk
	.txp = 2, // t_xp >= MAX(3, 6ns),t_xard
	.tcksre = 1,			 // t_cksre >= MAX(5, 10ns)
	.tcksrx = 1,			 // t_cksrx >= MAX(5 ,10ns)
	.trrd = ns_to_tck(10),	 // t_rrd >= MAX(4, 7.5ns)
	.tcke = 3,			 // t_cke >= MAX(3, 5ns)
	.twtr = ps_to_tck(7500),       // t_wtr >= MAX(4, 7.5ns)
	.trtp = ps_to_tck(7500),       // t_rtp >= MAX(4, 7.5ns)
};


void ddr_set_timing(struct ddr_info *ddr)
{
	struct ddr_timing *t;
	switch (ddr->freq) {
	case 533:
		ddr->timing = &ddr_1066Mbps;
		break;
	case 400:
		ddr->timing = &ddr_800Mbps;
		break;
	case 333:
		ddr->timing = &ddr_667Mbps;
		break;
        default:
		printf("ERROR: DDR freq %dMHz not support!!!\n", ddr->freq);
		ddr->freq = 400;
		printf("Use default value %dMHz!!!\n", ddr->freq);
                ddr->timing = &ddr_800Mbps;
	}
	t = ddr->timing;
#undef Tck
#define Tck (t->tck)
	t->rl = t->al + t->cl;
	t->wl = t->rl - 1;
	t->cwl = t->wl - t->al;
	t->tckesr = t->tcke + 1;
	t->tras = t->tras_min;
	t->txs = MAX((u32)200, ns_to_tck(10) + t->trfc);
	ddr->bank = 4;
}
