/* ----------------------------------------------------------------------------
 * Copyright (c) 2015-2017 Semiconductor Components Industries, LLC (d/b/a
 * ON Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 *
 * This is Reusable Code.
 *
 * ----------------------------------------------------------------------------
 * app.c
 * - Main application file
 * ----------------------------------------------------------------------------
 * $Revision: 1.13 $
 * $Date: 2018/10/10 19:28:55 $
 * ------------------------------------------------------------------------- */

#include "app.h"
#include "sys_fota.h"
#include "RC5receiver.h"
#include "EggLogic.h"

DRIVER_GPIO_t *gpio;

// Prototype for HardFault Handler defined in STARTUP assembly code
void HardFault_Handler(void);

/* ----------------------------------------------------------------------------
 * Application Version
 * ------------------------------------------------------------------------- */
SYS_FOTA_VERSION(VER_ID, VER_MAJOR, VER_MINOR, VER_REVISION);


/* ----------------------------------------------------------------------------
 * Function      : void GPIOirq_EventCallback(void)
 * ----------------------------------------------------------------------------
 * Description   : This function is a callback registered by the function
 *                 Initialize. Based on event argument different actions are
 *                 executed.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void GPIOirq_EventCallback(uint32_t event)
{
    switch ( event )
    {
    	case GPIO_EVENT_0_IRQ:
    		// da TOUCH und GYRO am gleichen Bus hängen, könnte es zu Konflikten kommen
    		// daher pollen wir z.Z. in der YAKINDU Tick Funktion beide alle 100ms
    	{
    	} break;
    	case GPIO_EVENT_1_IRQ:
    		RC5_HandleSignalChange();
		    break;
    	default:
    		break;
    }
}


void HardFault_Handler(void)
{
	static volatile uint32_t _Continue;

	_Continue = 0u;
	while ( 0 == _Continue );
}


int main(void)
{
	// drive the POWER_ON signal as soon as possible, so the REED contact can open again
    DIO->CFG[POWER_ON_DIO] = DIO_MODE_GPIO_OUT_1;

    App_Initialize();

    SystemCoreClockUpdate();

    /* Start Update when button is pressed at startup */
    DIO->CFG[RECOVERY_FOTA_DEBUG_DIO] = DIO_MODE_INPUT  | DIO_WEAK_PULL_UP | DIO_LPF_DISABLE | DIO_6X_DRIVE;
    if (DIO_DATA->ALIAS[RECOVERY_FOTA_DEBUG_DIO] == 0)
    {
        /* Erase the bond list from NVR2 flash */
        BondList_RemoveAll();
        // start software update
        Sys_Fota_StartDfu(1);
    }
    DIO->CFG[RECOVERY_FOTA_DEBUG_DIO] = DIO_MODE_GPIO_OUT_0;

    /* Debug/trace initialization. In order to enable UART or RTT trace,
     *  configure the 'RSL10_DEBUG' macro in app_trace.h */
    TRACE_INIT();
    PRINTF("**********************************************************\r\n");
    PRINTF("EasterEggApp started (build %s %s)\r\n", __DATE__, __TIME__);
//    PRINTF("SystemCoreClock = %ldHz\r\n", SystemCoreClock);

	EggLogic_init();

    /* Event Kernel Scheduler running in thread */
    while(true)
    {
        /* Dispatch all events in Kernel queue */
        Kernel_Schedule();

        /* Send battery level to all connected clients if battery service is
         * enabled */
        for (unsigned int i = 0; i < NUM_MASTERS; i++)
        {
            if (ble_env[i].state == APPM_CONNECTED &&
                VALID_BOND_INFO(ble_env[i].bond_info.STATE))
            {
                if (app_env.send_batt_ntf[i] && bass_support_env[i].enable)
                {
                    app_env.send_batt_ntf[i] = 0;
                    Batt_LevelUpdateSend(ble_env[i].conidx,
                                         app_env.batt_lvl, 0);
                }
            }
        }

        /* Refresh the watchdog timer */
        Sys_Watchdog_Refresh();

        /* Wait for an event before executing the scheduler again */
        SYS_WAIT_FOR_EVENT;
    }
}
