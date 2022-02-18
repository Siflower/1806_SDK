#include <common.h>
#include <mach/pcba_test.h>
#include <asm/gpio.h>

struct gpio_test_param {
	int gpio_num;
	int value;
};

int gpio_test(void *d)
{
	struct gpio_test_param *gpio_test_param = (struct gpio_test_param *)d;

	if (gpio_request(gpio_test_param->gpio_num, "sf_gpio")) {
		printf("Failed to request gpio %d\n",
				gpio_test_param->gpio_num);
		return CMD_FAIL;
	}
	if (gpio_direction_output(gpio_test_param->gpio_num,
			    gpio_test_param->value)) {
		printf("Failed to set gpio value %d\n",
				gpio_test_param->gpio_num);
		return CMD_FAIL;
	}

	gpio_free(gpio_test_param->gpio_num);
	return CMD_SUCCESS;
}
