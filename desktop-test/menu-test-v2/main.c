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
void Menu_LED_Profile(void);
void Menu_Brightness_Adjustment(void);
void Menu_LED_Array_Size(void);
void Menu_About(void);

// Main function
int main(void)
{
    // Initialize home menu
    homeMenu = Menu_ctor("Home Menu");
    homeMenu->addOption(homeMenu, "1. Select LED Profile", Menu_LED_Profile);
    homeMenu->addOption(homeMenu, "2. Brightness Adjustment", Menu_Brightness_Adjustment);
    homeMenu->addOption(homeMenu, "3. Adjust LED Array Size", Menu_LED_Array_Size);
    homeMenu->addOption(homeMenu, "4. About", Menu_About);

    // Initialize submenus;


    // Select the current menu
    currentMenu = homeMenu;

    // Test out the menu
    int userInput;
    while(!quit)
    {
        currentMenu->renderMenu(currentMenu);
        printf("\nChoose an option: ");
        scanf("%d", &userInput);
        currentMenu->runOption(currentMenu, userInput);
    }

    Menu_dtor(homeMenu);
    return 0;
}

// Function defines
void Menu_LED_Profile(void)
{
    printf("Select LED profile here.\n");
}

void Menu_Brightness_Adjustment(void)
{
    printf("Adjust brightness...\n");
}

void Menu_LED_Array_Size(void)
{
    printf("LED array size...\n");
}

void Menu_About(void)
{
    printf("This is an about...\n");
    // Figure out how to keep this menu rendered until the user presses enter...
    // For now, let's quit.
    quit = 1;
}



/******************************************
 * End of File
*/