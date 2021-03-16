#ifndef _SFAX8_THERMAL_H_
#define _SFAX8_THERMAL_H_

struct sfax8_zone_device_ops{
	int (*get_temp)(void *, int *);
	int (*get_trend)(void *,enum thermal_trend *);
};

struct sfax8_cooling_device_ops {
	int (*get_max_state)(void *, unsigned long *);
	int (*get_cur_state)(void *, unsigned long *);
	int (*set_cur_state)(void *, unsigned long);
};

#ifdef CONFIG_SFAX8_THERMAL
int sfax8_thermal_sensor_register(struct device *dev,
			struct sfax8_zone_device_ops *ops, void *data);
void sfax8_thermal_sensor_unregister(struct device *dev);

int sfax8_cooling_device_register(struct device *dev, void *devdata,
			struct sfax8_cooling_device_ops *ops);
void sfax8_cooling_device_unregister(struct device *dev);
#else
inline int sfax8_thermal_sensor_register(struct device *dev,
			  int (*get_temp)(void *, long *),
			  int (*get_trend)(void *, long *), void *data)
{
	return 0;
}
void sfax8_thermal_sensor_unregister(struct device *dev) {}

inline int sfax8_cooling_device_register(struct device *dev, void *devdata,
			struct sfax8_cooling_device_ops *ops)
{
	return 0;
}
void sfax8_cooling_device_unregister(struct device *dev) {}
#endif

#endif
