
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
 *        Created:  11/7/2018 09:42:52 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  star , star.jiang@siflower.com.cn
 *        Company:  Siflower Communication Tenology Co.,Ltd
 *
 * =====================================================================================
 */
#ifndef _FACTORY_READ_SYSFS_H_
#define _FACTORY_READ_SYSFS_H_
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
int sf_factory_read_sysfs_register(struct platform_device *pdev, char *parent);

/*func:
 * destroy the sysfs created
 * params:
 * struct platform_driver *pdev
 *
 **/
int sf_factory_read_sysfs_unregister(struct platform_device *pdev);
#endif
