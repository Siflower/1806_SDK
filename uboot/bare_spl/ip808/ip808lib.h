#ifndef _IP808_H_
#define _IP808_H_

//typedef unsigned long u32;
//typedef unsigned short u16;
//typedef unsigned char u8;


struct IP808General
{
	unsigned short i2caddr;
	unsigned short regaddr;
	unsigned short regdata;
};


#define OK  0
#define ERROR (-1)


#define IP808IC_PORT_NUM	8
#define ENABLE 1
#define DISABLE 0
/********* IP808 Register **********/

#define PAGE_N_DEVICE_ADDR	0x0

#define IP808_V15			15
#define IP808_V14			14
/********* Page 0 *********/
//Analog Configure and Control
#define AF_AT_MODE				0x25

//Detection Result
#define R_DETECT_PORT0			0x68
//Classification Results
#define	CLASS_DETECT0			0x88
//Port Currents
#define CURRENT_MSB0			0xA0
#define CURRENT_LSB0			0xA1
//Port Temperatures
#define TEMP_MSB0				0xC0
#define TEMP_LSB0				0xC1
//Supply Voltage
#define SUPPLY_VOLTAGE_MSB		0xE0
#define SUPPLY_VOLTAGE_LSB		0xE1
//IVT Poll Control 
#define IVT_POLL_CONTROL		0xE3

/********* Page 1 *********/
//System Configuration & Control Registers
#define SYSTEM_CONFIG			0x01
#define SYSTEM_CONTROL			0x02
//LED Configuration
#define LED_CONFIG				0x08
#define LED_LOW_TIME_R			0x09
#define LED_HIGH_TIME_R			0x0A
#define LED_START_IDX			0x0B
//Power Management
#define POWER_MANAGE_MODE0		0x10
#define POWER_LIMIT0			0x18

//Power trunk Control & Configuration
#define TRUNK0_PL_MSB			0x40
#define TRUNK0_PL_LSB			0x41
#define TRUNK_SELECT			0x69


//Port Status
#define PORT_EVENT0			0x70

#define PORT_POWER_STATUS		0x82
//State Machine Control & Status
#define PORT_STATE_MCN_STAT0	0x90

#define PORT_PW_CONTROL0		0x98


/*********** Function***********/
int IP808_init(unsigned char id);


int i2c_write_reg(unsigned char id, unsigned char reg_num , unsigned char value);
int i2c_read_reg(unsigned char id, unsigned char reg_num, unsigned char *value);


#define IP808_REG_PAGE0		0x0
#define IP808_REG_PAGE1		0x1
int get_page(unsigned char id, unsigned char *page);
int set_page(unsigned char id, unsigned char page);

int get_device_address(unsigned char id, unsigned char *addr);

int get_op_mode(unsigned char id, unsigned char *mode);

int software_reset(unsigned char id);

int get_led_enable(unsigned char id, unsigned char *state);
int set_led_enable(unsigned char id, unsigned char state);

#define LED_OBVERSE			0x0
#define LED_REVERSE			0x1
int get_led_order(unsigned char id, unsigned char *order);
int set_led_order(unsigned char id, unsigned char order);

#define LED_LV_LOW			0x0
#define LED_LV_HIGH			0x1
int get_led_act_lv(unsigned char id, unsigned char *level);
int set_led_act_lv(unsigned char id, unsigned char level);

int get_led_init_lv(unsigned char id, unsigned char *level);
int set_led_init_lv(unsigned char id, unsigned char level);

#define LED_CLOCK_D16		0x0
#define LED_CLOCK_D8		0x1
int get_led_clock_rate(unsigned char id, unsigned char *rate);
int set_led_clock_rate(unsigned char id, unsigned char rate);

#define LED_MODE_SLAVE		0x0
#define LED_MODE_MASTER		0x1
int get_led_mode(unsigned char id, unsigned char *mode);
int set_led_mode(unsigned char id, unsigned char mode);

int get_led_start_idx(unsigned char id, unsigned char *index);
int set_led_start_idx(unsigned char id ,unsigned char index);

#define PW_MODE_CLASS	    0x1
#define PW_MODE_HIGHEST	  0x2
int get_power_mode(unsigned char id, unsigned char *mode);
int set_power_mode(unsigned char id, unsigned char select, unsigned char mode);

#define TRUNK_0		0x0
#define TRUNK_1		0x1
int get_trunk_power_limit(unsigned char id, unsigned char trunk, unsigned short *power);
int set_trunk_power_limit(unsigned char id, unsigned char trunk, unsigned short power);

int get_trunk_select(unsigned char id, unsigned char *trunk);
int set_trunk_select(unsigned char id, unsigned char trunk);

int get_port_event(unsigned char id, unsigned char port, unsigned char *event);
int clear_port_event(unsigned char id, unsigned char port);

int get_power_status(unsigned char id, unsigned char *status);

int start_state_machine(unsigned char id, unsigned char port);


#define STATE_DISABLE	0x0
#define STATE_ENABLE	0x1
int get_port_state(unsigned char id, unsigned char port, unsigned char *state);
int set_port_state(unsigned char id, unsigned char port, unsigned char state);

#define AF_MODE		0x0
#define AT_MODE		0x1
int get_af_at_mode(unsigned char id, unsigned char port, unsigned char *mode);
int set_af_at_mode(unsigned char id, unsigned char port, unsigned char mode);
int get_af_at_mode_all(unsigned char id, unsigned char *modes);
int set_af_at_mode_all(unsigned char id, unsigned char modes);

#define R_BAD	0x0
#define R_GOOD	0x1
#define R_OPEN	0x2
int get_dectect_sign(unsigned char id, unsigned char port, unsigned char *sign);

int get_pd_class(unsigned char id, unsigned char port, unsigned char *class_);
int get_port_current(unsigned char id, unsigned char port, unsigned short *current_);
int get_port_temperature(unsigned char id, unsigned char port, unsigned short *temp);
int get_supply_voltage(unsigned char id, unsigned short *voltage);

int get_ivt_auto_poll(unsigned char id, unsigned char *state);
int set_ivt_auto_poll(unsigned char id, unsigned char state);

int get_ivt_poll_period(unsigned char id, unsigned char *period);
int set_ivt_poll_period(unsigned char id, unsigned char period);

#endif
