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

#include <rsl10.h>

void TLC5955drv_init( void );
void SPI0_Master_CallBack(uint32_t event);

/* ----------------------------------------------------------------------------
 * Close the 'extern "C"' block
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif    /* ifdef __cplusplus */

#endif /* COMPONENTS_TCL5955DRV_H_ */
