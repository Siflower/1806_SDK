/**
 * @file mib.h
 * @brief MIB counter
 */

/**
 * @defgroup mib_group MIB
 * @{
 */

/**
 * @example example_mib_port_cnt.c
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

#ifndef __JL_API_MIB_H__
#define __JL_API_MIB_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @public typedef jl_mib_counter_t
 */
typedef jl_uint64 jl_mib_counter_t;

/**
 * @enum jl_port_mib_type_u
 * @brief port MIB counter type.
 */
enum jl_port_mib_type_u {
	PORT_MIB_TX_PKT_CNT = 0,/**< 0 */
	PORT_MIB_TX_PKT_BYTE_CNT,/**< 1 */
	PORT_MIB_TX_PKT_UC_CNT,/**< 2 */
	PORT_MIB_TX_PKT_UC_BYTE_CNT,/**< 3 */
	PORT_MIB_TX_PKT_MC_CNT,/**< 4 */
	PORT_MIB_TX_PKT_MC_BYTE_CNT,/**< 5 */
	PORT_MIB_TX_PKT_BC_CNT,/**< 6 */
	PORT_MIB_TX_PKT_BC_BYTE_CNT,/**< 7 */
	PORT_MIB_TX_PKT_DRP_CNT,/**< 8 */
	PORT_MIB_TX_PKT_ERR_CNT,/**< 9 */

	PORT_MIB_RX_PKT_CNT,/**< 10 */
	PORT_MIB_RX_PKT_BYTE_CNT,/**< 11 */
	PORT_MIB_RX_PKT_UC_CNT,/**< 12 */
	PORT_MIB_RX_PKT_UC_BYTE_CNT,/**< 13 */
	PORT_MIB_RX_PKT_MC_CNT,/**< 14 */
	PORT_MIB_RX_PKT_MC_BYTE_CNT,/**< 15 */
	PORT_MIB_RX_PKT_BC_CNT,/**< 16 */
	PORT_MIB_RX_PKT_BC_BYTE_CNT,/**< 17 */
	PORT_MIB_RX_PKT_DRP_CNT,/**< 18 */
	PORT_MIB_RX_PKT_CRC_ERR_CNT,/**< 19 */
	PORT_MIB_RX_FRAG_ERR_CNT,/**< 20 */

	PORT_MIB_TYPE_END/**< Invalid */
};

/**
 * @public typedef jl_port_mib_type_t
 */
typedef enum jl_port_mib_type_u jl_port_mib_type_t;

/**
 * @struct jl_port_mib_cnt_s
 * @brief port MIB counters.
 */
struct jl_port_mib_cnt_s {
	jl_uint32 rc_enable;/**< Indicates read clear mode */
	jl_uint64 tx_pkt_cnt;/**< Tx packet counter */
	jl_uint64 tx_pkt_byte_cnt;/**< Tx packet byte counter */
	jl_uint64 tx_pkt_uc_cnt;/**< Tx unicast packet counter */
	jl_uint64 tx_pkt_uc_byte_cnt;/**< Tx unicast packet byte counter */
	jl_uint64 tx_pkt_mc_cnt;/**< Tx mulicast packet counter */
	jl_uint64 tx_pkt_mc_byte_cnt;/**< Tx mulicast packet byte counter */
	jl_uint64 tx_pkt_bc_cnt;/**< Tx broadcast packet counter */
	jl_uint64 tx_pkt_bc_byte_cnt;/**< Tx broadcast packet byte counter */
	jl_uint32 tx_pkt_drp_cnt;/**< Tx drop packet counter */
	jl_uint32 tx_pkt_err_cnt;/**< Tx error packet counter */

	jl_uint64 rx_pkt_cnt;/**< Rx packet counter */
	jl_uint64 rx_pkt_byte_cnt;/**< Rx packet byte counter */
	jl_uint64 rx_pkt_uc_cnt;/**< Rx unicast packet counter */
	jl_uint64 rx_pkt_uc_byte_cnt;/**< Rx unicast packet byte counter */
	jl_uint64 rx_pkt_mc_cnt;/**< Rx multicast packet counter */
	jl_uint64 rx_pkt_mc_byte_cnt;/**< Rx multicast packet byte counter */
	jl_uint64 rx_pkt_bc_cnt;/**< Rx broacast packet counter */
	jl_uint64 rx_pkt_bc_byte_cnt;/**< Rx broadcast packet byte counter */
	jl_uint32 rx_pkt_drp_cnt;/**< Rx drop packet counter */
	jl_uint32 rx_pkt_crc_err_cnt;/**< Rx CRC error packet counter */
	jl_uint32 rx_frag_err_cnt;/**< Rx fragmentation error counter */
};

/**
 * @public typedef jl_port_mib_cnt_t
 */
typedef struct jl_port_mib_cnt_s jl_port_mib_cnt_t;


/**
 * JL_MIB_CHECK_TYPE(type)
 */
#define JL_MIB_CHECK_TYPE(type)		\
	do {								\
		if (type >= PORT_MIB_TYPE_END)	\
			return JL_ERR_PARAM;		\
	} while(0)


/**
 * @brief Get port read-clear attribute of the MIB counter
 * @details This function gets port read-clear attribute of the MIB counter.
 * @param[in]	port	Index of port number
 * @param[out]	penable	MIB counter read-clear enable or disable
 * @return get port read-clear attribute of the MIB counter successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_mib_port_rc_mode_get(jl_port_t port, jl_enable_t *penable);

/**
 * @brief Set port read-clear attribute of the MIB counter
 * @details This function globally sets the read-clear attribute of the MIB counter
 * @param[in]	port	Index of port number
 * @param[in]	enable	MIB counter read-clear enable or disable
 * @return set port read-clear attribute of the MIB counter successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_mib_port_rc_mode_set(jl_port_t port, jl_enable_t enable);

/**
 * @brief Clears port MIB counter of the specified type
 * @details This function clears port MIB counter of the specified type
 * @param[in]	port	Index of port number
 * @param[in]	type	The type of MIB rule
 * @return clears port MIB counter of the specified type successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_mib_port_clear(jl_port_t port, jl_port_mib_type_t type);

/**
 * @brief Clears port all type MIB counter
 * @details This function clears port all type MIB counter.
 * @param[in]	port	Index of port number
 * @return clears port all types of MIB counter successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_mib_port_clear_all(jl_port_t port);

/**
 * @brief Get port MIB counter as specified type
 * @details This function gets the port MIB counter as specified type.
 * @param[in]	port	Index of port number
 * @param[in]	type	The type of MIB counter
 * @param[out]	pcnt	MIB counter value
 * @return get port MIB counter of the specified type successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_mib_port_get(jl_port_t port, jl_port_mib_type_t type, jl_mib_counter_t *pcnt);

/**
 * @brief Get all kinds of MIB counters of the specified port
 * @details This function gets all kinds of MIB counters of specified port.
 * @param[in]	port		Index of port number
 * @param[out]	pport_cnt	All kinds of MIB counter of a port
 * @return get all kinds of MIB counters of the specified port successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_mib_port_get_all(jl_port_t port, jl_port_mib_cnt_t *pport_cnt);

/**
 * @struct jl_mib_swc_glb_drop_s
 * @brief Swcore MIB global drop counters.
 */
/**
 * @public typedef jl_mib_swc_glb_drop_t
 */
typedef struct jl_mib_swc_glb_drop_s {
	jl_uint32 ipp_broken_pkt;/**< IPP Broken Packets */
	jl_uint32 ipp_pm_drop;/**< IPP PM Drop */
	jl_uint32 epp_pm_drop;/**< EPP PM Drop */
	jl_uint32 unknown_ingress_drop;/**< Unknown Ingress Drop */
	jl_uint32 empty_mask_drop;/**< Empty Mask Drop */
	jl_uint32 ingress_stp_listen_drop;/**< Ingress Spanning Tree Drop Listen */
	jl_uint32 ingress_stp_learn_drop;/**< Ingress Spanning Tree Drop Learning */
	jl_uint32 ingress_stp_block_drop;/**< Ingress Spanning Tree Drop Blocking */
	jl_uint32 l2_lookup_drop;/**< L2 Lookup Drop */
	jl_uint32 ingress_config_acl_drop;/**< Ingress Configurable ACL Drop */
	jl_uint32 vlan_member_drop;/**< VLAN Member Drop */
	jl_uint32 mini_allow_vlan_drop;/**< Minimum Allowed VLAN Drop */
	jl_uint32 max_allow_vlan_drop;/**< Maximum Allowed VLAN Drop */
	jl_uint32 buf_overflow_drop;/**< Buffer Overflow Drop */
	jl_uint32 ingress_res_manager_drop;/**< Ingress Resource Manager Drop */
	jl_uint32 re_queue_overflow_drop;/**< Re-queue Overflow Drop */
} jl_mib_swc_glb_drop_t;

/**
 * @struct jl_mib_swc_port_drop_s
 * @brief Swcore port MIB port drop counters.
 */
/**
 * @public typedef jl_mib_swc_port_drop_t
 */
typedef struct jl_mib_swc_port_drop_s {
	jl_uint32 sp_overflow_drop;/**< SP Overflow Drop */
	jl_uint32 ps_err_underrun;/**< PS Error Counter underrun */
	jl_uint32 ps_err_overflow;/**< PS Error Counter overflow */
	jl_uint32 unknown_egress_drop;/**< Unknown Egress Drop */
	jl_uint32 egress_port_dis_drop;/**< Egress Port Disabled Drop */
	jl_uint32 queue_off_drop;/**< Queue Off Drop */
	jl_uint32 egress_stp_drop;/**< Egress Spanning Tree Drop */
	jl_uint32 mbsc_drop;/**< MBSC Drop */
	jl_uint32 drain_port_drop;/**< Drain Port Drop */
	jl_uint32 mac_rx_broken_pkt;/**< MAC RX Broken Packets */
	jl_uint32 mac_rx_short_pkt_drop;/**< MAC RX Short Packet Drop */
	jl_uint32 egress_res_manage_drop;/**< Egress Resource Management Drop */
} jl_mib_swc_port_drop_t;

/**
 * @brief Get all kinds of swc global drop counters.
 * @details This function gets all kinds of swc global drop counters.
 * @param	pport_cnt	All kinds of global drop counters
 * @return Get all kinds of swc global drop counters successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_mib_swc_global_drop_get(jl_mib_swc_glb_drop_t *pport_cnt);

/**
 * @brief Clear all kinds of swc global drop counters.
 * @details This function gets all kinds of swc global drop counters.
 * @return Clear all kinds of swc global drop counters successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_mib_swc_global_drop_clear(void);

/**
 * @brief Get all kinds of swc drop counters of the specified port
 * @details This function gets all kinds of swc drop counters of the specified port.
 * @param	port		Index of port number
 * @param	pport_cnt	All kinds of swc drop counters of a port
 * @return Get all kinds of swc drop counters of the specified port successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_mib_swc_port_drop_get(jl_port_t port, jl_mib_swc_port_drop_t *pport_cnt);

/**
 * @brief Clear all kinds of swc drop counters of the specified port
 * @details This function gets all kinds of swc drop counters of the specified port.
 * @param	port		Index of port number
 * @return Clear all kinds of swc drop counters of the specified port successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_mib_swc_port_drop_clear(jl_port_t port);
#ifdef __cplusplus
}
#endif
#endif /* __JL_API_MIB_H__ */
/** @} MIB APIs*/
