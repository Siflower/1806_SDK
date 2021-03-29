/*
 * sfax8_thermal.c - Siflower Ax8 serials thermal glue.
 * This driver provide an interface between thermal core and our cooling
 * device, to simplify devices' register and unregister.
 *
 * Copyright (C) 2017 Shanghai Siflower Communication Technology Co., Ltd.
 *
 * Author: Qi Zhang <qi.zhang@siflower.com.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

//#include <linux/cpu_cooling.h>
#include <linux/cpufreq.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/thermal.h>
#include <sfax8_thermal.h>

struct sfax8_sensor_data {
	struct device *dev;
	struct thermal_zone_device *tzd;
	void *data;
	struct sfax8_zone_device_ops *ops;
	long last_temp;
	long temp;
	struct list_head node;
};

struct sfax8_cooling_device_data {
	struct device *dev;
	struct thermal_cooling_device *cdev;
	struct sfax8_cooling_device_ops *ops;
	void *data;
	struct list_head node;
};

static LIST_HEAD(sfax8_sensor_list);
static LIST_HEAD(sfax8_cdev_list);

static DEFINE_MUTEX(sfax8_sensor_list_lock);
static DEFINE_MUTEX(sfax8_cdev_list_lock);


static int __sfax8_get_temperature(void *data,int *temp)
{
	struct sfax8_sensor_data *sensor_data = data;
	int ret;

	if (!sensor_data)
		return -EINVAL;

	ret = sensor_data->ops->get_temp(sensor_data->data, temp);
	sensor_data->last_temp = sensor_data->temp;
	sensor_data->temp = *temp;

	return ret;
}

static int __sfax8_get_trend(void *data,int a, enum thermal_trend *trend)
{
	struct sfax8_sensor_data *sensor_data = data;
	long delta;

	if (!sensor_data)
		return -EINVAL;

	if (sensor_data->ops->get_trend)
		return sensor_data->ops->get_trend(sensor_data->data, trend);

	if ((sensor_data->temp == THERMAL_TEMP_INVALID) ||
	    (sensor_data->last_temp == THERMAL_TEMP_INVALID)) {
		*trend = 0;
		return 0;
	}

	delta = sensor_data->temp - sensor_data->last_temp;
	*trend = (abs(delta) < 3) ? 0 : delta;
	return 0;
}

static const struct thermal_zone_of_device_ops sfax8_of_thermal_ops={
	.get_temp = __sfax8_get_temperature,
	.get_trend = __sfax8_get_trend,
};

int sfax8_thermal_sensor_register(struct device *dev,
				struct sfax8_zone_device_ops *ops, void *data)
{
	int sensor_id = 0;
	struct sfax8_sensor_data *sdata;
	struct thermal_zone_device *tzd;
	if (!ops->get_temp)
		return -EINVAL;

	sdata = kzalloc(sizeof(*sdata), GFP_KERNEL);
	if (!sdata)
		return -ENOMEM;
	printk("**************************************************************%s",__func__);
	sdata->dev = dev;
	sdata->data = data;
	sdata->ops = ops;

	tzd = thermal_zone_of_sensor_register(dev, sensor_id, sdata,
		&sfax8_of_thermal_ops);
	if (IS_ERR(tzd))
		return PTR_ERR(tzd);

	sdata->tzd = tzd;

	mutex_lock(&sfax8_sensor_list_lock);
	list_add_tail(&sdata->node, &sfax8_sensor_list);
	mutex_unlock(&sfax8_sensor_list_lock);

	return 0;
}
EXPORT_SYMBOL_GPL(sfax8_thermal_sensor_register);

static void __sfax8_thermal_sensor_unregister(struct sfax8_sensor_data *sdata)
{
	thermal_zone_of_sensor_unregister(sdata->dev, sdata->tzd);
}

void sfax8_thermal_sensor_unregister(struct device *dev)
{
	struct sfax8_sensor_data *pos, *next;

	mutex_lock(&sfax8_sensor_list_lock);
	list_for_each_entry_safe(pos, next, &sfax8_sensor_list, node) {
		if (pos->dev != dev)
			continue;

		__sfax8_thermal_sensor_unregister(pos);
		list_del(&pos->node);
		mutex_unlock(&sfax8_sensor_list_lock);
		kfree(pos);
		return;
	}
	mutex_unlock(&sfax8_sensor_list_lock);

	return;
}
EXPORT_SYMBOL_GPL(sfax8_thermal_sensor_unregister);

static int sfax8_get_max_state(struct thermal_cooling_device *cdev, unsigned long *state)
{
	struct sfax8_cooling_device_data *cdata = cdev->devdata;

	return cdata->ops->get_max_state(cdata->data, state);
}

static int sfax8_get_cur_state(struct thermal_cooling_device *cdev, unsigned long *state)
{
	struct sfax8_cooling_device_data *cdata = cdev->devdata;

	return cdata->ops->get_cur_state(cdata->data, state);
}

static int sfax8_set_cur_state(struct thermal_cooling_device *cdev, unsigned long state)
{
	struct sfax8_cooling_device_data *cdata = cdev->devdata;

	return cdata->ops->set_cur_state(cdata->data, state);
}

struct thermal_cooling_device_ops sfax8_thermal_ops = {
	.get_max_state = sfax8_get_max_state,
	.get_cur_state = sfax8_get_cur_state,
	.set_cur_state = sfax8_set_cur_state,
};

int sfax8_cooling_device_register(struct device *dev, void *devdata,
				  struct sfax8_cooling_device_ops *ops)
{
	struct device_node *np;
	struct thermal_cooling_device *cdev;
	struct sfax8_cooling_device_data *cdata;
	char type[THERMAL_NAME_LENGTH];

	if (!dev || !dev->of_node)
		return -ENODEV;

	if (!ops || !ops->get_max_state || !ops->get_cur_state ||
	    !ops->set_cur_state)
		return -EINVAL;

	cdata = kzalloc(sizeof(*cdata), GFP_KERNEL);
	if (!cdata)
		return -ENOMEM;

	np = dev->of_node;
	strlcpy(type, np->name, sizeof(type));

	cdata->data = devdata;
	cdata->ops = ops;

	cdev = thermal_of_cooling_device_register(np, type, cdata, &sfax8_thermal_ops);
	if (IS_ERR(cdev))
		return PTR_ERR(cdev);

	cdata->cdev = cdev;
	cdata->dev = dev;

	mutex_lock(&sfax8_cdev_list_lock);
	list_add_tail(&cdata->node, &sfax8_cdev_list);
	mutex_unlock(&sfax8_cdev_list_lock);

	return 0;
}
EXPORT_SYMBOL_GPL(sfax8_cooling_device_register);

void sfax8_cooling_device_unregister(struct device *dev)
{
	struct sfax8_cooling_device_data *pos, *next;

	mutex_lock(&sfax8_cdev_list_lock);
	list_for_each_entry_safe(pos, next, &sfax8_cdev_list, node) {
		if (pos->dev != dev)
			continue;

		thermal_cooling_device_unregister(pos->cdev);
		list_del(&pos->node);
		kfree(pos);
		mutex_unlock(&sfax8_cdev_list_lock);
		return;
	}
	mutex_unlock(&sfax8_cdev_list_lock);
}
EXPORT_SYMBOL_GPL(sfax8_cooling_device_unregister);

/* Nothing to be done for module init */
static int __init sfax8_thermal_init(void)
{
	return 0;
}

static void __exit sfax8_thermal_exit(void)
{
	struct sfax8_cooling_device_data *cpos, *cnext;
	struct sfax8_sensor_data *spos, *snext;

	/* unregister cooling devices */
	mutex_lock(&sfax8_cdev_list_lock);
	list_for_each_entry_safe(cpos, cnext, &sfax8_cdev_list, node) {
		thermal_cooling_device_unregister(cpos->cdev);
		list_del(&cpos->node);
		kfree(cpos);
	}
	mutex_unlock(&sfax8_cdev_list_lock);

	/* unregister thermal sensors */
	mutex_lock(&sfax8_sensor_list_lock);
	list_for_each_entry_safe(spos, snext, &sfax8_sensor_list, node) {
		__sfax8_thermal_sensor_unregister(spos);
		list_del(&spos->node);
		kfree(spos);
	}
	mutex_unlock(&sfax8_sensor_list_lock);

	return;
}

module_init(sfax8_thermal_init);
module_exit(sfax8_thermal_exit);

MODULE_AUTHOR("Qi Zhang <qi.zhang@siflower.com.cn>");
MODULE_DESCRIPTION("Siflower 16Ax8 serials thermal sensor");
MODULE_LICENSE("GPL");
