/*
 * CompBraille.c
 *
 *  Created on: 27.01.2021
 *      Author: mschmidl
 *
 *  https://de.wikipedia.org/wiki/Brailleschrift
 *
 *  Da BRAILLE für Zahlen und Buchstaben den gleichen Code verwendet, muss vor jeder Zahl ein # stehen.
 *  Ebenso werden Großbuchstaben durch vorangestelltes $ angezeigt.
 *  "Test123" -> "$test#1#2#3"
 */

#include <ctype.h>
#include "CompBraille.h"

#define _BRAILLE_READING_DELAY_CYCLES   ( 50 * _CHAR_ANIMATION_DELAY_CYCLES )

#define _BRAILLE_OL         _LED_H42
#define _BRAILLE_OR         _LED_H22
#define _BRAILLE_ML         _LED_H44
#define _BRAILLE_MR         _LED_H24
#define _BRAILLE_UL         _LED_H46
#define _BRAILLE_UR         _LED_H26

static const tlcPort _brailleLeds[6] =
{
    //  SegOL        SegOR        SegML        SegMR        SegUL        SegUR
      _BRAILLE_OL, _BRAILLE_OR, _BRAILLE_ML, _BRAILLE_MR, _BRAILLE_UL, _BRAILLE_UR
};

/**
 *         OL OR
 *         ML MR
 *         UL UR
 */
// bit mask for the "font byte" we need to drive the dot matrix display
#define _SEGOL_BIT  (1 )
#define _SEGOR_BIT  (1 << 1U)
#define _SEGML_BIT  (1 << 2U)
#define _SEGMR_BIT  (1 << 3U)
#define _SEGUL_BIT  (1 << 4U)
#define _SEGUR_BIT  (1 << 5U)
//                                          _SEGOL_BIT | _SEGOR_BIT | _SEGML_BIT | _SEGMR_BIT | _SEGUL_BIT | _SEGUR_BIT;
static const uint8_t segments_A1          = _SEGOL_BIT                                                                 ;
static const uint8_t segments_B2          = _SEGOL_BIT              | _SEGML_BIT                                       ;
static const uint8_t segments_C3          = _SEGOL_BIT | _SEGOR_BIT                                                    ;
static const uint8_t segments_D4          = _SEGOL_BIT | _SEGOR_BIT              | _SEGMR_BIT                          ;
static const uint8_t segments_E5          = _SEGOL_BIT                           | _SEGMR_BIT                          ;
static const uint8_t segments_F6          = _SEGOL_BIT | _SEGOR_BIT | _SEGML_BIT                                       ;
static const uint8_t segments_G7          = _SEGOL_BIT | _SEGOR_BIT | _SEGML_BIT | _SEGMR_BIT                          ;
static const uint8_t segments_H8          = _SEGOL_BIT              | _SEGML_BIT | _SEGMR_BIT                          ;
static const uint8_t segments_I9          =              _SEGOR_BIT | _SEGML_BIT                                       ;
static const uint8_t segments_J0          =              _SEGOR_BIT | _SEGML_BIT | _SEGMR_BIT                          ;
static const uint8_t segments_K           = _SEGOL_BIT                                        | _SEGUL_BIT             ;
static const uint8_t segments_L           = _SEGOL_BIT              | _SEGML_BIT              | _SEGUL_BIT             ;
static const uint8_t segments_M           = _SEGOL_BIT | _SEGOR_BIT                           | _SEGUL_BIT             ;
static const uint8_t segments_N           = _SEGOL_BIT | _SEGOR_BIT              | _SEGMR_BIT | _SEGUL_BIT             ;
static const uint8_t segments_O           = _SEGOL_BIT                           | _SEGMR_BIT | _SEGUL_BIT             ;
static const uint8_t segments_P           = _SEGOL_BIT | _SEGOR_BIT | _SEGML_BIT              | _SEGUL_BIT             ;
static const uint8_t segments_Q           = _SEGOL_BIT | _SEGOR_BIT | _SEGML_BIT | _SEGMR_BIT | _SEGUL_BIT             ;
static const uint8_t segments_R           = _SEGOL_BIT              | _SEGML_BIT | _SEGMR_BIT | _SEGUL_BIT             ;
static const uint8_t segments_S           =              _SEGOR_BIT | _SEGML_BIT              | _SEGUL_BIT             ;
static const uint8_t segments_T           =              _SEGOR_BIT | _SEGML_BIT | _SEGMR_BIT | _SEGUL_BIT             ;
static const uint8_t segments_U           = _SEGOL_BIT                                        | _SEGUL_BIT | _SEGUR_BIT;
static const uint8_t segments_V           = _SEGOL_BIT              | _SEGML_BIT              | _SEGUL_BIT | _SEGUR_BIT;
static const uint8_t segments_W           =              _SEGOR_BIT | _SEGML_BIT | _SEGMR_BIT              | _SEGUR_BIT;
static const uint8_t segments_X           = _SEGOL_BIT | _SEGOR_BIT                           | _SEGUL_BIT | _SEGUR_BIT;
static const uint8_t segments_Y           = _SEGOL_BIT | _SEGOR_BIT              | _SEGMR_BIT | _SEGUL_BIT | _SEGUR_BIT;
static const uint8_t segments_Z           = _SEGOL_BIT                           | _SEGMR_BIT | _SEGUL_BIT | _SEGUR_BIT;
static const uint8_t segments_ZAHL        =              _SEGOR_BIT              | _SEGMR_BIT | _SEGUL_BIT | _SEGUR_BIT; //#
static const uint8_t segments_GROSS       =              _SEGOR_BIT                                        | _SEGUR_BIT; //$
static const uint8_t segments_PUNKT       =                                                     _SEGUL_BIT             ; //.
static const uint8_t segments_DOPPELPUNKT =                           _SEGML_BIT | _SEGMR_BIT                          ; //:
static const uint8_t segments_MINUS       =                                                     _SEGUL_BIT | _SEGUR_BIT; //-
static const uint8_t segments_SPACE       = 0                                                                          ; //SPACE
static const uint8_t segments_UNKNOWN     = _SEGOL_BIT | _SEGOR_BIT | _SEGML_BIT | _SEGMR_BIT | _SEGUL_BIT | _SEGUR_BIT;



void compBraille_renderGlyph( char c )
{
    uint8_t ledsToShowBitField = segments_UNKNOWN;

    switch ( c )
    {
        case '#': // ZAHL folgt
           ledsToShowBitField = segments_ZAHL;
           break;
        case '$': // GROSSBUCHSTABE folgt
           ledsToShowBitField = segments_GROSS;
           break;
        case '.':
           ledsToShowBitField = segments_PUNKT;
           break;
        case ':':
           ledsToShowBitField = segments_DOPPELPUNKT;
           break;
        case '-':
           ledsToShowBitField = segments_MINUS;
           break;
        case ' ':
            ledsToShowBitField = segments_SPACE;
            break;
        case 'A':
        case 'a':
        case '1':
            ledsToShowBitField = segments_A1;
            break;
        case 'B':
        case 'b':
        case '2':
            ledsToShowBitField = segments_B2;
            break;
        case 'C':
        case 'c':
        case '3':
            ledsToShowBitField = segments_C3;
            break;
        case 'D':
        case 'd':
        case '4':
            ledsToShowBitField = segments_D4;
            break;
        case 'E':
        case 'e':
        case '5':
            ledsToShowBitField = segments_E5;
            break;
        case 'F':
        case 'f':
        case '6':
            ledsToShowBitField = segments_F6;
            break;
        case 'G':
        case 'g':
        case '7':
            ledsToShowBitField = segments_G7;
            break;
        case 'H':
        case 'h':
        case '8':
            ledsToShowBitField = segments_H8;
            break;
        case 'I':
        case 'i':
        case '9':
            ledsToShowBitField = segments_I9;
            break;
        case 'J':
        case 'j':
        case '0':
            ledsToShowBitField = segments_J0;
            break;
        case 'K':
        case 'k':
            ledsToShowBitField = segments_K;
            break;
        case 'L':
        case 'l':
            ledsToShowBitField = segments_L;
            break;
        case 'M':
        case 'm':
            ledsToShowBitField = segments_M;
            break;
        case 'N':
        case 'n':
            ledsToShowBitField = segments_N;
            break;
        case 'O':
        case 'o':
            ledsToShowBitField = segments_O;
            break;
        case 'P':
        case 'p':
            ledsToShowBitField = segments_P;
            break;
        case 'Q':
        case 'q':
            ledsToShowBitField = segments_Q;
            break;
        case 'R':
        case 'r':
            ledsToShowBitField = segments_R;
            break;
        case 'S':
        case 's':
            ledsToShowBitField = segments_S;
            break;
        case 'T':
        case 't':
            ledsToShowBitField = segments_T;
            break;
        case 'U':
        case 'u':
            ledsToShowBitField = segments_U;
            break;
        case 'V':
        case 'v':
            ledsToShowBitField = segments_V;
            break;
        case 'W':
        case 'w':
            ledsToShowBitField = segments_W;
            break;
        case 'X':
        case 'x':
            ledsToShowBitField = segments_X;
            break;
        case 'Y':
        case 'y':
            ledsToShowBitField = segments_Y;
            break;
        case 'Z':
        case 'z':
            ledsToShowBitField = segments_Z;
            break;
        default:
            ledsToShowBitField = segments_UNKNOWN;
            break;
    } // switch c

    uint8_t mask = 0x01;
    for ( int i = 0; i < 6; i++ )
    {
        if ( ledsToShowBitField & mask )
        {
        	TLC5955drv_setChannelBrightness ( _brailleLeds[i], _DEFAULT_BRIGHTNESS );
        }
        else
        {
        	TLC5955drv_setChannelBrightness ( _brailleLeds[i], 0 );
        }
        mask <<= 1;
    }
}


void compBraille_showText( char *pszText )
{
	while( *pszText != '\0' )
	{
		char c = *pszText++;
		LED_renderGlyph( ' '  ); // clear all dots
		if ( isdigit( c ) )
		{
			compBraille_renderGlyph( '#' );
			TLC5955drv_refresh();
			Sys_Delay_ProgramROM( _BRAILLE_READING_DELAY_CYCLES );
		}
		else if ( isupper( c ) )
		{
			compBraille_renderGlyph( '$' );
			TLC5955drv_refresh();
			Sys_Delay_ProgramROM( _BRAILLE_READING_DELAY_CYCLES );
		}
		compBraille_renderGlyph( c );
		TLC5955drv_refresh();
		Sys_Delay_ProgramROM( _BRAILLE_READING_DELAY_CYCLES );
	}
}
