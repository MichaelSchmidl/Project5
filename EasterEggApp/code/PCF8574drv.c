/*
 * PCF8574drv.c
 *
 *  Created on: 19.03.2021
 *      Author: mschmidl
 */

#include "PCF8574drv.h"
#include "i2c_master.h"

uint8_t PCF8574_read( void )
{
	uint8_t val = 0xCC;
	if ( 0 == I2C_readByte( PCF8574_I2C_SA, &val ) )
	{
	    PRINTF("ERR: PCF8574_read NACK\r\n");
	}
	return val;
}


void PCF8574_write( uint8_t val )
{
	if ( 0 == I2C_writeByte( PCF8574_I2C_SA, val ) )
	{
	    PRINTF("ERR: PCF8574_write NACK\r\n");
	}
}
