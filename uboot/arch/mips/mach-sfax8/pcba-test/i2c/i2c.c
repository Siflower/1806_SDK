/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <mach/sfax8.h>
#include <asm/io.h>
#include <errno.h>

#define I2C_BASE_ADDR (0xB8100000)
#define I2C_CON (0x00 + I2C_BASE_ADDR) // I2C Control register
#define I2C_TAR (0x04 + I2C_BASE_ADDR) // I2C Targer address register
#define I2C_DATA_CMD                                                           \
	(0x10 + I2C_BASE_ADDR) // I2C Rx/Tx data buffer and command register
#define I2C_SS_SCL_HCNT                                                        \
	(0x14 + I2C_BASE_ADDR) // Standard speed I2C clock SCL high count
			       // register
#define I2C_SS_SCL_LCNT                                                        \
	(0x18 + I2C_BASE_ADDR) // Standard speed I2C clock SCL low count
			       // register
#define I2C_ENABLE (0x6C + I2C_BASE_ADDR)     // I2C enable register
#define I2C_STATUS (0x70 + I2C_BASE_ADDR)     // I2C status register
#define I2C_TX_ABRT	(0x80 + I2C_BASE_ADDR)	  // I2c tx abrt source register
#define STATUS_RFNE (1 << 3)


void i2c_reset(void)
{

	writew(0x0, (void *)(IIC_SYSM_RESET));
	writew(0xff, (void *)(IIC_SYSM_RESET));

	writew(0xFF, (void *)(IIC_SOFT_CLK_EN));

	writew(0xFF, (void *)(IIC_SOFT_RESET));
	writew(0x00, (void *)(IIC_SOFT_RESET));
	writew(0x0F, (void *)(IIC_SOFT_BOE));
}

/*
*i2c_write() transmit data to peripheral
*@buf: the data transmitted to peripheral
*@len: the data len
*@addr: the peripheral addr
*@n: the i2c controller number
*
*return 0 if tranmit success,
*retrun 1 if wait inactive time out.
*/
int i2c_write(u8 *buf, u8 len, u8 addr, int n)
{
	u32 i = 0;

	// disable ctrl
	writew(0x0, (void *)(n * 0x1000 + I2C_ENABLE));

	// config ctrl
	writew(0x6b, (void *)(n * 0x1000 + I2C_CON));

	// set speed
	writew(0xf0, (void *)(n * 0x1000 + I2C_SS_SCL_HCNT));
	writew(0xf8, (void *)(n * 0x1000 + I2C_SS_SCL_LCNT));

	// set target
	writew(addr, (void *)(n * 0x1000 + I2C_TAR));

	// enable ctrl
	writew(0x1, (void *)(n * 0x1000 + I2C_ENABLE));

	// write data
	while (i < len) {
		writew(buf[i], (void *)(n * 0x1000 + I2C_DATA_CMD));
		i++;
	}

	//check slave ack
	for(i = 0; i < 1000; i++){
		if(readw((void *)(n * 0x1000 + I2C_TX_ABRT)) & 0x1){
			return 1;	// 7-bit slave address not ack
		}
		udelay(10);
	}
	//wait controller inactive
	for(i = 0 ; i < 1000; i++){
		if(!(readw((void *)(n * 0x1000 + I2C_STATUS)) & 0x1))
			return 0;
		mdelay(1);
	}
	return 1;
}

/*
* i2c read interface.
*@addr: peripheral addr
*@reg: peripheral register
*@buf: where the data stroed in
*@len: the data length
*@n: then i2c controller number
*@return 0: success; 1:timeout
*/
int i2c_read(u8 addr, u8 reg, u8 *buf, u8 len, int n)
{
	u32 i = 0;
	u32 m = 0;
	// disable ctrl
	writew(0x0, (void *)(n * 0x1000 + I2C_ENABLE));

	// config ctrl
	writew(0x6b, (void *)(n * 0x1000 + I2C_CON));

	// set speed
	writew(0xf0, (void *)(n * 0x1000 + I2C_SS_SCL_HCNT));
	writew(0xf8, (void *)(n * 0x1000 + I2C_SS_SCL_LCNT));

	// set target
	writew(addr, (void *)(n * 0x1000 + I2C_TAR));

	// enable ctrl
	writew(0x1, (void *)(n * 0x1000 + I2C_ENABLE));

	// write data
	writew(reg, (void *)(n * 0x1000 + I2C_DATA_CMD));

	writew(0x100, (void *)(n * 0x1000 + I2C_DATA_CMD));

	while (i < len) {
		if(readw((void *)(n * 0x1000 + I2C_STATUS)) & STATUS_RFNE){
			buf[i++] = (u8)readw((void *)(n * 0x1000 + I2C_DATA_CMD));
		}else{
			udelay(200);
			m++;
			if(m > 100)
				goto err;
		}
	}
	//disable i2c control
	writew(0x0, (void *)(n * 0x1000 + I2C_ENABLE));

	//wait inactive.
	for(i = 0 ; i < 1000; i++){
		if(!(readw((void *)(n * 0x1000 + I2C_STATUS)) & 0x1))
			return 0;
		mdelay(1);
	}
err:
	return 1;
}
