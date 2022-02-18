#include "gdu.h"

struct lcdc_config_t lcd_cfg = {
	.width = 800,
	.height = 480,
	.hspw = 20,
	.hbpd = 46,
	.hfpd = 210,
	.vspw = 10,
	.vfpd = 22,
	.vbpd = 23,
	.SignalDataMapping = 0x6,
	.signalVclkInverse = 0,
	.signalHsyncInverse = 1,
	.signalVsyncInverse = 1,
	.signalDataInverse = 0,
	.signalVdenInverse = 0,
	.signalPwrenInverse = 0,
	.signalPwrenEnable = 0,
	.fps = 60,
	.source_clock = 198000000,
};

struct ocw_config_t ocw0 = {
	.ocw_reg = (void *)OVCW0CR,
	.map_color = 0,
	.bpp_mode = 0xb,
	.auto_ping_pang = 0,
	.buffer_num = 2,
	.rb_exchange = 0,
};

struct ocw1_config_t ocw1 = {
	.cfg = {
		.ocw_reg = (void *)OVCW1CR,
		.map_color = 0,
		.bpp_mode = 0xb,
		.auto_ping_pang = 0,
		.buffer_num = 2,
		.rb_exchange = 0,
	},
	.blend_category = 0,
	.alpha_sel = 0,
	.alpha0 = 255,
	.color_key_val = -1,
};

int power_gpios[2] = {26, 37};