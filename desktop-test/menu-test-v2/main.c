//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// main.c
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: mjneri
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: main.c
// Description: <Lorem ipsum dolor sit amet>
//
// Revisions:
// Revision 0.01 - File Created (2022-08-27)
// Additional Comments:
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Includes
#include "menu.h"
#include <stdio.h>
#include "vt100.h"

// Private "attributes"
Menu *currentMenu = NULL;
Menu *prevMenu = NULL;
Menu *nextMenu = NULL;

// Globals
Menu *homeMenu = NULL;
Menu *ledMenu = NULL;
Menu *brightMenu = NULL;
Menu *sizeMenu = NULL;
Menu *aboutMenu = NULL;

bool quit = false;

// Function prototypes
void Home_LED_Profile(void);
void Home_Brightness_Adjustment(void);
void Home_LED_Array_Size(void);
void Home_About(void);

// Other menus
void LED_CometsTail(void);
void Bright_Adjustment(void);
void Arrsz_Adjust(void);

// Main function
int main(void)
{
    // Initialize home menu
    homeMenu = Menu_ctor("Home Menu");
    homeMenu->addOption(homeMenu, "1. Select LED Profile", Home_LED_Profile);
    homeMenu->addOption(homeMenu, "2. Brightness Adjustment", Home_Brightness_Adjustment);
    homeMenu->addOption(homeMenu, "3. Adjust LED Array Size", Home_LED_Array_Size);
    homeMenu->addOption(homeMenu, "4. About", Home_About);

    // Initialize submenus;
    ledMenu = Menu_ctor("Select LED Profile");
    ledMenu->addOption(ledMenu, "Comet's Tail", LED_CometsTail);

    brightMenu = Menu_ctor("Brightness Adjustment");
    brightMenu->addOption(brightMenu, "[0 - 255]: ", Bright_Adjustment);

    arrszMenu = Menu_ctor("Adjust LED Array Size");
    arrszMenu->addOption(arrszMenu, "0 - 4095", Arrsz_Adjust);
    

    // Select the current menu
    currentMenu = homeMenu;

    // Test out the menu
    int userInput;
    printf(_clrscreen);
    while(!quit)
    {
        currentMenu->renderMenu(currentMenu);
        printf("\nChoose an option: ");
        scanf("%d", &userInput);
        currentMenu->runOption(currentMenu, userInput);
    }

    Menu_dtor(homeMenu);
    Menu_dtor(ledMenu);
    Menu_dtor(brightMenu);
    Menu_dtor(arrszMenu);
    return 0;
}

// Function defines
void Home_LED_Profile(void)
{
    printf("Select LED profile here.\n");
}

void Home_Brightness_Adjustment(void)
{
    printf("Adjust brightness...\n");
}

void Home_LED_Array_Size(void)
{
    printf("LED array size...\n");
}

void Home_About(void)
{
    printf("This is an about...\n");
    // Figure out how to keep this menu rendered until the user presses enter...
    // For now, let's quit.
    quit = 1;
}



/******************************************
 * End of File
*/