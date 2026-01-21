/*******************************************************************************
*                                                                              *
*  Copyright (c), 2024, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef YT_BOARD_H
#define YT_BOARD_H

typedef enum yt_board_profile_id_e
{
    BOARD_ID_YT9215	= 92150,
    BOARD_ID_YT9215RB	= 92153,
    BOARD_ID_YT9215S	= 92152,
    BOARD_ID_YT9215SC	= 92151,
    BOARD_ID_YT9215RB_YT8531 = 92154,
    BOARD_ID_YT9215S_YT8531 = 92155, 
    BOARD_ID_YT9218N	= 92181,
    BOARD_ID_YT9218MB	= 92180,
    BOARD_ID_YT9218MB_YT8531 = 92182,
    BOARD_ID_YT9214NB	= 92140,
    BOARD_ID_YT9213NB	= 92130,
    BOARD_ID_YT921X     = 92100,
    BOARD_ID_YT9230	= 92300,
    BOARD_ID_YT9231	= 92310,
    BOARD_ID_YT9231_6 = 92311,
    BOARD_ID_YT9232_24G_4GC	= 92320,
    BOARD_ID_YT9232_24G_2FIB = 92321,
    BOARD_ID_YT9232_24G_4FIB = 92322,
    BOARD_ID_YT9232_24G_4GE = 92323,
    BOARD_ID_YT9232_24G_2GE_2FIB = 92324,
    BOARD_ID_YT9232_24G = 92325,
    BOARD_ID_YT9232_16G_2GC = 92326,
    BOARD_ID_YT9232_16G_2FIB = 92327,
    BOARD_ID_YT9232_16G = 92328,
    BOARD_ID_YT9228	= 92200,
    BOARD_ID_YT9224 = 92201,
}yt_board_profile_id_t;

typedef enum yt_port_attri_e
{
    PORT_ATTR_NONE		= 0,
    PORT_ATTR_PHY,
    PORT_ATTR_SERDES,
    PORT_ATTR_XMII,
    PORT_ATTR_INT_CPU,			/* Internal CPU port */
}yt_port_attri_t;

/* switch core register access method (from main CPU) */
typedef enum yt_swAccMethod_e
{
    SWCHIP_ACC_NONE = 0,            /* not able to access register */
    SWCHIP_ACC_SPI,                     /* the switch core registers are accessed through SPI interface by main CPU */
    SWCHIP_ACC_I2C,                     /* the switch core registers are accessed through I2C interface by main CPU */
    SWCHIP_ACC_SMI,                     /* the switch core registers are accessed through SMI interface by main CPU */
}yt_swAccMethod_t;

typedef enum yt_i2c_mode_e
{
    YT_I2C_STD_MSB = 1,
    YT_I2C_STD_LSB,
    YT_I2C_SIMPLE,
}yt_i2c_mode_t;

/* Control Interface (CRTLIF)--start */
/* SMI */
typedef uint32_t (*smi_cl22_write)(uint8_t phyAddr, uint8_t regAddr, uint16_t regValue);
typedef uint32_t (*smi_cl22_read)(uint8_t phyAddr, uint8_t regAddr, uint16_t *pRegValue);
/* SPI */
typedef int32_t (*spi_cb_write)(uint8_t *regAddr, uint32_t addrLen, uint8_t *regValue, uint32_t valueLen);
typedef int32_t (*spi_cb_read)(uint8_t *regAddr, uint32_t addrLen, uint8_t *regValue, uint32_t valueLen);
/* I2C */
typedef int32_t (*i2c_cb_write)(uint8_t *regAddr, uint32_t addrLen, uint8_t *regValue, uint32_t valueLen);
typedef int32_t (*i2c_cb_read)(uint8_t *regAddr, uint32_t addrLen, uint8_t *regValue, uint32_t valueLen);

typedef struct yt_smi_desc_s
{
    uint8_t switchId;
    uint8_t phyAddr;
    smi_cl22_write smi_write;
    smi_cl22_read smi_read;
}yt_smi_desc_t;

typedef struct yt_spi_desc_s
{
    spi_cb_write spi_write;
    spi_cb_read spi_read;
}yt_spi_desc_t;

typedef struct yt_i2c_desc_s
{
    yt_i2c_mode_t i2c_mode;
    i2c_cb_write i2c_write;
    i2c_cb_read i2c_read;
}yt_i2c_desc_t;

typedef struct yt_switch_access_s
{
    yt_swAccMethod_t swreg_acc_method;
    union
    {
        yt_smi_desc_t smi_controller;
        yt_spi_desc_t spi_controller;
        yt_i2c_desc_t i2c_controller;
    }controller;
}yt_switch_access_t;

typedef enum yt_switch_chip_model_e
{
	YT_SW_MODEL_9215 = 0,
	YT_SW_MODEL_9218,
	YT_SW_MODEL_9218N,
	YT_SW_MODEL_9213,
	YT_SW_MODEL_9214,
	YT_SW_MODEL_9230, /*8+2*/
	YT_SW_MODEL_9231, /*8+6*/
	YT_SW_MODEL_9231_6, /*only 6 serdes*/
	YT_SW_MODEL_9232, /* 24+4GC/24+4GE/24+4FX/24+2GE+2FX/24G/16G+2GC/16G */
	YT_SW_MODEL_9232_26, /*24+2FX/16+2FX*/
	YT_SW_MODEL_9228, /*8+1*/
	YT_SW_MODEL_9224, /*4+2*/
	YT_SW_MODEL_END
}yt_switch_chip_model_t;

typedef enum yt_switch_chip_id_e
{
    YT_SW_ID_9215	= 0x9002,
    YT_SW_ID_9218	= 0x9001,
    YT_SW_ID_923X	= 0x9003,
    YT_SW_ID_922X	= 0x9004,
}yt_switch_chip_id_t;

/* port description */
typedef struct yt_portDescp_s
{
    uint8_t   mac_id;               /* Physical MAC ID */
    yt_port_attri_t attribute;    /* only consider switch internal port attribute */
    uint16_t phy_addr;           /* phy address, bit0~7 int phy address, bit8~15 ext phy address*/
    yt_extif_mode_t extifMode;
    yt_port_speed_duplex_t speedDuplex; /* speed and duplex */
    yt_port_medium_t medium;              /* port media type, yt_port_medium_t*/
    uint8_t phyType;          /* phy type, yt_phy_type_t */
}yt_portDescp_t;

typedef struct yt_switch_desc_cfg_s
{
    yt_switch_chip_id_t swChipId;
    yt_switch_chip_model_t swChipModel;
    yt_switch_access_t swAccess;
    uint8_t portNum;
    yt_portDescp_t portDesc[YT_MAX_PORT_PER_UNIT];
}yt_switch_desc_cfg_t;

typedef struct yt_board_desc_cfg_s
{
    uint8_t valid;
    uint32_t boardId;
    uint8_t	switchCnt; /* number of switch chip on board */ 
    yt_switch_desc_cfg_t swDesc[YT_MAX_UNIT];
}yt_board_desc_cfg_t;

uint32_t yt_board_profile_id_set(uint32_t boardId);
void yt_board_profile_id_get(uint32_t *pBoardId);
yt_ret_t yt_board_ext_phy_addr_set(yt_unit_t unit, yt_port_t port, uint8_t phyAddr);
yt_ret_t yt_board_ext_phy_addr_get(yt_unit_t unit, yt_port_t port, uint8_t *pPhyAddr);
void yt_board_profile_clear(void);
yt_ret_t yt_board_profile_set(yt_board_desc_cfg_t *pBoardDesc);
yt_ret_t yt_board_port_num_get(yt_unit_t unit, uint32_t *pMaxPorts);
#endif