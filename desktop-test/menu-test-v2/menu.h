//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// menu.h -- Menu class header & defines
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: mjneri
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: menu.h
// Description: <Lorem ipsum dolor sit amet>
//
// Revisions:
// Revision 0.01 - File Created (2022-08-27)
// Additional Comments:
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _MENU_H_
#define _MENU_H_

// Includes
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

// Constants/Macros
#define MAXOPTIONS 10       // Max options in a menu object
#define MAXINSTANCES 10     // Max. allowable instances of the menu class

// Forward declaration
typedef struct menu_t Menu;

// Struct to contain option properties
// **May** be treated as a private attribute in menu.c
typedef struct menu_options_t
{
    char *optionString;         // String to display
    void (*optionHandler)(void);// Function to execute
} Menu_options;

// Class object definition
struct menu_t
{
    // "Public" class attributes -- accessible through an object
    int currentOption;
    int numOfOptions;
    char *menuTitle;
    Menu_options menuTbl[MAXOPTIONS]; // Will need to find out how to make this private and usable by multiple instances... maybe look at H3 code?

    // Class methods
    void (*addOption)(Menu *self, char *str, void (*handler)(void));
    void (*renderMenu)(Menu *self);
    void (*runOption)(Menu *self, int optionN);
};

// Class constructor and destructor defines
Menu *Menu_ctor(char *title);
void Menu_dtor(Menu *self);

#endif /*_MENU_H*/

/******************************************
 * End of File
*/