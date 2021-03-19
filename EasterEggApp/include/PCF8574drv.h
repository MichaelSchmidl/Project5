/*
 * PCF8574drv.h
 *
 *  Created on: 19.03.2021
 *      Author: mschmidl
 */

#ifndef INCLUDE_PCF8574DRV_H_
#define INCLUDE_PCF8574DRV_H_

#include "app.h"

uint8_t PCF8574_read( void );
void PCF8574_write( uint8_t val );

#endif /* INCLUDE_PCF8574DRV_H_ */
