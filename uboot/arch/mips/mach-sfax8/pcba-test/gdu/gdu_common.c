/*
 * =====================================================================================
 *
 *       Filename:  gdu_common.c
 *
 *    Description:  This file includes basic functions of gdu module.
 *
 *        Version:  1.0
 *        Created:  10/27/2016 01:58:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qi , qi.zhang@siflower.com.cn
 *        Company:  Siflower Communication Tenology Co.,Ltd
 *
 * =====================================================================================
 */
#include <common.h>
#include <asm/io.h>
#include "gdu.h"

static u8 displaylogo = 0;

static __maybe_unused struct pwm_config_t pbus_198M_to_120Hz = {
		.prescaler = 165,
		.divider = pwm_div_16,
		.count = 625,
};

static __maybe_unused struct pwm_config_t pbus_198M_to_240Hz = {
		.prescaler = 165,
		.divider = pwm_div_8,
		.count = 625,
};

#ifdef CONFIG_DM_PWM
/*
 * GDU needs pwm0's timer2 and timer3 to generate window interrupt, we
 * set timers here directly.
 */
static void gdu_setup_pwm0(struct pwm_config_t *cfg)
{
	/* set prescaler */
	writel((cfg->prescaler - 1) << 8, SF_PWM_TCFG0);
	/* set divider */
	writel((cfg->divider << 8) | (cfg->divider << 12), SF_PWM_TCFG1);
	/* set count buffer */
	writel(cfg->count, SF_PWM_TCNTB(2));
	writel(cfg->count, SF_PWM_TCNTB(3));
	/* set auto reload, output inverter and manual update */
	writel((0x7 << 13) | (0x7 << 17), SF_PWM_TCON);
	/* clear manual update, start timer */
	writel((0xd << 12) | (0xd << 16), SF_PWM_TCON);
}
#else
static void gdu_setup_pwm0(struct pwm_config_t *cfg) { return; }
#endif

static void sfax8_gdu_load_enable(void)
{
	// default, flush regs directly.
	writel(OVCDCR_LOAD_PARA_EN, OVCDCR);

	return;
}

static __maybe_unused void set_background_color(u32 color)
{
	writel(color, OVCBKCOLOR);
}

static void ocw1_extra_config(struct ocw1_config_t *ocw1_cfg)
{
	struct ocw1_controller *ocw1 = ((struct ocw_config_t *)ocw1_cfg)->ocw_reg;
	u32 ctl = readl(&ocw1->cr);
	u32 ckcr = 0;

	if(ocw1_cfg->blend_category)
		ctl |= BLD_PIX;
	else
		ctl &= ~BLD_PIX;

	if (ocw1_cfg->alpha_sel)
		ctl |= ALPHA_SEL;
	else
		ctl &= ~ALPHA_SEL;
	writel(ctl, &ocw1->cr);

	writel(((ocw1_cfg->alpha0) << ALPHA0_SHIFT) | ((ocw1_cfg->alpha1) << ALPHA1_SHIFT),
			&ocw1->pccr);

	if (ocw1_cfg->color_key_val != -1) {
		ckcr |= KEYEN;
		writel(ocw1_cfg->color_key_val, &ocw1->ckr);
		if (ocw1_cfg->color_key_blend)
			ckcr |= KEYBLEN;
		if (ocw1_cfg->color_key_direction)
			ckcr |= DIRCON;
		ckcr |= ocw1_cfg->comp_key;
		writel(ckcr, &ocw1->ckcr);
	}
}

static void ocw_config(void *cfg, enum ocw_type ocw)
{
	struct ocw_config_t *ocw_cfg = cfg;
	u32 ctl = 0;
	u8 buffer_num = 0;
	struct position *p = &ocw_cfg->pos;
	u32 i = 0;
	void *reg = &ocw_ctrl;

	*(u32 *)reg = (u32)ocw_cfg->ocw_reg;

	if (p) {
		WRITEL((p->left_top_y << LEFT_TOP_Y_SHIFT) | (p->left_top_x << LEFT_TOP_X_SHIFT),
				ocw, pcar);
		WRITEL((p->right_bot_y << RIGHT_BOT_Y_SHIFT) | (p->right_bot_x << RIGHT_BOT_X_SHIFT),
				ocw, pcbr);
	}

	if(ocw_cfg->auto_ping_pang)
		ctl |= BUFAUTOEN;

	for (i = 0, buffer_num = 0; (i < 4) && (ocw_cfg->buffer_addr[i] != 0);
			i++, buffer_num++)
		WRITEL(ocw_cfg->buffer_addr[i], ocw, bxsar[i]);

	ctl |= (buffer_num - 1) << BUF_NUM_SHIFT;
	ctl |= ocw_cfg->bpp_mode << BPP_MODE_SHIFT;

	if (ocw_cfg->rb_exchange)
		ctl |= RBEXG;

	WRITEL(ctl, ocw, cr);

	if(ocw_cfg->map_color)
		WRITEL(MAPCOLEN | ocw_cfg->map_color, ocw, cmr);

	if (ocw == win1)
		ocw1_extra_config(cfg);

	WRITEL(READL(ocw, cr) | OVCWxCR_ENWIN, ocw, cr);
}

static int sfax8_lcd_configure(struct lcdc_config_t *cfg)
{
	int div2, val, rgb_order;
	unsigned int pixTotal, clock;

	debug("================= func %s start\n", __func__);
	debug("width =%d - \n", cfg->width);
	debug("height=%d - \n", cfg->height);
	debug("vspw  =%d - \n", cfg->vspw);
	debug("vbpd  =%d - \n", cfg->vbpd);
	debug("vfpd  =%d - \n", cfg->vfpd);
	debug("hspw  =%d - \n", cfg->hspw);
	debug("hbpd  =%d - \n", cfg->hbpd);
	debug("hfpd  =%d - \n", cfg->hfpd);
	debug("fps=%d - \n", cfg->fps);

	rgb_order = cfg->SignalDataMapping;

	// calc the clock needed
	pixTotal = (cfg->width + cfg->hspw + cfg->hbpd + cfg->hfpd) * (cfg->height + cfg->vspw + cfg->vfpd + cfg->vbpd);
	debug(" pixTotal = %d - \n", pixTotal);
	clock = pixTotal * cfg->fps;
	debug(" clock = %d - \n", clock);
	// div = EPLL_CLK / clock;
	div2 = cfg->source_clock / clock;
	debug(" div2 = %d - \n", div2);

	// to configure the ids set and show the logo
	// lcd configure
	val = ((cfg->vbpd - 1) << LCDCON2_VBPD) | ((cfg->vfpd - 1) << LCDCON2_VFPD);
	writel(val, LCDCON2);
	val = ((cfg->vspw - 1) << LCDCON3_VSPW) | ((cfg->hspw - 1) << LCDCON3_HSPW);
	writel(val, LCDCON3);
	val = ((cfg->hbpd - 1) << LCDCON4_HBPD) | ((cfg->hfpd - 1) << LCDCON4_HFPD);
	writel(val, LCDCON4);
	val = (cfg->rgb_565 << LCDCON5_RGB565IF) | (rgb_order << LCDCON5_RGBORDER) | (cfg->dsp_type << LCDCON5_DSPTYPE) |
	      (cfg->signalVclkInverse << LCDCON5_INVVCLK) | (cfg->signalHsyncInverse << LCDCON5_INVVLINE) |
	      (cfg->signalVsyncInverse << LCDCON5_INVVFRAME) | (cfg->signalDataInverse << LCDCON5_INVVD) |
	      (cfg->signalVdenInverse << LCDCON5_INVVDEN) | (cfg->dsp_order << LCDCON5_CONFIGORDER);
	writel(val, LCDCON5);
	val = ((cfg->height - 1) << LCDCON6_LINEVAL) | ((cfg->width - 1) << LCDCON6_HOZVAL);
	writel(val, LCDCON6);
	val = ((div2 - 1) << LCDCON1_CLKVAL) | (0 << LCDCON1_VMMODE) | (0 << LCDCON1_ENVID);
	if (displaylogo != 1) {
		writel(val, LCDCON1);
	}
	writel(readl(LCDVCLKFSR) | 0xF000000, LCDVCLKFSR);

	debug("================= func %s end\n", __func__);
	return 0;
}

void sfax8_gdu_setup(struct lcdc_config_t *lcd, struct ocw_config_t *ocw0_cfg,
		struct ocw1_config_t *ocw1_cfg)
{
	u32 imgWidth;

	imgWidth = lcd->width & (~0x07);

	// osd
	sfax8_gdu_load_enable();

	if (ocw0_cfg)
		writel(imgWidth, &((struct ocw0_controller *)(ocw0_cfg->ocw_reg))->vssr);
	if (ocw1_cfg)
		writel(imgWidth, &((struct ocw1_controller *)(ocw1_cfg->cfg.ocw_reg))->vssr);

	if (ocw0_cfg)
		ocw_config(ocw0_cfg, win0);
	if (ocw1_cfg)
		ocw_config(ocw1_cfg, win1);

	sfax8_lcd_configure(lcd);

	gdu_setup_pwm0(&pbus_198M_to_240Hz);
}
