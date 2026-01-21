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
#ifndef __JL_PORTABLE_H__
#define __JL_PORTABLE_H__

#ifndef __KERNEL__
#include <stdint.h>
#endif

#include "jl_error.h"
#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gpio_descriptor_s {
	jl_int32 pin;
	void *hd;
	char name[16];
};
#define MDC_PIN		5
#define MDIO_PIN	6

jl_ret_t port_gpio_request(struct gpio_descriptor_s *gpio);
jl_ret_t port_gpio_free(struct gpio_descriptor_s *gpio);
void port_gpio_set_dir(struct gpio_descriptor_s *gpio, jl_int32 dir);
void port_gpio_set_value(struct gpio_descriptor_s *gpio, jl_int32 val);
jl_int32 port_gpio_get_value(struct gpio_descriptor_s *gpio);


jl_ret_t port_spi_init(void);
jl_ret_t port_spi_deinit(void);
jl_ret_t port_spi_read(jl_uint8 *tx_buf, jl_uint8 *rx_buf, jl_uint32 size);
jl_ret_t port_spi_write(jl_uint8 *tx_buf, jl_uint8 *rx_buf, jl_uint32 size);


jl_ret_t port_mdio_init(void);
jl_ret_t port_mdio_deinit(void);
void port_mdio_write(jl_uint8 phy, jl_uint8 reg, jl_uint16 val);
jl_uint16 port_mdio_read(jl_uint8 phy, jl_uint8 reg);


void *port_mutex_init(void);
void port_mutex_deinit(void *arg);
void port_mutex_lock(void *arg);
void port_mutex_unlock(void *arg);
void port_ndelay(jl_uint32 ns);
void port_udelay(jl_uint32 us);


#ifdef __cplusplus
}
#endif

#endif /* __JL_PORTABLE_H__ */
