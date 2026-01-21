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

#include "jl_base.h"
#include "driver/jl_reg_io.h"
#include "jl51xx/reg_mag.h"
#include "jl51xx/reg_swcore.h"
#include "jl51xx/reg_apb_frontend.h"
#include "jl51xx/reg_clkgen.h"
#include "jl51xx/reg_pinmux.h"
#include "jl51xx/jl51xx_drv_vlan.h"
#include "jl51xx/jl51xx_drv_cpu.h"
#include "jl51xx/jl51xx_drv_phy.h"
#include "jl51xx/jl51xx_drv_led.h"
#include "jl51xx/jl51xx_mac_uctrl.h"
#ifdef CONFIG_JLFE_LOAD_FORMAL_PATCH
#include "jl51xx/jl51xx_drv_formal_patch.h"
#endif
#ifdef CONFIG_JLFE_LOAD_CUSTOMIZED_PATCH
#include "jl51xx/jl51xx_drv_customized_patch.h"
#endif

/* enable/disable LED group0&group1 by reset led pinmux */
jl_ret_t _led_enable_set(jl_uint8 led_group, jl_uint8 enable)
{
	jl_ret_t ret;
	jl_uint32 pinmux0_val;
	jl_uint32 pinmux1_val;
	jl_uint32 pinmux0_addr = PINMUX_BASE + PIN_MUX_0_OFFSET;
	jl_uint32 pinmux1_addr = PINMUX_BASE + PIN_MUX_1_OFFSET;

	ret = jl_apb_reg_read(pinmux0_addr, &pinmux0_val);
	if (ret)
		return ret;
	ret = jl_apb_reg_read(pinmux1_addr, &pinmux1_val);
	if (ret)
		return ret;


	if (led_group == 0) {
		if (enable)
			SET_BITS(pinmux0_val, 7, 14);
		else
			CLR_BITS(pinmux0_val, 7, 14);

		ret = jl_apb_reg_write(pinmux0_addr, pinmux0_val);
		if (ret)
			return ret;
	}

	if (led_group == 1) {
		if (enable) {
			SET_BIT(pinmux0_val, 15);
			SET_BITS(pinmux1_val, 0, 5);
		} else {
			CLR_BIT(pinmux0_val, 15);
			CLR_BITS(pinmux1_val, 0, 5);
		}

		ret = jl_apb_reg_write(pinmux0_addr, pinmux0_val);
		if (ret)
			return ret;
		ret = jl_apb_reg_write(pinmux1_addr, pinmux1_val);
		if (ret)
			return ret;
	}

	return JL_ERR_OK;
}

jl_ret_t _led_reset_blink_set(jl_uint8 led_blink)
{
	jl_ret_t ret;
	jl_uint32 cpu_reserved0_val;
	jl_uint32 rsvd0_addr = APB_FRONTEND_BASE + CPU_RESERVED0_OFFSET;

	ret = jl_apb_reg_read(rsvd0_addr, &cpu_reserved0_val);
	if (ret)
		return ret;

	/* enable/disable led reset blink */
	if (led_blink)
		CLR_BIT(cpu_reserved0_val, 19);
	else
		SET_BIT(cpu_reserved0_val, 19);

	ret = jl_apb_reg_write(rsvd0_addr, cpu_reserved0_val);
	if (ret)
		return ret;

	return ret;
}

jl_ret_t _force_soft_reset(jl_uint8 with_analog,
				jl_uint8 with_swc, jl_uint8 with_mag,
				jl_uint8 led_blink)
{
	jl_ret_t ret;
	jl_uint32 cpu_reserved1_val;
	jl_uint32 rsvd1_addr = APB_FRONTEND_BASE + CPU_RESERVED1_OFFSET;
	jl_uint32 clkgen0_val;
	jl_uint32 clkgen0_addr = CLKGEN_BASE + CLKGEN_CTL_0_OFFSET;
	jl_uint32 count = 10;
	jl_uint32 port_mask = 0;

	_led_reset_blink_set(led_blink);

	ret = jl_apb_reg_read(rsvd1_addr, &cpu_reserved1_val);
	if (ret)
		return ret;

	ret = jl_apb_reg_read(clkgen0_addr, &clkgen0_val);
	if (ret)
		return ret;


	/* fast soft reset, for led blink conern */
	if (!with_analog && !with_swc && !with_mag) {
		SET_BIT(cpu_reserved1_val, 31);
		ret = jl_apb_reg_write(rsvd1_addr, cpu_reserved1_val);
		if (ret)
			return ret;

		goto exit;
	}

	/* XXX rm this line, if led reset status is fixed in fw */
	_led_enable_set(0, 0);

	/* Set switch_pd = 1, switch_pd_with_ana = 1 */
	/* Set disable_ext_eeprom = 1 */
	if (with_analog)
		cpu_reserved1_val |= 0x30000008;
	else
		cpu_reserved1_val |= 0x20000008;
	ret = jl_apb_reg_write(rsvd1_addr, cpu_reserved1_val);
	if (ret)
		return ret;

	/* Set MAG_RSTN = 0, CORE_6P25 = 0 */
	if (with_swc)
		CLR_BIT(clkgen0_val, 4);
	if (with_mag)
		CLR_BIT(clkgen0_val, 7);
	ret = jl_apb_reg_write(clkgen0_addr, clkgen0_val);
	if (ret)
		return ret;

	port_udelay(10000);
	/* XXX rm this line, if led reset status is fixed in fw */
	_led_enable_set(0, 1);

	ret = jl_apb_reg_read(rsvd1_addr, &cpu_reserved1_val);
	if (ret)
		return ret;

	/* Set switch_pd = 0, switch_pd_witch_ana = 0 */
	CLR_BITS(cpu_reserved1_val, 28, 29);
	ret = jl_apb_reg_write(rsvd1_addr, cpu_reserved1_val);
	if (ret)
		return ret;

exit:
	if (!led_blink) {
		while (count--) {
			port_udelay(1000);

			/*read ADDR_LOAD_DATA4,capture signal FLAG_PROCESS for enabling leds */
			ret = jl51xx_phy_direct_read_ext(0, 7, 0x15, &port_mask);
			if (ret)
				return ret;

			if (port_mask == 0xb) {
				ret = jl51xx_led_enable_set(LED_GROUP0, 0xff);
				if (ret)
					return ret;

				break;
			}
		};
	}

	return JL_ERR_OK;
}

jl_ret_t __check_ext_port_mode_param(void)
{
	jl_chip_id_t chip;

	chip = gp_switch_dev->chip_id;
	if (chip == CHIP_ID_JL5105 || chip == CHIP_ID_JL5108)
		return JL_ERR_CONFIG;

#if (defined CONFIG_JLFE_EXT_PORT0_MII_MAC_MODE) || (defined CONFIG_JLFE_EXT_PORT0_MII_PHY_MODE)
	if (chip == CHIP_ID_JL5104) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
				"JL5104 support RMII mode only, check your EXT_PORT0 config\n");
		return JL_ERR_CONFIG;
	}
#endif

		return JL_ERR_OK;
	}

static void __setup_5110_ext_port_mode(jl_uint32 *cpu_reserved0_val)
{
	jl_uint32 val = *cpu_reserved0_val;

	/* setup MAC8 */
	do {
#ifdef CONFIG_JLFE_EXT_PORT0_DISABLE_MODE
		CLR_BIT(val, 7); //disable MAC8
#elif defined CONFIG_JLFE_EXT_PORT0_MII_MAC_MODE
		SET_BIT(val, 7); //enable MAC8
		CLR_BIT(val, 4); //MII
		CLR_BIT(val, 5); //MAC_MODE: clock_out=0
#elif defined CONFIG_JLFE_EXT_PORT0_MII_PHY_MODE
		SET_BIT(val, 7); //enable MAC8
		CLR_BIT(val, 4); //MII
		SET_BIT(val, 5); //PHY_MODE: clock_out=1
#elif defined CONFIG_JLFE_EXT_PORT0_RMII_MAC_MODE
		SET_BIT(val, 7); //enable MAC8
		SET_BIT(val, 4); //RMII
		CLR_BIT(val, 5); //PHY_MODE: clock_out=0
#elif defined CONFIG_JLFE_EXT_PORT0_RMII_PHY_MODE
		SET_BIT(val, 7); //enable MAC8
		SET_BIT(val, 4); //RMII
		SET_BIT(val, 5); //PHY_MODE: clock_out=1
#endif
	} while (0);

	/* setup MAC9 */
	do {
#if defined CONFIG_JLFE_EXT_PORT1_DISABLE_MODE
		CLR_BIT(val, 15); //disable MAC9
#elif defined CONFIG_JLFE_EXT_PORT1_RMII_MAC_MODE
		SET_BIT(val, 15); //enable MAC9
		SET_BIT(val, 12); //RMII
		CLR_BIT(val, 13); //MAC_MODE: clock_out=0
#elif defined CONFIG_JLFE_EXT_PORT1_RMII_PHY_MODE
		SET_BIT(val, 15); //enable MAC9
		SET_BIT(val, 12); //RMII
		SET_BIT(val, 13); //PHY_MODE: clock_out=1
#endif
	} while (0);


	*cpu_reserved0_val = val;
}

static void __setup_mac9_mode(jl_uint32 *cpu_reserved0_val)
{
	jl_uint32 val = *cpu_reserved0_val;

	/* setup MAC9 */
	do {
#ifdef CONFIG_JLFE_EXT_PORT0_DISABLE_MODE
		CLR_BIT(val, 15); //disable mac9
#elif defined CONFIG_JLFE_EXT_PORT0_MII_MAC_MODE
		SET_BIT(val, 15); //enable mac9
		CLR_BIT(val, 12); //MII
		CLR_BIT(val, 13); //MAC_MODE: clock_out=0
#elif defined CONFIG_JLFE_EXT_PORT0_MII_PHY_MODE
		SET_BIT(val, 15); //enable mac9
		CLR_BIT(val, 12); //MII
		SET_BIT(val, 13); //PHY_MODE: clock_out=1
#elif defined CONFIG_JLFE_EXT_PORT0_RMII_MAC_MODE
		SET_BIT(val, 15); //enable mac9
		SET_BIT(val, 12); //RMII
		CLR_BIT(val, 13); //MAC_MODE: clock_out=0
#elif defined CONFIG_JLFE_EXT_PORT0_RMII_PHY_MODE
		SET_BIT(val, 15); //enable mac9
		SET_BIT(val, 12); //RMII
		SET_BIT(val, 13); //PHY_MODE: clock_out=1
#endif
	} while (0);

	*cpu_reserved0_val = val;
}

jl_ret_t _force_ext_port_mode(void)
{
	jl_ret_t ret;
	jl_uint32 cpu_reserved0_val;
	jl_uint32 rsvd0_addr = APB_FRONTEND_BASE + CPU_RESERVED0_OFFSET;

	ret = __check_ext_port_mode_param();
	if (ret == JL_ERR_CONFIG) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO,
			"Wrong rmii/mii config, skip the ext port mode setting!\n");
		return JL_ERR_OK; /* Skip wrong configs */
	}

	ret = jl_apb_reg_read(rsvd0_addr, &cpu_reserved0_val);
		if (ret)
			return ret;

	if (gp_switch_dev->chip_id == CHIP_ID_JL5110)
		__setup_5110_ext_port_mode(&cpu_reserved0_val);
	else
		__setup_mac9_mode(&cpu_reserved0_val);

	ret = jl_apb_reg_write(rsvd0_addr, cpu_reserved0_val);
		if (ret)
			return ret;

	ret = _force_soft_reset(0, 0, 0, 0);
	return ret;
}

jl_ret_t _uplink_mac_set_link(void)
{
	jl_ret_t ret;
	jl_port_t phy_port;
	jl_uint64 mac_is_link;
	jl_uint32 mac_user_ctrl_val[1];
	jl_uint8 *preg_val = NULL;

	preg_val = (jl_uint8 *)&mac_user_ctrl_val[0];

	JL_FOR_EACH_PHY_EXT_PORT(phy_port) {
		ret = jl51xx_mac_uctrl_read(phy_port, &mac_user_ctrl_val[0]);
		if (ret)
			return ret;

		mac_is_link = 1;
		jl_write_bits(preg_val, mac_is_link, 0, 1);

		ret = jl51xx_mac_uctrl_write(phy_port, mac_user_ctrl_val[0]);
		if (ret)
			return ret;
	}

	return JL_ERR_OK;
}

jl_ret_t _chk_core_ver(void)
{
	jl_ret_t ret;
	jl_uint64 core_ver = 0;
	jl_uint32 reg_val[1] = {0};
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = (jl_uint8 *)&reg_val[0];

	/* check switch core version */
	reg_addr = SWCORE_BASE + CORE_VERSION_OFFSET;
	ret = jl_apb_reg_burst_read(reg_addr, &reg_val[0], 1);
	if (ret)
		return ret;

	core_ver = 0;
	jl_read_bits(preg_val, &core_ver, 0, 16);

	switch (core_ver) {
	case CORE_VER0:
		g_switch_ctrl.core_ver = CORE_VER0;
		break;
	case CORE_VER1:
		g_switch_ctrl.core_ver = CORE_VER1;
		break;
	default:
		return JL_ERR_BADID;
	}

	return JL_ERR_OK;
}

jl_ret_t _chk_pkg_mode(void)
{
	jl_ret_t ret;
	jl_uint64 chip_id = 0;
	jl_uint32 reg_val[1] = {0};
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = (jl_uint8 *)&reg_val[0];

	reg_addr = APB_FRONTEND_BASE + CPU_RESERVED0_OFFSET;
	reg_val[0] = 0;
	ret = jl_apb_reg_burst_read(reg_addr, &reg_val[0], 1);
	if (ret)
		return ret;

	chip_id = 0;
	jl_read_bits(preg_val, &chip_id, 28, 4);

#ifdef CONFIG_JLFE_CHIP_ID_51XX
	switch (chip_id) {
	case (CHIP_ID_JL5104):
		gp_switch_dev = &g_switch_dev_5104;
		break;
	case (CHIP_ID_JL5105):
		gp_switch_dev = &g_switch_dev_5105;
		break;
	case (CHIP_ID_JL5106):
		gp_switch_dev = &g_switch_dev_5106;
		break;
	case (CHIP_ID_JL5108):
		gp_switch_dev = &g_switch_dev_5108;
		break;
	case (CHIP_ID_JL5109):
		gp_switch_dev = &g_switch_dev_5109;
		break;
	case (CHIP_ID_JL5110):
		gp_switch_dev = &g_switch_dev_5110;
		break;
	default:
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
			"Unknown package mode [0x%llx]!!!\n", chip_id);
		return JL_ERR_BADID;
	}
#else
	if (chip_id != gp_switch_dev->chip_id) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
			"Wrong package mode [0x%x], expect [0x%llx]!!!\n",
			gp_switch_dev->chip_id, chip_id);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
			"Package mode:JL5104[0x0], JL5105[0x1], JL5106[0x2], JL5108[0x3], JL5109[0x4], JL5110[0x5]\n");

		return JL_ERR_BADID;
	}
#endif
	return JL_ERR_OK;
}

jl_ret_t _get_eco_ver(void)
{
	jl_ret_t ret;
	jl_uint64 eco_ver = 0;
	jl_uint32 reg_val[1] = {0};
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = (jl_uint8 *)&reg_val[0];

	reg_addr = APB_FRONTEND_BASE + CPU_RESERVED6_OFFSET;
	reg_val[0] = 0;
	ret = jl_apb_reg_burst_read(reg_addr, &reg_val[0], 1);
	if (ret)
		return ret;

	eco_ver = 0;
	jl_read_bits(preg_val, &eco_ver, 0, 4);

	g_switch_ctrl.eco_ver = eco_ver;

	return JL_ERR_OK;
}

jl_ret_t _check_hw_info(void)
{
	jl_ret_t ret;

	/* check switch core version */
	ret = _chk_core_ver();
	if (ret)
		return ret;

	/* check package mode */
	ret = _chk_pkg_mode();
	if (ret)
		return ret;

	/* get eco version */
	ret = _get_eco_ver();
	if (ret)
		return ret;

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Switch Chip HW information:\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\tpackage mode: [0x%x]\n",
						gp_switch_dev->chip_id);
	return JL_ERR_OK;
}

/* register LOAD_DATA4 stores fw flag information
 * LOAD_DATA4 page_id = 7, reg_id = 21;
 */
#define JL_FW_FLAG_PROCESS		0xb
jl_ret_t _check_fw_stat(void)
{
	jl_uint16 load_data4 = 0;

	load_data4 = jl_phy_reg_read_ext(0, 7, 21);
	if (load_data4 != JL_FW_FLAG_PROCESS) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
				"Switch Initialize fail with wrong firmware flag [%d]!!!\n",
				load_data4);
		return JL_ERR_NOT_READY;
	}

	return JL_ERR_OK;
}

#ifdef CONFIG_JLFE_LOAD_PATCH
#define PHY_ID(x)		(((x) >> 0x15U) & 0x1FU)
#define PAGE_ID(x)		(((x) >> 0x5U) & 0xFFFFU)
#define REG_ID(x)		((x) & 0x1FU)
#define MAX_BURST_SIZE		8U

static jl_ret_t __do_write(struct jl_cmd_line_s *cmd_line)
{
	jl_api_ret_t ret = JL_ERR_OK;

	if ((cmd_line->flag != JL_CMD_FLAG_DA) &&
			((cmd_line->flag != JL_CMD_FLAG_IDA))) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Abort, Unknown WRITE command flag!!!\n");
		return JL_ERR_PARAM;
	}

	if (cmd_line->flag == JL_CMD_FLAG_DA)
		jl_phy_reg_write_ext(PHY_ID(cmd_line->addr),
					PAGE_ID(cmd_line->addr),
					REG_ID(cmd_line->addr),
					(jl_uint16)cmd_line->val);
	else if (cmd_line->flag == JL_CMD_FLAG_IDA) {
		ret = jl_apb_reg_write(cmd_line->addr, cmd_line->val);
		if (ret)
			return ret;
	}

	return ret;
}

static jl_ret_t __do_burst_write(struct jl_cmd_line_s *cmd_line)
{
	jl_api_ret_t ret = JL_ERR_OK;
	jl_uint32 burst_val[8] = {0};
	jl_uint32 size = (jl_uint32)cmd_line->flag;
	jl_uint32 i = 0;

	if ((size == 0) || (size > MAX_BURST_SIZE)) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Abort, BURST WRITE command with wrong size!!!\n");
		return JL_ERR_PARAM;
	}

	for (i = 0; i < size; i++) {
		if ((i > 0)  && (cmd_line[i].cmd ||
				cmd_line[i].flag ||
				cmd_line[i].addr)) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Abort, BRUST WRITE with wrong burst value format!!!\n");
			return JL_ERR_PARAM;
		}

		burst_val[i] = cmd_line[i].val;
	}

	ret = jl_apb_reg_burst_write(cmd_line->addr, &burst_val[0], size);
	if (ret)
		return ret;

	return (jl_ret_t)size;
}

#ifdef CONFIG_JLFE_LOAD_FORMAL_PATCH
static jl_ret_t __patch_select(void)
{
	jl_uint8 i;

	for (i = 0; g_eco_patch_tbl[i].pkg_ver != 0xff; i++) {
		if ((gp_switch_dev->chip_id == g_eco_patch_tbl[i].pkg_ver)
			&& (g_switch_ctrl.eco_ver == g_eco_patch_tbl[i].eco_ver)) {
			g_cmd_line_desc.p_line_set = g_eco_patch_tbl[i].peco_patch;
			g_cmd_line_desc.lines = g_eco_patch_tbl[i].eco_patch_size;

			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "patch size[%d]lines\n",
					g_eco_patch_tbl[i].eco_patch_size);

			return JL_ERR_OK;
		}
	}

	return JL_ERR_UNAVAIL;
}
#endif

jl_ret_t _load_source_patch(void)
{
	jl_uint32 lines = 0;
	jl_uint32 i = 0;
	struct jl_cmd_line_s *cmd_line = NULL;
	jl_ret_t ret = 0;

#ifdef CONFIG_JLFE_LOAD_FORMAL_PATCH
	ret = __patch_select();
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Could not find patch file, skip load patch!!!\n");
		return JL_ERR_OK;
	}

#endif

	lines = g_cmd_line_desc.lines;

	while (lines) {
		cmd_line = &g_cmd_line_desc.p_line_set[i];

		switch (cmd_line->cmd) {
		case (JL_CMD_WRITE):
			ret = __do_write(cmd_line);
			if (ret)
				goto error;

			lines--;
			i++;
			break;
		case (JL_CMD_BWRITE):
			ret = __do_burst_write(cmd_line);
			if (ret < 0)
				goto error;

			lines -= ret;
			i += ret;
			break;
		case (JL_CMD_DELAY):
			port_udelay(cmd_line->val);
#ifdef CONFIG_JLFE_DUMP_REG_WRITE
			jl_dump_add_delay(cmd_line->val);
#endif

			lines--;
			i++;
			break;
		default:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Abort, Unknown command!!!\n");
			goto error;
		}
	}

	return JL_ERR_OK;

error:
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Load source patch fail!!!\n");
	return JL_ERR_FAIL;
}
#endif

jl_ret_t jl51xx_sw_core_init(void)
{
	jl_ret_t ret;
	jl_uint64 allow_bpdu, disable, answer, tickcnt, tickindex;
	jl_uint32 send2cpu_val[2];
	jl_uint32 disable_cpu_tag_val[1];
	jl_uint32 vid2vtab_tcam_answer_val[1];
	jl_uint32 time2age_val[2];
	jl_uint32 reg_addr = 0;
	jl_uint8 *preg_val = NULL;

	ret = _check_fw_stat();
	if (ret)
		return ret;

	ret = _check_hw_info();
	if (ret)
		return ret;

	/* disable led group 0 */
	/* XXX rm this line, if led reset status is fixed in fw */
	_led_enable_set(0, 0);
	_led_reset_blink_set(0);

#ifdef CONFIG_JLFE_LOAD_PATCH
	ret = _load_source_patch();
	if (ret)
		return ret;
#endif

#ifdef CONFIG_JLFE_EXT_PORT_MODE
	ret = _force_ext_port_mode();
	if (ret)
		return ret;
#endif

	/* XXX rm this line, if led reset status is fixed in fw */
	jl51xx_led_group_active_high_set(LED_GROUP0,
			gp_switch_dev->phy_portmask & UTP_PORT_MASK);

	/* enable led group 0 */
	_led_enable_set(0, 1);

	ret = _uplink_mac_set_link();
	if (ret)
		return ret;

	ret = jl51xx_cpu_allow_lldp_enable_set(DISABLED);
	if (ret)
		return ret;

	do {
		reg_addr  = SWCORE_BASE + SEND_TO_CPU_OFFSET;
		preg_val = (jl_uint8 *)&send2cpu_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &send2cpu_val[0], 2);
		if (ret)
			return ret;

		allow_bpdu = 0;
		jl_write_bits(preg_val, allow_bpdu, 0, 1);

		ret = jl_apb_reg_burst_write(reg_addr, &send2cpu_val[0], 2);
		if (ret)
			return ret;
	} while (0);

	do {
		reg_addr  = SWCORE_BASE + DISABLE_CPU_TAG_ON_CPU_PORT_OFFSET;
		preg_val = (jl_uint8 *)&disable_cpu_tag_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &disable_cpu_tag_val[0], 1);
		if (ret)
			return ret;

		disable = 1;
		jl_write_bits(preg_val, disable, 0, 1);

		ret = jl_apb_reg_burst_write(reg_addr, &disable_cpu_tag_val[0], 1);
		if (ret)
			return ret;
	} while (0);

	/* disable vlan by default */
	do {
		reg_addr = SWCORE_BASE + VID_TO_VLAN_TABLE_TCAM_ANSWER_OFFSET + VLAN_IDX_DROP * 1;
		preg_val = (jl_uint8 *)&vid2vtab_tcam_answer_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &vid2vtab_tcam_answer_val[0], 1);
		if (ret)
			return ret;

		answer = VLAN_IDX_DEFAULT;
		jl_write_bits(preg_val, answer, 0, 4);

		ret = jl_apb_reg_burst_write(reg_addr, &vid2vtab_tcam_answer_val[0], 1);
		if (ret)
			return ret;
	} while (0);

	/* set time to age tickCnt:0x7735940 by default */
	do {
		reg_addr = SWCORE_BASE + TIME_TO_AGE_OFFSET;
		preg_val = (jl_uint8 *)&time2age_val[0];
		ret = jl_apb_reg_burst_read(reg_addr, &time2age_val[0], 2);
		if (ret)
			return ret;

		tickcnt = DEFAULT_TICK_NUM;
		tickindex = DEFAULT_TICK_ID;
		jl_write_bits(preg_val, tickcnt, 0, 32);
		jl_write_bits(preg_val, tickindex, 32, 3);
		ret = jl_apb_reg_burst_write(reg_addr, &time2age_val[0], 2);
		if (ret)
			return ret;
	} while (0);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_sw_core_deinit(void)
{
	jl_ret_t ret;

	ret = _force_soft_reset(1, 1, 1, 1);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "soft reset fail!\n"); \

	return ret;
}

