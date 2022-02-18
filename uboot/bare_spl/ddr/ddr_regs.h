#ifndef __DDR_H__
#define __DDR_H__
#include <types.h>
#include <sf_mmap.h>

//DDR PUB Register Map
#define DDR_PHY_RIDR                    	(SIFLOWER_DDR_PHY_BASE+0x000)		//Revision Identification Register, read only
#define DDR_PHY_PIR                     	(SIFLOWER_DDR_PHY_BASE+0x004)		//PHY Initialization Register
#define DDR_PHY_PGCR0                    	(SIFLOWER_DDR_PHY_BASE+0x008)		//PHY General Configuration Register 0
#define DDR_PHY_PGCR1						(SIFLOWER_DDR_PHY_BASE+0x00c)		//PHY General Configuration Register 1
#define DDR_PHY_PGSR0                   	(SIFLOWER_DDR_PHY_BASE+0x010)		//PHY General Status Register 0
#define DDR_PHY_PGSR1						(SIFLOWER_DDR_PHY_BASE+0x014)		//PHY General Status Register 1
#define DDR_PHY_PLLCR						(SIFLOWER_DDR_PHY_BASE+0x018)		//PLL Control Register
#define DDR_PHY_PTR0                    	(SIFLOWER_DDR_PHY_BASE+0x01c)		//PHY Timing Register 0
#define DDR_PHY_PTR1                    	(SIFLOWER_DDR_PHY_BASE+0x020)		//PHY Timing Register 1
#define DDR_PHY_PTR2                    	(SIFLOWER_DDR_PHY_BASE+0x024)		//PHY Timing Register 2
#define DDR_PHY_PTR3                    	(SIFLOWER_DDR_PHY_BASE+0x028)		//PHY Timing Register 3
#define DDR_PHY_PTR4                    	(SIFLOWER_DDR_PHY_BASE+0x02c)		//PHY Timing Register 4
#define DDR_PHY_ACMDLR                  	(SIFLOWER_DDR_PHY_BASE+0x030)		//AC Master Delay Line Register
#define DDR_PHY_ACBDLR                   	(SIFLOWER_DDR_PHY_BASE+0x034)		//AC Bit Delay Line Register
#define DDR_PHY_ACIOCR                   	(SIFLOWER_DDR_PHY_BASE+0x038)		//AC I/O Configuration Register
#define DDR_PHY_DXCCR						(SIFLOWER_DDR_PHY_BASE+0x03c)		//DATX8 Common Configuration Register
#define DDR_PHY_DSGCR						(SIFLOWER_DDR_PHY_BASE+0x040)		//DDR System General Configuration Register
#define DDR_PHY_DCR							(SIFLOWER_DDR_PHY_BASE+0x044)		//DRAM Configuration Register
#define DDR_PHY_DTPR0                   	(SIFLOWER_DDR_PHY_BASE+0x048)		//DRAM Timing Parameters Register 0
#define DDR_PHY_DTPR1                   	(SIFLOWER_DDR_PHY_BASE+0x04c)		//DRAM Timing Parameters Register 1
#define DDR_PHY_DTPR2                   	(SIFLOWER_DDR_PHY_BASE+0x050)		//DRAM Timing Parameters Register 2
#define DDR_PHY_MR0                     	(SIFLOWER_DDR_PHY_BASE+0x054)		//Mode Register 0
#define DDR_PHY_MR1                     	(SIFLOWER_DDR_PHY_BASE+0x058)		//Mode Register 1
#define DDR_PHY_MR2                     	(SIFLOWER_DDR_PHY_BASE+0x05c)		//Mode Register 2/Extended Mode Register 2
#define DDR_PHY_MR3                     	(SIFLOWER_DDR_PHY_BASE+0x060)		//Mode Register 3
#define DDR_PHY_ODTCR                   	(SIFLOWER_DDR_PHY_BASE+0x064)		//ODT Configuration Register
#define DDR_PHY_DTCR						(SIFLOWER_DDR_PHY_BASE+0x068)		//Data Training Configuration Register
#define DDR_PHY_DTAR0                    	(SIFLOWER_DDR_PHY_BASE+0x06c)		//Data Training Address Register 0
#define DDR_PHY_DTAR1                    	(SIFLOWER_DDR_PHY_BASE+0x070)		//Data Training Address Register 1
#define DDR_PHY_DTAR2                    	(SIFLOWER_DDR_PHY_BASE+0x074)		//Data Training Address Register 2
#define DDR_PHY_DTAR3                    	(SIFLOWER_DDR_PHY_BASE+0x078)		//Data Training Address Register 3
#define DDR_PHY_DTDR0                   	(SIFLOWER_DDR_PHY_BASE+0x07c)		//Data Training Data Register 0
#define DDR_PHY_DTDR1                    	(SIFLOWER_DDR_PHY_BASE+0x080)		//Data Training Data Register 1
#define DDR_PHY_DTEDR0						(SIFLOWER_DDR_PHY_BASE+0x084)		//Data Training Eye Data Register 0
#define DDR_PHY_DTEDR1						(SIFLOWER_DDR_PHY_BASE+0x088)		//Data Training Eye Data Register 1
#define DDR_PHY_PGCR2						(SIFLOWER_DDR_PHY_BASE+0x08c)		//PHY General Configuration Register 2
#define DDR_PHY_RDIMMGCR0					(SIFLOWER_DDR_PHY_BASE+0x0b0)		//RDIMM General Configuration Register 0
#define DDR_PHY_RDIMMGCR1					(SIFLOWER_DDR_PHY_BASE+0x0b4)		//RDIMM General Configuration Register 1
#define DDR_PHY_RDIMMCR0					(SIFLOWER_DDR_PHY_BASE+0x0b8)		//RDIMM Control Register 0
#define DDR_PHY_RDIMMCR1					(SIFLOWER_DDR_PHY_BASE+0x0bc)		//RDIMM Control Register 1
#define DDR_PHY_DCUAR                   	(SIFLOWER_DDR_PHY_BASE+0x0c0)		//DCU Address Register
#define DDR_PHY_DCUDR                   	(SIFLOWER_DDR_PHY_BASE+0x0c4)		//DCU Data Register
#define DDR_PHY_DCURR                   	(SIFLOWER_DDR_PHY_BASE+0x0c8)		//DCU Run Register
#define DDR_PHY_DCULR                   	(SIFLOWER_DDR_PHY_BASE+0x0cc)		//DCU Loop Register
#define DDR_PHY_DCUGCR                  	(SIFLOWER_DDR_PHY_BASE+0x0d0)		//DCU General Configuration Register
#define DDR_PHY_DCUTPR                  	(SIFLOWER_DDR_PHY_BASE+0x0d4)		//DCU Timing Rarameter Register
#define DDR_PHY_DCUSR0                  	(SIFLOWER_DDR_PHY_BASE+0x0d8)		//DCU Status Register 0
#define DDR_PHY_DCUSR1                  	(SIFLOWER_DDR_PHY_BASE+0x0dc)		//DCU Status Register 1
#define DDR_PHY_BISTRR                  	(SIFLOWER_DDR_PHY_BASE+0x100)		//BIST Run Register
#define DDR_PHY_BISTWCR                 	(SIFLOWER_DDR_PHY_BASE+0x104)		//BIST Word Count Register
#define DDR_PHY_BISTMSKR0               	(SIFLOWER_DDR_PHY_BASE+0x108)		//BIST Mask Register 0
#define DDR_PHY_BISTMSKR1               	(SIFLOWER_DDR_PHY_BASE+0x10c)		//BIST Mask Register 1
#define DDR_PHY_BISTMSKR2                 	(SIFLOWER_DDR_PHY_BASE+0x110)		//BIST Mask Register 2
#define DDR_PHY_BISTLSR                 	(SIFLOWER_DDR_PHY_BASE+0x114)		//BIST LFSR Seed Register
#define DDR_PHY_BISTAR0                 	(SIFLOWER_DDR_PHY_BASE+0x118)		//BIST Address Register 0
#define DDR_PHY_BISTAR1                 	(SIFLOWER_DDR_PHY_BASE+0x11c)		//BIST Address Register 1
#define DDR_PHY_BISTAR2                 	(SIFLOWER_DDR_PHY_BASE+0x120)		//BIST Address Register 2
#define DDR_PHY_BISTUDPR                	(SIFLOWER_DDR_PHY_BASE+0x124)		//BIST User Data Pattern Register
#define DDR_PHY_BISTGSR                 	(SIFLOWER_DDR_PHY_BASE+0x128)		//BIST General Status Register
#define DDR_PHY_BISTWER                 	(SIFLOWER_DDR_PHY_BASE+0x12c)		//BIST Word Error Register
#define DDR_PHY_BISTBER0                	(SIFLOWER_DDR_PHY_BASE+0x130)		//BIST Bit Error Register 0
#define DDR_PHY_BISTBER1                	(SIFLOWER_DDR_PHY_BASE+0x134)		//BIST Bit Error Register 1
#define DDR_PHY_BISTBER2                	(SIFLOWER_DDR_PHY_BASE+0x138)		//BIST Bit Error Register 2
#define DDR_PHY_BISTBER3					(SIFLOWER_DDR_PHY_BASE+0x13c)		//BIST Bit Error Register 3
#define DDR_PHY_BISTWCSR                	(SIFLOWER_DDR_PHY_BASE+0x140)		//BIST Word Count Status Register
#define DDR_PHY_BISTFWR0                	(SIFLOWER_DDR_PHY_BASE+0x144)		//BIST Fail Word Register 0
#define DDR_PHY_BISTFWR1                	(SIFLOWER_DDR_PHY_BASE+0x148)		//BIST Fail Word Register 1
#define DDR_PHY_BISTFWR2					(SIFLOWER_DDR_PHY_BASE+0x14c)		//BIST Fail Word Register 2
#define DDR_PHY_AACR                		(SIFLOWER_DDR_PHY_BASE+0x174)		//Anti-Aging Control Register
#define DDR_PHY_GPR0                		(SIFLOWER_DDR_PHY_BASE+0x178)		//General Purpose Register 0
#define DDR_PHY_GPR1                		(SIFLOWER_DDR_PHY_BASE+0x17c)		//General Purpose Register 1
#define DDR_PHY_ZQ0CR0                  	(SIFLOWER_DDR_PHY_BASE+0x180)		//ZQ 0 Impedance Control Register 0
#define DDR_PHY_ZQ0CR1                  	(SIFLOWER_DDR_PHY_BASE+0x184)		//ZQ 0 Impedance Control Register 1
#define DDR_PHY_ZQ0SR0                  	(SIFLOWER_DDR_PHY_BASE+0x188)		//ZQ 0 Impedance Status Register 0
#define DDR_PHY_ZQ0SR1                  	(SIFLOWER_DDR_PHY_BASE+0x18c)		//ZQ 0 Impedance Status Register 1
#define DDR_PHY_ZQ1CR0                  	(SIFLOWER_DDR_PHY_BASE+0x190)		//ZQ 1 Impedance Control Register 0
#define DDR_PHY_ZQ1CR1                  	(SIFLOWER_DDR_PHY_BASE+0x194)		//ZQ 1 Impedance Control Register 1
#define DDR_PHY_ZQ1SR0                  	(SIFLOWER_DDR_PHY_BASE+0x198)		//ZQ 1 Impedance Status Register 0
#define DDR_PHY_ZQ1SR1                  	(SIFLOWER_DDR_PHY_BASE+0x19c)		//ZQ 1 Impedance Status Register 1
#define DDR_PHY_ZQ2CR0                  	(SIFLOWER_DDR_PHY_BASE+0x1a0)		//ZQ 2 Impedance Control Register 0
#define DDR_PHY_ZQ2CR1                  	(SIFLOWER_DDR_PHY_BASE+0x1a4)		//ZQ 2 Impedance Control Register 1
#define DDR_PHY_ZQ2SR0                  	(SIFLOWER_DDR_PHY_BASE+0x1a8)		//ZQ 2 Impedance Status Register 0
#define DDR_PHY_ZQ2SR1                  	(SIFLOWER_DDR_PHY_BASE+0x1ac)		//ZQ 2 Impedance Status Register 1
#define DDR_PHY_ZQ3CR0                  	(SIFLOWER_DDR_PHY_BASE+0x1b0)		//ZQ 3 Impedance Control Register 0
#define DDR_PHY_ZQ3CR1                  	(SIFLOWER_DDR_PHY_BASE+0x1b4)		//ZQ 3 Impedance Control Register 1
#define DDR_PHY_ZQ3SR0                  	(SIFLOWER_DDR_PHY_BASE+0x1b8)		//ZQ 3 Impedance Status Register 0
#define DDR_PHY_ZQ3SR1                  	(SIFLOWER_DDR_PHY_BASE+0x1bc)		//ZQ 3 Impedance Status Register 1
#define DDR_PHY_DX0GCR                   	(SIFLOWER_DDR_PHY_BASE+0x1c0)		//DATX8 0 General Configuration Register
#define DDR_PHY_DX0GSR0                 	(SIFLOWER_DDR_PHY_BASE+0x1c4)		//DATX8 0 General Status Register 0
#define DDR_PHY_DX0GSR1                 	(SIFLOWER_DDR_PHY_BASE+0x1c8)		//DATX8 0 General Status Register 1
#define DDR_PHY_DX0BDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x1cc)		//DATX8 0 Bit Delay Line Register 0
#define DDR_PHY_DX0BDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x1d0)		//DATX8 0 Bit Delay Line Register 1
#define DDR_PHY_DX0BDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x1d4)		//DATX8 0 Bit Delay Line Register 2
#define DDR_PHY_DX0BDLR3                 	(SIFLOWER_DDR_PHY_BASE+0x1d8)		//DATX8 0 Bit Delay Line Register 3
#define DDR_PHY_DX0BDLR4                 	(SIFLOWER_DDR_PHY_BASE+0x1dc)		//DATX8 0 Bit Delay Line Register 4
#define DDR_PHY_DX0LCDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x1e0)		//DATX8 0 Local Calibrated Delay Line Register 0
#define DDR_PHY_DX0LCDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x1e4)		//DATX8 0 Local Calibrated Delay Line Register 1
#define DDR_PHY_DX0LCDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x1e8)		//DATX8 0 Local Calibrated Delay Line Register 2
#define DDR_PHY_DX0MDLR                 	(SIFLOWER_DDR_PHY_BASE+0x1ec)		//DATX8 0 Master Delay Line Register
#define DDR_PHY_DX0GTR						(SIFLOWER_DDR_PHY_BASE+0x1f0)		//DATX8 0 General Timing Register
#define DDR_PHY_DX0GSR2                     (SIFLOWER_DDR_PHY_BASE+0x1f4)		//DATX8 0 General Status Register 2
#define DDR_PHY_DX1GCR                  	(SIFLOWER_DDR_PHY_BASE+0x200)		//DATX8 1 General Configuration Register
#define DDR_PHY_DX1GSR0                  	(SIFLOWER_DDR_PHY_BASE+0x204)		//DATX8 1 General Status Register 0
#define DDR_PHY_DX1GSR1                 	(SIFLOWER_DDR_PHY_BASE+0x208)		//DATX8 1 General Status Register 1
#define DDR_PHY_DX1BDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x20c)		//DATX8 1 Bit Delay Line Register 0
#define DDR_PHY_DX1BDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x210)		//DATX8 1 Bit Delay Line Register 1
#define DDR_PHY_DX1BDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x214)		//DATX8 1 Bit Delay Line Register 2
#define DDR_PHY_DX1BDLR3                 	(SIFLOWER_DDR_PHY_BASE+0x218)		//DATX8 1 Bit Delay Line Register 3
#define DDR_PHY_DX1BDLR4                 	(SIFLOWER_DDR_PHY_BASE+0x21c)		//DATX8 1 Bit Delay Line Register 4
#define DDR_PHY_DX1LCDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x220)		//DATX8 1 Local Calibrated Delay Line Register 0
#define DDR_PHY_DX1LCDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x224)		//DATX8 1 Local Calibrated Delay Line Register 1
#define DDR_PHY_DX1LCDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x228)		//DATX8 1 Local Calibrated Delay Line Register 2
#define DDR_PHY_DX1MDLR                 	(SIFLOWER_DDR_PHY_BASE+0x22c)		//DATX8 1 Master Delay Line Register
#define DDR_PHY_DX1GTR						(SIFLOWER_DDR_PHY_BASE+0x230)		//DATX8 1 General Timing Register
#define DDR_PHY_DX1GSR2                     (SIFLOWER_DDR_PHY_BASE+0x234)		//DATX8 1 General Status Register 2
#define DDR_PHY_DX2GCR                  	(SIFLOWER_DDR_PHY_BASE+0x240)		//DATX8 2 General Configuration Register
#define DDR_PHY_DX2GSR0                 	(SIFLOWER_DDR_PHY_BASE+0x244)		//DATX8 2 General Status Register 0
#define DDR_PHY_DX2GSR1                 	(SIFLOWER_DDR_PHY_BASE+0x248)		//DATX8 2 General Status Register 1
#define DDR_PHY_DX2BDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x24c)		//DATX8 2 Bit Delay Line Register 0
#define DDR_PHY_DX2BDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x250)		//DATX8 2 Bit Delay Line Register 1
#define DDR_PHY_DX2BDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x254)		//DATX8 2 Bit Delay Line Register 2
#define DDR_PHY_DX2BDLR3                 	(SIFLOWER_DDR_PHY_BASE+0x258)		//DATX8 2 Bit Delay Line Register 3
#define DDR_PHY_DX2BDLR4                 	(SIFLOWER_DDR_PHY_BASE+0x25c)		//DATX8 2 Bit Delay Line Register 4
#define DDR_PHY_DX2LCDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x260)		//DATX8 2 Local Calibrated Delay Line Register 0
#define DDR_PHY_DX2LCDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x264)		//DATX8 2 Local Calibrated Delay Line Register 1
#define DDR_PHY_DX2LCDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x268)		//DATX8 2 Local Calibrated Delay Line Register 2
#define DDR_PHY_DX2MDLR                 	(SIFLOWER_DDR_PHY_BASE+0x26c)		//DATX8 2 Master Delay Line Register
#define DDR_PHY_DX2GTR						(SIFLOWER_DDR_PHY_BASE+0x270)		//DATX8 2 General Timing Register
#define DDR_PHY_DX3GCR                  	(SIFLOWER_DDR_PHY_BASE+0x280)		//DATX8 3 General Configuration Register
#define DDR_PHY_DX3GSR0                 	(SIFLOWER_DDR_PHY_BASE+0x284)		//DATX8 3 General Status Register 0
#define DDR_PHY_DX3GSR1						(SIFLOWER_DDR_PHY_BASE+0x288)		//DATX8 3 General Status Register 1
#define DDR_PHY_DX3BDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x28c)		//DATX8 3 Bit Delay Line Register 0
#define DDR_PHY_DX3BDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x290)		//DATX8 3 Bit Delay Line Register 1
#define DDR_PHY_DX3BDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x294)		//DATX8 3 Bit Delay Line Register 2
#define DDR_PHY_DX3BDLR3                 	(SIFLOWER_DDR_PHY_BASE+0x298)		//DATX8 3 Bit Delay Line Register 3
#define DDR_PHY_DX3BDLR4                 	(SIFLOWER_DDR_PHY_BASE+0x29c)		//DATX8 3 Bit Delay Line Register 4
#define DDR_PHY_DX3LCDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x2a0)		//DATX8 3 Local Calibrated Delay Line Register 0
#define DDR_PHY_DX3LCDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x2a4)		//DATX8 3 Local Calibrated Delay Line Register 1
#define DDR_PHY_DX3LCDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x2a8)		//DATX8 3 Local Calibrated Delay Line Register 2
#define DDR_PHY_DX3MDLR                 	(SIFLOWER_DDR_PHY_BASE+0x2ac)		//DATX8 3 Master Delay Line Register
#define DDR_PHY_DX3GTR						(SIFLOWER_DDR_PHY_BASE+0x2b0)		//DATX8 3 General Timing Register
#define DDR_PHY_DX4GCR                  	(SIFLOWER_DDR_PHY_BASE+0x2c0)		//DATX8 4 General Configuration Register
#define DDR_PHY_DX4GSR0                 	(SIFLOWER_DDR_PHY_BASE+0x2c4)		//DATX8 4 General Status Register 0
#define DDR_PHY_DX4GSR1                 	(SIFLOWER_DDR_PHY_BASE+0x2c8)		//DATX8 4 General Status Register 1
#define DDR_PHY_DX4BDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x2cc)		//DATX8 4 Bit Delay Line Register 0
#define DDR_PHY_DX4BDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x2d0)		//DATX8 4 Bit Delay Line Register 1
#define DDR_PHY_DX4BDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x2d4)		//DATX8 4 Bit Delay Line Register 2
#define DDR_PHY_DX4BDLR3                 	(SIFLOWER_DDR_PHY_BASE+0x2d8)		//DATX8 4 Bit Delay Line Register 3
#define DDR_PHY_DX4BDLR4                 	(SIFLOWER_DDR_PHY_BASE+0x2dc)		//DATX8 4 Bit Delay Line Register 4
#define DDR_PHY_DX4LCDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x2e0)		//DATX8 4 Local Calibrated Delay Line Register 0
#define DDR_PHY_DX4LCDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x2e4)		//DATX8 4 Local Calibrated Delay Line Register 1
#define DDR_PHY_DX4LCDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x2e8)		//DATX8 4 Local Calibrated Delay Line Register 2
#define DDR_PHY_DX4MDLR                 	(SIFLOWER_DDR_PHY_BASE+0x2ec)		//DATX8 4 Master Delay Line Register
#define DDR_PHY_DX4GTR						(SIFLOWER_DDR_PHY_BASE+0x2f0)		//DATX8 4 General Timing Register
#define DDR_PHY_DX5GCR                  	(SIFLOWER_DDR_PHY_BASE+0x300)		//DATX8 5 General Configuration Register
#define DDR_PHY_DX5GSR0                 	(SIFLOWER_DDR_PHY_BASE+0x304)		//DATX8 5 General Status Register 0
#define DDR_PHY_DX5GSR1                 	(SIFLOWER_DDR_PHY_BASE+0x308)		//DATX8 5 General Status Register 1
#define DDR_PHY_DX5BDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x30c)		//DATX8 5 Bit Delay Line Register 0
#define DDR_PHY_DX5BDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x310)		//DATX8 5 Bit Delay Line Register 1
#define DDR_PHY_DX5BDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x314)		//DATX8 5 Bit Delay Line Register 2
#define DDR_PHY_DX5BDLR3                 	(SIFLOWER_DDR_PHY_BASE+0x318)		//DATX8 5 Bit Delay Line Register 3
#define DDR_PHY_DX5BDLR4                 	(SIFLOWER_DDR_PHY_BASE+0x31c)		//DATX8 5 Bit Delay Line Register 4
#define DDR_PHY_DX5LCDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x320)		//DATX8 5 Local Calibrated Delay Line Register 0
#define DDR_PHY_DX5LCDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x324)		//DATX8 5 Local Calibrated Delay Line Register 1
#define DDR_PHY_DX5LCDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x328)		//DATX8 5 Local Calibrated Delay Line Register 2
#define DDR_PHY_DX5MDLR                 	(SIFLOWER_DDR_PHY_BASE+0x32c)		//DATX8 5 Master Delay Line Register
#define DDR_PHY_DX5GTR						(SIFLOWER_DDR_PHY_BASE+0x330)		//DATX8 5 General Timing Register
#define DDR_PHY_DX6GCR                  	(SIFLOWER_DDR_PHY_BASE+0x340)		//DATX8 6 General Configuration Register
#define DDR_PHY_DX6GSR0                 	(SIFLOWER_DDR_PHY_BASE+0x344)		//DATX8 6 General Status Register 0
#define DDR_PHY_DX6GSR1                 	(SIFLOWER_DDR_PHY_BASE+0x348)		//DATX8 6 General Status Register 1
#define DDR_PHY_DX6BDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x34c)		//DATX8 6 Bit Delay Line Register 0
#define DDR_PHY_DX6BDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x350)		//DATX8 6 Bit Delay Line Register 1
#define DDR_PHY_DX6BDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x354)		//DATX8 6 Bit Delay Line Register 2
#define DDR_PHY_DX6BDLR3                 	(SIFLOWER_DDR_PHY_BASE+0x358)		//DATX8 6 Bit Delay Line Register 3
#define DDR_PHY_DX6BDLR4                 	(SIFLOWER_DDR_PHY_BASE+0x35c)		//DATX8 6 Bit Delay Line Register 4
#define DDR_PHY_DX6LCDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x360)		//DATX8 6 Local Calibrated Delay Line Register 0
#define DDR_PHY_DX6LCDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x364)		//DATX8 6 Local Calibrated Delay Line Register 1
#define DDR_PHY_DX6LCDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x368)		//DATX8 6 Local Calibrated Delay Line Register 2
#define DDR_PHY_DX6MDLR                 	(SIFLOWER_DDR_PHY_BASE+0x36c)		//DATX8 6 Master Delay Line Register
#define DDR_PHY_DX6GTR						(SIFLOWER_DDR_PHY_BASE+0x370)		//DATX8 6 General Timing Register
#define DDR_PHY_DX7GCR                  	(SIFLOWER_DDR_PHY_BASE+0x380)		//DATX8 7 General Configuration Register
#define DDR_PHY_DX7GSR0                 	(SIFLOWER_DDR_PHY_BASE+0x384)		//DATX8 7 General Status Register 0
#define DDR_PHY_DX7GSR1                 	(SIFLOWER_DDR_PHY_BASE+0x388)		//DATX8 7 General Status Register 1
#define DDR_PHY_DX7BDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x38c)		//DATX8 7 Bit Delay Line Register 0
#define DDR_PHY_DX7BDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x390)		//DATX8 7 Bit Delay Line Register 1
#define DDR_PHY_DX7BDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x394)		//DATX8 7 Bit Delay Line Register 2
#define DDR_PHY_DX7BDLR3                 	(SIFLOWER_DDR_PHY_BASE+0x398)		//DATX8 7 Bit Delay Line Register 3
#define DDR_PHY_DX7BDLR4                 	(SIFLOWER_DDR_PHY_BASE+0x39c)		//DATX8 7 Bit Delay Line Register 4
#define DDR_PHY_DX7LCDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x3a0)		//DATX8 7 Local Calibrated Delay Line Register 0
#define DDR_PHY_DX7LCDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x3a4)		//DATX8 7 Local Calibrated Delay Line Register 1
#define DDR_PHY_DX7LCDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x3a8)		//DATX8 7 Local Calibrated Delay Line Register 2
#define DDR_PHY_DX7MDLR                 	(SIFLOWER_DDR_PHY_BASE+0x3ac)		//DATX8 7 Master Delay Line Register
#define DDR_PHY_DX7GTR						(SIFLOWER_DDR_PHY_BASE+0x3b0)		//DATX8 7 General Timing Register
#define DDR_PHY_DX8GCR                  	(SIFLOWER_DDR_PHY_BASE+0x3c0)		//DATX8 8 General Configuration Register
#define DDR_PHY_DX8GSR0                 	(SIFLOWER_DDR_PHY_BASE+0x3c4)		//DATX8 8 General Status Register 0
#define DDR_PHY_DX8GSR1                 	(SIFLOWER_DDR_PHY_BASE+0x3c8)		//DATX8 8 General Status Register 1
#define DDR_PHY_DX8BDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x3cc)		//DATX8 8 Bit Delay Line Register 0
#define DDR_PHY_DX8BDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x3d0)		//DATX8 8 Bit Delay Line Register 1
#define DDR_PHY_DX8BDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x3d4)		//DATX8 8 Bit Delay Line Register 2
#define DDR_PHY_DX8BDLR3                 	(SIFLOWER_DDR_PHY_BASE+0x3d8)		//DATX8 8 Bit Delay Line Register 3
#define DDR_PHY_DX8BDLR4                 	(SIFLOWER_DDR_PHY_BASE+0x3dc)		//DATX8 8 Bit Delay Line Register 4
#define DDR_PHY_DX8LCDLR0                 	(SIFLOWER_DDR_PHY_BASE+0x3e0)		//DATX8 8 Local Calibrated Delay Line Register 0
#define DDR_PHY_DX8LCDLR1                 	(SIFLOWER_DDR_PHY_BASE+0x3e4)		//DATX8 8 Local Calibrated Delay Line Register 1
#define DDR_PHY_DX8LCDLR2                 	(SIFLOWER_DDR_PHY_BASE+0x3e8)		//DATX8 8 Local Calibrated Delay Line Register 2
#define DDR_PHY_DX8MDLR                 	(SIFLOWER_DDR_PHY_BASE+0x3ec)		//DATX8 8 Master Delay Line Register
#define DDR_PHY_DX8GTR						(SIFLOWER_DDR_PHY_BASE+0x3f0)		//DATX8 8 General Timing Register


//DWC uMCTL2 Register Map
#define DDR_UMCTL2_MSTR                  	(SIFLOWER_DDR_CTL_BASE+0x000)		//Master Register 0
#define DDR_UMCTL2_STAT						(SIFLOWER_DDR_CTL_BASE+0x004)		//Operating Mode Status Register, read only
#define DDR_UMCTL2_MSTR1					(SIFLOWER_DDR_CTL_BASE+0x008)		//Master Register 1
#define DDR_UMCTL2_MRCTRL0               	(SIFLOWER_DDR_CTL_BASE+0x010)		//Mode Register Read/Write Control Register 0
#define DDR_UMCTL2_MRCTRL1               	(SIFLOWER_DDR_CTL_BASE+0x014)		//Mode Register Read/Write Control Register 1
#define DDR_UMCTL2_MRSTAT                	(SIFLOWER_DDR_CTL_BASE+0x018)		//Mode Register Read/Write Status Register, read only
#define DDR_UMCTL2_MRCTRL2					(SIFLOWER_DDR_CTL_BASE+0x01c)		//Mode Register Read/Write Control Register 2
#define DDR_UMCTL2_DERATEEN              	(SIFLOWER_DDR_CTL_BASE+0x020)		//Temperature Derate Enable Register, LPDDR2
#define DDR_UMCTL2_DERATEINT             	(SIFLOWER_DDR_CTL_BASE+0x024)		//Temperature Derate Interval Register, LPDDR2
#define DDR_UMCTL2_PWRCTL                	(SIFLOWER_DDR_CTL_BASE+0x030)		//Low Power Control Register
#define DDR_UMCTL2_PWRTMG                	(SIFLOWER_DDR_CTL_BASE+0x034)		//Low Power Timing Register
#define DDR_UMCTL2_HWLPCTL					(SIFLOWER_DDR_CTL_BASE+0x038)		//Hardware Low Power Control Register
#define DDR_UMCTL2_RFSHCTL0              	(SIFLOWER_DDR_CTL_BASE+0x050)		//Refresh Control Register 0, use default
#define DDR_UMCTL2_RFSHCTL1              	(SIFLOWER_DDR_CTL_BASE+0x054)		//Refresh Control Register 1, RANK>=2
#define DDR_UMCTL2_RFSHCTL2              	(SIFLOWER_DDR_CTL_BASE+0x058)		//Refresh Control Register 2, RANK=4
#define DDR_UMCTL2_RFSHCTL3              	(SIFLOWER_DDR_CTL_BASE+0x060)		//Refresh Control Register 3, use default
#define DDR_UMCTL2_RFSHTMG               	(SIFLOWER_DDR_CTL_BASE+0x064)		//Refresh Timing Register
#define DDR_UMCTL2_CRCPARCTL0				(SIFLOWER_DDR_CTL_BASE+0x0c0)		//CRC Parity Control Register 0
#define DDR_UMCTL2_CRCPARCTL1               (SIFLOWER_DDR_CTL_BASE+0x0c4)		//CRC Parity Control Register 1
#define DDR_UMCTL2_CRCPARCTL2				(SIFLOWER_DDR_CTL_BASE+0x0c8)		//CRC Parity Control Register 2
#define DDR_UMCTL2_CRCPARSTAT               (SIFLOWER_DDR_CTL_BASE+0x0cc)		//CRC Parity Status Register
#define DDR_UMCTL2_INIT0                 	(SIFLOWER_DDR_CTL_BASE+0x0D0)		//SDRAM Initialization Register 0
#define DDR_UMCTL2_INIT1                 	(SIFLOWER_DDR_CTL_BASE+0x0D4)		//SDRAM Initialization Register 1
#define DDR_UMCTL2_INIT2                 	(SIFLOWER_DDR_CTL_BASE+0x0D8)		//SDRAM Initialization Register 2, LPDDR2
#define DDR_UMCTL2_INIT3                 	(SIFLOWER_DDR_CTL_BASE+0x0DC)		//SDRAM Initialization Register 3
#define DDR_UMCTL2_INIT4                 	(SIFLOWER_DDR_CTL_BASE+0x0E0)		//SDRAM Initialization Register 4
#define DDR_UMCTL2_INIT5                 	(SIFLOWER_DDR_CTL_BASE+0x0E4)		//SDRAM Initialization Register 5, LPDDR2/DDR3
#define DDR_UMCTL2_DIMMCTL               	(SIFLOWER_DDR_CTL_BASE+0x0F0)		//DIMM Control Register
#define DDR_UMCTL2_RANKCTL               	(SIFLOWER_DDR_CTL_BASE+0x0F4)		//Rank Control Register, RANK>1
#define DDR_UMCTL2_DRAMTMG0              	(SIFLOWER_DDR_CTL_BASE+0x100)		//SDRAM Timing Register 0
#define DDR_UMCTL2_DRAMTMG1              	(SIFLOWER_DDR_CTL_BASE+0x104)		//SDRAM Timing Register 1
#define DDR_UMCTL2_DRAMTMG2              	(SIFLOWER_DDR_CTL_BASE+0x108)		//SDRAM Timing Register 2
#define DDR_UMCTL2_DRAMTMG3              	(SIFLOWER_DDR_CTL_BASE+0x10C)		//SDRAM Timing Register 3
#define DDR_UMCTL2_DRAMTMG4              	(SIFLOWER_DDR_CTL_BASE+0x110)		//SDRAM Timing Register 4
#define DDR_UMCTL2_DRAMTMG5              	(SIFLOWER_DDR_CTL_BASE+0x114)		//SDRAM Timing Register 5
#define DDR_UMCTL2_DRAMTMG6              	(SIFLOWER_DDR_CTL_BASE+0x118)		//SDRAM Timing Register 6
#define DDR_UMCTL2_DRAMTMG7              	(SIFLOWER_DDR_CTL_BASE+0x11C)		//SDRAM Timing Register 7
#define DDR_UMCTL2_DRAMTMG8              	(SIFLOWER_DDR_CTL_BASE+0x120)		//SDRAM Timing Register 8
#define DDR_UMCTL2_ZQCTL0                	(SIFLOWER_DDR_CTL_BASE+0x180)		//ZQ Control Register 0, LPDDR2/DDR3
#define DDR_UMCTL2_ZQCTL1                	(SIFLOWER_DDR_CTL_BASE+0x184)		//ZQ Control Register 1, LPDDR2/DDR3
#define DDR_UMCTL2_ZQCTL2                	(SIFLOWER_DDR_CTL_BASE+0x188)		//ZQ Control Register 2, LPDDR2
#define DDR_UMCTL2_ZQSTAT                	(SIFLOWER_DDR_CTL_BASE+0x18C)		//ZQ Status Register, LPDDR2
#define DDR_UMCTL2_DFITMG0               	(SIFLOWER_DDR_CTL_BASE+0x190)		//DFI Timing Register 0
#define DDR_UMCTL2_DFITMG1               	(SIFLOWER_DDR_CTL_BASE+0x194)		//DFI Timing Register 1
#define DDR_UMCTL2_DFILPCFG0				(SIFLOWER_DDR_CTL_BASE+0x198)		//DFI Low Power Configuration Register 0
#define DDR_UMCTL2_DFILPCFG1				(SIFLOWER_DDR_CTL_BASE+0x19C)		//DFI Low Power Configuration Register 1
#define DDR_UMCTL2_DFIUPD0               	(SIFLOWER_DDR_CTL_BASE+0x1A0)		//DFI Update Register 0
#define DDR_UMCTL2_DFIUPD1               	(SIFLOWER_DDR_CTL_BASE+0x1A4)		//DFI Update Register 1
#define DDR_UMCTL2_DFIUPD2               	(SIFLOWER_DDR_CTL_BASE+0x1A8)		//DFI Update Register 2
#define DDR_UMCTL2_DFIMISC               	(SIFLOWER_DDR_CTL_BASE+0x1B0)		//DFI Miscellaneous Control Register
#define DDR_UMCTL2_DFITMG2					(SIFLOWER_DDR_CTL_BASE+0x1B4)		//DFI Timing Register 2
#define DDR_UMCTL2_DFITMG3					(SIFLOWER_DDR_CTL_BASE+0x1B8)		//DFI Timing Register 3
#define DDR_UMCTL2_DBICTL					(SIFLOWER_DDR_CTL_BASE+0x1C0)		//DM/DBI Control Register
#define DDR_UMCTL2_ADDRMAP0              	(SIFLOWER_DDR_CTL_BASE+0x200)		//Address Map Register 0
#define DDR_UMCTL2_ADDRMAP1              	(SIFLOWER_DDR_CTL_BASE+0x204)		//Address Map Register 1
#define DDR_UMCTL2_ADDRMAP2              	(SIFLOWER_DDR_CTL_BASE+0x208)		//Address Map Register 2
#define DDR_UMCTL2_ADDRMAP3              	(SIFLOWER_DDR_CTL_BASE+0x20C)		//Address Map Register 3
#define DDR_UMCTL2_ADDRMAP4              	(SIFLOWER_DDR_CTL_BASE+0x210)		//Address Map Register 4
#define DDR_UMCTL2_ADDRMAP5              	(SIFLOWER_DDR_CTL_BASE+0x214)		//Address Map Register 5
#define DDR_UMCTL2_ADDRMAP6              	(SIFLOWER_DDR_CTL_BASE+0x218)		//Address Map Register 6
#define DDR_UMCTL2_ADDRMAP7					(SIFLOWER_DDR_CTL_BASE+0x21C)		//Address Map Register 7
#define DDR_UMCTL2_ADDRMAP8                 (SIFLOWER_DDR_CTL_BASE+0x220)		//Address Map Register 8
#define DDR_UMCTL2_ADDRMAP9                 (SIFLOWER_DDR_CTL_BASE+0x224)		//Address Map Register 9
#define DDR_UMCTL2_ADDRMAP10				(SIFLOWER_DDR_CTL_BASE+0x228)		//Address Map Register 10
#define DDR_UMCTL2_ADDRMAP11                (SIFLOWER_DDR_CTL_BASE+0x22c)		//Address Map Register 11
#define DDR_UMCTL2_ODTCFG                	(SIFLOWER_DDR_CTL_BASE+0x240)		//ODT Configuration Register
#define DDR_UMCTL2_ODTMAP                	(SIFLOWER_DDR_CTL_BASE+0x244)		//ODT/Rank Map Register
#define DDR_UMCTL2_SCHED                 	(SIFLOWER_DDR_CTL_BASE+0x250)		//Scheduler Control Register
#define DDR_UMCTL2_SCHED1					(SIFLOWER_DDR_CTL_BASE+0x254)		//Scheduler Control Register 1
#define DDR_UMCTL2_PERFHPR1              	(SIFLOWER_DDR_CTL_BASE+0x25C)		//High Priority Read CAM Register 1
#define DDR_UMCTL2_PERFLPR1              	(SIFLOWER_DDR_CTL_BASE+0x264)		//Low Priority Read CAM Register 1
#define DDR_UMCTL2_PERFWR1               	(SIFLOWER_DDR_CTL_BASE+0x26C)		//Write CAM Register 1
#define DDR_UMCTL2_PERFVPR1					(SIFLOWER_DDR_CTL_BASE+0x274)		//Variable Priority Read CAM Register 1
#define DDR_UMCTL2_PERFVPW1					(SIFLOWER_DDR_CTL_BASE+0x278)		//Variable Priority Write CAM Register 1
#define DDR_UMCTL2_DBG0                  	(SIFLOWER_DDR_CTL_BASE+0x300)		//Debug Register 0
#define DDR_UMCTL2_DBG1                  	(SIFLOWER_DDR_CTL_BASE+0x304)		//Debug Register 1
#define DDR_UMCTL2_DBGCAM                	(SIFLOWER_DDR_CTL_BASE+0x308)		//CAM Debug Register
#define DDR_UMCTL2_DBGCMD					(SIFLOWER_DDR_CTL_BASE+0x30C)		//Command Debug Register
#define DDR_UMCTL2_DBGSTAT					(SIFLOWER_DDR_CTL_BASE+0x310)		//Status Debug Register
#define DDR_UMCTL2_SWCTL					(SIFLOWER_DDR_CTL_BASE+0x320)		//Software Register Programming Control Enable
#define DDR_UMCTL2_SWSTAT					(SIFLOWER_DDR_CTL_BASE+0x324)		//Software Register Programming Control Status
#define DDR_UMCTL2_POISONCFG				(SIFLOWER_DDR_CTL_BASE+0x36c)		//AXI Poison Configuration Register
#define DDR_UMCTL2_POISONSTAT				(SIFLOWER_DDR_CTL_BASE+0x370)		//AXI Poison Status Register
#define DDR_UMCTL2_PSTAT                    (SIFLOWER_DDR_CTL_BASE+0x3fc)		//Port Status Register, read only
#define DDR_UMCTL2_PCCFG					(SIFLOWER_DDR_CTL_BASE+0x400)		//Port Common Configuration Register
#define DDR_UMCTL2_PCFGR_0					(SIFLOWER_DDR_CTL_BASE+0x404)		//Port 0 Configuration Read Register
#define DDR_UMCTL2_PCFGW_0					(SIFLOWER_DDR_CTL_BASE+0x408)		//Port 0 Configuration Write Register
#define DDR_UMCTL2_PCTRL_0					(SIFLOWER_DDR_CTL_BASE+0x490)		//Port 0 Control Register
#define DDR_UMCTL2_PCFGQOS0_0				(SIFLOWER_DDR_CTL_BASE+0x494)		//Port 0 Read QoS Configuration Register 0
#define DDR_UMCTL2_PCFGQOS1_0               (SIFLOWER_DDR_CTL_BASE+0x498)		//Port 0 Read QoS Configuration Register 1
#define DDR_UMCTL2_PCFGWQOS0_0				(SIFLOWER_DDR_CTL_BASE+0x49C)		//Port 0 Write QoS Configuration Register 0
#define DDR_UMCTL2_PCFGWQOS1_0              (SIFLOWER_DDR_CTL_BASE+0x4A0)		//Port 0 Write QoS Configuration Register 1
#define DDR_UMCTL2_PCFGR_1					(SIFLOWER_DDR_CTL_BASE+0x4B4)		//Port 1 Configuration Read Register
#define DDR_UMCTL2_PCFGW_1					(SIFLOWER_DDR_CTL_BASE+0x4B8)		//Port 1 Configuration Write Register
#define DDR_UMCTL2_PCTRL_1					(SIFLOWER_DDR_CTL_BASE+0x540)		//Port 1 Control Register
#define DDR_UMCTL2_PCFGQOS0_1               (SIFLOWER_DDR_CTL_BASE+0x544)		//Port 1 Read QoS Configuration Register 0
#define DDR_UMCTL2_PCFGQOS1_1               (SIFLOWER_DDR_CTL_BASE+0x548)		//Port 1 Read QoS Configuration Register 1
#define DDR_UMCTL2_PCFGWQOS0_1              (SIFLOWER_DDR_CTL_BASE+0x54C)		//Port 1 Write QoS Configuration Register 0
#define DDR_UMCTL2_PCFGWQOS1_1              (SIFLOWER_DDR_CTL_BASE+0x550)		//Port 1 Write QoS Configuration Register 1
#define DDR_UMCTL2_PCFGR_2					(SIFLOWER_DDR_CTL_BASE+0x564)		//Port 2 Configuration Read Register
#define DDR_UMCTL2_PCFGW_2					(SIFLOWER_DDR_CTL_BASE+0x568)		//Port 2 Configuration Write Register
#define DDR_UMCTL2_PCTRL_2					(SIFLOWER_DDR_CTL_BASE+0x5f0)		//Port 2 Control Register
#define DDR_UMCTL2_PCFGQOS0_2               (SIFLOWER_DDR_CTL_BASE+0x5f4)		//Port 2 Read QoS Configuration Register 0
#define DDR_UMCTL2_PCFGQOS1_2               (SIFLOWER_DDR_CTL_BASE+0x5f8)		//Port 2 Read QoS Configuration Register 1
#define DDR_UMCTL2_PCFGWQOS0_2              (SIFLOWER_DDR_CTL_BASE+0x5fC)		//Port 2 Write QoS Configuration Register 0
#define DDR_UMCTL2_PCFGWQOS1_2              (SIFLOWER_DDR_CTL_BASE+0x600)		//Port 2 Write QoS Configuration Register 1
#define DDR_UMCTL2_PCFGR_3					(SIFLOWER_DDR_CTL_BASE+0x614)		//Port 3 Configuration Read Register
#define DDR_UMCTL2_PCFGW_3					(SIFLOWER_DDR_CTL_BASE+0x618)		//Port 3 Configuration Write Register
#define DDR_UMCTL2_PCTRL_3					(SIFLOWER_DDR_CTL_BASE+0x6a0)		//Port 3 Control Register
#define DDR_UMCTL2_PCFGQOS0_3               (SIFLOWER_DDR_CTL_BASE+0x6a4)		//Port 3 Read QoS Configuration Register 0
#define DDR_UMCTL2_PCFGQOS1_3               (SIFLOWER_DDR_CTL_BASE+0x6a8)		//Port 3 Read QoS Configuration Register 1
#define DDR_UMCTL2_PCFGWQOS0_3              (SIFLOWER_DDR_CTL_BASE+0x6ac)		//Port 3 Write QoS Configuration Register 0
#define DDR_UMCTL2_PCFGWQOS1_3              (SIFLOWER_DDR_CTL_BASE+0x6b0)		//Port 3 Write QoS Configuration Register 1

#define SHADOW			0x2000				//the offset of shadow register

struct ddr_timing
{
	u32 tck;
	u32 mr[4];
	u32 cl;
	u32 al;
	u32 cwl;
	u32 wl;
	u32 rl;
	u32 trefi;
	u32 tfaw;
	u32 trc;
	u32 twr;
	u32 trfc;
	u32 trcd;
	u32 trp;
	u32 tras_min;
	u32 tras_max;
	u32 tras;
	u32 twlo;
	u32 twlmrd;
	u32 tmod;
	u32 tmrd;
	u32 tccd;
	u32 tdllk;
	u32 txs;
	u32 txpr;
	u32 txsdll;
	u32 tzqinit;
	u32 tzqoper;
	u32 tzqcs;
	u32 txpdll;
	u32 txp;
	u32 tcksre;
	u32 tcksrx;
	u32 trrd;
	u32 tcke;
	u32 twtr;
	u32 trtp;
	u32 tckesr;
};

struct ddr_info
{
	u16 size;
	u16 freq;
	u8 bl;
	u8 bank;
	struct ddr_timing *timing;
};

#define BIT(n) (1 << (n))
/*
 * Create a contiguous bitmask starting at bit position @l and ending at
 * position @h. For example
 * GENMASK_ULL(39, 21) gives us the 64bit vector 0x000000ffffe00000.
 */
#define GENMASK(h, l) \
	(((~0UL) << (l)) & (~0UL >> (BITS_PER_LONG - 1 - (h))))

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define DIV_ROUND_UP(n, d)	(((n) + (d) - 1) / (d))
#define ps_to_tck(t) DIV_ROUND_UP(t, Tck)
#define ns_to_tck(t) ps_to_tck(t * 1000)
#define us_to_tck(t) ns_to_tck(t * 1000)
#define tck_to_uctl2(n) DIV_ROUND_UP(n, 2)
#define tck_to_x1024(n) tck_to_uctl2(DIV_ROUND_UP(n, 1024))
#define tck_to_x32(n) tck_to_uctl2(DIV_ROUND_UP(n, 32))

#define set_bits(bits, addr)				\
	do {	u32 data = readl((void *)(addr));	\
		data |= (bits);				\
		writel(data, (void *)(addr));     	\
	} while(0)

#define clear_bits(bits, addr)			\
	do {  u32 data = readl((void *)(addr));	\
	   data &= ~(bits);			\
	   writel(data, (void *)(addr));     	\
	} while(0)

#define set_clear_bits(sets, clears, addr)	\
	do {  u32 data = readl((void *)(addr)); \
	   data |= (sets);			\
	   data &= ~(clears);			\
	   writel(data, (void *)(addr));	\
	} while(0)

void ddr_init(void);
void set_umctl2_reg(struct ddr_info *ddr);
void ddr_phy_init(struct ddr_info *ddr);
void umctl2_init_ddr(void);
void ddr_training(void);
void set_ddr_clock(u32 freq);
void ddr_common_init(struct ddr_info *ddr);
void ddr_set_timing(struct ddr_info *ddr);
#ifdef MPW0
void set_pll_ratio(u32 freq, u8 need_save, u8 push_phase);
#else
void set_pll_ratio(u32 freq);
#endif

#endif
