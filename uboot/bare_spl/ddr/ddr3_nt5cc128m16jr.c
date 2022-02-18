#include <common.h>
#include <io.h>
#include "ddr_regs.h"

#define SET_COMMON_PARA() .trefi = ns_to_tck(7800), \
                        .trfc = ns_to_tck(160), \
                        .tmod = MAX((u32)12, ns_to_tck(15)), \
                        .txsdll = 512, \
                        .tzqinit = MAX((u32)512, ns_to_tck(640)), \
                        .tzqoper = MAX((u32)256, ns_to_tck(320)), \
                        .tzqcs = MAX((u32)64, ns_to_tck(80)), \
                        .txpdll = MAX((u32)10, ns_to_tck(24)), \
                        .tcksre = MAX((u32)5, ns_to_tck(10)), \
                        .tcksrx = MAX((u32)5, ns_to_tck(10)), \
                        .twtr = MAX((u32)4, ps_to_tck(7500)), \
                        .trtp = MAX((u32)4, ps_to_tck(7500)), \

static struct ddr_timing ddr_800Mbps = {
#undef Tck
#define Tck 2500
        .tck = Tck,

	.cl = 6,
	.al = 0,
	.cwl = 5,

	.trefi = ns_to_tck(7800),
	.tfaw = ns_to_tck(35), //EM6GD16EWKG only need 40ns
	.trc = ps_to_tck(47910),
	.twr = ns_to_tck(15),
	.trfc = ns_to_tck(160),
	.trcd = ps_to_tck(13910),
	.trp = ps_to_tck(13910),
	.tras_min = ns_to_tck(34),
	.twlo = ps_to_tck(7500), //EM6GD16EWKG need 7.5ns
	.twlmrd = 40,
	.tmrd = 4,
	.tmod = MAX((u32)12, ns_to_tck(15)),
	.tccd = 4,
	.tdllk = 512,
	.txsdll = 512,
	.tzqinit = MAX((u32)512, ns_to_tck(640)),
	.tzqoper = MAX((u32)256, ns_to_tck(320)),
	.tzqcs = MAX((u32)64, ns_to_tck(80)),
	.txpdll = MAX((u32)10, ns_to_tck(24)),
	.txp = MAX((u32)3, ps_to_tck(6000)), //EM6GD16EWKG only need 3CK or 6ns
	.tcksre = MAX((u32)5, ns_to_tck(10)),
	.tcksrx = MAX((u32)5, ns_to_tck(10)),
	.trrd = MAX((u32)4, ps_to_tck(7500)), //EM6GD16EWKG only need 7.5ns
	.tcke = MAX((u32)3, ps_to_tck(5000)),//EM6GD16EWKG only need 3CK or 5ns
	.twtr = MAX((u32)4, ps_to_tck(7500)),
	.trtp = MAX((u32)4, ps_to_tck(7500)),
};

static struct ddr_timing ddr_1066Mbps = {
#undef Tck
#define Tck 1875
        .tck = Tck,

	.cl = 7,
	.al = 0,
	.cwl = 6,

	.trefi = ns_to_tck(7800),
	.tfaw = ns_to_tck(35), //EM6GD16EWKG only need 40ns
	.trc = ps_to_tck(47910),
	.twr = ns_to_tck(15),
	.trfc = ns_to_tck(160),
	.trcd = ps_to_tck(13910),
	.trp = ps_to_tck(13910),
	.tras_min = ns_to_tck(34),
	.twlo = ps_to_tck(7500), //EM6GD16EWKG need 7.5ns
	.twlmrd = 40,
	.tmrd = 4,
	.tmod = MAX((u32)12, ns_to_tck(15)),
	.tccd = 4,
	.tdllk = 512,
	.txsdll = 512,
	.tzqinit = MAX((u32)512, ns_to_tck(640)),
	.tzqoper = MAX((u32)256, ns_to_tck(320)),
	.tzqcs = MAX((u32)64, ns_to_tck(80)),
	.txpdll = MAX((u32)10, ns_to_tck(24)),
	.txp = MAX((u32)3, ps_to_tck(6000)), //EM6GD16EWKG only need 3CK or 6ns
	.tcksre = MAX((u32)5, ns_to_tck(10)),
	.tcksrx = MAX((u32)5, ns_to_tck(10)),
	.trrd = MAX((u32)4, ps_to_tck(5000)), //EM6GD16EWKG only need 7.5ns
	.tcke = MAX((u32)3, ps_to_tck(5000)),//EM6GD16EWKG only need 3CK or 5ns
	.twtr = MAX((u32)4, ps_to_tck(7500)),
	.trtp = MAX((u32)4, ps_to_tck(7500)),
};



static struct ddr_timing ddr_1333Mbps = {
#undef Tck
#define Tck 1500
        .tck = Tck,

	.cl = 9,
	.al = 0,
	.cwl = 7,

	.trefi = ns_to_tck(7800),
	.tfaw = ns_to_tck(35), //EM6GD16EWKG only need 40ns
	.trc = ps_to_tck(49125),
	.twr = ns_to_tck(15),
	.trfc = ns_to_tck(160),
	.trcd = ps_to_tck(13125),
	.trp = ps_to_tck(13125),
	.tras_min = ns_to_tck(36),
	.twlo = ps_to_tck(7500), //EM6GD16EWKG need 7.5ns
	.twlmrd = 40,
	.tmrd = 4,
	.tmod = MAX((u32)12, ns_to_tck(15)),
	.tccd = 4,
	.tdllk = 512,
	.txsdll = 512,
	.tzqinit = MAX((u32)512, ns_to_tck(640)),
	.tzqoper = MAX((u32)256, ns_to_tck(320)),
	.tzqcs = MAX((u32)64, ns_to_tck(80)),
	.txpdll = MAX((u32)10, ns_to_tck(24)),
	.txp = MAX((u32)3, ps_to_tck(6000)), //EM6GD16EWKG only need 3CK or 6ns
	.tcksre = MAX((u32)5, ns_to_tck(10)),
	.tcksrx = MAX((u32)5, ns_to_tck(10)),
	.trrd = MAX((u32)4, ps_to_tck(5000)), //EM6GD16EWKG only need 7.5ns
	.tcke = MAX((u32)3, ps_to_tck(5000)),//EM6GD16EWKG only need 3CK or 5ns
	.twtr = MAX((u32)4, ps_to_tck(7500)),
	.trtp = MAX((u32)4, ps_to_tck(7500)),
};



static struct ddr_timing ddr_1600Mbps = {
#undef Tck
#define Tck 1250
        .tck = Tck,

	.cl = 11,
	.al = 0,
	.cwl = 8,

	.trefi = ns_to_tck(7800),
	.tfaw = ns_to_tck(35), //EM6GD16EWKG only need 40ns
	.trc = ps_to_tck(47910),
	.twr = ns_to_tck(15),
	.trfc = ns_to_tck(160),
	.trcd = ps_to_tck(13910),
	.trp = ps_to_tck(13910),
	.tras_min = ns_to_tck(34),
	.twlo = ps_to_tck(7500), //EM6GD16EWKG need 7.5ns
	.twlmrd = 40,
	.tmrd = 4,
	.tmod = MAX((u32)12, ns_to_tck(15)),
	.tccd = 4,
	.tdllk = 512,
	.txsdll = 512,
	.tzqinit = MAX((u32)512, ns_to_tck(640)),
	.tzqoper = MAX((u32)256, ns_to_tck(320)),
	.tzqcs = MAX((u32)64, ns_to_tck(80)),
	.txpdll = MAX((u32)10, ns_to_tck(24)),
	.txp = MAX((u32)3, ps_to_tck(6000)), //EM6GD16EWKG only need 3CK or 6ns
	.tcksre = MAX((u32)5, ns_to_tck(10)),
	.tcksrx = MAX((u32)5, ns_to_tck(10)),
	.trrd = MAX((u32)4, ps_to_tck(5000)), //EM6GD16EWKG only need 7.5ns
	.tcke = MAX((u32)3, ps_to_tck(5000)),//EM6GD16EWKG only need 3CK or 5ns
	.twtr = MAX((u32)4, ps_to_tck(7500)),
	.trtp = MAX((u32)4, ps_to_tck(7500)),
};


static struct ddr_timing ddr_1866Mbps = {
#undef Tck
#define Tck 1070
        .tck = Tck,

	.cl = 13,
	.al = 0,
	.cwl = 9,

	.trefi = ns_to_tck(7800),
	.tfaw = ns_to_tck(35), //EM6GD16EWKG only need 40ns
	.trc = ps_to_tck(47910),
	.twr = ns_to_tck(15),
	.trfc = ns_to_tck(160),
	.trcd = ps_to_tck(13910),
	.trp = ps_to_tck(13910),
	.tras_min = ns_to_tck(34),
	.twlo = ps_to_tck(7500), //EM6GD16EWKG need 7.5ns
	.twlmrd = 40,
	.tmrd = 4,
	.tmod = MAX((u32)12, ns_to_tck(15)),
	.tccd = 4,
	.tdllk = 512,
	.txsdll = 512,
	.tzqinit = MAX((u32)512, ns_to_tck(640)),
	.tzqoper = MAX((u32)256, ns_to_tck(320)),
	.tzqcs = MAX((u32)64, ns_to_tck(80)),
	.txpdll = MAX((u32)10, ns_to_tck(24)),
	.txp = MAX((u32)3, ps_to_tck(6000)), //EM6GD16EWKG only need 3CK or 6ns
	.tcksre = MAX((u32)5, ns_to_tck(10)),
	.tcksrx = MAX((u32)5, ns_to_tck(10)),
	.trrd = MAX((u32)4, ps_to_tck(5000)), //EM6GD16EWKG only need 7.5ns
	.tcke = MAX((u32)3, ps_to_tck(5000)),//EM6GD16EWKG only need 3CK or 5ns
	.twtr = MAX((u32)4, ps_to_tck(7500)),
	.trtp = MAX((u32)4, ps_to_tck(7500)),
};



void ddr_set_timing(struct ddr_info *ddr)
{
	struct ddr_timing *t;
	switch (ddr->freq) {
	case 400:
		ddr->timing = &ddr_800Mbps;
		break;
        case 533:
		ddr->timing = &ddr_1066Mbps;
		break;
	case 667:
		ddr->timing = &ddr_1333Mbps;
		break;
	case 800:
		ddr->timing = &ddr_1600Mbps;
		break;
	case 933:
		ddr->timing = &ddr_1866Mbps;
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
	t->wl = t->al + t->cwl;
	t->rl = t->al + t->cl;
	t->tckesr = t->tcke + 1;
	t->tras = t->tras_min;
	t->tras_max = 9 * t->trefi;
	t->txs = MAX((u32)5, ns_to_tck(10) + t->trfc);
        t->txpr = MAX((u32)5, ns_to_tck(10) + t->trfc);
}
