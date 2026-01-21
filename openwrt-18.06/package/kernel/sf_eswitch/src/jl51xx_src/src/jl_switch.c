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

#ifdef __KERNEL__
#include <linux/module.h>
#endif
#include "jl_base.h"
#include "jl51xx/jl51xx_drv_phy.h"
#include "jl51xx/jl51xx_drv_switch.h"
#include "version.h"


jl_switch_ctrl_t g_switch_ctrl = {STAT_UNINIT, CORE_VER0, ECO_R0P0, NULL};

#if ((defined CONFIG_JLFE_CHIP_ID_5104) || (defined CONFIG_JLFE_CHIP_ID_51XX))
jl_switch_dev_t g_switch_dev_5104 = {
	/* switch chip id */
	CHIP_ID_JL5104,

	/* port transform: phy to mac */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, EXT_PORT1, UNDEF_PORT},

	/* port transform: mac to phy */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, EXT_PORT0},

	/* port transform: logical port to vendor specific port */
	{UTP_PORT0, UTP_PORT3, UTP_PORT4, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT},

	/* port transform: vendor specific port to logical port */
	{UTP_PORT0, UNDEF_PORT, UNDEF_PORT, UTP_PORT1, UTP_PORT2, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT},

	/* phy port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_EXT, PORT_TYPE_UNDEF},

	/* mac port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_EXT},

	/* vendor specific port */
	{PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF},

	/* phy port mask */
	0x0107,

	/* mac port mask */
	0x0207,

	/* vendor port mask */
	0x0019,
};
#endif
#if ((defined CONFIG_JLFE_CHIP_ID_5105) || (defined CONFIG_JLFE_CHIP_ID_51XX))
jl_switch_dev_t g_switch_dev_5105 = {
	/* switch chip id */
	CHIP_ID_JL5105,

	/* port transform: phy to mac */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT},

	/* port transform: mac to phy */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT},

	/* port transform: logical port to vendor specific port */
	{UTP_PORT0, UTP_PORT2, UTP_PORT5, UTP_PORT6, UTP_PORT7, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT},

	/* port transform: vendor specific port to logical port */
	{UTP_PORT0, UNDEF_PORT, UTP_PORT1, UNDEF_PORT, UNDEF_PORT, UTP_PORT2, UTP_PORT3, UTP_PORT4, UNDEF_PORT, UNDEF_PORT},

	/* phy port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF},

	/* mac port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF},

	/* vendor port type */
	{PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF},

	/* phy port mask */
	0x001f,

	/* mac port mask */
	0x001f,

	/* vendor port mask */
	0x00e5,
};
#endif
#if ((defined CONFIG_JLFE_CHIP_ID_5106) || (defined CONFIG_JLFE_CHIP_ID_51XX))
jl_switch_dev_t g_switch_dev_5106 = {
	/* switch chip id */
	CHIP_ID_JL5106,

	/* port transform: phy to mac */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, EXT_PORT1, UNDEF_PORT},

	/* port transform: mac to phy */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, EXT_PORT0},

	/* port transform: logical port to vendor specific port */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT},

	/* port transform: vendor specific port to logical port */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT},

	/* phy port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_EXT, PORT_TYPE_UNDEF},

	/* mac port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_EXT},

	/* vendor port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF},

	/* phy port mask */
	0x011f,

	/* mac port mask */
	0x021f,

	/* vendor port mask */
	0x001f,

};
#endif
#if ((defined CONFIG_JLFE_CHIP_ID_5108) || (defined CONFIG_JLFE_CHIP_ID_51XX))
jl_switch_dev_t g_switch_dev_5108 = {
	/* switch chip id */
	CHIP_ID_JL5108,

	/* port transform: phy to mac */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UTP_PORT5, UTP_PORT6, UTP_PORT7, UNDEF_PORT, UNDEF_PORT},

	/* port transform: mac to phy */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UTP_PORT5, UTP_PORT6, UTP_PORT7, UNDEF_PORT, UNDEF_PORT},

	/* port transform: logical port to vendor specific port */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UTP_PORT5, UTP_PORT6, UTP_PORT7, UNDEF_PORT, UNDEF_PORT},

	/* port transform: vendor specific port to logical port */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UTP_PORT5, UTP_PORT6, UTP_PORT7, UNDEF_PORT, UNDEF_PORT},

	/* phy port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF},

	/* mac port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF},

	/* vendor port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF},

	/* phy port mask */
	0x00ff,

	/* mac port mask */
	0x00ff,

	/* vendor port mask */
	0x00ff,
};
#endif
#if ((defined CONFIG_JLFE_CHIP_ID_5109) || (defined CONFIG_JLFE_CHIP_ID_51XX))
jl_switch_dev_t g_switch_dev_5109 = {
	/* switch chip id */
	CHIP_ID_JL5109,

	/* port transform: phy to mac */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UTP_PORT5, UTP_PORT6, UTP_PORT7, EXT_PORT1, UNDEF_PORT},

	/* port transform: mac to phy */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UTP_PORT5, UTP_PORT6, UTP_PORT7, UNDEF_PORT, EXT_PORT0},

	/* port transform: logical port to vendor specific port */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UTP_PORT5, UTP_PORT6, UTP_PORT7, UNDEF_PORT, UNDEF_PORT},

	/* port transform: vendor specific port to logical port */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UTP_PORT5, UTP_PORT6, UTP_PORT7, UNDEF_PORT, UNDEF_PORT},

	/* phy port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_EXT, PORT_TYPE_UNDEF},

	/* mac port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_EXT},

	/* vendor port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF},

	/* phy port mask */
	0x01ff,

	/* mac port mask */
	0x02ff,

	/* vendor port mask */
	0x00ff,

};
#endif
#if ((defined CONFIG_JLFE_CHIP_ID_5110) || (defined CONFIG_JLFE_CHIP_ID_51XX))
jl_switch_dev_t g_switch_dev_5110 = {
	/* switch chip id */
	CHIP_ID_JL5110,

	/* port transform: phy to mac */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UTP_PORT5, UTP_PORT6, UTP_PORT7, EXT_PORT0, EXT_PORT1},

	/* port transform: mac to phy */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UTP_PORT5, UTP_PORT6, UTP_PORT7, EXT_PORT0, EXT_PORT1},

	/* port transform: logical port to vendor specific port */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UTP_PORT5, UTP_PORT6, UTP_PORT7, UNDEF_PORT, UNDEF_PORT},

	/* port transform: vendor specific port to logical port */
	{UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UTP_PORT5, UTP_PORT6, UTP_PORT7, UNDEF_PORT, UNDEF_PORT},

	/* phy port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_EXT, PORT_TYPE_EXT},

	/* mac port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_EXT, PORT_TYPE_EXT},

	/* vendor port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF},

	/* phy port mask */
	0x03ff,

	/* mac port mask */
	0x03ff,

	/* vendor port mask */
	0x00ff,
};
#endif

#ifdef CONFIG_JLFE_CHIP_ID_51XX
jl_switch_dev_t *gp_switch_dev;
#elif defined(CONFIG_JLFE_CHIP_ID_5104)
jl_switch_dev_t *gp_switch_dev = &g_switch_dev_5104;
#elif defined(CONFIG_JLFE_CHIP_ID_5105)
jl_switch_dev_t *gp_switch_dev = &g_switch_dev_5105;
#elif defined(CONFIG_JLFE_CHIP_ID_5106)
jl_switch_dev_t *gp_switch_dev = &g_switch_dev_5106;
#elif defined(CONFIG_JLFE_CHIP_ID_5108)
jl_switch_dev_t *gp_switch_dev = &g_switch_dev_5108;
#elif defined(CONFIG_JLFE_CHIP_ID_5109)
jl_switch_dev_t *gp_switch_dev = &g_switch_dev_5109;
#elif defined(CONFIG_JLFE_CHIP_ID_5110)
jl_switch_dev_t *gp_switch_dev = &g_switch_dev_5110;
#endif


void _jl_switch_info(jl_switch_info_t *pinfo)
{
	pinfo->core_ver = g_switch_ctrl.core_ver;
	pinfo->pkg = gp_switch_dev->chip_id;
	pinfo->eco_ver = g_switch_ctrl.eco_ver;
	pinfo->sdk_ver = (VERSION_JL_SDK_MAJOR << 28)
		| (VERSION_JL_SDK_MINOR << 16)
		| (VERSION_JL_SDK_MAINTENANCE);
}

jl_api_ret_t jl_switch_init(void)
{
	jl_api_ret_t ret = JL_ERR_OK;

	if (JL_STAT_IS_READY())
		return JL_ERR_OK;

	JL_API_MUTEX_INIT();

#ifdef CONFIG_JLFE_APB_MUTEX
	ret = jl_reg_io_init();
	if (ret)
		return ret;
#endif

#ifdef CONFIG_JLFE_DUMP_REG_WRITE
	jl_dump_enable();
	jl_dump_add_comment("======jl switch init======");
#endif

	ret = jl51xx_sw_core_init();
	if (ret)
		return ret;

	JL_STAT_INIT();

	return JL_ERR_OK;
}
EXPORT_SYMBOL(jl_switch_init);

jl_api_ret_t jl_switch_deinit(void)
{
	jl_api_ret_t ret = JL_ERR_OK;

	if (JL_STAT_IS_UNINIT())
		return JL_ERR_OK;

	JL_API_MUTEX_LOCK();

#ifdef CONFIG_JLFE_DUMP_REG_WRITE
	jl_dump_add_comment("======jl switch deinit======");
#endif

	ret = jl51xx_sw_core_deinit();
	if (ret)
		goto exit;

#ifdef CONFIG_JLFE_APB_MUTEX
	ret = jl_reg_io_deinit();
	if (ret)
		goto exit;
#endif

exit:
	JL_API_MUTEX_UNLOCK();

	JL_API_MUTEX_DEINIT();
	JL_STAT_DEINIT();

	return ret;
}
EXPORT_SYMBOL(jl_switch_deinit);

jl_api_ret_t jl_switch_info(jl_switch_info_t *pinfo)
{
	JL_CHECK_STAT();

	JL_CHECK_POINTER(pinfo);

	_jl_switch_info(pinfo);

	return JL_ERR_OK;
}
EXPORT_SYMBOL(jl_switch_info);

jl_ret_t jl_switch_phyport_check(jl_port_t phy_port)
{
	if (phy_port >= JL_PORT_MAX)
		return JL_ERR_PORT;

	if (gp_switch_dev->phy_port_type[phy_port] == PORT_TYPE_UNDEF)
		return JL_ERR_PORT;

	return JL_ERR_OK;
}

jl_ret_t jl_switch_phyport_check_utp(jl_port_t phy_port)
{
	if (phy_port >= JL_PORT_MAX)
		return JL_ERR_PORT;

	if (gp_switch_dev->phy_port_type[phy_port] != PORT_TYPE_UTP)
		return JL_ERR_PORT;

	return JL_ERR_OK;
}

jl_ret_t jl_switch_phyport_check_ext(jl_port_t phy_port)
{
	if (phy_port >= JL_PORT_MAX)
		return JL_ERR_PORT;

	if (gp_switch_dev->phy_port_type[phy_port] != PORT_TYPE_EXT)
		return JL_ERR_PORT;
	return JL_ERR_OK;
}

jl_ret_t jl_switch_macport_check(jl_port_t mac_port)
{
	if (mac_port >= JL_PORT_MAX)
		return JL_ERR_PORT;

	if (gp_switch_dev->mac_port_type[mac_port] == PORT_TYPE_UNDEF)
		return JL_ERR_PORT;

	return JL_ERR_OK;
}

jl_ret_t jl_switch_vendorport_check(jl_port_t vendor_port)
{
	if (vendor_port >= JL_PORT_MAX)
		return JL_ERR_PORT;

	if (gp_switch_dev->vendor_port_type[vendor_port] != PORT_TYPE_UTP)
		return JL_ERR_PORT;

	return JL_ERR_OK;
}

jl_portmask_t jl_switch_portmask_p2m(jl_portmask_t phy_portmask)
{
	jl_portmask_t mac_portmask = 0;
	jl_port_t phy_port;
	jl_port_t mac_port;

	JL_FOR_EACH_PHY_PORT(phy_port) {
		if (phy_portmask & (1 << phy_port)) {
			mac_port = JL_PORT_P2M(phy_port);
			mac_portmask |= 1 << mac_port;
		}
	}

	return mac_portmask;
}

jl_portmask_t jl_switch_portmask_m2p(jl_portmask_t mac_portmask)
{
	jl_portmask_t phy_portmask = 0;
	jl_port_t mac_port;
	jl_port_t phy_port;

	JL_FOR_EACH_MAC_PORT(mac_port) {
		if (mac_portmask & (1 << mac_port)) {
			phy_port = JL_PORT_M2P(mac_port);
			phy_portmask |= 1 << phy_port;
		}
	}

	return phy_portmask;
}

jl_portmask_t jl_switch_portmask_p2vp(jl_portmask_t phy_portmask)
{
	jl_portmask_t vendor_portmask = 0;
	jl_port_t phy_port;
	jl_port_t vendor_port;

	JL_FOR_EACH_PHY_UTP_PORT(phy_port) {
		if (phy_portmask & (1 << phy_port)) {
			vendor_port = JL_PORT_P2VP(phy_port);
			vendor_portmask |= 1 << vendor_port;
		}
	}

	return vendor_portmask;
}

jl_portmask_t jl_switch_portmask_vp2p(jl_portmask_t vendor_portmask)
{
	jl_portmask_t phy_portmask = 0;
	jl_port_t vendor_port;
	jl_port_t phy_port;

	JL_FOR_EACH_VENDOR_PORT(vendor_port) {
		if (vendor_portmask & (1 << vendor_port)) {
			phy_port = JL_PORT_VP2P(vendor_port);
			phy_portmask |= 1 << phy_port;
		}
	}

	return phy_portmask;
}

jl_ret_t jl_switch_init_state_set(jl_stat_t state)
{
	if (state >= STAT_END)
		return JL_ERR_PARAM;

	g_switch_ctrl.stat = state;

	return JL_ERR_OK;
}
EXPORT_SYMBOL(jl_switch_init_state_set);

jl_ret_t jl_switch_init_state_get(jl_stat_t *pstate)
{
	JL_CHECK_POINTER(pstate);

	*pstate = g_switch_ctrl.stat;

	return JL_ERR_OK;
}
EXPORT_SYMBOL(jl_switch_init_state_get);

#ifdef __KERNEL__
MODULE_DESCRIPTION("JLSemi ethernet switch main module");
MODULE_AUTHOR("JLSemi Limited");
MODULE_LICENSE("GPL v2");
#endif
