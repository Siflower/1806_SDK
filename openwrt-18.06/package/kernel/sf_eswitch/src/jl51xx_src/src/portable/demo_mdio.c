#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "jl_error.h"
#include "jl_debug.h"

#ifdef __USE_MINGW_ANSI_STDIO
#include <windows.h>
#include <winusb.h>
#include <cfgmgr32.h>

#define _unused_ __attribute__((unused))

typedef struct {
	HANDLE dev;
	WINUSB_INTERFACE_HANDLE win_itf;
} libusb_device_handle;

#else
#include <libusb.h>

#endif

#define CFG_SMI_CMD_BUFSIZE	16
#define SMI_CLAUSE22		0x22
#define SMI_SPEED_HIGH		0x00
#define SMI_SPEED_LOW		0x01
#define SMI_DATA_LEN		8

#define USBD_VID	0x0483
#define USBD_PID	0x00DE
#define EP_OUT		0x03
#define EP_IN		0x83

static jl_uint8 CMD_SMI_CFG[] = {0x30, 0x00, 0x00, 0x22, 0x00};
static jl_uint8 CMD_SMI_WRITE[] = {0x31, 0x00, 0x00};
static jl_uint8 CMD_SMI_READ[] = {0x32, 0x00, 0x00};

static libusb_device_handle *g_device;

#ifdef __USE_MINGW_ANSI_STDIO
static int libusb_get_first_device_path(GUID guid, PTCHAR path)
{
	ULONG list_size = 0;
	int ret = JL_ERR_FAIL;
	TCHAR *list = NULL;
	CONFIGRET cr = CR_SUCCESS;

	cr = CM_Get_Device_Interface_List_Size(&list_size, &guid, NULL,
							CM_GET_DEVICE_INTERFACE_LIST_PRESENT);
	if (cr != CR_SUCCESS) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "CM_Get_Device_Interface_List_Size fail\n");
		goto end;
	}

	list = (TCHAR *)calloc(list_size, sizeof(TCHAR));
	if (list == NULL) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "malloc fail\n");
		goto end;
	}

	cr = CM_Get_Device_Interface_List(&guid, NULL, list, list_size,
							CM_GET_DEVICE_INTERFACE_LIST_PRESENT);
	if (cr != CR_SUCCESS || list[0] == 0) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "CM_Get_Device_Interface_List fail\n");
		goto end;
	}

	memcpy(path, list, sizeof(TCHAR) * list_size);
	ret = JL_ERR_OK;

end:
	if (list) {
		free(list);
		list = NULL;
	}
	return ret;
}

static int libusb_init(_unused_ void *ptr)
{
	return JL_ERR_OK;
}

static int libusb_exit(_unused_ void *ptr)
{
	return JL_ERR_OK;
}

static void libusb_close(libusb_device_handle *dev_handle)
{
	CloseHandle(dev_handle->dev);
	WinUsb_Free(dev_handle->win_itf);
	free(dev_handle);
}

static libusb_device_handle *libusb_open_device_with_vid_pid(
	_unused_ void *ptr, _unused_ uint16_t vid, _unused_ uint16_t pid)
{
	libusb_device_handle *dev_handle = malloc(sizeof(libusb_device_handle));
	TCHAR path[MAX_PATH];
	GUID guid = {
		.Data1 = 0x975F44D9,
		.Data2 = 0x0D08,
		.Data3 = 0x43FD,
		.Data4 = {0x8B, 0x3E, 0x12, 0x7C, 0xA8, 0xAF, 0xFF, 0x9D}
	};
#if 0
	JL_DBG_MSG(JL_FLAG_SMI, _DBG_INFO,
		"GUID: %08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%2X%02X\n",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
#endif
	if (libusb_get_first_device_path(guid, path)) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Device not found\n");
		return NULL;
	}

	if (!dev_handle) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Failed to malloc device\n");
		return NULL;
	}

	dev_handle->dev = CreateFile(path,
								GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								NULL,
								OPEN_EXISTING,
								FILE_FLAG_OVERLAPPED,
								NULL);
	if (dev_handle->dev == INVALID_HANDLE_VALUE) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "CreateFile fail\n");
		free(dev_handle);
		return NULL;
	}

	if (!WinUsb_Initialize(dev_handle->dev, &dev_handle->win_itf)) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "WinUsb_Initialize fail\n");
		CloseHandle(dev_handle->dev);
		free(dev_handle);
		return NULL;
	}

	JL_DBG_MSG(JL_FLAG_SMI, _DBG_ON, "Device open success\n");

	return dev_handle;
}

static int libusb_bulk_transfer(libusb_device_handle *dev_handle,
	unsigned char endpoint, unsigned char *data, int length,
	int *transferred, _unused_ unsigned int timeout)
{
	int ret = 0;

	if (endpoint == EP_IN) {
		ret = WinUsb_ReadPipe(dev_handle->win_itf,
			endpoint, data, length, (PULONG)transferred, 0);
	} else {
		ret = WinUsb_WritePipe(dev_handle->win_itf,
			endpoint, data, length, (PULONG)transferred, 0);
	}
	return !ret;
}
#endif

static int __win_usb_open(void)
{
	libusb_device_handle *dev_handle;
	int ret = 0;

	ret = libusb_init(NULL);
	if (ret < 0) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "WinUSB Init Error\n");
		return JL_ERR_FAIL;
	}

	dev_handle = libusb_open_device_with_vid_pid(NULL, USBD_VID, USBD_PID);
	if (dev_handle == NULL) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "WinUSB can't open device\n");
		libusb_exit(NULL);
		return JL_ERR_FAIL;
	}

	g_device = dev_handle;
	JL_DBG_MSG(JL_FLAG_SMI, _DBG_INFO, "WinUSB Device opened, VID:0x%04x, PID:0x%04x\n", USBD_VID, USBD_PID);

	return JL_ERR_OK;
}

static int __win_usb_close(void)
{
	libusb_close(g_device);
	libusb_exit(NULL);
	return JL_ERR_OK;
}

static int __win_usb_wait_resp(uint8_t *resp, int len)
{
	int transferred;

	if (len > CFG_SMI_CMD_BUFSIZE) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "WinUSB response buffer size overflow\n");
		return JL_ERR_OUT_OF_RANGE;
	}
	if (libusb_bulk_transfer(g_device, EP_IN, resp, len, &transferred, 1000)) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "WinUSB response error\n");
		return JL_ERR_FAIL;
	}

	return JL_ERR_OK;
}

static int __win_usb_write(uint8_t *buf, int len)
{
	int transferred;

	if (len > CFG_SMI_CMD_BUFSIZE) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "WinUSB write buffer size overflow\n");
		return JL_ERR_OUT_OF_RANGE;
	}
	if (libusb_bulk_transfer(g_device, EP_OUT, buf, len, &transferred, 1000)) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "WinUSB write fail\n");
		return JL_ERR_FAIL;
	}

	JL_DBG_MSG(JL_FLAG_SMI, _DBG_ON, "WinUSB write %d bytes data success\n", transferred);
	return JL_ERR_OK;
}

static jl_ret_t __win_usb_config(void)
{
	jl_ret_t ret = 0;
	uint8_t smi_cmd[sizeof(CMD_SMI_CFG)] = {0};
	jl_uint8 smi_resp[CFG_SMI_CMD_BUFSIZE] = {0};

	memcpy(smi_cmd, CMD_SMI_CFG, sizeof(CMD_SMI_CFG));
	smi_cmd[3] = SMI_CLAUSE22;
	smi_cmd[4] = SMI_SPEED_HIGH;

	ret = __win_usb_write(smi_cmd, sizeof(smi_cmd));
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "WinUSB SMI config fail\n");
		return JL_ERR_FAIL;
	}
	ret = __win_usb_wait_resp(smi_resp, sizeof(smi_resp));
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "WinUSB smi config wait response fail\n");
		exit(EXIT_FAILURE);
	} else if (smi_resp[1]) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "WinUSB smi config response error code:%d\n", smi_resp[1]);
		exit(EXIT_FAILURE);
	}
	JL_DBG_MSG(JL_FLAG_SMI, _DBG_INFO, "WinUSB firmware version: %u.%u\n", smi_resp[3], smi_resp[4]);

	return JL_ERR_OK;
}

jl_ret_t port_mdio_init(void)
{
	if (__win_usb_open()) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "WinUSB SMI init fail\n");
		return JL_ERR_FAIL;
	}
	if (__win_usb_config()) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "WinUSB SMI config fail\n");
		return JL_ERR_FAIL;
	}

	return JL_ERR_OK;
}

jl_ret_t port_mdio_deinit(void)
{
	return __win_usb_close();
}

void port_mdio_write(jl_uint8 phy, jl_uint8 reg, jl_uint16 val)
{
	jl_ret_t ret = 0;
	jl_uint8 smi_frame[sizeof(CMD_SMI_WRITE) + SMI_DATA_LEN] = {0};
	jl_uint8 *smi_cmd = &smi_frame[0];
	jl_uint8 *smi_data = &smi_frame[sizeof(CMD_SMI_WRITE)];
	jl_uint16 op_phy_reg = 0;

	/* operation: 0x5 for smi write */
	op_phy_reg = (0x5 << 12) | ((jl_uint16)(phy & 0x1f) << 7)
				| ((jl_uint16)(reg & 0x1f) << 2) | 0x2;
	smi_data[0] = 0xff;
	smi_data[1] = 0xff;
	smi_data[2] = 0xff;
	smi_data[3] = 0xff;
	smi_data[4] = op_phy_reg >> 8;
	smi_data[5] = op_phy_reg & 0xff;
	smi_data[6] = val >> 8;
	smi_data[7] = val & 0xff;

	memcpy(smi_cmd, CMD_SMI_WRITE, sizeof(CMD_SMI_WRITE));
	/* smi data size */
	smi_cmd[1] = 8;

	ret = __win_usb_write(smi_frame, sizeof(smi_frame));
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "WinUSB smi write fail\n");
		exit(EXIT_FAILURE);
	}
}

jl_uint16 port_mdio_read(jl_uint8 phy, jl_uint8 reg)
{
	jl_ret_t ret = 0;
	jl_uint8 smi_frame[sizeof(CMD_SMI_READ) + SMI_DATA_LEN] = {0};
	jl_uint8 smi_resp[CFG_SMI_CMD_BUFSIZE] = {0};
	jl_uint8 *smi_cmd = &smi_frame[0];
	jl_uint8 *smi_data = &smi_frame[sizeof(CMD_SMI_READ)];
	jl_uint16 op_phy_reg = 0;

	/* operation: 0x6 for smi read */
	op_phy_reg = (0x6 << 12) | ((jl_uint16)(phy & 0x1f) << 7)
				| ((jl_uint16)(reg & 0x1f) << 2) | 0x2;
	smi_data[0] = 0xff;
	smi_data[1] = 0xff;
	smi_data[2] = 0xff;
	smi_data[3] = 0xff;
	smi_data[4] = op_phy_reg >> 8;
	smi_data[5] = op_phy_reg & 0xff;
	smi_data[6] = 0xff;
	smi_data[7] = 0xff;

	memcpy(smi_cmd, CMD_SMI_READ, sizeof(CMD_SMI_READ));
	/* smi data size */
	smi_cmd[1] = 8;

	ret = __win_usb_write(smi_frame, sizeof(smi_frame));
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "WinUSB smi read fail\n");
		exit(EXIT_FAILURE);
	}

	ret = __win_usb_wait_resp(smi_resp, sizeof(smi_resp));
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "WinUSB smi read wait response fail\n");
		exit(EXIT_FAILURE);
	} else if (smi_resp[1]) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "WinUSB smi read response error code:%d\n", smi_resp[1]);
		exit(EXIT_FAILURE);
	}

	return ((jl_uint16)(smi_resp[sizeof(CMD_SMI_READ) + 6] << 8
			| smi_resp[sizeof(CMD_SMI_READ) + 7]));
}

