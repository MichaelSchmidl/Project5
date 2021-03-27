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
#include "GYROdrv.h"

#define _DEFAULT_BRIGHTNESS   500
#define _INDICATOR_BRIGHTNESS 10

#define _CHAR_ANIMATION_DELAY_MS    ( 50 ) // 50ms
#define _CHAR_READING_DELAY_MS      ( 10 * _CHAR_ANIMATION_DELAY_MS )

#define _DGB_LED_H3          tlcPortR0
#define _DGB_LED_H4          tlcPortG0
#define _DGB_LED_H5          tlcPortB0

#define _BLE_CONNECTED_LED1  tlcPortR8
#define _BLE_CONNECTED_LED2  tlcPortR3
#define _BLE_CONNECTED_LED3  tlcPortR11
#define _BLE_CONNECTED_LED4  tlcPortB6
#define _BLE_ADV_LED         tlcPortR10

#define _LED_H11             tlcPortR8
#define _LED_H12             tlcPortB12
#define _LED_H13             tlcPortR9
#define _LED_H14             tlcPortG13
#define _LED_H15             tlcPortB10
#define _LED_H16             tlcPortB3
#define _LED_H17             tlcPortB6

#define _LED_H21             tlcPortG8
#define _LED_H22             tlcPortB9
#define _LED_H23             tlcPortR13
#define _LED_H24             tlcPortG14
#define _LED_H25             tlcPortG10
#define _LED_H26             tlcPortR6
#define _LED_H27             tlcPortR2

#define _LED_H31             tlcPortG12
#define _LED_H32             tlcPortB13
#define _LED_H33             tlcPortR14
#define _LED_H34             tlcPortR10
#define _LED_H35             tlcPortG6
#define _LED_H36             tlcPortG2
#define _LED_H37             tlcPortB2

#define _LED_H41             tlcPortR12
#define _LED_H42             tlcPortG9
#define _LED_H43             tlcPortB14
#define _LED_H44             tlcPortB15
#define _LED_H45             tlcPortR15
#define _LED_H46             tlcPortG15
#define _LED_H47             tlcPortB11

#define _LED_H51             tlcPortR3
#define _LED_H52             tlcPortG3
#define _LED_H53             tlcPortB7
#define _LED_H54             tlcPortR7
#define _LED_H55             tlcPortG7
#define _LED_H56             tlcPortG11
#define _LED_H57             tlcPortR11

void LED_setBLEconnectedIndicator( uint16_t On );
void LED_setBLEADVIndicator( uint16_t On );
void LED_renderGlyph( char c );
void LED_showText( char *szString );
void LED_setDbgLed( tlcPort which, uint16_t brightness );
void LED_setGYROIndicator( GRYRO_Orientation_t orientation );

#endif /* COMP_LEDS_H_ */
