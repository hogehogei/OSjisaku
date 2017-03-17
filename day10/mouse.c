#include "mouse.h"
#include "keyboard.h"
#include "graphic.h"
#include "global.h"

void enable_mouse( MouseDecode* mdec )
{
    static uint8_t mouse_buf[256];
    wait_KBC_sendready();
    _io_out8( PORT_KEYCMD, KEYCMD_SENDTO_MOUSE );
    wait_KBC_sendready();
    _io_out8( PORT_KEYDAT, MOUSECMD_ENABLE );

    fifo8_init( &gMouseFIFO, mouse_buf, sizeof(mouse_buf) );
    // 成功すると ACK(0xFA) が送信されてくる
    
    mdec->phase = 0;
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
}

int mouse_decode( MouseDecode* mdec, uint8_t raw_data )
{
    if( mdec->phase == 0 ){
        // raw_data の 0xFA を待っている段階
        if( raw_data == 0xFA ){
            mdec->phase = 1;
        }
        return 0;
    }
    if( mdec->phase == 1 ){
        // 1バイト目を待っている段階
        if( (raw_data & 0xc8) == 0x08 ){
            // 正しい1バイト目だった
            mdec->buf[0] = raw_data;
            mdec->phase = 2;
        }
        else {
            // 正しくない1バイト目の場合はエラーを返す
            return -1;
        }

        return 0;
    }
    if( mdec->phase == 2 ){
        // 2バイト目を待っている段階
        mdec->buf[1] = raw_data;
        mdec->phase = 3;
        return 0;
    }
    if( mdec->phase == 3 ){
        // 3バイト目を待っている段階
        mdec->buf[2] = raw_data;
        mdec->phase = 1;

        mdec->btn = mdec->buf[0] & 0x07;
        mdec->x = mdec->buf[1];
        mdec->y = mdec->buf[2];
        if( (mdec->buf[0] & 0x10) ){
            mdec->x |= 0xFFFFFF00;
        }
        if( (mdec->buf[0] & 0x20) ){
            mdec->y |= 0xFFFFFF00; 
        }
        mdec->y = - mdec->y;    // マウスではy方向の符号が画面と逆

        return 1;
    }

    // ここに来ることはないはず
    return -1;
}
