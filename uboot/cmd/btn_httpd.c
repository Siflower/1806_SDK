#include <common.h>
#include <command.h>
#include <mach/sfax8.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <errno.h>

#define CHECK_TIME_MS 2000

#ifdef CONFIG_ETH_LED
extern void request_eth_led(void);
#endif

#ifdef CONFIG_BTN_HTTPD_PMU

/* following is IP6103 info. */
#define PMU_ADDR		(0x30) //the slave addr to i2c for pmu ip6103
#define TEST_REG		(0x00) //test reg addr for pmu ip6103
#define PMU_RST_STATUS	(0x62)	//reset button status for pmu ip6103
#define PMU_RST_MASK	(1 << 5) // reset button status mask

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
#define I2C_TX_ABRT (0x80 + I2C_BASE_ADDR)    // I2c tx abrt source register
#define STATUS_RFNE (1 << 3)

/*
* i2c read interface.
*@addr: peripheral addr
*@reg: peripheral register
*@buf: where the data stroed in
*@len: the data length
*@n: then i2c controller number
*@return 0: success; 1:timeout
*/
static int i2c_read(u8 addr, u8 reg, u8 *buf, u8 len, int n)
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
static int i2c_write(u8 *buf, u8 len, u8 addr, int n)
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

static int check_pmu_reset_mode(void)
{
	int i = 0;
	u8 var = 0;
	u8 value[2] = {0};
	int ms = CHECK_TIME_MS;

	// close 8s press reboot.
	i2c_read(PMU_ADDR, 0x1, &var, 1, 0);
	var &= ~(1 << 7);
	value[0] = 0x1;
	value[1] = var;
	i2c_write(value, 2, 0x30, 0);

	// set long press time as 4s.
	i2c_read(PMU_ADDR, 0x2, &var, 1, 0);
	var |= ((1 << 7) | (1 << 6));
	value[0] = 0x2;
	value[1] = var;
	i2c_write(value, 2, 0x30, 0);

	// enable long/short press irq.
	i2c_read(PMU_ADDR, 0x60, &var, 1, 0);
	var |= (1 << 1) | (1 << 2);
	value[0] = 0x60;
	value[1] = var;
	i2c_write(value, 2, 0x30, 0);

	// check press first
	i2c_read(PMU_ADDR, 0x62, &var, 1, 0);
	if (!(var & (1 << 5)))
		return 0;

	while (1) {
		// check long press
		i2c_read(PMU_ADDR, 0x62, &var, 1, 0);
		if (var & (1 << 2))
			return 1;
		mdelay(50);
		i++;
		if (i >= ms / 50)
			break;
	}
	return 0;
}

static void pmu_led_init(void)
{
	int led_gpio = 55;
	//enable all eth led on
	for(led_gpio = 55; led_gpio <= 59; led_gpio++){
		gpio_request(led_gpio, "sf_gpio");
		gpio_direction_output(led_gpio, 0);
	}
}

static void pmu_btn_exit(void)
{
#ifdef CONFIG_ETH_LED
	request_eth_led();
#else
	//todo: maybe we shoud restore jtag config
#endif
}

#endif


#ifdef CONFIG_BTN_HTTPD_GPIO
static int g_btn_gpio = CONFIG_BTN_HTTPD_GPIO_VAL;
static int check_gpio_reset_mode(void)
{
	int val = 0;
	int i = 0;
	int cnt = 0;
	gpio_request(g_btn_gpio, "btn_gpio");
	gpio_direction_input(g_btn_gpio);
	for(i = 0 ; i< 5; i++)
	{
		val = gpio_get_value(g_btn_gpio);
		if(val == 0) cnt++;
		mdelay(10);
	}
	if(cnt > 3){
		printf("check_gpio_reset_mode success---gpio=%d cnt=%d\n",g_btn_gpio,cnt);
		return 1;
	}
	return 0;
}

static void gpio_led_init(void)
{
	int led_gpio = 55;
	//enable all eth led on
	for(led_gpio = 55; led_gpio <= 59; led_gpio++){
		gpio_request(led_gpio, "sf_gpio");
		gpio_direction_output(led_gpio, 0);
	}
}

static void gpio_btn_exit(void)
{
#ifdef CONFIG_ETH_LED
	request_eth_led();
#else
	//todo: maybe we shoud restore jtag config
#endif
}
#endif

struct in_addr btn_net_httpd_ip;

struct btn_httpd_ops{
	int (*btn_enter_detect)(void);
	void (*btn_led_init)(void);
	void (*btn_exit)(void);
};


static const struct btn_httpd_ops g_btn_httpd_ops = {
#ifdef CONFIG_BTN_HTTPD_PMU
	.btn_enter_detect = check_pmu_reset_mode,
	.btn_led_init = pmu_led_init,
	.btn_exit = pmu_btn_exit
#else
#ifdef CONFIG_BTN_HTTPD_GPIO
	//config by gpio
	.btn_enter_detect = check_gpio_reset_mode,
	.btn_led_init = gpio_led_init,
	.btn_exit = gpio_btn_exit
#else
	.btn_enter_detect = NULL,
	.btn_led_init = NULL,
	.btn_exit = NULL
#endif
#endif
};

static int
check_btn_httpd(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	char httpdcmd[64];
	int ret = 0;
	if (argc < 2)
		return CMD_RET_USAGE;

	btn_net_httpd_ip = string_to_ip(argv[1]);
	if (btn_net_httpd_ip.s_addr == 0)
		return CMD_RET_USAGE;
	if(g_btn_httpd_ops.btn_enter_detect == NULL || g_btn_httpd_ops.btn_led_init == NULL) return 0;
	if (g_btn_httpd_ops.btn_enter_detect()) {
		g_btn_httpd_ops.btn_led_init();
		sprintf(httpdcmd,"httpd %s",argv[1]);
		ret = run_command_list(httpdcmd, -1, 0);
		if(g_btn_httpd_ops.btn_exit) g_btn_httpd_ops.btn_exit();
		return ret;
	} else
		return 0;
}

U_BOOT_CMD(btn_httpd_detect,
		CONFIG_SYS_MAXARGS,
		1,
		check_btn_httpd,
		"check pmu buttom to run httpd",
		"btn_httpd_detect IP")
