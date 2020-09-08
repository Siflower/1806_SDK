/*
 * =====================================================================================
 *
 *       Filename:  sfax8_factory_read.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2018年11月05日 20时22分03秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  star (), star.jiang@siflower.com.cn
 *        Company:  Siflower
 *
 * =====================================================================================
 */
#ifndef _SFAX8_FACTORY_READ_H_
#define _SFAX8_FACTORY_READ_H_

//size unit: BYTE
#define MACADDR_SIZE            6
#define SN_SIZE                 16
#define SN_FLAG_SIZE            1
#define PCBA_BOOT_SIZE          4
#define HARDWARE_VER_FLAG_SIZE  2
#define HARDWARE_VER_SIZE       32
#define MODEL_VER_FLAG_SIZE     2
#define MODEL_VER_SIZE          32
#define COUNTRYID_SIZE          2
#define HW_FEATURE_SIZE         4
#define VENDER_FLAG_SIZE        2
#define VENDER_SIZE             16
#define PRODUCT_KEY_FLAG_SIZE   2
#define PRODUCT_KEY_SIZE        32
#define LOGIN_INFO_FLAG_SIZE    2
#define LOGIN_INFO_SIZE         4
#define ROM_TYPE_FLAG_SIZE      2
#define ROM_TYPE_SIZE           4
#define WIFI_VERSION_SIZE       2
#define WIFI_INFO_SIZE          2046
#define XO_CONFIG_SIZE          2
/**
* @name HW feature definitions
* <pre>
*   Bits	Name		Value
*  --------------------------------
*  31:02				Reserved
*  --------------------------------
*  02	 HOTSINK_EXIST	1->hw has hotsink
*						0->no hotsink
*  --------------------------------
*  01	 32K_BY_GPIO	1->hw connect GPIO to 32k low power pin
*						0->no this connection
*  --------------------------------
*  00	 32K_BY_PMU		1->hw could provide 32K low power clock by PMU or by external CLK
*						0->can not provide
* </pre>
*/
// field definitions
//32K_BY_PMU field mask
#define HW_FEATURE_32K_BY_PMU_MASK		((uint32_t)0x00000001)
//32K_BY_PMU LSB position
#define HW_FEATURE_32K_BY_PMU_LSB		0
//32K_BY_GPIO field mask
#define HW_FEATURE_32K_BY_GPIO_MASK		((uint32_t)0x00000002)
//32K_BY_GPIO LSB position
#define HW_FEATURE_32K_BY_GPIO_LSB		1
//HOTSINK_EXISTfield mask
#define HW_FEATURE_HOTSINK_EXIST_MASK   ((uint32_t)0x00000004)
//HOTSINK_EXIST LSB position
#define HW_FEATURE_HOTSINK_EXIST_LSB	2

/*@name login info definitions
 *
 * Bits        Name            Value
 * 0           telnetd         0->enable telnetd, 1->disable it
 * 1           ssh             0->disable ssh, 1->enable it (haven't accomplished now)
 * 2           serial port     0->disable serial port, 1->disable it (haven't accomplished now))
 *
 * */

enum sfax8_factory_read_action {
	//for eth basic address
	READ_MAC_ADDRESS,
	//for wifi lb basic address
	READ_WIFI_LB_MAC_ADDRESS,
	//for wifi hb bsic address
	READ_WIFI_HB_MAC_ADDRESS,
	//for wan basic address
	READ_WAN_MAC_ADDRESS,
	READ_SN,
	READ_SN_FLAG,
	READ_PCBA_BOOT,
	READ_HARDWARE_VER_FLAG,
	READ_HARDWARE_VER,
	READ_MODEL_VER_FLAG,
	READ_MODEL_VER,
	READ_COUNTRY_ID,
	READ_HW_FEATURE,
	READ_VENDER_FLAG,
	READ_VENDER,
	READ_PRODUCT_KEY_FLAG,
	READ_PRODUCT_KEY,
	READ_LOGIN_INFO_FLAG,
	READ_LOGIN_INFO,
	READ_ROM_TYPE_FLAG,
	READ_ROM_TYPE,
	READ_WIFI_VERSION,
	READ_RF_XO_CONFIG,
	READ_WIFI_INFO,
};

int sf_get_value_from_factory(enum sfax8_factory_read_action action, void *buffer, int len);
#endif
