#include <types.h>
#include <io.h>
#include <errorno.h>
#include <sf_mmap.h>
#include <pad.h>

/**************************************************************************************************
*                                                                                                 *
* -Description:                                                                                   *
*    This part is used to set pin output level.                                                   *
*                                                                                                 *
***************************************************************************************************/
int sf_pad_set_value(u32 index, pad_output_level level)
{
	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	writeb(level, GPIO_WDAT(index));

	return 0;
}

#ifdef SF19A28_MPW0
static void module_analysis(sf_module module, pad_index *pi)
{
	switch(module)
	{
		case SF_JTAG:
		pi->low = 0;
		pi->high = 4;
		pi->func = FUNC0;
		break;

		case SF_ETH:
		pi->low = 0;
		pi->high = 2;
		pi->func = FUNC1;
		break;

		case SF_PWM:
		pi->low = 3;
		pi->high = 4;
		pi->func = FUNC1;
		break;

		case SF_SPI2:
		pi->low = 5;
		pi->high = 8;
		pi->func = FUNC0;
		break;

		case SF_UART1:
		pi->low = 5;
		pi->high = 6;
		pi->func = FUNC1;
		break;

		case SF_I2C1:
		pi->low = 7;
		pi->high = 8;
		pi->func = FUNC1;
		break;

		case SF_UART0:
		pi->low = 9;
		pi->high = 10;
		pi->func = FUNC0;
		break;

		case SF_I2C0:
		pi->low = 11;
		pi->high = 12;
		pi->func = FUNC0;
		break;

		case SF_SPI1:
		pi->low = 9;
		pi->high = 12;
		pi->func = FUNC1;
		break;

		default:
		pi->low = pi->high = 0xff;
		break;
	}
}

int sf_sw_oen_set(u32 index , u8 value)
{
    u8 tmp;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

    switch(value)
    {
        case 1:
        tmp = readb(PAD_IO_REG1(index));
        tmp |= SW_OEN;
        writeb(tmp, PAD_IO_REG1(index));
        break;

        case 0:
        tmp = readb(PAD_IO_REG1(index));
        tmp &= ~SW_OEN;
        writeb(tmp, PAD_IO_REG1(index));
        break;

        default:
        return -EINVAL;
        break;
    }

    return 0;
}

int sf_sw_ie_set(u32 index , u8 value)
{
    u8 tmp;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

    switch(value)
    {
        case 1:
        tmp = readb(PAD_IO_REG1(index));
        tmp |= SW_IE;
        writeb(tmp, PAD_IO_REG1(index));
        break;

        case 0:
        tmp = readb(PAD_IO_REG1(index));
        tmp &= ~SW_IE;
        writeb(tmp, PAD_IO_REG1(index));
        break;

        default:
        return -EINVAL;
        break;
    }

    return 0;
}

int sf_pad_set_func(u32 index, pad_func func)
{
	u8 tmp;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	switch (func)
	{
		case FUNC0:
		tmp = readb(PAD_IO_REG1(index));
		tmp |= SW_IE;
		writeb(tmp, PAD_IO_REG1(index));

		tmp = readb(PAD_IO_REG2(index));
		tmp |= FUNC_SW_SEL;
		tmp &= ~(FMUX_SEL | MODE_BIT0 | MODE_BIT1);
		writeb(tmp, PAD_IO_REG2(index));
		break;

		case FUNC1:
		tmp = readb(PAD_IO_REG1(index));
		tmp |= SW_IE;
		writeb(tmp, PAD_IO_REG1(index));

		tmp = readb(PAD_IO_REG2(index));
		tmp |= (FUNC_SW_SEL | MODE_BIT0);
		tmp &= ~(FMUX_SEL | MODE_BIT1);
		writeb(tmp, PAD_IO_REG2(index));
		break;

		case GPIO_INPUT:
		tmp = readb(PAD_IO_REG1(index));
		tmp |= (SW_IE | SW_OEN);
		writeb(tmp, PAD_IO_REG1(index));

		tmp = readb(PAD_IO_REG2(index));
		tmp |= (FUNC_SW_SEL | FMUX_SEL);
		writeb(tmp, PAD_IO_REG2(index));

		writeb(1, GPIO_DIR(index));
		break;

		case GPIO_OUTPUT:
		tmp = readb(PAD_IO_REG1(index));
		tmp &= ~(SW_IE | SW_OEN);
		writeb(tmp, PAD_IO_REG1(index));

		tmp = readb(PAD_IO_REG2(index));
		tmp |= (FUNC_SW_SEL | FMUX_SEL);
		writeb(tmp, PAD_IO_REG2(index));

		writeb(0, GPIO_DIR(index));
		break;

		default:
		return -EINVAL;
		break;
	}

	return 0;
}

u32 sf_pad_get_value(u32 index)
{
	u8 tmp;
	int level = 0;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	tmp = readb(PAD_IO_REG2(index)) & FMUX_SEL;
	if (tmp)
	{
		tmp = readb(GPIO_DIR(index));
		if (tmp)
			level = readb(GPIO_RDAT(index));
		else
			level = readb(GPIO_WDAT(index));
	}

	return level;
}

int sf_module_set_pad_func(sf_module module)
{
	int index;
	pad_index pad_range;

	module_analysis(module,&pad_range);

	if (pad_range.high > PAD_INDEX_MAX || pad_range.low > PAD_INDEX_MAX)
		return -EINVAL;

	for(index = pad_range.low; index <= pad_range.high; index++)
		sf_pad_set_func(index, pad_range.func);

	return 0;
}

int sf_sw_sel_set(u32 index , u8 value)
{
	u8 tmp;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	switch(value)
	{
		case 1:
			tmp = readb(PAD_IO_REG2(index));
			tmp |= (FUNC_SW_SEL);
			writeb(tmp, PAD_IO_REG2(index));
			break;

		case 0:
			tmp = readb(PAD_IO_REG2(index));
			tmp &= ~(FUNC_SW_SEL);
			writeb(tmp, PAD_IO_REG2(index));
			break;

		default:
			return -EINVAL;
			break;
	}

	return 0;
}
#elif defined(SF19A28_FULLMASK)
static void module_analysis(sf_module module, pad_index *pi)
{
	switch(module)
	{
		case SF_JTAG:
		pi->low = 0;
		pi->high = 4;
		pi->func = FUNC0;
		break;

		case SF_ETH:
		pi->low = 0;
		pi->high = 2;
		pi->func = FUNC2;
		break;

		case SF_PWM0:
		pi->low = 3;
		pi->high = 3;
		pi->func = FUNC3;
		break;

		case SF_PWM5:
		pi->low = 4;
		pi->high = 4;
		pi->func = FUNC2;
		break;

		case SF_SPI2:
		pi->low = 5;
		pi->high = 8;
		pi->func = FUNC0;
		break;

		case SF_UART0:
		pi->low = 9;
		pi->high = 12;
		pi->func = FUNC0;
		break;

		case SF_UART1:
		pi->low = 0;
		pi->high = 3;
		pi->func = FUNC1;
		break;

		case SF_UART2:
		pi->low = 11;
		pi->high = 12;
		pi->func = FUNC1;
		break;

		case SF_I2C0:
		pi->low = 11;
		pi->high = 12;
		pi->func = FUNC2;
		break;

		case SF_RGMII:
		pi->low = 13;
		pi->high = 29;
		pi->func = FUNC0;
		break;

		case SF_RF:
		pi->low = 30;
		pi->high = 45;
		pi->func = FUNC0;
		break;

		case SF_CATIP_DEBUG:
		pi->low = 30;
		pi->high = 45;
		pi->func = FUNC3;
		break;

		case SF_CLK:
		pi->low = 46;
		pi->high = 47;
		pi->func = FUNC0;
		break;

		case SF_USB:
		pi->low = 48;
		pi->high = 48;
		pi->func = FUNC0;
		break;

		default:
		pi->low = pi->high = 0xff;
		break;
	}
}

int sf_sw_oen_set(u32 index , u8 value)
{
    u8 tmp;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

    switch(value)
    {
        case 1:
        tmp = readb(PAD_IO_REG1(index));
        tmp |= SW_OEN;
        writeb(tmp, PAD_IO_REG1(index));
        break;

        case 0:
        tmp = readb(PAD_IO_REG1(index));
        tmp &= ~SW_OEN;
        writeb(tmp, PAD_IO_REG1(index));
        break;

        default:
        return -EINVAL;
        break;
    }

    return 0;
}

int sf_sw_ie_set(u32 index , u8 value)
{
    u8 tmp;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

    switch(value)
    {
        case 1:
        tmp = readb(PAD_IO_REG1(index));
        tmp |= SW_IE;
        writeb(tmp, PAD_IO_REG1(index));
        break;

        case 0:
        tmp = readb(PAD_IO_REG1(index));
        tmp &= ~SW_IE;
        writeb(tmp, PAD_IO_REG1(index));
        break;

        default:
        return -EINVAL;
        break;
    }

    return 0;
}

int sf_pad_set_func(u32 index, pad_func func)
{
	u8 tmp;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	switch (func)
	{
		case FUNC0:
		tmp = readb(PAD_IO_REG2(index));
		tmp |= FUNC_SW_SEL;
		tmp &= ~(FMUX_SEL | MODE_BIT0 | MODE_BIT1);
		writeb(tmp, PAD_IO_REG2(index));

		tmp = readb(PAD_IO_REG1(index));
		tmp |= SW_IE;
		tmp &= ~(SW_OEN);
		writeb(tmp, PAD_IO_REG1(index));
		break;

		case FUNC1:
		tmp = readb(PAD_IO_REG2(index));
		tmp |= (FUNC_SW_SEL | MODE_BIT0);
		tmp &= ~(FMUX_SEL | MODE_BIT1);
		writeb(tmp, PAD_IO_REG2(index));

		tmp = readb(PAD_IO_REG1(index));
		tmp |= SW_IE;
		tmp &= ~(SW_OEN);
		writeb(tmp, PAD_IO_REG1(index));
		break;

		case FUNC2:
		tmp = readb(PAD_IO_REG2(index));
		tmp |= (FUNC_SW_SEL | MODE_BIT1);
		tmp &= ~(FMUX_SEL | MODE_BIT0);
		writeb(tmp, PAD_IO_REG2(index));

		tmp = readb(PAD_IO_REG1(index));
		tmp |= SW_IE;
		tmp &= ~(SW_OEN);
		writeb(tmp, PAD_IO_REG1(index));
		break;

		case FUNC3:
		tmp = readb(PAD_IO_REG2(index));
		tmp |= (FUNC_SW_SEL | MODE_BIT0 | MODE_BIT1);
		tmp &= ~(FMUX_SEL);
		writeb(tmp, PAD_IO_REG2(index));

		tmp = readb(PAD_IO_REG1(index));
		tmp |= SW_IE;
		tmp &= ~(SW_OEN);
		writeb(tmp, PAD_IO_REG1(index));
		break;

		case GPIO_INPUT:
		tmp = readb(PAD_IO_REG1(index));
		tmp |= (SW_IE | SW_OEN);
		writeb(tmp, PAD_IO_REG1(index));

		tmp = readb(PAD_IO_REG2(index));
		tmp |= (FUNC_SW_SEL | FMUX_SEL);
		writeb(tmp, PAD_IO_REG2(index));

		writeb(1, GPIO_DIR(index));
		break;

		case GPIO_OUTPUT:
		tmp = readb(PAD_IO_REG1(index));
		tmp &= ~(SW_IE | SW_OEN);
		writeb(tmp, PAD_IO_REG1(index));

		tmp = readb(PAD_IO_REG2(index));
		tmp |= (FUNC_SW_SEL | FMUX_SEL);
		writeb(tmp, PAD_IO_REG2(index));

		writeb(0, GPIO_DIR(index));
		break;

		default:
		return -EINVAL;
		break;
	}

	return 0;
}

u32 sf_pad_get_value(u32 index)
{
	u8 tmp;
	int level = 0;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	tmp = readb(PAD_IO_REG2(index)) & FMUX_SEL;
	if (tmp)
	{
		tmp = readb(GPIO_DIR(index));
		if (tmp)
			level = readb(GPIO_RDAT(index));
		else
			level = readb(GPIO_WDAT(index));
	}

	return level;
}

int sf_module_set_pad_func(sf_module module)
{
	int index;
	pad_index pad_range;

	if (module == SF_CATIP) {
		sf_pad_set_func(0, FUNC3);
		sf_pad_set_func(1, FUNC3);
		sf_pad_set_func(2, FUNC3);
		sf_pad_set_func(4, FUNC3);
		return 0;
	}

	module_analysis(module,&pad_range);

	if (pad_range.high > PAD_INDEX_MAX || pad_range.low > PAD_INDEX_MAX)
		return -EINVAL;

	for(index = pad_range.low; index <= pad_range.high; index++)
		sf_pad_set_func(index, pad_range.func);

	return 0;
}

int sf_sw_sel_set(u32 index , u8 value)
{
	u8 tmp;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	switch(value)
	{
		case 1:
			tmp = readb(PAD_IO_REG2(index));
			tmp |= (FUNC_SW_SEL);
			writeb(tmp, PAD_IO_REG2(index));
			break;

		case 0:
			tmp = readb(PAD_IO_REG2(index));
			tmp &= ~(FUNC_SW_SEL);
			writeb(tmp, PAD_IO_REG2(index));
			break;

		default:
			return -EINVAL;
			break;
	}

	return 0;
}

int sf_pad_set_ds(u32 index, u8 value)
{
	u8 tmp;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	tmp = readb(PAD_IO_REG1(index));
	tmp &= ~(0x7);
	tmp |= value;
	writeb(tmp, PAD_IO_REG1(index));

	return 0;
}

#else /* !SF19A28_MPW0*/
static void module_analysis(sf_module module, pad_index *pi)
{
	switch(module)
	{
		case SF_EMMC:
		pi->low = 0;
		pi->high = 10;
		pi->func = FUNC0;
		break;

		case SF_SPI0:
		pi->low = 0;
		pi->high = 6;
		pi->func = FUNC1;
		break;

		case SF_I2C1:
		pi->low = 22;
		pi->high = 23;
		pi->func = FUNC0;
		break;

		case SF_I2C2:
		pi->low = 24;
		pi->high = 25;
		pi->func = FUNC0;
		break;

		//case SF_PCM1:
		//pi->low = 22;
		//pi->high = 25;
		//pi->func = FUNC1;
		//break;

		case SF_UART2:
		pi->low = 22;
		pi->high = 25;
		pi->func = FUNC2;
		break;

		case SF_I2S0:
		pi->low = 26;
		pi->high = 30;
		pi->func = FUNC0;
		break;

		case SF_PCM0:
		pi->low = 26;
		pi->high = 30;
		pi->func = FUNC1;
		break;

		case SF_UART1:
		pi->low = 26;
		pi->high = 30;
		pi->func = FUNC2;
		break;

		case SF_I2S1:
		pi->low = 31;
		pi->high = 35;
		pi->func = FUNC0;
		break;

		case SF_SPI1:
		pi->low = 31;
		pi->high = 34;
		pi->func = FUNC1;
		break;

		//case SF_SPDIF0:
		//pi->low = 35;
		//pi->high = 35;
		//pi->func = FUNC1;
		//break;

		case SF_UART3:
		pi->low = 31;
		pi->high = 35;
		pi->func = FUNC2;
		break;

		case SF_PWM0:
		pi->low = 36;
		pi->high = 36;
		pi->func = FUNC0;
		break;

		case SF_RGMII:
		pi->low = 38;
		pi->high = 54;
		pi->func = FUNC0;
		break;

		case SF_RMII:
		pi->low = 38;
		pi->high = 54;
		pi->func = FUNC1;
		break;

		case SF_ETH_LED:
		pi->low = 55;
		pi->high = 59;
		pi->func = FUNC0;
		break;

		case SF_JTAG:
		pi->low = 55;
		pi->high = 59;
		pi->func = FUNC1;
		break;

		//case SF_SPDIF1:
		//pi->low = 61;
		//pi->high = 61;
		//pi->func = FUNC1;
		//break;

		case SF_PWM1:
		pi->low = 37;
		pi->high = 37;
		pi->func = FUNC0;

		case SF_SDIO:
		case SF_UART0:
		case SF_I2C0:
		case SF_RTC:
		case SF_USB:
		case SF_DDR:
		default:
		pi->low = pi->high = 0xff;
		break;
	}
}

int sf_sw_oen_set(u32 index , u8 value)
{
    u8 tmp;
    int cnt = 0;
    int num = 0;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

    cnt = index / 8;
    num = index % 8;

    switch(value)
    {
        case 1:
        tmp = readb(FUNC_SW_OEN(cnt));
        tmp |= (1 << num);
        writeb(tmp, FUNC_SW_OEN(cnt));
        break;

        case 0:
        tmp = readb(FUNC_SW_OEN(cnt));
        tmp &= ~(1 << num);
        writeb(tmp, FUNC_SW_OEN(cnt));
        break;

        default:
        return -EINVAL;
        break;
    }

    return 0;
}

int sf_sw_ie_set(u32 index , u8 value)
{
    u8 tmp;
    int cnt = 0;
    int num = 0;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

    cnt = index / 8;
    num = index % 8;

    switch(value)
    {
        case 1:
        tmp = readb(FUNC_SW_IE(cnt));
        tmp |= (1 << num);
        writeb(tmp, FUNC_SW_IE(cnt));
        break;

        case 0:
        tmp = readb(FUNC_SW_IE(cnt));
        tmp &= ~(1 << num);
        writeb(tmp, FUNC_SW_IE(cnt));
        break;

        default:
        return -EINVAL;
        break;
    }

    return 0;
}
/**************************************************************************************************
*                                                                                                 *
* -Description:                                                                                   *
*    This part is used to set pin funciton.                                                       *
*                                                                                                 *
***************************************************************************************************/
int sf_pad_set_func(u32 index, pad_func func)
{
	u8 tmp;
	int mod_cnt = 0;
	int mod_num = 0;
	int fun_cnt = 0;
	int fun_num = 0;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	fun_cnt = index / 8;
	fun_num = index % 8;
	mod_cnt = index / PAD_PER_GROUP_PINS;
	mod_num = index % PAD_PER_GROUP_PINS;

	switch (func)
	{
		case FUNC0:
		tmp = readb(FUNC_SW_IE(fun_cnt));
		tmp |= 1 << fun_num;
		writeb(tmp, FUNC_SW_IE(fun_cnt));

		tmp = readb(FUNC_SW_SEL(fun_cnt));
		tmp |= (1 << fun_num);
		writeb(tmp, FUNC_SW_SEL(fun_cnt));

		tmp = readb(PAD_FUCN_SEL(fun_cnt));
		tmp &= ~(1 << fun_num);
		writeb(tmp, PAD_FUCN_SEL(fun_cnt));

		tmp = readb(PAD_MODE_SEL(mod_cnt));
		tmp &= ~(0x3 << (mod_num * 2));
		tmp |= (0 << (mod_num * 2));
		writeb(tmp, PAD_MODE_SEL(mod_cnt));
		break;


		case FUNC1:
		tmp = readb(FUNC_SW_IE(fun_cnt));
		tmp |= 1 << fun_num;
		writeb(tmp, FUNC_SW_IE(fun_cnt));

		tmp = readb(FUNC_SW_SEL(fun_cnt));
		tmp |= (1 << fun_num);
		writeb(tmp, FUNC_SW_SEL(fun_cnt));

		tmp = readb(PAD_FUCN_SEL(fun_cnt));
		tmp &= ~(1 << fun_num);
		writeb(tmp, PAD_FUCN_SEL(fun_cnt));

		tmp = readb(PAD_MODE_SEL(mod_cnt));

		tmp &= ~(0x3 << (mod_num * 2));
		tmp |= (1 << (mod_num * 2));
		writeb(tmp, PAD_MODE_SEL(mod_cnt));
		break;


		case FUNC2:
		tmp = readb(FUNC_SW_IE(fun_cnt));
		tmp |= 1 << fun_num;
		writeb(tmp, FUNC_SW_IE(fun_cnt));

		tmp = readb(FUNC_SW_SEL(fun_cnt));
		tmp |= (1 << fun_num);
		writeb(tmp, FUNC_SW_SEL(fun_cnt));

		tmp = readb(PAD_FUCN_SEL(fun_cnt));
		tmp &= ~(1 << fun_num);
		writeb(tmp, PAD_FUCN_SEL(fun_cnt));

		tmp = readb(PAD_MODE_SEL(mod_cnt));
		tmp &= ~(0x3 << (mod_num * 2));
		tmp |= (2 << (mod_num * 2));
		writeb(tmp, PAD_MODE_SEL(mod_cnt));
		break;


		case FUNC3:
		tmp = readb(FUNC_SW_IE(fun_cnt));
		tmp |= 1 << fun_num;
		writeb(tmp, FUNC_SW_IE(fun_cnt));

		tmp = readb(FUNC_SW_SEL(fun_cnt));
		tmp |= (1 << fun_num);
		writeb(tmp, FUNC_SW_SEL(fun_cnt));

		tmp = readb(PAD_FUCN_SEL(fun_cnt));
		tmp &= ~(1 << fun_num);
		writeb(tmp, PAD_FUCN_SEL(fun_cnt));

		tmp = readb(PAD_MODE_SEL(mod_cnt));
		tmp &= ~(0x3 << (mod_num * 2));
		tmp |= (3 << (mod_num * 2));
		writeb(tmp, PAD_MODE_SEL(mod_cnt));
		break;


		case GPIO_INPUT:
		tmp = readb(FUNC_SW_SEL(fun_cnt));
		tmp |= (1 << fun_num);
		writeb(tmp, FUNC_SW_SEL(fun_cnt));

		tmp = readb(FUNC_SW_IE(fun_cnt));
		tmp |= 1 << fun_num;
		writeb(tmp, FUNC_SW_IE(fun_cnt));
		tmp = readb(FUNC_SW_OEN(fun_cnt));
		tmp |= 1 << fun_num;
		writeb(tmp, FUNC_SW_OEN(fun_cnt));

		tmp = readb(PAD_FUCN_SEL(fun_cnt));
		tmp |= 1 << fun_num;
		writeb(tmp, PAD_FUCN_SEL(fun_cnt));

		writeb(1, GPIO_DIR(index));
		break;

		case GPIO_OUTPUT:
		tmp = readb(FUNC_SW_SEL(fun_cnt));
		tmp |= (1 << fun_num);
		writeb(tmp, FUNC_SW_SEL(fun_cnt));

		tmp = readb(FUNC_SW_OEN(fun_cnt));
		tmp &= ~(1 << fun_num);
		writeb(tmp, FUNC_SW_OEN(fun_cnt));
		tmp = readb(FUNC_SW_IE(fun_cnt));
		tmp &= ~(1 << fun_num);
		writeb(tmp, FUNC_SW_IE(fun_cnt));

		tmp = readb(PAD_FUCN_SEL(fun_cnt));
		tmp |= 1 << fun_num;
		writeb(tmp, PAD_FUCN_SEL(fun_cnt));

		writeb(0, GPIO_DIR(index));
		break;

		default:
		printf("sf_pad_set_func error! index:%d func:%d \n", index, func);
		return -EINVAL;
		break;
	}

	return 0;
}

/**************************************************************************************************
*                                                                                                 *
* -Description:                                                                                   *
*    This part is used to get pin output level.                                                   *
*                                                                                                 *
***************************************************************************************************/
u32 sf_pad_get_value(u32 index)
{
	u8 tmp;
	int cnt = 0;
	int num = 0;
	int level = 0;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	cnt = index / 8;
	num = index % 8;

	tmp = readb(PAD_FUCN_SEL(cnt));
	if (tmp & (1 << num))
	{
		tmp = readb(GPIO_DIR(index));
		if (tmp)
			level = readb(GPIO_RDAT(index));
		else
			level = readb(GPIO_WDAT(index));
	}

	return level;
}
/**************************************************************************************************
*                                                                                                 *
* -Description:                                                                                   *
*    This part is used to set mudule pin funtion.                                                 *
*                                                                                                 *
***************************************************************************************************/
int sf_module_set_pad_func(sf_module module)
{
	int index;
	pad_index pad_range;

	if(module == SF_PCM1)
	{
		sf_pad_set_func(22, FUNC1);
		sf_pad_set_func(23, FUNC1);
		sf_pad_set_func(24, FUNC1);
		sf_pad_set_func(25, FUNC1);
		sf_pad_set_func(36, FUNC1);
	}
	else if(module == SF_SPDIF)
	{
		sf_pad_set_func(35, FUNC1);
		sf_pad_set_func(62, FUNC1);
	}
	else
	{
		module_analysis(module,&pad_range);

		if (pad_range.high > PAD_INDEX_MAX || pad_range.low > PAD_INDEX_MAX)
			return -EINVAL;

		for(index = pad_range.low; index <= pad_range.high; index++)
			sf_pad_set_func(index, pad_range.func);
	}

	return 0;
}

/*
 * Set function / pad status.
 * Attention: this function is only used in eth download.
 */
int sf_sw_sel_set(u32 index , u8 value)
{
	u8 tmp;
	int cnt = 0;
	int num = 0;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	cnt = index / 8;
	num = index % 8;

	switch(value)
	{
		case 1:
			tmp = readb(FUNC_SW_SEL(cnt));
			tmp |= (1 << num);
			writeb(tmp, FUNC_SW_SEL(cnt));
			break;

		case 0:
			tmp = readb(FUNC_SW_SEL(cnt));
			tmp &= ~(1 << num);
			writeb(tmp, FUNC_SW_SEL(cnt));
			break;

		default:
			return -EINVAL;
			break;
	}

	return 0;
}
#endif /* SF19A28_MPW0 */
