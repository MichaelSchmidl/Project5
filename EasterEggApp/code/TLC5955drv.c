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

#define TLC5855_SHIFT_REGISTER_BIT_LEN      769    // see TLC datasheet
#define TLC5955_SHIFT_REGISTER_STREAM_LEN   ( (TLC5855_SHIFT_REGISTER_BIT_LEN + 7) / 8 )      // usually + 1, but we need some overhead to work at the last bits
#define TLC5955_DATA_SHIFT_REG_FIRST_GS_PWM_BIT 767     // GS PWM bit offset position, each port 16bits
#define TLC5955_DATA_SHIFT_REG_FIRST_LSD_BIT    672     // LED short detection bit offset position
#define TLC5955_DATA_SHIFT_REG_FIRST_LOD_BIT    720     // LED open detection bit offset position
#define BITS_PER_BYTE                             8
#define GS_PWM_BITS                              16
static uint8_t  m_u8DataShiftStreamIn[TLC5955_SHIFT_REGISTER_STREAM_LEN];
static uint8_t  m_u8DataShiftStreamOut[TLC5955_SHIFT_REGISTER_STREAM_LEN];

static uint8_t SPItransferDone = 1;

#define STREAM_LATCH_WAIT_MS  ( 2 )

#define TLC_LATCH_SELECT_BIT_POS  768
#define TLC_LATCH_SELECT_BIT_LEN  1

#define TLC_DECODER_BIT_POS  767
#define TLC_DECODER_BIT_LEN  8

#define TLC_FC_BIT_POS  370
#define TLC_FC_BIT_LEN  5

#define TLC_BC_BIT_POS  365
#define TLC_BC_BIT_LEN  21

#define TLC_MC_BIT_POS  344
#define TLC_MC_BIT_LEN  9

#define TLC_DC_BIT_POS  335
#define TLC_DC_BIT_LEN  7

#define BITS_PER_BYTE   8

#define _BITS_PER_BYTE              8U   //!< how many bits are in one byte
#define _N_DAISYCHAINED_TLCS        1U   //!< how many TLCs are daisy-chained in our hardware
#define _TLC_BITSTREAM_LENGTH       769U //!< bit stream length for one TLC as given by the datasheet
#define _CHANNELS_PER_TLC           48U  //!< number of channels for each TLC
#define _ERRORS_PER_CHANNEL         2U   //!< LED Open Detection (LOD) and LED Short Detection (LSD) uses one bit each
#define _ERRORS_PER_TLC             ( _CHANNELS_PER_TLC * _ERRORS_PER_CHANNEL ) //!< number of error bits for one TLC
#define _NECESSARY_STUFFING_BITS    ( _BITS_PER_BYTE - _N_DAISYCHAINED_TLCS ) //!< necessary stuffing bits in first stream byte because we have a couple of TLCs in chain

/** calculate how many bytes we need to store the complete bit stream for all TLCs in memory */
#define _TLC_DMA_BUFFER_SIZE_IN_BYTES  ( ( _N_DAISYCHAINED_TLCS * ( _TLC_BITSTREAM_LENGTH / _BITS_PER_BYTE ) ) + 1U )
/** calculate how many 32bit values we need to store the complete bit stream for all TLCs in memory */
#define _TLC_DMA_BUFFER_SIZE_IN_LONGS  ( ( _TLC_DMA_BUFFER_SIZE_IN_BYTES + ( sizeof(uint32_t) - 1U ) ) / sizeof(uint32_t) )

/** how many PWM channels do we have in total */
#define _TOTAL_TLC_CHANNEL_NUMBERS  (_CHANNELS_PER_TLC * _N_DAISYCHAINED_TLCS)

/** bit position in the TLC stream for the LATCH bit (see TLC5955 documentation for more information) */
#define _LATCH_SELECT_BIT           768U

/** bit position in the TLC stream where the command DECODER bits start (see TLC5955 documentation for more information) */
#define _TLC_DECODER_BITSTART       760U
#define _TLC_DECODER_BITLENGTH      ( ( 767U - _TLC_DECODER_BITSTART ) + 1U) //!< number of bits for DECODER

/** bit position in the TLC stream where the FUNCTION CONTROL bits start (see TLC5955 documentation for more information) */
#define _TLC_FC_BITSTART            366U
#define _TLC_FC_BITLENGTH           ( ( 370U - _TLC_FC_BITSTART ) + 1U) //!< number of bits for FC

/** bit position in the TLC stream where the BRIGHTNESS CONTROL bits start (see TLC5955 documentation for more information) */
#define _TLC_BC_BITSTART            345U
#define _TLC_BC_BITLENGTH           ( ( 365U - _TLC_BC_BITSTART ) + 1U) //!< number of bits for BC

/** bit position in the TLC stream where the MAXIMUM CURRENT bits start (see TLC5955 documentation for more information) */
#define _TLC_MC_BITSTART            336U //!< to be commented
#define _TLC_MC_BITLENGTH           ( ( 344U - _TLC_MC_BITSTART ) + 1U) //!< number of bits for MC

/** bit position in the TLC stream where the DOT CORRECTION bits start (see TLC5955 documentation for more information) */
#define _TLC_DC_BITSTART            0U
#define _TLC_DC_VALUELENGTH         7U //!< number of bits per dot correction value
#define _TLC_DC_N_VALUES            _CHANNELS_PER_TLC //!< number of dot correction values

#define _BITS_PER_LONG              ( _BITS_PER_BYTE * sizeof(uint32_t) )   //!< how many bits are in one 32bit value


// function prototype
void SPI0_Master_CallBack(uint32_t event);
static int32_t _writeSetup( void );
static void _fillTlcControlDataLatch( void );
static void _writeStreamBits( uint8_t *      pu8DataShiftStreamIn,
                                     const uint32_t u32BitPos,
                                     const uint32_t u32Value,
                                     const uint8_t  u8BitLen );
static void _writeStreamBit( uint8_t *      pu8DataShiftStreamIn,
                                    const uint32_t u32BitPos,
                                    const uint8_t  u8BitValue );


/*!************************************************************************************************************************************************************
 *
 *************************************************************************************************************************************************************/

static int32_t _writeSetup( void )
{
	int32_t ret = -1;
    memset( m_u8DataShiftStreamIn, 0x0, TLC5955_SHIFT_REGISTER_STREAM_LEN );

    _fillTlcControlDataLatch();

    // transmit setup data twice
    ret = TLC5955drv_refresh();

//!TODO wait for transfer done    vTaskDelay( pdMS_TO_TICKS( STREAM_LATCH_WAIT_MS ) );

    ret = TLC5955drv_refresh();

//!TODO wait for transfer done    vTaskDelay( pdMS_TO_TICKS( STREAM_LATCH_WAIT_MS ) );

    return ret;
}

/*!************************************************************************************************************************************************************
 *
 *************************************************************************************************************************************************************/

static void _fillTlcControlDataLatch( void )
{
    _writeStreamBits( m_u8DataShiftStreamIn,
                     TLC_LATCH_SELECT_BIT_POS, 0x1UL, TLC_LATCH_SELECT_BIT_LEN );
    // the DECODER bytes must be set to 0x96 for setup data
    _writeStreamBits( m_u8DataShiftStreamIn,
                     TLC_DECODER_BIT_POS, 0x96UL, TLC_DECODER_BIT_LEN );
    // set the FC bits (all functions enabled)
    _writeStreamBits( m_u8DataShiftStreamIn,
                     TLC_FC_BIT_POS, 0x1FUL, TLC_FC_BIT_LEN );
    // set the BC bits - full brightness
    _writeStreamBits( m_u8DataShiftStreamIn,
                     TLC_BC_BIT_POS, 0x1FFFFFUL, TLC_BC_BIT_LEN );
    // set the MC bits -  3mA which is binary 000. Therefore the value is binary 0 0000 0000 = 0x000
    // set the MC bits - 19mA which is binary 100. Therefore the value is binary 1 0010 0100 = 0x124
    // set the MC bits - 24mA which is binary 101. Therefore the value is binary 1 0110 1101 = 0x16D
    _writeStreamBits( m_u8DataShiftStreamIn,
                     TLC_MC_BIT_POS, 0x124L, TLC_MC_BIT_LEN );

    // set DC bits - dot correction - for all channels of the given TLC5955
    int nBitPos = TLC_DC_BIT_POS;
    for ( int i = 0; i < tlcPortNum; i++, nBitPos -= TLC_DC_BIT_LEN )
    {
        // full brightness
        _writeStreamBits( m_u8DataShiftStreamIn,
                         nBitPos, 0x7FUL, TLC_DC_BIT_LEN );
    }
}


static void _writeStreamBits( uint8_t *      pu8DataShiftStreamIn,
                                     const uint32_t u32BitPos,
                                     const uint32_t u32Value,
                                     const uint8_t  u8BitLen )
{
    uint32_t u32ShiftValue = u32Value;

    for ( int i = 0; i < u8BitLen; i++ )
    {
        _writeStreamBit( pu8DataShiftStreamIn,
                        u32BitPos - u8BitLen + i + 1,
                        u32ShiftValue & 0x1 );
        u32ShiftValue >>= 1;
    }
}

/*!************************************************************************************************************************************************************
 *
 *************************************************************************************************************************************************************/
static void _writeStreamBit( uint8_t *      pu8DataShiftStreamIn,
                                    const uint32_t u32BitPos,
                                    const uint8_t  u8BitValue )
{
    uint32_t u32InverseBitPos = TLC5855_SHIFT_REGISTER_BIT_LEN - 1 - u32BitPos + 7;
    uint32_t u32ByteBitPos    = BITS_PER_BYTE - ( u32InverseBitPos % BITS_PER_BYTE ) - 1;
    uint32_t u32BytePos       = u32InverseBitPos / BITS_PER_BYTE;
    uint8_t  u8StreamValue    = ( u8BitValue << u32ByteBitPos );
    uint8_t  u8BitMask        = ( 1 << u32ByteBitPos );

    pu8DataShiftStreamIn[u32BytePos] &= ~u8BitMask;
    pu8DataShiftStreamIn[u32BytePos] |= u8StreamValue;
}


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
        	SPItransferDone = 1;
        }
        break;

        /* Data lost event */
        case ARM_SPI_EVENT_DATA_LOST:
        {
            /* Abort current transfer */
            spi0->Control(ARM_SPI_ABORT_TRANSFER, 0);
        	SPItransferDone = 1;
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
	while ( !SPItransferDone );
	SPItransferDone = 0;
	int32_t ret = ARM_DRIVER_ERROR;
    /* Activate SSEL line and start transfer on SPI0/master */
    ret = spi0->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
    if ( ARM_DRIVER_OK == ret )
    {
        ret = spi0->Transfer(m_u8DataShiftStreamOut, m_u8DataShiftStreamIn, TLC5955_SHIFT_REGISTER_STREAM_LEN);
    }
    return ret;
}


void TLC5955drv_start( void )
{
	_startGSCLK();
	_writeSetup();
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
