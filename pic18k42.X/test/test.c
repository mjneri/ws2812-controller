

// Header files
#include <pic18.h>

#include "test.h"

#define LEDLATCH() __delay_ms(1)        // wait for LED to latch

// Globals for testing
volatile uint8_t selected = 0;
volatile bool switchSelected = false;
volatile bool isSwitchPressed = false;
volatile bool isSwitchHeld = false;

volatile uint16_t rotCntCCW = 0;
volatile uint16_t rotCntCW = 0;

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

void CLC2_Callback(void)
{
    if(CLC2_OutputStatusGet())
    {
        rotCntCCW++;
    }
}

void CLC3_Callback(void)
{
    if(CLC3_OutputStatusGet())
    {
        rotCntCW++;
    }
}

void RotClkDebounce(void)
{
    if(!RC6_GetValue())
    {
        DEB_ROT_A_SetLow();
    }
    else
    {
        DEB_ROT_A_SetHigh();
    }
}

void RotDTDebounce(void)
{
    if(!RD0_GetValue())
    {
        DEB_ROT_B_SetLow();
    }
    else
    {
        DEB_ROT_B_SetHigh();
    }
}

// TEST CODE FOR MILLIS() ********************************************
static void TEST_MILLIS(void);

// ************************************************************

// Local function prototypes
static void TEST_OLED(void);
static void TEST_PIXELS(void);
static void TEST_ROTARYENCODER(void);

// Other local function prototypes
void displayMenu(uint8_t selectedOption);
static void profitestCometsTail(uint8_t tailLen);

// TEST CODE FOR BUTTON DEBOUNCING SM ********************************************
typedef enum
{
    BTN_INIT,
    BTN_WAIT_PRESS,
    BTN_DEBOUNCE,
    BTN_PRESSED,
    BTN_HELD
} button_state_t;

#define DEBOUNCECOUNT 1
//#define DEBOUNCE_DEBUG

static void Test_ButtonTasks(void);             // button state machine maintainer
static bool get_button_pressed(void);           // interface func
static bool get_button_held(void);              // interface func
static uint32_t get_button_presscount(void);
static void reset_button_presscount(void);

static bool is_button_pressed = false;
static bool is_button_held = false;
static uint32_t button_press_count = 0;         // how many button presses?
static uint8_t pressDebCount = 0;               // used for debouncing button presses
static uint8_t unpressDebCount = 0;             // for debouncing button unpresses
static button_state_t buttonState = BTN_INIT;

static void TEST_BUTTON_SM(void);

// ************************************************************

// Main Test function definition
void TEST_Function(void)
{
    // Register the heartbeat callback function
    TMR1_SetInterruptHandler(HeartbeatCallback);
    
    // Register TMR4 Callback
    //TMR4_SetInterruptHandler(DebounceCallback);
    
    // Test the OLED functions
    //TEST_OLED();
    
    // Test SPI LED functions
    //TEST_PIXELS();
    
    // Test the rotary encoder
    //TEST_ROTARYENCODER();
    
    // Register callbacks for millis() & run test code
    //TEST_MILLIS();
    
    // Test the button debouncing state machine code
    TEST_BUTTON_SM();
    
    return;
}

// TEST CODE FOR BUTTON DEBOUNCING SM ********************************************
static void TEST_BUTTON_SM(void)
{
    millis_Initialize();
    buttonState = BTN_INIT;
    
    __delay_ms(100);
    OLED_Initialize();
    OLED_ClearDisplay();
    
    OLED_DrawBitmap();
    __delay_ms(1000);
    OLED_ClearDisplay();
    
    char teststring[64];
    
    while(1)
    {
        Test_ButtonTasks();
        
        sprintf(teststring, "%lu", get_button_presscount());
        OLED_DrawString(0, 0, teststring, font5x7, 0);
        
        sprintf(teststring, "P: %d H: %d", get_button_pressed(), get_button_held());
        OLED_DrawString(8, 0, teststring, font5x7, 0);
    }
}

static bool get_button_pressed(void)
{
    return is_button_pressed;
}

static bool get_button_held(void)
{
    return is_button_held;
}

static uint32_t get_button_presscount(void)
{
    return button_press_count;
}

static void reset_button_presscount(void)
{
    button_press_count = 0;
}

static void Test_ButtonTasks(void)
{
    // local variables with limited scope
    uint64_t t0 = millis();
    
#ifdef DEBOUNCE_DEBUG
    DEBUG_GPIO_OUT_Toggle();
#endif
    
    switch(buttonState)
    {
        case BTN_INIT:
        {
            is_button_held = false;
            is_button_pressed = false;
            button_press_count = 0;
            pressDebCount = 0;
            unpressDebCount = 0;
            
            buttonState = BTN_WAIT_PRESS;
            break;
        }
        
        case BTN_WAIT_PRESS:
        {
            is_button_pressed = false;
            is_button_held = false;
            
#ifdef DEBOUNCE_DEBUG
            //DEBUG_GPIO_OUT_SetHigh();
#endif
            
            if(!SW0_GetValue())
            {
                buttonState = BTN_DEBOUNCE;
            }
            else
            {
                buttonState = BTN_WAIT_PRESS;
            }
            break;
        }
        
        case BTN_DEBOUNCE:
        {
            if(!SW0_GetValue())
            {
                unpressDebCount = 0;    // Reset this since button is pressed
                if(pressDebCount > DEBOUNCECOUNT)
                {
                    pressDebCount = 0;  // Reset
                    buttonState = BTN_PRESSED;
                }
                else
                {
                    pressDebCount++;
                    buttonState = BTN_DEBOUNCE;
                }
            }
            else
            {
                pressDebCount = 0;      // Reset this since button is not pressed
                if(unpressDebCount > DEBOUNCECOUNT)
                {
                    unpressDebCount = 0;
                    buttonState = BTN_WAIT_PRESS;
                }
                else
                {
                    unpressDebCount++;
                    buttonState = BTN_DEBOUNCE;
                }
            }
            break;
        }
        
        case BTN_PRESSED:
        {
            is_button_pressed = true;
            button_press_count++;
            
#ifdef DEBOUNCE_DEBUG
            //DEBUG_GPIO_OUT_SetLow();
#endif
            
            if(!SW0_GetValue())
            {
                buttonState = BTN_HELD;
            }
            else
            {
                buttonState = BTN_DEBOUNCE;
            }
            break;
        }
        
        case BTN_HELD:
        {
            is_button_held = true;
            is_button_pressed = true;
            
            if(!SW0_GetValue())
            {
                buttonState = BTN_HELD;
            }
            else
            {
                buttonState = BTN_DEBOUNCE;
            }
            break;
        }
        
        default:
        {
            break;
        }
    }
}

// ************************************************************

static void TEST_MILLIS(void)
{
    millis_Initialize();
    
    OLED_Initialize();
    OLED_ClearDisplay();
    
    OLED_DrawBitmap();
    __delay_ms(1000);
    OLED_ClearDisplay();
    
    char teststring[64];
    
    uint64_t init_millis = millis();
    
    uint32_t seconds_counter = 0;
    
    while(1)
    {
        sprintf(teststring, "%llu", millis() - init_millis);
        OLED_DrawString(0, 0, teststring, font5x7, 0);
    }
}

// Local function definitions
static void TEST_ROTARYENCODER(void)
{
    // Register callbacks for rotary encoder debouncing
    // Note: TMR4 and TMR6 period is 1ms
    TMR4_SetInterruptHandler(RotClkDebounce);
    TMR6_SetInterruptHandler(RotDTDebounce);
    
    OLED_Initialize();
    OLED_ClearDisplay();
    
    OLED_DrawBitmap();
    __delay_ms(1000);
    OLED_ClearDisplay();
    
    // Quick software thing
    uint32_t i = 0;
    char teststring[64];
    while(1)
    {
        // Insert code interpreting the decoded signals on CLC2OUT and CLC3OUT
        sprintf(teststring, "%d %d", rotCntCCW, rotCntCW);
        OLED_DrawString(0, 0, teststring, font5x7, 0);
    }
}

static void TEST_OLED(void)
{
    OLED_Initialize();
    OLED_ClearDisplay();
    
    OLED_DrawBitmap();
    __delay_ms(1000);
    OLED_ClearDisplay();
//    OLED_DrawString(0, 20, "Hello world!", font5x7, false);
//    OLED_DrawString(9, 0, "The quick brown fox", font5x7, true);
    
    // For dummy read
    //displayMenu(0);
    
    // Quick software thing - count how long switch is pressed
    uint32_t i = 0;
    
    while(1)
    {
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
    
    profitestCometsTail(6);
    
    while(1)
    {
        
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