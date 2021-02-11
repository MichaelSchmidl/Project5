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

/* attribute structure for BLE thread */
const osThreadAttr_t thread_ble_attr =
{
    .name = "thBLE",
    .priority = osPriorityNormal,
    .stack_size = 3*1024
};
__NO_RETURN void vThread_BLE(void *argument);

volatile uint32_t malloc_failed_count = 0;

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
    		EGG_sendMessage( EGGLOGIC_MESSAGE_TOUCH_IRQ,
    				         0UL ); // timeout=0 because IRQ context
		    break;
    	case GPIO_EVENT_1_IRQ:
    		RC5_HandleSignalChange();
		    break;
    	default:
    		break;
    }
}


int main(void)
{
	// drive the POWER_ON signal as soon as possible, so the REED contact can open again
    DIO->CFG[POWER_ON_DIO] = DIO_MODE_GPIO_OUT_1;

    /* Ensure all priority bits are assigned as preemption priority bits.
     * Should not be changed! */
    NVIC_SetPriorityGrouping(0);

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
    PRINTF("**********************************************************\n");
    PRINTF("EasterEggApp started (build %s %s)\n", __DATE__, __TIME__);
//    PRINTF("SystemCoreClock = %ldHz\r\n", SystemCoreClock);

    /* RTOS  initialization */
    osKernelInitialize();
//    PRINTF("RTOS kernel tick frequency = %ldHz\n", osKernelGetTickFreq());
//    PRINTF("RTOS kernel system timer frequency = %ldHz\n", osKernelGetSysTimerFreq());

    /* Ensure that Priority Grouping was not changed during device initialization.
     * Call it after logs are initialized. */
    configASSERT(NVIC_GetPriorityGrouping() == 0);

    // initialize threads but do not start them so far...
    EGG_initThread();

    /* Create application main thread for BLE Stack */
    osThreadNew(vThread_BLE, NULL, &thread_ble_attr);

    /* start thread for egg logic */
    EGG_startThread();

    /* Start RTOS scheduler */
    PRINTF("starting kernel...\n");
    if (osKernelGetState() == osKernelReady)
    {
        osKernelStart();
    }
}


/* ----------------------------------------------------------------------------
 * Function      : void vThread_BLE(void *argument)
 * ----------------------------------------------------------------------------
 * Description   : Main BLE Thread function.
 * Inputs        : - argument       - pointer to Thread arguments
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
__NO_RETURN void vThread_BLE(void *argument)
{
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

#if 0
        /* Wait for an event before executing the scheduler again */
        Sys_GPIO_Set_Low(DEBUG_DIO_NUM);
        SYS_WAIT_FOR_EVENT;
        Sys_GPIO_Set_High(DEBUG_DIO_NUM);
#else
        /* OS delay */
        osDelay(APP_BLE_DELAY_TICKS);
#endif
    }
}

