#include <linux/phy.h>
#include <linux/ethtool.h>
#include "sf_eth.h"
#include "sf_eth_addr.h"
#include "sf_common.h"
#include "sf_mdio.h"

spinlock_t phy_lock[5];

int get_mac_cfg_addr(int mac_no)
{
	switch(mac_no){
		case 0: return REG_EMAC_CFG(0);
		case 1: return REG_EMAC_CFG(1);
		case 2: return REG_EMAC_CFG(2);
		case 3: return REG_EMAC_CFG(3);
		case 4: return REG_EMAC_CFG(4);
		default: return REG_EMAC_CFG(0);
	}
	return 0;
}

int get_mac_address_addr(int mac_no, int type)
{
	if (!type)
	  switch(mac_no){
		  case 0: return REG_MAC_ADDRESS_LOW(0);
		  case 1: return REG_MAC_ADDRESS_LOW(1);
		  case 2: return REG_MAC_ADDRESS_LOW(2);
		  case 3: return REG_MAC_ADDRESS_LOW(3);
		  case 4: return REG_MAC_ADDRESS_LOW(4);
		  default: return REG_MAC_ADDRESS_LOW(0);
	  }
	else
	  switch(mac_no){
		  case 0: return REG_MAC_ADDRESS_HIGH(0);
		  case 1: return REG_MAC_ADDRESS_HIGH(1);
		  case 2: return REG_MAC_ADDRESS_HIGH(2);
		  case 3: return REG_MAC_ADDRESS_HIGH(3);
		  case 4: return REG_MAC_ADDRESS_HIGH(4);
		  default: return REG_MAC_ADDRESS_HIGH(0);
	  }

	return 0;
}

static int get_mii_data_addr(int phy_no)
{

	switch(phy_no){

		case 0: return REG_EMAC_GMII_DATA(0);
		case 1: return REG_EMAC_GMII_DATA(1);
		case 2: return REG_EMAC_GMII_DATA(2);
		case 3: return REG_EMAC_GMII_DATA(3);
		case 4: return REG_EMAC_GMII_DATA(4);
		default: return REG_EMAC_GMII_DATA(0);
	}
	return 0;
}

static int get_mii_cmd_addr(int phy_no)
{
	switch(phy_no){
		case 0: return REG_EMAC_GMII_ADDRESS(0);
		case 1: return REG_EMAC_GMII_ADDRESS(1);
		case 2: return REG_EMAC_GMII_ADDRESS(2);
		case 3: return REG_EMAC_GMII_ADDRESS(3);
		case 4: return REG_EMAC_GMII_ADDRESS(4);
		default: return REG_EMAC_GMII_ADDRESS(0);
	}
	return 0;
}

int wait_phy_rw_not_busy(int phy_no) {
	int reg = sf_readl(get_mii_cmd_addr(phy_no));
	// wait for GMII Register GB is not busy
	while(reg & GMII_ADDR_MASK_GB){
		reg = sf_readl(get_mii_cmd_addr(phy_no));
	}
	return reg;
}

//set phy reg with mac register
void write_phy_reg(int phy_no,int phy_reg,int value,struct phy_device* phy_dev, int lock) {

	int addr_value;
	if (lock)
	  sf_lock_phy(phy_no);
	//prepare data for register 5
	addr_value = wait_phy_rw_not_busy(phy_no);
	sf_writel(value & GMII_DATA_MASK,get_mii_data_addr(phy_no));
	//set address flag
	addr_value = (addr_value & (~GMII_ADDR_MASK_GR)) | ((phy_reg << 6) & GMII_ADDR_MASK_GR);
	//set phy addr
	addr_value = (addr_value & (~GMII_ADDR_MASK_PA)) | ((phy_dev[phy_no].mdio.addr << 11) & GMII_ADDR_MASK_PA);
	//set read flag
	addr_value = (addr_value & (~GMII_ADDR_MASK_GW)) | (GMII_ADDR_VALUE_GW_WRITE & GMII_ADDR_MASK_GW);
	//set GB flag to indica is busy now
	addr_value = addr_value | GMII_ADDR_MASK_GB;
	sf_writel(addr_value,get_mii_cmd_addr(phy_no));
	//wait for finish
	wait_phy_rw_not_busy(phy_no);
	if (lock)
	  sf_unlock_phy(phy_no);
	// if( phy_no == 0)
	//   printk("write value is 0x%04x reg is %02d phy_no %d \n",value, phy_reg, phy_no);
}

int read_phy_reg_direct(int phy_no,int phy_addr,int phy_reg, int lock) {

	int ret_value = 0, value;
	if (lock)
	  sf_lock_phy(phy_no);
	//clear the data first
	value = wait_phy_rw_not_busy(phy_no);
	sf_writel(DEFAULT_PHY_VALUE ,get_mii_data_addr(phy_no));
	//set address flag
	value = (value & (~GMII_ADDR_MASK_GR)) | ((phy_reg << 6) & GMII_ADDR_MASK_GR);
	//set phy addr
	value = (value & (~GMII_ADDR_MASK_PA)) | ((phy_addr << 11) & GMII_ADDR_MASK_PA);
	//set read flag
	value = (value & (~GMII_ADDR_MASK_GW)) | (GMII_ADDR_VALUE_GW_READ & GMII_ADDR_MASK_GW);
	//set gb flag to indica is busy now
	value = value | GMII_ADDR_MASK_GB;
	sf_writel(value ,get_mii_cmd_addr(phy_no));
	//wait for complete
	wait_phy_rw_not_busy(phy_no);

	ret_value = sf_readl(get_mii_data_addr(phy_no)) & GMII_DATA_MASK;
	if (lock)
	  sf_unlock_phy(phy_no);
	// if(phy_no == 0)
	//   printk("read  value is 0x%04x reg is %02d phy_no %d \n",ret_value, phy_reg, phy_no);
	return ret_value;
}

//read phy reg with mac register
int read_phy_reg(int phy_no,int phy_reg, struct phy_device* phy_dev, int lock) {
	return read_phy_reg_direct(phy_no,phy_dev[phy_no].mdio.addr,phy_reg,lock);
}

void sf_phy_init(struct phy_device* phy_dev) {
    int i;
    for (i = 0; i < 5; i++)
        spin_lock_init(&phy_lock[i]);

    //scan first to get phy id and addr
    phy_scan(phy_dev);
#ifdef CONFIG_SFAX8_SWITCH_FPGA
	for(i = 0; i < SF_MAC_PORTS; i++){
		if (!(SF_MAC_PORTS_LIST & (1 << i)))
			continue;
		phy_dev[i].duplex = DUPLEX_FULL;
		phy_dev[i].speed = SPEED_100;
		//init marvell phy
		/*Marvell speed Bit 6,13  of Control registers
		  |   10 = 1000 Mbps
		  |   01 = 100 Mbps
		  |   00 = 10 Mbps
		  |   */
		//init marvell phy(almost the same)
		//Change Page Number for copper
		write_phy_reg(i,MIIM_88E1111_PHY_PAGE,0x0000, phy_dev, 1);
		//read first
		int phy_ctrl = read_phy_reg(i,PHY_BASE_CONTROL_REGISTER, phy_dev, 1);

		if(phy_dev[i].duplex == DUPLEX_FULL){

			phy_ctrl |= PHY_BASE_ENABLE_DUPLEX_FULL;
		}else{

			phy_ctrl = phy_ctrl & (~PHY_BASE_ENABLE_DUPLEX_FULL);
		}
		phy_ctrl &= (~PHY_BASE_ENABLE_AUTO_NEG);
		if(phy_dev[i].speed == SPEED_10){

			//set 10 mbps
			phy_ctrl = (phy_ctrl & (~MAVELL_PHY_SPEED_MASK)) | MAVELL_PHY_SPEED_10m;
		}else if(phy_dev[i].speed == SPEED_100){

			phy_ctrl = (phy_ctrl & (~MAVELL_PHY_SPEED_MASK)) | MAVELL_PHY_SPEED_100m;
		}else{

			phy_ctrl = (phy_ctrl & (~MAVELL_PHY_SPEED_MASK)) | PHY_BASE_ENABLE_AUTO_NEG;
		}
		write_phy_reg(i,PHY_BASE_CONTROL_REGISTER,phy_ctrl, phy_dev, 1);
		//reset phy and wait reset flag be cleared
		phy_ctrl = (read_phy_reg(i,PHY_BASE_CONTROL_REGISTER, phy_dev, 1) | PHY_BASE_ENABLE_RESET);
		write_phy_reg(i,PHY_BASE_CONTROL_REGISTER,phy_ctrl, phy_dev, 1);
		while (phy_ctrl & PHY_BASE_ENABLE_RESET) {

			phy_ctrl = read_phy_reg(i,PHY_BASE_CONTROL_REGISTER, phy_dev, 1);
		}
		// enable marvell phy csr signal assert when transmit,by set register 16 bit 11
		// we have to use cable to make sure that the phy is linked.
		// c = Assert on transmit
		// 0 = Never assert on transmit
		int reg = read_phy_reg(i, (0x10), phy_dev, 1) | (0x1 << 11);
		write_phy_reg(i, 0x10, reg, phy_dev, 1);

	}

	//todo: now marvell phy reset very slow,and we have to wait 4 seconds to ensure it works in the right state
	//we will confirm it latter
	msleep(5000);

// #else
// 	for(i = 0; i < SF_MAC_PORTS; i++){
// 		write_phy_reg(i, OMINI_PHY_INTMASK_REGISTER, PHY_INT_ENABLE_ALL_NORMAL, phy_dev, 1);
// 	}
#endif
}

int phy_scan(struct phy_device* phy_dev)
{
	int addr = 0;
	int addr_index = 0;
	int phy_no = 0;
	int phy_id1 = 0;
	int phy_id2 = 0;
	int phy_id = 0;
	int phy_scan_success_count = 0;

	for (phy_no = 0; phy_no < SF_MAC_PORTS; phy_no++) {
		if (!(SF_MAC_PORTS_LIST & (1 << phy_no)))
			continue;
		//scan 0~31 phy id
		for (addr_index = -1; addr_index < 32; addr_index++) {
#ifndef CONFIG_SFAX8_SWITCH_FPGA
			addr = (addr_index == -1) ? (OMINI_PHY1_ADDR + phy_no) : addr_index;
#else
			if(addr_index == -1) continue;
			addr = addr_index;
#endif
			phy_id1 = read_phy_reg_direct(phy_no,addr,PHY_IDENTIFY_1, 1) & 0xffff;
			phy_id2 = read_phy_reg_direct(phy_no,addr,PHY_IDENTIFY_2, 1) & 0xffff;
			phy_id = (phy_id2 | (phy_id1 << 16));
			//If the phy_id is mostly Fs, there is no device there(accord with linux standard phy driver detect code)
			if ((phy_id & 0x1fffffff) != 0x1fffffff){

				//PHY IDENTIFIER1[15:0] -----------PHY_ID[15:0]
				//PHY IDENTIFIER2[15:10]-----------PHY_ID[21:16]
				phy_dev[phy_no].phy_id = phy_id1 | ((phy_id2 << 6) & 0x3F0000);
				phy_dev[phy_no].mdio.addr = addr;
				phy_scan_success_count++;
				break;
			}
		}
	}
	return phy_scan_success_count;
}

void sys_omini_phy_reset(void)
{

	//emac2_cfg_np_msg_code[2:0]
	//emac[5-1]_cfg_power_down[4:0] Default value of True power down bit (Reg24.0)
	writeb(0x0, (void *)OMINI_PHY_CFG_POWERDOWN);
	//emac3_cfg_np_msg_code[2:0]
	//emac[5-1]_cfg_powerup_reset[4:0] Power Up Reset bit(Reg24.1)
	writeb(0x0, (void *)OMINI_PHY_CFG_POWERUP_RESET);
	//emac1_cfg_mode[2:0] emac1_cfg_phy_addr[4:0]
	//emac1_cfg_mode[2:0] 001 - 10Base-T, Full Duplex, Auto negotiation disabled 0x20
	//emac1_cfg_mode[2:0] 111 All capable, Full Duplex, 10 & 100 BT, Auto negotiation enabled AutoMDIX enable --0xE0
	//emac1_cfg_mode[2:0] 011 100Base-TX, Full Duplex, Auto-negotiation disabled --0x60
	writeb(OMINI_PHY1_ADDR | OMINI_PHY_CFG_MODE, (void *)OMINI_PHY1_CFG_ADDR);
	writeb(OMINI_PHY2_ADDR | OMINI_PHY_CFG_MODE, (void *)OMINI_PHY2_CFG_ADDR);
	writeb(OMINI_PHY3_ADDR | OMINI_PHY_CFG_MODE, (void *)OMINI_PHY3_CFG_ADDR);
	writeb(OMINI_PHY4_ADDR | OMINI_PHY_CFG_MODE, (void *)OMINI_PHY4_CFG_ADDR);
	writeb(OMINI_PHY5_ADDR | OMINI_PHY_CFG_MODE, (void *)OMINI_PHY5_CFG_ADDR);
	//OMINI_PHY1_CFG_FX_PLL_MODE
	/*
	   {

	   |   emac2_cfg_automdix_en, 1
	   |   emac1_cfg_en_high,  0
	   |   emac1_cfg_fx_mode,  0
	   |   emac1_cfg_adc_bp,   0            bypass mode. (Reg18.8)
	   |   emac1_cfg_pll_bp,   0            bypass mode (Reg18.9)
	   |   emac1_cfg_smii_source_sync, 0    smii source sync register Only relevant for SMII mode
	   |   emac1_cfg_mii_mode[1:0],  00           “00” for MII mode,"01" for RMII mode,"10" for SMII,"11" reserved
	   }
	   */
	writeb(0x80, (void *)OMINI_PHY1_CFG_FX_PLL_MODE);
	writeb(0x80, (void *)OMINI_PHY2_CFG_FX_PLL_MODE);
	writeb(0x80, (void *)OMINI_PHY3_CFG_FX_PLL_MODE);
	writeb(0x80, (void *)OMINI_PHY4_CFG_FX_PLL_MODE);
	writeb(0x80, (void *)OMINI_PHY5_CFG_FX_PLL_MODE);
	//emac4_cfg_np_msg_code[2:0]
	//emac[5-1]_cfg_clk_freq
	//‘0’ for 25 MHz clock input; ‘1’ for 50 MHz clock input.
	writeb(0x00, (void *)OMINI_PHY_CFG_CLK_FREQ);
	//emac5_cfg_np_msg_code[2:0]
	//emac[5-1]_cfg_ref_clk_sel(NA now)
	writeb(0x00, (void *)OMINI_PHY_CFG_CLK_REF_SEL);

	//	    emac1_cfg_phy_id[7:0]
	writeb(OMINI_PHY_ID_1 & 0xFF, (void *)OMINI_PHY1_CFG_PHY_ID_LOW8);
	//emac1_cfg_phy_id[15:8]
	writeb(((OMINI_PHY_ID_1 & 0xFF00) >> 8), (void *)OMINI_PHY1_CFG_PHY_ID_HIGH8);
	//{emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]}
	writeb(((OMINI_PHY_ID_1 & 0x3F0000) >> 16) | ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6), (void *)OMINI_PHY1_CFG_MODEL_NR);
	//{emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]}
	writeb((OMINI_PHY_REV_NUMBER << 4) | ((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2), (void *)OMINI_PHY1_CFG_REV_NR);


	//emac1_cfg_phy_id[7:0]
	writeb(OMINI_PHY_ID_2 & 0xFF, (void *)OMINI_PHY2_CFG_PHY_ID_LOW8);
	//emac1_cfg_phy_id[15:8]
	writeb(((OMINI_PHY_ID_2 & 0xFF00) >> 8), (void *)OMINI_PHY2_CFG_PHY_ID_HIGH8);
	//{ emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]}
	writeb(((OMINI_PHY_ID_2 & 0x3F0000) >> 16) | ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6), (void *)OMINI_PHY2_CFG_MODEL_NR);
	//{ emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]}
	writeb((OMINI_PHY_REV_NUMBER << 4) | ((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2), (void *)OMINI_PHY2_CFG_REV_NR);


	//emac1_cfg_phy_id[7:0]
	writeb(OMINI_PHY_ID_3 & 0xFF, (void *)OMINI_PHY3_CFG_PHY_ID_LOW8);
	//emac1_cfg_phy_id[15:8]
	writeb(((OMINI_PHY_ID_3 & 0xFF00) >> 8), (void *)OMINI_PHY3_CFG_PHY_ID_HIGH8);
	//{ emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]}
	writeb(((OMINI_PHY_ID_3 & 0x3F0000) >> 16) | ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6), (void *)OMINI_PHY3_CFG_MODEL_NR);
	//{ emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]}
	writeb((OMINI_PHY_REV_NUMBER << 4) | ((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2), (void *)OMINI_PHY3_CFG_REV_NR);

	//emac1_cfg_phy_id[7:0]
	writeb(OMINI_PHY_ID_4 & 0xFF, (void *)OMINI_PHY4_CFG_PHY_ID_LOW8);
	//emac1_cfg_phy_id[15:8]
	writeb(((OMINI_PHY_ID_4 & 0xFF00) >> 8), (void *)OMINI_PHY4_CFG_PHY_ID_HIGH8);
	//{ emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]}
	writeb(((OMINI_PHY_ID_4 & 0x3F0000) >> 16) | ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6), (void *)OMINI_PHY4_CFG_MODEL_NR);
	//{ emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]}
	writeb((OMINI_PHY_REV_NUMBER << 4) | ((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2), (void *)OMINI_PHY4_CFG_REV_NR);

	//emac1_cfg_phy_id[7:0]
	writeb(OMINI_PHY_ID_5 & 0xFF, (void *)OMINI_PHY5_CFG_PHY_ID_LOW8);
	//emac1_cfg_phy_id[15:8]
	writeb(((OMINI_PHY_ID_5 & 0xFF00) >> 8), (void *)OMINI_PHY5_CFG_PHY_ID_HIGH8);
	//{ emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]}
	writeb(((OMINI_PHY_ID_5 & 0x3F0000) >> 16) | ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6), (void *)OMINI_PHY5_CFG_MODEL_NR);
	//{ emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]}
	writeb((OMINI_PHY_REV_NUMBER << 4) | ((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2), (void *)OMINI_PHY5_CFG_REV_NR);
}

void sf_mac_enable(unsigned int port_list)
{
	unsigned int i = 0, mac_cfg = 0;
	for(i = 0 ; i < SF_MAC_PORTS; i++){
		if( port_list & (0x1 << i) ){
			mac_cfg = sf_readl(get_mac_cfg_addr(i));
			sf_writel(mac_cfg | MAC_CFG_TX_EN | MAC_CFG_RX_EN , get_mac_cfg_addr(i));
		}
	}
}

void sf_mac_disable_rx(unsigned int port_list)
{
	unsigned int i = 0, mac_cfg = 0;
	for(i = 0 ; i < SF_MAC_PORTS; i++){
		if( port_list & (0x1 << i) ){
			mac_cfg = sf_readl(get_mac_cfg_addr(i));
			mac_cfg &= ~MAC_CFG_RX_EN;
			sf_writel(mac_cfg, get_mac_cfg_addr(i));
		}
	}
}

void sf_mac_disable_tx(unsigned int port_list)
{
	unsigned int i = 0, mac_cfg = 0;
	for(i = 0 ; i < SF_MAC_PORTS; i++){
		if( port_list & (0x1 << i) ){
			mac_cfg = sf_readl(get_mac_cfg_addr(i));
			mac_cfg &= ~MAC_CFG_TX_EN;
			sf_writel(mac_cfg, get_mac_cfg_addr(i));
		}
	}
}

int check_phy_linkup(int phy_no, struct sf_priv *priv, int lock){
#ifdef CONFIG_SFAX8_SWITCH_FPGA
	//marvell register 17-bit10
	int phy_status = read_phy_reg_direct(phy_no, priv->phy_dev[phy_no].mdio.addr, MIIM_88E1111_PHY_SPL_STATUS_REG, lock);
	if((phy_status & DEFAULT_PHY_VALUE) == 0xffff) return 0;
	return (phy_status & (0x1 << 10)) ? 1 : 0;
#else
	int phy_status = read_phy_reg_direct(phy_no, priv->phy_dev[phy_no].mdio.addr, PHY_BASE_STATUS_REGISTER, lock);
	if((phy_status & DEFAULT_PHY_VALUE) == 0xffff) return 0;
	return (phy_status & PHY_STATUS_LINK_UP) ? 1 : 0;
#endif
}

void sf_phy_re_auto(struct sf_priv *priv , unsigned int port, int lock){
	int phy_value;
	if (lock)
	  sf_lock_phy(port);
	phy_value = read_phy_reg(port, PHY_BASE_CONTROL_REGISTER, priv->phy_dev, 0);
	write_phy_reg(port, PHY_BASE_CONTROL_REGISTER, phy_value | PHY_BASE_ENABLE_RESTART_NEG, priv->phy_dev, 0);
	if (lock)
	  sf_unlock_phy(port);
}

#ifdef SF_TX_SHUTDOWN
int sf_phy_reset(int phy_idx, struct sf_priv *priv){
	unsigned int phy_ctrl;
	phy_ctrl = (read_phy_reg(phy_idx, PHY_BASE_CONTROL_REGISTER, priv->phy_dev, 0) | PHY_BASE_ENABLE_RESET);
	write_phy_reg(phy_idx, PHY_BASE_CONTROL_REGISTER,phy_ctrl, priv->phy_dev, 0);
	while ((phy_ctrl & PHY_BASE_ENABLE_RESET) || (phy_ctrl == 0x0)) {
		phy_ctrl = read_phy_reg(phy_idx, PHY_BASE_CONTROL_REGISTER, priv->phy_dev, 0);
	}
	sf_phy_init_test_mode(phy_idx, priv, 0);
	// writeb(0x1e, OMINI_PHY_ENABLE);
	// writew(0x10, NPU_SOFT_RESET);
	// writew(0x6f, NPU_SOFT_CLK_EN);
	// udelay(10);
	// writew(0x7f, NPU_SOFT_CLK_EN);
	// writew(0x0, NPU_SOFT_RESET);
	// writeb(0x1f, OMINI_PHY_ENABLE);
	// udelay(300);
	return 0;
}
int sf_phy_init_test_mode(int phy_idx, struct sf_priv *priv, int lock){
	unsigned int phy_status;
	if (lock)
	  sf_lock_phy(phy_idx);
	phy_status = read_phy_reg(phy_idx, OMINI_PHY_TSTCNTL_REGISTER, priv->phy_dev, 0);
	if(!(phy_status &  OMINI_PHY_TSTCNTL_ENABLE)){
		write_phy_reg(phy_idx, OMINI_PHY_TSTCNTL_REGISTER, 0x0, priv->phy_dev, 0);
		write_phy_reg(phy_idx, OMINI_PHY_TSTCNTL_REGISTER,OMINI_PHY_TSTCNTL_ENABLE, priv->phy_dev, 0);
		write_phy_reg(phy_idx, OMINI_PHY_TSTCNTL_REGISTER, 0x0, priv->phy_dev, 0);
		write_phy_reg(phy_idx, OMINI_PHY_TSTCNTL_REGISTER,OMINI_PHY_TSTCNTL_ENABLE, priv->phy_dev, 0);
	}
	if (lock)
	  sf_unlock_phy(phy_idx);
	return 0;
}
int sf_phy_write_test_reg(int phy_idx, struct sf_priv *priv, unsigned int reg_addr, unsigned reg_value, int phy_mode, int lock){
	unsigned int phy_value;
	if (lock)
	  sf_lock_phy(phy_idx);
	// sf_phy_init_test_mode(phy_idx, priv, 1);
	phy_value =  OMINI_PHY_TSTCNTL_ENABLE | OMINI_PHY_TSTCNTL_WRITE | reg_addr | phy_mode;
	write_phy_reg(phy_idx, OMINI_PHY_TSTWRITE_REGISTER, reg_value, priv->phy_dev, 0);
	write_phy_reg(phy_idx, OMINI_PHY_TSTCNTL_REGISTER, phy_value, priv->phy_dev, 0);
	if (lock)
	  sf_unlock_phy(phy_idx);
	return 0;
}

int sf_phy_read_test_reg(int phy_idx, struct sf_priv *priv, unsigned int reg_addr, int phy_mode){
	unsigned int phy_value;
	sf_lock_phy(phy_idx);
	// sf_phy_init_test_mode(phy_idx, priv, 1);
	phy_value =  OMINI_PHY_TSTCNTL_ENABLE | OMINI_PHY_TSTCNTL_READ | (reg_addr << 0x5) | phy_mode;
	write_phy_reg(phy_idx, OMINI_PHY_TSTCNTL_REGISTER, phy_value, priv->phy_dev, 0);
	phy_value = read_phy_reg(phy_idx, OMINI_PHY_TSTREAD_REGISTER, priv->phy_dev, 0);
	sf_unlock_phy(phy_idx);

	return phy_value;
}


// mode should be  OMINI_PHY_ANALOG_A5_CONFIG_TX_IDLE or 10M or OFF
int sf_phy_a5_tx_config(int phy_idx, struct sf_priv *priv, unsigned int value, int lock){
	if (lock)
	  sf_phy_write_test_reg(phy_idx, priv, OMINI_PHY_ANALOG_A5_CONFIG_REGISTER,value ,OMINI_PHY_TSTCNTL_MODE_ANA, 1);
	else
	  sf_phy_write_test_reg(phy_idx, priv, OMINI_PHY_ANALOG_A5_CONFIG_REGISTER,value ,OMINI_PHY_TSTCNTL_MODE_ANA, 0);
	return 0;
}

void sf_phy_init_normal(struct sf_priv *priv, unsigned int port){
	unsigned int phy_value = 0;

	sf_lock_phy(port);
	sf_phy_a5_tx_config(port, priv, OMINI_PHY_ANALOG_A5_CONFIG_TX_ON, 0);

	phy_value = read_phy_reg(port, PHY_BASE_CONTROL_REGISTER, priv->phy_dev, 0);
	if(!(phy_value & PHY_BASE_ENABLE_AUTO_NEG))
	  write_phy_reg(port, PHY_BASE_CONTROL_REGISTER, phy_value | PHY_BASE_ENABLE_AUTO_NEG, priv->phy_dev, 0);

	write_phy_reg(port, OMINI_PHY_MODEL_CTRL_REGISTER, phy_value | OMINI_PHY_MODEL_CTRL_AUTO, priv->phy_dev, 0);
	sf_unlock_phy(port);

	return;
}

// init phy with 10m half
void sf_phy_init_low_power(struct sf_priv *priv , unsigned int port){
	int  phy_value = 0, phy_status =0;

	sf_lock_phy(port);

	phy_status = read_phy_reg(port,  PHY_BASE_STATUS_REGISTER, priv->phy_dev, 0);
	if (!(phy_status & PHY_STATUS_LINK_UP)){

		phy_value = read_phy_reg(port, PHY_BASE_CONTROL_REGISTER, priv->phy_dev, 0);
		write_phy_reg(port, PHY_BASE_CONTROL_REGISTER, phy_value & ~PHY_BASE_ENABLE_AUTO_NEG, priv->phy_dev, 0);

		//config phy to test mode
		sf_phy_a5_tx_config(port, priv,OMINI_PHY_ANALOG_A5_CONFIG_TX_IDLE, 0);
	}
	sf_unlock_phy(port);
}


#endif


#ifdef CONFIG_SFAX8_SWITCH_POWERSAVE
void sf_phy_energy_detect_init(struct sf_priv *priv){
	int i = 0, phy_value = 0, phy_status =0;
	for(i = 0; i < SF_POWERSAVE_PORTS; i++){
		phy_status = read_phy_reg(i, PHY_BASE_STATUS_REGISTER, priv->phy_dev, 1);
		if (!(phy_status & PHY_STATUS_LINK_UP)){
			/*disable Auto-negotiation and AutoMDIX while enable energy detect accroding to ominiphy*/
			phy_value = read_phy_reg(i, PHY_BASE_CONTROL_REGISTER, priv->phy_dev, 1);
			if(phy_value & PHY_BASE_ENABLE_AUTO_NEG)
			  write_phy_reg(i, PHY_BASE_CONTROL_REGISTER, phy_value & (~PHY_BASE_ENABLE_AUTO_NEG), priv->phy_dev, 1);

			phy_value = read_phy_reg(i, OMINI_PHY_MODEL_CTRL_REGISTER, priv->phy_dev, 1);
			phy_value = (phy_value & (~OMINI_PHY_MODEL_CTRL_AUTO)) | OMINI_PHY_MODEL_CTRL_ENERGY_DETECT;
			write_phy_reg(i, OMINI_PHY_MODEL_CTRL_REGISTER, phy_value, priv->phy_dev, 1);
		}
	}
}

void sf_phy_energy_detect_enable(int phy_no, struct phy_device* phy_dev){
	/*disable Auto-negotiation and AutoMDIX while enable energy detect accroding to ominiphy*/
	int phy_value = read_phy_reg(phy_no, PHY_BASE_CONTROL_REGISTER, phy_dev, 1);
	if(phy_value & PHY_BASE_ENABLE_AUTO_NEG)
	  write_phy_reg(phy_no, PHY_BASE_CONTROL_REGISTER, phy_value & (~PHY_BASE_ENABLE_AUTO_NEG), phy_dev, 1);

	phy_value = read_phy_reg(phy_no, OMINI_PHY_MODEL_CTRL_REGISTER, phy_dev, 1);
	if (phy_value & OMINI_PHY_MODEL_CTRL_AUTO)
		write_phy_reg(phy_no, OMINI_PHY_MODEL_CTRL_REGISTER, phy_value & (~OMINI_PHY_MODEL_CTRL_AUTO), phy_dev, 1);

	phy_value = read_phy_reg(phy_no, OMINI_PHY_MODEL_CTRL_REGISTER, phy_dev, 1);
	if (!(phy_value & OMINI_PHY_MODEL_CTRL_ENERGY_DETECT))
		write_phy_reg(phy_no, OMINI_PHY_MODEL_CTRL_REGISTER, phy_value | OMINI_PHY_MODEL_CTRL_ENERGY_DETECT, phy_dev, 1);
}

void sf_phy_energy_detect_disable(int phy_no, struct phy_device* phy_dev){
	int phy_value = read_phy_reg(phy_no, OMINI_PHY_MODEL_CTRL_REGISTER, phy_dev, 1);
	if (phy_value & OMINI_PHY_MODEL_CTRL_ENERGY_DETECT)
		write_phy_reg(phy_no, OMINI_PHY_MODEL_CTRL_REGISTER, phy_value & (~OMINI_PHY_MODEL_CTRL_ENERGY_DETECT), phy_dev, 1);
}
#endif

//0 half 1 full
int get_link_duplex(int phy_no, struct sf_priv *priv){
#ifdef CONFIG_SFAX8_SWITCH_FPGA
	//marvell register 17-bit10
	int phy_status = read_phy_reg_direct(phy_no, priv->phy_dev[phy_no].mdio.addr, MIIM_88E1111_PHY_SPL_STATUS_REG, 1);
	return phy_status >> 13 & 0x1 ;
#else
	int phy_status = read_phy_reg_direct(phy_no, priv->phy_dev[phy_no].mdio.addr, OMINI_PHY_SPECIAL_CONTROL_STATUS, 1);
	if((phy_status >> 2  & 0x7) == 0x1 || (phy_status >> 2  & 0x7) == 0x2)
	  return 0;
	else
	  return 1;
#endif
}


// 0 10m 1 100m
int get_link_speed(int phy_no, struct sf_priv *priv){
#ifdef CONFIG_SFAX8_SWITCH_FPGA
	//marvell register 17-bit10
	int phy_status = read_phy_reg_direct(phy_no, priv->phy_dev[phy_no].mdio.addr, MIIM_88E1111_PHY_SPL_STATUS_REG, 1);
	if((phy_status & DEFAULT_PHY_VALUE) == 0xffff) return 0;
	return phy_status >> 14 & 0x3 ;
#else
	int phy_status = read_phy_reg_direct(phy_no, priv->phy_dev[phy_no].mdio.addr, OMINI_PHY_SPECIAL_CONTROL_STATUS, 1);
	if((phy_status >> 2  & 0x7) == 0x1 || (phy_status >> 2  & 0x7) == 0x5)
	  return 0;
	else
	  return 1;
#endif
}

int sf_mac_set_link(int port, int speed, int duplex){
	int mac_cfg = sf_readl(get_mac_cfg_addr(port));
	mac_cfg &= ~MAC_CFG_DUPLEX_FULL;
	mac_cfg &= ~MAC_CFG_DIS_REC_OWN;

	if(duplex == ETH_DUPLEX_FULL)
	  mac_cfg |= MAC_CFG_DUPLEX_FULL;
	if(duplex == ETH_DUPLEX_HALF)
	  mac_cfg |= MAC_CFG_DIS_REC_OWN;
	if(speed == ETH_SPEED_10){
		//for 10m speed
		mac_cfg = (mac_cfg & (~MAC_CFG_FES_100));
	}else{
		//for 100m
		mac_cfg |=  MAC_CFG_FES_100 ;
	}

	sf_writel(mac_cfg,get_mac_cfg_addr(port));
	return 0;
}
