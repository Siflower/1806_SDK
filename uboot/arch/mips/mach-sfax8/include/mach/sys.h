#ifndef __PLL_DIV_H__
#define __PLL_DIV_H__

#include <mach/sfax8.h>

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

#define CRYSTAL_6M_CPU_PARA		(0x4b000000150)
#define CRYSTAL_6M_DDR_PARA		(0x510000001BC)			//ddr phy clock source: 0x510000001BC for 1332MHz, 0x4900000010a for 1596MHz
#define CRYSTAL_6M_CMN_PARA		(0x4a00000018c)
#define CRYSTAL_6M_SPC_PARA		(0x4a0000001f4)

#define CRYSTAL_12M_CPU_PARA	(0x4b0000000a8)
#define CRYSTAL_12M_DDR_PARA	(0X4a00000010a)
#define CRYSTAL_12M_CMN_PARA	(0x4a0000000c6)
#define CRYSTAL_12M_SPC_PARA	(0X4a0000000fa)

#define SF_CPU_PLL_CLK       (1792000000) //OSC_CLK

#endif
