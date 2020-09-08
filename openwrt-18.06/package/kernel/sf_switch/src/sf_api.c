#ifdef CONFIG_SFAX8_SWITCH_API
#include "sf_api.h"
#endif
#include "sf_eth.h"
#include "sf_mdio.h"
#include "sf_common.h"
#ifdef CONFIG_SFAX8_SWITCH_VLAN
#include "sf_switch.h"
#endif
#include "sf_eth_addr.h"
#include "sf_switch_init.h"

extern int get_gpi_addr_base( int dev_no );
/* dump packet content
 * @buf skb
 */
int dump_packet(unsigned char *buf , int len){
	int i = 0;
	printk("\n");
	printk("======packet content=len is %d========\n",len);
	for(;i < len; i++){
		printk("0x%02x,", buf[i]);
		if(((i+1)%16 == 0) && (i != 0)){
			printk("\n");
		}
	}

	printk("\n");
	printk("==============dump end=============\n");
	printk("\n");
	return 0;
}

/* set global snoop special mac
 * @idx decide which addr register to choose;
 * @mac a pointer to the mac addr;
 * @mask a pointer to the mask addr;
 * @enable enable snoop control register,always set to 1;
 * return 0 success,-1 fail;
 * Description : save the special mac addr.If match the mac addr,the packet will be punted to host.
 */
int set_global_snoop_special_mac(int idx, unsigned long long *mac, unsigned long long *mask, int enable)
{
	int read_val = 0;
	int mac_msb   = ((*mac >> 40) & 0xFF) | ((*mac >> 24) & 0xFF00);
	int mac_lsb   = *mac & 0xFFFFFFFF;
	int mask_msb  = ((*mask >> 40) & 0xFF) | ((*mask >> 24) & 0xFF00);
	int mask_lsb  = *mask & 0xFFFFFFFF;
	if(idx == 0) {
		if(enable) {
			read_val = sf_readl(REG_CLASS_SNOOP_CTRL);
			sf_writel(STP_CTRL_ADDR1_REG_VAL|read_val, REG_CLASS_SNOOP_CTRL);

			sf_writel(mac_lsb, REG_CLASS_SNOOP_SPL_MCAST_ADDR1_LSB);
			sf_writel(mac_msb, REG_CLASS_SNOOP_SPL_MCAST_ADDR1_MSB);
			sf_writel(mask_lsb, REG_CLASS_SNOOP_SPL_MCAST_MASK1_LSB);
			sf_writel(mask_msb, REG_CLASS_SNOOP_SPL_MCAST_MASK1_MSB);
			printk("set value 0x%x to REG_CLASS_SNOOP_CTRL,read value is 0x%x\n", (STP_CTRL_ADDR1_REG_VAL|read_val), sf_readl(REG_CLASS_SNOOP_CTRL));
			printk("set mac_lsb 0x%x mac_msb 0x%x, read value is 0x%x 0x%x\n",
					mac_lsb, mac_msb, sf_readl(REG_CLASS_SNOOP_SPL_MCAST_ADDR1_LSB), sf_readl(REG_CLASS_SNOOP_SPL_MCAST_ADDR1_MSB));
			printk("set mask_lsb 0x%x mask_msb 0x%x, read value is 0x%x 0x%x\n",
					mask_lsb, mask_msb, sf_readl(REG_CLASS_SNOOP_SPL_MCAST_MASK1_LSB), sf_readl(REG_CLASS_SNOOP_SPL_MCAST_MASK1_MSB));
		}
		else {
			read_val = sf_readl(REG_CLASS_SNOOP_CTRL);
			sf_writel((~STP_CTRL_ADDR1_REG_VAL)&read_val, REG_CLASS_SNOOP_CTRL);
			printk("set value 0x%x to REG_CLASS_SNOOP_CTRL, read value is 0x%x\n", (~STP_CTRL_ADDR1_REG_VAL)&read_val, sf_readl(REG_CLASS_SNOOP_CTRL));
		}

	}
	else if (idx == 1) {
		if(enable) {
			read_val = sf_readl(REG_CLASS_SNOOP_CTRL);
			sf_writel(STP_CTRL_ADDR2_REG_VAL|read_val, REG_CLASS_SNOOP_CTRL);

			sf_writel(mac_lsb, REG_CLASS_SNOOP_SPL_MCAST_ADDR2_LSB);
			sf_writel(mac_msb, REG_CLASS_SNOOP_SPL_MCAST_ADDR2_MSB);
			sf_writel(mask_lsb, REG_CLASS_SNOOP_SPL_MCAST_MASK2_LSB);
			sf_writel(mask_msb, REG_CLASS_SNOOP_SPL_MCAST_MASK2_MSB);
			printk("set value 0x%x to REG_CLASS_SNOOP_CTRL,read value is 0x%x\n", (STP_CTRL_ADDR2_REG_VAL|read_val), sf_readl(REG_CLASS_SNOOP_CTRL));
			printk("set mac_lsb 0x%x mac_msb 0x%x, read value is 0x%x 0x%x\n",
					mac_lsb, mac_msb, sf_readl(REG_CLASS_SNOOP_SPL_MCAST_ADDR2_LSB), sf_readl(REG_CLASS_SNOOP_SPL_MCAST_ADDR2_MSB));
			printk("set mask_lsb 0x%x mask_msb 0x%x, read value is 0x%x 0x%x\n",
					mask_lsb, mask_msb, sf_readl(REG_CLASS_SNOOP_SPL_MCAST_MASK2_LSB), sf_readl(REG_CLASS_SNOOP_SPL_MCAST_MASK2_MSB));

		}
		else {
			read_val = sf_readl(REG_CLASS_SNOOP_CTRL);
			sf_writel((~STP_CTRL_ADDR2_REG_VAL)&read_val, REG_CLASS_SNOOP_CTRL);
			printk("set value 0x%x to REG_CLASS_SNOOP_CTRL,read value is 0x%x\n", (STP_CTRL_ADDR2_REG_VAL|read_val), sf_readl(REG_CLASS_SNOOP_CTRL));
		}
	}
	else {
		return -1;
	}
	return 0;
}

/*set mac loopback
 *@port_list 0x1f bit 1 equivalent to port 0,bit 5 equivalent to port 4
 *Description : decide which ports to set loopback,set to 1 means set loopback
 */
void set_mac_loopback(int port_list)
{
	int i = 0, mac_cfg = 0, value = 0;
	for(i = 0; i < 5; i++){
		value = port_list & (1 << i);
		if(value){
			mac_cfg = sf_readl(get_mac_cfg_addr(i));
			mac_cfg |= MAC_CFG_LOOPBACK_MODE;
			sf_writel(mac_cfg, get_mac_cfg_addr(i));
		}
	}
}

void set_mac_no_loopback(int port)
{
	int mac_cfg = 0;
	mac_cfg = sf_readl(get_mac_cfg_addr(port));
	mac_cfg &= ~MAC_CFG_LOOPBACK_MODE;
	sf_writel(mac_cfg, get_mac_cfg_addr(port));
}

/* configure tmu schedulers
 * @psch a pointer to struct tmu_schd_params_t
 * return 0 success,-1 fail;
 */
int set_tmu_scheduler_config(tmu_schd_params_t *psch)
{
	int read_val = 0;

	psch->tlsd_schd_id--;
	psch->tlsd_dev_id--;
	psch->tlsd_position--;

	/*checking for allw_tdq_prog-> bit 4 in PHY0_TDQ_CTRL register */
	read_val = sf_readl(REG_TMU_PHY0_TDQ_CTRL + (REG_LEN * psch->tlsd_dev_id));

	/* checking  for allw_tdq_prog-> 1 and hw_en -> 0 */
	if (!(((read_val & 0x00000010) == 0x00000010) &&
				((read_val & 0x00000002) == 0)))
		return -1;

	/* writing algorithm type */
	sf_writel(psch->tlsd_algo,
			REG_TMU_PHY0_SCH0_CTRL + psch->tlsd_schd_id * TMU_EACH_SCH_OFFSET +
			TMU_EACH_PHY_OFFFSET * psch->tlsd_dev_id );
	printk("set tmu algorithm type 0x%x ,read value is 0x%x\n", psch->tlsd_algo,
			sf_readl(REG_TMU_PHY0_SCH0_CTRL + psch->tlsd_schd_id * TMU_EACH_SCH_OFFSET +TMU_EACH_PHY_OFFFSET * psch->tlsd_dev_id));

	if(psch->tlsd_rate_units < 2){
		sf_writel(psch->tlsd_rate_units,
				REG_TMU_PHY0_SCH1_BIT_RATE + psch->tlsd_schd_id * TMU_EACH_SCH_OFFSET +
				TMU_EACH_PHY_OFFFSET * psch->tlsd_dev_id );
		printk("set tmu rate units 0x%x in scheduler,read value is 0x%x\n", psch->tlsd_rate_units,
				sf_readl(REG_TMU_PHY0_SCH1_BIT_RATE + psch->tlsd_schd_id * TMU_EACH_SCH_OFFSET + TMU_EACH_PHY_OFFFSET * psch->tlsd_dev_id));
	}

	if (psch->tlsd_schd_id == 0) {
		/* writing the o/p of sch0 position connection to sch1 */
		sf_writel(psch->tlsd_position,
				REG_TMU_PHY0_SCH0_POS +
				TMU_EACH_PHY_OFFFSET * psch->tlsd_dev_id );
		printk("set tmu sch0 position 0x%x ,read value is 0x%x\n", psch->tlsd_position,
				sf_readl(REG_TMU_PHY0_SCH0_POS + TMU_EACH_PHY_OFFFSET * psch->tlsd_dev_id));

		/*Update que weight*/
		sf_writel(psch->tlsd_weight,
				REG_TMU_PHY0_SCH0_Q0_WGHT + TMU_EACH_SCH_OFFSET * 1 + REG_LEN * psch->tlsd_position +
				TMU_EACH_PHY_OFFFSET * psch->tlsd_dev_id );
		printk("set tmu queue weight 0x%x ,read value is 0x%x\n", psch->tlsd_weight,
				sf_readl(REG_TMU_PHY0_SCH0_Q0_WGHT + TMU_EACH_SCH_OFFSET * 1 + REG_LEN * psch->tlsd_position + TMU_EACH_PHY_OFFFSET * psch->tlsd_dev_id));
	}

	return 0;
}


/* Description : To get shaper config parameters calculated based on rate and frequency */
int tmu_shaper_config_parameters_pps(uint rate_units, uint rate,
		uint clk_frequency, uint *intwt, uint *frcwt, uint *clkd){
	int wt = 0, sysclk_khz = 0, temp = 0;
	int clkdiv = 0, trydiv = 0, w_i = 0, w_f = 0;

	/*Clock division intial value*/
	clkdiv = 2;

	/*Changing Mhz to KHz*/
	sysclk_khz = clk_frequency * 1000;

	/* find the clkdiv value that gives us the largest valid wt value */
	//define MAX_CLKDIV is 16384,MAX_WT is 0xFFFF
	while (clkdiv < 16384) {
		trydiv = clkdiv << 1;
		wt = (((rate * trydiv)/sysclk_khz) * (4096));
		if (wt > 0xFFFFF)
			break;
		clkdiv = trydiv;
		temp++;
	}
	wt = (((rate * clkdiv)/sysclk_khz) * (4096));
	w_i = wt >> 12;
	w_f = wt & 0xfff;

	/*Updating values*/
	/* *clkd = clkdiv; */
	*clkd = temp;
	*frcwt = w_f;
	*intwt = w_i;
	return 0;
}

/* Description : To get shaper config parameters calculated based on rate and frequency */
int tmu_shaper_config_parameters(uint rate_units, uint rate,
		uint clk_frequency, uint *intwt, uint *frcwt, uint *clkd){
	int wt = 0, sysclk_khz = 0, temp = 0;
	int clkdiv = 0, trydiv = 0, w_i = 0, w_f = 0;

	/*Clock division intial value*/
	clkdiv = 2;

	/*Changing Mhz to KHz*/
	sysclk_khz = clk_frequency * 1023;

	/* find the clkdiv value that gives us the largest valid wt value */
	while (clkdiv < 16384) {
		trydiv = clkdiv << 1;
		/* wt = (rate * trydiv * (4096 / 8)) / (sysclk_khz) ; */
		wt = (((rate * trydiv)/sysclk_khz) * (4096 / 8));
		if (wt > 0xFFFFF)
			break;
		clkdiv = trydiv;
		temp++;
	}
	/* wt = (rate * clkdiv * (4096 / 8)) / (sysclk_khz); */
	wt = (((rate * clkdiv)/sysclk_khz) * (4096 / 8));
	w_i = wt >> 12;
	w_f = wt & 0xfff;

	/*Updating values*/
	/* *clkd = clkdiv; */
	*clkd = temp;
	*frcwt = w_f;
	*intwt = w_i;
	return 0;
}

/*configure tmu shapers
 *@pshp a pointer to struct tmu_shpr_params_t
 *@dynamic dynamic condition to checking appropriate value is set
 * */
int set_tmu_shaper_config(tmu_shpr_params_t *pshp,  uint dynamic)
{
	int read_val = 0;
	int write_val = 0;
	int intwt, frc_wt, clk_div;

	pshp->tlsp_shpr_id--;
	pshp->tlsp_dev_id--;
	pshp->tlsp_position--;

	/*To get shpaer config parameters
	  calculated based on rate and frequency*/
	if (pshp->tlsp_rate_units)
		tmu_shaper_config_parameters_pps(pshp->tlsp_rate_units,
				pshp->tlsp_rate,
				pshp->tlsp_clk_frequency,
				&intwt, &frc_wt, &clk_div);
	else
		tmu_shaper_config_parameters(pshp->tlsp_rate_units,
				pshp->tlsp_rate,
				pshp->tlsp_clk_frequency,
				&intwt, &frc_wt, &clk_div);

	/*checking for allw_tdq_prog-> bit 4 in PHY0_TDQ_CTRL register */
	read_val = sf_readl(REG_TMU_PHY0_TDQ_CTRL + (REG_LEN * pshp->tlsp_dev_id));

	if (!dynamic) {
		/* checking  for allw_tdq_prog-> 1 and hw_en -> 0 */
		if (!(((read_val & 0x00000010) == 0x00000010) &&
					((read_val & 0x00000002) == 0)))
			return -1;
	}

	/* writing clock division value in TMU_SHP0_CTRL register */
	write_val = SHPR_EN_BIT + (clk_div << 1);
	sf_writel(write_val,REG_TMU_PHY0_SHP0_CTRL + (SHPR_ADDRESS_MASK * pshp->tlsp_shpr_id) +
			(DEV_ADDRESS_MASK * pshp->tlsp_dev_id));
	printk("set clk_div 0x%x,read value is 0x%x\n", write_val,
			sf_readl(REG_TMU_PHY0_SHP0_CTRL + (SHPR_ADDRESS_MASK * pshp->tlsp_shpr_id) + (DEV_ADDRESS_MASK * pshp->tlsp_dev_id)));

	write_val = 0;
	/* writing fractional wght and int_wgt in TMU_SHP0_WGHT register */
	write_val = (frc_wt & 0x00000FFF) | ((intwt & 0x000000FF) << 12);
	sf_writel(write_val,REG_TMU_PHY0_SHP0_WGHT + (SHPR_ADDRESS_MASK * pshp->tlsp_shpr_id) +
			(DEV_ADDRESS_MASK * pshp->tlsp_dev_id));
	printk("set weight 0x%x,read value is 0x%x\n", write_val,
			sf_readl(REG_TMU_PHY0_SHP0_WGHT + (SHPR_ADDRESS_MASK * pshp->tlsp_shpr_id) + (DEV_ADDRESS_MASK * pshp->tlsp_dev_id)));

	write_val = 0;
	/* writing max credit value in TMU_SHP0_MAX_CREDIT register */
	write_val = ((pshp->tlsp_max_credit) << 10);
	sf_writel(write_val,REG_TMU_PHY0_SHP0_MAX_CREDIT + (SHPR_ADDRESS_MASK * pshp->tlsp_shpr_id) +
			(DEV_ADDRESS_MASK * pshp->tlsp_dev_id));
	printk("set max credit value 0x%x,read value is 0x%x\n", write_val,
			sf_readl(REG_TMU_PHY0_SHP0_MAX_CREDIT + (SHPR_ADDRESS_MASK * pshp->tlsp_shpr_id) + (DEV_ADDRESS_MASK * pshp->tlsp_dev_id)));

	write_val = 0;
	/* writing rate units and position in TMU_SHP0_CTRL2 register */
	write_val =  ((pshp->tlsp_rate_units) |
			((pshp->tlsp_position) << 1)|
			((pshp->tlsp_mode) << 6));

	sf_writel(write_val,REG_TMU_PHY0_SHP0_CTRL2 + (SHPR_ADDRESS_MASK * pshp->tlsp_shpr_id) +
			(DEV_ADDRESS_MASK * pshp->tlsp_dev_id));
	printk("set rate units and position 0x%x in shaper,read value is 0x%x\n", write_val,
			sf_readl(REG_TMU_PHY0_SHP0_CTRL2 + (SHPR_ADDRESS_MASK * pshp->tlsp_shpr_id) + (DEV_ADDRESS_MASK * pshp->tlsp_dev_id)));

	write_val = 0;
	/* writing min credit in TMU_SHP0_MIN_CREDIT register */
	write_val =  ((pshp->tlsp_min_credit));
	sf_writel(write_val,REG_TMU_PHY0_SHP0_MIN_CREDIT + (SHPR_ADDRESS_MASK * pshp->tlsp_shpr_id) +
			(DEV_ADDRESS_MASK * pshp->tlsp_dev_id));
	printk("set min credit 0x%x,read value is 0x%x\n", write_val,
			sf_readl(REG_TMU_PHY0_SHP0_MIN_CREDIT + (SHPR_ADDRESS_MASK * pshp->tlsp_shpr_id) + (DEV_ADDRESS_MASK * pshp->tlsp_dev_id)));

	return 0;
}

/*Description : Context memory register write function Writes given value in given address.*/
void write_fasw_context_memory(uint data, uint que_no, uint phy_no, uint reg_no)
{
	/* selecting register */
	sf_writel( (reg_no + que_no * TOTAL_CONTEXT_REGS)
			+ (phy_no << 16), REG_TMU_CNTX_ADDR);
	printk("selecting register 0x%x ,read value is 0x%x\n", (reg_no + que_no * TOTAL_CONTEXT_REGS) + (phy_no << 16), sf_readl(REG_TMU_CNTX_ADDR));

	/* initialising the register with 0 */
	sf_writel(data, REG_TMU_CNTX_DATA);
	printk("initialising register with 0x%x ,read value is 0x%x\n", data, sf_readl(REG_TMU_CNTX_DATA));

	/* csr_cntx_ind_cmd -> 1 and csr_cntx_ind_start->1 */
	sf_writel(WRITE_CMD, REG_TMU_CNTX_CMD);
	sf_writel( WRITE_CMD + START_CMD, REG_TMU_CNTX_CMD);
	printk("set csr_cntx_ind_cmd and csr_cntx_ind_start 0x%x ,read value is 0x%x\n",WRITE_CMD + START_CMD, sf_readl(REG_TMU_CNTX_CMD));

	/* poll for csr_cntx_ind_done bit */
	while (!(sf_readl(REG_TMU_CNTX_CMD)
				& DONE_BIT));  /* End of while loop */

}

/*Description : Helper function for pcm_classq_add() Function*/
int tmu_add_queue2scheduler(tmu_classq_params_t *que)
{
	uint read_val = 0;
	uint read_val1 = 0, read_val2 = 0;

	/*Loop for number of achedulers*/
	/*Read Alloc0 register*/
	read_val1 = sf_readl(REG_TMU_PHY0_SCH0_Q_ALLOC0 + SHED_ADDRESS_MASK * que->tlcq_schd_id +
			(DEV_ADDRESS_MASK * que->tlcq_dev_id));
	/*Read alloc1 register*/
	read_val2 = sf_readl(REG_TMU_PHY0_SCH0_Q_ALLOC1 + SHED_ADDRESS_MASK * que->tlcq_schd_id +
			(DEV_ADDRESS_MASK * que->tlcq_dev_id));

	/*Switch to select que priority*/
	switch (que->tlcq_schd_slot) {
		case SCH_SLOT_ZERO:
			/*Update alloc register with given queue number*/
			read_val1 = (read_val1 & 0xFFFFFFF0) |
				(que->tlcq_classq_id & 0x0000000F);
			break;
		case SCH_SLOT_ONE:
			/*Update alloc register with given queue number*/
			read_val1 = (read_val1 & 0xFFFFF0FF) |
				((que->tlcq_classq_id & 0x0000000F) << 8);
			break;
		case SCH_SLOT_TWO:
			/*Update alloc register with given queue number*/
			read_val1 = (read_val1 & 0xFFF0FFFF) |
				((que->tlcq_classq_id & 0x0000000F) << 16);
			break;
		case SCH_SLOT_THREE:
			/*Update alloc register with given queue number*/
			read_val1 = (read_val1 & 0xF0FFFFFF) |
				((que->tlcq_classq_id & 0x0000000F) << 24);
			break;
		case SCH_SLOT_FOUR:
			/*Update alloc register with given queue number*/
			read_val2 = (read_val2 & 0xFFFFFFF0) | (que->tlcq_classq_id & 0x0000000F);
			break;
		case SCH_SLOT_FIVE:
			/*Update alloc register with given queue number*/
			read_val2 = (read_val2 & 0xFFFFF0FF) | ((que->tlcq_classq_id & 0x0000000F) << 8);
			break;
		case SCH_SLOT_SIX:
			/*Update alloc register with given queue number*/
			read_val2 = (read_val2 & 0xFFF0FFFF) | ((que->tlcq_classq_id & 0x0000000F) << 16);
			break;
		case SCH_SLOT_SEVEN:
			/*Update alloc register with given queue number*/
			read_val2 = (read_val2 & 0xF0FFFFFF) | ((que->tlcq_classq_id & 0x0000000F) << 24);
			break;
		default:
			break;
	}

	/*checking for allw_tdq_prog-> bit 4 in PHY0_TDQ_CTRL register */
	read_val = sf_readl(REG_TMU_PHY0_TDQ_CTRL + (REG_LEN * que->tlcq_dev_id));

	/* checking  for allw_tdq_prog-> 1 and hw_en -> 0 */
	if (!(((read_val & 0x00000010) == 0x00000010) &&
				((read_val & 0x00000002) == 0))) {

		return -1;
	}

	/*Update que weight*/
	sf_writel(que->tlcq_weight,REG_TMU_PHY0_SCH0_Q0_WGHT + REG_LEN * que->tlcq_classq_id +
			SHED_ADDRESS_MASK * que->tlcq_schd_id + (DEV_ADDRESS_MASK * que->tlcq_dev_id ));

	/*Write que number into hardware*/
	if (que->tlcq_schd_slot < SCH_SLOT_FOUR) {

		sf_writel(read_val1,REG_TMU_PHY0_SCH0_Q_ALLOC0 +
				SHED_ADDRESS_MASK * que->tlcq_schd_id + (DEV_ADDRESS_MASK * que->tlcq_dev_id ));

	} else {

		sf_writel(read_val2,REG_TMU_PHY0_SCH0_Q_ALLOC1 +
				SHED_ADDRESS_MASK * que->tlcq_schd_id + (DEV_ADDRESS_MASK * que->tlcq_dev_id));
	}

	return 0;
}

/*Description : This function adds classq for a given scheduler*/
int set_tmu_classq_add(tmu_classq_params_t *que)
{
	uint write_val = 0;

	/*Sett al id's to original val*/
	que->tlcq_classq_id--;
	que->tlcq_dev_id--;
	que->tlcq_schd_id--;
	que->tlcq_schd_slot--;

	if (que->tlcq_qmgmt == TMU_QUE_TAIL_DROP) {
		/* writing queue managment in register */

		/* Qmax: Qmin : cfg = ([19:11],[10:2],[1:0]) */
		write_val = ((que->tlcq_max_len) << 11) + que->tlcq_qmgmt;

		write_fasw_context_memory(write_val, que->tlcq_classq_id,
				que->tlcq_dev_id, CURQ_QMAX_QMIN_CFG);

		/* if queue management is WRED */
	} else if (que->tlcq_qmgmt == TMU_QUE_WRED) {
		/* writing queue managment , Qmax and Qmin values to register */

		write_val = 0;
		/* Qmax: Qmin : cfg = ([19:11],[10:2],[1:0]) */
		write_val = ((que->tlcq_max_len) << 11) +
			((que->tlcq_min_len) << 2) + que->tlcq_qmgmt;

		write_fasw_context_memory(write_val, que->tlcq_classq_id,
				que->tlcq_dev_id, CURQ_QMAX_QMIN_CFG);

		/* writing probability values to prob_cfg registers */
		write_val = 0;
		write_val = ((que->tlcq_prob_cfg5) << 25) +
			((que->tlcq_prob_cfg4) << 20) +
			((que->tlcq_prob_cfg3) << 15) +
			((que->tlcq_prob_cfg2) << 10) +
			((que->tlcq_prob_cfg1) << 5) +
			que->tlcq_prob_cfg0;
		write_fasw_context_memory(write_val, que->tlcq_classq_id,
				que->tlcq_dev_id, CURQ_HW_PROB_CFG_TBL0);

		write_val = 0;
		write_val = ((que->tlcq_prob_cfg7) << 5) +
			que->tlcq_prob_cfg6;
		write_fasw_context_memory(write_val, que->tlcq_classq_id,
				que->tlcq_dev_id,
				CURQ_HW_PROB_CFG_TBL1);

	}

	/*Add queue to scheduler*/
	tmu_add_queue2scheduler(que);

	return 0;
}

void set_tmu_tdq_default_enable(void) {
	sf_writel(TMU_HW_EN, REG_TMU_PHY0_TDQ_CTRL);
	sf_writel(TMU_HW_EN, REG_TMU_PHY1_TDQ_CTRL);
	sf_writel(TMU_HW_EN, REG_TMU_PHY2_TDQ_CTRL);
	sf_writel(TMU_HW_EN, REG_TMU_PHY3_TDQ_CTRL);
	sf_writel(TMU_HW_EN, REG_TMU_PHY4_TDQ_CTRL);
	sf_writel(TMU_HW_EN, REG_TMU_PHY5_TDQ_CTRL);
	printk("set tmu HW_EN 0x%x for every port, read value is port0 0x%x port1 0x%x port2 0x%x port3 0x%x port4 0x%x port 5 0x%x\n",TMU_HW_EN,
			sf_readl(REG_TMU_PHY0_TDQ_CTRL), sf_readl(REG_TMU_PHY1_TDQ_CTRL), sf_readl(REG_TMU_PHY2_TDQ_CTRL),
			sf_readl(REG_TMU_PHY3_TDQ_CTRL), sf_readl(REG_TMU_PHY4_TDQ_CTRL), sf_readl(REG_TMU_PHY5_TDQ_CTRL));
}

/*TMU_PHY0_TDQ_CTRL
 *Description:	bit 0 - shp_clk_cntrl_en
 *				bit 1 - hw_en for tdq_sch_shaper
 *				bit 3:2 - for enabling the schedulers
 *				bit 4 - allw_tdq_prog
 * */
int set_tmu_tdq_enable(int dev_no, int enable)
{
	int read_val = 0,write_val = 0;

	if(enable) {
		/*making the hw_en -> 1 */
		read_val = sf_readl(REG_TMU_PHY0_TDQ_CTRL + (REG_LEN * dev_no));
		write_val = (read_val & 0xFFFFFFFD) | 0x2;
		sf_writel(write_val, REG_TMU_PHY0_TDQ_CTRL + (REG_LEN * dev_no));
		printk("set tmu_tdg_enable 0x%x ,read value is 0x%x\n", write_val, sf_readl(REG_TMU_PHY0_TDQ_CTRL + (REG_LEN * dev_no)));

	}
	else {
		/*making the hw_en -> 0 */
		read_val = sf_readl(REG_TMU_PHY0_TDQ_CTRL + (REG_LEN * dev_no));
		write_val = read_val & 0xFFFFFFFD;
		sf_writel(write_val, REG_TMU_PHY0_TDQ_CTRL + (REG_LEN * dev_no));
		printk("set dev_no %d tmu_tdg_disable 0x%x ,read value is 0x%x\n", dev_no, write_val, sf_readl(REG_TMU_PHY0_TDQ_CTRL + (REG_LEN * dev_no)));
		/* polling for allw_tdq_prog-> 1 */
		while (!((sf_readl(REG_TMU_PHY0_TDQ_CTRL + (REG_LEN * dev_no)) & 0x00000010) == 0x00000010)); /* End of while loop */
	}
	return 0;
}

/*set tmu various configure
 *@tsch_in scheduler parameters;
 *@tshp_in shaper parameters;
 *@tque_in queues parameters;
 *@port_list port list;
 *Description : set tmu configure,else with default.
 * */
int set_tmu_various_cfg(tmu_schd_params_t* tsch_in,tmu_shpr_params_t* tshp_in, tmu_classq_params_t* tque_in,int port_list)
{
	int i = 0, j = 0;
	int  phy_no = 0, max_dev_no = 0, ret_val = 0;
	tmu_schd_params_t tsch;
	tmu_shpr_params_t tshp;
	tmu_classq_params_t tque;

	memset(&tsch, 0, sizeof(tmu_schd_params_t));
	memset(&tshp, 0, sizeof(tmu_shpr_params_t));
	memset(&tque, 0, sizeof(tmu_classq_params_t));

	max_dev_no = TMU_MAX_DEV_NUM;

	for (phy_no = 0; phy_no < max_dev_no; phy_no++) {
		if((port_list != 0) && ((port_list & (0x1 << phy_no)) == 0))
			continue;
		set_tmu_tdq_enable(phy_no, 0);
	}

	/*Scheduler config*/
	for (j = 1; j <= max_dev_no; j++) {
		if((port_list != 0) && ((port_list & (0x1 << (j-1) )) == 0))
			continue;

		for (i = 1; i <= TMU_MAX_SCHED_NUM; i++) {
			if (i == 1) {
				tsch.tlsd_position = 1;
				if(tsch_in) {
					tsch.tlsd_algo = tsch_in->tlsd_algo;
					tsch.tlsd_weight = tsch_in->tlsd_weight;
					tsch.tlsd_rate_units = tsch_in->tlsd_rate_units;
				}
				else {
					tsch.tlsd_algo = 3; /* Round Robbin */
				}
			} else {
				if(tsch_in){
					//tsch.tlsd_algo = tsch_in->tlsd_algo;
					tsch.tlsd_algo = 0;
					tsch.tlsd_weight = tsch_in->tlsd_weight;
					tsch.tlsd_rate_units = tsch_in->tlsd_rate_units;
				}
				else {
					tsch.tlsd_algo = 0; /* priority queuing */
				}
			}
			tsch.tlsd_dev_id = j;
			tsch.tlsd_schd_id = i;
			ret_val = set_tmu_scheduler_config(&tsch);

		}
	}

	/*Shaper config*/
	for (j = 1; j <= max_dev_no; j++) {
		if((port_list != 0) && ((port_list & (0x1 << (j-1) )) == 0))
			continue;

		for (i = 1; i <= TMU_MAX_SHPR_NUM; i++) {

			if(tshp_in){
				tshp.tlsp_max_credit = tshp_in->tlsp_max_credit;
				tshp.tlsp_rate_units = tshp_in->tlsp_rate_units;
				/* 40 Mhz system clock */
				tshp.tlsp_clk_frequency  = tshp_in->tlsp_clk_frequency;
				tshp.tlsp_rate  = tshp_in->tlsp_rate ; /* 50Mbps */
			}
			else {
				tshp.tlsp_max_credit = 0x3FFE00;
				tshp.tlsp_rate_units = 0;
				/* 40 Mhz system clock */
				tshp.tlsp_clk_frequency  = 250;
				tshp.tlsp_rate  = 100000; /* 100Mbps */
			}
			tshp.tlsp_dev_id = j;
			tshp.tlsp_shpr_id = i;
			/* shaper 0 and 1 are connected to SP0 and SP1 */
			/* shaper 2 and 3 are Invalid */
			if (i < 3) {
				if(tshp_in){
					tshp.tlsp_mode = tshp_in->tlsp_mode; /*0x1-credit based mode */
				}
				else {
					tshp.tlsp_mode = 0x1; /*0x1-credit based mode */
				}
				tshp.tlsp_position = i;
			} else {
				tshp.tlsp_mode = 0x0; /*0x1-credit based mode */
				tshp.tlsp_position = 18;
			}
			if (tshp.tlsp_mode == 0)
				tshp.tlsp_min_credit = (tshp.tlsp_max_credit);
			else
				tshp.tlsp_min_credit = tshp.tlsp_max_credit;
			ret_val = set_tmu_shaper_config(&tshp, 0);
		}
	}

	/* queues adding*/
	for (j = 1; j <= max_dev_no; j++) {
		if((port_list != 0) && ((port_list & (0x1 << (j-1) )) == 0))
			continue;

		for (i = 1; i <= TMU_MAX_QUE_NO; i++) {
			if(tque_in) {
				tque.tlcq_qmgmt  =  tque_in->tlcq_qmgmt;
				tque.tlcq_max_len = tque_in->tlcq_max_len;
				if(tque.tlcq_qmgmt == TMU_QUE_WRED) {
					tque.tlcq_min_len   = tque_in->tlcq_min_len;
					tque.tlcq_prob_cfg0 = tque_in->tlcq_prob_cfg0;
					tque.tlcq_prob_cfg1 = tque_in->tlcq_prob_cfg1;
					tque.tlcq_prob_cfg2 = tque_in->tlcq_prob_cfg2;
					tque.tlcq_prob_cfg3 = tque_in->tlcq_prob_cfg3;
					tque.tlcq_prob_cfg4 = tque_in->tlcq_prob_cfg4;
					tque.tlcq_prob_cfg5 = tque_in->tlcq_prob_cfg5;
					tque.tlcq_prob_cfg6 = tque_in->tlcq_prob_cfg6;
					tque.tlcq_prob_cfg7 = tque_in->tlcq_prob_cfg7;
				}
				else {
					tque.tlcq_max_len =  tque_in->tlcq_max_len;
					tque.tlcq_qmgmt = 1;
				}
			}
			else {
				tque.tlcq_max_len = 100;
				tque.tlcq_qmgmt = 0;
			}
			tque.tlcq_weight = 0x3fe;
			tque.tlcq_dev_id = j;
			tque.tlcq_classq_id = i;

			if ((i == 3) || (i == 4)) {
				tque.tlcq_schd_id = 2;
				/* Actuval Q2 and Q3 connected to schedular-1, q6 and q7 */
				tque.tlcq_schd_slot = i+4;
			} else {
				tque.tlcq_schd_id = 1;
				tque.tlcq_schd_slot = i;
			}
			ret_val = set_tmu_classq_add(&tque);
		}
	}

	/* HW enable for the tdq fsm */
	set_tmu_tdq_default_enable();
	return 0;

}

/*npu gpi ingress config
 *@pshp shaper parameters;
 *@dev_no device number;
 *@effect Bits [1:0] value 00 Shaper0 is for port level data rate,
 *value 01 Shaper0 is for BCAST Packets, value 10 Shaper0 is for MCAST Packets.
 *		  Bits [3:2] value 00 Shaper1 is for port level data rate,
 *value 01 Shaper1 is for BCAST Packets, value 10 Shaper1 is for MCAST Packets
 *Description : ingress qos, config shape register.
 * */
int npu_gpi_ingress_config(tmu_shpr_params_t * pshp, int dev_no, int effect)
{
	int reg_shp_config = 0, reg_shp0_ctrl = 0, reg_shp0_wght = 0, reg_shp1_ctrl = 0, reg_shp1_wght = 0;
	reg_shp_config = get_gpi_addr_base(dev_no) + REG_EGPI_PORT_SHP_CONFIG;
	reg_shp0_ctrl = get_gpi_addr_base(dev_no) + REG_EGPI_PORT_SHP0_CTRL;
	reg_shp0_wght = get_gpi_addr_base(dev_no) + REG_EGPI_PORT_SHP0_WGHT;
	reg_shp1_ctrl = get_gpi_addr_base(dev_no) + REG_EGPI_PORT_SHP1_CTRL;
	reg_shp1_wght = get_gpi_addr_base(dev_no) + REG_EGPI_PORT_SHP1_WGHT;

	if(pshp == NULL){
		sf_writel(0x10,     reg_shp_config);
		sf_writel(0x3fff0005, reg_shp0_ctrl);
		sf_writel(0x260,    reg_shp0_wght);
        sf_writel(0x3fff0005, reg_shp1_ctrl);
        sf_writel(0x260,    reg_shp1_wght);
    }
    else{
        int intwt = 0, frc_wt = 0, clk_div = 0, write_val = 0;

        if (pshp->tlsp_rate_units)
            tmu_shaper_config_parameters_pps(pshp->tlsp_rate_units,pshp->tlsp_rate,pshp->tlsp_clk_frequency,&intwt, &frc_wt, &clk_div);
        else
            tmu_shaper_config_parameters(pshp->tlsp_rate_units,pshp->tlsp_rate,pshp->tlsp_clk_frequency, &intwt, &frc_wt, &clk_div);

        //write clk div && max credit
        write_val = SHPR_EN_BIT | (clk_div << 0x1) | (pshp->tlsp_max_credit << 0x8);
        sf_writel(write_val, reg_shp0_ctrl);
        sf_writel(write_val, reg_shp1_ctrl);
		printk("set value 0x%x to reg_shp0_ctrl and reg_shp1_ctrl,read value is 0x%x 0x%x\n", write_val, sf_readl(reg_shp0_ctrl), sf_readl(reg_shp1_ctrl));
        write_val = 0;
        //write weight
        write_val = (frc_wt & 0x000000FF) | ((intwt & 0x0007) << 8);
        sf_writel(write_val, reg_shp0_wght);
        sf_writel(write_val, reg_shp1_wght);
		printk("set value 0x%x to reg_shp0_wght and reg_shp1_wght,read value is 0x%x 0x%x\n", write_val, sf_readl(reg_shp0_wght), sf_readl(reg_shp1_wght));
        //write data units && effect
        write_val = 0;
        write_val = pshp->tlsp_rate_units | (pshp->tlsp_rate_units << 0x1 ) | (effect << 0x2);
        sf_writel(write_val,reg_shp_config);
		printk("set value 0x%x to reg_shp_config,read value is 0x%x\n", write_val, sf_readl(reg_shp_config));
    }

    return 0;
}

/*set global cutthru etype
 *@port_list 0x3f global ports
 *@cutthru_type cutthru type
 * */
int set_cutthru(int port_list, int cutthru_type)
{
	//init global cutthru list
	//indicates etype/tpid will be rewritten on some egress ports
	sf_writel( port_list| (cutthru_type << CUTTHRU_TYPE_START_POS),
			REG_CLASS_HW_GLOBAL_CUTTHRU_REG);
	printk("set value 0x%x to REG_CLASS_HW_GLOBAL_CUTTHRU_REG,read value is 0x%x\n",
			(port_list| (cutthru_type << CUTTHRU_TYPE_START_POS)), sf_readl(REG_CLASS_HW_GLOBAL_CUTTHRU_REG));

	return 0;
}

/*set dos register's value
 *@item decide which dos filter module to load or choose a function;
 *@value decide which type to filtering or what value set;
 *return 0 success, 0 fail.
 * */
int set_dos_value(int item, int value)
{
    int address = 0, write_val = 0,read_val = 0;
    switch ( item )
    {
        case DOS_ENABLE :
            address = REG_CLASS_DOS_CONTRL1;
            read_val = sf_readl(address);
            write_val = (value & DOS_ALL_ENABLE_MASK) | (read_val & (~DOS_ALL_ENABLE_MASK));
            break;

        case DOS_FILTER_ENABLE :
            address = REG_CLASS_DOS_CONTRL2;
            read_val = sf_readl(address);
            switch(value) {
                case IPV4_SIP_EQ_DIP_EN:
                    write_val = read_val| 0x1;
                    break;
                case IPV4_ICMP_FRAG_EN:
                    write_val = read_val| (0x1 << 1);
                    break;
                case IPV4_ICMP_PING_TOO_BIG_EN:
                    write_val = read_val| (0x1 << 2);
                    break;
                case IPV4_UDP_SP_EQ_DP_EN:
                    write_val = read_val| (0x1 << 3);
                    break;
                case IPV4_TCP_FLAG_CHK:
                    write_val = read_val| (0x1 << 4);
                    break;
                case IPV4_TCP_SP_EQ_DP:
                    write_val = read_val| (0x1 << 5);
                    break;
                case IPV4_TCP_FRAG_OFF1:
                    write_val = read_val| (0x1 << 6);
                    break;
                case IPV4_TCP_SYN_ERR:
                    write_val = read_val| (0x1 << 7);
                    break;
                case IPV4_TCP_XMAS:
                    write_val = read_val| (0x1 << 8);
                    break;
                case IPV4_TCP_NULL:
                    write_val = read_val| (0x1 << 9);
                    break;
                case IPV6_SIP_EQ_DIP_EN:
                    write_val = read_val| (0x1 << 16);
                    break;
                case IPV6_ICMP_FRAG_EN:
                    write_val = read_val| (0x1 << 17);
                    break;
                case IPV6_ICMP_PING_TOO_BIG_EN:
                    write_val = read_val| (0x1 << 18);
                    break;
                case IPV6_UDP_SP_EQ_DP_EN:
                    write_val = read_val| (0x1 << 19);
                    break;
                case IPV6_TCP_FLAG_CHK:
                    write_val = read_val| (0x1 << 20);
                    break;
                case IPV6_TCP_SP_EQ_DP:
                    write_val = read_val| (0x1 << 21);
                    break;
                case IPV6_TCP_FRAG_OFF1:
                    write_val = read_val| (0x1 << 22);
                    break;
                case IPV6_TCP_SYN_ERR:
                    write_val = read_val| (0x1 << 23);
                    break;
                case IPV6_TCP_XMAS:
                    write_val = read_val| (0x1 << 24);
                    break;
                case IPV6_TCP_NULL:
                    write_val = read_val| (0x1 << 25);
                    break;
                default:
                    break;
            }
            break;

        case DOS_COMB_FILTER_ENABLE :
            address = REG_CLASS_DOS_CONTRL1;
            read_val = sf_readl(address);
            switch(value) {
                case COMB_FILTER_1:
                    write_val = ((0x1 & DOS_COMB_CONTROL_MASK) << DOS_COMB_CONTROL_START_POS) | read_val;
                    break;
                case COMB_FILTER_2:
                    write_val = (((0x1 << 1) & DOS_COMB_CONTROL_MASK) << DOS_COMB_CONTROL_START_POS) | read_val;
                    break;
                case COMB_FILTER_3:
                    write_val = (((0x1 << 2) & DOS_COMB_CONTROL_MASK) << DOS_COMB_CONTROL_START_POS) | read_val ;
                    break;
                case COMB_FILTER_4:
                    write_val = (((0x1 << 3) & DOS_COMB_CONTROL_MASK) << DOS_COMB_CONTROL_START_POS) | read_val;
                    break;
                default:
                    break;
            }
            break;

        case DOS_COMB_FILTER_VALUE :
            address = REG_CLASS_DOS_TCP_FLAGCHK_COMB_VALUE1;
            write_val = (value & DOS_COMB_VALUE_MASK);
            break;

        case DOS_ICMPV4_MAX_LEN :
            address = REG_CLASS_DOS_ICMPV4_MAX_PKTLEN;
            write_val = (value & DOS_IPV4_MAX_LEN_MASK);
            break;

        default:
            return -1;
    }

	sf_writel(write_val, address);
	printk("write value 0x%x to address 0x%x,read value 0x%x\n", write_val, address, sf_readl(address));
    return 0;
}

/*2 Bit TC_Sel value mapping to DSCP2TC or PCP2TC or PID2TC as given below
  TC_SEL[1:0] == 0 -> DSCP2TC map
  TC_SEL[1:0] == 1 -> PCP2TC Map
  TC_Sel[1:0] == 2 -> MACDA2TC Map
  TC_Sel[1:0] == 3 -> PID2TC Map
  */
int set_tc_sel(int port, int map) {
	int val = 0, addr =0;
	switch(port){
		case 0:
			val = sf_readl(REG_CLASS_QOS_PORT01_TC_SEL);
            val &= 0xFFFF0000;
            map &= 0x0000FFFF;
            addr = REG_CLASS_QOS_PORT01_TC_SEL;
			break;

        case 1:
            val = sf_readl(REG_CLASS_QOS_PORT01_TC_SEL);
            val &= 0x0000FFFF;
            map &= 0xFFFF0000;
            addr = REG_CLASS_QOS_PORT01_TC_SEL;
            break;

        case 2:
            val = sf_readl(REG_CLASS_QOS_PORT23_TC_SEL);
            val &= 0xFFFF0000;
            map &= 0x0000FFFF;
            addr = REG_CLASS_QOS_PORT23_TC_SEL;
            break;

        case 3:
            val = sf_readl(REG_CLASS_QOS_PORT23_TC_SEL);
            val &= 0x0000FFFF;
            map &= 0xFFFF0000;
            addr = REG_CLASS_QOS_PORT23_TC_SEL;
            break;

        case 4:
            val = sf_readl(REG_CLASS_QOS_PORT45_TC_SEL);
            val &= 0xFFFF0000;
            map &= 0x0000FFFF;
            addr = REG_CLASS_QOS_PORT45_TC_SEL;
            break;

        case 5:
            val = sf_readl(REG_CLASS_QOS_PORT45_TC_SEL);
            val &= 0x0000FFFF;
            map &= 0xFFFF0000;
            addr = REG_CLASS_QOS_PORT45_TC_SEL;
            break;

        default:
            return -1;
			break;
    }

	sf_writel(map|val, addr);
	printk("set TC_Sel value 0x%x, read value is 0x%x\n", map|val, sf_readl(addr));
    return 0;
}

/*provides the TC value to be mapped for each PCP of the packet.*/
int set_pcp2tc_map(int port,int map){
	int addr = 0;
	switch(port){
		case 0:
			addr = REG_CLASS_QOS_PORT0_PCP2TC_MAP;
			break;
		case 1:
			addr = REG_CLASS_QOS_PORT1_PCP2TC_MAP;
			break;
		case 2:
			addr = REG_CLASS_QOS_PORT2_PCP2TC_MAP;
			break;
		case 3:
			addr = REG_CLASS_QOS_PORT3_PCP2TC_MAP;
			break;
		case 4:
			addr = REG_CLASS_QOS_PORT4_PCP2TC_MAP;
			break;
		case 5:
			addr = REG_CLASS_QOS_PORT5_PCP2TC_MAP;
			break;
		default:
			return -1;
			break;
	}
	sf_writel(map, addr);
	printk("set pcp2tc_map 0x%x ,read value is 0x%x\n", map, sf_readl(addr));
	return 0;
}

/*Based on the packet receive Ingress port, the respect PID value is mapped as TC*/
int set_pid2tc_map(int map){
	sf_writel(map, REG_CLASS_QOS_PORT_PID2TC_MAP);
	printk("set map 0x%x to REG_CLASS_QOS_PORT_PID2TC_MAP,read value is 0x%x\n", map, sf_readl(REG_CLASS_QOS_PORT_PID2TC_MAP));
	return 0;
}

int set_tc_sel_one_to_one(int port, int type)
{
	int map = 0;
	switch(type){
		case TC_DSCP:
			map = 0;
			break;
		case TC_PCP:
			map = 0x5555;
			break;
		case TC_PID:
			map = 0xAAAA;
			break;
		case TC_MACDA:
			map = 0xFFFF;
			break;
		default:
			break;
	}

	set_tc_sel(port, map);

	return 0;
}

/*set global management ether type
 *@mgmt_type management ether type, always set to 0x8100
 * */
int set_global_mgmt_port(int port, int mgmt_type)
{
	sf_writel(mgmt_type| (port << GLOBAL_MGMT_PORT_START_POS),
			REG_CLASS_GLOBAL_MGMT_REG);
	printk("set global mgmt_type 0x%x to port address,read value is 0x%x\n", mgmt_type| (port << GLOBAL_MGMT_PORT_START_POS), sf_readl(REG_CLASS_GLOBAL_MGMT_REG));
	return 0;
}

/*set port control*/
int set_port_enable(int port_num,int enable)
{
	int addr2 = get_port_address(port_num,2);
	int val = ((enable -1) < 0) ? 1 : 0;
	int tmp = sf_readl(addr2);
	if(addr2 != 0){
		tmp &= ~PORT_SHUTDOWN_MASK;
		sf_writel( ((val << PORT_SHUTDOWN_START_POS) & PORT_SHUTDOWN_MASK)| tmp, addr2);
		printk("set port control 0x%x to port address,read value is 0x%x\n", ((val << PORT_SHUTDOWN_START_POS) & PORT_SHUTDOWN_MASK)| tmp, sf_readl(addr2));
	}
	return 0;
}

/*set port vlan attribute*/
int set_port_vlan_attr(int port_num, int attr)
{
	int addr2 = get_port_address(port_num,2);
	int tmp = sf_readl(addr2);
	if(addr2 != 0){
		tmp &= ~PORT_AFT_MASK;
		sf_writel( ((attr<< PORT_AFT_START_POS) & PORT_AFT_MASK)| tmp, addr2);
		printk("set port vlan_attr 0x%x to port address,read value is 0x%x\n", ((attr<< PORT_AFT_START_POS) & PORT_AFT_MASK)| tmp, sf_readl(addr2));
	}
	return 0;
}

/*set global snoop ethertype
 *@idx decide snoop ethertype module;
 *@snoop_etype decide snoop ethertype value;
 *@enable enable or shutdown.
 * */
int set_global_snoop_ethertype(int idx, unsigned long long* snoop_etype, int enable)
{
    int read_val = 0, set_val = 0;
    switch ( idx ){
        case SNOOP_IGMP :
            if(enable)
                set_val = 0x1 << SNOOP_IGMP;
            else
                set_val = ~(0x1 << SNOOP_IGMP);
            break;
        case SNOOP_ETYPE0:
            if(enable){
                int read_val = sf_readl(REG_CLASS_SNOOP_SPL_ETYPE_REG01);
                sf_writel( (((*snoop_etype >> 8) & 0x00ff) | ((*snoop_etype << 8) & 0xff00) | (read_val & 0xffff0000)), REG_CLASS_SNOOP_SPL_ETYPE_REG01);
                set_val = 0x1 << SNOOP_ETYPE0;
            }
            else
                set_val = ~(0x1 << SNOOP_ETYPE0);
            break;
        case SNOOP_ETYPE1 :
            if(enable){
                int read_val = sf_readl(REG_CLASS_SNOOP_SPL_ETYPE_REG01);
                sf_writel( (((*snoop_etype >> 8) & 0x00ff) | ((*snoop_etype << 8) & 0xff00) | (read_val & 0x0000ffff)), REG_CLASS_SNOOP_SPL_ETYPE_REG01);
                set_val = 0x1 << SNOOP_ETYPE1;
            }
            else
                set_val = ~(0x1 << SNOOP_ETYPE1);
            break;
        case SNOOP_ETYPE2:
            if(enable){
                sf_writel( (((*snoop_etype >> 8) & 0x00ff) | ((*snoop_etype << 8) & 0xff00)), REG_CLASS_SNOOP_SPL_ETYPE_REG23);
                set_val = 0x1 << SNOOP_ETYPE2;
            }
            else
                set_val = ~(0x1 << SNOOP_ETYPE2);
            break;
        case SNOOP_ETYPE3:
            if(enable){
                sf_writel( ((((*snoop_etype >> 8) & 0x00ff) | ((*snoop_etype << 8) & 0xff00)) << 16), REG_CLASS_SNOOP_SPL_ETYPE_REG23);
                set_val = 0x1 << SNOOP_ETYPE3;
            }
            else
                set_val = ~(0x1 << SNOOP_ETYPE3);
            break;
        case SNOOP_ETYPE_ARP:
            if(enable)
                set_val = 0x1 << SNOOP_ETYPE_ARP;
            else
                set_val = ~(0x1 << SNOOP_ETYPE_ARP);
            break;
        case SNOOP_ETYPE_RARP:

            if(enable)
                set_val = 0x1 << SNOOP_ETYPE_RARP;
            else
                set_val = ~(0x1 << SNOOP_ETYPE_RARP);
            break;

        case SNOOP_HOPOPTS:

            if(enable)
                set_val = 0x1 << SNOOP_HOPOPTS;
            else
                set_val = ~(0x1 << SNOOP_HOPOPTS);
            break;

        case SNOOP_MLDV2:
            if(enable)
                set_val = 0x1 << SNOOP_MLDV2;
            else
                set_val = ~(0x1 << SNOOP_MLDV2);
            break;
        default:
            break;
        }
    if(enable){
        read_val = sf_readl(REG_CLASS_SNOOP_CTRL);
        sf_writel( set_val| read_val, REG_CLASS_SNOOP_CTRL);
		printk("set global snoop ethertype enable/disable 0x%x ,read value is 0x%x\n", set_val| read_val, sf_readl(REG_CLASS_SNOOP_CTRL));
    }
    else{
        read_val = sf_readl(REG_CLASS_SNOOP_CTRL);
        sf_writel( set_val& read_val, REG_CLASS_SNOOP_CTRL);
		printk("set global snoop ethertype enable/disable 0x%x ,read value is 0x%x\n", set_val| read_val, sf_readl(REG_CLASS_SNOOP_CTRL));
    }

    return 0;
}

int set_port_untag(int port_num)
{
	int addr2 = get_port_address(port_num,2);
	if(addr2 != 0){
		int tmp = sf_readl(addr2);
		sf_writel(((0x1 << PORT_UNTAG_FROM_BTABLE_START_POS) & PORT_UNTAG_FROM_BTABLE_MASK) | tmp, addr2);
		printk("set port untag 0x%x ,read value is 0x%x\n", ((0x1 << PORT_UNTAG_FROM_BTABLE_START_POS) & PORT_UNTAG_FROM_BTABLE_MASK) | tmp, sf_readl(addr2));
	}
	return 0;
}

int set_port_vid_prefix(int port_num)
{
	int addr2 = get_port_address(port_num,2);
	if(addr2 != 0){
		int tmp = sf_readl(addr2);
		sf_writel(((0x1 << PORT_VID_PREFIX_START_POS) & PORT_VID_PREFIX_MASK) | tmp, addr2);
		printk("set port vid 0x%x ,read value is 0x%x\n", ((0x1 << PORT_VID_PREFIX_START_POS) & PORT_VID_PREFIX_MASK) | tmp, sf_readl(addr2));
	}
	return 0;
}

int set_bmu_watermark(int bmu_low_watermark, int bmu_high_watermark)
{
	sf_writel(bmu_low_watermark, REG_BMU_LOW_WATERMARK);
	sf_writel(bmu_high_watermark, REG_BMU_HIGH_WATERMARK);
	printk("set bmu_low_watermark 0x%x bmu_high_watermark 0x%x,read value is 0x%x 0x%x\n",
			bmu_low_watermark ,bmu_high_watermark, sf_readl(REG_BMU_LOW_WATERMARK), sf_readl(REG_BMU_HIGH_WATERMARK));

	return 0;
}

int init_gpi_fw_control(int dev_no, int phy_low_watermark, int phy_high_watermark){

	int addr = get_gpi_addr_base(dev_no);

	//enable fw control
	sf_writel(phy_low_watermark, addr + REG_GPI_BMU1_PHY_LOW_WATERMARK_OFFSET);
	sf_writel(phy_high_watermark, addr + REG_GPI_BMU1_PHY_HIGH_WATERMARK_OFFSET);
	printk("set phy_low_watermark 0x%x phy_high_watermark 0x%x,read value is 0x%x 0x%x\n",
			phy_low_watermark ,phy_high_watermark, sf_readl(addr + REG_GPI_BMU1_PHY_LOW_WATERMARK_OFFSET), sf_readl(addr + REG_GPI_BMU1_PHY_HIGH_WATERMARK_OFFSET));

	//npu_sleep_us(1);
	sf_writel(0x00040303, addr + REG_GPI_FW_CONTROL_OFFSET);
	printk("set 0x00040303 to REG_GPI_FW_CONTROL,read value is 0x%x", sf_readl(addr + REG_GPI_FW_CONTROL_OFFSET));

	return 0;
}

/*set mac address*/
int gmac_set_mac_addr1(unsigned long long *mac)
{
	int address_upper = sf_readl(get_mac_address_addr(0, 1));
	int address_lower = (*mac >> 40) | ((*mac >> 24) & 0xff00) | ((*mac >> 8) & 0xff0000) | ((*mac << 8) & 0xff000000);
	address_upper = address_upper & 0xffff0000;
	address_upper |= ((*mac << 8) & 0xff00) | ((*mac >> 8) & 0xff);
	sf_writel(address_upper, get_mac_address_addr(0, 1));
	sf_writel(address_lower, get_mac_address_addr(0, 0));
	return 0;
}
