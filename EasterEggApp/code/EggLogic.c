/*
 * EggLogic.c
 *
 *  Created on: 26.01.2021
 *      Author: mschmidl
 */

#include "app.h"
#include "EggLogic.h"

#include "usb_hid_keys.h"
static const struct on_semi_banner_str on_semi_banner[] =
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


static void _showStateMachineOnDebugPin( uint32_t state )
{
    Sys_GPIO_Set_Low(LED_DIO_NUM);
	while (state--)
	{
	    Sys_GPIO_Set_High(LED_DIO_NUM);
	    Sys_GPIO_Set_Low(LED_DIO_NUM);
	}
    Sys_GPIO_Set_High(LED_DIO_NUM);
}


void EGG_stateMachine( void )
{
	// if not connected, restart the EGG logic
    if (ble_env[0].state != APPM_CONNECTED )
    {
		app_env.eggState = EGG_WAIT4_BLE_CONNECT;
    }

    switch ( app_env.eggState )
	{
		case EGG_WAIT4_BLE_CONNECT: {
		    if (ble_env[0].state == APPM_CONNECTED &&
		        VALID_BOND_INFO(ble_env[0].bond_info.STATE))
		    {
				app_env.eggState = EGG_SEND_URL_PART1;
		    }
		} break;

		case EGG_SEND_URL_PART1: {
            if (hogpd_support_env.enable == true && app_env.key_pushed == true)
            {
            	app_env.key_pushed = false;
                Send_Keystroke(on_semi_banner[act_key].key,
                               on_semi_banner[act_key].mod);
            }
		} break;
		default: {
			app_env.eggState = EGG_WAIT4_BLE_CONNECT;
		} break;
	}

    _showStateMachineOnDebugPin((uint32_t) app_env.eggState);
}


