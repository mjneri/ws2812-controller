/**
  SPI1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    spi1.c

  @Summary
    This is the generated driver implementation file for the SPI1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides implementations for driver APIs for SPI1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F47K42
        Driver Version    :  1.0.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above or later
        MPLAB             :  MPLAB X 6.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "spi1.h"
#include <xc.h>
#include <string.h>

// User-added code

// Ring buffer "pointer" variables declared as uint16_t
// so max. buffer size is 65535 instead of a measly 255 bytes
static volatile uint16_t spi1TxHead = 0;
static volatile uint16_t spi1TxTail = 0;
volatile uint16_t spi1TxBufferRemaining;

static volatile uint8_t spi1TxBuffer[SPI1_TX_BUFFER_SIZE];
// End of user-added code

void SPI1_DefaultHandler(void);
void SPI1_DefaultRxHandler(void);
void SPI1_DefaultTxHandler(void);

typedef struct { 
    uint8_t con0; 
    uint8_t con1; 
    uint8_t con2; 
    uint8_t baud; 
    uint8_t operation;
} spi1_configuration_t;


//con0 == SPIxCON0, con1 == SPIxCON1, con2 == SPIxCON2, baud == SPIxBAUD, operation == Master/Slave
static const spi1_configuration_t spi1_configuration[] = {   
    { 0x2, 0x40, 0x0, 0x0, 0 },
    { 0x3, 0x40, 0x2, 0x0, 0 }      // Half Duplex transmit only.
};

void SPI1_Initialize(void)
{
    //EN disabled; LSBF MSb first; MST bus slave; BMODE last byte; 
    SPI1CON0 = 0x02;
    //SMP Middle; CKE Idle to active; CKP Idle:High, Active:Low; FST disabled; SSP active high; SDIP active high; SDOP active high; 
    SPI1CON1 = 0x20;
    //SSET disabled; TXR not required for a transfer; RXR data is not stored in the FIFO; 
    SPI1CON2 = 0x00;
    //CLKSEL TMR2_Postscaled; 
    SPI1CLK = 0x05;
    //BAUD 0; 
    SPI1BAUD = 0x00;
    TRISCbits.TRISC0 = 0;
    
    SPI1_SetInterruptHandler(SPI1_DefaultHandler);
    SPI1_SetRxInterruptHandler(SPI1_DefaultRxHandler);
    SPI1_SetTxInterruptHandler(SPI1_DefaultTxHandler);
    
    // User-added - initialize buffer
    spi1TxHead = 0;
    spi1TxTail = 0;
    memset((void *)spi1TxBuffer, 0, sizeof(spi1TxBuffer));
    spi1TxBufferRemaining = sizeof(spi1TxBuffer);
}

bool SPI1_Open(spi1_modes_t spi1UniqueConfiguration)
{
    if(!SPI1CON0bits.EN)
    {
        SPI1CON0 = spi1_configuration[spi1UniqueConfiguration].con0;
        SPI1CON1 = spi1_configuration[spi1UniqueConfiguration].con1;
        SPI1CON2 = spi1_configuration[spi1UniqueConfiguration].con2;
        SPI1BAUD = spi1_configuration[spi1UniqueConfiguration].baud;        
        TRISCbits.TRISC0 = spi1_configuration[spi1UniqueConfiguration].operation;
        SPI1CON0bits.EN = 1;
        return true;
    }
    return false;
}

void SPI1_Close(void)
{
    SPI1CON0bits.EN = 0;
}

// User-added begins here
bool SPI1_IsTXReady(void)
{
    // Return true only when Tx buffer is empty
    return ((spi1TxBufferRemaining == sizeof(spi1TxBuffer)) ? true : false);
}

uint16_t SPI1_GetBufferSize(void)
{
    return spi1TxBufferRemaining;
}

int SPI1_Write(uint8_t txData)
{
    if(0 == spi1TxBufferRemaining)
    {
        return -1;
    }

    if(0 == PIE2bits.SPI1TXIE)
    {
        SPI1TXB = txData;
    }
    else
    {
        PIE2bits.SPI1TXIE = 0;
        spi1TxBuffer[spi1TxHead++] = txData;
        if(sizeof(spi1TxBuffer) <= spi1TxHead)
        {
            spi1TxHead = 0;
        }
        spi1TxBufferRemaining--;
    }
    PIE2bits.SPI1TXIE = 1;
}

void SPI1_TX_ISR(void)
{
    if(sizeof(spi1TxBuffer) > spi1TxBufferRemaining)
    {
       SPI1TXB = spi1TxBuffer[spi1TxTail++];
       if(sizeof(spi1TxBuffer) <= spi1TxTail)
        {
            spi1TxTail = 0;
        }
        spi1TxBufferRemaining++;
    }
    else
    {
        PIE2bits.SPI1TXIE = 0;
    }
}

// User-added ends here

void SPI1_DefaultHandler(void)
{
    // add your SPI1 interrupt custom code
}

void SPI1_SetInterruptHandler(spi1InterruptHandler_t handler)
{
    SPI1_InterruptHandler = handler;
}

void __interrupt(irq(SPI1),base(8)) SPI1_Isr(void)
{
    if(SPI1_InterruptHandler)
    {
        SPI1_InterruptHandler();
    }
}

void SPI1_DefaultRxHandler(void)
{
    // add your SPI1RX interrupt custom code
}

void SPI1_SetRxInterruptHandler(spi1InterruptHandler_t handler)
{
    SPI1_RxInterruptHandler = handler;
}

void __interrupt(irq(SPI1RX),base(8)) SPI1_RxIsr(void)
{
    if(SPI1_RxInterruptHandler)
    {
        SPI1_RxInterruptHandler();
    }
}

void SPI1_DefaultTxHandler(void)
{
    // add your SPI1TX interrupt custom code
    SPI1_TX_ISR();
}

void SPI1_SetTxInterruptHandler(spi1InterruptHandler_t handler)
{
    SPI1_TxInterruptHandler = handler;
}

void __interrupt(irq(SPI1TX),base(8)) SPI1_TxIsr(void)
{
    if(SPI1_TxInterruptHandler)
    {
        SPI1_TxInterruptHandler();
    }
}