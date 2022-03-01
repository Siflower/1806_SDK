#ifndef __SFAX8_DEBUG_H
#define __SFAX8_DEBUG_H

#ifdef SF_DEBUG
#include <mach/sfax8.h>

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
#define I2C_IGNORE_ACK (0xA0 + I2C_BASE_ADDR) // I2C enable status register
#define STATUS_RFNE (1 << 3)

static void i2c_reset(void)
{

	writew(0x0, (void *)(IIC_SYSM_RESET));
	writew(0xff, (void *)(IIC_SYSM_RESET));

	writew(0xFF, (void *)(IIC_SOFT_CLK_EN));

	writew(0xFF, (void *)(IIC_SOFT_RESET));
	writew(0x00, (void *)(IIC_SOFT_RESET));
	writew(0x0F, (void *)(IIC_SOFT_BOE));
}
void pm_i2c_write(u8 *buf, u8 len)
{
	u32 i = 0;

	// disable ctrl
	writew(0x0, (void *)I2C_ENABLE);

	// config ctrl
	writew(0x6b, (void *)I2C_CON);

	// set speed
	writew(0x3d7, (void *)I2C_SS_SCL_HCNT);
	writew(0x3dd, (void *)I2C_SS_SCL_LCNT);

	// set target
	writew(0x30, (void *)I2C_TAR);

	// no ack
	writew(0x102, (void *)I2C_IGNORE_ACK);

	// enable ctrl
	writew(0x1, (void *)I2C_ENABLE);

	// write data
	while (i < len) {
		writew(buf[i], (void *)I2C_DATA_CMD);
		i++;
	}

	while (!(readw((void *)I2C_STATUS) & 0x1))
		; // wait inactive
}

static void pm_i2c_read(u8 reg, u8 *buf, u8 len)
{
	u32 i = 0;
	pm_i2c_write(&reg, 1);

	while (i < len) {
		writew(0x100, (void *)I2C_DATA_CMD);
		i++;
	}

	i = 0;
	while (i < len) {
		while (!(readw((void *)I2C_STATUS) & STATUS_RFNE))
			; // wait rx fifo not empty
		buf[i] = (u8)readw((void *)I2C_DATA_CMD);
		i++;
	}

	while (!(readw((void *)I2C_STATUS) & 0x1))
		; // wait inactive
}

u32 read_rtc_time(void)
{
	u32 time = 0;
	u8 buf[3] = {0};

	i2c_reset();
	// read date
	// pm_i2c_read(0x7a, buf, 1);
	// time = buf[0];

	// read hour
	pm_i2c_read(0x79, buf, 1);
	time = (time << 8) | buf[0];

	// read minute
	pm_i2c_read(0x78, buf, 1);
	time = (time << 8) | buf[0];

	// read second
	pm_i2c_read(0x77, buf, 1);
	time = (time << 8) | buf[0];

	return time;
}

void sys_pmu_reboot(void)
{
	u8 buf[3] = {0};

	printf("Running %s() ...\n", __func__);
	i2c_reset();

	pm_i2c_read(0x21, buf, 1);

	buf[0] = 0x1;  // reg
	buf[1] = 0xbd; // value
	pm_i2c_write(buf, 2);
}

#define PAD_BASE (SIFLOWER_SYSCTL_BASE + 0x30000)
#define PAD_FUCN_SEL (PAD_BASE + 0xFF60)

#define GPIO_BASE (0xB9D00000)
#define GPIO_RDAT(n) (GPIO_BASE + (n) * (0x40) + 0x0)
#define GPIO_WDAT(n) (GPIO_BASE + (n) * (0x40) + 0x4)
#define GPIO_DIR(n) (GPIO_BASE + (n) * (0x40) + 0x8)

void pin_set_value(u32 gpio, u32 lh)
{
	u8 tmp;
	int fun_cnt = gpio / 8;
	int fun_num = gpio % 8;

	tmp = readb((void *)(PAD_FUCN_SEL + fun_cnt * 4));
	tmp |= 1 << fun_num;
	writeb(tmp, (void *)(PAD_FUCN_SEL + fun_cnt * 4));

	writeb(0, (void *)(GPIO_DIR(gpio)));
	writeb(lh, (void *)(GPIO_WDAT(gpio)));
}

void led_onoff(u32 onoff)
{
	if (onoff) {
		pin_set_value(61, 0);
		pin_set_value(36, 0);
	} else {
		pin_set_value(61, 1);
		pin_set_value(36, 1);
	}
}

#define SIFLOWER_UART_BASE 0xB8300000
#define UART_DR (SIFLOWER_UART_BASE + 0x00)   // Data Register
#define UART_IBRD (SIFLOWER_UART_BASE + 0x24) // Integer Baud Rate Register
#define UART_FBRD (SIFLOWER_UART_BASE + 0x28) // Fractional Baud Rate Register
#define UART_LCRH (SIFLOWER_UART_BASE + 0x2c) // Line Control Register
#define UART_CR (SIFLOWER_UART_BASE + 0x30)   // Control Register
#define UART_FR (SIFLOWER_UART_BASE + 0x18)

void printz(char *s)
{
	volatile int init_flag = 0;
	volatile u8 *p = (volatile u8 *)s;

	if (init_flag == 0) {
		// uart enable
		writel(0xf01, (void *)UART_CR);

		// set Baud Rate 115200
		writel(0x3a, (void *)UART_IBRD);
		writel(0x26, (void *)UART_FBRD);

		writel(0x70, (void *)UART_LCRH);
		init_flag = 1;
	}

	while (*p != '\0') {
		writel(*p++, (void *)UART_DR);
	}

	writel('\r', (void *)UART_DR);
	writel('\n', (void *)UART_DR);
	while (!(readl((void *)UART_FR) & (1 << 7)))
		;
}
#else
u32 read_rtc_time(void) { return 0; }

void sys_pmu_reboot(void) {}

void led_onoff(u32 onoff) {}

void printz(char *s) {}
#endif

#endif
