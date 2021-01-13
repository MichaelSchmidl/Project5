/* ----------------------------------------------------------------------------
 * Copyright (c) 2019 Semiconductor Components Industries, LLC (d/b/a ON
 * Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * startup.S
 * - GCC compatible CMSIS Cortex-M3 Core Device start-up file for the
 *   ARM Cortex-M3 processor
 *
 * - It has two vector tables, one for the Boot-Up and the other for FW update
 *   process. The later is relocated to PRAM by the linker.
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Boot Vector Table
 * ------------------------------------------------------------------------- */

    .section ".boot_vector"
    .globl  BootVector_Table
    .type   BootVector_Table, %object


BootVector_Table:
    .long   __stack                         /*  0 Initial Stack Pointer */
    .long   Sys_Boot_ResetHandler           /*  1 Reset Handler */
    .long   Sys_Boot_ResetHandler           /*  2 Non-Maskable Interrupt Handler */
    .long   Sys_Boot_ResetHandler           /*  3 Hard Fault Handler */
    .long   Sys_Boot_ResetHandler           /*  4 Mem Manage Fault Handler */
    .long   Sys_Boot_ResetHandler           /*  5 Bus Fault Handler */
    .long   Sys_Boot_ResetHandler           /*  6 Usage Fault Handler */
    .long   Sys_Boot_version                /*  7 Pointer to version info */
    .long   Sys_Boot_Updater                /*  8 Entry point for Updater */
    .long   Sys_Boot_NextImage              /*  9 Sys_Boot_GetNextImage */
    .size   BootVector_Table, . - BootVector_Table



/* ----------------------------------------------------------------------------
 * RAM Vector Table
 * ------------------------------------------------------------------------- */

    .section ".interrupt_vector"
    .globl  ISR_Vector_Table
    .type   ISR_Vector_Table, %object


ISR_Vector_Table:
    .long   __stack                         /*  0 Initial Stack Pointer */
    .long   Reset_Handler                   /*  1 Reset Handler */
    .long   NMI_Handler                     /*  2 Non-Maskable Interrupt Handler */
    .long   HardFault_Handler               /*  3 Hard Fault Handler */
    .long   MemManage_Handler               /*  4 Mem Manage Fault Handler */
    .long   BusFault_Handler                /*  5 Bus Fault Handler */
    .long   UsageFault_Handler              /*  6 Usage Fault Handler */
    .long   Sys_Boot_version                /*  7 Pointer to version info */
    .long   0                               /*  8 Reserved */
    .long   0                               /*  9 Reserved */
    .long   0                               /* 10 Reserved */
    .long   SVC_Handler                     /* 11 Supervisor Call Handler */
    .long   DebugMon_Handler                /* 12 Debug Monitor Handler */
    .long   0                               /* 13 Reserved */
    .long   PendSV_Handler                  /* 14 PendSV Handler */
    .long   SysTick_Handler                 /* 15 SysTick Handler */
    .size   ISR_Vector_Table, . - ISR_Vector_Table

    .thumb

/* ----------------------------------------------------------------------------
 * Reset Handler
 * ------------------------------------------------------------------------- */

    .section .reset,"x",%progbits
    .thumb_func
    .globl  Reset_Handler
    .type   Reset_Handler, %function
Reset_Handler:
    .fnstart
    LDR     R0, =SystemInit
    BLX     R0
    LDR     R0, =_start
    BX      R0
    .pool
    .cantunwind
    .fnend
    .size   Reset_Handler, . - Reset_Handler

    .section ".text"

/* ----------------------------------------------------------------------------
 * Place-holder Exception Handlers
 * - Weak definitions
 * - If not modified or replaced, these handlers implement infinite loops
 * ------------------------------------------------------------------------- */
    .weak   NMI_Handler
    .type   NMI_Handler, %function
NMI_Handler:
    B       .
    .size   NMI_Handler, . - NMI_Handler

    .weak   HardFault_Handler
    .type   HardFault_Handler, %function
HardFault_Handler:
    B       .
    .size   HardFault_Handler, . - HardFault_Handler

    .weak   MemManage_Handler
    .type   MemManage_Handler, %function
MemManage_Handler:
    B       .
    .size   MemManage_Handler, . - MemManage_Handler

    .weak   BusFault_Handler
    .type   BusFault_Handler, %function
BusFault_Handler:
    B       .
    .size   BusFault_Handler, . - BusFault_Handler

    .weak   UsageFault_Handler
    .type   UsageFault_Handler, %function
UsageFault_Handler:
    B       .
    .size   UsageFault_Handler, . - UsageFault_Handler

    .weak   SVC_Handler
    .type   SVC_Handler, %function
SVC_Handler:
    B       .
    .size   SVC_Handler, . - SVC_Handler

    .weak   DebugMon_Handler
    .type   DebugMon_Handler, %function
DebugMon_Handler:
    B       .
    .size   DebugMon_Handler, . - DebugMon_Handler

    .weak   PendSV_Handler
    .type   PendSV_Handler, %function
PendSV_Handler:
    B       .
    .size   PendSV_Handler, . - PendSV_Handler

    .weak   SysTick_Handler
    .type   SysTick_Handler, %function
SysTick_Handler:
    B       .
    .size   SysTick_Handler, . - SysTick_Handler

    .end

