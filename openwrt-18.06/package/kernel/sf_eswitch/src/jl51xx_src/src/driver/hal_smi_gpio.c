/*
 * Copyright (c) 2014-2024 JLSemi Limited
 * All Rights Reserved
 *
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of JLSemi Limited
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code.
 *
 * No part of this code may be reproduced, stored in a retrieval system,
 * or transmitted, in any form or by any means, electronic, mechanical,
 * photocopying, recording, or otherwise, without the prior written
 * permission of JLSemi Limited
 */

#include "jl_types.h"
#include "jl_error.h"
#include "portable/jl_portable.h"
#include "driver/hal_smi.h"

#define SMI_READ	2
#define SMI_WRITE	1

#define SMI_SETUP_TIME	    10
#define SMI_HOLD_TIME	    10

/* Minimum MDC period is 400 ns, plus some margin for error.  smi_DELAY
  * is done twice per period.
  */
#define SMI_DELAY 250

/* The PHY may take up to 300 ns to produce data, plus some margin
 * for error.
 */
#define SMI_READ_DELAY 350

struct smibb_gpio_s {
	struct gpio_descriptor_s mdc;
	struct gpio_descriptor_s mdio;
};

struct smibb_gpio_s g_smi_gpio;
struct smi_device_s g_smi_dev;


static jl_ret_t init_smi(struct smibb_gpio_s *gpio)
{
	jl_ret_t ret = JL_ERR_OK;

	ret = port_gpio_request(&gpio->mdc);
	if (ret)
		return ret;
	ret = port_gpio_request(&gpio->mdio);
	if (ret)
		return ret;

	port_gpio_set_dir(&gpio->mdc, 1);
	port_gpio_set_value(&gpio->mdc, 0);
	port_gpio_set_dir(&gpio->mdio, 0);

	return ret;
}

static jl_ret_t deinit_smi(struct smibb_gpio_s *gpio)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_ret_t _ret = JL_ERR_OK;

	_ret = port_gpio_free(&gpio->mdc);
	if (_ret)
		ret = JL_ERR_FAIL;

	_ret = port_gpio_free(&gpio->mdio);
	if (_ret)
		ret = JL_ERR_FAIL;

	return ret;
}

static void set_mdc(struct smibb_gpio_s *gpio, jl_int32 val)
{
	port_gpio_set_value(&gpio->mdc, val);
}

static void set_mdio_dir(struct smibb_gpio_s *gpio , jl_int32 dir)
{
	port_gpio_set_dir(&gpio->mdio, dir);
}

static void set_mdio_data(struct smibb_gpio_s *gpio, jl_int32 val)
{
	port_gpio_set_value(&gpio->mdio, val);
}

static jl_ret_t get_mdio_data(struct smibb_gpio_s *gpio)
{
	return port_gpio_get_value(&gpio->mdio);
}

static void smibb_send_bit(jl_int32 val)
{
	set_mdio_data(&g_smi_gpio, val);
	port_ndelay(SMI_DELAY);
	set_mdc(&g_smi_gpio, 1);
	port_ndelay(SMI_DELAY);
	set_mdc(&g_smi_gpio, 0);
}

static jl_ret_t smibb_get_bit(void)
{
	port_ndelay(SMI_DELAY);
	set_mdc(&g_smi_gpio, 1);
	port_ndelay(SMI_READ_DELAY);
	set_mdc(&g_smi_gpio, 0);

	return get_mdio_data(&g_smi_gpio);
}

static void smibb_send_num(jl_uint16 val, jl_int32 bits)
{
	jl_int32 i;

	for (i = bits - 1; i >= 0; i--)
		smibb_send_bit((val >> i) & 1);
}

static jl_uint16 smibb_get_num(jl_int32 bits)
{
	jl_int32 i;
	jl_uint16 ret = 0;

	for (i = bits - 1; i >= 0; i--) {
		ret <<= 1;
		ret |= smibb_get_bit();
	}

	return ret;
}

static void smibb_cmd(jl_int32 op, jl_uint8 phy, jl_uint8 reg)
{
	jl_int32 i;

	set_mdio_dir(&g_smi_gpio, 1);

	/*
	 * Send a 32 bit preamble ('1's) with an extra '1' bit for good
	 * measure.  The IEEE spec says this is a PHY optional
	 * requirement.  The AMD 79C874 requires one after power up and
	 * one after a MII communications error.  This means that we are
	 * doing more preambles than we need, but it is safer and will be
	 * much more robust.
	 */

	for (i = 0; i < 32; i++)
		smibb_send_bit(1);

	/* TODO Clause 45 */
	/* send the start bit (01) and the read opcode (10) or write (01).
	   Clause 45 operation uses 00 for the start and 11, 10 for
	   read/write */
	smibb_send_bit(0);
	smibb_send_bit(1);
	smibb_send_bit((op >> 1) & 1);
	smibb_send_bit((op >> 0) & 1);

	smibb_send_num(phy, 5);
	smibb_send_num(reg, 5);
}

jl_uint16 jl_smi_read(jl_uint8 phy, jl_uint8 reg)
{
	jl_uint16 ret;

#ifdef CONFIG_JLFE_PHY_MUTEX
	port_mutex_lock(g_smi_dev.phy_mutex);
#endif

	smibb_cmd(SMI_READ, phy, reg);

	set_mdio_dir(&g_smi_gpio, 0);

	smibb_get_bit();

	ret = smibb_get_num(16);

#ifdef CONFIG_JLFE_PHY_MUTEX
	port_mutex_unlock(g_smi_dev.phy_mutex);
#endif

	return ret;
}

void jl_smi_write(jl_uint8 phy, jl_uint8 reg, jl_uint16 val)
{
#ifdef CONFIG_JLFE_PHY_MUTEX
	port_mutex_lock(g_smi_dev.phy_mutex);
#endif

	smibb_cmd(SMI_WRITE, phy, reg);

	smibb_send_bit(1);
	smibb_send_bit(0);

	smibb_send_num(val, 16);

	set_mdio_dir(&g_smi_gpio, 0);

#ifdef CONFIG_JLFE_PHY_MUTEX
	port_mutex_unlock(g_smi_dev.phy_mutex);
#endif
}

jl_api_ret_t jl_smi_init(void)
{
	g_smi_gpio.mdc.pin = MDC_PIN;
	g_smi_gpio.mdc.hd = NULL;
	g_smi_gpio.mdio.pin = MDIO_PIN;
	g_smi_gpio.mdio.hd = NULL;

	g_smi_dev.device = &g_smi_gpio;

#ifdef CONFIG_JLFE_PHY_MUTEX
	g_smi_dev.phy_mutex = port_mutex_init();
	if (!g_smi_dev.phy_mutex)
		return JL_ERR_INIT;
#endif

#ifdef CONFIG_JLFE_APB_MUTEX
	g_smi_dev.apb_mutex = port_mutex_init();
	if (!g_smi_dev.apb_mutex)
		return JL_ERR_INIT;
#endif

	return init_smi(&g_smi_gpio);
}

jl_api_ret_t jl_smi_deinit(void)
{
#ifdef CONFIG_JLFE_PHY_MUTEX
	if (g_smi_dev.phy_mutex) {
		port_mutex_deinit(g_smi_dev.phy_mutex);
		g_smi_dev.phy_mutex = NULL;
	}
#endif

#ifdef CONFIG_JLFE_APB_MUTEX
	if (g_smi_dev.apb_mutex) {
		port_mutex_deinit(g_smi_dev.apb_mutex);
		g_smi_dev.apb_mutex = NULL;
	}
#endif

	return deinit_smi(&g_smi_gpio);
}
