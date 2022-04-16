

#ifndef _MENU_H_
#define _MENU_H_

// Includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// Main reference for OOP:
// https://github.com/QuantumLeaps/OOP-in-C

// Forward declaration
struct menu_object_intf;

// Menu object definition
// Maximum of 8 options per menu. Uses OOP
typedef struct menu_object_t
{
    // Attributes
    char *self;
    struct menu_object_t *optionObj[8];
    struct menu_object_t *prevOption;

    // Class Methods
    struct menu_object_intf const *intf;

} menu_object_t;

typedef struct menu_object_intf
{
    void (*selfHandler)(void);
    void (*displayOptions)(void);

} menu_object_intf;

void test(void);
void menuDisplay(void);

// Class constructor
void menuCtor(menu_object_t * const me, char *self, 
                menu_object_t *optionObj[], menu_object_t *prevOption,
                menu_object_intf const *intf);

// General class methods
void _printMenuOptions(void);

// Handlers
void mainMenuHandler(void);
void aboutHandler(void);

#endif /*_MENU_H_*/