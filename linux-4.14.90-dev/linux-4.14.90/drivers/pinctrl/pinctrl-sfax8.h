#ifndef __PAD_H__
#define __PAD_H__
#include <linux/types.h>

#ifdef CONFIG_SOC_SF16A18
#include <sf16a18.h>
#endif

#define GPIO_RDAT(n)		((n)*(0x40) + 0x0)
#define GPIO_WDAT(n)		((n)*(0x40) + 0x4)
#define GPIO_DIR(n)         ((n)*(0x40) + 0x8)

#define GPIO_INTMSK(n)		((n)*(0x40) + 0xC)
#define GPIO_INTGMSK(n)		((n)*(0x40) + 0x10)
#define GPIO_INTPEND(n)		((n)*(0x40) + 0x14)
#define GPIO_INTTYPE(n)		((n)*(0x40) + 0x18)
#define GPIO_FILTER(n)		((n)*(0x40) + 0x1C)
#define GPIO_CLKDIV(n)		((n)*(0x40) + 0x20)
#define GPIO_INTPENDGLB(n)	((n)*4 + 0x4000)

#define PAD_INDEX_INVALID 0xFF
#define PAD_IRQ_PER_GROUP 16

#ifdef CONFIG_SOC_SF19A28

#define PAD_INDEX_MAX 50

#define SW_DS0_REG          0
#define SW_DS1_REG          1
#define SW_DS2_REG          2
#define SW_PU_REG           3
#define SW_PD_REG           4
#define SW_IE_REG           5
#define SW_ST_REG           6
#define SW_OEN_REG          7
#define MODE_BIT0_REG       0
#define MODE_BIT1_REG       1
#define FMUX_SEL_REG        2
#define FUNC_SW_SEL_REG     3

#define PAD_INDEX_REG0(n)    (0xFC00 + (n)*(0x8))
#define PAD_INDEX_REG1(n)    (0xFC04 + (n)*(0x8))

#else /* !CONFIG_SOC_SF19A28 */

#define PAD_FUCN_SEL(n)     (n < 8 ? (0xF60 + (n)*(0x4)) : (0xB60 + (n-8)*(0x4)))
#define PAD_MODE_SEL(n)     (n < 16 ? (0xC00 + (n)*(0x4)) : (0x800 + (n-16)*(0x4)))
#define PAD_OFFSET          (4)
#define PAD_PER_GROUP_PINS  (4)

#ifdef CONFIG_SF16A18_MPW0
#define PAD_INDEX_MAX		62
#else
#define PAD_INDEX_MAX		70
#endif

#define PAD_IO_PULLUP(n)	(n < 8 ? (0xDE0 + (n)*(0x4)) : (0x9E0 + (n-8)*(0x4)))
#define PAD_IO_PULLDOWN(n)	(n < 8 ? (0xE40 + (n)*(0x4)) : (0xA40 + (n-8)*(0x4)))

#define FUNC_SW_SEL(n)		(n < 8 ? (0xC40 + (n)*(0x4)) : (0x840 + (n-8)*(0x4)))
#define FUNC_SW_OEN(n)		(n < 8 ? (0xC80 + (n)*(0x4)) : (0x880 + (n-8)*(0x4)))
#define FUNC_SW_IE(n)		(n < 8 ? (0xEA0 + (n)*(0x4)) : (0xAA0 + (n-8)*(0x4)))


#define PADSRANGE(a, b)	\
	((abs(b) << 16) | abs(a) | (a < 0 && b < 0 && a < b) ? 0x80000000 : 0)

#define PAD_INIT(_index, _name, _module, _func, _pull, _io, _level)	\
{                                       \
		.index	= _index,					\
		.pad_name = _name,					\
		.module	= _module,					\
		.func	= _func,					\
		.pull	= _pull,					\
		.io     = _io,                      \
		.level	= _level,					\
}

#endif /* CONFIG_SOC_SF19A28 */

typedef enum pad_trigger_type_t
{
	EINT_TRIGGER_LOW =0,
	EINT_TRIGGER_HIGH,
	EINT_TRIGGER_FALLING = 3,
	EINT_TRIGGER_RISING = 5,
	EINT_TRIGGER_DOUBLE = 7
} pad_trigger_type;


typedef enum pad_func_t{
	FUNC0,
	FUNC1,
	FUNC2,
	FUNC3,
	GPIO_INPUT,
	GPIO_OUTPUT
} pad_func;

typedef enum pad_io_t{
	INPUT,
	OUTPUT,
	TRISTATE
} pad_io;

typedef enum pad_pull_t{
	NOPULL,
	PULLUP,
	PULLDOWN
} pad_pull;

typedef enum pad_output_levet_t{
	LOW_LEVEL,
	HIGH_LEVEL,
} pad_output_level;

typedef enum pad_group_t{
	group0,
	group1,
	group2,
	group3
} pad_pad_group;


struct pad_init_info {
	int index;
	const char pad_name[16];
	const char module[16];
	pad_func func;
	pad_pull pull;
	pad_io io;
	int level;
};

typedef struct pad_index_t {
	u16 high;
	u16 low;
	pad_func func;
} pad_index;

typedef enum sf_module_t{
	SF_SPI0,
	SF_EMMC,
	SF_SDIO,
	SF_UART0,
	SF_I2C0,
	SF_I2C1,
	SF_I2C2,
	SF_SPI1,
	SF_I2S0,
	SF_PCM0,
	SF_UART1,
	SF_I2S1,
	SF_PCM1,
	SF_UART2,
	SF_SPDIF,
	SF_UART3,
	SF_PWM0,
	SF_PWM1,
	SF_RTC,
	SF_ETH_LED,
	SF_JTAG,
	SF_USB,
	SF_DDR,
	SF_RGMII,
	SF_RMII

} sf_module;


enum sfax8_fsel {
	SFAX8_FSEL_FUNC0 = 0,
	SFAX8_FSEL_FUNC1 = 1,
	SFAX8_FSEL_FUNC2 = 2,
	SFAX8_FSEL_FUNC3 = 3,
	SFAX8_FSEL_GPIO_IN = 4,
	SFAX8_FSEL_GPIO_OUT = 5,
	SFAX8_FSEL_COUNT = 6,
	SFAX8_FSEL_MASK = 0x7,
};

static const char * const sfax8_functions[SFAX8_FSEL_COUNT] = {
	[SFAX8_FSEL_FUNC0] = "func0",
	[SFAX8_FSEL_FUNC1] = "func1",
	[SFAX8_FSEL_FUNC2] = "func2",
	[SFAX8_FSEL_FUNC3] = "func3",
	[SFAX8_FSEL_GPIO_IN] = "gpio_in",
	[SFAX8_FSEL_GPIO_OUT] = "gpio_out",
};

#endif
