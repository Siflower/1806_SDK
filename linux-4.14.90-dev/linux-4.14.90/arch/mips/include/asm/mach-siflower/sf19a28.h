/*
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 2000 MIPS Technologies, Inc.  All rights reserved.
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * Defines of the sf16a18 soc specific address-MAP, registers, etc.
 */
#ifndef __ASM_MIPS_SF19A28_H
#define __ASM_MIPS_SF19A28_H

#include <asm/addrspace.h>
#include <asm/io.h>

/*
 * GCMP Specific definitions
 */
#define GCMP_BASE_ADDR			0x1fbf8000
#define GCMP_ADDRSPACE_SZ		(256 * 1024)

/*
 * GIC Specific definitions
 */
#define GIC_BASE_ADDR			0x1bdc0000
#define GIC_ADDRSPACE_SZ		(128 * 1024)

/*
 * CPC Specific definitions
 */
#define CPC_BASE_ADDR           0x1bde0000

/*system controller address*/
#define SIFLOWER_SYSCTL_BASE    0xB9E00000
#define SIFLOWER_SW_RESET       (SIFLOWER_SYSCTL_BASE + 0x2030)
#define SIFLOWER_SW_TRIG_RESET  0x1

/*
 * These are used for sys reboot event record when
 * syscon driver not initialised.
 */
#define SOFT_EVENTS_RO			(SIFLOWER_SYSCTL_BASE + 0x2054) //soft reboot
#define SYS_EVTS70			(SIFLOWER_SYSCTL_BASE + 0x2058)
#define SYS_EVTS158			(SIFLOWER_SYSCTL_BASE + 0x205C)
#define SYS_EVTS2316			(SIFLOWER_SYSCTL_BASE + 0x2060)
#define SYS_EVTS3124			(SIFLOWER_SYSCTL_BASE + 0x2064)
#define SYS_EVTS3932			(SIFLOWER_SYSCTL_BASE + 0x2068)
#define SYS_EVTS4740			(SIFLOWER_SYSCTL_BASE + 0x206C)
#define SYS_EVTS5548			(SIFLOWER_SYSCTL_BASE + 0x2070)
#define SYS_EVTS6356			(SIFLOWER_SYSCTL_BASE + 0x2074) //bit 63:wdt; bit 62: catip

//M_SFDSP syscon
#define M_SFDSP_0_POR_RESETN                      (SIFLOWER_SYSCTL_BASE + 0x30000)
//don't reset ARESETN to zero by single when m_SFDSP is powerup,the b1m2 reset will reset m_SFDSP state in siflower wireless driver
#define M_SFDSP_0_ARESETN                         (SIFLOWER_SYSCTL_BASE + 0x30004)
#define M_SFDSP_0_SI_ExceptionBase_12_19          (SIFLOWER_SYSCTL_BASE + 0x3003C)
#define M_SFDSP_0_SI_ExceptionBase_20_27          (SIFLOWER_SYSCTL_BASE + 0x30040)
#define M_SFDSP_0_SI_ExceptionBase_28             (SIFLOWER_SYSCTL_BASE + 0x30044)
#define M_SFDSP_1_POR_RESETN                      (SIFLOWER_SYSCTL_BASE + 0x30400)
#define M_SFDSP_1_ARESETN                         (SIFLOWER_SYSCTL_BASE + 0x30404)
#define M_SFDSP_1_SI_ExceptionBase_12_19          (SIFLOWER_SYSCTL_BASE + 0x3043C)
#define M_SFDSP_1_SI_ExceptionBase_20_27          (SIFLOWER_SYSCTL_BASE + 0x30440)
#define M_SFDSP_1_SI_ExceptionBase_28             (SIFLOWER_SYSCTL_BASE + 0x30444)
#define SYS_MISC_CTRL				(SIFLOWER_SYSCTL_BASE + 0x202C)

//module reset
#define SF_IRAM_SOFT_RESET      (0x0C00)
#define SF_IRAM_SYSM_RESET      (0x2C30)

#define SF_IIS_SOFT_RESET              (0x24000)
#define SF_IIS_SYSM_RESET       (0x02D44)

#define SF_PCM_SOFT_RESET              (0x25000)
#define SF_PCM_SYSM_RESET       (0x02D54)

#define SF_SPDIF_SOFT_RESET            (0x26000)
#define SF_SPDIF_SYSM_RESET     (0x02D64)


#define SF_GDU_SOFT_RESET              (0x4800)
#define SF_GDU_SYSM_RESET       (0x2C4C)

#define SF_CRYPTO_SOFT_RESET   (0x4C00)
#define SF_CRYPTO_SYSM_RESET    (0x2C50)

#define SF_EMMC_SOFT_RESET             (0xC400)
#define SF_EMMC_SYSM_RESET      (0x2C88)

#define SF_SDIO_SOFT_RESET             (0xC800)
#define SF_SDIO_SYSM_RESET      (0x2C8C)

#define SF_NPU_SOFT_RESET		(0x4000)
#define SF_NPU_SYSM_RESET       (0x2C44)

#define SF_EMAC_SOFT_RESET		(0x4400)
#define SF_EMAC_SYSM_RESET		(0x2C48)


#define SF_WIFI_1_SOFT_RESET	(0x8000)
#define SF_WIFI_1_SYSM_RESET    (0x2C64)

#define SF_WIFI_2_SOFT_RESET	(0xC400)
#define SF_WIFI_2_SYSM_RESET    (0x2C88)

#define SF_GDMA_SOFT_RESET		(0x8400)
#define SF_GDMA_SYSM_RESET      (0x2C68)

#define SF_SCDMA_SOFT_RESET		(0x8C00)
#define SF_SCDMA_SYSM_RESET     (0x2C70)

#define SF_USB_SOFT_RESET		(0xC000)
#define SF_USB_SYSM_RESET       (0x2C84)

#define SF_CATIP_SOFT_RESET		(0x8800)
#define SF_CATIP_SYSM_RESET     (0x2C6C)
//TODO, there are still some other config registers along with this one, to be added later
#define CATIP_MAIN_CLK_DEBUG_EN	(0x8C80)
#define WIFI_RF_960M_DIV32			            (SIFLOWER_SYSCTL_BASE + 0x8C80)  //[0:0] Active High
#define WIFI_RF_CLK_SOURCE_SEL			        (SIFLOWER_SYSCTL_BASE + 0x8C84)  //[0:0] Active High

#define SF_IIC_SOFT_RESET		(0x24400)
#define SF_IIC_SYSM_RESET       (0x02D48)

#define SF_SPI_SOFT_RESET		(0x24800)
#define SF_SPI_SYSM_RESET       (0x02D4C)

#define SF_UART_SOFT_RESET		(0x24C00)
#define SF_UART_SYSM_RESET      (0x02D50)

#define SF_PWM_SOFT_RESET		(0x25400)
#define SF_PWM_SYSM_RESET       (0x02D58)

#define SF_TIMER_SOFT_RESET		(0x25800)
#define SF_TIMER_SYSM_RESET     (0x02D5C)

#define SF_WDT_SOFT_RESET		(0x25C00)
#define SF_WDT_SYSM_RESET       (0x02D60)

extern int release_reset(unsigned int offset);
extern int release_reset_with_value(unsigned int offset,unsigned int initvalue);
extern int hold_reset(unsigned int offset);
extern u32 get_module_clk_gate(u32 soft_reset_offset, bool is_iram_clk1);
extern void set_module_clk_gate(u32 soft_reset_offset, u32 val, bool is_iram_clk1);

#endif /* __ASM_MIPS_BOARDS_SF19A28_H */
