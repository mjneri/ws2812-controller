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

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

void RGB_SPI_Initialize(void)
{
    // Open SPI port
    SPI1_Open(SPI1_HDX_TXONLY);
    
    // Turn off all LEDs
    RGB_SPI_Clear(LEDSTRIPSIZE);
    
    return;
}

void RGB_SPI_Clear(uint16_t ledcount)
{
    for(int i = 0; i < ledcount; i++)
    {
        RGB_SPI_Write(RGB_TO_VAL(0, 0, 0));
        
        if(!RGB_SPI_IsTxReady())
        {
            break;
        }
    }
    
    return;
}

bool RGB_SPI_Write(uint24_t val)
{
    if(!RGB_SPI_IsTxReady())
    {
        return false;
    }
    
    rgb_led_t rgbData = (rgb_led_t) val;
    
    SPI1_Write(rgbData.green);
    SPI1_Write(rgbData.red);
    SPI1_Write(rgbData.blue);
    
    return true;
}

bool RGB_SPI_IsTxReady(void)
{
    // Return true only if there's at least 3 bytes of free space
    return ((SPI1_GetBufferSize() >= 3)? true : false);
}

/*******************************************************************************
 End of File
*/

