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

// Private "global" variable to store buffer size accessible from callback functions
static size_t gBufSize = 0;

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

static oled_op_queue_t readRingBuf(void)
{
    oled_op_queue_t returnData = {0};
    
    if(oledData.isEmpty)
    {
        // Ring buffer is empty, do nothing
    }
    else
    {
        // Ring buffer contains some data.
        returnData = oledData.buffer[oledData.rdIndex];
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
    
    return returnData;
}

static int writeRingBuf(oled_op_queue_t data)
{
    if(oledData.isFull)
    {
        // Ring buffer is full. Do nothing
        return -1;
    }
    else
    {
        // Write to the ring buffer
        oledData.buffer[oledData.wrIndex] = data;
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
    oled_op_queue_t temp = {0};
    temp.bufSize = bufSize;
    temp.dBuf = buffer;
    temp.readOp = false;
    
    return writeRingBuf(temp);
}

// NOTE: refactor the next two functions to make use of the ring buffer.
int OLED_Recv(uint8_t *buffer, size_t bufSize)
{
    // Open the I2C Bus
    I2C1_Open(SH1106_I2C_ADDR);
    
    // Pass buffer address to I2C PLIB
    I2C1_SetBuffer(buffer, bufSize);
    
    I2C1_SetDataCompleteCallback(OpCompleteHandler, NULL);
    
    I2C1_MasterRead();                      // Initiate I2C Read
    //while(I2C1_BUSY == I2C1_Close());       // Wait until the I2C bus can be closed.
    return;                                 // Data can be accessed through the buffer.
}

int OLED_SetCursor(uint8_t row, uint8_t col)
{
    // Open the I2C Bus
//    I2C1_Open(SH1106_I2C_ADDR);
//    
//    // Set cursor position
//    I2C1_SetBuffer((uint8_t[])
//        {SH1106_COMMANDBYTE, \
//         (PAGEADDR | (row >> 3)), \
//         (SETCOLUMNADDRLOW | ((col + SH1106_SEGOFFSET) & 0x0F)), \
//         (SETCOLUMNADDRHIGH | (col + SH1106_SEGOFFSET) >> 4)}, 4);
//        
//    I2C1_SetDataCompleteCallback(OpCompleteHandler, NULL);
//    I2C1_MasterWrite();                     // Initialize the I2C Write.
    //while(I2C1_BUSY == I2C1_Close());       // Wait until the I2C bus can be closed
    
    // Insert data into the ring buffer
    oled_op_queue_t temp = {0};
    temp.bufSize = 4;
    temp.dBuf = (uint8_t[])
        {SH1106_COMMANDBYTE, \
         (PAGEADDR | (row >> 3)), \
         (SETCOLUMNADDRLOW | ((col + SH1106_SEGOFFSET) & 0x0F)), \
         (SETCOLUMNADDRHIGH | (col + SH1106_SEGOFFSET) >> 4)};
    temp.readOp = false;
    
    return writeRingBuf(temp);
}

static int OLED_Stop(void)
{
    if(I2C1_BUSY == I2C1_Close())
    {
        return -1;
    }
    
    return 0;
}

void OLED_Tasks(void)
{
    switch(oledState)
    {
        OLED_INIT:
        {
            // NOTE: Send initialization commands here?
            
            oledState = OLED_WAIT_FOR_OP;
            break;
        }
        
        OLED_WAIT_FOR_OP:
        {
            break;
        }
        
        OLED_START_OP:
        {
            // REgister callback here too.
            // Read from the ring buf, then check for the operation type
            break;
        }
        
        OLED_CHECK_OP_STAT:
        {
            if(isOpComplete)
            {
                // reset flag
                isOpComplete = false;
                oledState = OLED_WRAP_UP_OP;
            }
            break;
        }
        
        OLED_WRAP_UP_OP:
        {
            // Close the I2C bus? or define a function like OLED_Stop to close the transaction?
            // Note: OLED_Stop has been defined.
            // Check here for OLED Stop return value
            // Dont move to next state if the bus is still busy for some reason
            if(OLED_Stop() >= 0)
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