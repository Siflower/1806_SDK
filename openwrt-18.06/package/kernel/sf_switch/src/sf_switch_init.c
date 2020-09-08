#include <linux/timer.h>
#include <linux/delay.h>
#include "sf_eth_addr.h"
#include "sf_common.h"
#include "sf_switch_hash_table.h"
#include "sf_mdio.h"
#ifdef CONFIG_SFAX8_SWITCH_VLAN
#include "sf_switch.h"
#endif
#include "sf_switch_init.h"
#include "sf_eth.h"
#include "sf_debug.h"
#include <linux/io.h>
// SF_COMMON_H */300s
#define SF_SWITCH_INTERVAL        (600*HZ)

extern unsigned char start_ageing;
/*******************************************************reg init table**************************************/
static hwinit_data_t sw_hwreg_data[] = {

/*******EMAC configuration*******/
#if (SF_MAC_PORTS_LIST & (1 << 0))
 CONFIG_EMAC_SECTION(0)
#endif
#if (SF_MAC_PORTS_LIST & (1 << 1))
 CONFIG_EMAC_SECTION(1)
#endif
#if (SF_MAC_PORTS_LIST & (1 << 2))
 CONFIG_EMAC_SECTION(2)
#endif
#if (SF_MAC_PORTS_LIST & (1 << 3))
 CONFIG_EMAC_SECTION(3)
#endif
#if (SF_MAC_PORTS_LIST & (1 << 4))
 CONFIG_EMAC_SECTION(4)
#endif

/*******BMU Configuration*******/
{ REG_BMU_CTRL,									0x00000002},            //Soft reset the BMU block
{ REG_BMU_UCAST_CONFIG,							0x00000400},            //Program for Maximum buffer count as 1024
{ REG_BMU_UCAST_BASEADDR,						0x00100000},            //Program LMEM block base address
{ REG_BMU_BUF_SIZE,								0x00000007},            //Buffer size bits of LMEM buffer
{ REG_BMU_THRES,								0x02000200},            //Threshold number of LMEM buffers occupied to generate interrupt
{ REG_BMU_INT_SRC,								0x000001ff},            //Clearing the interrupts
{ REG_BMU_INT_ENABLE,							0x00000000},            //Disabling the interrupts

/*******Class HW Block Configuration*******/
{ REG_CLASS_HDR_SIZE,							0x00000030},                   //LMEM first buffer header size value
{ REG_CLASS_TM_INQ_ADDR,						0x00310008},  //Address of the Traffic Manager's input Queue
{ REG_CLASS_BMU_BUF_FREE,						0x00240034},  //Address of BMU,where buffer should be freed
{ REG_CLASS_SNOOP_SPL_MCAST_MASK1_LSB,			0xffffffff}, //Snoop MCAST address lower 32bit mask programming
{ REG_CLASS_SNOOP_SPL_MCAST_MASK1_MSB,			0x0000ffff}, //Snoop MCAST address upper 16bit mask programming
{ REG_CLASS_SNOOP_SPL_MCAST_ADDR1_LSB,			0xc200000e}, //SPL multicast address1 lsb 32 bits, program DA lower 32bit address of address packets to punt to host control channel
{ REG_CLASS_SNOOP_SPL_MCAST_ADDR1_MSB,			0x00000180}, //SPL multicast address1 msb remaining 16 bits, program DA upper 16bits address of PTP packets to punt to host control channel
{ REG_CLASS_USE_TMU_INQ,						0x00000001}, // add this to use tmu inq full signal to avoid fifo full
{ REG_CLASS_PPPOE_CFG,							0x00000001 }, // for rm 1100
{ REG_CLASS_TCP_OFFLOAD_PKTLEN_THRESH,			0x00000640 }, // for checksumoffload
{ REG_CLASS_PORT_TCP_CHKSUM_OFFLOAD,			0x0000ffff }, // for checksum offload

/*******EGPI 0/1 / 2 / 3 / 4 Blocks Configuration*******/
#if (SF_MAC_PORTS_LIST & (1 << 0))
CONFIG_EGPI_SECTION(0)
#endif
#if (SF_MAC_PORTS_LIST & (1 << 1))
CONFIG_EGPI_SECTION(1)
#endif
#if (SF_MAC_PORTS_LIST & (1 << 2))
CONFIG_EGPI_SECTION(2)
#endif
#if (SF_MAC_PORTS_LIST & (1 << 3))
CONFIG_EGPI_SECTION(3)
#endif
#if (SF_MAC_PORTS_LIST & (1 << 4))
CONFIG_EGPI_SECTION(4)
#endif

#ifdef CONFIG_NET_SFAX8_PTP
// add for etgpi config
#if (SF_MAC_PORTS_LIST & (1 << 0))
CONFIG_ETGPI_SECTION(0)
#endif
#if (SF_MAC_PORTS_LIST & (1 << 1))
CONFIG_ETGPI_SECTION(1)
#endif
#if (SF_MAC_PORTS_LIST & (1 << 2))
CONFIG_ETGPI_SECTION(2)
#endif
#if (SF_MAC_PORTS_LIST & (1 << 3))
CONFIG_ETGPI_SECTION(3)
#endif
#if (SF_MAC_PORTS_LIST & (1 << 4))
CONFIG_ETGPI_SECTION(4)
#endif
#endif
/*******HGPI Block Configuration*******/
CONFIG_HGPI_SECTION()
/*******TMU Block Configuration*******/
#if (SF_MAC_PORTS_LIST & (1 << 0))
{ REG_TMU_PHY0_INQ_ADDR,			REG_GPI_INQ_PKTPTR0},      //INQ address of PORTX is EGPIX
#endif
#if (SF_MAC_PORTS_LIST & (1 << 1))
{ REG_TMU_PHY1_INQ_ADDR,   			REG_GPI_INQ_PKTPTR1},
#endif
#if (SF_MAC_PORTS_LIST & (1 << 2))
{ REG_TMU_PHY2_INQ_ADDR,   			REG_GPI_INQ_PKTPTR2},
#endif
#if (SF_MAC_PORTS_LIST & (1 << 3))
{ REG_TMU_PHY3_INQ_ADDR,   			REG_GPI_INQ_PKTPTR3},
#endif
#if (SF_MAC_PORTS_LIST & (1 << 4))
{ REG_TMU_PHY4_INQ_ADDR,   			REG_GPI_INQ_PKTPTR4},
#endif
{ REG_TMU_PHY5_INQ_ADDR,   			REG_GPI_INQ_PKTPTR5},
{ REG_TMU_CNTX_ACCESS_CTRL,			0x00000001},             //controls the direct/indirect access to context memory. 0 - indirect, 1 - direct
#if (SF_MAC_PORTS_LIST & (1 << 0))
{ REG_TMU_PHY0_TDQ_CTRL,   			0x0000000f},             //bit 0 - shp_clk_cntrl_en;bit 1 - hw_en for tdq_sch_shaper;bit 3:2 - for enabling the schedulers;bit 4 - allw_tdq_prog
#endif
#if (SF_MAC_PORTS_LIST & (1 << 1))
{ REG_TMU_PHY1_TDQ_CTRL,   			0x0000000f},
#endif
#if (SF_MAC_PORTS_LIST & (1 << 2))
{ REG_TMU_PHY2_TDQ_CTRL,   			0x0000000f},
#endif
#if (SF_MAC_PORTS_LIST & (1 << 3))
{ REG_TMU_PHY3_TDQ_CTRL,   			0x0000000f},
#endif
#if (SF_MAC_PORTS_LIST & (1 << 4))
{ REG_TMU_PHY4_TDQ_CTRL,   			0x0000000f},
#endif
{ REG_TMU_PHY5_TDQ_CTRL,   			0x0000000f},
{ REG_TMU_BMU_INQ_ADDR,    			0x00240034},//Address of BMU, where buffer should be freed
// tmu overflow and hif is block, if set this ,tmu will drop packet
// { REG_TMU_TEQ_CTRL,        			0x03},
/*******HIF Block Configura			tion*******/
{ REG_HIF_POLL_CTRL,       			0x00080008},             //Bit [15:0] - TX BD poll counter value ;Bit [31:16] - RX BD poll counter value,which enable TX/RX BD fetch after every poll counter expiry
{ REG_HIF_TX_ALLOC,        			0x00000008},             //Number of cycles that HIF TX block would wait before retrying for checking for destination buffer allocation
{ REG_HIF_RX_ALLOC,        			0x00000008},             //Number of cycles that HIF RX block would wait before retrying for checking for destination buffer allocation

/*******Enable of NPU IP Bl			ocks*******/
{ REG_BMU_CTRL,            			0x00000001},             //BMU core enable
#if (SF_MAC_PORTS_LIST & (1 << 0))
{ REG_GPI_CTRL(0),         			0x00000001},             //EGPI core enable
#endif
#if (SF_MAC_PORTS_LIST & (1 << 1))
{ REG_GPI_CTRL(1),         			0x00000001},             //EGPI core enable
#endif
#if (SF_MAC_PORTS_LIST & (1 << 2))
{ REG_GPI_CTRL(2),         			0x00000001},             //EGPI core enable
#endif
#if (SF_MAC_PORTS_LIST & (1 << 3))
{ REG_GPI_CTRL(3),         			0x00000001},             //EGPI core enable
#endif
#if (SF_MAC_PORTS_LIST & (1 << 4))
{ REG_GPI_CTRL(4),         			0x00000001},             //EGPI core enable
#endif
{ REG_HGPI_CTRL,           			0x00000001},             //HGPI core enable
{ REG_CLASS_TX_CTRL,       			0x00000001},              //Class HW core enable
{ 0,								0}              //Class HW core enable
};
/* The timer structure used to refresh the mac table entries */
static struct timer_list sf_switch_age_timer;

void sys_npu_reset(void)
{

	//reset npu system manager module
	writew(0x0, (void *)NPU_SYSM_RESET);
	writew(0x1, (void *)NPU_SYSM_RESET);

	//enable npu clk
	writew(0xFF, (void *)NPU_SOFT_CLK_EN);

	//hardward reset npu
	writew(0xFF, (void *)NPU_SOFT_RESET);
#ifndef CONFIG_SFAX8_SWITCH_FPGA
	//emac[5-1]_phy_enable[4:0]
	writeb(0xFF, (void *)OMINI_PHY_ENABLE);
	sys_omini_phy_reset();
#endif
	writew(0x00, (void *)NPU_SOFT_RESET);

	//set npu boe
	writew(0x0F, (void *)NPU_SOFT_BOE);

}

void sf_local_reset(void)
{
	int value = 0;
	//already config in uboot
	// sys_npu_reset(void);
	sf_writel(0x40000000,REG_WSP_SYS_GENERIC_CONTROL);
	udelay(10000);
	value = sf_readl(REG_WSP_SYS_GENERIC_CONTROL);
	sf_writel(value & 0xBFFFFFFF,REG_WSP_SYS_GENERIC_CONTROL);
}

void load_hwreg_table(hwinit_data_t *initdata)
{
	hwinit_data_t* hw_data = initdata;
	while (hw_data->offset != 0) {
		sf_writel(hw_data->data, hw_data->offset);
		hw_data++;
	}
	return;
}
int get_port_address(int port_num, int index) {
	int addr1 = 0,addr2 = 0;
	switch (port_num) {
		case 0:
			addr1 = REG_CLASS_HW_PORT0_STRUC1;
			addr2 = REG_CLASS_HW_PORT0_STRUC2;
			break;
		case 1:
			addr1 = REG_CLASS_HW_PORT1_STRUC1;
			addr2 = REG_CLASS_HW_PORT1_STRUC2;
			break;
		case 2:
			addr1 = REG_CLASS_HW_PORT2_STRUC1;
			addr2 = REG_CLASS_HW_PORT2_STRUC2;
			break;
		case 3:
			addr1 = REG_CLASS_HW_PORT3_STRUC1;
			addr2 = REG_CLASS_HW_PORT3_STRUC2;
			break;
		case 4:
			addr1 = REG_CLASS_HW_PORT4_STRUC1;
			addr2 = REG_CLASS_HW_PORT4_STRUC2;
			break;
		case 5:
			addr1 = REG_CLASS_HW_PORT5_STRUC1;
			addr2 = REG_CLASS_HW_PORT5_STRUC2;
			break;
		default:
			return -1;
	}
	if(index == 1)
	  return addr1;
	else
	  return addr2;
}

int sf_port_init(void * pswitch) {
	uint i, addr1 = 0, addr2 = 0,max_pnum = 0;
	struct port_s *psfax8_port = NULL;
	/* init port data structure */
#ifdef CONFIG_SFAX8_SWITCH_VLAN
	struct sfax8_switch_t *psfax8_switch = pswitch;
	psfax8_port = psfax8_switch->port;
	max_pnum = psfax8_switch->max_port_num;
#else
	psfax8_port = pswitch;
	max_pnum = SF_MAX_PORTS;
#endif
	for (i = 0; i < max_pnum; i++) {
		if (!(SF_MAC_PORTS_LIST & (1 << i)) && (i != SF_HOST_PORT))
			continue;
		addr1 = addr2 = 0;
		psfax8_port[i].shutdown = 0;
		psfax8_port[i].tpid     = VLAN_TAGGED_FRAME;

		psfax8_port[i].aft      = ANY_TAGGING;

#ifdef CONFIG_SFAX8_SWITCH_VLAN
		if(i == SF_HOST_PORT)
		   psfax8_port[i].aft    = TAGGED_ONLY;
#endif
		// do not change the value for keeping vlan cfg when reset
		// psfax8_port[i].fallback_bd_id = DEF_PORT_FALLBACK_BD_ID;

		psfax8_port[i].block_state = FORWARDING;

		psfax8_port[i].csr_untag_from_btable = 0;

#ifdef CONFIG_SFAX8_SWITCH_VLAN
		// if(i != SF_HOST_PORT)
		//   psfax8_port[i].csr_untag_from_btable = 1;
#endif

		addr1 = get_port_address(i,1);
		addr2 = get_port_address(i,2);
		/* class hw port csr reg:
		 *======================================================*
		 *res |block_state |aft |shutdown |fallback_bd_id |tpid *
		 *======================================================*
		 *20-bit| 4-bit   |4-bit| 1-bit   | 16-bit      |16-bit *
		 *=====================================================*/
		if (addr1 && addr2) {
			sf_writel( ((DEF_PORT_FALLBACK_BD_ID << PORT_FALLBACK_BDID_START_POS) & PORT_FALLBACK_BDID_MASK)
						| ((psfax8_port[i].tpid << PORT_TPID_START_POS) & PORT_TPID_MASK)
						,addr1);

			sf_writel( ((psfax8_port[i].csr_untag_from_btable << PORT_UNTAG_FROM_BTABLE_START_POS) & PORT_UNTAG_FROM_BTABLE_MASK)
						| ((psfax8_port[i].block_state << PORT_BLOCKSTATE_START_POS) & PORT_BLOCKSTATE_MASK)
						| ((psfax8_port[i].aft << PORT_AFT_START_POS) & PORT_AFT_MASK)
						| ((psfax8_port[i].vid_prefix << PORT_VID_PREFIX_START_POS) & PORT_VID_PREFIX_MASK)
						| ((psfax8_port[i].shutdown << PORT_SHUTDOWN_START_POS) & PORT_SHUTDOWN_MASK)
						, addr2);
		}
	}

	return 0;
}

#if defined(SF_SPEED_BANLANCE) || defined(CONFIG_SFAX8_SWITCH_API)
int get_gpi_addr_base( int dev_no)
{
	int addr = 0;
	switch (dev_no)
	{
		case 0 :
			addr = EGPI0_BASE_OFFSET;
			break;
		case 1 :
			addr = EGPI1_BASE_OFFSET;
			break;
		case 2 :
			addr = EGPI2_BASE_OFFSET;
			break;
		case 3 :
			addr = EGPI3_BASE_OFFSET;
			break;
		case 4 :
			addr = EGPI4_BASE_OFFSET;
			break;
		case 5 :
			addr = HGPI_BASE_OFFSET;
			break;
		default:
			break;
	}
	return addr;
}
#endif

#ifdef SF_SPEED_BANLANCE
void gpi_ing_shaper_init(int limit)
{
	int phy_no = 0, write_val = 0;
	int clkdiv = 0, trydiv = 0, div = 0, wt = 0;

	unsigned int max_credit = 0x3ff00;
	unsigned int min_credit = 0x3ff00;
	int sys_clk = 250; //Mhz
	int rate = limit; //Mbps
	int mode = 0; /* 0 for bbs, 1 for pps */
	int type = 0; /* value 00 Shaper is for port level data rate, value 01 Shaper is for BCAST Packets, value 10 Shaper is for MCAST Packets. */

	/*Clock division intial value*/
	div = 2;

	/* find the clkdiv value that gives us the largest valid wt value, clk_div has 7 bits, weight has 11 bits */
	while (div <= 128) {
		trydiv = div << 1;
		/* calculate will not over int */
		wt = ((rate * trydiv * 256)/(sys_clk * 8));
		if (wt > 0x7FF)
			break;
		div = trydiv;
		clkdiv++;
	}

	wt = ((rate * div * 256)/(sys_clk * 8));

	for (phy_no = 0; phy_no < SF_MAC_PORTS; phy_no++) {
		if (!(SF_MAC_PORTS_LIST & (1 << phy_no)))
			continue;
		/* writing clock division and max_credit value in GPI_SHP0_CTRL register */
		write_val = SHPR_EN_BIT + (clkdiv << 1) + (max_credit << 8);
		sf_writel(write_val, (get_gpi_addr_base(phy_no) + REG_EGPI_PORT_SHP0_CTRL));

		/* writing weight value in GPI_PORT_SHP0_WEIGHT register */
		write_val = wt;
		sf_writel(write_val, (get_gpi_addr_base(phy_no) + REG_EGPI_PORT_SHP0_WGHT));

		/* writing min_credit value in GPI_PORT_SHP0_MIN_CREDOT register */
		write_val = min_credit;
		sf_writel(write_val, (get_gpi_addr_base(phy_no) + REG_EGPI_PORT_SHP0_MIN_CREDOT));

		/* writing shaper mode and type in GPI_PORT_SHP0_MIN_CREDOT register */
		write_val = mode | (type << 2);
		sf_writel(write_val, (get_gpi_addr_base(phy_no) + REG_EGPI_PORT_SHP_CONFIG));
	}
}
#endif

void tmu_queue_init(void)
{
    uint phy_no = 0;
    uint queue_no = 0;
    for (phy_no = 0; phy_no < SF_MAX_PORTS; phy_no++) {
		if (!(SF_MAC_PORTS_LIST & (1 << phy_no)))
			continue;

        for (queue_no = 0; queue_no < 8; queue_no++) {

            sf_writel((queue_no + ((phy_no << 8) & 0xf00)),REG_TMU_PHY_QUEUE_SEL_ADDR);

            sf_writel(0,REG_TMU_CURQ_PTR_ADDR);
            sf_writel(0,REG_TMU_CURQ_PKT_CNT_ADDR);
            sf_writel(0,REG_TMU_CURQ_DROP_CNT_ADDR);
            sf_writel(0,REG_TMU_CURQ_TRANS_CNT_ADDR);
            sf_writel(0,REG_TMU_CURQ_QSTAT_ADDR);
            sf_writel(0,REG_TMU_HW_PROB_CFG_TBL0_ADDR);
            sf_writel(0,REG_TMU_HW_PROB_CFG_TBL1_ADDR);
            sf_writel(0,REG_TMU_CURQ_DEBUG_ADDR);
        }
    }
    sf_writel(0,REG_TMU_CNTX_ACCESS_CTRL);
}

int sf_init_switch_hw(void) {
	load_hwreg_table(&sw_hwreg_data[0]);
#ifdef SF_SPEED_BANLANCE
//	gpi_ing_shaper_init(100);
#endif
	mac_hash_table_init();
	vlan_hash_table_init();
	tmu_queue_init();

	return 0;
}


void sf_switch_age_timer_handler(unsigned long data) {
	if(!start_ageing){
		start_ageing = 1;
		host_ageing();
		start_ageing = 0;
	}
	/* Update the refresh timer to call this fucntion again */
	sf_switch_age_timer.expires = jiffies + SF_SWITCH_INTERVAL;
	add_timer(&sf_switch_age_timer);
}


int sf_switch_init_timer(int init)
{

	int ret = 0;
	if (!init)
	  goto delbr_timer;

	/*Start FP table refresh timer for Bridge*/
	init_timer(&sf_switch_age_timer);
	sf_switch_age_timer.expires  = jiffies + SF_SWITCH_INTERVAL;
	sf_switch_age_timer.data     = (unsigned long) &sf_switch_age_timer;
	sf_switch_age_timer.function = sf_switch_age_timer_handler;
	add_timer((struct timer_list *)&sf_switch_age_timer);
	return 0;

delbr_timer:
	del_timer((struct timer_list *)&sf_switch_age_timer);
	return ret;
}
