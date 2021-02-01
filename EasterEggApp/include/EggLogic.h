/*
 * EggLogic.h
 *
 *  Created on: 26.01.2021
 *      Author: mschmidl
 */

#ifndef INCLUDE_EGGLOGIC_H_
#define INCLUDE_EGGLOGIC_H_

#include <stdint.h>

void EGG_initThread( void );
void EGG_startThread( void );

void EGG_sendMessage( char c, uint32_t timeout );


#endif /* INCLUDE_EGGLOGIC_H_ */
