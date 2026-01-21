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

#include "jl_error.h"
#include "jl_types.h"
#include "portable/jl_portable.h"
#include "driver/hal_smi.h"


struct bit_buffer_s {
	jl_uint8 *p_data;
	jl_uint8  i_mask;
	jl_int32 i_size;
	jl_int32 i_data;
};

struct smi_device_s g_smi_dev;


#define write_bits(buffer, count, bits)\
do {\
	struct bit_buffer_s *p_buffer = (buffer);\
	jl_int32 i_count = (count);\
	jl_uint64 i_bits = (bits);\
	while (i_count > 0) {\
		i_count--;\
		if ((i_bits >> i_count)&0x01)\
			p_buffer->p_data[p_buffer->i_data] |= p_buffer->i_mask;\
		else\
			p_buffer->p_data[p_buffer->i_data] &= ~p_buffer->i_mask;\
		p_buffer->i_mask >>= 1;\
		if (p_buffer->i_mask == 0) {\
			p_buffer->i_data++;\
			p_buffer->i_mask = 0x80;\
		}\
	}\
} while (0)

#define read_bits(buffer, count, bits)\
do {\
	struct bit_buffer_s *p_buffer = (buffer);\
	jl_int32 i_count = (count);\
	*bits = 0;\
	while (i_count > 0) {\
		i_count--;\
		*bits += p_buffer->p_data[p_buffer->i_data] & p_buffer->i_mask;\
		p_buffer->i_mask >>= 1;\
		*bits <<= 1;\
		if (p_buffer->i_mask == 0) {\
			p_buffer->i_data++;\
			p_buffer->i_mask = 0x80;\
		}\
	}\
} while (0)

/* wr:0,read;1:write*/
static void __pack_spi_frame(jl_int32 wr, jl_uint8 *buf, jl_uint8 phy, jl_uint8 reg, jl_uint16 data)
{
	struct bit_buffer_s bitbuf;

	bitbuf.i_size = 8;			/* a clause-22 frame size in bytes */
	bitbuf.i_mask = 0x80;
	bitbuf.p_data = buf;
	bitbuf.i_data = 0;

	write_bits(&bitbuf, 32, 0xffffffff);	/* 32 bits preambles:32-1 */
	write_bits(&bitbuf, 2, 1);		/* 2 bits start code:01 */
	if (wr)
		write_bits(&bitbuf, 2, 1);		/* 2 bits opcode(write:01, read:10) */
	else
		write_bits(&bitbuf, 2, 2);
	write_bits(&bitbuf, 5, phy);		/* 5 bits phy id */
	write_bits(&bitbuf, 5, reg);		/* 5 bits register address */
	write_bits(&bitbuf, 2, 2);		/* 2 bits turn around(a trick way) */
	write_bits(&bitbuf, 16, data);		/* 16 bits data, if spi read, tx data is 0xff(MISO/MOSI wired-AND) */
}

jl_uint16 jl_smi_read(jl_uint8 phy, jl_uint8 reg)
{
	jl_uint8 tx_buf[8];
	jl_uint8 rx_buf[8];

	/* XXX mutex is not neccessary if you are using spidev driver
	 * casue it's transfer ioctl is thread safe */
#ifdef CONFIG_JLFE_PHY_MUTEX
	port_mutex_lock(g_smi_dev.phy_mutex);
#endif

	__pack_spi_frame(0, tx_buf, phy, reg, 0xffff);
	port_spi_read(tx_buf, rx_buf, 8);

#ifdef CONFIG_JLFE_PHY_MUTEX
	port_mutex_unlock(g_smi_dev.phy_mutex);
#endif

	return ((jl_uint16) ((jl_uint16)rx_buf[6] << 8 | rx_buf[7]));
}

void jl_smi_write(jl_uint8 phy, jl_uint8 reg, jl_uint16 val)
{
	jl_uint8 tx_buf[8];
	jl_uint8 rx_buf[8];

	/* XXX mutex is not neccessary if you are using spidev driver
	 * casue it's transfer ioctl is thread safe */
#ifdef CONFIG_JLFE_PHY_MUTEX
	port_mutex_lock(g_smi_dev.phy_mutex);
#endif

	__pack_spi_frame(1, tx_buf, phy, reg, val);
	port_spi_write(tx_buf, rx_buf, 8);

#ifdef CONFIG_JLFE_PHY_MUTEX
	port_mutex_unlock(g_smi_dev.phy_mutex);
#endif
}

jl_api_ret_t jl_smi_init(void)
{
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

	return port_spi_init();
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

	return port_spi_deinit();
}
