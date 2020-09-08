#ifndef SF_MDIO_H
#define SF_MDIO_H
#include <linux/phy.h>

#define DEFAULT_PHY_VALUE 0xffffffff
//[4:0] PHY ADD register field
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
#define OMINI_PHY_REV_NUMBER				(0x01)

#define OMINI_PHY_CFG_MODE					0xE0

#define MAVELL_PHY_SPEED_MASK				0x2040
#define MAVELL_PHY_SPEED_1000m				0x0040
#define MAVELL_PHY_SPEED_100m				0x2000
#define MAVELL_PHY_SPEED_10m				0x0000

//for PHY register
#define PHY_BASE_CONTROL_REGISTER			0x00
#define PHY_BASE_STATUS_REGISTER			0x01
#define PHY_IDENTIFY_1						0x02
#define PHY_IDENTIFY_2						0x03
#define PHY_AUTO_NEGOTIATION_ADEWS			0x04
#define PHY_AUTO_NEGOTIATION_LINK_PARTNER	0x05
//special for ominiphy(reg 17)
#define OMINI_PHY_MODEL_CTRL_REGISTER		0x11

#define OMINI_PHY_TSTCNTL_REGISTER			0x14
#define OMINI_PHY_TSTREAD_REGISTER			0x15
#define OMINI_PHY_TSTWRITE_REGISTER			0x17

#define OMINI_PHY_INTSRC_REGISTER			0x1d
#define OMINI_PHY_INTMASK_REGISTER			0x1e
#define OMINI_PHY_SPECIAL_CONTROL_STATUS	0x1f
#define PHY_INT_ENABLE_ALL_NORMAL			0xFF


#define OMINI_PHY_TSTCNTL_ENABLE			0x400
#define OMINI_PHY_TSTCNTL_READ				0x8000
#define OMINI_PHY_TSTCNTL_WRITE				0x4000
#define OMINI_PHY_TSTCNTL_MODE_ANA          0x0
#define OMINI_PHY_TSTCNTL_MODE_LPI			0x800
#define OMINI_PHY_TSTCNTL_READ_ADDR_MASK	0x3E0
#define OMINI_PHY_TSTCNTL_WRITE_ADDR_MASK	0x1F


#define OMINI_PHY_ANALOG_A5_CONFIG_REGISTER       0x16
#define OMINI_PHY_ANALOG_A5_CONFIG_TX_IDLE        0x487
#define OMINI_PHY_ANALOG_A5_CONFIG_TX_10M_MDI     0x471
#define OMINI_PHY_ANALOG_A5_CONFIG_TX_10M_MDIX    0x571
#define OMINI_PHY_ANALOG_A5_CONFIG_TX_ON		  0x406

//phy control register value mask
//1 = software reset. Bit is self-clearing
#define PHY_BASE_ENABLE_RESET            (1 << 15)
//1 = loopback mode, 0 = normal operation
#define PHY_BASE_ENABLE_LOOPBACK         (1 << 14)
//1 = 100Mbps, 0 = 10Mbps Ignored if Auto Negotiation is enable#define PHY_BASE_ENABLE_RESET       (1 << 15)
#define PHY_BASE_ENABLE_SPEED_100        (1 << 13)
//1 = enable auto-negotiate process (overrides 0.13 and 0.8)
#define PHY_BASE_ENABLE_AUTO_NEG         (1 << 12)
//1 = General power down mode, 0 = norma
#define PHY_BASE_ENABLE_POWER_DOWN       (1 << 11)
//1 = electrical isolation of PHY from MII 0 = normal operation
#define PHY_BASE_ENABLE_ISOLATE          (1 << 10)
//1 = restart auto-negotiate process Bit is self-clearing.
#define PHY_BASE_ENABLE_RESTART_NEG      (1 << 9)
//Duplex Mode 1 = full duplex, 0 = half duplex
#define PHY_BASE_ENABLE_DUPLEX_FULL           (1 << 8)
//1 = enable Collision test, 0 = disable COL test
#define PHY_BASE_ENABLE_COLTEST          (1 << 7)

//phy status register value mask
#define PHY_STATUS_LINK_UP               (1 << 2)

//ominiphy mode control/status reg-17
//MDI mode
#define OMINI_PHY_MODEL_CTRL_MDI_MASK    0xc0
#define OMINI_PHY_MODEL_CTRL_FIX_MDI     0x00
#define OMINI_PHY_MODEL_CTRL_FIX_MDIX    0x40
//bit 14 of register 17 can speed up link step of 10M
#define OMINI_PHY_MODEL_CTRL_10M_SPEEDUP 0x4000
#define OMINI_PHY_MODEL_CTRL_AUTO        0x80
//Auto-Negotiation Test Mode
#define OMINI_PHY_MODEL_CTRL_AEG_TEST_ENABLE	0x0100

#define OMINI_PHY_MODEL_CTRL_FAREND_LOOPBACK	0x0200
#define OMINI_PHY_MODEL_CTRL_ENERGY_DETECT		0x2000
#define OMINI_PHY_MODEL_CTRL_ENERGY_ON			0x2

//special for Marvell phy
#define MIIM_88E1111_PHY_PAGE 22
#define MIIM_88E1111_PHY_SPL_STATUS_REG 0x11
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

#define MAVELL_PHY_SPEED_MASK 0x2040
#define MAVELL_PHY_SPEED_1000m 0x0040
#define MAVELL_PHY_SPEED_100m  0x2000
#define MAVELL_PHY_SPEED_10m   0x0000
	//emac special control reg
	//rx package drop enable for watchdog timeout error
#define EMAC_CONTROL_DROP_RX_WATCHDOG (0x1 << 17)
//rx package drop enable for runt frame
#define EMAC_CONTROL_DROP_RUNT (0x1 << 18)
//rx package drop enable for giant frame
#define EMAC_CONTROL_DROP_GIANT (0x1 << 19)
//rx package drop enable for late collision seen
#define EMAC_CONTROL_DROP_LATECOL (0x1 << 20)
//rx package drop enable for gmii error
#define EMAC_CONTROL_DROP_GMII_ERR (0x1 << 21)
//rx package drop enable for crc error
#define EMAC_CONTROL_DROP_CRC_ERR (0x1 << 22)
//rx package drop enable for length error
#define EMAC_CONTROL_DROP_LENGTH_ERR (0x1 << 23)
//rx package drop enable for da filter fail
#define EMAC_CONTROL_DROP_DA_FAIL (0x1 << 24)
//rx package drop enable for sa filter fail
#define EMAC_CONTROL_DROP_SA_FAIL (0x1 << 25)
//rx package drop enable for ip header error
#define EMAC_CONTROL_DROP_IP_CHSUM (0x1 << 26)
//rx package drop enable for ip payload error
#define EMAC_CONTROL_DROP_PAYLOAD_CHSUM (0x1 << 27)
#define MAC_CFG_TX_EN (1 << 3)
#define MAC_CFG_RX_EN (1 << 2)

#define MAC_CFG_FES_100 (1<<14)
#define MAC_CFG_DUPLEX_FULL (1 << 11)
#define MAC_CFG_DIS_REC_OWN (1 << 13)

#define ETH_SPEED_AUTO  2
#define ETH_SPEED_10    0
#define ETH_SPEED_100   1
//eth duplex for mac/phy
#define ETH_DUPLEX_FULL 1
#define ETH_DUPLEX_HALF 0

extern spinlock_t phy_lock[5];

#define sf_try_lock_phy(a)   spin_trylock(&phy_lock[a])
#define sf_try_unlock_phy(a)   spin_unlock(&phy_lock[a])
#define sf_lock_phy(a)   spin_lock_bh(&phy_lock[a])
#define sf_unlock_phy(a)  spin_unlock_bh(&phy_lock[a])
#define sf_lock_phy_irqsave(a, b)   spin_lock_irqsave(&phy_lock[a], b)
#define sf_unlock_phy_irqsave(a, b)  spin_unlock_irqrestore(&phy_lock[a], b)

void sf_phy_init(struct phy_device *phy_dev);

void sf_mac_enable(unsigned int port_list);
void sf_mac_disable_tx(unsigned int port_list);
void sf_mac_disable_rx(unsigned int port_list);

int get_mac_cfg_addr(int mac_no);
int get_mac_address_addr(int mac_no, int type);

void write_phy_reg(int phy_no,int phy_reg,int value, struct phy_device* phy_dev, int lock);

int read_phy_reg(int phy_no,int phy_reg, struct phy_device* phy_dev, int lock);

int read_phy_reg_direct(int phy_no,int phy_addr,int phy_reg, int lock);

void sys_omini_phy_reset(void);
int phy_scan(struct phy_device* phy_dev);

#ifdef CONFIG_SFAX8_SWITCH_POWERSAVE
void sf_phy_energy_detect_enable(int phy_no, struct phy_device* phy_dev);
void sf_phy_energy_detect_disable(int phy_no, struct phy_device* phy_dev);
void sf_phy_energy_detect_init(struct sf_priv *priv);
#endif


int check_phy_linkup(int phy_no, struct sf_priv *priv, int lock);
int get_link_speed(int phy_no, struct sf_priv *priv);
int get_link_duplex(int phy_no, struct sf_priv *priv);
int sf_mac_set_link(int port, int speed, int duplex);

void sf_phy_re_auto(struct sf_priv *priv , unsigned int port, int lock);
#ifdef SF_TX_SHUTDOWN
int sf_phy_reset(int phy_idx, struct sf_priv *priv);
int sf_phy_a5_tx_config(int phy_idx, struct sf_priv *priv, unsigned int value, int lock);
void sf_phy_init_low_power(struct sf_priv *priv, unsigned int port);
void sf_phy_init_normal(struct sf_priv *priv, unsigned int port);
int sf_phy_init_test_mode(int phy_idx, struct sf_priv *priv, int lock);
#endif
#endif /* ifndef SF_MDIO_H */
