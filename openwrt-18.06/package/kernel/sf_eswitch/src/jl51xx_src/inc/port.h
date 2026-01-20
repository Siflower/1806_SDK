/**
 * @file port.h
 * @brief Port
 */

/**
 * @defgroup port_group port
 * @{
 */

/**
 * @example example_port_mac_port.c
 * @example example_port_phy_port.c
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

#ifndef __JL_API_PORT_H__
#define __JL_API_PORT_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum jl_port_link_status_e
 * @brief port link status.
 */
enum jl_port_link_status_e {
	PORT_LINKDOWN = 0,/**< 0 */
	PORT_LINKUP,/**< 1 */
	PORT_LINKSTATUS_END/**< Invalid */
};

/**
 * @public typedef jl_port_link_status_t
 */
typedef enum jl_port_link_status_e jl_port_link_status_t;

/**
 * @enum jl_port_duplex_e
 * @brief port duplex.
 */
enum jl_port_duplex_e {
	PORT_HALF_DUPLEX = 0,/**< 0 */
	PORT_FULL_DUPLEX,/**< 1 */
	PORT_DUPLEX_END/**< Invalid */
};

/**
 * @public typedef jl_port_duplex_t
 */
typedef enum jl_port_duplex_e jl_port_duplex_t;

/**
 * @struct jl_port_mac_ability_s
 * @brief mac port ability.
 */
struct jl_port_mac_ability_s {
	jl_uint32 force_mode;/**< Indicates whether MAC status control in force mode or not */
	jl_uint32 speed;/**< Indicates the speed of MAC, 0: 10M, 1: 100M */
	jl_uint32 duplex;/**< Indicates the duplex mode of MAC, 0: Half, 1: Full */
	jl_uint32 link;/**< Indicates the link status of MAC */
	jl_uint32 force_fc_en;/**< Indicates whether MAC flow control in force mode or not */
	jl_uint32 tx_pause;/**< Indicates the tx flow control of MAC */
	jl_uint32 rx_pause;/**< Indicates the rx flow control of MAC */
};

/**
 * @public typedef jl_port_mac_ability_t
 */
typedef struct jl_port_mac_ability_s jl_port_mac_ability_t;

/**
 * @struct jl_port_ext_mac_ability_s
 * @brief external mac port ability.
 */
struct jl_port_ext_mac_ability_s {
	jl_uint32 force_mode;/**< Indicates whether External MAC operate in force mode or not */
	jl_uint32 speed;/**< Indicates the speed of MAC, 0: 10M, 1: 100M */
	jl_uint32 duplex;/**< Indicates the duplex mode of MAC, 0: Half, 1: Full */
	jl_uint32 link;/**< Indicates the link status of MAC */
	jl_uint32 tx_pause;/**< Indicates the tx flow control of MAC */
	jl_uint32 rx_pause;/**< Indicates the rx flow control of MAC */
};

/**
 * @public typedef jl_port_ext_mac_ability_t
 */
typedef struct jl_port_ext_mac_ability_s jl_port_ext_mac_ability_t;

/**
 * @struct jl_port_phy_ability_s
 * @brief phy port ability.
 */
struct jl_port_phy_ability_s {
	jl_uint32 auto_negotiation;/**< Indicates whether PHY operate in auto negotiation mode or not */
	jl_uint32 half_duplex_10;/**< Indicates PHY operate in half duplex 10M mode */
	jl_uint32 full_duplex_10;/**< Indicates PHY operate in full duplex 10M mode */
	jl_uint32 half_duplex_100;/**< Indicates PHY operate in half duplex 100M mode */
	jl_uint32 full_duplex_100;/**< Indicates PHY operate in full duplex 100M mode */
	jl_uint32 flow_control;/**< Indicates PHY support flow control */
	jl_uint32 asym_flow_control;/**< Indicates PHY support asymmetric flow control */
};

/**
 * @public typedef jl_port_phy_ability_t
 */
typedef struct jl_port_phy_ability_s jl_port_phy_ability_t;

/**
 * @enum jl_speed_e
 * @brief port speed.
 */
enum jl_speed_e {
	PORT_SPEED_10M = 0,/**< 0 */
	PORT_SPEED_100M,/**< 1 */
	PORT_SPEED_END/**< Invalid */
};

/**
 * @public typedef jl_port_phy_ability_t
 */
typedef enum jl_speed_e jl_port_speed_t;

/**
 * @enum jl_port_phy_reg_e
 * @brief phy port Reg.
 */
enum jl_port_phy_reg_e {
	PHY_CONTRL_REG = 0,/**< 0 */
	PHY_STATUS_REG,/**< 1 */
	PHY_ID1_REG,/**< 2 */
	PHY_ID2_REG,/**< 3 */
	PHY_AN_ADV_REG,/**< 4 */
	PHY_AN_LINKP_REG,/**< 5 */
	PHY_AN_EXP_REG,/**< 6 */
	PHY_PAGE_SEL_REG = 0x1F,/**< 31 */
	PHY_REG_END/**< Invalid */
};

/**
 * @public typedef jl_port_phy_reg_t
 */
typedef enum jl_port_phy_reg_e jl_port_phy_reg_t;

/**
 * @public typedef jl_port_phy_page_t
 */
typedef jl_uint32 jl_port_phy_page_t;

/**
 * @public typedef jl_port_phy_data_t
 */
typedef jl_uint32 jl_port_phy_data_t;

/**
 * @brief Set ethernet PHY auto-negotiation ability
 * @details This function configures PHY capability of specified port.
 * If Full_1000 bit is set to 1, the auto negotiation will be automatic set to 1.
 * While both auto negotiation and Full_1000 are set to 0, the PHY speed and duplex
 * selection will be set as following 100F > 100H > 10F > 10H priority sequence.
 * @param[in] port	Index of port number
 * @param[in] pability	PHY ability structure
 * @return set PHY ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *		While auto-negotiation is set to 0, the PHY speed and duplex selection will be set as following 100F > 100H > 10F > 10H priority sequence
 */
JL_API jl_api_ret_t jl_port_phy_autoneg_ability_set(jl_port_t port, jl_port_phy_ability_t *pability);

/**
 * @brief Get ethernet PHY auto-negotiation ability
 * @details This function gets PHY auto negotiation capability of specified port.
 * @param[in]	port		Index of port number
 * @param[out]	pability	PHY ablility structure
 * @return get PHY ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *		Get the auto-negotiation ability of specific port
 */
JL_API jl_api_ret_t jl_port_phy_autoneg_ability_get(jl_port_t port, jl_port_phy_ability_t *pability);

/**
 * @brief Set ethernet PHY force-mode ability
 * @details While both auto negotiation is set to 0, the PHY speed and duplex selection will be
 * set as following 100F > 100H > 10F > 10H priority sequence.
 * @param[in] port	Index of port number
 * @param[in] pability	PHY ability structure
 * @return set PHY ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *		While auto-negotiation is set to 0, the PHY speed and duplex selection will
 *      be set as following 100F > 100H > 10F > 10H priority sequence
 */
JL_API jl_api_ret_t jl_port_phy_force_ability_set(jl_port_t port, jl_port_phy_ability_t *pability);

/**
 * @brief Get ethernet PHY auto-negotiation ability
 * @details This function gets the force-mode PHY capability of specified port.
 * @param[in]	port		Index of port number
 * @param[out]	pability	PHY ablility structure
 * @return get PHY ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can get the force-mode ability of specific port
 */
JL_API jl_api_ret_t jl_port_phy_force_ability_get(jl_port_t port, jl_port_phy_ability_t *pability);

/**
 * @brief Get ethernet PHY link status
 * @details This function gets PHY linking status of specified port.
 * @param[in]	port	Index of port number
 * @param[out]	pstatus	PHY Link Status
 * @param[out]	pduplex	PHY Duplex Mode
 * @param[out]	pspeed  PHY Speed
 * @return get PHY link status successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *		API will return the current PHY status of specific port
 */
JL_API jl_api_ret_t jl_port_phy_link_status_get(jl_port_t port, jl_port_link_status_t *pstatus, jl_port_duplex_t *pduplex, jl_port_speed_t *pspeed);

/**
 * @brief Set ethernet PHY direct access register
 * @details This function sets PHY register data of the specified port.
 * @param[in]	port	Index of port number
 * @param[in]	page	page id
 * @param[in]	reg	register id
 * @param[in]	regval	Register data
 * @return set PHY direct access register successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can set PHY direct access register value of specific port
 */
JL_API jl_api_ret_t jl_port_phy_reg_set(jl_port_t port, jl_port_phy_page_t page, jl_port_phy_reg_t reg, jl_port_phy_data_t regval);

/**
 * @brief Get ethernet PHY direct access register
 * @details This function gets PHY register data of specified port.
 * @param[in]	port	Index of port number
 * @param[in]	page	page id
 * @param[in]	reg	register id
 * @param[out]	pregval Register data
 * @return get PHY direct access register successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can return PHY direct access register value of specific port
 */
JL_API jl_api_ret_t jl_port_phy_reg_get(jl_port_t port, jl_port_phy_page_t page, jl_port_phy_reg_t reg, jl_port_phy_data_t *pregval);

/**
 * @brief Set ethernet PHY indirect access register 
 * @details This function sets PHY indirect registers data of the specified port.
 * The input register address is the offset address of the port base register.
 * This function is used to write non-IEEE PHY registers mainly defined in reg_fephy.h
 * @param[in] port	Index of port number
 * @param[in] reg	Register address
 * @param[in] regval	Register data
 * @return set PHY indirect access register successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can set PHY indirect access register value of specific port
 */
JL_API jl_api_ret_t jl_port_phy_indirect_reg_set(jl_port_t port, jl_port_phy_reg_t reg, jl_port_phy_data_t regval);

/**
 * @brief Get ethernet PHY indirect access register
 * @details This function gets PHY indirect register data of specified port.
 * The input register address is the offset address of the port based register.
 * This function is used to read non-IEEE PHY registers mainly defined in reg_fephy.h
 * @param[in]	port	Index of port number
 * @param[in]	reg	Register address
 * @param[out]	pregval	Register data
 * @return get PHY indirect access register successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can return PHY indirect access register value of specific port
 */
JL_API jl_api_ret_t jl_port_phy_indirect_reg_get(jl_port_t port, jl_port_phy_reg_t reg, jl_port_phy_data_t *pregval);

/**
 * @brief Set PHY isolate status on specific port
 * @details This function sets PHY electrically isolate from MII/RMII of specified port.
 * @param[in]	port	Index of port numb
 * @param[in]	isolate	Isolate status
 * @return set PHY isolate function successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can enable or disable PHY isolate status on specific port
 */
JL_API jl_api_ret_t jl_port_phy_isolate_set(jl_port_t port, jl_enable_t isolate);

/**
 * @brief Get PHY isolate status on specific port
 * @details This function gets PHY isolate status of specified port
 * @param[in]	port	Index of port number
 * @param[out]	pisolate	Isolate status
 * @return get PHY isolate status successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can get PHY isolate status on specific port
 */
JL_API jl_api_ret_t jl_port_phy_isolate_get(jl_port_t port, jl_enable_t *pisolate);

/**
 * @brief Set ethernet PHY enable status
 * @details This function enables or disables the PHY as specified port
 * @param[in]	port	Index of port number
 * @param[in]	enable	PHY enable state
 * @return set PHY enable status successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can set PHY enable status on specific port
 */
JL_API jl_api_ret_t jl_port_phy_enable_set(jl_port_t port, jl_enable_t enable);

/**
 * @brief Get ethernet PHY enable status
 * @details This function gets PHY enable status as specified port.
 * @param[in]	port	Index of port number
 * @param[out]	penable	PHY enable status
 * @return get PHY enable status successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can return PHY enable status on specific port
 */
JL_API jl_api_ret_t jl_port_phy_enable_get(jl_port_t port, jl_enable_t *penable);

/**
 * @brief Set ethernet all PHY enable status
 * @details This function enables or disables all the PHYs
 * @param[in]	enable	PHY enable state
 * @return set all PHY enable status successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can enable or disable all the port
 */
JL_API jl_api_ret_t jl_port_phy_all_enable_set(jl_enable_t enable);

/**
 * @brief Set port force link ability
 * @details This function sets MAC force-link capability of specified port.
 * @param[in] port	Index of port number
 * @param[in] pability	MAC ability structure
 * @return set port MAC force link ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can set MAC force link ability of a specific port
 */
JL_API jl_api_ret_t jl_port_mac_force_link_set(jl_port_t port, jl_port_mac_ability_t *pability);

/**
 * @brief Get port force link ability
 * @details This function gets MAC force-link capability of specified port.
 * @param[in]	port		Index of port number
 * @param[out]	pability	MAC ability structure
 * @return get port MAC force link ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can get MAC force link ability of a specific port
 */
JL_API jl_api_ret_t jl_port_mac_force_link_get(jl_port_t port, jl_port_mac_ability_t *pability);

/**
 * @brief Set external port force link ability
 * @details This function sets MAC force-link capability of specified Extension port
 * @param[in] port	Index of port number
 * @param[in] pability	Ext MAC ability structure
 * @return set external port MAC force link ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can set external port mode and force link ability
 */
JL_API jl_api_ret_t jl_port_mac_force_link_ext_set(jl_port_t port, jl_port_ext_mac_ability_t *pability);

/**
 * @brief Get external port force link ability
 * @details This function gets MAC force-link capability of specified Extension port
 * @param[in]	port		Index of port number
 * @param[out]	pability	Ext MAC ability structure
 * @return get external port MAC force link ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can get external port mode and force link ability
 */
JL_API jl_api_ret_t jl_port_mac_force_link_ext_get(jl_port_t port, jl_port_ext_mac_ability_t *pability);

/**
 * @brief Get port link status
 * @details This function gets force MAC link parameters of specified port.
 * @param[in]	port		Index of port number
 * @param[out]	pability	Current mac Status
 * @return get port link status successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can get link status of a specific port
 */
JL_API jl_api_ret_t jl_port_mac_status_get(jl_port_t port, jl_port_mac_ability_t *pability);

/**
 * @brief Set port local loopback (redirect tx to rx)
 * @details This function sets MAC loopback status of specified port
 * @param[in]	port	portid
 * @param[in]	enable	enable MAC loopback status
 * @return enable/disable local loopback of a specific port
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can get link status of a specific port
 */
JL_API jl_api_ret_t jl_port_mac_loopback_set(jl_port_t port, jl_enable_t enable);

/**
 * @brief Get port local loopback status
 * @details This function gets MAC loopback status of specified port.
 * @param[in]	port	Index of port number
 * @param[out]	penable	enable MAC loopback status
 * @return local loopback status of a specific port
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can get local loopback is enabled or not of a specific port
 */
JL_API jl_api_ret_t jl_port_mac_loopback_get(jl_port_t port, jl_enable_t *penable);

#ifdef __cplusplus
}
#endif

#endif /* __JL_API_PORT_H__ */
/** @} Port APIs*/
