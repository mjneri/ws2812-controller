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

// Header files
#include "sh1106.h"

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

// A buffer containing only 0's, used for clearing the OLED display
// Should be enough 0's to clear one page
const uint8_t OLED_BlankDisplay[132] = {0};

// Extern Variables
// ...

// Local function prototypes
static i2c1_operations_t WriteCompleteHandler(void *data);

// Functions
void OLED_Initialize(void)
{   
    // Set callback functions NULL
    I2C1_SetDataCompleteCallback(NULL, NULL);
    I2C1_SetAddressNackCallback(NULL, NULL);
    I2C1_SetDataNackCallback(NULL, NULL);
    I2C1_SetTimeoutCallback(NULL, NULL);
    
    OLED_Send(OLED_InitCommands, sizeof(OLED_InitCommands));
    return;
}

void OLED_Send(uint8_t *buffer, size_t bufSize)
{
    // Open the I2C Bus
    I2C1_Open(SH1106_I2C_ADDR);
    
    I2C1_SetBuffer(buffer, bufSize);
    I2C1_SetDataCompleteCallback(NULL, NULL);
    I2C1_MasterWrite();                     // Initialize the I2C Write.
    while(I2C1_BUSY == I2C1_Close());       // Wait until the I2C bus can be closed
}

void OLED_Recv(uint8_t *buffer, size_t bufSize)
{
    // Open the I2C Bus
    I2C1_Open(SH1106_I2C_ADDR);
    
    // Pass buffer address to I2C PLIB
    I2C1_SetBuffer(buffer, bufSize);
    
    I2C1_MasterRead();                      // Initiate I2C Read
    while(I2C1_BUSY == I2C1_Close());       // Wait until the I2C bus can be closed.
    return;                                 // Data can be accessed through the buffer.
}

void OLED_ClearDisplay(void)
{   
    // Clear the display by writing 0's to the SH1106 RAM
    for(uint8_t i = 0; i < SH1106_PAGES; i++)
    {
        // Open the I2C Bus
        I2C1_Open(SH1106_I2C_ADDR);
        
        // Set cursor position & send 0's
        I2C1_SetBuffer((uint8_t []) \
            {SH1106_ONECOMMAND, (PAGEADDR | i), \
             SH1106_ONECOMMAND, (SETCOLUMNADDRLOW | 0), \
             SH1106_ONECOMMAND, (SETCOLUMNADDRHIGH | 0), \
             SH1106_DATABYTE}, 7);
        
        I2C1_SetDataCompleteCallback(WriteCompleteHandler, OLED_BlankDisplay);
        gBufSize = sizeof(OLED_BlankDisplay);
        
        I2C1_MasterWrite();                     // Initialize the I2C Write.
        while(I2C1_BUSY == I2C1_Close());       // Wait until the I2C bus can be closed
    }
}

void OLED_SetCursor(uint8_t row, uint8_t col)
{
    // Open the I2C Bus
    I2C1_Open(SH1106_I2C_ADDR);
    
    // Set cursor position
    I2C1_SetBuffer((uint8_t[])
        {SH1106_COMMANDBYTE, \
         (PAGEADDR | (row >> 3)), \
         (SETCOLUMNADDRLOW | ((col + SH1106_SEGOFFSET) & 0x0F)), \
         (SETCOLUMNADDRHIGH | (col + SH1106_SEGOFFSET) >> 4)}, 4);
        
    I2C1_SetDataCompleteCallback(NULL, NULL);
    I2C1_MasterWrite();                     // Initialize the I2C Write.
    while(I2C1_BUSY == I2C1_Close());       // Wait until the I2C bus can be closed
}

void OLED_DrawBitmap(void)
{
    for(uint8_t i = 0; i < SH1106_PAGES; i++)
    {
        // Set page and column addresses
        OLED_SetCursor((i << 3), 0);
        
        // Open the I2C Bus
        I2C1_Open(SH1106_I2C_ADDR);
        
        // Send bitmap data to the OLED
        I2C1_SetBuffer((uint8_t []){SH1106_DATABYTE}, 1);
        I2C1_SetDataCompleteCallback(WriteCompleteHandler, &Owl[128*i]);
        I2C1_MasterWrite();                     // Initialize the I2C Write.
        while(I2C1_BUSY == I2C1_Close());       // Wait until the I2C bus can be closed
    }
}

void OLED_DrawFont(uint8_t row, uint8_t col, char c, uint8_t *font, bool invertBG)
{
    // To move characters downward, each byte must be left shifted.
    uint16_t chIndex = 0;
    
    // Commenting out variables not needed for simple text printing.
    // For now, I'll assume that font5x7 is used, and row values will always be row % 8 == 0
    // uint8_t startPage = 0;
    // uint8_t endPage = 0;
    // char tempC = 0;
    
    // Get font width and height from the font array
    uint8_t fontWidth = font[FONT_WIDTH_INDEX];
    uint8_t fontHeight = font[FONT_HEIGHT_INDEX];
    uint8_t firstChar = font[FONT_FIRST_ASCII_INDEX];
    
    // How many bytes does each character occupy in the font array?
    uint8_t fontBytes = (fontWidth*(((fontHeight - 1) >> 3) + 1));
    
    // How many bits should the character shift?
//    uint8_t shiftAmt = row % SH1106_PAGES;
    
    // Get equivalent char index of c within the font array.
    // Array index starts at [4], first byte for the 'sp' char
    // TODO: define a macro for the start byte of the font array.
    switch(c)
    {
        case 0x0A:
        case 0x0D:
            chIndex = FONT_FIRST_CHAR_INDEX;
            break;
            
        default:
        {
            chIndex = FONT_FIRST_CHAR_INDEX + (c - firstChar)*fontBytes;
            break;
        }
    }
    
    // using chIndex, write each byte to the SH1106 RAM
    // TODO: update the low&high column addresses used. I'm assuming it's always 0 for now.
//    startPage = row % SH1106_PAGES;
//    endPage = (row + fontHeight) % SH1106_PAGES;
//    for(uint8_t x = startPage; x < endPage; x++)
//    {
//        // Set page and column addresses
//        OLED_SendCommand((uint8_t[]){(PAGEADDR | x), (SETCOLUMNADDRLOW | 0x00), (SETCOLUMNADDRHIGH | 0x00)}, 3);
//        
//        // Adjust character byte
//        //tempC = c << 
//    }
    
    // Iterate through each column of the character
//    for(uint8_t x = 0; x < fontWidth; x++)
//    {
//        // Set page and column address
//        OLED_SendCommand((uint8_t[]){(PAGEADDR | x), \
//                (SETCOLUMNADDRLOW | (x & 0x0F)),     \
//                (SETCOLUMNADDRHIGH | (x >> 4))}, 3);
//        
//        // Iterate through each row of the character
//        for(uint8_t y = startPage; y < endPage; y++)
//        {
//            // TODO: perform bit shifting here.
//            tempC = font[chIndex];
//            
//            // Send shifted data to the OLED
//            OLED_SendData((uint8_t []){font[chIndex] << shiftAmt}, 1);
//        }
//        
//    }
    
    // Simple printing of text
    for(uint8_t i = 0; i < fontWidth; i++)
    {
        if(invertBG)
        {
            OLED_Send((uint8_t []){SH1106_DATABYTE, ~(font[chIndex + i])}, 2);
        }
        else
        {
            OLED_Send((uint8_t []){SH1106_DATABYTE, font[chIndex + i]}, 2);
        }
    }
    
    // Insert one pixel space between characters
    if(invertBG)
    {
        OLED_Send((uint8_t[]){SH1106_DATABYTE, 0xFF}, 2);
    }
    else
    {
        OLED_Send((uint8_t[]){SH1106_DATABYTE, 0x00}, 2);
    }
}

void OLED_DrawString(uint8_t row, uint8_t col, char *str, uint8_t *font, bool invertBG)
{
    // Assumption: row values are 0, 8, 16, 24, 32, 40, etc. to make sure each byte
    // for each character is written to one page only, simplifying the write functions
    
    // Set col. and page address
    // col = col + SH1106_SEGOFFSET;
    OLED_SetCursor(row, col);
    
    for(uint8_t i = 0; i < strlen(str); i++)
    {
        OLED_DrawFont(row, col, str[i], font, invertBG);
    }
}

void OLED_SetPixel(uint8_t row, uint8_t col, uint8_t pixStatus)
{
    // Set pixel function without using local frame buffer.
    uint8_t readByte[2] = {0, 0};           // Temporary byte to store data from OLED
    
    // Open the I2C Bus
    I2C1_Open(SH1106_I2C_ADDR);
    
    // Set cursor position & start read-modify-write
    I2C1_SetBuffer((uint8_t[])
        {SH1106_ONECOMMAND, (PAGEADDR | (row >> 3)), \
         SH1106_ONECOMMAND, (SETCOLUMNADDRLOW | ((col + SH1106_SEGOFFSET) & 0x0F)), \
         SH1106_ONECOMMAND, (SETCOLUMNADDRHIGH | (col + SH1106_SEGOFFSET) >> 4), \
         SH1106_ONECOMMAND, RMWSTART, SH1106_ONEDATA}, 9);
    
    I2C1_SetDataCompleteCallback(NULL, NULL);
    I2C1_MasterWrite();                     // Initialize the I2C Write.
    while(I2C1_BUSY == I2C1_Close());       // Wait until the I2C bus can be closed
    
    // Perform required dummy read and actual read
    OLED_Recv(readByte, 2);
    
    // Modify specific pixel based on row argument
    if(pixStatus)
    {
        readByte[1] = readByte[1] | (1 << (row & 0x7));
    }
    else
    {
        readByte[1] = readByte[1] & ~(1 << (row & 0x7));
    }
    
    // Send byte back to the OLED
    OLED_Send((uint8_t[]){SH1106_DATABYTE, readByte[1]}, 2);
    
    // Stop Read-Modify-Write mode
    OLED_Send((uint8_t[]){SH1106_COMMANDBYTE, RMWEND}, 2);
    
    return;
}

// Local Functions
static i2c1_operations_t WriteCompleteHandler(void *data)
{
    // Store buffer from I2C_SendBuffer() to I2C PLIB Buffer
    I2C1_SetBuffer(data, gBufSize);
    
    // Reset data transfer completion callback to NULL
    I2C1_SetDataCompleteCallback(NULL, NULL);
    
    // Continue the I2C Transfer
    return I2C1_CONTINUE;
}