#ifndef I2C_H
#define I2C_H

#define NUM 0
 
void I2C0GPIOBEnable(void);
unsigned char SINGLE_read(unsigned char device_addr,char data);
void i2c_read(int num, unsigned char device_addr, char *data);
void i2c_write(int num, unsigned char device_addr, char *data);

#endif