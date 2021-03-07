/*
 * EggLogic.c
 *
 *  Created on: 26.01.2021
 *      Author: mschmidl
 */

#include "app.h"
#include "EggLogic.h"
#include "TLC5955drv.h"
#include "CompLEDs.h"

///////////////////////////////////////////////////////////////////////////////
// YAKINDU stuff
#include "Statechart_required.h"
#include "sc_timer_service.h"

struct Statechart eggStatechart;
#define MAX_TIMERS 20
#define TICK_MS 100
static sc_timer_t timers[MAX_TIMERS];
static sc_timer_service_t timer_service;
///////////////////////////////////////////////////////////////////////////////

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
    { KEY_SPACE,     /*   */     KEY_MOD_NONE },
    { KEY_SPACE,     /*   */     KEY_MOD_NONE },
    { KEY_SPACE,     /*   */     KEY_MOD_NONE },
    { KEY_SPACE,     /*   */     KEY_MOD_NONE },
    { KEY_SPACE,     /*   */     KEY_MOD_NONE },
    { KEY_SPACE,     /*   */     KEY_MOD_NONE },
    { KEY_SPACE,     /*   */     KEY_MOD_NONE },
    { KEY_SPACE,     /*   */     KEY_MOD_NONE },

    { KEY_ESC,       /*    */    KEY_MOD_NONE },
};


struct keystroke_definition
{
	const struct usb_hid_keystroke *pKeystrokes;
    uint32_t numberOfKeystrokes;
};

#define GREETINGS_INDEX 0
#define URL1_INDEX      1
static const struct keystroke_definition keystrokeSet[] =
{
	{
		GREETINGS_keystrokes,
		sizeof(GREETINGS_keystrokes) / sizeof(struct usb_hid_keystroke)
	},
	{
		URL1_keystrokes,
		sizeof(URL1_keystrokes) / sizeof(struct usb_hid_keystroke)
	}
};


static void _sendKeystroke(const uint8_t key, const uint8_t mod_id)
{
//	app_env.key_pushed = true;
	app_env.key_state = KEY_REL; // next we have to send KEY RELEASE - done in ble_hogd.c function Hogpd_ReportUpdRsp()
	Hogpd_ReportUpdReq( ble_env[0].conidx,
					    key,
					    mod_id);
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
}


///////////////////////////////////////////////////////////////////////////////
// YAKINDU function implementations
void statechart_set_timer(Statechart* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic){
	sc_timer_start(&timer_service, (void*) handle, evid, time_ms, periodic);
}


void statechart_unset_timer(Statechart* handle, const sc_eventid evid){
	sc_timer_cancel(&timer_service, evid);
	(void) handle;
}


void EGG_doneWithSendingKeyStroke( void )
{
	statechart_raise_kBDstrokeSent( &eggStatechart );
}


void statechart_toggleDebugLED(Statechart* handle)
{
//    Sys_GPIO_Toggle(RECOVERY_FOTA_DEBUG_DIO); // toggle debug LED
}


void statechart_shutDownSystem(Statechart* handle)
{
    PRINTF("%s entered\n", __func__);
    Sys_GPIO_Set_High(RECOVERY_FOTA_DEBUG_DIO); // turn OFF on board LED
#if 1
    Sys_GPIO_Set_Low(POWER_ON_DIO); // turn off immediately
#else
	#warning NO AUTOSHUTDOWN
#endif
}


void statechart_sendKBDstroke(Statechart* handle, const sc_integer whichString, const sc_integer index)
{
	EGG_doneWithSendingKeyStroke();
	return;
//    PRINTF("%s entered with %d/%d\n", __func__, whichString, index);
	//!TODO: check index end raise STRING_DONE if so
	if ( index >= keystrokeSet[whichString].numberOfKeystrokes )
	{

	}
	else
	{
		_sendKeystroke( keystrokeSet[whichString].pKeystrokes[index].key,
						keystrokeSet[whichString].pKeystrokes[index].mod );
	}
}


sc_integer statechart_getKBDstringLength( Statechart* handle, const sc_integer whichString)
{
//    PRINTF("%s entered\n", __func__);
	return keystrokeSet[whichString].numberOfKeystrokes;
}


void statechart_sendTLCbraille( Statechart* handle)
{

}


void statechart_sendTLCmorse( Statechart* handle, const sc_integer index)
{

}


void statechart_sendURLstroke( Statechart* handle)
{

}
///////////////////////////////////////////////////////////////////////////////


void EggLogic_init( void )
{
//    PRINTF("%s entered\n", __func__);

    // initialize hardware drivers we want to use
	TLC5955drv_start();

	// YAKINDU stuff
    sc_timer_service_init(&timer_service, timers, MAX_TIMERS, (sc_raise_time_event_fp) &statechart_raise_time_event);
	statechart_init( &eggStatechart );
	statechart_enter( &eggStatechart);

	// turn on ON inidicator
	LED_setBLEADVIndicator(1);

	compBraille_showText( "P5 lebt " );
	LED_showText(" * P5 lebt! *  ");
}


void EggLogic_timerTick( uint32_t ms )
{
	static uint32_t lastBLEconnected = 0;
//    PRINTF("%s entered with %dms\n", __func__, ms);
    sc_timer_service_proceed(&timer_service, ms);

	// if connected start...
    if (ble_env[0].state == APPM_CONNECTED &&
        VALID_BOND_INFO(ble_env[0].bond_info.STATE))
    {
    	if ( 0 == lastBLEconnected )
    	{
//    		LED_setBLEADVIndicator(0);
//    		LED_setBLEconnectedIndicator(1);
    		statechart_raise_bLEconnected( &eggStatechart );
        	lastBLEconnected = 1;
    	}
    }
	// if not connected, restart the EGG logic
    if (ble_env[0].state != APPM_CONNECTED )
    {
    	if ( 1 == lastBLEconnected )
    	{
    		LED_setBLEconnectedIndicator(0);
    		LED_setBLEADVIndicator(1);
        	statechart_raise_bLEdisconnected( &eggStatechart );
        	lastBLEconnected = 0;
    	}
    }
}
