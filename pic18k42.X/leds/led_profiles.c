//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// led_profiles.c -- All LED Profiles defined here
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: mjneri
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: led_profiles.c
// Description: Interface between user app and low-level SPI code.
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

#include "led_profiles.h"

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

void cometsTail(uint8_t tailLen)
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

void theaterChase(uint8_t szLight, uint8_t szSpace)
{
    uint16_t i, p = 0;
    uint16_t combinedSegLen = szlight + szspace;
    uint16_t ledOffset = combinedSegLen;
    
    while(1)
    {
        for(i=0, p=0+ledOffset; i < LEDSTRIPSIZE; i++, p=(p+1)%combinedSegLen)
        {
            // quick code - needs to be shortened/optimized later?
            if(p < szlight)
            {
                RGB_SetColor(RGB_TO_VAL(0x19,0,0));
            }
            else
            {
                RGB_SetColor(0);
            }
        }
        LEDLATCH();
        __delay_ms(20);
        
        // Uncomment the other to reverse direction
        //ledOffset = (ledOffset+1) % combinedSegLen;
        ledOffset = (ledOffset == 0)? combinedSegLen-1 : (ledOffset-1) % combinedSegLen;
    }
}

void oneColorPulse(void)
{
    int16_t i = 0;
    
    while(1)
    {
        for(i = 0; i < 64; i++)
        {
            RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(i,0,0));
            __delay_ms(10);
        }

        for(i = 64; i >= 0; i--)
        {
            RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(i,0,0));
            __delay_ms(10);
        }
        __delay_ms(100);
    }
}

void solidColor(uint24_t rgb)
{
    uint16_t i = 0;
    
    // Cycle between 7 colors
    while(1)
    {
        switch(i)
        {
            case 1:
                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(0,0,25));
                break;
            case 2:
                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(0,25,0));
                break;
            case 3:
                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(0,25,25));
                break;
            case 4:
                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(25,0,0));
                break;
            case 5:
                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(25,0,25));
                break;
            case 6:
                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(25,25,0));
                break;
            case 7:
                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(25,25,25));
                break;
            default:
                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(0,0,0));
                break;
        }
        
        i = (i+1) % 8;
        i = (i==0)? 1 : i;
        LEDLATCH();
        __delay_ms(1000);
    }
}

void colorWave(void)
{
    
}

void soundReactive(void)
{
    
}

void multiColorPulse(void)
{
    
}

void sparkles(void)
{
    
}

void rainbowCycle(void)
{
    
}

void alternatingColors(void)
{
    
}

/*******************************************************************************
 End of File
*/

