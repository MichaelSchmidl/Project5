/*!
 * \file   i2c_master.c
 * \brief  I2C Master using bit-banging
 * \author MSchmidl
 * \copyright 2012 Ultratronik GmbH
 *
 * Bit-banging I2C master functions. Because the only I2C slave device
 * is the RTC, it is very simple. There is no multi-master support
 * and clock stretching necessary because the RTC does not support
 * this.
 * Only at the I2C_init() there is a check for SDA stuck at ZERO which
 * may happen if the microprocessor restarts right in the middle of an
 * I2C read transfer. In case the RTC is about to send a LOW on the SDA
 * it will keep this LOW even after restart. So the I2C_init() function
 * sends a couple of SCL clocks in case the SDA is not free at startup.
 */
#include "app.h"
#include "i2c_master.h"

#define tSETUP_US 1
#define tHOLD_US  1
#define tLOW_US   2
#define tHIGH_US  7

// macros for I2C bitbanging
#define SCL_0       DIO->DATA &= ~( 1 << SCL_DIO_NUM ); DIO->DIR |= ( 1 << SCL_DIO_NUM )
#define SCL_H       DIO->DIR &= ~( 1 << SCL_DIO_NUM )
#define Is_SCL_high DIO_DATA->ALIAS[SCL_DIO_NUM] == 1
#define SDA_0       DIO->DATA &= ~( 1 << SDA_DIO_NUM ); DIO->DIR |= ( 1 << SDA_DIO_NUM )
#define SDA_H       DIO->DIR &= ~( 1 << SDA_DIO_NUM )
#define Is_SDA_high DIO_DATA->ALIAS[SDA_DIO_NUM] == 1

/*!
 * \brief generate a I2C START condition on the bus
 */
static void _I2C_start(void)
{
   SDA_H;
   SCL_H;
   DELAY_US(tHIGH_US);
   SDA_0;
   DELAY_US(tSETUP_US);
   SCL_0;
}


/*!
 * \brief generate a I2C STOP condition on the bus
 */
static void _I2C_stop(void)
{
   SDA_0;
   DELAY_US(tSETUP_US);
   SCL_H;
   DELAY_US(tHIGH_US);
   SDA_H;
   DELAY_US(tHOLD_US);
}


/*!
 * \brief generic transmission of a byte over I2C
 *
 * \param[in] val byte to transmit over I2C
 * \returns 1=ACK 0=NACK
 */
static uint8_t _I2C_txd(uint8_t val)
{
   uint8_t ack = 0;
   uint8_t mask = (1 << 7);
   while(mask) {
	   DELAY_US(tHOLD_US);
      if (val & mask) {
         SDA_H;
      } else {
         SDA_0;
      }
      DELAY_US(tLOW_US);
      SCL_H;
      DELAY_US(tHIGH_US);
      SCL_0;
      mask >>= 1;
   }
   DELAY_US(tLOW_US);
   SDA_H;
   DELAY_US(tSETUP_US);
   SCL_H;
   DELAY_US(tHIGH_US);
   if (Is_SDA_high) ack = 0;
   else ack = 1;
   SCL_0;
   DELAY_US(tLOW_US);
   return ack;
}


/*!
 * \brief generic reception of a byte over I2C
 *
 * \param[in] bWithAck if 1 an ACK is send at the end of the byte, else a NACK
 * \returns byte received from slave
 */
static uint8_t _I2C_rxd(uint8_t bWithAck)
{
   uint8_t ret = 0;
   uint8_t n;
   for (n = 0; n < 8; n++) {
      ret <<= 1;
      SCL_H;
      DELAY_US(tHIGH_US);
      if (Is_SDA_high) ret |= 1;
      SCL_0;
      DELAY_US(tLOW_US);
   }
   if (bWithAck) {
      SDA_0;
   } else {
      SDA_H;
   }
   DELAY_US(tSETUP_US);
   SCL_H;
   DELAY_US(tHIGH_US);
   SCL_0;
   DELAY_US(tSETUP_US);
   SDA_H;
   DELAY_US(tLOW_US);
   return ret;
}


///////////////////////////////////////////////////////////////////////
/*! 
 * \brief initialize the I2C bus by generating a STOP condition
 *
 * At first generate a STOP condition pattern on the bus.
 * After that check whether the SDA is high. If not, a couple
 * of SCL clocks are generated until the SDA goes high to release the
 * bus in case the RTC is in a stuck state because it was in the middle
 * of a read transfer.
 */
void I2C_init(void)
{
   SCL_H;
   SDA_H;
   if (!Is_SCL_high) { // SCL must be released now - otherwise it's an error condition
      PRINTF("ERR: SCL not high\r\n");
   }
   if (!Is_SDA_high) { // SDA should be released now - otherwise try to recover
      PRINTF("ERR: SDA not high\r\n");
   }
}


static uint8_t _I2C_setaddr(uint8_t slave_addr, uint8_t addr, uint8_t bWithStop)
{
   uint8_t ack = 0;
   _I2C_start();
   ack = _I2C_txd(slave_addr);
   if (ack) ack = _I2C_txd(addr);
   if (bWithStop) _I2C_stop();
   return ack;
}


uint8_t I2C_readReg8(uint8_t slave_addr, uint8_t reg, uint8_t *pBuffer)
{
   uint8_t ack = 0;
   ack = _I2C_setaddr(slave_addr, reg, 0);
   SCL_H; // prepare for repeated START
   _I2C_start();
   ack = _I2C_txd(slave_addr+1);
   if (ack) {
      *pBuffer = _I2C_rxd(0);
   }
   _I2C_stop();
   return ack;
}


uint8_t I2C_readBuf8(uint8_t slave_addr, uint8_t reg, uint8_t *pBuffer, uint16_t nBytes)
{
   uint8_t ack = 0;
   ack = _I2C_setaddr(slave_addr, reg, 0);
   _I2C_start();
   ack = _I2C_txd(slave_addr+1);
   if (ack) {
	   while ( nBytes--)
	   {
	      *pBuffer++ = _I2C_rxd( (nBytes==0)?0:1 );
	   }
   }
   _I2C_stop();
   return ack;
}


uint8_t I2C_writeReg8(uint8_t slave_addr, uint8_t reg, uint8_t val)
{
   uint8_t ack = 0;
   ack = _I2C_setaddr(slave_addr, reg, 0);
   if (ack) ack = _I2C_txd(val);
   _I2C_stop();
   return ack;
}


uint8_t I2C_readByte(uint8_t slave_addr, uint8_t *pBuffer)
{
   uint8_t ack = 0;
   _I2C_start();
   ack = _I2C_txd(slave_addr+1);
   if (ack) {
      *pBuffer = _I2C_rxd(0);
   }
   _I2C_stop();
   return ack;
}


uint8_t I2C_writeByte(uint8_t slave_addr, uint8_t val)
{
   uint8_t ack = 0;
   _I2C_start();
   ack = _I2C_txd(slave_addr);
   if (ack) ack = _I2C_txd(val);
   _I2C_stop();
   return ack;
}


