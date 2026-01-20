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

#ifndef __JL51XX_DRV_QOS_H__
#define __JL51XX_DRV_QOS_H__

#include "jl_error.h"
#include "qos.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct jl_qos_res_mng_s{
    jl_uint8 mng_mode[11]; /**< Management mode on each port*/
	jl_uint8 grt_pointer[11]; /**< Guarantee pointer on each port*/
	jl_uint8 guarantee[4]; /**< Guarantee resource setting */
	jl_uint16 global_margin; /**< Global margin resource */
	jl_uint16 ffa_margin; /**< FFA margin resource */
	jl_uint8 en_mask[JL_PORT_MAX]; /**< Enabled queue bitmask */
} jl_qos_res_mng_t;


jl_ret_t jl51xx_qos_l3_get(struct qos_l3_config *config);
jl_ret_t jl51xx_qos_l3_set(struct qos_l3_config *config);
jl_ret_t jl51xx_qos_l3_port_enable_get(jl_port_t port, jl_uint8 *enable);
jl_ret_t jl51xx_qos_l3_port_enable_set(jl_port_t port, jl_uint8 enable);

jl_ret_t jl51xx_qos_port_get(jl_port_t port, struct qos_port_config *config);
jl_ret_t jl51xx_qos_port_set(jl_port_t port, struct qos_port_config *config);
jl_ret_t jl51xx_qos_vlan_get(struct qos_vlan_config *config);
jl_ret_t jl51xx_qos_vlan_set(struct qos_vlan_config *config);

/*new qos api*/
jl_ret_t jl51xx_qos_table_init(void);
jl_ret_t jl51xx_qos_init(void);
jl_ret_t jl51xx_qos_deinit(void);

jl_ret_t jl51xx_qos_buf_manage_set(jl_port_t port, jl_uint32 mode);
jl_ret_t jl51xx_qos_buf_manage_get(jl_port_t port, jl_uint32 *pmode);
jl_ret_t jl51xx_qos_queue_guarantee_set(jl_uint32 index, jl_uint32 cells);
jl_ret_t jl51xx_qos_queue_guarantee_get(jl_uint32 index, jl_uint32 *pcells);

jl_ret_t jl51xx_qos_l3_queue_assign_enable_set(jl_port_t port, jl_enable_t enable);
jl_ret_t jl51xx_qos_l3_queue_assign_enable_get(jl_port_t port, jl_enable_t *penable);

jl_ret_t jl51xx_qos_l3_dscp2queue_map_set(jl_uint32 dscp, jl_uint32 queue);
jl_ret_t jl51xx_qos_l3_dscp2queue_map_get(jl_uint32 dscp, jl_uint32 *pqueue);

jl_ret_t jl51xx_qos_l3_exp2queue_map_set(jl_uint32 exp, jl_uint32 queue);
jl_ret_t jl51xx_qos_l3_exp2queue_map_get(jl_uint32 exp, jl_uint32 *pqueue);

jl_ret_t jl51xx_qos_l2_pcp2queue_map_set(jl_uint32 vlan_pcp, jl_uint32 queue);
jl_ret_t jl51xx_qos_l2_pcp2queue_map_get(jl_uint32 vlan_pcp, jl_uint32 *pqueue);

jl_ret_t jl51xx_qos_l2_port_pcp_set(jl_port_t port, jl_uint32 pcp);
jl_ret_t jl51xx_qos_l2_port_pcp_get(jl_port_t port, jl_uint32 *ppcp);

jl_ret_t jl51xx_qos_queue_pri_set(jl_port_t port, jl_uint32 queue, jl_uint32 priority);
jl_ret_t jl51xx_qos_queue_pri_get(jl_port_t port, jl_uint32 queue, jl_uint32 *ppriority);

jl_ret_t jl51xx_qos_queue_dwrr_set(jl_port_t port, jl_uint32 queue, jl_uint32 weight);
jl_ret_t jl51xx_qos_queue_dwrr_get(jl_port_t port, jl_uint32 queue, jl_uint32 *pweight);


jl_ret_t jl51xx_qos_schedule_set(jl_uint8 port, jl_qos_schedule_t *psche);
jl_ret_t jl51xx_qos_schedule_get(jl_uint8 port, jl_qos_schedule_t *psche);

jl_ret_t jl51xx_qos_enqueue_set(jl_uint8 port, jl_uint8 enmask);
jl_ret_t jl51xx_qos_enqueue_get(jl_uint8 port, jl_uint8 *penmask);

#ifdef __cplusplus
}
#endif

#endif /* __JL51XX_DRV_QOS_H__ */
