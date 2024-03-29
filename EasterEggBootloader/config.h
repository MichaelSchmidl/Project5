/* ----------------------------------------------------------------------------
 * Copyright (c) 2020 Semiconductor Components Industries, LLC (d/b/a ON
 * Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 *
 * This is Reusable Code.
 *
 * ----------------------------------------------------------------------------
 * config.h
 * - Defines of all configuration options for the BootLoader.
 * ------------------------------------------------------------------------- */

#ifndef _CONFIG_H    /* avoids multiple inclusion */
#define _CONFIG_H

/* ----------------------------------------------------------------------------
 * Version information
 * ------------------------------------------------------------------------- */

#ifdef _DEBUG
#define VER_ID                          "BOOTL*"
#endif    /* ifdef _DEBUG */
#ifdef NDEBUG
#define VER_ID                          "BOOTLD"
#endif    /* ifdef NDEBUG */

#define VER_MAJOR                       5
#define VER_MINOR                       5
#define VER_REVISION                    5

/* ----------------------------------------------------------------------------
 * Options
 * ------------------------------------------------------------------------- */

/*** TARG ***/
#define CFG_BUCK_COV                    0

/* Flags can be used to control if building for development board or dongle */
#define RSL10_DEV                       0
#define RSL10_DONGLE                    1
#define RSL10_P5                        2
#ifndef RSL10_DEV_OR_DONGLE
#define RSL10_DEV_OR_DONGLE             RSL10_P5
#endif

/*** GPIO ***/
#if (RSL10_DEV_OR_DONGLE == RSL10_P5)
#define CFG_nUPDATE_DIO                 11 // N.C.
#define CFG_LED_RED_DIO                 12
#elif (RSL10_DEV_OR_DONGLE == RSL10_DONGLE)
#define CFG_nUPDATE_DIO                 6
#define CFG_LED_RED_DIO                 8
#else
#define CFG_nUPDATE_DIO                 5
#define CFG_LED_RED_DIO                 6
#endif

/*** UART ***/
#define CFG_UART_BAUD_RATE              1000000
#if (RSL10_DEV_OR_DONGLE == RSL10_P5)
#define CFG_UART_RXD_DIO                7 // N.C.
#define CFG_UART_TXD_DIO                8 // N.C.
#elif (RSL10_DEV_OR_DONGLE == RSL10_DONGLE)
#define CFG_UART_RXD_DIO                7
#define CFG_UART_TXD_DIO                10
#else
#define CFG_UART_RXD_DIO                4
#define CFG_UART_TXD_DIO                5
#endif

/*** Updater ***/
#define CFG_TIMEOUT                     30  /* in seconds, 0 = no timeout */
#define CFG_READ_SUPPORT                0

#endif    /* _CONFIG_H */
