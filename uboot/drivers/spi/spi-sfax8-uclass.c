/*
 * Copyright (c) 2014 Google, Inc
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <fdtdec.h>
#include <malloc.h>
#include <spi.h>
#include <dm/device-internal.h>
#include <dm/uclass-internal.h>
#include <dm/root.h>
#include <dm/lists.h>
#include <dm/util.h>

DECLARE_GLOBAL_DATA_PTR;

int dm_spi_claim_bus(struct udevice *dev)
{
	struct udevice *bus = dev->parent;
	struct dm_spi_ops *ops = spi_get_ops(bus);

	return ops->claim_bus ? ops->claim_bus(dev) : 0;
}

void dm_spi_release_bus(struct udevice *dev)
{}

int dm_spi_xfer(struct udevice *dev, unsigned int bitlen,
		const void *dout, void *din, unsigned long flags)
{
	struct udevice *bus = dev->parent;

	if (bus->uclass->uc_drv->id != UCLASS_SPI)
		return -EOPNOTSUPP;

	return spi_get_ops(bus)->xfer(dev, bitlen, dout, din, flags);
}

int spi_claim_bus(struct spi_slave *slave)
{
	return dm_spi_claim_bus(slave->dev);
}

void spi_release_bus(struct spi_slave *slave)
{
	dm_spi_release_bus(slave->dev);
}

int spi_xfer(struct spi_slave *slave, unsigned int bitlen,
	     const void *dout, void *din, unsigned long flags)
{
	return dm_spi_xfer(slave->dev, bitlen, dout, din, flags);
}

static int spi_post_bind(struct udevice *dev)
{
	/* Scan the bus for devices */
	return dm_scan_fdt_node(dev, gd->fdt_blob, dev->of_offset, false);
}

static int spi_child_post_bind(struct udevice *dev)
{
	struct dm_spi_slave_platdata *plat = dev_get_parent_platdata(dev);

	if (dev->of_offset == -1)
		return 0;

	return spi_slave_ofdata_to_platdata(gd->fdt_blob, dev->of_offset, plat);
}

static int spi_child_pre_probe(struct udevice *dev)
{
	struct spi_slave *slave = dev_get_parent_priv(dev);

	/*
	 * This is needed because we pass struct spi_slave around the place
	 * instead slave->dev (a struct udevice). So we have to have some
	 * way to access the slave udevice given struct spi_slave. Once we
	 * change the SPI API to use udevice instead of spi_slave, we can
	 * drop this.
	 */
	slave->dev = dev;

	return 0;
}

int spi_chip_select(struct udevice *dev)
{
	struct dm_spi_slave_platdata *plat = dev_get_parent_platdata(dev);

	return plat ? plat->cs : -ENOENT;
}

int spi_find_chip_select(struct udevice *bus, int cs, struct udevice **devp)
{
	struct udevice *dev;

	for (device_find_first_child(bus, &dev); dev;
	     device_find_next_child(&dev)) {
		struct dm_spi_slave_platdata *plat;

		plat = dev_get_parent_platdata(dev);
		debug("%s: plat=%p, cs=%d\n", __func__, plat, plat->cs);
		if (plat->cs == cs) {
			*devp = dev;
			return 0;
		}
	}

	return -ENODEV;
}

int spi_cs_is_valid(unsigned int busnum, unsigned int cs)
{
	struct spi_cs_info info;
	struct udevice *bus;
	int ret;

	ret = uclass_find_device_by_seq(UCLASS_SPI, busnum, false, &bus);
	if (ret) {
		debug("%s: No bus %d\n", __func__, busnum);
		return ret;
	}

	return spi_cs_info(bus, cs, &info);
}

int spi_cs_info(struct udevice *bus, uint cs, struct spi_cs_info *info)
{
	struct spi_cs_info local_info;
	int ret;

	if (!info)
		info = &local_info;

	/* If there is a device attached, return it */
	info->dev = NULL;
	ret = spi_find_chip_select(bus, cs, &info->dev);
	if (!ret)
		return 0;
	/*
	 * We could assume there is at least one valid chip select, but best
	 * to be sure and return an error in this case. The driver didn't
	 * care enough to tell us.
	 */
	return -ENODEV;
}

int spi_find_bus_and_cs(int busnum, int cs, struct udevice **busp,
			struct udevice **devp)
{
	struct udevice *bus, *dev;
	int ret;

	ret = uclass_find_device_by_seq(UCLASS_SPI, busnum, false, &bus);
	if (ret) {
		debug("%s: No bus %d\n", __func__, busnum);
		return ret;
	}
	ret = spi_find_chip_select(bus, cs, &dev);
	if (ret) {
		debug("%s: No cs %d\n", __func__, cs);
		return ret;
	}
	*busp = bus;
	*devp = dev;

	return ret;
}

int spi_get_bus_and_cs(int busnum, int cs, int speed, int mode,
		       const char *drv_name, const char *dev_name,
		       struct udevice **busp, struct spi_slave **devp)
{
	struct udevice *bus, *dev;
	bool created = false;
	int ret;

	ret = uclass_get_device_by_seq(UCLASS_SPI, busnum, &bus);
	if (ret) {
		printf("Invalid bus %d (err=%d)\n", busnum, ret);
		return ret;
	}
	ret = spi_find_chip_select(bus, cs, &dev);

	/*
	 * If there is no such device, create one automatically. This means
	 * that we don't need a device tree node or platform data for the
	 * SPI flash chip - we will bind to the correct driver.
	 */
	if (ret == -ENODEV && drv_name) {
		struct dm_spi_slave_platdata *plat;

		debug("%s: Binding new device '%s', busnum=%d, cs=%d, driver=%s\n",
		      __func__, dev_name, busnum, cs, drv_name);
		ret = device_bind_driver(bus, drv_name, dev_name, &dev);
		if (ret)
			return ret;
		plat = dev_get_parent_platdata(dev);
		plat->cs = cs;
		created = true;
	} else if (ret) {
		printf("Invalid chip select %d:%d (err=%d)\n", busnum, cs,
		       ret);
		return ret;
	}

	if (!device_active(dev)) {
		struct spi_slave *slave;

		ret = device_probe(dev);
		if (ret)
			goto err;
		slave = dev_get_parent_priv(dev);
		slave->dev = dev;
	}

	*busp = bus;
	*devp = dev_get_parent_priv(dev);
	debug("%s: bus=%p, slave=%p\n", __func__, bus, *devp);

	return 0;

err:
	debug("%s: Error path, credted=%d, device '%s'\n", __func__,
	      created, dev->name);
	if (created) {
		device_remove(dev);
		device_unbind(dev);
	}

	return ret;
}

void spi_free_slave(struct spi_slave *slave)
{
	device_remove(slave->dev);
	slave->dev = NULL;
}

int spi_slave_ofdata_to_platdata(const void *blob, int node,
				 struct dm_spi_slave_platdata *plat)
{
	plat->cs = fdtdec_get_int(blob, node, "reg", -1);

	return 0;
}

UCLASS_DRIVER(spi) = {
	.id		= UCLASS_SPI,
	.name		= "spi",
	.flags		= DM_UC_FLAG_SEQ_ALIAS,
	.post_bind	= spi_post_bind,
	.child_pre_probe = spi_child_pre_probe,
	.per_device_auto_alloc_size = sizeof(struct dm_spi_bus),
	.per_child_auto_alloc_size = sizeof(struct spi_slave),
	.per_child_platdata_auto_alloc_size =
			sizeof(struct dm_spi_slave_platdata),
	.child_post_bind = spi_child_post_bind,
};

UCLASS_DRIVER(spi_generic) = {
	.id		= UCLASS_SPI_GENERIC,
	.name		= "spi_generic",
};

U_BOOT_DRIVER(spi_generic_drv) = {
	.name		= "spi_generic_drv",
	.id		= UCLASS_SPI_GENERIC,
};
