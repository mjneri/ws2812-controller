

#ifndef _MENU_H_
#define _MENU_H_

// Includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// Menu object definition
// Maximum of 8 options per menu. Uses linked lists
typedef struct menu_object_t
{
    char *self;
    void (*selfHandler)(void);
    struct menu_object_t *selfObj;

    char *option1;
    struct menu_object_t *option1Obj;

    char *option2;
    struct menu_object_t *option2Obj;

    char *option3;
    struct menu_object_t *option3Obj;

    char *option4;
    struct menu_object_t *option4Obj;

    char *option5;
    struct menu_object_t *option5Obj;

    char *option6;
    struct menu_object_t *option6Obj;

    char *option7;
    struct menu_object_t *option7Obj;

    char *option8;
    struct menu_object_t *option8Obj;

    struct menu_object_t *prevOption;
} menu_object_t;

void test(void);

#endif /*_MENU_H_*/