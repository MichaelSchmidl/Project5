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

#include "CompBraille.h"


static const tlcPort _segmentsPerDisplay[1][6] =
{
    //  SegOL        SegOR        SegML        SegMR        SegUL        SegUR
    { _BRAILLE_OL, _BRAILLE_OR, _BRAILLE_ML, _BRAILLE_MR, _BRAILLE_UL, _BRAILLE_UR }
};

/**
 *         OL OR
 *         ML MR
 *         UL UR
 */
// bit mask for the "font byte" we need to drive the 7-segment display
#define _SEGOL_BIT  (1 )
#define _SEGML_BIT  (1 << 1U)
#define _SEGUL_BIT  (1 << 2U)
#define _SEGOR_BIT  (1 << 3U)
#define _SEGMR_BIT  (1 << 4U)
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
static const uint8_t segments_UNKNOWN     = _SEGOL_BIT | _SEGOR_BIT | _SEGML_BIT | _SEGMR_BIT | _SEGUL_BIT | _SEGUR_BIT;



void compBraille_showCharOnLEDs( char c, uint16_t brightness )
{
    uint8_t segmentMask = 0x01;
    uint8_t segmentsToShowBitField = segments_UNKNOWN;

    switch ( c )
    {
        case 'A':
        case 'a':
        case '1':
            segmentsToShowBitField = segments_A1;
            break;
        case 'B':
        case 'b':
        case '2':
            segmentsToShowBitField = segments_B2;
            break;
        default:
            segmentsToShowBitField = segments_UNKNOWN;
            break;
    } // switch c

    for ( int i = 0; i < 6; i++ )
    {
        if ( segmentsToShowBitField & segmentMask )
        {
        	TLC5955drv_setChannelBrightness ( _segmentsPerDisplay[0][i], brightness );
        }
        else
        {
        	TLC5955drv_setChannelBrightness ( _segmentsPerDisplay[0][i], 0 );
        }
        segmentMask <<= 1;
    }
}

