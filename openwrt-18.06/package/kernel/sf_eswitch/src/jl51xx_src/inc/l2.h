/**
 * @file l2.h
 * @brief L2 Switching
 */

/**
 * @defgroup l2_switching_group L2 Switching
 * @{
 */

/**
 * @example example_l2_aging_learning.c
 * @example example_l2_del_uni_entry.c
 * @example example_l2_get_mul_entry.c
 * @example example_l2_get_table.c
 * @example example_l2_set_mul_entry.c
 * @example example_l2_set_uni_entry.c
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

#ifndef __JL_API_L2_H__
#define __JL_API_L2_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "jl_types.h"

#define JL_L2_MAC_MASK	((jl_uint64)0xFFFFFFFFFFFFU) /**< JL_L2_MAC_MASK */

/**
 * @public typedef jl_l2_table_entry_idx_t
 */
typedef jl_uint32 jl_l2_table_entry_idx_t;

#define JL_L2_LOOKUP_TABLE_BUCKET_NUM					(4)	/**< JL_L2_LOOKUP_TABLE_BUCKET_NUM */
#define JL_L2_LOOKUP_TABLE_ENTRY_NUM					(512) /**< JL_L2_LOOKUP_TABLE_ENTRY_NUM */
#define JL_L2_LOOKUP_TABLE_SIZE							(2048) /**< JL_L2_LOOKUP_TABLE_SIZE */

#define JL_L2_COLLISION_TABLE_ENTRY_NUM					(16) /**< JL_L2_COLLISION_TABLE_ENTRY_NUM */
#define JL_L2_COLLISION_TABLE_SIZE						(16) /**< JL_L2_COLLISION_TABLE_SIZE */
#define JL_L2_COLLISION_TABLE_MASKS_SIZE				(4) /**< JL_L2_COLLISION_TABLE_MASKS_SIZE */

/**
 * JL_L2_LOOKUP_MAX_ENTRY
 */
#define JL_L2_LOOKUP_MAX_ENTRY \
		(JL_L2_LOOKUP_TABLE_BUCKET_NUM + JL_L2_COLLISION_TABLE_ENTRY_NUM)

#define JL_L2_MC_TABLE_SIZE								(64) /**< JL_L2_MC_TABLE_SIZE */
#define JL_L2_MC_PORT_MASK								(0x3FF) /**< JL_L2_MC_PORT_MASK */

#define JL_L2_DA_TABLE_SIZE								(2064) /**< JL_L2_DA_TABLE_SIZE */
#define JL_L2_DA_COL_TABLE_SIZE							(16) /**< JL_L2_DA_COL_TABLE_SIZE */
#define JL_L2_COL_DA_OFFSET								(2048) /**< JL_L2_COL_DA_OFFSET */

#define JL_L2_AGING_TABLE_SIZE							(2048) /**< JL_L2_AGING_TABLE_SIZE */
#define JL_L2_AGING_COL_TABLE_SIZE						(16) /**< JL_L2_AGING_COL_TABLE_SIZE */

/*
 * We have two types of table:
 * - hash table(hash + 4 ways)
 * - collision table
 *
 * is_collision: to indicate if this key is for collision table.
 * mac_addr    : mac address, this paramenter will be used to calculate hash
 *               code when this key is not for collision table.
 * fid (filtering identifier): ranges from 0 ~ 15.
 * entry_index : represents lookup table index when is not for collision. repesents collisiton table entry index when is for collision.
 */

/**
 * @struct jl_l2_fib_key_s
 * @brief L2 switching fib key configuration.
 */
struct jl_l2_fib_key_s {
	jl_bool is_collision;/**< Indicates whether operate collision table or not */
	jl_mac_addr_t mac_addr;/**< Indicates mac address,
				*this paramenter will be used to calculate hash code when this key is not for collision table */
	jl_uint32 fid;/**< Filtering Identifier, used for SVL/IVL, range[0, 14] */
	jl_uint32 entry_index;/**< Per DA hash table, it is the index of the hash lookup table entry 
				*And per collision, it is the index of collision lookup table entry */
};

/**
 * @public typedef jl_l2_fib_key_t
 */
typedef struct jl_l2_fib_key_s jl_l2_fib_key_t;

/**
 * @struct jl_l2_fib_aging_config_s
 * @brief L2 switching fib aging configuration.
 */
struct jl_l2_fib_aging_config_s {
	jl_bool is_valid;/**< Set the specified aging table entry as valid or not */
	jl_bool is_static;/**< Set the specified aging table entry as static or not */
};

/**
 * @public typedef jl_l2_fib_aging_config_t
 */
typedef struct jl_l2_fib_aging_config_s jl_l2_fib_aging_config_t;

/**
 * @struct jl_l2_fib_dest_config_s
 * @brief L2 switching fib destination port configuration.
 */
typedef struct jl_l2_fib_dest_config_s {
	jl_bool is_unicast;/**< Unicast or not, i.e Multicast */
	jl_uint16 port;/**< L2 MAC table search result. It is destination port number for unicast or port mask table index for multicast.\n For multicast, port represents index which set in \ref jl_l2_mc_port_mask_set */
	jl_bool pkt_drop;/**< drop the packet or not */
} jl_l2_fib_dest_config_t;

/**
 * @public typedef jl_l2_fib_dest_config_t
 */
typedef struct jl_l2_fib_dest_config_s jl_l2_fib_dest_config_t;

/**
 * @struct jl_l2_fib_config_s
 * @brief L2 switching fib integrate aging and dest configuration.
 */
struct jl_l2_fib_config_s {
	jl_l2_fib_aging_config_t aging;/**< L2 FIB aging table configure */
	jl_l2_fib_dest_config_t dest;/**< L2 MAC search result configure */
};

/**
 * @public typedef jl_l2_fib_config_t
 */
typedef struct jl_l2_fib_config_s jl_l2_fib_config_t;

/**
 * @enum jl_l2_aging_unit_e
 * @brief l2 aging unit
 */
enum jl_l2_aging_unit_e {
	L2_AGING_NS,/**< 0 */
	L2_AGING_US,/**< 1 */
	L2_AGING_MS,/**< 2 */
	L2_AGING_S,/**< 3 */
	L2_AGING_END,/**< Invalid */
};

/**
 * @public typedef jl_l2_aging_unit_t
 */
typedef enum jl_l2_aging_unit_e jl_l2_aging_unit_t;

/**
 * @brief Get L2 aging and learning status
 * @details This function gets the global configuration of hardware aging and learning status
 * @param[out] penable_aging	Hardware MAC table aging enabled status
 * @param[out] penable_learning	Hardware MAC table learning enabled status
 * @return Get L2 aging and learning status successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_l2_aging_learning_get(jl_enable_t *penable_aging, jl_enable_t *penable_learning);

/**
 * @brief Set L2 aging and learning status
 * @details This function sets the global configuration of hardware aging and learning
 * @param[in] enable_aging	Hardware MAC table aging enabled status
 * @param[in] enable_learning	Hardware MAC table learning enable status
 * @return Set L2 aging and learning status successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_l2_aging_learning_set(jl_enable_t enable_aging, jl_enable_t enable_learning);

/**
 * @brief Get L2 multicast portmask
 * @details This function gets the value of multicast port mask table which is used in the multicast searching.
 * @param[in]	index		Address of port mask table
 * @param[out]	pport_mask	Value of port mask table
 * @return Get L2 muticast portmask successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_OUT_OF_RANGE	Index out of table range
 */
JL_API jl_api_ret_t jl_l2_mc_port_mask_get(jl_l2_table_entry_idx_t index, jl_uint32 *pport_mask);

/**
 * @brief Set L2 multicast portmask
 * @details This function sets the value of multicast port mask table which is used in the multicast searching.\n
 * Every multicast port mask is a multicast group, the total number of mulicast group is 64.\n
 * Every entry of multicast port mask is 10 bits width corresponds to 10 ports.\n
 * Set to 1 is part of the index specified multicast group.
 * @param[in] index	Index of port mask table
 * @param[in] port_mask	Value of port mask table
 * @return Set L2 muticast portmask successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_OUT_OF_RANGE	Index out of table range
 */
JL_API jl_api_ret_t jl_l2_mc_port_mask_set(jl_l2_table_entry_idx_t index, jl_uint32 port_mask);

/**
 * @brief Get specific port's L2 multicast learning status
 * @details This function gets multicast learning status of the specified port
 * @param[in]	port Port 	port index
 * @param[out]	pen_learning 	The port's multicast learning status
 * @return Get port muticast learning stat successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_l2_mc_port_learning_get(jl_port_t port, jl_enable_t *pen_learning);

/**
 * @brief Set specific port's L2 multicast learning status
 * @details This function sets multicast learning status of the specified port
 * @param[in]	port		port index
 * @param[in]	en_learning 	value of the port mc learning status
 * @return Set port muticast learning stat successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_l2_mc_port_learning_set(jl_port_t port, jl_enable_t en_learning);

/**
 * @brief Get L2 fib configurations
 * @details This function takes the specified MAC address and vid field of pkey as input,
 * searches it in hash table and collision table. \n
 * Then return the matched L2 info if found or return JL_ERR_NOT_FOUND if not.
 * @param[in]	pkey	The indication (Hash table or collision table) and address of the table
 * @param[out]	pcfg	The vaule of table
 * @return Get L2 fib configurations successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_NOT_FOUND	Not Matched Finally
 * @note
 *		Specify the mac and fid as the input parameters,
 *		the API will return the related information in L2 lookup tables.
 *
 */
JL_API jl_api_ret_t jl_l2_fib_config_get(jl_l2_fib_key_t *pkey, jl_l2_fib_config_t *pcfg);

/**
 * @brief Get L2 fib configurations by index
 * @details This function gets all the L2 info by the specified entry index
 * @param[in]	entry_index	the L2 entry index, ranges[0, 2063]
 * @param[out]	pkey		The indication (Hash table or collision table) and the MAC Address of the table
 * @param[out]	pcfg		The info of table
 * @return Get L2 fib configurations successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 * @note
 *		Specify the entry_index as the input parameters,
 *		the API will return the related information in L2 lookup tables in pkey and pcfg.
 *
 */
JL_API jl_api_ret_t jl_l2_fib_config_get_by_index(jl_uint32 entry_index, jl_l2_fib_key_t *pkey, jl_l2_fib_config_t *pcfg);

/**
 * @brief Add the MAC address automatically in the L2 MAC table
 * @details This function adds the MAC address automatically in the L2 MAC table. It works in an un-aggressive mode, i.e, \n
 * The MAC address will be added successfully only no DA has collision or collision table is non-full. Or it will return fail.
 * @param[in] pkey	input pkey MAC and fid for L2 MAC table, entry index is output
 * @param[in] pcfg	Data of L2 MAC table
 * @return Set L2 fib configurations successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 * @note 
 - It works in an un-aggressive mode, i.e, the MAC address will be added successfully only no DA has collision or collision table is non-full. Or it will return fail.
 - You can set the is_valid = false to clear one mac address with input parameters mac_addr and fid.
 */
JL_API jl_api_ret_t jl_l2_fib_config_set_auto(jl_l2_fib_key_t *pkey, jl_l2_fib_config_t *pcfg);

/**
 * @brief Fast clear the dynamic entries in MAC lookup table and the collision table
 * @details This function clears all the dynamic entries in L2 DA hash table and collision table simultaneously.
 * @return Fast clear opertion is successful or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_l2_fib_fast_clear_dynamic(void);

/**
 * @brief Clear the MAC lookup table or the collision table
 * @details This function clears the L2 DA hash table or collision table as required
 * @param[in] is_collison	clear the collision table 0: clear the lookup table
 * @return Clear opertion is successful or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_l2_fib_clear(jl_bool is_collison);

/**
 * @brief Clear the MAC lookup table and the collision table
 * @details This function clears the L2 DA hash table and collision table simultaneously
 * @return Clear opertion is successful or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_l2_fib_clear_all(void);

/**
 * @brief Set L2 aging time
 * @details This function configures the L2 MAC table hardware aging period.
 * @param[in] time	MAC table hardware aging period
 * @param[in] unit	The unit of aging period
 * @return Set L2 aging time successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_l2_aging_time_set(jl_uint32 time, jl_uint8 unit);

/**
 * @brief Get L2 aging time
 * @details This function gets the L2 MAC table hardware aging period.
 * @param[out] ptime	The MAC table hardware aging period
 * @param[out] punit	The unit of aging period
 * @return Get L2 aging time successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_l2_aging_time_get(jl_uint32 *ptime, jl_uint8 *punit);

#ifdef __cplusplus
}
#endif

#endif /* __JL_API_L2_H__ */
/** @} L2 Switching APIs*/
