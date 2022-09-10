

// Header files
#include <pic18.h>

#include "test.h"

#define LEDLATCH() __delay_ms(1)        // wait for LED to latch

// Globals for testing
volatile uint8_t selected = 0;
volatile bool switchSelected = false;
volatile bool isSwitchPressed = false;
volatile bool isSwitchHeld = false;

// Callback for heartbeat LED
void HeartbeatCallback(void)
{
    LED0_Toggle();
}

void DebounceCallback(void)
{
    // NOTE: Make sure SW0 is connected to TM4_ers input (RC6)
    if(!SW0_GetValue())
    {
        isSwitchPressed = true;
    }
    else
    {
        isSwitchPressed = false;
    }
}

// Local function prototypes
static void TEST_OLED(void);
static void TEST_PIXELS(void);

// Other local function prototypes
void displayMenu(uint8_t selectedOption);
static void pixels_StartupSequence(void);
static void lowmemLED(void);
static void rainbowCycle(unsigned char frames , unsigned int frameAdvance, unsigned int pixelAdvance );
static void profiletestSolid(void);
static uint24_t race_color(uint8_t x);
static void profitestRacing(uint8_t szlight, uint8_t szspace);
static void profitestCometsTail(uint8_t tailLen);
static void profitestPulsingSingleColor(void);

// Main Test function definition
void TEST_Function(void)
{
    // Register the callback function
    //TMR1_SetInterruptHandler(HeartbeatCallback);
    
    // Register TMR4 Callback
    //TMR4_SetInterruptHandler(DebounceCallback);
    
    // Test the OLED functions
    //TEST_OLED();
    
    // Test SPI LED functions
    TEST_PIXELS();
    
    return;
}

// Local function definitions
static void TEST_OLED(void)
{
    OLED_Initialize();
    OLED_ClearDisplay();
    
    OLED_DrawBitmap();
    __delay_ms(1000);
    OLED_ClearDisplay();
//    OLED_DrawString(0, 20, "Hello world!", font5x7, false);
//    OLED_DrawString(9, 0, "The quick brown fox", font5x7, true);
    
    displayMenu(0);
    
    // Quick software thing - count how long switch is pressed
    uint32_t i = 0;
    
    while(1)
    {
        // 2022-02-01 >> Simple test for the set pixel function
//        for(uint8_t i = 0; i < SH1106_LCD_HEIGHT; i+=8)
//        {
//            for(uint8_t j = 0; j < 8; j++)
//            {
//                for(uint8_t k = 0; k < SH1106_SEGMENTS; k++)
//                {
//                    OLED_SetPixel(j + i, k, 1);
//                }
//            }
//        }
//        for(uint8_t i = 0; i < SH1106_LCD_HEIGHT; i+=8)
//        {
//            for(uint8_t j = 0; j < SH1106_SEGMENTS; j++)
//            {
//                for(uint8_t k = 0; k < 8; k++)
//                {
//                    OLED_SetPixel(k + i, j, 0);
//                }
//            }
//        }
//        for(uint8_t i = 0; i < 64; i++)
//        {
////            OLED_SetPixel(i, 5, 1);
////            OLED_SetCursor(0, i);
////            OLED_SendData((uint8_t []){0xFF}, 1);
////            __delay_ms(100);
//            OLED_SetCursor(i, 0);
//            OLED_RecvData(dummyRead, sizeof(dummyRead));
////            OLED_RecvData(&dummyRead, 1);
////            OLED_RecvData(&dummyRead, 1);
////            OLED_RecvData(&dummyRead, 1);
////            OLED_RecvData(&dummyRead, 1);            
//            __delay_ms(100);
//        }
        
        // 2022-02-26 Test code
        if(isSwitchPressed && !isSwitchHeld)
        {
            isSwitchHeld = true;
            i = 0;
        }
        else if(!isSwitchPressed && isSwitchHeld)
        {
            // Should not be executed if key press is long.
            i = 0;
            if(!switchSelected)
            {
                isSwitchHeld = false;
                selected = (selected+1) % 3;
                displayMenu(selected);
            }
            else
            {
                switchSelected = false;
                isSwitchHeld = false;
            }
        }
        else if(isSwitchPressed && isSwitchHeld)
        {
            i++;
            if(i == 0x80000)
            {
                // Some event selecting the menu option.
                i = 0;
                LED0_Toggle();
                switchSelected = true;
            }
        }
        
        // Do something if the user chooses an option from the menu
        if(switchSelected)
        {
            switch(selected)
            {
                case 0:
                {
                    // Select LED Profile
                    break;
                }
                case 1:
                {
                    // Adjust LED Count
                    break;
                }
                case 2:
                {
                    // About
                    OLED_ClearDisplay();
                    OLED_DrawString(0, 46, "About", font5x7, 0);
                    OLED_DrawString(8, 0, "A personal project", font5x7, 0);
                    OLED_DrawString(16, 0, "by MJ Neri", font5x7, 0);
                    OLED_DrawString(24, 0, "github.com/mjneri/", font5x7, 0);
                    OLED_DrawString(32, 3, "ws2812-controller", font5x7, 0);
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }
}

static void TEST_PIXELS(void)
{   
    // Open SPI port
    SPI1_Open(SPI1_DEFAULT);
    
    // 2022-03-20: Minimal memory footprint test code
    RGB_Clear(LEDSTRIPSIZE);
    
    //pixels_StartupSequence();
    //__delay_ms(250);
    //lowmemLED();
    //profiletestSolid();
    //profitestRacing(3, 9);
    profitestCometsTail(6);
    //profitestPulsingSingleColor();
    
    while(1)
    {
        //rainbowCycle(255, 1, 1);
    }
    return;
}

// Other local test functions
void displayMenu(uint8_t selectedOption)
{
    OLED_DrawString(0, 0, "Main Menu", font5x7, 0);
    OLED_DrawString(8, 5, "Select LED Profile", font5x7, (selectedOption == 0));
    OLED_DrawString(16, 5, "Adjust LED Count", font5x7, (selectedOption == 1));
    OLED_DrawString(24, 5, "About", font5x7, (selectedOption == 2));
}

static void pixels_StartupSequence(void)
{
    // Start-up sequence for the LEDs
    uint16_t pIndex = 0;
    int16_t i = 0;
    
    // Walking white light
    for(i = 0; i < LEDSTRIPSIZE; i++)
    {
       pIndex = 0;
       while(pIndex++ < i)
       {
           RGB_SetColor(RGB_TO_VAL(0,0,0));
       }
       
       RGB_SetColor(RGB_TO_VAL(25,25,25));
           
        LEDLATCH();
        __delay_ms(10);
    }
    
    // Reverse the direction!
    __delay_ms(100);
    for(i = LEDSTRIPSIZE-1; i >= 0; i--)
    {
        pIndex = 0;
        while(pIndex++ < i )
        {
           RGB_SetColor(RGB_TO_VAL(0,0,0));
        }

        RGB_SetColor(RGB_TO_VAL(25,25,25));
        
        while(pIndex++ < LEDSTRIPSIZE)
        {
           RGB_SetColor(RGB_TO_VAL(0,0,0));
        }
        LEDLATCH();
        __delay_ms(10);
    }
    
    // Turn each LED Red
    __delay_ms(200);
    for(i = 0; i < LEDSTRIPSIZE; i++)
    {
        pIndex = 0;
        while(pIndex++ <= i )
        {
           RGB_SetColor(RGB_TO_VAL(25,0,0));
        }
        while(pIndex++ <= LEDSTRIPSIZE )
        {
           RGB_SetColor(RGB_TO_VAL(0,0,0));
        }
        LEDLATCH();
        __delay_ms(10);
    }
    
    // Turn off each LED one by one
    for(i = 0; i < LEDSTRIPSIZE; i++)
    {
        pIndex = 0;
        while(pIndex++ <= i )
        {
           RGB_SetColor(RGB_TO_VAL(0,0,0));
        }
        while(pIndex++ <= LEDSTRIPSIZE )
        {
           RGB_SetColor(RGB_TO_VAL(25,0,0));
        }
        LEDLATCH();
        __delay_ms(10);
    }
    
    // Turn each LED blue starting from the center of the stick
    // Assumes that there is an even number of LEDs present
    __delay_ms(200);
    for(i = LEDSTRIPSIZE/2-1; i >= 0; i--)
    {
        pIndex = 0;
        while(pIndex < i)
        {
            RGB_SetColor(RGB_TO_VAL(0,0,0));
            pIndex++;
        }
        while(pIndex <= (LEDSTRIPSIZE-i))
        {
            RGB_SetColor(RGB_TO_VAL(1,1,25));
            pIndex++;
        }
        while(pIndex < LEDSTRIPSIZE)
        {
            RGB_SetColor(RGB_TO_VAL(0,0,0));
            pIndex++;
        }
        
        LEDLATCH();
        __delay_ms(10);
    }
    
    // Turn each LED off using the same pattern as above
    __delay_ms(200);
    for(i = LEDSTRIPSIZE/2-1; i >= 0; i--)
    {
        pIndex = 0;
        while(pIndex < i)
        {
            RGB_SetColor(RGB_TO_VAL(1,1,25));
            pIndex++;
        }
        while(pIndex < (LEDSTRIPSIZE-i))
        {
            RGB_SetColor(RGB_TO_VAL(0,0,0));
            pIndex++;
        }
        while(pIndex < LEDSTRIPSIZE)
        {
            RGB_SetColor(RGB_TO_VAL(1,1,25));
            pIndex++;
        }
        
        LEDLATCH();
        __delay_ms(10);
    }
    
    // Turn each LED Pink starting from the last LED
    __delay_ms(200);
    for(i = LEDSTRIPSIZE-1; i >= 0; i--)
    {
        pIndex = 0;
        while(pIndex++ < i)
        {
            RGB_SetColor(RGB_TO_VAL(0,0,0));
        }
        RGB_SetColor(RGB_TO_VAL(23, 0, 14));
        LEDLATCH();
        __delay_ms(10);
    }
    
    // Turn each LED off
    __delay_ms(200);
    for(i = 0; i < LEDSTRIPSIZE; i++)
    {
        pIndex = 0;
        while(pIndex++ <= i)
        {
            RGB_SetColor(RGB_TO_VAL(0,0,0));
        }
        while(pIndex++ <= LEDSTRIPSIZE)
        {
            RGB_SetColor(RGB_TO_VAL(23, 0, 14));
        }
        LEDLATCH();
        __delay_ms(10);
    }
    
    // Breathing Green LEDs
    __delay_ms(200);
    for(i = 0; i < 64; i++)
    {
        RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(0, i, 0));
        __delay_ms(5);
    }
    
    for(i = 64; i >= 0; i--)
    {
        RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(0, i, 0));
        __delay_ms(5);
    }
}

static void lowmemLED(void)
{
    uint16_t i, p = 0;
    
    // Turn all LEDs Blue
    RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(0,25,25));
    __delay_ms(1000);
    
    // Turn LED's off one-by one
    while(1)
    {
        for(i=0; i < LEDSTRIPSIZE; i++)
        {
            p = 0;

            while(p++ <= i)
            {
                RGB_SetColor(RGB_TO_VAL(0,0,0));
            }

            while(p++ <= LEDSTRIPSIZE)
            {
                RGB_SetColor(RGB_TO_VAL(0,25,25));
            }

            __delay_ms(10);
        }

        // Turn on LEDs one by one
        for(i=0; i < LEDSTRIPSIZE; i++)
        {
            p = 0;

            while(p++ <= i)
            {
                RGB_SetColor(RGB_TO_VAL(0,25,25));
            }
            while(p++ <= LEDSTRIPSIZE)
            {
                RGB_SetColor(RGB_TO_VAL(0,0,0));
            }

            __delay_ms(10);
        }
    }

    __delay_ms(100);
    
}

// Function below was taken from
// https://github.com/bigjosh/SimpleNeoPixelDemo/blob/master/SimpleNeopixelDemo/SimpleNeopixelDemo.ino
// which was based on
// https://github.com/adafruit/Adafruit_NeoPixel/blob/master/examples/strandtest/strandtest.ino
static void rainbowCycle(unsigned char frames , unsigned int frameAdvance, unsigned int pixelAdvance )
{
  
  // Hue is a number between 0 and 3*256 than defines a mix of r->g->b where
  // hue of 0 = Full red
  // hue of 128 = 1/2 red and 1/2 green
  // hue of 256 = Full Green
  // hue of 384 = 1/2 green and 1/2 blue
  // ...
  
  unsigned int firstPixelHue = 0;     // Color for the first pixel in the string
  
  for(unsigned int j=0; j<frames; j++) {                                  
    
    unsigned int currentPixelHue = firstPixelHue;
        
    for(unsigned int i=0; i< LEDSTRIPSIZE; i++) {
      
      if (currentPixelHue>=(3*256)) {                  // Normalize back down incase we incremented and overflowed
        currentPixelHue -= (3*256);
      }
            
      unsigned char phase = currentPixelHue >> 8;
      unsigned char step = currentPixelHue & 0x1f;
      unsigned char nstep = (~step) & 0x1f;
                 
      switch (phase) {
        
        case 0: 
          RGB_SetColor(RGB_TO_VAL(nstep,step,0));
          break;
          
        case 1: 
          RGB_SetColor( RGB_TO_VAL(0 , nstep , step) );
          break;

        case 2: 
          RGB_SetColor(  RGB_TO_VAL(step ,0 , nstep) );
          break;
          
      }
      
      currentPixelHue+=pixelAdvance;                                      
      
                          
    } 
    LEDLATCH();
    __delay_ms(10);
    
    firstPixelHue += frameAdvance;
           
  }
}

static void profiletestSolid(void)
{
    uint16_t i = 0;
    
    // Cycle between 7 colors
    while(1)
    {
        switch(i)
        {
            case 1:
                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(0,0,25));
                break;
            case 2:
                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(0,25,0));
                break;
            case 3:
                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(0,25,25));
                break;
            case 4:
                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(25,0,0));
                break;
            case 5:
                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(25,0,25));
                break;
            case 6:
                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(25,25,0));
                break;
            case 7:
                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(25,25,25));
                break;
            default:
                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(0,0,0));
                break;
        }
        
        i = ++i % 8;
        i = (i==0)? 1 : i;
        LEDLATCH();
        __delay_ms(1000);
    }
}

// Test macros for profitestRacing()
static uint24_t race_color(uint8_t x)
{
    // switch case with range
    switch(x)
    {
        case 0x00 ... 0x1f: return 0x000019;
        case 0x20 ... 0x3f: return 0x001900;
        case 0x40 ... 0x5f: return 0x001919;
        case 0x60 ... 0x7f: return 0x190000;
        case 0x80 ... 0x9f: return 0x190019;
        case 0xa0 ... 0xbf: return 0x191900;
        case 0xc0 ... 0xe0: return 0x191919;
        default: return 0x1f0320;
    }
}

static void profitestRacing(uint8_t szlight, uint8_t szspace)
{
    uint16_t i, p = 0;
    uint16_t combinedSegLen = szlight + szspace;
    uint16_t ledOffset = combinedSegLen;
    
    while(1)
    {
        for(i=0, p=0+ledOffset; i < LEDSTRIPSIZE; i++, p=(p+1)%combinedSegLen)
        {
            // quick code - needs to be shortened/optimized later?
            if(p < szlight)
            {
                RGB_SetColor(0x001900);
            }
            else
            {
                RGB_SetColor(0);
            }
        }
        LEDLATCH();
        __delay_ms(20);
        
        // Uncomment the other to reverse direction
        //ledOffset = (ledOffset+1) % combinedSegLen;
        ledOffset = (ledOffset == 0)? combinedSegLen-1 : (ledOffset-1) % combinedSegLen;
    }
}

static void profitestCometsTail(uint8_t tailLen)
{
    int16_t i, q = 0;
    int16_t ledN = 0;
    
    // Determines where the comet's head and tail are located
    int16_t cometHeadLoc = 0;
    int16_t cometTailLoc = 0;
    int16_t cometLength = tailLen + 1;
    
    uint16_t frames = LEDSTRIPSIZE + tailLen;
    
    while(1)
    {
        // Forward comet flight
        for(i=0; i < frames; i++)
        {
            for(ledN = 0; ledN < cometTailLoc; ledN++)
            {
                RGB_SetColor(0);
            }
            for(q = tailLen; ledN < cometHeadLoc; ledN++, q--)
            {
                RGB_SetColor(RGB_TO_VAL(0x7f>>q, 0, 0));
            }
            if(ledN == cometHeadLoc)
            {
                RGB_SetColor(RGB_TO_VAL(0x7f, 0, 0));
            }
            
            // Advance head and tail
            cometHeadLoc = (cometHeadLoc < LEDSTRIPSIZE-1)? cometHeadLoc+1 : LEDSTRIPSIZE-1;
            cometTailLoc = (cometHeadLoc < tailLen)? 0 : \
                (cometHeadLoc == LEDSTRIPSIZE-1)? cometTailLoc+1 : cometHeadLoc - tailLen;
            
            LEDLATCH();
            __delay_ms(25);
        }
        
        __delay_ms(50);
        cometHeadLoc = LEDSTRIPSIZE - 1;
        cometTailLoc = LEDSTRIPSIZE - 1;
        
        // Reverse flight
        for(i=0; i < frames; i++)
        {
            for(ledN = 0; ledN < cometHeadLoc; ledN++)
            {
                RGB_SetColor(0);
            }
            if(ledN++ == cometHeadLoc)
            {
                RGB_SetColor(RGB_TO_VAL(0x7f, 0, 0));
            }
            for(q = 1; ledN <= cometTailLoc; ledN++, q++)
            {
                RGB_SetColor(RGB_TO_VAL(0x7f>>q, 0, 0));
            }
            if(ledN > cometTailLoc)
            {
                RGB_SetColor(0);
            }
            
            // Advance head and tail
            cometHeadLoc = (cometHeadLoc > 0)? cometHeadLoc-1 : 0;
            cometTailLoc = ((cometTailLoc-cometHeadLoc) <= tailLen && cometHeadLoc > 0)? \
                            LEDSTRIPSIZE-1 : (cometTailLoc > 0)? cometTailLoc - 1 : 0;
            
            LEDLATCH();
            __delay_ms(25);
        }
    }
}

static void profitestPulsingSingleColor(void)
{
    int16_t i = 0;
    
    while(1)
    {
        for(i = 0; i < 64; i++)
        {
            RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(i,0,0));
            __delay_ms(10);
        }

        for(i = 64; i >= 0; i--)
        {
            RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(i,0,0));
            __delay_ms(10);
        }
        __delay_ms(100);
    }
    
}

/**
 End of File
*/