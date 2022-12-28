

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
    // NOTE: Make sure SW0 (RE2) is connected to TM4_ers input (RC6)
    // NOTE: Make sure this callback function is registered
    if(!SW0_GetValue())
    {
        isSwitchPressed = true;
    }
    else
    {
        isSwitchPressed = false;
    }
}

// ************************************************************

// Local function prototypes
static void TEST_PIXELS(void);

// Other local function prototypes
void displayMenu(uint8_t selectedOption);
static void profitestCometsTail(uint8_t tailLen);

// TEST CODE FOR USER INPUTS ********************************************
static void TEST_USERINPUTS(void);

// ************************************************************

// Main Test function definition
void TEST_Function(void)
{
    // Register the heartbeat callback function
    TMR1_SetInterruptHandler(HeartbeatCallback);
    
    // Register TMR4 Callback
    //TMR4_SetInterruptHandler(DebounceCallback);
    
    // Test SPI LED functions
    //TEST_PIXELS();
    
    // Test the button debouncing state machine code
    TEST_USERINPUTS();
    
    return;
}

// TEST CODE FOR BUTTON DEBOUNCING SM ********************************************
static void TEST_USERINPUTS(void)
{
    // Initialize millis and some variables
    millis_Initialize();
    Button_Initialize();
    ROTENC_Initialize();
    
    // Variables
    uint64_t t0 = 0;
    char teststring[64];
    uint16_t cw_count, ccw_count;
    ROT_DIR rotDirection;
    const char *dir_ascii[] = {"CCW", "CW"};
    uint16_t rotEnc_InputCounter = 0;       // Counts buffered inputs
    
    // Make sure to clear the software framebuffer after initializing the OLED
    __delay_ms(100);
    OLED_Initialize();
    GFX_Clear();
    
    // To render the bitmap, call OLED_Tasks() until all pages have been updated.
    // Here, we assume that this process is finished after 1sec.
    // We should expect that the ring buffer is empty once all pages have been updated.
    GFX_Draw(nailandgear);
    GFX_Render();
    t0 = millis();
    
    // Keep calling OLED_Tasks until 1s has passed.
    while(millis() - t0 < 1000)
    {
        OLED_Tasks();
    }
    
    // Clear fbuf again so text has a blank canvas to work with.
    GFX_Clear();
    
    // Re-anchor t0
    t0 = millis();
    while(1)
    {
        Button_Tasks();
        OLED_Tasks();
        
        // Update the screen whenever possible (i.e. fastest refresh rate)
        if(!OLED_IsBusy())
        {
            sprintf(teststring, "SW Press: %lu", get_button_presscount());
            GFX_Text(0, 0, teststring, &font5x7, 0);

            sprintf(teststring, "Status P: %d H: %d", get_button_pressed(), get_button_held());
            GFX_Text(8, 0, teststring, &font5x7, 0);
            
            ROTENC_GetRotationCount(&cw_count, &ccw_count);
            sprintf(teststring, "CCW: %d CW: %d", ccw_count, cw_count);
            GFX_Text(16, 0, teststring, &font5x7, 0);
            
            rotDirection = ROTENC_ReadRingBuf();
            if(rotDirection != ROTENC_ERR)
            {
                sprintf(teststring, "Rotation: %s   ", dir_ascii[rotDirection]);
                GFX_Text(24, 0, teststring, &font5x7, 0);
                
                // Increment the buffered input counter.
                rotEnc_InputCounter++;
            }
            else
            {
                sprintf(teststring, "Rotation: ---------");
                GFX_Text(24, 0, teststring, &font5x7, 0);
            }
            
            sprintf(teststring, "Buffered: %u", rotEnc_InputCounter);
            GFX_Text(32, 0, teststring, &font5x7, 0);
            
            sprintf(teststring, "millis = %llu", millis());
            GFX_Text(40, 0, teststring, &font5x7, 0);
            
            GFX_Render();
            
            t0 = millis();
        }
    }
}



// ************************************************************
// Local function definitions
static void TEST_PIXELS(void)
{   
    // Open SPI port
    SPI1_Open(SPI1_DEFAULT);
    
    // 2022-03-20: Minimal memory footprint test code
    RGB_Clear(LEDSTRIPSIZE);
    
    profitestCometsTail(6);
    
    while(1)
    {
        
    }
    return;
}

// Other local test functions
void displayMenu(uint8_t selectedOption)
{
//    OLED_DrawString(0, 0, "Main Menu", font5x7, 0);
//    OLED_DrawString(8, 5, "Select LED Profile", font5x7, (selectedOption == 0));
//    OLED_DrawString(16, 5, "Adjust LED Count", font5x7, (selectedOption == 1));
//    OLED_DrawString(24, 5, "About", font5x7, (selectedOption == 2));
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

/**
 End of File
*/