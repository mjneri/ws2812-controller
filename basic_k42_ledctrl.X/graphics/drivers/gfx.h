//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// gfx.h -- Header file for gfx.c
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: mjneri
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: gfx.h
// Description: 
//
// Revisions:
// Revision 0.01 - File Created (2022-04-09)
// Additional Comments:
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Header guard
#ifndef _GFX_H_
#define _GFX_H_

// *****************************************************************************
// *****************************************************************************
// Section: Header Files
// *****************************************************************************
// *****************************************************************************

#include "sh1106.h"
#include "../gui/menu.h"

// *****************************************************************************
// *****************************************************************************
// Section: Constant Defines
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Data type definitions
// *****************************************************************************
// *****************************************************************************

typedef enum
{
    GFX_INIT,
    GFX_WAIT_INPUT,
    GFX_SERVICE_INPUT,
    GFX_DISPLAY_OFF,
    GFX_DISPLAY_WAKEUP,
    GFX_MENU_UPDATE,
    GFX_MENU_CHANGE_SELECT
} GFX_STATES;

typedef struct
{
    // Current menu option selected;
    // isUsrInput;
    // isSleep;
    // Refer to lcdgfx for ideas
} gfx_object_t;

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

void GFX_Initialize(void);
void GFX_DrawString(uint8_t row, uint8_t col, char *str, uint8_t *font, bool invertBG);
void GFX_DrawBitmap(uint8_t *bitmap);


#endif /*_GFX_H_*/

/*******************************************************************************
 End of File
*/
