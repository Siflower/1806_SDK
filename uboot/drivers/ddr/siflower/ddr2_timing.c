#include <common.h>
#include <asm/io.h>
#include "ddr_regs.h"

static void ddr_533Mbps(struct ddr_timing *t)
{
	u32 tck = t->tck = 3750;

	t->cl = 4; //cl = 4
	//t->cwl = t->cl - 1;		//cwl = 3
	t->cwl = 6;
	t->trcd = ns_to_tck(15); //t_rcd >= 15ns
	/* FIXME */
	//t->al = t->trcd - 1;		//al = 3
	t->al = 0;
	t->rl = t->al + t->cl; //rl = 7
	t->wl = t->rl - 1;     //wl = 6

	t->trefi = ns_to_tck(7800);			 //t_refi <= 0x20
	t->tfaw = ns_to_tck(50);			 //t_faw = 14
	t->trc = ns_to_tck(55);				 //t_rc >= 55ns
	t->twr = ns_to_tck(15);				 //t_wr = 4
	t->trfc = ps_to_tck(127500);			 //t_rfc = 0x22
	t->trp = ns_to_tck(15);				 //t_rp >= 15ns
	t->tras_min = ns_to_tck(40);			 //t_ras_min = 40ns
	t->tras_max = ns_to_tck(70000);			 //t_ras_max = 70k ns
	t->tras = t->tras_min;				 //t_ras = 100ns, notice!!!
	t->tmrd = 2;					 //t_mrd >= 4,
	t->tmod = 0xc;					 //t_mod not needed by DDR2, set to umctl2 default value
	t->tccd = 8 / 2;				 //t_ccd = bl/2
	t->tdllk = 512;					 //t_dllk >= 512(not found in DDR2)
	t->txs = max(t->trfc + ns_to_tck(10), (u32)200); //t_xs >= max(5, (t_rfc + 10ns))
	t->txpr = ns_to_tck(400);			 //t_xpr >= max(5, (t_rfc + 10ns))
	t->txsdll = 200;				 //t_xsdll >= t_dllk
	t->txp = max((u32)2, 6 - t->al);		 //t_xp >= max(3, 6ns)
	t->tcksre = 1;					 //t_cksre >= max(5, 10ns)
	t->tcksrx = 1;					 //t_cksrx >= max(5 ,10ns)
	t->trrd = ns_to_tck(10);			 //t_rrd >= max(4, 7.5ns)
	t->tcke = 3;					 //t_cke >= max(3, 5ns)
	t->twtr = ps_to_tck(7500);			 //t_wtr >= max(4, 7.5ns)
	t->trtp = ps_to_tck(7500);			 //t_rtp >= max(4, 7.5ns)
	t->tckesr = t->tcke;				 //t_ckesr >= t_cke + 1
}

static void ddr_667Mbps(struct ddr_timing *t)
{
        u32 tck = t->tck = 3000;

        t->cl = 5; // cl = 4
	// t->cwl = t->cl - 1;		//cwl = 3
	t->cwl = 4;
	t->trcd = ns_to_tck(15); // t_rcd >= 15ns
	/* FIXME */
	// t->al = t->trcd - 1;		//al = 3
	t->al = 0;
	t->rl = t->al + t->cl; // rl = 5
	t->wl = t->rl - 1;     // wl = 4

	t->trefi = ns_to_tck(7800);     // t_refi <= 0x20
	t->tfaw = ns_to_tck(50);	// t_faw = 14
	t->trc = ns_to_tck(55);      // t_rc >= 55ns
	t->twr = ns_to_tck(15);		// t_wr = 4
	t->trfc = ps_to_tck(127500);    // t_rfc = 0x22
	t->trp = ps_to_tck(15000);      // t_rp >= 15ns
	t->tras_min = ns_to_tck(40);    // t_ras_min = 40ns
	t->tras_max = ns_to_tck(70000); // t_ras_max = 70k ns
	t->tras = t->tras_min;		// t_ras = 100ns, notice!!!
	t->tmrd = 2;			// t_mrd >= 4,
	t->tmod = 0xc; // t_mod not needed by DDR2, set to umctl2 default value
	t->tccd = 8 / 2; // t_ccd = bl/2
	t->tdllk = 512;	// t_dllk >= 512(not found in DDR2)
	t->txs = max(t->trfc + ns_to_tck(10),(u32)200);       // t_xs >= max(5, (t_rfc + 10ns))
	t->txpr = ns_to_tck(400);	// not used. t_xpr >= max(5, (t_rfc + 10ns))
	t->txsdll = 200;		 // t_isxr:t_xsdll =t_is
	t->txp = max((u32)2, 7 - t->al); // t_xp >= max(3, 6ns),t_xard
	t->tcksre = 1;			 // always 1
	t->tcksrx = 1;			 // always 1
	t->trrd = ns_to_tck(10);	 // t_rrd >= max(4, 7.5ns)
	t->tcke = 3;			 // all 3
	t->twtr = ps_to_tck(7500);       // t_wtr >= max(4, 7.5ns)
	t->trtp = ps_to_tck(7500);       // t_rtp >= max(4, 7.5ns)
	t->tckesr = t->tcke;		 // t_ckesr >= t_cke
}

static void ddr_800Mbps(struct ddr_timing *t)
{
        u32 tck = t->tck = 2500;

        t->cl = 5; // cl = 4
	// t->cwl = t->cl - 1;		//cwl = 3
	t->cwl = 6;
	t->trcd = ps_to_tck(12500); // t_rcd >= 15ns
	/* FIXME */
	// t->al = t->trcd - 1;		//al = 3
	t->al = 0;
	t->rl = t->al + t->cl; // rl = 7
	t->wl = t->rl - 1;     // wl = 6

	t->trefi = ns_to_tck(7800);     // t_refi <= 0x20
	t->tfaw = ns_to_tck(45);	// t_faw = 14
	t->trc = ps_to_tck(57500);      // t_rc >= 55ns
	t->twr = ns_to_tck(15);		// t_wr = 4
	t->trfc = ps_to_tck(127500);    // t_rfc = 0x22
	t->trp = ps_to_tck(12500);      // t_rp >= 15ns
	t->tras_min = ns_to_tck(40);    // t_ras_min = 40ns
	t->tras_max = ns_to_tck(70000); // t_ras_max = 70k ns
	t->tras = t->tras_min;		// t_ras = 100ns, notice!!!
	t->tmrd = 2;			// t_mrd >= 4,
	t->tmod = 0xc; // t_mod not needed by DDR2, set to umctl2 default value
	t->tccd = 8 / 2; // t_ccd = bl/2
	t->tdllk = 512;	// t_dllk >= 512(not found in DDR2)
	t->txs = max(t->trfc + ns_to_tck(10), (u32)200); // t_xs >= max(5, (t_rfc + 10ns))
	t->txpr = ns_to_tck(400);			 // t_xpr >= max(5, (t_rfc + 10ns))
	t->txsdll = 200;		 // t_xsdll >= t_dllk
	t->txp = max((u32)2, 8 - t->al); // t_xp >= max(3, 6ns),t_xard
	t->tcksre = 1;			 // t_cksre >= max(5, 10ns)
	t->tcksrx = 1;			 // t_cksrx >= max(5 ,10ns)
	t->trrd = ns_to_tck(10);	 // t_rrd >= max(4, 7.5ns)
	t->tcke = 3;			 // t_cke >= max(3, 5ns)
	t->twtr = ps_to_tck(7500);       // t_wtr >= max(4, 7.5ns)
	t->trtp = ps_to_tck(7500);       // t_rtp >= max(4, 7.5ns)
	t->tckesr = t->tcke;		 // t_ckesr >= t_cke + 1
}

void ddr_set_timing(struct ddr_info *ddr)
{
	switch (ddr->freq) {
        case 267:
                ddr_533Mbps(&ddr->timing);
                break;
        case 333:
        	ddr_667Mbps(&ddr->timing);
        	break;
	case 400:
		ddr_800Mbps(&ddr->timing);
		break;
	case 533:
		//ddr_1066Mbps(&ddr->timing);
		//break;
        default:
                printf("ERROR: DDR freq %dMHz not support!!!\n", ddr->freq);
		ddr->freq = 400;
		printf("Use default value %dMHz!!!\n", ddr->freq);
                ddr_800Mbps(&ddr->timing);
	}
}
