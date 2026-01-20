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

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "jl_error.h"
#include "jl_types.h"
#include "jl_debug.h"
#include "portable/jl_portable.h"
#include "driver/hal_smi.h"
#include "driver/demo_hidapi.h"

#define VID	0x483
#define PID	0xde

#define SMI_CLAUSE22		0x22

#ifdef WIN32
#define HID_STM32_CMD_MAX_SIZE	65
#else
#define HID_STM32_CMD_MAX_SIZE	64
#endif

/* cmd_index,length0,lenght1,clause22,deviceid */
jl_uint8 CMD_SMI_CFG[] = {0x30, 0x00, 0x00, 0x22, 0x00};
jl_uint8 CMD_SMI_WRITE[] = {0x31, 0x00, 0x00};
jl_uint8 CMD_SMI_READ[] = {0x32, 0x00, 0x00};

struct smi_device_s g_smi_dev;

jl_ret_t __hid_stm32_wait_resp(jl_uint8 *data)
{
	jl_ret_t ret;

	/* ret = hid_read(g_smi_dev.device, data, 64); */
	ret = hid_read_timeout(g_smi_dev.device, data, 64, 1000);
	if (ret <= 0) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "HID over STM32 response error, error code :%d\n", ret);
		return JL_ERR_FAIL;
	}

	return JL_ERR_OK;
}

jl_ret_t __hid_stm32_write(jl_uint8 *data, jl_uint8 size)
{
	jl_ret_t ret;
	jl_uint8 buf[65] = {0};

	if (size > HID_STM32_CMD_MAX_SIZE) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "HID over STM32 write buffer size overflow\n");
		return JL_ERR_OUT_OF_RANGE;
	}

#ifdef WIN32
	memcpy(&buf[1], data, size);
#else
	memcpy(&buf[0], data, size);
#endif

	ret = hid_write(g_smi_dev.device, buf, HID_STM32_CMD_MAX_SIZE);
	if (ret == -1) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "HID over STM32 write fail\n");
		return JL_ERR_FAIL;
	}

	return JL_ERR_OK;
}

jl_ret_t __hid_smi_config(void)
{
	jl_ret_t ret;
	jl_uint8 hid_smi_cmd[5] = {0};
	jl_uint8 hid_smi_resp[64] = {0};
	/* STM32 spi device id ??? */
	jl_uint8 dev = 0;

	memcpy(hid_smi_cmd, CMD_SMI_CFG, 5);
	hid_smi_cmd[3] = SMI_CLAUSE22;
	hid_smi_cmd[4] = dev;

	ret = __hid_stm32_write(hid_smi_cmd, 5);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "HID over STM32 config write fail\n");
		return JL_ERR_FAIL;
	}

	ret = __hid_stm32_wait_resp(hid_smi_resp);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "HID over STM32 config wait response fail\n");
		return JL_ERR_FAIL;
	} else if (hid_smi_resp[1]) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "HID over STM32 config response error code:%d\n", hid_smi_resp[1]);
		return JL_ERR_FAIL;
	}

	return JL_ERR_OK;
}

hid_device *__hid_device_init(void)
{
	jl_int32 ret;
	#define MAX_STR 255
	wchar_t wstr[MAX_STR];
	hid_device *handle;

	ret = hid_init();
	if (ret) {
		JL_DBG_MSG(JL_FLAG_ALL, _DBG_ERROR, "hid device init fail\n");
		return NULL;
	}

	handle = hid_open(VID, PID, NULL);
	if (!handle) {
		JL_DBG_MSG(JL_FLAG_ALL, _DBG_ERROR, "hid device open fail\n");
		hid_exit();
		return NULL;
	}

	// Read the Manufacturer String
	wstr[0] = 0x0000;
	ret = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	if (ret < 0)
		JL_DBG_MSG(JL_FLAG_ALL, _DBG_ERROR, "Unable to read manufacturer string\n");
	JL_DBG_MSG(JL_FLAG_SMI, _DBG_INFO, "Manufacturer String: %ls\n", wstr);

	// Read the Product String
	wstr[0] = 0x0000;
	ret = hid_get_product_string(handle, wstr, MAX_STR);
	if (ret < 0)
		JL_DBG_MSG(JL_FLAG_ALL, _DBG_ERROR, "Unable to read product string\n");
	JL_DBG_MSG(JL_FLAG_SMI, _DBG_INFO, "Product String: %ls\n", wstr);

	// Read the Serial Number String
	wstr[0] = 0x0000;
	ret = hid_get_serial_number_string(handle, wstr, MAX_STR);
	if (ret < 0)
		JL_DBG_MSG(JL_FLAG_ALL, _DBG_ERROR, "Unable to read serial number string\n");
	JL_DBG_MSG(JL_FLAG_SMI, _DBG_INFO, "Serial Number String: (%d) %ls", wstr[0], wstr);
	JL_DBG_MSG(JL_FLAG_SMI, _DBG_INFO, "\n");

	// Read Indexed String 1
	wstr[0] = 0x0000;
	ret = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	if (ret < 0)
		JL_DBG_MSG(JL_FLAG_ALL, _DBG_ERROR, "Unable to read indexed string 1\n");
	JL_DBG_MSG(JL_FLAG_SMI, _DBG_INFO, "Indexed String 1: %ls\n", wstr);

	// Set the hid_read() function to be non-blocking.
	/* hid_set_nonblocking(handle, 1); */

	return handle;
}

jl_ret_t __hid_device_deinit(void)
{
	if (g_smi_dev.device)
		hid_close(g_smi_dev.device);

	/* Free static HIDAPI objects. */
	hid_exit();

	return JL_ERR_OK;
}

jl_uint16 jl_smi_read(jl_uint8 phy, jl_uint8 reg)
{
	jl_ret_t ret;
	jl_uint8 hid_smi_frame[sizeof(CMD_SMI_READ) + 8] = {0};
	jl_uint8 hid_smi_resp[64] = {0};
	jl_uint8 *hid_smi_cmd = &hid_smi_frame[0];
	jl_uint8 *hid_smi_data = &hid_smi_frame[sizeof(CMD_SMI_READ)];
	jl_uint16 op_phy_reg = 0;

	/* operation: 0x6 for smi read */
	op_phy_reg = (0x6 << 12) | ((jl_uint16)(phy & 0x1f) << 7) | ((jl_uint16)(reg & 0x1f) << 2) | 0x2;
	hid_smi_data[0] = 0xff;
	hid_smi_data[1] = 0xff;
	hid_smi_data[2] = 0xff;
	hid_smi_data[3] = 0xff;
	hid_smi_data[4] = op_phy_reg >> 8;
	hid_smi_data[5] = op_phy_reg & 0xff;
	hid_smi_data[6] = 0xff;
	hid_smi_data[7] = 0xff;

	memcpy(hid_smi_cmd, CMD_SMI_READ, sizeof(CMD_SMI_READ));
	/* hid smi data size */
	hid_smi_cmd[1] = 8;

#ifdef CONFIG_JLFE_PHY_MUTEX
	port_mutex_lock(g_smi_dev.phy_mutex);
#endif

	ret = __hid_stm32_write(hid_smi_frame, sizeof(hid_smi_frame));
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "HID over STM32 smi read fail\n");
		exit(EXIT_FAILURE);
	}

	ret = __hid_stm32_wait_resp(hid_smi_resp);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "HID over STM32 smi write wait response fail\n");
		exit(EXIT_FAILURE);
	} else if (hid_smi_resp[1]) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "HID over STM32 smi write response error code:%d\n", hid_smi_resp[1]);
		exit(EXIT_FAILURE);
	}

#ifdef CONFIG_JLFE_PHY_MUTEX
	port_mutex_unlock(g_smi_dev.phy_mutex);
#endif

	return ((jl_uint16 ) (hid_smi_resp[sizeof(CMD_SMI_READ) + 6] << 8 | hid_smi_resp[sizeof(CMD_SMI_READ) + 7]));
}

void jl_smi_write(jl_uint8 phy, jl_uint8 reg, jl_uint16 val)
{
	jl_ret_t ret;
	jl_uint8 hid_smi_frame[sizeof(CMD_SMI_WRITE) + 8] = {0};
	jl_uint8 hid_smi_resp[64] = {0};
	jl_uint8 *hid_smi_cmd = &hid_smi_frame[0];
	jl_uint8 *hid_smi_data = &hid_smi_frame[sizeof(CMD_SMI_WRITE)];
	jl_uint16 op_phy_reg = 0;

	/* operation: 0x5 for smi write */
	op_phy_reg = (0x5 << 12) | ((jl_uint16)(phy & 0x1f) << 7) | ((jl_uint16)(reg & 0x1f) << 2) | 0x2;
	hid_smi_data[0] = 0xff;
	hid_smi_data[1] = 0xff;
	hid_smi_data[2] = 0xff;
	hid_smi_data[3] = 0xff;
	hid_smi_data[4] = op_phy_reg >> 8;
	hid_smi_data[5] = op_phy_reg & 0xff;
	hid_smi_data[6] = val >> 8;
	hid_smi_data[7] = val & 0xff;

	memcpy(hid_smi_cmd, CMD_SMI_WRITE, sizeof(CMD_SMI_WRITE));
	/* hid smi data size */
	hid_smi_cmd[1] = 8;

#ifdef CONFIG_JLFE_PHY_MUTEX
	port_mutex_lock(g_smi_dev.phy_mutex);
#endif

	ret = __hid_stm32_write(hid_smi_frame, sizeof(hid_smi_frame));
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "HID over STM32 smi write fail\n");
		exit(EXIT_FAILURE);
	}

	ret = __hid_stm32_wait_resp(hid_smi_resp);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "HID over STM32 smi write wait response fail\n");
		exit(EXIT_FAILURE);
	} else if (hid_smi_resp[1]) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "HID over STM32 smi write response error code:%d\n", hid_smi_resp[1]);
		exit(EXIT_FAILURE);
	}

#ifdef CONFIG_JLFE_PHY_MUTEX
	port_mutex_unlock(g_smi_dev.phy_mutex);
#endif
}

jl_api_ret_t jl_smi_init(void)
{
	/* jl_ret_t ret; */

#ifdef CONFIG_JLFE_PHY_MUTEX
	g_smi_dev.phy_mutex = port_mutex_init();
	if (!g_smi_dev.phy_mutex)
		return JL_ERR_INIT;
#endif

#ifdef CONFIG_JLFE_APB_MUTEX
	g_smi_dev.apb_mutex = port_mutex_init();
	if (!g_smi_dev.apb_mutex)
		return JL_ERR_INIT;
#endif
	g_smi_dev.device = __hid_device_init();
	if (!g_smi_dev.device)
		exit(EXIT_FAILURE);

	/* ret = __hid_smi_config(); */
	/* if (ret) */
	/* 	exit(EXIT_FAILURE); */

	return JL_ERR_OK;
}

jl_api_ret_t jl_smi_deinit(void)
{
#ifdef CONFIG_JLFE_PHY_MUTEX
	if (g_smi_dev.phy_mutex) {
		port_mutex_deinit(g_smi_dev.phy_mutex);
		g_smi_dev.phy_mutex = NULL;
	}
#endif

#ifdef CONFIG_JLFE_APB_MUTEX
	if (g_smi_dev.apb_mutex) {
		port_mutex_deinit(g_smi_dev.apb_mutex);
		g_smi_dev.apb_mutex = NULL;
	}
#endif

	return __hid_device_deinit();
}
