#include <common.h>
#include <asm/io.h>
#include "ddr_regs.h"

#define SET_COMMON_PARA() t->trefi = ns_to_tck(7800); \
                        t->trfc = ns_to_tck(160); \
                        t->tras_max = 9 * t->trefi; \
                        t->tmod = max((u32)12, ns_to_tck(15)); \
                        t->txs = max((u32)5, ns_to_tck(10) + t->trfc); \
                        t->txpr = max((u32)5, ns_to_tck(10) + t->trfc); \
                        t->txsdll = 512; \
                        t->tzqinit = max((u32)512, ns_to_tck(640)); \
                        t->tzqoper = max((u32)256, ns_to_tck(320)); \
                        t->tzqcs = max((u32)64, ns_to_tck(80)); \
                        t->txpdll = max((u32)10, ns_to_tck(24)); \
                        t->tcksre = max((u32)5, ns_to_tck(10)); \
                        t->tcksrx = max((u32)5, ns_to_tck(10)); \
                        t->twtr = max((u32)4, ps_to_tck(7500)); \
                        t->trtp = max((u32)4, ps_to_tck(7500)); \
                        t->tckesr = t->tcke + 1;

static void ddr_800Mbps(struct ddr_timing *t)
{
        u32 tck = t->tck = 2500;

	t->cl = 6;
	t->al = 0;
	t->cwl = 5;
	t->wl = t->al + t->cwl;
	t->rl = t->al + t->cl;

	t->trefi = ns_to_tck(7800);
	t->tfaw = ns_to_tck(50);
	t->trc = ps_to_tck(52500);
	t->twr = ns_to_tck(15);
	t->trfc = ns_to_tck(160);
	t->trcd = ns_to_tck(15);
	t->trp = ns_to_tck(15);
	t->tras_min = ps_to_tck(37500);
	t->tras_max = 9 * t->trefi;
	t->tras = ps_to_tck(57500);
	t->twlo = ns_to_tck(5);
	t->twlmrd = 40;
	t->tmrd = 4;
	t->tmod = max((u32)12, ns_to_tck(15));
	t->tccd = 4;
	t->tdllk = 512;
	t->txs = max((u32)5, ns_to_tck(10) + t->trfc);
	t->txpr = max((u32)5, ns_to_tck(10) + t->trfc);
	t->txsdll = 512;
	t->tzqinit = max((u32)512, ns_to_tck(640));
	t->tzqoper = max((u32)256, ns_to_tck(320));
	t->tzqcs = max((u32)64, ns_to_tck(80));
	t->txpdll = max((u32)10, ns_to_tck(24));
	t->txp = max((u32)3, ps_to_tck(7500));
	t->tcksre = max((u32)5, ns_to_tck(10));
	t->tcksrx = max((u32)5, ns_to_tck(10));
	t->trrd = max((u32)4, ns_to_tck(10));
	t->tcke = max((u32)3, ps_to_tck(7500));
	t->twtr = max((u32)4, ps_to_tck(7500));
	t->trtp = max((u32)4, ps_to_tck(7500));
	t->tckesr = t->tcke + 1;
}

static void ddr_1066Mbps(struct ddr_timing *t)
{
        u32 tck = t->tck = 1875;

	t->cl = 8;
	t->al = 0;
	t->cwl = 6;
	t->wl = t->al + t->cwl;
	t->rl = t->al + t->cl;

	t->trefi = ns_to_tck(7800);
	t->tfaw = ns_to_tck(50);
	t->trc = ps_to_tck(50625);
	t->twr = ns_to_tck(15);
	t->trfc = ns_to_tck(160);
	t->trcd = ps_to_tck(13125);
	t->trp = ps_to_tck(13125);
	t->tras_min = ps_to_tck(37500);
	t->tras_max = 9 * t->trefi;
	t->tras = ps_to_tck(37500);
	t->twlo = ns_to_tck(5);
	t->twlmrd = 40;
	t->tmrd = 4;
	t->tmod = max((u32)12, ns_to_tck(15));
	t->tccd = 4;
	t->tdllk = 512;
	t->txs = max((u32)5, ns_to_tck(10) + t->trfc);
	t->txpr = max((u32)5, ns_to_tck(10) + t->trfc);
	t->txsdll = 512;
	t->tzqinit = max((u32)512, ns_to_tck(640));
	t->tzqoper = max((u32)256, ns_to_tck(320));
	t->tzqcs = max((u32)64, ns_to_tck(80));
	t->txpdll = max((u32)10, ns_to_tck(24));
	t->txp = max((u32)3, ps_to_tck(7500));
	t->tcksre = max((u32)5, ns_to_tck(10));
	t->tcksrx = max((u32)5, ns_to_tck(10));
	t->trrd = max((u32)4, ps_to_tck(7500));
	t->tcke = max((u32)3, ps_to_tck(5625));
	t->twtr = max((u32)4, ps_to_tck(7500));
	t->trtp = max((u32)4, ps_to_tck(7500));
	t->tckesr = t->tcke + 1;
}

static void ddr_1333Mbps(struct ddr_timing *t)
{
        u32 tck = t->tck = 1500;

	t->cl = 9;
	t->al = 0;
	t->cwl = 7;
	t->wl = t->al + t->cwl;
	t->rl = t->al + t->cl;

	t->trefi = ns_to_tck(7800);
	t->tfaw = ns_to_tck(45);
	t->trc = ps_to_tck(49500);
	t->twr = ns_to_tck(15);
	t->trfc = ns_to_tck(160);
	t->trcd = ps_to_tck(13500);
	t->trp = ps_to_tck(13500);
	t->tras_min = ns_to_tck(36);
	t->tras_max = 9 * t->trefi;
	t->tras = ns_to_tck(36);
	t->twlo = ns_to_tck(9);
	t->twlmrd = 40;
	t->tmrd = 4;
	t->tmod = max((u32)12, ns_to_tck(15));
	t->tccd = 4;
	t->tdllk = 512;
	t->txs = max((u32)5, ns_to_tck(10) + t->trfc);
	t->txpr = max((u32)5, ns_to_tck(10) + t->trfc);
	t->txsdll = 512;
	t->tzqinit = max((u32)512, ns_to_tck(640));
	t->tzqoper = max((u32)256, ns_to_tck(320));
	t->tzqcs = max((u32)64, ns_to_tck(80));
	t->txpdll = max((u32)10, ns_to_tck(24));
	t->txp = max((u32)3, ns_to_tck(6));
	t->tcksre = max((u32)5, ns_to_tck(10));
	t->tcksrx = max((u32)5, ns_to_tck(10));
	t->trrd = max((u32)4, ps_to_tck(7500));
	t->tcke = max((u32)3, ps_to_tck(5625));
	t->twtr = max((u32)4, ps_to_tck(7500));
	t->trtp = max((u32)4, ps_to_tck(7500));
	t->tckesr = t->tcke + 1;
}

static void ddr_1600Mbps(struct ddr_timing *t)
{
        u32 tck = t->tck = 1250;

	t->cl = 11;
	t->al = 0;
	t->cwl = 8;
	t->wl = t->al + t->cwl;
	t->rl = t->al + t->cl;

	t->tfaw = ns_to_tck(40);
	t->trc = ps_to_tck(48750);
	t->twr = ns_to_tck(15);
	t->trcd = ps_to_tck(13750);
	t->trp = ps_to_tck(13750);
	t->tras_min = ns_to_tck(35);
	t->tras = ns_to_tck(35);
	t->twlo = ps_to_tck(7500);
	t->twlmrd = 40;
	t->tmrd = 4;
	t->tccd = 4;
	t->tdllk = 512;
	t->txp = max((u32)3, ns_to_tck(6));
	t->trrd = max((u32)4, ps_to_tck(7500));
	t->tcke = max((u32)3, ps_to_tck(5000));

        SET_COMMON_PARA();
}

void ddr_set_timing(struct ddr_info *ddr)
{
	switch (ddr->freq) {
	case 400:
		ddr_800Mbps(&ddr->timing);
		break;
        case 533:
		ddr_1066Mbps(&ddr->timing);
		break;
	case 667:
		ddr_1333Mbps(&ddr->timing);
		break;
	case 800:
		ddr_1600Mbps(&ddr->timing);
		break;
        default:
		printf("ERROR: DDR freq %dMHz not support!!!\n", ddr->freq);
		ddr->freq = 667;
		printf("Use default value %dMHz!!!\n", ddr->freq);
                ddr_1333Mbps(&ddr->timing);
	}
}
