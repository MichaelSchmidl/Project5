/*
 * CompBraille.h
 *
 *  Created on: 27.01.2021
 *      Author: mschmidl
 */

#ifndef INCLUDE_COMPBRAILLE_H_
#define INCLUDE_COMPBRAILLE_H_

/*!************************************************************************************************************************************************************
 *
 *************************************************************************************************************************************************************/
#include "TLC5955Drv.h"

#define _BRAILLE_OL         tlcPortR5
#define _BRAILLE_OR         tlcPortR5
#define _BRAILLE_ML         tlcPortR5
#define _BRAILLE_MR         tlcPortR5
#define _BRAILLE_UL         tlcPortR5
#define _BRAILLE_UR         tlcPortR5

void compBraille_showCharOnLEDs( char c, uint16_t brightness );



#endif /* INCLUDE_COMPBRAILLE_H_ */
