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

/* For use with PIXELS_SelectProfile */
/* For examples that use function pointers, see:
   https://github.com/FastLED/FastLED/blob/master/examples/DemoReel100/DemoReel100.ino */
#define PIXEL_PROFILE_COUNT 11       // How many profiles are available?


// *****************************************************************************
// *****************************************************************************
// Section: Data type definitions
// *****************************************************************************
// *****************************************************************************

typedef enum
{
    PIXEL_PROFILE_INDEX_WALK = 0,
    PIXEL_PROFILE_INDEX_COMETSTAIL,
    PIXEL_PROFILE_THEATERCHASE,
    PIXEL_PROFILE_ONECOLORPULSE,
    PIXEL_PROFILE_SOLIDCOLOR,
    PIXEL_PROFILE_COLORWAVE,
    PIXEL_PROFILE_SOUNDREACTIVE,
    PIXEL_PROFILE_MULTICOLORPULSE,
    PIXEL_PROFILE_SPARKLES,
    PIXEL_PROFILE_RAINBOWCYCLE,
    PIXEL_PROFILE_ALTERNATINGCOLORS,
            
    PIXEL_PROFILE_INVALID
} pixel_profile_index_t;

// Array of function pointers pointing to each profile.
// Will not work if each profile takes different arguments
// https://www.geeksforgeeks.org/function-pointer-in-c/
//typedef void (* profileList ) (void);

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

void PIXELS_Initialize(void);

int PIXELS_SelectProfile(pixel_profile_index_t index);

void PIXELS_Tasks(void);

void PIXEL_Walk(void);
void PIXEL_CometsTail(void);
void PIXEL_TheaterChase(void);
void PIXEL_OneColorPulse(void);
void PIXEL_SolidColor(void);
void PIXEL_ColorWave(void);
void PIXEL_SoundReactive(void);
void PIXEL_MultiColorPulse(void);
void PIXEL_Sparkles(void);
void PIXEL_RainbowCycle(void);
void PIXEL_AlternatingColors(void);


#endif /*_LED_PROFILES_H_*/

/*******************************************************************************
 End of File
*/

