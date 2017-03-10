#include "mouse.h"
#include "keyboard.h"
#include "graphic.h"
#include "global.h"

void init_mouse(void)
{
    static uint8_t mouse_buf[256];
    wait_KBC_sendready();
    _io_out8( PORT_KEYCMD, KEYCMD_SENDTO_MOUSE );
    wait_KBC_sendready();
    _io_out8( PORT_KEYDAT, MOUSECMD_ENABLE );

    fifo8_init( &gMouseFIFO, mouse_buf, sizeof(mouse_buf) );
    // 成功すると ACK(0xFA) が送信されてくる
}

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
