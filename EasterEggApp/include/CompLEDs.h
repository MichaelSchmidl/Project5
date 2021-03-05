/*
 * CompLEDs.h
 *
 *  Created on: 09.01.2020
 *      Author: mschmidl
 */

#ifndef COMP_LEDS_H_
#define COMP_LEDS_H_

/*!************************************************************************************************************************************************************
 *
 *************************************************************************************************************************************************************/
#include "TLC5955Drv.h"

void LED_setBLEconnectedIndicator( uint8_t On );

void LED_setDbgLed( tlcPort which, uint16_t brightness );

#endif /* COMP_LEDS_H_ */
