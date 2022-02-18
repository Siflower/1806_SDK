/*
 * Pinctrl driver for Siflower SoCs
 * Copyright (c) 2015 Siflower, Inc
 * Written by neil Zhang <neil.zhang@siflower.com.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <syscon.h>
#include <asm/io.h>
#include <dm/pinctrl.h>
#include <dm/root.h>
#include <mach/pad.h>

typedef enum pad_func_t{
	FUNC0 = 0,
	FUNC1,
	FUNC2,
	FUNC3,
} pad_func;

typedef struct pad_index_t {
	u16 high;
	u16 low;
	pad_func func;
} pad_index;

struct sfa18_pinctrl_priv {
	u32 pad_base;
	u32 gpio_base;
};

DECLARE_GLOBAL_DATA_PTR;

/**************************************************************************************************
*                                                                                                 *
* -Description:                                                                                   *
*    This part is used to get module pin number.                                                  *
*                                                                                                 *
***************************************************************************************************/
static void sf_module_analysis(int module, pad_index *pi)
{
	switch(module)
	{
		case PERIPH_ID_EMMC:
		pi->low = 0;
		pi->high = 10;
		pi->func = FUNC0;
		break;

		case PERIPH_ID_SPI0:
		pi->low = 0;
		pi->high = 6;
		pi->func = FUNC1;
		break;

		case PERIPH_ID_I2C1:
		pi->low = 22;
		pi->high = 23;
		pi->func = FUNC0;
		break;

		case PERIPH_ID_I2C2:
		pi->low = 24;
		pi->high = 25;
		pi->func = FUNC0;
		break;

		//case PERIPH_ID_PCM1:
		//pi->low = 22;
		//pi->high = 25;
		//pi->func = FUNC1;
		//break;

		case PERIPH_ID_UART2:
		pi->low = 22;
		pi->high = 25;
		pi->func = FUNC2;
		break;

		case PERIPH_ID_I2S0:
		pi->low = 26;
		pi->high = 30;
		pi->func = FUNC0;
		break;

		case PERIPH_ID_PCM0:
		pi->low = 26;
		pi->high = 30;
		pi->func = FUNC1;
		break;

		case PERIPH_ID_UART1:
		pi->low = 26;
		pi->high = 30;
		pi->func = FUNC2;
		break;

		case PERIPH_ID_I2S1:
		pi->low = 31;
		pi->high = 35;
		pi->func = FUNC0;
		break;

		case PERIPH_ID_SPI1:
		pi->low = 31;
		pi->high = 34;
		pi->func = FUNC1;
		break;

		//case PERIPH_ID_SPDIF0:
		//pi->low = 35;
		//pi->high = 35;
		//pi->func = FUNC1;
		//break;

		case PERIPH_ID_UART3:
		pi->low = 31;
		pi->high = 35;
		pi->func = FUNC2;
		break;

		case PERIPH_ID_PWM0:
		pi->low = 36;
		pi->high = 36;
		pi->func = FUNC0;
		break;

		case PERIPH_ID_RGMII:
		pi->low = 38;
		pi->high = 54;
		pi->func = FUNC0;
		break;

		case PERIPH_ID_RMII:
		pi->low = 38;
		pi->high = 54;
		pi->func = FUNC1;
		break;

		case PERIPH_ID_ETH_LED:
		pi->low = 55;
		pi->high = 59;
		pi->func = FUNC0;
		break;

		case PERIPH_ID_JTAG:
		pi->low = 55;
		pi->high = 59;
		pi->func = FUNC1;
		break;

		//case PERIPH_ID_SPDIF1:
		//pi->low = 61;
		//pi->high = 61;
		//pi->func = FUNC1;
		//break;

		case PERIPH_ID_SDIO:
		case PERIPH_ID_UART0:
		case PERIPH_ID_I2C0:
		case PERIPH_ID_PWM1:
		case PERIPH_ID_RTC:
		case PERIPH_ID_USB:
		case PERIPH_ID_DDR:
		default:
		pi->low = pi->high = 0xff;
		printf("this module don't multiplexer pads!!!\n");
		break;
	}
}

/**************************************************************************************************
*                                                                                                 *
* -Description:                                                                                   *
*    This part is used to set pin funciton.                                                       *
*                                                                                                 *
***************************************************************************************************/
static int sf_pad_set_func(u32 index, pad_func func, u32 pbase)
{
	u8 tmp;
	int mod_cnt = 0;
	int mod_num = 0;
	int fun_cnt = 0;
	int fun_num = 0;
	u32 func_reg = 0;
	u32 mod_reg = 0;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	fun_cnt = index / PAD_FUN_SEL_REG_WID;
	fun_num = index % PAD_FUN_SEL_REG_WID;
	mod_cnt = index / PAD_PER_GROUP_PINS;
	mod_num = index % PAD_PER_GROUP_PINS;

	func_reg = PAD_FUCN_SEL(pbase, fun_cnt); 
	mod_reg = PAD_MODE_SEL(pbase, mod_cnt);

#ifndef CONFIG_TARGET_SFA18_MPW0	
	tmp = readb((void*)FUNC_SW_IE(pbase, fun_cnt));
	tmp |= 1 << fun_num;
	writeb(tmp, (void*)FUNC_SW_IE(pbase, fun_cnt));

	tmp = readb((void*)FUNC_SW_SEL(pbase, fun_cnt));
	tmp |= (1 << fun_num);
	writeb(tmp, (void*)FUNC_SW_SEL(pbase, fun_cnt));
#endif

	if(func > FUNC3) {
		printf("sf_pad_set_func error! index:%d func:%d \n", index, func);
		return -EINVAL;
	}
	else {
		tmp = readb((const void *)func_reg);
		tmp &= ~(1 << fun_num);
		writeb(tmp, (volatile void *)func_reg);

		tmp = readb((const void *)mod_reg);
		tmp &= ~(0x3 << (mod_num * 2));
		tmp |= (func << (mod_num * 2));
		writeb(tmp, (volatile void *)mod_reg);
	}

	return 0;
}

#ifndef CONFIG_TARGET_SFA18_MPW0
static int sf_sw_sel_set(u32 index, u8 value, u32 pbase)
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
        tmp = readb((void*)FUNC_SW_SEL(pbase, cnt));
		tmp |= (1 << num);
	    writeb(tmp, (void*)FUNC_SW_SEL(pbase, cnt));
	    break;

	    case 0:
	    tmp = readb((void*)FUNC_SW_SEL(pbase, cnt));
	    tmp &= ~(1 << num);
	    writeb(tmp, (void*)FUNC_SW_SEL(pbase, cnt));
	    break;

	    default:
	    return -EINVAL;
	    break;
	}

	return 0;
}

static int sf_sw_ie_set(u32 index, u8 value, u32 pbase)
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
			tmp = readb((void*)FUNC_SW_IE(pbase, cnt));
			tmp |= (1 << num);
			writeb(tmp, (void*)FUNC_SW_IE(pbase, cnt));
			break;

		case 0:
			tmp = readb((void*)FUNC_SW_IE(pbase, cnt));
			tmp &= ~(1 << num);
			writeb(tmp, (void*)FUNC_SW_IE(pbase, cnt));
			break;

		default:
			return -EINVAL;
			break;
	}

	return 0;
}

static void sfa18_init_pad_status(u32 pbase)
{
	u32 index = 0;

	for(index = 0; index <= PAD_INDEX_MAX; index++)
	{
	    sf_sw_sel_set(index, 1, pbase);
	    sf_sw_ie_set(index, 1, pbase);

	    if(index == 58){
	        index ++;
	    }
	}

}
#endif
/**************************************************************************************************
*                                                                                                 *
* -Description:                                                                                   *
*    This part is used to set mudule pin funtion.                                                 *
*                                                                                                 *
***************************************************************************************************/
static int sf_module_set_pad_func(int module, u32 pbase)
{
	int index;
	pad_index pad_range;

	if(module == PERIPH_ID_PCM1)
	{
		sf_pad_set_func(22, FUNC1, pbase);
		sf_pad_set_func(23, FUNC1, pbase);
		sf_pad_set_func(24, FUNC1, pbase);
		sf_pad_set_func(25, FUNC1, pbase);
		sf_pad_set_func(36, FUNC1, pbase);
	}
	else if(module == PERIPH_ID_SPDIF)
	{
		sf_pad_set_func(35, FUNC1, pbase);
		sf_pad_set_func(62, FUNC1, pbase);
	}
	else if(module == PERIPH_ID_GDU)
	{
		for(index = 7; index <= 10; index++)
		{
			//sf_pad_set_func(index, FUNC1, pbase);
			sf_pad_set_func(index, FUNC3, pbase);
		}
		sf_pad_set_func(24, FUNC3, pbase);
		sf_pad_set_func(25, FUNC3, pbase);
		for(index = 31; index <= 35; index++)
		{
			sf_pad_set_func(index, FUNC3, pbase);
		}
		for(index = 38; index <= 54; index++)
		{
			sf_pad_set_func(index, FUNC3, pbase);
		}
	}
	else
	{
		sf_module_analysis(module,&pad_range);

		if (pad_range.high > PAD_INDEX_MAX || pad_range.low > PAD_INDEX_MAX)
			return -EINVAL;

	    for(index = pad_range.low; index <= pad_range.high; index++)
			sf_pad_set_func(index, pad_range.func, pbase);
	}

	return 0;
}


static int sfa18_pinctrl_request(struct udevice *dev, int func, int flags)
{
	struct sfa18_pinctrl_priv *priv = dev_get_priv(dev);

	debug("%s: func=%x, flags=%x\n", __func__, func, flags);
	if((PERIPH_ID_GDU < func) && (PERIPH_ID_SPI0 > func))
		return -EINVAL;
	else
		sf_module_set_pad_func(func, priv->pad_base);

	return 0;
}

static struct pinctrl_ops sfa18_pinctrl_ops = {
	.request	= sfa18_pinctrl_request,
};

static int sfa18_pinctrl_bind(struct udevice *dev)
{
	/* scan child GPIO banks */
	return dm_scan_fdt_node(dev, gd->fdt_blob, dev->of_offset, false);
}

static int sfa18_pinctrl_probe(struct udevice *dev)
{
	struct sfa18_pinctrl_priv *priv = dev_get_priv(dev);
	int ret = 0;
	struct fdt_resource res;
	void *fdt = (void *)gd->fdt_blob;
	int node = dev->of_offset;
	fdt_addr_t node_addr;

	node_addr = dev_get_addr(dev);
	if (node_addr == FDT_ADDR_T_NONE)
		return -EINVAL;

	ret = fdt_get_named_resource(fdt, node, "reg", "reg-names",
				     "pad_base", &res);
	if (ret < 0) {
		printf("pinctrl: resource \"pad_base\" not found\n");
		return ret;
	}
	priv->pad_base = res.start + 0xB0000000;

#ifndef CONFIG_TARGET_SFA18_MPW0
	sfa18_init_pad_status(priv->pad_base);
#endif

	return 0;
}

static const struct udevice_id sfa18_pinctrl_ids[] = {
	{ .compatible = "siflower,sfa18-pinctrl" },
	{ .compatible = "siflower,sfa28-pinctrl" },
	{ }
};

U_BOOT_DRIVER(pinctrl_sfa18) = {
	.name		= "pinctrl_sfa18",
	.id		= UCLASS_PINCTRL,
	.of_match	= sfa18_pinctrl_ids,
	.priv_auto_alloc_size = sizeof(struct sfa18_pinctrl_priv),
	.ops		= &sfa18_pinctrl_ops,
	.bind		= sfa18_pinctrl_bind,
	.probe		= sfa18_pinctrl_probe,
};
