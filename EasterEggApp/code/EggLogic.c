/*
 * EggLogic.c
 *
 *  Created on: 26.01.2021
 *      Author: mschmidl
 */

#include "app.h"
#include "EggLogic.h"
#include "TLC5955drv.h"

#include "Statechart.h"

struct Statechart eggStatechart;

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

const osTimerAttr_t timer_systemOFF_attr =
{
	.name = "OffTimer"
};
osTimerId_t hSystemOFFimer = NULL;
void systemOffTimer_CB( void *argument );


#include "usb_hid_keys.h"

struct usb_hid_keystroke
{
    uint8_t key;
    uint8_t mod;
};

static const struct usb_hid_keystroke URL1_keystrokes[] =
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

	{ KEY_CAPSLOCK,  /*   */     KEY_MOD_NONE },
	{ KEY_CAPSLOCK,  /*   */     KEY_MOD_NONE },

    { KEY_ESC,       /*    */    KEY_MOD_NONE },
//	{ KEY_ENTER,     /* ENTER */ KEY_MOD_NONE }
};

static const struct usb_hid_keystroke GREETINGS_keystrokes[] =
{
	{ KEY_LEFTMETA,  /* WIN */   KEY_MOD_LMETA },

	// toggle keyboard LED
	{ KEY_CAPSLOCK,  /*    */    KEY_MOD_NONE },
	{ KEY_CAPSLOCK,  /*   */     KEY_MOD_NONE },

    { KEY_D,         /* d */     KEY_MOD_NONE },
    { KEY_O,         /* o */     KEY_MOD_NONE },
    { KEY_N,         /* n */     KEY_MOD_NONE },
    { KEY_T,         /* t */     KEY_MOD_NONE },
    { KEY_SPACE,     /*   */     KEY_MOD_NONE },
    { KEY_T,         /* t */     KEY_MOD_NONE },
    { KEY_O,         /* o */     KEY_MOD_NONE },
    { KEY_U,         /* u */     KEY_MOD_NONE },
    { KEY_C,         /* c */     KEY_MOD_NONE },
    { KEY_H,         /* h */     KEY_MOD_NONE },
    { KEY_SPACE,     /*   */     KEY_MOD_NONE },
    { KEY_T,         /* t */     KEY_MOD_NONE },
    { KEY_H,         /* h */     KEY_MOD_NONE },
    { KEY_E,         /* e */     KEY_MOD_NONE },
    { KEY_SPACE,     /*   */     KEY_MOD_NONE },
    { KEY_F,         /* f */     KEY_MOD_NONE },
    { KEY_A,         /* a */     KEY_MOD_NONE },
    { KEY_C,         /* c */     KEY_MOD_NONE },
    { KEY_E,         /* e */     KEY_MOD_NONE },
    { KEY_S,         /* s */     KEY_MOD_NONE },

    { KEY_SPACE,     /*   */     KEY_MOD_NONE },
    { KEY_SPACE,     /*   */     KEY_MOD_NONE },
    { KEY_SPACE,     /*   */     KEY_MOD_NONE },
    { KEY_ESC,       /*    */    KEY_MOD_NONE }
};


struct keystroke_definition
{
	const struct usb_hid_keystroke *pKeystrokes;
    uint32_t numberOfKeystrokes;
    eggLogicMessage_t moreKeysToSendMessage;
    eggLogicMessage_t lastKeySentMessage;
};

#define GREETINGS_INDEX 0
#define URL1_INDEX      1
static const struct keystroke_definition keystrokeSet[] =
{
	{
		GREETINGS_keystrokes,
		sizeof(GREETINGS_keystrokes) / sizeof(struct usb_hid_keystroke),
		EGGLOGIC_MESSAGE_SEND_NEXT_GREETING_KEY,
		EGGLOGIC_MESSAGE_LAST_GREETING_KEY_SENT
	},
	{
		URL1_keystrokes,
		sizeof(URL1_keystrokes) / sizeof(struct usb_hid_keystroke),
		EGGLOGIC_MESSAGE_NOP,
		EGGLOGIC_MESSAGE_DONE_WITH_URL
	}
};

static uint32_t actualKeyIndex = 0;
static uint32_t actualStringIndex = 0;


void EGG_doneWithSendingKeyStroke(void)
{
	// set key state back, because we have send keypress and keyrelease now
//    app_env.key_pushed = false;
    app_env.key_state = KEY_IDLE;

    /* Prepare next banner's key */
    if ( actualKeyIndex < keystrokeSet[ actualStringIndex ].numberOfKeystrokes )
    {
        actualKeyIndex++;
    	EGG_sendMessage( keystrokeSet[actualStringIndex].moreKeysToSendMessage,
    			         DEFAULT_QUEUE_POST_TIMEOUT ); // we are done with URL typing
    }
    else
    {
    	EGG_sendMessage( keystrokeSet[actualStringIndex].lastKeySentMessage,
    			         DEFAULT_QUEUE_POST_TIMEOUT ); // we are done with URL typing
    }
}


static void _sendKeystroke(const uint8_t key, const uint8_t mod_id)
{
//	app_env.key_pushed = true;
	app_env.key_state = KEY_REL; // next we have to send KEY RELEASE - done in ble_hogd.c function Hogpd_ReportUpdRsp()
	Hogpd_ReportUpdReq( ble_env[0].conidx,
					    key,
					    mod_id);
}


static void _restartAutoOFFtimer( void )
{
	osTimerStop( hSystemOFFimer );
	osTimerStart( hSystemOFFimer, pdMS_TO_TICKS( 15UL * 60UL * 1000UL ) ); // 15min
}


void EGG_sendMessage( eggLogicMessage_t msg, uint32_t timeout )
{
	osMessageQueuePut( hEggLogicQueue, // Queue Handle
	                   &msg,           // Message to send
	                   0,              // prio - unused
	                   timeout );      // timeout for posting the message - must be ZERO for IRQ context
}


static void _showStateMachineDebugInfo( uint32_t state )
{
	Sys_GPIO_Set_High(RECOVERY_FOTA_DEBUG_DIO);
	while (state--)
	{
	    Sys_GPIO_Set_Low(RECOVERY_FOTA_DEBUG_DIO);
	    Sys_GPIO_Set_High(RECOVERY_FOTA_DEBUG_DIO);
	}
    Sys_GPIO_Set_Low(RECOVERY_FOTA_DEBUG_DIO);

    static eggLogic_state_t last_state = EGG_UNKNOWN_STATE;
	if ( last_state != app_env.eggState )
	{
	    PRINTF("%s %d->%d\n", __func__, last_state, app_env.eggState);
	    last_state = app_env.eggState;
	}
}


static void _stateMachine( eggLogicMessage_t msg )
{
    static eggLogic_state_t last_state = EGG_UNKNOWN_STATE;

    switch ( app_env.eggState )
	{
		case EGG_WAIT4_BLE_CONNECT: {
		    if (ble_env[0].state == APPM_CONNECTED &&
		        VALID_BOND_INFO(ble_env[0].bond_info.STATE))
		    {
				app_env.eggState = EGG_SEND_GREETING;
				actualKeyIndex = 0;
		    	EGG_sendMessage( EGGLOGIC_MESSAGE_SEND_NEXT_GREETING_KEY,
		    			         DEFAULT_QUEUE_POST_TIMEOUT );
		    }
		} break;
		case EGG_SEND_GREETING: {
			actualStringIndex = GREETINGS_INDEX; // GREETINGS
   	        if (( EGGLOGIC_MESSAGE_TIMER_TICK == msg ) && (hogpd_support_env.enable == true))
            {
				_sendKeystroke( keystrokeSet[actualStringIndex].pKeystrokes[actualKeyIndex].key,
								keystrokeSet[actualStringIndex].pKeystrokes[actualKeyIndex].mod );
            }
   	        else if ( EGGLOGIC_MESSAGE_LAST_GREETING_KEY_SENT == msg )
   	        {
				app_env.eggState = EGG_SEND_URL_PART1;
				actualKeyIndex = 0;
   	        }
		} break;
		case EGG_SEND_URL_PART1: {
   	        if (( EGGLOGIC_MESSAGE_TOUCH1_EVENT == msg ) && (hogpd_support_env.enable == true))
            {
   				_restartAutoOFFtimer();
				/* Set the key status */
				_sendKeystroke( keystrokeSet[actualStringIndex].pKeystrokes[actualKeyIndex].key,
		                        keystrokeSet[actualStringIndex].pKeystrokes[actualKeyIndex].mod );
   	        }
   	        else if ( EGGLOGIC_MESSAGE_DONE_WITH_URL == msg )
   	        {
				app_env.eggState = EGG_WAIT4_RC5;
   	        }
		} break;
		case EGG_WAIT4_RC5: {
			if ( EGGLOGIC_MESSAGE_RC5_MATCH == msg )
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

    // restart the AUTO OFF timer with every state change
	if ( last_state != app_env.eggState )
	{
		_restartAutoOFFtimer();
	    last_state = app_env.eggState;
	}

	// show debug information about state machine on RTT and DEBUG pin
    _showStateMachineDebugInfo((uint32_t) app_env.eggState);
}


__NO_RETURN void vThread_EggLogic(void *argument)
{
    PRINTF("%s entered\n", __func__);
	osTimerStart( hEggLogicTimer, pdMS_TO_TICKS(100) );

	// YAKINDU stuff
	statechart_init( &eggStatechart );
	statechart_enter( &eggStatechart);

    while(true)
    {
    	eggLogicMessage_t msg = EGGLOGIC_MESSAGE_NOP;
    	if ( osOK == osMessageQueueGet( hEggLogicQueue, &msg, NULL, -1 ))
    	{
    		switch (msg)
    		{
    			case EGGLOGIC_MESSAGE_TIMER_TICK:
    				// if connected start...
    			    if (ble_env[0].state == APPM_CONNECTED &&
    			        VALID_BOND_INFO(ble_env[0].bond_info.STATE))
    			    {
    			    	statechart_raise_bLEconnected( &eggStatechart );
    			    }
    				// if not connected, restart the EGG logic
    			    if (ble_env[0].state != APPM_CONNECTED )
    			    {
    			    	statechart_raise_bLEdisconnected( &eggStatechart );
    					app_env.eggState = EGG_WAIT4_BLE_CONNECT;
    					actualKeyIndex = 0;
    			    }
    				break;
    			case EGGLOGIC_MESSAGE_TOUCH_IRQ:{
    				//! TODO: nachsehen welcher der drei Touches den IRQ ausgelöst hat
    				EGG_sendMessage( EGGLOGIC_MESSAGE_TOUCH1_EVENT, // for now, we assume TOUCH1
    						         DEFAULT_QUEUE_POST_TIMEOUT );
    			} break;
    			default:
    				break;
    		}
        	_stateMachine( msg );
    	}
    }
}


void systemOffTimer_CB( void *argument )
{
    Sys_GPIO_Set_Low(POWER_ON_DIO); // turn off immediately
}


void eggLogicTimer_CB( void *argument )
{
	EGG_sendMessage( EGGLOGIC_MESSAGE_TIMER_TICK,
			         DEFAULT_QUEUE_POST_TIMEOUT );
}


void EGG_initThread( void )
{
    PRINTF("%s entered\n", __func__);
	// create queues a.s.o
	hEggLogicQueue = osMessageQueueNew( EGGLOGIC_QUEUE_DEPTH,
			                            sizeof(eggLogicMessage_t),
										&queue_egglogic_attr );
	hEggLogicTimer = osTimerNew( eggLogicTimer_CB,
			                     osTimerPeriodic,
								 NULL,
								 &timer_egglogic_attr);
	hSystemOFFimer = osTimerNew( systemOffTimer_CB,
			                     osTimerOnce,
								 NULL,
								 &timer_systemOFF_attr);

	// initialize hardware drivers we want to use
	TLC5955drv_start();
}


void EGG_startThread( void )
{
    PRINTF("%s entered\n", __func__);
    osThreadNew(vThread_EggLogic, NULL, &thread_egglogic_attr);
}


