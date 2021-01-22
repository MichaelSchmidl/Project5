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

const char *compLed_getNameOfChannel( tlcPort channel );

void compLed_showStringOn7SEG( char *szString );


#endif /* COMP_LEDS_H_ */
