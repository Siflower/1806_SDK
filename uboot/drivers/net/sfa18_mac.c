#include "sfa18_mac.h"
#include <asm/io.h>
static int get_mac_cfg_addr(int mac_no)
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
}

void sf_mac_enable(void) {
	int i = 0;
	for(i = 0 ; i < SF_MAC_PORTS; i++){
		int mac_cfg = sf_readl(get_mac_cfg_addr(i));
		sf_writel(mac_cfg | MAC_CFG_TX_EN | MAC_CFG_RX_EN , get_mac_cfg_addr(i));
	}
}

void sf_mac_disable_rx(void) {
	int i = 0;
	for(i = 0 ; i < SF_MAC_PORTS; i++){
		int mac_cfg = sf_readl(get_mac_cfg_addr(i));
		mac_cfg &= ~MAC_CFG_RX_EN;
		sf_writel(mac_cfg, get_mac_cfg_addr(i));
	}
}

void sf_mac_disable_tx(void) {
	int i = 0;
	for(i = 0 ; i < SF_MAC_PORTS; i++){
		int mac_cfg = sf_readl(get_mac_cfg_addr(i));
		mac_cfg &= ~MAC_CFG_TX_EN;
		sf_writel(mac_cfg, get_mac_cfg_addr(i));
	}
}

int wait_phy_rw_not_busy(int phy_no)
{

	int reg = sf_readl(get_mii_cmd_addr(phy_no));
	// wait for GMII Register GB is not busy
	while(reg & GMII_ADDR_MASK_GB){
		reg = sf_readl(get_mii_cmd_addr(phy_no));
	}
	return reg;
}

//set phy reg with mac register
void write_phy_reg(int phy_no,int phy_reg,int value, struct phy_device* phy_dev)
{

	int addr_value = wait_phy_rw_not_busy(phy_no);
	//prepare data for register 5
	sf_writel(value & GMII_DATA_MASK, get_mii_data_addr(phy_no));
	//set address flag
	addr_value = (addr_value & (~GMII_ADDR_MASK_GR)) | ((phy_reg << 6) & GMII_ADDR_MASK_GR);
	//set phy addr
	addr_value = (addr_value & (~GMII_ADDR_MASK_PA)) | ((phy_dev[phy_no].addr << 11) & GMII_ADDR_MASK_PA);
	//set read flag
	addr_value = (addr_value & (~GMII_ADDR_MASK_GW)) | (GMII_ADDR_VALUE_GW_WRITE & GMII_ADDR_MASK_GW);
	//set GB flag to indica is busy now
	addr_value = addr_value | GMII_ADDR_MASK_GB;
	sf_writel(addr_value, get_mii_cmd_addr(phy_no));
	//wait for finish
	wait_phy_rw_not_busy(phy_no);
}

int read_phy_reg_direct(int phy_no,int phy_addr,int phy_reg)
{
	int value = wait_phy_rw_not_busy(phy_no);
	//clear the data first
	sf_writel(DEFAULT_PHY_VALUE, get_mii_data_addr(phy_no));
	//set address flag
	value = (value & (~GMII_ADDR_MASK_GR)) | ((phy_reg << 6) & GMII_ADDR_MASK_GR);
	//set phy addr
	value = (value & (~GMII_ADDR_MASK_PA)) | ((phy_addr << 11) & GMII_ADDR_MASK_PA);
	//set read flag
	value = (value & (~GMII_ADDR_MASK_GW)) | (GMII_ADDR_VALUE_GW_READ & GMII_ADDR_MASK_GW);
	//set GB flag to indica is busy now
	value = value | GMII_ADDR_MASK_GB;
	sf_writel(value, get_mii_cmd_addr(phy_no));
	//wait for complete
	wait_phy_rw_not_busy(phy_no);
	return sf_readl(get_mii_data_addr(phy_no)) & GMII_DATA_MASK;
}

int phy_scan(struct phy_device *phy_dev) {
	int addr_index = 0;
	int phy_no = 0;
	int phy_id1 = 0;
	int phy_id2 = 0;
	int phy_id = 0;
	int phy_scan_success_count = 0;
	for (phy_no = 0; phy_no < 5; phy_no++) {

		int addr = 0;
		//scan 0~31 phy id
		for (addr_index = -1; addr_index < 32; addr_index++) {
#ifndef FPGA
			addr = (addr_index == -1) ? (OMINI_PHY1_ADDR + phy_no) : addr_index;
#else
			if(addr_index == -1) continue;
			addr = addr_index;
#endif
			phy_id1 = read_phy_reg_direct(phy_no,addr,PHY_IDENTIFY_1) & 0xffff;
			phy_id2 = read_phy_reg_direct(phy_no,addr,PHY_IDENTIFY_2) & 0xffff;
			phy_id = (phy_id2 | (phy_id1 << 16));
			//If the phy_id is mostly Fs, there is no device there(accord with linux standard phy driver detect code)
			if ((phy_id & 0x1fffffff) != 0x1fffffff){
				//PHY IDENTIFIER1[15:0] -----------PHY_ID[15:0]
				//PHY IDENTIFIER2[15:10]-----------PHY_ID[21:16]
				phy_dev[phy_no].phy_id = phy_id1 | ((phy_id2 << 6) & 0x3F0000);
				phy_dev[phy_no].addr = addr;
				phy_scan_success_count++;
				break;
			}
		}
	}
	return phy_scan_success_count;
}

int read_phy_reg(int phy_no,int phy_reg, struct phy_device* phy_dev) {
	return read_phy_reg_direct(phy_no,phy_dev[phy_no].addr,phy_reg);
}

void sf_phy_init(struct phy_device* phy_dev) {
	//scan first to get phy id and addr
	phy_scan(phy_dev);

#ifdef CONFIG_SF16A18_SWITCH_FPGA
	int i = 0;
	for(i = 0 ; i < SF_MAC_PORTS ; i++){ phy_dev[i].duplex = DUPLEX_FULL;
		phy_dev[i].speed = SPEED_100;
		//init marvell phy
		/*Marvell speed Bit 6,13  of Control registers
		  |   10 = 1000 Mbps
		  |   01 = 100 Mbps
		  |   00 = 10 Mbps
		  |   */
		//init marvell phy(almost the same)
		//Change Page Number for copper
		write_phy_reg(i,MIIM_88E1111_PHY_PAGE,0x0000, phy_dev);
		//read first
		int phy_ctrl = read_phy_reg(i,PHY_BASE_CONTROL_REGISTER, phy_dev);

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

			phy_ctrl = (phy_ctrl & (~MAVELL_PHY_SPEED_MASK)) | MAVELL_PHY_SPEED_1000m;
		}
		write_phy_reg(i,PHY_BASE_CONTROL_REGISTER,phy_ctrl, phy_dev);
		//reset phy and wait reset flag be cleared
		phy_ctrl = (read_phy_reg(i,PHY_BASE_CONTROL_REGISTER, phy_dev) | PHY_BASE_ENABLE_RESET);
		write_phy_reg(i,PHY_BASE_CONTROL_REGISTER,phy_ctrl, phy_dev);
		while (phy_ctrl & PHY_BASE_ENABLE_RESET) {

			phy_ctrl = read_phy_reg(i,PHY_BASE_CONTROL_REGISTER, phy_dev);
		}
		// enable marvell phy csr signal assert when transmit,by set register 16 bit 11
		// we have to use cable to make sure that the phy is linked.
		// c = Assert on transmit
		// 0 = Never assert on transmit
		int reg = read_phy_reg(i, (0x10), phy_dev) | (0x1 << 11);
		write_phy_reg(i, 0x10, reg, phy_dev);
	}

	//todo: now marvell phy reset very slow,and we have to wait 4 seconds to ensure it works in the right state
	//we will confirm it latter
	msleep(5000);
#endif
}

#ifdef CONFIG_SFA18_PCBA_TEST
extern struct sf_priv *g_priv;
int check_phy_linkup(int phy_no, struct sf_priv *priv){
	int phy_status = read_phy_reg_direct(phy_no, priv->phy_dev[phy_no].addr, PHY_BASE_STATUS_REGISTER);
	if((phy_status & DEFAULT_PHY_VALUE) == 0xffff) return 0;
	return (phy_status & PHY_STATUS_LINK_UP) ? 1 : 0;
}

void sf_phy_mode_init(int phy_num, struct phy_device* phy_dev, int mode){
	int phy_value = 0;

	phy_value = read_phy_reg(phy_num, OMINI_PHY_MODEL_CTRL_REGISTER, phy_dev);
	phy_value &= ~OMINI_PHY_MODEL_CTRL_MDI_MASK;
	write_phy_reg(phy_num, OMINI_PHY_MODEL_CTRL_REGISTER, phy_value|mode, phy_dev);
}

#define OMINI_PHY_TSTCNTL_REGISTER          0x14
#define OMINI_PHY_TSTCNTL_ENABLE			0x400
int sf_phy_init_test_mode(int phy_idx, struct sf_priv *priv){
	unsigned int phy_status;
	phy_status = read_phy_reg(phy_idx, OMINI_PHY_TSTCNTL_REGISTER, priv->phy_dev);
	if(!(phy_status &  OMINI_PHY_TSTCNTL_ENABLE)){
		write_phy_reg(phy_idx, OMINI_PHY_TSTCNTL_REGISTER, 0x0, priv->phy_dev);
		write_phy_reg(phy_idx, OMINI_PHY_TSTCNTL_REGISTER,OMINI_PHY_TSTCNTL_ENABLE, priv->phy_dev);
		write_phy_reg(phy_idx, OMINI_PHY_TSTCNTL_REGISTER, 0x0, priv->phy_dev);
		write_phy_reg(phy_idx, OMINI_PHY_TSTCNTL_REGISTER,OMINI_PHY_TSTCNTL_ENABLE, priv->phy_dev);
	}
	return 0;
}

#define PHY_BASE_ENABLE_RESET            (1 << 15)
int sf_phy_reset(int phy_idx, struct sf_priv *priv){
	unsigned int phy_ctrl = (read_phy_reg(phy_idx, PHY_BASE_CONTROL_REGISTER, priv->phy_dev) | PHY_BASE_ENABLE_RESET);
	write_phy_reg(phy_idx, PHY_BASE_CONTROL_REGISTER,phy_ctrl, priv->phy_dev);
	udelay(256);// wait for reset
	while ((phy_ctrl & PHY_BASE_ENABLE_RESET) || (phy_ctrl == 0x0)) {
		phy_ctrl = read_phy_reg(phy_idx, PHY_BASE_CONTROL_REGISTER, priv->phy_dev);
		udelay(10);
	}
	sf_phy_init_test_mode(phy_idx, priv);
	return 0;
}

#define OMINI_PHY_TSTCNTL_WRITE				0x4000
#define OMINI_PHY_TSTWRITE_REGISTER			0x17
int sf_phy_write_test_reg(int phy_idx, struct sf_priv *priv, unsigned int reg_addr, unsigned reg_value, int phy_mode ){
	unsigned int phy_value = 0;
	phy_value =  OMINI_PHY_TSTCNTL_ENABLE | OMINI_PHY_TSTCNTL_WRITE | reg_addr | phy_mode;
	write_phy_reg(phy_idx, OMINI_PHY_TSTWRITE_REGISTER, reg_value, priv->phy_dev);
	write_phy_reg(phy_idx, OMINI_PHY_TSTCNTL_REGISTER, phy_value, priv->phy_dev);
	return 0;
}

#define OMINI_PHY_ANALOG_A5_CONFIG_REGISTER       0x16
#define OMINI_PHY_TSTCNTL_MODE_ANA                0x0
#define OMINI_PHY_ANALOG_A5_CONFIG_TX_IDLE        0x487
int sf_phy_a5_tx_config(int phy_idx, struct sf_priv *priv, unsigned int value){
	sf_phy_write_test_reg(phy_idx, priv, OMINI_PHY_ANALOG_A5_CONFIG_REGISTER, value, OMINI_PHY_TSTCNTL_MODE_ANA);
	return 0;
}

/**
 *  sf_low_power - set unlinked phy power save
 *  Description: save power for wifi pcba test
 *  Comment: It will cost 2~3s for phy link, so all phy will be found not link when pcba init.
 *  So we set all phy to power save and reset it in eth_test.
 */
void sf_low_power(void){
	int i = 0, phy_value = 0;
	int port_list = CONFIG_SFA18_ETH_PORTS;

	for(i = 0 ; i < SF_MAC_PORTS; i++){
		if (port_list & (1 << i)){
			// communicate pc with default tx port, should not set to power save
#ifdef CONFIG_SFAX8_DEFAULT_TX_PORT
			if (i != CONFIG_SFAX8_DEFAULT_TX_PORT) {
#endif
				//set phy 10M duplex half to save power
				phy_value = read_phy_reg(i, PHY_BASE_CONTROL_REGISTER, g_priv->phy_dev);
				write_phy_reg(i, PHY_BASE_CONTROL_REGISTER, phy_value & ~PHY_BASE_ENABLE_AUTO_NEG, g_priv->phy_dev);

				//config phy to test mode, use a5 analog register to reduce tx power
				sf_phy_init_test_mode(i, g_priv);
				sf_phy_a5_tx_config(i, g_priv, OMINI_PHY_ANALOG_A5_CONFIG_TX_IDLE);
#ifdef CONFIG_SFAX8_DEFAULT_TX_PORT
			}
#endif
		}
	}
}

/**
 *  sf_phy_reset_for_low_power - reset phy for link
 *  Description: reset all phy to normal and check phy link up
 *  Comment: Once check phy link timeout, will return here
 */
int sf_phy_reset_for_low_power(void){
	int i, status, timer;
	int port_list = CONFIG_SFA18_ETH_PORTS;

	// reset all phy normal first
	for(i = 0; i < SF_MAC_PORTS; i++)
		if (port_list & (1 << i)
#ifdef CONFIG_SFAX8_DEFAULT_TX_PORT
				&& i != CONFIG_SFAX8_DEFAULT_TX_PORT
#endif
				)
		sf_phy_reset(i, g_priv);

	// set self connect MDI and MDIX, like p10m and ac
	if (port_list == 0xf || port_list == 0x1f || port_list == 0x3 || port_list == 0x13){
		for(i = 0; i < SF_MAC_PORTS; i++){
			if (port_list & (1 << i)
#ifdef CONFIG_SFAX8_DEFAULT_TX_PORT
				       	&& i != CONFIG_SFAX8_DEFAULT_TX_PORT
#endif
					) {
				if ((i + 1) % 2)
				  sf_phy_mode_init(i, g_priv->phy_dev, OMINI_PHY_MODEL_CTRL_MDIX);
				else
				  sf_phy_mode_init(i, g_priv->phy_dev, OMINI_PHY_MODEL_CTRL_MDI);
			}
		}
	}

	// wait for phy link
	for(i = 0; i < SF_MAC_PORTS; i++){
		if (port_list & (1 << i)){
			timer = 0;
			while(1){
				status = check_phy_linkup(i, g_priv);
				if(status || timer > 300)
				  break;
				timer++;
				mdelay(10);
			}

			if(timer > 300){
				printf("check phy%d link 3s timeout\n", i);
				return -1;
			}else
			  printf("check phy%d link time %d ms\n", i, timer*10);

			// check phy link in 100M
			status = read_phy_reg(i, 31, g_priv->phy_dev);
			if ((status & 0x1c) != 0x18){
				printf("check phy%d link not in 100M, status 0x%x\n", i, status);
				return -2;
			}
		}
	}

	return 0;
}
#endif
