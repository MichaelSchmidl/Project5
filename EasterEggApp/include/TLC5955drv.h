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

void TLC5955drv_init( void );
int32_t TLC5955drv_refresh( void );
void TLC5955drv_start( void );
void TLC5955drv_stop( void );

/* ----------------------------------------------------------------------------
 * Close the 'extern "C"' block
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif    /* ifdef __cplusplus */

#endif /* COMPONENTS_TCL5955DRV_H_ */
