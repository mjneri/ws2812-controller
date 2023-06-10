//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// gfx.c -- Hardware-agnostic graphics library
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: mjneri
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: gfx.c
// Description: Inspired by lcdgfx, this serves as the interface between user
//              applications and the OLED driver, whatever it may be.
//
// Revisions:
// Revision 0.01 - File Created (2022-04-09)
// Additional Comments:
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// *****************************************************************************
// *****************************************************************************
// Section: Header Includes
// *****************************************************************************
// *****************************************************************************

#include "gfx.h"

// *****************************************************************************
// *****************************************************************************
// Section: Variables
// *****************************************************************************
// *****************************************************************************

// Software framebuffer
static uint8_t GFX_Framebuffer[SH1106_RAM_SIZE];

// "Dirty page" tracker. If bit is set, corresponding page was modified.
// Only modified pages will be written to by Gfx_render
static uint8_t modifiedPage = 0x00;

// *****************************************************************************
// *****************************************************************************
// Section: Local Function Defines
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

bool GFX_Render(void)
{
    if(OLED_IsBusy())
    {
        return false;
    }
    
    uint8_t tempbuf[129];
    
    // Render the framebuffer on the OLED per page.
    for (uint8_t i = 0; i < SH1106_PAGES; i++)
    {
        if(!(modifiedPage & (1 << i)))
        {
            // Skip if page has not been modified yet.
            continue;
        }
        
        // Copy over frame buffer contents to tempbuf, prepended by SH1106_DATABYTE
        tempbuf[0] = SH1106_DATABYTE;
        memcpy(&tempbuf[1], &GFX_Framebuffer[128*i], 128);
        
        // Set page and column addresses
        // Note: Shift left by 3; e.g., when i==2, we want to be at page 2 (or row 16)
        // OLED_SetCursor assumes we pass row values, so we convert the page to a row
        // value by shifting left 3 bits (hope this makes sense)
        OLED_SetCursor((i << 3), 0);
        
        // Send the framebuffer prepended by SH1106_DATABYTE
        // so the OLED knows the bytes it receives are data.
        OLED_Send(tempbuf, sizeof(tempbuf));
    }
    
    // Reset modified page
    modifiedPage = 0x00;
    
    return true;
}

void GFX_Pixel(uint8_t row, uint8_t col, bool state)
{
    // Get array and pixel index from row and col
    uint8_t page = row >> 3;
    uint16_t arrIdx = SH1106_SEGMENTS*(page) + col;
    uint16_t pixIdx = row - SH1106_PAGES*(page);
    
    uint8_t modPix = 1 << pixIdx;
    
    // state == 1, on; state == 0, off
    if(state)
    {
        GFX_Framebuffer[arrIdx] |= modPix;
    }
    else
    {
        GFX_Framebuffer[arrIdx] &= ~modPix;
    }
    
    // Update modified page...
    modifiedPage |= (1 << page);
    
    return;
}

void GFX_Text(uint8_t row, uint8_t col, char *str, uint8_t *font, bool invertBG)
{
    // To move characters downward, each byte must be left shifted. See illustration below
    
    /* |-Col 0-|-Col 1-|.......|-Col 4-|
     * | Bit 0 |       |.......|       |
     * | Bit 1 |       |.......|       |
     * | Bit 2 |       |.......|       |
     * | Bit 3 |       |.......|       |
     * | ...   |       |.......|       |
     * | Bit 7 |       |.......|       |
     * |-------|-------|-------|-------|
     * 
     * To move a character downward, bit shift left.
     */
    
    // chIndex stores the index value of the data in the byte array.
    uint16_t chIndex = 0;
    
    // Get font width and height from the font array
    uint8_t fontWidth = font[FONT_WIDTH_INDEX];
    uint8_t fontHeight = font[FONT_HEIGHT_INDEX];
    uint8_t firstChar = font[FONT_FIRST_ASCII_INDEX];
    
    // How many bytes does each character occupy in the font array?
    uint8_t fontBytes = (fontWidth*(((fontHeight - 1) >> 3) + 1));
    
    // Check if text will fit in a single page based on height
    if(fontHeight <= SH1106_PAGESZ)
    {
        // Then, check if row input is a multiple of a page.
        // If not, bit shifts are required.
        if((row % 8) == 0)
        {
            // Get page and array index for software framebuffer 
            uint8_t page = row >> 3;
            uint16_t arrIdx = SH1106_SEGMENTS*(page) + col;
            
            for(int i = 0; i < strlen(str); i++)
            {
                // Get starting byte index from the array
                switch(str[i])
                {
                    case 0x0A:
                    case 0x0D:
                        chIndex = FONT_FIRST_CHAR_INDEX;
                        break;

                    default:
                    {
                        chIndex = FONT_FIRST_CHAR_INDEX + (str[i] - firstChar)*fontBytes;
                        break;
                    }
                }
                
                for(int j = 0; j < fontWidth; j++)
                {
                    if(invertBG)
                    {
                        GFX_Framebuffer[arrIdx++] = ~font[chIndex + j];
                    }
                    else
                    {
                        GFX_Framebuffer[arrIdx++] = font[chIndex + j];
                    }
                }

                // Insert one pixel space between characters
                if(invertBG)
                {
                    GFX_Framebuffer[arrIdx++] = 0xFF;
                }
                else
                {
                    GFX_Framebuffer[arrIdx++] = 0x00;
                }
            }
            
            // Update modified page...
            modifiedPage |= (1 << page);
            
            return;
        }
        else
        {
            // Do bit shifts or something.
            // To be implemented in the future.
        }
    }
    else
    {
        // To be implemented in the future
        // Pseudocode:
        /*Int I = height >> 3; // Determines how many pages are occupied
		Get offset;
		If (I > 1)
		{
			// Loop through hex data of font;
			Loop(height)
			{
				Loop(width)
				{
					// Place data to array
				}
			}
		}*/
    }
    
    return;
}

void GFX_Draw(uint8_t *image)
{
    // Copy the image bitmap to the framebuffer
    // NOTE: assume that the image is 128*64 for now. ADD ERROR CHECKING LATER
    memcpy(GFX_Framebuffer, image, sizeof(GFX_Framebuffer));
    
    // Update modified page...
    modifiedPage |= 0xFF;
    return;
}

void GFX_Clear(void)
{
    // Clear the framebuffer contents
    memset(GFX_Framebuffer, 0, sizeof(GFX_Framebuffer));
    
    // Update modified page...
    modifiedPage |= 0xFF;
}

/*******************************************************************************
 End of File
*/

//void GFX_Text(uint8_t row, uint8_t col, char *str, uint8_t *font, bool invertBG)
//{
//    // To move characters downward, each byte must be left shifted. See illustration below
//    
//    /* |-Col 0-|-Col 1-|.......|-Col 4-|
//     * | Bit 0 |       |.......|       |
//     * | Bit 1 |       |.......|       |
//     * | Bit 2 |       |.......|       |
//     * | Bit 3 |       |.......|       |
//     * | ...   |       |.......|       |
//     * | Bit 7 |       |.......|       |
//     * |-------|-------|-------|-------|
//     * 
//     * To move a character downward, bit shift left.
//     */
//    
//    // chIndex stores the index value of the data in the byte array.
//    uint16_t chIndex = 0;
//    
//    // Get font width and height from the font array
//    uint8_t fontWidth = font[FONT_WIDTH_INDEX];
//    uint8_t fontHeight = font[FONT_HEIGHT_INDEX];
//    uint8_t firstChar = font[FONT_FIRST_ASCII_INDEX];
//    
//    // How many bytes does each character occupy in the font array?
//    uint8_t fontBytes = (fontWidth*(((fontHeight - 1) >> 3) + 1));
//    
//    // Loop through each char in the string
//    for(int i = 0; i < strlen(str); i++)
//    {
//        // Get starting byte index from the array
//        switch(str[i])
//        {
//            case 0x0A:
//            case 0x0D:
//                chIndex = FONT_FIRST_CHAR_INDEX;
//                break;
//
//            default:
//            {
//                chIndex = FONT_FIRST_CHAR_INDEX + (str[i] - firstChar)*fontBytes;
//                break;
//            }
//        }
//
//        // Loop through each column byte of the char
//        for(int j = 0; j < fontWidth; j++)
//        {
//            // Get column byte value and increment chIndex for accessing the next column byte
//            uint8_t colByte = font[chIndex++];
//            
//            // Get the pixel's row value
//            uint8_t pRow = row;
//            
//            // Loop through each bit of the column byte
//            for(int k = 0; k < fontHeight; k++)
//            {
//                // Get the pixel's column value
//                uint8_t pCol = col;
//                
//                if(invertBG)
//                {
//                    GFX_Pixel(pRow, pCol, (~colByte) & 0x01);
//                }
//                else
//                {
//                    GFX_Pixel(pRow, pCol, colByte & 0x01);
//                }
//                
//                // Increment pRow s.t., it points to the next row pixel
//                pRow++;
//                
//                // Bit shift the column byte
//                colByte >>= 1;
//            }
//            
//            // Increment to next column
//            col++;
//        }
//
//        // Insert one pixel space between characters
//        col++;
//    }
//    
//    return;
//}