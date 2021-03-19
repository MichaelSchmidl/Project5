#ifndef __I2C_MASTER_H__
#define __I2C_MASTER_H__

void    I2C_init(void);

uint8_t I2C_readReg8(uint8_t slave_addr, uint8_t reg, uint8_t *pBuffer);
uint8_t I2C_writeReg8(uint8_t slave_addr, uint8_t reg, uint8_t val);

uint8_t I2C_readByte(uint8_t slave_addr, uint8_t *pBuffer);
uint8_t I2C_writeByte(uint8_t slave_addr, uint8_t val);

uint8_t I2C_readBuf8(uint8_t slave_addr, uint8_t reg, uint8_t *pBuffer, uint16_t nBytes);

#endif // __I2C_MASTER_H__
