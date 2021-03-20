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
    // check lower 4bits - must be ZERO
    if ( 0 != ( val & 0x000F ) )
    {
	    PRINTF("ERR: possible wrong value read\r\n");
    }
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


int8_t GYRO_getPL( void )
{
	int8_t val = 0;
    val = _readReg( 0x10 );
    return val;
}


GRYRO_Orientation_t GYRO_getOrientation( void )
{
#define GYRO_ORIENTATION_THRESHOLD 500
	int16_t x = GYRO_readX();
	int16_t y = GYRO_readY();
	int16_t z = GYRO_readZ();
	if ( ( abs(x) < GYRO_ORIENTATION_THRESHOLD ) &&
	     ( abs(y) < GYRO_ORIENTATION_THRESHOLD ) &&
	     (     z  < -GYRO_ORIENTATION_THRESHOLD ) )
		return BACK; // X=0g, Y=0g, Z=-1g
	if ( ( abs(x) < GYRO_ORIENTATION_THRESHOLD ) &&
	     ( abs(y) < GYRO_ORIENTATION_THRESHOLD ) &&
	     (     z  > GYRO_ORIENTATION_THRESHOLD ) )
		return FRONT; // X=0g, Y=0g, Z=1g
	if ( ( abs(x) < GYRO_ORIENTATION_THRESHOLD ) &&
	     (     y  <  -GYRO_ORIENTATION_THRESHOLD ) &&
	     ( abs(z) < GYRO_ORIENTATION_THRESHOLD ) )
		return PORTRAIT_UP; // X=0g, Y=-1g, Z=0g
	if ( (     x  > GYRO_ORIENTATION_THRESHOLD ) &&
	     ( abs(y) < GYRO_ORIENTATION_THRESHOLD ) &&
	     ( abs(z) < GYRO_ORIENTATION_THRESHOLD ) )
		return LANDSCAPE_RIGHT; // X=1g, Y=0g, Z=0g
	if ( ( abs(x) < GYRO_ORIENTATION_THRESHOLD ) &&
	     (     y  > GYRO_ORIENTATION_THRESHOLD ) &&
	     ( abs(z) < GYRO_ORIENTATION_THRESHOLD ) )
		return PORTRAIT_DOWN; // X=0g, Y=1g, Z=0g
	if ( (     x  < -GYRO_ORIENTATION_THRESHOLD ) &&
	     ( abs(y) < GYRO_ORIENTATION_THRESHOLD ) &&
	     ( abs(z) < GYRO_ORIENTATION_THRESHOLD ) )
		return LANDSCAPE_LEFT; // X=-1g, Y=0g, Z=0g
	return UNKNOWN;
}


uint8_t GYRO_init( void )
{
    if ( GYRO_ID_VAL != _readReg( GYRO_ID_REG ) )
    {
	    PRINTF("ERR: incorrect GYRO chip\n");
	    return 0;
    }
    _writeReg( 0x2B, 0x40); // soft reset
	DELAY_MS(1);
    _writeReg( 0x2A, 1 ); // set ACTIVE
	DELAY_MS(1);
	_writeReg( 0x11, 0xC0 ); // enable Portrait/Landscape detection
    DELAY_MS(1);
#if 0
    while (1)
	{
		int16_t x = GYRO_readX();
		int16_t y = GYRO_readY();
		int16_t z = GYRO_readZ();
		GRYRO_Orientation_t orientation = GYRO_getOrientation();
		PRINTF( "X=%d Y=%d Z=%d %d\r\n", x, y, z, (int)orientation );
		DELAY_MS(1000);
	}
#endif
	return 1;
}
