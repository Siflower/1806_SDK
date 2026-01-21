/**
 * @file vlan.h
 * @brief vlan
 */

/**
 * @defgroup vlan_group Vlan
 * @{
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

#ifndef __JL_API_VLAN_H__
#define __JL_API_VLAN_H__

#include "jl_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @example example_vlan_mtu.c
 */

/**
 * @enum vlan_assignment
 * @brief Controls how a packets Ingress VID is assigned
 * @note The default vlaue is VLAN_ASSIGN_PKT. If vlan_assignment is VLAN_ASSIGN_PKT and the incoming packet doesn't have that header, then
 * this table entry's defaultVid will be used.
 */
enum vlan_assignment {
	VLAN_ASSIGN_PKT = 0, /**< the Ingress VID is assigned from the incoming packets outermost VLAN header  */
	VLAN_ASSIGN_PORT = 1,/**< the packets Ingress VID is assigned from this table entry's defaultVid  */
	VLAN_ASSIGN_MIXED = 2,/**< if there are two VLANs in the incoming packet, the inner VLAN is chosen. If the incoming packet has only 0 or 1 VLAN, then it will select this table entry's defaultVid */
};

/**
 * @enum vlan_operation
 * @brief A number of operations that can be performed on the packet's VLAN headers such as push/pop etc
 */
enum vlan_operation {
	VLAN_OPERATION_NONE = 0,/**< No operation */
	VLAN_OPERATION_SWAP = 1,/**< The outermost VLAN header in the packet is replaced, the new VID comes from the ingress port Port VID */
	VLAN_OPERATION_PUSH = 2,/**< A new VLAN header is added to the packet before any previous VLANs, the new VID comes from the ingress port Port VID*/
	VLAN_OPERATION_POP = 3, /**< The outermost VLAN header in the packet is removed */
	VLAN_OPERATION_REMOVE_ALL = 4, /**< All VLAN headers are removed from the packet. */
	VLAN_OPERATION_END /**< enum delimiter */
};

/**
 * @public typedef vlan_operation_t
 */
typedef enum vlan_operation vlan_operation_t;

 /**
  * @enum jl_vlan_acpt_frmt_e
  * @brief The vlan accept frame type on the specified port
  */

enum jl_vlan_acpt_frmt_e {
	VLAN_ACCEPT_FRAME_TYPE_ALL = 0, /**< accept all frames  */
	VLAN_ACCEPT_FRAME_TYPE_TAG_ONLY, /**< only accept tagged frames */
	VLAN_ACCEPT_FRAME_TYPE_UNTAG_ONLY,/**< only accept untagged frames */
	VLAN_ACCEPT_FRAME_TYPE_END/**< enum delimiter */
};

/**
 * @public typedef jl_vlan_acpt_frmt_t
 */
typedef enum jl_vlan_acpt_frmt_e jl_vlan_acpt_frmt_t;

/**
  * @enum jl_vlanid_inherit_select_e
  * @brief Selects which VID to use when building a new VLAN header in a push or swap operation. 
  *        If the selected VLAN header does not exist in the packet then table entry's vid will be used.
  */
enum jl_vlanid_inherit_select_e {
	VLANID_INHERIT_FROM_ORIGINAL_OUTERMOST = 0,  /**<From the outermost VLAN in the original packet (if any) */
	VLANID_INHERIT_FROM_TABLE_VALUE,           /**<From this table entry's vid */
	VLANID_INHERIT_FROM_ORIGINAL_SECOND_VLAN,  /**<From the second VLAN in the original packet (if any) */
	VLANID_INHERIT_FROM_END /**< enum delimiter */
};

/**
 * @public typedef jl_vlanid_inherit_select_e
 */
typedef enum jl_vlanid_inherit_select_e jl_vlanid_inherit_select_t;

/**
 * @struct vlan_config
 * @brief vlan configuration
 * @note "port_mask" means the ports in Vlan group, one bit represents one port, bit0 -> port0, bit1->port1...
 */
struct vlan_config {
	jl_uint8 valid;/**< Valid flag of VLAN table entry, if set 1, means this vlan is valid */
	jl_uint16 vid;/**< VLAN ID */
	jl_uint32 port_mask;/**< Ports in VLAN group */
	jl_uint32 fid;/**< Filtering Identifier, used for SVL/IVL, range[0, 14] */
	vlan_operation_t operation; /**<The ingress VLAN operation to perform on the packet,default is none */
	jl_vlanid_inherit_select_t vid_sel; /**< Selects which vid to use when building a new vlan tag if operation is push or swap */
	jl_uint32 new_vid; /**<The vid used in VLAN push or swap operation if vid_sel is set VLANID_INHERIT_FROM_TABLE_VALUE */
};

/**
 * @struct vlan_item
 * @brief Vlan table information
 */
struct vlan_item {
	jl_uint16 index;/**< Entry index of VLAN table  */
	struct vlan_config config;/**< VLAN configuration used in VLAN table  */
};

/**
 * @brief Init Vlan
 * @details This function initialize the vlan module. A default VLAN with VID==1 and contains all ports(PVID==1) will be created.
 * @return Init vlan successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 * @see		jl_error.h
 */
JL_API jl_api_ret_t jl_vlan_init(void);

/**
 * @brief Deinit Vlan
 * @details This function will reset and disable VLAN module
 * @return Deinit vlan successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail

 */
JL_API jl_api_ret_t jl_vlan_deinit(void);

/**
 * @brief Get the VLAN assignment for the specified port.
 * @details the function Gets how a packets Ingress VID is assigned for the specified port.
 * @param[in] port: Port number
 * @param[out] opt: vlan assignment
 * @return Get successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_port_vlan_assignment_get(jl_port_t port, enum vlan_assignment *opt);

/**
 * @brief Set the VLAN assignment for the specified port.
 * @details  the function Controls how a packets Ingress VID is assigned for the specified port.
 * @param[in] port: Port number
 * @param[in] opt:  vlan assignment
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 * @note If the opt is VLAN_ASSIGN_PKT and the incoming packet doesn't have that header, then
 * this table entry's defaultVid will be used.
 */
JL_API jl_api_ret_t jl_port_vlan_assignment_set(jl_port_t port, enum vlan_assignment opt);

/**
 * @brief Get the port Default VLAN ID
 * @param[in] port: Port number
 * @param[out] vid: VLAN ID
 * @return Get successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_port_vlan_get(jl_port_t port, jl_uint32 *vid);

/**
 * @brief Set the port Default VLAN ID
 * @param[in] port: Port number
 * @param[in] vid: VLAN ID
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_port_vlan_set(jl_port_t port, jl_uint32 vid);

/**
 * @brief Get the packets VLAN header updating operation at the specified port and direction.
 * @details The function get the operation that was performed on the packet's VLAN headers such as push/pop/swap etc
 * @param[in] port: Port number
 * @param[in] dir: 0 = port ingress direction , 1 = port egress direction
 * @param[out] opt: vlan_operation
 * @return Get successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_port_vlan_operation_get(jl_port_t port, jl_port_dir_t dir, enum vlan_operation *opt);

/**
 * @brief Set the packets VLAN header updating operation at the specified port and direction.
 * @param[in] port: Port number
 * @param[in] dir: 0 = port ingress direction, 1 = port egress direction
 * @param[in] opt: vlan_operation
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 * 		@retval minus_value		Fail
 * @note for port egress direction, not support Swap/Push operation
 */
JL_API jl_api_ret_t jl_port_vlan_operation_set(jl_port_t port, jl_port_dir_t dir, enum vlan_operation opt);

/**
 * @brief Get a VLAN Group configuration from a specified VLAN table entry
 * @param[in] vlan_index: Entry index of VLAN table, range [0,14]
 * @param[out] config: vlan_config
 * @return Get successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_vlan_get(jl_uint32 vlan_index, struct vlan_config *config);

/**
 * @brief Set a VLAN Group configuration to a specified VLAN table entry
 * @details This function sets a valid/invalid VLAN Group configuration into the specified VLAN table entry. VLAN table entry 0 stores Default VLAN configuration, be careful to modify this entry.
 * @param[in] vlan_index: Entry index of VLAN table,range [0,14]
 * @param[in] config: vlan_config
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 * @note By default, all vlan entries owe the same FID=0. It is called SVL(Shared Vlan Learning).
 * If Vlan entries are configured to different FIDs, then the same source MAC Addr with multiple FIDs can be learned into different look-up table entries. It is called IVL(Independent Vlan Learning).
 */
JL_API jl_api_ret_t jl_vlan_set(jl_uint32 vlan_index, struct vlan_config *config);

/**
 * @brief Set a batch VLAN Group configurations into specified VLAN table entries
 * @param[in] v_list: vlan_item
 * @param[in] size: The total configuration entries in the list
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_vlan_set_batch(struct vlan_item *v_list, jl_uint16 size);

/**
 * @brief Set the VLAN accept frame type at the specified port, by default accept all
 * @param[in] port: Port number
 * @param[in] frmt: jl_vlan_acpt_frmt_e
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_vlan_acpt_frmt_set(jl_port_t port, jl_vlan_acpt_frmt_t frmt);

/**
 * @brief Get the VLAN accept frame type at the specified port
 * @param[in] port Port number
 * @param[out] pfrmt jl_vlan_acpt_frmt_e
 * @return Get successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_vlan_acpt_frmt_get(jl_port_t port, jl_vlan_acpt_frmt_t *pfrmt);

#ifdef __cplusplus
}
#endif

#endif /* __JL_API_VLAN_H__*/
/** @} Vlan APIs*/

