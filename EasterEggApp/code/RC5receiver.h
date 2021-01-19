/*
 * RC5receiver.h
 *
 *  Created on: 19.01.2021
 *      Author: mschmidl
 */

#ifndef COMPONENTS_RC5RECEIVER_H_
#define COMPONENTS_RC5RECEIVER_H_

/* ----------------------------------------------------------------------------
 * If building with a C++ compiler, make all of the definitions in this header
 * have a C binding.
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif    /* ifdef __cplusplus */

#include <rsl10.h>

void RC5_init( void);
void RC5_SignalChangeDetected( void );
void TIMER0_IRQHandler(void);


/* ----------------------------------------------------------------------------
 * Close the 'extern "C"' block
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif    /* ifdef __cplusplus */

#endif /* COMPONENTS_RC5RECEIVER_H_ */
