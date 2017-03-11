#ifndef    GRAPHIC_H_INCLUDED
#define    GRAPHIC_H_INCLUDED

#include "type.h" 
#include "misc.h"

enum VGAPalletColor    // 0 - 15 16色
{
    COLOR_BLACK = 0, 
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_PURPLE,
    COLOR_LIGHT_BLUE,
    COLOR_WHITE,
    COLOR_LIGHT_GRAY,
    COLOR_DARK_RED,
    COLOR_DARK_GREEN,
    COLOR_DARK_YELLOW,
    COLOR_DARK_BLUE,
    COLOR_DARK_PURPLE,
    COLOR_DEEP_BLUE,
    COLOR_DARK_GRAY
};

void init_pallet(void);
void boxfill8( int x0, int y0, int x1, int y1, uint8_t color );
void putfont8( int x0, int y0, uint8_t color, uint8_t c );
Point putstr8( int x0, int y0, uint8_t color, const uint8_t* s );

void draw_cursor( int x0, int y0, const uint8_t* cursor );

#endif
