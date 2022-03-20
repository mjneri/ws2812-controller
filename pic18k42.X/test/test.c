

// Header files
#include <pic18.h>

#include "test.h"

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
static void pixels_StartupSequence(rgb_led_t *pixelObj, uint16_t size);
static void lowmemLED(void);

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

static void pixels_StartupSequence(rgb_led_t *pixelObj, uint16_t size)
{
    // Start-up sequence for the LEDs
    int16_t i = 0;
    rgb_led_t temp;
    
    // Initialize the temp var
    temp.val = 0;
    
    // Walking white light
    // NOTE: "white" is not being shifted between array members. Rather, 
    // each LED is turned white via PIXEL_SetColor() then turned off by clearing
    // the LED's color values before the next iteration of the for loop.
    // Hope this makes sense.
    for(i = 0; i < size; i++)
    {
        // Turn the LED white
        RGB_SetColor(&pixelObj[i], RGB_TO_VAL(25, 25, 25));
        RGB_Update(pixelObj, size);
        
        // Clear the LED color values
        pixelObj[i].val = 0;
        __delay_ms(50);
    }
    
    // Reverse the direction!
    for(i = size - 1; i >= 0; i--)
    {
        // Turn the LED white
        RGB_SetColor(&pixelObj[i], RGB_TO_VAL(25, 25, 25));
        RGB_Update(pixelObj, size);
        
        // Clear the LED color values
        pixelObj[i].val = 0;
        __delay_ms(50);
    }
    
    // Turn each LED Red
    for(i = 0; i < size; i++)
    {
        RGB_SetColor(&pixelObj[i], RGB_TO_VAL(25, 1, 1));
        RGB_Update(pixelObj, size);
        __delay_ms(50);
    }
    
    // Turn off each LED one by one
    for(i = 0; i < size; i++)
    {
        // Clear the LED color values
        RGB_SetColor(&pixelObj[i], RGB_TO_VAL(0, 0, 0));
        RGB_Update(pixelObj, size);
        __delay_ms(50);
    }
    
    // Turn each LED blue starting from the center of the stick
    __delay_ms(200);
    for(i = size/2 - 1; i >= 0; i--)
    {
        RGB_SetColor(&pixelObj[i], RGB_TO_VAL(1, 1, 25));                 // LEDs 4, 3, 2, 1, 0
        RGB_SetColor(&pixelObj[size-1-i], RGB_TO_VAL(1, 1, 25));    // LEDs 5, 6, 7, 8, 9
        RGB_Update(pixelObj, size);
        __delay_ms(50);
    }
    
    // Turn each LED off using the same pattern as above
    __delay_ms(200);
    for(i = size/2 - 1; i >= 0; i--)
    {
        RGB_SetColor(&pixelObj[i], RGB_TO_VAL(0,0,0));                 // LEDs 4, 3, 2, 1, 0
        RGB_SetColor(&pixelObj[size-1-i], RGB_TO_VAL(0,0,0));    // LEDs 5, 6, 7, 8, 9
        RGB_Update(pixelObj, size);
        __delay_ms(50);
    }
    
    // Turn each LED Pink starting from the last LED
    __delay_ms(200);
    for(i = size - 1; i >= 0; i--)
    {
        RGB_SetColor(&pixelObj[i], RGB_TO_VAL(23, 0, 14));
        RGB_Update(pixelObj, size);
        __delay_ms(50);
    }
    
    // Turn each LED off
    __delay_ms(200);
    for(i = 0; i < size; i++)
    {
        RGB_SetColor(&pixelObj[i], RGB_TO_VAL(0, 0, 0));
        RGB_Update(pixelObj, size);
        __delay_ms(50);
    }
    
    // Breathing Green LEDs
    __delay_ms(200);
    for(i = 0; i < 64; i++)
    {
        RGB_ALLSetColor(pixelObj, size, RGB_TO_VAL(0, i, 0));
        RGB_Update(pixelObj, size);
        __delay_ms(5);
    }
    
    for(i = 64; i >= 0; i--)
    {
        RGB_ALLSetColor(pixelObj, size, RGB_TO_VAL(0, i, 0));
        RGB_Update(pixelObj, size);
        __delay_ms(5);
    }
}

static void lowmemLED(void)
{
    uint16_t i = 0;
    
    // Turn all LEDs Blue
    for(i=LEDSTRIPSIZE; i-- > 0;)
    {
        RGB_SetColor(i, RGB_TO_VAL(0,0,25));
        
        // Check the LED's datasheet for the minimum time for RES
        __delay_us(310);
    }
    
    // Turn LED's off one-by one
    for(i=LEDSTRIPSIZE;)
}

/**
 End of File
*/