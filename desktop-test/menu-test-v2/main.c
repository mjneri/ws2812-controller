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
Menu *myMenu = NULL;
Menu *yourMenu = NULL;
bool quit = false;

// Function prototypes
void foo1(void);
void foo2(void);
void foo3(void);

void chon1(void);
void chon2(void);

// Main function
int main(void)
{
    // Initialize the first menu
    myMenu = Menu_ctor("myMenu");

    myMenu->addOption(myMenu, "Foo1", foo1);
    myMenu->addOption(myMenu, "Foo2", foo2);
    myMenu->addOption(myMenu, "Foo3 next menu", foo3);

    // Initialize the second menu
    yourMenu = Menu_ctor("yourMenu");
    yourMenu->addOption(yourMenu, "Chon1 back", chon1);
    yourMenu->addOption(yourMenu, "Chon2 quit", chon2);

    // Select the current menu
    currentMenu = myMenu;
    nextMenu = yourMenu;

    // Test out the menu
    int userInput;
    while(!quit)
    {
        currentMenu->renderMenu(currentMenu);
        printf("\nChoose an option: ");
        scanf("%d", &userInput);
        currentMenu->runOption(currentMenu, userInput);
    }

    Menu_dtor(myMenu);
    Menu_dtor(yourMenu);
    return 0;
}

// Function defines
void foo1(void)
{
    printf("You picked the wrong house foo!\r\n");
}

void foo2(void)
{
    printf("Ayo, it's me Carl! chill! chill!\r\n");
}

void foo3(void)
{
    // Shift menu
    prevMenu = currentMenu;
    currentMenu = nextMenu;
    printf("Going to menu: %s\n", currentMenu->menuTitle);
}

void chon1(void)
{
    // Go back
    nextMenu = currentMenu;
    currentMenu = prevMenu;
    printf("Returned to menu: %s\n", currentMenu->menuTitle);
}

void chon2(void)
{
    // quit
    printf("Quitting...\n");
    quit = true;
}


/******************************************
 * End of File
*/