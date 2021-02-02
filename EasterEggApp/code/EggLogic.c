/*
 * EggLogic.c
 *
 *  Created on: 26.01.2021
 *      Author: mschmidl
 */

#include "app.h"
#include "EggLogic.h"
#include "TLC5955drv.h"

/* attribute structure for EGG LOGIC thread */
const osThreadAttr_t thread_egglogic_attr =
{
    .name = "thEGG",
    .priority = osPriorityNormal,
    .stack_size = 2048
};
__NO_RETURN void vThread_EggLogic(void *argument);

#define EGGLOGIC_QUEUE_DEPTH 10
const osMessageQueueAttr_t queue_egglogic_attr =
{
   .name = "EggQueue"
};
osMessageQueueId_t hEggLogicQueue = NULL;
#define DEFAULT_QUEUE_POST_TIMEOUT 10000UL

const osTimerAttr_t timer_egglogic_attr =
{
	.name = "EggTimer"
};
osTimerId_t hEggLogicTimer = NULL;
void eggLogicTimer_CB( void *argument );


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

    // versteh nicht, warum man das hier braucht, damit es mit dem String weitergeht. Vorher war das auch nicht so!!!
    Restart_Keystroke_Env();
    /* Prepare next banner's key */
    act_key++;
    act_key = act_key % on_semi_banner_size;

    if ( 0 == act_key )
    {
    	EGG_sendMessage( EGG_DONE_WITH_URL,
    			         DEFAULT_QUEUE_POST_TIMEOUT ); // we are done with URL typing
    }
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

        // da kommen wir eh nicht mehr vorbei - warum auch immer
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


void EGG_sendMessage( eggLogicMessage_t msg, uint32_t timeout )
{
	osMessageQueuePut( hEggLogicQueue, // Queue Handle
	                   &msg,           // Message to send
	                   0,              // prio - unused
	                   timeout );      // timeout for posting the message - must be ZERO for IRQ context
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


static void _stateMachine( eggLogicMessage_t msg )
{
    switch ( app_env.eggState )
	{
		case EGG_WAIT4_BLE_CONNECT: {
		    if (ble_env[0].state == APPM_CONNECTED &&
		        VALID_BOND_INFO(ble_env[0].bond_info.STATE))
		    {
				app_env.eggState = EGG_SEND_URL_PART1;
				act_key = 0;
		    }
		} break;

		case EGG_SEND_URL_PART1: {
   	        if (( EGG_TOUCH1_EVENT == msg ) && (hogpd_support_env.enable == true))
            {
				/* Set the key status */
				app_env.key_pushed = true;
				app_env.key_state = KEY_PUSH;
				Send_Keystroke(on_semi_banner[act_key].key,
							   on_semi_banner[act_key].mod);
   	        }
   	        else if ( 't' == msg )
   	        {
				app_env.eggState = EGG_WAIT4_RC5;
   	        }
		} break;
		case EGG_WAIT4_RC5: {
			if ( '5' == msg )
			{
				TLC5955drv_refresh();
				app_env.eggState = EGG_SEND_BRAILLE;
			}
		} break;
		case EGG_SEND_BRAILLE: {

		} break;
		default: {
			app_env.eggState = EGG_WAIT4_BLE_CONNECT;
		} break;
	}

    _showStateMachineOnDebugPin((uint32_t) app_env.eggState);
}


__NO_RETURN void vThread_EggLogic(void *argument)
{
	osTimerStart( hEggLogicTimer, pdMS_TO_TICKS(100) );
    /* Event Kernel Scheduler running in thread */
    while(true)
    {
    	eggLogicMessage_t msg = EGG_NOP;
    	if ( osOK == osMessageQueueGet( hEggLogicQueue, &msg, NULL, -1 ))
    	{
    		switch (msg)
    		{
    			case EGG_TIMER_TICK:
    				// if not connected, restart the EGG logic
    			    if (ble_env[0].state != APPM_CONNECTED )
    			    {
    					app_env.eggState = EGG_WAIT4_BLE_CONNECT;
    					act_key = 0;
    			    }
    				break;
    		}
        	_stateMachine( msg );
    	}
    }
}


void eggLogicTimer_CB( void *argument )
{
	EGG_sendMessage( EGG_TIMER_TICK,
			         DEFAULT_QUEUE_POST_TIMEOUT );
}


void EGG_initThread( void )
{
	// create queues a.s.o
	hEggLogicQueue = osMessageQueueNew( EGGLOGIC_QUEUE_DEPTH,
			                            sizeof(eggLogicMessage_t),
										&queue_egglogic_attr );
	hEggLogicTimer = osTimerNew( eggLogicTimer_CB,
			                     osTimerPeriodic,
								 NULL,
								 &timer_egglogic_attr);
}


void EGG_startThread( void )
{
    osThreadNew(vThread_EggLogic, NULL, &thread_egglogic_attr);
}


