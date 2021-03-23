/*
 * EggLogic.h
 *
 *  Created on: 26.01.2021
 *      Author: mschmidl
 */

#ifndef INCLUDE_EGGLOGIC_H_
#define INCLUDE_EGGLOGIC_H_

#include <stdint.h>

void EggLogic_init( void );
void EggLogic_timerTick( uint32_t ms );
void EggLogic_updateGyroAndTouchInfo( void );

void EGG_doneWithSendingKeyStroke( void );
void EggLogic_RC5match( void );



#endif /* INCLUDE_EGGLOGIC_H_ */
