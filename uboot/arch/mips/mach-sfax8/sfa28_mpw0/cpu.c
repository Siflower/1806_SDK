#include <common.h>
#include <asm/io.h>
#include <mach/sfax8.h>

//[4:0] PHY ADD register field
#define OMINI_PHY1_ADDR (0x10)
#define OMINI_PHY2_ADDR (0x11)
#define OMINI_PHY3_ADDR (0x12)
#define OMINI_PHY4_ADDR (0x13)
#define OMINI_PHY5_ADDR (0x14)

//PHY ID Number[21:0]
#define OMINI_PHY_ID_1  (0x0F1F1F)
#define OMINI_PHY_ID_2  (0x0F2F2F)
#define OMINI_PHY_ID_3  (0x0F3F3F)
#define OMINI_PHY_ID_4  (0x0F4F5F)
#define OMINI_PHY_ID_5  (0x0F5F5F)

//Manufacturer’s Model Number[5:0]
#define OMINI_PHY_MODEL_NUMBER (0x1F)
//Manufacturer’s Revision Number[3:0]
#define OMINI_PHY_REV_NUMBER (0x01)

//OMINIPHY cfg_mode defination
#define OMINI_PHY_CFG_MODE_AUTO_FULL 0xE0
#define OMINI_PHY_CFG_MODE_10M_FULL 0x20
#define OMINI_PHY_CFG_MODE_100M_FULL 0x60
#define OMINI_PHY_CFG_MODE_10M_HALF 0x00
#define OMINI_PHY_CFG_MODE_100M_HALF 0x40
#define OMINI_PHY_CFG_MODE_AUTO_HALF 0x80
#define OMINI_PHY_CFG_MODE_REPEATER_100M_HALF 0xa0
#define OMINI_PHY_CFG_MODE_POWERDOWN 0xc0


#define OMINI_PHY_DUPLEX_FULL 0
#define OMINI_PHY_DUPLEX_HALF 1

#define OMINI_SPEED_100   0
#define OMINI_SPEED_10    1
#define OMINI_SPEED_AUTO  2

void sys_omini_phy_reset(void)
{
    //emac2_cfg_np_msg_code[2:0]
    //emac[5-1]_cfg_power_down[4:0] Default value of True power down bit (Reg24.0)
    writeb(0x0,(void*)(OMINI_PHY_CFG_POWERDOWN));
    //emac3_cfg_np_msg_code[2:0]
    //emac[5-1]_cfg_powerup_reset[4:0] Power Up Reset bit(Reg24.1)
    writeb(0x0,(void*)(OMINI_PHY_CFG_POWERUP_RESET));
    //emac1_cfg_mode[2:0] emac1_cfg_phy_addr[4:0]
    //001 - 10Base-T, Full Duplex, Auto negotiation disabled 0x20
    //111 - All capable, Full Duplex, 10 & 100 BT, Auto negotiation enabled AutoMDIX enable --0xE0
    //011 - 100Base-TX, Full Duplex, Auto-negotiation disabled --0x60
    //000 - 10BaseT, Half Duplex, Auto negotiation disabled
    //010 - 100Base-TX, Half Duplex, Auto-negotiation disabled
    //100 - 100Base-Tx, Half Duplex, Auto-negotaition Enabled
    //101 - Repeater mode,100Base-Tx, Half Duplex,Auto-negotiation Enabled
    //110 - Power down mode,In this mode phy wake up in power down mode

    int phy_cfg_mode = OMINI_PHY_CFG_MODE_AUTO_FULL;
#ifdef NPU_ETH_DUPLEX
    int phy_duplex = NPU_ETH_DUPLEX;
#else
    int phy_duplex = OMINI_PHY_DUPLEX_FULL;
#endif

#ifdef NPU_ETH_SPEED
    int phy_speed = NPU_ETH_SPEED;
#else
    int phy_speed = OMINI_SPEED_AUTO;
#endif

    if(phy_duplex == OMINI_PHY_DUPLEX_FULL){
        if(phy_speed == OMINI_SPEED_100) phy_cfg_mode = OMINI_PHY_CFG_MODE_100M_FULL;
        if(phy_speed == OMINI_SPEED_10) phy_cfg_mode = OMINI_PHY_CFG_MODE_10M_FULL;
        if(phy_speed == OMINI_SPEED_AUTO) phy_cfg_mode = OMINI_PHY_CFG_MODE_AUTO_FULL;
    }else{
        if(phy_speed == OMINI_SPEED_100) phy_cfg_mode = OMINI_PHY_CFG_MODE_100M_HALF;
        if(phy_speed == OMINI_SPEED_10) phy_cfg_mode = OMINI_PHY_CFG_MODE_10M_HALF;
        if(phy_speed == OMINI_SPEED_AUTO) phy_cfg_mode = OMINI_PHY_CFG_MODE_AUTO_HALF;
    }

    writeb(OMINI_PHY1_ADDR | phy_cfg_mode,(void*)(OMINI_PHY1_CFG_ADDR));
    writeb(OMINI_PHY2_ADDR | phy_cfg_mode,(void*)(OMINI_PHY2_CFG_ADDR));
    writeb(OMINI_PHY3_ADDR | phy_cfg_mode,(void*)(OMINI_PHY3_CFG_ADDR));
    writeb(OMINI_PHY4_ADDR | phy_cfg_mode,(void*)(OMINI_PHY4_CFG_ADDR));
    writeb(OMINI_PHY5_ADDR | phy_cfg_mode,(void*)(OMINI_PHY5_CFG_ADDR));
    //OMINI_PHY1_CFG_FX_PLL_MODE
    /*
    {
        emac2_cfg_automdix_en, 1
        emac1_cfg_en_high,  0
        emac1_cfg_fx_mode,  0
        emac1_cfg_adc_bp,   0            bypass mode. (Reg18.8)
        emac1_cfg_pll_bp,   0            bypass mode (Reg18.9)
        emac1_cfg_smii_source_sync, 0    smii source sync register Only relevant for SMII mode
        emac1_cfg_mii_mode[1:0],  00           “00” for MII mode,"01" for RMII mode,"10" for SMII,"11" reserved
    }
    */
    writeb(0x80,(void*)(OMINI_PHY1_CFG_FX_PLL_MODE));
    writeb(0x80,(void*)(OMINI_PHY2_CFG_FX_PLL_MODE));
    writeb(0x80,(void*)(OMINI_PHY3_CFG_FX_PLL_MODE));
    writeb(0x80,(void*)(OMINI_PHY4_CFG_FX_PLL_MODE));
    writeb(0x80,(void*)(OMINI_PHY5_CFG_FX_PLL_MODE));
    //emac4_cfg_np_msg_code[2:0]
    //emac[5-1]_cfg_clk_freq
    //‘0’ for 25 MHz clock input; ‘1’ for 50 MHz clock input.
    writeb(0x00,(void*)(OMINI_PHY_CFG_CLK_FREQ));
    //emac5_cfg_np_msg_code[2:0]
    //emac[5-1]_cfg_ref_clk_sel(NA now)
    writeb(0x00,(void*)(OMINI_PHY_CFG_CLK_REF_SEL));

    //emac1_cfg_phy_id[7:0]
    writeb(OMINI_PHY_ID_1 & 0xFF,(void*)(OMINI_PHY1_CFG_PHY_ID_LOW8));
    //emac1_cfg_phy_id[15:8]
    writeb(((OMINI_PHY_ID_1 & 0xFF00) >> 8),(void*)(OMINI_PHY1_CFG_PHY_ID_HIGH8));
    //{emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]}
    writeb(((OMINI_PHY_ID_1 & 0x3F0000) >> 16) | ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6),(void*)(OMINI_PHY1_CFG_MODEL_NR));
    //{emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]}
    writeb((OMINI_PHY_REV_NUMBER << 4) | ((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2),(void*)(OMINI_PHY1_CFG_REV_NR));


    //emac1_cfg_phy_id[7:0]
    writeb(OMINI_PHY_ID_2 & 0xFF,(void*)(OMINI_PHY2_CFG_PHY_ID_LOW8));
    //emac1_cfg_phy_id[15:8]
    writeb(((OMINI_PHY_ID_2 & 0xFF00) >> 8),(void*)(OMINI_PHY2_CFG_PHY_ID_HIGH8));
    //{emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]}
    writeb(((OMINI_PHY_ID_2 & 0x3F0000) >> 16) | ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6),(void*)(OMINI_PHY2_CFG_MODEL_NR));
    //{emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]}
    writeb((OMINI_PHY_REV_NUMBER << 4) | ((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2),(void*)(OMINI_PHY2_CFG_REV_NR));


    //emac1_cfg_phy_id[7:0]
    writeb(OMINI_PHY_ID_3 & 0xFF,(void*)(OMINI_PHY3_CFG_PHY_ID_LOW8));
    //emac1_cfg_phy_id[15:8]
    writeb(((OMINI_PHY_ID_3 & 0xFF00) >> 8),(void*)(OMINI_PHY3_CFG_PHY_ID_HIGH8));
    //{emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]}
    writeb(((OMINI_PHY_ID_3 & 0x3F0000) >> 16) | ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6),(void*)(OMINI_PHY3_CFG_MODEL_NR));
    //{emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]}
    writeb((OMINI_PHY_REV_NUMBER << 4) | ((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2),(void*)(OMINI_PHY3_CFG_REV_NR));

    //emac1_cfg_phy_id[7:0]
    writeb(OMINI_PHY_ID_4 & 0xFF,(void*)(OMINI_PHY4_CFG_PHY_ID_LOW8));
    //emac1_cfg_phy_id[15:8]
    writeb(((OMINI_PHY_ID_4 & 0xFF00) >> 8),(void*)(OMINI_PHY4_CFG_PHY_ID_HIGH8));
    //{emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]}
    writeb(((OMINI_PHY_ID_4 & 0x3F0000) >> 16) | ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6),(void*)(OMINI_PHY4_CFG_MODEL_NR));
    //{emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]}
    writeb((OMINI_PHY_REV_NUMBER << 4) | ((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2),(void*)(OMINI_PHY4_CFG_REV_NR));

    //emac1_cfg_phy_id[7:0]
    writeb(OMINI_PHY_ID_5 & 0xFF,(void*)(OMINI_PHY5_CFG_PHY_ID_LOW8));
    //emac1_cfg_phy_id[15:8]
    writeb(((OMINI_PHY_ID_5 & 0xFF00) >> 8),(void*)(OMINI_PHY5_CFG_PHY_ID_HIGH8));
    //{emac1_cfg_model_nr[1:0],emac1_cfg_phy_id[21:16]}
    writeb(((OMINI_PHY_ID_5 & 0x3F0000) >> 16) | ((OMINI_PHY_MODEL_NUMBER & 0x3) << 6),(void*)(OMINI_PHY5_CFG_MODEL_NR));
    //{emac1_cfg_rev_nr[3:0],emac1_cfg_model_nr[5:2]}
    writeb((OMINI_PHY_REV_NUMBER << 4) | ((OMINI_PHY_MODEL_NUMBER & 0x3C) >> 2),(void*)(OMINI_PHY5_CFG_REV_NR));
}

void sys_npu_reset(void)
{
	//reset npu system manager module
	writew(0x0,(void*)(NPU_SYSM_RESET));
	writew(0x1,(void*)(NPU_SYSM_RESET));

	//enable npu clk
	writew(0xFF,(void*)(NPU_SOFT_CLK_EN));

    //hardward reset npu
    writew(0xFF,(void*)(NPU_SOFT_RESET));
#ifndef FPGA
    //emac[5-1]_phy_enable[4:0]
    writeb(0x1F,(void*)(OMINI_PHY_ENABLE));
    sys_omini_phy_reset();
#endif
    writew(0x00,(void*)(NPU_SOFT_RESET));

    //we can hardware reset the phy by single with the 4 bit of NPU_SOFT_RESET
#if 0
    writew(0x10,(void*)(NPU_SOFT_RESET));
    timer_count_down(0, 0, USER, 0, 10, 0);
    timer_count_down_over(0, 0);
    timer_disable(0, 0);
    writew(0x00,(void*)(NPU_SOFT_RESET));
    timer_count_down(0, 0, USER, 0, 10, 0);
    timer_count_down_over(0, 0);
    timer_disable(0, 0);
#endif

    //set npu boe
	writew(0x0F,(void*)(NPU_SOFT_BOE));
}

/* arch specific CPU init */
int lowlevel_init(void)
{
	/* nothing to do */
	return 0;
}
