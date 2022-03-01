/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <errno.h>
#include <errno.h>

#define CODEC_ADDR		(0x11)	//codec addr
extern int i2c_write(u8 *buf, u8 len, u8 addr, int n);
extern void i2c_reset(void);

/*
*@reg:config reg addr
*@val:config value
*
*return 0: config reg success
*return 1: config reg time out
*/
static int config_codec_reg(u8 reg, u8 val)
{

	u8 data[2] = { reg, val};
	return i2c_write(data, sizeof(data), CODEC_ADDR, 1);
}

/*
* init codec
*/
int codec_init(int volume)
{
	int ret = 0;
	i2c_reset();
	volume = volume > 192 ? 192 : volume;
	ret |= config_codec_reg(0x00,0x3F); //IC Rst On
	mdelay(1); //Delay
	ret |= config_codec_reg(0x00,0x00); //IC Normal
	ret |= config_codec_reg(0x0C,0xFF); // Cal Time Set
	ret |= config_codec_reg(0x02,0x09); //Clock Set
	ret |= config_codec_reg(0x03,0x20); //adc Clock Set

	ret |= config_codec_reg(0x06,0x11); //dac Clock Set
	ret |= config_codec_reg(0x07,0x00); //dac Clock Set

	ret |= config_codec_reg(0x08,0x00); //Bclk Set
	ret |= config_codec_reg(0x09,0x04); //Cpclk Set
	ret |= config_codec_reg(0x01,0x7F); //:System Clock Set
	ret |= config_codec_reg(0x31,0x00); //dac Single Speed

	ret |= config_codec_reg(0x33,volume); //dacVolumeL set
	ret |= config_codec_reg(0x34,volume); //dacVolumeR set

	ret |= config_codec_reg(0x0B,0x0C); //:dac IIS Mode
	ret |= config_codec_reg(0x10,0x11); //:dac set
	ret |= config_codec_reg(0x0E,0x04); //:Lower Power Mode
	ret |= config_codec_reg(0x0F,0x00); //:Lower Power Mode
	ret |= config_codec_reg(0x2F,0x00); //:dac set
	ret |= config_codec_reg(0x13,0x00); //:Hpmix Set
	ret |= config_codec_reg(0x14,0x88); //:Hpmix Set
	ret |= config_codec_reg(0x15,0x88); //:Hpmix Set
	ret |= config_codec_reg(0x16,0xBB); //:Hpmix Set
	ret |= config_codec_reg(0x1A,0x10); //:Cphp Set
	ret |= config_codec_reg(0x1B,0x30); //:Cphp Set
	ret |= config_codec_reg(0x19,0x02); //:Cphp Set
	ret |= config_codec_reg(0x18,0x00); //:Cphp Set
	ret |= config_codec_reg(0x1E,0x90); //CAL set
	ret |= config_codec_reg(0x1F,0x90); //:CAL set
	ret |= config_codec_reg(0x1C,0x0F); //:CAL set
	mdelay(20);
	ret |= config_codec_reg(0x00,0xC0); //:Clk on
	mdelay(20);
	ret |= config_codec_reg(0x17,0x66); //:dac output
	return ret;
}

