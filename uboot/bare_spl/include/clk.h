#ifndef __CLK_H__
#define __CLK_H__

#ifndef __ASSEMBLY__
#include <types.h>
extern int clk_gating_init(void);
extern int clk_update(void);
#endif

#define KHZ          (1000)
#define MHZ          ((1000 * KHZ))

/* SiFlower sfax8 clock frequency */
#ifdef FPGA
#define SF_OSC_CLK		(12 * MHZ)
#define SF_CPU_PLL_CLK		(0 * MHZ)
#define SF_DDR_PLL_CLK		(0 * MHZ)
#define SF_CMN_PLL_CLK		(0 * MHZ)
#define SF_SPC_PLL_CLK		(0 * MHZ)
#define SF_BUS1_XN_CLK		(50 * MHZ)
#define SF_BUS2_XN_CLK		(50 * MHZ)
#define SF_BUS3_XN_CLK		(80 * MHZ)
#define SF_CPU_CLK		(40 * MHZ)
#define SF_PBUS_CLK		(50 * MHZ)
#define SF_MEM_PHY_CLK		(140 * MHZ)
#define SF_AUDIO_CLK		(60 * MHZ)
#define SF_UART_CLK		(60 * MHZ)
#define SF_SPDIF_CLK		(60 * MHZ)
#define SF_SDHC_CLK		(50 * MHZ)
#define SF_EMMC_CLK		(50 * MHZ)
#define SF_ETH_REF_CLK		(25 * MHZ)
#define SF_ETH_BYP_REF_CLK	(125 * MHZ)
#define SF_ETH_TSU_REF_CLK	(25 * MHZ)
#ifdef SF19A28_FULLMASK
#define SF_GMAC_BYP_REF_CLK	(125*MHZ) //CMN_PLL  phy bypass
#endif
#define SF_WLAN24_MAC_WT_CLK	(30 * MHZ)
#define SF_WLAN5_MAC_WT_CLK	(30 * MHZ)
#define SF_USB_PHY_REF_CLK	(50 * MHZ)
#define SF_TCLK			(50 * MHZ)
#define SF_NPU_PE_CLK		(500 * MHZ)
#define SF_GDU0_CLK		(70 * MHZ)
#define SF_GDU0_EITF_CLK	(237600 * KHZ)
#define SF_TVIF0_CLK		(198 * MHZ)
#elif defined(SF19A28_MPW0)
#define SF_OSC_CLK           (12*MHZ)
#define SF_CPU_PLL_CLK       (672*MHZ)//OSC_CLK
#define SF_DDR_PLL_CLK       (1600*MHZ)//OSC_CLK
#define SF_CMN_PLL_CLK       (1500*MHZ)//OSC_CLK
#define SF_BUS1_XN_CLK       (250*MHZ) //CMN_PLL
#define SF_BUS2_XN_CLK       (320*MHZ) //DDR_PLL
#define SF_BUS3_XN_CLK       (320*MHZ) //DDR_PLL
#define SF_CPU_CLK           (672*MHZ) //CPU_PLL
#define SF_PBUS_CLK          (150*MHZ) //CPU_PLL
#define SF_MEM_PHY_CLK       (400*MHZ) //DDR_PLL
#define SF_UART_CLK          (125*MHZ) //CMN_PLL
#define SF_ETH_REF_CLK       (50*MHZ)  //CMN_PLL
#define SF_ETH_BYP_REF_CLK   (125*MHZ) //CMN_PLL  phy bypass
#define SF_ETH_TSU_REF_CLK   (100*MHZ) //CMN_PLL
#define SF_M6251_0_CLK		 (500*MHZ)//CMN_PLL don't used!!
#define SF_M6251_1_CLK		 (500*MHZ)//CMN_PLL don't used!!
#define SF_WLAN24_MAC_WT_CLK (375*MHZ)//CMN_PLL
#define SF_WLAN5_MAC_WT_CLK  (375*MHZ)//CMN_PLL
#define SF_USB_PHY_REF_CLK   (50*MHZ) //CMN_PLL
#define SF_TCLK              (75*MHZ) //CMN_PLL
#define SF_NPU_PE_CLK        (500*MHZ)//SPC_PLL
#define SF_CATIP_LP_CLK      (32*KHZ)//

#elif defined(SF19A28_FULLMASK)
#define SF_OSC_CLK           (40*MHZ)

#define SF_CPU_PLL_CLK       (1400*MHZ)//OSC_CLK
#define SF_DDR_PLL_CLK       (400*MHZ)//OSC_CLK
#define SF_CMN_PLL_CLK       (1500*MHZ)//OSC_CLK

#define SF_BUS1_XN_CLK       (300*MHZ) //CMN_PLL
#define SF_BUS2_XN_CLK       (300*MHZ) //DDR_PLL
#define SF_BUS3_XN_CLK       (300*MHZ) //DDR_PLL
#define SF_CPU_CLK           (700*MHZ) //CPU_PLL
#define SF_PBUS_CLK          (100*MHZ) //CPU_PLL
#define SF_MEM_PHY_CLK       (400*MHZ) //DDR_PLL
#define SF_UART_CLK          (125*MHZ) //CMN_PLL
#define SF_GMAC_BYP_REF_CLK  (125*MHZ) //CMN_PLL
#define SF_ETH_TSU_CLK	     (50*MHZ) //CMN_PLL
#define SF_M6251_0_CLK	     (375*MHZ)//CMN_PLL
#define SF_M6251_1_CLK	     (375*MHZ)//CMN_PLL
#define SF_WLAN24_MAC_WT_CLK (300*MHZ)//CMN_PLL
#define SF_WLAN5_MAC_WT_CLK  (300*MHZ)//CMN_PLL
#define SF_USB_PHY_REF_CLK   (50*MHZ) //CMN_PLL
#define SF_TCLK              (25*MHZ) //CMN_PLL
#define SF_CATIP_LP_CLK      (32*KHZ)//

#else
#define SF_OSC_CLK		(12 * MHZ)
#define SF_CPU_PLL_CLK		(672 * MHZ)
#define SF_DDR_PLL_CLK		(1596 * MHZ)
#define SF_CMN_PLL_CLK		(1188 * MHZ)
#define SF_SPC_PLL_CLK		(1500 * MHZ)
#define SF_BUS1_XN_CLK		(500 * MHZ)
#define SF_BUS2_XN_CLK		(594 * MHZ)
#define SF_BUS3_XN_CLK		(594 * MHZ)
#define SF_CPU_CLK		(672 * MHZ)
#define SF_PBUS_CLK		(198 * MHZ)
#define SF_MEM_PHY_CLK		(399 * MHZ)
#define SF_AUDIO_CLK		(108 * MHZ)
#define SF_UART_CLK		(108 * MHZ)
#define SF_SPDIF_CLK		(108 * MHZ)
#define SF_SDHC_CLK		(150 * MHZ)
#define SF_EMMC_CLK		(150 * MHZ)
#define SF_ETH_REF_CLK		(25 * MHZ)
#define SF_ETH_BYP_REF_CLK	(125 * MHZ)
#define SF_ETH_TSU_REF_CLK	(100 * MHZ)
#define SF_WLAN24_MAC_WT_CLK	(150 * MHZ)
#define SF_WLAN5_MAC_WT_CLK	(150 * MHZ)
#define SF_USB_PHY_REF_CLK	(50 * MHZ)
#define SF_TCLK			(56571 * KHZ)
#define SF_NPU_PE_CLK		(500 * MHZ)
#define SF_GDU0_CLK		(198 * MHZ)
#define SF_GDU0_EITF_CLK	(237600 * KHZ)
#define SF_TVIF0_CLK		(198 * MHZ)
#endif /* FPGA */

#define SPI_CLOCK		SF_PBUS_CLK
#define UART_CLOCK		SF_UART_CLK
#define SD_CLOCK		SF_SDHC_CLK
#define EMMC_CLOCK		SF_EMMC_CLK

//PLL infm
#define PLL_BASE             CPU_PLL_POWER
#define PLL_COUNT_MAX        4
#define PLL_REG_COUNT_MAX    8
#define PLL_REG_WID          4//btye
#define PLL_OFFSET           0x40

//CLK infm
#define CLK_BASE             BUS1_XN_CLK_CONFIG
#define CLK_COUNT_MAX        26
#define CLK_REG_COUNT_MAX    5
#define CLK_REG_WID          4//btye
#define CLK_OFFSET           0x20

#define PLL_PD_DSM_PD         (1 << 5)
#define PLL_PD_DAC_PD         (1 << 4)
#define PLL_PD_FOUT_4PHASE_PD (1 << 3)
#define PLL_PD_FOUT_POST_PD   (1 << 2)
#define PLL_PD_FOUT_VCO_PD    (1 << 1)
#define PLL_PD_ALL_PD         (1 << 0)

#define PLL_CFG_LOAD         (1 << 5)
#define PLL_CFG_GATE         (1 << 4)
#define PLL_CFG_OCLK_SEL     (1 << 3)
#define PLL_CFG_SRC_SEL      (3 << 1)
#define PLL_CFG_BYPASS       (1 << 0)

#define CLK_CFG_USE_NCO       (1 << 3)
#define CLK_CFG_SRC_SEL       (7 << 0)

#endif
