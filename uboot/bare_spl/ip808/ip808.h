#ifndef IP808_H
#define IP808_H
//#define IP808DEBUG

#define I2C_RETRYTIMES 3

#define IP808_IOC_TYPE		0xE0
#define IP808_READ   _IOC(_IOC_READ, IP808_IOC_TYPE, 0, 6)
#define IP808_WRITE  _IOC(_IOC_WRITE, IP808_IOC_TYPE, 0, 6)


//extern void ip808_mdio_wr(unsigned short pa, unsigned short ra, unsigned short va);
//extern unsigned short ip808_mdio_rd(unsigned short pa, unsigned short ra);
extern int PoE_i2c_write(int device, int write_addr, int write_value);
extern int PoE_i2c_read(int device, int read_addr, unsigned char *value);
extern int sfax8_ip808_init(void);
#endif		/* IP1829_H */
