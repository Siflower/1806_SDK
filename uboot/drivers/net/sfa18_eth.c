#include <common.h>
#include <malloc.h>
#include <net.h>
#include <config.h>

#include <phy.h>
#include <miiphy.h>

#include <asm/io.h>
#include <netdev.h>
#include "sfa18_mac.h"

static hwinit_data_t sw_hwreg_data[] = {
/*******BMU Configuration*******/
{ REG_BMU_CTRL,									0x00000002},            //Soft reset the BMU block
{ REG_BMU_UCAST_CONFIG,							0x00000400},            //Program for Maximum buffer count as 1024
{ REG_BMU_UCAST_BASEADDR,						0x00100000},            //Program LMEM block base address
{ REG_BMU_BUF_SIZE,								0x00000007},            //Buffer size bits of LMEM buffer
{ REG_BMU_THRES,								0x02000200},            //Threshold number of LMEM buffers occupied to generate interrupt
{ REG_BMU_INT_SRC,								0x000001ff},            //Clearing the interrupts
{ REG_BMU_INT_ENABLE,							0x00000000},            //Disabling the interrupts
/*******EMAC configuration*******/
 CONFIG_EMAC_SECTION(0)
 CONFIG_EMAC_SECTION(1)
 CONFIG_EMAC_SECTION(2)
 CONFIG_EMAC_SECTION(3)
 CONFIG_EMAC_SECTION(4)

/*******Class HW Block Configuration*******/
{ REG_CLASS_HDR_SIZE,							0x00000030},                   //LMEM first buffer header size value
{ REG_CLASS_TM_INQ_ADDR,						0x00310008},  //Address of the Traffic Manager's input Queue
{ REG_CLASS_BMU_BUF_FREE,						0x00240034},  //Address of BMU,where buffer should be freed
{ REG_CLASS_GLOBAL_CFG,							0x0480003f},            //bit 0 --l2 special punt enable for all ports bit 31:1 - global fall back bd entry, 16 bit default forward list for bd entry,punt to all ports if miss for unicast/multicast
{ REG_CLASS_SNOOP_SPL_MCAST_MASK1_LSB,			0xffffffff}, //Snoop MCAST address lower 32bit mask programming
{ REG_CLASS_SNOOP_SPL_MCAST_MASK1_MSB,			0x0000ffff}, //Snoop MCAST address upper 16bit mask programming
{ REG_CLASS_SNOOP_SPL_MCAST_ADDR1_LSB,			0xc200000e}, //SPL multicast address1 lsb 32 bits, program DA lower 32bit address of address packets to punt to host control channel
{ REG_CLASS_SNOOP_SPL_MCAST_ADDR1_MSB,			0x00000180}, //SPL multicast address1 msb remaining 16 bits, program DA upper 16bits address of PTP packets to punt to host control channel
{ REG_CLASS_USE_TMU_INQ,						0x00000001}, // add this to use tmu inq full signal to avoid fifo full

/*******EGPI 0/1 / 2 / 3 / 4 Blocks Configuration*******/
CONFIG_EGPI_SECTION(0)
CONFIG_EGPI_SECTION(1)
CONFIG_EGPI_SECTION(2)
CONFIG_EGPI_SECTION(3)
CONFIG_EGPI_SECTION(4)

#ifdef CONFIG_NET_SFA18_PTP
// add for etgpi config
CONFIG_ETGPI_SECTION(0)
CONFIG_ETGPI_SECTION(1)
CONFIG_ETGPI_SECTION(2)
CONFIG_ETGPI_SECTION(3)
CONFIG_ETGPI_SECTION(4)
#endif
/*******HGPI Block Configuration*******/
CONFIG_HGPI_SECTION()
/*******TMU Block Configuration*******/
{ REG_TMU_PHY0_INQ_ADDR,			REG_GPI_INQ_PKTPTR0},      //INQ address of PORTX is EGPIX
{ REG_TMU_PHY1_INQ_ADDR,   			REG_GPI_INQ_PKTPTR1},
{ REG_TMU_PHY2_INQ_ADDR,   			REG_GPI_INQ_PKTPTR2},
{ REG_TMU_PHY3_INQ_ADDR,   			REG_GPI_INQ_PKTPTR3},
{ REG_TMU_PHY4_INQ_ADDR,   			REG_GPI_INQ_PKTPTR4},
{ REG_TMU_PHY5_INQ_ADDR,   			REG_GPI_INQ_PKTPTR5},
{ REG_TMU_CNTX_ACCESS_CTRL,			0x00000001},             //controls the direct/indirect access to context memory. 0 - indirect, 1 - direct
{ REG_TMU_PHY0_TDQ_CTRL,   			0x0000000f},             //bit 0 - shp_clk_cntrl_en;bit 1 - hw_en for tdq_sch_shaper;bit 3:2 - for enabling the schedulers;bit 4 - allw_tdq_prog
{ REG_TMU_PHY1_TDQ_CTRL,   			0x0000000f},
{ REG_TMU_PHY2_TDQ_CTRL,   			0x0000000f},
{ REG_TMU_PHY3_TDQ_CTRL,   			0x0000000f},
{ REG_TMU_PHY4_TDQ_CTRL,   			0x0000000f},
{ REG_TMU_PHY5_TDQ_CTRL,   			0x0000000f},
{ REG_TMU_BMU_INQ_ADDR,    			0x00240034},//Address of BMU, where buffer should be freed
// tmu overflow and hif is block, if set this ,tmu will drop packet
// { REG_TMU_TEQ_CTRL,        			0x03},
/*******HIF Block Configura			tion*******/
{ REG_HIF_POLL_CTRL,       			0x00640064},             //Bit [15:0] - TX BD poll counter value ;Bit [31:16] - RX BD poll counter value,which enable TX/RX BD fetch after every poll counter expiry
{ REG_HIF_TX_ALLOC,        			0x00000064},             //Number of cycles that HIF TX block would wait before retrying for checking for destination buffer allocation
{ REG_HIF_RX_ALLOC,        			0x00000064},             //Number of cycles that HIF RX block would wait before retrying for checking for destination buffer allocation

/*******Enable of NPU IP Bl			ocks*******/
{ REG_BMU_CTRL,            			0x00000001},             //BMU core enable
{ REG_GPI_CTRL(0),         			0x00000001},             //EGPI core enable
{ REG_GPI_CTRL(1),         			0x00000001},             //EGPI core enable
{ REG_GPI_CTRL(2),         			0x00000001},             //EGPI core enable
{ REG_GPI_CTRL(3),         			0x00000001},             //EGPI core enable
{ REG_GPI_CTRL(4),         			0x00000001},             //EGPI core enable
{ REG_HGPI_CTRL,           			0x00000001},             //HGPI core enable
{ REG_CLASS_TX_CTRL,       			0x00000001},              //Class HW core enable
{ 0,								0}              //Class HW core enable
};
char macaddr[6]={0x00,0x11,0x22,0x33,0x44,0x55};
int  sf_base = NPU_ADDR_BASE;

#ifdef CONFIG_SFA18_PCBA_TEST
struct sf_priv *g_priv = NULL;
extern unsigned char eth_test_pkt[PCBA_PKT_SIZE];
#endif

void sf_writel(uint value,uint offset){
	return writel(value, (void *)(sf_base + offset));
}

int sf_readl(uint offset){
	return readl((void *)(sf_base + offset));
}


#define MAP_ADDR_TO_UNCACHE(addr) (((addr) & 0x0fffffff) | 0xA0000000)

#define MAP_ADDR_TO_CACHED(addr) (((addr) & 0x0fffffff) | 0x80000000)

extern void sys_npu_reset(void);

void sf_local_reset(void)
{
	int value = 0;
	//already config in uboot
	// sys_npu_reset(void);
	sf_writel(0x40000000,REG_WSP_SYS_GENERIC_CONTROL);
	udelay(10000);
	value = sf_readl(REG_WSP_SYS_GENERIC_CONTROL);
	sf_writel(value & 0xBFFFFFFF,REG_WSP_SYS_GENERIC_CONTROL);
	//NPU_hw_rst_prgrm_en this is set for npu unmanagement mode should not be set
	//TODO should check if init fail
	if(readw((void *)NPU_HW_RST_PGM_EN)){
		writew(0x0, (void *)NPU_HW_RST_PGM_EN);
	}
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

int sf_port_init(struct sf_switch_s *sf_switch_dev) {
	uint i, addr1 = 0, addr2 = 0;
	/* init port data structure */
	for (i = 0; i < sf_switch_dev->max_port_num; i++) {
		addr1 = addr2 = 0;
		sf_switch_dev->port[i].shutdown = 0;
		sf_switch_dev->port[i].tpid     = 0x8100;
		sf_switch_dev->port[i].aft      = ANY_TAGGING;
		sf_switch_dev->port[i].fallback_bd_id = DEF_PORT_FALLBACK_BD_ID;
		sf_switch_dev->port[i].block_state = FORWARDING;
		sf_switch_dev->port[i].csr_untag_from_btable = 0;
		addr1 = get_port_address(i,1);
		addr2 = get_port_address(i,2);
		/* class hw port csr reg:
		 *======================================================*
		 *res |block_state |aft |shutdown |fallback_bd_id |tpid *
		 *======================================================*
		 *20-bit| 4-bit   |4-bit| 1-bit   | 16-bit      |16-bit *
		 *=====================================================*/
		if (addr1 && addr2) {
			sf_writel( ((sf_switch_dev->port[i].fallback_bd_id << PORT_FALLBACK_BDID_START_POS) & PORT_FALLBACK_BDID_MASK)
						| ((sf_switch_dev->port[i].tpid << PORT_TPID_START_POS) & PORT_TPID_MASK)
						,addr1);

			sf_writel( ((sf_switch_dev->port[i].csr_untag_from_btable << PORT_UNTAG_FROM_BTABLE_START_POS) & PORT_UNTAG_FROM_BTABLE_MASK)
						| ((sf_switch_dev->port[i].block_state << PORT_BLOCKSTATE_START_POS) & PORT_BLOCKSTATE_MASK)
						| ((sf_switch_dev->port[i].aft << PORT_AFT_START_POS) & PORT_AFT_MASK)
						| ((sf_switch_dev->port[i].vid_prefix << PORT_VID_PREFIX_START_POS) & PORT_VID_PREFIX_MASK)
						| ((sf_switch_dev->port[i].shutdown << PORT_SHUTDOWN_START_POS) & PORT_SHUTDOWN_MASK)
						, addr2);
		}
	}

	return 0;
}

void tmu_queue_init(void)
{
	uint phy_no = 0;
	uint queue_no = 0;
    for (phy_no = 0; phy_no < SF_MAX_PORTS; phy_no++) {
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
int  hash_table_config_done(int type)
{
	uint status_reg = 0;
	int cmd_done = 0;
	if(type == MAC_TABLE)
	  status_reg  = REQ_STATUS_REG;
	else if(type == VLAN_TABLE)
	  status_reg  = REQ_VLAN_STATUS_REG;
	else
	  return -1;

	/*Read command status rteg*/
	cmd_done = sf_readl(status_reg);
	/* Poll the Status Register for CMD DONE */
	while (!(cmd_done & 0x1)) {
		cmd_done = sf_readl(status_reg);
	} /*End of while loop*/
	/* Clear Status Reg */
	sf_writel( CLR_STATUS_REG,status_reg);
	return cmd_done;
}
void mac_hash_table_init(void)
{
	int  i = 0;
	unsigned int index = 0, mac_table_addr = 0, col_ptr = 0;

	/* 2-field Mac table format: Table Depth - 1024(hash 512 + collison 512)
	 *============================================================================*
	 * valid bits | col_ptr | portNO | field_valids | action entry |vlanid |MAC *
	 *============================================================================*
	 * 4-bit      | 16-bit  | 4-bit  | 8-bit        | 31-bit       |13-bit |48-bit *
	 *============================================================================*/

	/* INIT CMD: Init total Hash Space with zeros */
	sf_writel( CMD_INIT ,REQ_CMD_REG);

	hash_table_config_done(MAC_TABLE);

	/* initialize mac addresses  Link List formation */
	sf_writel( 0x0, REQ_MAC1_ADDR_REG);
	sf_writel( 0x0, REQ_MAC2_ADDR_REG);
	sf_writel( 0x0, REQ_MAC3_ADDR_REG);
	sf_writel( 0x0, REQ_MAC4_ADDR_REG);
	sf_writel( 0x0, REQ_MAC5_ADDR_REG);
	sf_writel( 0x0, REQ_ENTRY_REG);
	/* chain the collision space */
	for (i = 0; i < MAC_COLL_ENTRIES; i++) {
		/* 32bit: 4res + 4validbits + 16colptr + 4portnum +
		   (4)part of fieldvalids */
		//{8’d4, col_ptr[15:0], 8’d0}
		col_ptr = (0x40201 + i) << 8;
		sf_writel( col_ptr, REQ_MAC4_ADDR_REG);
		index = MAC_HASH_ENTRIES + i;
		mac_table_addr = (index << 16) + CMD_MEM_WRITE;
		sf_writel( mac_table_addr ,REQ_CMD_REG);
		hash_table_config_done(MAC_TABLE);
	} /*End of for loop collision space*/
	/* Intialize Free List Head Ptr */
	sf_writel( MAC_INIT_HEAD_PTR, FREE_LIST_HEAD_PTR);

	/* Initialize Free List Ptr */
	sf_writel( MAC_INIT_TAIL_PTR, FREE_LIST_TAIL_PTR);

	/* Initialize No of entries */
	sf_writel( MAC_COLL_ENTRIES, FREE_LIST_ENTRIES);
} /*end of hw_hash_table_init Function*/

void vlan_hash_table_init(void)
{
	int  i = 0;
	unsigned int index = 0, mac_table_addr = 0, col_ptr = 0;

	/* Avoid Multiple PE's Initialization */

	/* vlan group table format *
	 *=====================================================================*
	 * valid bits | col_ptr | portNO | field_valids | action entry | vlan  *
	 *=====================================================================*
	 * 4-bit      | 16-bit  | 4-bit  | 8-bit        | 32-bit       | 12-bit*
	 *=====================================================================*/
	/* VLAN HASH MEMORY INTIALIZATION */

	/* Initialize CMD register for Hash Table Intializes with zeros */
	sf_writel( CMD_INIT, REQ_VLAN_CMD_REG);

	hash_table_config_done(VLAN_TABLE);

	sf_writel(0,REQ_VLAN_MAC1_ADDR_REG);
	sf_writel(0,REQ_VLAN_MAC2_ADDR_REG);
	sf_writel(0,REQ_VLAN_MAC3_ADDR_REG);
	sf_writel(0,REQ_VLAN_MAC4_ADDR_REG);
	sf_writel(0,REQ_VLAN_MAC5_ADDR_REG);

	/* clear mac1 & mac2 registers */
	/* chain the vlan collision space */
	for (i = 0; i < 64; i++) {
		col_ptr = (0x40041 + i);
		sf_writel( ((col_ptr & 0xFF) << 24), REQ_VLAN_MAC2_ADDR_REG);
		sf_writel( (col_ptr >> 8) & 0xFFF, REQ_VLAN_MAC3_ADDR_REG );
		index = VLAN_HASH_ENTRIES + i;
		mac_table_addr = (index << 16);
		sf_writel( CMD_MEM_WRITE | mac_table_addr, REQ_VLAN_CMD_REG);

		hash_table_config_done(VLAN_TABLE);
	} /*End of vlan collision spce for loop*/

	/* Intialize Free List Head Ptr */
	sf_writel( VLAN_INIT_HEAD_PTR, FREE_VLAN_LIST_HEAD_PTR);

	/* Initialize Free List Ptr */
	sf_writel( VLAN_INIT_TAIL_PTR, FREE_VLAN_LIST_TAIL_PTR);

	/* Initialize No of entries */
	sf_writel( VLAN_FREE_LIST_ENTRIES, FREE_VLAN_LIST_ENTRIES);


} /*end of hw_vlan_hash_table_init Function*/

int sf_init_switch_hw(struct sf_switch_s *sf_switch_dev)
{
	sf_switch_dev->max_port_num      = SF_MAX_PORTS;
	sf_switch_dev->host_port     = SF_HOST_PORT;
	sf_switch_dev->fwd_port_list = FWD_PORT_LIST_MASK;
	load_hwreg_table(&sw_hwreg_data[0]);
	mac_hash_table_init();
	vlan_hash_table_init();
	sf_port_init(sf_switch_dev);
	tmu_queue_init();
	return 0;
}

int sf_flush_cache(struct sf_priv *priv, uint is_tx, uint is_tbl, uint buf_index){
#ifndef NO_CACHE
	if(is_tx){
		if(is_tbl)
			flush_dcache_range((uint)(priv->tx_bd_tbl_align + buf_index), (uint)((uint)(priv->tx_bd_tbl_align + buf_index) + sizeof(struct bd) ));
		else
			flush_dcache_range((uint)priv->tx_buf_align + MAX_FRAME_SIZE * buf_index, (uint)(priv->tx_buf_align + MAX_FRAME_SIZE * (buf_index + 1) ));
	}
	else{
		if(is_tbl)
			flush_dcache_range((uint)(priv->rx_bd_tbl_align + buf_index), (uint)((uint)(priv->rx_bd_tbl_align + buf_index) + sizeof(struct bd) ));
		else
			flush_dcache_range((uint)priv->rx_buf_align + MAX_FRAME_SIZE * buf_index, (uint)(priv->rx_buf_align + MAX_FRAME_SIZE * (buf_index + 1) ));
	}
#endif
	return 0;
}
int sf_invalidate_cache(struct sf_priv *priv, uint is_tx, uint is_tbl, uint buf_index){
#ifndef NO_CACHE

	if(is_tx){
		if(is_tbl)
			invalidate_dcache_range((uint)(priv->tx_bd_tbl_align + buf_index), (uint)((uint)(priv->tx_bd_tbl_align + buf_index) + sizeof(struct bd) ));
		else
			invalidate_dcache_range((uint)priv->tx_buf_align + MAX_FRAME_SIZE * buf_index, (uint)(priv->tx_buf_align + MAX_FRAME_SIZE * (buf_index + 1) ));
	}
	else{
		if(is_tbl)
			invalidate_dcache_range((uint)(priv->rx_bd_tbl_align + buf_index), (uint)((uint)(priv->rx_bd_tbl_align + buf_index) + sizeof(struct bd)));
		else
			invalidate_dcache_range((uint)priv->rx_buf_align + MAX_FRAME_SIZE *buf_index, (uint)(priv->rx_buf_align + MAX_FRAME_SIZE * (buf_index + 1) ));
	}
#endif
	return 0;
}

int sf_dma_start(struct sf_priv *priv)
{
	uint tx_ctrl;
	uint rx_ctrl;
	int i = 0;
	/* Initialize the rx and tx buffer descriptor table pointer
	 *
	 *	 * registers.  **/
	// flush_dcache_range((unsigned long )(priv->tx_bd_tbl),(unsigned long )(priv->tx_bd_tbl) + NUM_TX_DESCR * sizeof(struct bd) + BUF_ALIGN);
	// flush_dcache_range((unsigned long )(priv->rx_bd_tbl),(unsigned long )(priv->rx_bd_tbl) + NUM_RX_DESCR * sizeof(struct bd) + BUF_ALIGN);
	for(;i < NUM_TX_DESCR; i++){
		sf_invalidate_cache(priv, 1, 1, i);
	}
	i = 0;
	for(;i < NUM_RX_DESCR; i++){
		sf_invalidate_cache(priv, 0, 1, i);
	}

	sf_writel((uint)priv->tx_bd_tbl_align, REG_HIF_TX_BDP_ADDR);
	sf_writel((uint)priv->rx_bd_tbl_align, REG_HIF_RX_BDP_ADDR);

	/* Enable the interrupts */
	sf_writel((HIF_INT_EN | HIF_RXPKT_INT_EN | HIF_TXPKT_INT_EN),REG_HIF_INT_EN);


	tx_ctrl = sf_readl(REG_HIF_TX_CTRL);
	tx_ctrl |= CSR_TX_DMA_EN |CSR_TX_BDP_POLL_CNTR_EN;
	sf_writel(tx_ctrl, REG_HIF_TX_CTRL);

	rx_ctrl = sf_readl(REG_HIF_RX_CTRL);
	rx_ctrl |= CSR_RX_DMA_EN |CSR_RX_BDP_POLL_CNTR_EN;
	sf_writel(rx_ctrl, REG_HIF_RX_CTRL);

	return 0;
}

int sf_eth_init(struct eth_device *dev, bd_t *bt) {
	/* Enable Emac Registers */
	sf_mac_enable();
	return 0;
}

void sf_eth_halt(struct eth_device *dev)
{
	int count = 0;
	sf_mac_disable_rx();
	sf_mac_disable_tx();
	// sf_writel(0, REG_HIF_TX_CTRL);
	count = sf_readl(REG_BMU_REM_BUF_CNT);
	int i = 0;
	while (count != 0x400) {
		mdelay(10);
		count = sf_readl(REG_BMU_REM_BUF_CNT);
		if(i == 100){
			break;
			error("bmu error count %d", count);
		}
		i++;
	}

	// sf_writel(0, REG_HIF_RX_CTRL);
	return ;
}

void sf_eth_stop(struct eth_device *dev)
{
	int count = 0;
	sf_mac_disable_rx();
	sf_writel(0, REG_HIF_TX_CTRL);
	count = sf_readl(REG_BMU_REM_BUF_CNT);
	int i = 0;
	while (count != 0x400) {
		mdelay(10);
		count = sf_readl(REG_BMU_REM_BUF_CNT);
		if(i == 100){
			break;
			error("bmu error count %d", count);
		}
		i++;
	}
	sf_writel(0, REG_HIF_RX_CTRL);
	sf_mac_disable_tx();
	return ;
}

int bd_tx_ring_init(struct sf_priv *priv) {
	int i,size = 0;
	struct bd * txd= NULL;
	uint tmp= 0;
	size = NUM_TX_DESCR * sizeof(struct bd);
	/* Allocate and initialize the tx bd ring */
	priv->tx_bd_tbl = malloc(size + BUF_ALIGN);
	if (priv->tx_bd_tbl == NULL)
	{
		error("Out of memory allocating tx bd ring\n");
		return -1;
	}
	memset(priv->tx_bd_tbl, 0, size);
	priv->tx_bd_tbl_align = (struct bd *)(priv->tx_bd_tbl + BUF_ALIGN - ((uint)priv->tx_bd_tbl % BUF_ALIGN));
	priv->tx_buf = malloc(MAX_FRAME_SIZE * NUM_RX_DESCR + BUF_ALIGN);
	if(priv->tx_buf == NULL) {
		error("SKB ALLOC FAILED [%s:%d]\n", __func__, __LINE__);
		return -1;
	}
	priv->tx_buf_align = (priv->tx_buf + BUF_ALIGN  - ((uint)priv->tx_buf % BUF_ALIGN));
	for (i = 0; i < NUM_TX_DESCR; i++)
	{
		txd = priv->tx_bd_tbl_align + i;
		tmp = virt_to_phys(priv->tx_bd_tbl_align);
		txd->bd_next = tmp + (sizeof(struct bd) * ((i + 1) % NUM_TX_DESCR));
		txd->bd_status = 0;
		tmp = virt_to_phys(priv->tx_buf_align);
		txd->bd_bufptr = tmp + (i * MAX_FRAME_SIZE);
		txd->bd_ctrl = 0;
	}
	priv->txd_index = 0;
	return 0;
}

int bd_rx_ring_init(struct sf_priv *priv)
{

	int i = 0,size = 0;
	struct bd *rxd;
	uint tmp = 0;
	size = NUM_RX_DESCR * sizeof(struct bd);
	/* Allocate and initialize the rx bd ring */
	priv->rx_bd_tbl =  malloc(size + BUF_ALIGN);

	if (priv->rx_bd_tbl == NULL)
	{
		error("Out of memory allocating rx bd ring\n");
		return -1;
	}

	memset(priv->rx_bd_tbl, 0, size);
	priv->rx_bd_tbl_align = (struct bd *)(priv->rx_bd_tbl + BUF_ALIGN - ((uint)priv->rx_bd_tbl % BUF_ALIGN));
	priv->rx_buf = malloc(MAX_FRAME_SIZE * NUM_RX_DESCR + BUF_ALIGN);

	if(priv->rx_buf == NULL) {
		error("SKB ALLOC FAILED [%s:%d]\n", __func__, __LINE__);
		return -1;
	}
	priv->rx_buf_align = (priv->rx_buf + BUF_ALIGN - ((uint)priv->rx_buf % BUF_ALIGN));
	for (i = 0; i < NUM_RX_DESCR; i++)
	{
		rxd = priv->rx_bd_tbl_align + i;
		tmp = virt_to_phys(priv->rx_bd_tbl_align);
		rxd->bd_next = tmp  + (sizeof(struct bd) * ((i + 1) % NUM_RX_DESCR));
		rxd->bd_status = 0;
		tmp = virt_to_phys(priv->rx_buf_align);
		rxd->bd_bufptr = tmp + (i * MAX_FRAME_SIZE);
		//last bd fill with 0, hardware should stop here
		if(i == (NUM_RX_DESCR -1))
		  rxd->bd_ctrl = 0;
		else
		  /* Enable per packet receive interrupt */
		  rxd->bd_ctrl = MAX_FRAME_SIZE | (BD_CTRL_PKT_INT_EN | BD_CTRL_LIFM |
					  BD_CTRL_DIR | BD_CTRL_DESC_EN);
	}

	/* Initialize the rx bd read pointer */
	priv->rxd_index = 0;

	return 0;
}

int sf_bd_ring_init(struct sf_priv *priv)
{
	int i = 0;
	/* Allocate memory and initialize bd rings */
	if (bd_tx_ring_init(priv) < 0) {
		return -1;
	}
	if (bd_rx_ring_init(priv) < 0)
	{
		return -1;
	}
	for(;i < NUM_TX_DESCR; i++){
		sf_flush_cache(priv, 1, 1, i);
	}
	i = 0;
	for(;i < NUM_RX_DESCR; i++){
		sf_flush_cache(priv, 0, 1, i);
	}
	return 0;
}

int check_txbd_index(struct sf_priv *priv)
{
	struct bd *n_txd;
	uint n_tx_idx;
	/* Add the frame to tx bd ring. After adding the frame, set
	 ** the next descriptor's DESC_EN bit 0. This is required
	 ** because the hardware expects a dummy descriptor at the
	 ** end of descriptor's with valid frames.
	 **/

	// invalidate_dcache_range((unsigned long )(priv->tx_bd_tbl_align),(unsigned long )(priv->tx_bd_tbl_align) + NUM_TX_DESCR * sizeof(struct bd) + BUF_ALIGN);

	n_tx_idx = ((priv->txd_index + 1) & (NUM_TX_DESCR - 1));
	n_txd = priv->tx_bd_tbl_align + n_tx_idx;

	sf_invalidate_cache(priv, 1, 1, n_tx_idx);
	/* Check queue full condition */
	if (n_txd->bd_ctrl & BD_CTRL_DESC_EN) {
		error("Tx Queue Full txd_index %d bdctrl 0x%x\n",n_tx_idx,n_txd->bd_ctrl);
		return -1;
	}

	return 0;
}

int sf_add_tx_header(char * buf,int txport) {
	struct tx_header *txhdr;
	txhdr = (struct tx_header *)buf;
	memset(txhdr, 0, sizeof(struct tx_header));

	txhdr->ctrl = SF_TX_PKT_INJECT_EN;
#ifdef CONFIG_SFA18_PCBA_TEST
	txhdr->txport_map = g_priv->txport;
#else
	txhdr->txport_map = txport;
#endif

	txhdr->rsvd0 = 0;
	txhdr->queue = 0;
	txhdr->rsvd1 = 0;

	return 0;
}

int check_padding(char* buf, int* len){

	int i = 0;
	if (*len < 60) {
		for(i=0; i<60-*len; i++) {
			buf[*len+i] = 0;
		}
		*len = 60;
	}
	return 0;
}
int sf_eth_send(struct eth_device *dev, void *packet, int length) {
	struct bd *txd;
	uint pkt_len;
	char *ptr;
	int ctrl_word = 0;
	int i = 0;
#ifndef CONFIG_SFA18_PCBA_TEST
	unsigned long port_list;
#endif
	struct sf_priv* priv = (struct sf_priv *)dev->priv;
	// check_padding(packet, &length);
	if(check_txbd_index(priv)){
		error("tx bd fail \n");
		return -1;
	}
	// don't do the loop in pcba test!
#ifndef CONFIG_SFA18_PCBA_TEST
	port_list = CONFIG_SFA18_ETH_PORTS;
	for (i = 0; i < SF_MAC_PORTS; i++) {
		if (port_list & (1 << i)) {
#endif
				txd = priv->tx_bd_tbl_align + priv->txd_index;
				ptr = phys_to_virt(txd->bd_bufptr);
				/* Prepend the tx header to transmit packet */
				sf_add_tx_header(ptr,0x1<<i);
				/* copy packet data into dma buffer */
				memcpy(ptr + 16, packet, length);
				sf_flush_cache(priv, 1, 0, priv->txd_index);
				// flush_dcache_range((unsigned long)ptr,(unsigned long)ptr + length + 16);
				pkt_len = length + 16;
				ctrl_word = (uint)(BD_CTRL_PKT_INT_EN |
						BD_CTRL_LIFM | BD_CTRL_DESC_EN |
						pkt_len |
						BD_CTRL_RTFETCH_DISABLE);

				txd->bd_ctrl = ctrl_word;

				sf_flush_cache(priv, 1, 1, priv->txd_index);
				//error("txd->bd_ctrl %08x\n",txd->bd_ctrl);
				////wait for transfer done
				while(1){
					sf_invalidate_cache(priv,1,1,priv->txd_index);

					if(!(txd->bd_ctrl & BD_CTRL_DESC_EN))
						break;
					udelay(10);
				}
				// error("transfer done txd->bd_ctrl %08x\n",txd->bd_ctrl);
				// error("txd in ram after invalid is %08x\n",txd_tmp->bd_ctrl);
				priv->txd_index = ((priv->txd_index + 1) & (NUM_TX_DESCR - 1));
				// error("pnext txd index  is %d\n",priv->txd_index);
				// sf_writel(hif_int_src,REG_HIF_INT_SRC);
#ifndef CONFIG_SFA18_PCBA_TEST
		}
	}
#endif
	return 0;
}


int sf_eth_recv(struct eth_device *dev) {

	//volatile struct bd *rxd, *p_rxd;
	struct bd *rxd, *p_rxd;

	// struct bd *rxd_debug, *p_rxd_debug;
	struct sf_priv* priv = (struct sf_priv *)dev->priv;

	uint p_rx_index;
	char* rx_pkt = NULL;
	uchar *buf = net_rx_packets[0];
	struct rx_header* rx_hdr = NULL;
	do{
		rxd = priv->rx_bd_tbl_align + priv->rxd_index;
		sf_invalidate_cache(priv,0,1,priv->rxd_index);
		if (rxd->bd_ctrl & BD_CTRL_DESC_EN){
			//error("no nore rx bd_index %d\n",priv->rxd_index);
			// rxd_debug = (struct bd *)(MAP_ADDR_TO_UNCACHE((unsigned long)rxd));
			// if(!(rxd_debug->bd_ctrl & BD_CTRL_DESC_EN))
			//   error("ram cache different index %d",priv->rxd_index);
			return -1;
		}

		p_rx_index = ((priv->rxd_index + NUM_RX_DESCR -1) & (NUM_RX_DESCR - 1));;

		p_rxd = priv->rx_bd_tbl_align + p_rx_index;

#if 1
		//todo: check why have to use uncached memory
		// p_rxd_debug = (struct bd *)(MAP_ADDR_TO_UNCACHE((unsigned long)p_rxd));
#endif

		//No more descriptors to process
#if 0
		invalidate_dcache_range((unsigned long)rxd,(unsigned long)rxd + sizeof(struct bd));
		invalidate_dcache_range((unsigned long)p_rxd,(unsigned long)p_rxd + sizeof(struct bd));
#endif
		//invalidate_dcache_range((unsigned long)(priv->rx_bd_tbl),(unsigned long)(priv->rx_bd_tbl) + NUM_RX_DESCR * sizeof(struct bd) + BUF_ALIGN);
		uint len = (rxd->bd_ctrl & BD_CTRL_BD_BUFLEN_MASK);
		if (len > 4 && len < PKTSIZE_ALIGN) {
			buf = net_rx_packets[priv->rxd_index % PKTBUFSRX];

			rx_pkt = (char *)phys_to_virt(rxd->bd_bufptr);
			// invalidate_dcache_range((unsigned long)rx_pkt,(unsigned long)rx_pkt + PKTSIZE_ALIGN);
			sf_invalidate_cache(priv,0,0,priv->rxd_index);

			rx_hdr = (struct rx_header *)rx_pkt;

			if(rx_hdr->punt_reason & 0x2){
			  debug("rx_packet punt 0x%x len %d\n",rx_hdr->punt_reason,len);
			}

#ifdef CONFIG_SFAX8_NPU_TEST
			if (priv->pcba_npu_test == 0){
#endif
				memcpy(buf, rx_pkt, len);
				net_process_received_packet(buf + 16, len - 16);
#ifdef CONFIG_SFAX8_NPU_TEST
			}else{
				printf("npu test here, recv port %d\n", rx_hdr->rxport_num);
				//printf("recv port %d DA: 0x%02x:%02x:%02x:%02x:%02x:%02x SA: 0x%02x:%02x:%02x:%02x:%02x:%02x\n", rx_hdr->rxport_num,*(rx_pkt+16), *(rx_pkt+16+1),*(rx_pkt+16+2),*(rx_pkt+16+3),*(rx_pkt     +16+4),*(rx_pkt+16+5), *(rx_pkt+16+6), *(rx_pkt+16+7),*(rx_pkt+16+8),*(rx_pkt+16+9),*(rx_pkt+16+10),*(rx_pkt+16+11));
				//printf("recv VLAN tag: 0x%02x:%02x:%02x:%02x\n", *(rx_pkt+16+12),*(rx_pkt+16+13), *(rx_pkt+16+14),*(rx_pkt+16+15));
				if (rx_hdr->rxport_num == priv->rxport && (len - 16) == PCBA_PKT_SIZE){
					if (!memcmp(rx_pkt + 16, eth_test_pkt, PCBA_PKT_SIZE))
					  priv->test_result = 1;
				}
			}
#endif
		}else{
			error("%s: Wrong Ethernet packet size (%d B), skip!\n",
			      __func__, len);
		}

		sf_invalidate_cache(priv,0,1,p_rx_index);
		if(p_rxd->bd_ctrl == 0) {
			/*  Enable per packet receive interrupt */
			rxd->bd_ctrl = 0;
			sf_flush_cache(priv,0,1,priv->rxd_index);
			p_rxd->bd_ctrl = MAX_FRAME_SIZE|(BD_CTRL_PKT_INT_EN |BD_CTRL_LIFM | BD_CTRL_DIR | BD_CTRL_DESC_EN);
			sf_flush_cache(priv,0,1,p_rx_index);
			priv->rxd_index = ((priv->rxd_index + 1) & (NUM_RX_DESCR - 1));

#if 0
			flush_dcache_range((unsigned long)rxd,(unsigned long)rxd + sizeof(struct bd));
			flush_dcache_range((unsigned long)p_rxd,(unsigned long)p_rxd + sizeof(struct bd));
#endif
		}
		else
			error("previous rx bd ctrl is not zero, error %x\n",p_rxd->bd_ctrl);

	}while(1);
	return 0;
}

#define STATUS_CMD_DONE              0x00000001
#define STATUS_ENTRY_ADDED           0x00000008
int vlan_entry_add(uint field_entries,
			uint portNo, uint action_entry, uint field_valids) {
	int cmd_done = 0;
	/* fill vlan reg */

	sf_writel(1,REQ_VLAN_MAC1_ADDR_REG);
	sf_writel(0,REQ_VLAN_MAC2_ADDR_REG);
	sf_writel(0,REQ_VLAN_MAC3_ADDR_REG);
	sf_writel(0,REQ_VLAN_MAC4_ADDR_REG);
	sf_writel(0,REQ_VLAN_MAC5_ADDR_REG);
	/* fill ENTRY reg */
	sf_writel(action_entry, REQ_VLAN_ENTRY_REG);

	/* Issue ADD cmd */
	sf_writel(CMD_ADD | field_valids , REQ_VLAN_CMD_REG);

 	/*Read command status rteg*/
	cmd_done = sf_readl(REQ_VLAN_STATUS_REG);

	/* Poll the Status Register for CMD DONE */
	while (!(cmd_done & 0x1)) {
        cmd_done = sf_readl(REQ_VLAN_STATUS_REG);
	}

	sf_writel(0xffffffff,REQ_VLAN_STATUS_REG);

	if ((cmd_done & STATUS_CMD_DONE) &&(cmd_done & STATUS_ENTRY_ADDED)) {
		return 0;
	} else {
		error("add vlan default entry fail\n");
		return -1;
	}
}


#define BIT_SET_VLAN_1F_VLANTBL     0x00000100
int add_vlan_entry_with_details(int vlanid, int  port, int port_list,int untag_list)
{
	uint action_entry = 0;

	/*Adding vlan id 1, port list*/
    action_entry =  (port_list << 0) | (untag_list << 8);

    return vlan_entry_add(vlanid, port, action_entry, BIT_SET_VLAN_1F_VLANTBL);
}

#define  swap_short(in_usValue) (((0xFF) & ((in_usValue) >> 8)) | ((0xFF00) & ((in_usValue) << 8)))
#define  swap_long(in_usValue) (((0xFF) & ((in_usValue) >> 24)) | ((0xFF00) & ((in_usValue) >> 8)) | ((0xFF0000) & ((in_usValue) << 8)) | ((0xFF000000) & ((in_usValue) << 24)))

int mac_entry_add(uint field_entries[5], uint iport,
					uint brentry, uint field_valids) {
	int cmd_done = 0;

	/* initialize MAC regs */
	sf_writel(swap_long(field_entries[0]), REQ_MAC1_ADDR_REG);
	sf_writel((field_entries[1] & 0xFFFF0000) |
				swap_short(field_entries[1]), REQ_MAC2_ADDR_REG);
	sf_writel( swap_long(field_entries[2]), REQ_MAC3_ADDR_REG);
	sf_writel( swap_long(field_entries[3]), REQ_MAC4_ADDR_REG);

	/* fill ENTRY reg */
	sf_writel(brentry, REQ_ENTRY_REG);

	/* Issue ADD cmd */
	sf_writel(CMD_ADD | field_valids | (iport << 16), REQ_CMD_REG);

 	/*Read command status rteg*/
	cmd_done = sf_readl(REQ_STATUS_REG);

	/* Poll the Status Register for CMD DONE */
	while (!(cmd_done & 0x1)) {
        cmd_done = sf_readl(REQ_STATUS_REG);
	}

	sf_writel(0xffffffff,REQ_STATUS_REG);

	if ((cmd_done & STATUS_CMD_DONE) &&( cmd_done & STATUS_ENTRY_ADDED)) {
		return 0;
	} else {
		error("add default mac entry fail");
		return -1;
	}
}

#define BIT_SET_MAC_2F_MACTBL     0x00000100
#define BIT_SET_VLAN_2F_MACTBL    0x00000200
int add_default_mac_entry(unsigned char *mac, int port)
{
	int action_entry = 0;
	//punt to host
	int phy_port = port;

	int vlanid = 1;
//for vid_prefix set
   // int vlanid = (0x1 << 12) | 0x1;

	int field_valids = BIT_SET_MAC_2F_MACTBL | BIT_SET_VLAN_2F_MACTBL;
	uint  field_entries[5];

	field_entries[0] = ((*mac << 0)&0x000000ff) |((*(mac + 1) << 8)&0x0000ff00) | ((*(mac + 2) << 16)&0x00ff0000) |((*(mac + 3) << 24)&0xff000000);
	field_entries[1] = ((*(mac + 4) << 0)&0x000000ff) |((*(mac + 5) << 8)&0x0000ff00);
	field_entries[1] |= (vlanid << 16);
	field_entries[2] = 0;
	field_entries[3] = 0;
	field_entries[4] = 0;

	action_entry |= ((0x1 << phy_port) & FWD_PORT_LIST_MASK);
	int add = mac_entry_add(field_entries, phy_port,action_entry, field_valids);
	return add;
}

int sf_eth_write_hwaddr(struct eth_device *dev) {
	int tmp = 0;

	sf_writel(htonl(*(uint32_t *)dev->enetaddr), REG_MAC_ADDRESS0_LOW(0));
	tmp = sf_readl(REG_MAC_ADDRESS0_HIGH(0));
	tmp = (tmp & 0xffff0000) | htons(*(uint32_t *)&dev->enetaddr[4]);
	sf_writel(tmp, REG_MAC_ADDRESS0_HIGH(0));
	sf_writel(htonl(*(uint32_t *)dev->enetaddr), REG_MAC_ADDRESS0_LOW(1));
	tmp = sf_readl(REG_MAC_ADDRESS0_HIGH(1));
	tmp = (tmp & 0xffff0000) | htons(*(uint32_t *)&dev->enetaddr[4]);
	sf_writel(tmp, REG_MAC_ADDRESS0_HIGH(1));
	sf_writel(htonl(*(uint32_t *)dev->enetaddr), REG_MAC_ADDRESS0_LOW(2));
	tmp = sf_readl(REG_MAC_ADDRESS0_HIGH(2));
	tmp = (tmp & 0xffff0000) | htons(*(uint32_t *)&dev->enetaddr[4]);
	sf_writel(tmp, REG_MAC_ADDRESS0_HIGH(2));
	sf_writel(htonl(*(uint32_t *)dev->enetaddr), REG_MAC_ADDRESS0_LOW(3));
	tmp = sf_readl(REG_MAC_ADDRESS0_HIGH(3));
	tmp = (tmp & 0xffff0000) | htons(*(uint32_t *)&dev->enetaddr[4]);
	sf_writel(tmp, REG_MAC_ADDRESS0_HIGH(3));
	sf_writel(htonl(*(uint32_t *)dev->enetaddr), REG_MAC_ADDRESS0_LOW(4));
	tmp = sf_readl(REG_MAC_ADDRESS0_HIGH(4));
	tmp = (tmp & 0xffff0000) | htons(*(uint32_t *)&dev->enetaddr[4]);
	sf_writel(tmp, REG_MAC_ADDRESS0_HIGH(4));

	add_default_mac_entry(dev->enetaddr, 0);

    add_vlan_entry_with_details(DEF_PORT_FALLBACK_BD_ID, 0, FWD_PORT_LIST_MASK, 0);
	return 0;
}

int sf_eth_register(void){
#ifdef CONFIG_SFA18_PCBA_TEST
	unsigned long port_list;
#ifdef CONFIG_TARGET_SFA18_86V
	unsigned long addr1, temp_port_entry;
#endif
#endif
	struct eth_device *dev;
	struct sf_priv    *priv;
	dev = (struct eth_device *)malloc(sizeof(struct eth_device));
	if (dev == NULL) {
		error("%s: Not enough memory!\n", __func__);
		return -1;
	}
	priv = (struct sf_priv *)malloc(sizeof(struct sf_priv));
	if (priv == NULL) {
		error("%s: Not enough memory!\n", __func__);
		return -1;
	}
	memset(dev, 0, sizeof(*dev));
	memset(priv, 0, sizeof(*priv));

	sprintf(dev->name, "sf_eth0");
	dev->priv = (void *)priv;
	dev->iobase = NPU_ADDR_BASE;
	dev->init = sf_eth_init;
	dev->halt = sf_eth_halt;
	dev->stop = sf_eth_stop;
	dev->send = sf_eth_send;
	dev->recv = sf_eth_recv;
	dev->write_hwaddr = sf_eth_write_hwaddr;
#if !defined CONFIG_SFA18_PCBA_TEST || defined CONFIG_SFAX8_WLAN_NPU
	eth_register(dev);
#endif
	sys_npu_reset();

	sf_writel(0, REG_HIF_INT_EN);
	uint int_src = sf_readl(REG_HIF_INT_SRC);
	sf_writel(int_src, REG_HIF_INT_SRC);

	sf_local_reset();
	sf_phy_init(priv->phy_dev);
	sf_init_switch_hw(&priv->sf_switch);

#ifdef CONFIG_SFA18_PCBA_TEST
	port_list = CONFIG_SFA18_ETH_PORTS;
	//printf("get config portlist %lx\n", port_list);

#ifdef CONFIG_TARGET_SFA18_86V
	// To prevent brodcast package loopback in 86v, set port0 tagged only
	addr1 = get_port_address(0, 2);
	temp_port_entry = sf_readl(addr1) & (~PORT_AFT_MASK);
	temp_port_entry |= TAGGED_ONLY << PORT_AFT_START_POS;
	sf_writel(temp_port_entry, addr1);
#endif

	// disable unused ports
	sf_writel( ((port_list | 1<<SF_HOST_PORT)|1<<BRENTRY_UCAST_MISS_ACT_START_POS|1<<BRENTRY_MCAST_MISS_ACT_START_POS) << 1, REG_CLASS_GLOBAL_CFG);
	writeb(port_list, (void*)(OMINI_PHY_ENABLE));

#ifdef CONFIG_SFAX8_DEFAULT_TX_PORT
	priv->txport = 1 << CONFIG_SFAX8_DEFAULT_TX_PORT;
#endif

	priv->dev = dev;
	g_priv = priv;
#endif
	/* Allocate memory and initialize bd rings */
	if (sf_bd_ring_init(priv) < 0) {
		error( "bd ring init fail\n");
		return -1;
	}
	sf_dma_start(dev->priv);
	return 0;
}
