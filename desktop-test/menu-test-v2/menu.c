//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// menu.c -- Menu class implementation
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: mjneri
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: menu.c
// Description: <Lorem ipsum dolor sit amet>
//
// Revisions:
// Revision 0.01 - File Created (2022-08-27)
// Additional Comments:
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Includes
#include "menu.h"

// Method prototypes
static void Menu_addOption(Menu *self, char *str, void (*handler)(void));
static void Menu_renderMenu(Menu *self);
static void Menu_runOption(Menu *self, int optionN);

// Method declarations
static void Menu_addOption(Menu *self, char *str, void (*handler)(void))
{
    int i = self->numOfOptions;
    
    if(i >= MAXOPTIONS)
    {
        printf("No more slots.\r\n");
        return;
    }

    self->menuTbl[i].optionString = str;
    self->menuTbl[i].optionHandler = handler;

    self->numOfOptions = i + 1;
    return;
}

static void Menu_renderMenu(Menu *self)
{
    printf("\r\n%s\r\n", self->menuTitle);
    printf("Available options: %d\r\n", self->numOfOptions);
    for(int i = 0; i < self->numOfOptions; i++)
    {
        printf("%d. %s\r\n", i+1, self->menuTbl[i].optionString);
    }
    
    return;
}

static void Menu_runOption(Menu *self, int optionN)
{
    // Assumption: options start from '1'
    if(optionN <= 0)
    {
        printf("Invalid option\r\n");
        return;
    }

    if(optionN > self->numOfOptions)
    {
        printf("Invalid option\r\n");
        return;
    }

    // Run the function
    self->menuTbl[optionN-1].optionHandler();
}

// Class constructor/destructor
Menu *Menu_ctor(char *title)
{
    Menu *self = calloc(1, sizeof(Menu));

    // Interfaces
    self->renderMenu = Menu_renderMenu;
    self->runOption = Menu_runOption;
    self->addOption = Menu_addOption;

    // Attributes
    self->numOfOptions = 0;
    self->currentOption = 0;
    self->menuTitle = title;

    memset(self->menuTbl, 0, sizeof(self->menuTbl));

    return self;
}

void Menu_dtor(Menu *self)
{
    free(self);
}

/******************************************
 * End of File
*/