#include <linux/of_gpio.h>
#include "siwifi_led.h"
#include "siwifi_mem.h"

#define MAC80211_BLINK_DELAY 50 /* ms */

int siwifi_hw_set_gpio(struct siwifi_hw *hw, u32 val)
{
    if (hw->led_pin < 0) {
        printk(KERN_ERR "error: set_gpio failed: no such gpio!<<<<<<<<<<<<<<<<<<<<<<<\n");
        return -ENODEV;
    }
    gpio_set_value(hw->led_pin, val);
    return 0;
}

// siwifi_set_gpio_pin(hw, gpio_lable) @gpio_lable: "gpio-leds"
int siwifi_set_gpio_pin(struct siwifi_hw *hw, char *name)
{
    int ret = 0;
#ifdef CONFIG_LED_MULTICOLOUR
    struct device_node *np, *child;
    const char *state;
    int default_on_gpio;
#endif
    hw->led_on = 1;
    hw->led_pin = of_get_named_gpio(hw->dev->of_node, name, 0);

#ifdef CONFIG_LED_MULTICOLOUR
    np = of_find_node_by_path("/gpio-leds");
    if (!np) {
        printk(KERN_ERR "Failed to find /gpio-leds node\n");
        return -ENODEV;
    }

    for_each_child_of_node(np, child) {
        if (!of_property_read_string(child, "default-state", &state)
            && strcmp(state, "on") == 0) {
            default_on_gpio = of_get_named_gpio(child, "gpios", 0);
            if (default_on_gpio >= 0) {
                printk(KERN_INFO "Node %pOF has GPIO %d\n", child, default_on_gpio);
#ifdef CONFIG_LED_PIN_ACTIVE_LOW
                gpio_direction_output(default_on_gpio, hw->led_on);
#else
                gpio_direction_output(default_on_gpio, !hw->led_on);
#endif
                break;
            }
        }
    }
#endif

    if (hw->led_pin < 0) {
        printk(KERN_ERR "error: of_get_named_gpio failed!<<<<<<<<<<<<<<<<<<<<<<<\n");
        return -ENODEV;
    } else
        printk(KERN_INFO "info: gpio-%u is got successfully!<<<<<<<<<<<<<<<<<<<<<<<\n", hw->led_pin);

    if (!gpio_is_valid(hw->led_pin))
        printk(KERN_ERR "error: gpio-%u is invalid!<<<<<<<<<<<<<<<<<<<<<<<\n", hw->led_pin);

    ret = devm_gpio_request(hw->dev, hw->led_pin, name); //-16 means gpio ebusy.
    if (ret == -16) {
        printk(KERN_INFO "info ret = %d, request failed but keep setting!<<<<<<<<<<<<<<<<<<<<<<<\n", ret);
        ret = 0;
        goto out;
    }
    /*
       ret = gpio_request_one(hw->led_pin, GPIOF_OUT_INIT_LOW, name);
    //a gpio/a group gpios   gpio_request/devm_gpio_request
    */
    else if (ret < 0) {
        printk(KERN_ERR "error ret = %d: gpio_request gpio-%u is failed!<<<<<<<<<<<<<<<<<<<<<<<\n", ret, hw->led_pin);
        return -ENODEV;
    } else
#ifdef CONFIG_LED_PIN_ACTIVE_LOW
        ret = gpio_direction_output(hw->led_pin, !hw->led_on);
#else
        ret = gpio_direction_output(hw->led_pin, hw->led_on); // hw->led_on or hw->led_off
#endif
    if (ret)
        printk(KERN_ERR "error: gpio_direction_output failed!<<<<<<<<<<<<<<<<<<<<<<<\n");
    else
        goto out;
out:
    return ret;
}


int siwifi_led_register(struct siwifi_hw *hw, struct siwifi_led *led,
                const char *name, const char *trigger)
{
    int err;
    led->hw = hw;
    strncpy(led->name, name, sizeof(led->name));
    led->name[sizeof(led->name) - 1] = 0;
    led->led_dev.name = led->name;
    led->led_dev.default_trigger = trigger;
    led->led_dev.brightness_set = siwifi_led_brightness_set;

    err = led_classdev_register(hw->dev, &led->led_dev);
    if (err)
        led->hw = NULL;
    return err;
}

static void siwifi_led_off(struct siwifi_hw *hw)
{
    siwifi_hw_set_gpio(hw, !hw->led_on);
}

static void siwifi_led_on(struct siwifi_hw *hw)
{
    siwifi_hw_set_gpio(hw, hw->led_on);
}

static void siwifi_led_unregister(struct siwifi_led *led)
{
    if (!led->hw)
        return;
    led_classdev_unregister(&led->led_dev);
    siwifi_led_off(led->hw);
    led->hw = NULL;
}

void siwifi_leds_unregister(struct siwifi_hw *siwifi_hw)
{
	//printk("=====siwifi_leds_unregister=====\n");
	siwifi_led_unregister(&siwifi_hw->rx_led);
    siwifi_led_unregister(&siwifi_hw->tx_led);
	led_trigger_unregister(&siwifi_hw->local_rx_led);
    led_trigger_unregister(&siwifi_hw->local_tx_led);
}

void siwifi_led_brightness_set(struct led_classdev *led_dev,
                enum led_brightness brightness)
{
    struct siwifi_led *led = container_of(led_dev, struct siwifi_led,
                    led_dev);
    if (led->hw->mod_params->led_status == 0)
        siwifi_led_off(led->hw);
    else if (led->hw->mod_params->led_status == 2)
        siwifi_led_on(led->hw);
    else if (brightness == LED_OFF)
        siwifi_led_off(led->hw);
    else
        siwifi_led_on(led->hw);

    return;
}


int siwifi_led_init(struct siwifi_hw *siwifi_hw)
{
    int ret = 0;
    char *gpio_lable = LED_GPIO_LABEL; //lable in dts;
    char name[SIWIFI_LED_MAX_NAME_LEN + 1];

	ret = siwifi_set_gpio_pin(siwifi_hw, gpio_lable); //set and bind gpio pin to siwifi_hw;

    if (ret)
        goto out;
    snprintf(name, sizeof(name), "%srx", wiphy_name(siwifi_hw->wiphy));

    ret = siwifi_led_register(siwifi_hw, &siwifi_hw->rx_led, name, siwifi_hw->local_rx_led.name);

    if (ret) {
        printk(KERN_ERR "error: rx_register_init failed!<<<<<<<<<<<<<<<<<<<<<<<\n");
        goto out;
    }
    snprintf(name, sizeof(name), "%stx", wiphy_name(siwifi_hw->wiphy));
    ret = siwifi_led_register(siwifi_hw, &siwifi_hw->tx_led, name, siwifi_hw->local_tx_led.name);
    if (ret) {
        printk(KERN_ERR "error: tx_register_init failed!<<<<<<<<<<<<<<<<<<<<<<<\n");
        goto out;
    }
out:
    return ret;
}

void siwifi_led_deinit(struct siwifi_hw *hw)
{
    siwifi_leds_unregister(hw);
    siwifi_local_free_led_names(hw);
}

void siwifi_led_rx(struct siwifi_hw *siwifi_hw)
{
	unsigned long led_delay = MAC80211_BLINK_DELAY;
	if (!atomic_read(&siwifi_hw->rx_led_active))
		return;
	if(&siwifi_hw->local_rx_led != NULL){
		led_trigger_blink_oneshot(&siwifi_hw->local_rx_led, &led_delay, &led_delay, 0);
//	printk("===== led siwifi_led_rx=====\n");
	}
}

void siwifi_led_tx(struct siwifi_hw *siwifi_hw)
{
	unsigned long led_delay = MAC80211_BLINK_DELAY;
	if (!atomic_read(&siwifi_hw->tx_led_active))
		return;
	if(&siwifi_hw->local_tx_led != NULL){
		led_trigger_blink_oneshot(&siwifi_hw->local_tx_led, &led_delay, &led_delay, 0);
//	printk("===== led siwifi_led_tx=====\n");
	}
}

static void siwifi_local_rx_led_activate(struct led_classdev *led_cdev)
{
	struct siwifi_hw *siwifi_hw = container_of(led_cdev->trigger,
						     struct siwifi_hw,
						     local_rx_led);

	atomic_inc(&siwifi_hw->rx_led_active);
}

static void siwifi_local_rx_led_deactivate(struct led_classdev *led_cdev)
{
	struct siwifi_hw *siwifi_hw = container_of(led_cdev->trigger,
						     struct siwifi_hw,
						     local_rx_led);

	atomic_dec(&siwifi_hw->rx_led_active);
}

static void siwifi_local_tx_led_activate(struct led_classdev *led_cdev)
{
	struct siwifi_hw *siwifi_hw = container_of(led_cdev->trigger,
						     struct siwifi_hw,
						     local_tx_led);

	atomic_inc(&siwifi_hw->tx_led_active);
}

static void siwifi_local_tx_led_deactivate(struct led_classdev *led_cdev)
{
	struct siwifi_hw *siwifi_hw = container_of(led_cdev->trigger,
						     struct siwifi_hw,
						     local_tx_led);

	atomic_dec(&siwifi_hw->tx_led_active);
}

int siwifi_local_alloc_led_names(struct siwifi_hw *siwifi_hw)
{
	siwifi_hw->local_rx_led.name = kasprintf(GFP_KERNEL, "%srx", wiphy_name(siwifi_hw->wiphy));
	if (!siwifi_hw->local_rx_led.name)
		return -ENOMEM;

	siwifi_hw->local_tx_led.name = kasprintf(GFP_KERNEL, "%stx", wiphy_name(siwifi_hw->wiphy));
	if (!siwifi_hw->local_tx_led.name) {
		siwifi_kfree(siwifi_hw->local_rx_led.name);
		return -ENOMEM;
	}

	return 0;
}

void siwifi_local_free_led_names(struct siwifi_hw *siwifi_hw)
{
	siwifi_kfree(siwifi_hw->local_rx_led.name);
	siwifi_kfree(siwifi_hw->local_tx_led.name);
}

void siwifi_local_led_init(struct siwifi_hw *siwifi_hw)
{
	if (siwifi_local_alloc_led_names(siwifi_hw)) {
		printk(KERN_ERR "Failed to allocate LED names\n");
		return;
	}

	atomic_set(&siwifi_hw->rx_led_active, 1);
	siwifi_hw->local_rx_led.activate = siwifi_local_rx_led_activate;
	siwifi_hw->local_rx_led.deactivate = siwifi_local_rx_led_deactivate;
	//printk("===== register name %s ================\n",siwifi_hw->local_rx_led.name);
	if (siwifi_hw->local_rx_led.name && led_trigger_register(&siwifi_hw->local_rx_led)) {
		//printk("===== register %s fail================\n",siwifi_hw->local_rx_led.name);
		siwifi_kfree(siwifi_hw->local_rx_led.name);
		siwifi_hw->local_rx_led.name = NULL;
	}

	atomic_set(&siwifi_hw->tx_led_active, 1);
	siwifi_hw->local_tx_led.activate = siwifi_local_tx_led_activate;
	siwifi_hw->local_tx_led.deactivate = siwifi_local_tx_led_deactivate;
	if (siwifi_hw->local_tx_led.name && led_trigger_register(&siwifi_hw->local_tx_led)) {
		//printk("===== register %s fail================\n",siwifi_hw->local_tx_led.name);
		siwifi_kfree(siwifi_hw->local_tx_led.name);
		siwifi_hw->local_tx_led.name = NULL;
	}
}
