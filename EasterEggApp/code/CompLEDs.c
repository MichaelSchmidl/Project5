/*
 * CompLEDs.c
 *
 *  Created on: 05.12.2019
 *      Author: mschmidl
 */

#include "app.h"
#include "CompLEDs.h"

#define FONT_WIDTH 5
#define FONT_HEIGHT 7

const uint8_t _glyphColumnLed[FONT_WIDTH][FONT_HEIGHT] =
{
   /* starting with left column */
   { _LED_H51, _LED_H52, _LED_H53, _LED_H54, _LED_H55, _LED_H56, _LED_H57 },
   { _LED_H41, _LED_H42, _LED_H43, _LED_H44, _LED_H45, _LED_H46, _LED_H47 },
   { _LED_H31, _LED_H32, _LED_H33, _LED_H34, _LED_H35, _LED_H36, _LED_H37 },
   { _LED_H21, _LED_H22, _LED_H23, _LED_H24, _LED_H25, _LED_H26, _LED_H27 },
   { _LED_H11, _LED_H12, _LED_H13, _LED_H14, _LED_H15, _LED_H16, _LED_H17 },
};

const uint8_t font5x7[] = {
        0x00, 0x00, 0x00, 0x00, 0x00,// (space)
        0x00, 0x00, 0x5F, 0x00, 0x00,// !
        0x00, 0x07, 0x00, 0x07, 0x00,// "
        0x14, 0x7F, 0x14, 0x7F, 0x14,// #
        0x24, 0x2A, 0x7F, 0x2A, 0x12,// $
        0x23, 0x13, 0x08, 0x64, 0x62,// %
        0x36, 0x49, 0x55, 0x22, 0x50,// &
        0x00, 0x05, 0x03, 0x00, 0x00,// '
        0x00, 0x1C, 0x22, 0x41, 0x00,// (
        0x00, 0x41, 0x22, 0x1C, 0x00,// )
        0x08, 0x2A, 0x1C, 0x2A, 0x08,// *
        0x08, 0x08, 0x3E, 0x08, 0x08,// +
        0x00, 0x50, 0x30, 0x00, 0x00,// ,
        0x08, 0x08, 0x08, 0x08, 0x08,// -
        0x00, 0x60, 0x60, 0x00, 0x00,// .
        0x20, 0x10, 0x08, 0x04, 0x02,// /
        0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
        0x00, 0x42, 0x7F, 0x40, 0x00,// 1
        0x42, 0x61, 0x51, 0x49, 0x46,// 2
        0x21, 0x41, 0x45, 0x4B, 0x31,// 3
        0x18, 0x14, 0x12, 0x7F, 0x10,// 4
        0x27, 0x45, 0x45, 0x45, 0x39,// 5
        0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
        0x01, 0x71, 0x09, 0x05, 0x03,// 7
        0x36, 0x49, 0x49, 0x49, 0x36,// 8
        0x06, 0x49, 0x49, 0x29, 0x1E,// 9
        0x00, 0x36, 0x36, 0x00, 0x00,// :
        0x00, 0x56, 0x36, 0x00, 0x00,// ;
        0x00, 0x08, 0x14, 0x22, 0x41,// <
        0x14, 0x14, 0x14, 0x14, 0x14,// =
        0x41, 0x22, 0x14, 0x08, 0x00,// >
        0x02, 0x01, 0x51, 0x09, 0x06,// ?
        0x32, 0x49, 0x79, 0x41, 0x3E,// @
        0x7E, 0x11, 0x11, 0x11, 0x7E,// A
        0x7F, 0x49, 0x49, 0x49, 0x36,// B
        0x3E, 0x41, 0x41, 0x41, 0x22,// C
        0x7F, 0x41, 0x41, 0x22, 0x1C,// D
        0x7F, 0x49, 0x49, 0x49, 0x41,// E
        0x7F, 0x09, 0x09, 0x01, 0x01,// F
        0x3E, 0x41, 0x41, 0x51, 0x32,// G
        0x7F, 0x08, 0x08, 0x08, 0x7F,// H
        0x00, 0x41, 0x7F, 0x41, 0x00,// I
        0x20, 0x40, 0x41, 0x3F, 0x01,// J
        0x7F, 0x08, 0x14, 0x22, 0x41,// K
        0x7F, 0x40, 0x40, 0x40, 0x40,// L
        0x7F, 0x02, 0x04, 0x02, 0x7F,// M
        0x7F, 0x04, 0x08, 0x10, 0x7F,// N
        0x3E, 0x41, 0x41, 0x41, 0x3E,// O
        0x7F, 0x09, 0x09, 0x09, 0x06,// P
        0x3E, 0x41, 0x51, 0x21, 0x5E,// Q
        0x7F, 0x09, 0x19, 0x29, 0x46,// R
        0x46, 0x49, 0x49, 0x49, 0x31,// S
        0x01, 0x01, 0x7F, 0x01, 0x01,// T
        0x3F, 0x40, 0x40, 0x40, 0x3F,// U
        0x1F, 0x20, 0x40, 0x20, 0x1F,// V
        0x7F, 0x20, 0x18, 0x20, 0x7F,// W
        0x63, 0x14, 0x08, 0x14, 0x63,// X
        0x03, 0x04, 0x78, 0x04, 0x03,// Y
        0x61, 0x51, 0x49, 0x45, 0x43,// Z
        0x00, 0x00, 0x7F, 0x41, 0x41,// [
        0x02, 0x04, 0x08, 0x10, 0x20,// "\"
        0x41, 0x41, 0x7F, 0x00, 0x00,// ]
        0x04, 0x02, 0x01, 0x02, 0x04,// ^
        0x40, 0x40, 0x40, 0x40, 0x40,// _
        0x00, 0x01, 0x02, 0x04, 0x00,// `
        0x20, 0x54, 0x54, 0x54, 0x78,// a
        0x7F, 0x48, 0x44, 0x44, 0x38,// b
        0x38, 0x44, 0x44, 0x44, 0x20,// c
        0x38, 0x44, 0x44, 0x48, 0x7F,// d
        0x38, 0x54, 0x54, 0x54, 0x18,// e
        0x08, 0x7E, 0x09, 0x01, 0x02,// f
        0x08, 0x14, 0x54, 0x54, 0x3C,// g
        0x7F, 0x08, 0x04, 0x04, 0x78,// h
        0x00, 0x44, 0x7D, 0x40, 0x00,// i
        0x20, 0x40, 0x44, 0x3D, 0x00,// j
        0x00, 0x7F, 0x10, 0x28, 0x44,// k
        0x00, 0x41, 0x7F, 0x40, 0x00,// l
        0x7C, 0x04, 0x18, 0x04, 0x78,// m
        0x7C, 0x08, 0x04, 0x04, 0x78,// n
        0x38, 0x44, 0x44, 0x44, 0x38,// o
        0x7C, 0x14, 0x14, 0x14, 0x08,// p
        0x08, 0x14, 0x14, 0x18, 0x7C,// q
        0x7C, 0x08, 0x04, 0x04, 0x08,// r
        0x48, 0x54, 0x54, 0x54, 0x20,// s
        0x04, 0x3F, 0x44, 0x40, 0x20,// t
        0x3C, 0x40, 0x40, 0x20, 0x7C,// u
        0x1C, 0x20, 0x40, 0x20, 0x1C,// v
        0x3C, 0x40, 0x30, 0x40, 0x3C,// w
        0x44, 0x28, 0x10, 0x28, 0x44,// x
        0x0C, 0x50, 0x50, 0x50, 0x3C,// y
        0x44, 0x64, 0x54, 0x4C, 0x44,// z
        0x00, 0x08, 0x36, 0x41, 0x00,// {
        0x00, 0x00, 0x7F, 0x00, 0x00,// |
        0x00, 0x41, 0x36, 0x08, 0x00,// }
        0x08, 0x08, 0x2A, 0x1C, 0x08,// ->
        0x08, 0x1C, 0x2A, 0x08, 0x08 // <-
};

static void _renderGlyph( char c, int xPos, int glyphShiftLeft );


/*!*************************************************************************************************************************************************************
 *
 **************************************************************************************************************************************************************/
#define _CHARACTER_GAP 1
void LED_showText( const char *szString )
{
    PRINTF("%s(%s)\r\n", __func__, szString);

	char c_now;
	char c_next;
	while( *szString != '\0' )
	{
		c_now = *szString++;
		c_next = *szString;
		if ( c_next == '\0' ) c_next = ' ';
		for ( int n = 0; n < FONT_WIDTH; n++ )
		{
			_renderGlyph( ' ', 0, 0 ); // clear all dots
			_renderGlyph( c_now, 0, n );
			if ( n > 0 )
			{
				_renderGlyph( ' ', FONT_WIDTH - n , 0 );
			}
			if ( n > _CHARACTER_GAP )
			{
				_renderGlyph( c_next, FONT_WIDTH - (n - _CHARACTER_GAP) , 0 );
			}
			/* Dispatch all events in Kernel queue */
			Kernel_Schedule();
	        /* Refresh the watchdog timer */
	        Sys_Watchdog_Refresh();
			TLC5955drv_refresh();
			DELAY_MS( _CHAR_ANIMATION_DELAY_MS );
			if ( n == 0 ) DELAY_MS( _CHAR_READING_DELAY_MS );
		}
	}
}


/*!*************************************************************************************************************************************************************
 *
 **************************************************************************************************************************************************************/
static void _renderGlyph( char c, int xPos, int glyphShiftLeft )
{
	if ( xPos > FONT_WIDTH ) return;
	if ( glyphShiftLeft > FONT_WIDTH ) return;

	int font5x7pos = ((c - ' ') * FONT_WIDTH) ;
    if ( font5x7pos >= sizeof(font5x7) ) return;

    int glyphEndPos = font5x7pos + FONT_WIDTH;

    // set real startpoint based on given glyphShiftLeft
    font5x7pos += glyphShiftLeft;

	for ( int x = xPos; x < FONT_WIDTH; x++ )
	{
		uint8_t mask = 1;
        for ( int y = 0; y < FONT_HEIGHT; y++ )
        {
        	if ( font5x7[font5x7pos] & mask )
        	{
        		TLC5955drv_setChannelBrightness( _glyphColumnLed[x][y], _DEFAULT_BRIGHTNESS );
        	}
        	else
        	{
        		TLC5955drv_setChannelBrightness( _glyphColumnLed[x][y], 0 );
        	}
        	mask <<= 1;
        }
        font5x7pos++;
        if ( font5x7pos >= glyphEndPos ) return;
	}
}


/*!*************************************************************************************************************************************************************
 *
 **************************************************************************************************************************************************************/
void LED_renderGlyph( char c )
{
	_renderGlyph( c, 0, 0 );
    TLC5955drv_refresh();
}


/*!*************************************************************************************************************************************************************
 *
 **************************************************************************************************************************************************************/
void LED_setDbgLed( tlcPort which, uint16_t brightness )
{
	switch ( which )
	{
	    case 1:
		   TLC5955drv_setChannelBrightness(_DGB_LED_H3, brightness);
		   break;
	    case 2:
		   TLC5955drv_setChannelBrightness(_DGB_LED_H4, brightness);
		   break;
	    case 3:
		   TLC5955drv_setChannelBrightness(_DGB_LED_H5, brightness);
		   break;
	    default:
	       return;
		   break;
	}
    TLC5955drv_refresh();
}


/*!*************************************************************************************************************************************************************
 *
 **************************************************************************************************************************************************************/
void LED_setBLEconnectedIndicator( uint16_t On )
{
	TLC5955drv_setChannelBrightness( _BLE_CONNECTED_LED1, On * _INDICATOR_BRIGHTNESS );
	TLC5955drv_setChannelBrightness( _BLE_CONNECTED_LED2, On * _INDICATOR_BRIGHTNESS );
	TLC5955drv_setChannelBrightness( _BLE_CONNECTED_LED3, On * _INDICATOR_BRIGHTNESS );
	TLC5955drv_setChannelBrightness( _BLE_CONNECTED_LED4, On * _INDICATOR_BRIGHTNESS );
    TLC5955drv_refresh();
}


/*!*************************************************************************************************************************************************************
 *
 **************************************************************************************************************************************************************/
void LED_setBLEADVIndicator( uint16_t On )
{
	TLC5955drv_setChannelBrightness( _BLE_ADV_LED, On * _INDICATOR_BRIGHTNESS );
    TLC5955drv_refresh();
}


/*!*************************************************************************************************************************************************************
 *
 **************************************************************************************************************************************************************/
void LED_setGYROIndicator( GRYRO_Orientation_t orientation )
{
	switch ( orientation )
	{
		case FRONT:
			TLC5955drv_setChannelBrightness( _LED_H54, 0 );
			TLC5955drv_setChannelBrightness( _LED_H37, 0 );
			TLC5955drv_setChannelBrightness( _LED_H14, 0 );
			TLC5955drv_setChannelBrightness( _LED_H31, 0 );
		    TLC5955drv_refresh();
		    break;
		case BACK:
			TLC5955drv_setChannelBrightness( _LED_H54, _DEFAULT_BRIGHTNESS );
			TLC5955drv_setChannelBrightness( _LED_H37, _DEFAULT_BRIGHTNESS );
			TLC5955drv_setChannelBrightness( _LED_H14, _DEFAULT_BRIGHTNESS );
			TLC5955drv_setChannelBrightness( _LED_H31, _DEFAULT_BRIGHTNESS );
		    TLC5955drv_refresh();
		    break;
		case PORTRAIT_UP:
			TLC5955drv_setChannelBrightness( _LED_H54, _DEFAULT_BRIGHTNESS );
			TLC5955drv_setChannelBrightness( _LED_H37, 0 );
			TLC5955drv_setChannelBrightness( _LED_H14, 0 );
			TLC5955drv_setChannelBrightness( _LED_H31, 0 );
		    TLC5955drv_refresh();
		    break;
		case LANDSCAPE_RIGHT:
			TLC5955drv_setChannelBrightness( _LED_H54, 0 );
			TLC5955drv_setChannelBrightness( _LED_H37, 0 );
			TLC5955drv_setChannelBrightness( _LED_H14, 0 );
			TLC5955drv_setChannelBrightness( _LED_H31, _DEFAULT_BRIGHTNESS );
		    TLC5955drv_refresh();
		    break;
		case PORTRAIT_DOWN:
			TLC5955drv_setChannelBrightness( _LED_H54, 0 );
			TLC5955drv_setChannelBrightness( _LED_H37, 0 );
			TLC5955drv_setChannelBrightness( _LED_H14, _DEFAULT_BRIGHTNESS );
			TLC5955drv_setChannelBrightness( _LED_H31, 0 );
		    TLC5955drv_refresh();
		    break;
		case LANDSCAPE_LEFT:
			TLC5955drv_setChannelBrightness( _LED_H54, 0 );
			TLC5955drv_setChannelBrightness( _LED_H37, _DEFAULT_BRIGHTNESS );
			TLC5955drv_setChannelBrightness( _LED_H14, 0 );
			TLC5955drv_setChannelBrightness( _LED_H31, 0 );
		    TLC5955drv_refresh();
		    break;
		default:
			break;
	}

}
