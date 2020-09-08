/*
	Siflower sfax8 recovery driver head file.
*/
#include <linux/mfd/sfax8.h>
struct sfax8_data{
	int irq;
	struct device *dev;
};

typedef enum {
	LONG_PRESS_INT,
	SHORT_PRESS_INT,
	UNKNOW_INT, //get int type failed.
}interrupt_type;

struct rb_ops{
	int (*init)(struct sfax8_data *, struct sfax8 *);
	interrupt_type (*clear_interrupt)(struct sfax8 *);
	int (*set_led_onoff)(bool);
	int (*set_interrupt_mask)(bool, struct sfax8 *);
};
/*
*	if init error, retrun negative.
*/
extern int sfax8_recovery_init(struct sfax8_data *data);
/*
*  return 0 if  irq matched the recovery condition,
*  else return negative.
*/
extern interrupt_type sfax8_recovery_clear_interrupt(void);
/*
* set led on  and off
*/
extern int sfax8_recovery_led_on(bool);
