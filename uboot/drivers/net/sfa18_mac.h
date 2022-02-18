#ifndef SFA18_MAC_H
#define SFA18_MAC_H
#include "sfa18_eth.h"

//OMINI PHY
#define OMINI_PHY_ENABLE                 (SIFLOWER_SYSCTL_BASE + 0x4040)
#define OMINI_PHY_CFG_POWERDOWN          (SIFLOWER_SYSCTL_BASE + 0x4044)
#define OMINI_PHY_CFG_POWERUP_RESET      (SIFLOWER_SYSCTL_BASE + 0x4048)

#define OMINI_PHY1_CFG_ADDR              (SIFLOWER_SYSCTL_BASE + 0x404C)
#define OMINI_PHY2_CFG_ADDR              (SIFLOWER_SYSCTL_BASE + 0x4050)
#define OMINI_PHY3_CFG_ADDR              (SIFLOWER_SYSCTL_BASE + 0x4054)
#define OMINI_PHY4_CFG_ADDR              (SIFLOWER_SYSCTL_BASE + 0x4058)
#define OMINI_PHY5_CFG_ADDR              (SIFLOWER_SYSCTL_BASE + 0x405C)

#define OMINI_PHY1_CFG_FX_PLL_MODE       (SIFLOWER_SYSCTL_BASE + 0x4060)
#define OMINI_PHY2_CFG_FX_PLL_MODE       (SIFLOWER_SYSCTL_BASE + 0x4064)
#define OMINI_PHY3_CFG_FX_PLL_MODE       (SIFLOWER_SYSCTL_BASE + 0x4068)
#define OMINI_PHY4_CFG_FX_PLL_MODE       (SIFLOWER_SYSCTL_BASE + 0x406C)
#define OMINI_PHY5_CFG_FX_PLL_MODE       (SIFLOWER_SYSCTL_BASE + 0x4070)

#define OMINI_PHY_CFG_CLK_FREQ           (SIFLOWER_SYSCTL_BASE + 0x4074)
#define OMINI_PHY_CFG_CLK_REF_SEL        (SIFLOWER_SYSCTL_BASE + 0x4078)

#define OMINI_PHY1_CFG_PHY_ID_LOW8       (SIFLOWER_SYSCTL_BASE + 0x407C)
#define OMINI_PHY1_CFG_PHY_ID_HIGH8      (SIFLOWER_SYSCTL_BASE + 0x4080)
#define OMINI_PHY1_CFG_MODEL_NR          (SIFLOWER_SYSCTL_BASE + 0x4084)
#define OMINI_PHY1_CFG_REV_NR            (SIFLOWER_SYSCTL_BASE + 0x4088)

#define OMINI_PHY2_CFG_PHY_ID_LOW8       (SIFLOWER_SYSCTL_BASE + 0x408C)
#define OMINI_PHY2_CFG_PHY_ID_HIGH8      (SIFLOWER_SYSCTL_BASE + 0x4090)
#define OMINI_PHY2_CFG_MODEL_NR          (SIFLOWER_SYSCTL_BASE + 0x4094)
#define OMINI_PHY2_CFG_REV_NR            (SIFLOWER_SYSCTL_BASE + 0x4098)

#define OMINI_PHY3_CFG_PHY_ID_LOW8       (SIFLOWER_SYSCTL_BASE + 0x409C)
#define OMINI_PHY3_CFG_PHY_ID_HIGH8      (SIFLOWER_SYSCTL_BASE + 0x40A0)
#define OMINI_PHY3_CFG_MODEL_NR          (SIFLOWER_SYSCTL_BASE + 0x40A4)
#define OMINI_PHY3_CFG_REV_NR            (SIFLOWER_SYSCTL_BASE + 0x40A8)

#define OMINI_PHY4_CFG_PHY_ID_LOW8       (SIFLOWER_SYSCTL_BASE + 0x40AC)
#define OMINI_PHY4_CFG_PHY_ID_HIGH8      (SIFLOWER_SYSCTL_BASE + 0x40B0)
#define OMINI_PHY4_CFG_MODEL_NR          (SIFLOWER_SYSCTL_BASE + 0x40B4)
#define OMINI_PHY4_CFG_REV_NR            (SIFLOWER_SYSCTL_BASE + 0x40B8)

#define OMINI_PHY5_CFG_PHY_ID_LOW8       (SIFLOWER_SYSCTL_BASE + 0x40BC)
#define OMINI_PHY5_CFG_PHY_ID_HIGH8      (SIFLOWER_SYSCTL_BASE + 0x40C0)
#define OMINI_PHY5_CFG_MODEL_NR          (SIFLOWER_SYSCTL_BASE + 0x40C4)
#define OMINI_PHY5_CFG_REV_NR            (SIFLOWER_SYSCTL_BASE + 0x40C8)
//define in base/base_code/common/sys_manager.c
////[4:0] PHY ADD register field
#define OMINI_PHY1_ADDR (0x10)
#define OMINI_PHY2_ADDR (0x11)
#define OMINI_PHY3_ADDR (0x12)
#define OMINI_PHY4_ADDR (0x13)
#define OMINI_PHY5_ADDR (0x14)
////PHY ID Number[21:0]
#define OMINI_PHY_ID_1  (0x0F1F1F)
#define OMINI_PHY_ID_2  (0x0F2F2F)
#define OMINI_PHY_ID_3  (0x0F3F3F)
#define OMINI_PHY_ID_4  (0x0F4F5F)
#define OMINI_PHY_ID_5  (0x0F5F5F)
#define PHY_IDENTIFY_1                 0x02
#define PHY_IDENTIFY_2                 0x03
#define OMINI_PHY_CFG_MODE 0xE0
//Manufacturer’s Model Number[5:0]
#define OMINI_PHY_MODEL_NUMBER (0x1F)
////Manufacturer’s Revision Number[3:0]
#define OMINI_PHY_REV_NUMBER (0x01)
//for Register 4 (GMII Address Register)
//if GMII Busy
#define GMII_ADDR_MASK_GB (1 << 0)
//GMII Write or read(0)
#define GMII_ADDR_MASK_GW (1 << 1)
//CSR Clock Range
#define GMII_ADDR_MASK_CR (0xf << 2)
//GMII Register
#define GMII_ADDR_MASK_GR (0x1f << 6)
//Physical Layer Address
#define GMII_ADDR_MASK_PA (0x1f << 11)

#define GMII_ADDR_VALUE_GW_READ   0 << 1
#define GMII_ADDR_VALUE_GW_WRITE  1 << 1
#define GMII_DATA_MASK  0xffff

#define DEFAULT_PHY_VALUE 0xffffffff
void sys_omini_phy_reset(void);
void sf_mac_enable(void);
void sf_mac_disable_rx(void);
void sf_mac_disable_tx(void);
void write_phy_reg(int phy_no,int phy_reg,int value, struct phy_device* phy_dev);
void sf_phy_init(struct phy_device* phy_dev);

#ifdef CONFIG_SFA18_PCBA_TEST
#define PHY_STATUS_LINK_UP              (1 << 2)
#define PHY_BASE_ENABLE_AUTO_NEG        (1 << 12)
#define OMINI_PHY_MODEL_CTRL_MDIX       (1 << 6)
#define OMINI_PHY_MODEL_CTRL_MDI        (0 << 6)
#define OMINI_PHY_MODEL_CTRL_AUTO       0x80
#define BRENTRY_UCAST_MISS_ACT_START_POS      22
#define BRENTRY_MCAST_MISS_ACT_START_POS      25
#define OMINI_PHY_MODEL_CTRL_ENERGY_DETECT    0x2000
#define PHY_BASE_STATUS_REGISTER        0x01
#define PHY_BASE_CONTROL_REGISTER       0x00
#define OMINI_PHY_MODEL_CTRL_REGISTER   0x11
#define OMINI_PHY_MODEL_CTRL_MDI_MASK   0xc0

int check_phy_linkup(int phy_no, struct sf_priv *priv);
void sf_phy_mode_init(int phy_num, struct phy_device* phy_dev, int mode);
int sf_phy_reset_for_low_power(void);
#endif
#endif
