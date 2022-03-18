//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// spi_led.c -- SPI Driver for Addressable LEDs
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: mjneri
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: spi_led.c
// Description:
//
// Revisions:
// Revision 0.01 - File Created (2022-03-18)
// Additional Comments:
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// *****************************************************************************
// *****************************************************************************
// Section: Header Includes
// *****************************************************************************
// *****************************************************************************

#include "spi_led.h"

// *****************************************************************************
// *****************************************************************************
// Section: Local Function Defines
// *****************************************************************************
// *****************************************************************************

static void RGB_SendData(rgb_led_t rgbData);

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

void RGB_Init(rgb_led_t *buf, uint16_t size)
{
    // Initialize the array
    memset(buf, 0, size*sizeof(rgb_led_t));
    
    // Clear each LED in the array
    RGB_Update(buf, size);
    
    // wait until the LEDs reset
    __delay_ms(1);
    
    return;
}

void RGB_Clear(rgb_led_t *buf, uint16_t size)
{
    // This function is basically the same as RGB_Init()
    RGB_Init(buf, size);
    return;
}

void RGB_SetColor(rgb_led_t *led, uint24_t val)
{
    // Set the color values of a specific LED
    led->val = val;
    return;
}

void RGB_ALLSetColor(rgb_led_t *buf, uint16_t size, uint24_t val)
{
    // Set the color values of all LEDs
    uint16_t i = 0;
    for(i = 0; i < size; i++)
    {
        buf[i].val = val;
    }
}

void RGB_Update(rgb_led_t *buf, const uint16_t size)
{
    // Update each LED
    uint16_t i = 0;
    for(i = 0; i < size; i++)
    {
        RGB_SendData(buf[i]);
    }
}

static void RGB_SendData(rgb_led_t rgbData)
{
    SPI1_WriteByte(rgbData.green);
    SPI1_WriteByte(rgbData.red);
    SPI1_WriteByte(rgbData.blue);
}

/*******************************************************************************
 End of File
*/

