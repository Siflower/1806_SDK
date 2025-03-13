/*
 * SiFlower sf16a18 boardinfo definition.
 */

#ifndef __PLAT_H_
#define __PLAT_H_

#define MAX_NAME_LEN	32
#define MAX_LED_SUPPORTED	24
#define MAX_BTNS	5

enum {
	BOARD_0, /* SF16A18_P10_GMAC */
	BOARD_1, /* SF16A18_86V */
	BOARD_2, /* SF16A18_AC */
	BOARD_3, /* SF16A18_REPEATER */
	BOARD_4, /* SF16A18_P20 */
	BOARD_5, /* SF16A18_P10H */
	BOARD_6, /* SF16A18_ZEGBEE_BLE */
	BOARD_7, /* SF16A18_86V_C2 */
	BOARD_8, /* SF16A18_REPEATER_NOPA */
	BOARD_9, /* SF16A18_AC20 */
	BOARD_10, /* SF16A18_EVB_v5 */
	BOARD_11, /* SF19A28_EVB */
	BOARD_12, /* SF19A28_AC28 */
	BOARD_13, /* SF19A28_REP */
	BOARD_14, /* SF19A28_AC28S */
	BOARD_15, /* SF19A28_BPI */
	BOARD_16, /* SF19A28_REP_SW */
	BOARD_17, /* SF19A28_REP_PHY */
	BOARD_18, /* SF19A28_BPI_LUCI2 */
	BOARD_19, /* SF19A28_BPI_LUCI2_NEUTRAL */
	BOARD_20, /* SF19A28_BPI_LUCI2_SIFLOWER */
	BOARD_MAX, /* not used */
};

struct board_name {
	int board;
	char name[MAX_NAME_LEN];
} __attribute__((__packed__));

struct board_info {
	int ledcnt;
	char **bd_leds;
	struct board_name bn;
} __attribute__((__packed__));

extern struct board_name *plat_get_board(void);

extern int plat_led_init(int *);
extern int plat_led_deinit(void);

/* global actions: all leds */
extern int plat_set_led_on(void);
extern int plat_set_led_timer(void);
extern int plat_set_led_off(void);

/* repeater actions */
/* wifi-status timer:100:100 */
extern int plat_set_led_rep_1(void);
/* wifi-status default-on */
extern int plat_set_led_rep_2(void);
/* repeater-signal timer:800:800 */
extern int plat_set_led_rep_3(void);
/* repeater-signal default-on */
extern int plat_set_led_rep_4(void);
/*
 * The led action when reset button is long press
 * */
extern int plat_set_led_rep_5(void);

/* repeater actions */
/* wifi-status timer:100:100 */
extern int plat_set_led_rep_nopa_1(void);
/* wifi-status default-on */
extern int plat_set_led_rep_nopa_2(void);
/* repeater-signal timer:800:800 */
extern int plat_set_led_rep_nopa_3(void);
/* repeater-signal default-on */
extern int plat_set_led_rep_nopa_4(void);
/*
 * The led action when reset button is long press
 * */
extern int plat_set_led_rep_nopa_5(void);

/* button handler */
extern int plat_btn_handler(int, int, struct board_name *, int);
/*
 * The led action when 86v finish boot or reset button is long press
 * */
extern int  plat_set_led_86v_flicker(int sec);
/*
 * The led timer action of 86v
 * */
extern int plat_set_led_86v_delay(const char *delayon, const char *delayoff);
/*
 * p10h led action
 * */
extern int plat_set_led_p10h(void);
/*
 * p10m led action
 * */
extern int plat_set_led_p10m(void);
/*
 * evb led action
 * */
extern int plat_set_led_evb(void);
/*
 * sfax8 file lock by fcntli's F_SETLKW
 * */
extern int sfax8_file_lock_set(int fd, int cmd, int type);
extern int sfax8_file_lock_check(const char *file_name);

#endif /* __PLAT_H_ */
