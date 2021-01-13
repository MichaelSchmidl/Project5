/* ----------------------------------------------------------------------------
 * Copyright (c) 2018 Semiconductor Components Industries, LLC (d/b/a
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
 * ble_diss.h
 * - Bluetooth device information service header
 * ------------------------------------------------------------------------- */

#ifndef BLE_DISS_H
#define BLE_DISS_H

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
#include <diss_task.h>
#include <ke_timer.h>

/* ----------------------------------------------------------------------------
 * Defines
 * --------------------------------------------------------------------------*/

/* ----------------------------------------------------------------------------
 * Global variables and types
 * --------------------------------------------------------------------------*/

/* Global DISS Specific Info Structure */
struct DISS_DeviceInfoField_t
{
    uint8_t len;
    uint8_t *data;
};


/* Global DISS Info Compilation Structure */
struct DISS_DeviceInfo_t
{
    struct DISS_DeviceInfoField_t MANUFACTURER_NAME;
    struct DISS_DeviceInfoField_t MODEL_NB_STR;
    struct DISS_DeviceInfoField_t SERIAL_NB_STR;
    struct DISS_DeviceInfoField_t FIRM_REV_STR;
    struct DISS_DeviceInfoField_t SYSTEM_ID;
    struct DISS_DeviceInfoField_t HARD_REV_STR;
    struct DISS_DeviceInfoField_t SW_REV_STR;
    struct DISS_DeviceInfoField_t IEEE;
    struct DISS_DeviceInfoField_t PNP;
};

/**
 * DISS state structure
 */
typedef struct
{
    /** DISS features that will be enabled. See \ref diss_task.h */
    uint16_t features;
    /** The flag that indicates that service has been added */
    bool serviceAdded;
    /** DISS Device Information compilation structure arm*/
    const struct DISS_DeviceInfo_t *deviceInfo;
} DISS_Env_t;

/* ----------------------------------------------------------------------------
 * Function prototype definitions
 * --------------------------------------------------------------------------*/
int DISS_Initialize( uint16_t features, const struct DISS_DeviceInfo_t *deviceInfo );

void DISS_ProfileTaskAddCmd( void );

const DISS_Env_t* DISS_EnvGet( void );

bool DISS_IsAdded(void);

void DISS_MsgHandler( ke_msg_id_t const msg_id, void const *param,
        ke_task_id_t const dest_id, ke_task_id_t const src_id );

void DISS_DeviceInfoValueReqInd( ke_msg_id_t const msgid, void const * param,
        ke_task_id_t const dest_id, ke_task_id_t const src_id );

/* ----------------------------------------------------------------------------
 * Close the 'extern "C"' block
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif    /* ifdef __cplusplus */

#endif    /* BLE_BASS_H */
