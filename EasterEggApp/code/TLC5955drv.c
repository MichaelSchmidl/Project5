/*
 * TLC5955drv.c
 *
 *  Created on: 19.01.2021
 *      Author: mschmidl
 */

#include "TLC5955drv.h"
#include "app.h"
#include <RTE_Device.h>
#include <GPIO_RSLxx.h>
#include <SPI_RSLxx.h>

static ARM_DRIVER_SPI *spi0;
extern ARM_DRIVER_SPI Driver_SPI0;

uint8_t spi0_tx_buffer[] __attribute__ ((aligned(4))) = "EasterEgg_KBD TEST";
uint8_t spi0_rx_buffer[sizeof(spi0_tx_buffer)] __attribute__ ((aligned(4)));
size_t buff_size = sizeof(spi0_tx_buffer);

// function prototype
void SPI0_Master_CallBack(uint32_t event);


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


static void _startGSCLK( void )
{
    Sys_DIO_Config(GSCLK_DIO_NUM, DIO_MODE_SYSCLK | DIO_WEAK_PULL_UP | DIO_LPF_DISABLE);
}


static void _stopGSCLK( void )
{
    Sys_DIO_Config(GSCLK_DIO_NUM, DIO_MODE_GPIO_OUT_0 | DIO_WEAK_PULL_UP | DIO_LPF_DISABLE);
}


static void _initSPI( void )
{
    /* Initialize spi0 driver structure */
    spi0 = &Driver_SPI0;

    /* Initialize spi0, register callback function */
    spi0->Initialize(SPI0_Master_CallBack);
}


int32_t TLC5955drv_refresh( void )
{
	int32_t ret = ARM_DRIVER_ERROR;
    /* Activate SSEL line and start transfer on SPI0/master */
    ret = spi0->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
    if ( ARM_DRIVER_OK == ret )
    {
        ret = spi0->Transfer(spi0_tx_buffer, spi0_rx_buffer, buff_size);
    }
    return ret;
}


void TLC5955drv_start( void )
{
	_startGSCLK();
}


void TLC5955drv_stop( void )
{
	_stopGSCLK();
}


void TLC5955drv_init( void )
{
	_initSPI();
	_stopGSCLK();
}
