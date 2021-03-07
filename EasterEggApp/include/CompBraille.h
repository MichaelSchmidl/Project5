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
#include "CompLeds.h"


void compBraille_renderGlyph( char c );
void compBraille_showText( char *szText );


#endif /* INCLUDE_COMPBRAILLE_H_ */
