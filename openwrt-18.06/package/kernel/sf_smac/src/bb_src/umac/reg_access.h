/**
 ******************************************************************************
 *
 * @file reg_access.h
 *
 * @brief Definitions and macros for MAC HW and platform register accesses
 *
 * Copyright (C) Siflower 2018-2025
 *
 ******************************************************************************
 */

#ifndef REG_ACCESS_H_
#define REG_ACCESS_H_

#include "siwifi_mod_params.h"

//fast define the wifi base address
#ifdef CONFIG_SFA28_FULLMASK
#define WIFI_BASE_ADDR(band) (band ? 0xB7800000 : 0xB1000000)
#else
#define WIFI_BASE_ADDR(band) (band ? 0xB1400000 : 0xB1000000)
#endif

/*****************************************************************************
 * Addresses within SIWIFI_ADDR_CPU
 *****************************************************************************/
#define RAM_LMAC_FW_ADDR               0x00000000
#define REG_MAC_CORE_BASE_ADDR(band) (WIFI_BASE_ADDR(band)+0x00080000)
#define REG_MDM_CFG_BASE_ADDR(band) (WIFI_BASE_ADDR(band)+0x00100000)
/*****************************************************************************
 * Addresses within SIWIFI_ADDR_SYSTEM
 *****************************************************************************/
/* Shard RAM */
#define SHARED_RAM_START_ADDR         0x00000000

//todo: ycchang check unuseful regs
//#define REG_SYSM_BUS2_RESET         (volatile void *)(0xB9E00000 + 0x2DD4)
//#define REG_SYSM_WIFI1_RESET        (volatile void *)(0xB9E00000 + 0x2C64)
//#define REG_SYSM_WIFI1_SOFT_RESET   (volatile void *)(0xB9E00000 + 0x8000)
//#define REG_SYSM_WIFI1_SOFT_CLKEN   (volatile void *)(0xB9E00000 + 0x8004)
//#define REG_SYSM_WIFI1_SOFT_BOE     (volatile void *)(0xB9E00000 + 0x800C)
#ifdef CONFIG_SFA28_FULLMASK
#define REG_SYSM_SHARE_RAM_SEL          (0xB9E00000 + 0x8844)//bit0:la_mode_from_24g;bit1:la_mode_from_5g
#define REG_SYSM_WIFI1_LA_CLK_SEL   (volatile void *)(0xB9E00000 + 0x8048)
#define WIFI_1_LA_THRES                         (0xB9E00000 + 0x8044)
#define WIFI_2_LA_THRES                         (0xB9E00000 + 0xC444)
#else
#define REG_SYSM_SHARE_RAM_SEL      (volatile void *)(0xB9E00000 + 0x242C)
#define REG_SYSM_WIFI1_LA_CLK_SEL   (volatile void *)(0xB9E00000 + 0x8040)
#endif

#define REG_SYSM_WIFI2_RESET        (volatile void *)(0xB9E00000 + 0x2C68)
//#define REG_SYSM_WIFI2_SOFT_RESET   (volatile void *)(0xB9E00000 + 0x8400)
//#define REG_SYSM_WIFI2_SOFT_CLKEN   (volatile void *)(0xB9E00000 + 0x8404)
//#define REG_SYSM_WIFI2_SOFT_BOE     (volatile void *)(0xB9E00000 + 0x840C)
#ifdef CONFIG_SFA28_FULLMASK
#define REG_SYSM_WIFI2_LA_CLK_SEL   (volatile void *)(0xB9E00000 + 0xC448)
#else
#define REG_SYSM_WIFI2_LA_CLK_SEL   (volatile void *)(0xB9E00000 + 0x8440)
#endif
//#define REG_SYSM_PAD_BASE           (volatile void *)(0xB9E00000 + 0x30000)

#define MEM_LB_SHARE_MEM_BASE       (volatile void *)(0xB1000000)

#ifdef CONFIG_SFA28_FULLMASK
#define MEM_HB_SHARE_MEM_BASE       (volatile void *)(0xB7800000)
#else
#define MEM_HB_SHARE_MEM_BASE       (volatile void *)(0xB1400000)
#endif

/* IPC registers */
#define IPC_REG_BASE_ADDR             0x000C0000

/* MAC platform */
#define REG_MAC_PL_BASE_ADDR(band) (WIFI_BASE_ADDR(band) + 0x00088000)
#define NXMAC_VERSION_1_ADDR           (REG_MAC_CORE_BASE_ADDR+0x00B00004)
#define   NXMAC_MU_MIMO_TX_BIT         BIT(19)
#define   NXMAC_BFMER_BIT              BIT(18)
#define   NXMAC_BFMEE_BIT              BIT(17)
#define   NXMAC_MAC_80211MH_FORMAT_BIT BIT(16)
#define   NXMAC_COEX_BIT               BIT(14)
#define   NXMAC_WAPI_BIT               BIT(13)
#define   NXMAC_TPC_BIT                BIT(12)
#define   NXMAC_VHT_BIT                BIT(11)
#define   NXMAC_HT_BIT                 BIT(10)
#define   NXMAC_RCE_BIT                BIT(8)
#define   NXMAC_CCMP_BIT               BIT(7)
#define   NXMAC_TKIP_BIT               BIT(6)
#define   NXMAC_WEP_BIT                BIT(5)
#define   NXMAC_SECURITY_BIT           BIT(4)
#define   NXMAC_SME_BIT                BIT(3)
#define   NXMAC_HCCA_BIT               BIT(2)
#define   NXMAC_EDCA_BIT               BIT(1)
#define   NXMAC_QOS_BIT                BIT(0)

//#define NXMAC_RX_CNTRL_ADDR                     0x00B00060
#define   NXMAC_EN_DUPLICATE_DETECTION_BIT      BIT(31)
#define   NXMAC_ACCEPT_UNKNOWN_BIT              BIT(30)
#define   NXMAC_ACCEPT_OTHER_DATA_FRAMES_BIT    BIT(29)
#define   NXMAC_ACCEPT_QO_S_NULL_BIT            BIT(28)
#define   NXMAC_ACCEPT_QCFWO_DATA_BIT           BIT(27)
#define   NXMAC_ACCEPT_Q_DATA_BIT               BIT(26)
#define   NXMAC_ACCEPT_CFWO_DATA_BIT            BIT(25)
#define   NXMAC_ACCEPT_DATA_BIT                 BIT(24)
#define   NXMAC_ACCEPT_OTHER_CNTRL_FRAMES_BIT   BIT(23)
#define   NXMAC_ACCEPT_CF_END_BIT               BIT(22)
#define   NXMAC_ACCEPT_ACK_BIT                  BIT(21)
#define   NXMAC_ACCEPT_CTS_BIT                  BIT(20)
#define   NXMAC_ACCEPT_RTS_BIT                  BIT(19)
#define   NXMAC_ACCEPT_PS_POLL_BIT              BIT(18)
#define   NXMAC_ACCEPT_BA_BIT                   BIT(17)
#define   NXMAC_ACCEPT_BAR_BIT                  BIT(16)
#define   NXMAC_ACCEPT_OTHER_MGMT_FRAMES_BIT    BIT(15)
#define   NXMAC_ACCEPT_BFMEE_FRAMES_BIT         BIT(14)
#define   NXMAC_ACCEPT_ALL_BEACON_BIT           BIT(13)
#define   NXMAC_ACCEPT_NOT_EXPECTED_BA_BIT      BIT(12)
#define   NXMAC_ACCEPT_DECRYPT_ERROR_FRAMES_BIT BIT(11)
#define   NXMAC_ACCEPT_BEACON_BIT               BIT(10)
#define   NXMAC_ACCEPT_PROBE_RESP_BIT           BIT(9)
#define   NXMAC_ACCEPT_PROBE_REQ_BIT            BIT(8)
#define   NXMAC_ACCEPT_MY_UNICAST_BIT           BIT(7)
#define   NXMAC_ACCEPT_UNICAST_BIT              BIT(6)
#define   NXMAC_ACCEPT_ERROR_FRAMES_BIT         BIT(5)
#define   NXMAC_ACCEPT_OTHER_BSSID_BIT          BIT(4)
#define   NXMAC_ACCEPT_BROADCAST_BIT            BIT(3)
#define   NXMAC_ACCEPT_MULTICAST_BIT            BIT(2)
#define   NXMAC_DONT_DECRYPT_BIT                BIT(1)
#define   NXMAC_EXC_UNENCRYPTED_BIT             BIT(0)

#define NXMAC_DEBUG_PORT_SEL_ADDR     (REG_MAC_PL_BASE_ADDR+0x510)
#define NXMAC_SW_SET_PROFILING_ADDR(band)   (REG_MAC_PL_BASE_ADDR(band)+0x564)
#define NXMAC_SW_CLEAR_PROFILING_ADDR(band) (REG_MAC_PL_BASE_ADDR(band)+0x568)

/*RIU*/
#define REG_RIU_BASE_ADDR(band) (WIFI_BASE_ADDR(band)+0x00100000)
#define RIU_AGCINBDPOW20PSTAT_ADDR(band)   (REG_RIU_BASE_ADDR(band)+0xB20C)
#define RIU_INBDPOW20PDBM0_MASK   ((uint32_t)0x000000FF)
#define RIU_INBDPOW20PDBM1_LSB    8
#define RIU_INBDPOW20PDBM1_MASK   ((uint32_t)0x0000FF00)
#define RIU_INBDPOW20PDBM0_LSB    0
/* Modem Status */
#define MDM_HDMCONFIG_ADDR             0x00C00000

/* Clock gating configuration */
#define MDM_MEMCLKCTRL0_ADDR           0x00C00848
#define CRM_CLKGATEFCTRL0_ADDR         0x00940010

/* AGC (trident) */
#define AGC_SIWIFIAGCCNTL_ADDR           0x00C02060

/* LDPC RAM*/
#define PHY_LDPC_RAM_ADDR              0x00C09000

/* FCU (elma )*/
#define FCU_SIWIFIFCAGCCNTL_ADDR         0x00C09034

/* AGC RAM */
#define PHY_AGC_UCODE_ADDR             0x00C0A000

/* FCU RAM */
#define PHY_FCU_UCODE_ADDR             0x00C0E000

/* RF ITF */
#define FPGAB_MPIF_SEL_ADDR            0x00C10030
#define RF_V6_DIAGPORT_CONF1_ADDR      0x00C10010
#define RF_v6_PHYDIAG_CONF1_ADDR       0x00C10018

#define RF_V7_DIAGPORT_CONF1_ADDR      0x00F10010
#define RF_v7_PHYDIAG_CONF1_ADDR       0x00F10018

#define REG_MIB_BASE(band)                (REG_MAC_CORE_BASE_ADDR(band) + 0x800)
/*****************************************************************************
 * Macros for generated register files
 *****************************************************************************/
/* Macros for IPC registers access (used in reg_ipc_app.h) */
#define REG_IPC_APP_RD(env, INDEX)                                      \
    (*(volatile u32*)((u8*)env + IPC_REG_BASE_ADDR + 4*(INDEX)))

#define REG_IPC_APP_WR(env, INDEX, value)                               \
    (*(volatile u32*)((u8*)env + IPC_REG_BASE_ADDR + 4*(INDEX)) = value)

/* Macro used in reg_mac_core.h */
/// Macro to read a platform register
#define REG_PL_RD(addr)     (*(volatile uint32_t *)(addr))

/// Macro to write a platform register
#define REG_PL_WR(addr, value)       (*(volatile uint32_t *)(addr)) = (value)

/// Macro to read a 16 bits width platform register, bb access rf
#define REG_PL_RD16(addr)              (*(volatile uint16_t *)(addr))

/// Macro to write a 16 bits width platform register, bb access rf
#define REG_PL_WR16(addr, value)       (*(volatile uint16_t *)(addr)) = (value)

/// Macro to read a 8 bits width platform register, access system manager
#define REG_PL_RD8(addr)              (*(volatile uint8_t *)(addr))

/// Macro to write a 8 bits width platform register, bb access rf
#define REG_PL_WR8(addr, value)       (*(volatile uint8_t *)(addr)) = (value)

#endif /* REG_ACCESS_H_ */
