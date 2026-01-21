/**
 * @file acl.h
 * @brief ACL
 */

/**
 * 
 * @defgroup acl_group ACL
 * @{
 */

/**
 * @example example_acl_set_rule.c
 */

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

#ifndef __JL_API_ACL_H__
#define __JL_API_ACL_H__

#include "jl_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct jl_acl_rule_s
 * @brief ACL rules.
 */
struct jl_acl_rule_s {
	jl_uint8 compare_ethtype;/**< Determines if the EthType field in this entry shall be compared. */
	jl_uint8 type_of_comparison_ethtype;/**< What type of comparison shall be considered as hit */
	jl_uint16 eth_type;/**< Ethernet Type */
	jl_uint8 compare_da;/**< Determines if the DaMac field in this entry shall be compared. */
	jl_uint8 type_of_comparison_da;/**< What type of comparison shall be considered as hit. */
	jl_uint64 da_mac;/**< Destination MAC address */
	jl_uint8 compare_sa;/**< Determines if the SaMac field in this entry shall be compared. */
	jl_uint8 type_of_comparison_sa;/**< What type of comparison shall be considered as hit */
	jl_uint64 sa_mac;/**< Source MAC address. */
	jl_uint8 compare_vid;/**< Determines if the Vid field in this entry shall becompared */
	jl_uint8 type_of_comparison_vid;/**< What type of comparison shall be considered as hit */
	jl_uint16 vid;/**< Compared with the packets VLAN VID after Ingress vlan assignment and Source Port Table VLAN operation. */
	jl_uint8 compare_pcp;/**< Determines if the Pcp field in this entry shall becompared. */
	jl_uint8 type_of_comparison_pcp;/**< What type of comparison shall be considered as hit. */
	jl_uint8 pcp;/**< Compared with the packets VLAN PCP after Source Port Table VLAN operation */
	jl_uint8 compare_dei;/**< Determines if the Dei field in this entry shall becompared */
	jl_uint8 type_of_comparison_dei;/**< What type of comparison shall be considered as hit */
	jl_uint8 dei;/**< Compared with the packets VLAN CFI/DEI after Source Port Table VLAN operation */
	jl_uint8 compare_has_vlans;/**< Determines if the HasVlans field in this entry shall be compared */
	jl_uint8 type_of_comparison_has_vlans;/**< What type of comparison shall be considered as hit */
	jl_uint8 has_vlans;/**< Is there at least one VLAN in the packet. */
	jl_uint8 compare_cstag;/**< Determines if the Cstag field in this entry shall be compared */
	jl_uint8 type_of_comparison_cstag;/**< What type of comparison shall be considered as hit */
	jl_uint8 cstag;/**< Is the outermost VLAN tag a C-tag or S-Tag. If a packet does not have a VLAN or the VLAN was removed
			*due to a pop operation in the source port vlan operation then the value will be set to zero(0). */
	jl_portmask_t ports;/**< Ports that this filter rule applies to.One bit per source port, bit 0 for port 0, bit 1 for port 1 etc. */
};

/**
 * @public typedef jl_acl_rule_t
 */
typedef struct jl_acl_rule_s jl_acl_rule_t;

/**
 * @struct jl_acl_action_s
 * @brief ACL actions.
 */
struct jl_acl_action_s {
	jl_enable_t drop_enable;/**< If set, the packet shall be dropped. */
	jl_enable_t send_to_cpu_enable;/**< If set, the packet shall be sent to the CPU port. */
	jl_enable_t force_queue_enable;/**< If set, the packet shall have a forced egress queue. */
	jl_uint8 queue_id;/**< The egress queue to be assigned if the forceQueue field in this entry is set to 1. */
	jl_enable_t send_to_port_enable;/**< If set,the packet shall be sent to a specific port. */
	jl_port_t dest_port;/**< The port which the packet shall be sent to */
	jl_enable_t update_pcp_enable;/**< If set,the PCP value of the packets outermost VLAN should be updated. */
	jl_uint8 new_pcp_value;/**< The PCP value to update to. */
	jl_enable_t update_vid_enable;/**< If set,the Vid value of the packets outermost VLAN should be updated. */
	jl_uint16 new_vid_value;/**< The Vid value to update to. */
};

/**
 * @public typedef jl_acl_rule_t
 */
typedef struct jl_acl_action_s jl_acl_action_t;

/**
 * @brief Set the ACL filter rules and ACL actions
 * @details This function sets the ACL filter rules and the correspond actions into the ACL table,
 * and applys the ACL rule to the specified ports. There are 32 ACL Rule Entries and 32
 * ACL Action Entries in the ACL table. Each ACL Rule Entry contains several compare
 * fields, and each ACL Action Entry also contains several actions.
 * @param[in]	entry_index	Index of ACL table, range [0,31]
 * @param[in]	acl_rule	ACL rule of this entry
 * @param[in]	acl_action	ACL action of this entry
 * @return set the ACL filter rules and ACL actions successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_acl_set(jl_uint8 entry_index, jl_acl_rule_t *acl_rule, jl_acl_action_t *acl_action);

/**
 * @brief Get the filter rule and action of the entry from the ACL table
 * @param[in]	entry_index	Index of ACL table, range [0,31]
 * @param[out]	acl_rule	ACL rule of this entry
 * @param[out]	acl_action	ACL action of this entry
 * @return get the ACL filter rules and ACL actions successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_acl_get(jl_uint8 entry_index, jl_acl_rule_t *acl_rule, jl_acl_action_t *acl_action);

#ifdef __cplusplus
}
#endif

#endif /* __JL_API_ACL_H__*/
/** @} ACL APIs*/
