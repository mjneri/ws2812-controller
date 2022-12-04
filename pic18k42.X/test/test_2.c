
#include "test.h"

// Function prototypes
static void OLED_TEST(void);

// Function prototypes of what will eventually be added in gfx.c
static bool Gfx_render(void);       // Send software framebuffer to OLED
static void Gfx_pixel(uint8_t row, uint8_t col, bool state);    // Turn on a pixel in the framebuffer
static void Gfx_text(uint8_t row, uint8_t col, char *str, uint8_t *font, bool invertBG);
static void Gfx_draw(uint8_t *image);
static void Gfx_clear(void);

// Software framebuffer
static uint8_t Gfx_framebuffer[SH1106_RAM_SIZE];

// "Dirty page" tracker. If bit is set, corresponding page was modified.
// Only modified pages will be written to by Gfx_render
static uint8_t modifiedPage = 0x00;

// Regular heartbeat callback using Timer1
static void HeartbeatCallback(void)
{
    LED0_Toggle();
}

// Function define
void TEST_2_Function(void)
{
    // Register the heartbeat callback function
    TMR1_SetInterruptHandler(HeartbeatCallback);
    
    millis_Initialize();
    OLED_Initialize();
    
    // OLED Test
    OLED_TEST();
    
    return;
}

static void OLED_TEST(void)
{
    Gfx_clear();
    
    // NOTE: Add this to the GFX Initialization code later.
    // NOTE 2: Or, just call Gfx_clear in the init code like what I did above.
    //memset(Gfx_framebuffer, 0, sizeof(Gfx_framebuffer));
    
    // Test pixel function
//    for(uint16_t i = 0; i < SH1106_PAGES; i++)
//    {
//        for(uint16_t j = 0; j < SH1106_SEGMENTS; j++)
//        {
//            for(uint8_t k = 0; k < 8; k++)
//            {
//                Gfx_pixel((i << 3) + k, j, 1);
//                Gfx_render();
//            }
//        }
//    }

    Gfx_text(5,15, "Hello world", &font5x7, false);
    Gfx_render();
    
    uint64_t t0 = millis();
    uint8_t bitmapSel = 0;
    while(1)
    {
        // Cycle through bitmaps
        if(millis() - t0 >= 2000)
        {
            t0 = millis();      // Update t0 again
            Gfx_clear();
            
            switch(bitmapSel)
            {
                case 0:
                    Gfx_draw(Owl);
                    break;
                case 1:
                    Gfx_draw(pochita);
                    break;
                case 2:
                    Gfx_draw(nailandgear);
                    break;
                case 3:
                    Gfx_draw(hollowknight);
                    break;
                default:
                    break;
            }
            
            bitmapSel = (bitmapSel+1) % 4;
            Gfx_render();
        }
        
        DEBUG_GPIO_OUT_Toggle();
        OLED_Tasks();
    }
}

// FUNCTION TESTS
static bool Gfx_render(void)
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
        memcpy(&tempbuf[1], &Gfx_framebuffer[128*i], 128);
        
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

static void Gfx_pixel(uint8_t row, uint8_t col, bool state)
{
    // Get array and pixel index from row and col
    uint8_t page = row >> 3;
    uint16_t arrIdx = SH1106_SEGMENTS*(page) + col;
    uint16_t pixIdx = row - SH1106_PAGES*(page);
    
    uint8_t modPix = 1 << pixIdx;
    
    // state == 1, on; state == 0, off
    if(state)
    {
        Gfx_framebuffer[arrIdx] |= modPix;
    }
    else
    {
        Gfx_framebuffer[arrIdx] &= ~modPix;
    }
    
    // Update modified page...
    modifiedPage |= (1 << page);
    
    return;
}

static void Gfx_text(uint8_t row, uint8_t col, char *str, uint8_t *font, bool invertBG)
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
    
    // Loop through each char in the string
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

        // Loop through each column byte of the char
        for(int j = 0; j < fontWidth; j++)
        {
            // Get column byte value and increment chIndex for accessing the next column byte
            uint8_t colByte = font[chIndex++];
            
            // Get the pixel's row value
            uint8_t pRow = row;
            
            // Loop through each bit of the column byte
            for(int k = 0; k < fontHeight; k++)
            {
                // Get the pixel's column value
                uint8_t pCol = col;
                
                if(invertBG)
                {
                    Gfx_pixel(pRow, pCol, (~colByte) & 0x01);
                }
                else
                {
                    Gfx_pixel(pRow, pCol, colByte & 0x01);
                }
                
                // Increment pRow s.t., it points to the next row pixel
                pRow++;
                
                // Bit shift the column byte
                colByte >>= 1;
            }
            
            // Increment to next column
            col++;
        }

        // Insert one pixel space between characters
        col++;
    }
    
    return;
}

static void Gfx_draw(uint8_t *image)
{
    // Copy the image bitmap to the framebuffer
    // NOTE: assume that the image is 128*64 for now. ADD ERROR CHECKING LATER
    memcpy(Gfx_framebuffer, image, sizeof(Gfx_framebuffer));
    
    // Update modified page...
    modifiedPage |= 0xFF;
    return;
}

static void Gfx_clear(void)
{
    // Clear the framebuffer contents
    memset(Gfx_framebuffer, 0, sizeof(Gfx_framebuffer));
    
    // Update modified page...
    modifiedPage |= 0xFF;
}

/********************************************
 END OF FILE
 **********************************************/