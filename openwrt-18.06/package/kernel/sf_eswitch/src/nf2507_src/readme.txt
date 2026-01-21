1.根据不同的平台，用户需要修改smi.c中的管理接口的代码。
void fun_regist()
{
    m_fun.iic_read = Master_Read2Byte;
    m_fun.iic_write = Master_Write2Byte;
    m_fun.spi_read = switchchip_read;
    m_fun.spi_write = switchchip_write;
    m_fun.mdio_read = mdiobb_read;
    m_fun.mdio_write = mdiobb_write;
    m_fun.eeprom_read = eeprom_read;
    m_fun.eeprom_write = eeprom_write;
}
Master_Read2Byte等API是由对应的平台代码提供。



版本号                     修改内容
L2SW-sdk-src-esc.2.1.2     1.解决平台切换后编译报错问题
                           2.解决counter值打印问题
L2SW-sdk-src-esc.2.1.1     初始版本
