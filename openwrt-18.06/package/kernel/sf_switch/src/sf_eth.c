#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/reset.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/skbuff.h>
#include <linux/interrupt.h>
#include <linux/etherdevice.h>
#include <linux/stat.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/clk.h>
#include <asm/clock.h>
#include <linux/if_vlan.h>
#include <linux/mtd/mtd.h>
#include <linux/of_net.h>
#include <sf16a18.h>
#ifdef CONFIG_SFAX8_FACTORY_READ
#include <linux/sfax8_factory_read.h>
#endif
#include "sf_eth.h"
#include "sf_mdio.h"
#include "sf_ethtool.h"
#include "sf_eth_addr.h"
#include "sf_switch_init.h"
#ifdef CONFIG_SFAX8_SWITCH_VLAN
#include "sf_switch.h"
#endif
#include "sf_switch_hash_table.h"
#include "sf_common.h"
#ifdef CONFIG_SFAX8_SWITCH_API
#include "sf_api.h"
#endif

#include "sf_debug.h"
#ifdef CONFIG_DEBUG_FS
#include <linux/debugfs.h>
#endif

volatile int rx_speed = 0;
volatile int tx_speed = 0;
volatile int sync_flag = 0;

#ifdef CONFIG_SFAX8_SWITCH_DPS
/* register generic netlink family for DPS */
extern int sf_genl_family_init(sf_nlfamily *family, char *family_name, char *group_name);
extern int sf_genl_family_exit(struct genl_family *family);
#endif

extern void sfax8_switch_disable_int(struct sf_priv *priv);
static int sf_msg_type = -1;
module_param_named(msg_type, sf_msg_type, int, S_IRUGO);
MODULE_PARM_DESC(msg_type, "Message level (-1=defaults,0=none,...,16=all)");
static void __iomem *sf_base = 0;

/*Statistics reset times, tx queue stop times and skb alloc fail times*/
int eth_reset_count = 0;
int tx_q_stop_count = 0;
int skb_al_fail_count = 0;

#ifdef SF_TX_SHUTDOWN
static unsigned char remove_auto[SF_MAC_PORTS] = {0};
unsigned char start_tx_shutdown[SF_MAC_PORTS] = {0};
#endif
struct sf_priv * g_sf_priv = NULL;

// 0xbc000000 0xbc010000
void sf_writel(uint value,uint offset) {
	return writel(value, sf_base + offset);
}

int sf_readl(uint offset){
	return readl(sf_base + offset);
}


void sf_tx_inject_packet(struct sk_buff *skb, int rx_port, struct sf_priv* priv)
{

	struct sk_buff *new_skb = NULL;
	int port_num = 0, port_list = 0, vlanid; /*Fallback bd id*/
	ushort eth_type;
	vlanEthhdr_t *vlanEthHdr;

	print_msg(ETH_TX_DEBUG,priv->netdev,"%s in\n",__FUNCTION__);
	eth_type = SF_SHORT_SWAP(((struct ethhdr *)skb->data)->h_proto);

	if (eth_type == ETH_P_8021Q) {

		vlanEthHdr = (vlanEthhdr_t *)(skb->data);
		vlanid = SF_SHORT_SWAP(vlanEthHdr->h_vlan_TCI) & 0xFFF;
	}
	else{
		vlanid = DEF_PORT_FALLBACK_BD_ID;
#ifdef CONFIG_SFAX8_SWITCH_VLAN
		vlanid = priv->sfax8_switch->port[rx_port].fallback_bd_id;
#endif
	}

	/*Getting vlan table entry port list*/
	port_list = vlan_hash_table_search(vlanid);
	if (port_list == -1) {
		port_list = port_list & FWD_PORT_LIST_MASK;
#ifdef CONFIG_SFAX8_SWITCH_VLAN
		port_list = (priv->sfax8_switch->fallback_bd_entry.forward_list &
					FWD_PORT_LIST_MASK);
#endif
	}


	/* this condition should not occur, flood to all ports except iport */
	for (port_num = 0; port_num < SF_MAX_PORTS; port_num++) {
		/* check for port bit set */
		if ((port_list >> port_num) & 0x1) {
			/* fwd port shldn't be input port and hif port */
			if ((rx_port != port_num) && (port_num != SF_HOST_PORT)) {

				/* Don't touch skb, use local new_skb */
				new_skb = skb_clone(skb, GFP_ATOMIC);
				if(new_skb != NULL){
					new_skb->cb[CB_INJ_TX_FLAG] = CB_TX_SA_MISS;
					new_skb->cb[CB_INJ_TX_PORT] = port_num;
					dev_queue_xmit(new_skb);
				}
				else{
					dev_err(priv->device,"skb clone error\n");
				}
			}
		}
	}
	print_msg(ETH_TX_DEBUG,priv->netdev,"%s done\n",__FUNCTION__);
}

int bd_tx_ring_init(struct sf_priv *priv) {
	int i,size = 0;
	struct bd * tmp = NULL;
	size = NUM_TX_DESCR * sizeof(struct bd);
	/* Allocate and initialize the tx bd ring */
	if(priv->iram_addr > 0){
		priv->tx_bd_tbl = (struct bd *)(priv->iram_addr);
		// turn iram addr to phy addr
		priv->tx_bd_tbl_pa = (dma_addr_t)((int)priv->tx_bd_tbl & 0x5fffffff);
	}
	else
	  priv->tx_bd_tbl = (struct bd *) dma_alloc_coherent(&priv->netdev->dev, size, &priv->tx_bd_tbl_pa,GFP_ATOMIC);

	print_msg(ETH_INIT_DEBUG,priv->netdev,"tx_bd_tbl is 0x%p\n",priv->tx_bd_tbl);
	// if (priv->tx_bd_tbl == NULL)
	// {
	// 	dev_err(priv->device,"Out of memory allocating tx bd ring\n");
	// 	return -1;
	// }
	memset(priv->tx_bd_tbl,0,size);

	for (i = 0; i < NUM_TX_DESCR; i++)
	{
		tmp = priv->tx_bd_tbl + i;
		tmp->bd_next = (uint)(priv->tx_bd_tbl_pa + (sizeof(struct bd) * ((i + 1) % NUM_TX_DESCR)));
		priv->p_tx_skb[i] = NULL;
	}

	/* Initialize the tx bd write pointer */
	priv->txd_next_index = 0;
	priv->txd_free_index= 0;
	return 0;
}

int bd_rx_ring_init(struct sf_priv *priv) {

	int i = 0, size = 0;
	int bufsz = priv->netdev->mtu + ETH_HLEN + ETH_FCS_LEN+ SF_RX_HDR_LEN;
	unsigned int align_bufsz = ALIGN(bufsz, 8);
	struct bd *rxd;
	struct sk_buff *skb;
	dma_addr_t dma_addr= 0;

	size = NUM_RX_DESCR * sizeof(struct bd);
	/* Allocate and initialize the rx bd ring */
	if(priv->iram_addr > 0){
		priv->rx_bd_tbl = (struct bd *)(priv->iram_addr + sizeof(struct bd) * NUM_TX_DESCR);
		priv->rx_bd_tbl_pa = (dma_addr_t)((int)priv->rx_bd_tbl & 0x5fffffff);
	}
	else
	  priv->rx_bd_tbl = (struct bd *) dma_alloc_coherent(&priv->netdev->dev, size, &priv->rx_bd_tbl_pa,GFP_ATOMIC);

	print_msg(ETH_INIT_DEBUG,priv->netdev,"rx_bd_tbl is 0x%p\n",priv->rx_bd_tbl);
	// if (priv->rx_bd_tbl == NULL)
	// {
	// 	dev_err(priv->device,"Out of memory allocating rx bd ring\n");
	// 	return -1;
	// }
	memset(priv->rx_bd_tbl,0,size);

	for (i = 0; i < NUM_RX_DESCR; i++)
	{
		rxd = priv->rx_bd_tbl + i;
		rxd->bd_next = (uint)(priv->rx_bd_tbl_pa + (sizeof(struct bd) * ((i + 1) % NUM_RX_DESCR)));

		skb = __netdev_alloc_skb_ip_align(priv->netdev, align_bufsz + EXTER_HEADROOM, GFP_KERNEL|GFP_DMA);
		if(skb == NULL) {
			dev_err(priv->device,"SKB ALLOC FAILED [%s:%d]\n", __func__, __LINE__);
            skb_al_fail_count++;
			return -1;
		}
		skb_reserve(skb, EXTER_HEADROOM);
		priv->p_rx_skb[i] = skb;
		// size = skb_end_pointer(skb) - skb->data;
		dma_addr = dma_map_single(priv->device, skb->data,
					align_bufsz, DMA_FROM_DEVICE);
		rxd->bd_status = 0;
		rxd->bd_bufptr = (uint)dma_addr;
		//last bd fill with 0, hardware should stop here
		if(i == (NUM_RX_DESCR -1))
		  rxd->bd_ctrl = 0;
		else
		  /* Enable per packet receive interrupt */
		  rxd->bd_ctrl = align_bufsz | (BD_CTRL_PKT_INT_EN | BD_CTRL_LIFM |
					  BD_CTRL_DIR | BD_CTRL_DESC_EN);
	}

	/* Initialize the rx bd read pointer */
	priv->rxd_index = 0;

	return 0;
}

void bd_ring_deinit(int type, struct sf_priv *priv){
	struct bd **tmp_bd_tbl = NULL;
	struct sk_buff **p_tmp_skb;
	dma_addr_t *tmp_bd_tbl_pa;
	int bd_count, i = 0;
	int bufsz = priv->netdev->mtu + ETH_HLEN + ETH_FCS_LEN+ SF_RX_HDR_LEN;
	unsigned int align_bufsz = ALIGN(bufsz, 8);

	if (type == 0){
		bd_count = NUM_TX_DESCR;
		p_tmp_skb = priv->p_tx_skb;
		tmp_bd_tbl = &(priv->tx_bd_tbl);
		tmp_bd_tbl_pa = &(priv->tx_bd_tbl_pa);
	}else{
		bd_count = NUM_RX_DESCR;
		p_tmp_skb = priv->p_rx_skb;
		tmp_bd_tbl = &(priv->rx_bd_tbl);
		tmp_bd_tbl_pa = &(priv->rx_bd_tbl_pa);
	}

	for(;i < bd_count; i++){
		if (p_tmp_skb[i] != NULL){
			if((*tmp_bd_tbl)[i].bd_bufptr && p_tmp_skb[i]){
				if (type == 0)
				  dma_unmap_single(&priv->netdev->dev, (*tmp_bd_tbl)[i].bd_bufptr, p_tmp_skb[i]->len, DMA_TO_DEVICE);
				else
				  dma_unmap_single(&priv->netdev->dev, (*tmp_bd_tbl)[i].bd_bufptr, align_bufsz, DMA_FROM_DEVICE);
			}
			if (p_tmp_skb[i])
				dev_kfree_skb(p_tmp_skb[i]);
		}
	}
	memset(p_tmp_skb, 0, sizeof(p_tmp_skb) * bd_count);

	if (*tmp_bd_tbl != NULL){
		dma_free_coherent(&priv->netdev->dev,
					(sizeof(struct bd) * bd_count), *tmp_bd_tbl, *tmp_bd_tbl_pa);
		*tmp_bd_tbl = NULL;
		*tmp_bd_tbl_pa = 0;
	}

	return;
}

void bd_tx_ring_deinit(struct sf_priv *priv) {
	bd_ring_deinit(0, priv);
}

void bd_rx_ring_deinit(struct sf_priv *priv) {
	bd_ring_deinit(1, priv);
}

int sf_bd_ring_init(struct sf_priv *priv) {
	if (bd_tx_ring_init(priv) < 0) {
		return (-ENOMEM);
	}
	if (bd_rx_ring_init(priv) < 0)
	{
		bd_tx_ring_deinit(priv);
		return (-ENOMEM);
	}
	return 0;
}
// if hw process this curr bd and waiting next bd, shoud give the next idx to hw
int sf_find_desc(struct sf_priv *priv, unsigned int * ptx_hw_addr) {
	unsigned int i = 0, curr_idx = 0;
	struct bd* tx_bd = NULL;
	for(; i < NUM_TX_DESCR; i++){
		curr_idx = (priv->txd_free_index + i) & (NUM_TX_DESCR -1);
		if(priv->tx_bd_tbl_pa + curr_idx * sizeof(struct bd) == *ptx_hw_addr){
			break;
		}
	}
	tx_bd = priv->tx_bd_tbl + curr_idx;
	if((curr_idx != priv->txd_next_index) && !(tx_bd->bd_ctrl & BD_CTRL_DESC_EN) ){
		*ptx_hw_addr += sizeof(struct bd);
		print_msg(ETH_RESET_DEBUG,priv->netdev,"curr_idx %d get correct address %08x\n", curr_idx ,*ptx_hw_addr);
	}

	return 0;
}

int sf_dma_start(struct sf_priv *priv, unsigned int tx_hw_addr, unsigned int rx_hw_addr) {
	uint tx_ctrl;
	uint rx_ctrl;
	/* Initialize the rx and tx buffer descriptor table pointer
	 *	 * registers.
	 *		 */
	sf_writel(tx_hw_addr, REG_HIF_TX_BDP_ADDR);
	sf_writel(rx_hw_addr, REG_HIF_RX_BDP_ADDR);

	/* Enable the interrupts */
	sf_writel((HIF_INT_EN | HIF_RXPKT_INT_EN | HIF_TXPKT_INT_EN),REG_HIF_INT_EN);

	tx_ctrl = sf_readl(REG_HIF_TX_CTRL);
	// poll mode, hw automatic poll bd, for reduce ddr use, not use it
	tx_ctrl |= CSR_TX_DMA_EN |CSR_TX_BDP_POLL_CNTR_EN;
	// tx_ctrl |= CSR_TX_DMA_EN;
	sf_writel(tx_ctrl, REG_HIF_TX_CTRL);

	rx_ctrl = sf_readl(REG_HIF_RX_CTRL);
	rx_ctrl |= CSR_RX_DMA_EN |CSR_RX_BDP_POLL_CNTR_EN;
	sf_writel(rx_ctrl, REG_HIF_RX_CTRL);

	return 0;
}

void sf_disable_int(struct sf_priv *priv) {
	uint  int_src = 0;
	sf_writel(0, REG_HIF_INT_EN);
	int_src = sf_readl(REG_HIF_INT_SRC);
	sf_writel(int_src, REG_HIF_INT_SRC);
	return;
}

int sf_eth_reset(struct sf_priv *priv){
	int clkgate = 0;
	unsigned int tx_hw_addr, rx_hw_addr;
#ifdef SF_TX_SHUTDOWN
	unsigned int i = 0;
#endif

	// diasble irq
	sfax8_switch_disable_int(priv);
	sf_disable_int(priv);
#ifdef CONFIG_SFAX8_SWITCH_AGEING
	sf_switch_init_timer(0);
#endif
	sf_mac_disable_rx(SF_MAC_PORTS_LIST);
	sf_writel(0, REG_HIF_RX_CTRL);
	napi_disable(&priv->napi_tx);
	napi_disable(&priv->napi_rx);
	netif_tx_disable(priv->netdev);
	sf_writel(0, REG_HIF_TX_CTRL);
	sf_mac_disable_tx(SF_MAC_PORTS_LIST);


	//keep current hardware process bd
	tx_hw_addr = sf_readl(REG_HIF_TX_CURR_BD_ADDR);
	rx_hw_addr = sf_readl(REG_HIF_RX_CURR_BD_ADDR);

	print_msg(ETH_RESET_DEBUG,priv->netdev,"keep txhw %08x rx hw %08x tx_idx %d tx free idx %d rx_idx %d\n", tx_hw_addr, rx_hw_addr, priv->txd_next_index, priv->txd_free_index,priv->rxd_index );

	sf_find_desc(priv, &tx_hw_addr);

	writew(0xFF, (void *)NPU_SOFT_CLK_EN);
	writew(0xeF, (void *)NPU_SOFT_RESET);
	writew(0x00, (void *)NPU_SOFT_RESET);


	sf_init_switch_hw();
	print_msg(ETH_RESET_DEBUG,priv->netdev,"end hw init\n");

#ifdef CONFIG_SFAX8_SWITCH_VLAN
	priv->sf_switch_data->set_mac(priv->netdev->dev_addr);
#endif
	/*should set after sf_init_switch_hw,otherwise it will set mac status wrong*/

#ifdef CONFIG_SFAX8_SWITCH_POWERSAVE
	sf_phy_energy_detect_init(priv);
#else
#ifdef SF_TX_SHUTDOWN
	for(i = 0; i < SF_MAC_PORTS; i++){
		if (!(SF_MAC_PORTS_LIST & (1 << i)))
			continue;
		if (priv->shutdown_list & (1 << i)){
			sf_phy_init_test_mode(i, priv, 1);
			sf_phy_init_low_power(priv, i);
		}
	}
#endif
#endif

	print_msg(ETH_RESET_DEBUG,priv->netdev,"start switch reset\n");
	priv->sf_switch_data->switch_reset(priv);

	/* Allocate memory and initialize bd rings */
	napi_enable(&priv->napi_tx);
	napi_enable(&priv->napi_rx);
	sf_dma_start(priv,tx_hw_addr,rx_hw_addr);


	/* Enable Emac Registers */
	sf_mac_enable(SF_MAC_PORTS_LIST);

	netif_start_queue(priv->netdev);
// add schedule to avoid irq miss when reset, should trigger free but not acutually do it
	napi_schedule(&priv->napi_rx);
// trigger tx to process current bd to transfer data
	//sf_writel(CSR_TX_DMA_EN|CSR_TX_BDP_CH_START_WSTB, REG_HIF_TX_CTRL);
	sf_writel(CSR_TX_DMA_EN|CSR_TX_BDP_POLL_CNTR_EN, REG_HIF_TX_CTRL);
	// napi_schedule(&priv->napi_tx);

	/* gating unused eth_byp_ref_clk */
	clkgate = get_module_clk_gate(SF_NPU_SOFT_RESET, false);
	set_module_clk_gate(SF_NPU_SOFT_RESET, clkgate & (~NPU_ETH_BYPREF_CLK), false);
#ifndef CONFIG_SFAX8_PTP
	/* gating unused eth_tsu_clk */
	clkgate = get_module_clk_gate(SF_NPU_SOFT_RESET, false);
	set_module_clk_gate(SF_NPU_SOFT_RESET, clkgate & (~NPU_ETH_TSU_CLK), false);
#endif

	return 0;
}

static int sfax8_eth_reset_thread(void *data) {
	struct sf_priv *priv = data;
	int i = 0, bmu_rem_count = 0;
	unsigned int tx_idx= 0,tx_free_idx = 0,  rx_idx= 0;
        unsigned long tx_bytes = 0, rx_bytes = 0;
#ifdef SF_TX_SHUTDOWN
	unsigned int phy_value = 0, cycle = 0, rand_port = 0, phy_status = 0;
#endif

	print_msg(ETH_RESET_DEBUG,priv->netdev,"Enter npu reset thread\n");
	while (!kthread_should_stop()) {
		rx_bytes = priv->netstats.rx_bytes;
		tx_bytes = priv->netstats.tx_bytes;
		msleep(RESET_SLEEP_TIME);
		rx_speed = (priv->netstats.rx_bytes - rx_bytes)*8/(1024 * 1024 * RESET_SLEEP_TIME/1000);
		tx_speed = (priv->netstats.tx_bytes - tx_bytes)*8/(1024 * 1024 * RESET_SLEEP_TIME/1000);

		tx_idx = priv->txd_next_index;
		tx_free_idx = priv->txd_free_index;
		rx_idx = priv->rxd_index;
		for(i = 0; i < 5; i++){
			bmu_rem_count = sf_readl(REG_BMU_REM_BUF_CNT);
			if(bmu_rem_count > MAX_SKB_FRAGS)
			  break;

			if(i == 4)
			  if((priv->txd_next_index == tx_idx)
				&& (priv->rxd_index == rx_idx)
				&& (priv->txd_free_index == tx_free_idx)
				&& (tx_speed < 1 && rx_speed < 1)){
				  dev_err(priv->device, "bmu error, start npu reset\n");
				  rtnl_lock();
				  sf_eth_reset(priv);
                  eth_reset_count++;
				  rtnl_unlock();
			  }
			msleep(5);
		}

#ifdef SF_SPEED_BANLANCE
		//To make wan-lan two-way speed smooth, wake queue here if host bmu occupied count lower than a threshold value
		if (netif_queue_stopped(priv->netdev) && sf_readl(0x240074) < 93) {
			netif_wake_queue(priv->netdev);
			priv->sfstats.tx_wake_queue++;
			print_msg(ETH_RESET_DEBUG,priv->netdev,"workaround trigger index %d free idx %d stop queue %ld wake queue %ld\n",priv->txd_next_index, priv->txd_free_index, priv->sfstats.tx_wake_queue, priv->sfstats.tx_stop_queue);
		}
#endif

#ifdef SF_TX_SHUTDOWN
		get_random_bytes(&rand_port, sizeof(unsigned int));
                for(i = 0; i < SF_MAC_PORTS; i++){
                        if (!(SF_MAC_PORTS_LIST & (1 << i)))
                                continue;
			rand_port = (rand_port + 1) % 5;
			if(!((1 << rand_port) & priv->shutdown_list))
				continue;

			sf_lock_phy(rand_port);
                        if (!sync_flag){
                                phy_value = read_phy_reg(rand_port, PHY_BASE_CONTROL_REGISTER, priv->phy_dev, 0);
                                phy_status = read_phy_reg(rand_port, OMINI_PHY_MODEL_CTRL_REGISTER, priv->phy_dev, 0);
                                //TODO: energy_on bit is not always right
                                if(!(phy_status & OMINI_PHY_MODEL_CTRL_ENERGY_ON) && (phy_value & PHY_BASE_ENABLE_AUTO_NEG)){
                                        if (remove_auto[rand_port] == 3 ){
                                                phy_value = phy_value & ~PHY_BASE_ENABLE_AUTO_NEG;
                                                write_phy_reg(rand_port, PHY_BASE_CONTROL_REGISTER, phy_value, priv->phy_dev, 0);
                                                remove_auto[rand_port] = 0;
                                        }
                                        else{
                                                remove_auto[rand_port]++;
                                        }
                                }
                                else{
                                        remove_auto[rand_port] = 0;
                                }
                        }
			sf_unlock_phy(rand_port);

			sf_lock_phy(rand_port);
                        if (!sync_flag){
                                phy_value = read_phy_reg(rand_port, PHY_BASE_CONTROL_REGISTER, priv->phy_dev, 0);
                                if(!(phy_value & PHY_BASE_ENABLE_AUTO_NEG)){
                                        if(start_tx_shutdown[rand_port] == 1){
                                                if(cycle == 0){
                                                        write_phy_reg(rand_port, OMINI_PHY_MODEL_CTRL_REGISTER, (phy_status & ~OMINI_PHY_MODEL_CTRL_AUTO) & ~OMINI_PHY_MODEL_CTRL_FIX_MDIX, priv->phy_dev, 0);
                                                        sf_phy_a5_tx_config(rand_port, priv, OMINI_PHY_ANALOG_A5_CONFIG_TX_10M_MDI, 0);
                                                }
                                                else{
                                                        write_phy_reg(rand_port, OMINI_PHY_MODEL_CTRL_REGISTER, (phy_status & ~OMINI_PHY_MODEL_CTRL_AUTO) | OMINI_PHY_MODEL_CTRL_FIX_MDIX, priv->phy_dev, 0);
                                                        sf_phy_a5_tx_config(rand_port, priv, OMINI_PHY_ANALOG_A5_CONFIG_TX_10M_MDIX, 0);
                                                }
                                        }
                                }
                        }
			sf_unlock_phy(rand_port);


			msleep(g_i_tx_wake_time);
			sf_lock_phy(rand_port);
                        if (!sync_flag){
                                phy_value = read_phy_reg(rand_port, PHY_BASE_CONTROL_REGISTER, priv->phy_dev, 0);
                                if(!(phy_value & PHY_BASE_ENABLE_AUTO_NEG)){

                                        if(start_tx_shutdown[rand_port] == 1){
                                                sf_phy_a5_tx_config(rand_port, priv, OMINI_PHY_ANALOG_A5_CONFIG_TX_IDLE, 0);
                                        }
                                        else
                                                start_tx_shutdown[rand_port]++;

                                }
                        }
			sf_unlock_phy(rand_port);
		}
		cycle = (cycle + 1) % 2;
#endif
	}

	return 0;
}

irqreturn_t sf_handle_irq(int irq, void *dev) {
	struct sf_priv *priv = (struct sf_priv*)netdev_priv(dev);
	uint hif_int_src = 0;

	unsigned int int_en = 0;

	priv->sfstats.all_irq++;
	/* reading interrupt soruce register */
	hif_int_src = sf_readl(REG_HIF_INT_SRC);
	/* Interrupt is not ours */
	if (!(hif_int_src)){
		return IRQ_NONE;
	}

	int_en = sf_readl(REG_HIF_INT_EN);

	if ((hif_int_src & HIF_RXPKT_INT) && (int_en & HIF_RXPKT_INT_EN)) {
		int_en &= ~HIF_RXPKT_INT_EN;
		sf_writel(int_en, REG_HIF_INT_EN);
		/* Schedule napi routine */
		napi_schedule(&priv->napi_rx);
		priv->sfstats.rx_irq++;
	}

	// int_en = sf_readl(REG_HIF_INT_EN);
	if(( hif_int_src & HIF_TXPKT_INT) && (int_en & HIF_TXPKT_INT_EN)){
		int_en &= ~HIF_TXPKT_INT_EN;
		sf_writel(int_en, REG_HIF_INT_EN);
		napi_schedule(&priv->napi_tx);
		priv->sfstats.tx_irq++;
	}

	print_msg(ETH_IRQ_DEBUG,priv->netdev,"int src = 0x%x int en 0x%x\n",hif_int_src, int_en);

	return IRQ_HANDLED;
}

#ifdef SF_SPEED_BANLANCE
irqreturn_t sf_bmu_handle_irq(int irq, void *dev) {
	struct sf_priv *priv = (struct sf_priv*)netdev_priv(dev);
	uint bmu_int_src = 0;

	bmu_int_src = sf_readl(REG_BMU_INT_SRC);
	// Interrupt is not ours
	if (!(bmu_int_src)){
		return IRQ_NONE;
	}

	//disable bmu irq
        disable_irq_nosync(priv->bmu_irq);
        atomic_add_unless(&priv->bmu_irq_count, 1, 1);
	//clear bmu int src
	sf_writel(bmu_int_src, REG_BMU_INT_SRC);

	if (bmu_int_src & BMU_EMPTY_INT){
		if (likely(netif_queue_stopped(priv->netdev))) {
			netif_wake_queue(priv->netdev);
			priv->sfstats.tx_wake_queue++;
		}
	}

	return IRQ_HANDLED;
}
#endif

int sf_eth_open(struct net_device *dev) {
	struct sf_priv *priv = netdev_priv(dev);
	int i = 0, rc = 0, clkgate = 0;
        u16 led_on_off_time[2] = {0};
#ifdef CONFIG_SMP
	struct cpumask irq_affi;
	unsigned int cpu = 0;
#endif

        // read led-on-off-time from dts
	rc = of_property_read_u16_array(priv->device->of_node, "led-on-off-time", led_on_off_time, 2);
	if (rc < 0){
                print_msg(ETH_INIT_DEBUG,priv->netdev,"read led-on-off-time form dts fail, ret=%d\n", rc);
                return rc;
	}
        print_msg(ETH_INIT_DEBUG,dev,"read led-on-off-time form dts: 0x%x 0x%x\n", led_on_off_time[0], led_on_off_time[1]);

	writeb(priv->port_list & SF_MAC_PORTS_LIST, (void*)(OMINI_PHY_ENABLE));
	sys_npu_reset();

	/* set ethernet led on/off time, should set after npu system manager reset */
        for(i = 0; i < SF_MAC_PORTS; i++){
                if (!(SF_MAC_PORTS_LIST & (1 << i)))
                        continue;
                if (priv->port_list & (1 << i)){
                        writeb(led_on_off_time[1] & 0xff, (void *)EMAC1_LED_OFF_COUNT_LOW + 0x10 * i);
                        writeb(led_on_off_time[1] >> 8, (void *)EMAC1_LED_OFF_COUNT_LOW + 0x10 * i + 4);
                        writeb(led_on_off_time[0] & 0xff, (void *)EMAC1_LED_OFF_COUNT_LOW + 0x10 * i + 8);
                        writeb(led_on_off_time[0] >> 8, (void *)EMAC1_LED_OFF_COUNT_LOW + 0x10 * i + 12);
                }
        }
	// sf_local_reset(); local reset cannot reset when bmu error happen, if down up interface
	// when bmu error happened ,system hang
	sf_phy_init(priv->phy_dev);
	print_msg(ETH_INIT_DEBUG,dev,"%s phy init done\n", __FUNCTION__);

#ifndef CONFIG_SFAX8_SWITCH_POWERSAVE
#ifdef SF_TX_SHUTDOWN
        for(i = 0; i < SF_MAC_PORTS; i++){
                if (!(SF_MAC_PORTS_LIST & (1 << i)))
                        continue;
		if (priv->shutdown_list & (1 << i)){
			sf_phy_init_test_mode(i, priv, 1);
			sf_phy_init_low_power(priv, i);
			print_msg(ETH_INIT_DEBUG,dev,"%s phy low power port %d\n", __FUNCTION__, i);
		}
	}

#endif
#endif
	sf_init_switch_hw();

#ifdef CONFIG_SFAX8_SWITCH_AGEING
	sf_switch_init_timer(1);
#endif

#ifdef CONFIG_SFAX8_SWITCH_VLAN
	priv->sf_switch_data->set_mac(dev->dev_addr);
#endif
	/*should set after sf_init_switch_hw,otherwise it will set mac status wrong*/
#ifdef CONFIG_SFAX8_SWITCH_POWERSAVE
	sf_phy_energy_detect_init(priv);
#endif

	priv->sf_switch_data->switch_open(priv);

	/* Allocate memory and initialize bd rings */
	rc = sf_bd_ring_init(priv);
	if (rc < 0) {
		dev_err(priv->device, "bd ring init fail\n");
		return rc;
	}

	// for tx header
	dev->needed_headroom = 32;
	rc = devm_request_irq(priv->device, dev->irq, sf_handle_irq, 0,
				dev_name(priv->device), dev);
	if (rc)
	  return rc;

#ifdef SF_SPEED_BANLANCE
	/*request bmu irq*/
	rc = devm_request_irq(priv->device, priv->bmu_irq, sf_bmu_handle_irq, 0,
				dev_name(priv->device), dev);
	if (rc){
      printk("devm_request_irq---bmu_irq=%d rc=%d\n",priv->bmu_irq, rc);
	  return rc;
    }
#endif

#ifdef CONFIG_SMP
	if (of_property_read_u32(priv->device->of_node, "smp-affinity", &cpu) == 0) {
		if (cpu < NR_CPUS) {
			memset(&irq_affi, 0, sizeof(struct cpumask));
			cpumask_set_cpu(cpu, &irq_affi);
			if (irq_set_affinity_hint(dev->irq, &irq_affi))
				netdev_err(priv->netdev, "can not set the affinity for irq : %d\n", dev->irq);
			if (irq_set_affinity_hint(priv->bmu_irq, &irq_affi))
				netdev_err(priv->netdev, "can not set the affinity for irq : %d\n", priv->bmu_irq);
		}
	}
#endif

    sf_writel((BMU_INT_EN|BMU_EMPTY_INT_EN), REG_BMU_INT_ENABLE);
	sf_dma_start(priv, priv->tx_bd_tbl_pa, priv->rx_bd_tbl_pa);

	napi_enable(&priv->napi_tx);
	napi_enable(&priv->napi_rx);

	/* Enable Emac Registers */
	sf_mac_enable(SF_MAC_PORTS_LIST);

	/* Enable bmu irq*/
	//sf_writel((BMU_INT_EN|BMU_FULL_INT_EN), REG_BMU_INT_ENABLE);
	netif_start_queue(dev);
	/*enable bmu thread*/
	priv->bmu_kthread = kthread_run(sfax8_eth_reset_thread, priv, "%s", priv->netdev->name);
	if (IS_ERR(priv->bmu_kthread)){
		netdev_err(priv->netdev,"Could not allocate kthread for bmu\n");
		return PTR_ERR(priv->bmu_kthread);
	}

	/* gating unused eth_byp_ref_clk */
	clkgate = get_module_clk_gate(SF_NPU_SOFT_RESET, false);
	set_module_clk_gate(SF_NPU_SOFT_RESET, clkgate & (~NPU_ETH_BYPREF_CLK), false);
#ifndef CONFIG_SFAX8_PTP
	/* gating unused eth_tsu_clk */
	clkgate = get_module_clk_gate(SF_NPU_SOFT_RESET, false);
	set_module_clk_gate(SF_NPU_SOFT_RESET, clkgate & (~NPU_ETH_TSU_CLK), false);
#endif

	return 0;
}


static int sf_eth_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd) {
	return 0;
}

/**
 *  sf_eth_change_mtu - entry point to change MTU size for the device.
 *  @dev : device pointer.
 *  @new_mtu : the new MTU size for the device.
 *  Description: the Maximum Transfer Unit (MTU) doesn't include ethernet header
 *  and FCS, so the MIN=(64 - 14 - 4), MAX=(9018 - 14 - 4)
 */
static int sf_eth_change_mtu(struct net_device *dev, int new_mtu) {
	struct sf_priv *priv = netdev_priv(dev);
	int old_mtu;

	if (new_mtu < MIN_FRAME_SIZE || new_mtu > MAX_FRAME_SIZE){
		netdev_err(priv->netdev, "invalid MTU, max MTU is: %d\n", MAX_FRAME_SIZE);
		return -EINVAL;
	}

	old_mtu = dev->mtu;

	/* return early if the buffer sizes will not change */
	if (old_mtu == new_mtu)
		return 0;

	dev->mtu = new_mtu;
	//for checksum update
	sf_writel(new_mtu,	REG_CLASS_TCP_OFFLOAD_PKTLEN_THRESH);
	/* if device has been brouht down, return here */
	if (unlikely(!netif_running(dev)))
		return 0;

	/* if device up, reload the device */
	sf_eth_reset(priv);
    eth_reset_count++;

	return 0;
}

static struct net_device_stats * sf_eth_get_stats(struct net_device *dev) {
	struct sf_priv *priv = netdev_priv(dev);
	return &priv->netstats;
}

static int sf_set_mac_address(struct net_device *dev, void *p) {
	int ret = eth_mac_addr(dev, p);
	if(!ret){
#ifdef CONFIG_SFAX8_SWITCH_VLAN
		struct sf_priv *priv = netdev_priv(dev);
		priv->sf_switch_data->set_mac(dev->dev_addr);
#endif
	}

	return ret;
}

#ifdef CONFIG_DEBUG_FS
static struct file_operations npu_debug_ops = {
	.owner = THIS_MODULE,
	.open  = sfax8_npu_debug_open,
	.read  = sfax8_npu_debug_read,
	.write  = sfax8_npu_debug_write,
	.llseek  = default_llseek,
};

static struct file_operations npu_counts_ops = {
	.owner = THIS_MODULE,
	.open  = sfax8_npu_counts_open,
	.read  = sfax8_npu_counts_read,
	.write  = sfax8_npu_counts_write,
	.llseek  = default_llseek,
};
#endif

struct net_device_ops sf_netdev_ops = {
	.ndo_start_xmit		= sf_eth_xmit,
	.ndo_open			= sf_eth_open,
	.ndo_stop			= sf_eth_stop,
	.ndo_do_ioctl		= sf_eth_ioctl,
	.ndo_get_stats		= sf_eth_get_stats,
	.ndo_set_mac_address   = sf_set_mac_address,
	//TODO: add change mtu
	.ndo_change_mtu		= sf_eth_change_mtu,
};

// return > 1 means free
int get_free_space(struct sf_priv *priv){
	// smp_rmb();
	barrier();
	return  NUM_TX_DESCR - ((priv->txd_next_index - priv->txd_free_index) & (NUM_TX_DESCR -1));
}

int calc_free_space(struct sf_priv *priv, unsigned int free_offset){
	// smp_rmb();
	barrier();
	return  NUM_TX_DESCR - ((priv->txd_next_index - priv->txd_free_index - free_offset) & (NUM_TX_DESCR -1));
}

int sf_add_tx_header(struct sf_priv *priv, struct sk_buff **ppskb) {
	struct tx_header *txhdr;
	struct sk_buff * new_skb;

	if (skb_headroom(*ppskb) < SF_TX_HDR_LEN) {
		new_skb = skb_realloc_headroom(*ppskb, SF_TX_HDR_LEN);
		if(new_skb == NULL){
			return -1;
		}else{
			dev_kfree_skb(*ppskb);
			*ppskb = new_skb;
		}
	}

	skb_push(*ppskb, SF_TX_HDR_LEN);
	txhdr = (struct tx_header *)((*ppskb)->data);
	memset(txhdr, 0, SF_TX_HDR_LEN);

	if (((*ppskb)->cb[CB_INJ_TX_FLAG] == CB_TX_SA_MISS) && ((1 << (*ppskb)->cb[CB_INJ_TX_PORT]) & priv->port_list)) {

		txhdr->ctrl = SF_TX_PKT_INJECT_EN;
		txhdr->txport_map = 1 << (*ppskb)->cb[CB_INJ_TX_PORT];
	}

	txhdr->rsvd0 = 0;
	txhdr->queue = 0;
	txhdr->rsvd1 = 0;

	return 0;
}

int sf_txbd_update(struct sk_buff *skb, struct sf_priv *priv, int nfrags)
{
	struct bd *txd = priv->tx_bd_tbl + priv->txd_next_index;
	dma_addr_t dma_handle1 = 0;
	unsigned int len;

	len = skb_headlen(skb);
	dma_handle1 = dma_map_single(priv->device, skb->data,
				len, DMA_TO_DEVICE);
	txd->bd_bufptr = dma_handle1;

	// if((priv->txd_next_index + 1) % 8 == 0){
	if (nfrags)
		txd->bd_ctrl  = (BD_CTRL_PKT_INT_EN |
				BD_CTRL_DESC_EN |
				len);
	else {
		txd->bd_ctrl  = (BD_CTRL_PKT_INT_EN |
				BD_CTRL_LIFM | BD_CTRL_DESC_EN |
				len);
	}
	// }else {
	  // txd->bd_ctrl  = (BD_CTRL_LIFM | BD_CTRL_DESC_EN |
				  // skb->len);
	// }

	if (!nfrags)
		priv->p_tx_skb[priv->txd_next_index] = skb;
	// strobe mode , write reg when packet is ready, not used when in poll mode
	// for reduce ddr use, use strobe mode
	// sf_writel(CSR_TX_DMA_EN|CSR_TX_BDP_CH_START_WSTB, REG_HIF_TX_CTRL);

	priv->txd_next_index = (priv->txd_next_index + 1) & (NUM_TX_DESCR -1);
	/* Advance the write pointer */
	return 0;
}



int sf_eth_xmit(struct sk_buff *skb, struct net_device *dev) {
	struct sf_priv *priv;
	int nfrags = skb_shinfo(skb)->nr_frags;
	unsigned int ctrl_word, len, i;
	void *addr;
	dma_addr_t paddr;
#ifdef SF_SPEED_BANLANCE
	int count = 0;
#endif
	priv  = netdev_priv(dev);

	// add for npu check vlan packet len > 32, otherwise drop the packet
	if (unlikely(skb->len < 40)) {
		int ret = 0;
		if ((ret = skb_pad(skb, 40 - skb->len)) < 0){
			dev_err(priv->device,"padding fail %d \n",ret);
		}
		skb->len = 40;
		skb_set_tail_pointer(skb, 40);
	}

	if (unlikely(get_free_space(priv) <=  (nfrags + 1))) {
		// smp_wmb();
		netif_stop_queue(dev);
		priv->sfstats.tx_stop_queue++;
        tx_q_stop_count++;
#ifdef SF_SPEED_BANLANCE
//		print_msg(ETH_INIT_DEBUG,priv->netdev,"queue stop index %d free idx %d stop queue %ld wake queue %ld\n",priv->txd_next_index, priv->txd_free_index, priv->sfstats.tx_wake_queue, priv->sfstats.tx_stop_queue);
#endif
		return NETDEV_TX_BUSY;
	}

#ifdef SF_SPEED_BANLANCE
	// limit host bmu count to control Host tx
	count = sf_readl(0x240074);
	if (count > BMU_LIMIT_COUNT && (rx_speed > 30 || tx_speed > 94)){
		netif_stop_queue(dev);
		priv->sfstats.tx_stop_queue++;
        tx_q_stop_count++;
		if (atomic_add_unless(&priv->bmu_irq_count, -1, 0))
            enable_irq(priv->bmu_irq);
	}
#endif

	/* Prepend the tx header to transmit packet */
	if (unlikely(sf_add_tx_header(priv, &skb) < 0)) {
		dev_err(priv->device,"add tx headr fail\n");
		dev_kfree_skb(skb);
		return NETDEV_TX_OK;
	}

	sf_txbd_update(skb, priv, nfrags);
	priv->netstats.tx_packets++;
	priv->netstats.tx_bytes += skb->len;

	for (i = 0; i < nfrags; i++) {
		struct bd *txd = priv->tx_bd_tbl + priv->txd_next_index;
		skb_frag_t *frag = &skb_shinfo(skb)->frags[i];
		len = skb_frag_size(frag);
		addr = skb_frag_address(frag);
		paddr = dma_map_single(priv->device, addr, len, DMA_TO_DEVICE);

		txd->bd_bufptr = paddr;
		if (i < (nfrags -1)) {
			ctrl_word  = (BD_CTRL_PKT_INT_EN | BD_CTRL_DESC_EN | len);
			priv->p_tx_skb[priv->txd_next_index] = NULL;
		} else {
			ctrl_word  = (BD_CTRL_PKT_INT_EN | BD_CTRL_LIFM |
						len | BD_CTRL_DESC_EN);
			priv->p_tx_skb[priv->txd_next_index] = skb;
		}

		txd->bd_ctrl = ctrl_word;
		priv->txd_next_index = (priv->txd_next_index + 1) & (NUM_TX_DESCR -1);
	}

	return NETDEV_TX_OK;
}

int sf_rx_handler(struct sk_buff *skb, struct sf_priv *priv) {
	struct rx_header *rxhdr;
	struct rx_header t_rxhdr;
#ifdef CONFIG_SFAX8_PTP
	u64 ns = 0;
#endif
	rxhdr = &t_rxhdr;
	memcpy(rxhdr,  skb->data,sizeof(struct rx_header) );
	/* incoming packet is rx packet */
	// data would change in inject_pkt effect by skb clone maybe
	// rxhdr = (struct rx_header *)skb->data;

#ifdef CONFIG_SFAX8_PTP
	if (rxhdr->ctrl & (1 << 4)) {

		struct egress_report *egrhdr;
		/* incoming packet is egress time stamp report */
		egrhdr = (struct egress_report *)skb->data;

		if (egrhdr->rxport_num == 0)
		  dev = fp_phy0_addr;
		else if (egrhdr->rxport_num == 1)
		  dev = fp_phy1_addr;

		else {

			print_msg(ETH_RX_DEBUG,priv->netdev,
						"fp_process_rx_header device unknown\n");
			dev_kfree_skb(skb);
			return -1;
		}

		ns = (egrhdr->egress_timestamp_sec * 1000000000ULL) +
			(egrhdr->egress_timestamp_nsec);
		nfp_priv = NFP_DEV_INFO(dev);

		if (nfp_priv->ref_num != egrhdr->ref_num) {

			print_msg(ETH_RX_DEBUG,priv->netdev,
						"fp_process_rx_header: ref_num not matching\n");
			dev_kfree_skb(skb);
			return -1;
		}

		nfp_priv->ns = ns;
		npu_ptp_tx_handler(dev);

		dev_kfree_skb(skb);
		return 0;
	}
#endif

	skb_pull(skb, sizeof(struct rx_header));

	// push vlan tag into packet
	if (rxhdr->ctrl & SF_PUNT_VALID) {
#ifdef CONFIG_SFAX8_SWITCH_VLAN
		u16 vlanid = DEF_PORT_FALLBACK_BD_ID;
		u32 entry = vlan_hash_table_search(vlanid) & FWD_PORT_LIST_MASK;
		if(__vlan_get_tag(skb, &vlanid) && entry != priv->port_list) {
			vlanid = priv->sfax8_switch->port[rxhdr->rxport_num].fallback_bd_id;
			vlan_insert_tag(skb, htons(ETH_P_8021Q), vlanid);
		}
#endif

		if(sf_process_packet(skb->data, skb->len, rxhdr, priv) != 1){
			if (rxhdr->punt_reason == SF_PUNT_SA_MISS) {
				sf_tx_inject_packet(skb, rxhdr->rxport_num, priv);
			}

			if (rxhdr->punt_reason == SF_PUNT_IGMP) {
				sf_tx_inject_packet(skb, rxhdr->rxport_num, priv);
			}
		}

		if(rxhdr->rxport_num == SF_HOST_PORT){
			dev_kfree_skb(skb);
			return 0;
		}
	}
	skb->protocol = eth_type_trans(skb, skb->dev);
#ifdef CONFIG_SFAX8_PTP
	if (unlikely(ntohs(skb->protocol) == 0x88F7)) {
		ns = (rxhdr->rx_timestamp_sec * 1000000000ULL) +
			(rxhdr->rx_timestamp_nsec);
		npu_ptp_rx_pktstamp(skb, ns);
		skb->dev = dev;
	}
#endif /* CONFIG_SFAX8_SWITCH_PTP */
	// print_msg(ETH_RX_DEBUG,priv->netdev,"send skb to kernel len %d\n",skb->len);
	// netif_receive_skb(skb);
	// check sum need to enhance
	skb->ip_summed = CHECKSUM_UNNECESSARY;
	napi_gro_receive(&priv->napi_rx, skb);

	return 0;
}

int sf_eth_napi_poll_rx(struct napi_struct *napi, int budget) {
	struct sf_priv *priv = container_of(napi, struct sf_priv, napi_rx);
	int bufsz = priv->netdev->mtu + ETH_HLEN + ETH_FCS_LEN + SF_RX_HDR_LEN;
	unsigned int align_bufsz = ALIGN(bufsz, 8);
	unsigned int len = 0;
	struct bd *rxd;
	struct bd *p_rxd;
	struct sk_buff *new_skb = NULL;
	struct sk_buff *skb = NULL;
	dma_addr_t dma_handle1 = 0;
	//unsigned int bytes_count = -1;
	unsigned int rx_done = 0;
	unsigned int skb_idx = 0;
	unsigned int int_en = 0;

	do {
		/* Read the rx bd ring descriptors */
		skb_idx = (rx_done+ priv->rxd_index) & (NUM_RX_DESCR -1);
		rxd = priv->rx_bd_tbl + skb_idx;
		p_rxd = priv->rx_bd_tbl + ((skb_idx == 0) ? (NUM_RX_DESCR -1) : (skb_idx - 1));
		/* No more descriptors to process */
		if (rxd->bd_ctrl & BD_CTRL_DESC_EN) {
			break;
		}
		skb = priv->p_rx_skb[skb_idx];
		dma_unmap_single(&priv->netdev->dev, rxd->bd_bufptr,
					align_bufsz, DMA_FROM_DEVICE);
		len = rxd->bd_ctrl & BD_CTRL_BD_BUFLEN_MASK;
		new_skb = netdev_alloc_skb_ip_align(priv->netdev, align_bufsz + EXTER_HEADROOM);
		skb_reserve(new_skb, EXTER_HEADROOM);
		dma_handle1 = dma_map_single(&priv->netdev->dev, new_skb->data,
					align_bufsz, DMA_FROM_DEVICE);
		rxd->bd_status = 0;
		rxd->bd_bufptr = dma_handle1;
		priv->p_rx_skb[skb_idx] = new_skb;
		rxd->bd_ctrl =  0;
		rx_done++;
		/* Get the virtual address of the frame */
		skb_put(skb, len);
		skb->dev = priv->netdev;
		sf_rx_handler(skb, priv);
		/* hand over the sk_buff to corresponding fp_phy interface */
		priv->netstats.rx_packets++;
		priv->netstats.rx_bytes += skb->len;
		/* Enable per packet receive interrupt */
		p_rxd->bd_ctrl |= align_bufsz | (BD_CTRL_PKT_INT_EN |BD_CTRL_LIFM |
					BD_CTRL_DIR | BD_CTRL_DESC_EN);
	} while (rx_done< budget);
	priv->rxd_index = (priv->rxd_index + rx_done) & (NUM_RX_DESCR - 1);
	if(rx_done < budget)
	  sf_writel(HIF_RXPKT_INT, REG_HIF_INT_SRC);

	if(rx_done < budget){
		napi_complete(napi);
		int_en = sf_readl(REG_HIF_INT_EN);
		int_en |= HIF_RXPKT_INT_EN;
		sf_writel(int_en, REG_HIF_INT_EN);
	}
	return rx_done;
}

int sf_eth_napi_poll_tx(struct napi_struct *napi, int budget) {
	struct sf_priv *priv = container_of(napi, struct sf_priv, napi_tx);
	struct bd *txd;
	unsigned int tx_done= 0;
	unsigned int tx_again= 0;
	unsigned int int_en = 0;

	do {
		unsigned int skb_idx = (tx_done+ priv->txd_free_index) & (NUM_TX_DESCR -1);
		txd = priv->tx_bd_tbl + skb_idx;
		// smp_mb();
		if((txd->bd_ctrl & BD_CTRL_DESC_EN) || (calc_free_space(priv, tx_done) == NUM_TX_DESCR )){
			break;
		}
		//if(unlikely(priv->p_tx_skb[skb_idx] == NULL))
		  //print_msg(ETH_INIT_DEBUG,priv->netdev,"txd_next_index %d free idx %d tx_done is %d  bd ctrl %0x\n",priv->txd_next_index, priv->txd_free_index, tx_done ,txd->bd_ctrl);
		//else{
		if(priv->p_tx_skb[skb_idx] != NULL) {
				dma_unmap_single(&priv->netdev->dev, txd->bd_bufptr,
						priv->p_tx_skb[skb_idx]->len, DMA_TO_DEVICE);
				dev_kfree_skb_any(priv->p_tx_skb[skb_idx]);
				priv->p_tx_skb[skb_idx] = NULL;
		}
		//}
		tx_done++;
	}while(tx_done < budget);

	priv->txd_free_index = (priv->txd_free_index + tx_done) & (NUM_TX_DESCR -1);

	txd = priv->tx_bd_tbl + priv->txd_free_index;
	if((txd->bd_ctrl & BD_CTRL_DESC_EN) || (calc_free_space(priv, 0) == NUM_TX_DESCR ))
	  sf_writel(HIF_TXPKT_INT, REG_HIF_INT_SRC);
	else
	  tx_again = 1;


	if(tx_done){
		// netdev_completed_queue(priv->netdev, tx_done, bytes_count);
		smp_mb();
		if (unlikely(netif_queue_stopped(priv->netdev) && get_free_space(priv) > MAX_SKB_FRAGS)) {
			netif_wake_queue(priv->netdev);
			priv->sfstats.tx_wake_queue++;
		}
	}

	if( !tx_again ){
		napi_complete(napi);
		int_en = sf_readl(REG_HIF_INT_EN);
		int_en |= HIF_TXPKT_INT_EN;
		sf_writel(int_en, REG_HIF_INT_EN);
		// if tx done == NAPI poll weight and we stop queue here, cause free napi from list twice RM1803
		return 0;
	}else
	  return budget;

}

const struct of_device_id of_sf_match[] = {
	{ .compatible = "siflower,sfax8-eth", .data = &sf_switch_data},
	{},
};

int sf_eth_clk_setup(struct device *dev, struct clk *sf_clk, int index) {
	int ret = 0;

	sf_clk = of_clk_get(dev->of_node, index);
	if (IS_ERR(sf_clk)){
		dev_err(dev, "unable to get sf eth clock\n");
		return -1;
	}

	ret = clk_prepare_enable(sf_clk);
	if (ret)
	  dev_err(dev, "unable to enable eth clock\n");

	return ret;
}

static int sf_get_iram_addr(struct device_node *np, int * addr, int* length){
	struct device_node *iram_np = NULL;
	int size ;
	const __be32 *list;
	phandle phandle;

	*addr = -1;
	*length = -1;
	list = of_get_property(np, "iram-addr", &size);
	if (!list)
		return -1;

	phandle = be32_to_cpup(list);
	if (phandle)
		iram_np = of_find_node_by_phandle(phandle);

	if (!iram_np)
		return -1;

	list = of_get_property(iram_np, "addr", &size);
	if (!list || (size != (2 * sizeof(*list))))
		return -1;

	*addr = be32_to_cpup(list++);
	*length = be32_to_cpup(list);

	return 0;
}

static int set_sf_address(char * mac) {
	//10:16:88
	mac[0] = 0x10;
	mac[1] = 0x16;
	mac[2] = 0x88;
	return 0;
}

static int sf_eth_probe(struct platform_device *pdev) {
	struct sf_priv *priv;
	struct net_device *netdev = NULL;
	//char macaddr[6]={0x00,0x11,0x22,0x33,0x44,0x77};
	int rc = 0, i = 0, err = 0;
	const char *shutdown_portlist = NULL;
	struct resource *res = NULL;
	const struct of_device_id *match;

	print_msg(ETH_INIT_DEBUG,netdev,"ps_probe start\n");
	// device_reset(&pdev->dev);
	netdev = alloc_etherdev(sizeof(*priv));
	if (!netdev) {
		dev_err(&pdev->dev, "alloc_etherdev failed\n");
		return -ENOMEM;
	}
	SET_NETDEV_DEV(netdev, &pdev->dev);
	ether_setup(netdev);
	netdev->flags &= ~IFF_UP;
	/* mask this function for flash optimize */
	//netdev->ethtool_ops = &sf_ethtool_ops;
	netdev->netdev_ops   = &sf_netdev_ops;
	netdev->mtu = ETH_DATA_LEN;
	netdev->max_mtu = MAX_FRAME_SIZE;
	//mac enable checksum offload
	netdev->hw_features =  NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM| NETIF_F_RXCSUM| NETIF_F_SG;
	netdev->features |= netdev->hw_features;
	/* Mac address for HIF device */
	//memcpy(netdev->dev_addr, macaddr, sizeof(macaddr));

	priv = (struct sf_priv *)netdev_priv(netdev);
	g_sf_priv = priv;
	priv->netdev = netdev;
	priv->device = &pdev->dev;

	/* enable bus1_clk */
#ifndef CONFIG_SFAX8_SWITCH_FPGA
	rc = sf_eth_clk_setup(priv->device, priv->eth_bus_clk, 3);
	if (rc)
	  goto err_free_dev;

	/* enable eth_ref_clk, supply for ephy */
	rc = sf_eth_clk_setup(priv->device, priv->eth_ref_clk, 0);
	if (rc)
	  goto err_bus_clk;

	/* enable eth_byp_ref_clk, supply for mac gmii_refclk, not used */
	rc = sf_eth_clk_setup(priv->device, priv->eth_byp_ref_clk, 1);
	if (rc)
	  goto err_ref_clk;

	/* enable eth_tsu_clk, supply for timestamp unit, not used */
	rc = sf_eth_clk_setup(priv->device, priv->eth_tsu_clk, 2);
	if (rc)
	  goto err_byp_ref_clk;

#if defined(CONFIG_SF16A18_MPW1) || defined(CONFIG_SF16A18_FULL_MASK)
	/* enable eth_tsu_clk, supply for npu processor */
	rc = sf_eth_clk_setup(priv->device, priv->npu_pe_clk, 4);
	if (rc)
	  goto err_tsu_clk;
#endif

#endif
	if(release_reset(SF_NPU_SOFT_RESET)){
		err = -EFAULT;
		goto err_pe_clk;
	}

	release_reset(SF_IRAM_SOFT_RESET);

	priv->msg_enable = netif_msg_init(sf_msg_type, SF_DEFAULT_MSG_ENABLE);

	err = of_property_read_string(priv->device->of_node, "shutdown-portlist", &shutdown_portlist);
	if (err < 0){
		print_msg(ETH_INIT_DEBUG,netdev,"read shutdown-portlist form dts fail, ret=%d\n", err);
		goto err_pe_clk;
	}

        for(i = 0; i < SF_MAC_PORTS; i++){
                if (!(SF_MAC_PORTS_LIST & (1 << i)))
                        continue;
		if(shutdown_portlist[i] != '*')
			priv->shutdown_list += 1 << i;
	}
	print_msg(ETH_INIT_DEBUG,netdev,"read shutdown-portlist form dts: 0x%lx\n", priv->shutdown_list);

	match = of_match_device(of_sf_match, &pdev->dev);
	priv->sf_switch_data = (struct sf_switch_data_t *)match->data;

#if (defined CONFIG_SFAX8_FACTORY_READ) && (!defined CONFIG_SFAX8_SWITCH_FPGA)
	if(sf_get_value_from_factory(READ_MAC_ADDRESS, netdev->dev_addr, 6)){
#endif
	if(eth_platform_get_mac_address(priv->device, netdev->dev_addr) < 0){
		eth_hw_addr_random(netdev);
		set_sf_address(netdev->dev_addr);
	}
#if (defined CONFIG_SFAX8_FACTORY_READ) && (!defined CONFIG_SFAX8_SWITCH_FPGA)
	}
#endif
	if(sf_get_iram_addr(priv->device->of_node,&priv->iram_addr,  &priv->iram_len) < 0)
		printk("get iram fail\n");

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	sf_base = devm_ioremap_resource(&pdev->dev, res);
	// sf_base = NPU_ADDR_BASE;
	if (!sf_base) {
		err = -ENOMEM;
		goto  err_pe_clk;
	}
	netdev->base_addr = (int)sf_base;
	print_msg(ETH_INIT_DEBUG,netdev,"netdev->base_addr:%x\n", (uint)sf_base);

	netdev->irq = platform_get_irq(pdev, 0);
	if (netdev->irq < 0) {
		dev_err(&pdev->dev, "no IRQ resource found\n");
		err = -ENXIO;
		goto err_iounmap;
	}
    priv->bmu_irq = platform_get_irq(pdev, 1);
	print_msg(ETH_INIT_DEBUG,netdev,"netdev->irq:%d bmu_irq:%d\n", netdev->irq, priv->bmu_irq);

	netif_napi_add(netdev, &priv->napi_tx, sf_eth_napi_poll_tx, NAPI_POLL_WEIGHT);
	netif_napi_add(netdev, &priv->napi_rx, sf_eth_napi_poll_rx, NAPI_POLL_WEIGHT);

	//TODO  is it the name shows in system
	//strcpy(netdev->name, "fp%d");

	rc = register_netdev(netdev);
	if (rc) {
		goto err_napi;
	}

	// spin_lock_init(&priv->lock);
	platform_set_drvdata(pdev, netdev);
	//register switch device

	priv->sf_switch_data->switch_probe(priv);
#ifdef CONFIG_SFAX8_SWITCH_DPS
	/*detect port status code init*/
	sf_genl_family_init(&(priv->dps_family), "DPS_NL", "updown");
	printk("sf_genl_family_init success,get family id %d!\n",priv->dps_family.family.id);
#endif

#ifdef CONFIG_DEBUG_FS
	priv->npu_debug = debugfs_create_file("npu_debug", 0777, NULL, NULL, &npu_debug_ops);
	priv->npu_counts = debugfs_create_file("npu_counts", 0777, NULL, NULL, &npu_counts_ops);
#endif
	print_msg(ETH_INIT_DEBUG,netdev,"sf eth probe success\n");
	return 0;

err_napi:
	netif_napi_del(&priv->napi_tx);
	netif_napi_del(&priv->napi_rx);
err_iounmap:
	devm_iounmap(&pdev->dev, sf_base);
err_pe_clk:
#ifndef CONFIG_SFAX8_SWITCH_FPGA
#if defined(CONFIG_SF16A18_MPW1) || defined(CONFIG_SF16A18_FULL_MASK)
	clk_disable_unprepare(priv->npu_pe_clk);
#endif
err_tsu_clk:
	clk_disable_unprepare(priv->eth_tsu_clk);
err_byp_ref_clk:
	clk_disable_unprepare(priv->eth_byp_ref_clk);
err_ref_clk:
	clk_disable_unprepare(priv->eth_ref_clk);
err_bus_clk:
	clk_disable_unprepare(priv->eth_bus_clk);
err_free_dev:
#endif
	free_netdev(netdev);

	print_msg(ETH_INIT_DEBUG,netdev,"ps_probe end err\n");
	return err;
}

int sf_eth_stop(struct net_device *dev) {
	struct sf_priv *priv = netdev_priv(dev);
	int count = 0, i = 0;
// close phy irq
	sfax8_switch_disable_int(priv);
#ifdef CONFIG_SFAX8_SWITCH_AGEING
	sf_switch_init_timer(0);
#endif
	netif_tx_disable(dev);

	sf_mac_disable_rx(SF_MAC_PORTS_LIST);

	sf_writel(0, REG_HIF_TX_CTRL);

	count = sf_readl(REG_BMU_REM_BUF_CNT);
	while (count != 0x400) {
		mdelay(10);
		count = sf_readl(REG_BMU_REM_BUF_CNT);
		print_msg(ETH_INIT_DEBUG,dev,"bmu remain count %d\n",count);
		if(i == 100){
			break;
		}
		i++;
	}

	napi_disable(&priv->napi_tx);
	napi_disable(&priv->napi_rx);
	sf_writel(0, REG_HIF_RX_CTRL);
	sf_mac_disable_tx(SF_MAC_PORTS_LIST);

	sf_disable_int(priv);

	kthread_stop(priv->bmu_kthread);
	/* Cleanup the bd rings */
	// spin_lock_bh(&priv->lock);
	bd_rx_ring_deinit(priv);
	bd_tx_ring_deinit(priv);
	// spin_unlock_bh(&priv->lock);
#ifdef CONFIG_SMP
	irq_set_affinity_hint(dev->irq, NULL);
#endif
	devm_free_irq(priv->device, dev->irq, dev);
#ifdef SF_SPEED_BANLANCE
#ifdef CONFIG_SMP
	irq_set_affinity_hint(priv->bmu_irq, NULL);
#endif
	devm_free_irq(priv->device, priv->bmu_irq, dev);
#endif

#ifdef CONFIG_SFAX8_SWITCH_VLAN
	if (priv->sfax8_switch)
#endif
		priv->sf_switch_data->switch_stop(priv);
	/* disable all phy so that subclient could refresh ip */
	writeb(0x0, (void*)(OMINI_PHY_ENABLE));

	return 0;
}


static int sf_eth_remove(struct platform_device *pdev) {
	struct sf_priv *priv = NULL;
	struct net_device *netdev = platform_get_drvdata(pdev);
	if (netdev == NULL)
	  return -1;
	priv = netdev_priv(netdev);
	if (priv == NULL)
	  return -1;

	priv->sf_switch_data->switch_remove(priv);
#ifdef CONFIG_SFAX8_SWITCH_DPS
	/* unregister port status change family */
	sf_genl_family_exit(&(priv->dps_family.family));
#endif

	if(netdev->base_addr)
	  devm_iounmap(&pdev->dev, (void *)netdev->base_addr);

	netif_napi_del(&priv->napi_tx);
	netif_napi_del(&priv->napi_rx);
	unregister_netdev(netdev);
#ifdef CONFIG_DEBUG_FS
	debugfs_remove(priv->npu_debug);
#endif

	/*disable npu clk */
#ifndef CONFIG_SFAX8_SWITCH_FPGA
	clk_disable_unprepare(priv->eth_bus_clk);
	clk_disable_unprepare(priv->eth_ref_clk);
	clk_disable_unprepare(priv->eth_byp_ref_clk);
	clk_disable_unprepare(priv->eth_tsu_clk);
#if defined(CONFIG_SF16A18_MPW1) || defined(CONFIG_SF16A18_FULL_MASK)
	clk_disable_unprepare(priv->npu_pe_clk);
#endif
#endif
	g_sf_priv = NULL;
	free_netdev(netdev);
	platform_set_drvdata(pdev, NULL);

	/* emac[5-1]_phy_enable[4:0] disable all phy and hold reset */
	writeb(0x0, (void*)(OMINI_PHY_ENABLE));
	if(hold_reset(SF_NPU_SOFT_RESET))
	  return -EFAULT;

	hold_reset(SF_IRAM_SOFT_RESET);
	print_msg(ETH_INIT_DEBUG,netdev, "clean up eth module");

	return 0;
}


static struct platform_driver sf_eth_driver = {
	.probe = sf_eth_probe,
	.remove = sf_eth_remove,
	.driver = {
		.name = "sf_eth",
		.owner = THIS_MODULE,
		.of_match_table = of_sf_match,
	},
};



static void __exit exit_sf_eth(void) {
	platform_driver_unregister(&sf_eth_driver);
	return;
}

static int __init init_sf_eth(void) {
	int ret = platform_driver_register(&sf_eth_driver);
	return ret;
}

module_init(init_sf_eth);
module_exit(exit_sf_eth);

MODULE_DEVICE_TABLE(of, of_sf_match);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("phoenix shen <phoenix.shen@siflower.com.cn>");
MODULE_DESCRIPTION("Ethernet driver for sfax8");
MODULE_VERSION(SF_VERSION);
