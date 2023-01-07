//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// rotaryencoder.c -- Decode rotary encoder inputs
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: mjneri
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: rotaryencoder.c
// Description: Makes use of two gated timers for debouncing and two CLCs for
//              decoding the debounced inputs.
//
// Revisions:
// Revision 0.01 - File Created (2022-12-10)
// Additional Comments:
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// *****************************************************************************
// *****************************************************************************
// Section: Header Includes
// *****************************************************************************
// *****************************************************************************

#include "rotaryencoder.h"

// *****************************************************************************
// *****************************************************************************
// Section: Variables
// *****************************************************************************
// *****************************************************************************

rot_ring_buf_t rotData;

static volatile uint16_t rotCntCCW = 0;
static volatile uint16_t rotCntCW = 0;

// Variables used for measuring velocity
static volatile uint16_t prevCntCCW = 0;
static volatile uint16_t prevCntCW = 0;
static volatile uint64_t vel_t0 = 0;

// *****************************************************************************
// *****************************************************************************
// Section: Local Function Defines
// *****************************************************************************
// *****************************************************************************

static int ROTENC_WriteRingBuf(ROT_DIR dir)
{
    if(rotData.isFull)
    {
        // Ring buffer is full. Do nothing
        return -1;
    }
    else
    {
        // Write to the ring buffer
        if(dir == ROTENC_CW)
        {
            rotData.buffer[rotData.wrIndex] |= 1 << rotData.wrBitIndex;
        }
        else
        {
            rotData.buffer[rotData.wrIndex] &= ~(1 << rotData.wrBitIndex);
        }
        
        rotData.wrBitIndex = (rotData.wrBitIndex + 1) % 8;
        
        // if wrBitIndex has circled back to 0, increment the wrIndex.
        if(rotData.wrBitIndex == 0)
        {
            rotData.wrIndex = (rotData.wrIndex + 1) % ROTENC_RINGBUF_MAXSIZE;
        }
        
        // Since 1 byte has been written, the buffer is no longer empty
        rotData.isEmpty = false;
        rotData.dataAmt++;
        
        // Check if the buffer is now full after writing
        if(rotData.dataAmt == ROTENC_RINGBUF_DATA_MAX)
        {
            rotData.isFull = true;
        }
    }
    
    return 0;
}

// *****************************************************************************
// *****************************************************************************
// Section: Callback Functions
// *****************************************************************************
// *****************************************************************************

// TIMER CALLBACKS
void RotClkDebounce(void)
{
    if(!RC6_GetValue())
    {
        DEB_ROT_A_SetLow();
    }
    else
    {
        DEB_ROT_A_SetHigh();
    }
}

void RotDTDebounce(void)
{
    if(!RD0_GetValue())
    {
        DEB_ROT_B_SetLow();
    }
    else
    {
        DEB_ROT_B_SetHigh();
    }
}

// CLC CALLBACKS
void CLC2_Callback(void)
{
    if(CLC2_OutputStatusGet())
    {
        rotCntCCW++;
        ROTENC_WriteRingBuf(ROTENC_CCW);
    }
}

void CLC3_Callback(void)
{
    if(CLC3_OutputStatusGet())
    {
        rotCntCW++;
        ROTENC_WriteRingBuf(ROTENC_CW);
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

void ROTENC_Initialize(void)
{
    memset(&rotData, 0, sizeof(rotData));
    rotData.isEmpty = true;
    
    // Register callbacks for rotary encoder debouncing
    // Note: TMR4 and TMR6 period is 1ms
    TMR4_SetInterruptHandler(RotClkDebounce);
    TMR6_SetInterruptHandler(RotDTDebounce);
    
    vel_t0 = millis();
    
    return;
}

ROT_DIR ROTENC_ReadRingBuf(void)
{
    ROT_DIR retData = ROTENC_ERR;
    
    if(rotData.isEmpty)
    {
        // Ring buffer is empty; do nothing
    }
    else
    {
        // Ring buffer contains some data; read some data
        retData = (rotData.buffer[rotData.rdIndex] >> rotData.rdBitIndex) & 0x01;
        
        // Advance indices
        rotData.rdBitIndex = (rotData.rdBitIndex + 1) % 8;
        
        // If rdBitIndex has circled back to 0, increment rdIndex
        if(rotData.rdBitIndex == 0)
        {
            rotData.rdIndex = (rotData.rdIndex + 1) % ROTENC_RINGBUF_MAXSIZE;
        }
        
        // Since 1 byte has been read, the buffer is no longer full
        rotData.isFull = false;
        rotData.dataAmt--;
        
        // Check if the buffer is now empty after reading
        if(rotData.dataAmt == 0)
        {
            rotData.isEmpty = true;
        }
    }
    
    return retData;
}

void ROTENC_GetRotationCount(uint16_t *cw_count, uint16_t *ccw_count)
{
    if(cw_count == NULL)
    {
        return;
    }
    if(ccw_count == NULL)
    {
        return;
    }
    
    *cw_count = rotCntCW;
    *ccw_count = rotCntCCW;
    return;
}

uint16_t ROTENC_Velocity(void)
{
    // Compute velocity using total rotation counts only.
    // (Technically we're measuring speed, but the function name is good enough)
    uint16_t rotTotal = rotCntCW + rotCntCCW;
    uint16_t prevRotTotal = prevCntCW + prevCntCCW;
    
    
    // Left shift deltaTime 10 bits to approximately convert ms to s
    // NOTE: We don't need an accurate velocity reading here, we just need a
    // number that represents velocity well enough.
    // NOTE: Add 1 after left shift to make sure we don't div by 0.
    uint16_t deltaTime = ((millis() - vel_t0) >> 10) + 1;
    uint16_t deltaRot = rotTotal - prevRotTotal;
    
    // We don't need a floating value.
    // Approx. unit is turns per s
    uint16_t rotVelocity = deltaRot / deltaTime;
    
    // Before returning, update various variables
    prevCntCW = rotCntCW;
    prevCntCCW = rotCntCCW;
    vel_t0 = millis();
    
    return rotVelocity;
}

/*******************************************************************************
 End of File
*/

