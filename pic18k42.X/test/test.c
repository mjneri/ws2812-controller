

// Header files
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

// Main Test function definition
void TEST_Function(void)
{
    // Register the callback function
    TMR1_SetInterruptHandler(HeartbeatCallback);
    
    // Test the OLED functions
    TEST_OLED();
    
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