//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// menu.c --
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: mjneri
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: menu.c
// Description: State-machine implementation of a menu
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

#include "menu.h"

// *****************************************************************************
// *****************************************************************************
// Section: Variables
// *****************************************************************************
// *****************************************************************************

// Menu-related variables
MENU_STATE menuState;

uint8_t itemSelected = 0;       // Determines which menu item is selected. 0 means no item is selected
uint8_t numOfOptions = 0;       // Determines how many options are available per menu.
uint64_t menu_t0 = 0;           // Local counter derived from millis
char menuString[64];            // Local buffer storing string to be displayed
bool isRenderCalled = false;    // Set when GFX_Render is called. Cleared once enough time elapses

// User-input-related variables
uint16_t rotaryVel = 0;
ROT_DIR rotaryDir;

// *****************************************************************************
// *****************************************************************************
// Section: Local Function Defines
// *****************************************************************************
// *****************************************************************************

static void MENU_GetUserInput(void)
{
    // Update itemSelected based on user input
    rotaryVel = ROTENC_Velocity();
    if(rotaryVel > 0)
    {
        rotaryDir = ROTENC_ReadRingBuf();
        if(rotaryDir != ROTENC_ERR)
        {
            if(rotaryDir == ROTENC_CW)
            {
                itemSelected = ((itemSelected) % numOfOptions) + 1;
            }
            else
            {
                itemSelected -= 1;
                itemSelected = (itemSelected >= 1)? itemSelected : numOfOptions;
            }
        }
    }
}

static void MENU_Display_MainMenu(void)
{
    if(millis() - menu_t0 >= 100)
    {
        isRenderCalled = false;
        menu_t0 = millis();
    }

    if(!isRenderCalled)
    {
        isRenderCalled = true;

        sprintf(menuString, "WS2812B Controller");
        GFX_Text(0, 8, menuString, &font5x7, 0);

        sprintf(menuString, "1. Select LED Profile");
        GFX_Text(16, 0, menuString, &font5x7, itemSelected==1);

        sprintf(menuString, "2. Adjust Brightness");
        GFX_Text(32, 0, menuString, &font5x7, itemSelected==2);

        sprintf(menuString, "3. About");
        GFX_Text(48, 0, menuString, &font5x7, itemSelected==3);

        GFX_Render();       
    }
    
    return;
}

static void MENU_Display_AboutMenu(void)
{
    if(millis() - menu_t0 >= 100)
    {
        isRenderCalled = false;
        menu_t0 = millis();
    }

    if(!isRenderCalled)
    {
        isRenderCalled = true;

        sprintf(menuString, "About");
        GFX_Text(0, 40, menuString, &font5x7, 0);

        sprintf(menuString, "Project by MJ Neri");
        GFX_Text(16, 0, menuString, &font5x7, 0);

        sprintf(menuString, "Source code @:");
        GFX_Text(24, 0, menuString, &font5x7, 0);

        sprintf(menuString, "github.com/mjneri/");
        GFX_Text(32, 0, menuString, &font5x7, 0);

        sprintf(menuString, "ws2812-controller");
        GFX_Text(40, 5, menuString, &font5x7, 0);

        sprintf(menuString, "Back");
        GFX_Text(56, 0, menuString, &font5x7, itemSelected==1);

        GFX_Render();
    }
    
    return;
}

static void MENU_Display_LEDProfileSelectMenu(void)
{
    if(millis() - menu_t0 >= 100)
    {
        isRenderCalled = false;
        menu_t0 = millis();
    }

    if(!isRenderCalled)
    {
        isRenderCalled = true;

        GFX_Clear();
    
        sprintf(menuString, "Select LED Profile");
        GFX_Text(0, 8, menuString, &font5x7, 0);

        if(itemSelected <= 5)
        {
            sprintf(menuString, "1. Walking Pixel");
            GFX_Text(8, 0, menuString, &font5x7, itemSelected==1);

            sprintf(menuString, "2. Comet's Tail");
            GFX_Text(16, 0, menuString, &font5x7, itemSelected==2);

            sprintf(menuString, "3. Theater Chase");
            GFX_Text(24, 0, menuString, &font5x7, itemSelected==3);

            sprintf(menuString, "4. 1 Color Pulse");
            GFX_Text(32, 0, menuString, &font5x7, itemSelected==4);

            sprintf(menuString, "5. Solid Color");
            GFX_Text(40, 0, menuString, &font5x7, itemSelected==5);

            sprintf(menuString, "+ more below---");
            GFX_Text(48, 0, menuString, &font5x7, 0);
        }
        else
        {
            sprintf(menuString, "6. Color Wave");
            GFX_Text(8, 0, menuString, &font5x7, itemSelected==6);

            sprintf(menuString, "7. Sound Reactive");
            GFX_Text(16, 0, menuString, &font5x7, itemSelected==7);

            sprintf(menuString, "8. Multicolor Pulse");
            GFX_Text(24, 0, menuString, &font5x7, itemSelected==8);

            sprintf(menuString, "9. Sparkles");
            GFX_Text(32, 0, menuString, &font5x7, itemSelected==9);

            sprintf(menuString, "10. Rainbow Cycle");
            GFX_Text(40, 0, menuString, &font5x7, itemSelected==10);

            sprintf(menuString, "11. Alternating Colors");
            GFX_Text(48, 0, menuString, &font5x7, itemSelected==11);
        }

        sprintf(menuString, "Back");
        GFX_Text(56, 0, menuString, &font5x7, itemSelected==12);

        GFX_Render();
    }
    
    return;
}

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

void MENU_Initialize(void)
{
    menuState = MENU_INIT;
}

void MENU_Tasks(void)
{
    switch(menuState)
    {
        case MENU_INIT:
        {
            // Initialize variables
            itemSelected = 0;
            numOfOptions = 0;
            menu_t0 = millis();
            memset(menuString, 0, sizeof(menuString));
            isRenderCalled = false;
            
            menuState = MENU_WELCOME;
            break;
        }
        
        case MENU_WELCOME:
        {
            // Display a welcome splash screen for 2s
            if(!isRenderCalled)
            {
                GFX_Draw(nailandgear);
                GFX_Render();
                isRenderCalled = true;
            }
            
            if(millis() - menu_t0 >= 2000)
            {
                // Reset the flag, clear the fbuf, re-anchor menu_t0, then move to the next state
                isRenderCalled = false;
                GFX_Clear();
                menu_t0 = millis();
                
                // Update numOfOptions before transitioning to the next state
                numOfOptions = NUMOPT_MAIN_MENU;
                menuState = MENU_MAIN;
            }
            break;
        }
        
        case MENU_MAIN:
        {
            // Display the main menu, updated every 100ms.
            MENU_Display_MainMenu();
            
            // Update itemSelected based on user input
            MENU_GetUserInput();
            
            // Change state on button press based on currently selected item
            if(get_button_pressed())
            {
                if(itemSelected == 1)
                {
                    // Select LED Profile
                    // Reset the flag, clear the fbuf, re-anchor menu_t0, then move to the next state
                    isRenderCalled = false;
                    GFX_Clear();
                    menu_t0 = millis();
                    itemSelected = 0;

                    // Update numOfOptions before transitioning to the next state
                    numOfOptions = NUMOPT_PRFSEL_MENU;
                    menuState = MENU_PROFILE_SELECT;
                }
                else if(itemSelected == 2)
                {
                    // Adjust brightness
                }
                else if(itemSelected == 3)
                {
                    // About page
                    // Reset the flag, clear the fbuf, re-anchor menu_t0, then move to the next state
                    isRenderCalled = false;
                    GFX_Clear();
                    menu_t0 = millis();
                    itemSelected = 0;

                    // Update numOfOptions before transitioning to the next state
                    numOfOptions = NUMOPT_ABOUT;
                    menuState = MENU_ABOUT;
                }
                else
                {
                    // Do nothing
                }
            }
            
            break;
        }
        
        case MENU_PROFILE_SELECT:
        {
            // Display the menu, updated every 100ms.
            MENU_Display_LEDProfileSelectMenu();
            
            // Update itemSelected based on user input
            MENU_GetUserInput();
            
            // Change state on button press based on currently selected item
            if(get_button_pressed())
            {
                if(itemSelected == 12)
                {
                    // Go back
                    // Reset the flag, clear the fbuf, re-anchor menu_t0, then move to the next state
                    isRenderCalled = false;
                    GFX_Clear();
                    menu_t0 = millis();
                    itemSelected = 0;

                    // Update numOfOptions before transitioning to the next state
                    numOfOptions = NUMOPT_MAIN_MENU;
                    menuState = MENU_MAIN;
                }
                else if(itemSelected == 1)
                {
                    // Walking Pixel
                    PIXELS_SelectProfile(PIXEL_PROFILE_INDEX_WALK);
                }
                else if(itemSelected == 2)
                {
                    // Comet's Tail
                    PIXELS_SelectProfile(PIXEL_PROFILE_INDEX_COMETSTAIL);
                }
                else
                {
                    // Do nothing
                }
            }
            break;
        }
        
        case MENU_ABOUT:
        {
            // Display the main menu, updated every 100ms.
            MENU_Display_AboutMenu();
            
            // Update itemSelected based on user input
            MENU_GetUserInput();
            
            // Change state on button press based on currently selected item
            if(get_button_pressed())
            {
                if(itemSelected == 1)
                {
                    // Back
                    // Reset the flag, clear the fbuf, re-anchor menu_t0, then move to the next state
                    isRenderCalled = false;
                    GFX_Clear();
                    menu_t0 = millis();
                    itemSelected = 0;

                    // Update numOfOptions before transitioning to the next state
                    numOfOptions = NUMOPT_MAIN_MENU;
                    menuState = MENU_MAIN;
                }
                else
                {
                    // Do nothing
                }
            }
        }
    }
}

/*******************************************************************************
 End of File
*/

