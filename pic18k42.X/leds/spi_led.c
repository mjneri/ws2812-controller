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

void RGB_Init(rgb_led_t *buf, uint16_t ledcount)
{
    // Initialize the array
    memset(buf, 0, ledcount*sizeof(rgb_led_t));
    
    // Clear each LED in the array
    RGB_Update(buf, ledcount);
    
    // wait until the LEDs reset
    __delay_ms(1);
    
    return;
}

void RGB_Clear(uint16_t ledcount)
{
    rgb_led_t temp;
    
    for(uint16_t i = 0; i < ledcount; i++)
    {
        temp.val = 0;
        RGB_SendData(temp);
    }
    __delay_ms(1);
    return;
}

void RGB_SetColor(uint16_t ledIndex, uint24_t val)
{
    rgb_led_t temp;
    temp.val = 0;
    
    for(uint16_t i = 0; i < ledIndex+1; i++)
    {
        temp.val = (i==ledIndex)? val : 0;
        RGB_SendData(temp);
    }
    return;
}

void RGB_ALLSetColor(rgb_led_t *buf, uint16_t ledcount, uint24_t val)
{
    // Set the color values of all LEDs
    uint16_t i = 0;
    for(i = 0; i < ledcount; i++)
    {
        buf[i].val = val;
    }
}

void RGB_Update(rgb_led_t *buf, const uint16_t ledcount)
{
    // Update each LED
    uint16_t i = 0;
    for(i = 0; i < ledcount; i++)
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

