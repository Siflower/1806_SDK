/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

//#define DEBUG
#include <common.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <fdtdec.h>
#include <dm.h>
#include <dm/pinctrl.h>
#include <mach/pad.h>
#include <mach/pcba_test.h>
#include "gdu.h"

#define GPIO_LB_EN			0
#define GPIO_LCD_EN			1

extern void sys_gdu_reset(void);

extern struct lcdc_config_t lcd_cfg;
extern struct ocw_config_t ocw0;
extern struct ocw1_config_t ocw1;
extern int power_gpios[2];

static void set_window_position(struct position *pos)
{
	int imgWidth, imgHeight;

	imgWidth = lcd_cfg.width & (~0x7);
	imgHeight = lcd_cfg.height & (~0x03);

	pos->left_top_y = (lcd_cfg.height - imgHeight) >> 1;
	pos->left_top_x = (lcd_cfg.width - imgWidth) >> 1;
	pos->right_bot_x = ((lcd_cfg.width - imgWidth) >> 1) + imgWidth - 1;
	pos->right_bot_y = ((lcd_cfg.height - imgHeight) >> 1) + imgHeight - 1;
}

static int sfax8_gdu_set_pad(void)
{
	struct udevice *pinctrl;
	int ret;

	ret = uclass_get_device(UCLASS_PINCTRL, 0, &pinctrl);
	if (ret) {
	    debug("%s: Cannot find pinctrl device\n", __func__);
	    goto err;
	}

	ret = pinctrl_request_noflags(pinctrl, PERIPH_ID_GDU);
	if (ret) {
	    debug("%s: Failed to set up GDU pinctrl!\n", __func__);
	    goto err;
	}
	return 0;

err:
	return -ENODEV;
}

static int sfax8_gdu_set_power(int gpio, const char *label)
{
	if (gpio_request(gpio, label)) {
		printf("gpio %d request fail\n", gpio);
		return -ENODEV;
	}

	if (gpio_direction_output(gpio, 1)) {
		printf("gpio %d's direction set fail", gpio);
		return -EINVAL;
	}

	return 0;
}

int sfax8_gdu_init(void)
{
	int err;

	sys_gdu_reset();
	err = sfax8_gdu_set_pad();
	if (err)
		return err;

	err = sfax8_gdu_set_power(power_gpios[0], "gdu-power0");
	if (err)
		return err;
	err = sfax8_gdu_set_power(power_gpios[1], "gdu-power1");
	if (err)
		return err;

	/* set position */
	set_window_position(&ocw0.pos);
	set_window_position(&ocw1.cfg.pos);

	return 0;
}

int lcd_test(void *data)
{
	u32 *color = data;
	int err;

	err = sfax8_gdu_init();
	if (err)
		return err;

	debug("colors: %x %x %x\n", color[0], color[1], color[2]);
	/* set buffer */
	ocw0.buffer_addr[0] = (u32)0xa0000000;
	ocw0.map_color = color[0];
	ocw0.buffer_addr[1] = 0;

	sfax8_gdu_setup(&lcd_cfg, &ocw0, 0);

	/* enable LCD */
	writel(readl(LCDCON1) | 0x1, LCDCON1);

	mdelay(1000);
	writel(MAPCOLEN | color[1], OVCW0CMR);
	mdelay(1000);
	writel(MAPCOLEN | color[2], OVCW0CMR);
	mdelay(1000);

	gpio_free(power_gpios[0]);
	gpio_free(power_gpios[1]);

	return CMD_SUCCESS;
}
