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
#include "TLC5955drv.h"

DRIVER_GPIO_t *gpio;

/* attribute structure for BLE thread */
const osThreadAttr_t thread_ble_attr =
{
    .name = "thBLE",
    .priority = osPriorityNormal,
    .stack_size = 2048
};

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
		    /* Set the key status */
		    app_env.key_pushed = true;
		    app_env.key_state = KEY_PUSH;
		    break;
    	case GPIO_EVENT_1_IRQ:
    		RC5_SignalChangeDetected();
		    break;
    	default:
    		break;
    }
}


int main(void)
{
    /* Ensure all priority bits are assigned as preemption priority bits.
     * Should not be changed! */
    NVIC_SetPriorityGrouping(0);

    App_Initialize();

    SystemCoreClockUpdate();

    TLC5955drv_start();
#if 0
    /* Debug/trace initialization. In order to enable UART or RTT trace,
     *  configure the 'RSL10_DEBUG' macro in app_trace.h */
//    TRACE_INIT();
//    PRINTF("RTOS + ble_peripheral_server_bond started (build date:%s,%s)\r\n", \
           __DATE__, __TIME__);
//    PRINTF("SystemCoreClock = %ldHz\r\n", SystemCoreClock);

    /* RTOS  initialization */
    osKernelInitialize();
//    PRINTF("RTOS kernel tick frequency = %ldHz\r\n", osKernelGetTickFreq());
//    PRINTF("RTOS kernel system timer frequency = %ldHz\r\n", osKernelGetSysTimerFreq());

    /* Ensure that Priority Grouping was not changed during device initialization.
     * Call it after logs are initialized. */
    configASSERT(NVIC_GetPriorityGrouping() == 0);

    /* Create application main thread for BLE Stack */
    osThreadNew(vThread_BLE, NULL, &thread_ble_attr);

    /* Start RTOS scheduler */
    if (osKernelGetState() == osKernelReady)
    {
        osKernelStart();
    }
#endif
    /* Main application loop:
     * - Run the kernel scheduler
     * - Update the battery voltage
     * - Refresh the watchdog and wait for an event before continuing
     * - Check for the custom services
     */
    while (1)
    {
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


        /* Start Update when button is pressed */
        if (DIO_DATA->ALIAS[BUTTON2_DIO] == 0)
        {
        	Sys_Fota_StartDfu(1);
        }

        /* Refresh the watchdog timer */
        Sys_Watchdog_Refresh();

        /* Wait for an event before executing the scheduler again */
        Sys_GPIO_Set_Low(DEBUG_DIO_NUM);
        SYS_WAIT_FOR_EVENT;
        Sys_GPIO_Set_High(DEBUG_DIO_NUM);
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
#if 0
    /* Run the following command when erasing flash/bond_list is desirable */
    /* BondList_RemoveAll(); */

    /* Configure application-specific advertising data and scan response  data*/
    APP_SetAdvScanData();

    /* Configure Battery Service Server */
    BASS_Initialize(APP_BAS_NB, APP_BASS_ReadBatteryLevel);
    BASS_NotifyOnBattLevelChange(TIMER_SETTING_S(1));     /* Periodically monitor the battery level. Only notify changes */
    BASS_NotifyOnTimeout(TIMER_SETTING_S(6));             /* Periodically notify the battery level to connected peers */
    APP_BASS_SetBatMonAlarm(BATMON_SUPPLY_THRESHOLD_CFG); /* BATMON alarm configuration */

    /* Configure Custom Service Server */
    CUSTOMSS_Initialize();
    CUSTOMSS_NotifyOnTimeout(TIMER_SETTING_S(6)); /* Notify client and fire CUSTOMSS_NTF_TIMEOUT periodically */

    /* Add application message handlers */
    MsgHandler_Add(TASK_ID_GAPM, APP_GAPM_GATTM_Handler);
    MsgHandler_Add(GATTM_ADD_SVC_RSP, APP_GAPM_GATTM_Handler);
    MsgHandler_Add(TASK_ID_GAPC, APP_GAPC_Handler);
    MsgHandler_Add(APP_LED_TIMEOUT, APP_LED_Timeout_Handler);
    MsgHandler_Add(APP_BATT_LEVEL_LOW, APP_BASS_BattLevelLow_Handler);

    /* Reset the GAP manager. Trigger GAPM_CMP_EVT / GAPM_RESET when finished. See APP_GAPM_GATTM_Handler */
    GAPM_ResetCmd();
#endif
    /* Event Kernel Scheduler runing in thread */
    while(true)
    {
        /* Dispatch all events in Kernel queue */
        Kernel_Schedule();

        /* Refresh the watchdog timer */
        Sys_Watchdog_Refresh();

        /* OS delay */
        osDelay(APP_BLE_DELAY_TICKS);
    }
}

