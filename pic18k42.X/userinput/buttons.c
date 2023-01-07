//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// buttons.c -- Button debouncing
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: mjneri
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: buttons.c
// Description: Debounces button inputs through a state machine
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

#include "buttons.h"

// *****************************************************************************
// *****************************************************************************
// Section: Variables
// *****************************************************************************
// *****************************************************************************

BUTTON_STATE buttonState;

bool is_button_pressed;
bool is_button_held;
uint32_t button_press_count;         // how many button presses?
uint8_t pressDebCount;               // used for debouncing button presses
uint8_t unpressDebCount;             // for debouncing button unpresses
uint16_t longPressCount;              // for detecting long presses

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

bool get_button_pressed(void)
{
    return is_button_pressed;
}

bool get_button_held(void)
{
    return is_button_held;
}

uint32_t get_button_presscount(void)
{
    return button_press_count;
}

void reset_button_presscount(void)
{
    button_press_count = 0;
}

void Button_Initialize(void)
{
    buttonState = BTN_INIT;
    
    is_button_pressed = false;
    is_button_held = false;
    button_press_count = 0;
    pressDebCount = 0;
    unpressDebCount = 0;
    longPressCount = 0;
}

void Button_Tasks(void)
{
#ifdef DEBUG_BTNTASKRATE
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
            DEBUG_GPIO_OUT_SetHigh();
#endif
            
            if(!BUTTON_GetValue())
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
            if(!BUTTON_GetValue())
            {
                unpressDebCount = 0;    // Reset this since button is pressed
                if(pressDebCount > DEBOUNCECOUNT)
                {
                    pressDebCount = 0;  // Reset
                    button_press_count++;       // Increment button press count here
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
            
#ifdef DEBOUNCE_DEBUG
            DEBUG_GPIO_OUT_SetLow();
#endif
            
            if(!BUTTON_GetValue())
            {
                // Check for long press
                if(longPressCount >= LONGPRESSCOUNT)
                {
                    longPressCount = 0;
                    buttonState = BTN_HELD;
                }
                else
                {
                    longPressCount++;
                    buttonState = BTN_PRESSED;
                }
            }
            else
            {
                longPressCount = 0;
                buttonState = BTN_DEBOUNCE;
            }
            break;
        }
        
        case BTN_HELD:
        {
            is_button_held = true;
            is_button_pressed = true;
            
            if(!BUTTON_GetValue())
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

/*******************************************************************************
 End of File
*/

