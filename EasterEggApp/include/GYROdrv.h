/*
 * GYROdrv.h
 *
 *  Created on: 19.03.2021
 *      Author: mschmidl
 */

#ifndef INCLUDE_GYRODRV_H_
#define INCLUDE_GYRODRV_H_

#include "app.h"

typedef enum {
	UNKNOWN = 0,
	BACK = 1,
	FRONT = 2,
	PORTRAIT_UP = 3,
	LANDSCAPE_RIGHT = 4,
	PORTRAIT_DOWN = 5,
	LANDSCAPE_LEFT = 6
}GRYRO_Orientation_t;

uint8_t GYRO_init( void );

int16_t GYRO_readX( void );
int16_t GYRO_readY( void );
int16_t GYRO_readZ( void );
int8_t GYRO_getPL( void );
GRYRO_Orientation_t GYRO_getOrientation( void );


#endif /* INCLUDE_GYRODRV_H_ */
