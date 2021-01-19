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
#include "code/RC5receiver.h"

DRIVER_GPIO_t *gpio;

/* ----------------------------------------------------------------------------
 * Application Version
 * ------------------------------------------------------------------------- */
SYS_FOTA_VERSION(VER_ID, VER_MAJOR, VER_MINOR, VER_REVISION);

#include "usb_hid_keys.h"
static struct on_semi_banner_str on_semi_banner[] =
{
	{ KEY_LEFTMETA,  /* WIN */   KEY_MOD_LMETA },

    { KEY_H,         /* h */     KEY_MOD_NONE },
    { KEY_T,         /* t */     KEY_MOD_NONE },
    { KEY_T,         /* t */     KEY_MOD_NONE },
    { KEY_P,         /* p */     KEY_MOD_NONE },
    { KEY_DOT,       /* : */     KEY_MOD_LSHIFT },
    { KEY_7,         /* / */     KEY_MOD_LSHIFT },
    { KEY_7,         /* / */     KEY_MOD_LSHIFT },
    { KEY_K,         /* k */     KEY_MOD_NONE },
    { KEY_A,         /* a */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_L,         /* l */     KEY_MOD_NONE },
    { KEY_DOT,       /* . */     KEY_MOD_NONE },
    { KEY_D,         /* d */     KEY_MOD_NONE },
    { KEY_E,         /* e */     KEY_MOD_NONE },
    { KEY_7,         /* / */     KEY_MOD_LSHIFT },
    { KEY_O,         /* O */     KEY_MOD_LSHIFT },
    { KEY_L,         /* l */     KEY_MOD_NONE },
    { KEY_L,         /* l */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_DOT,       /* . */     KEY_MOD_NONE },
    { KEY_H,         /* h */     KEY_MOD_NONE },
    { KEY_T,         /* t */     KEY_MOD_NONE },
    { KEY_M,         /* m */     KEY_MOD_NONE },
    { KEY_L,         /* l */     KEY_MOD_NONE },

	{ KEY_CAPSLOCK,  /*    */   KEY_MOD_NONE },
	{ KEY_CAPSLOCK,  /*   */   KEY_MOD_NONE },

    { KEY_ESC,  /*    */   KEY_MOD_NONE },
//	{ KEY_ENTER,     /* ENTER */ KEY_MOD_NONE }
};
static uint32_t on_semi_banner_size = \
    sizeof(on_semi_banner) / \
    sizeof(struct on_semi_banner_str);
static uint32_t act_key = 0;

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


/* ----------------------------------------------------------------------------
 * Function      : void Restart_Keystroke_Env(void)
 * ----------------------------------------------------------------------------
 * Description   : Initialize keyboard environment
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Restart_Keystroke_Env(void)
{
    app_env.key_pushed = false;
    app_env.key_state = KEY_IDLE;
}

/* ----------------------------------------------------------------------------
 * Function      : void Update_Keystroke_Env(void)
 * ----------------------------------------------------------------------------
 * Description   : Initialize keyboard environment
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Update_Keystroke_Env(void)
{
    app_env.key_state = KEY_UPDATE;
}

/* ----------------------------------------------------------------------------
 * Function      : Send_Keystroke(void)
 * ----------------------------------------------------------------------------
 * Description   : Send keystroke to the host
 * Inputs        : key, mod_id
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static void Send_Keystroke(const uint8_t key, const uint8_t mod_id)
{
    switch (app_env.key_state)
    {
        case KEY_PUSH:
        {
            app_env.key_state = KEY_REL;
            Hogpd_ReportUpdReq(ble_env[0].conidx,
                               key,
                               mod_id);
        }
        break;

        case KEY_UPDATE:
        {
            /* Restart Keyboard Environment */
            Restart_Keystroke_Env();

            /* Prepare next banner's key */
            act_key++;
            act_key = act_key % on_semi_banner_size;
        }
        break;

        case KEY_REL:
        default:
        {
        }
        break;
    }
}

int main(void)
{
    App_Initialize();

//    DFUS_Initialize(); /* Initialize DFU Service Server */

    SystemCoreClockUpdate();

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

        /* Send keystrokes to client if DIO5 is pushed */
        if (ble_env[0].state == APPM_CONNECTED &&
            VALID_BOND_INFO(ble_env[0].bond_info.STATE))
        {
            if (hogpd_support_env.enable == true && app_env.key_pushed == true)
            {
                Send_Keystroke(on_semi_banner[act_key].key,
                               on_semi_banner[act_key].mod);
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
        SYS_WAIT_FOR_EVENT;
    }
}
