#include <common.h>
#include <io.h>
#include <stdio.h>
#include <pad.h>
#include "i2c.h"

#include "ip808.h"
#include "ip808lib.h"

//extern int printf(const char *fmt, ...);
static int regmap_bulk_write(int device, int write_addr, int write_value, int len)
{

	u8 buf[2] = {(u8)write_addr,(u8)write_value};

	i2c_write(1, (u8)device, buf, 2);

	return 0;
}

static int regmap_bulk_read(int device, int read_addr, u8 * read_value, int len)
{

	i2c_read(1, (u8)device, (u8)read_addr, read_value, (u32)len);


	return 0;
}

int PoE_i2c_write(int device, int write_addr, int write_value)
{
	return regmap_bulk_write(device, write_addr, write_value, 1);
}

int PoE_i2c_read(int device, int read_addr, unsigned char *value)
{
	return regmap_bulk_read(device, read_addr, value, 1);
}

int sfax8_ip808_init(void)
{

	sf_module_set_pad_func(SF_I2C1);

	IP808_init(0x50);

	return 0;
}


