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
#ifndef __JL51XX_DRV_PORT_H__
#define __JL51XX_DRV_PORT_H__

#include "jl51xx/jl51xx_drv_phy.h"
#include "jl51xx/reg_phy.h"
#include "port.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl51xx_port_phy_control_reg_set(jl_port_t port, jl_uint32 regval);
jl_ret_t jl51xx_port_phy_control_reg_get(jl_port_t port, jl_uint32 *pregval);
jl_ret_t jl51xx_port_phy_an_adv_reg_set(jl_port_t port, jl_uint32 pregval);
jl_ret_t jl51xx_port_phy_an_adv_reg_get(jl_port_t port, jl_uint32 *pregval);
jl_ret_t jl51xx_port_phy_linkp_an_reg_get(jl_port_t port, jl_uint32 *pregval);
jl_ret_t jl51xx_port_phy_status_reg_get(jl_port_t port, jl_uint32 *regval);
jl_ret_t jl51xx_port_phy_isolate_set(jl_port_t port, jl_enable_t enable);
jl_ret_t jl51xx_port_phy_isolate_get(jl_port_t port, jl_enable_t *penable);
jl_ret_t jl51xx_port_phy_enable_set(jl_port_t port, jl_enable_t enable);
jl_ret_t jl51xx_port_phy_enable_get(jl_port_t port, jl_enable_t *penable);
jl_ret_t jl51xx_port_phy_loopback_get(jl_port_t port, jl_enable_t *pen_loopback);
jl_ret_t jl51xx_port_phy_loopback_set(jl_port_t port, jl_enable_t en_loopback);

jl_ret_t jl51xx_port_mac_force_link_set(jl_port_t port, jl_port_mac_ability_t *pability);
jl_ret_t jl51xx_port_mac_force_link_get(jl_port_t port, jl_port_mac_ability_t *pability);

jl_ret_t jl51xx_port_mac_force_link_ext_set(jl_port_t port, jl_port_ext_mac_ability_t *pability);
jl_ret_t jl51xx_port_mac_force_link_ext_get(jl_port_t port, jl_port_ext_mac_ability_t *pability);

jl_ret_t jl51xx_port_mac_loopback_set(jl_port_t port, jl_enable_t en_loopback);
jl_ret_t jl51xx_port_mac_loopback_get(jl_port_t port, jl_enable_t *pen_loopback);
#ifdef __cplusplus
}
#endif
#endif /* __JL51XX_DRV_PORT_H */
