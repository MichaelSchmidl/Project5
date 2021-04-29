/*
 * EggLogic.c
 *
 *  Created on: 26.01.2021
 *      Author: mschmidl
 */

#include "app.h"
#include "EggLogic.h"
#include "TLC5955drv.h"
#include "GYROdrv.h"
#include "PCF8574drv.h"
#include "RC5receiver.h"
#include "CompLEDs.h"
#include "CompBraille.h"
#include "CompMorse.h"

///////////////////////////////////////////////////////////////////////////////
// YAKINDU stuff
#include "Statechart_required.h"
#include "sc_timer_service.h"

struct Statechart eggStatechart;
#define MAX_TIMERS 20
#define TICK_MS 100
static sc_timer_t timers[MAX_TIMERS];
static sc_timer_service_t timer_service;

const char szBrailleText[] = "starte Plattenspieler Release Candidate 5";
uint8_t bBrailleCharDone = 0;

const char szMorseText[] = "bild drehen ";
uint8_t bMorseCharDone = 0;

const char szFinalURLtext[] = " oliverolschewski.markus-kail.de  ";

typedef enum {
	eWatchEventNONE = 0,
	eWatchEventTOUCH = 1,
	eWatchEventGYRO = 2,
	eWatchEventRC5 = 3
}eWatchEvent_t;
static eWatchEvent_t gWatchEvent = eWatchEventNONE;

///////////////////////////////////////////////////////////////////////////////

#include "usb_hid_keys.h"

struct usb_hid_keystroke
{
    uint8_t key;
    uint8_t mod;
};

// https://de.wikipedia.org/wiki/Brailleschrift
static const struct usb_hid_keystroke URL1_keystrokes[] =
{
#if 1
	{ KEY_LEFTMETA,  /* WIN */   KEY_MOD_LMETA },

    { KEY_H,         /* h */     KEY_MOD_NONE },
    { KEY_T,         /* t */     KEY_MOD_NONE },
    { KEY_T,         /* t */     KEY_MOD_NONE },
    { KEY_P,         /* p */     KEY_MOD_NONE },
    { KEY_S,         /* s */     KEY_MOD_NONE },
    { KEY_DOT,       /* : */     KEY_MOD_LSHIFT },
    { KEY_7,         /* / */     KEY_MOD_LSHIFT },
    { KEY_7,         /* / */     KEY_MOD_LSHIFT },
    { KEY_D,         /* d */     KEY_MOD_NONE },
    { KEY_E,         /* e */     KEY_MOD_NONE },
    { KEY_DOT,       /* . */     KEY_MOD_NONE },
    { KEY_W,         /* w */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_K,         /* k */     KEY_MOD_NONE },
#endif
#if 0
	{ KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_P,         /* p */     KEY_MOD_NONE },
    { KEY_E,         /* e */     KEY_MOD_NONE },
    { KEY_D,         /* d */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_A,         /* a */     KEY_MOD_NONE },
    { KEY_DOT,       /* . */     KEY_MOD_NONE },
    { KEY_O,         /* o */     KEY_MOD_NONE },
    { KEY_R,         /* r */     KEY_MOD_NONE },
    { KEY_G,         /* g */     KEY_MOD_NONE },
    { KEY_7,         /* / */     KEY_MOD_LSHIFT },
    { KEY_W,         /* w */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_K,         /* k */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
#endif
#if 0
    { KEY_7,         /* / */     KEY_MOD_LSHIFT },
    { KEY_B,         /* B */     KEY_MOD_LSHIFT },
    { KEY_R,         /* r */     KEY_MOD_NONE },
    { KEY_A,         /* a */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_L,         /* l */     KEY_MOD_NONE },
    { KEY_L,         /* l */     KEY_MOD_NONE },
    { KEY_E,         /* e */     KEY_MOD_NONE },
    { KEY_S,         /* s */     KEY_MOD_NONE },
    { KEY_C,         /* c */     KEY_MOD_NONE },
    { KEY_H,         /* h */     KEY_MOD_NONE },
    { KEY_R,         /* r */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_F,         /* f */     KEY_MOD_NONE },
    { KEY_T,         /* t */     KEY_MOD_NONE },

	{ KEY_ENTER,     /* ENTER */ KEY_MOD_NONE },
#endif
};

static const struct usb_hid_keystroke URL2_keystrokes[] =
{
#if 0
	{ KEY_LEFTMETA,  /* WIN */   KEY_MOD_LMETA },

    { KEY_H,         /* h */     KEY_MOD_NONE },
    { KEY_T,         /* t */     KEY_MOD_NONE },
    { KEY_T,         /* t */     KEY_MOD_NONE },
    { KEY_P,         /* p */     KEY_MOD_NONE },
    { KEY_S,         /* s */     KEY_MOD_NONE },
    { KEY_DOT,       /* : */     KEY_MOD_LSHIFT },
    { KEY_7,         /* / */     KEY_MOD_LSHIFT },
    { KEY_7,         /* / */     KEY_MOD_LSHIFT },
    { KEY_D,         /* d */     KEY_MOD_NONE },
    { KEY_E,         /* e */     KEY_MOD_NONE },
    { KEY_DOT,       /* . */     KEY_MOD_NONE },
    { KEY_W,         /* w */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_K,         /* k */     KEY_MOD_NONE },
#endif
#if 1
	{ KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_P,         /* p */     KEY_MOD_NONE },
    { KEY_E,         /* e */     KEY_MOD_NONE },
    { KEY_D,         /* d */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_A,         /* a */     KEY_MOD_NONE },
    { KEY_DOT,       /* . */     KEY_MOD_NONE },
    { KEY_O,         /* o */     KEY_MOD_NONE },
    { KEY_R,         /* r */     KEY_MOD_NONE },
    { KEY_G,         /* g */     KEY_MOD_NONE },
    { KEY_7,         /* / */     KEY_MOD_LSHIFT },
    { KEY_W,         /* w */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_K,         /* k */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
#endif
#if 0
    { KEY_7,         /* / */     KEY_MOD_LSHIFT },
    { KEY_B,         /* B */     KEY_MOD_LSHIFT },
    { KEY_R,         /* r */     KEY_MOD_NONE },
    { KEY_A,         /* a */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_L,         /* l */     KEY_MOD_NONE },
    { KEY_L,         /* l */     KEY_MOD_NONE },
    { KEY_E,         /* e */     KEY_MOD_NONE },
    { KEY_S,         /* s */     KEY_MOD_NONE },
    { KEY_C,         /* c */     KEY_MOD_NONE },
    { KEY_H,         /* h */     KEY_MOD_NONE },
    { KEY_R,         /* r */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_F,         /* f */     KEY_MOD_NONE },
    { KEY_T,         /* t */     KEY_MOD_NONE },

	{ KEY_ENTER,     /* ENTER */ KEY_MOD_NONE },
#endif
};

static const struct usb_hid_keystroke URL3_keystrokes[] =
{
#if 0
	{ KEY_LEFTMETA,  /* WIN */   KEY_MOD_LMETA },

    { KEY_H,         /* h */     KEY_MOD_NONE },
    { KEY_T,         /* t */     KEY_MOD_NONE },
    { KEY_T,         /* t */     KEY_MOD_NONE },
    { KEY_P,         /* p */     KEY_MOD_NONE },
    { KEY_S,         /* s */     KEY_MOD_NONE },
    { KEY_DOT,       /* : */     KEY_MOD_LSHIFT },
    { KEY_7,         /* / */     KEY_MOD_LSHIFT },
    { KEY_7,         /* / */     KEY_MOD_LSHIFT },
    { KEY_D,         /* d */     KEY_MOD_NONE },
    { KEY_E,         /* e */     KEY_MOD_NONE },
    { KEY_DOT,       /* . */     KEY_MOD_NONE },
    { KEY_W,         /* w */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_K,         /* k */     KEY_MOD_NONE },
#endif
#if 0
	{ KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_P,         /* p */     KEY_MOD_NONE },
    { KEY_E,         /* e */     KEY_MOD_NONE },
    { KEY_D,         /* d */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_A,         /* a */     KEY_MOD_NONE },
    { KEY_DOT,       /* . */     KEY_MOD_NONE },
    { KEY_O,         /* o */     KEY_MOD_NONE },
    { KEY_R,         /* r */     KEY_MOD_NONE },
    { KEY_G,         /* g */     KEY_MOD_NONE },
    { KEY_7,         /* / */     KEY_MOD_LSHIFT },
    { KEY_W,         /* w */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_K,         /* k */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
#endif
#if 1
    { KEY_7,         /* / */     KEY_MOD_LSHIFT },
    { KEY_B,         /* B */     KEY_MOD_LSHIFT },
    { KEY_R,         /* r */     KEY_MOD_NONE },
    { KEY_A,         /* a */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_L,         /* l */     KEY_MOD_NONE },
    { KEY_L,         /* l */     KEY_MOD_NONE },
    { KEY_E,         /* e */     KEY_MOD_NONE },
    { KEY_S,         /* s */     KEY_MOD_NONE },
    { KEY_C,         /* c */     KEY_MOD_NONE },
    { KEY_H,         /* h */     KEY_MOD_NONE },
    { KEY_R,         /* r */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_F,         /* f */     KEY_MOD_NONE },
    { KEY_T,         /* t */     KEY_MOD_NONE },

	{ KEY_ENTER,     /* ENTER */ KEY_MOD_NONE },
#endif
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
    { KEY_E,         /* e */     KEY_MOD_NONE },
    { KEY_L,         /* l */     KEY_MOD_NONE },
    { KEY_E,         /* e */     KEY_MOD_NONE },
    { KEY_C,         /* c */     KEY_MOD_NONE },
    { KEY_T,         /* t */     KEY_MOD_NONE },
    { KEY_R,         /* r */     KEY_MOD_NONE },
    { KEY_O,         /* o */     KEY_MOD_NONE },
    { KEY_N,         /* n */     KEY_MOD_NONE },
    { KEY_I,         /* i */     KEY_MOD_NONE },
    { KEY_C,         /* c */     KEY_MOD_NONE },
    { KEY_S,         /* s */     KEY_MOD_NONE },

	{ KEY_CAPSLOCK,  /*    */    KEY_MOD_NONE },
	{ KEY_CAPSLOCK,  /*   */     KEY_MOD_NONE },

	{ KEY_CAPSLOCK,  /*    */    KEY_MOD_NONE },
	{ KEY_CAPSLOCK,  /*   */     KEY_MOD_NONE },

	{ KEY_CAPSLOCK,  /*    */    KEY_MOD_NONE },
	{ KEY_CAPSLOCK,  /*   */     KEY_MOD_NONE },

	{ KEY_CAPSLOCK,  /*    */    KEY_MOD_NONE },
	{ KEY_CAPSLOCK,  /*   */     KEY_MOD_NONE },

	{ KEY_CAPSLOCK,  /*    */    KEY_MOD_NONE },
	{ KEY_CAPSLOCK,  /*   */     KEY_MOD_NONE },

    { KEY_ESC,       /*    */    KEY_MOD_NONE },
};


struct keystroke_definition
{
	const struct usb_hid_keystroke *pKeystrokes;
    const uint32_t numberOfKeystrokes;
};

#define GREETINGS_KEYSTROKES_INDEX   0
#define URL_PART1_KEYSTROKES_INDEX   1
#define URL_PART2_KEYSTROKES_INDEX   2
#define URL_PART3_KEYSTROKES_INDEX   3
#define MAX_KEYSTROKES_INDEX         4
static const struct keystroke_definition keystrokeSet[] =
{
	{
		GREETINGS_keystrokes,
		sizeof(GREETINGS_keystrokes) / sizeof(struct usb_hid_keystroke)
	},
	{
		URL1_keystrokes,
		sizeof(URL1_keystrokes) / sizeof(struct usb_hid_keystroke)
	},
	{
		URL2_keystrokes,
		sizeof(URL2_keystrokes) / sizeof(struct usb_hid_keystroke)
	},
	{
		URL3_keystrokes,
		sizeof(URL3_keystrokes) / sizeof(struct usb_hid_keystroke)
	}
};


static void _sendKeystroke(const uint8_t key, const uint8_t mod_id)
{
//	app_env.key_pushed = true;
	app_env.key_state = KEY_PUSH; // next we have to send KEY RELEASE - done in ble_hogd.c function Hogpd_ReportUpdRsp()
	Hogpd_ReportUpdReq( ble_env[0].conidx,
					    key,
					    mod_id);
}


static void _showStateMachineDebugInfo( void )
{
	static uint32_t lastStateConfVector = 0;
	uint32_t currentStateConfVector = (uint32_t)eggStatechart.stateConfVector[0];
	if ( lastStateConfVector !=  currentStateConfVector )
	{
		PRINTF( "SC: %d -> %d\r\n", lastStateConfVector, currentStateConfVector );
		lastStateConfVector = currentStateConfVector;
	}
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


void statechart_shutDownSystem(Statechart* handle)
{
    PRINTF("%s entered\n", __func__);
    ke_timer_clear(YAKINDU_TIMER, TASK_APP);

    Sys_GPIO_Set_High(RECOVERY_FOTA_DEBUG_DIO); // turn OFF on board LED
#if 1
    Sys_GPIO_Set_Low(POWER_ON_DIO); // turn off immediately
#else
	#warning NO AUTOSHUTDOWN
#endif
}


void statechart_sendKBDstroke(Statechart* handle, const sc_integer whichString, const sc_integer index)
{
//    PRINTF("%s entered with %d/%d\n", __func__, whichString, index);
	if ( MAX_KEYSTROKES_INDEX <= whichString ) return;

	if ( index < keystrokeSet[whichString].numberOfKeystrokes )
	{
		_sendKeystroke( keystrokeSet[whichString].pKeystrokes[index].key,
						keystrokeSet[whichString].pKeystrokes[index].mod );
	}
}


sc_integer statechart_getKBDstringLength( Statechart* handle, const sc_integer whichString)
{
//    PRINTF("%s entered\r\n", __func__);
	return keystrokeSet[whichString].numberOfKeystrokes;
}


sc_integer statechart_getBrailleTextLength( Statechart* handle)
{
//    PRINTF("%s entered\r\n", __func__);
	return strlen( szBrailleText );
}


void statechart_sendTLCbraille( Statechart* handle, const sc_integer i)
{
	bBrailleCharDone = 0;
	if ( i < statechart_getBrailleTextLength( &eggStatechart ))
	{
		compBraille_showChar( szBrailleText[i] );
	}
	bBrailleCharDone = 1;
}


sc_integer statechart_getMorseTextLength( Statechart* handle)
{
	//    PRINTF("%s entered\r\n", __func__);
		return strlen( szMorseText );
}


void statechart_sendTLCmorse( Statechart* handle, const sc_integer i)
{
	bMorseCharDone = 0;
	if ( i < statechart_getMorseTextLength( &eggStatechart ))
	{
		compMorse_showChar( szMorseText[i] );
	}
	bMorseCharDone = 1;
}


void EggLogic_RC5match( void )
{
	statechart_raise_rC5match( &eggStatechart );
}


void statechart_showLaufschrift( Statechart* handle)
{
	LED_showText( szFinalURLtext );
}


void statechart_setWatchEvent(Statechart* handle, const sc_integer which )
{
	gWatchEvent = (eWatchEvent_t)which;
	if ( eWatchEventRC5 == which )
	{
	    NVIC_EnableIRQ(DIO1_IRQn);
	}
}

///////////////////////////////////////////////////////////////////////////////


static void _updateGyroAndTouchInfo( void )
{
	if ( 0 != RC5_doWeHaveAmatch() )
	{
		statechart_raise_rC5match( &eggStatechart );
	}

    static GRYRO_Orientation_t lastGyroState = 0xFF;
    if ( eWatchEventGYRO == gWatchEvent )
    {
    	GRYRO_Orientation_t gyro = GYRO_getOrientation();
    	if ( gyro != lastGyroState )
    	{
    		PRINTF("GYRO %d\r\n", (int)gyro);
    		if ( LANDSCAPE_RIGHT == gyro )
    		statechart_raise_gYROtilt( &eggStatechart );
    	}
    	lastGyroState = gyro;
    }

	static uint8_t lastTouchState = 0;
	if ( eWatchEventTOUCH == gWatchEvent )
	{
		uint8_t touch = PCF8574_read() & 0x07;
		if ( touch != lastTouchState )
		{
			PRINTF("TOUCH %X\r\n", touch);
			if ( 0 != touch )
			{
				if ( touch & (1 << 0) )
				{
					statechart_raise_touch1IRQ( &eggStatechart );
				}
				if ( touch & (1 << 1) )
				{
					statechart_raise_touch2IRQ( &eggStatechart );
				}
				if ( touch & (1 << 2) )
				{
					statechart_raise_touch3IRQ( &eggStatechart );
				}
			}
		}
		lastTouchState = touch;
	}
}


void EggLogic_init( void )
{
//    PRINTF("%s entered\n", __func__);

    // initialize hardware drivers we want to use
	TLC5955drv_start();

	// YAKINDU stuff
    sc_timer_service_init(&timer_service, timers, MAX_TIMERS, (sc_raise_time_event_fp) &statechart_raise_time_event);
	statechart_init( &eggStatechart );
	statechart_enter( &eggStatechart);

	// turn on ON indicator
	LED_setBLEADVIndicator(1);
#if 0
	DELAY_MS(1000);
	LED_showText(" * P5 lebt! *  ");
	DELAY_MS(1000);

	compMorse_showText( "P5 is sinking SOS" );
	DELAY_MS(1000);
	compBraille_showText( "P5 lebt " );
#endif
}


void EggLogic_timerTick( uint32_t ms )
{
	static uint32_t lastBLEconnected = 0;
//    PRINTF("%s entered with %dms\n", __func__, ms);
//	if ( 0 == _bLongRunningFunctionActive )
	{
	    sc_timer_service_proceed(&timer_service, ms);

		// if connected start...
	    if (ble_env[0].state == APPM_CONNECTED &&
	        VALID_BOND_INFO(ble_env[0].bond_info.STATE))
	    {
	    	if ( 0 == lastBLEconnected )
	    	{
	    		LED_setBLEADVIndicator(0);
	    		LED_setBLEconnectedIndicator(1);
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

	    if ( 1 == bBrailleCharDone )
	    {
	    	bBrailleCharDone = 0;
	    	statechart_raise_brailleCharDone( &eggStatechart );
	    }

	    if ( 1 == bMorseCharDone )
	    {
	    	bMorseCharDone = 0;
	    	statechart_raise_morseCharDone( &eggStatechart );
	    }

	    // poll info of GYRO and TOUCHes every 100ms
	    _updateGyroAndTouchInfo();

	    // show statemachine status
	    _showStateMachineDebugInfo();
	}

}
