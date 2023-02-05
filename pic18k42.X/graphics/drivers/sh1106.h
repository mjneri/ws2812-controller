//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// sh1106.h -- Header file for sh1106.c
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: mjneri
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: sh1106.h
// Description:
//
// Revisions:
// Revision 0.01 - File Created (2021-12-30)
// Additional Comments:
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Header guards
#ifndef _SH1106_H_
#define _SH1106_H_

// *****************************************************************************
// *****************************************************************************
// Section: Header Files
// *****************************************************************************
// *****************************************************************************

#include <string.h>
#include "../../mcc_generated_files/i2c1_master.h"

#include "../canvas/fonts.h"
#include "../canvas/bitmaps.h"

// *****************************************************************************
// *****************************************************************************
// Section: Constant Defines
// *****************************************************************************
// *****************************************************************************

#define SH1106_I2C_ADDR     0x3C            // I2C slave address
#define SH1106_LCD_WIDTH    128             // This amount was found through testing (filling and clearing the display to see how many pixels remain)
#define SH1106_LCD_HEIGHT   64
#define SH1106_RAM_SIZE     1024            // LCD width * height

#define SH1106_PAGES        8
#define SH1106_SEGMENTS     128

#define SH1106_PAGESZ       8               // Size of a single page in pixels

#define SH1106_SEGOFFSET    2               // Segments [2, 130] are mapped to the OLED

#define SH1106_COMMANDBYTE  0x00            // D/nC bit is cleared; Co bit is 0
#define SH1106_DATABYTE     0x40            // D/nC bit is set; Co bit is 0
#define SH1106_ONECOMMAND   0x80            // D/nC bit is cleared; Co bit is 1; for sending single commands
#define SH1106_ONEDATA      0xC0            // D/nC bit is set; Co bit is 1; for sending single data

#define RINGBUF_MAXSIZE     16
#define ARRAY_MAXSIZE       132

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Data type definitions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief       SH1106 Commands
 * 
 * @description This enumeration defines the various commands that the SH1106 recognizes.
 * 
 */
typedef enum
{
    SETCONTRAST = 0x81,
    DISPLAYALLON_RESUME = 0xA4,
    DISPLAYALLON = 0xA5,
    NORMALDISPLAY = 0xA6,
    INVERTDISPLAY = 0xA7,
            
    DISPLAYOFF = 0xAE,
    DISPLAYON = 0xAF,
            
    SETDISPLAYOFFSET = 0xD3,
    SETCOMPINS = 0xDA,
    SETVCOMDETECT = 0xDB,
    SETDISPLAYCLOCKDIV = 0xD5,
    SETPRECHARGE = 0xD9,
    SETMULTIPLEX = 0xA8,
            
    SETCOLUMNADDRLOW = 0x00,
    SETCOLUMNADDRHIGH = 0x10,
            
    SETSTARTLINE = 0x40,
    PAGEADDR = 0xB0,
            
    COMSCANINC = 0xC0,
    COMSCANDEC = 0xC8,
            
    SEGREMAP = 0xA0,
    CHARGEPUMP = 0x8D,
    EXTERNALVCC = 0x1,
    SWITCHCAPVCC = 0x2,

    MESSAGETYPE_COMMAND = 0x80,
    MESSAGETYPE_DATA = 0x40,
          
    RMWSTART = 0xE0,
    RMWEND = 0xEE,
            
} SH1106_CMD;

/**
 * @brief       OLED state machine states
 * 
 * @description This enumeration defines the valid states for the state machine
 * 
 */

typedef enum
{
    OLED_INIT,
    OLED_WAIT_FOR_OP,
    OLED_START_OP,
    OLED_CHECK_OP_STAT,
    OLED_WRAP_UP_OP
} OLED_STATE;

/**
 * @brief       OLED Operation Queue
 * 
 * @description This struct defines the elements that need to be stored in a ring buffer
 *              such as the data buffer address, its size, and what type of operation it is.
 */
typedef struct
{
    uint8_t dBuf[ARRAY_MAXSIZE];// Array; for writes, data is taken from here; for reads, data is stored here
    uint16_t bufSize;   // Defined as 16-bit so max. buffer size is not limited to 255
    bool readOp;        // If true, the operation is a read. Otherwise, it's a write.
} oled_op_queue_t;

/**
 * @brief       OLED Ring buffer
 * 
 * @description This struct defines the ring buffer itself and the necessary attributes for
 *              performing operations with it.
 */
typedef struct
{
    volatile oled_op_queue_t buffer[RINGBUF_MAXSIZE];
    uint8_t rdIndex;            // Index pointing to data to be read
    uint8_t wrIndex;            // Index pointing to data to be written
    bool isEmpty;               // Flag; set when the ring buffer is empty
    bool isFull;                // Flag; set when the ring buffer is full
    uint8_t dataAmt;            // Determines how many ring buffer elements are stored
} oled_ring_buf_t;

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

/**
 * Initializes the SH1106 OLED Driver
 */
void OLED_Initialize(void);

/**
 * Enters data into the ring buffer to be processed by
 * the state machine.
 */
int OLED_Send(uint8_t *buffer, size_t bufSize);

/**
 * Enters a set cursor command into the ring buffer, to be
 * processed by the state machine
 */
int OLED_SetCursor(uint8_t row, uint8_t col);

/**
 * Indicates if the OLED state machine is busy with an I2C transfer
 * @return false - if the state machine is in OLED_WAIT_FOR_OP or OLED_INIT
 * @return true otherwise
 */
bool OLED_IsBusy(void);

/**
 * OLED state machine maintainer.
 */
void OLED_Tasks(void);

/*
 * NOTE: OLED_Recv is not usable right now. The application will not use this function
 * anyway.
 * In the future, I might implement callback functions within sh1106.c so modules are
 * alerted that the read operation has completed.
 * Example pseudocode:
 * int OLED_Callback(void *op, void *data)
 * {
 *      switch(*op)
 *      {
 *          OLED_READ:  // do something
 *          OLED_WRITE: // do something
 *      }
 *      return;
 * }
 * 
 * void OLED_Initialize(void)
 * {
 *      OLED_RegisterCallback(OLED_Callback);
 *      // ...
 * }
 * 
 * void OLED_Tasks(void)
 * {
 *      // ...
 *      OLED_WRAP_UP_OP:
 *      {
 *          if(cb != NULL)
 *          {
 *              callback(op, data);
 *          }
 *      }
 *      // ...
 * }
 */
int OLED_Recv(uint8_t *buffer, size_t bufSize);

#endif /*_SH1106_H_*/

/*******************************************************************************
 End of File
*/

