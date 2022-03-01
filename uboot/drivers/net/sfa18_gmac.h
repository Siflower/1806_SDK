#ifndef SFA18_GMAC_H
#define SFA18_GMAC_H

#include <common.h>
#include <malloc.h>
#include <net.h>
#include <config.h>
#include <usb_ether.h>
#include <phy.h>
#include <mach/sfax8.h>
#include <asm/gpio.h>


// gmac delay mtd read/write size
#define GMAC_DELAY_READ_SIZE 4
#define GMAC_DELAY_READ_OFFSET 178

#ifdef CONFIG_SPI_NAND
#define SF_FACTORY_SIZE 0x20000
#define SF_FACTORY_OFFSET 0xa0000
#else
#define SF_FACTORY_SIZE 4096
#ifdef CONFIG_SFA18_UBOOT_LITE
#define SF_FACTORY_OFFSET 0x31000
#else
#define SF_FACTORY_OFFSET 0x90000
#endif
#endif


#define GMAC_ADDR_BASE 0xB0800000

#define PHY_IDENTIFY_1                              0x02
#define PHY_IDENTIFY_2                              0x03

/* Bit: 'PACT' */
/* Description: 'PHY Active Status' */
#define MAC_PSTAT_PACT_OFFSET   0x900
#define MAC_PSTAT_PACT_SHIFT    11
#define MAC_PSTAT_PACT_SIZE     1
/* Bit: 'ADDR' */
/* Description: 'PHY Address' */
#define PHY_ADDR_0_ADDR_OFFSET 0xF415
#define PHY_ADDR_0_ADDR_SHIFT  0
#define PHY_ADDR_0_ADDR_SIZE  5
#define PHY_ADDR_0_REG_SIZE  16
/* Bit: 'LPIEN' */
/* Description: 'LPI Mode Enable' */
#define MAC_CTRL_4_LPIEN_OFFSET 0x907
#define MAC_CTRL_4_LPIEN_SHIFT  7
#define MAC_CTRL_4_LPIEN_SIZE       1
/*PHY Reg 0x4 */
#define PHY_AN_ADV_10HDX 0x20
#define PHY_AN_ADV_10FDX 0x40
#define PHY_AN_ADV_100HDX 0x80
#define PHY_AN_ADV_100FDX 0x100
/*PHY Reg 0x9 */
#define PHY_AN_ADV_1000HDX 0x100
#define PHY_AN_ADV_1000FDX 0x20
/* Bit: 'TXDLY' */
/* Description: 'Configure Transmit Clock Delay' */
#define PCDU_5_TXDLY_OFFSET	0xF101
#define PCDU_5_TXDLY_SHIFT	0
#define PCDU_5_TXDLY_SIZE	3
/* Bit: 'RXDLY' */
/* Description: 'Configure Transmit Clock Delay' */
#define PCDU_5_RXDLY_OFFSET	0xF101
#define PCDU_5_RXDLY_SHIFT	7
#define PCDU_5_RXDLY_SIZE	3
/* Bit: 'VLAN' */
/* Description: 'VLAN-aware Switching' */
#define PCE_GCTRL_0_VLAN_OFFSET 0x456
#define PCE_GCTRL_0_VLAN_SHIFT  14
#define PCE_GCTRL_0_VLAN_SIZE       1


//for Register 4 (GMII Address Register)
////if GMII Busy
#define GMII_ADDR_MASK_GB (1 << 0)
////GMII Write(1) or read(0)
#define GMII_ADDR_VALUE_GW_WRITE  (1 << 1)
////CSR Clock Range
#define GMII_ADDR_MASK_CR (0xf << 2)
////GMII Register
#define GMII_ADDR_MASK_GR (0x1f << 6)
////Physical Layer Address
#define GMII_ADDR_MASK_PA (0x1f << 11)
//
#define GMII_DATA_MASK  0xffff

#define IP10XX_SPEC_CTRL_STATUS      16
#define IP1001_RXPHASE_SEL       (1 << 0)
struct sgmac_dma_desc {
	__le32 flags;		// desc0
	__le32 buf_size;	// desc1
	__le32 buf1_addr;	// desc2	/* Buffer 1 Address Pointer */
	__le32 buf2_addr;	// desc3	/* Buffer 2 Address Pointer */
	__le32 ext_status;	// desc4
	__le32 res;		// desc5
	__le32 timestamp_low;	// desc6
	__le32 timestamp_high;	// desc7
};

struct sgmac_priv {
	struct sgmac_dma_desc *dma_rx; // the whole list of rx desc
	char *dma_rx_alloc; // the whole list of rx desc
	unsigned int rx_index; // next rx desc owned by dma
	char * rx_buf_alloc;
	char * rx_buf;

	struct sgmac_dma_desc *dma_tx; // the whole list of tx desc
	char* dma_tx_alloc; // the whole list of tx desc
	unsigned int tx_index; // next tx desc owned by dma
	int tx_irq_cnt; // when we xmit 32 frames, we will set tx irq
	char * tx_buf_alloc;
	char * tx_buf;

	void __iomem *base;
	unsigned int dma_buf_sz;
	dma_addr_t dma_rx_phy; // first rx desc address
	dma_addr_t dma_tx_phy; // first tx desc address

	int hwts_tx_en;
	int hwts_rx_en;

	int gswitch;
	int max_speed;

	int link;   // PHY's last seen link state, 0 is no link and 1 is link.
	int speed;  // PHY's last set duplex mode.
	int duplex; // PHY's last set speed.
	struct phy_device *phydev;
	struct mii_dev *bus;

#ifdef CONFIG_SFAX8_RGMII_GMAC
	int gpio51;
	int gpio52;
#endif
};

#define SGMAC_REG_SPACE_SIZE		0x1058
/* GMAC Register list */
#define GMAC_CONTROL			0x00000000	/* Register 0 (MAC Configuration Register)*/
#define GMAC_FRAME_FILTER		0x00000004	/* Register 1 (MAC Frame Filter) */
//we define DWC_GMAC_HASH_TABLE 256 so we use GMAC_HASH(7:0) and don't use GMAC_HASH_HIGH and GMAC_HASH_LOW.
#define GMAC_HASH_HIGH			0x00000008	/* Register 2 (Hash Table High Register) */
#define GMAC_HASH_LOW			0x0000000c	/* Register 3 (Hash Table Low Register) */
#define GMAC_GMII_ADDR			0x00000010	/* Register 4 (GMII Address Register) */
#define GMAC_GMII_DATA			0x00000014	/* Register 5 (GMII Data Register) */
#define GMAC_FLOW_CTRL			0x00000018	/* Register 6 (Flow Control Register) */
#define GMAC_VLAN_TAG			0x0000001C	/* Register 7 (VLAN Tag Register) */
#define GMAC_VERSION			0x00000020	/* Register 8 (Version Register) */
#define GMAC_DEBUG			0x00000024	/* Register 9 (Debug Register) */
#define GMAC_REMOTE_WAKE		0x00000028	/* Register 10 (Remote Wake-Up Frame Filter Register) */
#define GMAC_PMT			0x0000002c	/* Register 11 (PMT Control and Status Register) */
#define GMAC_LPI_CTRL			0x00000030	/* Register 12 (LPI Control and Status Register) */
#define GMAC_LPI_TIMER			0x00000034	/* Register 13 (LPI Timers Control Register) */
#define GMAC_INT_STAT			0x00000038	/* Register 14 (Interrupt Status Register) */
#define GMAC_INT_MASK			0x0000003c	/* Register 15 (Interrupt Mask Register) */
#define GMAC_LINK_STATUS		0x000000D8	/* Register 54 (SGMII/RGMII/SMII Control and Status Register) */

/*Register 16 (MAC Address0 High Register) to Register 47 (MAC Address15 Low Register) and
 *Register 512 (MAC Address16 High Register) to Register 553 (MAC Address31 Low Register) */
#define GMAC_ADDR_HIGH(reg)     ((reg) <= 15? 0x00000040 + (reg) * 8:0x00000720 + (reg) * 8)
#define GMAC_ADDR_LOW(reg)      ((reg) <= 15? 0x00000044 + (reg) * 8:0x00000724 + (reg) * 8)

#if 0
#define GMAC_ADDR_HIGH0			0x00000040					/* Register 16 (MAC Address0 High Register) */
#define GMAC_ADDR_LOW0			0x00000044					/* Register 17 (MAC Address0 Low Register) */
#define GMAC_ADDR_HIGH1			0x00000048					/* Register 18 (MAC Address1 High Register) */
#define GMAC_ADDR_LOW1			0x0000004c					/* Register 19 (MAC Address1 Low Register) */
#define GMAC_ADDR_HIGH2			0x00000050					/* Register 20 (MAC Address2 High Register) */
#define GMAC_ADDR_LOW2			0x00000054					/* Register 21 (MAC Address2 Low Register) */
#define GMAC_ADDR_HIGH3			0x00000058					/* Register 22 (MAC Address3 High Register) */
#define GMAC_ADDR_LOW3			0x0000005c					/* Register 23 (MAC Address3 Low Register) */
#define GMAC_ADDR_HIGH4			0x00000060					/* Register 24 (MAC Address4 High Register) */
#define GMAC_ADDR_LOW4			0x00000064					/* Register 25 (MAC Address4 Low Register) */
#define GMAC_ADDR_HIGH5			0x00000068					/* Register 26 (MAC Address5 High Register) */
#define GMAC_ADDR_LOW5			0x0000006c					/* Register 27 (MAC Address5 Low Register) */
#define GMAC_ADDR_HIGH6			0x00000070					/* Register 28 (MAC Address6 High Register) */
#define GMAC_ADDR_LOW6			0x00000074					/* Register 29 (MAC Address6 Low Register) */
#define GMAC_ADDR_HIGH7			0x00000078					/* Register 30 (MAC Address7 High Register) */
#define GMAC_ADDR_LOW7			0x0000007c					/* Register 31 (MAC Address7 Low Register) */
#define GMAC_ADDR_HIGH8			0x00000080					/* Register 32 (MAC Address8 High Register) */
#define GMAC_ADDR_LOW8			0x00000084					/* Register 33 (MAC Address8 Low Register) */
#define GMAC_ADDR_HIGH9			0x00000088					/* Register 34 (MAC Address9 High Register) */
#define GMAC_ADDR_LOW9			0x0000008c					/* Register 35 (MAC Address9 Low Register) */
#define GMAC_ADDR_HIGH10		0x00000090					/* Register 36 (MAC Address10 High Register) */
#define GMAC_ADDR_LOW10			0x00000094					/* Register 37 (MAC Address10 Low Register) */
#define GMAC_ADDR_HIGH11		0x00000098					/* Register 38 (MAC Address11 High Register) */
#define GMAC_ADDR_LOW11			0x0000009c					/* Register 39 (MAC Address11 Low Register) */
#define GMAC_ADDR_HIGH12		0x000000a0					/* Register 40 (MAC Address12 High Register) */
#define GMAC_ADDR_LOW12			0x000000a4					/* Register 41 (MAC Address12 Low Register) */
#define GMAC_ADDR_HIGH13		0x000000a8					/* Register 42 (MAC Address13 High Register) */
#define GMAC_ADDR_LOW13			0x000000ac					/* Register 43 (MAC Address13 Low Register) */
#define GMAC_ADDR_HIGH14		0x000000b0					/* Register 44 (MAC Address14 High Register) */
#define GMAC_ADDR_LOW14			0x000000b4					/* Register 45 (MAC Address14 Low Register) */
#define GMAC_ADDR_HIGH15		0x000000b8					/* Register 46 (MAC Address15 High Register) */
#define GMAC_ADDR_LOW15			0x000000bc					/* Register 47 (MAC Address15 Low Register) */
#endif

#define GMAC_HASH(n)			(0x00000500 + (n) * 4)		/* Register 320 (Hash Table Register 0) to Register 327 (Hash Table Register 7)*/
#define GMAC_VLAN_INCL			0x00000584					/* Register 353 (VLAN Tag Inclusion or Replacement Register) */
#define GMAC_VLAN_HASH			0x00000588					/* Register 354 (VLAN Hash Table Register) */

#if 0
#define GMAC_ADDR_HIGH16		0x00000800					/* Register 512 (MAC Address16 High Register) */
#define GMAC_ADDR_LOW16			0x00000804					/* Register 513 (MAC Address16 Low Register) */
#define GMAC_ADDR_HIGH17		0x00000808					/* Register 514 (MAC Address17 High Register) */
#define GMAC_ADDR_LOW17			0x0000080c					/* Register 515 (MAC Address17 Low Register) */
#define GMAC_ADDR_HIGH18		0x00000810					/* Register 516 (MAC Address18 High Register) */
#define GMAC_ADDR_LOW18			0x00000814					/* Register 517 (MAC Address18 Low Register) */
#define GMAC_ADDR_HIGH19		0x00000818					/* Register 518 (MAC Address19 High Register) */
#define GMAC_ADDR_LOW19			0x0000081c					/* Register 519 (MAC Address19 Low Register) */
#define GMAC_ADDR_HIGH20		0x00000820					/* Register 520 (MAC Address20 High Register) */
#define GMAC_ADDR_LOW20			0x00000824					/* Register 521 (MAC Address20 Low Register) */
#define GMAC_ADDR_HIGH21		0x00000828					/* Register 522 (MAC Address21 High Register) */
#define GMAC_ADDR_LOW21			0x0000082c					/* Register 523 (MAC Address21 Low Register) */
#define GMAC_ADDR_HIGH22		0x00000830					/* Register 524 (MAC Address22 High Register) */
#define GMAC_ADDR_LOW22			0x00000834					/* Register 525 (MAC Address22 Low Register) */
#define GMAC_ADDR_HIGH23		0x00000838					/* Register 536 (MAC Address23 High Register) */
#define GMAC_ADDR_LOW23			0x0000083c					/* Register 537 (MAC Address23 Low Register) */
#define GMAC_ADDR_HIGH24		0x00000840					/* Register 538 (MAC Address24 High Register) */
#define GMAC_ADDR_LOW24			0x00000844					/* Register 539 (MAC Address24 Low Register) */
#define GMAC_ADDR_HIGH25		0x00000848					/* Register 540 (MAC Address25 High Register) */
#define GMAC_ADDR_LOW25			0x0000084c					/* Register 541 (MAC Address25 Low Register) */
#define GMAC_ADDR_HIGH26		0x00000850					/* Register 542 (MAC Address26 High Register) */
#define GMAC_ADDR_LOW26			0x00000854					/* Register 543 (MAC Address26 Low Register) */
#define GMAC_ADDR_HIGH27		0x00000858					/* Register 544 (MAC Address27 High Register) */
#define GMAC_ADDR_LOW27			0x0000085c					/* Register 545 (MAC Address27 Low Register) */
#define GMAC_ADDR_HIGH28		0x00000860					/* Register 546 (MAC Address28 High Register) */
#define GMAC_ADDR_LOW28			0x00000864					/* Register 547 (MAC Address28 Low Register) */
#define GMAC_ADDR_HIGH29		0x00000868					/* Register 548 (MAC Address29 High Register) */
#define GMAC_ADDR_LOW29			0x0000086c					/* Register 549 (MAC Address29 Low Register) */
#define GMAC_ADDR_HIGH30		0x00000870					/* Register 550 (MAC Address30 High Register) */
#define GMAC_ADDR_LOW30			0x00000874					/* Register 551 (MAC Address30 Low Register) */
#define GMAC_ADDR_HIGH31		0x00000878					/* Register 552 (MAC Address31 High Register) */
#define GMAC_ADDR_LOW31			0x0000087c					/* Register 553 (MAC Address31 Low Register) */
#endif
//austin:we only have 8 hash table regs so maybe we have to modify some code.
#define GMAC_NUM_HASH			8

/* DMA Register list */
#define GMAC_DMA_BUS_MODE		0x00001000	/* Register 0 (Bus Mode Register) */
#define GMAC_DMA_TX_POLL		0x00001004	/* Register 1 (Transmit Poll Demand Register) */
#define GMAC_DMA_RX_POLL		0x00001008	/* Register 2 (Receive Poll Demand Register) */
#define GMAC_DMA_RX_BASE_ADDR		0x0000100c	/* Register 3 (Receive Descriptor List Address Register) */
#define GMAC_DMA_TX_BASE_ADDR		0x00001010	/* Register 4 (Transmit Descriptor List Address Register) */
#define GMAC_DMA_STATUS			0x00001014	/* Register 5 (Status Register) */
#define GMAC_DMA_OPERATION		0x00001018	/* Register 6 (Operation Mode Register) */
#define GMAC_DMA_INTR_ENA		0x0000101c	/* Register 7 (Interrupt Enable Register) */
#define GMAC_DMA_MISS_FRAME_CTR		0x00001020	/* Register 8 (Missed Frame and Buffer Overflow Counter Register) */
#define GMAC_DMA_RI_WDOG_TIMER		0x00001024	/* Register 9 (Receive Interrupt Watchdog Timer Register) */
#define GMAC_DMA_AXI_BUS		0x00001028	/* Register 10 (AXI Bus Mode Register) */
#define GMAC_DMA_AXI_STATUS		0x0000102C	/* Register 11 (AHB or AXI Status Register) */
#define GMAC_DMA_HW_FEATURE		0x00001058	/* Register 22 (HW Feature Register) */

/* PTP Register list */
#define GMAC_TS_CONTROL			0x00000700	/* Register 448 (Timestamp Control Register) */
#define GMAC_TS_SUBSEC_INCR		0x00000704	/* Register 449 (Sub-Second Increment Register) */
#define GMAC_TS_HIGH			0x00000708	/* Register 450 (System Time - Seconds Register) */
#define GMAC_TS_LOW			0x0000070c	/* Register 451 (System Time - Nanoseconds Register) */
#define GMAC_TS_HIGH_UPDATE		0x00000710	/* Register 452 (System Time - Seconds Update Register) */
#define GMAC_TS_LOW_UPDATE		0x00000714	/* Register 453 (System Time - Nanoseconds Update Register) */
#define GMAC_TS_ADDEND			0x00000718	/* Register 454 (Timestamp Addend Register) */
#define GMAC_TS_TARGET_HIGH		0x0000071c	/* Register 455 (Target Time Seconds Register) */
#define GMAC_TS_TARGET_LOW		0x00000720	/* Register 456 (Target Time Nanoseconds Register) */
#define GMAC_TS_HIGH_WORD		0x00000724	/* Register 457 (System Time - Higher Word Seconds Register) */
#define GMAC_TS_STATUS			0x00000728	/* Register 458 (Timestamp Status Register) */
#define GMAC_TS_AUXI_HIGH		0x00000730	/* Register 460 (Auxiliary Timestamp - Nanoseconds Register) */
#define GMAC_TS_AUXI_LOW		0x00000734	/* Register 461 (Auxiliary Timestamp - Seconds Register) */

/* MAC Configuration Register */
#define GMAC_CONTROL_SARC_MASK		0xc0000000	/* bit [30:28] Source Address Insertion or Replacement Control */
#define GMAC_CONTROL_TWOKPE		0x08000000	/* bit [27] IEEE 802.3as Support for 2K Packets */
#define GMAC_CONTROL_CST		0x02000000	/* bit [25] CRC Stripping for Type Frames */
#define GMAC_CONTROL_WD			0x00800000	/* bit [23] Watchdog Disable */
#define GMAC_CONTROL_JD			0x00400000	/* bit [22] Jabber Disable */
#define GMAC_CONTROL_JE			0x00100000	/* bit [20] Jumbo Frame Enable */
#define GMAC_CONTROL_SPD_MASK		0x0000c000	/* bit [15:14] Speed Mask(10M/100M/1000M) */
#define GMAC_CONTROL_PS			0x00008000	/* bit [15] Port Select */
#define GMAC_CONTROL_FES		0x00004000	/* bit [14] speed (0: 10 Mbps, 1: 100 Mbps) */
#define GMAC_CONTROL_LM			0x00001000	/* bit [12] Loopback Mode */
#define GMAC_CONTROL_DM			0x00000800	/* bit [11] Duplex Mode */
#define GMAC_CONTROL_IPC		0x00000400	/* bit [10] Checksum Offload */
#define GMAC_CONTROL_ACS		0x00000080	/* bit [7] Automatic Pad/FCS Strip */
#define GMAC_CONTROL_TE			0x00000008	/* bit [3] Transmitter Enable */
#define GMAC_CONTROL_RE			0x00000004	/* bit [2] Receiver Enable */

#define GMAC_SPEED_1000M		0
#define GMAC_SPEED_100M			GMAC_CONTROL_SPD_MASK
#define GMAC_SPEED_10M			GMAC_CONTROL_PS

/* MAC Frame Filter */
#define GMAC_FRAME_FILTER_RA	0x80000000	/* bit [31] Receive All */
#define GMAC_FRAME_FILTER_HPF	0x00000400	/* bit [10] Hash or perfect Filter */
#define GMAC_FRAME_FILTER_SAF	0x00000200	/* bit [9] Source Address Filter Enable */
#define GMAC_FRAME_FILTER_SAIF	0x00000100	/* bit [8] SA Inverse Filtering */
#define GMAC_FRAME_FILTER_PCF	0x000000c0	/* bit [7:6] Pass Control Frames */
#define GMAC_FRAME_FILTER_DBF	0x00000020	/* bit [5] Disable Broadcast Frames */
#define GMAC_FRAME_FILTER_PM	0x00000010	/* bit [4] Pass All Multicast */
#define GMAC_FRAME_FILTER_DAIF	0x00000008	/* bit [3] DA Inverse Filtering */
#define GMAC_FRAME_FILTER_HMC	0x00000004	/* bit [2] Hash Multicast */
#define GMAC_FRAME_FILTER_HUC	0x00000002	/* bit [1] Hash Unicast */
#define GMAC_FRAME_FILTER_PR	0x00000001	/* bit [0] Promiscuous Mode */

/* GMII Address Register */
#define GMAC_GMII_ADDR_PA		0x0000f800	/* bit [15:11] Physical Layer Address */
#define GMAC_GMII_ADDR_GR		0x000007c0	/* bit [10:6] GMII Register */
#define GMAC_GMII_ADDR_CR_MASK	0x0000003c	/* bit [5:2] CSR Clock Range */
#define GMAC_GMII_ADDR_CR		0x00000014	/* bit [5:2] 0101: The CSR clock frequency is 250–300 MHz and the MDC clock is CSR clock/124.*/
#define GMAC_GMII_ADDR_GW		0x00000002	/* bit [1] GMII Write */
#define GMAC_GMII_ADDR_GB		0x00000001	/* bit [0] GMII Busy */

/* GMII Data Register */
#define GMAC_GMII_DATA_GD		0x0000ffff	/* bit [15:0] GMII Data */

/* Flow Control Register */
#define GMAC_FLOW_CTRL_PT_MASK	0xffff0000	/* bit [31:16] Pause Time Mask */
#define GMAC_FLOW_CTRL_PT_SHIFT	16
#define GMAC_FLOW_CTRL_DZQP		0x00000080	/* bit [7] Disable Zero-Quanta Pause */
#define GMAC_FLOW_CTRL_PLT		0x00000020	/* The threshold is Pause time minus 28 slot times (PT – 28 slot times). */
#define GMAC_FLOW_CTRL_PLT_MASK	0x00000030	/* bit [5:4] Pause Low Threshhold */
#define GMAC_FLOW_CTRL_UP		0x00000008	/* bit [3] Unicast Pause Frame Detect */
#define GMAC_FLOW_CTRL_RFE		0x00000004	/* bit [2] Rx Flow Control Enable */
#define GMAC_FLOW_CTRL_TFE		0x00000002	/* bit [1] Tx Flow Control Enable */
#define GMAC_FLOW_CTRL_FCB_BPA	0x00000001	/* bit [0] Flow Control Busy or Backpressure Activate */

/* VLAN Tag Register */
#define GMAC_VLAN_VTHM			0x00080000	/* bit [19] VLAN Tag Hash Table Match Enable */
#define GMAC_VLAN_ESVL			0x00040000	/* bit [18] Enable S-VLAN */
#define GMAC_VLAN_VTIM			0x00020000	/* bit [17] VLAN Tag Inverse Match Enable */
#define GMAC_VLAN_ETV			0x00010000	/* bit [16] Enable 12-Bit VLAN Tag Comparison */
#define GMAC_VLAN_VL			0x0000ffff	/* bit [15:0] VLAN Tag Identifier for Receive Frames */

//If we use hash filter, the VL[11:0] can't equal 0
#define GMAC_VLAN_HASH_VALUE	(GMAC_VLAN_VTHM | 1)

/* PMT Control and Status Register */
#define GMAC_PMT_RWKFILTRST		0x80000000	/* bit [31] Remote Wake-Up Frame Filter Register Pointer Reset */
#define GMAC_PMT_GLBLUCAST		0x00000200	/* bit [9] Global Unicast */
#define GMAC_PMT_RWKPRCVD		0x00000040	/* bit [6] Remote Wake-Up Frame Received */
#define GMAC_PMT_MGKPRCVD		0x00000020	/* bit [5] Magic Packet Received */
#define GMAC_PMT_RWKPKTEN		0x00000004	/* bit [2] Remote Wake-Up Frame Enable */
#define GMAC_PMT_MGKPKTEN		0x00000002	/* bit [1] Magic Packet Enable */
#define GMAC_PMT_PWRDWN			0x00000001	/* bit [0] Power Down */

/* Interrupt Status Register */
#define GMAC_INT_STAT_GPIIS		0x00000800	/* bit [11] GPI Interrupt Status */
#define GMAC_INT_STAT_LPIIS		0x00000400	/* bit [10] LPI Interrupt Status */
#define GMAC_INT_STAT_TSIS		0x00000200	/* bit [9] Timestamp Interrupt Status */
#define GMAC_INT_STAT_PMTIS		0x00000008	/* bit [3] PMT Interrupt Status */

/* Interrupt Mask Register */
#define GMAC_INT_MASK_LPIIM		0x00000400	/* bit [10] LPI Interrupt Mask */
#define GMAC_INT_MASK_TSIM		0x00000200	/* bit [9] Timestamp Interrupt Mask */
#define GMAC_INT_MASK_PMTIM		0x00000008	/* bit [3] PMT Interrupt Mask */

/* MAC Address0 High Register */
#define GMAC_ADDR_AE			0x80000000	/* bit [31] Address Enable */

/* DMA Bus Mode Register */
#define DMA_BUS_MODE_AAL		0x02000000	/* bit [25] Address-Aligned Beats */
#define DMA_BUS_MODE_8PBL		0x01000000	/* bit [24] PBLx8 Mode */
#define DMA_BUS_MODE_USP		0x00800000	/* bit [23] Use Separate PBL */
#define DMA_BUS_MODE_RPBL_MASK	0x003e0000	/* bit [22:17] Rx-Programmable Burst Len */
#define DMA_BUS_MODE_RPBL_SHIFT	17
#define DMA_BUS_MODE_FB			0x00010000	/* bit [16] Fixed burst */
#define DMA_BUS_MODE_PBL_MASK	0x00003f00	/* bit [13:8] Programmable Burst Len */
#define DMA_BUS_MODE_PBL_SHIFT	8
#define DMA_BUS_MODE_ATDS		0x00000080	/* bit [7] Alternate Descriptor Size */
#define DMA_BUS_MODE_DSL_MASK	0x0000007c	/* bit [6:2] Descriptor Skip Length */
#define DMA_BUS_MODE_DSL_SHIFT	2			/* (in DWORDS) */
#define DMA_BUS_MODE_SFT_RESET	0x00000001	/* bit [0] Software Reset */

/* DMA Operation Mode Register */
//austin:we have to config DMA_OPERATION_RFD, DMA_OPERATION_RFA and DMA_OPERATION_RTC_256.
#define DMA_OPERATION_DT		0x00000040	/* bit [26] Disable Dropping of TCP/IP Checksum Error Frames */
#define DMA_OPERATION_RSF		0x00000020	/* bit [25] Receive Store and Forward*/
#define DMA_OPERATION_RFA2		0x00800000	/* bit [23] MSB of Threshold for Activating Flow Control */
#define DMA_OPERATION_RFD2		0x00400000	/* bit [22] MSB of Threshold for Deactivating Flow Control */
#define DMA_OPERATION_TSF		0x00200000	/* bit [21] TX FIFO Store and Forward */
#define DMA_OPERATION_FTF		0x00100000	/* bit [20] Flush Transmit FIFO */
#define DMA_OPERATION_TTC_MASK	0x0001c000	/* bit [16:14] Transmit Threshhold Ctrl */
#define DMA_OPERATION_RFD_MASK	0x00001800	/* bit [12:11] Threshold for Deactivating Flow Control */
#define DMA_OPERATION_RFD		0x00000800	/* bit [11] 01: Full minus 2 KB, that is, FULL — 2 KB */
#define DMA_OPERATION_RFA_MASK	0x00000600	/* bit [10:9] Threshold for Activating Flow Control */
#define DMA_OPERATION_RFA		0x00000200	/* bit [9] 01: Full minus 2 KB, that is, FULL—2KB. */
#define DMA_OPERATION_EFC		0x00000100	/* bit [8] Enable Hardware FC */
#define DMA_OPERATION_FEF		0x00000080	/* bit [7] Forward Error Frames */
#define DMA_OPERATION_RTC_256	0x00000018	/* RX Threshhold Ctrl */
#define DMA_OPERATION_RTC_MASK	0x00000018	/* bit [4:3] Receive Threshold Control */

#define DMA_OPERATION_DFF		0x01000000	/* bit [24] Disable Flushing of Received Frames */
#define DMA_OPERATION_ST		0x00002000	/* bit [13] Start/Stop Transmission */
#define DMA_OPERATION_OSF		0x00000004	/* bit [3] Operate on 2nd tx frame */
#define DMA_OPERATION_SR		0x00000002	/* bit [2] Start/Stop Receive */

/* DMA Interrupt Enable Register */
#define DMA_INTR_ENA_NIE		0x00010000	/* bit [16] Normal Interrupt Summary Enable */
#define DMA_INTR_ENA_AIE		0x00008000	/* bit [15] Abnormal Interrupt Summary Enable */
#define DMA_INTR_ENA_ERE		0x00004000	/* bit [14] Early Receive Interrupt Enable */
#define DMA_INTR_ENA_FBE		0x00002000	/* bit [13] Fatal Bus Error Enable */
#define DMA_INTR_ENA_ETE		0x00000400	/* bit [10] Early Transmit Interrupt Enable */
#define DMA_INTR_ENA_RWE		0x00000200	/* bit [9] Receive Watchdog Timeout Enable */
#define DMA_INTR_ENA_RSE		0x00000100	/* bit [8] Receive Stopped Enable */
#define DMA_INTR_ENA_RUE		0x00000080	/* bit [7] Receive Buffer Unavailable Enable */
#define DMA_INTR_ENA_RIE		0x00000040	/* bit [6] Receive Interrupt Enable */
#define DMA_INTR_ENA_UNE		0x00000020	/* bit [5] Underflow Interrupt Enable */
#define DMA_INTR_ENA_OVE		0x00000010	/* bit [4] Overflow Interrupt Enable */
#define DMA_INTR_ENA_TJE		0x00000008	/* bit [3] Transmit Jabber Timeout Enable */
#define DMA_INTR_ENA_TUE		0x00000004	/* bit [2] Transmit Buffer Unavailable Enable */
#define DMA_INTR_ENA_TSE		0x00000002	/* bit [1] Transmit Stopped Enable */
#define DMA_INTR_ENA_TIE		0x00000001	/* bit [0] Transmit Interrupt Enable*/

#define DMA_INTR_NORMAL		(DMA_INTR_ENA_NIE | DMA_INTR_ENA_RIE | \
				 DMA_INTR_ENA_TUE | DMA_INTR_ENA_TIE)

#define DMA_INTR_ABNORMAL	(DMA_INTR_ENA_AIE | DMA_INTR_ENA_FBE | \
				 DMA_INTR_ENA_RWE | DMA_INTR_ENA_RSE | \
				 DMA_INTR_ENA_RUE | DMA_INTR_ENA_UNE | \
				 DMA_INTR_ENA_OVE | DMA_INTR_ENA_TJE | \
				 DMA_INTR_ENA_TSE)

/* DMA default interrupt mask */
#define DMA_INTR_DEFAULT_MASK	(DMA_INTR_NORMAL | DMA_INTR_ABNORMAL)

/* DMA Status Register */
#define DMA_STATUS_TTI			0x20000000	/* bit [29] Timestamp Trigger Interrupt */
#define DMA_STATUS_GPI			0x10000000	/* bit [28] GMAC PMT Interrupt*/
#define DMA_STATUS_GLI			0x04000000	/* bit [26] GMAC Line Interface Interrupt */
#define DMA_STATUS_EB_MASK		0x00380000	/* bit [25:23] Error Bits */
#define DMA_STATUS_TS_MASK		0x00700000	/* bit [22:20] Transmit Process State */
#define DMA_STATUS_TS_SHIFT		20
#define DMA_STATUS_RS_MASK		0x000e0000	/* bit [19:17] Receive Process State */
#define DMA_STATUS_RS_SHIFT		17
#define DMA_STATUS_NIS			0x00010000	/* bit [16] Normal Interrupt Summary */
#define DMA_STATUS_AIS			0x00008000	/* bit [15] Abnormal Interrupt Summary */
#define DMA_STATUS_ERI			0x00004000	/* bit [14] Early Receive Interrupt */
#define DMA_STATUS_FBI			0x00002000	/* bit [13] Fatal Bus Error Interrupt */
#define DMA_STATUS_ETI			0x00000400	/* bit [10] Early Transmit Interrupt */
#define DMA_STATUS_RWT			0x00000200	/* bit [9] Receive Watchdog Timeout */
#define DMA_STATUS_RPS			0x00000100	/* bit [8] Receive Process Stopped */
#define DMA_STATUS_RU			0x00000080	/* bit [7] Receive Buffer Unavailable */
#define DMA_STATUS_RI			0x00000040	/* bit [6] Receive Interrupt */
#define DMA_STATUS_UNF			0x00000020	/* bit [5] Transmit Underflow */
#define DMA_STATUS_OVF			0x00000010	/* bit [4] Receive Overflow */
#define DMA_STATUS_TJT			0x00000008	/* bit [3] Transmit Jabber Timeout */
#define DMA_STATUS_TU			0x00000004	/* bit [2] Transmit Buffer Unavail */
#define DMA_STATUS_TPS			0x00000002	/* bit [1] Transmit Process Stopped */
#define DMA_STATUS_TI			0x00000001	/* bit [0] Transmit Interrupt */

/* DMA HW Features Register */
#define DMA_HW_FEAT_TXCOESEL	0x00010000	/* TX Checksum offload */

/* Timestamp Control Register */
#define GMAC_TS_CONTROL_ATSEN3		0x10000000	/* bit [28] Auxiliary Snapshot 3 Enable */
#define GMAC_TS_CONTROL_ATSEN2		0x08000000	/* bit [27] Auxiliary Snapshot 2 Enable */
#define GMAC_TS_CONTROL_ATSEN1		0x04000000	/* bit [26] Auxiliary Snapshot 1 Enable */
#define GMAC_TS_CONTROL_ATSEN0		0x02000000	/* bit [25] Auxiliary Snapshot 0 Enable */
#define GMAC_TS_CONTROL_ATSFC		0x01000000	/* bit [24] Auxiliary Snapshot FIFO Clear */
#define GMAC_TS_CONTROL_ENMACADDR	0x00040000	/* bit [18] Enable MAC address for PTP Frame Filtering */
#define GMAC_TS_CONTROL_SNAPTYPSEL	0x00030000	/* bit [17:16] Select PTP packets for Taking Snapshots */
#define GMAC_TS_CONTROL_BIT16		0x00010000	/* bit [16] Select all PTP packets for Taking Snapshots */
#define GMAC_TS_CONTROL_TSMSTRENA	0x00008000	/* bit [15] Enable Snapshot for Messages Relevant to Master */
#define GMAC_TS_CONTROL_TSEVNTENA	0x00004000	/* bit [14] Enable Timestamp Snapshot for Event Messages */
#define GMAC_TS_CONTROL_TSIPV4ENA	0x00002000	/* bit [13] Enable Processing of PTP Frames Sent over IPv4-UDP */
#define GMAC_TS_CONTROL_TSIPV6ENA	0x00001000  /* bit [12] Enable Processing of PTP Frames Sent over IPv6-UDP */
#define GMAC_TS_CONTROL_TSIPENA		0x00000800  /* bit [11] Enable Processing of PTP over Ethernet Frames */
#define GMAC_TS_CONTROL_TSVER2ENA	0x00000400  /* bit [10] Enable PTP packet Processing for Version 2 Format */
#define GMAC_TS_CONTROL_TSCTRLSSR	0x00000200  /* bit [9] Timestamp Digital or Binary Rollover Control */
#define GMAC_TS_CONTROL_TSENALL		0x00000100  /* bit [8] Enable Timestamp for All Frames */
#define GMAC_TS_CONTROL_TSADDREG	0x00000020  /* bit [5] Addend Reg Update */
#define GMAC_TS_CONTROL_TSTRIG		0x00000010  /* bit [4] Timestamp Interrupt Trigger Enable */
#define GMAC_TS_CONTROL_TSUPDT		0x00000008  /* bit [3] Timestamp Update */
#define GMAC_TS_CONTROL_TSINIT		0x00000004  /* bit [2] Timestamp Initialize */
#define GMAC_TS_CONTROL_TSCFUPDT	0x00000002  /* bit [1] Timestamp Fine or Coarse Update */
#define GMAC_TS_CONTROL_TSENA		0x00000001  /* bit [0] Timestamp Enable */

#define GMAC_TS_CONTROL_DEFAULT		(GMAC_TS_CONTROL_ATSEN0 | GMAC_TS_CONTROL_BIT16 | GMAC_TS_CONTROL_TSIPV4ENA | GMAC_TS_CONTROL_TSCFUPDT | \
									GMAC_TS_CONTROL_TSIPENA | GMAC_TS_CONTROL_TSCTRLSSR | GMAC_TS_CONTROL_TSENA)

/* Timestamp Timestamp Status Register */
#define GMAC_TS_STATUS_ATSNS		0xbe000000	/* bit [29:25] Number of Auxiliary Timestamp Snapshots */
#define GMAC_TS_STATUS_ATSSTM		0x01000000	/* bit [24] Auxiliary Timestamp Snapshot Trigger Missed */
#define GMAC_TS_STATUS_ATSSTN		0x000f0000	/* bit [19:16] Auxiliary Timestamp Snapshot Trigger Identifier */
#define GMAC_TS_STATUS_TSTRGTERR3	0x00000200	/* bit [9] Timestamp Target Time Error */
#define GMAC_TS_STATUS_TSTARGT3		0x00000100	/* bit [8] Timestamp Target Time Reached for Target Time PPS3 */
#define GMAC_TS_STATUS_TSTRGTERR2	0x00000080	/* bit [7] Timestamp Target Time Error */
#define GMAC_TS_STATUS_TSTARGT2		0x00000040	/* bit [6] Timestamp Target Time Reached for Target Time PPS2 */
#define GMAC_TS_STATUS_TSTRGTERR1	0x00000020	/* bit [5] Timestamp Target Time Error */
#define GMAC_TS_STATUS_TSTARGT1		0x00000010	/* bit [4] Timestamp Target Time Reached for Target Time PPS1 */
#define GMAC_TS_STATUS_TSTRGTERR	0x00000008	/* bit [3] Timestamp Target Time Error */
#define GMAC_TS_STATUS_AUXTSTRIG	0x00000004	/* bit [2] Auxiliary Timestamp Trigger Snapshot */
#define GMAC_TS_STATUS_TSTARGT		0x00000002	/* bit [1] Timestamp Target Time Reached */
#define GMAC_TS_STATUS_TSSOVF		0x00000001	/* bit [0] Timestamp Seconds Overflow */

#define GMAC_PTP_CLOCK				50			/* 50MHz */

/* GMAC Descriptor Defines */
#define MAX_DESC_BUF_SZ			(0x2000 - 8)

//rxdesc0
#define RXDESC_EXT_STATUS		0x00000001	/* bit [0] Extended Status Available */
#define RXDESC_CRC_ERR			0x00000002	/* bit [1] CE: CRC Error */
#define RXDESC_RX_ERR			0x00000008	/* bit [3] RE: Receive Error */
#define RXDESC_RX_WDOG			0x00000010	/* bit [4] RWT: Receive Watchdog Timeout */
#define RXDESC_FRAME_TYPE		0x00000020	/* bit [5] FT: Frame Type */
#define RXDESC_CHECKSUM_ERR		0x00000080	/* bit [7] IPC Checksum Error */
#define RXDESC_LAST_SEG			0x00000100	/* bit [8] LS: Last Descriptor */
#define RXDESC_FIRST_SEG		0x00000200	/* bit [9] FS: First Descriptor */
#define RXDESC_VLAN_FRAME		0x00000400	/* bit [10] VLAN: VLAN Tag */
#define RXDESC_OVERFLOW_ERR		0x00000800	/* bit [11] OE: Overflow Error */
#define RXDESC_LENGTH_ERR		0x00001000	/* bit [12] LE: Length Error */
#define RXDESC_SA_FILTER_FAIL	0x00002000	/* bit [13] SAF: Source Address Filter Fail */
#define RXDESC_DESCRIPTOR_ERR	0x00004000	/* bit [14] DE: Descriptor Error */
#define RXDESC_ERROR_SUMMARY	0x00008000	/* bit [15] ES: Error Summary */
#define RXDESC_FRAME_LEN_OFFSET	16
#define RXDESC_FRAME_LEN_MASK	0x3fff0000	/* bit [29:16] FL: Frame Length */
#define RXDESC_DA_FILTER_FAIL	0x40000000	/* bit [30] AFM: Destination Address Filter Fail */

//rxdesc1
#define RXDESC1_END_RING		0x00008000	/* bit [15] RER: Receive End of Ring */

//rxdesc4
#define RXDESC_IP_PAYLOAD_MASK	0x00000003	/* bit [1:0] IP Payload Type */
#define RXDESC_IP_PAYLOAD_UDP	0x00000001	/* bit [0] UDP */
#define RXDESC_IP_PAYLOAD_TCP	0x00000002	/* bit [1] TCP */
#define RXDESC_IP_PAYLOAD_ICMP	0x00000003	/* bit [1:0] ICMP */
#define RXDESC_IP_HEADER_ERR	0x00000008	/* bit [3] IP Header Error */
#define RXDESC_IP_PAYLOAD_ERR	0x00000010	/* bit [4] IP Payload Error */
#define RXDESC_IPV4_PACKET		0x00000040	/* bit [6] IPv4 Packet Received */
#define RXDESC_IPV6_PACKET		0x00000080	/* bit [7] IPv6 Packet Received */

//txdesc0
#define TXDESC_UNDERFLOW_ERR	0x00000002	/* bit [1] UF: Underflow Error */
#define TXDESC_VLAN_FRAME		0x00000080	/* bit [7] VF: VLAN Frame */
#define TXDESC_PAYLOAD_CSUM_ERR	0x00001000	/* bit [12] IPE: IP Payload Error */
#define TXDESC_FRAME_FLUSHED	0x00002000	/* bit [13] FF: Frame Flushed */
#define TXDESC_JABBER_TIMEOUT	0x00004000	/* bit [14] JT: Jabber Timeout */
#define TXDESC_ERROR_SUMMARY	0x00008000	/* bit [15] IPE: IP Payload Error */
#define TXDESC_IP_HEADER_ERR	0x00010000	/* bit [16] IHE: IP Header Error */
#define TXDESC_TX_TS_STATUS		0x00020000	/* bit [17] TTSS: Transmit Timestamp Status */
#define TXDESC_VLAN_REMOVE		0x00040000	/* bit [18] Remove VLAN tag */
#define TXDESC_VLAN_INSERT		0x00080000	/* bit [19] Insert a VLAN tag */
#define TXDESC_VLAN_REPLACE		0x000c0000	/* bit [19:18] Replace the VLAN tag */
#define TXDESC_2ND_ADDR_CHAINED	0x00100000	/* bit [20] TCH: Second Address Chained */
#define TXDESC_END_RING			0x00200000	/* bit [21] TER: Transmit End of Ring */
#define TXDESC_CSUM_IP			0x00400000	/* bit [22] Only IP header checksum calculation and insertion are enabled */
#define TXDESC_CSUM_IP_PAYLD	0x00800000	/* bit [23] IP header checksum and payload checksum calculation and insertion are enabled,
											   but pseudo-header checksum is not calculated in hardware */
#define TXDESC_CSUM_ALL			0x00C00000	/* bit [23:22] CIC: Checksum Insertion Control */
#define TXDESC_CRC_EN_REPLACE	0x01000000	/* bit [24] CRCR: CRC Replacement Control */
#define TXDESC_TX_TS_EN			0x02000000	/* bit [25] TTSE: Transmit Timestamp Enable */
#define TXDESC_DISABLE_PAD		0x04000000	/* bit [26] DP: Disable Pad */
#define TXDESC_FIRST_SEG		0x10000000	/* bit [28] FS: First Segment */
#define TXDESC_LAST_SEG			0x20000000	/* bit [29] LS: Last Segment */
#define TXDESC_INTERRUPT		0x40000000	/* bit [30] IC: Interrupt on Completion */

//rxdesc0 and txdesc0
#define DESC_OWN				0x80000000	/* bit [31] OWN: Own Bit */

//rxdesc1 and txdesc1
#define DESC_BUFFER1_SZ_MASK	0x00001fff	/* bit [12:0] TBS1: Transmit Buffer 1 Size */
#define DESC_BUFFER2_SZ_MASK	0x1fff0000	/* bit [28:16] TBS2: Transmit Buffer 2 Size */
#define DESC_BUFFER2_SZ_OFFSET	16

#define MAX_FRAME_SIZE 1600
#define BUF_ALIGN                    32

#define DMA_RX_RING_SZ 32
#define DMA_TX_RING_SZ  16
/* Return count in buffer.  */
#define CIRC_CNT(head,tail,size) (((head) - (tail)) & ((size)-1))

/* Return space available, 0..size-1.  We always leave one free char
 *    as a completely full buffer has head == tail, which is the same as
 *       empty.  */
#define CIRC_SPACE(head,tail,size) CIRC_CNT((tail),((head)+1),(size))

#define dma_ring_cnt(h, t, s) CIRC_CNT(h, t, s)
#define dma_ring_incr(n, s) (((n) + 1) & ((s)-1))
#define dma_ring_space(h, t, s) CIRC_SPACE(h, t, s)

#define ETHSW_CAP_1_PPORTS_OFFSET   0x0007
#define ETHSW_CAP_1_PPORTS_SHIFT    0
#define ETHSW_CAP_1_PPORTS_SIZE     4

#define PHY_ADDR_0_ADDR_OFFSET 0xF415
#define PHY_ADDR_0_ADDR_SHIFT  0
#define PHY_ADDR_0_ADDR_SIZE  5
#define PHY_ADDR_0_REG_SIZE  16

#define MMDC_CFG_0_PEN_ALL_OFFSET 0xF40B
#define MMDC_CFG_0_PEN_ALL_SHIFT  0
#define MMDC_CFG_0_PEN_ALL_SIZE   7


#endif
