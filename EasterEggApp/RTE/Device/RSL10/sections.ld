/* ----------------------------------------------------------------------------
 * Copyright (c) 2019 Semiconductor Components Industries, LLC (d/b/a ON
 * Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.

 * ----------------------------------------------------------------------------
 * fota_sections.ld
 * - Simple linker script for FOTA applications
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Memory linker description
 * ------------------------------------------------------------------------- */
MEMORY
{
  ROM  (r) : ORIGIN = 0x00000000, LENGTH = 4K
  FLASH (xrw) : ORIGIN = 0x00100000, LENGTH = 380K /* netto minus 160KB f. Bootloader(8K) und FOTA-Lib(152K) */
  PRAM (xrw) : ORIGIN = 0x00200000, LENGTH = 32K

  DRAM (xrw) : ORIGIN = 0x20000000, LENGTH = 70K /* minus 13,3KByte lost because of __app_ram_start=0x2000352C */
/*  DRAM_DSP (xrw) : ORIGIN = 0x20008000, LENGTH = 40K*/ /* seems to be unused, so we use it in the app as DRAM */
  DRAM_BB (xrw) : ORIGIN = 0x20012000, LENGTH = 16K
}

/* ----------------------------------------------------------------------------
 * Stack related defines and provided variables
 * ------------------------------------------------------------------------- */
__stack = ORIGIN(DRAM) + LENGTH(DRAM);

PROVIDE ( __stack = __stack ) ;

/*
 * Default stack sizes.
 * These are used by the startup in order to allocate stacks
 * for the different modes.
 */

__Main_Stack_Size = 1024 ;
PROVIDE ( _Main_Stack_Size = __Main_Stack_Size ) ;

__Main_Stack_Limit = __stack  - __Main_Stack_Size ;
PROVIDE ( _Main_Stack_Limit = __Main_Stack_Limit ) ;

/* ----------------------------------------------------------------------------
 * Heap related defines and provided variables
 * ------------------------------------------------------------------------- */
PROVIDE ( __Heap_Begin__ = __noinit_end__ ) ;
PROVIDE ( __Heap_Limit__ = __stack - __Main_Stack_Size ) ;

/*
 * The entry point is informative, for debuggers and simulators,
 * since the Cortex-M vector points to it anyway.
 */
ENTRY(Reset_Handler)

/* 
 * As is the VTOR register, we refer to it in startup documentation
 */
__VTOR = 0xE000ED08;

/* ----------------------------------------------------------------------------
 * Section definitions
 * ------------------------------------------------------------------------- */
SECTIONS
{
    /*
     * For Cortex-M devices, the beginning of the startup code is stored in
     * the .interrupt_vector section, which goes to FLASH
     */
    __rom_start = DEFINED(__app_rom_start) ? __app_rom_start : ORIGIN(FLASH);
    __image_size = __data_init__ + SIZEOF(.data) - __rom_start;
    .text __rom_start :
    {
        . = ALIGN(4);
        KEEP(*(.interrupt_vector))

        /*
         * This section is here to store the startup code immediately after
         * the interrupt vectors, as required by the program ROM.
         */
        *(.reset)
        
        /*
         * FOTA BootLoader descriptor
         */
        *(.rodata.fota.image-size)
        KEEP(*(.rodata.fota.build-id))
        
        /*
         * FOTA version descriptor
         */
        *(.rodata.boot.version)

        /* Pre-initialization Code */
        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start__ = .);

        /* System initialization and the platform initialization (if present)
         * should be first */
        KEEP(*(.preinit_array_sysinit .preinit_array_sysinit.*))
        KEEP(*(.preinit_array_platform .preinit_array_platform.*))

        /* Pre-initialization functions (to be executed before C++
         * constructors are run) */
        KEEP(*(.preinit_array .preinit_array.*))

        PROVIDE_HIDDEN (__preinit_array_end__ = .);

        /* Initialization Code */
        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start__ = .);

        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))

        PROVIDE_HIDDEN (__init_array_end__ = .);

        /*
         * The program code is stored in the .text section,
         * which goes to FLASH.
         */
        . = ALIGN(4);

        *(.text .text.*)            /* all remaining code */
        *(.rodata .rodata.*)        /* read-only data (constants) */

        . = ALIGN(4);
        __dsp_start__ = . ;
        KEEP(*(.dsp .dsp.*))        /* all remaining DSP code */
        __dsp_end__ = . ;

        . = ALIGN(4);
    } >FLASH

    /*
     * This address is used by the startup code to
     * initialize the .data section.
     */
    . = ALIGN(4);
    __data_init__ = .;

    /* Place the SystemClock variable needed for CMSIS in a place that is
     * compatible with the ROM's placement of this variable so that the
     * variable can be used by CMSIS and the ROM's flash write libary */
    .systemclock (NOLOAD) :
    {
        . = ALIGN(4);
        KEEP(*(.systemclock))
    } > DRAM

    /*
     * The initialized data section.
     * The program executes knowing that the data is in the RAM
     * but the loader puts the initial values in the FLASH (inidata).
     * It is one task of the startup to copy the initial values from
     * FLASH to RAM.
     */
    __ram_start = DEFINED(__app_ram_start) ? __app_ram_start : .;
    .data __ram_start : AT ( __data_init__ )
    {
        . = ALIGN(4);

        /* This is used by the startup code to initialize the .data section */
        __data_start__ = . ;
        *(.data_begin .data_begin.*)
        *(.data .data.*)
        *(.data_end .data_end.*)
        . = ALIGN(4);

        /* This is used by the startup code to initialize the .data section */
        __data_end__ = . ;
    } >DRAM

    /*
     * The uninitialized data section. NOLOAD is used to avoid
     * the "section `.bss' type changed to PROGBITS" warning
     */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss_begin .bss_begin.*)

        *(.bss .bss.*)
        *(COMMON)

        *(.bss_end .bss_end.*)
        . = ALIGN(4);
        __bss_end__ = .;
    } >DRAM

    .noinit (NOLOAD) :
    {
        . = ALIGN(4);
        __noinit_start__ = .;

        *(.noinit .noinit.*)

         . = ALIGN(4) ;
        __noinit_end__ = .;
    } > DRAM
    
    /* Check if there is enough space to allocate the main stack */
    ._stack (NOLOAD) :
    {
        . = ALIGN(4);
        
        . = . + __Main_Stack_Size ;
        
        . = ALIGN(4);
    } >DRAM
}
