#include <common.h>
#include <mach/pcba_test.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <mach/sfax8.h>

#ifdef CONFIG_SFAX8_PMU_TEST
extern int get_pmu_rst_status(u8 *val);
#endif

struct button_test_param {
	int button_type;
	int gpio_num;
	int value;
	int test_time_ms;
};

enum button_type {
	PMU_BUTTON = 0,
	GPIO_BUTTON,
};

int button_test(void *d)
{
	struct button_test_param *button_test_param = (struct button_test_param *)d;
	int timeout = button_test_param->test_time_ms / 50;
	int test_ret = CMD_SUCCESS;

	if (button_test_param->button_type == PMU_BUTTON) {
#ifdef CONFIG_SFAX8_PMU_TEST
		u8 pend;
		int ret;
		while (timeout--) {
			ret = get_pmu_rst_status(&pend);
			if (!ret && pend) {
				printf("get pmu irq!\n");
				break;
			}
			mdelay(50);
		}
		if (timeout < 0)
			test_ret = CMD_TIMEOUT;
#else
		test_ret = CMD_FAIL;
#endif
	} else if (button_test_param->button_type == GPIO_BUTTON) {
		if (gpio_request(button_test_param->gpio_num, "sf_button")) {
			printf("Failed to request gpio %d\n",
					button_test_param->gpio_num);
			test_ret = CMD_FAIL;
			goto END;
		}
		if (gpio_direction_input(button_test_param->gpio_num)) {
			printf("Failed to set gpio num %d\n",
					button_test_param->gpio_num);
			test_ret = CMD_FAIL;
			gpio_free(button_test_param->gpio_num);
			goto END;
		}
		// in p10h, the first value of gpio 60 is 0, then it will change to 1. So we delay some time here.
		mdelay(10);
		while (timeout--) {
			if (gpio_get_value(button_test_param->gpio_num) ==
					button_test_param->value)
				break;
			mdelay(50);
		}
		if (timeout < 0) {
			printf("Failed to set gpio value %d\n",
					button_test_param->gpio_num);
			test_ret = CMD_FAIL;
		}

		gpio_free(button_test_param->gpio_num);
	} else
		test_ret = CMD_FAIL;
END:
	return test_ret;
}
