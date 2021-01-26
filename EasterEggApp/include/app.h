/* ----------------------------------------------------------------------------
 * Copyright (c) 2015-2017 Semiconductor Components Industries, LLC (d/b/a
 * ON Semiconductor), All Rights Reserved
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 * This module is derived in part from example code provided by RivieraWaves
 * and as such the underlying code is the property of RivieraWaves [a member
 * of the CEVA, Inc. group of companies], together with additional code which
 * is the property of ON Semiconductor. The code (in whole or any part) may not
 * be redistributed in any form without prior written permission from
 * ON Semiconductor.
 *
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 *
 * This is Reusable Code.
 *
 * ----------------------------------------------------------------------------
 * app.h
 * - Main application header
 * ----------------------------------------------------------------------------
 * $Revision: 1.13 $
 * $Date: 2018/10/10 19:28:56 $
 * ------------------------------------------------------------------------- */

#ifndef APP_H
#define APP_H

/* ----------------------------------------------------------------------------
 * If building with a C++ compiler, make all of the definitions in this header
 * have a C binding.
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif    /* ifdef __cplusplus */

/* ----------------------------------------------------------------------------
 * Include files
 * --------------------------------------------------------------------------*/
#include "RTE_Components.h"
#include CMSIS_device_header
#include <FreeRTOS.h>
#include <task.h>
#include <cmsis_os2.h>

#include <rsl10.h>
#include <rsl10_ke.h>
#include <rsl10_ble.h>
#include <rsl10_profiles.h>
#include <rsl10_map_nvr.h>
#include <stdbool.h>
#include <rsl10_flash_rom.h>
#include <rsl10_protocol.h>

#include "ble_std.h"
#include "ble_diss.h"
#include "ble_bass.h"
#include "ble_sec.h"
#include "ble_hogpd.h"

#include <GPIO_RSLxx.h>



/* ----------------------------------------------------------------------------
* Application Version
* ------------------------------------------------------------------------- */
#define VER_ID                  "EEAPP "
#define VER_MAJOR               0
#define VER_MINOR               0
#define VER_REVISION            4

/* ----------------------------------------------------------------------------
 * Defines
 * --------------------------------------------------------------------------*/
/* Delay cycle for BLE Thread in ms */
#define APP_BLE_DELAY_MS                10

/* Delay cycle for Debug Thread in ms */
#define APP_DEBUG_DELAY_MS              3000

/* Macros */
/* Rounding Macro relaying on integer division of positive integer numbers
 * N and S. It rounds up N to the next interval S. For N=0 result is 0.
 * e.g. ROUND_UP(12,10) => 20  */
#define ROUND_UP(N, S) ((((N) + (S)-1) / (S)) * (S))

/* BLE Thread delay conversion from ms to os ticks with round up to the  portTICK_PERIOD_MS */
#define APP_BLE_DELAY_TICKS             ((uint32_t) \
                                         ((ROUND_UP((APP_BLE_DELAY_MS), portTICK_PERIOD_MS)) / portTICK_PERIOD_MS))

/* Debug Thread delay conversion from ms to os ticks with round up to the  portTICK_PERIOD_MS */
#define APP_DEBUG_DELAY_TICKS           ((uint32_t) \
                                         ((ROUND_UP((APP_DEBUG_DELAY_MS), portTICK_PERIOD_MS)) / portTICK_PERIOD_MS))

/* --------------------------------------------------------------------------
 *  Device Information used for Device Information Server Service (DISS)
 * ----------------------------------------------------------------------- */

/* Manufacturer Name Value */
#define APP_DIS_MANUFACTURER_NAME       ("EasterEgg_Keyboard")
#define APP_DIS_MANUFACTURER_NAME_LEN   (strlen(APP_DIS_MANUFACTURER_NAME) + 1)

/* Model Number String Value */
#define APP_DIS_MODEL_NB_STR            ("RW-BLE-1.0")
#define APP_DIS_MODEL_NB_STR_LEN        (10)

/* Serial Number */
#define APP_DIS_SERIAL_NB_STR           ("1.0.0.0-LE")
#define APP_DIS_SERIAL_NB_STR_LEN       (10)

/* Firmware Revision */
#define APP_DIS_FIRM_REV_STR            ("6.1.2")
#define APP_DIS_FIRM_REV_STR_LEN        (5)

/* System ID Value - LSB -> MSB */
#define APP_DIS_SYSTEM_ID               ("\x12\x34\x56\xFF\xFE\x9A\xBC\xDE")
#define APP_DIS_SYSTEM_ID_LEN           (8)

/* Hardware Revision String */
#define APP_DIS_HARD_REV_STR            ("1.0.0")
#define APP_DIS_HARD_REV_STR_LEN        (5)

/* Software Revision String */
#define APP_DIS_SW_REV_STR              ("0.0.1")
#define APP_DIS_SW_REV_STR_LEN          (5)

/* IEEE */
#define APP_DIS_IEEE                    ("\xFF\xEE\xDD\xCC\xBB\xAA")
#define APP_DIS_IEEE_LEN                (6)

/**
 * PNP ID Value - LSB -> MSB
 *      Vendor ID Source : 0x02 (USB Implementer’s Forum assigned Vendor ID value)
 *      Vendor ID : 0x045E      (Microsoft Corp)
 *      Product ID : 0x0040
 *      Product Version : 0x0300
 */

#define APP_DIS_PNP_ID               ("\x02\x5E\x04\x40\x00\x00\x03")
#define APP_DIS_PNP_ID_LEN           (7)
#define APP_DIS_FEATURES             (DIS_ALL_FEAT_SUP)

/* Minimum and maximum VBAT measurements */
#define VBAT_1p1V_MEASURED              0x1200
#define VBAT_1p4V_MEASURED              0x16cc

/* Maximum battery level */
#define BAT_LVL_MAX                     100

/* DIO numbers we use */
#define RC5_DIO_NUM                     0
#define BUTTON2_DIO                     2
#define BUTTON3_DIO                     3
#define MISO_DIO_NUM                    4
#define BUTTON_DIO                      5
#define LED_DIO_NUM                     6
#define SCLK_DIO_NUM                    7
#define SS_DIO_NUM                      8
#define MOSI_DIO_NUM                    9
#define DEBUG_DIO_NUM                   10
#define RECOVERY_DIO                    12
#define GSCLK_DIO_NUM                   13

/* Output power */
#define OUTPUT_POWER_DBM                0

/* Set timer to 200 ms (20 times the 10 ms kernel timer resolution) */
#define TIMER_200MS_SETTING             20

/* Set timer to 2s (200 times the 10 ms kernel timer resolution) */
#define TIMER_2S_SETTING                200

/* Set timer to 10 s (1000 times the 10 ms kernel timer resolution) */
#define TIMER_10S_SETTING               1000

/* Set timer to 30 s (3000 times the 10 ms kernel timer resolution) */
#define TIMER_30S_SETTING             3000

typedef void (*appm_add_svc_func_t)(void);
#define DEFINE_SERVICE_ADD_FUNCTION(func) (appm_add_svc_func_t)func
#define DEFINE_MESSAGE_HANDLER(message, handler) { message, \
                                                   (ke_msg_func_t)handler }

/* List of message handlers that are used by the different profiles/services */
#define APP_MESSAGE_HANDLER_LIST                       \
    DEFINE_MESSAGE_HANDLER(APP_TEST_TIMER, APP_Timer), \
    DEFINE_MESSAGE_HANDLER(LED_TIMER, LED_Timer)

/* List of functions used to create the database */
#define SERVICE_ADD_FUNCTION_LIST                        \
    DEFINE_SERVICE_ADD_FUNCTION(DeviceInfo_ServiceAdd),  \
    DEFINE_SERVICE_ADD_FUNCTION(Batt_ServiceAdd_Server), \
    DEFINE_SERVICE_ADD_FUNCTION(Hogpd_ServiceAdd)

typedef void (*appm_enable_svc_func_t)(uint8_t);
#define DEFINE_SERVICE_ENABLE_FUNCTION(func) (appm_enable_svc_func_t)func

/* List of functions used to enable client services */
#define SERVICE_ENABLE_FUNCTION_LIST                           \
    DEFINE_SERVICE_ENABLE_FUNCTION(Batt_ServiceEnable_Server), \
    DEFINE_SERVICE_ENABLE_FUNCTION(Hogpd_EnableReq)

#define DEVICE_CSRK                     { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, \
                                          0x07, 0x07, 0x08, 0x09, 0x0a, 0x0b, \
                                          0x0c, 0x0d, 0x0e, 0x0f }
#define DEVICE_IRK                      { 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, \
                                          0x70, 0x70, 0x80, 0x90, 0xa0, 0xb0, \
                                          0xc0, 0xd0, 0xe0, 0xf0 }

#define CONCAT(x, y)                    x##y
#define DIO_SRC(x)                      CONCAT(DIO_SRC_DIO_, x)

#define BOOT_REPORT_IND_NUM             3
#define BOOT_REPORT_IND_FEQ             0.5

/* ----------------------------------------------------------------------------
 * Global variables and types
 * --------------------------------------------------------------------------*/
extern const struct ke_task_desc TASK_DESC_APP;

/* APP Task messages */
enum appm_msg
{
    APPM_DUMMY_MSG = TASK_FIRST_MSG(TASK_ID_APP),

    /* Timer used to have a tick periodically for application */
    APP_TEST_TIMER,
    APP_ADV_REDUCED_POWER,
    APP_ADV_WL_FILTERING,
    LED_TIMER
};

/* Key states */
enum key_state
{
    KEY_IDLE = 0,
    KEY_PUSH,
    KEY_REL,
    KEY_UPDATE
};

typedef enum
{
    EGG_WAIT4_BLE_CONNECT = 0,
	EGG_SEND_URL_PART1,
	EGG_DONE
}eggLogic_state_t;

struct app_env_tag
{
    /* Battery service */
    uint8_t batt_lvl;
    uint32_t sum_batt_lvl;
    uint16_t num_batt_read;
    bool send_batt_ntf[NUM_MASTERS];

    /* HID service */
    uint8_t key_state;
    bool key_pushed;

    /* business logic */
    eggLogic_state_t eggState;
};

struct on_semi_banner_str
{
    uint8_t key;
    uint8_t mod;
};

/* Support for the application manager and the application environment */
extern struct app_env_tag app_env;

/* List of functions used to create the database */
extern const appm_add_svc_func_t appm_add_svc_func_list[];

extern DRIVER_GPIO_t Driver_GPIO;
extern DRIVER_GPIO_t *gpio;

/* ---------------------------------------------------------------------------
* Function prototype definitions
* --------------------------------------------------------------------------*/
extern void App_Initialize(void);

extern void App_Env_Initialize(void);

extern void APP_eggLogicStateMachine( void );

extern int APP_Timer(ke_msg_id_t const msg_id, void const *param,
                     ke_task_id_t const dest_id,
                     ke_task_id_t const src_id);

extern int LED_Timer(ke_msg_id_t const msg_id, void const *param,
                     ke_task_id_t const dest_id,
                     ke_task_id_t const src_id);

extern void Operation_Cancel(void);

extern int Msg_Handler(ke_msg_id_t const msgid, void *param,
                       ke_task_id_t const dest_id,
                       ke_task_id_t const src_id);

extern void Restart_Keystroke_Env(void);

extern void Update_Keystroke_Env(void);

void GPIOirq_EventCallback(uint32_t event);

__NO_RETURN void vThread_BLE(void *argument);


/* ----------------------------------------------------------------------------
 * Close the 'extern "C"' block
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif    /* ifdef __cplusplus */

#endif    /* APP_H */
