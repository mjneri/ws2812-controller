

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

    char *option2;
    void (*opt2Handler)(void);

    char *option3;
    void (*opt3Handler)(void);

    char *option4;
    void (*opt4Handler)(void);

    char *option5;
    void (*opt5Handler)(void);

    char *option6;
    void (*opt6Handler)(void);

    char *option7;
    void (*opt7Handler)(void);

    char *option8;
    void (*opt8Handler)(void);

    struct menu_object_t *prevOption;
    struct menu_object_t *nextOption;
} menu_object_t;

void test(void);

#endif /*_MENU_H_*/