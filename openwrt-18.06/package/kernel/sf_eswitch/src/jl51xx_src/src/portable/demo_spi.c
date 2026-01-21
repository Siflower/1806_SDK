#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "jl_types.h"
#include "jl_error.h"
#include "jl_debug.h"

/* A SPI (over FT232H) porting example. */

#include <ftd2xx.h>

#ifdef CONFIG_CROSS_COMPILER_MSVC
#include <Windows.h>
#else
#include <unistd.h>
#endif

#ifdef CONFIG_CROSS_COMPILER_MSVC
#define MSLEEP Sleep
#else
#define MSLEEP(t) usleep((t) * 1000)
#endif


/* SPI_MODE = 0, CPOL = 0, CPHA = 0 */
/* SPI_MODE = 1, CPOL = 0, CPHA = 1 */
/* SPI_MODE = 2, CPOL = 1, CPHA = 0 */
/* SPI_MODE = 3  CPOL = 1, CPHA = 1 */
#define SPI_MODE	1
#define SPI_BITS	8
#define SPI_SPEED	400000 /* 400KHz, 800KHz, 1600KHz */
#define SPI_DELAY_USEC	0
#define SPI_CS_CHANGE	0

#define SPI_DEVICE	"/dev/spidev0.0"

static FT_HANDLE ftdih;

static const char *ftd2xx_status_string(FT_STATUS status)
{
	switch (status) {
	case FT_OK:						return "OK";
	case FT_INVALID_HANDLE:					return "invalid handle";
	case FT_DEVICE_NOT_FOUND:				return "device not found";
	case FT_DEVICE_NOT_OPENED:				return "device not opened";
	case FT_IO_ERROR:					return "io error";
	case FT_INSUFFICIENT_RESOURCES:				return "insufficient resources";
	case FT_INVALID_PARAMETER:				return "invalid parameter";
	case FT_INVALID_BAUD_RATE:				return "invalid baud rate";

	case FT_DEVICE_NOT_OPENED_FOR_ERASE:			return "device not opened for erase";
	case FT_DEVICE_NOT_OPENED_FOR_WRITE:			return "device not opened for write";
	case FT_FAILED_TO_WRITE_DEVICE:				return "failed to write device";
	case FT_EEPROM_READ_FAILED:				return "eeprom read failed";
	case FT_EEPROM_WRITE_FAILED:				return "eeprom write failed";
	case FT_EEPROM_ERASE_FAILED:				return "eeprom erase failed";
	case FT_EEPROM_NOT_PRESENT:				return "eeprom not present";
	case FT_EEPROM_NOT_PROGRAMMED:				return "eeprom not programmed";
	case FT_INVALID_ARGS:					return "invalid args";
	case FT_NOT_SUPPORTED:					return "not supported";
	case FT_OTHER_ERROR:					return "other error";
	}

	return "undefined FTD2xx error";
}

static jl_ret_t jl_mdio_init_ftd2xx(void)
{
	FT_STATUS status;
	DWORD dw_bytes_read;
	DWORD nrecv;
	jl_uint8 buf[32];

	status = FT_Open(0, &ftdih);
	if (status != FT_OK) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "unable to open ftdi device\n");
		return JL_ERR_RESOURCE;
	}

	/* Reset USB device */
	status = FT_ResetDevice(ftdih);
	if (status != FT_OK) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "failed to reset ftdi device");
		return JL_ERR_INIT;
	}

	/* Purge USB receive buffer first by reading out all old data from
	 * FT2232H receive buffer */

	/* Get the number of bytes in the FT2232H */
	dw_bytes_read = 0;
	status = FT_GetQueueStatus(ftdih, &dw_bytes_read);
	if (status != FT_OK) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "failed to FT_GetQueueStatus");
		return JL_ERR_FAIL;
	}

	/* Read out the data from FT2232H receive buffer */
	if ((status == FT_OK) && (dw_bytes_read > 0)) {
		FT_Read(ftdih, &buf, dw_bytes_read, &nrecv);
		if (status != FT_OK) {
			JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "unable to read, status: %s\n",
				ftd2xx_status_string(status));
			return JL_ERR_FAIL;
		}
	}

	status = FT_SetTimeouts(ftdih, 100, 5000);
	if (status != FT_OK) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "unable to set timeouts: %s\n",
			ftd2xx_status_string(status));
		return JL_ERR_FAIL;
	}

	status = FT_SetBitMode(ftdih, 0x0b, 2);
	if (status != FT_OK) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "unable to enable bit i/o mode: %s\n",
			ftd2xx_status_string(status));
		return JL_ERR_FAIL;
	}

	MSLEEP(5);

	return JL_ERR_OK;
}

jl_ret_t jl_mdio_config(void)
{
	FT_STATUS status;
	/* SCL Frequency = 40/((1+0x004F)*2) (MHz) = 250khz */
	DWORD dw_clk_div_mdio = 0x7; /* TODO: should be a config option */
	DWORD dw_bytes_send;
	DWORD dw_bytes_sent;
	jl_uint8 buf[32];

	dw_bytes_send = 0;
	/* Ensure disable clock divide by 5 for 60Mhz master clock */
	buf[dw_bytes_send++] = (jl_uint8)0x8A;
	/* Ensure turn off adaptive clocking */
	buf[dw_bytes_send++] = (jl_uint8)0x97;
	/* 0x8D/0x8C Disable/Enable 3 phase data clock, used by I2C to allow data on both clock edges */
	buf[dw_bytes_send++] = (jl_uint8)0x8D;

	/* Send off the commands */
	status = FT_Write(ftdih, buf, dw_bytes_send, &dw_bytes_sent);
	if (status != FT_OK) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "failed to FT_Write: %s\n",
			ftd2xx_status_string(status));
		return JL_ERR_FAIL;
	}

	/* Clear output buffer */
	dw_bytes_send = 0;

	MSLEEP(5);

	/* Command to set directions of lower 8 pins and force value on bits set as output */
	buf[dw_bytes_send++] = (jl_uint8)0x80;
	/* Set SDA, SCL high, WP disabled by SK, DO at bit ¡®1¡¯, GPIOL0 at bit ¡®0¡¯ */
	buf[dw_bytes_send++] = (jl_uint8)0xF3;
	/* Set SK,DO,GPIOL0 pins as output with bit ¡¯1¡¯, other pins as input with bit ¡®0¡¯ */
	buf[dw_bytes_send++] = (jl_uint8)0xF3;

	/* The SK clock frequency can be worked out by below algorithm with divide by 5 set as off */
	/* SK frequency  = 60MHz /((1 +  [(1 +0xValueH*256) OR 0xValueL])*2) */
	/* Command to set clock divisor */
	buf[dw_bytes_send++] = (jl_uint8)0x86;
	/* Set 0xValueL of clock divisor */
	buf[dw_bytes_send++] = (jl_uint8)(dw_clk_div_mdio & 0xFF);
	/* Set 0xValueH of clock divisor */
	buf[dw_bytes_send++] = (jl_uint8)((dw_clk_div_mdio >> 8) & 0xFF);
	/* Send off the commands */
	status = FT_Write(ftdih, buf, dw_bytes_send, &dw_bytes_sent);
	if (status != FT_OK) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "failed to FT_Write: %s\n",
			ftd2xx_status_string(status));
		return JL_ERR_FAIL;
	}

	/* Delay for a while */
	MSLEEP(5);

	dw_bytes_send = 0;
	/* Enable 3 phase clocking */
	buf[dw_bytes_send++] = (jl_uint8)0x8C;
	/* Send off the commands */
	status = FT_Write(ftdih, buf, dw_bytes_send, &dw_bytes_sent);
	if (status != FT_OK) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "failed to FT_Write: %s\n",
			ftd2xx_status_string(status));
		return JL_ERR_FAIL;
	}
	MSLEEP(5);

	return JL_ERR_OK;
}

static jl_ret_t jl_mdio_purge_ftd2xx(void)
{
	FT_STATUS status;

	status = FT_Purge(ftdih, FT_PURGE_RX | FT_PURGE_TX);
	if (status != FT_OK) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "error purging ftd2xx device: %s\n",
			ftd2xx_status_string(status));
		return JL_ERR_FAIL;
	}

	return JL_ERR_OK;
}

jl_ret_t port_spi_init(void)
{
	jl_ret_t ret;

	JL_DBG_MSG(JL_FLAG_SMI, _DBG_INFO, "jl_mdio init\n");

	ret = jl_mdio_init_ftd2xx();
	if (ret != 0)
		return ret;

	ret = jl_mdio_config();
	if (ret != 0)
		return ret;

	ret = jl_mdio_purge_ftd2xx();
	if (ret != 0)
		return ret;

	MSLEEP(10);

	return JL_ERR_OK;
}

jl_ret_t port_spi_deinit(void)
{
	FT_STATUS status;

	JL_DBG_MSG(JL_FLAG_SMI, _DBG_INFO, "jl_mdio close\n");

	status = FT_Close(ftdih);
	if (status != FT_OK) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Failed to close ftd2xx device!");
		return JL_ERR_FAIL;
	}

	return JL_ERR_OK;
}

jl_ret_t port_spi_write(jl_uint8 *tx_buf, jl_uint8 *rx_buf, jl_uint32 size)
{
	(void) rx_buf;
	FT_STATUS status;
	DWORD i;
	jl_uint8 buf[32];
	DWORD len;
	DWORD nsent;
	DWORD nsend;

	nsend = 17 - 8;
	len = nsend+3+1;
	for (i = 0; i < len; i++) {
		buf[i] = (jl_uint8)0xff;
	}

	/* clock data byte in, MSB first */
	buf[0] = (jl_uint8)0x10;
	/* length low */
	buf[1] = 17 - 8 - 1;
	/* length high */
	buf[2] = (jl_uint8)0x00;

	memcpy(&buf[3], tx_buf, size);
	/* buf[ 4+3] = ((jl_uint8)0x0f & (phy >> 1)) | 0x50; */
	/* buf[ 5+3] = (phy << 7) | (addr << 2) | 0x02; */
	/* buf[ 6+3] = (jl_uint8)(data >> 8); */
	/* buf[ 7+3] = (jl_uint8)data; */

	buf[8+3] = (jl_uint8)0x7f;

	buf[12] = (jl_uint8)0x87;

	status = FT_Write(ftdih, buf, len, &nsent);
	if (status != FT_OK) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "unable to FT_Write: %s\n",
			ftd2xx_status_string(status));
		return JL_ERR_FAIL;
	}

	return JL_ERR_OK;
}

jl_ret_t port_spi_read(jl_uint8 *tx_buf, jl_uint8 *rx_buf, jl_uint32 size)
{
	DWORD dw_bytes_read;
	DWORD tot_bytes_read;
	FT_STATUS status;
	jl_uint8 buf[32];
	DWORD i;
	DWORD len;
	DWORD nsend;
	DWORD nsent;
	jl_int32 timeout;

	nsend = 17 - 8;
	len = nsend+3+1;
	for (i = 0; i < len; i++) {
		buf[i] = (jl_uint8)0xff;
	}


	buf[0] = (jl_uint8)0x34; //clock data byte in, MSB first
	buf[1] = 17 - 8 - 1; //Length L
	buf[2] = (jl_uint8)0x00; //Length H

	memcpy(&buf[3], tx_buf, size);
	/* buf[ 4+3] = ((jl_uint8)0x0f & (phy >> 1)) | 0X060; */
	/* buf[ 5+3] = (phy << 7) | (addr << 2) | 0x02; */

	buf[8+3] = (jl_uint8)0x7f;
	/* Send answer back immediate command */
	buf[12] = (jl_uint8)0x87;

	status = FT_Write(ftdih, buf, len, &nsent);
	if (status != FT_OK) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "unable to FT_Write: %s\n",
			ftd2xx_status_string(status));
		return JL_ERR_FAIL;
	}


	tot_bytes_read = 0;
	timeout = 5;
	while (tot_bytes_read < nsend && timeout--) {
		status = FT_Read(ftdih, &buf[tot_bytes_read], nsend - tot_bytes_read, &dw_bytes_read);
		if (status != FT_OK) {
			JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "FT_Read returned: %s\n", ftd2xx_status_string(status));
			return JL_ERR_FAIL;
		}
		tot_bytes_read += dw_bytes_read;
	}

	if (tot_bytes_read < nsend) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "couldn't read enough bytes from "
			"ftd2xx addrice (%i < %i)",
			(unsigned)tot_bytes_read,
			(unsigned)nsend);
		return JL_ERR_FAIL;
	}

	memcpy(rx_buf, buf, size);

	return JL_ERR_OK;
}
