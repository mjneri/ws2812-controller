//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// led_profiles.h -- Header file for led_profiles.c
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: mjneri
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: led_profiles.h
// Description: 
//
// Revisions:
// Revision 0.01 - File Created (2022-04-09)
// Additional Comments:
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Header guard
#ifndef _LED_PROFILES_H_
#define _LED_PROFILES_H_

// *****************************************************************************
// *****************************************************************************
// Section: Header Files
// *****************************************************************************
// *****************************************************************************

#include "spi_led.h"

// *****************************************************************************
// *****************************************************************************
// Section: Constant Defines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Data type definitions
// *****************************************************************************
// *****************************************************************************

// Array of function pointers pointing to each profile.
// Will not work if each profile takes different arguments
// https://www.geeksforgeeks.org/function-pointer-in-c/
//typedef void (* profileList ) (void);

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

// Hopefully the function names explain how the profiles look on an LED strip.

void cometsTail(uint8_t tailLen);
void theaterChase(uint8_t szLight, uint8_t szSpace);
void oneColorPulse(void);
void solidColor(uint24_t rgb);
void colorWave(void);
void soundReactive(void);
void multiColorPulse(void);
void sparkles(void);
void rainbowCycle(void);
void alternatingColors(void);


#endif /*_LED_PROFILES_H_*/

/*******************************************************************************
 End of File
*/

