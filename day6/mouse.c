#include "mouse.h"
#include "graphic.h"

void create_mouse_cursor_bitmap( uint8_t* cursor_dst, uint8_t bg_color )
{
    static char cursor[16][16] = {
        "**************..",
        "*OOOOOOOOOOO*...",
        "*OOOOOOOOOO*....",
        "*OOOOOOOOO*.....",
        "*OOOOOOOO*......",
        "*OOOOOOO*.......",
        "*OOOOOOO*.......",
        "*OOOOOOOO*......",
        "*OOOO**OOO*.....",
        "*OOO*..*OOO*....",
        "*OO*....*OOO*...",
        "*O*......*OOO*..",
        "**........*OOO*.",
        "*..........*OOO*",
        "............*OO*",
        ".............***",
    };

    int x, y;
    char c;
    uint8_t* d = cursor_dst;

    for( y = 0; y < 16; ++y ){
        for( x = 0; x < 16; ++x ){
            c = cursor[y][x];
            if( c == '*' ){
                d[y * 16 + x] = COLOR_BLACK;
            }
            if( c == 'O' ){
                d[y * 16 + x] = COLOR_WHITE;
            }
            if( c == '.' ){
                d[y * 16 + x] = bg_color;
            }
        }
    }

    return;
}
