/*
 * CompLEDs.c
 *
 *  Created on: 05.12.2019
 *      Author: mschmidl
 */

#include "CompLEDs.h"

#define _DGB_LED_H3          tlcPortR0
#define _DGB_LED_H4          tlcPortG0
#define _DGB_LED_H5          tlcPortB0

#define _DISP4_SEG_A         tlcPortR5
#define _DISP4_SEG_B         tlcPortB5
#define _DISP4_SEG_C         tlcPortG0
#define _DISP4_SEG_D         tlcPortG1
#define _DISP4_SEG_E         tlcPortB2
#define _DISP4_SEG_F         tlcPortR1
#define _DISP4_SEG_G         tlcPortG5

#define _DISP3_SEG_A         tlcPortR6
#define _DISP3_SEG_B         tlcPortG2
#define _DISP3_SEG_C         tlcPortB6
#define _DISP3_SEG_D         tlcPortG3
#define _DISP3_SEG_E         tlcPortB7
#define _DISP3_SEG_F         tlcPortR3
#define _DISP3_SEG_G         tlcPortG6

#define _DISP2_SEG_A         tlcPortB11
#define _DISP2_SEG_B         tlcPortR11
#define _DISP2_SEG_C         tlcPortG15
#define _DISP2_SEG_D         tlcPortB15
#define _DISP2_SEG_E         tlcPortB10
#define _DISP2_SEG_F         tlcPortR10
#define _DISP2_SEG_G         tlcPortR15

#define _DISP1_SEG_A         tlcPortR9
#define _DISP1_SEG_B         tlcPortG13
#define _DISP1_SEG_C         tlcPortB14
#define _DISP1_SEG_D         tlcPortG9
#define _DISP1_SEG_E         tlcPortR12
#define _DISP1_SEG_F         tlcPortB9
#define _DISP1_SEG_G         tlcPortB13

static const tlcPort segmentsPerDisplay[4][7] =
{
    //  SegA          SegB          SegC          SegD          SegE          SegF          SegG
    { _DISP1_SEG_A, _DISP1_SEG_B, _DISP1_SEG_C, _DISP1_SEG_D, _DISP1_SEG_E, _DISP1_SEG_F, _DISP1_SEG_G }, // ledDispDrv_Display_INFO3
    { _DISP2_SEG_A, _DISP2_SEG_B, _DISP2_SEG_C, _DISP2_SEG_D, _DISP2_SEG_E, _DISP2_SEG_F, _DISP2_SEG_G }, // ledDispDrv_Display_INFO2
    { _DISP3_SEG_A, _DISP3_SEG_B, _DISP3_SEG_C, _DISP3_SEG_D, _DISP3_SEG_E, _DISP3_SEG_F, _DISP3_SEG_G }, // ledDispDrv_Display_INFO1
    { _DISP4_SEG_A, _DISP4_SEG_B, _DISP4_SEG_C, _DISP4_SEG_D, _DISP4_SEG_E, _DISP4_SEG_F, _DISP4_SEG_G }, // ledDispDrv_Display_INFO0
};

/**
 *             A
 *         F       B
 *             G
 *         E       C
 *             D
 */
// bit mask for the "font byte" we need to drive the 7-segment display
#define _SEGA_BIT  (1 )
#define _SEGB_BIT  (1 << 1U)
#define _SEGC_BIT  (1 << 2U)
#define _SEGD_BIT  (1 << 3U)
#define _SEGE_BIT  (1 << 4U)
#define _SEGF_BIT  (1 << 5U)
#define _SEGG_BIT  (1 << 6U)
//                                      _SEGA_BIT | _SEGB_BIT | _SEGC_BIT | _SEGD_BIT | _SEGE_BIT | _SEGF_BIT | _SEGG_BIT
static const uint8_t segments_A       = _SEGA_BIT | _SEGB_BIT | _SEGC_BIT             | _SEGE_BIT | _SEGF_BIT | _SEGG_BIT  ;//!< A = ABCEFG
static const uint8_t segments_b       =                         _SEGC_BIT | _SEGD_BIT | _SEGE_BIT | _SEGF_BIT | _SEGG_BIT  ;//!< b = CDEFG
static const uint8_t segments_C       = _SEGA_BIT                         | _SEGD_BIT | _SEGE_BIT | _SEGF_BIT              ;//!< C = ADEF
static const uint8_t segments_c       =                                     _SEGD_BIT | _SEGE_BIT             | _SEGG_BIT  ;//!< c = DEG
static const uint8_t segments_d       =             _SEGB_BIT | _SEGC_BIT | _SEGD_BIT | _SEGE_BIT             | _SEGG_BIT  ;//!< d = BCDEG
static const uint8_t segments_E       = _SEGA_BIT                         | _SEGD_BIT | _SEGE_BIT | _SEGF_BIT | _SEGG_BIT  ;//!< E = ADEFG
static const uint8_t segments_F       = _SEGA_BIT                                     | _SEGE_BIT | _SEGF_BIT | _SEGG_BIT  ;//!< F = AEFG
static const uint8_t segments_H       =             _SEGB_BIT | _SEGC_BIT             | _SEGE_BIT | _SEGF_BIT | _SEGG_BIT  ;//!< H = BCEFG
static const uint8_t segments_h       =                         _SEGC_BIT             | _SEGE_BIT | _SEGF_BIT | _SEGG_BIT  ;//!< h = CEFG
static const uint8_t segments_L       =                                     _SEGD_BIT | _SEGE_BIT | _SEGF_BIT              ;//!< L = DEF
static const uint8_t segments_n       =                         _SEGC_BIT             | _SEGE_BIT             | _SEGG_BIT  ;//!< n = CEG
static const uint8_t segments_o       =                         _SEGC_BIT | _SEGD_BIT | _SEGE_BIT             | _SEGG_BIT  ;//!< o = CDEG
static const uint8_t segments_P       = _SEGA_BIT | _SEGB_BIT                         | _SEGE_BIT | _SEGF_BIT | _SEGG_BIT  ;//!< P = ABEFG
static const uint8_t segments_r       =                                                 _SEGE_BIT             | _SEGG_BIT  ;//!< r = EG
static const uint8_t segments_S       = _SEGA_BIT             | _SEGC_BIT | _SEGD_BIT             | _SEGF_BIT | _SEGG_BIT  ;//!< S = ACDFG
static const uint8_t segments_t       =                                     _SEGD_BIT | _SEGE_BIT | _SEGF_BIT | _SEGG_BIT  ;//!< t = DEFG
static const uint8_t segments_u       =                         _SEGC_BIT | _SEGD_BIT | _SEGE_BIT                          ;//!< u = CDE
static const uint8_t segments_U       =             _SEGB_BIT | _SEGC_BIT | _SEGD_BIT | _SEGE_BIT | _SEGF_BIT              ;//!< U = BCDEF
static const uint8_t segments_Y       =             _SEGB_BIT | _SEGC_BIT                       | _SEGF_BIT   | _SEGG_BIT  ;//!< y = BCFG
static const uint8_t segments_0       = _SEGA_BIT | _SEGB_BIT | _SEGC_BIT | _SEGD_BIT | _SEGE_BIT | _SEGF_BIT              ;//!< 0 = ABCDEF
static const uint8_t segments_1       =             _SEGB_BIT | _SEGC_BIT                                                  ;//!< 1 = BC
static const uint8_t segments_2       = _SEGA_BIT | _SEGB_BIT             | _SEGD_BIT | _SEGE_BIT             | _SEGG_BIT  ;//!< 2 = ABDEG
static const uint8_t segments_3       = _SEGA_BIT | _SEGB_BIT | _SEGC_BIT | _SEGD_BIT                         | _SEGG_BIT  ;//!< 3 = ABCDG
static const uint8_t segments_4       =             _SEGB_BIT | _SEGC_BIT                         | _SEGF_BIT | _SEGG_BIT  ;//!< 4 = BCFG
static const uint8_t segments_5       = _SEGA_BIT             | _SEGC_BIT | _SEGD_BIT             | _SEGF_BIT | _SEGG_BIT  ;//!< 5 = ACDFG
static const uint8_t segments_6       = _SEGA_BIT             | _SEGC_BIT | _SEGD_BIT | _SEGE_BIT | _SEGF_BIT | _SEGG_BIT  ;//!< 6 = ACDEFG
static const uint8_t segments_7       = _SEGA_BIT | _SEGB_BIT | _SEGC_BIT                                                  ;//!< 7 = ABC
static const uint8_t segments_8       = _SEGA_BIT | _SEGB_BIT | _SEGC_BIT | _SEGD_BIT | _SEGE_BIT | _SEGF_BIT | _SEGG_BIT  ;//!< 8 = ABCDEFG
static const uint8_t segments_9       = _SEGA_BIT | _SEGB_BIT | _SEGC_BIT | _SEGD_BIT             | _SEGF_BIT | _SEGG_BIT  ;//!< 9 = ABCDFG
static const uint8_t segments__       =                                     _SEGD_BIT                                      ;//!< _ = D
static const uint8_t segments_MINUS   =                                                                         _SEGG_BIT  ;//!< - = G
static const uint8_t segments_EQUAL   =                                     _SEGD_BIT                         | _SEGG_BIT  ;//!< = = DG
// special "characters" follow now...
//static const uint8_t segments_UPPERO  = _SEGA_BIT | _SEGB_BIT                                     | _SEGF_BIT | _SEGG_BIT  ; //!< ABFG
//static const uint8_t segments_FULL    = _SEGA_BIT                         | _SEGD_BIT                         | _SEGG_BIT  ;//!< ADG
//static const uint8_t segments_USCORE  = _SEGA_BIT                                                                          ;
static const uint8_t segments_UNKNOWN = _SEGA_BIT                         | _SEGD_BIT                                      ;

/*!*************************************************************************************************************************************************************
 *
 **************************************************************************************************************************************************************/
static void _set7SEGDisplay( uint8_t displayNr,
                      char symbol,
                      uint16_t brightness)
{
    uint8_t segmentMask = 0x01;
    uint8_t i;
    uint8_t segmentsToShowBitField;

    switch ( symbol )
    {
        case 'A':
            segmentsToShowBitField = segments_A;
            break;
        case 'b':
            segmentsToShowBitField = segments_b;
            break;
        case 'C':
            segmentsToShowBitField = segments_C;
            break;
        case 'c':
            segmentsToShowBitField = segments_c;
            break;
        case 'd':
            segmentsToShowBitField = segments_d;
            break;
        case 'E':
            segmentsToShowBitField = segments_E;
            break;
        case 'F':
            segmentsToShowBitField = segments_F;
            break;
        case 'H':
            segmentsToShowBitField = segments_H;
            break;
        case 'h':
            segmentsToShowBitField = segments_h;
            break;
        case 'L':
            segmentsToShowBitField = segments_L;
            break;
        case 'n':
            segmentsToShowBitField = segments_n;
            break;
        case 'o':
            segmentsToShowBitField = segments_o;
            break;
        case 'P':
            segmentsToShowBitField = segments_P;
            break;
        case 'r':
            segmentsToShowBitField = segments_r;
            break;
        case 'S':
            segmentsToShowBitField = segments_S;
            break;
        case 't':
            segmentsToShowBitField = segments_t;
            break;
        case 'U':
            segmentsToShowBitField = segments_U;
            break;
        case 'u':
            segmentsToShowBitField = segments_u;
            break;
        case 'Y':
            segmentsToShowBitField = segments_Y;
            break;
        case 'O':
        case '0':
            segmentsToShowBitField = segments_0;
            break;
        case '1':
            segmentsToShowBitField = segments_1;
            break;
        case '2':
            segmentsToShowBitField = segments_2;
            break;
        case '3':
            segmentsToShowBitField = segments_3;
            break;
        case '4':
            segmentsToShowBitField = segments_4;
            break;
        case '5':
            segmentsToShowBitField = segments_5;
            break;
        case '6':
            segmentsToShowBitField = segments_6;
            break;
        case '7':
            segmentsToShowBitField = segments_7;
            break;
        case '8':
            segmentsToShowBitField = segments_8;
            break;
        case '9':
            segmentsToShowBitField = segments_9;
            break;
        case '_':
            segmentsToShowBitField = segments__;
            break;
        case '-':
            segmentsToShowBitField = segments_MINUS;
            break;
        case '=':
            segmentsToShowBitField = segments_EQUAL;
            break;
        case ' ':
            segmentsToShowBitField = 0; // no LEDs
            break;
        // special "characters" follow now - they are accessible with in reality unprintable characters like "BDefGgijKklMmNpQqRsTVvWwXxyZz"
        // these are all the unprintable characters...
        default:
            segmentsToShowBitField = segments_UNKNOWN;
            break;
    } // switch symbol

    for ( i = 0; i < 7; i++ )
    {
        if ( segmentsToShowBitField & segmentMask )
        {
        	TLC5955drv_setChannelBrightness ( segmentsPerDisplay[displayNr][i], brightness );
        }
        else
        {
        	TLC5955drv_setChannelBrightness ( segmentsPerDisplay[displayNr][i], 0 );
        }
        segmentMask <<= 1;
    }
}


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


void LED_setBLEconnectedIndicator( uint8_t On )
{
	TLC5955drv_setChannelBrightness( tlcPortR10, On * 25);
    TLC5955drv_refresh();
}

