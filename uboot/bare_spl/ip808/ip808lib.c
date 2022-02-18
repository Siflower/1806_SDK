#include "ip808.h"
#include "ip808lib.h"
#include <common.h>
#include <stdio.h>


int i2c_read_reg(unsigned char id, unsigned char reg_num, unsigned char *value)
{
	return PoE_i2c_read(id,reg_num,value);
}
int i2c_write_reg(unsigned char id, unsigned char reg_num, unsigned char value)
{
  return PoE_i2c_write(id,reg_num,value);  
}



//**************************************************
//            API Function
//**************************************************

/*In manual mode, user should call  IP808_init function， 
 Then IP808 able to start suppling power*/
int IP808_init(unsigned char id)
{
	int i,ret=0;
	if(set_page(id,IP808_REG_PAGE0)==ERROR)
		return ERROR;
		
	// set IP808 initial value
	if(i2c_write_reg(id,0x54,0x7A)==ERROR || i2c_write_reg(id,0x55,0x72)==ERROR)
	 return ERROR;
	
	// set AF or AT mode
	if((ret=set_af_at_mode_all(id, AT_MODE))==ERROR)
		return ERROR;
	// enable ivt poll function
	if((ret=set_ivt_auto_poll(id, ENABLE))==ERROR)
		goto IP808_init_out;
			
	//start per port state machine
	for(i=0;i<IP808IC_PORT_NUM;i++)
	{
		if((ret=start_state_machine(id,i))==ERROR)
			goto IP808_init_out;
	}
	//power on each port
	for(i=0;i<IP808IC_PORT_NUM;i++)
	{
		if((ret=set_port_state(id, i, STATE_ENABLE))==ERROR)
			goto IP808_init_out;
	}	
	IP808_init_out:
	if(ret==ERROR)
#ifdef USER_API
		printf("IP808 id[%x] init fail!\n",id);
#else
		printf("IP808 id[%x] init fail!\n",id);
#endif
	else
#ifdef USER_API
		printf("IP808 id[%x] init success!\n",id);
#else
		printf("IP808 id[%x] init success!\n",id);
#endif	
	return ret;
}


int set_page(unsigned char id, unsigned char page)
{
	unsigned char tmp;
	if(i2c_read_reg(id,PAGE_N_DEVICE_ADDR,&tmp)==ERROR)
	 return ERROR;
	tmp &= 0xbf;
	return (i2c_write_reg(id, PAGE_N_DEVICE_ADDR, tmp|((page&0x1)<<6)));
}

int get_page(unsigned char id, unsigned char *page)
{
  unsigned char tmp;
	if(i2c_read_reg(id,PAGE_N_DEVICE_ADDR,&tmp)==ERROR)
	 return ERROR;  
  *page = (tmp>>6)&0x1;
	return OK;
}

int get_device_address(unsigned char id, unsigned char *addr)
{
  unsigned char tmp;
	if(i2c_read_reg(id,PAGE_N_DEVICE_ADDR,&tmp)==ERROR)
	 return ERROR;  
  *addr = (tmp&0x7);
	return OK;	
}

int get_op_mode(unsigned char id, unsigned char *mode)
{   
  unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
  if(i2c_read_reg(id, SYSTEM_CONFIG,&tmp)==ERROR)
    return ERROR;
  *mode = (tmp>>6)&0x3;    	
	return OK;
}

int software_reset(unsigned char id)
{
	printf("--Mark : enter software reset!\n");
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	return i2c_write_reg(id, SYSTEM_CONTROL, 0x1);
}

int get_led_enable(unsigned char id, unsigned char *state)
{
  unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	if(i2c_read_reg(id,LED_CONFIG, &tmp)==ERROR)
    return ERROR;  
	*state = (tmp>>7)&0x1; 
    return OK;	
}

int set_led_enable(unsigned char id, unsigned char state)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	if(i2c_read_reg(id, LED_CONFIG, &tmp)==ERROR)
    return ERROR;
	if(state)
		tmp |= 0x80;
	else
		tmp &= 0x7F;
	return i2c_write_reg(id, LED_CONFIG, tmp);
}

int get_led_order(unsigned char id, unsigned char *order)
{
  unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	if(i2c_read_reg(id, LED_CONFIG, &tmp)==ERROR)
    return ERROR;
  *order = (tmp>>4)&0x1;    
	return OK;
}

int set_led_order(unsigned char id, unsigned char order)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	if(i2c_read_reg(id, LED_CONFIG, &tmp)==ERROR)
    return ERROR;
	if(order)
		tmp |= 0x10;
	else
		tmp &= 0xEF;
	return i2c_write_reg(id, LED_CONFIG, tmp);
}

int get_led_act_lv(unsigned char id, unsigned char *level)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	if(i2c_read_reg(id, LED_CONFIG, &tmp)==ERROR)
    return ERROR;    
	*level = ((tmp)>>3)&0x1;
	  return OK;	
}

int set_led_act_lv(unsigned char id, unsigned char level)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	if(i2c_read_reg(id, LED_CONFIG, &tmp)==ERROR)
    return ERROR;   
	if(level)
		tmp |= 0x08;
	else
		tmp &= 0xF7;
	return i2c_write_reg(id, LED_CONFIG, tmp);
}

int get_led_init_lv(unsigned char id, unsigned char *level)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	if(i2c_read_reg(id, LED_CONFIG, &tmp)==ERROR)
    return ERROR;       
	*level = (tmp>>2)&0x1;
	return OK;	
}

int set_led_init_lv(unsigned char id, unsigned char level)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	if(i2c_read_reg(id, LED_CONFIG, &tmp)==ERROR)
    return ERROR;  
	if(level)
		tmp |= 0x04;
	else
		tmp &= 0xFB;
	return i2c_write_reg(id, LED_CONFIG, tmp);
}

int get_led_clock_rate(unsigned char id, unsigned char *rate)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	if(i2c_read_reg(id, LED_CONFIG, &tmp)==ERROR)
    return ERROR;  
	*rate = (tmp>>1)&0x1;
	return OK;
}

int set_led_clock_rate(unsigned char id, unsigned char rate)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	if(i2c_read_reg(id, LED_CONFIG, &tmp)==ERROR)
    return ERROR;  
	if(rate)
		tmp |= 0x02;
	else
		tmp &= 0xFD;
	return i2c_write_reg(id, LED_CONFIG, tmp);
}

int get_led_mode(unsigned char id, unsigned char *mode)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	if(i2c_read_reg(id, LED_CONFIG, &tmp)==ERROR)
    return ERROR;
	*mode = tmp&0x1;
	return OK;	
}

int set_led_mode(unsigned char id, unsigned char mode)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	if(i2c_read_reg(id, LED_CONFIG, &tmp)==ERROR)
    return ERROR;
	if(mode)
		tmp |= 0x01;
	else
		tmp &= 0xFE;
	return i2c_write_reg(id, LED_CONFIG, tmp);
}

int get_led_start_idx(unsigned char id, unsigned char *index)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	if(i2c_read_reg(id, LED_START_IDX, &tmp)==ERROR)
    return ERROR;
	*index = tmp&0x3F;
	return OK;	
}

int set_led_start_idx(unsigned char id ,unsigned char index)
{
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	return i2c_write_reg(id, LED_START_IDX, index&0x3F);
}

int get_power_mode(unsigned char id, unsigned char *mode)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
  if(i2c_read_reg(id, POWER_MANAGE_MODE0, &tmp)==ERROR)
    return ERROR;
  *mode = (tmp>>3)&0x3;
	return OK;
}

int set_power_mode(unsigned char id, unsigned char select, unsigned char mode)
{
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	return i2c_write_reg(id, POWER_MANAGE_MODE0, (mode%0x3) << 3);
}

int get_trunk_power_limit(unsigned char id, unsigned char trunk, unsigned short *power)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	if(i2c_read_reg(id, TRUNK0_PL_MSB+(trunk%2), &tmp)==ERROR)
    return ERROR;
  *power = tmp;
  *power <<= 8;  	 
	if(i2c_read_reg(id, TRUNK0_PL_LSB+(trunk%2), &tmp)==ERROR)
    return ERROR;
	*power |= tmp;
	return OK;
}

int set_trunk_power_limit(unsigned char id, unsigned char trunk, unsigned short power)
{
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	if(i2c_write_reg(id, TRUNK0_PL_MSB+(trunk%2), (power>>8)&0x7)==ERROR ||
	i2c_write_reg(id, TRUNK0_PL_LSB+(trunk%2), power&0xFF)==ERROR)
    return ERROR;
  else
    return OK;
}

int get_trunk_select(unsigned char id, unsigned char *trunk)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
  if(i2c_read_reg(id, TRUNK_SELECT, &tmp)==ERROR)   
    return ERROR; 
  *trunk = tmp&0x3;
	return OK;
}

int set_trunk_select(unsigned char id, unsigned char trunk)
{
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	return i2c_write_reg(id, TRUNK_SELECT, trunk%2);
}

int get_port_event(unsigned char id, unsigned char port, unsigned char *event)
{
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	return i2c_read_reg(id,PORT_EVENT0+(port%8), event);
}
	
int clear_port_event(unsigned char id, unsigned char port)
{
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	return i2c_write_reg(id, PORT_EVENT0+(port%8), 0xFF);
}

int get_power_status(unsigned char id, unsigned char *status)
{
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	return i2c_read_reg(id, PORT_POWER_STATUS, status);
}

int start_state_machine(unsigned char id, unsigned char port)
{
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	return i2c_write_reg(id, PORT_STATE_MCN_STAT0+(port%8), 0x80);
}

int get_port_state(unsigned char id, unsigned char port, unsigned char *state)
{
  unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
  if(i2c_read_reg(id, PORT_PW_CONTROL0+(port%8), &tmp)==ERROR)
    return ERROR;
  *state = tmp&0x3;
	return OK;
}

int set_port_state(unsigned char id, unsigned char port, unsigned char state)
{
	if(set_page(id,IP808_REG_PAGE1)==ERROR)
    return ERROR;
	return i2c_write_reg(id, PORT_PW_CONTROL0+(port%8), state&0x3);
}

int get_af_at_mode(unsigned char id, unsigned char port, unsigned char *mode)
{
  unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE0)==ERROR)
    return ERROR;
  if(i2c_read_reg(id, AF_AT_MODE, &tmp)==ERROR)	
    return ERROR;
	*mode = (tmp>>(port%8))&0x1;
	return OK;	
}

int set_af_at_mode(unsigned char id, unsigned char port, unsigned char mode)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE0)==ERROR)
    return ERROR;
  if(i2c_read_reg(id, AF_AT_MODE, &tmp)==ERROR)	
    return ERROR;
	if(mode)
		tmp |= (0x1<<(port%8));
	else
		tmp &= ~(0x1<<(port%8));
	return i2c_write_reg(id, AF_AT_MODE, tmp);
}

int get_af_at_mode_all(unsigned char id, unsigned char *modes)
{
	if(set_page(id,IP808_REG_PAGE0)==ERROR)
    return ERROR;
	return i2c_read_reg(id, AF_AT_MODE, modes);
}

int set_af_at_mode_all(unsigned char id, unsigned char modes)
{
	if(set_page(id,IP808_REG_PAGE0)==ERROR)
		return ERROR;
	if(modes == AF_MODE)
		return i2c_write_reg(id, AF_AT_MODE, 0x0);
	else
		return i2c_write_reg(id, AF_AT_MODE, 0xFF);		
}

int get_dectect_sign(unsigned char id, unsigned char port, unsigned char *sign)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE0)==ERROR)
    return ERROR;
  if(i2c_read_reg(id, R_DETECT_PORT0+(port%8), &tmp)==ERROR)
    return ERROR;
	*sign =  tmp&0x3;
	return OK;	
}

int get_pd_class(unsigned char id, unsigned char port, unsigned char *class_)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE0)==ERROR)
    return ERROR;
  if(i2c_read_reg(id, CLASS_DETECT0+((port%8)/2), &tmp)==ERROR)
    return ERROR;  
	*class_ = (tmp>>((port%2)*4))&0x7;
	return OK;	
}

int get_port_current(unsigned char id, unsigned char port, unsigned short *current_)
{
	unsigned char tmp;
	
	if(get_power_status(id, &tmp)==ERROR)
		return ERROR;
	//check power on status
	if(!((tmp>>port)&0x1))//power off
	{	*current_ =0;	return OK;	}
	
	if(set_page(id,IP808_REG_PAGE0)==ERROR)
		return ERROR;
	if(i2c_read_reg(id, CURRENT_MSB0+((port%8)*2), &tmp)==ERROR)
		return ERROR;
	*current_ = tmp&0xF;
	*current_ <<= 8;
	if(i2c_read_reg(id, CURRENT_LSB0+((port%8)*2), &tmp)==ERROR)
		return ERROR;    
	*current_ |= tmp;
	return OK;
}

int get_port_temperature(unsigned char id, unsigned char port, unsigned short *temp)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE0)==ERROR)
    return ERROR;
  if(i2c_read_reg(id, TEMP_MSB0+((port%8)*2), &tmp)==ERROR)
    return ERROR;
  *temp = tmp&0xF;
  *temp <<= 8;
  if(i2c_read_reg(id, TEMP_LSB0+((port%8)*2), &tmp)==ERROR)
    return ERROR;    
  *temp |= tmp;
	return OK;
}

int get_supply_voltage(unsigned char id, unsigned short *voltage)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE0)==ERROR)
    return ERROR;
  if(i2c_read_reg(id, SUPPLY_VOLTAGE_MSB, &tmp)==ERROR)
    return ERROR;
  *voltage = tmp&0xF;
  *voltage <<= 8;
  if(i2c_read_reg(id, SUPPLY_VOLTAGE_LSB, &tmp)==ERROR)
    return ERROR;    
  *voltage |= tmp;
	return OK;
}

int get_ivt_auto_poll(unsigned char id, unsigned char *state)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE0)==ERROR)
    return ERROR;
  if(i2c_read_reg(id, IVT_POLL_CONTROL, &tmp)==ERROR)
    return ERROR;
  *state = (tmp>>5)&0x1;
	return OK;
}

int set_ivt_auto_poll(unsigned char id, unsigned char state)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE0)==ERROR)
    return ERROR;
  if(i2c_read_reg(id, IVT_POLL_CONTROL, &tmp)==ERROR)
    return ERROR;
	if(state)
		tmp |= 0x20;
	else
		tmp &= 0xDF;
	return i2c_write_reg(id, IVT_POLL_CONTROL, tmp);
}

int get_ivt_poll_period(unsigned char id, unsigned char *period)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE0)==ERROR)
    return ERROR;
  if(i2c_read_reg(id, IVT_POLL_CONTROL, &tmp))  
    return ERROR;
  *period = tmp&0x1F;
	return OK;
}

int set_ivt_poll_period(unsigned char id, unsigned char period)
{
	unsigned char tmp;
	if(set_page(id,IP808_REG_PAGE0)==ERROR)
    return ERROR;
  if(i2c_read_reg(id, IVT_POLL_CONTROL, &tmp))  
    return ERROR;
	period &= 0x1F;
	if(period<10)
		period = 10;
	tmp &= ~0x1F;
	tmp |= period;
	return i2c_write_reg(id, IVT_POLL_CONTROL, tmp);
}


