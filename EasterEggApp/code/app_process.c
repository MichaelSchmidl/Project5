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
 * app_process.c
 * - Application task handler definition and support processes
 * ----------------------------------------------------------------------------
 * $Revision: 1.10 $
 * $Date: 2018/02/27 15:42:17 $
 * ------------------------------------------------------------------------- */

#include "app.h"

const struct ke_task_desc TASK_DESC_APP = {
    NULL,       &appm_default_handler,
    appm_state, APPM_STATE_MAX,
    APP_IDX_MAX
};

/* State and event handler definition */
const struct ke_msg_handler appm_default_state[] =
{
    /* Note: Put the default handler on top as this is used for handling any
     *       messages without a defined handler */
    { KE_MSG_DEFAULT_HANDLER, (ke_msg_func_t)Msg_Handler },
    BLE_MESSAGE_HANDLER_LIST,
    BASS_MESSAGE_HANDLER_LIST,
    SEC_MESSAGE_HANDLER_LIST,
    DISS_MESSAGE_HANDLER_LIST,
    HOGPD_MESSAGE_HANDLER_LIST,
    APP_MESSAGE_HANDLER_LIST
};

/* Use the state and event handler definition for all states. */
const struct ke_state_handler appm_default_handler
    = KE_STATE_HANDLER(appm_default_state);

/* Defines a place holder for all task instance's state */
ke_state_t appm_state[APP_IDX_MAX];

/* ----------------------------------------------------------------------------
 * Function      : int APP_Timer(ke_msg_idd_t const msg_id,
 *                               void const *param,
 *                               ke_task_id_t const dest_id,
 *                               ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Handle the application timer event message
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameter (unused)
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int APP_Timer(ke_msg_id_t const msg_id,
              void const *param,
              ke_task_id_t const dest_id,
              ke_task_id_t const src_id)
{
    uint16_t level;

    /* Restart timer */
    ke_timer_set(APP_TEST_TIMER, TASK_APP, TIMER_200MS_SETTING);

    /* Calculate the battery level as a percentage, scaling the battery
     * voltage between 1.4V (max) and 1.1V (min) */
    level = ((ADC->DATA_TRIM_CH[0] - VBAT_1p1V_MEASURED) * BAT_LVL_MAX
             / (VBAT_1p4V_MEASURED - VBAT_1p1V_MEASURED));
    level = ((level > BAT_LVL_MAX) ? BAT_LVL_MAX : level);

    /* Add to the current sum and increment the number of reads,
     * calculating the average over 16 voltage reads */
    app_env.sum_batt_lvl += level;
    app_env.num_batt_read++;
    if (app_env.num_batt_read == 16)
    {
        if ((app_env.sum_batt_lvl >> 4) != app_env.batt_lvl)
        {
            for (unsigned int i = 0; i < NUM_MASTERS; i++)
            {
                app_env.send_batt_ntf[i] = true;
            }

            app_env.batt_lvl = (app_env.sum_batt_lvl >> 4);
        }

        app_env.num_batt_read = 0;
        app_env.sum_batt_lvl = 0;
    }
    return (KE_MSG_CONSUMED);
}

/* ----------------------------------------------------------------------------
 * Function      : int LED_Timer(ke_msg_idd_t const msg_id,
 *                               void const *param,
 *                               ke_task_id_t const dest_id,
 *                               ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Control GPIO "LED_DIO_NUM" behavior using a timer.
 *                 Possible LED behaviors:
 *                     - If the device has not started advertising: the LED
 *                       is off.
 *                     - If the device is advertising but it has not connected
 *                       to any peer: the LED blinks every 200 ms.
 *                     - If the device is advertising and it is connecting to
 *                       fewer than NUM_MASTERS peers: the LED blinks every 2
 *                       seconds according to the number of connected peers
 *                       (i.e., blinks once if one peer is connected, twice if
 *                       two peers are connected, etc.).
 *                     - If the device is connecting to NUM_MASTERS peers (it
 *                       stopped advertising): the LED is steady on.
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameter (unused)
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int LED_Timer(ke_msg_id_t const msg_id,
              void const *param,
              ke_task_id_t const dest_id,
              ke_task_id_t const src_id)
{
    static uint8_t toggle_cnt = 0;
    uint8_t connected_peer_num = Connected_Peer_Num();

    if (ble_env[0].state < APPM_ADVERTISING)
    {
        Sys_GPIO_Set_High(RECOVERY_FOTA_DEBUG_DIO);

        /* Reschedule timer */
        ke_timer_set(LED_TIMER, TASK_APP, TIMER_200MS_SETTING);

        return (KE_MSG_CONSUMED);
    }

    /* Blink LED according to the number of connections */
    switch (connected_peer_num)
    {
        case 0:
        {
            /* Reschedule timer */
            ke_timer_set(LED_TIMER, TASK_APP, TIMER_200MS_SETTING);

            /* Toggle LED_DIO_NUM every 200ms */
            Sys_GPIO_Toggle(RECOVERY_FOTA_DEBUG_DIO);

            toggle_cnt = 0;
        }
        break;

        case NUM_MASTERS:
        {
            /* Reschedule timer */
            ke_timer_set(LED_TIMER, TASK_APP, TIMER_200MS_SETTING);

            /* LED_DIO_NUM steady high */
            Sys_GPIO_Set_Low(RECOVERY_FOTA_DEBUG_DIO);

            toggle_cnt = 0;
        }
        break;

        default:

            /* connected_peer_num is between 1 and NUM_MASTERS (exclusive) */
            if (toggle_cnt >= connected_peer_num * 2)
            {
                toggle_cnt = 0;

                /* Schedule timer for 2s later*/
                ke_timer_set(LED_TIMER, TASK_APP, TIMER_2S_SETTING);

                /* LED_DIO_NUM steady high until next 2s blinking period */
                Sys_GPIO_Set_Low(RECOVERY_FOTA_DEBUG_DIO);
            }
            else
            {
                toggle_cnt++;

                /* Toggle LED_DIO_NUM */
                Sys_GPIO_Toggle(RECOVERY_FOTA_DEBUG_DIO);

                /* Schedule timer for 200ms later*/
                ke_timer_set(LED_TIMER, TASK_APP, TIMER_200MS_SETTING);
            }
    }

    return (KE_MSG_CONSUMED);
}

/* ----------------------------------------------------------------------------
 * Function      : int YAKINDU_Timer(ke_msg_idd_t const msg_id,
 *                                   void const *param,
 *                                   ke_task_id_t const dest_id,
 *                                   ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Handle the application timer event message
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameter (unused)
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int YAKINDU_Timer(ke_msg_id_t const msg_id,
              void const *param,
              ke_task_id_t const dest_id,
              ke_task_id_t const src_id)
{
    /* Restart timer */
    ke_timer_set(YAKINDU_TIMER, TASK_APP, TIMER_YAKINDU_SETTING);

    EggLogic_timerTick( TIMER_YAKINDU_SETTING * 10 );

    return (KE_MSG_CONSUMED);
}


/* ----------------------------------------------------------------------------
 * Function      : void Operation_Cancel(void)
 * ----------------------------------------------------------------------------
 * Description   : Sends a operation cancel
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Operation_Cancel(void)
{
    struct gapm_cancel_cmd *cmd;

    /* Prepare the GAPM_CANCEL_CMD message  */
    cmd = KE_MSG_ALLOC(GAPM_CANCEL_CMD, TASK_GAPM,
                       TASK_APP, gapm_cancel_cmd);

    cmd->operation = GAPM_CANCEL;

    /* Send the message */
    ke_msg_send(cmd);
}

/* ----------------------------------------------------------------------------
 * Function      : int Msg_Handler(ke_msg_id_t const msg_id,
 *                                 void const *param,
 *                                 ke_task_id_t const dest_id,
 *                                 ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Handle any message received from kernel that doesn't have
 *                 a dedicated handler
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameter (unused)
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int Msg_Handler(ke_msg_id_t const msg_id, void *param,
                ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    return (KE_MSG_CONSUMED);
}
