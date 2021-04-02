/*
 * CompMorse.c
 *
 *  Created on: 07.03.2021
 *      Author: mschmidl
 */

#include "app.h"
#include "CompMorse.h"

#define _DIT_LENGTH_MS        ( 150 )                 // '1' coded in "font"
#define _DAH_LENGTH_MS        ( 3 * _DIT_LENGTH_MS )  // '3' coded in "font"
#define _INTERSYMBOL_DELAY_MS ( 1 * _DIT_LENGTH_MS )
#define _INTERCHAR_DELAY_MS   ( 3 * _DIT_LENGTH_MS )
#define _INTERWORD_DELAY_MS   ( 7 * _DIT_LENGTH_MS )

// '1' for DIT, '3' for DAH
static const char _szA[]="13";
static const char _szB[]="3111";
static const char _szC[]="3131";
static const char _szD[]="311";
static const char _szE[]="1";
static const char _szF[]="1131";
static const char _szG[]="331";
static const char _szH[]="1111";
static const char _szI[]="11";
static const char _szJ[]="1333";
static const char _szK[]="313";
static const char _szL[]="1311";
static const char _szM[]="33";
static const char _szN[]="31";
static const char _szO[]="333";
static const char _szP[]="1331";
static const char _szQ[]="3313";
static const char _szR[]="131";
static const char _szS[]="111";
static const char _szT[]="3";
static const char _szU[]="113";
static const char _szV[]="1113";
static const char _szW[]="133";
static const char _szX[]="3113";
static const char _szY[]="3133";
static const char _szZ[]="3311";
static const char _sz1[]="1333";
static const char _sz2[]="11333";
static const char _sz3[]="11133";
static const char _sz4[]="11113";
static const char _sz5[]="11111";
static const char _sz6[]="31111";
static const char _sz7[]="33111";
static const char _sz8[]="33311";
static const char _sz9[]="33331";
static const char _sz0[]="33333";


static void _setMorseLedsBrightness( uint16_t val )
{
	TLC5955drv_setChannelBrightness ( _LED_H43, val );
	TLC5955drv_setChannelBrightness ( _LED_H44, val );
	TLC5955drv_setChannelBrightness ( _LED_H45, val );

	TLC5955drv_setChannelBrightness ( _LED_H33, val );
	TLC5955drv_setChannelBrightness ( _LED_H34, val );
	TLC5955drv_setChannelBrightness ( _LED_H35, val );

	TLC5955drv_setChannelBrightness ( _LED_H23, val );
	TLC5955drv_setChannelBrightness ( _LED_H24, val );
	TLC5955drv_setChannelBrightness ( _LED_H25, val );
}


static void _sendMorse( const char *pszDitsDahs )
{
	while ( *pszDitsDahs != '\0' )
	{
    	_setMorseLedsBrightness( _DEFAULT_BRIGHTNESS );
		TLC5955drv_refresh();
		int n = *pszDitsDahs - '0'; // "ASCII2INT"
		DELAY_MS( n * _DIT_LENGTH_MS );
		LED_renderGlyph( ' '  ); // clear all dots
		TLC5955drv_refresh();
		DELAY_MS( _INTERSYMBOL_DELAY_MS );
    	pszDitsDahs++;
	}
	DELAY_MS( _INTERCHAR_DELAY_MS - _INTERSYMBOL_DELAY_MS );
}


void compMorse_showChar( char c )
{
    PRINTF("%s('%c')\r\n", __func__, c);
	switch (c)
	{
		case ' ':
			LED_renderGlyph( ' '  ); // clear all dots
			TLC5955drv_refresh();
			DELAY_MS( _INTERWORD_DELAY_MS );
			break;
		case '0':
			_sendMorse( _sz0 );
			break;
		case '1':
			_sendMorse( _sz1 );
			break;
		case '2':
			_sendMorse( _sz2 );
			break;
		case '3':
			_sendMorse( _sz3 );
			break;
		case '4':
			_sendMorse( _sz4 );
			break;
		case '5':
			_sendMorse( _sz5 );
			break;
		case '6':
			_sendMorse( _sz6 );
			break;
		case '7':
			_sendMorse( _sz7 );
			break;
		case '8':
			_sendMorse( _sz8 );
			break;
		case '9':
			_sendMorse( _sz9 );
			break;
		case 'A':
		case 'a':
			_sendMorse( _szA );
			break;
		case 'B':
		case 'b':
			_sendMorse( _szB );
			break;
		case 'C':
		case 'c':
			_sendMorse( _szC );
			break;
		case 'D':
		case 'd':
			_sendMorse( _szD );
			break;
		case 'E':
		case 'e':
			_sendMorse( _szE );
			break;
		case 'F':
		case 'f':
			_sendMorse( _szF );
			break;
		case 'G':
		case 'g':
			_sendMorse( _szG );
			break;
		case 'H':
		case 'h':
			_sendMorse( _szH );
			break;
		case 'I':
		case 'i':
			_sendMorse( _szI );
			break;
		case 'J':
		case 'j':
			_sendMorse( _szJ );
			break;
		case 'K':
		case 'k':
			_sendMorse( _szK );
			break;
		case 'L':
		case 'l':
			_sendMorse( _szL );
			break;
		case 'M':
		case 'm':
			_sendMorse( _szM );
			break;
		case 'N':
		case 'n':
			_sendMorse( _szN );
			break;
		case 'O':
		case 'o':
			_sendMorse( _szO );
			break;
		case 'P':
		case 'p':
			_sendMorse( _szP );
			break;
		case 'Q':
		case 'q':
			_sendMorse( _szQ );
			break;
		case 'R':
		case 'r':
			_sendMorse( _szR );
			break;
		case 'S':
		case 's':
			_sendMorse( _szS );
			break;
		case 'T':
		case 't':
			_sendMorse( _szT );
			break;
		case 'U':
		case 'u':
			_sendMorse( _szU );
			break;
		case 'V':
		case 'v':
			_sendMorse( _szV );
			break;
		case 'W':
		case 'w':
			_sendMorse( _szW );
			break;
		case 'X':
		case 'x':
			_sendMorse( _szX );
			break;
		case 'Y':
		case 'y':
			_sendMorse( _szY );
			break;
		case 'Z':
		case 'z':
			_sendMorse( _szZ );
			break;
		default:
			break;
	}
}


void compMorse_showText( char *pszText )
{
//    PRINTF("%s(%s)\r\n", __func__, pszText);

	while ( *pszText != '\0' )
	{
        /* Dispatch all events in Kernel queue */
        Kernel_Schedule();
		compMorse_showChar( *pszText++ );
	}
}
