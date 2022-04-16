#include "menu.h"

// Declare globals
menu_object_t aboutObj =
{
    .self = "About",
    .selfHandler = &aboutHandler,

    .prevOption = &mainMenuObj
};

menu_object_t mainMenuObj =
{
    .self = "Main Menu",
    .selfHandler = &mainMenuHandler,

    .option1 = "About",
    .option1Obj = &aboutObj,

    .prevOption = NULL
};

void test(void)
{
    printf("Hello world\r\n");
    return;
}

void mainMenuHandler(void)
{
    // Print options
    printf("%s\r\n", mainMenuObj.self);
    
}

void aboutHandler(void)
{
    printf("%s\r\n", aboutObj.self);

    printf("Lorem ipsum dolor sit amet consectetum\r\n");
}