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

// Project includes
#include <string.h>
#include "../../mcc_generated_files/i2c1_master.h"

#include "../canvas/fonts.h"
#include "../canvas/bitmaps.h"

// Application-level constants
#define SH1106_I2C_ADDR     0x3C            // I2C slave address
#define SH1106_LCD_WIDTH    128             // This amount was found through testing (filling and clearing the display to see how many pixels remain)
#define SH1106_LCD_HEIGHT   64
#define SH1106_RAM_SIZE     1024            // LCD width * height

#define SH1106_PAGES        8
#define SH1106_SEGMENTS     128

#define SH1106_SEGOFFSET    2               // Segments [2, 130] are mapped to the OLED

#define SH1106_COMMANDBYTE  0x00            // D/nC bit is cleared; Co bit is 0
#define SH1106_DATABYTE     0x40            // D/nC bit is set; Co bit is 0
#define SH1106_ONECOMMAND   0x80            // D/nC bit is cleared; Co bit is 1; for sending single commands
#define SH1106_ONEDATA      0xC0            // D/nC bit is set; Co bit is 1; for sending single data

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


// Function prototypes
void OLED_Initialize(void);
void OLED_Send(uint8_t *buffer, size_t bufSize);
void OLED_Recv(uint8_t *buffer, size_t bufSize);

void OLED_ClearDisplay(void);
void OLED_SetCursor(uint8_t row, uint8_t col);
void OLED_DrawBitmap(void);
void OLED_DrawFont(uint8_t row, uint8_t col, char c, uint8_t *font, bool invertBG);
void OLED_DrawString(uint8_t row, uint8_t col, char *str, uint8_t *font, bool invertBG);
void OLED_SetPixel(uint8_t row, uint8_t col, uint8_t pixStatus);

#endif /*_SH1106_H_*/