/*
 * =====================================================================================
 *
 *       Filename:  gdu.h
 *
 *    Description:  This file contains gdu registers' addresses and structs
 *
 *        Version:  1.0
 *        Created:  10/21/2016 11:46:37 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qi (), qi.zhang@siflower.com.cn
 *        Company:  Siflower Communication Tenology Co.,Ltd
 *
 * =====================================================================================
 */
#ifndef _GDU_REG_H_
#define _GDU_REG_H_

#include <common.h>
#include <asm/gpio.h>
#include <fdtdec.h>

#define ARRAY_NUM(A)	(sizeof(A) / sizeof(A[0]))

#define SIFLOWER_GDU_BASE	((void *)0xB0800000)

#define GDU0_REG(x)		(SIFLOWER_GDU_BASE + (x))
#define OVCDCR			GDU0_REG(0x1000)
#define OVCPCR			GDU0_REG(0x1004)
#define OVCBKCOLOR		GDU0_REG(0x1008)
#define OVCWPR			GDU0_REG(0x100c)
#define OVCW0CR			GDU0_REG(0x1080)
#define OVCW0PCAR		GDU0_REG(0x1084)
#define OVCW0PCBR		GDU0_REG(0x1088)
#define OVCW0VSSR		GDU0_REG(0x108c)
#define OVCW0CMR		GDU0_REG(0x1090)
#define OVCW0B0SAR		GDU0_REG(0x1094)
#define OVCW0B1SAR		GDU0_REG(0x1098)
#define OVCW0B2SAR		GDU0_REG(0x109c)
#define OVCW0B3SAR		GDU0_REG(0x10a0)
#define OVCW1CR			GDU0_REG(0x1100)
#define OVCW1PCAR		GDU0_REG(0x1104)
#define OVCW1PCBR		GDU0_REG(0x1108)
#define OVCW1PCCR		GDU0_REG(0x110c)
#define OVCW1VSSR		GDU0_REG(0x1110)
#define OVCW1CKCR		GDU0_REG(0x1114)
#define OVCW1CKR		GDU0_REG(0x1118)
#define OVCW1CMR		GDU0_REG(0x111c)
#define OVCW1B0SAR		GDU0_REG(0x1120)
#define OVCW1B1SAR		GDU0_REG(0x1124)
#define OVCW1B2SAR		GDU0_REG(0x1128)
#define OVCW1B3SAR		GDU0_REG(0x112c)

// osd offset
#define OVCDCR_LOAD_PARA_EN BIT(11)
#define OVCDCR_IFTYPE		(1)
#define OVCWxCR_ENWIN		BIT(0)

#define OVCOMC_ToRGB		(31)
#define OVCOMC_oft_b		(8)
#define OVCOMC_oft_a		(0)
#define BUFAUTOEN			BIT(16)
#define BUFSEL_SHIFT		17
#define BUF_NUM_SHIFT		14
#define RBEXG				BIT(6)
#define BPP_MODE_SHIFT		1
#define LEFT_TOP_Y_SHIFT	16
#define LEFT_TOP_X_SHIFT	0
#define RIGHT_BOT_Y_SHIFT	16
#define RIGHT_BOT_X_SHIFT	0
#define MAPCOLEN			BIT(24)

#define ALPHA_SEL			BIT(8)
#define BLD_PIX				BIT(7)
#define ALPHA0_SHIFT		12
#define ALPHA1_SHIFT		0
#define KEYBLEN				BIT(26)
#define KEYEN				BIT(25)
#define DIRCON				BIT(24)
#define COMPKEY_SHIFT		0

// lcd reg & offset
#define LCDCON1			GDU0_REG(0x000) //LCD control 1
#define LCDCON2			GDU0_REG(0x004) //LCD control 2
#define LCDCON3			GDU0_REG(0x008) //LCD control 3
#define LCDCON4			GDU0_REG(0x00c) //LCD control 4
#define LCDCON5			GDU0_REG(0x010) //LCD control 5
#define LCDCON6			GDU0_REG(0x018)
#define LCDVCLKFSR		GDU0_REG(0x030)
#define GDUINTPND		GDU0_REG(0x054) //LCD Interrupt pending
#define GDUSRCPND		GDU0_REG(0x058) //LCD Interrupt source
#define GDUINTMSK		GDU0_REG(0x05c) //LCD Interrupt mask

/* interrupt bits */
#define OSDERR			BIT(4)
#define OSDW1INT		BIT(3)
#define OSDW0INT		BIT(2)
#define VCLKINT			BIT(1)
#define LCDINT			BIT(0)

/* LCD control register 1 */
#define LCDCON1_LINECNT 18 // [29:18]
#define LCDCON1_CLKVAL 8   // [17:8]
#define LCDCON1_VMMODE 7	 // [7:7]
#define LCDCON1_PNRMODE 5
#define LCDCON1_STNBPP 1
#define LCDCON1_ENVID 0 // [0:0]

#define LCDCON2_VBPD 16 // [26:16]
#define LCDCON2_VFPD 0  // [10:0]

#define LCDCON3_VSPW 16 // [26:16]
#define LCDCON3_HSPW 0  // [10:0]

#define LCDCON4_HBPD 16 // [26:16]
#define LCDCON4_HFPD 0  // [10:0]

#define LCDCON5_RGB565IF 31
#define LCDCON5_RGBORDER 24    // [29:24]
#define LCDCON5_CONFIGORDER 20 // [22:20] 0->dsi24bpp, 1->dsi16bpp1, 2->dsi16bpp2,3->dsi16bpp3,4->dsi18bpp1,5->dsi18bpp2
#define LCDCON5_VSTATUS 15     // [16:15]
#define LCDCON5_HSTATUS 13     // [14:13]
#define LCDCON5_DSPTYPE 11     // [12:11]
#define LCDCON5_INVVCLK 10     // [10:10]
#define LCDCON5_INVVLINE 9     // [9:9]
#define LCDCON5_INVVFRAME 8    // [8:8]
#define LCDCON5_INVVD 7	// [7:7]
#define LCDCON5_INVVDEN 6      // [6:6]
#define LCDCON5_INVPWREN 5     // [5:5]
#define LCDCON5_PWREN 3	// [3:3]

#define LCDCON6_LINEVAL 16 // [26:16]
#define LCDCON6_HOZVAL 0   // [10:0]

#define DATASIGMAPPING_RGB 0x6  // [5:4]2b'00, [3:2]2b'01, [1:0]2b'10. RGB
#define DATASIGMAPPING_RBG 0x9  // [5:4]2b'00, [3:2]2b'10, [1:0]2b'01. RBG
#define DATASIGMAPPING_GRB 0x12 // [5:4]2b'01, [3:2]2b'00, [1:0]2b'10. GRB
#define DATASIGMAPPING_GBR 0x18 // [5:4]2b'01, [3:2]2b'10, [1:0]2b'00. GBR
#define DATASIGMAPPING_BRG 0x21 // [5:4]2b'10, [3:2]2b'00, [1:0]2b'01. BRG
#define DATASIGMAPPING_BGR 0x24 // [5:4]2b'10, [3:2]2b'01, [1:0]2b'00. BGR

/* Color value in RGB_888 mode */
#define RED	0xff0000
#define GREEN	0x00ff00
#define BLUE	0x0000ff

struct lcdc_config_t {
	int width;	//h
	int height;	//v

	int vspw; //mVSyncPulseWidth
	int vbpd; //mVBlanking - mVSyncOffset -mVSyncPulseWidth
	int vfpd; //mVSyncOffset
	int hspw;
	int hbpd;
	int hfpd;

	int signalPwrenEnable;  //1
	int signalPwrenInverse; //0
	int signalVclkInverse;  //plority
	int signalHsyncInverse;
	int signalVsyncInverse;
	int signalDataInverse; //0
	int signalVdenInverse; //0

	int rgb_565;
	int SignalDataMapping; //DATASIGMAPPING_RGB???
	int fps;	  //60
	int dsp_type;	  //00 : TFT LCD display, 03: RGB_565, 16 bit mode
	int dsp_order;	  //0->dsi24bpp, 1->dsi16bpp1, 2->dsi16bpp2,3->dsi16bpp3,4->dsi18bpp1,5->dsi18bpp2

	int phyWidth;  //0
	int phyHeight; //0

	int source_clock;
};

struct ocw0_controller {
	u32 cr;
	u32 pcar;
	u32 pcbr;
	u32 vssr;
	u32 cmr;
	u32 bxsar[4];
};

struct ocw1_controller {
	u32 cr;
	u32 pcar;
	u32 pcbr;
	u32 pccr;
	u32 vssr;
	u32 ckcr;
	u32 ckr;
	u32 cmr;
	u32 bxsar[4];
};


struct position{
	u32 left_top_y;
	u32 left_top_x;
	u32 right_bot_y;
	u32 right_bot_x;
};

struct ocw_config_t {
	void *const ocw_reg;
	struct position pos;
	/* Display this color instead of origin image */
	u32 map_color;
	u32 buffer_addr[4];
	/* RGB_888:0xb, RGB_565:0x5 */
	u8 bpp_mode;
	u8 auto_ping_pang;
	u8 buffer_num;
	u8 rb_exchange;
};

struct ocw1_config_t {
	struct ocw_config_t cfg;
	u8 blend_category;
	u8 alpha_sel;
	u16 alpha0;
	u16 alpha1;
	u8 color_key_direction;
	u8 color_key_blend;
	int color_key_val;
	u32 comp_key;
};

enum pwm_div_t
{
    pwm_div_off =0x0,
    pwm_div_2 =0x8,
    pwm_div_4 =0x9,
    pwm_div_8 =0xa,
    pwm_div_16 =0xb,
};

struct pwm_config_t{
	u16 prescaler;
	enum pwm_div_t divider;
	u16 count;
};

union {
	struct ocw0_controller *ocw0;
	struct ocw1_controller *ocw1;
} ocw_ctrl;

enum ocw_type {
	win0,
	win1,
};

#define WRITEL(data, type, reg)                            \
	do {                                               \
		if (type == win0)                          \
			writel(data, &ocw_ctrl.ocw0->reg); \
		else if (type == win1)                     \
			writel(data, &ocw_ctrl.ocw1->reg); \
	} while (0)

#define READL(type, reg) ({                          \
	u32 __data;                                  \
	if (type == win0)                            \
		__data = readl(&ocw_ctrl.ocw0->reg); \
	else if (type == win1)                       \
		__data = readl(&ocw_ctrl.ocw1->reg); \
	__data;                                      \
})

void sfax8_gdu_setup(struct lcdc_config_t *, struct ocw_config_t *,
	       struct ocw1_config_t *);

#endif
