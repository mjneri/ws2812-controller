

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
    char *option1;
    void (*opt1Handler)(void);
    struct menu_object_t *option1Obj;

    char *option2;
    void (*opt2Handler)(void);
    struct menu_object_t *option2Obj;

    char *option3;
    void (*opt3Handler)(void);
    struct menu_object_t *option3Obj;

    char *option4;
    void (*opt4Handler)(void);
    struct menu_object_t *option4Obj;

    char *option5;
    void (*opt5Handler)(void);
    struct menu_object_t *option5Obj;

    char *option6;
    void (*opt6Handler)(void);
    struct menu_object_t *option6Obj;

    char *option7;
    void (*opt7Handler)(void);
    struct menu_object_t *option7Obj;

    char *option8;
    void (*opt8Handler)(void);
    struct menu_object_t *option8Obj;

    struct menu_object_t *prevOption;
} menu_object_t;

void test(void);

#endif /*_MENU_H_*/