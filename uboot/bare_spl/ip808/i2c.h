#ifndef __I2C_H
#define __I2C_H
extern int i2c_write(int n, u8 addr, u8 *buf, u8 len);
extern int i2c_read(int n, u8 addr, u8 reg, u8 *buf, u8 len);
#endif
