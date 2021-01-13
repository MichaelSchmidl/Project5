/* ----------------------------------------------------------------------------
 * Copyright (c) 2015-2018 Semiconductor Components Industries, LLC (d/b/a
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
 * ble_bass.h
 * - Bluetooth battery service header
 * ------------------------------------------------------------------------- */

#ifndef BLE_BASS_H
#define BLE_BASS_H

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
#include <bass_task.h>
#include <ke_timer.h>
/* ----------------------------------------------------------------------------
 * Defines
 * --------------------------------------------------------------------------*/

/* ----------------------------------------------------------------------------
 * Global variables and types
 * --------------------------------------------------------------------------*/

typedef struct
{
    /* Number of battery instances [1,BASS_NB_BAS_INSTANCES_MAX] */
    uint8_t bas_nb;

    /* The current value of CCCD of battery value that has been set by
     * the client device */
    uint8_t batt_ntf_cfg[BLE_CONNECTION_MAX];

    /* The flag that indicates that service has been enabled */
    bool enabled[BLE_CONNECTION_MAX];

    /* The flag that indicates that service has been added */
    bool serviceAdded;

    uint32_t battLevelNotificationTimeout;
    bool battLevelNotificationTimerEnqueued;

    uint32_t battLevelMonitoringTimeout;
    bool battLevelMonitoringTimerEnqueued;

    uint8_t lastBattLevel[BASS_NB_BAS_INSTANCES_MAX];

    /* Pointer to an application function that returns the battery level */
    uint8_t (*readBattLevelCallback)(uint8_t bas_nb);
} BASS_Env_t;

enum bass_app_msg_id
{
    BASS_BATT_LEVEL_NTF_TIMEOUT = TASK_FIRST_MSG(TASK_ID_BASS) + 50,
    BASS_BATT_MONITORING_TIMEOUT,
    BASS_BATT_LEVEL_CHANGED
};

/* ----------------------------------------------------------------------------
 * Function prototype definitions
 * --------------------------------------------------------------------------*/
void BASS_Initialize(uint8_t bas_nb,
                     uint8_t (*readBattLevelCallback)(uint8_t bas_nb));

void BASS_NotifyOnTimeout(uint32_t timeout);

void BASS_NotifyOnBattLevelChange(uint32_t timeout);

void BASS_ProfileTaskAddCmd(void);

void BASS_EnableReq(uint8_t conidx);

void BASS_BattLevelUpdReq(uint8_t batt_lvl, uint8_t bas_nb);

const BASS_Env_t* BASS_GetEnv(void);

bool BASS_IsAdded(void);

void BASS_MsgHandler(ke_msg_id_t const msg_id, void const *param,
                     ke_task_id_t const dest_id, ke_task_id_t const src_id);

/* ----------------------------------------------------------------------------
 * Close the 'extern "C"' block
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif    /* ifdef __cplusplus */

#endif    /* BLE_BASS_H */
