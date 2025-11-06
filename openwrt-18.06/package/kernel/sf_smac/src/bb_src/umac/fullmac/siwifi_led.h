/**
 ******************************************************************************
 *
 * @file siwifi_led.h
 *
 * @brief siwifi fullmac led func
 *
 * Copyright (C) Siflower
 *
 ******************************************************************************
 */

#ifndef _SIWIFI_LED_H_
#define _SIWIFI_LED_H_

#include "siwifi_defs.h"

void siwifi_led_rx(struct siwifi_hw *siwifi_hw);
void siwifi_led_tx(struct siwifi_hw *siwifi_hw);
static void siwifi_led_unregister(struct siwifi_led *led);
void siwifi_leds_unregister(struct siwifi_hw *hw);
static void siwifi_led_off(struct siwifi_hw *hw);
static void siwifi_led_on(struct siwifi_hw *hw);
void siwifi_led_brightness_set(struct led_classdev *led_dev, enum led_brightness brightness);
int siwifi_set_gpio_pin(struct siwifi_hw *hw, char *name);
int siwifi_led_register(struct siwifi_hw *hw, struct siwifi_led *led, const char *name, const char *trigger);
int siwifi_led_init(struct siwifi_hw *siwifi_hw);
void siwifi_led_deinit(struct siwifi_hw *hw);
void siwifi_local_free_led_names(struct siwifi_hw *siwifi_hw);
void siwifi_local_led_init(struct siwifi_hw *siwifi_hw);

#endif /* _SIWIFI_LED_H_ */
