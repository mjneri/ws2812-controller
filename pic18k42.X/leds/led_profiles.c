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
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

uint64_t pixels_t0 = 0;                         // Used for determining when to update the frame  
static uint16_t cp_framecount = LEDSTRIPSIZE;   // Current profile framecount; # of frames for the current profile
static uint16_t current_frame = 0;              // # of current frame
static uint16_t current_led = 0;                // Indicates which LED is being updated              
static bool didProfileChange = false;           // Set if profile was recently changed.
static bool isframedone = false;                // Checked by Tasks, updated by profile functions.
pixel_profile_index_t currentProfileIndex = PIXEL_PROFILE_INDEX_WALK;

// Function pointer array. Functions added here should not take arguments.
void (*pixelProfileList[PIXEL_PROFILE_COUNT])(void) = 
{
    PIXEL_Walk,
    PIXEL_CometsTail,
    PIXEL_TheaterChase,
    PIXEL_OneColorPulse,
    PIXEL_SolidColor,
    PIXEL_ColorWave,
    PIXEL_SoundReactive,
    PIXEL_MultiColorPulse,
    PIXEL_Sparkles,
    PIXEL_RainbowCycle,
    PIXEL_AlternatingColors
};

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

void PIXELS_Initialize(void)
{
    RGB_SPI_Initialize();
        
    pixels_t0 = millis();
}

int PIXELS_SelectProfile(pixel_profile_index_t index)
{
    // Check if profile index is valid
    if(index >= PIXEL_PROFILE_INVALID)
    {
        return -1;
    }
    
    currentProfileIndex = index;
    
    // Reset state variables
    current_frame = 0;
    current_led = 0;
    
    // Code below is "hard-coded" for now. Eventually, it will be replaced with 
    // a more generalized implementation using structs such that names of pixel profiles
    // won't need to be specified.
    switch(currentProfileIndex)
    {
        case PIXEL_PROFILE_INDEX_WALK:
        {
            cp_framecount = LEDSTRIPSIZE;
            break;
        }
        
        case PIXEL_PROFILE_INDEX_COMETSTAIL:
        {
            cp_framecount = LEDSTRIPSIZE + 7;   // 7 is tailLen
            break;
        }
        
        case PIXEL_PROFILE_THEATERCHASE:
        {
            cp_framecount = 9;  // 9 is combinedSegLen
            break;
        }
        
        default:
            break;
    }
    
    didProfileChange = true;
    return 0;
}

void PIXELS_Tasks(void)
{
    if(!RGB_SPI_IsTxReady())
    {
        return;
    }
    
    if(isframedone)
    {
        if(millis() - pixels_t0 < 25)
        {
            // Update only every 25ms
            // Note: at 800kHz, it takes ~3-4ms to update 120 WS2812B LEDs.
            // 120LEDs * 3 bytes/LED * 8 bits/byte / 800kHz = 3.6ms
            // At > 833 LEDs, it will take more than 25ms to update all of them.
            // As long there are less than 833 LEDs, this framerate should be "fine" (subject to testing)
            
            /* 2023-05-20: Instead of hardcoded 25ms, consider using a variable.
             This way, each profile can run at different framerates. */
            return;
        }
    }
        
    isframedone = false;
    
    // Replace this later - see pseudocode in OneNote
    //TEST_PIXEL_Walk();
    //TEST_PIXEL_COMETSTAIL();
    //TEST_PIXEL_THEATERCHASE();
    (*pixelProfileList[currentProfileIndex])();
    pixels_t0 = millis();
}

// *****************************************************************************
// *****************************************************************************
// Section: Pixel Function Definitions
// *****************************************************************************
// *****************************************************************************

void PIXEL_Walk(void)
{
    static uint16_t activeLed = 0;  // determines which LED is turned on.
    
    if(didProfileChange)
    {
        // Reset the activeLed
        activeLed = 0;
        didProfileChange = false;
    }
    
    while(RGB_SPI_IsTxReady())
    {
        if(current_frame < cp_framecount)
        {
            if(current_led == activeLed)
            {
                // Ideally, application will check SPI1_Write's return value.
                // Here, for simplicity, we just assume it will always return 0;
                RGB_SPI_Write(RGB_TO_VAL(0x10, 0, 0));

                // Advance frame
                current_frame++;
                isframedone = true;
                current_led = 0;
                activeLed = (activeLed+1) % LEDSTRIPSIZE;
                break;
            }
            else
            {
                RGB_SPI_Write(RGB_TO_VAL(0, 0, 0));

                // Advance current led
                current_led++;
            }
        }
        else
        {
            current_frame = 0;
        }
    }
}

void PIXEL_CometsTail(void)
{
    uint8_t tailLen = 7;    // Hard-coded value.
    
    // Determines where the comet's head and tail are located
    static int16_t cometHeadLoc = 0;
    static int16_t cometTailLoc = 0;
    static bool isForward = true;   // Determines comet flight direction
    
    //uint16_t frames = cp_framecount + tailLen;
    // Update cp_framecount. NOTE: This shall evenetually be done by TEST_PIXEL_SelectProfile
    //cp_framecount = LEDSTRIPSIZE + tailLen;
    
    if(didProfileChange)
    {
        // Reset the static variables
        cometHeadLoc = 0;
        cometTailLoc = 0;
        isForward = true;
        didProfileChange = false;
    }
    
    while(RGB_SPI_IsTxReady())
    {
        // Forward comet flight
        if(isForward)
        {
            if(current_frame < cp_framecount)
            {
                if(current_led < cometTailLoc)
                {
                    RGB_SPI_Write(RGB_TO_VAL(0, 0, 0));
                    current_led++;
                }
                else if(current_led < cometHeadLoc)
                {
                    RGB_SPI_Write(RGB_TO_VAL(0, 0, 0x7f>>(cometHeadLoc-current_led)));
                    current_led++;
                }
                else if(current_led == cometHeadLoc)
                {
                    RGB_SPI_Write(RGB_TO_VAL(0,0,0x7f));
                    
                    // Advance comet head/tail locations
                    cometHeadLoc = (cometHeadLoc < LEDSTRIPSIZE-1)? cometHeadLoc+1 : LEDSTRIPSIZE-1;
                    cometTailLoc = (cometHeadLoc < tailLen)? 0 : \
                        (cometHeadLoc == LEDSTRIPSIZE-1)? cometTailLoc+1 : cometHeadLoc - tailLen;
                                        
                    // Advance frame
                    current_frame++;
                    isframedone = true;
                    current_led = 0;
                    return;
                }
                else
                {
                    // Do nothing. This condition should not be reached.
                    return;
                }
            }
            else
            {
                // Frames for forward flight are done "rendering". Time to reverse the flight
                current_frame = 0;
                isForward = false;
            }
        }
        else
        {
            // Reverse comet flight
            if(current_frame < cp_framecount)
            {
                if(current_led < cometHeadLoc)
                {
                    RGB_SPI_Write(RGB_TO_VAL(0, 0, 0));
                    current_led++;
                }
                else if(current_led == cometHeadLoc)
                {
                    RGB_SPI_Write(RGB_TO_VAL(0, 0, 0x7f));
                    current_led++;
                }
                else if(current_led <= cometTailLoc)
                {
                    RGB_SPI_Write(RGB_TO_VAL(0,0,0x7f>>(current_led-cometHeadLoc)));
                    current_led++;
                }
                else if(current_led > cometTailLoc)
                {
                    RGB_SPI_Write(RGB_TO_VAL(0, 0, 0));
                    
                    // Advance comet head/tail locations
                    cometHeadLoc = (cometHeadLoc > 0)? cometHeadLoc-1 : 0;
                    cometTailLoc = ((cometTailLoc-cometHeadLoc) <= tailLen && cometHeadLoc > 0)? \
                                    LEDSTRIPSIZE-1 : (cometTailLoc > 0)? cometTailLoc - 1 : 0;
                                        
                    // Advance frame
                    current_frame++;
                    isframedone = true;
                    current_led = 0;
                    return;
                }
                else
                {
                    // Do nothing. This condition should not be reached.
                    return;
                }
            }
            else
            {
                // Frames for reverse flight are done "rendering"
                current_frame = 0;
                isForward = true;
            }
        }
    }
    
    return;
}

void PIXEL_TheaterChase(void)
{
    // Hard-coded values
    uint8_t szLight = 3;
    uint8_t szSpace = 6;
    
    uint16_t combinedSegLen = szLight + szSpace;
    static uint16_t ledOffset = 0;
    
    // Update cp_framecount. NOTE: This shall evenetually be done by TEST_PIXEL_SelectProfile
    //cp_framecount = combinedSegLen;
    
    if(didProfileChange)
    {
        // Reset the static variables
        ledOffset = 0;
        didProfileChange = false;
    }
    
    while(RGB_SPI_IsTxReady())
    {
        if(current_frame < cp_framecount)
        {
            if(current_led < LEDSTRIPSIZE)
            {
                if((current_led + ledOffset) % combinedSegLen < szLight)
                {
                    RGB_SPI_Write(RGB_TO_VAL(0x19,0,0));
                }
                else
                {
                    RGB_SPI_Write((RGB_TO_VAL(0,0,0)));
                }
                
                current_led++;
            }
            else
            {
                // Uncomment the other to reverse direction
                //ledOffset = (ledOffset+1) % combinedSegLen;
                ledOffset = (ledOffset == 0)? combinedSegLen-1 : (ledOffset-1) % combinedSegLen;
                
                // current_led has reached ledpos, which is the last LED to update in the
                // current_frame. Thus, it's time to advance to the next frame and
                // reset current_led to the beginning of the LED array.
                current_frame++;
                current_led = 0;
                isframedone = true;     // Indicate to the task routine that the frame is done
                return;
            }
        }
        else
        {
            current_frame = 0;
        }
    }
    
    return;
}

void PIXEL_OneColorPulse(void)
{
    
}

void PIXEL_SolidColor(void)
{
    
}

void PIXEL_ColorWave(void)
{
    
}

void PIXEL_SoundReactive(void)
{
    
}

void PIXEL_MultiColorPulse(void)
{
    
}

void PIXEL_Sparkles(void)
{
    
}

void PIXEL_RainbowCycle(void)
{
    
}

void PIXEL_AlternatingColors(void)
{
    
}

//void oneColorPulse(void)
//{
//    int16_t i = 0;
//    
//    while(1)
//    {
//        for(i = 0; i < 64; i++)
//        {
//            RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(i,0,0));
//            __delay_ms(10);
//        }
//
//        for(i = 64; i >= 0; i--)
//        {
//            RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(i,0,0));
//            __delay_ms(10);
//        }
//        __delay_ms(100);
//    }
//}
//
//void solidColor(uint24_t rgb)
//{
//    uint16_t i = 0;
//    
//    // Cycle between 7 colors
//    while(1)
//    {
//        switch(i)
//        {
//            case 1:
//                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(0,0,25));
//                break;
//            case 2:
//                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(0,25,0));
//                break;
//            case 3:
//                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(0,25,25));
//                break;
//            case 4:
//                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(25,0,0));
//                break;
//            case 5:
//                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(25,0,25));
//                break;
//            case 6:
//                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(25,25,0));
//                break;
//            case 7:
//                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(25,25,25));
//                break;
//            default:
//                RGB_ALLSetColor(LEDSTRIPSIZE, RGB_TO_VAL(0,0,0));
//                break;
//        }
//        
//        i = (i+1) % 8;
//        i = (i==0)? 1 : i;
//        LEDLATCH();
//        __delay_ms(1000);
//    }
//}
//
//void colorWave(void)
//{
//    
//}
//
//void soundReactive(void)
//{
//    
//}
//
//void multiColorPulse(void)
//{
//    
//}
//
//void sparkles(void)
//{
//    
//}
//
//// Function below was taken from
//// https://github.com/bigjosh/SimpleNeoPixelDemo/blob/master/SimpleNeopixelDemo/SimpleNeopixelDemo.ino
//// which was based on
//// https://github.com/adafruit/Adafruit_NeoPixel/blob/master/examples/strandtest/strandtest.ino
//void rainbowCycle(unsigned char frames , unsigned int frameAdvance, unsigned int pixelAdvance)
//{
//  // Hue is a number between 0 and 3*256 than defines a mix of r->g->b where
//  // hue of 0 = Full red
//  // hue of 128 = 1/2 red and 1/2 green
//  // hue of 256 = Full Green
//  // hue of 384 = 1/2 green and 1/2 blue
//  // ...
//  
//  unsigned int firstPixelHue = 0;     // Color for the first pixel in the string
//  
//  for(unsigned int j=0; j<frames; j++) {                                  
//    
//    unsigned int currentPixelHue = firstPixelHue;
//        
//    for(unsigned int i=0; i< LEDSTRIPSIZE; i++) {
//      
//      if (currentPixelHue>=(3*256)) {                  // Normalize back down incase we incremented and overflowed
//        currentPixelHue -= (3*256);
//      }
//            
//      unsigned char phase = currentPixelHue >> 8;
//      unsigned char step = currentPixelHue & 0x1f;
//      unsigned char nstep = (~step) & 0x1f;
//                 
//      switch (phase) {
//        
//        case 0: 
//          RGB_SetColor(RGB_TO_VAL(nstep,step,0));
//          break;
//          
//        case 1: 
//          RGB_SetColor( RGB_TO_VAL(0 , nstep , step) );
//          break;
//
//        case 2: 
//          RGB_SetColor(  RGB_TO_VAL(step ,0 , nstep) );
//          break;
//          
//      }
//      
//      currentPixelHue+=pixelAdvance;                                      
//      
//                          
//    } 
//    LEDLATCH();
//    __delay_ms(10);
//    
//    firstPixelHue += frameAdvance;
//           
//  }
//}
//
//void alternatingColors(void)
//{
//    
//}

/*******************************************************************************
 End of File
*/

