/*
 * TLC5955drv.c
 *
 *  Created on: 19.01.2021
 *      Author: mschmidl
 */

#include "TLC5955drv.h"
#include "app.h"
#include <RTE_Device.h>
#include <PWM_RSLxx.h>
#include <GPIO_RSLxx.h>
#include <SPI_RSLxx.h>

#define GSCLK_PWM_NUM PWM_0

static DRIVER_PWM_t *pwm;
extern DRIVER_PWM_t Driver_PWM;

static ARM_DRIVER_SPI *spi0;
extern ARM_DRIVER_SPI Driver_SPI0;

uint8_t spi0_tx_buffer[] __attribute__ ((aligned(4))) = "RSL10 SPI TEST";
uint8_t spi1_tx_buffer[] __attribute__ ((aligned(4))) = "RSL10 SPI TEST";
uint8_t spi0_rx_buffer[sizeof(spi0_tx_buffer)] __attribute__ ((aligned(4)));
uint8_t spi1_rx_buffer[sizeof(spi0_tx_buffer)] __attribute__ ((aligned(4)));
size_t buff_size = sizeof(spi0_tx_buffer);

/* ----------------------------------------------------------------------------
 * Function      : void SPI0_Master_CallBack(uint32_t event)
 * ----------------------------------------------------------------------------
 * Description   : This function is a callback registered by the function
 *                 Initialize, for SPI0. The parameter event indicates one or
 *                 more events that occurred during driver operation.
 * Inputs        : event    -    SPI Events notification mask
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void SPI0_Master_CallBack(uint32_t event)
{
    /* Check event */
    switch (event)
    {
        /* Transfer complete event */
        case ARM_SPI_EVENT_TRANSFER_COMPLETE:
        {
            /* Deactivate SSEL line */
            spi0->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
        }
        break;

        /* Data lost event */
        case ARM_SPI_EVENT_DATA_LOST:
        {
            /* Abort current transfer */
            spi0->Control(ARM_SPI_ABORT_TRANSFER, 0);
        }
        break;
    }
}


static void _initGSCLK( void )
{
    pwm = &Driver_PWM;
    pwm->Initialize();
    pwm->SetDutyCycle(GSCLK_PWM_NUM, 50);
}


static void _initSPI( void )
{
    /* Initialize spi0 driver structure */
    spi0 = &Driver_SPI0;

    /* Initialize spi0, register callback function */
    spi0->Initialize(SPI0_Master_CallBack);
}


void TLC5955drv_init( void )
{
	_initGSCLK();
	_initSPI();

	pwm->Start(GSCLK_PWM_NUM);
}
