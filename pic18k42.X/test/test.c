

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
    
    displayMenu(0);
    
    while(1)
    {
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