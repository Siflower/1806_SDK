#ifndef _SF_NF_API_H_
#define _SF_NF_API_H_

#define MDC_MDIO_PHY_ID_0    29
#define MDC_MDIO_CTRL0_REG          31
#define MDC_MDIO_START_REG          29
#define MDC_MDIO_CTRL1_REG          21
#define MDC_MDIO_ADDRESS_REG        23
#define MDC_MDIO_DATA_WRITE_REG     24
#define MDC_MDIO_DATA_READ_REG      25
#define MDC_MDIO_PREAMBLE_LEN       32

#define MDC_MDIO_START_OP          0xFFFF
#define MDC_MDIO_ADDR_OP           0x000E
#define MDC_MDIO_READ_OP           0x0001
#define MDC_MDIO_WRITE_OP          0x0003


#define    L2SW_REG_DIGITAL_INTERFACE_SELECT    0x1305
#define    L2SW_REG_DIGITAL_INTERFACE_SELECT_1    0x13c3
#define    L2SW_REG_DIGITAL_INTERFACE2_FORCE    0x13c4
#define    L2SW_REG_DIGITAL_INTERFACE1_FORCE    0x1311
#define    L2SW_REG_EXT1_RGMXF    0x1307
#define    L2SW_REG_EXT2_RGMXF    0x13c5
#define    L2SW_REG_ECO_0_REG                       0x3ee0


void sf_nf_mdio_init(void);
int l2sw_getAsicReg(uint32_t reg, uint32_t *val);
int l2sw_setAsicReg(uint32_t reg, uint32_t val);
void nf_switch_init(void);
#endif
