/*
 * RC5receiver.c
 *
 *  Created on: 19.01.2021
 *      Author: mschmidl
 */

#include "RC5receiver.h"
#include "app.h"
#include "EggLogic.h"

#warning RC5_EXPECTED_CODE is for LOEWE PLAY so far
#define RC5_EXPECTED_CODE 0x31B5 //!< TBD

#define RC5_MASK          0xF7FF

#define MAX_RC5_SAMPLES (3 + (13*4))
static uint8_t RC5_sampleCounter = 0;
static uint32_t RC5_val = 0;

#undef RC5_VERBOSE_DEBUG_SIGNAL
#define DEBUG_DIO_NUM RECOVERY_FOTA_DEBUG_DIO

static void RC5_sampleFunc( void )
{
    if ( 1 == (RC5_sampleCounter % 4) )
    {
    	RC5_val <<= 1;
#ifdef RC5_VERBOSE_DEBUG_SIGNAL
        Sys_GPIO_Set_Low(DEBUG_DIO_NUM);
        Sys_GPIO_Set_High(DEBUG_DIO_NUM);
        Sys_GPIO_Set_Low(DEBUG_DIO_NUM);
#endif
        if ( 0 == DIO_DATA->ALIAS[RC5_DIO_NUM] )
        {
#ifdef RC5_VERBOSE_DEBUG_SIGNAL
            Sys_GPIO_Set_High(DEBUG_DIO_NUM);
#endif
            RC5_val |= 1;
        }
    }
    if ( MAX_RC5_SAMPLES < RC5_sampleCounter )
    {
    	// we have now our RC5 data
    	if ( RC5_EXPECTED_CODE == (RC5_val & RC5_MASK) )
    	{
//!TODO        	EGG_sendMessage( EGGLOGIC_MESSAGE_RC5_MATCH, // we are done with RC5
//        			         0 );           // timeout=0 because IRQ context
    	}

    	// restart sampling
		Sys_Timers_Stop( SELECT_TIMER0 );
		RC5_sampleCounter = 0;
		RC5_val = 0;
#ifdef RC5_VERBOSE_DEBUG_SIGNAL
        Sys_GPIO_Set_Low(DEBUG_DIO_NUM);
        Sys_GPIO_Set_High(DEBUG_DIO_NUM);
        Sys_GPIO_Set_Low(DEBUG_DIO_NUM);
#endif
    }
    else
    {
        RC5_sampleCounter++;
    }
}


void RC5_HandleSignalChange( void )
{
#if 1
	if ( 0 == RC5_sampleCounter )
	{
		Sys_Timers_Start( SELECT_TIMER0 );
		RC5_sampleFunc();
	}
#else
	Sys_Timers_Stop( SELECT_TIMER0 );
	RC5_sampleFunc();
	Sys_Timers_Start( SELECT_TIMER0 );
#endif
}

/* ----------------------------------------------------------------------------
 * Function      : void TIMER0_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : timer0 interrupt configured in free run mode
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void TIMER0_IRQHandler(void)
{
	RC5_sampleFunc();
}


void RC5_init( void)
{
    /* Configure timer[0] in free run mode.
     * - Force multi-count to 8 in order to show that field is not active */
    Sys_Timer_Set_Control(0, (TIMER_MULTI_COUNT_8 |
                              TIMER_FREE_RUN      |
                              TIMER_SLOWCLK_DIV2  |
                              TIMER_PRESCALE_1)   | 222); // at little bit slower then 445us

    NVIC_EnableIRQ(TIMER0_IRQn);
}
