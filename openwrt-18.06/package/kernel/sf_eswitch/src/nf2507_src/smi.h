/**  @file
  *  @brief    brief  description
  *  @author   
  *  @date     2020.5.7
  *  @version  1.0
  *  @note
  *  detailed  description
  *  $LastChangedDate$
  *  $LastChangedRevision$
  *  $LastChangedBy$
  */

#ifndef __SMI_H__
#define __SMI_H__

#include "l2sw_types.h"
#include "l2sw_error.h"

#define MDIO0_man                   0
#define MDIO1_man                   1
#define SPI2_man                    2
#define SPI3_man                    3
#define I2C0_man                    0
#define I2C2_man                    2

#define MDC_MDIO_PHY_ID_0    0  /* PHY ID 0 or 29 */
#define MDC_MDIO_PHY_ID_29    29  /* PHY ID 0 or 29 */

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

#define SPI_READ_OP                 0x3
#define SPI_WRITE_OP                0x2
#define SPI_READ_OP_LEN             0x8
#define SPI_WRITE_OP_LEN            0x8
#define SPI_REG_LEN                 16
#define SPI_DATA_LEN                16

#define I2C_SLAVE_ADDR              0x5c /*注意：此地址值没考虑最低的读写位*/

typedef struct user_mgmt_fun_s
{
    l2sw_int32 (*iic_read)(l2sw_uint32 I2Cx, l2sw_uint8 SlaveAddr, l2sw_uint16 ReadAddr, l2sw_uint16 *ReadBuff);
    l2sw_int32 (*iic_write)(l2sw_uint32 I2Cx, l2sw_uint8 SlaveAddr, l2sw_uint16 WriteAddr, l2sw_uint16 DataToWrite);
    l2sw_int32 (*spi_read)(l2sw_uint32 SPIx, l2sw_uint16 ReadAddr, l2sw_uint8 *pBuffer, l2sw_uint16 NumByteToRead);
    l2sw_int32 (*spi_write)(l2sw_uint32 SPIx, l2sw_uint16 WriteAddr, l2sw_uint8 *pBuffer, l2sw_uint16 NumByteToWrite);
    l2sw_int32 (*mdio_read)(l2sw_uint32 MDIOx, l2sw_uint32 phy, l2sw_uint32 reg, l2sw_uint16 *val);
    l2sw_int32 (*mdio_write)(l2sw_uint32 MDIOx, l2sw_uint32 phy, l2sw_uint32 reg, l2sw_uint16 val);
}user_mgmt_fun_t;

void fun_init(void);
void fun_regist(void);

l2sw_int32 l2sw_smi_read(l2sw_uint8 units, l2sw_uint32 mAddrs, l2sw_uint32 *rData);
l2sw_int32 l2sw_smi_write(l2sw_uint8 units, l2sw_uint32 mAddrs, l2sw_uint32 rData);

#endif /* __SMI_H__ */


