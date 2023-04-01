

// Header files
#include <pic18.h>

#include "test.h"

#define LEDLATCH() __delay_ms(1)        // wait for LED to latch

// Globals for testing
uint64_t t0 = 0;
char teststring[64];
uint16_t cw_count, ccw_count;
ROT_DIR rotDirection;
const char *dir_ascii[] = {"CCW", "CW"};

uint16_t rotEnc_InputCounter = 0;       // Counts buffered inputs
uint16_t buf_cw_count = 0, buf_ccw_count = 0;   // Increments based on buffered inputs

int64_t testcounter = 0;        // for testing with velocity function
uint16_t rotVelocity = 0;

// Callback for heartbeat LED
void HeartbeatCallback(void)
{
    LED0_Toggle();
}

// ************************************************************

// Local function prototypes
static void TEST_PIXELSInit(void);
static void TEST_PIXELS(void);
static void TEST_DisplayInit(void);
static void TEST_Display(void);

// TEST CODE FOR USER INPUTS
static void TEST_USERINPUTS(void);

// Velocity testing
static void TEST_VELOCITY(void);

// ************************************************************

// Main Test function definition
void TEST_Function(void)
{
    // Register the heartbeat callback function
    TMR1_SetInterruptHandler(HeartbeatCallback);
    
    // Initialize millis and some variables
    millis_Initialize();
    Button_Initialize();
    ROTENC_Initialize();
    
    // Make sure to clear the software framebuffer after initializing the OLED
    __delay_ms(100);
    OLED_Initialize();
    GFX_Clear();
    
    TEST_PIXELSInit();
    
    TEST_DisplayInit();
    
    while(true)
    {
        // Test code for anything involving the OLED in some way
        TEST_Display();
        
        // Test SPI LED functions
        TEST_PIXELS();
    }
    return;
}

static void TEST_DisplayInit(void)
{
    // To render the bitmap, call OLED_Tasks() until all pages have been updated.
    // Here, we assume that this process is finished after 1sec.
    // We should expect that the ring buffer is empty once all pages have been updated.
    GFX_Draw(nailandgear);
    GFX_Render();
    t0 = millis();
    
    // Keep calling OLED_Tasks until 1s has passed.
    while(millis() - t0 < 1000)
    {
        OLED_Tasks();
    }
    
    // Clear fbuf again so text has a blank canvas to work with.
    GFX_Clear();
    
    // Re-anchor t0
    t0 = millis();
}

static void TEST_Display(void)
{
    Button_Tasks();
    OLED_Tasks();

    // Read buffered inputs
    rotVelocity = ROTENC_Velocity();
    if(rotVelocity > 0)
    {
        rotDirection = ROTENC_ReadRingBuf();
        if(rotDirection != ROTENC_ERR)
        {
            // Increment the buffered input counter.
            rotEnc_InputCounter++;

            if(rotDirection == ROTENC_CW)
            {
                buf_cw_count++;
                testcounter = testcounter + rotVelocity;
            }
            else
            {
                buf_ccw_count++;
                testcounter = testcounter - rotVelocity;
            }
        }
    }

    // Update the screen whenever possible (i.e. fastest refresh rate)
    if(!OLED_IsBusy())
    {
        //TEST_USERINPUTS();
        TEST_VELOCITY();

        GFX_Render();
        t0 = millis();
    }
}


static void TEST_USERINPUTS(void)
{   
    // Count button presses
    sprintf(teststring, "SW Press: %lu", get_button_presscount());
    GFX_Text(0, 0, teststring, &font5x7, 0);

    // Show whether button is press and held
    sprintf(teststring, "Status P: %d H: %d", get_button_pressed(), get_button_held());
    GFX_Text(8, 0, teststring, &font5x7, 0);

    // Display **ACTUAL** rotation count
    ROTENC_GetRotationCount(&cw_count, &ccw_count);
    sprintf(teststring, "CCW: %d CW: %d = %d", ccw_count, cw_count, ccw_count+cw_count);
    GFX_Text(16, 0, teststring, &font5x7, 0);

    // Display buffered rotation direction.
    if(rotDirection != ROTENC_ERR)
    {
        sprintf(teststring, "Rotation: %s   ", dir_ascii[rotDirection]);
        GFX_Text(32, 0, teststring, &font5x7, 0);
    }
    else
    {
        sprintf(teststring, "Rotation: ---------");
        GFX_Text(32, 0, teststring, &font5x7, 0);
    }

    // Display total buffered inputs
    sprintf(teststring, "Buffered: %u", rotEnc_InputCounter);
    GFX_Text(40, 0, teststring, &font5x7, 0);

    // Display buffered CW and CCW inputs
    sprintf(teststring, "BUF CCW: %u CW: %u", buf_ccw_count, buf_cw_count);
    GFX_Text(48, 0, teststring, &font5x7, 0);

    // Display millis() in realtime
    sprintf(teststring, "millis = %llu", millis());
    GFX_Text(56, 0, teststring, &font5x7, 0);
    
    return;
}

static void TEST_VELOCITY(void)
{
    // Count button presses
    sprintf(teststring, "SW Press: %lu", get_button_presscount());
    GFX_Text(0, 0, teststring, &font5x7, 0);

    // Show whether button is press and held
    sprintf(teststring, "Status P: %d H: %d", get_button_pressed(), get_button_held());
    GFX_Text(8, 0, teststring, &font5x7, 0);

    // Print velocity value
    sprintf(teststring, "Vel: %u t/s   ", rotVelocity);
    GFX_Text(16, 0, teststring, &font5x7, 0);
    
    // print testcounter value
    sprintf(teststring, "%lld   ", testcounter);
    GFX_Text(24, 0, teststring, &font5x7, 0);

    // Display total buffered inputs
    sprintf(teststring, "Buffered: %u", rotEnc_InputCounter);
    GFX_Text(40, 0, teststring, &font5x7, 0);

    // Display buffered CW and CCW inputs
    sprintf(teststring, "BUF CCW: %u CW: %u", buf_ccw_count, buf_cw_count);
    GFX_Text(48, 0, teststring, &font5x7, 0);

    // Display millis() in realtime
    sprintf(teststring, "millis = %llu", millis());
    GFX_Text(56, 0, teststring, &font5x7, 0);
    
    return;
}

// ************************************************************
// Functions for testing Non-blocking Pixels
uint64_t rgb_t0 = 0;
static uint16_t cp_framecount = LEDSTRIPSIZE;
static uint16_t current_frame = 0;
static uint16_t current_led = 0;                // Indicates which LED is being updated
static bool isframedone = false;

static void TEST_PIXEL_Walk(void);  // simple animation only
static void TEST_PIXEL_COMETSTAIL(uint8_t tailLen);
static void TEST_PIXEL_THEATERCHASE(uint8_t szLight, uint8_t szSpace);

// Move function below to spi_led.c after testing
static bool RGB_SPI_IsTxReady(void)
{
    // Return true only if there's at least 3 bytes of free space
    return ((SPI1_GetBufferSize() >= 3)? true : false);
}

// Move function below to spi_led.c after testing
// Maybe make this return int instead of void? So app can check return value
static void RGB_SPI_Write(uint24_t val)
{
    if(!RGB_SPI_IsTxReady())
    {
        return;
    }
    
    rgb_led_t rgbData = (rgb_led_t) val;
    
    SPI1_Write(rgbData.green);
    SPI1_Write(rgbData.red);
    SPI1_Write(rgbData.blue);
    
    return;
}

static int TEST_PIXEL_SelectProfile(void)
{
    // Do nothing for now
    /* PSEUDOCODE:
     if(profile is invalid)
    {
        return -1;
    }
    current_profile = profile;
     cp_framecount = frames;
    return 0;
     */
}

static void TEST_RGB_Tasks(void)
{   
    if(!RGB_SPI_IsTxReady())
    {
        return;
    }
    
    if(isframedone)
    {
        if(millis() - rgb_t0 < 25)
        {
            // Update only every 25ms
            return;
        }
    }
        
    isframedone = false;
    
    // Replace this later - see pseudocode in OneNote
    //TEST_PIXEL_Walk();
    //TEST_PIXEL_COMETSTAIL(6);
    TEST_PIXEL_THEATERCHASE(3, 9);
    rgb_t0 = millis();
}

static void TEST_PIXELSInit(void)
{
    // Open SPI port
    SPI1_Open(SPI1_HDX_TXONLY);
    
    // 2022-03-20: Minimal memory footprint test code
    //RGB_Clear(LEDSTRIPSIZE);
    
    rgb_t0 = millis();
}

static void TEST_PIXELS(void)
{   
    // Check this pin with a logic analyzer - frequency of this function being called.
    DEBUG_GPIO_OUT_Toggle();
    
    TEST_RGB_Tasks();
    return;
}

static void TEST_PIXEL_Walk(void)
{
    static uint16_t activeLed = 0;  // determines which LED is turned on.
    
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

static void TEST_PIXEL_COMETSTAIL(uint8_t tailLen)
{
    // Determines where the comet's head and tail are located
    static int16_t cometHeadLoc = 0;
    static int16_t cometTailLoc = 0;
    static bool isForward = true;   // Determines comet flight direction
    
    //uint16_t frames = cp_framecount + tailLen;
    // Update cp_framecount. NOTE: This shall evenetually be done by TEST_PIXEL_SelectProfile
    cp_framecount = LEDSTRIPSIZE + tailLen;
    
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

static void TEST_PIXEL_THEATERCHASE(uint8_t szLight, uint8_t szSpace)
{
    uint16_t combinedSegLen = szLight + szSpace;
    static uint16_t ledOffset = 0;
    
    // Update cp_framecount. NOTE: This shall evenetually be done by TEST_PIXEL_SelectProfile
    cp_framecount = combinedSegLen;
    
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
/**
 End of File
*/