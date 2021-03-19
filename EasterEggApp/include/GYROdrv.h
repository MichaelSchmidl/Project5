/*
 * GYROdrv.h
 *
 *  Created on: 19.03.2021
 *      Author: mschmidl
 */

#ifndef INCLUDE_GYRODRV_H_
#define INCLUDE_GYRODRV_H_

#include "app.h"

uint8_t GYRO_init( void );

int16_t GYRO_readX( void );
int16_t GYRO_readY( void );
int16_t GYRO_readZ( void );


#endif /* INCLUDE_GYRODRV_H_ */
