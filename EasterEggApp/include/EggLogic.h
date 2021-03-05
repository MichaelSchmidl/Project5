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

typedef enum
{
	EGGLOGIC_MESSAGE_NOP = 0,
	EGGLOGIC_MESSAGE_TIMER_TICK,
	EGGLOGIC_MESSAGE_TOUCH_IRQ,
	EGGLOGIC_MESSAGE_TOUCH1_EVENT,
	EGGLOGIC_MESSAGE_TOUCH2_EVENT,
	EGGLOGIC_MESSAGE_TOUCH3_EVENT,
	EGGLOGIC_MESSAGE_RC5_MATCH
}eggLogicMessage_t;

//void EGG_sendMessage( eggLogicMessage_t msg, uint32_t timeout );
void EGG_doneWithSendingKeyStroke( void );


#endif /* INCLUDE_EGGLOGIC_H_ */
