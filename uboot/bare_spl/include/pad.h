#ifndef __PAD_H__
#define __PAD_H__
#include <types.h>

#define PAD_BASE            (SIFLOWER_SYSCTL_BASE+0x30000)
#define PAD_FUCN_SEL(n)        (PAD_BASE + (n < 8 ? (0xFF60 + (n)*(0x4)) : (0xFB60 + (n - 8)*(0x4))))
#define PAD_MODE_SEL(n)        (PAD_BASE + (n < 16 ? (0xFC00 + (n)*(0x4)) : (0xF800 + (n - 16) * (0x4))))
#define PAD_OFFSET          (4)
#define PAD_PER_GROUP_PINS  (4)
#define PAD_INDEX_INVALID 0xFF
#define PAD_IRQ_PER_GROUP 16
#ifdef SF19A28
#define SW_OEN		(1 << 7)
#define SW_ST		(1 << 6)
#define SW_IE		(1 << 5)
#define SW_PD		(1 << 4)
#define SW_PU		(1 << 3)
#define SW_DS2		(1 << 2)
#define	SW_DS1		(1 << 1)
#define SW_DS0		(1 << 0)
#define PAD_IO_BASE			(PAD_BASE + 0xFC00)
#define PAD_INDEX_REG0(n)    (PAD_BASE + 0xFC00 + (n)*(0x8))
#define PAD_IO_REG1(n)		(PAD_IO_BASE + 0x8*n)
#define PAD_IO_REG2(n)		(PAD_IO_BASE + 0x8*n + 0x4)
#define PAD_INDEX_MAX 48
#else
#define PAD_INDEX_MAX 70
#endif /* SF19A28 */

#define PAD_DS0_BASE(n)        (PAD_BASE + (n < 8 ? (0xFCC0 + (n)*(0x4)) : (0xF8C0 + (n - 8) * (0x4))))
#define PAD_DS1_BASE(n)        (PAD_BASE + (n < 8 ? (0xFD20 + (n)*(0x4)) : (0xF920 + (n - 8) * (0x4))))
#define PAD_DS2_BASE(n)        (PAD_BASE + (n < 8 ? (0xFD80 + (n)*(0x4)) : (0xF980 + (n - 8) * (0x4))))

#define PAD_IO_PULLUP(n)	(PAD_BASE + (n < 8 ? (0xFDE0 + (n)*(0x4)) : (0xF9E0 + (n - 8)*(0x4))))
#define PAD_IO_PULLDOWN(n)	(PAD_BASE + (n < 8 ? (0xFE40 + (n)*(0x4)) : (0xFA40 + (n - 8)*(0x4))))

#define GPIO_BASE		    (SIFLOWER_GPIO_BASE)
#define GPIO_RDAT(n)		(GPIO_BASE + (n)*(0x40) + 0x0)
#define GPIO_WDAT(n)		(GPIO_BASE + (n)*(0x40) + 0x4)
#define GPIO_DIR(n)         (GPIO_BASE + (n)*(0x40) + 0x8)

#define GPIO_INTMSK(n)		(GPIO_BASE + (n)*(0x40) + 0xC)
#define GPIO_INTGMSK(n)		(GPIO_BASE + (n)*(0x40) + 0x10)
#define GPIO_INTPEND(n)		(GPIO_BASE + (n)*(0x40) + 0x14)
#define GPIO_INTTYPE(n)		(GPIO_BASE + (n)*(0x40) + 0x18)
#define GPIO_FILTER(n)		(GPIO_BASE + (n)*(0x40) + 0x1C)
#define GPIO_CLKDIV(n)		(GPIO_BASE + (n)*(0x40) + 0x20)
#define GPIO_INTPENDGLB(n)	(GPIO_BASE + (n)*4 + 0x4000)

#ifdef SF19A28
#define FUNC_SW_SEL (1 << 3)
#define FMUX_SEL	(1 << 2)
#define MODE_BIT1	(1 << 1)
#define MODE_BIT0	(1 << 0)
#else
#define FUNC_SW_SEL(n)		(PAD_BASE + (n < 8 ? (0xFC40 + (n)*(0x4)) : 0xF840))
#endif /* SF19A28 */

#define FUNC_SW_OEN(n)		(PAD_BASE + (n < 8 ? (0xFC80 + (n)*(0x4)) : 0xF880))
#define FUNC_SW_IE(n)		(PAD_BASE + (n < 8 ? (0xFEA0 + (n)*(0x4)) : (0xFAA0 + (n - 8)*(0x4))))

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

typedef enum pad_driver_strength_t{
	//pad_0ma,
	pad_2ma,
	pad_4ma,
	pad_6ma,
	pad_8ma,
	pad_10ma,
	pad_12ma,
	pad_14ma,
	pad_16ma
} pad_driver_strength;

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
	SF_RMII,
	SF_RF,
	SF_CLK,
	SF_GDU,
	SF_SPI2,
	SF_ETH,
	SF_PWM,
	SF_PWM5,
	SF_CATIP,
	SF_CATIP_DEBUG

} sf_module;


extern int sf_pad_set_func(u32 index, pad_func func);
extern pad_func sf_pad_get_func(u32 index);
extern int sf_pad_set_pull(u32 index, pad_pull pull);
extern pad_pull sf_pad_get_pull(u32 index);
extern int sf_pad_set_value(u32 index, pad_output_level level);
extern u32 sf_pad_get_value(u32 index);
#ifdef SF19A28_FULLMASK
extern int sf_pad_set_ds(u32 index, u8 value);
#endif

extern int sf_pad_to_irq(u32 index);
extern int sf_irq_to_pad(u32 irq);
extern int sf_pad_irq_config(u32 index, pad_trigger_type trigger, u16 filter);
extern int sf_pad_irq_pending(u32 index);
extern int sf_pad_irq_clear(u32 index);
extern int sf_pad_irq_mask(u32 index, int mask);

extern int sf_module_set_pad_func(sf_module module);
extern void sf_init_pad_status(void);

extern int sf_sw_sel_set(u32 index , u8 value);
extern int sf_sw_oen_set(u32 index , u8 value);
#endif
