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
 * app_init.c
 * - Application initialization
 * ----------------------------------------------------------------------------
 * $Revision: 1.17 $
 * $Date: 2019/08/27 15:35:04 $
 * ------------------------------------------------------------------------- */

#include "app.h"
#include "RC5receiver.h"
#include "TLC5955drv.h"
#include "i2c_master.h"
#include "PCF8574drv.h"
#include "GYROdrv.h"

/* Application Environment Structure */
struct app_env_tag app_env;

/* ----------------------------------------------------------------------------
 * Function      : void App_Initialize(void)
 * ----------------------------------------------------------------------------
 * Description   : Initialize the system for proper application execution
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void App_Initialize(void)
{
    /* Mask all interrupts */
    __set_PRIMASK(PRIMASK_DISABLE_INTERRUPTS);

    /* Disable all interrupts and clear any pending interrupts */
    Sys_NVIC_DisableAllInt();
    Sys_NVIC_ClearAllPendingInt();

    /* Configure the current trim settings for VCC, VDDA */
    ACS_VCC_CTRL->ICH_TRIM_BYTE = VCC_ICHTRIM_16MA_BYTE;
    ACS_VDDA_CP_CTRL->PTRIM_BYTE = VDDA_PTRIM_16MA_BYTE;

    /* Start 48 MHz XTAL oscillator */
    ACS_VDDRF_CTRL->ENABLE_ALIAS = VDDRF_ENABLE_BITBAND;
    ACS_VDDRF_CTRL->CLAMP_ALIAS = VDDRF_DISABLE_HIZ_BITBAND;

    /* Wait until VDDRF supply has powered up */
    while (ACS_VDDRF_CTRL->READY_ALIAS != VDDRF_READY_BITBAND);

    /* Disable RF TX power amplifier supply voltage and
    * connect the switched output to VDDRF regulator */
    ACS_VDDPA_CTRL->ENABLE_ALIAS = VDDPA_DISABLE_BITBAND;
    ACS_VDDPA_CTRL->VDDPA_SW_CTRL_ALIAS = VDDPA_SW_VDDRF_BITBAND;

    /* Enable RF power switches */
    SYSCTRL_RF_POWER_CFG->RF_POWER_ALIAS = RF_POWER_ENABLE_BITBAND;

    /* Remove RF isolation */
    SYSCTRL_RF_ACCESS_CFG->RF_ACCESS_ALIAS = RF_ACCESS_ENABLE_BITBAND;

    /* Start the 48 MHz oscillator without changing the other register bits */
    RF->XTAL_CTRL = ((RF->XTAL_CTRL & ~XTAL_CTRL_DISABLE_OSCILLATOR) |
                     XTAL_CTRL_REG_VALUE_SEL_INTERNAL);

    /* Enable the 48 MHz oscillator divider using the desired prescale value */
    RF_REG2F->CK_DIV_1_6_CK_DIV_1_6_BYTE = CK_DIV_1_6_PRESCALE_6_BYTE;

    /* Wait until 48 MHz oscillator is started */
    while (RF_REG39->ANALOG_INFO_CLK_DIG_READY_ALIAS !=
           ANALOG_INFO_CLK_DIG_READY_BITBAND);

    /* Switch to (divided 48 MHz) oscillator clock */
    Sys_Clocks_SystemClkConfig(JTCK_PRESCALE_1   |
                               EXTCLK_PRESCALE_1 |
                               SYSCLK_CLKSRC_RFCLK);

    /* Configure clock dividers */
    CLK->DIV_CFG0 = (SLOWCLK_PRESCALE_8 | BBCLK_PRESCALE_1 |
                     USRCLK_PRESCALE_1);
    CLK->DIV_CFG1 = (PWM0CLK_PRESCALE_2_BYTE);
    CLK->DIV_CFG2 = (CPCLK_PRESCALE_8 | DCCLK_PRESCALE_2);

    BBIF->CTRL = (BB_CLK_ENABLE | BBCLK_DIVIDER_8 | BB_WAKEUP);

    /* Configure ADC channel 0 to measure VBAT/2 */
    Sys_ADC_Set_Config(ADC_VBAT_DIV2_NORMAL | ADC_NORMAL |
                       ADC_PRESCALE_6400);
    Sys_ADC_InputSelectConfig(0,
                              (ADC_NEG_INPUT_GND |
                               ADC_POS_INPUT_VBAT_DIV2));

    // enable DIO13/DIO14/DIO15 as GPIOs
    DIO_JTAG_SW_PAD_CFG->CM3_JTAG_TRST_EN_ALIAS = CM3_JTAG_TRST_DISABLED_BITBAND;
    DIO_JTAG_SW_PAD_CFG->CM3_JTAG_DATA_EN_ALIAS = CM3_JTAG_DATA_DISABLED_BITBAND;

    /* Configure DIOs */
    Sys_DIO_Config(POWER_ON_DIO, DIO_MODE_GPIO_OUT_1);
    Sys_DIO_Config(RECOVERY_FOTA_DEBUG_DIO, DIO_MODE_GPIO_OUT_0);
    Sys_DIO_Config(RC5_DIO_NUM,  DIO_MODE_GPIO_IN_1 | DIO_STRONG_PULL_UP | DIO_LPF_DISABLE);
    Sys_DIO_Config(TOUCH_IRQ_DIO_NUM,  DIO_MODE_GPIO_IN_0 | DIO_STRONG_PULL_UP | DIO_LPF_DISABLE);

    /* Initialize the baseband and BLE stack */
    BLE_Initialize();
	
	/* Set radio output power of RF */
    Sys_RFFE_SetTXPower(OUTPUT_POWER_DBM);

    /* Initialize environment */
    App_Env_Initialize();
    /* Initialize gpio structure */
    gpio = &Driver_GPIO;

    /* Initialize gpio driver */
    gpio->Initialize(GPIOirq_EventCallback);

    /* Initialize i2c driver */
    I2C_init();
    PCF8574_write(0xFF); // set all pins to input
    GYRO_init();

    // setup out own hardware components
    RC5_init();
    TLC5955drv_init();


    NVIC_DisableIRQ(DIO1_IRQn);

    /* Stop masking interrupts */
    __set_PRIMASK(PRIMASK_ENABLE_INTERRUPTS);
    __set_FAULTMASK(FAULTMASK_ENABLE_INTERRUPTS);

    /* Enable interrupts */
    NVIC_EnableIRQ(DIO0_IRQn);
}

/* ----------------------------------------------------------------------------
 * Function      : void App_Env_Initialize(void)
 * ----------------------------------------------------------------------------
 * Description   : Initialize application environment
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void App_Env_Initialize(void)
{
    /* Reset the application manager environment */
    memset(&app_env, 0, sizeof(app_env));

    /* Create the application task handler */
    ke_task_create(TASK_APP, &TASK_DESC_APP);

    /* Initialize the device info service environment */
    Diss_Env_Initialize();

    /* Initialize the battery service server environment */
    Bass_Env_Initialize();

    /* Initialize the HID over GATT service server environment */
    Hogpd_Env_Initialize();
}
