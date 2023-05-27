//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// buttons.h -- Header file for buttons.c
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: mjneri
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: buttons.h
// Description: 
//
// Revisions:
// Revision 0.01 - File Created (2022-04-09)
// Additional Comments:
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Header guard
#ifndef _BUTTONS_H_
#define _BUTTONS_H_

// *****************************************************************************
// *****************************************************************************
// Section: Header Files
// *****************************************************************************
// *****************************************************************************

#include "../mcc_generated_files/mcc.h"

// *****************************************************************************
// *****************************************************************************
// Section: Constant Defines
// *****************************************************************************
// *****************************************************************************

#define DEBOUNCECOUNT 64
#define LONGPRESSCOUNT 16384

#ifndef BUTTON_GetValue()
#define BUTTON_GetValue()   ROT_SW_GetValue()
#endif

// For debugging purposes only. Only one macro should be used.
//#define DEBOUNCE_DEBUG
//#define DEBUG_BTNTASKRATE

// *****************************************************************************
// *****************************************************************************
// Section: Data type definitions
// *****************************************************************************
// *****************************************************************************

typedef enum
{
    BTN_INIT,
    BTN_WAIT_PRESS,
    BTN_DEBOUNCE,
    BTN_PRESSED,
    BTN_HELD
} BUTTON_STATE;

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

bool get_button_pressed(void);           // interface func
bool get_button_held(void);              // interface func
uint32_t get_button_presscount(void);
void reset_button_presscount(void);

void Button_Initialize(void);
void Button_Tasks(void);             // button state machine maintainer


#endif /*_BUTTONS_H_*/

/*******************************************************************************
 End of File
*/

