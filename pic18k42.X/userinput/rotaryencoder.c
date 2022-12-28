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

/*******************************************************************************
 End of File
*/

