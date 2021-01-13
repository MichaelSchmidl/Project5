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
 *  This is Reusable Code.
 *
 * ----------------------------------------------------------------------------
 * ble_bass.c
 * - Bluetooth battery service server functions
 * ------------------------------------------------------------------------- */
#include <rsl10.h>
#include <ble_bass.h>
#include <ble_gap.h>
#include <msg_handler.h>
#include <prf_utils.h>
#include <bass.h>
/* Global variable definition */
static BASS_Env_t bass_env;

/* ----------------------------------------------------------------------------
 * Function      : void BASS_Initialize(uint8_t bas_nb,
                              uint8_t (*readBattLevelCallback)(uint8_t bas_nb))
 * ----------------------------------------------------------------------------
 * Description   : Initialize battery service server service environment and
 *                 configure message handlers
 * Inputs        : bas_nb                 - Number of battery instances [1,2]
 *                 readBattLevelCallback  - Pointer to user application
 *                     callback function that returns the battery value
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void BASS_Initialize(uint8_t bas_nb,
                     uint8_t (*readBattLevelCallback)(uint8_t bas_nb))
{
    memset(&bass_env, 0, sizeof(BASS_Env_t));

    bass_env.readBattLevelCallback = readBattLevelCallback;
    bass_env.bas_nb = bas_nb;

    /* Add BASS message handlers */
    MsgHandler_Add(TASK_ID_BASS,            BASS_MsgHandler);
    MsgHandler_Add(GAPM_CMP_EVT,            BASS_MsgHandler);
    MsgHandler_Add(GAPC_CONNECTION_REQ_IND, BASS_MsgHandler);
    MsgHandler_Add(GAPC_DISCONNECT_IND,     BASS_MsgHandler);
    MsgHandler_Add(GAPM_PROFILE_ADDED_IND,  BASS_MsgHandler);
}

/* ----------------------------------------------------------------------------
 * Function      : void BASS_NotifyOnTimeout(uint32_t timeout)
 * ----------------------------------------------------------------------------
 * Description   : Configure battery service to send periodic notifications.
 * Inputs        : timeout       - in units of 10ms. Disabled if timeout == 0.
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void BASS_NotifyOnTimeout(uint32_t timeout)
{
    bass_env.battLevelNotificationTimeout = timeout;

    if(timeout && GAPC_GetConnectionCount() > 0)
    {
        if(!bass_env.battLevelNotificationTimerEnqueued)
        {
            ke_timer_set(BASS_BATT_LEVEL_NTF_TIMEOUT, TASK_APP, timeout);
            bass_env.battLevelNotificationTimerEnqueued = true;
        }
    }
    else /* timeout == 0 or no peers are connected. Stop timer. */
    {
        ke_timer_clear(BASS_BATT_LEVEL_NTF_TIMEOUT, TASK_APP);
        bass_env.battLevelNotificationTimerEnqueued = false;
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void BASS_NotifyOnBattLevelChange(uint32_t timeout)
 * ----------------------------------------------------------------------------
 * Description   : Configure battery service to periodically monitor the
 *                 battery level according to the timeout. The battery level is
 *                 queried read through the callback function passed as
 *                 argument to BASS_Initialize(). Connected peers are only
 *                 notified if the battery level changes. It triggers a
 *                 BASS_BATT_LEVEL_CHANGED event, which the user application
 *                 may subscribe to.
 * Inputs        : timeout       - in units of 10ms. Disabled if timeout == 0.
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void BASS_NotifyOnBattLevelChange(uint32_t timeout)
{
    bass_env.battLevelMonitoringTimeout = timeout;

    if(timeout && GAPC_GetConnectionCount() > 0)
    {
        if(!bass_env.battLevelMonitoringTimerEnqueued)
        {
            ke_timer_set(BASS_BATT_MONITORING_TIMEOUT, TASK_APP, timeout);
            bass_env.battLevelMonitoringTimerEnqueued = true;
        }
    }
    else /* timeout == 0 or no peers are connected. Stop timer. */
    {
        ke_timer_clear(BASS_BATT_MONITORING_TIMEOUT, TASK_APP);
        bass_env.battLevelMonitoringTimerEnqueued = false;
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void BASS_ProfileTaskAddCmd(void)
 * ----------------------------------------------------------------------------
 * Description   : Send request to add battery profile in kernel and DB
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void BASS_ProfileTaskAddCmd(void)
{
    struct bass_db_cfg db_cfg;

    /* Configure BASS database parameters  */
    db_cfg.bas_nb = bass_env.bas_nb;
    for(uint8_t i = 0; i < bass_env.bas_nb; i++)
    {
        db_cfg.features[i] = BAS_BATT_LVL_NTF_SUP;
        db_cfg.batt_level_pres_format[i].description = 0;
        db_cfg.batt_level_pres_format[i].exponent = 0;
        db_cfg.batt_level_pres_format[i].format = 0x4;
        db_cfg.batt_level_pres_format[i].name_space = 1;
        db_cfg.batt_level_pres_format[i].unit = ATT_UNIT_PERCENTAGE;
    }

    /* Add BASS profile task to the stack. A GAPM_PROFILE_ADDED_IND event
     * is generated when finished. */
    GAPM_ProfileTaskAddCmd(PERM(SVC_AUTH, DISABLE), TASK_ID_BASS, TASK_APP,
                           0, (uint8_t*)&db_cfg, sizeof(struct bass_db_cfg));
}

/* ----------------------------------------------------------------------------
 * Function      : void BASS_EnableReq(uint8_t conidx)
 * ----------------------------------------------------------------------------
 * Description   : Send request for enabling battery service to battery server
 * Inputs        : conidx    - connection index
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void BASS_EnableReq(uint8_t conidx)
{
    struct bass_enable_req *req;

    /* Allocate the message to enable battery service server */
    req = KE_MSG_ALLOC(BASS_ENABLE_REQ,
                       prf_src_task_get(&(PRF_ENV_GET(BASS, bass)->prf_env),
                                        conidx), TASK_APP, bass_enable_req);

    /* Fill in the parameter structure, initializing the old battery levels
     * to 0 as there are no previous measurements */
    req->conidx = conidx;
    req->old_batt_lvl[0] = 0;
    req->old_batt_lvl[1] = 0;

    /* Enable notification for this client by default */
    req->ntf_cfg = 0x3;
    bass_env.batt_ntf_cfg[conidx] = 0x3;

    ke_msg_send(req);
}

/* ----------------------------------------------------------------------------
 * Function      : void BASS_BattLevelUpdReq(uint8_t batt_level, uint8_t bas_nb)
 * ----------------------------------------------------------------------------
 * Description   : Send request for sending battery level notification
 * Inputs        : - batt_level - battery level [0, 100]
 *               : - bas_nb     - battery server instance number
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void BASS_BattLevelUpdReq(uint8_t batt_level, uint8_t bas_nb)
{
    struct bass_batt_level_upd_req *req;

    bass_env.lastBattLevel[bas_nb] = batt_level;

    /* Allocate the battery level update request message */
    req = KE_MSG_ALLOC(BASS_BATT_LEVEL_UPD_REQ,
                       prf_src_task_get(&(PRF_ENV_GET(BASS, bass)->prf_env), 0),
                       TASK_APP, bass_batt_level_upd_req);

    /* Fill in the parameter structure */
    req->bas_instance = bas_nb;
    req->batt_level = batt_level;

    ke_msg_send(req);
}

/* ----------------------------------------------------------------------------
 * Function      : void BASS_GetEnv(void)
 * ----------------------------------------------------------------------------
 * Description   : Return a reference to the internal environment structure.
 * Inputs        : None
 * Outputs       : A constant pointer to BASS_Env_t
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
const BASS_Env_t* BASS_GetEnv(void)
{
    return &bass_env;
}

/* ----------------------------------------------------------------------------
 * Function      : bool BASS_IsAdded(void)
 * ----------------------------------------------------------------------------
 * Description   : Return a boolean indication if the BAS server has been
 *                  successfully added to the services database.
 * Inputs        : None
 * Outputs       : Return value: true if the service has been successfully added, 
 * 					false if not
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
bool BASS_IsAdded(void)
{
    return bass_env.serviceAdded;
}

/* ----------------------------------------------------------------------------
 * Function      : void BASS_MsgHandler(ke_msg_id_t const msg_id,
 *                                      void const *param,
 *                                      ke_task_id_t const dest_id,
 *                                      ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Handle all events related to the battery service server
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameter
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void BASS_MsgHandler(ke_msg_id_t const msg_id, void const *param,
                     ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    uint8_t conidx = KE_IDX_GET(src_id);
    switch(msg_id)
    {
        case GAPM_CMP_EVT:
        {
            if(((struct gapm_cmp_evt*)param)->operation == GAPM_SET_DEV_CONFIG)
            {
                BASS_ProfileTaskAddCmd();
            }
        }
        break;

        case GAPM_PROFILE_ADDED_IND:
        {
            struct gapm_profile_added_ind *ind = (struct gapm_profile_added_ind*) param;
            if( ind->prf_task_id == TASK_ID_BASS )
            {
                bass_env.serviceAdded = true;
            }
        }
        break;

        case GAPC_CONNECTION_REQ_IND:
        {
            BASS_EnableReq(conidx);
        }
        break;

        case GAPC_DISCONNECT_IND:
        {
            bass_env.enabled[conidx] = false;
            bass_env.batt_ntf_cfg[conidx] = PRF_CLI_STOP_NTFIND;

            /* Stop timers, if needed */
            BASS_NotifyOnTimeout(bass_env.battLevelNotificationTimeout);
            BASS_NotifyOnBattLevelChange(bass_env.battLevelMonitoringTimeout);
        }
        break;

        case BASS_ENABLE_RSP:
        {
            if (((struct bass_enable_rsp*)param)->status == GAP_ERR_NO_ERROR)
            {
                bass_env.enabled[conidx] = true;

                for(uint8_t i = 0; i < bass_env.bas_nb; i++)
                {
                    BASS_BattLevelUpdReq(bass_env.readBattLevelCallback(i), i); /* Notify */
                }

                /* Start timers, if needed */
                BASS_NotifyOnTimeout(bass_env.battLevelNotificationTimeout);
                BASS_NotifyOnBattLevelChange(bass_env.battLevelMonitoringTimeout);
            }
        }
        break;

        case BASS_BATT_LEVEL_NTF_CFG_IND:
        {
            /* Store the new notification configuration */
            bass_env.batt_ntf_cfg[conidx] =
                    ((struct bass_batt_level_ntf_cfg_ind*)param)->ntf_cfg;
        }
        break;

        case BASS_BATT_LEVEL_NTF_TIMEOUT:
        {
            bass_env.battLevelNotificationTimerEnqueued = false;
            for(uint8_t i = 0; i < bass_env.bas_nb; i++)
            {
                bass_env.lastBattLevel[i] = bass_env.readBattLevelCallback(i);
                BASS_BattLevelUpdReq(bass_env.lastBattLevel[i], i); /* Notify clients */
            }
            BASS_NotifyOnTimeout(bass_env.battLevelNotificationTimeout); /* Restart timer */
        }
        break;

        case BASS_BATT_MONITORING_TIMEOUT:
        {
            bass_env.battLevelMonitoringTimerEnqueued = false;

            for(uint8_t i = 0; i < bass_env.bas_nb; i++)
            {
                uint8_t battLevel = bass_env.readBattLevelCallback(i);
                if(bass_env.lastBattLevel[i] != battLevel)
                {
                    BASS_BattLevelUpdReq(battLevel, i);
                    ke_msg_send_basic(BASS_BATT_LEVEL_CHANGED, TASK_APP, TASK_APP);
                }
            }

            BASS_NotifyOnBattLevelChange(bass_env.battLevelMonitoringTimeout); /* Restart timer */
        }
        break;
    }
}
