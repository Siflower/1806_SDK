/**
 * @file qos.h
 * @brief QoS
 */

/**
 * @defgroup qos_group QoS
 * @{
 */

/**
 * @example example_qos_dscp_to_queue_map.c
 * @example example_qos_exp_to_queue_map.c
 * @example example_qos_pcp_to_queue_map.c
 * @example example_qos_schedule.c
 * @example example_qos_enqueue.c
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


#ifndef __JL_API_QOS_H__
#define __JL_API_QOS_H__

#include "jl_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* QOS_QUEUE_NUM
*/
#define QOS_QUEUE_NUM  (4)
/**
* QOS_MULTICAST_RES_MANAGEMENT_ID
*/
#define QOS_MULTICAST_RES_MANAGEMENT_ID (10)
/**
* QOS_RES_CELLS
*/
#define QOS_RES_CELLS (912)

/**
 * @enum qos_l3_type
 * @brief L3 packet type
 */
enum qos_l3_type {
	QOS_L3_IPV4 = 0, /**< 0 */
	QOS_L3_IPV6,    /**< 1 */
	QOS_L3_MPLS,    /**< 2 */
	QOS_L3_UNKNOWN, /**< 3 */
};

/**
 * @enum qos_sche_type
 * @brief queue schedule type
 */
enum qos_sche_type {
	QOS_SCHE_SP = 0, /**< 0 */
	QOS_SCHE_DWRR,    /**< 1 */
	QOS_SCHE_UNKNOWN, /**< 2 */
};

/**
 * @enum qos_res_mode_e
 * @brief queue resource mode
 */
enum qos_res_mode_e {
	QOS_RES_LIMITER = 0, /**< 0 */
	QOS_RES_GUARANTEE, /**< 1 */
	QOS_RES_UNKNOWN /**< Invalid */
};

/**
 * @struct jl_qos_schedule_s
 * @brief Queue schedule type (e.g. SP or DWRR)
 */
struct jl_qos_schedule_s {
	enum qos_sche_type type[QOS_QUEUE_NUM]; /**< Queue schedule type*/
};

/**
 * @public typedef jl_qos_schedule_t
 */
typedef struct jl_qos_schedule_s jl_qos_schedule_t;

/**
 * @struct qos_l3_ipv4
 * @brief config tos and queue priority for ipv4 packet
 */
struct qos_l3_ipv4 {
	jl_uint32 tos;     /**< Type Of Service (TOS), from TOS/ Class of service in the IPv4 header,range[0,255]  */
	jl_uint32 priority;/**< queue priority,range[0,3] */
};

/**
 * @struct qos_l3_ipv6
 * @brief config tos and queue priority for ipv6 packet
 */
struct qos_l3_ipv6 {
	jl_uint32 tos;      /**< Type Of Service (TOS), from TOS/ Class of service in the IPv6 header,range[0,255]  */
	jl_uint32 priority; /**< queue priority,range[0,3] */
};

/**
 * @struct qos_l3_mpls
 * @brief config exp and queue priority for mpls packet
 */
struct qos_l3_mpls {
	jl_uint32 exp;     /**< exp value,from packet MPLS EXP priority bits,range[0,7]  */
	jl_uint32 priority;/**< queue priority,range[0,3] */
};

/**
 * @struct qos_l3_unknown
 * @brief If a packet does not contain IPv4, IPv6, MPLS or PPPoE carrying IPv4/IPv6 field, there is an option to
force the packet to a certain egress queue.
 */
struct qos_l3_unknown {
	jl_uint8 force_en;  /**< enable bit. If set, the packet shall have a forced queue priority.  */
	jl_uint32 priority; /**< queue priority,range[0,3] */
};

/**
 * @union qos_l3_prio_u
 * @brief qos l3 priority union structure.
 */
union qos_l3_prio_u {
		struct qos_l3_ipv4 ipv4;     /**< config tos and queue priority mapping for ipv4 packet*/
		struct qos_l3_ipv6 ipv6;     /**< config tos and queue priority mapping for ipv6 packet*/
		struct qos_l3_mpls mpls;     /**< config exp and queue priority mapping for mpls packet*/
		struct qos_l3_unknown unknown;/**< option to force packet to a certain queue priority*/
};

/**
 * @struct qos_l3_config
 * @brief The priority of L3 packet map to queue priority.
 */
struct qos_l3_config {
	enum qos_l3_type type; /**< select l3 packet priority mapping type.  */
	union qos_l3_prio_u qos_l3;/**< config l3 packet priority mapping to queue priority.  */
};

/**
 * @struct qos_port_config
 * @brief config default pcp in source port table.
 */
struct qos_port_config {
	jl_uint32 pcp_priority;/**< port default pcp ,range[0,7].  */
};

/**
 * @struct qos_vlan_config
 * @brief config mapping table from VLAN PCP priority bits to queue priority.
 */
struct qos_vlan_config {
	jl_uint16 pcp;            /**< Priority code point(PCP), from the outermost VLAN tag or from port default pcp for untagged packet ,range[0,7].  */
	jl_uint32 vlan_priority;  /**< config pcp mapping to queue priority.  */
};

/**
 * @brief Set mapping table from L3 packet priorities to queue priority
 * @details  This function sets the packet priority mapping from priority field of L3 to queue priority.
 * @param[in]  config    Packet priority assignment based on L3
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 * @deprecated use \ref jl_qos_l3_dscp2queue_map_set() \ref jl_qos_l3_exp2queue_map_set().
 */
attribute_deprecated
JL_API jl_api_ret_t jl_qos_l3_set(struct qos_l3_config *config);

/**
 * @brief Get mapping table from L3 packet priorities to queue priority
 * @details  This function gets the packet priority mapping from priority field of L3 to queue priority.
 * @param[in,out]  config    Packet priority assignment based on L3
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 * @deprecated use \ref jl_qos_l3_dscp2queue_map_get() \ref jl_qos_l3_exp2queue_map_get().
 */
attribute_deprecated
JL_API jl_api_ret_t jl_qos_l3_get(struct qos_l3_config *config);

/**
 * @brief Set enable or disable l3 packet priority assignment
 * @details  This function can enable or disable packet queue priority assignment based on L3 protocol in the specified port.
 * @param[in]  port      Port ID
 * @param[in]  enable    Enable or disable queue priority assignment based on L3 packet in the specified port.
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 * @deprecated use \ref jl_qos_l3_queue_assign_enable_set().
 */
attribute_deprecated
JL_API jl_api_ret_t jl_qos_l3_port_enable_set(jl_port_t port, jl_uint8 enable);

/**
 * @brief Get l3 packet priority assignment status
 * @details  This function can get enable status of packet queue priority assignment based on L3 protocol in the specified port.
 * @param[in]   port      Port ID
 * @param[out]  enable    Enable status.
 * @return Set successfully or not
 * 		@retval JL_ERR_OK		Ok
 * 		@retval minus_value		Fail
 * @deprecated use \ref jl_qos_l3_queue_assign_enable_get().
 */
attribute_deprecated
JL_API jl_api_ret_t jl_qos_l3_port_enable_get(jl_port_t port, jl_uint8 *enable);

/**
 * @brief Set the Port Default PCP
 * @details  This function sets the Port Default PCP for the specified port.
 * @param[in]  port      Port ID
 * @param[in]  config    The Port Default PCP.
 * @return Set successfully or not
 * 		@retval JL_ERR_OK		Ok
 * 		@retval minus_value		Fail
 * @deprecated use \ref jl_qos_l2_port_pcp_set().
 */
attribute_deprecated
JL_API jl_api_ret_t jl_qos_port_set(jl_port_t port, struct qos_port_config *config);

/**
 * @brief Get the Port Default PCP
 * @details  This function gets the Port Default PCP for the specified port.
 * @param[in]  port      port ID
 * @param[out] config    The Port Default PCP.
 * @return Set successfully or not
 * 		@retval JL_ERR_OK		Ok
 * 		@retval minus_value		Fail
 * @deprecated use \ref jl_qos_l2_port_pcp_get().
 */
attribute_deprecated
JL_API jl_api_ret_t jl_qos_port_get(jl_port_t port, struct qos_port_config *config);

/**
 * @brief Set PCP to queue Priority Mapping
 * @details  This function sets vlan PCP to queue Priority Mapping.
 * @param[in]  config    The mapping between PCP to queue Priority
 * @return Set successfully or not
 * 		@retval JL_ERR_OK		Ok
 * 		@retval minus_value		Fail
 * @deprecated use \ref jl_qos_l2_pcp2queue_map_set().
 */
attribute_deprecated
JL_API jl_api_ret_t jl_qos_vlan_set(struct qos_vlan_config *config);

/**
 * @brief Get PCP to queue Priority Mapping
 * @details  This function gets vlan PCP to queue Priority Mapping.
 * @param[in,out] config    The mapping between PCP to queue Priority
 * @return Set successfully or not
 * 		@retval JL_ERR_OK		Ok
 * 		@retval minus_value		Fail
 * @deprecated use \ref jl_qos_l2_pcp2queue_map_get().
 */
attribute_deprecated
JL_API jl_api_ret_t jl_qos_vlan_get(struct qos_vlan_config *config);

/* new qos api */

/**
 * @brief Init Qos
 * @details  This function initialize the qos module.
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_qos_init(void);

/**
 * @brief Deinit Qos
 * @details  This function will disable Qos module. All related qos tables will restore to the default values.
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_qos_deinit(void);

/**
 * @brief Set enable or disable L3 based assignment
 * @details  This function can enable or disable queue assignment based on L3 protocol in the specified port.
 * @param[in]  port      Port ID
 * @param[in]  enable    Enable or disable
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_qos_l3_queue_assign_enable_set(jl_port_t port, jl_enable_t enable);

/**
 * @brief Get L3 based assignment status
 * @details  This function can get enable status of queue assignment based on L3 protocol in the specified port.
 * @param[in]  port       Port ID
 * @param[out] penable    Get enable status
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_qos_l3_queue_assign_enable_get(jl_port_t port, jl_enable_t *penable);

/**
 * @brief Set mapping table from DSCP to queue
 * @details  This function sets the mapping table from DSCP to queue.
 * @param[in] dscp     From DSCP in the IPV4 or IPV6 header,range[0,63]
 * @param[in] queue    Queue id, range[0,3]
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_qos_l3_dscp2queue_map_set(jl_uint32 dscp, jl_uint32 queue);

/**
 * @brief Get mapping table from DSCP to queue
 * @details  This function gets the mapping table from DSCP to queue.
 * @param[in]  dscp      From DSCP in the IPV4 or IPV6 header,range[0,63]
 * @param[out] pqueue    Queue id
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_qos_l3_dscp2queue_map_get(jl_uint32 dscp, jl_uint32 *pqueue);

/**
 * @brief Set mapping table from exp to queue
 * @details  This function sets the mapping table from exp to queue.
 * @param[in]  exp       From packet MPLS EXP priority bits,range[0,7]
 * @param[in]  queue     Queue id,range[0,3]
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_qos_l3_exp2queue_map_set(jl_uint32 exp, jl_uint32 queue);

/**
 * @brief Get mapping table from exp to queue
 * @details  This function gets the mapping table from exp to queue.
 * @param[in]   exp       From packet MPLS EXP priority bits,range[0,7]
 * @param[out]  pqueue    Queue id
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_qos_l3_exp2queue_map_get(jl_uint32 exp, jl_uint32 *pqueue);

/**
 * @brief Set mapping table from pcp to queue
 * @details  This function sets the mapping table from vlan pcp to queue.
 * @param[in]  vlan_pcp    Priority code point(PCP),from the outermost VLAN tag or from port default pcp for untagged packet,range[0,7]
 * @param[in]  queue       Queue id,range[0,3]
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_qos_l2_pcp2queue_map_set(jl_uint32 vlan_pcp, jl_uint32 queue);

/**
 * @brief Get mapping table from pcp to queue
 * @details  This function gets the mapping table from vlan pcp to queue.
 * @param[in]   vlan_pcp    Priority code point(PCP), from the outermost VLAN tag or from port default pcp for untagged packet, range[0,7]
 * @param[out]  pqueue      Queue id
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_qos_l2_pcp2queue_map_get(jl_uint32 vlan_pcp, jl_uint32 *pqueue);

/**
 * @brief Set the Port Default PCP
 * @details  This function sets the Port Default PCP for the specified port.
 * @param[in]  port    Port ID
 * @param[in]  pcp     The Port Default PCP
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_qos_l2_port_pcp_set(jl_port_t port, jl_uint32 pcp);

/**
 * @brief Get the Port Default PCP
 * @details  This function gets the Port Default PCP for the specified port.
 * @param[in]   port    Port ID
 * @param[out]  ppcp    The Port Default PCP
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_qos_l2_port_pcp_get(jl_port_t port, jl_uint32 *ppcp);

/**
 * @brief Set mapping queue to priority
 * @details  This function sets mapping queue to priority. Each queue is mapped to one of four egress priorities.
 * @param[in]  port        Port ID
 * @param[in]  queue       Queue id,range[0,3]
 * @param[in]  priority    Queue priority,range[0,3]
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 * @note   The priorities are ranked in descending order, from the highest priority (zero), to the lowest (three).
 *         Strict priority scheduling is performed between different priorities.
 * @deprecated use \ref jl_qos_schedule_set().
 */
attribute_deprecated
JL_API jl_api_ret_t jl_qos_queue_pri_set(jl_port_t port, jl_uint32 queue, jl_uint32 priority);

/**
 * @brief Get mapping queue to priority
 * @details  This function gets mapping queue to priority.
 * @param[in]   port         Port ID
 * @param[in]   queue        Queue id,range[0,3]
 * @param[out]  ppriority    Queue priority
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_qos_queue_pri_get(jl_port_t port, jl_uint32 queue, jl_uint32 *ppriority);

/**
 * @brief Set queue's DWRR weight
 * @details  This function sets queue's dwrr weight.
 * @param[in]  port      Port ID
 * @param[in]  queue     Queue id,range[0,3]
 * @param[in]  weight    The relative weight of the queue,range[0,127]
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 * @note  DWRR scheduling is performed among the same priorities.
 */
JL_API jl_api_ret_t jl_qos_queue_dwrr_set(jl_port_t port, jl_uint32 queue, jl_uint32 weight);

/**
 * @brief Get queue's DWRR weight
 * @details  This function gets queue's dwrr weight.
 * @param[in]   port       Port ID
 * @param[in]   queue      Queue id,range[0,3]
 * @param[out]  pweight    The relative weight of the queue
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_qos_queue_dwrr_get(jl_port_t port, jl_uint32 queue, jl_uint32 *pweight);

/**
 * @brief Set queue's schedule type
 * @details Set queue's schedule type, type SP or DWRR.
 * @param[in]	port		   		Egress Port Id
 * @param[in]	psche				Schedule Type Info
 * @return Set queue's schedule type successfully or not
 * 		@retval JL_ERR_OK		Ok
 * 		@retval JL_ERR_INIT		Check Switch State Fail
 * 		@retval JL_ERR_PORT		Port Error
 * 		@retval JL_ERR_PARAM		Param Error
 * 		@retval JL_ERR_CONFIG	Config Error
 * 		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 * 		@retval JL_ERR_FAIL		Read/Write Reg Fail
 * @note	Queue 0 to 3 in SP scheduler follows strict priority from highest to lowest. 
 *	In addition, Queues in DWRR scheduler have lower priority than those in SP.
 */
JL_API jl_api_ret_t jl_qos_schedule_set(jl_uint8 port, jl_qos_schedule_t *psche);
/**
 * @brief Get queue's schedule type
 * @details Get queue's schedule type, type SP or DWRR. 
 * @param[in]	port		   		Egress Port Id
 * @param[out]	psche				Schedule Type Info
 * @return Get queue's schedule type successfully or not
 * 		@retval JL_ERR_OK		Ok
 * 		@retval JL_ERR_INIT		Check Switch State Fail
 * 		@retval JL_ERR_PORT		Port Error
 * 		@retval JL_ERR_PARAM		Param Error
 * 		@retval JL_ERR_CONFIG	Config Error
 * 		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 * 		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_schedule_get(jl_uint8 port, jl_qos_schedule_t *psche);

/**
 * @brief Set queue's enable state
 * @details Set enabled queue on each port
 * @param[in]	port		Egress Port Id
 * @param[in]	enmask		Enabled Queue Bitmask, bit 0-3 represent to queue 0-3
 * @return Set queue's enable state successfully or not
 * 		@retval JL_ERR_OK		Ok
 * 		@retval JL_ERR_INIT		Check Switch State Fail
 * 		@retval JL_ERR_PORT		Port Error
 * 		@retval JL_ERR_PARAM		Param Error
 * 		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 * 		@retval JL_ERR_FAIL		Read/Write Reg Fail
 * @deprecated not in use
 */
attribute_deprecated
JL_API jl_api_ret_t jl_qos_enqueue_set(jl_uint8 port, jl_uint8 enmask);

/**
 * @brief Get queue's enable state
 * @details Get enabled queue on each port
 * @param[in]	port		Egress Port Id
 * @param[out]	penmask		Enabled Queue Bitmask, bit 0-3 represent to queue 0-3
 * @return Get queue's enable state successfully or not
 * 		@retval JL_ERR_OK		Ok
 * 		@retval JL_ERR_INIT		Check Switch State Fail
 * 		@retval JL_ERR_PORT		Port Error
 * 		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 * 		@retval JL_ERR_FAIL		Read/Write Reg Fail
 * @deprecated not in use
 */
attribute_deprecated
JL_API jl_api_ret_t jl_qos_enqueue_get(jl_uint8 port, jl_uint8 *penmask);

#ifdef __cplusplus
}
#endif

#endif /* __JL_API_QOS_H__*/
/** @} QoS APIs*/

