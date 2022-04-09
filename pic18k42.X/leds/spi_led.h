//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// spi_led.h -- Header file for spi_led.c
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: mjneri
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: spi_led.h
// Description: Provides data type defines and function prototypes
//
// Revisions:
// Revision 0.01 - File Created (2022-03-18)
// Additional Comments:
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Header guard
#ifndef _SPI_LED_H_
#define _SPI_LED_H_

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

#define LEDSTRIPSIZE 120

// *****************************************************************************
// *****************************************************************************
// Section: Macro Defines
// *****************************************************************************
// *****************************************************************************

#define RGB_TO_VAL(r, g, b) ( (uint24_t) (((uint32_t)(b) << 16) | ((uint32_t)(r) << 8) | ((uint32_t)(g))) )

#define LEDLATCH() __delay_ms(1)        // wait for LED to latch

// *****************************************************************************
// *****************************************************************************
// Section: Data type definitions
// *****************************************************************************
// *****************************************************************************

/* rgb_led_t
 *      + This holds RGB data for an
 *          individual addressable LED
 *      + It's expected that an array of this data type is declared.
 *      + No linked lists for now.
 */
typedef union
{
    uint24_t val;
    struct
    {
        uint8_t green;      // Stored in lowest address
        uint8_t red;
        uint8_t blue;       // Stored in highest address
    };
} rgb_led_t;

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

/* A Note:
 *      "uint24_t val" args for the functions below use the following format:
 *          {green<23:16>, red<15:8>, blue<7:0>}
 */

// RGB_Clear(void)
//      + Turns off all LEDs in the array
void RGB_Clear(uint16_t ledcount);

// RGB_SetColor();
//      + Sets the RGB brightness values of an individual LED
//      + Pass by reference so the function can modify the variable itself
void RGB_SetColor(uint24_t val);

// RGB_ALLSetColor();
//      + Sets the RGB brightness values of all LEDs in the array
void RGB_ALLSetColor(uint16_t ledcount, uint24_t val);

#endif /*_SPI_LED_H_*/

/*******************************************************************************
 End of File
*/

