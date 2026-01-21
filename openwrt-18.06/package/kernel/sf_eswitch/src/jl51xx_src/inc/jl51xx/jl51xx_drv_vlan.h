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

#ifndef __JL51XX_DRV_VLAN_H__
#define __JL51XX_DRV_VLAN_H__

#include "jl_base.h"
#include "vlan.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl51xx_vlan_init(void);
jl_ret_t jl51xx_vlan_deinit(void);
jl_ret_t __vlan_table_reset(void);
jl_ret_t jl51xx_port_vlan_assignment_get(jl_port_t port, enum vlan_assignment *opt);
jl_ret_t jl51xx_port_vlan_assignment_set(jl_port_t port, enum vlan_assignment opt);
jl_ret_t jl51xx_port_vlan_ingress_operation_get(jl_uint32 port, enum vlan_operation *opt);
jl_ret_t jl51xx_port_vlan_ingress_operation_set(jl_uint32 port, enum vlan_operation opt);
jl_ret_t jl51xx_port_vlan_egress_operation_get(jl_uint32 port, enum vlan_operation *opt);
jl_ret_t jl51xx_port_vlan_egress_operation_set(jl_uint32 port, enum vlan_operation opt);
jl_ret_t jl51xx_port_vlan_get(jl_port_t port, jl_uint32 *vid);
jl_ret_t jl51xx_port_vlan_set(jl_port_t port, jl_uint32 vid);
jl_ret_t jl51xx_vlan_get(jl_uint32 vlan_index, struct vlan_config *config);
jl_ret_t jl51xx_vlan_set(jl_uint32 vlan_index, struct vlan_config *config);
jl_ret_t jl51xx_vlan_set_batch(struct vlan_item *v_list, uint16_t size);
jl_ret_t jl51xx_vlan_acpt_frmt_set(jl_uint32 port, jl_vlan_acpt_frmt_t frmt);
jl_ret_t jl51xx_vlan_acpt_frmt_get(jl_uint32 port, jl_vlan_acpt_frmt_t *pfrmt);

#ifdef __cplusplus
}
#endif

#endif /* __JL51XX_DRV_VLAN_H__ */
