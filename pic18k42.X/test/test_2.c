
#include "test.h"

// Function prototypes
static void OLED_TEST(void);

// Function prototypes of what will eventually be added in gfx.c


// Regular heartbeat callback using Timer1
static void HeartbeatCallback(void)
{
    LED0_Toggle();
}

// Function define
void TEST_2_Function(void)
{
    // Register the heartbeat callback function
    TMR1_SetInterruptHandler(HeartbeatCallback);
    
    millis_Initialize();
    OLED_Initialize();
    
    // OLED Test
    OLED_TEST();
    
    return;
}

static void OLED_TEST(void)
{
    GFX_Clear();
    
    // NOTE: Add this to the GFX Initialization code later.
    // NOTE 2: Or, just call Gfx_clear in the init code like what I did above.
    //memset(Gfx_framebuffer, 0, sizeof(Gfx_framebuffer));
    
    // Test pixel function
//    for(uint16_t i = 0; i < SH1106_PAGES; i++)
//    {
//        for(uint16_t j = 0; j < SH1106_SEGMENTS; j++)
//        {
//            for(uint8_t k = 0; k < 8; k++)
//            {
//                Gfx_pixel((i << 3) + k, j, 1);
//                Gfx_render();
//            }
//        }
//    }

    GFX_Text(5,15, "Hello world", &font5x7, false);
    GFX_Render();
    
    uint64_t t0 = millis();
    uint8_t bitmapSel = 0;
    while(1)
    {
        // Cycle through bitmaps
        if(millis() - t0 >= 2000)
        {
            t0 = millis();      // Update t0 again
            GFX_Clear();
            
            switch(bitmapSel)
            {
                case 0:
                    GFX_Draw(Owl);
                    break;
                case 1:
                    GFX_Draw(pochita);
                    break;
                case 2:
                    GFX_Draw(nailandgear);
                    break;
                default:
                    GFX_Draw(hollowknight);
                    break;
            }
            
            bitmapSel = (bitmapSel+1) % 4;
            GFX_Render();
        }
        
        DEBUG_GPIO_OUT_Toggle();
        OLED_Tasks();
    }
}

// FUNCTION TESTS

/********************************************
 END OF FILE
 **********************************************/