/*
 * Synopsys DesignWare Multimedia Card Interface driver
 *  (Based on NXP driver for lpc 31xx)
 *
 * Copyright (C) 2009 NXP Semiconductors
 * Copyright (C) 2009, 2010 Imagination Technologies Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#ifndef MMC_H
#define MMC_H
#include <sf_mmap.h>

#define SDIO_BASE_ADDR              (SIFLOWER_EMMC_BASE)

#define DW_MMC_240A			        0x240a

#define SDIO0_CTRL			        (0x000 + SDIO_BASE_ADDR)
#define SDIO0_PWREN			        (0x004 + SDIO_BASE_ADDR)
#define SDIO0_CLKDIV		        (0x008 + SDIO_BASE_ADDR)
#define SDIO0_CLKSRC		        (0x00c + SDIO_BASE_ADDR)
#define SDIO0_CLKENA		        (0x010 + SDIO_BASE_ADDR)
#define SDIO0_TMOUT			        (0x014 + SDIO_BASE_ADDR)
#define SDIO0_CTYPE			        (0x018 + SDIO_BASE_ADDR)
#define SDIO0_BLKSIZ		        (0x01c + SDIO_BASE_ADDR)
#define SDIO0_BYTCNT		        (0x020 + SDIO_BASE_ADDR)
#define SDIO0_INTMASK		        (0x024 + SDIO_BASE_ADDR)
#define SDIO0_CMDARG		        (0x028 + SDIO_BASE_ADDR)
#define SDIO0_CMD                   (0x02c + SDIO_BASE_ADDR)
#define SDIO0_RESP0			        (0x030 + SDIO_BASE_ADDR)
#define SDIO0_RESP1			        (0x034 + SDIO_BASE_ADDR)
#define SDIO0_RESP2			        (0x038 + SDIO_BASE_ADDR)
#define SDIO0_RESP3			        (0x03c + SDIO_BASE_ADDR)
#define SDIO0_MINTSTS		        (0x040 + SDIO_BASE_ADDR)
#define SDIO0_RINTSTS		        (0x044 + SDIO_BASE_ADDR)
#define SDIO0_STATUS		        (0x048 + SDIO_BASE_ADDR)
#define SDIO0_FIFOTH		        (0x04c + SDIO_BASE_ADDR)
#define SDIO0_CDETECT		        (0x050 + SDIO_BASE_ADDR)
#define SDIO0_WRTPRT		        (0x054 + SDIO_BASE_ADDR)
#define SDIO0_DLC                   (0x058 + SDIO_BASE_ADDR)
#define SDIO0_TCBCNT		        (0x05c + SDIO_BASE_ADDR)
#define SDIO0_TBBCNT		        (0x060 + SDIO_BASE_ADDR)
#define SDIO0_DEBNCE		        (0x064 + SDIO_BASE_ADDR)
#define SDIO0_USRID			        (0x068 + SDIO_BASE_ADDR)
#define SDIO0_VERID			        (0x06c + SDIO_BASE_ADDR)
#define SDIO0_HCON			        (0x070 + SDIO_BASE_ADDR)
#define SDIO0_UHS_REG		        (0x074 + SDIO_BASE_ADDR)
#define SDIO0_RES_n			        (0x078 + SDIO_BASE_ADDR)
#define SDIO0_BMOD			        (0x080 + SDIO_BASE_ADDR)
#define SDIO0_PLDMND		        (0x084 + SDIO_BASE_ADDR)
#define SDIO0_DBADDR		        (0x088 + SDIO_BASE_ADDR)
#define SDIO0_IDSTS			        (0x08c + SDIO_BASE_ADDR)
#define SDIO0_IDINTEN		        (0x090 + SDIO_BASE_ADDR)
#define SDIO0_DSCADDR		        (0x094 + SDIO_BASE_ADDR)
#define SDIO0_BUFADDR		        (0x098 + SDIO_BASE_ADDR)
#define SDIO0_BEP                   (0x100 + SDIO_BASE_ADDR)
#define SDIO0_DATA			        (0x200 + SDIO_BASE_ADDR)


/*CMD index defined */

#define MMC_CMD_GO_IDLE_STATE           0
#define MMC_CMD_SEND_OP_COND            1
#define MMC_CMD_ALL_SEND_CID            2
#define MMC_CMD_SET_RELATIVE_ADDR       3
#define MMC_CMD_SET_DSR                 4
#define MMC_CMD_SWITCH                  6
#define MMC_CMD_SELECT_CARD             7
#define MMC_CMD_SEND_EXT_CSD            8
#define MMC_CMD_SEND_CSD                9
#define MMC_CMD_SEND_CID                10
#define MMC_CMD_STOP_TRANSMISSION       12
#define MMC_CMD_SEND_STATUS             13
#define MMC_CMD_SET_BLOCKLEN            16
#define MMC_CMD_READ_SINGLE_BLOCK       17
#define MMC_CMD_READ_MULTIPLE_BLOCK     18
#define MMC_CMD_SET_BLKCNT              23
#define MMC_CMD_WRITE_SINGLE_BLOCK      24
#define MMC_CMD_WRITE_MULTIPLE_BLOCK    25
#define MMC_CMD_ERASE_START             35
#define MMC_CMD_ERASE_END               36
#define MMC_CMD_ERASE_BEGIN             38
#define MMC_CMD_APP_CMD                 55
#define MMC_CMD_SPI_READ_OCR            58
#define MMC_CMD_SPI_CRC_ON_OFF          59


#define SD_CMD_GO_IDLE_STATE            0
#define SD_CMD_ALL_SEND_CID             2
#define SD_CMD_SEND_RELATIVE_ADDR       3
#define SD_CMD_SET_DSR                  4
#define SD_CMD_SWITCH_FUNC              6
#define SD_CMD_SELECT_CARD              7
#define SD_CMD_SEND_IF_COND             8
#define SD_CMD_SEND_CSD                 9
#define SD_CMD_SEND_CID                 10
#define SD_CMD_STOP_TRANSMISSION        12
#define SD_CMD_SEND_STATUS              13
#define SD_CMD_SET_BLOCKLEN             16
#define SD_CMD_READ_SINGLE_BLOCK        17
#define SD_CMD_READ_MULTIPLE_BLOCK      18
#define SD_CMD_WRITE_SINGLE_BLOCK       24
#define SD_CMD_WRITE_MULTIPLE_BLOCK     25
#define SD_CMD_PROGRAM_CSD              27
#define SD_CMD_SET_WRITE_PROT           28
#define SD_CMD_CLE_WRITE_PROT           29
#define SD_CMD_SEND_WRITE_PROT          30
#define SD_CMD_ERASE_START              32
#define SD_CMD_ERASE_END                33
#define SD_CMD_ERASE_BEGIN              38
#define SD_CMD_APP_CMD                  55

#define SD_CMD_APP_SET_BUS_WIDTH        6
#define SD_CMD_APP_SEND_OP_COND         41
#define SD_CMD_APP_SEND_SCR             51
#define SD_CMD_APP_SD_CARD              13


/*
 * Data offset is difference according to Version
 * Lower than 2.40a : data register offest is 0x100
 */

/* shift bit field */
#define _SBF(f, v)		((v) << (f))

/* Control register defines */
#define SDIO0_CTRL_USE_IDMAC		(1 << 25)
#define SDIO0_CTRL_CEATA_INT_EN		(1 << 11)
#define SDIO0_CTRL_SEND_AS_CCSD		(1 << 10)
#define SDIO0_CTRL_SEND_CCSD		(1 << 9)
#define SDIO0_CTRL_ABRT_READ_DATA	(1 << 8)
#define SDIO0_CTRL_SEND_IRQ_RESP	(1 << 7)
#define SDIO0_CTRL_READ_WAIT		(1 << 6)
#define SDIO0_CTRL_DMA_ENABLE		(1 << 5)
#define SDIO0_CTRL_INT_ENABLE		(1 << 4)
#define SDIO0_CTRL_DMA_RESET		(1 << 2)
#define SDIO0_CTRL_FIFO_RESET		(1 << 1)
#define SDIO0_CTRL_RESET	    	(1 << 0)
/* Clock Enable register defines */
#define SDIO0_CLKEN_LOW_PWR	    	(1 << 16)
#define SDIO0_CLKEN_ENABLE   		(1 << 0)
/* time-out register defines */
#define SDIO0_TMOUT_DATA(n)	    	_SBF(8, (n))
#define SDIO0_TMOUT_DATA_MSK		0xFFFFFF00
#define SDIO0_TMOUT_RESP(n)	    	((n) & 0xFF)
#define SDIO0_TMOUT_RESP_MSK		0xFF
/* card-type register defines */
#define SDIO0_CTYPE_8BIT		(1 << 16)
#define SDIO0_CTYPE_4BIT		(1 << 0)
#define SDIO0_CTYPE_1BIT		0
/* Interrupt status & mask register defines */
#define SDIO0_INT_SDIO(n)		(1 << (16 + (n)))
#define SDIO0_INT_EBE			(1 << 15)
#define SDIO0_INT_ACD			(1 << 14)
#define SDIO0_INT_SBE			(1 << 13)
#define SDIO0_INT_HLE			(1 << 12)
#define SDIO0_INT_FRUN			(1 << 11)
#define SDIO0_INT_HTO			(1 << 10)
#define SDIO0_INT_DTO			(1 << 9)
#define SDIO0_INT_RTO			(1 << 8)
#define SDIO0_INT_DCRC			(1 << 7)
#define SDIO0_INT_RCRC			(1 << 6)
#define SDIO0_INT_RXDR			(1 << 5)
#define SDIO0_INT_TXDR			(1 << 4)
#define SDIO0_INT_DATA_OVER		(1 << 3)
#define SDIO0_INT_CMD_DONE		(1 << 2)
#define SDIO0_INT_RESP_ERR		(1 << 1)
#define SDIO0_INT_CD			(1 << 0)
#define SDIO0_INT_ERROR			0xbfc2
/* Command register defines */
#define SDIO0_CMD_START			(1 << 31)
#define SDIO0_CMD_USEHOLD       (1 << 29)
#define SDIO0_CMD_BOOTMODE      (1 << 27)
#define SDIO0_CMD_CCS_EXP		(1 << 23)
#define SDIO0_CMD_CEATA_RD		(1 << 22)
#define SDIO0_CMD_UPD_CLK		(1 << 21)
#define SDIO0_CMD_INIT			(1 << 15)
#define SDIO0_CMD_STOP			(1 << 14)
#define SDIO0_CMD_PRV_DAT_WAIT		(1 << 13)
#define SDIO0_CMD_SEND_STOP		(1 << 12)
#define SDIO0_CMD_STRM_MODE		(1 << 11)
#define SDIO0_CMD_DAT_WR		(1 << 10)
#define SDIO0_CMD_DAT_EXP		(1 << 9)
#define SDIO0_CMD_RESP_CRC		(1 << 8)
#define SDIO0_CMD_RESP_LONG		(1 << 7)
#define SDIO0_CMD_RESP_EXPT		(1 << 6)
#define SDIO0_CMD_INDX(n)		((n) & 0x1F)
/* Status register defines */
#define SDIO0_GET_FCNT(x)		(((x)>>17) & 0x1FFF)
#define SDIO0_DATA_BUZY         (1 << 9)
/* Internal DMAC interrupt defines */
#define SDIO0_IDMAC_INT_AI		(1 << 9)
#define SDIO0_IDMAC_INT_NI		(1 << 8)
#define SDIO0_IDMAC_INT_CES		(1 << 5)
#define SDIO0_IDMAC_INT_DU		(1 << 4)
#define SDIO0_IDMAC_INT_FBE		(1 << 2)
#define SDIO0_IDMAC_INT_RI		(1 << 1)
#define SDIO0_IDMAC_INT_TI		(1 << 0)
/* Internal DMAC bus mode bits */
#define SDIO0_IDMAC_ENABLE		(1 << 7)
#define SDIO0_IDMAC_FB			(1 << 1)
#define SDIO0_IDMAC_SWRESET		(1 << 0)
/* Version ID register define */
#define SDIO0_GET_VERID(x)		((x) & 0xFFFF)
#define OCR_BUSY		0x80000000
#define OCR_HCS			0x40000000

#define MMC_INIT_RATE	400000
#define MMC_WR_RATE	5000000

extern int mmc_initialization(u32 dev_choose);
extern int mmc_data_read(u32 addr, u8 * buf, u32 blk_size , u32 byte_count);

#endif          /*SDIO_H*/
