/*
 * GYROdrv.c
 *
 *  Created on: 19.03.2021
 *      Author: mschmidl
 */

#include "GYROdrv.h"
#include "i2c_master.h"

#define GYRO_ID_REG 0x0D
#define GYRO_ID_VAL 0x4A //!< expected value for MMA8652

#define GYRO_X_REG 1
#define GYRO_Y_REG 3
#define GYRO_Z_REG 5

static void _writeReg( uint8_t reg, uint8_t val )
{
	if ( 0 == I2C_writeReg8( GYRO_I2C_SA,
			                reg,
							val ) )
	{
	    PRINTF("ERR: GYRO_write NACK\r\n");
	}
}


static uint8_t _readReg( uint8_t reg )
{
	uint8_t val = 0xCC;

	if ( 0 == I2C_readReg8( GYRO_I2C_SA,
			                reg,
							&val ) )
	{
	    PRINTF("ERR: GYRO_read NACK\r\n");
	}
	return val;
}


static int16_t _read12bit( uint8_t startReg )
{
	int16_t val = 0;
	uint8_t buf[2];
    I2C_readBuf8( GYRO_I2C_SA,
    		      startReg,
				  buf,
				  sizeof(buf) );
    uint8_t *pVal = (uint8_t*)&val;
    // revese indeaness
    *pVal++ = buf[1];
    *pVal++ = buf[0];
    // remove lower 4 bits
    val /= 16;
    return val;
}


int16_t GYRO_readX( void )
{
	int16_t val = 0;
    val = _read12bit( GYRO_X_REG );
    return val;
}


int16_t GYRO_readY( void )
{
	int16_t val = 0;
    val = _read12bit( GYRO_Y_REG );
    return val;
}


int16_t GYRO_readZ( void )
{
	int16_t val = 0;
    val = _read12bit( GYRO_Z_REG );
    return val;
}


uint8_t GYRO_init( void )
{
    if ( GYRO_ID_VAL != _readReg( GYRO_ID_REG ) )
    {
	    PRINTF("ERR: incorrect GYRO chip\n");
	    return 0;
    }
    _writeReg( 0x2B, 0x40); // soft reset
//    _writeReg( 0x2A, 1 ); // set ACTIVE

	while (1)
	{
		int16_t x = GYRO_readX();
		int16_t y = GYRO_readY();
		int16_t z = GYRO_readZ();
		PRINTF("X=%d Y=%d Z=%d\r\n", x, y, z);
		DELAY_MS(250);
	}

	return 1;
}
