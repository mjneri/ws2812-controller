//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// fonts.h -- Header file for fonts.c
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: mjneri
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: fonts.h
// Description:
//
// Revisions:
// Revision 0.01 - File Created (2021-12-31)
// Additional Comments:
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Header guards
#ifndef _FONTS_H_
#define _FONTS_H_

// Project includes
#include <stdint.h>

// Font Macros
#define FONT_WIDTH_INDEX        1
#define FONT_HEIGHT_INDEX       2
#define FONT_FIRST_ASCII_INDEX  3
#define FONT_FIRST_CHAR_INDEX   4

// Fonts
extern const uint8_t font5x7[];
extern const uint8_t font8x16[];

#endif /*_FONTS_H_*/