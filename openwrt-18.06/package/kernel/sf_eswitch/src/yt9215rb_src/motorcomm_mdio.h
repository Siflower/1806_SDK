/* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#ifndef __MOTORCOMM_MDIO_H__
#define __MOTORCOMM_MDIO_H__

#include <linux/phy.h>

struct motorcomm_sw_reg_ops {
    void (*sw_reg_write)(struct switch_dev *, u32, u32);
    void (*sw_reg_read)(struct switch_dev *, u32, u32 *);
    void (*sw_reg_rmw)(struct switch_dev *, u32, u32, u32);
};

struct motorcomm_priv {
    struct switch_dev swDev;
    struct device *dev;
    uint8_t  switchId;
    uint8_t  devAddr;
    struct mii_bus *hostMiiBus;
    const struct motorcomm_sw_reg_ops *ops;
    const struct motorcomm_switchs *sw;
    /* protect among processes for registers access*/
    struct mutex reg_mutex;
    /* protect among processes for config access*/
    struct mutex cfg_mutex;
};

enum motorcomm_chip_id {
    YT92XX = 0x9002,
};

struct motorcomm_sw_desc {
    enum motorcomm_chip_id chipId;
    const char *name;
    uint8_t  switchId;
    uint8_t  devAddr;
    int (*detect)(struct motorcomm_priv *);
    int (*init)(struct motorcomm_priv *);
};

struct motorcomm_switchs {
    const struct switch_dev_ops *swDevOps;
    const struct motorcomm_sw_desc *swDesc;
};

#endif
