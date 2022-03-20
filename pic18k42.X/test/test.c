

// Header files
#include <pic18.h>

#include "test.h"

#define LEDLATCH() __delay_ms(1)        // wait for LED to latch

// Globals for testing
volatile uint8_t selected = 0;
volatile bool switchSelected = false;

// Callback for heartbeat LED
void HeartbeatCallback(void)
{
    LED0_Toggle();
    
    selected = (++selected) % 3;
    switchSelected = true;
}

// Local function prototypes
static void TEST_OLED(void);
static void TEST_PIXELS(void);

// Other local function prototypes
void displayMenu(uint8_t selectedOption);
static void pixels_StartupSequence(void);
static void lowmemLED(void);
static void rainbowCycle(unsigned char frames , unsigned int frameAdvance, unsigned int pixelAdvance );

// Main Test function definition
void TEST_Function(void)
{
    // Register the callback function
    TMR1_SetInterruptHandler(HeartbeatCallback);
    
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
    
    displayMenu(0xFF);
    
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
        if(switchSelected)
        {
            switchSelected = false;
            
            displayMenu(selected);
        }
    }
}

static void TEST_PIXELS(void)
{   
    // Open SPI port
    SPI1_Open(SPI1_DEFAULT);
    
    // 2022-03-20: Minimal memory footprint test code
    RGB_Clear(LEDSTRIPSIZE);
    
    pixels_StartupSequence();
    __delay_ms(250);
    lowmemLED();
    
    while(1);
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
        __delay_ms(50);
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
        __delay_ms(50);
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
        __delay_ms(50);
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
        __delay_ms(50);
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
        __delay_ms(50);
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
        __delay_ms(50);
    }
    
    // Turn each LED Pink starting from the last LED
    __delay_ms(200);
    for(i = LEDSTRIPSIZE; i >= 0; i--)
    {
        pIndex = 0;
        while(pIndex++ < i)
        {
            RGB_SetColor(RGB_TO_VAL(0,0,0));
        }
        RGB_SetColor(RGB_TO_VAL(23, 0, 14));
        LEDLATCH();
        __delay_ms(50);
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
        __delay_ms(50);
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

            __delay_ms(50);
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

            __delay_ms(50);
        }
    }

    __delay_ms(100);
    
}

// Function below was taken from
// https://github.com/bigjosh/SimpleNeoPixelDemo/blob/master/SimpleNeopixelDemo/SimpleNeopixelDemo.ino
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
      unsigned char step = currentPixelHue & 0xff;
                 
      switch (phase) {
        
        case 0: 
          RGB_SetColor(RGB_TO_VAL(~step,step,0));
          break;
          
        case 1: 
          RGB_SetColor( RGB_TO_VAL(0 , ~step , step) );
          break;

        case 2: 
          RGB_SetColor(  RGB_TO_VAL(step ,0 , ~step) );
          break;
          
      }
      
      currentPixelHue+=pixelAdvance;                                      
      
                          
    } 
    LEDLATCH();
    
    firstPixelHue += frameAdvance;
           
  }
}

/**
 End of File
*/