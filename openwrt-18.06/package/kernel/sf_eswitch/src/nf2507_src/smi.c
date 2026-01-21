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

#include "smi.h"
#if defined(L2SW_LINUX_KERNEL)
#include <linux/init.h>
#include <linux/module.h>

#elif defined(SPI_OPERATION) || defined(I2C_OPERATION) || defined(MDC_MDIO_OPERATION)
//#include "spi.h"
//#include "iic.h"
//#include "mdio.h"
#endif

extern int mdio_write_ext(int phyaddr, int phyreg, int phydata);
extern int mdio_read_ext(int phyaddr, int phyreg, int *phydata);

l2sw_uint16 value;

l2sw_int32 sf_mdio_read(l2sw_uint32 MDIOx, l2sw_uint32 phy, l2sw_uint32 reg, l2sw_uint16 *val)
{
	int phyData;
	mdio_read_ext(phy, reg, &phyData);
	*val = phyData & 0xFFFF;
	return 0;
}

l2sw_int32 sf_mdio_write(l2sw_uint32 MDIOx, l2sw_uint32 phy, l2sw_uint32 reg, l2sw_uint16 val)
{
	mdio_write_ext(phy, reg, val);
	return 0;
}

user_mgmt_fun_t m_fun = {
	.mdio_read = sf_mdio_read,
	.mdio_write = sf_mdio_write
};

void fun_init()
{
    m_fun.iic_read = NULL;
    m_fun.iic_write = NULL;
    m_fun.spi_read = NULL;
    m_fun.spi_write = NULL;
    m_fun.mdio_read = NULL;
    m_fun.mdio_write = NULL;
}

void fun_regist()
{
    m_fun.iic_read = NULL;
    m_fun.iic_write = NULL;
    m_fun.spi_read = NULL;
    m_fun.spi_write = NULL;
    m_fun.mdio_read = sf_mdio_read;
    m_fun.mdio_write = sf_mdio_write;
}

static void rtlglue_drvMutexLock(l2sw_uint8 units)
{
    /* It is empty currently. Implement this function if Lock/Unlock function is needed */
    return;
}

static void rtlglue_drvMutexUnlock(l2sw_uint8 units)
{
    /* It is empty currently. Implement this function if Lock/Unlock function is needed */
    return;
}


#ifdef L2SW_LINUX_KERNEL
l2sw_int32 l2sw_smi_read(l2sw_uint8 units, l2sw_uint32 mAddrs, l2sw_uint32 *rData)
{
    /*please add driver here when use linux*/
	l2sw_int32 ret = L2SW_ERR_OK;

	return ret;
}

l2sw_int32 l2sw_smi_write(l2sw_uint8 units, l2sw_uint32 mAddrs, l2sw_uint32 rData)
{
    /*please add driver here when use linux*/
	l2sw_int32 ret = L2SW_ERR_OK;

	return ret;
}


#else
l2sw_int32 l2sw_smi_read(l2sw_uint8 units, l2sw_uint32 mAddrs, l2sw_uint32 *rData)
{
    //l2sw_int32 ret;
#ifdef SPI_OPERATION
	l2sw_uint8 pBuffer[2] = {0};
#endif
    if(mAddrs > 0xFFFF)
    {
        return L2SW_ERR_INPUT;
    }

    if(rData == NULL)
    {
        return L2SW_ERR_NULL_POINTER;
    }

    *rData = 0;

#if defined(MDC_MDIO_OPERATION)


	if(m_fun.mdio_read == NULL)
    {
        printk("MDIO_READ_FUN == NULL\n");
        return -1;
    }

    /* Lock */
    rtlglue_drvMutexLock(units);

    //chip A
    if(0 == units)
    {
        /* Write address control code to register 31 */
        m_fun.mdio_write(MDIO0_man, MDC_MDIO_PHY_ID_29, MDC_MDIO_CTRL0_REG, MDC_MDIO_ADDR_OP);

        /* Write address to register 23 */
        m_fun.mdio_write(MDIO0_man, MDC_MDIO_PHY_ID_29, MDC_MDIO_ADDRESS_REG, mAddrs);

        /* Write read control code to register 21 */
        m_fun.mdio_write(MDIO0_man, MDC_MDIO_PHY_ID_29, MDC_MDIO_CTRL1_REG, MDC_MDIO_READ_OP);

        /* Read data from register 25 */
        m_fun.mdio_read(MDIO0_man, MDC_MDIO_PHY_ID_29, MDC_MDIO_DATA_READ_REG, &value);
    }

    //chip B
    else if(1 == units)
    {
        /* Write address control code to register 31 */
        m_fun.mdio_write(MDIO1_man, MDC_MDIO_PHY_ID_29, MDC_MDIO_CTRL0_REG, MDC_MDIO_ADDR_OP);

        /* Write address to register 23 */
        m_fun.mdio_write(MDIO1_man, MDC_MDIO_PHY_ID_29, MDC_MDIO_ADDRESS_REG, mAddrs);

        /* Write read control code to register 21 */
        m_fun.mdio_write(MDIO1_man, MDC_MDIO_PHY_ID_29, MDC_MDIO_CTRL1_REG, MDC_MDIO_READ_OP);

        /* Read data from register 25 */
        m_fun.mdio_read(MDIO1_man, MDC_MDIO_PHY_ID_29, MDC_MDIO_DATA_READ_REG, &value);
    }
    else
    {
        printk("unit %d not support now when use I2C\n\r", units);
    }

    *rData = value;

    /* Unlock */
    rtlglue_drvMutexUnlock(units);

    return L2SW_ERR_OK;

#elif defined(SPI_OPERATION)

    if(m_fun.spi_read == NULL)
    {
        printk("SPI_READ_FUN == NULL\n");
        return -1;
    }

    /* Lock */
    rtlglue_drvMutexLock(units);

    //chip A
    if(0 == units)
    {
        m_fun.spi_read(SPI2_man, mAddrs, pBuffer, 2);
    }

    //chip B
    else if(1 == units)
    {
        m_fun.spi_read(SPI3_man, mAddrs, pBuffer, 2);
    }
    else
    {
        printk("unit %d not support now when use I2C\n\r", units);
    }
    *rData = ((pBuffer[0] << 8) | pBuffer[1]);

    /* Unlock */
    rtlglue_drvMutexUnlock(units);

    return L2SW_ERR_OK;

#elif defined(I2C_OPERATION)
    if(m_fun.iic_read == NULL)
    {
        printk("IIC_READ_FUN == NULL\n");
        return -1;
    }

    /* Lock */
    rtlglue_drvMutexLock(units);
    /* Read 16 bits data */

    //chip A
    if(0 == units)
    {
        m_fun.iic_read(I2C0_man, I2C_SLAVE_ADDR, (l2sw_uint16)mAddrs, (l2sw_uint16 *)rData);
        //printk("register read addr 0x%x data:0x%x\n", mAddrs, *rData);
    }
    //chip B
    else if(1 == units)
    {
        m_fun.iic_read(I2C2_man, I2C_SLAVE_ADDR, (l2sw_uint16)mAddrs, (l2sw_uint16 *)rData);
    }
    else
    {
        printk("unit %d not support now when use I2C\n\r", units);
    }

    /* Unlock */
    rtlglue_drvMutexUnlock(units);
	return L2SW_ERR_OK;

#else
    printk("There is no interface to access register.\n");

    //return ret;
    return 0;
#endif /* end of #if defined(MDC_MDIO_OPERATION) */
}



l2sw_int32 l2sw_smi_write(l2sw_uint8 units, l2sw_uint32 mAddrs, l2sw_uint32 rData)
{
#ifdef SPI_OPERATION
	l2sw_uint8 pBuffer[2] = {0};
#endif
    //l2sw_int32 ret;

    if(mAddrs > 0xFFFF)
        return L2SW_ERR_INPUT;

    if(rData > 0xFFFF)
        return L2SW_ERR_INPUT;

#if defined(MDC_MDIO_OPERATION)
    if(m_fun.mdio_write == NULL)
    {
        printk("MDIO_WRITE_FUN == NULL\n");
        return -1;
    }

    /* Lock */
    rtlglue_drvMutexLock(units);

    //chip A
    if(0 == units)
    {
        m_fun.mdio_write(MDIO0_man, MDC_MDIO_PHY_ID_29, MDC_MDIO_CTRL0_REG, MDC_MDIO_ADDR_OP);

        /* Write address to register 23 */
        m_fun.mdio_write(MDIO0_man, MDC_MDIO_PHY_ID_29, MDC_MDIO_ADDRESS_REG, mAddrs);

        /* Write read control code to register 24 */
        m_fun.mdio_write(MDIO0_man, MDC_MDIO_PHY_ID_29, MDC_MDIO_DATA_WRITE_REG, rData);

        /* Read data from register 21 */
        m_fun.mdio_write(MDIO0_man, MDC_MDIO_PHY_ID_29, MDC_MDIO_CTRL1_REG, MDC_MDIO_WRITE_OP);
    }

    //chip B
    else if(1 == units)
    {
        m_fun.mdio_write(MDIO1_man, MDC_MDIO_PHY_ID_29, MDC_MDIO_CTRL0_REG, MDC_MDIO_ADDR_OP);

        /* Write address to register 23 */
        m_fun.mdio_write(MDIO1_man, MDC_MDIO_PHY_ID_29, MDC_MDIO_ADDRESS_REG, mAddrs);

        /* Write read control code to register 24 */
        m_fun.mdio_write(MDIO1_man, MDC_MDIO_PHY_ID_29, MDC_MDIO_DATA_WRITE_REG, rData);

        /* Read data from register 21 */
        m_fun.mdio_write(MDIO1_man, MDC_MDIO_PHY_ID_29, MDC_MDIO_CTRL1_REG, MDC_MDIO_WRITE_OP);
    }
    else
    {
        printk("unit %d not support now when use MDIO\n\r", units);
    }

    /* Unlock */
    rtlglue_drvMutexUnlock(units);

    return L2SW_ERR_OK;

#elif defined(SPI_OPERATION)
    if(m_fun.spi_write == NULL)
    {
        printk("SPI_WRITE_FUN == NULL\n");
        return -1;
    }

    /* Lock */
    rtlglue_drvMutexLock(units);

    pBuffer[0] = (rData >> 8) & 0x000000ff;
	pBuffer[1] = rData & 0x000000ff;

	//chip A
    if(0 == units)
    {
        m_fun.spi_write(SPI2_man, mAddrs, pBuffer, 2);
    }
    //chip B
    else if(1 == units)
    {
        m_fun.spi_write(SPI3_man, mAddrs, pBuffer, 2);
    }
    else
    {
        printk("unit %d not support now when use I2C\n\r", units);
    }

    /* Unlock */
    rtlglue_drvMutexUnlock(units);

    return L2SW_ERR_OK;

#elif defined(I2C_OPERATION)
    if(m_fun.iic_write == NULL)
    {
        printk("IIC_WRITE_FUN == NULL\n");
        return -1;
    }

	/* Lock */
	rtlglue_drvMutexLock(units);

	/* Write 16 bits data */
	//chip A
    if(0 == units)
    {
        m_fun.iic_write(I2C0_man, I2C_SLAVE_ADDR, (l2sw_uint16)mAddrs, (l2sw_uint16)rData);
        //printk("register write addr 0x%x vlaue 0x%x\n", mAddrs, rData);
    }

    //chip B
    else if(1 == units)
    {
        m_fun.iic_write(I2C2_man, I2C_SLAVE_ADDR, (l2sw_uint16)mAddrs, (l2sw_uint16)rData);
    }
	else
    {
        printk("unit %d not support now when use I2C\n\r", units);
    }

	/* Unlock */
	rtlglue_drvMutexUnlock(units);

	return L2SW_ERR_OK;

#else
    printk("There is no interface to access register.\n");

    //return ret;
    return 0;
#endif /* end of #if defined(MDC_MDIO_OPERATION) */
}
#endif
