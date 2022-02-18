#include <common.h>
#include <io.h>
#include "ddr_regs.h"

static struct ddr_timing ddr_800Mbps = {
#undef Tck
#define Tck 2500
        .tck = 2500,

        .cl = 6,
	.trcd = ns_to_tck(15),
	.al = 0,

	.trefi = ns_to_tck(7800),
	.tfaw = ns_to_tck(45),
	.trc = ns_to_tck(60),
	.twr = ns_to_tck(15),
	.trfc = ns_to_tck(105),
	.trp = ns_to_tck(15),
	.tras_min = ns_to_tck(45),
	.tras_max = ns_to_tck(70000),
	.tmrd = 2,
	.tmod = ns_to_tck(12),
	.tccd = 2,
	.txp = 2,
	.trrd = ns_to_tck(10),
	.tcke = 3,
	.twtr = ps_to_tck(7500),
	.trtp = ps_to_tck(7500),
};


static struct ddr_timing ddr_667Mbps = {
#undef Tck
#define Tck 3000
        .tck = 3000,

        .cl = 5,
	.trcd = ns_to_tck(15),
	.al = 0,

	.trefi = ns_to_tck(7800),
	.tfaw = ns_to_tck(50),
	.trc = ns_to_tck(60),
	.twr = ns_to_tck(15),
	.trfc = ns_to_tck(105),
	.trp = ns_to_tck(15),
	.tras_min = ns_to_tck(45),
	.tras_max = ns_to_tck(70000),
	.tmrd = 2,
	.tmod = ns_to_tck(12),
	.tccd = 2,
	.txp = 2,
	.trrd = ns_to_tck(10),
	.tcke = 3,
	.twtr = ps_to_tck(7500),
	.trtp = ps_to_tck(7500),
};

static struct ddr_timing ddr_533Mbps = {
#undef Tck
#define Tck 3750
        .tck = 3750,

        .cl = 4,
	.trcd = ns_to_tck(15),
	.al = 0,

	.trefi = ns_to_tck(7800),
	.tfaw = ns_to_tck(50),
	.trc = ns_to_tck(60),
	.twr = ns_to_tck(15),
	.trfc = ns_to_tck(105),
	.trp = ns_to_tck(15),
	.tras_min = ns_to_tck(45),
	.tras_max = ns_to_tck(70000),
	.tmrd = 2,
	.tmod = ns_to_tck(12),
	.tccd = 2,
	.txp = 2,
	.trrd = ns_to_tck(10),
	.tcke = 3,
	.twtr = ps_to_tck(7500),
	.trtp = ps_to_tck(7500),
};

static struct ddr_timing ddr_400Mbps = {
#undef Tck
#define Tck 5000
        .tck = 5000,

        .cl = 4,
	.trcd = ns_to_tck(20),
	.al = 0,

	.trefi = ns_to_tck(7800),
	.tfaw = ns_to_tck(50),
	.trc = ns_to_tck(65),
	.twr = ns_to_tck(15),
	.trfc = ns_to_tck(105),
	.trp = ns_to_tck(20),
	.tras_min = ns_to_tck(45),
	.tras_max = ns_to_tck(70000),
	.tmrd = 2,
	.tmod = ns_to_tck(12),
	.tccd = 2,
	.txp = 2,
	.trrd = ns_to_tck(10),
	.tcke = 3,
	.twtr = ns_to_tck(10),
	.trtp = ps_to_tck(7500),
};


void ddr_set_timing(struct ddr_info *ddr)
{
	struct ddr_timing *t;
	switch (ddr->freq) {
	case 400:
		ddr->timing = &ddr_800Mbps;
		break;
	case 333:
		ddr->timing = &ddr_667Mbps;
		break;
	case 267:
		ddr->timing = &ddr_533Mbps;
		break;
	case 200:
		ddr->timing = &ddr_400Mbps;
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
	ddr->bank = 8;
}
