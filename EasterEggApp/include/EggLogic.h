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

typedef enum
{
	EGG_NOP = 0,
	EGG_TIMER_TICK,
	EGG_TOUCH_IRQ,
	EGG_TOUCH1_EVENT,
	EGG_TOUCH2_EVENT,
	EGG_TOUCH3_EVENT,
	EGG_DONE_WITH_URL,
	EGG_RC5_MATCH
}eggLogicMessage_t;
void EGG_sendMessage( eggLogicMessage_t msg, uint32_t timeout );


#endif /* INCLUDE_EGGLOGIC_H_ */
