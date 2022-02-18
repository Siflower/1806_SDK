/*
 * Library to support early TI EVM EEPROM handling
 *
 * Copyright (C) 2015-2016 Texas Instruments Incorporated - http://www.ti.com/
 *	Lokesh Vutla
 *	Steve Kipisz
 *
 * SPDX-License-Identifier:    GPL-2.0+
 */

#include <common.h>
#include <asm/omap_common.h>
#include <i2c.h>

#include "board_detect.h"

/**
 * ti_i2c_eeprom_init - Initialize an i2c bus and probe for a device
 * @i2c_bus: i2c bus number to initialize
 * @dev_addr: Device address to probe for
 *
 * Return: 0 on success or corresponding error on failure.
 */
static int __maybe_unused ti_i2c_eeprom_init(int i2c_bus, int dev_addr)
{
	int rc;

	if (i2c_bus >= 0) {
		rc = i2c_set_bus_num(i2c_bus);
		if (rc)
			return rc;
	}

	return i2c_probe(dev_addr);
}

/**
 * ti_i2c_eeprom_read - Read data from an EEPROM
 * @dev_addr: The device address of the EEPROM
 * @offset: Offset to start reading in the EEPROM
 * @ep: Pointer to a buffer to read into
 * @epsize: Size of buffer
 *
 * Return: 0 on success or corresponding result of i2c_read
 */
static int __maybe_unused ti_i2c_eeprom_read(int dev_addr, int offset,
					     uchar *ep, int epsize)
{
	return i2c_read(dev_addr, offset, 2, ep, epsize);
}

/**
 * ti_eeprom_string_cleanup() - Handle eeprom programming errors
 * @s:	eeprom string (should be NULL terminated)
 *
 * Some Board manufacturers do not add a NULL termination at the
 * end of string, instead some binary information is kludged in, hence
 * convert the string to just printable characters of ASCII chart.
 */
static void __maybe_unused ti_eeprom_string_cleanup(char *s)
{
	int i, l;

	l = strlen(s);
	for (i = 0; i < l; i++, s++)
		if (*s < ' ' || *s > '~') {
			*s = 0;
			break;
		}
}

__weak void gpi2c_init(void)
{
}

static int __maybe_unused ti_i2c_eeprom_get(int bus_addr, int dev_addr,
					    u32 header, u32 size, uint8_t *ep)
{
	u32 byte, hdr_read;
	int rc;

	gpi2c_init();
	rc = ti_i2c_eeprom_init(bus_addr, dev_addr);
	if (rc)
		return rc;

	/*
	 * Read the header first then only read the other contents.
	 */
	byte = 2;
	rc = i2c_read(dev_addr, 0x0, byte, (uint8_t *)&hdr_read, 4);
	if (rc)
		return rc;

	/* Corrupted data??? */
	if (hdr_read != header) {
		rc = i2c_read(dev_addr, 0x0, byte, (uint8_t *)&hdr_read, 4);
		/*
		 * read the eeprom header using i2c again, but use only a
		 * 1 byte address (some legacy boards need this..)
		 */
		byte = 1;
		if (rc)
			rc = i2c_read(dev_addr, 0x0, byte, (uint8_t *)&hdr_read,
				      4);
		if (rc)
			return rc;
	}
	if (hdr_read != header)
		return -1;

	rc = i2c_read(dev_addr, 0x0, byte, ep, size);
	if (rc)
		return rc;

	return 0;
}

int __maybe_unused ti_i2c_eeprom_am_get(int bus_addr, int dev_addr)
{
	int rc;
	struct ti_am_eeprom am_ep;
	struct ti_common_eeprom *ep;

	ep = TI_EEPROM_DATA;
	if (ep->header == TI_EEPROM_HEADER_MAGIC)
		goto already_read;

	/* Initialize with a known bad marker for i2c fails.. */
	ep->header = TI_DEAD_EEPROM_MAGIC;
	ep->name[0] = 0x0;
	ep->version[0] = 0x0;
	ep->serial[0] = 0x0;

	rc = ti_i2c_eeprom_get(bus_addr, dev_addr, TI_EEPROM_HEADER_MAGIC,
			       sizeof(am_ep), (uint8_t *)&am_ep);
	if (rc)
		return rc;

	ep->header = am_ep.header;
	strlcpy(ep->name, am_ep.name, TI_EEPROM_HDR_NAME_LEN + 1);
	ti_eeprom_string_cleanup(ep->name);

	/* BeagleBone Green '1' eeprom, board_rev: 0x1a 0x00 0x00 0x00 */
	if (am_ep.version[0] == 0x1a && am_ep.version[1] == 0x00 &&
	    am_ep.version[2] == 0x00 && am_ep.version[3] == 0x00)
		strlcpy(ep->version, "BBG1", TI_EEPROM_HDR_REV_LEN + 1);
	else
		strlcpy(ep->version, am_ep.version, TI_EEPROM_HDR_REV_LEN + 1);
	ti_eeprom_string_cleanup(ep->version);
	strlcpy(ep->serial, am_ep.serial, TI_EEPROM_HDR_SERIAL_LEN + 1);
	ti_eeprom_string_cleanup(ep->serial);
	strlcpy(ep->config, am_ep.config, TI_EEPROM_HDR_CONFIG_LEN + 1);
	ti_eeprom_string_cleanup(ep->config);

	memcpy(ep->mac_addr, am_ep.mac_addr,
	       TI_EEPROM_HDR_NO_OF_MAC_ADDR * TI_EEPROM_HDR_ETH_ALEN);

already_read:
	return 0;
}

int __maybe_unused ti_i2c_eeprom_dra7_get(int bus_addr, int dev_addr)
{
	int rc, offset = 0;
	struct dra7_eeprom dra7_ep;
	struct ti_common_eeprom *ep;

	ep = TI_EEPROM_DATA;
	if (ep->header == DRA7_EEPROM_HEADER_MAGIC)
		goto already_read;

	/* Initialize with a known bad marker for i2c fails.. */
	ep->header = 0xADEAD12C;
	ep->name[0] = 0x0;
	ep->version[0] = 0x0;
	ep->serial[0] = 0x0;
	ep->emif1_size = 0;
	ep->emif2_size = 0;

	rc = ti_i2c_eeprom_get(bus_addr, dev_addr, DRA7_EEPROM_HEADER_MAGIC,
			       sizeof(dra7_ep), (uint8_t *)&dra7_ep);
	if (rc)
		return rc;

	ep->header = dra7_ep.header;
	strlcpy(ep->name, dra7_ep.name, TI_EEPROM_HDR_NAME_LEN + 1);
	ti_eeprom_string_cleanup(ep->name);

	offset = dra7_ep.version_major - 1;

	/* Rev F is skipped */
	if (offset >= 5)
		offset = offset + 1;
	snprintf(ep->version, TI_EEPROM_HDR_REV_LEN + 1, "%c.%d",
		 'A' + offset, dra7_ep.version_minor);
	ti_eeprom_string_cleanup(ep->version);
	ep->emif1_size = (u64)dra7_ep.emif1_size;
	ep->emif2_size = (u64)dra7_ep.emif2_size;
	strlcpy(ep->config, dra7_ep.config, TI_EEPROM_HDR_CONFIG_LEN + 1);
	ti_eeprom_string_cleanup(ep->config);

already_read:
	return 0;
}

bool __maybe_unused board_ti_is(char *name_tag)
{
	struct ti_common_eeprom *ep = TI_EEPROM_DATA;

	if (ep->header == TI_DEAD_EEPROM_MAGIC)
		return false;
	return !strncmp(ep->name, name_tag, TI_EEPROM_HDR_NAME_LEN);
}

bool __maybe_unused board_ti_rev_is(char *rev_tag, int cmp_len)
{
	struct ti_common_eeprom *ep = TI_EEPROM_DATA;
	int l;

	if (ep->header == TI_DEAD_EEPROM_MAGIC)
		return false;

	l = cmp_len > TI_EEPROM_HDR_REV_LEN ? TI_EEPROM_HDR_REV_LEN : cmp_len;
	return !strncmp(ep->version, rev_tag, l);
}

char * __maybe_unused board_ti_get_rev(void)
{
	struct ti_common_eeprom *ep = TI_EEPROM_DATA;

	if (ep->header == TI_DEAD_EEPROM_MAGIC)
		return NULL;

	return ep->version;
}

char * __maybe_unused board_ti_get_config(void)
{
	struct ti_common_eeprom *ep = TI_EEPROM_DATA;

	if (ep->header == TI_DEAD_EEPROM_MAGIC)
		return NULL;

	return ep->config;
}

char * __maybe_unused board_ti_get_name(void)
{
	struct ti_common_eeprom *ep = TI_EEPROM_DATA;

	if (ep->header == TI_DEAD_EEPROM_MAGIC)
		return NULL;

	return ep->name;
}

void __maybe_unused
board_ti_get_eth_mac_addr(int index,
			  u8 mac_addr[TI_EEPROM_HDR_ETH_ALEN])
{
	struct ti_common_eeprom *ep = TI_EEPROM_DATA;

	if (ep->header == TI_DEAD_EEPROM_MAGIC)
		goto fail;

	if (index < 0 || index >= TI_EEPROM_HDR_NO_OF_MAC_ADDR)
		goto fail;

	memcpy(mac_addr, ep->mac_addr[index], TI_EEPROM_HDR_ETH_ALEN);
	return;

fail:
	memset(mac_addr, 0, TI_EEPROM_HDR_ETH_ALEN);
}

u64 __maybe_unused board_ti_get_emif1_size(void)
{
	struct ti_common_eeprom *ep = TI_EEPROM_DATA;

	if (ep->header != DRA7_EEPROM_HEADER_MAGIC)
		return 0;

	return ep->emif1_size;
}

u64 __maybe_unused board_ti_get_emif2_size(void)
{
	struct ti_common_eeprom *ep = TI_EEPROM_DATA;

	if (ep->header != DRA7_EEPROM_HEADER_MAGIC)
		return 0;

	return ep->emif2_size;
}

void __maybe_unused set_board_info_env(char *name)
{
	char *unknown = "unknown";
	struct ti_common_eeprom *ep = TI_EEPROM_DATA;

	if (name)
		setenv("board_name", name);
	else if (ep->name)
		setenv("board_name", ep->name);
	else
		setenv("board_name", unknown);

	if (ep->version)
		setenv("board_rev", ep->version);
	else
		setenv("board_rev", unknown);

	if (ep->serial)
		setenv("board_serial", ep->serial);
	else
		setenv("board_serial", unknown);
}
