/*
 * TCL5955drv.h
 *
 *  Created on: 19.01.2021
 *      Author: mschmidl
 */

#ifndef COMPONENTS_TCL5955DRV_H_
#define COMPONENTS_TCL5955DRV_H_

/* ----------------------------------------------------------------------------
 * If building with a C++ compiler, make all of the definitions in this header
 * have a C binding.
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif    /* ifdef __cplusplus */

#include <stdint.h>
#include <rsl10.h>

typedef enum
{
    tlcPortR0 = 0,
    tlcPortG0,
    tlcPortB0,
    tlcPortR1,
    tlcPortG1,
    tlcPortB1,
    tlcPortR2,
    tlcPortG2,
    tlcPortB2,
    tlcPortR3,
    tlcPortG3,
    tlcPortB3,
    tlcPortR4,
    tlcPortG4,
    tlcPortB4,
    tlcPortR5,
    tlcPortG5,
    tlcPortB5,
    tlcPortR6,
    tlcPortG6,
    tlcPortB6,
    tlcPortR7,
    tlcPortG7,
    tlcPortB7,
    tlcPortR8,
    tlcPortG8,
    tlcPortB8,
    tlcPortR9,
    tlcPortG9,
    tlcPortB9,
    tlcPortR10,
    tlcPortG10,
    tlcPortB10,
    tlcPortR11,
    tlcPortG11,
    tlcPortB11,
    tlcPortR12,
    tlcPortG12,
    tlcPortB12,
    tlcPortR13,
    tlcPortG13,
    tlcPortB13,
    tlcPortR14,
    tlcPortG14,
    tlcPortB14,
    tlcPortR15,
    tlcPortG15,
    tlcPortB15,

    tlcPortNum,
} tlcPort;

void TLC5955drv_init( void );
int32_t TLC5955drv_refresh( void );
void TLC5955drv_start( void );
void TLC5955drv_stop( void );
void TLC5955drv_setChannelBrightness( tlcPort channelNumber, uint16_t value );

/* ----------------------------------------------------------------------------
 * Close the 'extern "C"' block
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif    /* ifdef __cplusplus */

#endif /* COMPONENTS_TCL5955DRV_H_ */
