//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// sh1106.c -- Driver for the SH1106 OLED Driver using I2C
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: mjneri
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: sh1106.c
// Description:
//
// Revisions:
// Revision 0.01 - File Created (2021-12-30)
// Additional Comments:
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


// *****************************************************************************
// *****************************************************************************
// Section: Header Includes
// *****************************************************************************
// *****************************************************************************

#include "sh1106.h"

// *****************************************************************************
// *****************************************************************************
// Section: Variables
// *****************************************************************************
// *****************************************************************************

// Local variables
// Primary reference for the initialization commands:
//  + https://github.com/notisrac/SH1106Lib/blob/master/SH1106Lib.h
static const SH1106_CMD OLED_InitCommands[] =
{
    SH1106_COMMANDBYTE,         // All subsequent bytes are command bytes
    
    DISPLAYOFF,
    SETDISPLAYCLOCKDIV,
    0x80,
    SETMULTIPLEX,
    0x3F,
    SETDISPLAYOFFSET,
    0x00,
    
    (SETSTARTLINE | 0x0),
    CHARGEPUMP,
    0x10,
    (SEGREMAP | 0x01),
    COMSCANDEC,
    SETCOMPINS,
    0x12,
    SETCONTRAST,
    0x9F,
    SETPRECHARGE,
    0x22,
    SETVCOMDETECT,
    0x40,
    DISPLAYALLON_RESUME,
    NORMALDISPLAY,
    DISPLAYON,
    
    (SETCOLUMNADDRLOW | 0x00),
    (SETCOLUMNADDRHIGH | 0x00),
    (SETSTARTLINE | 0x00)
};

// Private global variables to store ring buffer entries; implemented as a workaround to error 712
// (a.k.a. can't generate code for this expression)
static oled_op_queue_t gWriteBuf = {0};
static oled_op_queue_t gReadBuf = {0};

// Determines if an operation has completed
static volatile bool isOpComplete = false;

// Ring buffer
static oled_ring_buf_t oledData;

// State machine variable
static OLED_STATE oledState;

// Extern Variables
// ...

// *****************************************************************************
// *****************************************************************************
// Section: Callback Functions
// *****************************************************************************
// *****************************************************************************

static i2c1_operations_t OpCompleteHandler(void *data)
{
    isOpComplete = true;
    
    // Reset data transfer completion callback to NULL
    I2C1_SetDataCompleteCallback(NULL, NULL);
    
    // Stop the I2C transfer
    return I2C1_STOP;
}

// *****************************************************************************
// *****************************************************************************
// Section: Local functions
// *****************************************************************************
// *****************************************************************************

static inline void resetRingBuf(void)
{
    memset((void *)(oledData.buffer), 0, sizeof(oledData.buffer));
    oledData.rdIndex = 0;
    oledData.wrIndex = 0;
    oledData.isFull = false;
    oledData.isEmpty = true;
    oledData.dataAmt = 0;
}

static void readRingBuf(void)
{   
    if(oledData.isEmpty)
    {
        // Ring buffer is empty, do nothing
    }
    else
    {
        // Ring buffer contains some data.
        gReadBuf = oledData.buffer[oledData.rdIndex];
        oledData.rdIndex = (oledData.rdIndex + 1) % RINGBUF_MAXSIZE;
        
        // Since 1 byte has been read, the buffer is no longer full
        oledData.isFull = false;
        oledData.dataAmt--;
        
        // Check if the buffer is now empty after reading
        if(oledData.dataAmt == 0)
        {
            oledData.isEmpty = true;
        }
    }
    
    return;
}

static int writeRingBuf(void)
{
    if(oledData.isFull)
    {
        // Ring buffer is full. Do nothing
        return -1;
    }
    else
    {
        // Write to the ring buffer
        oledData.buffer[oledData.wrIndex] = gWriteBuf;
        oledData.wrIndex = (oledData.wrIndex + 1) % RINGBUF_MAXSIZE;
        
        // Since 1 byte has been written, the buffer is no longer empty
        oledData.isEmpty = false;
        oledData.dataAmt++;
        
        // Check if the buffer is now full after writing
        if(oledData.dataAmt == RINGBUF_MAXSIZE)
        {
            oledData.isFull = true;
        }
    }
    
    return 0;
}

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

void OLED_Initialize(void)
{   
    resetRingBuf();
    oledState = OLED_INIT;
    
    // Set callback functions NULL
    I2C1_SetDataCompleteCallback(NULL, NULL);
    I2C1_SetAddressNackCallback(NULL, NULL);
    I2C1_SetDataNackCallback(NULL, NULL);
    I2C1_SetTimeoutCallback(NULL, NULL);
    
    // Open the I2C Bus
    I2C1_Open(SH1106_I2C_ADDR);
    
    I2C1_SetBuffer(OLED_InitCommands, sizeof(OLED_InitCommands));
    I2C1_SetDataCompleteCallback(OpCompleteHandler, NULL);
    I2C1_MasterWrite();                     // Initialize the I2C Write.
    while(I2C1_BUSY == I2C1_Close());       // Wait until the I2C bus can be closed
    
    return;
}


int OLED_Send(uint8_t *buffer, size_t bufSize)
{   
    // Insert data into the ring buffer
    gWriteBuf.bufSize = bufSize;
    memcpy(gWriteBuf.dBuf, buffer, bufSize);
    gWriteBuf.readOp = false;
    
    return writeRingBuf();
    
}

// NOTE: see sh1106.h for comments about OLED_Recv
int OLED_Recv(uint8_t *buffer, size_t bufSize)
{
    // Insert data into the ring buffer
    gReadBuf.bufSize = bufSize;
    memcpy(gReadBuf.dBuf, buffer, bufSize);
    gReadBuf.readOp = true;
    
    return writeRingBuf();
}

int OLED_SetCursor(uint8_t row, uint8_t col)
{   
    // Insert data into the ring buffer
    uint8_t command[] = {SH1106_COMMANDBYTE, \
                        (PAGEADDR | (row >> 3)),               \
                        (SETCOLUMNADDRLOW | ((col + SH1106_SEGOFFSET) & 0x0F)), \
                        (SETCOLUMNADDRHIGH | (col + SH1106_SEGOFFSET) >> 4)};
    
    gWriteBuf.bufSize = 4;
    memcpy(gWriteBuf.dBuf, command, sizeof(command));
    gWriteBuf.readOp = false;
    
    return writeRingBuf();
}

bool OLED_IsBusy(void)
{
    if((oledState == OLED_WAIT_FOR_OP) || (oledState == OLED_INIT))
    {
        return false;
    }
    else
    {
        return true;
    }
}

void OLED_Tasks(void)
{
    oled_op_queue_t smData = {0};
    
    switch(oledState)
    {
        case OLED_INIT:
        {
            oledState = OLED_WAIT_FOR_OP;
            break;
        }
        
        case OLED_WAIT_FOR_OP:
        {
            if(!oledData.isEmpty)
            {
                oledState = OLED_START_OP;
            }
            else
            {
                oledState = OLED_WAIT_FOR_OP;
            }
            break;
        }
        
        case OLED_START_OP:
        {     
            // Read from the ring buffer
            readRingBuf();
            smData = gReadBuf;
            
            // Open the I2C bus
            I2C1_Open(SH1106_I2C_ADDR);
            
            // Pass buffer address to I2C PLIB
            I2C1_SetBuffer(smData.dBuf, smData.bufSize);
            
            // Register callback
            I2C1_SetDataCompleteCallback(OpCompleteHandler, NULL);
            
            // Initiate the operation
            if(smData.readOp)
            {
                I2C1_MasterRead();      // See OLED_Recv note in sh1106.h; I added this here for completeness
            }
            else
            {
                I2C1_MasterWrite();
            }
            
            oledState = OLED_CHECK_OP_STAT;
            break;
        }
        
        case OLED_CHECK_OP_STAT:
        {
            if(isOpComplete)
            {
                // reset flag
                isOpComplete = false;
                oledState = OLED_WRAP_UP_OP;
            }
            else
            {
                oledState = OLED_CHECK_OP_STAT;
            }
            break;
        }
        
        case OLED_WRAP_UP_OP:
        {
            // Dont move to next state if the bus is still busy for some reason
            if(I2C1_BUSY == I2C1_Close())
            {
                oledState = OLED_WRAP_UP_OP;
            }
            else
            {
                oledState = OLED_WAIT_FOR_OP;
            }
            
            break;
        }
        
        default:
        {
            break;
        }
    }
    return;
}

/*******************************************************************************
 End of File
*/