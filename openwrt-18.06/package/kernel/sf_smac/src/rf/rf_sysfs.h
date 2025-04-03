
/*
 * =====================================================================================
 *
 *       Filename:  rf_sysfs.h
 *
 *    Description: this is used for system debug fs for rf, this functions will be implemented:
 *                  1,read/write rf register
 *                  2,force to do calibrations and get the calibration data
 *                  3,how many bandband are successfully registered
 *
 *
 *        Version:  1.0
 *        Created:  12/31/2016 09:42:52 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Siflower Communication Tenology Co.,Ltd
 *
 * =====================================================================================
 */
#ifndef _WIFI_RF_SYSFS_H_
#define _WIFI_RF_SYSFS_H_
#include <linux/platform_device.h>

/*func:
 * create a sysfs debug node with a specified parent node
 *  if the parent is not exsit, just create it
 * params:
 *  struct platform_driver *pdev
 *  char *parent
 * return:
 * 0 successful, otherwise failed
 **/
int sf_wifi_rf_sysfs_register(struct platform_device *pdev, char *parent);

/*func:
 * destroy the sysfs created
 * params:
 * struct platform_driver *pdev
 *
 **/
int sf_wifi_rf_sysfs_unregister(struct platform_device *pdev);
#endif
